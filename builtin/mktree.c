/*
 * GIT - the stupid content tracker
 *
 * Copyright (c) Junio C Hamano, 2006, 2009
 */
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"

ਸਥਿਰ ਬਣਤਰ treeent {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ;
	ਬਣਤਰ object_id oid;
	ਪੂਰਨ len;
	ਅੱਖਰ name[FLEX_ARRAY];
} **entries;
ਸਥਿਰ ਪੂਰਨ alloc, ਵਰਤਿਆ_2;

ਸਥਿਰ ਖਾਲੀ append_ਮੰਜ਼ਿਲ_ਰੁੱਖ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ, ਬਣਤਰ object_id *oid, ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਣਤਰ treeent *ent;
	ਮਾਪ_t len = ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ);
	ਜੇ (strchr(ਰਸਤਾ, '/'))
		die("path %s contains slash", ਰਸਤਾ);

	FLEX_ALLOC_MEM(ent, name, ਰਸਤਾ, len);
	ent->ਬਹੁਲਕ = ਬਹੁਲਕ;
	ent->len = len;
	oidcpy(&ent->oid, oid);

	ALLOC_GROW(entries, ਵਰਤਿਆ_2 + 1, alloc);
	entries[ਵਰਤਿਆ_2++] = ent;
}

ਸਥਿਰ ਪੂਰਨ ent_compare(ਅਚਲ ਖਾਲੀ *a_, ਅਚਲ ਖਾਲੀ *b_)
{
	ਬਣਤਰ treeent *a = *(ਬਣਤਰ treeent **)a_;
	ਬਣਤਰ treeent *b = *(ਬਣਤਰ treeent **)b_;
	ਵਾਪਸ ਅਧਾਰ_name_compare(a->name, a->len, a->ਬਹੁਲਕ,
				 b->name, b->len, b->ਬਹੁਲਕ);
}

