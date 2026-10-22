#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "tree-walk.h"
#അന്തര്ഭാവ "xdiff-interface.h"
#അന്തര്ഭാവ "help.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "merge-ort.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "blob.h"
#അന്തര്ഭാവ "merge-blobs.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "write-or-die.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ വരി_termination = '\n';

ഘടന merge_list {
	ഘടന merge_list *next;
	ഘടന merge_list *link;	/* other stages for this object */

	unsigned പൂർണ്ണസംഖ്യ stage : 2;
	unsigned പൂർണ്ണസംഖ്യ mode;
	സ്ഥിരം അക്ഷരം *path;
	ഘടന blob *blob;
};

സ്ഥിതം ഘടന merge_list *merge_result, **merge_result_end = &merge_result;

സ്ഥിതം ശൂന്യം add_merge_entry(ഘടന merge_list *entry)
{
	*merge_result_end = entry;
	merge_result_end = &entry->next;
}

സ്ഥിതം ശൂന്യം trivial_merge_trees(ഘടന വൃക്ഷം_desc t[3], സ്ഥിരം അക്ഷരം *base);

സ്ഥിതം സ്ഥിരം അക്ഷരം *explanation(ഘടന merge_list *entry)
{
	തിരഞ്ഞെടുപ്പ് (entry->stage) {
	അവസ്ഥ 0:
		തിരികെ "merged";
	അവസ്ഥ 3:
		തിരികെ "added in remote";
	അവസ്ഥ 2:
		എങ്കിൽ (entry->link)
			തിരികെ "added in both";
		തിരികെ "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	എങ്കിൽ (!entry)
		തിരികെ "removed in both";

	എങ്കിൽ (entry->link)
		തിരികെ "changed in both";

	എങ്കിൽ (entry->stage == 3)
		തിരികെ "removed in local";
	തിരികെ "removed in remote";
}

സ്ഥിതം ശൂന്യം *result(ഘടന merge_list *entry, size_t *size)
{
	എണ്ണൽ object_type type;
	ഘടന blob *base, *our, *their;
	സ്ഥിരം അക്ഷരം *path = entry->path;

	എങ്കിൽ (!entry->stage)
		തിരികെ odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       size);
	base = ശൂന്യ;
	എങ്കിൽ (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = ശൂന്യ;
	എങ്കിൽ (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = ശൂന്യ;
	എങ്കിൽ (entry)
		their = entry->blob;
	തിരികെ merge_blobs(the_repository->index, path,
			   base, our, their, size);
}

സ്ഥിതം ശൂന്യം *origin(ഘടന merge_list *entry, size_t *size)
{
	എണ്ണൽ object_type type;
	ആകുമ്പോൾ (entry) {
		എങ്കിൽ (entry->stage == 2)
			തിരികെ odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, size);
		entry = entry->link;
	}
	തിരികെ ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_outf(ശൂന്യം *priv UNUSED, mmfile_t *mb, പൂർണ്ണസംഖ്യ nbuf)
{
	പൂർണ്ണസംഖ്യ i;
	വേണ്ടി (i = 0; i < nbuf; i++)
		printf("%.*s", (പൂർണ്ണസംഖ്യ) mb[i].size, mb[i].ptr);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം show_diff(ഘടന merge_list *entry)
{
	size_t size;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_വരി = show_outf };

	memset(&xpp, 0, വലുപ്പം(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, വലുപ്പം(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &size);
	എങ്കിൽ (!src.ptr)
		size = 0;
	src.size = size;
	dst.ptr = result(entry, &size);
	എങ്കിൽ (!dst.ptr)
		size = 0;
	dst.size = size;
	എങ്കിൽ (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	സ്വതന്ത്രം(src.ptr);
	സ്വതന്ത്രം(dst.ptr);
}

സ്ഥിതം ശൂന്യം show_result_list(ഘടന merge_list *entry)
{
	printf("%s\n", explanation(entry));
	ചെയ്യുക {
		ഘടന merge_list *link = entry->link;
		സ്ഥിതം സ്ഥിരം അക്ഷരം *desc[4] = { "result", "base", "our", "their" };
		printf("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} ആകുമ്പോൾ (entry);
}

സ്ഥിതം ശൂന്യം show_result(ശൂന്യം)
{
	ഘടന merge_list *walk;

	walk = merge_result;
	ആകുമ്പോൾ (walk) {
		show_result_list(walk);
		show_diff(walk);
		walk = walk->next;
	}
}

/* An empty entry never compares same, not even to another empty entry */
സ്ഥിതം പൂർണ്ണസംഖ്യ same_entry(ഘടന name_entry *a, ഘടന name_entry *b)
{
	തിരികെ	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ both_empty(ഘടന name_entry *a, ഘടന name_entry *b)
{
	തിരികെ is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

സ്ഥിതം ഘടന merge_list *create_entry(unsigned stage, unsigned mode, സ്ഥിരം ഘടന object_id *oid, സ്ഥിരം അക്ഷരം *path)
{
	ഘടന merge_list *res = xcalloc(1, വലുപ്പം(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	തിരികെ res;
}

സ്ഥിതം അക്ഷരം *traverse_path(സ്ഥിരം ഘടന traverse_info *info, സ്ഥിരം ഘടന name_entry *n)
{
	ഘടന strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	തിരികെ strbuf_detach(&buf, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം resolve(സ്ഥിരം ഘടന traverse_info *info, ഘടന name_entry *ours, ഘടന name_entry *result)
{
	ഘടന merge_list *orig, *final;
	സ്ഥിരം അക്ഷരം *path;

	/* If it's already ours, don't bother showing it */
	എങ്കിൽ (!ours)
		തിരികെ;

	path = traverse_path(info, result);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, result->mode, &result->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

സ്ഥിതം ശൂന്യം unresolved_directory(സ്ഥിരം ഘടന traverse_info *info,
				 ഘടന name_entry n[3])
{
	ഘടന repository *r = the_repository;
	അക്ഷരം *newbase;
	ഘടന name_entry *p;
	ഘടന വൃക്ഷം_desc t[3];
	ശൂന്യം *buf0, *buf1, *buf2;

	വേണ്ടി (p = n; p < n + 3; p++) {
		എങ്കിൽ (p->mode && S_ISDIR(p->mode))
			നിർത്തുക;
	}
	എങ്കിൽ (n + 3 <= p)
		തിരികെ; /* there is no tree here */

	newbase = traverse_path(info, p);

#define ENTRY_OID(e) (((e)->mode && S_ISDIR((e)->mode)) ? &(e)->oid : ശൂന്യ)
	buf0 = fill_വൃക്ഷം_descriptor(r, t + 0, ENTRY_OID(n + 0));
	buf1 = fill_വൃക്ഷം_descriptor(r, t + 1, ENTRY_OID(n + 1));
	buf2 = fill_വൃക്ഷം_descriptor(r, t + 2, ENTRY_OID(n + 2));
#undef ENTRY_OID

	trivial_merge_trees(t, newbase);

	സ്വതന്ത്രം(buf0);
	സ്വതന്ത്രം(buf1);
	സ്വതന്ത്രം(buf2);
	സ്വതന്ത്രം(newbase);
}


സ്ഥിതം ഘടന merge_list *link_entry(unsigned stage, സ്ഥിരം ഘടന traverse_info *info, ഘടന name_entry *n, ഘടന merge_list *entry)
{
	സ്ഥിരം അക്ഷരം *path;
	ഘടന merge_list *link;

	എങ്കിൽ (!n->mode)
		തിരികെ entry;
	എങ്കിൽ (entry)
		path = entry->path;
	അല്ലെങ്കിൽ
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	തിരികെ link;
}

സ്ഥിതം ശൂന്യം unresolved(സ്ഥിരം ഘടന traverse_info *info, ഘടന name_entry n[3])
{
	ഘടന merge_list *entry = ശൂന്യ;
	പൂർണ്ണസംഖ്യ i;
	unsigned dirmask = 0, mask = 0;

	വേണ്ടി (i = 0; i < 3; i++) {
		mask |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		എങ്കിൽ (!n[i].mode || S_ISDIR(n[i].mode))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	എങ്കിൽ (dirmask == mask)
		തിരികെ;

	എങ്കിൽ (n[2].mode && !S_ISDIR(n[2].mode))
		entry = link_entry(3, info, n + 2, entry);
	എങ്കിൽ (n[1].mode && !S_ISDIR(n[1].mode))
		entry = link_entry(2, info, n + 1, entry);
	എങ്കിൽ (n[0].mode && !S_ISDIR(n[0].mode))
		entry = link_entry(1, info, n + 0, entry);

	add_merge_entry(entry);
}

/*
 * Merge two trees together (t[1] and t[2]), using a common base (t[0])
 * as the origin.
 *
 * This walks the (sorted) trees in lock-step, checking every possible
 * name. Note that directories automatically sort differently from other
 * files (see "base_name_compare"), so you'll never see file/directory
 * conflicts, because they won't ever compare the same.
 *
 * IOW, if a directory changes to a filename, it will automatically be
 * seen as the directory going away, and the filename being created.
 *
 * Think of this as a three-way diff.
 *
 * The output will be either:
 *  - successful merge
 *	 "0 mode sha1 filename"
 *    NOTE NOTE NOTE! FIXME! We really really need to walk the index
 *    in parallel with this too!
 *
 *  - conflict:
 *	"1 mode sha1 filename"
 *	"2 mode sha1 filename"
 *	"3 mode sha1 filename"
 *    where not all of the 1/2/3 lines may exist, of course.
 *
 * The successful merge rules are the same as for the three-way merge
 * in git-read-tree.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ threeway_callback(പൂർണ്ണസംഖ്യ n UNUSED, unsigned long mask,
			     unsigned long dirmask UNUSED,
			     ഘടന name_entry *entry, ഘടന traverse_info *info)
{
	/* Same in both? */
	എങ്കിൽ (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, ശൂന്യ, entry+1);
		തിരികെ mask;
	}

	എങ്കിൽ (same_entry(entry+0, entry+1)) {
		എങ്കിൽ (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			തിരികെ mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	എങ്കിൽ (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, ശൂന്യ, entry+1);
		തിരികെ mask;
	}

	unresolved(info, entry);
	തിരികെ mask;
}

സ്ഥിതം ശൂന്യം trivial_merge_trees(ഘടന വൃക്ഷം_desc t[3], സ്ഥിരം അക്ഷരം *base)
{
	ഘടന traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->index, 3, t, &info);
}

സ്ഥിതം ശൂന്യം *get_വൃക്ഷം_descriptor(ഘടന repository *r,
				 ഘടന വൃക്ഷം_desc *desc,
				 സ്ഥിരം അക്ഷരം *rev)
{
	ഘടന object_id oid;
	ശൂന്യം *buf;

	എങ്കിൽ (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_വൃക്ഷം_descriptor(r, desc, &oid);
	എങ്കിൽ (!buf)
		die("%s is not a tree", rev);
	തിരികെ buf;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ trivial_merge(സ്ഥിരം അക്ഷരം *base,
			 സ്ഥിരം അക്ഷരം *branch1,
			 സ്ഥിരം അക്ഷരം *branch2)
{
	ഘടന repository *r = the_repository;
	ഘടന വൃക്ഷം_desc t[3];
	ശൂന്യം *buf1, *buf2, *buf3;

	buf1 = get_വൃക്ഷം_descriptor(r, t+0, base);
	buf2 = get_വൃക്ഷം_descriptor(r, t+1, branch1);
	buf3 = get_വൃക്ഷം_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	സ്വതന്ത്രം(buf1);
	സ്വതന്ത്രം(buf2);
	സ്വതന്ത്രം(buf3);

	show_result();
	തിരികെ 0;
}

എണ്ണൽ mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

ഘടന merge_വൃക്ഷം_options {
	പൂർണ്ണസംഖ്യ mode;
	പൂർണ്ണസംഖ്യ allow_unrelated_histories;
	പൂർണ്ണസംഖ്യ show_messages;
	പൂർണ്ണസംഖ്യ name_only;
	പൂർണ്ണസംഖ്യ use_stdin;
	ഘടന merge_options merge_options;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ real_merge(ഘടന merge_വൃക്ഷം_options *o,
		      സ്ഥിരം അക്ഷരം *merge_base,
		      സ്ഥിരം അക്ഷരം *branch1, സ്ഥിരം അക്ഷരം *branch2,
		      സ്ഥിരം അക്ഷരം *prefix)
{
	ഘടന commit *parent1, *parent2;
	ഘടന commit_list *merge_bases = ശൂന്യ;
	ഘടന merge_result result = { 0 };
	പൂർണ്ണസംഖ്യ show_messages = o->show_messages;
	ഘടന merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	എങ്കിൽ (merge_base) {
		ഘടന വൃക്ഷം *base_വൃക്ഷം, *parent1_വൃക്ഷം, *parent2_വൃക്ഷം;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		ഘടന object_id base_oid, head_oid, merge_oid;

		എങ്കിൽ (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository,
						     &base_oid);
		എങ്കിൽ (!base_വൃക്ഷം)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		എങ്കിൽ (repo_get_oid_treeish(the_repository, branch1, &head_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository,
							&head_oid);
		എങ്കിൽ (!parent1_വൃക്ഷം)
			die(_("unable to read tree (%s)"), oid_to_hex(&head_oid));
		എങ്കിൽ (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository,
							&merge_oid);
		എങ്കിൽ (!parent2_വൃക്ഷം)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_വൃക്ഷം, parent1_വൃക്ഷം, parent2_വൃക്ഷം, &result);
	} അല്ലെങ്കിൽ {
		parent1 = get_merge_parent(branch1);
		എങ്കിൽ (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		എങ്കിൽ (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		എങ്കിൽ (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			exit(128);
		എങ്കിൽ (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &result);
		commit_list_free(merge_bases);
	}

	എങ്കിൽ (result.clean < 0)
		die(_("failure to merge"));

	എങ്കിൽ (o->merge_options.mergeability_only)
		പോകുക cleanup;

	എങ്കിൽ (show_messages == -1)
		show_messages = !result.clean;

	എങ്കിൽ (o->use_stdin)
		printf("%d%c", result.clean, വരി_termination);
	printf("%s%c", oid_to_hex(&result.വൃക്ഷം->object.oid), വരി_termination);
	എങ്കിൽ (!result.clean) {
		ഘടന string_list conflicted_files = STRING_LIST_INIT_NODUP;
		സ്ഥിരം അക്ഷരം *last = ശൂന്യ;

		merge_get_conflicted_files(&result, &conflicted_files);
		വേണ്ടി (size_t i = 0; i < conflicted_files.nr; i++) {
			സ്ഥിരം അക്ഷരം *name = conflicted_files.ഇനങ്ങൾ[i].string;
			ഘടന stage_info *c = conflicted_files.ഇനങ്ങൾ[i].util;
			എങ്കിൽ (!o->name_only)
				printf("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			അല്ലെങ്കിൽ എങ്കിൽ (last && !strcmp(last, name))
				തുടരുക;
			write_name_quoted_relative(
				name, prefix, stdout, വരി_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	എങ്കിൽ (show_messages) {
		putchar(വരി_termination);
		merge_display_update_messages(&opt, വരി_termination == '\0',
					      &result);
	}
	എങ്കിൽ (o->use_stdin)
		putchar(വരി_termination);

cleanup:
	merge_finalize(&opt, &result);
	clear_merge_options(&opt);
	തിരികെ !result.clean; /* result.clean < 0 handled above */
}

പൂർണ്ണസംഖ്യ cmd_merge_വൃക്ഷം(പൂർണ്ണസംഖ്യ argc,
		   സ്ഥിരം അക്ഷരം **argv,
		   സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo UNUSED)
{
	ഘടന merge_വൃക്ഷം_options o = { .show_messages = -1 };
	ഘടന strvec xopts = STRVEC_INIT;
	പൂർണ്ണസംഖ്യ expected_remaining_argc;
	പൂർണ്ണസംഖ്യ original_argc;
	സ്ഥിരം അക്ഷരം *merge_base = ശൂന്യ;
	പൂർണ്ണസംഖ്യ ret;
	പൂർണ്ണസംഖ്യ quiet = 0;

	സ്ഥിരം അക്ഷരം * സ്ഥിരം merge_വൃക്ഷം_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		ശൂന്യ
	};
	ഘടന option mt_options[] = {
		OPT_CMDMODE(0, "write-tree", &o.mode,
			    N_("do a real merge instead of a trivial merge"),
			    MODE_REAL),
		OPT_CMDMODE(0, "trivial-merge", &o.mode,
			    N_("do a trivial merge only"), MODE_TRIVIAL),
		OPT_BOOL(0, "messages", &o.show_messages,
			 N_("also show informational/conflict messages")),
		OPT_BOOL_F(0, "quiet",
			   &quiet,
			   N_("suppress all output; only exit status wanted"),
			   PARSE_OPT_NONEG),
		OPT_SET_INT('z', ശൂന്യ, &വരി_termination,
			    N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL_F(0, "name-only",
			   &o.name_only,
			   N_("list filenames without modes/oids/stages"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "allow-unrelated-histories",
			   &o.allow_unrelated_histories,
			   N_("allow merging unrelated histories"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin",
			   &o.use_stdin,
			   N_("perform multiple merges, one per line of input"),
			   PARSE_OPT_NONEG),
		OPT_STRING(0, "merge-base",
			   &merge_base,
			   N_("tree-ish"),
			   N_("specify a merge-base for the merge")),
		OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
			N_("option for selected merge strategy")),
		OPT_END()
	};

	/* Init merge options */
	init_basic_merge_options(&o.merge_options, the_repository);

	/* Parse arguments */
	original_argc = argc - 1; /* ignoring argv[0] */
	argc = parse_options(argc, argv, prefix, mt_options,
			     merge_വൃക്ഷം_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	എങ്കിൽ (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !വരി_termination, "-z");

	എങ്കിൽ (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	വേണ്ടി (size_t x = 0; x < xopts.nr; x++)
		എങ്കിൽ (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	എങ്കിൽ (o.use_stdin) {
		ഘടന strbuf buf = STRBUF_INIT;

		എങ്കിൽ (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		എങ്കിൽ (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		വരി_termination = '\0';
		ആകുമ്പോൾ (strbuf_getline_lf(&buf, stdin) != EOF) {
			ഘടന string_list split = STRING_LIST_INIT_NODUP;
			സ്ഥിരം അക്ഷരം *നിവേശം_merge_base = ശൂന്യ;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			എങ്കിൽ (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			എങ്കിൽ (!strcmp(split.ഇനങ്ങൾ[1].string, "--")) {
				നിവേശം_merge_base = split.ഇനങ്ങൾ[0].string;
			}

			എങ്കിൽ (നിവേശം_merge_base && split.nr == 4) {
				real_merge(&o, നിവേശം_merge_base,
					   split.ഇനങ്ങൾ[2].string, split.ഇനങ്ങൾ[3].string,
					   prefix);
			} അല്ലെങ്കിൽ എങ്കിൽ (!നിവേശം_merge_base && split.nr == 2) {
				real_merge(&o, ശൂന്യ,
					   split.ഇനങ്ങൾ[0].string, split.ഇനങ്ങൾ[1].string,
					   prefix);
			} അല്ലെങ്കിൽ {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&split, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		പോകുക out;
	}

	/* Figure out which mode to use */
	തിരഞ്ഞെടുപ്പ് (o.mode) {
	സ്ഥിരസ്ഥിതി:
		BUG("unexpected command mode %d", o.mode);
	അവസ്ഥ MODE_UNKNOWN:
		തിരഞ്ഞെടുപ്പ് (argc) {
		സ്ഥിരസ്ഥിതി:
			usage_with_options(merge_വൃക്ഷം_usage, mt_options);
		അവസ്ഥ 2:
			o.mode = MODE_REAL;
			നിർത്തുക;
		അവസ്ഥ 3:
			o.mode = MODE_TRIVIAL;
			നിർത്തുക;
		}
		expected_remaining_argc = argc;
		നിർത്തുക;
	അവസ്ഥ MODE_REAL:
		expected_remaining_argc = 2;
		നിർത്തുക;
	അവസ്ഥ MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		നിർത്തുക;
	}
	എങ്കിൽ (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	എങ്കിൽ (argc != expected_remaining_argc)
		usage_with_options(merge_വൃക്ഷം_usage, mt_options);

	repo_config(the_repository, git_default_config, ശൂന്യ);

	/* Do the relevant type of merge */
	എങ്കിൽ (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	അല്ലെങ്കിൽ
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	തിരികെ ret;
}
