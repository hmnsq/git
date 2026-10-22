/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "convert.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "ident.h"
#अन्तर्भाव "list-objects-filter-options.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "userdiff.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/streaming.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "promisor-remote.h"
#अन्तर्भाव "mailmap.h"
#अन्तर्भाव "write-or-die.h"
#अन्तर्भाव "alias.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "transport.h"
#अन्तर्भाव "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#परिभाषय MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#परिभाषय MAX_ALLOWED_OBJ_सीमा 10000

/* Maximum input size permitted for the remote-object-info command. */
#परिभाषय MAX_REMOTE_OBJ_INFO_पङ्क्तिः \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_सीमा * (GIT_MAX_HEXSZ + 1))

गणना batch_बहुलकम् {
	BATCH_बहुलकम्_CONTENTS,
	BATCH_बहुलकम्_INFO,
	BATCH_बहुलकम्_QUEUE_AND_DISPATCH,
};

संरचना batch_options {
	संरचना list_objects_filter_options objects_filter;
	पूर्णाङ्क enabled;
	पूर्णाङ्क follow_symlinks;
	गणना batch_बहुलकम् batch_बहुलकम्;
	पूर्णाङ्क बफर_output;
	पूर्णाङ्क all_objects;
	पूर्णाङ्क unordered;
	पूर्णाङ्क transform_बहुलकम्; /* may be 'w' or 'c' for --filters or --textconv */
	वर्ण निवेशः_delim;
	वर्ण output_delim;
	ध्रुव वर्ण *format;
};

स्थिर ध्रुव वर्ण *force_मार्गः;

स्थिर संरचना string_list mailmap = STRING_LIST_INIT_NODUP;
स्थिर पूर्णाङ्क use_mailmap;

