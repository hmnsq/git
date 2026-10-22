#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "replace-object.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "fsck.h"
#ಅನ್ತರ್ಭಾವ "config.h"

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_mktag_usage[] = {
	"git mktag",
	ಖಾಲಿ
};
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_strict = 1;

ಸ್ಥಿರ ರಚನೆ fsck_options fsck_options;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mktag_fsck_error_func(ರಚನೆ fsck_options *o ಬಳಸದ,
				 ಶೂನ್ಯ *fsck_report ಬಳಸದ,
				 ಎಣಿಕೆ fsck_ಸಂದೇಶ_type ಸಂದೇಶ_type,
				 ಎಣಿಕೆ fsck_ಸಂದೇಶ_id ಸಂದೇಶ_id_2 ಬಳಸದ,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2)
{
	ಆಯ್ಕೆ (ಸಂದೇಶ_type) {
	ಸ್ಥಿತಿ FSCK_WARN:
		ಆದರೆ (!option_strict) {
			fprintf_ln(stderr, _("warning: tag input does not pass fsck: %s"), ಸಂದೇಶ_2);
			ಹಿಂತಿರುಗಿ 0;

		}
		/* fallthrough */
	ಸ್ಥಿತಿ FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(stderr, _("error: tag input does not pass fsck: %s"), ಸಂದೇಶ_2);
		ಹಿಂತಿರುಗಿ 1;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    ಸಂದೇಶ_type);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verify_object_ಒಳ_ಟ್ಯಾಗ್(ರಚನೆ object_id *tagged_oid, ಪೂರ್ಣಾಂಕ *tagged_type)
{
	ಪೂರ್ಣಾಂಕ ret;
	ಎಣಿಕೆ object_type type;
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
	ಶೂನ್ಯ *ಬಫರ್;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *repl;

	ಬಫರ್ = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &ಗಾತ್ರ_2);
	ಆದರೆ (!ಬಫರ್)
		die(_("could not read tagged object '%s'"),
		    oid_ಗಮ್ಯ_hex(tagged_oid));
	ಆದರೆ (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_ಗಮ್ಯ_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, ಬಫರ್, ಗಾತ್ರ_2,
				     *tagged_type);
	ಬಿಡುಗಡೆ(ಬಫರ್);

	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ cmd_mktag(ಪೂರ್ಣಾಂಕ argc,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	      ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ರಚನೆ option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ object_id tagged_oid;
	ಪೂರ್ಣಾಂಕ tagged_type;
	ರಚನೆ object_id ಫಲಿತಾಂಶ;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	ಆದರೆ (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_ಗಣ_ಸಂದೇಶ_type_from_ids(&fsck_options, FSCK_ಸಂದೇಶ_EXTRA_ಶೀರ್ಷಿಕೆ_ನಮೂದು,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	ಆದರೆ (fsck_ಟ್ಯಾಗ್_standalone(ಖಾಲಿ, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	ಆದರೆ (verify_object_ಒಳ_ಟ್ಯಾಗ್(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	ಆದರೆ (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_ಟ್ಯಾಗ್, &ಫಲಿತಾಂಶ) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	puts(oid_ಗಮ್ಯ_hex(&ಫಲಿತಾಂಶ));
	ಹಿಂತಿರುಗಿ 0;
}
