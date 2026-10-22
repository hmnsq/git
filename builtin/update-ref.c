#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "quote.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	ఖాళీ
};

స్థిర అక్షరం పంక్తి_termination = '\n';
స్థిర గుర్తులేని పూర్ణాంకం update_flags;
స్థిర గుర్తులేని పూర్ణాంకం default_flags;
స్థిర గుర్తులేని create_reflog_flag;
స్థిర స్థిరరాశి అక్షరం *సందేశం;

నిర్మాణం command_options {
	/*
	 * Individual updates are allowed to fail without causing
	 * update-ref to exit. This is set when using the
	 * '--batch-updates' flag.
	 */
	bool allow_update_failures;
};

/*
 * Parse one whitespace- or NUL-terminated, possibly C-quoted argument
 * and append the result to arg.  Return a pointer to the terminator.
 * Die if there is an error in how the argument is C-quoted.  This
 * function is only used if not -z.
 */
స్థిర స్థిరరాశి అక్షరం *parse_arg(స్థిరరాశి అక్షరం *తదుపరి, నిర్మాణం strbuf *arg)
{
	ఒకవేళ (*తదుపరి == '"') {
		స్థిరరాశి అక్షరం *orig = తదుపరి;

		ఒకవేళ (unquote_c_style(arg, తదుపరి, &తదుపరి))
			die("badly quoted argument: %s", orig);
		ఒకవేళ (*తదుపరి && !ఖాళీనా(*తదుపరి))
			die("unexpected character after quoted argument: %s", orig);
	} లేకపోతే {
		అయితే (*తదుపరి && !ఖాళీనా(*తదుపరి))
			strbuf_addch(arg, *తదుపరి++);
	}

	తిరిగి తదుపరి;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
స్థిర అక్షరం *parse_refname(స్థిరరాశి అక్షరం **తదుపరి)
{
	నిర్మాణం strbuf ref = STRBUF_INIT;

	ఒకవేళ (పంక్తి_termination) {
		/* Without -z, use the next argument */
		*తదుపరి = parse_arg(*తదుపరి, &ref);
	} లేకపోతే {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *తదుపరి);
		*తదుపరి += ref.len;
	}

	ఒకవేళ (!ref.len) {
		strbuf_release(&ref);
		తిరిగి ఖాళీ;
	}

	ఒకవేళ (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	తిరిగి strbuf_detach(&ref, ఖాళీ);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
స్థిర అక్షరం *parse_తదుపరి_refname(స్థిరరాశి అక్షరం **తదుపరి)
{
	ఒకవేళ (పంక్తి_termination) {
		/* Without -z, consume SP and use next argument */
		ఒకవేళ (!**తదుపరి || **తదుపరి == పంక్తి_termination)
			తిరిగి ఖాళీ;
		ఒకవేళ (**తదుపరి != ' ')
			die("expected SP but got: %s", *తదుపరి);
	} లేకపోతే {
		/* With -z, read the next NUL-terminated line */
		ఒకవేళ (**తదుపరి)
			తిరిగి ఖాళీ;
	}
	/* Skip the delimiter */
	(*తదుపరి)++;

	తిరిగి parse_refname(తదుపరి);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
స్థిర అక్షరం *parse_తదుపరి_arg(స్థిరరాశి అక్షరం **తదుపరి)
{
	నిర్మాణం strbuf arg = STRBUF_INIT;

	ఒకవేళ (పంక్తి_termination) {
		/* Without -z, consume SP and use next argument */
		ఒకవేళ (!**తదుపరి || **తదుపరి == పంక్తి_termination)
			తిరిగి ఖాళీ;
		ఒకవేళ (**తదుపరి != ' ')
			die("expected SP but got: %s", *తదుపరి);
	} లేకపోతే {
		/* With -z, read the next NUL-terminated line */
		ఒకవేళ (**తదుపరి)
			తిరిగి ఖాళీ;
	}
	/* Skip the delimiter */
	(*తదుపరి)++;

	ఒకవేళ (పంక్తి_termination) {
		/* Without -z, use the next argument */
		*తదుపరి = parse_arg(*తదుపరి, &arg);
	} లేకపోతే {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *తదుపరి);
		*తదుపరి += arg.len;
	}

	ఒకవేళ (arg.len)
		తిరిగి strbuf_detach(&arg, ఖాళీ);

	strbuf_release(&arg);
	తిరిగి ఖాళీ;
}

/*
 * The value being parsed is <old-oid> (as opposed to <new-oid>; the
 * difference affects which error messages are generated):
 */
#నిర్వచించు PARSE_SHA1_OLD 0x01

/*
 * For backwards compatibility, accept an empty string for update's
 * <new-oid> in binary mode to be equivalent to specifying zeros.
 */
#నిర్వచించు PARSE_SHA1_ALLOW_ఖాళీలు 0x02

/*
 * Parse an argument separator followed by the next argument, if any.
 * If there is an argument, convert it to a SHA-1, write it to sha1,
 * set *next to point at the character terminating the argument, and
 * return 0.  If there is no argument at all (not even the empty
 * string), return 1 and leave *next unchanged.  If the value is
 * provided but cannot be converted to a SHA-1, die.  flags can
 * include PARSE_SHA1_OLD and/or PARSE_SHA1_ALLOW_EMPTY.
 */
స్థిర పూర్ణాంకం parse_తదుపరి_oid(స్థిరరాశి అక్షరం **తదుపరి, స్థిరరాశి అక్షరం *చివర,
			  నిర్మాణం object_id *oid,
			  స్థిరరాశి అక్షరం *command, స్థిరరాశి అక్షరం *refname,
			  పూర్ణాంకం flags)
{
	నిర్మాణం strbuf arg = STRBUF_INIT;
	పూర్ణాంకం ret = 0;

	ఒకవేళ (*తదుపరి == చివర)
		వెళ్ళు eof;

	ఒకవేళ (పంక్తి_termination) {
		/* Without -z, consume SP and use next argument */
		ఒకవేళ (!**తదుపరి || **తదుపరి == పంక్తి_termination)
			తిరిగి 1;
		ఒకవేళ (**తదుపరి != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *తదుపరి);
		(*తదుపరి)++;
		*తదుపరి = parse_arg(*తదుపరి, &arg);
		ఒకవేళ (arg.len) {
			ఒకవేళ (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				వెళ్ళు invalid;
		} లేకపోతే {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} లేకపోతే {
		/* With -z, read the next NUL-terminated line */
		ఒకవేళ (**తదుపరి)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *తదుపరి);
		(*తదుపరి)++;
		ఒకవేళ (*తదుపరి == చివర)
			వెళ్ళు eof;
		strbuf_addstr(&arg, *తదుపరి);
		*తదుపరి += arg.len;

		ఒకవేళ (arg.len) {
			ఒకవేళ (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				వెళ్ళు invalid;
		} లేకపోతే ఒకవేళ (flags & PARSE_SHA1_ALLOW_ఖాళీలు) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} లేకపోతే {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	తిరిగి ret;

 invalid:
	die(flags & PARSE_SHA1_OLD ?
	    "%s %s: invalid <old-oid>: %s" :
	    "%s %s: invalid <new-oid>: %s",
	    command, refname, arg.buf);

 eof:
	die(flags & PARSE_SHA1_OLD ?
	    "%s %s: unexpected end of input when reading <old-oid>" :
	    "%s %s: unexpected end of input when reading <new-oid>",
	    command, refname);
}

స్థిర శూన్య print_rejected_refs(స్థిరరాశి అక్షరం *refname,
				స్థిరరాశి నిర్మాణం object_id *old_oid,
				స్థిరరాశి నిర్మాణం object_id *new_oid,
				స్థిరరాశి అక్షరం *old_లక్ష్యం,
				స్థిరరాశి అక్షరం *new_లక్ష్యం,
				గణన ref_transaction_error err,
				స్థిరరాశి అక్షరం *details,
				శూన్య *cb_దత్తాంశం వాడని)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;

	ఒకవేళ (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_గమ్యం_hex(new_oid) : new_లక్ష్యం,
		    old_oid ? oid_గమ్యం_hex(old_oid) : old_లక్ష్యం,
		    ref_transaction_error_సందేశం(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
స్థిర శూన్య handle_ref_transaction_error(స్థిరరాశి అక్షరం *refname,
					 నిర్మాణం object_id *new_oid,
					 నిర్మాణం object_id *old_oid,
					 స్థిరరాశి అక్షరం *new_లక్ష్యం,
					 స్థిరరాశి అక్షరం *old_లక్ష్యం,
					 గణన ref_transaction_error tx_err,
					 నిర్మాణం strbuf *err,
					 నిర్మాణం command_options *opts)
{
	ఒకవేళ (!tx_err)
		తిరిగి;

	ఒకవేళ (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_లక్ష్యం,
				    new_లక్ష్యం, tx_err, err->buf, ఖాళీ);
		తిరిగి;
	}

	die("%s", err->buf);
}

/*
 * The following five parse_cmd_*() functions parse the corresponding
 * command.  In each case, next points at the character following the
 * command name and the following space.  They each return a pointer
 * to the character terminating the command, and die with an
 * explanatory message if there are any parsing problems.  All of
 * these functions handle either text or binary format input,
 * depending on how line_termination is set.
 */

స్థిర శూన్య parse_cmd_update(నిర్మాణం ref_transaction *transaction,
			     స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర,
			     నిర్మాణం command_options *opts)
{
	నిర్మాణం strbuf err = STRBUF_INIT;
	అక్షరం *refname;
	నిర్మాణం object_id new_oid, old_oid;
	గణన ref_transaction_error tx_err;
	పూర్ణాంకం have_old;

	refname = parse_refname(&తదుపరి);
	ఒకవేళ (!refname)
		die("update: missing <ref>");

	ఒకవేళ (parse_తదుపరి_oid(&తదుపరి, చివర, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_ఖాళీలు))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_తదుపరి_oid(&తదుపరి, చివర, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	ఒకవేళ (*తదుపరి != పంక్తి_termination)
		die("update %s: extra input: %s", refname, తదుపరి);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : ఖాళీ,
					ఖాళీ, ఖాళీ,
					update_flags | create_reflog_flag,
					సందేశం, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : ఖాళీ,
				     ఖాళీ, ఖాళీ, tx_err, &err, opts);


	update_flags = default_flags;
	విడుదల(refname);
	strbuf_release(&err);
}

స్థిర శూన్య parse_cmd_symref_update(నిర్మాణం ref_transaction *transaction,
				    స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర వాడని,
				    నిర్మాణం command_options *opts)
{
	అక్షరం *refname, *new_లక్ష్యం, *old_arg;
	గణన ref_transaction_error tx_err;
	అక్షరం *old_లక్ష్యం = ఖాళీ;
	నిర్మాణం strbuf err = STRBUF_INIT;
	నిర్మాణం object_id old_oid;
	పూర్ణాంకం have_old_oid = 0;

	refname = parse_refname(&తదుపరి);
	ఒకవేళ (!refname)
		die("symref-update: missing <ref>");

	new_లక్ష్యం = parse_తదుపరి_refname(&తదుపరి);
	ఒకవేళ (!new_లక్ష్యం)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_తదుపరి_arg(&తదుపరి);
	ఒకవేళ (old_arg) {
		old_లక్ష్యం = parse_తదుపరి_arg(&తదుపరి);
		ఒకవేళ (!old_లక్ష్యం)
			die("symref-update %s: expected old value", refname);

		ఒకవేళ (!strcmp(old_arg, "oid")) {
			ఒకవేళ (repo_get_oid_with_flags(the_repository, old_లక్ష్యం, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_లక్ష్యం);

			have_old_oid = 1;
		} లేకపోతే ఒకవేళ (!strcmp(old_arg, "ref")) {
			ఒకవేళ (check_refname_format(old_లక్ష్యం, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_లక్ష్యం);
		} లేకపోతే {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	ఒకవేళ (*తదుపరి != పంక్తి_termination)
		die("symref-update %s: extra input: %s", refname, తదుపరి);

	tx_err = ref_transaction_update(transaction, refname, ఖాళీ,
					have_old_oid ? &old_oid : ఖాళీ,
					new_లక్ష్యం,
					have_old_oid ? ఖాళీ : old_లక్ష్యం,
					update_flags | create_reflog_flag,
					సందేశం, &err);
	handle_ref_transaction_error(refname, ఖాళీ, have_old_oid ? &old_oid : ఖాళీ,
				     new_లక్ష్యం, have_old_oid ? ఖాళీ : old_లక్ష్యం,
				     tx_err, &err, opts);

	update_flags = default_flags;
	విడుదల(refname);
	విడుదల(old_arg);
	విడుదల(old_లక్ష్యం);
	విడుదల(new_లక్ష్యం);
	strbuf_release(&err);
}

స్థిర శూన్య parse_cmd_create(నిర్మాణం ref_transaction *transaction,
			     స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర,
			     నిర్మాణం command_options *opts)
{
	నిర్మాణం strbuf err = STRBUF_INIT;
	అక్షరం *refname;
	నిర్మాణం object_id new_oid;
	గణన ref_transaction_error tx_err;

	refname = parse_refname(&తదుపరి);
	ఒకవేళ (!refname)
		die("create: missing <ref>");

	ఒకవేళ (parse_తదుపరి_oid(&తదుపరి, చివర, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	ఒకవేళ (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	ఒకవేళ (*తదుపరి != పంక్తి_termination)
		die("create %s: extra input: %s", refname, తదుపరి);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, ఖాళీ,
					update_flags | create_reflog_flag,
					సందేశం, &err);
	handle_ref_transaction_error(refname, &new_oid, ఖాళీ, ఖాళీ, ఖాళీ, tx_err,
				     &err, opts);

	update_flags = default_flags;
	విడుదల(refname);
	strbuf_release(&err);
}

స్థిర శూన్య parse_cmd_symref_create(నిర్మాణం ref_transaction *transaction,
				    స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర వాడని,
				    నిర్మాణం command_options *opts)
{
	నిర్మాణం strbuf err = STRBUF_INIT;
	అక్షరం *refname, *new_లక్ష్యం;
	గణన ref_transaction_error tx_err;

	refname = parse_refname(&తదుపరి);
	ఒకవేళ (!refname)
		die("symref-create: missing <ref>");

	new_లక్ష్యం = parse_తదుపరి_refname(&తదుపరి);
	ఒకవేళ (!new_లక్ష్యం)
		die("symref-create %s: missing <new-target>", refname);

	ఒకవేళ (*తదుపరి != పంక్తి_termination)
		die("symref-create %s: extra input: %s", refname, తదుపరి);

	tx_err = ref_transaction_create(transaction, refname, ఖాళీ, new_లక్ష్యం,
					update_flags | create_reflog_flag,
					సందేశం, &err);
	handle_ref_transaction_error(refname, ఖాళీ, ఖాళీ, new_లక్ష్యం, ఖాళీ,
				     tx_err, &err, opts);

	update_flags = default_flags;
	విడుదల(refname);
	విడుదల(new_లక్ష్యం);
	strbuf_release(&err);
}

స్థిర శూన్య parse_cmd_delete(నిర్మాణం ref_transaction *transaction,
			     స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర,
			     నిర్మాణం command_options *opts వాడని)
{
	నిర్మాణం strbuf err = STRBUF_INIT;
	అక్షరం *refname;
	నిర్మాణం object_id old_oid;
	పూర్ణాంకం have_old;

	refname = parse_refname(&తదుపరి);
	ఒకవేళ (!refname)
		die("delete: missing <ref>");

	ఒకవేళ (parse_తదుపరి_oid(&తదుపరి, చివర, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} లేకపోతే {
		ఒకవేళ (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	ఒకవేళ (*తదుపరి != పంక్తి_termination)
		die("delete %s: extra input: %s", refname, తదుపరి);

	ఒకవేళ (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : ఖాళీ,
				   ఖాళీ, update_flags, సందేశం, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	విడుదల(refname);
	strbuf_release(&err);
}

స్థిర శూన్య parse_cmd_symref_delete(నిర్మాణం ref_transaction *transaction,
				    స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర వాడని,
				    నిర్మాణం command_options *opts వాడని)
{
	నిర్మాణం strbuf err = STRBUF_INIT;
	అక్షరం *refname, *old_లక్ష్యం;

	ఒకవేళ (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&తదుపరి);
	ఒకవేళ (!refname)
		die("symref-delete: missing <ref>");

	old_లక్ష్యం = parse_తదుపరి_refname(&తదుపరి);

	ఒకవేళ (*తదుపరి != పంక్తి_termination)
		die("symref-delete %s: extra input: %s", refname, తదుపరి);

	ఒకవేళ (ref_transaction_delete(transaction, refname, ఖాళీ,
				   old_లక్ష్యం, update_flags, సందేశం, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	విడుదల(refname);
	విడుదల(old_లక్ష్యం);
	strbuf_release(&err);
}

స్థిర శూన్య parse_cmd_verify(నిర్మాణం ref_transaction *transaction,
			     స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర,
			     నిర్మాణం command_options *opts వాడని)
{
	నిర్మాణం strbuf err = STRBUF_INIT;
	అక్షరం *refname;
	నిర్మాణం object_id old_oid;

	refname = parse_refname(&తదుపరి);
	ఒకవేళ (!refname)
		die("verify: missing <ref>");

	ఒకవేళ (parse_తదుపరి_oid(&తదుపరి, చివర, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	ఒకవేళ (*తదుపరి != పంక్తి_termination)
		die("verify %s: extra input: %s", refname, తదుపరి);

	ఒకవేళ (ref_transaction_verify(transaction, refname, &old_oid,
				   ఖాళీ, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	విడుదల(refname);
	strbuf_release(&err);
}

స్థిర శూన్య parse_cmd_symref_verify(నిర్మాణం ref_transaction *transaction,
				    స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర వాడని,
				    నిర్మాణం command_options *opts వాడని)
{
	నిర్మాణం strbuf err = STRBUF_INIT;
	నిర్మాణం object_id old_oid;
	అక్షరం *refname, *old_లక్ష్యం;

	ఒకవేళ (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&తదుపరి);
	ఒకవేళ (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_లక్ష్యం = parse_తదుపరి_refname(&తదుపరి);
	ఒకవేళ (!old_లక్ష్యం)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	ఒకవేళ (*తదుపరి != పంక్తి_termination)
		die("symref-verify %s: extra input: %s", refname, తదుపరి);

	ఒకవేళ (ref_transaction_verify(transaction, refname,
				   old_లక్ష్యం ? ఖాళీ : &old_oid,
				   old_లక్ష్యం, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	విడుదల(refname);
	విడుదల(old_లక్ష్యం);
	strbuf_release(&err);
}

స్థిర శూన్య report_ok(స్థిరరాశి అక్షరం *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

స్థిర శూన్య parse_cmd_option(నిర్మాణం ref_transaction *transaction వాడని,
			     స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర వాడని,
			     నిర్మాణం command_options *opts వాడని)
{
	స్థిరరాశి అక్షరం *rest;
	ఒకవేళ (skip_ఉపసర్గ(తదుపరి, "no-deref", &rest) && *rest == పంక్తి_termination)
		update_flags |= REF_NO_DEREF;
	లేకపోతే
		die("option unknown: %s", తదుపరి);
}

స్థిర శూన్య parse_cmd_ప్రారంభం(నిర్మాణం ref_transaction *transaction వాడని,
			    స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర వాడని,
			    నిర్మాణం command_options *opts వాడని)
{
	ఒకవేళ (*తదుపరి != పంక్తి_termination)
		die("start: extra input: %s", తదుపరి);
	report_ok("start");
}

స్థిర శూన్య parse_cmd_prepare(నిర్మాణం ref_transaction *transaction,
			      స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర వాడని,
			      నిర్మాణం command_options *opts వాడని)
{
	నిర్మాణం strbuf error = STRBUF_INIT;
	ఒకవేళ (*తదుపరి != పంక్తి_termination)
		die("prepare: extra input: %s", తదుపరి);
	ఒకవేళ (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

స్థిర శూన్య parse_cmd_abort(నిర్మాణం ref_transaction *transaction,
			    స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర వాడని,
			    నిర్మాణం command_options *opts వాడని)
{
	నిర్మాణం strbuf error = STRBUF_INIT;
	ఒకవేళ (*తదుపరి != పంక్తి_termination)
		die("abort: extra input: %s", తదుపరి);
	ఒకవేళ (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

స్థిర శూన్య parse_cmd_commit(నిర్మాణం ref_transaction *transaction,
			     స్థిరరాశి అక్షరం *తదుపరి, స్థిరరాశి అక్షరం *చివర వాడని,
			     నిర్మాణం command_options *opts వాడని)
{
	నిర్మాణం strbuf error = STRBUF_INIT;
	ఒకవేళ (*తదుపరి != పంక్తి_termination)
		die("commit: extra input: %s", తదుపరి);
	ఒకవేళ (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ఖాళీ);

	report_ok("commit");
	ref_transaction_free(transaction);
}

గణన update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

స్థిర స్థిరరాశి నిర్మాణం parse_cmd {
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	శూన్య (*fn)(నిర్మాణం ref_transaction *, స్థిరరాశి అక్షరం *, స్థిరరాశి అక్షరం *,
		   నిర్మాణం command_options *);
	గుర్తులేని వాదనలు;
	గణన update_refs_state state;
} command[] = {
	{ "update",        parse_cmd_update,        3, UPDATE_REFS_OPEN },
	{ "create",        parse_cmd_create,        2, UPDATE_REFS_OPEN },
	{ "delete",        parse_cmd_delete,        2, UPDATE_REFS_OPEN },
	{ "verify",        parse_cmd_verify,        2, UPDATE_REFS_OPEN },
	{ "symref-update", parse_cmd_symref_update, 4, UPDATE_REFS_OPEN },
	{ "symref-create", parse_cmd_symref_create, 2, UPDATE_REFS_OPEN },
	{ "symref-delete", parse_cmd_symref_delete, 2, UPDATE_REFS_OPEN },
	{ "symref-verify", parse_cmd_symref_verify, 2, UPDATE_REFS_OPEN },
	{ "option",        parse_cmd_option,        1, UPDATE_REFS_OPEN },
	{ "start",         parse_cmd_ప్రారంభం,         0, UPDATE_REFS_STARTED },
	{ "prepare",       parse_cmd_prepare,       0, UPDATE_REFS_PREPARED },
	{ "abort",         parse_cmd_abort,         0, UPDATE_REFS_CLOSED },
	{ "commit",        parse_cmd_commit,        0, UPDATE_REFS_CLOSED },
};

స్థిర శూన్య update_refs_stdin(గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం strbuf ఆదానం = STRBUF_INIT, err = STRBUF_INIT;
	గణన update_refs_state state = UPDATE_REFS_OPEN;
	నిర్మాణం ref_transaction *transaction;
	పూర్ణాంకం i, j;

	నిర్మాణం command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	ఒకవేళ (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	అయితే (!strbuf_getwholeline(&ఆదానం, stdin, పంక్తి_termination)) {
		స్థిరరాశి నిర్మాణం parse_cmd *cmd = ఖాళీ;

		ఒకవేళ (*ఆదానం.buf == పంక్తి_termination)
			die("empty command in input");
		లేకపోతే ఒకవేళ (ఖాళీనా(*ఆదానం.buf))
			die("whitespace before command: %s", ఆదానం.buf);

		కొరకు (i = 0; i < ARRAY_కొలత(command); i++) {
			స్థిరరాశి అక్షరం *ఉపసర్గ = command[i].ఉపసర్గ;
			అక్షరం c;

			ఒకవేళ (!starts_with(ఆదానం.buf, ఉపసర్గ))
				కొనసాగు;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].వాదనలు ? ' ' : పంక్తి_termination;
			ఒకవేళ (ఆదానం.buf[అక్షరశ్రేణి_పొడవు(ఉపసర్గ)] != c)
				కొనసాగు;

			cmd = &command[i];
			ఆపు;
		}
		ఒకవేళ (!cmd)
			die("unknown command: %s", ఆదానం.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		కొరకు (j = 1; పంక్తి_termination == '\0' && j < cmd->వాదనలు; j++)
			ఒకవేళ (strbuf_appendwholeline(&ఆదానం, stdin, పంక్తి_termination))
				ఆపు;

		ఎంపిక (state) {
		స్థితి UPDATE_REFS_OPEN:
		స్థితి UPDATE_REFS_STARTED:
			ఒకవేళ (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			ఒకవేళ (cmd->state >= state)
				state = cmd->state;
			ఆపు;
		స్థితి UPDATE_REFS_PREPARED:
			ఒకవేళ (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			ఆపు;
		స్థితి UPDATE_REFS_CLOSED:
			ఒకవేళ (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			ఒకవేళ (!transaction)
				die("%s", err.buf);

			ఆపు;
		}

		cmd->fn(transaction, ఆదానం.buf + అక్షరశ్రేణి_పొడవు(cmd->ఉపసర్గ) + !!cmd->వాదనలు,
			ఆదానం.buf + ఆదానం.len, &opts);
	}

	ఎంపిక (state) {
	స్థితి UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		ఒకవేళ (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ఖాళీ);
		ref_transaction_free(transaction);
		ఆపు;
	స్థితి UPDATE_REFS_STARTED:
	స్థితి UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		ఒకవేళ (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		ఆపు;
	స్థితి UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		ఆపు;
	}

	strbuf_release(&err);
	strbuf_release(&ఆదానం);
}

పూర్ణాంకం cmd_update_ref(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	స్థిరరాశి అక్షరం *refname, *oldval;
	నిర్మాణం object_id oid, oldoid;
	పూర్ణాంకం delete = 0, no_deref = 0, read_stdin = 0, చివర_null = 0;
	పూర్ణాంకం create_reflog = 0;
	గుర్తులేని పూర్ణాంకం flags = 0;

	నిర్మాణం option options[] = {
		OPT_STRING( 'm', ఖాళీ, &సందేశం, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', ఖాళీ, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', ఖాళీ, &చివర_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_బిట్టు('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_చివర(),
	};

	repo_config(the_repository, git_default_config, ఖాళీ);
	argc = parse_options(argc, argv, ఉపసర్గ, options, git_update_ref_usage,
			     0);
	ఒకవేళ (సందేశం && !*సందేశం)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	ఒకవేళ (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	ఒకవేళ (read_stdin) {
		ఒకవేళ (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		ఒకవేళ (చివర_null)
			పంక్తి_termination = '\0';
		update_refs_stdin(flags);
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	ఒకవేళ (చివర_null)
		usage_with_options(git_update_ref_usage, options);

	ఒకవేళ (delete) {
		ఒకవేళ (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} లేకపోతే {
		స్థిరరాశి అక్షరం *విలువ;
		ఒకవేళ (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		విలువ = argv[1];
		oldval = argv[2];
		ఒకవేళ (repo_get_oid_with_flags(the_repository, విలువ, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", విలువ);
	}

	ఒకవేళ (oldval) {
		ఒకవేళ (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		లేకపోతే ఒకవేళ (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	ఒకవేళ (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		తిరిగి refs_delete_ref(get_main_ref_store(the_repository),
				       సందేశం, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : ఖాళీ,
				       default_flags);
	లేకపోతే
		తిరిగి refs_update_ref(get_main_ref_store(the_repository),
				       సందేశం, refname, &oid,
				       oldval ? &oldoid : ఖాళీ,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
