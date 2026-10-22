#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "gettext.h"
#include "hex.h"
#include "parse-options.h"
#include "strbuf.h"
#include "replace-object.h"
#include "object-file.h"
#include "odb.h"
#include "fsck.h"
#include "config.h"

static var_na const * const builtin_mktag_usage[] = {
	"git mktag",
	NULL
};
static puur_naa_nka option_strict = 1;

static sa_nracanaa fsck_options fsck_options;

static puur_naa_nka mktag_fsck_error_func(sa_nracanaa fsck_options *o UNUSED,
				 void *fsck_report UNUSED,
				 ga_nanaa fsck_msg_type msg_type,
				 ga_nanaa fsck_msg_id msg_id UNUSED,
				 const var_na *message)
{
	switch (msg_type) {
	pak_sa FSCK_WARN:
		ꦪꦼꦤ꧀ (!option_strict) {
			fprintf_ln(stderr, _("warning: tag input does not pass fsck: %s"), message);
			ꦮꦭꦶ 0;

		}
		/* fallthrough */
	pak_sa FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(stderr, _("error: tag input does not pass fsck: %s"), message);
		ꦮꦭꦶ 1;
	default:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    msg_type);
	}
}

static puur_naa_nka verify_object_in_tag(sa_nracanaa object_id *tagged_oid, puur_naa_nka *tagged_type)
{
	puur_naa_nka ret;
	ga_nanaa object_type type;
	size_t size;
	void *buffer;
	const sa_nracanaa object_id *repl;

	buffer = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &size);
	ꦪꦼꦤ꧀ (!buffer)
		die(_("could not read tagged object '%s'"),
		    oid_to_hex(tagged_oid));
	ꦪꦼꦤ꧀ (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_to_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, buffer, size,
				     *tagged_type);
	free(buffer);

	ꦮꦭꦶ ret;
}

puur_naa_nka cmd_mktag(puur_naa_nka argc,
	      const var_na **argv,
	      const var_na *prefix,
	      sa_nracanaa repository *repo)
{
	static sa_nracanaa option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_END(),
	};
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa object_id tagged_oid;
	puur_naa_nka tagged_type;
	sa_nracanaa object_id result;

	argc = parse_options(argc, argv, prefix,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	ꦪꦼꦤ꧀ (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_set_msg_type_from_ids(&fsck_options, FSCK_MSG_EXTRA_HEADER_ENTRY,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	ꦪꦼꦤ꧀ (fsck_tag_standalone(NULL, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	ꦪꦼꦤ꧀ (verify_object_in_tag(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	ꦪꦼꦤ꧀ (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_TAG, &result) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	puts(oid_to_hex(&result));
	ꦮꦭꦶ 0;
}
