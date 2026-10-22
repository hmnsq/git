#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "xdiff-interface.h"
#अन्तर्भाव "help.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "merge-ort.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "merge-blobs.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "write-or-die.h"

स्थिर पूर्णाङ्क पंक्ति_termination = '\n';

संरचना merge_list {
	संरचना merge_list *next;
	संरचना merge_list *link;	/* other stages for this object */

	अचिन्हित पूर्णाङ्क stage : 2;
	अचिन्हित पूर्णाङ्क mode;
	ध्रुव वर्ण *path;
	संरचना blob *blob;
};

स्थिर संरचना merge_list *merge_result, **merge_result_end = &merge_result;

स्थिर शून्य add_merge_entry(संरचना merge_list *entry)
{
	*merge_result_end = entry;
	merge_result_end = &entry->next;
}

स्थिर शून्य trivial_merge_trees(संरचना वृक्ष_desc t[3], ध्रुव वर्ण *base);

स्थिर ध्रुव वर्ण *explanation(संरचना merge_list *entry)
{
	विकल्प (entry->stage) {
	पक्ष 0:
		प्रत्यागम "merged";
	पक्ष 3:
		प्रत्यागम "added in remote";
	पक्ष 2:
		यदि (entry->link)
			प्रत्यागम "added in both";
		प्रत्यागम "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	यदि (!entry)
		प्रत्यागम "removed in both";

	यदि (entry->link)
		प्रत्यागम "changed in both";

	यदि (entry->stage == 3)
		प्रत्यागम "removed in local";
	प्रत्यागम "removed in remote";
}

स्थिर शून्य *result(संरचना merge_list *entry, माप_प्रकार *size)
{
	गणना object_type type;
	संरचना blob *base, *our, *their;
	ध्रुव वर्ण *path = entry->path;

	यदि (!entry->stage)
		प्रत्यागम odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       size);
	base = रिक्त;
	यदि (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = रिक्त;
	यदि (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = रिक्त;
	यदि (entry)
		their = entry->blob;
	प्रत्यागम merge_blobs(the_repository->index, path,
			   base, our, their, size);
}

स्थिर शून्य *origin(संरचना merge_list *entry, माप_प्रकार *size)
{
	गणना object_type type;
	यावत् (entry) {
		यदि (entry->stage == 2)
			प्रत्यागम odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, size);
		entry = entry->link;
	}
	प्रत्यागम रिक्त;
}

स्थिर पूर्णाङ्क show_outf(शून्य *priv UNUSED, mmfile_t *mb, पूर्णाङ्क nbuf)
{
	पूर्णाङ्क i;
	प्रति (i = 0; i < nbuf; i++)
		म_लिखो("%.*s", (पूर्णाङ्क) mb[i].size, mb[i].ptr);
	प्रत्यागम 0;
}

स्थिर शून्य show_diff(संरचना merge_list *entry)
{
	माप_प्रकार size;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_पंक्ति = show_outf };

	स_रखो(&xpp, 0, परिमाण(xpp));
	xpp.flags = 0;
	स_रखो(&xecfg, 0, परिमाण(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &size);
	यदि (!src.ptr)
		size = 0;
	src.size = size;
	dst.ptr = result(entry, &size);
	यदि (!dst.ptr)
		size = 0;
	dst.size = size;
	यदि (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	मुक्त(src.ptr);
	मुक्त(dst.ptr);
}

स्थिर शून्य show_result_list(संरचना merge_list *entry)
{
	म_लिखो("%s\n", explanation(entry));
	कुरु {
		संरचना merge_list *link = entry->link;
		स्थिर ध्रुव वर्ण *desc[4] = { "result", "base", "our", "their" };
		म_लिखो("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} यावत् (entry);
}

स्थिर शून्य show_result(शून्य)
{
	संरचना merge_list *walk;

	walk = merge_result;
	यावत् (walk) {
		show_result_list(walk);
		show_diff(walk);
		walk = walk->next;
	}
}

/* An empty entry never compares same, not even to another empty entry */
स्थिर पूर्णाङ्क same_entry(संरचना name_entry *a, संरचना name_entry *b)
{
	प्रत्यागम	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

स्थिर पूर्णाङ्क both_empty(संरचना name_entry *a, संरचना name_entry *b)
{
	प्रत्यागम is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

स्थिर संरचना merge_list *create_entry(अचिन्हित stage, अचिन्हित mode, ध्रुव संरचना object_id *oid, ध्रुव वर्ण *path)
{
	संरचना merge_list *res = xcalloc(1, परिमाण(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	प्रत्यागम res;
}

स्थिर वर्ण *traverse_path(ध्रुव संरचना traverse_info *info, ध्रुव संरचना name_entry *n)
{
	संरचना strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	प्रत्यागम strbuf_detach(&buf, रिक्त);
}

स्थिर शून्य resolve(ध्रुव संरचना traverse_info *info, संरचना name_entry *ours, संरचना name_entry *result)
{
	संरचना merge_list *orig, *final;
	ध्रुव वर्ण *path;

	/* If it's already ours, don't bother showing it */
	यदि (!ours)
		प्रत्यागम;

	path = traverse_path(info, result);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, result->mode, &result->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

स्थिर शून्य unresolved_directory(ध्रुव संरचना traverse_info *info,
				 संरचना name_entry n[3])
{
	संरचना repository *r = the_repository;
	वर्ण *newbase;
	संरचना name_entry *p;
	संरचना वृक्ष_desc t[3];
	शून्य *buf0, *buf1, *buf2;

	प्रति (p = n; p < n + 3; p++) {
		यदि (p->mode && S_ISDIR(p->mode))
			भङ्ग;
	}
	यदि (n + 3 <= p)
		प्रत्यागम; /* there is no tree here */

	newbase = traverse_path(info, p);

#define ENTRY_OID(e) (((e)->mode && S_ISDIR((e)->mode)) ? &(e)->oid : रिक्त)
	buf0 = fill_वृक्ष_descriptor(r, t + 0, ENTRY_OID(n + 0));
	buf1 = fill_वृक्ष_descriptor(r, t + 1, ENTRY_OID(n + 1));
	buf2 = fill_वृक्ष_descriptor(r, t + 2, ENTRY_OID(n + 2));
#undef ENTRY_OID

	trivial_merge_trees(t, newbase);

	मुक्त(buf0);
	मुक्त(buf1);
	मुक्त(buf2);
	मुक्त(newbase);
}


स्थिर संरचना merge_list *link_entry(अचिन्हित stage, ध्रुव संरचना traverse_info *info, संरचना name_entry *n, संरचना merge_list *entry)
{
	ध्रुव वर्ण *path;
	संरचना merge_list *link;

	यदि (!n->mode)
		प्रत्यागम entry;
	यदि (entry)
		path = entry->path;
	अन्यथा
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	प्रत्यागम link;
}

स्थिर शून्य unresolved(ध्रुव संरचना traverse_info *info, संरचना name_entry n[3])
{
	संरचना merge_list *entry = रिक्त;
	पूर्णाङ्क i;
	अचिन्हित dirmask = 0, mask = 0;

	प्रति (i = 0; i < 3; i++) {
		mask |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		यदि (!n[i].mode || S_ISDIR(n[i].mode))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	यदि (dirmask == mask)
		प्रत्यागम;

	यदि (n[2].mode && !S_ISDIR(n[2].mode))
		entry = link_entry(3, info, n + 2, entry);
	यदि (n[1].mode && !S_ISDIR(n[1].mode))
		entry = link_entry(2, info, n + 1, entry);
	यदि (n[0].mode && !S_ISDIR(n[0].mode))
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
स्थिर पूर्णाङ्क threeway_callback(पूर्णाङ्क n UNUSED, अचिन्हित दीर्घ mask,
			     अचिन्हित दीर्घ dirmask UNUSED,
			     संरचना name_entry *entry, संरचना traverse_info *info)
{
	/* Same in both? */
	यदि (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, रिक्त, entry+1);
		प्रत्यागम mask;
	}

	यदि (same_entry(entry+0, entry+1)) {
		यदि (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			प्रत्यागम mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	यदि (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, रिक्त, entry+1);
		प्रत्यागम mask;
	}

	unresolved(info, entry);
	प्रत्यागम mask;
}

स्थिर शून्य trivial_merge_trees(संरचना वृक्ष_desc t[3], ध्रुव वर्ण *base)
{
	संरचना traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->index, 3, t, &info);
}

स्थिर शून्य *get_वृक्ष_descriptor(संरचना repository *r,
				 संरचना वृक्ष_desc *desc,
				 ध्रुव वर्ण *rev)
{
	संरचना object_id oid;
	शून्य *buf;

	यदि (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_वृक्ष_descriptor(r, desc, &oid);
	यदि (!buf)
		die("%s is not a tree", rev);
	प्रत्यागम buf;
}

स्थिर पूर्णाङ्क trivial_merge(ध्रुव वर्ण *base,
			 ध्रुव वर्ण *branch1,
			 ध्रुव वर्ण *branch2)
{
	संरचना repository *r = the_repository;
	संरचना वृक्ष_desc t[3];
	शून्य *buf1, *buf2, *buf3;

	buf1 = get_वृक्ष_descriptor(r, t+0, base);
	buf2 = get_वृक्ष_descriptor(r, t+1, branch1);
	buf3 = get_वृक्ष_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	मुक्त(buf1);
	मुक्त(buf2);
	मुक्त(buf3);

	show_result();
	प्रत्यागम 0;
}

गणना mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

संरचना merge_वृक्ष_options {
	पूर्णाङ्क mode;
	पूर्णाङ्क allow_unrelated_histories;
	पूर्णाङ्क show_messages;
	पूर्णाङ्क name_only;
	पूर्णाङ्क use_stdin;
	संरचना merge_options merge_options;
};

स्थिर पूर्णाङ्क real_merge(संरचना merge_वृक्ष_options *o,
		      ध्रुव वर्ण *merge_base,
		      ध्रुव वर्ण *branch1, ध्रुव वर्ण *branch2,
		      ध्रुव वर्ण *prefix)
{
	संरचना commit *parent1, *parent2;
	संरचना commit_list *merge_bases = रिक्त;
	संरचना merge_result result = { 0 };
	पूर्णाङ्क show_messages = o->show_messages;
	संरचना merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	यदि (merge_base) {
		संरचना वृक्ष *base_वृक्ष, *parent1_वृक्ष, *parent2_वृक्ष;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		संरचना object_id base_oid, head_oid, merge_oid;

		यदि (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_वृक्ष = repo_parse_वृक्ष_indirect(the_repository,
						     &base_oid);
		यदि (!base_वृक्ष)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		यदि (repo_get_oid_treeish(the_repository, branch1, &head_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_वृक्ष = repo_parse_वृक्ष_indirect(the_repository,
							&head_oid);
		यदि (!parent1_वृक्ष)
			die(_("unable to read tree (%s)"), oid_to_hex(&head_oid));
		यदि (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_वृक्ष = repo_parse_वृक्ष_indirect(the_repository,
							&merge_oid);
		यदि (!parent2_वृक्ष)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_वृक्ष, parent1_वृक्ष, parent2_वृक्ष, &result);
	} अन्यथा {
		parent1 = get_merge_parent(branch1);
		यदि (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		यदि (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		यदि (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			निकास(128);
		यदि (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &result);
		commit_list_free(merge_bases);
	}

	यदि (result.clean < 0)
		die(_("failure to merge"));

	यदि (o->merge_options.mergeability_only)
		गच्छ cleanup;

	यदि (show_messages == -1)
		show_messages = !result.clean;

	यदि (o->use_stdin)
		म_लिखो("%d%c", result.clean, पंक्ति_termination);
	म_लिखो("%s%c", oid_to_hex(&result.वृक्ष->object.oid), पंक्ति_termination);
	यदि (!result.clean) {
		संरचना string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ध्रुव वर्ण *last = रिक्त;

		merge_get_conflicted_files(&result, &conflicted_files);
		प्रति (माप_प्रकार i = 0; i < conflicted_files.nr; i++) {
			ध्रुव वर्ण *name = conflicted_files.वस्तुएँ[i].string;
			संरचना stage_info *c = conflicted_files.वस्तुएँ[i].util;
			यदि (!o->name_only)
				म_लिखो("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			अन्यथा यदि (last && !म_भेद(last, name))
				अनुवर्त;
			write_name_quoted_relative(
				name, prefix, मानक_निकास, पंक्ति_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	यदि (show_messages) {
		अक्षर_दो(पंक्ति_termination);
		merge_display_update_messages(&opt, पंक्ति_termination == '\0',
					      &result);
	}
	यदि (o->use_stdin)
		अक्षर_दो(पंक्ति_termination);

cleanup:
	merge_finalize(&opt, &result);
	clear_merge_options(&opt);
	प्रत्यागम !result.clean; /* result.clean < 0 handled above */
}

पूर्णाङ्क cmd_merge_वृक्ष(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *prefix,
		   संरचना repository *repo UNUSED)
{
	संरचना merge_वृक्ष_options o = { .show_messages = -1 };
	संरचना strvec xopts = STRVEC_INIT;
	पूर्णाङ्क expected_remaining_argc;
	पूर्णाङ्क original_argc;
	ध्रुव वर्ण *merge_base = रिक्त;
	पूर्णाङ्क ret;
	पूर्णाङ्क quiet = 0;

	ध्रुव वर्ण * ध्रुव merge_वृक्ष_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		रिक्त
	};
	संरचना option mt_options[] = {
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
		OPT_SET_INT('z', रिक्त, &पंक्ति_termination,
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
			     merge_वृक्ष_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	यदि (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !पंक्ति_termination, "-z");

	यदि (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	प्रति (माप_प्रकार x = 0; x < xopts.nr; x++)
		यदि (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	यदि (o.use_stdin) {
		संरचना strbuf buf = STRBUF_INIT;

		यदि (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		यदि (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		पंक्ति_termination = '\0';
		यावत् (strbuf_getline_lf(&buf, मानक_निवेश) != खातापूर्ण) {
			संरचना string_list split = STRING_LIST_INIT_NODUP;
			ध्रुव वर्ण *निवेश_merge_base = रिक्त;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			यदि (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			यदि (!म_भेद(split.वस्तुएँ[1].string, "--")) {
				निवेश_merge_base = split.वस्तुएँ[0].string;
			}

			यदि (निवेश_merge_base && split.nr == 4) {
				real_merge(&o, निवेश_merge_base,
					   split.वस्तुएँ[2].string, split.वस्तुएँ[3].string,
					   prefix);
			} अन्यथा यदि (!निवेश_merge_base && split.nr == 2) {
				real_merge(&o, रिक्त,
					   split.वस्तुएँ[0].string, split.वस्तुएँ[1].string,
					   prefix);
			} अन्यथा {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(मानक_निकास, "stdout");

			string_list_clear(&split, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		गच्छ out;
	}

	/* Figure out which mode to use */
	विकल्प (o.mode) {
	स्वभाव:
		BUG("unexpected command mode %d", o.mode);
	पक्ष MODE_UNKNOWN:
		विकल्प (argc) {
		स्वभाव:
			usage_with_options(merge_वृक्ष_usage, mt_options);
		पक्ष 2:
			o.mode = MODE_REAL;
			भङ्ग;
		पक्ष 3:
			o.mode = MODE_TRIVIAL;
			भङ्ग;
		}
		expected_remaining_argc = argc;
		भङ्ग;
	पक्ष MODE_REAL:
		expected_remaining_argc = 2;
		भङ्ग;
	पक्ष MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		भङ्ग;
	}
	यदि (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	यदि (argc != expected_remaining_argc)
		usage_with_options(merge_वृक्ष_usage, mt_options);

	repo_config(the_repository, git_default_config, रिक्त);

	/* Do the relevant type of merge */
	यदि (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	अन्यथा
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	प्रत्यागम ret;
}
