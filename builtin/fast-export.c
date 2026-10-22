/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "refs.h"
#समावेश "refspec.h"
#समावेश "object-file.h"
#समावेश "odb.h"
#समावेश "commit.h"
#समावेश "object.h"
#समावेश "tag.h"
#समावेश "diff.h"
#समावेश "diffcore.h"
#समावेश "log-tree.h"
#समावेश "revision.h"
#समावेश "decorate.h"
#समावेश "string-list.h"
#समावेश "utf8.h"
#समावेश "parse-options.h"
#समावेश "quote.h"
#समावेश "remote.h"
#समावेश "blob.h"
#समावेश "gpg-interface.h"

अटल स्थिर अक्षर *स्थिर fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	शून्य
};

अटल पूर्णांक progress;
अटल क्रमागत sign_mode signed_tag_mode = SIGN_ABORT;
अटल क्रमागत sign_mode signed_commit_mode = SIGN_STRIP;
अटल क्रमागत tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
अटल क्रमागत reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
अटल पूर्णांक fake_missing_tagger;
अटल पूर्णांक use_done_feature;
अटल पूर्णांक no_तथ्य;
अटल पूर्णांक full_वृक्ष;
अटल पूर्णांक reference_excluded_commits;
अटल पूर्णांक show_original_ids;
अटल पूर्णांक mark_tags;
अटल काष्ठा string_list extra_refs = STRING_LIST_INIT_DUP;
अटल काष्ठा string_list tag_refs = STRING_LIST_INIT_DUP;
अटल काष्ठा refspec refspecs;
अटल पूर्णांक anonymize;
अटल काष्ठा hashmap anonymized_seeds;
अटल काष्ठा revision_sources revision_sources;

