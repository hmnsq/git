/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "refspec.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "object.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "diffcore.h"
#അന്തര്ഭാവ "log-tree.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "decorate.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "utf8.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "blob.h"
#അന്തര്ഭാവ "gpg-interface.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ progress;
സ്ഥിതം എണ്ണൽ sign_mode signed_tag_mode = SIGN_ABORT;
സ്ഥിതം എണ്ണൽ sign_mode signed_commit_mode = SIGN_STRIP;
സ്ഥിതം എണ്ണൽ tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
സ്ഥിതം എണ്ണൽ reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
സ്ഥിതം പൂർണ്ണസംഖ്യ fake_missing_tagger;
സ്ഥിതം പൂർണ്ണസംഖ്യ use_done_feature;
സ്ഥിതം പൂർണ്ണസംഖ്യ no_data;
സ്ഥിതം പൂർണ്ണസംഖ്യ full_വൃക്ഷം;
സ്ഥിതം പൂർണ്ണസംഖ്യ reference_excluded_commits;
സ്ഥിതം പൂർണ്ണസംഖ്യ show_original_ids;
സ്ഥിതം പൂർണ്ണസംഖ്യ mark_tags;
സ്ഥിതം ഘടന string_list extra_refs = STRING_LIST_INIT_DUP;
സ്ഥിതം ഘടന string_list tag_refs = STRING_LIST_INIT_DUP;
സ്ഥിതം ഘടന refspec refspecs;
സ്ഥിതം പൂർണ്ണസംഖ്യ anonymize;
സ്ഥിതം ഘടന hashmap anonymized_seeds;
സ്ഥിതം ഘടന revision_sources revision_sources;

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_sign_mode(സ്ഥിരം ഘടന option *opt,
			       സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	എണ്ണൽ sign_mode *val = opt->മൂല്യം;

	എങ്കിൽ (unset)
		തിരികെ 0;

	എങ്കിൽ (parse_sign_mode(arg, val, ശൂന്യ) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		തിരികെ error(_("unknown %s mode: %s"), opt->long_name, arg);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_tag_of_filtered_mode(സ്ഥിരം ഘടന option *opt,
					  സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	എണ്ണൽ tag_of_filtered_mode *val = opt->മൂല്യം;

	എങ്കിൽ (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "drop"))
		*val = DROP;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	അല്ലെങ്കിൽ
		തിരികെ error(_("unknown tag-of-filtered mode: %s"), arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_reencode_mode(സ്ഥിരം ഘടന option *opt,
				   സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	എണ്ണൽ reencode_mode *val = opt->മൂല്യം;

	എങ്കിൽ (unset) {
		*val = REENCODE_ABORT;
		തിരികെ 0;
	}

	തിരഞ്ഞെടുപ്പ് (git_parse_maybe_bool(arg)) {
	അവസ്ഥ 0:
		*val = REENCODE_NO;
		നിർത്തുക;
	അവസ്ഥ 1:
		*val = REENCODE_YES;
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		എങ്കിൽ (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		അല്ലെങ്കിൽ
			തിരികെ error(_("unknown reencoding mode: %s"), arg);
	}

	തിരികെ 0;
}

സ്ഥിതം ഘടന decoration idnums;
സ്ഥിതം uint32_t last_idnum;
ഘടന anonymized_entry {
	ഘടന hashmap_entry hash;
	അക്ഷരം *anon;
	സ്ഥിരം അക്ഷരം orig[FLEX_ARRAY];
};

ഘടന anonymized_entry_താക്കോൽ {
	ഘടന hashmap_entry hash;
	സ്ഥിരം അക്ഷരം *orig;
	size_t orig_len;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ anonymized_entry_cmp(സ്ഥിരം ശൂന്യം *cmp_data UNUSED,
				സ്ഥിരം ഘടന hashmap_entry *eptr,
				സ്ഥിരം ഘടന hashmap_entry *entry_or_താക്കോൽ,
				സ്ഥിരം ശൂന്യം *keydata)
{
	സ്ഥിരം ഘടന anonymized_entry *a, *b;

	a = container_of(eptr, സ്ഥിരം ഘടന anonymized_entry, hash);
	എങ്കിൽ (keydata) {
		സ്ഥിരം ഘടന anonymized_entry_താക്കോൽ *താക്കോൽ = keydata;
		പൂർണ്ണസംഖ്യ equal = !xstrncmpz(a->orig, താക്കോൽ->orig, താക്കോൽ->orig_len);
		തിരികെ !equal;
	}

	b = container_of(entry_or_താക്കോൽ, സ്ഥിരം ഘടന anonymized_entry, hash);
	തിരികെ strcmp(a->orig, b->orig);
}

സ്ഥിതം ഘടന anonymized_entry *add_anonymized_entry(ഘടന hashmap *map,
						     unsigned hash,
						     സ്ഥിരം അക്ഷരം *orig, size_t len,
						     അക്ഷരം *anon)
{
	ഘടന anonymized_entry *ret, *old;

	എങ്കിൽ (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, ശൂന്യ, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	എങ്കിൽ (old) {
		സ്വതന്ത്രം(old->anon);
		സ്വതന്ത്രം(old);
	}

	തിരികെ ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
സ്ഥിതം സ്ഥിരം അക്ഷരം *anonymize_str(ഘടന hashmap *map,
				 അക്ഷരം *(*generate)(ശൂന്യം),
				 സ്ഥിരം അക്ഷരം *orig, size_t len)
{
	ഘടന anonymized_entry_താക്കോൽ താക്കോൽ;
	ഘടന anonymized_entry *ret;

	hashmap_entry_init(&താക്കോൽ.hash, memhash(orig, len));
	താക്കോൽ.orig = orig;
	താക്കോൽ.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &താക്കോൽ, hash, &താക്കോൽ);

	/* ...otherwise check if we've already seen it in this context... */
	എങ്കിൽ (!ret)
		ret = hashmap_get_entry(map, &താക്കോൽ, hash, &താക്കോൽ);

	/* ...and finally generate a new mapping if necessary */
	എങ്കിൽ (!ret)
		ret = add_anonymized_entry(map, താക്കോൽ.hash.hash,
					   orig, len, generate());

	തിരികെ ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
സ്ഥിതം ശൂന്യം anonymize_path(ഘടന strbuf *out, സ്ഥിരം അക്ഷരം *path,
			   ഘടന hashmap *map,
			   അക്ഷരം *(*generate)(ശൂന്യം))
{
	ആകുമ്പോൾ (*path) {
		സ്ഥിരം അക്ഷരം *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		സ്ഥിരം അക്ഷരം *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		എങ്കിൽ (*path)
			strbuf_addch(out, *path++);
	}
}

സ്ഥിതം വരിയിൽ ശൂന്യം *mark_to_ptr(uint32_t mark)
{
	തിരികെ (ശൂന്യം *)(uintptr_t)mark;
}

സ്ഥിതം വരിയിൽ uint32_t ptr_to_mark(ശൂന്യം * mark)
{
	തിരികെ (uint32_t)(uintptr_t)mark;
}

സ്ഥിതം വരിയിൽ ശൂന്യം mark_object(ഘടന object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

സ്ഥിതം വരിയിൽ ശൂന്യം mark_next_object(ഘടന object *object)
{
	mark_object(object, ++last_idnum);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_object_mark(ഘടന object *object)
{
	ശൂന്യം *decoration = lookup_decoration(&idnums, object);
	എങ്കിൽ (!decoration)
		തിരികെ 0;
	തിരികെ ptr_to_mark(decoration);
}

സ്ഥിതം ഘടന commit *rewrite_commit(ഘടന commit *p)
{
	വേണ്ടി (;;) {
		എങ്കിൽ (p->parents && p->parents->next)
			നിർത്തുക;
		എങ്കിൽ (p->object.flags & UNINTERESTING)
			നിർത്തുക;
		എങ്കിൽ (!(p->object.flags & TREESAME))
			നിർത്തുക;
		എങ്കിൽ (!p->parents)
			തിരികെ ശൂന്യ;
		p = p->parents->item;
	}
	തിരികെ p;
}

സ്ഥിതം ശൂന്യം show_progress(ശൂന്യം)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ counter = 0;
	എങ്കിൽ (!progress)
		തിരികെ;
	എങ്കിൽ ((++counter % progress) == 0)
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
സ്ഥിതം അക്ഷരം *anonymize_blob(unsigned long *size)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ counter;
	ഘടന strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	തിരികെ strbuf_detach(&out, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം export_blob(സ്ഥിരം ഘടന object_id *oid)
{
	unsigned long size;
	എണ്ണൽ object_type type;
	അക്ഷരം *buf;
	ഘടന object *object;
	പൂർണ്ണസംഖ്യ eaten;

	എങ്കിൽ (no_data)
		തിരികെ;

	എങ്കിൽ (is_null_oid(oid))
		തിരികെ;

	object = lookup_object(the_repository, oid);
	എങ്കിൽ (object && object->flags & SHOWN)
		തിരികെ;

	എങ്കിൽ (anonymize) {
		buf = anonymize_blob(&size);
		object = (ഘടന object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} അല്ലെങ്കിൽ {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		എങ്കിൽ (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		എങ്കിൽ (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	എങ്കിൽ (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	എങ്കിൽ (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	എങ്കിൽ (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	എങ്കിൽ (!eaten)
		സ്വതന്ത്രം(buf);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ depth_first(സ്ഥിരം ശൂന്യം *a_, സ്ഥിരം ശൂന്യം *b_)
{
	സ്ഥിരം ഘടന diff_filepair *a = *((സ്ഥിരം ഘടന diff_filepair **)a_);
	സ്ഥിരം ഘടന diff_filepair *b = *((സ്ഥിരം ഘടന diff_filepair **)b_);
	സ്ഥിരം അക്ഷരം *name_a, *name_b;
	പൂർണ്ണസംഖ്യ len_a, len_b, len;
	പൂർണ്ണസംഖ്യ cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	എങ്കിൽ (cmp)
		തിരികെ cmp;
	cmp = len_b - len_a;
	എങ്കിൽ (cmp)
		തിരികെ cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	തിരികെ (a->status == 'R') - (b->status == 'R');
}

സ്ഥിതം ശൂന്യം print_path_1(സ്ഥിരം അക്ഷരം *path)
{
	പൂർണ്ണസംഖ്യ need_quote = quote_c_style(path, ശൂന്യ, ശൂന്യ, 0);
	എങ്കിൽ (need_quote)
		quote_c_style(path, ശൂന്യ, stdout, 0);
	അല്ലെങ്കിൽ എങ്കിൽ (strchr(path, ' '))
		printf("\"%s\"", path);
	അല്ലെങ്കിൽ
		printf("%s", path);
}

സ്ഥിതം അക്ഷരം *anonymize_path_component(ശൂന്യം)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ counter;
	ഘടന strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	തിരികെ strbuf_detach(&out, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം print_path(സ്ഥിരം അക്ഷരം *path)
{
	എങ്കിൽ (!anonymize)
		print_path_1(path);
	അല്ലെങ്കിൽ {
		സ്ഥിതം ഘടന hashmap paths;
		സ്ഥിതം ഘടന strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

സ്ഥിതം അക്ഷരം *generate_fake_oid(ശൂന്യം)
{
	സ്ഥിതം uint32_t counter = 1; /* avoid null oid */
	സ്ഥിരം unsigned hashsz = the_hash_algo->rawsz;
	ഘടന object_id oid;
	അക്ഷരം *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	തിരികെ oid_to_hex_r(hex, &oid);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *anonymize_oid(സ്ഥിരം അക്ഷരം *oid_hex)
{
	സ്ഥിതം ഘടന hashmap objs;
	size_t len = strlen(oid_hex);
	തിരികെ anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

സ്ഥിതം ശൂന്യം show_filemodify(ഘടന diff_queue_struct *q,
			    ഘടന diff_options *options UNUSED, ശൂന്യം *data)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	വേണ്ടി (i = 0; i < q->nr; i++) {
		ഘടന diff_filespec *ospec = q->queue[i]->one;
		ഘടന diff_filespec *spec = q->queue[i]->two;

		തിരഞ്ഞെടുപ്പ് (q->queue[i]->status) {
		അവസ്ഥ DIFF_STATUS_DELETED:
			printf("D ");
			print_path(spec->path);
			string_list_തിരുകുക(changed, spec->path);
			putchar('\n');
			നിർത്തുക;

		അവസ്ഥ DIFF_STATUS_COPIED:
		അവസ്ഥ DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			എങ്കിൽ (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				print_path(ospec->path);
				putchar(' ');
				print_path(spec->path);
				string_list_തിരുകുക(changed, spec->path);
				putchar('\n');

				എങ്കിൽ (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					നിർത്തുക;
			}
			/* fallthrough */

		അവസ്ഥ DIFF_STATUS_TYPE_CHANGED:
		അവസ്ഥ DIFF_STATUS_MODIFIED:
		അവസ്ഥ DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			എങ്കിൽ (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			അല്ലെങ്കിൽ {
				ഘടന object *object = lookup_object(the_repository,
								      &spec->oid);
				printf("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_തിരുകുക(changed, spec->path);
			putchar('\n');
			നിർത്തുക;

		സ്ഥിരസ്ഥിതി:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

സ്ഥിതം അക്ഷരം *anonymize_ref_component(ശൂന്യം)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ counter;
	ഘടന strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	തിരികെ strbuf_detach(&out, ശൂന്യ);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *anonymize_refname(സ്ഥിരം അക്ഷരം *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	സ്ഥിതം സ്ഥിരം അക്ഷരം *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	സ്ഥിതം ഘടന hashmap refs;
	സ്ഥിതം ഘടന strbuf anon = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ i;

	strbuf_reset(&anon);
	വേണ്ടി (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		എങ്കിൽ (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			നിർത്തുക;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	തിരികെ anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
സ്ഥിതം അക്ഷരം *anonymize_commit_message(ശൂന്യം)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ counter;
	തിരികെ xstrfmt("subject %d\n\nbody\n", counter++);
}

സ്ഥിതം അക്ഷരം *anonymize_ident(ശൂന്യം)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ counter;
	ഘടന strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	തിരികെ strbuf_detach(&out, ശൂന്യ);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
സ്ഥിതം ശൂന്യം anonymize_ident_വരി(സ്ഥിരം അക്ഷരം **beg, സ്ഥിരം അക്ഷരം **end)
{
	സ്ഥിതം ഘടന hashmap idents;
	സ്ഥിതം ഘടന strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	സ്ഥിതം unsigned which_buffer;

	ഘടന strbuf *out;
	ഘടന ident_split split;
	സ്ഥിരം അക്ഷരം *end_of_header;

	out = &buffers[which_buffer++];
	which_buffer %= ARRAY_SIZE(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = strchr(*beg, ' ');
	എങ്കിൽ (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (പൂർണ്ണസംഖ്യ)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	എങ്കിൽ (!split_ident_വരി(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		സ്ഥിരം അക്ഷരം *ident;
		size_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} അല്ലെങ്കിൽ {
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
സ്ഥിതം സ്ഥിരം അക്ഷരം *find_commit_multiline_header(സ്ഥിരം അക്ഷരം *സന്ദേശം,
						സ്ഥിരം അക്ഷരം *താക്കോൽ,
						സ്ഥിരം അക്ഷരം **end)
{
	ഘടന strbuf val = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *bol, *eol;
	size_t len;

	bol = find_commit_header(സന്ദേശം, താക്കോൽ, &len);
	എങ്കിൽ (!bol)
		തിരികെ ശൂന്യ;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	ആകുമ്പോൾ (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	തിരികെ strbuf_detach(&val, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം print_signature(സ്ഥിരം അക്ഷരം *signature, സ്ഥിരം അക്ഷരം *object_hash)
{
	എങ്കിൽ (!signature)
		തിരികെ;

	printf("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (unsigned)strlen(signature),
	       signature);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *append_signatures_for_header(ഘടന string_list *signatures,
						സ്ഥിരം അക്ഷരം *pos,
						സ്ഥിരം അക്ഷരം *header,
						സ്ഥിരം അക്ഷരം *object_hash)
{
	സ്ഥിരം അക്ഷരം *signature;
	സ്ഥിരം അക്ഷരം *start = pos;
	സ്ഥിരം അക്ഷരം *end = pos;

	ആകുമ്പോൾ ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (ശൂന്യം *)object_hash;
		സ്വതന്ത്രം((അക്ഷരം *)signature);
		start = end;
	}

	തിരികെ end;
}

സ്ഥിതം ശൂന്യം handle_commit(ഘടന commit *commit, ഘടന rev_info *rev,
			  ഘടന string_list *paths_of_changed_objects)
{
	പൂർണ്ണസംഖ്യ saved_output_format = rev->diffopt.output_format;
	സ്ഥിരം അക്ഷരം *commit_buffer, *commit_buffer_cursor;
	സ്ഥിരം അക്ഷരം *author, *author_end, *committer, *committer_end;
	സ്ഥിരം അക്ഷരം *encoding = ശൂന്യ;
	size_t encoding_len;
	ഘടന string_list signatures = STRING_LIST_INIT_DUP;
	സ്ഥിരം അക്ഷരം *message;
	അക്ഷരം *reencoded = ശൂന്യ;
	ഘടന commit_list *p;
	സ്ഥിരം അക്ഷരം *refname;
	പൂർണ്ണസംഖ്യ i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, ശൂന്യ);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	എങ്കിൽ (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	എങ്കിൽ (!committer)
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

	എങ്കിൽ (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		എങ്കിൽ (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	എങ്കിൽ (*commit_buffer_cursor == '\n') {
		സ്ഥിരം അക്ഷരം *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		സ്ഥിരം അക്ഷരം *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
	എങ്കിൽ (message)
		message += 2;

	എങ്കിൽ (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_വൃക്ഷം) {
		parse_commit_or_die(commit->parents->item);
		diff_വൃക്ഷം_oid(get_commit_വൃക്ഷം_oid(commit->parents->item),
			      get_commit_വൃക്ഷം_oid(commit), "", &rev->diffopt);
	}
	അല്ലെങ്കിൽ
		diff_വേര്_വൃക്ഷം_oid(get_commit_വൃക്ഷം_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	വേണ്ടി (i = 0; i < diff_queued_diff.nr; i++)
		എങ്കിൽ (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	എങ്കിൽ (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_വരി(&committer, &committer_end);
		anonymize_ident_വരി(&author, &author_end);
	}

	mark_next_object(&commit->object);
	എങ്കിൽ (anonymize) {
		reencoded = anonymize_commit_message();
	} അല്ലെങ്കിൽ എങ്കിൽ (encoding) {
		അക്ഷരം *buf;
		തിരഞ്ഞെടുപ്പ് (reencode_mode) {
		അവസ്ഥ REENCODE_YES:
			buf = xstrfmt("%.*s", (പൂർണ്ണസംഖ്യ)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			സ്വതന്ത്രം(buf);
			നിർത്തുക;
		അവസ്ഥ REENCODE_NO:
			നിർത്തുക;
		അവസ്ഥ REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (പൂർണ്ണസംഖ്യ)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	എങ്കിൽ (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	എങ്കിൽ (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (പൂർണ്ണസംഖ്യ)(author_end - author), author,
	       (പൂർണ്ണസംഖ്യ)(committer_end - committer), committer);
	എങ്കിൽ (signatures.nr) {
		തിരഞ്ഞെടുപ്പ് (signed_commit_mode) {
		/* Exporting modes */
		അവസ്ഥ SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		അവസ്ഥ SIGN_VERBATIM:
			വേണ്ടി (size_t i = 0; i < signatures.nr; i++) {
				ഘടന string_list_item *item = &signatures.ഇനങ്ങൾ[i];
				print_signature(item->string, item->util);
			}
			നിർത്തുക;

		/* Stripping modes */
		അവസ്ഥ SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		അവസ്ഥ SIGN_STRIP:
			നിർത്തുക;

		/* Aborting modes */
		അവസ്ഥ SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		സ്ഥിരസ്ഥിതി:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	എങ്കിൽ (!reencoded && encoding)
		printf("encoding %.*s\n", (പൂർണ്ണസംഖ്യ)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	സ്വതന്ത്രം(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	വേണ്ടി (i = 0, p = commit->parents; p; p = p->next) {
		ഘടന object *obj = &p->item->object;
		പൂർണ്ണസംഖ്യ mark = get_object_mark(obj);

		എങ്കിൽ (!mark && !reference_excluded_commits)
			തുടരുക;
		എങ്കിൽ (i == 0)
			printf("from ");
		അല്ലെങ്കിൽ
			printf("merge ");
		എങ്കിൽ (mark)
			printf(":%d\n", mark);
		അല്ലെങ്കിൽ
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	എങ്കിൽ (full_വൃക്ഷം)
		printf("deleteall\n");
	log_വൃക്ഷം_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

സ്ഥിതം അക്ഷരം *anonymize_tag(ശൂന്യം)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ counter;
	ഘടന strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	തിരികെ strbuf_detach(&out, ശൂന്യ);
}


സ്ഥിതം ശൂന്യം handle_tag(സ്ഥിരം അക്ഷരം *name, ഘടന tag *tag)
{
	size_t size;
	എണ്ണൽ object_type type;
	അക്ഷരം *buf;
	സ്ഥിരം അക്ഷരം *tagger, *tagger_end, *message;
	size_t message_size = 0;
	ഘടന object *tagged;
	പൂർണ്ണസംഖ്യ tagged_mark;
	ഘടന commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	ആകുമ്പോൾ (tagged->type == OBJ_TAG) {
		tagged = ((ഘടന tag *)tagged)->tagged;
	}
	എങ്കിൽ (tagged->type == OBJ_വൃക്ഷം) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		തിരികെ;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	എങ്കിൽ (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	എങ്കിൽ (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	എങ്കിൽ (!tagger) {
		എങ്കിൽ (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		അല്ലെങ്കിൽ
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} അല്ലെങ്കിൽ {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		എങ്കിൽ (anonymize)
			anonymize_ident_വരി(&tagger, &tagger_end);
	}

	എങ്കിൽ (anonymize) {
		name = anonymize_refname(name);
		എങ്കിൽ (message) {
			സ്ഥിതം ഘടന hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	എങ്കിൽ (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		എങ്കിൽ (sig_offset < message_size)
			തിരഞ്ഞെടുപ്പ് (signed_tag_mode) {
			/* Exporting modes */
			അവസ്ഥ SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			അവസ്ഥ SIGN_VERBATIM:
				നിർത്തുക;

			/* Stripping modes */
			അവസ്ഥ SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			അവസ്ഥ SIGN_STRIP:
				message_size = sig_offset;
				നിർത്തുക;

			/* Aborting modes */
			അവസ്ഥ SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			സ്ഥിരസ്ഥിതി:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	എങ്കിൽ (!tagged_mark) {
		തിരഞ്ഞെടുപ്പ് (tag_of_filtered_mode) {
		അവസ്ഥ TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		അവസ്ഥ DROP:
			/* Ignore this tag altogether */
			സ്വതന്ത്രം(buf);
			തിരികെ;
		അവസ്ഥ REWRITE:
			എങ്കിൽ (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} അല്ലെങ്കിൽ എങ്കിൽ (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((ഘടന commit *)tagged);
				എങ്കിൽ (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					സ്വതന്ത്രം(buf);
					തിരികെ;
				}
				tagged_mark = get_object_mark(&p->object);
			} അല്ലെങ്കിൽ {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	എങ്കിൽ (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	എങ്കിൽ (mark_tags) {
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	എങ്കിൽ (tagged_mark)
		printf("from :%d\n", tagged_mark);
	അല്ലെങ്കിൽ
		printf("from %s\n", oid_to_hex(&tagged->oid));

	എങ്കിൽ (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (പൂർണ്ണസംഖ്യ)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (പൂർണ്ണസംഖ്യ)message_size, (പൂർണ്ണസംഖ്യ)message_size, message ? message : "");
	സ്വതന്ത്രം(buf);
}

സ്ഥിതം ഘടന commit *get_commit(ഘടന rev_cmdline_entry *e, സ്ഥിരം അക്ഷരം *full_name)
{
	തിരഞ്ഞെടുപ്പ് (e->item->type) {
	അവസ്ഥ OBJ_COMMIT:
		തിരികെ (ഘടന commit *)e->item;
	അവസ്ഥ OBJ_TAG: {
		ഘടന tag *tag = (ഘടന tag *)e->item;

		/* handle nested tags */
		ആകുമ്പോൾ (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (ഘടന tag *)tag->tagged;
		}
		എങ്കിൽ (!tag)
			die(_("tag %s points nowhere?"), e->name);
		തിരികെ (ഘടന commit *)tag;
	}
	സ്ഥിരസ്ഥിതി:
		തിരികെ ശൂന്യ;
	}
}

സ്ഥിതം ശൂന്യം get_tags_and_duplicates(ഘടന rev_cmdline_info *info)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < info->nr; i++) {
		ഘടന rev_cmdline_entry *e = info->rev + i;
		ഘടന object_id oid;
		ഘടന commit *commit;
		അക്ഷരം *full_name = ശൂന്യ;

		എങ്കിൽ (e->flags & UNINTERESTING)
			തുടരുക;

		എങ്കിൽ (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			സ്വതന്ത്രം(full_name);
			തുടരുക;
		}

		എങ്കിൽ (refspecs.nr) {
			അക്ഷരം *സ്വകാര്യ;
			സ്വകാര്യ = apply_refspecs(&refspecs, full_name);
			എങ്കിൽ (സ്വകാര്യ) {
				സ്വതന്ത്രം(full_name);
				full_name = സ്വകാര്യ;
			}
		}

		commit = get_commit(e, full_name);
		എങ്കിൽ (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			സ്വതന്ത്രം(full_name);
			തുടരുക;
		}

		തിരഞ്ഞെടുപ്പ് (commit->object.type) {
		അവസ്ഥ OBJ_COMMIT:
			നിർത്തുക;
		അവസ്ഥ OBJ_BLOB:
			export_blob(&commit->object.oid);
			സ്വതന്ത്രം(full_name);
			തുടരുക;
		സ്ഥിരസ്ഥിതി: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			സ്വതന്ത്രം(full_name);
			തുടരുക;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		എങ്കിൽ (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		എങ്കിൽ (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		അല്ലെങ്കിൽ
			സ്വതന്ത്രം(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

സ്ഥിതം ശൂന്യം handle_tags_and_duplicates(ഘടന string_list *extras)
{
	ഘടന commit *commit;
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = extras->nr - 1; i >= 0; i--) {
		സ്ഥിരം അക്ഷരം *name = extras->ഇനങ്ങൾ[i].string;
		ഘടന object *object = extras->ഇനങ്ങൾ[i].util;
		പൂർണ്ണസംഖ്യ mark;

		തിരഞ്ഞെടുപ്പ് (object->type) {
		അവസ്ഥ OBJ_TAG:
			handle_tag(name, (ഘടന tag *)object);
			നിർത്തുക;
		അവസ്ഥ OBJ_COMMIT:
			എങ്കിൽ (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((ഘടന commit *)object);
			എങ്കിൽ (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				printf("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				തുടരുക;
			}

			mark = get_object_mark(&commit->object);
			എങ്കിൽ (!mark) {
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
				എങ്കിൽ (!reference_excluded_commits) {
					/* delete the ref */
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					തുടരുക;
				}
				/* set ref to commit using oid, not mark */
				printf("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				തുടരുക;
			}

			printf("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			നിർത്തുക;
		}
	}
}

സ്ഥിതം ശൂന്യം export_marks(അക്ഷരം *file)
{
	unsigned പൂർണ്ണസംഖ്യ i;
	uint32_t mark;
	ഘടന decoration_entry *deco = idnums.entries;
	FILE *f;
	പൂർണ്ണസംഖ്യ e = 0;

	f = fopen_for_writing(file);
	എങ്കിൽ (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	വേണ്ടി (i = 0; i < idnums.size; i++) {
		എങ്കിൽ (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			എങ്കിൽ (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    നിർത്തുക;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	എങ്കിൽ (e)
		error(_("unable to write marks file %s."), file);
}

സ്ഥിതം ശൂന്യം import_marks(അക്ഷരം *നിവേശം_file, പൂർണ്ണസംഖ്യ check_exists)
{
	അക്ഷരം വരി[512];
	FILE *f;
	ഘടന stat sb;

	എങ്കിൽ (check_exists && stat(നിവേശം_file, &sb))
		തിരികെ;

	f = xfopen(നിവേശം_file, "r");
	ആകുമ്പോൾ (fgets(വരി, വലുപ്പം(വരി), f)) {
		uint32_t mark;
		അക്ഷരം *വരി_end, *mark_end;
		ഘടന object_id oid;
		ഘടന object *object;
		ഘടന commit *commit;
		എണ്ണൽ object_type type;

		വരി_end = strchr(വരി, '\n');
		എങ്കിൽ (വരി[0] != ':' || !വരി_end)
			die(_("corrupt mark line: %s"), വരി);
		*വരി_end = '\0';

		mark = strtoumax(വരി + 1, &mark_end, 10);
		എങ്കിൽ (!mark || mark_end == വരി + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), വരി);

		എങ്കിൽ (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, ശൂന്യ);
		എങ്കിൽ (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		എങ്കിൽ (type != OBJ_COMMIT)
			/* only commits */
			തുടരുക;

		commit = lookup_commit(the_repository, &oid);
		എങ്കിൽ (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		എങ്കിൽ (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

സ്ഥിതം ശൂന്യം handle_deletes(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ i;
	വേണ്ടി (i = 0; i < refspecs.nr; i++) {
		ഘടന refspec_item *refspec = &refspecs.ഇനങ്ങൾ[i];
		എങ്കിൽ (*refspec->src)
			തുടരുക;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_anonymize_map(സ്ഥിരം ഘടന option *opt,
				   സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന hashmap *map = opt->മൂല്യം;
	സ്ഥിരം അക്ഷരം *delim, *മൂല്യം;
	size_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	എങ്കിൽ (delim) {
		keylen = delim - arg;
		മൂല്യം = delim + 1;
	} അല്ലെങ്കിൽ {
		keylen = strlen(arg);
		മൂല്യം = arg;
	}

	എങ്കിൽ (!keylen || !*മൂല്യം)
		തിരികെ error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(മൂല്യം));

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_fast_export(പൂർണ്ണസംഖ്യ argc,
		    സ്ഥിരം അക്ഷരം **argv,
		    സ്ഥിരം അക്ഷരം *prefix,
		    ഘടന repository *repo UNUSED)
{
	ഘടന rev_info revs;
	ഘടന commit *commit;
	അക്ഷരം *export_filename = ശൂന്യ,
	     *import_filename = ശൂന്യ,
	     *import_filename_if_exists = ശൂന്യ;
	uint32_t lastimportid;
	ഘടന string_list refspecs_list = STRING_LIST_INIT_NODUP;
	ഘടന string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	ഘടന option options[] = {
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
		OPT_BOOL(0, "full-tree", &full_വൃക്ഷം,
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

	എങ്കിൽ (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, ശൂന്യ);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, prefix);
	init_revision_sources(&revision_sources);
	revs.topo_order = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, prefix, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = setup_revisions(argc, argv, &revs, ശൂന്യ);
	എങ്കിൽ (argc > 1)
		usage_with_options (fast_export_usage, options);

	എങ്കിൽ (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	എങ്കിൽ (refspecs_list.nr) {
		പൂർണ്ണസംഖ്യ i;

		വേണ്ടി (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.ഇനങ്ങൾ[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	എങ്കിൽ (use_done_feature)
		printf("feature done\n");

	എങ്കിൽ (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	എങ്കിൽ (import_filename)
		import_marks(import_filename, 0);
	അല്ലെങ്കിൽ എങ്കിൽ (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	എങ്കിൽ (import_filename && revs.prune_data.nr)
		full_വൃക്ഷം = 1;

	get_tags_and_duplicates(&revs.cmdline);

	എങ്കിൽ (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	ആകുമ്പോൾ ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	എങ്കിൽ (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	എങ്കിൽ (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	തിരികെ 0;
}
