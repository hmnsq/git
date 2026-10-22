#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "environment.h"
#समावेश "tree-walk.h"
#समावेश "xdiff-interface.h"
#समावेश "help.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "commit.h"
#समावेश "commit-reach.h"
#समावेश "merge-ort.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "parse-options.h"
#समावेश "blob.h"
#समावेश "merge-blobs.h"
#समावेश "quote.h"
#समावेश "tree.h"
#समावेश "config.h"
#समावेश "strvec.h"
#समावेश "write-or-die.h"

अटल पूर्णांक पंक्ति_termination = '\n';

काष्ठा merge_list {
	काष्ठा merge_list *अग्र;
	काष्ठा merge_list *link;	/* other stages for this object */

	अचिन्हित पूर्णांक stage : 2;
	अचिन्हित पूर्णांक mode;
	स्थिर अक्षर *path;
	काष्ठा blob *blob;
};

अटल काष्ठा merge_list *merge_परिणाम, **merge_परिणाम_end = &merge_परिणाम;

अटल व्योम add_merge_entry(काष्ठा merge_list *entry)
{
	*merge_परिणाम_end = entry;
	merge_परिणाम_end = &entry->अग्र;
}

अटल व्योम trivial_merge_trees(काष्ठा वृक्ष_desc t[3], स्थिर अक्षर *base);

