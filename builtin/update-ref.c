#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	ଖାଲି
};

ସ୍ଥିର ଅକ୍ଷର ଧାଡ଼ି_termination = '\n';
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା update_flags;
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_flags;
ସ୍ଥିର unsigned create_reflog_flag;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ;

ସଂରଚନା command_options {
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
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *parse_arg(ଅଚଳ ଅକ୍ଷର *next, ସଂରଚନା strbuf *arg)
{
	ଯଦି (*next == '"') {
		ଅଚଳ ଅକ୍ଷର *orig = next;

		ଯଦି (unquote_c_style(arg, next, &next))
			die("badly quoted argument: %s", orig);
		ଯଦି (*next && !isspace(*next))
			die("unexpected character after quoted argument: %s", orig);
	} ଅନ୍ୟଥା {
		ଯେତେବେଳେ (*next && !isspace(*next))
			strbuf_addch(arg, *next++);
	}

	ଫେରନ୍ତୁ next;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
ସ୍ଥିର ଅକ୍ଷର *parse_refname(ଅଚଳ ଅକ୍ଷର **next)
{
	ସଂରଚନା strbuf ref = STRBUF_INIT;

	ଯଦି (ଧାଡ଼ି_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &ref);
	} ଅନ୍ୟଥା {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *next);
		*next += ref.len;
	}

	ଯଦି (!ref.len) {
		strbuf_release(&ref);
		ଫେରନ୍ତୁ ଖାଲି;
	}

	ଯଦି (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	ଫେରନ୍ତୁ strbuf_detach(&ref, ଖାଲି);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
ସ୍ଥିର ଅକ୍ଷର *parse_next_refname(ଅଚଳ ଅକ୍ଷର **next)
{
	ଯଦି (ଧାଡ଼ି_termination) {
		/* Without -z, consume SP and use next argument */
		ଯଦି (!**next || **next == ଧାଡ଼ି_termination)
			ଫେରନ୍ତୁ ଖାଲି;
		ଯଦି (**next != ' ')
			die("expected SP but got: %s", *next);
	} ଅନ୍ୟଥା {
		/* With -z, read the next NUL-terminated line */
		ଯଦି (**next)
			ଫେରନ୍ତୁ ଖାଲି;
	}
	/* Skip the delimiter */
	(*next)++;

	ଫେରନ୍ତୁ parse_refname(next);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
ସ୍ଥିର ଅକ୍ଷର *parse_next_arg(ଅଚଳ ଅକ୍ଷର **next)
{
	ସଂରଚନା strbuf arg = STRBUF_INIT;

	ଯଦି (ଧାଡ଼ି_termination) {
		/* Without -z, consume SP and use next argument */
		ଯଦି (!**next || **next == ଧାଡ଼ି_termination)
			ଫେରନ୍ତୁ ଖାଲି;
		ଯଦି (**next != ' ')
			die("expected SP but got: %s", *next);
	} ଅନ୍ୟଥା {
		/* With -z, read the next NUL-terminated line */
		ଯଦି (**next)
			ଫେରନ୍ତୁ ଖାଲି;
	}
	/* Skip the delimiter */
	(*next)++;

	ଯଦି (ଧାଡ଼ି_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &arg);
	} ଅନ୍ୟଥା {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *next);
		*next += arg.len;
	}

	ଯଦି (arg.len)
		ଫେରନ୍ତୁ strbuf_detach(&arg, ଖାଲି);

	strbuf_release(&arg);
	ଫେରନ୍ତୁ ଖାଲି;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_next_oid(ଅଚଳ ଅକ୍ଷର **next, ଅଚଳ ଅକ୍ଷର *end,
			  ସଂରଚନା object_id *oid,
			  ଅଚଳ ଅକ୍ଷର *command, ଅଚଳ ଅକ୍ଷର *refname,
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା strbuf arg = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ଯଦି (*next == end)
		ଯାଅ eof;

	ଯଦି (ଧାଡ଼ି_termination) {
		/* Without -z, consume SP and use next argument */
		ଯଦି (!**next || **next == ଧାଡ଼ି_termination)
			ଫେରନ୍ତୁ 1;
		ଯଦି (**next != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *next);
		(*next)++;
		*next = parse_arg(*next, &arg);
		ଯଦି (arg.len) {
			ଯଦି (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				ଯାଅ invalid;
		} ଅନ୍ୟଥା {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} ଅନ୍ୟଥା {
		/* With -z, read the next NUL-terminated line */
		ଯଦି (**next)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *next);
		(*next)++;
		ଯଦି (*next == end)
			ଯାଅ eof;
		strbuf_addstr(&arg, *next);
		*next += arg.len;

		ଯଦି (arg.len) {
			ଯଦି (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				ଯାଅ invalid;
		} ଅନ୍ୟଥା ଯଦି (flags & PARSE_SHA1_ALLOW_EMPTY) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} ଅନ୍ୟଥା {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	ଫେରନ୍ତୁ ret;

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

ସ୍ଥିର ଶୂନ୍ୟ print_rejected_refs(ଅଚଳ ଅକ୍ଷର *refname,
				ଅଚଳ ସଂରଚନା object_id *old_oid,
				ଅଚଳ ସଂରଚନା object_id *new_oid,
				ଅଚଳ ଅକ୍ଷର *old_target,
				ଅଚଳ ଅକ୍ଷର *new_target,
				ଗଣନା ref_transaction_error err,
				ଅଚଳ ଅକ୍ଷର *details,
				ଶୂନ୍ୟ *cb_data UNUSED)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	ଯଦି (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_to_hex(new_oid) : new_target,
		    old_oid ? oid_to_hex(old_oid) : old_target,
		    ref_transaction_error_ସନ୍ଦେଶ(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
ସ୍ଥିର ଶୂନ୍ୟ handle_ref_transaction_error(ଅଚଳ ଅକ୍ଷର *refname,
					 ସଂରଚନା object_id *new_oid,
					 ସଂରଚନା object_id *old_oid,
					 ଅଚଳ ଅକ୍ଷର *new_target,
					 ଅଚଳ ଅକ୍ଷର *old_target,
					 ଗଣନା ref_transaction_error tx_err,
					 ସଂରଚନା strbuf *err,
					 ସଂରଚନା command_options *opts)
{
	ଯଦି (!tx_err)
		ଫେରନ୍ତୁ;

	ଯଦି (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_target,
				    new_target, tx_err, err->buf, ଖାଲି);
		ଫେରନ୍ତୁ;
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

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_update(ସଂରଚନା ref_transaction *transaction,
			     ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end,
			     ସଂରଚନା command_options *opts)
{
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ଅକ୍ଷର *refname;
	ସଂରଚନା object_id new_oid, old_oid;
	ଗଣନା ref_transaction_error tx_err;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା have_old;

	refname = parse_refname(&next);
	ଯଦି (!refname)
		die("update: missing <ref>");

	ଯଦି (parse_next_oid(&next, end, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_EMPTY))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_next_oid(&next, end, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	ଯଦି (*next != ଧାଡ଼ି_termination)
		die("update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : ଖାଲି,
					ଖାଲି, ଖାଲି,
					update_flags | create_reflog_flag,
					ସନ୍ଦେଶ, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : ଖାଲି,
				     ଖାଲି, ଖାଲି, tx_err, &err, opts);


	update_flags = default_flags;
	ମୁକ୍ତ(refname);
	strbuf_release(&err);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_symref_update(ସଂରଚନା ref_transaction *transaction,
				    ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end UNUSED,
				    ସଂରଚନା command_options *opts)
{
	ଅକ୍ଷର *refname, *new_target, *old_arg;
	ଗଣନା ref_transaction_error tx_err;
	ଅକ୍ଷର *old_target = ଖାଲି;
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ସଂରଚନା object_id old_oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା have_old_oid = 0;

	refname = parse_refname(&next);
	ଯଦି (!refname)
		die("symref-update: missing <ref>");

	new_target = parse_next_refname(&next);
	ଯଦି (!new_target)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_next_arg(&next);
	ଯଦି (old_arg) {
		old_target = parse_next_arg(&next);
		ଯଦି (!old_target)
			die("symref-update %s: expected old value", refname);

		ଯଦି (!strcmp(old_arg, "oid")) {
			ଯଦି (repo_get_oid_with_flags(the_repository, old_target, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_target);

			have_old_oid = 1;
		} ଅନ୍ୟଥା ଯଦି (!strcmp(old_arg, "ref")) {
			ଯଦି (check_refname_format(old_target, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_target);
		} ଅନ୍ୟଥା {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	ଯଦି (*next != ଧାଡ଼ି_termination)
		die("symref-update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname, ଖାଲି,
					have_old_oid ? &old_oid : ଖାଲି,
					new_target,
					have_old_oid ? ଖାଲି : old_target,
					update_flags | create_reflog_flag,
					ସନ୍ଦେଶ, &err);
	handle_ref_transaction_error(refname, ଖାଲି, have_old_oid ? &old_oid : ଖାଲି,
				     new_target, have_old_oid ? ଖାଲି : old_target,
				     tx_err, &err, opts);

	update_flags = default_flags;
	ମୁକ୍ତ(refname);
	ମୁକ୍ତ(old_arg);
	ମୁକ୍ତ(old_target);
	ମୁକ୍ତ(new_target);
	strbuf_release(&err);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_create(ସଂରଚନା ref_transaction *transaction,
			     ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end,
			     ସଂରଚନା command_options *opts)
{
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ଅକ୍ଷର *refname;
	ସଂରଚନା object_id new_oid;
	ଗଣନା ref_transaction_error tx_err;

	refname = parse_refname(&next);
	ଯଦି (!refname)
		die("create: missing <ref>");

	ଯଦି (parse_next_oid(&next, end, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	ଯଦି (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	ଯଦି (*next != ଧାଡ଼ି_termination)
		die("create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, ଖାଲି,
					update_flags | create_reflog_flag,
					ସନ୍ଦେଶ, &err);
	handle_ref_transaction_error(refname, &new_oid, ଖାଲି, ଖାଲି, ଖାଲି, tx_err,
				     &err, opts);

	update_flags = default_flags;
	ମୁକ୍ତ(refname);
	strbuf_release(&err);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_symref_create(ସଂରଚନା ref_transaction *transaction,
				    ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end UNUSED,
				    ସଂରଚନା command_options *opts)
{
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ଅକ୍ଷର *refname, *new_target;
	ଗଣନା ref_transaction_error tx_err;

	refname = parse_refname(&next);
	ଯଦି (!refname)
		die("symref-create: missing <ref>");

	new_target = parse_next_refname(&next);
	ଯଦି (!new_target)
		die("symref-create %s: missing <new-target>", refname);

	ଯଦି (*next != ଧାଡ଼ି_termination)
		die("symref-create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, ଖାଲି, new_target,
					update_flags | create_reflog_flag,
					ସନ୍ଦେଶ, &err);
	handle_ref_transaction_error(refname, ଖାଲି, ଖାଲି, new_target, ଖାଲି,
				     tx_err, &err, opts);

	update_flags = default_flags;
	ମୁକ୍ତ(refname);
	ମୁକ୍ତ(new_target);
	strbuf_release(&err);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_delete(ସଂରଚନା ref_transaction *transaction,
			     ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end,
			     ସଂରଚନା command_options *opts UNUSED)
{
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ଅକ୍ଷର *refname;
	ସଂରଚନା object_id old_oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା have_old;

	refname = parse_refname(&next);
	ଯଦି (!refname)
		die("delete: missing <ref>");

	ଯଦି (parse_next_oid(&next, end, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} ଅନ୍ୟଥା {
		ଯଦି (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	ଯଦି (*next != ଧାଡ଼ି_termination)
		die("delete %s: extra input: %s", refname, next);

	ଯଦି (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : ଖାଲି,
				   ଖାଲି, update_flags, ସନ୍ଦେଶ, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ମୁକ୍ତ(refname);
	strbuf_release(&err);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_symref_delete(ସଂରଚନା ref_transaction *transaction,
				    ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end UNUSED,
				    ସଂରଚନା command_options *opts UNUSED)
{
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ଅକ୍ଷର *refname, *old_target;

	ଯଦି (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&next);
	ଯଦି (!refname)
		die("symref-delete: missing <ref>");

	old_target = parse_next_refname(&next);

	ଯଦି (*next != ଧାଡ଼ି_termination)
		die("symref-delete %s: extra input: %s", refname, next);

	ଯଦି (ref_transaction_delete(transaction, refname, ଖାଲି,
				   old_target, update_flags, ସନ୍ଦେଶ, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ମୁକ୍ତ(refname);
	ମୁକ୍ତ(old_target);
	strbuf_release(&err);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_verify(ସଂରଚନା ref_transaction *transaction,
			     ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end,
			     ସଂରଚନା command_options *opts UNUSED)
{
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ଅକ୍ଷର *refname;
	ସଂରଚନା object_id old_oid;

	refname = parse_refname(&next);
	ଯଦି (!refname)
		die("verify: missing <ref>");

	ଯଦି (parse_next_oid(&next, end, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	ଯଦି (*next != ଧାଡ଼ି_termination)
		die("verify %s: extra input: %s", refname, next);

	ଯଦି (ref_transaction_verify(transaction, refname, &old_oid,
				   ଖାଲି, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ମୁକ୍ତ(refname);
	strbuf_release(&err);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_symref_verify(ସଂରଚନା ref_transaction *transaction,
				    ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end UNUSED,
				    ସଂରଚନା command_options *opts UNUSED)
{
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ସଂରଚନା object_id old_oid;
	ଅକ୍ଷର *refname, *old_target;

	ଯଦି (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&next);
	ଯଦି (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_target = parse_next_refname(&next);
	ଯଦି (!old_target)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	ଯଦି (*next != ଧାଡ଼ି_termination)
		die("symref-verify %s: extra input: %s", refname, next);

	ଯଦି (ref_transaction_verify(transaction, refname,
				   old_target ? ଖାଲି : &old_oid,
				   old_target, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ମୁକ୍ତ(refname);
	ମୁକ୍ତ(old_target);
	strbuf_release(&err);
}

ସ୍ଥିର ଶୂନ୍ୟ report_ok(ଅଚଳ ଅକ୍ଷର *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_option(ସଂରଚନା ref_transaction *transaction UNUSED,
			     ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end UNUSED,
			     ସଂରଚନା command_options *opts UNUSED)
{
	ଅଚଳ ଅକ୍ଷର *rest;
	ଯଦି (skip_prefix(next, "no-deref", &rest) && *rest == ଧାଡ଼ି_termination)
		update_flags |= REF_NO_DEREF;
	ଅନ୍ୟଥା
		die("option unknown: %s", next);
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_start(ସଂରଚନା ref_transaction *transaction UNUSED,
			    ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end UNUSED,
			    ସଂରଚନା command_options *opts UNUSED)
{
	ଯଦି (*next != ଧାଡ଼ି_termination)
		die("start: extra input: %s", next);
	report_ok("start");
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_prepare(ସଂରଚନା ref_transaction *transaction,
			      ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end UNUSED,
			      ସଂରଚନା command_options *opts UNUSED)
{
	ସଂରଚନା strbuf error = STRBUF_INIT;
	ଯଦି (*next != ଧାଡ଼ି_termination)
		die("prepare: extra input: %s", next);
	ଯଦି (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_abort(ସଂରଚନା ref_transaction *transaction,
			    ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end UNUSED,
			    ସଂରଚନା command_options *opts UNUSED)
{
	ସଂରଚନା strbuf error = STRBUF_INIT;
	ଯଦି (*next != ଧାଡ଼ି_termination)
		die("abort: extra input: %s", next);
	ଯଦି (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

ସ୍ଥିର ଶୂନ୍ୟ parse_cmd_commit(ସଂରଚନା ref_transaction *transaction,
			     ଅଚଳ ଅକ୍ଷର *next, ଅଚଳ ଅକ୍ଷର *end UNUSED,
			     ସଂରଚନା command_options *opts UNUSED)
{
	ସଂରଚନା strbuf error = STRBUF_INIT;
	ଯଦି (*next != ଧାଡ଼ି_termination)
		die("commit: extra input: %s", next);
	ଯଦି (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ଖାଲି);

	report_ok("commit");
	ref_transaction_free(transaction);
}

ଗଣନା update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

ସ୍ଥିର ଅଚଳ ସଂରଚନା parse_cmd {
	ଅଚଳ ଅକ୍ଷର *prefix;
	ଶୂନ୍ୟ (*fn)(ସଂରଚନା ref_transaction *, ଅଚଳ ଅକ୍ଷର *, ଅଚଳ ଅକ୍ଷର *,
		   ସଂରଚନା command_options *);
	unsigned args;
	ଗଣନା update_refs_state state;
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

ସ୍ଥିର ଶୂନ୍ୟ update_refs_stdin(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା strbuf ନିବେଶ = STRBUF_INIT, err = STRBUF_INIT;
	ଗଣନା update_refs_state state = UPDATE_REFS_OPEN;
	ସଂରଚନା ref_transaction *transaction;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;

	ସଂରଚନା command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	ଯଦି (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	ଯେତେବେଳେ (!strbuf_getwholeline(&ନିବେଶ, stdin, ଧାଡ଼ି_termination)) {
		ଅଚଳ ସଂରଚନା parse_cmd *cmd = ଖାଲି;

		ଯଦି (*ନିବେଶ.buf == ଧାଡ଼ି_termination)
			die("empty command in input");
		ଅନ୍ୟଥା ଯଦି (isspace(*ନିବେଶ.buf))
			die("whitespace before command: %s", ନିବେଶ.buf);

		ପାଇଁ (i = 0; i < ARRAY_SIZE(command); i++) {
			ଅଚଳ ଅକ୍ଷର *prefix = command[i].prefix;
			ଅକ୍ଷର c;

			ଯଦି (!starts_with(ନିବେଶ.buf, prefix))
				ଜାରି;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].args ? ' ' : ଧାଡ଼ି_termination;
			ଯଦି (ନିବେଶ.buf[strlen(prefix)] != c)
				ଜାରି;

			cmd = &command[i];
			ଭାଙ୍ଗ;
		}
		ଯଦି (!cmd)
			die("unknown command: %s", ନିବେଶ.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		ପାଇଁ (j = 1; ଧାଡ଼ି_termination == '\0' && j < cmd->args; j++)
			ଯଦି (strbuf_appendwholeline(&ନିବେଶ, stdin, ଧାଡ଼ି_termination))
				ଭାଙ୍ଗ;

		ବାଛ (state) {
		ସ୍ଥିତି UPDATE_REFS_OPEN:
		ସ୍ଥିତି UPDATE_REFS_STARTED:
			ଯଦି (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			ଯଦି (cmd->state >= state)
				state = cmd->state;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି UPDATE_REFS_PREPARED:
			ଯଦି (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି UPDATE_REFS_CLOSED:
			ଯଦି (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			ଯଦି (!transaction)
				die("%s", err.buf);

			ଭାଙ୍ଗ;
		}

		cmd->fn(transaction, ନିବେଶ.buf + strlen(cmd->prefix) + !!cmd->args,
			ନିବେଶ.buf + ନିବେଶ.len, &opts);
	}

	ବାଛ (state) {
	ସ୍ଥିତି UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		ଯଦି (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ଖାଲି);
		ref_transaction_free(transaction);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି UPDATE_REFS_STARTED:
	ସ୍ଥିତି UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		ଯଦି (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		ଭାଙ୍ଗ;
	}

	strbuf_release(&err);
	strbuf_release(&ନିବେଶ);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_update_ref(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		   ଅଚଳ ଅକ୍ଷର **argv,
		   ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo UNUSED)
{
	ଅଚଳ ଅକ୍ଷର *refname, *oldval;
	ସଂରଚନା object_id oid, oldoid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା delete = 0, no_deref = 0, read_stdin = 0, end_null = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା create_reflog = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;

	ସଂରଚନା option options[] = {
		OPT_STRING( 'm', ଖାଲି, &ସନ୍ଦେଶ, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', ଖାଲି, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', ଖାଲି, &end_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_BIT('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ଖାଲି);
	argc = parse_options(argc, argv, prefix, options, git_update_ref_usage,
			     0);
	ଯଦି (ସନ୍ଦେଶ && !*ସନ୍ଦେଶ)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	ଯଦି (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	ଯଦି (read_stdin) {
		ଯଦି (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		ଯଦି (end_null)
			ଧାଡ଼ି_termination = '\0';
		update_refs_stdin(flags);
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	ଯଦି (end_null)
		usage_with_options(git_update_ref_usage, options);

	ଯଦି (delete) {
		ଯଦି (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} ଅନ୍ୟଥା {
		ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ;
		ଯଦି (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		ମୂଲ୍ୟ = argv[1];
		oldval = argv[2];
		ଯଦି (repo_get_oid_with_flags(the_repository, ମୂଲ୍ୟ, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", ମୂଲ୍ୟ);
	}

	ଯଦି (oldval) {
		ଯଦି (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		ଅନ୍ୟଥା ଯଦି (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	ଯଦି (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		ଫେରନ୍ତୁ refs_delete_ref(get_main_ref_store(the_repository),
				       ସନ୍ଦେଶ, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : ଖାଲି,
				       default_flags);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ refs_update_ref(get_main_ref_store(the_repository),
				       ସନ୍ଦେଶ, refname, &oid,
				       oldval ? &oldoid : ଖାଲି,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
