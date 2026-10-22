/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "editor.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "gpg-interface.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "column.h"
#అన్తర్భావ "ref-filter.h"
#అన్తర్భావ "date.h"
#అన్తర్భావ "write-or-die.h"
#అన్తర్భావ "object-file-convert.h"
#అన్తర్భావ "trailer.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_ట్యాగ్_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	ఖాళీ
};

స్థిర గుర్తులేని పూర్ణాంకం colopts;
స్థిర పూర్ణాంకం force_sign_annotate;
స్థిర పూర్ణాంకం config_sign_ట్యాగ్ = -1; /* unspecified */

స్థిర పూర్ణాంకం list_ట్యాగులు(నిర్మాణం ref_filter *filter, నిర్మాణం ref_క్రమీకరణ *క్రమీకరణ,
		     నిర్మాణం ref_format *format)
{
	అక్షరం *గమ్యం_free = ఖాళీ;

	ఒకవేళ (filter->పంక్తులు == -1)
		filter->పంక్తులు = 0;

	ఒకవేళ (!format->format) {
		ఒకవేళ (filter->పంక్తులు) {
			గమ్యం_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->పంక్తులు);
			format->format = గమ్యం_free;
		} లేకపోతే
			format->format = "%(refname:lstrip=2)";
	}

	ఒకవేళ (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_ట్యాగ్_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_ట్యాగులు, క్రమీకరణ, format);

	విడుదల(గమ్యం_free);

	తిరిగి 0;
}

రకనిర్వచనం పూర్ణాంకం (*each_ట్యాగ్_name_fn)(స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *ref,
				స్థిరరాశి నిర్మాణం object_id *oid, శూన్య *cb_దత్తాంశం);

స్థిర పూర్ణాంకం for_each_ట్యాగ్_name(స్థిరరాశి అక్షరం **argv, each_ట్యాగ్_name_fn fn,
			     శూన్య *cb_దత్తాంశం)
{
	స్థిరరాశి అక్షరం **p;
	నిర్మాణం strbuf ref = STRBUF_INIT;
	పూర్ణాంకం had_error = 0;
	నిర్మాణం object_id oid;

	కొరకు (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		ఒకవేళ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			కొనసాగు;
		}
		ఒకవేళ (fn(*p, ref.buf, &oid, cb_దత్తాంశం))
			had_error = 1;
	}
	strbuf_release(&ref);
	తిరిగి had_error;
}

