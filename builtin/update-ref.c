#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hash.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "quote.h"

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	রিক্ত
};

স্থির ৱর্ণ সাৰি_termination = '\n';
স্থির unsigned পূর্ণাঙ্ক update_flags;
স্থির unsigned পূর্ণাঙ্ক default_flags;
স্থির unsigned create_reflog_flag;
স্থির ধ্রুৱ ৱর্ণ *বাৰ্তা;

সংরচনা command_options {
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
স্থির ধ্রুৱ ৱর্ণ *parse_arg(ধ্রুৱ ৱর্ণ *next, সংরচনা strbuf *arg)
{
	যদি (*next == '"') {
		ধ্রুৱ ৱর্ণ *orig = next;

		যদি (unquote_c_style(arg, next, &next))
			die("badly quoted argument: %s", orig);
		যদি (*next && !isspace(*next))
			die("unexpected character after quoted argument: %s", orig);
	} অন্যথা {
		যেতিয়ালৈ (*next && !isspace(*next))
			strbuf_addch(arg, *next++);
	}

	ঘূৰাই next;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
স্থির ৱর্ণ *parse_refname(ধ্রুৱ ৱর্ণ **next)
{
	সংরচনা strbuf ref = STRBUF_INIT;

	যদি (সাৰি_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &ref);
	} অন্যথা {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *next);
		*next += ref.len;
	}

	যদি (!ref.len) {
		strbuf_release(&ref);
		ঘূৰাই রিক্ত;
	}

	যদি (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	ঘূৰাই strbuf_detach(&ref, রিক্ত);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
স্থির ৱর্ণ *parse_next_refname(ধ্রুৱ ৱর্ণ **next)
{
	যদি (সাৰি_termination) {
		/* Without -z, consume SP and use next argument */
		যদি (!**next || **next == সাৰি_termination)
			ঘূৰাই রিক্ত;
		যদি (**next != ' ')
			die("expected SP but got: %s", *next);
	} অন্যথা {
		/* With -z, read the next NUL-terminated line */
		যদি (**next)
			ঘূৰাই রিক্ত;
	}
	/* Skip the delimiter */
	(*next)++;

	ঘূৰাই parse_refname(next);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
স্থির ৱর্ণ *parse_next_arg(ধ্রুৱ ৱর্ণ **next)
{
	সংরচনা strbuf arg = STRBUF_INIT;

	যদি (সাৰি_termination) {
		/* Without -z, consume SP and use next argument */
		যদি (!**next || **next == সাৰি_termination)
			ঘূৰাই রিক্ত;
		যদি (**next != ' ')
			die("expected SP but got: %s", *next);
	} অন্যথা {
		/* With -z, read the next NUL-terminated line */
		যদি (**next)
			ঘূৰাই রিক্ত;
	}
	/* Skip the delimiter */
	(*next)++;

	যদি (সাৰি_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &arg);
	} অন্যথা {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *next);
		*next += arg.len;
	}

	যদি (arg.len)
		ঘূৰাই strbuf_detach(&arg, রিক্ত);

	strbuf_release(&arg);
	ঘূৰাই রিক্ত;
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
স্থির পূর্ণাঙ্ক parse_next_oid(ধ্রুৱ ৱর্ণ **next, ধ্রুৱ ৱর্ণ *end,
			  সংরচনা object_id *oid,
			  ধ্রুৱ ৱর্ণ *command, ধ্রুৱ ৱর্ণ *refname,
			  পূর্ণাঙ্ক flags)
{
	সংরচনা strbuf arg = STRBUF_INIT;
	পূর্ণাঙ্ক ret = 0;

	যদি (*next == end)
		গচ্ছ eof;

	যদি (সাৰি_termination) {
		/* Without -z, consume SP and use next argument */
		যদি (!**next || **next == সাৰি_termination)
			ঘূৰাই 1;
		যদি (**next != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *next);
		(*next)++;
		*next = parse_arg(*next, &arg);
		যদি (arg.len) {
			যদি (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				গচ্ছ invalid;
		} অন্যথা {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} অন্যথা {
		/* With -z, read the next NUL-terminated line */
		যদি (**next)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *next);
		(*next)++;
		যদি (*next == end)
			গচ্ছ eof;
		strbuf_addstr(&arg, *next);
		*next += arg.len;

		যদি (arg.len) {
			যদি (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				গচ্ছ invalid;
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

	ঘূৰাই ret;

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

স্থির শূন্য print_rejected_refs(ধ্রুৱ ৱর্ণ *refname,
				ধ্রুৱ সংরচনা object_id *old_oid,
				ধ্রুৱ সংরচনা object_id *new_oid,
				ধ্রুৱ ৱর্ণ *old_target,
				ধ্রুৱ ৱর্ণ *new_target,
				গণনা ref_transaction_error err,
				ধ্রুৱ ৱর্ণ *details,
				শূন্য *cb_data UNUSED)
{
	সংরচনা strbuf sb = STRBUF_INIT;

	যদি (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_to_hex(new_oid) : new_target,
		    old_oid ? oid_to_hex(old_oid) : old_target,
		    ref_transaction_error_বাৰ্তা(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
স্থির শূন্য handle_ref_transaction_error(ধ্রুৱ ৱর্ণ *refname,
					 সংরচনা object_id *new_oid,
					 সংরচনা object_id *old_oid,
					 ধ্রুৱ ৱর্ণ *new_target,
					 ধ্রুৱ ৱর্ণ *old_target,
					 গণনা ref_transaction_error tx_err,
					 সংরচনা strbuf *err,
					 সংরচনা command_options *opts)
{
	যদি (!tx_err)
		ঘূৰাই;

	যদি (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_target,
				    new_target, tx_err, err->buf, রিক্ত);
		ঘূৰাই;
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

স্থির শূন্য parse_cmd_update(সংরচনা ref_transaction *transaction,
			     ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end,
			     সংরচনা command_options *opts)
{
	সংরচনা strbuf err = STRBUF_INIT;
	ৱর্ণ *refname;
	সংরচনা object_id new_oid, old_oid;
	গণনা ref_transaction_error tx_err;
	পূর্ণাঙ্ক have_old;

	refname = parse_refname(&next);
	যদি (!refname)
		die("update: missing <ref>");

	যদি (parse_next_oid(&next, end, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_EMPTY))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_next_oid(&next, end, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	যদি (*next != সাৰি_termination)
		die("update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : রিক্ত,
					রিক্ত, রিক্ত,
					update_flags | create_reflog_flag,
					বাৰ্তা, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : রিক্ত,
				     রিক্ত, রিক্ত, tx_err, &err, opts);


	update_flags = default_flags;
	মুক্ত(refname);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_symref_update(সংরচনা ref_transaction *transaction,
				    ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end UNUSED,
				    সংরচনা command_options *opts)
{
	ৱর্ণ *refname, *new_target, *old_arg;
	গণনা ref_transaction_error tx_err;
	ৱর্ণ *old_target = রিক্ত;
	সংরচনা strbuf err = STRBUF_INIT;
	সংরচনা object_id old_oid;
	পূর্ণাঙ্ক have_old_oid = 0;

	refname = parse_refname(&next);
	যদি (!refname)
		die("symref-update: missing <ref>");

	new_target = parse_next_refname(&next);
	যদি (!new_target)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_next_arg(&next);
	যদি (old_arg) {
		old_target = parse_next_arg(&next);
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

	যদি (*next != সাৰি_termination)
		die("symref-update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname, রিক্ত,
					have_old_oid ? &old_oid : রিক্ত,
					new_target,
					have_old_oid ? রিক্ত : old_target,
					update_flags | create_reflog_flag,
					বাৰ্তা, &err);
	handle_ref_transaction_error(refname, রিক্ত, have_old_oid ? &old_oid : রিক্ত,
				     new_target, have_old_oid ? রিক্ত : old_target,
				     tx_err, &err, opts);

	update_flags = default_flags;
	মুক্ত(refname);
	মুক্ত(old_arg);
	মুক্ত(old_target);
	মুক্ত(new_target);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_create(সংরচনা ref_transaction *transaction,
			     ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end,
			     সংরচনা command_options *opts)
{
	সংরচনা strbuf err = STRBUF_INIT;
	ৱর্ণ *refname;
	সংরচনা object_id new_oid;
	গণনা ref_transaction_error tx_err;

	refname = parse_refname(&next);
	যদি (!refname)
		die("create: missing <ref>");

	যদি (parse_next_oid(&next, end, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	যদি (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	যদি (*next != সাৰি_termination)
		die("create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, রিক্ত,
					update_flags | create_reflog_flag,
					বাৰ্তা, &err);
	handle_ref_transaction_error(refname, &new_oid, রিক্ত, রিক্ত, রিক্ত, tx_err,
				     &err, opts);

	update_flags = default_flags;
	মুক্ত(refname);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_symref_create(সংরচনা ref_transaction *transaction,
				    ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end UNUSED,
				    সংরচনা command_options *opts)
{
	সংরচনা strbuf err = STRBUF_INIT;
	ৱর্ণ *refname, *new_target;
	গণনা ref_transaction_error tx_err;

	refname = parse_refname(&next);
	যদি (!refname)
		die("symref-create: missing <ref>");

	new_target = parse_next_refname(&next);
	যদি (!new_target)
		die("symref-create %s: missing <new-target>", refname);

	যদি (*next != সাৰি_termination)
		die("symref-create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, রিক্ত, new_target,
					update_flags | create_reflog_flag,
					বাৰ্তা, &err);
	handle_ref_transaction_error(refname, রিক্ত, রিক্ত, new_target, রিক্ত,
				     tx_err, &err, opts);

	update_flags = default_flags;
	মুক্ত(refname);
	মুক্ত(new_target);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_delete(সংরচনা ref_transaction *transaction,
			     ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end,
			     সংরচনা command_options *opts UNUSED)
{
	সংরচনা strbuf err = STRBUF_INIT;
	ৱর্ণ *refname;
	সংরচনা object_id old_oid;
	পূর্ণাঙ্ক have_old;

	refname = parse_refname(&next);
	যদি (!refname)
		die("delete: missing <ref>");

	যদি (parse_next_oid(&next, end, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} অন্যথা {
		যদি (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	যদি (*next != সাৰি_termination)
		die("delete %s: extra input: %s", refname, next);

	যদি (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : রিক্ত,
				   রিক্ত, update_flags, বাৰ্তা, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	মুক্ত(refname);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_symref_delete(সংরচনা ref_transaction *transaction,
				    ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end UNUSED,
				    সংরচনা command_options *opts UNUSED)
{
	সংরচনা strbuf err = STRBUF_INIT;
	ৱর্ণ *refname, *old_target;

	যদি (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&next);
	যদি (!refname)
		die("symref-delete: missing <ref>");

	old_target = parse_next_refname(&next);

	যদি (*next != সাৰি_termination)
		die("symref-delete %s: extra input: %s", refname, next);

	যদি (ref_transaction_delete(transaction, refname, রিক্ত,
				   old_target, update_flags, বাৰ্তা, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	মুক্ত(refname);
	মুক্ত(old_target);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_verify(সংরচনা ref_transaction *transaction,
			     ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end,
			     সংরচনা command_options *opts UNUSED)
{
	সংরচনা strbuf err = STRBUF_INIT;
	ৱর্ণ *refname;
	সংরচনা object_id old_oid;

	refname = parse_refname(&next);
	যদি (!refname)
		die("verify: missing <ref>");

	যদি (parse_next_oid(&next, end, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	যদি (*next != সাৰি_termination)
		die("verify %s: extra input: %s", refname, next);

	যদি (ref_transaction_verify(transaction, refname, &old_oid,
				   রিক্ত, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	মুক্ত(refname);
	strbuf_release(&err);
}

স্থির শূন্য parse_cmd_symref_verify(সংরচনা ref_transaction *transaction,
				    ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end UNUSED,
				    সংরচনা command_options *opts UNUSED)
{
	সংরচনা strbuf err = STRBUF_INIT;
	সংরচনা object_id old_oid;
	ৱর্ণ *refname, *old_target;

	যদি (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&next);
	যদি (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_target = parse_next_refname(&next);
	যদি (!old_target)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	যদি (*next != সাৰি_termination)
		die("symref-verify %s: extra input: %s", refname, next);

	যদি (ref_transaction_verify(transaction, refname,
				   old_target ? রিক্ত : &old_oid,
				   old_target, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	মুক্ত(refname);
	মুক্ত(old_target);
	strbuf_release(&err);
}

স্থির শূন্য report_ok(ধ্রুৱ ৱর্ণ *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

স্থির শূন্য parse_cmd_option(সংরচনা ref_transaction *transaction UNUSED,
			     ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end UNUSED,
			     সংরচনা command_options *opts UNUSED)
{
	ধ্রুৱ ৱর্ণ *rest;
	যদি (skip_prefix(next, "no-deref", &rest) && *rest == সাৰি_termination)
		update_flags |= REF_NO_DEREF;
	অন্যথা
		die("option unknown: %s", next);
}

স্থির শূন্য parse_cmd_start(সংরচনা ref_transaction *transaction UNUSED,
			    ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end UNUSED,
			    সংরচনা command_options *opts UNUSED)
{
	যদি (*next != সাৰি_termination)
		die("start: extra input: %s", next);
	report_ok("start");
}

স্থির শূন্য parse_cmd_prepare(সংরচনা ref_transaction *transaction,
			      ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end UNUSED,
			      সংরচনা command_options *opts UNUSED)
{
	সংরচনা strbuf error = STRBUF_INIT;
	যদি (*next != সাৰি_termination)
		die("prepare: extra input: %s", next);
	যদি (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

স্থির শূন্য parse_cmd_abort(সংরচনা ref_transaction *transaction,
			    ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end UNUSED,
			    সংরচনা command_options *opts UNUSED)
{
	সংরচনা strbuf error = STRBUF_INIT;
	যদি (*next != সাৰি_termination)
		die("abort: extra input: %s", next);
	যদি (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

স্থির শূন্য parse_cmd_commit(সংরচনা ref_transaction *transaction,
			     ধ্রুৱ ৱর্ণ *next, ধ্রুৱ ৱর্ণ *end UNUSED,
			     সংরচনা command_options *opts UNUSED)
{
	সংরচনা strbuf error = STRBUF_INIT;
	যদি (*next != সাৰি_termination)
		die("commit: extra input: %s", next);
	যদি (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, রিক্ত);

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

স্থির ধ্রুৱ সংরচনা parse_cmd {
	ধ্রুৱ ৱর্ণ *prefix;
	শূন্য (*fn)(সংরচনা ref_transaction *, ধ্রুৱ ৱর্ণ *, ধ্রুৱ ৱর্ণ *,
		   সংরচনা command_options *);
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

স্থির শূন্য update_refs_stdin(unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা strbuf প্ৰবেশ = STRBUF_INIT, err = STRBUF_INIT;
	গণনা update_refs_state state = UPDATE_REFS_OPEN;
	সংরচনা ref_transaction *transaction;
	পূর্ণাঙ্ক i, j;

	সংরচনা command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	যদি (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	যেতিয়ালৈ (!strbuf_getwholeline(&প্ৰবেশ, stdin, সাৰি_termination)) {
		ধ্রুৱ সংরচনা parse_cmd *cmd = রিক্ত;

		যদি (*প্ৰবেশ.buf == সাৰি_termination)
			die("empty command in input");
		অন্যথা যদি (isspace(*প্ৰবেশ.buf))
			die("whitespace before command: %s", প্ৰবেশ.buf);

		বাবে (i = 0; i < ARRAY_SIZE(command); i++) {
			ধ্রুৱ ৱর্ণ *prefix = command[i].prefix;
			ৱর্ণ c;

			যদি (!starts_with(প্ৰবেশ.buf, prefix))
				অনুৱর্ত;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].args ? ' ' : সাৰি_termination;
			যদি (প্ৰবেশ.buf[strlen(prefix)] != c)
				অনুৱর্ত;

			cmd = &command[i];
			ভঙ্গ;
		}
		যদি (!cmd)
			die("unknown command: %s", প্ৰবেশ.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		বাবে (j = 1; সাৰি_termination == '\0' && j < cmd->args; j++)
			যদি (strbuf_appendwholeline(&প্ৰবেশ, stdin, সাৰি_termination))
				ভঙ্গ;

		ৱিকল্প (state) {
		পক্ষ UPDATE_REFS_OPEN:
		পক্ষ UPDATE_REFS_STARTED:
			যদি (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			যদি (cmd->state >= state)
				state = cmd->state;
			ভঙ্গ;
		পক্ষ UPDATE_REFS_PREPARED:
			যদি (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			ভঙ্গ;
		পক্ষ UPDATE_REFS_CLOSED:
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

			ভঙ্গ;
		}

		cmd->fn(transaction, প্ৰবেশ.buf + strlen(cmd->prefix) + !!cmd->args,
			প্ৰবেশ.buf + প্ৰবেশ.len, &opts);
	}

	ৱিকল্প (state) {
	পক্ষ UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		যদি (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, রিক্ত);
		ref_transaction_free(transaction);
		ভঙ্গ;
	পক্ষ UPDATE_REFS_STARTED:
	পক্ষ UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		যদি (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		ভঙ্গ;
	পক্ষ UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		ভঙ্গ;
	}

	strbuf_release(&err);
	strbuf_release(&প্ৰবেশ);
}

পূর্ণাঙ্ক cmd_update_ref(পূর্ণাঙ্ক argc,
		   ধ্রুৱ ৱর্ণ **argv,
		   ধ্রুৱ ৱর্ণ *prefix,
		   সংরচনা repository *repo UNUSED)
{
	ধ্রুৱ ৱর্ণ *refname, *oldval;
	সংরচনা object_id oid, oldoid;
	পূর্ণাঙ্ক delete = 0, no_deref = 0, read_stdin = 0, end_null = 0;
	পূর্ণাঙ্ক create_reflog = 0;
	unsigned পূর্ণাঙ্ক flags = 0;

	সংরচনা option options[] = {
		OPT_STRING( 'm', রিক্ত, &বাৰ্তা, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', রিক্ত, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', রিক্ত, &end_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_BIT('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, রিক্ত);
	argc = parse_options(argc, argv, prefix, options, git_update_ref_usage,
			     0);
	যদি (বাৰ্তা && !*বাৰ্তা)
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
			সাৰি_termination = '\0';
		update_refs_stdin(flags);
		ঘূৰাই 0;
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
		ধ্রুৱ ৱর্ণ *মান;
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
		ঘূৰাই refs_delete_ref(get_main_ref_store(the_repository),
				       বাৰ্তা, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : রিক্ত,
				       default_flags);
	অন্যথা
		ঘূৰাই refs_update_ref(get_main_ref_store(the_repository),
				       বাৰ্তা, refname, &oid,
				       oldval ? &oldoid : রিক্ত,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
