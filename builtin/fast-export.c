/*
 * "git fast-export" builtin command
 *
 * Copyright (C) 2007 Johannes E. Schindelin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "refspec.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "object.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "diffcore.h"
#અન્તર્ભાવ "log-tree.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "decorate.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "utf8.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "blob.h"
#અન્તર્ભાવ "gpg-interface.h"

સ્થિર અચલ અક્ષર *અચલ fast_export_usage[] = {
	N_("git fast-export [<options>] [<revision-range>] [[--] <path>...]"),
	ખાલી
};

સ્થિર પૂર્ણાંક progress;
સ્થિર ગણના sign_mode signed_tag_mode = SIGN_ABORT;
સ્થિર ગણના sign_mode signed_commit_mode = SIGN_STRIP;
સ્થિર ગણના tag_of_filtered_mode { TAG_FILTERING_ABORT, DROP, REWRITE } tag_of_filtered_mode = TAG_FILTERING_ABORT;
સ્થિર ગણના reencode_mode { REENCODE_ABORT, REENCODE_YES, REENCODE_NO } reencode_mode = REENCODE_ABORT;
સ્થિર પૂર્ણાંક fake_missing_tagger;
સ્થિર પૂર્ણાંક use_done_feature;
સ્થિર પૂર્ણાંક no_data;
સ્થિર પૂર્ણાંક full_વૃક્ષ;
સ્થિર પૂર્ણાંક reference_excluded_commits;
સ્થિર પૂર્ણાંક show_original_ids;
સ્થિર પૂર્ણાંક mark_tags;
સ્થિર સંરચના string_list extra_refs = STRING_LIST_INIT_DUP;
સ્થિર સંરચના string_list tag_refs = STRING_LIST_INIT_DUP;
સ્થિર સંરચના refspec refspecs;
સ્થિર પૂર્ણાંક anonymize;
સ્થિર સંરચના hashmap anonymized_seeds;
સ્થિર સંરચના revision_sources revision_sources;

સ્થિર પૂર્ણાંક parse_opt_sign_mode(અચલ સંરચના option *opt,
			       અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	ગણના sign_mode *val = opt->કિંમત;

	જો (unset)
		પાછા 0;

	જો (parse_sign_mode(arg, val, ખાલી) || (*val == SIGN_STRIP_IF_INVALID) ||
	    (*val == SIGN_SIGN_IF_INVALID) || (*val == SIGN_ABORT_IF_INVALID))
		પાછા error(_("unknown %s mode: %s"), opt->long_name, arg);

	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_opt_tag_of_filtered_mode(અચલ સંરચના option *opt,
					  અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	ગણના tag_of_filtered_mode *val = opt->કિંમત;

	જો (unset || !strcmp(arg, "abort"))
		*val = TAG_FILTERING_ABORT;
	નહીંતર જો (!strcmp(arg, "drop"))
		*val = DROP;
	નહીંતર જો (!strcmp(arg, "rewrite"))
		*val = REWRITE;
	નહીંતર
		પાછા error(_("unknown tag-of-filtered mode: %s"), arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_opt_reencode_mode(અચલ સંરચના option *opt,
				   અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	ગણના reencode_mode *val = opt->કિંમત;

	જો (unset) {
		*val = REENCODE_ABORT;
		પાછા 0;
	}

	પસંદગી (git_parse_maybe_bool(arg)) {
	સ્થિતિ 0:
		*val = REENCODE_NO;
		તોડો;
	સ્થિતિ 1:
		*val = REENCODE_YES;
		તોડો;
	મૂળભૂત:
		જો (!strcasecmp(arg, "abort"))
			*val = REENCODE_ABORT;
		નહીંતર
			પાછા error(_("unknown reencoding mode: %s"), arg);
	}

	પાછા 0;
}

સ્થિર સંરચના decoration idnums;
સ્થિર uint32_t last_idnum;
સંરચના anonymized_entry {
	સંરચના hashmap_entry hash;
	અક્ષર *anon;
	અચલ અક્ષર orig[FLEX_ARRAY];
};

સંરચના anonymized_entry_ચાવી {
	સંરચના hashmap_entry hash;
	અચલ અક્ષર *orig;
	size_t orig_len;
};

સ્થિર પૂર્ણાંક anonymized_entry_cmp(અચલ શૂન્ય *cmp_data UNUSED,
				અચલ સંરચના hashmap_entry *eptr,
				અચલ સંરચના hashmap_entry *entry_or_ચાવી,
				અચલ શૂન્ય *keydata)
{
	અચલ સંરચના anonymized_entry *a, *b;

	a = container_of(eptr, અચલ સંરચના anonymized_entry, hash);
	જો (keydata) {
		અચલ સંરચના anonymized_entry_ચાવી *ચાવી = keydata;
		પૂર્ણાંક equal = !xstrncmpz(a->orig, ચાવી->orig, ચાવી->orig_len);
		પાછા !equal;
	}

	b = container_of(entry_or_ચાવી, અચલ સંરચના anonymized_entry, hash);
	પાછા strcmp(a->orig, b->orig);
}

સ્થિર સંરચના anonymized_entry *add_anonymized_entry(સંરચના hashmap *map,
						     unsigned hash,
						     અચલ અક્ષર *orig, size_t len,
						     અક્ષર *anon)
{
	સંરચના anonymized_entry *ret, *old;

	જો (!map->cmpfn)
		hashmap_init(map, anonymized_entry_cmp, ખાલી, 0);

	FLEX_ALLOC_MEM(ret, orig, orig, len);
	hashmap_entry_init(&ret->hash, hash);
	ret->anon = anon;
	old = hashmap_put_entry(map, ret, hash);

	જો (old) {
		મુક્ત(old->anon);
		મુક્ત(old);
	}

	પાછા ret;
}

/*
 * Basically keep a cache of X->Y so that we can repeatedly replace
 * the same anonymized string with another. The actual generation
 * is farmed out to the generate function.
 */
