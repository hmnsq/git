#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hash.h"
#समावेश "hex.h"
#समावेश "refs.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "quote.h"

अटल स्थिर अक्षर * स्थिर git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	शून्य
};

अटल अक्षर पंक्ति_termination = '\n';
अटल अचिन्हित पूर्णांक update_flags;
अटल अचिन्हित पूर्णांक default_flags;
अटल अचिन्हित create_reflog_flag;
अटल स्थिर अक्षर *संदेश;

काष्ठा command_options {
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
अटल स्थिर अक्षर *parse_arg(स्थिर अक्षर *अग्र, काष्ठा strbuf *arg)
{
	अगर (*अग्र == '"') {
		स्थिर अक्षर *orig = अग्र;

		अगर (unquote_c_style(arg, अग्र, &अग्र))
			die("badly quoted argument: %s", orig);
		अगर (*अग्र && !है_खाली(*अग्र))
			die("unexpected character after quoted argument: %s", orig);
	} अन्यथा {
		जबतक (*अग्र && !है_खाली(*अग्र))
			strbuf_addch(arg, *अग्र++);
	}

	वापस अग्र;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
अटल अक्षर *parse_refname(स्थिर अक्षर **अग्र)
{
	काष्ठा strbuf ref = STRBUF_INIT;

	अगर (पंक्ति_termination) {
		/* Without -z, use the next argument */
		*अग्र = parse_arg(*अग्र, &ref);
	} अन्यथा {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *अग्र);
		*अग्र += ref.len;
	}

	अगर (!ref.len) {
		strbuf_release(&ref);
		वापस शून्य;
	}

	अगर (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	वापस strbuf_detach(&ref, शून्य);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
अटल अक्षर *parse_अग्र_refname(स्थिर अक्षर **अग्र)
{
	अगर (पंक्ति_termination) {
		/* Without -z, consume SP and use next argument */
		अगर (!**अग्र || **अग्र == पंक्ति_termination)
			वापस शून्य;
		अगर (**अग्र != ' ')
			die("expected SP but got: %s", *अग्र);
	} अन्यथा {
		/* With -z, read the next NUL-terminated line */
		अगर (**अग्र)
			वापस शून्य;
	}
	/* Skip the delimiter */
	(*अग्र)++;

	वापस parse_refname(अग्र);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
अटल अक्षर *parse_अग्र_arg(स्थिर अक्षर **अग्र)
{
	काष्ठा strbuf arg = STRBUF_INIT;

	अगर (पंक्ति_termination) {
		/* Without -z, consume SP and use next argument */
		अगर (!**अग्र || **अग्र == पंक्ति_termination)
			वापस शून्य;
		अगर (**अग्र != ' ')
			die("expected SP but got: %s", *अग्र);
	} अन्यथा {
		/* With -z, read the next NUL-terminated line */
		अगर (**अग्र)
			वापस शून्य;
	}
	/* Skip the delimiter */
	(*अग्र)++;

	अगर (पंक्ति_termination) {
		/* Without -z, use the next argument */
		*अग्र = parse_arg(*अग्र, &arg);
	} अन्यथा {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *अग्र);
		*अग्र += arg.len;
	}

	अगर (arg.len)
		वापस strbuf_detach(&arg, शून्य);

	strbuf_release(&arg);
	वापस शून्य;
}

/*
 * The value being parsed is <old-oid> (as opposed to <new-oid>; the
 * difference affects which error messages are generated):
 */
#घोषणा PARSE_SHA1_OLD 0x01

/*
 * For backwards compatibility, accept an empty string for update's
 * <new-oid> in binary mode to be equivalent to specifying zeros.
 */
#घोषणा PARSE_SHA1_ALLOW_EMPTY 0x02

/*
 * Parse an argument separator followed by the next argument, if any.
 * If there is an argument, convert it to a SHA-1, write it to sha1,
 * set *next to point at the character terminating the argument, and
 * return 0.  If there is no argument at all (not even the empty
 * string), return 1 and leave *next unchanged.  If the value is
 * provided but cannot be converted to a SHA-1, die.  flags can
 * include PARSE_SHA1_OLD and/or PARSE_SHA1_ALLOW_EMPTY.
 */
अटल पूर्णांक parse_अग्र_oid(स्थिर अक्षर **अग्र, स्थिर अक्षर *end,
			  काष्ठा object_id *oid,
			  स्थिर अक्षर *command, स्थिर अक्षर *refname,
			  पूर्णांक flags)
{
	काष्ठा strbuf arg = STRBUF_INIT;
	पूर्णांक ret = 0;

	अगर (*अग्र == end)
		जाओ eof;

	अगर (पंक्ति_termination) {
		/* Without -z, consume SP and use next argument */
		अगर (!**अग्र || **अग्र == पंक्ति_termination)
			वापस 1;
		अगर (**अग्र != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *अग्र);
		(*अग्र)++;
		*अग्र = parse_arg(*अग्र, &arg);
		अगर (arg.len) {
			अगर (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				जाओ invalid;
		} अन्यथा {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} अन्यथा {
		/* With -z, read the next NUL-terminated line */
		अगर (**अग्र)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *अग्र);
		(*अग्र)++;
		अगर (*अग्र == end)
			जाओ eof;
		strbuf_addstr(&arg, *अग्र);
		*अग्र += arg.len;

		अगर (arg.len) {
			अगर (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				जाओ invalid;
		} अन्यथा अगर (flags & PARSE_SHA1_ALLOW_EMPTY) {
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

	वापस ret;

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

अटल व्योम print_rejected_refs(स्थिर अक्षर *refname,
				स्थिर काष्ठा object_id *old_oid,
				स्थिर काष्ठा object_id *new_oid,
				स्थिर अक्षर *old_target,
				स्थिर अक्षर *new_target,
				क्रमागत ref_transaction_error err,
				स्थिर अक्षर *details,
				व्योम *cb_तथ्य UNUSED)
{
	काष्ठा strbuf sb = STRBUF_INIT;

	अगर (details && *details)
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
अटल व्योम handle_ref_transaction_error(स्थिर अक्षर *refname,
					 काष्ठा object_id *new_oid,
					 काष्ठा object_id *old_oid,
					 स्थिर अक्षर *new_target,
					 स्थिर अक्षर *old_target,
					 क्रमागत ref_transaction_error tx_err,
					 काष्ठा strbuf *err,
					 काष्ठा command_options *opts)
{
	अगर (!tx_err)
		वापस;

	अगर (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_target,
				    new_target, tx_err, err->buf, शून्य);
		वापस;
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

अटल व्योम parse_cmd_update(काष्ठा ref_transaction *transaction,
			     स्थिर अक्षर *अग्र, स्थिर अक्षर *end,
			     काष्ठा command_options *opts)
{
	काष्ठा strbuf err = STRBUF_INIT;
	अक्षर *refname;
	काष्ठा object_id new_oid, old_oid;
	क्रमागत ref_transaction_error tx_err;
	पूर्णांक have_old;

	refname = parse_refname(&अग्र);
	अगर (!refname)
		die("update: missing <ref>");

	अगर (parse_अग्र_oid(&अग्र, end, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_EMPTY))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_अग्र_oid(&अग्र, end, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	अगर (*अग्र != पंक्ति_termination)
		die("update %s: extra input: %s", refname, अग्र);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : शून्य,
					शून्य, शून्य,
					update_flags | create_reflog_flag,
					संदेश, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : शून्य,
				     शून्य, शून्य, tx_err, &err, opts);


	update_flags = default_flags;
	मुक्त(refname);
	strbuf_release(&err);
}

अटल व्योम parse_cmd_symref_update(काष्ठा ref_transaction *transaction,
				    स्थिर अक्षर *अग्र, स्थिर अक्षर *end UNUSED,
				    काष्ठा command_options *opts)
{
	अक्षर *refname, *new_target, *old_arg;
	क्रमागत ref_transaction_error tx_err;
	अक्षर *old_target = शून्य;
	काष्ठा strbuf err = STRBUF_INIT;
	काष्ठा object_id old_oid;
	पूर्णांक have_old_oid = 0;

	refname = parse_refname(&अग्र);
	अगर (!refname)
		die("symref-update: missing <ref>");

	new_target = parse_अग्र_refname(&अग्र);
	अगर (!new_target)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_अग्र_arg(&अग्र);
	अगर (old_arg) {
		old_target = parse_अग्र_arg(&अग्र);
		अगर (!old_target)
			die("symref-update %s: expected old value", refname);

		अगर (!म_भेद(old_arg, "oid")) {
			अगर (repo_get_oid_with_flags(the_repository, old_target, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_target);

			have_old_oid = 1;
		} अन्यथा अगर (!म_भेद(old_arg, "ref")) {
			अगर (check_refname_format(old_target, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_target);
		} अन्यथा {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	अगर (*अग्र != पंक्ति_termination)
		die("symref-update %s: extra input: %s", refname, अग्र);

	tx_err = ref_transaction_update(transaction, refname, शून्य,
					have_old_oid ? &old_oid : शून्य,
					new_target,
					have_old_oid ? शून्य : old_target,
					update_flags | create_reflog_flag,
					संदेश, &err);
	handle_ref_transaction_error(refname, शून्य, have_old_oid ? &old_oid : शून्य,
				     new_target, have_old_oid ? शून्य : old_target,
				     tx_err, &err, opts);

	update_flags = default_flags;
	मुक्त(refname);
	मुक्त(old_arg);
	मुक्त(old_target);
	मुक्त(new_target);
	strbuf_release(&err);
}

अटल व्योम parse_cmd_create(काष्ठा ref_transaction *transaction,
			     स्थिर अक्षर *अग्र, स्थिर अक्षर *end,
			     काष्ठा command_options *opts)
{
	काष्ठा strbuf err = STRBUF_INIT;
	अक्षर *refname;
	काष्ठा object_id new_oid;
	क्रमागत ref_transaction_error tx_err;

	refname = parse_refname(&अग्र);
	अगर (!refname)
		die("create: missing <ref>");

	अगर (parse_अग्र_oid(&अग्र, end, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	अगर (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	अगर (*अग्र != पंक्ति_termination)
		die("create %s: extra input: %s", refname, अग्र);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, शून्य,
					update_flags | create_reflog_flag,
					संदेश, &err);
	handle_ref_transaction_error(refname, &new_oid, शून्य, शून्य, शून्य, tx_err,
				     &err, opts);

	update_flags = default_flags;
	मुक्त(refname);
	strbuf_release(&err);
}

अटल व्योम parse_cmd_symref_create(काष्ठा ref_transaction *transaction,
				    स्थिर अक्षर *अग्र, स्थिर अक्षर *end UNUSED,
				    काष्ठा command_options *opts)
{
	काष्ठा strbuf err = STRBUF_INIT;
	अक्षर *refname, *new_target;
	क्रमागत ref_transaction_error tx_err;

	refname = parse_refname(&अग्र);
	अगर (!refname)
		die("symref-create: missing <ref>");

	new_target = parse_अग्र_refname(&अग्र);
	अगर (!new_target)
		die("symref-create %s: missing <new-target>", refname);

	अगर (*अग्र != पंक्ति_termination)
		die("symref-create %s: extra input: %s", refname, अग्र);

	tx_err = ref_transaction_create(transaction, refname, शून्य, new_target,
					update_flags | create_reflog_flag,
					संदेश, &err);
	handle_ref_transaction_error(refname, शून्य, शून्य, new_target, शून्य,
				     tx_err, &err, opts);

	update_flags = default_flags;
	मुक्त(refname);
	मुक्त(new_target);
	strbuf_release(&err);
}

अटल व्योम parse_cmd_delete(काष्ठा ref_transaction *transaction,
			     स्थिर अक्षर *अग्र, स्थिर अक्षर *end,
			     काष्ठा command_options *opts UNUSED)
{
	काष्ठा strbuf err = STRBUF_INIT;
	अक्षर *refname;
	काष्ठा object_id old_oid;
	पूर्णांक have_old;

	refname = parse_refname(&अग्र);
	अगर (!refname)
		die("delete: missing <ref>");

	अगर (parse_अग्र_oid(&अग्र, end, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} अन्यथा {
		अगर (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	अगर (*अग्र != पंक्ति_termination)
		die("delete %s: extra input: %s", refname, अग्र);

	अगर (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : शून्य,
				   शून्य, update_flags, संदेश, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	मुक्त(refname);
	strbuf_release(&err);
}

अटल व्योम parse_cmd_symref_delete(काष्ठा ref_transaction *transaction,
				    स्थिर अक्षर *अग्र, स्थिर अक्षर *end UNUSED,
				    काष्ठा command_options *opts UNUSED)
{
	काष्ठा strbuf err = STRBUF_INIT;
	अक्षर *refname, *old_target;

	अगर (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&अग्र);
	अगर (!refname)
		die("symref-delete: missing <ref>");

	old_target = parse_अग्र_refname(&अग्र);

	अगर (*अग्र != पंक्ति_termination)
		die("symref-delete %s: extra input: %s", refname, अग्र);

	अगर (ref_transaction_delete(transaction, refname, शून्य,
				   old_target, update_flags, संदेश, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	मुक्त(refname);
	मुक्त(old_target);
	strbuf_release(&err);
}

अटल व्योम parse_cmd_verify(काष्ठा ref_transaction *transaction,
			     स्थिर अक्षर *अग्र, स्थिर अक्षर *end,
			     काष्ठा command_options *opts UNUSED)
{
	काष्ठा strbuf err = STRBUF_INIT;
	अक्षर *refname;
	काष्ठा object_id old_oid;

	refname = parse_refname(&अग्र);
	अगर (!refname)
		die("verify: missing <ref>");

	अगर (parse_अग्र_oid(&अग्र, end, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	अगर (*अग्र != पंक्ति_termination)
		die("verify %s: extra input: %s", refname, अग्र);

	अगर (ref_transaction_verify(transaction, refname, &old_oid,
				   शून्य, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	मुक्त(refname);
	strbuf_release(&err);
}

अटल व्योम parse_cmd_symref_verify(काष्ठा ref_transaction *transaction,
				    स्थिर अक्षर *अग्र, स्थिर अक्षर *end UNUSED,
				    काष्ठा command_options *opts UNUSED)
{
	काष्ठा strbuf err = STRBUF_INIT;
	काष्ठा object_id old_oid;
	अक्षर *refname, *old_target;

	अगर (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&अग्र);
	अगर (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_target = parse_अग्र_refname(&अग्र);
	अगर (!old_target)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	अगर (*अग्र != पंक्ति_termination)
		die("symref-verify %s: extra input: %s", refname, अग्र);

	अगर (ref_transaction_verify(transaction, refname,
				   old_target ? शून्य : &old_oid,
				   old_target, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	मुक्त(refname);
	मुक्त(old_target);
	strbuf_release(&err);
}

अटल व्योम report_ok(स्थिर अक्षर *command)
{
	ख_लिखो(मानक_निकास, "%s: ok\n", command);
	ख_साफ(मानक_निकास);
}

अटल व्योम parse_cmd_option(काष्ठा ref_transaction *transaction UNUSED,
			     स्थिर अक्षर *अग्र, स्थिर अक्षर *end UNUSED,
			     काष्ठा command_options *opts UNUSED)
{
	स्थिर अक्षर *rest;
	अगर (skip_prefix(अग्र, "no-deref", &rest) && *rest == पंक्ति_termination)
		update_flags |= REF_NO_DEREF;
	अन्यथा
		die("option unknown: %s", अग्र);
}

अटल व्योम parse_cmd_start(काष्ठा ref_transaction *transaction UNUSED,
			    स्थिर अक्षर *अग्र, स्थिर अक्षर *end UNUSED,
			    काष्ठा command_options *opts UNUSED)
{
	अगर (*अग्र != पंक्ति_termination)
		die("start: extra input: %s", अग्र);
	report_ok("start");
}

अटल व्योम parse_cmd_prepare(काष्ठा ref_transaction *transaction,
			      स्थिर अक्षर *अग्र, स्थिर अक्षर *end UNUSED,
			      काष्ठा command_options *opts UNUSED)
{
	काष्ठा strbuf error = STRBUF_INIT;
	अगर (*अग्र != पंक्ति_termination)
		die("prepare: extra input: %s", अग्र);
	अगर (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

अटल व्योम parse_cmd_abort(काष्ठा ref_transaction *transaction,
			    स्थिर अक्षर *अग्र, स्थिर अक्षर *end UNUSED,
			    काष्ठा command_options *opts UNUSED)
{
	काष्ठा strbuf error = STRBUF_INIT;
	अगर (*अग्र != पंक्ति_termination)
		die("abort: extra input: %s", अग्र);
	अगर (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

अटल व्योम parse_cmd_commit(काष्ठा ref_transaction *transaction,
			     स्थिर अक्षर *अग्र, स्थिर अक्षर *end UNUSED,
			     काष्ठा command_options *opts UNUSED)
{
	काष्ठा strbuf error = STRBUF_INIT;
	अगर (*अग्र != पंक्ति_termination)
		die("commit: extra input: %s", अग्र);
	अगर (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, शून्य);

	report_ok("commit");
	ref_transaction_free(transaction);
}

क्रमागत update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

अटल स्थिर काष्ठा parse_cmd {
	स्थिर अक्षर *prefix;
	व्योम (*fn)(काष्ठा ref_transaction *, स्थिर अक्षर *, स्थिर अक्षर *,
		   काष्ठा command_options *);
	अचिन्हित args;
	क्रमागत update_refs_state state;
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

अटल व्योम update_refs_stdin(अचिन्हित पूर्णांक flags)
{
	काष्ठा strbuf निवेश = STRBUF_INIT, err = STRBUF_INIT;
	क्रमागत update_refs_state state = UPDATE_REFS_OPEN;
	काष्ठा ref_transaction *transaction;
	पूर्णांक i, j;

	काष्ठा command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	अगर (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	जबतक (!strbuf_getwholeline(&निवेश, मानक_निवेश, पंक्ति_termination)) {
		स्थिर काष्ठा parse_cmd *cmd = शून्य;

		अगर (*निवेश.buf == पंक्ति_termination)
			die("empty command in input");
		अन्यथा अगर (है_खाली(*निवेश.buf))
			die("whitespace before command: %s", निवेश.buf);

		क्रम (i = 0; i < ARRAY_आकार(command); i++) {
			स्थिर अक्षर *prefix = command[i].prefix;
			अक्षर c;

			अगर (!starts_with(निवेश.buf, prefix))
				जारी;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].args ? ' ' : पंक्ति_termination;
			अगर (निवेश.buf[म_माप(prefix)] != c)
				जारी;

			cmd = &command[i];
			अवरोध;
		}
		अगर (!cmd)
			die("unknown command: %s", निवेश.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		क्रम (j = 1; पंक्ति_termination == '\0' && j < cmd->args; j++)
			अगर (strbuf_appendwholeline(&निवेश, मानक_निवेश, पंक्ति_termination))
				अवरोध;

		चयन (state) {
		हाल UPDATE_REFS_OPEN:
		हाल UPDATE_REFS_STARTED:
			अगर (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			अगर (cmd->state >= state)
				state = cmd->state;
			अवरोध;
		हाल UPDATE_REFS_PREPARED:
			अगर (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			अवरोध;
		हाल UPDATE_REFS_CLOSED:
			अगर (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			अगर (!transaction)
				die("%s", err.buf);

			अवरोध;
		}

		cmd->fn(transaction, निवेश.buf + म_माप(cmd->prefix) + !!cmd->args,
			निवेश.buf + निवेश.len, &opts);
	}

	चयन (state) {
	हाल UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		अगर (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, शून्य);
		ref_transaction_free(transaction);
		अवरोध;
	हाल UPDATE_REFS_STARTED:
	हाल UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		अगर (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		अवरोध;
	हाल UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		अवरोध;
	}

	strbuf_release(&err);
	strbuf_release(&निवेश);
}

पूर्णांक cmd_update_ref(पूर्णांक argc,
		   स्थिर अक्षर **argv,
		   स्थिर अक्षर *prefix,
		   काष्ठा repository *repo UNUSED)
{
	स्थिर अक्षर *refname, *oldval;
	काष्ठा object_id oid, oldoid;
	पूर्णांक delete = 0, no_deref = 0, read_stdin = 0, end_null = 0;
	पूर्णांक create_reflog = 0;
	अचिन्हित पूर्णांक flags = 0;

	काष्ठा option options[] = {
		OPT_STRING( 'm', शून्य, &संदेश, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', शून्य, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', शून्य, &end_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_BIT('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, शून्य);
	argc = parse_options(argc, argv, prefix, options, git_update_ref_usage,
			     0);
	अगर (संदेश && !*संदेश)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	अगर (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	अगर (read_stdin) {
		अगर (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		अगर (end_null)
			पंक्ति_termination = '\0';
		update_refs_stdin(flags);
		वापस 0;
	} अन्यथा अगर (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	अगर (end_null)
		usage_with_options(git_update_ref_usage, options);

	अगर (delete) {
		अगर (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} अन्यथा {
		स्थिर अक्षर *मूल्य;
		अगर (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		मूल्य = argv[1];
		oldval = argv[2];
		अगर (repo_get_oid_with_flags(the_repository, मूल्य, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", मूल्य);
	}

	अगर (oldval) {
		अगर (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		अन्यथा अगर (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	अगर (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		वापस refs_delete_ref(get_main_ref_store(the_repository),
				       संदेश, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : शून्य,
				       default_flags);
	अन्यथा
		वापस refs_update_ref(get_main_ref_store(the_repository),
				       संदेश, refname, &oid,
				       oldval ? &oldoid : शून्य,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