अटल स्थिर अक्षर *explanation(काष्ठा merge_list *entry)
{
	चयन (entry->stage) {
	हाल 0:
		वापस "merged";
	हाल 3:
		वापस "added in remote";
	हाल 2:
		अगर (entry->link)
			वापस "added in both";
		वापस "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	अगर (!entry)
		वापस "removed in both";

	अगर (entry->link)
		वापस "changed in both";

	अगर (entry->stage == 3)
		वापस "removed in local";
	वापस "removed in remote";
}

अटल व्योम *परिणाम(काष्ठा merge_list *entry, माप_प्रकार *आकार)
{
	क्रमागत object_type type;
	काष्ठा blob *base, *our, *their;
	स्थिर अक्षर *path = entry->path;

	अगर (!entry->stage)
		वापस odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       आकार);
	base = शून्य;
	अगर (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = शून्य;
	अगर (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = शून्य;
	अगर (entry)
		their = entry->blob;
	वापस merge_blobs(the_repository->सूचकांक, path,
			   base, our, their, आकार);
}

अटल व्योम *origin(काष्ठा merge_list *entry, माप_प्रकार *आकार)
{
	क्रमागत object_type type;
	जबतक (entry) {
		अगर (entry->stage == 2)
			वापस odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, आकार);
		entry = entry->link;
	}
	वापस शून्य;
}

अटल पूर्णांक show_outf(व्योम *priv UNUSED, mmfile_t *mb, पूर्णांक nbuf)
{
	पूर्णांक i;
	क्रम (i = 0; i < nbuf; i++)
		म_लिखो("%.*s", (पूर्णांक) mb[i].आकार, mb[i].ptr);
	वापस 0;
}

अटल व्योम show_diff(काष्ठा merge_list *entry)
{
	माप_प्रकार आकार;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_पंक्ति = show_outf };

	स_रखो(&xpp, 0, माप(xpp));
	xpp.flags = 0;
	स_रखो(&xecfg, 0, माप(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &आकार);
	अगर (!src.ptr)
		आकार = 0;
	src.आकार = आकार;
	dst.ptr = परिणाम(entry, &आकार);
	अगर (!dst.ptr)
		आकार = 0;
	dst.आकार = आकार;
	अगर (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	मुक्त(src.ptr);
	मुक्त(dst.ptr);
}

अटल व्योम show_परिणाम_list(काष्ठा merge_list *entry)
{
	म_लिखो("%s\n", explanation(entry));
	करो {
		काष्ठा merge_list *link = entry->link;
		अटल स्थिर अक्षर *desc[4] = { "result", "base", "our", "their" };
		म_लिखो("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} जबतक (entry);
}

अटल व्योम show_परिणाम(व्योम)
{
	काष्ठा merge_list *walk;

	walk = merge_परिणाम;
	जबतक (walk) {
		show_परिणाम_list(walk);
		show_diff(walk);
		walk = walk->अग्र;
	}
}

/* An empty entry never compares same, not even to another empty entry */
अटल पूर्णांक same_entry(काष्ठा name_entry *a, काष्ठा name_entry *b)
{
	वापस	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

अटल पूर्णांक both_empty(काष्ठा name_entry *a, काष्ठा name_entry *b)
{
	वापस is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

अटल काष्ठा merge_list *create_entry(अचिन्हित stage, अचिन्हित mode, स्थिर काष्ठा object_id *oid, स्थिर अक्षर *path)
{
	काष्ठा merge_list *res = xcalloc(1, माप(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	वापस res;
}

अटल अक्षर *traverse_path(स्थिर काष्ठा traverse_info *info, स्थिर काष्ठा name_entry *n)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	वापस strbuf_detach(&buf, शून्य);
}

अटल व्योम resolve(स्थिर काष्ठा traverse_info *info, काष्ठा name_entry *ours, काष्ठा name_entry *परिणाम)
{
	काष्ठा merge_list *orig, *final;
	स्थिर अक्षर *path;

	/* If it's already ours, don't bother showing it */
	अगर (!ours)
		वापस;

	path = traverse_path(info, परिणाम);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, परिणाम->mode, &परिणाम->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

अटल व्योम unresolved_directory(स्थिर काष्ठा traverse_info *info,
				 काष्ठा name_entry n[3])
{
	काष्ठा repository *r = the_repository;
	अक्षर *newbase;
	काष्ठा name_entry *p;
	काष्ठा वृक्ष_desc t[3];
	व्योम *buf0, *buf1, *buf2;

	क्रम (p = n; p < n + 3; p++) {
		अगर (p->mode && S_ISDIR(p->mode))
			अवरोध;
	}
	अगर (n + 3 <= p)
		वापस; /* there is no tree here */

	newbase = traverse_path(info, p);

#घोषणा ENTRY_OID(e) (((e)->mode && S_ISDIR((e)->mode)) ? &(e)->oid : शून्य)
	buf0 = fill_वृक्ष_descriptor(r, t + 0, ENTRY_OID(n + 0));
	buf1 = fill_वृक्ष_descriptor(r, t + 1, ENTRY_OID(n + 1));
	buf2 = fill_वृक्ष_descriptor(r, t + 2, ENTRY_OID(n + 2));
#अघोषित ENTRY_OID

	trivial_merge_trees(t, newbase);

	मुक्त(buf0);
	मुक्त(buf1);
	मुक्त(buf2);
	मुक्त(newbase);
}


अटल काष्ठा merge_list *link_entry(अचिन्हित stage, स्थिर काष्ठा traverse_info *info, काष्ठा name_entry *n, काष्ठा merge_list *entry)
{
	स्थिर अक्षर *path;
	काष्ठा merge_list *link;

	अगर (!n->mode)
		वापस entry;
	अगर (entry)
		path = entry->path;
	अन्यथा
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	वापस link;
}

अटल व्योम unresolved(स्थिर काष्ठा traverse_info *info, काष्ठा name_entry n[3])
{
	काष्ठा merge_list *entry = शून्य;
	पूर्णांक i;
	अचिन्हित dirmask = 0, mask = 0;

	क्रम (i = 0; i < 3; i++) {
		mask |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		अगर (!n[i].mode || S_ISDIR(n[i].mode))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	अगर (dirmask == mask)
		वापस;

	अगर (n[2].mode && !S_ISDIR(n[2].mode))
		entry = link_entry(3, info, n + 2, entry);
	अगर (n[1].mode && !S_ISDIR(n[1].mode))
		entry = link_entry(2, info, n + 1, entry);
	अगर (n[0].mode && !S_ISDIR(n[0].mode))
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
अटल पूर्णांक threeway_callback(पूर्णांक n UNUSED, अचिन्हित दीर्घ mask,
			     अचिन्हित दीर्घ dirmask UNUSED,
			     काष्ठा name_entry *entry, काष्ठा traverse_info *info)
{
	/* Same in both? */
	अगर (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, शून्य, entry+1);
		वापस mask;
	}

	अगर (same_entry(entry+0, entry+1)) {
		अगर (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			वापस mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	अगर (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, शून्य, entry+1);
		वापस mask;
	}

	unresolved(info, entry);
	वापस mask;
}

अटल व्योम trivial_merge_trees(काष्ठा वृक्ष_desc t[3], स्थिर अक्षर *base)
{
	काष्ठा traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->सूचकांक, 3, t, &info);
}

अटल व्योम *get_वृक्ष_descriptor(काष्ठा repository *r,
				 काष्ठा वृक्ष_desc *desc,
				 स्थिर अक्षर *rev)
{
	काष्ठा object_id oid;
	व्योम *buf;

	अगर (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_वृक्ष_descriptor(r, desc, &oid);
	अगर (!buf)
		die("%s is not a tree", rev);
	वापस buf;
}

अटल पूर्णांक trivial_merge(स्थिर अक्षर *base,
			 स्थिर अक्षर *branch1,
			 स्थिर अक्षर *branch2)
{
	काष्ठा repository *r = the_repository;
	काष्ठा वृक्ष_desc t[3];
	व्योम *buf1, *buf2, *buf3;

	buf1 = get_वृक्ष_descriptor(r, t+0, base);
	buf2 = get_वृक्ष_descriptor(r, t+1, branch1);
	buf3 = get_वृक्ष_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	मुक्त(buf1);
	मुक्त(buf2);
	मुक्त(buf3);

	show_परिणाम();
	वापस 0;
}

क्रमागत mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

काष्ठा merge_वृक्ष_options {
	पूर्णांक mode;
	पूर्णांक allow_unrelated_histories;
	पूर्णांक show_messages;
	पूर्णांक name_only;
	पूर्णांक use_stdin;
	काष्ठा merge_options merge_options;
};

अटल पूर्णांक real_merge(काष्ठा merge_वृक्ष_options *o,
		      स्थिर अक्षर *merge_base,
		      स्थिर अक्षर *branch1, स्थिर अक्षर *branch2,
		      स्थिर अक्षर *prefix)
{
	काष्ठा commit *parent1, *parent2;
	काष्ठा commit_list *merge_bases = शून्य;
	काष्ठा merge_परिणाम परिणाम = { 0 };
	पूर्णांक show_messages = o->show_messages;
	काष्ठा merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	अगर (merge_base) {
		काष्ठा वृक्ष *base_वृक्ष, *parent1_वृक्ष, *parent2_वृक्ष;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		काष्ठा object_id base_oid, शीर्ष_oid, merge_oid;

		अगर (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_वृक्ष = repo_parse_वृक्ष_indirect(the_repository,
						     &base_oid);
		अगर (!base_वृक्ष)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		अगर (repo_get_oid_treeish(the_repository, branch1, &शीर्ष_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_वृक्ष = repo_parse_वृक्ष_indirect(the_repository,
							&शीर्ष_oid);
		अगर (!parent1_वृक्ष)
			die(_("unable to read tree (%s)"), oid_to_hex(&शीर्ष_oid));
		अगर (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_वृक्ष = repo_parse_वृक्ष_indirect(the_repository,
							&merge_oid);
		अगर (!parent2_वृक्ष)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_वृक्ष, parent1_वृक्ष, parent2_वृक्ष, &परिणाम);
	} अन्यथा {
		parent1 = get_merge_parent(branch1);
		अगर (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		अगर (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		अगर (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			निकास(128);
		अगर (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &परिणाम);
		commit_list_free(merge_bases);
	}

	अगर (परिणाम.clean < 0)
		die(_("failure to merge"));

	अगर (o->merge_options.mergeability_only)
		जाओ cleanup;

	अगर (show_messages == -1)
		show_messages = !परिणाम.clean;

	अगर (o->use_stdin)
		म_लिखो("%d%c", परिणाम.clean, पंक्ति_termination);
	म_लिखो("%s%c", oid_to_hex(&परिणाम.वृक्ष->object.oid), पंक्ति_termination);
	अगर (!परिणाम.clean) {
		काष्ठा string_list conflicted_files = STRING_LIST_INIT_NODUP;
		स्थिर अक्षर *last = शून्य;

		merge_get_conflicted_files(&परिणाम, &conflicted_files);
		क्रम (माप_प्रकार i = 0; i < conflicted_files.nr; i++) {
			स्थिर अक्षर *name = conflicted_files.वस्तुएँ[i].string;
			काष्ठा stage_info *c = conflicted_files.वस्तुएँ[i].util;
			अगर (!o->name_only)
				म_लिखो("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			अन्यथा अगर (last && !म_भेद(last, name))
				जारी;
			write_name_quoted_relative(
				name, prefix, मानक_निकास, पंक्ति_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	अगर (show_messages) {
		अक्षर_दो(पंक्ति_termination);
		merge_display_update_messages(&opt, पंक्ति_termination == '\0',
					      &परिणाम);
	}
	अगर (o->use_stdin)
		अक्षर_दो(पंक्ति_termination);

cleanup:
	merge_finalize(&opt, &परिणाम);
	clear_merge_options(&opt);
	वापस !परिणाम.clean; /* result.clean < 0 handled above */
}

पूर्णांक cmd_merge_वृक्ष(पूर्णांक argc,
		   स्थिर अक्षर **argv,
		   स्थिर अक्षर *prefix,
		   काष्ठा repository *repo UNUSED)
{
	काष्ठा merge_वृक्ष_options o = { .show_messages = -1 };
	काष्ठा strvec xopts = STRVEC_INIT;
	पूर्णांक expected_remaining_argc;
	पूर्णांक original_argc;
	स्थिर अक्षर *merge_base = शून्य;
	पूर्णांक ret;
	पूर्णांक quiet = 0;

	स्थिर अक्षर * स्थिर merge_वृक्ष_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		शून्य
	};
	काष्ठा option mt_options[] = {
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
		OPT_SET_INT('z', शून्य, &पंक्ति_termination,
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

	अगर (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !पंक्ति_termination, "-z");

	अगर (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	क्रम (माप_प्रकार x = 0; x < xopts.nr; x++)
		अगर (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	अगर (o.use_stdin) {
		काष्ठा strbuf buf = STRBUF_INIT;

		अगर (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		अगर (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		पंक्ति_termination = '\0';
		जबतक (strbuf_getline_lf(&buf, मानक_निवेश) != खातापूर्ण) {
			काष्ठा string_list split = STRING_LIST_INIT_NODUP;
			स्थिर अक्षर *निवेश_merge_base = शून्य;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			अगर (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			अगर (!म_भेद(split.वस्तुएँ[1].string, "--")) {
				निवेश_merge_base = split.वस्तुएँ[0].string;
			}

			अगर (निवेश_merge_base && split.nr == 4) {
				real_merge(&o, निवेश_merge_base,
					   split.वस्तुएँ[2].string, split.वस्तुएँ[3].string,
					   prefix);
			} अन्यथा अगर (!निवेश_merge_base && split.nr == 2) {
				real_merge(&o, शून्य,
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
		जाओ out;
	}

	/* Figure out which mode to use */
	चयन (o.mode) {
	शेष:
		BUG("unexpected command mode %d", o.mode);
	हाल MODE_UNKNOWN:
		चयन (argc) {
		शेष:
			usage_with_options(merge_वृक्ष_usage, mt_options);
		हाल 2:
			o.mode = MODE_REAL;
			अवरोध;
		हाल 3:
			o.mode = MODE_TRIVIAL;
			अवरोध;
		}
		expected_remaining_argc = argc;
		अवरोध;
	हाल MODE_REAL:
		expected_remaining_argc = 2;
		अवरोध;
	हाल MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		अवरोध;
	}
	अगर (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	अगर (argc != expected_remaining_argc)
		usage_with_options(merge_वृक्ष_usage, mt_options);

	repo_config(the_repository, git_default_config, शून्य);

	/* Do the relevant type of merge */
	अगर (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	अन्यथा
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	वापस ret;
}
