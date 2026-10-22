/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "convert.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "resolve-undo.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "pathspec.h"
#অন্তর্ভাব "read-cache.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "sparse-index.h"
#অন্তর্ভাব "submodule.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "hex.h"


স্থির পূর্ণসংখ্যা abbrev;
স্থির পূর্ণসংখ্যা show_deleted;
স্থির পূর্ণসংখ্যা show_cached;
স্থির পূর্ণসংখ্যা show_others;
স্থির পূর্ণসংখ্যা show_stage;
স্থির পূর্ণসংখ্যা show_unmerged;
স্থির পূর্ণসংখ্যা show_resolve_undo;
স্থির পূর্ণসংখ্যা show_modified;
স্থির পূর্ণসংখ্যা show_killed;
স্থির পূর্ণসংখ্যা show_valid_bit;
স্থির পূর্ণসংখ্যা show_fsmonitor_bit;
স্থির পূর্ণসংখ্যা সারি_terminator = '\n';
স্থির পূর্ণসংখ্যা debug_mode;
স্থির পূর্ণসংখ্যা show_eol;
স্থির পূর্ণসংখ্যা recurse_submodules;
স্থির পূর্ণসংখ্যা skipping_duplicates;
স্থির পূর্ণসংখ্যা show_sparse_dirs;

স্থির ধ্রুবক অক্ষর *prefix;
স্থির পূর্ণসংখ্যা max_prefix_len;
স্থির পূর্ণসংখ্যা prefix_len;
স্থির কাঠামো pathspec pathspec;
স্থির পূর্ণসংখ্যা error_unmatch;
স্থির অক্ষর *ps_matched;
স্থির ধ্রুবক অক্ষর *with_বৃক্ষ;
স্থির পূর্ণসংখ্যা exc_given;
স্থির পূর্ণসংখ্যা exclude_args;
স্থির ধ্রুবক অক্ষর *format;

স্থির ধ্রুবক অক্ষর *tag_cached = "";
স্থির ধ্রুবক অক্ষর *tag_unmerged = "";
স্থির ধ্রুবক অক্ষর *tag_removed = "";
স্থির ধ্রুবক অক্ষর *tag_other = "";
স্থির ধ্রুবক অক্ষর *tag_killed = "";
স্থির ধ্রুবক অক্ষর *tag_modified = "";
স্থির ধ্রুবক অক্ষর *tag_skip_worktree = "";
স্থির ধ্রুবক অক্ষর *tag_resolve_undo = "";

