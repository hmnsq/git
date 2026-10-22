#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ವರ್ಣ ಸಾಲು_termination = '\n';
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ update_flags;
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ default_flags;
ಸ್ಥಿರ unsigned create_reflog_flag;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ;

ಸಂರಚನಾ command_options {
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
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *parse_arg(ಧ್ರುವ ವರ್ಣ *next, ಸಂರಚನಾ strbuf *arg)
{
	ಯದಿ (*next == '"') {
		ಧ್ರುವ ವರ್ಣ *orig = next;

		ಯದಿ (unquote_c_style(arg, next, &next))
			die("badly quoted argument: %s", orig);
		ಯದಿ (*next && !isspace(*next))
			die("unexpected character after quoted argument: %s", orig);
	} ಅನ್ಯಥಾ {
		ಯಾವತ್ (*next && !isspace(*next))
			strbuf_addch(arg, *next++);
	}

	ಪ್ರತ್ಯಾಗಮ next;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
ಸ್ಥಿರ ವರ್ಣ *parse_refname(ಧ್ರುವ ವರ್ಣ **next)
{
	ಸಂರಚನಾ strbuf ref = STRBUF_INIT;

	ಯದಿ (ಸಾಲು_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &ref);
	} ಅನ್ಯಥಾ {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *next);
		*next += ref.len;
	}

	ಯದಿ (!ref.len) {
		strbuf_release(&ref);
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}

	ಯದಿ (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	ಪ್ರತ್ಯಾಗಮ strbuf_detach(&ref, ರಿಕ್ತ);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
ಸ್ಥಿರ ವರ್ಣ *parse_next_refname(ಧ್ರುವ ವರ್ಣ **next)
{
	ಯದಿ (ಸಾಲು_termination) {
		/* Without -z, consume SP and use next argument */
		ಯದಿ (!**next || **next == ಸಾಲು_termination)
			ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
		ಯದಿ (**next != ' ')
			die("expected SP but got: %s", *next);
	} ಅನ್ಯಥಾ {
		/* With -z, read the next NUL-terminated line */
		ಯದಿ (**next)
			ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}
	/* Skip the delimiter */
	(*next)++;

	ಪ್ರತ್ಯಾಗಮ parse_refname(next);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
ಸ್ಥಿರ ವರ್ಣ *parse_next_arg(ಧ್ರುವ ವರ್ಣ **next)
{
	ಸಂರಚನಾ strbuf arg = STRBUF_INIT;

	ಯದಿ (ಸಾಲು_termination) {
		/* Without -z, consume SP and use next argument */
		ಯದಿ (!**next || **next == ಸಾಲು_termination)
			ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
		ಯದಿ (**next != ' ')
			die("expected SP but got: %s", *next);
	} ಅನ್ಯಥಾ {
		/* With -z, read the next NUL-terminated line */
		ಯದಿ (**next)
			ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}
	/* Skip the delimiter */
	(*next)++;

	ಯದಿ (ಸಾಲು_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &arg);
	} ಅನ್ಯಥಾ {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *next);
		*next += arg.len;
	}

	ಯದಿ (arg.len)
		ಪ್ರತ್ಯಾಗಮ strbuf_detach(&arg, ರಿಕ್ತ);

	strbuf_release(&arg);
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_next_oid(ಧ್ರುವ ವರ್ಣ **next, ಧ್ರುವ ವರ್ಣ *end,
			  ಸಂರಚನಾ object_id *oid,
			  ಧ್ರುವ ವರ್ಣ *command, ಧ್ರುವ ವರ್ಣ *refname,
			  ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ strbuf arg = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಯದಿ (*next == end)
		ಗಚ್ಛ eof;

	ಯದಿ (ಸಾಲು_termination) {
		/* Without -z, consume SP and use next argument */
		ಯದಿ (!**next || **next == ಸಾಲು_termination)
			ಪ್ರತ್ಯಾಗಮ 1;
		ಯದಿ (**next != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *next);
		(*next)++;
		*next = parse_arg(*next, &arg);
		ಯದಿ (arg.len) {
			ಯದಿ (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				ಗಚ್ಛ invalid;
		} ಅನ್ಯಥಾ {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} ಅನ್ಯಥಾ {
		/* With -z, read the next NUL-terminated line */
		ಯದಿ (**next)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *next);
		(*next)++;
		ಯದಿ (*next == end)
			ಗಚ್ಛ eof;
		strbuf_addstr(&arg, *next);
		*next += arg.len;

		ಯದಿ (arg.len) {
			ಯದಿ (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				ಗಚ್ಛ invalid;
		} ಅನ್ಯಥಾ ಯದಿ (flags & PARSE_SHA1_ALLOW_EMPTY) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} ಅನ್ಯಥಾ {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	ಪ್ರತ್ಯಾಗಮ ret;

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

ಸ್ಥಿರ ಶೂನ್ಯ print_rejected_refs(ಧ್ರುವ ವರ್ಣ *refname,
				ಧ್ರುವ ಸಂರಚನಾ object_id *old_oid,
				ಧ್ರುವ ಸಂರಚನಾ object_id *new_oid,
				ಧ್ರುವ ವರ್ಣ *old_target,
				ಧ್ರುವ ವರ್ಣ *new_target,
				ಗಣನಾ ref_transaction_error err,
				ಧ್ರುವ ವರ್ಣ *details,
				ಶೂನ್ಯ *cb_data UNUSED)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	ಯದಿ (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_to_hex(new_oid) : new_target,
		    old_oid ? oid_to_hex(old_oid) : old_target,
		    ref_transaction_error_ಸಂದೇಶ(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
ಸ್ಥಿರ ಶೂನ್ಯ handle_ref_transaction_error(ಧ್ರುವ ವರ್ಣ *refname,
					 ಸಂರಚನಾ object_id *new_oid,
					 ಸಂರಚನಾ object_id *old_oid,
					 ಧ್ರುವ ವರ್ಣ *new_target,
					 ಧ್ರುವ ವರ್ಣ *old_target,
					 ಗಣನಾ ref_transaction_error tx_err,
					 ಸಂರಚನಾ strbuf *err,
					 ಸಂರಚನಾ command_options *opts)
{
	ಯದಿ (!tx_err)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_target,
				    new_target, tx_err, err->buf, ರಿಕ್ತ);
		ಪ್ರತ್ಯಾಗಮ;
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

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_update(ಸಂರಚನಾ ref_transaction *transaction,
			     ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end,
			     ಸಂರಚನಾ command_options *opts)
{
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ವರ್ಣ *refname;
	ಸಂರಚನಾ object_id new_oid, old_oid;
	ಗಣನಾ ref_transaction_error tx_err;
	ಪೂರ್ಣಾಙ್ಕ have_old;

	refname = parse_refname(&next);
	ಯದಿ (!refname)
		die("update: missing <ref>");

	ಯದಿ (parse_next_oid(&next, end, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_EMPTY))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_next_oid(&next, end, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	ಯದಿ (*next != ಸಾಲು_termination)
		die("update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : ರಿಕ್ತ,
					ರಿಕ್ತ, ರಿಕ್ತ,
					update_flags | create_reflog_flag,
					ಸಂದೇಶ, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : ರಿಕ್ತ,
				     ರಿಕ್ತ, ರಿಕ್ತ, tx_err, &err, opts);


	update_flags = default_flags;
	ಮುಕ್ತ(refname);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_symref_update(ಸಂರಚನಾ ref_transaction *transaction,
				    ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end UNUSED,
				    ಸಂರಚನಾ command_options *opts)
{
	ವರ್ಣ *refname, *new_target, *old_arg;
	ಗಣನಾ ref_transaction_error tx_err;
	ವರ್ಣ *old_target = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಸಂರಚನಾ object_id old_oid;
	ಪೂರ್ಣಾಙ್ಕ have_old_oid = 0;

	refname = parse_refname(&next);
	ಯದಿ (!refname)
		die("symref-update: missing <ref>");

	new_target = parse_next_refname(&next);
	ಯದಿ (!new_target)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_next_arg(&next);
	ಯದಿ (old_arg) {
		old_target = parse_next_arg(&next);
		ಯದಿ (!old_target)
			die("symref-update %s: expected old value", refname);

		ಯದಿ (!strcmp(old_arg, "oid")) {
			ಯದಿ (repo_get_oid_with_flags(the_repository, old_target, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_target);

			have_old_oid = 1;
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(old_arg, "ref")) {
			ಯದಿ (check_refname_format(old_target, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_target);
		} ಅನ್ಯಥಾ {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	ಯದಿ (*next != ಸಾಲು_termination)
		die("symref-update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname, ರಿಕ್ತ,
					have_old_oid ? &old_oid : ರಿಕ್ತ,
					new_target,
					have_old_oid ? ರಿಕ್ತ : old_target,
					update_flags | create_reflog_flag,
					ಸಂದೇಶ, &err);
	handle_ref_transaction_error(refname, ರಿಕ್ತ, have_old_oid ? &old_oid : ರಿಕ್ತ,
				     new_target, have_old_oid ? ರಿಕ್ತ : old_target,
				     tx_err, &err, opts);

	update_flags = default_flags;
	ಮುಕ್ತ(refname);
	ಮುಕ್ತ(old_arg);
	ಮುಕ್ತ(old_target);
	ಮುಕ್ತ(new_target);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_create(ಸಂರಚನಾ ref_transaction *transaction,
			     ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end,
			     ಸಂರಚನಾ command_options *opts)
{
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ವರ್ಣ *refname;
	ಸಂರಚನಾ object_id new_oid;
	ಗಣನಾ ref_transaction_error tx_err;

	refname = parse_refname(&next);
	ಯದಿ (!refname)
		die("create: missing <ref>");

	ಯದಿ (parse_next_oid(&next, end, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	ಯದಿ (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	ಯದಿ (*next != ಸಾಲು_termination)
		die("create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, ರಿಕ್ತ,
					update_flags | create_reflog_flag,
					ಸಂದೇಶ, &err);
	handle_ref_transaction_error(refname, &new_oid, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, tx_err,
				     &err, opts);

	update_flags = default_flags;
	ಮುಕ್ತ(refname);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_symref_create(ಸಂರಚನಾ ref_transaction *transaction,
				    ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end UNUSED,
				    ಸಂರಚನಾ command_options *opts)
{
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ವರ್ಣ *refname, *new_target;
	ಗಣನಾ ref_transaction_error tx_err;

	refname = parse_refname(&next);
	ಯದಿ (!refname)
		die("symref-create: missing <ref>");

	new_target = parse_next_refname(&next);
	ಯದಿ (!new_target)
		die("symref-create %s: missing <new-target>", refname);

	ಯದಿ (*next != ಸಾಲು_termination)
		die("symref-create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, ರಿಕ್ತ, new_target,
					update_flags | create_reflog_flag,
					ಸಂದೇಶ, &err);
	handle_ref_transaction_error(refname, ರಿಕ್ತ, ರಿಕ್ತ, new_target, ರಿಕ್ತ,
				     tx_err, &err, opts);

	update_flags = default_flags;
	ಮುಕ್ತ(refname);
	ಮುಕ್ತ(new_target);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_delete(ಸಂರಚನಾ ref_transaction *transaction,
			     ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end,
			     ಸಂರಚನಾ command_options *opts UNUSED)
{
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ವರ್ಣ *refname;
	ಸಂರಚನಾ object_id old_oid;
	ಪೂರ್ಣಾಙ್ಕ have_old;

	refname = parse_refname(&next);
	ಯದಿ (!refname)
		die("delete: missing <ref>");

	ಯದಿ (parse_next_oid(&next, end, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} ಅನ್ಯಥಾ {
		ಯದಿ (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	ಯದಿ (*next != ಸಾಲು_termination)
		die("delete %s: extra input: %s", refname, next);

	ಯದಿ (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : ರಿಕ್ತ,
				   ರಿಕ್ತ, update_flags, ಸಂದೇಶ, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ಮುಕ್ತ(refname);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_symref_delete(ಸಂರಚನಾ ref_transaction *transaction,
				    ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end UNUSED,
				    ಸಂರಚನಾ command_options *opts UNUSED)
{
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ವರ್ಣ *refname, *old_target;

	ಯದಿ (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&next);
	ಯದಿ (!refname)
		die("symref-delete: missing <ref>");

	old_target = parse_next_refname(&next);

	ಯದಿ (*next != ಸಾಲು_termination)
		die("symref-delete %s: extra input: %s", refname, next);

	ಯದಿ (ref_transaction_delete(transaction, refname, ರಿಕ್ತ,
				   old_target, update_flags, ಸಂದೇಶ, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ಮುಕ್ತ(refname);
	ಮುಕ್ತ(old_target);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_verify(ಸಂರಚನಾ ref_transaction *transaction,
			     ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end,
			     ಸಂರಚನಾ command_options *opts UNUSED)
{
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ವರ್ಣ *refname;
	ಸಂರಚನಾ object_id old_oid;

	refname = parse_refname(&next);
	ಯದಿ (!refname)
		die("verify: missing <ref>");

	ಯದಿ (parse_next_oid(&next, end, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	ಯದಿ (*next != ಸಾಲು_termination)
		die("verify %s: extra input: %s", refname, next);

	ಯದಿ (ref_transaction_verify(transaction, refname, &old_oid,
				   ರಿಕ್ತ, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ಮುಕ್ತ(refname);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_symref_verify(ಸಂರಚನಾ ref_transaction *transaction,
				    ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end UNUSED,
				    ಸಂರಚನಾ command_options *opts UNUSED)
{
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಸಂರಚನಾ object_id old_oid;
	ವರ್ಣ *refname, *old_target;

	ಯದಿ (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&next);
	ಯದಿ (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_target = parse_next_refname(&next);
	ಯದಿ (!old_target)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	ಯದಿ (*next != ಸಾಲು_termination)
		die("symref-verify %s: extra input: %s", refname, next);

	ಯದಿ (ref_transaction_verify(transaction, refname,
				   old_target ? ರಿಕ್ತ : &old_oid,
				   old_target, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ಮುಕ್ತ(refname);
	ಮುಕ್ತ(old_target);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ report_ok(ಧ್ರುವ ವರ್ಣ *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_option(ಸಂರಚನಾ ref_transaction *transaction UNUSED,
			     ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end UNUSED,
			     ಸಂರಚನಾ command_options *opts UNUSED)
{
	ಧ್ರುವ ವರ್ಣ *rest;
	ಯದಿ (skip_prefix(next, "no-deref", &rest) && *rest == ಸಾಲು_termination)
		update_flags |= REF_NO_DEREF;
	ಅನ್ಯಥಾ
		die("option unknown: %s", next);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_start(ಸಂರಚನಾ ref_transaction *transaction UNUSED,
			    ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end UNUSED,
			    ಸಂರಚನಾ command_options *opts UNUSED)
{
	ಯದಿ (*next != ಸಾಲು_termination)
		die("start: extra input: %s", next);
	report_ok("start");
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_prepare(ಸಂರಚನಾ ref_transaction *transaction,
			      ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end UNUSED,
			      ಸಂರಚನಾ command_options *opts UNUSED)
{
	ಸಂರಚನಾ strbuf error = STRBUF_INIT;
	ಯದಿ (*next != ಸಾಲು_termination)
		die("prepare: extra input: %s", next);
	ಯದಿ (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_abort(ಸಂರಚನಾ ref_transaction *transaction,
			    ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end UNUSED,
			    ಸಂರಚನಾ command_options *opts UNUSED)
{
	ಸಂರಚನಾ strbuf error = STRBUF_INIT;
	ಯದಿ (*next != ಸಾಲು_termination)
		die("abort: extra input: %s", next);
	ಯದಿ (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_commit(ಸಂರಚನಾ ref_transaction *transaction,
			     ಧ್ರುವ ವರ್ಣ *next, ಧ್ರುವ ವರ್ಣ *end UNUSED,
			     ಸಂರಚನಾ command_options *opts UNUSED)
{
	ಸಂರಚನಾ strbuf error = STRBUF_INIT;
	ಯದಿ (*next != ಸಾಲು_termination)
		die("commit: extra input: %s", next);
	ಯದಿ (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ರಿಕ್ತ);

	report_ok("commit");
	ref_transaction_free(transaction);
}

ಗಣನಾ update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ parse_cmd {
	ಧ್ರುವ ವರ್ಣ *prefix;
	ಶೂನ್ಯ (*fn)(ಸಂರಚನಾ ref_transaction *, ಧ್ರುವ ವರ್ಣ *, ಧ್ರುವ ವರ್ಣ *,
		   ಸಂರಚನಾ command_options *);
	unsigned args;
	ಗಣನಾ update_refs_state state;
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

ಸ್ಥಿರ ಶೂನ್ಯ update_refs_stdin(unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ strbuf ಒಳಹರಿವು = STRBUF_INIT, err = STRBUF_INIT;
	ಗಣನಾ update_refs_state state = UPDATE_REFS_OPEN;
	ಸಂರಚನಾ ref_transaction *transaction;
	ಪೂರ್ಣಾಙ್ಕ i, j;

	ಸಂರಚನಾ command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	ಯದಿ (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	ಯಾವತ್ (!strbuf_getwholeline(&ಒಳಹರಿವು, stdin, ಸಾಲು_termination)) {
		ಧ್ರುವ ಸಂರಚನಾ parse_cmd *cmd = ರಿಕ್ತ;

		ಯದಿ (*ಒಳಹರಿವು.buf == ಸಾಲು_termination)
			die("empty command in input");
		ಅನ್ಯಥಾ ಯದಿ (isspace(*ಒಳಹರಿವು.buf))
			die("whitespace before command: %s", ಒಳಹರಿವು.buf);

		ಪ್ರತಿ (i = 0; i < ARRAY_SIZE(command); i++) {
			ಧ್ರುವ ವರ್ಣ *prefix = command[i].prefix;
			ವರ್ಣ c;

			ಯದಿ (!starts_with(ಒಳಹರಿವು.buf, prefix))
				ಅನುವರ್ತ;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].args ? ' ' : ಸಾಲು_termination;
			ಯದಿ (ಒಳಹರಿವು.buf[strlen(prefix)] != c)
				ಅನುವರ್ತ;

			cmd = &command[i];
			ಭಙ್ಗ;
		}
		ಯದಿ (!cmd)
			die("unknown command: %s", ಒಳಹರಿವು.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		ಪ್ರತಿ (j = 1; ಸಾಲು_termination == '\0' && j < cmd->args; j++)
			ಯದಿ (strbuf_appendwholeline(&ಒಳಹರಿವು, stdin, ಸಾಲು_termination))
				ಭಙ್ಗ;

		ವಿಕಲ್ಪ (state) {
		ಪಕ್ಷ UPDATE_REFS_OPEN:
		ಪಕ್ಷ UPDATE_REFS_STARTED:
			ಯದಿ (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			ಯದಿ (cmd->state >= state)
				state = cmd->state;
			ಭಙ್ಗ;
		ಪಕ್ಷ UPDATE_REFS_PREPARED:
			ಯದಿ (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			ಭಙ್ಗ;
		ಪಕ್ಷ UPDATE_REFS_CLOSED:
			ಯದಿ (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			ಯದಿ (!transaction)
				die("%s", err.buf);

			ಭಙ್ಗ;
		}

		cmd->fn(transaction, ಒಳಹರಿವು.buf + strlen(cmd->prefix) + !!cmd->args,
			ಒಳಹರಿವು.buf + ಒಳಹರಿವು.len, &opts);
	}

	ವಿಕಲ್ಪ (state) {
	ಪಕ್ಷ UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		ಯದಿ (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ರಿಕ್ತ);
		ref_transaction_free(transaction);
		ಭಙ್ಗ;
	ಪಕ್ಷ UPDATE_REFS_STARTED:
	ಪಕ್ಷ UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		ಯದಿ (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		ಭಙ್ಗ;
	ಪಕ್ಷ UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		ಭಙ್ಗ;
	}

	strbuf_release(&err);
	strbuf_release(&ಒಳಹರಿವು);
}

ಪೂರ್ಣಾಙ್ಕ cmd_update_ref(ಪೂರ್ಣಾಙ್ಕ argc,
		   ಧ್ರುವ ವರ್ಣ **argv,
		   ಧ್ರುವ ವರ್ಣ *prefix,
		   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಧ್ರುವ ವರ್ಣ *refname, *oldval;
	ಸಂರಚನಾ object_id oid, oldoid;
	ಪೂರ್ಣಾಙ್ಕ delete = 0, no_deref = 0, read_stdin = 0, end_null = 0;
	ಪೂರ್ಣಾಙ್ಕ create_reflog = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ flags = 0;

	ಸಂರಚನಾ option options[] = {
		OPT_STRING( 'm', ರಿಕ್ತ, &ಸಂದೇಶ, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', ರಿಕ್ತ, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', ರಿಕ್ತ, &end_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_BIT('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
	argc = parse_options(argc, argv, prefix, options, git_update_ref_usage,
			     0);
	ಯದಿ (ಸಂದೇಶ && !*ಸಂದೇಶ)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	ಯದಿ (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	ಯದಿ (read_stdin) {
		ಯದಿ (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		ಯದಿ (end_null)
			ಸಾಲು_termination = '\0';
		update_refs_stdin(flags);
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	ಯದಿ (end_null)
		usage_with_options(git_update_ref_usage, options);

	ಯದಿ (delete) {
		ಯದಿ (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} ಅನ್ಯಥಾ {
		ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ;
		ಯದಿ (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		ಮೌಲ್ಯ = argv[1];
		oldval = argv[2];
		ಯದಿ (repo_get_oid_with_flags(the_repository, ಮೌಲ್ಯ, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", ಮೌಲ್ಯ);
	}

	ಯದಿ (oldval) {
		ಯದಿ (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		ಅನ್ಯಥಾ ಯದಿ (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	ಯದಿ (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		ಪ್ರತ್ಯಾಗಮ refs_delete_ref(get_main_ref_store(the_repository),
				       ಸಂದೇಶ, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : ರಿಕ್ತ,
				       default_flags);
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ refs_update_ref(get_main_ref_store(the_repository),
				       ಸಂದೇಶ, refname, &oid,
				       oldval ? &oldoid : ರಿಕ್ತ,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
