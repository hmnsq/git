/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "convert.h"
#شامل "environment.h"
#شامل "quote.h"
#شامل "dir.h"
#شامل "gettext.h"
#شامل "object-name.h"
#شامل "strbuf.h"
#شامل "parse-options.h"
#شامل "resolve-undo.h"
#شامل "string-list.h"
#شامل "path.h"
#شامل "pathspec.h"
#شامل "read-cache.h"
#شامل "setup.h"
#شامل "sparse-index.h"
#شامل "submodule.h"
#شامل "odb.h"
#شامل "hex.h"


ایستا صحیح abbrev;
ایستا صحیح show_deleted;
ایستا صحیح show_cached;
ایستا صحیح show_others;
ایستا صحیح show_stage;
ایستا صحیح show_unmerged;
ایستا صحیح show_resolve_undo;
ایستا صحیح show_modified;
ایستا صحیح show_killed;
ایستا صحیح show_valid_bit;
ایستا صحیح show_fsmonitor_bit;
ایستا صحیح سطر_terminator = '\n';
ایستا صحیح debug_mode;
ایستا صحیح show_eol;
ایستا صحیح recurse_submodules;
ایستا صحیح skipping_duplicates;
ایستا صحیح show_sparse_dirs;

ایستا ثابت نویسه *prefix;
ایستا صحیح max_prefix_len;
ایستا صحیح prefix_len;
ایستا ساختار pathspec pathspec;
ایستا صحیح error_unmatch;
ایستا نویسه *ps_matched;
ایستا ثابت نویسه *with_درخت;
ایستا صحیح exc_given;
ایستا صحیح exclude_args;
ایستا ثابت نویسه *format;

ایستا ثابت نویسه *tag_cached = "";
ایستا ثابت نویسه *tag_unmerged = "";
ایستا ثابت نویسه *tag_removed = "";
ایستا ثابت نویسه *tag_other = "";
ایستا ثابت نویسه *tag_killed = "";
ایستا ثابت نویسه *tag_modified = "";
ایستا ثابت نویسه *tag_skip_worktree = "";
ایستا ثابت نویسه *tag_resolve_undo = "";

