/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "convert.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "resolve-undo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pathspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"


ਸਥਿਰ ਪੂਰਨ abbrev;
ਸਥਿਰ ਪੂਰਨ show_deleted;
ਸਥਿਰ ਪੂਰਨ show_cached;
ਸਥਿਰ ਪੂਰਨ show_others;
ਸਥਿਰ ਪੂਰਨ show_stage;
ਸਥਿਰ ਪੂਰਨ show_unmerged;
ਸਥਿਰ ਪੂਰਨ show_resolve_undo;
ਸਥਿਰ ਪੂਰਨ show_modified;
ਸਥਿਰ ਪੂਰਨ show_killed;
ਸਥਿਰ ਪੂਰਨ show_valid_ਦੋਆਧਾਰੀ_ਅੰਕ;
ਸਥਿਰ ਪੂਰਨ show_fsmonitor_ਦੋਆਧਾਰੀ_ਅੰਕ;
ਸਥਿਰ ਪੂਰਨ ਸਤਰ_terminator = '\n';
ਸਥਿਰ ਪੂਰਨ debug_ਬਹੁਲਕ;
ਸਥਿਰ ਪੂਰਨ show_eol;
ਸਥਿਰ ਪੂਰਨ recurse_submodules;
ਸਥਿਰ ਪੂਰਨ skipping_duplicates;
ਸਥਿਰ ਪੂਰਨ show_sparse_dirs;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
ਸਥਿਰ ਪੂਰਨ max_ਅਗੇਤਰ_len;
ਸਥਿਰ ਪੂਰਨ ਅਗੇਤਰ_len;
ਸਥਿਰ ਬਣਤਰ pathspec pathspec;
ਸਥਿਰ ਪੂਰਨ error_unmatch;
ਸਥਿਰ ਅੱਖਰ *ps_matched;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *with_ਰੁੱਖ;
ਸਥਿਰ ਪੂਰਨ exc_given;
ਸਥਿਰ ਪੂਰਨ exclude_ਦਲੀਲਾਂ;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *format;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਟੈਗ_cached = "";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਟੈਗ_unmerged = "";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਟੈਗ_removed = "";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਟੈਗ_other = "";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਟੈਗ_killed = "";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਟੈਗ_modified = "";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਟੈਗ_skip_worktree = "";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਟੈਗ_resolve_undo = "";

