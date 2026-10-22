#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "quote.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	ശൂന്യ
};

സ്ഥിതം അക്ഷരം വരി_termination = '\n';
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ update_flags;
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ default_flags;
സ്ഥിതം unsigned create_reflog_flag;
സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം;

ഘടന command_options {
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
സ്ഥിതം സ്ഥിരം അക്ഷരം *parse_arg(സ്ഥിരം അക്ഷരം *next, ഘടന strbuf *arg)
{
	എങ്കിൽ (*next == '"') {
		സ്ഥിരം അക്ഷരം *orig = next;

		എങ്കിൽ (unquote_c_style(arg, next, &next))
			die("badly quoted argument: %s", orig);
		എങ്കിൽ (*next && !isspace(*next))
			die("unexpected character after quoted argument: %s", orig);
	} അല്ലെങ്കിൽ {
		ആകുമ്പോൾ (*next && !isspace(*next))
			strbuf_addch(arg, *next++);
	}

	തിരികെ next;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
സ്ഥിതം അക്ഷരം *parse_refname(സ്ഥിരം അക്ഷരം **next)
{
	ഘടന strbuf ref = STRBUF_INIT;

	എങ്കിൽ (വരി_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &ref);
	} അല്ലെങ്കിൽ {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *next);
		*next += ref.len;
	}

	എങ്കിൽ (!ref.len) {
		strbuf_release(&ref);
		തിരികെ ശൂന്യ;
	}

	എങ്കിൽ (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	തിരികെ strbuf_detach(&ref, ശൂന്യ);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
സ്ഥിതം അക്ഷരം *parse_next_refname(സ്ഥിരം അക്ഷരം **next)
{
	എങ്കിൽ (വരി_termination) {
		/* Without -z, consume SP and use next argument */
		എങ്കിൽ (!**next || **next == വരി_termination)
			തിരികെ ശൂന്യ;
		എങ്കിൽ (**next != ' ')
			die("expected SP but got: %s", *next);
	} അല്ലെങ്കിൽ {
		/* With -z, read the next NUL-terminated line */
		എങ്കിൽ (**next)
			തിരികെ ശൂന്യ;
	}
	/* Skip the delimiter */
	(*next)++;

	തിരികെ parse_refname(next);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
സ്ഥിതം അക്ഷരം *parse_next_arg(സ്ഥിരം അക്ഷരം **next)
{
	ഘടന strbuf arg = STRBUF_INIT;

	എങ്കിൽ (വരി_termination) {
		/* Without -z, consume SP and use next argument */
		എങ്കിൽ (!**next || **next == വരി_termination)
			തിരികെ ശൂന്യ;
		എങ്കിൽ (**next != ' ')
			die("expected SP but got: %s", *next);
	} അല്ലെങ്കിൽ {
		/* With -z, read the next NUL-terminated line */
		എങ്കിൽ (**next)
			തിരികെ ശൂന്യ;
	}
	/* Skip the delimiter */
	(*next)++;

	എങ്കിൽ (വരി_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &arg);
	} അല്ലെങ്കിൽ {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *next);
		*next += arg.len;
	}

	എങ്കിൽ (arg.len)
		തിരികെ strbuf_detach(&arg, ശൂന്യ);

	strbuf_release(&arg);
	തിരികെ ശൂന്യ;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ parse_next_oid(സ്ഥിരം അക്ഷരം **next, സ്ഥിരം അക്ഷരം *end,
			  ഘടന object_id *oid,
			  സ്ഥിരം അക്ഷരം *command, സ്ഥിരം അക്ഷരം *refname,
			  പൂർണ്ണസംഖ്യ flags)
{
	ഘടന strbuf arg = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret = 0;

	എങ്കിൽ (*next == end)
		പോകുക eof;

	എങ്കിൽ (വരി_termination) {
		/* Without -z, consume SP and use next argument */
		എങ്കിൽ (!**next || **next == വരി_termination)
			തിരികെ 1;
		എങ്കിൽ (**next != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *next);
		(*next)++;
		*next = parse_arg(*next, &arg);
		എങ്കിൽ (arg.len) {
			എങ്കിൽ (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				പോകുക invalid;
		} അല്ലെങ്കിൽ {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} അല്ലെങ്കിൽ {
		/* With -z, read the next NUL-terminated line */
		എങ്കിൽ (**next)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *next);
		(*next)++;
		എങ്കിൽ (*next == end)
			പോകുക eof;
		strbuf_addstr(&arg, *next);
		*next += arg.len;

		എങ്കിൽ (arg.len) {
			എങ്കിൽ (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				പോകുക invalid;
		} അല്ലെങ്കിൽ എങ്കിൽ (flags & PARSE_SHA1_ALLOW_EMPTY) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} അല്ലെങ്കിൽ {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	തിരികെ ret;

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

സ്ഥിതം ശൂന്യം print_rejected_refs(സ്ഥിരം അക്ഷരം *refname,
				സ്ഥിരം ഘടന object_id *old_oid,
				സ്ഥിരം ഘടന object_id *new_oid,
				സ്ഥിരം അക്ഷരം *old_target,
				സ്ഥിരം അക്ഷരം *new_target,
				എണ്ണൽ ref_transaction_error err,
				സ്ഥിരം അക്ഷരം *details,
				ശൂന്യം *cb_data UNUSED)
{
	ഘടന strbuf sb = STRBUF_INIT;

	എങ്കിൽ (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_to_hex(new_oid) : new_target,
		    old_oid ? oid_to_hex(old_oid) : old_target,
		    ref_transaction_error_സന്ദേശം(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
സ്ഥിതം ശൂന്യം handle_ref_transaction_error(സ്ഥിരം അക്ഷരം *refname,
					 ഘടന object_id *new_oid,
					 ഘടന object_id *old_oid,
					 സ്ഥിരം അക്ഷരം *new_target,
					 സ്ഥിരം അക്ഷരം *old_target,
					 എണ്ണൽ ref_transaction_error tx_err,
					 ഘടന strbuf *err,
					 ഘടന command_options *opts)
{
	എങ്കിൽ (!tx_err)
		തിരികെ;

	എങ്കിൽ (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_target,
				    new_target, tx_err, err->buf, ശൂന്യ);
		തിരികെ;
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

സ്ഥിതം ശൂന്യം parse_cmd_update(ഘടന ref_transaction *transaction,
			     സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end,
			     ഘടന command_options *opts)
{
	ഘടന strbuf err = STRBUF_INIT;
	അക്ഷരം *refname;
	ഘടന object_id new_oid, old_oid;
	എണ്ണൽ ref_transaction_error tx_err;
	പൂർണ്ണസംഖ്യ have_old;

	refname = parse_refname(&next);
	എങ്കിൽ (!refname)
		die("update: missing <ref>");

	എങ്കിൽ (parse_next_oid(&next, end, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_EMPTY))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_next_oid(&next, end, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	എങ്കിൽ (*next != വരി_termination)
		die("update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : ശൂന്യ,
					ശൂന്യ, ശൂന്യ,
					update_flags | create_reflog_flag,
					സന്ദേശം, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : ശൂന്യ,
				     ശൂന്യ, ശൂന്യ, tx_err, &err, opts);


	update_flags = default_flags;
	സ്വതന്ത്രം(refname);
	strbuf_release(&err);
}

സ്ഥിതം ശൂന്യം parse_cmd_symref_update(ഘടന ref_transaction *transaction,
				    സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end UNUSED,
				    ഘടന command_options *opts)
{
	അക്ഷരം *refname, *new_target, *old_arg;
	എണ്ണൽ ref_transaction_error tx_err;
	അക്ഷരം *old_target = ശൂന്യ;
	ഘടന strbuf err = STRBUF_INIT;
	ഘടന object_id old_oid;
	പൂർണ്ണസംഖ്യ have_old_oid = 0;

	refname = parse_refname(&next);
	എങ്കിൽ (!refname)
		die("symref-update: missing <ref>");

	new_target = parse_next_refname(&next);
	എങ്കിൽ (!new_target)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_next_arg(&next);
	എങ്കിൽ (old_arg) {
		old_target = parse_next_arg(&next);
		എങ്കിൽ (!old_target)
			die("symref-update %s: expected old value", refname);

		എങ്കിൽ (!strcmp(old_arg, "oid")) {
			എങ്കിൽ (repo_get_oid_with_flags(the_repository, old_target, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_target);

			have_old_oid = 1;
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(old_arg, "ref")) {
			എങ്കിൽ (check_refname_format(old_target, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_target);
		} അല്ലെങ്കിൽ {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	എങ്കിൽ (*next != വരി_termination)
		die("symref-update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname, ശൂന്യ,
					have_old_oid ? &old_oid : ശൂന്യ,
					new_target,
					have_old_oid ? ശൂന്യ : old_target,
					update_flags | create_reflog_flag,
					സന്ദേശം, &err);
	handle_ref_transaction_error(refname, ശൂന്യ, have_old_oid ? &old_oid : ശൂന്യ,
				     new_target, have_old_oid ? ശൂന്യ : old_target,
				     tx_err, &err, opts);

	update_flags = default_flags;
	സ്വതന്ത്രം(refname);
	സ്വതന്ത്രം(old_arg);
	സ്വതന്ത്രം(old_target);
	സ്വതന്ത്രം(new_target);
	strbuf_release(&err);
}

സ്ഥിതം ശൂന്യം parse_cmd_create(ഘടന ref_transaction *transaction,
			     സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end,
			     ഘടന command_options *opts)
{
	ഘടന strbuf err = STRBUF_INIT;
	അക്ഷരം *refname;
	ഘടന object_id new_oid;
	എണ്ണൽ ref_transaction_error tx_err;

	refname = parse_refname(&next);
	എങ്കിൽ (!refname)
		die("create: missing <ref>");

	എങ്കിൽ (parse_next_oid(&next, end, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	എങ്കിൽ (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	എങ്കിൽ (*next != വരി_termination)
		die("create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, ശൂന്യ,
					update_flags | create_reflog_flag,
					സന്ദേശം, &err);
	handle_ref_transaction_error(refname, &new_oid, ശൂന്യ, ശൂന്യ, ശൂന്യ, tx_err,
				     &err, opts);

	update_flags = default_flags;
	സ്വതന്ത്രം(refname);
	strbuf_release(&err);
}

സ്ഥിതം ശൂന്യം parse_cmd_symref_create(ഘടന ref_transaction *transaction,
				    സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end UNUSED,
				    ഘടന command_options *opts)
{
	ഘടന strbuf err = STRBUF_INIT;
	അക്ഷരം *refname, *new_target;
	എണ്ണൽ ref_transaction_error tx_err;

	refname = parse_refname(&next);
	എങ്കിൽ (!refname)
		die("symref-create: missing <ref>");

	new_target = parse_next_refname(&next);
	എങ്കിൽ (!new_target)
		die("symref-create %s: missing <new-target>", refname);

	എങ്കിൽ (*next != വരി_termination)
		die("symref-create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, ശൂന്യ, new_target,
					update_flags | create_reflog_flag,
					സന്ദേശം, &err);
	handle_ref_transaction_error(refname, ശൂന്യ, ശൂന്യ, new_target, ശൂന്യ,
				     tx_err, &err, opts);

	update_flags = default_flags;
	സ്വതന്ത്രം(refname);
	സ്വതന്ത്രം(new_target);
	strbuf_release(&err);
}

സ്ഥിതം ശൂന്യം parse_cmd_delete(ഘടന ref_transaction *transaction,
			     സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end,
			     ഘടന command_options *opts UNUSED)
{
	ഘടന strbuf err = STRBUF_INIT;
	അക്ഷരം *refname;
	ഘടന object_id old_oid;
	പൂർണ്ണസംഖ്യ have_old;

	refname = parse_refname(&next);
	എങ്കിൽ (!refname)
		die("delete: missing <ref>");

	എങ്കിൽ (parse_next_oid(&next, end, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	എങ്കിൽ (*next != വരി_termination)
		die("delete %s: extra input: %s", refname, next);

	എങ്കിൽ (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : ശൂന്യ,
				   ശൂന്യ, update_flags, സന്ദേശം, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	സ്വതന്ത്രം(refname);
	strbuf_release(&err);
}

സ്ഥിതം ശൂന്യം parse_cmd_symref_delete(ഘടന ref_transaction *transaction,
				    സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end UNUSED,
				    ഘടന command_options *opts UNUSED)
{
	ഘടന strbuf err = STRBUF_INIT;
	അക്ഷരം *refname, *old_target;

	എങ്കിൽ (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&next);
	എങ്കിൽ (!refname)
		die("symref-delete: missing <ref>");

	old_target = parse_next_refname(&next);

	എങ്കിൽ (*next != വരി_termination)
		die("symref-delete %s: extra input: %s", refname, next);

	എങ്കിൽ (ref_transaction_delete(transaction, refname, ശൂന്യ,
				   old_target, update_flags, സന്ദേശം, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	സ്വതന്ത്രം(refname);
	സ്വതന്ത്രം(old_target);
	strbuf_release(&err);
}

സ്ഥിതം ശൂന്യം parse_cmd_verify(ഘടന ref_transaction *transaction,
			     സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end,
			     ഘടന command_options *opts UNUSED)
{
	ഘടന strbuf err = STRBUF_INIT;
	അക്ഷരം *refname;
	ഘടന object_id old_oid;

	refname = parse_refname(&next);
	എങ്കിൽ (!refname)
		die("verify: missing <ref>");

	എങ്കിൽ (parse_next_oid(&next, end, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	എങ്കിൽ (*next != വരി_termination)
		die("verify %s: extra input: %s", refname, next);

	എങ്കിൽ (ref_transaction_verify(transaction, refname, &old_oid,
				   ശൂന്യ, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	സ്വതന്ത്രം(refname);
	strbuf_release(&err);
}

സ്ഥിതം ശൂന്യം parse_cmd_symref_verify(ഘടന ref_transaction *transaction,
				    സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end UNUSED,
				    ഘടന command_options *opts UNUSED)
{
	ഘടന strbuf err = STRBUF_INIT;
	ഘടന object_id old_oid;
	അക്ഷരം *refname, *old_target;

	എങ്കിൽ (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&next);
	എങ്കിൽ (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_target = parse_next_refname(&next);
	എങ്കിൽ (!old_target)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	എങ്കിൽ (*next != വരി_termination)
		die("symref-verify %s: extra input: %s", refname, next);

	എങ്കിൽ (ref_transaction_verify(transaction, refname,
				   old_target ? ശൂന്യ : &old_oid,
				   old_target, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	സ്വതന്ത്രം(refname);
	സ്വതന്ത്രം(old_target);
	strbuf_release(&err);
}

സ്ഥിതം ശൂന്യം report_ok(സ്ഥിരം അക്ഷരം *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

സ്ഥിതം ശൂന്യം parse_cmd_option(ഘടന ref_transaction *transaction UNUSED,
			     സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end UNUSED,
			     ഘടന command_options *opts UNUSED)
{
	സ്ഥിരം അക്ഷരം *rest;
	എങ്കിൽ (skip_prefix(next, "no-deref", &rest) && *rest == വരി_termination)
		update_flags |= REF_NO_DEREF;
	അല്ലെങ്കിൽ
		die("option unknown: %s", next);
}

സ്ഥിതം ശൂന്യം parse_cmd_start(ഘടന ref_transaction *transaction UNUSED,
			    സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end UNUSED,
			    ഘടന command_options *opts UNUSED)
{
	എങ്കിൽ (*next != വരി_termination)
		die("start: extra input: %s", next);
	report_ok("start");
}

സ്ഥിതം ശൂന്യം parse_cmd_prepare(ഘടന ref_transaction *transaction,
			      സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end UNUSED,
			      ഘടന command_options *opts UNUSED)
{
	ഘടന strbuf error = STRBUF_INIT;
	എങ്കിൽ (*next != വരി_termination)
		die("prepare: extra input: %s", next);
	എങ്കിൽ (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

സ്ഥിതം ശൂന്യം parse_cmd_abort(ഘടന ref_transaction *transaction,
			    സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end UNUSED,
			    ഘടന command_options *opts UNUSED)
{
	ഘടന strbuf error = STRBUF_INIT;
	എങ്കിൽ (*next != വരി_termination)
		die("abort: extra input: %s", next);
	എങ്കിൽ (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

സ്ഥിതം ശൂന്യം parse_cmd_commit(ഘടന ref_transaction *transaction,
			     സ്ഥിരം അക്ഷരം *next, സ്ഥിരം അക്ഷരം *end UNUSED,
			     ഘടന command_options *opts UNUSED)
{
	ഘടന strbuf error = STRBUF_INIT;
	എങ്കിൽ (*next != വരി_termination)
		die("commit: extra input: %s", next);
	എങ്കിൽ (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ശൂന്യ);

	report_ok("commit");
	ref_transaction_free(transaction);
}

എണ്ണൽ update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

സ്ഥിതം സ്ഥിരം ഘടന parse_cmd {
	സ്ഥിരം അക്ഷരം *prefix;
	ശൂന്യം (*fn)(ഘടന ref_transaction *, സ്ഥിരം അക്ഷരം *, സ്ഥിരം അക്ഷരം *,
		   ഘടന command_options *);
	unsigned args;
	എണ്ണൽ update_refs_state state;
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

സ്ഥിതം ശൂന്യം update_refs_stdin(unsigned പൂർണ്ണസംഖ്യ flags)
{
	ഘടന strbuf നിവേശം = STRBUF_INIT, err = STRBUF_INIT;
	എണ്ണൽ update_refs_state state = UPDATE_REFS_OPEN;
	ഘടന ref_transaction *transaction;
	പൂർണ്ണസംഖ്യ i, j;

	ഘടന command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	എങ്കിൽ (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	ആകുമ്പോൾ (!strbuf_getwholeline(&നിവേശം, stdin, വരി_termination)) {
		സ്ഥിരം ഘടന parse_cmd *cmd = ശൂന്യ;

		എങ്കിൽ (*നിവേശം.buf == വരി_termination)
			die("empty command in input");
		അല്ലെങ്കിൽ എങ്കിൽ (isspace(*നിവേശം.buf))
			die("whitespace before command: %s", നിവേശം.buf);

		വേണ്ടി (i = 0; i < ARRAY_SIZE(command); i++) {
			സ്ഥിരം അക്ഷരം *prefix = command[i].prefix;
			അക്ഷരം c;

			എങ്കിൽ (!starts_with(നിവേശം.buf, prefix))
				തുടരുക;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].args ? ' ' : വരി_termination;
			എങ്കിൽ (നിവേശം.buf[strlen(prefix)] != c)
				തുടരുക;

			cmd = &command[i];
			നിർത്തുക;
		}
		എങ്കിൽ (!cmd)
			die("unknown command: %s", നിവേശം.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		വേണ്ടി (j = 1; വരി_termination == '\0' && j < cmd->args; j++)
			എങ്കിൽ (strbuf_appendwholeline(&നിവേശം, stdin, വരി_termination))
				നിർത്തുക;

		തിരഞ്ഞെടുപ്പ് (state) {
		അവസ്ഥ UPDATE_REFS_OPEN:
		അവസ്ഥ UPDATE_REFS_STARTED:
			എങ്കിൽ (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			എങ്കിൽ (cmd->state >= state)
				state = cmd->state;
			നിർത്തുക;
		അവസ്ഥ UPDATE_REFS_PREPARED:
			എങ്കിൽ (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			നിർത്തുക;
		അവസ്ഥ UPDATE_REFS_CLOSED:
			എങ്കിൽ (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			എങ്കിൽ (!transaction)
				die("%s", err.buf);

			നിർത്തുക;
		}

		cmd->fn(transaction, നിവേശം.buf + strlen(cmd->prefix) + !!cmd->args,
			നിവേശം.buf + നിവേശം.len, &opts);
	}

	തിരഞ്ഞെടുപ്പ് (state) {
	അവസ്ഥ UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		എങ്കിൽ (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ശൂന്യ);
		ref_transaction_free(transaction);
		നിർത്തുക;
	അവസ്ഥ UPDATE_REFS_STARTED:
	അവസ്ഥ UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		എങ്കിൽ (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		നിർത്തുക;
	അവസ്ഥ UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		നിർത്തുക;
	}

	strbuf_release(&err);
	strbuf_release(&നിവേശം);
}

പൂർണ്ണസംഖ്യ cmd_update_ref(പൂർണ്ണസംഖ്യ argc,
		   സ്ഥിരം അക്ഷരം **argv,
		   സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo UNUSED)
{
	സ്ഥിരം അക്ഷരം *refname, *oldval;
	ഘടന object_id oid, oldoid;
	പൂർണ്ണസംഖ്യ delete = 0, no_deref = 0, read_stdin = 0, end_null = 0;
	പൂർണ്ണസംഖ്യ create_reflog = 0;
	unsigned പൂർണ്ണസംഖ്യ flags = 0;

	ഘടന option options[] = {
		OPT_STRING( 'm', ശൂന്യ, &സന്ദേശം, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', ശൂന്യ, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', ശൂന്യ, &end_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_BIT('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ശൂന്യ);
	argc = parse_options(argc, argv, prefix, options, git_update_ref_usage,
			     0);
	എങ്കിൽ (സന്ദേശം && !*സന്ദേശം)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	എങ്കിൽ (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	എങ്കിൽ (read_stdin) {
		എങ്കിൽ (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		എങ്കിൽ (end_null)
			വരി_termination = '\0';
		update_refs_stdin(flags);
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	എങ്കിൽ (end_null)
		usage_with_options(git_update_ref_usage, options);

	എങ്കിൽ (delete) {
		എങ്കിൽ (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} അല്ലെങ്കിൽ {
		സ്ഥിരം അക്ഷരം *മൂല്യം;
		എങ്കിൽ (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		മൂല്യം = argv[1];
		oldval = argv[2];
		എങ്കിൽ (repo_get_oid_with_flags(the_repository, മൂല്യം, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", മൂല്യം);
	}

	എങ്കിൽ (oldval) {
		എങ്കിൽ (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		അല്ലെങ്കിൽ എങ്കിൽ (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	എങ്കിൽ (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		തിരികെ refs_delete_ref(get_main_ref_store(the_repository),
				       സന്ദേശം, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : ശൂന്യ,
				       default_flags);
	അല്ലെങ്കിൽ
		തിരികെ refs_update_ref(get_main_ref_store(the_repository),
				       സന്ദേശം, refname, &oid,
				       oldval ? &oldoid : ശൂന്യ,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
