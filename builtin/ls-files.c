/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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


ساکن صحیح abbrev;
ساکن صحیح show_deleted;
ساکن صحیح show_cached;
ساکن صحیح show_others;
ساکن صحیح show_stage;
ساکن صحیح show_unmerged;
ساکن صحیح show_resolve_undo;
ساکن صحیح show_modified;
ساکن صحیح show_killed;
ساکن صحیح show_valid_ثنائی_ہندسہ;
ساکن صحیح show_fsmonitor_ثنائی_ہندسہ;
ساکن صحیح سطر_terminator = '\n';
ساکن صحیح debug_منوال;
ساکن صحیح show_eol;
ساکن صحیح recurse_submodules;
ساکن صحیح skipping_duplicates;
ساکن صحیح show_sparse_dirs;

ساکن ثابت حرف *سابقہ;
ساکن صحیح max_سابقہ_len;
ساکن صحیح سابقہ_len;
ساکن ساخت pathspec pathspec;
ساکن صحیح error_unmatch;
ساکن حرف *ps_matched;
ساکن ثابت حرف *with_درخت;
ساکن صحیح exc_given;
ساکن صحیح exclude_دلائل;
ساکن ثابت حرف *format;

ساکن ثابت حرف *لیبل_cached = "";
ساکن ثابت حرف *لیبل_unmerged = "";
ساکن ثابت حرف *لیبل_removed = "";
ساکن ثابت حرف *لیبل_other = "";
ساکن ثابت حرف *لیبل_killed = "";
ساکن ثابت حرف *لیبل_modified = "";
ساکن ثابت حرف *لیبل_skip_worktree = "";
ساکن ثابت حرف *لیبل_resolve_undo = "";

