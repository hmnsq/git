/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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
ساكن صحيح show_valid_ٻٽو_انگ;
ساكن صحيح show_fsmonitor_ٻٽو_انگ;
ساكن صحيح سٽ_terminator = '\n';
ساكن صحيح debug_گهڻائي;
ساكن صحيح show_eol;
ساكن صحيح recurse_submodules;
ساكن صحيح skipping_duplicates;
ساكن صحيح show_sparse_dirs;

ساكن ثابت حرف *اڳياڙي;
ساكن صحيح max_اڳياڙي_len;
ساكن صحيح اڳياڙي_len;
ساكن بنية pathspec pathspec;
ساكن صحيح error_unmatch;
ساكن حرف *ps_matched;
ساكن ثابت حرف *with_وڻ;
ساكن صحيح exc_given;
ساكن صحيح exclude_دليلون;
ساكن ثابت حرف *format;

ساكن ثابت حرف *ٽيگ_cached = "";
ساكن ثابت حرف *ٽيگ_unmerged = "";
ساكن ثابت حرف *ٽيگ_removed = "";
ساكن ثابت حرف *ٽيگ_other = "";
ساكن ثابت حرف *ٽيگ_killed = "";
ساكن ثابت حرف *ٽيگ_modified = "";
ساكن ثابت حرف *ٽيگ_skip_worktree = "";
ساكن ثابت حرف *ٽيگ_resolve_undo = "";

