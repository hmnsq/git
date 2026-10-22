#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "fsck.h"
#अन्तर्भाव "config.h"

स्थिर अक्षर स्थिरांक * स्थिरांक builtin_mktag_usage[] = {
	"git mktag",
	रिक्त
};
स्थिर पूर्णांक option_strict = 1;

स्थिर संरचना fsck_options fsck_options;

स्थिर पूर्णांक mktag_fsck_error_func(संरचना fsck_options *o UNUSED,
				 शून्य *fsck_report UNUSED,
				 गणना fsck_संदेश_type संदेश_type,
				 गणना fsck_संदेश_id संदेश_id UNUSED,
				 स्थिरांक अक्षर *message)
{
	निवड (संदेश_type) {
	स्थिती FSCK_WARN:
		जर (!option_strict) {
			fprintf_ln(मानक_त्रुटि, _("warning: tag input does not pass fsck: %s"), message);
			परत 0;

		}
		/* fallthrough */
	स्थिती FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(मानक_त्रुटि, _("error: tag input does not pass fsck: %s"), message);
		परत 1;
	पूर्वनिर्धारित:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    संदेश_type);
	}
}

स्थिर पूर्णांक verify_object_in_tag(संरचना object_id *tagged_oid, पूर्णांक *tagged_type)
{
	पूर्णांक ret;
	गणना object_type type;
	माप_प्रकार size;
	शून्य *buffer;
	स्थिरांक संरचना object_id *repl;

	buffer = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &size);
	जर (!buffer)
		die(_("could not read tagged object '%s'"),
		    oid_to_hex(tagged_oid));
	जर (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_to_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, buffer, size,
				     *tagged_type);
	मुक्त(buffer);

	परत ret;
}

पूर्णांक cmd_mktag(पूर्णांक argc,
	      स्थिरांक अक्षर **argv,
	      स्थिरांक अक्षर *prefix,
	      संरचना repository *repo)
{
	स्थिर संरचना option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_END(),
	};
	संरचना strbuf buf = STRBUF_INIT;
	संरचना object_id tagged_oid;
	पूर्णांक tagged_type;
	संरचना object_id result;

	argc = parse_options(argc, argv, prefix,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	जर (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_set_संदेश_type_from_ids(&fsck_options, FSCK_संदेश_EXTRA_HEADER_ENTRY,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	जर (fsck_tag_standalone(रिक्त, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	जर (verify_object_in_tag(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	जर (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_TAG, &result) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	माला_दो(oid_to_hex(&result));
	परत 0;
}
