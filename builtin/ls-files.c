/*
 * This merges the file listing in the directory cache index
 * with the actual working directory list, and shows different
 * combinations of the two.
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "convert.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "resolve-undo.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "pathspec.h"
#అన్తర్భావ "read-cache.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "sparse-index.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "hex.h"


స్థిర పూర్ణాంకం abbrev;
స్థిర పూర్ణాంకం show_deleted;
స్థిర పూర్ణాంకం show_cached;
స్థిర పూర్ణాంకం show_others;
స్థిర పూర్ణాంకం show_stage;
స్థిర పూర్ణాంకం show_unmerged;
స్థిర పూర్ణాంకం show_resolve_undo;
స్థిర పూర్ణాంకం show_modified;
స్థిర పూర్ణాంకం show_killed;
స్థిర పూర్ణాంకం show_valid_బిట్టు;
స్థిర పూర్ణాంకం show_fsmonitor_బిట్టు;
స్థిర పూర్ణాంకం పంక్తి_terminator = '\n';
స్థిర పూర్ణాంకం debug_బహుళకం;
స్థిర పూర్ణాంకం show_eol;
స్థిర పూర్ణాంకం recurse_submodules;
స్థిర పూర్ణాంకం skipping_duplicates;
స్థిర పూర్ణాంకం show_sparse_dirs;

స్థిర స్థిరరాశి అక్షరం *ఉపసర్గ;
స్థిర పూర్ణాంకం max_ఉపసర్గ_len;
స్థిర పూర్ణాంకం ఉపసర్గ_len;
స్థిర నిర్మాణం pathspec pathspec;
స్థిర పూర్ణాంకం error_unmatch;
స్థిర అక్షరం *ps_matched;
స్థిర స్థిరరాశి అక్షరం *with_వృక్షం;
స్థిర పూర్ణాంకం exc_given;
స్థిర పూర్ణాంకం exclude_వాదనలు;
స్థిర స్థిరరాశి అక్షరం *format;

స్థిర స్థిరరాశి అక్షరం *ట్యాగ్_cached = "";
స్థిర స్థిరరాశి అక్షరం *ట్యాగ్_unmerged = "";
స్థిర స్థిరరాశి అక్షరం *ట్యాగ్_removed = "";
స్థిర స్థిరరాశి అక్షరం *ట్యాగ్_other = "";
స్థిర స్థిరరాశి అక్షరం *ట్యాగ్_killed = "";
స్థిర స్థిరరాశి అక్షరం *ట్యాగ్_modified = "";
స్థిర స్థిరరాశి అక్షరం *ట్యాగ్_skip_worktree = "";
స్థిర స్థిరరాశి అక్షరం *ట్యాగ్_resolve_undo = "";

స్థిర శూన్య write_eolinfo(నిర్మాణం సూచీ_state *istate,
			  స్థిరరాశి నిర్మాణం cache_నమోదు *ce, స్థిరరాశి అక్షరం *మార్గం)
{
	ఒకవేళ (show_eol) {
		నిర్మాణం stat st;
		స్థిరరాశి అక్షరం *i_txt = "";
		స్థిరరాశి అక్షరం *w_txt = "";
		స్థిరరాశి అక్షరం *a_txt = get_convert_attr_ascii(istate, మార్గం);
		ఒకవేళ (ce && S_ISREG(ce->ce_బహుళకం))
			i_txt = get_cached_convert_stats_ascii(istate,
							       ce->name);
		ఒకవేళ (!lstat(మార్గం, &st) && S_ISREG(st.st_బహుళకం))
			w_txt = get_wt_convert_stats_ascii(మార్గం);
		రాయి("i/%-5s w/%-5s attr/%-17s\t", i_txt, w_txt, a_txt);
	}
}

స్థిర శూన్య write_name(స్థిరరాశి అక్షరం *name)
{
	/*
	 * With "--full-name", prefix_len=0; this caller needs to pass
	 * an empty string in that case (a NULL is good for "").
	 */
	write_name_quoted_relative(name, ఉపసర్గ_len ? ఉపసర్గ : ఖాళీ,
				   stdout, పంక్తి_terminator);
}