ਸਥਿਰ ਖਾਲੀ write_eolinfo(ਬਣਤਰ ਸੂਚਕਾਂਕ_state *istate,
			  ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਜੇ (show_eol) {
		ਬਣਤਰ stat st;
		ਅਚਲ ਅੱਖਰ *i_txt = "";
		ਅਚਲ ਅੱਖਰ *w_txt = "";
		ਅਚਲ ਅੱਖਰ *a_txt = get_convert_attr_ascii(istate, ਰਸਤਾ);
		ਜੇ (ce && S_ISREG(ce->ce_ਬਹੁਲਕ))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		ਜੇ (!lstat(ਰਸਤਾ, &st) && S_ISREG(st.st_ਬਹੁਲਕ))
			w_txt = get_wt_convert_stats_ascii(ਰਸਤਾ);
		ਲਿਖੋ("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

ਸਥਿਰ ਖਾਲੀ write_name(ਅਚਲ ਅੱਖਰ *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, ਅਗੇਤਰ_len ? ਅਗੇਤਰ : ਨਿਰਾ,
				   stdout, ਸਤਰ_terminator);
}

ਸਥਿਰ ਖਾਲੀ write_name_ਮੰਜ਼ਿਲ_buf(ਬਣਤਰ strbuf *sb, ਅਚਲ ਅੱਖਰ *name)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *rel = relative_ਰਸਤਾ(name, ਅਗੇਤਰ_len ? ਅਗੇਤਰ : ਨਿਰਾ, &buf);

	ਜੇ (ਸਤਰ_terminator)
		quote_c_style(rel, sb, ਨਿਰਾ, 0);
	ਨਹੀਂਤਾਂ
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_ਟੈਗ(ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce, ਅਚਲ ਅੱਖਰ *ਟੈਗ)
{
	ਸਥਿਰ ਅੱਖਰ alttag[4];

	ਜੇ (ਟੈਗ && *ਟੈਗ && ((show_valid_ਦੋਆਧਾਰੀ_ਅੰਕ && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_ਦੋਆਧਾਰੀ_ਅੰਕ && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(alttag, ਟੈਗ, 3);

		ਜੇ (ਅੱਖਰ_ਹੈ(ਟੈਗ[0])) {
			alttag[0] = tolower(ਟੈਗ[0]);
		} ਨਹੀਂਤਾਂ ਜੇ (ਟੈਗ[0] == '?') {
			alttag[0] = '!';
		} ਨਹੀਂਤਾਂ {
			alttag[0] = 'v';
			alttag[1] = ਟੈਗ[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		ਟੈਗ = alttag;
	}

	ਵਾਪਸ ਟੈਗ;
}

ਸਥਿਰ ਖਾਲੀ print_debug(ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce)
{
	ਜੇ (debug_ਬਹੁਲਕ) {
		ਅਚਲ ਬਣਤਰ stat_ਅੰਕੜੇ *sd = &ce->ce_stat_ਅੰਕੜੇ;

		ਲਿਖੋ("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		ਲਿਖੋ("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		ਲਿਖੋ("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		ਲਿਖੋ("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		ਲਿਖੋ("  size: %u\tflags: %x\n", sd->sd_ਮਾਪ, ce->ce_flags);
	}
}

ਸਥਿਰ ਖਾਲੀ show_dir_ਇੰਦਰਾਜ(ਬਣਤਰ ਸੂਚਕਾਂਕ_state *istate,
			   ਅਚਲ ਅੱਖਰ *ਟੈਗ, ਬਣਤਰ dir_ਇੰਦਰਾਜ *ent)
{
	ਪੂਰਨ len = max_ਅਗੇਤਰ_len;

	ਜੇ (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	ਜੇ (ps_matched)
		dir_ਰਸਤਾ_ਮੇਲ(istate, ent, &pathspec, len, ps_matched);

	fputs(ਟੈਗ, stdout);
	write_eolinfo(istate, ਨਿਰਾ, ent->name);
	write_name(ent->name);
}

ਸਥਿਰ ਖਾਲੀ show_other_files(ਬਣਤਰ ਸੂਚਕਾਂਕ_state *istate,
			     ਅਚਲ ਬਣਤਰ dir_struct *dir)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < dir->nr; i++) {
		ਬਣਤਰ dir_ਇੰਦਰਾਜ *ent = dir->entries[i];
		ਜੇ (!ਸੂਚਕਾਂਕ_name_is_other(istate, ent->name, ent->len))
			ਜਾਰੀ;
		show_dir_ਇੰਦਰਾਜ(istate, ਟੈਗ_other, ent);
	}
}

ਸਥਿਰ ਖਾਲੀ show_killed_files(ਬਣਤਰ ਸੂਚਕਾਂਕ_state *istate,
			      ਅਚਲ ਬਣਤਰ dir_struct *dir)
{
	ਪੂਰਨ i;
	ਲਈ (i = 0; i < dir->nr; i++) {
		ਬਣਤਰ dir_ਇੰਦਰਾਜ *ent = dir->entries[i];
		ਅੱਖਰ *cp, *sp;
		ਪੂਰਨ pos, len, killed = 0;

		ਲਈ (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			ਜੇ (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = ਸੂਚਕਾਂਕ_name_pos(istate, ent->name, ent->len);
				ਜੇ (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				ਜਦੋਂ (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				ਜੇ (istate->cache_nr <= pos)
					ਰੋਕੋ;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				ਜੇ ((ent->len < len) &&
				    !ਲੜੀ_ਤੁਲਨਾ(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				ਰੋਕੋ;
			}
			ਜੇ (0 <= ਸੂਚਕਾਂਕ_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				ਰੋਕੋ;
			}
		}
		ਜੇ (killed)
			show_dir_ਇੰਦਰਾਜ(istate, ਟੈਗ_killed, dir->entries[i]);
	}
}

ਸਥਿਰ ਖਾਲੀ show_files(ਬਣਤਰ repository *repo, ਬਣਤਰ dir_struct *dir);

ਸਥਿਰ ਖਾਲੀ show_submodule(ਬਣਤਰ repository *superproject,
			   ਬਣਤਰ dir_struct *dir, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਣਤਰ repository subrepo;

	ਜੇ (repo_submodule_init(&subrepo, superproject, ਰਸਤਾ,
				null_oid(superproject->hash_algo)))
		ਵਾਪਸ;

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

ਸਥਿਰ ਖਾਲੀ expand_objectsize(ਬਣਤਰ repository *repo, ਬਣਤਰ strbuf *ਸਤਰ,
			      ਅਚਲ ਬਣਤਰ object_id *oid,
			      ਅਚਲ ਗਿਣਤੀ object_type type, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ padded)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ padding[] = "       ";
	ਮਾਪ_t min_len = padded ? ਲੜੀ_ਲੰਬਾਈ(padding) : 0;
	ਮਾਪ_t orig_len = ਸਤਰ->len;
	ਮਾਪ_t len;

	ਜੇ (type == OBJ_BLOB) {
		ਮਾਪ_t ਮਾਪ;
		ਜੇ (odb_read_object_info(repo->objects, oid, &ਮਾਪ) < 0)
			die(_("could not get object info about '%s'"),
			    oid_ਮੰਜ਼ਿਲ_hex(oid));
		strbuf_ਜੋੜੋ_uint(ਸਤਰ, ਮਾਪ);
	} ਨਹੀਂਤਾਂ {
		strbuf_addstr(ਸਤਰ, "-");
	}
	len = ਸਤਰ->len - orig_len;
	ਜੇ (len < min_len)
		strbuf_ਪਾਓ(ਸਤਰ, orig_len, padding, min_len - len);
}

ਸਥਿਰ ਖਾਲੀ show_ce_fmt(ਬਣਤਰ repository *repo, ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce,
			ਅਚਲ ਅੱਖਰ *format, ਅਚਲ ਅੱਖਰ *fullname) {
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	ਜਦੋਂ (strbuf_expand_ਕਦਮ(&sb, &format)) {
		ਮਾਪ_t len;
		ਬਣਤਰ stat st;

		ਜੇ (skip_ਅਗੇਤਰ(format, "%", &format))
			strbuf_addch(&sb, '%');
		ਨਹੀਂਤਾਂ ਜੇ ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_ਬਹੁਲਕ);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(objectname)", &format))
			strbuf_ਜੋੜੋ_unique_abbrev(&sb, &ce->oid, abbrev);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_ਬਹੁਲਕ)));
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_ਬਹੁਲਕ), 1);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_ਬਹੁਲਕ), 0);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_ਬਹੁਲਕ) ?
				      get_cached_convert_stats_ascii(repo->ਸੂਚਕਾਂਕ,
				      ce->name) : "");
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_ਬਹੁਲਕ) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->ਸੂਚਕਾਂਕ,
								 fullname));
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(format, "(path)", &format))
			write_name_ਮੰਜ਼ਿਲ_buf(&sb, fullname);
		ਨਹੀਂਤਾਂ
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, ਸਤਰ_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

