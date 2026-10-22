/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "convert.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "resolve-undo.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "pathspec.h"
#ଅନ୍ତର୍ଭାଵ "read-cache.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "sparse-index.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"


ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା abbrev;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_deleted;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_cached;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_others;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_stage;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_unmerged;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_resolve_undo;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_modified;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_killed;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_valid_bit;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_fsmonitor_bit;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_terminator = '\n';
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା debug_mode;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_eol;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse_submodules;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା skipping_duplicates;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_sparse_dirs;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *prefix;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_prefix_len;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prefix_len;
ସ୍ଥିର ସଂରଚନା pathspec pathspec;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା error_unmatch;
ସ୍ଥିର ଅକ୍ଷର *ps_matched;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *with_ବୃକ୍ଷ;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା exc_given;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude_args;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *format;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *tag_cached = "";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *tag_unmerged = "";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *tag_removed = "";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *tag_other = "";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *tag_killed = "";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *tag_modified = "";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *tag_skip_worktree = "";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *tag_resolve_undo = "";

ସ୍ଥିର ଶୂନ୍ୟ write_eolinfo(ସଂରଚନା index_state *istate,
			  ଅଚଳ ସଂରଚନା cache_entry *ce, ଅଚଳ ଅକ୍ଷର *path)
{
	ଯଦି (show_eol) {
		ସଂରଚନା stat st;
		ଅଚଳ ଅକ୍ଷର *i_txt = "";
		ଅଚଳ ଅକ୍ଷର *w_txt = "";
		ଅଚଳ ଅକ୍ଷର *a_txt = get_convert_attr_ascii(istate, path);
		ଯଦି (ce && S_ISREG(ce->ce_mode))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		ଯଦି (!lstat(path, &st) && S_ISREG(st.st_mode))
			w_txt = get_wt_convert_stats_ascii(path);
		printf("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ write_name(ଅଚଳ ଅକ୍ଷର *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, prefix_len ? prefix : ଖାଲି,
				   stdout, ଧାଡ଼ି_terminator);
}

ସ୍ଥିର ଶୂନ୍ୟ write_name_to_buf(ସଂରଚନା strbuf *sb, ଅଚଳ ଅକ୍ଷର *name)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *rel = relative_path(name, prefix_len ? prefix : ଖାଲି, &buf);

	ଯଦି (ଧାଡ଼ି_terminator)
		quote_c_style(rel, sb, ଖାଲି, 0);
	ଅନ୍ୟଥା
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_tag(ଅଚଳ ସଂରଚନା cache_entry *ce, ଅଚଳ ଅକ୍ଷର *tag)
{
	ସ୍ଥିର ଅକ୍ଷର alttag[4];

	ଯଦି (tag && *tag && ((show_valid_bit && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_bit && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		memcpy(alttag, tag, 3);

		ଯଦି (isalpha(tag[0])) {
			alttag[0] = tolower(tag[0]);
		} ଅନ୍ୟଥା ଯଦି (tag[0] == '?') {
			alttag[0] = '!';
		} ଅନ୍ୟଥା {
			alttag[0] = 'v';
			alttag[1] = tag[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		tag = alttag;
	}

	ଫେରନ୍ତୁ tag;
}

ସ୍ଥିର ଶୂନ୍ୟ print_debug(ଅଚଳ ସଂରଚନା cache_entry *ce)
{
	ଯଦି (debug_mode) {
		ଅଚଳ ସଂରଚନା stat_data *sd = &ce->ce_stat_data;

		printf("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		printf("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		printf("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		printf("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		printf("  size: %u\tflags: %x\n", sd->sd_size, ce->ce_flags);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ show_dir_entry(ସଂରଚନା index_state *istate,
			   ଅଚଳ ଅକ୍ଷର *tag, ସଂରଚନା dir_entry *ent)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = max_prefix_len;

	ଯଦି (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	ଯଦି (ps_matched)
		dir_path_match(istate, ent, &pathspec, len, ps_matched);

	fputs(tag, stdout);
	write_eolinfo(istate, ଖାଲି, ent->name);
	write_name(ent->name);
}

ସ୍ଥିର ଶୂନ୍ୟ show_other_files(ସଂରଚନା index_state *istate,
			     ଅଚଳ ସଂରଚନା dir_struct *dir)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < dir->nr; i++) {
		ସଂରଚନା dir_entry *ent = dir->entries[i];
		ଯଦି (!index_name_is_other(istate, ent->name, ent->len))
			ଜାରି;
		show_dir_entry(istate, tag_other, ent);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ show_killed_files(ସଂରଚନା index_state *istate,
			      ଅଚଳ ସଂରଚନା dir_struct *dir)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପାଇଁ (i = 0; i < dir->nr; i++) {
		ସଂରଚନା dir_entry *ent = dir->entries[i];
		ଅକ୍ଷର *cp, *sp;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos, len, killed = 0;

		ପାଇଁ (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			ଯଦି (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = index_name_pos(istate, ent->name, ent->len);
				ଯଦି (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				ଯେତେବେଳେ (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				ଯଦି (istate->cache_nr <= pos)
					ଭାଙ୍ଗ;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				ଯଦି ((ent->len < len) &&
				    !strncmp(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				ଭାଙ୍ଗ;
			}
			ଯଦି (0 <= index_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				ଭାଙ୍ଗ;
			}
		}
		ଯଦି (killed)
			show_dir_entry(istate, tag_killed, dir->entries[i]);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ show_files(ସଂରଚନା repository *repo, ସଂରଚନା dir_struct *dir);

ସ୍ଥିର ଶୂନ୍ୟ show_submodule(ସଂରଚନା repository *superproject,
			   ସଂରଚନା dir_struct *dir, ଅଚଳ ଅକ୍ଷର *path)
{
	ସଂରଚନା repository subrepo;

	ଯଦି (repo_submodule_init(&subrepo, superproject, path,
				null_oid(superproject->hash_algo)))
		ଫେରନ୍ତୁ;

	ଯଦି (repo_read_index(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

ସ୍ଥିର ଶୂନ୍ୟ expand_objectsize(ସଂରଚନା repository *repo, ସଂରଚନା strbuf *ଧାଡ଼ି,
			      ଅଚଳ ସଂରଚନା object_id *oid,
			      ଅଚଳ ଗଣନା object_type type, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା padded)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର padding[] = "       ";
	size_t min_len = padded ? strlen(padding) : 0;
	size_t orig_len = ଧାଡ଼ି->len;
	size_t len;

	ଯଦି (type == OBJ_BLOB) {
		size_t size;
		ଯଦି (odb_read_object_info(repo->objects, oid, &size) < 0)
			die(_("could not get object info about '%s'"),
			    oid_to_hex(oid));
		strbuf_add_uint(ଧାଡ଼ି, size);
	} ଅନ୍ୟଥା {
		strbuf_addstr(ଧାଡ଼ି, "-");
	}
	len = ଧାଡ଼ି->len - orig_len;
	ଯଦି (len < min_len)
		strbuf_ଭର୍ତ୍ତି(ଧାଡ଼ି, orig_len, padding, min_len - len);
}

ସ୍ଥିର ଶୂନ୍ୟ show_ce_fmt(ସଂରଚନା repository *repo, ଅଚଳ ସଂରଚନା cache_entry *ce,
			ଅଚଳ ଅକ୍ଷର *format, ଅଚଳ ଅକ୍ଷର *fullname) {
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	ଯେତେବେଳେ (strbuf_expand_step(&sb, &format)) {
		size_t len;
		ସଂରଚନା stat st;

		ଯଦି (skip_prefix(format, "%", &format))
			strbuf_addch(&sb, '%');
		ଅନ୍ୟଥା ଯଦି ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_mode);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(objectname)", &format))
			strbuf_add_unique_abbrev(&sb, &ce->oid, abbrev);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_mode)));
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 1);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_mode), 0);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_mode) ?
				      get_cached_convert_stats_ascii(repo->index,
				      ce->name) : "");
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_mode) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->index,
								 fullname));
		ଅନ୍ୟଥା ଯଦି (skip_prefix(format, "(path)", &format))
			write_name_to_buf(&sb, fullname);
		ଅନ୍ୟଥା
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, ଧାଡ଼ି_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

ସ୍ଥିର ଶୂନ୍ୟ show_ce(ସଂରଚନା repository *repo, ସଂରଚନା dir_struct *dir,
		    ଅଚଳ ସଂରଚନା cache_entry *ce, ଅଚଳ ଅକ୍ଷର *fullname,
		    ଅଚଳ ଅକ୍ଷର *tag)
{
	ଯଦି (max_prefix_len > strlen(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	ଯଦି (recurse_submodules && S_ISGITLINK(ce->ce_mode) &&
	    is_submodule_active(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} ଅନ୍ୟଥା ଯଦି (match_pathspec(repo->index, &pathspec, fullname, strlen(fullname),
				  max_prefix_len, ps_matched,
				  S_ISDIR(ce->ce_mode) ||
				  S_ISGITLINK(ce->ce_mode))) {
		ଯଦି (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			ଫେରନ୍ତୁ;
		}

		tag = get_tag(ce, tag);

		ଯଦି (!show_stage) {
			fputs(tag, stdout);
		} ଅନ୍ୟଥା {
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

ସ୍ଥିର ଶୂନ୍ୟ show_ru_info(ସଂରଚନା repository *repo, ସଂରଚନା index_state *istate)
{
	ସଂରଚନା string_list_item *item;

	ଯଦି (!istate->resolve_undo)
		ଫେରନ୍ତୁ;

	for_each_string_list_item(item, istate->resolve_undo) {
		ଅଚଳ ଅକ୍ଷର *path = item->string;
		ସଂରଚନା resolve_undo_info *ui = item->util;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, len;

		len = strlen(path);
		ଯଦି (len < max_prefix_len)
			ଜାରି; /* outside of the prefix */
		ଯଦି (!match_pathspec(istate, &pathspec, path, len,
				    max_prefix_len, ps_matched, 0))
			ଜାରି; /* uninterested */
		ପାଇଁ (i = 0; i < 3; i++) {
			ଯଦି (!ui->mode[i])
				ଜାରି;
			printf("%s%06o %s %d\t", tag_resolve_undo, ui->mode[i],
			       repo_find_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(path);
		}
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ce_excluded(ସଂରଚନା dir_struct *dir, ସଂରଚନା index_state *istate,
		       ଅଚଳ ଅକ୍ଷର *fullname, ଅଚଳ ସଂରଚନା cache_entry *ce)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = ce_to_dtype(ce);
	ଫେରନ୍ତୁ is_excluded(dir, istate, fullname, &dtype);
}

ସ୍ଥିର ଶୂନ୍ୟ construct_fullname(ସଂରଚନା strbuf *out, ଅଚଳ ସଂରଚନା repository *repo,
			       ଅଚଳ ସଂରଚନା cache_entry *ce)
{
	strbuf_reset(out);
	ଯଦି (repo->submodule_prefix)
		strbuf_addstr(out, repo->submodule_prefix);
	strbuf_addstr(out, ce->name);
}

ସ୍ଥିର ଶୂନ୍ୟ show_files(ସଂରଚନା repository *repo, ସଂରଚନା dir_struct *dir)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	ଯଦି (show_others || show_killed) {
		ଯଦି (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->index, &pathspec);
		ଯଦି (show_others)
			show_other_files(repo->index, dir);
		ଯଦି (show_killed)
			show_killed_files(repo->index, dir);
	}

	ଯଦି (!(show_cached || show_stage || show_deleted || show_modified))
		ଫେରନ୍ତୁ;

	ପାଇଁ (i = 0; i < repo->index->cache_nr; i++) {
		ଅଚଳ ସଂରଚନା cache_entry *ce = repo->index->cache[i];
		ସଂରଚନା stat st;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା stat_err;

		ଯଦି (S_ISSPARSEDIR(ce->ce_mode) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_full_index(repo->index);
			ce = repo->index->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		ଯଦି ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->index, fullname.buf, ce))
			ଜାରି;
		ଯଦି (ce->ce_flags & CE_UPDATE)
			ଜାରି;
		ଯଦି ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? tag_unmerged :
				(ce_skip_worktree(ce) ? tag_skip_worktree :
				 tag_cached));
			ଯଦି (skipping_duplicates)
				ଯାଅ skip_to_next_name;
		}

		ଯଦି (!(show_deleted || show_modified))
			ଜାରି;
		ଯଦି (ce_skip_worktree(ce))
			ଜାରି;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		ଯଦି (pathspec.nr == 1 &&
		    !match_pathspec(repo->index, &pathspec, fullname.buf,
				    fullname.len, max_prefix_len, ଖାଲି,
				    S_ISDIR(ce->ce_mode) ||
				    S_ISGITLINK(ce->ce_mode)))
			ଜାରି;
		stat_err = lstat(fullname.buf, &st);
		ଯଦି (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		ଯଦି (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, tag_removed);
			ଯଦି (skipping_duplicates)
				ଯାଅ skip_to_next_name;
		}
		ଯଦି (show_modified &&
		    (stat_err || ie_modified(repo->index, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, tag_modified);
			ଯଦି (skipping_duplicates)
				ଯାଅ skip_to_next_name;
		}
		ଜାରି;

skip_to_next_name:
		{
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
			ସଂରଚନା cache_entry **cache = repo->index->cache;
			ପାଇଁ (j = i + 1; j < repo->index->cache_nr; j++)
				ଯଦି (strcmp(ce->name, cache[j]->name))
					ଭାଙ୍ଗ;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
ସ୍ଥିର ଶୂନ୍ୟ prune_index(ସଂରଚନା index_state *istate,
			ଅଚଳ ଅକ୍ଷର *prefix, size_t prefixlen)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା first, last;

	ଯଦି (!prefix || !istate->cache_nr)
		ଫେରନ୍ତୁ;
	pos = index_name_pos(istate, prefix, prefixlen);
	ଯଦି (pos < 0)
		pos = -pos-1;
	first = pos;
	last = istate->cache_nr;
	ଯେତେବେଳେ (last > first) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା next = first + ((last - first) >> 1);
		ଅଚଳ ସଂରଚନା cache_entry *ce = istate->cache[next];
		ଯଦି (!strncmp(ce->name, prefix, prefixlen)) {
			first = next+1;
			ଜାରି;
		}
		last = next;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, last - pos);
	istate->cache_nr = last - pos;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_common_prefix_len(ଅଚଳ ଅକ୍ଷର *common_prefix)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା common_prefix_len;

	ଯଦି (!common_prefix)
		ଫେରନ୍ତୁ 0;

	common_prefix_len = strlen(common_prefix);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	ଯଦି (common_prefix[common_prefix_len - 1] == '/')
		common_prefix_len--;

	ଫେରନ୍ତୁ common_prefix_len;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_exclude(ଅଚଳ ସଂରଚନା option *opt,
				ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା string_list *exclude_list = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_exclude_from(ଅଚଳ ସଂରଚନା option *opt,
				     ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା dir_struct *dir = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	add_patterns_from_file(dir, arg);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_exclude_standard(ଅଚଳ ସଂରଚନା option *opt,
					 ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା dir_struct *dir = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	setup_standard_excludes(dir);

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_ls_files(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		 ଅଚଳ ଅକ୍ଷର **argv,
		 ଅଚଳ ଅକ୍ଷର *cmd_prefix,
		 ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା require_work_ବୃକ୍ଷ = 0, show_tag = 0, i;
	ଅକ୍ଷର *max_prefix;
	ସଂରଚନା dir_struct dir = DIR_INIT;
	ସଂରଚନା pattern_list *pl;
	ସଂରଚନା string_list exclude_list = STRING_LIST_INIT_NODUP;
	ସଂରଚନା option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_SET_INT('z', ଖାଲି, &ଧାଡ଼ି_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', ଖାଲି, &show_tag,
			N_("identify the file status with tags")),
		OPT_BOOL('v', ଖାଲି, &show_valid_bit,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', ଖାଲି, &show_fsmonitor_bit,
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
		OPT_CALLBACK_F(0, "exclude-standard", &dir, ଖାଲି,
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
		OPT_STRING(0, "with-tree", &with_ବୃକ୍ଷ, N_("tree-ish"),
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
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	prefix = cmd_prefix;
	ଯଦି (prefix)
		prefix_len = strlen(prefix);
	repo_config(repo, git_default_config, ଖାଲି);

	ଯଦି (repo_read_index(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, prefix, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	ପାଇଁ (i = 0; i < exclude_list.nr; i++) {
		add_pattern(exclude_list.ବସ୍ତୁଗୁଡ଼ିକ[i].string, "", 0, pl, --exclude_args);
	}

	ଯଦି (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_tag))
			usage_ସନ୍ଦେଶ_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	ଯଦି (show_tag || show_valid_bit || show_fsmonitor_bit) {
		tag_cached = "H ";
		tag_unmerged = "M ";
		tag_removed = "R ";
		tag_modified = "C ";
		tag_other = "? ";
		tag_killed = "K ";
		tag_skip_worktree = "S ";
		tag_resolve_undo = "U ";
	}
	ଯଦି (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_ବୃକ୍ଷ = 1;
	ଯଦି (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	ଯଦି (show_tag || show_stage)
		skipping_duplicates = 0;
	ଯଦି (dir.exclude_per_dir)
		exc_given = 1;

	ଯଦି (require_work_ବୃକ୍ଷ && !is_inside_work_ବୃକ୍ଷ(repo))
		setup_work_ବୃକ୍ଷ(repo);

	ଯଦି (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_ବୃକ୍ଷ))
		die("ls-files --recurse-submodules unsupported mode");

	ଯଦି (recurse_submodules && error_unmatch)
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
	ଯଦି (recurse_submodules)
		max_prefix = ଖାଲି;
	ଅନ୍ୟଥା
		max_prefix = common_prefix(&pathspec);
	max_prefix_len = get_common_prefix_len(max_prefix);

	prune_index(repo->index, max_prefix, max_prefix_len);

	/* Treat unmatching pathspec elements as errors */
	ଯଦି (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	ଯଦି ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	ଯଦି ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	ଯଦି (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	ଯଦି (with_ବୃକ୍ଷ) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		ଯଦି (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_ବୃକ୍ଷ_on_index(repo->index, with_ବୃକ୍ଷ, max_prefix);
	}

	show_files(repo, &dir);

	ଯଦି (show_resolve_undo)
		show_ru_info(repo, repo->index);

	ଯଦି (ps_matched && report_path_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	ମୁକ୍ତ(max_prefix);
	ଫେରନ୍ତୁ ret;
}
