#define USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "xdiff-interface.h"
#ಅನ್ತರ್ಭಾವ "help.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "merge-ort.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "blob.h"
#ಅನ್ತರ್ಭಾವ "merge-blobs.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_termination = '\n';

ಸಂರಚನಾ merge_list {
	ಸಂರಚನಾ merge_list *next;
	ಸಂರಚನಾ merge_list *link;	/* other stages for this object */

	unsigned ಪೂರ್ಣಾಙ್ಕ stage : 2;
	unsigned ಪೂರ್ಣಾಙ್ಕ mode;
	ಧ್ರುವ ವರ್ಣ *path;
	ಸಂರಚನಾ blob *blob;
};

ಸ್ಥಿರ ಸಂರಚನಾ merge_list *merge_result, **merge_result_end = &merge_result;

ಸ್ಥಿರ ಶೂನ್ಯ add_merge_entry(ಸಂರಚನಾ merge_list *entry)
{
	*merge_result_end = entry;
	merge_result_end = &entry->next;
}

ಸ್ಥಿರ ಶೂನ್ಯ trivial_merge_trees(ಸಂರಚನಾ ವೃಕ್ಷ_desc t[3], ಧ್ರುವ ವರ್ಣ *base);

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *explanation(ಸಂರಚನಾ merge_list *entry)
{
	ವಿಕಲ್ಪ (entry->stage) {
	ಪಕ್ಷ 0:
		ಪ್ರತ್ಯಾಗಮ "merged";
	ಪಕ್ಷ 3:
		ಪ್ರತ್ಯಾಗಮ "added in remote";
	ಪಕ್ಷ 2:
		ಯದಿ (entry->link)
			ಪ್ರತ್ಯಾಗಮ "added in both";
		ಪ್ರತ್ಯಾಗಮ "added in local";
	}

	/* Existed in base */
	entry = entry->link;
	ಯದಿ (!entry)
		ಪ್ರತ್ಯಾಗಮ "removed in both";

	ಯದಿ (entry->link)
		ಪ್ರತ್ಯಾಗಮ "changed in both";

	ಯದಿ (entry->stage == 3)
		ಪ್ರತ್ಯಾಗಮ "removed in local";
	ಪ್ರತ್ಯಾಗಮ "removed in remote";
}

ಸ್ಥಿರ ಶೂನ್ಯ *result(ಸಂರಚನಾ merge_list *entry, size_t *size)
{
	ಗಣನಾ object_type type;
	ಸಂರಚನಾ blob *base, *our, *their;
	ಧ್ರುವ ವರ್ಣ *path = entry->path;

	ಯದಿ (!entry->stage)
		ಪ್ರತ್ಯಾಗಮ odb_read_object(the_repository->objects,
				       &entry->blob->object.oid, &type,
				       size);
	base = ರಿಕ್ತ;
	ಯದಿ (entry->stage == 1) {
		base = entry->blob;
		entry = entry->link;
	}
	our = ರಿಕ್ತ;
	ಯದಿ (entry && entry->stage == 2) {
		our = entry->blob;
		entry = entry->link;
	}
	their = ರಿಕ್ತ;
	ಯದಿ (entry)
		their = entry->blob;
	ಪ್ರತ್ಯಾಗಮ merge_blobs(the_repository->index, path,
			   base, our, their, size);
}

