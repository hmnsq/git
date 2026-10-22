#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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

ساکن حرف ثابت * ثابت builtin_mktag_usage[] = {
	"git mktag",
	تہی
};
ساکن صحیح option_strict = 1;

ساکن ساخت fsck_options fsck_options;

ساکن صحیح mktag_fsck_error_func(ساخت fsck_options *o غیر_مستعمل,
				 خالی *fsck_report غیر_مستعمل,
				 شمار fsck_پیغام_type پیغام_type,
				 شمار fsck_پیغام_id پیغام_id_2 غیر_مستعمل,
				 ثابت حرف *پیغام_2)
{
	انتخاب (پیغام_type) {
	حالت FSCK_WARN:
		اگر (!option_strict) {
			fprintf_ln(stderr, _("warning: tag input does not pass fsck: %s"), پیغام_2);
			واپس 0;

		}
		/* fallthrough */
	حالت FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(stderr, _("error: tag input does not pass fsck: %s"), پیغام_2);
		واپس 1;
	طےشدہ:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    پیغام_type);
	}
}

ساکن صحیح verify_object_اندر_لیبل(ساخت object_id *tagged_oid, صحیح *tagged_type)
{
	صحیح ret;
	شمار object_type type;
	سائز_t سائز;
	خالی *بفر;
	ثابت ساخت object_id *repl;

	بفر = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &سائز);
	اگر (!بفر)
		die(_("could not read tagged object '%s'"),
		    oid_مقصد_hex(tagged_oid));
	اگر (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_مقصد_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, بفر, سائز,
				     *tagged_type);
	آزاد(بفر);

	واپس ret;
}

صحیح cmd_mktag(صحیح argc,
	      ثابت حرف **argv,
	      ثابت حرف *سابقہ,
	      ساخت repository *repo)
{
	ساکن ساخت option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_انتہا(),
	};
	ساخت strbuf buf = STRBUF_INIT;
	ساخت object_id tagged_oid;
	صحیح tagged_type;
	ساخت object_id نتیجہ;

	argc = parse_options(argc, argv, سابقہ,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	اگر (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_سیٹ_پیغام_type_from_ids(&fsck_options, FSCK_پیغام_EXTRA_سرخی_اندراج,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	اگر (fsck_لیبل_standalone(تہی, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	اگر (verify_object_اندر_لیبل(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	اگر (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_لیبل, &نتیجہ) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	puts(oid_مقصد_hex(&نتیجہ));
	واپس 0;
}
