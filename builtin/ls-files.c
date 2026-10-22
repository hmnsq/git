/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "convert.h"
#ضمن "environment.h"
#ضمن "quote.h"
#ضمن "dir.h"
#ضمن "gettext.h"
#ضمن "object-name.h"
#ضمن "strbuf.h"
#ضمن "parse-options.h"
#ضمن "resolve-undo.h"
#ضمن "string-list.h"
#ضمن "path.h"
#ضمن "pathspec.h"
#ضمن "read-cache.h"
#ضمن "setup.h"
#ضمن "sparse-index.h"
#ضمن "submodule.h"
#ضمن "odb.h"
#ضمن "hex.h"


ساكن صحيح abbrev;
ساكن صحيح show_deleted;
ساكن صحيح show_cached;
ساكن صحيح show_others;
ساكن صحيح show_stage;
ساكن صحيح show_unmerged;
ساكن صحيح show_resolve_undo;
ساكن صحيح show_modified;
ساكن صحيح show_killed;
ساكن صحيح show_valid_ثنائی_ہندسہ;
ساكن صحيح show_fsmonitor_ثنائی_ہندسہ;
ساكن صحيح سطر_terminator = '\n';
ساكن صحيح debug_منوال;
ساكن صحيح show_eol;
ساكن صحيح recurse_submodules;
ساكن صحيح skipping_duplicates;
ساكن صحيح show_sparse_dirs;

ساكن ثابت حرف *اگیتر;
ساكن صحيح max_اگیتر_len;
ساكن صحيح اگیتر_len;
ساكن بنية pathspec pathspec;
ساكن صحيح error_unmatch;
ساكن حرف *ps_matched;
ساكن ثابت حرف *with_درخت;
ساكن صحيح exc_given;
ساكن صحيح exclude_دلیلاں;
ساكن ثابت حرف *format;

ساكن ثابت حرف *ٹیگ_cached = "";
ساكن ثابت حرف *ٹیگ_unmerged = "";
ساكن ثابت حرف *ٹیگ_removed = "";
ساكن ثابت حرف *ٹیگ_other = "";
ساكن ثابت حرف *ٹیگ_killed = "";
ساكن ثابت حرف *ٹیگ_modified = "";
ساكن ثابت حرف *ٹیگ_skip_worktree = "";
ساكن ثابت حرف *ٹیگ_resolve_undo = "";

