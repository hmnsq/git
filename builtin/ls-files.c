/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "convert.h"
#समावेश "environment.h"
#समावेश "quote.h"
#समावेश "dir.h"
#समावेश "gettext.h"
#समावेश "object-name.h"
#समावेश "strbuf.h"
#समावेश "parse-options.h"
#समावेश "resolve-undo.h"
#समावेश "string-list.h"
#समावेश "path.h"
#समावेश "pathspec.h"
#समावेश "read-cache.h"
#समावेश "setup.h"
#समावेश "sparse-index.h"
#समावेश "submodule.h"
#समावेश "odb.h"
#समावेश "hex.h"


अटल पूर्णांक abbrev;
अटल पूर्णांक show_deleted;
अटल पूर्णांक show_cached;
अटल पूर्णांक show_others;
अटल पूर्णांक show_stage;
अटल पूर्णांक show_unmerged;
अटल पूर्णांक show_resolve_undo;
अटल पूर्णांक show_modified;
अटल पूर्णांक show_killed;
अटल पूर्णांक show_valid_bit;
अटल पूर्णांक show_fsmonitor_bit;
अटल पूर्णांक पंक्ति_terminator = '\n';
अटल पूर्णांक debug_mode;
अटल पूर्णांक show_eol;
अटल पूर्णांक recurse_submodules;
अटल पूर्णांक skipping_duplicates;
अटल पूर्णांक show_sparse_dirs;

अटल स्थिर अक्षर *prefix;
अटल पूर्णांक max_prefix_len;
अटल पूर्णांक prefix_len;
अटल काष्ठा pathspec pathspec;
अटल पूर्णांक error_unmatch;
अटल अक्षर *ps_matched;
अटल स्थिर अक्षर *with_वृक्ष;
अटल पूर्णांक exc_given;
अटल पूर्णांक exclude_args;
अटल स्थिर अक्षर *format;

अटल स्थिर अक्षर *tag_cached = "";
अटल स्थिर अक्षर *tag_unmerged = "";
अटल स्थिर अक्षर *tag_removed = "";
अटल स्थिर अक्षर *tag_other = "";
अटल स्थिर अक्षर *tag_killed = "";
अटल स्थिर अक्षर *tag_modified = "";
अटल स्थिर अक्षर *tag_skip_worktree = "";
अटल स्थिर अक्षर *tag_resolve_undo = "";

