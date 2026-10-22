#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "parse-options.h"
#ضمن "strbuf.h"
#ضمن "replace-object.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "fsck.h"
#ضمن "config.h"

ساكن حرف ثابت * ثابت builtin_mktag_usage[] = {
	"git mktag",
	عدم
};
ساكن صحيح option_strict = 1;

ساكن بنية fsck_options fsck_options;

ساكن صحيح mktag_fsck_error_func(بنية fsck_options *o استفادهنشده,
				 فراغ *fsck_report استفادهنشده,
				 تعداد fsck_پیام_type پیام_type,
				 تعداد fsck_پیام_id پیام_id_2 استفادهنشده,
				 ثابت حرف *پیام_2)
{
	بدل (پیام_type) {
	حالة FSCK_WARN:
		إذا (!option_strict) {
			fprintf_ln(stderr, _("warning: tag input does not pass fsck: %s"), پیام_2);
			أرجع 0;

		}
		/* fallthrough */
	حالة FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(stderr, _("error: tag input does not pass fsck: %s"), پیام_2);
		أرجع 1;
	افتراضي:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    پیام_type);
	}
}

ساكن صحيح verify_object_درون_برچسب(بنية object_id *tagged_oid, صحيح *tagged_type)
{
	صحيح ret;
	تعداد object_type type;
	اندازه_t اندازه;
	فراغ *میانگیر;
	ثابت بنية object_id *repl;

	میانگیر = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &اندازه);
	إذا (!میانگیر)
		die(_("could not read tagged object '%s'"),
		    oid_مقصد_hex(tagged_oid));
	إذا (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_مقصد_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, میانگیر, اندازه,
				     *tagged_type);
	حرر(میانگیر);

	أرجع ret;
}

صحيح cmd_mktag(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *پیشوند,
	      بنية repository *repo)
{
	ساكن بنية option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_انتها(),
	};
	بنية strbuf buf = STRBUF_INIT;
	بنية object_id tagged_oid;
	صحيح tagged_type;
	بنية object_id نتیجه;

	argc = parse_options(argc, argv, پیشوند,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	إذا (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_مجموعه_پیام_type_from_ids(&fsck_options, FSCK_پیام_EXTRA_سرآیند_مدخل,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	إذا (fsck_برچسب_standalone(عدم, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	إذا (verify_object_درون_برچسب(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	إذا (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_برچسب, &نتیجه) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	puts(oid_مقصد_hex(&نتیجه));
	أرجع 0;
}