स्थिर वर्ण *replace_idents_using_mailmap(वर्ण *, आकार_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
स्थिर शून्य आरोपय_mailmap(शून्य)
{
	यदि (mailmap.strdup_वाक्यानि)
		प्रत्यागम;

	read_mailmap(the_repository, &mailmap);
}

स्थिर वर्ण *replace_idents_using_mailmap(वर्ण *object_buf, आकार_t *आकार)
{
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *शीर्षकाणि[] = { "author ", "committer ", "tagger ", रिक्त };

	strbuf_attach(&sb, object_buf, *आकार, *आकार + 1);
	apply_mailmap_गन्तव्यम्_शीर्षकम्(&sb, शीर्षकाणि, &mailmap);
	*आकार = sb.len;
	प्रत्यागम strbuf_detach(&sb, रिक्त);
}

स्थिर पूर्णाङ्क filter_object(ध्रुव वर्ण *मार्गः, अचिह्नित बहुलकम्,
			 ध्रुव संरचना object_id *oid,
			 वर्ण **buf, आकार_t *आकार)
{
	गणना object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, आकार);
	यदि (!*buf)
		प्रत्यागम error(_("cannot read object %s '%s'"),
			     oid_गन्तव्यम्_hex(oid), मार्गः);
	यदि ((type == OBJ_BLOB) && S_ISREG(बहुलकम्)) {
		संरचना strbuf strbuf = STRBUF_INIT;
		संरचना checkout_metadata meta;

		init_checkout_metadata(&meta, रिक्त, रिक्त, oid);
		यदि (convert_गन्तव्यम्_working_वृक्षः(the_repository->सूचकांक, मार्गः, *buf, *आकार, &strbuf, &meta)) {
			मुक्त(*buf);
			*आकार = strbuf.len;
			*buf = strbuf_detach(&strbuf, रिक्त);
		}
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क प्रवाहः_blob(ध्रुव संरचना object_id *oid)
{
	यदि (odb_प्रवाहः_blob_गन्तव्यम्_fd(the_repository->objects, 1, oid, रिक्त, 0))
		die("unable to stream %s to stdout", oid_गन्तव्यम्_hex(oid));
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cat_one_file(पूर्णाङ्क opt, ध्रुव वर्ण *exp_type, ध्रुव वर्ण *obj_name)
{
	पूर्णाङ्क ret;
	संरचना object_id oid;
	गणना object_type type;
	वर्ण *buf;
	आकार_t आकार;
	संरचना object_context obj_context = {0};
	संरचना object_info oi = OBJECT_INFO_INIT;
	अचिह्नित flags = OBJECT_INFO_LOOKUP_REPLACE;
	अचिह्नित get_oid_flags =
		GET_OID_लेखः_मार्गः |
		GET_OID_ONLY_गन्तव्यम्_DIE |
		GET_OID_HASH_ANY;
	ध्रुव वर्ण *मार्गः = force_मार्गः;
	ध्रुव पूर्णाङ्क opt_cw = (opt == 'c' || opt == 'w');
	यदि (!मार्गः && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_मार्गः;

	यदि (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	यदि (!मार्गः)
		मार्गः = obj_context.मार्गः;
	यदि (obj_context.बहुलकम् == S_IFINVALID)
		obj_context.बहुलकम् = 0100644;

	buf = रिक्त;
	विकल्प (opt) {
	पक्ष 't':
		oi.typep = &type;
		यदि (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		प्रारूप_लिख("%s\n", type_name(type));
		ret = 0;
		गच्छ cleanup;

	पक्ष 's':
		oi.sizep = &आकार;

		यदि (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (शून्य**)&buf;
		}

		यदि (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		यदि (use_mailmap && (type == OBJ_COMMIT || type == OBJ_चिह्नकम्))
			buf = replace_idents_using_mailmap(buf, &आकार);

		प्रारूप_लिख("%"PRIuMAX"\n", (uintmax_t)आकार);
		ret = 0;
		गच्छ cleanup;

	पक्ष 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		गच्छ cleanup;

	पक्ष 'w':

		यदि (filter_object(मार्गः, obj_context.बहुलकम्,
				  &oid, &buf, &आकार)) {
			ret = -1;
			गच्छ cleanup;
		}
		भङ्ग;

	पक्ष 'c':
	{
		अचिह्नित दीर्घ आकार_ul = 0;
		पूर्णाङ्क textconv_ret = textconv_object(the_repository, मार्गः,
						   obj_context.बहुलकम्, &oid, 1,
						   &buf, &आकार_ul);
		आकार = आकार_ul;
		यदि (textconv_ret)
			भङ्ग;
	}
		/* else fallthrough */

	पक्ष 'p':
		type = odb_read_object_info(the_repository->objects, &oid, रिक्त);
		यदि (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		यदि (type == OBJ_वृक्षः) {
			ध्रुव वर्ण *ls_तर्काः[3] = { रिक्त };
			ls_तर्काः[0] =  "ls-tree";
			ls_तर्काः[1] =  obj_name;
			ret = cmd_ls_वृक्षः(2, ls_तर्काः, रिक्त, the_repository);
			गच्छ cleanup;
		}

		यदि (type == OBJ_BLOB) {
			ret = प्रवाहः_blob(&oid);
			गच्छ cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &आकार);
		यदि (!buf)
			die("Cannot read object %s", obj_name);

		यदि (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &आकार);

		/* otherwise just spit out the data */
		भङ्ग;

	पक्ष 0:
	{
		गणना object_type exp_type_id = type_from_string(exp_type);

		यदि (exp_type_id == OBJ_BLOB) {
			संरचना object_id blob_oid;
			यदि (odb_read_object_info(the_repository->objects,
						 &oid, रिक्त) == OBJ_चिह्नकम्) {
				वर्ण *बफर = odb_read_object(the_repository->objects,
							       &oid, &type, &आकार);
				ध्रुव वर्ण *target;

				यदि (!बफर)
					die(_("unable to read %s"), oid_गन्तव्यम्_hex(&oid));

				यदि (!skip_उपसर्गः(बफर, "object ", &target) ||
				    get_oid_hex_algop(target, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_गन्तव्यम्_hex(&oid));
				मुक्त(बफर);
			} अन्यथा
				oidcpy(&blob_oid, &oid);

			यदि (odb_read_object_info(the_repository->objects,
						 &blob_oid, रिक्त) == OBJ_BLOB) {
				ret = प्रवाहः_blob(&blob_oid);
				गच्छ cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &आकार, रिक्त);

		यदि (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &आकार);
		भङ्ग;
	}
	स्वभाव:
		die("git cat-file: unknown option: %s", exp_type);
	}

	यदि (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, आकार);
	ret = 0;
cleanup:
	मुक्त(buf);
	object_context_release(&obj_context);
	प्रत्यागम ret;
}

संरचना expand_दत्त {
	संरचना object_id oid;
	गणना object_type type;
	आकार_t आकार;
	अचिह्नित ह्रस्व बहुलकम्;
	off_t disk_आकार;
	ध्रुव वर्ण *rest;
	संरचना object_id delta_आधारः_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	पूर्णाङ्क mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	पूर्णाङ्क विभज_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	संरचना object_info info;

	/*
	 * This flag will be true if the requested batch format and options
	 * don't require us to call oid_object_info, which can then be
	 * optimized out.
	 */
	अचिह्नित skip_object_info : 1;

	/*
	 * Flags about when an object info is being fetched from remote.
	 */
	अचिह्नित is_remote:1;

	/*
	 * List of atoms (i.e. "objectsize") that the server supports. Built
	 * from the server's object-info advertised capabilities.
	 */
	संरचना string_list remote_allowed_atoms;
};

#परिभाषय EXPAND_दत्त_INIT  {  \
	.बहुलकम् = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

स्थिर पूर्णाङ्क is_atom(ध्रुव वर्ण *atom, ध्रुव वर्ण *s, पूर्णाङ्क slen)
{
	पूर्णाङ्क alen = वाक्यदैर्घ्यम्(atom);
	प्रत्यागम alen == slen && !memcmp(atom, s, alen);
}

स्थिर पूर्णाङ्क expand_atom(संरचना strbuf *sb, ध्रुव वर्ण *atom, पूर्णाङ्क len,
		       संरचना expand_दत्त *दत्त)
{
	यदि (दत्त->is_remote) {
		आकार_t i;
		प्रति (i = 0; i < दत्त->remote_allowed_atoms.nr; i++)
			यदि (is_atom(दत्त->remote_allowed_atoms.वस्तूनि[i].string,
				    atom, len))
				भङ्ग;
		यदि (i == दत्त->remote_allowed_atoms.nr)
			प्रत्यागम 1;
	}

	यदि (is_atom("objectname", atom, len)) {
		यदि (!दत्त->mark_query)
			strbuf_सङ्कलय_oid_hex(sb, &दत्त->oid);
	} अन्यथा यदि (is_atom("objecttype", atom, len)) {
		यदि (दत्त->mark_query) {
			दत्त->info.typep = &दत्त->type;
		} अन्यथा {
			ध्रुव वर्ण *t = type_name(दत्त->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} अन्यथा यदि (is_atom("objectsize", atom, len)) {
		यदि (दत्त->mark_query)
			दत्त->info.sizep = &दत्त->आकार;
		अन्यथा
			strbuf_सङ्कलय_uint(sb, दत्त->आकार);
	} अन्यथा यदि (is_atom("objectsize:disk", atom, len)) {
		यदि (दत्त->mark_query)
			दत्त->info.disk_sizep = &दत्त->disk_आकार;
		अन्यथा
			strbuf_सङ्कलय_uint(sb, दत्त->disk_आकार);
	} अन्यथा यदि (is_atom("rest", atom, len)) {
		यदि (दत्त->mark_query)
			दत्त->विभज_on_whitespace = 1;
		अन्यथा यदि (दत्त->rest)
			strbuf_addstr(sb, दत्त->rest);
	} अन्यथा यदि (is_atom("deltabase", atom, len)) {
		यदि (दत्त->mark_query)
			दत्त->info.delta_आधारः_oid = &दत्त->delta_आधारः_oid;
		अन्यथा
			strbuf_सङ्कलय_oid_hex(sb, &दत्त->delta_आधारः_oid);
	} अन्यथा यदि (is_atom("objectmode", atom, len)) {
		यदि (!दत्त->mark_query && !(S_IFINVALID == दत्त->बहुलकम्))
			strbuf_addf(sb, "%06o", दत्त->बहुलकम्);
	} अन्यथा
		प्रत्यागम 0;
	प्रत्यागम 1;
}

स्थिर शून्य expand_format(संरचना strbuf *sb, ध्रुव वर्ण *start,
			  संरचना expand_दत्त *दत्त)
{
	यावत् (strbuf_expand_चरणः(sb, &start)) {
		ध्रुव वर्ण *अन्तः;

		यदि (skip_उपसर्गः(start, "%", &start) || *start != '(')
			strbuf_addch(sb, '%');
		अन्यथा यदि ((अन्तः = strchr(start + 1, ')')) &&
			 expand_atom(sb, start + 1, अन्तः - start - 1, दत्त))
			start = अन्तः + 1;
		अन्यथा
			strbuf_expand_bad_format(start, "cat-file");
	}
}

स्थिर शून्य batch_write(संरचना batch_options *opt, ध्रुव शून्य *दत्त, पूर्णाङ्क len)
{
	यदि (opt->बफर_output) {
		यदि (fwrite(दत्त, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} अन्यथा
		write_or_die(1, दत्त, len);
}

स्थिर शून्य print_object_or_die(संरचना batch_options *opt, संरचना expand_दत्त *दत्त)
{
	ध्रुव संरचना object_id *oid = &दत्त->oid;

	प्रतिज्ञा(दत्त->info.typep);

	यदि (दत्त->type == OBJ_BLOB) {
		यदि (opt->बफर_output)
			fflush(stdout);
		यदि (opt->transform_बहुलकम्) {
			वर्ण *contents;
			आकार_t आकार;

			यदि (!दत्त->rest)
				die("missing path for '%s'", oid_गन्तव्यम्_hex(oid));

			यदि (opt->transform_बहुलकम् == 'w') {
				यदि (filter_object(दत्त->rest, 0100644, oid,
						  &contents, &आकार))
					die("could not convert '%s' %s",
					    oid_गन्तव्यम्_hex(oid), दत्त->rest);
			} अन्यथा यदि (opt->transform_बहुलकम् == 'c') {
				गणना object_type type;
				अचिह्नित दीर्घ आकार_ul = 0;
				यदि (textconv_object(the_repository,
						    दत्त->rest, 0100644, oid,
						    1, &contents, &आकार_ul))
					आकार = आकार_ul;
				अन्यथा
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &आकार);
				यदि (!contents)
					die("could not convert '%s' %s",
					    oid_गन्तव्यम्_hex(oid), दत्त->rest);
			} अन्यथा
				BUG("invalid transform_mode: %c", opt->transform_बहुलकम्);
			batch_write(opt, contents, आकार);
			मुक्त(contents);
		} अन्यथा {
			प्रवाहः_blob(oid);
		}
	}
	अन्यथा {
		गणना object_type type;
		आकार_t आकार;
		शून्य *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &आकार);
		यदि (!contents)
			die("object %s disappeared", oid_गन्तव्यम्_hex(oid));

		यदि (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &आकार);

		यदि (type != दत्त->type)
			die("object %s changed type!?", oid_गन्तव्यम्_hex(oid));
		यदि (दत्त->info.sizep && आकार != दत्त->आकार && !use_mailmap)
			die("object %s changed size!?", oid_गन्तव्यम्_hex(oid));

		batch_write(opt, contents, आकार);
		मुक्त(contents);
	}
}

स्थिर शून्य print_default_format(संरचना strbuf *scratch, संरचना expand_दत्त *दत्त,
				 संरचना batch_options *opt)
{
	strbuf_सङ्कलय_oid_hex(scratch, &दत्त->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(दत्त->type));
	strbuf_addch(scratch, ' ');
	strbuf_सङ्कलय_uint(scratch, दत्त->आकार);
	strbuf_addch(scratch, opt->output_delim);
}

स्थिर शून्य report_object_स्थितिः(संरचना batch_options *opt,
				 ध्रुव वर्ण *obj_name,
				 ध्रुव संरचना object_id *oid,
				 ध्रुव वर्ण *स्थितिः)
{
	प्रारूप_लिख("%s %s%c", obj_name ? obj_name : oid_गन्तव्यम्_hex(oid),
	       स्थितिः, opt->output_delim);
	fflush(stdout);
}

/*
 * If "pack" is non-NULL, then "offset" is the byte offset within the pack from
 * which the object may be accessed (though note that we may also rely on
 * data->oid, too). If "pack" is NULL, then offset is ignored.
 */
स्थिर शून्य batch_object_write(ध्रुव वर्ण *obj_name,
			       संरचना strbuf *scratch,
			       संरचना batch_options *opt,
			       संरचना expand_दत्त *दत्त,
			       संरचना packed_git *pack,
			       off_t विस्थापनम्)
{
	यदि (!दत्त->skip_object_info) {
		पूर्णाङ्क ret;

		यदि (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_सीमा ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			दत्त->info.typep = &दत्त->type;
		यदि (opt->objects_filter.choice == LOFC_BLOB_सीमा)
			दत्त->info.sizep = &दत्त->आकार;

		यदि (pack)
			ret = packed_object_info(रिक्त, pack, विस्थापनम्, &दत्त->info);
		अन्यथा
			ret = odb_read_object_info_extended(the_repository->objects,
							    &दत्त->oid, &दत्त->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		यदि (ret < 0) {
			यदि (दत्त->बहुलकम् == S_IFGITLINK)
				report_object_स्थितिः(opt, रिक्त, &दत्त->oid, "submodule");
			अन्यथा
				report_object_स्थितिः(opt, obj_name, &दत्त->oid, "missing");
			प्रत्यागम;
		}

		विकल्प (opt->objects_filter.choice) {
		पक्ष LOFC_DISABLED:
			भङ्ग;
		पक्ष LOFC_BLOB_NONE:
			यदि (दत्त->type == OBJ_BLOB) {
				यदि (!opt->all_objects)
					report_object_स्थितिः(opt, obj_name,
							     &दत्त->oid, "excluded");
				प्रत्यागम;
			}
			भङ्ग;
		पक्ष LOFC_BLOB_सीमा:
			यदि (दत्त->type == OBJ_BLOB &&
			    दत्त->आकार >= opt->objects_filter.blob_सीमा_मानम्) {
				यदि (!opt->all_objects)
					report_object_स्थितिः(opt, obj_name,
							     &दत्त->oid, "excluded");
				प्रत्यागम;
			}
			भङ्ग;
		पक्ष LOFC_OBJECT_TYPE:
			यदि (दत्त->type != opt->objects_filter.object_type) {
				यदि (!opt->all_objects)
					report_object_स्थितिः(opt, obj_name,
							     &दत्त->oid, "excluded");
				प्रत्यागम;
			}
			भङ्ग;
		स्वभाव:
			BUG("unsupported objects filter");
		}

		यदि (use_mailmap && (दत्त->type == OBJ_COMMIT || दत्त->type == OBJ_चिह्नकम्)) {
			वर्ण *buf = रिक्त;

			buf = odb_read_object(the_repository->objects, &दत्त->oid,
					      &दत्त->type, &दत्त->आकार);
			यदि (!buf)
				die(_("unable to read %s"), oid_गन्तव्यम्_hex(&दत्त->oid));
			buf = replace_idents_using_mailmap(buf, &दत्त->आकार);

			मुक्त(buf);
		}
	}

	strbuf_reset(scratch);

	यदि (!opt->format) {
		print_default_format(scratch, दत्त, opt);
	} अन्यथा {
		expand_format(scratch, opt->format, दत्त);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	यदि (opt->batch_बहुलकम् == BATCH_बहुलकम्_CONTENTS) {
		print_object_or_die(opt, दत्त);
		batch_write(opt, &opt->output_delim, 1);
	}
}

स्थिर शून्य batch_one_object(ध्रुव वर्ण *obj_name,
			     संरचना strbuf *scratch,
			     संरचना batch_options *opt,
			     संरचना expand_दत्त *दत्त)
{
	संरचना object_context ctx = {0};
	पूर्णाङ्क flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	गणना get_oid_परिणाम परिणाम;

	परिणाम = get_oid_with_context(the_repository, obj_name,
				      flags, &दत्त->oid, &ctx);
	यदि (परिणाम != प्राप्तम्) {
		विकल्प (परिणाम) {
		पक्ष MISSING_OBJECT:
			report_object_स्थितिः(opt, obj_name, &दत्त->oid, "missing");
			भङ्ग;
		पक्ष SHORT_NAME_AMBIGUOUS:
			report_object_स्थितिः(opt, obj_name, &दत्त->oid, "ambiguous");
			भङ्ग;
		पक्ष DANGLING_SYMLINK:
			प्रारूप_लिख("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)वाक्यदैर्घ्यम्(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			भङ्ग;
		पक्ष SYMLINK_LOOP:
			प्रारूप_लिख("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)वाक्यदैर्घ्यम्(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			भङ्ग;
		पक्ष NOT_DIR:
			प्रारूप_लिख("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)वाक्यदैर्घ्यम्(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			भङ्ग;
		स्वभाव:
			BUG("unknown get_sha1_with_context result %d\n",
			       परिणाम);
			भङ्ग;
		}
		fflush(stdout);

		गच्छ बहिर्गमनम्;
	}

	यदि (ctx.बहुलकम् == 0) {
		प्रारूप_लिख("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_मार्गः.len,
		       opt->output_delim, ctx.symlink_मार्गः.buf, opt->output_delim);
		fflush(stdout);
		गच्छ बहिर्गमनम्;
	}

	दत्त->बहुलकम् = ctx.बहुलकम्;
	batch_object_write(obj_name, scratch, opt, दत्त, रिक्त, 0);

बहिर्गमनम्:
	object_context_release(&ctx);
}

स्थिर पूर्णाङ्क get_remote_info(पूर्णाङ्क argc,
			   ध्रुव वर्ण **argv,
			   संरचना fetch_object_info_results *results,
			   संरचना oid_array *object_info_oids)
{
	पूर्णाङ्क retval = 0;
	संरचना remote *remote = रिक्त;
	संरचना object_id oid;
	संरचना transport *gtransport;

	remote = remote_get(argv[0]);
	यदि (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	प्रति (आकार_t i = 1; i < argc; i++) {
		यदि (get_oid_hex(argv[i], &oid)) {
			आकार_t len = वाक्यदैर्घ्यम्(argv[i]);

			यदि (len < the_hash_algo->hexsz && len >= 4) {
				आकार_t j;
				प्रति (j = 0; j < len; j++)
					यदि (!isxdigit(argv[i][j]))
						भङ्ग;
				यदि (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (पूर्णाङ्क)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	यदि (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, रिक्त);

	यदि (!gtransport->smart_options) {
		retval = -1;
		गच्छ cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	प्रत्यागम retval;
}

संरचना object_cb_दत्त {
	संरचना batch_options *opt;
	संरचना expand_दत्त *expand;
	संरचना oidset *आलोकितम्;
	संरचना strbuf *scratch;
};

स्थिर पूर्णाङ्क batch_object_cb(ध्रुव संरचना object_id *oid, शून्य *vdata)
{
	संरचना object_cb_दत्त *दत्त = vdata;
	oidcpy(&दत्त->expand->oid, oid);
	batch_object_write(रिक्त, दत्त->scratch, दत्त->opt, दत्त->expand,
			   रिक्त, 0);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क collect_object(ध्रुव संरचना object_id *oid,
			  संरचना packed_git *pack अप्रयुक्तम्,
			  off_t विस्थापनम् अप्रयुक्तम्,
			  शून्य *दत्त)
{
	oid_array_append(दत्त, oid);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क batch_unordered_object(ध्रुव संरचना object_id *oid,
				  संरचना packed_git *pack,
				  off_t विस्थापनम्,
				  शून्य *vdata)
{
	संरचना object_cb_दत्त *दत्त = vdata;

	यदि (oidset_निवेशय(दत्त->आलोकितम्, oid))
		प्रत्यागम 0;

	oidcpy(&दत्त->expand->oid, oid);
	batch_object_write(रिक्त, दत्त->scratch, दत्त->opt, दत्त->expand,
			   pack, विस्थापनम्);
	प्रत्यागम 0;
}

प्रकारनिर्देश शून्य (*parse_cmd_fn_t)(संरचना batch_options *, ध्रुव वर्ण *,
			       संरचना strbuf *, संरचना expand_दत्त *);

संरचना queued_cmd {
	parse_cmd_fn_t fn;
	वर्ण *पङ्क्तिः;
};

स्थिर शून्य parse_cmd_contents(संरचना batch_options *opt,
			     ध्रुव वर्ण *पङ्क्तिः,
			     संरचना strbuf *output,
			     संरचना expand_दत्त *दत्त)
{
	गणना object_type *saved_typep = दत्त->info.typep;
	दत्त->info.typep = &दत्त->type;
	opt->batch_बहुलकम् = BATCH_बहुलकम्_CONTENTS;
	batch_one_object(पङ्क्तिः, output, opt, दत्त);
	दत्त->info.typep = saved_typep;
}

स्थिर शून्य parse_cmd_info(संरचना batch_options *opt,
			   ध्रुव वर्ण *पङ्क्तिः,
			   संरचना strbuf *output,
			   संरचना expand_दत्त *दत्त)
{
	opt->batch_बहुलकम् = BATCH_बहुलकम्_INFO;
	batch_one_object(पङ्क्तिः, output, opt, दत्त);
}

स्थिर शून्य parse_cmd_mailmap(संरचना batch_options *opt अप्रयुक्तम्,
			      ध्रुव वर्ण *पङ्क्तिः,
			      संरचना strbuf *output अप्रयुक्तम्,
			      संरचना expand_दत्त *दत्त अप्रयुक्तम्)
{
	use_mailmap = git_parse_maybe_bool(पङ्क्तिः);

	यदि (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), पङ्क्तिः);

	यदि (use_mailmap)
		आरोपय_mailmap();
}

स्थिर शून्य parse_cmd_remote_object_info(संरचना batch_options *opt,
					 ध्रुव वर्ण *पङ्क्तिः, संरचना strbuf *output,
					 संरचना expand_दत्त *दत्त)
{
	पूर्णाङ्क गणनम्;
	ध्रुव वर्ण **argv;
	वर्ण *पङ्क्तिः_गन्तव्यम्_विभज;
	संरचना fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	संरचना oid_array object_info_oids = OID_ARRAY_INIT;

	यदि (वाक्यदैर्घ्यम्(पङ्क्तिः) >= MAX_REMOTE_OBJ_INFO_पङ्क्तिः)
		die(_("remote-object-info command too long"));

	पङ्क्तिः_गन्तव्यम्_विभज = xstrdup(पङ्क्तिः);
	गणनम् = विभज_cmdline(पङ्क्तिः_गन्तव्यम्_विभज, &argv);
	यदि (गणनम् < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    विभज_cmdline_strerror(गणनम्));
	यदि (गणनम् - 1 > MAX_ALLOWED_OBJ_सीमा)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_सीमा);

	यदि (दत्त->info.sizep)
		results.इच्छति_आकार = 1;
	यदि (दत्त->info.typep)
		results.इच्छति_type = 1;

	यदि (get_remote_info(गणनम्, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&दत्त->remote_allowed_atoms, 0);
	string_list_append(&दत्त->remote_allowed_atoms, "objectname");
	यदि (results.sizes)
		string_list_append(&दत्त->remote_allowed_atoms, "objectsize");
	यदि (results.types)
		string_list_append(&दत्त->remote_allowed_atoms, "objecttype");

	दत्त->skip_object_info = 1;
	प्रति (आकार_t i = 0; i < results.nr; i++) {
		दत्त->oid = object_info_oids.oid[i];

		यदि (results.unrecognized[i]) {
			report_object_स्थितिः(opt, oid_गन्तव्यम्_hex(&दत्त->oid),
					     &दत्त->oid, "missing");
			अनुवर्त;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		यदि (results.sizes)
			दत्त->आकार = results.sizes[i];

		यदि (results.types)
			दत्त->type = results.types[i];

		opt->batch_बहुलकम् = BATCH_बहुलकम्_INFO;
		दत्त->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, दत्त, रिक्त, 0);
		दत्त->is_remote = 0;
	}
	दत्त->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	मुक्त(पङ्क्तिः_गन्तव्यम्_विभज);
	मुक्त(argv);
	oid_array_clear(&object_info_oids);
}

स्थिर शून्य dispatch_calls(संरचना batch_options *opt,
		संरचना strbuf *output,
		संरचना expand_दत्त *दत्त,
		संरचना queued_cmd *cmd,
		आकार_t nr)
{
	यदि (!opt->बफर_output)
		die(_("flush is only for --buffer mode"));

	प्रति (आकार_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].पङ्क्तिः, output, दत्त);

	fflush(stdout);
}

स्थिर शून्य free_cmds(संरचना queued_cmd *cmd, आकार_t *nr)
{
	प्रति (आकार_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].पङ्क्तिः);

	*nr = 0;
}


स्थिर ध्रुव संरचना parse_cmd {
	ध्रुव वर्ण *name;
	parse_cmd_fn_t fn;
	अचिह्नित takes_तर्काः;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", रिक्त, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

स्थिर शून्य batch_objects_command(संरचना batch_options *opt,
				    संरचना strbuf *output,
				    संरचना expand_दत्त *दत्त)
{
	संरचना strbuf निवेशः = STRBUF_INIT;
	संरचना queued_cmd *queued_cmd = रिक्त;
	आकार_t alloc = 0, nr = 0;

	यावत् (strbuf_getdelim_strip_crlf(&निवेशः, stdin, opt->निवेशः_delim) != EOF) {
		ध्रुव संरचना parse_cmd *cmd = रिक्त;
		ध्रुव वर्ण *p = रिक्त, *cmd_अन्तः;
		संरचना queued_cmd call = {0};

		यदि (!निवेशः.len)
			die(_("empty command in input"));
		यदि (रिक्तम्_वा(*निवेशः.buf))
			die(_("whitespace before command: '%s'"), निवेशः.buf);

		प्रति (आकार_t i = 0; i < ARRAY_आकार(commands); i++) {
			यदि (!skip_उपसर्गः(निवेशः.buf, commands[i].name, &cmd_अन्तः))
				अनुवर्त;

			cmd = &commands[i];
			यदि (cmd->takes_तर्काः) {
				यदि (*cmd_अन्तः != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_अन्तः + 1;
			} अन्यथा यदि (*cmd_अन्तः) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			भङ्ग;
		}

		यदि (!cmd)
			die(_("unknown command: '%s'"), निवेशः.buf);

		यदि (!वाक्यं_तुलय(cmd->name, "flush")) {
			dispatch_calls(opt, output, दत्त, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} अन्यथा यदि (!opt->बफर_output) {
			cmd->fn(opt, p, output, दत्त);
		} अन्यथा {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.पङ्क्तिः = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	यदि (opt->बफर_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, दत्त, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	मुक्त(queued_cmd);
	strbuf_release(&निवेशः);
}

#परिभाषय DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

प्रकारनिर्देश पूर्णाङ्क (*for_each_object_fn)(ध्रुव संरचना object_id *oid, संरचना packed_git *pack,
				  off_t विस्थापनम्, शून्य *दत्त);

संरचना for_each_object_payload {
	for_each_object_fn callback;
	शून्य *payload;
};

स्थिर पूर्णाङ्क batch_one_object_oi(ध्रुव संरचना object_id *oid,
			       संरचना object_info *oi,
			       शून्य *_payload)
{
	संरचना for_each_object_payload *payload = _payload;
	यदि (oi && oi->source_infop->source->type == ODB_SOURCE_PACKED)
		प्रत्यागम payload->callback(oid, oi->source_infop->u.packed.pack,
					 oi->source_infop->u.packed.विस्थापनम्,
					 payload->payload);
	प्रत्यागम payload->callback(oid, रिक्त, 0, payload->payload);
}

स्थिर शून्य batch_each_object(संरचना batch_options *opt,
			      for_each_object_fn callback,
			      अचिह्नित flags,
			      शून्य *_payload)
{
	संरचना for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	संरचना odb_source_info source_info;
	संरचना object_info oi = {
		.source_infop = &source_info,
	};
	संरचना odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

स्थिर पूर्णाङ्क batch_objects(संरचना batch_options *opt)
{
	संरचना strbuf निवेशः = STRBUF_INIT;
	संरचना strbuf output = STRBUF_INIT;
	संरचना expand_दत्त दत्त = EXPAND_दत्त_INIT;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);
	पूर्णाङ्क save_warning;
	पूर्णाङ्क retval = 0;

	/*
	 * Expand once with our special mark_query flag, which will prime the
	 * object_info to be handed to odb_read_object_info_extended for each
	 * object.
	 */
	दत्त.mark_query = 1;
	expand_format(&output,
		      opt->format ? opt->format : DEFAULT_FORMAT,
		      &दत्त);
	दत्त.mark_query = 0;
	strbuf_release(&output);
	यदि (opt->transform_बहुलकम्)
		दत्त.विभज_on_whitespace = 1;

	यदि (opt->format && !वाक्यं_तुलय(opt->format, DEFAULT_FORMAT))
		opt->format = रिक्त;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	यदि (opt->batch_बहुलकम् == BATCH_बहुलकम्_CONTENTS)
		दत्त.info.typep = &दत्त.type;

	यदि (opt->all_objects) {
		संरचना object_cb_दत्त cb;
		संरचना object_info रिक्तानि = OBJECT_INFO_INIT;

		यदि (!memcmp(&दत्त.info, &रिक्तानि, परिमाण(रिक्तानि)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			दत्त.skip_object_info = 1;

		यदि (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &दत्त;
		cb.scratch = &output;

		यदि (opt->unordered) {
			संरचना oidset आलोकितम् = OIDSET_INIT;

			cb.आलोकितम् = &आलोकितम्;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_क्रमः, &cb);

			oidset_clear(&आलोकितम्);
		} अन्यथा {
			संरचना oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		प्रत्यागम 0;
	}

	/*
	 * We are going to call get_sha1 on a potentially very large number of
	 * objects. In most large cases, these will be actual object sha1s. The
	 * cost to double-check that each one is not also a ref (just so we can
	 * warn) ends up dwarfing the actual cost of the object lookups
	 * themselves. We can work around it by just turning off the warning.
	 */
	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	यदि (opt->batch_बहुलकम् == BATCH_बहुलकम्_QUEUE_AND_DISPATCH) {
		batch_objects_command(opt, &output, &दत्त);
		गच्छ cleanup;
	}

	यावत् (strbuf_getdelim_strip_crlf(&निवेशः, stdin, opt->निवेशः_delim) != EOF) {
		यदि (दत्त.विभज_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			वर्ण *p = strpbrk(निवेशः.buf, " \t");
			यदि (p) {
				यावत् (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			दत्त.rest = p;
		}

		batch_one_object(निवेशः.buf, &output, opt, &दत्त);
	}

 cleanup:
	strbuf_release(&निवेशः);
	strbuf_release(&output);
	string_list_clear(&दत्त.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क git_cat_file_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			       ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	यदि (userdiff_config(var, मानम्) < 0)
		प्रत्यागम -1;

	प्रत्यागम git_default_config(var, मानम्, ctx, cb);
}

स्थिर पूर्णाङ्क batch_option_callback(ध्रुव संरचना option *opt,
				 ध्रुव वर्ण *arg,
				 पूर्णाङ्क unset)
{
	संरचना batch_options *bo = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	यदि (bo->enabled) {
		प्रत्यागम error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	यदि (!वाक्यं_तुलय(opt->long_name, "batch"))
		bo->batch_बहुलकम् = BATCH_बहुलकम्_CONTENTS;
	अन्यथा यदि (!वाक्यं_तुलय(opt->long_name, "batch-check"))
		bo->batch_बहुलकम् = BATCH_बहुलकम्_INFO;
	अन्यथा यदि (!वाक्यं_तुलय(opt->long_name, "batch-command"))
		bo->batch_बहुलकम् = BATCH_बहुलकम्_QUEUE_AND_DISPATCH;
	अन्यथा
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_cat_file(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *उपसर्गः,
		 संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क opt = 0;
	पूर्णाङ्क opt_cw = 0;
	पूर्णाङ्क opt_epts = 0;
	ध्रुव वर्ण *exp_type = रिक्त, *obj_name = रिक्त;
	संरचना batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	पूर्णाङ्क unknown_type = 0;
	पूर्णाङ्क निवेशः_nul_terminated = 0;
	पूर्णाङ्क nul_terminated = 0;
	पूर्णाङ्क ret;

	ध्रुव वर्ण * ध्रुव builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		रिक्त
	};
	ध्रुव संरचना option options[] = {
		/* Simple queries */
		OPT_गणः(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', रिक्त, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', रिक्त, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_गणः(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', रिक्त, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', रिक्त, &opt, N_("show object size"), 's'),
		OPT_HIDDEN_BOOL(0, "allow-unknown-type", &unknown_type,
			  N_("historical option -- no-op")),
		OPT_BOOL(0, "use-mailmap", &use_mailmap, N_("use mail map file")),
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		/* Batch mode */
		OPT_गणः(N_("Batch objects requested on stdin (or --batch-all-objects)")),
		OPT_CALLBACK_F(0, "batch", &batch, N_("format"),
			N_("show full <object> or <rev> contents"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CALLBACK_F(0, "batch-check", &batch, N_("format"),
			N_("like --batch, but don't emit <contents>"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_BOOL_F('z', रिक्त, &निवेशः_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', रिक्त, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
		OPT_CALLBACK_F(0, "batch-command", &batch, N_("format"),
			N_("read commands from stdin"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CMDMODE(0, "batch-all-objects", &opt,
			    N_("with --batch[-check]: ignores stdin, batches all known objects"), 'b'),
		/* Batch-specific options */
		OPT_गणः(N_("Change or optimize batch output")),
		OPT_BOOL(0, "buffer", &batch.बफर_output, N_("buffer --batch output")),
		OPT_BOOL(0, "follow-symlinks", &batch.follow_symlinks,
			 N_("follow in-tree symlinks")),
		OPT_BOOL(0, "unordered", &batch.unordered,
			 N_("do not order objects before emitting them")),
		/* Textconv options, stand-ole*/
		OPT_गणः(N_("Emit object (blob or tree) with conversion or filter (stand-alone, or with batch)")),
		OPT_CMDMODE(0, "textconv", &opt,
			    N_("run textconv on object's content"), 'c'),
		OPT_CMDMODE(0, "filters", &opt,
			    N_("run filters on object's content"), 'w'),
		OPT_STRING(0, "path", &force_मार्गः, N_("blob|tree"),
			   N_("use a <path> for (--textconv | --filters); Not with 'batch'")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&batch.objects_filter),
		OPT_अन्तः()
	};

	repo_config(the_repository, git_cat_file_config, रिक्त);

	batch.बफर_output = -1;

	argc = parse_options(argc, argv, उपसर्गः, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	यदि (use_mailmap)
		आरोपय_mailmap();

	विकल्प (batch.objects_filter.choice) {
	पक्ष LOFC_DISABLED:
		भङ्ग;
	पक्ष LOFC_BLOB_NONE:
	पक्ष LOFC_BLOB_सीमा:
	पक्ष LOFC_OBJECT_TYPE:
		यदि (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		भङ्ग;
	स्वभाव:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	यदि (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	यदि (force_मार्गः && !opt_cw)
		usage_सन्देशः_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	यदि (batch.enabled)
		;
	अन्यथा यदि (batch.follow_symlinks)
		usage_सन्देशः_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	अन्यथा यदि (batch.बफर_output >= 0)
		usage_सन्देशः_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	अन्यथा यदि (batch.all_objects)
		usage_सन्देशः_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	अन्यथा यदि (निवेशः_nul_terminated)
		usage_सन्देशः_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	अन्यथा यदि (nul_terminated)
		usage_सन्देशः_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.निवेशः_delim = batch.output_delim = '\n';
	यदि (निवेशः_nul_terminated)
		batch.निवेशः_delim = '\0';
	यदि (nul_terminated)
		batch.निवेशः_delim = batch.output_delim = '\0';

	/* Batch defaults */
	यदि (batch.बफर_output < 0)
		batch.बफर_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	/* Return early if we're in batch mode? */
	यदि (batch.enabled) {
		यदि (opt_cw)
			batch.transform_बहुलकम् = opt;
		अन्यथा यदि (opt && opt != 'b')
			usage_सन्देशः_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		अन्यथा यदि (argc)
			usage_सन्देशः_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		गच्छ बहिर्गमनम्;
	}

	यदि (opt) {
		यदि (!argc && opt == 'c')
			usage_सन्देशः_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		अन्यथा यदि (!argc && opt == 'w')
			usage_सन्देशः_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		अन्यथा यदि (!argc && opt_epts)
			usage_सन्देशः_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		अन्यथा यदि (argc == 1)
			obj_name = argv[0];
		अन्यथा
			usage_सन्देशः_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} अन्यथा यदि (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} अन्यथा यदि (argc != 2) {
		usage_सन्देशः_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} अन्यथा यदि (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

बहिर्गमनम्:
	list_objects_filter_release(&batch.objects_filter);
	प्रत्यागम ret;
}
