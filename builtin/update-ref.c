#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساکن ثابت حرف * ثابت git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	تہی
};

ساکن حرف سطر_termination = '\n';
ساکن غیرعلامتی صحیح update_flags;
ساکن غیرعلامتی صحیح default_flags;
ساکن غیرعلامتی create_reflog_flag;
ساکن ثابت حرف *پیغام;

ساخت command_options {
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
ساکن ثابت حرف *parse_arg(ثابت حرف *اگلا, ساخت strbuf *arg)
{
	اگر (*اگلا == '"') {
		ثابت حرف *orig = اگلا;

		اگر (unquote_c_style(arg, اگلا, &اگلا))
			die("badly quoted argument: %s", orig);
		اگر (*اگلا && !خلا_ہے(*اگلا))
			die("unexpected character after quoted argument: %s", orig);
	} ورنہ {
		جب (*اگلا && !خلا_ہے(*اگلا))
			strbuf_addch(arg, *اگلا++);
	}

	واپس اگلا;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
ساکن حرف *parse_refname(ثابت حرف **اگلا)
{
	ساخت strbuf ref = STRBUF_INIT;

	اگر (سطر_termination) {
		/* Without -z, use the next argument */
		*اگلا = parse_arg(*اگلا, &ref);
	} ورنہ {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *اگلا);
		*اگلا += ref.len;
	}

	اگر (!ref.len) {
		strbuf_release(&ref);
		واپس تہی;
	}

	اگر (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	واپس strbuf_detach(&ref, تہی);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
ساکن حرف *parse_اگلا_refname(ثابت حرف **اگلا)
{
	اگر (سطر_termination) {
		/* Without -z, consume SP and use next argument */
		اگر (!**اگلا || **اگلا == سطر_termination)
			واپس تہی;
		اگر (**اگلا != ' ')
			die("expected SP but got: %s", *اگلا);
	} ورنہ {
		/* With -z, read the next NUL-terminated line */
		اگر (**اگلا)
			واپس تہی;
	}
	/* Skip the delimiter */
	(*اگلا)++;

	واپس parse_refname(اگلا);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
ساکن حرف *parse_اگلا_arg(ثابت حرف **اگلا)
{
	ساخت strbuf arg = STRBUF_INIT;

	اگر (سطر_termination) {
		/* Without -z, consume SP and use next argument */
		اگر (!**اگلا || **اگلا == سطر_termination)
			واپس تہی;
		اگر (**اگلا != ' ')
			die("expected SP but got: %s", *اگلا);
	} ورنہ {
		/* With -z, read the next NUL-terminated line */
		اگر (**اگلا)
			واپس تہی;
	}
	/* Skip the delimiter */
	(*اگلا)++;

	اگر (سطر_termination) {
		/* Without -z, use the next argument */
		*اگلا = parse_arg(*اگلا, &arg);
	} ورنہ {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *اگلا);
		*اگلا += arg.len;
	}

	اگر (arg.len)
		واپس strbuf_detach(&arg, تہی);

	strbuf_release(&arg);
	واپس تہی;
}

/*
 * The value being parsed is <old-oid> (as opposed to <new-oid>; the
 * difference affects which error messages are generated):
 */
#تعریف_کرو PARSE_SHA1_OLD 0x01

/*
 * For backwards compatibility, accept an empty string for update's
 * <new-oid> in binary mode to be equivalent to specifying zeros.
 */
#تعریف_کرو PARSE_SHA1_ALLOW_خالی_خانے 0x02

/*
 * Parse an argument separator followed by the next argument, if any.
 * If there is an argument, convert it to a SHA-1, write it to sha1,
 * set *next to point at the character terminating the argument, and
 * return 0.  If there is no argument at all (not even the empty
 * string), return 1 and leave *next unchanged.  If the value is
 * provided but cannot be converted to a SHA-1, die.  flags can
 * include PARSE_SHA1_OLD and/or PARSE_SHA1_ALLOW_EMPTY.
 */
ساکن صحیح parse_اگلا_oid(ثابت حرف **اگلا, ثابت حرف *انتہا,
			  ساخت object_id *oid,
			  ثابت حرف *command, ثابت حرف *refname,
			  صحیح flags)
{
	ساخت strbuf arg = STRBUF_INIT;
	صحیح ret = 0;

	اگر (*اگلا == انتہا)
		جاؤ eof;

	اگر (سطر_termination) {
		/* Without -z, consume SP and use next argument */
		اگر (!**اگلا || **اگلا == سطر_termination)
			واپس 1;
		اگر (**اگلا != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *اگلا);
		(*اگلا)++;
		*اگلا = parse_arg(*اگلا, &arg);
		اگر (arg.len) {
			اگر (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				جاؤ invalid;
		} ورنہ {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} ورنہ {
		/* With -z, read the next NUL-terminated line */
		اگر (**اگلا)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *اگلا);
		(*اگلا)++;
		اگر (*اگلا == انتہا)
			جاؤ eof;
		strbuf_addstr(&arg, *اگلا);
		*اگلا += arg.len;

		اگر (arg.len) {
			اگر (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				جاؤ invalid;
		} ورنہ اگر (flags & PARSE_SHA1_ALLOW_خالی_خانے) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} ورنہ {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	واپس ret;

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

ساکن خالی print_rejected_refs(ثابت حرف *refname,
				ثابت ساخت object_id *old_oid,
				ثابت ساخت object_id *new_oid,
				ثابت حرف *old_ہدف,
				ثابت حرف *new_ہدف,
				شمار ref_transaction_error err,
				ثابت حرف *details,
				خالی *cb_ڈیٹا غیر_مستعمل)
{
	ساخت strbuf sb = STRBUF_INIT;

	اگر (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_مقصد_hex(new_oid) : new_ہدف,
		    old_oid ? oid_مقصد_hex(old_oid) : old_ہدف,
		    ref_transaction_error_پیغام(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
ساکن خالی handle_ref_transaction_error(ثابت حرف *refname,
					 ساخت object_id *new_oid,
					 ساخت object_id *old_oid,
					 ثابت حرف *new_ہدف,
					 ثابت حرف *old_ہدف,
					 شمار ref_transaction_error tx_err,
					 ساخت strbuf *err,
					 ساخت command_options *opts)
{
	اگر (!tx_err)
		واپس;

	اگر (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_ہدف,
				    new_ہدف, tx_err, err->buf, تہی);
		واپس;
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

ساکن خالی parse_cmd_update(ساخت ref_transaction *transaction,
			     ثابت حرف *اگلا, ثابت حرف *انتہا,
			     ساخت command_options *opts)
{
	ساخت strbuf err = STRBUF_INIT;
	حرف *refname;
	ساخت object_id new_oid, old_oid;
	شمار ref_transaction_error tx_err;
	صحیح have_old;

	refname = parse_refname(&اگلا);
	اگر (!refname)
		die("update: missing <ref>");

	اگر (parse_اگلا_oid(&اگلا, انتہا, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_خالی_خانے))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_اگلا_oid(&اگلا, انتہا, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	اگر (*اگلا != سطر_termination)
		die("update %s: extra input: %s", refname, اگلا);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : تہی,
					تہی, تہی,
					update_flags | create_reflog_flag,
					پیغام, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : تہی,
				     تہی, تہی, tx_err, &err, opts);


	update_flags = default_flags;
	آزاد(refname);
	strbuf_release(&err);
}

ساکن خالی parse_cmd_symref_update(ساخت ref_transaction *transaction,
				    ثابت حرف *اگلا, ثابت حرف *انتہا غیر_مستعمل,
				    ساخت command_options *opts)
{
	حرف *refname, *new_ہدف, *old_arg;
	شمار ref_transaction_error tx_err;
	حرف *old_ہدف = تہی;
	ساخت strbuf err = STRBUF_INIT;
	ساخت object_id old_oid;
	صحیح have_old_oid = 0;

	refname = parse_refname(&اگلا);
	اگر (!refname)
		die("symref-update: missing <ref>");

	new_ہدف = parse_اگلا_refname(&اگلا);
	اگر (!new_ہدف)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_اگلا_arg(&اگلا);
	اگر (old_arg) {
		old_ہدف = parse_اگلا_arg(&اگلا);
		اگر (!old_ہدف)
			die("symref-update %s: expected old value", refname);

		اگر (!strcmp(old_arg, "oid")) {
			اگر (repo_get_oid_with_flags(the_repository, old_ہدف, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_ہدف);

			have_old_oid = 1;
		} ورنہ اگر (!strcmp(old_arg, "ref")) {
			اگر (check_refname_format(old_ہدف, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_ہدف);
		} ورنہ {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	اگر (*اگلا != سطر_termination)
		die("symref-update %s: extra input: %s", refname, اگلا);

	tx_err = ref_transaction_update(transaction, refname, تہی,
					have_old_oid ? &old_oid : تہی,
					new_ہدف,
					have_old_oid ? تہی : old_ہدف,
					update_flags | create_reflog_flag,
					پیغام, &err);
	handle_ref_transaction_error(refname, تہی, have_old_oid ? &old_oid : تہی,
				     new_ہدف, have_old_oid ? تہی : old_ہدف,
				     tx_err, &err, opts);

	update_flags = default_flags;
	آزاد(refname);
	آزاد(old_arg);
	آزاد(old_ہدف);
	آزاد(new_ہدف);
	strbuf_release(&err);
}

ساکن خالی parse_cmd_create(ساخت ref_transaction *transaction,
			     ثابت حرف *اگلا, ثابت حرف *انتہا,
			     ساخت command_options *opts)
{
	ساخت strbuf err = STRBUF_INIT;
	حرف *refname;
	ساخت object_id new_oid;
	شمار ref_transaction_error tx_err;

	refname = parse_refname(&اگلا);
	اگر (!refname)
		die("create: missing <ref>");

	اگر (parse_اگلا_oid(&اگلا, انتہا, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	اگر (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	اگر (*اگلا != سطر_termination)
		die("create %s: extra input: %s", refname, اگلا);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, تہی,
					update_flags | create_reflog_flag,
					پیغام, &err);
	handle_ref_transaction_error(refname, &new_oid, تہی, تہی, تہی, tx_err,
				     &err, opts);

	update_flags = default_flags;
	آزاد(refname);
	strbuf_release(&err);
}

ساکن خالی parse_cmd_symref_create(ساخت ref_transaction *transaction,
				    ثابت حرف *اگلا, ثابت حرف *انتہا غیر_مستعمل,
				    ساخت command_options *opts)
{
	ساخت strbuf err = STRBUF_INIT;
	حرف *refname, *new_ہدف;
	شمار ref_transaction_error tx_err;

	refname = parse_refname(&اگلا);
	اگر (!refname)
		die("symref-create: missing <ref>");

	new_ہدف = parse_اگلا_refname(&اگلا);
	اگر (!new_ہدف)
		die("symref-create %s: missing <new-target>", refname);

	اگر (*اگلا != سطر_termination)
		die("symref-create %s: extra input: %s", refname, اگلا);

	tx_err = ref_transaction_create(transaction, refname, تہی, new_ہدف,
					update_flags | create_reflog_flag,
					پیغام, &err);
	handle_ref_transaction_error(refname, تہی, تہی, new_ہدف, تہی,
				     tx_err, &err, opts);

	update_flags = default_flags;
	آزاد(refname);
	آزاد(new_ہدف);
	strbuf_release(&err);
}

ساکن خالی parse_cmd_delete(ساخت ref_transaction *transaction,
			     ثابت حرف *اگلا, ثابت حرف *انتہا,
			     ساخت command_options *opts غیر_مستعمل)
{
	ساخت strbuf err = STRBUF_INIT;
	حرف *refname;
	ساخت object_id old_oid;
	صحیح have_old;

	refname = parse_refname(&اگلا);
	اگر (!refname)
		die("delete: missing <ref>");

	اگر (parse_اگلا_oid(&اگلا, انتہا, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} ورنہ {
		اگر (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	اگر (*اگلا != سطر_termination)
		die("delete %s: extra input: %s", refname, اگلا);

	اگر (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : تہی,
				   تہی, update_flags, پیغام, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	آزاد(refname);
	strbuf_release(&err);
}

ساکن خالی parse_cmd_symref_delete(ساخت ref_transaction *transaction,
				    ثابت حرف *اگلا, ثابت حرف *انتہا غیر_مستعمل,
				    ساخت command_options *opts غیر_مستعمل)
{
	ساخت strbuf err = STRBUF_INIT;
	حرف *refname, *old_ہدف;

	اگر (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&اگلا);
	اگر (!refname)
		die("symref-delete: missing <ref>");

	old_ہدف = parse_اگلا_refname(&اگلا);

	اگر (*اگلا != سطر_termination)
		die("symref-delete %s: extra input: %s", refname, اگلا);

	اگر (ref_transaction_delete(transaction, refname, تہی,
				   old_ہدف, update_flags, پیغام, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	آزاد(refname);
	آزاد(old_ہدف);
	strbuf_release(&err);
}

ساکن خالی parse_cmd_verify(ساخت ref_transaction *transaction,
			     ثابت حرف *اگلا, ثابت حرف *انتہا,
			     ساخت command_options *opts غیر_مستعمل)
{
	ساخت strbuf err = STRBUF_INIT;
	حرف *refname;
	ساخت object_id old_oid;

	refname = parse_refname(&اگلا);
	اگر (!refname)
		die("verify: missing <ref>");

	اگر (parse_اگلا_oid(&اگلا, انتہا, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	اگر (*اگلا != سطر_termination)
		die("verify %s: extra input: %s", refname, اگلا);

	اگر (ref_transaction_verify(transaction, refname, &old_oid,
				   تہی, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	آزاد(refname);
	strbuf_release(&err);
}

ساکن خالی parse_cmd_symref_verify(ساخت ref_transaction *transaction,
				    ثابت حرف *اگلا, ثابت حرف *انتہا غیر_مستعمل,
				    ساخت command_options *opts غیر_مستعمل)
{
	ساخت strbuf err = STRBUF_INIT;
	ساخت object_id old_oid;
	حرف *refname, *old_ہدف;

	اگر (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&اگلا);
	اگر (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_ہدف = parse_اگلا_refname(&اگلا);
	اگر (!old_ہدف)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	اگر (*اگلا != سطر_termination)
		die("symref-verify %s: extra input: %s", refname, اگلا);

	اگر (ref_transaction_verify(transaction, refname,
				   old_ہدف ? تہی : &old_oid,
				   old_ہدف, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	آزاد(refname);
	آزاد(old_ہدف);
	strbuf_release(&err);
}

ساکن خالی report_ok(ثابت حرف *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

ساکن خالی parse_cmd_option(ساخت ref_transaction *transaction غیر_مستعمل,
			     ثابت حرف *اگلا, ثابت حرف *انتہا غیر_مستعمل,
			     ساخت command_options *opts غیر_مستعمل)
{
	ثابت حرف *rest;
	اگر (skip_سابقہ(اگلا, "no-deref", &rest) && *rest == سطر_termination)
		update_flags |= REF_NO_DEREF;
	ورنہ
		die("option unknown: %s", اگلا);
}

ساکن خالی parse_cmd_آغاز(ساخت ref_transaction *transaction غیر_مستعمل,
			    ثابت حرف *اگلا, ثابت حرف *انتہا غیر_مستعمل,
			    ساخت command_options *opts غیر_مستعمل)
{
	اگر (*اگلا != سطر_termination)
		die("start: extra input: %s", اگلا);
	report_ok("start");
}

ساکن خالی parse_cmd_prepare(ساخت ref_transaction *transaction,
			      ثابت حرف *اگلا, ثابت حرف *انتہا غیر_مستعمل,
			      ساخت command_options *opts غیر_مستعمل)
{
	ساخت strbuf error = STRBUF_INIT;
	اگر (*اگلا != سطر_termination)
		die("prepare: extra input: %s", اگلا);
	اگر (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

ساکن خالی parse_cmd_abort(ساخت ref_transaction *transaction,
			    ثابت حرف *اگلا, ثابت حرف *انتہا غیر_مستعمل,
			    ساخت command_options *opts غیر_مستعمل)
{
	ساخت strbuf error = STRBUF_INIT;
	اگر (*اگلا != سطر_termination)
		die("abort: extra input: %s", اگلا);
	اگر (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

ساکن خالی parse_cmd_commit(ساخت ref_transaction *transaction,
			     ثابت حرف *اگلا, ثابت حرف *انتہا غیر_مستعمل,
			     ساخت command_options *opts غیر_مستعمل)
{
	ساخت strbuf error = STRBUF_INIT;
	اگر (*اگلا != سطر_termination)
		die("commit: extra input: %s", اگلا);
	اگر (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, تہی);

	report_ok("commit");
	ref_transaction_free(transaction);
}

شمار update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

ساکن ثابت ساخت parse_cmd {
	ثابت حرف *سابقہ;
	خالی (*fn)(ساخت ref_transaction *, ثابت حرف *, ثابت حرف *,
		   ساخت command_options *);
	غیرعلامتی دلائل;
	شمار update_refs_state state;
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
	{ "start",         parse_cmd_آغاز,         0, UPDATE_REFS_STARTED },
	{ "prepare",       parse_cmd_prepare,       0, UPDATE_REFS_PREPARED },
	{ "abort",         parse_cmd_abort,         0, UPDATE_REFS_CLOSED },
	{ "commit",        parse_cmd_commit,        0, UPDATE_REFS_CLOSED },
};

ساکن خالی update_refs_stdin(غیرعلامتی صحیح flags)
{
	ساخت strbuf داخلہ = STRBUF_INIT, err = STRBUF_INIT;
	شمار update_refs_state state = UPDATE_REFS_OPEN;
	ساخت ref_transaction *transaction;
	صحیح i, j;

	ساخت command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	اگر (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	جب (!strbuf_getwholeline(&داخلہ, stdin, سطر_termination)) {
		ثابت ساخت parse_cmd *cmd = تہی;

		اگر (*داخلہ.buf == سطر_termination)
			die("empty command in input");
		ورنہ اگر (خلا_ہے(*داخلہ.buf))
			die("whitespace before command: %s", داخلہ.buf);

		برائے (i = 0; i < ARRAY_سائز(command); i++) {
			ثابت حرف *سابقہ = command[i].سابقہ;
			حرف c;

			اگر (!starts_with(داخلہ.buf, سابقہ))
				جاری;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].دلائل ? ' ' : سطر_termination;
			اگر (داخلہ.buf[لڑی_لمبائی(سابقہ)] != c)
				جاری;

			cmd = &command[i];
			توڑو;
		}
		اگر (!cmd)
			die("unknown command: %s", داخلہ.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		برائے (j = 1; سطر_termination == '\0' && j < cmd->دلائل; j++)
			اگر (strbuf_appendwholeline(&داخلہ, stdin, سطر_termination))
				توڑو;

		انتخاب (state) {
		حالت UPDATE_REFS_OPEN:
		حالت UPDATE_REFS_STARTED:
			اگر (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			اگر (cmd->state >= state)
				state = cmd->state;
			توڑو;
		حالت UPDATE_REFS_PREPARED:
			اگر (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			توڑو;
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

			توڑو;
		}

		cmd->fn(transaction, داخلہ.buf + لڑی_لمبائی(cmd->سابقہ) + !!cmd->دلائل,
			داخلہ.buf + داخلہ.len, &opts);
	}

	انتخاب (state) {
	حالت UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		اگر (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, تہی);
		ref_transaction_free(transaction);
		توڑو;
	حالت UPDATE_REFS_STARTED:
	حالت UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		اگر (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		توڑو;
	حالت UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		توڑو;
	}

	strbuf_release(&err);
	strbuf_release(&داخلہ);
}

صحیح cmd_update_ref(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *سابقہ,
		   ساخت repository *repo غیر_مستعمل)
{
	ثابت حرف *refname, *oldval;
	ساخت object_id oid, oldoid;
	صحیح delete = 0, no_deref = 0, read_stdin = 0, انتہا_null = 0;
	صحیح create_reflog = 0;
	غیرعلامتی صحیح flags = 0;

	ساخت option options[] = {
		OPT_STRING( 'm', تہی, &پیغام, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', تہی, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', تہی, &انتہا_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_ثنائی_ہندسہ('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_انتہا(),
	};

	repo_config(the_repository, git_default_config, تہی);
	argc = parse_options(argc, argv, سابقہ, options, git_update_ref_usage,
			     0);
	اگر (پیغام && !*پیغام)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	اگر (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	اگر (read_stdin) {
		اگر (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		اگر (انتہا_null)
			سطر_termination = '\0';
		update_refs_stdin(flags);
		واپس 0;
	} ورنہ اگر (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	اگر (انتہا_null)
		usage_with_options(git_update_ref_usage, options);

	اگر (delete) {
		اگر (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} ورنہ {
		ثابت حرف *قدر;
		اگر (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		قدر = argv[1];
		oldval = argv[2];
		اگر (repo_get_oid_with_flags(the_repository, قدر, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", قدر);
	}

	اگر (oldval) {
		اگر (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		ورنہ اگر (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	اگر (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		واپس refs_delete_ref(get_main_ref_store(the_repository),
				       پیغام, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : تہی,
				       default_flags);
	ورنہ
		واپس refs_update_ref(get_main_ref_store(the_repository),
				       پیغام, refname, &oid,
				       oldval ? &oldoid : تہی,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