ساكن فراغ write_eolinfo(بنية اشاریہ_state *istate,
			  ثابت بنية cache_اندراج *ce, ثابت حرف *رستہ)
{
	إذا (show_eol) {
		بنية stat st;
		ثابت حرف *i_txt = "";
		ثابت حرف *w_txt = "";
		ثابت حرف *a_txt = get_convert_attr_ascii(istate, رستہ);
		إذا (ce && S_ISREG(ce->ce_منوال))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		إذا (!lstat(رستہ, &st) && S_ISREG(st.st_منوال))
			w_txt = get_wt_convert_stats_ascii(رستہ);
		لکھو("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

ساكن فراغ write_name(ثابت حرف *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, اگیتر_len ? اگیتر : عدم,
				   stdout, سطر_terminator);
}

ساكن فراغ write_name_منزل_buf(بنية strbuf *sb, ثابت حرف *name)
{
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *rel = relative_رستہ(name, اگیتر_len ? اگیتر : عدم, &buf);

	إذا (سطر_terminator)
		quote_c_style(rel, sb, عدم, 0);
	وإلا
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

ساكن ثابت حرف *get_ٹیگ(ثابت بنية cache_اندراج *ce, ثابت حرف *ٹیگ)
{
	ساكن حرف alttag[4];

	إذا (ٹیگ && *ٹیگ && ((show_valid_ثنائی_ہندسہ && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_ثنائی_ہندسہ && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		حافظہ_نقل(alttag, ٹیگ, 3);

		إذا (حرف_اے(ٹیگ[0])) {
			alttag[0] = tolower(ٹیگ[0]);
		} وإلا إذا (ٹیگ[0] == '?') {
			alttag[0] = '!';
		} وإلا {
			alttag[0] = 'v';
			alttag[1] = ٹیگ[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		ٹیگ = alttag;
	}

	أرجع ٹیگ;
}

ساكن فراغ print_debug(ثابت بنية cache_اندراج *ce)
{
	إذا (debug_منوال) {
		ثابت بنية stat_انکڑے *sd = &ce->ce_stat_انکڑے;

		لکھو("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		لکھو("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		لکھو("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		لکھو("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		لکھو("  size: %u\tflags: %x\n", sd->sd_ناپ, ce->ce_flags);
	}
}

ساكن فراغ show_dir_اندراج(بنية اشاریہ_state *istate,
			   ثابت حرف *ٹیگ, بنية dir_اندراج *ent)
{
	صحيح len = max_اگیتر_len;

	إذا (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	إذا (ps_matched)
		dir_رستہ_میل(istate, ent, &pathspec, len, ps_matched);

	fputs(ٹیگ, stdout);
	write_eolinfo(istate, عدم, ent->name);
	write_name(ent->name);
}

ساكن فراغ show_other_files(بنية اشاریہ_state *istate,
			     ثابت بنية dir_struct *dir)
{
	صحيح i;

	لكل (i = 0; i < dir->nr; i++) {
		بنية dir_اندراج *ent = dir->entries[i];
		إذا (!اشاریہ_name_is_other(istate, ent->name, ent->len))
			استمر;
		show_dir_اندراج(istate, ٹیگ_other, ent);
	}
}

ساكن فراغ show_killed_files(بنية اشاریہ_state *istate,
			      ثابت بنية dir_struct *dir)
{
	صحيح i;
	لكل (i = 0; i < dir->nr; i++) {
		بنية dir_اندراج *ent = dir->entries[i];
		حرف *cp, *sp;
		صحيح pos, len, killed = 0;

		لكل (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			إذا (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = اشاریہ_name_pos(istate, ent->name, ent->len);
				إذا (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				طالما (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				إذا (istate->cache_nr <= pos)
					اكسر;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				إذا ((ent->len < len) &&
				    !لڑی_مقابلہ(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				اكسر;
			}
			إذا (0 <= اشاریہ_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				اكسر;
			}
		}
		إذا (killed)
			show_dir_اندراج(istate, ٹیگ_killed, dir->entries[i]);
	}
}

ساكن فراغ show_files(بنية repository *repo, بنية dir_struct *dir);

ساكن فراغ show_submodule(بنية repository *superproject,
			   بنية dir_struct *dir, ثابت حرف *رستہ)
{
	بنية repository subrepo;

	إذا (repo_submodule_init(&subrepo, superproject, رستہ,
				null_oid(superproject->hash_algo)))
		أرجع;

	إذا (repo_read_اشاریہ(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

ساكن فراغ expand_objectsize(بنية repository *repo, بنية strbuf *سطر,
			      ثابت بنية object_id *oid,
			      ثابت تعداد object_type type, بنانشانی صحيح padded)
{
	ساكن ثابت حرف padding[] = "       ";
	ناپ_t min_len = padded ? لڑی_لمبائی(padding) : 0;
	ناپ_t orig_len = سطر->len;
	ناپ_t len;

	إذا (type == OBJ_BLOB) {
		ناپ_t ناپ;
		إذا (odb_read_object_info(repo->objects, oid, &ناپ) < 0)
			die(_("could not get object info about '%s'"),
			    oid_منزل_hex(oid));
		strbuf_جوڑو_uint(سطر, ناپ);
	} وإلا {
		strbuf_addstr(سطر, "-");
	}
	len = سطر->len - orig_len;
	إذا (len < min_len)
		strbuf_داخل_کرو(سطر, orig_len, padding, min_len - len);
}

ساكن فراغ show_ce_fmt(بنية repository *repo, ثابت بنية cache_اندراج *ce,
			ثابت حرف *format, ثابت حرف *fullname) {
	بنية strbuf sb = STRBUF_INIT;

	طالما (strbuf_expand_قدم(&sb, &format)) {
		ناپ_t len;
		بنية stat st;

		إذا (skip_اگیتر(format, "%", &format))
			strbuf_addch(&sb, '%');
		وإلا إذا ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		وإلا إذا (skip_اگیتر(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_منوال);
		وإلا إذا (skip_اگیتر(format, "(objectname)", &format))
			strbuf_جوڑو_unique_abbrev(&sb, &ce->oid, abbrev);
		وإلا إذا (skip_اگیتر(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_منوال)));
		وإلا إذا (skip_اگیتر(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_منوال), 1);
		وإلا إذا (skip_اگیتر(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_منوال), 0);
		وإلا إذا (skip_اگیتر(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		وإلا إذا (skip_اگیتر(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_منوال) ?
				      get_cached_convert_stats_ascii(repo->اشاریہ,
				      ce->name) : "");
		وإلا إذا (skip_اگیتر(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_منوال) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		وإلا إذا (skip_اگیتر(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->اشاریہ,
								 fullname));
		وإلا إذا (skip_اگیتر(format, "(path)", &format))
			write_name_منزل_buf(&sb, fullname);
		وإلا
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, سطر_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

ساكن فراغ show_ce(بنية repository *repo, بنية dir_struct *dir,
		    ثابت بنية cache_اندراج *ce, ثابت حرف *fullname,
		    ثابت حرف *ٹیگ)
{
	إذا (max_اگیتر_len > لڑی_لمبائی(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	إذا (recurse_submodules && S_ISGITLINK(ce->ce_منوال) &&
	    is_submodule_سرگرم(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} وإلا إذا (میل_pathspec(repo->اشاریہ, &pathspec, fullname, لڑی_لمبائی(fullname),
				  max_اگیتر_len, ps_matched,
				  S_ISDIR(ce->ce_منوال) ||
				  S_ISGITLINK(ce->ce_منوال))) {
		إذا (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			أرجع;
		}

		ٹیگ = get_ٹیگ(ce, ٹیگ);

		إذا (!show_stage) {
			fputs(ٹیگ, stdout);
		} وإلا {
			لکھو("%s%06o %s %d\t",
			       ٹیگ,
			       ce->ce_منوال,
			       repo_لبھو_unique_abbrev(repo, &ce->oid, abbrev),
			       ce_stage(ce));
		}
		write_eolinfo(repo->اشاریہ, ce, fullname);
		write_name(fullname);
		print_debug(ce);
	}
}

ساكن فراغ show_ru_info(بنية repository *repo, بنية اشاریہ_state *istate)
{
	بنية string_list_شے *شے;

	إذا (!istate->resolve_undo)
		أرجع;

	for_each_string_list_شے(شے, istate->resolve_undo) {
		ثابت حرف *رستہ = شے->string;
		بنية resolve_undo_info *ui = شے->util;
		صحيح i, len;

		len = لڑی_لمبائی(رستہ);
		إذا (len < max_اگیتر_len)
			استمر; /* outside of the prefix */
		إذا (!میل_pathspec(istate, &pathspec, رستہ, len,
				    max_اگیتر_len, ps_matched, 0))
			استمر; /* uninterested */
		لكل (i = 0; i < 3; i++) {
			إذا (!ui->منوال[i])
				استمر;
			لکھو("%s%06o %s %d\t", ٹیگ_resolve_undo, ui->منوال[i],
			       repo_لبھو_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(رستہ);
		}
	}
}

ساكن صحيح ce_excluded(بنية dir_struct *dir, بنية اشاریہ_state *istate,
		       ثابت حرف *fullname, ثابت بنية cache_اندراج *ce)
{
	صحيح dtype = ce_منزل_dtype(ce);
	أرجع is_excluded(dir, istate, fullname, &dtype);
}

ساكن فراغ construct_fullname(بنية strbuf *باہرلا, ثابت بنية repository *repo,
			       ثابت بنية cache_اندراج *ce)
{
	strbuf_reset(باہرلا);
	إذا (repo->submodule_اگیتر)
		strbuf_addstr(باہرلا, repo->submodule_اگیتر);
	strbuf_addstr(باہرلا, ce->name);
}

ساكن فراغ show_files(بنية repository *repo, بنية dir_struct *dir)
{
	صحيح i;
	بنية strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	إذا (show_others || show_killed) {
		إذا (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->اشاریہ, &pathspec);
		إذا (show_others)
			show_other_files(repo->اشاریہ, dir);
		إذا (show_killed)
			show_killed_files(repo->اشاریہ, dir);
	}

	إذا (!(show_cached || show_stage || show_deleted || show_modified))
		أرجع;

	لكل (i = 0; i < repo->اشاریہ->cache_nr; i++) {
		ثابت بنية cache_اندراج *ce = repo->اشاریہ->cache[i];
		بنية stat st;
		صحيح stat_err;

		إذا (S_ISSPARSEDIR(ce->ce_منوال) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_بھریا_اشاریہ(repo->اشاریہ);
			ce = repo->اشاریہ->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		إذا ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->اشاریہ, fullname.buf, ce))
			استمر;
		إذا (ce->ce_flags & CE_UPDATE)
			استمر;
		إذا ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? ٹیگ_unmerged :
				(ce_skip_worktree(ce) ? ٹیگ_skip_worktree :
				 ٹیگ_cached));
			إذا (skipping_duplicates)
				اذهب skip_منزل_اگلا_name;
		}

		إذا (!(show_deleted || show_modified))
			استمر;
		إذا (ce_skip_worktree(ce))
			استمر;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		إذا (pathspec.nr == 1 &&
		    !میل_pathspec(repo->اشاریہ, &pathspec, fullname.buf,
				    fullname.len, max_اگیتر_len, عدم,
				    S_ISDIR(ce->ce_منوال) ||
				    S_ISGITLINK(ce->ce_منوال)))
			استمر;
		stat_err = lstat(fullname.buf, &st);
		إذا (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		إذا (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, ٹیگ_removed);
			إذا (skipping_duplicates)
				اذهب skip_منزل_اگلا_name;
		}
		إذا (show_modified &&
		    (stat_err || ie_modified(repo->اشاریہ, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, ٹیگ_modified);
			إذا (skipping_duplicates)
				اذهب skip_منزل_اگلا_name;
		}
		استمر;

skip_منزل_اگلا_name:
		{
			صحيح j;
			بنية cache_اندراج **cache = repo->اشاریہ->cache;
			لكل (j = i + 1; j < repo->اشاریہ->cache_nr; j++)
				إذا (strcmp(ce->name, cache[j]->name))
					اكسر;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
ساكن فراغ prune_اشاریہ(بنية اشاریہ_state *istate,
			ثابت حرف *اگیتر, ناپ_t prefixlen)
{
	صحيح pos;
	بنانشانی صحيح پہلا, آخری;

	إذا (!اگیتر || !istate->cache_nr)
		أرجع;
	pos = اشاریہ_name_pos(istate, اگیتر, prefixlen);
	إذا (pos < 0)
		pos = -pos-1;
	پہلا = pos;
	آخری = istate->cache_nr;
	طالما (آخری > پہلا) {
		صحيح اگلا = پہلا + ((آخری - پہلا) >> 1);
		ثابت بنية cache_اندراج *ce = istate->cache[اگلا];
		إذا (!لڑی_مقابلہ(ce->name, اگیتر, prefixlen)) {
			پہلا = اگلا+1;
			استمر;
		}
		آخری = اگلا;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, آخری - pos);
	istate->cache_nr = آخری - pos;
}

ساكن صحيح get_common_اگیتر_len(ثابت حرف *common_اگیتر)
{
	صحيح common_اگیتر_len;

	إذا (!common_اگیتر)
		أرجع 0;

	common_اگیتر_len = لڑی_لمبائی(common_اگیتر);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	إذا (common_اگیتر[common_اگیتر_len - 1] == '/')
		common_اگیتر_len--;

	أرجع common_اگیتر_len;
}

ساكن ثابت حرف * ثابت ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	عدم
};

ساكن صحيح option_parse_exclude(ثابت بنية option *opt,
				ثابت حرف *arg, صحيح unset)
{
	بنية string_list *exclude_list = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	أرجع 0;
}

ساكن صحيح option_parse_exclude_from(ثابت بنية option *opt,
				     ثابت حرف *arg, صحيح unset)
{
	بنية dir_struct *dir = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	جوڑو_patterns_from_file(dir, arg);

	أرجع 0;
}

ساكن صحيح option_parse_exclude_standard(ثابت بنية option *opt,
					 ثابت حرف *arg, صحيح unset)
{
	بنية dir_struct *dir = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	تیاری_standard_excludes(dir);

	أرجع 0;
}

صحيح cmd_ls_files(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *cmd_اگیتر,
		 بنية repository *repo)
{
	صحيح require_work_درخت = 0, show_ٹیگ = 0, i;
	حرف *max_اگیتر;
	بنية dir_struct dir = DIR_INIT;
	بنية نمونہ_list *pl;
	بنية string_list exclude_list = STRING_LIST_INIT_NODUP;
	بنية option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_سیٹ_INT('z', عدم, &سطر_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', عدم, &show_ٹیگ,
			N_("identify the file status with tags")),
		OPT_BOOL('v', عدم, &show_valid_ثنائی_ہندسہ,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', عدم, &show_fsmonitor_ثنائی_ہندسہ,
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
			DIR_HIDE_خالی_تھاواں_DIRECTORIES),
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
		OPT_CALLBACK_F(0, "exclude-standard", &dir, عدم,
			N_("add the standard git exclusions"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			option_parse_exclude_standard),
		OPT_سیٹ_INT_F(0, "full-name", &اگیتر_len,
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
		OPT_سرا()
	};
	صحيح ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_بھریا_اشاریہ = 0;

	اگیتر = cmd_اگیتر;
	إذا (اگیتر)
		اگیتر_len = لڑی_لمبائی(اگیتر);
	repo_config(repo, git_default_config, عدم);

	إذا (repo_read_اشاریہ(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, اگیتر, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = جوڑو_نمونہ_list(&dir, EXC_CMDL, "--exclude option");
	لكل (i = 0; i < exclude_list.nr; i++) {
		جوڑو_نمونہ(exclude_list.اشیاء[i].string, "", 0, pl, --exclude_دلیلاں);
	}

	إذا (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_ٹیگ))
			usage_پیغام_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	إذا (show_ٹیگ || show_valid_ثنائی_ہندسہ || show_fsmonitor_ثنائی_ہندسہ) {
		ٹیگ_cached = "H ";
		ٹیگ_unmerged = "M ";
		ٹیگ_removed = "R ";
		ٹیگ_modified = "C ";
		ٹیگ_other = "? ";
		ٹیگ_killed = "K ";
		ٹیگ_skip_worktree = "S ";
		ٹیگ_resolve_undo = "U ";
	}
	إذا (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_درخت = 1;
	إذا (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	إذا (show_ٹیگ || show_stage)
		skipping_duplicates = 0;
	إذا (dir.exclude_فی_dir)
		exc_given = 1;

	إذا (require_work_درخت && !is_inside_work_درخت(repo))
		تیاری_work_درخت(repo);

	إذا (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_درخت))
		die("ls-files --recurse-submodules unsupported mode");

	إذا (recurse_submodules && error_unmatch)
		die("ls-files --recurse-submodules does not support "
		    "--error-unmatch");

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       اگیتر, argv);

	/*
	 * Find common prefix for all pathspec's
	 * This is used as a performance optimization which unfortunately cannot
	 * be done when recursing into submodules because when a pathspec is
	 * given which spans repository boundaries you can't simply remove the
	 * submodule entry because the pathspec may match something inside the
	 * submodule.
	 */
	إذا (recurse_submodules)
		max_اگیتر = عدم;
	وإلا
		max_اگیتر = common_اگیتر(&pathspec);
	max_اگیتر_len = get_common_اگیتر_len(max_اگیتر);

	prune_اشاریہ(repo->اشاریہ, max_اگیتر, max_اگیتر_len);

	/* Treat unmatching pathspec elements as errors */
	إذا (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	إذا ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	إذا ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	إذا (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	إذا (with_درخت) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		إذا (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_درخت_on_اشاریہ(repo->اشاریہ, with_درخت, max_اگیتر);
	}

	show_files(repo, &dir);

	إذا (show_resolve_undo)
		show_ru_info(repo, repo->اشاریہ);

	إذا (ps_matched && report_رستہ_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	حرر(max_اگیتر);
	أرجع ret;
}
