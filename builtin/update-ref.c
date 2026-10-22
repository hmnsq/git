#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "quote.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	ফাঁকা
};

স্থির অক্ষর সারি_termination = '\n';
স্থির unsigned পূর্ণসংখ্যা update_flags;
স্থির unsigned পূর্ণসংখ্যা default_flags;
স্থির unsigned create_reflog_flag;
স্থির ধ্রুবক অক্ষর *বার্তা;

কাঠামো command_options {
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
স্থির ধ্রুবক অক্ষর *parse_arg(ধ্রুবক অক্ষর *পরবর্তী, কাঠামো strbuf *arg)
{
	যদি (*পরবর্তী == '"') {
		ধ্রুবক অক্ষর *orig = পরবর্তী;

		যদি (unquote_c_style(arg, পরবর্তী, &পরবর্তী))
			die("badly quoted argument: %s", orig);
		যদি (*পরবর্তী && !isspace(*পরবর্তী))
			die("unexpected character after quoted argument: %s", orig);
	} অন্যথা {
		যখন (*পরবর্তী && !isspace(*পরবর্তী))
			strbuf_addch(arg, *পরবর্তী++);
	}

	ফেরত পরবর্তী;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
স্থির অক্ষর *parse_refname(ধ্রুবক অক্ষর **পরবর্তী)
{
	কাঠামো strbuf ref = STRBUF_INIT;

	যদি (সারি_termination) {
		/* Without -z, use the next argument */
		*পরবর্তী = parse_arg(*পরবর্তী, &ref);
	} অন্যথা {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *পরবর্তী);
		*পরবর্তী += ref.len;
	}

	যদি (!ref.len) {
		strbuf_release(&ref);
		ফেরত ফাঁকা;
	}

	যদি (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	ফেরত strbuf_detach(&ref, ফাঁকা);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
স্থির অক্ষর *parse_পরবর্তী_refname(ধ্রুবক অক্ষর **পরবর্তী)
{
	যদি (সারি_termination) {
		/* Without -z, consume SP and use next argument */
		যদি (!**পরবর্তী || **পরবর্তী == সারি_termination)
			ফেরত ফাঁকা;
		যদি (**পরবর্তী != ' ')
			die("expected SP but got: %s", *পরবর্তী);
	} অন্যথা {
		/* With -z, read the next NUL-terminated line */
		যদি (**পরবর্তী)
			ফেরত ফাঁকা;
	}
	/* Skip the delimiter */
	(*পরবর্তী)++;

	ফেরত parse_refname(পরবর্তী);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
স্থির অক্ষর *parse_পরবর্তী_arg(ধ্রুবক অক্ষর **পরবর্তী)
{
	কাঠামো strbuf arg = STRBUF_INIT;

	যদি (সারি_termination) {
		/* Without -z, consume SP and use next argument */
		যদি (!**পরবর্তী || **পরবর্তী == সারি_termination)
			ফেরত ফাঁকা;
		যদি (**পরবর্তী != ' ')
			die("expected SP but got: %s", *পরবর্তী);
	} অন্যথা {
		/* With -z, read the next NUL-terminated line */
		যদি (**পরবর্তী)
			ফেরত ফাঁকা;
	}
	/* Skip the delimiter */
	(*পরবর্তী)++;

	যদি (সারি_termination) {
		/* Without -z, use the next argument */
		*পরবর্তী = parse_arg(*পরবর্তী, &arg);
	} অন্যথা {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *পরবর্তী);
		*পরবর্তী += arg.len;
	}

	যদি (arg.len)
		ফেরত strbuf_detach(&arg, ফাঁকা);

	strbuf_release(&arg);
	ফেরত ফাঁকা;
}

/*
 * The value being parsed is <old-oid> (as opposed to <new-oid>; the
 * difference affects which error messages are generated):
 */
#define PARSE_SHA1_OLD 0x01

/*
 * For backwards compatibility, accept an empty string for update's
 * <new-oid> in binary mode to be equivalent to specifying zeros.
 */
#define PARSE_SHA1_ALLOW_EMPTY 0x02

/*
 * Parse an argument separator followed by the next argument, if any.
 * If there is an argument, convert it to a SHA-1, write it to sha1,
 * set *next to point at the character terminating the argument, and
 * return 0.  If there is no argument at all (not even the empty
 * string), return 1 and leave *next unchanged.  If the value is
 * provided but cannot be converted to a SHA-1, die.  flags can
 * include PARSE_SHA1_OLD and/or PARSE_SHA1_ALLOW_EMPTY.
 */
স্থির পূর্ণসংখ্যা parse_পরবর্তী_oid(ধ্রুবক অক্ষর **পরবর্তী, ধ্রুবক অক্ষর *end,
			  কাঠামো object_id *oid,
			  ধ্রুবক অক্ষর *command, ধ্রুবক অক্ষর *refname,
			  পূর্ণসংখ্যা flags)
{
	কাঠামো strbuf arg = STRBUF_INIT;
	পূর্ণসংখ্যা ret = 0;

	যদি (*পরবর্তী == end)
		যাও eof;

	যদি (সারি_termination) {
		/* Without -z, consume SP and use next argument */
		যদি (!**পরবর্তী || **পরবর্তী == সারি_termination)
			ফেরত 1;
		যদি (**পরবর্তী != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *পরবর্তী);
		(*পরবর্তী)++;
		*পরবর্তী = parse_arg(*পরবর্তী, &arg);
		যদি (arg.len) {
			যদি (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				যাও invalid;
		} অন্যথা {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} অন্যথা {
		/* With -z, read the next NUL-terminated line */
		যদি (**পরবর্তী)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *পরবর্তী);
		(*পরবর্তী)++;
		যদি (*পরবর্তী == end)
			যাও eof;
		strbuf_addstr(&arg, *পরবর্তী);
		*পরবর্তী += arg.len;

		যদি (arg.len) {
			যদি (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				যাও invalid;
		} অন্যথা যদি (flags & PARSE_SHA1_ALLOW_EMPTY) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} অন্যথা {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	ফেরত ret;

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

স্থির শূন্য print_rejected_refs(ধ্রুবক অক্ষর *refname,
				ধ্রুবক কাঠামো object_id *old_oid,
				ধ্রুবক কাঠামো object_id *new_oid,
				ধ্রুবক অক্ষর *old_target,
				ধ্রুবক অক্ষর *new_target,
				গণনা ref_transaction_error err,
				ধ্রুবক অক্ষর *details,
				শূন্য *cb_তথ্য UNUSED)
{
	কাঠামো strbuf sb = STRBUF_INIT;

	যদি (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_to_hex(new_oid) : new_target,
		    old_oid ? oid_to_hex(old_oid) : old_target,
		    ref_transaction_error_বার্তা(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
স্থির শূন্য handle_ref_transaction_error(ধ্রুবক অক্ষর *refname,
					 কাঠামো object_id *new_oid,
					 কাঠামো object_id *old_oid,
					 ধ্রুবক অক্ষর *new_target,
					 ধ্রুবক অক্ষর *old_target,
					 গণনা ref_transaction_error tx_err,
					 কাঠামো strbuf *err,
					 কাঠামো command_options *opts)
{
	যদি (!tx_err)
		ফেরত;

	যদি (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_target,
				    new_target, tx_err, err->buf, ফাঁকা);
		ফেরত;
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

স্থির শূন্য parse_cmd_update(কাঠামো ref_transaction *transaction,
			     ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end,
			     কাঠামো command_options *opts)
{
	কাঠামো strbuf err = STRBUF_INIT;
	অক্ষর *refname;
	কাঠামো object_id new_oid, old_oid;
	গণনা ref_transaction_error tx_err;
	পূর্ণসংখ্যা have_old;

	refname = parse_refname(&পরবর্তী);
	যদি (!refname)
		die("update: missing <ref>");

	যদি (parse_পরবর্তী_oid(&পরবর্তী, end, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_EMPTY))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_পরবর্তী_oid(&পরবর্তী, end, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	যদি (*পরবর্তী != সারি_termination)
		die("update %s: extra input: %s", refname, পরবর্তী);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : ফাঁকা,
					ফাঁকা, ফাঁকা,
					update_flags | create_reflog_flag,
					বার্তা, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : ফাঁকা,
				     ফাঁকা, ফাঁকা, tx_err, &err, opts);


	update_flags = default_flags;
	মুক্ত(refname);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_symref_update(কাঠামো ref_transaction *transaction,
				    ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end UNUSED,
				    কাঠামো command_options *opts)
{
	অক্ষর *refname, *new_target, *old_arg;
	গণনা ref_transaction_error tx_err;
	অক্ষর *old_target = ফাঁকা;
	কাঠামো strbuf err = STRBUF_INIT;
	কাঠামো object_id old_oid;
	পূর্ণসংখ্যা have_old_oid = 0;

	refname = parse_refname(&পরবর্তী);
	যদি (!refname)
		die("symref-update: missing <ref>");

	new_target = parse_পরবর্তী_refname(&পরবর্তী);
	যদি (!new_target)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_পরবর্তী_arg(&পরবর্তী);
	যদি (old_arg) {
		old_target = parse_পরবর্তী_arg(&পরবর্তী);
		যদি (!old_target)
			die("symref-update %s: expected old value", refname);

		যদি (!strcmp(old_arg, "oid")) {
			যদি (repo_get_oid_with_flags(the_repository, old_target, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_target);

			have_old_oid = 1;
		} অন্যথা যদি (!strcmp(old_arg, "ref")) {
			যদি (check_refname_format(old_target, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_target);
		} অন্যথা {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	যদি (*পরবর্তী != সারি_termination)
		die("symref-update %s: extra input: %s", refname, পরবর্তী);

	tx_err = ref_transaction_update(transaction, refname, ফাঁকা,
					have_old_oid ? &old_oid : ফাঁকা,
					new_target,
					have_old_oid ? ফাঁকা : old_target,
					update_flags | create_reflog_flag,
					বার্তা, &err);
	handle_ref_transaction_error(refname, ফাঁকা, have_old_oid ? &old_oid : ফাঁকা,
				     new_target, have_old_oid ? ফাঁকা : old_target,
				     tx_err, &err, opts);

	update_flags = default_flags;
	মুক্ত(refname);
	মুক্ত(old_arg);
	মুক্ত(old_target);
	মুক্ত(new_target);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_create(কাঠামো ref_transaction *transaction,
			     ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end,
			     কাঠামো command_options *opts)
{
	কাঠামো strbuf err = STRBUF_INIT;
	অক্ষর *refname;
	কাঠামো object_id new_oid;
	গণনা ref_transaction_error tx_err;

	refname = parse_refname(&পরবর্তী);
	যদি (!refname)
		die("create: missing <ref>");

	যদি (parse_পরবর্তী_oid(&পরবর্তী, end, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	যদি (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	যদি (*পরবর্তী != সারি_termination)
		die("create %s: extra input: %s", refname, পরবর্তী);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, ফাঁকা,
					update_flags | create_reflog_flag,
					বার্তা, &err);
	handle_ref_transaction_error(refname, &new_oid, ফাঁকা, ফাঁকা, ফাঁকা, tx_err,
				     &err, opts);

	update_flags = default_flags;
	মুক্ত(refname);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_symref_create(কাঠামো ref_transaction *transaction,
				    ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end UNUSED,
				    কাঠামো command_options *opts)
{
	কাঠামো strbuf err = STRBUF_INIT;
	অক্ষর *refname, *new_target;
	গণনা ref_transaction_error tx_err;

	refname = parse_refname(&পরবর্তী);
	যদি (!refname)
		die("symref-create: missing <ref>");

	new_target = parse_পরবর্তী_refname(&পরবর্তী);
	যদি (!new_target)
		die("symref-create %s: missing <new-target>", refname);

	যদি (*পরবর্তী != সারি_termination)
		die("symref-create %s: extra input: %s", refname, পরবর্তী);

	tx_err = ref_transaction_create(transaction, refname, ফাঁকা, new_target,
					update_flags | create_reflog_flag,
					বার্তা, &err);
	handle_ref_transaction_error(refname, ফাঁকা, ফাঁকা, new_target, ফাঁকা,
				     tx_err, &err, opts);

	update_flags = default_flags;
	মুক্ত(refname);
	মুক্ত(new_target);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_delete(কাঠামো ref_transaction *transaction,
			     ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end,
			     কাঠামো command_options *opts UNUSED)
{
	কাঠামো strbuf err = STRBUF_INIT;
	অক্ষর *refname;
	কাঠামো object_id old_oid;
	পূর্ণসংখ্যা have_old;

	refname = parse_refname(&পরবর্তী);
	যদি (!refname)
		die("delete: missing <ref>");

	যদি (parse_পরবর্তী_oid(&পরবর্তী, end, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} অন্যথা {
		যদি (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	যদি (*পরবর্তী != সারি_termination)
		die("delete %s: extra input: %s", refname, পরবর্তী);

	যদি (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : ফাঁকা,
				   ফাঁকা, update_flags, বার্তা, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	মুক্ত(refname);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_symref_delete(কাঠামো ref_transaction *transaction,
				    ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end UNUSED,
				    কাঠামো command_options *opts UNUSED)
{
	কাঠামো strbuf err = STRBUF_INIT;
	অক্ষর *refname, *old_target;

	যদি (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&পরবর্তী);
	যদি (!refname)
		die("symref-delete: missing <ref>");

	old_target = parse_পরবর্তী_refname(&পরবর্তী);

	যদি (*পরবর্তী != সারি_termination)
		die("symref-delete %s: extra input: %s", refname, পরবর্তী);

	যদি (ref_transaction_delete(transaction, refname, ফাঁকা,
				   old_target, update_flags, বার্তা, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	মুক্ত(refname);
	মুক্ত(old_target);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_verify(কাঠামো ref_transaction *transaction,
			     ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end,
			     কাঠামো command_options *opts UNUSED)
{
	কাঠামো strbuf err = STRBUF_INIT;
	অক্ষর *refname;
	কাঠামো object_id old_oid;

	refname = parse_refname(&পরবর্তী);
	যদি (!refname)
		die("verify: missing <ref>");

	যদি (parse_পরবর্তী_oid(&পরবর্তী, end, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	যদি (*পরবর্তী != সারি_termination)
		die("verify %s: extra input: %s", refname, পরবর্তী);

	যদি (ref_transaction_verify(transaction, refname, &old_oid,
				   ফাঁকা, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	মুক্ত(refname);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_symref_verify(কাঠামো ref_transaction *transaction,
				    ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end UNUSED,
				    কাঠামো command_options *opts UNUSED)
{
	কাঠামো strbuf err = STRBUF_INIT;
	কাঠামো object_id old_oid;
	অক্ষর *refname, *old_target;

	যদি (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&পরবর্তী);
	যদি (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_target = parse_পরবর্তী_refname(&পরবর্তী);
	যদি (!old_target)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	যদি (*পরবর্তী != সারি_termination)
		die("symref-verify %s: extra input: %s", refname, পরবর্তী);

	যদি (ref_transaction_verify(transaction, refname,
				   old_target ? ফাঁকা : &old_oid,
				   old_target, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	মুক্ত(refname);
	মুক্ত(old_target);
	strbuf_release(&err);
}

স্থির শূন্য report_ok(ধ্রুবক অক্ষর *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

স্থির শূন্য parse_cmd_option(কাঠামো ref_transaction *transaction UNUSED,
			     ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end UNUSED,
			     কাঠামো command_options *opts UNUSED)
{
	ধ্রুবক অক্ষর *rest;
	যদি (skip_prefix(পরবর্তী, "no-deref", &rest) && *rest == সারি_termination)
		update_flags |= REF_NO_DEREF;
	অন্যথা
		die("option unknown: %s", পরবর্তী);
}

স্থির শূন্য parse_cmd_start(কাঠামো ref_transaction *transaction UNUSED,
			    ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end UNUSED,
			    কাঠামো command_options *opts UNUSED)
{
	যদি (*পরবর্তী != সারি_termination)
		die("start: extra input: %s", পরবর্তী);
	report_ok("start");
}

স্থির শূন্য parse_cmd_prepare(কাঠামো ref_transaction *transaction,
			      ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end UNUSED,
			      কাঠামো command_options *opts UNUSED)
{
	কাঠামো strbuf error = STRBUF_INIT;
	যদি (*পরবর্তী != সারি_termination)
		die("prepare: extra input: %s", পরবর্তী);
	যদি (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

স্থির শূন্য parse_cmd_abort(কাঠামো ref_transaction *transaction,
			    ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end UNUSED,
			    কাঠামো command_options *opts UNUSED)
{
	কাঠামো strbuf error = STRBUF_INIT;
	যদি (*পরবর্তী != সারি_termination)
		die("abort: extra input: %s", পরবর্তী);
	যদি (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

স্থির শূন্য parse_cmd_commit(কাঠামো ref_transaction *transaction,
			     ধ্রুবক অক্ষর *পরবর্তী, ধ্রুবক অক্ষর *end UNUSED,
			     কাঠামো command_options *opts UNUSED)
{
	কাঠামো strbuf error = STRBUF_INIT;
	যদি (*পরবর্তী != সারি_termination)
		die("commit: extra input: %s", পরবর্তী);
	যদি (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ফাঁকা);

	report_ok("commit");
	ref_transaction_free(transaction);
}

গণনা update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

স্থির ধ্রুবক কাঠামো parse_cmd {
	ধ্রুবক অক্ষর *prefix;
	শূন্য (*fn)(কাঠামো ref_transaction *, ধ্রুবক অক্ষর *, ধ্রুবক অক্ষর *,
		   কাঠামো command_options *);
	unsigned args;
	গণনা update_refs_state state;
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
	{ "start",         parse_cmd_start,         0, UPDATE_REFS_STARTED },
	{ "prepare",       parse_cmd_prepare,       0, UPDATE_REFS_PREPARED },
	{ "abort",         parse_cmd_abort,         0, UPDATE_REFS_CLOSED },
	{ "commit",        parse_cmd_commit,        0, UPDATE_REFS_CLOSED },
};

স্থির শূন্য update_refs_stdin(unsigned পূর্ণসংখ্যা flags)
{
	কাঠামো strbuf প্রবেশ = STRBUF_INIT, err = STRBUF_INIT;
	গণনা update_refs_state state = UPDATE_REFS_OPEN;
	কাঠামো ref_transaction *transaction;
	পূর্ণসংখ্যা i, j;

	কাঠামো command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	যদি (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	যখন (!strbuf_getwholeline(&প্রবেশ, stdin, সারি_termination)) {
		ধ্রুবক কাঠামো parse_cmd *cmd = ফাঁকা;

		যদি (*প্রবেশ.buf == সারি_termination)
			die("empty command in input");
		অন্যথা যদি (isspace(*প্রবেশ.buf))
			die("whitespace before command: %s", প্রবেশ.buf);

		জন্য (i = 0; i < ARRAY_আকার(command); i++) {
			ধ্রুবক অক্ষর *prefix = command[i].prefix;
			অক্ষর c;

			যদি (!starts_with(প্রবেশ.buf, prefix))
				চালু;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].args ? ' ' : সারি_termination;
			যদি (প্রবেশ.buf[strlen(prefix)] != c)
				চালু;

			cmd = &command[i];
			বিরতি;
		}
		যদি (!cmd)
			die("unknown command: %s", প্রবেশ.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		জন্য (j = 1; সারি_termination == '\0' && j < cmd->args; j++)
			যদি (strbuf_appendwholeline(&প্রবেশ, stdin, সারি_termination))
				বিরতি;

		নির্বাচন (state) {
		ক্ষেত্র UPDATE_REFS_OPEN:
		ক্ষেত্র UPDATE_REFS_STARTED:
			যদি (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			যদি (cmd->state >= state)
				state = cmd->state;
			বিরতি;
		ক্ষেত্র UPDATE_REFS_PREPARED:
			যদি (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			বিরতি;
		ক্ষেত্র UPDATE_REFS_CLOSED:
			যদি (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			যদি (!transaction)
				die("%s", err.buf);

			বিরতি;
		}

		cmd->fn(transaction, প্রবেশ.buf + strlen(cmd->prefix) + !!cmd->args,
			প্রবেশ.buf + প্রবেশ.len, &opts);
	}

	নির্বাচন (state) {
	ক্ষেত্র UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		যদি (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ফাঁকা);
		ref_transaction_free(transaction);
		বিরতি;
	ক্ষেত্র UPDATE_REFS_STARTED:
	ক্ষেত্র UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		যদি (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		বিরতি;
	ক্ষেত্র UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		বিরতি;
	}

	strbuf_release(&err);
	strbuf_release(&প্রবেশ);
}

পূর্ণসংখ্যা cmd_update_ref(পূর্ণসংখ্যা argc,
		   ধ্রুবক অক্ষর **argv,
		   ধ্রুবক অক্ষর *prefix,
		   কাঠামো repository *repo UNUSED)
{
	ধ্রুবক অক্ষর *refname, *oldval;
	কাঠামো object_id oid, oldoid;
	পূর্ণসংখ্যা delete = 0, no_deref = 0, read_stdin = 0, end_null = 0;
	পূর্ণসংখ্যা create_reflog = 0;
	unsigned পূর্ণসংখ্যা flags = 0;

	কাঠামো option options[] = {
		OPT_STRING( 'm', ফাঁকা, &বার্তা, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', ফাঁকা, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', ফাঁকা, &end_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_BIT('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ফাঁকা);
	argc = parse_options(argc, argv, prefix, options, git_update_ref_usage,
			     0);
	যদি (বার্তা && !*বার্তা)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	যদি (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	যদি (read_stdin) {
		যদি (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		যদি (end_null)
			সারি_termination = '\0';
		update_refs_stdin(flags);
		ফেরত 0;
	} অন্যথা যদি (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	যদি (end_null)
		usage_with_options(git_update_ref_usage, options);

	যদি (delete) {
		যদি (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} অন্যথা {
		ধ্রুবক অক্ষর *মান;
		যদি (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		মান = argv[1];
		oldval = argv[2];
		যদি (repo_get_oid_with_flags(the_repository, মান, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", মান);
	}

	যদি (oldval) {
		যদি (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		অন্যথা যদি (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	যদি (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		ফেরত refs_delete_ref(get_main_ref_store(the_repository),
				       বার্তা, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : ফাঁকা,
				       default_flags);
	অন্যথা
		ফেরত refs_update_ref(get_main_ref_store(the_repository),
				       বার্তা, refname, &oid,
				       oldval ? &oldoid : ফাঁকা,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