अटल पूर्णांक parse_opt_sign_mode(स्थिर काष्ठा option *opt,
			       स्थिर अक्षर *arg, पूर्णांक unset)
{
	क्रमागत sign_mode *val = opt->मूल्य;

	अगर (unset)
		वापस 0;

	अगर (parse_sign_mode(arg, val, शून्य) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		वापस error(_("unknown %s mode: %s"), opt->long_name, arg);

	वापस 0;
}

अटल पूर्णांक parse_opt_tag_of_filtered_mode(स्थिर काष्ठा option *opt,
					  स्थिर अक्षर *arg, पूर्णांक unset)
{
	क्रमागत tag_of_filtered_mode *val = opt->मूल्य;

	अगर (unset || !म_भेद(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	अन्यथा अगर (!म_भेद(arg, "drop"))
		*val = DROP;
	अन्यथा अगर (!म_भेद(arg, "rewrite"))
		*val = REWRITE;
	अन्यथा
		वापस error(_("unknown tag-of-filtered mode: %s"), arg);
	वापस 0;
}

अटल पूर्णांक parse_opt_reencode_mode(स्थिर काष्ठा option *opt,
				   स्थिर अक्षर *arg, पूर्णांक unset)
{
	क्रमागत reencode_mode *val = opt->मूल्य;

	अगर (unset) {
		*val = REENCODE_ABORT;
		वापस 0;
	}

	चयन (git_parse_maybe_bool(arg)) {
	हाल 0:
		*val = REENCODE_NO;
		अवरोध;
	हाल 1:
		*val = REENCODE_YES;
		अवरोध;
	शेष:
		अगर (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		अन्यथा
			वापस error(_("unknown reencoding mode: %s"), arg);
	}

	वापस 0;
}

अटल काष्ठा decoration idnums;
अटल uint32_t last_idnum;
काष्ठा anonymized_entry {
	काष्ठा hashmap_entry hash;
	अक्षर *anon;
	स्थिर अक्षर orig[FLEX_ARRAY];
};

काष्ठा anonymized_entry_कुंजी {
	काष्ठा hashmap_entry hash;
	स्थिर अक्षर *orig;
	माप_प्रकार orig_len;
};

अटल पूर्णांक anonymized_entry_cmp(स्थिर व्योम *cmp_तथ्य UNUSED,
				स्थिर काष्ठा hashmap_entry *eptr,
				स्थिर काष्ठा hashmap_entry *entry_or_कुंजी,
				स्थिर व्योम *keydata)
{
	स्थिर काष्ठा anonymized_entry *a, *b;

	a = container_of(eptr, स्थिर काष्ठा anonymized_entry, hash);
	अगर (keydata) {
		स्थिर काष्ठा anonymized_entry_कुंजी *कुंजी = keydata;
		पूर्णांक equal = !xstrncmpz(a->orig, कुंजी->orig, कुंजी->orig_len);
		वापस !equal;
	}

	b = container_of(entry_or_कुंजी, स्थिर काष्ठा anonymized_entry, hash);
	वापस म_भेद(a->orig, b->orig);
}

अटल काष्ठा anonymized_entry *add_anonymized_entry(काष्ठा hashmap *map,
						     अचिन्हित hash,
						     स्थिर अक्षर *orig, माप_प्रकार len,
						     अक्षर *anon)
{
	काष्ठा anonymized_entry *ret, *old;

	अगर (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, शून्य, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	अगर (old) {
		मुक्त(old->anon);
		मुक्त(old);
	}

	वापस ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
अटल स्थिर अक्षर *anonymize_str(काष्ठा hashmap *map,
				 अक्षर *(*generate)(व्योम),
				 स्थिर अक्षर *orig, माप_प्रकार len)
{
	काष्ठा anonymized_entry_कुंजी कुंजी;
	काष्ठा anonymized_entry *ret;

	hashmap_entry_init(&कुंजी.hash, memhash(orig, len));
	कुंजी.orig = orig;
	कुंजी.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &कुंजी, hash, &कुंजी);

	/* ...otherwise check if we've already seen it in this context... */
	अगर (!ret)
		ret = hashmap_get_entry(map, &कुंजी, hash, &कुंजी);

	/* ...and finally generate a new mapping if necessary */
	अगर (!ret)
		ret = add_anonymized_entry(map, कुंजी.hash.hash,
					   orig, len, generate());

	वापस ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
अटल व्योम anonymize_path(काष्ठा strbuf *out, स्थिर अक्षर *path,
			   काष्ठा hashmap *map,
			   अक्षर *(*generate)(व्योम))
{
	जबतक (*path) {
		स्थिर अक्षर *end_of_component = strchrnul(path, '/');
		माप_प्रकार len = end_of_component - path;
		स्थिर अक्षर *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		अगर (*path)
			strbuf_addch(out, *path++);
	}
}

अटल अंतरभूत व्योम *mark_to_ptr(uint32_t mark)
{
	वापस (व्योम *)(uintptr_t)mark;
}

अटल अंतरभूत uint32_t ptr_to_mark(व्योम * mark)
{
	वापस (uint32_t)(uintptr_t)mark;
}

अटल अंतरभूत व्योम mark_object(काष्ठा object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

अटल अंतरभूत व्योम mark_अग्र_object(काष्ठा object *object)
{
	mark_object(object, ++last_idnum);
}

अटल पूर्णांक get_object_mark(काष्ठा object *object)
{
	व्योम *decoration = lookup_decoration(&idnums, object);
	अगर (!decoration)
		वापस 0;
	वापस ptr_to_mark(decoration);
}

अटल काष्ठा commit *rewrite_commit(काष्ठा commit *p)
{
	क्रम (;;) {
		अगर (p->parents && p->parents->अग्र)
			अवरोध;
		अगर (p->object.flags & UNINTERESTING)
			अवरोध;
		अगर (!(p->object.flags & TREESAME))
			अवरोध;
		अगर (!p->parents)
			वापस शून्य;
		p = p->parents->item;
	}
	वापस p;
}

अटल व्योम show_progress(व्योम)
{
	अटल पूर्णांक गणक = 0;
	अगर (!progress)
		वापस;
	अगर ((++गणक % progress) == 0)
		म_लिखो("progress %d objects\n", गणक);
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
अटल अक्षर *anonymize_blob(अचिन्हित दीर्घ *आकार)
{
	अटल पूर्णांक गणक;
	काष्ठा strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", गणक++);
	*आकार = out.len;
	वापस strbuf_detach(&out, शून्य);
}

अटल व्योम export_blob(स्थिर काष्ठा object_id *oid)
{
	अचिन्हित दीर्घ आकार;
	क्रमागत object_type type;
	अक्षर *buf;
	काष्ठा object *object;
	पूर्णांक eaten;

	अगर (no_तथ्य)
		वापस;

	अगर (is_null_oid(oid))
		वापस;

	object = lookup_object(the_repository, oid);
	अगर (object && object->flags & SHOWN)
		वापस;

	अगर (anonymize) {
		buf = anonymize_blob(&आकार);
		object = (काष्ठा object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} अन्यथा {
		माप_प्रकार आकार_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &आकार_st);
		आकार = cast_आकार_t_to_ulong(आकार_st);
		अगर (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		अगर (check_object_signature(the_repository, oid, buf, आकार,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_बफर(the_repository, oid, type,
					     आकार, buf, &eaten);
	}

	अगर (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_अग्र_object(object);

	म_लिखो("blob\nmark :%"PRIu32"\n", last_idnum);
	अगर (show_original_ids)
		म_लिखो("original-oid %s\n", oid_to_hex(oid));
	म_लिखो("data %"PRIuMAX"\n", (uintmax_t)आकार);
	अगर (आकार && ख_डालो(buf, आकार, 1, मानक_निकास) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	म_लिखो("\n");

	show_progress();

	object->flags |= SHOWN;
	अगर (!eaten)
		मुक्त(buf);
}

अटल पूर्णांक depth_first(स्थिर व्योम *a_, स्थिर व्योम *b_)
{
	स्थिर काष्ठा diff_filepair *a = *((स्थिर काष्ठा diff_filepair **)a_);
	स्थिर काष्ठा diff_filepair *b = *((स्थिर काष्ठा diff_filepair **)b_);
	स्थिर अक्षर *name_a, *name_b;
	पूर्णांक len_a, len_b, len;
	पूर्णांक cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = म_माप(name_a);
	len_b = म_माप(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = स_भेद(name_a, name_b, len);
	अगर (cmp)
		वापस cmp;
	cmp = len_b - len_a;
	अगर (cmp)
		वापस cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	वापस (a->status == 'R') - (b->status == 'R');
}

अटल व्योम print_path_1(स्थिर अक्षर *path)
{
	पूर्णांक need_quote = quote_c_style(path, शून्य, शून्य, 0);
	अगर (need_quote)
		quote_c_style(path, शून्य, मानक_निकास, 0);
	अन्यथा अगर (म_अक्षर(path, ' '))
		म_लिखो("\"%s\"", path);
	अन्यथा
		म_लिखो("%s", path);
}

अटल अक्षर *anonymize_path_component(व्योम)
{
	अटल पूर्णांक गणक;
	काष्ठा strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", गणक++);
	वापस strbuf_detach(&out, शून्य);
}

अटल व्योम print_path(स्थिर अक्षर *path)
{
	अगर (!anonymize)
		print_path_1(path);
	अन्यथा {
		अटल काष्ठा hashmap paths;
		अटल काष्ठा strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

अटल अक्षर *generate_fake_oid(व्योम)
{
	अटल uint32_t गणक = 1; /* avoid null oid */
	स्थिर अचिन्हित hashsz = the_hash_algo->rawsz;
	काष्ठा object_id oid;
	अक्षर *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, गणक++);
	वापस oid_to_hex_r(hex, &oid);
}

अटल स्थिर अक्षर *anonymize_oid(स्थिर अक्षर *oid_hex)
{
	अटल काष्ठा hashmap objs;
	माप_प्रकार len = म_माप(oid_hex);
	वापस anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

अटल व्योम show_filemodify(काष्ठा diff_queue_struct *q,
			    काष्ठा diff_options *options UNUSED, व्योम *तथ्य)
{
	पूर्णांक i;
	काष्ठा string_list *changed = तथ्य;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	क्रम (i = 0; i < q->nr; i++) {
		काष्ठा diff_filespec *ospec = q->queue[i]->one;
		काष्ठा diff_filespec *spec = q->queue[i]->two;

		चयन (q->queue[i]->status) {
		हाल DIFF_STATUS_DELETED:
			म_लिखो("D ");
			print_path(spec->path);
			string_list_जोड़ो(changed, spec->path);
			अक्षर_दो('\n');
			अवरोध;

		हाल DIFF_STATUS_COPIED:
		हाल DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			अगर (!string_list_has_string(changed, ospec->path)) {
				म_लिखो("%c ", q->queue[i]->status);
				print_path(ospec->path);
				अक्षर_दो(' ');
				print_path(spec->path);
				string_list_जोड़ो(changed, spec->path);
				अक्षर_दो('\n');

				अगर (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					अवरोध;
			}
			/* fallthrough */

		हाल DIFF_STATUS_TYPE_CHANGED:
		हाल DIFF_STATUS_MODIFIED:
		हाल DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			अगर (no_तथ्य || S_ISGITLINK(spec->mode))
				म_लिखो("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			अन्यथा {
				काष्ठा object *object = lookup_object(the_repository,
								      &spec->oid);
				म_लिखो("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_जोड़ो(changed, spec->path);
			अक्षर_दो('\n');
			अवरोध;

		शेष:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

अटल अक्षर *anonymize_ref_component(व्योम)
{
	अटल पूर्णांक गणक;
	काष्ठा strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", गणक++);
	वापस strbuf_detach(&out, शून्य);
}

अटल स्थिर अक्षर *anonymize_refname(स्थिर अक्षर *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	अटल स्थिर अक्षर *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	अटल काष्ठा hashmap refs;
	अटल काष्ठा strbuf anon = STRBUF_INIT;
	पूर्णांक i;

	strbuf_reset(&anon);
	क्रम (i = 0; i < ARRAY_आकार(prefixes); i++) {
		अगर (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			अवरोध;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	वापस anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
अटल अक्षर *anonymize_commit_message(व्योम)
{
	अटल पूर्णांक गणक;
	वापस xstrfmt("subject %d\n\nbody\n", गणक++);
}

अटल अक्षर *anonymize_ident(व्योम)
{
	अटल पूर्णांक गणक;
	काष्ठा strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", गणक, गणक);
	गणक++;
	वापस strbuf_detach(&out, शून्य);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
अटल व्योम anonymize_ident_पंक्ति(स्थिर अक्षर **beg, स्थिर अक्षर **end)
{
	अटल काष्ठा hashmap idents;
	अटल काष्ठा strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	अटल अचिन्हित which_बफर;

	काष्ठा strbuf *out;
	काष्ठा ident_split split;
	स्थिर अक्षर *end_of_header;

	out = &buffers[which_बफर++];
	which_बफर %= ARRAY_आकार(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = म_अक्षर(*beg, ' ');
	अगर (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (पूर्णांक)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	अगर (!split_ident_पंक्ति(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		स्थिर अक्षर *ident;
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
अटल स्थिर अक्षर *find_commit_multiline_header(स्थिर अक्षर *संदेश,
						स्थिर अक्षर *कुंजी,
						स्थिर अक्षर **end)
{
	काष्ठा strbuf val = STRBUF_INIT;
	स्थिर अक्षर *bol, *eol;
	माप_प्रकार len;

	bol = find_commit_header(संदेश, कुंजी, &len);
	अगर (!bol)
		वापस शून्य;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	जबतक (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	वापस strbuf_detach(&val, शून्य);
}

अटल व्योम print_signature(स्थिर अक्षर *signature, स्थिर अक्षर *object_hash)
{
	अगर (!signature)
		वापस;

	म_लिखो("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (अचिन्हित)म_माप(signature),
	       signature);
}

अटल स्थिर अक्षर *append_signatures_for_header(काष्ठा string_list *signatures,
						स्थिर अक्षर *pos,
						स्थिर अक्षर *header,
						स्थिर अक्षर *object_hash)
{
	स्थिर अक्षर *signature;
	स्थिर अक्षर *start = pos;
	स्थिर अक्षर *end = pos;

	जबतक ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (व्योम *)object_hash;
		मुक्त((अक्षर *)signature);
		start = end;
	}

	वापस end;
}

अटल व्योम handle_commit(काष्ठा commit *commit, काष्ठा rev_info *rev,
			  काष्ठा string_list *paths_of_changed_objects)
{
	पूर्णांक saved_output_format = rev->diffopt.output_format;
	स्थिर अक्षर *commit_बफर, *commit_बफर_cursor;
	स्थिर अक्षर *author, *author_end, *committer, *committer_end;
	स्थिर अक्षर *encoding = शून्य;
	माप_प्रकार encoding_len;
	काष्ठा string_list signatures = STRING_LIST_INIT_DUP;
	स्थिर अक्षर *message;
	अक्षर *reencoded = शून्य;
	काष्ठा commit_list *p;
	स्थिर अक्षर *refname;
	पूर्णांक i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_बफर_cursor = commit_बफर = repo_get_commit_बफर(the_repository, commit, शून्य);

	author = म_माला(commit_बफर_cursor, "\nauthor ");
	अगर (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_बफर_cursor = author_end = strchrnul(author, '\n');

	committer = म_माला(commit_बफर_cursor, "\ncommitter ");
	अगर (!committer)
		die(_("could not find committer in commit %s"),
		    oid_to_hex(&commit->object.oid));
	committer++;
	commit_बफर_cursor = committer_end = strchrnul(committer, '\n');

	/*
	 * find_commit_header() and find_commit_multiline_header() get
	 * a `+ 1` because commit_buffer_cursor points at the trailing
	 * "\n" at the end of the previous line, but they want a
	 * pointer to the beginning of the next line.
	 */

	अगर (*commit_बफर_cursor == '\n') {
		encoding = find_commit_header(commit_बफर_cursor + 1, "encoding", &encoding_len);
		अगर (encoding)
			commit_बफर_cursor = encoding + encoding_len;
	}

	अगर (*commit_बफर_cursor == '\n') {
		स्थिर अक्षर *after_sha1 = append_signatures_for_header(&signatures, commit_बफर_cursor,
								      "gpgsig", "sha1");
		स्थिर अक्षर *after_sha256 = append_signatures_for_header(&signatures, commit_बफर_cursor,
									"gpgsig-sha256", "sha256");
		commit_बफर_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = म_माला(commit_बफर_cursor, "\n\n");
	अगर (message)
		message += 2;

	अगर (commit->parents &&
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
	क्रम (i = 0; i < diff_queued_diff.nr; i++)
		अगर (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	अगर (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_पंक्ति(&committer, &committer_end);
		anonymize_ident_पंक्ति(&author, &author_end);
	}

	mark_अग्र_object(&commit->object);
	अगर (anonymize) {
		reencoded = anonymize_commit_message();
	} अन्यथा अगर (encoding) {
		अक्षर *buf;
		चयन (reencode_mode) {
		हाल REENCODE_YES:
			buf = xstrfmt("%.*s", (पूर्णांक)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			मुक्त(buf);
			अवरोध;
		हाल REENCODE_NO:
			अवरोध;
		हाल REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (पूर्णांक)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	अगर (!commit->parents)
		म_लिखो("reset %s\n", refname);
	म_लिखो("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	अगर (show_original_ids)
		म_लिखो("original-oid %s\n", oid_to_hex(&commit->object.oid));
	म_लिखो("%.*s\n%.*s\n",
	       (पूर्णांक)(author_end - author), author,
	       (पूर्णांक)(committer_end - committer), committer);
	अगर (signatures.nr) {
		चयन (signed_commit_mode) {
		/* Exporting modes */
		हाल SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		हाल SIGN_VERBATIM:
			क्रम (माप_प्रकार i = 0; i < signatures.nr; i++) {
				काष्ठा string_list_item *item = &signatures.वस्तुएँ[i];
				print_signature(item->string, item->util);
			}
			अवरोध;

		/* Stripping modes */
		हाल SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		हाल SIGN_STRIP:
			अवरोध;

		/* Aborting modes */
		हाल SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		शेष:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	अगर (!reencoded && encoding)
		म_लिखो("encoding %.*s\n", (पूर्णांक)encoding_len, encoding);
	म_लिखो("data %u\n%s",
	       (अचिन्हित)(reencoded
			  ? म_माप(reencoded) : message
			  ? म_माप(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	मुक्त(reencoded);
	repo_unuse_commit_बफर(the_repository, commit, commit_बफर);

	क्रम (i = 0, p = commit->parents; p; p = p->अग्र) {
		काष्ठा object *obj = &p->item->object;
		पूर्णांक mark = get_object_mark(obj);

		अगर (!mark && !reference_excluded_commits)
			जारी;
		अगर (i == 0)
			म_लिखो("from ");
		अन्यथा
			म_लिखो("merge ");
		अगर (mark)
			म_लिखो(":%d\n", mark);
		अन्यथा
			म_लिखो("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	अगर (full_वृक्ष)
		म_लिखो("deleteall\n");
	log_वृक्ष_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	म_लिखो("\n");

	show_progress();
}

अटल अक्षर *anonymize_tag(व्योम)
{
	अटल पूर्णांक गणक;
	काष्ठा strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", गणक++);
	वापस strbuf_detach(&out, शून्य);
}


अटल व्योम handle_tag(स्थिर अक्षर *name, काष्ठा tag *tag)
{
	माप_प्रकार आकार;
	क्रमागत object_type type;
	अक्षर *buf;
	स्थिर अक्षर *tagger, *tagger_end, *message;
	माप_प्रकार message_आकार = 0;
	काष्ठा object *tagged;
	पूर्णांक tagged_mark;
	काष्ठा commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	जबतक (tagged->type == OBJ_TAG) {
		tagged = ((काष्ठा tag *)tagged)->tagged;
	}
	अगर (tagged->type == OBJ_वृक्ष) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		वापस;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &आकार);
	अगर (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, आकार, "\n\n", 2);
	अगर (message) {
		message += 2;
		message_आकार = म_माप(message);
	}
	tagger = memmem(buf, message ? message - buf : आकार, "\ntagger ", 8);
	अगर (!tagger) {
		अगर (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		अन्यथा
			tagger = "";
		tagger_end = tagger + म_माप(tagger);
	} अन्यथा {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		अगर (anonymize)
			anonymize_ident_पंक्ति(&tagger, &tagger_end);
	}

	अगर (anonymize) {
		name = anonymize_refname(name);
		अगर (message) {
			अटल काष्ठा hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_आकार);
			message_आकार = म_माप(message);
		}
	}

	/* handle signed tags */
	अगर (message) {
		माप_प्रकार sig_offset = parse_signed_बफर(message, message_आकार);
		अगर (sig_offset < message_आकार)
			चयन (signed_tag_mode) {
			/* Exporting modes */
			हाल SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			हाल SIGN_VERBATIM:
				अवरोध;

			/* Stripping modes */
			हाल SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			हाल SIGN_STRIP:
				message_आकार = sig_offset;
				अवरोध;

			/* Aborting modes */
			हाल SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			शेष:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	अगर (!tagged_mark) {
		चयन (tag_of_filtered_mode) {
		हाल TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		हाल DROP:
			/* Ignore this tag altogether */
			मुक्त(buf);
			वापस;
		हाल REWRITE:
			अगर (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} अन्यथा अगर (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((काष्ठा commit *)tagged);
				अगर (!p) {
					म_लिखो("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					मुक्त(buf);
					वापस;
				}
				tagged_mark = get_object_mark(&p->object);
			} अन्यथा {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	अगर (tagged->type == OBJ_TAG) {
		म_लिखो("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	म_लिखो("tag %s\n", name);
	अगर (mark_tags) {
		mark_अग्र_object(&tag->object);
		म_लिखो("mark :%"PRIu32"\n", last_idnum);
	}
	अगर (tagged_mark)
		म_लिखो("from :%d\n", tagged_mark);
	अन्यथा
		म_लिखो("from %s\n", oid_to_hex(&tagged->oid));

	अगर (show_original_ids)
		म_लिखो("original-oid %s\n", oid_to_hex(&tag->object.oid));
	म_लिखो("%.*s%sdata %d\n%.*s\n",
	       (पूर्णांक)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (पूर्णांक)message_आकार, (पूर्णांक)message_आकार, message ? message : "");
	मुक्त(buf);
}

अटल काष्ठा commit *get_commit(काष्ठा rev_cmdline_entry *e, स्थिर अक्षर *full_name)
{
	चयन (e->item->type) {
	हाल OBJ_COMMIT:
		वापस (काष्ठा commit *)e->item;
	हाल OBJ_TAG: {
		काष्ठा tag *tag = (काष्ठा tag *)e->item;

		/* handle nested tags */
		जबतक (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (काष्ठा tag *)tag->tagged;
		}
		अगर (!tag)
			die(_("tag %s points nowhere?"), e->name);
		वापस (काष्ठा commit *)tag;
	}
	शेष:
		वापस शून्य;
	}
}

अटल व्योम get_tags_and_duplicates(काष्ठा rev_cmdline_info *info)
{
	पूर्णांक i;

	क्रम (i = 0; i < info->nr; i++) {
		काष्ठा rev_cmdline_entry *e = info->rev + i;
		काष्ठा object_id oid;
		काष्ठा commit *commit;
		अक्षर *full_name = शून्य;

		अगर (e->flags & UNINTERESTING)
			जारी;

		अगर (repo_dwim_ref(the_repository, e->name, म_माप(e->name),
				  &oid, &full_name, 0) != 1) {
			मुक्त(full_name);
			जारी;
		}

		अगर (refspecs.nr) {
			अक्षर *निजी;
			निजी = apply_refspecs(&refspecs, full_name);
			अगर (निजी) {
				मुक्त(full_name);
				full_name = निजी;
			}
		}

		commit = get_commit(e, full_name);
		अगर (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			मुक्त(full_name);
			जारी;
		}

		चयन (commit->object.type) {
		हाल OBJ_COMMIT:
			अवरोध;
		हाल OBJ_BLOB:
			export_blob(&commit->object.oid);
			मुक्त(full_name);
			जारी;
		शेष: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			मुक्त(full_name);
			जारी;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		अगर (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		अगर (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		अन्यथा
			मुक्त(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

अटल व्योम handle_tags_and_duplicates(काष्ठा string_list *extras)
{
	काष्ठा commit *commit;
	पूर्णांक i;

	क्रम (i = extras->nr - 1; i >= 0; i--) {
		स्थिर अक्षर *name = extras->वस्तुएँ[i].string;
		काष्ठा object *object = extras->वस्तुएँ[i].util;
		पूर्णांक mark;

		चयन (object->type) {
		हाल OBJ_TAG:
			handle_tag(name, (काष्ठा tag *)object);
			अवरोध;
		हाल OBJ_COMMIT:
			अगर (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((काष्ठा commit *)object);
			अगर (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				म_लिखो("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				जारी;
			}

			mark = get_object_mark(&commit->object);
			अगर (!mark) {
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
				अगर (!reference_excluded_commits) {
					/* delete the ref */
					म_लिखो("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					जारी;
				}
				/* set ref to commit using oid, not mark */
				म_लिखो("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				जारी;
			}

			म_लिखो("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			अवरोध;
		}
	}
}

अटल व्योम export_marks(अक्षर *file)
{
	अचिन्हित पूर्णांक i;
	uint32_t mark;
	काष्ठा decoration_entry *deco = idnums.entries;
	खाता *f;
	पूर्णांक e = 0;

	f = fopen_for_writing(file);
	अगर (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	क्रम (i = 0; i < idnums.आकार; i++) {
		अगर (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			अगर (ख_लिखो(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    अवरोध;
			}
		}
		deco++;
	}

	e |= ख_त्रुटि(f);
	e |= ख_बंद(f);
	अगर (e)
		error(_("unable to write marks file %s."), file);
}

अटल व्योम import_marks(अक्षर *निवेश_file, पूर्णांक check_exists)
{
	अक्षर पंक्ति[512];
	खाता *f;
	काष्ठा stat sb;

	अगर (check_exists && stat(निवेश_file, &sb))
		वापस;

	f = xfopen(निवेश_file, "r");
	जबतक (ख_माला_लो(पंक्ति, माप(पंक्ति), f)) {
		uint32_t mark;
		अक्षर *पंक्ति_end, *mark_end;
		काष्ठा object_id oid;
		काष्ठा object *object;
		काष्ठा commit *commit;
		क्रमागत object_type type;

		पंक्ति_end = म_अक्षर(पंक्ति, '\n');
		अगर (पंक्ति[0] != ':' || !पंक्ति_end)
			die(_("corrupt mark line: %s"), पंक्ति);
		*पंक्ति_end = '\0';

		mark = strtoumax(पंक्ति + 1, &mark_end, 10);
		अगर (!mark || mark_end == पंक्ति + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), पंक्ति);

		अगर (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, शून्य);
		अगर (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		अगर (type != OBJ_COMMIT)
			/* only commits */
			जारी;

		commit = lookup_commit(the_repository, &oid);
		अगर (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		अगर (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	ख_बंद(f);
}

अटल व्योम handle_deletes(व्योम)
{
	पूर्णांक i;
	क्रम (i = 0; i < refspecs.nr; i++) {
		काष्ठा refspec_item *refspec = &refspecs.वस्तुएँ[i];
		अगर (*refspec->src)
			जारी;

		म_लिखो("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

अटल पूर्णांक parse_opt_anonymize_map(स्थिर काष्ठा option *opt,
				   स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा hashmap *map = opt->मूल्य;
	स्थिर अक्षर *delim, *मूल्य;
	माप_प्रकार keylen;

	BUG_ON_OPT_NEG(unset);

	delim = म_अक्षर(arg, ':');
	अगर (delim) {
		keylen = delim - arg;
		मूल्य = delim + 1;
	} अन्यथा {
		keylen = म_माप(arg);
		मूल्य = arg;
	}

	अगर (!keylen || !*मूल्य)
		वापस error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(मूल्य));

	वापस 0;
}

पूर्णांक cmd_fast_export(पूर्णांक argc,
		    स्थिर अक्षर **argv,
		    स्थिर अक्षर *prefix,
		    काष्ठा repository *repo UNUSED)
{
	काष्ठा rev_info revs;
	काष्ठा commit *commit;
	अक्षर *export_filename = शून्य,
	     *import_filename = शून्य,
	     *import_filename_if_exists = शून्य;
	uint32_t lastimportid;
	काष्ठा string_list refspecs_list = STRING_LIST_INIT_NODUP;
	काष्ठा string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	काष्ठा option options[] = {
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
		OPT_BOOL(0, "no-data", &no_तथ्य, N_("skip output of blob data")),
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

	अगर (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, शून्य);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, prefix);
	init_revision_sources(&revision_sources);
	revs.topo_order = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, prefix, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = setup_revisions(argc, argv, &revs, शून्य);
	अगर (argc > 1)
		usage_with_options (fast_export_usage, options);

	अगर (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	अगर (refspecs_list.nr) {
		पूर्णांक i;

		क्रम (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.वस्तुएँ[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	अगर (use_done_feature)
		म_लिखो("feature done\n");

	अगर (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	अगर (import_filename)
		import_marks(import_filename, 0);
	अन्यथा अगर (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	अगर (import_filename && revs.prune_तथ्य.nr)
		full_वृक्ष = 1;

	get_tags_and_duplicates(&revs.cmdline);

	अगर (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_तथ्य = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	जबतक ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	अगर (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	अगर (use_done_feature)
		म_लिखो("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	वापस 0;
}
