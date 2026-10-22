/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "refs.h"
#ضمن "refspec.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "commit.h"
#ضمن "object.h"
#ضمن "tag.h"
#ضمن "diff.h"
#ضمن "diffcore.h"
#ضمن "log-tree.h"
#ضمن "revision.h"
#ضمن "decorate.h"
#ضمن "string-list.h"
#ضمن "utf8.h"
#ضمن "parse-options.h"
#ضمن "quote.h"
#ضمن "remote.h"
#ضمن "blob.h"
#ضمن "gpg-interface.h"

ساكن ثابت حرف *ثابت fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	عدم
};

ساكن صحيح progress;
ساكن تعداد sign_mode signed_tag_mode = SIGN_ABORT;
ساكن تعداد sign_mode signed_commit_mode = SIGN_STRIP;
ساكن تعداد tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
ساكن تعداد reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
ساكن صحيح fake_missing_tagger;
ساكن صحيح use_done_feature;
ساكن صحيح no_data;
ساكن صحيح full_بشيا;
ساكن صحيح reference_excluded_commits;
ساكن صحيح show_original_ids;
ساكن صحيح mark_tags;
ساكن بنية string_list extra_refs = STRING_LIST_INIT_DUP;
ساكن بنية string_list tag_refs = STRING_LIST_INIT_DUP;
ساكن بنية refspec refspecs;
ساكن صحيح anonymize;
ساكن بنية hashmap anonymized_seeds;
ساكن بنية revision_sources revision_sources;

