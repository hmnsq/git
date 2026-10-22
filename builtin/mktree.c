/*
 * GIT - the stupid content tracker
 *
 * Copyright (c) Junio C Hamano, 2006, 2009
 */
#شامل "builtin.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "quote.h"
#شامل "strbuf.h"
#شامل "tree.h"
#شامل "parse-options.h"
#شامل "object-file.h"
#شامل "odb.h"

ایستا ساختار treeent {
	unsigned mode;
	ساختار object_id oid;
	صحیح len;
	نویسه name[FLEX_ARRAY];
} **entries;
ایستا صحیح alloc, used;

ایستا تهی append_to_درخت(unsigned mode, ساختار object_id *oid, نویسه *path)
{
	ساختار treeent *ent;
	size_t len = strlen(path);
	اگر (strchr(path, '/'))
		die("path %s contains slash", path);

	FLEX_ALLOC_MEM(ent, name, path, len);
	ent->mode = mode;
	ent->len = len;
	oidcpy(&ent->oid, oid);

	ALLOC_GROW(entries, used + 1, alloc);
	entries[used++] = ent;
}

ایستا صحیح ent_compare(ثابت تهی *a_, ثابت تهی *b_)
{
	ساختار treeent *a = *(ساختار treeent **)a_;
	ساختار treeent *b = *(ساختار treeent **)b_;
	بازگشت base_name_compare(a->name, a->len, a->mode,
				 b->name, b->len, b->mode);
}

ایستا تهی write_درخت(ساختار repository *repo, ساختار object_id *oid)
{
	ساختار strbuf buf;
	size_t size;
	صحیح i;

	QSORT(entries, used, ent_compare);
	برای (size = i = 0; i < used; i++)
		size += 32 + entries[i]->len;

	strbuf_init(&buf, size);
	برای (i = 0; i < used; i++) {
		ساختار treeent *ent = entries[i];
		strbuf_addf(&buf, "%o %s%c", ent->mode, ent->name, '\0');
		strbuf_add(&buf, ent->oid.hash, repo->hash_algo->rawsz);
	}

	odb_write_object(repo->objects, buf.buf, buf.len, OBJ_درخت, oid);
	strbuf_release(&buf);
}

ایستا ثابت نویسه *ثابت mktree_usage[] = {
	"git mktree [-z] [--missing] [--batch]",
	پوچ
};

ایستا تهی mktree_سطر(ساختار repository *repo, نویسه *buf, صحیح nul_term_سطر, صحیح allow_missing)
{
	نویسه *ptr, *ntr;
	ثابت نویسه *p;
	unsigned mode;
	شمارش object_type mode_type; /* object type derived from mode */
	شمارش object_type obj_type; /* object type derived from sha */
	ساختار object_info oi = OBJECT_INFO_INIT;
	نویسه *path, *to_free = پوچ;
	ساختار object_id oid;

	ptr = buf;
	/*
	 * Read non-recursive ls-tree output format:
	 *     mode SP type SP sha1 TAB name
	 */
	mode = strtoul(ptr, &ntr, 8);
	اگر (ptr == ntr || !ntr || *ntr != ' ')
		die("input format error: %s", buf);
	ptr = ntr + 1; /* type */
	ntr = strchr(ptr, ' ');
	اگر (!ntr || parse_oid_hex_algop(ntr + 1, &oid, &p, repo->hash_algo) ||
	    *p != '\t')
		die("input format error: %s", buf);

	/* It is perfectly normal if we do not have a commit from a submodule */
	اگر (S_ISGITLINK(mode))
		allow_missing = 1;


	*ntr++ = 0; /* now at the beginning of SHA1 */

	path = (نویسه *)p + 1;  /* at the beginning of name */
	اگر (!nul_term_سطر && path[0] == '"') {
		ساختار strbuf p_uq = STRBUF_INIT;
		اگر (unquote_c_style(&p_uq, path, پوچ))
			die("invalid quoting");
		path = to_free = strbuf_detach(&p_uq, پوچ);
	}

	/*
	 * Object type is redundantly derivable three ways.
	 * These should all agree.
	 */
	mode_type = object_type(mode);
	اگر (mode_type != type_from_string(ptr)) {
		die("entry '%s' object type (%s) doesn't match mode type (%s)",
			path, ptr, type_name(mode_type));
	}

	/* Check the type of object identified by oid without fetching objects */
	oi.typep = &obj_type;
	اگر (odb_read_object_info_extended(repo->objects, &oid, &oi,
					  OBJECT_INFO_LOOKUP_REPLACE |
					  OBJECT_INFO_SKIP_FETCH_OBJECT) < 0)
		obj_type = -1;

	اگر (obj_type < 0) {
		اگر (allow_missing) {
			; /* no problem - missing objects are presumed to be of the right type */
		} وگرنه {
			die("entry '%s' object %s is unavailable", path, oid_to_hex(&oid));
		}
	} وگرنه {
		اگر (obj_type != mode_type) {
			/*
			 * The object exists but is of the wrong type.
			 * This is a problem regardless of allow_missing
			 * because the new tree entry will never be correct.
			 */
			die("entry '%s' object %s is a %s but specified type was (%s)",
				path, oid_to_hex(&oid), type_name(obj_type), type_name(mode_type));
		}
	}

	append_to_درخت(mode, &oid, path);
	آزاد(to_free);
}

صحیح cmd_mktree(صحیح ac,
	       ثابت نویسه **av,
	       ثابت نویسه *prefix,
	       ساختار repository *repo)
{
	ساختار strbuf sb = STRBUF_INIT;
	ساختار object_id oid;
	صحیح nul_term_سطر = 0;
	صحیح allow_missing = 0;
	صحیح is_batch_mode = 0;
	صحیح got_eof = 0;
	strbuf_getline_fn getline_fn;

	ثابت ساختار option option[] = {
		OPT_BOOL('z', پوچ, &nul_term_سطر, N_("input is NUL terminated")),
		OPT_SET_INT( 0 , "missing", &allow_missing, N_("allow missing objects"), 1),
		OPT_SET_INT( 0 , "batch", &is_batch_mode, N_("allow creation of more than one tree"), 1),
		OPT_END()
	};

	ac = parse_options(ac, av, prefix, option, mktree_usage, 0);
	getline_fn = nul_term_سطر ? strbuf_getline_nul : strbuf_getline_lf;

	وقتی (!got_eof) {
		وقتی (1) {
			اگر (getline_fn(&sb, stdin) == EOF) {
				got_eof = 1;
				بشکن;
			}
			اگر (sb.buf[0] == '\0') {
				/* empty lines denote tree boundaries in batch mode */
				اگر (is_batch_mode)
					بشکن;
				die("input format error: (blank line only valid in batch mode)");
			}
			mktree_سطر(repo, sb.buf, nul_term_سطر, allow_missing);
		}
		اگر (is_batch_mode && got_eof && used < 1) {
			/*
			 * Execution gets here if the last tree entry is terminated with a
			 * new-line.  The final new-line has been made optional to be
			 * consistent with the original non-batch behaviour of mktree.
			 */
			; /* skip creating an empty tree */
		} وگرنه {
			write_درخت(repo, &oid);
			puts(oid_to_hex(&oid));
			fflush(stdout);
		}
		برای (صحیح i = 0; i < used; i++)
			آزاد(entries[i]);
		used=0; /* reset tree entry buffer for re-use in batch mode */
	}
	آزاد(entries);
	strbuf_release(&sb);

	بازگشت 0;
}
