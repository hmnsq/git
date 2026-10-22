#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

#ವ್ಯಾಖ್ಯಾನಿಸು PACK_ID_ಬಿಟ್ಟುಗಳು 16
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_PACK_ID ((1<<PACK_ID_ಬಿಟ್ಟುಗಳು)-1)
#ವ್ಯಾಖ್ಯಾನಿಸು ಆಳ_ಬಿಟ್ಟುಗಳು 13
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_ಆಳ ((1<<ಆಳ_ಬಿಟ್ಟುಗಳು)-1)

/*
 * We abuse the setuid bit on directories to mean "do not delta".
 */
#ವ್ಯಾಖ್ಯಾನಿಸು NO_DELTA S_ISUID

/*
 * The amount of additional space required in order to write an object into the
 * current pack. This is the hash lengths at the end of the pack, plus the
 * length of one object ID.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು PACK_ಗಾತ್ರ_ಮಿತಿ (the_hash_algo->rawsz * 3)

ರಚನೆ object_ನಮೂದು {
	ರಚನೆ pack_idx_ನಮೂದು idx;
	ರಚನೆ hashmap_ನಮೂದು ent;
	uint32_t type : TYPE_ಬಿಟ್ಟುಗಳು,
		pack_id : PACK_ID_ಬಿಟ್ಟುಗಳು,
		ಆಳ : ಆಳ_ಬಿಟ್ಟುಗಳು;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ object_ನಮೂದು_hashcmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *map_ದತ್ತಾಂಶ ಬಳಸದ,
				ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *eptr,
				ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *ನಮೂದು_or_ಕೀಲಿ,
				ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *keydata)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid = keydata;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು *e1, *e2;

	e1 = container_ದ(eptr, ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು, ent);
	ಆದರೆ (oid)
		ಹಿಂತಿರುಗಿ oidcmp(&e1->idx.oid, oid);

	e2 = container_ದ(ನಮೂದು_or_ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ರಚನೆ object_ನಮೂದು, ent);
	ಹಿಂತಿರುಗಿ oidcmp(&e1->idx.oid, &e2->idx.oid);
}

ರಚನೆ object_ನಮೂದು_pool {
	ರಚನೆ object_ನಮೂದು_pool *ಮುಂದಿನ_pool;
	ರಚನೆ object_ನಮೂದು *ಮುಂದಿನ_free;
	ರಚನೆ object_ನಮೂದು *ಕೊನೆ;
	ರಚನೆ object_ನಮೂದು entries[FLEX_ARRAY]; /*ಇನ್ನಷ್ಟು*/
};

ರಚನೆ mark_ಗಣ {
	ಒಕ್ಕೂಟ {
		ರಚನೆ object_id *oids[1024];
		ರಚನೆ object_ನಮೂದು *marked[1024];
		ರಚನೆ mark_ಗಣ *sets[1024];
	} ದತ್ತಾಂಶ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸರಿಸು;
};

ರಚನೆ ಕೊನೆಯ_object {
	ರಚನೆ strbuf ದತ್ತಾಂಶ;
	off_t ಸ್ಥಾನಾಂತರ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಆಳ;
	ಚಿಹ್ನೆರಹಿತ no_ಅದಲುಬದಲು : 1;
};

ರಚನೆ atom_str {
	ರಚನೆ atom_str *ಮುಂದಿನ_atom;
	ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ str_len;
	ಅಕ್ಷರ str_dat[FLEX_ARRAY]; /*ಇನ್ನಷ್ಟು*/
};

ರಚನೆ ವೃಕ್ಷ_content;
ರಚನೆ ವೃಕ್ಷ_ನಮೂದು {
	ರಚನೆ ವೃಕ್ಷ_content *ವೃಕ್ಷ;
	ರಚನೆ atom_str *name;
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು_ms {
		uint16_t ಬಹುಲಕ;
		ರಚನೆ object_id oid;
	} versions[2];
};

ರಚನೆ ವೃಕ್ಷ_content {
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ನಮೂದು_ಸಾಮರ್ಥ್ಯ; /* must match avail_tree_content */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ನಮೂದು_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ delta_ಆಳ;
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *entries[FLEX_ARRAY]; /*ಇನ್ನಷ್ಟು*/
};

ರಚನೆ avail_ವೃಕ್ಷ_content {
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ನಮೂದು_ಸಾಮರ್ಥ್ಯ; /* must match tree_content */
	ರಚನೆ avail_ವೃಕ್ಷ_content *ಮುಂದಿನ_avail;
};

ರಚನೆ branch {
	ರಚನೆ branch *ಕೋಷ್ಟಕ_ಮುಂದಿನ_branch;
	ರಚನೆ branch *ಸಕ್ರಿಯ_ಮುಂದಿನ_branch;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು branch_ವೃಕ್ಷ;
	uintmax_t ಕೊನೆಯ_commit;
	uintmax_t ಅಂಕೆ_notes;
	ಚಿಹ್ನೆರಹಿತ ಸಕ್ರಿಯ : 1;
	ಚಿಹ್ನೆರಹಿತ delete : 1;
	ಚಿಹ್ನೆರಹಿತ pack_id : PACK_ID_ಬಿಟ್ಟುಗಳು;
	ರಚನೆ object_id oid;
};

ರಚನೆ ಟ್ಯಾಗ್ {
	ರಚನೆ ಟ್ಯಾಗ್ *ಮುಂದಿನ_ಟ್ಯಾಗ್;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ pack_id;
	ರಚನೆ object_id oid;
};

ರಚನೆ hash_list {
	ರಚನೆ hash_list *ಮುಂದಿನ;
	ರಚನೆ object_id oid;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

ರಚನೆ recent_command {
	ರಚನೆ recent_command *prev;
	ರಚನೆ recent_command *ಮುಂದಿನ;
	ಅಕ್ಷರ *buf;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*mark_ಗಣ_inserter_t)(ರಚನೆ mark_ಗಣ **s, ರಚನೆ object_id *oid, uintmax_t mark);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*each_mark_fn_t)(uintmax_t mark, ಶೂನ್ಯ *obj, ಶೂನ್ಯ *cbp);

/* Configured limits on output */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_ಆಳ = 50;
ಸ್ಥಿರ off_t max_packsize;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ unpack_ಎಲ್ಲೆ = 100;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ force_update;

/* Stats and misc. counters */
ಸ್ಥಿರ uintmax_t alloc_ಎಣಿಕೆ;
ಸ್ಥಿರ uintmax_t marks_ಗಣ_ಎಣಿಕೆ;
ಸ್ಥಿರ uintmax_t object_ಎಣಿಕೆ_by_type[1 << TYPE_ಬಿಟ್ಟುಗಳು];
ಸ್ಥಿರ uintmax_t duplicate_ಎಣಿಕೆ_by_type[1 << TYPE_ಬಿಟ್ಟುಗಳು];
ಸ್ಥಿರ uintmax_t delta_ಎಣಿಕೆ_by_type[1 << TYPE_ಬಿಟ್ಟುಗಳು];
ಸ್ಥಿರ uintmax_t delta_ಎಣಿಕೆ_attempts_by_type[1 << TYPE_ಬಿಟ್ಟುಗಳು];
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ object_ಎಣಿಕೆ;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ branch_ಎಣಿಕೆ;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ branch_ತುಂಬು_ಎಣಿಕೆ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ failure;
ಸ್ಥಿರ FILE *pack_ಅಂಚುಗಳು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_stats = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ quiet;
ಸ್ಥಿರ ಎಣಿಕೆ sign_ಬಹುಲಕ signed_ಟ್ಯಾಗ್_ಬಹುಲಕ = SIGN_VERBATIM;
ಸ್ಥಿರ ಎಣಿಕೆ sign_ಬಹುಲಕ signed_commit_ಬಹುಲಕ = SIGN_VERBATIM;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *signed_commit_keyid;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *signed_ಟ್ಯಾಗ್_keyid;

/* Memory pools */
ಸ್ಥಿರ ರಚನೆ mem_pool fi_mem_pool = {
	.ಖಂಡ_alloc = 2*1024*1024 - ಗಾತ್ರ(ರಚನೆ mp_ಖಂಡ),
};

/* Atom management */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ atom_ಕೋಷ್ಟಕ_sz = 4451;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ atom_cnt;
ಸ್ಥಿರ ರಚನೆ atom_str **atom_ಕೋಷ್ಟಕ;

/* The .pack file being generated */
ಸ್ಥಿರ ರಚನೆ pack_idx_option pack_idx_opts;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ pack_id;
ಸ್ಥಿರ ರಚನೆ hashfile *pack_file;
ಸ್ಥಿರ ರಚನೆ packed_git *pack_ದತ್ತಾಂಶ;
ಸ್ಥಿರ ರಚನೆ packed_git **all_packs;
ಸ್ಥಿರ off_t pack_ಗಾತ್ರ;

/* Table of objects we've written. */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ object_ನಮೂದು_alloc = 5000;
ಸ್ಥಿರ ರಚನೆ object_ನಮೂದು_pool *ಖಂಡಗಳು;
ಸ್ಥಿರ ರಚನೆ hashmap object_ಕೋಷ್ಟಕ;
ಸ್ಥಿರ ರಚನೆ mark_ಗಣ *marks;
ಸ್ಥಿರ ಅಕ್ಷರ *export_marks_file;
ಸ್ಥಿರ ಅಕ್ಷರ *import_marks_file;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ import_marks_file_from_ಹರಿವು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ import_marks_file_ignore_missing;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ import_marks_file_ಮುಗಿದ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ relative_marks_paths;

/* Our last blob */
ಸ್ಥಿರ ರಚನೆ ಕೊನೆಯ_object ಕೊನೆಯ_blob = {
	.ದತ್ತಾಂಶ = STRBUF_INIT,
 };

/* Tree management */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ವೃಕ್ಷ_ನಮೂದು_alloc = 1000;
ಸ್ಥಿರ ಶೂನ್ಯ *avail_ವೃಕ್ಷ_ನಮೂದು;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ avail_ವೃಕ್ಷ_ಕೋಷ್ಟಕ_sz = 100;
ಸ್ಥಿರ ರಚನೆ avail_ವೃಕ್ಷ_content **avail_ವೃಕ್ಷ_ಕೋಷ್ಟಕ;
ಸ್ಥಿರ ಗಾತ್ರ_t ವೃಕ್ಷ_ನಮೂದು_allocd;
ಸ್ಥಿರ ರಚನೆ strbuf old_ವೃಕ್ಷ = STRBUF_INIT;
ಸ್ಥಿರ ರಚನೆ strbuf new_ವೃಕ್ಷ = STRBUF_INIT;

/* Branch data */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_ಸಕ್ರಿಯ_branches = 5;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cur_ಸಕ್ರಿಯ_branches;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ branch_ಕೋಷ್ಟಕ_sz = 1039;
ಸ್ಥಿರ ರಚನೆ branch **branch_ಕೋಷ್ಟಕ;
ಸ್ಥಿರ ರಚನೆ branch *ಸಕ್ರಿಯ_branches;

/* Tag data */
ಸ್ಥಿರ ರಚನೆ ಟ್ಯಾಗ್ *ಮೊದಲ_ಟ್ಯಾಗ್;
ಸ್ಥಿರ ರಚನೆ ಟ್ಯಾಗ್ *ಕೊನೆಯ_ಟ್ಯಾಗ್;

/* Input stream parsing */
ಸ್ಥಿರ whenspec_type whenspec = WHENSPEC_RAW;
ಸ್ಥಿರ ರಚನೆ strbuf command_buf = STRBUF_INIT;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ unread_command_buf;
ಸ್ಥಿರ ರಚನೆ recent_command cmd_hist = {
	.prev = &cmd_hist,
	.ಮುಂದಿನ = &cmd_hist,
};
ಸ್ಥಿರ ರಚನೆ recent_command *cmd_ಬಾಲ = &cmd_hist;
ಸ್ಥಿರ ರಚನೆ recent_command *rc_free;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cmd_save = 100;
ಸ್ಥಿರ uintmax_t ಮುಂದಿನ_mark;
ಸ್ಥಿರ ರಚನೆ strbuf new_ದತ್ತಾಂಶ = STRBUF_INIT;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ require_explicit_termination;

/* Signal handling */
ಸ್ಥಿರ ಅಸ್ಥಿರ sig_atomic_t checkpoint_requested;

/* Submodule marks */
ಸ್ಥಿರ ರಚನೆ string_list sub_marks_from = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ ರಚನೆ string_list sub_marks_ಗಮ್ಯ = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cat_blob_fd = STDOUT_FILENO;

/* Command state */
ರಚನೆ fast_import_state {
	ಪೂರ್ಣಾಂಕ argc;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಪೂರ್ಣಾಂಕ ನೋಡಿದ_ದತ್ತಾಂಶ_command;
	ಪೂರ್ಣಾಂಕ allow_unsafe_features;
	ರಚನೆ option *option;
};

