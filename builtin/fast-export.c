/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "refs.h"
#شامل "refspec.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "commit.h"
#شامل "object.h"
#شامل "tag.h"
#شامل "diff.h"
#شامل "diffcore.h"
#شامل "log-tree.h"
#شامل "revision.h"
#شامل "decorate.h"
#شامل "string-list.h"
#شامل "utf8.h"
#شامل "parse-options.h"
#شامل "quote.h"
#شامل "remote.h"
#شامل "blob.h"
#شامل "gpg-interface.h"

ایستا ثابت نویسه *ثابت fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	پوچ
};

ایستا صحیح progress;
ایستا شمارش sign_mode signed_tag_mode = SIGN_ABORT;
ایستا شمارش sign_mode signed_commit_mode = SIGN_STRIP;
ایستا شمارش tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
ایستا شمارش reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
ایستا صحیح fake_missing_tagger;
ایستا صحیح use_done_feature;
ایستا صحیح no_data;
ایستا صحیح full_درخت;
ایستا صحیح reference_excluded_commits;
ایستا صحیح show_original_ids;
ایستا صحیح mark_tags;
ایستا ساختار string_list extra_refs = STRING_LIST_INIT_DUP;
ایستا ساختار string_list tag_refs = STRING_LIST_INIT_DUP;
ایستا ساختار refspec refspecs;
ایستا صحیح anonymize;
ایستا ساختار hashmap anonymized_seeds;
ایستا ساختار revision_sources revision_sources;

