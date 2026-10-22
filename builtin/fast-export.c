/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "refspec.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "diffcore.h"
#अन्तर्भाव "log-tree.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "decorate.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "utf8.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "gpg-interface.h"

स्थिर ध्रुव वर्ण *ध्रुव fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	रिक्त
};

स्थिर पूर्णाङ्क progress;
स्थिर गणना sign_mode signed_tag_mode = SIGN_ABORT;
स्थिर गणना sign_mode signed_commit_mode = SIGN_STRIP;
स्थिर गणना tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
स्थिर गणना reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
स्थिर पूर्णाङ्क fake_missing_tagger;
स्थिर पूर्णाङ्क use_done_feature;
स्थिर पूर्णाङ्क no_data;
स्थिर पूर्णाङ्क full_वृक्ष;
स्थिर पूर्णाङ्क reference_excluded_commits;
स्थिर पूर्णाङ्क show_original_ids;
स्थिर पूर्णाङ्क mark_tags;
स्थिर संरचना string_list extra_refs = STRING_LIST_INIT_DUP;
स्थिर संरचना string_list tag_refs = STRING_LIST_INIT_DUP;
स्थिर संरचना refspec refspecs;
स्थिर पूर्णाङ्क anonymize;
स्थिर संरचना hashmap anonymized_seeds;
स्थिर संरचना revision_sources revision_sources;

