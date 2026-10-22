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

স্থির ধ্রুব বর্ণ *ধ্রুব fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	রিক্ত
};

স্থির পূর্ণাঙ্ক progress;
স্থির গণনা sign_mode signed_tag_mode = SIGN_ABORT;
স্থির গণনা sign_mode signed_commit_mode = SIGN_STRIP;
স্থির গণনা tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
স্থির গণনা reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
স্থির পূর্ণাঙ্ক fake_missing_tagger;
স্থির পূর্ণাঙ্ক use_done_feature;
স্থির পূর্ণাঙ্ক no_data;
স্থির পূর্ণাঙ্ক full_বৃক্ষ;
স্থির পূর্ণাঙ্ক reference_excluded_commits;
স্থির পূর্ণাঙ্ক show_original_ids;
স্থির পূর্ণাঙ্ক mark_tags;
স্থির সংরচনা string_list extra_refs = STRING_LIST_INIT_DUP;
স্থির সংরচনা string_list tag_refs = STRING_LIST_INIT_DUP;
স্থির সংরচনা refspec refspecs;
স্থির পূর্ণাঙ্ক anonymize;
স্থির সংরচনা hashmap anonymized_seeds;
স্থির সংরচনা revision_sources revision_sources;

স্থির পূর্ণাঙ্ক parse_opt_sign_mode(ধ্রুব সংরচনা option *opt,
			       ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	গণনা sign_mode *val = opt->মান;

	যদি (unset)
		প্রত্যাগম 0;

	যদি (parse_sign_mode(arg, val, রিক্ত) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		প্রত্যাগম error(_("unknown %s mode: %s"), opt->long_name, arg);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক parse_opt_tag_of_filtered_mode(ধ্রুব সংরচনা option *opt,
					  ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	গণনা tag_of_filtered_mode *val = opt->মান;

	যদি (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	অন্যথা যদি (!strcmp(arg, "drop"))
		*val = DROP;
	অন্যথা যদি (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	অন্যথা
		প্রত্যাগম error(_("unknown tag-of-filtered mode: %s"), arg);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক parse_opt_reencode_mode(ধ্রুব সংরচনা option *opt,
				   ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	গণনা reencode_mode *val = opt->মান;

	যদি (unset) {
		*val = REENCODE_ABORT;
		প্রত্যাগম 0;
	}

	বিকল্প (git_parse_maybe_bool(arg)) {
	পক্ষ 0:
		*val = REENCODE_NO;
		ভঙ্গ;
	পক্ষ 1:
		*val = REENCODE_YES;
		ভঙ্গ;
	স্বভাব:
		যদি (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		অন্যথা
			প্রত্যাগম error(_("unknown reencoding mode: %s"), arg);
	}

	প্রত্যাগম 0;
}

স্থির সংরচনা decoration idnums;
স্থির uint32_t last_idnum;
সংরচনা anonymized_entry {
	সংরচনা hashmap_entry hash;
	বর্ণ *anon;
	ধ্রুব বর্ণ orig[FLEX_ARRAY];
};

সংরচনা anonymized_entry_চাবি {
	সংরচনা hashmap_entry hash;
	ধ্রুব বর্ণ *orig;
	size_t orig_len;
};

স্থির পূর্ণাঙ্ক anonymized_entry_cmp(ধ্রুব শূন্য *cmp_data UNUSED,
				ধ্রুব সংরচনা hashmap_entry *eptr,
				ধ্রুব সংরচনা hashmap_entry *entry_or_চাবি,
				ধ্রুব শূন্য *keydata)
{
	ধ্রুব সংরচনা anonymized_entry *a, *b;

	a = container_of(eptr, ধ্রুব সংরচনা anonymized_entry, hash);
	যদি (keydata) {
		ধ্রুব সংরচনা anonymized_entry_চাবি *চাবি = keydata;
		পূর্ণাঙ্ক equal = !xstrncmpz(a->orig, চাবি->orig, চাবি->orig_len);
		প্রত্যাগম !equal;
	}

	b = container_of(entry_or_চাবি, ধ্রুব সংরচনা anonymized_entry, hash);
	প্রত্যাগম strcmp(a->orig, b->orig);
}

স্থির সংরচনা anonymized_entry *add_anonymized_entry(সংরচনা hashmap *map,
						     unsigned hash,
						     ধ্রুব বর্ণ *orig, size_t len,
						     বর্ণ *anon)
{
	সংরচনা anonymized_entry *ret, *old;

	যদি (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, রিক্ত, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	যদি (old) {
		মুক্ত(old->anon);
		মুক্ত(old);
	}

	প্রত্যাগম ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
স্থির ধ্রুব বর্ণ *anonymize_str(সংরচনা hashmap *map,
				 বর্ণ *(*generate)(শূন্য),
				 ধ্রুব বর্ণ *orig, size_t len)
{
	সংরচনা anonymized_entry_চাবি চাবি;
	সংরচনা anonymized_entry *ret;

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

	প্রত্যাগম ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
স্থির শূন্য anonymize_path(সংরচনা strbuf *out, ধ্রুব বর্ণ *path,
			   সংরচনা hashmap *map,
			   বর্ণ *(*generate)(শূন্য))
{
	যাবত্ (*path) {
		ধ্রুব বর্ণ *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		ধ্রুব বর্ণ *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		যদি (*path)
			strbuf_addch(out, *path++);
	}
}

স্থির অন্তঃস্থ শূন্য *mark_to_ptr(uint32_t mark)
{
	প্রত্যাগম (শূন্য *)(uintptr_t)mark;
}

স্থির অন্তঃস্থ uint32_t ptr_to_mark(শূন্য * mark)
{
	প্রত্যাগম (uint32_t)(uintptr_t)mark;
}

স্থির অন্তঃস্থ শূন্য mark_object(সংরচনা object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

স্থির অন্তঃস্থ শূন্য mark_next_object(সংরচনা object *object)
{
	mark_object(object, ++last_idnum);
}

স্থির পূর্ণাঙ্ক get_object_mark(সংরচনা object *object)
{
	শূন্য *decoration = lookup_decoration(&idnums, object);
	যদি (!decoration)
		প্রত্যাগম 0;
	প্রত্যাগম ptr_to_mark(decoration);
}

স্থির সংরচনা commit *rewrite_commit(সংরচনা commit *p)
{
	প্রতি (;;) {
		যদি (p->parents && p->parents->next)
			ভঙ্গ;
		যদি (p->object.flags & UNINTERESTING)
			ভঙ্গ;
		যদি (!(p->object.flags & TREESAME))
			ভঙ্গ;
		যদি (!p->parents)
			প্রত্যাগম রিক্ত;
		p = p->parents->item;
	}
	প্রত্যাগম p;
}

স্থির শূন্য show_progress(শূন্য)
{
	স্থির পূর্ণাঙ্ক counter = 0;
	যদি (!progress)
		প্রত্যাগম;
	যদি ((++counter % progress) == 0)
		printf("progress %d objects\n", counter);
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
স্থির বর্ণ *anonymize_blob(unsigned long *size)
{
	স্থির পূর্ণাঙ্ক counter;
	সংরচনা strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	প্রত্যাগম strbuf_detach(&out, রিক্ত);
}

স্থির শূন্য export_blob(ধ্রুব সংরচনা object_id *oid)
{
	unsigned long size;
	গণনা object_type type;
	বর্ণ *buf;
	সংরচনা object *object;
	পূর্ণাঙ্ক eaten;

	যদি (no_data)
		প্রত্যাগম;

	যদি (is_null_oid(oid))
		প্রত্যাগম;

	object = lookup_object(the_repository, oid);
	যদি (object && object->flags & SHOWN)
		প্রত্যাগম;

	যদি (anonymize) {
		buf = anonymize_blob(&size);
		object = (সংরচনা object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} অন্যথা {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		যদি (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		যদি (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	যদি (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	যদি (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	যদি (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	যদি (!eaten)
		মুক্ত(buf);
}

স্থির পূর্ণাঙ্ক depth_first(ধ্রুব শূন্য *a_, ধ্রুব শূন্য *b_)
{
	ধ্রুব সংরচনা diff_filepair *a = *((ধ্রুব সংরচনা diff_filepair **)a_);
	ধ্রুব সংরচনা diff_filepair *b = *((ধ্রুব সংরচনা diff_filepair **)b_);
	ধ্রুব বর্ণ *name_a, *name_b;
	পূর্ণাঙ্ক len_a, len_b, len;
	পূর্ণাঙ্ক cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	যদি (cmp)
		প্রত্যাগম cmp;
	cmp = len_b - len_a;
	যদি (cmp)
		প্রত্যাগম cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	প্রত্যাগম (a->status == 'R') - (b->status == 'R');
}

স্থির শূন্য print_path_1(ধ্রুব বর্ণ *path)
{
	পূর্ণাঙ্ক need_quote = quote_c_style(path, রিক্ত, রিক্ত, 0);
	যদি (need_quote)
		quote_c_style(path, রিক্ত, stdout, 0);
	অন্যথা যদি (strchr(path, ' '))
		printf("\"%s\"", path);
	অন্যথা
		printf("%s", path);
}

স্থির বর্ণ *anonymize_path_component(শূন্য)
{
	স্থির পূর্ণাঙ্ক counter;
	সংরচনা strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	প্রত্যাগম strbuf_detach(&out, রিক্ত);
}

স্থির শূন্য print_path(ধ্রুব বর্ণ *path)
{
	যদি (!anonymize)
		print_path_1(path);
	অন্যথা {
		স্থির সংরচনা hashmap paths;
		স্থির সংরচনা strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

স্থির বর্ণ *generate_fake_oid(শূন্য)
{
	স্থির uint32_t counter = 1; /* avoid null oid */
	ধ্রুব unsigned hashsz = the_hash_algo->rawsz;
	সংরচনা object_id oid;
	বর্ণ *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	প্রত্যাগম oid_to_hex_r(hex, &oid);
}

স্থির ধ্রুব বর্ণ *anonymize_oid(ধ্রুব বর্ণ *oid_hex)
{
	স্থির সংরচনা hashmap objs;
	size_t len = strlen(oid_hex);
	প্রত্যাগম anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

স্থির শূন্য show_filemodify(সংরচনা diff_queue_struct *q,
			    সংরচনা diff_options *options UNUSED, শূন্য *data)
{
	পূর্ণাঙ্ক i;
	সংরচনা string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	প্রতি (i = 0; i < q->nr; i++) {
		সংরচনা diff_filespec *ospec = q->queue[i]->one;
		সংরচনা diff_filespec *spec = q->queue[i]->two;

		বিকল্প (q->queue[i]->status) {
		পক্ষ DIFF_STATUS_DELETED:
			printf("D ");
			print_path(spec->path);
			string_list_ঢোকাও(changed, spec->path);
			putchar('\n');
			ভঙ্গ;

		পক্ষ DIFF_STATUS_COPIED:
		পক্ষ DIFF_STATUS_RENAMED:
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
					ভঙ্গ;
			}
			/* fallthrough */

		পক্ষ DIFF_STATUS_TYPE_CHANGED:
		পক্ষ DIFF_STATUS_MODIFIED:
		পক্ষ DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			যদি (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			অন্যথা {
				সংরচনা object *object = lookup_object(the_repository,
								      &spec->oid);
				printf("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_ঢোকাও(changed, spec->path);
			putchar('\n');
			ভঙ্গ;

		স্বভাব:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

স্থির বর্ণ *anonymize_ref_component(শূন্য)
{
	স্থির পূর্ণাঙ্ক counter;
	সংরচনা strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	প্রত্যাগম strbuf_detach(&out, রিক্ত);
}

স্থির ধ্রুব বর্ণ *anonymize_refname(ধ্রুব বর্ণ *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	স্থির ধ্রুব বর্ণ *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	স্থির সংরচনা hashmap refs;
	স্থির সংরচনা strbuf anon = STRBUF_INIT;
	পূর্ণাঙ্ক i;

	strbuf_reset(&anon);
	প্রতি (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		যদি (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			ভঙ্গ;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	প্রত্যাগম anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
স্থির বর্ণ *anonymize_commit_message(শূন্য)
{
	স্থির পূর্ণাঙ্ক counter;
	প্রত্যাগম xstrfmt("subject %d\n\nbody\n", counter++);
}

স্থির বর্ণ *anonymize_ident(শূন্য)
{
	স্থির পূর্ণাঙ্ক counter;
	সংরচনা strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	প্রত্যাগম strbuf_detach(&out, রিক্ত);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
স্থির শূন্য anonymize_ident_সারি(ধ্রুব বর্ণ **beg, ধ্রুব বর্ণ **end)
{
	স্থির সংরচনা hashmap idents;
	স্থির সংরচনা strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	স্থির unsigned which_buffer;

	সংরচনা strbuf *out;
	সংরচনা ident_split split;
	ধ্রুব বর্ণ *end_of_header;

	out = &buffers[which_buffer++];
	which_buffer %= ARRAY_SIZE(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = strchr(*beg, ' ');
	যদি (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (পূর্ণাঙ্ক)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	যদি (!split_ident_সারি(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		ধ্রুব বর্ণ *ident;
		size_t len;

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
স্থির ধ্রুব বর্ণ *find_commit_multiline_header(ধ্রুব বর্ণ *বার্তা,
						ধ্রুব বর্ণ *চাবি,
						ধ্রুব বর্ণ **end)
{
	সংরচনা strbuf val = STRBUF_INIT;
	ধ্রুব বর্ণ *bol, *eol;
	size_t len;

	bol = find_commit_header(বার্তা, চাবি, &len);
	যদি (!bol)
		প্রত্যাগম রিক্ত;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	যাবত্ (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	প্রত্যাগম strbuf_detach(&val, রিক্ত);
}

স্থির শূন্য print_signature(ধ্রুব বর্ণ *signature, ধ্রুব বর্ণ *object_hash)
{
	যদি (!signature)
		প্রত্যাগম;

	printf("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (unsigned)strlen(signature),
	       signature);
}

স্থির ধ্রুব বর্ণ *append_signatures_for_header(সংরচনা string_list *signatures,
						ধ্রুব বর্ণ *pos,
						ধ্রুব বর্ণ *header,
						ধ্রুব বর্ণ *object_hash)
{
	ধ্রুব বর্ণ *signature;
	ধ্রুব বর্ণ *start = pos;
	ধ্রুব বর্ণ *end = pos;

	যাবত্ ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (শূন্য *)object_hash;
		মুক্ত((বর্ণ *)signature);
		start = end;
	}

	প্রত্যাগম end;
}

স্থির শূন্য handle_commit(সংরচনা commit *commit, সংরচনা rev_info *rev,
			  সংরচনা string_list *paths_of_changed_objects)
{
	পূর্ণাঙ্ক saved_output_format = rev->diffopt.output_format;
	ধ্রুব বর্ণ *commit_buffer, *commit_buffer_cursor;
	ধ্রুব বর্ণ *author, *author_end, *committer, *committer_end;
	ধ্রুব বর্ণ *encoding = রিক্ত;
	size_t encoding_len;
	সংরচনা string_list signatures = STRING_LIST_INIT_DUP;
	ধ্রুব বর্ণ *message;
	বর্ণ *reencoded = রিক্ত;
	সংরচনা commit_list *p;
	ধ্রুব বর্ণ *refname;
	পূর্ণাঙ্ক i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, রিক্ত);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	যদি (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	যদি (!committer)
		die(_("could not find committer in commit %s"),
		    oid_to_hex(&commit->object.oid));
	committer++;
	commit_buffer_cursor = committer_end = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	যদি (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		যদি (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	যদি (*commit_buffer_cursor == '\n') {
		ধ্রুব বর্ণ *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		ধ্রুব বর্ণ *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
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
	প্রতি (i = 0; i < diff_queued_diff.nr; i++)
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

	mark_next_object(&commit->object);
	যদি (anonymize) {
		reencoded = anonymize_commit_message();
	} অন্যথা যদি (encoding) {
		বর্ণ *buf;
		বিকল্প (reencode_mode) {
		পক্ষ REENCODE_YES:
			buf = xstrfmt("%.*s", (পূর্ণাঙ্ক)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			মুক্ত(buf);
			ভঙ্গ;
		পক্ষ REENCODE_NO:
			ভঙ্গ;
		পক্ষ REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (পূর্ণাঙ্ক)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	যদি (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	যদি (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (পূর্ণাঙ্ক)(author_end - author), author,
	       (পূর্ণাঙ্ক)(committer_end - committer), committer);
	যদি (signatures.nr) {
		বিকল্প (signed_commit_mode) {
		/* Exporting modes */
		পক্ষ SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		পক্ষ SIGN_VERBATIM:
			প্রতি (size_t i = 0; i < signatures.nr; i++) {
				সংরচনা string_list_item *item = &signatures.বস্তুসমূহ[i];
				print_signature(item->string, item->util);
			}
			ভঙ্গ;

		/* Stripping modes */
		পক্ষ SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		পক্ষ SIGN_STRIP:
			ভঙ্গ;

		/* Aborting modes */
		পক্ষ SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		স্বভাব:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	যদি (!reencoded && encoding)
		printf("encoding %.*s\n", (পূর্ণাঙ্ক)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	মুক্ত(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	প্রতি (i = 0, p = commit->parents; p; p = p->next) {
		সংরচনা object *obj = &p->item->object;
		পূর্ণাঙ্ক mark = get_object_mark(obj);

		যদি (!mark && !reference_excluded_commits)
			অনুবর্ত;
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

স্থির বর্ণ *anonymize_tag(শূন্য)
{
	স্থির পূর্ণাঙ্ক counter;
	সংরচনা strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	প্রত্যাগম strbuf_detach(&out, রিক্ত);
}


স্থির শূন্য handle_tag(ধ্রুব বর্ণ *name, সংরচনা tag *tag)
{
	size_t size;
	গণনা object_type type;
	বর্ণ *buf;
	ধ্রুব বর্ণ *tagger, *tagger_end, *message;
	size_t message_size = 0;
	সংরচনা object *tagged;
	পূর্ণাঙ্ক tagged_mark;
	সংরচনা commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	যাবত্ (tagged->type == OBJ_TAG) {
		tagged = ((সংরচনা tag *)tagged)->tagged;
	}
	যদি (tagged->type == OBJ_বৃক্ষ) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		প্রত্যাগম;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	যদি (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	যদি (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
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
			স্থির সংরচনা hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	যদি (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		যদি (sig_offset < message_size)
			বিকল্প (signed_tag_mode) {
			/* Exporting modes */
			পক্ষ SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			পক্ষ SIGN_VERBATIM:
				ভঙ্গ;

			/* Stripping modes */
			পক্ষ SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			পক্ষ SIGN_STRIP:
				message_size = sig_offset;
				ভঙ্গ;

			/* Aborting modes */
			পক্ষ SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			স্বভাব:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	যদি (!tagged_mark) {
		বিকল্প (tag_of_filtered_mode) {
		পক্ষ TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		পক্ষ DROP:
			/* Ignore this tag altogether */
			মুক্ত(buf);
			প্রত্যাগম;
		পক্ষ REWRITE:
			যদি (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} অন্যথা যদি (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((সংরচনা commit *)tagged);
				যদি (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					মুক্ত(buf);
					প্রত্যাগম;
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
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	যদি (tagged_mark)
		printf("from :%d\n", tagged_mark);
	অন্যথা
		printf("from %s\n", oid_to_hex(&tagged->oid));

	যদি (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (পূর্ণাঙ্ক)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (পূর্ণাঙ্ক)message_size, (পূর্ণাঙ্ক)message_size, message ? message : "");
	মুক্ত(buf);
}

স্থির সংরচনা commit *get_commit(সংরচনা rev_cmdline_entry *e, ধ্রুব বর্ণ *full_name)
{
	বিকল্প (e->item->type) {
	পক্ষ OBJ_COMMIT:
		প্রত্যাগম (সংরচনা commit *)e->item;
	পক্ষ OBJ_TAG: {
		সংরচনা tag *tag = (সংরচনা tag *)e->item;

		/* handle nested tags */
		যাবত্ (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (সংরচনা tag *)tag->tagged;
		}
		যদি (!tag)
			die(_("tag %s points nowhere?"), e->name);
		প্রত্যাগম (সংরচনা commit *)tag;
	}
	স্বভাব:
		প্রত্যাগম রিক্ত;
	}
}

স্থির শূন্য get_tags_and_duplicates(সংরচনা rev_cmdline_info *info)
{
	পূর্ণাঙ্ক i;

	প্রতি (i = 0; i < info->nr; i++) {
		সংরচনা rev_cmdline_entry *e = info->rev + i;
		সংরচনা object_id oid;
		সংরচনা commit *commit;
		বর্ণ *full_name = রিক্ত;

		যদি (e->flags & UNINTERESTING)
			অনুবর্ত;

		যদি (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			মুক্ত(full_name);
			অনুবর্ত;
		}

		যদি (refspecs.nr) {
			বর্ণ *গুপ্ত;
			গুপ্ত = apply_refspecs(&refspecs, full_name);
			যদি (গুপ্ত) {
				মুক্ত(full_name);
				full_name = গুপ্ত;
			}
		}

		commit = get_commit(e, full_name);
		যদি (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			মুক্ত(full_name);
			অনুবর্ত;
		}

		বিকল্প (commit->object.type) {
		পক্ষ OBJ_COMMIT:
			ভঙ্গ;
		পক্ষ OBJ_BLOB:
			export_blob(&commit->object.oid);
			মুক্ত(full_name);
			অনুবর্ত;
		স্বভাব: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			মুক্ত(full_name);
			অনুবর্ত;
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

স্থির শূন্য handle_tags_and_duplicates(সংরচনা string_list *extras)
{
	সংরচনা commit *commit;
	পূর্ণাঙ্ক i;

	প্রতি (i = extras->nr - 1; i >= 0; i--) {
		ধ্রুব বর্ণ *name = extras->বস্তুসমূহ[i].string;
		সংরচনা object *object = extras->বস্তুসমূহ[i].util;
		পূর্ণাঙ্ক mark;

		বিকল্প (object->type) {
		পক্ষ OBJ_TAG:
			handle_tag(name, (সংরচনা tag *)object);
			ভঙ্গ;
		পক্ষ OBJ_COMMIT:
			যদি (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((সংরচনা commit *)object);
			যদি (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				printf("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				অনুবর্ত;
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
					অনুবর্ত;
				}
				/* set ref to commit using oid, not mark */
				printf("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				অনুবর্ত;
			}

			printf("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			ভঙ্গ;
		}
	}
}

স্থির শূন্য export_marks(বর্ণ *file)
{
	unsigned পূর্ণাঙ্ক i;
	uint32_t mark;
	সংরচনা decoration_entry *deco = idnums.entries;
	FILE *f;
	পূর্ণাঙ্ক e = 0;

	f = fopen_for_writing(file);
	যদি (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	প্রতি (i = 0; i < idnums.size; i++) {
		যদি (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			যদি (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    ভঙ্গ;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	যদি (e)
		error(_("unable to write marks file %s."), file);
}

স্থির শূন্য import_marks(বর্ণ *প্রবেশ_file, পূর্ণাঙ্ক check_exists)
{
	বর্ণ সারি[512];
	FILE *f;
	সংরচনা stat sb;

	যদি (check_exists && stat(প্রবেশ_file, &sb))
		প্রত্যাগম;

	f = xfopen(প্রবেশ_file, "r");
	যাবত্ (fgets(সারি, পরিমাণ(সারি), f)) {
		uint32_t mark;
		বর্ণ *সারি_end, *mark_end;
		সংরচনা object_id oid;
		সংরচনা object *object;
		সংরচনা commit *commit;
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

		type = odb_read_object_info(the_repository->objects, &oid, রিক্ত);
		যদি (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		যদি (type != OBJ_COMMIT)
			/* only commits */
			অনুবর্ত;

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
	পূর্ণাঙ্ক i;
	প্রতি (i = 0; i < refspecs.nr; i++) {
		সংরচনা refspec_item *refspec = &refspecs.বস্তুসমূহ[i];
		যদি (*refspec->src)
			অনুবর্ত;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

স্থির পূর্ণাঙ্ক parse_opt_anonymize_map(ধ্রুব সংরচনা option *opt,
				   ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা hashmap *map = opt->মান;
	ধ্রুব বর্ণ *delim, *মান;
	size_t keylen;

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
		প্রত্যাগম error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(মান));

	প্রত্যাগম 0;
}

পূর্ণাঙ্ক cmd_fast_export(পূর্ণাঙ্ক argc,
		    ধ্রুব বর্ণ **argv,
		    ধ্রুব বর্ণ *prefix,
		    সংরচনা repository *repo UNUSED)
{
	সংরচনা rev_info revs;
	সংরচনা commit *commit;
	বর্ণ *export_filename = রিক্ত,
	     *import_filename = রিক্ত,
	     *import_filename_if_exists = রিক্ত;
	uint32_t lastimportid;
	সংরচনা string_list refspecs_list = STRING_LIST_INIT_NODUP;
	সংরচনা string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	সংরচনা option options[] = {
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
		OPT_BOOL(0, "no-data", &no_data, N_("skip output of blob data")),
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
	repo_config(the_repository, git_default_config, রিক্ত);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, prefix);
	init_revision_sources(&revision_sources);
	revs.topo_order = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, prefix, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = setup_revisions(argc, argv, &revs, রিক্ত);
	যদি (argc > 1)
		usage_with_options (fast_export_usage, options);

	যদি (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	যদি (refspecs_list.nr) {
		পূর্ণাঙ্ক i;

		প্রতি (i = 0; i < refspecs_list.nr; i++)
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

	যদি (import_filename && revs.prune_data.nr)
		full_বৃক্ষ = 1;

	get_tags_and_duplicates(&revs.cmdline);

	যদি (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	যাবত্ ((commit = get_revision(&revs)))
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

	প্রত্যাগম 0;
}