ಸ್ಥಿರ ಶೂನ್ಯ *origin(ಸಂರಚನಾ merge_list *entry, size_t *size)
{
	ಗಣನಾ object_type type;
	ಯಾವತ್ (entry) {
		ಯದಿ (entry->stage == 2)
			ಪ್ರತ್ಯಾಗಮ odb_read_object(the_repository->objects,
					       &entry->blob->object.oid,
					       &type, size);
		entry = entry->link;
	}
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_outf(ಶೂನ್ಯ *priv UNUSED, mmfile_t *mb, ಪೂರ್ಣಾಙ್ಕ nbuf)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪ್ರತಿ (i = 0; i < nbuf; i++)
		printf("%.*s", (ಪೂರ್ಣಾಙ್ಕ) mb[i].size, mb[i].ptr);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_diff(ಸಂರಚನಾ merge_list *entry)
{
	size_t size;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .out_ಸಾಲು = show_outf };

	memset(&xpp, 0, ಪರಿಮಾಣ(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, ಪರಿಮಾಣ(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(entry, &size);
	ಯದಿ (!src.ptr)
		size = 0;
	src.size = size;
	dst.ptr = result(entry, &size);
	ಯದಿ (!dst.ptr)
		size = 0;
	dst.size = size;
	ಯದಿ (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	ಮುಕ್ತ(src.ptr);
	ಮುಕ್ತ(dst.ptr);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_result_list(ಸಂರಚನಾ merge_list *entry)
{
	printf("%s\n", explanation(entry));
	ಕುರು {
		ಸಂರಚನಾ merge_list *link = entry->link;
		ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *desc[4] = { "result", "base", "our", "their" };
		printf("  %-6s %o %s %s\n", desc[entry->stage], entry->mode, oid_to_hex(&entry->blob->object.oid), entry->path);
		entry = link;
	} ಯಾವತ್ (entry);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_result(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ merge_list *walk;

	walk = merge_result;
	ಯಾವತ್ (walk) {
		show_result_list(walk);
		show_diff(walk);
		walk = walk->next;
	}
}

/* An empty entry never compares same, not even to another empty entry */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ same_entry(ಸಂರಚನಾ name_entry *a, ಸಂರಚನಾ name_entry *b)
{
	ಪ್ರತ್ಯಾಗಮ	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->mode == b->mode;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ both_empty(ಸಂರಚನಾ name_entry *a, ಸಂರಚನಾ name_entry *b)
{
	ಪ್ರತ್ಯಾಗಮ is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

ಸ್ಥಿರ ಸಂರಚನಾ merge_list *create_entry(unsigned stage, unsigned mode, ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಧ್ರುವ ವರ್ಣ *path)
{
	ಸಂರಚನಾ merge_list *res = xcalloc(1, ಪರಿಮಾಣ(*res));

	res->stage = stage;
	res->path = path;
	res->mode = mode;
	res->blob = lookup_blob(the_repository, oid);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ವರ್ಣ *traverse_path(ಧ್ರುವ ಸಂರಚನಾ traverse_info *info, ಧ್ರುವ ಸಂರಚನಾ name_entry *n)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_path(&buf, info, n->path, n->pathlen);
	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&buf, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ resolve(ಧ್ರುವ ಸಂರಚನಾ traverse_info *info, ಸಂರಚನಾ name_entry *ours, ಸಂರಚನಾ name_entry *result)
{
	ಸಂರಚನಾ merge_list *orig, *final;
	ಧ್ರುವ ವರ್ಣ *path;

	/* If it's already ours, don't bother showing it */
	ಯದಿ (!ours)
		ಪ್ರತ್ಯಾಗಮ;

	path = traverse_path(info, result);
	orig = create_entry(2, ours->mode, &ours->oid, path);
	final = create_entry(0, result->mode, &result->oid, path);

	final->link = orig;

	add_merge_entry(final);
}

ಸ್ಥಿರ ಶೂನ್ಯ unresolved_directory(ಧ್ರುವ ಸಂರಚನಾ traverse_info *info,
				 ಸಂರಚನಾ name_entry n[3])
{
	ಸಂರಚನಾ repository *r = the_repository;
	ವರ್ಣ *newbase;
	ಸಂರಚನಾ name_entry *p;
	ಸಂರಚನಾ ವೃಕ್ಷ_desc t[3];
	ಶೂನ್ಯ *buf0, *buf1, *buf2;

	ಪ್ರತಿ (p = n; p < n + 3; p++) {
		ಯದಿ (p->mode && S_ISDIR(p->mode))
			ಭಙ್ಗ;
	}
	ಯದಿ (n + 3 <= p)
		ಪ್ರತ್ಯಾಗಮ; /* there is no tree here */

	newbase = traverse_path(info, p);

#define ENTRY_OID(e) (((e)->mode && S_ISDIR((e)->mode)) ? &(e)->oid : ರಿಕ್ತ)
	buf0 = fill_ವೃಕ್ಷ_descriptor(r, t + 0, ENTRY_OID(n + 0));
	buf1 = fill_ವೃಕ್ಷ_descriptor(r, t + 1, ENTRY_OID(n + 1));
	buf2 = fill_ವೃಕ್ಷ_descriptor(r, t + 2, ENTRY_OID(n + 2));
#undef ENTRY_OID

	trivial_merge_trees(t, newbase);

	ಮುಕ್ತ(buf0);
	ಮುಕ್ತ(buf1);
	ಮುಕ್ತ(buf2);
	ಮುಕ್ತ(newbase);
}


ಸ್ಥಿರ ಸಂರಚನಾ merge_list *link_entry(unsigned stage, ಧ್ರುವ ಸಂರಚನಾ traverse_info *info, ಸಂರಚನಾ name_entry *n, ಸಂರಚನಾ merge_list *entry)
{
	ಧ್ರುವ ವರ್ಣ *path;
	ಸಂರಚನಾ merge_list *link;

	ಯದಿ (!n->mode)
		ಪ್ರತ್ಯಾಗಮ entry;
	ಯದಿ (entry)
		path = entry->path;
	ಅನ್ಯಥಾ
		path = traverse_path(info, n);
	link = create_entry(stage, n->mode, &n->oid, path);
	link->link = entry;
	ಪ್ರತ್ಯಾಗಮ link;
}

ಸ್ಥಿರ ಶೂನ್ಯ unresolved(ಧ್ರುವ ಸಂರಚನಾ traverse_info *info, ಸಂರಚನಾ name_entry n[3])
{
	ಸಂರಚನಾ merge_list *entry = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ i;
	unsigned dirmask = 0, mask = 0;

	ಪ್ರತಿ (i = 0; i < 3; i++) {
		mask |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		ಯದಿ (!n[i].mode || S_ISDIR(n[i].mode))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	ಯದಿ (dirmask == mask)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (n[2].mode && !S_ISDIR(n[2].mode))
		entry = link_entry(3, info, n + 2, entry);
	ಯದಿ (n[1].mode && !S_ISDIR(n[1].mode))
		entry = link_entry(2, info, n + 1, entry);
	ಯದಿ (n[0].mode && !S_ISDIR(n[0].mode))
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ threeway_callback(ಪೂರ್ಣಾಙ್ಕ n UNUSED, unsigned long mask,
			     unsigned long dirmask UNUSED,
			     ಸಂರಚನಾ name_entry *entry, ಸಂರಚನಾ traverse_info *info)
{
	/* Same in both? */
	ಯದಿ (same_entry(entry+1, entry+2) || both_empty(entry+1, entry+2)) {
		/* Modified, added or removed identically */
		resolve(info, ರಿಕ್ತ, entry+1);
		ಪ್ರತ್ಯಾಗಮ mask;
	}

	ಯದಿ (same_entry(entry+0, entry+1)) {
		ಯದಿ (!is_null_oid(&entry[2].oid) && !S_ISDIR(entry[2].mode)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, entry+1, entry+2);
			ಪ್ರತ್ಯಾಗಮ mask;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	ಯದಿ (same_entry(entry+0, entry+2) || both_empty(entry+0, entry+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, ರಿಕ್ತ, entry+1);
		ಪ್ರತ್ಯಾಗಮ mask;
	}

	unresolved(info, entry);
	ಪ್ರತ್ಯಾಗಮ mask;
}

ಸ್ಥಿರ ಶೂನ್ಯ trivial_merge_trees(ಸಂರಚನಾ ವೃಕ್ಷ_desc t[3], ಧ್ರುವ ವರ್ಣ *base)
{
	ಸಂರಚನಾ traverse_info info;

	setup_traverse_info(&info, base);
	info.fn = threeway_callback;
	traverse_trees(the_repository->index, 3, t, &info);
}

ಸ್ಥಿರ ಶೂನ್ಯ *get_ವೃಕ್ಷ_descriptor(ಸಂರಚನಾ repository *r,
				 ಸಂರಚನಾ ವೃಕ್ಷ_desc *desc,
				 ಧ್ರುವ ವರ್ಣ *rev)
{
	ಸಂರಚನಾ object_id oid;
	ಶೂನ್ಯ *buf;

	ಯದಿ (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_ವೃಕ್ಷ_descriptor(r, desc, &oid);
	ಯದಿ (!buf)
		die("%s is not a tree", rev);
	ಪ್ರತ್ಯಾಗಮ buf;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ trivial_merge(ಧ್ರುವ ವರ್ಣ *base,
			 ಧ್ರುವ ವರ್ಣ *branch1,
			 ಧ್ರುವ ವರ್ಣ *branch2)
{
	ಸಂರಚನಾ repository *r = the_repository;
	ಸಂರಚನಾ ವೃಕ್ಷ_desc t[3];
	ಶೂನ್ಯ *buf1, *buf2, *buf3;

	buf1 = get_ವೃಕ್ಷ_descriptor(r, t+0, base);
	buf2 = get_ವೃಕ್ಷ_descriptor(r, t+1, branch1);
	buf3 = get_ವೃಕ್ಷ_descriptor(r, t+2, branch2);
	trivial_merge_trees(t, "");
	ಮುಕ್ತ(buf1);
	ಮುಕ್ತ(buf2);
	ಮುಕ್ತ(buf3);

	show_result();
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಗಣನಾ mode {
	MODE_UNKNOWN,
	MODE_TRIVIAL,
	MODE_REAL,
};

ಸಂರಚನಾ merge_ವೃಕ್ಷ_options {
	ಪೂರ್ಣಾಙ್ಕ mode;
	ಪೂರ್ಣಾಙ್ಕ allow_unrelated_histories;
	ಪೂರ್ಣಾಙ್ಕ show_messages;
	ಪೂರ್ಣಾಙ್ಕ name_only;
	ಪೂರ್ಣಾಙ್ಕ use_stdin;
	ಸಂರಚನಾ merge_options merge_options;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ real_merge(ಸಂರಚನಾ merge_ವೃಕ್ಷ_options *o,
		      ಧ್ರುವ ವರ್ಣ *merge_base,
		      ಧ್ರುವ ವರ್ಣ *branch1, ಧ್ರುವ ವರ್ಣ *branch2,
		      ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಸಂರಚನಾ commit *parent1, *parent2;
	ಸಂರಚನಾ commit_list *merge_bases = ರಿಕ್ತ;
	ಸಂರಚನಾ merge_result result = { 0 };
	ಪೂರ್ಣಾಙ್ಕ show_messages = o->show_messages;
	ಸಂರಚನಾ merge_options opt;

	copy_merge_options(&opt, &o->merge_options);
	opt.show_rename_progress = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	ಯದಿ (merge_base) {
		ಸಂರಚನಾ ವೃಕ್ಷ *base_ವೃಕ್ಷ, *parent1_ವೃಕ್ಷ, *parent2_ವೃಕ್ಷ;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		ಸಂರಚನಾ object_id base_oid, head_oid, merge_oid;

		ಯದಿ (repo_get_oid_treeish(the_repository, merge_base, &base_oid))
			die(_("could not parse as tree '%s'"), merge_base);
		base_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository,
						     &base_oid);
		ಯದಿ (!base_ವೃಕ್ಷ)
			die(_("unable to read tree (%s)"), oid_to_hex(&base_oid));
		ಯದಿ (repo_get_oid_treeish(the_repository, branch1, &head_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository,
							&head_oid);
		ಯದಿ (!parent1_ವೃಕ್ಷ)
			die(_("unable to read tree (%s)"), oid_to_hex(&head_oid));
		ಯದಿ (repo_get_oid_treeish(the_repository, branch2, &merge_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository,
							&merge_oid);
		ಯದಿ (!parent2_ವೃಕ್ಷ)
			die(_("unable to read tree (%s)"), oid_to_hex(&merge_oid));

		opt.ancestor = merge_base;
		merge_incore_nonrecursive(&opt, base_ವೃಕ್ಷ, parent1_ವೃಕ್ಷ, parent2_ವೃಕ್ಷ, &result);
	} ಅನ್ಯಥಾ {
		parent1 = get_merge_parent(branch1);
		ಯದಿ (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_merge_parent(branch2);
		ಯದಿ (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		ಯದಿ (repo_get_merge_bases(the_repository, parent1,
					 parent2, &merge_bases) < 0)
			exit(128);
		ಯದಿ (!merge_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		merge_bases = commit_list_reverse(merge_bases);
		merge_incore_recursive(&opt, merge_bases, parent1, parent2, &result);
		commit_list_free(merge_bases);
	}

	ಯದಿ (result.clean < 0)
		die(_("failure to merge"));

	ಯದಿ (o->merge_options.mergeability_only)
		ಗಚ್ಛ cleanup;

	ಯದಿ (show_messages == -1)
		show_messages = !result.clean;

	ಯದಿ (o->use_stdin)
		printf("%d%c", result.clean, ಸಾಲು_termination);
	printf("%s%c", oid_to_hex(&result.ವೃಕ್ಷ->object.oid), ಸಾಲು_termination);
	ಯದಿ (!result.clean) {
		ಸಂರಚನಾ string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ಧ್ರುವ ವರ್ಣ *last = ರಿಕ್ತ;

		merge_get_conflicted_files(&result, &conflicted_files);
		ಪ್ರತಿ (size_t i = 0; i < conflicted_files.nr; i++) {
			ಧ್ರುವ ವರ್ಣ *name = conflicted_files.ವಸ್ತುಗಳು[i].string;
			ಸಂರಚನಾ stage_info *c = conflicted_files.ವಸ್ತುಗಳು[i].util;
			ಯದಿ (!o->name_only)
				printf("%06o %s %d\t",
				       c->mode, oid_to_hex(&c->oid), c->stage);
			ಅನ್ಯಥಾ ಯದಿ (last && !strcmp(last, name))
				ಅನುವರ್ತ;
			write_name_quoted_relative(
				name, prefix, stdout, ಸಾಲು_termination);
			last = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	ಯದಿ (show_messages) {
		putchar(ಸಾಲು_termination);
		merge_display_update_messages(&opt, ಸಾಲು_termination == '\0',
					      &result);
	}
	ಯದಿ (o->use_stdin)
		putchar(ಸಾಲು_termination);

cleanup:
	merge_finalize(&opt, &result);
	clear_merge_options(&opt);
	ಪ್ರತ್ಯಾಗಮ !result.clean; /* result.clean < 0 handled above */
}

ಪೂರ್ಣಾಙ್ಕ cmd_merge_ವೃಕ್ಷ(ಪೂರ್ಣಾಙ್ಕ argc,
		   ಧ್ರುವ ವರ್ಣ **argv,
		   ಧ್ರುವ ವರ್ಣ *prefix,
		   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ merge_ವೃಕ್ಷ_options o = { .show_messages = -1 };
	ಸಂರಚನಾ strvec xopts = STRVEC_INIT;
	ಪೂರ್ಣಾಙ್ಕ expected_remaining_argc;
	ಪೂರ್ಣಾಙ್ಕ original_argc;
	ಧ್ರುವ ವರ್ಣ *merge_base = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;

	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ merge_ವೃಕ್ಷ_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		ರಿಕ್ತ
	};
	ಸಂರಚನಾ option mt_options[] = {
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
		OPT_SET_INT('z', ರಿಕ್ತ, &ಸಾಲು_termination,
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
			     merge_ವೃಕ್ಷ_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	ಯದಿ (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.merge_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !ಸಾಲು_termination, "-z");

	ಯದಿ (xopts.nr && o.mode == MODE_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	ಪ್ರತಿ (size_t x = 0; x < xopts.nr; x++)
		ಯದಿ (parse_merge_opt(&o.merge_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	ಯದಿ (o.use_stdin) {
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

		ಯದಿ (o.mode == MODE_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		ಯದಿ (merge_base)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		ಸಾಲು_termination = '\0';
		ಯಾವತ್ (strbuf_getline_lf(&buf, stdin) != EOF) {
			ಸಂರಚನಾ string_list split = STRING_LIST_INIT_NODUP;
			ಧ್ರುವ ವರ್ಣ *ಒಳಹರಿವು_merge_base = ರಿಕ್ತ;

			string_list_split_in_place_f(&split, buf.buf, " ", -1,
						     STRING_LIST_SPLIT_TRIM);

			ಯದಿ (split.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			ಯದಿ (!strcmp(split.ವಸ್ತುಗಳು[1].string, "--")) {
				ಒಳಹರಿವು_merge_base = split.ವಸ್ತುಗಳು[0].string;
			}

			ಯದಿ (ಒಳಹರಿವು_merge_base && split.nr == 4) {
				real_merge(&o, ಒಳಹರಿವು_merge_base,
					   split.ವಸ್ತುಗಳು[2].string, split.ವಸ್ತುಗಳು[3].string,
					   prefix);
			} ಅನ್ಯಥಾ ಯದಿ (!ಒಳಹರಿವು_merge_base && split.nr == 2) {
				real_merge(&o, ರಿಕ್ತ,
					   split.ವಸ್ತುಗಳು[0].string, split.ವಸ್ತುಗಳು[1].string,
					   prefix);
			} ಅನ್ಯಥಾ {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&split, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		ಗಚ್ಛ out;
	}

	/* Figure out which mode to use */
	ವಿಕಲ್ಪ (o.mode) {
	ಸ್ವಭಾವ:
		BUG("unexpected command mode %d", o.mode);
	ಪಕ್ಷ MODE_UNKNOWN:
		ವಿಕಲ್ಪ (argc) {
		ಸ್ವಭಾವ:
			usage_with_options(merge_ವೃಕ್ಷ_usage, mt_options);
		ಪಕ್ಷ 2:
			o.mode = MODE_REAL;
			ಭಙ್ಗ;
		ಪಕ್ಷ 3:
			o.mode = MODE_TRIVIAL;
			ಭಙ್ಗ;
		}
		expected_remaining_argc = argc;
		ಭಙ್ಗ;
	ಪಕ್ಷ MODE_REAL:
		expected_remaining_argc = 2;
		ಭಙ್ಗ;
	ಪಕ್ಷ MODE_TRIVIAL:
		expected_remaining_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		ಭಙ್ಗ;
	}
	ಯದಿ (o.mode == MODE_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	ಯದಿ (argc != expected_remaining_argc)
		usage_with_options(merge_ವೃಕ್ಷ_usage, mt_options);

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	/* Do the relevant type of merge */
	ಯದಿ (o.mode == MODE_REAL)
		ret = real_merge(&o, merge_base, argv[0], argv[1], prefix);
	ಅನ್ಯಥಾ
		ret = trivial_merge(argv[0], argv[1], argv[2]);

out:
	strvec_clear(&xopts);
	ಪ್ರತ್ಯಾಗಮ ret;
}
