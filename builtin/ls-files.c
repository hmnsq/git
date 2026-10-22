/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "convert.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "dir.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "strbuf.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "resolve-undo.h"
#অন্তর্ভাৱ "string-list.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "pathspec.h"
#অন্তর্ভাৱ "read-cache.h"
#অন্তর্ভাৱ "setup.h"
#অন্তর্ভাৱ "sparse-index.h"
#অন্তর্ভাৱ "submodule.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "hex.h"


স্থির পূর্ণাঙ্ক abbrev;
স্থির পূর্ণাঙ্ক show_deleted;
স্থির পূর্ণাঙ্ক show_cached;
স্থির পূর্ণাঙ্ক show_others;
স্থির পূর্ণাঙ্ক show_stage;
স্থির পূর্ণাঙ্ক show_unmerged;
স্থির পূর্ণাঙ্ক show_resolve_undo;
স্থির পূর্ণাঙ্ক show_modified;
স্থির পূর্ণাঙ্ক show_killed;
স্থির পূর্ণাঙ্ক show_valid_bit;
স্থির পূর্ণাঙ্ক show_fsmonitor_bit;
স্থির পূর্ণাঙ্ক সাৰি_terminator = '\n';
স্থির পূর্ণাঙ্ক debug_mode;
স্থির পূর্ণাঙ্ক show_eol;
স্থির পূর্ণাঙ্ক recurse_submodules;
স্থির পূর্ণাঙ্ক skipping_duplicates;
স্থির পূর্ণাঙ্ক show_sparse_dirs;

স্থির ধ্রুৱ ৱর্ণ *prefix;
স্থির পূর্ণাঙ্ক max_prefix_len;
স্থির পূর্ণাঙ্ক prefix_len;
স্থির সংরচনা pathspec pathspec;
স্থির পূর্ণাঙ্ক error_unmatch;
স্থির ৱর্ণ *ps_matched;
স্থির ধ্রুৱ ৱর্ণ *with_বৃক্ষ;
স্থির পূর্ণাঙ্ক exc_given;
স্থির পূর্ণাঙ্ক exclude_args;
স্থির ধ্রুৱ ৱর্ণ *format;

স্থির ধ্রুৱ ৱর্ণ *tag_cached = "";
স্থির ধ্রুৱ ৱর্ণ *tag_unmerged = "";
স্থির ধ্রুৱ ৱর্ণ *tag_removed = "";
স্থির ধ্রুৱ ৱর্ণ *tag_other = "";
স্থির ধ্রুৱ ৱর্ণ *tag_killed = "";
স্থির ধ্রুৱ ৱর্ণ *tag_modified = "";
স্থির ধ্রুৱ ৱর্ণ *tag_skip_worktree = "";
স্থির ধ্রুৱ ৱর্ণ *tag_resolve_undo = "";

