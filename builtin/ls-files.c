/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "convert.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "resolve-undo.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "pathspec.h"
#அந்தர்பாவ "read-cache.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "sparse-index.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "hex.h"


நிலையான முழு abbrev;
நிலையான முழு show_deleted;
நிலையான முழு show_cached;
நிலையான முழு show_others;
நிலையான முழு show_stage;
நிலையான முழு show_unmerged;
நிலையான முழு show_resolve_undo;
நிலையான முழு show_modified;
நிலையான முழு show_killed;
நிலையான முழு show_valid_bit;
நிலையான முழு show_fsmonitor_bit;
நிலையான முழு வரி_terminator = '\n';
நிலையான முழு debug_mode;
நிலையான முழு show_eol;
நிலையான முழு recurse_submodules;
நிலையான முழு skipping_duplicates;
நிலையான முழு show_sparse_dirs;

நிலையான மாறிலி எழுத்து *prefix;
நிலையான முழு max_prefix_len;
நிலையான முழு prefix_len;
நிலையான கட்டமைப்பு pathspec pathspec;
நிலையான முழு error_unmatch;
நிலையான எழுத்து *ps_matched;
நிலையான மாறிலி எழுத்து *with_மரம்;
நிலையான முழு exc_given;
நிலையான முழு exclude_args;
நிலையான மாறிலி எழுத்து *format;

நிலையான மாறிலி எழுத்து *tag_cached = "";
நிலையான மாறிலி எழுத்து *tag_unmerged = "";
நிலையான மாறிலி எழுத்து *tag_removed = "";
நிலையான மாறிலி எழுத்து *tag_other = "";
நிலையான மாறிலி எழுத்து *tag_killed = "";
நிலையான மாறிலி எழுத்து *tag_modified = "";
நிலையான மாறிலி எழுத்து *tag_skip_worktree = "";
நிலையான மாறிலி எழுத்து *tag_resolve_undo = "";

