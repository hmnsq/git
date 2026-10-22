#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "hex.h"
#شامل "refs.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "quote.h"

ایستا ثابت نویسه * ثابت git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	پوچ
};

ایستا نویسه سطر_termination = '\n';
ایستا unsigned صحیح update_flags;
ایستا unsigned صحیح default_flags;
ایستا unsigned create_reflog_flag;
ایستا ثابت نویسه *پیام;

ساختار command_options {
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
ایستا ثابت نویسه *parse_arg(ثابت نویسه *next, ساختار strbuf *arg)
{
	اگر (*next == '"') {
		ثابت نویسه *orig = next;

		اگر (unquote_c_style(arg, next, &next))
			die("badly quoted argument: %s", orig);
		اگر (*next && !isspace(*next))
			die("unexpected character after quoted argument: %s", orig);
	} وگرنه {
		وقتی (*next && !isspace(*next))
			strbuf_addch(arg, *next++);
	}

	بازگشت next;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
ایستا نویسه *parse_refname(ثابت نویسه **next)
{
	ساختار strbuf ref = STRBUF_INIT;

	اگر (سطر_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &ref);
	} وگرنه {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *next);
		*next += ref.len;
	}

	اگر (!ref.len) {
		strbuf_release(&ref);
		بازگشت پوچ;
	}

	اگر (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	بازگشت strbuf_detach(&ref, پوچ);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
ایستا نویسه *parse_next_refname(ثابت نویسه **next)
{
	اگر (سطر_termination) {
		/* Without -z, consume SP and use next argument */
		اگر (!**next || **next == سطر_termination)
			بازگشت پوچ;
		اگر (**next != ' ')
			die("expected SP but got: %s", *next);
	} وگرنه {
		/* With -z, read the next NUL-terminated line */
		اگر (**next)
			بازگشت پوچ;
	}
	/* Skip the delimiter */
	(*next)++;

	بازگشت parse_refname(next);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
ایستا نویسه *parse_next_arg(ثابت نویسه **next)
{
	ساختار strbuf arg = STRBUF_INIT;

	اگر (سطر_termination) {
		/* Without -z, consume SP and use next argument */
		اگر (!**next || **next == سطر_termination)
			بازگشت پوچ;
		اگر (**next != ' ')
			die("expected SP but got: %s", *next);
	} وگرنه {
		/* With -z, read the next NUL-terminated line */
		اگر (**next)
			بازگشت پوچ;
	}
	/* Skip the delimiter */
	(*next)++;

	اگر (سطر_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &arg);
	} وگرنه {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *next);
		*next += arg.len;
	}

	اگر (arg.len)
		بازگشت strbuf_detach(&arg, پوچ);

	strbuf_release(&arg);
	بازگشت پوچ;
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
ایستا صحیح parse_next_oid(ثابت نویسه **next, ثابت نویسه *end,
			  ساختار object_id *oid,
			  ثابت نویسه *command, ثابت نویسه *refname,
			  صحیح flags)
{
	ساختار strbuf arg = STRBUF_INIT;
	صحیح ret = 0;

	اگر (*next == end)
		برو eof;

	اگر (سطر_termination) {
		/* Without -z, consume SP and use next argument */
		اگر (!**next || **next == سطر_termination)
			بازگشت 1;
		اگر (**next != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *next);
		(*next)++;
		*next = parse_arg(*next, &arg);
		اگر (arg.len) {
			اگر (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				برو invalid;
		} وگرنه {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} وگرنه {
		/* With -z, read the next NUL-terminated line */
		اگر (**next)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *next);
		(*next)++;
		اگر (*next == end)
			برو eof;
		strbuf_addstr(&arg, *next);
		*next += arg.len;

		اگر (arg.len) {
			اگر (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				برو invalid;
		} وگرنه اگر (flags & PARSE_SHA1_ALLOW_EMPTY) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} وگرنه {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	بازگشت ret;

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

ایستا تهی print_rejected_refs(ثابت نویسه *refname,
				ثابت ساختار object_id *old_oid,
				ثابت ساختار object_id *new_oid,
				ثابت نویسه *old_target,
				ثابت نویسه *new_target,
				شمارش ref_transaction_error err,
				ثابت نویسه *details,
				تهی *cb_data UNUSED)
{
	ساختار strbuf sb = STRBUF_INIT;

	اگر (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_to_hex(new_oid) : new_target,
		    old_oid ? oid_to_hex(old_oid) : old_target,
		    ref_transaction_error_پیام(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
ایستا تهی handle_ref_transaction_error(ثابت نویسه *refname,
					 ساختار object_id *new_oid,
					 ساختار object_id *old_oid,
					 ثابت نویسه *new_target,
					 ثابت نویسه *old_target,
					 شمارش ref_transaction_error tx_err,
					 ساختار strbuf *err,
					 ساختار command_options *opts)
{
	اگر (!tx_err)
		بازگشت;

	اگر (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_target,
				    new_target, tx_err, err->buf, پوچ);
		بازگشت;
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

ایستا تهی parse_cmd_update(ساختار ref_transaction *transaction,
			     ثابت نویسه *next, ثابت نویسه *end,
			     ساختار command_options *opts)
{
	ساختار strbuf err = STRBUF_INIT;
	نویسه *refname;
	ساختار object_id new_oid, old_oid;
	شمارش ref_transaction_error tx_err;
	صحیح have_old;

	refname = parse_refname(&next);
	اگر (!refname)
		die("update: missing <ref>");

	اگر (parse_next_oid(&next, end, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_EMPTY))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_next_oid(&next, end, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	اگر (*next != سطر_termination)
		die("update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : پوچ,
					پوچ, پوچ,
					update_flags | create_reflog_flag,
					پیام, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : پوچ,
				     پوچ, پوچ, tx_err, &err, opts);


	update_flags = default_flags;
	آزاد(refname);
	strbuf_release(&err);
}

ایستا تهی parse_cmd_symref_update(ساختار ref_transaction *transaction,
				    ثابت نویسه *next, ثابت نویسه *end UNUSED,
				    ساختار command_options *opts)
{
	نویسه *refname, *new_target, *old_arg;
	شمارش ref_transaction_error tx_err;
	نویسه *old_target = پوچ;
	ساختار strbuf err = STRBUF_INIT;
	ساختار object_id old_oid;
	صحیح have_old_oid = 0;

	refname = parse_refname(&next);
	اگر (!refname)
		die("symref-update: missing <ref>");

	new_target = parse_next_refname(&next);
	اگر (!new_target)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_next_arg(&next);
	اگر (old_arg) {
		old_target = parse_next_arg(&next);
		اگر (!old_target)
			die("symref-update %s: expected old value", refname);

		اگر (!strcmp(old_arg, "oid")) {
			اگر (repo_get_oid_with_flags(the_repository, old_target, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_target);

			have_old_oid = 1;
		} وگرنه اگر (!strcmp(old_arg, "ref")) {
			اگر (check_refname_format(old_target, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_target);
		} وگرنه {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	اگر (*next != سطر_termination)
		die("symref-update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname, پوچ,
					have_old_oid ? &old_oid : پوچ,
					new_target,
					have_old_oid ? پوچ : old_target,
					update_flags | create_reflog_flag,
					پیام, &err);
	handle_ref_transaction_error(refname, پوچ, have_old_oid ? &old_oid : پوچ,
				     new_target, have_old_oid ? پوچ : old_target,
				     tx_err, &err, opts);

	update_flags = default_flags;
	آزاد(refname);
	آزاد(old_arg);
	آزاد(old_target);
	آزاد(new_target);
	strbuf_release(&err);
}

ایستا تهی parse_cmd_create(ساختار ref_transaction *transaction,
			     ثابت نویسه *next, ثابت نویسه *end,
			     ساختار command_options *opts)
{
	ساختار strbuf err = STRBUF_INIT;
	نویسه *refname;
	ساختار object_id new_oid;
	شمارش ref_transaction_error tx_err;

	refname = parse_refname(&next);
	اگر (!refname)
		die("create: missing <ref>");

	اگر (parse_next_oid(&next, end, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	اگر (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	اگر (*next != سطر_termination)
		die("create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, پوچ,
					update_flags | create_reflog_flag,
					پیام, &err);
	handle_ref_transaction_error(refname, &new_oid, پوچ, پوچ, پوچ, tx_err,
				     &err, opts);

	update_flags = default_flags;
	آزاد(refname);
	strbuf_release(&err);
}

ایستا تهی parse_cmd_symref_create(ساختار ref_transaction *transaction,
				    ثابت نویسه *next, ثابت نویسه *end UNUSED,
				    ساختار command_options *opts)
{
	ساختار strbuf err = STRBUF_INIT;
	نویسه *refname, *new_target;
	شمارش ref_transaction_error tx_err;

	refname = parse_refname(&next);
	اگر (!refname)
		die("symref-create: missing <ref>");

	new_target = parse_next_refname(&next);
	اگر (!new_target)
		die("symref-create %s: missing <new-target>", refname);

	اگر (*next != سطر_termination)
		die("symref-create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, پوچ, new_target,
					update_flags | create_reflog_flag,
					پیام, &err);
	handle_ref_transaction_error(refname, پوچ, پوچ, new_target, پوچ,
				     tx_err, &err, opts);

	update_flags = default_flags;
	آزاد(refname);
	آزاد(new_target);
	strbuf_release(&err);
}

ایستا تهی parse_cmd_delete(ساختار ref_transaction *transaction,
			     ثابت نویسه *next, ثابت نویسه *end,
			     ساختار command_options *opts UNUSED)
{
	ساختار strbuf err = STRBUF_INIT;
	نویسه *refname;
	ساختار object_id old_oid;
	صحیح have_old;

	refname = parse_refname(&next);
	اگر (!refname)
		die("delete: missing <ref>");

	اگر (parse_next_oid(&next, end, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} وگرنه {
		اگر (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	اگر (*next != سطر_termination)
		die("delete %s: extra input: %s", refname, next);

	اگر (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : پوچ,
				   پوچ, update_flags, پیام, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	آزاد(refname);
	strbuf_release(&err);
}

ایستا تهی parse_cmd_symref_delete(ساختار ref_transaction *transaction,
				    ثابت نویسه *next, ثابت نویسه *end UNUSED,
				    ساختار command_options *opts UNUSED)
{
	ساختار strbuf err = STRBUF_INIT;
	نویسه *refname, *old_target;

	اگر (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&next);
	اگر (!refname)
		die("symref-delete: missing <ref>");

	old_target = parse_next_refname(&next);

	اگر (*next != سطر_termination)
		die("symref-delete %s: extra input: %s", refname, next);

	اگر (ref_transaction_delete(transaction, refname, پوچ,
				   old_target, update_flags, پیام, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	آزاد(refname);
	آزاد(old_target);
	strbuf_release(&err);
}

ایستا تهی parse_cmd_verify(ساختار ref_transaction *transaction,
			     ثابت نویسه *next, ثابت نویسه *end,
			     ساختار command_options *opts UNUSED)
{
	ساختار strbuf err = STRBUF_INIT;
	نویسه *refname;
	ساختار object_id old_oid;

	refname = parse_refname(&next);
	اگر (!refname)
		die("verify: missing <ref>");

	اگر (parse_next_oid(&next, end, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	اگر (*next != سطر_termination)
		die("verify %s: extra input: %s", refname, next);

	اگر (ref_transaction_verify(transaction, refname, &old_oid,
				   پوچ, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	آزاد(refname);
	strbuf_release(&err);
}

ایستا تهی parse_cmd_symref_verify(ساختار ref_transaction *transaction,
				    ثابت نویسه *next, ثابت نویسه *end UNUSED,
				    ساختار command_options *opts UNUSED)
{
	ساختار strbuf err = STRBUF_INIT;
	ساختار object_id old_oid;
	نویسه *refname, *old_target;

	اگر (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&next);
	اگر (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_target = parse_next_refname(&next);
	اگر (!old_target)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	اگر (*next != سطر_termination)
		die("symref-verify %s: extra input: %s", refname, next);

	اگر (ref_transaction_verify(transaction, refname,
				   old_target ? پوچ : &old_oid,
				   old_target, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	آزاد(refname);
	آزاد(old_target);
	strbuf_release(&err);
}

ایستا تهی report_ok(ثابت نویسه *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

ایستا تهی parse_cmd_option(ساختار ref_transaction *transaction UNUSED,
			     ثابت نویسه *next, ثابت نویسه *end UNUSED,
			     ساختار command_options *opts UNUSED)
{
	ثابت نویسه *rest;
	اگر (skip_prefix(next, "no-deref", &rest) && *rest == سطر_termination)
		update_flags |= REF_NO_DEREF;
	وگرنه
		die("option unknown: %s", next);
}

ایستا تهی parse_cmd_start(ساختار ref_transaction *transaction UNUSED,
			    ثابت نویسه *next, ثابت نویسه *end UNUSED,
			    ساختار command_options *opts UNUSED)
{
	اگر (*next != سطر_termination)
		die("start: extra input: %s", next);
	report_ok("start");
}

ایستا تهی parse_cmd_prepare(ساختار ref_transaction *transaction,
			      ثابت نویسه *next, ثابت نویسه *end UNUSED,
			      ساختار command_options *opts UNUSED)
{
	ساختار strbuf error = STRBUF_INIT;
	اگر (*next != سطر_termination)
		die("prepare: extra input: %s", next);
	اگر (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

ایستا تهی parse_cmd_abort(ساختار ref_transaction *transaction,
			    ثابت نویسه *next, ثابت نویسه *end UNUSED,
			    ساختار command_options *opts UNUSED)
{
	ساختار strbuf error = STRBUF_INIT;
	اگر (*next != سطر_termination)
		die("abort: extra input: %s", next);
	اگر (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

ایستا تهی parse_cmd_commit(ساختار ref_transaction *transaction,
			     ثابت نویسه *next, ثابت نویسه *end UNUSED,
			     ساختار command_options *opts UNUSED)
{
	ساختار strbuf error = STRBUF_INIT;
	اگر (*next != سطر_termination)
		die("commit: extra input: %s", next);
	اگر (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, پوچ);

	report_ok("commit");
	ref_transaction_free(transaction);
}

شمارش update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

ایستا ثابت ساختار parse_cmd {
	ثابت نویسه *prefix;
	تهی (*fn)(ساختار ref_transaction *, ثابت نویسه *, ثابت نویسه *,
		   ساختار command_options *);
	unsigned args;
	شمارش update_refs_state state;
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

ایستا تهی update_refs_stdin(unsigned صحیح flags)
{
	ساختار strbuf درون_داد = STRBUF_INIT, err = STRBUF_INIT;
	شمارش update_refs_state state = UPDATE_REFS_OPEN;
	ساختار ref_transaction *transaction;
	صحیح i, j;

	ساختار command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	اگر (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	وقتی (!strbuf_getwholeline(&درون_داد, stdin, سطر_termination)) {
		ثابت ساختار parse_cmd *cmd = پوچ;

		اگر (*درون_داد.buf == سطر_termination)
			die("empty command in input");
		وگرنه اگر (isspace(*درون_داد.buf))
			die("whitespace before command: %s", درون_داد.buf);

		برای (i = 0; i < ARRAY_SIZE(command); i++) {
			ثابت نویسه *prefix = command[i].prefix;
			نویسه c;

			اگر (!starts_with(درون_داد.buf, prefix))
				ادامه;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].args ? ' ' : سطر_termination;
			اگر (درون_داد.buf[strlen(prefix)] != c)
				ادامه;

			cmd = &command[i];
			بشکن;
		}
		اگر (!cmd)
			die("unknown command: %s", درون_داد.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		برای (j = 1; سطر_termination == '\0' && j < cmd->args; j++)
			اگر (strbuf_appendwholeline(&درون_داد, stdin, سطر_termination))
				بشکن;

		انتخاب (state) {
		حالت UPDATE_REFS_OPEN:
		حالت UPDATE_REFS_STARTED:
			اگر (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			اگر (cmd->state >= state)
				state = cmd->state;
			بشکن;
		حالت UPDATE_REFS_PREPARED:
			اگر (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			بشکن;
		حالت UPDATE_REFS_CLOSED:
			اگر (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			اگر (!transaction)
				die("%s", err.buf);

			بشکن;
		}

		cmd->fn(transaction, درون_داد.buf + strlen(cmd->prefix) + !!cmd->args,
			درون_داد.buf + درون_داد.len, &opts);
	}

	انتخاب (state) {
	حالت UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		اگر (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, پوچ);
		ref_transaction_free(transaction);
		بشکن;
	حالت UPDATE_REFS_STARTED:
	حالت UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		اگر (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		بشکن;
	حالت UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		بشکن;
	}

	strbuf_release(&err);
	strbuf_release(&درون_داد);
}

صحیح cmd_update_ref(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix,
		   ساختار repository *repo UNUSED)
{
	ثابت نویسه *refname, *oldval;
	ساختار object_id oid, oldoid;
	صحیح delete = 0, no_deref = 0, read_stdin = 0, end_null = 0;
	صحیح create_reflog = 0;
	unsigned صحیح flags = 0;

	ساختار option options[] = {
		OPT_STRING( 'm', پوچ, &پیام, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', پوچ, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', پوچ, &end_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_BIT('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, پوچ);
	argc = parse_options(argc, argv, prefix, options, git_update_ref_usage,
			     0);
	اگر (پیام && !*پیام)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	اگر (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	اگر (read_stdin) {
		اگر (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		اگر (end_null)
			سطر_termination = '\0';
		update_refs_stdin(flags);
		بازگشت 0;
	} وگرنه اگر (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	اگر (end_null)
		usage_with_options(git_update_ref_usage, options);

	اگر (delete) {
		اگر (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} وگرنه {
		ثابت نویسه *مقدار;
		اگر (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		مقدار = argv[1];
		oldval = argv[2];
		اگر (repo_get_oid_with_flags(the_repository, مقدار, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", مقدار);
	}

	اگر (oldval) {
		اگر (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		وگرنه اگر (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	اگر (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		بازگشت refs_delete_ref(get_main_ref_store(the_repository),
				       پیام, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : پوچ,
				       default_flags);
	وگرنه
		بازگشت refs_update_ref(get_main_ref_store(the_repository),
				       پیام, refname, &oid,
				       oldval ? &oldoid : پوچ,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
