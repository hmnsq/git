#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "tree-walk.h"
#ଅନ୍ତର୍ଭାଵ "xdiff-interface.h"
#ଅନ୍ତର୍ଭାଵ "help.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"
#ଅନ୍ତର୍ଭାଵ "merge-ort.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "blob.h"
#ଅନ୍ତର୍ଭାଵ "merge-blobs.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_termination = '\n';

ସଂରଚନା merge_list {
	ସଂରଚନା merge_list *next;
	ସଂରଚନା merge_list *link;	/* other stages for this object */

	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stage : 2;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;
	ଅଚଳ ଅକ୍ଷର *path;
	ସଂରଚନା blob *blob;
};

ସ୍ଥିର ସଂରଚନା merge_list *merge_result, **merge_result_end = &merge_result;

ସ୍ଥିର ଶୂନ୍ୟ add_merge_entry(ସଂରଚନା merge_list *entry)
{
	*merge_result_end = entry;
	merge_result_end = &entry->next;
}

ସ୍ଥିର ଶୂନ୍ୟ trivial_merge_trees(ସଂରଚନା ବୃକ୍ଷ_desc t[3], ଅଚଳ ଅକ୍ଷର *base);

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *explanation(ସଂରଚନା merge_list *entry)
{
	ବାଛ (entry->stage) {
	ସ୍ଥିତି 0:
		ଫେରନ୍ତୁ "merged";
	ସ୍ଥିତି 3:
		ଫେରନ୍ତୁ "added in remote";
	ସ୍ଥିତି 2:
		ଯଦି (entry->link)
			ଫେରନ୍ତୁ "added in both";
		ଫେରନ୍ତୁ "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	ଯଦି (!entry)
		ଫେରନ୍ତୁ "removed in both";

	ଯଦି (entry->link)
		ଫେରନ୍ତୁ "changed in both";

	ଯଦି (entry->stage == 3)
		ଫେରନ୍ତୁ "removed in local";
	ଫେରନ୍ତୁ "removed in remote";
}

ସ୍ଥିର ଶୂନ୍ୟ *result(ସଂରଚନା merge_list *entry, size_t *size)
{
	ଗଣନା object_type type;
	ସଂରଚନା blob *base, *our, *their;
	ଅଚଳ ଅକ୍ଷର *path = entry->path;

	ଯଦି (!entry->stage)
		ଫେରନ୍ତୁ odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       size);
	base = ଖାଲି;
	ଯଦି (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = ଖାଲି;
	ଯଦି (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = ଖାଲି;
	ଯଦି (entry)
		their = entry->blob;
	ଫେରନ୍ତୁ merge_blobs(the_repository->index, path,
			   base, our, their, size);
}

ସ୍ଥିର ଶୂନ୍ୟ *origin(ସଂରଚନା merge_list *entry, size_t *size)
{
	ଗଣନା object_type type;
	ଯେତେବେଳେ (entry) {
		ଯଦି (entry->stage == 2)
			ଫେରନ୍ତୁ odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, size);
		entry = entry->link;
	}
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_outf(ଶୂନ୍ୟ *priv UNUSED, mmfile_t *mb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nbuf)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପାଇଁ (i = 0; i < nbuf; i++)
		printf("%.*s", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mb[i].size, mb[i].ptr);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ show_diff(ସଂରଚନା merge_list *entry)
{
	size_t size;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_ଧାଡ଼ି = show_outf };

	memset(&xpp, 0, ଆକାର(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, ଆକାର(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &size);
	ଯଦି (!src.ptr)
		size = 0;
	src.size = size;
	dst.ptr = result(entry, &size);
	ଯଦି (!dst.ptr)
		size = 0;
	dst.size = size;
	ଯଦି (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	ମୁକ୍ତ(src.ptr);
	ମୁକ୍ତ(dst.ptr);
}

ସ୍ଥିର ଶୂନ୍ୟ show_result_list(ସଂରଚନା merge_list *entry)
{
	printf("%s\n", explanation(entry));
	କର {
		ସଂରଚନା merge_list *link = entry->link;
		ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *desc[4] = { "result", "base", "our", "their" };
		printf("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} ଯେତେବେଳେ (entry);
}

ସ୍ଥିର ଶୂନ୍ୟ show_result(ଶୂନ୍ୟ)
{
	ସଂରଚନା merge_list *walk;

	walk = merge_result;
	ଯେତେବେଳେ (walk) {
		show_result_list(walk);
		show_diff(walk);
		walk = walk->next;
	}
}

/* An empty entry never compares same, not even to another empty entry */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା same_entry(ସଂରଚନା name_entry *a, ସଂରଚନା name_entry *b)
{
	ଫେରନ୍ତୁ	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା both_empty(ସଂରଚନା name_entry *a, ସଂରଚନା name_entry *b)
{
	ଫେରନ୍ତୁ is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

ସ୍ଥିର ସଂରଚନା merge_list *create_entry(unsigned stage, unsigned mode, ଅଚଳ ସଂରଚନା object_id *oid, ଅଚଳ ଅକ୍ଷର *path)
{
	ସଂରଚନା merge_list *res = xcalloc(1, ଆକାର(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଅକ୍ଷର *traverse_path(ଅଚଳ ସଂରଚନା traverse_info *info, ଅଚଳ ସଂରଚନା name_entry *n)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	ଫେରନ୍ତୁ strbuf_detach(&buf, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ resolve(ଅଚଳ ସଂରଚନା traverse_info *info, ସଂରଚନା name_entry *ours, ସଂରଚନା name_entry *result)
{
	ସଂରଚନା merge_list *orig, *final;
	ଅଚଳ ଅକ୍ଷର *path;

	/* If it's already ours, don't bother showing it */
	ଯଦି (!ours)
		ଫେରନ୍ତୁ;

	path = traverse_path(info, result);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, result->mode, &result->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

ସ୍ଥିର ଶୂନ୍ୟ unresolved_directory(ଅଚଳ ସଂରଚନା traverse_info *info,
				 ସଂରଚନା name_entry n[3])
{
	ସଂରଚନା repository *r = the_repository;
	ଅକ୍ଷର *newbase;
	ସଂରଚନା name_entry *p;
	ସଂରଚନା ବୃକ୍ଷ_desc t[3];
	ଶୂନ୍ୟ *buf0, *buf1, *buf2;

	ପାଇଁ (p = n; p < n + 3; p++) {
		ଯଦି (p->mode && S_ISDIR(p->mode))
			ଭାଙ୍ଗ;
	}
	ଯଦି (n + 3 <= p)
		ଫେରନ୍ତୁ; /* there is no tree here */

	newbase = traverse_path(info, p);

#define ENTRY_OID(e) (((e)->mode && S_ISDIR((e)->mode)) ? &(e)->oid : ଖାଲି)
	buf0 = fill_ବୃକ୍ଷ_descriptor(r, t + 0, ENTRY_OID(n + 0));
	buf1 = fill_ବୃକ୍ଷ_descriptor(r, t + 1, ENTRY_OID(n + 1));
	buf2 = fill_ବୃକ୍ଷ_descriptor(r, t + 2, ENTRY_OID(n + 2));
#undef ENTRY_OID

	trivial_merge_trees(t, newbase);

	ମୁକ୍ତ(buf0);
	ମୁକ୍ତ(buf1);
	ମୁକ୍ତ(buf2);
	ମୁକ୍ତ(newbase);
}


ସ୍ଥିର ସଂରଚନା merge_list *link_entry(unsigned stage, ଅଚଳ ସଂରଚନା traverse_info *info, ସଂରଚନା name_entry *n, ସଂରଚନା merge_list *entry)
{
	ଅଚଳ ଅକ୍ଷର *path;
	ସଂରଚନା merge_list *link;

	ଯଦି (!n->mode)
		ଫେରନ୍ତୁ entry;
	ଯଦି (entry)
		path = entry->path;
	ଅନ୍ୟଥା
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	ଫେରନ୍ତୁ link;
}

ସ୍ଥିର ଶୂନ୍ୟ unresolved(ଅଚଳ ସଂରଚନା traverse_info *info, ସଂରଚନା name_entry n[3])
{
	ସଂରଚନା merge_list *entry = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	unsigned dirmask = 0, mask = 0;

	ପାଇଁ (i = 0; i < 3; i++) {
		mask |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		ଯଦି (!n[i].mode || S_ISDIR(n[i].mode))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	ଯଦି (dirmask == mask)
		ଫେରନ୍ତୁ;

	ଯଦି (n[2].mode && !S_ISDIR(n[2].mode))
		entry = link_entry(3, info, n + 2, entry);
	ଯଦି (n[1].mode && !S_ISDIR(n[1].mode))
		entry = link_entry(2, info, n + 1, entry);
	ଯଦି (n[0].mode && !S_ISDIR(n[0].mode))
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା threeway_callback(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n UNUSED, unsigned long mask,
			     unsigned long dirmask UNUSED,
			     ସଂରଚନା name_entry *entry, ସଂରଚନା traverse_info *info)
{
	/* Same in both? */
	ଯଦି (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, ଖାଲି, entry+1);
		ଫେରନ୍ତୁ mask;
	}

	ଯଦି (same_entry(entry+0, entry+1)) {
		ଯଦି (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			ଫେରନ୍ତୁ mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	ଯଦି (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, ଖାଲି, entry+1);
		ଫେରନ୍ତୁ mask;
	}

	unresolved(info, entry);
	ଫେରନ୍ତୁ mask;
}

ସ୍ଥିର ଶୂନ୍ୟ trivial_merge_trees(ସଂରଚନା ବୃକ୍ଷ_desc t[3], ଅଚଳ ଅକ୍ଷର *base)
{
	ସଂରଚନା traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->index, 3, t, &info);
}

ସ୍ଥିର ଶୂନ୍ୟ *get_ବୃକ୍ଷ_descriptor(ସଂରଚନା repository *r,
				 ସଂରଚନା ବୃକ୍ଷ_desc *desc,
				 ଅଚଳ ଅକ୍ଷର *rev)
{
	ସଂରଚନା object_id oid;
	ଶୂନ୍ୟ *buf;

	ଯଦି (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_ବୃକ୍ଷ_descriptor(r, desc, &oid);
	ଯଦି (!buf)
		die("%s is not a tree", rev);
	ଫେରନ୍ତୁ buf;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା trivial_merge(ଅଚଳ ଅକ୍ଷର *base,
			 ଅଚଳ ଅକ୍ଷର *branch1,
			 ଅଚଳ ଅକ୍ଷର *branch2)
{
	ସଂରଚନା repository *r = the_repository;
	ସଂରଚନା ବୃକ୍ଷ_desc t[3];
	ଶୂନ୍ୟ *buf1, *buf2, *buf3;

	buf1 = get_ବୃକ୍ଷ_descriptor(r, t+0, base);
	buf2 = get_ବୃକ୍ଷ_descriptor(r, t+1, branch1);
	buf3 = get_ବୃକ୍ଷ_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	ମୁକ୍ତ(buf1);
	ମୁକ୍ତ(buf2);
	ମୁକ୍ତ(buf3);

	show_result();
	ଫେରନ୍ତୁ 0;
}

ଗଣନା mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

ସଂରଚନା merge_ବୃକ୍ଷ_options {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_unrelated_histories;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_messages;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_only;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_stdin;
	ସଂରଚନା merge_options merge_options;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା real_merge(ସଂରଚନା merge_ବୃକ୍ଷ_options *o,
		      ଅଚଳ ଅକ୍ଷର *merge_base,
		      ଅଚଳ ଅକ୍ଷର *branch1, ଅଚଳ ଅକ୍ଷର *branch2,
		      ଅଚଳ ଅକ୍ଷର *prefix)
{
	ସଂରଚନା commit *parent1, *parent2;
	ସଂରଚନା commit_list *merge_bases = ଖାଲି;
	ସଂରଚନା merge_result result = { 0 };
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_messages = o->show_messages;
	ସଂରଚନା merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	ଯଦି (merge_base) {
		ସଂରଚନା ବୃକ୍ଷ *base_ବୃକ୍ଷ, *parent1_ବୃକ୍ଷ, *parent2_ବୃକ୍ଷ;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		ସଂରଚନା object_id base_oid, head_oid, merge_oid;

		ଯଦି (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository,
						     &base_oid);
		ଯଦି (!base_ବୃକ୍ଷ)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		ଯଦି (repo_get_oid_treeish(the_repository, branch1, &head_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository,
							&head_oid);
		ଯଦି (!parent1_ବୃକ୍ଷ)
			die(_("unable to read tree (%s)"), oid_to_hex(&head_oid));
		ଯଦି (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository,
							&merge_oid);
		ଯଦି (!parent2_ବୃକ୍ଷ)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_ବୃକ୍ଷ, parent1_ବୃକ୍ଷ, parent2_ବୃକ୍ଷ, &result);
	} ଅନ୍ୟଥା {
		parent1 = get_merge_parent(branch1);
		ଯଦି (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		ଯଦି (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		ଯଦି (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			exit(128);
		ଯଦି (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &result);
		commit_list_free(merge_bases);
	}

	ଯଦି (result.clean < 0)
		die(_("failure to merge"));

	ଯଦି (o->merge_options.mergeability_only)
		ଯାଅ cleanup;

	ଯଦି (show_messages == -1)
		show_messages = !result.clean;

	ଯଦି (o->use_stdin)
		printf("%d%c", result.clean, ଧାଡ଼ି_termination);
	printf("%s%c", oid_to_hex(&result.ବୃକ୍ଷ->object.oid), ଧାଡ଼ି_termination);
	ଯଦି (!result.clean) {
		ସଂରଚନା string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ଅଚଳ ଅକ୍ଷର *last = ଖାଲି;

		merge_get_conflicted_files(&result, &conflicted_files);
		ପାଇଁ (size_t i = 0; i < conflicted_files.nr; i++) {
			ଅଚଳ ଅକ୍ଷର *name = conflicted_files.ବସ୍ତୁଗୁଡ଼ିକ[i].string;
			ସଂରଚନା stage_info *c = conflicted_files.ବସ୍ତୁଗୁଡ଼ିକ[i].util;
			ଯଦି (!o->name_only)
				printf("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			ଅନ୍ୟଥା ଯଦି (last && !strcmp(last, name))
				ଜାରି;
			write_name_quoted_relative(
				name, prefix, stdout, ଧାଡ଼ି_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	ଯଦି (show_messages) {
		putchar(ଧାଡ଼ି_termination);
		merge_display_update_messages(&opt, ଧାଡ଼ି_termination == '\0',
					      &result);
	}
	ଯଦି (o->use_stdin)
		putchar(ଧାଡ଼ି_termination);

cleanup:
	merge_finalize(&opt, &result);
	clear_merge_options(&opt);
	ଫେରନ୍ତୁ !result.clean; /* result.clean < 0 handled above */
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_merge_ବୃକ୍ଷ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		   ଅଚଳ ଅକ୍ଷର **argv,
		   ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା merge_ବୃକ୍ଷ_options o = { .show_messages = -1 };
	ସଂରଚନା strvec xopts = STRVEC_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା expected_remaining_argc;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା original_argc;
	ଅଚଳ ଅକ୍ଷର *merge_base = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;

	ଅଚଳ ଅକ୍ଷର * ଅଚଳ merge_ବୃକ୍ଷ_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		ଖାଲି
	};
	ସଂରଚନା option mt_options[] = {
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
		OPT_SET_INT('z', ଖାଲି, &ଧାଡ଼ି_termination,
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
			     merge_ବୃକ୍ଷ_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	ଯଦି (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !ଧାଡ଼ି_termination, "-z");

	ଯଦି (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	ପାଇଁ (size_t x = 0; x < xopts.nr; x++)
		ଯଦି (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	ଯଦି (o.use_stdin) {
		ସଂରଚନା strbuf buf = STRBUF_INIT;

		ଯଦି (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		ଯଦି (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		ଧାଡ଼ି_termination = '\0';
		ଯେତେବେଳେ (strbuf_getline_lf(&buf, stdin) != EOF) {
			ସଂରଚନା string_list split = STRING_LIST_INIT_NODUP;
			ଅଚଳ ଅକ୍ଷର *ନିବେଶ_merge_base = ଖାଲି;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			ଯଦି (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			ଯଦି (!strcmp(split.ବସ୍ତୁଗୁଡ଼ିକ[1].string, "--")) {
				ନିବେଶ_merge_base = split.ବସ୍ତୁଗୁଡ଼ିକ[0].string;
			}

			ଯଦି (ନିବେଶ_merge_base && split.nr == 4) {
				real_merge(&o, ନିବେଶ_merge_base,
					   split.ବସ୍ତୁଗୁଡ଼ିକ[2].string, split.ବସ୍ତୁଗୁଡ଼ିକ[3].string,
					   prefix);
			} ଅନ୍ୟଥା ଯଦି (!ନିବେଶ_merge_base && split.nr == 2) {
				real_merge(&o, ଖାଲି,
					   split.ବସ୍ତୁଗୁଡ଼ିକ[0].string, split.ବସ୍ତୁଗୁଡ଼ିକ[1].string,
					   prefix);
			} ଅନ୍ୟଥା {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&split, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		ଯାଅ out;
	}

	/* Figure out which mode to use */
	ବାଛ (o.mode) {
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unexpected command mode %d", o.mode);
	ସ୍ଥିତି MODE_UNKNOWN:
		ବାଛ (argc) {
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			usage_with_options(merge_ବୃକ୍ଷ_usage, mt_options);
		ସ୍ଥିତି 2:
			o.mode = MODE_REAL;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 3:
			o.mode = MODE_TRIVIAL;
			ଭାଙ୍ଗ;
		}
		expected_remaining_argc = argc;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି MODE_REAL:
		expected_remaining_argc = 2;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		ଭାଙ୍ଗ;
	}
	ଯଦି (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	ଯଦି (argc != expected_remaining_argc)
		usage_with_options(merge_ବୃକ୍ଷ_usage, mt_options);

	repo_config(the_repository, git_default_config, ଖାଲି);

	/* Do the relevant type of merge */
	ଯଦି (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	ଅନ୍ୟଥା
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	ଫେରନ୍ତୁ ret;
}
