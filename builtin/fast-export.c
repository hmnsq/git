/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "refspec.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "object.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "diffcore.h"
#அந்தர்பாவ "log-tree.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "decorate.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "utf8.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "blob.h"
#அந்தர்பாவ "gpg-interface.h"

நிலையான மாறிலி எழுத்து *மாறிலி fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	வெற்று
};

நிலையான முழு progress;
நிலையான எண்ணிக்கை sign_mode signed_tag_mode = SIGN_ABORT;
நிலையான எண்ணிக்கை sign_mode signed_commit_mode = SIGN_STRIP;
நிலையான எண்ணிக்கை tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
நிலையான எண்ணிக்கை reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
நிலையான முழு fake_missing_tagger;
நிலையான முழு use_done_feature;
நிலையான முழு no_data;
நிலையான முழு full_மரம்;
நிலையான முழு reference_excluded_commits;
நிலையான முழு show_original_ids;
நிலையான முழு mark_tags;
நிலையான கட்டமைப்பு string_list extra_refs = STRING_LIST_INIT_DUP;
நிலையான கட்டமைப்பு string_list tag_refs = STRING_LIST_INIT_DUP;
நிலையான கட்டமைப்பு refspec refspecs;
நிலையான முழு anonymize;
நிலையான கட்டமைப்பு hashmap anonymized_seeds;
நிலையான கட்டமைப்பு revision_sources revision_sources;