స్థిర శూన్య write_name_గమ్యం_buf(నిర్మాణం strbuf *sb, స్థిరరాశి అక్షరం *name)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	స్థిరరాశి అక్షరం *rel = relative_మార్గం(name, ఉపసర్గ_len ? ఉపసర్గ : ఖాళీ, &buf);

	ఒకవేళ (పంక్తి_terminator)
		quote_c_style(rel, sb, ఖాళీ, 0);
	లేకపోతే
		strbuf_addstr(sb, rel);

	strbuf_release(&buf);
}

స్థిర స్థిరరాశి అక్షరం *get_ట్యాగ్(స్థిరరాశి నిర్మాణం cache_నమోదు *ce, స్థిరరాశి అక్షరం *ట్యాగ్)
{
	స్థిర అక్షరం alttag[4];

	ఒకవేళ (ట్యాగ్ && *ట్యాగ్ && ((show_valid_బిట్టు && (ce->ce_flags & CE_VALID)) ||
		(show_fsmonitor_బిట్టు && (ce->ce_flags & CE_FSMONITOR_VALID)))) {
		జ్ఞాపకం_నకలు(alttag, ట్యాగ్, 3);

		ఒకవేళ (అక్షరమా(ట్యాగ్[0])) {
			alttag[0] = tolower(ట్యాగ్[0]);
		} లేకపోతే ఒకవేళ (ట్యాగ్[0] == '?') {
			alttag[0] = '!';
		} లేకపోతే {
			alttag[0] = 'v';
			alttag[1] = ట్యాగ్[0];
			alttag[2] = ' ';
			alttag[3] = 0;
		}

		ట్యాగ్ = alttag;
	}

	తిరిగి ట్యాగ్;
}

స్థిర శూన్య print_debug(స్థిరరాశి నిర్మాణం cache_నమోదు *ce)
{
	ఒకవేళ (debug_బహుళకం) {
		స్థిరరాశి నిర్మాణం stat_దత్తాంశం *sd = &ce->ce_stat_దత్తాంశం;

		రాయి("  ctime: %u:%u\n", sd->sd_ctime.sec, sd->sd_ctime.nsec);
		రాయి("  mtime: %u:%u\n", sd->sd_mtime.sec, sd->sd_mtime.nsec);
		రాయి("  dev: %u\tino: %u\n", sd->sd_dev, sd->sd_ino);
		రాయి("  uid: %u\tgid: %u\n", sd->sd_uid, sd->sd_gid);
		రాయి("  size: %u\tflags: %x\n", sd->sd_కొలత, ce->ce_flags);
	}
}

స్థిర శూన్య show_dir_నమోదు(నిర్మాణం సూచీ_state *istate,
			   స్థిరరాశి అక్షరం *ట్యాగ్, నిర్మాణం dir_నమోదు *ent)
{
	పూర్ణాంకం len = max_ఉపసర్గ_len;

	ఒకవేళ (len > ent->len)
		die("git ls-files: internal error - directory entry not superset of prefix");

	/* If ps_matches is non-NULL, figure out which pathspec(s) match. */
	ఒకవేళ (ps_matched)
		dir_మార్గం_సరిపోలిక(istate, ent, &pathspec, len, ps_matched);

	fputs(ట్యాగ్, stdout);
	write_eolinfo(istate, ఖాళీ, ent->name);
	write_name(ent->name);
}

స్థిర శూన్య show_other_files(నిర్మాణం సూచీ_state *istate,
			     స్థిరరాశి నిర్మాణం dir_struct *dir)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < dir->nr; i++) {
		నిర్మాణం dir_నమోదు *ent = dir->entries[i];
		ఒకవేళ (!సూచీ_name_is_other(istate, ent->name, ent->len))
			కొనసాగు;
		show_dir_నమోదు(istate, ట్యాగ్_other, ent);
	}
}

