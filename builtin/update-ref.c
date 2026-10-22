#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "quote.h"

நிலையான மாறிலி எழுத்து * மாறிலி git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	வெற்று
};

நிலையான எழுத்து வரி_termination = '\n';
நிலையான unsigned முழு update_flags;
நிலையான unsigned முழு default_flags;
நிலையான unsigned create_reflog_flag;
நிலையான மாறிலி எழுத்து *செய்தி;

கட்டமைப்பு command_options {
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
நிலையான மாறிலி எழுத்து *parse_arg(மாறிலி எழுத்து *next, கட்டமைப்பு strbuf *arg)
{
	எனில் (*next == '"') {
		மாறிலி எழுத்து *orig = next;

		எனில் (unquote_c_style(arg, next, &next))
			die("badly quoted argument: %s", orig);
		எனில் (*next && !isspace(*next))
			die("unexpected character after quoted argument: %s", orig);
	} இல்லையெனில் {
		போது (*next && !isspace(*next))
			strbuf_addch(arg, *next++);
	}

	திரும்பு next;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
நிலையான எழுத்து *parse_refname(மாறிலி எழுத்து **next)
{
	கட்டமைப்பு strbuf ref = STRBUF_INIT;

	எனில் (வரி_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &ref);
	} இல்லையெனில் {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *next);
		*next += ref.len;
	}

	எனில் (!ref.len) {
		strbuf_release(&ref);
		திரும்பு வெற்று;
	}

	எனில் (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	திரும்பு strbuf_detach(&ref, வெற்று);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
நிலையான எழுத்து *parse_next_refname(மாறிலி எழுத்து **next)
{
	எனில் (வரி_termination) {
		/* Without -z, consume SP and use next argument */
		எனில் (!**next || **next == வரி_termination)
			திரும்பு வெற்று;
		எனில் (**next != ' ')
			die("expected SP but got: %s", *next);
	} இல்லையெனில் {
		/* With -z, read the next NUL-terminated line */
		எனில் (**next)
			திரும்பு வெற்று;
	}
	/* Skip the delimiter */
	(*next)++;

	திரும்பு parse_refname(next);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
நிலையான எழுத்து *parse_next_arg(மாறிலி எழுத்து **next)
{
	கட்டமைப்பு strbuf arg = STRBUF_INIT;

	எனில் (வரி_termination) {
		/* Without -z, consume SP and use next argument */
		எனில் (!**next || **next == வரி_termination)
			திரும்பு வெற்று;
		எனில் (**next != ' ')
			die("expected SP but got: %s", *next);
	} இல்லையெனில் {
		/* With -z, read the next NUL-terminated line */
		எனில் (**next)
			திரும்பு வெற்று;
	}
	/* Skip the delimiter */
	(*next)++;

	எனில் (வரி_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &arg);
	} இல்லையெனில் {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *next);
		*next += arg.len;
	}

	எனில் (arg.len)
		திரும்பு strbuf_detach(&arg, வெற்று);

	strbuf_release(&arg);
	திரும்பு வெற்று;
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
நிலையான முழு parse_next_oid(மாறிலி எழுத்து **next, மாறிலி எழுத்து *end,
			  கட்டமைப்பு object_id *oid,
			  மாறிலி எழுத்து *command, மாறிலி எழுத்து *refname,
			  முழு flags)
{
	கட்டமைப்பு strbuf arg = STRBUF_INIT;
	முழு ret = 0;

	எனில் (*next == end)
		செல் eof;

	எனில் (வரி_termination) {
		/* Without -z, consume SP and use next argument */
		எனில் (!**next || **next == வரி_termination)
			திரும்பு 1;
		எனில் (**next != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *next);
		(*next)++;
		*next = parse_arg(*next, &arg);
		எனில் (arg.len) {
			எனில் (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				செல் invalid;
		} இல்லையெனில் {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} இல்லையெனில் {
		/* With -z, read the next NUL-terminated line */
		எனில் (**next)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *next);
		(*next)++;
		எனில் (*next == end)
			செல் eof;
		strbuf_addstr(&arg, *next);
		*next += arg.len;

		எனில் (arg.len) {
			எனில் (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				செல் invalid;
		} இல்லையெனில் எனில் (flags & PARSE_SHA1_ALLOW_EMPTY) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} இல்லையெனில் {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	திரும்பு ret;

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

நிலையான வெறுமை print_rejected_refs(மாறிலி எழுத்து *refname,
				மாறிலி கட்டமைப்பு object_id *old_oid,
				மாறிலி கட்டமைப்பு object_id *new_oid,
				மாறிலி எழுத்து *old_target,
				மாறிலி எழுத்து *new_target,
				எண்ணிக்கை ref_transaction_error err,
				மாறிலி எழுத்து *details,
				வெறுமை *cb_data UNUSED)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	எனில் (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_to_hex(new_oid) : new_target,
		    old_oid ? oid_to_hex(old_oid) : old_target,
		    ref_transaction_error_செய்தி(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
நிலையான வெறுமை handle_ref_transaction_error(மாறிலி எழுத்து *refname,
					 கட்டமைப்பு object_id *new_oid,
					 கட்டமைப்பு object_id *old_oid,
					 மாறிலி எழுத்து *new_target,
					 மாறிலி எழுத்து *old_target,
					 எண்ணிக்கை ref_transaction_error tx_err,
					 கட்டமைப்பு strbuf *err,
					 கட்டமைப்பு command_options *opts)
{
	எனில் (!tx_err)
		திரும்பு;

	எனில் (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_target,
				    new_target, tx_err, err->buf, வெற்று);
		திரும்பு;
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

நிலையான வெறுமை parse_cmd_update(கட்டமைப்பு ref_transaction *transaction,
			     மாறிலி எழுத்து *next, மாறிலி எழுத்து *end,
			     கட்டமைப்பு command_options *opts)
{
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	எழுத்து *refname;
	கட்டமைப்பு object_id new_oid, old_oid;
	எண்ணிக்கை ref_transaction_error tx_err;
	முழு have_old;

	refname = parse_refname(&next);
	எனில் (!refname)
		die("update: missing <ref>");

	எனில் (parse_next_oid(&next, end, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_EMPTY))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_next_oid(&next, end, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	எனில் (*next != வரி_termination)
		die("update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : வெற்று,
					வெற்று, வெற்று,
					update_flags | create_reflog_flag,
					செய்தி, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : வெற்று,
				     வெற்று, வெற்று, tx_err, &err, opts);


	update_flags = default_flags;
	விடுவி(refname);
	strbuf_release(&err);
}

நிலையான வெறுமை parse_cmd_symref_update(கட்டமைப்பு ref_transaction *transaction,
				    மாறிலி எழுத்து *next, மாறிலி எழுத்து *end UNUSED,
				    கட்டமைப்பு command_options *opts)
{
	எழுத்து *refname, *new_target, *old_arg;
	எண்ணிக்கை ref_transaction_error tx_err;
	எழுத்து *old_target = வெற்று;
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	கட்டமைப்பு object_id old_oid;
	முழு have_old_oid = 0;

	refname = parse_refname(&next);
	எனில் (!refname)
		die("symref-update: missing <ref>");

	new_target = parse_next_refname(&next);
	எனில் (!new_target)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_next_arg(&next);
	எனில் (old_arg) {
		old_target = parse_next_arg(&next);
		எனில் (!old_target)
			die("symref-update %s: expected old value", refname);

		எனில் (!strcmp(old_arg, "oid")) {
			எனில் (repo_get_oid_with_flags(the_repository, old_target, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_target);

			have_old_oid = 1;
		} இல்லையெனில் எனில் (!strcmp(old_arg, "ref")) {
			எனில் (check_refname_format(old_target, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_target);
		} இல்லையெனில் {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	எனில் (*next != வரி_termination)
		die("symref-update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname, வெற்று,
					have_old_oid ? &old_oid : வெற்று,
					new_target,
					have_old_oid ? வெற்று : old_target,
					update_flags | create_reflog_flag,
					செய்தி, &err);
	handle_ref_transaction_error(refname, வெற்று, have_old_oid ? &old_oid : வெற்று,
				     new_target, have_old_oid ? வெற்று : old_target,
				     tx_err, &err, opts);

	update_flags = default_flags;
	விடுவி(refname);
	விடுவி(old_arg);
	விடுவி(old_target);
	விடுவி(new_target);
	strbuf_release(&err);
}

நிலையான வெறுமை parse_cmd_create(கட்டமைப்பு ref_transaction *transaction,
			     மாறிலி எழுத்து *next, மாறிலி எழுத்து *end,
			     கட்டமைப்பு command_options *opts)
{
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	எழுத்து *refname;
	கட்டமைப்பு object_id new_oid;
	எண்ணிக்கை ref_transaction_error tx_err;

	refname = parse_refname(&next);
	எனில் (!refname)
		die("create: missing <ref>");

	எனில் (parse_next_oid(&next, end, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	எனில் (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	எனில் (*next != வரி_termination)
		die("create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, வெற்று,
					update_flags | create_reflog_flag,
					செய்தி, &err);
	handle_ref_transaction_error(refname, &new_oid, வெற்று, வெற்று, வெற்று, tx_err,
				     &err, opts);

	update_flags = default_flags;
	விடுவி(refname);
	strbuf_release(&err);
}

நிலையான வெறுமை parse_cmd_symref_create(கட்டமைப்பு ref_transaction *transaction,
				    மாறிலி எழுத்து *next, மாறிலி எழுத்து *end UNUSED,
				    கட்டமைப்பு command_options *opts)
{
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	எழுத்து *refname, *new_target;
	எண்ணிக்கை ref_transaction_error tx_err;

	refname = parse_refname(&next);
	எனில் (!refname)
		die("symref-create: missing <ref>");

	new_target = parse_next_refname(&next);
	எனில் (!new_target)
		die("symref-create %s: missing <new-target>", refname);

	எனில் (*next != வரி_termination)
		die("symref-create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, வெற்று, new_target,
					update_flags | create_reflog_flag,
					செய்தி, &err);
	handle_ref_transaction_error(refname, வெற்று, வெற்று, new_target, வெற்று,
				     tx_err, &err, opts);

	update_flags = default_flags;
	விடுவி(refname);
	விடுவி(new_target);
	strbuf_release(&err);
}

நிலையான வெறுமை parse_cmd_delete(கட்டமைப்பு ref_transaction *transaction,
			     மாறிலி எழுத்து *next, மாறிலி எழுத்து *end,
			     கட்டமைப்பு command_options *opts UNUSED)
{
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	எழுத்து *refname;
	கட்டமைப்பு object_id old_oid;
	முழு have_old;

	refname = parse_refname(&next);
	எனில் (!refname)
		die("delete: missing <ref>");

	எனில் (parse_next_oid(&next, end, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} இல்லையெனில் {
		எனில் (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	எனில் (*next != வரி_termination)
		die("delete %s: extra input: %s", refname, next);

	எனில் (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : வெற்று,
				   வெற்று, update_flags, செய்தி, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	விடுவி(refname);
	strbuf_release(&err);
}

நிலையான வெறுமை parse_cmd_symref_delete(கட்டமைப்பு ref_transaction *transaction,
				    மாறிலி எழுத்து *next, மாறிலி எழுத்து *end UNUSED,
				    கட்டமைப்பு command_options *opts UNUSED)
{
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	எழுத்து *refname, *old_target;

	எனில் (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&next);
	எனில் (!refname)
		die("symref-delete: missing <ref>");

	old_target = parse_next_refname(&next);

	எனில் (*next != வரி_termination)
		die("symref-delete %s: extra input: %s", refname, next);

	எனில் (ref_transaction_delete(transaction, refname, வெற்று,
				   old_target, update_flags, செய்தி, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	விடுவி(refname);
	விடுவி(old_target);
	strbuf_release(&err);
}

நிலையான வெறுமை parse_cmd_verify(கட்டமைப்பு ref_transaction *transaction,
			     மாறிலி எழுத்து *next, மாறிலி எழுத்து *end,
			     கட்டமைப்பு command_options *opts UNUSED)
{
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	எழுத்து *refname;
	கட்டமைப்பு object_id old_oid;

	refname = parse_refname(&next);
	எனில் (!refname)
		die("verify: missing <ref>");

	எனில் (parse_next_oid(&next, end, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	எனில் (*next != வரி_termination)
		die("verify %s: extra input: %s", refname, next);

	எனில் (ref_transaction_verify(transaction, refname, &old_oid,
				   வெற்று, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	விடுவி(refname);
	strbuf_release(&err);
}

நிலையான வெறுமை parse_cmd_symref_verify(கட்டமைப்பு ref_transaction *transaction,
				    மாறிலி எழுத்து *next, மாறிலி எழுத்து *end UNUSED,
				    கட்டமைப்பு command_options *opts UNUSED)
{
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	கட்டமைப்பு object_id old_oid;
	எழுத்து *refname, *old_target;

	எனில் (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&next);
	எனில் (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_target = parse_next_refname(&next);
	எனில் (!old_target)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	எனில் (*next != வரி_termination)
		die("symref-verify %s: extra input: %s", refname, next);

	எனில் (ref_transaction_verify(transaction, refname,
				   old_target ? வெற்று : &old_oid,
				   old_target, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	விடுவி(refname);
	விடுவி(old_target);
	strbuf_release(&err);
}

நிலையான வெறுமை report_ok(மாறிலி எழுத்து *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

நிலையான வெறுமை parse_cmd_option(கட்டமைப்பு ref_transaction *transaction UNUSED,
			     மாறிலி எழுத்து *next, மாறிலி எழுத்து *end UNUSED,
			     கட்டமைப்பு command_options *opts UNUSED)
{
	மாறிலி எழுத்து *rest;
	எனில் (skip_prefix(next, "no-deref", &rest) && *rest == வரி_termination)
		update_flags |= REF_NO_DEREF;
	இல்லையெனில்
		die("option unknown: %s", next);
}

நிலையான வெறுமை parse_cmd_start(கட்டமைப்பு ref_transaction *transaction UNUSED,
			    மாறிலி எழுத்து *next, மாறிலி எழுத்து *end UNUSED,
			    கட்டமைப்பு command_options *opts UNUSED)
{
	எனில் (*next != வரி_termination)
		die("start: extra input: %s", next);
	report_ok("start");
}

நிலையான வெறுமை parse_cmd_prepare(கட்டமைப்பு ref_transaction *transaction,
			      மாறிலி எழுத்து *next, மாறிலி எழுத்து *end UNUSED,
			      கட்டமைப்பு command_options *opts UNUSED)
{
	கட்டமைப்பு strbuf error = STRBUF_INIT;
	எனில் (*next != வரி_termination)
		die("prepare: extra input: %s", next);
	எனில் (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

நிலையான வெறுமை parse_cmd_abort(கட்டமைப்பு ref_transaction *transaction,
			    மாறிலி எழுத்து *next, மாறிலி எழுத்து *end UNUSED,
			    கட்டமைப்பு command_options *opts UNUSED)
{
	கட்டமைப்பு strbuf error = STRBUF_INIT;
	எனில் (*next != வரி_termination)
		die("abort: extra input: %s", next);
	எனில் (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

நிலையான வெறுமை parse_cmd_commit(கட்டமைப்பு ref_transaction *transaction,
			     மாறிலி எழுத்து *next, மாறிலி எழுத்து *end UNUSED,
			     கட்டமைப்பு command_options *opts UNUSED)
{
	கட்டமைப்பு strbuf error = STRBUF_INIT;
	எனில் (*next != வரி_termination)
		die("commit: extra input: %s", next);
	எனில் (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, வெற்று);

	report_ok("commit");
	ref_transaction_free(transaction);
}

எண்ணிக்கை update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

நிலையான மாறிலி கட்டமைப்பு parse_cmd {
	மாறிலி எழுத்து *prefix;
	வெறுமை (*fn)(கட்டமைப்பு ref_transaction *, மாறிலி எழுத்து *, மாறிலி எழுத்து *,
		   கட்டமைப்பு command_options *);
	unsigned args;
	எண்ணிக்கை update_refs_state state;
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

நிலையான வெறுமை update_refs_stdin(unsigned முழு flags)
{
	கட்டமைப்பு strbuf உள்ளீடு = STRBUF_INIT, err = STRBUF_INIT;
	எண்ணிக்கை update_refs_state state = UPDATE_REFS_OPEN;
	கட்டமைப்பு ref_transaction *transaction;
	முழு i, j;

	கட்டமைப்பு command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	எனில் (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	போது (!strbuf_getwholeline(&உள்ளீடு, stdin, வரி_termination)) {
		மாறிலி கட்டமைப்பு parse_cmd *cmd = வெற்று;

		எனில் (*உள்ளீடு.buf == வரி_termination)
			die("empty command in input");
		இல்லையெனில் எனில் (isspace(*உள்ளீடு.buf))
			die("whitespace before command: %s", உள்ளீடு.buf);

		க்கு (i = 0; i < ARRAY_SIZE(command); i++) {
			மாறிலி எழுத்து *prefix = command[i].prefix;
			எழுத்து c;

			எனில் (!starts_with(உள்ளீடு.buf, prefix))
				தொடர்;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].args ? ' ' : வரி_termination;
			எனில் (உள்ளீடு.buf[strlen(prefix)] != c)
				தொடர்;

			cmd = &command[i];
			நிறுத்து;
		}
		எனில் (!cmd)
			die("unknown command: %s", உள்ளீடு.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		க்கு (j = 1; வரி_termination == '\0' && j < cmd->args; j++)
			எனில் (strbuf_appendwholeline(&உள்ளீடு, stdin, வரி_termination))
				நிறுத்து;

		தேர்வு (state) {
		நிலை UPDATE_REFS_OPEN:
		நிலை UPDATE_REFS_STARTED:
			எனில் (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			எனில் (cmd->state >= state)
				state = cmd->state;
			நிறுத்து;
		நிலை UPDATE_REFS_PREPARED:
			எனில் (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			நிறுத்து;
		நிலை UPDATE_REFS_CLOSED:
			எனில் (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			எனில் (!transaction)
				die("%s", err.buf);

			நிறுத்து;
		}

		cmd->fn(transaction, உள்ளீடு.buf + strlen(cmd->prefix) + !!cmd->args,
			உள்ளீடு.buf + உள்ளீடு.len, &opts);
	}

	தேர்வு (state) {
	நிலை UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		எனில் (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, வெற்று);
		ref_transaction_free(transaction);
		நிறுத்து;
	நிலை UPDATE_REFS_STARTED:
	நிலை UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		எனில் (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		நிறுத்து;
	நிலை UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		நிறுத்து;
	}

	strbuf_release(&err);
	strbuf_release(&உள்ளீடு);
}

முழு cmd_update_ref(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	மாறிலி எழுத்து *refname, *oldval;
	கட்டமைப்பு object_id oid, oldoid;
	முழு delete = 0, no_deref = 0, read_stdin = 0, end_null = 0;
	முழு create_reflog = 0;
	unsigned முழு flags = 0;

	கட்டமைப்பு option options[] = {
		OPT_STRING( 'm', வெற்று, &செய்தி, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', வெற்று, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', வெற்று, &end_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_BIT('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, வெற்று);
	argc = parse_options(argc, argv, prefix, options, git_update_ref_usage,
			     0);
	எனில் (செய்தி && !*செய்தி)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	எனில் (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	எனில் (read_stdin) {
		எனில் (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		எனில் (end_null)
			வரி_termination = '\0';
		update_refs_stdin(flags);
		திரும்பு 0;
	} இல்லையெனில் எனில் (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	எனில் (end_null)
		usage_with_options(git_update_ref_usage, options);

	எனில் (delete) {
		எனில் (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} இல்லையெனில் {
		மாறிலி எழுத்து *மதிப்பு;
		எனில் (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		மதிப்பு = argv[1];
		oldval = argv[2];
		எனில் (repo_get_oid_with_flags(the_repository, மதிப்பு, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", மதிப்பு);
	}

	எனில் (oldval) {
		எனில் (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		இல்லையெனில் எனில் (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	எனில் (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		திரும்பு refs_delete_ref(get_main_ref_store(the_repository),
				       செய்தி, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : வெற்று,
				       default_flags);
	இல்லையெனில்
		திரும்பு refs_update_ref(get_main_ref_store(the_repository),
				       செய்தி, refname, &oid,
				       oldval ? &oldoid : வெற்று,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
