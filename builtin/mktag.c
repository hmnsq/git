#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "parse-options.h"
#समावेश "strbuf.h"
#समावेश "replace-object.h"
#समावेश "object-file.h"
#समावेश "odb.h"
#समावेश "fsck.h"
#समावेश "config.h"

अटल अक्षर स्थिर * स्थिर builtin_mktag_usage[] = {
	"git mktag",
	शून्य
};
अटल पूर्णांक option_strict = 1;

अटल काष्ठा fsck_options fsck_options;

अटल पूर्णांक mktag_fsck_error_func(काष्ठा fsck_options *o UNUSED,
				 व्योम *fsck_report UNUSED,
				 क्रमागत fsck_संदेश_type संदेश_type,
				 क्रमागत fsck_संदेश_id संदेश_id UNUSED,
				 स्थिर अक्षर *message)
{
	चयन (संदेश_type) {
	हाल FSCK_WARN:
		अगर (!option_strict) {
			fprintf_ln(मानक_त्रुटि, _("warning: tag input does not pass fsck: %s"), message);
			वापस 0;

		}
		/* fallthrough */
	हाल FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(मानक_त्रुटि, _("error: tag input does not pass fsck: %s"), message);
		वापस 1;
	शेष:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    संदेश_type);
	}
}

अटल पूर्णांक verify_object_in_tag(काष्ठा object_id *tagged_oid, पूर्णांक *tagged_type)
{
	पूर्णांक ret;
	क्रमागत object_type type;
	माप_प्रकार आकार;
	व्योम *बफर;
	स्थिर काष्ठा object_id *repl;

	बफर = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &आकार);
	अगर (!बफर)
		die(_("could not read tagged object '%s'"),
		    oid_to_hex(tagged_oid));
	अगर (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_to_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, बफर, आकार,
				     *tagged_type);
	मुक्त(बफर);

	वापस ret;
}

पूर्णांक cmd_mktag(पूर्णांक argc,
	      स्थिर अक्षर **argv,
	      स्थिर अक्षर *prefix,
	      काष्ठा repository *repo)
{
	अटल काष्ठा option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_END(),
	};
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा object_id tagged_oid;
	पूर्णांक tagged_type;
	काष्ठा object_id परिणाम;

	argc = parse_options(argc, argv, prefix,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	अगर (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_set_संदेश_type_from_ids(&fsck_options, FSCK_संदेश_EXTRA_HEADER_ENTRY,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	अगर (fsck_tag_standalone(शून्य, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	अगर (verify_object_in_tag(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	अगर (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_TAG, &परिणाम) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	माला_दो(oid_to_hex(&परिणाम));
	वापस 0;
}