ਸਥਿਰ ਖਾਲੀ write_ਰੁੱਖ(ਬਣਤਰ repository *repo, ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ strbuf buf;
	ਮਾਪ_t ਮਾਪ;
	ਪੂਰਨ i;

	QSORT(entries, ਵਰਤਿਆ_2, ent_compare);
	ਲਈ (ਮਾਪ = i = 0; i < ਵਰਤਿਆ_2; i++)
		ਮਾਪ += 32 + entries[i]->len;

	strbuf_init(&buf, ਮਾਪ);
	ਲਈ (i = 0; i < ਵਰਤਿਆ_2; i++) {
		ਬਣਤਰ treeent *ent = entries[i];
		strbuf_addf(&buf, "%o %s%c", ent->ਬਹੁਲਕ, ent->name, '\0');
		strbuf_ਜੋੜੋ(&buf, ent->oid.hash, repo->hash_algo->rawsz);
	}

	odb_write_object(repo->objects, buf.buf, buf.len, OBJ_ਰੁੱਖ, oid);
	strbuf_release(&buf);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ mktree_usage[] = {
	"git mktree [-z] [--missing] [--batch]",
	ਨਿਰਾ
};

ਸਥਿਰ ਖਾਲੀ mktree_ਸਤਰ(ਬਣਤਰ repository *repo, ਅੱਖਰ *buf, ਪੂਰਨ nul_ਪਦ_ਸਤਰ, ਪੂਰਨ allow_missing)
{
	ਅੱਖਰ *ptr, *ntr;
	ਅਚਲ ਅੱਖਰ *p;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ;
	ਗਿਣਤੀ object_type ਬਹੁਲਕ_type; /* object type derived from mode */
	ਗਿਣਤੀ object_type obj_type; /* object type derived from sha */
	ਬਣਤਰ object_info oi = OBJECT_INFO_INIT;
	ਅੱਖਰ *ਰਸਤਾ, *ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਬਣਤਰ object_id oid;

	ptr = buf;
	/*
	 * Read non-recursive ls-tree output format:
	 *     mode SP type SP sha1 TAB name
	 */
	ਬਹੁਲਕ = strtoul(ptr, &ntr, 8);
	ਜੇ (ptr == ntr || !ntr || *ntr != ' ')
		die("input format error: %s", buf);
	ptr = ntr + 1; /* type */
	ntr = strchr(ptr, ' ');
	ਜੇ (!ntr || parse_oid_hex_algop(ntr + 1, &oid, &p, repo->hash_algo) ||
	    *p != '\t')
		die("input format error: %s", buf);

	/* It is perfectly normal if we do not have a commit from a submodule */
	ਜੇ (S_ISGITLINK(ਬਹੁਲਕ))
		allow_missing = 1;


	*ntr++ = 0; /* now at the beginning of SHA1 */

	ਰਸਤਾ = (ਅੱਖਰ *)p + 1;  /* at the beginning of name */
	ਜੇ (!nul_ਪਦ_ਸਤਰ && ਰਸਤਾ[0] == '"') {
		ਬਣਤਰ strbuf p_uq = STRBUF_INIT;
		ਜੇ (unquote_c_style(&p_uq, ਰਸਤਾ, ਨਿਰਾ))
			die("invalid quoting");
		ਰਸਤਾ = ਮੰਜ਼ਿਲ_free = strbuf_detach(&p_uq, ਨਿਰਾ);
	}

	/*
	 * Object type is redundantly derivable three ways.
	 * These should all agree.
	 */
	ਬਹੁਲਕ_type = object_type(ਬਹੁਲਕ);
	ਜੇ (ਬਹੁਲਕ_type != type_from_string(ptr)) {
		die("entry '%s' object type (%s) doesn't match mode type (%s)",
			ਰਸਤਾ, ptr, type_name(ਬਹੁਲਕ_type));
	}

	/* Check the type of object identified by oid without fetching objects */
	oi.typep = &obj_type;
	ਜੇ (odb_read_object_info_extended(repo->objects, &oid, &oi,
					  OBJECT_INFO_LOOKUP_REPLACE |
					  OBJECT_INFO_SKIP_FETCH_OBJECT) < 0)
		obj_type = -1;

	ਜੇ (obj_type < 0) {
		ਜੇ (allow_missing) {
			; /* no problem - missing objects are presumed to be of the right type */
		} ਨਹੀਂਤਾਂ {
			die("entry '%s' object %s is unavailable", ਰਸਤਾ, oid_ਮੰਜ਼ਿਲ_hex(&oid));
		}
	} ਨਹੀਂਤਾਂ {
		ਜੇ (obj_type != ਬਹੁਲਕ_type) {
			/*
			 * The object exists but is of the wrong type.
			 * This is a problem regardless of allow_missing
			 * because the new tree entry will never be correct.
			 */
			die("entry '%s' object %s is a %s but specified type was (%s)",
				ਰਸਤਾ, oid_ਮੰਜ਼ਿਲ_hex(&oid), type_name(obj_type), type_name(ਬਹੁਲਕ_type));
		}
	}

	append_ਮੰਜ਼ਿਲ_ਰੁੱਖ(ਬਹੁਲਕ, &oid, ਰਸਤਾ);
	ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
}

ਪੂਰਨ cmd_mktree(ਪੂਰਨ ac,
	       ਅਚਲ ਅੱਖਰ **av,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਬਣਤਰ object_id oid;
	ਪੂਰਨ nul_ਪਦ_ਸਤਰ = 0;
	ਪੂਰਨ allow_missing = 0;
	ਪੂਰਨ is_batch_ਬਹੁਲਕ = 0;
	ਪੂਰਨ got_eof = 0;
	strbuf_getline_fn getline_fn;

	ਅਚਲ ਬਣਤਰ option option[] = {
		OPT_BOOL('z', ਨਿਰਾ, &nul_ਪਦ_ਸਤਰ, N_("input is NUL terminated")),
		OPT_ਸੈੱਟ_INT( 0 , "missing", &allow_missing, N_("allow missing objects"), 1),
		OPT_ਸੈੱਟ_INT( 0 , "batch", &is_batch_ਬਹੁਲਕ, N_("allow creation of more than one tree"), 1),
		OPT_ਸਿਰਾ()
	};

	ac = parse_options(ac, av, ਅਗੇਤਰ, option, mktree_usage, 0);
	getline_fn = nul_ਪਦ_ਸਤਰ ? strbuf_getline_nul : strbuf_getline_lf;

	ਜਦੋਂ (!got_eof) {
		ਜਦੋਂ (1) {
			ਜੇ (getline_fn(&sb, stdin) == EOF) {
				got_eof = 1;
				ਰੋਕੋ;
			}
			ਜੇ (sb.buf[0] == '\0') {
				/* empty lines denote tree boundaries in batch mode */
				ਜੇ (is_batch_ਬਹੁਲਕ)
					ਰੋਕੋ;
				die("input format error: (blank line only valid in batch mode)");
			}
			mktree_ਸਤਰ(repo, sb.buf, nul_ਪਦ_ਸਤਰ, allow_missing);
		}
		ਜੇ (is_batch_ਬਹੁਲਕ && got_eof && ਵਰਤਿਆ_2 < 1) {
			/*
			 * Execution gets here if the last tree entry is terminated with a
			 * new-line.  The final new-line has been made optional to be
			 * consistent with the original non-batch behaviour of mktree.
			 */
			; /* skip creating an empty tree */
		} ਨਹੀਂਤਾਂ {
			write_ਰੁੱਖ(repo, &oid);
			puts(oid_ਮੰਜ਼ਿਲ_hex(&oid));
			fflush(stdout);
		}
		ਲਈ (ਪੂਰਨ i = 0; i < ਵਰਤਿਆ_2; i++)
			ਮੁਕਤ(entries[i]);
		ਵਰਤਿਆ_2=0; /* reset tree entry buffer for re-use in batch mode */
	}
	ਮੁਕਤ(entries);
	strbuf_release(&sb);

	ਵਾਪਸ 0;
}