ساکن خالی write_eolinfo(ساخت اشاریہ_state *istate,
			  ثابت ساخت cache_اندراج *ce, ثابت حرف *راستہ)
{
	اگر (show_eol) {
		ساخت stat st;
		ثابت حرف *i_txt = "";
		ثابت حرف *w_txt = "";
		ثابت حرف *a_txt = get_convert_attr_ascii(istate, راستہ);
		اگر (ce && S_ISREG(ce->ce_منوال))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		اگر (!lstat(راستہ, &st) && S_ISREG(st.st_منوال))
			w_txt = get_wt_convert_stats_ascii(راستہ);
		لکھو("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

ساکن خالی write_name(ثابت حرف *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, سابقہ_len ? سابقہ : تہی,
				   stdout, سطر_terminator);
}

ساکن خالی write_name_مقصد_buf(ساخت strbuf *sb, ثابت حرف *name)
{
	ساخت strbuf buf = STRBUF_INIT;
	ثابت حرف *rel = relative_راستہ(name, سابقہ_len ? سابقہ : تہی, &buf);

	اگر (سطر_terminator)
		quote_c_style(rel, sb, تہی, 0);
	ورنہ
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

ساکن ثابت حرف *get_لیبل(ثابت ساخت cache_اندراج *ce, ثابت حرف *لیبل)
{
	ساکن حرف alttag[4];

	اگر (لیبل && *لیبل && ((show_valid_ثنائی_ہندسہ && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_ثنائی_ہندسہ && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		حافظہ_نقل(alttag, لیبل, 3);

		اگر (حرف_ہے(لیبل[0])) {
			alttag[0] = tolower(لیبل[0]);
		} ورنہ اگر (لیبل[0] == '?') {
			alttag[0] = '!';
		} ورنہ {
			alttag[0] = 'v';
			alttag[1] = لیبل[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		لیبل = alttag;
	}

	واپس لیبل;
}

ساکن خالی print_debug(ثابت ساخت cache_اندراج *ce)
{
	اگر (debug_منوال) {
		ثابت ساخت stat_ڈیٹا *sd = &ce->ce_stat_ڈیٹا;

		لکھو("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		لکھو("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		لکھو("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		لکھو("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		لکھو("  size: %u\tflags: %x\n", sd->sd_سائز, ce->ce_flags);
	}
}

ساکن خالی show_dir_اندراج(ساخت اشاریہ_state *istate,
			   ثابت حرف *لیبل, ساخت dir_اندراج *ent)
{
	صحیح len = max_سابقہ_len;

	اگر (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	اگر (ps_matched)
		dir_راستہ_مطابقت(istate, ent, &pathspec, len, ps_matched);

	fputs(لیبل, stdout);
	write_eolinfo(istate, تہی, ent->name);
	write_name(ent->name);
}

ساکن خالی show_other_files(ساخت اشاریہ_state *istate,
			     ثابت ساخت dir_struct *dir)
{
	صحیح i;

	برائے (i = 0; i < dir->nr; i++) {
		ساخت dir_اندراج *ent = dir->entries[i];
		اگر (!اشاریہ_name_is_other(istate, ent->name, ent->len))
			جاری;
		show_dir_اندراج(istate, لیبل_other, ent);
	}
}

ساکن خالی show_killed_files(ساخت اشاریہ_state *istate,
			      ثابت ساخت dir_struct *dir)
{
	صحیح i;
	برائے (i = 0; i < dir->nr; i++) {
		ساخت dir_اندراج *ent = dir->entries[i];
		حرف *cp, *sp;
		صحیح pos, len, killed = 0;

		برائے (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			اگر (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = اشاریہ_name_pos(istate, ent->name, ent->len);
				اگر (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				جب (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				اگر (istate->cache_nr <= pos)
					توڑو;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				اگر ((ent->len < len) &&
				    !لڑی_موازنہ(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				توڑو;
			}
			اگر (0 <= اشاریہ_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				توڑو;
			}
		}
		اگر (killed)
			show_dir_اندراج(istate, لیبل_killed, dir->entries[i]);
	}
}

ساکن خالی show_files(ساخت repository *repo, ساخت dir_struct *dir);

ساکن خالی show_submodule(ساخت repository *superproject,
			   ساخت dir_struct *dir, ثابت حرف *راستہ)
{
	ساخت repository subrepo;

	اگر (repo_submodule_init(&subrepo, superproject, راستہ,
				null_oid(superproject->hash_algo)))
		واپس;

	اگر (repo_read_اشاریہ(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

ساکن خالی expand_objectsize(ساخت repository *repo, ساخت strbuf *سطر,
			      ثابت ساخت object_id *oid,
			      ثابت شمار object_type type, غیرعلامتی صحیح padded)
{
	ساکن ثابت حرف padding[] = "       ";
	سائز_t min_len = padded ? لڑی_لمبائی(padding) : 0;
	سائز_t orig_len = سطر->len;
	سائز_t len;

	اگر (type == OBJ_BLOB) {
		سائز_t سائز;
		اگر (odb_read_object_info(repo->objects, oid, &سائز) < 0)
			die(_("could not get object info about '%s'"),
			    oid_مقصد_hex(oid));
		strbuf_جوڑو_uint(سطر, سائز);
	} ورنہ {
		strbuf_addstr(سطر, "-");
	}
	len = سطر->len - orig_len;
	اگر (len < min_len)
		strbuf_داخل_کرو(سطر, orig_len, padding, min_len - len);
}

ساکن خالی show_ce_fmt(ساخت repository *repo, ثابت ساخت cache_اندراج *ce,
			ثابت حرف *format, ثابت حرف *fullname) {
	ساخت strbuf sb = STRBUF_INIT;

	جب (strbuf_expand_قدم(&sb, &format)) {
		سائز_t len;
		ساخت stat st;

		اگر (skip_سابقہ(format, "%", &format))
			strbuf_addch(&sb, '%');
		ورنہ اگر ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		ورنہ اگر (skip_سابقہ(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_منوال);
		ورنہ اگر (skip_سابقہ(format, "(objectname)", &format))
			strbuf_جوڑو_unique_abbrev(&sb, &ce->oid, abbrev);
		ورنہ اگر (skip_سابقہ(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_منوال)));
		ورنہ اگر (skip_سابقہ(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_منوال), 1);
		ورنہ اگر (skip_سابقہ(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_منوال), 0);
		ورنہ اگر (skip_سابقہ(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		ورنہ اگر (skip_سابقہ(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_منوال) ?
				      get_cached_convert_stats_ascii(repo->اشاریہ,
				      ce->name) : "");
		ورنہ اگر (skip_سابقہ(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_منوال) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		ورنہ اگر (skip_سابقہ(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->اشاریہ,
								 fullname));
		ورنہ اگر (skip_سابقہ(format, "(path)", &format))
			write_name_مقصد_buf(&sb, fullname);
		ورنہ
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, سطر_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

ساکن خالی show_ce(ساخت repository *repo, ساخت dir_struct *dir,
		    ثابت ساخت cache_اندراج *ce, ثابت حرف *fullname,
		    ثابت حرف *لیبل)
{
	اگر (max_سابقہ_len > لڑی_لمبائی(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	اگر (recurse_submodules && S_ISGITLINK(ce->ce_منوال) &&
	    is_submodule_فعال(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} ورنہ اگر (مطابقت_pathspec(repo->اشاریہ, &pathspec, fullname, لڑی_لمبائی(fullname),
				  max_سابقہ_len, ps_matched,
				  S_ISDIR(ce->ce_منوال) ||
				  S_ISGITLINK(ce->ce_منوال))) {
		اگر (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			واپس;
		}

		لیبل = get_لیبل(ce, لیبل);

		اگر (!show_stage) {
			fputs(لیبل, stdout);
		} ورنہ {
			لکھو("%s%06o %s %d\t",
			       لیبل,
			       ce->ce_منوال,
			       repo_ڈھونڈو_unique_abbrev(repo, &ce->oid, abbrev),
			       ce_stage(ce));
		}
		write_eolinfo(repo->اشاریہ, ce, fullname);
		write_name(fullname);
		print_debug(ce);
	}
}

ساکن خالی show_ru_info(ساخت repository *repo, ساخت اشاریہ_state *istate)
{
	ساخت string_list_شے *شے;

	اگر (!istate->resolve_undo)
		واپس;

	for_each_string_list_شے(شے, istate->resolve_undo) {
		ثابت حرف *راستہ = شے->string;
		ساخت resolve_undo_info *ui = شے->util;
		صحیح i, len;

		len = لڑی_لمبائی(راستہ);
		اگر (len < max_سابقہ_len)
			جاری; /* outside of the prefix */
		اگر (!مطابقت_pathspec(istate, &pathspec, راستہ, len,
				    max_سابقہ_len, ps_matched, 0))
			جاری; /* uninterested */
		برائے (i = 0; i < 3; i++) {
			اگر (!ui->منوال[i])
				جاری;
			لکھو("%s%06o %s %d\t", لیبل_resolve_undo, ui->منوال[i],
			       repo_ڈھونڈو_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(راستہ);
		}
	}
}

ساکن صحیح ce_excluded(ساخت dir_struct *dir, ساخت اشاریہ_state *istate,
		       ثابت حرف *fullname, ثابت ساخت cache_اندراج *ce)
{
	صحیح dtype = ce_مقصد_dtype(ce);
	واپس is_excluded(dir, istate, fullname, &dtype);
}

ساکن خالی construct_fullname(ساخت strbuf *برآمد, ثابت ساخت repository *repo,
			       ثابت ساخت cache_اندراج *ce)
{
	strbuf_reset(برآمد);
	اگر (repo->submodule_سابقہ)
		strbuf_addstr(برآمد, repo->submodule_سابقہ);
	strbuf_addstr(برآمد, ce->name);
}

ساکن خالی show_files(ساخت repository *repo, ساخت dir_struct *dir)
{
	صحیح i;
	ساخت strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	اگر (show_others || show_killed) {
		اگر (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->اشاریہ, &pathspec);
		اگر (show_others)
			show_other_files(repo->اشاریہ, dir);
		اگر (show_killed)
			show_killed_files(repo->اشاریہ, dir);
	}

	اگر (!(show_cached || show_stage || show_deleted || show_modified))
		واپس;

	برائے (i = 0; i < repo->اشاریہ->cache_nr; i++) {
		ثابت ساخت cache_اندراج *ce = repo->اشاریہ->cache[i];
		ساخت stat st;
		صحیح stat_err;

		اگر (S_ISSPARSEDIR(ce->ce_منوال) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_بھرا_اشاریہ(repo->اشاریہ);
			ce = repo->اشاریہ->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		اگر ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->اشاریہ, fullname.buf, ce))
			جاری;
		اگر (ce->ce_flags & CE_UPDATE)
			جاری;
		اگر ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? لیبل_unmerged :
				(ce_skip_worktree(ce) ? لیبل_skip_worktree :
				 لیبل_cached));
			اگر (skipping_duplicates)
				جاؤ skip_مقصد_اگلا_name;
		}

		اگر (!(show_deleted || show_modified))
			جاری;
		اگر (ce_skip_worktree(ce))
			جاری;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		اگر (pathspec.nr == 1 &&
		    !مطابقت_pathspec(repo->اشاریہ, &pathspec, fullname.buf,
				    fullname.len, max_سابقہ_len, تہی,
				    S_ISDIR(ce->ce_منوال) ||
				    S_ISGITLINK(ce->ce_منوال)))
			جاری;
		stat_err = lstat(fullname.buf, &st);
		اگر (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		اگر (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, لیبل_removed);
			اگر (skipping_duplicates)
				جاؤ skip_مقصد_اگلا_name;
		}
		اگر (show_modified &&
		    (stat_err || ie_modified(repo->اشاریہ, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, لیبل_modified);
			اگر (skipping_duplicates)
				جاؤ skip_مقصد_اگلا_name;
		}
		جاری;

skip_مقصد_اگلا_name:
		{
			صحیح j;
			ساخت cache_اندراج **cache = repo->اشاریہ->cache;
			برائے (j = i + 1; j < repo->اشاریہ->cache_nr; j++)
				اگر (strcmp(ce->name, cache[j]->name))
					توڑو;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
ساکن خالی prune_اشاریہ(ساخت اشاریہ_state *istate,
			ثابت حرف *سابقہ, سائز_t prefixlen)
{
	صحیح pos;
	غیرعلامتی صحیح پہلا, آخری;

	اگر (!سابقہ || !istate->cache_nr)
		واپس;
	pos = اشاریہ_name_pos(istate, سابقہ, prefixlen);
	اگر (pos < 0)
		pos = -pos-1;
	پہلا = pos;
	آخری = istate->cache_nr;
	جب (آخری > پہلا) {
		صحیح اگلا = پہلا + ((آخری - پہلا) >> 1);
		ثابت ساخت cache_اندراج *ce = istate->cache[اگلا];
		اگر (!لڑی_موازنہ(ce->name, سابقہ, prefixlen)) {
			پہلا = اگلا+1;
			جاری;
		}
		آخری = اگلا;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, آخری - pos);
	istate->cache_nr = آخری - pos;
}

ساکن صحیح get_common_سابقہ_len(ثابت حرف *common_سابقہ)
{
	صحیح common_سابقہ_len;

	اگر (!common_سابقہ)
		واپس 0;

	common_سابقہ_len = لڑی_لمبائی(common_سابقہ);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	اگر (common_سابقہ[common_سابقہ_len - 1] == '/')
		common_سابقہ_len--;

	واپس common_سابقہ_len;
}

ساکن ثابت حرف * ثابت ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	تہی
};

ساکن صحیح option_parse_exclude(ثابت ساخت option *opt,
				ثابت حرف *arg, صحیح unset)
{
	ساخت string_list *exclude_list = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	واپس 0;
}

ساکن صحیح option_parse_exclude_from(ثابت ساخت option *opt,
				     ثابت حرف *arg, صحیح unset)
{
	ساخت dir_struct *dir = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	جوڑو_patterns_from_file(dir, arg);

	واپس 0;
}

ساکن صحیح option_parse_exclude_standard(ثابت ساخت option *opt,
					 ثابت حرف *arg, صحیح unset)
{
	ساخت dir_struct *dir = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	تیاری_standard_excludes(dir);

	واپس 0;
}

صحیح cmd_ls_files(صحیح argc,
		 ثابت حرف **argv,
		 ثابت حرف *cmd_سابقہ,
		 ساخت repository *repo)
{
	صحیح require_work_درخت = 0, show_لیبل = 0, i;
	حرف *max_سابقہ;
	ساخت dir_struct dir = DIR_INIT;
	ساخت نمونہ_list *pl;
	ساخت string_list exclude_list = STRING_LIST_INIT_NODUP;
	ساخت option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_سیٹ_INT('z', تہی, &سطر_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', تہی, &show_لیبل,
			N_("identify the file status with tags")),
		OPT_BOOL('v', تہی, &show_valid_ثنائی_ہندسہ,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', تہی, &show_fsmonitor_ثنائی_ہندسہ,
			N_("use lowercase letters for 'fsmonitor clean' files")),
		OPT_BOOL('c', "cached", &show_cached,
			N_("show cached files in the output (default)")),
		OPT_BOOL('d', "deleted", &show_deleted,
			N_("show deleted files in the output")),
		OPT_BOOL('m', "modified", &show_modified,
			N_("show modified files in the output")),
		OPT_BOOL('o', "others", &show_others,
			N_("show other files in the output")),
		OPT_ثنائی_ہندسہ('i', "ignored", &dir.flags,
			N_("show ignored files in the output"),
			DIR_SHOW_IGNORED),
		OPT_BOOL('s', "stage", &show_stage,
			N_("show staged contents' object name in the output")),
		OPT_BOOL('k', "killed", &show_killed,
			N_("show files on the filesystem that need to be removed")),
		OPT_ثنائی_ہندسہ(0, "directory", &dir.flags,
			N_("show 'other' directories' names only"),
			DIR_SHOW_OTHER_DIRECTORIES),
		OPT_BOOL(0, "eol", &show_eol, N_("show line endings of files")),
		OPT_NEGBIT(0, "empty-directory", &dir.flags,
			N_("don't show empty directories"),
			DIR_HIDE_خالی_خانے_DIRECTORIES),
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
		OPT_STRING(0, "exclude-per-directory", &dir.exclude_فی_dir, N_("file"),
			N_("read additional per-directory exclude patterns in <file>")),
		OPT_CALLBACK_F(0, "exclude-standard", &dir, تہی,
			N_("add the standard git exclusions"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			option_parse_exclude_standard),
		OPT_سیٹ_INT_F(0, "full-name", &سابقہ_len,
			      N_("make the output relative to the project top directory"),
			      0, PARSE_OPT_NONEG),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			N_("recurse through submodules")),
		OPT_BOOL(0, "error-unmatch", &error_unmatch,
			N_("if any <file> is not in the index, treat this as an error")),
		OPT_STRING(0, "with-tree", &with_درخت, N_("tree-ish"),
			N_("pretend that paths removed since <tree-ish> are still present")),
		OPT__ABBREV(&abbrev),
		OPT_BOOL(0, "debug", &debug_منوال, N_("show debugging data")),
		OPT_BOOL(0, "deduplicate", &skipping_duplicates,
			 N_("suppress duplicate entries")),
		OPT_BOOL(0, "sparse", &show_sparse_dirs,
			 N_("show sparse directories in the presence of a sparse index")),
		OPT_STRING_F(0, "format", &format, N_("format"),
			     N_("format to use for the output"),
			     PARSE_OPT_NONEG),
		OPT_انتہا()
	};
	صحیح ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_بھرا_اشاریہ = 0;

	سابقہ = cmd_سابقہ;
	اگر (سابقہ)
		سابقہ_len = لڑی_لمبائی(سابقہ);
	repo_config(repo, git_default_config, تہی);

	اگر (repo_read_اشاریہ(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, سابقہ, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = جوڑو_نمونہ_list(&dir, EXC_CMDL, "--exclude option");
	برائے (i = 0; i < exclude_list.nr; i++) {
		جوڑو_نمونہ(exclude_list.اشیاء[i].string, "", 0, pl, --exclude_دلائل);
	}

	اگر (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_لیبل))
			usage_پیغام_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	اگر (show_لیبل || show_valid_ثنائی_ہندسہ || show_fsmonitor_ثنائی_ہندسہ) {
		لیبل_cached = "H ";
		لیبل_unmerged = "M ";
		لیبل_removed = "R ";
		لیبل_modified = "C ";
		لیبل_other = "? ";
		لیبل_killed = "K ";
		لیبل_skip_worktree = "S ";
		لیبل_resolve_undo = "U ";
	}
	اگر (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_درخت = 1;
	اگر (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	اگر (show_لیبل || show_stage)
		skipping_duplicates = 0;
	اگر (dir.exclude_فی_dir)
		exc_given = 1;

	اگر (require_work_درخت && !is_inside_work_درخت(repo))
		تیاری_work_درخت(repo);

	اگر (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_درخت))
		die("ls-files --recurse-submodules unsupported mode");

	اگر (recurse_submodules && error_unmatch)
		die("ls-files --recurse-submodules does not support "
		    "--error-unmatch");

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       سابقہ, argv);

	/*
	 * Find common prefix for all pathspec's
	 * This is used as a performance optimization which unfortunately cannot
	 * be done when recursing into submodules because when a pathspec is
	 * given which spans repository boundaries you can't simply remove the
	 * submodule entry because the pathspec may match something inside the
	 * submodule.
	 */
	اگر (recurse_submodules)
		max_سابقہ = تہی;
	ورنہ
		max_سابقہ = common_سابقہ(&pathspec);
	max_سابقہ_len = get_common_سابقہ_len(max_سابقہ);

	prune_اشاریہ(repo->اشاریہ, max_سابقہ, max_سابقہ_len);

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
		overlay_درخت_on_اشاریہ(repo->اشاریہ, with_درخت, max_سابقہ);
	}

	show_files(repo, &dir);

	اگر (show_resolve_undo)
		show_ru_info(repo, repo->اشاریہ);

	اگر (ps_matched && report_راستہ_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	آزاد(max_سابقہ);
	واپس ret;
}
