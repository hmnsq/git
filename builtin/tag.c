/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "gpg-interface.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "column.h"
#अन्तर्भाव "ref-filter.h"
#अन्तर्भाव "date.h"
#अन्तर्भाव "write-or-die.h"
#अन्तर्भाव "object-file-convert.h"
#अन्तर्भाव "trailer.h"

स्थिर ध्रुव वर्ण * ध्रुव git_चिह्नकम्_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	रिक्त
};

स्थिर अचिह्नित पूर्णाङ्क colopts;
स्थिर पूर्णाङ्क force_sign_annotate;
स्थिर पूर्णाङ्क config_sign_चिह्नकम् = -1; /* unspecified */

स्थिर पूर्णाङ्क list_चिह्नकानि(संरचना ref_filter *filter, संरचना ref_क्रमणम् *क्रमणम्,
		     संरचना ref_format *format)
{
	वर्ण *गन्तव्यम्_free = रिक्त;

	यदि (filter->पङ्क्तयः == -1)
		filter->पङ्क्तयः = 0;

	यदि (!format->format) {
		यदि (filter->पङ्क्तयः) {
			गन्तव्यम्_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->पङ्क्तयः);
			format->format = गन्तव्यम्_free;
		} अन्यथा
			format->format = "%(refname:lstrip=2)";
	}

	यदि (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_चिह्नकम्_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_चिह्नकानि, क्रमणम्, format);

	मुक्त(गन्तव्यम्_free);

	प्रत्यागम 0;
}

प्रकारनिर्देश पूर्णाङ्क (*each_चिह्नकम्_name_fn)(ध्रुव वर्ण *name, ध्रुव वर्ण *ref,
				ध्रुव संरचना object_id *oid, शून्य *cb_दत्त);

स्थिर पूर्णाङ्क for_each_चिह्नकम्_name(ध्रुव वर्ण **argv, each_चिह्नकम्_name_fn fn,
			     शून्य *cb_दत्त)
{
	ध्रुव वर्ण **p;
	संरचना strbuf ref = STRBUF_INIT;
	पूर्णाङ्क had_error = 0;
	संरचना object_id oid;

	प्रति (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		यदि (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			अनुवर्त;
		}
		यदि (fn(*p, ref.buf, &oid, cb_दत्त))
			had_error = 1;
	}
	strbuf_release(&ref);
	प्रत्यागम had_error;
}