ایستا تهی write_eolinfo(ساختار index_state *istate,
			  ثابت ساختار cache_entry *ce, ثابت نویسه *path)
{
	اگر (show_eol) {
		ساختار stat st;
		ثابت نویسه *i_txt = "";
		ثابت نویسه *w_txt = "";
		ثابت نویسه *a_txt = get_convert_attr_ascii(istate, path);
		اگر (ce && S_ISREG(ce->ce_mode))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		اگر (!lstat(path, &st) && S_ISREG(st.st_mode))
			w_txt = get_wt_convert_stats_ascii(path);
		printf("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

ایستا تهی write_name(ثابت نویسه *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, prefix_len ? prefix : پوچ,
				   stdout, سطر_terminator);
}

ایستا تهی write_name_to_buf(ساختار strbuf *sb, ثابت نویسه *name)
{
	ساختار strbuf buf = STRBUF_INIT;
	ثابت نویسه *rel = relative_path(name, prefix_len ? prefix : پوچ, &buf);

	اگر (سطر_terminator)
		quote_c_style(rel, sb, پوچ, 0);
	وگرنه
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

ایستا ثابت نویسه *get_tag(ثابت ساختار cache_entry *ce, ثابت نویسه *tag)
{
	ایستا نویسه alttag[4];

	اگر (tag && *tag && ((show_valid_bit && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_bit && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		memcpy(alttag, tag, 3);

		اگر (isalpha(tag[0])) {
			alttag[0] = tolower(tag[0]);
		} وگرنه اگر (tag[0] == '?') {
			alttag[0] = '!';
		} وگرنه {
			alttag[0] = 'v';
			alttag[1] = tag[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		tag = alttag;
	}

	بازگشت tag;
}

ایستا تهی print_debug(ثابت ساختار cache_entry *ce)
{
	اگر (debug_mode) {
		ثابت ساختار stat_data *sd = &ce->ce_stat_data;

		printf("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		printf("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		printf("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		printf("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		printf("  size: %u\tflags: %x\n", sd->sd_size, ce->ce_flags);
	}
}

ایستا تهی show_dir_entry(ساختار index_state *istate,
			   ثابت نویسه *tag, ساختار dir_entry *ent)
{
	صحیح len = max_prefix_len;

	اگر (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	اگر (ps_matched)
		dir_path_match(istate, ent, &pathspec, len, ps_matched);

	fputs(tag, stdout);
	write_eolinfo(istate, پوچ, ent->name);
	write_name(ent->name);
}

ایستا تهی show_other_files(ساختار index_state *istate,
			     ثابت ساختار dir_struct *dir)
{
	صحیح i;

	برای (i = 0; i < dir->nr; i++) {
		ساختار dir_entry *ent = dir->entries[i];
		اگر (!index_name_is_other(istate, ent->name, ent->len))
			ادامه;
		show_dir_entry(istate, tag_other, ent);
	}
}

ایستا تهی show_killed_files(ساختار index_state *istate,
			      ثابت ساختار dir_struct *dir)
{
	صحیح i;
	برای (i = 0; i < dir->nr; i++) {
		ساختار dir_entry *ent = dir->entries[i];
		نویسه *cp, *sp;
		صحیح pos, len, killed = 0;

		برای (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			اگر (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = index_name_pos(istate, ent->name, ent->len);
				اگر (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				وقتی (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				اگر (istate->cache_nr <= pos)
					بشکن;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				اگر ((ent->len < len) &&
				    !strncmp(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				بشکن;
			}
			اگر (0 <= index_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				بشکن;
			}
		}
		اگر (killed)
			show_dir_entry(istate, tag_killed, dir->entries[i]);
	}
}

ایستا تهی show_files(ساختار repository *repo, ساختار dir_struct *dir);

ایستا تهی show_submodule(ساختار repository *superproject,
			   ساختار dir_struct *dir, ثابت نویسه *path)
{
	ساختار repository subrepo;

	اگر (repo_submodule_init(&subrepo, superproject, path,
				null_oid(superproject->hash_algo)))
		بازگشت;

	اگر (repo_read_index(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

ایستا تهی expand_objectsize(ساختار repository *repo, ساختار strbuf *سطر,
			      ثابت ساختار object_id *oid,
			      ثابت شمارش object_type type, unsigned صحیح padded)
{
	ایستا ثابت نویسه padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = سطر->len;
	size_t len;

	اگر (type == OBJ_BLOB) {
		size_t size;
		اگر (odb_read_object_info(repo->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(سطر, size);
	} وگرنه {
		strbuf_addstr(سطر, "-");
	}
	len = سطر->len - orig_len;
	اگر (len < min_len)
		strbuf_درج(سطر, orig_len, padding, min_len - len);
}

ایستا تهی show_ce_fmt(ساختار repository *repo, ثابت ساختار cache_entry *ce,
			ثابت نویسه *format, ثابت نویسه *fullname) {
	ساختار strbuf sb = STRBUF_INIT;

	وقتی (strbuf_expand_step(&sb, &format)) {
		size_t len;
		ساختار stat st;

		اگر (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		وگرنه اگر ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		وگرنه اگر (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_mode);
		وگرنه اگر (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, &ce->oid, abbrev);
		وگرنه اگر (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_mode)));
		وگرنه اگر (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 1);
		وگرنه اگر (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 0);
		وگرنه اگر (skip_prefix(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		وگرنه اگر (skip_prefix(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_mode) ?
				      get_cached_convert_stats_ascii(repo->index,
				      ce->name) : "");
		وگرنه اگر (skip_prefix(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_mode) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		وگرنه اگر (skip_prefix(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->index,
								 fullname));
		وگرنه اگر (skip_prefix(format, "(path)", &format))
			write_name_to_buf(&sb, fullname);
		وگرنه
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, سطر_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

ایستا تهی show_ce(ساختار repository *repo, ساختار dir_struct *dir,
		    ثابت ساختار cache_entry *ce, ثابت نویسه *fullname,
		    ثابت نویسه *tag)
{
	اگر (max_prefix_len > strlen(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	اگر (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
	    is_submodule_active(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} وگرنه اگر (match_pathspec(repo->index, &pathspec, fullname, strlen(fullname),
				  max_prefix_len, ps_matched,
				  S_ISDIR(ce->ce_mode) ||
				  S_ISGITLINK(ce->ce_mode))) {
		اگر (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			بازگشت;
		}

		tag = get_tag(ce, tag);

		اگر (!show_stage) {
			fputs(tag, stdout);
		} وگرنه {
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

ایستا تهی show_ru_info(ساختار repository *repo, ساختار index_state *istate)
{
	ساختار string_list_item *item;

	اگر (!istate->resolve_undo)
		بازگشت;

	for_each_string_list_item(item, istate->resolve_undo) {
		ثابت نویسه *path = item->string;
		ساختار resolve_undo_info *ui = item->util;
		صحیح i, len;

		len = strlen(path);
		اگر (len < max_prefix_len)
			ادامه; /* outside of the prefix */
		اگر (!match_pathspec(istate, &pathspec, path, len,
				    max_prefix_len, ps_matched, 0))
			ادامه; /* uninterested */
		برای (i = 0; i < 3; i++) {
			اگر (!ui->mode[i])
				ادامه;
			printf("%s%06o %s %d\t", tag_resolve_undo, ui->mode[i],
			       repo_find_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(path);
		}
	}
}

ایستا صحیح ce_excluded(ساختار dir_struct *dir, ساختار index_state *istate,
		       ثابت نویسه *fullname, ثابت ساختار cache_entry *ce)
{
	صحیح dtype = ce_to_dtype(ce);
	بازگشت is_excluded(dir, istate, fullname, &dtype);
}

ایستا تهی construct_fullname(ساختار strbuf *out, ثابت ساختار repository *repo,
			       ثابت ساختار cache_entry *ce)
{
	strbuf_reset(out);
	اگر (repo->submodule_prefix)
		strbuf_addstr(out, repo->submodule_prefix);
	strbuf_addstr(out, ce->name);
}

ایستا تهی show_files(ساختار repository *repo, ساختار dir_struct *dir)
{
	صحیح i;
	ساختار strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	اگر (show_others || show_killed) {
		اگر (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->index, &pathspec);
		اگر (show_others)
			show_other_files(repo->index, dir);
		اگر (show_killed)
			show_killed_files(repo->index, dir);
	}

	اگر (!(show_cached || show_stage || show_deleted || show_modified))
		بازگشت;

	برای (i = 0; i < repo->index->cache_nr; i++) {
		ثابت ساختار cache_entry *ce = repo->index->cache[i];
		ساختار stat st;
		صحیح stat_err;

		اگر (S_ISSPARSEDIR(ce->ce_mode) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_full_index(repo->index);
			ce = repo->index->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		اگر ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->index, fullname.buf, ce))
			ادامه;
		اگر (ce->ce_flags & CE_UPDATE)
			ادامه;
		اگر ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? tag_unmerged :
				(ce_skip_worktree(ce) ? tag_skip_worktree :
				 tag_cached));
			اگر (skipping_duplicates)
				برو skip_to_next_name;
		}

		اگر (!(show_deleted || show_modified))
			ادامه;
		اگر (ce_skip_worktree(ce))
			ادامه;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		اگر (pathspec.nr == 1 &&
		    !match_pathspec(repo->index, &pathspec, fullname.buf,
				    fullname.len, max_prefix_len, پوچ,
				    S_ISDIR(ce->ce_mode) ||
				    S_ISGITLINK(ce->ce_mode)))
			ادامه;
		stat_err = lstat(fullname.buf, &st);
		اگر (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		اگر (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, tag_removed);
			اگر (skipping_duplicates)
				برو skip_to_next_name;
		}
		اگر (show_modified &&
		    (stat_err || ie_modified(repo->index, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, tag_modified);
			اگر (skipping_duplicates)
				برو skip_to_next_name;
		}
		ادامه;

skip_to_next_name:
		{
			صحیح j;
			ساختار cache_entry **cache = repo->index->cache;
			برای (j = i + 1; j < repo->index->cache_nr; j++)
				اگر (strcmp(ce->name, cache[j]->name))
					بشکن;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
ایستا تهی prune_index(ساختار index_state *istate,
			ثابت نویسه *prefix, size_t prefixlen)
{
	صحیح pos;
	unsigned صحیح first, last;

	اگر (!prefix || !istate->cache_nr)
		بازگشت;
	pos = index_name_pos(istate, prefix, prefixlen);
	اگر (pos < 0)
		pos = -pos-1;
	first = pos;
	last = istate->cache_nr;
	وقتی (last > first) {
		صحیح next = first + ((last - first) >> 1);
		ثابت ساختار cache_entry *ce = istate->cache[next];
		اگر (!strncmp(ce->name, prefix, prefixlen)) {
			first = next+1;
			ادامه;
		}
		last = next;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, last - pos);
	istate->cache_nr = last - pos;
}

ایستا صحیح get_common_prefix_len(ثابت نویسه *common_prefix)
{
	صحیح common_prefix_len;

	اگر (!common_prefix)
		بازگشت 0;

	common_prefix_len = strlen(common_prefix);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	اگر (common_prefix[common_prefix_len - 1] == '/')
		common_prefix_len--;

	بازگشت common_prefix_len;
}

ایستا ثابت نویسه * ثابت ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	پوچ
};

ایستا صحیح option_parse_exclude(ثابت ساختار option *opt,
				ثابت نویسه *arg, صحیح unset)
{
	ساختار string_list *exclude_list = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	بازگشت 0;
}

ایستا صحیح option_parse_exclude_from(ثابت ساختار option *opt,
				     ثابت نویسه *arg, صحیح unset)
{
	ساختار dir_struct *dir = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	add_patterns_from_file(dir, arg);

	بازگشت 0;
}

ایستا صحیح option_parse_exclude_standard(ثابت ساختار option *opt,
					 ثابت نویسه *arg, صحیح unset)
{
	ساختار dir_struct *dir = opt->مقدار;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	setup_standard_excludes(dir);

	بازگشت 0;
}

صحیح cmd_ls_files(صحیح argc,
		 ثابت نویسه **argv,
		 ثابت نویسه *cmd_prefix,
		 ساختار repository *repo)
{
	صحیح require_work_درخت = 0, show_tag = 0, i;
	نویسه *max_prefix;
	ساختار dir_struct dir = DIR_INIT;
	ساختار pattern_list *pl;
	ساختار string_list exclude_list = STRING_LIST_INIT_NODUP;
	ساختار option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_SET_INT('z', پوچ, &سطر_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', پوچ, &show_tag,
			N_("identify the file status with tags")),
		OPT_BOOL('v', پوچ, &show_valid_bit,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', پوچ, &show_fsmonitor_bit,
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
		OPT_CALLBACK_F(0, "exclude-standard", &dir, پوچ,
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
		OPT_STRING(0, "with-tree", &with_درخت, N_("tree-ish"),
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
	صحیح ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	prefix = cmd_prefix;
	اگر (prefix)
		prefix_len = strlen(prefix);
	repo_config(repo, git_default_config, پوچ);

	اگر (repo_read_index(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, prefix, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	برای (i = 0; i < exclude_list.nr; i++) {
		add_pattern(exclude_list.اقلام[i].string, "", 0, pl, --exclude_args);
	}

	اگر (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_tag))
			usage_پیام_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	اگر (show_tag || show_valid_bit || show_fsmonitor_bit) {
		tag_cached = "H ";
		tag_unmerged = "M ";
		tag_removed = "R ";
		tag_modified = "C ";
		tag_other = "? ";
		tag_killed = "K ";
		tag_skip_worktree = "S ";
		tag_resolve_undo = "U ";
	}
	اگر (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_درخت = 1;
	اگر (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	اگر (show_tag || show_stage)
		skipping_duplicates = 0;
	اگر (dir.exclude_per_dir)
		exc_given = 1;

	اگر (require_work_درخت && !is_inside_work_درخت(repo))
		setup_work_درخت(repo);

	اگر (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_درخت))
		die("ls-files --recurse-submodules unsupported mode");

	اگر (recurse_submodules && error_unmatch)
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
	اگر (recurse_submodules)
		max_prefix = پوچ;
	وگرنه
		max_prefix = common_prefix(&pathspec);
	max_prefix_len = get_common_prefix_len(max_prefix);

	prune_index(repo->index, max_prefix, max_prefix_len);

	/* Treat unmatching pathspec elements as errors */
	اگر (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	اگر ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	اگر ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	اگر (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	اگر (with_درخت) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		اگر (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_درخت_on_index(repo->index, with_درخت, max_prefix);
	}

	show_files(repo, &dir);

	اگر (show_resolve_undo)
		show_ru_info(repo, repo->index);

	اگر (ps_matched && report_path_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	آزاد(max_prefix);
	بازگشت ret;
}