స్థిర శూన్య show_killed_files(నిర్మాణం సూచీ_state *istate,
			      స్థిరరాశి నిర్మాణం dir_struct *dir)
{
	పూర్ణాంకం i;
	కొరకు (i = 0; i < dir->nr; i++) {
		నిర్మాణం dir_నమోదు *ent = dir->entries[i];
		అక్షరం *cp, *sp;
		పూర్ణాంకం pos, len, killed = 0;

		కొరకు (cp = ent->name; cp - ent->name < ent->len; cp = sp + 1) {
			sp = strchr(cp, '/');
			ఒకవేళ (!sp) {
				/* If ent->name is prefix of an entry in the
				 * cache, it will be killed.
				 */
				pos = సూచీ_name_pos(istate, ent->name, ent->len);
				ఒకవేళ (0 <= pos)
					BUG("killed-file %.*s not found",
						ent->len, ent->name);
				pos = -pos - 1;
				అయితే (pos < istate->cache_nr &&
				       ce_stage(istate->cache[pos]))
					pos++; /* skip unmerged */
				ఒకవేళ (istate->cache_nr <= pos)
					ఆపు;
				/* pos points at a name immediately after
				 * ent->name in the cache.  Does it expect
				 * ent->name to be a directory?
				 */
				len = ce_namelen(istate->cache[pos]);
				ఒకవేళ ((ent->len < len) &&
				    !అక్షరశ్రేణి_పోలిక(istate->cache[pos]->name,
					     ent->name, ent->len) &&
				    istate->cache[pos]->name[ent->len] == '/')
					killed = 1;
				ఆపు;
			}
			ఒకవేళ (0 <= సూచీ_name_pos(istate, ent->name, sp - ent->name)) {
				/* If any of the leading directories in
				 * ent->name is registered in the cache,
				 * ent->name will be killed.
				 */
				killed = 1;
				ఆపు;
			}
		}
		ఒకవేళ (killed)
			show_dir_నమోదు(istate, ట్యాగ్_killed, dir->entries[i]);
	}
}

స్థిర శూన్య show_files(నిర్మాణం repository *repo, నిర్మాణం dir_struct *dir);

స్థిర శూన్య show_submodule(నిర్మాణం repository *superproject,
			   నిర్మాణం dir_struct *dir, స్థిరరాశి అక్షరం *మార్గం)
{
	నిర్మాణం repository subrepo;

	ఒకవేళ (repo_submodule_init(&subrepo, superproject, మార్గం,
				null_oid(superproject->hash_algo)))
		తిరిగి;

	ఒకవేళ (repo_read_సూచీ(&subrepo) < 0)
		die("index file corrupt");

	show_files(&subrepo, dir);

	repo_clear(&subrepo);
}

స్థిర శూన్య expand_objectsize(నిర్మాణం repository *repo, నిర్మాణం strbuf *పంక్తి,
			      స్థిరరాశి నిర్మాణం object_id *oid,
			      స్థిరరాశి గణన object_type type, గుర్తులేని పూర్ణాంకం padded)
{
	స్థిర స్థిరరాశి అక్షరం padding[] = "       ";
	కొలత_t min_len = padded ? అక్షరశ్రేణి_పొడవు(padding) : 0;
	కొలత_t orig_len = పంక్తి->len;
	కొలత_t len;

	ఒకవేళ (type == OBJ_BLOB) {
		కొలత_t కొలత;
		ఒకవేళ (odb_read_object_info(repo->objects, oid, &కొలత) < 0)
			die(_("could not get object info about '%s'"),
			    oid_గమ్యం_hex(oid));
		strbuf_కూడు_uint(పంక్తి, కొలత);
	} లేకపోతే {
		strbuf_addstr(పంక్తి, "-");
	}
	len = పంక్తి->len - orig_len;
	ఒకవేళ (len < min_len)
		strbuf_చొప్పించు(పంక్తి, orig_len, padding, min_len - len);
}

స్థిర శూన్య show_ce_fmt(నిర్మాణం repository *repo, స్థిరరాశి నిర్మాణం cache_నమోదు *ce,
			స్థిరరాశి అక్షరం *format, స్థిరరాశి అక్షరం *fullname) {
	నిర్మాణం strbuf sb = STRBUF_INIT;

	అయితే (strbuf_expand_అడుగు(&sb, &format)) {
		కొలత_t len;
		నిర్మాణం stat st;

		ఒకవేళ (skip_ఉపసర్గ(format, "%", &format))
			strbuf_addch(&sb, '%');
		లేకపోతే ఒకవేళ ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", ce->ce_బహుళకం);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(objectname)", &format))
			strbuf_కూడు_unique_abbrev(&sb, &ce->oid, abbrev);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(object_type(ce->ce_బహుళకం)));
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(objectsize:padded)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_బహుళకం), 1);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(objectsize)", &format))
			expand_objectsize(repo, &sb, &ce->oid,
					  object_type(ce->ce_బహుళకం), 0);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(stage)", &format))
			strbuf_addf(&sb, "%d", ce_stage(ce));
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(eolinfo:index)", &format))
			strbuf_addstr(&sb, S_ISREG(ce->ce_బహుళకం) ?
				      get_cached_convert_stats_ascii(repo->సూచీ,
				      ce->name) : "");
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(eolinfo:worktree)", &format))
			strbuf_addstr(&sb, !lstat(fullname, &st) &&
				      S_ISREG(st.st_బహుళకం) ?
				      get_wt_convert_stats_ascii(fullname) : "");
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(eolattr)", &format))
			strbuf_addstr(&sb, get_convert_attr_ascii(repo->సూచీ,
								 fullname));
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(path)", &format))
			write_name_గమ్యం_buf(&sb, fullname);
		లేకపోతే
			strbuf_expand_bad_format(format, "ls-files");
	}
	strbuf_addch(&sb, పంక్తి_terminator);
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

