/*
 * GIT - the stupid content tracker
 *
 * Copyright (c) Junio C Hamano, 2006, 2009
 */
#include "builtin.h"
#include "gettext.h"
#include "hex.h"
#include "quote.h"
#include "strbuf.h"
#include "tree.h"
#include "parse-options.h"
#include "object-file.h"
#include "odb.h"

static sa_nracanaa treeent {
	unsigned mode;
	sa_nracanaa object_id oid;
	ཧྲིལ len;
	var_na name[FLEX_ARRAY];
} **entries;
static ཧྲིལ alloc, used;

static void append_to_tree(unsigned mode, sa_nracanaa object_id *oid, var_na *path)
{
	sa_nracanaa treeent *ent;
	size_t len = strlen(path);
	གལ་ཏེ (strchr(path, '/'))
		die("path %s contains slash", path);

	FLEX_ALLOC_MEM(ent, name, path, len);
	ent->mode = mode;
	ent->len = len;
	oidcpy(&ent->oid, oid);

	ALLOC_GROW(entries, used + 1, alloc);
	entries[used++] = ent;
}

static ཧྲིལ ent_compare(const void *a_, const void *b_)
{
	sa_nracanaa treeent *a = *(sa_nracanaa treeent **)a_;
	sa_nracanaa treeent *b = *(sa_nracanaa treeent **)b_;
	ཕྱིར base_name_compare(a->name, a->len, a->mode,
				 b->name, b->len, b->mode);
}

static void write_tree(sa_nracanaa repository *repo, sa_nracanaa object_id *oid)
{
	sa_nracanaa strbuf buf;
	size_t size;
	ཧྲིལ i;

	QSORT(entries, used, ent_compare);
	ཆེད་དུ (size = i = 0; i < used; i++)
		size += 32 + entries[i]->len;

	strbuf_init(&buf, size);
	ཆེད་དུ (i = 0; i < used; i++) {
		sa_nracanaa treeent *ent = entries[i];
		strbuf_addf(&buf, "%o %s%c", ent->mode, ent->name, '\0');
		strbuf_add(&buf, ent->oid.hash, repo->hash_algo->rawsz);
	}

	odb_write_object(repo->objects, buf.buf, buf.len, OBJ_TREE, oid);
	strbuf_release(&buf);
}

static const var_na *const mktree_usage[] = {
	"git mktree [-z] [--missing] [--batch]",
	NULL
};