స్థిర పూర్ణాంకం collect_ట్యాగులు(స్థిరరాశి అక్షరం *name వాడని, స్థిరరాశి అక్షరం *ref,
			స్థిరరాశి నిర్మాణం object_id *oid, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం string_list *ref_list = cb_దత్తాంశం;

	string_list_append(ref_list, ref);
	ref_list->వస్తువులు[ref_list->nr - 1].util = oiddup(oid);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం delete_ట్యాగులు(స్థిరరాశి అక్షరం **argv)
{
	పూర్ణాంకం ఫలితం;
	నిర్మాణం string_list refs_గమ్యం_delete = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list_అంశం *అంశం;

	ఫలితం = for_each_ట్యాగ్_name(argv, collect_ట్యాగులు, (శూన్య *)&refs_గమ్యం_delete);
	ఒకవేళ (refs_delete_refs(get_main_ref_store(the_repository), ఖాళీ, &refs_గమ్యం_delete, REF_NO_DEREF))
		ఫలితం = 1;

	for_each_string_list_అంశం(అంశం, &refs_గమ్యం_delete) {
		స్థిరరాశి అక్షరం *name = అంశం->string;
		నిర్మాణం object_id *oid = అంశం->util;
		ఒకవేళ (!refs_ref_exists(get_main_ref_store(the_repository), name))
			రాయి(_("Deleted tag '%s' (was %s)\n"),
				అంశం->string + 10,
				repo_కనుగొను_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		విడుదల(oid);
	}
	string_list_clear(&refs_గమ్యం_delete, 0);
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం verify_ట్యాగ్(స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *ref వాడని,
		      స్థిరరాశి నిర్మాణం object_id *oid, శూన్య *cb_దత్తాంశం)
{
	పూర్ణాంకం flags;
	నిర్మాణం ref_format *format = cb_దత్తాంశం;
	flags = GPG_VERIFY_VERBOSE;

	ఒకవేళ (format->format)
		flags = GPG_VERIFY_OMIT_స్థితి;

	ఒకవేళ (gpg_verify_ట్యాగ్(the_repository, oid, name, flags))
		తిరిగి -1;

	ఒకవేళ (format->format)
		pretty_print_ref(name, oid, ఖాళీ, format);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం do_sign(నిర్మాణం strbuf *బఫరు, నిర్మాణం object_id **compat_oid,
		   నిర్మాణం object_id *compat_oid_buf)
{
	స్థిరరాశి నిర్మాణం git_hash_algo *compat = the_repository->compat_hash_algo;
	నిర్మాణం strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	నిర్మాణం strbuf compat_buf = STRBUF_INIT;
	అక్షరం *keyid = get_signing_కీలకం();
	పూర్ణాంకం ret = -1;

	ఒకవేళ (sign_బఫరు(బఫరు, &sig, keyid, 0))
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (compat) {
		స్థిరరాశి నిర్మాణం git_hash_algo *algo = the_repository->hash_algo;

		ఒకవేళ (convert_object_file(the_repository ,&compat_buf, algo, compat,
					బఫరు->buf, బఫరు->len, OBJ_ట్యాగ్, 1))
			వెళ్ళు బహిర్గతం;
		ఒకవేళ (sign_బఫరు(&compat_buf, &compat_sig, keyid, 0))
			వెళ్ళు బహిర్గతం;
		కూడు_శీర్షిక_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_ట్యాగ్, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	ఒకవేళ (compat_sig.len)
		కూడు_శీర్షిక_signature(బఫరు, &compat_sig, compat);

	strbuf_addbuf(బఫరు, &sig);
	ret = 0;
బహిర్గతం:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	విడుదల(keyid);
	తిరిగి ret;
}

స్థిర స్థిరరాశి అక్షరం ట్యాగ్_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

స్థిర స్థిరరాశి అక్షరం ట్యాగ్_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

స్థిర పూర్ణాంకం git_ట్యాగ్_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			  స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(var, "tag.gpgsign")) {
		config_sign_ట్యాగ్ = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "tag.sort")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		string_list_append(cb, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (starts_with(var, "column."))
		తిరిగి git_స్తంభం_config(var, విలువ, "tag", &colopts);

	ఒకవేళ (git_color_config(var, విలువ, cb) < 0)
		తిరిగి -1;

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

స్థిర శూన్య write_ట్యాగ్_body(పూర్ణాంకం fd, స్థిరరాశి నిర్మాణం object_id *oid)
{
	కొలత_t కొలత;
	గణన object_type type;
	అక్షరం *buf, *sp, *orig;
	నిర్మాణం strbuf payload = STRBUF_INIT;
	నిర్మాణం strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &కొలత);
	ఒకవేళ (!buf)
		తిరిగి;
	ఒకవేళ (parse_signature(buf, కొలత, &payload, &signature)) {
		buf = payload.buf;
		కొలత = payload.len;
	}
	/* skip header */
	sp = అక్షరశ్రేణి_శోధన(buf, "\n\n");

	ఒకవేళ (!sp || !కొలత || type != OBJ_ట్యాగ్) {
		విడుదల(buf);
		తిరిగి;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + కొలత - sp);

	విడుదల(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

స్థిర పూర్ణాంకం build_ట్యాగ్_object(నిర్మాణం strbuf *buf, పూర్ణాంకం sign, నిర్మాణం object_id *ఫలితం)
{
	నిర్మాణం object_id *compat_oid = ఖాళీ, compat_oid_buf;
	ఒకవేళ (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		తిరిగి error(_("unable to sign the tag"));
	ఒకవేళ (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_ట్యాగ్, ఫలితం, compat_oid, 0) < 0)
		తిరిగి error(_("unable to write tag file"));
	తిరిగి 0;
}

నిర్మాణం create_ట్యాగ్_options {
	గుర్తులేని పూర్ణాంకం సందేశం_given:1;
	గుర్తులేని పూర్ణాంకం use_editor:1;
	గుర్తులేని పూర్ణాంకం sign;
	గణన {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_బహుళకం;
};

స్థిర స్థిరరాశి అక్షరం సందేశం_advice_nested_ట్యాగ్[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

స్థిర శూన్య create_ట్యాగ్(స్థిరరాశి నిర్మాణం object_id *object, స్థిరరాశి అక్షరం *object_ref,
		       స్థిరరాశి అక్షరం *ట్యాగ్,
		       నిర్మాణం strbuf *buf, నిర్మాణం create_ట్యాగ్_options *opt,
		       నిర్మాణం object_id *prev, నిర్మాణం object_id *ఫలితం,
		       నిర్మాణం strvec *trailer_వాదనలు, అక్షరం *మార్గం)
{
	గణన object_type type;
	నిర్మాణం strbuf శీర్షిక = STRBUF_INIT;
	పూర్ణాంకం should_edit;

	type = odb_read_object_info(the_repository->objects, object, ఖాళీ);
	ఒకవేళ (type <= OBJ_NONE)
		die(_("bad object type."));

	ఒకవేళ (type == OBJ_ట్యాగ్)
		advise_if_enabled(ADVICE_NESTED_ట్యాగ్, _(సందేశం_advice_nested_ట్యాగ్),
				  ట్యాగ్, object_ref);

	strbuf_addf(&శీర్షిక,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n"
		    "tagger %s\n\n",
		    oid_గమ్యం_hex(object),
		    type_name(type),
		    ట్యాగ్,
		    git_committer_info(IDENT_STRICT));

	should_edit = opt->use_editor || !opt->సందేశం_given;
	ఒకవేళ (should_edit || trailer_వాదనలు->nr) {
		పూర్ణాంకం fd;

		/* write the template message before editing: */
		fd = xopen(మార్గం, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		ఒకవేళ (opt->సందేశం_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} లేకపోతే ఒకవేళ (!is_null_oid(prev)) {
			write_ట్యాగ్_body(fd, prev);
		} లేకపోతే {
			నిర్మాణం strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			ఒకవేళ (opt->cleanup_బహుళకం == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_పంక్తి_str,
				      _(ట్యాగ్_template), ట్యాగ్, comment_పంక్తి_str);
			లేకపోతే
				strbuf_commented_addf(&buf, comment_పంక్తి_str,
				      _(ట్యాగ్_template_nocleanup), ట్యాగ్, comment_పంక్తి_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		ఒకవేళ (trailer_వాదనలు->nr && amend_file_with_trailers(మార్గం, trailer_వాదనలు))
			die(_("unable to pass trailers to --trailers"));

		ఒకవేళ (should_edit) {
			ఒకవేళ (launch_editor(మార్గం, buf, ఖాళీ)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				నిష్క్రమించు(1);
			}
		} లేకపోతే ఒకవేళ (trailer_వాదనలు->nr) {
			strbuf_reset(buf);
			ఒకవేళ (strbuf_read_file(buf, మార్గం, 0) < 0)
				die_errno(_("failed to read '%s'"), మార్గం);
		}
	}

	ఒకవేళ (opt->cleanup_బహుళకం != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_బహుళకం == CLEANUP_ALL ? comment_పంక్తి_str : ఖాళీ);

	ఒకవేళ (!opt->సందేశం_given && !buf->len)
		die(_("no tag message?"));

	strbuf_చొప్పించు(buf, 0, శీర్షిక.buf, శీర్షిక.len);
	strbuf_release(&శీర్షిక);

	ఒకవేళ (build_ట్యాగ్_object(buf, opt->sign, ఫలితం) < 0) {
		ఒకవేళ (మార్గం)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				మార్గం);
		నిష్క్రమించు(128);
	}
}

స్థిర శూన్య create_reflog_సందేశం(స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం strbuf *sb)
{
	గణన object_type type;
	నిర్మాణం commit *c;
	అక్షరం *buf;
	కొలత_t కొలత;
	పూర్ణాంకం subject_len = 0;
	స్థిరరాశి అక్షరం *subject_ప్రారంభం;

	అక్షరం *rla = getenv("GIT_REFLOG_ACTION");
	ఒకవేళ (rla) {
		strbuf_addstr(sb, rla);
	} లేకపోతే {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_కూడు_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, ఖాళీ);
	ఎంపిక (type) {
	అప్రమేయ:
		strbuf_addstr(sb, "object of unknown type");
		ఆపు;
	స్థితి OBJ_COMMIT:
		ఒకవేళ ((buf = odb_read_object(the_repository->objects, oid, &type, &కొలత))) {
			subject_len = కనుగొను_commit_subject(buf, &subject_ప్రారంభం);
			strbuf_చొప్పించు(sb, sb->len, subject_ప్రారంభం, subject_len);
		} లేకపోతే {
			strbuf_addstr(sb, "commit object");
		}
		విడుదల(buf);

		ఒకవేళ ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_బహుళకం(SHORT)));
		ఆపు;
	స్థితి OBJ_వృక్షం:
		strbuf_addstr(sb, "tree object");
		ఆపు;
	స్థితి OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		ఆపు;
	స్థితి OBJ_ట్యాగ్:
		strbuf_addstr(sb, "other tag object");
		ఆపు;
	}
	strbuf_addch(sb, ')');
}

నిర్మాణం సందేశం_arg {
	పూర్ణాంకం given;
	నిర్మాణం strbuf buf;
};

స్థిర పూర్ణాంకం parse_సందేశం_arg(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం సందేశం_arg *సందేశం = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (!arg)
		తిరిగి -1;
	ఒకవేళ (సందేశం->buf.len)
		strbuf_addstr(&(సందేశం->buf), "\n\n");
	strbuf_addstr(&(సందేశం->buf), arg);
	సందేశం->given = 1;
	తిరిగి 0;
}

పూర్ణాంకం cmd_ట్యాగ్(పూర్ణాంకం argc,
	    స్థిరరాశి అక్షరం **argv,
	    స్థిరరాశి అక్షరం *ఉపసర్గ,
	    నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం strbuf ref = STRBUF_INIT;
	నిర్మాణం strbuf reflog_సందేశం = STRBUF_INIT;
	నిర్మాణం object_id object, prev;
	స్థిరరాశి అక్షరం *object_ref, *ట్యాగ్;
	నిర్మాణం create_ట్యాగ్_options opt;
	అక్షరం *cleanup_arg = ఖాళీ;
	పూర్ణాంకం create_reflog = 0;
	పూర్ణాంకం annotate = 0, force = 0;
	పూర్ణాంకం cmdmode = 0, create_ట్యాగ్_object = 0;
	అక్షరం *msgfile = ఖాళీ;
	స్థిరరాశి అక్షరం *keyid = ఖాళీ;
	నిర్మాణం సందేశం_arg సందేశం = { .buf = STRBUF_INIT };
	నిర్మాణం ref_transaction *transaction;
	నిర్మాణం strbuf err = STRBUF_INIT;
	నిర్మాణం ref_filter filter = REF_FILTER_INIT;
	నిర్మాణం ref_క్రమీకరణ *క్రమీకరణ;
	నిర్మాణం string_list క్రమీకరణ_options = STRING_LIST_INIT_DUP;
	నిర్మాణం ref_format format = REF_FORMAT_INIT;
	నిర్మాణం strvec trailer_వాదనలు = STRVEC_INIT;
	పూర్ణాంకం icase = 0;
	పూర్ణాంకం edit_flag = 0;
	నిర్మాణం option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.విలువ = &filter.పంక్తులు,
			.precision = పరిమాణం(filter.పంక్తులు),
			.argh = N_("n"),
			.help = N_("print <n> lines of each tag message"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete tags"), 'd'),
		OPT_CMDMODE('v', "verify", &cmdmode, N_("verify tags"), 'v'),

		OPT_సమూహం(N_("Tag creation options")),
		OPT_BOOL('a', "annotate", &annotate,
					N_("annotated tag, needs a message")),
		OPT_CALLBACK_F('m', "message", &సందేశం, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_సందేశం_arg),
		OPT_FILENAME('F', "file", &msgfile, N_("read message from file")),
		OPT_STRVEC(0, "trailer", &trailer_వాదనలు, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of tag message")),
		OPT_BOOL('s', "sign", &opt.sign, N_("annotated and GPG-signed tag")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_STRING('u', "local-user", &keyid, N_("key-id"),
					N_("use another key to sign the tag")),
		OPT__FORCE(&force, N_("replace the tag if exists"), 0),
		OPT_BOOL(0, "create-reflog", &create_reflog, N_("create a reflog")),

		OPT_సమూహం(N_("Tag listing options")),
		OPT_స్తంభం(0, "column", &colopts, N_("show tag list in columns")),
		OPT_కలిగియుంది(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_NO_కలిగియుంది(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_MERGED(&filter, N_("print only tags that are merged")),
		OPT_NO_MERGED(&filter, N_("print only tags that are not merged")),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_ఖాళీలు,
			N_("do not output a newline after empty formatted refs")),
		OPT_REF_క్రమీకరించు(&క్రమీకరణ_options),
		{
			.type = OPTION_CALLBACK,
			.long_name = "points-at",
			.విలువ = &filter.points_at,
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
		OPT_చివర()
	};
	పూర్ణాంకం ret = 0;
	స్థిరరాశి అక్షరం *only_లోపల_list = ఖాళీ;
	అక్షరం *మార్గం = ఖాళీ;

	అమరిక_ref_filter_porcelain_సందేశం();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_ట్యాగ్_config, &క్రమీకరణ_options);
	ఒకవేళ (!క్రమీకరణ_options.nr)
		string_list_append(&క్రమీకరణ_options, "refname");

	memset(&opt, 0, పరిమాణం(opt));
	filter.పంక్తులు = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, ఉపసర్గ, options, git_ట్యాగ్_usage, 0);

	ఒకవేళ (!cmdmode) {
		ఒకవేళ (argc == 0)
			cmdmode = 'l';
		లేకపోతే ఒకవేళ (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.పంక్తులు != -1)
			cmdmode = 'l';
	}

	ఒకవేళ (cmdmode == 'l')
		అమరిక_auto_pager("tag", 1);

	ఒకవేళ (trailer_వాదనలు.nr)
		trailer_config_init();

	ఒకవేళ (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_ట్యాగ్ > 0;

	ఒకవేళ (keyid) {
		opt.sign = 1;
		సమితి_signing_కీలకం(keyid);
	}
	create_ట్యాగ్_object = (opt.sign || annotate || సందేశం.given || msgfile ||
			     edit_flag || trailer_వాదనలు.nr);

	ఒకవేళ ((create_ట్యాగ్_object || force) && (cmdmode != 0))
		usage_with_options(git_ట్యాగ్_usage, options);

	finalize_colopts(&colopts, -1);
	ఒకవేళ (cmdmode == 'l' && filter.పంక్తులు != -1) {
		ఒకవేళ (explicitly_enable_స్తంభం(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	క్రమీకరణ = ref_క్రమీకరణ_options(&క్రమీకరణ_options);
	ref_క్రమీకరణ_సమితి_క్రమీకరించు_flags_all(క్రమీకరణ, REF_క్రమీకరణ_ICASE, icase);
	filter.ignore_case = icase;
	ఒకవేళ (cmdmode == 'l') {
		ఒకవేళ (స్తంభం_చురుకైన(colopts)) {
			నిర్మాణం స్తంభం_options copts;
			memset(&copts, 0, పరిమాణం(copts));
			copts.padding = 2;
			ఒకవేళ (నిరంతరం_స్తంభం_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_ట్యాగులు(&filter, క్రమీకరణ, &format);
		ఒకవేళ (స్తంభం_చురుకైన(colopts))
			stop_స్తంభం_filter();
		వెళ్ళు cleanup;
	}
	ఒకవేళ (filter.పంక్తులు != -1)
		only_లోపల_list = "-n";
	లేకపోతే ఒకవేళ (filter.with_commit)
		only_లోపల_list = "--contains";
	లేకపోతే ఒకవేళ (filter.no_commit)
		only_లోపల_list = "--no-contains";
	లేకపోతే ఒకవేళ (filter.points_at.nr)
		only_లోపల_list = "--points-at";
	లేకపోతే ఒకవేళ (filter.reachable_from)
		only_లోపల_list = "--merged";
	లేకపోతే ఒకవేళ (filter.unreachable_from)
		only_లోపల_list = "--no-merged";
	ఒకవేళ (only_లోపల_list)
		die(_("the '%s' option is only allowed in list mode"), only_లోపల_list);
	ఒకవేళ (cmdmode == 'd') {
		ret = delete_ట్యాగులు(argv);
		వెళ్ళు cleanup;
	}
	ఒకవేళ (cmdmode == 'v') {
		ఒకవేళ (format.format && verify_ref_format(&format))
			usage_with_options(git_ట్యాగ్_usage, options);
		ret = for_each_ట్యాగ్_name(argv, verify_ట్యాగ్, &format);
		వెళ్ళు cleanup;
	}

	ఒకవేళ (సందేశం.given || msgfile) {
		ఒకవేళ (సందేశం.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		ఒకవేళ (సందేశం.given)
			strbuf_addbuf(&buf, &(సందేశం.buf));
		లేకపోతే {
			ఒకవేళ (!strcmp(msgfile, "-")) {
				ఒకవేళ (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} లేకపోతే {
				ఒకవేళ (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	ట్యాగ్ = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	ఒకవేళ (argc > 2)
		die(_("too many arguments"));

	ఒకవేళ (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	ఒకవేళ (check_ట్యాగ్_ref(&ref, ట్యాగ్))
		die(_("'%s' is not a valid tag name."), ట్యాగ్);

	ఒకవేళ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	లేకపోతే ఒకవేళ (!force)
		die(_("tag '%s' already exists"), ట్యాగ్);

	opt.సందేశం_given = సందేశం.given || msgfile;
	opt.use_editor = edit_flag;

	ఒకవేళ (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_బహుళకం = CLEANUP_ALL;
	లేకపోతే ఒకవేళ (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_బహుళకం = CLEANUP_NONE;
	లేకపోతే ఒకవేళ (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_బహుళకం = CLEANUP_SPACE;
	లేకపోతే
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_సందేశం(&object, &reflog_సందేశం);

	ఒకవేళ (create_ట్యాగ్_object) {
		ఒకవేళ (force_sign_annotate && !annotate)
			opt.sign = 1;
		మార్గం = repo_git_మార్గం(the_repository, "TAG_EDITMSG");
		create_ట్యాగ్(&object, object_ref, ట్యాగ్, &buf, &opt, &prev, &object,
			   &trailer_వాదనలు, మార్గం);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ఒకవేళ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   ఖాళీ, ఖాళీ,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_సందేశం.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ఒకవేళ (మార్గం)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				మార్గం);
		die("%s", err.buf);
	}
	ఒకవేళ (మార్గం) {
		unlink_or_warn(మార్గం);
		విడుదల(మార్గం);
	}
	ref_transaction_free(transaction);
	ఒకవేళ (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		రాయి(_("Updated tag '%s' (was %s)\n"), ట్యాగ్,
		       repo_కనుగొను_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_క్రమీకరణ_release(క్రమీకరణ);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_సందేశం);
	strbuf_release(&సందేశం.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_వాదనలు);
	విడుదల(msgfile);
	తిరిగి ret;
}
