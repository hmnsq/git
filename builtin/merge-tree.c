#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "tree-walk.h"
#அந்தர்பாவ "xdiff-interface.h"
#அந்தர்பாவ "help.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "merge-ort.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "blob.h"
#அந்தர்பாவ "merge-blobs.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "write-or-die.h"

நிலையான முழு வரி_termination = '\n';

கட்டமைப்பு merge_list {
	கட்டமைப்பு merge_list *next;
	கட்டமைப்பு merge_list *link;	/* other stages for this object */

	unsigned முழு stage : 2;
	unsigned முழு mode;
	மாறிலி எழுத்து *path;
	கட்டமைப்பு blob *blob;
};

நிலையான கட்டமைப்பு merge_list *merge_result, **merge_result_end = &merge_result;

நிலையான வெறுமை add_merge_entry(கட்டமைப்பு merge_list *entry)
{
	*merge_result_end = entry;
	merge_result_end = &entry->next;
}

நிலையான வெறுமை trivial_merge_trees(கட்டமைப்பு மரம்_desc t[3], மாறிலி எழுத்து *base);

நிலையான மாறிலி எழுத்து *explanation(கட்டமைப்பு merge_list *entry)
{
	தேர்வு (entry->stage) {
	நிலை 0:
		திரும்பு "merged";
	நிலை 3:
		திரும்பு "added in remote";
	நிலை 2:
		எனில் (entry->link)
			திரும்பு "added in both";
		திரும்பு "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	எனில் (!entry)
		திரும்பு "removed in both";

	எனில் (entry->link)
		திரும்பு "changed in both";

	எனில் (entry->stage == 3)
		திரும்பு "removed in local";
	திரும்பு "removed in remote";
}

நிலையான வெறுமை *result(கட்டமைப்பு merge_list *entry, size_t *size)
{
	எண்ணிக்கை object_type type;
	கட்டமைப்பு blob *base, *our, *their;
	மாறிலி எழுத்து *path = entry->path;

	எனில் (!entry->stage)
		திரும்பு odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       size);
	base = வெற்று;
	எனில் (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = வெற்று;
	எனில் (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = வெற்று;
	எனில் (entry)
		their = entry->blob;
	திரும்பு merge_blobs(the_repository->index, path,
			   base, our, their, size);
}

நிலையான வெறுமை *origin(கட்டமைப்பு merge_list *entry, size_t *size)
{
	எண்ணிக்கை object_type type;
	போது (entry) {
		எனில் (entry->stage == 2)
			திரும்பு odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, size);
		entry = entry->link;
	}
	திரும்பு வெற்று;
}

நிலையான முழு show_outf(வெறுமை *priv UNUSED, mmfile_t *mb, முழு nbuf)
{
	முழு i;
	க்கு (i = 0; i < nbuf; i++)
		printf("%.*s", (முழு) mb[i].size, mb[i].ptr);
	திரும்பு 0;
}

நிலையான வெறுமை show_diff(கட்டமைப்பு merge_list *entry)
{
	size_t size;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_வரி = show_outf };

	memset(&xpp, 0, அளவு(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, அளவு(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &size);
	எனில் (!src.ptr)
		size = 0;
	src.size = size;
	dst.ptr = result(entry, &size);
	எனில் (!dst.ptr)
		size = 0;
	dst.size = size;
	எனில் (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	விடுவி(src.ptr);
	விடுவி(dst.ptr);
}

நிலையான வெறுமை show_result_list(கட்டமைப்பு merge_list *entry)
{
	printf("%s\n", explanation(entry));
	செய் {
		கட்டமைப்பு merge_list *link = entry->link;
		நிலையான மாறிலி எழுத்து *desc[4] = { "result", "base", "our", "their" };
		printf("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} போது (entry);
}

நிலையான வெறுமை show_result(வெறுமை)
{
	கட்டமைப்பு merge_list *walk;

	walk = merge_result;
	போது (walk) {
		show_result_list(walk);
		show_diff(walk);
		walk = walk->next;
	}
}

/* An empty entry never compares same, not even to another empty entry */
நிலையான முழு same_entry(கட்டமைப்பு name_entry *a, கட்டமைப்பு name_entry *b)
{
	திரும்பு	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

நிலையான முழு both_empty(கட்டமைப்பு name_entry *a, கட்டமைப்பு name_entry *b)
{
	திரும்பு is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

நிலையான கட்டமைப்பு merge_list *create_entry(unsigned stage, unsigned mode, மாறிலி கட்டமைப்பு object_id *oid, மாறிலி எழுத்து *path)
{
	கட்டமைப்பு merge_list *res = xcalloc(1, அளவு(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	திரும்பு res;
}

நிலையான எழுத்து *traverse_path(மாறிலி கட்டமைப்பு traverse_info *info, மாறிலி கட்டமைப்பு name_entry *n)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	திரும்பு strbuf_detach(&buf, வெற்று);
}

நிலையான வெறுமை resolve(மாறிலி கட்டமைப்பு traverse_info *info, கட்டமைப்பு name_entry *ours, கட்டமைப்பு name_entry *result)
{
	கட்டமைப்பு merge_list *orig, *final;
	மாறிலி எழுத்து *path;

	/* If it's already ours, don't bother showing it */
	எனில் (!ours)
		திரும்பு;

	path = traverse_path(info, result);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, result->mode, &result->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

நிலையான வெறுமை unresolved_directory(மாறிலி கட்டமைப்பு traverse_info *info,
				 கட்டமைப்பு name_entry n[3])
{
	கட்டமைப்பு repository *r = the_repository;
	எழுத்து *newbase;
	கட்டமைப்பு name_entry *p;
	கட்டமைப்பு மரம்_desc t[3];
	வெறுமை *buf0, *buf1, *buf2;

	க்கு (p = n; p < n + 3; p++) {
		எனில் (p->mode && S_ISDIR(p->mode))
			நிறுத்து;
	}
	எனில் (n + 3 <= p)
		திரும்பு; /* there is no tree here */

	newbase = traverse_path(info, p);

#define ENTRY_OID(e) (((e)->mode && S_ISDIR((e)->mode)) ? &(e)->oid : வெற்று)
	buf0 = fill_மரம்_descriptor(r, t + 0, ENTRY_OID(n + 0));
	buf1 = fill_மரம்_descriptor(r, t + 1, ENTRY_OID(n + 1));
	buf2 = fill_மரம்_descriptor(r, t + 2, ENTRY_OID(n + 2));
#undef ENTRY_OID

	trivial_merge_trees(t, newbase);

	விடுவி(buf0);
	விடுவி(buf1);
	விடுவி(buf2);
	விடுவி(newbase);
}


நிலையான கட்டமைப்பு merge_list *link_entry(unsigned stage, மாறிலி கட்டமைப்பு traverse_info *info, கட்டமைப்பு name_entry *n, கட்டமைப்பு merge_list *entry)
{
	மாறிலி எழுத்து *path;
	கட்டமைப்பு merge_list *link;

	எனில் (!n->mode)
		திரும்பு entry;
	எனில் (entry)
		path = entry->path;
	இல்லையெனில்
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	திரும்பு link;
}

நிலையான வெறுமை unresolved(மாறிலி கட்டமைப்பு traverse_info *info, கட்டமைப்பு name_entry n[3])
{
	கட்டமைப்பு merge_list *entry = வெற்று;
	முழு i;
	unsigned dirmask = 0, mask = 0;

	க்கு (i = 0; i < 3; i++) {
		mask |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		எனில் (!n[i].mode || S_ISDIR(n[i].mode))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	எனில் (dirmask == mask)
		திரும்பு;

	எனில் (n[2].mode && !S_ISDIR(n[2].mode))
		entry = link_entry(3, info, n + 2, entry);
	எனில் (n[1].mode && !S_ISDIR(n[1].mode))
		entry = link_entry(2, info, n + 1, entry);
	எனில் (n[0].mode && !S_ISDIR(n[0].mode))
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
நிலையான முழு threeway_callback(முழு n UNUSED, unsigned long mask,
			     unsigned long dirmask UNUSED,
			     கட்டமைப்பு name_entry *entry, கட்டமைப்பு traverse_info *info)
{
	/* Same in both? */
	எனில் (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, வெற்று, entry+1);
		திரும்பு mask;
	}

	எனில் (same_entry(entry+0, entry+1)) {
		எனில் (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			திரும்பு mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	எனில் (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, வெற்று, entry+1);
		திரும்பு mask;
	}

	unresolved(info, entry);
	திரும்பு mask;
}

நிலையான வெறுமை trivial_merge_trees(கட்டமைப்பு மரம்_desc t[3], மாறிலி எழுத்து *base)
{
	கட்டமைப்பு traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->index, 3, t, &info);
}

நிலையான வெறுமை *get_மரம்_descriptor(கட்டமைப்பு repository *r,
				 கட்டமைப்பு மரம்_desc *desc,
				 மாறிலி எழுத்து *rev)
{
	கட்டமைப்பு object_id oid;
	வெறுமை *buf;

	எனில் (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_மரம்_descriptor(r, desc, &oid);
	எனில் (!buf)
		die("%s is not a tree", rev);
	திரும்பு buf;
}

நிலையான முழு trivial_merge(மாறிலி எழுத்து *base,
			 மாறிலி எழுத்து *branch1,
			 மாறிலி எழுத்து *branch2)
{
	கட்டமைப்பு repository *r = the_repository;
	கட்டமைப்பு மரம்_desc t[3];
	வெறுமை *buf1, *buf2, *buf3;

	buf1 = get_மரம்_descriptor(r, t+0, base);
	buf2 = get_மரம்_descriptor(r, t+1, branch1);
	buf3 = get_மரம்_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	விடுவி(buf1);
	விடுவி(buf2);
	விடுவி(buf3);

	show_result();
	திரும்பு 0;
}

எண்ணிக்கை mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

கட்டமைப்பு merge_மரம்_options {
	முழு mode;
	முழு allow_unrelated_histories;
	முழு show_messages;
	முழு name_only;
	முழு use_stdin;
	கட்டமைப்பு merge_options merge_options;
};

நிலையான முழு real_merge(கட்டமைப்பு merge_மரம்_options *o,
		      மாறிலி எழுத்து *merge_base,
		      மாறிலி எழுத்து *branch1, மாறிலி எழுத்து *branch2,
		      மாறிலி எழுத்து *prefix)
{
	கட்டமைப்பு commit *parent1, *parent2;
	கட்டமைப்பு commit_list *merge_bases = வெற்று;
	கட்டமைப்பு merge_result result = { 0 };
	முழு show_messages = o->show_messages;
	கட்டமைப்பு merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	எனில் (merge_base) {
		கட்டமைப்பு மரம் *base_மரம், *parent1_மரம், *parent2_மரம்;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		கட்டமைப்பு object_id base_oid, head_oid, merge_oid;

		எனில் (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_மரம் = repo_parse_மரம்_indirect(the_repository,
						     &base_oid);
		எனில் (!base_மரம்)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		எனில் (repo_get_oid_treeish(the_repository, branch1, &head_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_மரம் = repo_parse_மரம்_indirect(the_repository,
							&head_oid);
		எனில் (!parent1_மரம்)
			die(_("unable to read tree (%s)"), oid_to_hex(&head_oid));
		எனில் (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_மரம் = repo_parse_மரம்_indirect(the_repository,
							&merge_oid);
		எனில் (!parent2_மரம்)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_மரம், parent1_மரம், parent2_மரம், &result);
	} இல்லையெனில் {
		parent1 = get_merge_parent(branch1);
		எனில் (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		எனில் (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		எனில் (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			exit(128);
		எனில் (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &result);
		commit_list_free(merge_bases);
	}

	எனில் (result.clean < 0)
		die(_("failure to merge"));

	எனில் (o->merge_options.mergeability_only)
		செல் cleanup;

	எனில் (show_messages == -1)
		show_messages = !result.clean;

	எனில் (o->use_stdin)
		printf("%d%c", result.clean, வரி_termination);
	printf("%s%c", oid_to_hex(&result.மரம்->object.oid), வரி_termination);
	எனில் (!result.clean) {
		கட்டமைப்பு string_list conflicted_files = STRING_LIST_INIT_NODUP;
		மாறிலி எழுத்து *last = வெற்று;

		merge_get_conflicted_files(&result, &conflicted_files);
		க்கு (size_t i = 0; i < conflicted_files.nr; i++) {
			மாறிலி எழுத்து *name = conflicted_files.பொருட்கள்[i].string;
			கட்டமைப்பு stage_info *c = conflicted_files.பொருட்கள்[i].util;
			எனில் (!o->name_only)
				printf("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			இல்லையெனில் எனில் (last && !strcmp(last, name))
				தொடர்;
			write_name_quoted_relative(
				name, prefix, stdout, வரி_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	எனில் (show_messages) {
		putchar(வரி_termination);
		merge_display_update_messages(&opt, வரி_termination == '\0',
					      &result);
	}
	எனில் (o->use_stdin)
		putchar(வரி_termination);

cleanup:
	merge_finalize(&opt, &result);
	clear_merge_options(&opt);
	திரும்பு !result.clean; /* result.clean < 0 handled above */
}

முழு cmd_merge_மரம்(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு merge_மரம்_options o = { .show_messages = -1 };
	கட்டமைப்பு strvec xopts = STRVEC_INIT;
	முழு expected_remaining_argc;
	முழு original_argc;
	மாறிலி எழுத்து *merge_base = வெற்று;
	முழு ret;
	முழு quiet = 0;

	மாறிலி எழுத்து * மாறிலி merge_மரம்_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		வெற்று
	};
	கட்டமைப்பு option mt_options[] = {
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
		OPT_SET_INT('z', வெற்று, &வரி_termination,
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
			     merge_மரம்_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	எனில் (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !வரி_termination, "-z");

	எனில் (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	க்கு (size_t x = 0; x < xopts.nr; x++)
		எனில் (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	எனில் (o.use_stdin) {
		கட்டமைப்பு strbuf buf = STRBUF_INIT;

		எனில் (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		எனில் (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		வரி_termination = '\0';
		போது (strbuf_getline_lf(&buf, stdin) != EOF) {
			கட்டமைப்பு string_list split = STRING_LIST_INIT_NODUP;
			மாறிலி எழுத்து *உள்ளீடு_merge_base = வெற்று;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			எனில் (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			எனில் (!strcmp(split.பொருட்கள்[1].string, "--")) {
				உள்ளீடு_merge_base = split.பொருட்கள்[0].string;
			}

			எனில் (உள்ளீடு_merge_base && split.nr == 4) {
				real_merge(&o, உள்ளீடு_merge_base,
					   split.பொருட்கள்[2].string, split.பொருட்கள்[3].string,
					   prefix);
			} இல்லையெனில் எனில் (!உள்ளீடு_merge_base && split.nr == 2) {
				real_merge(&o, வெற்று,
					   split.பொருட்கள்[0].string, split.பொருட்கள்[1].string,
					   prefix);
			} இல்லையெனில் {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&split, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		செல் out;
	}

	/* Figure out which mode to use */
	தேர்வு (o.mode) {
	இயல்பு:
		BUG("unexpected command mode %d", o.mode);
	நிலை MODE_UNKNOWN:
		தேர்வு (argc) {
		இயல்பு:
			usage_with_options(merge_மரம்_usage, mt_options);
		நிலை 2:
			o.mode = MODE_REAL;
			நிறுத்து;
		நிலை 3:
			o.mode = MODE_TRIVIAL;
			நிறுத்து;
		}
		expected_remaining_argc = argc;
		நிறுத்து;
	நிலை MODE_REAL:
		expected_remaining_argc = 2;
		நிறுத்து;
	நிலை MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		நிறுத்து;
	}
	எனில் (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	எனில் (argc != expected_remaining_argc)
		usage_with_options(merge_மரம்_usage, mt_options);

	repo_config(the_repository, git_default_config, வெற்று);

	/* Do the relevant type of merge */
	எனில் (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	இல்லையெனில்
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	திரும்பு ret;
}
