#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "replace-object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fsck.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_mktag_usage[] = {
	"git mktag",
	ਨਿਰਾ
};
ਸਥਿਰ ਪੂਰਨ option_strict = 1;

ਸਥਿਰ ਬਣਤਰ fsck_options fsck_options;

ਸਥਿਰ ਪੂਰਨ mktag_fsck_error_func(ਬਣਤਰ fsck_options *o ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ *fsck_report ਅਣਵਰਤਿਆ,
				 ਗਿਣਤੀ fsck_ਸੁਨੇਹਾ_type ਸੁਨੇਹਾ_type,
				 ਗਿਣਤੀ fsck_ਸੁਨੇਹਾ_id ਸੁਨੇਹਾ_id_2 ਅਣਵਰਤਿਆ,
				 ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_2)
{
	ਚੋਣ (ਸੁਨੇਹਾ_type) {
	ਹਾਲਤ FSCK_WARN:
		ਜੇ (!option_strict) {
			fprintf_ln(stderr, _("warning: tag input does not pass fsck: %s"), ਸੁਨੇਹਾ_2);
			ਵਾਪਸ 0;

		}
		/* fallthrough */
	ਹਾਲਤ FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(stderr, _("error: tag input does not pass fsck: %s"), ਸੁਨੇਹਾ_2);
		ਵਾਪਸ 1;
	ਮੂਲ:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    ਸੁਨੇਹਾ_type);
	}
}

ਸਥਿਰ ਪੂਰਨ verify_object_ਵਿੱਚ_ਟੈਗ(ਬਣਤਰ object_id *tagged_oid, ਪੂਰਨ *tagged_type)
{
	ਪੂਰਨ ret;
	ਗਿਣਤੀ object_type type;
	ਮਾਪ_t ਮਾਪ;
	ਖਾਲੀ *ਬਫਰ;
	ਅਚਲ ਬਣਤਰ object_id *repl;

	ਬਫਰ = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &ਮਾਪ);
	ਜੇ (!ਬਫਰ)
		die(_("could not read tagged object '%s'"),
		    oid_ਮੰਜ਼ਿਲ_hex(tagged_oid));
	ਜੇ (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_ਮੰਜ਼ਿਲ_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, ਬਫਰ, ਮਾਪ,
				     *tagged_type);
	ਮੁਕਤ(ਬਫਰ);

	ਵਾਪਸ ret;
}

ਪੂਰਨ cmd_mktag(ਪੂਰਨ argc,
	      ਅਚਲ ਅੱਖਰ **argv,
	      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	      ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਬਣਤਰ option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ object_id tagged_oid;
	ਪੂਰਨ tagged_type;
	ਬਣਤਰ object_id ਨਤੀਜਾ;

	argc = parse_options(argc, argv, ਅਗੇਤਰ,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	ਜੇ (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_ਸੈੱਟ_ਸੁਨੇਹਾ_type_from_ids(&fsck_options, FSCK_ਸੁਨੇਹਾ_EXTRA_ਸਿਰਲੇਖ_ਇੰਦਰਾਜ,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	ਜੇ (fsck_ਟੈਗ_standalone(ਨਿਰਾ, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	ਜੇ (verify_object_ਵਿੱਚ_ਟੈਗ(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	ਜੇ (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_ਟੈਗ, &ਨਤੀਜਾ) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	puts(oid_ਮੰਜ਼ਿਲ_hex(&ਨਤੀਜਾ));
	ਵਾਪਸ 0;
}