స్థిర శూన్య show_ce(నిర్మాణం repository *repo, నిర్మాణం dir_struct *dir,
		    స్థిరరాశి నిర్మాణం cache_నమోదు *ce, స్థిరరాశి అక్షరం *fullname,
		    స్థిరరాశి అక్షరం *ట్యాగ్)
{
	ఒకవేళ (max_ఉపసర్గ_len > అక్షరశ్రేణి_పొడవు(fullname))
		die("git ls-files: internal error - cache entry not superset of prefix");

	ఒకవేళ (recurse_submodules && S_ISGITLINK(ce->ce_బహుళకం) &&
	    is_submodule_చురుకైన(repo, ce->name)) {
		show_submodule(repo, dir, ce->name);
	} లేకపోతే ఒకవేళ (సరిపోలిక_pathspec(repo->సూచీ, &pathspec, fullname, అక్షరశ్రేణి_పొడవు(fullname),
				  max_ఉపసర్గ_len, ps_matched,
				  S_ISDIR(ce->ce_బహుళకం) ||
				  S_ISGITLINK(ce->ce_బహుళకం))) {
		ఒకవేళ (format) {
			show_ce_fmt(repo, ce, format, fullname);
			print_debug(ce);
			తిరిగి;
		}

		ట్యాగ్ = get_ట్యాగ్(ce, ట్యాగ్);

		ఒకవేళ (!show_stage) {
			fputs(ట్యాగ్, stdout);
		} లేకపోతే {
			రాయి("%s%06o %s %d\t",
			       ట్యాగ్,
			       ce->ce_బహుళకం,
			       repo_కనుగొను_unique_abbrev(repo, &ce->oid, abbrev),
			       ce_stage(ce));
		}
		write_eolinfo(repo->సూచీ, ce, fullname);
		write_name(fullname);
		print_debug(ce);
	}
}

స్థిర శూన్య show_ru_info(నిర్మాణం repository *repo, నిర్మాణం సూచీ_state *istate)
{
	నిర్మాణం string_list_అంశం *అంశం;

	ఒకవేళ (!istate->resolve_undo)
		తిరిగి;

	for_each_string_list_అంశం(అంశం, istate->resolve_undo) {
		స్థిరరాశి అక్షరం *మార్గం = అంశం->string;
		నిర్మాణం resolve_undo_info *ui = అంశం->util;
		పూర్ణాంకం i, len;

		len = అక్షరశ్రేణి_పొడవు(మార్గం);
		ఒకవేళ (len < max_ఉపసర్గ_len)
			కొనసాగు; /* outside of the prefix */
		ఒకవేళ (!సరిపోలిక_pathspec(istate, &pathspec, మార్గం, len,
				    max_ఉపసర్గ_len, ps_matched, 0))
			కొనసాగు; /* uninterested */
		కొరకు (i = 0; i < 3; i++) {
			ఒకవేళ (!ui->బహుళకం[i])
				కొనసాగు;
			రాయి("%s%06o %s %d\t", ట్యాగ్_resolve_undo, ui->బహుళకం[i],
			       repo_కనుగొను_unique_abbrev(repo, &ui->oid[i], abbrev),
			       i + 1);
			write_name(మార్గం);
		}
	}
}

స్థిర పూర్ణాంకం ce_excluded(నిర్మాణం dir_struct *dir, నిర్మాణం సూచీ_state *istate,
		       స్థిరరాశి అక్షరం *fullname, స్థిరరాశి నిర్మాణం cache_నమోదు *ce)
{
	పూర్ణాంకం dtype = ce_గమ్యం_dtype(ce);
	తిరిగి is_excluded(dir, istate, fullname, &dtype);
}

