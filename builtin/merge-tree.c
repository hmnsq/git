#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "tree-walk.h"
#અન્તર્ભાવ "xdiff-interface.h"
#અન્તર્ભાવ "help.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "merge-ort.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "blob.h"
#અન્તર્ભાવ "merge-blobs.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "write-or-die.h"

સ્થિર પૂર્ણાંક લીટી_termination = '\n';

સંરચના merge_list {
	સંરચના merge_list *next;
	સંરચના merge_list *link;	/* other stages for this object */

	unsigned પૂર્ણાંક stage : 2;
	unsigned પૂર્ણાંક mode;
	અચલ અક્ષર *path;
	સંરચના blob *blob;
};

સ્થિર સંરચના merge_list *merge_result, **merge_result_end = &merge_result;

સ્થિર શૂન્ય add_merge_entry(સંરચના merge_list *entry)
{
	*merge_result_end = entry;
	merge_result_end = &entry->next;
}

સ્થિર શૂન્ય trivial_merge_trees(સંરચના વૃક્ષ_desc t[3], અચલ અક્ષર *base);

સ્થિર અચલ અક્ષર *explanation(સંરચના merge_list *entry)
{
	પસંદગી (entry->stage) {
	સ્થિતિ 0:
		પાછા "merged";
	સ્થિતિ 3:
		પાછા "added in remote";
	સ્થિતિ 2:
		જો (entry->link)
			પાછા "added in both";
		પાછા "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	જો (!entry)
		પાછા "removed in both";

	જો (entry->link)
		પાછા "changed in both";

	જો (entry->stage == 3)
		પાછા "removed in local";
	પાછા "removed in remote";
}

સ્થિર શૂન્ય *result(સંરચના merge_list *entry, size_t *size)
{
	ગણના object_type type;
	સંરચના blob *base, *our, *their;
	અચલ અક્ષર *path = entry->path;

	જો (!entry->stage)
		પાછા odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       size);
	base = ખાલી;
	જો (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = ખાલી;
	જો (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = ખાલી;
	જો (entry)
		their = entry->blob;
	પાછા merge_blobs(the_repository->index, path,
			   base, our, their, size);
}

સ્થિર શૂન્ય *origin(સંરચના merge_list *entry, size_t *size)
{
	ગણના object_type type;
	જ્યારે (entry) {
		જો (entry->stage == 2)
			પાછા odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, size);
		entry = entry->link;
	}
	પાછા ખાલી;
}

સ્થિર પૂર્ણાંક show_outf(શૂન્ય *priv UNUSED, mmfile_t *mb, પૂર્ણાંક nbuf)
{
	પૂર્ણાંક i;
	માટે (i = 0; i < nbuf; i++)
		printf("%.*s", (પૂર્ણાંક) mb[i].size, mb[i].ptr);
	પાછા 0;
}

સ્થિર શૂન્ય show_diff(સંરચના merge_list *entry)
{
	size_t size;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_લીટી = show_outf };

	memset(&xpp, 0, કદ(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, કદ(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &size);
	જો (!src.ptr)
		size = 0;
	src.size = size;
	dst.ptr = result(entry, &size);
	જો (!dst.ptr)
		size = 0;
	dst.size = size;
	જો (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	મુક્ત(src.ptr);
	મુક્ત(dst.ptr);
}

સ્થિર શૂન્ય show_result_list(સંરચના merge_list *entry)
{
	printf("%s\n", explanation(entry));
	કરો {
		સંરચના merge_list *link = entry->link;
		સ્થિર અચલ અક્ષર *desc[4] = { "result", "base", "our", "their" };
		printf("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} જ્યારે (entry);
}

સ્થિર શૂન્ય show_result(શૂન્ય)
{
	સંરચના merge_list *walk;

	walk = merge_result;
	જ્યારે (walk) {
		show_result_list(walk);
		show_diff(walk);
		walk = walk->next;
	}
}

/* An empty entry never compares same, not even to another empty entry */
સ્થિર પૂર્ણાંક same_entry(સંરચના name_entry *a, સંરચના name_entry *b)
{
	પાછા	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

સ્થિર પૂર્ણાંક both_empty(સંરચના name_entry *a, સંરચના name_entry *b)
{
	પાછા is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

સ્થિર સંરચના merge_list *create_entry(unsigned stage, unsigned mode, અચલ સંરચના object_id *oid, અચલ અક્ષર *path)
{
	સંરચના merge_list *res = xcalloc(1, કદ(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	પાછા res;
}

સ્થિર અક્ષર *traverse_path(અચલ સંરચના traverse_info *info, અચલ સંરચના name_entry *n)
{
	સંરચના strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	પાછા strbuf_detach(&buf, ખાલી);
}

સ્થિર શૂન્ય resolve(અચલ સંરચના traverse_info *info, સંરચના name_entry *ours, સંરચના name_entry *result)
{
	સંરચના merge_list *orig, *final;
	અચલ અક્ષર *path;

	/* If it's already ours, don't bother showing it */
	જો (!ours)
		પાછા;

	path = traverse_path(info, result);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, result->mode, &result->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

સ્થિર શૂન્ય unresolved_directory(અચલ સંરચના traverse_info *info,
				 સંરચના name_entry n[3])
{
	સંરચના repository *r = the_repository;
	અક્ષર *newbase;
	સંરચના name_entry *p;
	સંરચના વૃક્ષ_desc t[3];
	શૂન્ય *buf0, *buf1, *buf2;

	માટે (p = n; p < n + 3; p++) {
		જો (p->mode && S_ISDIR(p->mode))
			તોડો;
	}
	જો (n + 3 <= p)
		પાછા; /* there is no tree here */

	newbase = traverse_path(info, p);

#define ENTRY_OID(e) (((e)->mode && S_ISDIR((e)->mode)) ? &(e)->oid : ખાલી)
	buf0 = fill_વૃક્ષ_descriptor(r, t + 0, ENTRY_OID(n + 0));
	buf1 = fill_વૃક્ષ_descriptor(r, t + 1, ENTRY_OID(n + 1));
	buf2 = fill_વૃક્ષ_descriptor(r, t + 2, ENTRY_OID(n + 2));
#undef ENTRY_OID

	trivial_merge_trees(t, newbase);

	મુક્ત(buf0);
	મુક્ત(buf1);
	મુક્ત(buf2);
	મુક્ત(newbase);
}


સ્થિર સંરચના merge_list *link_entry(unsigned stage, અચલ સંરચના traverse_info *info, સંરચના name_entry *n, સંરચના merge_list *entry)
{
	અચલ અક્ષર *path;
	સંરચના merge_list *link;

	જો (!n->mode)
		પાછા entry;
	જો (entry)
		path = entry->path;
	નહીંતર
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	પાછા link;
}

સ્થિર શૂન્ય unresolved(અચલ સંરચના traverse_info *info, સંરચના name_entry n[3])
{
	સંરચના merge_list *entry = ખાલી;
	પૂર્ણાંક i;
	unsigned dirmask = 0, mask = 0;

	માટે (i = 0; i < 3; i++) {
		mask |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		જો (!n[i].mode || S_ISDIR(n[i].mode))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	જો (dirmask == mask)
		પાછા;

	જો (n[2].mode && !S_ISDIR(n[2].mode))
		entry = link_entry(3, info, n + 2, entry);
	જો (n[1].mode && !S_ISDIR(n[1].mode))
		entry = link_entry(2, info, n + 1, entry);
	જો (n[0].mode && !S_ISDIR(n[0].mode))
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
સ્થિર પૂર્ણાંક threeway_callback(પૂર્ણાંક n UNUSED, unsigned long mask,
			     unsigned long dirmask UNUSED,
			     સંરચના name_entry *entry, સંરચના traverse_info *info)
{
	/* Same in both? */
	જો (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, ખાલી, entry+1);
		પાછા mask;
	}

	જો (same_entry(entry+0, entry+1)) {
		જો (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			પાછા mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	જો (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, ખાલી, entry+1);
		પાછા mask;
	}

	unresolved(info, entry);
	પાછા mask;
}

સ્થિર શૂન્ય trivial_merge_trees(સંરચના વૃક્ષ_desc t[3], અચલ અક્ષર *base)
{
	સંરચના traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->index, 3, t, &info);
}

સ્થિર શૂન્ય *get_વૃક્ષ_descriptor(સંરચના repository *r,
				 સંરચના વૃક્ષ_desc *desc,
				 અચલ અક્ષર *rev)
{
	સંરચના object_id oid;
	શૂન્ય *buf;

	જો (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_વૃક્ષ_descriptor(r, desc, &oid);
	જો (!buf)
		die("%s is not a tree", rev);
	પાછા buf;
}

સ્થિર પૂર્ણાંક trivial_merge(અચલ અક્ષર *base,
			 અચલ અક્ષર *branch1,
			 અચલ અક્ષર *branch2)
{
	સંરચના repository *r = the_repository;
	સંરચના વૃક્ષ_desc t[3];
	શૂન્ય *buf1, *buf2, *buf3;

	buf1 = get_વૃક્ષ_descriptor(r, t+0, base);
	buf2 = get_વૃક્ષ_descriptor(r, t+1, branch1);
	buf3 = get_વૃક્ષ_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	મુક્ત(buf1);
	મુક્ત(buf2);
	મુક્ત(buf3);

	show_result();
	પાછા 0;
}

ગણના mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

સંરચના merge_વૃક્ષ_options {
	પૂર્ણાંક mode;
	પૂર્ણાંક allow_unrelated_histories;
	પૂર્ણાંક show_messages;
	પૂર્ણાંક name_only;
	પૂર્ણાંક use_stdin;
	સંરચના merge_options merge_options;
};

સ્થિર પૂર્ણાંક real_merge(સંરચના merge_વૃક્ષ_options *o,
		      અચલ અક્ષર *merge_base,
		      અચલ અક્ષર *branch1, અચલ અક્ષર *branch2,
		      અચલ અક્ષર *prefix)
{
	સંરચના commit *parent1, *parent2;
	સંરચના commit_list *merge_bases = ખાલી;
	સંરચના merge_result result = { 0 };
	પૂર્ણાંક show_messages = o->show_messages;
	સંરચના merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	જો (merge_base) {
		સંરચના વૃક્ષ *base_વૃક્ષ, *parent1_વૃક્ષ, *parent2_વૃક્ષ;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		સંરચના object_id base_oid, head_oid, merge_oid;

		જો (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository,
						     &base_oid);
		જો (!base_વૃક્ષ)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		જો (repo_get_oid_treeish(the_repository, branch1, &head_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository,
							&head_oid);
		જો (!parent1_વૃક્ષ)
			die(_("unable to read tree (%s)"), oid_to_hex(&head_oid));
		જો (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository,
							&merge_oid);
		જો (!parent2_વૃક્ષ)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_વૃક્ષ, parent1_વૃક્ષ, parent2_વૃક્ષ, &result);
	} નહીંતર {
		parent1 = get_merge_parent(branch1);
		જો (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		જો (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		જો (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			exit(128);
		જો (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &result);
		commit_list_free(merge_bases);
	}

	જો (result.clean < 0)
		die(_("failure to merge"));

	જો (o->merge_options.mergeability_only)
		જાઓ cleanup;

	જો (show_messages == -1)
		show_messages = !result.clean;

	જો (o->use_stdin)
		printf("%d%c", result.clean, લીટી_termination);
	printf("%s%c", oid_to_hex(&result.વૃક્ષ->object.oid), લીટી_termination);
	જો (!result.clean) {
		સંરચના string_list conflicted_files = STRING_LIST_INIT_NODUP;
		અચલ અક્ષર *last = ખાલી;

		merge_get_conflicted_files(&result, &conflicted_files);
		માટે (size_t i = 0; i < conflicted_files.nr; i++) {
			અચલ અક્ષર *name = conflicted_files.વસ્તુઓ[i].string;
			સંરચના stage_info *c = conflicted_files.વસ્તુઓ[i].util;
			જો (!o->name_only)
				printf("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			નહીંતર જો (last && !strcmp(last, name))
				ચાલુ;
			write_name_quoted_relative(
				name, prefix, stdout, લીટી_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	જો (show_messages) {
		putchar(લીટી_termination);
		merge_display_update_messages(&opt, લીટી_termination == '\0',
					      &result);
	}
	જો (o->use_stdin)
		putchar(લીટી_termination);

cleanup:
	merge_finalize(&opt, &result);
	clear_merge_options(&opt);
	પાછા !result.clean; /* result.clean < 0 handled above */
}

પૂર્ણાંક cmd_merge_વૃક્ષ(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *prefix,
		   સંરચના repository *repo UNUSED)
{
	સંરચના merge_વૃક્ષ_options o = { .show_messages = -1 };
	સંરચના strvec xopts = STRVEC_INIT;
	પૂર્ણાંક expected_remaining_argc;
	પૂર્ણાંક original_argc;
	અચલ અક્ષર *merge_base = ખાલી;
	પૂર્ણાંક ret;
	પૂર્ણાંક quiet = 0;

	અચલ અક્ષર * અચલ merge_વૃક્ષ_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		ખાલી
	};
	સંરચના option mt_options[] = {
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
		OPT_SET_INT('z', ખાલી, &લીટી_termination,
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
			     merge_વૃક્ષ_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	જો (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !લીટી_termination, "-z");

	જો (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	માટે (size_t x = 0; x < xopts.nr; x++)
		જો (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	જો (o.use_stdin) {
		સંરચના strbuf buf = STRBUF_INIT;

		જો (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		જો (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		લીટી_termination = '\0';
		જ્યારે (strbuf_getline_lf(&buf, stdin) != EOF) {
			સંરચના string_list split = STRING_LIST_INIT_NODUP;
			અચલ અક્ષર *આગત_merge_base = ખાલી;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			જો (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			જો (!strcmp(split.વસ્તુઓ[1].string, "--")) {
				આગત_merge_base = split.વસ્તુઓ[0].string;
			}

			જો (આગત_merge_base && split.nr == 4) {
				real_merge(&o, આગત_merge_base,
					   split.વસ્તુઓ[2].string, split.વસ્તુઓ[3].string,
					   prefix);
			} નહીંતર જો (!આગત_merge_base && split.nr == 2) {
				real_merge(&o, ખાલી,
					   split.વસ્તુઓ[0].string, split.વસ્તુઓ[1].string,
					   prefix);
			} નહીંતર {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&split, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		જાઓ out;
	}

	/* Figure out which mode to use */
	પસંદગી (o.mode) {
	મૂળભૂત:
		BUG("unexpected command mode %d", o.mode);
	સ્થિતિ MODE_UNKNOWN:
		પસંદગી (argc) {
		મૂળભૂત:
			usage_with_options(merge_વૃક્ષ_usage, mt_options);
		સ્થિતિ 2:
			o.mode = MODE_REAL;
			તોડો;
		સ્થિતિ 3:
			o.mode = MODE_TRIVIAL;
			તોડો;
		}
		expected_remaining_argc = argc;
		તોડો;
	સ્થિતિ MODE_REAL:
		expected_remaining_argc = 2;
		તોડો;
	સ્થિતિ MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		તોડો;
	}
	જો (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	જો (argc != expected_remaining_argc)
		usage_with_options(merge_વૃક્ષ_usage, mt_options);

	repo_config(the_repository, git_default_config, ખાલી);

	/* Do the relevant type of merge */
	જો (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	નહીંતર
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	પાછા ret;
}
