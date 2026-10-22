#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "hex.h"
#ضمن "refs.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "quote.h"

ساكن ثابت حرف * ثابت git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	عدم
};

ساكن حرف سٽ_termination = '\n';
ساكن بنانشان صحيح update_flags;
ساكن بنانشان صحيح default_flags;
ساكن بنانشان create_reflog_flag;
ساكن ثابت حرف *نياپو;

بنية command_options {
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
ساكن ثابت حرف *parse_arg(ثابت حرف *ايندڙ, بنية strbuf *arg)
{
	إذا (*ايندڙ == '"') {
		ثابت حرف *orig = ايندڙ;

		إذا (unquote_c_style(arg, ايندڙ, &ايندڙ))
			die("badly quoted argument: %s", orig);
		إذا (*ايندڙ && !خال_آهي(*ايندڙ))
			die("unexpected character after quoted argument: %s", orig);
	} وإلا {
		طالما (*ايندڙ && !خال_آهي(*ايندڙ))
			strbuf_addch(arg, *ايندڙ++);
	}

	أرجع ايندڙ;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
ساكن حرف *parse_refname(ثابت حرف **ايندڙ)
{
	بنية strbuf ref = STRBUF_INIT;

	إذا (سٽ_termination) {
		/* Without -z, use the next argument */
		*ايندڙ = parse_arg(*ايندڙ, &ref);
	} وإلا {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *ايندڙ);
		*ايندڙ += ref.len;
	}

	إذا (!ref.len) {
		strbuf_release(&ref);
		أرجع عدم;
	}

	إذا (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	أرجع strbuf_detach(&ref, عدم);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
ساكن حرف *parse_ايندڙ_refname(ثابت حرف **ايندڙ)
{
	إذا (سٽ_termination) {
		/* Without -z, consume SP and use next argument */
		إذا (!**ايندڙ || **ايندڙ == سٽ_termination)
			أرجع عدم;
		إذا (**ايندڙ != ' ')
			die("expected SP but got: %s", *ايندڙ);
	} وإلا {
		/* With -z, read the next NUL-terminated line */
		إذا (**ايندڙ)
			أرجع عدم;
	}
	/* Skip the delimiter */
	(*ايندڙ)++;

	أرجع parse_refname(ايندڙ);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
ساكن حرف *parse_ايندڙ_arg(ثابت حرف **ايندڙ)
{
	بنية strbuf arg = STRBUF_INIT;

	إذا (سٽ_termination) {
		/* Without -z, consume SP and use next argument */
		إذا (!**ايندڙ || **ايندڙ == سٽ_termination)
			أرجع عدم;
		إذا (**ايندڙ != ' ')
			die("expected SP but got: %s", *ايندڙ);
	} وإلا {
		/* With -z, read the next NUL-terminated line */
		إذا (**ايندڙ)
			أرجع عدم;
	}
	/* Skip the delimiter */
	(*ايندڙ)++;

	إذا (سٽ_termination) {
		/* Without -z, use the next argument */
		*ايندڙ = parse_arg(*ايندڙ, &arg);
	} وإلا {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *ايندڙ);
		*ايندڙ += arg.len;
	}

	إذا (arg.len)
		أرجع strbuf_detach(&arg, عدم);

	strbuf_release(&arg);
	أرجع عدم;
}

/*
 * The value being parsed is <old-oid> (as opposed to <new-oid>; the
 * difference affects which error messages are generated):
 */
#وصف_ڪريو PARSE_SHA1_OLD 0x01

/*
 * For backwards compatibility, accept an empty string for update's
 * <new-oid> in binary mode to be equivalent to specifying zeros.
 */
#وصف_ڪريو PARSE_SHA1_ALLOW_خالي_جايون 0x02

/*
 * Parse an argument separator followed by the next argument, if any.
 * If there is an argument, convert it to a SHA-1, write it to sha1,
 * set *next to point at the character terminating the argument, and
 * return 0.  If there is no argument at all (not even the empty
 * string), return 1 and leave *next unchanged.  If the value is
 * provided but cannot be converted to a SHA-1, die.  flags can
 * include PARSE_SHA1_OLD and/or PARSE_SHA1_ALLOW_EMPTY.
 */
ساكن صحيح parse_ايندڙ_oid(ثابت حرف **ايندڙ, ثابت حرف *پڇاڙي,
			  بنية object_id *oid,
			  ثابت حرف *command, ثابت حرف *refname,
			  صحيح flags)
{
	بنية strbuf arg = STRBUF_INIT;
	صحيح ret = 0;

	إذا (*ايندڙ == پڇاڙي)
		اذهب eof;

	إذا (سٽ_termination) {
		/* Without -z, consume SP and use next argument */
		إذا (!**ايندڙ || **ايندڙ == سٽ_termination)
			أرجع 1;
		إذا (**ايندڙ != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *ايندڙ);
		(*ايندڙ)++;
		*ايندڙ = parse_arg(*ايندڙ, &arg);
		إذا (arg.len) {
			إذا (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				اذهب invalid;
		} وإلا {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} وإلا {
		/* With -z, read the next NUL-terminated line */
		إذا (**ايندڙ)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *ايندڙ);
		(*ايندڙ)++;
		إذا (*ايندڙ == پڇاڙي)
			اذهب eof;
		strbuf_addstr(&arg, *ايندڙ);
		*ايندڙ += arg.len;

		إذا (arg.len) {
			إذا (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				اذهب invalid;
		} وإلا إذا (flags & PARSE_SHA1_ALLOW_خالي_جايون) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} وإلا {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	أرجع ret;

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

ساكن فراغ print_rejected_refs(ثابت حرف *refname,
				ثابت بنية object_id *old_oid,
				ثابت بنية object_id *new_oid,
				ثابت حرف *old_هدف,
				ثابت حرف *new_هدف,
				تعداد ref_transaction_error err,
				ثابت حرف *details,
				فراغ *cb_معلومات اڻ_استعمال_ٿيل)
{
	بنية strbuf sb = STRBUF_INIT;

	إذا (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_منزل_hex(new_oid) : new_هدف,
		    old_oid ? oid_منزل_hex(old_oid) : old_هدف,
		    ref_transaction_error_نياپو(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
ساكن فراغ handle_ref_transaction_error(ثابت حرف *refname,
					 بنية object_id *new_oid,
					 بنية object_id *old_oid,
					 ثابت حرف *new_هدف,
					 ثابت حرف *old_هدف,
					 تعداد ref_transaction_error tx_err,
					 بنية strbuf *err,
					 بنية command_options *opts)
{
	إذا (!tx_err)
		أرجع;

	إذا (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_هدف,
				    new_هدف, tx_err, err->buf, عدم);
		أرجع;
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

ساكن فراغ parse_cmd_update(بنية ref_transaction *transaction,
			     ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي,
			     بنية command_options *opts)
{
	بنية strbuf err = STRBUF_INIT;
	حرف *refname;
	بنية object_id new_oid, old_oid;
	تعداد ref_transaction_error tx_err;
	صحيح have_old;

	refname = parse_refname(&ايندڙ);
	إذا (!refname)
		die("update: missing <ref>");

	إذا (parse_ايندڙ_oid(&ايندڙ, پڇاڙي, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_خالي_جايون))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_ايندڙ_oid(&ايندڙ, پڇاڙي, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	إذا (*ايندڙ != سٽ_termination)
		die("update %s: extra input: %s", refname, ايندڙ);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : عدم,
					عدم, عدم,
					update_flags | create_reflog_flag,
					نياپو, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : عدم,
				     عدم, عدم, tx_err, &err, opts);


	update_flags = default_flags;
	حرر(refname);
	strbuf_release(&err);
}

ساكن فراغ parse_cmd_symref_update(بنية ref_transaction *transaction,
				    ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي اڻ_استعمال_ٿيل,
				    بنية command_options *opts)
{
	حرف *refname, *new_هدف, *old_arg;
	تعداد ref_transaction_error tx_err;
	حرف *old_هدف = عدم;
	بنية strbuf err = STRBUF_INIT;
	بنية object_id old_oid;
	صحيح have_old_oid = 0;

	refname = parse_refname(&ايندڙ);
	إذا (!refname)
		die("symref-update: missing <ref>");

	new_هدف = parse_ايندڙ_refname(&ايندڙ);
	إذا (!new_هدف)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_ايندڙ_arg(&ايندڙ);
	إذا (old_arg) {
		old_هدف = parse_ايندڙ_arg(&ايندڙ);
		إذا (!old_هدف)
			die("symref-update %s: expected old value", refname);

		إذا (!strcmp(old_arg, "oid")) {
			إذا (repo_get_oid_with_flags(the_repository, old_هدف, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_هدف);

			have_old_oid = 1;
		} وإلا إذا (!strcmp(old_arg, "ref")) {
			إذا (check_refname_format(old_هدف, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_هدف);
		} وإلا {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	إذا (*ايندڙ != سٽ_termination)
		die("symref-update %s: extra input: %s", refname, ايندڙ);

	tx_err = ref_transaction_update(transaction, refname, عدم,
					have_old_oid ? &old_oid : عدم,
					new_هدف,
					have_old_oid ? عدم : old_هدف,
					update_flags | create_reflog_flag,
					نياپو, &err);
	handle_ref_transaction_error(refname, عدم, have_old_oid ? &old_oid : عدم,
				     new_هدف, have_old_oid ? عدم : old_هدف,
				     tx_err, &err, opts);

	update_flags = default_flags;
	حرر(refname);
	حرر(old_arg);
	حرر(old_هدف);
	حرر(new_هدف);
	strbuf_release(&err);
}

ساكن فراغ parse_cmd_create(بنية ref_transaction *transaction,
			     ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي,
			     بنية command_options *opts)
{
	بنية strbuf err = STRBUF_INIT;
	حرف *refname;
	بنية object_id new_oid;
	تعداد ref_transaction_error tx_err;

	refname = parse_refname(&ايندڙ);
	إذا (!refname)
		die("create: missing <ref>");

	إذا (parse_ايندڙ_oid(&ايندڙ, پڇاڙي, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	إذا (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	إذا (*ايندڙ != سٽ_termination)
		die("create %s: extra input: %s", refname, ايندڙ);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, عدم,
					update_flags | create_reflog_flag,
					نياپو, &err);
	handle_ref_transaction_error(refname, &new_oid, عدم, عدم, عدم, tx_err,
				     &err, opts);

	update_flags = default_flags;
	حرر(refname);
	strbuf_release(&err);
}

ساكن فراغ parse_cmd_symref_create(بنية ref_transaction *transaction,
				    ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي اڻ_استعمال_ٿيل,
				    بنية command_options *opts)
{
	بنية strbuf err = STRBUF_INIT;
	حرف *refname, *new_هدف;
	تعداد ref_transaction_error tx_err;

	refname = parse_refname(&ايندڙ);
	إذا (!refname)
		die("symref-create: missing <ref>");

	new_هدف = parse_ايندڙ_refname(&ايندڙ);
	إذا (!new_هدف)
		die("symref-create %s: missing <new-target>", refname);

	إذا (*ايندڙ != سٽ_termination)
		die("symref-create %s: extra input: %s", refname, ايندڙ);

	tx_err = ref_transaction_create(transaction, refname, عدم, new_هدف,
					update_flags | create_reflog_flag,
					نياپو, &err);
	handle_ref_transaction_error(refname, عدم, عدم, new_هدف, عدم,
				     tx_err, &err, opts);

	update_flags = default_flags;
	حرر(refname);
	حرر(new_هدف);
	strbuf_release(&err);
}

ساكن فراغ parse_cmd_delete(بنية ref_transaction *transaction,
			     ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي,
			     بنية command_options *opts اڻ_استعمال_ٿيل)
{
	بنية strbuf err = STRBUF_INIT;
	حرف *refname;
	بنية object_id old_oid;
	صحيح have_old;

	refname = parse_refname(&ايندڙ);
	إذا (!refname)
		die("delete: missing <ref>");

	إذا (parse_ايندڙ_oid(&ايندڙ, پڇاڙي, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} وإلا {
		إذا (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	إذا (*ايندڙ != سٽ_termination)
		die("delete %s: extra input: %s", refname, ايندڙ);

	إذا (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : عدم,
				   عدم, update_flags, نياپو, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	حرر(refname);
	strbuf_release(&err);
}

ساكن فراغ parse_cmd_symref_delete(بنية ref_transaction *transaction,
				    ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي اڻ_استعمال_ٿيل,
				    بنية command_options *opts اڻ_استعمال_ٿيل)
{
	بنية strbuf err = STRBUF_INIT;
	حرف *refname, *old_هدف;

	إذا (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&ايندڙ);
	إذا (!refname)
		die("symref-delete: missing <ref>");

	old_هدف = parse_ايندڙ_refname(&ايندڙ);

	إذا (*ايندڙ != سٽ_termination)
		die("symref-delete %s: extra input: %s", refname, ايندڙ);

	إذا (ref_transaction_delete(transaction, refname, عدم,
				   old_هدف, update_flags, نياپو, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	حرر(refname);
	حرر(old_هدف);
	strbuf_release(&err);
}

ساكن فراغ parse_cmd_verify(بنية ref_transaction *transaction,
			     ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي,
			     بنية command_options *opts اڻ_استعمال_ٿيل)
{
	بنية strbuf err = STRBUF_INIT;
	حرف *refname;
	بنية object_id old_oid;

	refname = parse_refname(&ايندڙ);
	إذا (!refname)
		die("verify: missing <ref>");

	إذا (parse_ايندڙ_oid(&ايندڙ, پڇاڙي, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	إذا (*ايندڙ != سٽ_termination)
		die("verify %s: extra input: %s", refname, ايندڙ);

	إذا (ref_transaction_verify(transaction, refname, &old_oid,
				   عدم, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	حرر(refname);
	strbuf_release(&err);
}

ساكن فراغ parse_cmd_symref_verify(بنية ref_transaction *transaction,
				    ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي اڻ_استعمال_ٿيل,
				    بنية command_options *opts اڻ_استعمال_ٿيل)
{
	بنية strbuf err = STRBUF_INIT;
	بنية object_id old_oid;
	حرف *refname, *old_هدف;

	إذا (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&ايندڙ);
	إذا (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_هدف = parse_ايندڙ_refname(&ايندڙ);
	إذا (!old_هدف)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	إذا (*ايندڙ != سٽ_termination)
		die("symref-verify %s: extra input: %s", refname, ايندڙ);

	إذا (ref_transaction_verify(transaction, refname,
				   old_هدف ? عدم : &old_oid,
				   old_هدف, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	حرر(refname);
	حرر(old_هدف);
	strbuf_release(&err);
}

ساكن فراغ report_ok(ثابت حرف *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

ساكن فراغ parse_cmd_option(بنية ref_transaction *transaction اڻ_استعمال_ٿيل,
			     ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي اڻ_استعمال_ٿيل,
			     بنية command_options *opts اڻ_استعمال_ٿيل)
{
	ثابت حرف *rest;
	إذا (skip_اڳياڙي(ايندڙ, "no-deref", &rest) && *rest == سٽ_termination)
		update_flags |= REF_NO_DEREF;
	وإلا
		die("option unknown: %s", ايندڙ);
}

ساكن فراغ parse_cmd_شروعات(بنية ref_transaction *transaction اڻ_استعمال_ٿيل,
			    ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي اڻ_استعمال_ٿيل,
			    بنية command_options *opts اڻ_استعمال_ٿيل)
{
	إذا (*ايندڙ != سٽ_termination)
		die("start: extra input: %s", ايندڙ);
	report_ok("start");
}

ساكن فراغ parse_cmd_prepare(بنية ref_transaction *transaction,
			      ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي اڻ_استعمال_ٿيل,
			      بنية command_options *opts اڻ_استعمال_ٿيل)
{
	بنية strbuf error = STRBUF_INIT;
	إذا (*ايندڙ != سٽ_termination)
		die("prepare: extra input: %s", ايندڙ);
	إذا (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

ساكن فراغ parse_cmd_abort(بنية ref_transaction *transaction,
			    ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي اڻ_استعمال_ٿيل,
			    بنية command_options *opts اڻ_استعمال_ٿيل)
{
	بنية strbuf error = STRBUF_INIT;
	إذا (*ايندڙ != سٽ_termination)
		die("abort: extra input: %s", ايندڙ);
	إذا (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

ساكن فراغ parse_cmd_commit(بنية ref_transaction *transaction,
			     ثابت حرف *ايندڙ, ثابت حرف *پڇاڙي اڻ_استعمال_ٿيل,
			     بنية command_options *opts اڻ_استعمال_ٿيل)
{
	بنية strbuf error = STRBUF_INIT;
	إذا (*ايندڙ != سٽ_termination)
		die("commit: extra input: %s", ايندڙ);
	إذا (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, عدم);

	report_ok("commit");
	ref_transaction_free(transaction);
}

تعداد update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

ساكن ثابت بنية parse_cmd {
	ثابت حرف *اڳياڙي;
	فراغ (*fn)(بنية ref_transaction *, ثابت حرف *, ثابت حرف *,
		   بنية command_options *);
	بنانشان دليلون;
	تعداد update_refs_state state;
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
	{ "start",         parse_cmd_شروعات,         0, UPDATE_REFS_STARTED },
	{ "prepare",       parse_cmd_prepare,       0, UPDATE_REFS_PREPARED },
	{ "abort",         parse_cmd_abort,         0, UPDATE_REFS_CLOSED },
	{ "commit",        parse_cmd_commit,        0, UPDATE_REFS_CLOSED },
};

ساكن فراغ update_refs_stdin(بنانشان صحيح flags)
{
	بنية strbuf داخلا_2 = STRBUF_INIT, err = STRBUF_INIT;
	تعداد update_refs_state state = UPDATE_REFS_OPEN;
	بنية ref_transaction *transaction;
	صحيح i, j;

	بنية command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	إذا (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	طالما (!strbuf_getwholeline(&داخلا_2, stdin, سٽ_termination)) {
		ثابت بنية parse_cmd *cmd = عدم;

		إذا (*داخلا_2.buf == سٽ_termination)
			die("empty command in input");
		وإلا إذا (خال_آهي(*داخلا_2.buf))
			die("whitespace before command: %s", داخلا_2.buf);

		لكل (i = 0; i < ARRAY_سائيز(command); i++) {
			ثابت حرف *اڳياڙي = command[i].اڳياڙي;
			حرف c;

			إذا (!starts_with(داخلا_2.buf, اڳياڙي))
				استمر;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].دليلون ? ' ' : سٽ_termination;
			إذا (داخلا_2.buf[لڙي_ڊيگهه(اڳياڙي)] != c)
				استمر;

			cmd = &command[i];
			اكسر;
		}
		إذا (!cmd)
			die("unknown command: %s", داخلا_2.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		لكل (j = 1; سٽ_termination == '\0' && j < cmd->دليلون; j++)
			إذا (strbuf_appendwholeline(&داخلا_2, stdin, سٽ_termination))
				اكسر;

		بدل (state) {
		حالة UPDATE_REFS_OPEN:
		حالة UPDATE_REFS_STARTED:
			إذا (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			إذا (cmd->state >= state)
				state = cmd->state;
			اكسر;
		حالة UPDATE_REFS_PREPARED:
			إذا (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			اكسر;
		حالة UPDATE_REFS_CLOSED:
			إذا (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			إذا (!transaction)
				die("%s", err.buf);

			اكسر;
		}

		cmd->fn(transaction, داخلا_2.buf + لڙي_ڊيگهه(cmd->اڳياڙي) + !!cmd->دليلون,
			داخلا_2.buf + داخلا_2.len, &opts);
	}

	بدل (state) {
	حالة UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		إذا (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, عدم);
		ref_transaction_free(transaction);
		اكسر;
	حالة UPDATE_REFS_STARTED:
	حالة UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		إذا (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		اكسر;
	حالة UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		اكسر;
	}

	strbuf_release(&err);
	strbuf_release(&داخلا_2);
}

صحيح cmd_update_ref(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *اڳياڙي,
		   بنية repository *repo اڻ_استعمال_ٿيل)
{
	ثابت حرف *refname, *oldval;
	بنية object_id oid, oldoid;
	صحيح delete = 0, no_deref = 0, read_stdin = 0, پڇاڙي_null = 0;
	صحيح create_reflog = 0;
	بنانشان صحيح flags = 0;

	بنية option options[] = {
		OPT_STRING( 'm', عدم, &نياپو, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', عدم, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', عدم, &پڇاڙي_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_ٻٽو_انگ('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_پڇاڙي(),
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, اڳياڙي, options, git_update_ref_usage,
			     0);
	إذا (نياپو && !*نياپو)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	إذا (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	إذا (read_stdin) {
		إذا (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		إذا (پڇاڙي_null)
			سٽ_termination = '\0';
		update_refs_stdin(flags);
		أرجع 0;
	} وإلا إذا (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	إذا (پڇاڙي_null)
		usage_with_options(git_update_ref_usage, options);

	إذا (delete) {
		إذا (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} وإلا {
		ثابت حرف *قدر;
		إذا (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		قدر = argv[1];
		oldval = argv[2];
		إذا (repo_get_oid_with_flags(the_repository, قدر, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", قدر);
	}

	إذا (oldval) {
		إذا (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		وإلا إذا (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	إذا (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		أرجع refs_delete_ref(get_main_ref_store(the_repository),
				       نياپو, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : عدم,
				       default_flags);
	وإلا
		أرجع refs_update_ref(get_main_ref_store(the_repository),
				       نياپو, refname, &oid,
				       oldval ? &oldoid : عدم,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
