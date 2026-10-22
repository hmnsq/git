#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blob.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "delta.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "csum-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mem-pool.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "khash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "date.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gpg-interface.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PACK_ID_ਬਿੱਟ 16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_PACK_ID ((1<<PACK_ID_ਬਿੱਟ)-1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਡੂੰਘਾਈ_ਬਿੱਟ 13
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_ਡੂੰਘਾਈ ((1<<ਡੂੰਘਾਈ_ਬਿੱਟ)-1)

/*
 * We abuse the setuid bit on directories to mean "do not delta".
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_DELTA S_ISUID

/*
 * The amount of additional space required in order to write an object into the
 * current pack. This is the hash lengths at the end of the pack, plus the
 * length of one object ID.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PACK_ਮਾਪ_ਹੱਦ (the_hash_algo->rawsz * 3)

ਬਣਤਰ object_ਇੰਦਰਾਜ {
	ਬਣਤਰ pack_idx_ਇੰਦਰਾਜ idx;
	ਬਣਤਰ hashmap_ਇੰਦਰਾਜ ent;
	uint32_t type : TYPE_ਬਿੱਟ,
		pack_id : PACK_ID_ਬਿੱਟ,
		ਡੂੰਘਾਈ : ਡੂੰਘਾਈ_ਬਿੱਟ;
};

ਸਥਿਰ ਪੂਰਨ object_ਇੰਦਰਾਜ_hashcmp(ਅਚਲ ਖਾਲੀ *map_ਅੰਕੜੇ ਅਣਵਰਤਿਆ,
				ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *eptr,
				ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ_or_ਕੁੰਜੀ,
				ਅਚਲ ਖਾਲੀ *keydata)
{
	ਅਚਲ ਬਣਤਰ object_id *oid = keydata;
	ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ *e1, *e2;

	e1 = container_ਦਾ(eptr, ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ, ent);
	ਜੇ (oid)
		ਵਾਪਸ oidcmp(&e1->idx.oid, oid);

	e2 = container_ਦਾ(ਇੰਦਰਾਜ_or_ਕੁੰਜੀ, ਅਚਲ ਬਣਤਰ object_ਇੰਦਰਾਜ, ent);
	ਵਾਪਸ oidcmp(&e1->idx.oid, &e2->idx.oid);
}

ਬਣਤਰ object_ਇੰਦਰਾਜ_pool {
	ਬਣਤਰ object_ਇੰਦਰਾਜ_pool *ਅਗਲਾ_pool;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *ਅਗਲਾ_free;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *ਸਿਰਾ;
	ਬਣਤਰ object_ਇੰਦਰਾਜ entries[FLEX_ARRAY]; /*ਹੋਰ*/
};

ਬਣਤਰ mark_ਸੈੱਟ {
	ਸੰਘ {
		ਬਣਤਰ object_id *oids[1024];
		ਬਣਤਰ object_ਇੰਦਰਾਜ *marked[1024];
		ਬਣਤਰ mark_ਸੈੱਟ *sets[1024];
	} ਅੰਕੜੇ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਿਸਕਾਓ;
};

ਬਣਤਰ ਆਖਰੀ_object {
	ਬਣਤਰ strbuf ਅੰਕੜੇ;
	off_t ਵਿੱਥ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਡੂੰਘਾਈ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ no_ਅਦਲਾ_ਬਦਲੀ : 1;
};

ਬਣਤਰ atom_str {
	ਬਣਤਰ atom_str *ਅਗਲਾ_atom;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ str_len;
	ਅੱਖਰ str_dat[FLEX_ARRAY]; /*ਹੋਰ*/
};

ਬਣਤਰ ਰੁੱਖ_content;
ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ {
	ਬਣਤਰ ਰੁੱਖ_content *ਰੁੱਖ;
	ਬਣਤਰ atom_str *name;
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ_ms {
		uint16_t ਬਹੁਲਕ;
		ਬਣਤਰ object_id oid;
	} versions[2];
};

ਬਣਤਰ ਰੁੱਖ_content {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਇੰਦਰਾਜ_ਸਮਰੱਥਾ; /* must match avail_tree_content */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਇੰਦਰਾਜ_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ delta_ਡੂੰਘਾਈ;
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *entries[FLEX_ARRAY]; /*ਹੋਰ*/
};

ਬਣਤਰ avail_ਰੁੱਖ_content {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਇੰਦਰਾਜ_ਸਮਰੱਥਾ; /* must match tree_content */
	ਬਣਤਰ avail_ਰੁੱਖ_content *ਅਗਲਾ_avail;
};

ਬਣਤਰ branch {
	ਬਣਤਰ branch *ਸਾਰਣੀ_ਅਗਲਾ_branch;
	ਬਣਤਰ branch *ਸਰਗਰਮ_ਅਗਲਾ_branch;
	ਅਚਲ ਅੱਖਰ *name;
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ branch_ਰੁੱਖ;
	uintmax_t ਆਖਰੀ_commit;
	uintmax_t ਅੰਕ_notes;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਰਗਰਮ : 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ delete : 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ pack_id : PACK_ID_ਬਿੱਟ;
	ਬਣਤਰ object_id oid;
};

ਬਣਤਰ ਟੈਗ {
	ਬਣਤਰ ਟੈਗ *ਅਗਲਾ_ਟੈਗ;
	ਅਚਲ ਅੱਖਰ *name;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pack_id;
	ਬਣਤਰ object_id oid;
};

ਬਣਤਰ hash_list {
	ਬਣਤਰ hash_list *ਅਗਲਾ;
	ਬਣਤਰ object_id oid;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

ਬਣਤਰ recent_command {
	ਬਣਤਰ recent_command *prev;
	ਬਣਤਰ recent_command *ਅਗਲਾ;
	ਅੱਖਰ *buf;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*mark_ਸੈੱਟ_inserter_t)(ਬਣਤਰ mark_ਸੈੱਟ **s, ਬਣਤਰ object_id *oid, uintmax_t mark);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*each_mark_fn_t)(uintmax_t mark, ਖਾਲੀ *obj, ਖਾਲੀ *cbp);

/* Configured limits on output */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_ਡੂੰਘਾਈ = 50;
ਸਥਿਰ off_t max_packsize;
ਸਥਿਰ ਪੂਰਨ unpack_ਹੱਦਬੰਦੀ = 100;
ਸਥਿਰ ਪੂਰਨ force_update;

/* Stats and misc. counters */
ਸਥਿਰ uintmax_t alloc_ਗਿਣਤੀ;
ਸਥਿਰ uintmax_t marks_ਸੈੱਟ_ਗਿਣਤੀ;
ਸਥਿਰ uintmax_t object_ਗਿਣਤੀ_by_type[1 << TYPE_ਬਿੱਟ];
ਸਥਿਰ uintmax_t duplicate_ਗਿਣਤੀ_by_type[1 << TYPE_ਬਿੱਟ];
ਸਥਿਰ uintmax_t delta_ਗਿਣਤੀ_by_type[1 << TYPE_ਬਿੱਟ];
ਸਥਿਰ uintmax_t delta_ਗਿਣਤੀ_attempts_by_type[1 << TYPE_ਬਿੱਟ];
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ object_ਗਿਣਤੀ;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ branch_ਗਿਣਤੀ;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ branch_ਲੱਦੋ_ਗਿਣਤੀ;
ਸਥਿਰ ਪੂਰਨ failure;
ਸਥਿਰ FILE *pack_ਕਿਨਾਰੇ;
ਸਥਿਰ ਪੂਰਨ show_stats = 1;
ਸਥਿਰ ਪੂਰਨ quiet;
ਸਥਿਰ ਗਿਣਤੀ sign_ਬਹੁਲਕ signed_ਟੈਗ_ਬਹੁਲਕ = SIGN_VERBATIM;
ਸਥਿਰ ਗਿਣਤੀ sign_ਬਹੁਲਕ signed_commit_ਬਹੁਲਕ = SIGN_VERBATIM;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *signed_commit_keyid;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *signed_ਟੈਗ_keyid;

/* Memory pools */
ਸਥਿਰ ਬਣਤਰ mem_pool fi_mem_pool = {
	.ਖੰਡ_alloc = 2*1024*1024 - ਆਕਾਰ(ਬਣਤਰ mp_ਖੰਡ),
};

/* Atom management */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ atom_ਸਾਰਣੀ_sz = 4451;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ atom_cnt;
ਸਥਿਰ ਬਣਤਰ atom_str **atom_ਸਾਰਣੀ;

/* The .pack file being generated */
ਸਥਿਰ ਬਣਤਰ pack_idx_option pack_idx_opts;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pack_id;
ਸਥਿਰ ਬਣਤਰ hashfile *pack_file;
ਸਥਿਰ ਬਣਤਰ packed_git *pack_ਅੰਕੜੇ;
ਸਥਿਰ ਬਣਤਰ packed_git **all_packs;
ਸਥਿਰ off_t pack_ਮਾਪ;

/* Table of objects we've written. */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ object_ਇੰਦਰਾਜ_alloc = 5000;
ਸਥਿਰ ਬਣਤਰ object_ਇੰਦਰਾਜ_pool *ਖੰਡਾਂ;
ਸਥਿਰ ਬਣਤਰ hashmap object_ਸਾਰਣੀ;
ਸਥਿਰ ਬਣਤਰ mark_ਸੈੱਟ *marks;
ਸਥਿਰ ਅੱਖਰ *export_marks_file;
ਸਥਿਰ ਅੱਖਰ *import_marks_file;
ਸਥਿਰ ਪੂਰਨ import_marks_file_from_ਧਾਰਾ;
ਸਥਿਰ ਪੂਰਨ import_marks_file_ignore_missing;
ਸਥਿਰ ਪੂਰਨ import_marks_file_ਪੂਰਾ;
ਸਥਿਰ ਪੂਰਨ relative_marks_paths;

/* Our last blob */
ਸਥਿਰ ਬਣਤਰ ਆਖਰੀ_object ਆਖਰੀ_blob = {
	.ਅੰਕੜੇ = STRBUF_INIT,
 };

/* Tree management */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਰੁੱਖ_ਇੰਦਰਾਜ_alloc = 1000;
ਸਥਿਰ ਖਾਲੀ *avail_ਰੁੱਖ_ਇੰਦਰਾਜ;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ avail_ਰੁੱਖ_ਸਾਰਣੀ_sz = 100;
ਸਥਿਰ ਬਣਤਰ avail_ਰੁੱਖ_content **avail_ਰੁੱਖ_ਸਾਰਣੀ;
ਸਥਿਰ ਮਾਪ_t ਰੁੱਖ_ਇੰਦਰਾਜ_allocd;
ਸਥਿਰ ਬਣਤਰ strbuf old_ਰੁੱਖ = STRBUF_INIT;
ਸਥਿਰ ਬਣਤਰ strbuf new_ਰੁੱਖ = STRBUF_INIT;

/* Branch data */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_ਸਰਗਰਮ_branches = 5;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cur_ਸਰਗਰਮ_branches;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ branch_ਸਾਰਣੀ_sz = 1039;
ਸਥਿਰ ਬਣਤਰ branch **branch_ਸਾਰਣੀ;
ਸਥਿਰ ਬਣਤਰ branch *ਸਰਗਰਮ_branches;

/* Tag data */
ਸਥਿਰ ਬਣਤਰ ਟੈਗ *ਪਹਿਲਾ_ਟੈਗ;
ਸਥਿਰ ਬਣਤਰ ਟੈਗ *ਆਖਰੀ_ਟੈਗ;

/* Input stream parsing */
ਸਥਿਰ whenspec_type whenspec = WHENSPEC_RAW;
ਸਥਿਰ ਬਣਤਰ strbuf command_buf = STRBUF_INIT;
ਸਥਿਰ ਪੂਰਨ unread_command_buf;
ਸਥਿਰ ਬਣਤਰ recent_command cmd_hist = {
	.prev = &cmd_hist,
	.ਅਗਲਾ = &cmd_hist,
};
ਸਥਿਰ ਬਣਤਰ recent_command *cmd_ਪੂਛ = &cmd_hist;
ਸਥਿਰ ਬਣਤਰ recent_command *rc_free;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cmd_save = 100;
ਸਥਿਰ uintmax_t ਅਗਲਾ_mark;
ਸਥਿਰ ਬਣਤਰ strbuf new_ਅੰਕੜੇ = STRBUF_INIT;
ਸਥਿਰ ਪੂਰਨ require_explicit_termination;

/* Signal handling */
ਸਥਿਰ ਅਸਥਿਰ sig_atomic_t checkpoint_requested;

/* Submodule marks */
ਸਥਿਰ ਬਣਤਰ string_list sub_marks_from = STRING_LIST_INIT_DUP;
ਸਥਿਰ ਬਣਤਰ string_list sub_marks_ਮੰਜ਼ਿਲ = STRING_LIST_INIT_DUP;
ਸਥਿਰ kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
ਸਥਿਰ ਪੂਰਨ cat_blob_fd = STDOUT_FILENO;

/* Command state */
ਬਣਤਰ fast_import_state {
	ਪੂਰਨ argc;
	ਅਚਲ ਅੱਖਰ **argv;
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਪੂਰਨ ਵੇਖਿਆ_ਅੰਕੜੇ_command;
	ਪੂਰਨ allow_unsafe_features;
	ਬਣਤਰ option *option;
};

ਸਥਿਰ ਖਾਲੀ fast_import_state_init(ਬਣਤਰ fast_import_state *state,
				   ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
				   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਬਣਤਰ option *option)
{
	memset(state, 0, ਆਕਾਰ(*state));
	state->argc = argc;
	state->argv = argv;
	state->ਅਗੇਤਰ = ਅਗੇਤਰ;
	state->option = option;
}

ਸਥਿਰ ਖਾਲੀ parse_argv(ਬਣਤਰ fast_import_state *state);
ਸਥਿਰ ਖਾਲੀ parse_get_mark(ਬਣਤਰ fast_import_state *state, ਅਚਲ ਅੱਖਰ *p);
ਸਥਿਰ ਖਾਲੀ parse_cat_blob(ਬਣਤਰ fast_import_state *state, ਅਚਲ ਅੱਖਰ *p);
ਸਥਿਰ ਖਾਲੀ parse_ls(ਬਣਤਰ fast_import_state *state,
		     ਅਚਲ ਅੱਖਰ *p,
		     ਬਣਤਰ branch *b);

ਸਥਿਰ ਖਾਲੀ for_each_mark(ਬਣਤਰ mark_ਸੈੱਟ *m, uintmax_t ਅਧਾਰ, each_mark_fn_t callback, ਖਾਲੀ *p)
{
	uintmax_t k;
	ਜੇ (m->ਖਿਸਕਾਓ) {
		ਲਈ (k = 0; k < 1024; k++) {
			ਜੇ (m->ਅੰਕੜੇ.sets[k])
				for_each_mark(m->ਅੰਕੜੇ.sets[k], ਅਧਾਰ + (k << m->ਖਿਸਕਾਓ), callback, p);
		}
	} ਨਹੀਂਤਾਂ {
		ਲਈ (k = 0; k < 1024; k++) {
			ਜੇ (m->ਅੰਕੜੇ.marked[k])
				callback(ਅਧਾਰ + k, m->ਅੰਕੜੇ.marked[k], p);
		}
	}
}

ਸਥਿਰ ਖਾਲੀ dump_marks_fn(uintmax_t mark, ਖਾਲੀ *object, ਖਾਲੀ *cbp) {
	ਬਣਤਰ object_ਇੰਦਰਾਜ *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_ਮੰਜ਼ਿਲ_hex(&e->idx.oid));
}

