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

static struct treeent {
	unsigned mode;
	struct object_id oid;
	정수 len;
	char name[FLEX_ARRAY];
} **entries;
static 정수 alloc, used;

static void append_to_tree(unsigned mode, struct object_id *oid, char *path)
{
	struct treeent *ent;
	size_t len = strlen(path);
	만약 (strchr(path, '/'))
		die("path %s contains slash", path);

	FLEX_ALLOC_MEM(ent, name, path, len);
	ent->mode = mode;
	ent->len = len;
	oidcpy(&ent->oid, oid);

	ALLOC_GROW(entries, used + 1, alloc);
	entries[used++] = ent;
}

static 정수 ent_compare(const void *a_, const void *b_)
{
	struct treeent *a = *(struct treeent **)a_;
	struct treeent *b = *(struct treeent **)b_;
	반환 base_name_compare(a->name, a->len, a->mode,
				 b->name, b->len, b->mode);
}

static void write_tree(struct repository *repo, struct object_id *oid)
{
	struct strbuf buf;
	size_t size;
	정수 i;

	QSORT(entries, used, ent_compare);
	위해 (size = i = 0; i < used; i++)
		size += 32 + entries[i]->len;

	strbuf_init(&buf, size);
	위해 (i = 0; i < used; i++) {
		struct treeent *ent = entries[i];
		strbuf_addf(&buf, "%o %s%c", ent->mode, ent->name, '\0');
		strbuf_add(&buf, ent->oid.hash, repo->hash_algo->rawsz);
	}

	odb_write_object(repo->objects, buf.buf, buf.len, OBJ_TREE, oid);
	strbuf_release(&buf);
}

static const char *const mktree_usage[] = {
	"git mktree [-z] [--missing] [--batch]",
	NULL
};

static void mktree_line(struct repository *repo, char *buf, 정수 nul_term_line, 정수 allow_missing)
{
	char *ptr, *ntr;
	const char *p;
	unsigned mode;
	enum object_type mode_type; /* object type derived from mode */
	enum object_type obj_type; /* object type derived from sha */
	struct object_info oi = OBJECT_INFO_INIT;
	char *path, *to_free = NULL;
	struct object_id oid;

	ptr = buf;
	/*
	 * Read non-recursive ls-tree output format:
	 *     mode SP type SP sha1 TAB name
	 */
	mode = strtoul(ptr, &ntr, 8);
	만약 (ptr == ntr || !ntr || *ntr != ' ')
		die("input format error: %s", buf);
	ptr = ntr + 1; /* type */
	ntr = strchr(ptr, ' ');
	만약 (!ntr || parse_oid_hex_algop(ntr + 1, &oid, &p, repo->hash_algo) ||
	    *p != '\t')
		die("input format error: %s", buf);

	/* It is perfectly normal if we do not have a commit from a submodule */
	만약 (S_ISGITLINK(mode))
		allow_missing = 1;


	*ntr++ = 0; /* now at the beginning of SHA1 */

	path = (char *)p + 1;  /* at the beginning of name */
	만약 (!nul_term_line && path[0] == '"') {
		struct strbuf p_uq = STRBUF_INIT;
		만약 (unquote_c_style(&p_uq, path, NULL))
			die("invalid quoting");
		path = to_free = strbuf_detach(&p_uq, NULL);
	}

	/*
	 * Object type is redundantly derivable three ways.
	 * These should all agree.
	 */
	mode_type = object_type(mode);
	만약 (mode_type != type_from_string(ptr)) {
		die("entry '%s' object type (%s) doesn't match mode type (%s)",
			path, ptr, type_name(mode_type));
	}

	/* Check the type of object identified by oid without fetching objects */
	oi.typep = &obj_type;
	만약 (odb_read_object_info_extended(repo->objects, &oid, &oi,
					  OBJECT_INFO_LOOKUP_REPLACE |
					  OBJECT_INFO_SKIP_FETCH_OBJECT) < 0)
		obj_type = -1;

	만약 (obj_type < 0) {
		만약 (allow_missing) {
			; /* no problem - missing objects are presumed to be of the right type */
		} 아니면 {
			die("entry '%s' object %s is unavailable", path, oid_to_hex(&oid));
		}
	} 아니면 {
		만약 (obj_type != mode_type) {
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

정수 cmd_mktree(정수 ac,
	       const char **av,
	       const char *prefix,
	       struct repository *repo)
{
	struct strbuf sb = STRBUF_INIT;
	struct object_id oid;
	정수 nul_term_line = 0;
	정수 allow_missing = 0;
	정수 is_batch_mode = 0;
	정수 got_eof = 0;
	strbuf_getline_fn getline_fn;

	const struct option option[] = {
		OPT_BOOL('z', NULL, &nul_term_line, N_("input is NUL terminated")),
		OPT_SET_INT( 0 , "missing", &allow_missing, N_("allow missing objects"), 1),
		OPT_SET_INT( 0 , "batch", &is_batch_mode, N_("allow creation of more than one tree"), 1),
		OPT_END()
	};

	ac = parse_options(ac, av, prefix, option, mktree_usage, 0);
	getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;

	동안 (!got_eof) {
		동안 (1) {
			만약 (getline_fn(&sb, stdin) == EOF) {
				got_eof = 1;
				break;
			}
			만약 (sb.buf[0] == '\0') {
				/* empty lines denote tree boundaries in batch mode */
				만약 (is_batch_mode)
					break;
				die("input format error: (blank line only valid in batch mode)");
			}
			mktree_line(repo, sb.buf, nul_term_line, allow_missing);
		}
		만약 (is_batch_mode && got_eof && used < 1) {
			/*
			 * Execution gets here if the last tree entry is terminated with a
			 * new-line.  The final new-line has been made optional to be
			 * consistent with the original non-batch behaviour of mktree.
			 */
			; /* skip creating an empty tree */
		} 아니면 {
			write_tree(repo, &oid);
			puts(oid_to_hex(&oid));
			fflush(stdout);
		}
		위해 (정수 i = 0; i < used; i++)
			free(entries[i]);
		used=0; /* reset tree entry buffer for re-use in batch mode */
	}
	free(entries);
	strbuf_release(&sb);

	반환 0;
}
