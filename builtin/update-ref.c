#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hash.h"
#include "hex.h"
#include "refs.h"
#include "object-name.h"
#include "parse-options.h"
#include "quote.h"

static const var_na * const git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	NULL
};

static var_na line_termination = '\n';
static unsigned පූර්ණ_සංඛ්‍යා update_flags;
static unsigned පූර්ණ_සංඛ්‍යා default_flags;
static unsigned create_reflog_flag;
static const var_na *msg;

sa_nracanaa command_options {
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
static const var_na *parse_arg(const var_na *next, sa_nracanaa strbuf *arg)
{
	නම් (*next == '"') {
		const var_na *orig = next;

		නම් (unquote_c_style(arg, next, &next))
			die("badly quoted argument: %s", orig);
		නම් (*next && !isspace(*next))
			die("unexpected character after quoted argument: %s", orig);
	} නැතහොත් {
		අතරතුර (*next && !isspace(*next))
			strbuf_addch(arg, *next++);
	}

	ආපසු next;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
static var_na *parse_refname(const var_na **next)
{
	sa_nracanaa strbuf ref = STRBUF_INIT;

	නම් (line_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &ref);
	} නැතහොත් {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *next);
		*next += ref.len;
	}

	නම් (!ref.len) {
		strbuf_release(&ref);
		ආපසු NULL;
	}

	නම් (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	ආපසු strbuf_detach(&ref, NULL);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
static var_na *parse_next_refname(const var_na **next)
{
	නම් (line_termination) {
		/* Without -z, consume SP and use next argument */
		නම් (!**next || **next == line_termination)
			ආපසු NULL;
		නම් (**next != ' ')
			die("expected SP but got: %s", *next);
	} නැතහොත් {
		/* With -z, read the next NUL-terminated line */
		නම් (**next)
			ආපසු NULL;
	}
	/* Skip the delimiter */
	(*next)++;

	ආපසු parse_refname(next);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
static var_na *parse_next_arg(const var_na **next)
{
	sa_nracanaa strbuf arg = STRBUF_INIT;

	නම් (line_termination) {
		/* Without -z, consume SP and use next argument */
		නම් (!**next || **next == line_termination)
			ආපසු NULL;
		නම් (**next != ' ')
			die("expected SP but got: %s", *next);
	} නැතහොත් {
		/* With -z, read the next NUL-terminated line */
		නම් (**next)
			ආපසු NULL;
	}
	/* Skip the delimiter */
	(*next)++;

	නම් (line_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &arg);
	} නැතහොත් {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *next);
		*next += arg.len;
	}

	නම් (arg.len)
		ආපසු strbuf_detach(&arg, NULL);

	strbuf_release(&arg);
	ආපසු NULL;
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
static පූර්ණ_සංඛ්‍යා parse_next_oid(const var_na **next, const var_na *end,
			  sa_nracanaa object_id *oid,
			  const var_na *command, const var_na *refname,
			  පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa strbuf arg = STRBUF_INIT;
	පූර්ණ_සංඛ්‍යා ret = 0;

	නම් (*next == end)
		goto eof;

	නම් (line_termination) {
		/* Without -z, consume SP and use next argument */
		නම් (!**next || **next == line_termination)
			ආපසු 1;
		නම් (**next != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *next);
		(*next)++;
		*next = parse_arg(*next, &arg);
		නම් (arg.len) {
			නම් (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				goto invalid;
		} නැතහොත් {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} නැතහොත් {
		/* With -z, read the next NUL-terminated line */
		නම් (**next)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *next);
		(*next)++;
		නම් (*next == end)
			goto eof;
		strbuf_addstr(&arg, *next);
		*next += arg.len;

		නම් (arg.len) {
			නම් (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				goto invalid;
		} නැතහොත් නම් (flags & PARSE_SHA1_ALLOW_EMPTY) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} නැතහොත් {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	ආපසු ret;

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

static void print_rejected_refs(const var_na *refname,
				const sa_nracanaa object_id *old_oid,
				const sa_nracanaa object_id *new_oid,
				const var_na *old_target,
				const var_na *new_target,
				ga_nanaa ref_transaction_error err,
				const var_na *details,
				void *cb_data UNUSED)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;

	නම් (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_to_hex(new_oid) : new_target,
		    old_oid ? oid_to_hex(old_oid) : old_target,
		    ref_transaction_error_msg(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
static void handle_ref_transaction_error(const var_na *refname,
					 sa_nracanaa object_id *new_oid,
					 sa_nracanaa object_id *old_oid,
					 const var_na *new_target,
					 const var_na *old_target,
					 ga_nanaa ref_transaction_error tx_err,
					 sa_nracanaa strbuf *err,
					 sa_nracanaa command_options *opts)
{
	නම් (!tx_err)
		ආපසු;

	නම් (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_target,
				    new_target, tx_err, err->buf, NULL);
		ආපසු;
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

static void parse_cmd_update(sa_nracanaa ref_transaction *transaction,
			     const var_na *next, const var_na *end,
			     sa_nracanaa command_options *opts)
{
	sa_nracanaa strbuf err = STRBUF_INIT;
	var_na *refname;
	sa_nracanaa object_id new_oid, old_oid;
	ga_nanaa ref_transaction_error tx_err;
	පූර්ණ_සංඛ්‍යා have_old;

	refname = parse_refname(&next);
	නම් (!refname)
		die("update: missing <ref>");

	නම් (parse_next_oid(&next, end, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_EMPTY))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_next_oid(&next, end, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	නම් (*next != line_termination)
		die("update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : NULL,
					NULL, NULL,
					update_flags | create_reflog_flag,
					msg, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : NULL,
				     NULL, NULL, tx_err, &err, opts);


	update_flags = default_flags;
	free(refname);
	strbuf_release(&err);
}

static void parse_cmd_symref_update(sa_nracanaa ref_transaction *transaction,
				    const var_na *next, const var_na *end UNUSED,
				    sa_nracanaa command_options *opts)
{
	var_na *refname, *new_target, *old_arg;
	ga_nanaa ref_transaction_error tx_err;
	var_na *old_target = NULL;
	sa_nracanaa strbuf err = STRBUF_INIT;
	sa_nracanaa object_id old_oid;
	පූර්ණ_සංඛ්‍යා have_old_oid = 0;

	refname = parse_refname(&next);
	නම් (!refname)
		die("symref-update: missing <ref>");

	new_target = parse_next_refname(&next);
	නම් (!new_target)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_next_arg(&next);
	නම් (old_arg) {
		old_target = parse_next_arg(&next);
		නම් (!old_target)
			die("symref-update %s: expected old value", refname);

		නම් (!strcmp(old_arg, "oid")) {
			නම් (repo_get_oid_with_flags(the_repository, old_target, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_target);

			have_old_oid = 1;
		} නැතහොත් නම් (!strcmp(old_arg, "ref")) {
			නම් (check_refname_format(old_target, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_target);
		} නැතහොත් {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	නම් (*next != line_termination)
		die("symref-update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname, NULL,
					have_old_oid ? &old_oid : NULL,
					new_target,
					have_old_oid ? NULL : old_target,
					update_flags | create_reflog_flag,
					msg, &err);
	handle_ref_transaction_error(refname, NULL, have_old_oid ? &old_oid : NULL,
				     new_target, have_old_oid ? NULL : old_target,
				     tx_err, &err, opts);

	update_flags = default_flags;
	free(refname);
	free(old_arg);
	free(old_target);
	free(new_target);
	strbuf_release(&err);
}

static void parse_cmd_create(sa_nracanaa ref_transaction *transaction,
			     const var_na *next, const var_na *end,
			     sa_nracanaa command_options *opts)
{
	sa_nracanaa strbuf err = STRBUF_INIT;
	var_na *refname;
	sa_nracanaa object_id new_oid;
	ga_nanaa ref_transaction_error tx_err;

	refname = parse_refname(&next);
	නම් (!refname)
		die("create: missing <ref>");

	නම් (parse_next_oid(&next, end, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	නම් (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	නම් (*next != line_termination)
		die("create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, NULL,
					update_flags | create_reflog_flag,
					msg, &err);
	handle_ref_transaction_error(refname, &new_oid, NULL, NULL, NULL, tx_err,
				     &err, opts);

	update_flags = default_flags;
	free(refname);
	strbuf_release(&err);
}

static void parse_cmd_symref_create(sa_nracanaa ref_transaction *transaction,
				    const var_na *next, const var_na *end UNUSED,
				    sa_nracanaa command_options *opts)
{
	sa_nracanaa strbuf err = STRBUF_INIT;
	var_na *refname, *new_target;
	ga_nanaa ref_transaction_error tx_err;

	refname = parse_refname(&next);
	නම් (!refname)
		die("symref-create: missing <ref>");

	new_target = parse_next_refname(&next);
	නම් (!new_target)
		die("symref-create %s: missing <new-target>", refname);

	නම් (*next != line_termination)
		die("symref-create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, NULL, new_target,
					update_flags | create_reflog_flag,
					msg, &err);
	handle_ref_transaction_error(refname, NULL, NULL, new_target, NULL,
				     tx_err, &err, opts);

	update_flags = default_flags;
	free(refname);
	free(new_target);
	strbuf_release(&err);
}

static void parse_cmd_delete(sa_nracanaa ref_transaction *transaction,
			     const var_na *next, const var_na *end,
			     sa_nracanaa command_options *opts UNUSED)
{
	sa_nracanaa strbuf err = STRBUF_INIT;
	var_na *refname;
	sa_nracanaa object_id old_oid;
	පූර්ණ_සංඛ්‍යා have_old;

	refname = parse_refname(&next);
	නම් (!refname)
		die("delete: missing <ref>");

	නම් (parse_next_oid(&next, end, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} නැතහොත් {
		නම් (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	නම් (*next != line_termination)
		die("delete %s: extra input: %s", refname, next);

	නම් (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : NULL,
				   NULL, update_flags, msg, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	free(refname);
	strbuf_release(&err);
}

static void parse_cmd_symref_delete(sa_nracanaa ref_transaction *transaction,
				    const var_na *next, const var_na *end UNUSED,
				    sa_nracanaa command_options *opts UNUSED)
{
	sa_nracanaa strbuf err = STRBUF_INIT;
	var_na *refname, *old_target;

	නම් (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&next);
	නම් (!refname)
		die("symref-delete: missing <ref>");

	old_target = parse_next_refname(&next);

	නම් (*next != line_termination)
		die("symref-delete %s: extra input: %s", refname, next);

	නම් (ref_transaction_delete(transaction, refname, NULL,
				   old_target, update_flags, msg, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	free(refname);
	free(old_target);
	strbuf_release(&err);
}

static void parse_cmd_verify(sa_nracanaa ref_transaction *transaction,
			     const var_na *next, const var_na *end,
			     sa_nracanaa command_options *opts UNUSED)
{
	sa_nracanaa strbuf err = STRBUF_INIT;
	var_na *refname;
	sa_nracanaa object_id old_oid;

	refname = parse_refname(&next);
	නම් (!refname)
		die("verify: missing <ref>");

	නම් (parse_next_oid(&next, end, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	නම් (*next != line_termination)
		die("verify %s: extra input: %s", refname, next);

	නම් (ref_transaction_verify(transaction, refname, &old_oid,
				   NULL, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	free(refname);
	strbuf_release(&err);
}

static void parse_cmd_symref_verify(sa_nracanaa ref_transaction *transaction,
				    const var_na *next, const var_na *end UNUSED,
				    sa_nracanaa command_options *opts UNUSED)
{
	sa_nracanaa strbuf err = STRBUF_INIT;
	sa_nracanaa object_id old_oid;
	var_na *refname, *old_target;

	නම් (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&next);
	නම් (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_target = parse_next_refname(&next);
	නම් (!old_target)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	නම් (*next != line_termination)
		die("symref-verify %s: extra input: %s", refname, next);

	නම් (ref_transaction_verify(transaction, refname,
				   old_target ? NULL : &old_oid,
				   old_target, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	free(refname);
	free(old_target);
	strbuf_release(&err);
}

static void report_ok(const var_na *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

static void parse_cmd_option(sa_nracanaa ref_transaction *transaction UNUSED,
			     const var_na *next, const var_na *end UNUSED,
			     sa_nracanaa command_options *opts UNUSED)
{
	const var_na *rest;
	නම් (skip_prefix(next, "no-deref", &rest) && *rest == line_termination)
		update_flags |= REF_NO_DEREF;
	නැතහොත්
		die("option unknown: %s", next);
}

static void parse_cmd_start(sa_nracanaa ref_transaction *transaction UNUSED,
			    const var_na *next, const var_na *end UNUSED,
			    sa_nracanaa command_options *opts UNUSED)
{
	නම් (*next != line_termination)
		die("start: extra input: %s", next);
	report_ok("start");
}

static void parse_cmd_prepare(sa_nracanaa ref_transaction *transaction,
			      const var_na *next, const var_na *end UNUSED,
			      sa_nracanaa command_options *opts UNUSED)
{
	sa_nracanaa strbuf error = STRBUF_INIT;
	නම් (*next != line_termination)
		die("prepare: extra input: %s", next);
	නම් (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

static void parse_cmd_abort(sa_nracanaa ref_transaction *transaction,
			    const var_na *next, const var_na *end UNUSED,
			    sa_nracanaa command_options *opts UNUSED)
{
	sa_nracanaa strbuf error = STRBUF_INIT;
	නම් (*next != line_termination)
		die("abort: extra input: %s", next);
	නම් (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

static void parse_cmd_commit(sa_nracanaa ref_transaction *transaction,
			     const var_na *next, const var_na *end UNUSED,
			     sa_nracanaa command_options *opts UNUSED)
{
	sa_nracanaa strbuf error = STRBUF_INIT;
	නම් (*next != line_termination)
		die("commit: extra input: %s", next);
	නම් (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, NULL);

	report_ok("commit");
	ref_transaction_free(transaction);
}

ga_nanaa update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

static const sa_nracanaa parse_cmd {
	const var_na *prefix;
	void (*fn)(sa_nracanaa ref_transaction *, const var_na *, const var_na *,
		   sa_nracanaa command_options *);
	unsigned args;
	ga_nanaa update_refs_state state;
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

static void update_refs_stdin(unsigned පූර්ණ_සංඛ්‍යා flags)
{
	sa_nracanaa strbuf input = STRBUF_INIT, err = STRBUF_INIT;
	ga_nanaa update_refs_state state = UPDATE_REFS_OPEN;
	sa_nracanaa ref_transaction *transaction;
	පූර්ණ_සංඛ්‍යා i, j;

	sa_nracanaa command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	නම් (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	අතරතුර (!strbuf_getwholeline(&input, stdin, line_termination)) {
		const sa_nracanaa parse_cmd *cmd = NULL;

		නම් (*input.buf == line_termination)
			die("empty command in input");
		නැතහොත් නම් (isspace(*input.buf))
			die("whitespace before command: %s", input.buf);

		සඳහා (i = 0; i < ARRAY_SIZE(command); i++) {
			const var_na *prefix = command[i].prefix;
			var_na c;

			නම් (!starts_with(input.buf, prefix))
				continue;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].args ? ' ' : line_termination;
			නම් (input.buf[strlen(prefix)] != c)
				continue;

			cmd = &command[i];
			bha_nga;
		}
		නම් (!cmd)
			die("unknown command: %s", input.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		සඳහා (j = 1; line_termination == '\0' && j < cmd->args; j++)
			නම් (strbuf_appendwholeline(&input, stdin, line_termination))
				bha_nga;

		switch (state) {
		pak_sa UPDATE_REFS_OPEN:
		pak_sa UPDATE_REFS_STARTED:
			නම් (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			නම් (cmd->state >= state)
				state = cmd->state;
			bha_nga;
		pak_sa UPDATE_REFS_PREPARED:
			නම් (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			bha_nga;
		pak_sa UPDATE_REFS_CLOSED:
			නම් (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			නම් (!transaction)
				die("%s", err.buf);

			bha_nga;
		}

		cmd->fn(transaction, input.buf + strlen(cmd->prefix) + !!cmd->args,
			input.buf + input.len, &opts);
	}

	switch (state) {
	pak_sa UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		නම් (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, NULL);
		ref_transaction_free(transaction);
		bha_nga;
	pak_sa UPDATE_REFS_STARTED:
	pak_sa UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		නම් (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		bha_nga;
	pak_sa UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		bha_nga;
	}

	strbuf_release(&err);
	strbuf_release(&input);
}

පූර්ණ_සංඛ්‍යා cmd_update_ref(පූර්ණ_සංඛ්‍යා argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	const var_na *refname, *oldval;
	sa_nracanaa object_id oid, oldoid;
	පූර්ණ_සංඛ්‍යා delete = 0, no_deref = 0, read_stdin = 0, end_null = 0;
	පූර්ණ_සංඛ්‍යා create_reflog = 0;
	unsigned පූර්ණ_සංඛ්‍යා flags = 0;

	sa_nracanaa option options[] = {
		OPT_STRING( 'm', NULL, &msg, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', NULL, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', NULL, &end_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_BIT('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, NULL);
	argc = parse_options(argc, argv, prefix, options, git_update_ref_usage,
			     0);
	නම් (msg && !*msg)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	නම් (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	නම් (read_stdin) {
		නම් (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		නම් (end_null)
			line_termination = '\0';
		update_refs_stdin(flags);
		ආපසු 0;
	} නැතහොත් නම් (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	නම් (end_null)
		usage_with_options(git_update_ref_usage, options);

	නම් (delete) {
		නම් (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} නැතහොත් {
		const var_na *value;
		නම් (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		value = argv[1];
		oldval = argv[2];
		නම් (repo_get_oid_with_flags(the_repository, value, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", value);
	}

	නම් (oldval) {
		නම් (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		නැතහොත් නම් (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	නම් (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		ආපසු refs_delete_ref(get_main_ref_store(the_repository),
				       msg, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : NULL,
				       default_flags);
	නැතහොත්
		ආපසු refs_update_ref(get_main_ref_store(the_repository),
				       msg, refname, &oid,
				       oldval ? &oldoid : NULL,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
