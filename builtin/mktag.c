#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "parse-options.h"
#شامل "strbuf.h"
#شامل "replace-object.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "fsck.h"
#شامل "config.h"

ایستا نویسه ثابت * ثابت builtin_mktag_usage[] = {
	"git mktag",
	پوچ
};
ایستا صحیح option_strict = 1;

ایستا ساختار fsck_options fsck_options;

ایستا صحیح mktag_fsck_error_func(ساختار fsck_options *o UNUSED,
				 تهی *fsck_report UNUSED,
				 شمارش fsck_پیام_type پیام_type,
				 شمارش fsck_پیام_id پیام_id UNUSED,
				 ثابت نویسه *message)
{
	انتخاب (پیام_type) {
	حالت FSCK_WARN:
		اگر (!option_strict) {
			fprintf_ln(stderr, _("warning: tag input does not pass fsck: %s"), message);
			بازگشت 0;

		}
		/* fallthrough */
	حالت FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(stderr, _("error: tag input does not pass fsck: %s"), message);
		بازگشت 1;
	پیشفرض:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    پیام_type);
	}
}

ایستا صحیح verify_object_in_tag(ساختار object_id *tagged_oid, صحیح *tagged_type)
{
	صحیح ret;
	شمارش object_type type;
	size_t size;
	تهی *buffer;
	ثابت ساختار object_id *repl;

	buffer = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &size);
	اگر (!buffer)
		die(_("could not read tagged object '%s'"),
		    oid_to_hex(tagged_oid));
	اگر (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_to_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, buffer, size,
				     *tagged_type);
	آزاد(buffer);

	بازگشت ret;
}

صحیح cmd_mktag(صحیح argc,
	      ثابت نویسه **argv,
	      ثابت نویسه *prefix,
	      ساختار repository *repo)
{
	ایستا ساختار option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_END(),
	};
	ساختار strbuf buf = STRBUF_INIT;
	ساختار object_id tagged_oid;
	صحیح tagged_type;
	ساختار object_id result;

	argc = parse_options(argc, argv, prefix,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	اگر (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_set_پیام_type_from_ids(&fsck_options, FSCK_پیام_EXTRA_HEADER_ENTRY,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	اگر (fsck_tag_standalone(پوچ, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	اگر (verify_object_in_tag(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	اگر (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_TAG, &result) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	puts(oid_to_hex(&result));
	بازگشت 0;
}
