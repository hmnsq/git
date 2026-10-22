#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "replace-object.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "fsck.h"
#ଅନ୍ତର୍ଭାଵ "config.h"

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_mktag_usage[] = {
	"git mktag",
	ଖାଲି
};
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_strict = 1;

ସ୍ଥିର ସଂରଚନା fsck_options fsck_options;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mktag_fsck_error_func(ସଂରଚନା fsck_options *o UNUSED,
				 ଶୂନ୍ୟ *fsck_report UNUSED,
				 ଗଣନା fsck_ସନ୍ଦେଶ_type ସନ୍ଦେଶ_type,
				 ଗଣନା fsck_ସନ୍ଦେଶ_id ସନ୍ଦେଶ_id UNUSED,
				 ଅଚଳ ଅକ୍ଷର *message)
{
	ବାଛ (ସନ୍ଦେଶ_type) {
	ସ୍ଥିତି FSCK_WARN:
		ଯଦି (!option_strict) {
			fprintf_ln(stderr, _("warning: tag input does not pass fsck: %s"), message);
			ଫେରନ୍ତୁ 0;

		}
		/* fallthrough */
	ସ୍ଥିତି FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(stderr, _("error: tag input does not pass fsck: %s"), message);
		ଫେରନ୍ତୁ 1;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    ସନ୍ଦେଶ_type);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verify_object_in_tag(ସଂରଚନା object_id *tagged_oid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *tagged_type)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ଗଣନା object_type type;
	size_t size;
	ଶୂନ୍ୟ *buffer;
	ଅଚଳ ସଂରଚନା object_id *repl;

	buffer = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &size);
	ଯଦି (!buffer)
		die(_("could not read tagged object '%s'"),
		    oid_to_hex(tagged_oid));
	ଯଦି (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_to_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, buffer, size,
				     *tagged_type);
	ମୁକ୍ତ(buffer);

	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_mktag(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	      ଅଚଳ ଅକ୍ଷର **argv,
	      ଅଚଳ ଅକ୍ଷର *prefix,
	      ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ସଂରଚନା option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_END(),
	};
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା object_id tagged_oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା tagged_type;
	ସଂରଚନା object_id result;

	argc = parse_options(argc, argv, prefix,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	ଯଦି (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_set_ସନ୍ଦେଶ_type_from_ids(&fsck_options, FSCK_ସନ୍ଦେଶ_EXTRA_HEADER_ENTRY,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	ଯଦି (fsck_tag_standalone(ଖାଲି, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	ଯଦି (verify_object_in_tag(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	ଯଦି (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_TAG, &result) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	puts(oid_to_hex(&result));
	ଫେରନ୍ତୁ 0;
}