સ્થિર અચલ અક્ષર *anonymize_str(સંરચના hashmap *map,
				 અક્ષર *(*generate)(શૂન્ય),
				 અચલ અક્ષર *orig, size_t len)
{
	સંરચના anonymized_entry_ચાવી ચાવી;
	સંરચના anonymized_entry *ret;

	hashmap_entry_init(&ચાવી.hash, memhash(orig, len));
	ચાવી.orig = orig;
	ચાવી.orig_len = len;

	/* First check if it's a token the user configured manually... */
	ret = hashmap_get_entry(&anonymized_seeds, &ચાવી, hash, &ચાવી);

	/* ...otherwise check if we've already seen it in this context... */
	જો (!ret)
		ret = hashmap_get_entry(map, &ચાવી, hash, &ચાવી);

	/* ...and finally generate a new mapping if necessary */
	જો (!ret)
		ret = add_anonymized_entry(map, ચાવી.hash.hash,
					   orig, len, generate());

	પાછા ret->anon;
}

/*
 * We anonymize each component of a path individually,
 * so that paths a/b and a/c will share a common root.
 * The paths are cached via anonymize_mem so that repeated
 * lookups for "a" will yield the same value.
 */
સ્થિર શૂન્ય anonymize_path(સંરચના strbuf *out, અચલ અક્ષર *path,
			   સંરચના hashmap *map,
			   અક્ષર *(*generate)(શૂન્ય))
{
	જ્યારે (*path) {
		અચલ અક્ષર *end_of_component = strchrnul(path, '/');
		size_t len = end_of_component - path;
		અચલ અક્ષર *c = anonymize_str(map, generate, path, len);
		strbuf_addstr(out, c);
		path = end_of_component;
		જો (*path)
			strbuf_addch(out, *path++);
	}
}

સ્થિર અંતઃરેખ શૂન્ય *mark_to_ptr(uint32_t mark)
{
	પાછા (શૂન્ય *)(uintptr_t)mark;
}

સ્થિર અંતઃરેખ uint32_t ptr_to_mark(શૂન્ય * mark)
{
	પાછા (uint32_t)(uintptr_t)mark;
}

સ્થિર અંતઃરેખ શૂન્ય mark_object(સંરચના object *object, uint32_t mark)
{
	add_decoration(&idnums, object, mark_to_ptr(mark));
}

સ્થિર અંતઃરેખ શૂન્ય mark_next_object(સંરચના object *object)
{
	mark_object(object, ++last_idnum);
}

સ્થિર પૂર્ણાંક get_object_mark(સંરચના object *object)
{
	શૂન્ય *decoration = lookup_decoration(&idnums, object);
	જો (!decoration)
		પાછા 0;
	પાછા ptr_to_mark(decoration);
}

સ્થિર સંરચના commit *rewrite_commit(સંરચના commit *p)
{
	માટે (;;) {
		જો (p->parents && p->parents->next)
			તોડો;
		જો (p->object.flags & UNINTERESTING)
			તોડો;
		જો (!(p->object.flags & TREESAME))
			તોડો;
		જો (!p->parents)
			પાછા ખાલી;
		p = p->parents->item;
	}
	પાછા p;
}

સ્થિર શૂન્ય show_progress(શૂન્ય)
{
	સ્થિર પૂર્ણાંક counter = 0;
	જો (!progress)
		પાછા;
	જો ((++counter % progress) == 0)
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
સ્થિર અક્ષર *anonymize_blob(unsigned long *size)
{
	સ્થિર પૂર્ણાંક counter;
	સંરચના strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "anonymous blob %d", counter++);
	*size = out.len;
	પાછા strbuf_detach(&out, ખાલી);
}

સ્થિર શૂન્ય export_blob(અચલ સંરચના object_id *oid)
{
	unsigned long size;
	ગણના object_type type;
	અક્ષર *buf;
	સંરચના object *object;
	પૂર્ણાંક eaten;

	જો (no_data)
		પાછા;

	જો (is_null_oid(oid))
		પાછા;

	object = lookup_object(the_repository, oid);
	જો (object && object->flags & SHOWN)
		પાછા;

	જો (anonymize) {
		buf = anonymize_blob(&size);
		object = (સંરચના object *)lookup_blob(the_repository, oid);
		eaten = 0;
	} નહીંતર {
		size_t size_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &size_st);
		size = cast_size_t_to_ulong(size_st);
		જો (!buf)
			die(_("could not read blob %s"), oid_to_hex(oid));
		જો (check_object_signature(the_repository, oid, buf, size,
					   type) < 0)
			die(_("oid mismatch in blob %s"), oid_to_hex(oid));
		object = parse_object_buffer(the_repository, oid, type,
					     size, buf, &eaten);
	}

	જો (!object)
		die(_("could not read blob %s"), oid_to_hex(oid));

	mark_next_object(object);

	printf("blob\nmark :%"PRIu32"\n", last_idnum);
	જો (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(oid));
	printf("data %"PRIuMAX"\n", (uintmax_t)size);
	જો (size && fwrite(buf, size, 1, stdout) != 1)
		die_errno(_("could not write blob '%s'"), oid_to_hex(oid));
	printf("\n");

	show_progress();

	object->flags |= SHOWN;
	જો (!eaten)
		મુક્ત(buf);
}

સ્થિર પૂર્ણાંક depth_first(અચલ શૂન્ય *a_, અચલ શૂન્ય *b_)
{
	અચલ સંરચના diff_filepair *a = *((અચલ સંરચના diff_filepair **)a_);
	અચલ સંરચના diff_filepair *b = *((અચલ સંરચના diff_filepair **)b_);
	અચલ અક્ષર *name_a, *name_b;
	પૂર્ણાંક len_a, len_b, len;
	પૂર્ણાંક cmp;

	name_a = a->one ? a->one->path : a->two->path;
	name_b = b->one ? b->one->path : b->two->path;

	len_a = strlen(name_a);
	len_b = strlen(name_b);
	len = (len_a < len_b) ? len_a : len_b;

	/* strcmp will sort 'd' before 'd/e', we want 'd/e' before 'd' */
	cmp = memcmp(name_a, name_b, len);
	જો (cmp)
		પાછા cmp;
	cmp = len_b - len_a;
	જો (cmp)
		પાછા cmp;
	/*
	 * Move 'R'ename entries last so that all references of the file
	 * appear in the output before it is renamed (e.g., when a file
	 * was copied and renamed in the same commit).
	 */
	પાછા (a->status == 'R') - (b->status == 'R');
}

