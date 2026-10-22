#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "replace-object.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "fsck.h"
#অন্তর্ভাব "config.h"

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_mktag_usage[] = {
	"git mktag",
	ফাঁকা
};
স্থির পূর্ণসংখ্যা option_strict = 1;

স্থির কাঠামো fsck_options fsck_options;

স্থির পূর্ণসংখ্যা mktag_fsck_error_func(কাঠামো fsck_options *o UNUSED,
				 শূন্য *fsck_report UNUSED,
				 গণনা fsck_বার্তা_type বার্তা_type,
				 গণনা fsck_বার্তা_id বার্তা_id UNUSED,
				 ধ্রুবক অক্ষর *message)
{
	নির্বাচন (বার্তা_type) {
	ক্ষেত্র FSCK_WARN:
		যদি (!option_strict) {
			fprintf_ln(stderr, _("warning: tag input does not pass fsck: %s"), message);
			ফেরত 0;

		}
		/* fallthrough */
	ক্ষেত্র FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(stderr, _("error: tag input does not pass fsck: %s"), message);
		ফেরত 1;
	পূর্বনির্ধারিত:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    বার্তা_type);
	}
}

স্থির পূর্ণসংখ্যা verify_object_in_tag(কাঠামো object_id *tagged_oid, পূর্ণসংখ্যা *tagged_type)
{
	পূর্ণসংখ্যা ret;
	গণনা object_type type;
	আকার_t আকার_2;
	শূন্য *বাফার;
	ধ্রুবক কাঠামো object_id *repl;

	বাফার = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &আকার_2);
	যদি (!বাফার)
		die(_("could not read tagged object '%s'"),
		    oid_to_hex(tagged_oid));
	যদি (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_to_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, বাফার, আকার_2,
				     *tagged_type);
	মুক্ত(বাফার);

	ফেরত ret;
}

পূর্ণসংখ্যা cmd_mktag(পূর্ণসংখ্যা argc,
	      ধ্রুবক অক্ষর **argv,
	      ধ্রুবক অক্ষর *prefix,
	      কাঠামো repository *repo)
{
	স্থির কাঠামো option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_END(),
	};
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো object_id tagged_oid;
	পূর্ণসংখ্যা tagged_type;
	কাঠামো object_id ফলাফল;

	argc = parse_options(argc, argv, prefix,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	যদি (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_set_বার্তা_type_from_ids(&fsck_options, FSCK_বার্তা_EXTRA_HEADER_ENTRY,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	যদি (fsck_tag_standalone(ফাঁকা, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	যদি (verify_object_in_tag(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	যদি (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_TAG, &ফলাফল) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	puts(oid_to_hex(&ফলাফল));
	ফেরত 0;
}