स्थिर पूर्णाङ्क parse_opt_sign_mode(ध्रुव संरचना option *opt,
			       ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	गणना sign_mode *val = opt->मूल्य;

	यदि (unset)
		प्रत्यागम 0;

	यदि (parse_sign_mode(arg, val, रिक्त) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		प्रत्यागम error(_("unknown %s mode: %s"), opt->long_name, arg);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_opt_tag_of_filtered_mode(ध्रुव संरचना option *opt,
					  ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	गणना tag_of_filtered_mode *val = opt->मूल्य;

	यदि (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	अन्यथा यदि (!strcmp(arg, "drop"))
		*val = DROP;
	अन्यथा यदि (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	अन्यथा
		प्रत्यागम error(_("unknown tag-of-filtered mode: %s"), arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_opt_reencode_mode(ध्रुव संरचना option *opt,
				   ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	गणना reencode_mode *val = opt->मूल्य;

	यदि (unset) {
		*val = REENCODE_ABORT;
		प्रत्यागम 0;
	}

	विकल्प (git_parse_maybe_bool(arg)) {
	पक्ष 0:
		*val = REENCODE_NO;
		भङ्ग;
	पक्ष 1:
		*val = REENCODE_YES;
		भङ्ग;
	स्वभाव:
		यदि (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		अन्यथा
			प्रत्यागम error(_("unknown reencoding mode: %s"), arg);
	}

	प्रत्यागम 0;
}

स्थिर संरचना decoration idnums;
स्थिर uint32_t last_idnum;
संरचना anonymized_entry {
	संरचना hashmap_entry hash;
	वर्ण *anon;
	ध्रुव वर्ण orig[FLEX_ARRAY];
};

संरचना anonymized_entry_कुंजी {
	संरचना hashmap_entry hash;
	ध्रुव वर्ण *orig;
	size_t orig_len;
};

स्थिर पूर्णाङ्क anonymized_entry_cmp(ध्रुव शून्य *cmp_data UNUSED,
				ध्रुव संरचना hashmap_entry *eptr,
				ध्रुव संरचना hashmap_entry *entry_or_कुंजी,
				ध्रुव शून्य *keydata)
{
	ध्रुव संरचना anonymized_entry *a, *b;

	a = container_of(eptr, ध्रुव संरचना anonymized_entry, hash);
	यदि (keydata) {
		ध्रुव संरचना anonymized_entry_कुंजी *कुंजी = keydata;
		पूर्णाङ्क equal = !xstrncmpz(a->orig, कुंजी->orig, कुंजी->orig_len);
		प्रत्यागम !equal;
	}

	b = container_of(entry_or_कुंजी, ध्रुव संरचना anonymized_entry, hash);
	प्रत्यागम strcmp(a->orig, b->orig);
}

स्थिर संरचना anonymized_entry *add_anonymized_entry(संरचना hashmap *map,
						     unsigned hash,
						     ध्रुव वर्ण *orig, size_t len,
						     वर्ण *anon)
{
	संरचना anonymized_entry *ret, *old;

	यदि (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, रिक्त, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	यदि (old) {
		मुक्त(old->anon);
		मुक्त(old);
	}

	प्रत्यागम ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
स्थिर ध्रुव वर्ण *anonymize_str(संरचना hashmap *map,
				 वर्ण *(*generate)(शून्य),
				 ध्रुव वर्ण *orig, size_t len)
{
	संरचना anonymized_entry_कुंजी कुंजी;
	संरचना anonymized_entry *ret;

	hashmap_entry_init(&कुंजी.hash, memhash(orig, len));
	कुंजी.orig = orig;
	कुंजी.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &कुंजी, hash, &कुंजी);

	/* ...otherwise check if we've already seen it in this context... */
	यदि (!ret)
		ret = hashmap_get_entry(map, &कुंजी, hash, &कुंजी);

	/* ...and finally generate a new mapping if necessary */
	यदि (!ret)
		ret = add_anonymized_entry(map, कुंजी.hash.hash,
					   orig, len, generate());

	प्रत्यागम ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
स्थिर शून्य anonymize_path(संरचना strbuf *out, ध्रुव वर्ण *path,
			   संरचना hashmap *map,
			   वर्ण *(*generate)(शून्य))
{
	यावत् (*path) {
		ध्रुव वर्ण *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		ध्रुव वर्ण *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		यदि (*path)
			strbuf_addch(out, *path++);
	}
}

स्थिर अन्तःस्थ शून्य *mark_to_ptr(uint32_t mark)
{
	प्रत्यागम (शून्य *)(uintptr_t)mark;
}

स्थिर अन्तःस्थ uint32_t ptr_to_mark(शून्य * mark)
{
	प्रत्यागम (uint32_t)(uintptr_t)mark;
}

स्थिर अन्तःस्थ शून्य mark_object(संरचना object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

स्थिर अन्तःस्थ शून्य mark_next_object(संरचना object *object)
{
	mark_object(object, ++last_idnum);
}

स्थिर पूर्णाङ्क get_object_mark(संरचना object *object)
{
	शून्य *decoration = lookup_decoration(&idnums, object);
	यदि (!decoration)
		प्रत्यागम 0;
	प्रत्यागम ptr_to_mark(decoration);
}

स्थिर संरचना commit *rewrite_commit(संरचना commit *p)
{
	प्रति (;;) {
		यदि (p->parents && p->parents->next)
			भङ्ग;
		यदि (p->object.flags & UNINTERESTING)
			भङ्ग;
		यदि (!(p->object.flags & TREESAME))
			भङ्ग;
		यदि (!p->parents)
			प्रत्यागम रिक्त;
		p = p->parents->item;
	}
	प्रत्यागम p;
}

स्थिर शून्य show_progress(शून्य)
{
	स्थिर पूर्णाङ्क counter = 0;
	यदि (!progress)
		प्रत्यागम;
	यदि ((++counter % progress) == 0)
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
स्थिर वर्ण *anonymize_blob(unsigned long *size)
{
	स्थिर पूर्णाङ्क counter;
	संरचना strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	प्रत्यागम strbuf_detach(&out, रिक्त);
}

स्थिर शून्य export_blob(ध्रुव संरचना object_id *oid)
{
	unsigned long size;
	गणना object_type type;
	वर्ण *buf;
	संरचना object *object;
	पूर्णाङ्क eaten;

	यदि (no_data)
		प्रत्यागम;

	यदि (is_null_oid(oid))
		प्रत्यागम;

	object = lookup_object(the_repository, oid);
	यदि (object && object->flags & SHOWN)
		प्रत्यागम;

	यदि (anonymize) {
		buf = anonymize_blob(&size);
		object = (संरचना object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} अन्यथा {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		यदि (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		यदि (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	यदि (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	यदि (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	यदि (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	यदि (!eaten)
		मुक्त(buf);
}

स्थिर पूर्णाङ्क depth_first(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	ध्रुव संरचना diff_filepair *a = *((ध्रुव संरचना diff_filepair **)a_);
	ध्रुव संरचना diff_filepair *b = *((ध्रुव संरचना diff_filepair **)b_);
	ध्रुव वर्ण *name_a, *name_b;
	पूर्णाङ्क len_a, len_b, len;
	पूर्णाङ्क cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	यदि (cmp)
		प्रत्यागम cmp;
	cmp = len_b - len_a;
	यदि (cmp)
		प्रत्यागम cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	प्रत्यागम (a->status == 'R') - (b->status == 'R');
}

स्थिर शून्य print_path_1(ध्रुव वर्ण *path)
{
	पूर्णाङ्क need_quote = quote_c_style(path, रिक्त, रिक्त, 0);
	यदि (need_quote)
		quote_c_style(path, रिक्त, stdout, 0);
	अन्यथा यदि (strchr(path, ' '))
		printf("\"%s\"", path);
	अन्यथा
		printf("%s", path);
}

स्थिर वर्ण *anonymize_path_component(शून्य)
{
	स्थिर पूर्णाङ्क counter;
	संरचना strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	प्रत्यागम strbuf_detach(&out, रिक्त);
}

स्थिर शून्य print_path(ध्रुव वर्ण *path)
{
	यदि (!anonymize)
		print_path_1(path);
	अन्यथा {
		स्थिर संरचना hashmap paths;
		स्थिर संरचना strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

स्थिर वर्ण *generate_fake_oid(शून्य)
{
	स्थिर uint32_t counter = 1; /* avoid null oid */
	ध्रुव unsigned hashsz = the_hash_algo->rawsz;
	संरचना object_id oid;
	वर्ण *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	प्रत्यागम oid_to_hex_r(hex, &oid);
}

स्थिर ध्रुव वर्ण *anonymize_oid(ध्रुव वर्ण *oid_hex)
{
	स्थिर संरचना hashmap objs;
	size_t len = strlen(oid_hex);
	प्रत्यागम anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

स्थिर शून्य show_filemodify(संरचना diff_queue_struct *q,
			    संरचना diff_options *options UNUSED, शून्य *data)
{
	पूर्णाङ्क i;
	संरचना string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	प्रति (i = 0; i < q->nr; i++) {
		संरचना diff_filespec *ospec = q->queue[i]->one;
		संरचना diff_filespec *spec = q->queue[i]->two;

		विकल्प (q->queue[i]->status) {
		पक्ष DIFF_STATUS_DELETED:
			printf("D ");
			print_path(spec->path);
			string_list_जोड़ो(changed, spec->path);
			putchar('\n');
			भङ्ग;

		पक्ष DIFF_STATUS_COPIED:
		पक्ष DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			यदि (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				print_path(ospec->path);
				putchar(' ');
				print_path(spec->path);
				string_list_जोड़ो(changed, spec->path);
				putchar('\n');

				यदि (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					भङ्ग;
			}
			/* fallthrough */

		पक्ष DIFF_STATUS_TYPE_CHANGED:
		पक्ष DIFF_STATUS_MODIFIED:
		पक्ष DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			यदि (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			अन्यथा {
				संरचना object *object = lookup_object(the_repository,
								      &spec->oid);
				printf("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_जोड़ो(changed, spec->path);
			putchar('\n');
			भङ्ग;

		स्वभाव:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

स्थिर वर्ण *anonymize_ref_component(शून्य)
{
	स्थिर पूर्णाङ्क counter;
	संरचना strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	प्रत्यागम strbuf_detach(&out, रिक्त);
}

स्थिर ध्रुव वर्ण *anonymize_refname(ध्रुव वर्ण *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	स्थिर ध्रुव वर्ण *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	स्थिर संरचना hashmap refs;
	स्थिर संरचना strbuf anon = STRBUF_INIT;
	पूर्णाङ्क i;

	strbuf_reset(&anon);
	प्रति (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		यदि (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			भङ्ग;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	प्रत्यागम anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
स्थिर वर्ण *anonymize_commit_message(शून्य)
{
	स्थिर पूर्णाङ्क counter;
	प्रत्यागम xstrfmt("subject %d\n\nbody\n", counter++);
}

स्थिर वर्ण *anonymize_ident(शून्य)
{
	स्थिर पूर्णाङ्क counter;
	संरचना strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	प्रत्यागम strbuf_detach(&out, रिक्त);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
स्थिर शून्य anonymize_ident_पंक्ति(ध्रुव वर्ण **beg, ध्रुव वर्ण **end)
{
	स्थिर संरचना hashmap idents;
	स्थिर संरचना strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	स्थिर unsigned which_buffer;

	संरचना strbuf *out;
	संरचना ident_split split;
	ध्रुव वर्ण *end_of_header;

	out = &buffers[which_buffer++];
	which_buffer %= ARRAY_SIZE(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = strchr(*beg, ' ');
	यदि (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (पूर्णाङ्क)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	यदि (!split_ident_पंक्ति(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		ध्रुव वर्ण *ident;
		size_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} अन्यथा {
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
स्थिर ध्रुव वर्ण *find_commit_multiline_header(ध्रुव वर्ण *संदेश,
						ध्रुव वर्ण *कुंजी,
						ध्रुव वर्ण **end)
{
	संरचना strbuf val = STRBUF_INIT;
	ध्रुव वर्ण *bol, *eol;
	size_t len;

	bol = find_commit_header(संदेश, कुंजी, &len);
	यदि (!bol)
		प्रत्यागम रिक्त;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	यावत् (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	प्रत्यागम strbuf_detach(&val, रिक्त);
}

स्थिर शून्य print_signature(ध्रुव वर्ण *signature, ध्रुव वर्ण *object_hash)
{
	यदि (!signature)
		प्रत्यागम;

	printf("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (unsigned)strlen(signature),
	       signature);
}

स्थिर ध्रुव वर्ण *append_signatures_for_header(संरचना string_list *signatures,
						ध्रुव वर्ण *pos,
						ध्रुव वर्ण *header,
						ध्रुव वर्ण *object_hash)
{
	ध्रुव वर्ण *signature;
	ध्रुव वर्ण *start = pos;
	ध्रुव वर्ण *end = pos;

	यावत् ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (शून्य *)object_hash;
		मुक्त((वर्ण *)signature);
		start = end;
	}

	प्रत्यागम end;
}

स्थिर शून्य handle_commit(संरचना commit *commit, संरचना rev_info *rev,
			  संरचना string_list *paths_of_changed_objects)
{
	पूर्णाङ्क saved_output_format = rev->diffopt.output_format;
	ध्रुव वर्ण *commit_buffer, *commit_buffer_cursor;
	ध्रुव वर्ण *author, *author_end, *committer, *committer_end;
	ध्रुव वर्ण *encoding = रिक्त;
	size_t encoding_len;
	संरचना string_list signatures = STRING_LIST_INIT_DUP;
	ध्रुव वर्ण *message;
	वर्ण *reencoded = रिक्त;
	संरचना commit_list *p;
	ध्रुव वर्ण *refname;
	पूर्णाङ्क i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, रिक्त);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	यदि (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	यदि (!committer)
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

	यदि (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		यदि (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	यदि (*commit_buffer_cursor == '\n') {
		ध्रुव वर्ण *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		ध्रुव वर्ण *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
	यदि (message)
		message += 2;

	यदि (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_वृक्ष) {
		parse_commit_or_die(commit->parents->item);
		diff_वृक्ष_oid(get_commit_वृक्ष_oid(commit->parents->item),
			      get_commit_वृक्ष_oid(commit), "", &rev->diffopt);
	}
	अन्यथा
		diff_मूल_वृक्ष_oid(get_commit_वृक्ष_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	प्रति (i = 0; i < diff_queued_diff.nr; i++)
		यदि (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	यदि (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_पंक्ति(&committer, &committer_end);
		anonymize_ident_पंक्ति(&author, &author_end);
	}

	mark_next_object(&commit->object);
	यदि (anonymize) {
		reencoded = anonymize_commit_message();
	} अन्यथा यदि (encoding) {
		वर्ण *buf;
		विकल्प (reencode_mode) {
		पक्ष REENCODE_YES:
			buf = xstrfmt("%.*s", (पूर्णाङ्क)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			मुक्त(buf);
			भङ्ग;
		पक्ष REENCODE_NO:
			भङ्ग;
		पक्ष REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (पूर्णाङ्क)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	यदि (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	यदि (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (पूर्णाङ्क)(author_end - author), author,
	       (पूर्णाङ्क)(committer_end - committer), committer);
	यदि (signatures.nr) {
		विकल्प (signed_commit_mode) {
		/* Exporting modes */
		पक्ष SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		पक्ष SIGN_VERBATIM:
			प्रति (size_t i = 0; i < signatures.nr; i++) {
				संरचना string_list_item *item = &signatures.वस्तुएँ[i];
				print_signature(item->string, item->util);
			}
			भङ्ग;

		/* Stripping modes */
		पक्ष SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		पक्ष SIGN_STRIP:
			भङ्ग;

		/* Aborting modes */
		पक्ष SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		स्वभाव:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	यदि (!reencoded && encoding)
		printf("encoding %.*s\n", (पूर्णाङ्क)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	मुक्त(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	प्रति (i = 0, p = commit->parents; p; p = p->next) {
		संरचना object *obj = &p->item->object;
		पूर्णाङ्क mark = get_object_mark(obj);

		यदि (!mark && !reference_excluded_commits)
			अनुवर्त;
		यदि (i == 0)
			printf("from ");
		अन्यथा
			printf("merge ");
		यदि (mark)
			printf(":%d\n", mark);
		अन्यथा
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	यदि (full_वृक्ष)
		printf("deleteall\n");
	log_वृक्ष_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

स्थिर वर्ण *anonymize_tag(शून्य)
{
	स्थिर पूर्णाङ्क counter;
	संरचना strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	प्रत्यागम strbuf_detach(&out, रिक्त);
}


स्थिर शून्य handle_tag(ध्रुव वर्ण *name, संरचना tag *tag)
{
	size_t size;
	गणना object_type type;
	वर्ण *buf;
	ध्रुव वर्ण *tagger, *tagger_end, *message;
	size_t message_size = 0;
	संरचना object *tagged;
	पूर्णाङ्क tagged_mark;
	संरचना commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	यावत् (tagged->type == OBJ_TAG) {
		tagged = ((संरचना tag *)tagged)->tagged;
	}
	यदि (tagged->type == OBJ_वृक्ष) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		प्रत्यागम;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	यदि (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	यदि (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	यदि (!tagger) {
		यदि (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		अन्यथा
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} अन्यथा {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		यदि (anonymize)
			anonymize_ident_पंक्ति(&tagger, &tagger_end);
	}

	यदि (anonymize) {
		name = anonymize_refname(name);
		यदि (message) {
			स्थिर संरचना hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	यदि (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		यदि (sig_offset < message_size)
			विकल्प (signed_tag_mode) {
			/* Exporting modes */
			पक्ष SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			पक्ष SIGN_VERBATIM:
				भङ्ग;

			/* Stripping modes */
			पक्ष SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			पक्ष SIGN_STRIP:
				message_size = sig_offset;
				भङ्ग;

			/* Aborting modes */
			पक्ष SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			स्वभाव:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	यदि (!tagged_mark) {
		विकल्प (tag_of_filtered_mode) {
		पक्ष TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		पक्ष DROP:
			/* Ignore this tag altogether */
			मुक्त(buf);
			प्रत्यागम;
		पक्ष REWRITE:
			यदि (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} अन्यथा यदि (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((संरचना commit *)tagged);
				यदि (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					मुक्त(buf);
					प्रत्यागम;
				}
				tagged_mark = get_object_mark(&p->object);
			} अन्यथा {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	यदि (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	यदि (mark_tags) {
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	यदि (tagged_mark)
		printf("from :%d\n", tagged_mark);
	अन्यथा
		printf("from %s\n", oid_to_hex(&tagged->oid));

	यदि (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (पूर्णाङ्क)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (पूर्णाङ्क)message_size, (पूर्णाङ्क)message_size, message ? message : "");
	मुक्त(buf);
}

स्थिर संरचना commit *get_commit(संरचना rev_cmdline_entry *e, ध्रुव वर्ण *full_name)
{
	विकल्प (e->item->type) {
	पक्ष OBJ_COMMIT:
		प्रत्यागम (संरचना commit *)e->item;
	पक्ष OBJ_TAG: {
		संरचना tag *tag = (संरचना tag *)e->item;

		/* handle nested tags */
		यावत् (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (संरचना tag *)tag->tagged;
		}
		यदि (!tag)
			die(_("tag %s points nowhere?"), e->name);
		प्रत्यागम (संरचना commit *)tag;
	}
	स्वभाव:
		प्रत्यागम रिक्त;
	}
}

स्थिर शून्य get_tags_and_duplicates(संरचना rev_cmdline_info *info)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < info->nr; i++) {
		संरचना rev_cmdline_entry *e = info->rev + i;
		संरचना object_id oid;
		संरचना commit *commit;
		वर्ण *full_name = रिक्त;

		यदि (e->flags & UNINTERESTING)
			अनुवर्त;

		यदि (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			मुक्त(full_name);
			अनुवर्त;
		}

		यदि (refspecs.nr) {
			वर्ण *गुप्त;
			गुप्त = apply_refspecs(&refspecs, full_name);
			यदि (गुप्त) {
				मुक्त(full_name);
				full_name = गुप्त;
			}
		}

		commit = get_commit(e, full_name);
		यदि (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			मुक्त(full_name);
			अनुवर्त;
		}

		विकल्प (commit->object.type) {
		पक्ष OBJ_COMMIT:
			भङ्ग;
		पक्ष OBJ_BLOB:
			export_blob(&commit->object.oid);
			मुक्त(full_name);
			अनुवर्त;
		स्वभाव: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			मुक्त(full_name);
			अनुवर्त;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		यदि (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		यदि (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		अन्यथा
			मुक्त(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

स्थिर शून्य handle_tags_and_duplicates(संरचना string_list *extras)
{
	संरचना commit *commit;
	पूर्णाङ्क i;

	प्रति (i = extras->nr - 1; i >= 0; i--) {
		ध्रुव वर्ण *name = extras->वस्तुएँ[i].string;
		संरचना object *object = extras->वस्तुएँ[i].util;
		पूर्णाङ्क mark;

		विकल्प (object->type) {
		पक्ष OBJ_TAG:
			handle_tag(name, (संरचना tag *)object);
			भङ्ग;
		पक्ष OBJ_COMMIT:
			यदि (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((संरचना commit *)object);
			यदि (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				printf("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				अनुवर्त;
			}

			mark = get_object_mark(&commit->object);
			यदि (!mark) {
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
				यदि (!reference_excluded_commits) {
					/* delete the ref */
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					अनुवर्त;
				}
				/* set ref to commit using oid, not mark */
				printf("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				अनुवर्त;
			}

			printf("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			भङ्ग;
		}
	}
}

स्थिर शून्य export_marks(वर्ण *file)
{
	unsigned पूर्णाङ्क i;
	uint32_t mark;
	संरचना decoration_entry *deco = idnums.entries;
	FILE *f;
	पूर्णाङ्क e = 0;

	f = fopen_for_writing(file);
	यदि (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	प्रति (i = 0; i < idnums.size; i++) {
		यदि (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			यदि (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    भङ्ग;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	यदि (e)
		error(_("unable to write marks file %s."), file);
}

स्थिर शून्य import_marks(वर्ण *निवेश_file, पूर्णाङ्क check_exists)
{
	वर्ण पंक्ति[512];
	FILE *f;
	संरचना stat sb;

	यदि (check_exists && stat(निवेश_file, &sb))
		प्रत्यागम;

	f = xfopen(निवेश_file, "r");
	यावत् (fgets(पंक्ति, परिमाण(पंक्ति), f)) {
		uint32_t mark;
		वर्ण *पंक्ति_end, *mark_end;
		संरचना object_id oid;
		संरचना object *object;
		संरचना commit *commit;
		गणना object_type type;

		पंक्ति_end = strchr(पंक्ति, '\n');
		यदि (पंक्ति[0] != ':' || !पंक्ति_end)
			die(_("corrupt mark line: %s"), पंक्ति);
		*पंक्ति_end = '\0';

		mark = strtoumax(पंक्ति + 1, &mark_end, 10);
		यदि (!mark || mark_end == पंक्ति + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), पंक्ति);

		यदि (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, रिक्त);
		यदि (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		यदि (type != OBJ_COMMIT)
			/* only commits */
			अनुवर्त;

		commit = lookup_commit(the_repository, &oid);
		यदि (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		यदि (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

स्थिर शून्य handle_deletes(शून्य)
{
	पूर्णाङ्क i;
	प्रति (i = 0; i < refspecs.nr; i++) {
		संरचना refspec_item *refspec = &refspecs.वस्तुएँ[i];
		यदि (*refspec->src)
			अनुवर्त;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

स्थिर पूर्णाङ्क parse_opt_anonymize_map(ध्रुव संरचना option *opt,
				   ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना hashmap *map = opt->मूल्य;
	ध्रुव वर्ण *delim, *मूल्य;
	size_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	यदि (delim) {
		keylen = delim - arg;
		मूल्य = delim + 1;
	} अन्यथा {
		keylen = strlen(arg);
		मूल्य = arg;
	}

	यदि (!keylen || !*मूल्य)
		प्रत्यागम error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(मूल्य));

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_fast_export(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *prefix,
		    संरचना repository *repo UNUSED)
{
	संरचना rev_info revs;
	संरचना commit *commit;
	वर्ण *export_filename = रिक्त,
	     *import_filename = रिक्त,
	     *import_filename_if_exists = रिक्त;
	uint32_t lastimportid;
	संरचना string_list refspecs_list = STRING_LIST_INIT_NODUP;
	संरचना string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	संरचना option options[] = {
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
		OPT_BOOL(0, "full-tree", &full_वृक्ष,
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

	यदि (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, रिक्त);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, prefix);
	init_revision_sources(&revision_sources);
	revs.topo_order = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, prefix, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = setup_revisions(argc, argv, &revs, रिक्त);
	यदि (argc > 1)
		usage_with_options (fast_export_usage, options);

	यदि (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	यदि (refspecs_list.nr) {
		पूर्णाङ्क i;

		प्रति (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.वस्तुएँ[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	यदि (use_done_feature)
		printf("feature done\n");

	यदि (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	यदि (import_filename)
		import_marks(import_filename, 0);
	अन्यथा यदि (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	यदि (import_filename && revs.prune_data.nr)
		full_वृक्ष = 1;

	get_tags_and_duplicates(&revs.cmdline);

	यदि (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	यावत् ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	यदि (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	यदि (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	प्रत्यागम 0;
}