ساكن صحيح parse_opt_sign_mode(ثابت بنية option *opt,
			       ثابت حرف *arg, صحيح unset)
{
	تعداد sign_mode *val = opt->درج;

	إذا (unset)
		أرجع 0;

	إذا (parse_sign_mode(arg, val, عدم) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		أرجع error(_("unknown %s mode: %s"), opt->long_name, arg);

	أرجع 0;
}

ساكن صحيح parse_opt_tag_of_filtered_mode(ثابت بنية option *opt,
					  ثابت حرف *arg, صحيح unset)
{
	تعداد tag_of_filtered_mode *val = opt->درج;

	إذا (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	وإلا إذا (!strcmp(arg, "drop"))
		*val = DROP;
	وإلا إذا (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	وإلا
		أرجع error(_("unknown tag-of-filtered mode: %s"), arg);
	أرجع 0;
}

ساكن صحيح parse_opt_reencode_mode(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	تعداد reencode_mode *val = opt->درج;

	إذا (unset) {
		*val = REENCODE_ABORT;
		أرجع 0;
	}

	بدل (git_parse_maybe_bool(arg)) {
	حالة 0:
		*val = REENCODE_NO;
		اكسر;
	حالة 1:
		*val = REENCODE_YES;
		اكسر;
	افتراضي:
		إذا (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		وإلا
			أرجع error(_("unknown reencoding mode: %s"), arg);
	}

	أرجع 0;
}

ساكن بنية decoration idnums;
ساكن uint32_t last_idnum;
بنية anonymized_entry {
	بنية hashmap_entry hash;
	حرف *anon;
	ثابت حرف orig[FLEX_ARRAY];
};

بنية anonymized_entry_مبودي {
	بنية hashmap_entry hash;
	ثابت حرف *orig;
	size_t orig_len;
};

ساكن صحيح anonymized_entry_cmp(ثابت فراغ *cmp_data UNUSED,
				ثابت بنية hashmap_entry *eptr,
				ثابت بنية hashmap_entry *entry_or_مبودي,
				ثابت فراغ *keydata)
{
	ثابت بنية anonymized_entry *a, *b;

	a = container_of(eptr, ثابت بنية anonymized_entry, hash);
	إذا (keydata) {
		ثابت بنية anonymized_entry_مبودي *مبودي = keydata;
		صحيح equal = !xstrncmpz(a->orig, مبودي->orig, مبودي->orig_len);
		أرجع !equal;
	}

	b = container_of(entry_or_مبودي, ثابت بنية anonymized_entry, hash);
	أرجع strcmp(a->orig, b->orig);
}

ساكن بنية anonymized_entry *add_anonymized_entry(بنية hashmap *map,
						     unsigned hash,
						     ثابت حرف *orig, size_t len,
						     حرف *anon)
{
	بنية anonymized_entry *ret, *old;

	إذا (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, عدم, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	إذا (old) {
		حرر(old->anon);
		حرر(old);
	}

	أرجع ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
ساكن ثابت حرف *anonymize_str(بنية hashmap *map,
				 حرف *(*generate)(فراغ),
				 ثابت حرف *orig, size_t len)
{
	بنية anonymized_entry_مبودي مبودي;
	بنية anonymized_entry *ret;

	hashmap_entry_init(&مبودي.hash, memhash(orig, len));
	مبودي.orig = orig;
	مبودي.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &مبودي, hash, &مبودي);

	/* ...otherwise check if we've already seen it in this context... */
	إذا (!ret)
		ret = hashmap_get_entry(map, &مبودي, hash, &مبودي);

	/* ...and finally generate a new mapping if necessary */
	إذا (!ret)
		ret = add_anonymized_entry(map, مبودي.hash.hash,
					   orig, len, generate());

	أرجع ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
ساكن فراغ anonymize_path(بنية strbuf *out, ثابت حرف *path,
			   بنية hashmap *map,
			   حرف *(*generate)(فراغ))
{
	طالما (*path) {
		ثابت حرف *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		ثابت حرف *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		إذا (*path)
			strbuf_addch(out, *path++);
	}
}

ساكن مضمن فراغ *mark_to_ptr(uint32_t mark)
{
	أرجع (فراغ *)(uintptr_t)mark;
}

ساكن مضمن uint32_t ptr_to_mark(فراغ * mark)
{
	أرجع (uint32_t)(uintptr_t)mark;
}

ساكن مضمن فراغ mark_object(بنية object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

ساكن مضمن فراغ mark_next_object(بنية object *object)
{
	mark_object(object, ++last_idnum);
}

ساكن صحيح get_object_mark(بنية object *object)
{
	فراغ *decoration = lookup_decoration(&idnums, object);
	إذا (!decoration)
		أرجع 0;
	أرجع ptr_to_mark(decoration);
}

ساكن بنية commit *rewrite_commit(بنية commit *p)
{
	لكل (;;) {
		إذا (p->parents && p->parents->next)
			اكسر;
		إذا (p->object.flags & UNINTERESTING)
			اكسر;
		إذا (!(p->object.flags & TREESAME))
			اكسر;
		إذا (!p->parents)
			أرجع عدم;
		p = p->parents->item;
	}
	أرجع p;
}

ساكن فراغ show_progress(فراغ)
{
	ساكن صحيح counter = 0;
	إذا (!progress)
		أرجع;
	إذا ((++counter % progress) == 0)
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
ساكن حرف *anonymize_blob(unsigned long *size)
{
	ساكن صحيح counter;
	بنية strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	أرجع strbuf_detach(&out, عدم);
}

ساكن فراغ export_blob(ثابت بنية object_id *oid)
{
	unsigned long size;
	تعداد object_type type;
	حرف *buf;
	بنية object *object;
	صحيح eaten;

	إذا (no_data)
		أرجع;

	إذا (is_null_oid(oid))
		أرجع;

	object = lookup_object(the_repository, oid);
	إذا (object && object->flags & SHOWN)
		أرجع;

	إذا (anonymize) {
		buf = anonymize_blob(&size);
		object = (بنية object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} وإلا {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		إذا (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		إذا (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	إذا (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	إذا (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	إذا (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	إذا (!eaten)
		حرر(buf);
}

ساكن صحيح depth_first(ثابت فراغ *a_, ثابت فراغ *b_)
{
	ثابت بنية diff_filepair *a = *((ثابت بنية diff_filepair **)a_);
	ثابت بنية diff_filepair *b = *((ثابت بنية diff_filepair **)b_);
	ثابت حرف *name_a, *name_b;
	صحيح len_a, len_b, len;
	صحيح cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	إذا (cmp)
		أرجع cmp;
	cmp = len_b - len_a;
	إذا (cmp)
		أرجع cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	أرجع (a->status == 'R') - (b->status == 'R');
}

ساكن فراغ بغ_path_1(ثابت حرف *path)
{
	صحيح need_quote = quote_c_style(path, عدم, عدم, 0);
	إذا (need_quote)
		quote_c_style(path, عدم, stdout, 0);
	وإلا إذا (strchr(path, ' '))
		printf("\"%s\"", path);
	وإلا
		printf("%s", path);
}

ساكن حرف *anonymize_path_component(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	أرجع strbuf_detach(&out, عدم);
}

ساكن فراغ بغ_path(ثابت حرف *path)
{
	إذا (!anonymize)
		بغ_path_1(path);
	وإلا {
		ساكن بنية hashmap paths;
		ساكن بنية strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		بغ_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

ساكن حرف *generate_fake_oid(فراغ)
{
	ساكن uint32_t counter = 1; /* avoid null oid */
	ثابت unsigned hashsz = the_hash_algo->rawsz;
	بنية object_id oid;
	حرف *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	أرجع oid_to_hex_r(hex, &oid);
}

ساكن ثابت حرف *anonymize_oid(ثابت حرف *oid_hex)
{
	ساكن بنية hashmap objs;
	size_t len = strlen(oid_hex);
	أرجع anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

ساكن فراغ show_filemodify(بنية diff_queue_struct *q,
			    بنية diff_options *options UNUSED, فراغ *data)
{
	صحيح i;
	بنية string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	لكل (i = 0; i < q->nr; i++) {
		بنية diff_filespec *ospec = q->queue[i]->one;
		بنية diff_filespec *spec = q->queue[i]->two;

		بدل (q->queue[i]->status) {
		حالة DIFF_STATUS_DELETED:
			printf("D ");
			بغ_path(spec->path);
			string_list_سك(changed, spec->path);
			putchar('\n');
			اكسر;

		حالة DIFF_STATUS_COPIED:
		حالة DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			إذا (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				بغ_path(ospec->path);
				putchar(' ');
				بغ_path(spec->path);
				string_list_سك(changed, spec->path);
				putchar('\n');

				إذا (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					اكسر;
			}
			/* fallthrough */

		حالة DIFF_STATUS_TYPE_CHANGED:
		حالة DIFF_STATUS_MODIFIED:
		حالة DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			إذا (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			وإلا {
				بنية object *object = lookup_object(the_repository,
								      &spec->oid);
				printf("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			بغ_path(spec->path);
			string_list_سك(changed, spec->path);
			putchar('\n');
			اكسر;

		افتراضي:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

ساكن حرف *anonymize_ref_component(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	أرجع strbuf_detach(&out, عدم);
}

ساكن ثابت حرف *anonymize_refname(ثابت حرف *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	ساكن ثابت حرف *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	ساكن بنية hashmap refs;
	ساكن بنية strbuf anon = STRBUF_INIT;
	صحيح i;

	strbuf_reset(&anon);
	لكل (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		إذا (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			اكسر;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	أرجع anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
ساكن حرف *anonymize_commit_message(فراغ)
{
	ساكن صحيح counter;
	أرجع xstrfmt("subject %d\n\nbody\n", counter++);
}

ساكن حرف *anonymize_ident(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	أرجع strbuf_detach(&out, عدم);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
ساكن فراغ anonymize_ident_لي(ثابت حرف **beg, ثابت حرف **end)
{
	ساكن بنية hashmap idents;
	ساكن بنية strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	ساكن unsigned which_buffer;

	بنية strbuf *out;
	بنية ident_split split;
	ثابت حرف *end_of_header;

	out = &buffers[which_buffer++];
	which_buffer %= ARRAY_SIZE(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = strchr(*beg, ' ');
	إذا (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (صحيح)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	إذا (!split_ident_لي(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		ثابت حرف *ident;
		size_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} وإلا {
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
ساكن ثابت حرف *find_commit_multiline_header(ثابت حرف *سكو,
						ثابت حرف *مبودي,
						ثابت حرف **end)
{
	بنية strbuf val = STRBUF_INIT;
	ثابت حرف *bol, *eol;
	size_t len;

	bol = find_commit_header(سكو, مبودي, &len);
	إذا (!bol)
		أرجع عدم;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	طالما (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	أرجع strbuf_detach(&val, عدم);
}

ساكن فراغ بغ_signature(ثابت حرف *signature, ثابت حرف *object_hash)
{
	إذا (!signature)
		أرجع;

	printf("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (unsigned)strlen(signature),
	       signature);
}

ساكن ثابت حرف *append_signatures_for_header(بنية string_list *signatures,
						ثابت حرف *pos,
						ثابت حرف *header,
						ثابت حرف *object_hash)
{
	ثابت حرف *signature;
	ثابت حرف *start = pos;
	ثابت حرف *end = pos;

	طالما ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (فراغ *)object_hash;
		حرر((حرف *)signature);
		start = end;
	}

	أرجع end;
}

ساكن فراغ handle_commit(بنية commit *commit, بنية rev_info *rev,
			  بنية string_list *paths_of_changed_objects)
{
	صحيح saved_output_format = rev->diffopt.output_format;
	ثابت حرف *commit_buffer, *commit_buffer_cursor;
	ثابت حرف *author, *author_end, *committer, *committer_end;
	ثابت حرف *encoding = عدم;
	size_t encoding_len;
	بنية string_list signatures = STRING_LIST_INIT_DUP;
	ثابت حرف *message;
	حرف *reencoded = عدم;
	بنية commit_list *p;
	ثابت حرف *refname;
	صحيح i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, عدم);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	إذا (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	إذا (!committer)
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

	إذا (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		إذا (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	إذا (*commit_buffer_cursor == '\n') {
		ثابت حرف *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		ثابت حرف *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
	إذا (message)
		message += 2;

	إذا (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_بشيا) {
		parse_commit_or_die(commit->parents->item);
		diff_بشيا_oid(get_commit_بشيا_oid(commit->parents->item),
			      get_commit_بشيا_oid(commit), "", &rev->diffopt);
	}
	وإلا
		diff_سيو_بشيا_oid(get_commit_بشيا_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	لكل (i = 0; i < diff_queued_diff.nr; i++)
		إذا (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	إذا (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_لي(&committer, &committer_end);
		anonymize_ident_لي(&author, &author_end);
	}

	mark_next_object(&commit->object);
	إذا (anonymize) {
		reencoded = anonymize_commit_message();
	} وإلا إذا (encoding) {
		حرف *buf;
		بدل (reencode_mode) {
		حالة REENCODE_YES:
			buf = xstrfmt("%.*s", (صحيح)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			حرر(buf);
			اكسر;
		حالة REENCODE_NO:
			اكسر;
		حالة REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (صحيح)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	إذا (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	إذا (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (صحيح)(author_end - author), author,
	       (صحيح)(committer_end - committer), committer);
	إذا (signatures.nr) {
		بدل (signed_commit_mode) {
		/* Exporting modes */
		حالة SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		حالة SIGN_VERBATIM:
			لكل (size_t i = 0; i < signatures.nr; i++) {
				بنية string_list_item *item = &signatures.ابوبو[i];
				بغ_signature(item->string, item->util);
			}
			اكسر;

		/* Stripping modes */
		حالة SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		حالة SIGN_STRIP:
			اكسر;

		/* Aborting modes */
		حالة SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		افتراضي:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	إذا (!reencoded && encoding)
		printf("encoding %.*s\n", (صحيح)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	حرر(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	لكل (i = 0, p = commit->parents; p; p = p->next) {
		بنية object *obj = &p->item->object;
		صحيح mark = get_object_mark(obj);

		إذا (!mark && !reference_excluded_commits)
			استمر;
		إذا (i == 0)
			printf("from ");
		وإلا
			printf("merge ");
		إذا (mark)
			printf(":%d\n", mark);
		وإلا
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	إذا (full_بشيا)
		printf("deleteall\n");
	log_بشيا_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

ساكن حرف *anonymize_tag(فراغ)
{
	ساكن صحيح counter;
	بنية strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	أرجع strbuf_detach(&out, عدم);
}


ساكن فراغ handle_tag(ثابت حرف *name, بنية tag *tag)
{
	size_t size;
	تعداد object_type type;
	حرف *buf;
	ثابت حرف *tagger, *tagger_end, *message;
	size_t message_size = 0;
	بنية object *tagged;
	صحيح tagged_mark;
	بنية commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	طالما (tagged->type == OBJ_TAG) {
		tagged = ((بنية tag *)tagged)->tagged;
	}
	إذا (tagged->type == OBJ_بشيا) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		أرجع;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	إذا (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	إذا (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	إذا (!tagger) {
		إذا (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		وإلا
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} وإلا {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		إذا (anonymize)
			anonymize_ident_لي(&tagger, &tagger_end);
	}

	إذا (anonymize) {
		name = anonymize_refname(name);
		إذا (message) {
			ساكن بنية hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	إذا (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		إذا (sig_offset < message_size)
			بدل (signed_tag_mode) {
			/* Exporting modes */
			حالة SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			حالة SIGN_VERBATIM:
				اكسر;

			/* Stripping modes */
			حالة SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			حالة SIGN_STRIP:
				message_size = sig_offset;
				اكسر;

			/* Aborting modes */
			حالة SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			افتراضي:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	إذا (!tagged_mark) {
		بدل (tag_of_filtered_mode) {
		حالة TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		حالة DROP:
			/* Ignore this tag altogether */
			حرر(buf);
			أرجع;
		حالة REWRITE:
			إذا (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} وإلا إذا (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((بنية commit *)tagged);
				إذا (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					حرر(buf);
					أرجع;
				}
				tagged_mark = get_object_mark(&p->object);
			} وإلا {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	إذا (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	إذا (mark_tags) {
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	إذا (tagged_mark)
		printf("from :%d\n", tagged_mark);
	وإلا
		printf("from %s\n", oid_to_hex(&tagged->oid));

	إذا (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (صحيح)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (صحيح)message_size, (صحيح)message_size, message ? message : "");
	حرر(buf);
}

ساكن بنية commit *get_commit(بنية rev_cmdline_entry *e, ثابت حرف *full_name)
{
	بدل (e->item->type) {
	حالة OBJ_COMMIT:
		أرجع (بنية commit *)e->item;
	حالة OBJ_TAG: {
		بنية tag *tag = (بنية tag *)e->item;

		/* handle nested tags */
		طالما (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (بنية tag *)tag->tagged;
		}
		إذا (!tag)
			die(_("tag %s points nowhere?"), e->name);
		أرجع (بنية commit *)tag;
	}
	افتراضي:
		أرجع عدم;
	}
}

ساكن فراغ get_tags_and_duplicates(بنية rev_cmdline_info *info)
{
	صحيح i;

	لكل (i = 0; i < info->nr; i++) {
		بنية rev_cmdline_entry *e = info->rev + i;
		بنية object_id oid;
		بنية commit *commit;
		حرف *full_name = عدم;

		إذا (e->flags & UNINTERESTING)
			استمر;

		إذا (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			حرر(full_name);
			استمر;
		}

		إذا (refspecs.nr) {
			حرف *خاص;
			خاص = apply_refspecs(&refspecs, full_name);
			إذا (خاص) {
				حرر(full_name);
				full_name = خاص;
			}
		}

		commit = get_commit(e, full_name);
		إذا (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			حرر(full_name);
			استمر;
		}

		بدل (commit->object.type) {
		حالة OBJ_COMMIT:
			اكسر;
		حالة OBJ_BLOB:
			export_blob(&commit->object.oid);
			حرر(full_name);
			استمر;
		افتراضي: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			حرر(full_name);
			استمر;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		إذا (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		إذا (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		وإلا
			حرر(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

ساكن فراغ handle_tags_and_duplicates(بنية string_list *extras)
{
	بنية commit *commit;
	صحيح i;

	لكل (i = extras->nr - 1; i >= 0; i--) {
		ثابت حرف *name = extras->ابوبو[i].string;
		بنية object *object = extras->ابوبو[i].util;
		صحيح mark;

		بدل (object->type) {
		حالة OBJ_TAG:
			handle_tag(name, (بنية tag *)object);
			اكسر;
		حالة OBJ_COMMIT:
			إذا (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((بنية commit *)object);
			إذا (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				printf("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				استمر;
			}

			mark = get_object_mark(&commit->object);
			إذا (!mark) {
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
				إذا (!reference_excluded_commits) {
					/* delete the ref */
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					استمر;
				}
				/* set ref to commit using oid, not mark */
				printf("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				استمر;
			}

			printf("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			اكسر;
		}
	}
}

ساكن فراغ export_marks(حرف *file)
{
	unsigned صحيح i;
	uint32_t mark;
	بنية decoration_entry *deco = idnums.entries;
	FILE *f;
	صحيح e = 0;

	f = fopen_for_writing(file);
	إذا (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	لكل (i = 0; i < idnums.size; i++) {
		إذا (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			إذا (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    اكسر;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	إذا (e)
		error(_("unable to write marks file %s."), file);
}

ساكن فراغ import_marks(حرف *شغرو_file, صحيح check_exists)
{
	حرف لي[512];
	FILE *f;
	بنية stat sb;

	إذا (check_exists && stat(شغرو_file, &sb))
		أرجع;

	f = xfopen(شغرو_file, "r");
	طالما (fgets(لي, حجم(لي), f)) {
		uint32_t mark;
		حرف *لي_end, *mark_end;
		بنية object_id oid;
		بنية object *object;
		بنية commit *commit;
		تعداد object_type type;

		لي_end = strchr(لي, '\n');
		إذا (لي[0] != ':' || !لي_end)
			die(_("corrupt mark line: %s"), لي);
		*لي_end = '\0';

		mark = strtoumax(لي + 1, &mark_end, 10);
		إذا (!mark || mark_end == لي + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), لي);

		إذا (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, عدم);
		إذا (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		إذا (type != OBJ_COMMIT)
			/* only commits */
			استمر;

		commit = lookup_commit(the_repository, &oid);
		إذا (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		إذا (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

ساكن فراغ handle_deletes(فراغ)
{
	صحيح i;
	لكل (i = 0; i < refspecs.nr; i++) {
		بنية refspec_item *refspec = &refspecs.ابوبو[i];
		إذا (*refspec->src)
			استمر;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

ساكن صحيح parse_opt_anonymize_map(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	بنية hashmap *map = opt->درج;
	ثابت حرف *delim, *درج;
	size_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	إذا (delim) {
		keylen = delim - arg;
		درج = delim + 1;
	} وإلا {
		keylen = strlen(arg);
		درج = arg;
	}

	إذا (!keylen || !*درج)
		أرجع error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(درج));

	أرجع 0;
}

صحيح cmd_fast_export(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *prefix,
		    بنية repository *repo UNUSED)
{
	بنية rev_info revs;
	بنية commit *commit;
	حرف *export_filename = عدم,
	     *import_filename = عدم,
	     *import_filename_if_exists = عدم;
	uint32_t lastimportid;
	بنية string_list refspecs_list = STRING_LIST_INIT_NODUP;
	بنية string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	بنية option options[] = {
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
		OPT_BOOL(0, "full-tree", &full_بشيا,
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

	إذا (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, عدم);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, prefix);
	init_revision_sources(&revision_sources);
	revs.topo_order = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, prefix, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = setup_revisions(argc, argv, &revs, عدم);
	إذا (argc > 1)
		usage_with_options (fast_export_usage, options);

	إذا (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	إذا (refspecs_list.nr) {
		صحيح i;

		لكل (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.ابوبو[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	إذا (use_done_feature)
		printf("feature done\n");

	إذا (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	إذا (import_filename)
		import_marks(import_filename, 0);
	وإلا إذا (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	إذا (import_filename && revs.prune_data.nr)
		full_بشيا = 1;

	get_tags_and_duplicates(&revs.cmdline);

	إذا (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	طالما ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	إذا (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	إذا (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	أرجع 0;
}