સ્થિર શૂન્ય print_path_1(અચલ અક્ષર *path)
{
	પૂર્ણાંક need_quote = quote_c_style(path, ખાલી, ખાલી, 0);
	જો (need_quote)
		quote_c_style(path, ખાલી, stdout, 0);
	નહીંતર જો (strchr(path, ' '))
		printf("\"%s\"", path);
	નહીંતર
		printf("%s", path);
}

સ્થિર અક્ષર *anonymize_path_component(શૂન્ય)
{
	સ્થિર પૂર્ણાંક counter;
	સંરચના strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "path%d", counter++);
	પાછા strbuf_detach(&out, ખાલી);
}

સ્થિર શૂન્ય print_path(અચલ અક્ષર *path)
{
	જો (!anonymize)
		print_path_1(path);
	નહીંતર {
		સ્થિર સંરચના hashmap paths;
		સ્થિર સંરચના strbuf anon = STRBUF_INIT;

		anonymize_path(&anon, path, &paths, anonymize_path_component);
		print_path_1(anon.buf);
		strbuf_reset(&anon);
	}
}

સ્થિર અક્ષર *generate_fake_oid(શૂન્ય)
{
	સ્થિર uint32_t counter = 1; /* avoid null oid */
	અચલ unsigned hashsz = the_hash_algo->rawsz;
	સંરચના object_id oid;
	અક્ષર *hex = xmallocz(GIT_MAX_HEXSZ);

	oidclr(&oid, the_repository->hash_algo);
	put_be32(oid.hash + hashsz - 4, counter++);
	પાછા oid_to_hex_r(hex, &oid);
}

સ્થિર અચલ અક્ષર *anonymize_oid(અચલ અક્ષર *oid_hex)
{
	સ્થિર સંરચના hashmap objs;
	size_t len = strlen(oid_hex);
	પાછા anonymize_str(&objs, generate_fake_oid, oid_hex, len);
}

સ્થિર શૂન્ય show_filemodify(સંરચના diff_queue_struct *q,
			    સંરચના diff_options *options UNUSED, શૂન્ય *data)
{
	પૂર્ણાંક i;
	સંરચના string_list *changed = data;

	/*
	 * Handle files below a directory first, in case they are all deleted
	 * and the directory changes to a file or symlink.
	 */
	QSORT(q->queue, q->nr, depth_first);

	માટે (i = 0; i < q->nr; i++) {
		સંરચના diff_filespec *ospec = q->queue[i]->one;
		સંરચના diff_filespec *spec = q->queue[i]->two;

		પસંદગી (q->queue[i]->status) {
		સ્થિતિ DIFF_STATUS_DELETED:
			printf("D ");
			print_path(spec->path);
			string_list_ઉમેરો(changed, spec->path);
			putchar('\n');
			તોડો;

		સ્થિતિ DIFF_STATUS_COPIED:
		સ્થિતિ DIFF_STATUS_RENAMED:
			/*
			 * If a change in the file corresponding to ospec->path
			 * has been observed, we cannot trust its contents
			 * because the diff is calculated based on the prior
			 * contents, not the current contents.  So, declare a
			 * copy or rename only if there was no change observed.
			 */
			જો (!string_list_has_string(changed, ospec->path)) {
				printf("%c ", q->queue[i]->status);
				print_path(ospec->path);
				putchar(' ');
				print_path(spec->path);
				string_list_ઉમેરો(changed, spec->path);
				putchar('\n');

				જો (oideq(&ospec->oid, &spec->oid) &&
				    ospec->mode == spec->mode)
					તોડો;
			}
			/* fallthrough */

		સ્થિતિ DIFF_STATUS_TYPE_CHANGED:
		સ્થિતિ DIFF_STATUS_MODIFIED:
		સ્થિતિ DIFF_STATUS_ADDED:
			/*
			 * Links refer to objects in another repositories;
			 * output the SHA-1 verbatim.
			 */
			જો (no_data || S_ISGITLINK(spec->mode))
				printf("M %06o %s ", spec->mode,
				       anonymize ?
				       anonymize_oid(oid_to_hex(&spec->oid)) :
				       oid_to_hex(&spec->oid));
			નહીંતર {
				સંરચના object *object = lookup_object(the_repository,
								      &spec->oid);
				printf("M %06o :%d ", spec->mode,
				       get_object_mark(object));
			}
			print_path(spec->path);
			string_list_ઉમેરો(changed, spec->path);
			putchar('\n');
			તોડો;

		મૂળભૂત:
			die(_("unexpected comparison status '%c' for %s, %s"),
			    q->queue[i]->status,
			    ospec->path ? ospec->path : _("none"),
			    spec->path ? spec->path : _("none"));
		}
	}
}

સ્થિર અક્ષર *anonymize_ref_component(શૂન્ય)
{
	સ્થિર પૂર્ણાંક counter;
	સંરચના strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "ref%d", counter++);
	પાછા strbuf_detach(&out, ખાલી);
}

