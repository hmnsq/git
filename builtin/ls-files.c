/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

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
स्थिर पूर्णाङ्क पङ्क्तिः_terminator = '\n';
स्थिर पूर्णाङ्क debug_बहुलकम्;
स्थिर पूर्णाङ्क show_eol;
स्थिर पूर्णाङ्क recurse_submodules;
स्थिर पूर्णाङ्क skipping_duplicates;
स्थिर पूर्णाङ्क show_sparse_dirs;

स्थिर ध्रुव वर्ण *उपसर्गः;
स्थिर पूर्णाङ्क max_उपसर्गः_len;
स्थिर पूर्णाङ्क उपसर्गः_len;
स्थिर संरचना pathspec pathspec;
स्थिर पूर्णाङ्क error_unmatch;
स्थिर वर्ण *ps_matched;
स्थिर ध्रुव वर्ण *with_वृक्षः;
स्थिर पूर्णाङ्क exc_given;
स्थिर पूर्णाङ्क exclude_तर्काः;
स्थिर ध्रुव वर्ण *format;

स्थिर ध्रुव वर्ण *चिह्नकम्_cached = "";
स्थिर ध्रुव वर्ण *चिह्नकम्_unmerged = "";
स्थिर ध्रुव वर्ण *चिह्नकम्_removed = "";
स्थिर ध्रुव वर्ण *चिह्नकम्_other = "";
स्थिर ध्रुव वर्ण *चिह्नकम्_killed = "";
स्थिर ध्रुव वर्ण *चिह्नकम्_modified = "";
स्थिर ध्रुव वर्ण *चिह्नकम्_skip_worktree = "";
स्थिर ध्रुव वर्ण *चिह्नकम्_resolve_undo = "";

