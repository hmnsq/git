/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "convert.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "resolve-undo.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "pathspec.h"
#अन्तर्भाव "read-cache.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "sparse-index.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "hex.h"


स्थिर पूर्णाङ्क abbrev;
स्थिर पूर्णाङ्क show_deleted;
स्थिर पूर्णाङ्क show_cached;
स्थिर पूर्णाङ्क show_others;
स्थिर पूर्णाङ्क show_stage;
स्थिर पूर्णाङ्क show_unmerged;
स्थिर पूर्णाङ्क show_resolve_undo;
स्थिर पूर्णाङ्क show_modified;
स्थिर पूर्णाङ्क show_killed;
स्थिर पूर्णाङ्क show_valid_bit;
स्थिर पूर्णाङ्क show_fsmonitor_bit;
स्थिर पूर्णाङ्क ओळ_terminator = '\n';
स्थिर पूर्णाङ्क debug_mode;
स्थिर पूर्णाङ्क show_eol;
स्थिर पूर्णाङ्क recurse_submodules;
स्थिर पूर्णाङ्क skipping_duplicates;
स्थिर पूर्णाङ्क show_sparse_dirs;

स्थिर ध्रुव वर्ण *prefix;
स्थिर पूर्णाङ्क max_prefix_len;
स्थिर पूर्णाङ्क prefix_len;
स्थिर संरचना pathspec pathspec;
स्थिर पूर्णाङ्क error_unmatch;
स्थिर वर्ण *ps_matched;
स्थिर ध्रुव वर्ण *with_वृक्ष;
स्थिर पूर्णाङ्क exc_given;
स्थिर पूर्णाङ्क exclude_args;
स्थिर ध्रुव वर्ण *format;

स्थिर ध्रुव वर्ण *tag_cached = "";
स्थिर ध्रुव वर्ण *tag_unmerged = "";
स्थिर ध्रुव वर्ण *tag_removed = "";
स्थिर ध्रुव वर्ण *tag_other = "";
स्थिर ध्रुव वर्ण *tag_killed = "";
स्थिर ध्रुव वर्ण *tag_modified = "";
स्थिर ध्रुव वर्ण *tag_skip_worktree = "";
स्थिर ध्रुव वर्ण *tag_resolve_undo = "";