स्थिर पूर्णाङ्क collect_चिह्नकानि(ध्रुव वर्ण *name अप्रयुक्तम्, ध्रुव वर्ण *ref,
			ध्रुव संरचना object_id *oid, शून्य *cb_दत्त)
{
	संरचना string_list *ref_list = cb_दत्त;

	string_list_append(ref_list, ref);
	ref_list->वस्तूनि[ref_list->nr - 1].util = oiddup(oid);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क delete_चिह्नकानि(ध्रुव वर्ण **argv)
{
	पूर्णाङ्क परिणाम;
	संरचना string_list refs_गन्तव्यम्_delete = STRING_LIST_INIT_DUP;
	संरचना string_list_item *item;

	परिणाम = for_each_चिह्नकम्_name(argv, collect_चिह्नकानि, (शून्य *)&refs_गन्तव्यम्_delete);
	यदि (refs_delete_refs(get_main_ref_store(the_repository), रिक्त, &refs_गन्तव्यम्_delete, REF_NO_DEREF))
		परिणाम = 1;

	for_each_string_list_item(item, &refs_गन्तव्यम्_delete) {
		ध्रुव वर्ण *name = item->string;
		संरचना object_id *oid = item->util;
		यदि (!refs_ref_exists(get_main_ref_store(the_repository), name))
			प्रारूप_लिख(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		मुक्त(oid);
	}
	string_list_clear(&refs_गन्तव्यम्_delete, 0);
	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क verify_चिह्नकम्(ध्रुव वर्ण *name, ध्रुव वर्ण *ref अप्रयुक्तम्,
		      ध्रुव संरचना object_id *oid, शून्य *cb_दत्त)
{
	पूर्णाङ्क flags;
	संरचना ref_format *format = cb_दत्त;
	flags = GPG_VERIFY_VERBOSE;

	यदि (format->format)
		flags = GPG_VERIFY_OMIT_स्थितिः;

	यदि (gpg_verify_चिह्नकम्(the_repository, oid, name, flags))
		प्रत्यागम -1;

	यदि (format->format)
		pretty_print_ref(name, oid, रिक्त, format);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क do_sign(संरचना strbuf *बफर, संरचना object_id **compat_oid,
		   संरचना object_id *compat_oid_buf)
{
	ध्रुव संरचना git_hash_algo *compat = the_repository->compat_hash_algo;
	संरचना strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	संरचना strbuf compat_buf = STRBUF_INIT;
	वर्ण *keyid = get_signing_कुञ्चिका();
	पूर्णाङ्क ret = -1;

	यदि (sign_बफर(बफर, &sig, keyid, 0))
		गच्छ बहिर्गमनम्;

	यदि (compat) {
		ध्रुव संरचना git_hash_algo *algo = the_repository->hash_algo;

		यदि (convert_object_file(the_repository ,&compat_buf, algo, compat,
					बफर->buf, बफर->len, OBJ_चिह्नकम्, 1))
			गच्छ बहिर्गमनम्;
		यदि (sign_बफर(&compat_buf, &compat_sig, keyid, 0))
			गच्छ बहिर्गमनम्;
		सङ्कलय_शीर्षकम्_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_चिह्नकम्, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	यदि (compat_sig.len)
		सङ्कलय_शीर्षकम्_signature(बफर, &compat_sig, compat);

	strbuf_addbuf(बफर, &sig);
	ret = 0;
बहिर्गमनम्:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	मुक्त(keyid);
	प्रत्यागम ret;
}

स्थिर ध्रुव वर्ण चिह्नकम्_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

स्थिर ध्रुव वर्ण चिह्नकम्_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

स्थिर पूर्णाङ्क git_चिह्नकम्_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			  ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (!वाक्यं_तुलय(var, "tag.gpgsign")) {
		config_sign_चिह्नकम् = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(var, "tag.sort")) {
		यदि (!मानम्)
			प्रत्यागम config_error_nonbool(var);
		string_list_append(cb, मानम्);
		प्रत्यागम 0;
	}

	यदि (!वाक्यं_तुलय(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, मानम्);
		प्रत्यागम 0;
	}

	यदि (starts_with(var, "column."))
		प्रत्यागम git_column_config(var, मानम्, "tag", &colopts);

	यदि (git_color_config(var, मानम्, cb) < 0)
		प्रत्यागम -1;

	प्रत्यागम git_default_config(var, मानम्, ctx, cb);
}

स्थिर शून्य write_चिह्नकम्_body(पूर्णाङ्क fd, ध्रुव संरचना object_id *oid)
{
	आकार_t आकार;
	गणना object_type type;
	वर्ण *buf, *sp, *orig;
	संरचना strbuf payload = STRBUF_INIT;
	संरचना strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &आकार);
	यदि (!buf)
		प्रत्यागम;
	यदि (parse_signature(buf, आकार, &payload, &signature)) {
		buf = payload.buf;
		आकार = payload.len;
	}
	/* skip header */
	sp = वाक्ये_अन्विष(buf, "\n\n");

	यदि (!sp || !आकार || type != OBJ_चिह्नकम्) {
		मुक्त(buf);
		प्रत्यागम;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + आकार - sp);

	मुक्त(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

स्थिर पूर्णाङ्क build_चिह्नकम्_object(संरचना strbuf *buf, पूर्णाङ्क sign, संरचना object_id *परिणाम)
{
	संरचना object_id *compat_oid = रिक्त, compat_oid_buf;
	यदि (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		प्रत्यागम error(_("unable to sign the tag"));
	यदि (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_चिह्नकम्, परिणाम, compat_oid, 0) < 0)
		प्रत्यागम error(_("unable to write tag file"));
	प्रत्यागम 0;
}

संरचना create_चिह्नकम्_options {
	अचिह्नित पूर्णाङ्क सन्देशः_given:1;
	अचिह्नित पूर्णाङ्क use_editor:1;
	अचिह्नित पूर्णाङ्क sign;
	गणना {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_बहुलकम्;
};

स्थिर ध्रुव वर्ण सन्देशः_advice_nested_चिह्नकम्[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

स्थिर शून्य create_चिह्नकम्(ध्रुव संरचना object_id *object, ध्रुव वर्ण *object_ref,
		       ध्रुव वर्ण *चिह्नकम्,
		       संरचना strbuf *buf, संरचना create_चिह्नकम्_options *opt,
		       संरचना object_id *पूर्व, संरचना object_id *परिणाम,
		       संरचना strvec *trailer_तर्काः, वर्ण *मार्गः)
{
	गणना object_type type;
	संरचना strbuf शीर्षकम् = STRBUF_INIT;
	पूर्णाङ्क should_edit;

	type = odb_read_object_info(the_repository->objects, object, रिक्त);
	यदि (type <= OBJ_NONE)
		die(_("bad object type."));

	यदि (type == OBJ_चिह्नकम्)
		advise_if_enabled(ADVICE_NESTED_चिह्नकम्, _(सन्देशः_advice_nested_चिह्नकम्),
				  चिह्नकम्, object_ref);

	strbuf_addf(&शीर्षकम्,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n"
		    "tagger %s\n\n",
		    oid_गन्तव्यम्_hex(object),
		    type_name(type),
		    चिह्नकम्,
		    git_committer_info(IDENT_STRICT));

	should_edit = opt->use_editor || !opt->सन्देशः_given;
	यदि (should_edit || trailer_तर्काः->nr) {
		पूर्णाङ्क fd;

		/* write the template message before editing: */
		fd = xopen(मार्गः, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		यदि (opt->सन्देशः_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} अन्यथा यदि (!is_null_oid(पूर्व)) {
			write_चिह्नकम्_body(fd, पूर्व);
		} अन्यथा {
			संरचना strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			यदि (opt->cleanup_बहुलकम् == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_पङ्क्तिः_str,
				      _(चिह्नकम्_template), चिह्नकम्, comment_पङ्क्तिः_str);
			अन्यथा
				strbuf_commented_addf(&buf, comment_पङ्क्तिः_str,
				      _(चिह्नकम्_template_nocleanup), चिह्नकम्, comment_पङ्क्तिः_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		यदि (trailer_तर्काः->nr && amend_file_with_trailers(मार्गः, trailer_तर्काः))
			die(_("unable to pass trailers to --trailers"));

		यदि (should_edit) {
			यदि (launch_editor(मार्गः, buf, रिक्त)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				निर्गच्छ(1);
			}
		} अन्यथा यदि (trailer_तर्काः->nr) {
			strbuf_reset(buf);
			यदि (strbuf_read_file(buf, मार्गः, 0) < 0)
				die_errno(_("failed to read '%s'"), मार्गः);
		}
	}

	यदि (opt->cleanup_बहुलकम् != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_बहुलकम् == CLEANUP_ALL ? comment_पङ्क्तिः_str : रिक्त);

	यदि (!opt->सन्देशः_given && !buf->len)
		die(_("no tag message?"));

	strbuf_निवेशय(buf, 0, शीर्षकम्.buf, शीर्षकम्.len);
	strbuf_release(&शीर्षकम्);

	यदि (build_चिह्नकम्_object(buf, opt->sign, परिणाम) < 0) {
		यदि (मार्गः)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				मार्गः);
		निर्गच्छ(128);
	}
}

स्थिर शून्य create_reflog_सन्देशः(ध्रुव संरचना object_id *oid, संरचना strbuf *sb)
{
	गणना object_type type;
	संरचना commit *c;
	वर्ण *buf;
	आकार_t आकार;
	पूर्णाङ्क subject_len = 0;
	ध्रुव वर्ण *subject_start;

	वर्ण *rla = getenv("GIT_REFLOG_ACTION");
	यदि (rla) {
		strbuf_addstr(sb, rla);
	} अन्यथा {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_सङ्कलय_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, रिक्त);
	विकल्प (type) {
	स्वभाव:
		strbuf_addstr(sb, "object of unknown type");
		भङ्ग;
	पक्ष OBJ_COMMIT:
		यदि ((buf = odb_read_object(the_repository->objects, oid, &type, &आकार))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_निवेशय(sb, sb->len, subject_start, subject_len);
		} अन्यथा {
			strbuf_addstr(sb, "commit object");
		}
		मुक्त(buf);

		यदि ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_बहुलकम्(SHORT)));
		भङ्ग;
	पक्ष OBJ_वृक्षः:
		strbuf_addstr(sb, "tree object");
		भङ्ग;
	पक्ष OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		भङ्ग;
	पक्ष OBJ_चिह्नकम्:
		strbuf_addstr(sb, "other tag object");
		भङ्ग;
	}
	strbuf_addch(sb, ')');
}

संरचना सन्देशः_arg {
	पूर्णाङ्क given;
	संरचना strbuf buf;
};

स्थिर पूर्णाङ्क parse_सन्देशः_arg(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना सन्देशः_arg *सन्देशः = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	यदि (!arg)
		प्रत्यागम -1;
	यदि (सन्देशः->buf.len)
		strbuf_addstr(&(सन्देशः->buf), "\n\n");
	strbuf_addstr(&(सन्देशः->buf), arg);
	सन्देशः->given = 1;
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_चिह्नकम्(पूर्णाङ्क argc,
	    ध्रुव वर्ण **argv,
	    ध्रुव वर्ण *उपसर्गः,
	    संरचना repository *repo अप्रयुक्तम्)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf ref = STRBUF_INIT;
	संरचना strbuf reflog_सन्देशः = STRBUF_INIT;
	संरचना object_id object, पूर्व;
	ध्रुव वर्ण *object_ref, *चिह्नकम्;
	संरचना create_चिह्नकम्_options opt;
	वर्ण *cleanup_arg = रिक्त;
	पूर्णाङ्क create_reflog = 0;
	पूर्णाङ्क annotate = 0, force = 0;
	पूर्णाङ्क cmdmode = 0, create_चिह्नकम्_object = 0;
	वर्ण *msgfile = रिक्त;
	ध्रुव वर्ण *keyid = रिक्त;
	संरचना सन्देशः_arg सन्देशः = { .buf = STRBUF_INIT };
	संरचना ref_transaction *transaction;
	संरचना strbuf err = STRBUF_INIT;
	संरचना ref_filter filter = REF_FILTER_INIT;
	संरचना ref_क्रमणम् *क्रमणम्;
	संरचना string_list क्रमणम्_options = STRING_LIST_INIT_DUP;
	संरचना ref_format format = REF_FORMAT_INIT;
	संरचना strvec trailer_तर्काः = STRVEC_INIT;
	पूर्णाङ्क icase = 0;
	पूर्णाङ्क edit_flag = 0;
	संरचना option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.मानम् = &filter.पङ्क्तयः,
			.precision = परिमाण(filter.पङ्क्तयः),
			.argh = N_("n"),
			.help = N_("print <n> lines of each tag message"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete tags"), 'd'),
		OPT_CMDMODE('v', "verify", &cmdmode, N_("verify tags"), 'v'),

		OPT_गणः(N_("Tag creation options")),
		OPT_BOOL('a', "annotate", &annotate,
					N_("annotated tag, needs a message")),
		OPT_CALLBACK_F('m', "message", &सन्देशः, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_सन्देशः_arg),
		OPT_FILENAME('F', "file", &msgfile, N_("read message from file")),
		OPT_STRVEC(0, "trailer", &trailer_तर्काः, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of tag message")),
		OPT_BOOL('s', "sign", &opt.sign, N_("annotated and GPG-signed tag")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_STRING('u', "local-user", &keyid, N_("key-id"),
					N_("use another key to sign the tag")),
		OPT__FORCE(&force, N_("replace the tag if exists"), 0),
		OPT_BOOL(0, "create-reflog", &create_reflog, N_("create a reflog")),

		OPT_गणः(N_("Tag listing options")),
		OPT_COLUMN(0, "column", &colopts, N_("show tag list in columns")),
		OPT_अन्तर्भवति(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_NO_अन्तर्भवति(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_MERGED(&filter, N_("print only tags that are merged")),
		OPT_NO_MERGED(&filter, N_("print only tags that are not merged")),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_रिक्तानि,
			N_("do not output a newline after empty formatted refs")),
		OPT_REF_क्रमय(&क्रमणम्_options),
		{
			.type = OPTION_CALLBACK,
			.long_name = "points-at",
			.मानम् = &filter.points_at,
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
		OPT_अन्तः()
	};
	पूर्णाङ्क ret = 0;
	ध्रुव वर्ण *only_अन्तर्_list = रिक्त;
	वर्ण *मार्गः = रिक्त;

	सज्जता_ref_filter_porcelain_सन्देशः();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_चिह्नकम्_config, &क्रमणम्_options);
	यदि (!क्रमणम्_options.nr)
		string_list_append(&क्रमणम्_options, "refname");

	स्मृतिं_पूरय(&opt, 0, परिमाण(opt));
	filter.पङ्क्तयः = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, उपसर्गः, options, git_चिह्नकम्_usage, 0);

	यदि (!cmdmode) {
		यदि (argc == 0)
			cmdmode = 'l';
		अन्यथा यदि (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.पङ्क्तयः != -1)
			cmdmode = 'l';
	}

	यदि (cmdmode == 'l')
		सज्जता_auto_pager("tag", 1);

	यदि (trailer_तर्काः.nr)
		trailer_config_init();

	यदि (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_चिह्नकम् > 0;

	यदि (keyid) {
		opt.sign = 1;
		समुच्चयः_signing_कुञ्चिका(keyid);
	}
	create_चिह्नकम्_object = (opt.sign || annotate || सन्देशः.given || msgfile ||
			     edit_flag || trailer_तर्काः.nr);

	यदि ((create_चिह्नकम्_object || force) && (cmdmode != 0))
		usage_with_options(git_चिह्नकम्_usage, options);

	finalize_colopts(&colopts, -1);
	यदि (cmdmode == 'l' && filter.पङ्क्तयः != -1) {
		यदि (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	क्रमणम् = ref_क्रमणम्_options(&क्रमणम्_options);
	ref_क्रमणम्_समुच्चयः_क्रमय_flags_all(क्रमणम्, REF_क्रमणम्_ICASE, icase);
	filter.ignore_case = icase;
	यदि (cmdmode == 'l') {
		यदि (column_सक्रिय(colopts)) {
			संरचना column_options copts;
			स्मृतिं_पूरय(&copts, 0, परिमाण(copts));
			copts.padding = 2;
			यदि (सततम्_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_चिह्नकानि(&filter, क्रमणम्, &format);
		यदि (column_सक्रिय(colopts))
			stop_column_filter();
		गच्छ cleanup;
	}
	यदि (filter.पङ्क्तयः != -1)
		only_अन्तर्_list = "-n";
	अन्यथा यदि (filter.with_commit)
		only_अन्तर्_list = "--contains";
	अन्यथा यदि (filter.no_commit)
		only_अन्तर्_list = "--no-contains";
	अन्यथा यदि (filter.points_at.nr)
		only_अन्तर्_list = "--points-at";
	अन्यथा यदि (filter.reachable_from)
		only_अन्तर्_list = "--merged";
	अन्यथा यदि (filter.unreachable_from)
		only_अन्तर्_list = "--no-merged";
	यदि (only_अन्तर्_list)
		die(_("the '%s' option is only allowed in list mode"), only_अन्तर्_list);
	यदि (cmdmode == 'd') {
		ret = delete_चिह्नकानि(argv);
		गच्छ cleanup;
	}
	यदि (cmdmode == 'v') {
		यदि (format.format && verify_ref_format(&format))
			usage_with_options(git_चिह्नकम्_usage, options);
		ret = for_each_चिह्नकम्_name(argv, verify_चिह्नकम्, &format);
		गच्छ cleanup;
	}

	यदि (सन्देशः.given || msgfile) {
		यदि (सन्देशः.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		यदि (सन्देशः.given)
			strbuf_addbuf(&buf, &(सन्देशः.buf));
		अन्यथा {
			यदि (!वाक्यं_तुलय(msgfile, "-")) {
				यदि (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} अन्यथा {
				यदि (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	चिह्नकम् = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	यदि (argc > 2)
		die(_("too many arguments"));

	यदि (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	यदि (check_चिह्नकम्_ref(&ref, चिह्नकम्))
		die(_("'%s' is not a valid tag name."), चिह्नकम्);

	यदि (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &पूर्व))
		oidclr(&पूर्व, the_repository->hash_algo);
	अन्यथा यदि (!force)
		die(_("tag '%s' already exists"), चिह्नकम्);

	opt.सन्देशः_given = सन्देशः.given || msgfile;
	opt.use_editor = edit_flag;

	यदि (!cleanup_arg || !वाक्यं_तुलय(cleanup_arg, "strip"))
		opt.cleanup_बहुलकम् = CLEANUP_ALL;
	अन्यथा यदि (!वाक्यं_तुलय(cleanup_arg, "verbatim"))
		opt.cleanup_बहुलकम् = CLEANUP_NONE;
	अन्यथा यदि (!वाक्यं_तुलय(cleanup_arg, "whitespace"))
		opt.cleanup_बहुलकम् = CLEANUP_SPACE;
	अन्यथा
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_सन्देशः(&object, &reflog_सन्देशः);

	यदि (create_चिह्नकम्_object) {
		यदि (force_sign_annotate && !annotate)
			opt.sign = 1;
		मार्गः = repo_git_मार्गः(the_repository, "TAG_EDITMSG");
		create_चिह्नकम्(&object, object_ref, चिह्नकम्, &buf, &opt, &पूर्व, &object,
			   &trailer_तर्काः, मार्गः);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	यदि (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &पूर्व,
				   रिक्त, रिक्त,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_सन्देशः.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		यदि (मार्गः)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				मार्गः);
		die("%s", err.buf);
	}
	यदि (मार्गः) {
		unlink_or_warn(मार्गः);
		मुक्त(मार्गः);
	}
	ref_transaction_free(transaction);
	यदि (force && !is_null_oid(&पूर्व) && !oideq(&पूर्व, &object))
		प्रारूप_लिख(_("Updated tag '%s' (was %s)\n"), चिह्नकम्,
		       repo_find_unique_abbrev(the_repository, &पूर्व, DEFAULT_ABBREV));

cleanup:
	ref_क्रमणम्_release(क्रमणम्);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_सन्देशः);
	strbuf_release(&सन्देशः.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_तर्काः);
	मुक्त(msgfile);
	प्रत्यागम ret;
}
