#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "quote.h"

स्थिर ध्रुव वर्ण * ध्रुव git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	रिक्त
};

स्थिर वर्ण पंक्ति_termination = '\n';
स्थिर अचिन्हित पूर्णाङ्क update_flags;
स्थिर अचिन्हित पूर्णाङ्क default_flags;
स्थिर अचिन्हित create_reflog_flag;
स्थिर ध्रुव वर्ण *संदेश;

संरचना command_options {
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
स्थिर ध्रुव वर्ण *parse_arg(ध्रुव वर्ण *next, संरचना strbuf *arg)
{
	यदि (*next == '"') {
		ध्रुव वर्ण *orig = next;

		यदि (unquote_c_style(arg, next, &next))
			die("badly quoted argument: %s", orig);
		यदि (*next && !है_खाली(*next))
			die("unexpected character after quoted argument: %s", orig);
	} अन्यथा {
		यावत् (*next && !है_खाली(*next))
			strbuf_addch(arg, *next++);
	}

	प्रत्यागम next;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
स्थिर वर्ण *parse_refname(ध्रुव वर्ण **next)
{
	संरचना strbuf ref = STRBUF_INIT;

	यदि (पंक्ति_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &ref);
	} अन्यथा {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *next);
		*next += ref.len;
	}

	यदि (!ref.len) {
		strbuf_release(&ref);
		प्रत्यागम रिक्त;
	}

	यदि (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	प्रत्यागम strbuf_detach(&ref, रिक्त);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
स्थिर वर्ण *parse_next_refname(ध्रुव वर्ण **next)
{
	यदि (पंक्ति_termination) {
		/* Without -z, consume SP and use next argument */
		यदि (!**next || **next == पंक्ति_termination)
			प्रत्यागम रिक्त;
		यदि (**next != ' ')
			die("expected SP but got: %s", *next);
	} अन्यथा {
		/* With -z, read the next NUL-terminated line */
		यदि (**next)
			प्रत्यागम रिक्त;
	}
	/* Skip the delimiter */
	(*next)++;

	प्रत्यागम parse_refname(next);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
स्थिर वर्ण *parse_next_arg(ध्रुव वर्ण **next)
{
	संरचना strbuf arg = STRBUF_INIT;

	यदि (पंक्ति_termination) {
		/* Without -z, consume SP and use next argument */
		यदि (!**next || **next == पंक्ति_termination)
			प्रत्यागम रिक्त;
		यदि (**next != ' ')
			die("expected SP but got: %s", *next);
	} अन्यथा {
		/* With -z, read the next NUL-terminated line */
		यदि (**next)
			प्रत्यागम रिक्त;
	}
	/* Skip the delimiter */
	(*next)++;

	यदि (पंक्ति_termination) {
		/* Without -z, use the next argument */
		*next = parse_arg(*next, &arg);
	} अन्यथा {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *next);
		*next += arg.len;
	}

	यदि (arg.len)
		प्रत्यागम strbuf_detach(&arg, रिक्त);

	strbuf_release(&arg);
	प्रत्यागम रिक्त;
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
स्थिर पूर्णाङ्क parse_next_oid(ध्रुव वर्ण **next, ध्रुव वर्ण *end,
			  संरचना object_id *oid,
			  ध्रुव वर्ण *command, ध्रुव वर्ण *refname,
			  पूर्णाङ्क flags)
{
	संरचना strbuf arg = STRBUF_INIT;
	पूर्णाङ्क ret = 0;

	यदि (*next == end)
		गच्छ eof;

	यदि (पंक्ति_termination) {
		/* Without -z, consume SP and use next argument */
		यदि (!**next || **next == पंक्ति_termination)
			प्रत्यागम 1;
		यदि (**next != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *next);
		(*next)++;
		*next = parse_arg(*next, &arg);
		यदि (arg.len) {
			यदि (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				गच्छ invalid;
		} अन्यथा {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} अन्यथा {
		/* With -z, read the next NUL-terminated line */
		यदि (**next)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *next);
		(*next)++;
		यदि (*next == end)
			गच्छ eof;
		strbuf_addstr(&arg, *next);
		*next += arg.len;

		यदि (arg.len) {
			यदि (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				गच्छ invalid;
		} अन्यथा यदि (flags & PARSE_SHA1_ALLOW_EMPTY) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} अन्यथा {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	प्रत्यागम ret;

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

स्थिर शून्य print_rejected_refs(ध्रुव वर्ण *refname,
				ध्रुव संरचना object_id *old_oid,
				ध्रुव संरचना object_id *new_oid,
				ध्रुव वर्ण *old_target,
				ध्रुव वर्ण *new_target,
				गणना ref_transaction_error err,
				ध्रुव वर्ण *details,
				शून्य *cb_data UNUSED)
{
	संरचना strbuf sb = STRBUF_INIT;

	यदि (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_to_hex(new_oid) : new_target,
		    old_oid ? oid_to_hex(old_oid) : old_target,
		    ref_transaction_error_संदेश(err));

	ख_डालो(sb.buf, sb.len, 1, मानक_निकास);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
स्थिर शून्य handle_ref_transaction_error(ध्रुव वर्ण *refname,
					 संरचना object_id *new_oid,
					 संरचना object_id *old_oid,
					 ध्रुव वर्ण *new_target,
					 ध्रुव वर्ण *old_target,
					 गणना ref_transaction_error tx_err,
					 संरचना strbuf *err,
					 संरचना command_options *opts)
{
	यदि (!tx_err)
		प्रत्यागम;

	यदि (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_target,
				    new_target, tx_err, err->buf, रिक्त);
		प्रत्यागम;
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

स्थिर शून्य parse_cmd_update(संरचना ref_transaction *transaction,
			     ध्रुव वर्ण *next, ध्रुव वर्ण *end,
			     संरचना command_options *opts)
{
	संरचना strbuf err = STRBUF_INIT;
	वर्ण *refname;
	संरचना object_id new_oid, old_oid;
	गणना ref_transaction_error tx_err;
	पूर्णाङ्क have_old;

	refname = parse_refname(&next);
	यदि (!refname)
		die("update: missing <ref>");

	यदि (parse_next_oid(&next, end, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_EMPTY))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_next_oid(&next, end, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	यदि (*next != पंक्ति_termination)
		die("update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : रिक्त,
					रिक्त, रिक्त,
					update_flags | create_reflog_flag,
					संदेश, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : रिक्त,
				     रिक्त, रिक्त, tx_err, &err, opts);


	update_flags = default_flags;
	मुक्त(refname);
	strbuf_release(&err);
}

स्थिर शून्य parse_cmd_symref_update(संरचना ref_transaction *transaction,
				    ध्रुव वर्ण *next, ध्रुव वर्ण *end UNUSED,
				    संरचना command_options *opts)
{
	वर्ण *refname, *new_target, *old_arg;
	गणना ref_transaction_error tx_err;
	वर्ण *old_target = रिक्त;
	संरचना strbuf err = STRBUF_INIT;
	संरचना object_id old_oid;
	पूर्णाङ्क have_old_oid = 0;

	refname = parse_refname(&next);
	यदि (!refname)
		die("symref-update: missing <ref>");

	new_target = parse_next_refname(&next);
	यदि (!new_target)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_next_arg(&next);
	यदि (old_arg) {
		old_target = parse_next_arg(&next);
		यदि (!old_target)
			die("symref-update %s: expected old value", refname);

		यदि (!म_भेद(old_arg, "oid")) {
			यदि (repo_get_oid_with_flags(the_repository, old_target, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_target);

			have_old_oid = 1;
		} अन्यथा यदि (!म_भेद(old_arg, "ref")) {
			यदि (check_refname_format(old_target, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_target);
		} अन्यथा {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	यदि (*next != पंक्ति_termination)
		die("symref-update %s: extra input: %s", refname, next);

	tx_err = ref_transaction_update(transaction, refname, रिक्त,
					have_old_oid ? &old_oid : रिक्त,
					new_target,
					have_old_oid ? रिक्त : old_target,
					update_flags | create_reflog_flag,
					संदेश, &err);
	handle_ref_transaction_error(refname, रिक्त, have_old_oid ? &old_oid : रिक्त,
				     new_target, have_old_oid ? रिक्त : old_target,
				     tx_err, &err, opts);

	update_flags = default_flags;
	मुक्त(refname);
	मुक्त(old_arg);
	मुक्त(old_target);
	मुक्त(new_target);
	strbuf_release(&err);
}

स्थिर शून्य parse_cmd_create(संरचना ref_transaction *transaction,
			     ध्रुव वर्ण *next, ध्रुव वर्ण *end,
			     संरचना command_options *opts)
{
	संरचना strbuf err = STRBUF_INIT;
	वर्ण *refname;
	संरचना object_id new_oid;
	गणना ref_transaction_error tx_err;

	refname = parse_refname(&next);
	यदि (!refname)
		die("create: missing <ref>");

	यदि (parse_next_oid(&next, end, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	यदि (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	यदि (*next != पंक्ति_termination)
		die("create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, रिक्त,
					update_flags | create_reflog_flag,
					संदेश, &err);
	handle_ref_transaction_error(refname, &new_oid, रिक्त, रिक्त, रिक्त, tx_err,
				     &err, opts);

	update_flags = default_flags;
	मुक्त(refname);
	strbuf_release(&err);
}

स्थिर शून्य parse_cmd_symref_create(संरचना ref_transaction *transaction,
				    ध्रुव वर्ण *next, ध्रुव वर्ण *end UNUSED,
				    संरचना command_options *opts)
{
	संरचना strbuf err = STRBUF_INIT;
	वर्ण *refname, *new_target;
	गणना ref_transaction_error tx_err;

	refname = parse_refname(&next);
	यदि (!refname)
		die("symref-create: missing <ref>");

	new_target = parse_next_refname(&next);
	यदि (!new_target)
		die("symref-create %s: missing <new-target>", refname);

	यदि (*next != पंक्ति_termination)
		die("symref-create %s: extra input: %s", refname, next);

	tx_err = ref_transaction_create(transaction, refname, रिक्त, new_target,
					update_flags | create_reflog_flag,
					संदेश, &err);
	handle_ref_transaction_error(refname, रिक्त, रिक्त, new_target, रिक्त,
				     tx_err, &err, opts);

	update_flags = default_flags;
	मुक्त(refname);
	मुक्त(new_target);
	strbuf_release(&err);
}

स्थिर शून्य parse_cmd_delete(संरचना ref_transaction *transaction,
			     ध्रुव वर्ण *next, ध्रुव वर्ण *end,
			     संरचना command_options *opts UNUSED)
{
	संरचना strbuf err = STRBUF_INIT;
	वर्ण *refname;
	संरचना object_id old_oid;
	पूर्णाङ्क have_old;

	refname = parse_refname(&next);
	यदि (!refname)
		die("delete: missing <ref>");

	यदि (parse_next_oid(&next, end, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} अन्यथा {
		यदि (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	यदि (*next != पंक्ति_termination)
		die("delete %s: extra input: %s", refname, next);

	यदि (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : रिक्त,
				   रिक्त, update_flags, संदेश, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	मुक्त(refname);
	strbuf_release(&err);
}

स्थिर शून्य parse_cmd_symref_delete(संरचना ref_transaction *transaction,
				    ध्रुव वर्ण *next, ध्रुव वर्ण *end UNUSED,
				    संरचना command_options *opts UNUSED)
{
	संरचना strbuf err = STRBUF_INIT;
	वर्ण *refname, *old_target;

	यदि (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&next);
	यदि (!refname)
		die("symref-delete: missing <ref>");

	old_target = parse_next_refname(&next);

	यदि (*next != पंक्ति_termination)
		die("symref-delete %s: extra input: %s", refname, next);

	यदि (ref_transaction_delete(transaction, refname, रिक्त,
				   old_target, update_flags, संदेश, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	मुक्त(refname);
	मुक्त(old_target);
	strbuf_release(&err);
}

स्थिर शून्य parse_cmd_verify(संरचना ref_transaction *transaction,
			     ध्रुव वर्ण *next, ध्रुव वर्ण *end,
			     संरचना command_options *opts UNUSED)
{
	संरचना strbuf err = STRBUF_INIT;
	वर्ण *refname;
	संरचना object_id old_oid;

	refname = parse_refname(&next);
	यदि (!refname)
		die("verify: missing <ref>");

	यदि (parse_next_oid(&next, end, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	यदि (*next != पंक्ति_termination)
		die("verify %s: extra input: %s", refname, next);

	यदि (ref_transaction_verify(transaction, refname, &old_oid,
				   रिक्त, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	मुक्त(refname);
	strbuf_release(&err);
}

स्थिर शून्य parse_cmd_symref_verify(संरचना ref_transaction *transaction,
				    ध्रुव वर्ण *next, ध्रुव वर्ण *end UNUSED,
				    संरचना command_options *opts UNUSED)
{
	संरचना strbuf err = STRBUF_INIT;
	संरचना object_id old_oid;
	वर्ण *refname, *old_target;

	यदि (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&next);
	यदि (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_target = parse_next_refname(&next);
	यदि (!old_target)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	यदि (*next != पंक्ति_termination)
		die("symref-verify %s: extra input: %s", refname, next);

	यदि (ref_transaction_verify(transaction, refname,
				   old_target ? रिक्त : &old_oid,
				   old_target, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	मुक्त(refname);
	मुक्त(old_target);
	strbuf_release(&err);
}

स्थिर शून्य report_ok(ध्रुव वर्ण *command)
{
	ख_लिखो(मानक_निकास, "%s: ok\n", command);
	ख_साफ(मानक_निकास);
}

स्थिर शून्य parse_cmd_option(संरचना ref_transaction *transaction UNUSED,
			     ध्रुव वर्ण *next, ध्रुव वर्ण *end UNUSED,
			     संरचना command_options *opts UNUSED)
{
	ध्रुव वर्ण *rest;
	यदि (skip_prefix(next, "no-deref", &rest) && *rest == पंक्ति_termination)
		update_flags |= REF_NO_DEREF;
	अन्यथा
		die("option unknown: %s", next);
}

स्थिर शून्य parse_cmd_start(संरचना ref_transaction *transaction UNUSED,
			    ध्रुव वर्ण *next, ध्रुव वर्ण *end UNUSED,
			    संरचना command_options *opts UNUSED)
{
	यदि (*next != पंक्ति_termination)
		die("start: extra input: %s", next);
	report_ok("start");
}

स्थिर शून्य parse_cmd_prepare(संरचना ref_transaction *transaction,
			      ध्रुव वर्ण *next, ध्रुव वर्ण *end UNUSED,
			      संरचना command_options *opts UNUSED)
{
	संरचना strbuf error = STRBUF_INIT;
	यदि (*next != पंक्ति_termination)
		die("prepare: extra input: %s", next);
	यदि (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

स्थिर शून्य parse_cmd_abort(संरचना ref_transaction *transaction,
			    ध्रुव वर्ण *next, ध्रुव वर्ण *end UNUSED,
			    संरचना command_options *opts UNUSED)
{
	संरचना strbuf error = STRBUF_INIT;
	यदि (*next != पंक्ति_termination)
		die("abort: extra input: %s", next);
	यदि (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

स्थिर शून्य parse_cmd_commit(संरचना ref_transaction *transaction,
			     ध्रुव वर्ण *next, ध्रुव वर्ण *end UNUSED,
			     संरचना command_options *opts UNUSED)
{
	संरचना strbuf error = STRBUF_INIT;
	यदि (*next != पंक्ति_termination)
		die("commit: extra input: %s", next);
	यदि (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, रिक्त);

	report_ok("commit");
	ref_transaction_free(transaction);
}

गणना update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

स्थिर ध्रुव संरचना parse_cmd {
	ध्रुव वर्ण *prefix;
	शून्य (*fn)(संरचना ref_transaction *, ध्रुव वर्ण *, ध्रुव वर्ण *,
		   संरचना command_options *);
	अचिन्हित args;
	गणना update_refs_state state;
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

स्थिर शून्य update_refs_stdin(अचिन्हित पूर्णाङ्क flags)
{
	संरचना strbuf निवेश = STRBUF_INIT, err = STRBUF_INIT;
	गणना update_refs_state state = UPDATE_REFS_OPEN;
	संरचना ref_transaction *transaction;
	पूर्णाङ्क i, j;

	संरचना command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	यदि (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	यावत् (!strbuf_getwholeline(&निवेश, मानक_निवेश, पंक्ति_termination)) {
		ध्रुव संरचना parse_cmd *cmd = रिक्त;

		यदि (*निवेश.buf == पंक्ति_termination)
			die("empty command in input");
		अन्यथा यदि (है_खाली(*निवेश.buf))
			die("whitespace before command: %s", निवेश.buf);

		प्रति (i = 0; i < ARRAY_SIZE(command); i++) {
			ध्रुव वर्ण *prefix = command[i].prefix;
			वर्ण c;

			यदि (!starts_with(निवेश.buf, prefix))
				अनुवर्त;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].args ? ' ' : पंक्ति_termination;
			यदि (निवेश.buf[म_माप(prefix)] != c)
				अनुवर्त;

			cmd = &command[i];
			भङ्ग;
		}
		यदि (!cmd)
			die("unknown command: %s", निवेश.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		प्रति (j = 1; पंक्ति_termination == '\0' && j < cmd->args; j++)
			यदि (strbuf_appendwholeline(&निवेश, मानक_निवेश, पंक्ति_termination))
				भङ्ग;

		विकल्प (state) {
		पक्ष UPDATE_REFS_OPEN:
		पक्ष UPDATE_REFS_STARTED:
			यदि (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			यदि (cmd->state >= state)
				state = cmd->state;
			भङ्ग;
		पक्ष UPDATE_REFS_PREPARED:
			यदि (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			भङ्ग;
		पक्ष UPDATE_REFS_CLOSED:
			यदि (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			यदि (!transaction)
				die("%s", err.buf);

			भङ्ग;
		}

		cmd->fn(transaction, निवेश.buf + म_माप(cmd->prefix) + !!cmd->args,
			निवेश.buf + निवेश.len, &opts);
	}

	विकल्प (state) {
	पक्ष UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		यदि (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, रिक्त);
		ref_transaction_free(transaction);
		भङ्ग;
	पक्ष UPDATE_REFS_STARTED:
	पक्ष UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		यदि (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		भङ्ग;
	पक्ष UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		भङ्ग;
	}

	strbuf_release(&err);
	strbuf_release(&निवेश);
}

पूर्णाङ्क cmd_update_ref(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *prefix,
		   संरचना repository *repo UNUSED)
{
	ध्रुव वर्ण *refname, *oldval;
	संरचना object_id oid, oldoid;
	पूर्णाङ्क delete = 0, no_deref = 0, read_stdin = 0, end_null = 0;
	पूर्णाङ्क create_reflog = 0;
	अचिन्हित पूर्णाङ्क flags = 0;

	संरचना option options[] = {
		OPT_STRING( 'm', रिक्त, &संदेश, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', रिक्त, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', रिक्त, &end_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_BIT('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, prefix, options, git_update_ref_usage,
			     0);
	यदि (संदेश && !*संदेश)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	यदि (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	यदि (read_stdin) {
		यदि (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		यदि (end_null)
			पंक्ति_termination = '\0';
		update_refs_stdin(flags);
		प्रत्यागम 0;
	} अन्यथा यदि (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	यदि (end_null)
		usage_with_options(git_update_ref_usage, options);

	यदि (delete) {
		यदि (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} अन्यथा {
		ध्रुव वर्ण *मूल्य;
		यदि (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		मूल्य = argv[1];
		oldval = argv[2];
		यदि (repo_get_oid_with_flags(the_repository, मूल्य, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", मूल्य);
	}

	यदि (oldval) {
		यदि (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		अन्यथा यदि (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	यदि (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		प्रत्यागम refs_delete_ref(get_main_ref_store(the_repository),
				       संदेश, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : रिक्त,
				       default_flags);
	अन्यथा
		प्रत्यागम refs_update_ref(get_main_ref_store(the_repository),
				       संदेश, refname, &oid,
				       oldval ? &oldoid : रिक्त,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