ਸਥਿਰ ਖਾਲੀ write_branch_report(FILE *rpt, ਬਣਤਰ branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	ਜੇ (b->ਸਰਗਰਮ)
		fputs(" active", rpt);
	ਜੇ (b->branch_ਰੁੱਖ.ਰੁੱਖ)
		fputs(" loaded", rpt);
	ਜੇ (is_null_oid(&b->branch_ਰੁੱਖ.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_ਮੰਜ਼ਿਲ_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_ਮੰਜ਼ਿਲ_hex(&b->branch_ਰੁੱਖ.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_ਮੰਜ਼ਿਲ_hex(&b->branch_ਰੁੱਖ.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->ਆਖਰੀ_commit);

	fputs("  last pack   : ", rpt);
	ਜੇ (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

ਸਥਿਰ ਖਾਲੀ write_crash_report(ਅਚਲ ਅੱਖਰ *err)
{
	ਅੱਖਰ *loc = repo_git_ਰਸਤਾ(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	ਬਣਤਰ branch *b;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ lu;
	ਬਣਤਰ recent_command *rc;

	ਜੇ (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		ਮੁਕਤ(loc);
		ਵਾਪਸ;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(ਸਮਾਂ(ਨਿਰਾ), 0, DATE_ਬਹੁਲਕ(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	ਲਈ (rc = cmd_hist.ਅਗਲਾ; rc != &cmd_hist; rc = rc->ਅਗਲਾ) {
		ਜੇ (rc->ਅਗਲਾ == &cmd_hist)
			fputs("* ", rpt);
		ਨਹੀਂਤਾਂ
			fputs("  ", rpt);
		fputs(rc->buf, rpt);
		fputc('\n', rpt);
	}

	fputc('\n', rpt);
	fputs("Active Branch LRU\n", rpt);
	fputs("-----------------\n", rpt);
	fprintf(rpt, "    active_branches = %lu cur, %lu max\n",
		cur_ਸਰਗਰਮ_branches,
		max_ਸਰਗਰਮ_branches);
	fputc('\n', rpt);
	fputs("  pos  clock name\n", rpt);
	fputs("  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", rpt);
	ਲਈ (b = ਸਰਗਰਮ_branches, lu = 0; b; b = b->ਸਰਗਰਮ_ਅਗਲਾ_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->ਆਖਰੀ_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	ਲਈ (lu = 0; lu < branch_ਸਾਰਣੀ_sz; lu++) {
		ਲਈ (b = branch_ਸਾਰਣੀ[lu]; b; b = b->ਸਾਰਣੀ_ਅਗਲਾ_branch)
			write_branch_report(rpt, b);
	}

	ਜੇ (ਪਹਿਲਾ_ਟੈਗ) {
		ਬਣਤਰ ਟੈਗ *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		ਲਈ (tg = ਪਹਿਲਾ_ਟੈਗ; tg; tg = tg->ਅਗਲਾ_ਟੈਗ) {
			fputs(oid_ਮੰਜ਼ਿਲ_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	ਜੇ (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	ਨਹੀਂਤਾਂ
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	ਮੁਕਤ(loc);
}

ਸਥਿਰ ਖਾਲੀ ਸਿਰਾ_packfile(ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ unkeep_all_packs(ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ dump_marks(ਖਾਲੀ);

ਸਥਿਰ NORETURN ਖਾਲੀ die_nicely(ਅਚਲ ਅੱਖਰ *err, va_list params)
{
	va_list cp;
	ਸਥਿਰ ਪੂਰਨ zombie;
	report_fn die_ਸੁਨੇਹਾ_fn = get_die_ਸੁਨੇਹਾ_routine();

	va_copy(cp, params);
	die_ਸੁਨੇਹਾ_fn(err, params);

	ਜੇ (!zombie) {
		ਅੱਖਰ ਸੁਨੇਹਾ_2[2 * ਰਸਤਾ_MAX];

		zombie = 1;
		vsnprintf(ਸੁਨੇਹਾ_2, ਆਕਾਰ(ਸੁਨੇਹਾ_2), err, cp);
		write_crash_report(ਸੁਨੇਹਾ_2);
		ਸਿਰਾ_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	ਬਾਹਰ_ਨਿਕਲੋ(128);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SIGUSR1	/* Windows, for example */

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_checkpoint_signal(ਖਾਲੀ)
{
}

#ਨਹੀਂਤਾਂ

ਸਥਿਰ ਖਾਲੀ checkpoint_signal(ਪੂਰਨ signo ਅਣਵਰਤਿਆ)
{
	checkpoint_requested = 1;
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_checkpoint_signal(ਖਾਲੀ)
{
	ਬਣਤਰ sigaction sa;

	memset(&sa, 0, ਆਕਾਰ(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_ਨਕਾਬ);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, ਨਿਰਾ);
}

#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ alloc_objects(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cnt)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ_pool *b;

	b = xmalloc(ਆਕਾਰ(ਬਣਤਰ object_ਇੰਦਰਾਜ_pool)
		+ cnt * ਆਕਾਰ(ਬਣਤਰ object_ਇੰਦਰਾਜ));
	b->ਅਗਲਾ_pool = ਖੰਡਾਂ;
	b->ਅਗਲਾ_free = b->entries;
	b->ਸਿਰਾ = b->entries + cnt;
	ਖੰਡਾਂ = b;
	alloc_ਗਿਣਤੀ += cnt;
}

ਸਥਿਰ ਬਣਤਰ object_ਇੰਦਰਾਜ *new_object(ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *e;

	ਜੇ (ਖੰਡਾਂ->ਅਗਲਾ_free == ਖੰਡਾਂ->ਸਿਰਾ)
		alloc_objects(object_ਇੰਦਰਾਜ_alloc);

	e = ਖੰਡਾਂ->ਅਗਲਾ_free++;
	oidcpy(&e->idx.oid, oid);
	ਵਾਪਸ e;
}

ਸਥਿਰ ਬਣਤਰ object_ਇੰਦਰਾਜ *ਲੱਭੋ_object(ਬਣਤਰ object_id *oid)
{
	ਵਾਪਸ hashmap_get_ਇੰਦਰਾਜ_from_hash(&object_ਸਾਰਣੀ, oidhash(oid), oid,
					   ਬਣਤਰ object_ਇੰਦਰਾਜ, ent);
}

ਸਥਿਰ ਬਣਤਰ object_ਇੰਦਰਾਜ *ਪਾਓ_object(ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *e;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hash = oidhash(oid);

	e = hashmap_get_ਇੰਦਰਾਜ_from_hash(&object_ਸਾਰਣੀ, hash, oid,
					ਬਣਤਰ object_ਇੰਦਰਾਜ, ent);
	ਜੇ (!e) {
		e = new_object(oid);
		e->idx.ਵਿੱਥ = 0;
		hashmap_ਇੰਦਰਾਜ_init(&e->ent, hash);
		hashmap_ਜੋੜੋ(&object_ਸਾਰਣੀ, &e->ent);
	}

	ਵਾਪਸ e;
}

ਸਥਿਰ ਖਾਲੀ invalidate_pack_id(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ id)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ lu;
	ਬਣਤਰ ਟੈਗ *t;
	ਬਣਤਰ hashmap_iter iter;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *e;

	hashmap_for_each_ਇੰਦਰਾਜ(&object_ਸਾਰਣੀ, &iter, e, ent) {
		ਜੇ (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	ਲਈ (lu = 0; lu < branch_ਸਾਰਣੀ_sz; lu++) {
		ਬਣਤਰ branch *b;

		ਲਈ (b = branch_ਸਾਰਣੀ[lu]; b; b = b->ਸਾਰਣੀ_ਅਗਲਾ_branch)
			ਜੇ (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	ਲਈ (t = ਪਹਿਲਾ_ਟੈਗ; t; t = t->ਅਗਲਾ_ਟੈਗ)
		ਜੇ (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hc_str(ਅਚਲ ਅੱਖਰ *s, ਮਾਪ_t len)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r = 0;
	ਜਦੋਂ (len-- > 0)
		r = r * 31 + *s++;
	ਵਾਪਸ r;
}

ਸਥਿਰ ਖਾਲੀ ਪਾਓ_mark(ਬਣਤਰ mark_ਸੈੱਟ **ਸਿਖਰ, uintmax_t idnum, ਬਣਤਰ object_ਇੰਦਰਾਜ *oe)
{
	ਬਣਤਰ mark_ਸੈੱਟ *s = *ਸਿਖਰ;

	ਜਦੋਂ ((idnum >> s->ਖਿਸਕਾਓ) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, ਆਕਾਰ(ਬਣਤਰ mark_ਸੈੱਟ));
		s->ਖਿਸਕਾਓ = (*ਸਿਖਰ)->ਖਿਸਕਾਓ + 10;
		s->ਅੰਕੜੇ.sets[0] = *ਸਿਖਰ;
		*ਸਿਖਰ = s;
	}
	ਜਦੋਂ (s->ਖਿਸਕਾਓ) {
		uintmax_t i = idnum >> s->ਖਿਸਕਾਓ;
		idnum -= i << s->ਖਿਸਕਾਓ;
		ਜੇ (!s->ਅੰਕੜੇ.sets[i]) {
			s->ਅੰਕੜੇ.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, ਆਕਾਰ(ਬਣਤਰ mark_ਸੈੱਟ));
			s->ਅੰਕੜੇ.sets[i]->ਖਿਸਕਾਓ = s->ਖਿਸਕਾਓ - 10;
		}
		s = s->ਅੰਕੜੇ.sets[i];
	}
	ਜੇ (!s->ਅੰਕੜੇ.marked[idnum])
		marks_ਸੈੱਟ_ਗਿਣਤੀ++;
	s->ਅੰਕੜੇ.marked[idnum] = oe;
}

ਸਥਿਰ ਖਾਲੀ *ਲੱਭੋ_mark(ਬਣਤਰ mark_ਸੈੱਟ *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *oe = ਨਿਰਾ;
	ਜੇ ((idnum >> s->ਖਿਸਕਾਓ) < 1024) {
		ਜਦੋਂ (s && s->ਖਿਸਕਾਓ) {
			uintmax_t i = idnum >> s->ਖਿਸਕਾਓ;
			idnum -= i << s->ਖਿਸਕਾਓ;
			s = s->ਅੰਕੜੇ.sets[i];
		}
		ਜੇ (s)
			oe = s->ਅੰਕੜੇ.marked[idnum];
	}
	ਜੇ (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	ਵਾਪਸ oe;
}

ਸਥਿਰ ਬਣਤਰ atom_str *ਮੰਜ਼ਿਲ_atom(ਅਚਲ ਅੱਖਰ *s, ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ len)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hc = hc_str(s, len) % atom_ਸਾਰਣੀ_sz;
	ਬਣਤਰ atom_str *c;

	ਲਈ (c = atom_ਸਾਰਣੀ[hc]; c; c = c->ਅਗਲਾ_atom)
		ਜੇ (c->str_len == len && !ਲੜੀ_ਤੁਲਨਾ(s, c->str_dat, len))
			ਵਾਪਸ c;

	c = mem_pool_alloc(&fi_mem_pool, ਆਕਾਰ(ਬਣਤਰ atom_str) + len + 1);
	c->str_len = len;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->ਅਗਲਾ_atom = atom_ਸਾਰਣੀ[hc];
	atom_ਸਾਰਣੀ[hc] = c;
	atom_cnt++;
	ਵਾਪਸ c;
}

ਸਥਿਰ ਬਣਤਰ branch *lookup_branch(ਅਚਲ ਅੱਖਰ *name)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hc = hc_str(name, ਲੜੀ_ਲੰਬਾਈ(name)) % branch_ਸਾਰਣੀ_sz;
	ਬਣਤਰ branch *b;

	ਲਈ (b = branch_ਸਾਰਣੀ[hc]; b; b = b->ਸਾਰਣੀ_ਅਗਲਾ_branch)
		ਜੇ (!strcmp(name, b->name))
			ਵਾਪਸ b;
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਬਣਤਰ branch *new_branch(ਅਚਲ ਅੱਖਰ *name)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hc = hc_str(name, ਲੜੀ_ਲੰਬਾਈ(name)) % branch_ਸਾਰਣੀ_sz;
	ਬਣਤਰ branch *b = lookup_branch(name);

	ਜੇ (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	ਜੇ (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, ਆਕਾਰ(ਬਣਤਰ branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->ਸਾਰਣੀ_ਅਗਲਾ_branch = branch_ਸਾਰਣੀ[hc];
	b->branch_ਰੁੱਖ.versions[0].ਬਹੁਲਕ = S_IFDIR;
	b->branch_ਰੁੱਖ.versions[1].ਬਹੁਲਕ = S_IFDIR;
	b->ਅੰਕ_notes = 0;
	b->ਸਰਗਰਮ = 0;
	b->pack_id = MAX_PACK_ID;
	branch_ਸਾਰਣੀ[hc] = b;
	branch_ਗਿਣਤੀ++;
	ਵਾਪਸ b;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hc_entries(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	ਵਾਪਸ cnt < avail_ਰੁੱਖ_ਸਾਰਣੀ_sz ? cnt : avail_ਰੁੱਖ_ਸਾਰਣੀ_sz - 1;
}

ਸਥਿਰ ਬਣਤਰ ਰੁੱਖ_content *new_ਰੁੱਖ_content(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cnt)
{
	ਬਣਤਰ avail_ਰੁੱਖ_content *f, *l = ਨਿਰਾ;
	ਬਣਤਰ ਰੁੱਖ_content *t;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hc = hc_entries(cnt);

	ਲਈ (f = avail_ਰੁੱਖ_ਸਾਰਣੀ[hc]; f; l = f, f = f->ਅਗਲਾ_avail)
		ਜੇ (f->ਇੰਦਰਾਜ_ਸਮਰੱਥਾ >= cnt)
			ਰੋਕੋ;

	ਜੇ (f) {
		ਜੇ (l)
			l->ਅਗਲਾ_avail = f->ਅਗਲਾ_avail;
		ਨਹੀਂਤਾਂ
			avail_ਰੁੱਖ_ਸਾਰਣੀ[hc] = f->ਅਗਲਾ_avail;
	} ਨਹੀਂਤਾਂ {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, ਆਕਾਰ(*t) + ਆਕਾਰ(t->entries[0]) * cnt);
		f->ਇੰਦਰਾਜ_ਸਮਰੱਥਾ = cnt;
	}

	t = (ਬਣਤਰ ਰੁੱਖ_content*)f;
	t->ਇੰਦਰਾਜ_ਗਿਣਤੀ = 0;
	t->delta_ਡੂੰਘਾਈ = 0;
	ਵਾਪਸ t;
}

ਸਥਿਰ ਖਾਲੀ release_ਰੁੱਖ_ਇੰਦਰਾਜ(ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *e);
ਸਥਿਰ ਖਾਲੀ release_ਰੁੱਖ_content(ਬਣਤਰ ਰੁੱਖ_content *t)
{
	ਬਣਤਰ avail_ਰੁੱਖ_content *f = (ਬਣਤਰ avail_ਰੁੱਖ_content*)t;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hc = hc_entries(f->ਇੰਦਰਾਜ_ਸਮਰੱਥਾ);
	f->ਅਗਲਾ_avail = avail_ਰੁੱਖ_ਸਾਰਣੀ[hc];
	avail_ਰੁੱਖ_ਸਾਰਣੀ[hc] = f;
}

ਸਥਿਰ ਖਾਲੀ release_ਰੁੱਖ_content_recursive(ਬਣਤਰ ਰੁੱਖ_content *t)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
	ਲਈ (i = 0; i < t->ਇੰਦਰਾਜ_ਗਿਣਤੀ; i++)
		release_ਰੁੱਖ_ਇੰਦਰਾਜ(t->entries[i]);
	release_ਰੁੱਖ_content(t);
}

ਸਥਿਰ ਬਣਤਰ ਰੁੱਖ_content *grow_ਰੁੱਖ_content(
	ਬਣਤਰ ਰੁੱਖ_content *t,
	ਪੂਰਨ amt)
{
	ਬਣਤਰ ਰੁੱਖ_content *r = new_ਰੁੱਖ_content(t->ਇੰਦਰਾਜ_ਗਿਣਤੀ + amt);
	r->ਇੰਦਰਾਜ_ਗਿਣਤੀ = t->ਇੰਦਰਾਜ_ਗਿਣਤੀ;
	r->delta_ਡੂੰਘਾਈ = t->delta_ਡੂੰਘਾਈ;
	COPY_ARRAY(r->entries, t->entries, t->ਇੰਦਰਾਜ_ਗਿਣਤੀ);
	release_ਰੁੱਖ_content(t);
	ਵਾਪਸ r;
}

ਸਥਿਰ ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *new_ਰੁੱਖ_ਇੰਦਰਾਜ(ਖਾਲੀ)
{
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *e;

	ਜੇ (!avail_ਰੁੱਖ_ਇੰਦਰਾਜ) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n = ਰੁੱਖ_ਇੰਦਰਾਜ_alloc;
		ਰੁੱਖ_ਇੰਦਰਾਜ_allocd += n * ਆਕਾਰ(ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ);
		ALLOC_ARRAY(e, n);
		avail_ਰੁੱਖ_ਇੰਦਰਾਜ = e;
		ਜਦੋਂ (n-- > 1) {
			*((ਖਾਲੀ**)e) = e + 1;
			e++;
		}
		*((ਖਾਲੀ**)e) = ਨਿਰਾ;
	}

	e = avail_ਰੁੱਖ_ਇੰਦਰਾਜ;
	avail_ਰੁੱਖ_ਇੰਦਰਾਜ = *((ਖਾਲੀ**)e);
	ਵਾਪਸ e;
}

ਸਥਿਰ ਖਾਲੀ release_ਰੁੱਖ_ਇੰਦਰਾਜ(ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *e)
{
	ਜੇ (e->ਰੁੱਖ)
		release_ਰੁੱਖ_content_recursive(e->ਰੁੱਖ);
	*((ਖਾਲੀ**)e) = avail_ਰੁੱਖ_ਇੰਦਰਾਜ;
	avail_ਰੁੱਖ_ਇੰਦਰਾਜ = e;
}

ਸਥਿਰ ਬਣਤਰ ਰੁੱਖ_content *dup_ਰੁੱਖ_content(ਬਣਤਰ ਰੁੱਖ_content *s)
{
	ਬਣਤਰ ਰੁੱਖ_content *d;
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *a, *b;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

	ਜੇ (!s)
		ਵਾਪਸ ਨਿਰਾ;
	d = new_ਰੁੱਖ_content(s->ਇੰਦਰਾਜ_ਗਿਣਤੀ);
	ਲਈ (i = 0; i < s->ਇੰਦਰਾਜ_ਗਿਣਤੀ; i++) {
		a = s->entries[i];
		b = new_ਰੁੱਖ_ਇੰਦਰਾਜ();
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(b, a, ਆਕਾਰ(*a));
		ਜੇ (a->ਰੁੱਖ && is_null_oid(&b->versions[1].oid))
			b->ਰੁੱਖ = dup_ਰੁੱਖ_content(a->ਰੁੱਖ);
		ਨਹੀਂਤਾਂ
			b->ਰੁੱਖ = ਨਿਰਾ;
		d->entries[i] = b;
	}
	d->ਇੰਦਰਾਜ_ਗਿਣਤੀ = s->ਇੰਦਰਾਜ_ਗਿਣਤੀ;
	d->delta_ਡੂੰਘਾਈ = s->delta_ਡੂੰਘਾਈ;

	ਵਾਪਸ d;
}

ਸਥਿਰ ਖਾਲੀ ਸ਼ੁਰੂ_packfile(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf tmp_file = STRBUF_INIT;
	ਬਣਤਰ packed_git *p;
	ਪੂਰਨ pack_fd;

	pack_fd = odb_mkstemp(the_repository->objects, &tmp_file,
			      "pack/tmp_pack_XXXXXX");
	FLEX_ALLOC_STR(p, pack_name, tmp_file.buf);
	strbuf_release(&tmp_file);

	p->pack_fd = pack_fd;
	p->do_not_close = 1;
	p->repo = the_repository;
	pack_file = hashfd(the_repository->hash_algo, pack_fd, p->pack_name);

	pack_ਅੰਕੜੇ = p;
	pack_ਮਾਪ = write_pack_ਸਿਰਲੇਖ(pack_file, 0);
	object_ਗਿਣਤੀ = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *create_ਸੂਚਕਾਂਕ(ਖਾਲੀ)
{
	ਅਚਲ ਅੱਖਰ *tmpfile;
	ਬਣਤਰ pack_idx_ਇੰਦਰਾਜ **idx, **c, **ਆਖਰੀ;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *e;
	ਬਣਤਰ object_ਇੰਦਰਾਜ_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_ਗਿਣਤੀ);
	c = idx;
	ਲਈ (o = ਖੰਡਾਂ; o; o = o->ਅਗਲਾ_pool)
		ਲਈ (e = o->ਅਗਲਾ_free; e-- != o->entries;)
			ਜੇ (pack_id == e->pack_id)
				*c++ = &e->idx;
	ਆਖਰੀ = idx + object_ਗਿਣਤੀ;
	ਜੇ (c != ਆਖਰੀ)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, ਨਿਰਾ, idx, object_ਗਿਣਤੀ,
				 &pack_idx_opts, pack_ਅੰਕੜੇ->hash);
	ਮੁਕਤ(idx);
	ਵਾਪਸ tmpfile;
}

ਸਥਿਰ ਅੱਖਰ *keep_pack(ਅਚਲ ਅੱਖਰ *curr_ਸੂਚਕਾਂਕ_name)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *keep_ਸੁਨੇਹਾ = "fast-import";
	ਬਣਤਰ strbuf name = STRBUF_INIT;
	ਪੂਰਨ keep_fd;

	odb_pack_name(pack_ਅੰਕੜੇ->repo, &name, pack_ਅੰਕੜੇ->hash, "keep");
	keep_fd = ਸੁਰੱਖਿਅਤ_create_file_with_leading_directories(pack_ਅੰਕੜੇ->repo,
							    name.buf);
	ਜੇ (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_ਸੁਨੇਹਾ, ਲੜੀ_ਲੰਬਾਈ(keep_ਸੁਨੇਹਾ));
	ਜੇ (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_ਅੰਕੜੇ->repo, &name, pack_ਅੰਕੜੇ->hash, "pack");
	ਜੇ (finalize_object_file(pack_ਅੰਕੜੇ->repo, pack_ਅੰਕੜੇ->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_ਅੰਕੜੇ->repo, &name, pack_ਅੰਕੜੇ->hash, "idx");
	ਜੇ (finalize_object_file(pack_ਅੰਕੜੇ->repo, curr_ਸੂਚਕਾਂਕ_name, name.buf))
		die(_("cannot store index file"));
	ਮੁਕਤ((ਖਾਲੀ *)curr_ਸੂਚਕਾਂਕ_name);
	ਵਾਪਸ strbuf_detach(&name, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ unkeep_all_packs(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf name = STRBUF_INIT;
	ਪੂਰਨ k;

	ਲਈ (k = 0; k < pack_id; k++) {
		ਬਣਤਰ packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

ਸਥਿਰ ਪੂਰਨ loosen_small_pack(ਅਚਲ ਬਣਤਰ packed_git *p)
{
	ਬਣਤਰ ਬੱਚਾ_process unpack = ਬੱਚਾ_PROCESS_INIT;

	ਜੇ (lseek(p->pack_fd, 0, SEEK_ਸੈੱਟ) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.ਵਿੱਚ = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_ਮੰਜ਼ਿਲ_stderr = 1;
	strvec_ਧੱਕੋ(&unpack.ਦਲੀਲਾਂ, "unpack-objects");
	ਜੇ (!show_stats)
		strvec_ਧੱਕੋ(&unpack.ਦਲੀਲਾਂ, "-q");

	ਵਾਪਸ ਲੜੀਵਾਰ_command(&unpack);
}

ਸਥਿਰ ਖਾਲੀ ਸਿਰਾ_packfile(ਖਾਲੀ)
{
	ਸਥਿਰ ਪੂਰਨ running;

	ਜੇ (running || !pack_ਅੰਕੜੇ)
		ਵਾਪਸ;

	running = 1;
	clear_delta_ਅਧਾਰ_cache();
	ਜੇ (object_ਗਿਣਤੀ) {
		ਬਣਤਰ odb_ਸਰੋਤ_files *files = odb_ਸਰੋਤ_files_downcast(pack_ਅੰਕੜੇ->repo->objects->ਸਰੋਤ);
		ਬਣਤਰ packed_git *new_p;
		ਬਣਤਰ object_id cur_pack_oid;
		ਅੱਖਰ *idx_name;
		ਪੂਰਨ i;
		ਬਣਤਰ branch *b;
		ਬਣਤਰ ਟੈਗ *t;

		close_pack_windows(pack_ਅੰਕੜੇ);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_ਸਿਰਲੇਖ_footer(the_hash_algo, pack_ਅੰਕੜੇ->pack_fd,
					 pack_ਅੰਕੜੇ->hash, pack_ਅੰਕੜੇ->pack_name,
					 object_ਗਿਣਤੀ, cur_pack_oid.hash,
					 pack_ਮਾਪ);

		ਜੇ (object_ਗਿਣਤੀ <= unpack_ਹੱਦਬੰਦੀ) {
			ਜੇ (!loosen_small_pack(pack_ਅੰਕੜੇ)) {
				invalidate_pack_id(pack_id);
				ਜਾਓ discard_pack;
			}
		}

		close(pack_ਅੰਕੜੇ->pack_fd);
		idx_name = keep_pack(create_ਸੂਚਕਾਂਕ());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_ਲੱਦੋ_pack(files->dirs->packed, idx_name, 1);
		ਜੇ (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		ਮੁਕਤ(idx_name);

		/* Print the boundary */
		ਜੇ (pack_ਕਿਨਾਰੇ) {
			fprintf(pack_ਕਿਨਾਰੇ, "%s:", new_p->pack_name);
			ਲਈ (i = 0; i < branch_ਸਾਰਣੀ_sz; i++) {
				ਲਈ (b = branch_ਸਾਰਣੀ[i]; b; b = b->ਸਾਰਣੀ_ਅਗਲਾ_branch) {
					ਜੇ (b->pack_id == pack_id)
						fprintf(pack_ਕਿਨਾਰੇ, " %s",
							oid_ਮੰਜ਼ਿਲ_hex(&b->oid));
				}
			}
			ਲਈ (t = ਪਹਿਲਾ_ਟੈਗ; t; t = t->ਅਗਲਾ_ਟੈਗ) {
				ਜੇ (t->pack_id == pack_id)
					fprintf(pack_ਕਿਨਾਰੇ, " %s",
						oid_ਮੰਜ਼ਿਲ_hex(&t->oid));
			}
			fputc('\n', pack_ਕਿਨਾਰੇ);
			fflush(pack_ਕਿਨਾਰੇ);
		}

		pack_id++;
	}
	ਨਹੀਂਤਾਂ {
discard_pack:
		close(pack_ਅੰਕੜੇ->pack_fd);
		unlink_or_warn(pack_ਅੰਕੜੇ->pack_name);
	}
	FREE_AND_NULL(pack_ਅੰਕੜੇ);
	running = 0;

	/* We can't carry a delta across packfiles. */
	strbuf_release(&ਆਖਰੀ_blob.ਅੰਕੜੇ);
	ਆਖਰੀ_blob.ਵਿੱਥ = 0;
	ਆਖਰੀ_blob.ਡੂੰਘਾਈ = 0;
}

ਸਥਿਰ ਖਾਲੀ ਚੱਕਰ_packfile(ਖਾਲੀ)
{
	ਸਿਰਾ_packfile();
	ਸ਼ੁਰੂ_packfile();
}

ਸਥਿਰ ਪੂਰਨ store_object(
	ਗਿਣਤੀ object_type type,
	ਬਣਤਰ strbuf *dat,
	ਬਣਤਰ ਆਖਰੀ_object *ਆਖਰੀ,
	ਬਣਤਰ object_id *oidout,
	uintmax_t mark)
{
	ਬਣਤਰ odb_ਸਰੋਤ_files *files = odb_ਸਰੋਤ_files_downcast(the_repository->objects->ਸਰੋਤ);
	ਖਾਲੀ *ਬਾਹਰਲਾ, *delta;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *e;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ hdr[96];
	ਬਣਤਰ object_id oid;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ hdrlen, deltalen = 0;
	ਬਣਤਰ git_hash_ctx c;
	git_zstream s;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	hdrlen = format_object_ਸਿਰਲੇਖ((ਅੱਖਰ *)hdr, ਆਕਾਰ(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	ਜੇ (oidout)
		oidcpy(oidout, &oid);

	e = ਪਾਓ_object(&oid);
	ਜੇ (mark)
		ਪਾਓ_mark(&marks, mark, e);
	ਜੇ (e->idx.ਵਿੱਥ) {
		duplicate_ਗਿਣਤੀ_by_type[type]++;
		ਵਾਪਸ 1;
	}

	ਲਈ (ਬਣਤਰ odb_files_dir *dir = files->dirs; dir; dir = dir->ਅਗਲਾ) {
		ਜੇ (!packfile_list_ਲੱਭੋ_oid(packfile_store_get_packs(dir->packed), &oid))
			ਜਾਰੀ;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.ਵਿੱਥ = 1; /* just not zero! */
		duplicate_ਗਿਣਤੀ_by_type[type]++;
		ਵਾਪਸ 1;
	}

	ਜੇ (ਆਖਰੀ && ਆਖਰੀ->ਅੰਕੜੇ.len && ਆਖਰੀ->ਅੰਕੜੇ.buf && ਆਖਰੀ->ਡੂੰਘਾਈ < max_ਡੂੰਘਾਈ
		&& dat->len > the_hash_algo->rawsz) {
		ਮਾਪ_t deltalen_st;

		delta_ਗਿਣਤੀ_attempts_by_type[type]++;
		delta = diff_delta(ਆਖਰੀ->ਅੰਕੜੇ.buf, ਆਖਰੀ->ਅੰਕੜੇ.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(deltalen_st);
	} ਨਹੀਂਤਾਂ
		delta = ਨਿਰਾ;

	git_deflate_init(&s, cfg->pack_compression_level);
	ਜੇ (delta) {
		s.ਅਗਲਾ_ਵਿੱਚ = delta;
		s.avail_ਵਿੱਚ = deltalen;
	} ਨਹੀਂਤਾਂ {
		s.ਅਗਲਾ_ਵਿੱਚ = (ਖਾਲੀ *)dat->buf;
		s.avail_ਵਿੱਚ = dat->len;
	}
	s.avail_ਬਾਹਰਲਾ = git_deflate_bound(&s, s.avail_ਵਿੱਚ);
	s.ਅਗਲਾ_ਬਾਹਰਲਾ = ਬਾਹਰਲਾ = xmalloc(s.avail_ਬਾਹਰਲਾ);
	ਜਦੋਂ (git_deflate(&s, Z_ਅੰਤ) == Z_OK)
		; /* nothing */
	git_deflate_ਸਿਰਾ(&s);

	/* Determine if we should auto-checkpoint. */
	ਜੇ ((max_packsize
		&& (pack_ਮਾਪ + PACK_ਮਾਪ_ਹੱਦ + s.ਕੁੱਲ_ਬਾਹਰਲਾ) > max_packsize)
		|| (pack_ਮਾਪ + PACK_ਮਾਪ_ਹੱਦ + s.ਕੁੱਲ_ਬਾਹਰਲਾ) < pack_ਮਾਪ) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		ਚੱਕਰ_packfile();

		/* We cannot carry a delta into the new pack. */
		ਜੇ (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.ਅਗਲਾ_ਵਿੱਚ = (ਖਾਲੀ *)dat->buf;
			s.avail_ਵਿੱਚ = dat->len;
			s.avail_ਬਾਹਰਲਾ = git_deflate_bound(&s, s.avail_ਵਿੱਚ);
			s.ਅਗਲਾ_ਬਾਹਰਲਾ = ਬਾਹਰਲਾ = xrealloc(ਬਾਹਰਲਾ, s.avail_ਬਾਹਰਲਾ);
			ਜਦੋਂ (git_deflate(&s, Z_ਅੰਤ) == Z_OK)
				; /* nothing */
			git_deflate_ਸਿਰਾ(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.ਵਿੱਥ = pack_ਮਾਪ;
	object_ਗਿਣਤੀ++;
	object_ਗਿਣਤੀ_by_type[type]++;

	ਚੱਕਰੀ_ਜਾਂਚ_32_begin(pack_file);

	ਜੇ (delta) {
		off_t ofs = e->idx.ਵਿੱਥ - ਆਖਰੀ->ਵਿੱਥ;
		ਬਿਨਾਨਿਸ਼ਾਨੀ pos = ਆਕਾਰ(hdr) - 1;

		delta_ਗਿਣਤੀ_by_type[type]++;
		e->ਡੂੰਘਾਈ = ਆਖਰੀ->ਡੂੰਘਾਈ + 1;

		hdrlen = encode_ਵਿੱਚ_pack_object_ਸਿਰਲੇਖ(hdr, ਆਕਾਰ(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_ਮਾਪ += hdrlen;

		hdr[pos] = ofs & 127;
		ਜਦੋਂ (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, ਆਕਾਰ(hdr) - pos);
		pack_ਮਾਪ += ਆਕਾਰ(hdr) - pos;
	} ਨਹੀਂਤਾਂ {
		e->ਡੂੰਘਾਈ = 0;
		hdrlen = encode_ਵਿੱਚ_pack_object_ਸਿਰਲੇਖ(hdr, ਆਕਾਰ(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_ਮਾਪ += hdrlen;
	}

	hashwrite(pack_file, ਬਾਹਰਲਾ, s.ਕੁੱਲ_ਬਾਹਰਲਾ);
	pack_ਮਾਪ += s.ਕੁੱਲ_ਬਾਹਰਲਾ;

	e->idx.ਚੱਕਰੀ_ਜਾਂਚ_32 = ਚੱਕਰੀ_ਜਾਂਚ_32_ਸਿਰਾ(pack_file);

	ਮੁਕਤ(ਬਾਹਰਲਾ);
	ਮੁਕਤ(delta);
	ਜੇ (ਆਖਰੀ) {
		ਜੇ (ਆਖਰੀ->no_ਅਦਲਾ_ਬਦਲੀ) {
			ਆਖਰੀ->ਅੰਕੜੇ = *dat;
		} ਨਹੀਂਤਾਂ {
			strbuf_ਅਦਲਾ_ਬਦਲੀ(&ਆਖਰੀ->ਅੰਕੜੇ, dat);
		}
		ਆਖਰੀ->ਵਿੱਥ = e->idx.ਵਿੱਥ;
		ਆਖਰੀ->ਡੂੰਘਾਈ = e->ਡੂੰਘਾਈ;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ truncate_pack(ਬਣਤਰ hashfile_checkpoint *checkpoint)
{
	ਜੇ (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_ਮਾਪ = checkpoint->ਵਿੱਥ;
}

ਸਥਿਰ ਖਾਲੀ ਧਾਰਾ_blob(uintmax_t len, ਬਣਤਰ object_id *oidout, uintmax_t mark)
{
	ਬਣਤਰ odb_ਸਰੋਤ_files *files = odb_ਸਰੋਤ_files_downcast(the_repository->objects->ਸਰੋਤ);
	ਮਾਪ_t ਵਿੱਚ_sz = 64 * 1024, ਬਾਹਰਲਾ_sz = 64 * 1024;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਵਿੱਚ_buf = xmalloc(ਵਿੱਚ_sz);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਬਾਹਰਲਾ_buf = xmalloc(ਬਾਹਰਲਾ_sz);
	ਬਣਤਰ object_ਇੰਦਰਾਜ *e;
	ਬਣਤਰ object_id oid;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ hdrlen;
	off_t ਵਿੱਥ;
	ਬਣਤਰ git_hash_ctx c;
	git_zstream s;
	ਬਣਤਰ hashfile_checkpoint checkpoint;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);
	ਪੂਰਨ ਸਥਿਤੀ = Z_OK;

	/* Determine if we should auto-checkpoint. */
	ਜੇ ((max_packsize
		&& (pack_ਮਾਪ + PACK_ਮਾਪ_ਹੱਦ + len) > max_packsize)
		|| (pack_ਮਾਪ + PACK_ਮਾਪ_ਹੱਦ + len) < pack_ਮਾਪ)
		ਚੱਕਰ_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	ਵਿੱਥ = checkpoint.ਵਿੱਥ;

	hdrlen = format_object_ਸਿਰਲੇਖ((ਅੱਖਰ *)ਬਾਹਰਲਾ_buf, ਬਾਹਰਲਾ_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, ਬਾਹਰਲਾ_buf, hdrlen);

	ਚੱਕਰੀ_ਜਾਂਚ_32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_ਵਿੱਚ_pack_object_ਸਿਰਲੇਖ(ਬਾਹਰਲਾ_buf, ਬਾਹਰਲਾ_sz, OBJ_BLOB, len);

	s.ਅਗਲਾ_ਬਾਹਰਲਾ = ਬਾਹਰਲਾ_buf + hdrlen;
	s.avail_ਬਾਹਰਲਾ = ਬਾਹਰਲਾ_sz - hdrlen;

	ਜਦੋਂ (ਸਥਿਤੀ != Z_ਧਾਰਾ_ਸਿਰਾ) {
		ਜੇ (0 < len && !s.avail_ਵਿੱਚ) {
			ਮਾਪ_t cnt = ਵਿੱਚ_sz < len ? ਵਿੱਚ_sz : (ਮਾਪ_t)len;
			ਮਾਪ_t n = fread(ਵਿੱਚ_buf, 1, cnt, stdin);
			ਜੇ (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, ਵਿੱਚ_buf, n);
			s.ਅਗਲਾ_ਵਿੱਚ = ਵਿੱਚ_buf;
			s.avail_ਵਿੱਚ = n;
			len -= n;
		}

		ਸਥਿਤੀ = git_deflate(&s, len ? 0 : Z_ਅੰਤ);

		ਜੇ (!s.avail_ਬਾਹਰਲਾ || ਸਥਿਤੀ == Z_ਧਾਰਾ_ਸਿਰਾ) {
			ਮਾਪ_t n = s.ਅਗਲਾ_ਬਾਹਰਲਾ - ਬਾਹਰਲਾ_buf;
			hashwrite(pack_file, ਬਾਹਰਲਾ_buf, n);
			pack_ਮਾਪ += n;
			s.ਅਗਲਾ_ਬਾਹਰਲਾ = ਬਾਹਰਲਾ_buf;
			s.avail_ਬਾਹਰਲਾ = ਬਾਹਰਲਾ_sz;
		}

		ਚੋਣ (ਸਥਿਤੀ) {
		ਹਾਲਤ Z_OK:
		ਹਾਲਤ Z_BUF_ERROR:
		ਹਾਲਤ Z_ਧਾਰਾ_ਸਿਰਾ:
			ਜਾਰੀ;
		ਮੂਲ:
			die(_("unexpected deflate failure: %d"), ਸਥਿਤੀ);
		}
	}
	git_deflate_ਸਿਰਾ(&s);
	git_hash_final_oid(&oid, &c);

	ਜੇ (oidout)
		oidcpy(oidout, &oid);

	e = ਪਾਓ_object(&oid);

	ਜੇ (mark)
		ਪਾਓ_mark(&marks, mark, e);

	ਜੇ (e->idx.ਵਿੱਥ) {
		duplicate_ਗਿਣਤੀ_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਲਈ (ਬਣਤਰ odb_files_dir *dir = files->dirs; dir; dir = dir->ਅਗਲਾ) {
		ਜੇ (!packfile_list_ਲੱਭੋ_oid(packfile_store_get_packs(dir->packed), &oid))
			ਜਾਰੀ;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.ਵਿੱਥ = 1; /* just not zero! */
		duplicate_ਗਿਣਤੀ_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	e->ਡੂੰਘਾਈ = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.ਵਿੱਥ = ਵਿੱਥ;
	e->idx.ਚੱਕਰੀ_ਜਾਂਚ_32 = ਚੱਕਰੀ_ਜਾਂਚ_32_ਸਿਰਾ(pack_file);
	object_ਗਿਣਤੀ++;
	object_ਗਿਣਤੀ_by_type[OBJ_BLOB]++;

ਬਾਹਰਲਾ:
	ਮੁਕਤ(ਵਿੱਚ_buf);
	ਮੁਕਤ(ਬਾਹਰਲਾ_buf);
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
ਸਥਿਰ ਖਾਲੀ *gfi_unpack_ਇੰਦਰਾਜ(
	ਬਣਤਰ object_ਇੰਦਰਾਜ *oe,
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *sizep)
{
	ਗਿਣਤੀ object_type type;
	ਮਾਪ_t ਮਾਪ_st = 0;
	ਖਾਲੀ *ਅੰਕੜੇ;
	ਬਣਤਰ packed_git *p = all_packs[oe->pack_id];
	ਜੇ (p == pack_ਅੰਕੜੇ && p->pack_ਮਾਪ < (pack_ਮਾਪ + the_hash_algo->rawsz)) {
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
		p->pack_ਮਾਪ = pack_ਮਾਪ + the_hash_algo->rawsz;
	}
	ਅੰਕੜੇ = unpack_ਇੰਦਰਾਜ(the_repository, p, oe->idx.ਵਿੱਥ, &type, &ਮਾਪ_st);
	ਜੇ (sizep)
		*sizep = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(ਮਾਪ_st);
	ਵਾਪਸ ਅੰਕੜੇ;
}

ਸਥਿਰ ਖਾਲੀ ਲੱਦੋ_ਰੁੱਖ(ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *ਜੜ੍ਹ)
{
	ਬਣਤਰ object_id *oid = &ਜੜ੍ਹ->versions[1].oid;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *myoe;
	ਬਣਤਰ ਰੁੱਖ_content *t;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;
	ਅੱਖਰ *buf;
	ਅਚਲ ਅੱਖਰ *c;

	ਜੜ੍ਹ->ਰੁੱਖ = t = new_ਰੁੱਖ_content(8);
	ਜੇ (is_null_oid(oid))
		ਵਾਪਸ;

	myoe = ਲੱਭੋ_object(oid);
	ਜੇ (myoe && myoe->pack_id != MAX_PACK_ID) {
		ਜੇ (myoe->type != OBJ_ਰੁੱਖ)
			die(_("not a tree: %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		t->delta_ਡੂੰਘਾਈ = myoe->ਡੂੰਘਾਈ;
		buf = gfi_unpack_ਇੰਦਰਾਜ(myoe, &ਮਾਪ);
		ਜੇ (!buf)
			die(_("can't load tree %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
	} ਨਹੀਂਤਾਂ {
		ਗਿਣਤੀ object_type type;
		ਮਾਪ_t ਮਾਪ_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &ਮਾਪ_st);
		ਮਾਪ = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(ਮਾਪ_st);
		ਜੇ (!buf || type != OBJ_ਰੁੱਖ)
			die(_("can't load tree %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
	}

	c = buf;
	ਜਦੋਂ (c != (buf + ਮਾਪ)) {
		ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *e = new_ਰੁੱਖ_ਇੰਦਰਾਜ();

		ਜੇ (t->ਇੰਦਰਾਜ_ਗਿਣਤੀ == t->ਇੰਦਰਾਜ_ਸਮਰੱਥਾ)
			ਜੜ੍ਹ->ਰੁੱਖ = t = grow_ਰੁੱਖ_content(t, t->ਇੰਦਰਾਜ_ਗਿਣਤੀ);
		t->entries[t->ਇੰਦਰਾਜ_ਗਿਣਤੀ++] = e;

		e->ਰੁੱਖ = ਨਿਰਾ;
		c = parse_ਬਹੁਲਕ(c, &e->versions[1].ਬਹੁਲਕ);
		ਜੇ (!c)
			die(_("corrupt mode in %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		e->versions[0].ਬਹੁਲਕ = e->versions[1].ਬਹੁਲਕ;
		e->name = ਮੰਜ਼ਿਲ_atom(c, ਲੜੀ_ਲੰਬਾਈ(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	ਮੁਕਤ(buf);
}

ਸਥਿਰ ਪੂਰਨ tecmp0 (ਅਚਲ ਖਾਲੀ *_a, ਅਚਲ ਖਾਲੀ *_b)
{
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *a = *((ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ**)_a);
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *b = *((ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ**)_b);
	ਵਾਪਸ ਅਧਾਰ_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].ਬਹੁਲਕ,
		b->name->str_dat, b->name->str_len, b->versions[0].ਬਹੁਲਕ);
}

ਸਥਿਰ ਪੂਰਨ tecmp1 (ਅਚਲ ਖਾਲੀ *_a, ਅਚਲ ਖਾਲੀ *_b)
{
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *a = *((ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ**)_a);
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *b = *((ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ**)_b);
	ਵਾਪਸ ਅਧਾਰ_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].ਬਹੁਲਕ,
		b->name->str_dat, b->name->str_len, b->versions[1].ਬਹੁਲਕ);
}

ਸਥਿਰ ਖਾਲੀ mktree(ਬਣਤਰ ਰੁੱਖ_content *t, ਪੂਰਨ v, ਬਣਤਰ strbuf *b)
{
	ਮਾਪ_t maxlen = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

	ਜੇ (!v)
		QSORT(t->entries, t->ਇੰਦਰਾਜ_ਗਿਣਤੀ, tecmp0);
	ਨਹੀਂਤਾਂ
		QSORT(t->entries, t->ਇੰਦਰਾਜ_ਗਿਣਤੀ, tecmp1);

	ਲਈ (i = 0; i < t->ਇੰਦਰਾਜ_ਗਿਣਤੀ; i++) {
		ਜੇ (t->entries[i]->versions[v].ਬਹੁਲਕ)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	ਲਈ (i = 0; i < t->ਇੰਦਰਾਜ_ਗਿਣਤੀ; i++) {
		ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *e = t->entries[i];
		ਜੇ (!e->versions[v].ਬਹੁਲਕ)
			ਜਾਰੀ;
		strbuf_addf(b, "%o %s%c",
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(e->versions[v].ਬਹੁਲਕ & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_ਜੋੜੋ(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

ਸਥਿਰ ਖਾਲੀ store_ਰੁੱਖ(ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *ਜੜ੍ਹ)
{
	ਬਣਤਰ ਰੁੱਖ_content *t;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, j, del;
	ਬਣਤਰ ਆਖਰੀ_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	ਬਣਤਰ object_ਇੰਦਰਾਜ *le = ਨਿਰਾ;

	ਜੇ (!is_null_oid(&ਜੜ੍ਹ->versions[1].oid))
		ਵਾਪਸ;

	ਜੇ (!ਜੜ੍ਹ->ਰੁੱਖ)
		ਲੱਦੋ_ਰੁੱਖ(ਜੜ੍ਹ);
	t = ਜੜ੍ਹ->ਰੁੱਖ;

	ਲਈ (i = 0; i < t->ਇੰਦਰਾਜ_ਗਿਣਤੀ; i++) {
		ਜੇ (t->entries[i]->ਰੁੱਖ)
			store_ਰੁੱਖ(t->entries[i]);
	}

	ਜੇ (!(ਜੜ੍ਹ->versions[0].ਬਹੁਲਕ & NO_DELTA))
		le = ਲੱਭੋ_object(&ਜੜ੍ਹ->versions[0].oid);
	ਜੇ (S_ISDIR(ਜੜ੍ਹ->versions[0].ਬਹੁਲਕ) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_ਰੁੱਖ);
		lo.ਅੰਕੜੇ = old_ਰੁੱਖ;
		lo.ਵਿੱਥ = le->idx.ਵਿੱਥ;
		lo.ਡੂੰਘਾਈ = t->delta_ਡੂੰਘਾਈ;
	}

	mktree(t, 1, &new_ਰੁੱਖ);
	store_object(OBJ_ਰੁੱਖ, &new_ਰੁੱਖ, &lo, &ਜੜ੍ਹ->versions[1].oid, 0);

	t->delta_ਡੂੰਘਾਈ = lo.ਡੂੰਘਾਈ;
	ਲਈ (i = 0, j = 0, del = 0; i < t->ਇੰਦਰਾਜ_ਗਿਣਤੀ; i++) {
		ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *e = t->entries[i];
		ਜੇ (e->versions[1].ਬਹੁਲਕ) {
			e->versions[0].ਬਹੁਲਕ = e->versions[1].ਬਹੁਲਕ;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} ਨਹੀਂਤਾਂ {
			release_ਰੁੱਖ_ਇੰਦਰਾਜ(e);
			del++;
		}
	}
	t->ਇੰਦਰਾਜ_ਗਿਣਤੀ -= del;
}

ਸਥਿਰ ਖਾਲੀ ਰੁੱਖ_content_replace(
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *ਜੜ੍ਹ,
	ਅਚਲ ਬਣਤਰ object_id *oid,
	ਅਚਲ uint16_t ਬਹੁਲਕ,
	ਬਣਤਰ ਰੁੱਖ_content *newtree)
{
	ਜੇ (!S_ISDIR(ਬਹੁਲਕ))
		die(_("root cannot be a non-directory"));
	oidclr(&ਜੜ੍ਹ->versions[0].oid, the_repository->hash_algo);
	oidcpy(&ਜੜ੍ਹ->versions[1].oid, oid);
	ਜੇ (ਜੜ੍ਹ->ਰੁੱਖ)
		release_ਰੁੱਖ_content_recursive(ਜੜ੍ਹ->ਰੁੱਖ);
	ਜੜ੍ਹ->ਰੁੱਖ = newtree;
}

ਸਥਿਰ ਪੂਰਨ ਰੁੱਖ_content_ਸੈੱਟ(
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *ਜੜ੍ਹ,
	ਅਚਲ ਅੱਖਰ *p,
	ਅਚਲ ਬਣਤਰ object_id *oid,
	ਅਚਲ uint16_t ਬਹੁਲਕ,
	ਬਣਤਰ ਰੁੱਖ_content *subtree)
{
	ਬਣਤਰ ਰੁੱਖ_content *t;
	ਅਚਲ ਅੱਖਰ *slash1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, n;
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	ਜੇ (!n)
		die(_("empty path component found in input"));
	ਜੇ (!*slash1 && !S_ISDIR(ਬਹੁਲਕ) && subtree)
		die(_("non-directories cannot have subtrees"));

	ਜੇ (!ਜੜ੍ਹ->ਰੁੱਖ)
		ਲੱਦੋ_ਰੁੱਖ(ਜੜ੍ਹ);
	t = ਜੜ੍ਹ->ਰੁੱਖ;
	ਲਈ (i = 0; i < t->ਇੰਦਰਾਜ_ਗਿਣਤੀ; i++) {
		e = t->entries[i];
		ਜੇ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ਜੇ (!*slash1) {
				ਜੇ (!S_ISDIR(ਬਹੁਲਕ)
						&& e->versions[1].ਬਹੁਲਕ == ਬਹੁਲਕ
						&& oideq(&e->versions[1].oid, oid))
					ਵਾਪਸ 0;
				e->versions[1].ਬਹੁਲਕ = ਬਹੁਲਕ;
				oidcpy(&e->versions[1].oid, oid);
				ਜੇ (e->ਰੁੱਖ)
					release_ਰੁੱਖ_content_recursive(e->ਰੁੱਖ);
				e->ਰੁੱਖ = subtree;

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
				ਜੇ (S_ISDIR(e->versions[0].ਬਹੁਲਕ))
					e->versions[0].ਬਹੁਲਕ |= NO_DELTA;

				oidclr(&ਜੜ੍ਹ->versions[1].oid, the_repository->hash_algo);
				ਵਾਪਸ 1;
			}
			ਜੇ (!S_ISDIR(e->versions[1].ਬਹੁਲਕ)) {
				e->ਰੁੱਖ = new_ਰੁੱਖ_content(8);
				e->versions[1].ਬਹੁਲਕ = S_IFDIR;
			}
			ਜੇ (!e->ਰੁੱਖ)
				ਲੱਦੋ_ਰੁੱਖ(e);
			ਜੇ (ਰੁੱਖ_content_ਸੈੱਟ(e, slash1 + 1, oid, ਬਹੁਲਕ, subtree)) {
				oidclr(&ਜੜ੍ਹ->versions[1].oid, the_repository->hash_algo);
				ਵਾਪਸ 1;
			}
			ਵਾਪਸ 0;
		}
	}

	ਜੇ (t->ਇੰਦਰਾਜ_ਗਿਣਤੀ == t->ਇੰਦਰਾਜ_ਸਮਰੱਥਾ)
		ਜੜ੍ਹ->ਰੁੱਖ = t = grow_ਰੁੱਖ_content(t, t->ਇੰਦਰਾਜ_ਗਿਣਤੀ);
	e = new_ਰੁੱਖ_ਇੰਦਰਾਜ();
	e->name = ਮੰਜ਼ਿਲ_atom(p, n);
	e->versions[0].ਬਹੁਲਕ = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->ਇੰਦਰਾਜ_ਗਿਣਤੀ++] = e;
	ਜੇ (*slash1) {
		e->ਰੁੱਖ = new_ਰੁੱਖ_content(8);
		e->versions[1].ਬਹੁਲਕ = S_IFDIR;
		ਰੁੱਖ_content_ਸੈੱਟ(e, slash1 + 1, oid, ਬਹੁਲਕ, subtree);
	} ਨਹੀਂਤਾਂ {
		e->ਰੁੱਖ = subtree;
		e->versions[1].ਬਹੁਲਕ = ਬਹੁਲਕ;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&ਜੜ੍ਹ->versions[1].oid, the_repository->hash_algo);
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ ਰੁੱਖ_content_remove(
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *ਜੜ੍ਹ,
	ਅਚਲ ਅੱਖਰ *p,
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *backup_ਪੱਤੀ,
	ਪੂਰਨ allow_ਜੜ੍ਹ)
{
	ਬਣਤਰ ਰੁੱਖ_content *t;
	ਅਚਲ ਅੱਖਰ *slash1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, n;
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	ਜੇ (!ਜੜ੍ਹ->ਰੁੱਖ)
		ਲੱਦੋ_ਰੁੱਖ(ਜੜ੍ਹ);

	ਜੇ (!*p && allow_ਜੜ੍ਹ) {
		e = ਜੜ੍ਹ;
		ਜਾਓ del_ਇੰਦਰਾਜ;
	}

	t = ਜੜ੍ਹ->ਰੁੱਖ;
	ਲਈ (i = 0; i < t->ਇੰਦਰਾਜ_ਗਿਣਤੀ; i++) {
		e = t->entries[i];
		ਜੇ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ਜੇ (*slash1 && !S_ISDIR(e->versions[1].ਬਹੁਲਕ))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				ਵਾਪਸ 1;
			ਜੇ (!*slash1 || !S_ISDIR(e->versions[1].ਬਹੁਲਕ))
				ਜਾਓ del_ਇੰਦਰਾਜ;
			ਜੇ (!e->ਰੁੱਖ)
				ਲੱਦੋ_ਰੁੱਖ(e);
			ਜੇ (ਰੁੱਖ_content_remove(e, slash1 + 1, backup_ਪੱਤੀ, 0)) {
				ਲਈ (n = 0; n < e->ਰੁੱਖ->ਇੰਦਰਾਜ_ਗਿਣਤੀ; n++) {
					ਜੇ (e->ਰੁੱਖ->entries[n]->versions[1].ਬਹੁਲਕ) {
						oidclr(&ਜੜ੍ਹ->versions[1].oid,
						       the_repository->hash_algo);
						ਵਾਪਸ 1;
					}
				}
				backup_ਪੱਤੀ = ਨਿਰਾ;
				ਜਾਓ del_ਇੰਦਰਾਜ;
			}
			ਵਾਪਸ 0;
		}
	}
	ਵਾਪਸ 0;

del_ਇੰਦਰਾਜ:
	ਜੇ (backup_ਪੱਤੀ)
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(backup_ਪੱਤੀ, e, ਆਕਾਰ(*backup_ਪੱਤੀ));
	ਨਹੀਂਤਾਂ ਜੇ (e->ਰੁੱਖ)
		release_ਰੁੱਖ_content_recursive(e->ਰੁੱਖ);
	e->ਰੁੱਖ = ਨਿਰਾ;
	e->versions[1].ਬਹੁਲਕ = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&ਜੜ੍ਹ->versions[1].oid, the_repository->hash_algo);
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ ਰੁੱਖ_content_get(
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *ਜੜ੍ਹ,
	ਅਚਲ ਅੱਖਰ *p,
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *ਪੱਤੀ,
	ਪੂਰਨ allow_ਜੜ੍ਹ)
{
	ਬਣਤਰ ਰੁੱਖ_content *t;
	ਅਚਲ ਅੱਖਰ *slash1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, n;
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	ਜੇ (!n && !allow_ਜੜ੍ਹ)
		die(_("empty path component found in input"));

	ਜੇ (!ਜੜ੍ਹ->ਰੁੱਖ)
		ਲੱਦੋ_ਰੁੱਖ(ਜੜ੍ਹ);

	ਜੇ (!n) {
		e = ਜੜ੍ਹ;
		ਜਾਓ ਮਿਲਿਆ_ਇੰਦਰਾਜ;
	}

	t = ਜੜ੍ਹ->ਰੁੱਖ;
	ਲਈ (i = 0; i < t->ਇੰਦਰਾਜ_ਗਿਣਤੀ; i++) {
		e = t->entries[i];
		ਜੇ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ਜੇ (!*slash1)
				ਜਾਓ ਮਿਲਿਆ_ਇੰਦਰਾਜ;
			ਜੇ (!S_ISDIR(e->versions[1].ਬਹੁਲਕ))
				ਵਾਪਸ 0;
			ਜੇ (!e->ਰੁੱਖ)
				ਲੱਦੋ_ਰੁੱਖ(e);
			ਵਾਪਸ ਰੁੱਖ_content_get(e, slash1 + 1, ਪੱਤੀ, 0);
		}
	}
	ਵਾਪਸ 0;

ਮਿਲਿਆ_ਇੰਦਰਾਜ:
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਪੱਤੀ, e, ਆਕਾਰ(*ਪੱਤੀ));
	ਜੇ (e->ਰੁੱਖ && is_null_oid(&e->versions[1].oid))
		ਪੱਤੀ->ਰੁੱਖ = dup_ਰੁੱਖ_content(e->ਰੁੱਖ);
	ਨਹੀਂਤਾਂ
		ਪੱਤੀ->ਰੁੱਖ = ਨਿਰਾ;
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ update_branch(ਬਣਤਰ branch *b)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ = "fast-import";
	ਬਣਤਰ ref_transaction *transaction;
	ਬਣਤਰ object_id old_oid;
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *replace_ਅਗੇਤਰ = "refs/replace/";

	ਜੇ (starts_with(b->name, replace_ਅਗੇਤਰ) &&
	    !strcmp(b->name + ਲੜੀ_ਲੰਬਾਈ(replace_ਅਗੇਤਰ),
		    oid_ਮੰਜ਼ਿਲ_hex(&b->oid))) {
		ਜੇ (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_ਮੰਜ਼ਿਲ_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				ਨਿਰਾ, b->name, ਨਿਰਾ, 0);
		ਵਾਪਸ 0;
	}
	ਜੇ (is_null_oid(&b->oid)) {
		ਜੇ (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					ਨਿਰਾ, b->name, ਨਿਰਾ, 0);
		ਵਾਪਸ 0;
	}
	ਜੇ (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	ਜੇ (!force_update && !is_null_oid(&old_oid)) {
		ਬਣਤਰ commit *old_cmit, *new_cmit;
		ਪੂਰਨ ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		ਜੇ (!old_cmit || !new_cmit)
			ਵਾਪਸ error(_("branch %s is missing commits."), b->name);

		ret = repo_ਵਿੱਚ_ਮਿਲਾਓ_bases(the_repository, old_cmit, new_cmit);
		ਜੇ (ret < 0)
			ਬਾਹਰ_ਨਿਕਲੋ(128);
		ਜੇ (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_ਮੰਜ਼ਿਲ_hex(&b->oid),
				oid_ਮੰਜ਼ਿਲ_hex(&old_oid));
			ਵਾਪਸ -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ਜੇ (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   ਨਿਰਾ, ਨਿਰਾ, 0, ਸੁਨੇਹਾ, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		ਵਾਪਸ -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ dump_branches(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
	ਬਣਤਰ branch *b;

	ਲਈ (i = 0; i < branch_ਸਾਰਣੀ_sz; i++) {
		ਲਈ (b = branch_ਸਾਰਣੀ[i]; b; b = b->ਸਾਰਣੀ_ਅਗਲਾ_branch)
			failure |= update_branch(b);
	}
}

ਸਥਿਰ ਖਾਲੀ dump_ਟੈਗ_ਸੂਚੀ(ਖਾਲੀ)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ = "fast-import";
	ਬਣਤਰ ਟੈਗ *t;
	ਬਣਤਰ strbuf ref_name = STRBUF_INIT;
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਬਣਤਰ ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ਜੇ (!transaction) {
		failure |= error("%s", err.buf);
		ਜਾਓ cleanup;
	}
	ਲਈ (t = ਪਹਿਲਾ_ਟੈਗ; t; t = t->ਅਗਲਾ_ਟੈਗ) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		ਜੇ (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ,
					   0, ਸੁਨੇਹਾ, &err)) {
			failure |= error("%s", err.buf);
			ਜਾਓ cleanup;
		}
	}
	ਜੇ (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

ਸਥਿਰ ਖਾਲੀ dump_marks(ਖਾਲੀ)
{
	ਬਣਤਰ ਤਾਲਾ_file mark_ਤਾਲਾ = ਤਾਲਾ_INIT;
	FILE *f;

	ਜੇ (!export_marks_file || (import_marks_file && !import_marks_file_ਪੂਰਾ))
		ਵਾਪਸ;

	ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		ਵਾਪਸ;
	}

	ਜੇ (hold_ਤਾਲਾ_file_for_update(&mark_ਤਾਲਾ, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		ਵਾਪਸ;
	}

	f = fdopen_ਤਾਲਾ_file(&mark_ਤਾਲਾ, "w");
	ਜੇ (!f) {
		ਪੂਰਨ saved_errno = errno;
		rollback_ਤਾਲਾ_file(&mark_ਤਾਲਾ);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		ਵਾਪਸ;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	ਜੇ (commit_ਤਾਲਾ_file(&mark_ਤਾਲਾ)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		ਵਾਪਸ;
	}
}

ਸਥਿਰ ਖਾਲੀ ਪਾਓ_object_ਇੰਦਰਾਜ(ਬਣਤਰ mark_ਸੈੱਟ **s, ਬਣਤਰ object_id *oid, uintmax_t mark)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *e;
	e = ਲੱਭੋ_object(oid);
	ਜੇ (!e) {
		ਗਿਣਤੀ object_type type = odb_read_object_info(the_repository->objects,
							     oid, ਨਿਰਾ);
		ਜੇ (type < 0)
			die(_("object not found: %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		e = ਪਾਓ_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.ਵਿੱਥ = 1; /* just not zero! */
	}
	ਪਾਓ_mark(s, mark, e);
}

ਸਥਿਰ ਖਾਲੀ ਪਾਓ_oid_ਇੰਦਰਾਜ(ਬਣਤਰ mark_ਸੈੱਟ **s, ਬਣਤਰ object_id *oid, uintmax_t mark)
{
	ਪਾਓ_mark(s, mark, xmemdupz(oid, ਆਕਾਰ(*oid)));
}

ਸਥਿਰ ਖਾਲੀ read_mark_file(ਬਣਤਰ mark_ਸੈੱਟ **s, FILE *f, mark_ਸੈੱਟ_inserter_t inserter)
{
	ਅੱਖਰ ਸਤਰ[512];
	ਜਦੋਂ (fgets(ਸਤਰ, ਆਕਾਰ(ਸਤਰ), f)) {
		uintmax_t mark;
		ਅੱਖਰ *ਸਿਰਾ;
		ਬਣਤਰ object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, ਆਕਾਰ(oid.hash));

		ਸਿਰਾ = strchr(ਸਤਰ, '\n');
		ਜੇ (ਸਤਰ[0] != ':' || !ਸਿਰਾ)
			die(_("corrupt mark line: %s"), ਸਤਰ);
		*ਸਿਰਾ = 0;
		mark = strtoumax(ਸਤਰ + 1, &ਸਿਰਾ, 10);
		ਜੇ (!mark || ਸਿਰਾ == ਸਤਰ + 1
			|| *ਸਿਰਾ != ' '
			|| get_oid_hex_any(ਸਿਰਾ + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), ਸਤਰ);
		inserter(s, &oid, mark);
	}
}

ਸਥਿਰ ਖਾਲੀ read_marks(ਖਾਲੀ)
{
	FILE *f = fopen(import_marks_file, "r");
	ਜੇ (f)
		;
	ਨਹੀਂਤਾਂ ਜੇ (import_marks_file_ignore_missing && errno == ENOENT)
		ਜਾਓ ਪੂਰਾ; /* Marks file does not exist */
	ਨਹੀਂਤਾਂ
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, ਪਾਓ_object_ਇੰਦਰਾਜ);
	fclose(f);
ਪੂਰਾ:
	import_marks_file_ਪੂਰਾ = 1;
}


ਸਥਿਰ ਪੂਰਨ read_ਅਗਲਾ_command(ਬਣਤਰ fast_import_state *state)
{
	ਸਥਿਰ ਪੂਰਨ stdin_eof = 0;

	ਜੇ (stdin_eof) {
		unread_command_buf = 0;
		ਵਾਪਸ EOF;
	}

	ਲਈ (;;) {
		ਜੇ (unread_command_buf) {
			unread_command_buf = 0;
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			ਜੇ (stdin_eof)
				ਵਾਪਸ EOF;

			ਜੇ (!state->ਵੇਖਿਆ_ਅੰਕੜੇ_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			ਜੇ (rc)
				rc_free = rc->ਅਗਲਾ;
			ਨਹੀਂਤਾਂ {
				rc = cmd_hist.ਅਗਲਾ;
				cmd_hist.ਅਗਲਾ = rc->ਅਗਲਾ;
				cmd_hist.ਅਗਲਾ->prev = &cmd_hist;
				ਮੁਕਤ(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_ਪੂਛ;
			rc->ਅਗਲਾ = cmd_hist.prev;
			rc->prev->ਅਗਲਾ = rc;
			cmd_ਪੂਛ = rc;
		}
		ਜੇ (command_buf.buf[0] == '#')
			ਜਾਰੀ;
		ਵਾਪਸ 0;
	}
}

ਸਥਿਰ ਖਾਲੀ skip_optional_lf(ਖਾਲੀ)
{
	ਪੂਰਨ ਪਦ_char = fgetc(stdin);
	ਜੇ (ਪਦ_char != '\n' && ਪਦ_char != EOF)
		ungetc(ਪਦ_char, stdin);
}

ਸਥਿਰ ਖਾਲੀ parse_mark(ਬਣਤਰ fast_import_state *state)
{
	ਅਚਲ ਅੱਖਰ *v;
	ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "mark :", &v)) {
		ਅਗਲਾ_mark = strtoumax(v, ਨਿਰਾ, 10);
		read_ਅਗਲਾ_command(state);
	}
	ਨਹੀਂਤਾਂ
		ਅਗਲਾ_mark = 0;
}

ਸਥਿਰ ਖਾਲੀ parse_original_identifier(ਬਣਤਰ fast_import_state *state)
{
	ਅਚਲ ਅੱਖਰ *v;
	ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "original-oid ", &v))
		read_ਅਗਲਾ_command(state);
}

ਸਥਿਰ ਪੂਰਨ parse_ਅੰਕੜੇ(ਬਣਤਰ strbuf *sb, uintmax_t ਹੱਦਬੰਦੀ, uintmax_t *len_res)
{
	ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ;
	strbuf_reset(sb);

	ਜੇ (!skip_ਅਗੇਤਰ(command_buf.buf, "data ", &ਅੰਕੜੇ))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	ਜੇ (skip_ਅਗੇਤਰ(ਅੰਕੜੇ, "<<", &ਅੰਕੜੇ)) {
		ਅੱਖਰ *ਪਦ = xstrdup(ਅੰਕੜੇ);
		ਮਾਪ_t ਪਦ_len = command_buf.len - (ਅੰਕੜੇ - command_buf.buf);

		ਲਈ (;;) {
			ਜੇ (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), ਪਦ);
			ਜੇ (ਪਦ_len == command_buf.len
				&& !strcmp(ਪਦ, command_buf.buf))
				ਰੋਕੋ;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		ਮੁਕਤ(ਪਦ);
	}
	ਨਹੀਂਤਾਂ {
		uintmax_t len = strtoumax(ਅੰਕੜੇ, ਨਿਰਾ, 10);
		ਮਾਪ_t n = 0, ਲੰਮਾਈ = (ਮਾਪ_t)len;

		ਜੇ (ਹੱਦਬੰਦੀ && ਹੱਦਬੰਦੀ < len) {
			*len_res = len;
			ਵਾਪਸ 0;
		}
		ਜੇ (ਲੰਮਾਈ < len)
			die(_("data is too large to use in this context"));

		ਜਦੋਂ (n < ਲੰਮਾਈ) {
			ਮਾਪ_t s = strbuf_fread(sb, ਲੰਮਾਈ - n, stdin);
			ਜੇ (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(ਲੰਮਾਈ - n));
			n += s;
		}
	}

	skip_optional_lf();
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ validate_raw_date(ਅਚਲ ਅੱਖਰ *src, ਬਣਤਰ strbuf *ਨਤੀਜਾ, ਪੂਰਨ strict)
{
	ਅਚਲ ਅੱਖਰ *orig_src = src;
	ਅੱਖਰ *endp;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅੰਕ;

	errno = 0;

	ਅੰਕ = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	ਜੇ (errno || endp == src || *endp != ' ')
		ਵਾਪਸ -1;

	src = endp + 1;
	ਜੇ (*src != '-' && *src != '+')
		ਵਾਪਸ -1;

	ਅੰਕ = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	ਜੇ (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < ਅੰਕ))             /* parsed a broken timezone */
	   )
		ਵਾਪਸ -1;

	strbuf_addstr(ਨਤੀਜਾ, orig_src);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅੱਖਰ *parse_ident(ਅਚਲ ਅੱਖਰ *buf)
{
	ਅਚਲ ਅੱਖਰ *ltgt;
	ਮਾਪ_t name_len;
	ਬਣਤਰ strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	ਜੇ (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	ਜੇ (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	ਜੇ (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	ਜੇ (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	ਜੇ (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_ਜੋੜੋ(&ident, buf, name_len);

	ਚੋਣ (whenspec) {
	ਹਾਲਤ WHENSPEC_RAW:
		ਜੇ (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		ਰੋਕੋ;
	ਹਾਲਤ WHENSPEC_RAW_PERMISSIVE:
		ਜੇ (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		ਰੋਕੋ;
	ਹਾਲਤ WHENSPEC_RFC2822:
		ਜੇ (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		ਰੋਕੋ;
	ਹਾਲਤ WHENSPEC_NOW:
		ਜੇ (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		ਰੋਕੋ;
	}

	ਵਾਪਸ strbuf_detach(&ident, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ parse_and_store_blob(
	ਬਣਤਰ ਆਖਰੀ_object *ਆਖਰੀ,
	ਬਣਤਰ object_id *oidout,
	uintmax_t mark)
{
	ਸਥਿਰ ਬਣਤਰ strbuf buf = STRBUF_INIT;
	uintmax_t len;

	ਜੇ (parse_ਅੰਕੜੇ(&buf, repo_settings_get_big_file_ਹੱਦ(the_repository), &len))
		store_object(OBJ_BLOB, &buf, ਆਖਰੀ, oidout, mark);
	ਨਹੀਂਤਾਂ {
		ਜੇ (ਆਖਰੀ) {
			strbuf_release(&ਆਖਰੀ->ਅੰਕੜੇ);
			ਆਖਰੀ->ਵਿੱਥ = 0;
			ਆਖਰੀ->ਡੂੰਘਾਈ = 0;
		}
		ਧਾਰਾ_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

ਸਥਿਰ ਖਾਲੀ parse_new_blob(ਬਣਤਰ fast_import_state *state)
{
	read_ਅਗਲਾ_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&ਆਖਰੀ_blob, ਨਿਰਾ, ਅਗਲਾ_mark);
}

ਸਥਿਰ ਖਾਲੀ unload_one_branch(ਖਾਲੀ)
{
	ਜਦੋਂ (cur_ਸਰਗਰਮ_branches
		&& cur_ਸਰਗਰਮ_branches >= max_ਸਰਗਰਮ_branches) {
		uintmax_t min_commit = ULONG_MAX;
		ਬਣਤਰ branch *e, *l = ਨਿਰਾ, *p = ਨਿਰਾ;

		ਲਈ (e = ਸਰਗਰਮ_branches; e; e = e->ਸਰਗਰਮ_ਅਗਲਾ_branch) {
			ਜੇ (e->ਆਖਰੀ_commit < min_commit) {
				p = l;
				min_commit = e->ਆਖਰੀ_commit;
			}
			l = e;
		}

		ਜੇ (p) {
			e = p->ਸਰਗਰਮ_ਅਗਲਾ_branch;
			p->ਸਰਗਰਮ_ਅਗਲਾ_branch = e->ਸਰਗਰਮ_ਅਗਲਾ_branch;
		} ਨਹੀਂਤਾਂ {
			e = ਸਰਗਰਮ_branches;
			ਸਰਗਰਮ_branches = e->ਸਰਗਰਮ_ਅਗਲਾ_branch;
		}
		e->ਸਰਗਰਮ = 0;
		e->ਸਰਗਰਮ_ਅਗਲਾ_branch = ਨਿਰਾ;
		ਜੇ (e->branch_ਰੁੱਖ.ਰੁੱਖ) {
			release_ਰੁੱਖ_content_recursive(e->branch_ਰੁੱਖ.ਰੁੱਖ);
			e->branch_ਰੁੱਖ.ਰੁੱਖ = ਨਿਰਾ;
		}
		cur_ਸਰਗਰਮ_branches--;
	}
}

ਸਥਿਰ ਖਾਲੀ ਲੱਦੋ_branch(ਬਣਤਰ branch *b)
{
	ਲੱਦੋ_ਰੁੱਖ(&b->branch_ਰੁੱਖ);
	ਜੇ (!b->ਸਰਗਰਮ) {
		b->ਸਰਗਰਮ = 1;
		b->ਸਰਗਰਮ_ਅਗਲਾ_branch = ਸਰਗਰਮ_branches;
		ਸਰਗਰਮ_branches = b;
		cur_ਸਰਗਰਮ_branches++;
		branch_ਲੱਦੋ_ਗਿਣਤੀ++;
	}
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ convert_ਅੰਕ_notes_ਮੰਜ਼ਿਲ_fanout(uintmax_t ਅੰਕ_notes)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ fanout = 0;
	ਜਦੋਂ ((ਅੰਕ_notes >>= 8))
		fanout++;
	ਵਾਪਸ fanout;
}

ਸਥਿਰ ਖਾਲੀ construct_ਰਸਤਾ_with_fanout(ਅਚਲ ਅੱਖਰ *hex_sha1,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ fanout, ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0, j = 0;
	ਜੇ (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	ਜਦੋਂ (fanout) {
		ਰਸਤਾ[i++] = hex_sha1[j++];
		ਰਸਤਾ[i++] = hex_sha1[j++];
		ਰਸਤਾ[i++] = '/';
		fanout--;
	}
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਰਸਤਾ + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	ਰਸਤਾ[i + the_hash_algo->hexsz - j] = '\0';
}

ਸਥਿਰ uintmax_t do_change_note_fanout(
		ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *orig_ਜੜ੍ਹ, ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *ਜੜ੍ਹ,
		ਅੱਖਰ *hex_oid, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hex_oid_len,
		ਅੱਖਰ *fullpath, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fullpath_len,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ fanout)
{
	ਬਣਤਰ ਰੁੱਖ_content *t;
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *e, ਪੱਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t ਅੰਕ_notes = 0;
	ਬਣਤਰ object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	ਅੱਖਰ realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ hexsz = the_hash_algo->hexsz;

	ਜੇ (!ਜੜ੍ਹ->ਰੁੱਖ)
		ਲੱਦੋ_ਰੁੱਖ(ਜੜ੍ਹ);
	t = ਜੜ੍ਹ->ਰੁੱਖ;

	ਲਈ (i = 0; t && i < t->ਇੰਦਰਾਜ_ਗਿਣਤੀ; i++) {
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
		ਜੇ (!e->versions[1].ਬਹੁਲਕ ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			ਜਾਰੀ;

		/* This _may_ be a note entry, or a subdir containing notes */
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		ਜੇ (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		ਜੇ (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			ਜੇ (fanout == 0xff) {
				/* Counting mode, no rename */
				ਅੰਕ_notes++;
				ਜਾਰੀ;
			}
			construct_ਰਸਤਾ_with_fanout(hex_oid, fanout, realpath);
			ਜੇ (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				ਅੰਕ_notes++;
				ਜਾਰੀ;
			}

			/* Rename fullpath to realpath */
			ਜੇ (!ਰੁੱਖ_content_remove(orig_ਜੜ੍ਹ, fullpath, &ਪੱਤੀ, 0))
				die(_("failed to remove path %s"), fullpath);
			ਰੁੱਖ_content_ਸੈੱਟ(orig_ਜੜ੍ਹ, realpath,
				&ਪੱਤੀ.versions[1].oid,
				ਪੱਤੀ.versions[1].ਬਹੁਲਕ,
				ਪੱਤੀ.ਰੁੱਖ);
		} ਨਹੀਂਤਾਂ ਜੇ (S_ISDIR(e->versions[1].ਬਹੁਲਕ)) {
			/* This is a subdir that may contain note entries */
			ਅੰਕ_notes += do_change_note_fanout(orig_ਜੜ੍ਹ, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = ਜੜ੍ਹ->ਰੁੱਖ;
	}
	ਵਾਪਸ ਅੰਕ_notes;
}

ਸਥਿਰ uintmax_t change_note_fanout(ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *ਜੜ੍ਹ,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	ਅੱਖਰ hex_oid[GIT_MAX_HEXSZ], ਰਸਤਾ[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	ਵਾਪਸ do_change_note_fanout(ਜੜ੍ਹ, ਜੜ੍ਹ, hex_oid, 0, ਰਸਤਾ, 0, fanout);
}

ਸਥਿਰ ਪੂਰਨ parse_mapped_oid_hex(ਅਚਲ ਅੱਖਰ *hex, ਬਣਤਰ object_id *oid, ਅਚਲ ਅੱਖਰ **ਸਿਰਾ)
{
	ਪੂਰਨ algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, ਆਕਾਰ(oid->hash));

	algo = parse_oid_hex_any(hex, oid, ਸਿਰਾ);
	ਜੇ (algo == GIT_HASH_UNKNOWN)
		ਵਾਪਸ -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	ਜੇ (it == kh_ਸਿਰਾ(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		ਜੇ (hash_algos[algo].format_id == the_hash_algo->format_id)
			ਵਾਪਸ 0;
		ਵਾਪਸ -1;
	}
	oidcpy(oid, kh_ਮੁੱਲ(sub_oid_map, it));
	ਵਾਪਸ 0;
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
ਸਥਿਰ uintmax_t parse_mark_ref(ਅਚਲ ਅੱਖਰ *p, ਅੱਖਰ **endptr)
{
	uintmax_t mark;

	ਪ੍ਰਤਿਜ੍ਞਾ(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	ਜੇ (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	ਵਾਪਸ mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
ਸਥਿਰ uintmax_t parse_mark_ref_eol(ਅਚਲ ਅੱਖਰ *p)
{
	ਅੱਖਰ *ਸਿਰਾ;
	uintmax_t mark;

	mark = parse_mark_ref(p, &ਸਿਰਾ);
	ਜੇ (*ਸਿਰਾ != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	ਵਾਪਸ mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
ਸਥਿਰ uintmax_t parse_mark_ref_space(ਅਚਲ ਅੱਖਰ **p)
{
	uintmax_t mark;
	ਅੱਖਰ *ਸਿਰਾ;

	mark = parse_mark_ref(*p, &ਸਿਰਾ);
	ਜੇ (*ਸਿਰਾ++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = ਸਿਰਾ;
	ਵਾਪਸ mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
ਸਥਿਰ ਖਾਲੀ parse_ਰਸਤਾ(ਬਣਤਰ strbuf *sb, ਅਚਲ ਅੱਖਰ *p, ਅਚਲ ਅੱਖਰ **endp,
		ਪੂਰਨ is_ਆਖਰੀ_ਖੇਤਰ, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ)
{
	ਜੇ (*p == '"') {
		ਜੇ (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), ਖੇਤਰ, command_buf.buf);
		ਜੇ (ਲੜੀ_ਲੰਬਾਈ(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), ਖੇਤਰ, command_buf.buf);
	} ਨਹੀਂਤਾਂ {
		/*
		 * Unless we are parsing the last field of a line,
		 * SP is the end of this field.
		 */
		*endp = is_ਆਖਰੀ_ਖੇਤਰ
			? p + ਲੜੀ_ਲੰਬਾਈ(p)
			: strchrnul(p, ' ');
		strbuf_ਜੋੜੋ(sb, p, *endp - p);
	}
}

/*
 * Parse the path string into the strbuf, and complain if this is not the end of
 * the string. Unquoted strings may contain spaces.
 */
ਸਥਿਰ ਖਾਲੀ parse_ਰਸਤਾ_eol(ਬਣਤਰ strbuf *sb, ਅਚਲ ਅੱਖਰ *p, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ)
{
	ਅਚਲ ਅੱਖਰ *ਸਿਰਾ;

	parse_ਰਸਤਾ(sb, p, &ਸਿਰਾ, 1, ਖੇਤਰ);
	ਜੇ (*ਸਿਰਾ)
		die(_("garbage after %s: %s"), ਖੇਤਰ, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
ਸਥਿਰ ਖਾਲੀ parse_ਰਸਤਾ_space(ਬਣਤਰ strbuf *sb, ਅਚਲ ਅੱਖਰ *p,
		ਅਚਲ ਅੱਖਰ **endp, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ)
{
	parse_ਰਸਤਾ(sb, p, endp, 0, ਖੇਤਰ);
	ਜੇ (**endp != ' ')
		die(_("missing space after %s: %s"), ਖੇਤਰ, command_buf.buf);
	(*endp)++;
}

ਸਥਿਰ ਖਾਲੀ file_change_m(ਬਣਤਰ fast_import_state *state,
			  ਅਚਲ ਅੱਖਰ *p,
			  ਬਣਤਰ branch *b)
{
	ਸਥਿਰ ਬਣਤਰ strbuf ਰਸਤਾ = STRBUF_INIT;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *oe;
	ਬਣਤਰ object_id oid;
	uint16_t ਬਹੁਲਕ, inline_ਅੰਕੜੇ = 0;

	p = parse_ਬਹੁਲਕ(p, &ਬਹੁਲਕ);
	ਜੇ (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	ਚੋਣ (ਬਹੁਲਕ) {
	ਹਾਲਤ 0644:
	ਹਾਲਤ 0755:
		ਬਹੁਲਕ |= S_IFREG;
	ਹਾਲਤ S_IFREG | 0644:
	ਹਾਲਤ S_IFREG | 0755:
	ਹਾਲਤ S_IFLNK:
	ਹਾਲਤ S_IFDIR:
	ਹਾਲਤ S_IFGITLINK:
		/* ok */
		ਰੋਕੋ;
	ਮੂਲ:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	ਜੇ (*p == ':') {
		oe = ਲੱਭੋ_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(p, "inline ", &p)) {
		inline_ਅੰਕੜੇ = 1;
		oe = ਨਿਰਾ; /* not used with inline_data, but makes gcc happy */
	} ਨਹੀਂਤਾਂ {
		ਜੇ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = ਲੱਭੋ_object(&oid);
		ਜੇ (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&ਰਸਤਾ);
	parse_ਰਸਤਾ_eol(&ਰਸਤਾ, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	ਜੇ (S_ISDIR(ਬਹੁਲਕ) &&
	    is_ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ_oid(&oid, the_repository->hash_algo) &&
	    *ਰਸਤਾ.buf) {
		ਰੁੱਖ_content_remove(&b->branch_ਰੁੱਖ, ਰਸਤਾ.buf, ਨਿਰਾ, 0);
		ਵਾਪਸ;
	}

	ਜੇ (S_ISGITLINK(ਬਹੁਲਕ)) {
		ਜੇ (inline_ਅੰਕੜੇ)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		ਨਹੀਂਤਾਂ ਜੇ (oe) {
			ਜੇ (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} ਨਹੀਂਤਾਂ ਜੇ (inline_ਅੰਕੜੇ) {
		ਜੇ (S_ISDIR(ਬਹੁਲਕ))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		ਜਦੋਂ (read_ਅਗਲਾ_command(state) != EOF) {
			ਅਚਲ ਅੱਖਰ *v;
			ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			ਨਹੀਂਤਾਂ {
				parse_and_store_blob(&ਆਖਰੀ_blob, &oid, 0);
				ਰੋਕੋ;
			}
		}
	} ਨਹੀਂਤਾਂ {
		ਗਿਣਤੀ object_type expected = S_ISDIR(ਬਹੁਲਕ) ?
						OBJ_ਰੁੱਖ: OBJ_BLOB;
		ਗਿਣਤੀ object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, ਨਿਰਾ);
		ਜੇ (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(ਬਹੁਲਕ) ?  _("tree") : _("blob"),
			    command_buf.buf);
		ਜੇ (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	ਜੇ (!*ਰਸਤਾ.buf) {
		ਰੁੱਖ_content_replace(&b->branch_ਰੁੱਖ, &oid, ਬਹੁਲਕ, ਨਿਰਾ);
		ਵਾਪਸ;
	}

	ਜੇ (!verify_ਰਸਤਾ(ਰਸਤਾ.buf, ਬਹੁਲਕ))
		die(_("invalid path '%s'"), ਰਸਤਾ.buf);
	ਰੁੱਖ_content_ਸੈੱਟ(&b->branch_ਰੁੱਖ, ਰਸਤਾ.buf, &oid, ਬਹੁਲਕ, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ file_change_d(ਅਚਲ ਅੱਖਰ *p, ਬਣਤਰ branch *b)
{
	ਸਥਿਰ ਬਣਤਰ strbuf ਰਸਤਾ = STRBUF_INIT;

	strbuf_reset(&ਰਸਤਾ);
	parse_ਰਸਤਾ_eol(&ਰਸਤਾ, p, "path");
	ਰੁੱਖ_content_remove(&b->branch_ਰੁੱਖ, ਰਸਤਾ.buf, ਨਿਰਾ, 1);
}

ਸਥਿਰ ਖਾਲੀ file_change_cr(ਅਚਲ ਅੱਖਰ *p, ਬਣਤਰ branch *b, ਪੂਰਨ rename)
{
	ਸਥਿਰ ਬਣਤਰ strbuf ਸਰੋਤ = STRBUF_INIT;
	ਸਥਿਰ ਬਣਤਰ strbuf dest = STRBUF_INIT;
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ ਪੱਤੀ;

	strbuf_reset(&ਸਰੋਤ);
	parse_ਰਸਤਾ_space(&ਸਰੋਤ, p, &p, "source");
	strbuf_reset(&dest);
	parse_ਰਸਤਾ_eol(&dest, p, "dest");

	memset(&ਪੱਤੀ, 0, ਆਕਾਰ(ਪੱਤੀ));
	ਜੇ (rename)
		ਰੁੱਖ_content_remove(&b->branch_ਰੁੱਖ, ਸਰੋਤ.buf, &ਪੱਤੀ, 1);
	ਨਹੀਂਤਾਂ
		ਰੁੱਖ_content_get(&b->branch_ਰੁੱਖ, ਸਰੋਤ.buf, &ਪੱਤੀ, 1);
	ਜੇ (!ਪੱਤੀ.versions[1].ਬਹੁਲਕ)
		die(_("path %s not in branch"), ਸਰੋਤ.buf);
	ਜੇ (!*dest.buf) {	/* C "path/to/subdir" "" */
		ਰੁੱਖ_content_replace(&b->branch_ਰੁੱਖ,
			&ਪੱਤੀ.versions[1].oid,
			ਪੱਤੀ.versions[1].ਬਹੁਲਕ,
			ਪੱਤੀ.ਰੁੱਖ);
		ਵਾਪਸ;
	}
	ਜੇ (!verify_ਰਸਤਾ(dest.buf, ਪੱਤੀ.versions[1].ਬਹੁਲਕ))
		die(_("invalid path '%s'"), dest.buf);
	ਰੁੱਖ_content_ਸੈੱਟ(&b->branch_ਰੁੱਖ, dest.buf,
		&ਪੱਤੀ.versions[1].oid,
		ਪੱਤੀ.versions[1].ਬਹੁਲਕ,
		ਪੱਤੀ.ਰੁੱਖ);
}

ਸਥਿਰ ਖਾਲੀ note_change_n(ਬਣਤਰ fast_import_state *state,
			  ਅਚਲ ਅੱਖਰ *p,
			  ਬਣਤਰ branch *b,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *old_fanout)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *oe;
	ਬਣਤਰ branch *s;
	ਬਣਤਰ object_id oid, commit_oid;
	ਅੱਖਰ ਰਸਤਾ[GIT_MAX_RAWSZ * 3];
	uint16_t inline_ਅੰਕੜੇ = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ new_fanout;

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
	ਜੇ (b->ਅੰਕ_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->ਅੰਕ_notes = change_note_fanout(&b->branch_ਰੁੱਖ, 0xff);
		*old_fanout = convert_ਅੰਕ_notes_ਮੰਜ਼ਿਲ_fanout(b->ਅੰਕ_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	ਜੇ (*p == ':') {
		oe = ਲੱਭੋ_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(p, "inline ", &p)) {
		inline_ਅੰਕੜੇ = 1;
		oe = ਨਿਰਾ; /* not used with inline_data, but makes gcc happy */
	} ਨਹੀਂਤਾਂ {
		ਜੇ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = ਲੱਭੋ_object(&oid);
		ਜੇ (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	ਜੇ (s) {
		ਜੇ (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} ਨਹੀਂਤਾਂ ਜੇ (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		ਬਣਤਰ object_ਇੰਦਰਾਜ *commit_oe = ਲੱਭੋ_mark(marks, commit_mark);
		ਜੇ (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} ਨਹੀਂਤਾਂ ਜੇ (!repo_get_oid(the_repository, p, &commit_oid)) {
		ਮਾਪ_t ਮਾਪ;
		ਅੱਖਰ *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &ਮਾਪ,
						   &commit_oid);
		ਜੇ (!buf || ਮਾਪ < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		ਮੁਕਤ(buf);
	} ਨਹੀਂਤਾਂ
		die(_("invalid ref name or SHA1 expression: %s"), p);

	ਜੇ (inline_ਅੰਕੜੇ) {
		read_ਅਗਲਾ_command(state);
		parse_and_store_blob(&ਆਖਰੀ_blob, &oid, 0);
	} ਨਹੀਂਤਾਂ ਜੇ (oe) {
		ਜੇ (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} ਨਹੀਂਤਾਂ ਜੇ (!is_null_oid(&oid)) {
		ਗਿਣਤੀ object_type type = odb_read_object_info(the_repository->objects, &oid,
							ਨਿਰਾ);
		ਜੇ (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		ਜੇ (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_ਰਸਤਾ_with_fanout(oid_ਮੰਜ਼ਿਲ_hex(&commit_oid), *old_fanout, ਰਸਤਾ);
	ਜੇ (ਰੁੱਖ_content_remove(&b->branch_ਰੁੱਖ, ਰਸਤਾ, ਨਿਰਾ, 0))
		b->ਅੰਕ_notes--;

	ਜੇ (is_null_oid(&oid))
		ਵਾਪਸ; /* nothing to insert */

	b->ਅੰਕ_notes++;
	new_fanout = convert_ਅੰਕ_notes_ਮੰਜ਼ਿਲ_fanout(b->ਅੰਕ_notes);
	construct_ਰਸਤਾ_with_fanout(oid_ਮੰਜ਼ਿਲ_hex(&commit_oid), new_fanout, ਰਸਤਾ);
	ਰੁੱਖ_content_ਸੈੱਟ(&b->branch_ਰੁੱਖ, ਰਸਤਾ, &oid, S_IFREG | 0644, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ file_change_deleteall(ਬਣਤਰ branch *b)
{
	release_ਰੁੱਖ_content_recursive(b->branch_ਰੁੱਖ.ਰੁੱਖ);
	oidclr(&b->branch_ਰੁੱਖ.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_ਰੁੱਖ.versions[1].oid, the_repository->hash_algo);
	ਲੱਦੋ_ਰੁੱਖ(&b->branch_ਰੁੱਖ);
	b->ਅੰਕ_notes = 0;
}

ਸਥਿਰ ਖਾਲੀ parse_from_commit(ਬਣਤਰ branch *b, ਅੱਖਰ *buf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ)
{
	ਜੇ (!buf || ਮਾਪ < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_ਮੰਜ਼ਿਲ_hex(&b->oid));
	ਜੇ (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_ਰੁੱਖ.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_ਮੰਜ਼ਿਲ_hex(&b->oid));
	oidcpy(&b->branch_ਰੁੱਖ.versions[0].oid,
	       &b->branch_ਰੁੱਖ.versions[1].oid);
}

ਸਥਿਰ ਖਾਲੀ parse_from_existing(ਬਣਤਰ branch *b)
{
	ਜੇ (is_null_oid(&b->oid)) {
		oidclr(&b->branch_ਰੁੱਖ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_ਰੁੱਖ.versions[1].oid, the_repository->hash_algo);
	} ਨਹੀਂਤਾਂ {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;
		ਮਾਪ_t ਮਾਪ_st = 0;
		ਅੱਖਰ *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &ਮਾਪ_st, &b->oid);
		ਮਾਪ = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(ਮਾਪ_st);
		parse_from_commit(b, buf, ਮਾਪ);
		ਮੁਕਤ(buf);
	}
}

ਸਥਿਰ ਪੂਰਨ parse_objectish(ਬਣਤਰ fast_import_state *state,
			   ਬਣਤਰ branch *b,
			   ਅਚਲ ਅੱਖਰ *objectish)
{
	ਬਣਤਰ branch *s;
	ਬਣਤਰ object_id oid;

	oidcpy(&oid, &b->branch_ਰੁੱਖ.versions[1].oid);

	s = lookup_branch(objectish);
	ਜੇ (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	ਨਹੀਂਤਾਂ ਜੇ (s) {
		ਬਣਤਰ object_id *t = &s->branch_ਰੁੱਖ.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_ਰੁੱਖ.versions[0].oid, t);
		oidcpy(&b->branch_ਰੁੱਖ.versions[1].oid, t);
	} ਨਹੀਂਤਾਂ ਜੇ (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		ਬਣਤਰ object_ਇੰਦਰਾਜ *oe = ਲੱਭੋ_mark(marks, idnum);
		ਜੇ (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		ਜੇ (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			ਜੇ (oe->pack_id != MAX_PACK_ID) {
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;
				ਅੱਖਰ *buf = gfi_unpack_ਇੰਦਰਾਜ(oe, &ਮਾਪ);
				parse_from_commit(b, buf, ਮਾਪ);
				ਮੁਕਤ(buf);
			} ਨਹੀਂਤਾਂ
				parse_from_existing(b);
		}
	} ਨਹੀਂਤਾਂ ਜੇ (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		ਜੇ (is_null_oid(&b->oid))
			b->delete = 1;
	}
	ਨਹੀਂਤਾਂ
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	ਜੇ (b->branch_ਰੁੱਖ.ਰੁੱਖ && !oideq(&oid, &b->branch_ਰੁੱਖ.versions[1].oid)) {
		release_ਰੁੱਖ_content_recursive(b->branch_ਰੁੱਖ.ਰੁੱਖ);
		b->branch_ਰੁੱਖ.ਰੁੱਖ = ਨਿਰਾ;
	}

	read_ਅਗਲਾ_command(state);
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ parse_from(ਬਣਤਰ fast_import_state *state, ਬਣਤਰ branch *b)
{
	ਅਚਲ ਅੱਖਰ *from;

	ਜੇ (!skip_ਅਗੇਤਰ(command_buf.buf, "from ", &from))
		ਵਾਪਸ 0;

	ਵਾਪਸ parse_objectish(state, b, from);
}

ਸਥਿਰ ਪੂਰਨ parse_objectish_with_ਅਗੇਤਰ(ਬਣਤਰ fast_import_state *state,
				       ਬਣਤਰ branch *b,
				       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਅਚਲ ਅੱਖਰ *ਅਧਾਰ;

	ਜੇ (!skip_ਅਗੇਤਰ(command_buf.buf, ਅਗੇਤਰ, &ਅਧਾਰ))
		ਵਾਪਸ 0;

	ਵਾਪਸ parse_objectish(state, b, ਅਧਾਰ);
}

ਸਥਿਰ ਬਣਤਰ hash_list *parse_ਮਿਲਾਓ(ਬਣਤਰ fast_import_state *state,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਗਿਣਤੀ_2)
{
	ਬਣਤਰ hash_list *list = ਨਿਰਾ, **ਪੂਛ = &list, *n;
	ਅਚਲ ਅੱਖਰ *from;
	ਬਣਤਰ branch *s;

	*ਗਿਣਤੀ_2 = 0;
	ਜਦੋਂ (skip_ਅਗੇਤਰ(command_buf.buf, "merge ", &from)) {
		n = xmalloc(ਆਕਾਰ(*n));
		s = lookup_branch(from);
		ਜੇ (s)
			oidcpy(&n->oid, &s->oid);
		ਨਹੀਂਤਾਂ ਜੇ (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			ਬਣਤਰ object_ਇੰਦਰਾਜ *oe = ਲੱਭੋ_mark(marks, idnum);
			ਜੇ (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} ਨਹੀਂਤਾਂ ਜੇ (!repo_get_oid(the_repository, from, &n->oid)) {
			ਮਾਪ_t ਮਾਪ;
			ਅੱਖਰ *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &ਮਾਪ, &n->oid);
			ਜੇ (!buf || ਮਾਪ < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			ਮੁਕਤ(buf);
		} ਨਹੀਂਤਾਂ
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->ਅਗਲਾ = ਨਿਰਾ;
		*ਪੂਛ = n;
		ਪੂਛ = &n->ਅਗਲਾ;

		(*ਗਿਣਤੀ_2)++;
		read_ਅਗਲਾ_command(state);
	}
	ਵਾਪਸ list;
}

ਬਣਤਰ signature_ਅੰਕੜੇ {
	ਅੱਖਰ *hash_algo;      /* "sha1" or "sha256" */
	ਅੱਖਰ *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	ਬਣਤਰ strbuf ਅੰਕੜੇ;   /* The actual signature data */
};

ਸਥਿਰ ਖਾਲੀ parse_one_signature(ਬਣਤਰ fast_import_state *state,
				ਬਣਤਰ signature_ਅੰਕੜੇ *sig,
				ਅਚਲ ਅੱਖਰ *v)
{
	ਅੱਖਰ *ਦਲੀਲਾਂ = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	ਅੱਖਰ *space = strchr(ਦਲੀਲਾਂ, ' ');

	ਜੇ (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), ਦਲੀਲਾਂ);
	*space = '\0';

	sig->hash_algo = ਦਲੀਲਾਂ;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	ਜੇ (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	ਜੇ (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	ਜੇ (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_ਅਗਲਾ_command(state);
	parse_ਅੰਕੜੇ(&sig->ਅੰਕੜੇ, 0, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ discard_one_signature(ਬਣਤਰ fast_import_state *state)
{
	ਬਣਤਰ strbuf ਅੰਕੜੇ = STRBUF_INIT;

	read_ਅਗਲਾ_command(state);
	parse_ਅੰਕੜੇ(&ਅੰਕੜੇ, 0, ਨਿਰਾ);
	strbuf_release(&ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_gpgsig_ਮੰਜ਼ਿਲ_commit(ਬਣਤਰ strbuf *commit_ਅੰਕੜੇ,
				 ਅਚਲ ਅੱਖਰ *ਸਿਰਲੇਖ,
				 ਬਣਤਰ signature_ਅੰਕੜੇ *sig)
{
	ਬਣਤਰ string_list siglines = STRING_LIST_INIT_NODUP;

	ਜੇ (!sig || !sig->hash_algo)
		ਵਾਪਸ;

	strbuf_addstr(commit_ਅੰਕੜੇ, ਸਿਰਲੇਖ);
	string_list_ਵੰਡੋ_ਵਿੱਚ_ਰੱਖੋ(&siglines, sig->ਅੰਕੜੇ.buf, "\n", -1);
	strbuf_ਜੋੜੋ_separated_string_list(commit_ਅੰਕੜੇ, "\n ", &siglines);
	strbuf_addch(commit_ਅੰਕੜੇ, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->ਅੰਕੜੇ);
	ਮੁਕਤ(sig->hash_algo);
}

ਸਥਿਰ ਖਾਲੀ store_signature(ਬਣਤਰ signature_ਅੰਕੜੇ *stored_sig,
			    ਬਣਤਰ signature_ਅੰਕੜੇ *new_sig,
			    ਅਚਲ ਅੱਖਰ *hash_type)
{
	ਜੇ (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->ਅੰਕੜੇ);
		ਮੁਕਤ(new_sig->hash_algo);
	} ਨਹੀਂਤਾਂ {
		*stored_sig = *new_sig;
	}
}

ਸਥਿਰ ਖਾਲੀ import_one_signature(ਬਣਤਰ fast_import_state *state,
				 ਬਣਤਰ signature_ਅੰਕੜੇ *sig_sha1,
				 ਬਣਤਰ signature_ਅੰਕੜੇ *sig_sha256,
				 ਅਚਲ ਅੱਖਰ *v)
{
	ਬਣਤਰ signature_ਅੰਕੜੇ sig = { ਨਿਰਾ, ਨਿਰਾ, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	ਜੇ (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	ਨਹੀਂਤਾਂ
		die(_("parse_one_signature() returned unknown hash algo"));
}

ਸਥਿਰ ਖਾਲੀ finalize_commit_ਬਫਰ(ਬਣਤਰ strbuf *new_ਅੰਕੜੇ,
				   ਬਣਤਰ signature_ਅੰਕੜੇ *sig_sha1,
				   ਬਣਤਰ signature_ਅੰਕੜੇ *sig_sha256,
				   ਬਣਤਰ strbuf *ਸੁਨੇਹਾ)
{
	ਜੋੜੋ_gpgsig_ਮੰਜ਼ਿਲ_commit(new_ਅੰਕੜੇ, "gpgsig ", sig_sha1);
	ਜੋੜੋ_gpgsig_ਮੰਜ਼ਿਲ_commit(new_ਅੰਕੜੇ, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_ਅੰਕੜੇ, '\n');
	strbuf_addbuf(new_ਅੰਕੜੇ, ਸੁਨੇਹਾ);
}

ਸਥਿਰ ਖਾਲੀ warn_invalid_signature(ਬਣਤਰ signature_check *check,
				   ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਗਿਣਤੀ sign_ਬਹੁਲਕ ਬਹੁਲਕ)
{
	ਅਚਲ ਅੱਖਰ *signer = check->signer ? check->signer : _("unknown");
	ਅਚਲ ਅੱਖਰ *subject;
	ਪੂਰਨ subject_len = ਲੱਭੋ_commit_subject(ਸੁਨੇਹਾ, &subject);

	ਚੋਣ (ਬਹੁਲਕ) {
	ਹਾਲਤ SIGN_STRIP_IF_INVALID:
		ਜੇ (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		ਨਹੀਂਤਾਂ ਜੇ (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		ਨਹੀਂਤਾਂ
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		ਰੋਕੋ;
	ਹਾਲਤ SIGN_SIGN_IF_INVALID:
		ਜੇ (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		ਨਹੀਂਤਾਂ ਜੇ (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		ਨਹੀਂਤਾਂ
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("unsupported signing mode");
	}
}

ਸਥਿਰ ਖਾਲੀ handle_signature_if_invalid(ਬਣਤਰ strbuf *new_ਅੰਕੜੇ,
					ਬਣਤਰ signature_ਅੰਕੜੇ *sig_sha1,
					ਬਣਤਰ signature_ਅੰਕੜੇ *sig_sha256,
					ਬਣਤਰ strbuf *ਸੁਨੇਹਾ,
					ਗਿਣਤੀ sign_ਬਹੁਲਕ ਬਹੁਲਕ)
{
	ਬਣਤਰ strbuf tmp_buf = STRBUF_INIT;
	ਬਣਤਰ signature_check signature_check = { 0 };
	ਪੂਰਨ ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_ਅੰਕੜੇ);
	finalize_commit_ਬਫਰ(&tmp_buf, sig_sha1, sig_sha256, ਸੁਨੇਹਾ);
	ret = verify_commit_ਬਫਰ(tmp_buf.buf, tmp_buf.len, &signature_check);

	ਜੇ (ret) {
		ਜੇ (ਬਹੁਲਕ == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, ਸੁਨੇਹਾ->buf, ਬਹੁਲਕ);

		ਜੇ (ਬਹੁਲਕ == SIGN_SIGN_IF_INVALID) {
			ਬਣਤਰ strbuf signature = STRBUF_INIT;
			ਬਣਤਰ strbuf payload = STRBUF_INIT;

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
			ਜੇ (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			ਜੇ (sign_ਬਫਰ(&payload, &signature, signed_commit_keyid,
					SIGN_ਬਫਰ_USE_DEFAULT_ਕੁੰਜੀ))
				die(_("failed to sign commit object"));
			ਜੋੜੋ_ਸਿਰਲੇਖ_signature(new_ਅੰਕੜੇ, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_ਬਫਰ(new_ਅੰਕੜੇ, ਨਿਰਾ, ਨਿਰਾ, ਸੁਨੇਹਾ);
	} ਨਹੀਂਤਾਂ {
		strbuf_ਅਦਲਾ_ਬਦਲੀ(new_ਅੰਕੜੇ, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

ਸਥਿਰ ਖਾਲੀ parse_new_commit(ਬਣਤਰ fast_import_state *state, ਅਚਲ ਅੱਖਰ *arg)
{
	ਸਥਿਰ ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ signature_ਅੰਕੜੇ sig_sha1 = { ਨਿਰਾ, ਨਿਰਾ, STRBUF_INIT };
	ਬਣਤਰ signature_ਅੰਕੜੇ sig_sha256 = { ਨਿਰਾ, ਨਿਰਾ, STRBUF_INIT };
	ਬਣਤਰ branch *b;
	ਅੱਖਰ *author = ਨਿਰਾ;
	ਅੱਖਰ *committer = ਨਿਰਾ;
	ਅੱਖਰ *encoding = ਨਿਰਾ;
	ਬਣਤਰ hash_list *ਮਿਲਾਓ_list = ਨਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਿਲਾਓ_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ prev_fanout, new_fanout;
	ਅਚਲ ਅੱਖਰ *v;

	b = lookup_branch(arg);
	ਜੇ (!b)
		b = new_branch(arg);

	read_ਅਗਲਾ_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_ਅਗਲਾ_command(state);
	}
	ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_ਅਗਲਾ_command(state);
	}
	ਜੇ (!committer)
		die(_("expected committer but didn't get one"));

	ਜਦੋਂ (skip_ਅਗੇਤਰ(command_buf.buf, "gpgsig ", &v)) {
		ਚੋਣ (signed_commit_ਬਹੁਲਕ) {

		/* First, modes that don't need the signature to be parsed */
		ਹਾਲਤ SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		ਹਾਲਤ SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		ਹਾਲਤ SIGN_STRIP:
			discard_one_signature(state);
			ਰੋਕੋ;

		/* Second, modes that parse the signature */
		ਹਾਲਤ SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		ਹਾਲਤ SIGN_VERBATIM:
		ਹਾਲਤ SIGN_STRIP_IF_INVALID:
		ਹਾਲਤ SIGN_SIGN_IF_INVALID:
		ਹਾਲਤ SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			ਰੋਕੋ;

		/* Third, BUG */
		ਮੂਲ:
			BUG("invalid signed_commit_mode value %d", signed_commit_ਬਹੁਲਕ);
		}
		read_ਅਗਲਾ_command(state);
	}

	ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_ਅਗਲਾ_command(state);
	}
	parse_ਅੰਕੜੇ(&ਸੁਨੇਹਾ, 0, ਨਿਰਾ);
	read_ਅਗਲਾ_command(state);
	parse_from(state, b);
	ਮਿਲਾਓ_list = parse_ਮਿਲਾਓ(state, &ਮਿਲਾਓ_ਗਿਣਤੀ);

	/* ensure the branch is active/loaded */
	ਜੇ (!b->branch_ਰੁੱਖ.ਰੁੱਖ || !max_ਸਰਗਰਮ_branches) {
		unload_one_branch();
		ਲੱਦੋ_branch(b);
	}

	prev_fanout = convert_ਅੰਕ_notes_ਮੰਜ਼ਿਲ_fanout(b->ਅੰਕ_notes);

	/* file_change* */
	ਜਦੋਂ (command_buf.len > 0) {
		ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		ਨਹੀਂਤਾਂ {
			unread_command_buf = 1;
			ਰੋਕੋ;
		}
		ਜੇ (read_ਅਗਲਾ_command(state) == EOF)
			ਰੋਕੋ;
	}

	new_fanout = convert_ਅੰਕ_notes_ਮੰਜ਼ਿਲ_fanout(b->ਅੰਕ_notes);
	ਜੇ (new_fanout != prev_fanout)
		b->ਅੰਕ_notes = change_note_fanout(&b->branch_ਰੁੱਖ, new_fanout);

	/* build the tree and the commit */
	store_ਰੁੱਖ(&b->branch_ਰੁੱਖ);
	oidcpy(&b->branch_ਰੁੱਖ.versions[0].oid,
	       &b->branch_ਰੁੱਖ.versions[1].oid);

	strbuf_reset(&new_ਅੰਕੜੇ);
	strbuf_addf(&new_ਅੰਕੜੇ, "tree %s\n",
		oid_ਮੰਜ਼ਿਲ_hex(&b->branch_ਰੁੱਖ.versions[1].oid));
	ਜੇ (!is_null_oid(&b->oid))
		strbuf_addf(&new_ਅੰਕੜੇ, "parent %s\n",
			    oid_ਮੰਜ਼ਿਲ_hex(&b->oid));
	ਜਦੋਂ (ਮਿਲਾਓ_list) {
		ਬਣਤਰ hash_list *ਅਗਲਾ = ਮਿਲਾਓ_list->ਅਗਲਾ;
		strbuf_addf(&new_ਅੰਕੜੇ, "parent %s\n",
			    oid_ਮੰਜ਼ਿਲ_hex(&ਮਿਲਾਓ_list->oid));
		ਮੁਕਤ(ਮਿਲਾਓ_list);
		ਮਿਲਾਓ_list = ਅਗਲਾ;
	}
	strbuf_addf(&new_ਅੰਕੜੇ,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	ਜੇ (encoding)
		strbuf_addf(&new_ਅੰਕੜੇ,
			"encoding %s\n",
			encoding);

	ਜੇ ((signed_commit_ਬਹੁਲਕ == SIGN_STRIP_IF_INVALID ||
	     signed_commit_ਬਹੁਲਕ == SIGN_SIGN_IF_INVALID ||
	     signed_commit_ਬਹੁਲਕ == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_ਅੰਕੜੇ, &sig_sha1, &sig_sha256,
					    &ਸੁਨੇਹਾ, signed_commit_ਬਹੁਲਕ);
	ਨਹੀਂਤਾਂ
		finalize_commit_ਬਫਰ(&new_ਅੰਕੜੇ, &sig_sha1, &sig_sha256, &ਸੁਨੇਹਾ);

	ਮੁਕਤ(author);
	ਮੁਕਤ(committer);
	ਮੁਕਤ(encoding);

	ਜੇ (!store_object(OBJ_COMMIT, &new_ਅੰਕੜੇ, ਨਿਰਾ, &b->oid, ਅਗਲਾ_mark))
		b->pack_id = pack_id;
	b->ਆਖਰੀ_commit = object_ਗਿਣਤੀ_by_type[OBJ_COMMIT];
}

ਸਥਿਰ ਖਾਲੀ handle_ਟੈਗ_signature_if_invalid(ਬਣਤਰ strbuf *buf,
					    ਬਣਤਰ strbuf *ਸੁਨੇਹਾ,
					    ਮਾਪ_t sig_ਵਿੱਥ)
{
	ਬਣਤਰ strbuf signature = STRBUF_INIT;
	ਬਣਤਰ strbuf payload = STRBUF_INIT;
	ਬਣਤਰ signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_ਜੋੜੋ(&payload, ਸੁਨੇਹਾ->buf, sig_ਵਿੱਥ);
	strbuf_ਜੋੜੋ(&signature, ਸੁਨੇਹਾ->buf + sig_ਵਿੱਥ, ਸੁਨੇਹਾ->len - sig_ਵਿੱਥ);

	sigc.payload_type = SIGNATURE_PAYLOAD_ਟੈਗ;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	ਜੇ (!check_signature(&sigc, signature.buf, signature.len))
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (signed_ਟੈਗ_ਬਹੁਲਕ == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(ਸੁਨੇਹਾ, sig_ਵਿੱਥ);

	ਜੇ (signed_ਟੈਗ_ਬਹੁਲਕ == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = ਨਿਰਾ;
		strbuf_reset(&signature);

		ਜੇ (sign_ਬਫਰ(&payload, &signature, signed_ਟੈਗ_keyid,
				SIGN_ਬਫਰ_USE_DEFAULT_ਕੁੰਜੀ))
			die(_("failed to sign tag object"));

		strbuf_addbuf(ਸੁਨੇਹਾ, &signature);
	}

ਬਾਹਰਲਾ:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

ਸਥਿਰ ਖਾਲੀ handle_ਟੈਗ_signature(ਬਣਤਰ strbuf *buf, ਬਣਤਰ strbuf *ਸੁਨੇਹਾ, ਅਚਲ ਅੱਖਰ *name)
{
	ਮਾਪ_t sig_ਵਿੱਥ = parse_signed_ਬਫਰ(ਸੁਨੇਹਾ->buf, ਸੁਨੇਹਾ->len);

	/* If there is no signature, there is nothing to do. */
	ਜੇ (sig_ਵਿੱਥ >= ਸੁਨੇਹਾ->len)
		ਵਾਪਸ;

	ਚੋਣ (signed_ਟੈਗ_ਬਹੁਲਕ) {

	/* First, modes that don't change anything */
	ਹਾਲਤ SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	ਹਾਲਤ SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		ਰੋਕੋ;

	/* Second, modes that remove the signature */
	ਹਾਲਤ SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	ਹਾਲਤ SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(ਸੁਨੇਹਾ, sig_ਵਿੱਥ);
		ਰੋਕੋ;
	ਹਾਲਤ SIGN_ABORT_IF_INVALID:
	ਹਾਲਤ SIGN_SIGN_IF_INVALID:
	ਹਾਲਤ SIGN_STRIP_IF_INVALID:
		handle_ਟੈਗ_signature_if_invalid(buf, ਸੁਨੇਹਾ, sig_ਵਿੱਥ);
		ਰੋਕੋ;

	/* Third, aborting modes */
	ਹਾਲਤ SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	ਮੂਲ:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_ਟੈਗ_ਬਹੁਲਕ, name);
	}
}

ਸਥਿਰ ਖਾਲੀ parse_new_ਟੈਗ(ਬਣਤਰ fast_import_state *state, ਅਚਲ ਅੱਖਰ *arg)
{
	ਸਥਿਰ ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *from;
	ਅੱਖਰ *tagger;
	ਬਣਤਰ branch *s;
	ਬਣਤਰ ਟੈਗ *t;
	uintmax_t from_mark = 0;
	ਬਣਤਰ object_id oid;
	ਗਿਣਤੀ object_type type;
	ਅਚਲ ਅੱਖਰ *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, ਆਕਾਰ(ਬਣਤਰ ਟੈਗ));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	ਜੇ (ਆਖਰੀ_ਟੈਗ)
		ਆਖਰੀ_ਟੈਗ->ਅਗਲਾ_ਟੈਗ = t;
	ਨਹੀਂਤਾਂ
		ਪਹਿਲਾ_ਟੈਗ = t;
	ਆਖਰੀ_ਟੈਗ = t;
	read_ਅਗਲਾ_command(state);
	parse_mark(state);

	/* from ... */
	ਜੇ (!skip_ਅਗੇਤਰ(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	ਜੇ (s) {
		ਜੇ (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} ਨਹੀਂਤਾਂ ਜੇ (*from == ':') {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = ਲੱਭੋ_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} ਨਹੀਂਤਾਂ ਜੇ (!repo_get_oid(the_repository, from, &oid)) {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *oe = ਲੱਭੋ_object(&oid);
		ਜੇ (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, ਨਿਰਾ);
			ਜੇ (type < 0)
				die(_("not a valid object: %s"), from);
		} ਨਹੀਂਤਾਂ
			type = oe->type;
	} ਨਹੀਂਤਾਂ
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_ਅਗਲਾ_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_ਅਗਲਾ_command(state);
	} ਨਹੀਂਤਾਂ
		tagger = ਨਿਰਾ;

	/* tag payload/message */
	parse_ਅੰਕੜੇ(&ਸੁਨੇਹਾ, 0, ਨਿਰਾ);

	/* build the tag object */
	strbuf_reset(&new_ਅੰਕੜੇ);

	strbuf_addf(&new_ਅੰਕੜੇ,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_ਮੰਜ਼ਿਲ_hex(&oid), type_name(type), t->name);
	ਜੇ (tagger)
		strbuf_addf(&new_ਅੰਕੜੇ,
			    "tagger %s\n", tagger);

	handle_ਟੈਗ_signature(&new_ਅੰਕੜੇ, &ਸੁਨੇਹਾ, t->name);

	strbuf_addch(&new_ਅੰਕੜੇ, '\n');
	strbuf_addbuf(&new_ਅੰਕੜੇ, &ਸੁਨੇਹਾ);
	ਮੁਕਤ(tagger);

	ਜੇ (store_object(OBJ_ਟੈਗ, &new_ਅੰਕੜੇ, ਨਿਰਾ, &t->oid, ਅਗਲਾ_mark))
		t->pack_id = MAX_PACK_ID;
	ਨਹੀਂਤਾਂ
		t->pack_id = pack_id;
}

ਸਥਿਰ ਖਾਲੀ parse_reset_branch(ਬਣਤਰ fast_import_state *state, ਅਚਲ ਅੱਖਰ *arg)
{
	ਬਣਤਰ branch *b;
	ਅਚਲ ਅੱਖਰ *ਟੈਗ_name;

	b = lookup_branch(arg);
	ਜੇ (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_ਰੁੱਖ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_ਰੁੱਖ.versions[1].oid, the_repository->hash_algo);
		ਜੇ (b->branch_ਰੁੱਖ.ਰੁੱਖ) {
			release_ਰੁੱਖ_content_recursive(b->branch_ਰੁੱਖ.ਰੁੱਖ);
			b->branch_ਰੁੱਖ.ਰੁੱਖ = ਨਿਰਾ;
		}
	}
	ਨਹੀਂਤਾਂ
		b = new_branch(arg);
	read_ਅਗਲਾ_command(state);
	parse_from(state, b);
	ਜੇ (b->delete && skip_ਅਗੇਤਰ(b->name, "refs/tags/", &ਟੈਗ_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		ਬਣਤਰ ਟੈਗ *t, *prev = ਨਿਰਾ;
		ਲਈ (t = ਪਹਿਲਾ_ਟੈਗ; t; t = t->ਅਗਲਾ_ਟੈਗ) {
			ਜੇ (!strcmp(t->name, ਟੈਗ_name))
				ਰੋਕੋ;
			prev = t;
		}
		ਜੇ (t) {
			ਜੇ (prev)
				prev->ਅਗਲਾ_ਟੈਗ = t->ਅਗਲਾ_ਟੈਗ;
			ਨਹੀਂਤਾਂ
				ਪਹਿਲਾ_ਟੈਗ = t->ਅਗਲਾ_ਟੈਗ;
			ਜੇ (!t->ਅਗਲਾ_ਟੈਗ)
				ਆਖਰੀ_ਟੈਗ = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	ਜੇ (command_buf.len > 0)
		unread_command_buf = 1;
}

ਸਥਿਰ ਖਾਲੀ cat_blob_write(ਅਚਲ ਅੱਖਰ *buf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ)
{
	ਜੇ (write_ਵਿੱਚ_ਭਰਿਆ(cat_blob_fd, buf, ਮਾਪ) < 0)
		die_errno(_("write to frontend failed"));
}

ਸਥਿਰ ਖਾਲੀ cat_blob(ਬਣਤਰ object_ਇੰਦਰਾਜ *oe, ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;
	ਬਣਤਰ iovec iov[3];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;
	ਗਿਣਤੀ object_type type = 0;
	ਅੱਖਰ *buf;

	ਜੇ (!oe || oe->pack_id == MAX_PACK_ID) {
		ਮਾਪ_t ਮਾਪ_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &ਮਾਪ_st);
		ਮਾਪ = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(ਮਾਪ_st);
	} ਨਹੀਂਤਾਂ {
		type = oe->type;
		buf = gfi_unpack_ਇੰਦਰਾਜ(oe, &ਮਾਪ);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	ਜੇ (type <= 0) {
		strbuf_reset(&ਸਤਰ);
		strbuf_addf(&ਸਤਰ, "%s missing\n", oid_ਮੰਜ਼ਿਲ_hex(oid));
		cat_blob_write(ਸਤਰ.buf, ਸਤਰ.len);
		strbuf_release(&ਸਤਰ);
		ਮੁਕਤ(buf);
		ਵਾਪਸ;
	}
	ਜੇ (!buf)
		die(_("can't read object %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
	ਜੇ (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_ਮੰਜ਼ਿਲ_hex(oid), type_name(type));
	strbuf_reset(&ਸਤਰ);
	strbuf_addf(&ਸਤਰ, "%s %s %"PRIuMAX"\n", oid_ਮੰਜ਼ਿਲ_hex(oid),
		    type_name(type), (uintmax_t)ਮਾਪ);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_ਅਧਾਰ = ਸਤਰ.buf;
	iov[0].iov_len = ਸਤਰ.len;
	iov[1].iov_ਅਧਾਰ = buf;
	iov[1].iov_len = ਮਾਪ;
	iov[2].iov_ਅਧਾਰ = (ਖਾਲੀ *) "\n";
	iov[2].iov_len = 1;

	ਜੇ (writev_ਵਿੱਚ_ਭਰਿਆ(cat_blob_fd, iov, ARRAY_ਮਾਪ(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&ਸਤਰ);
	ਜੇ (oe && oe->pack_id == pack_id) {
		ਆਖਰੀ_blob.ਵਿੱਥ = oe->idx.ਵਿੱਥ;
		strbuf_attach(&ਆਖਰੀ_blob.ਅੰਕੜੇ, buf, ਮਾਪ, ਮਾਪ + 1);
		ਆਖਰੀ_blob.ਡੂੰਘਾਈ = oe->ਡੂੰਘਾਈ;
	} ਨਹੀਂਤਾਂ
		ਮੁਕਤ(buf);
}

ਸਥਿਰ ਖਾਲੀ parse_get_mark(ਬਣਤਰ fast_import_state *state ਅਣਵਰਤਿਆ,
			   ਅਚਲ ਅੱਖਰ *p)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *oe;
	ਅੱਖਰ output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	ਜੇ (*p != ':')
		die(_("not a mark: %s"), p);

	oe = ਲੱਭੋ_mark(marks, parse_mark_ref_eol(p));
	ਜੇ (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, ਆਕਾਰ(output), "%s\n", oid_ਮੰਜ਼ਿਲ_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

ਸਥਿਰ ਖਾਲੀ parse_cat_blob(ਬਣਤਰ fast_import_state *state ਅਣਵਰਤਿਆ,
			   ਅਚਲ ਅੱਖਰ *p)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *oe;
	ਬਣਤਰ object_id oid;

	/* cat-blob SP <object> LF */
	ਜੇ (*p == ':') {
		oe = ਲੱਭੋ_mark(marks, parse_mark_ref_eol(p));
		ਜੇ (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		ਜੇ (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = ਲੱਭੋ_object(&oid);
	}

	cat_blob(oe, &oid);
}

ਸਥਿਰ ਬਣਤਰ object_ਇੰਦਰਾਜ *dereference(ਬਣਤਰ object_ਇੰਦਰਾਜ *oe,
					ਬਣਤਰ object_id *oid)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;
	ਅੱਖਰ *buf = ਨਿਰਾ;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ hexsz = the_hash_algo->hexsz;

	ਜੇ (!oe) {
		ਗਿਣਤੀ object_type type = odb_read_object_info(the_repository->objects,
							     oid, ਨਿਰਾ);
		ਜੇ (type < 0)
			die(_("object not found: %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));
		/* cache it! */
		oe = ਪਾਓ_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.ਵਿੱਥ = 1;
	}
	ਚੋਣ (oe->type) {
	ਹਾਲਤ OBJ_ਰੁੱਖ:	/* easy case. */
		ਵਾਪਸ oe;
	ਹਾਲਤ OBJ_COMMIT:
	ਹਾਲਤ OBJ_ਟੈਗ:
		ਰੋਕੋ;
	ਮੂਲ:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	ਜੇ (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_ਇੰਦਰਾਜ(oe, &ਮਾਪ);
	} ਨਹੀਂਤਾਂ {
		ਗਿਣਤੀ object_type ਅਣਵਰਤਿਆ_2;
		ਮਾਪ_t ਮਾਪ_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &ਅਣਵਰਤਿਆ_2, &ਮਾਪ_st);
		ਮਾਪ = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(ਮਾਪ_st);
	}
	ਜੇ (!buf)
		die(_("can't load object %s"), oid_ਮੰਜ਼ਿਲ_hex(oid));

	/* Peel one layer. */
	ਚੋਣ (oe->type) {
	ਹਾਲਤ OBJ_ਟੈਗ:
		ਜੇ (ਮਾਪ < hexsz + ਲੜੀ_ਲੰਬਾਈ("object ") ||
		    get_oid_hex(buf + ਲੜੀ_ਲੰਬਾਈ("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		ਰੋਕੋ;
	ਹਾਲਤ OBJ_COMMIT:
		ਜੇ (ਮਾਪ < hexsz + ਲੜੀ_ਲੰਬਾਈ("tree ") ||
		    get_oid_hex(buf + ਲੜੀ_ਲੰਬਾਈ("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	ਮੁਕਤ(buf);
	ਵਾਪਸ ਲੱਭੋ_object(oid);
}

ਸਥਿਰ ਖਾਲੀ ਪਾਓ_mapped_mark(uintmax_t mark, ਖਾਲੀ *object, ਖਾਲੀ *cbp)
{
	ਬਣਤਰ object_id *fromoid = object;
	ਬਣਤਰ object_id *tooid = ਲੱਭੋ_mark(cbp, mark);
	ਪੂਰਨ ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	ਜੇ (ret == 0)
		ਵਾਪਸ;
	kh_ਮੁੱਲ(sub_oid_map, it) = tooid;
}

ਸਥਿਰ ਖਾਲੀ build_mark_map_one(ਬਣਤਰ mark_ਸੈੱਟ *from, ਬਣਤਰ mark_ਸੈੱਟ *ਮੰਜ਼ਿਲ)
{
	for_each_mark(from, 0, ਪਾਓ_mapped_mark, ਮੰਜ਼ਿਲ);
}

ਸਥਿਰ ਖਾਲੀ build_mark_map(ਬਣਤਰ string_list *from, ਬਣਤਰ string_list *ਮੰਜ਼ਿਲ)
{
	ਬਣਤਰ string_list_ਵਸਤੂ *fromp, *ਸਿਖਰ;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_ਵਸਤੂ(fromp, from) {
		ਸਿਖਰ = string_list_lookup(ਮੰਜ਼ਿਲ, fromp->string);
		ਜੇ (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} ਨਹੀਂਤਾਂ ਜੇ (!ਸਿਖਰ || !ਸਿਖਰ->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, ਸਿਖਰ->util);
	}
}

ਸਥਿਰ ਬਣਤਰ object_ਇੰਦਰਾਜ *parse_treeish_dataref(ਅਚਲ ਅੱਖਰ **p)
{
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ object_ਇੰਦਰਾਜ *e;

	ਜੇ (**p == ':') {	/* <mark> */
		e = ਲੱਭੋ_mark(marks, parse_mark_ref_space(p));
		ਜੇ (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} ਨਹੀਂਤਾਂ {	/* <sha1> */
		ਜੇ (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = ਲੱਭੋ_object(&oid);
		ਜੇ (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	ਜਦੋਂ (!e || e->type != OBJ_ਰੁੱਖ)
		e = dereference(e, &oid);
	ਵਾਪਸ e;
}

ਸਥਿਰ ਖਾਲੀ print_ls(ਪੂਰਨ ਬਹੁਲਕ, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *hash, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਸਥਿਰ ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;

	/* See show_tree(). */
	ਅਚਲ ਅੱਖਰ *type =
		S_ISGITLINK(ਬਹੁਲਕ) ? commit_type :
		S_ISDIR(ਬਹੁਲਕ) ? ਰੁੱਖ_type :
		blob_type;

	ਜੇ (!ਬਹੁਲਕ) {
		/* missing SP path LF */
		strbuf_reset(&ਸਤਰ);
		strbuf_addstr(&ਸਤਰ, "missing ");
		quote_c_style(ਰਸਤਾ, &ਸਤਰ, ਨਿਰਾ, 0);
		strbuf_addch(&ਸਤਰ, '\n');
	} ਨਹੀਂਤਾਂ {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&ਸਤਰ);
		strbuf_addf(&ਸਤਰ, "%06o %s %s\t",
				ਬਹੁਲਕ & ~NO_DELTA, type, hash_ਮੰਜ਼ਿਲ_hex(hash));
		quote_c_style(ਰਸਤਾ, &ਸਤਰ, ਨਿਰਾ, 0);
		strbuf_addch(&ਸਤਰ, '\n');
	}
	cat_blob_write(ਸਤਰ.buf, ਸਤਰ.len);
}

ਸਥਿਰ ਖਾਲੀ parse_ls(ਬਣਤਰ fast_import_state *state ਅਣਵਰਤਿਆ,
		     ਅਚਲ ਅੱਖਰ *p,
		     ਬਣਤਰ branch *b)
{
	ਸਥਿਰ ਬਣਤਰ strbuf ਰਸਤਾ = STRBUF_INIT;
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ *ਜੜ੍ਹ = ਨਿਰਾ;
	ਬਣਤਰ ਰੁੱਖ_ਇੰਦਰਾਜ ਪੱਤੀ = {ਨਿਰਾ};

	/* ls SP (<tree-ish> SP)? <path> */
	ਜੇ (*p == '"') {
		ਜੇ (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		ਜੜ੍ਹ = &b->branch_ਰੁੱਖ;
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ object_ਇੰਦਰਾਜ *e = parse_treeish_dataref(&p);
		ਜੜ੍ਹ = new_ਰੁੱਖ_ਇੰਦਰਾਜ();
		oidcpy(&ਜੜ੍ਹ->versions[1].oid, &e->idx.oid);
		ਜੇ (!is_null_oid(&ਜੜ੍ਹ->versions[1].oid))
			ਜੜ੍ਹ->versions[1].ਬਹੁਲਕ = S_IFDIR;
		ਲੱਦੋ_ਰੁੱਖ(ਜੜ੍ਹ);
	}
	strbuf_reset(&ਰਸਤਾ);
	parse_ਰਸਤਾ_eol(&ਰਸਤਾ, p, "path");
	ਰੁੱਖ_content_get(ਜੜ੍ਹ, ਰਸਤਾ.buf, &ਪੱਤੀ, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	ਜੇ (S_ISDIR(ਪੱਤੀ.versions[1].ਬਹੁਲਕ))
		store_ਰੁੱਖ(&ਪੱਤੀ);

	print_ls(ਪੱਤੀ.versions[1].ਬਹੁਲਕ, ਪੱਤੀ.versions[1].oid.hash, ਰਸਤਾ.buf);
	ਜੇ (ਪੱਤੀ.ਰੁੱਖ)
		release_ਰੁੱਖ_content_recursive(ਪੱਤੀ.ਰੁੱਖ);
	ਜੇ (!b || ਜੜ੍ਹ != &b->branch_ਰੁੱਖ)
		release_ਰੁੱਖ_ਇੰਦਰਾਜ(ਜੜ੍ਹ);
}

ਸਥਿਰ ਖਾਲੀ checkpoint(ਖਾਲੀ)
{
	checkpoint_requested = 0;
	ਜੇ (object_ਗਿਣਤੀ) {
		ਚੱਕਰ_packfile();
	}
	dump_branches();
	dump_ਟੈਗ_ਸੂਚੀ();
	dump_marks();
}

ਸਥਿਰ ਖਾਲੀ parse_checkpoint(ਬਣਤਰ fast_import_state *state ਅਣਵਰਤਿਆ)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

ਸਥਿਰ ਖਾਲੀ parse_ਤਰੱਕੀ(ਬਣਤਰ fast_import_state *state ਅਣਵਰਤਿਆ)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

ਸਥਿਰ ਖਾਲੀ parse_alias(ਬਣਤਰ fast_import_state *state)
{
	ਬਣਤਰ object_ਇੰਦਰਾਜ *e;
	ਬਣਤਰ branch b;

	skip_optional_lf();
	read_ਅਗਲਾ_command(state);

	/* mark ... */
	parse_mark(state);
	ਜੇ (!ਅਗਲਾ_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, ਆਕਾਰ(b));
	ਜੇ (!parse_objectish_with_ਅਗੇਤਰ(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = ਲੱਭੋ_object(&b.oid);
	ਪ੍ਰਤਿਜ੍ਞਾ(e);
	ਪਾਓ_mark(&marks, ਅਗਲਾ_mark, e);
}

ਸਥਿਰ ਅੱਖਰ* make_fast_import_ਰਸਤਾ(ਬਣਤਰ fast_import_state *state,
				   ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਜੇ (!relative_marks_paths || is_absolute_ਰਸਤਾ(ਰਸਤਾ))
		ਵਾਪਸ ਅਗੇਤਰ_filename(state->ਅਗੇਤਰ, ਰਸਤਾ);
	ਵਾਪਸ repo_git_ਰਸਤਾ(the_repository, "info/fast-import/%s", ਰਸਤਾ);
}

ਸਥਿਰ ਖਾਲੀ option_import_marks(ਬਣਤਰ fast_import_state *state,
				ਅਚਲ ਅੱਖਰ *marks,
				ਪੂਰਨ from_ਧਾਰਾ,
				ਪੂਰਨ ignore_missing)
{
	ਜੇ (import_marks_file) {
		ਜੇ (from_ਧਾਰਾ)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		ਜੇ(!import_marks_file_from_ਧਾਰਾ)
			read_marks();
	}

	ਮੁਕਤ(import_marks_file);
	import_marks_file = make_fast_import_ਰਸਤਾ(state, marks);
	import_marks_file_from_ਧਾਰਾ = from_ਧਾਰਾ;
	import_marks_file_ignore_missing = ignore_missing;
}

ਸਥਿਰ ਖਾਲੀ option_date_format(ਅਚਲ ਅੱਖਰ *fmt)
{
	ਜੇ (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	ਨਹੀਂਤਾਂ
		die(_("unknown --date-format argument %s"), fmt);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ulong_arg(ਅਚਲ ਅੱਖਰ *option, ਅਚਲ ਅੱਖਰ *arg)
{
	ਅੱਖਰ *endptr;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ rv = strtoul(arg, &endptr, 0);
	ਜੇ (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	ਵਾਪਸ rv;
}

ਸਥਿਰ ਖਾਲੀ option_ਡੂੰਘਾਈ(ਅਚਲ ਅੱਖਰ *ਡੂੰਘਾਈ)
{
	max_ਡੂੰਘਾਈ = ulong_arg("--depth", ਡੂੰਘਾਈ);
	ਜੇ (max_ਡੂੰਘਾਈ > MAX_ਡੂੰਘਾਈ)
		die(_("--depth cannot exceed %u"), MAX_ਡੂੰਘਾਈ);
}

ਸਥਿਰ ਖਾਲੀ option_ਸਰਗਰਮ_branches(ਅਚਲ ਅੱਖਰ *branches)
{
	max_ਸਰਗਰਮ_branches = ulong_arg("--active-branches", branches);
}

ਸਥਿਰ ਖਾਲੀ option_export_marks(ਬਣਤਰ fast_import_state *state,
				ਅਚਲ ਅੱਖਰ *marks)
{
	ਮੁਕਤ(export_marks_file);
	export_marks_file = make_fast_import_ਰਸਤਾ(state, marks);
}

ਸਥਿਰ ਖਾਲੀ option_cat_blob_fd(ਬਣਤਰ fast_import_state *state ਅਣਵਰਤਿਆ,
			       ਅਚਲ ਅੱਖਰ *fd)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ n = ulong_arg("--cat-blob-fd", fd);
	ਜੇ (n > (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
		die(_("--cat-blob-fd cannot exceed %d"), ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
	cat_blob_fd = (ਪੂਰਨ) n;
}

ਸਥਿਰ ਖਾਲੀ option_export_pack_ਕਿਨਾਰੇ(ਬਣਤਰ fast_import_state *state,
				     ਅਚਲ ਅੱਖਰ *ਕਿਨਾਰੇ)
{
	ਅੱਖਰ *fn = ਅਗੇਤਰ_filename(state->ਅਗੇਤਰ, ਕਿਨਾਰੇ);
	ਜੇ (pack_ਕਿਨਾਰੇ)
		fclose(pack_ਕਿਨਾਰੇ);
	pack_ਕਿਨਾਰੇ = xfopen(fn, "a");
	ਮੁਕਤ(fn);
}

ਸਥਿਰ ਖਾਲੀ option_rewrite_submodules(ਬਣਤਰ fast_import_state *state,
				      ਅਚਲ ਅੱਖਰ *arg,
				      ਬਣਤਰ string_list *list)
{
	ਬਣਤਰ mark_ਸੈੱਟ *ms;
	FILE *fp;
	ਅੱਖਰ *s = xstrdup(arg);
	ਅੱਖਰ *f = strchr(s, ':');
	ਜੇ (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = ਅਗੇਤਰ_filename(state->ਅਗੇਤਰ, f);
	fp = fopen(f, "r");
	ਜੇ (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, ਪਾਓ_oid_ਇੰਦਰਾਜ);
	fclose(fp);
	ਮੁਕਤ(f);

	string_list_ਪਾਓ(list, s)->util = ms;

	ਮੁਕਤ(s);
}

ਸਥਿਰ ਖਾਲੀ option_max_pack_ਮਾਪ(ਅਚਲ ਅੱਖਰ *arg)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ v;

	ਜੇ (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	ਜੇ (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} ਨਹੀਂਤਾਂ ਜੇ (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

ਸਥਿਰ ਖਾਲੀ option_big_file_ਹੱਦ(ਅਚਲ ਅੱਖਰ *arg)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ v;

	ਜੇ (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_ਸੈੱਟ_big_file_ਹੱਦ(the_repository, v);
}

ਸਥਿਰ ਖਾਲੀ option_signed_commits(ਅਚਲ ਅੱਖਰ *arg)
{
	ਜੇ (parse_sign_ਬਹੁਲਕ(arg, &signed_commit_ਬਹੁਲਕ, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

ਸਥਿਰ ਖਾਲੀ option_signed_ਟੈਗ_ਸੂਚੀ(ਅਚਲ ਅੱਖਰ *arg)
{
	ਜੇ (parse_sign_ਬਹੁਲਕ(arg, &signed_ਟੈਗ_ਬਹੁਲਕ, &signed_ਟੈਗ_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

ਸਥਿਰ ਖਾਲੀ option_quiet(ਖਾਲੀ)
{
	show_stats = 0;
	quiet = 1;
}

ਸਥਿਰ ਪੂਰਨ parse_one_option(ਬਣਤਰ fast_import_state *state, ਅਚਲ ਅੱਖਰ *option)
{
	ਜੇ (skip_ਅਗੇਤਰ(option, "max-pack-size=", &option)) {
		option_max_pack_ਮਾਪ(option);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(option, "big-file-threshold=", &option)) {
		option_big_file_ਹੱਦ(option);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(option, "depth=", &option)) {
		option_ਡੂੰਘਾਈ(option);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(option, "active-branches=", &option)) {
		option_ਸਰਗਰਮ_branches(option);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(option, "export-pack-edges=", &option)) {
		option_export_pack_ਕਿਨਾਰੇ(state, option);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(option, "signed-tags=", &option)) {
		option_signed_ਟੈਗ_ਸੂਚੀ(option);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(option, "quiet")) {
		option_quiet();
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(option, "stats")) {
		show_stats = 1;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ 1;
}

ਸਥਿਰ ਖਾਲੀ check_unsafe_feature(ਬਣਤਰ fast_import_state *state,
				 ਅਚਲ ਅੱਖਰ *feature)
{
	ਜੇ (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

ਸਥਿਰ ਪੂਰਨ parse_one_feature(ਬਣਤਰ fast_import_state *state,
			     ਅਚਲ ਅੱਖਰ *feature)
{
	ਅਚਲ ਅੱਖਰ *arg;

	ਜੇ (skip_ਅਗੇਤਰ(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_ਮੰਜ਼ਿਲ);
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(feature, "force")) {
		force_update = 1;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ 1;
}

ਸਥਿਰ ਖਾਲੀ parse_feature(ਬਣਤਰ fast_import_state *state, ਅਚਲ ਅੱਖਰ *feature)
{
	ਜੇ (state->ਵੇਖਿਆ_ਅੰਕੜੇ_command)
		die(_("got feature command '%s' after data command"), feature);

	ਜੇ (parse_one_feature(state, feature))
		ਵਾਪਸ;

	die(_("this version of fast-import does not support feature %s."), feature);
}

ਸਥਿਰ ਖਾਲੀ parse_option(ਬਣਤਰ fast_import_state *state, ਅਚਲ ਅੱਖਰ *option)
{
	ਜੇ (state->ਵੇਖਿਆ_ਅੰਕੜੇ_command)
		die(_("got option command '%s' after data command"), option);

	ਜੇ (parse_one_option(state, option))
		ਵਾਪਸ;

	die(_("this version of fast-import does not support option: %s"), option);
}

ਸਥਿਰ ਖਾਲੀ git_pack_config(ਖਾਲੀ)
{
	ਪੂਰਨ indexversion_ਮੁੱਲ;
	ਪੂਰਨ ਹੱਦਬੰਦੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ packsizelimit_ਮੁੱਲ;

	ਜੇ (!repo_config_get_ulong(the_repository, "pack.depth", &max_ਡੂੰਘਾਈ)) {
		ਜੇ (max_ਡੂੰਘਾਈ > MAX_ਡੂੰਘਾਈ)
			max_ਡੂੰਘਾਈ = MAX_ਡੂੰਘਾਈ;
	}
	ਜੇ (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_ਮੁੱਲ)) {
		pack_idx_opts.ਸੰਸਕਰਣ = indexversion_ਮੁੱਲ;
		ਜੇ (pack_idx_opts.ਸੰਸਕਰਣ > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.ਸੰਸਕਰਣ);
	}
	ਜੇ (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_ਮੁੱਲ))
		max_packsize = packsizelimit_ਮੁੱਲ;

	ਜੇ (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &ਹੱਦਬੰਦੀ))
		unpack_ਹੱਦਬੰਦੀ = ਹੱਦਬੰਦੀ;
	ਨਹੀਂਤਾਂ ਜੇ (!repo_config_get_int(the_repository, "transfer.unpacklimit", &ਹੱਦਬੰਦੀ))
		unpack_ਹੱਦਬੰਦੀ = ਹੱਦਬੰਦੀ;

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਖਾਲੀ parse_argv(ਬਣਤਰ fast_import_state *state)
{
	ਪੂਰਨ argc = parse_options(state->argc, state->argv, state->ਅਗੇਤਰ,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	ਜੇ (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->ਵੇਖਿਆ_ਅੰਕੜੇ_command = 1;
	ਜੇ (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_ਮੰਜ਼ਿਲ);
}

ਸਥਿਰ ਪੂਰਨ option_parse_date_format(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
				    ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_export_pack_ਕਿਨਾਰੇ(ਅਚਲ ਬਣਤਰ option *opt,
					  ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_ਕਿਨਾਰੇ(opt->ਮੁੱਲ, arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_max_pack_ਮਾਪ(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
				      ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_ਮਾਪ(arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_big_file_ਹੱਦ(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
					   ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_ਹੱਦ(arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_signed_commits(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
				       ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_signed_ਟੈਗ_ਸੂਚੀ(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
				    ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_ਟੈਗ_ਸੂਚੀ(arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_rewrite_submodules_from(ਅਚਲ ਬਣਤਰ option *opt,
						ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->ਮੁੱਲ, arg, &sub_marks_from);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_rewrite_submodules_ਮੰਜ਼ਿਲ(ਅਚਲ ਬਣਤਰ option *opt,
					      ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->ਮੁੱਲ, arg, &sub_marks_ਮੰਜ਼ਿਲ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_cat_blob_fd(ਅਚਲ ਬਣਤਰ option *opt,
				    ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->ਮੁੱਲ, arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_import_marks(ਅਚਲ ਬਣਤਰ option *opt,
				     ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->ਮੁੱਲ, arg, 0, 0);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_import_marks_if_exists(ਅਚਲ ਬਣਤਰ option *opt,
					       ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->ਮੁੱਲ, arg, 0, 1);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_export_marks(ਅਚਲ ਬਣਤਰ option *opt,
				     ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->ਮੁੱਲ, arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_ਡੂੰਘਾਈ(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
			      ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_ਡੂੰਘਾਈ(arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_ਸਰਗਰਮ_branches(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
					ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_ਸਰਗਰਮ_branches(arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_quiet(ਅਚਲ ਬਣਤਰ option *opt ਅਣਵਰਤਿਆ,
			      ਅਚਲ ਅੱਖਰ *arg ਅਣਵਰਤਿਆ, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_fast_import(ਪੂਰਨ argc,
		    ਅਚਲ ਅੱਖਰ **argv,
		    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		    ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ fast_import_state state;

	ਬਣਤਰ option fast_import_options[] = {
		OPT_ਸਮੂਹ(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", ਨਿਰਾ, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", ਨਿਰਾ, ਨਿਰਾ,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", ਨਿਰਾ, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_ਮਾਪ),
		OPT_CALLBACK_F(0, "big-file-threshold", ਨਿਰਾ, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_ਹੱਦ),
		OPT_CALLBACK_F(0, "depth", ਨਿਰਾ, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_ਡੂੰਘਾਈ),
		OPT_CALLBACK_F(0, "active-branches", ਨਿਰਾ, N_("n"),
			       N_("maximum number of branches to maintain active"),
			       PARSE_OPT_NONEG, option_parse_ਸਰਗਰਮ_branches),
		OPT_ਸਮੂਹ(N_("Marks")),
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
		OPT_ਸਮੂਹ(N_("Submodule rewrite")),
		OPT_CALLBACK_F(0, "rewrite-submodules-from", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> from <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_from),
		OPT_CALLBACK_F(0, "rewrite-submodules-to", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> to <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_ਮੰਜ਼ਿਲ),
		OPT_ਸਮੂਹ(N_("Signing")),
		OPT_CALLBACK_F(0, "signed-commits", ਨਿਰਾ, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", ਨਿਰਾ, N_("mode"),
			       N_("how to handle signed tags"),
			       PARSE_OPT_NONEG, option_parse_signed_ਟੈਗ_ਸੂਚੀ),
		OPT_HIDDEN_ਸਮੂਹ(N_("Advanced")),
		OPT_BOOL_F(0, "allow-unsafe-features", &state.allow_unsafe_features,
			   N_("allow unsafe mark commands from the stream"),
			   PARSE_OPT_HIDDEN | PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "export-pack-edges", &state, N_("file"),
			       N_("dump edge commits to <file>"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_export_pack_ਕਿਨਾਰੇ),
		OPT_CALLBACK_F(0, "cat-blob-fd", &state, N_("fd"),
			       N_("write some responses to <fd> instead of stdout"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_cat_blob_fd),
		OPT_ਸਿਰਾ()
	};

	show_usage_with_options_if_asked(argc, argv, fast_import_usage, fast_import_options);

	fast_import_state_init(&state, argc, argv, ਅਗੇਤਰ, fast_import_options);

	reset_pack_idx_option(&pack_idx_opts);
	git_pack_config();

	alloc_objects(object_ਇੰਦਰਾਜ_alloc);
	strbuf_init(&command_buf, 0);
	CALLOC_ARRAY(atom_ਸਾਰਣੀ, atom_ਸਾਰਣੀ_sz);
	CALLOC_ARRAY(branch_ਸਾਰਣੀ, branch_ਸਾਰਣੀ_sz);
	CALLOC_ARRAY(avail_ਰੁੱਖ_ਸਾਰਣੀ, avail_ਰੁੱਖ_ਸਾਰਣੀ_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, ਆਕਾਰ(ਬਣਤਰ mark_ਸੈੱਟ));

	hashmap_init(&object_ਸਾਰਣੀ, object_ਇੰਦਰਾਜ_hashcmp, ਨਿਰਾ, 0);

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
	ਲਈ (ਪੂਰਨ i = 1; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];
		ਜੇ (*arg != '-' || !strcmp(arg, "--"))
			ਰੋਕੋ;
		ਜੇ (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * ਆਕਾਰ(*rc_free));
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < (cmd_save - 1); i++)
		rc_free[i].ਅਗਲਾ = &rc_free[i + 1];
	rc_free[cmd_save - 1].ਅਗਲਾ = ਨਿਰਾ;

	ਸ਼ੁਰੂ_packfile();
	ਸੈੱਟ_die_routine(die_nicely);
	ਸੈੱਟ_checkpoint_signal();
	ਜਦੋਂ (read_ਅਗਲਾ_command(&state) != EOF) {
		ਅਚਲ ਅੱਖਰ *v;
		ਜੇ (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "tag ", &v))
			parse_new_ਟੈਗ(&state, v);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, ਨਿਰਾ);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp("done", command_buf.buf))
			ਰੋਕੋ;
		ਨਹੀਂਤਾਂ ਜੇ (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		ਨਹੀਂਤਾਂ ਜੇ (starts_with(command_buf.buf, "progress "))
			parse_ਤਰੱਕੀ(&state);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		ਨਹੀਂਤਾਂ ਜੇ (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		ਨਹੀਂਤਾਂ
			die(_("unsupported command: %s"), command_buf.buf);

		ਜੇ (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	ਜੇ (!state.ਵੇਖਿਆ_ਅੰਕੜੇ_command)
		parse_argv(&state);

	ਜੇ (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	ਸਿਰਾ_packfile();

	dump_branches();
	dump_ਟੈਗ_ਸੂਚੀ();
	unkeep_all_packs();
	dump_marks();

	ਜੇ (pack_ਕਿਨਾਰੇ)
		fclose(pack_ਕਿਨਾਰੇ);

	ਜੇ (show_stats) {
		uintmax_t ਕੁੱਲ_ਗਿਣਤੀ = 0, duplicate_ਗਿਣਤੀ = 0;
		ਲਈ (ਮਾਪ_t i = 0; i < ARRAY_ਮਾਪ(object_ਗਿਣਤੀ_by_type); i++)
			ਕੁੱਲ_ਗਿਣਤੀ += object_ਗਿਣਤੀ_by_type[i];
		ਲਈ (ਮਾਪ_t i = 0; i < ARRAY_ਮਾਪ(duplicate_ਗਿਣਤੀ_by_type); i++)
			duplicate_ਗਿਣਤੀ += duplicate_ਗਿਣਤੀ_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_ਗਿਣਤੀ);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", ਕੁੱਲ_ਗਿਣਤੀ, duplicate_ਗਿਣਤੀ);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ਗਿਣਤੀ_by_type[OBJ_BLOB], duplicate_ਗਿਣਤੀ_by_type[OBJ_BLOB], delta_ਗਿਣਤੀ_by_type[OBJ_BLOB], delta_ਗਿਣਤੀ_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ਗਿਣਤੀ_by_type[OBJ_ਰੁੱਖ], duplicate_ਗਿਣਤੀ_by_type[OBJ_ਰੁੱਖ], delta_ਗਿਣਤੀ_by_type[OBJ_ਰੁੱਖ], delta_ਗਿਣਤੀ_attempts_by_type[OBJ_ਰੁੱਖ]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ਗਿਣਤੀ_by_type[OBJ_COMMIT], duplicate_ਗਿਣਤੀ_by_type[OBJ_COMMIT], delta_ਗਿਣਤੀ_by_type[OBJ_COMMIT], delta_ਗਿਣਤੀ_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ਗਿਣਤੀ_by_type[OBJ_ਟੈਗ], duplicate_ਗਿਣਤੀ_by_type[OBJ_ਟੈਗ], delta_ਗਿਣਤੀ_by_type[OBJ_ਟੈਗ], delta_ਗਿਣਤੀ_attempts_by_type[OBJ_ਟੈਗ]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_ਗਿਣਤੀ, branch_ਲੱਦੋ_ਗਿਣਤੀ);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->ਖਿਸਕਾਓ) * 1024, marks_ਸੈੱਟ_ਗਿਣਤੀ);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (ਰੁੱਖ_ਇੰਦਰਾਜ_allocd + fi_mem_pool.pool_alloc + alloc_ਗਿਣਤੀ*ਆਕਾਰ(ਬਣਤਰ object_ਇੰਦਰਾਜ))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)((ਰੁੱਖ_ਇੰਦਰਾਜ_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_ਗਿਣਤੀ*ਆਕਾਰ(ਬਣਤਰ object_ਇੰਦਰਾਜ))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	ਵਾਪਸ failure ? 1 : 0;
}