ساكن فراغ write_eolinfo(بنية اشاريو_state *istate,
			  ثابت بنية cache_داخلا *ce, ثابت حرف *رستو)
{
	إذا (show_eol) {
		بنية stat st;
		ثابت حرف *i_txt = "";
		ثابت حرف *w_txt = "";
		ثابت حرف *a_txt = get_convert_attr_ascii(istate, رستو);
		إذا (ce && S_ISREG(ce->ce_گهڻائي))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		إذا (!lstat(رستو, &st) && S_ISREG(st.st_گهڻائي))
			w_txt = get_wt_convert_stats_ascii(رستو);
		لکو("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

ساكن فراغ write_name(ثابت حرف *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, اڳياڙي_len ? اڳياڙي : عدم,
				   stdout, سٽ_terminator);
}

ساكن فراغ write_name_منزل_buf(بنية strbuf *sb, ثابت حرف *name)
{
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *rel = relative_رستو(name, اڳياڙي_len ? اڳياڙي : عدم, &buf);

	إذا (سٽ_terminator)
		quote_c_style(rel, sb, عدم, 0);
	وإلا
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

ساكن ثابت حرف *get_ٽيگ(ثابت بنية cache_داخلا *ce, ثابت حرف *ٽيگ)
{
	ساكن حرف alttag[4];

	إذا (ٽيگ && *ٽيگ && ((show_valid_ٻٽو_انگ && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_ٻٽو_انگ && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		يادگيري_نقل(alttag, ٽيگ, 3);

		إذا (اکر_آهي(ٽيگ[0])) {
			alttag[0] = tolower(ٽيگ[0]);
		} وإلا إذا (ٽيگ[0] == '?') {
			alttag[0] = '!';
		} وإلا {
			alttag[0] = 'v';
			alttag[1] = ٽيگ[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		ٽيگ = alttag;
	}

	أرجع ٽيگ;
}

ساكن فراغ print_debug(ثابت بنية cache_داخلا *ce)
{
	إذا (debug_گهڻائي) {
		ثابت بنية stat_معلومات *sd = &ce->ce_stat_معلومات;

		لکو("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		لکو("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		لکو("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		لکو("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		لکو("  size: %u\tflags: %x\n", sd->sd_سائيز, ce->ce_flags);
	}
}

ساكن فراغ show_dir_داخلا(بنية اشاريو_state *istate,
			   ثابت حرف *ٽيگ, بنية dir_داخلا *ent)
{
	صحيح len = max_اڳياڙي_len;

	إذا (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	إذا (ps_matched)
		dir_رستو_ميلاپ(istate, ent, &pathspec, len, ps_matched);

	fputs(ٽيگ, stdout);
	write_eolinfo(istate, عدم, ent->name);
	write_name(ent->name);
}

ساكن فراغ show_other_files(بنية اشاريو_state *istate,
			     ثابت بنية dir_struct *dir)
{
	صحيح i;

	لكل (i = 0; i < dir->nr; i++) {
		بنية dir_داخلا *ent = dir->entries[i];
		إذا (!اشاريو_name_is_other(istate, ent->name, ent->len))
			استمر;
		show_dir_داخلا(istate, ٽيگ_other, ent);
	}
}

ساكن فراغ show_killed_files(بنية اشاريو_state *istate,
			      ثابت بنية dir_struct *dir)
{
	صحيح i;
	لكل (i = 0; i < dir->nr; i++) {
		بنية dir_داخلا *ent = dir->entries[i];
		حرف *cp, *sp;
		صحيح pos, len, killed = 0;

		لكل (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			إذا (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = اشاريو_name_pos(istate, ent->name, ent->len);
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
				    !لڙي_ڀيٽ(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				اكسر;
			}
			إذا (0 <= اشاريو_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				اكسر;
			}
		}
		إذا (killed)
			show_dir_داخلا(istate, ٽيگ_killed, dir->entries[i]);
	}
}

ساكن فراغ show_files(بنية repository *repo, بنية dir_struct *dir);

ساكن فراغ show_submodule(بنية repository *superproject,
			   بنية dir_struct *dir, ثابت حرف *رستو)
{
	بنية repository subrepo;

	إذا (repo_submodule_init(&subrepo, superproject, رستو,
				null_oid(superproject->hash_algo)))
		أرجع;

	إذا (repo_read_اشاريو(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

ساكن فراغ expand_objectsize(بنية repository *repo, بنية strbuf *سٽ,
			      ثابت بنية object_id *oid,
			      ثابت تعداد object_type type, بنانشان صحيح padded)
{
	ساكن ثابت حرف padding[] = "       ";
	سائيز_t min_len = padded ? لڙي_ڊيگهه(padding) : 0;
	سائيز_t orig_len = سٽ->len;
	سائيز_t len;

	إذا (type == OBJ_BLOB) {
		سائيز_t سائيز;
		إذا (odb_read_object_info(repo->objects, oid, &سائيز) < 0)
			die(_("could not get object info about '%s'"),
			    oid_منزل_hex(oid));
		strbuf_جوڙيو_uint(سٽ, سائيز);
	} وإلا {
		strbuf_addstr(سٽ, "-");
	}
	len = سٽ->len - orig_len;
	إذا (len < min_len)
		strbuf_وجهو(سٽ, orig_len, padding, min_len - len);
}

ساكن فراغ show_ce_fmt(بنية repository *repo, ثابت بنية cache_داخلا *ce,
			ثابت حرف *format, ثابت حرف *fullname) {
	بنية strbuf sb = STRBUF_INIT;

	طالما (strbuf_expand_قدم(&sb, &format)) {
		سائيز_t len;
		بنية stat st;

		إذا (skip_اڳياڙي(format, "%", &format))
			strbuf_addch(&sb, '%');
		وإلا إذا ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		وإلا إذا (skip_اڳياڙي(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_گهڻائي);
		وإلا إذا (skip_اڳياڙي(format, "(objectname)", &format))
			strbuf_جوڙيو_unique_abbrev(&sb, &ce->oid, abbrev);
		وإلا إذا (skip_اڳياڙي(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_گهڻائي)));
		وإلا إذا (skip_اڳياڙي(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_گهڻائي), 1);
		وإلا إذا (skip_اڳياڙي(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_گهڻائي), 0);
		وإلا إذا (skip_اڳياڙي(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		وإلا إذا (skip_اڳياڙي(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_گهڻائي) ?
				      get_cached_convert_stats_ascii(repo->اشاريو,
				      ce->name) : "");
		وإلا إذا (skip_اڳياڙي(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_گهڻائي) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		وإلا إذا (skip_اڳياڙي(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->اشاريو,
								 fullname));
		وإلا إذا (skip_اڳياڙي(format, "(path)", &format))
			write_name_منزل_buf(&sb, fullname);
		وإلا
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, سٽ_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

ساكن فراغ show_ce(بنية repository *repo, بنية dir_struct *dir,
		    ثابت بنية cache_داخلا *ce, ثابت حرف *fullname,
		    ثابت حرف *ٽيگ)
{
	إذا (max_اڳياڙي_len > لڙي_ڊيگهه(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	إذا (recurse_submodules && S_ISGITLINK(ce->ce_گهڻائي) &&
	    is_submodule_سرگرم(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} وإلا إذا (ميلاپ_pathspec(repo->اشاريو, &pathspec, fullname, لڙي_ڊيگهه(fullname),
				  max_اڳياڙي_len, ps_matched,
				  S_ISDIR(ce->ce_گهڻائي) ||
				  S_ISGITLINK(ce->ce_گهڻائي))) {
		إذا (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			أرجع;
		}

		ٽيگ = get_ٽيگ(ce, ٽيگ);

		إذا (!show_stage) {
			fputs(ٽيگ, stdout);
		} وإلا {
			لکو("%s%06o %s %d\t",
			       ٽيگ,
			       ce->ce_گهڻائي,
			       repo_ڳوليو_unique_abbrev(repo, &ce->oid, abbrev),
			       ce_stage(ce));
		}
		write_eolinfo(repo->اشاريو, ce, fullname);
		write_name(fullname);
		print_debug(ce);
	}
}

ساكن فراغ show_ru_info(بنية repository *repo, بنية اشاريو_state *istate)
{
	بنية string_list_شي *شي;

	إذا (!istate->resolve_undo)
		أرجع;

	for_each_string_list_شي(شي, istate->resolve_undo) {
		ثابت حرف *رستو = شي->string;
		بنية resolve_undo_info *ui = شي->util;
		صحيح i, len;

		len = لڙي_ڊيگهه(رستو);
		إذا (len < max_اڳياڙي_len)
			استمر; /* outside of the prefix */
		إذا (!ميلاپ_pathspec(istate, &pathspec, رستو, len,
				    max_اڳياڙي_len, ps_matched, 0))
			استمر; /* uninterested */
		لكل (i = 0; i < 3; i++) {
			إذا (!ui->گهڻائي[i])
				استمر;
			لکو("%s%06o %s %d\t", ٽيگ_resolve_undo, ui->گهڻائي[i],
			       repo_ڳوليو_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(رستو);
		}
	}
}

ساكن صحيح ce_excluded(بنية dir_struct *dir, بنية اشاريو_state *istate,
		       ثابت حرف *fullname, ثابت بنية cache_داخلا *ce)
{
	صحيح dtype = ce_منزل_dtype(ce);
	أرجع is_excluded(dir, istate, fullname, &dtype);
}

ساكن فراغ construct_fullname(بنية strbuf *ٻاهريون, ثابت بنية repository *repo,
			       ثابت بنية cache_داخلا *ce)
{
	strbuf_reset(ٻاهريون);
	إذا (repo->submodule_اڳياڙي)
		strbuf_addstr(ٻاهريون, repo->submodule_اڳياڙي);
	strbuf_addstr(ٻاهريون, ce->name);
}

ساكن فراغ show_files(بنية repository *repo, بنية dir_struct *dir)
{
	صحيح i;
	بنية strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	إذا (show_others || show_killed) {
		إذا (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->اشاريو, &pathspec);
		إذا (show_others)
			show_other_files(repo->اشاريو, dir);
		إذا (show_killed)
			show_killed_files(repo->اشاريو, dir);
	}

	إذا (!(show_cached || show_stage || show_deleted || show_modified))
		أرجع;

	لكل (i = 0; i < repo->اشاريو->cache_nr; i++) {
		ثابت بنية cache_داخلا *ce = repo->اشاريو->cache[i];
		بنية stat st;
		صحيح stat_err;

		إذا (S_ISSPARSEDIR(ce->ce_گهڻائي) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_ڀريل_اشاريو(repo->اشاريو);
			ce = repo->اشاريو->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		إذا ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->اشاريو, fullname.buf, ce))
			استمر;
		إذا (ce->ce_flags & CE_UPDATE)
			استمر;
		إذا ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? ٽيگ_unmerged :
				(ce_skip_worktree(ce) ? ٽيگ_skip_worktree :
				 ٽيگ_cached));
			إذا (skipping_duplicates)
				اذهب skip_منزل_ايندڙ_name;
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
		    !ميلاپ_pathspec(repo->اشاريو, &pathspec, fullname.buf,
				    fullname.len, max_اڳياڙي_len, عدم,
				    S_ISDIR(ce->ce_گهڻائي) ||
				    S_ISGITLINK(ce->ce_گهڻائي)))
			استمر;
		stat_err = lstat(fullname.buf, &st);
		إذا (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		إذا (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, ٽيگ_removed);
			إذا (skipping_duplicates)
				اذهب skip_منزل_ايندڙ_name;
		}
		إذا (show_modified &&
		    (stat_err || ie_modified(repo->اشاريو, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, ٽيگ_modified);
			إذا (skipping_duplicates)
				اذهب skip_منزل_ايندڙ_name;
		}
		استمر;

skip_منزل_ايندڙ_name:
		{
			صحيح j;
			بنية cache_داخلا **cache = repo->اشاريو->cache;
			لكل (j = i + 1; j < repo->اشاريو->cache_nr; j++)
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
ساكن فراغ prune_اشاريو(بنية اشاريو_state *istate,
			ثابت حرف *اڳياڙي, سائيز_t prefixlen)
{
	صحيح pos;
	بنانشان صحيح پهريون, آخري;

	إذا (!اڳياڙي || !istate->cache_nr)
		أرجع;
	pos = اشاريو_name_pos(istate, اڳياڙي, prefixlen);
	إذا (pos < 0)
		pos = -pos-1;
	پهريون = pos;
	آخري = istate->cache_nr;
	طالما (آخري > پهريون) {
		صحيح ايندڙ = پهريون + ((آخري - پهريون) >> 1);
		ثابت بنية cache_داخلا *ce = istate->cache[ايندڙ];
		إذا (!لڙي_ڀيٽ(ce->name, اڳياڙي, prefixlen)) {
			پهريون = ايندڙ+1;
			استمر;
		}
		آخري = ايندڙ;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, آخري - pos);
	istate->cache_nr = آخري - pos;
}

ساكن صحيح get_common_اڳياڙي_len(ثابت حرف *common_اڳياڙي)
{
	صحيح common_اڳياڙي_len;

	إذا (!common_اڳياڙي)
		أرجع 0;

	common_اڳياڙي_len = لڙي_ڊيگهه(common_اڳياڙي);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	إذا (common_اڳياڙي[common_اڳياڙي_len - 1] == '/')
		common_اڳياڙي_len--;

	أرجع common_اڳياڙي_len;
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
	جوڙيو_patterns_from_file(dir, arg);

	أرجع 0;
}

ساكن صحيح option_parse_exclude_standard(ثابت بنية option *opt,
					 ثابت حرف *arg, صحيح unset)
{
	بنية dir_struct *dir = opt->قدر;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	تياري_standard_excludes(dir);

	أرجع 0;
}

صحيح cmd_ls_files(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *cmd_اڳياڙي,
		 بنية repository *repo)
{
	صحيح require_work_وڻ = 0, show_ٽيگ = 0, i;
	حرف *max_اڳياڙي;
	بنية dir_struct dir = DIR_INIT;
	بنية نمونو_list *pl;
	بنية string_list exclude_list = STRING_LIST_INIT_NODUP;
	بنية option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_سيٽ_INT('z', عدم, &سٽ_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', عدم, &show_ٽيگ,
			N_("identify the file status with tags")),
		OPT_BOOL('v', عدم, &show_valid_ٻٽو_انگ,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', عدم, &show_fsmonitor_ٻٽو_انگ,
			N_("use lowercase letters for 'fsmonitor clean' files")),
		OPT_BOOL('c', "cached", &show_cached,
			N_("show cached files in the output (default)")),
		OPT_BOOL('d', "deleted", &show_deleted,
			N_("show deleted files in the output")),
		OPT_BOOL('m', "modified", &show_modified,
			N_("show modified files in the output")),
		OPT_BOOL('o', "others", &show_others,
			N_("show other files in the output")),
		OPT_ٻٽو_انگ('i', "ignored", &dir.flags,
			N_("show ignored files in the output"),
			DIR_SHOW_IGNORED),
		OPT_BOOL('s', "stage", &show_stage,
			N_("show staged contents' object name in the output")),
		OPT_BOOL('k', "killed", &show_killed,
			N_("show files on the filesystem that need to be removed")),
		OPT_ٻٽو_انگ(0, "directory", &dir.flags,
			N_("show 'other' directories' names only"),
			DIR_SHOW_OTHER_DIRECTORIES),
		OPT_BOOL(0, "eol", &show_eol, N_("show line endings of files")),
		OPT_NEGBIT(0, "empty-directory", &dir.flags,
			N_("don't show empty directories"),
			DIR_HIDE_خالي_جايون_DIRECTORIES),
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
		OPT_STRING(0, "exclude-per-directory", &dir.exclude_في_dir, N_("file"),
			N_("read additional per-directory exclude patterns in <file>")),
		OPT_CALLBACK_F(0, "exclude-standard", &dir, عدم,
			N_("add the standard git exclusions"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			option_parse_exclude_standard),
		OPT_سيٽ_INT_F(0, "full-name", &اڳياڙي_len,
			      N_("make the output relative to the project top directory"),
			      0, PARSE_OPT_NONEG),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			N_("recurse through submodules")),
		OPT_BOOL(0, "error-unmatch", &error_unmatch,
			N_("if any <file> is not in the index, treat this as an error")),
		OPT_STRING(0, "with-tree", &with_وڻ, N_("tree-ish"),
			N_("pretend that paths removed since <tree-ish> are still present")),
		OPT__ABBREV(&abbrev),
		OPT_BOOL(0, "debug", &debug_گهڻائي, N_("show debugging data")),
		OPT_BOOL(0, "deduplicate", &skipping_duplicates,
			 N_("suppress duplicate entries")),
		OPT_BOOL(0, "sparse", &show_sparse_dirs,
			 N_("show sparse directories in the presence of a sparse index")),
		OPT_STRING_F(0, "format", &format, N_("format"),
			     N_("format to use for the output"),
			     PARSE_OPT_NONEG),
		OPT_پڇاڙي()
	};
	صحيح ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_ڀريل_اشاريو = 0;

	اڳياڙي = cmd_اڳياڙي;
	إذا (اڳياڙي)
		اڳياڙي_len = لڙي_ڊيگهه(اڳياڙي);
	repo_config(repo, git_default_config, عدم);

	إذا (repo_read_اشاريو(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, اڳياڙي, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = جوڙيو_نمونو_list(&dir, EXC_CMDL, "--exclude option");
	لكل (i = 0; i < exclude_list.nr; i++) {
		جوڙيو_نمونو(exclude_list.شيون[i].string, "", 0, pl, --exclude_دليلون);
	}

	إذا (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_ٽيگ))
			usage_نياپو_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	إذا (show_ٽيگ || show_valid_ٻٽو_انگ || show_fsmonitor_ٻٽو_انگ) {
		ٽيگ_cached = "H ";
		ٽيگ_unmerged = "M ";
		ٽيگ_removed = "R ";
		ٽيگ_modified = "C ";
		ٽيگ_other = "? ";
		ٽيگ_killed = "K ";
		ٽيگ_skip_worktree = "S ";
		ٽيگ_resolve_undo = "U ";
	}
	إذا (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_وڻ = 1;
	إذا (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	إذا (show_ٽيگ || show_stage)
		skipping_duplicates = 0;
	إذا (dir.exclude_في_dir)
		exc_given = 1;

	إذا (require_work_وڻ && !is_inside_work_وڻ(repo))
		تياري_work_وڻ(repo);

	إذا (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_وڻ))
		die("ls-files --recurse-submodules unsupported mode");

	إذا (recurse_submodules && error_unmatch)
		die("ls-files --recurse-submodules does not support "
		    "--error-unmatch");

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       اڳياڙي, argv);

	/*
	 * Find common prefix for all pathspec's
	 * This is used as a performance optimization which unfortunately cannot
	 * be done when recursing into submodules because when a pathspec is
	 * given which spans repository boundaries you can't simply remove the
	 * submodule entry because the pathspec may match something inside the
	 * submodule.
	 */
	إذا (recurse_submodules)
		max_اڳياڙي = عدم;
	وإلا
		max_اڳياڙي = common_اڳياڙي(&pathspec);
	max_اڳياڙي_len = get_common_اڳياڙي_len(max_اڳياڙي);

	prune_اشاريو(repo->اشاريو, max_اڳياڙي, max_اڳياڙي_len);

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

	إذا (with_وڻ) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		إذا (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_وڻ_on_اشاريو(repo->اشاريو, with_وڻ, max_اڳياڙي);
	}

	show_files(repo, &dir);

	إذا (show_resolve_undo)
		show_ru_info(repo, repo->اشاريو);

	إذا (ps_matched && report_رستو_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	حرر(max_اڳياڙي);
	أرجع ret;
}