స్థిర శూన్య construct_fullname(నిర్మాణం strbuf *బహిర్గతం, స్థిరరాశి నిర్మాణం repository *repo,
			       స్థిరరాశి నిర్మాణం cache_నమోదు *ce)
{
	strbuf_reset(బహిర్గతం);
	ఒకవేళ (repo->submodule_ఉపసర్గ)
		strbuf_addstr(బహిర్గతం, repo->submodule_ఉపసర్గ);
	strbuf_addstr(బహిర్గతం, ce->name);
}

స్థిర శూన్య show_files(నిర్మాణం repository *repo, నిర్మాణం dir_struct *dir)
{
	పూర్ణాంకం i;
	నిర్మాణం strbuf fullname = STRBUF_INIT;

	/* For cached/deleted files we don't need to even do the readdir */
	ఒకవేళ (show_others || show_killed) {
		ఒకవేళ (!show_others)
			dir->flags |= DIR_COLLECT_KILLED_ONLY;
		fill_directory(dir, repo->సూచీ, &pathspec);
		ఒకవేళ (show_others)
			show_other_files(repo->సూచీ, dir);
		ఒకవేళ (show_killed)
			show_killed_files(repo->సూచీ, dir);
	}

	ఒకవేళ (!(show_cached || show_stage || show_deleted || show_modified))
		తిరిగి;

	కొరకు (i = 0; i < repo->సూచీ->cache_nr; i++) {
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = repo->సూచీ->cache[i];
		నిర్మాణం stat st;
		పూర్ణాంకం stat_err;

		ఒకవేళ (S_ISSPARSEDIR(ce->ce_బహుళకం) && !show_sparse_dirs) {
			/*
			 * This is the first time we've hit a sparse dir,
			 * so expansion will leave the first 'i' entries
			 * alone.
			 */
			ensure_నిండిన_సూచీ(repo->సూచీ);
			ce = repo->సూచీ->cache[i];
		}

		construct_fullname(&fullname, repo, ce);

		ఒకవేళ ((dir->flags & DIR_SHOW_IGNORED) &&
			!ce_excluded(dir, repo->సూచీ, fullname.buf, ce))
			కొనసాగు;
		ఒకవేళ (ce->ce_flags & CE_UPDATE)
			కొనసాగు;
		ఒకవేళ ((show_cached || show_stage) &&
		    (!show_unmerged || ce_stage(ce))) {
			show_ce(repo, dir, ce, fullname.buf,
				ce_stage(ce) ? ట్యాగ్_unmerged :
				(ce_skip_worktree(ce) ? ట్యాగ్_skip_worktree :
				 ట్యాగ్_cached));
			ఒకవేళ (skipping_duplicates)
				వెళ్ళు skip_గమ్యం_తదుపరి_name;
		}

		ఒకవేళ (!(show_deleted || show_modified))
			కొనసాగు;
		ఒకవేళ (ce_skip_worktree(ce))
			కొనసాగు;
		/*
		 * match_pathspec() is linear in pathspec.nr, so prefilter only
		 * the single-pathspec case. Only entries shown by show_ce()
		 * satisfy --error-unmatch.
		 */
		ఒకవేళ (pathspec.nr == 1 &&
		    !సరిపోలిక_pathspec(repo->సూచీ, &pathspec, fullname.buf,
				    fullname.len, max_ఉపసర్గ_len, ఖాళీ,
				    S_ISDIR(ce->ce_బహుళకం) ||
				    S_ISGITLINK(ce->ce_బహుళకం)))
			కొనసాగు;
		stat_err = lstat(fullname.buf, &st);
		ఒకవేళ (stat_err && (errno != ENOENT && errno != ENOTDIR))
			error_errno("cannot lstat '%s'", fullname.buf);
		ఒకవేళ (stat_err && show_deleted) {
			show_ce(repo, dir, ce, fullname.buf, ట్యాగ్_removed);
			ఒకవేళ (skipping_duplicates)
				వెళ్ళు skip_గమ్యం_తదుపరి_name;
		}
		ఒకవేళ (show_modified &&
		    (stat_err || ie_modified(repo->సూచీ, ce, &st, 0))) {
			show_ce(repo, dir, ce, fullname.buf, ట్యాగ్_modified);
			ఒకవేళ (skipping_duplicates)
				వెళ్ళు skip_గమ్యం_తదుపరి_name;
		}
		కొనసాగు;

skip_గమ్యం_తదుపరి_name:
		{
			పూర్ణాంకం j;
			నిర్మాణం cache_నమోదు **cache = repo->సూచీ->cache;
			కొరకు (j = i + 1; j < repo->సూచీ->cache_nr; j++)
				ఒకవేళ (strcmp(ce->name, cache[j]->name))
					ఆపు;
			i = j - 1; /* compensate for the for loop */
		}
	}

	strbuf_release(&fullname);
}

