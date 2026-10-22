#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "quote.h"

સ્થિર અચલ અક્ષર * અચલ git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	ખાલી
};

સ્થિર અક્ષર લીટી_termination = '\n';
સ્થિર unsigned પૂર્ણાંક update_flags;
સ્થિર unsigned પૂર્ણાંક default_flags;
સ્થિર unsigned create_reflog_flag;
સ્થિર અચલ અક્ષર *સંદેશ;

સંરચના command_options {
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
સ્થિર અચલ અક્ષર *parse_arg(અચલ અક્ષર *next, સંરચના strbuf *arg)
{
	જો (*next == '"') {
		અચલ અક્ષર *orig = next;

		જો (unquote_c_style(arg, next, &next))
			die("badly quoted argument: %s", orig);
		જો (*next && !isspace(*next))
			die("unexpected character after quoted argument: %s", orig);
	} નહીંતર {
		જ્યારે (*next && !isspace(*next))
			strbuf_addch(arg, *next++);
	}

	પાછા next;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
સ્થિર અક્ષર *parse_refname(અચલ અક્ષર **next)
{
	સંરચના strbuf ref = STRBUF_INIT;

	જો (લીટી_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &ref);
	} નહીંતર {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *next);
		*next += ref.len;
	}

	જો (!ref.len) {
		strbuf_release(&ref);
		પાછા ખાલી;
	}

	જો (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	પાછા strbuf_detach(&ref, ખાલી);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
સ્થિર અક્ષર *parse_next_refname(અચલ અક્ષર **next)
{
	જો (લીટી_termination) {
		/* Without -z, consume SP and use next argument */
		જો (!**next || **next == લીટી_termination)
			પાછા ખાલી;
		જો (**next != ' ')
			die("expected SP but got: %s", *next);
	} નહીંતર {
		/* With -z, read the next NUL-terminated line */
		જો (**next)
			પાછા ખાલી;
	}
	/* Skip the delimiter */
	(*next)++;

	પાછા parse_refname(next);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
સ્થિર અક્ષર *parse_next_arg(અચલ અક્ષર **next)
{
	સંરચના strbuf arg = STRBUF_INIT;

	જો (લીટી_termination) {
		/* Without -z, consume SP and use next argument */
		જો (!**next || **next == લીટી_termination)
			પાછા ખાલી;
		જો (**next != ' ')
			die("expected SP but got: %s", *next);
	} નહીંતર {
		/* With -z, read the next NUL-terminated line */
		જો (**next)
			પાછા ખાલી;
	}
	/* Skip the delimiter */
	(*next)++;

	જો (લીટી_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &arg);
	} નહીંતર {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *next);
		*next += arg.len;
	}

	જો (arg.len)
		પાછા strbuf_detach(&arg, ખાલી);

	strbuf_release(&arg);
	પાછા ખાલી;
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
સ્થિર પૂર્ણાંક parse_next_oid(અચલ અક્ષર **next, અચલ અક્ષર *end,
			  સંરચના object_id *oid,
			  અચલ અક્ષર *command, અચલ અક્ષર *refname,
			  પૂર્ણાંક flags)
{
	સંરચના strbuf arg = STRBUF_INIT;
	પૂર્ણાંક ret = 0;

	જો (*next == end)
		જાઓ eof;

	જો (લીટી_termination) {
		/* Without -z, consume SP and use next argument */
		જો (!**next || **next == લીટી_termination)
			પાછા 1;
		જો (**next != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *next);
		(*next)++;
		*next = parse_arg(*next, &arg);
		જો (arg.len) {
			જો (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				જાઓ invalid;
		} નહીંતર {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} નહીંતર {
		/* With -z, read the next NUL-terminated line */
		જો (**next)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *next);
		(*next)++;
		જો (*next == end)
			જાઓ eof;
		strbuf_addstr(&arg, *next);
		*next += arg.len;

		જો (arg.len) {
			જો (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				જાઓ invalid;
		} નહીંતર જો (flags & PARSE_SHA1_ALLOW_EMPTY) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} નહીંતર {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	પાછા ret;

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

સ્થિર શૂન્ય print_rejected_refs(અચલ અક્ષર *refname,
				અચલ સંરચના object_id *old_oid,
				અચલ સંરચના object_id *new_oid,
				અચલ અક્ષર *old_target,
				અચલ અક્ષર *new_target,
				ગણના ref_transaction_error err,
				અચલ અક્ષર *details,
				શૂન્ય *cb_data UNUSED)
{
	સંરચના strbuf sb = STRBUF_INIT;

	જો (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_to_hex(new_oid) : new_target,
		    old_oid ? oid_to_hex(old_oid) : old_target,
		    ref_transaction_error_સંદેશ(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
સ્થિર શૂન્ય handle_ref_transaction_error(અચલ અક્ષર *refname,
					 સંરચના object_id *new_oid,
					 સંરચના object_id *old_oid,
					 અચલ અક્ષર *new_target,
					 અચલ અક્ષર *old_target,
					 ગણના ref_transaction_error tx_err,
					 સંરચના strbuf *err,
					 સંરચના command_options *opts)
{
	જો (!tx_err)
		પાછા;

	જો (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_target,
				    new_target, tx_err, err->buf, ખાલી);
		પાછા;
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

સ્થિર શૂન્ય parse_cmd_update(સંરચના ref_transaction *transaction,
			     અચલ અક્ષર *next, અચલ અક્ષર *end,
			     સંરચના command_options *opts)
{
	સંરચના strbuf err = STRBUF_INIT;
	અક્ષર *refname;
	સંરચના object_id new_oid, old_oid;
	ગણના ref_transaction_error tx_err;
	પૂર્ણાંક have_old;

	refname = parse_refname(&next);
	જો (!refname)
		die("update: missing <ref>");

	જો (parse_next_oid(&next, end, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_EMPTY))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_next_oid(&next, end, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	જો (*next != લીટી_termination)
		die("update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : ખાલી,
					ખાલી, ખાલી,
					update_flags | create_reflog_flag,
					સંદેશ, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : ખાલી,
				     ખાલી, ખાલી, tx_err, &err, opts);


	update_flags = default_flags;
	મુક્ત(refname);
	strbuf_release(&err);
}

સ્થિર શૂન્ય parse_cmd_symref_update(સંરચના ref_transaction *transaction,
				    અચલ અક્ષર *next, અચલ અક્ષર *end UNUSED,
				    સંરચના command_options *opts)
{
	અક્ષર *refname, *new_target, *old_arg;
	ગણના ref_transaction_error tx_err;
	અક્ષર *old_target = ખાલી;
	સંરચના strbuf err = STRBUF_INIT;
	સંરચના object_id old_oid;
	પૂર્ણાંક have_old_oid = 0;

	refname = parse_refname(&next);
	જો (!refname)
		die("symref-update: missing <ref>");

	new_target = parse_next_refname(&next);
	જો (!new_target)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_next_arg(&next);
	જો (old_arg) {
		old_target = parse_next_arg(&next);
		જો (!old_target)
			die("symref-update %s: expected old value", refname);

		જો (!strcmp(old_arg, "oid")) {
			જો (repo_get_oid_with_flags(the_repository, old_target, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_target);

			have_old_oid = 1;
		} નહીંતર જો (!strcmp(old_arg, "ref")) {
			જો (check_refname_format(old_target, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_target);
		} નહીંતર {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	જો (*next != લીટી_termination)
		die("symref-update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname, ખાલી,
					have_old_oid ? &old_oid : ખાલી,
					new_target,
					have_old_oid ? ખાલી : old_target,
					update_flags | create_reflog_flag,
					સંદેશ, &err);
	handle_ref_transaction_error(refname, ખાલી, have_old_oid ? &old_oid : ખાલી,
				     new_target, have_old_oid ? ખાલી : old_target,
				     tx_err, &err, opts);

	update_flags = default_flags;
	મુક્ત(refname);
	મુક્ત(old_arg);
	મુક્ત(old_target);
	મુક્ત(new_target);
	strbuf_release(&err);
}

સ્થિર શૂન્ય parse_cmd_create(સંરચના ref_transaction *transaction,
			     અચલ અક્ષર *next, અચલ અક્ષર *end,
			     સંરચના command_options *opts)
{
	સંરચના strbuf err = STRBUF_INIT;
	અક્ષર *refname;
	સંરચના object_id new_oid;
	ગણના ref_transaction_error tx_err;

	refname = parse_refname(&next);
	જો (!refname)
		die("create: missing <ref>");

	જો (parse_next_oid(&next, end, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	જો (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	જો (*next != લીટી_termination)
		die("create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, ખાલી,
					update_flags | create_reflog_flag,
					સંદેશ, &err);
	handle_ref_transaction_error(refname, &new_oid, ખાલી, ખાલી, ખાલી, tx_err,
				     &err, opts);

	update_flags = default_flags;
	મુક્ત(refname);
	strbuf_release(&err);
}

સ્થિર શૂન્ય parse_cmd_symref_create(સંરચના ref_transaction *transaction,
				    અચલ અક્ષર *next, અચલ અક્ષર *end UNUSED,
				    સંરચના command_options *opts)
{
	સંરચના strbuf err = STRBUF_INIT;
	અક્ષર *refname, *new_target;
	ગણના ref_transaction_error tx_err;

	refname = parse_refname(&next);
	જો (!refname)
		die("symref-create: missing <ref>");

	new_target = parse_next_refname(&next);
	જો (!new_target)
		die("symref-create %s: missing <new-target>", refname);

	જો (*next != લીટી_termination)
		die("symref-create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, ખાલી, new_target,
					update_flags | create_reflog_flag,
					સંદેશ, &err);
	handle_ref_transaction_error(refname, ખાલી, ખાલી, new_target, ખાલી,
				     tx_err, &err, opts);

	update_flags = default_flags;
	મુક્ત(refname);
	મુક્ત(new_target);
	strbuf_release(&err);
}

સ્થિર શૂન્ય parse_cmd_delete(સંરચના ref_transaction *transaction,
			     અચલ અક્ષર *next, અચલ અક્ષર *end,
			     સંરચના command_options *opts UNUSED)
{
	સંરચના strbuf err = STRBUF_INIT;
	અક્ષર *refname;
	સંરચના object_id old_oid;
	પૂર્ણાંક have_old;

	refname = parse_refname(&next);
	જો (!refname)
		die("delete: missing <ref>");

	જો (parse_next_oid(&next, end, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} નહીંતર {
		જો (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	જો (*next != લીટી_termination)
		die("delete %s: extra input: %s", refname, next);

	જો (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : ખાલી,
				   ખાલી, update_flags, સંદેશ, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	મુક્ત(refname);
	strbuf_release(&err);
}

સ્થિર શૂન્ય parse_cmd_symref_delete(સંરચના ref_transaction *transaction,
				    અચલ અક્ષર *next, અચલ અક્ષર *end UNUSED,
				    સંરચના command_options *opts UNUSED)
{
	સંરચના strbuf err = STRBUF_INIT;
	અક્ષર *refname, *old_target;

	જો (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&next);
	જો (!refname)
		die("symref-delete: missing <ref>");

	old_target = parse_next_refname(&next);

	જો (*next != લીટી_termination)
		die("symref-delete %s: extra input: %s", refname, next);

	જો (ref_transaction_delete(transaction, refname, ખાલી,
				   old_target, update_flags, સંદેશ, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	મુક્ત(refname);
	મુક્ત(old_target);
	strbuf_release(&err);
}

સ્થિર શૂન્ય parse_cmd_verify(સંરચના ref_transaction *transaction,
			     અચલ અક્ષર *next, અચલ અક્ષર *end,
			     સંરચના command_options *opts UNUSED)
{
	સંરચના strbuf err = STRBUF_INIT;
	અક્ષર *refname;
	સંરચના object_id old_oid;

	refname = parse_refname(&next);
	જો (!refname)
		die("verify: missing <ref>");

	જો (parse_next_oid(&next, end, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	જો (*next != લીટી_termination)
		die("verify %s: extra input: %s", refname, next);

	જો (ref_transaction_verify(transaction, refname, &old_oid,
				   ખાલી, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	મુક્ત(refname);
	strbuf_release(&err);
}

સ્થિર શૂન્ય parse_cmd_symref_verify(સંરચના ref_transaction *transaction,
				    અચલ અક્ષર *next, અચલ અક્ષર *end UNUSED,
				    સંરચના command_options *opts UNUSED)
{
	સંરચના strbuf err = STRBUF_INIT;
	સંરચના object_id old_oid;
	અક્ષર *refname, *old_target;

	જો (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&next);
	જો (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_target = parse_next_refname(&next);
	જો (!old_target)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	જો (*next != લીટી_termination)
		die("symref-verify %s: extra input: %s", refname, next);

	જો (ref_transaction_verify(transaction, refname,
				   old_target ? ખાલી : &old_oid,
				   old_target, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	મુક્ત(refname);
	મુક્ત(old_target);
	strbuf_release(&err);
}

સ્થિર શૂન્ય report_ok(અચલ અક્ષર *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

સ્થિર શૂન્ય parse_cmd_option(સંરચના ref_transaction *transaction UNUSED,
			     અચલ અક્ષર *next, અચલ અક્ષર *end UNUSED,
			     સંરચના command_options *opts UNUSED)
{
	અચલ અક્ષર *rest;
	જો (skip_prefix(next, "no-deref", &rest) && *rest == લીટી_termination)
		update_flags |= REF_NO_DEREF;
	નહીંતર
		die("option unknown: %s", next);
}

સ્થિર શૂન્ય parse_cmd_start(સંરચના ref_transaction *transaction UNUSED,
			    અચલ અક્ષર *next, અચલ અક્ષર *end UNUSED,
			    સંરચના command_options *opts UNUSED)
{
	જો (*next != લીટી_termination)
		die("start: extra input: %s", next);
	report_ok("start");
}

સ્થિર શૂન્ય parse_cmd_prepare(સંરચના ref_transaction *transaction,
			      અચલ અક્ષર *next, અચલ અક્ષર *end UNUSED,
			      સંરચના command_options *opts UNUSED)
{
	સંરચના strbuf error = STRBUF_INIT;
	જો (*next != લીટી_termination)
		die("prepare: extra input: %s", next);
	જો (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

સ્થિર શૂન્ય parse_cmd_abort(સંરચના ref_transaction *transaction,
			    અચલ અક્ષર *next, અચલ અક્ષર *end UNUSED,
			    સંરચના command_options *opts UNUSED)
{
	સંરચના strbuf error = STRBUF_INIT;
	જો (*next != લીટી_termination)
		die("abort: extra input: %s", next);
	જો (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

સ્થિર શૂન્ય parse_cmd_commit(સંરચના ref_transaction *transaction,
			     અચલ અક્ષર *next, અચલ અક્ષર *end UNUSED,
			     સંરચના command_options *opts UNUSED)
{
	સંરચના strbuf error = STRBUF_INIT;
	જો (*next != લીટી_termination)
		die("commit: extra input: %s", next);
	જો (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ખાલી);

	report_ok("commit");
	ref_transaction_free(transaction);
}

ગણના update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

સ્થિર અચલ સંરચના parse_cmd {
	અચલ અક્ષર *prefix;
	શૂન્ય (*fn)(સંરચના ref_transaction *, અચલ અક્ષર *, અચલ અક્ષર *,
		   સંરચના command_options *);
	unsigned args;
	ગણના update_refs_state state;
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

સ્થિર શૂન્ય update_refs_stdin(unsigned પૂર્ણાંક flags)
{
	સંરચના strbuf આગત = STRBUF_INIT, err = STRBUF_INIT;
	ગણના update_refs_state state = UPDATE_REFS_OPEN;
	સંરચના ref_transaction *transaction;
	પૂર્ણાંક i, j;

	સંરચના command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	જો (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	જ્યારે (!strbuf_getwholeline(&આગત, stdin, લીટી_termination)) {
		અચલ સંરચના parse_cmd *cmd = ખાલી;

		જો (*આગત.buf == લીટી_termination)
			die("empty command in input");
		નહીંતર જો (isspace(*આગત.buf))
			die("whitespace before command: %s", આગત.buf);

		માટે (i = 0; i < ARRAY_SIZE(command); i++) {
			અચલ અક્ષર *prefix = command[i].prefix;
			અક્ષર c;

			જો (!starts_with(આગત.buf, prefix))
				ચાલુ;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].args ? ' ' : લીટી_termination;
			જો (આગત.buf[strlen(prefix)] != c)
				ચાલુ;

			cmd = &command[i];
			તોડો;
		}
		જો (!cmd)
			die("unknown command: %s", આગત.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		માટે (j = 1; લીટી_termination == '\0' && j < cmd->args; j++)
			જો (strbuf_appendwholeline(&આગત, stdin, લીટી_termination))
				તોડો;

		પસંદગી (state) {
		સ્થિતિ UPDATE_REFS_OPEN:
		સ્થિતિ UPDATE_REFS_STARTED:
			જો (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			જો (cmd->state >= state)
				state = cmd->state;
			તોડો;
		સ્થિતિ UPDATE_REFS_PREPARED:
			જો (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			તોડો;
		સ્થિતિ UPDATE_REFS_CLOSED:
			જો (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			જો (!transaction)
				die("%s", err.buf);

			તોડો;
		}

		cmd->fn(transaction, આગત.buf + strlen(cmd->prefix) + !!cmd->args,
			આગત.buf + આગત.len, &opts);
	}

	પસંદગી (state) {
	સ્થિતિ UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		જો (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ખાલી);
		ref_transaction_free(transaction);
		તોડો;
	સ્થિતિ UPDATE_REFS_STARTED:
	સ્થિતિ UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		જો (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		તોડો;
	સ્થિતિ UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		તોડો;
	}

	strbuf_release(&err);
	strbuf_release(&આગત);
}

પૂર્ણાંક cmd_update_ref(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *prefix,
		   સંરચના repository *repo UNUSED)
{
	અચલ અક્ષર *refname, *oldval;
	સંરચના object_id oid, oldoid;
	પૂર્ણાંક delete = 0, no_deref = 0, read_stdin = 0, end_null = 0;
	પૂર્ણાંક create_reflog = 0;
	unsigned પૂર્ણાંક flags = 0;

	સંરચના option options[] = {
		OPT_STRING( 'm', ખાલી, &સંદેશ, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', ખાલી, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', ખાલી, &end_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_BIT('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ખાલી);
	argc = parse_options(argc, argv, prefix, options, git_update_ref_usage,
			     0);
	જો (સંદેશ && !*સંદેશ)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	જો (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	જો (read_stdin) {
		જો (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		જો (end_null)
			લીટી_termination = '\0';
		update_refs_stdin(flags);
		પાછા 0;
	} નહીંતર જો (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	જો (end_null)
		usage_with_options(git_update_ref_usage, options);

	જો (delete) {
		જો (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} નહીંતર {
		અચલ અક્ષર *કિંમત;
		જો (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		કિંમત = argv[1];
		oldval = argv[2];
		જો (repo_get_oid_with_flags(the_repository, કિંમત, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", કિંમત);
	}

	જો (oldval) {
		જો (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		નહીંતર જો (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	જો (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		પાછા refs_delete_ref(get_main_ref_store(the_repository),
				       સંદેશ, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : ખાલી,
				       default_flags);
	નહીંતર
		પાછા refs_update_ref(get_main_ref_store(the_repository),
				       સંદેશ, refname, &oid,
				       oldval ? &oldoid : ખાલી,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
