/*
 * GIT - the stupid content tracker
 *
 * Copyright (c) Junio C Hamano, 2006, 2009
 */
#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "quote.h"
#ضمن "strbuf.h"
#ضمن "tree.h"
#ضمن "parse-options.h"
#ضمن "object-file.h"
#ضمن "odb.h"

ساكن بنية treeent {
	بیعلامت نما;
	بنية object_id oid;
	صحيح len;
	حرف name[FLEX_ARRAY];
} **entries;
ساكن صحيح alloc, استفاده_شده_2;

ساكن فراغ append_مقصد_درخت(بیعلامت نما, بنية object_id *oid, حرف *مسیر)
{
	بنية treeent *ent;
	اندازه_t len = طول_رشته(مسیر);
	إذا (strchr(مسیر, '/'))
		die("path %s contains slash", مسیر);

	FLEX_ALLOC_MEM(ent, name, مسیر, len);
	ent->نما = نما;
	ent->len = len;
	oidcpy(&ent->oid, oid);

	ALLOC_GROW(entries, استفاده_شده_2 + 1, alloc);
	entries[استفاده_شده_2++] = ent;
}

ساكن صحيح ent_compare(ثابت فراغ *a_, ثابت فراغ *b_)
{
	بنية treeent *a = *(بنية treeent **)a_;
	بنية treeent *b = *(بنية treeent **)b_;
	أرجع پایه_name_compare(a->name, a->len, a->نما,
				 b->name, b->len, b->نما);
}

ساكن فراغ write_درخت(بنية repository *repo, بنية object_id *oid)
{
	بنية strbuf buf;
	اندازه_t اندازه;
	صحيح i;

	QSORT(entries, استفاده_شده_2, ent_compare);
	لكل (اندازه = i = 0; i < استفاده_شده_2; i++)
		اندازه += 32 + entries[i]->len;

	strbuf_init(&buf, اندازه);
	لكل (i = 0; i < استفاده_شده_2; i++) {
		بنية treeent *ent = entries[i];
		strbuf_addf(&buf, "%o %s%c", ent->نما, ent->name, '\0');
		strbuf_جمع_کن(&buf, ent->oid.hash, repo->hash_algo->rawsz);
	}

	odb_write_object(repo->objects, buf.buf, buf.len, OBJ_درخت, oid);
	strbuf_release(&buf);
}

ساكن ثابت حرف *ثابت mktree_usage[] = {
	"git mktree [-z] [--missing] [--batch]",
	عدم
};

