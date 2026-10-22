#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "replace-object.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "fsck.h"
#અન્તર્ભાવ "config.h"

સ્થિર અક્ષર અચલ * અચલ builtin_mktag_usage[] = {
	"git mktag",
	ખાલી
};
સ્થિર પૂર્ણાંક option_strict = 1;

સ્થિર સંરચના fsck_options fsck_options;

સ્થિર પૂર્ણાંક mktag_fsck_error_func(સંરચના fsck_options *o UNUSED,
				 શૂન્ય *fsck_report UNUSED,
				 ગણના fsck_સંદેશ_type સંદેશ_type,
				 ગણના fsck_સંદેશ_id સંદેશ_id UNUSED,
				 અચલ અક્ષર *message)
{
	પસંદગી (સંદેશ_type) {
	સ્થિતિ FSCK_WARN:
		જો (!option_strict) {
			fprintf_ln(stderr, _("warning: tag input does not pass fsck: %s"), message);
			પાછા 0;

		}
		/* fallthrough */
	સ્થિતિ FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(stderr, _("error: tag input does not pass fsck: %s"), message);
		પાછા 1;
	મૂળભૂત:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    સંદેશ_type);
	}
}

સ્થિર પૂર્ણાંક verify_object_in_tag(સંરચના object_id *tagged_oid, પૂર્ણાંક *tagged_type)
{
	પૂર્ણાંક ret;
	ગણના object_type type;
	size_t size;
	શૂન્ય *buffer;
	અચલ સંરચના object_id *repl;

	buffer = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &size);
	જો (!buffer)
		die(_("could not read tagged object '%s'"),
		    oid_to_hex(tagged_oid));
	જો (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_to_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, buffer, size,
				     *tagged_type);
	મુક્ત(buffer);

	પાછા ret;
}

પૂર્ણાંક cmd_mktag(પૂર્ણાંક argc,
	      અચલ અક્ષર **argv,
	      અચલ અક્ષર *prefix,
	      સંરચના repository *repo)
{
	સ્થિર સંરચના option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_END(),
	};
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના object_id tagged_oid;
	પૂર્ણાંક tagged_type;
	સંરચના object_id result;

	argc = parse_options(argc, argv, prefix,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	જો (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_set_સંદેશ_type_from_ids(&fsck_options, FSCK_સંદેશ_EXTRA_HEADER_ENTRY,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	જો (fsck_tag_standalone(ખાલી, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	જો (verify_object_in_tag(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	જો (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_TAG, &result) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	puts(oid_to_hex(&result));
	પાછા 0;
}
