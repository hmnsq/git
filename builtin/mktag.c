#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "replace-object.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "fsck.h"
#അന്തര്ഭാവ "config.h"

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_mktag_usage[] = {
	"git mktag",
	ശൂന്യ
};
സ്ഥിതം പൂർണ്ണസംഖ്യ option_strict = 1;

സ്ഥിതം ഘടന fsck_options fsck_options;

സ്ഥിതം പൂർണ്ണസംഖ്യ mktag_fsck_error_func(ഘടന fsck_options *o UNUSED,
				 ശൂന്യം *fsck_report UNUSED,
				 എണ്ണൽ fsck_സന്ദേശം_type സന്ദേശം_type,
				 എണ്ണൽ fsck_സന്ദേശം_id സന്ദേശം_id UNUSED,
				 സ്ഥിരം അക്ഷരം *message)
{
	തിരഞ്ഞെടുപ്പ് (സന്ദേശം_type) {
	അവസ്ഥ FSCK_WARN:
		എങ്കിൽ (!option_strict) {
			fprintf_ln(stderr, _("warning: tag input does not pass fsck: %s"), message);
			തിരികെ 0;

		}
		/* fallthrough */
	അവസ്ഥ FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(stderr, _("error: tag input does not pass fsck: %s"), message);
		തിരികെ 1;
	സ്ഥിരസ്ഥിതി:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    സന്ദേശം_type);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ verify_object_in_tag(ഘടന object_id *tagged_oid, പൂർണ്ണസംഖ്യ *tagged_type)
{
	പൂർണ്ണസംഖ്യ ret;
	എണ്ണൽ object_type type;
	size_t size;
	ശൂന്യം *buffer;
	സ്ഥിരം ഘടന object_id *repl;

	buffer = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &size);
	എങ്കിൽ (!buffer)
		die(_("could not read tagged object '%s'"),
		    oid_to_hex(tagged_oid));
	എങ്കിൽ (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_to_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, buffer, size,
				     *tagged_type);
	സ്വതന്ത്രം(buffer);

	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ cmd_mktag(പൂർണ്ണസംഖ്യ argc,
	      സ്ഥിരം അക്ഷരം **argv,
	      സ്ഥിരം അക്ഷരം *prefix,
	      ഘടന repository *repo)
{
	സ്ഥിതം ഘടന option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_END(),
	};
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന object_id tagged_oid;
	പൂർണ്ണസംഖ്യ tagged_type;
	ഘടന object_id result;

	argc = parse_options(argc, argv, prefix,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	എങ്കിൽ (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_set_സന്ദേശം_type_from_ids(&fsck_options, FSCK_സന്ദേശം_EXTRA_HEADER_ENTRY,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	എങ്കിൽ (fsck_tag_standalone(ശൂന്യ, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	എങ്കിൽ (verify_object_in_tag(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	എങ്കിൽ (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_TAG, &result) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	puts(oid_to_hex(&result));
	തിരികെ 0;
}