ساكن فراغ mktree_سطر(بنية repository *repo, حرف *buf, صحيح nul_جمله_سطر, صحيح allow_missing)
{
	حرف *ptr, *ntr;
	ثابت حرف *p;
	بیعلامت نما;
	تعداد object_type نما_type; /* object type derived from mode */
	تعداد object_type obj_type; /* object type derived from sha */
	بنية object_info oi = OBJECT_INFO_INIT;
	حرف *مسیر, *مقصد_free = عدم;
	بنية object_id oid;

	ptr = buf;
	/*
	 * Read non-recursive ls-tree output format:
	 *     mode SP type SP sha1 TAB name
	 */
	نما = strtoul(ptr, &ntr, 8);
	إذا (ptr == ntr || !ntr || *ntr != ' ')
		die("input format error: %s", buf);
	ptr = ntr + 1; /* type */
	ntr = strchr(ptr, ' ');
	إذا (!ntr || parse_oid_hex_algop(ntr + 1, &oid, &p, repo->hash_algo) ||
	    *p != '\t')
		die("input format error: %s", buf);

	/* It is perfectly normal if we do not have a commit from a submodule */
	إذا (S_ISGITLINK(نما))
		allow_missing = 1;


	*ntr++ = 0; /* now at the beginning of SHA1 */

	مسیر = (حرف *)p + 1;  /* at the beginning of name */
	إذا (!nul_جمله_سطر && مسیر[0] == '"') {
		بنية strbuf p_uq = STRBUF_INIT;
		إذا (unquote_c_style(&p_uq, مسیر, عدم))
			die("invalid quoting");
		مسیر = مقصد_free = strbuf_detach(&p_uq, عدم);
	}

	/*
	 * Object type is redundantly derivable three ways.
	 * These should all agree.
	 */
	نما_type = object_type(نما);
	إذا (نما_type != type_from_string(ptr)) {
		die("entry '%s' object type (%s) doesn't match mode type (%s)",
			مسیر, ptr, type_name(نما_type));
	}

	/* Check the type of object identified by oid without fetching objects */
	oi.typep = &obj_type;
	إذا (odb_read_object_info_extended(repo->objects, &oid, &oi,
					  OBJECT_INFO_LOOKUP_REPLACE |
					  OBJECT_INFO_SKIP_FETCH_OBJECT) < 0)
		obj_type = -1;

	إذا (obj_type < 0) {
		إذا (allow_missing) {
			; /* no problem - missing objects are presumed to be of the right type */
		} وإلا {
			die("entry '%s' object %s is unavailable", مسیر, oid_مقصد_hex(&oid));
		}
	} وإلا {
		إذا (obj_type != نما_type) {
			/*
			 * The object exists but is of the wrong type.
			 * This is a problem regardless of allow_missing
			 * because the new tree entry will never be correct.
			 */
			die("entry '%s' object %s is a %s but specified type was (%s)",
				مسیر, oid_مقصد_hex(&oid), type_name(obj_type), type_name(نما_type));
		}
	}

	append_مقصد_درخت(نما, &oid, مسیر);
	حرر(مقصد_free);
}

صحيح cmd_mktree(صحيح ac,
	       ثابت حرف **av,
	       ثابت حرف *پیشوند,
	       بنية repository *repo)
{
	بنية strbuf sb = STRBUF_INIT;
	بنية object_id oid;
	صحيح nul_جمله_سطر = 0;
	صحيح allow_missing = 0;
	صحيح is_batch_نما = 0;
	صحيح got_eof = 0;
	strbuf_getline_fn getline_fn;

	ثابت بنية option option[] = {
		OPT_BOOL('z', عدم, &nul_جمله_سطر, N_("input is NUL terminated")),
		OPT_مجموعه_INT( 0 , "missing", &allow_missing, N_("allow missing objects"), 1),
		OPT_مجموعه_INT( 0 , "batch", &is_batch_نما, N_("allow creation of more than one tree"), 1),
		OPT_انتها()
	};

	ac = parse_options(ac, av, پیشوند, option, mktree_usage, 0);
	getline_fn = nul_جمله_سطر ? strbuf_getline_nul : strbuf_getline_lf;

	طالما (!got_eof) {
		طالما (1) {
			إذا (getline_fn(&sb, stdin) == EOF) {
				got_eof = 1;
				اكسر;
			}
			إذا (sb.buf[0] == '\0') {
				/* empty lines denote tree boundaries in batch mode */
				إذا (is_batch_نما)
					اكسر;
				die("input format error: (blank line only valid in batch mode)");
			}
			mktree_سطر(repo, sb.buf, nul_جمله_سطر, allow_missing);
		}
		إذا (is_batch_نما && got_eof && استفاده_شده_2 < 1) {
			/*
			 * Execution gets here if the last tree entry is terminated with a
			 * new-line.  The final new-line has been made optional to be
			 * consistent with the original non-batch behaviour of mktree.
			 */
			; /* skip creating an empty tree */
		} وإلا {
			write_درخت(repo, &oid);
			puts(oid_مقصد_hex(&oid));
			fflush(stdout);
		}
		لكل (صحيح i = 0; i < استفاده_شده_2; i++)
			حرر(entries[i]);
		استفاده_شده_2=0; /* reset tree entry buffer for re-use in batch mode */
	}
	حرر(entries);
	strbuf_release(&sb);

	أرجع 0;
}