ਸਥਿਰ ਖਾਲੀ show_ce(ਬਣਤਰ repository *repo, ਬਣਤਰ dir_struct *dir,
		    ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce, ਅਚਲ ਅੱਖਰ *fullname,
		    ਅਚਲ ਅੱਖਰ *ਟੈਗ)
{
	ਜੇ (max_ਅਗੇਤਰ_len > ਲੜੀ_ਲੰਬਾਈ(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	ਜੇ (recurse_submodules && S_ISGITLINK(ce->ce_ਬਹੁਲਕ) &&
	    is_submodule_ਸਰਗਰਮ(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} ਨਹੀਂਤਾਂ ਜੇ (ਮੇਲ_pathspec(repo->ਸੂਚਕਾਂਕ, &pathspec, fullname, ਲੜੀ_ਲੰਬਾਈ(fullname),
				  max_ਅਗੇਤਰ_len, ps_matched,
				  S_ISDIR(ce->ce_ਬਹੁਲਕ) ||
				  S_ISGITLINK(ce->ce_ਬਹੁਲਕ))) {
		ਜੇ (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			ਵਾਪਸ;
		}

		ਟੈਗ = get_ਟੈਗ(ce, ਟੈਗ);

		ਜੇ (!show_stage) {
			fputs(ਟੈਗ, stdout);
		} ਨਹੀਂਤਾਂ {
			ਲਿਖੋ("%s%06o %s %d\t",
			       ਟੈਗ,
			       ce->ce_ਬਹੁਲਕ,
			       repo_ਲੱਭੋ_unique_abbrev(repo, &ce->oid, abbrev),
			       ce_stage(ce));
		}
		write_eolinfo(repo->ਸੂਚਕਾਂਕ, ce, fullname);
		write_name(fullname);
		print_debug(ce);
	}
}

ਸਥਿਰ ਖਾਲੀ show_ru_info(ਬਣਤਰ repository *repo, ਬਣਤਰ ਸੂਚਕਾਂਕ_state *istate)
{
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

	ਜੇ (!istate->resolve_undo)
		ਵਾਪਸ;

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, istate->resolve_undo) {
		ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = ਵਸਤੂ->string;
		ਬਣਤਰ resolve_undo_info *ui = ਵਸਤੂ->util;
		ਪੂਰਨ i, len;

		len = ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ);
		ਜੇ (len < max_ਅਗੇਤਰ_len)
			ਜਾਰੀ; /* outside of the prefix */
		ਜੇ (!ਮੇਲ_pathspec(istate, &pathspec, ਰਸਤਾ, len,
				    max_ਅਗੇਤਰ_len, ps_matched, 0))
			ਜਾਰੀ; /* uninterested */
		ਲਈ (i = 0; i < 3; i++) {
			ਜੇ (!ui->ਬਹੁਲਕ[i])
				ਜਾਰੀ;
			ਲਿਖੋ("%s%06o %s %d\t", ਟੈਗ_resolve_undo, ui->ਬਹੁਲਕ[i],
			       repo_ਲੱਭੋ_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(ਰਸਤਾ);
		}
	}
}

ਸਥਿਰ ਪੂਰਨ ce_excluded(ਬਣਤਰ dir_struct *dir, ਬਣਤਰ ਸੂਚਕਾਂਕ_state *istate,
		       ਅਚਲ ਅੱਖਰ *fullname, ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce)
{
	ਪੂਰਨ dtype = ce_ਮੰਜ਼ਿਲ_dtype(ce);
	ਵਾਪਸ is_excluded(dir, istate, fullname, &dtype);
}

ਸਥਿਰ ਖਾਲੀ construct_fullname(ਬਣਤਰ strbuf *ਬਾਹਰਲਾ, ਅਚਲ ਬਣਤਰ repository *repo,
			       ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce)
{
	strbuf_reset(ਬਾਹਰਲਾ);
	ਜੇ (repo->submodule_ਅਗੇਤਰ)
		strbuf_addstr(ਬਾਹਰਲਾ, repo->submodule_ਅਗੇਤਰ);
	strbuf_addstr(ਬਾਹਰਲਾ, ce->name);
}

ਸਥਿਰ ਖਾਲੀ show_files(ਬਣਤਰ repository *repo, ਬਣਤਰ dir_struct *dir)
{
	ਪੂਰਨ i;
	ਬਣਤਰ strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	ਜੇ (show_others || show_killed) {
		ਜੇ (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->ਸੂਚਕਾਂਕ, &pathspec);
		ਜੇ (show_others)
			show_other_files(repo->ਸੂਚਕਾਂਕ, dir);
		ਜੇ (show_killed)
			show_killed_files(repo->ਸੂਚਕਾਂਕ, dir);
	}

	ਜੇ (!(show_cached || show_stage || show_deleted || show_modified))
		ਵਾਪਸ;

	ਲਈ (i = 0; i < repo->ਸੂਚਕਾਂਕ->cache_nr; i++) {
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = repo->ਸੂਚਕਾਂਕ->cache[i];
		ਬਣਤਰ stat st;
		ਪੂਰਨ stat_err;

		ਜੇ (S_ISSPARSEDIR(ce->ce_ਬਹੁਲਕ) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(repo->ਸੂਚਕਾਂਕ);
			ce = repo->ਸੂਚਕਾਂਕ->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		ਜੇ ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->ਸੂਚਕਾਂਕ, fullname.buf, ce))
			ਜਾਰੀ;
		ਜੇ (ce->ce_flags & CE_UPDATE)
			ਜਾਰੀ;
		ਜੇ ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? ਟੈਗ_unmerged :
				(ce_skip_worktree(ce) ? ਟੈਗ_skip_worktree :
				 ਟੈਗ_cached));
			ਜੇ (skipping_duplicates)
				ਜਾਓ skip_ਮੰਜ਼ਿਲ_ਅਗਲਾ_name;
		}

		ਜੇ (!(show_deleted || show_modified))
			ਜਾਰੀ;
		ਜੇ (ce_skip_worktree(ce))
			ਜਾਰੀ;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		ਜੇ (pathspec.nr == 1 &&
		    !ਮੇਲ_pathspec(repo->ਸੂਚਕਾਂਕ, &pathspec, fullname.buf,
				    fullname.len, max_ਅਗੇਤਰ_len, ਨਿਰਾ,
				    S_ISDIR(ce->ce_ਬਹੁਲਕ) ||
				    S_ISGITLINK(ce->ce_ਬਹੁਲਕ)))
			ਜਾਰੀ;
		stat_err = lstat(fullname.buf, &st);
		ਜੇ (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		ਜੇ (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, ਟੈਗ_removed);
			ਜੇ (skipping_duplicates)
				ਜਾਓ skip_ਮੰਜ਼ਿਲ_ਅਗਲਾ_name;
		}
		ਜੇ (show_modified &&
		    (stat_err || ie_modified(repo->ਸੂਚਕਾਂਕ, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, ਟੈਗ_modified);
			ਜੇ (skipping_duplicates)
				ਜਾਓ skip_ਮੰਜ਼ਿਲ_ਅਗਲਾ_name;
		}
		ਜਾਰੀ;

skip_ਮੰਜ਼ਿਲ_ਅਗਲਾ_name:
		{
			ਪੂਰਨ j;
			ਬਣਤਰ cache_ਇੰਦਰਾਜ **cache = repo->ਸੂਚਕਾਂਕ->cache;
			ਲਈ (j = i + 1; j < repo->ਸੂਚਕਾਂਕ->cache_nr; j++)
				ਜੇ (strcmp(ce->name, cache[j]->name))
					ਰੋਕੋ;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
ਸਥਿਰ ਖਾਲੀ prune_ਸੂਚਕਾਂਕ(ਬਣਤਰ ਸੂਚਕਾਂਕ_state *istate,
			ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਮਾਪ_t prefixlen)
{
	ਪੂਰਨ pos;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪਹਿਲਾ, ਆਖਰੀ;

	ਜੇ (!ਅਗੇਤਰ || !istate->cache_nr)
		ਵਾਪਸ;
	pos = ਸੂਚਕਾਂਕ_name_pos(istate, ਅਗੇਤਰ, prefixlen);
	ਜੇ (pos < 0)
		pos = -pos-1;
	ਪਹਿਲਾ = pos;
	ਆਖਰੀ = istate->cache_nr;
	ਜਦੋਂ (ਆਖਰੀ > ਪਹਿਲਾ) {
		ਪੂਰਨ ਅਗਲਾ = ਪਹਿਲਾ + ((ਆਖਰੀ - ਪਹਿਲਾ) >> 1);
		ਅਚਲ ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = istate->cache[ਅਗਲਾ];
		ਜੇ (!ਲੜੀ_ਤੁਲਨਾ(ce->name, ਅਗੇਤਰ, prefixlen)) {
			ਪਹਿਲਾ = ਅਗਲਾ+1;
			ਜਾਰੀ;
		}
		ਆਖਰੀ = ਅਗਲਾ;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, ਆਖਰੀ - pos);
	istate->cache_nr = ਆਖਰੀ - pos;
}

ਸਥਿਰ ਪੂਰਨ get_common_ਅਗੇਤਰ_len(ਅਚਲ ਅੱਖਰ *common_ਅਗੇਤਰ)
{
	ਪੂਰਨ common_ਅਗੇਤਰ_len;

	ਜੇ (!common_ਅਗੇਤਰ)
		ਵਾਪਸ 0;

	common_ਅਗੇਤਰ_len = ਲੜੀ_ਲੰਬਾਈ(common_ਅਗੇਤਰ);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	ਜੇ (common_ਅਗੇਤਰ[common_ਅਗੇਤਰ_len - 1] == '/')
		common_ਅਗੇਤਰ_len--;

	ਵਾਪਸ common_ਅਗੇਤਰ_len;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ option_parse_exclude(ਅਚਲ ਬਣਤਰ option *opt,
				ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ string_list *exclude_list = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_exclude_from(ਅਚਲ ਬਣਤਰ option *opt,
				     ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ dir_struct *dir = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	ਜੋੜੋ_patterns_from_file(dir, arg);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ option_parse_exclude_standard(ਅਚਲ ਬਣਤਰ option *opt,
					 ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ dir_struct *dir = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	ਤਿਆਰੀ_standard_excludes(dir);

	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_ls_files(ਪੂਰਨ argc,
		 ਅਚਲ ਅੱਖਰ **argv,
		 ਅਚਲ ਅੱਖਰ *cmd_ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ require_work_ਰੁੱਖ = 0, show_ਟੈਗ = 0, i;
	ਅੱਖਰ *max_ਅਗੇਤਰ;
	ਬਣਤਰ dir_struct dir = DIR_INIT;
	ਬਣਤਰ ਨਮੂਨਾ_list *pl;
	ਬਣਤਰ string_list exclude_list = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_ਸੈੱਟ_INT('z', ਨਿਰਾ, &ਸਤਰ_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', ਨਿਰਾ, &show_ਟੈਗ,
			N_("identify the file status with tags")),
		OPT_BOOL('v', ਨਿਰਾ, &show_valid_ਦੋਆਧਾਰੀ_ਅੰਕ,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', ਨਿਰਾ, &show_fsmonitor_ਦੋਆਧਾਰੀ_ਅੰਕ,
			N_("use lowercase letters for 'fsmonitor clean' files")),
		OPT_BOOL('c', "cached", &show_cached,
			N_("show cached files in the output (default)")),
		OPT_BOOL('d', "deleted", &show_deleted,
			N_("show deleted files in the output")),
		OPT_BOOL('m', "modified", &show_modified,
			N_("show modified files in the output")),
		OPT_BOOL('o', "others", &show_others,
			N_("show other files in the output")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('i', "ignored", &dir.flags,
			N_("show ignored files in the output"),
			DIR_SHOW_IGNORED),
		OPT_BOOL('s', "stage", &show_stage,
			N_("show staged contents' object name in the output")),
		OPT_BOOL('k', "killed", &show_killed,
			N_("show files on the filesystem that need to be removed")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "directory", &dir.flags,
			N_("show 'other' directories' names only"),
			DIR_SHOW_OTHER_DIRECTORIES),
		OPT_BOOL(0, "eol", &show_eol, N_("show line endings of files")),
		OPT_NEGBIT(0, "empty-directory", &dir.flags,
			N_("don't show empty directories"),
			DIR_HIDE_ਖਾਲੀ_ਥਾਵਾਂ_DIRECTORIES),
		OPT_BOOL('u', "unmerged", &show_unmerged,
			N_("show unmerged files in the output")),
		OPT_BOOL(0, "resolve-undo", &show_resolve_undo,
			    N_("show resolve-undo information")),
		OPT_CALLBACK_F('x', "exclude", &exclude_list, N_("pattern"),
			N_("skip files matching pattern"),
			PARSE_OPT_NONEG, option_parse_exclude),
		OPT_CALLBACK_F('X', "exclude-from", &dir, N_("file"),
			N_("read exclude patterns from <file>"),
			PARSE_OPT_NONEG, option_parse_exclude_from),
		OPT_STRING(0, "exclude-per-directory", &dir.exclude_ਪ੍ਰਤੀ_dir, N_("file"),
			N_("read additional per-directory exclude patterns in <file>")),
		OPT_CALLBACK_F(0, "exclude-standard", &dir, ਨਿਰਾ,
			N_("add the standard git exclusions"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			option_parse_exclude_standard),
		OPT_ਸੈੱਟ_INT_F(0, "full-name", &ਅਗੇਤਰ_len,
			      N_("make the output relative to the project top directory"),
			      0, PARSE_OPT_NONEG),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			N_("recurse through submodules")),
		OPT_BOOL(0, "error-unmatch", &error_unmatch,
			N_("if any <file> is not in the index, treat this as an error")),
		OPT_STRING(0, "with-tree", &with_ਰੁੱਖ, N_("tree-ish"),
			N_("pretend that paths removed since <tree-ish> are still present")),
		OPT__ABBREV(&abbrev),
		OPT_BOOL(0, "debug", &debug_ਬਹੁਲਕ, N_("show debugging data")),
		OPT_BOOL(0, "deduplicate", &skipping_duplicates,
			 N_("suppress duplicate entries")),
		OPT_BOOL(0, "sparse", &show_sparse_dirs,
			 N_("show sparse directories in the presence of a sparse index")),
		OPT_STRING_F(0, "format", &format, N_("format"),
			     N_("format to use for the output"),
			     PARSE_OPT_NONEG),
		OPT_ਸਿਰਾ()
	};
	ਪੂਰਨ ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਅਗੇਤਰ = cmd_ਅਗੇਤਰ;
	ਜੇ (ਅਗੇਤਰ)
		ਅਗੇਤਰ_len = ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ);
	repo_config(repo, git_default_config, ਨਿਰਾ);

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = ਜੋੜੋ_ਨਮੂਨਾ_list(&dir, EXC_CMDL, "--exclude option");
	ਲਈ (i = 0; i < exclude_list.nr; i++) {
		ਜੋੜੋ_ਨਮੂਨਾ(exclude_list.ਚੀਜ਼ਾਂ[i].string, "", 0, pl, --exclude_ਦਲੀਲਾਂ);
	}

	ਜੇ (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_ਟੈਗ))
			usage_ਸੁਨੇਹਾ_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	ਜੇ (show_ਟੈਗ || show_valid_ਦੋਆਧਾਰੀ_ਅੰਕ || show_fsmonitor_ਦੋਆਧਾਰੀ_ਅੰਕ) {
		ਟੈਗ_cached = "H ";
		ਟੈਗ_unmerged = "M ";
		ਟੈਗ_removed = "R ";
		ਟੈਗ_modified = "C ";
		ਟੈਗ_other = "? ";
		ਟੈਗ_killed = "K ";
		ਟੈਗ_skip_worktree = "S ";
		ਟੈਗ_resolve_undo = "U ";
	}
	ਜੇ (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_ਰੁੱਖ = 1;
	ਜੇ (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	ਜੇ (show_ਟੈਗ || show_stage)
		skipping_duplicates = 0;
	ਜੇ (dir.exclude_ਪ੍ਰਤੀ_dir)
		exc_given = 1;

	ਜੇ (require_work_ਰੁੱਖ && !is_inside_work_ਰੁੱਖ(repo))
		ਤਿਆਰੀ_work_ਰੁੱਖ(repo);

	ਜੇ (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_ਰੁੱਖ))
		die("ls-files --recurse-submodules unsupported mode");

	ਜੇ (recurse_submodules && error_unmatch)
		die("ls-files --recurse-submodules does not support "
		    "--error-unmatch");

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       ਅਗੇਤਰ, argv);

	/*
	 * Find common prefix for all pathspec's
	 * This is used as a performance optimization which unfortunately cannot
	 * be done when recursing into submodules because when a pathspec is
	 * given which spans repository boundaries you can't simply remove the
	 * submodule entry because the pathspec may match something inside the
	 * submodule.
	 */
	ਜੇ (recurse_submodules)
		max_ਅਗੇਤਰ = ਨਿਰਾ;
	ਨਹੀਂਤਾਂ
		max_ਅਗੇਤਰ = common_ਅਗੇਤਰ(&pathspec);
	max_ਅਗੇਤਰ_len = get_common_ਅਗੇਤਰ_len(max_ਅਗੇਤਰ);

	prune_ਸੂਚਕਾਂਕ(repo->ਸੂਚਕਾਂਕ, max_ਅਗੇਤਰ, max_ਅਗੇਤਰ_len);

	/* Treat unmatching pathspec elements as errors */
	ਜੇ (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	ਜੇ ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	ਜੇ ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	ਜੇ (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	ਜੇ (with_ਰੁੱਖ) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		ਜੇ (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_ਰੁੱਖ_on_ਸੂਚਕਾਂਕ(repo->ਸੂਚਕਾਂਕ, with_ਰੁੱਖ, max_ਅਗੇਤਰ);
	}

	show_files(repo, &dir);

	ਜੇ (show_resolve_undo)
		show_ru_info(repo, repo->ਸੂਚਕਾਂਕ);

	ਜੇ (ps_matched && report_ਰਸਤਾ_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	ਮੁਕਤ(max_ਅਗੇਤਰ);
	ਵਾਪਸ ret;
}
