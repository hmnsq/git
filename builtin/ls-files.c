/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "convert.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "resolve-undo.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"
#ಅನ್ತರ್ಭಾವ "read-cache.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "hex.h"


ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ abbrev;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_deleted;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_cached;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_others;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_stage;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_unmerged;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_resolve_undo;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_modified;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_killed;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_valid_ಬಿಟ್;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_fsmonitor_ಬಿಟ್;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಾಲು_terminator = '\n';
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ debug_ಬಹುಲಕ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_eol;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ recurse_submodules;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ skipping_duplicates;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_sparse_dirs;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ max_ಪೂರ್ವಪ್ರತ್ಯಯ_len;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_len;
ಸ್ಥಿರ ರಚನೆ pathspec pathspec;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ error_unmatch;
ಸ್ಥಿರ ಅಕ್ಷರ *ps_matched;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *with_ವೃಕ್ಷ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ exc_given;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ exclude_ವಾದಗಳು;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್_cached = "";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್_unmerged = "";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್_removed = "";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್_other = "";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್_killed = "";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್_modified = "";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್_skip_worktree = "";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್_resolve_undo = "";

ಸ್ಥಿರ ಶೂನ್ಯ write_eolinfo(ರಚನೆ ಸೂಚ್ಯಂಕ_state *istate,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಆದರೆ (show_eol) {
		ರಚನೆ stat st;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *i_txt = "";
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *w_txt = "";
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a_txt = get_convert_attr_ascii(istate, ಮಾರ್ಗ);
		ಆದರೆ (ce && S_ISREG(ce->ce_ಬಹುಲಕ))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		ಆದರೆ (!lstat(ಮಾರ್ಗ, &st) && S_ISREG(st.st_ಬಹುಲಕ))
			w_txt = get_wt_convert_stats_ascii(ಮಾರ್ಗ);
		ಬರೆ("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ write_name(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, ಪೂರ್ವಪ್ರತ್ಯಯ_len ? ಪೂರ್ವಪ್ರತ್ಯಯ : ಖಾಲಿ,
				   stdout, ಸಾಲು_terminator);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_name_ಗಮ್ಯ_buf(ರಚನೆ strbuf *sb, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel = relative_ಮಾರ್ಗ(name, ಪೂರ್ವಪ್ರತ್ಯಯ_len ? ಪೂರ್ವಪ್ರತ್ಯಯ : ಖಾಲಿ, &buf);

	ಆದರೆ (ಸಾಲು_terminator)
		quote_c_style(rel, sb, ಖಾಲಿ, 0);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_ಟ್ಯಾಗ್(ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್)
{
	ಸ್ಥಿರ ಅಕ್ಷರ alttag[4];

	ಆದರೆ (ಟ್ಯಾಗ್ && *ಟ್ಯಾಗ್ && ((show_valid_ಬಿಟ್ && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_ಬಿಟ್ && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		ಸ್ಮರಣೆ_ನಕಲು(alttag, ಟ್ಯಾಗ್, 3);

		ಆದರೆ (ಅಕ್ಷರವೇ(ಟ್ಯಾಗ್[0])) {
			alttag[0] = tolower(ಟ್ಯಾಗ್[0]);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಟ್ಯಾಗ್[0] == '?') {
			alttag[0] = '!';
		} ಇಲ್ಲದಿದ್ದರೆ {
			alttag[0] = 'v';
			alttag[1] = ಟ್ಯಾಗ್[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		ಟ್ಯಾಗ್ = alttag;
	}

	ಹಿಂತಿರುಗಿ ಟ್ಯಾಗ್;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_debug(ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce)
{
	ಆದರೆ (debug_ಬಹುಲಕ) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ stat_ದತ್ತಾಂಶ *sd = &ce->ce_stat_ದತ್ತಾಂಶ;

		ಬರೆ("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		ಬರೆ("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		ಬರೆ("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		ಬರೆ("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		ಬರೆ("  size: %u\tflags: %x\n", sd->sd_ಗಾತ್ರ, ce->ce_flags);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_dir_ನಮೂದು(ರಚನೆ ಸೂಚ್ಯಂಕ_state *istate,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್, ರಚನೆ dir_ನಮೂದು *ent)
{
	ಪೂರ್ಣಾಂಕ len = max_ಪೂರ್ವಪ್ರತ್ಯಯ_len;

	ಆದರೆ (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	ಆದರೆ (ps_matched)
		dir_ಮಾರ್ಗ_ಹೊಂದಿಕೆ(istate, ent, &pathspec, len, ps_matched);

	fputs(ಟ್ಯಾಗ್, stdout);
	write_eolinfo(istate, ಖಾಲಿ, ent->name);
	write_name(ent->name);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_other_files(ರಚನೆ ಸೂಚ್ಯಂಕ_state *istate,
			     ಸ್ಥಿರಾಂಕ ರಚನೆ dir_struct *dir)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < dir->nr; i++) {
		ರಚನೆ dir_ನಮೂದು *ent = dir->entries[i];
		ಆದರೆ (!ಸೂಚ್ಯಂಕ_name_is_other(istate, ent->name, ent->len))
			ಮುಂದುವರಿ;
		show_dir_ನಮೂದು(istate, ಟ್ಯಾಗ್_other, ent);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_killed_files(ರಚನೆ ಸೂಚ್ಯಂಕ_state *istate,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ dir_struct *dir)
{
	ಪೂರ್ಣಾಂಕ i;
	ಗಾಗಿ (i = 0; i < dir->nr; i++) {
		ರಚನೆ dir_ನಮೂದು *ent = dir->entries[i];
		ಅಕ್ಷರ *cp, *sp;
		ಪೂರ್ಣಾಂಕ pos, len, killed = 0;

		ಗಾಗಿ (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			ಆದರೆ (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = ಸೂಚ್ಯಂಕ_name_pos(istate, ent->name, ent->len);
				ಆದರೆ (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				ಆಗ (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				ಆದರೆ (istate->cache_nr <= pos)
					ನಿಲ್ಲಿಸು;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				ಆದರೆ ((ent->len < len) &&
				    !ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				ನಿಲ್ಲಿಸು;
			}
			ಆದರೆ (0 <= ಸೂಚ್ಯಂಕ_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				ನಿಲ್ಲಿಸು;
			}
		}
		ಆದರೆ (killed)
			show_dir_ನಮೂದು(istate, ಟ್ಯಾಗ್_killed, dir->entries[i]);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_files(ರಚನೆ repository *repo, ರಚನೆ dir_struct *dir);

ಸ್ಥಿರ ಶೂನ್ಯ show_submodule(ರಚನೆ repository *superproject,
			   ರಚನೆ dir_struct *dir, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ರಚನೆ repository subrepo;

	ಆದರೆ (repo_submodule_init(&subrepo, superproject, ಮಾರ್ಗ,
				null_oid(superproject->hash_algo)))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

ಸ್ಥಿರ ಶೂನ್ಯ expand_objectsize(ರಚನೆ repository *repo, ರಚನೆ strbuf *ಸಾಲು,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
			      ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ object_type type, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ padded)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ padding[] = "       ";
	ಗಾತ್ರ_t min_len = padded ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(padding) : 0;
	ಗಾತ್ರ_t orig_len = ಸಾಲು->len;
	ಗಾತ್ರ_t len;

	ಆದರೆ (type == OBJ_BLOB) {
		ಗಾತ್ರ_t ಗಾತ್ರ_2;
		ಆದರೆ (odb_read_object_info(repo->objects, oid, &ಗಾತ್ರ_2) < 0)
			die(_("could not get object info about '%s'"),
			    oid_ಗಮ್ಯ_hex(oid));
		strbuf_ಕೂಡಿಸು_uint(ಸಾಲು, ಗಾತ್ರ_2);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addstr(ಸಾಲು, "-");
	}
	len = ಸಾಲು->len - orig_len;
	ಆದರೆ (len < min_len)
		strbuf_ಸೇರಿಸು(ಸಾಲು, orig_len, padding, min_len - len);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ce_fmt(ರಚನೆ repository *repo, ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fullname) {
	ರಚನೆ strbuf sb = STRBUF_INIT;

	ಆಗ (strbuf_expand_ಹೆಜ್ಜೆ(&sb, &format)) {
		ಗಾತ್ರ_t len;
		ರಚನೆ stat st;

		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "%", &format))
			strbuf_addch(&sb, '%');
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_ಬಹುಲಕ);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(objectname)", &format))
			strbuf_ಕೂಡಿಸು_unique_abbrev(&sb, &ce->oid, abbrev);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_ಬಹುಲಕ)));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_ಬಹುಲಕ), 1);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_ಬಹುಲಕ), 0);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_ಬಹುಲಕ) ?
				      get_cached_convert_stats_ascii(repo->ಸೂಚ್ಯಂಕ,
				      ce->name) : "");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_ಬಹುಲಕ) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->ಸೂಚ್ಯಂಕ,
								 fullname));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(format, "(path)", &format))
			write_name_ಗಮ್ಯ_buf(&sb, fullname);
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, ಸಾಲು_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ce(ರಚನೆ repository *repo, ರಚನೆ dir_struct *dir,
		    ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fullname,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್)
{
	ಆದರೆ (max_ಪೂರ್ವಪ್ರತ್ಯಯ_len > ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	ಆದರೆ (recurse_submodules && S_ISGITLINK(ce->ce_ಬಹುಲಕ) &&
	    is_submodule_ಸಕ್ರಿಯ(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಹೊಂದಿಕೆ_pathspec(repo->ಸೂಚ್ಯಂಕ, &pathspec, fullname, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(fullname),
				  max_ಪೂರ್ವಪ್ರತ್ಯಯ_len, ps_matched,
				  S_ISDIR(ce->ce_ಬಹುಲಕ) ||
				  S_ISGITLINK(ce->ce_ಬಹುಲಕ))) {
		ಆದರೆ (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			ಹಿಂತಿರುಗಿ;
		}

		ಟ್ಯಾಗ್ = get_ಟ್ಯಾಗ್(ce, ಟ್ಯಾಗ್);

		ಆದರೆ (!show_stage) {
			fputs(ಟ್ಯಾಗ್, stdout);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಬರೆ("%s%06o %s %d\t",
			       ಟ್ಯಾಗ್,
			       ce->ce_ಬಹುಲಕ,
			       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(repo, &ce->oid, abbrev),
			       ce_stage(ce));
		}
		write_eolinfo(repo->ಸೂಚ್ಯಂಕ, ce, fullname);
		write_name(fullname);
		print_debug(ce);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ru_info(ರಚನೆ repository *repo, ರಚನೆ ಸೂಚ್ಯಂಕ_state *istate)
{
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

	ಆದರೆ (!istate->resolve_undo)
		ಹಿಂತಿರುಗಿ;

	for_each_string_list_ಅಂಶ(ಅಂಶ, istate->resolve_undo) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = ಅಂಶ->string;
		ರಚನೆ resolve_undo_info *ui = ಅಂಶ->util;
		ಪೂರ್ಣಾಂಕ i, len;

		len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ);
		ಆದರೆ (len < max_ಪೂರ್ವಪ್ರತ್ಯಯ_len)
			ಮುಂದುವರಿ; /* outside of the prefix */
		ಆದರೆ (!ಹೊಂದಿಕೆ_pathspec(istate, &pathspec, ಮಾರ್ಗ, len,
				    max_ಪೂರ್ವಪ್ರತ್ಯಯ_len, ps_matched, 0))
			ಮುಂದುವರಿ; /* uninterested */
		ಗಾಗಿ (i = 0; i < 3; i++) {
			ಆದರೆ (!ui->ಬಹುಲಕ[i])
				ಮುಂದುವರಿ;
			ಬರೆ("%s%06o %s %d\t", ಟ್ಯಾಗ್_resolve_undo, ui->ಬಹುಲಕ[i],
			       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(ಮಾರ್ಗ);
		}
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ce_excluded(ರಚನೆ dir_struct *dir, ರಚನೆ ಸೂಚ್ಯಂಕ_state *istate,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fullname, ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce)
{
	ಪೂರ್ಣಾಂಕ dtype = ce_ಗಮ್ಯ_dtype(ce);
	ಹಿಂತಿರುಗಿ is_excluded(dir, istate, fullname, &dtype);
}

ಸ್ಥಿರ ಶೂನ್ಯ construct_fullname(ರಚನೆ strbuf *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ರಚನೆ repository *repo,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce)
{
	strbuf_reset(ಹೊರಹೋಗು);
	ಆದರೆ (repo->submodule_ಪೂರ್ವಪ್ರತ್ಯಯ)
		strbuf_addstr(ಹೊರಹೋಗು, repo->submodule_ಪೂರ್ವಪ್ರತ್ಯಯ);
	strbuf_addstr(ಹೊರಹೋಗು, ce->name);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_files(ರಚನೆ repository *repo, ರಚನೆ dir_struct *dir)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	ಆದರೆ (show_others || show_killed) {
		ಆದರೆ (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->ಸೂಚ್ಯಂಕ, &pathspec);
		ಆದರೆ (show_others)
			show_other_files(repo->ಸೂಚ್ಯಂಕ, dir);
		ಆದರೆ (show_killed)
			show_killed_files(repo->ಸೂಚ್ಯಂಕ, dir);
	}

	ಆದರೆ (!(show_cached || show_stage || show_deleted || show_modified))
		ಹಿಂತಿರುಗಿ;

	ಗಾಗಿ (i = 0; i < repo->ಸೂಚ್ಯಂಕ->cache_nr; i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = repo->ಸೂಚ್ಯಂಕ->cache[i];
		ರಚನೆ stat st;
		ಪೂರ್ಣಾಂಕ stat_err;

		ಆದರೆ (S_ISSPARSEDIR(ce->ce_ಬಹುಲಕ) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(repo->ಸೂಚ್ಯಂಕ);
			ce = repo->ಸೂಚ್ಯಂಕ->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		ಆದರೆ ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->ಸೂಚ್ಯಂಕ, fullname.buf, ce))
			ಮುಂದುವರಿ;
		ಆದರೆ (ce->ce_flags & CE_UPDATE)
			ಮುಂದುವರಿ;
		ಆದರೆ ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? ಟ್ಯಾಗ್_unmerged :
				(ce_skip_worktree(ce) ? ಟ್ಯಾಗ್_skip_worktree :
				 ಟ್ಯಾಗ್_cached));
			ಆದರೆ (skipping_duplicates)
				ಹೋಗು skip_ಗಮ್ಯ_ಮುಂದಿನ_name;
		}

		ಆದರೆ (!(show_deleted || show_modified))
			ಮುಂದುವರಿ;
		ಆದರೆ (ce_skip_worktree(ce))
			ಮುಂದುವರಿ;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		ಆದರೆ (pathspec.nr == 1 &&
		    !ಹೊಂದಿಕೆ_pathspec(repo->ಸೂಚ್ಯಂಕ, &pathspec, fullname.buf,
				    fullname.len, max_ಪೂರ್ವಪ್ರತ್ಯಯ_len, ಖಾಲಿ,
				    S_ISDIR(ce->ce_ಬಹುಲಕ) ||
				    S_ISGITLINK(ce->ce_ಬಹುಲಕ)))
			ಮುಂದುವರಿ;
		stat_err = lstat(fullname.buf, &st);
		ಆದರೆ (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		ಆದರೆ (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, ಟ್ಯಾಗ್_removed);
			ಆದರೆ (skipping_duplicates)
				ಹೋಗು skip_ಗಮ್ಯ_ಮುಂದಿನ_name;
		}
		ಆದರೆ (show_modified &&
		    (stat_err || ie_modified(repo->ಸೂಚ್ಯಂಕ, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, ಟ್ಯಾಗ್_modified);
			ಆದರೆ (skipping_duplicates)
				ಹೋಗು skip_ಗಮ್ಯ_ಮುಂದಿನ_name;
		}
		ಮುಂದುವರಿ;

skip_ಗಮ್ಯ_ಮುಂದಿನ_name:
		{
			ಪೂರ್ಣಾಂಕ j;
			ರಚನೆ cache_ನಮೂದು **cache = repo->ಸೂಚ್ಯಂಕ->cache;
			ಗಾಗಿ (j = i + 1; j < repo->ಸೂಚ್ಯಂಕ->cache_nr; j++)
				ಆದರೆ (strcmp(ce->name, cache[j]->name))
					ನಿಲ್ಲಿಸು;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
ಸ್ಥಿರ ಶೂನ್ಯ prune_ಸೂಚ್ಯಂಕ(ರಚನೆ ಸೂಚ್ಯಂಕ_state *istate,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಗಾತ್ರ_t prefixlen)
{
	ಪೂರ್ಣಾಂಕ pos;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೊದಲ, ಕೊನೆಯ;

	ಆದರೆ (!ಪೂರ್ವಪ್ರತ್ಯಯ || !istate->cache_nr)
		ಹಿಂತಿರುಗಿ;
	pos = ಸೂಚ್ಯಂಕ_name_pos(istate, ಪೂರ್ವಪ್ರತ್ಯಯ, prefixlen);
	ಆದರೆ (pos < 0)
		pos = -pos-1;
	ಮೊದಲ = pos;
	ಕೊನೆಯ = istate->cache_nr;
	ಆಗ (ಕೊನೆಯ > ಮೊದಲ) {
		ಪೂರ್ಣಾಂಕ ಮುಂದಿನ = ಮೊದಲ + ((ಕೊನೆಯ - ಮೊದಲ) >> 1);
		ಸ್ಥಿರಾಂಕ ರಚನೆ cache_ನಮೂದು *ce = istate->cache[ಮುಂದಿನ];
		ಆದರೆ (!ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(ce->name, ಪೂರ್ವಪ್ರತ್ಯಯ, prefixlen)) {
			ಮೊದಲ = ಮುಂದಿನ+1;
			ಮುಂದುವರಿ;
		}
		ಕೊನೆಯ = ಮುಂದಿನ;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, ಕೊನೆಯ - pos);
	istate->cache_nr = ಕೊನೆಯ - pos;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_common_ಪೂರ್ವಪ್ರತ್ಯಯ_len(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *common_ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಪೂರ್ಣಾಂಕ common_ಪೂರ್ವಪ್ರತ್ಯಯ_len;

	ಆದರೆ (!common_ಪೂರ್ವಪ್ರತ್ಯಯ)
		ಹಿಂತಿರುಗಿ 0;

	common_ಪೂರ್ವಪ್ರತ್ಯಯ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(common_ಪೂರ್ವಪ್ರತ್ಯಯ);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	ಆದರೆ (common_ಪೂರ್ವಪ್ರತ್ಯಯ[common_ಪೂರ್ವಪ್ರತ್ಯಯ_len - 1] == '/')
		common_ಪೂರ್ವಪ್ರತ್ಯಯ_len--;

	ಹಿಂತಿರುಗಿ common_ಪೂರ್ವಪ್ರತ್ಯಯ_len;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_exclude(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ string_list *exclude_list = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_exclude_from(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ dir_struct *dir = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	ಕೂಡಿಸು_patterns_from_file(dir, arg);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_exclude_standard(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
					 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ dir_struct *dir = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	ಸಿದ್ಧತೆ_standard_excludes(dir);

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_ls_files(ಪೂರ್ಣಾಂಕ argc,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd_ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ require_work_ವೃಕ್ಷ = 0, show_ಟ್ಯಾಗ್ = 0, i;
	ಅಕ್ಷರ *max_ಪೂರ್ವಪ್ರತ್ಯಯ;
	ರಚನೆ dir_struct dir = DIR_INIT;
	ರಚನೆ ನಮೂನೆ_list *pl;
	ರಚನೆ string_list exclude_list = STRING_LIST_INIT_NODUP;
	ರಚನೆ option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_ಗಣ_INT('z', ಖಾಲಿ, &ಸಾಲು_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', ಖಾಲಿ, &show_ಟ್ಯಾಗ್,
			N_("identify the file status with tags")),
		OPT_BOOL('v', ಖಾಲಿ, &show_valid_ಬಿಟ್,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', ಖಾಲಿ, &show_fsmonitor_ಬಿಟ್,
			N_("use lowercase letters for 'fsmonitor clean' files")),
		OPT_BOOL('c', "cached", &show_cached,
			N_("show cached files in the output (default)")),
		OPT_BOOL('d', "deleted", &show_deleted,
			N_("show deleted files in the output")),
		OPT_BOOL('m', "modified", &show_modified,
			N_("show modified files in the output")),
		OPT_BOOL('o', "others", &show_others,
			N_("show other files in the output")),
		OPT_ಬಿಟ್('i', "ignored", &dir.flags,
			N_("show ignored files in the output"),
			DIR_SHOW_IGNORED),
		OPT_BOOL('s', "stage", &show_stage,
			N_("show staged contents' object name in the output")),
		OPT_BOOL('k', "killed", &show_killed,
			N_("show files on the filesystem that need to be removed")),
		OPT_ಬಿಟ್(0, "directory", &dir.flags,
			N_("show 'other' directories' names only"),
			DIR_SHOW_OTHER_DIRECTORIES),
		OPT_BOOL(0, "eol", &show_eol, N_("show line endings of files")),
		OPT_NEGBIT(0, "empty-directory", &dir.flags,
			N_("don't show empty directories"),
			DIR_HIDE_ಖಾಲಿಗಳು_DIRECTORIES),
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
		OPT_STRING(0, "exclude-per-directory", &dir.exclude_ಪ್ರತಿ_dir, N_("file"),
			N_("read additional per-directory exclude patterns in <file>")),
		OPT_CALLBACK_F(0, "exclude-standard", &dir, ಖಾಲಿ,
			N_("add the standard git exclusions"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			option_parse_exclude_standard),
		OPT_ಗಣ_INT_F(0, "full-name", &ಪೂರ್ವಪ್ರತ್ಯಯ_len,
			      N_("make the output relative to the project top directory"),
			      0, PARSE_OPT_NONEG),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			N_("recurse through submodules")),
		OPT_BOOL(0, "error-unmatch", &error_unmatch,
			N_("if any <file> is not in the index, treat this as an error")),
		OPT_STRING(0, "with-tree", &with_ವೃಕ್ಷ, N_("tree-ish"),
			N_("pretend that paths removed since <tree-ish> are still present")),
		OPT__ABBREV(&abbrev),
		OPT_BOOL(0, "debug", &debug_ಬಹುಲಕ, N_("show debugging data")),
		OPT_BOOL(0, "deduplicate", &skipping_duplicates,
			 N_("suppress duplicate entries")),
		OPT_BOOL(0, "sparse", &show_sparse_dirs,
			 N_("show sparse directories in the presence of a sparse index")),
		OPT_STRING_F(0, "format", &format, N_("format"),
			     N_("format to use for the output"),
			     PARSE_OPT_NONEG),
		OPT_ಕೊನೆ()
	};
	ಪೂರ್ಣಾಂಕ ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ಪೂರ್ವಪ್ರತ್ಯಯ = cmd_ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ)
		ಪೂರ್ವಪ್ರತ್ಯಯ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ);
	repo_config(repo, git_default_config, ಖಾಲಿ);

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = ಕೂಡಿಸು_ನಮೂನೆ_list(&dir, EXC_CMDL, "--exclude option");
	ಗಾಗಿ (i = 0; i < exclude_list.nr; i++) {
		ಕೂಡಿಸು_ನಮೂನೆ(exclude_list.ವಸ್ತುಗಳು[i].string, "", 0, pl, --exclude_ವಾದಗಳು);
	}

	ಆದರೆ (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_ಟ್ಯಾಗ್))
			usage_ಸಂದೇಶ_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	ಆದರೆ (show_ಟ್ಯಾಗ್ || show_valid_ಬಿಟ್ || show_fsmonitor_ಬಿಟ್) {
		ಟ್ಯಾಗ್_cached = "H ";
		ಟ್ಯಾಗ್_unmerged = "M ";
		ಟ್ಯಾಗ್_removed = "R ";
		ಟ್ಯಾಗ್_modified = "C ";
		ಟ್ಯಾಗ್_other = "? ";
		ಟ್ಯಾಗ್_killed = "K ";
		ಟ್ಯಾಗ್_skip_worktree = "S ";
		ಟ್ಯಾಗ್_resolve_undo = "U ";
	}
	ಆದರೆ (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_ವೃಕ್ಷ = 1;
	ಆದರೆ (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	ಆದರೆ (show_ಟ್ಯಾಗ್ || show_stage)
		skipping_duplicates = 0;
	ಆದರೆ (dir.exclude_ಪ್ರತಿ_dir)
		exc_given = 1;

	ಆದರೆ (require_work_ವೃಕ್ಷ && !is_inside_work_ವೃಕ್ಷ(repo))
		ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(repo);

	ಆದರೆ (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_ವೃಕ್ಷ))
		die("ls-files --recurse-submodules unsupported mode");

	ಆದರೆ (recurse_submodules && error_unmatch)
		die("ls-files --recurse-submodules does not support "
		    "--error-unmatch");

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv);

	/*
	 * Find common prefix for all pathspec's
	 * This is used as a performance optimization which unfortunately cannot
	 * be done when recursing into submodules because when a pathspec is
	 * given which spans repository boundaries you can't simply remove the
	 * submodule entry because the pathspec may match something inside the
	 * submodule.
	 */
	ಆದರೆ (recurse_submodules)
		max_ಪೂರ್ವಪ್ರತ್ಯಯ = ಖಾಲಿ;
	ಇಲ್ಲದಿದ್ದರೆ
		max_ಪೂರ್ವಪ್ರತ್ಯಯ = common_ಪೂರ್ವಪ್ರತ್ಯಯ(&pathspec);
	max_ಪೂರ್ವಪ್ರತ್ಯಯ_len = get_common_ಪೂರ್ವಪ್ರತ್ಯಯ_len(max_ಪೂರ್ವಪ್ರತ್ಯಯ);

	prune_ಸೂಚ್ಯಂಕ(repo->ಸೂಚ್ಯಂಕ, max_ಪೂರ್ವಪ್ರತ್ಯಯ, max_ಪೂರ್ವಪ್ರತ್ಯಯ_len);

	/* Treat unmatching pathspec elements as errors */
	ಆದರೆ (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	ಆದರೆ ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	ಆದರೆ ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	ಆದರೆ (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	ಆದರೆ (with_ವೃಕ್ಷ) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		ಆದರೆ (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_ವೃಕ್ಷ_on_ಸೂಚ್ಯಂಕ(repo->ಸೂಚ್ಯಂಕ, with_ವೃಕ್ಷ, max_ಪೂರ್ವಪ್ರತ್ಯಯ);
	}

	show_files(repo, &dir);

	ಆದರೆ (show_resolve_undo)
		show_ru_info(repo, repo->ಸೂಚ್ಯಂಕ);

	ಆದರೆ (ps_matched && report_ಮಾರ್ಗ_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	ಬಿಡುಗಡೆ(max_ಪೂರ್ವಪ್ರತ್ಯಯ);
	ಹಿಂತಿರುಗಿ ret;
}
