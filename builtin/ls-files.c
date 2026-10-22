/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "convert.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "resolve-undo.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "pathspec.h"
#അന്തര്ഭാവ "read-cache.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "sparse-index.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "hex.h"


സ്ഥിതം പൂർണ്ണസംഖ്യ abbrev;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_deleted;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_cached;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_others;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_stage;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_unmerged;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_resolve_undo;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_modified;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_killed;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_valid_bit;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_fsmonitor_bit;
സ്ഥിതം പൂർണ്ണസംഖ്യ വരി_terminator = '\n';
സ്ഥിതം പൂർണ്ണസംഖ്യ debug_mode;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_eol;
സ്ഥിതം പൂർണ്ണസംഖ്യ recurse_submodules;
സ്ഥിതം പൂർണ്ണസംഖ്യ skipping_duplicates;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_sparse_dirs;

സ്ഥിതം സ്ഥിരം അക്ഷരം *prefix;
സ്ഥിതം പൂർണ്ണസംഖ്യ max_prefix_len;
സ്ഥിതം പൂർണ്ണസംഖ്യ prefix_len;
സ്ഥിതം ഘടന pathspec pathspec;
സ്ഥിതം പൂർണ്ണസംഖ്യ error_unmatch;
സ്ഥിതം അക്ഷരം *ps_matched;
സ്ഥിതം സ്ഥിരം അക്ഷരം *with_വൃക്ഷം;
സ്ഥിതം പൂർണ്ണസംഖ്യ exc_given;
സ്ഥിതം പൂർണ്ണസംഖ്യ exclude_args;
സ്ഥിതം സ്ഥിരം അക്ഷരം *format;

സ്ഥിതം സ്ഥിരം അക്ഷരം *tag_cached = "";
സ്ഥിതം സ്ഥിരം അക്ഷരം *tag_unmerged = "";
സ്ഥിതം സ്ഥിരം അക്ഷരം *tag_removed = "";
സ്ഥിതം സ്ഥിരം അക്ഷരം *tag_other = "";
സ്ഥിതം സ്ഥിരം അക്ഷരം *tag_killed = "";
സ്ഥിതം സ്ഥിരം അക്ഷരം *tag_modified = "";
സ്ഥിതം സ്ഥിരം അക്ഷരം *tag_skip_worktree = "";
സ്ഥിതം സ്ഥിരം അക്ഷരം *tag_resolve_undo = "";

