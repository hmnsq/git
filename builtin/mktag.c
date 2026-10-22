#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "replace-object.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "fsck.h"
#அந்தர்பாவ "config.h"

நிலையான எழுத்து மாறிலி * மாறிலி builtin_mktag_usage[] = {
	"git mktag",
	வெற்று
};
நிலையான முழு option_strict = 1;

நிலையான கட்டமைப்பு fsck_options fsck_options;

நிலையான முழு mktag_fsck_error_func(கட்டமைப்பு fsck_options *o UNUSED,
				 வெறுமை *fsck_report UNUSED,
				 எண்ணிக்கை fsck_செய்தி_type செய்தி_type,
				 எண்ணிக்கை fsck_செய்தி_id செய்தி_id UNUSED,
				 மாறிலி எழுத்து *message)
{
	தேர்வு (செய்தி_type) {
	நிலை FSCK_WARN:
		எனில் (!option_strict) {
			fprintf_ln(stderr, _("warning: tag input does not pass fsck: %s"), message);
			திரும்பு 0;

		}
		/* fallthrough */
	நிலை FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(stderr, _("error: tag input does not pass fsck: %s"), message);
		திரும்பு 1;
	இயல்பு:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    செய்தி_type);
	}
}

நிலையான முழு verify_object_in_tag(கட்டமைப்பு object_id *tagged_oid, முழு *tagged_type)
{
	முழு ret;
	எண்ணிக்கை object_type type;
	size_t size;
	வெறுமை *buffer;
	மாறிலி கட்டமைப்பு object_id *repl;

	buffer = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &size);
	எனில் (!buffer)
		die(_("could not read tagged object '%s'"),
		    oid_to_hex(tagged_oid));
	எனில் (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_to_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, buffer, size,
				     *tagged_type);
	விடுவி(buffer);

	திரும்பு ret;
}

முழு cmd_mktag(முழு argc,
	      மாறிலி எழுத்து **argv,
	      மாறிலி எழுத்து *prefix,
	      கட்டமைப்பு repository *repo)
{
	நிலையான கட்டமைப்பு option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_END(),
	};
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு object_id tagged_oid;
	முழு tagged_type;
	கட்டமைப்பு object_id result;

	argc = parse_options(argc, argv, prefix,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	எனில் (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_set_செய்தி_type_from_ids(&fsck_options, FSCK_செய்தி_EXTRA_HEADER_ENTRY,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	எனில் (fsck_tag_standalone(வெற்று, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	எனில் (verify_object_in_tag(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	எனில் (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_TAG, &result) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	puts(oid_to_hex(&result));
	திரும்பு 0;
}
