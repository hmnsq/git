/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "editor.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "gpg-interface.h"
#ଅନ୍ତର୍ଭାଵ "oid-array.h"
#ଅନ୍ତର୍ଭାଵ "column.h"
#ଅନ୍ତର୍ଭାଵ "ref-filter.h"
#ଅନ୍ତର୍ଭାଵ "date.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"
#ଅନ୍ତର୍ଭାଵ "object-file-convert.h"
#ଅନ୍ତର୍ଭାଵ "trailer.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_tag_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	ଖାଲି
};

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା colopts;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_sign_annotate;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା config_sign_tag = -1; /* unspecified */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା list_tags(ସଂରଚନା ref_filter *filter, ସଂରଚନା ref_sorting *sorting,
		     ସଂରଚନା ref_format *format)
{
	ଅକ୍ଷର *to_free = ଖାଲି;

	ଯଦି (filter->ଧାଡ଼ିଗୁଡ଼ିକ == -1)
		filter->ଧାଡ଼ିଗୁଡ଼ିକ = 0;

	ଯଦି (!format->format) {
		ଯଦି (filter->ଧାଡ଼ିଗୁଡ଼ିକ) {
			to_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->ଧାଡ଼ିଗୁଡ଼ିକ);
			format->format = to_free;
		} ଅନ୍ୟଥା
			format->format = "%(refname:lstrip=2)";
	}

	ଯଦି (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_tag_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_TAGS, sorting, format);

	ମୁକ୍ତ(to_free);

	ଫେରନ୍ତୁ 0;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*each_tag_name_fn)(ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *ref,
				ଅଚଳ ସଂରଚନା object_id *oid, ଶୂନ୍ୟ *cb_data);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା for_each_tag_name(ଅଚଳ ଅକ୍ଷର **argv, each_tag_name_fn fn,
			     ଶୂନ୍ୟ *cb_data)
{
	ଅଚଳ ଅକ୍ଷର **p;
	ସଂରଚନା strbuf ref = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା had_error = 0;
	ସଂରଚନା object_id oid;

	ପାଇଁ (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		ଯଦି (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			ଜାରି;
		}
		ଯଦି (fn(*p, ref.buf, &oid, cb_data))
			had_error = 1;
	}
	strbuf_release(&ref);
	ଫେରନ୍ତୁ had_error;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା collect_tags(ଅଚଳ ଅକ୍ଷର *name UNUSED, ଅଚଳ ଅକ୍ଷର *ref,
			ଅଚଳ ସଂରଚନା object_id *oid, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା string_list *ref_list = cb_data;

	string_list_append(ref_list, ref);
	ref_list->ବସ୍ତୁଗୁଡ଼ିକ[ref_list->nr - 1].util = oiddup(oid);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା delete_tags(ଅଚଳ ଅକ୍ଷର **argv)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;
	ସଂରଚନା string_list refs_to_delete = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list_item *item;

	result = for_each_tag_name(argv, collect_tags, (ଶୂନ୍ୟ *)&refs_to_delete);
	ଯଦି (refs_delete_refs(get_main_ref_store(the_repository), ଖାଲି, &refs_to_delete, REF_NO_DEREF))
		result = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		ଅଚଳ ଅକ୍ଷର *name = item->string;
		ସଂରଚନା object_id *oid = item->util;
		ଯଦି (!refs_ref_exists(get_main_ref_store(the_repository), name))
			printf(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		ମୁକ୍ତ(oid);
	}
	string_list_clear(&refs_to_delete, 0);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verify_tag(ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *ref UNUSED,
		      ଅଚଳ ସଂରଚନା object_id *oid, ଶୂନ୍ୟ *cb_data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
	ସଂରଚନା ref_format *format = cb_data;
	flags = GPG_VERIFY_VERBOSE;

	ଯଦି (format->format)
		flags = GPG_VERIFY_OMIT_STATUS;

	ଯଦି (gpg_verify_tag(the_repository, oid, name, flags))
		ଫେରନ୍ତୁ -1;

	ଯଦି (format->format)
		pretty_print_ref(name, oid, ଖାଲି, format);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_sign(ସଂରଚନା strbuf *buffer, ସଂରଚନା object_id **compat_oid,
		   ସଂରଚନା object_id *compat_oid_buf)
{
	ଅଚଳ ସଂରଚନା git_hash_algo *compat = the_repository->compat_hash_algo;
	ସଂରଚନା strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	ସଂରଚନା strbuf compat_buf = STRBUF_INIT;
	ଅକ୍ଷର *keyid = get_signing_ଚାବି();
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;

	ଯଦି (sign_buffer(buffer, &sig, keyid, 0))
		ଯାଅ out;

	ଯଦି (compat) {
		ଅଚଳ ସଂରଚନା git_hash_algo *algo = the_repository->hash_algo;

		ଯଦି (convert_object_file(the_repository ,&compat_buf, algo, compat,
					buffer->buf, buffer->len, OBJ_TAG, 1))
			ଯାଅ out;
		ଯଦି (sign_buffer(&compat_buf, &compat_sig, keyid, 0))
			ଯାଅ out;
		add_header_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_TAG, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	ଯଦି (compat_sig.len)
		add_header_signature(buffer, &compat_sig, compat);

	strbuf_addbuf(buffer, &sig);
	ret = 0;
out:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	ମୁକ୍ତ(keyid);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର tag_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର tag_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_tag_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			  ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(var, "tag.gpgsign")) {
		config_sign_tag = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(var, "tag.sort")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		string_list_append(cb, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (starts_with(var, "column."))
		ଫେରନ୍ତୁ git_column_config(var, ମୂଲ୍ୟ, "tag", &colopts);

	ଯଦି (git_color_config(var, ମୂଲ୍ୟ, cb) < 0)
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ସ୍ଥିର ଶୂନ୍ୟ write_tag_body(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ସଂରଚନା object_id *oid)
{
	size_t size;
	ଗଣନା object_type type;
	ଅକ୍ଷର *buf, *sp, *orig;
	ସଂରଚନା strbuf payload = STRBUF_INIT;
	ସଂରଚନା strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &size);
	ଯଦି (!buf)
		ଫେରନ୍ତୁ;
	ଯଦି (parse_signature(buf, size, &payload, &signature)) {
		buf = payload.buf;
		size = payload.len;
	}
	/* skip header */
	sp = strstr(buf, "\n\n");

	ଯଦି (!sp || !size || type != OBJ_TAG) {
		ମୁକ୍ତ(buf);
		ଫେରନ୍ତୁ;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + size - sp);

	ମୁକ୍ତ(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା build_tag_object(ସଂରଚନା strbuf *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sign, ସଂରଚନା object_id *result)
{
	ସଂରଚନା object_id *compat_oid = ଖାଲି, compat_oid_buf;
	ଯଦି (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		ଫେରନ୍ତୁ error(_("unable to sign the tag"));
	ଯଦି (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_TAG, result, compat_oid, 0) < 0)
		ଫେରନ୍ତୁ error(_("unable to write tag file"));
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା create_tag_options {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା message_given:1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_editor:1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sign;
	ଗଣନା {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_mode;
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର message_advice_nested_tag[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

ସ୍ଥିର ଶୂନ୍ୟ create_tag(ଅଚଳ ସଂରଚନା object_id *object, ଅଚଳ ଅକ୍ଷର *object_ref,
		       ଅଚଳ ଅକ୍ଷର *tag,
		       ସଂରଚନା strbuf *buf, ସଂରଚନା create_tag_options *opt,
		       ସଂରଚନା object_id *prev, ସଂରଚନା object_id *result,
		       ସଂରଚନା strvec *trailer_args, ଅକ୍ଷର *path)
{
	ଗଣନା object_type type;
	ସଂରଚନା strbuf header = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା should_edit;

	type = odb_read_object_info(the_repository->objects, object, ଖାଲି);
	ଯଦି (type <= OBJ_NONE)
		die(_("bad object type."));

	ଯଦି (type == OBJ_TAG)
		advise_if_enabled(ADVICE_NESTED_TAG, _(message_advice_nested_tag),
				  tag, object_ref);

	strbuf_addf(&header,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n"
		    "tagger %s\n\n",
		    oid_to_hex(object),
		    type_name(type),
		    tag,
		    git_committer_info(IDENT_STRICT));

	should_edit = opt->use_editor || !opt->message_given;
	ଯଦି (should_edit || trailer_args->nr) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

		/* write the template message before editing: */
		fd = xopen(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		ଯଦି (opt->message_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} ଅନ୍ୟଥା ଯଦି (!is_null_oid(prev)) {
			write_tag_body(fd, prev);
		} ଅନ୍ୟଥା {
			ସଂରଚନା strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			ଯଦି (opt->cleanup_mode == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_ଧାଡ଼ି_str,
				      _(tag_template), tag, comment_ଧାଡ଼ି_str);
			ଅନ୍ୟଥା
				strbuf_commented_addf(&buf, comment_ଧାଡ଼ି_str,
				      _(tag_template_nocleanup), tag, comment_ଧାଡ଼ି_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		ଯଦି (trailer_args->nr && amend_file_with_trailers(path, trailer_args))
			die(_("unable to pass trailers to --trailers"));

		ଯଦି (should_edit) {
			ଯଦି (launch_editor(path, buf, ଖାଲି)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				exit(1);
			}
		} ଅନ୍ୟଥା ଯଦି (trailer_args->nr) {
			strbuf_reset(buf);
			ଯଦି (strbuf_read_file(buf, path, 0) < 0)
				die_errno(_("failed to read '%s'"), path);
		}
	}

	ଯଦି (opt->cleanup_mode != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_mode == CLEANUP_ALL ? comment_ଧାଡ଼ି_str : ଖାଲି);

	ଯଦି (!opt->message_given && !buf->len)
		die(_("no tag message?"));

	strbuf_ଭର୍ତ୍ତି(buf, 0, header.buf, header.len);
	strbuf_release(&header);

	ଯଦି (build_tag_object(buf, opt->sign, result) < 0) {
		ଯଦି (path)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				path);
		exit(128);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ create_reflog_ସନ୍ଦେଶ(ଅଚଳ ସଂରଚନା object_id *oid, ସଂରଚନା strbuf *sb)
{
	ଗଣନା object_type type;
	ସଂରଚନା commit *c;
	ଅକ୍ଷର *buf;
	size_t size;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା subject_len = 0;
	ଅଚଳ ଅକ୍ଷର *subject_start;

	ଅକ୍ଷର *rla = getenv("GIT_REFLOG_ACTION");
	ଯଦି (rla) {
		strbuf_addstr(sb, rla);
	} ଅନ୍ୟଥା {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_add_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, ଖାଲି);
	ବାଛ (type) {
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		strbuf_addstr(sb, "object of unknown type");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି OBJ_COMMIT:
		ଯଦି ((buf = odb_read_object(the_repository->objects, oid, &type, &size))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_ଭର୍ତ୍ତି(sb, sb->len, subject_start, subject_len);
		} ଅନ୍ୟଥା {
			strbuf_addstr(sb, "commit object");
		}
		ମୁକ୍ତ(buf);

		ଯଦି ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_MODE(SHORT)));
		ଭାଙ୍ଗ;
	ସ୍ଥିତି OBJ_ବୃକ୍ଷ:
		strbuf_addstr(sb, "tree object");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି OBJ_TAG:
		strbuf_addstr(sb, "other tag object");
		ଭାଙ୍ଗ;
	}
	strbuf_addch(sb, ')');
}

ସଂରଚନା ସନ୍ଦେଶ_arg {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା given;
	ସଂରଚନା strbuf buf;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_ସନ୍ଦେଶ_arg(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା ସନ୍ଦେଶ_arg *ସନ୍ଦେଶ = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	ଯଦି (!arg)
		ଫେରନ୍ତୁ -1;
	ଯଦି (ସନ୍ଦେଶ->buf.len)
		strbuf_addstr(&(ସନ୍ଦେଶ->buf), "\n\n");
	strbuf_addstr(&(ସନ୍ଦେଶ->buf), arg);
	ସନ୍ଦେଶ->given = 1;
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_tag(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	    ଅଚଳ ଅକ୍ଷର **argv,
	    ଅଚଳ ଅକ୍ଷର *prefix,
	    ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା strbuf ref = STRBUF_INIT;
	ସଂରଚନା strbuf reflog_ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା object_id object, prev;
	ଅଚଳ ଅକ୍ଷର *object_ref, *tag;
	ସଂରଚନା create_tag_options opt;
	ଅକ୍ଷର *cleanup_arg = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା create_reflog = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା annotate = 0, force = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmdmode = 0, create_tag_object = 0;
	ଅକ୍ଷର *msgfile = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *keyid = ଖାଲି;
	ସଂରଚନା ସନ୍ଦେଶ_arg ସନ୍ଦେଶ = { .buf = STRBUF_INIT };
	ସଂରଚନା ref_transaction *transaction;
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ସଂରଚନା ref_filter filter = REF_FILTER_INIT;
	ସଂରଚନା ref_sorting *sorting;
	ସଂରଚନା string_list sorting_options = STRING_LIST_INIT_DUP;
	ସଂରଚନା ref_format format = REF_FORMAT_INIT;
	ସଂରଚନା strvec trailer_args = STRVEC_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା icase = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା edit_flag = 0;
	ସଂରଚନା option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.ମୂଲ୍ୟ = &filter.ଧାଡ଼ିଗୁଡ଼ିକ,
			.precision = ଆକାର(filter.ଧାଡ଼ିଗୁଡ଼ିକ),
			.argh = N_("n"),
			.help = N_("print <n> lines of each tag message"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete tags"), 'd'),
		OPT_CMDMODE('v', "verify", &cmdmode, N_("verify tags"), 'v'),

		OPT_GROUP(N_("Tag creation options")),
		OPT_BOOL('a', "annotate", &annotate,
					N_("annotated tag, needs a message")),
		OPT_CALLBACK_F('m', "message", &ସନ୍ଦେଶ, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_ସନ୍ଦେଶ_arg),
		OPT_FILENAME('F', "file", &msgfile, N_("read message from file")),
		OPT_STRVEC(0, "trailer", &trailer_args, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of tag message")),
		OPT_BOOL('s', "sign", &opt.sign, N_("annotated and GPG-signed tag")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_STRING('u', "local-user", &keyid, N_("key-id"),
					N_("use another key to sign the tag")),
		OPT__FORCE(&force, N_("replace the tag if exists"), 0),
		OPT_BOOL(0, "create-reflog", &create_reflog, N_("create a reflog")),

		OPT_GROUP(N_("Tag listing options")),
		OPT_COLUMN(0, "column", &colopts, N_("show tag list in columns")),
		OPT_CONTAINS(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_NO_CONTAINS(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_MERGED(&filter, N_("print only tags that are merged")),
		OPT_NO_MERGED(&filter, N_("print only tags that are not merged")),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),
		OPT_REF_SORT(&sorting_options),
		{
			.type = OPTION_CALLBACK,
			.long_name = "points-at",
			.ମୂଲ୍ୟ = &filter.points_at,
			.argh = N_("object"),
			.help = N_("print only tags of the object"),
			.flags = PARSE_OPT_LASTARG_DEFAULT,
			.callback = parse_opt_object_name,
			.defval = (intptr_t) "HEAD",
		},
		OPT_STRING(  0 , "format", &format.format, N_("format"),
			   N_("format to use for the output")),
		OPT__COLOR(&format.use_color, N_("respect format colors")),
		OPT_BOOL('i', "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_END()
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ଅଚଳ ଅକ୍ଷର *only_in_list = ଖାଲି;
	ଅକ୍ଷର *path = ଖାଲି;

	setup_ref_filter_porcelain_ସନ୍ଦେଶ();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_tag_config, &sorting_options);
	ଯଦି (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	memset(&opt, 0, ଆକାର(opt));
	filter.ଧାଡ଼ିଗୁଡ଼ିକ = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, prefix, options, git_tag_usage, 0);

	ଯଦି (!cmdmode) {
		ଯଦି (argc == 0)
			cmdmode = 'l';
		ଅନ୍ୟଥା ଯଦି (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.ଧାଡ଼ିଗୁଡ଼ିକ != -1)
			cmdmode = 'l';
	}

	ଯଦି (cmdmode == 'l')
		setup_auto_pager("tag", 1);

	ଯଦି (trailer_args.nr)
		trailer_config_init();

	ଯଦି (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_tag > 0;

	ଯଦି (keyid) {
		opt.sign = 1;
		set_signing_ଚାବି(keyid);
	}
	create_tag_object = (opt.sign || annotate || ସନ୍ଦେଶ.given || msgfile ||
			     edit_flag || trailer_args.nr);

	ଯଦି ((create_tag_object || force) && (cmdmode != 0))
		usage_with_options(git_tag_usage, options);

	finalize_colopts(&colopts, -1);
	ଯଦି (cmdmode == 'l' && filter.ଧାଡ଼ିଗୁଡ଼ିକ != -1) {
		ଯଦି (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;
	ଯଦି (cmdmode == 'l') {
		ଯଦି (column_active(colopts)) {
			ସଂରଚନା column_options copts;
			memset(&copts, 0, ଆକାର(copts));
			copts.padding = 2;
			ଯଦି (run_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_tags(&filter, sorting, &format);
		ଯଦି (column_active(colopts))
			stop_column_filter();
		ଯାଅ cleanup;
	}
	ଯଦି (filter.ଧାଡ଼ିଗୁଡ଼ିକ != -1)
		only_in_list = "-n";
	ଅନ୍ୟଥା ଯଦି (filter.with_commit)
		only_in_list = "--contains";
	ଅନ୍ୟଥା ଯଦି (filter.no_commit)
		only_in_list = "--no-contains";
	ଅନ୍ୟଥା ଯଦି (filter.points_at.nr)
		only_in_list = "--points-at";
	ଅନ୍ୟଥା ଯଦି (filter.reachable_from)
		only_in_list = "--merged";
	ଅନ୍ୟଥା ଯଦି (filter.unreachable_from)
		only_in_list = "--no-merged";
	ଯଦି (only_in_list)
		die(_("the '%s' option is only allowed in list mode"), only_in_list);
	ଯଦି (cmdmode == 'd') {
		ret = delete_tags(argv);
		ଯାଅ cleanup;
	}
	ଯଦି (cmdmode == 'v') {
		ଯଦି (format.format && verify_ref_format(&format))
			usage_with_options(git_tag_usage, options);
		ret = for_each_tag_name(argv, verify_tag, &format);
		ଯାଅ cleanup;
	}

	ଯଦି (ସନ୍ଦେଶ.given || msgfile) {
		ଯଦି (ସନ୍ଦେଶ.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		ଯଦି (ସନ୍ଦେଶ.given)
			strbuf_addbuf(&buf, &(ସନ୍ଦେଶ.buf));
		ଅନ୍ୟଥା {
			ଯଦି (!strcmp(msgfile, "-")) {
				ଯଦି (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} ଅନ୍ୟଥା {
				ଯଦି (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	tag = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	ଯଦି (argc > 2)
		die(_("too many arguments"));

	ଯଦି (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	ଯଦି (check_tag_ref(&ref, tag))
		die(_("'%s' is not a valid tag name."), tag);

	ଯଦି (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	ଅନ୍ୟଥା ଯଦି (!force)
		die(_("tag '%s' already exists"), tag);

	opt.message_given = ସନ୍ଦେଶ.given || msgfile;
	opt.use_editor = edit_flag;

	ଯଦି (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_mode = CLEANUP_ALL;
	ଅନ୍ୟଥା ଯଦି (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_mode = CLEANUP_NONE;
	ଅନ୍ୟଥା ଯଦି (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_mode = CLEANUP_SPACE;
	ଅନ୍ୟଥା
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_ସନ୍ଦେଶ(&object, &reflog_ସନ୍ଦେଶ);

	ଯଦି (create_tag_object) {
		ଯଦି (force_sign_annotate && !annotate)
			opt.sign = 1;
		path = repo_git_path(the_repository, "TAG_EDITMSG");
		create_tag(&object, object_ref, tag, &buf, &opt, &prev, &object,
			   &trailer_args, path);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ଯଦି (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   ଖାଲି, ଖାଲି,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_ସନ୍ଦେଶ.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ଯଦି (path)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				path);
		die("%s", err.buf);
	}
	ଯଦି (path) {
		unlink_or_warn(path);
		ମୁକ୍ତ(path);
	}
	ref_transaction_free(transaction);
	ଯଦି (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		printf(_("Updated tag '%s' (was %s)\n"), tag,
		       repo_find_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_sorting_release(sorting);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_ସନ୍ଦେଶ);
	strbuf_release(&ସନ୍ଦେଶ.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_args);
	ମୁକ୍ତ(msgfile);
	ଫେରନ୍ତୁ ret;
}