স্থির শূন্য write_eolinfo(কাঠামো সূচক_state *istate,
			  ধ্রুবক কাঠামো cache_entry *ce, ধ্রুবক অক্ষর *path)
{
	যদি (show_eol) {
		কাঠামো stat st;
		ধ্রুবক অক্ষর *i_txt = "";
		ধ্রুবক অক্ষর *w_txt = "";
		ধ্রুবক অক্ষর *a_txt = get_convert_attr_ascii(istate, path);
		যদি (ce && S_ISREG(ce->ce_mode))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		যদি (!lstat(path, &st) && S_ISREG(st.st_mode))
			w_txt = get_wt_convert_stats_ascii(path);
		printf("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

স্থির শূন্য write_name(ধ্রুবক অক্ষর *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, prefix_len ? prefix : ফাঁকা,
				   stdout, সারি_terminator);
}

স্থির শূন্য write_name_to_buf(কাঠামো strbuf *sb, ধ্রুবক অক্ষর *name)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	ধ্রুবক অক্ষর *rel = relative_path(name, prefix_len ? prefix : ফাঁকা, &buf);

	যদি (সারি_terminator)
		quote_c_style(rel, sb, ফাঁকা, 0);
	অন্যথা
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

স্থির ধ্রুবক অক্ষর *get_tag(ধ্রুবক কাঠামো cache_entry *ce, ধ্রুবক অক্ষর *tag)
{
	স্থির অক্ষর alttag[4];

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

	ফেরত tag;
}

স্থির শূন্য print_debug(ধ্রুবক কাঠামো cache_entry *ce)
{
	যদি (debug_mode) {
		ধ্রুবক কাঠামো stat_তথ্য *sd = &ce->ce_stat_তথ্য;

		printf("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		printf("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		printf("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		printf("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		printf("  size: %u\tflags: %x\n", sd->sd_আকার, ce->ce_flags);
	}
}

স্থির শূন্য show_dir_entry(কাঠামো সূচক_state *istate,
			   ধ্রুবক অক্ষর *tag, কাঠামো dir_entry *ent)
{
	পূর্ণসংখ্যা len = max_prefix_len;

	যদি (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	যদি (ps_matched)
		dir_path_match(istate, ent, &pathspec, len, ps_matched);

	fputs(tag, stdout);
	write_eolinfo(istate, ফাঁকা, ent->name);
	write_name(ent->name);
}

স্থির শূন্য show_other_files(কাঠামো সূচক_state *istate,
			     ধ্রুবক কাঠামো dir_struct *dir)
{
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < dir->nr; i++) {
		কাঠামো dir_entry *ent = dir->entries[i];
		যদি (!সূচক_name_is_other(istate, ent->name, ent->len))
			চালু;
		show_dir_entry(istate, tag_other, ent);
	}
}

স্থির শূন্য show_killed_files(কাঠামো সূচক_state *istate,
			      ধ্রুবক কাঠামো dir_struct *dir)
{
	পূর্ণসংখ্যা i;
	জন্য (i = 0; i < dir->nr; i++) {
		কাঠামো dir_entry *ent = dir->entries[i];
		অক্ষর *cp, *sp;
		পূর্ণসংখ্যা pos, len, killed = 0;

		জন্য (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			যদি (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = সূচক_name_pos(istate, ent->name, ent->len);
				যদি (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				যখন (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				যদি (istate->cache_nr <= pos)
					বিরতি;
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
				বিরতি;
			}
			যদি (0 <= সূচক_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				বিরতি;
			}
		}
		যদি (killed)
			show_dir_entry(istate, tag_killed, dir->entries[i]);
	}
}

স্থির শূন্য show_files(কাঠামো repository *repo, কাঠামো dir_struct *dir);

স্থির শূন্য show_submodule(কাঠামো repository *superproject,
			   কাঠামো dir_struct *dir, ধ্রুবক অক্ষর *path)
{
	কাঠামো repository subrepo;

	যদি (repo_submodule_init(&subrepo, superproject, path,
				null_oid(superproject->hash_algo)))
		ফেরত;

	যদি (repo_read_সূচক(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

স্থির শূন্য expand_objectsize(কাঠামো repository *repo, কাঠামো strbuf *সারি,
			      ধ্রুবক কাঠামো object_id *oid,
			      ধ্রুবক গণনা object_type type, unsigned পূর্ণসংখ্যা padded)
{
	স্থির ধ্রুবক অক্ষর padding[] = "       ";
	আকার_t min_len = padded ? strlen(padding) : 0;
	আকার_t orig_len = সারি->len;
	আকার_t len;

	যদি (type == OBJ_BLOB) {
		আকার_t আকার_2;
		যদি (odb_read_object_info(repo->objects, oid, &আকার_2) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(সারি, আকার_2);
	} অন্যথা {
		strbuf_addstr(সারি, "-");
	}
	len = সারি->len - orig_len;
	যদি (len < min_len)
		strbuf_ঢোকাও(সারি, orig_len, padding, min_len - len);
}

স্থির শূন্য show_ce_fmt(কাঠামো repository *repo, ধ্রুবক কাঠামো cache_entry *ce,
			ধ্রুবক অক্ষর *format, ধ্রুবক অক্ষর *fullname) {
	কাঠামো strbuf sb = STRBUF_INIT;

	যখন (strbuf_expand_step(&sb, &format)) {
		আকার_t len;
		কাঠামো stat st;

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
				      get_cached_convert_stats_ascii(repo->সূচক,
				      ce->name) : "");
		অন্যথা যদি (skip_prefix(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_mode) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		অন্যথা যদি (skip_prefix(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->সূচক,
								 fullname));
		অন্যথা যদি (skip_prefix(format, "(path)", &format))
			write_name_to_buf(&sb, fullname);
		অন্যথা
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, সারি_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

স্থির শূন্য show_ce(কাঠামো repository *repo, কাঠামো dir_struct *dir,
		    ধ্রুবক কাঠামো cache_entry *ce, ধ্রুবক অক্ষর *fullname,
		    ধ্রুবক অক্ষর *tag)
{
	যদি (max_prefix_len > strlen(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	যদি (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
	    is_submodule_active(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} অন্যথা যদি (match_pathspec(repo->সূচক, &pathspec, fullname, strlen(fullname),
				  max_prefix_len, ps_matched,
				  S_ISDIR(ce->ce_mode) ||
				  S_ISGITLINK(ce->ce_mode))) {
		যদি (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			ফেরত;
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
		write_eolinfo(repo->সূচক, ce, fullname);
		write_name(fullname);
		print_debug(ce);
	}
}

স্থির শূন্য show_ru_info(কাঠামো repository *repo, কাঠামো সূচক_state *istate)
{
	কাঠামো string_list_item *item;

	যদি (!istate->resolve_undo)
		ফেরত;

	for_each_string_list_item(item, istate->resolve_undo) {
		ধ্রুবক অক্ষর *path = item->string;
		কাঠামো resolve_undo_info *ui = item->util;
		পূর্ণসংখ্যা i, len;

		len = strlen(path);
		যদি (len < max_prefix_len)
			চালু; /* outside of the prefix */
		যদি (!match_pathspec(istate, &pathspec, path, len,
				    max_prefix_len, ps_matched, 0))
			চালু; /* uninterested */
		জন্য (i = 0; i < 3; i++) {
			যদি (!ui->mode[i])
				চালু;
			printf("%s%06o %s %d\t", tag_resolve_undo, ui->mode[i],
			       repo_find_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(path);
		}
	}
}

স্থির পূর্ণসংখ্যা ce_excluded(কাঠামো dir_struct *dir, কাঠামো সূচক_state *istate,
		       ধ্রুবক অক্ষর *fullname, ধ্রুবক কাঠামো cache_entry *ce)
{
	পূর্ণসংখ্যা dtype = ce_to_dtype(ce);
	ফেরত is_excluded(dir, istate, fullname, &dtype);
}

স্থির শূন্য construct_fullname(কাঠামো strbuf *out, ধ্রুবক কাঠামো repository *repo,
			       ধ্রুবক কাঠামো cache_entry *ce)
{
	strbuf_reset(out);
	যদি (repo->submodule_prefix)
		strbuf_addstr(out, repo->submodule_prefix);
	strbuf_addstr(out, ce->name);
}

স্থির শূন্য show_files(কাঠামো repository *repo, কাঠামো dir_struct *dir)
{
	পূর্ণসংখ্যা i;
	কাঠামো strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	যদি (show_others || show_killed) {
		যদি (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->সূচক, &pathspec);
		যদি (show_others)
			show_other_files(repo->সূচক, dir);
		যদি (show_killed)
			show_killed_files(repo->সূচক, dir);
	}

	যদি (!(show_cached || show_stage || show_deleted || show_modified))
		ফেরত;

	জন্য (i = 0; i < repo->সূচক->cache_nr; i++) {
		ধ্রুবক কাঠামো cache_entry *ce = repo->সূচক->cache[i];
		কাঠামো stat st;
		পূর্ণসংখ্যা stat_err;

		যদি (S_ISSPARSEDIR(ce->ce_mode) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_full_সূচক(repo->সূচক);
			ce = repo->সূচক->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		যদি ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->সূচক, fullname.buf, ce))
			চালু;
		যদি (ce->ce_flags & CE_UPDATE)
			চালু;
		যদি ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? tag_unmerged :
				(ce_skip_worktree(ce) ? tag_skip_worktree :
				 tag_cached));
			যদি (skipping_duplicates)
				যাও skip_to_পরবর্তী_name;
		}

		যদি (!(show_deleted || show_modified))
			চালু;
		যদি (ce_skip_worktree(ce))
			চালু;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		যদি (pathspec.nr == 1 &&
		    !match_pathspec(repo->সূচক, &pathspec, fullname.buf,
				    fullname.len, max_prefix_len, ফাঁকা,
				    S_ISDIR(ce->ce_mode) ||
				    S_ISGITLINK(ce->ce_mode)))
			চালু;
		stat_err = lstat(fullname.buf, &st);
		যদি (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		যদি (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, tag_removed);
			যদি (skipping_duplicates)
				যাও skip_to_পরবর্তী_name;
		}
		যদি (show_modified &&
		    (stat_err || ie_modified(repo->সূচক, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, tag_modified);
			যদি (skipping_duplicates)
				যাও skip_to_পরবর্তী_name;
		}
		চালু;

skip_to_পরবর্তী_name:
		{
			পূর্ণসংখ্যা j;
			কাঠামো cache_entry **cache = repo->সূচক->cache;
			জন্য (j = i + 1; j < repo->সূচক->cache_nr; j++)
				যদি (strcmp(ce->name, cache[j]->name))
					বিরতি;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
স্থির শূন্য prune_সূচক(কাঠামো সূচক_state *istate,
			ধ্রুবক অক্ষর *prefix, আকার_t prefixlen)
{
	পূর্ণসংখ্যা pos;
	unsigned পূর্ণসংখ্যা first, last;

	যদি (!prefix || !istate->cache_nr)
		ফেরত;
	pos = সূচক_name_pos(istate, prefix, prefixlen);
	যদি (pos < 0)
		pos = -pos-1;
	first = pos;
	last = istate->cache_nr;
	যখন (last > first) {
		পূর্ণসংখ্যা পরবর্তী = first + ((last - first) >> 1);
		ধ্রুবক কাঠামো cache_entry *ce = istate->cache[পরবর্তী];
		যদি (!strncmp(ce->name, prefix, prefixlen)) {
			first = পরবর্তী+1;
			চালু;
		}
		last = পরবর্তী;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, last - pos);
	istate->cache_nr = last - pos;
}

স্থির পূর্ণসংখ্যা get_common_prefix_len(ধ্রুবক অক্ষর *common_prefix)
{
	পূর্ণসংখ্যা common_prefix_len;

	যদি (!common_prefix)
		ফেরত 0;

	common_prefix_len = strlen(common_prefix);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	যদি (common_prefix[common_prefix_len - 1] == '/')
		common_prefix_len--;

	ফেরত common_prefix_len;
}

স্থির ধ্রুবক অক্ষর * ধ্রুবক ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	ফাঁকা
};

স্থির পূর্ণসংখ্যা option_parse_exclude(ধ্রুবক কাঠামো option *opt,
				ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো string_list *exclude_list = opt->মান;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_exclude_from(ধ্রুবক কাঠামো option *opt,
				     ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো dir_struct *dir = opt->মান;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	add_patterns_from_file(dir, arg);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা option_parse_exclude_standard(ধ্রুবক কাঠামো option *opt,
					 ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো dir_struct *dir = opt->মান;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	setup_standard_excludes(dir);

	ফেরত 0;
}

পূর্ণসংখ্যা cmd_ls_files(পূর্ণসংখ্যা argc,
		 ধ্রুবক অক্ষর **argv,
		 ধ্রুবক অক্ষর *cmd_prefix,
		 কাঠামো repository *repo)
{
	পূর্ণসংখ্যা require_work_বৃক্ষ = 0, show_tag = 0, i;
	অক্ষর *max_prefix;
	কাঠামো dir_struct dir = DIR_INIT;
	কাঠামো pattern_list *pl;
	কাঠামো string_list exclude_list = STRING_LIST_INIT_NODUP;
	কাঠামো option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_SET_INT('z', ফাঁকা, &সারি_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', ফাঁকা, &show_tag,
			N_("identify the file status with tags")),
		OPT_BOOL('v', ফাঁকা, &show_valid_bit,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', ফাঁকা, &show_fsmonitor_bit,
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
		OPT_CALLBACK_F(0, "exclude-standard", &dir, ফাঁকা,
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
	পূর্ণসংখ্যা ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_সূচক = 0;

	prefix = cmd_prefix;
	যদি (prefix)
		prefix_len = strlen(prefix);
	repo_config(repo, git_default_config, ফাঁকা);

	যদি (repo_read_সূচক(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, prefix, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	জন্য (i = 0; i < exclude_list.nr; i++) {
		add_pattern(exclude_list.বস্তুসমূহ[i].string, "", 0, pl, --exclude_args);
	}

	যদি (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_tag))
			usage_বার্তা_opt(_("--format cannot be used with -s, -o, -k, -t, "
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
		max_prefix = ফাঁকা;
	অন্যথা
		max_prefix = common_prefix(&pathspec);
	max_prefix_len = get_common_prefix_len(max_prefix);

	prune_সূচক(repo->সূচক, max_prefix, max_prefix_len);

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
		overlay_বৃক্ষ_on_সূচক(repo->সূচক, with_বৃক্ষ, max_prefix);
	}

	show_files(repo, &dir);

	যদি (show_resolve_undo)
		show_ru_info(repo, repo->সূচক);

	যদি (ps_matched && report_path_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	মুক্ত(max_prefix);
	ফেরত ret;
}