ایستا صحیح parse_opt_sign_mode(ثابت ساختار option *opt,
			       ثابت نویسه *arg, صحیح unset)
{
	شمارش sign_mode *val = opt->مقدار;

	اگر (unset)
		بازگشت 0;

	اگر (parse_sign_mode(arg, val, پوچ) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		بازگشت error(_("unknown %s mode: %s"), opt->long_name, arg);

	بازگشت 0;
}

ایستا صحیح parse_opt_tag_of_filtered_mode(ثابت ساختار option *opt,
					  ثابت نویسه *arg, صحیح unset)
{
	شمارش tag_of_filtered_mode *val = opt->مقدار;

	اگر (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	وگرنه اگر (!strcmp(arg, "drop"))
		*val = DROP;
	وگرنه اگر (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	وگرنه
		بازگشت error(_("unknown tag-of-filtered mode: %s"), arg);
	بازگشت 0;
}

ایستا صحیح parse_opt_reencode_mode(ثابت ساختار option *opt,
				   ثابت نویسه *arg, صحیح unset)
{
	شمارش reencode_mode *val = opt->مقدار;

	اگر (unset) {
		*val = REENCODE_ABORT;
		بازگشت 0;
	}

	انتخاب (git_parse_maybe_bool(arg)) {
	حالت 0:
		*val = REENCODE_NO;
		بشکن;
	حالت 1:
		*val = REENCODE_YES;
		بشکن;
	پیشفرض:
		اگر (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		وگرنه
			بازگشت error(_("unknown reencoding mode: %s"), arg);
	}

	بازگشت 0;
}

ایستا ساختار decoration idnums;
ایستا uint32_t last_idnum;
ساختار anonymized_entry {
	ساختار hashmap_entry hash;
	نویسه *anon;
	ثابت نویسه orig[FLEX_ARRAY];
};

ساختار anonymized_entry_کلید {
	ساختار hashmap_entry hash;
	ثابت نویسه *orig;
	size_t orig_len;
};

ایستا صحیح anonymized_entry_cmp(ثابت تهی *cmp_data UNUSED,
				ثابت ساختار hashmap_entry *eptr,
				ثابت ساختار hashmap_entry *entry_or_کلید,
				ثابت تهی *keydata)
{
	ثابت ساختار anonymized_entry *a, *b;

	a = container_of(eptr, ثابت ساختار anonymized_entry, hash);
	اگر (keydata) {
		ثابت ساختار anonymized_entry_کلید *کلید = keydata;
		صحیح equal = !xstrncmpz(a->orig, کلید->orig, کلید->orig_len);
		بازگشت !equal;
	}

	b = container_of(entry_or_کلید, ثابت ساختار anonymized_entry, hash);
	بازگشت strcmp(a->orig, b->orig);
}

ایستا ساختار anonymized_entry *add_anonymized_entry(ساختار hashmap *map,
						     unsigned hash,
						     ثابت نویسه *orig, size_t len,
						     نویسه *anon)
{
	ساختار anonymized_entry *ret, *old;

	اگر (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, پوچ, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	اگر (old) {
		آزاد(old->anon);
		آزاد(old);
	}

	بازگشت ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
ایستا ثابت نویسه *anonymize_str(ساختار hashmap *map,
				 نویسه *(*generate)(تهی),
				 ثابت نویسه *orig, size_t len)
{
	ساختار anonymized_entry_کلید کلید;
	ساختار anonymized_entry *ret;

	hashmap_entry_init(&کلید.hash, memhash(orig, len));
	کلید.orig = orig;
	کلید.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &کلید, hash, &کلید);

	/* ...otherwise check if we've already seen it in this context... */
	اگر (!ret)
		ret = hashmap_get_entry(map, &کلید, hash, &کلید);

	/* ...and finally generate a new mapping if necessary */
	اگر (!ret)
		ret = add_anonymized_entry(map, کلید.hash.hash,
					   orig, len, generate());

	بازگشت ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
ایستا تهی anonymize_path(ساختار strbuf *out, ثابت نویسه *path,
			   ساختار hashmap *map,
			   نویسه *(*generate)(تهی))
{
	وقتی (*path) {
		ثابت نویسه *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		ثابت نویسه *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		اگر (*path)
			strbuf_addch(out, *path++);
	}
}

ایستا درونخطی تهی *mark_to_ptr(uint32_t mark)
{
	بازگشت (تهی *)(uintptr_t)mark;
}

ایستا درونخطی uint32_t ptr_to_mark(تهی * mark)
{
	بازگشت (uint32_t)(uintptr_t)mark;
}

ایستا درونخطی تهی mark_object(ساختار object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

ایستا درونخطی تهی mark_next_object(ساختار object *object)
{
	mark_object(object, ++last_idnum);
}

ایستا صحیح get_object_mark(ساختار object *object)
{
	تهی *decoration = lookup_decoration(&idnums, object);
	اگر (!decoration)
		بازگشت 0;
	بازگشت ptr_to_mark(decoration);
}

ایستا ساختار commit *rewrite_commit(ساختار commit *p)
{
	برای (;;) {
		اگر (p->parents && p->parents->next)
			بشکن;
		اگر (p->object.flags & UNINTERESTING)
			بشکن;
		اگر (!(p->object.flags & TREESAME))
			بشکن;
		اگر (!p->parents)
			بازگشت پوچ;
		p = p->parents->item;
	}
	بازگشت p;
}

ایستا تهی show_progress(تهی)
{
	ایستا صحیح counter = 0;
	اگر (!progress)
		بازگشت;
	اگر ((++counter % progress) == 0)
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
ایستا نویسه *anonymize_blob(unsigned long *size)
{
	ایستا صحیح counter;
	ساختار strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	بازگشت strbuf_detach(&out, پوچ);
}

ایستا تهی export_blob(ثابت ساختار object_id *oid)
{
	unsigned long size;
	شمارش object_type type;
	نویسه *buf;
	ساختار object *object;
	صحیح eaten;

	اگر (no_data)
		بازگشت;

	اگر (is_null_oid(oid))
		بازگشت;

	object = lookup_object(the_repository, oid);
	اگر (object && object->flags & SHOWN)
		بازگشت;

	اگر (anonymize) {
		buf = anonymize_blob(&size);
		object = (ساختار object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} وگرنه {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		اگر (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		اگر (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	اگر (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	اگر (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	اگر (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	اگر (!eaten)
		آزاد(buf);
}

ایستا صحیح depth_first(ثابت تهی *a_, ثابت تهی *b_)
{
	ثابت ساختار diff_filepair *a = *((ثابت ساختار diff_filepair **)a_);
	ثابت ساختار diff_filepair *b = *((ثابت ساختار diff_filepair **)b_);
	ثابت نویسه *name_a, *name_b;
	صحیح len_a, len_b, len;
	صحیح cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	اگر (cmp)
		بازگشت cmp;
	cmp = len_b - len_a;
	اگر (cmp)
		بازگشت cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	بازگشت (a->status == 'R') - (b->status == 'R');
}

ایستا تهی print_path_1(ثابت نویسه *path)
{
	صحیح need_quote = quote_c_style(path, پوچ, پوچ, 0);
	اگر (need_quote)
		quote_c_style(path, پوچ, stdout, 0);
	وگرنه اگر (strchr(path, ' '))
		printf("\"%s\"", path);
	وگرنه
		printf("%s", path);
}

ایستا نویسه *anonymize_path_component(تهی)
{
	ایستا صحیح counter;
	ساختار strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	بازگشت strbuf_detach(&out, پوچ);
}

ایستا تهی print_path(ثابت نویسه *path)
{
	اگر (!anonymize)
		print_path_1(path);
	وگرنه {
		ایستا ساختار hashmap paths;
		ایستا ساختار strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

ایستا نویسه *generate_fake_oid(تهی)
{
	ایستا uint32_t counter = 1; /* avoid null oid */
	ثابت unsigned hashsz = the_hash_algo->rawsz;
	ساختار object_id oid;
	نویسه *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	بازگشت oid_to_hex_r(hex, &oid);
}

ایستا ثابت نویسه *anonymize_oid(ثابت نویسه *oid_hex)
{
	ایستا ساختار hashmap objs;
	size_t len = strlen(oid_hex);
	بازگشت anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

ایستا تهی show_filemodify(ساختار diff_queue_struct *q,
			    ساختار diff_options *options UNUSED, تهی *data)
{
	صحیح i;
	ساختار string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	برای (i = 0; i < q->nr; i++) {
		ساختار diff_filespec *ospec = q->queue[i]->one;
		ساختار diff_filespec *spec = q->queue[i]->two;

		انتخاب (q->queue[i]->status) {
		حالت DIFF_STATUS_DELETED:
			printf("D ");
			print_path(spec->path);
			string_list_درج(changed, spec->path);
			putchar('\n');
			بشکن;

		حالت DIFF_STATUS_COPIED:
		حالت DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			اگر (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				print_path(ospec->path);
				putchar(' ');
				print_path(spec->path);
				string_list_درج(changed, spec->path);
				putchar('\n');

				اگر (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					بشکن;
			}
			/* fallthrough */

		حالت DIFF_STATUS_TYPE_CHANGED:
		حالت DIFF_STATUS_MODIFIED:
		حالت DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			اگر (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			وگرنه {
				ساختار object *object = lookup_object(the_repository,
								      &spec->oid);
				printf("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_درج(changed, spec->path);
			putchar('\n');
			بشکن;

		پیشفرض:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

ایستا نویسه *anonymize_ref_component(تهی)
{
	ایستا صحیح counter;
	ساختار strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	بازگشت strbuf_detach(&out, پوچ);
}

ایستا ثابت نویسه *anonymize_refname(ثابت نویسه *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	ایستا ثابت نویسه *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	ایستا ساختار hashmap refs;
	ایستا ساختار strbuf anon = STRBUF_INIT;
	صحیح i;

	strbuf_reset(&anon);
	برای (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		اگر (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			بشکن;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	بازگشت anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
ایستا نویسه *anonymize_commit_message(تهی)
{
	ایستا صحیح counter;
	بازگشت xstrfmt("subject %d\n\nbody\n", counter++);
}

ایستا نویسه *anonymize_ident(تهی)
{
	ایستا صحیح counter;
	ساختار strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	بازگشت strbuf_detach(&out, پوچ);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
ایستا تهی anonymize_ident_سطر(ثابت نویسه **beg, ثابت نویسه **end)
{
	ایستا ساختار hashmap idents;
	ایستا ساختار strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	ایستا unsigned which_buffer;

	ساختار strbuf *out;
	ساختار ident_split split;
	ثابت نویسه *end_of_header;

	out = &buffers[which_buffer++];
	which_buffer %= ARRAY_SIZE(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = strchr(*beg, ' ');
	اگر (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (صحیح)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	اگر (!split_ident_سطر(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		ثابت نویسه *ident;
		size_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} وگرنه {
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
ایستا ثابت نویسه *find_commit_multiline_header(ثابت نویسه *پیام,
						ثابت نویسه *کلید,
						ثابت نویسه **end)
{
	ساختار strbuf val = STRBUF_INIT;
	ثابت نویسه *bol, *eol;
	size_t len;

	bol = find_commit_header(پیام, کلید, &len);
	اگر (!bol)
		بازگشت پوچ;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	وقتی (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	بازگشت strbuf_detach(&val, پوچ);
}

ایستا تهی print_signature(ثابت نویسه *signature, ثابت نویسه *object_hash)
{
	اگر (!signature)
		بازگشت;

	printf("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (unsigned)strlen(signature),
	       signature);
}

ایستا ثابت نویسه *append_signatures_for_header(ساختار string_list *signatures,
						ثابت نویسه *pos,
						ثابت نویسه *header,
						ثابت نویسه *object_hash)
{
	ثابت نویسه *signature;
	ثابت نویسه *start = pos;
	ثابت نویسه *end = pos;

	وقتی ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (تهی *)object_hash;
		آزاد((نویسه *)signature);
		start = end;
	}

	بازگشت end;
}

ایستا تهی handle_commit(ساختار commit *commit, ساختار rev_info *rev,
			  ساختار string_list *paths_of_changed_objects)
{
	صحیح saved_output_format = rev->diffopt.output_format;
	ثابت نویسه *commit_buffer, *commit_buffer_cursor;
	ثابت نویسه *author, *author_end, *committer, *committer_end;
	ثابت نویسه *encoding = پوچ;
	size_t encoding_len;
	ساختار string_list signatures = STRING_LIST_INIT_DUP;
	ثابت نویسه *message;
	نویسه *reencoded = پوچ;
	ساختار commit_list *p;
	ثابت نویسه *refname;
	صحیح i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, پوچ);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	اگر (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	اگر (!committer)
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

	اگر (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		اگر (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	اگر (*commit_buffer_cursor == '\n') {
		ثابت نویسه *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		ثابت نویسه *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
	اگر (message)
		message += 2;

	اگر (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_درخت) {
		parse_commit_or_die(commit->parents->item);
		diff_درخت_oid(get_commit_درخت_oid(commit->parents->item),
			      get_commit_درخت_oid(commit), "", &rev->diffopt);
	}
	وگرنه
		diff_ریشه_درخت_oid(get_commit_درخت_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	برای (i = 0; i < diff_queued_diff.nr; i++)
		اگر (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	اگر (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_سطر(&committer, &committer_end);
		anonymize_ident_سطر(&author, &author_end);
	}

	mark_next_object(&commit->object);
	اگر (anonymize) {
		reencoded = anonymize_commit_message();
	} وگرنه اگر (encoding) {
		نویسه *buf;
		انتخاب (reencode_mode) {
		حالت REENCODE_YES:
			buf = xstrfmt("%.*s", (صحیح)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			آزاد(buf);
			بشکن;
		حالت REENCODE_NO:
			بشکن;
		حالت REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (صحیح)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	اگر (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	اگر (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (صحیح)(author_end - author), author,
	       (صحیح)(committer_end - committer), committer);
	اگر (signatures.nr) {
		انتخاب (signed_commit_mode) {
		/* Exporting modes */
		حالت SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		حالت SIGN_VERBATIM:
			برای (size_t i = 0; i < signatures.nr; i++) {
				ساختار string_list_item *item = &signatures.اقلام[i];
				print_signature(item->string, item->util);
			}
			بشکن;

		/* Stripping modes */
		حالت SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		حالت SIGN_STRIP:
			بشکن;

		/* Aborting modes */
		حالت SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		پیشفرض:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	اگر (!reencoded && encoding)
		printf("encoding %.*s\n", (صحیح)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	آزاد(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	برای (i = 0, p = commit->parents; p; p = p->next) {
		ساختار object *obj = &p->item->object;
		صحیح mark = get_object_mark(obj);

		اگر (!mark && !reference_excluded_commits)
			ادامه;
		اگر (i == 0)
			printf("from ");
		وگرنه
			printf("merge ");
		اگر (mark)
			printf(":%d\n", mark);
		وگرنه
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	اگر (full_درخت)
		printf("deleteall\n");
	log_درخت_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

ایستا نویسه *anonymize_tag(تهی)
{
	ایستا صحیح counter;
	ساختار strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	بازگشت strbuf_detach(&out, پوچ);
}


ایستا تهی handle_tag(ثابت نویسه *name, ساختار tag *tag)
{
	size_t size;
	شمارش object_type type;
	نویسه *buf;
	ثابت نویسه *tagger, *tagger_end, *message;
	size_t message_size = 0;
	ساختار object *tagged;
	صحیح tagged_mark;
	ساختار commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	وقتی (tagged->type == OBJ_TAG) {
		tagged = ((ساختار tag *)tagged)->tagged;
	}
	اگر (tagged->type == OBJ_درخت) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		بازگشت;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	اگر (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	اگر (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	اگر (!tagger) {
		اگر (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		وگرنه
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} وگرنه {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		اگر (anonymize)
			anonymize_ident_سطر(&tagger, &tagger_end);
	}

	اگر (anonymize) {
		name = anonymize_refname(name);
		اگر (message) {
			ایستا ساختار hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	اگر (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		اگر (sig_offset < message_size)
			انتخاب (signed_tag_mode) {
			/* Exporting modes */
			حالت SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			حالت SIGN_VERBATIM:
				بشکن;

			/* Stripping modes */
			حالت SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			حالت SIGN_STRIP:
				message_size = sig_offset;
				بشکن;

			/* Aborting modes */
			حالت SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			پیشفرض:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	اگر (!tagged_mark) {
		انتخاب (tag_of_filtered_mode) {
		حالت TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		حالت DROP:
			/* Ignore this tag altogether */
			آزاد(buf);
			بازگشت;
		حالت REWRITE:
			اگر (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} وگرنه اگر (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((ساختار commit *)tagged);
				اگر (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					آزاد(buf);
					بازگشت;
				}
				tagged_mark = get_object_mark(&p->object);
			} وگرنه {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	اگر (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	اگر (mark_tags) {
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	اگر (tagged_mark)
		printf("from :%d\n", tagged_mark);
	وگرنه
		printf("from %s\n", oid_to_hex(&tagged->oid));

	اگر (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (صحیح)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (صحیح)message_size, (صحیح)message_size, message ? message : "");
	آزاد(buf);
}

ایستا ساختار commit *get_commit(ساختار rev_cmdline_entry *e, ثابت نویسه *full_name)
{
	انتخاب (e->item->type) {
	حالت OBJ_COMMIT:
		بازگشت (ساختار commit *)e->item;
	حالت OBJ_TAG: {
		ساختار tag *tag = (ساختار tag *)e->item;

		/* handle nested tags */
		وقتی (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (ساختار tag *)tag->tagged;
		}
		اگر (!tag)
			die(_("tag %s points nowhere?"), e->name);
		بازگشت (ساختار commit *)tag;
	}
	پیشفرض:
		بازگشت پوچ;
	}
}

ایستا تهی get_tags_and_duplicates(ساختار rev_cmdline_info *info)
{
	صحیح i;

	برای (i = 0; i < info->nr; i++) {
		ساختار rev_cmdline_entry *e = info->rev + i;
		ساختار object_id oid;
		ساختار commit *commit;
		نویسه *full_name = پوچ;

		اگر (e->flags & UNINTERESTING)
			ادامه;

		اگر (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			آزاد(full_name);
			ادامه;
		}

		اگر (refspecs.nr) {
			نویسه *خصوصی;
			خصوصی = apply_refspecs(&refspecs, full_name);
			اگر (خصوصی) {
				آزاد(full_name);
				full_name = خصوصی;
			}
		}

		commit = get_commit(e, full_name);
		اگر (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			آزاد(full_name);
			ادامه;
		}

		انتخاب (commit->object.type) {
		حالت OBJ_COMMIT:
			بشکن;
		حالت OBJ_BLOB:
			export_blob(&commit->object.oid);
			آزاد(full_name);
			ادامه;
		پیشفرض: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			آزاد(full_name);
			ادامه;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		اگر (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		اگر (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		وگرنه
			آزاد(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

ایستا تهی handle_tags_and_duplicates(ساختار string_list *extras)
{
	ساختار commit *commit;
	صحیح i;

	برای (i = extras->nr - 1; i >= 0; i--) {
		ثابت نویسه *name = extras->اقلام[i].string;
		ساختار object *object = extras->اقلام[i].util;
		صحیح mark;

		انتخاب (object->type) {
		حالت OBJ_TAG:
			handle_tag(name, (ساختار tag *)object);
			بشکن;
		حالت OBJ_COMMIT:
			اگر (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((ساختار commit *)object);
			اگر (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				printf("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				ادامه;
			}

			mark = get_object_mark(&commit->object);
			اگر (!mark) {
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
				اگر (!reference_excluded_commits) {
					/* delete the ref */
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					ادامه;
				}
				/* set ref to commit using oid, not mark */
				printf("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				ادامه;
			}

			printf("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			بشکن;
		}
	}
}

ایستا تهی export_marks(نویسه *file)
{
	unsigned صحیح i;
	uint32_t mark;
	ساختار decoration_entry *deco = idnums.entries;
	FILE *f;
	صحیح e = 0;

	f = fopen_for_writing(file);
	اگر (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	برای (i = 0; i < idnums.size; i++) {
		اگر (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			اگر (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    بشکن;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	اگر (e)
		error(_("unable to write marks file %s."), file);
}

ایستا تهی import_marks(نویسه *درون_داد_file, صحیح check_exists)
{
	نویسه سطر[512];
	FILE *f;
	ساختار stat sb;

	اگر (check_exists && stat(درون_داد_file, &sb))
		بازگشت;

	f = xfopen(درون_داد_file, "r");
	وقتی (fgets(سطر, اندازه(سطر), f)) {
		uint32_t mark;
		نویسه *سطر_end, *mark_end;
		ساختار object_id oid;
		ساختار object *object;
		ساختار commit *commit;
		شمارش object_type type;

		سطر_end = strchr(سطر, '\n');
		اگر (سطر[0] != ':' || !سطر_end)
			die(_("corrupt mark line: %s"), سطر);
		*سطر_end = '\0';

		mark = strtoumax(سطر + 1, &mark_end, 10);
		اگر (!mark || mark_end == سطر + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), سطر);

		اگر (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, پوچ);
		اگر (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		اگر (type != OBJ_COMMIT)
			/* only commits */
			ادامه;

		commit = lookup_commit(the_repository, &oid);
		اگر (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		اگر (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

ایستا تهی handle_deletes(تهی)
{
	صحیح i;
	برای (i = 0; i < refspecs.nr; i++) {
		ساختار refspec_item *refspec = &refspecs.اقلام[i];
		اگر (*refspec->src)
			ادامه;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

ایستا صحیح parse_opt_anonymize_map(ثابت ساختار option *opt,
				   ثابت نویسه *arg, صحیح unset)
{
	ساختار hashmap *map = opt->مقدار;
	ثابت نویسه *delim, *مقدار;
	size_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	اگر (delim) {
		keylen = delim - arg;
		مقدار = delim + 1;
	} وگرنه {
		keylen = strlen(arg);
		مقدار = arg;
	}

	اگر (!keylen || !*مقدار)
		بازگشت error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(مقدار));

	بازگشت 0;
}

صحیح cmd_fast_export(صحیح argc,
		    ثابت نویسه **argv,
		    ثابت نویسه *prefix,
		    ساختار repository *repo UNUSED)
{
	ساختار rev_info revs;
	ساختار commit *commit;
	نویسه *export_filename = پوچ,
	     *import_filename = پوچ,
	     *import_filename_if_exists = پوچ;
	uint32_t lastimportid;
	ساختار string_list refspecs_list = STRING_LIST_INIT_NODUP;
	ساختار string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	ساختار option options[] = {
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
		OPT_BOOL(0, "full-tree", &full_درخت,
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

	اگر (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, پوچ);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, prefix);
	init_revision_sources(&revision_sources);
	revs.topo_order = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, prefix, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = setup_revisions(argc, argv, &revs, پوچ);
	اگر (argc > 1)
		usage_with_options (fast_export_usage, options);

	اگر (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	اگر (refspecs_list.nr) {
		صحیح i;

		برای (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.اقلام[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	اگر (use_done_feature)
		printf("feature done\n");

	اگر (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	اگر (import_filename)
		import_marks(import_filename, 0);
	وگرنه اگر (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	اگر (import_filename && revs.prune_data.nr)
		full_درخت = 1;

	get_tags_and_duplicates(&revs.cmdline);

	اگر (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	وقتی ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	اگر (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	اگر (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	بازگشت 0;
}