स्थिर शून्य write_eolinfo(संरचना index_state *istate,
			  ध्रुव संरचना cache_entry *ce, ध्रुव वर्ण *path)
{
	यदि (show_eol) {
		संरचना stat st;
		ध्रुव वर्ण *i_txt = "";
		ध्रुव वर्ण *w_txt = "";
		ध्रुव वर्ण *a_txt = get_convert_attr_ascii(istate, path);
		यदि (ce && S_ISREG(ce->ce_mode))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		यदि (!lstat(path, &st) && S_ISREG(st.st_mode))
			w_txt = get_wt_convert_stats_ascii(path);
		printf("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

स्थिर शून्य write_name(ध्रुव वर्ण *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, prefix_len ? prefix : रिक्त,
				   stdout, ओळ_terminator);
}

स्थिर शून्य write_name_to_buf(संरचना strbuf *sb, ध्रुव वर्ण *name)
{
	संरचना strbuf buf = STRBUF_INIT;
	ध्रुव वर्ण *rel = relative_path(name, prefix_len ? prefix : रिक्त, &buf);

	यदि (ओळ_terminator)
		quote_c_style(rel, sb, रिक्त, 0);
	अन्यथा
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

स्थिर ध्रुव वर्ण *get_tag(ध्रुव संरचना cache_entry *ce, ध्रुव वर्ण *tag)
{
	स्थिर वर्ण alttag[4];

	यदि (tag && *tag && ((show_valid_bit && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_bit && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		memcpy(alttag, tag, 3);

		यदि (isalpha(tag[0])) {
			alttag[0] = tolower(tag[0]);
		} अन्यथा यदि (tag[0] == '?') {
			alttag[0] = '!';
		} अन्यथा {
			alttag[0] = 'v';
			alttag[1] = tag[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		tag = alttag;
	}

	प्रत्यागम tag;
}

स्थिर शून्य print_debug(ध्रुव संरचना cache_entry *ce)
{
	यदि (debug_mode) {
		ध्रुव संरचना stat_data *sd = &ce->ce_stat_data;

		printf("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		printf("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		printf("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		printf("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		printf("  size: %u\tflags: %x\n", sd->sd_size, ce->ce_flags);
	}
}

स्थिर शून्य show_dir_entry(संरचना index_state *istate,
			   ध्रुव वर्ण *tag, संरचना dir_entry *ent)
{
	पूर्णाङ्क len = max_prefix_len;

	यदि (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	यदि (ps_matched)
		dir_path_match(istate, ent, &pathspec, len, ps_matched);

	fputs(tag, stdout);
	write_eolinfo(istate, रिक्त, ent->name);
	write_name(ent->name);
}

स्थिर शून्य show_other_files(संरचना index_state *istate,
			     ध्रुव संरचना dir_struct *dir)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < dir->nr; i++) {
		संरचना dir_entry *ent = dir->entries[i];
		यदि (!index_name_is_other(istate, ent->name, ent->len))
			अनुवर्त;
		show_dir_entry(istate, tag_other, ent);
	}
}

स्थिर शून्य show_killed_files(संरचना index_state *istate,
			      ध्रुव संरचना dir_struct *dir)
{
	पूर्णाङ्क i;
	प्रति (i = 0; i < dir->nr; i++) {
		संरचना dir_entry *ent = dir->entries[i];
		वर्ण *cp, *sp;
		पूर्णाङ्क pos, len, killed = 0;

		प्रति (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			यदि (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = index_name_pos(istate, ent->name, ent->len);
				यदि (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				यावत् (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				यदि (istate->cache_nr <= pos)
					भङ्ग;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				यदि ((ent->len < len) &&
				    !strncmp(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				भङ्ग;
			}
			यदि (0 <= index_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				भङ्ग;
			}
		}
		यदि (killed)
			show_dir_entry(istate, tag_killed, dir->entries[i]);
	}
}

स्थिर शून्य show_files(संरचना repository *repo, संरचना dir_struct *dir);

स्थिर शून्य show_submodule(संरचना repository *superproject,
			   संरचना dir_struct *dir, ध्रुव वर्ण *path)
{
	संरचना repository subrepo;

	यदि (repo_submodule_init(&subrepo, superproject, path,
				null_oid(superproject->hash_algo)))
		प्रत्यागम;

	यदि (repo_read_index(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

स्थिर शून्य expand_objectsize(संरचना repository *repo, संरचना strbuf *ओळ,
			      ध्रुव संरचना object_id *oid,
			      ध्रुव गणना object_type type, unsigned पूर्णाङ्क padded)
{
	स्थिर ध्रुव वर्ण padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = ओळ->len;
	size_t len;

	यदि (type == OBJ_BLOB) {
		size_t size;
		यदि (odb_read_object_info(repo->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(ओळ, size);
	} अन्यथा {
		strbuf_addstr(ओळ, "-");
	}
	len = ओळ->len - orig_len;
	यदि (len < min_len)
		strbuf_घाला(ओळ, orig_len, padding, min_len - len);
}

स्थिर शून्य show_ce_fmt(संरचना repository *repo, ध्रुव संरचना cache_entry *ce,
			ध्रुव वर्ण *format, ध्रुव वर्ण *fullname) {
	संरचना strbuf sb = STRBUF_INIT;

	यावत् (strbuf_expand_step(&sb, &format)) {
		size_t len;
		संरचना stat st;

		यदि (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		अन्यथा यदि ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		अन्यथा यदि (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_mode);
		अन्यथा यदि (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, &ce->oid, abbrev);
		अन्यथा यदि (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_mode)));
		अन्यथा यदि (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 1);
		अन्यथा यदि (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 0);
		अन्यथा यदि (skip_prefix(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		अन्यथा यदि (skip_prefix(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_mode) ?
				      get_cached_convert_stats_ascii(repo->index,
				      ce->name) : "");
		अन्यथा यदि (skip_prefix(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_mode) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		अन्यथा यदि (skip_prefix(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->index,
								 fullname));
		अन्यथा यदि (skip_prefix(format, "(path)", &format))
			write_name_to_buf(&sb, fullname);
		अन्यथा
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, ओळ_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

स्थिर शून्य show_ce(संरचना repository *repo, संरचना dir_struct *dir,
		    ध्रुव संरचना cache_entry *ce, ध्रुव वर्ण *fullname,
		    ध्रुव वर्ण *tag)
{
	यदि (max_prefix_len > strlen(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	यदि (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
	    is_submodule_active(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} अन्यथा यदि (match_pathspec(repo->index, &pathspec, fullname, strlen(fullname),
				  max_prefix_len, ps_matched,
				  S_ISDIR(ce->ce_mode) ||
				  S_ISGITLINK(ce->ce_mode))) {
		यदि (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			प्रत्यागम;
		}

		tag = get_tag(ce, tag);

		यदि (!show_stage) {
			fputs(tag, stdout);
		} अन्यथा {
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

स्थिर शून्य show_ru_info(संरचना repository *repo, संरचना index_state *istate)
{
	संरचना string_list_item *item;

	यदि (!istate->resolve_undo)
		प्रत्यागम;

	for_each_string_list_item(item, istate->resolve_undo) {
		ध्रुव वर्ण *path = item->string;
		संरचना resolve_undo_info *ui = item->util;
		पूर्णाङ्क i, len;

		len = strlen(path);
		यदि (len < max_prefix_len)
			अनुवर्त; /* outside of the prefix */
		यदि (!match_pathspec(istate, &pathspec, path, len,
				    max_prefix_len, ps_matched, 0))
			अनुवर्त; /* uninterested */
		प्रति (i = 0; i < 3; i++) {
			यदि (!ui->mode[i])
				अनुवर्त;
			printf("%s%06o %s %d\t", tag_resolve_undo, ui->mode[i],
			       repo_find_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(path);
		}
	}
}

स्थिर पूर्णाङ्क ce_excluded(संरचना dir_struct *dir, संरचना index_state *istate,
		       ध्रुव वर्ण *fullname, ध्रुव संरचना cache_entry *ce)
{
	पूर्णाङ्क dtype = ce_to_dtype(ce);
	प्रत्यागम is_excluded(dir, istate, fullname, &dtype);
}

स्थिर शून्य construct_fullname(संरचना strbuf *out, ध्रुव संरचना repository *repo,
			       ध्रुव संरचना cache_entry *ce)
{
	strbuf_reset(out);
	यदि (repo->submodule_prefix)
		strbuf_addstr(out, repo->submodule_prefix);
	strbuf_addstr(out, ce->name);
}

स्थिर शून्य show_files(संरचना repository *repo, संरचना dir_struct *dir)
{
	पूर्णाङ्क i;
	संरचना strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	यदि (show_others || show_killed) {
		यदि (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->index, &pathspec);
		यदि (show_others)
			show_other_files(repo->index, dir);
		यदि (show_killed)
			show_killed_files(repo->index, dir);
	}

	यदि (!(show_cached || show_stage || show_deleted || show_modified))
		प्रत्यागम;

	प्रति (i = 0; i < repo->index->cache_nr; i++) {
		ध्रुव संरचना cache_entry *ce = repo->index->cache[i];
		संरचना stat st;
		पूर्णाङ्क stat_err;

		यदि (S_ISSPARSEDIR(ce->ce_mode) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_full_index(repo->index);
			ce = repo->index->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		यदि ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->index, fullname.buf, ce))
			अनुवर्त;
		यदि (ce->ce_flags & CE_UPDATE)
			अनुवर्त;
		यदि ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? tag_unmerged :
				(ce_skip_worktree(ce) ? tag_skip_worktree :
				 tag_cached));
			यदि (skipping_duplicates)
				गच्छ skip_to_next_name;
		}

		यदि (!(show_deleted || show_modified))
			अनुवर्त;
		यदि (ce_skip_worktree(ce))
			अनुवर्त;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		यदि (pathspec.nr == 1 &&
		    !match_pathspec(repo->index, &pathspec, fullname.buf,
				    fullname.len, max_prefix_len, रिक्त,
				    S_ISDIR(ce->ce_mode) ||
				    S_ISGITLINK(ce->ce_mode)))
			अनुवर्त;
		stat_err = lstat(fullname.buf, &st);
		यदि (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		यदि (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, tag_removed);
			यदि (skipping_duplicates)
				गच्छ skip_to_next_name;
		}
		यदि (show_modified &&
		    (stat_err || ie_modified(repo->index, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, tag_modified);
			यदि (skipping_duplicates)
				गच्छ skip_to_next_name;
		}
		अनुवर्त;

skip_to_next_name:
		{
			पूर्णाङ्क j;
			संरचना cache_entry **cache = repo->index->cache;
			प्रति (j = i + 1; j < repo->index->cache_nr; j++)
				यदि (strcmp(ce->name, cache[j]->name))
					भङ्ग;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
स्थिर शून्य prune_index(संरचना index_state *istate,
			ध्रुव वर्ण *prefix, size_t prefixlen)
{
	पूर्णाङ्क pos;
	unsigned पूर्णाङ्क first, last;

	यदि (!prefix || !istate->cache_nr)
		प्रत्यागम;
	pos = index_name_pos(istate, prefix, prefixlen);
	यदि (pos < 0)
		pos = -pos-1;
	first = pos;
	last = istate->cache_nr;
	यावत् (last > first) {
		पूर्णाङ्क next = first + ((last - first) >> 1);
		ध्रुव संरचना cache_entry *ce = istate->cache[next];
		यदि (!strncmp(ce->name, prefix, prefixlen)) {
			first = next+1;
			अनुवर्त;
		}
		last = next;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, last - pos);
	istate->cache_nr = last - pos;
}

स्थिर पूर्णाङ्क get_common_prefix_len(ध्रुव वर्ण *common_prefix)
{
	पूर्णाङ्क common_prefix_len;

	यदि (!common_prefix)
		प्रत्यागम 0;

	common_prefix_len = strlen(common_prefix);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	यदि (common_prefix[common_prefix_len - 1] == '/')
		common_prefix_len--;

	प्रत्यागम common_prefix_len;
}

स्थिर ध्रुव वर्ण * ध्रुव ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	रिक्त
};

स्थिर पूर्णाङ्क option_parse_exclude(ध्रुव संरचना option *opt,
				ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना string_list *exclude_list = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_exclude_from(ध्रुव संरचना option *opt,
				     ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना dir_struct *dir = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	add_patterns_from_file(dir, arg);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_exclude_standard(ध्रुव संरचना option *opt,
					 ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना dir_struct *dir = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	setup_standard_excludes(dir);

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_ls_files(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *cmd_prefix,
		 संरचना repository *repo)
{
	पूर्णाङ्क require_work_वृक्ष = 0, show_tag = 0, i;
	वर्ण *max_prefix;
	संरचना dir_struct dir = DIR_INIT;
	संरचना pattern_list *pl;
	संरचना string_list exclude_list = STRING_LIST_INIT_NODUP;
	संरचना option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_SET_INT('z', रिक्त, &ओळ_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', रिक्त, &show_tag,
			N_("identify the file status with tags")),
		OPT_BOOL('v', रिक्त, &show_valid_bit,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', रिक्त, &show_fsmonitor_bit,
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
		OPT_CALLBACK_F(0, "exclude-standard", &dir, रिक्त,
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
	पूर्णाङ्क ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	prefix = cmd_prefix;
	यदि (prefix)
		prefix_len = strlen(prefix);
	repo_config(repo, git_default_config, रिक्त);

	यदि (repo_read_index(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, prefix, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	प्रति (i = 0; i < exclude_list.nr; i++) {
		add_pattern(exclude_list.वस्तुसंच[i].string, "", 0, pl, --exclude_args);
	}

	यदि (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_tag))
			usage_संदेश_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	यदि (show_tag || show_valid_bit || show_fsmonitor_bit) {
		tag_cached = "H ";
		tag_unmerged = "M ";
		tag_removed = "R ";
		tag_modified = "C ";
		tag_other = "? ";
		tag_killed = "K ";
		tag_skip_worktree = "S ";
		tag_resolve_undo = "U ";
	}
	यदि (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_वृक्ष = 1;
	यदि (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	यदि (show_tag || show_stage)
		skipping_duplicates = 0;
	यदि (dir.exclude_per_dir)
		exc_given = 1;

	यदि (require_work_वृक्ष && !is_inside_work_वृक्ष(repo))
		setup_work_वृक्ष(repo);

	यदि (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_वृक्ष))
		die("ls-files --recurse-submodules unsupported mode");

	यदि (recurse_submodules && error_unmatch)
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
	यदि (recurse_submodules)
		max_prefix = रिक्त;
	अन्यथा
		max_prefix = common_prefix(&pathspec);
	max_prefix_len = get_common_prefix_len(max_prefix);

	prune_index(repo->index, max_prefix, max_prefix_len);

	/* Treat unmatching pathspec elements as errors */
	यदि (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	यदि ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	यदि ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	यदि (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	यदि (with_वृक्ष) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		यदि (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_वृक्ष_on_index(repo->index, with_वृक्ष, max_prefix);
	}

	show_files(repo, &dir);

	यदि (show_resolve_undo)
		show_ru_info(repo, repo->index);

	यदि (ps_matched && report_path_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	मुक्त(max_prefix);
	प्रत्यागम ret;
}
