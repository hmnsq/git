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

स्थिर स्थिरांक अक्षर *स्थिरांक fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	रिक्त
};

स्थिर पूर्णांक progress;
स्थिर गणना sign_mode signed_tag_mode = SIGN_ABORT;
स्थिर गणना sign_mode signed_commit_mode = SIGN_STRIP;
स्थिर गणना tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
स्थिर गणना reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
स्थिर पूर्णांक fake_missing_tagger;
स्थिर पूर्णांक use_done_feature;
स्थिर पूर्णांक no_data;
स्थिर पूर्णांक full_वृक्ष;
स्थिर पूर्णांक reference_excluded_commits;
स्थिर पूर्णांक show_original_ids;
स्थिर पूर्णांक mark_tags;
स्थिर संरचना string_list extra_refs = STRING_LIST_INIT_DUP;
स्थिर संरचना string_list tag_refs = STRING_LIST_INIT_DUP;
स्थिर संरचना refspec refspecs;
स्थिर पूर्णांक anonymize;
स्थिर संरचना hashmap anonymized_seeds;
स्थिर संरचना revision_sources revision_sources;

स्थिर पूर्णांक parse_opt_sign_mode(स्थिरांक संरचना option *opt,
			       स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	गणना sign_mode *val = opt->मूल्य;

	जर (unset)
		परत 0;

	जर (parse_sign_mode(arg, val, रिक्त) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		परत error(_("unknown %s mode: %s"), opt->long_name, arg);

	परत 0;
}

स्थिर पूर्णांक parse_opt_tag_of_filtered_mode(स्थिरांक संरचना option *opt,
					  स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	गणना tag_of_filtered_mode *val = opt->मूल्य;

	जर (unset || !म_भेद(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	अन्यथा जर (!म_भेद(arg, "drop"))
		*val = DROP;
	अन्यथा जर (!म_भेद(arg, "rewrite"))
		*val = REWRITE;
	अन्यथा
		परत error(_("unknown tag-of-filtered mode: %s"), arg);
	परत 0;
}

स्थिर पूर्णांक parse_opt_reencode_mode(स्थिरांक संरचना option *opt,
				   स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	गणना reencode_mode *val = opt->मूल्य;

	जर (unset) {
		*val = REENCODE_ABORT;
		परत 0;
	}

	निवड (git_parse_maybe_bool(arg)) {
	स्थिती 0:
		*val = REENCODE_NO;
		थांबा;
	स्थिती 1:
		*val = REENCODE_YES;
		थांबा;
	पूर्वनिर्धारित:
		जर (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		अन्यथा
			परत error(_("unknown reencoding mode: %s"), arg);
	}

	परत 0;
}

स्थिर संरचना decoration idnums;
स्थिर uint32_t last_idnum;
संरचना anonymized_entry {
	संरचना hashmap_entry hash;
	अक्षर *anon;
	स्थिरांक अक्षर orig[FLEX_ARRAY];
};

संरचना anonymized_entry_किल्ली {
	संरचना hashmap_entry hash;
	स्थिरांक अक्षर *orig;
	माप_प्रकार orig_len;
};

स्थिर पूर्णांक anonymized_entry_cmp(स्थिरांक शून्य *cmp_data UNUSED,
				स्थिरांक संरचना hashmap_entry *eptr,
				स्थिरांक संरचना hashmap_entry *entry_or_किल्ली,
				स्थिरांक शून्य *keydata)
{
	स्थिरांक संरचना anonymized_entry *a, *b;

	a = container_of(eptr, स्थिरांक संरचना anonymized_entry, hash);
	जर (keydata) {
		स्थिरांक संरचना anonymized_entry_किल्ली *किल्ली = keydata;
		पूर्णांक equal = !xstrncmpz(a->orig, किल्ली->orig, किल्ली->orig_len);
		परत !equal;
	}

	b = container_of(entry_or_किल्ली, स्थिरांक संरचना anonymized_entry, hash);
	परत म_भेद(a->orig, b->orig);
}

स्थिर संरचना anonymized_entry *add_anonymized_entry(संरचना hashmap *map,
						     अचिन्हित hash,
						     स्थिरांक अक्षर *orig, माप_प्रकार len,
						     अक्षर *anon)
{
	संरचना anonymized_entry *ret, *old;

	जर (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, रिक्त, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	जर (old) {
		मुक्त(old->anon);
		मुक्त(old);
	}

	परत ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
स्थिर स्थिरांक अक्षर *anonymize_str(संरचना hashmap *map,
				 अक्षर *(*generate)(शून्य),
				 स्थिरांक अक्षर *orig, माप_प्रकार len)
{
	संरचना anonymized_entry_किल्ली किल्ली;
	संरचना anonymized_entry *ret;

	hashmap_entry_init(&किल्ली.hash, memhash(orig, len));
	किल्ली.orig = orig;
	किल्ली.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &किल्ली, hash, &किल्ली);

	/* ...otherwise check if we've already seen it in this context... */
	जर (!ret)
		ret = hashmap_get_entry(map, &किल्ली, hash, &किल्ली);

	/* ...and finally generate a new mapping if necessary */
	जर (!ret)
		ret = add_anonymized_entry(map, किल्ली.hash.hash,
					   orig, len, generate());

	परत ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
स्थिर शून्य anonymize_path(संरचना strbuf *out, स्थिरांक अक्षर *path,
			   संरचना hashmap *map,
			   अक्षर *(*generate)(शून्य))
{
	पर्यंत (*path) {
		स्थिरांक अक्षर *end_of_component = strchrnul(path, '/');
		माप_प्रकार len = end_of_component - path;
		स्थिरांक अक्षर *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		जर (*path)
			strbuf_addch(out, *path++);
	}
}

स्थिर ओळीत शून्य *mark_to_ptr(uint32_t mark)
{
	परत (शून्य *)(uintptr_t)mark;
}

स्थिर ओळीत uint32_t ptr_to_mark(शून्य * mark)
{
	परत (uint32_t)(uintptr_t)mark;
}

स्थिर ओळीत शून्य mark_object(संरचना object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

स्थिर ओळीत शून्य mark_next_object(संरचना object *object)
{
	mark_object(object, ++last_idnum);
}

स्थिर पूर्णांक get_object_mark(संरचना object *object)
{
	शून्य *decoration = lookup_decoration(&idnums, object);
	जर (!decoration)
		परत 0;
	परत ptr_to_mark(decoration);
}

स्थिर संरचना commit *rewrite_commit(संरचना commit *p)
{
	साठी (;;) {
		जर (p->parents && p->parents->next)
			थांबा;
		जर (p->object.flags & UNINTERESTING)
			थांबा;
		जर (!(p->object.flags & TREESAME))
			थांबा;
		जर (!p->parents)
			परत रिक्त;
		p = p->parents->item;
	}
	परत p;
}

स्थिर शून्य show_progress(शून्य)
{
	स्थिर पूर्णांक counter = 0;
	जर (!progress)
		परत;
	जर ((++counter % progress) == 0)
		म_लिखो("progress %d objects\n", counter);
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
स्थिर अक्षर *anonymize_blob(अचिन्हित दीर्घ *size)
{
	स्थिर पूर्णांक counter;
	संरचना strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	परत strbuf_detach(&out, रिक्त);
}

स्थिर शून्य export_blob(स्थिरांक संरचना object_id *oid)
{
	अचिन्हित दीर्घ size;
	गणना object_type type;
	अक्षर *buf;
	संरचना object *object;
	पूर्णांक eaten;

	जर (no_data)
		परत;

	जर (is_null_oid(oid))
		परत;

	object = lookup_object(the_repository, oid);
	जर (object && object->flags & SHOWN)
		परत;

	जर (anonymize) {
		buf = anonymize_blob(&size);
		object = (संरचना object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} अन्यथा {
		माप_प्रकार size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		जर (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		जर (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	जर (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	म_लिखो("blob\nmark :%"PRIu32"\n", last_idnum);
	जर (show_original_ids)
		म_लिखो("original-oid %s\n", oid_to_hex(oid));
	म_लिखो("data %"PRIuMAX"\n", (uintmax_t)size);
	जर (size && ख_डालो(buf, size, 1, मानक_निकास) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	म_लिखो("\n");

	show_progress();

	object->flags |= SHOWN;
	जर (!eaten)
		मुक्त(buf);
}

स्थिर पूर्णांक depth_first(स्थिरांक शून्य *a_, स्थिरांक शून्य *b_)
{
	स्थिरांक संरचना diff_filepair *a = *((स्थिरांक संरचना diff_filepair **)a_);
	स्थिरांक संरचना diff_filepair *b = *((स्थिरांक संरचना diff_filepair **)b_);
	स्थिरांक अक्षर *name_a, *name_b;
	पूर्णांक len_a, len_b, len;
	पूर्णांक cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = म_माप(name_a);
	len_b = म_माप(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = स_भेद(name_a, name_b, len);
	जर (cmp)
		परत cmp;
	cmp = len_b - len_a;
	जर (cmp)
		परत cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	परत (a->status == 'R') - (b->status == 'R');
}

स्थिर शून्य print_path_1(स्थिरांक अक्षर *path)
{
	पूर्णांक need_quote = quote_c_style(path, रिक्त, रिक्त, 0);
	जर (need_quote)
		quote_c_style(path, रिक्त, मानक_निकास, 0);
	अन्यथा जर (म_अक्षर(path, ' '))
		म_लिखो("\"%s\"", path);
	अन्यथा
		म_लिखो("%s", path);
}

स्थिर अक्षर *anonymize_path_component(शून्य)
{
	स्थिर पूर्णांक counter;
	संरचना strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	परत strbuf_detach(&out, रिक्त);
}

स्थिर शून्य print_path(स्थिरांक अक्षर *path)
{
	जर (!anonymize)
		print_path_1(path);
	अन्यथा {
		स्थिर संरचना hashmap paths;
		स्थिर संरचना strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

स्थिर अक्षर *generate_fake_oid(शून्य)
{
	स्थिर uint32_t counter = 1; /* avoid null oid */
	स्थिरांक अचिन्हित hashsz = the_hash_algo->rawsz;
	संरचना object_id oid;
	अक्षर *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	परत oid_to_hex_r(hex, &oid);
}

स्थिर स्थिरांक अक्षर *anonymize_oid(स्थिरांक अक्षर *oid_hex)
{
	स्थिर संरचना hashmap objs;
	माप_प्रकार len = म_माप(oid_hex);
	परत anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

स्थिर शून्य show_filemodify(संरचना diff_queue_struct *q,
			    संरचना diff_options *options UNUSED, शून्य *data)
{
	पूर्णांक i;
	संरचना string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	साठी (i = 0; i < q->nr; i++) {
		संरचना diff_filespec *ospec = q->queue[i]->one;
		संरचना diff_filespec *spec = q->queue[i]->two;

		निवड (q->queue[i]->status) {
		स्थिती DIFF_STATUS_DELETED:
			म_लिखो("D ");
			print_path(spec->path);
			string_list_घाला(changed, spec->path);
			अक्षर_दो('\n');
			थांबा;

		स्थिती DIFF_STATUS_COPIED:
		स्थिती DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			जर (!string_list_has_string(changed, ospec->path)) {
				म_लिखो("%c ", q->queue[i]->status);
				print_path(ospec->path);
				अक्षर_दो(' ');
				print_path(spec->path);
				string_list_घाला(changed, spec->path);
				अक्षर_दो('\n');

				जर (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					थांबा;
			}
			/* fallthrough */

		स्थिती DIFF_STATUS_TYPE_CHANGED:
		स्थिती DIFF_STATUS_MODIFIED:
		स्थिती DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			जर (no_data || S_ISGITLINK(spec->mode))
				म_लिखो("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			अन्यथा {
				संरचना object *object = lookup_object(the_repository,
								      &spec->oid);
				म_लिखो("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_घाला(changed, spec->path);
			अक्षर_दो('\n');
			थांबा;

		पूर्वनिर्धारित:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

स्थिर अक्षर *anonymize_ref_component(शून्य)
{
	स्थिर पूर्णांक counter;
	संरचना strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	परत strbuf_detach(&out, रिक्त);
}

स्थिर स्थिरांक अक्षर *anonymize_refname(स्थिरांक अक्षर *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	स्थिर स्थिरांक अक्षर *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	स्थिर संरचना hashmap refs;
	स्थिर संरचना strbuf anon = STRBUF_INIT;
	पूर्णांक i;

	strbuf_reset(&anon);
	साठी (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		जर (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			थांबा;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	परत anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
स्थिर अक्षर *anonymize_commit_message(शून्य)
{
	स्थिर पूर्णांक counter;
	परत xstrfmt("subject %d\n\nbody\n", counter++);
}

स्थिर अक्षर *anonymize_ident(शून्य)
{
	स्थिर पूर्णांक counter;
	संरचना strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	परत strbuf_detach(&out, रिक्त);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
स्थिर शून्य anonymize_ident_ओळ(स्थिरांक अक्षर **beg, स्थिरांक अक्षर **end)
{
	स्थिर संरचना hashmap idents;
	स्थिर संरचना strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	स्थिर अचिन्हित which_buffer;

	संरचना strbuf *out;
	संरचना ident_split split;
	स्थिरांक अक्षर *end_of_header;

	out = &buffers[which_buffer++];
	which_buffer %= ARRAY_SIZE(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = म_अक्षर(*beg, ' ');
	जर (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (पूर्णांक)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	जर (!split_ident_ओळ(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		स्थिरांक अक्षर *ident;
		माप_प्रकार len;

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
स्थिर स्थिरांक अक्षर *find_commit_multiline_header(स्थिरांक अक्षर *संदेश,
						स्थिरांक अक्षर *किल्ली,
						स्थिरांक अक्षर **end)
{
	संरचना strbuf val = STRBUF_INIT;
	स्थिरांक अक्षर *bol, *eol;
	माप_प्रकार len;

	bol = find_commit_header(संदेश, किल्ली, &len);
	जर (!bol)
		परत रिक्त;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	पर्यंत (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	परत strbuf_detach(&val, रिक्त);
}

स्थिर शून्य print_signature(स्थिरांक अक्षर *signature, स्थिरांक अक्षर *object_hash)
{
	जर (!signature)
		परत;

	म_लिखो("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (अचिन्हित)म_माप(signature),
	       signature);
}

स्थिर स्थिरांक अक्षर *append_signatures_for_header(संरचना string_list *signatures,
						स्थिरांक अक्षर *pos,
						स्थिरांक अक्षर *header,
						स्थिरांक अक्षर *object_hash)
{
	स्थिरांक अक्षर *signature;
	स्थिरांक अक्षर *start = pos;
	स्थिरांक अक्षर *end = pos;

	पर्यंत ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (शून्य *)object_hash;
		मुक्त((अक्षर *)signature);
		start = end;
	}

	परत end;
}

स्थिर शून्य handle_commit(संरचना commit *commit, संरचना rev_info *rev,
			  संरचना string_list *paths_of_changed_objects)
{
	पूर्णांक saved_output_format = rev->diffopt.output_format;
	स्थिरांक अक्षर *commit_buffer, *commit_buffer_cursor;
	स्थिरांक अक्षर *author, *author_end, *committer, *committer_end;
	स्थिरांक अक्षर *encoding = रिक्त;
	माप_प्रकार encoding_len;
	संरचना string_list signatures = STRING_LIST_INIT_DUP;
	स्थिरांक अक्षर *message;
	अक्षर *reencoded = रिक्त;
	संरचना commit_list *p;
	स्थिरांक अक्षर *refname;
	पूर्णांक i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, रिक्त);

	author = म_माला(commit_buffer_cursor, "\nauthor ");
	जर (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = म_माला(commit_buffer_cursor, "\ncommitter ");
	जर (!committer)
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

	जर (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		जर (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	जर (*commit_buffer_cursor == '\n') {
		स्थिरांक अक्षर *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		स्थिरांक अक्षर *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = म_माला(commit_buffer_cursor, "\n\n");
	जर (message)
		message += 2;

	जर (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_वृक्ष) {
		parse_commit_or_die(commit->parents->item);
		diff_वृक्ष_oid(get_commit_वृक्ष_oid(commit->parents->item),
			      get_commit_वृक्ष_oid(commit), "", &rev->diffopt);
	}
	अन्यथा
		diff_मूळ_वृक्ष_oid(get_commit_वृक्ष_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	साठी (i = 0; i < diff_queued_diff.nr; i++)
		जर (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	जर (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_ओळ(&committer, &committer_end);
		anonymize_ident_ओळ(&author, &author_end);
	}

	mark_next_object(&commit->object);
	जर (anonymize) {
		reencoded = anonymize_commit_message();
	} अन्यथा जर (encoding) {
		अक्षर *buf;
		निवड (reencode_mode) {
		स्थिती REENCODE_YES:
			buf = xstrfmt("%.*s", (पूर्णांक)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			मुक्त(buf);
			थांबा;
		स्थिती REENCODE_NO:
			थांबा;
		स्थिती REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (पूर्णांक)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	जर (!commit->parents)
		म_लिखो("reset %s\n", refname);
	म_लिखो("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	जर (show_original_ids)
		म_लिखो("original-oid %s\n", oid_to_hex(&commit->object.oid));
	म_लिखो("%.*s\n%.*s\n",
	       (पूर्णांक)(author_end - author), author,
	       (पूर्णांक)(committer_end - committer), committer);
	जर (signatures.nr) {
		निवड (signed_commit_mode) {
		/* Exporting modes */
		स्थिती SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		स्थिती SIGN_VERBATIM:
			साठी (माप_प्रकार i = 0; i < signatures.nr; i++) {
				संरचना string_list_item *item = &signatures.वस्तुसंच[i];
				print_signature(item->string, item->util);
			}
			थांबा;

		/* Stripping modes */
		स्थिती SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		स्थिती SIGN_STRIP:
			थांबा;

		/* Aborting modes */
		स्थिती SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		पूर्वनिर्धारित:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	जर (!reencoded && encoding)
		म_लिखो("encoding %.*s\n", (पूर्णांक)encoding_len, encoding);
	म_लिखो("data %u\n%s",
	       (अचिन्हित)(reencoded
			  ? म_माप(reencoded) : message
			  ? म_माप(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	मुक्त(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	साठी (i = 0, p = commit->parents; p; p = p->next) {
		संरचना object *obj = &p->item->object;
		पूर्णांक mark = get_object_mark(obj);

		जर (!mark && !reference_excluded_commits)
			सुरूठेवा;
		जर (i == 0)
			म_लिखो("from ");
		अन्यथा
			म_लिखो("merge ");
		जर (mark)
			म_लिखो(":%d\n", mark);
		अन्यथा
			म_लिखो("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	जर (full_वृक्ष)
		म_लिखो("deleteall\n");
	log_वृक्ष_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	म_लिखो("\n");

	show_progress();
}

स्थिर अक्षर *anonymize_tag(शून्य)
{
	स्थिर पूर्णांक counter;
	संरचना strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	परत strbuf_detach(&out, रिक्त);
}


स्थिर शून्य handle_tag(स्थिरांक अक्षर *name, संरचना tag *tag)
{
	माप_प्रकार size;
	गणना object_type type;
	अक्षर *buf;
	स्थिरांक अक्षर *tagger, *tagger_end, *message;
	माप_प्रकार message_size = 0;
	संरचना object *tagged;
	पूर्णांक tagged_mark;
	संरचना commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	पर्यंत (tagged->type == OBJ_TAG) {
		tagged = ((संरचना tag *)tagged)->tagged;
	}
	जर (tagged->type == OBJ_वृक्ष) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		परत;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	जर (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	जर (message) {
		message += 2;
		message_size = म_माप(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	जर (!tagger) {
		जर (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		अन्यथा
			tagger = "";
		tagger_end = tagger + म_माप(tagger);
	} अन्यथा {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		जर (anonymize)
			anonymize_ident_ओळ(&tagger, &tagger_end);
	}

	जर (anonymize) {
		name = anonymize_refname(name);
		जर (message) {
			स्थिर संरचना hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = म_माप(message);
		}
	}

	/* handle signed tags */
	जर (message) {
		माप_प्रकार sig_offset = parse_signed_buffer(message, message_size);
		जर (sig_offset < message_size)
			निवड (signed_tag_mode) {
			/* Exporting modes */
			स्थिती SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			स्थिती SIGN_VERBATIM:
				थांबा;

			/* Stripping modes */
			स्थिती SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			स्थिती SIGN_STRIP:
				message_size = sig_offset;
				थांबा;

			/* Aborting modes */
			स्थिती SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			पूर्वनिर्धारित:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	जर (!tagged_mark) {
		निवड (tag_of_filtered_mode) {
		स्थिती TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		स्थिती DROP:
			/* Ignore this tag altogether */
			मुक्त(buf);
			परत;
		स्थिती REWRITE:
			जर (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} अन्यथा जर (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((संरचना commit *)tagged);
				जर (!p) {
					म_लिखो("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					मुक्त(buf);
					परत;
				}
				tagged_mark = get_object_mark(&p->object);
			} अन्यथा {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	जर (tagged->type == OBJ_TAG) {
		म_लिखो("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	म_लिखो("tag %s\n", name);
	जर (mark_tags) {
		mark_next_object(&tag->object);
		म_लिखो("mark :%"PRIu32"\n", last_idnum);
	}
	जर (tagged_mark)
		म_लिखो("from :%d\n", tagged_mark);
	अन्यथा
		म_लिखो("from %s\n", oid_to_hex(&tagged->oid));

	जर (show_original_ids)
		म_लिखो("original-oid %s\n", oid_to_hex(&tag->object.oid));
	म_लिखो("%.*s%sdata %d\n%.*s\n",
	       (पूर्णांक)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (पूर्णांक)message_size, (पूर्णांक)message_size, message ? message : "");
	मुक्त(buf);
}

स्थिर संरचना commit *get_commit(संरचना rev_cmdline_entry *e, स्थिरांक अक्षर *full_name)
{
	निवड (e->item->type) {
	स्थिती OBJ_COMMIT:
		परत (संरचना commit *)e->item;
	स्थिती OBJ_TAG: {
		संरचना tag *tag = (संरचना tag *)e->item;

		/* handle nested tags */
		पर्यंत (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (संरचना tag *)tag->tagged;
		}
		जर (!tag)
			die(_("tag %s points nowhere?"), e->name);
		परत (संरचना commit *)tag;
	}
	पूर्वनिर्धारित:
		परत रिक्त;
	}
}

स्थिर शून्य get_tags_and_duplicates(संरचना rev_cmdline_info *info)
{
	पूर्णांक i;

	साठी (i = 0; i < info->nr; i++) {
		संरचना rev_cmdline_entry *e = info->rev + i;
		संरचना object_id oid;
		संरचना commit *commit;
		अक्षर *full_name = रिक्त;

		जर (e->flags & UNINTERESTING)
			सुरूठेवा;

		जर (repo_dwim_ref(the_repository, e->name, म_माप(e->name),
				  &oid, &full_name, 0) != 1) {
			मुक्त(full_name);
			सुरूठेवा;
		}

		जर (refspecs.nr) {
			अक्षर *खाजगी;
			खाजगी = apply_refspecs(&refspecs, full_name);
			जर (खाजगी) {
				मुक्त(full_name);
				full_name = खाजगी;
			}
		}

		commit = get_commit(e, full_name);
		जर (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			मुक्त(full_name);
			सुरूठेवा;
		}

		निवड (commit->object.type) {
		स्थिती OBJ_COMMIT:
			थांबा;
		स्थिती OBJ_BLOB:
			export_blob(&commit->object.oid);
			मुक्त(full_name);
			सुरूठेवा;
		पूर्वनिर्धारित: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			मुक्त(full_name);
			सुरूठेवा;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		जर (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		जर (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		अन्यथा
			मुक्त(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

स्थिर शून्य handle_tags_and_duplicates(संरचना string_list *extras)
{
	संरचना commit *commit;
	पूर्णांक i;

	साठी (i = extras->nr - 1; i >= 0; i--) {
		स्थिरांक अक्षर *name = extras->वस्तुसंच[i].string;
		संरचना object *object = extras->वस्तुसंच[i].util;
		पूर्णांक mark;

		निवड (object->type) {
		स्थिती OBJ_TAG:
			handle_tag(name, (संरचना tag *)object);
			थांबा;
		स्थिती OBJ_COMMIT:
			जर (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((संरचना commit *)object);
			जर (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				म_लिखो("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				सुरूठेवा;
			}

			mark = get_object_mark(&commit->object);
			जर (!mark) {
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
				जर (!reference_excluded_commits) {
					/* delete the ref */
					म_लिखो("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					सुरूठेवा;
				}
				/* set ref to commit using oid, not mark */
				म_लिखो("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				सुरूठेवा;
			}

			म_लिखो("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			थांबा;
		}
	}
}

स्थिर शून्य export_marks(अक्षर *file)
{
	अचिन्हित पूर्णांक i;
	uint32_t mark;
	संरचना decoration_entry *deco = idnums.entries;
	खाता *f;
	पूर्णांक e = 0;

	f = fopen_for_writing(file);
	जर (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	साठी (i = 0; i < idnums.size; i++) {
		जर (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			जर (ख_लिखो(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    थांबा;
			}
		}
		deco++;
	}

	e |= ख_त्रुटि(f);
	e |= ख_बंद(f);
	जर (e)
		error(_("unable to write marks file %s."), file);
}

स्थिर शून्य import_marks(अक्षर *आदान_file, पूर्णांक check_exists)
{
	अक्षर ओळ[512];
	खाता *f;
	संरचना stat sb;

	जर (check_exists && stat(आदान_file, &sb))
		परत;

	f = xfopen(आदान_file, "r");
	पर्यंत (ख_माला_लो(ओळ, आकार(ओळ), f)) {
		uint32_t mark;
		अक्षर *ओळ_end, *mark_end;
		संरचना object_id oid;
		संरचना object *object;
		संरचना commit *commit;
		गणना object_type type;

		ओळ_end = म_अक्षर(ओळ, '\n');
		जर (ओळ[0] != ':' || !ओळ_end)
			die(_("corrupt mark line: %s"), ओळ);
		*ओळ_end = '\0';

		mark = strtoumax(ओळ + 1, &mark_end, 10);
		जर (!mark || mark_end == ओळ + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), ओळ);

		जर (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, रिक्त);
		जर (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		जर (type != OBJ_COMMIT)
			/* only commits */
			सुरूठेवा;

		commit = lookup_commit(the_repository, &oid);
		जर (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		जर (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	ख_बंद(f);
}

स्थिर शून्य handle_deletes(शून्य)
{
	पूर्णांक i;
	साठी (i = 0; i < refspecs.nr; i++) {
		संरचना refspec_item *refspec = &refspecs.वस्तुसंच[i];
		जर (*refspec->src)
			सुरूठेवा;

		म_लिखो("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

स्थिर पूर्णांक parse_opt_anonymize_map(स्थिरांक संरचना option *opt,
				   स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना hashmap *map = opt->मूल्य;
	स्थिरांक अक्षर *delim, *मूल्य;
	माप_प्रकार keylen;

	BUG_ON_OPT_NEG(unset);

	delim = म_अक्षर(arg, ':');
	जर (delim) {
		keylen = delim - arg;
		मूल्य = delim + 1;
	} अन्यथा {
		keylen = म_माप(arg);
		मूल्य = arg;
	}

	जर (!keylen || !*मूल्य)
		परत error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(मूल्य));

	परत 0;
}

पूर्णांक cmd_fast_export(पूर्णांक argc,
		    स्थिरांक अक्षर **argv,
		    स्थिरांक अक्षर *prefix,
		    संरचना repository *repo UNUSED)
{
	संरचना rev_info revs;
	संरचना commit *commit;
	अक्षर *export_filename = रिक्त,
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

	जर (argc == 1)
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
	जर (argc > 1)
		usage_with_options (fast_export_usage, options);

	जर (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	जर (refspecs_list.nr) {
		पूर्णांक i;

		साठी (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.वस्तुसंच[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	जर (use_done_feature)
		म_लिखो("feature done\n");

	जर (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	जर (import_filename)
		import_marks(import_filename, 0);
	अन्यथा जर (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	जर (import_filename && revs.prune_data.nr)
		full_वृक्ष = 1;

	get_tags_and_duplicates(&revs.cmdline);

	जर (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	पर्यंत ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	जर (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	जर (use_done_feature)
		म_लिखो("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	परत 0;
}
