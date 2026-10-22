#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "replace-object.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "fsck.h"
#అన్తర్భావ "config.h"

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_mktag_usage[] = {
	"git mktag",
	ఖాళీ
};
స్థిర పూర్ణాంకం option_strict = 1;

స్థిర నిర్మాణం fsck_options fsck_options;

స్థిర పూర్ణాంకం mktag_fsck_error_func(నిర్మాణం fsck_options *o వాడని,
				 శూన్య *fsck_report వాడని,
				 గణన fsck_సందేశం_type సందేశం_type,
				 గణన fsck_సందేశం_id సందేశం_id_2 వాడని,
				 స్థిరరాశి అక్షరం *సందేశం_2)
{
	ఎంపిక (సందేశం_type) {
	స్థితి FSCK_WARN:
		ఒకవేళ (!option_strict) {
			fprintf_ln(stderr, _("warning: tag input does not pass fsck: %s"), సందేశం_2);
			తిరిగి 0;

		}
		/* fallthrough */
	స్థితి FSCK_ERROR:
		/*
		 * We treat both warnings and errors as errors, things
		 * like missing "tagger" lines are "only" warnings
		 * under fsck, we've always considered them an error.
		 */
		fprintf_ln(stderr, _("error: tag input does not pass fsck: %s"), సందేశం_2);
		తిరిగి 1;
	అప్రమేయ:
		BUG("%d (FSCK_IGNORE?) should never trigger this callback",
		    సందేశం_type);
	}
}

స్థిర పూర్ణాంకం verify_object_లోపల_ట్యాగ్(నిర్మాణం object_id *tagged_oid, పూర్ణాంకం *tagged_type)
{
	పూర్ణాంకం ret;
	గణన object_type type;
	కొలత_t కొలత;
	శూన్య *బఫరు;
	స్థిరరాశి నిర్మాణం object_id *repl;

	బఫరు = odb_read_object(the_repository->objects, tagged_oid,
				 &type, &కొలత);
	ఒకవేళ (!బఫరు)
		die(_("could not read tagged object '%s'"),
		    oid_గమ్యం_hex(tagged_oid));
	ఒకవేళ (type != *tagged_type)
		die(_("object '%s' tagged as '%s', but is a '%s' type"),
		    oid_గమ్యం_hex(tagged_oid),
		    type_name(*tagged_type), type_name(type));

	repl = lookup_replace_object(the_repository, tagged_oid);
	ret = check_object_signature(the_repository, repl, బఫరు, కొలత,
				     *tagged_type);
	విడుదల(బఫరు);

	తిరిగి ret;
}

పూర్ణాంకం cmd_mktag(పూర్ణాంకం argc,
	      స్థిరరాశి అక్షరం **argv,
	      స్థిరరాశి అక్షరం *ఉపసర్గ,
	      నిర్మాణం repository *repo)
{
	స్థిర నిర్మాణం option builtin_mktag_options[] = {
		OPT_BOOL(0, "strict", &option_strict,
			 N_("enable more strict checking")),
		OPT_చివర(),
	};
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం object_id tagged_oid;
	పూర్ణాంకం tagged_type;
	నిర్మాణం object_id ఫలితం;

	argc = parse_options(argc, argv, ఉపసర్గ,
			     builtin_mktag_options,
			     builtin_mktag_usage, 0);

	ఒకవేళ (strbuf_read(&buf, 0, 0) < 0)
		die_errno(_("could not read from stdin"));

	fsck_options_init(&fsck_options, repo, FSCK_OPTIONS_STRICT);
	fsck_options.error_func = mktag_fsck_error_func;
	fsck_సమితి_సందేశం_type_from_ids(&fsck_options, FSCK_సందేశం_EXTRA_శీర్షిక_నమోదు,
				   FSCK_WARN);
	/* config might set fsck.extraHeaderEntry=* again */
	repo_config(the_repository, git_fsck_config, &fsck_options);
	ఒకవేళ (fsck_ట్యాగ్_standalone(ఖాళీ, buf.buf, buf.len, &fsck_options,
				&tagged_oid, &tagged_type))
		die(_("tag on stdin did not pass our strict fsck check"));

	ఒకవేళ (verify_object_లోపల_ట్యాగ్(&tagged_oid, &tagged_type) < 0)
		die(_("tag on stdin did not refer to a valid object"));

	ఒకవేళ (odb_write_object(the_repository->objects, buf.buf, buf.len, OBJ_ట్యాగ్, &ఫలితం) < 0)
		die(_("unable to write tag file"));

	strbuf_release(&buf);
	puts(oid_గమ్యం_hex(&ఫలితం));
	తిరిగి 0;
}
