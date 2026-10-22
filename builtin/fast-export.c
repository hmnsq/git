/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "refspec.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "object.h"
#অন্তর্ভাব "tag.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "diffcore.h"
#অন্তর্ভাব "log-tree.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "decorate.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "utf8.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "blob.h"
#অন্তর্ভাব "gpg-interface.h"

স্থির ধ্রুবক অক্ষর *ধ্রুবক fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	ফাঁকা
};

স্থির পূর্ণসংখ্যা progress;
স্থির গণনা sign_mode signed_tag_mode = SIGN_ABORT;
স্থির গণনা sign_mode signed_commit_mode = SIGN_STRIP;
স্থির গণনা tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
স্থির গণনা reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
স্থির পূর্ণসংখ্যা fake_missing_tagger;
স্থির পূর্ণসংখ্যা use_done_feature;
স্থির পূর্ণসংখ্যা no_তথ্য;
স্থির পূর্ণসংখ্যা full_বৃক্ষ;
স্থির পূর্ণসংখ্যা reference_excluded_commits;
স্থির পূর্ণসংখ্যা show_original_ids;
স্থির পূর্ণসংখ্যা mark_tags;
স্থির কাঠামো string_list extra_refs = STRING_LIST_INIT_DUP;
স্থির কাঠামো string_list tag_refs = STRING_LIST_INIT_DUP;
স্থির কাঠামো refspec refspecs;
স্থির পূর্ণসংখ্যা anonymize;
স্থির কাঠামো hashmap anonymized_seeds;
স্থির কাঠামো revision_sources revision_sources;

