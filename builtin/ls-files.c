/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "convert.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "resolve-undo.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "pathspec.h"
#અન્તર્ભાવ "read-cache.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "sparse-index.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "hex.h"


સ્થિર પૂર્ણાંક abbrev;
સ્થિર પૂર્ણાંક show_deleted;
સ્થિર પૂર્ણાંક show_cached;
સ્થિર પૂર્ણાંક show_others;
સ્થિર પૂર્ણાંક show_stage;
સ્થિર પૂર્ણાંક show_unmerged;
સ્થિર પૂર્ણાંક show_resolve_undo;
સ્થિર પૂર્ણાંક show_modified;
સ્થિર પૂર્ણાંક show_killed;
સ્થિર પૂર્ણાંક show_valid_bit;
સ્થિર પૂર્ણાંક show_fsmonitor_bit;
સ્થિર પૂર્ણાંક લીટી_terminator = '\n';
સ્થિર પૂર્ણાંક debug_mode;
સ્થિર પૂર્ણાંક show_eol;
સ્થિર પૂર્ણાંક recurse_submodules;
સ્થિર પૂર્ણાંક skipping_duplicates;
સ્થિર પૂર્ણાંક show_sparse_dirs;

સ્થિર અચલ અક્ષર *prefix;
સ્થિર પૂર્ણાંક max_prefix_len;
સ્થિર પૂર્ણાંક prefix_len;
સ્થિર સંરચના pathspec pathspec;
સ્થિર પૂર્ણાંક error_unmatch;
સ્થિર અક્ષર *ps_matched;
સ્થિર અચલ અક્ષર *with_વૃક્ષ;
સ્થિર પૂર્ણાંક exc_given;
સ્થિર પૂર્ણાંક exclude_args;
સ્થિર અચલ અક્ષર *format;

સ્થિર અચલ અક્ષર *tag_cached = "";
સ્થિર અચલ અક્ષર *tag_unmerged = "";
સ્થિર અચલ અક્ષર *tag_removed = "";
સ્થિર અચલ અક્ષર *tag_other = "";
સ્થિર અચલ અક્ષર *tag_killed = "";
સ્થિર અચલ અક્ષર *tag_modified = "";
સ્થિર અચલ અક્ષર *tag_skip_worktree = "";
સ્થિર અચલ અક્ષર *tag_resolve_undo = "";