স্থির শূন্য write_eolinfo(সংরচনা index_state *istate,
			  ধ্রুৱ সংরচনা cache_entry *ce, ধ্রুৱ ৱর্ণ *path)
{
	যদি (show_eol) {
		সংরচনা stat st;
		ধ্রুৱ ৱর্ণ *i_txt = "";
		ধ্রুৱ ৱর্ণ *w_txt = "";
		ধ্রুৱ ৱর্ণ *a_txt = get_convert_attr_ascii(istate, path);
		যদি (ce && S_ISREG(ce->ce_mode))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		যদি (!lstat(path, &st) && S_ISREG(st.st_mode))
			w_txt = get_wt_convert_stats_ascii(path);
		printf("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

স্থির শূন্য write_name(ধ্রুৱ ৱর্ণ *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, prefix_len ? prefix : রিক্ত,
				   stdout, সাৰি_terminator);
}

স্থির শূন্য write_name_to_buf(সংরচনা strbuf *sb, ধ্রুৱ ৱর্ণ *name)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	ধ্রুৱ ৱর্ণ *rel = relative_path(name, prefix_len ? prefix : রিক্ত, &buf);

	যদি (সাৰি_terminator)
		quote_c_style(rel, sb, রিক্ত, 0);
	অন্যথা
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

স্থির ধ্রুৱ ৱর্ণ *get_tag(ধ্রুৱ সংরচনা cache_entry *ce, ধ্রুৱ ৱর্ণ *tag)
{
	স্থির ৱর্ণ alttag[4];

	যদি (tag && *tag && ((show_valid_bit && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_bit && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		memcpy(alttag, tag, 3);

		যদি (isalpha(tag[0])) {
			alttag[0] = tolower(tag[0]);
		} অন্যথা যদি (tag[0] == '?') {
			alttag[0] = '!';
		} অন্যথা {
			alttag[0] = 'v';
			alttag[1] = tag[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		tag = alttag;
	}

	ঘূৰাই tag;
}

স্থির শূন্য print_debug(ধ্রুৱ সংরচনা cache_entry *ce)
{
	যদি (debug_mode) {
		ধ্রুৱ সংরচনা stat_data *sd = &ce->ce_stat_data;

		printf("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		printf("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		printf("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		printf("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		printf("  size: %u\tflags: %x\n", sd->sd_size, ce->ce_flags);
	}
}

স্থির শূন্য show_dir_entry(সংরচনা index_state *istate,
			   ধ্রুৱ ৱর্ণ *tag, সংরচনা dir_entry *ent)
{
	পূর্ণাঙ্ক len = max_prefix_len;

	যদি (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	যদি (ps_matched)
		dir_path_match(istate, ent, &pathspec, len, ps_matched);

	fputs(tag, stdout);
	write_eolinfo(istate, রিক্ত, ent->name);
	write_name(ent->name);
}

স্থির শূন্য show_other_files(সংরচনা index_state *istate,
			     ধ্রুৱ সংরচনা dir_struct *dir)
{
	পূর্ণাঙ্ক i;

	বাবে (i = 0; i < dir->nr; i++) {
		সংরচনা dir_entry *ent = dir->entries[i];
		যদি (!index_name_is_other(istate, ent->name, ent->len))
			অনুৱর্ত;
		show_dir_entry(istate, tag_other, ent);
	}
}

স্থির শূন্য show_killed_files(সংরচনা index_state *istate,
			      ধ্রুৱ সংরচনা dir_struct *dir)
{
	পূর্ণাঙ্ক i;
	বাবে (i = 0; i < dir->nr; i++) {
		সংরচনা dir_entry *ent = dir->entries[i];
		ৱর্ণ *cp, *sp;
		পূর্ণাঙ্ক pos, len, killed = 0;

		বাবে (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			যদি (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = index_name_pos(istate, ent->name, ent->len);
				যদি (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				যেতিয়ালৈ (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				যদি (istate->cache_nr <= pos)
					ভঙ্গ;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				যদি ((ent->len < len) &&
				    !strncmp(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				ভঙ্গ;
			}
			যদি (0 <= index_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				ভঙ্গ;
			}
		}
		যদি (killed)
			show_dir_entry(istate, tag_killed, dir->entries[i]);
	}
}

স্থির শূন্য show_files(সংরচনা repository *repo, সংরচনা dir_struct *dir);

স্থির শূন্য show_submodule(সংরচনা repository *superproject,
			   সংরচনা dir_struct *dir, ধ্রুৱ ৱর্ণ *path)
{
	সংরচনা repository subrepo;

	যদি (repo_submodule_init(&subrepo, superproject, path,
				null_oid(superproject->hash_algo)))
		ঘূৰাই;

	যদি (repo_read_index(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

স্থির শূন্য expand_objectsize(সংরচনা repository *repo, সংরচনা strbuf *সাৰি,
			      ধ্রুৱ সংরচনা object_id *oid,
			      ধ্রুৱ গণনা object_type type, unsigned পূর্ণাঙ্ক padded)
{
	স্থির ধ্রুৱ ৱর্ণ padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = সাৰি->len;
	size_t len;

	যদি (type == OBJ_BLOB) {
		size_t size;
		যদি (odb_read_object_info(repo->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(সাৰি, size);
	} অন্যথা {
		strbuf_addstr(সাৰি, "-");
	}
	len = সাৰি->len - orig_len;
	যদি (len < min_len)
		strbuf_ঢোকাও(সাৰি, orig_len, padding, min_len - len);
}

স্থির শূন্য show_ce_fmt(সংরচনা repository *repo, ধ্রুৱ সংরচনা cache_entry *ce,
			ধ্রুৱ ৱর্ণ *format, ধ্রুৱ ৱর্ণ *fullname) {
	সংরচনা strbuf sb = STRBUF_INIT;

	যেতিয়ালৈ (strbuf_expand_step(&sb, &format)) {
		size_t len;
		সংরচনা stat st;

		যদি (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		অন্যথা যদি ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		অন্যথা যদি (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_mode);
		অন্যথা যদি (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, &ce->oid, abbrev);
		অন্যথা যদি (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_mode)));
		অন্যথা যদি (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 1);
		অন্যথা যদি (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 0);
		অন্যথা যদি (skip_prefix(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		অন্যথা যদি (skip_prefix(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_mode) ?
				      get_cached_convert_stats_ascii(repo->index,
				      ce->name) : "");
		অন্যথা যদি (skip_prefix(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_mode) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		অন্যথা যদি (skip_prefix(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->index,
								 fullname));
		অন্যথা যদি (skip_prefix(format, "(path)", &format))
			write_name_to_buf(&sb, fullname);
		অন্যথা
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, সাৰি_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

স্থির শূন্য show_ce(সংরচনা repository *repo, সংরচনা dir_struct *dir,
		    ধ্রুৱ সংরচনা cache_entry *ce, ধ্রুৱ ৱর্ণ *fullname,
		    ধ্রুৱ ৱর্ণ *tag)
{
	যদি (max_prefix_len > strlen(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	যদি (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
	    is_submodule_active(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} অন্যথা যদি (match_pathspec(repo->index, &pathspec, fullname, strlen(fullname),
				  max_prefix_len, ps_matched,
				  S_ISDIR(ce->ce_mode) ||
				  S_ISGITLINK(ce->ce_mode))) {
		যদি (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			ঘূৰাই;
		}

		tag = get_tag(ce, tag);

		যদি (!show_stage) {
			fputs(tag, stdout);
		} অন্যথা {
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

স্থির শূন্য show_ru_info(সংরচনা repository *repo, সংরচনা index_state *istate)
{
	সংরচনা string_list_item *item;

	যদি (!istate->resolve_undo)
		ঘূৰাই;

	for_each_string_list_item(item, istate->resolve_undo) {
		ধ্রুৱ ৱর্ণ *path = item->string;
		সংরচনা resolve_undo_info *ui = item->util;
		পূর্ণাঙ্ক i, len;

		len = strlen(path);
		যদি (len < max_prefix_len)
			অনুৱর্ত; /* outside of the prefix */
		যদি (!match_pathspec(istate, &pathspec, path, len,
				    max_prefix_len, ps_matched, 0))
			অনুৱর্ত; /* uninterested */
		বাবে (i = 0; i < 3; i++) {
			যদি (!ui->mode[i])
				অনুৱর্ত;
			printf("%s%06o %s %d\t", tag_resolve_undo, ui->mode[i],
			       repo_find_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(path);
		}
	}
}

স্থির পূর্ণাঙ্ক ce_excluded(সংরচনা dir_struct *dir, সংরচনা index_state *istate,
		       ধ্রুৱ ৱর্ণ *fullname, ধ্রুৱ সংরচনা cache_entry *ce)
{
	পূর্ণাঙ্ক dtype = ce_to_dtype(ce);
	ঘূৰাই is_excluded(dir, istate, fullname, &dtype);
}

স্থির শূন্য construct_fullname(সংরচনা strbuf *out, ধ্রুৱ সংরচনা repository *repo,
			       ধ্রুৱ সংরচনা cache_entry *ce)
{
	strbuf_reset(out);
	যদি (repo->submodule_prefix)
		strbuf_addstr(out, repo->submodule_prefix);
	strbuf_addstr(out, ce->name);
}

স্থির শূন্য show_files(সংরচনা repository *repo, সংরচনা dir_struct *dir)
{
	পূর্ণাঙ্ক i;
	সংরচনা strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	যদি (show_others || show_killed) {
		যদি (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->index, &pathspec);
		যদি (show_others)
			show_other_files(repo->index, dir);
		যদি (show_killed)
			show_killed_files(repo->index, dir);
	}

	যদি (!(show_cached || show_stage || show_deleted || show_modified))
		ঘূৰাই;

	বাবে (i = 0; i < repo->index->cache_nr; i++) {
		ধ্রুৱ সংরচনা cache_entry *ce = repo->index->cache[i];
		সংরচনা stat st;
		পূর্ণাঙ্ক stat_err;

		যদি (S_ISSPARSEDIR(ce->ce_mode) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_full_index(repo->index);
			ce = repo->index->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		যদি ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->index, fullname.buf, ce))
			অনুৱর্ত;
		যদি (ce->ce_flags & CE_UPDATE)
			অনুৱর্ত;
		যদি ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? tag_unmerged :
				(ce_skip_worktree(ce) ? tag_skip_worktree :
				 tag_cached));
			যদি (skipping_duplicates)
				গচ্ছ skip_to_next_name;
		}

		যদি (!(show_deleted || show_modified))
			অনুৱর্ত;
		যদি (ce_skip_worktree(ce))
			অনুৱর্ত;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		যদি (pathspec.nr == 1 &&
		    !match_pathspec(repo->index, &pathspec, fullname.buf,
				    fullname.len, max_prefix_len, রিক্ত,
				    S_ISDIR(ce->ce_mode) ||
				    S_ISGITLINK(ce->ce_mode)))
			অনুৱর্ত;
		stat_err = lstat(fullname.buf, &st);
		যদি (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		যদি (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, tag_removed);
			যদি (skipping_duplicates)
				গচ্ছ skip_to_next_name;
		}
		যদি (show_modified &&
		    (stat_err || ie_modified(repo->index, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, tag_modified);
			যদি (skipping_duplicates)
				গচ্ছ skip_to_next_name;
		}
		অনুৱর্ত;

skip_to_next_name:
		{
			পূর্ণাঙ্ক j;
			সংরচনা cache_entry **cache = repo->index->cache;
			বাবে (j = i + 1; j < repo->index->cache_nr; j++)
				যদি (strcmp(ce->name, cache[j]->name))
					ভঙ্গ;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
স্থির শূন্য prune_index(সংরচনা index_state *istate,
			ধ্রুৱ ৱর্ণ *prefix, size_t prefixlen)
{
	পূর্ণাঙ্ক pos;
	unsigned পূর্ণাঙ্ক first, last;

	যদি (!prefix || !istate->cache_nr)
		ঘূৰাই;
	pos = index_name_pos(istate, prefix, prefixlen);
	যদি (pos < 0)
		pos = -pos-1;
	first = pos;
	last = istate->cache_nr;
	যেতিয়ালৈ (last > first) {
		পূর্ণাঙ্ক next = first + ((last - first) >> 1);
		ধ্রুৱ সংরচনা cache_entry *ce = istate->cache[next];
		যদি (!strncmp(ce->name, prefix, prefixlen)) {
			first = next+1;
			অনুৱর্ত;
		}
		last = next;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, last - pos);
	istate->cache_nr = last - pos;
}

স্থির পূর্ণাঙ্ক get_common_prefix_len(ধ্রুৱ ৱর্ণ *common_prefix)
{
	পূর্ণাঙ্ক common_prefix_len;

	যদি (!common_prefix)
		ঘূৰাই 0;

	common_prefix_len = strlen(common_prefix);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	যদি (common_prefix[common_prefix_len - 1] == '/')
		common_prefix_len--;

	ঘূৰাই common_prefix_len;
}

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	রিক্ত
};

স্থির পূর্ণাঙ্ক option_parse_exclude(ধ্রুৱ সংরচনা option *opt,
				ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা string_list *exclude_list = opt->মান;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_exclude_from(ধ্রুৱ সংরচনা option *opt,
				     ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা dir_struct *dir = opt->মান;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	add_patterns_from_file(dir, arg);

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক option_parse_exclude_standard(ধ্রুৱ সংরচনা option *opt,
					 ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা dir_struct *dir = opt->মান;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	setup_standard_excludes(dir);

	ঘূৰাই 0;
}

পূর্ণাঙ্ক cmd_ls_files(পূর্ণাঙ্ক argc,
		 ধ্রুৱ ৱর্ণ **argv,
		 ধ্রুৱ ৱর্ণ *cmd_prefix,
		 সংরচনা repository *repo)
{
	পূর্ণাঙ্ক require_work_বৃক্ষ = 0, show_tag = 0, i;
	ৱর্ণ *max_prefix;
	সংরচনা dir_struct dir = DIR_INIT;
	সংরচনা pattern_list *pl;
	সংরচনা string_list exclude_list = STRING_LIST_INIT_NODUP;
	সংরচনা option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_SET_INT('z', রিক্ত, &সাৰি_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', রিক্ত, &show_tag,
			N_("identify the file status with tags")),
		OPT_BOOL('v', রিক্ত, &show_valid_bit,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', রিক্ত, &show_fsmonitor_bit,
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
		OPT_CALLBACK_F(0, "exclude-standard", &dir, রিক্ত,
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
		OPT_STRING(0, "with-tree", &with_বৃক্ষ, N_("tree-ish"),
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
	পূর্ণাঙ্ক ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	prefix = cmd_prefix;
	যদি (prefix)
		prefix_len = strlen(prefix);
	repo_config(repo, git_default_config, রিক্ত);

	যদি (repo_read_index(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, prefix, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	বাবে (i = 0; i < exclude_list.nr; i++) {
		add_pattern(exclude_list.বস্তুসমূহ[i].string, "", 0, pl, --exclude_args);
	}

	যদি (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_tag))
			usage_বাৰ্তা_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	যদি (show_tag || show_valid_bit || show_fsmonitor_bit) {
		tag_cached = "H ";
		tag_unmerged = "M ";
		tag_removed = "R ";
		tag_modified = "C ";
		tag_other = "? ";
		tag_killed = "K ";
		tag_skip_worktree = "S ";
		tag_resolve_undo = "U ";
	}
	যদি (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_বৃক্ষ = 1;
	যদি (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	যদি (show_tag || show_stage)
		skipping_duplicates = 0;
	যদি (dir.exclude_per_dir)
		exc_given = 1;

	যদি (require_work_বৃক্ষ && !is_inside_work_বৃক্ষ(repo))
		setup_work_বৃক্ষ(repo);

	যদি (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_বৃক্ষ))
		die("ls-files --recurse-submodules unsupported mode");

	যদি (recurse_submodules && error_unmatch)
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
	যদি (recurse_submodules)
		max_prefix = রিক্ত;
	অন্যথা
		max_prefix = common_prefix(&pathspec);
	max_prefix_len = get_common_prefix_len(max_prefix);

	prune_index(repo->index, max_prefix, max_prefix_len);

	/* Treat unmatching pathspec elements as errors */
	যদি (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	যদি ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	যদি ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	যদি (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	যদি (with_বৃক্ষ) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		যদি (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_বৃক্ষ_on_index(repo->index, with_বৃক্ষ, max_prefix);
	}

	show_files(repo, &dir);

	যদি (show_resolve_undo)
		show_ru_info(repo, repo->index);

	যদি (ps_matched && report_path_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	মুক্ত(max_prefix);
	ঘূৰাই ret;
}
