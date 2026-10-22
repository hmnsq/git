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

स्थिर वर्ण अचर * अचर builtin_mktag_usage[] = {
	"git mktag",
	रिक्त
};
स्थिर पूर्णाङ्क option_strict = 1;

स्थिर संरचना fsck_options fsck_options;

स्थिर पूर्णाङ्क mktag_fsck_error_func(संरचना fsck_options *o UNUSED,
				 शून्य *fsck_report UNUSED,
				 गणना fsck_सन्देश_type सन्देश_type,
				 गणना fsck_सन्देश_id सन्देश_id UNUSED,
				 अचर वर्ण *message)
{
	विकल्प (सन्देश_type) {
	पक्ष FSCK_WARN:
		यदि (!option_strict) {
			fprintf_ln(मानक_त्रुटि, _("warning: tag input does not pass fsck: %s"), message);
			फर्काऊ 0;

		}
		/* fallthrough */
	पक्ष FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(मानक_त्रुटि, _("error: tag input does not pass fsck: %s"), message);
		फर्काऊ 1;
	स्वभाव:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    सन्देश_type);
	}
}

स्थिर पूर्णाङ्क verify_object_in_tag(संरचना object_id *tagged_oid, पूर्णाङ्क *tagged_type)
{
	पूर्णाङ्क ret;
	गणना object_type type;
	माप_प्रकार size;
	शून्य *buffer;
	अचर संरचना object_id *repl;

	buffer = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &size);
	यदि (!buffer)
		die(_("could not read tagged object '%s'"),
		    oid_to_hex(tagged_oid));
	यदि (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_to_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, buffer, size,
				     *tagged_type);
	मुक्त(buffer);

	फर्काऊ ret;
}

पूर्णाङ्क cmd_mktag(पूर्णाङ्क argc,
	      अचर वर्ण **argv,
	      अचर वर्ण *prefix,
	      संरचना repository *repo)
{
	स्थिर संरचना option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_END(),
	};
	संरचना strbuf buf = STRBUF_INIT;
	संरचना object_id tagged_oid;
	पूर्णाङ्क tagged_type;
	संरचना object_id result;

	argc = parse_options(argc, argv, prefix,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	यदि (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_set_सन्देश_type_from_ids(&fsck_options, FSCK_सन्देश_EXTRA_HEADER_ENTRY,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	यदि (fsck_tag_standalone(रिक्त, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	यदि (verify_object_in_tag(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	यदि (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_TAG, &result) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	माला_दो(oid_to_hex(&result));
	फर्काऊ 0;
}