સ્થિર અચલ અક્ષર *anonymize_refname(અચલ અક્ષર *refname)
{
	/*
	 * If any of these prefixes is found, we will leave it intact
	 * so that tags remain tags and so forth.
	 */
	સ્થિર અચલ અક્ષર *prefixes[] = {
		"refs/heads/",
		"refs/tags/",
		"refs/remotes/",
		"refs/"
	};
	સ્થિર સંરચના hashmap refs;
	સ્થિર સંરચના strbuf anon = STRBUF_INIT;
	પૂર્ણાંક i;

	strbuf_reset(&anon);
	માટે (i = 0; i < ARRAY_SIZE(prefixes); i++) {
		જો (skip_prefix(refname, prefixes[i], &refname)) {
			strbuf_addstr(&anon, prefixes[i]);
			તોડો;
		}
	}

	anonymize_path(&anon, refname, &refs, anonymize_ref_component);
	પાછા anon.buf;
}

/*
 * We do not even bother to cache commit messages, as they are unlikely
 * to be repeated verbatim, and it is not that interesting when they are.
 */
સ્થિર અક્ષર *anonymize_commit_message(શૂન્ય)
{
	સ્થિર પૂર્ણાંક counter;
	પાછા xstrfmt("subject %d\n\nbody\n", counter++);
}

સ્થિર અક્ષર *anonymize_ident(શૂન્ય)
{
	સ્થિર પૂર્ણાંક counter;
	સંરચના strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "User %d <user%d@example.com>", counter, counter);
	counter++;
	પાછા strbuf_detach(&out, ખાલી);
}

/*
 * Our strategy here is to anonymize the names and email addresses,
 * but keep timestamps intact, as they influence things like traversal
 * order (and by themselves should not be too revealing).
 */