நிலையான வெறுமை write_eolinfo(கட்டமைப்பு index_state *istate,
			  மாறிலி கட்டமைப்பு cache_entry *ce, மாறிலி எழுத்து *path)
{
	எனில் (show_eol) {
		கட்டமைப்பு stat st;
		மாறிலி எழுத்து *i_txt = "";
		மாறிலி எழுத்து *w_txt = "";
		மாறிலி எழுத்து *a_txt = get_convert_attr_ascii(istate, path);
		எனில் (ce && S_ISREG(ce->ce_mode))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		எனில் (!lstat(path, &st) && S_ISREG(st.st_mode))
			w_txt = get_wt_convert_stats_ascii(path);
		printf("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

நிலையான வெறுமை write_name(மாறிலி எழுத்து *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, prefix_len ? prefix : வெற்று,
				   stdout, வரி_terminator);
}

நிலையான வெறுமை write_name_to_buf(கட்டமைப்பு strbuf *sb, மாறிலி எழுத்து *name)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	மாறிலி எழுத்து *rel = relative_path(name, prefix_len ? prefix : வெற்று, &buf);

	எனில் (வரி_terminator)
		quote_c_style(rel, sb, வெற்று, 0);
	இல்லையெனில்
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

நிலையான மாறிலி எழுத்து *get_tag(மாறிலி கட்டமைப்பு cache_entry *ce, மாறிலி எழுத்து *tag)
{
	நிலையான எழுத்து alttag[4];

	எனில் (tag && *tag && ((show_valid_bit && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_bit && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		memcpy(alttag, tag, 3);

		எனில் (isalpha(tag[0])) {
			alttag[0] = tolower(tag[0]);
		} இல்லையெனில் எனில் (tag[0] == '?') {
			alttag[0] = '!';
		} இல்லையெனில் {
			alttag[0] = 'v';
			alttag[1] = tag[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		tag = alttag;
	}

	திரும்பு tag;
}

நிலையான வெறுமை print_debug(மாறிலி கட்டமைப்பு cache_entry *ce)
{
	எனில் (debug_mode) {
		மாறிலி கட்டமைப்பு stat_data *sd = &ce->ce_stat_data;

		printf("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		printf("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		printf("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		printf("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		printf("  size: %u\tflags: %x\n", sd->sd_size, ce->ce_flags);
	}
}

நிலையான வெறுமை show_dir_entry(கட்டமைப்பு index_state *istate,
			   மாறிலி எழுத்து *tag, கட்டமைப்பு dir_entry *ent)
{
	முழு len = max_prefix_len;

	எனில் (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	எனில் (ps_matched)
		dir_path_match(istate, ent, &pathspec, len, ps_matched);

	fputs(tag, stdout);
	write_eolinfo(istate, வெற்று, ent->name);
	write_name(ent->name);
}

நிலையான வெறுமை show_other_files(கட்டமைப்பு index_state *istate,
			     மாறிலி கட்டமைப்பு dir_struct *dir)
{
	முழு i;

	க்கு (i = 0; i < dir->nr; i++) {
		கட்டமைப்பு dir_entry *ent = dir->entries[i];
		எனில் (!index_name_is_other(istate, ent->name, ent->len))
			தொடர்;
		show_dir_entry(istate, tag_other, ent);
	}
}

நிலையான வெறுமை show_killed_files(கட்டமைப்பு index_state *istate,
			      மாறிலி கட்டமைப்பு dir_struct *dir)
{
	முழு i;
	க்கு (i = 0; i < dir->nr; i++) {
		கட்டமைப்பு dir_entry *ent = dir->entries[i];
		எழுத்து *cp, *sp;
		முழு pos, len, killed = 0;

		க்கு (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			எனில் (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = index_name_pos(istate, ent->name, ent->len);
				எனில் (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				போது (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				எனில் (istate->cache_nr <= pos)
					நிறுத்து;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				எனில் ((ent->len < len) &&
				    !strncmp(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				நிறுத்து;
			}
			எனில் (0 <= index_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				நிறுத்து;
			}
		}
		எனில் (killed)
			show_dir_entry(istate, tag_killed, dir->entries[i]);
	}
}

நிலையான வெறுமை show_files(கட்டமைப்பு repository *repo, கட்டமைப்பு dir_struct *dir);

நிலையான வெறுமை show_submodule(கட்டமைப்பு repository *superproject,
			   கட்டமைப்பு dir_struct *dir, மாறிலி எழுத்து *path)
{
	கட்டமைப்பு repository subrepo;

	எனில் (repo_submodule_init(&subrepo, superproject, path,
				null_oid(superproject->hash_algo)))
		திரும்பு;

	எனில் (repo_read_index(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

நிலையான வெறுமை expand_objectsize(கட்டமைப்பு repository *repo, கட்டமைப்பு strbuf *வரி,
			      மாறிலி கட்டமைப்பு object_id *oid,
			      மாறிலி எண்ணிக்கை object_type type, unsigned முழு padded)
{
	நிலையான மாறிலி எழுத்து padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = வரி->len;
	size_t len;

	எனில் (type == OBJ_BLOB) {
		size_t size;
		எனில் (odb_read_object_info(repo->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(வரி, size);
	} இல்லையெனில் {
		strbuf_addstr(வரி, "-");
	}
	len = வரி->len - orig_len;
	எனில் (len < min_len)
		strbuf_செருகு(வரி, orig_len, padding, min_len - len);
}

நிலையான வெறுமை show_ce_fmt(கட்டமைப்பு repository *repo, மாறிலி கட்டமைப்பு cache_entry *ce,
			மாறிலி எழுத்து *format, மாறிலி எழுத்து *fullname) {
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	போது (strbuf_expand_step(&sb, &format)) {
		size_t len;
		கட்டமைப்பு stat st;

		எனில் (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		இல்லையெனில் எனில் ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		இல்லையெனில் எனில் (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_mode);
		இல்லையெனில் எனில் (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, &ce->oid, abbrev);
		இல்லையெனில் எனில் (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_mode)));
		இல்லையெனில் எனில் (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 1);
		இல்லையெனில் எனில் (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 0);
		இல்லையெனில் எனில் (skip_prefix(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		இல்லையெனில் எனில் (skip_prefix(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_mode) ?
				      get_cached_convert_stats_ascii(repo->index,
				      ce->name) : "");
		இல்லையெனில் எனில் (skip_prefix(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_mode) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		இல்லையெனில் எனில் (skip_prefix(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->index,
								 fullname));
		இல்லையெனில் எனில் (skip_prefix(format, "(path)", &format))
			write_name_to_buf(&sb, fullname);
		இல்லையெனில்
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, வரி_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

நிலையான வெறுமை show_ce(கட்டமைப்பு repository *repo, கட்டமைப்பு dir_struct *dir,
		    மாறிலி கட்டமைப்பு cache_entry *ce, மாறிலி எழுத்து *fullname,
		    மாறிலி எழுத்து *tag)
{
	எனில் (max_prefix_len > strlen(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	எனில் (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
	    is_submodule_active(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} இல்லையெனில் எனில் (match_pathspec(repo->index, &pathspec, fullname, strlen(fullname),
				  max_prefix_len, ps_matched,
				  S_ISDIR(ce->ce_mode) ||
				  S_ISGITLINK(ce->ce_mode))) {
		எனில் (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			திரும்பு;
		}

		tag = get_tag(ce, tag);

		எனில் (!show_stage) {
			fputs(tag, stdout);
		} இல்லையெனில் {
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

நிலையான வெறுமை show_ru_info(கட்டமைப்பு repository *repo, கட்டமைப்பு index_state *istate)
{
	கட்டமைப்பு string_list_item *item;

	எனில் (!istate->resolve_undo)
		திரும்பு;

	for_each_string_list_item(item, istate->resolve_undo) {
		மாறிலி எழுத்து *path = item->string;
		கட்டமைப்பு resolve_undo_info *ui = item->util;
		முழு i, len;

		len = strlen(path);
		எனில் (len < max_prefix_len)
			தொடர்; /* outside of the prefix */
		எனில் (!match_pathspec(istate, &pathspec, path, len,
				    max_prefix_len, ps_matched, 0))
			தொடர்; /* uninterested */
		க்கு (i = 0; i < 3; i++) {
			எனில் (!ui->mode[i])
				தொடர்;
			printf("%s%06o %s %d\t", tag_resolve_undo, ui->mode[i],
			       repo_find_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(path);
		}
	}
}

நிலையான முழு ce_excluded(கட்டமைப்பு dir_struct *dir, கட்டமைப்பு index_state *istate,
		       மாறிலி எழுத்து *fullname, மாறிலி கட்டமைப்பு cache_entry *ce)
{
	முழு dtype = ce_to_dtype(ce);
	திரும்பு is_excluded(dir, istate, fullname, &dtype);
}

நிலையான வெறுமை construct_fullname(கட்டமைப்பு strbuf *out, மாறிலி கட்டமைப்பு repository *repo,
			       மாறிலி கட்டமைப்பு cache_entry *ce)
{
	strbuf_reset(out);
	எனில் (repo->submodule_prefix)
		strbuf_addstr(out, repo->submodule_prefix);
	strbuf_addstr(out, ce->name);
}

நிலையான வெறுமை show_files(கட்டமைப்பு repository *repo, கட்டமைப்பு dir_struct *dir)
{
	முழு i;
	கட்டமைப்பு strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	எனில் (show_others || show_killed) {
		எனில் (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->index, &pathspec);
		எனில் (show_others)
			show_other_files(repo->index, dir);
		எனில் (show_killed)
			show_killed_files(repo->index, dir);
	}

	எனில் (!(show_cached || show_stage || show_deleted || show_modified))
		திரும்பு;

	க்கு (i = 0; i < repo->index->cache_nr; i++) {
		மாறிலி கட்டமைப்பு cache_entry *ce = repo->index->cache[i];
		கட்டமைப்பு stat st;
		முழு stat_err;

		எனில் (S_ISSPARSEDIR(ce->ce_mode) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_full_index(repo->index);
			ce = repo->index->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		எனில் ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->index, fullname.buf, ce))
			தொடர்;
		எனில் (ce->ce_flags & CE_UPDATE)
			தொடர்;
		எனில் ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? tag_unmerged :
				(ce_skip_worktree(ce) ? tag_skip_worktree :
				 tag_cached));
			எனில் (skipping_duplicates)
				செல் skip_to_next_name;
		}

		எனில் (!(show_deleted || show_modified))
			தொடர்;
		எனில் (ce_skip_worktree(ce))
			தொடர்;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		எனில் (pathspec.nr == 1 &&
		    !match_pathspec(repo->index, &pathspec, fullname.buf,
				    fullname.len, max_prefix_len, வெற்று,
				    S_ISDIR(ce->ce_mode) ||
				    S_ISGITLINK(ce->ce_mode)))
			தொடர்;
		stat_err = lstat(fullname.buf, &st);
		எனில் (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		எனில் (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, tag_removed);
			எனில் (skipping_duplicates)
				செல் skip_to_next_name;
		}
		எனில் (show_modified &&
		    (stat_err || ie_modified(repo->index, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, tag_modified);
			எனில் (skipping_duplicates)
				செல் skip_to_next_name;
		}
		தொடர்;

skip_to_next_name:
		{
			முழு j;
			கட்டமைப்பு cache_entry **cache = repo->index->cache;
			க்கு (j = i + 1; j < repo->index->cache_nr; j++)
				எனில் (strcmp(ce->name, cache[j]->name))
					நிறுத்து;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
நிலையான வெறுமை prune_index(கட்டமைப்பு index_state *istate,
			மாறிலி எழுத்து *prefix, size_t prefixlen)
{
	முழு pos;
	unsigned முழு first, last;

	எனில் (!prefix || !istate->cache_nr)
		திரும்பு;
	pos = index_name_pos(istate, prefix, prefixlen);
	எனில் (pos < 0)
		pos = -pos-1;
	first = pos;
	last = istate->cache_nr;
	போது (last > first) {
		முழு next = first + ((last - first) >> 1);
		மாறிலி கட்டமைப்பு cache_entry *ce = istate->cache[next];
		எனில் (!strncmp(ce->name, prefix, prefixlen)) {
			first = next+1;
			தொடர்;
		}
		last = next;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, last - pos);
	istate->cache_nr = last - pos;
}

நிலையான முழு get_common_prefix_len(மாறிலி எழுத்து *common_prefix)
{
	முழு common_prefix_len;

	எனில் (!common_prefix)
		திரும்பு 0;

	common_prefix_len = strlen(common_prefix);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	எனில் (common_prefix[common_prefix_len - 1] == '/')
		common_prefix_len--;

	திரும்பு common_prefix_len;
}

நிலையான மாறிலி எழுத்து * மாறிலி ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	வெற்று
};

நிலையான முழு option_parse_exclude(மாறிலி கட்டமைப்பு option *opt,
				மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு string_list *exclude_list = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	திரும்பு 0;
}

நிலையான முழு option_parse_exclude_from(மாறிலி கட்டமைப்பு option *opt,
				     மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு dir_struct *dir = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	add_patterns_from_file(dir, arg);

	திரும்பு 0;
}

நிலையான முழு option_parse_exclude_standard(மாறிலி கட்டமைப்பு option *opt,
					 மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு dir_struct *dir = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	setup_standard_excludes(dir);

	திரும்பு 0;
}

முழு cmd_ls_files(முழு argc,
		 மாறிலி எழுத்து **argv,
		 மாறிலி எழுத்து *cmd_prefix,
		 கட்டமைப்பு repository *repo)
{
	முழு require_work_மரம் = 0, show_tag = 0, i;
	எழுத்து *max_prefix;
	கட்டமைப்பு dir_struct dir = DIR_INIT;
	கட்டமைப்பு pattern_list *pl;
	கட்டமைப்பு string_list exclude_list = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_SET_INT('z', வெற்று, &வரி_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', வெற்று, &show_tag,
			N_("identify the file status with tags")),
		OPT_BOOL('v', வெற்று, &show_valid_bit,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', வெற்று, &show_fsmonitor_bit,
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
		OPT_CALLBACK_F(0, "exclude-standard", &dir, வெற்று,
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
		OPT_STRING(0, "with-tree", &with_மரம், N_("tree-ish"),
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
	முழு ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	prefix = cmd_prefix;
	எனில் (prefix)
		prefix_len = strlen(prefix);
	repo_config(repo, git_default_config, வெற்று);

	எனில் (repo_read_index(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, prefix, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	க்கு (i = 0; i < exclude_list.nr; i++) {
		add_pattern(exclude_list.பொருட்கள்[i].string, "", 0, pl, --exclude_args);
	}

	எனில் (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_tag))
			usage_செய்தி_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	எனில் (show_tag || show_valid_bit || show_fsmonitor_bit) {
		tag_cached = "H ";
		tag_unmerged = "M ";
		tag_removed = "R ";
		tag_modified = "C ";
		tag_other = "? ";
		tag_killed = "K ";
		tag_skip_worktree = "S ";
		tag_resolve_undo = "U ";
	}
	எனில் (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_மரம் = 1;
	எனில் (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	எனில் (show_tag || show_stage)
		skipping_duplicates = 0;
	எனில் (dir.exclude_per_dir)
		exc_given = 1;

	எனில் (require_work_மரம் && !is_inside_work_மரம்(repo))
		setup_work_மரம்(repo);

	எனில் (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_மரம்))
		die("ls-files --recurse-submodules unsupported mode");

	எனில் (recurse_submodules && error_unmatch)
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
	எனில் (recurse_submodules)
		max_prefix = வெற்று;
	இல்லையெனில்
		max_prefix = common_prefix(&pathspec);
	max_prefix_len = get_common_prefix_len(max_prefix);

	prune_index(repo->index, max_prefix, max_prefix_len);

	/* Treat unmatching pathspec elements as errors */
	எனில் (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	எனில் ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	எனில் ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	எனில் (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	எனில் (with_மரம்) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		எனில் (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_மரம்_on_index(repo->index, with_மரம், max_prefix);
	}

	show_files(repo, &dir);

	எனில் (show_resolve_undo)
		show_ru_info(repo, repo->index);

	எனில் (ps_matched && report_path_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	விடுவி(max_prefix);
	திரும்பு ret;
}