/*
 * Prune the index to only contain stuff starting with "prefix"
 */
స్థిర శూన్య prune_సూచీ(నిర్మాణం సూచీ_state *istate,
			స్థిరరాశి అక్షరం *ఉపసర్గ, కొలత_t prefixlen)
{
	పూర్ణాంకం pos;
	గుర్తులేని పూర్ణాంకం మొదటి, చివరి;

	ఒకవేళ (!ఉపసర్గ || !istate->cache_nr)
		తిరిగి;
	pos = సూచీ_name_pos(istate, ఉపసర్గ, prefixlen);
	ఒకవేళ (pos < 0)
		pos = -pos-1;
	మొదటి = pos;
	చివరి = istate->cache_nr;
	అయితే (చివరి > మొదటి) {
		పూర్ణాంకం తదుపరి = మొదటి + ((చివరి - మొదటి) >> 1);
		స్థిరరాశి నిర్మాణం cache_నమోదు *ce = istate->cache[తదుపరి];
		ఒకవేళ (!అక్షరశ్రేణి_పోలిక(ce->name, ఉపసర్గ, prefixlen)) {
			మొదటి = తదుపరి+1;
			కొనసాగు;
		}
		చివరి = తదుపరి;
	}
	MOVE_ARRAY(istate->cache, istate->cache + pos, చివరి - pos);
	istate->cache_nr = చివరి - pos;
}

స్థిర పూర్ణాంకం get_common_ఉపసర్గ_len(స్థిరరాశి అక్షరం *common_ఉపసర్గ)
{
	పూర్ణాంకం common_ఉపసర్గ_len;

	ఒకవేళ (!common_ఉపసర్గ)
		తిరిగి 0;

	common_ఉపసర్గ_len = అక్షరశ్రేణి_పొడవు(common_ఉపసర్గ);

	/*
	 * If the prefix has a trailing slash, strip it so that submodules won't
	 * be pruned from the index.
	 */
	ఒకవేళ (common_ఉపసర్గ[common_ఉపసర్గ_len - 1] == '/')
		common_ఉపసర్గ_len--;

	తిరిగి common_ఉపసర్గ_len;
}

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి ls_files_usage[] = {
	N_("git ls-files [<options>] [<file>...]"),
	ఖాళీ
};