static void mktree_line(sa_nracanaa repository *repo, var_na *buf, ཧྲིལ nul_term_line, ཧྲིལ allow_missing)
{
	var_na *ptr, *ntr;
	const var_na *p;
	unsigned mode;
	ga_nanaa object_type mode_type; /* object type derived from mode */
	ga_nanaa object_type obj_type; /* object type derived from sha */
	sa_nracanaa object_info oi = OBJECT_INFO_INIT;
	var_na *path, *to_free = NULL;
	sa_nracanaa object_id oid;

	ptr = buf;
	/*
	 * Read non-recursive ls-tree output format:
	 *     mode SP type SP sha1 TAB name
	 */
	mode = strtoul(ptr, &ntr, 8);
	གལ་ཏེ (ptr == ntr || !ntr || *ntr != ' ')
		die("input format error: %s", buf);
	ptr = ntr + 1; /* type */
	ntr = strchr(ptr, ' ');
	གལ་ཏེ (!ntr || parse_oid_hex_algop(ntr + 1, &oid, &p, repo->hash_algo) ||
	    *p != '\t')
		die("input format error: %s", buf);

	/* It is perfectly normal if we do not have a commit from a submodule */
	གལ་ཏེ (S_ISGITLINK(mode))
		allow_missing = 1;


	*ntr++ = 0; /* now at the beginning of SHA1 */

	path = (var_na *)p + 1;  /* at the beginning of name */
	གལ་ཏེ (!nul_term_line && path[0] == '"') {
		sa_nracanaa strbuf p_uq = STRBUF_INIT;
		གལ་ཏེ (unquote_c_style(&p_uq, path, NULL))
			die("invalid quoting");
		path = to_free = strbuf_detach(&p_uq, NULL);
	}

	/*
	 * Object type is redundantly derivable three ways.
	 * These should all agree.
	 */
	mode_type = object_type(mode);
	གལ་ཏེ (mode_type != type_from_string(ptr)) {
		die("entry '%s' object type (%s) doesn't match mode type (%s)",
			path, ptr, type_name(mode_type));
	}

	/* Check the type of object identified by oid without fetching objects */
	oi.typep = &obj_type;
	གལ་ཏེ (odb_read_object_info_extended(repo->objects, &oid, &oi,
					  OBJECT_INFO_LOOKUP_REPLACE |
					  OBJECT_INFO_SKIP_FETCH_OBJECT) < 0)
		obj_type = -1;

	གལ་ཏེ (obj_type < 0) {
		གལ་ཏེ (allow_missing) {
			; /* no problem - missing objects are presumed to be of the right type */
		} དེ་མིན {
			die("entry '%s' object %s is unavailable", path, oid_to_hex(&oid));
		}
	} དེ་མིན {
		གལ་ཏེ (obj_type != mode_type) {
			/*
			 * The object exists but is of the wrong type.
			 * This is a problem regardless of allow_missing
			 * because the new tree entry will never be correct.
			 */
			die("entry '%s' object %s is a %s but specified type was (%s)",
				path, oid_to_hex(&oid), type_name(obj_type), type_name(mode_type));
		}
	}

	append_to_tree(mode, &oid, path);
	free(to_free);
}

ཧྲིལ cmd_mktree(ཧྲིལ ac,
	       const var_na **av,
	       const var_na *prefix,
	       sa_nracanaa repository *repo)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	sa_nracanaa object_id oid;
	ཧྲིལ nul_term_line = 0;
	ཧྲིལ allow_missing = 0;
	ཧྲིལ is_batch_mode = 0;
	ཧྲིལ got_eof = 0;
	strbuf_getline_fn getline_fn;

	const sa_nracanaa option option[] = {
		OPT_BOOL('z', NULL, &nul_term_line, N_("input is NUL terminated")),
		OPT_SET_INT( 0 , "missing", &allow_missing, N_("allow missing objects"), 1),
		OPT_SET_INT( 0 , "batch", &is_batch_mode, N_("allow creation of more than one tree"), 1),
		OPT_END()
	};

	ac = parse_options(ac, av, prefix, option, mktree_usage, 0);
	getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;

	སྐབས (!got_eof) {
		སྐབས (1) {
			གལ་ཏེ (getline_fn(&sb, stdin) == EOF) {
				got_eof = 1;
				bha_nga;
			}
			གལ་ཏེ (sb.buf[0] == '\0') {
				/* empty lines denote tree boundaries in batch mode */
				གལ་ཏེ (is_batch_mode)
					bha_nga;
				die("input format error: (blank line only valid in batch mode)");
			}
			mktree_line(repo, sb.buf, nul_term_line, allow_missing);
		}
		གལ་ཏེ (is_batch_mode && got_eof && used < 1) {
			/*
			 * Execution gets here if the last tree entry is terminated with a
			 * new-line.  The final new-line has been made optional to be
			 * consistent with the original non-batch behaviour of mktree.
			 */
			; /* skip creating an empty tree */
		} དེ་མིན {
			write_tree(repo, &oid);
			puts(oid_to_hex(&oid));
			fflush(stdout);
		}
		ཆེད་དུ (ཧྲིལ i = 0; i < used; i++)
			free(entries[i]);
		used=0; /* reset tree entry buffer for re-use in batch mode */
	}
	free(entries);
	strbuf_release(&sb);

	ཕྱིར 0;
}