अटल व्योम write_eolinfo(काष्ठा सूचकांक_state *istate,
			  स्थिर काष्ठा cache_entry *ce, स्थिर अक्षर *path)
{
	अगर (show_eol) {
		काष्ठा stat st;
		स्थिर अक्षर *i_txt = "";
		स्थिर अक्षर *w_txt = "";
		स्थिर अक्षर *a_txt = get_convert_attr_ascii(istate, path);
		अगर (ce && S_ISREG(ce->ce_mode))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		अगर (!lstat(path, &st) && S_ISREG(st.st_mode))
			w_txt = get_wt_convert_stats_ascii(path);
		म_लिखो("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

अटल व्योम write_name(स्थिर अक्षर *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, prefix_len ? prefix : शून्य,
				   मानक_निकास, पंक्ति_terminator);
}

अटल व्योम write_name_to_buf(काष्ठा strbuf *sb, स्थिर अक्षर *name)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	स्थिर अक्षर *rel = relative_path(name, prefix_len ? prefix : शून्य, &buf);

	अगर (पंक्ति_terminator)
		quote_c_style(rel, sb, शून्य, 0);
	अन्यथा
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

अटल स्थिर अक्षर *get_tag(स्थिर काष्ठा cache_entry *ce, स्थिर अक्षर *tag)
{
	अटल अक्षर alttag[4];

	अगर (tag && *tag && ((show_valid_bit && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_bit && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		स_नकल(alttag, tag, 3);

		अगर (है_अक्षर(tag[0])) {
			alttag[0] = छोटे(tag[0]);
		} अन्यथा अगर (tag[0] == '?') {
			alttag[0] = '!';
		} अन्यथा {
			alttag[0] = 'v';
			alttag[1] = tag[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		tag = alttag;
	}

	वापस tag;
}

अटल व्योम print_debug(स्थिर काष्ठा cache_entry *ce)
{
	अगर (debug_mode) {
		स्थिर काष्ठा stat_तथ्य *sd = &ce->ce_stat_तथ्य;

		म_लिखो("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		म_लिखो("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		म_लिखो("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		म_लिखो("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		म_लिखो("  size: %u\tflags: %x\n", sd->sd_आकार, ce->ce_flags);
	}
}

अटल व्योम show_dir_entry(काष्ठा सूचकांक_state *istate,
			   स्थिर अक्षर *tag, काष्ठा dir_entry *ent)
{
	पूर्णांक len = max_prefix_len;

	अगर (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	अगर (ps_matched)
		dir_path_match(istate, ent, &pathspec, len, ps_matched);

	ख_माला_दो(tag, मानक_निकास);
	write_eolinfo(istate, शून्य, ent->name);
	write_name(ent->name);
}

अटल व्योम show_other_files(काष्ठा सूचकांक_state *istate,
			     स्थिर काष्ठा dir_struct *dir)
{
	पूर्णांक i;

	क्रम (i = 0; i < dir->nr; i++) {
		काष्ठा dir_entry *ent = dir->entries[i];
		अगर (!सूचकांक_name_is_other(istate, ent->name, ent->len))
			जारी;
		show_dir_entry(istate, tag_other, ent);
	}
}

अटल व्योम show_killed_files(काष्ठा सूचकांक_state *istate,
			      स्थिर काष्ठा dir_struct *dir)
{
	पूर्णांक i;
	क्रम (i = 0; i < dir->nr; i++) {
		काष्ठा dir_entry *ent = dir->entries[i];
		अक्षर *cp, *sp;
		पूर्णांक pos, len, killed = 0;

		क्रम (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = म_अक्षर(cp, '/');
			अगर (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = सूचकांक_name_pos(istate, ent->name, ent->len);
				अगर (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				जबतक (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				अगर (istate->cache_nr <= pos)
					अवरोध;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				अगर ((ent->len < len) &&
				    !म_भेदन(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				अवरोध;
			}
			अगर (0 <= सूचकांक_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				अवरोध;
			}
		}
		अगर (killed)
			show_dir_entry(istate, tag_killed, dir->entries[i]);
	}
}

अटल व्योम show_files(काष्ठा repository *repo, काष्ठा dir_struct *dir);

अटल व्योम show_submodule(काष्ठा repository *superproject,
			   काष्ठा dir_struct *dir, स्थिर अक्षर *path)
{
	काष्ठा repository subrepo;

	अगर (repo_submodule_init(&subrepo, superproject, path,
				null_oid(superproject->hash_algo)))
		वापस;

	अगर (repo_read_सूचकांक(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

अटल व्योम expand_objectsize(काष्ठा repository *repo, काष्ठा strbuf *पंक्ति,
			      स्थिर काष्ठा object_id *oid,
			      स्थिर क्रमागत object_type type, अचिन्हित पूर्णांक padded)
{
	अटल स्थिर अक्षर padding[] = "       ";
	माप_प्रकार min_len = padded ? म_माप(padding) : 0;
	माप_प्रकार orig_len = पंक्ति->len;
	माप_प्रकार len;

	अगर (type == OBJ_BLOB) {
		माप_प्रकार आकार;
		अगर (odb_read_object_info(repo->objects, oid, &आकार) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(पंक्ति, आकार);
	} अन्यथा {
		strbuf_addstr(पंक्ति, "-");
	}
	len = पंक्ति->len - orig_len;
	अगर (len < min_len)
		strbuf_जोड़ो(पंक्ति, orig_len, padding, min_len - len);
}

अटल व्योम show_ce_fmt(काष्ठा repository *repo, स्थिर काष्ठा cache_entry *ce,
			स्थिर अक्षर *format, स्थिर अक्षर *fullname) {
	काष्ठा strbuf sb = STRBUF_INIT;

	जबतक (strbuf_expand_step(&sb, &format)) {
		माप_प्रकार len;
		काष्ठा stat st;

		अगर (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		अन्यथा अगर ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		अन्यथा अगर (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_mode);
		अन्यथा अगर (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, &ce->oid, abbrev);
		अन्यथा अगर (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_mode)));
		अन्यथा अगर (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 1);
		अन्यथा अगर (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 0);
		अन्यथा अगर (skip_prefix(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		अन्यथा अगर (skip_prefix(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_mode) ?
				      get_cached_convert_stats_ascii(repo->सूचकांक,
				      ce->name) : "");
		अन्यथा अगर (skip_prefix(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_mode) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		अन्यथा अगर (skip_prefix(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->सूचकांक,
								 fullname));
		अन्यथा अगर (skip_prefix(format, "(path)", &format))
			write_name_to_buf(&sb, fullname);
		अन्यथा
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, पंक्ति_terminator);
	ख_डालो(sb.buf, sb.len, 1, मानक_निकास);
	strbuf_release(&sb);
}

अटल व्योम show_ce(काष्ठा repository *repo, काष्ठा dir_struct *dir,
		    स्थिर काष्ठा cache_entry *ce, स्थिर अक्षर *fullname,
		    स्थिर अक्षर *tag)
{
	अगर (max_prefix_len > म_माप(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	अगर (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
	    is_submodule_active(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} अन्यथा अगर (match_pathspec(repo->सूचकांक, &pathspec, fullname, म_माप(fullname),
				  max_prefix_len, ps_matched,
				  S_ISDIR(ce->ce_mode) ||
				  S_ISGITLINK(ce->ce_mode))) {
		अगर (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			वापस;
		}

		tag = get_tag(ce, tag);

		अगर (!show_stage) {
			ख_माला_दो(tag, मानक_निकास);
		} अन्यथा {
			म_लिखो("%s%06o %s %d\t",
			       tag,
			       ce->ce_mode,
			       repo_find_unique_abbrev(repo, &ce->oid, abbrev),
			       ce_stage(ce));
		}
		write_eolinfo(repo->सूचकांक, ce, fullname);
		write_name(fullname);
		print_debug(ce);
	}
}

अटल व्योम show_ru_info(काष्ठा repository *repo, काष्ठा सूचकांक_state *istate)
{
	काष्ठा string_list_item *item;

	अगर (!istate->resolve_undo)
		वापस;

	for_each_string_list_item(item, istate->resolve_undo) {
		स्थिर अक्षर *path = item->string;
		काष्ठा resolve_undo_info *ui = item->util;
		पूर्णांक i, len;

		len = म_माप(path);
		अगर (len < max_prefix_len)
			जारी; /* outside of the prefix */
		अगर (!match_pathspec(istate, &pathspec, path, len,
				    max_prefix_len, ps_matched, 0))
			जारी; /* uninterested */
		क्रम (i = 0; i < 3; i++) {
			अगर (!ui->mode[i])
				जारी;
			म_लिखो("%s%06o %s %d\t", tag_resolve_undo, ui->mode[i],
			       repo_find_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(path);
		}
	}
}

अटल पूर्णांक ce_excluded(काष्ठा dir_struct *dir, काष्ठा सूचकांक_state *istate,
		       स्थिर अक्षर *fullname, स्थिर काष्ठा cache_entry *ce)
{
	पूर्णांक dtype = ce_to_dtype(ce);
	वापस is_excluded(dir, istate, fullname, &dtype);
}

अटल व्योम construct_fullname(काष्ठा strbuf *out, स्थिर काष्ठा repository *repo,
			       स्थिर काष्ठा cache_entry *ce)
{
	strbuf_reset(out);
	अगर (repo->submodule_prefix)
		strbuf_addstr(out, repo->submodule_prefix);
	strbuf_addstr(out, ce->name);
}

अटल व्योम show_files(काष्ठा repository *repo, काष्ठा dir_struct *dir)
{
	पूर्णांक i;
	काष्ठा strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	अगर (show_others || show_killed) {
		अगर (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->सूचकांक, &pathspec);
		अगर (show_others)
			show_other_files(repo->सूचकांक, dir);
		अगर (show_killed)
			show_killed_files(repo->सूचकांक, dir);
	}

	अगर (!(show_cached || show_stage || show_deleted || show_modified))
		वापस;

	क्रम (i = 0; i < repo->सूचकांक->cache_nr; i++) {
		स्थिर काष्ठा cache_entry *ce = repo->सूचकांक->cache[i];
		काष्ठा stat st;
		पूर्णांक stat_err;

		अगर (S_ISSPARSEDIR(ce->ce_mode) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_full_सूचकांक(repo->सूचकांक);
			ce = repo->सूचकांक->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		अगर ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->सूचकांक, fullname.buf, ce))
			जारी;
		अगर (ce->ce_flags & CE_UPDATE)
			जारी;
		अगर ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? tag_unmerged :
				(ce_skip_worktree(ce) ? tag_skip_worktree :
				 tag_cached));
			अगर (skipping_duplicates)
				जाओ skip_to_अग्र_name;
		}

		अगर (!(show_deleted || show_modified))
			जारी;
		अगर (ce_skip_worktree(ce))
			जारी;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		अगर (pathspec.nr == 1 &&
		    !match_pathspec(repo->सूचकांक, &pathspec, fullname.buf,
				    fullname.len, max_prefix_len, शून्य,
				    S_ISDIR(ce->ce_mode) ||
				    S_ISGITLINK(ce->ce_mode)))
			जारी;
		stat_err = lstat(fullname.buf, &st);
		अगर (stat_err && (त्रुटि_सं != ENOENT && त्रुटि_सं != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		अगर (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, tag_removed);
			अगर (skipping_duplicates)
				जाओ skip_to_अग्र_name;
		}
		अगर (show_modified &&
		    (stat_err || ie_modified(repo->सूचकांक, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, tag_modified);
			अगर (skipping_duplicates)
				जाओ skip_to_अग्र_name;
		}
		जारी;

skip_to_अग्र_name:
		{
			पूर्णांक j;
			काष्ठा cache_entry **cache = repo->सूचकांक->cache;
			क्रम (j = i + 1; j < repo->सूचकांक->cache_nr; j++)
				अगर (म_भेद(ce->name, cache[j]->name))
					अवरोध;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
अटल व्योम prune_सूचकांक(काष्ठा सूचकांक_state *istate,
			स्थिर अक्षर *prefix, माप_प्रकार prefixlen)
{
	पूर्णांक pos;
	अचिन्हित पूर्णांक first, last;

	अगर (!prefix || !istate->cache_nr)
		वापस;
	pos = सूचकांक_name_pos(istate, prefix, prefixlen);
	अगर (pos < 0)
		pos = -pos-1;
	first = pos;
	last = istate->cache_nr;
	जबतक (last > first) {
		पूर्णांक अग्र = first + ((last - first) >> 1);
		स्थिर काष्ठा cache_entry *ce = istate->cache[अग्र];
		अगर (!म_भेदन(ce->name, prefix, prefixlen)) {
			first = अग्र+1;
			जारी;
		}
		last = अग्र;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, last - pos);
	istate->cache_nr = last - pos;
}

अटल पूर्णांक get_common_prefix_len(स्थिर अक्षर *common_prefix)
{
	पूर्णांक common_prefix_len;

	अगर (!common_prefix)
		वापस 0;

	common_prefix_len = म_माप(common_prefix);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	अगर (common_prefix[common_prefix_len - 1] == '/')
		common_prefix_len--;

	वापस common_prefix_len;
}

अटल स्थिर अक्षर * स्थिर ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	शून्य
};

अटल पूर्णांक option_parse_exclude(स्थिर काष्ठा option *opt,
				स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा string_list *exclude_list = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	वापस 0;
}

अटल पूर्णांक option_parse_exclude_from(स्थिर काष्ठा option *opt,
				     स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा dir_struct *dir = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	add_patterns_from_file(dir, arg);

	वापस 0;
}

अटल पूर्णांक option_parse_exclude_standard(स्थिर काष्ठा option *opt,
					 स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा dir_struct *dir = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	setup_standard_excludes(dir);

	वापस 0;
}

पूर्णांक cmd_ls_files(पूर्णांक argc,
		 स्थिर अक्षर **argv,
		 स्थिर अक्षर *cmd_prefix,
		 काष्ठा repository *repo)
{
	पूर्णांक require_work_वृक्ष = 0, show_tag = 0, i;
	अक्षर *max_prefix;
	काष्ठा dir_struct dir = DIR_INIT;
	काष्ठा pattern_list *pl;
	काष्ठा string_list exclude_list = STRING_LIST_INIT_NODUP;
	काष्ठा option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_SET_INT('z', शून्य, &पंक्ति_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', शून्य, &show_tag,
			N_("identify the file status with tags")),
		OPT_BOOL('v', शून्य, &show_valid_bit,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', शून्य, &show_fsmonitor_bit,
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
		OPT_CALLBACK_F(0, "exclude-standard", &dir, शून्य,
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
		OPT_STRING(0, "with-tree", &with_वृक्ष, N_("tree-ish"),
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
	पूर्णांक ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_सूचकांक = 0;

	prefix = cmd_prefix;
	अगर (prefix)
		prefix_len = म_माप(prefix);
	repo_config(repo, git_default_config, शून्य);

	अगर (repo_read_सूचकांक(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, prefix, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	क्रम (i = 0; i < exclude_list.nr; i++) {
		add_pattern(exclude_list.वस्तुएँ[i].string, "", 0, pl, --exclude_args);
	}

	अगर (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_tag))
			usage_संदेश_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	अगर (show_tag || show_valid_bit || show_fsmonitor_bit) {
		tag_cached = "H ";
		tag_unmerged = "M ";
		tag_removed = "R ";
		tag_modified = "C ";
		tag_other = "? ";
		tag_killed = "K ";
		tag_skip_worktree = "S ";
		tag_resolve_undo = "U ";
	}
	अगर (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_वृक्ष = 1;
	अगर (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	अगर (show_tag || show_stage)
		skipping_duplicates = 0;
	अगर (dir.exclude_per_dir)
		exc_given = 1;

	अगर (require_work_वृक्ष && !is_inside_work_वृक्ष(repo))
		setup_work_वृक्ष(repo);

	अगर (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_वृक्ष))
		die("ls-files --recurse-submodules unsupported mode");

	अगर (recurse_submodules && error_unmatch)
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
	अगर (recurse_submodules)
		max_prefix = शून्य;
	अन्यथा
		max_prefix = common_prefix(&pathspec);
	max_prefix_len = get_common_prefix_len(max_prefix);

	prune_सूचकांक(repo->सूचकांक, max_prefix, max_prefix_len);

	/* Treat unmatching pathspec elements as errors */
	अगर (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	अगर ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	अगर ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	अगर (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	अगर (with_वृक्ष) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		अगर (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_वृक्ष_on_सूचकांक(repo->सूचकांक, with_वृक्ष, max_prefix);
	}

	show_files(repo, &dir);

	अगर (show_resolve_undo)
		show_ru_info(repo, repo->सूचकांक);

	अगर (ps_matched && report_path_error(ps_matched, &pathspec)) {
		ख_लिखो(मानक_त्रुटि, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	मुक्त(max_prefix);
	वापस ret;
}