స్థిర పూర్ణాంకం option_parse_exclude(స్థిరరాశి నిర్మాణం option *opt,
				స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం string_list *exclude_list = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	string_list_append(exclude_list, arg);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_exclude_from(స్థిరరాశి నిర్మాణం option *opt,
				     స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం dir_struct *dir = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	exc_given = 1;
	కూడు_patterns_from_file(dir, arg);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_exclude_standard(స్థిరరాశి నిర్మాణం option *opt,
					 స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం dir_struct *dir = opt->విలువ;

	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);

	exc_given = 1;
	అమరిక_standard_excludes(dir);

	తిరిగి 0;
}

పూర్ణాంకం cmd_ls_files(పూర్ణాంకం argc,
		 స్థిరరాశి అక్షరం **argv,
		 స్థిరరాశి అక్షరం *cmd_ఉపసర్గ,
		 నిర్మాణం repository *repo)
{
	పూర్ణాంకం require_work_వృక్షం = 0, show_ట్యాగ్ = 0, i;
	అక్షరం *max_ఉపసర్గ;
	నిర్మాణం dir_struct dir = DIR_INIT;
	నిర్మాణం నమూనా_list *pl;
	నిర్మాణం string_list exclude_list = STRING_LIST_INIT_NODUP;
	నిర్మాణం option builtin_ls_files_options[] = {
		/* Think twice before adding "--nul" synonym to this */
		OPT_సమితి_INT('z', ఖాళీ, &పంక్తి_terminator,
			N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL('t', ఖాళీ, &show_ట్యాగ్,
			N_("identify the file status with tags")),
		OPT_BOOL('v', ఖాళీ, &show_valid_బిట్టు,
			N_("use lowercase letters for 'assume unchanged' files")),
		OPT_BOOL('f', ఖాళీ, &show_fsmonitor_బిట్టు,
			N_("use lowercase letters for 'fsmonitor clean' files")),
		OPT_BOOL('c', "cached", &show_cached,
			N_("show cached files in the output (default)")),
		OPT_BOOL('d', "deleted", &show_deleted,
			N_("show deleted files in the output")),
		OPT_BOOL('m', "modified", &show_modified,
			N_("show modified files in the output")),
		OPT_BOOL('o', "others", &show_others,
			N_("show other files in the output")),
		OPT_బిట్టు('i', "ignored", &dir.flags,
			N_("show ignored files in the output"),
			DIR_SHOW_IGNORED),
		OPT_BOOL('s', "stage", &show_stage,
			N_("show staged contents' object name in the output")),
		OPT_BOOL('k', "killed", &show_killed,
			N_("show files on the filesystem that need to be removed")),
		OPT_బిట్టు(0, "directory", &dir.flags,
			N_("show 'other' directories' names only"),
			DIR_SHOW_OTHER_DIRECTORIES),
		OPT_BOOL(0, "eol", &show_eol, N_("show line endings of files")),
		OPT_NEGBIT(0, "empty-directory", &dir.flags,
			N_("don't show empty directories"),
			DIR_HIDE_ఖాళీలు_DIRECTORIES),
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
		OPT_STRING(0, "exclude-per-directory", &dir.exclude_చొప్పున_dir, N_("file"),
			N_("read additional per-directory exclude patterns in <file>")),
		OPT_CALLBACK_F(0, "exclude-standard", &dir, ఖాళీ,
			N_("add the standard git exclusions"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			option_parse_exclude_standard),
		OPT_సమితి_INT_F(0, "full-name", &ఉపసర్గ_len,
			      N_("make the output relative to the project top directory"),
			      0, PARSE_OPT_NONEG),
		OPT_BOOL(0, "recurse-submodules", &recurse_submodules,
			N_("recurse through submodules")),
		OPT_BOOL(0, "error-unmatch", &error_unmatch,
			N_("if any <file> is not in the index, treat this as an error")),
		OPT_STRING(0, "with-tree", &with_వృక్షం, N_("tree-ish"),
			N_("pretend that paths removed since <tree-ish> are still present")),
		OPT__ABBREV(&abbrev),
		OPT_BOOL(0, "debug", &debug_బహుళకం, N_("show debugging data")),
		OPT_BOOL(0, "deduplicate", &skipping_duplicates,
			 N_("suppress duplicate entries")),
		OPT_BOOL(0, "sparse", &show_sparse_dirs,
			 N_("show sparse directories in the presence of a sparse index")),
		OPT_STRING_F(0, "format", &format, N_("format"),
			     N_("format to use for the output"),
			     PARSE_OPT_NONEG),
		OPT_చివర()
	};
	పూర్ణాంకం ret = 0;

	show_usage_with_options_if_asked(argc, argv,
					 ls_files_usage, builtin_ls_files_options);

	prepare_repo_settings(repo);
	repo->settings.command_requires_నిండిన_సూచీ = 0;

	ఉపసర్గ = cmd_ఉపసర్గ;
	ఒకవేళ (ఉపసర్గ)
		ఉపసర్గ_len = అక్షరశ్రేణి_పొడవు(ఉపసర్గ);
	repo_config(repo, git_default_config, ఖాళీ);

	ఒకవేళ (repo_read_సూచీ(repo) < 0)
		die("index file corrupt");

	argc = parse_options(argc, argv, ఉపసర్గ, builtin_ls_files_options,
			ls_files_usage, 0);
	pl = కూడు_నమూనా_list(&dir, EXC_CMDL, "--exclude option");
	కొరకు (i = 0; i < exclude_list.nr; i++) {
		కూడు_నమూనా(exclude_list.వస్తువులు[i].string, "", 0, pl, --exclude_వాదనలు);
	}

	ఒకవేళ (format && (show_stage || show_others || show_killed ||
		show_resolve_undo || skipping_duplicates || show_eol || show_ట్యాగ్))
			usage_సందేశం_opt(_("--format cannot be used with -s, -o, -k, -t, "
				      "--resolve-undo, --deduplicate, --eol"),
				      ls_files_usage, builtin_ls_files_options);

	ఒకవేళ (show_ట్యాగ్ || show_valid_బిట్టు || show_fsmonitor_బిట్టు) {
		ట్యాగ్_cached = "H ";
		ట్యాగ్_unmerged = "M ";
		ట్యాగ్_removed = "R ";
		ట్యాగ్_modified = "C ";
		ట్యాగ్_other = "? ";
		ట్యాగ్_killed = "K ";
		ట్యాగ్_skip_worktree = "S ";
		ట్యాగ్_resolve_undo = "U ";
	}
	ఒకవేళ (show_modified || show_others || show_deleted || (dir.flags & DIR_SHOW_IGNORED) || show_killed)
		require_work_వృక్షం = 1;
	ఒకవేళ (show_unmerged)
		/*
		 * There's no point in showing unmerged unless
		 * you also show the stage information.
		 */
		show_stage = 1;
	ఒకవేళ (show_ట్యాగ్ || show_stage)
		skipping_duplicates = 0;
	ఒకవేళ (dir.exclude_చొప్పున_dir)
		exc_given = 1;

	ఒకవేళ (require_work_వృక్షం && !is_inside_work_వృక్షం(repo))
		అమరిక_work_వృక్షం(repo);

	ఒకవేళ (recurse_submodules &&
	    (show_deleted || show_others || show_unmerged ||
	     show_killed || show_modified || show_resolve_undo || with_వృక్షం))
		die("ls-files --recurse-submodules unsupported mode");

	ఒకవేళ (recurse_submodules && error_unmatch)
		die("ls-files --recurse-submodules does not support "
		    "--error-unmatch");

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       ఉపసర్గ, argv);

	/*
	 * Find common prefix for all pathspec's
	 * This is used as a performance optimization which unfortunately cannot
	 * be done when recursing into submodules because when a pathspec is
	 * given which spans repository boundaries you can't simply remove the
	 * submodule entry because the pathspec may match something inside the
	 * submodule.
	 */
	ఒకవేళ (recurse_submodules)
		max_ఉపసర్గ = ఖాళీ;
	లేకపోతే
		max_ఉపసర్గ = common_ఉపసర్గ(&pathspec);
	max_ఉపసర్గ_len = get_common_ఉపసర్గ_len(max_ఉపసర్గ);

	prune_సూచీ(repo->సూచీ, max_ఉపసర్గ, max_ఉపసర్గ_len);

	/* Treat unmatching pathspec elements as errors */
	ఒకవేళ (pathspec.nr && error_unmatch)
		ps_matched = xcalloc(pathspec.nr, 1);

	ఒకవేళ ((dir.flags & DIR_SHOW_IGNORED) && !show_others && !show_cached)
		die("ls-files -i must be used with either -o or -c");

	ఒకవేళ ((dir.flags & DIR_SHOW_IGNORED) && !exc_given)
		die("ls-files --ignored needs some exclude pattern");

	/* With no flags, we default to showing the cached files */
	ఒకవేళ (!(show_stage || show_deleted || show_others || show_unmerged ||
	      show_killed || show_modified || show_resolve_undo))
		show_cached = 1;

	ఒకవేళ (with_వృక్షం) {
		/*
		 * Basic sanity check; show-stages and show-unmerged
		 * would not make any sense with this option.
		 */
		ఒకవేళ (show_stage || show_unmerged)
			die(_("options '%s' and '%s' cannot be used together"), "ls-files --with-tree", "-s/-u");
		overlay_వృక్షం_on_సూచీ(repo->సూచీ, with_వృక్షం, max_ఉపసర్గ);
	}

	show_files(repo, &dir);

	ఒకవేళ (show_resolve_undo)
		show_ru_info(repo, repo->సూచీ);

	ఒకవేళ (ps_matched && report_మార్గం_error(ps_matched, &pathspec)) {
		fprintf(stderr, "Did you forget to 'git add'?\n");
		ret = 1;
	}

	string_list_clear(&exclude_list, 0);
	dir_clear(&dir);
	విడుదల(max_ఉపసర్గ);
	తిరిగి ret;
}