സ്ഥിതം ശൂന്യം write_eolinfo(ഘടന index_state *istate,
			  സ്ഥിരം ഘടന cache_entry *ce, സ്ഥിരം അക്ഷരം *path)
{
	എങ്കിൽ (show_eol) {
		ഘടന stat st;
		സ്ഥിരം അക്ഷരം *i_txt = "";
		സ്ഥിരം അക്ഷരം *w_txt = "";
		സ്ഥിരം അക്ഷരം *a_txt = get_convert_attr_ascii(istate, path);
		എങ്കിൽ (ce && S_ISREG(ce->ce_mode))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		എങ്കിൽ (!lstat(path, &st) && S_ISREG(st.st_mode))
			w_txt = get_wt_convert_stats_ascii(path);
		printf("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

സ്ഥിതം ശൂന്യം write_name(സ്ഥിരം അക്ഷരം *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, prefix_len ? prefix : ശൂന്യ,
				   stdout, വരി_terminator);
}

സ്ഥിതം ശൂന്യം write_name_to_buf(ഘടന strbuf *sb, സ്ഥിരം അക്ഷരം *name)
{
	ഘടന strbuf buf = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *rel = relative_path(name, prefix_len ? prefix : ശൂന്യ, &buf);

	എങ്കിൽ (വരി_terminator)
		quote_c_style(rel, sb, ശൂന്യ, 0);
	അല്ലെങ്കിൽ
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *get_tag(സ്ഥിരം ഘടന cache_entry *ce, സ്ഥിരം അക്ഷരം *tag)
{
	സ്ഥിതം അക്ഷരം alttag[4];

	എങ്കിൽ (tag && *tag && ((show_valid_bit && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_bit && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		memcpy(alttag, tag, 3);

		എങ്കിൽ (isalpha(tag[0])) {
			alttag[0] = tolower(tag[0]);
		} അല്ലെങ്കിൽ എങ്കിൽ (tag[0] == '?') {
			alttag[0] = '!';
		} അല്ലെങ്കിൽ {
			alttag[0] = 'v';
			alttag[1] = tag[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		tag = alttag;
	}

	തിരികെ tag;
}

സ്ഥിതം ശൂന്യം print_debug(സ്ഥിരം ഘടന cache_entry *ce)
{
	എങ്കിൽ (debug_mode) {
		സ്ഥിരം ഘടന stat_data *sd = &ce->ce_stat_data;

		printf("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		printf("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		printf("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		printf("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		printf("  size: %u\tflags: %x\n", sd->sd_size, ce->ce_flags);
	}
}

സ്ഥിതം ശൂന്യം show_dir_entry(ഘടന index_state *istate,
			   സ്ഥിരം അക്ഷരം *tag, ഘടന dir_entry *ent)
{
	പൂർണ്ണസംഖ്യ len = max_prefix_len;

	എങ്കിൽ (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	എങ്കിൽ (ps_matched)
		dir_path_match(istate, ent, &pathspec, len, ps_matched);

	fputs(tag, stdout);
	write_eolinfo(istate, ശൂന്യ, ent->name);
	write_name(ent->name);
}

സ്ഥിതം ശൂന്യം show_other_files(ഘടന index_state *istate,
			     സ്ഥിരം ഘടന dir_struct *dir)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < dir->nr; i++) {
		ഘടന dir_entry *ent = dir->entries[i];
		എങ്കിൽ (!index_name_is_other(istate, ent->name, ent->len))
			തുടരുക;
		show_dir_entry(istate, tag_other, ent);
	}
}

സ്ഥിതം ശൂന്യം show_killed_files(ഘടന index_state *istate,
			      സ്ഥിരം ഘടന dir_struct *dir)
{
	പൂർണ്ണസംഖ്യ i;
	വേണ്ടി (i = 0; i < dir->nr; i++) {
		ഘടന dir_entry *ent = dir->entries[i];
		അക്ഷരം *cp, *sp;
		പൂർണ്ണസംഖ്യ pos, len, killed = 0;

		വേണ്ടി (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			എങ്കിൽ (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = index_name_pos(istate, ent->name, ent->len);
				എങ്കിൽ (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				ആകുമ്പോൾ (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				എങ്കിൽ (istate->cache_nr <= pos)
					നിർത്തുക;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				എങ്കിൽ ((ent->len < len) &&
				    !strncmp(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				നിർത്തുക;
			}
			എങ്കിൽ (0 <= index_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				നിർത്തുക;
			}
		}
		എങ്കിൽ (killed)
			show_dir_entry(istate, tag_killed, dir->entries[i]);
	}
}

സ്ഥിതം ശൂന്യം show_files(ഘടന repository *repo, ഘടന dir_struct *dir);

സ്ഥിതം ശൂന്യം show_submodule(ഘടന repository *superproject,
			   ഘടന dir_struct *dir, സ്ഥിരം അക്ഷരം *path)
{
	ഘടന repository subrepo;

	എങ്കിൽ (repo_submodule_init(&subrepo, superproject, path,
				null_oid(superproject->hash_algo)))
		തിരികെ;

	എങ്കിൽ (repo_read_index(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

സ്ഥിതം ശൂന്യം expand_objectsize(ഘടന repository *repo, ഘടന strbuf *വരി,
			      സ്ഥിരം ഘടന object_id *oid,
			      സ്ഥിരം എണ്ണൽ object_type type, unsigned പൂർണ്ണസംഖ്യ padded)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = വരി->len;
	size_t len;

	എങ്കിൽ (type == OBJ_BLOB) {
		size_t size;
		എങ്കിൽ (odb_read_object_info(repo->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(വരി, size);
	} അല്ലെങ്കിൽ {
		strbuf_addstr(വരി, "-");
	}
	len = വരി->len - orig_len;
	എങ്കിൽ (len < min_len)
		strbuf_തിരുകുക(വരി, orig_len, padding, min_len - len);
}

സ്ഥിതം ശൂന്യം show_ce_fmt(ഘടന repository *repo, സ്ഥിരം ഘടന cache_entry *ce,
			സ്ഥിരം അക്ഷരം *format, സ്ഥിരം അക്ഷരം *fullname) {
	ഘടന strbuf sb = STRBUF_INIT;

	ആകുമ്പോൾ (strbuf_expand_step(&sb, &format)) {
		size_t len;
		ഘടന stat st;

		എങ്കിൽ (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		അല്ലെങ്കിൽ എങ്കിൽ ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_mode);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, &ce->oid, abbrev);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_mode)));
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 1);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 0);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_mode) ?
				      get_cached_convert_stats_ascii(repo->index,
				      ce->name) : "");
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_mode) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->index,
								 fullname));
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(format, "(path)", &format))
			write_name_to_buf(&sb, fullname);
		അല്ലെങ്കിൽ
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, വരി_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

സ്ഥിതം ശൂന്യം show_ce(ഘടന repository *repo, ഘടന dir_struct *dir,
		    സ്ഥിരം ഘടന cache_entry *ce, സ്ഥിരം അക്ഷരം *fullname,
		    സ്ഥിരം അക്ഷരം *tag)
{
	എങ്കിൽ (max_prefix_len > strlen(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	എങ്കിൽ (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
	    is_submodule_active(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} അല്ലെങ്കിൽ എങ്കിൽ (match_pathspec(repo->index, &pathspec, fullname, strlen(fullname),
				  max_prefix_len, ps_matched,
				  S_ISDIR(ce->ce_mode) ||
				  S_ISGITLINK(ce->ce_mode))) {
		എങ്കിൽ (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			തിരികെ;
		}

		tag = get_tag(ce, tag);

		എങ്കിൽ (!show_stage) {
			fputs(tag, stdout);
		} അല്ലെങ്കിൽ {
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

സ്ഥിതം ശൂന്യം show_ru_info(ഘടന repository *repo, ഘടന index_state *istate)
{
	ഘടന string_list_item *item;

	എങ്കിൽ (!istate->resolve_undo)
		തിരികെ;

	for_each_string_list_item(item, istate->resolve_undo) {
		സ്ഥിരം അക്ഷരം *path = item->string;
		ഘടന resolve_undo_info *ui = item->util;
		പൂർണ്ണസംഖ്യ i, len;

		len = strlen(path);
		എങ്കിൽ (len < max_prefix_len)
			തുടരുക; /* outside of the prefix */
		എങ്കിൽ (!match_pathspec(istate, &pathspec, path, len,
				    max_prefix_len, ps_matched, 0))
			തുടരുക; /* uninterested */
		വേണ്ടി (i = 0; i < 3; i++) {
			എങ്കിൽ (!ui->mode[i])
				തുടരുക;
			printf("%s%06o %s %d\t", tag_resolve_undo, ui->mode[i],
			       repo_find_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(path);
		}
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ ce_excluded(ഘടന dir_struct *dir, ഘടന index_state *istate,
		       സ്ഥിരം അക്ഷരം *fullname, സ്ഥിരം ഘടന cache_entry *ce)
{
	പൂർണ്ണസംഖ്യ dtype = ce_to_dtype(ce);
	തിരികെ is_excluded(dir, istate, fullname, &dtype);
}

സ്ഥിതം ശൂന്യം construct_fullname(ഘടന strbuf *out, സ്ഥിരം ഘടന repository *repo,
			       സ്ഥിരം ഘടന cache_entry *ce)
{
	strbuf_reset(out);
	എങ്കിൽ (repo->submodule_prefix)
		strbuf_addstr(out, repo->submodule_prefix);
	strbuf_addstr(out, ce->name);
}

സ്ഥിതം ശൂന്യം show_files(ഘടന repository *repo, ഘടന dir_struct *dir)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	എങ്കിൽ (show_others || show_killed) {
		എങ്കിൽ (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->index, &pathspec);
		എങ്കിൽ (show_others)
			show_other_files(repo->index, dir);
		എങ്കിൽ (show_killed)
			show_killed_files(repo->index, dir);
	}

	എങ്കിൽ (!(show_cached || show_stage || show_deleted || show_modified))
		തിരികെ;

	വേണ്ടി (i = 0; i < repo->index->cache_nr; i++) {
		സ്ഥിരം ഘടന cache_entry *ce = repo->index->cache[i];
		ഘടന stat st;
		പൂർണ്ണസംഖ്യ stat_err;

		എങ്കിൽ (S_ISSPARSEDIR(ce->ce_mode) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_full_index(repo->index);
			ce = repo->index->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		എങ്കിൽ ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->index, fullname.buf, ce))
			തുടരുക;
		എങ്കിൽ (ce->ce_flags & CE_UPDATE)
			തുടരുക;
		എങ്കിൽ ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? tag_unmerged :
				(ce_skip_worktree(ce) ? tag_skip_worktree :
				 tag_cached));
			എങ്കിൽ (skipping_duplicates)
				പോകുക skip_to_next_name;
		}

		എങ്കിൽ (!(show_deleted || show_modified))
			തുടരുക;
		എങ്കിൽ (ce_skip_worktree(ce))
			തുടരുക;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		എങ്കിൽ (pathspec.nr == 1 &&
		    !match_pathspec(repo->index, &pathspec, fullname.buf,
				    fullname.len, max_prefix_len, ശൂന്യ,
				    S_ISDIR(ce->ce_mode) ||
				    S_ISGITLINK(ce->ce_mode)))
			തുടരുക;
		stat_err = lstat(fullname.buf, &st);
		എങ്കിൽ (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		എങ്കിൽ (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, tag_removed);
			എങ്കിൽ (skipping_duplicates)
				പോകുക skip_to_next_name;
		}
		എങ്കിൽ (show_modified &&
		    (stat_err || ie_modified(repo->index, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, tag_modified);
			എങ്കിൽ (skipping_duplicates)
				പോകുക skip_to_next_name;
		}
		തുടരുക;

skip_to_next_name:
		{
			പൂർണ്ണസംഖ്യ j;
			ഘടന cache_entry **cache = repo->index->cache;
			വേണ്ടി (j = i + 1; j < repo->index->cache_nr; j++)
				എങ്കിൽ (strcmp(ce->name, cache[j]->name))
					നിർത്തുക;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
സ്ഥിതം ശൂന്യം prune_index(ഘടന index_state *istate,
			സ്ഥിരം അക്ഷരം *prefix, size_t prefixlen)
{
	പൂർണ്ണസംഖ്യ pos;
	unsigned പൂർണ്ണസംഖ്യ first, last;

	എങ്കിൽ (!prefix || !istate->cache_nr)
		തിരികെ;
	pos = index_name_pos(istate, prefix, prefixlen);
	എങ്കിൽ (pos < 0)
		pos = -pos-1;
	first = pos;
	last = istate->cache_nr;
	ആകുമ്പോൾ (last > first) {
		പൂർണ്ണസംഖ്യ next = first + ((last - first) >> 1);
		സ്ഥിരം ഘടന cache_entry *ce = istate->cache[next];
		എങ്കിൽ (!strncmp(ce->name, prefix, prefixlen)) {
			first = next+1;
			തുടരുക;
		}
		last = next;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, last - pos);
	istate->cache_nr = last - pos;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_common_prefix_len(സ്ഥിരം അക്ഷരം *common_prefix)
{
	പൂർണ്ണസംഖ്യ common_prefix_len;

	എങ്കിൽ (!common_prefix)
		തിരികെ 0;

	common_prefix_len = strlen(common_prefix);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	എങ്കിൽ (common_prefix[common_prefix_len - 1] == '/')
		common_prefix_len--;

	തിരികെ common_prefix_len;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_exclude(സ്ഥിരം ഘടന option *opt,
				സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന string_list *exclude_list = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_exclude_from(സ്ഥിരം ഘടന option *opt,
				     സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന dir_struct *dir = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	add_patterns_from_file(dir, arg);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_exclude_standard(സ്ഥിരം ഘടന option *opt,
					 സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന dir_struct *dir = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	setup_standard_excludes(dir);

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_ls_files(പൂർണ്ണസംഖ്യ argc,
		 സ്ഥിരം അക്ഷരം **argv,
		 സ്ഥിരം അക്ഷരം *cmd_prefix,
		 ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ require_work_വൃക്ഷം = 0, show_tag = 0, i;
	അക്ഷരം *max_prefix;
	ഘടന dir_struct dir = DIR_INIT;
	ഘടന pattern_list *pl;
	ഘടന string_list exclude_list = STRING_LIST_INIT_NODUP;
	ഘടന option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_SET_INT('z', ശൂന്യ, &വരി_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', ശൂന്യ, &show_tag,
			N_("identify the file status with tags")),
		OPT_BOOL('v', ശൂന്യ, &show_valid_bit,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', ശൂന്യ, &show_fsmonitor_bit,
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
		OPT_CALLBACK_F(0, "exclude-standard", &dir, ശൂന്യ,
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
		OPT_STRING(0, "with-tree", &with_വൃക്ഷം, N_("tree-ish"),
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
	പൂർണ്ണസംഖ്യ ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	prefix = cmd_prefix;
	എങ്കിൽ (prefix)
		prefix_len = strlen(prefix);
	repo_config(repo, git_default_config, ശൂന്യ);

	എങ്കിൽ (repo_read_index(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, prefix, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	വേണ്ടി (i = 0; i < exclude_list.nr; i++) {
		add_pattern(exclude_list.ഇനങ്ങൾ[i].string, "", 0, pl, --exclude_args);
	}

	എങ്കിൽ (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_tag))
			usage_സന്ദേശം_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	എങ്കിൽ (show_tag || show_valid_bit || show_fsmonitor_bit) {
		tag_cached = "H ";
		tag_unmerged = "M ";
		tag_removed = "R ";
		tag_modified = "C ";
		tag_other = "? ";
		tag_killed = "K ";
		tag_skip_worktree = "S ";
		tag_resolve_undo = "U ";
	}
	എങ്കിൽ (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_വൃക്ഷം = 1;
	എങ്കിൽ (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	എങ്കിൽ (show_tag || show_stage)
		skipping_duplicates = 0;
	എങ്കിൽ (dir.exclude_per_dir)
		exc_given = 1;

	എങ്കിൽ (require_work_വൃക്ഷം && !is_inside_work_വൃക്ഷം(repo))
		setup_work_വൃക്ഷം(repo);

	എങ്കിൽ (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_വൃക്ഷം))
		die("ls-files --recurse-submodules unsupported mode");

	എങ്കിൽ (recurse_submodules && error_unmatch)
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
	എങ്കിൽ (recurse_submodules)
		max_prefix = ശൂന്യ;
	അല്ലെങ്കിൽ
		max_prefix = common_prefix(&pathspec);
	max_prefix_len = get_common_prefix_len(max_prefix);

	prune_index(repo->index, max_prefix, max_prefix_len);

	/* Treat unmatching pathspec elements as errors */
	എങ്കിൽ (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	എങ്കിൽ ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	എങ്കിൽ ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	എങ്കിൽ (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	എങ്കിൽ (with_വൃക്ഷം) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		എങ്കിൽ (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_വൃക്ഷം_on_index(repo->index, with_വൃക്ഷം, max_prefix);
	}

	show_files(repo, &dir);

	എങ്കിൽ (show_resolve_undo)
		show_ru_info(repo, repo->index);

	എങ്കിൽ (ps_matched && report_path_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	സ്വതന്ത്രം(max_prefix);
	തിരികെ ret;
}