স্থির পূর্ণসংখ্যা parse_opt_sign_mode(ধ্রুবক কাঠামো option *opt,
			       ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	গণনা sign_mode *val = opt->মান;

	যদি (unset)
		ফেরত 0;

	যদি (parse_sign_mode(arg, val, ফাঁকা) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		ফেরত error(_("unknown %s mode: %s"), opt->long_name, arg);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা parse_opt_tag_of_filtered_mode(ধ্রুবক কাঠামো option *opt,
					  ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	গণনা tag_of_filtered_mode *val = opt->মান;

	যদি (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	অন্যথা যদি (!strcmp(arg, "drop"))
		*val = DROP;
	অন্যথা যদি (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	অন্যথা
		ফেরত error(_("unknown tag-of-filtered mode: %s"), arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা parse_opt_reencode_mode(ধ্রুবক কাঠামো option *opt,
				   ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	গণনা reencode_mode *val = opt->মান;

	যদি (unset) {
		*val = REENCODE_ABORT;
		ফেরত 0;
	}

	নির্বাচন (git_parse_maybe_bool(arg)) {
	ক্ষেত্র 0:
		*val = REENCODE_NO;
		বিরতি;
	ক্ষেত্র 1:
		*val = REENCODE_YES;
		বিরতি;
	পূর্বনির্ধারিত:
		যদি (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		অন্যথা
			ফেরত error(_("unknown reencoding mode: %s"), arg);
	}

	ফেরত 0;
}

স্থির কাঠামো decoration idnums;
স্থির uint32_t last_idnum;
কাঠামো anonymized_entry {
	কাঠামো hashmap_entry hash;
	অক্ষর *anon;
	ধ্রুবক অক্ষর orig[FLEX_ARRAY];
};

কাঠামো anonymized_entry_চাবি {
	কাঠামো hashmap_entry hash;
	ধ্রুবক অক্ষর *orig;
	আকার_t orig_len;
};

স্থির পূর্ণসংখ্যা anonymized_entry_cmp(ধ্রুবক শূন্য *cmp_তথ্য UNUSED,
				ধ্রুবক কাঠামো hashmap_entry *eptr,
				ধ্রুবক কাঠামো hashmap_entry *entry_or_চাবি,
				ধ্রুবক শূন্য *keydata)
{
	ধ্রুবক কাঠামো anonymized_entry *a, *b;

	a = container_of(eptr, ধ্রুবক কাঠামো anonymized_entry, hash);
	যদি (keydata) {
		ধ্রুবক কাঠামো anonymized_entry_চাবি *চাবি = keydata;
		পূর্ণসংখ্যা equal = !xstrncmpz(a->orig, চাবি->orig, চাবি->orig_len);
		ফেরত !equal;
	}

	b = container_of(entry_or_চাবি, ধ্রুবক কাঠামো anonymized_entry, hash);
	ফেরত strcmp(a->orig, b->orig);
}

স্থির কাঠামো anonymized_entry *add_anonymized_entry(কাঠামো hashmap *map,
						     unsigned hash,
						     ধ্রুবক অক্ষর *orig, আকার_t len,
						     অক্ষর *anon)
{
	কাঠামো anonymized_entry *ret, *old;

	যদি (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, ফাঁকা, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	যদি (old) {
		মুক্ত(old->anon);
		মুক্ত(old);
	}

	ফেরত ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
স্থির ধ্রুবক অক্ষর *anonymize_str(কাঠামো hashmap *map,
				 অক্ষর *(*generate)(শূন্য),
				 ধ্রুবক অক্ষর *orig, আকার_t len)
{
	কাঠামো anonymized_entry_চাবি চাবি;
	কাঠামো anonymized_entry *ret;

	hashmap_entry_init(&চাবি.hash, memhash(orig, len));
	চাবি.orig = orig;
	চাবি.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &চাবি, hash, &চাবি);

	/* ...otherwise check if we've already seen it in this context... */
	যদি (!ret)
		ret = hashmap_get_entry(map, &চাবি, hash, &চাবি);

	/* ...and finally generate a new mapping if necessary */
	যদি (!ret)
		ret = add_anonymized_entry(map, চাবি.hash.hash,
					   orig, len, generate());

	ফেরত ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
স্থির শূন্য anonymize_path(কাঠামো strbuf *out, ধ্রুবক অক্ষর *path,
			   কাঠামো hashmap *map,
			   অক্ষর *(*generate)(শূন্য))
{
	যখন (*path) {
		ধ্রুবক অক্ষর *end_of_component = strchrnul(path, '/');
		আকার_t len = end_of_component - path;
		ধ্রুবক অক্ষর *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		যদি (*path)
			strbuf_addch(out, *path++);
	}
}

স্থির অন্তঃরেখ শূন্য *mark_to_ptr(uint32_t mark)
{
	ফেরত (শূন্য *)(uintptr_t)mark;
}

স্থির অন্তঃরেখ uint32_t ptr_to_mark(শূন্য * mark)
{
	ফেরত (uint32_t)(uintptr_t)mark;
}

স্থির অন্তঃরেখ শূন্য mark_object(কাঠামো object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

স্থির অন্তঃরেখ শূন্য mark_পরবর্তী_object(কাঠামো object *object)
{
	mark_object(object, ++last_idnum);
}

স্থির পূর্ণসংখ্যা get_object_mark(কাঠামো object *object)
{
	শূন্য *decoration = lookup_decoration(&idnums, object);
	যদি (!decoration)
		ফেরত 0;
	ফেরত ptr_to_mark(decoration);
}

স্থির কাঠামো commit *rewrite_commit(কাঠামো commit *p)
{
	জন্য (;;) {
		যদি (p->parents && p->parents->পরবর্তী)
			বিরতি;
		যদি (p->object.flags & UNINTERESTING)
			বিরতি;
		যদি (!(p->object.flags & TREESAME))
			বিরতি;
		যদি (!p->parents)
			ফেরত ফাঁকা;
		p = p->parents->item;
	}
	ফেরত p;
}

স্থির শূন্য show_progress(শূন্য)
{
	স্থির পূর্ণসংখ্যা গণক = 0;
	যদি (!progress)
		ফেরত;
	যদি ((++গণক % progress) == 0)
		printf("progress %d objects\n", গণক);
}

/*
 * Ideally we would want some transformation of the blob data here
 * that is unreversible, but would still be the same size and have
 * the same data relationship to other blobs (so that we get the same
 * delta and packing behavior as the original). But the first and last
 * requirements there are probably mutually exclusive, so let's take
 * the easy way out for now, and just generate arbitrary content.
 *
 * There's no need to cache this result with anonymize_mem, since
 * we already handle blob content caching with marks.
 */
স্থির অক্ষর *anonymize_blob(unsigned long *আকার_2)
{
	স্থির পূর্ণসংখ্যা গণক;
	কাঠামো strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", গণক++);
	*আকার_2 = out.len;
	ফেরত strbuf_detach(&out, ফাঁকা);
}

স্থির শূন্য export_blob(ধ্রুবক কাঠামো object_id *oid)
{
	unsigned long আকার_2;
	গণনা object_type type;
	অক্ষর *buf;
	কাঠামো object *object;
	পূর্ণসংখ্যা eaten;

	যদি (no_তথ্য)
		ফেরত;

	যদি (is_null_oid(oid))
		ফেরত;

	object = lookup_object(the_repository, oid);
	যদি (object && object->flags & SHOWN)
		ফেরত;

	যদি (anonymize) {
		buf = anonymize_blob(&আকার_2);
		object = (কাঠামো object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} অন্যথা {
		আকার_t আকার_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &আকার_st);
		আকার_2 = cast_আকার_t_to_ulong(আকার_st);
		যদি (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		যদি (check_object_signature(the_repository, oid, buf, আকার_2,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_বাফার(the_repository, oid, type,
					     আকার_2, buf, &eaten);
	}

	যদি (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_পরবর্তী_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	যদি (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)আকার_2);
	যদি (আকার_2 && fwrite(buf, আকার_2, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	যদি (!eaten)
		মুক্ত(buf);
}

স্থির পূর্ণসংখ্যা depth_first(ধ্রুবক শূন্য *a_, ধ্রুবক শূন্য *b_)
{
	ধ্রুবক কাঠামো diff_filepair *a = *((ধ্রুবক কাঠামো diff_filepair **)a_);
	ধ্রুবক কাঠামো diff_filepair *b = *((ধ্রুবক কাঠামো diff_filepair **)b_);
	ধ্রুবক অক্ষর *name_a, *name_b;
	পূর্ণসংখ্যা len_a, len_b, len;
	পূর্ণসংখ্যা cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	যদি (cmp)
		ফেরত cmp;
	cmp = len_b - len_a;
	যদি (cmp)
		ফেরত cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	ফেরত (a->status == 'R') - (b->status == 'R');
}

স্থির শূন্য print_path_1(ধ্রুবক অক্ষর *path)
{
	পূর্ণসংখ্যা need_quote = quote_c_style(path, ফাঁকা, ফাঁকা, 0);
	যদি (need_quote)
		quote_c_style(path, ফাঁকা, stdout, 0);
	অন্যথা যদি (strchr(path, ' '))
		printf("\"%s\"", path);
	অন্যথা
		printf("%s", path);
}

স্থির অক্ষর *anonymize_path_component(শূন্য)
{
	স্থির পূর্ণসংখ্যা গণক;
	কাঠামো strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", গণক++);
	ফেরত strbuf_detach(&out, ফাঁকা);
}

স্থির শূন্য print_path(ধ্রুবক অক্ষর *path)
{
	যদি (!anonymize)
		print_path_1(path);
	অন্যথা {
		স্থির কাঠামো hashmap paths;
		স্থির কাঠামো strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

স্থির অক্ষর *generate_fake_oid(শূন্য)
{
	স্থির uint32_t গণক = 1; /* avoid null oid */
	ধ্রুবক unsigned hashsz = the_hash_algo->rawsz;
	কাঠামো object_id oid;
	অক্ষর *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, গণক++);
	ফেরত oid_to_hex_r(hex, &oid);
}

স্থির ধ্রুবক অক্ষর *anonymize_oid(ধ্রুবক অক্ষর *oid_hex)
{
	স্থির কাঠামো hashmap objs;
	আকার_t len = strlen(oid_hex);
	ফেরত anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

স্থির শূন্য show_filemodify(কাঠামো diff_queue_struct *q,
			    কাঠামো diff_options *options UNUSED, শূন্য *তথ্য)
{
	পূর্ণসংখ্যা i;
	কাঠামো string_list *changed = তথ্য;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	জন্য (i = 0; i < q->nr; i++) {
		কাঠামো diff_filespec *ospec = q->queue[i]->one;
		কাঠামো diff_filespec *spec = q->queue[i]->two;

		নির্বাচন (q->queue[i]->status) {
		ক্ষেত্র DIFF_STATUS_DELETED:
			printf("D ");
			print_path(spec->path);
			string_list_ঢোকাও(changed, spec->path);
			putchar('\n');
			বিরতি;

		ক্ষেত্র DIFF_STATUS_COPIED:
		ক্ষেত্র DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			যদি (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				print_path(ospec->path);
				putchar(' ');
				print_path(spec->path);
				string_list_ঢোকাও(changed, spec->path);
				putchar('\n');

				যদি (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					বিরতি;
			}
			/* fallthrough */

		ক্ষেত্র DIFF_STATUS_TYPE_CHANGED:
		ক্ষেত্র DIFF_STATUS_MODIFIED:
		ক্ষেত্র DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			যদি (no_তথ্য || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			অন্যথা {
				কাঠামো object *object = lookup_object(the_repository,
								      &spec->oid);
				printf("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_ঢোকাও(changed, spec->path);
			putchar('\n');
			বিরতি;

		পূর্বনির্ধারিত:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

স্থির অক্ষর *anonymize_ref_component(শূন্য)
{
	স্থির পূর্ণসংখ্যা গণক;
	কাঠামো strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", গণক++);
	ফেরত strbuf_detach(&out, ফাঁকা);
}

স্থির ধ্রুবক অক্ষর *anonymize_refname(ধ্রুবক অক্ষর *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	স্থির ধ্রুবক অক্ষর *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	স্থির কাঠামো hashmap refs;
	স্থির কাঠামো strbuf anon = STRBUF_INIT;
	পূর্ণসংখ্যা i;

	strbuf_reset(&anon);
	জন্য (i = 0; i < ARRAY_আকার(prefixes); i++) {
		যদি (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			বিরতি;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	ফেরত anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
স্থির অক্ষর *anonymize_commit_message(শূন্য)
{
	স্থির পূর্ণসংখ্যা গণক;
	ফেরত xstrfmt("subject %d\n\nbody\n", গণক++);
}

স্থির অক্ষর *anonymize_ident(শূন্য)
{
	স্থির পূর্ণসংখ্যা গণক;
	কাঠামো strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", গণক, গণক);
	গণক++;
	ফেরত strbuf_detach(&out, ফাঁকা);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
স্থির শূন্য anonymize_ident_সারি(ধ্রুবক অক্ষর **beg, ধ্রুবক অক্ষর **end)
{
	স্থির কাঠামো hashmap idents;
	স্থির কাঠামো strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	স্থির unsigned which_বাফার;

	কাঠামো strbuf *out;
	কাঠামো ident_split split;
	ধ্রুবক অক্ষর *end_of_header;

	out = &buffers[which_বাফার++];
	which_বাফার %= ARRAY_আকার(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = strchr(*beg, ' ');
	যদি (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (পূর্ণসংখ্যা)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	যদি (!split_ident_সারি(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		ধ্রুবক অক্ষর *ident;
		আকার_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} অন্যথা {
		strbuf_addstr(out, "Malformed Ident <malformed@example.com> 0 -0000");
	}

	*beg = out->buf;
	*end = out->buf + out->len;
}

/*
 * find_commit_multiline_header is similar to find_commit_header,
 * except that it handles multi-line headers, rather than simply
 * returning the first line of the header.
 *
 * The returned string has had the ' ' line continuation markers
 * removed, and points to allocated memory that must be free()d (not
 * to memory within 'msg').
 *
 * If the header is found, then *end is set to point at the '\n' in
 * msg that immediately follows the header value.
 */
স্থির ধ্রুবক অক্ষর *find_commit_multiline_header(ধ্রুবক অক্ষর *বার্তা,
						ধ্রুবক অক্ষর *চাবি,
						ধ্রুবক অক্ষর **end)
{
	কাঠামো strbuf val = STRBUF_INIT;
	ধ্রুবক অক্ষর *bol, *eol;
	আকার_t len;

	bol = find_commit_header(বার্তা, চাবি, &len);
	যদি (!bol)
		ফেরত ফাঁকা;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	যখন (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	ফেরত strbuf_detach(&val, ফাঁকা);
}

স্থির শূন্য print_signature(ধ্রুবক অক্ষর *signature, ধ্রুবক অক্ষর *object_hash)
{
	যদি (!signature)
		ফেরত;

	printf("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (unsigned)strlen(signature),
	       signature);
}

স্থির ধ্রুবক অক্ষর *append_signatures_for_header(কাঠামো string_list *signatures,
						ধ্রুবক অক্ষর *pos,
						ধ্রুবক অক্ষর *header,
						ধ্রুবক অক্ষর *object_hash)
{
	ধ্রুবক অক্ষর *signature;
	ধ্রুবক অক্ষর *start = pos;
	ধ্রুবক অক্ষর *end = pos;

	যখন ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (শূন্য *)object_hash;
		মুক্ত((অক্ষর *)signature);
		start = end;
	}

	ফেরত end;
}

স্থির শূন্য handle_commit(কাঠামো commit *commit, কাঠামো rev_info *rev,
			  কাঠামো string_list *paths_of_changed_objects)
{
	পূর্ণসংখ্যা saved_output_format = rev->diffopt.output_format;
	ধ্রুবক অক্ষর *commit_বাফার, *commit_বাফার_cursor;
	ধ্রুবক অক্ষর *author, *author_end, *committer, *committer_end;
	ধ্রুবক অক্ষর *encoding = ফাঁকা;
	আকার_t encoding_len;
	কাঠামো string_list signatures = STRING_LIST_INIT_DUP;
	ধ্রুবক অক্ষর *message;
	অক্ষর *reencoded = ফাঁকা;
	কাঠামো commit_list *p;
	ধ্রুবক অক্ষর *refname;
	পূর্ণসংখ্যা i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_বাফার_cursor = commit_বাফার = repo_get_commit_বাফার(the_repository, commit, ফাঁকা);

	author = strstr(commit_বাফার_cursor, "\nauthor ");
	যদি (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_বাফার_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_বাফার_cursor, "\ncommitter ");
	যদি (!committer)
		die(_("could not find committer in commit %s"),
		    oid_to_hex(&commit->object.oid));
	committer++;
	commit_বাফার_cursor = committer_end = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	যদি (*commit_বাফার_cursor == '\n') {
		encoding = find_commit_header(commit_বাফার_cursor + 1, "encoding", &encoding_len);
		যদি (encoding)
			commit_বাফার_cursor = encoding + encoding_len;
	}

	যদি (*commit_বাফার_cursor == '\n') {
		ধ্রুবক অক্ষর *after_sha1 = append_signatures_for_header(&signatures, commit_বাফার_cursor,
								      "gpgsig", "sha1");
		ধ্রুবক অক্ষর *after_sha256 = append_signatures_for_header(&signatures, commit_বাফার_cursor,
									"gpgsig-sha256", "sha256");
		commit_বাফার_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_বাফার_cursor, "\n\n");
	যদি (message)
		message += 2;

	যদি (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_বৃক্ষ) {
		parse_commit_or_die(commit->parents->item);
		diff_বৃক্ষ_oid(get_commit_বৃক্ষ_oid(commit->parents->item),
			      get_commit_বৃক্ষ_oid(commit), "", &rev->diffopt);
	}
	অন্যথা
		diff_মূল_বৃক্ষ_oid(get_commit_বৃক্ষ_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	জন্য (i = 0; i < diff_queued_diff.nr; i++)
		যদি (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	যদি (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_সারি(&committer, &committer_end);
		anonymize_ident_সারি(&author, &author_end);
	}

	mark_পরবর্তী_object(&commit->object);
	যদি (anonymize) {
		reencoded = anonymize_commit_message();
	} অন্যথা যদি (encoding) {
		অক্ষর *buf;
		নির্বাচন (reencode_mode) {
		ক্ষেত্র REENCODE_YES:
			buf = xstrfmt("%.*s", (পূর্ণসংখ্যা)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			মুক্ত(buf);
			বিরতি;
		ক্ষেত্র REENCODE_NO:
			বিরতি;
		ক্ষেত্র REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (পূর্ণসংখ্যা)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	যদি (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	যদি (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (পূর্ণসংখ্যা)(author_end - author), author,
	       (পূর্ণসংখ্যা)(committer_end - committer), committer);
	যদি (signatures.nr) {
		নির্বাচন (signed_commit_mode) {
		/* Exporting modes */
		ক্ষেত্র SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		ক্ষেত্র SIGN_VERBATIM:
			জন্য (আকার_t i = 0; i < signatures.nr; i++) {
				কাঠামো string_list_item *item = &signatures.বস্তুসমূহ[i];
				print_signature(item->string, item->util);
			}
			বিরতি;

		/* Stripping modes */
		ক্ষেত্র SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		ক্ষেত্র SIGN_STRIP:
			বিরতি;

		/* Aborting modes */
		ক্ষেত্র SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		পূর্বনির্ধারিত:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	যদি (!reencoded && encoding)
		printf("encoding %.*s\n", (পূর্ণসংখ্যা)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	মুক্ত(reencoded);
	repo_unuse_commit_বাফার(the_repository, commit, commit_বাফার);

	জন্য (i = 0, p = commit->parents; p; p = p->পরবর্তী) {
		কাঠামো object *obj = &p->item->object;
		পূর্ণসংখ্যা mark = get_object_mark(obj);

		যদি (!mark && !reference_excluded_commits)
			চালু;
		যদি (i == 0)
			printf("from ");
		অন্যথা
			printf("merge ");
		যদি (mark)
			printf(":%d\n", mark);
		অন্যথা
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	যদি (full_বৃক্ষ)
		printf("deleteall\n");
	log_বৃক্ষ_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

স্থির অক্ষর *anonymize_tag(শূন্য)
{
	স্থির পূর্ণসংখ্যা গণক;
	কাঠামো strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", গণক++);
	ফেরত strbuf_detach(&out, ফাঁকা);
}


স্থির শূন্য handle_tag(ধ্রুবক অক্ষর *name, কাঠামো tag *tag)
{
	আকার_t আকার_2;
	গণনা object_type type;
	অক্ষর *buf;
	ধ্রুবক অক্ষর *tagger, *tagger_end, *message;
	আকার_t message_আকার = 0;
	কাঠামো object *tagged;
	পূর্ণসংখ্যা tagged_mark;
	কাঠামো commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	যখন (tagged->type == OBJ_TAG) {
		tagged = ((কাঠামো tag *)tagged)->tagged;
	}
	যদি (tagged->type == OBJ_বৃক্ষ) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		ফেরত;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &আকার_2);
	যদি (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, আকার_2, "\n\n", 2);
	যদি (message) {
		message += 2;
		message_আকার = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : আকার_2, "\ntagger ", 8);
	যদি (!tagger) {
		যদি (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		অন্যথা
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} অন্যথা {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		যদি (anonymize)
			anonymize_ident_সারি(&tagger, &tagger_end);
	}

	যদি (anonymize) {
		name = anonymize_refname(name);
		যদি (message) {
			স্থির কাঠামো hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_আকার);
			message_আকার = strlen(message);
		}
	}

	/* handle signed tags */
	যদি (message) {
		আকার_t sig_offset = parse_signed_বাফার(message, message_আকার);
		যদি (sig_offset < message_আকার)
			নির্বাচন (signed_tag_mode) {
			/* Exporting modes */
			ক্ষেত্র SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			ক্ষেত্র SIGN_VERBATIM:
				বিরতি;

			/* Stripping modes */
			ক্ষেত্র SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			ক্ষেত্র SIGN_STRIP:
				message_আকার = sig_offset;
				বিরতি;

			/* Aborting modes */
			ক্ষেত্র SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			পূর্বনির্ধারিত:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	যদি (!tagged_mark) {
		নির্বাচন (tag_of_filtered_mode) {
		ক্ষেত্র TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		ক্ষেত্র DROP:
			/* Ignore this tag altogether */
			মুক্ত(buf);
			ফেরত;
		ক্ষেত্র REWRITE:
			যদি (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} অন্যথা যদি (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((কাঠামো commit *)tagged);
				যদি (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					মুক্ত(buf);
					ফেরত;
				}
				tagged_mark = get_object_mark(&p->object);
			} অন্যথা {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	যদি (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	যদি (mark_tags) {
		mark_পরবর্তী_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	যদি (tagged_mark)
		printf("from :%d\n", tagged_mark);
	অন্যথা
		printf("from %s\n", oid_to_hex(&tagged->oid));

	যদি (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (পূর্ণসংখ্যা)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (পূর্ণসংখ্যা)message_আকার, (পূর্ণসংখ্যা)message_আকার, message ? message : "");
	মুক্ত(buf);
}

স্থির কাঠামো commit *get_commit(কাঠামো rev_cmdline_entry *e, ধ্রুবক অক্ষর *full_name)
{
	নির্বাচন (e->item->type) {
	ক্ষেত্র OBJ_COMMIT:
		ফেরত (কাঠামো commit *)e->item;
	ক্ষেত্র OBJ_TAG: {
		কাঠামো tag *tag = (কাঠামো tag *)e->item;

		/* handle nested tags */
		যখন (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (কাঠামো tag *)tag->tagged;
		}
		যদি (!tag)
			die(_("tag %s points nowhere?"), e->name);
		ফেরত (কাঠামো commit *)tag;
	}
	পূর্বনির্ধারিত:
		ফেরত ফাঁকা;
	}
}

স্থির শূন্য get_tags_and_duplicates(কাঠামো rev_cmdline_info *info)
{
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < info->nr; i++) {
		কাঠামো rev_cmdline_entry *e = info->rev + i;
		কাঠামো object_id oid;
		কাঠামো commit *commit;
		অক্ষর *full_name = ফাঁকা;

		যদি (e->flags & UNINTERESTING)
			চালু;

		যদি (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			মুক্ত(full_name);
			চালু;
		}

		যদি (refspecs.nr) {
			অক্ষর *ব্যক্তিগত;
			ব্যক্তিগত = apply_refspecs(&refspecs, full_name);
			যদি (ব্যক্তিগত) {
				মুক্ত(full_name);
				full_name = ব্যক্তিগত;
			}
		}

		commit = get_commit(e, full_name);
		যদি (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			মুক্ত(full_name);
			চালু;
		}

		নির্বাচন (commit->object.type) {
		ক্ষেত্র OBJ_COMMIT:
			বিরতি;
		ক্ষেত্র OBJ_BLOB:
			export_blob(&commit->object.oid);
			মুক্ত(full_name);
			চালু;
		পূর্বনির্ধারিত: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			মুক্ত(full_name);
			চালু;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		যদি (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		যদি (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		অন্যথা
			মুক্ত(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

স্থির শূন্য handle_tags_and_duplicates(কাঠামো string_list *extras)
{
	কাঠামো commit *commit;
	পূর্ণসংখ্যা i;

	জন্য (i = extras->nr - 1; i >= 0; i--) {
		ধ্রুবক অক্ষর *name = extras->বস্তুসমূহ[i].string;
		কাঠামো object *object = extras->বস্তুসমূহ[i].util;
		পূর্ণসংখ্যা mark;

		নির্বাচন (object->type) {
		ক্ষেত্র OBJ_TAG:
			handle_tag(name, (কাঠামো tag *)object);
			বিরতি;
		ক্ষেত্র OBJ_COMMIT:
			যদি (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((কাঠামো commit *)object);
			যদি (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				printf("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				চালু;
			}

			mark = get_object_mark(&commit->object);
			যদি (!mark) {
				/*
				 * Getting here means we have a commit which
				 * was excluded by a negative refspec (e.g.
				 * fast-export ^HEAD HEAD).  If we are
				 * referencing excluded commits, set the ref
				 * to the exact commit.  Otherwise, the user
				 * wants the branch exported but every commit
				 * in its history to be deleted, which basically
				 * just means deletion of the ref.
				 */
				যদি (!reference_excluded_commits) {
					/* delete the ref */
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					চালু;
				}
				/* set ref to commit using oid, not mark */
				printf("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				চালু;
			}

			printf("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			বিরতি;
		}
	}
}

স্থির শূন্য export_marks(অক্ষর *file)
{
	unsigned পূর্ণসংখ্যা i;
	uint32_t mark;
	কাঠামো decoration_entry *deco = idnums.entries;
	FILE *f;
	পূর্ণসংখ্যা e = 0;

	f = fopen_for_writing(file);
	যদি (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	জন্য (i = 0; i < idnums.আকার_2; i++) {
		যদি (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			যদি (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    বিরতি;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	যদি (e)
		error(_("unable to write marks file %s."), file);
}

স্থির শূন্য import_marks(অক্ষর *প্রবেশ_file, পূর্ণসংখ্যা check_exists)
{
	অক্ষর সারি[512];
	FILE *f;
	কাঠামো stat sb;

	যদি (check_exists && stat(প্রবেশ_file, &sb))
		ফেরত;

	f = xfopen(প্রবেশ_file, "r");
	যখন (fgets(সারি, আকার(সারি), f)) {
		uint32_t mark;
		অক্ষর *সারি_end, *mark_end;
		কাঠামো object_id oid;
		কাঠামো object *object;
		কাঠামো commit *commit;
		গণনা object_type type;

		সারি_end = strchr(সারি, '\n');
		যদি (সারি[0] != ':' || !সারি_end)
			die(_("corrupt mark line: %s"), সারি);
		*সারি_end = '\0';

		mark = strtoumax(সারি + 1, &mark_end, 10);
		যদি (!mark || mark_end == সারি + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), সারি);

		যদি (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, ফাঁকা);
		যদি (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		যদি (type != OBJ_COMMIT)
			/* only commits */
			চালু;

		commit = lookup_commit(the_repository, &oid);
		যদি (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		যদি (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

স্থির শূন্য handle_deletes(শূন্য)
{
	পূর্ণসংখ্যা i;
	জন্য (i = 0; i < refspecs.nr; i++) {
		কাঠামো refspec_item *refspec = &refspecs.বস্তুসমূহ[i];
		যদি (*refspec->src)
			চালু;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

স্থির পূর্ণসংখ্যা parse_opt_anonymize_map(ধ্রুবক কাঠামো option *opt,
				   ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো hashmap *map = opt->মান;
	ধ্রুবক অক্ষর *delim, *মান;
	আকার_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	যদি (delim) {
		keylen = delim - arg;
		মান = delim + 1;
	} অন্যথা {
		keylen = strlen(arg);
		মান = arg;
	}

	যদি (!keylen || !*মান)
		ফেরত error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(মান));

	ফেরত 0;
}

পূর্ণসংখ্যা cmd_fast_export(পূর্ণসংখ্যা argc,
		    ধ্রুবক অক্ষর **argv,
		    ধ্রুবক অক্ষর *prefix,
		    কাঠামো repository *repo UNUSED)
{
	কাঠামো rev_info revs;
	কাঠামো commit *commit;
	অক্ষর *export_filename = ফাঁকা,
	     *import_filename = ফাঁকা,
	     *import_filename_if_exists = ফাঁকা;
	uint32_t lastimportid;
	কাঠামো string_list refspecs_list = STRING_LIST_INIT_NODUP;
	কাঠামো string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	কাঠামো option options[] = {
		OPT_INTEGER(0, "progress", &progress,
			    N_("show progress after <n> objects")),
		OPT_CALLBACK(0, "signed-tags", &signed_tag_mode, N_("mode"),
			     N_("select handling of signed tags"),
			     parse_opt_sign_mode),
		OPT_CALLBACK(0, "signed-commits", &signed_commit_mode, N_("mode"),
			     N_("select handling of signed commits"),
			     parse_opt_sign_mode),
		OPT_CALLBACK(0, "tag-of-filtered-object", &tag_of_filtered_mode, N_("mode"),
			     N_("select handling of tags that tag filtered objects"),
			     parse_opt_tag_of_filtered_mode),
		OPT_CALLBACK(0, "reencode", &reencode_mode, N_("mode"),
			     N_("select handling of commit messages in an alternate encoding"),
			     parse_opt_reencode_mode),
		OPT_STRING(0, "export-marks", &export_filename, N_("file"),
			     N_("dump marks to this file")),
		OPT_STRING(0, "import-marks", &import_filename, N_("file"),
			     N_("import marks from this file")),
		OPT_STRING(0, "import-marks-if-exists",
			     &import_filename_if_exists,
			     N_("file"),
			     N_("import marks from this file if it exists")),
		OPT_BOOL(0, "fake-missing-tagger", &fake_missing_tagger,
			 N_("fake a tagger when tags lack one")),
		OPT_BOOL(0, "full-tree", &full_বৃক্ষ,
			 N_("output full tree for each commit")),
		OPT_BOOL(0, "use-done-feature", &use_done_feature,
			     N_("use the done feature to terminate the stream")),
		OPT_BOOL(0, "no-data", &no_তথ্য, N_("skip output of blob data")),
		OPT_STRING_LIST(0, "refspec", &refspecs_list, N_("refspec"),
			     N_("apply refspec to exported refs")),
		OPT_BOOL(0, "anonymize", &anonymize, N_("anonymize output")),
		OPT_CALLBACK_F(0, "anonymize-map", &anonymized_seeds, N_("from:to"),
			       N_("convert <from> to <to> in anonymized output"),
			       PARSE_OPT_NONEG, parse_opt_anonymize_map),
		OPT_BOOL(0, "reference-excluded-parents",
			 &reference_excluded_commits, N_("reference parents which are not in fast-export stream by object id")),
		OPT_BOOL(0, "show-original-ids", &show_original_ids,
			    N_("show original object ids of blobs/commits")),
		OPT_BOOL(0, "mark-tags", &mark_tags,
			    N_("label tags with mark ids")),

		OPT_END()
	};

	যদি (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, ফাঁকা);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, prefix);
	init_revision_sources(&revision_sources);
	revs.topo_order = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, prefix, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = setup_revisions(argc, argv, &revs, ফাঁকা);
	যদি (argc > 1)
		usage_with_options (fast_export_usage, options);

	যদি (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	যদি (refspecs_list.nr) {
		পূর্ণসংখ্যা i;

		জন্য (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.বস্তুসমূহ[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	যদি (use_done_feature)
		printf("feature done\n");

	যদি (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	যদি (import_filename)
		import_marks(import_filename, 0);
	অন্যথা যদি (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	যদি (import_filename && revs.prune_তথ্য.nr)
		full_বৃক্ষ = 1;

	get_tags_and_duplicates(&revs.cmdline);

	যদি (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_তথ্য = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	যখন ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	যদি (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	যদি (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	ফেরত 0;
}