स्थिर शून्य write_eolinfo(संरचना सूचकांक_state *istate,
			  ध्रुव संरचना cache_प्रविष्टिः *ce, ध्रुव वर्ण *मार्गः)
{
	यदि (show_eol) {
		संरचना stat st;
		ध्रुव वर्ण *i_txt = "";
		ध्रुव वर्ण *w_txt = "";
		ध्रुव वर्ण *a_txt = get_convert_attr_ascii(istate, मार्गः);
		यदि (ce && S_ISREG(ce->ce_बहुलकम्))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		यदि (!lstat(मार्गः, &st) && S_ISREG(st.st_बहुलकम्))
			w_txt = get_wt_convert_stats_ascii(मार्गः);
		प्रारूप_लिख("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

स्थिर शून्य write_name(ध्रुव वर्ण *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, उपसर्गः_len ? उपसर्गः : रिक्त,
				   stdout, पङ्क्तिः_terminator);
}

स्थिर शून्य write_name_गन्तव्यम्_buf(संरचना strbuf *sb, ध्रुव वर्ण *name)
{
	संरचना strbuf buf = STRBUF_INIT;
	ध्रुव वर्ण *rel = relative_मार्गः(name, उपसर्गः_len ? उपसर्गः : रिक्त, &buf);

	यदि (पङ्क्तिः_terminator)
		quote_c_style(rel, sb, रिक्त, 0);
	अन्यथा
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

स्थिर ध्रुव वर्ण *get_चिह्नकम्(ध्रुव संरचना cache_प्रविष्टिः *ce, ध्रुव वर्ण *चिह्नकम्)
{
	स्थिर वर्ण alttag[4];

	यदि (चिह्नकम् && *चिह्नकम् && ((show_valid_bit && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_bit && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		स्मृतिं_प्रतिलिख(alttag, चिह्नकम्, 3);

		यदि (अक्षरम्_वा(चिह्नकम्[0])) {
			alttag[0] = tolower(चिह्नकम्[0]);
		} अन्यथा यदि (चिह्नकम्[0] == '?') {
			alttag[0] = '!';
		} अन्यथा {
			alttag[0] = 'v';
			alttag[1] = चिह्नकम्[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		चिह्नकम् = alttag;
	}

	प्रत्यागम चिह्नकम्;
}

स्थिर शून्य print_debug(ध्रुव संरचना cache_प्रविष्टिः *ce)
{
	यदि (debug_बहुलकम्) {
		ध्रुव संरचना stat_दत्त *sd = &ce->ce_stat_दत्त;

		प्रारूप_लिख("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		प्रारूप_लिख("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		प्रारूप_लिख("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		प्रारूप_लिख("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		प्रारूप_लिख("  size: %u\tflags: %x\n", sd->sd_आकार, ce->ce_flags);
	}
}

स्थिर शून्य show_dir_प्रविष्टिः(संरचना सूचकांक_state *istate,
			   ध्रुव वर्ण *चिह्नकम्, संरचना dir_प्रविष्टिः *ent)
{
	पूर्णाङ्क len = max_उपसर्गः_len;

	यदि (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	यदि (ps_matched)
		dir_मार्गः_मेलनम्(istate, ent, &pathspec, len, ps_matched);

	fputs(चिह्नकम्, stdout);
	write_eolinfo(istate, रिक्त, ent->name);
	write_name(ent->name);
}

स्थिर शून्य show_other_files(संरचना सूचकांक_state *istate,
			     ध्रुव संरचना dir_struct *dir)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < dir->nr; i++) {
		संरचना dir_प्रविष्टिः *ent = dir->entries[i];
		यदि (!सूचकांक_name_is_other(istate, ent->name, ent->len))
			अनुवर्त;
		show_dir_प्रविष्टिः(istate, चिह्नकम्_other, ent);
	}
}

स्थिर शून्य show_killed_files(संरचना सूचकांक_state *istate,
			      ध्रुव संरचना dir_struct *dir)
{
	पूर्णाङ्क i;
	प्रति (i = 0; i < dir->nr; i++) {
		संरचना dir_प्रविष्टिः *ent = dir->entries[i];
		वर्ण *cp, *sp;
		पूर्णाङ्क pos, len, killed = 0;

		प्रति (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			यदि (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = सूचकांक_name_pos(istate, ent->name, ent->len);
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
				    !वाक्ये_तुलय(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				भङ्ग;
			}
			यदि (0 <= सूचकांक_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				भङ्ग;
			}
		}
		यदि (killed)
			show_dir_प्रविष्टिः(istate, चिह्नकम्_killed, dir->entries[i]);
	}
}

स्थिर शून्य show_files(संरचना repository *repo, संरचना dir_struct *dir);

स्थिर शून्य show_submodule(संरचना repository *superproject,
			   संरचना dir_struct *dir, ध्रुव वर्ण *मार्गः)
{
	संरचना repository subrepo;

	यदि (repo_submodule_init(&subrepo, superproject, मार्गः,
				null_oid(superproject->hash_algo)))
		प्रत्यागम;

	यदि (repo_read_सूचकांक(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

स्थिर शून्य expand_objectsize(संरचना repository *repo, संरचना strbuf *पङ्क्तिः,
			      ध्रुव संरचना object_id *oid,
			      ध्रुव गणना object_type type, अचिह्नित पूर्णाङ्क padded)
{
	स्थिर ध्रुव वर्ण padding[] = "       ";
	आकार_t min_len = padded ? वाक्यदैर्घ्यम्(padding) : 0;
	आकार_t orig_len = पङ्क्तिः->len;
	आकार_t len;

	यदि (type == OBJ_BLOB) {
		आकार_t आकार;
		यदि (odb_read_object_info(repo->objects, oid, &आकार) < 0)
			die(_("could not get object info about '%s'"),
			    oid_गन्तव्यम्_hex(oid));
		strbuf_सङ्कलय_uint(पङ्क्तिः, आकार);
	} अन्यथा {
		strbuf_addstr(पङ्क्तिः, "-");
	}
	len = पङ्क्तिः->len - orig_len;
	यदि (len < min_len)
		strbuf_निवेशय(पङ्क्तिः, orig_len, padding, min_len - len);
}

स्थिर शून्य show_ce_fmt(संरचना repository *repo, ध्रुव संरचना cache_प्रविष्टिः *ce,
			ध्रुव वर्ण *format, ध्रुव वर्ण *fullname) {
	संरचना strbuf sb = STRBUF_INIT;

	यावत् (strbuf_expand_चरणः(&sb, &format)) {
		आकार_t len;
		संरचना stat st;

		यदि (skip_उपसर्गः(format, "%", &format))
			strbuf_addch(&sb, '%');
		अन्यथा यदि ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		अन्यथा यदि (skip_उपसर्गः(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_बहुलकम्);
		अन्यथा यदि (skip_उपसर्गः(format, "(objectname)", &format))
			strbuf_सङ्कलय_unique_abbrev(&sb, &ce->oid, abbrev);
		अन्यथा यदि (skip_उपसर्गः(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_बहुलकम्)));
		अन्यथा यदि (skip_उपसर्गः(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_बहुलकम्), 1);
		अन्यथा यदि (skip_उपसर्गः(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_बहुलकम्), 0);
		अन्यथा यदि (skip_उपसर्गः(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		अन्यथा यदि (skip_उपसर्गः(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_बहुलकम्) ?
				      get_cached_convert_stats_ascii(repo->सूचकांक,
				      ce->name) : "");
		अन्यथा यदि (skip_उपसर्गः(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_बहुलकम्) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		अन्यथा यदि (skip_उपसर्गः(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->सूचकांक,
								 fullname));
		अन्यथा यदि (skip_उपसर्गः(format, "(path)", &format))
			write_name_गन्तव्यम्_buf(&sb, fullname);
		अन्यथा
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, पङ्क्तिः_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

स्थिर शून्य show_ce(संरचना repository *repo, संरचना dir_struct *dir,
		    ध्रुव संरचना cache_प्रविष्टिः *ce, ध्रुव वर्ण *fullname,
		    ध्रुव वर्ण *चिह्नकम्)
{
	यदि (max_उपसर्गः_len > वाक्यदैर्घ्यम्(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	यदि (recurse_submodules && S_ISGITLINK(ce->ce_बहुलकम्) &&
	    is_submodule_सक्रिय(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} अन्यथा यदि (मेलनम्_pathspec(repo->सूचकांक, &pathspec, fullname, वाक्यदैर्घ्यम्(fullname),
				  max_उपसर्गः_len, ps_matched,
				  S_ISDIR(ce->ce_बहुलकम्) ||
				  S_ISGITLINK(ce->ce_बहुलकम्))) {
		यदि (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			प्रत्यागम;
		}

		चिह्नकम् = get_चिह्नकम्(ce, चिह्नकम्);

		यदि (!show_stage) {
			fputs(चिह्नकम्, stdout);
		} अन्यथा {
			प्रारूप_लिख("%s%06o %s %d\t",
			       चिह्नकम्,
			       ce->ce_बहुलकम्,
			       repo_find_unique_abbrev(repo, &ce->oid, abbrev),
			       ce_stage(ce));
		}
		write_eolinfo(repo->सूचकांक, ce, fullname);
		write_name(fullname);
		print_debug(ce);
	}
}

स्थिर शून्य show_ru_info(संरचना repository *repo, संरचना सूचकांक_state *istate)
{
	संरचना string_list_item *item;

	यदि (!istate->resolve_undo)
		प्रत्यागम;

	for_each_string_list_item(item, istate->resolve_undo) {
		ध्रुव वर्ण *मार्गः = item->string;
		संरचना resolve_undo_info *ui = item->util;
		पूर्णाङ्क i, len;

		len = वाक्यदैर्घ्यम्(मार्गः);
		यदि (len < max_उपसर्गः_len)
			अनुवर्त; /* outside of the prefix */
		यदि (!मेलनम्_pathspec(istate, &pathspec, मार्गः, len,
				    max_उपसर्गः_len, ps_matched, 0))
			अनुवर्त; /* uninterested */
		प्रति (i = 0; i < 3; i++) {
			यदि (!ui->बहुलकम्[i])
				अनुवर्त;
			प्रारूप_लिख("%s%06o %s %d\t", चिह्नकम्_resolve_undo, ui->बहुलकम्[i],
			       repo_find_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(मार्गः);
		}
	}
}

स्थिर पूर्णाङ्क ce_excluded(संरचना dir_struct *dir, संरचना सूचकांक_state *istate,
		       ध्रुव वर्ण *fullname, ध्रुव संरचना cache_प्रविष्टिः *ce)
{
	पूर्णाङ्क dtype = ce_गन्तव्यम्_dtype(ce);
	प्रत्यागम is_excluded(dir, istate, fullname, &dtype);
}

स्थिर शून्य construct_fullname(संरचना strbuf *बहिर्गमनम्, ध्रुव संरचना repository *repo,
			       ध्रुव संरचना cache_प्रविष्टिः *ce)
{
	strbuf_reset(बहिर्गमनम्);
	यदि (repo->submodule_उपसर्गः)
		strbuf_addstr(बहिर्गमनम्, repo->submodule_उपसर्गः);
	strbuf_addstr(बहिर्गमनम्, ce->name);
}

स्थिर शून्य show_files(संरचना repository *repo, संरचना dir_struct *dir)
{
	पूर्णाङ्क i;
	संरचना strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	यदि (show_others || show_killed) {
		यदि (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->सूचकांक, &pathspec);
		यदि (show_others)
			show_other_files(repo->सूचकांक, dir);
		यदि (show_killed)
			show_killed_files(repo->सूचकांक, dir);
	}

	यदि (!(show_cached || show_stage || show_deleted || show_modified))
		प्रत्यागम;

	प्रति (i = 0; i < repo->सूचकांक->cache_nr; i++) {
		ध्रुव संरचना cache_प्रविष्टिः *ce = repo->सूचकांक->cache[i];
		संरचना stat st;
		पूर्णाङ्क stat_err;

		यदि (S_ISSPARSEDIR(ce->ce_बहुलकम्) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_पूर्णम्_सूचकांक(repo->सूचकांक);
			ce = repo->सूचकांक->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		यदि ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->सूचकांक, fullname.buf, ce))
			अनुवर्त;
		यदि (ce->ce_flags & CE_UPDATE)
			अनुवर्त;
		यदि ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? चिह्नकम्_unmerged :
				(ce_skip_worktree(ce) ? चिह्नकम्_skip_worktree :
				 चिह्नकम्_cached));
			यदि (skipping_duplicates)
				गच्छ skip_गन्तव्यम्_अग्र_name;
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
		    !मेलनम्_pathspec(repo->सूचकांक, &pathspec, fullname.buf,
				    fullname.len, max_उपसर्गः_len, रिक्त,
				    S_ISDIR(ce->ce_बहुलकम्) ||
				    S_ISGITLINK(ce->ce_बहुलकम्)))
			अनुवर्त;
		stat_err = lstat(fullname.buf, &st);
		यदि (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		यदि (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, चिह्नकम्_removed);
			यदि (skipping_duplicates)
				गच्छ skip_गन्तव्यम्_अग्र_name;
		}
		यदि (show_modified &&
		    (stat_err || ie_modified(repo->सूचकांक, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, चिह्नकम्_modified);
			यदि (skipping_duplicates)
				गच्छ skip_गन्तव्यम्_अग्र_name;
		}
		अनुवर्त;

skip_गन्तव्यम्_अग्र_name:
		{
			पूर्णाङ्क j;
			संरचना cache_प्रविष्टिः **cache = repo->सूचकांक->cache;
			प्रति (j = i + 1; j < repo->सूचकांक->cache_nr; j++)
				यदि (वाक्यं_तुलय(ce->name, cache[j]->name))
					भङ्ग;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
स्थिर शून्य prune_सूचकांक(संरचना सूचकांक_state *istate,
			ध्रुव वर्ण *उपसर्गः, आकार_t prefixlen)
{
	पूर्णाङ्क pos;
	अचिह्नित पूर्णाङ्क first, चरमः;

	यदि (!उपसर्गः || !istate->cache_nr)
		प्रत्यागम;
	pos = सूचकांक_name_pos(istate, उपसर्गः, prefixlen);
	यदि (pos < 0)
		pos = -pos-1;
	first = pos;
	चरमः = istate->cache_nr;
	यावत् (चरमः > first) {
		पूर्णाङ्क अग्र = first + ((चरमः - first) >> 1);
		ध्रुव संरचना cache_प्रविष्टिः *ce = istate->cache[अग्र];
		यदि (!वाक्ये_तुलय(ce->name, उपसर्गः, prefixlen)) {
			first = अग्र+1;
			अनुवर्त;
		}
		चरमः = अग्र;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, चरमः - pos);
	istate->cache_nr = चरमः - pos;
}

स्थिर पूर्णाङ्क get_common_उपसर्गः_len(ध्रुव वर्ण *common_उपसर्गः)
{
	पूर्णाङ्क common_उपसर्गः_len;

	यदि (!common_उपसर्गः)
		प्रत्यागम 0;

	common_उपसर्गः_len = वाक्यदैर्घ्यम्(common_उपसर्गः);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	यदि (common_उपसर्गः[common_उपसर्गः_len - 1] == '/')
		common_उपसर्गः_len--;

	प्रत्यागम common_उपसर्गः_len;
}

स्थिर ध्रुव वर्ण * ध्रुव ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	रिक्त
};

स्थिर पूर्णाङ्क option_parse_exclude(ध्रुव संरचना option *opt,
				ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना string_list *exclude_list = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_exclude_from(ध्रुव संरचना option *opt,
				     ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना dir_struct *dir = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	सङ्कलय_patterns_from_file(dir, arg);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_exclude_standard(ध्रुव संरचना option *opt,
					 ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना dir_struct *dir = opt->मानम्;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	सज्जता_standard_excludes(dir);

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_ls_files(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *cmd_उपसर्गः,
		 संरचना repository *repo)
{
	पूर्णाङ्क require_work_वृक्षः = 0, show_चिह्नकम् = 0, i;
	वर्ण *max_उपसर्गः;
	संरचना dir_struct dir = DIR_INIT;
	संरचना आदर्शः_list *pl;
	संरचना string_list exclude_list = STRING_LIST_INIT_NODUP;
	संरचना option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_समुच्चयः_INT('z', रिक्त, &पङ्क्तिः_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', रिक्त, &show_चिह्नकम्,
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
			DIR_HIDE_रिक्तानि_DIRECTORIES),
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
		OPT_STRING(0, "exclude-per-directory", &dir.exclude_प्रत्येकम्_dir, N_("file"),
			N_("read additional per-directory exclude patterns in <file>")),
		OPT_CALLBACK_F(0, "exclude-standard", &dir, रिक्त,
			N_("add the standard git exclusions"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			option_parse_exclude_standard),
		OPT_समुच्चयः_INT_F(0, "full-name", &उपसर्गः_len,
			      N_("make the output relative to the project top directory"),
			      0, PARSE_OPT_NONEG),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			N_("recurse through submodules")),
		OPT_BOOL(0, "error-unmatch", &error_unmatch,
			N_("if any <file> is not in the index, treat this as an error")),
		OPT_STRING(0, "with-tree", &with_वृक्षः, N_("tree-ish"),
			N_("pretend that paths removed since <tree-ish> are still present")),
		OPT__ABBREV(&abbrev),
		OPT_BOOL(0, "debug", &debug_बहुलकम्, N_("show debugging data")),
		OPT_BOOL(0, "deduplicate", &skipping_duplicates,
			 N_("suppress duplicate entries")),
		OPT_BOOL(0, "sparse", &show_sparse_dirs,
			 N_("show sparse directories in the presence of a sparse index")),
		OPT_STRING_F(0, "format", &format, N_("format"),
			     N_("format to use for the output"),
			     PARSE_OPT_NONEG),
		OPT_अन्तः()
	};
	पूर्णाङ्क ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_पूर्णम्_सूचकांक = 0;

	उपसर्गः = cmd_उपसर्गः;
	यदि (उपसर्गः)
		उपसर्गः_len = वाक्यदैर्घ्यम्(उपसर्गः);
	repo_config(repo, git_default_config, रिक्त);

	यदि (repo_read_सूचकांक(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, उपसर्गः, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = सङ्कलय_आदर्शः_list(&dir, EXC_CMDL, "--exclude option");
	प्रति (i = 0; i < exclude_list.nr; i++) {
		सङ्कलय_आदर्शः(exclude_list.वस्तूनि[i].string, "", 0, pl, --exclude_तर्काः);
	}

	यदि (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_चिह्नकम्))
			usage_सन्देशः_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	यदि (show_चिह्नकम् || show_valid_bit || show_fsmonitor_bit) {
		चिह्नकम्_cached = "H ";
		चिह्नकम्_unmerged = "M ";
		चिह्नकम्_removed = "R ";
		चिह्नकम्_modified = "C ";
		चिह्नकम्_other = "? ";
		चिह्नकम्_killed = "K ";
		चिह्नकम्_skip_worktree = "S ";
		चिह्नकम्_resolve_undo = "U ";
	}
	यदि (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_वृक्षः = 1;
	यदि (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	यदि (show_चिह्नकम् || show_stage)
		skipping_duplicates = 0;
	यदि (dir.exclude_प्रत्येकम्_dir)
		exc_given = 1;

	यदि (require_work_वृक्षः && !is_inside_work_वृक्षः(repo))
		सज्जता_work_वृक्षः(repo);

	यदि (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_वृक्षः))
		die("ls-files --recurse-submodules unsupported mode");

	यदि (recurse_submodules && error_unmatch)
		die("ls-files --recurse-submodules does not support "
		    "--error-unmatch");

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       उपसर्गः, argv);

	/*
	 * Find common prefix for all pathspec's
	 * This is used as a performance optimization which unfortunately cannot
	 * be done when recursing into submodules because when a pathspec is
	 * given which spans repository boundaries you can't simply remove the
	 * submodule entry because the pathspec may match something inside the
	 * submodule.
	 */
	यदि (recurse_submodules)
		max_उपसर्गः = रिक्त;
	अन्यथा
		max_उपसर्गः = common_उपसर्गः(&pathspec);
	max_उपसर्गः_len = get_common_उपसर्गः_len(max_उपसर्गः);

	prune_सूचकांक(repo->सूचकांक, max_उपसर्गः, max_उपसर्गः_len);

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

	यदि (with_वृक्षः) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		यदि (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_वृक्षः_on_सूचकांक(repo->सूचकांक, with_वृक्षः, max_उपसर्गः);
	}

	show_files(repo, &dir);

	यदि (show_resolve_undo)
		show_ru_info(repo, repo->सूचकांक);

	यदि (ps_matched && report_मार्गः_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	मुक्त(max_उपसर्गः);
	प्रत्यागम ret;
}