ಸ್ಥಿರ ಶೂನ್ಯ fast_import_state_init(ರಚನೆ fast_import_state *state,
				   ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ರಚನೆ option *option)
{
	memset(state, 0, ಗಾತ್ರ(*state));
	state->argc = argc;
	state->argv = argv;
	state->ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;
	state->option = option;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_argv(ರಚನೆ fast_import_state *state);
ಸ್ಥಿರ ಶೂನ್ಯ parse_get_mark(ರಚನೆ fast_import_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p);
ಸ್ಥಿರ ಶೂನ್ಯ parse_cat_blob(ರಚನೆ fast_import_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p);
ಸ್ಥಿರ ಶೂನ್ಯ parse_ls(ರಚನೆ fast_import_state *state,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p,
		     ರಚನೆ branch *b);

ಸ್ಥಿರ ಶೂನ್ಯ for_each_mark(ರಚನೆ mark_ಗಣ *m, uintmax_t ತಳ, each_mark_fn_t callback, ಶೂನ್ಯ *p)
{
	uintmax_t k;
	ಆದರೆ (m->ಸರಿಸು) {
		ಗಾಗಿ (k = 0; k < 1024; k++) {
			ಆದರೆ (m->ದತ್ತಾಂಶ.sets[k])
				for_each_mark(m->ದತ್ತಾಂಶ.sets[k], ತಳ + (k << m->ಸರಿಸು), callback, p);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಗಾಗಿ (k = 0; k < 1024; k++) {
			ಆದರೆ (m->ದತ್ತಾಂಶ.marked[k])
				callback(ತಳ + k, m->ದತ್ತಾಂಶ.marked[k], p);
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_marks_fn(uintmax_t mark, ಶೂನ್ಯ *object, ಶೂನ್ಯ *cbp) {
	ರಚನೆ object_ನಮೂದು *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_ಗಮ್ಯ_hex(&e->idx.oid));
}

ಸ್ಥಿರ ಶೂನ್ಯ write_branch_report(FILE *rpt, ರಚನೆ branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	ಆದರೆ (b->ಸಕ್ರಿಯ)
		fputs(" active", rpt);
	ಆದರೆ (b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ)
		fputs(" loaded", rpt);
	ಆದರೆ (is_null_oid(&b->branch_ವೃಕ್ಷ.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_ಗಮ್ಯ_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_ಗಮ್ಯ_hex(&b->branch_ವೃಕ್ಷ.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_ಗಮ್ಯ_hex(&b->branch_ವೃಕ್ಷ.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->ಕೊನೆಯ_commit);

	fputs("  last pack   : ", rpt);
	ಆದರೆ (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_crash_report(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *err)
{
	ಅಕ್ಷರ *loc = repo_git_ಮಾರ್ಗ(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	ರಚನೆ branch *b;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ lu;
	ರಚನೆ recent_command *rc;

	ಆದರೆ (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		ಬಿಡುಗಡೆ(loc);
		ಹಿಂತಿರುಗಿ;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(ಸಮಯ(ಖಾಲಿ), 0, DATE_ಬಹುಲಕ(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	ಗಾಗಿ (rc = cmd_hist.ಮುಂದಿನ; rc != &cmd_hist; rc = rc->ಮುಂದಿನ) {
		ಆದರೆ (rc->ಮುಂದಿನ == &cmd_hist)
			fputs("* ", rpt);
		ಇಲ್ಲದಿದ್ದರೆ
			fputs("  ", rpt);
		fputs(rc->buf, rpt);
		fputc('\n', rpt);
	}

	fputc('\n', rpt);
	fputs("Active Branch LRU\n", rpt);
	fputs("-----------------\n", rpt);
	fprintf(rpt, "    active_branches = %lu cur, %lu max\n",
		cur_ಸಕ್ರಿಯ_branches,
		max_ಸಕ್ರಿಯ_branches);
	fputc('\n', rpt);
	fputs("  pos  clock name\n", rpt);
	fputs("  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", rpt);
	ಗಾಗಿ (b = ಸಕ್ರಿಯ_branches, lu = 0; b; b = b->ಸಕ್ರಿಯ_ಮುಂದಿನ_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->ಕೊನೆಯ_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	ಗಾಗಿ (lu = 0; lu < branch_ಕೋಷ್ಟಕ_sz; lu++) {
		ಗಾಗಿ (b = branch_ಕೋಷ್ಟಕ[lu]; b; b = b->ಕೋಷ್ಟಕ_ಮುಂದಿನ_branch)
			write_branch_report(rpt, b);
	}

	ಆದರೆ (ಮೊದಲ_ಟ್ಯಾಗ್) {
		ರಚನೆ ಟ್ಯಾಗ್ *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		ಗಾಗಿ (tg = ಮೊದಲ_ಟ್ಯಾಗ್; tg; tg = tg->ಮುಂದಿನ_ಟ್ಯಾಗ್) {
			fputs(oid_ಗಮ್ಯ_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	ಆದರೆ (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	ಇಲ್ಲದಿದ್ದರೆ
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	ಬಿಡುಗಡೆ(loc);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೊನೆ_packfile(ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ unkeep_all_packs(ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ dump_marks(ಶೂನ್ಯ);

ಸ್ಥಿರ NORETURN ಶೂನ್ಯ die_nicely(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *err, va_list params)
{
	va_list cp;
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ zombie;
	report_fn die_ಸಂದೇಶ_fn = get_die_ಸಂದೇಶ_routine();

	va_copy(cp, params);
	die_ಸಂದೇಶ_fn(err, params);

	ಆದರೆ (!zombie) {
		ಅಕ್ಷರ ಸಂದೇಶ_2[2 * ಮಾರ್ಗ_MAX];

		zombie = 1;
		vsnprintf(ಸಂದೇಶ_2, ಗಾತ್ರ(ಸಂದೇಶ_2), err, cp);
		write_crash_report(ಸಂದೇಶ_2);
		ಕೊನೆ_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	ನಿರ್ಗಮಿಸು(128);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SIGUSR1	/* Windows, for example */

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_checkpoint_signal(ಶೂನ್ಯ)
{
}

#ಇಲ್ಲದಿದ್ದರೆ

ಸ್ಥಿರ ಶೂನ್ಯ checkpoint_signal(ಪೂರ್ಣಾಂಕ signo ಬಳಸದ)
{
	checkpoint_requested = 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_checkpoint_signal(ಶೂನ್ಯ)
{
	ರಚನೆ sigaction sa;

	memset(&sa, 0, ಗಾತ್ರ(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_ಮುಖವಾಡ);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, ಖಾಲಿ);
}

#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ alloc_objects(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cnt)
{
	ರಚನೆ object_ನಮೂದು_pool *b;

	b = xmalloc(ಗಾತ್ರ(ರಚನೆ object_ನಮೂದು_pool)
		+ cnt * ಗಾತ್ರ(ರಚನೆ object_ನಮೂದು));
	b->ಮುಂದಿನ_pool = ಖಂಡಗಳು;
	b->ಮುಂದಿನ_free = b->entries;
	b->ಕೊನೆ = b->entries + cnt;
	ಖಂಡಗಳು = b;
	alloc_ಎಣಿಕೆ += cnt;
}

ಸ್ಥಿರ ರಚನೆ object_ನಮೂದು *new_object(ರಚನೆ object_id *oid)
{
	ರಚನೆ object_ನಮೂದು *e;

	ಆದರೆ (ಖಂಡಗಳು->ಮುಂದಿನ_free == ಖಂಡಗಳು->ಕೊನೆ)
		alloc_objects(object_ನಮೂದು_alloc);

	e = ಖಂಡಗಳು->ಮುಂದಿನ_free++;
	oidcpy(&e->idx.oid, oid);
	ಹಿಂತಿರುಗಿ e;
}

ಸ್ಥಿರ ರಚನೆ object_ನಮೂದು *ಕಂಡುಹಿಡಿ_object(ರಚನೆ object_id *oid)
{
	ಹಿಂತಿರುಗಿ hashmap_get_ನಮೂದು_from_hash(&object_ಕೋಷ್ಟಕ, oidhash(oid), oid,
					   ರಚನೆ object_ನಮೂದು, ent);
}

ಸ್ಥಿರ ರಚನೆ object_ನಮೂದು *ಸೇರಿಸು_object(ರಚನೆ object_id *oid)
{
	ರಚನೆ object_ನಮೂದು *e;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hash = oidhash(oid);

	e = hashmap_get_ನಮೂದು_from_hash(&object_ಕೋಷ್ಟಕ, hash, oid,
					ರಚನೆ object_ನಮೂದು, ent);
	ಆದರೆ (!e) {
		e = new_object(oid);
		e->idx.ಸ್ಥಾನಾಂತರ = 0;
		hashmap_ನಮೂದು_init(&e->ent, hash);
		hashmap_ಕೂಡಿಸು(&object_ಕೋಷ್ಟಕ, &e->ent);
	}

	ಹಿಂತಿರುಗಿ e;
}

ಸ್ಥಿರ ಶೂನ್ಯ invalidate_pack_id(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ id)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ lu;
	ರಚನೆ ಟ್ಯಾಗ್ *t;
	ರಚನೆ hashmap_iter iter;
	ರಚನೆ object_ನಮೂದು *e;

	hashmap_for_each_ನಮೂದು(&object_ಕೋಷ್ಟಕ, &iter, e, ent) {
		ಆದರೆ (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	ಗಾಗಿ (lu = 0; lu < branch_ಕೋಷ್ಟಕ_sz; lu++) {
		ರಚನೆ branch *b;

		ಗಾಗಿ (b = branch_ಕೋಷ್ಟಕ[lu]; b; b = b->ಕೋಷ್ಟಕ_ಮುಂದಿನ_branch)
			ಆದರೆ (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	ಗಾಗಿ (t = ಮೊದಲ_ಟ್ಯಾಗ್; t; t = t->ಮುಂದಿನ_ಟ್ಯಾಗ್)
		ಆದರೆ (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hc_str(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಗಾತ್ರ_t len)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ r = 0;
	ಆಗ (len-- > 0)
		r = r * 31 + *s++;
	ಹಿಂತಿರುಗಿ r;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_mark(ರಚನೆ mark_ಗಣ **ತುದಿ, uintmax_t idnum, ರಚನೆ object_ನಮೂದು *oe)
{
	ರಚನೆ mark_ಗಣ *s = *ತುದಿ;

	ಆಗ ((idnum >> s->ಸರಿಸು) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, ಗಾತ್ರ(ರಚನೆ mark_ಗಣ));
		s->ಸರಿಸು = (*ತುದಿ)->ಸರಿಸು + 10;
		s->ದತ್ತಾಂಶ.sets[0] = *ತುದಿ;
		*ತುದಿ = s;
	}
	ಆಗ (s->ಸರಿಸು) {
		uintmax_t i = idnum >> s->ಸರಿಸು;
		idnum -= i << s->ಸರಿಸು;
		ಆದರೆ (!s->ದತ್ತಾಂಶ.sets[i]) {
			s->ದತ್ತಾಂಶ.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, ಗಾತ್ರ(ರಚನೆ mark_ಗಣ));
			s->ದತ್ತಾಂಶ.sets[i]->ಸರಿಸು = s->ಸರಿಸು - 10;
		}
		s = s->ದತ್ತಾಂಶ.sets[i];
	}
	ಆದರೆ (!s->ದತ್ತಾಂಶ.marked[idnum])
		marks_ಗಣ_ಎಣಿಕೆ++;
	s->ದತ್ತಾಂಶ.marked[idnum] = oe;
}

ಸ್ಥಿರ ಶೂನ್ಯ *ಕಂಡುಹಿಡಿ_mark(ರಚನೆ mark_ಗಣ *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	ರಚನೆ object_ನಮೂದು *oe = ಖಾಲಿ;
	ಆದರೆ ((idnum >> s->ಸರಿಸು) < 1024) {
		ಆಗ (s && s->ಸರಿಸು) {
			uintmax_t i = idnum >> s->ಸರಿಸು;
			idnum -= i << s->ಸರಿಸು;
			s = s->ದತ್ತಾಂಶ.sets[i];
		}
		ಆದರೆ (s)
			oe = s->ದತ್ತಾಂಶ.marked[idnum];
	}
	ಆದರೆ (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	ಹಿಂತಿರುಗಿ oe;
}

ಸ್ಥಿರ ರಚನೆ atom_str *ಗಮ್ಯ_atom(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ len)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hc = hc_str(s, len) % atom_ಕೋಷ್ಟಕ_sz;
	ರಚನೆ atom_str *c;

	ಗಾಗಿ (c = atom_ಕೋಷ್ಟಕ[hc]; c; c = c->ಮುಂದಿನ_atom)
		ಆದರೆ (c->str_len == len && !ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(s, c->str_dat, len))
			ಹಿಂತಿರುಗಿ c;

	c = mem_pool_alloc(&fi_mem_pool, ಗಾತ್ರ(ರಚನೆ atom_str) + len + 1);
	c->str_len = len;
	ಸ್ಮರಣೆ_ನಕಲು(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->ಮುಂದಿನ_atom = atom_ಕೋಷ್ಟಕ[hc];
	atom_ಕೋಷ್ಟಕ[hc] = c;
	atom_cnt++;
	ಹಿಂತಿರುಗಿ c;
}

ಸ್ಥಿರ ರಚನೆ branch *lookup_branch(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hc = hc_str(name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name)) % branch_ಕೋಷ್ಟಕ_sz;
	ರಚನೆ branch *b;

	ಗಾಗಿ (b = branch_ಕೋಷ್ಟಕ[hc]; b; b = b->ಕೋಷ್ಟಕ_ಮುಂದಿನ_branch)
		ಆದರೆ (!strcmp(name, b->name))
			ಹಿಂತಿರುಗಿ b;
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ರಚನೆ branch *new_branch(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hc = hc_str(name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name)) % branch_ಕೋಷ್ಟಕ_sz;
	ರಚನೆ branch *b = lookup_branch(name);

	ಆದರೆ (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	ಆದರೆ (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, ಗಾತ್ರ(ರಚನೆ branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->ಕೋಷ್ಟಕ_ಮುಂದಿನ_branch = branch_ಕೋಷ್ಟಕ[hc];
	b->branch_ವೃಕ್ಷ.versions[0].ಬಹುಲಕ = S_IFDIR;
	b->branch_ವೃಕ್ಷ.versions[1].ಬಹುಲಕ = S_IFDIR;
	b->ಅಂಕೆ_notes = 0;
	b->ಸಕ್ರಿಯ = 0;
	b->pack_id = MAX_PACK_ID;
	branch_ಕೋಷ್ಟಕ[hc] = b;
	branch_ಎಣಿಕೆ++;
	ಹಿಂತಿರುಗಿ b;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hc_entries(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	ಹಿಂತಿರುಗಿ cnt < avail_ವೃಕ್ಷ_ಕೋಷ್ಟಕ_sz ? cnt : avail_ವೃಕ್ಷ_ಕೋಷ್ಟಕ_sz - 1;
}

ಸ್ಥಿರ ರಚನೆ ವೃಕ್ಷ_content *new_ವೃಕ್ಷ_content(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cnt)
{
	ರಚನೆ avail_ವೃಕ್ಷ_content *f, *l = ಖಾಲಿ;
	ರಚನೆ ವೃಕ್ಷ_content *t;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hc = hc_entries(cnt);

	ಗಾಗಿ (f = avail_ವೃಕ್ಷ_ಕೋಷ್ಟಕ[hc]; f; l = f, f = f->ಮುಂದಿನ_avail)
		ಆದರೆ (f->ನಮೂದು_ಸಾಮರ್ಥ್ಯ >= cnt)
			ನಿಲ್ಲಿಸು;

	ಆದರೆ (f) {
		ಆದರೆ (l)
			l->ಮುಂದಿನ_avail = f->ಮುಂದಿನ_avail;
		ಇಲ್ಲದಿದ್ದರೆ
			avail_ವೃಕ್ಷ_ಕೋಷ್ಟಕ[hc] = f->ಮುಂದಿನ_avail;
	} ಇಲ್ಲದಿದ್ದರೆ {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, ಗಾತ್ರ(*t) + ಗಾತ್ರ(t->entries[0]) * cnt);
		f->ನಮೂದು_ಸಾಮರ್ಥ್ಯ = cnt;
	}

	t = (ರಚನೆ ವೃಕ್ಷ_content*)f;
	t->ನಮೂದು_ಎಣಿಕೆ = 0;
	t->delta_ಆಳ = 0;
	ಹಿಂತಿರುಗಿ t;
}

ಸ್ಥಿರ ಶೂನ್ಯ release_ವೃಕ್ಷ_ನಮೂದು(ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *e);
ಸ್ಥಿರ ಶೂನ್ಯ release_ವೃಕ್ಷ_content(ರಚನೆ ವೃಕ್ಷ_content *t)
{
	ರಚನೆ avail_ವೃಕ್ಷ_content *f = (ರಚನೆ avail_ವೃಕ್ಷ_content*)t;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hc = hc_entries(f->ನಮೂದು_ಸಾಮರ್ಥ್ಯ);
	f->ಮುಂದಿನ_avail = avail_ವೃಕ್ಷ_ಕೋಷ್ಟಕ[hc];
	avail_ವೃಕ್ಷ_ಕೋಷ್ಟಕ[hc] = f;
}

ಸ್ಥಿರ ಶೂನ್ಯ release_ವೃಕ್ಷ_content_recursive(ರಚನೆ ವೃಕ್ಷ_content *t)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;
	ಗಾಗಿ (i = 0; i < t->ನಮೂದು_ಎಣಿಕೆ; i++)
		release_ವೃಕ್ಷ_ನಮೂದು(t->entries[i]);
	release_ವೃಕ್ಷ_content(t);
}

ಸ್ಥಿರ ರಚನೆ ವೃಕ್ಷ_content *grow_ವೃಕ್ಷ_content(
	ರಚನೆ ವೃಕ್ಷ_content *t,
	ಪೂರ್ಣಾಂಕ amt)
{
	ರಚನೆ ವೃಕ್ಷ_content *r = new_ವೃಕ್ಷ_content(t->ನಮೂದು_ಎಣಿಕೆ + amt);
	r->ನಮೂದು_ಎಣಿಕೆ = t->ನಮೂದು_ಎಣಿಕೆ;
	r->delta_ಆಳ = t->delta_ಆಳ;
	COPY_ARRAY(r->entries, t->entries, t->ನಮೂದು_ಎಣಿಕೆ);
	release_ವೃಕ್ಷ_content(t);
	ಹಿಂತಿರುಗಿ r;
}

ಸ್ಥಿರ ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *new_ವೃಕ್ಷ_ನಮೂದು(ಶೂನ್ಯ)
{
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *e;

	ಆದರೆ (!avail_ವೃಕ್ಷ_ನಮೂದು) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ n = ವೃಕ್ಷ_ನಮೂದು_alloc;
		ವೃಕ್ಷ_ನಮೂದು_allocd += n * ಗಾತ್ರ(ರಚನೆ ವೃಕ್ಷ_ನಮೂದು);
		ALLOC_ARRAY(e, n);
		avail_ವೃಕ್ಷ_ನಮೂದು = e;
		ಆಗ (n-- > 1) {
			*((ಶೂನ್ಯ**)e) = e + 1;
			e++;
		}
		*((ಶೂನ್ಯ**)e) = ಖಾಲಿ;
	}

	e = avail_ವೃಕ್ಷ_ನಮೂದು;
	avail_ವೃಕ್ಷ_ನಮೂದು = *((ಶೂನ್ಯ**)e);
	ಹಿಂತಿರುಗಿ e;
}

ಸ್ಥಿರ ಶೂನ್ಯ release_ವೃಕ್ಷ_ನಮೂದು(ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *e)
{
	ಆದರೆ (e->ವೃಕ್ಷ)
		release_ವೃಕ್ಷ_content_recursive(e->ವೃಕ್ಷ);
	*((ಶೂನ್ಯ**)e) = avail_ವೃಕ್ಷ_ನಮೂದು;
	avail_ವೃಕ್ಷ_ನಮೂದು = e;
}

ಸ್ಥಿರ ರಚನೆ ವೃಕ್ಷ_content *dup_ವೃಕ್ಷ_content(ರಚನೆ ವೃಕ್ಷ_content *s)
{
	ರಚನೆ ವೃಕ್ಷ_content *d;
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *a, *b;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (!s)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	d = new_ವೃಕ್ಷ_content(s->ನಮೂದು_ಎಣಿಕೆ);
	ಗಾಗಿ (i = 0; i < s->ನಮೂದು_ಎಣಿಕೆ; i++) {
		a = s->entries[i];
		b = new_ವೃಕ್ಷ_ನಮೂದು();
		ಸ್ಮರಣೆ_ನಕಲು(b, a, ಗಾತ್ರ(*a));
		ಆದರೆ (a->ವೃಕ್ಷ && is_null_oid(&b->versions[1].oid))
			b->ವೃಕ್ಷ = dup_ವೃಕ್ಷ_content(a->ವೃಕ್ಷ);
		ಇಲ್ಲದಿದ್ದರೆ
			b->ವೃಕ್ಷ = ಖಾಲಿ;
		d->entries[i] = b;
	}
	d->ನಮೂದು_ಎಣಿಕೆ = s->ನಮೂದು_ಎಣಿಕೆ;
	d->delta_ಆಳ = s->delta_ಆಳ;

	ಹಿಂತಿರುಗಿ d;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಆರಂಭ_packfile(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf tmp_file = STRBUF_INIT;
	ರಚನೆ packed_git *p;
	ಪೂರ್ಣಾಂಕ pack_fd;

	pack_fd = odb_mkstemp(the_repository->objects, &tmp_file,
			      "pack/tmp_pack_XXXXXX");
	FLEX_ALLOC_STR(p, pack_name, tmp_file.buf);
	strbuf_release(&tmp_file);

	p->pack_fd = pack_fd;
	p->do_not_close = 1;
	p->repo = the_repository;
	pack_file = hashfd(the_repository->hash_algo, pack_fd, p->pack_name);

	pack_ದತ್ತಾಂಶ = p;
	pack_ಗಾತ್ರ = write_pack_ಶೀರ್ಷಿಕೆ(pack_file, 0);
	object_ಎಣಿಕೆ = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *create_ಸೂಚ್ಯಂಕ(ಶೂನ್ಯ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tmpfile;
	ರಚನೆ pack_idx_ನಮೂದು **idx, **c, **ಕೊನೆಯ;
	ರಚನೆ object_ನಮೂದು *e;
	ರಚನೆ object_ನಮೂದು_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_ಎಣಿಕೆ);
	c = idx;
	ಗಾಗಿ (o = ಖಂಡಗಳು; o; o = o->ಮುಂದಿನ_pool)
		ಗಾಗಿ (e = o->ಮುಂದಿನ_free; e-- != o->entries;)
			ಆದರೆ (pack_id == e->pack_id)
				*c++ = &e->idx;
	ಕೊನೆಯ = idx + object_ಎಣಿಕೆ;
	ಆದರೆ (c != ಕೊನೆಯ)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, ಖಾಲಿ, idx, object_ಎಣಿಕೆ,
				 &pack_idx_opts, pack_ದತ್ತಾಂಶ->hash);
	ಬಿಡುಗಡೆ(idx);
	ಹಿಂತಿರುಗಿ tmpfile;
}

ಸ್ಥಿರ ಅಕ್ಷರ *keep_pack(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *curr_ಸೂಚ್ಯಂಕ_name)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *keep_ಸಂದೇಶ = "fast-import";
	ರಚನೆ strbuf name = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ keep_fd;

	odb_pack_name(pack_ದತ್ತಾಂಶ->repo, &name, pack_ದತ್ತಾಂಶ->hash, "keep");
	keep_fd = ಸುರಕ್ಷಿತ_create_file_with_leading_directories(pack_ದತ್ತಾಂಶ->repo,
							    name.buf);
	ಆದರೆ (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_ಸಂದೇಶ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(keep_ಸಂದೇಶ));
	ಆದರೆ (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_ದತ್ತಾಂಶ->repo, &name, pack_ದತ್ತಾಂಶ->hash, "pack");
	ಆದರೆ (finalize_object_file(pack_ದತ್ತಾಂಶ->repo, pack_ದತ್ತಾಂಶ->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_ದತ್ತಾಂಶ->repo, &name, pack_ದತ್ತಾಂಶ->hash, "idx");
	ಆದರೆ (finalize_object_file(pack_ದತ್ತಾಂಶ->repo, curr_ಸೂಚ್ಯಂಕ_name, name.buf))
		die(_("cannot store index file"));
	ಬಿಡುಗಡೆ((ಶೂನ್ಯ *)curr_ಸೂಚ್ಯಂಕ_name);
	ಹಿಂತಿರುಗಿ strbuf_detach(&name, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ unkeep_all_packs(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf name = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ k;

	ಗಾಗಿ (k = 0; k < pack_id; k++) {
		ರಚನೆ packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ loosen_small_pack(ಸ್ಥಿರಾಂಕ ರಚನೆ packed_git *p)
{
	ರಚನೆ ಮಗು_process unpack = ಮಗು_PROCESS_INIT;

	ಆದರೆ (lseek(p->pack_fd, 0, SEEK_ಗಣ) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.ಒಳ = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_ಗಮ್ಯ_stderr = 1;
	strvec_ತಳ್ಳು(&unpack.ವಾದಗಳು, "unpack-objects");
	ಆದರೆ (!show_stats)
		strvec_ತಳ್ಳು(&unpack.ವಾದಗಳು, "-q");

	ಹಿಂತಿರುಗಿ ಸತತ_command(&unpack);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೊನೆ_packfile(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ running;

	ಆದರೆ (running || !pack_ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ;

	running = 1;
	clear_delta_ತಳ_cache();
	ಆದರೆ (object_ಎಣಿಕೆ) {
		ರಚನೆ odb_ಮೂಲ_files *files = odb_ಮೂಲ_files_downcast(pack_ದತ್ತಾಂಶ->repo->objects->ಮೂಲ);
		ರಚನೆ packed_git *new_p;
		ರಚನೆ object_id cur_pack_oid;
		ಅಕ್ಷರ *idx_name;
		ಪೂರ್ಣಾಂಕ i;
		ರಚನೆ branch *b;
		ರಚನೆ ಟ್ಯಾಗ್ *t;

		close_pack_windows(pack_ದತ್ತಾಂಶ);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_ಶೀರ್ಷಿಕೆ_footer(the_hash_algo, pack_ದತ್ತಾಂಶ->pack_fd,
					 pack_ದತ್ತಾಂಶ->hash, pack_ದತ್ತಾಂಶ->pack_name,
					 object_ಎಣಿಕೆ, cur_pack_oid.hash,
					 pack_ಗಾತ್ರ);

		ಆದರೆ (object_ಎಣಿಕೆ <= unpack_ಎಲ್ಲೆ) {
			ಆದರೆ (!loosen_small_pack(pack_ದತ್ತಾಂಶ)) {
				invalidate_pack_id(pack_id);
				ಹೋಗು discard_pack;
			}
		}

		close(pack_ದತ್ತಾಂಶ->pack_fd);
		idx_name = keep_pack(create_ಸೂಚ್ಯಂಕ());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_ತುಂಬು_pack(files->dirs->packed, idx_name, 1);
		ಆದರೆ (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		ಬಿಡುಗಡೆ(idx_name);

		/* Print the boundary */
		ಆದರೆ (pack_ಅಂಚುಗಳು) {
			fprintf(pack_ಅಂಚುಗಳು, "%s:", new_p->pack_name);
			ಗಾಗಿ (i = 0; i < branch_ಕೋಷ್ಟಕ_sz; i++) {
				ಗಾಗಿ (b = branch_ಕೋಷ್ಟಕ[i]; b; b = b->ಕೋಷ್ಟಕ_ಮುಂದಿನ_branch) {
					ಆದರೆ (b->pack_id == pack_id)
						fprintf(pack_ಅಂಚುಗಳು, " %s",
							oid_ಗಮ್ಯ_hex(&b->oid));
				}
			}
			ಗಾಗಿ (t = ಮೊದಲ_ಟ್ಯಾಗ್; t; t = t->ಮುಂದಿನ_ಟ್ಯಾಗ್) {
				ಆದರೆ (t->pack_id == pack_id)
					fprintf(pack_ಅಂಚುಗಳು, " %s",
						oid_ಗಮ್ಯ_hex(&t->oid));
			}
			fputc('\n', pack_ಅಂಚುಗಳು);
			fflush(pack_ಅಂಚುಗಳು);
		}

		pack_id++;
	}
	ಇಲ್ಲದಿದ್ದರೆ {
discard_pack:
		close(pack_ದತ್ತಾಂಶ->pack_fd);
		unlink_or_warn(pack_ದತ್ತಾಂಶ->pack_name);
	}
	FREE_AND_NULL(pack_ದತ್ತಾಂಶ);
	running = 0;

	/* We can't carry a delta across packfiles. */
	strbuf_release(&ಕೊನೆಯ_blob.ದತ್ತಾಂಶ);
	ಕೊನೆಯ_blob.ಸ್ಥಾನಾಂತರ = 0;
	ಕೊನೆಯ_blob.ಆಳ = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಚಕ್ರ_packfile(ಶೂನ್ಯ)
{
	ಕೊನೆ_packfile();
	ಆರಂಭ_packfile();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ store_object(
	ಎಣಿಕೆ object_type type,
	ರಚನೆ strbuf *dat,
	ರಚನೆ ಕೊನೆಯ_object *ಕೊನೆಯ,
	ರಚನೆ object_id *oidout,
	uintmax_t mark)
{
	ರಚನೆ odb_ಮೂಲ_files *files = odb_ಮೂಲ_files_downcast(the_repository->objects->ಮೂಲ);
	ಶೂನ್ಯ *ಹೊರಹೋಗು, *delta;
	ರಚನೆ object_ನಮೂದು *e;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ hdr[96];
	ರಚನೆ object_id oid;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ hdrlen, deltalen = 0;
	ರಚನೆ git_hash_ctx c;
	git_zstream s;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	hdrlen = format_object_ಶೀರ್ಷಿಕೆ((ಅಕ್ಷರ *)hdr, ಗಾತ್ರ(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	ಆದರೆ (oidout)
		oidcpy(oidout, &oid);

	e = ಸೇರಿಸು_object(&oid);
	ಆದರೆ (mark)
		ಸೇರಿಸು_mark(&marks, mark, e);
	ಆದರೆ (e->idx.ಸ್ಥಾನಾಂತರ) {
		duplicate_ಎಣಿಕೆ_by_type[type]++;
		ಹಿಂತಿರುಗಿ 1;
	}

	ಗಾಗಿ (ರಚನೆ odb_files_dir *dir = files->dirs; dir; dir = dir->ಮುಂದಿನ) {
		ಆದರೆ (!packfile_list_ಕಂಡುಹಿಡಿ_oid(packfile_store_get_packs(dir->packed), &oid))
			ಮುಂದುವರಿ;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.ಸ್ಥಾನಾಂತರ = 1; /* just not zero! */
		duplicate_ಎಣಿಕೆ_by_type[type]++;
		ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (ಕೊನೆಯ && ಕೊನೆಯ->ದತ್ತಾಂಶ.len && ಕೊನೆಯ->ದತ್ತಾಂಶ.buf && ಕೊನೆಯ->ಆಳ < max_ಆಳ
		&& dat->len > the_hash_algo->rawsz) {
		ಗಾತ್ರ_t deltalen_st;

		delta_ಎಣಿಕೆ_attempts_by_type[type]++;
		delta = diff_delta(ಕೊನೆಯ->ದತ್ತಾಂಶ.buf, ಕೊನೆಯ->ದತ್ತಾಂಶ.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(deltalen_st);
	} ಇಲ್ಲದಿದ್ದರೆ
		delta = ಖಾಲಿ;

	git_deflate_init(&s, cfg->pack_compression_level);
	ಆದರೆ (delta) {
		s.ಮುಂದಿನ_ಒಳ = delta;
		s.avail_ಒಳ = deltalen;
	} ಇಲ್ಲದಿದ್ದರೆ {
		s.ಮುಂದಿನ_ಒಳ = (ಶೂನ್ಯ *)dat->buf;
		s.avail_ಒಳ = dat->len;
	}
	s.avail_ಹೊರಹೋಗು = git_deflate_bound(&s, s.avail_ಒಳ);
	s.ಮುಂದಿನ_ಹೊರಹೋಗು = ಹೊರಹೋಗು = xmalloc(s.avail_ಹೊರಹೋಗು);
	ಆಗ (git_deflate(&s, Z_ಮುಕ್ತಾಯ) == Z_OK)
		; /* nothing */
	git_deflate_ಕೊನೆ(&s);

	/* Determine if we should auto-checkpoint. */
	ಆದರೆ ((max_packsize
		&& (pack_ಗಾತ್ರ + PACK_ಗಾತ್ರ_ಮಿತಿ + s.ಒಟ್ಟು_ಹೊರಹೋಗು) > max_packsize)
		|| (pack_ಗಾತ್ರ + PACK_ಗಾತ್ರ_ಮಿತಿ + s.ಒಟ್ಟು_ಹೊರಹೋಗು) < pack_ಗಾತ್ರ) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		ಚಕ್ರ_packfile();

		/* We cannot carry a delta into the new pack. */
		ಆದರೆ (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.ಮುಂದಿನ_ಒಳ = (ಶೂನ್ಯ *)dat->buf;
			s.avail_ಒಳ = dat->len;
			s.avail_ಹೊರಹೋಗು = git_deflate_bound(&s, s.avail_ಒಳ);
			s.ಮುಂದಿನ_ಹೊರಹೋಗು = ಹೊರಹೋಗು = xrealloc(ಹೊರಹೋಗು, s.avail_ಹೊರಹೋಗು);
			ಆಗ (git_deflate(&s, Z_ಮುಕ್ತಾಯ) == Z_OK)
				; /* nothing */
			git_deflate_ಕೊನೆ(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.ಸ್ಥಾನಾಂತರ = pack_ಗಾತ್ರ;
	object_ಎಣಿಕೆ++;
	object_ಎಣಿಕೆ_by_type[type]++;

	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_begin(pack_file);

	ಆದರೆ (delta) {
		off_t ofs = e->idx.ಸ್ಥಾನಾಂತರ - ಕೊನೆಯ->ಸ್ಥಾನಾಂತರ;
		ಚಿಹ್ನೆರಹಿತ pos = ಗಾತ್ರ(hdr) - 1;

		delta_ಎಣಿಕೆ_by_type[type]++;
		e->ಆಳ = ಕೊನೆಯ->ಆಳ + 1;

		hdrlen = encode_ಒಳ_pack_object_ಶೀರ್ಷಿಕೆ(hdr, ಗಾತ್ರ(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_ಗಾತ್ರ += hdrlen;

		hdr[pos] = ofs & 127;
		ಆಗ (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, ಗಾತ್ರ(hdr) - pos);
		pack_ಗಾತ್ರ += ಗಾತ್ರ(hdr) - pos;
	} ಇಲ್ಲದಿದ್ದರೆ {
		e->ಆಳ = 0;
		hdrlen = encode_ಒಳ_pack_object_ಶೀರ್ಷಿಕೆ(hdr, ಗಾತ್ರ(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_ಗಾತ್ರ += hdrlen;
	}

	hashwrite(pack_file, ಹೊರಹೋಗು, s.ಒಟ್ಟು_ಹೊರಹೋಗು);
	pack_ಗಾತ್ರ += s.ಒಟ್ಟು_ಹೊರಹೋಗು;

	e->idx.ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32 = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_ಕೊನೆ(pack_file);

	ಬಿಡುಗಡೆ(ಹೊರಹೋಗು);
	ಬಿಡುಗಡೆ(delta);
	ಆದರೆ (ಕೊನೆಯ) {
		ಆದರೆ (ಕೊನೆಯ->no_ಅದಲುಬದಲು) {
			ಕೊನೆಯ->ದತ್ತಾಂಶ = *dat;
		} ಇಲ್ಲದಿದ್ದರೆ {
			strbuf_ಅದಲುಬದಲು(&ಕೊನೆಯ->ದತ್ತಾಂಶ, dat);
		}
		ಕೊನೆಯ->ಸ್ಥಾನಾಂತರ = e->idx.ಸ್ಥಾನಾಂತರ;
		ಕೊನೆಯ->ಆಳ = e->ಆಳ;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ truncate_pack(ರಚನೆ hashfile_checkpoint *checkpoint)
{
	ಆದರೆ (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_ಗಾತ್ರ = checkpoint->ಸ್ಥಾನಾಂತರ;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಹರಿವು_blob(uintmax_t len, ರಚನೆ object_id *oidout, uintmax_t mark)
{
	ರಚನೆ odb_ಮೂಲ_files *files = odb_ಮೂಲ_files_downcast(the_repository->objects->ಮೂಲ);
	ಗಾತ್ರ_t ಒಳ_sz = 64 * 1024, ಹೊರಹೋಗು_sz = 64 * 1024;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಒಳ_buf = xmalloc(ಒಳ_sz);
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಹೊರಹೋಗು_buf = xmalloc(ಹೊರಹೋಗು_sz);
	ರಚನೆ object_ನಮೂದು *e;
	ರಚನೆ object_id oid;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ hdrlen;
	off_t ಸ್ಥಾನಾಂತರ;
	ರಚನೆ git_hash_ctx c;
	git_zstream s;
	ರಚನೆ hashfile_checkpoint checkpoint;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = Z_OK;

	/* Determine if we should auto-checkpoint. */
	ಆದರೆ ((max_packsize
		&& (pack_ಗಾತ್ರ + PACK_ಗಾತ್ರ_ಮಿತಿ + len) > max_packsize)
		|| (pack_ಗಾತ್ರ + PACK_ಗಾತ್ರ_ಮಿತಿ + len) < pack_ಗಾತ್ರ)
		ಚಕ್ರ_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	ಸ್ಥಾನಾಂತರ = checkpoint.ಸ್ಥಾನಾಂತರ;

	hdrlen = format_object_ಶೀರ್ಷಿಕೆ((ಅಕ್ಷರ *)ಹೊರಹೋಗು_buf, ಹೊರಹೋಗು_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, ಹೊರಹೋಗು_buf, hdrlen);

	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_ಒಳ_pack_object_ಶೀರ್ಷಿಕೆ(ಹೊರಹೋಗು_buf, ಹೊರಹೋಗು_sz, OBJ_BLOB, len);

	s.ಮುಂದಿನ_ಹೊರಹೋಗು = ಹೊರಹೋಗು_buf + hdrlen;
	s.avail_ಹೊರಹೋಗು = ಹೊರಹೋಗು_sz - hdrlen;

	ಆಗ (ಸ್ಥಿತಿ_2 != Z_ಹರಿವು_ಕೊನೆ) {
		ಆದರೆ (0 < len && !s.avail_ಒಳ) {
			ಗಾತ್ರ_t cnt = ಒಳ_sz < len ? ಒಳ_sz : (ಗಾತ್ರ_t)len;
			ಗಾತ್ರ_t n = fread(ಒಳ_buf, 1, cnt, stdin);
			ಆದರೆ (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, ಒಳ_buf, n);
			s.ಮುಂದಿನ_ಒಳ = ಒಳ_buf;
			s.avail_ಒಳ = n;
			len -= n;
		}

		ಸ್ಥಿತಿ_2 = git_deflate(&s, len ? 0 : Z_ಮುಕ್ತಾಯ);

		ಆದರೆ (!s.avail_ಹೊರಹೋಗು || ಸ್ಥಿತಿ_2 == Z_ಹರಿವು_ಕೊನೆ) {
			ಗಾತ್ರ_t n = s.ಮುಂದಿನ_ಹೊರಹೋಗು - ಹೊರಹೋಗು_buf;
			hashwrite(pack_file, ಹೊರಹೋಗು_buf, n);
			pack_ಗಾತ್ರ += n;
			s.ಮುಂದಿನ_ಹೊರಹೋಗು = ಹೊರಹೋಗು_buf;
			s.avail_ಹೊರಹೋಗು = ಹೊರಹೋಗು_sz;
		}

		ಆಯ್ಕೆ (ಸ್ಥಿತಿ_2) {
		ಸ್ಥಿತಿ Z_OK:
		ಸ್ಥಿತಿ Z_BUF_ERROR:
		ಸ್ಥಿತಿ Z_ಹರಿವು_ಕೊನೆ:
			ಮುಂದುವರಿ;
		ಪೂರ್ವನಿಯೋಜಿತ:
			die(_("unexpected deflate failure: %d"), ಸ್ಥಿತಿ_2);
		}
	}
	git_deflate_ಕೊನೆ(&s);
	git_hash_final_oid(&oid, &c);

	ಆದರೆ (oidout)
		oidcpy(oidout, &oid);

	e = ಸೇರಿಸು_object(&oid);

	ಆದರೆ (mark)
		ಸೇರಿಸು_mark(&marks, mark, e);

	ಆದರೆ (e->idx.ಸ್ಥಾನಾಂತರ) {
		duplicate_ಎಣಿಕೆ_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಗಾಗಿ (ರಚನೆ odb_files_dir *dir = files->dirs; dir; dir = dir->ಮುಂದಿನ) {
		ಆದರೆ (!packfile_list_ಕಂಡುಹಿಡಿ_oid(packfile_store_get_packs(dir->packed), &oid))
			ಮುಂದುವರಿ;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.ಸ್ಥಾನಾಂತರ = 1; /* just not zero! */
		duplicate_ಎಣಿಕೆ_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	e->ಆಳ = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.ಸ್ಥಾನಾಂತರ = ಸ್ಥಾನಾಂತರ;
	e->idx.ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32 = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_ಕೊನೆ(pack_file);
	object_ಎಣಿಕೆ++;
	object_ಎಣಿಕೆ_by_type[OBJ_BLOB]++;

ಹೊರಹೋಗು:
	ಬಿಡುಗಡೆ(ಒಳ_buf);
	ಬಿಡುಗಡೆ(ಹೊರಹೋಗು_buf);
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
ಸ್ಥಿರ ಶೂನ್ಯ *gfi_unpack_ನಮೂದು(
	ರಚನೆ object_ನಮೂದು *oe,
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *sizep)
{
	ಎಣಿಕೆ object_type type;
	ಗಾತ್ರ_t ಗಾತ್ರ_st = 0;
	ಶೂನ್ಯ *ದತ್ತಾಂಶ;
	ರಚನೆ packed_git *p = all_packs[oe->pack_id];
	ಆದರೆ (p == pack_ದತ್ತಾಂಶ && p->pack_ಗಾತ್ರ < (pack_ಗಾತ್ರ + the_hash_algo->rawsz)) {
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
		p->pack_ಗಾತ್ರ = pack_ಗಾತ್ರ + the_hash_algo->rawsz;
	}
	ದತ್ತಾಂಶ = unpack_ನಮೂದು(the_repository, p, oe->idx.ಸ್ಥಾನಾಂತರ, &type, &ಗಾತ್ರ_st);
	ಆದರೆ (sizep)
		*sizep = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(ಗಾತ್ರ_st);
	ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ ತುಂಬು_ವೃಕ್ಷ(ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *ಬೇರು)
{
	ರಚನೆ object_id *oid = &ಬೇರು->versions[1].oid;
	ರಚನೆ object_ನಮೂದು *myoe;
	ರಚನೆ ವೃಕ್ಷ_content *t;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;
	ಅಕ್ಷರ *buf;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *c;

	ಬೇರು->ವೃಕ್ಷ = t = new_ವೃಕ್ಷ_content(8);
	ಆದರೆ (is_null_oid(oid))
		ಹಿಂತಿರುಗಿ;

	myoe = ಕಂಡುಹಿಡಿ_object(oid);
	ಆದರೆ (myoe && myoe->pack_id != MAX_PACK_ID) {
		ಆದರೆ (myoe->type != OBJ_ವೃಕ್ಷ)
			die(_("not a tree: %s"), oid_ಗಮ್ಯ_hex(oid));
		t->delta_ಆಳ = myoe->ಆಳ;
		buf = gfi_unpack_ನಮೂದು(myoe, &ಗಾತ್ರ_2);
		ಆದರೆ (!buf)
			die(_("can't load tree %s"), oid_ಗಮ್ಯ_hex(oid));
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಎಣಿಕೆ object_type type;
		ಗಾತ್ರ_t ಗಾತ್ರ_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &ಗಾತ್ರ_st);
		ಗಾತ್ರ_2 = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(ಗಾತ್ರ_st);
		ಆದರೆ (!buf || type != OBJ_ವೃಕ್ಷ)
			die(_("can't load tree %s"), oid_ಗಮ್ಯ_hex(oid));
	}

	c = buf;
	ಆಗ (c != (buf + ಗಾತ್ರ_2)) {
		ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *e = new_ವೃಕ್ಷ_ನಮೂದು();

		ಆದರೆ (t->ನಮೂದು_ಎಣಿಕೆ == t->ನಮೂದು_ಸಾಮರ್ಥ್ಯ)
			ಬೇರು->ವೃಕ್ಷ = t = grow_ವೃಕ್ಷ_content(t, t->ನಮೂದು_ಎಣಿಕೆ);
		t->entries[t->ನಮೂದು_ಎಣಿಕೆ++] = e;

		e->ವೃಕ್ಷ = ಖಾಲಿ;
		c = parse_ಬಹುಲಕ(c, &e->versions[1].ಬಹುಲಕ);
		ಆದರೆ (!c)
			die(_("corrupt mode in %s"), oid_ಗಮ್ಯ_hex(oid));
		e->versions[0].ಬಹುಲಕ = e->versions[1].ಬಹುಲಕ;
		e->name = ಗಮ್ಯ_atom(c, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	ಬಿಡುಗಡೆ(buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tecmp0 (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *_a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *_b)
{
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *a = *((ರಚನೆ ವೃಕ್ಷ_ನಮೂದು**)_a);
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *b = *((ರಚನೆ ವೃಕ್ಷ_ನಮೂದು**)_b);
	ಹಿಂತಿರುಗಿ ತಳ_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].ಬಹುಲಕ,
		b->name->str_dat, b->name->str_len, b->versions[0].ಬಹುಲಕ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tecmp1 (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *_a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *_b)
{
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *a = *((ರಚನೆ ವೃಕ್ಷ_ನಮೂದು**)_a);
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *b = *((ರಚನೆ ವೃಕ್ಷ_ನಮೂದು**)_b);
	ಹಿಂತಿರುಗಿ ತಳ_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].ಬಹುಲಕ,
		b->name->str_dat, b->name->str_len, b->versions[1].ಬಹುಲಕ);
}

ಸ್ಥಿರ ಶೂನ್ಯ mktree(ರಚನೆ ವೃಕ್ಷ_content *t, ಪೂರ್ಣಾಂಕ v, ರಚನೆ strbuf *b)
{
	ಗಾತ್ರ_t maxlen = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (!v)
		QSORT(t->entries, t->ನಮೂದು_ಎಣಿಕೆ, tecmp0);
	ಇಲ್ಲದಿದ್ದರೆ
		QSORT(t->entries, t->ನಮೂದು_ಎಣಿಕೆ, tecmp1);

	ಗಾಗಿ (i = 0; i < t->ನಮೂದು_ಎಣಿಕೆ; i++) {
		ಆದರೆ (t->entries[i]->versions[v].ಬಹುಲಕ)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	ಗಾಗಿ (i = 0; i < t->ನಮೂದು_ಎಣಿಕೆ; i++) {
		ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *e = t->entries[i];
		ಆದರೆ (!e->versions[v].ಬಹುಲಕ)
			ಮುಂದುವರಿ;
		strbuf_addf(b, "%o %s%c",
			(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)(e->versions[v].ಬಹುಲಕ & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_ಕೂಡಿಸು(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ store_ವೃಕ್ಷ(ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *ಬೇರು)
{
	ರಚನೆ ವೃಕ್ಷ_content *t;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i, j, del;
	ರಚನೆ ಕೊನೆಯ_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	ರಚನೆ object_ನಮೂದು *le = ಖಾಲಿ;

	ಆದರೆ (!is_null_oid(&ಬೇರು->versions[1].oid))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!ಬೇರು->ವೃಕ್ಷ)
		ತುಂಬು_ವೃಕ್ಷ(ಬೇರು);
	t = ಬೇರು->ವೃಕ್ಷ;

	ಗಾಗಿ (i = 0; i < t->ನಮೂದು_ಎಣಿಕೆ; i++) {
		ಆದರೆ (t->entries[i]->ವೃಕ್ಷ)
			store_ವೃಕ್ಷ(t->entries[i]);
	}

	ಆದರೆ (!(ಬೇರು->versions[0].ಬಹುಲಕ & NO_DELTA))
		le = ಕಂಡುಹಿಡಿ_object(&ಬೇರು->versions[0].oid);
	ಆದರೆ (S_ISDIR(ಬೇರು->versions[0].ಬಹುಲಕ) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_ವೃಕ್ಷ);
		lo.ದತ್ತಾಂಶ = old_ವೃಕ್ಷ;
		lo.ಸ್ಥಾನಾಂತರ = le->idx.ಸ್ಥಾನಾಂತರ;
		lo.ಆಳ = t->delta_ಆಳ;
	}

	mktree(t, 1, &new_ವೃಕ್ಷ);
	store_object(OBJ_ವೃಕ್ಷ, &new_ವೃಕ್ಷ, &lo, &ಬೇರು->versions[1].oid, 0);

	t->delta_ಆಳ = lo.ಆಳ;
	ಗಾಗಿ (i = 0, j = 0, del = 0; i < t->ನಮೂದು_ಎಣಿಕೆ; i++) {
		ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *e = t->entries[i];
		ಆದರೆ (e->versions[1].ಬಹುಲಕ) {
			e->versions[0].ಬಹುಲಕ = e->versions[1].ಬಹುಲಕ;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} ಇಲ್ಲದಿದ್ದರೆ {
			release_ವೃಕ್ಷ_ನಮೂದು(e);
			del++;
		}
	}
	t->ನಮೂದು_ಎಣಿಕೆ -= del;
}

ಸ್ಥಿರ ಶೂನ್ಯ ವೃಕ್ಷ_content_replace(
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *ಬೇರು,
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
	ಸ್ಥಿರಾಂಕ uint16_t ಬಹುಲಕ,
	ರಚನೆ ವೃಕ್ಷ_content *newtree)
{
	ಆದರೆ (!S_ISDIR(ಬಹುಲಕ))
		die(_("root cannot be a non-directory"));
	oidclr(&ಬೇರು->versions[0].oid, the_repository->hash_algo);
	oidcpy(&ಬೇರು->versions[1].oid, oid);
	ಆದರೆ (ಬೇರು->ವೃಕ್ಷ)
		release_ವೃಕ್ಷ_content_recursive(ಬೇರು->ವೃಕ್ಷ);
	ಬೇರು->ವೃಕ್ಷ = newtree;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವೃಕ್ಷ_content_ಗಣ(
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *ಬೇರು,
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p,
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
	ಸ್ಥಿರಾಂಕ uint16_t ಬಹುಲಕ,
	ರಚನೆ ವೃಕ್ಷ_content *subtree)
{
	ರಚನೆ ವೃಕ್ಷ_content *t;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *slash1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i, n;
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	ಆದರೆ (!n)
		die(_("empty path component found in input"));
	ಆದರೆ (!*slash1 && !S_ISDIR(ಬಹುಲಕ) && subtree)
		die(_("non-directories cannot have subtrees"));

	ಆದರೆ (!ಬೇರು->ವೃಕ್ಷ)
		ತುಂಬು_ವೃಕ್ಷ(ಬೇರು);
	t = ಬೇರು->ವೃಕ್ಷ;
	ಗಾಗಿ (i = 0; i < t->ನಮೂದು_ಎಣಿಕೆ; i++) {
		e = t->entries[i];
		ಆದರೆ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ಆದರೆ (!*slash1) {
				ಆದರೆ (!S_ISDIR(ಬಹುಲಕ)
						&& e->versions[1].ಬಹುಲಕ == ಬಹುಲಕ
						&& oideq(&e->versions[1].oid, oid))
					ಹಿಂತಿರುಗಿ 0;
				e->versions[1].ಬಹುಲಕ = ಬಹುಲಕ;
				oidcpy(&e->versions[1].oid, oid);
				ಆದರೆ (e->ವೃಕ್ಷ)
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
				ಆದರೆ (S_ISDIR(e->versions[0].ಬಹುಲಕ))
					e->versions[0].ಬಹುಲಕ |= NO_DELTA;

				oidclr(&ಬೇರು->versions[1].oid, the_repository->hash_algo);
				ಹಿಂತಿರುಗಿ 1;
			}
			ಆದರೆ (!S_ISDIR(e->versions[1].ಬಹುಲಕ)) {
				e->ವೃಕ್ಷ = new_ವೃಕ್ಷ_content(8);
				e->versions[1].ಬಹುಲಕ = S_IFDIR;
			}
			ಆದರೆ (!e->ವೃಕ್ಷ)
				ತುಂಬು_ವೃಕ್ಷ(e);
			ಆದರೆ (ವೃಕ್ಷ_content_ಗಣ(e, slash1 + 1, oid, ಬಹುಲಕ, subtree)) {
				oidclr(&ಬೇರು->versions[1].oid, the_repository->hash_algo);
				ಹಿಂತಿರುಗಿ 1;
			}
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	ಆದರೆ (t->ನಮೂದು_ಎಣಿಕೆ == t->ನಮೂದು_ಸಾಮರ್ಥ್ಯ)
		ಬೇರು->ವೃಕ್ಷ = t = grow_ವೃಕ್ಷ_content(t, t->ನಮೂದು_ಎಣಿಕೆ);
	e = new_ವೃಕ್ಷ_ನಮೂದು();
	e->name = ಗಮ್ಯ_atom(p, n);
	e->versions[0].ಬಹುಲಕ = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->ನಮೂದು_ಎಣಿಕೆ++] = e;
	ಆದರೆ (*slash1) {
		e->ವೃಕ್ಷ = new_ವೃಕ್ಷ_content(8);
		e->versions[1].ಬಹುಲಕ = S_IFDIR;
		ವೃಕ್ಷ_content_ಗಣ(e, slash1 + 1, oid, ಬಹುಲಕ, subtree);
	} ಇಲ್ಲದಿದ್ದರೆ {
		e->ವೃಕ್ಷ = subtree;
		e->versions[1].ಬಹುಲಕ = ಬಹುಲಕ;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&ಬೇರು->versions[1].oid, the_repository->hash_algo);
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವೃಕ್ಷ_content_remove(
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *ಬೇರು,
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p,
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *backup_ಎಲೆ,
	ಪೂರ್ಣಾಂಕ allow_ಬೇರು)
{
	ರಚನೆ ವೃಕ್ಷ_content *t;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *slash1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i, n;
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	ಆದರೆ (!ಬೇರು->ವೃಕ್ಷ)
		ತುಂಬು_ವೃಕ್ಷ(ಬೇರು);

	ಆದರೆ (!*p && allow_ಬೇರು) {
		e = ಬೇರು;
		ಹೋಗು del_ನಮೂದು;
	}

	t = ಬೇರು->ವೃಕ್ಷ;
	ಗಾಗಿ (i = 0; i < t->ನಮೂದು_ಎಣಿಕೆ; i++) {
		e = t->entries[i];
		ಆದರೆ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ಆದರೆ (*slash1 && !S_ISDIR(e->versions[1].ಬಹುಲಕ))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				ಹಿಂತಿರುಗಿ 1;
			ಆದರೆ (!*slash1 || !S_ISDIR(e->versions[1].ಬಹುಲಕ))
				ಹೋಗು del_ನಮೂದು;
			ಆದರೆ (!e->ವೃಕ್ಷ)
				ತುಂಬು_ವೃಕ್ಷ(e);
			ಆದರೆ (ವೃಕ್ಷ_content_remove(e, slash1 + 1, backup_ಎಲೆ, 0)) {
				ಗಾಗಿ (n = 0; n < e->ವೃಕ್ಷ->ನಮೂದು_ಎಣಿಕೆ; n++) {
					ಆದರೆ (e->ವೃಕ್ಷ->entries[n]->versions[1].ಬಹುಲಕ) {
						oidclr(&ಬೇರು->versions[1].oid,
						       the_repository->hash_algo);
						ಹಿಂತಿರುಗಿ 1;
					}
				}
				backup_ಎಲೆ = ಖಾಲಿ;
				ಹೋಗು del_ನಮೂದು;
			}
			ಹಿಂತಿರುಗಿ 0;
		}
	}
	ಹಿಂತಿರುಗಿ 0;

del_ನಮೂದು:
	ಆದರೆ (backup_ಎಲೆ)
		ಸ್ಮರಣೆ_ನಕಲು(backup_ಎಲೆ, e, ಗಾತ್ರ(*backup_ಎಲೆ));
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (e->ವೃಕ್ಷ)
		release_ವೃಕ್ಷ_content_recursive(e->ವೃಕ್ಷ);
	e->ವೃಕ್ಷ = ಖಾಲಿ;
	e->versions[1].ಬಹುಲಕ = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&ಬೇರು->versions[1].oid, the_repository->hash_algo);
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವೃಕ್ಷ_content_get(
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *ಬೇರು,
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p,
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *ಎಲೆ,
	ಪೂರ್ಣಾಂಕ allow_ಬೇರು)
{
	ರಚನೆ ವೃಕ್ಷ_content *t;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *slash1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i, n;
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	ಆದರೆ (!n && !allow_ಬೇರು)
		die(_("empty path component found in input"));

	ಆದರೆ (!ಬೇರು->ವೃಕ್ಷ)
		ತುಂಬು_ವೃಕ್ಷ(ಬೇರು);

	ಆದರೆ (!n) {
		e = ಬೇರು;
		ಹೋಗು ಸಿಕ್ಕಿತು_ನಮೂದು;
	}

	t = ಬೇರು->ವೃಕ್ಷ;
	ಗಾಗಿ (i = 0; i < t->ನಮೂದು_ಎಣಿಕೆ; i++) {
		e = t->entries[i];
		ಆದರೆ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ಆದರೆ (!*slash1)
				ಹೋಗು ಸಿಕ್ಕಿತು_ನಮೂದು;
			ಆದರೆ (!S_ISDIR(e->versions[1].ಬಹುಲಕ))
				ಹಿಂತಿರುಗಿ 0;
			ಆದರೆ (!e->ವೃಕ್ಷ)
				ತುಂಬು_ವೃಕ್ಷ(e);
			ಹಿಂತಿರುಗಿ ವೃಕ್ಷ_content_get(e, slash1 + 1, ಎಲೆ, 0);
		}
	}
	ಹಿಂತಿರುಗಿ 0;

ಸಿಕ್ಕಿತು_ನಮೂದು:
	ಸ್ಮರಣೆ_ನಕಲು(ಎಲೆ, e, ಗಾತ್ರ(*ಎಲೆ));
	ಆದರೆ (e->ವೃಕ್ಷ && is_null_oid(&e->versions[1].oid))
		ಎಲೆ->ವೃಕ್ಷ = dup_ವೃಕ್ಷ_content(e->ವೃಕ್ಷ);
	ಇಲ್ಲದಿದ್ದರೆ
		ಎಲೆ->ವೃಕ್ಷ = ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_branch(ರಚನೆ branch *b)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ = "fast-import";
	ರಚನೆ ref_transaction *transaction;
	ರಚನೆ object_id old_oid;
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *replace_ಪೂರ್ವಪ್ರತ್ಯಯ = "refs/replace/";

	ಆದರೆ (starts_with(b->name, replace_ಪೂರ್ವಪ್ರತ್ಯಯ) &&
	    !strcmp(b->name + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(replace_ಪೂರ್ವಪ್ರತ್ಯಯ),
		    oid_ಗಮ್ಯ_hex(&b->oid))) {
		ಆದರೆ (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_ಗಮ್ಯ_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				ಖಾಲಿ, b->name, ಖಾಲಿ, 0);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (is_null_oid(&b->oid)) {
		ಆದರೆ (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					ಖಾಲಿ, b->name, ಖಾಲಿ, 0);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	ಆದರೆ (!force_update && !is_null_oid(&old_oid)) {
		ರಚನೆ commit *old_cmit, *new_cmit;
		ಪೂರ್ಣಾಂಕ ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		ಆದರೆ (!old_cmit || !new_cmit)
			ಹಿಂತಿರುಗಿ error(_("branch %s is missing commits."), b->name);

		ret = repo_ಒಳ_ವಿಲೀನ_bases(the_repository, old_cmit, new_cmit);
		ಆದರೆ (ret < 0)
			ನಿರ್ಗಮಿಸು(128);
		ಆದರೆ (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_ಗಮ್ಯ_hex(&b->oid),
				oid_ಗಮ್ಯ_hex(&old_oid));
			ಹಿಂತಿರುಗಿ -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ಆದರೆ (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   ಖಾಲಿ, ಖಾಲಿ, 0, ಸಂದೇಶ, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		ಹಿಂತಿರುಗಿ -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_branches(ಶೂನ್ಯ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ branch *b;

	ಗಾಗಿ (i = 0; i < branch_ಕೋಷ್ಟಕ_sz; i++) {
		ಗಾಗಿ (b = branch_ಕೋಷ್ಟಕ[i]; b; b = b->ಕೋಷ್ಟಕ_ಮುಂದಿನ_branch)
			failure |= update_branch(b);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_ಟ್ಯಾಗುಗಳು(ಶೂನ್ಯ)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ = "fast-import";
	ರಚನೆ ಟ್ಯಾಗ್ *t;
	ರಚನೆ strbuf ref_name = STRBUF_INIT;
	ರಚನೆ strbuf err = STRBUF_INIT;
	ರಚನೆ ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ಆದರೆ (!transaction) {
		failure |= error("%s", err.buf);
		ಹೋಗು cleanup;
	}
	ಗಾಗಿ (t = ಮೊದಲ_ಟ್ಯಾಗ್; t; t = t->ಮುಂದಿನ_ಟ್ಯಾಗ್) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		ಆದರೆ (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ,
					   0, ಸಂದೇಶ, &err)) {
			failure |= error("%s", err.buf);
			ಹೋಗು cleanup;
		}
	}
	ಆದರೆ (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_marks(ಶೂನ್ಯ)
{
	ರಚನೆ ಬೀಗ_file mark_ಬೀಗ = ಬೀಗ_INIT;
	FILE *f;

	ಆದರೆ (!export_marks_file || (import_marks_file && !import_marks_file_ಮುಗಿದ))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (ಸುರಕ್ಷಿತ_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (hold_ಬೀಗ_file_for_update(&mark_ಬೀಗ, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		ಹಿಂತಿರುಗಿ;
	}

	f = fdopen_ಬೀಗ_file(&mark_ಬೀಗ, "w");
	ಆದರೆ (!f) {
		ಪೂರ್ಣಾಂಕ saved_errno = errno;
		rollback_ಬೀಗ_file(&mark_ಬೀಗ);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		ಹಿಂತಿರುಗಿ;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	ಆದರೆ (commit_ಬೀಗ_file(&mark_ಬೀಗ)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		ಹಿಂತಿರುಗಿ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_object_ನಮೂದು(ರಚನೆ mark_ಗಣ **s, ರಚನೆ object_id *oid, uintmax_t mark)
{
	ರಚನೆ object_ನಮೂದು *e;
	e = ಕಂಡುಹಿಡಿ_object(oid);
	ಆದರೆ (!e) {
		ಎಣಿಕೆ object_type type = odb_read_object_info(the_repository->objects,
							     oid, ಖಾಲಿ);
		ಆದರೆ (type < 0)
			die(_("object not found: %s"), oid_ಗಮ್ಯ_hex(oid));
		e = ಸೇರಿಸು_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.ಸ್ಥಾನಾಂತರ = 1; /* just not zero! */
	}
	ಸೇರಿಸು_mark(s, mark, e);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_oid_ನಮೂದು(ರಚನೆ mark_ಗಣ **s, ರಚನೆ object_id *oid, uintmax_t mark)
{
	ಸೇರಿಸು_mark(s, mark, xmemdupz(oid, ಗಾತ್ರ(*oid)));
}

ಸ್ಥಿರ ಶೂನ್ಯ read_mark_file(ರಚನೆ mark_ಗಣ **s, FILE *f, mark_ಗಣ_inserter_t inserter)
{
	ಅಕ್ಷರ ಸಾಲು[512];
	ಆಗ (fgets(ಸಾಲು, ಗಾತ್ರ(ಸಾಲು), f)) {
		uintmax_t mark;
		ಅಕ್ಷರ *ಕೊನೆ;
		ರಚನೆ object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, ಗಾತ್ರ(oid.hash));

		ಕೊನೆ = strchr(ಸಾಲು, '\n');
		ಆದರೆ (ಸಾಲು[0] != ':' || !ಕೊನೆ)
			die(_("corrupt mark line: %s"), ಸಾಲು);
		*ಕೊನೆ = 0;
		mark = strtoumax(ಸಾಲು + 1, &ಕೊನೆ, 10);
		ಆದರೆ (!mark || ಕೊನೆ == ಸಾಲು + 1
			|| *ಕೊನೆ != ' '
			|| get_oid_hex_any(ಕೊನೆ + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), ಸಾಲು);
		inserter(s, &oid, mark);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ read_marks(ಶೂನ್ಯ)
{
	FILE *f = fopen(import_marks_file, "r");
	ಆದರೆ (f)
		;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (import_marks_file_ignore_missing && errno == ENOENT)
		ಹೋಗು ಮುಗಿದ; /* Marks file does not exist */
	ಇಲ್ಲದಿದ್ದರೆ
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, ಸೇರಿಸು_object_ನಮೂದು);
	fclose(f);
ಮುಗಿದ:
	import_marks_file_ಮುಗಿದ = 1;
}


ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_ಮುಂದಿನ_command(ರಚನೆ fast_import_state *state)
{
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stdin_eof = 0;

	ಆದರೆ (stdin_eof) {
		unread_command_buf = 0;
		ಹಿಂತಿರುಗಿ EOF;
	}

	ಗಾಗಿ (;;) {
		ಆದರೆ (unread_command_buf) {
			unread_command_buf = 0;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			ಆದರೆ (stdin_eof)
				ಹಿಂತಿರುಗಿ EOF;

			ಆದರೆ (!state->ನೋಡಿದ_ದತ್ತಾಂಶ_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			ಆದರೆ (rc)
				rc_free = rc->ಮುಂದಿನ;
			ಇಲ್ಲದಿದ್ದರೆ {
				rc = cmd_hist.ಮುಂದಿನ;
				cmd_hist.ಮುಂದಿನ = rc->ಮುಂದಿನ;
				cmd_hist.ಮುಂದಿನ->prev = &cmd_hist;
				ಬಿಡುಗಡೆ(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_ಬಾಲ;
			rc->ಮುಂದಿನ = cmd_hist.prev;
			rc->prev->ಮುಂದಿನ = rc;
			cmd_ಬಾಲ = rc;
		}
		ಆದರೆ (command_buf.buf[0] == '#')
			ಮುಂದುವರಿ;
		ಹಿಂತಿರುಗಿ 0;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ skip_optional_lf(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ ಪದ_char = fgetc(stdin);
	ಆದರೆ (ಪದ_char != '\n' && ಪದ_char != EOF)
		ungetc(ಪದ_char, stdin);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_mark(ರಚನೆ fast_import_state *state)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v;
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "mark :", &v)) {
		ಮುಂದಿನ_mark = strtoumax(v, ಖಾಲಿ, 10);
		read_ಮುಂದಿನ_command(state);
	}
	ಇಲ್ಲದಿದ್ದರೆ
		ಮುಂದಿನ_mark = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_original_identifier(ರಚನೆ fast_import_state *state)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v;
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "original-oid ", &v))
		read_ಮುಂದಿನ_command(state);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_ದತ್ತಾಂಶ(ರಚನೆ strbuf *sb, uintmax_t ಎಲ್ಲೆ, uintmax_t *len_res)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ದತ್ತಾಂಶ;
	strbuf_reset(sb);

	ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "data ", &ದತ್ತಾಂಶ))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ದತ್ತಾಂಶ, "<<", &ದತ್ತಾಂಶ)) {
		ಅಕ್ಷರ *ಪದ = xstrdup(ದತ್ತಾಂಶ);
		ಗಾತ್ರ_t ಪದ_len = command_buf.len - (ದತ್ತಾಂಶ - command_buf.buf);

		ಗಾಗಿ (;;) {
			ಆದರೆ (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), ಪದ);
			ಆದರೆ (ಪದ_len == command_buf.len
				&& !strcmp(ಪದ, command_buf.buf))
				ನಿಲ್ಲಿಸು;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		ಬಿಡುಗಡೆ(ಪದ);
	}
	ಇಲ್ಲದಿದ್ದರೆ {
		uintmax_t len = strtoumax(ದತ್ತಾಂಶ, ಖಾಲಿ, 10);
		ಗಾತ್ರ_t n = 0, ಉದ್ದಳತೆ = (ಗಾತ್ರ_t)len;

		ಆದರೆ (ಎಲ್ಲೆ && ಎಲ್ಲೆ < len) {
			*len_res = len;
			ಹಿಂತಿರುಗಿ 0;
		}
		ಆದರೆ (ಉದ್ದಳತೆ < len)
			die(_("data is too large to use in this context"));

		ಆಗ (n < ಉದ್ದಳತೆ) {
			ಗಾತ್ರ_t s = strbuf_fread(sb, ಉದ್ದಳತೆ - n, stdin);
			ಆದರೆ (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(ಉದ್ದಳತೆ - n));
			n += s;
		}
	}

	skip_optional_lf();
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ validate_raw_date(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src, ರಚನೆ strbuf *ಫಲಿತಾಂಶ, ಪೂರ್ಣಾಂಕ strict)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *orig_src = src;
	ಅಕ್ಷರ *endp;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಅಂಕೆ;

	errno = 0;

	ಅಂಕೆ = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	ಆದರೆ (errno || endp == src || *endp != ' ')
		ಹಿಂತಿರುಗಿ -1;

	src = endp + 1;
	ಆದರೆ (*src != '-' && *src != '+')
		ಹಿಂತಿರುಗಿ -1;

	ಅಂಕೆ = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	ಆದರೆ (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < ಅಂಕೆ))             /* parsed a broken timezone */
	   )
		ಹಿಂತಿರುಗಿ -1;

	strbuf_addstr(ಫಲಿತಾಂಶ, orig_src);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಅಕ್ಷರ *parse_ident(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ltgt;
	ಗಾತ್ರ_t name_len;
	ರಚನೆ strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	ಆದರೆ (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	ಆದರೆ (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	ಆದರೆ (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	ಆದರೆ (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	ಆದರೆ (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_ಕೂಡಿಸು(&ident, buf, name_len);

	ಆಯ್ಕೆ (whenspec) {
	ಸ್ಥಿತಿ WHENSPEC_RAW:
		ಆದರೆ (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ WHENSPEC_RAW_PERMISSIVE:
		ಆದರೆ (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ WHENSPEC_RFC2822:
		ಆದರೆ (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ WHENSPEC_NOW:
		ಆದರೆ (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ strbuf_detach(&ident, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_and_store_blob(
	ರಚನೆ ಕೊನೆಯ_object *ಕೊನೆಯ,
	ರಚನೆ object_id *oidout,
	uintmax_t mark)
{
	ಸ್ಥಿರ ರಚನೆ strbuf buf = STRBUF_INIT;
	uintmax_t len;

	ಆದರೆ (parse_ದತ್ತಾಂಶ(&buf, repo_settings_get_big_file_ಮಿತಿ(the_repository), &len))
		store_object(OBJ_BLOB, &buf, ಕೊನೆಯ, oidout, mark);
	ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ಕೊನೆಯ) {
			strbuf_release(&ಕೊನೆಯ->ದತ್ತಾಂಶ);
			ಕೊನೆಯ->ಸ್ಥಾನಾಂತರ = 0;
			ಕೊನೆಯ->ಆಳ = 0;
		}
		ಹರಿವು_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_new_blob(ರಚನೆ fast_import_state *state)
{
	read_ಮುಂದಿನ_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&ಕೊನೆಯ_blob, ಖಾಲಿ, ಮುಂದಿನ_mark);
}

ಸ್ಥಿರ ಶೂನ್ಯ unload_one_branch(ಶೂನ್ಯ)
{
	ಆಗ (cur_ಸಕ್ರಿಯ_branches
		&& cur_ಸಕ್ರಿಯ_branches >= max_ಸಕ್ರಿಯ_branches) {
		uintmax_t min_commit = ULONG_MAX;
		ರಚನೆ branch *e, *l = ಖಾಲಿ, *p = ಖಾಲಿ;

		ಗಾಗಿ (e = ಸಕ್ರಿಯ_branches; e; e = e->ಸಕ್ರಿಯ_ಮುಂದಿನ_branch) {
			ಆದರೆ (e->ಕೊನೆಯ_commit < min_commit) {
				p = l;
				min_commit = e->ಕೊನೆಯ_commit;
			}
			l = e;
		}

		ಆದರೆ (p) {
			e = p->ಸಕ್ರಿಯ_ಮುಂದಿನ_branch;
			p->ಸಕ್ರಿಯ_ಮುಂದಿನ_branch = e->ಸಕ್ರಿಯ_ಮುಂದಿನ_branch;
		} ಇಲ್ಲದಿದ್ದರೆ {
			e = ಸಕ್ರಿಯ_branches;
			ಸಕ್ರಿಯ_branches = e->ಸಕ್ರಿಯ_ಮುಂದಿನ_branch;
		}
		e->ಸಕ್ರಿಯ = 0;
		e->ಸಕ್ರಿಯ_ಮುಂದಿನ_branch = ಖಾಲಿ;
		ಆದರೆ (e->branch_ವೃಕ್ಷ.ವೃಕ್ಷ) {
			release_ವೃಕ್ಷ_content_recursive(e->branch_ವೃಕ್ಷ.ವೃಕ್ಷ);
			e->branch_ವೃಕ್ಷ.ವೃಕ್ಷ = ಖಾಲಿ;
		}
		cur_ಸಕ್ರಿಯ_branches--;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ತುಂಬು_branch(ರಚನೆ branch *b)
{
	ತುಂಬು_ವೃಕ್ಷ(&b->branch_ವೃಕ್ಷ);
	ಆದರೆ (!b->ಸಕ್ರಿಯ) {
		b->ಸಕ್ರಿಯ = 1;
		b->ಸಕ್ರಿಯ_ಮುಂದಿನ_branch = ಸಕ್ರಿಯ_branches;
		ಸಕ್ರಿಯ_branches = b;
		cur_ಸಕ್ರಿಯ_branches++;
		branch_ತುಂಬು_ಎಣಿಕೆ++;
	}
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ convert_ಅಂಕೆ_notes_ಗಮ್ಯ_fanout(uintmax_t ಅಂಕೆ_notes)
{
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ fanout = 0;
	ಆಗ ((ಅಂಕೆ_notes >>= 8))
		fanout++;
	ಹಿಂತಿರುಗಿ fanout;
}

ಸ್ಥಿರ ಶೂನ್ಯ construct_ಮಾರ್ಗ_with_fanout(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hex_sha1,
		ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ fanout, ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0, j = 0;
	ಆದರೆ (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	ಆಗ (fanout) {
		ಮಾರ್ಗ[i++] = hex_sha1[j++];
		ಮಾರ್ಗ[i++] = hex_sha1[j++];
		ಮಾರ್ಗ[i++] = '/';
		fanout--;
	}
	ಸ್ಮರಣೆ_ನಕಲು(ಮಾರ್ಗ + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	ಮಾರ್ಗ[i + the_hash_algo->hexsz - j] = '\0';
}

ಸ್ಥಿರ uintmax_t do_change_note_fanout(
		ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *orig_ಬೇರು, ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *ಬೇರು,
		ಅಕ್ಷರ *hex_oid, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hex_oid_len,
		ಅಕ್ಷರ *fullpath, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ fullpath_len,
		ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ fanout)
{
	ರಚನೆ ವೃಕ್ಷ_content *t;
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *e, ಎಲೆ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t ಅಂಕೆ_notes = 0;
	ರಚನೆ object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	ಅಕ್ಷರ realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ hexsz = the_hash_algo->hexsz;

	ಆದರೆ (!ಬೇರು->ವೃಕ್ಷ)
		ತುಂಬು_ವೃಕ್ಷ(ಬೇರು);
	t = ಬೇರು->ವೃಕ್ಷ;

	ಗಾಗಿ (i = 0; t && i < t->ನಮೂದು_ಎಣಿಕೆ; i++) {
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
		ಆದರೆ (!e->versions[1].ಬಹುಲಕ ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			ಮುಂದುವರಿ;

		/* This _may_ be a note entry, or a subdir containing notes */
		ಸ್ಮರಣೆ_ನಕಲು(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		ಆದರೆ (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		ಸ್ಮರಣೆ_ನಕಲು(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		ಆದರೆ (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			ಆದರೆ (fanout == 0xff) {
				/* Counting mode, no rename */
				ಅಂಕೆ_notes++;
				ಮುಂದುವರಿ;
			}
			construct_ಮಾರ್ಗ_with_fanout(hex_oid, fanout, realpath);
			ಆದರೆ (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				ಅಂಕೆ_notes++;
				ಮುಂದುವರಿ;
			}

			/* Rename fullpath to realpath */
			ಆದರೆ (!ವೃಕ್ಷ_content_remove(orig_ಬೇರು, fullpath, &ಎಲೆ, 0))
				die(_("failed to remove path %s"), fullpath);
			ವೃಕ್ಷ_content_ಗಣ(orig_ಬೇರು, realpath,
				&ಎಲೆ.versions[1].oid,
				ಎಲೆ.versions[1].ಬಹುಲಕ,
				ಎಲೆ.ವೃಕ್ಷ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISDIR(e->versions[1].ಬಹುಲಕ)) {
			/* This is a subdir that may contain note entries */
			ಅಂಕೆ_notes += do_change_note_fanout(orig_ಬೇರು, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = ಬೇರು->ವೃಕ್ಷ;
	}
	ಹಿಂತಿರುಗಿ ಅಂಕೆ_notes;
}

ಸ್ಥಿರ uintmax_t change_note_fanout(ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *ಬೇರು,
		ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	ಅಕ್ಷರ hex_oid[GIT_MAX_HEXSZ], ಮಾರ್ಗ[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	ಹಿಂತಿರುಗಿ do_change_note_fanout(ಬೇರು, ಬೇರು, hex_oid, 0, ಮಾರ್ಗ, 0, fanout);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_mapped_oid_hex(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hex, ರಚನೆ object_id *oid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಕೊನೆ)
{
	ಪೂರ್ಣಾಂಕ algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, ಗಾತ್ರ(oid->hash));

	algo = parse_oid_hex_any(hex, oid, ಕೊನೆ);
	ಆದರೆ (algo == GIT_HASH_UNKNOWN)
		ಹಿಂತಿರುಗಿ -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	ಆದರೆ (it == kh_ಕೊನೆ(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		ಆದರೆ (hash_algos[algo].format_id == the_hash_algo->format_id)
			ಹಿಂತಿರುಗಿ 0;
		ಹಿಂತಿರುಗಿ -1;
	}
	oidcpy(oid, kh_ಮೌಲ್ಯ(sub_oid_map, it));
	ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ uintmax_t parse_mark_ref(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p, ಅಕ್ಷರ **endptr)
{
	uintmax_t mark;

	ಪ್ರತಿಜ್ಞಾ(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	ಆದರೆ (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	ಹಿಂತಿರುಗಿ mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
ಸ್ಥಿರ uintmax_t parse_mark_ref_eol(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p)
{
	ಅಕ್ಷರ *ಕೊನೆ;
	uintmax_t mark;

	mark = parse_mark_ref(p, &ಕೊನೆ);
	ಆದರೆ (*ಕೊನೆ != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	ಹಿಂತಿರುಗಿ mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
ಸ್ಥಿರ uintmax_t parse_mark_ref_space(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **p)
{
	uintmax_t mark;
	ಅಕ್ಷರ *ಕೊನೆ;

	mark = parse_mark_ref(*p, &ಕೊನೆ);
	ಆದರೆ (*ಕೊನೆ++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = ಕೊನೆ;
	ಹಿಂತಿರುಗಿ mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
ಸ್ಥಿರ ಶೂನ್ಯ parse_ಮಾರ್ಗ(ರಚನೆ strbuf *sb, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **endp,
		ಪೂರ್ಣಾಂಕ is_ಕೊನೆಯ_ಕ್ಷೇತ್ರ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ)
{
	ಆದರೆ (*p == '"') {
		ಆದರೆ (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), ಕ್ಷೇತ್ರ, command_buf.buf);
		ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), ಕ್ಷೇತ್ರ, command_buf.buf);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * Unless we are parsing the last field of a line,
		 * SP is the end of this field.
		 */
		*endp = is_ಕೊನೆಯ_ಕ್ಷೇತ್ರ
			? p + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(p)
			: strchrnul(p, ' ');
		strbuf_ಕೂಡಿಸು(sb, p, *endp - p);
	}
}

/*
 * Parse the path string into the strbuf, and complain if this is not the end of
 * the string. Unquoted strings may contain spaces.
 */
ಸ್ಥಿರ ಶೂನ್ಯ parse_ಮಾರ್ಗ_eol(ರಚನೆ strbuf *sb, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ;

	parse_ಮಾರ್ಗ(sb, p, &ಕೊನೆ, 1, ಕ್ಷೇತ್ರ);
	ಆದರೆ (*ಕೊನೆ)
		die(_("garbage after %s: %s"), ಕ್ಷೇತ್ರ, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
ಸ್ಥಿರ ಶೂನ್ಯ parse_ಮಾರ್ಗ_space(ರಚನೆ strbuf *sb, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **endp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ)
{
	parse_ಮಾರ್ಗ(sb, p, endp, 0, ಕ್ಷೇತ್ರ);
	ಆದರೆ (**endp != ' ')
		die(_("missing space after %s: %s"), ಕ್ಷೇತ್ರ, command_buf.buf);
	(*endp)++;
}

ಸ್ಥಿರ ಶೂನ್ಯ file_change_m(ರಚನೆ fast_import_state *state,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p,
			  ರಚನೆ branch *b)
{
	ಸ್ಥಿರ ರಚನೆ strbuf ಮಾರ್ಗ = STRBUF_INIT;
	ರಚನೆ object_ನಮೂದು *oe;
	ರಚನೆ object_id oid;
	uint16_t ಬಹುಲಕ, inline_ದತ್ತಾಂಶ = 0;

	p = parse_ಬಹುಲಕ(p, &ಬಹುಲಕ);
	ಆದರೆ (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	ಆಯ್ಕೆ (ಬಹುಲಕ) {
	ಸ್ಥಿತಿ 0644:
	ಸ್ಥಿತಿ 0755:
		ಬಹುಲಕ |= S_IFREG;
	ಸ್ಥಿತಿ S_IFREG | 0644:
	ಸ್ಥಿತಿ S_IFREG | 0755:
	ಸ್ಥಿತಿ S_IFLNK:
	ಸ್ಥಿತಿ S_IFDIR:
	ಸ್ಥಿತಿ S_IFGITLINK:
		/* ok */
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	ಆದರೆ (*p == ':') {
		oe = ಕಂಡುಹಿಡಿ_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(p, "inline ", &p)) {
		inline_ದತ್ತಾಂಶ = 1;
		oe = ಖಾಲಿ; /* not used with inline_data, but makes gcc happy */
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = ಕಂಡುಹಿಡಿ_object(&oid);
		ಆದರೆ (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&ಮಾರ್ಗ);
	parse_ಮಾರ್ಗ_eol(&ಮಾರ್ಗ, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	ಆದರೆ (S_ISDIR(ಬಹುಲಕ) &&
	    is_ಖಾಲಿಗಳು_ವೃಕ್ಷ_oid(&oid, the_repository->hash_algo) &&
	    *ಮಾರ್ಗ.buf) {
		ವೃಕ್ಷ_content_remove(&b->branch_ವೃಕ್ಷ, ಮಾರ್ಗ.buf, ಖಾಲಿ, 0);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (S_ISGITLINK(ಬಹುಲಕ)) {
		ಆದರೆ (inline_ದತ್ತಾಂಶ)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (oe) {
			ಆದರೆ (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (inline_ದತ್ತಾಂಶ) {
		ಆದರೆ (S_ISDIR(ಬಹುಲಕ))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		ಆಗ (read_ಮುಂದಿನ_command(state) != EOF) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v;
			ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			ಇಲ್ಲದಿದ್ದರೆ {
				parse_and_store_blob(&ಕೊನೆಯ_blob, &oid, 0);
				ನಿಲ್ಲಿಸು;
			}
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಎಣಿಕೆ object_type expected = S_ISDIR(ಬಹುಲಕ) ?
						OBJ_ವೃಕ್ಷ: OBJ_BLOB;
		ಎಣಿಕೆ object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, ಖಾಲಿ);
		ಆದರೆ (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(ಬಹುಲಕ) ?  _("tree") : _("blob"),
			    command_buf.buf);
		ಆದರೆ (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	ಆದರೆ (!*ಮಾರ್ಗ.buf) {
		ವೃಕ್ಷ_content_replace(&b->branch_ವೃಕ್ಷ, &oid, ಬಹುಲಕ, ಖಾಲಿ);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (!verify_ಮಾರ್ಗ(ಮಾರ್ಗ.buf, ಬಹುಲಕ))
		die(_("invalid path '%s'"), ಮಾರ್ಗ.buf);
	ವೃಕ್ಷ_content_ಗಣ(&b->branch_ವೃಕ್ಷ, ಮಾರ್ಗ.buf, &oid, ಬಹುಲಕ, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ file_change_d(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p, ರಚನೆ branch *b)
{
	ಸ್ಥಿರ ರಚನೆ strbuf ಮಾರ್ಗ = STRBUF_INIT;

	strbuf_reset(&ಮಾರ್ಗ);
	parse_ಮಾರ್ಗ_eol(&ಮಾರ್ಗ, p, "path");
	ವೃಕ್ಷ_content_remove(&b->branch_ವೃಕ್ಷ, ಮಾರ್ಗ.buf, ಖಾಲಿ, 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ file_change_cr(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p, ರಚನೆ branch *b, ಪೂರ್ಣಾಂಕ rename)
{
	ಸ್ಥಿರ ರಚನೆ strbuf ಮೂಲ = STRBUF_INIT;
	ಸ್ಥಿರ ರಚನೆ strbuf dest = STRBUF_INIT;
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು ಎಲೆ;

	strbuf_reset(&ಮೂಲ);
	parse_ಮಾರ್ಗ_space(&ಮೂಲ, p, &p, "source");
	strbuf_reset(&dest);
	parse_ಮಾರ್ಗ_eol(&dest, p, "dest");

	memset(&ಎಲೆ, 0, ಗಾತ್ರ(ಎಲೆ));
	ಆದರೆ (rename)
		ವೃಕ್ಷ_content_remove(&b->branch_ವೃಕ್ಷ, ಮೂಲ.buf, &ಎಲೆ, 1);
	ಇಲ್ಲದಿದ್ದರೆ
		ವೃಕ್ಷ_content_get(&b->branch_ವೃಕ್ಷ, ಮೂಲ.buf, &ಎಲೆ, 1);
	ಆದರೆ (!ಎಲೆ.versions[1].ಬಹುಲಕ)
		die(_("path %s not in branch"), ಮೂಲ.buf);
	ಆದರೆ (!*dest.buf) {	/* C "path/to/subdir" "" */
		ವೃಕ್ಷ_content_replace(&b->branch_ವೃಕ್ಷ,
			&ಎಲೆ.versions[1].oid,
			ಎಲೆ.versions[1].ಬಹುಲಕ,
			ಎಲೆ.ವೃಕ್ಷ);
		ಹಿಂತಿರುಗಿ;
	}
	ಆದರೆ (!verify_ಮಾರ್ಗ(dest.buf, ಎಲೆ.versions[1].ಬಹುಲಕ))
		die(_("invalid path '%s'"), dest.buf);
	ವೃಕ್ಷ_content_ಗಣ(&b->branch_ವೃಕ್ಷ, dest.buf,
		&ಎಲೆ.versions[1].oid,
		ಎಲೆ.versions[1].ಬಹುಲಕ,
		ಎಲೆ.ವೃಕ್ಷ);
}

ಸ್ಥಿರ ಶೂನ್ಯ note_change_n(ರಚನೆ fast_import_state *state,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p,
			  ರಚನೆ branch *b,
			  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *old_fanout)
{
	ರಚನೆ object_ನಮೂದು *oe;
	ರಚನೆ branch *s;
	ರಚನೆ object_id oid, commit_oid;
	ಅಕ್ಷರ ಮಾರ್ಗ[GIT_MAX_RAWSZ * 3];
	uint16_t inline_ದತ್ತಾಂಶ = 0;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ new_fanout;

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
	ಆದರೆ (b->ಅಂಕೆ_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->ಅಂಕೆ_notes = change_note_fanout(&b->branch_ವೃಕ್ಷ, 0xff);
		*old_fanout = convert_ಅಂಕೆ_notes_ಗಮ್ಯ_fanout(b->ಅಂಕೆ_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	ಆದರೆ (*p == ':') {
		oe = ಕಂಡುಹಿಡಿ_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(p, "inline ", &p)) {
		inline_ದತ್ತಾಂಶ = 1;
		oe = ಖಾಲಿ; /* not used with inline_data, but makes gcc happy */
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = ಕಂಡುಹಿಡಿ_object(&oid);
		ಆದರೆ (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	ಆದರೆ (s) {
		ಆದರೆ (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		ರಚನೆ object_ನಮೂದು *commit_oe = ಕಂಡುಹಿಡಿ_mark(marks, commit_mark);
		ಆದರೆ (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!repo_get_oid(the_repository, p, &commit_oid)) {
		ಗಾತ್ರ_t ಗಾತ್ರ_2;
		ಅಕ್ಷರ *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &ಗಾತ್ರ_2,
						   &commit_oid);
		ಆದರೆ (!buf || ಗಾತ್ರ_2 < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		ಬಿಡುಗಡೆ(buf);
	} ಇಲ್ಲದಿದ್ದರೆ
		die(_("invalid ref name or SHA1 expression: %s"), p);

	ಆದರೆ (inline_ದತ್ತಾಂಶ) {
		read_ಮುಂದಿನ_command(state);
		parse_and_store_blob(&ಕೊನೆಯ_blob, &oid, 0);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (oe) {
		ಆದರೆ (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!is_null_oid(&oid)) {
		ಎಣಿಕೆ object_type type = odb_read_object_info(the_repository->objects, &oid,
							ಖಾಲಿ);
		ಆದರೆ (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		ಆದರೆ (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_ಮಾರ್ಗ_with_fanout(oid_ಗಮ್ಯ_hex(&commit_oid), *old_fanout, ಮಾರ್ಗ);
	ಆದರೆ (ವೃಕ್ಷ_content_remove(&b->branch_ವೃಕ್ಷ, ಮಾರ್ಗ, ಖಾಲಿ, 0))
		b->ಅಂಕೆ_notes--;

	ಆದರೆ (is_null_oid(&oid))
		ಹಿಂತಿರುಗಿ; /* nothing to insert */

	b->ಅಂಕೆ_notes++;
	new_fanout = convert_ಅಂಕೆ_notes_ಗಮ್ಯ_fanout(b->ಅಂಕೆ_notes);
	construct_ಮಾರ್ಗ_with_fanout(oid_ಗಮ್ಯ_hex(&commit_oid), new_fanout, ಮಾರ್ಗ);
	ವೃಕ್ಷ_content_ಗಣ(&b->branch_ವೃಕ್ಷ, ಮಾರ್ಗ, &oid, S_IFREG | 0644, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ file_change_deleteall(ರಚನೆ branch *b)
{
	release_ವೃಕ್ಷ_content_recursive(b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ);
	oidclr(&b->branch_ವೃಕ್ಷ.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_ವೃಕ್ಷ.versions[1].oid, the_repository->hash_algo);
	ತುಂಬು_ವೃಕ್ಷ(&b->branch_ವೃಕ್ಷ);
	b->ಅಂಕೆ_notes = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_from_commit(ರಚನೆ branch *b, ಅಕ್ಷರ *buf, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2)
{
	ಆದರೆ (!buf || ಗಾತ್ರ_2 < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_ಗಮ್ಯ_hex(&b->oid));
	ಆದರೆ (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_ವೃಕ್ಷ.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_ಗಮ್ಯ_hex(&b->oid));
	oidcpy(&b->branch_ವೃಕ್ಷ.versions[0].oid,
	       &b->branch_ವೃಕ್ಷ.versions[1].oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_from_existing(ರಚನೆ branch *b)
{
	ಆದರೆ (is_null_oid(&b->oid)) {
		oidclr(&b->branch_ವೃಕ್ಷ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_ವೃಕ್ಷ.versions[1].oid, the_repository->hash_algo);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;
		ಗಾತ್ರ_t ಗಾತ್ರ_st = 0;
		ಅಕ್ಷರ *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &ಗಾತ್ರ_st, &b->oid);
		ಗಾತ್ರ_2 = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(ಗಾತ್ರ_st);
		parse_from_commit(b, buf, ಗಾತ್ರ_2);
		ಬಿಡುಗಡೆ(buf);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_objectish(ರಚನೆ fast_import_state *state,
			   ರಚನೆ branch *b,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *objectish)
{
	ರಚನೆ branch *s;
	ರಚನೆ object_id oid;

	oidcpy(&oid, &b->branch_ವೃಕ್ಷ.versions[1].oid);

	s = lookup_branch(objectish);
	ಆದರೆ (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s) {
		ರಚನೆ object_id *t = &s->branch_ವೃಕ್ಷ.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_ವೃಕ್ಷ.versions[0].oid, t);
		oidcpy(&b->branch_ವೃಕ್ಷ.versions[1].oid, t);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		ರಚನೆ object_ನಮೂದು *oe = ಕಂಡುಹಿಡಿ_mark(marks, idnum);
		ಆದರೆ (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		ಆದರೆ (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			ಆದರೆ (oe->pack_id != MAX_PACK_ID) {
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;
				ಅಕ್ಷರ *buf = gfi_unpack_ನಮೂದು(oe, &ಗಾತ್ರ_2);
				parse_from_commit(b, buf, ಗಾತ್ರ_2);
				ಬಿಡುಗಡೆ(buf);
			} ಇಲ್ಲದಿದ್ದರೆ
				parse_from_existing(b);
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		ಆದರೆ (is_null_oid(&b->oid))
			b->delete = 1;
	}
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	ಆದರೆ (b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ && !oideq(&oid, &b->branch_ವೃಕ್ಷ.versions[1].oid)) {
		release_ವೃಕ್ಷ_content_recursive(b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ);
		b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ = ಖಾಲಿ;
	}

	read_ಮುಂದಿನ_command(state);
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_from(ರಚನೆ fast_import_state *state, ರಚನೆ branch *b)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *from;

	ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "from ", &from))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ parse_objectish(state, b, from);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_objectish_with_ಪೂರ್ವಪ್ರತ್ಯಯ(ರಚನೆ fast_import_state *state,
				       ರಚನೆ branch *b,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ;

	ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, ಪೂರ್ವಪ್ರತ್ಯಯ, &ತಳ))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ parse_objectish(state, b, ತಳ);
}

ಸ್ಥಿರ ರಚನೆ hash_list *parse_ವಿಲೀನ(ರಚನೆ fast_import_state *state,
				     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *ಎಣಿಕೆ_2)
{
	ರಚನೆ hash_list *list = ಖಾಲಿ, **ಬಾಲ = &list, *n;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *from;
	ರಚನೆ branch *s;

	*ಎಣಿಕೆ_2 = 0;
	ಆಗ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "merge ", &from)) {
		n = xmalloc(ಗಾತ್ರ(*n));
		s = lookup_branch(from);
		ಆದರೆ (s)
			oidcpy(&n->oid, &s->oid);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			ರಚನೆ object_ನಮೂದು *oe = ಕಂಡುಹಿಡಿ_mark(marks, idnum);
			ಆದರೆ (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!repo_get_oid(the_repository, from, &n->oid)) {
			ಗಾತ್ರ_t ಗಾತ್ರ_2;
			ಅಕ್ಷರ *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &ಗಾತ್ರ_2, &n->oid);
			ಆದರೆ (!buf || ಗಾತ್ರ_2 < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			ಬಿಡುಗಡೆ(buf);
		} ಇಲ್ಲದಿದ್ದರೆ
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->ಮುಂದಿನ = ಖಾಲಿ;
		*ಬಾಲ = n;
		ಬಾಲ = &n->ಮುಂದಿನ;

		(*ಎಣಿಕೆ_2)++;
		read_ಮುಂದಿನ_command(state);
	}
	ಹಿಂತಿರುಗಿ list;
}

ರಚನೆ signature_ದತ್ತಾಂಶ {
	ಅಕ್ಷರ *hash_algo;      /* "sha1" or "sha256" */
	ಅಕ್ಷರ *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	ರಚನೆ strbuf ದತ್ತಾಂಶ;   /* The actual signature data */
};

ಸ್ಥಿರ ಶೂನ್ಯ parse_one_signature(ರಚನೆ fast_import_state *state,
				ರಚನೆ signature_ದತ್ತಾಂಶ *sig,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v)
{
	ಅಕ್ಷರ *ವಾದಗಳು = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	ಅಕ್ಷರ *space = strchr(ವಾದಗಳು, ' ');

	ಆದರೆ (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), ವಾದಗಳು);
	*space = '\0';

	sig->hash_algo = ವಾದಗಳು;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	ಆದರೆ (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	ಆದರೆ (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	ಆದರೆ (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_ಮುಂದಿನ_command(state);
	parse_ದತ್ತಾಂಶ(&sig->ದತ್ತಾಂಶ, 0, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ discard_one_signature(ರಚನೆ fast_import_state *state)
{
	ರಚನೆ strbuf ದತ್ತಾಂಶ = STRBUF_INIT;

	read_ಮುಂದಿನ_command(state);
	parse_ದತ್ತಾಂಶ(&ದತ್ತಾಂಶ, 0, ಖಾಲಿ);
	strbuf_release(&ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_gpgsig_ಗಮ್ಯ_commit(ರಚನೆ strbuf *commit_ದತ್ತಾಂಶ,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಶೀರ್ಷಿಕೆ,
				 ರಚನೆ signature_ದತ್ತಾಂಶ *sig)
{
	ರಚನೆ string_list siglines = STRING_LIST_INIT_NODUP;

	ಆದರೆ (!sig || !sig->hash_algo)
		ಹಿಂತಿರುಗಿ;

	strbuf_addstr(commit_ದತ್ತಾಂಶ, ಶೀರ್ಷಿಕೆ);
	string_list_ವಿಭಜಿಸು_ಒಳ_ಇರಿಸು(&siglines, sig->ದತ್ತಾಂಶ.buf, "\n", -1);
	strbuf_ಕೂಡಿಸು_separated_string_list(commit_ದತ್ತಾಂಶ, "\n ", &siglines);
	strbuf_addch(commit_ದತ್ತಾಂಶ, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->ದತ್ತಾಂಶ);
	ಬಿಡುಗಡೆ(sig->hash_algo);
}

ಸ್ಥಿರ ಶೂನ್ಯ store_signature(ರಚನೆ signature_ದತ್ತಾಂಶ *stored_sig,
			    ರಚನೆ signature_ದತ್ತಾಂಶ *new_sig,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hash_type)
{
	ಆದರೆ (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->ದತ್ತಾಂಶ);
		ಬಿಡುಗಡೆ(new_sig->hash_algo);
	} ಇಲ್ಲದಿದ್ದರೆ {
		*stored_sig = *new_sig;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ import_one_signature(ರಚನೆ fast_import_state *state,
				 ರಚನೆ signature_ದತ್ತಾಂಶ *sig_sha1,
				 ರಚನೆ signature_ದತ್ತಾಂಶ *sig_sha256,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v)
{
	ರಚನೆ signature_ದತ್ತಾಂಶ sig = { ಖಾಲಿ, ಖಾಲಿ, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	ಆದರೆ (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("parse_one_signature() returned unknown hash algo"));
}

ಸ್ಥಿರ ಶೂನ್ಯ finalize_commit_ಬಫರ್(ರಚನೆ strbuf *new_ದತ್ತಾಂಶ,
				   ರಚನೆ signature_ದತ್ತಾಂಶ *sig_sha1,
				   ರಚನೆ signature_ದತ್ತಾಂಶ *sig_sha256,
				   ರಚನೆ strbuf *ಸಂದೇಶ)
{
	ಕೂಡಿಸು_gpgsig_ಗಮ್ಯ_commit(new_ದತ್ತಾಂಶ, "gpgsig ", sig_sha1);
	ಕೂಡಿಸು_gpgsig_ಗಮ್ಯ_commit(new_ದತ್ತಾಂಶ, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_ದತ್ತಾಂಶ, '\n');
	strbuf_addbuf(new_ದತ್ತಾಂಶ, ಸಂದೇಶ);
}

ಸ್ಥಿರ ಶೂನ್ಯ warn_invalid_signature(ರಚನೆ signature_check *check,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಎಣಿಕೆ sign_ಬಹುಲಕ ಬಹುಲಕ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *signer = check->signer ? check->signer : _("unknown");
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subject;
	ಪೂರ್ಣಾಂಕ subject_len = ಕಂಡುಹಿಡಿ_commit_subject(ಸಂದೇಶ, &subject);

	ಆಯ್ಕೆ (ಬಹುಲಕ) {
	ಸ್ಥಿತಿ SIGN_STRIP_IF_INVALID:
		ಆದರೆ (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		ಇಲ್ಲದಿದ್ದರೆ
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SIGN_SIGN_IF_INVALID:
		ಆದರೆ (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		ಇಲ್ಲದಿದ್ದರೆ
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unsupported signing mode");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_signature_if_invalid(ರಚನೆ strbuf *new_ದತ್ತಾಂಶ,
					ರಚನೆ signature_ದತ್ತಾಂಶ *sig_sha1,
					ರಚನೆ signature_ದತ್ತಾಂಶ *sig_sha256,
					ರಚನೆ strbuf *ಸಂದೇಶ,
					ಎಣಿಕೆ sign_ಬಹುಲಕ ಬಹುಲಕ)
{
	ರಚನೆ strbuf tmp_buf = STRBUF_INIT;
	ರಚನೆ signature_check signature_check = { 0 };
	ಪೂರ್ಣಾಂಕ ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_ದತ್ತಾಂಶ);
	finalize_commit_ಬಫರ್(&tmp_buf, sig_sha1, sig_sha256, ಸಂದೇಶ);
	ret = verify_commit_ಬಫರ್(tmp_buf.buf, tmp_buf.len, &signature_check);

	ಆದರೆ (ret) {
		ಆದರೆ (ಬಹುಲಕ == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, ಸಂದೇಶ->buf, ಬಹುಲಕ);

		ಆದರೆ (ಬಹುಲಕ == SIGN_SIGN_IF_INVALID) {
			ರಚನೆ strbuf signature = STRBUF_INIT;
			ರಚನೆ strbuf payload = STRBUF_INIT;

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
			ಆದರೆ (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			ಆದರೆ (sign_ಬಫರ್(&payload, &signature, signed_commit_keyid,
					SIGN_ಬಫರ್_USE_DEFAULT_ಕೀಲಿ))
				die(_("failed to sign commit object"));
			ಕೂಡಿಸು_ಶೀರ್ಷಿಕೆ_signature(new_ದತ್ತಾಂಶ, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_ಬಫರ್(new_ದತ್ತಾಂಶ, ಖಾಲಿ, ಖಾಲಿ, ಸಂದೇಶ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_ಅದಲುಬದಲು(new_ದತ್ತಾಂಶ, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_new_commit(ರಚನೆ fast_import_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಸ್ಥಿರ ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ signature_ದತ್ತಾಂಶ sig_sha1 = { ಖಾಲಿ, ಖಾಲಿ, STRBUF_INIT };
	ರಚನೆ signature_ದತ್ತಾಂಶ sig_sha256 = { ಖಾಲಿ, ಖಾಲಿ, STRBUF_INIT };
	ರಚನೆ branch *b;
	ಅಕ್ಷರ *author = ಖಾಲಿ;
	ಅಕ್ಷರ *committer = ಖಾಲಿ;
	ಅಕ್ಷರ *encoding = ಖಾಲಿ;
	ರಚನೆ hash_list *ವಿಲೀನ_list = ಖಾಲಿ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ವಿಲೀನ_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ prev_fanout, new_fanout;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v;

	b = lookup_branch(arg);
	ಆದರೆ (!b)
		b = new_branch(arg);

	read_ಮುಂದಿನ_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_ಮುಂದಿನ_command(state);
	}
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_ಮುಂದಿನ_command(state);
	}
	ಆದರೆ (!committer)
		die(_("expected committer but didn't get one"));

	ಆಗ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "gpgsig ", &v)) {
		ಆಯ್ಕೆ (signed_commit_ಬಹುಲಕ) {

		/* First, modes that don't need the signature to be parsed */
		ಸ್ಥಿತಿ SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		ಸ್ಥಿತಿ SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		ಸ್ಥಿತಿ SIGN_STRIP:
			discard_one_signature(state);
			ನಿಲ್ಲಿಸು;

		/* Second, modes that parse the signature */
		ಸ್ಥಿತಿ SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		ಸ್ಥಿತಿ SIGN_VERBATIM:
		ಸ್ಥಿತಿ SIGN_STRIP_IF_INVALID:
		ಸ್ಥಿತಿ SIGN_SIGN_IF_INVALID:
		ಸ್ಥಿತಿ SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			ನಿಲ್ಲಿಸು;

		/* Third, BUG */
		ಪೂರ್ವನಿಯೋಜಿತ:
			BUG("invalid signed_commit_mode value %d", signed_commit_ಬಹುಲಕ);
		}
		read_ಮುಂದಿನ_command(state);
	}

	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_ಮುಂದಿನ_command(state);
	}
	parse_ದತ್ತಾಂಶ(&ಸಂದೇಶ, 0, ಖಾಲಿ);
	read_ಮುಂದಿನ_command(state);
	parse_from(state, b);
	ವಿಲೀನ_list = parse_ವಿಲೀನ(state, &ವಿಲೀನ_ಎಣಿಕೆ);

	/* ensure the branch is active/loaded */
	ಆದರೆ (!b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ || !max_ಸಕ್ರಿಯ_branches) {
		unload_one_branch();
		ತುಂಬು_branch(b);
	}

	prev_fanout = convert_ಅಂಕೆ_notes_ಗಮ್ಯ_fanout(b->ಅಂಕೆ_notes);

	/* file_change* */
	ಆಗ (command_buf.len > 0) {
		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		ಇಲ್ಲದಿದ್ದರೆ {
			unread_command_buf = 1;
			ನಿಲ್ಲಿಸು;
		}
		ಆದರೆ (read_ಮುಂದಿನ_command(state) == EOF)
			ನಿಲ್ಲಿಸು;
	}

	new_fanout = convert_ಅಂಕೆ_notes_ಗಮ್ಯ_fanout(b->ಅಂಕೆ_notes);
	ಆದರೆ (new_fanout != prev_fanout)
		b->ಅಂಕೆ_notes = change_note_fanout(&b->branch_ವೃಕ್ಷ, new_fanout);

	/* build the tree and the commit */
	store_ವೃಕ್ಷ(&b->branch_ವೃಕ್ಷ);
	oidcpy(&b->branch_ವೃಕ್ಷ.versions[0].oid,
	       &b->branch_ವೃಕ್ಷ.versions[1].oid);

	strbuf_reset(&new_ದತ್ತಾಂಶ);
	strbuf_addf(&new_ದತ್ತಾಂಶ, "tree %s\n",
		oid_ಗಮ್ಯ_hex(&b->branch_ವೃಕ್ಷ.versions[1].oid));
	ಆದರೆ (!is_null_oid(&b->oid))
		strbuf_addf(&new_ದತ್ತಾಂಶ, "parent %s\n",
			    oid_ಗಮ್ಯ_hex(&b->oid));
	ಆಗ (ವಿಲೀನ_list) {
		ರಚನೆ hash_list *ಮುಂದಿನ = ವಿಲೀನ_list->ಮುಂದಿನ;
		strbuf_addf(&new_ದತ್ತಾಂಶ, "parent %s\n",
			    oid_ಗಮ್ಯ_hex(&ವಿಲೀನ_list->oid));
		ಬಿಡುಗಡೆ(ವಿಲೀನ_list);
		ವಿಲೀನ_list = ಮುಂದಿನ;
	}
	strbuf_addf(&new_ದತ್ತಾಂಶ,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	ಆದರೆ (encoding)
		strbuf_addf(&new_ದತ್ತಾಂಶ,
			"encoding %s\n",
			encoding);

	ಆದರೆ ((signed_commit_ಬಹುಲಕ == SIGN_STRIP_IF_INVALID ||
	     signed_commit_ಬಹುಲಕ == SIGN_SIGN_IF_INVALID ||
	     signed_commit_ಬಹುಲಕ == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_ದತ್ತಾಂಶ, &sig_sha1, &sig_sha256,
					    &ಸಂದೇಶ, signed_commit_ಬಹುಲಕ);
	ಇಲ್ಲದಿದ್ದರೆ
		finalize_commit_ಬಫರ್(&new_ದತ್ತಾಂಶ, &sig_sha1, &sig_sha256, &ಸಂದೇಶ);

	ಬಿಡುಗಡೆ(author);
	ಬಿಡುಗಡೆ(committer);
	ಬಿಡುಗಡೆ(encoding);

	ಆದರೆ (!store_object(OBJ_COMMIT, &new_ದತ್ತಾಂಶ, ಖಾಲಿ, &b->oid, ಮುಂದಿನ_mark))
		b->pack_id = pack_id;
	b->ಕೊನೆಯ_commit = object_ಎಣಿಕೆ_by_type[OBJ_COMMIT];
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_ಟ್ಯಾಗ್_signature_if_invalid(ರಚನೆ strbuf *buf,
					    ರಚನೆ strbuf *ಸಂದೇಶ,
					    ಗಾತ್ರ_t sig_ಸ್ಥಾನಾಂತರ)
{
	ರಚನೆ strbuf signature = STRBUF_INIT;
	ರಚನೆ strbuf payload = STRBUF_INIT;
	ರಚನೆ signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_ಕೂಡಿಸು(&payload, ಸಂದೇಶ->buf, sig_ಸ್ಥಾನಾಂತರ);
	strbuf_ಕೂಡಿಸು(&signature, ಸಂದೇಶ->buf + sig_ಸ್ಥಾನಾಂತರ, ಸಂದೇಶ->len - sig_ಸ್ಥಾನಾಂತರ);

	sigc.payload_type = SIGNATURE_PAYLOAD_ಟ್ಯಾಗ್;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	ಆದರೆ (!check_signature(&sigc, signature.buf, signature.len))
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (signed_ಟ್ಯಾಗ್_ಬಹುಲಕ == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(ಸಂದೇಶ, sig_ಸ್ಥಾನಾಂತರ);

	ಆದರೆ (signed_ಟ್ಯಾಗ್_ಬಹುಲಕ == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = ಖಾಲಿ;
		strbuf_reset(&signature);

		ಆದರೆ (sign_ಬಫರ್(&payload, &signature, signed_ಟ್ಯಾಗ್_keyid,
				SIGN_ಬಫರ್_USE_DEFAULT_ಕೀಲಿ))
			die(_("failed to sign tag object"));

		strbuf_addbuf(ಸಂದೇಶ, &signature);
	}

ಹೊರಹೋಗು:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_ಟ್ಯಾಗ್_signature(ರಚನೆ strbuf *buf, ರಚನೆ strbuf *ಸಂದೇಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಗಾತ್ರ_t sig_ಸ್ಥಾನಾಂತರ = parse_signed_ಬಫರ್(ಸಂದೇಶ->buf, ಸಂದೇಶ->len);

	/* If there is no signature, there is nothing to do. */
	ಆದರೆ (sig_ಸ್ಥಾನಾಂತರ >= ಸಂದೇಶ->len)
		ಹಿಂತಿರುಗಿ;

	ಆಯ್ಕೆ (signed_ಟ್ಯಾಗ್_ಬಹುಲಕ) {

	/* First, modes that don't change anything */
	ಸ್ಥಿತಿ SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	ಸ್ಥಿತಿ SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		ನಿಲ್ಲಿಸು;

	/* Second, modes that remove the signature */
	ಸ್ಥಿತಿ SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	ಸ್ಥಿತಿ SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(ಸಂದೇಶ, sig_ಸ್ಥಾನಾಂತರ);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SIGN_ABORT_IF_INVALID:
	ಸ್ಥಿತಿ SIGN_SIGN_IF_INVALID:
	ಸ್ಥಿತಿ SIGN_STRIP_IF_INVALID:
		handle_ಟ್ಯಾಗ್_signature_if_invalid(buf, ಸಂದೇಶ, sig_ಸ್ಥಾನಾಂತರ);
		ನಿಲ್ಲಿಸು;

	/* Third, aborting modes */
	ಸ್ಥಿತಿ SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_ಟ್ಯಾಗ್_ಬಹುಲಕ, name);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_new_ಟ್ಯಾಗ್(ರಚನೆ fast_import_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಸ್ಥಿರ ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *from;
	ಅಕ್ಷರ *tagger;
	ರಚನೆ branch *s;
	ರಚನೆ ಟ್ಯಾಗ್ *t;
	uintmax_t from_mark = 0;
	ರಚನೆ object_id oid;
	ಎಣಿಕೆ object_type type;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, ಗಾತ್ರ(ರಚನೆ ಟ್ಯಾಗ್));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	ಆದರೆ (ಕೊನೆಯ_ಟ್ಯಾಗ್)
		ಕೊನೆಯ_ಟ್ಯಾಗ್->ಮುಂದಿನ_ಟ್ಯಾಗ್ = t;
	ಇಲ್ಲದಿದ್ದರೆ
		ಮೊದಲ_ಟ್ಯಾಗ್ = t;
	ಕೊನೆಯ_ಟ್ಯಾಗ್ = t;
	read_ಮುಂದಿನ_command(state);
	parse_mark(state);

	/* from ... */
	ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	ಆದರೆ (s) {
		ಆದರೆ (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*from == ':') {
		ರಚನೆ object_ನಮೂದು *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = ಕಂಡುಹಿಡಿ_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!repo_get_oid(the_repository, from, &oid)) {
		ರಚನೆ object_ನಮೂದು *oe = ಕಂಡುಹಿಡಿ_object(&oid);
		ಆದರೆ (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, ಖಾಲಿ);
			ಆದರೆ (type < 0)
				die(_("not a valid object: %s"), from);
		} ಇಲ್ಲದಿದ್ದರೆ
			type = oe->type;
	} ಇಲ್ಲದಿದ್ದರೆ
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_ಮುಂದಿನ_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_ಮುಂದಿನ_command(state);
	} ಇಲ್ಲದಿದ್ದರೆ
		tagger = ಖಾಲಿ;

	/* tag payload/message */
	parse_ದತ್ತಾಂಶ(&ಸಂದೇಶ, 0, ಖಾಲಿ);

	/* build the tag object */
	strbuf_reset(&new_ದತ್ತಾಂಶ);

	strbuf_addf(&new_ದತ್ತಾಂಶ,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_ಗಮ್ಯ_hex(&oid), type_name(type), t->name);
	ಆದರೆ (tagger)
		strbuf_addf(&new_ದತ್ತಾಂಶ,
			    "tagger %s\n", tagger);

	handle_ಟ್ಯಾಗ್_signature(&new_ದತ್ತಾಂಶ, &ಸಂದೇಶ, t->name);

	strbuf_addch(&new_ದತ್ತಾಂಶ, '\n');
	strbuf_addbuf(&new_ದತ್ತಾಂಶ, &ಸಂದೇಶ);
	ಬಿಡುಗಡೆ(tagger);

	ಆದರೆ (store_object(OBJ_ಟ್ಯಾಗ್, &new_ದತ್ತಾಂಶ, ಖಾಲಿ, &t->oid, ಮುಂದಿನ_mark))
		t->pack_id = MAX_PACK_ID;
	ಇಲ್ಲದಿದ್ದರೆ
		t->pack_id = pack_id;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_reset_branch(ರಚನೆ fast_import_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ರಚನೆ branch *b;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್_name;

	b = lookup_branch(arg);
	ಆದರೆ (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_ವೃಕ್ಷ.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_ವೃಕ್ಷ.versions[1].oid, the_repository->hash_algo);
		ಆದರೆ (b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ) {
			release_ವೃಕ್ಷ_content_recursive(b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ);
			b->branch_ವೃಕ್ಷ.ವೃಕ್ಷ = ಖಾಲಿ;
		}
	}
	ಇಲ್ಲದಿದ್ದರೆ
		b = new_branch(arg);
	read_ಮುಂದಿನ_command(state);
	parse_from(state, b);
	ಆದರೆ (b->delete && skip_ಪೂರ್ವಪ್ರತ್ಯಯ(b->name, "refs/tags/", &ಟ್ಯಾಗ್_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		ರಚನೆ ಟ್ಯಾಗ್ *t, *prev = ಖಾಲಿ;
		ಗಾಗಿ (t = ಮೊದಲ_ಟ್ಯಾಗ್; t; t = t->ಮುಂದಿನ_ಟ್ಯಾಗ್) {
			ಆದರೆ (!strcmp(t->name, ಟ್ಯಾಗ್_name))
				ನಿಲ್ಲಿಸು;
			prev = t;
		}
		ಆದರೆ (t) {
			ಆದರೆ (prev)
				prev->ಮುಂದಿನ_ಟ್ಯಾಗ್ = t->ಮುಂದಿನ_ಟ್ಯಾಗ್;
			ಇಲ್ಲದಿದ್ದರೆ
				ಮೊದಲ_ಟ್ಯಾಗ್ = t->ಮುಂದಿನ_ಟ್ಯಾಗ್;
			ಆದರೆ (!t->ಮುಂದಿನ_ಟ್ಯಾಗ್)
				ಕೊನೆಯ_ಟ್ಯಾಗ್ = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	ಆದರೆ (command_buf.len > 0)
		unread_command_buf = 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ cat_blob_write(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2)
{
	ಆದರೆ (write_ಒಳ_ತುಂಬಿದ(cat_blob_fd, buf, ಗಾತ್ರ_2) < 0)
		die_errno(_("write to frontend failed"));
}

ಸ್ಥಿರ ಶೂನ್ಯ cat_blob(ರಚನೆ object_ನಮೂದು *oe, ರಚನೆ object_id *oid)
{
	ರಚನೆ strbuf ಸಾಲು = STRBUF_INIT;
	ರಚನೆ iovec iov[3];
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;
	ಎಣಿಕೆ object_type type = 0;
	ಅಕ್ಷರ *buf;

	ಆದರೆ (!oe || oe->pack_id == MAX_PACK_ID) {
		ಗಾತ್ರ_t ಗಾತ್ರ_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &ಗಾತ್ರ_st);
		ಗಾತ್ರ_2 = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(ಗಾತ್ರ_st);
	} ಇಲ್ಲದಿದ್ದರೆ {
		type = oe->type;
		buf = gfi_unpack_ನಮೂದು(oe, &ಗಾತ್ರ_2);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	ಆದರೆ (type <= 0) {
		strbuf_reset(&ಸಾಲು);
		strbuf_addf(&ಸಾಲು, "%s missing\n", oid_ಗಮ್ಯ_hex(oid));
		cat_blob_write(ಸಾಲು.buf, ಸಾಲು.len);
		strbuf_release(&ಸಾಲು);
		ಬಿಡುಗಡೆ(buf);
		ಹಿಂತಿರುಗಿ;
	}
	ಆದರೆ (!buf)
		die(_("can't read object %s"), oid_ಗಮ್ಯ_hex(oid));
	ಆದರೆ (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_ಗಮ್ಯ_hex(oid), type_name(type));
	strbuf_reset(&ಸಾಲು);
	strbuf_addf(&ಸಾಲು, "%s %s %"PRIuMAX"\n", oid_ಗಮ್ಯ_hex(oid),
		    type_name(type), (uintmax_t)ಗಾತ್ರ_2);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_ತಳ = ಸಾಲು.buf;
	iov[0].iov_len = ಸಾಲು.len;
	iov[1].iov_ತಳ = buf;
	iov[1].iov_len = ಗಾತ್ರ_2;
	iov[2].iov_ತಳ = (ಶೂನ್ಯ *) "\n";
	iov[2].iov_len = 1;

	ಆದರೆ (writev_ಒಳ_ತುಂಬಿದ(cat_blob_fd, iov, ARRAY_ಗಾತ್ರ(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&ಸಾಲು);
	ಆದರೆ (oe && oe->pack_id == pack_id) {
		ಕೊನೆಯ_blob.ಸ್ಥಾನಾಂತರ = oe->idx.ಸ್ಥಾನಾಂತರ;
		strbuf_attach(&ಕೊನೆಯ_blob.ದತ್ತಾಂಶ, buf, ಗಾತ್ರ_2, ಗಾತ್ರ_2 + 1);
		ಕೊನೆಯ_blob.ಆಳ = oe->ಆಳ;
	} ಇಲ್ಲದಿದ್ದರೆ
		ಬಿಡುಗಡೆ(buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_get_mark(ರಚನೆ fast_import_state *state ಬಳಸದ,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p)
{
	ರಚನೆ object_ನಮೂದು *oe;
	ಅಕ್ಷರ output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	ಆದರೆ (*p != ':')
		die(_("not a mark: %s"), p);

	oe = ಕಂಡುಹಿಡಿ_mark(marks, parse_mark_ref_eol(p));
	ಆದರೆ (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, ಗಾತ್ರ(output), "%s\n", oid_ಗಮ್ಯ_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cat_blob(ರಚನೆ fast_import_state *state ಬಳಸದ,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p)
{
	ರಚನೆ object_ನಮೂದು *oe;
	ರಚನೆ object_id oid;

	/* cat-blob SP <object> LF */
	ಆದರೆ (*p == ':') {
		oe = ಕಂಡುಹಿಡಿ_mark(marks, parse_mark_ref_eol(p));
		ಆದರೆ (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		ಆದರೆ (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = ಕಂಡುಹಿಡಿ_object(&oid);
	}

	cat_blob(oe, &oid);
}

ಸ್ಥಿರ ರಚನೆ object_ನಮೂದು *dereference(ರಚನೆ object_ನಮೂದು *oe,
					ರಚನೆ object_id *oid)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;
	ಅಕ್ಷರ *buf = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ hexsz = the_hash_algo->hexsz;

	ಆದರೆ (!oe) {
		ಎಣಿಕೆ object_type type = odb_read_object_info(the_repository->objects,
							     oid, ಖಾಲಿ);
		ಆದರೆ (type < 0)
			die(_("object not found: %s"), oid_ಗಮ್ಯ_hex(oid));
		/* cache it! */
		oe = ಸೇರಿಸು_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.ಸ್ಥಾನಾಂತರ = 1;
	}
	ಆಯ್ಕೆ (oe->type) {
	ಸ್ಥಿತಿ OBJ_ವೃಕ್ಷ:	/* easy case. */
		ಹಿಂತಿರುಗಿ oe;
	ಸ್ಥಿತಿ OBJ_COMMIT:
	ಸ್ಥಿತಿ OBJ_ಟ್ಯಾಗ್:
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	ಆದರೆ (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_ನಮೂದು(oe, &ಗಾತ್ರ_2);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಎಣಿಕೆ object_type ಬಳಸದ_2;
		ಗಾತ್ರ_t ಗಾತ್ರ_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &ಬಳಸದ_2, &ಗಾತ್ರ_st);
		ಗಾತ್ರ_2 = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(ಗಾತ್ರ_st);
	}
	ಆದರೆ (!buf)
		die(_("can't load object %s"), oid_ಗಮ್ಯ_hex(oid));

	/* Peel one layer. */
	ಆಯ್ಕೆ (oe->type) {
	ಸ್ಥಿತಿ OBJ_ಟ್ಯಾಗ್:
		ಆದರೆ (ಗಾತ್ರ_2 < hexsz + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("object ") ||
		    get_oid_hex(buf + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ OBJ_COMMIT:
		ಆದರೆ (ಗಾತ್ರ_2 < hexsz + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("tree ") ||
		    get_oid_hex(buf + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	ಬಿಡುಗಡೆ(buf);
	ಹಿಂತಿರುಗಿ ಕಂಡುಹಿಡಿ_object(oid);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸೇರಿಸು_mapped_mark(uintmax_t mark, ಶೂನ್ಯ *object, ಶೂನ್ಯ *cbp)
{
	ರಚನೆ object_id *fromoid = object;
	ರಚನೆ object_id *tooid = ಕಂಡುಹಿಡಿ_mark(cbp, mark);
	ಪೂರ್ಣಾಂಕ ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	ಆದರೆ (ret == 0)
		ಹಿಂತಿರುಗಿ;
	kh_ಮೌಲ್ಯ(sub_oid_map, it) = tooid;
}

ಸ್ಥಿರ ಶೂನ್ಯ build_mark_map_one(ರಚನೆ mark_ಗಣ *from, ರಚನೆ mark_ಗಣ *ಗಮ್ಯ)
{
	for_each_mark(from, 0, ಸೇರಿಸು_mapped_mark, ಗಮ್ಯ);
}

ಸ್ಥಿರ ಶೂನ್ಯ build_mark_map(ರಚನೆ string_list *from, ರಚನೆ string_list *ಗಮ್ಯ)
{
	ರಚನೆ string_list_ಅಂಶ *fromp, *ತುದಿ;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_ಅಂಶ(fromp, from) {
		ತುದಿ = string_list_lookup(ಗಮ್ಯ, fromp->string);
		ಆದರೆ (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ತುದಿ || !ತುದಿ->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, ತುದಿ->util);
	}
}

ಸ್ಥಿರ ರಚನೆ object_ನಮೂದು *parse_treeish_dataref(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **p)
{
	ರಚನೆ object_id oid;
	ರಚನೆ object_ನಮೂದು *e;

	ಆದರೆ (**p == ':') {	/* <mark> */
		e = ಕಂಡುಹಿಡಿ_mark(marks, parse_mark_ref_space(p));
		ಆದರೆ (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} ಇಲ್ಲದಿದ್ದರೆ {	/* <sha1> */
		ಆದರೆ (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = ಕಂಡುಹಿಡಿ_object(&oid);
		ಆದರೆ (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	ಆಗ (!e || e->type != OBJ_ವೃಕ್ಷ)
		e = dereference(e, &oid);
	ಹಿಂತಿರುಗಿ e;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_ls(ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *hash, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಸ್ಥಿರ ರಚನೆ strbuf ಸಾಲು = STRBUF_INIT;

	/* See show_tree(). */
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type =
		S_ISGITLINK(ಬಹುಲಕ) ? commit_type :
		S_ISDIR(ಬಹುಲಕ) ? ವೃಕ್ಷ_type :
		blob_type;

	ಆದರೆ (!ಬಹುಲಕ) {
		/* missing SP path LF */
		strbuf_reset(&ಸಾಲು);
		strbuf_addstr(&ಸಾಲು, "missing ");
		quote_c_style(ಮಾರ್ಗ, &ಸಾಲು, ಖಾಲಿ, 0);
		strbuf_addch(&ಸಾಲು, '\n');
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&ಸಾಲು);
		strbuf_addf(&ಸಾಲು, "%06o %s %s\t",
				ಬಹುಲಕ & ~NO_DELTA, type, hash_ಗಮ್ಯ_hex(hash));
		quote_c_style(ಮಾರ್ಗ, &ಸಾಲು, ಖಾಲಿ, 0);
		strbuf_addch(&ಸಾಲು, '\n');
	}
	cat_blob_write(ಸಾಲು.buf, ಸಾಲು.len);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_ls(ರಚನೆ fast_import_state *state ಬಳಸದ,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p,
		     ರಚನೆ branch *b)
{
	ಸ್ಥಿರ ರಚನೆ strbuf ಮಾರ್ಗ = STRBUF_INIT;
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು *ಬೇರು = ಖಾಲಿ;
	ರಚನೆ ವೃಕ್ಷ_ನಮೂದು ಎಲೆ = {ಖಾಲಿ};

	/* ls SP (<tree-ish> SP)? <path> */
	ಆದರೆ (*p == '"') {
		ಆದರೆ (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		ಬೇರು = &b->branch_ವೃಕ್ಷ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ object_ನಮೂದು *e = parse_treeish_dataref(&p);
		ಬೇರು = new_ವೃಕ್ಷ_ನಮೂದು();
		oidcpy(&ಬೇರು->versions[1].oid, &e->idx.oid);
		ಆದರೆ (!is_null_oid(&ಬೇರು->versions[1].oid))
			ಬೇರು->versions[1].ಬಹುಲಕ = S_IFDIR;
		ತುಂಬು_ವೃಕ್ಷ(ಬೇರು);
	}
	strbuf_reset(&ಮಾರ್ಗ);
	parse_ಮಾರ್ಗ_eol(&ಮಾರ್ಗ, p, "path");
	ವೃಕ್ಷ_content_get(ಬೇರು, ಮಾರ್ಗ.buf, &ಎಲೆ, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	ಆದರೆ (S_ISDIR(ಎಲೆ.versions[1].ಬಹುಲಕ))
		store_ವೃಕ್ಷ(&ಎಲೆ);

	print_ls(ಎಲೆ.versions[1].ಬಹುಲಕ, ಎಲೆ.versions[1].oid.hash, ಮಾರ್ಗ.buf);
	ಆದರೆ (ಎಲೆ.ವೃಕ್ಷ)
		release_ವೃಕ್ಷ_content_recursive(ಎಲೆ.ವೃಕ್ಷ);
	ಆದರೆ (!b || ಬೇರು != &b->branch_ವೃಕ್ಷ)
		release_ವೃಕ್ಷ_ನಮೂದು(ಬೇರು);
}

ಸ್ಥಿರ ಶೂನ್ಯ checkpoint(ಶೂನ್ಯ)
{
	checkpoint_requested = 0;
	ಆದರೆ (object_ಎಣಿಕೆ) {
		ಚಕ್ರ_packfile();
	}
	dump_branches();
	dump_ಟ್ಯಾಗುಗಳು();
	dump_marks();
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_checkpoint(ರಚನೆ fast_import_state *state ಬಳಸದ)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_ಪ್ರಗತಿ(ರಚನೆ fast_import_state *state ಬಳಸದ)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_alias(ರಚನೆ fast_import_state *state)
{
	ರಚನೆ object_ನಮೂದು *e;
	ರಚನೆ branch b;

	skip_optional_lf();
	read_ಮುಂದಿನ_command(state);

	/* mark ... */
	parse_mark(state);
	ಆದರೆ (!ಮುಂದಿನ_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, ಗಾತ್ರ(b));
	ಆದರೆ (!parse_objectish_with_ಪೂರ್ವಪ್ರತ್ಯಯ(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = ಕಂಡುಹಿಡಿ_object(&b.oid);
	ಪ್ರತಿಜ್ಞಾ(e);
	ಸೇರಿಸು_mark(&marks, ಮುಂದಿನ_mark, e);
}

ಸ್ಥಿರ ಅಕ್ಷರ* make_fast_import_ಮಾರ್ಗ(ರಚನೆ fast_import_state *state,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಆದರೆ (!relative_marks_paths || is_absolute_ಮಾರ್ಗ(ಮಾರ್ಗ))
		ಹಿಂತಿರುಗಿ ಪೂರ್ವಪ್ರತ್ಯಯ_filename(state->ಪೂರ್ವಪ್ರತ್ಯಯ, ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ repo_git_ಮಾರ್ಗ(the_repository, "info/fast-import/%s", ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_import_marks(ರಚನೆ fast_import_state *state,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *marks,
				ಪೂರ್ಣಾಂಕ from_ಹರಿವು,
				ಪೂರ್ಣಾಂಕ ignore_missing)
{
	ಆದರೆ (import_marks_file) {
		ಆದರೆ (from_ಹರಿವು)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		ಆದರೆ(!import_marks_file_from_ಹರಿವು)
			read_marks();
	}

	ಬಿಡುಗಡೆ(import_marks_file);
	import_marks_file = make_fast_import_ಮಾರ್ಗ(state, marks);
	import_marks_file_from_ಹರಿವು = from_ಹರಿವು;
	import_marks_file_ignore_missing = ignore_missing;
}

ಸ್ಥಿರ ಶೂನ್ಯ option_date_format(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt)
{
	ಆದರೆ (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("unknown --date-format argument %s"), fmt);
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ulong_arg(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಅಕ್ಷರ *endptr;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ rv = strtoul(arg, &endptr, 0);
	ಆದರೆ (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	ಹಿಂತಿರುಗಿ rv;
}

ಸ್ಥಿರ ಶೂನ್ಯ option_ಆಳ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಆಳ)
{
	max_ಆಳ = ulong_arg("--depth", ಆಳ);
	ಆದರೆ (max_ಆಳ > MAX_ಆಳ)
		die(_("--depth cannot exceed %u"), MAX_ಆಳ);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_ಸಕ್ರಿಯ_branches(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branches)
{
	max_ಸಕ್ರಿಯ_branches = ulong_arg("--active-branches", branches);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_export_marks(ರಚನೆ fast_import_state *state,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *marks)
{
	ಬಿಡುಗಡೆ(export_marks_file);
	export_marks_file = make_fast_import_ಮಾರ್ಗ(state, marks);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_cat_blob_fd(ರಚನೆ fast_import_state *state ಬಳಸದ,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fd)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ n = ulong_arg("--cat-blob-fd", fd);
	ಆದರೆ (n > (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
		die(_("--cat-blob-fd cannot exceed %d"), ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
	cat_blob_fd = (ಪೂರ್ಣಾಂಕ) n;
}

ಸ್ಥಿರ ಶೂನ್ಯ option_export_pack_ಅಂಚುಗಳು(ರಚನೆ fast_import_state *state,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಅಂಚುಗಳು)
{
	ಅಕ್ಷರ *fn = ಪೂರ್ವಪ್ರತ್ಯಯ_filename(state->ಪೂರ್ವಪ್ರತ್ಯಯ, ಅಂಚುಗಳು);
	ಆದರೆ (pack_ಅಂಚುಗಳು)
		fclose(pack_ಅಂಚುಗಳು);
	pack_ಅಂಚುಗಳು = xfopen(fn, "a");
	ಬಿಡುಗಡೆ(fn);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_rewrite_submodules(ರಚನೆ fast_import_state *state,
				      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				      ರಚನೆ string_list *list)
{
	ರಚನೆ mark_ಗಣ *ms;
	FILE *fp;
	ಅಕ್ಷರ *s = xstrdup(arg);
	ಅಕ್ಷರ *f = strchr(s, ':');
	ಆದರೆ (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = ಪೂರ್ವಪ್ರತ್ಯಯ_filename(state->ಪೂರ್ವಪ್ರತ್ಯಯ, f);
	fp = fopen(f, "r");
	ಆದರೆ (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, ಸೇರಿಸು_oid_ನಮೂದು);
	fclose(fp);
	ಬಿಡುಗಡೆ(f);

	string_list_ಸೇರಿಸು(list, s)->util = ms;

	ಬಿಡುಗಡೆ(s);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_max_pack_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ v;

	ಆದರೆ (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	ಆದರೆ (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

ಸ್ಥಿರ ಶೂನ್ಯ option_big_file_ಮಿತಿ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ v;

	ಆದರೆ (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_ಗಣ_big_file_ಮಿತಿ(the_repository, v);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_signed_commits(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಆದರೆ (parse_sign_ಬಹುಲಕ(arg, &signed_commit_ಬಹುಲಕ, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_signed_ಟ್ಯಾಗುಗಳು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಆದರೆ (parse_sign_ಬಹುಲಕ(arg, &signed_ಟ್ಯಾಗ್_ಬಹುಲಕ, &signed_ಟ್ಯಾಗ್_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

ಸ್ಥಿರ ಶೂನ್ಯ option_quiet(ಶೂನ್ಯ)
{
	show_stats = 0;
	quiet = 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_one_option(ರಚನೆ fast_import_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option)
{
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(option, "max-pack-size=", &option)) {
		option_max_pack_ಗಾತ್ರ(option);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(option, "big-file-threshold=", &option)) {
		option_big_file_ಮಿತಿ(option);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(option, "depth=", &option)) {
		option_ಆಳ(option);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(option, "active-branches=", &option)) {
		option_ಸಕ್ರಿಯ_branches(option);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(option, "export-pack-edges=", &option)) {
		option_export_pack_ಅಂಚುಗಳು(state, option);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(option, "signed-tags=", &option)) {
		option_signed_ಟ್ಯಾಗುಗಳು(option);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(option, "quiet")) {
		option_quiet();
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(option, "stats")) {
		show_stats = 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_unsafe_feature(ರಚನೆ fast_import_state *state,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *feature)
{
	ಆದರೆ (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_one_feature(ರಚನೆ fast_import_state *state,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *feature)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg;

	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_ಗಮ್ಯ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(feature, "force")) {
		force_update = 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_feature(ರಚನೆ fast_import_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *feature)
{
	ಆದರೆ (state->ನೋಡಿದ_ದತ್ತಾಂಶ_command)
		die(_("got feature command '%s' after data command"), feature);

	ಆದರೆ (parse_one_feature(state, feature))
		ಹಿಂತಿರುಗಿ;

	die(_("this version of fast-import does not support feature %s."), feature);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_option(ರಚನೆ fast_import_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option)
{
	ಆದರೆ (state->ನೋಡಿದ_ದತ್ತಾಂಶ_command)
		die(_("got option command '%s' after data command"), option);

	ಆದರೆ (parse_one_option(state, option))
		ಹಿಂತಿರುಗಿ;

	die(_("this version of fast-import does not support option: %s"), option);
}

ಸ್ಥಿರ ಶೂನ್ಯ git_pack_config(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ indexversion_ಮೌಲ್ಯ;
	ಪೂರ್ಣಾಂಕ ಎಲ್ಲೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ packsizelimit_ಮೌಲ್ಯ;

	ಆದರೆ (!repo_config_get_ulong(the_repository, "pack.depth", &max_ಆಳ)) {
		ಆದರೆ (max_ಆಳ > MAX_ಆಳ)
			max_ಆಳ = MAX_ಆಳ;
	}
	ಆದರೆ (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_ಮೌಲ್ಯ)) {
		pack_idx_opts.ಆವೃತ್ತಿ = indexversion_ಮೌಲ್ಯ;
		ಆದರೆ (pack_idx_opts.ಆವೃತ್ತಿ > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.ಆವೃತ್ತಿ);
	}
	ಆದರೆ (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_ಮೌಲ್ಯ))
		max_packsize = packsizelimit_ಮೌಲ್ಯ;

	ಆದರೆ (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &ಎಲ್ಲೆ))
		unpack_ಎಲ್ಲೆ = ಎಲ್ಲೆ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!repo_config_get_int(the_repository, "transfer.unpacklimit", &ಎಲ್ಲೆ))
		unpack_ಎಲ್ಲೆ = ಎಲ್ಲೆ;

	repo_config(the_repository, git_default_config, ಖಾಲಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಶೂನ್ಯ parse_argv(ರಚನೆ fast_import_state *state)
{
	ಪೂರ್ಣಾಂಕ argc = parse_options(state->argc, state->argv, state->ಪೂರ್ವಪ್ರತ್ಯಯ,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	ಆದರೆ (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->ನೋಡಿದ_ದತ್ತಾಂಶ_command = 1;
	ಆದರೆ (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_ಗಮ್ಯ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_date_format(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_export_pack_ಅಂಚುಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
					  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_ಅಂಚುಗಳು(opt->ಮೌಲ್ಯ, arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_max_pack_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
				      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_ಗಾತ್ರ(arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_big_file_ಮಿತಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
					   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_ಮಿತಿ(arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_signed_commits(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_signed_ಟ್ಯಾಗುಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_ಟ್ಯಾಗುಗಳು(arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_rewrite_submodules_from(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
						ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->ಮೌಲ್ಯ, arg, &sub_marks_from);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_rewrite_submodules_ಗಮ್ಯ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
					      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->ಮೌಲ್ಯ, arg, &sub_marks_ಗಮ್ಯ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_cat_blob_fd(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->ಮೌಲ್ಯ, arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_import_marks(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->ಮೌಲ್ಯ, arg, 0, 0);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_import_marks_if_exists(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
					       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->ಮೌಲ್ಯ, arg, 0, 1);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_export_marks(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->ಮೌಲ್ಯ, arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_ಆಳ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_ಆಳ(arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_ಸಕ್ರಿಯ_branches(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
					ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_ಸಕ್ರಿಯ_branches(arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_quiet(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt ಬಳಸದ,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg ಬಳಸದ, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_fast_import(ಪೂರ್ಣಾಂಕ argc,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		    ರಚನೆ repository *repo)
{
	ರಚನೆ fast_import_state state;

	ರಚನೆ option fast_import_options[] = {
		OPT_ಗುಂಪು(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", ಖಾಲಿ, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", ಖಾಲಿ, ಖಾಲಿ,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", ಖಾಲಿ, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_ಗಾತ್ರ),
		OPT_CALLBACK_F(0, "big-file-threshold", ಖಾಲಿ, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_ಮಿತಿ),
		OPT_CALLBACK_F(0, "depth", ಖಾಲಿ, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_ಆಳ),
		OPT_CALLBACK_F(0, "active-branches", ಖಾಲಿ, N_("n"),
			       N_("maximum number of branches to maintain active"),
			       PARSE_OPT_NONEG, option_parse_ಸಕ್ರಿಯ_branches),
		OPT_ಗುಂಪು(N_("Marks")),
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
		OPT_ಗುಂಪು(N_("Submodule rewrite")),
		OPT_CALLBACK_F(0, "rewrite-submodules-from", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> from <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_from),
		OPT_CALLBACK_F(0, "rewrite-submodules-to", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> to <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_ಗಮ್ಯ),
		OPT_ಗುಂಪು(N_("Signing")),
		OPT_CALLBACK_F(0, "signed-commits", ಖಾಲಿ, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", ಖಾಲಿ, N_("mode"),
			       N_("how to handle signed tags"),
			       PARSE_OPT_NONEG, option_parse_signed_ಟ್ಯಾಗುಗಳು),
		OPT_HIDDEN_ಗುಂಪು(N_("Advanced")),
		OPT_BOOL_F(0, "allow-unsafe-features", &state.allow_unsafe_features,
			   N_("allow unsafe mark commands from the stream"),
			   PARSE_OPT_HIDDEN | PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "export-pack-edges", &state, N_("file"),
			       N_("dump edge commits to <file>"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_export_pack_ಅಂಚುಗಳು),
		OPT_CALLBACK_F(0, "cat-blob-fd", &state, N_("fd"),
			       N_("write some responses to <fd> instead of stdout"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_cat_blob_fd),
		OPT_ಕೊನೆ()
	};

	show_usage_with_options_if_asked(argc, argv, fast_import_usage, fast_import_options);

	fast_import_state_init(&state, argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, fast_import_options);

	reset_pack_idx_option(&pack_idx_opts);
	git_pack_config();

	alloc_objects(object_ನಮೂದು_alloc);
	strbuf_init(&command_buf, 0);
	CALLOC_ARRAY(atom_ಕೋಷ್ಟಕ, atom_ಕೋಷ್ಟಕ_sz);
	CALLOC_ARRAY(branch_ಕೋಷ್ಟಕ, branch_ಕೋಷ್ಟಕ_sz);
	CALLOC_ARRAY(avail_ವೃಕ್ಷ_ಕೋಷ್ಟಕ, avail_ವೃಕ್ಷ_ಕೋಷ್ಟಕ_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, ಗಾತ್ರ(ರಚನೆ mark_ಗಣ));

	hashmap_init(&object_ಕೋಷ್ಟಕ, object_ನಮೂದು_hashcmp, ಖಾಲಿ, 0);

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
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];
		ಆದರೆ (*arg != '-' || !strcmp(arg, "--"))
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * ಗಾತ್ರ(*rc_free));
	ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0; i < (cmd_save - 1); i++)
		rc_free[i].ಮುಂದಿನ = &rc_free[i + 1];
	rc_free[cmd_save - 1].ಮುಂದಿನ = ಖಾಲಿ;

	ಆರಂಭ_packfile();
	ಗಣ_die_routine(die_nicely);
	ಗಣ_checkpoint_signal();
	ಆಗ (read_ಮುಂದಿನ_command(&state) != EOF) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v;
		ಆದರೆ (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "tag ", &v))
			parse_new_ಟ್ಯಾಗ್(&state, v);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, ಖಾಲಿ);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp("done", command_buf.buf))
			ನಿಲ್ಲಿಸು;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(command_buf.buf, "progress "))
			parse_ಪ್ರಗತಿ(&state);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("unsupported command: %s"), command_buf.buf);

		ಆದರೆ (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	ಆದರೆ (!state.ನೋಡಿದ_ದತ್ತಾಂಶ_command)
		parse_argv(&state);

	ಆದರೆ (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	ಕೊನೆ_packfile();

	dump_branches();
	dump_ಟ್ಯಾಗುಗಳು();
	unkeep_all_packs();
	dump_marks();

	ಆದರೆ (pack_ಅಂಚುಗಳು)
		fclose(pack_ಅಂಚುಗಳು);

	ಆದರೆ (show_stats) {
		uintmax_t ಒಟ್ಟು_ಎಣಿಕೆ = 0, duplicate_ಎಣಿಕೆ = 0;
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ARRAY_ಗಾತ್ರ(object_ಎಣಿಕೆ_by_type); i++)
			ಒಟ್ಟು_ಎಣಿಕೆ += object_ಎಣಿಕೆ_by_type[i];
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ARRAY_ಗಾತ್ರ(duplicate_ಎಣಿಕೆ_by_type); i++)
			duplicate_ಎಣಿಕೆ += duplicate_ಎಣಿಕೆ_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_ಎಣಿಕೆ);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", ಒಟ್ಟು_ಎಣಿಕೆ, duplicate_ಎಣಿಕೆ);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ಎಣಿಕೆ_by_type[OBJ_BLOB], duplicate_ಎಣಿಕೆ_by_type[OBJ_BLOB], delta_ಎಣಿಕೆ_by_type[OBJ_BLOB], delta_ಎಣಿಕೆ_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ಎಣಿಕೆ_by_type[OBJ_ವೃಕ್ಷ], duplicate_ಎಣಿಕೆ_by_type[OBJ_ವೃಕ್ಷ], delta_ಎಣಿಕೆ_by_type[OBJ_ವೃಕ್ಷ], delta_ಎಣಿಕೆ_attempts_by_type[OBJ_ವೃಕ್ಷ]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ಎಣಿಕೆ_by_type[OBJ_COMMIT], duplicate_ಎಣಿಕೆ_by_type[OBJ_COMMIT], delta_ಎಣಿಕೆ_by_type[OBJ_COMMIT], delta_ಎಣಿಕೆ_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_ಎಣಿಕೆ_by_type[OBJ_ಟ್ಯಾಗ್], duplicate_ಎಣಿಕೆ_by_type[OBJ_ಟ್ಯಾಗ್], delta_ಎಣಿಕೆ_by_type[OBJ_ಟ್ಯಾಗ್], delta_ಎಣಿಕೆ_attempts_by_type[OBJ_ಟ್ಯಾಗ್]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_ಎಣಿಕೆ, branch_ತುಂಬು_ಎಣಿಕೆ);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->ಸರಿಸು) * 1024, marks_ಗಣ_ಎಣಿಕೆ);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (ವೃಕ್ಷ_ನಮೂದು_allocd + fi_mem_pool.pool_alloc + alloc_ಎಣಿಕೆ*ಗಾತ್ರ(ರಚನೆ object_ನಮೂದು))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)((ವೃಕ್ಷ_ನಮೂದು_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_ಎಣಿಕೆ*ಗಾತ್ರ(ರಚನೆ object_ನಮೂದು))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	ಹಿಂತಿರುಗಿ failure ? 1 : 0;
}
