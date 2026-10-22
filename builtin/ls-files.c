/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

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


ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ abbrev;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_deleted;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_cached;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_others;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_stage;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_unmerged;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_resolve_undo;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_modified;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_killed;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_valid_bit;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_fsmonitor_bit;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_terminator = '\n';
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ debug_mode;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_eol;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ recurse_submodules;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ skipping_duplicates;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_sparse_dirs;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *prefix;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ max_prefix_len;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prefix_len;
ಸ್ಥಿರ ಸಂರಚನಾ pathspec pathspec;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ error_unmatch;
ಸ್ಥಿರ ವರ್ಣ *ps_matched;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *with_ವೃಕ್ಷ;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ exc_given;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ exclude_args;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *format;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *tag_cached = "";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *tag_unmerged = "";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *tag_removed = "";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *tag_other = "";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *tag_killed = "";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *tag_modified = "";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *tag_skip_worktree = "";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *tag_resolve_undo = "";

ಸ್ಥಿರ ಶೂನ್ಯ write_eolinfo(ಸಂರಚನಾ index_state *istate,
			  ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce, ಧ್ರುವ ವರ್ಣ *path)
{
	ಯದಿ (show_eol) {
		ಸಂರಚನಾ stat st;
		ಧ್ರುವ ವರ್ಣ *i_txt = "";
		ಧ್ರುವ ವರ್ಣ *w_txt = "";
		ಧ್ರುವ ವರ್ಣ *a_txt = get_convert_attr_ascii(istate, path);
		ಯದಿ (ce && S_ISREG(ce->ce_mode))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		ಯದಿ (!lstat(path, &st) && S_ISREG(st.st_mode))
			w_txt = get_wt_convert_stats_ascii(path);
		printf("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ write_name(ಧ್ರುವ ವರ್ಣ *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, prefix_len ? prefix : ರಿಕ್ತ,
				   stdout, ಸಾಲು_terminator);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_name_to_buf(ಸಂರಚನಾ strbuf *sb, ಧ್ರುವ ವರ್ಣ *name)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *rel = relative_path(name, prefix_len ? prefix : ರಿಕ್ತ, &buf);

	ಯದಿ (ಸಾಲು_terminator)
		quote_c_style(rel, sb, ರಿಕ್ತ, 0);
	ಅನ್ಯಥಾ
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *get_tag(ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce, ಧ್ರುವ ವರ್ಣ *tag)
{
	ಸ್ಥಿರ ವರ್ಣ alttag[4];

	ಯದಿ (tag && *tag && ((show_valid_bit && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_bit && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		memcpy(alttag, tag, 3);

		ಯದಿ (isalpha(tag[0])) {
			alttag[0] = tolower(tag[0]);
		} ಅನ್ಯಥಾ ಯದಿ (tag[0] == '?') {
			alttag[0] = '!';
		} ಅನ್ಯಥಾ {
			alttag[0] = 'v';
			alttag[1] = tag[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		tag = alttag;
	}

	ಪ್ರತ್ಯಾಗಮ tag;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_debug(ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce)
{
	ಯದಿ (debug_mode) {
		ಧ್ರುವ ಸಂರಚನಾ stat_data *sd = &ce->ce_stat_data;

		printf("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		printf("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		printf("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		printf("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		printf("  size: %u\tflags: %x\n", sd->sd_size, ce->ce_flags);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_dir_entry(ಸಂರಚನಾ index_state *istate,
			   ಧ್ರುವ ವರ್ಣ *tag, ಸಂರಚನಾ dir_entry *ent)
{
	ಪೂರ್ಣಾಙ್ಕ len = max_prefix_len;

	ಯದಿ (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	ಯದಿ (ps_matched)
		dir_path_match(istate, ent, &pathspec, len, ps_matched);

	fputs(tag, stdout);
	write_eolinfo(istate, ರಿಕ್ತ, ent->name);
	write_name(ent->name);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_other_files(ಸಂರಚನಾ index_state *istate,
			     ಧ್ರುವ ಸಂರಚನಾ dir_struct *dir)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < dir->nr; i++) {
		ಸಂರಚನಾ dir_entry *ent = dir->entries[i];
		ಯದಿ (!index_name_is_other(istate, ent->name, ent->len))
			ಅನುವರ್ತ;
		show_dir_entry(istate, tag_other, ent);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_killed_files(ಸಂರಚನಾ index_state *istate,
			      ಧ್ರುವ ಸಂರಚನಾ dir_struct *dir)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪ್ರತಿ (i = 0; i < dir->nr; i++) {
		ಸಂರಚನಾ dir_entry *ent = dir->entries[i];
		ವರ್ಣ *cp, *sp;
		ಪೂರ್ಣಾಙ್ಕ pos, len, killed = 0;

		ಪ್ರತಿ (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			ಯದಿ (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = index_name_pos(istate, ent->name, ent->len);
				ಯದಿ (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				ಯಾವತ್ (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				ಯದಿ (istate->cache_nr <= pos)
					ಭಙ್ಗ;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				ಯದಿ ((ent->len < len) &&
				    !strncmp(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				ಭಙ್ಗ;
			}
			ಯದಿ (0 <= index_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				ಭಙ್ಗ;
			}
		}
		ಯದಿ (killed)
			show_dir_entry(istate, tag_killed, dir->entries[i]);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_files(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ dir_struct *dir);

ಸ್ಥಿರ ಶೂನ್ಯ show_submodule(ಸಂರಚನಾ repository *superproject,
			   ಸಂರಚನಾ dir_struct *dir, ಧ್ರುವ ವರ್ಣ *path)
{
	ಸಂರಚನಾ repository subrepo;

	ಯದಿ (repo_submodule_init(&subrepo, superproject, path,
				null_oid(superproject->hash_algo)))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (repo_read_index(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

ಸ್ಥಿರ ಶೂನ್ಯ expand_objectsize(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ strbuf *ಸಾಲು,
			      ಧ್ರುವ ಸಂರಚನಾ object_id *oid,
			      ಧ್ರುವ ಗಣನಾ object_type type, unsigned ಪೂರ್ಣಾಙ್ಕ padded)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = ಸಾಲು->len;
	size_t len;

	ಯದಿ (type == OBJ_BLOB) {
		size_t size;
		ಯದಿ (odb_read_object_info(repo->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(ಸಾಲು, size);
	} ಅನ್ಯಥಾ {
		strbuf_addstr(ಸಾಲು, "-");
	}
	len = ಸಾಲು->len - orig_len;
	ಯದಿ (len < min_len)
		strbuf_ಸೇರಿಸು(ಸಾಲು, orig_len, padding, min_len - len);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ce_fmt(ಸಂರಚನಾ repository *repo, ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce,
			ಧ್ರುವ ವರ್ಣ *format, ಧ್ರುವ ವರ್ಣ *fullname) {
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	ಯಾವತ್ (strbuf_expand_step(&sb, &format)) {
		size_t len;
		ಸಂರಚನಾ stat st;

		ಯದಿ (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		ಅನ್ಯಥಾ ಯದಿ ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_mode);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, &ce->oid, abbrev);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_mode)));
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 1);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 0);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_mode) ?
				      get_cached_convert_stats_ascii(repo->index,
				      ce->name) : "");
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_mode) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->index,
								 fullname));
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(format, "(path)", &format))
			write_name_to_buf(&sb, fullname);
		ಅನ್ಯಥಾ
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, ಸಾಲು_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ce(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ dir_struct *dir,
		    ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce, ಧ್ರುವ ವರ್ಣ *fullname,
		    ಧ್ರುವ ವರ್ಣ *tag)
{
	ಯದಿ (max_prefix_len > strlen(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	ಯದಿ (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
	    is_submodule_active(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} ಅನ್ಯಥಾ ಯದಿ (match_pathspec(repo->index, &pathspec, fullname, strlen(fullname),
				  max_prefix_len, ps_matched,
				  S_ISDIR(ce->ce_mode) ||
				  S_ISGITLINK(ce->ce_mode))) {
		ಯದಿ (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			ಪ್ರತ್ಯಾಗಮ;
		}

		tag = get_tag(ce, tag);

		ಯದಿ (!show_stage) {
			fputs(tag, stdout);
		} ಅನ್ಯಥಾ {
			printf("%s%06o %s %d\t",
			       tag,
			       ce->ce_mode,
			       repo_find_unique_abbrev(repo, &ce->oid, abbrev),
			       ce_stage(ce));
		}
		write_eolinfo(repo->index, ce, fullname);
		write_name(fullname);
		print_debug(ce);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ru_info(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ index_state *istate)
{
	ಸಂರಚನಾ string_list_item *item;

	ಯದಿ (!istate->resolve_undo)
		ಪ್ರತ್ಯಾಗಮ;

	for_each_string_list_item(item, istate->resolve_undo) {
		ಧ್ರುವ ವರ್ಣ *path = item->string;
		ಸಂರಚನಾ resolve_undo_info *ui = item->util;
		ಪೂರ್ಣಾಙ್ಕ i, len;

		len = strlen(path);
		ಯದಿ (len < max_prefix_len)
			ಅನುವರ್ತ; /* outside of the prefix */
		ಯದಿ (!match_pathspec(istate, &pathspec, path, len,
				    max_prefix_len, ps_matched, 0))
			ಅನುವರ್ತ; /* uninterested */
		ಪ್ರತಿ (i = 0; i < 3; i++) {
			ಯದಿ (!ui->mode[i])
				ಅನುವರ್ತ;
			printf("%s%06o %s %d\t", tag_resolve_undo, ui->mode[i],
			       repo_find_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(path);
		}
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ce_excluded(ಸಂರಚನಾ dir_struct *dir, ಸಂರಚನಾ index_state *istate,
		       ಧ್ರುವ ವರ್ಣ *fullname, ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce)
{
	ಪೂರ್ಣಾಙ್ಕ dtype = ce_to_dtype(ce);
	ಪ್ರತ್ಯಾಗಮ is_excluded(dir, istate, fullname, &dtype);
}

ಸ್ಥಿರ ಶೂನ್ಯ construct_fullname(ಸಂರಚನಾ strbuf *out, ಧ್ರುವ ಸಂರಚನಾ repository *repo,
			       ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce)
{
	strbuf_reset(out);
	ಯದಿ (repo->submodule_prefix)
		strbuf_addstr(out, repo->submodule_prefix);
	strbuf_addstr(out, ce->name);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_files(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ dir_struct *dir)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	ಯದಿ (show_others || show_killed) {
		ಯದಿ (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->index, &pathspec);
		ಯದಿ (show_others)
			show_other_files(repo->index, dir);
		ಯದಿ (show_killed)
			show_killed_files(repo->index, dir);
	}

	ಯದಿ (!(show_cached || show_stage || show_deleted || show_modified))
		ಪ್ರತ್ಯಾಗಮ;

	ಪ್ರತಿ (i = 0; i < repo->index->cache_nr; i++) {
		ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce = repo->index->cache[i];
		ಸಂರಚನಾ stat st;
		ಪೂರ್ಣಾಙ್ಕ stat_err;

		ಯದಿ (S_ISSPARSEDIR(ce->ce_mode) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_full_index(repo->index);
			ce = repo->index->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		ಯದಿ ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->index, fullname.buf, ce))
			ಅನುವರ್ತ;
		ಯದಿ (ce->ce_flags & CE_UPDATE)
			ಅನುವರ್ತ;
		ಯದಿ ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? tag_unmerged :
				(ce_skip_worktree(ce) ? tag_skip_worktree :
				 tag_cached));
			ಯದಿ (skipping_duplicates)
				ಗಚ್ಛ skip_to_next_name;
		}

		ಯದಿ (!(show_deleted || show_modified))
			ಅನುವರ್ತ;
		ಯದಿ (ce_skip_worktree(ce))
			ಅನುವರ್ತ;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		ಯದಿ (pathspec.nr == 1 &&
		    !match_pathspec(repo->index, &pathspec, fullname.buf,
				    fullname.len, max_prefix_len, ರಿಕ್ತ,
				    S_ISDIR(ce->ce_mode) ||
				    S_ISGITLINK(ce->ce_mode)))
			ಅನುವರ್ತ;
		stat_err = lstat(fullname.buf, &st);
		ಯದಿ (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		ಯದಿ (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, tag_removed);
			ಯದಿ (skipping_duplicates)
				ಗಚ್ಛ skip_to_next_name;
		}
		ಯದಿ (show_modified &&
		    (stat_err || ie_modified(repo->index, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, tag_modified);
			ಯದಿ (skipping_duplicates)
				ಗಚ್ಛ skip_to_next_name;
		}
		ಅನುವರ್ತ;

skip_to_next_name:
		{
			ಪೂರ್ಣಾಙ್ಕ j;
			ಸಂರಚನಾ cache_entry **cache = repo->index->cache;
			ಪ್ರತಿ (j = i + 1; j < repo->index->cache_nr; j++)
				ಯದಿ (strcmp(ce->name, cache[j]->name))
					ಭಙ್ಗ;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
ಸ್ಥಿರ ಶೂನ್ಯ prune_index(ಸಂರಚನಾ index_state *istate,
			ಧ್ರುವ ವರ್ಣ *prefix, size_t prefixlen)
{
	ಪೂರ್ಣಾಙ್ಕ pos;
	unsigned ಪೂರ್ಣಾಙ್ಕ first, last;

	ಯದಿ (!prefix || !istate->cache_nr)
		ಪ್ರತ್ಯಾಗಮ;
	pos = index_name_pos(istate, prefix, prefixlen);
	ಯದಿ (pos < 0)
		pos = -pos-1;
	first = pos;
	last = istate->cache_nr;
	ಯಾವತ್ (last > first) {
		ಪೂರ್ಣಾಙ್ಕ next = first + ((last - first) >> 1);
		ಧ್ರುವ ಸಂರಚನಾ cache_entry *ce = istate->cache[next];
		ಯದಿ (!strncmp(ce->name, prefix, prefixlen)) {
			first = next+1;
			ಅನುವರ್ತ;
		}
		last = next;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, last - pos);
	istate->cache_nr = last - pos;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_common_prefix_len(ಧ್ರುವ ವರ್ಣ *common_prefix)
{
	ಪೂರ್ಣಾಙ್ಕ common_prefix_len;

	ಯದಿ (!common_prefix)
		ಪ್ರತ್ಯಾಗಮ 0;

	common_prefix_len = strlen(common_prefix);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	ಯದಿ (common_prefix[common_prefix_len - 1] == '/')
		common_prefix_len--;

	ಪ್ರತ್ಯಾಗಮ common_prefix_len;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_exclude(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ string_list *exclude_list = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_exclude_from(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				     ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ dir_struct *dir = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	add_patterns_from_file(dir, arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_exclude_standard(ಧ್ರುವ ಸಂರಚನಾ option *opt,
					 ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ dir_struct *dir = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	setup_standard_excludes(dir);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_ls_files(ಪೂರ್ಣಾಙ್ಕ argc,
		 ಧ್ರುವ ವರ್ಣ **argv,
		 ಧ್ರುವ ವರ್ಣ *cmd_prefix,
		 ಸಂರಚನಾ repository *repo)
{
	ಪೂರ್ಣಾಙ್ಕ require_work_ವೃಕ್ಷ = 0, show_tag = 0, i;
	ವರ್ಣ *max_prefix;
	ಸಂರಚನಾ dir_struct dir = DIR_INIT;
	ಸಂರಚನಾ pattern_list *pl;
	ಸಂರಚನಾ string_list exclude_list = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_SET_INT('z', ರಿಕ್ತ, &ಸಾಲು_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', ರಿಕ್ತ, &show_tag,
			N_("identify the file status with tags")),
		OPT_BOOL('v', ರಿಕ್ತ, &show_valid_bit,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', ರಿಕ್ತ, &show_fsmonitor_bit,
			N_("use lowercase letters for 'fsmonitor clean' files")),
		OPT_BOOL('c', "cached", &show_cached,
			N_("show cached files in the output (default)")),
		OPT_BOOL('d', "deleted", &show_deleted,
			N_("show deleted files in the output")),
		OPT_BOOL('m', "modified", &show_modified,
			N_("show modified files in the output")),
		OPT_BOOL('o', "others", &show_others,
			N_("show other files in the output")),
		OPT_BIT('i', "ignored", &dir.flags,
			N_("show ignored files in the output"),
			DIR_SHOW_IGNORED),
		OPT_BOOL('s', "stage", &show_stage,
			N_("show staged contents' object name in the output")),
		OPT_BOOL('k', "killed", &show_killed,
			N_("show files on the filesystem that need to be removed")),
		OPT_BIT(0, "directory", &dir.flags,
			N_("show 'other' directories' names only"),
			DIR_SHOW_OTHER_DIRECTORIES),
		OPT_BOOL(0, "eol", &show_eol, N_("show line endings of files")),
		OPT_NEGBIT(0, "empty-directory", &dir.flags,
			N_("don't show empty directories"),
			DIR_HIDE_EMPTY_DIRECTORIES),
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
		OPT_STRING(0, "exclude-per-directory", &dir.exclude_per_dir, N_("file"),
			N_("read additional per-directory exclude patterns in <file>")),
		OPT_CALLBACK_F(0, "exclude-standard", &dir, ರಿಕ್ತ,
			N_("add the standard git exclusions"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			option_parse_exclude_standard),
		OPT_SET_INT_F(0, "full-name", &prefix_len,
			      N_("make the output relative to the project top directory"),
			      0, PARSE_OPT_NONEG),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			N_("recurse through submodules")),
		OPT_BOOL(0, "error-unmatch", &error_unmatch,
			N_("if any <file> is not in the index, treat this as an error")),
		OPT_STRING(0, "with-tree", &with_ವೃಕ್ಷ, N_("tree-ish"),
			N_("pretend that paths removed since <tree-ish> are still present")),
		OPT__ABBREV(&abbrev),
		OPT_BOOL(0, "debug", &debug_mode, N_("show debugging data")),
		OPT_BOOL(0, "deduplicate", &skipping_duplicates,
			 N_("suppress duplicate entries")),
		OPT_BOOL(0, "sparse", &show_sparse_dirs,
			 N_("show sparse directories in the presence of a sparse index")),
		OPT_STRING_F(0, "format", &format, N_("format"),
			     N_("format to use for the output"),
			     PARSE_OPT_NONEG),
		OPT_END()
	};
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	prefix = cmd_prefix;
	ಯದಿ (prefix)
		prefix_len = strlen(prefix);
	repo_config(repo, git_default_config, ರಿಕ್ತ);

	ಯದಿ (repo_read_index(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, prefix, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	ಪ್ರತಿ (i = 0; i < exclude_list.nr; i++) {
		add_pattern(exclude_list.ವಸ್ತುಗಳು[i].string, "", 0, pl, --exclude_args);
	}

	ಯದಿ (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_tag))
			usage_ಸಂದೇಶ_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	ಯದಿ (show_tag || show_valid_bit || show_fsmonitor_bit) {
		tag_cached = "H ";
		tag_unmerged = "M ";
		tag_removed = "R ";
		tag_modified = "C ";
		tag_other = "? ";
		tag_killed = "K ";
		tag_skip_worktree = "S ";
		tag_resolve_undo = "U ";
	}
	ಯದಿ (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_ವೃಕ್ಷ = 1;
	ಯದಿ (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	ಯದಿ (show_tag || show_stage)
		skipping_duplicates = 0;
	ಯದಿ (dir.exclude_per_dir)
		exc_given = 1;

	ಯದಿ (require_work_ವೃಕ್ಷ && !is_inside_work_ವೃಕ್ಷ(repo))
		setup_work_ವೃಕ್ಷ(repo);

	ಯದಿ (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_ವೃಕ್ಷ))
		die("ls-files --recurse-submodules unsupported mode");

	ಯದಿ (recurse_submodules && error_unmatch)
		die("ls-files --recurse-submodules does not support "
		    "--error-unmatch");

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	/*
	 * Find common prefix for all pathspec's
	 * This is used as a performance optimization which unfortunately cannot
	 * be done when recursing into submodules because when a pathspec is
	 * given which spans repository boundaries you can't simply remove the
	 * submodule entry because the pathspec may match something inside the
	 * submodule.
	 */
	ಯದಿ (recurse_submodules)
		max_prefix = ರಿಕ್ತ;
	ಅನ್ಯಥಾ
		max_prefix = common_prefix(&pathspec);
	max_prefix_len = get_common_prefix_len(max_prefix);

	prune_index(repo->index, max_prefix, max_prefix_len);

	/* Treat unmatching pathspec elements as errors */
	ಯದಿ (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	ಯದಿ ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	ಯದಿ ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	ಯದಿ (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	ಯದಿ (with_ವೃಕ್ಷ) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		ಯದಿ (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_ವೃಕ್ಷ_on_index(repo->index, with_ವೃಕ್ಷ, max_prefix);
	}

	show_files(repo, &dir);

	ಯದಿ (show_resolve_undo)
		show_ru_info(repo, repo->index);

	ಯದಿ (ps_matched && report_path_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	ಮುಕ್ತ(max_prefix);
	ಪ್ರತ್ಯಾಗಮ ret;
}