સ્થિર શૂન્ય write_eolinfo(સંરચના index_state *istate,
			  અચલ સંરચના cache_entry *ce, અચલ અક્ષર *path)
{
	જો (show_eol) {
		સંરચના stat st;
		અચલ અક્ષર *i_txt = "";
		અચલ અક્ષર *w_txt = "";
		અચલ અક્ષર *a_txt = get_convert_attr_ascii(istate, path);
		જો (ce && S_ISREG(ce->ce_mode))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		જો (!lstat(path, &st) && S_ISREG(st.st_mode))
			w_txt = get_wt_convert_stats_ascii(path);
		printf("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

સ્થિર શૂન્ય write_name(અચલ અક્ષર *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, prefix_len ? prefix : ખાલી,
				   stdout, લીટી_terminator);
}

સ્થિર શૂન્ય write_name_to_buf(સંરચના strbuf *sb, અચલ અક્ષર *name)
{
	સંરચના strbuf buf = STRBUF_INIT;
	અચલ અક્ષર *rel = relative_path(name, prefix_len ? prefix : ખાલી, &buf);

	જો (લીટી_terminator)
		quote_c_style(rel, sb, ખાલી, 0);
	નહીંતર
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

સ્થિર અચલ અક્ષર *get_tag(અચલ સંરચના cache_entry *ce, અચલ અક્ષર *tag)
{
	સ્થિર અક્ષર alttag[4];

	જો (tag && *tag && ((show_valid_bit && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_bit && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		memcpy(alttag, tag, 3);

		જો (isalpha(tag[0])) {
			alttag[0] = tolower(tag[0]);
		} નહીંતર જો (tag[0] == '?') {
			alttag[0] = '!';
		} નહીંતર {
			alttag[0] = 'v';
			alttag[1] = tag[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		tag = alttag;
	}

	પાછા tag;
}

સ્થિર શૂન્ય print_debug(અચલ સંરચના cache_entry *ce)
{
	જો (debug_mode) {
		અચલ સંરચના stat_data *sd = &ce->ce_stat_data;

		printf("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		printf("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		printf("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		printf("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		printf("  size: %u\tflags: %x\n", sd->sd_size, ce->ce_flags);
	}
}

સ્થિર શૂન્ય show_dir_entry(સંરચના index_state *istate,
			   અચલ અક્ષર *tag, સંરચના dir_entry *ent)
{
	પૂર્ણાંક len = max_prefix_len;

	જો (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	જો (ps_matched)
		dir_path_match(istate, ent, &pathspec, len, ps_matched);

	fputs(tag, stdout);
	write_eolinfo(istate, ખાલી, ent->name);
	write_name(ent->name);
}

સ્થિર શૂન્ય show_other_files(સંરચના index_state *istate,
			     અચલ સંરચના dir_struct *dir)
{
	પૂર્ણાંક i;

	માટે (i = 0; i < dir->nr; i++) {
		સંરચના dir_entry *ent = dir->entries[i];
		જો (!index_name_is_other(istate, ent->name, ent->len))
			ચાલુ;
		show_dir_entry(istate, tag_other, ent);
	}
}

સ્થિર શૂન્ય show_killed_files(સંરચના index_state *istate,
			      અચલ સંરચના dir_struct *dir)
{
	પૂર્ણાંક i;
	માટે (i = 0; i < dir->nr; i++) {
		સંરચના dir_entry *ent = dir->entries[i];
		અક્ષર *cp, *sp;
		પૂર્ણાંક pos, len, killed = 0;

		માટે (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			જો (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = index_name_pos(istate, ent->name, ent->len);
				જો (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				જ્યારે (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				જો (istate->cache_nr <= pos)
					તોડો;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				જો ((ent->len < len) &&
				    !strncmp(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				તોડો;
			}
			જો (0 <= index_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				તોડો;
			}
		}
		જો (killed)
			show_dir_entry(istate, tag_killed, dir->entries[i]);
	}
}

સ્થિર શૂન્ય show_files(સંરચના repository *repo, સંરચના dir_struct *dir);

સ્થિર શૂન્ય show_submodule(સંરચના repository *superproject,
			   સંરચના dir_struct *dir, અચલ અક્ષર *path)
{
	સંરચના repository subrepo;

	જો (repo_submodule_init(&subrepo, superproject, path,
				null_oid(superproject->hash_algo)))
		પાછા;

	જો (repo_read_index(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

સ્થિર શૂન્ય expand_objectsize(સંરચના repository *repo, સંરચના strbuf *લીટી,
			      અચલ સંરચના object_id *oid,
			      અચલ ગણના object_type type, unsigned પૂર્ણાંક padded)
{
	સ્થિર અચલ અક્ષર padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = લીટી->len;
	size_t len;

	જો (type == OBJ_BLOB) {
		size_t size;
		જો (odb_read_object_info(repo->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(લીટી, size);
	} નહીંતર {
		strbuf_addstr(લીટી, "-");
	}
	len = લીટી->len - orig_len;
	જો (len < min_len)
		strbuf_ઉમેરો(લીટી, orig_len, padding, min_len - len);
}

સ્થિર શૂન્ય show_ce_fmt(સંરચના repository *repo, અચલ સંરચના cache_entry *ce,
			અચલ અક્ષર *format, અચલ અક્ષર *fullname) {
	સંરચના strbuf sb = STRBUF_INIT;

	જ્યારે (strbuf_expand_step(&sb, &format)) {
		size_t len;
		સંરચના stat st;

		જો (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		નહીંતર જો ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		નહીંતર જો (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_mode);
		નહીંતર જો (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, &ce->oid, abbrev);
		નહીંતર જો (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_mode)));
		નહીંતર જો (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 1);
		નહીંતર જો (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 0);
		નહીંતર જો (skip_prefix(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		નહીંતર જો (skip_prefix(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_mode) ?
				      get_cached_convert_stats_ascii(repo->index,
				      ce->name) : "");
		નહીંતર જો (skip_prefix(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_mode) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		નહીંતર જો (skip_prefix(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->index,
								 fullname));
		નહીંતર જો (skip_prefix(format, "(path)", &format))
			write_name_to_buf(&sb, fullname);
		નહીંતર
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, લીટી_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

સ્થિર શૂન્ય show_ce(સંરચના repository *repo, સંરચના dir_struct *dir,
		    અચલ સંરચના cache_entry *ce, અચલ અક્ષર *fullname,
		    અચલ અક્ષર *tag)
{
	જો (max_prefix_len > strlen(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	જો (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
	    is_submodule_active(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} નહીંતર જો (match_pathspec(repo->index, &pathspec, fullname, strlen(fullname),
				  max_prefix_len, ps_matched,
				  S_ISDIR(ce->ce_mode) ||
				  S_ISGITLINK(ce->ce_mode))) {
		જો (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			પાછા;
		}

		tag = get_tag(ce, tag);

		જો (!show_stage) {
			fputs(tag, stdout);
		} નહીંતર {
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

સ્થિર શૂન્ય show_ru_info(સંરચના repository *repo, સંરચના index_state *istate)
{
	સંરચના string_list_item *item;

	જો (!istate->resolve_undo)
		પાછા;

	for_each_string_list_item(item, istate->resolve_undo) {
		અચલ અક્ષર *path = item->string;
		સંરચના resolve_undo_info *ui = item->util;
		પૂર્ણાંક i, len;

		len = strlen(path);
		જો (len < max_prefix_len)
			ચાલુ; /* outside of the prefix */
		જો (!match_pathspec(istate, &pathspec, path, len,
				    max_prefix_len, ps_matched, 0))
			ચાલુ; /* uninterested */
		માટે (i = 0; i < 3; i++) {
			જો (!ui->mode[i])
				ચાલુ;
			printf("%s%06o %s %d\t", tag_resolve_undo, ui->mode[i],
			       repo_find_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(path);
		}
	}
}

સ્થિર પૂર્ણાંક ce_excluded(સંરચના dir_struct *dir, સંરચના index_state *istate,
		       અચલ અક્ષર *fullname, અચલ સંરચના cache_entry *ce)
{
	પૂર્ણાંક dtype = ce_to_dtype(ce);
	પાછા is_excluded(dir, istate, fullname, &dtype);
}

સ્થિર શૂન્ય construct_fullname(સંરચના strbuf *out, અચલ સંરચના repository *repo,
			       અચલ સંરચના cache_entry *ce)
{
	strbuf_reset(out);
	જો (repo->submodule_prefix)
		strbuf_addstr(out, repo->submodule_prefix);
	strbuf_addstr(out, ce->name);
}

સ્થિર શૂન્ય show_files(સંરચના repository *repo, સંરચના dir_struct *dir)
{
	પૂર્ણાંક i;
	સંરચના strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	જો (show_others || show_killed) {
		જો (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->index, &pathspec);
		જો (show_others)
			show_other_files(repo->index, dir);
		જો (show_killed)
			show_killed_files(repo->index, dir);
	}

	જો (!(show_cached || show_stage || show_deleted || show_modified))
		પાછા;

	માટે (i = 0; i < repo->index->cache_nr; i++) {
		અચલ સંરચના cache_entry *ce = repo->index->cache[i];
		સંરચના stat st;
		પૂર્ણાંક stat_err;

		જો (S_ISSPARSEDIR(ce->ce_mode) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_full_index(repo->index);
			ce = repo->index->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		જો ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->index, fullname.buf, ce))
			ચાલુ;
		જો (ce->ce_flags & CE_UPDATE)
			ચાલુ;
		જો ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? tag_unmerged :
				(ce_skip_worktree(ce) ? tag_skip_worktree :
				 tag_cached));
			જો (skipping_duplicates)
				જાઓ skip_to_next_name;
		}

		જો (!(show_deleted || show_modified))
			ચાલુ;
		જો (ce_skip_worktree(ce))
			ચાલુ;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		જો (pathspec.nr == 1 &&
		    !match_pathspec(repo->index, &pathspec, fullname.buf,
				    fullname.len, max_prefix_len, ખાલી,
				    S_ISDIR(ce->ce_mode) ||
				    S_ISGITLINK(ce->ce_mode)))
			ચાલુ;
		stat_err = lstat(fullname.buf, &st);
		જો (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		જો (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, tag_removed);
			જો (skipping_duplicates)
				જાઓ skip_to_next_name;
		}
		જો (show_modified &&
		    (stat_err || ie_modified(repo->index, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, tag_modified);
			જો (skipping_duplicates)
				જાઓ skip_to_next_name;
		}
		ચાલુ;

skip_to_next_name:
		{
			પૂર્ણાંક j;
			સંરચના cache_entry **cache = repo->index->cache;
			માટે (j = i + 1; j < repo->index->cache_nr; j++)
				જો (strcmp(ce->name, cache[j]->name))
					તોડો;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
સ્થિર શૂન્ય prune_index(સંરચના index_state *istate,
			અચલ અક્ષર *prefix, size_t prefixlen)
{
	પૂર્ણાંક pos;
	unsigned પૂર્ણાંક first, last;

	જો (!prefix || !istate->cache_nr)
		પાછા;
	pos = index_name_pos(istate, prefix, prefixlen);
	જો (pos < 0)
		pos = -pos-1;
	first = pos;
	last = istate->cache_nr;
	જ્યારે (last > first) {
		પૂર્ણાંક next = first + ((last - first) >> 1);
		અચલ સંરચના cache_entry *ce = istate->cache[next];
		જો (!strncmp(ce->name, prefix, prefixlen)) {
			first = next+1;
			ચાલુ;
		}
		last = next;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, last - pos);
	istate->cache_nr = last - pos;
}

સ્થિર પૂર્ણાંક get_common_prefix_len(અચલ અક્ષર *common_prefix)
{
	પૂર્ણાંક common_prefix_len;

	જો (!common_prefix)
		પાછા 0;

	common_prefix_len = strlen(common_prefix);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	જો (common_prefix[common_prefix_len - 1] == '/')
		common_prefix_len--;

	પાછા common_prefix_len;
}

સ્થિર અચલ અક્ષર * અચલ ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	ખાલી
};

સ્થિર પૂર્ણાંક option_parse_exclude(અચલ સંરચના option *opt,
				અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના string_list *exclude_list = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_exclude_from(અચલ સંરચના option *opt,
				     અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના dir_struct *dir = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	add_patterns_from_file(dir, arg);

	પાછા 0;
}

સ્થિર પૂર્ણાંક option_parse_exclude_standard(અચલ સંરચના option *opt,
					 અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના dir_struct *dir = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	setup_standard_excludes(dir);

	પાછા 0;
}

પૂર્ણાંક cmd_ls_files(પૂર્ણાંક argc,
		 અચલ અક્ષર **argv,
		 અચલ અક્ષર *cmd_prefix,
		 સંરચના repository *repo)
{
	પૂર્ણાંક require_work_વૃક્ષ = 0, show_tag = 0, i;
	અક્ષર *max_prefix;
	સંરચના dir_struct dir = DIR_INIT;
	સંરચના pattern_list *pl;
	સંરચના string_list exclude_list = STRING_LIST_INIT_NODUP;
	સંરચના option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_SET_INT('z', ખાલી, &લીટી_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', ખાલી, &show_tag,
			N_("identify the file status with tags")),
		OPT_BOOL('v', ખાલી, &show_valid_bit,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', ખાલી, &show_fsmonitor_bit,
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
		OPT_CALLBACK_F(0, "exclude-standard", &dir, ખાલી,
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
		OPT_STRING(0, "with-tree", &with_વૃક્ષ, N_("tree-ish"),
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
	પૂર્ણાંક ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	prefix = cmd_prefix;
	જો (prefix)
		prefix_len = strlen(prefix);
	repo_config(repo, git_default_config, ખાલી);

	જો (repo_read_index(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, prefix, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	માટે (i = 0; i < exclude_list.nr; i++) {
		add_pattern(exclude_list.વસ્તુઓ[i].string, "", 0, pl, --exclude_args);
	}

	જો (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_tag))
			usage_સંદેશ_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	જો (show_tag || show_valid_bit || show_fsmonitor_bit) {
		tag_cached = "H ";
		tag_unmerged = "M ";
		tag_removed = "R ";
		tag_modified = "C ";
		tag_other = "? ";
		tag_killed = "K ";
		tag_skip_worktree = "S ";
		tag_resolve_undo = "U ";
	}
	જો (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_વૃક્ષ = 1;
	જો (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	જો (show_tag || show_stage)
		skipping_duplicates = 0;
	જો (dir.exclude_per_dir)
		exc_given = 1;

	જો (require_work_વૃક્ષ && !is_inside_work_વૃક્ષ(repo))
		setup_work_વૃક્ષ(repo);

	જો (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_વૃક્ષ))
		die("ls-files --recurse-submodules unsupported mode");

	જો (recurse_submodules && error_unmatch)
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
	જો (recurse_submodules)
		max_prefix = ખાલી;
	નહીંતર
		max_prefix = common_prefix(&pathspec);
	max_prefix_len = get_common_prefix_len(max_prefix);

	prune_index(repo->index, max_prefix, max_prefix_len);

	/* Treat unmatching pathspec elements as errors */
	જો (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	જો ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	જો ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	જો (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	જો (with_વૃક્ષ) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		જો (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_વૃક્ષ_on_index(repo->index, with_વૃક્ષ, max_prefix);
	}

	show_files(repo, &dir);

	જો (show_resolve_undo)
		show_ru_info(repo, repo->index);

	જો (ps_matched && report_path_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	મુક્ત(max_prefix);
	પાછા ret;
}