நிலையான முழு parse_opt_sign_mode(மாறிலி கட்டமைப்பு option *opt,
			       மாறிலி எழுத்து *arg, முழு unset)
{
	எண்ணிக்கை sign_mode *val = opt->மதிப்பு;

	எனில் (unset)
		திரும்பு 0;

	எனில் (parse_sign_mode(arg, val, வெற்று) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		திரும்பு error(_("unknown %s mode: %s"), opt->long_name, arg);

	திரும்பு 0;
}

நிலையான முழு parse_opt_tag_of_filtered_mode(மாறிலி கட்டமைப்பு option *opt,
					  மாறிலி எழுத்து *arg, முழு unset)
{
	எண்ணிக்கை tag_of_filtered_mode *val = opt->மதிப்பு;

	எனில் (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	இல்லையெனில் எனில் (!strcmp(arg, "drop"))
		*val = DROP;
	இல்லையெனில் எனில் (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	இல்லையெனில்
		திரும்பு error(_("unknown tag-of-filtered mode: %s"), arg);
	திரும்பு 0;
}

நிலையான முழு parse_opt_reencode_mode(மாறிலி கட்டமைப்பு option *opt,
				   மாறிலி எழுத்து *arg, முழு unset)
{
	எண்ணிக்கை reencode_mode *val = opt->மதிப்பு;

	எனில் (unset) {
		*val = REENCODE_ABORT;
		திரும்பு 0;
	}

	தேர்வு (git_parse_maybe_bool(arg)) {
	நிலை 0:
		*val = REENCODE_NO;
		நிறுத்து;
	நிலை 1:
		*val = REENCODE_YES;
		நிறுத்து;
	இயல்பு:
		எனில் (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		இல்லையெனில்
			திரும்பு error(_("unknown reencoding mode: %s"), arg);
	}

	திரும்பு 0;
}

நிலையான கட்டமைப்பு decoration idnums;
நிலையான uint32_t last_idnum;
கட்டமைப்பு anonymized_entry {
	கட்டமைப்பு hashmap_entry hash;
	எழுத்து *anon;
	மாறிலி எழுத்து orig[FLEX_ARRAY];
};

கட்டமைப்பு anonymized_entry_சாவி {
	கட்டமைப்பு hashmap_entry hash;
	மாறிலி எழுத்து *orig;
	size_t orig_len;
};

நிலையான முழு anonymized_entry_cmp(மாறிலி வெறுமை *cmp_data UNUSED,
				மாறிலி கட்டமைப்பு hashmap_entry *eptr,
				மாறிலி கட்டமைப்பு hashmap_entry *entry_or_சாவி,
				மாறிலி வெறுமை *keydata)
{
	மாறிலி கட்டமைப்பு anonymized_entry *a, *b;

	a = container_of(eptr, மாறிலி கட்டமைப்பு anonymized_entry, hash);
	எனில் (keydata) {
		மாறிலி கட்டமைப்பு anonymized_entry_சாவி *சாவி = keydata;
		முழு equal = !xstrncmpz(a->orig, சாவி->orig, சாவி->orig_len);
		திரும்பு !equal;
	}

	b = container_of(entry_or_சாவி, மாறிலி கட்டமைப்பு anonymized_entry, hash);
	திரும்பு strcmp(a->orig, b->orig);
}

நிலையான கட்டமைப்பு anonymized_entry *add_anonymized_entry(கட்டமைப்பு hashmap *map,
						     unsigned hash,
						     மாறிலி எழுத்து *orig, size_t len,
						     எழுத்து *anon)
{
	கட்டமைப்பு anonymized_entry *ret, *old;

	எனில் (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, வெற்று, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	எனில் (old) {
		விடுவி(old->anon);
		விடுவி(old);
	}

	திரும்பு ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
நிலையான மாறிலி எழுத்து *anonymize_str(கட்டமைப்பு hashmap *map,
				 எழுத்து *(*generate)(வெறுமை),
				 மாறிலி எழுத்து *orig, size_t len)
{
	கட்டமைப்பு anonymized_entry_சாவி சாவி;
	கட்டமைப்பு anonymized_entry *ret;

	hashmap_entry_init(&சாவி.hash, memhash(orig, len));
	சாவி.orig = orig;
	சாவி.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &சாவி, hash, &சாவி);

	/* ...otherwise check if we've already seen it in this context... */
	எனில் (!ret)
		ret = hashmap_get_entry(map, &சாவி, hash, &சாவி);

	/* ...and finally generate a new mapping if necessary */
	எனில் (!ret)
		ret = add_anonymized_entry(map, சாவி.hash.hash,
					   orig, len, generate());

	திரும்பு ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
நிலையான வெறுமை anonymize_path(கட்டமைப்பு strbuf *out, மாறிலி எழுத்து *path,
			   கட்டமைப்பு hashmap *map,
			   எழுத்து *(*generate)(வெறுமை))
{
	போது (*path) {
		மாறிலி எழுத்து *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		மாறிலி எழுத்து *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		எனில் (*path)
			strbuf_addch(out, *path++);
	}
}

நிலையான உள்வரி வெறுமை *mark_to_ptr(uint32_t mark)
{
	திரும்பு (வெறுமை *)(uintptr_t)mark;
}

நிலையான உள்வரி uint32_t ptr_to_mark(வெறுமை * mark)
{
	திரும்பு (uint32_t)(uintptr_t)mark;
}

நிலையான உள்வரி வெறுமை mark_object(கட்டமைப்பு object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

நிலையான உள்வரி வெறுமை mark_next_object(கட்டமைப்பு object *object)
{
	mark_object(object, ++last_idnum);
}

நிலையான முழு get_object_mark(கட்டமைப்பு object *object)
{
	வெறுமை *decoration = lookup_decoration(&idnums, object);
	எனில் (!decoration)
		திரும்பு 0;
	திரும்பு ptr_to_mark(decoration);
}

நிலையான கட்டமைப்பு commit *rewrite_commit(கட்டமைப்பு commit *p)
{
	க்கு (;;) {
		எனில் (p->parents && p->parents->next)
			நிறுத்து;
		எனில் (p->object.flags & UNINTERESTING)
			நிறுத்து;
		எனில் (!(p->object.flags & TREESAME))
			நிறுத்து;
		எனில் (!p->parents)
			திரும்பு வெற்று;
		p = p->parents->item;
	}
	திரும்பு p;
}

நிலையான வெறுமை show_progress(வெறுமை)
{
	நிலையான முழு counter = 0;
	எனில் (!progress)
		திரும்பு;
	எனில் ((++counter % progress) == 0)
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
நிலையான எழுத்து *anonymize_blob(unsigned long *size)
{
	நிலையான முழு counter;
	கட்டமைப்பு strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	திரும்பு strbuf_detach(&out, வெற்று);
}

நிலையான வெறுமை export_blob(மாறிலி கட்டமைப்பு object_id *oid)
{
	unsigned long size;
	எண்ணிக்கை object_type type;
	எழுத்து *buf;
	கட்டமைப்பு object *object;
	முழு eaten;

	எனில் (no_data)
		திரும்பு;

	எனில் (is_null_oid(oid))
		திரும்பு;

	object = lookup_object(the_repository, oid);
	எனில் (object && object->flags & SHOWN)
		திரும்பு;

	எனில் (anonymize) {
		buf = anonymize_blob(&size);
		object = (கட்டமைப்பு object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} இல்லையெனில் {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		எனில் (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		எனில் (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	எனில் (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	எனில் (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	எனில் (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	எனில் (!eaten)
		விடுவி(buf);
}

நிலையான முழு depth_first(மாறிலி வெறுமை *a_, மாறிலி வெறுமை *b_)
{
	மாறிலி கட்டமைப்பு diff_filepair *a = *((மாறிலி கட்டமைப்பு diff_filepair **)a_);
	மாறிலி கட்டமைப்பு diff_filepair *b = *((மாறிலி கட்டமைப்பு diff_filepair **)b_);
	மாறிலி எழுத்து *name_a, *name_b;
	முழு len_a, len_b, len;
	முழு cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	எனில் (cmp)
		திரும்பு cmp;
	cmp = len_b - len_a;
	எனில் (cmp)
		திரும்பு cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	திரும்பு (a->status == 'R') - (b->status == 'R');
}

நிலையான வெறுமை print_path_1(மாறிலி எழுத்து *path)
{
	முழு need_quote = quote_c_style(path, வெற்று, வெற்று, 0);
	எனில் (need_quote)
		quote_c_style(path, வெற்று, stdout, 0);
	இல்லையெனில் எனில் (strchr(path, ' '))
		printf("\"%s\"", path);
	இல்லையெனில்
		printf("%s", path);
}

நிலையான எழுத்து *anonymize_path_component(வெறுமை)
{
	நிலையான முழு counter;
	கட்டமைப்பு strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	திரும்பு strbuf_detach(&out, வெற்று);
}

நிலையான வெறுமை print_path(மாறிலி எழுத்து *path)
{
	எனில் (!anonymize)
		print_path_1(path);
	இல்லையெனில் {
		நிலையான கட்டமைப்பு hashmap paths;
		நிலையான கட்டமைப்பு strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

நிலையான எழுத்து *generate_fake_oid(வெறுமை)
{
	நிலையான uint32_t counter = 1; /* avoid null oid */
	மாறிலி unsigned hashsz = the_hash_algo->rawsz;
	கட்டமைப்பு object_id oid;
	எழுத்து *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	திரும்பு oid_to_hex_r(hex, &oid);
}

நிலையான மாறிலி எழுத்து *anonymize_oid(மாறிலி எழுத்து *oid_hex)
{
	நிலையான கட்டமைப்பு hashmap objs;
	size_t len = strlen(oid_hex);
	திரும்பு anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

நிலையான வெறுமை show_filemodify(கட்டமைப்பு diff_queue_struct *q,
			    கட்டமைப்பு diff_options *options UNUSED, வெறுமை *data)
{
	முழு i;
	கட்டமைப்பு string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	க்கு (i = 0; i < q->nr; i++) {
		கட்டமைப்பு diff_filespec *ospec = q->queue[i]->one;
		கட்டமைப்பு diff_filespec *spec = q->queue[i]->two;

		தேர்வு (q->queue[i]->status) {
		நிலை DIFF_STATUS_DELETED:
			printf("D ");
			print_path(spec->path);
			string_list_செருகு(changed, spec->path);
			putchar('\n');
			நிறுத்து;

		நிலை DIFF_STATUS_COPIED:
		நிலை DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			எனில் (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				print_path(ospec->path);
				putchar(' ');
				print_path(spec->path);
				string_list_செருகு(changed, spec->path);
				putchar('\n');

				எனில் (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					நிறுத்து;
			}
			/* fallthrough */

		நிலை DIFF_STATUS_TYPE_CHANGED:
		நிலை DIFF_STATUS_MODIFIED:
		நிலை DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			எனில் (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			இல்லையெனில் {
				கட்டமைப்பு object *object = lookup_object(the_repository,
								      &spec->oid);
				printf("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_செருகு(changed, spec->path);
			putchar('\n');
			நிறுத்து;

		இயல்பு:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

நிலையான எழுத்து *anonymize_ref_component(வெறுமை)
{
	நிலையான முழு counter;
	கட்டமைப்பு strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	திரும்பு strbuf_detach(&out, வெற்று);
}

நிலையான மாறிலி எழுத்து *anonymize_refname(மாறிலி எழுத்து *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	நிலையான மாறிலி எழுத்து *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	நிலையான கட்டமைப்பு hashmap refs;
	நிலையான கட்டமைப்பு strbuf anon = STRBUF_INIT;
	முழு i;

	strbuf_reset(&anon);
	க்கு (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		எனில் (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			நிறுத்து;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	திரும்பு anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
நிலையான எழுத்து *anonymize_commit_message(வெறுமை)
{
	நிலையான முழு counter;
	திரும்பு xstrfmt("subject %d\n\nbody\n", counter++);
}

நிலையான எழுத்து *anonymize_ident(வெறுமை)
{
	நிலையான முழு counter;
	கட்டமைப்பு strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	திரும்பு strbuf_detach(&out, வெற்று);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
நிலையான வெறுமை anonymize_ident_வரி(மாறிலி எழுத்து **beg, மாறிலி எழுத்து **end)
{
	நிலையான கட்டமைப்பு hashmap idents;
	நிலையான கட்டமைப்பு strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	நிலையான unsigned which_buffer;

	கட்டமைப்பு strbuf *out;
	கட்டமைப்பு ident_split split;
	மாறிலி எழுத்து *end_of_header;

	out = &buffers[which_buffer++];
	which_buffer %= ARRAY_SIZE(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = strchr(*beg, ' ');
	எனில் (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (முழு)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	எனில் (!split_ident_வரி(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		மாறிலி எழுத்து *ident;
		size_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} இல்லையெனில் {
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
நிலையான மாறிலி எழுத்து *find_commit_multiline_header(மாறிலி எழுத்து *செய்தி,
						மாறிலி எழுத்து *சாவி,
						மாறிலி எழுத்து **end)
{
	கட்டமைப்பு strbuf val = STRBUF_INIT;
	மாறிலி எழுத்து *bol, *eol;
	size_t len;

	bol = find_commit_header(செய்தி, சாவி, &len);
	எனில் (!bol)
		திரும்பு வெற்று;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	போது (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	திரும்பு strbuf_detach(&val, வெற்று);
}

நிலையான வெறுமை print_signature(மாறிலி எழுத்து *signature, மாறிலி எழுத்து *object_hash)
{
	எனில் (!signature)
		திரும்பு;

	printf("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (unsigned)strlen(signature),
	       signature);
}

நிலையான மாறிலி எழுத்து *append_signatures_for_header(கட்டமைப்பு string_list *signatures,
						மாறிலி எழுத்து *pos,
						மாறிலி எழுத்து *header,
						மாறிலி எழுத்து *object_hash)
{
	மாறிலி எழுத்து *signature;
	மாறிலி எழுத்து *start = pos;
	மாறிலி எழுத்து *end = pos;

	போது ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (வெறுமை *)object_hash;
		விடுவி((எழுத்து *)signature);
		start = end;
	}

	திரும்பு end;
}

நிலையான வெறுமை handle_commit(கட்டமைப்பு commit *commit, கட்டமைப்பு rev_info *rev,
			  கட்டமைப்பு string_list *paths_of_changed_objects)
{
	முழு saved_output_format = rev->diffopt.output_format;
	மாறிலி எழுத்து *commit_buffer, *commit_buffer_cursor;
	மாறிலி எழுத்து *author, *author_end, *committer, *committer_end;
	மாறிலி எழுத்து *encoding = வெற்று;
	size_t encoding_len;
	கட்டமைப்பு string_list signatures = STRING_LIST_INIT_DUP;
	மாறிலி எழுத்து *message;
	எழுத்து *reencoded = வெற்று;
	கட்டமைப்பு commit_list *p;
	மாறிலி எழுத்து *refname;
	முழு i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, வெற்று);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	எனில் (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	எனில் (!committer)
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

	எனில் (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		எனில் (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	எனில் (*commit_buffer_cursor == '\n') {
		மாறிலி எழுத்து *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		மாறிலி எழுத்து *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
	எனில் (message)
		message += 2;

	எனில் (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_மரம்) {
		parse_commit_or_die(commit->parents->item);
		diff_மரம்_oid(get_commit_மரம்_oid(commit->parents->item),
			      get_commit_மரம்_oid(commit), "", &rev->diffopt);
	}
	இல்லையெனில்
		diff_வேர்_மரம்_oid(get_commit_மரம்_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	க்கு (i = 0; i < diff_queued_diff.nr; i++)
		எனில் (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	எனில் (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_வரி(&committer, &committer_end);
		anonymize_ident_வரி(&author, &author_end);
	}

	mark_next_object(&commit->object);
	எனில் (anonymize) {
		reencoded = anonymize_commit_message();
	} இல்லையெனில் எனில் (encoding) {
		எழுத்து *buf;
		தேர்வு (reencode_mode) {
		நிலை REENCODE_YES:
			buf = xstrfmt("%.*s", (முழு)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			விடுவி(buf);
			நிறுத்து;
		நிலை REENCODE_NO:
			நிறுத்து;
		நிலை REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (முழு)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	எனில் (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	எனில் (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (முழு)(author_end - author), author,
	       (முழு)(committer_end - committer), committer);
	எனில் (signatures.nr) {
		தேர்வு (signed_commit_mode) {
		/* Exporting modes */
		நிலை SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		நிலை SIGN_VERBATIM:
			க்கு (size_t i = 0; i < signatures.nr; i++) {
				கட்டமைப்பு string_list_item *item = &signatures.பொருட்கள்[i];
				print_signature(item->string, item->util);
			}
			நிறுத்து;

		/* Stripping modes */
		நிலை SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		நிலை SIGN_STRIP:
			நிறுத்து;

		/* Aborting modes */
		நிலை SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		இயல்பு:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	எனில் (!reencoded && encoding)
		printf("encoding %.*s\n", (முழு)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	விடுவி(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	க்கு (i = 0, p = commit->parents; p; p = p->next) {
		கட்டமைப்பு object *obj = &p->item->object;
		முழு mark = get_object_mark(obj);

		எனில் (!mark && !reference_excluded_commits)
			தொடர்;
		எனில் (i == 0)
			printf("from ");
		இல்லையெனில்
			printf("merge ");
		எனில் (mark)
			printf(":%d\n", mark);
		இல்லையெனில்
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	எனில் (full_மரம்)
		printf("deleteall\n");
	log_மரம்_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

நிலையான எழுத்து *anonymize_tag(வெறுமை)
{
	நிலையான முழு counter;
	கட்டமைப்பு strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	திரும்பு strbuf_detach(&out, வெற்று);
}


நிலையான வெறுமை handle_tag(மாறிலி எழுத்து *name, கட்டமைப்பு tag *tag)
{
	size_t size;
	எண்ணிக்கை object_type type;
	எழுத்து *buf;
	மாறிலி எழுத்து *tagger, *tagger_end, *message;
	size_t message_size = 0;
	கட்டமைப்பு object *tagged;
	முழு tagged_mark;
	கட்டமைப்பு commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	போது (tagged->type == OBJ_TAG) {
		tagged = ((கட்டமைப்பு tag *)tagged)->tagged;
	}
	எனில் (tagged->type == OBJ_மரம்) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		திரும்பு;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	எனில் (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	எனில் (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	எனில் (!tagger) {
		எனில் (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		இல்லையெனில்
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} இல்லையெனில் {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		எனில் (anonymize)
			anonymize_ident_வரி(&tagger, &tagger_end);
	}

	எனில் (anonymize) {
		name = anonymize_refname(name);
		எனில் (message) {
			நிலையான கட்டமைப்பு hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	எனில் (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		எனில் (sig_offset < message_size)
			தேர்வு (signed_tag_mode) {
			/* Exporting modes */
			நிலை SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			நிலை SIGN_VERBATIM:
				நிறுத்து;

			/* Stripping modes */
			நிலை SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			நிலை SIGN_STRIP:
				message_size = sig_offset;
				நிறுத்து;

			/* Aborting modes */
			நிலை SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			இயல்பு:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	எனில் (!tagged_mark) {
		தேர்வு (tag_of_filtered_mode) {
		நிலை TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		நிலை DROP:
			/* Ignore this tag altogether */
			விடுவி(buf);
			திரும்பு;
		நிலை REWRITE:
			எனில் (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} இல்லையெனில் எனில் (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((கட்டமைப்பு commit *)tagged);
				எனில் (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					விடுவி(buf);
					திரும்பு;
				}
				tagged_mark = get_object_mark(&p->object);
			} இல்லையெனில் {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	எனில் (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	எனில் (mark_tags) {
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	எனில் (tagged_mark)
		printf("from :%d\n", tagged_mark);
	இல்லையெனில்
		printf("from %s\n", oid_to_hex(&tagged->oid));

	எனில் (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (முழு)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (முழு)message_size, (முழு)message_size, message ? message : "");
	விடுவி(buf);
}

நிலையான கட்டமைப்பு commit *get_commit(கட்டமைப்பு rev_cmdline_entry *e, மாறிலி எழுத்து *full_name)
{
	தேர்வு (e->item->type) {
	நிலை OBJ_COMMIT:
		திரும்பு (கட்டமைப்பு commit *)e->item;
	நிலை OBJ_TAG: {
		கட்டமைப்பு tag *tag = (கட்டமைப்பு tag *)e->item;

		/* handle nested tags */
		போது (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (கட்டமைப்பு tag *)tag->tagged;
		}
		எனில் (!tag)
			die(_("tag %s points nowhere?"), e->name);
		திரும்பு (கட்டமைப்பு commit *)tag;
	}
	இயல்பு:
		திரும்பு வெற்று;
	}
}

நிலையான வெறுமை get_tags_and_duplicates(கட்டமைப்பு rev_cmdline_info *info)
{
	முழு i;

	க்கு (i = 0; i < info->nr; i++) {
		கட்டமைப்பு rev_cmdline_entry *e = info->rev + i;
		கட்டமைப்பு object_id oid;
		கட்டமைப்பு commit *commit;
		எழுத்து *full_name = வெற்று;

		எனில் (e->flags & UNINTERESTING)
			தொடர்;

		எனில் (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			விடுவி(full_name);
			தொடர்;
		}

		எனில் (refspecs.nr) {
			எழுத்து *தனிப்பட்ட;
			தனிப்பட்ட = apply_refspecs(&refspecs, full_name);
			எனில் (தனிப்பட்ட) {
				விடுவி(full_name);
				full_name = தனிப்பட்ட;
			}
		}

		commit = get_commit(e, full_name);
		எனில் (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			விடுவி(full_name);
			தொடர்;
		}

		தேர்வு (commit->object.type) {
		நிலை OBJ_COMMIT:
			நிறுத்து;
		நிலை OBJ_BLOB:
			export_blob(&commit->object.oid);
			விடுவி(full_name);
			தொடர்;
		இயல்பு: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			விடுவி(full_name);
			தொடர்;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		எனில் (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		எனில் (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		இல்லையெனில்
			விடுவி(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

நிலையான வெறுமை handle_tags_and_duplicates(கட்டமைப்பு string_list *extras)
{
	கட்டமைப்பு commit *commit;
	முழு i;

	க்கு (i = extras->nr - 1; i >= 0; i--) {
		மாறிலி எழுத்து *name = extras->பொருட்கள்[i].string;
		கட்டமைப்பு object *object = extras->பொருட்கள்[i].util;
		முழு mark;

		தேர்வு (object->type) {
		நிலை OBJ_TAG:
			handle_tag(name, (கட்டமைப்பு tag *)object);
			நிறுத்து;
		நிலை OBJ_COMMIT:
			எனில் (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((கட்டமைப்பு commit *)object);
			எனில் (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				printf("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				தொடர்;
			}

			mark = get_object_mark(&commit->object);
			எனில் (!mark) {
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
				எனில் (!reference_excluded_commits) {
					/* delete the ref */
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					தொடர்;
				}
				/* set ref to commit using oid, not mark */
				printf("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				தொடர்;
			}

			printf("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			நிறுத்து;
		}
	}
}

நிலையான வெறுமை export_marks(எழுத்து *file)
{
	unsigned முழு i;
	uint32_t mark;
	கட்டமைப்பு decoration_entry *deco = idnums.entries;
	FILE *f;
	முழு e = 0;

	f = fopen_for_writing(file);
	எனில் (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	க்கு (i = 0; i < idnums.size; i++) {
		எனில் (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			எனில் (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    நிறுத்து;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	எனில் (e)
		error(_("unable to write marks file %s."), file);
}

நிலையான வெறுமை import_marks(எழுத்து *உள்ளீடு_file, முழு check_exists)
{
	எழுத்து வரி[512];
	FILE *f;
	கட்டமைப்பு stat sb;

	எனில் (check_exists && stat(உள்ளீடு_file, &sb))
		திரும்பு;

	f = xfopen(உள்ளீடு_file, "r");
	போது (fgets(வரி, அளவு(வரி), f)) {
		uint32_t mark;
		எழுத்து *வரி_end, *mark_end;
		கட்டமைப்பு object_id oid;
		கட்டமைப்பு object *object;
		கட்டமைப்பு commit *commit;
		எண்ணிக்கை object_type type;

		வரி_end = strchr(வரி, '\n');
		எனில் (வரி[0] != ':' || !வரி_end)
			die(_("corrupt mark line: %s"), வரி);
		*வரி_end = '\0';

		mark = strtoumax(வரி + 1, &mark_end, 10);
		எனில் (!mark || mark_end == வரி + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), வரி);

		எனில் (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, வெற்று);
		எனில் (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		எனில் (type != OBJ_COMMIT)
			/* only commits */
			தொடர்;

		commit = lookup_commit(the_repository, &oid);
		எனில் (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		எனில் (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

நிலையான வெறுமை handle_deletes(வெறுமை)
{
	முழு i;
	க்கு (i = 0; i < refspecs.nr; i++) {
		கட்டமைப்பு refspec_item *refspec = &refspecs.பொருட்கள்[i];
		எனில் (*refspec->src)
			தொடர்;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

நிலையான முழு parse_opt_anonymize_map(மாறிலி கட்டமைப்பு option *opt,
				   மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு hashmap *map = opt->மதிப்பு;
	மாறிலி எழுத்து *delim, *மதிப்பு;
	size_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	எனில் (delim) {
		keylen = delim - arg;
		மதிப்பு = delim + 1;
	} இல்லையெனில் {
		keylen = strlen(arg);
		மதிப்பு = arg;
	}

	எனில் (!keylen || !*மதிப்பு)
		திரும்பு error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(மதிப்பு));

	திரும்பு 0;
}

முழு cmd_fast_export(முழு argc,
		    மாறிலி எழுத்து **argv,
		    மாறிலி எழுத்து *prefix,
		    கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு rev_info revs;
	கட்டமைப்பு commit *commit;
	எழுத்து *export_filename = வெற்று,
	     *import_filename = வெற்று,
	     *import_filename_if_exists = வெற்று;
	uint32_t lastimportid;
	கட்டமைப்பு string_list refspecs_list = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	கட்டமைப்பு option options[] = {
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
		OPT_BOOL(0, "full-tree", &full_மரம்,
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

	எனில் (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, வெற்று);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, prefix);
	init_revision_sources(&revision_sources);
	revs.topo_order = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, prefix, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = setup_revisions(argc, argv, &revs, வெற்று);
	எனில் (argc > 1)
		usage_with_options (fast_export_usage, options);

	எனில் (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	எனில் (refspecs_list.nr) {
		முழு i;

		க்கு (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.பொருட்கள்[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	எனில் (use_done_feature)
		printf("feature done\n");

	எனில் (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	எனில் (import_filename)
		import_marks(import_filename, 0);
	இல்லையெனில் எனில் (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	எனில் (import_filename && revs.prune_data.nr)
		full_மரம் = 1;

	get_tags_and_duplicates(&revs.cmdline);

	எனில் (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	போது ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	எனில் (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	எனில் (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	திரும்பு 0;
}