સ્થિર શૂન્ય anonymize_ident_લીટી(અચલ અક્ષર **beg, અચલ અક્ષર **end)
{
	સ્થિર સંરચના hashmap idents;
	સ્થિર સંરચના strbuf buffers[] = { STRBUF_INIT, STRBUF_INIT };
	સ્થિર unsigned which_buffer;

	સંરચના strbuf *out;
	સંરચના ident_split split;
	અચલ અક્ષર *end_of_header;

	out = &buffers[which_buffer++];
	which_buffer %= ARRAY_SIZE(buffers);
	strbuf_reset(out);

	/* skip "committer", "author", "tagger", etc */
	end_of_header = strchr(*beg, ' ');
	જો (!end_of_header)
		BUG("malformed line fed to anonymize_ident_line: %.*s",
		    (પૂર્ણાંક)(*end - *beg), *beg);
	end_of_header++;
	strbuf_add(out, *beg, end_of_header - *beg);

	જો (!split_ident_લીટી(&split, end_of_header, *end - end_of_header) &&
	    split.date_begin) {
		અચલ અક્ષર *ident;
		size_t len;

		len = split.mail_end - split.name_begin;
		ident = anonymize_str(&idents, anonymize_ident,
				      split.name_begin, len);
		strbuf_addstr(out, ident);
		strbuf_addch(out, ' ');
		strbuf_add(out, split.date_begin, split.tz_end - split.date_begin);
	} નહીંતર {
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
સ્થિર અચલ અક્ષર *find_commit_multiline_header(અચલ અક્ષર *સંદેશ,
						અચલ અક્ષર *ચાવી,
						અચલ અક્ષર **end)
{
	સંરચના strbuf val = STRBUF_INIT;
	અચલ અક્ષર *bol, *eol;
	size_t len;

	bol = find_commit_header(સંદેશ, ચાવી, &len);
	જો (!bol)
		પાછા ખાલી;
	eol = bol + len;
	strbuf_add(&val, bol, len);

	જ્યારે (eol[0] == '\n' && eol[1] == ' ') {
		bol = eol + 2;
		eol = strchrnul(bol, '\n');
		strbuf_addch(&val, '\n');
		strbuf_add(&val, bol, eol - bol);
	}

	*end = eol;
	પાછા strbuf_detach(&val, ખાલી);
}

સ્થિર શૂન્ય print_signature(અચલ અક્ષર *signature, અચલ અક્ષર *object_hash)
{
	જો (!signature)
		પાછા;

	printf("gpgsig %s %s\ndata %u\n%s\n",
	       object_hash,
	       get_signature_format(signature),
	       (unsigned)strlen(signature),
	       signature);
}

સ્થિર અચલ અક્ષર *append_signatures_for_header(સંરચના string_list *signatures,
						અચલ અક્ષર *pos,
						અચલ અક્ષર *header,
						અચલ અક્ષર *object_hash)
{
	અચલ અક્ષર *signature;
	અચલ અક્ષર *start = pos;
	અચલ અક્ષર *end = pos;

	જ્યારે ((signature = find_commit_multiline_header(start + 1,
							 header,
							 &end))) {
		string_list_append(signatures, signature)->util = (શૂન્ય *)object_hash;
		મુક્ત((અક્ષર *)signature);
		start = end;
	}

	પાછા end;
}

સ્થિર શૂન્ય handle_commit(સંરચના commit *commit, સંરચના rev_info *rev,
			  સંરચના string_list *paths_of_changed_objects)
{
	પૂર્ણાંક saved_output_format = rev->diffopt.output_format;
	અચલ અક્ષર *commit_buffer, *commit_buffer_cursor;
	અચલ અક્ષર *author, *author_end, *committer, *committer_end;
	અચલ અક્ષર *encoding = ખાલી;
	size_t encoding_len;
	સંરચના string_list signatures = STRING_LIST_INIT_DUP;
	અચલ અક્ષર *message;
	અક્ષર *reencoded = ખાલી;
	સંરચના commit_list *p;
	અચલ અક્ષર *refname;
	પૂર્ણાંક i;

	rev->diffopt.output_format = DIFF_FORMAT_CALLBACK;

	parse_commit_or_die(commit);
	commit_buffer_cursor = commit_buffer = repo_get_commit_buffer(the_repository, commit, ખાલી);

	author = strstr(commit_buffer_cursor, "\nauthor ");
	જો (!author)
		die(_("could not find author in commit %s"),
		    oid_to_hex(&commit->object.oid));
	author++;
	commit_buffer_cursor = author_end = strchrnul(author, '\n');

	committer = strstr(commit_buffer_cursor, "\ncommitter ");
	જો (!committer)
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

	જો (*commit_buffer_cursor == '\n') {
		encoding = find_commit_header(commit_buffer_cursor + 1, "encoding", &encoding_len);
		જો (encoding)
			commit_buffer_cursor = encoding + encoding_len;
	}

	જો (*commit_buffer_cursor == '\n') {
		અચલ અક્ષર *after_sha1 = append_signatures_for_header(&signatures, commit_buffer_cursor,
								      "gpgsig", "sha1");
		અચલ અક્ષર *after_sha256 = append_signatures_for_header(&signatures, commit_buffer_cursor,
									"gpgsig-sha256", "sha256");
		commit_buffer_cursor = (after_sha1 > after_sha256) ? after_sha1 : after_sha256;
	}

	message = strstr(commit_buffer_cursor, "\n\n");
	જો (message)
		message += 2;

	જો (commit->parents &&
	    (get_object_mark(&commit->parents->item->object) != 0 ||
	     reference_excluded_commits) &&
	    !full_વૃક્ષ) {
		parse_commit_or_die(commit->parents->item);
		diff_વૃક્ષ_oid(get_commit_વૃક્ષ_oid(commit->parents->item),
			      get_commit_વૃક્ષ_oid(commit), "", &rev->diffopt);
	}
	નહીંતર
		diff_મૂળ_વૃક્ષ_oid(get_commit_વૃક્ષ_oid(commit),
				   "", &rev->diffopt);

	/* Export the referenced blobs, and remember the marks. */
	માટે (i = 0; i < diff_queued_diff.nr; i++)
		જો (!S_ISGITLINK(diff_queued_diff.queue[i]->two->mode))
			export_blob(&diff_queued_diff.queue[i]->two->oid);

	refname = *revision_sources_at(&revision_sources, commit);
	/*
	 * FIXME: string_list_remove() below for each ref is overall
	 * O(N^2).  Compared to a history walk and diffing trees, this is
	 * just lost in the noise in practice.  However, theoretically a
	 * repo may have enough refs for this to become slow.
	 */
	string_list_remove(&extra_refs, refname, 0);
	જો (anonymize) {
		refname = anonymize_refname(refname);
		anonymize_ident_લીટી(&committer, &committer_end);
		anonymize_ident_લીટી(&author, &author_end);
	}

	mark_next_object(&commit->object);
	જો (anonymize) {
		reencoded = anonymize_commit_message();
	} નહીંતર જો (encoding) {
		અક્ષર *buf;
		પસંદગી (reencode_mode) {
		સ્થિતિ REENCODE_YES:
			buf = xstrfmt("%.*s", (પૂર્ણાંક)encoding_len, encoding);
			reencoded = reencode_string(message, "UTF-8", buf);
			મુક્ત(buf);
			તોડો;
		સ્થિતિ REENCODE_NO:
			તોડો;
		સ્થિતિ REENCODE_ABORT:
			die(_("encountered commit-specific encoding %.*s in commit "
			      "%s; use --reencode=[yes|no] to handle it"),
			    (પૂર્ણાંક)encoding_len, encoding,
			    oid_to_hex(&commit->object.oid));
		}
	}
	જો (!commit->parents)
		printf("reset %s\n", refname);
	printf("commit %s\nmark :%"PRIu32"\n", refname, last_idnum);
	જો (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&commit->object.oid));
	printf("%.*s\n%.*s\n",
	       (પૂર્ણાંક)(author_end - author), author,
	       (પૂર્ણાંક)(committer_end - committer), committer);
	જો (signatures.nr) {
		પસંદગી (signed_commit_mode) {
		/* Exporting modes */
		સ્થિતિ SIGN_WARN_VERBATIM:
			warning(_("exporting %"PRIuMAX" signature(s) for commit %s"),
				(uintmax_t)signatures.nr, oid_to_hex(&commit->object.oid));
			/* fallthru */
		સ્થિતિ SIGN_VERBATIM:
			માટે (size_t i = 0; i < signatures.nr; i++) {
				સંરચના string_list_item *item = &signatures.વસ્તુઓ[i];
				print_signature(item->string, item->util);
			}
			તોડો;

		/* Stripping modes */
		સ્થિતિ SIGN_WARN_STRIP:
			warning(_("stripping signature(s) from commit %s"),
				oid_to_hex(&commit->object.oid));
			/* fallthru */
		સ્થિતિ SIGN_STRIP:
			તોડો;

		/* Aborting modes */
		સ્થિતિ SIGN_ABORT:
			die(_("encountered signed commit %s; use "
			      "--signed-commits=<mode> to handle it"),
			    oid_to_hex(&commit->object.oid));
		મૂળભૂત:
			BUG("invalid signed_commit_mode value %d", signed_commit_mode);
		}
		string_list_clear(&signatures, 0);
	}
	જો (!reencoded && encoding)
		printf("encoding %.*s\n", (પૂર્ણાંક)encoding_len, encoding);
	printf("data %u\n%s",
	       (unsigned)(reencoded
			  ? strlen(reencoded) : message
			  ? strlen(message) : 0),
	       reencoded ? reencoded : message ? message : "");
	મુક્ત(reencoded);
	repo_unuse_commit_buffer(the_repository, commit, commit_buffer);

	માટે (i = 0, p = commit->parents; p; p = p->next) {
		સંરચના object *obj = &p->item->object;
		પૂર્ણાંક mark = get_object_mark(obj);

		જો (!mark && !reference_excluded_commits)
			ચાલુ;
		જો (i == 0)
			printf("from ");
		નહીંતર
			printf("merge ");
		જો (mark)
			printf(":%d\n", mark);
		નહીંતર
			printf("%s\n",
			       anonymize ?
			       anonymize_oid(oid_to_hex(&obj->oid)) :
			       oid_to_hex(&obj->oid));
		i++;
	}

	જો (full_વૃક્ષ)
		printf("deleteall\n");
	log_વૃક્ષ_diff_flush(rev);
	string_list_clear(paths_of_changed_objects, 0);
	rev->diffopt.output_format = saved_output_format;

	printf("\n");

	show_progress();
}

સ્થિર અક્ષર *anonymize_tag(શૂન્ય)
{
	સ્થિર પૂર્ણાંક counter;
	સંરચના strbuf out = STRBUF_INIT;
	strbuf_addf(&out, "tag message %d", counter++);
	પાછા strbuf_detach(&out, ખાલી);
}


સ્થિર શૂન્ય handle_tag(અચલ અક્ષર *name, સંરચના tag *tag)
{
	size_t size;
	ગણના object_type type;
	અક્ષર *buf;
	અચલ અક્ષર *tagger, *tagger_end, *message;
	size_t message_size = 0;
	સંરચના object *tagged;
	પૂર્ણાંક tagged_mark;
	સંરચના commit *p;

	/* Trees have no identifier in fast-export output, thus we have no way
	 * to output tags of trees, tags of tags of trees, etc.  Simply omit
	 * such tags.
	 */
	tagged = tag->tagged;
	જ્યારે (tagged->type == OBJ_TAG) {
		tagged = ((સંરચના tag *)tagged)->tagged;
	}
	જો (tagged->type == OBJ_વૃક્ષ) {
		warning(_("omitting tag %s,\nsince tags of trees (or tags "
			  "of tags of trees, etc.) are not supported."),
			oid_to_hex(&tag->object.oid));
		પાછા;
	}

	buf = odb_read_object(the_repository->objects, &tag->object.oid,
			      &type, &size);
	જો (!buf)
		die(_("could not read tag %s"), oid_to_hex(&tag->object.oid));
	message = memmem(buf, size, "\n\n", 2);
	જો (message) {
		message += 2;
		message_size = strlen(message);
	}
	tagger = memmem(buf, message ? message - buf : size, "\ntagger ", 8);
	જો (!tagger) {
		જો (fake_missing_tagger)
			tagger = "tagger Unspecified Tagger "
				"<unspecified-tagger> 0 +0000";
		નહીંતર
			tagger = "";
		tagger_end = tagger + strlen(tagger);
	} નહીંતર {
		tagger++;
		tagger_end = strchrnul(tagger, '\n');
		જો (anonymize)
			anonymize_ident_લીટી(&tagger, &tagger_end);
	}

	જો (anonymize) {
		name = anonymize_refname(name);
		જો (message) {
			સ્થિર સંરચના hashmap tags;
			message = anonymize_str(&tags, anonymize_tag,
						message, message_size);
			message_size = strlen(message);
		}
	}

	/* handle signed tags */
	જો (message) {
		size_t sig_offset = parse_signed_buffer(message, message_size);
		જો (sig_offset < message_size)
			પસંદગી (signed_tag_mode) {
			/* Exporting modes */
			સ્થિતિ SIGN_WARN_VERBATIM:
				warning(_("exporting signed tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			સ્થિતિ SIGN_VERBATIM:
				તોડો;

			/* Stripping modes */
			સ્થિતિ SIGN_WARN_STRIP:
				warning(_("stripping signature from tag %s"),
					oid_to_hex(&tag->object.oid));
				/* fallthru */
			સ્થિતિ SIGN_STRIP:
				message_size = sig_offset;
				તોડો;

			/* Aborting modes */
			સ્થિતિ SIGN_ABORT:
				die(_("encountered signed tag %s; use "
				      "--signed-tags=<mode> to handle it"),
				    oid_to_hex(&tag->object.oid));
			મૂળભૂત:
				BUG("invalid signed_commit_mode value %d", signed_commit_mode);
			}
	}

	/* handle tag->tagged having been filtered out due to paths specified */
	tagged = tag->tagged;
	tagged_mark = get_object_mark(tagged);
	જો (!tagged_mark) {
		પસંદગી (tag_of_filtered_mode) {
		સ્થિતિ TAG_FILTERING_ABORT:
			die(_("tag %s tags unexported object; use "
			      "--tag-of-filtered-object=<mode> to handle it"),
			    oid_to_hex(&tag->object.oid));
		સ્થિતિ DROP:
			/* Ignore this tag altogether */
			મુક્ત(buf);
			પાછા;
		સ્થિતિ REWRITE:
			જો (tagged->type == OBJ_TAG && !mark_tags) {
				die(_("cannot export nested tags unless --mark-tags is specified."));
			} નહીંતર જો (tagged->type == OBJ_COMMIT) {
				p = rewrite_commit((સંરચના commit *)tagged);
				જો (!p) {
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					મુક્ત(buf);
					પાછા;
				}
				tagged_mark = get_object_mark(&p->object);
			} નહીંતર {
				/* tagged->type is either OBJ_BLOB or OBJ_TAG */
				tagged_mark = get_object_mark(tagged);
			}
		}
	}

	જો (tagged->type == OBJ_TAG) {
		printf("reset %s\nfrom %s\n\n",
		       name, oid_to_hex(null_oid(the_hash_algo)));
	}
	skip_prefix(name, "refs/tags/", &name);
	printf("tag %s\n", name);
	જો (mark_tags) {
		mark_next_object(&tag->object);
		printf("mark :%"PRIu32"\n", last_idnum);
	}
	જો (tagged_mark)
		printf("from :%d\n", tagged_mark);
	નહીંતર
		printf("from %s\n", oid_to_hex(&tagged->oid));

	જો (show_original_ids)
		printf("original-oid %s\n", oid_to_hex(&tag->object.oid));
	printf("%.*s%sdata %d\n%.*s\n",
	       (પૂર્ણાંક)(tagger_end - tagger), tagger,
	       tagger == tagger_end ? "" : "\n",
	       (પૂર્ણાંક)message_size, (પૂર્ણાંક)message_size, message ? message : "");
	મુક્ત(buf);
}

સ્થિર સંરચના commit *get_commit(સંરચના rev_cmdline_entry *e, અચલ અક્ષર *full_name)
{
	પસંદગી (e->item->type) {
	સ્થિતિ OBJ_COMMIT:
		પાછા (સંરચના commit *)e->item;
	સ્થિતિ OBJ_TAG: {
		સંરચના tag *tag = (સંરચના tag *)e->item;

		/* handle nested tags */
		જ્યારે (tag && tag->object.type == OBJ_TAG) {
			parse_object(the_repository, &tag->object.oid);
			string_list_append(&tag_refs, full_name)->util = tag;
			tag = (સંરચના tag *)tag->tagged;
		}
		જો (!tag)
			die(_("tag %s points nowhere?"), e->name);
		પાછા (સંરચના commit *)tag;
	}
	મૂળભૂત:
		પાછા ખાલી;
	}
}

સ્થિર શૂન્ય get_tags_and_duplicates(સંરચના rev_cmdline_info *info)
{
	પૂર્ણાંક i;

	માટે (i = 0; i < info->nr; i++) {
		સંરચના rev_cmdline_entry *e = info->rev + i;
		સંરચના object_id oid;
		સંરચના commit *commit;
		અક્ષર *full_name = ખાલી;

		જો (e->flags & UNINTERESTING)
			ચાલુ;

		જો (repo_dwim_ref(the_repository, e->name, strlen(e->name),
				  &oid, &full_name, 0) != 1) {
			મુક્ત(full_name);
			ચાલુ;
		}

		જો (refspecs.nr) {
			અક્ષર *ખાનગી;
			ખાનગી = apply_refspecs(&refspecs, full_name);
			જો (ખાનગી) {
				મુક્ત(full_name);
				full_name = ખાનગી;
			}
		}

		commit = get_commit(e, full_name);
		જો (!commit) {
			warning(_("%s: unexpected object of type %s, skipping."),
				e->name,
				type_name(e->item->type));
			મુક્ત(full_name);
			ચાલુ;
		}

		પસંદગી (commit->object.type) {
		સ્થિતિ OBJ_COMMIT:
			તોડો;
		સ્થિતિ OBJ_BLOB:
			export_blob(&commit->object.oid);
			મુક્ત(full_name);
			ચાલુ;
		મૂળભૂત: /* OBJ_TAG (nested tags) is already handled */
			warning(_("tag points to object of unexpected type %s, skipping."),
				type_name(commit->object.type));
			મુક્ત(full_name);
			ચાલુ;
		}

		/*
		 * Make sure this ref gets properly updated eventually, whether
		 * through a commit or manually at the end.
		 */
		જો (e->item->type != OBJ_TAG)
			string_list_append(&extra_refs, full_name)->util = commit;

		જો (!*revision_sources_at(&revision_sources, commit))
			*revision_sources_at(&revision_sources, commit) = full_name;
		નહીંતર
			મુક્ત(full_name);
	}

	string_list_sort_u(&extra_refs, 0);
}

સ્થિર શૂન્ય handle_tags_and_duplicates(સંરચના string_list *extras)
{
	સંરચના commit *commit;
	પૂર્ણાંક i;

	માટે (i = extras->nr - 1; i >= 0; i--) {
		અચલ અક્ષર *name = extras->વસ્તુઓ[i].string;
		સંરચના object *object = extras->વસ્તુઓ[i].util;
		પૂર્ણાંક mark;

		પસંદગી (object->type) {
		સ્થિતિ OBJ_TAG:
			handle_tag(name, (સંરચના tag *)object);
			તોડો;
		સ્થિતિ OBJ_COMMIT:
			જો (anonymize)
				name = anonymize_refname(name);
			/* create refs pointing to already seen commits */
			commit = rewrite_commit((સંરચના commit *)object);
			જો (!commit) {
				/*
				 * Neither this object nor any of its
				 * ancestors touch any relevant paths, so
				 * it has been filtered to nothing.  Delete
				 * it.
				 */
				printf("reset %s\nfrom %s\n\n",
				       name, oid_to_hex(null_oid(the_hash_algo)));
				ચાલુ;
			}

			mark = get_object_mark(&commit->object);
			જો (!mark) {
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
				જો (!reference_excluded_commits) {
					/* delete the ref */
					printf("reset %s\nfrom %s\n\n",
					       name, oid_to_hex(null_oid(the_hash_algo)));
					ચાલુ;
				}
				/* set ref to commit using oid, not mark */
				printf("reset %s\nfrom %s\n\n", name,
				       oid_to_hex(&commit->object.oid));
				ચાલુ;
			}

			printf("reset %s\nfrom :%d\n\n", name, mark
			       );
			show_progress();
			તોડો;
		}
	}
}

સ્થિર શૂન્ય export_marks(અક્ષર *file)
{
	unsigned પૂર્ણાંક i;
	uint32_t mark;
	સંરચના decoration_entry *deco = idnums.entries;
	FILE *f;
	પૂર્ણાંક e = 0;

	f = fopen_for_writing(file);
	જો (!f)
		die_errno(_("unable to open marks file %s for writing."), file);

	માટે (i = 0; i < idnums.size; i++) {
		જો (deco->base && deco->base->type == 1) {
			mark = ptr_to_mark(deco->decoration);
			જો (fprintf(f, ":%"PRIu32" %s\n", mark,
				oid_to_hex(&deco->base->oid)) < 0) {
			    e = 1;
			    તોડો;
			}
		}
		deco++;
	}

	e |= ferror(f);
	e |= fclose(f);
	જો (e)
		error(_("unable to write marks file %s."), file);
}

સ્થિર શૂન્ય import_marks(અક્ષર *આગત_file, પૂર્ણાંક check_exists)
{
	અક્ષર લીટી[512];
	FILE *f;
	સંરચના stat sb;

	જો (check_exists && stat(આગત_file, &sb))
		પાછા;

	f = xfopen(આગત_file, "r");
	જ્યારે (fgets(લીટી, કદ(લીટી), f)) {
		uint32_t mark;
		અક્ષર *લીટી_end, *mark_end;
		સંરચના object_id oid;
		સંરચના object *object;
		સંરચના commit *commit;
		ગણના object_type type;

		લીટી_end = strchr(લીટી, '\n');
		જો (લીટી[0] != ':' || !લીટી_end)
			die(_("corrupt mark line: %s"), લીટી);
		*લીટી_end = '\0';

		mark = strtoumax(લીટી + 1, &mark_end, 10);
		જો (!mark || mark_end == લીટી + 1
			|| *mark_end != ' ' || get_oid_hex(mark_end + 1, &oid))
			die(_("corrupt mark line: %s"), લીટી);

		જો (last_idnum < mark)
			last_idnum = mark;

		type = odb_read_object_info(the_repository->objects, &oid, ખાલી);
		જો (type < 0)
			die(_("object not found: %s"), oid_to_hex(&oid));

		જો (type != OBJ_COMMIT)
			/* only commits */
			ચાલુ;

		commit = lookup_commit(the_repository, &oid);
		જો (!commit)
			die(_("not a commit? can't happen: %s"), oid_to_hex(&oid));

		object = &commit->object;

		જો (object->flags & SHOWN)
			error(_("object %s already has a mark"), oid_to_hex(&oid));

		mark_object(object, mark);

		object->flags |= SHOWN;
	}
	fclose(f);
}

સ્થિર શૂન્ય handle_deletes(શૂન્ય)
{
	પૂર્ણાંક i;
	માટે (i = 0; i < refspecs.nr; i++) {
		સંરચના refspec_item *refspec = &refspecs.વસ્તુઓ[i];
		જો (*refspec->src)
			ચાલુ;

		printf("reset %s\nfrom %s\n\n",
				refspec->dst, oid_to_hex(null_oid(the_hash_algo)));
	}
}

સ્થિર પૂર્ણાંક parse_opt_anonymize_map(અચલ સંરચના option *opt,
				   અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના hashmap *map = opt->કિંમત;
	અચલ અક્ષર *delim, *કિંમત;
	size_t keylen;

	BUG_ON_OPT_NEG(unset);

	delim = strchr(arg, ':');
	જો (delim) {
		keylen = delim - arg;
		કિંમત = delim + 1;
	} નહીંતર {
		keylen = strlen(arg);
		કિંમત = arg;
	}

	જો (!keylen || !*કિંમત)
		પાછા error(_("--anonymize-map token cannot be empty"));

	add_anonymized_entry(map, memhash(arg, keylen), arg, keylen,
			     xstrdup(કિંમત));

	પાછા 0;
}

પૂર્ણાંક cmd_fast_export(પૂર્ણાંક argc,
		    અચલ અક્ષર **argv,
		    અચલ અક્ષર *prefix,
		    સંરચના repository *repo UNUSED)
{
	સંરચના rev_info revs;
	સંરચના commit *commit;
	અક્ષર *export_filename = ખાલી,
	     *import_filename = ખાલી,
	     *import_filename_if_exists = ખાલી;
	uint32_t lastimportid;
	સંરચના string_list refspecs_list = STRING_LIST_INIT_NODUP;
	સંરચના string_list paths_of_changed_objects = STRING_LIST_INIT_DUP;
	સંરચના option options[] = {
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
		OPT_BOOL(0, "full-tree", &full_વૃક્ષ,
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

	જો (argc == 1)
		usage_with_options (fast_export_usage, options);

	/* we handle encodings */
	repo_config(the_repository, git_default_config, ખાલી);

	refspec_init_fetch(&refspecs, the_hash_algo);

	repo_init_revisions(the_repository, &revs, prefix);
	init_revision_sources(&revision_sources);
	revs.topo_order = 1;
	revs.sources = &revision_sources;
	revs.rewrite_parents = 1;
	argc = parse_options(argc, argv, prefix, options, fast_export_usage,
			PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_UNKNOWN_OPT);
	argc = setup_revisions(argc, argv, &revs, ખાલી);
	જો (argc > 1)
		usage_with_options (fast_export_usage, options);

	જો (anonymized_seeds.cmpfn && !anonymize)
		die(_("the option '%s' requires '%s'"), "--anonymize-map", "--anonymize");

	જો (refspecs_list.nr) {
		પૂર્ણાંક i;

		માટે (i = 0; i < refspecs_list.nr; i++)
			refspec_append(&refspecs, refspecs_list.વસ્તુઓ[i].string);

		string_list_clear(&refspecs_list, 1);
	}

	જો (use_done_feature)
		printf("feature done\n");

	જો (import_filename && import_filename_if_exists)
		die(_("options '%s' and '%s' cannot be used together"), "--import-marks", "--import-marks-if-exists");
	જો (import_filename)
		import_marks(import_filename, 0);
	નહીંતર જો (import_filename_if_exists)
		import_marks(import_filename_if_exists, 1);
	lastimportid = last_idnum;

	જો (import_filename && revs.prune_data.nr)
		full_વૃક્ષ = 1;

	get_tags_and_duplicates(&revs.cmdline);

	જો (prepare_revision_walk(&revs))
		die(_("revision walk setup failed"));

	revs.reverse = 1;
	revs.diffopt.format_callback = show_filemodify;
	revs.diffopt.format_callback_data = &paths_of_changed_objects;
	revs.diffopt.flags.recursive = 1;

	revs.diffopt.no_free = 1;
	જ્યારે ((commit = get_revision(&revs)))
		handle_commit(commit, &revs, &paths_of_changed_objects);
	revs.diffopt.no_free = 0;

	handle_tags_and_duplicates(&extra_refs);
	handle_tags_and_duplicates(&tag_refs);
	handle_deletes();

	જો (export_filename && lastimportid != last_idnum)
		export_marks(export_filename);

	જો (use_done_feature)
		printf("done\n");

	refspec_clear(&refspecs);
	release_revisions(&revs);

	પાછા 0;
}
