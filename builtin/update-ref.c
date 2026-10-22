#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅੱਖਰ ਸਤਰ_termination = '\n';
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ update_flags;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ default_flags;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ create_reflog_flag;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ;

ਬਣਤਰ command_options {
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
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *parse_arg(ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਬਣਤਰ strbuf *arg)
{
	ਜੇ (*ਅਗਲਾ == '"') {
		ਅਚਲ ਅੱਖਰ *orig = ਅਗਲਾ;

		ਜੇ (unquote_c_style(arg, ਅਗਲਾ, &ਅਗਲਾ))
			die("badly quoted argument: %s", orig);
		ਜੇ (*ਅਗਲਾ && !ਖਾਲੀ_ਥਾਂ_ਹੈ(*ਅਗਲਾ))
			die("unexpected character after quoted argument: %s", orig);
	} ਨਹੀਂਤਾਂ {
		ਜਦੋਂ (*ਅਗਲਾ && !ਖਾਲੀ_ਥਾਂ_ਹੈ(*ਅਗਲਾ))
			strbuf_addch(arg, *ਅਗਲਾ++);
	}

	ਵਾਪਸ ਅਗਲਾ;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
ਸਥਿਰ ਅੱਖਰ *parse_refname(ਅਚਲ ਅੱਖਰ **ਅਗਲਾ)
{
	ਬਣਤਰ strbuf ref = STRBUF_INIT;

	ਜੇ (ਸਤਰ_termination) {
		/* Without -z, use the next argument */
		*ਅਗਲਾ = parse_arg(*ਅਗਲਾ, &ref);
	} ਨਹੀਂਤਾਂ {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *ਅਗਲਾ);
		*ਅਗਲਾ += ref.len;
	}

	ਜੇ (!ref.len) {
		strbuf_release(&ref);
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਜੇ (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	ਵਾਪਸ strbuf_detach(&ref, ਨਿਰਾ);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
ਸਥਿਰ ਅੱਖਰ *parse_ਅਗਲਾ_refname(ਅਚਲ ਅੱਖਰ **ਅਗਲਾ)
{
	ਜੇ (ਸਤਰ_termination) {
		/* Without -z, consume SP and use next argument */
		ਜੇ (!**ਅਗਲਾ || **ਅਗਲਾ == ਸਤਰ_termination)
			ਵਾਪਸ ਨਿਰਾ;
		ਜੇ (**ਅਗਲਾ != ' ')
			die("expected SP but got: %s", *ਅਗਲਾ);
	} ਨਹੀਂਤਾਂ {
		/* With -z, read the next NUL-terminated line */
		ਜੇ (**ਅਗਲਾ)
			ਵਾਪਸ ਨਿਰਾ;
	}
	/* Skip the delimiter */
	(*ਅਗਲਾ)++;

	ਵਾਪਸ parse_refname(ਅਗਲਾ);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
ਸਥਿਰ ਅੱਖਰ *parse_ਅਗਲਾ_arg(ਅਚਲ ਅੱਖਰ **ਅਗਲਾ)
{
	ਬਣਤਰ strbuf arg = STRBUF_INIT;

	ਜੇ (ਸਤਰ_termination) {
		/* Without -z, consume SP and use next argument */
		ਜੇ (!**ਅਗਲਾ || **ਅਗਲਾ == ਸਤਰ_termination)
			ਵਾਪਸ ਨਿਰਾ;
		ਜੇ (**ਅਗਲਾ != ' ')
			die("expected SP but got: %s", *ਅਗਲਾ);
	} ਨਹੀਂਤਾਂ {
		/* With -z, read the next NUL-terminated line */
		ਜੇ (**ਅਗਲਾ)
			ਵਾਪਸ ਨਿਰਾ;
	}
	/* Skip the delimiter */
	(*ਅਗਲਾ)++;

	ਜੇ (ਸਤਰ_termination) {
		/* Without -z, use the next argument */
		*ਅਗਲਾ = parse_arg(*ਅਗਲਾ, &arg);
	} ਨਹੀਂਤਾਂ {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *ਅਗਲਾ);
		*ਅਗਲਾ += arg.len;
	}

	ਜੇ (arg.len)
		ਵਾਪਸ strbuf_detach(&arg, ਨਿਰਾ);

	strbuf_release(&arg);
	ਵਾਪਸ ਨਿਰਾ;
}

/*
 * The value being parsed is <old-oid> (as opposed to <new-oid>; the
 * difference affects which error messages are generated):
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PARSE_SHA1_OLD 0x01

/*
 * For backwards compatibility, accept an empty string for update's
 * <new-oid> in binary mode to be equivalent to specifying zeros.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PARSE_SHA1_ALLOW_ਖਾਲੀ_ਥਾਵਾਂ 0x02

/*
 * Parse an argument separator followed by the next argument, if any.
 * If there is an argument, convert it to a SHA-1, write it to sha1,
 * set *next to point at the character terminating the argument, and
 * return 0.  If there is no argument at all (not even the empty
 * string), return 1 and leave *next unchanged.  If the value is
 * provided but cannot be converted to a SHA-1, die.  flags can
 * include PARSE_SHA1_OLD and/or PARSE_SHA1_ALLOW_EMPTY.
 */
ਸਥਿਰ ਪੂਰਨ parse_ਅਗਲਾ_oid(ਅਚਲ ਅੱਖਰ **ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ,
			  ਬਣਤਰ object_id *oid,
			  ਅਚਲ ਅੱਖਰ *command, ਅਚਲ ਅੱਖਰ *refname,
			  ਪੂਰਨ flags)
{
	ਬਣਤਰ strbuf arg = STRBUF_INIT;
	ਪੂਰਨ ret = 0;

	ਜੇ (*ਅਗਲਾ == ਸਿਰਾ)
		ਜਾਓ eof;

	ਜੇ (ਸਤਰ_termination) {
		/* Without -z, consume SP and use next argument */
		ਜੇ (!**ਅਗਲਾ || **ਅਗਲਾ == ਸਤਰ_termination)
			ਵਾਪਸ 1;
		ਜੇ (**ਅਗਲਾ != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *ਅਗਲਾ);
		(*ਅਗਲਾ)++;
		*ਅਗਲਾ = parse_arg(*ਅਗਲਾ, &arg);
		ਜੇ (arg.len) {
			ਜੇ (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				ਜਾਓ invalid;
		} ਨਹੀਂਤਾਂ {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} ਨਹੀਂਤਾਂ {
		/* With -z, read the next NUL-terminated line */
		ਜੇ (**ਅਗਲਾ)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *ਅਗਲਾ);
		(*ਅਗਲਾ)++;
		ਜੇ (*ਅਗਲਾ == ਸਿਰਾ)
			ਜਾਓ eof;
		strbuf_addstr(&arg, *ਅਗਲਾ);
		*ਅਗਲਾ += arg.len;

		ਜੇ (arg.len) {
			ਜੇ (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				ਜਾਓ invalid;
		} ਨਹੀਂਤਾਂ ਜੇ (flags & PARSE_SHA1_ALLOW_ਖਾਲੀ_ਥਾਵਾਂ) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} ਨਹੀਂਤਾਂ {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	ਵਾਪਸ ret;

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

ਸਥਿਰ ਖਾਲੀ print_rejected_refs(ਅਚਲ ਅੱਖਰ *refname,
				ਅਚਲ ਬਣਤਰ object_id *old_oid,
				ਅਚਲ ਬਣਤਰ object_id *new_oid,
				ਅਚਲ ਅੱਖਰ *old_ਨਿਸ਼ਾਨਾ,
				ਅਚਲ ਅੱਖਰ *new_ਨਿਸ਼ਾਨਾ,
				ਗਿਣਤੀ ref_transaction_error err,
				ਅਚਲ ਅੱਖਰ *details,
				ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	ਜੇ (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_ਮੰਜ਼ਿਲ_hex(new_oid) : new_ਨਿਸ਼ਾਨਾ,
		    old_oid ? oid_ਮੰਜ਼ਿਲ_hex(old_oid) : old_ਨਿਸ਼ਾਨਾ,
		    ref_transaction_error_ਸੁਨੇਹਾ(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
ਸਥਿਰ ਖਾਲੀ handle_ref_transaction_error(ਅਚਲ ਅੱਖਰ *refname,
					 ਬਣਤਰ object_id *new_oid,
					 ਬਣਤਰ object_id *old_oid,
					 ਅਚਲ ਅੱਖਰ *new_ਨਿਸ਼ਾਨਾ,
					 ਅਚਲ ਅੱਖਰ *old_ਨਿਸ਼ਾਨਾ,
					 ਗਿਣਤੀ ref_transaction_error tx_err,
					 ਬਣਤਰ strbuf *err,
					 ਬਣਤਰ command_options *opts)
{
	ਜੇ (!tx_err)
		ਵਾਪਸ;

	ਜੇ (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_ਨਿਸ਼ਾਨਾ,
				    new_ਨਿਸ਼ਾਨਾ, tx_err, err->buf, ਨਿਰਾ);
		ਵਾਪਸ;
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

ਸਥਿਰ ਖਾਲੀ parse_cmd_update(ਬਣਤਰ ref_transaction *transaction,
			     ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ,
			     ਬਣਤਰ command_options *opts)
{
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਅੱਖਰ *refname;
	ਬਣਤਰ object_id new_oid, old_oid;
	ਗਿਣਤੀ ref_transaction_error tx_err;
	ਪੂਰਨ have_old;

	refname = parse_refname(&ਅਗਲਾ);
	ਜੇ (!refname)
		die("update: missing <ref>");

	ਜੇ (parse_ਅਗਲਾ_oid(&ਅਗਲਾ, ਸਿਰਾ, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_ਖਾਲੀ_ਥਾਵਾਂ))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_ਅਗਲਾ_oid(&ਅਗਲਾ, ਸਿਰਾ, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	ਜੇ (*ਅਗਲਾ != ਸਤਰ_termination)
		die("update %s: extra input: %s", refname, ਅਗਲਾ);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : ਨਿਰਾ,
					ਨਿਰਾ, ਨਿਰਾ,
					update_flags | create_reflog_flag,
					ਸੁਨੇਹਾ, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : ਨਿਰਾ,
				     ਨਿਰਾ, ਨਿਰਾ, tx_err, &err, opts);


	update_flags = default_flags;
	ਮੁਕਤ(refname);
	strbuf_release(&err);
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_symref_update(ਬਣਤਰ ref_transaction *transaction,
				    ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ ਅਣਵਰਤਿਆ,
				    ਬਣਤਰ command_options *opts)
{
	ਅੱਖਰ *refname, *new_ਨਿਸ਼ਾਨਾ, *old_arg;
	ਗਿਣਤੀ ref_transaction_error tx_err;
	ਅੱਖਰ *old_ਨਿਸ਼ਾਨਾ = ਨਿਰਾ;
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਬਣਤਰ object_id old_oid;
	ਪੂਰਨ have_old_oid = 0;

	refname = parse_refname(&ਅਗਲਾ);
	ਜੇ (!refname)
		die("symref-update: missing <ref>");

	new_ਨਿਸ਼ਾਨਾ = parse_ਅਗਲਾ_refname(&ਅਗਲਾ);
	ਜੇ (!new_ਨਿਸ਼ਾਨਾ)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_ਅਗਲਾ_arg(&ਅਗਲਾ);
	ਜੇ (old_arg) {
		old_ਨਿਸ਼ਾਨਾ = parse_ਅਗਲਾ_arg(&ਅਗਲਾ);
		ਜੇ (!old_ਨਿਸ਼ਾਨਾ)
			die("symref-update %s: expected old value", refname);

		ਜੇ (!strcmp(old_arg, "oid")) {
			ਜੇ (repo_get_oid_with_flags(the_repository, old_ਨਿਸ਼ਾਨਾ, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_ਨਿਸ਼ਾਨਾ);

			have_old_oid = 1;
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(old_arg, "ref")) {
			ਜੇ (check_refname_format(old_ਨਿਸ਼ਾਨਾ, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_ਨਿਸ਼ਾਨਾ);
		} ਨਹੀਂਤਾਂ {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	ਜੇ (*ਅਗਲਾ != ਸਤਰ_termination)
		die("symref-update %s: extra input: %s", refname, ਅਗਲਾ);

	tx_err = ref_transaction_update(transaction, refname, ਨਿਰਾ,
					have_old_oid ? &old_oid : ਨਿਰਾ,
					new_ਨਿਸ਼ਾਨਾ,
					have_old_oid ? ਨਿਰਾ : old_ਨਿਸ਼ਾਨਾ,
					update_flags | create_reflog_flag,
					ਸੁਨੇਹਾ, &err);
	handle_ref_transaction_error(refname, ਨਿਰਾ, have_old_oid ? &old_oid : ਨਿਰਾ,
				     new_ਨਿਸ਼ਾਨਾ, have_old_oid ? ਨਿਰਾ : old_ਨਿਸ਼ਾਨਾ,
				     tx_err, &err, opts);

	update_flags = default_flags;
	ਮੁਕਤ(refname);
	ਮੁਕਤ(old_arg);
	ਮੁਕਤ(old_ਨਿਸ਼ਾਨਾ);
	ਮੁਕਤ(new_ਨਿਸ਼ਾਨਾ);
	strbuf_release(&err);
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_create(ਬਣਤਰ ref_transaction *transaction,
			     ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ,
			     ਬਣਤਰ command_options *opts)
{
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਅੱਖਰ *refname;
	ਬਣਤਰ object_id new_oid;
	ਗਿਣਤੀ ref_transaction_error tx_err;

	refname = parse_refname(&ਅਗਲਾ);
	ਜੇ (!refname)
		die("create: missing <ref>");

	ਜੇ (parse_ਅਗਲਾ_oid(&ਅਗਲਾ, ਸਿਰਾ, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	ਜੇ (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	ਜੇ (*ਅਗਲਾ != ਸਤਰ_termination)
		die("create %s: extra input: %s", refname, ਅਗਲਾ);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, ਨਿਰਾ,
					update_flags | create_reflog_flag,
					ਸੁਨੇਹਾ, &err);
	handle_ref_transaction_error(refname, &new_oid, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, tx_err,
				     &err, opts);

	update_flags = default_flags;
	ਮੁਕਤ(refname);
	strbuf_release(&err);
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_symref_create(ਬਣਤਰ ref_transaction *transaction,
				    ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ ਅਣਵਰਤਿਆ,
				    ਬਣਤਰ command_options *opts)
{
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਅੱਖਰ *refname, *new_ਨਿਸ਼ਾਨਾ;
	ਗਿਣਤੀ ref_transaction_error tx_err;

	refname = parse_refname(&ਅਗਲਾ);
	ਜੇ (!refname)
		die("symref-create: missing <ref>");

	new_ਨਿਸ਼ਾਨਾ = parse_ਅਗਲਾ_refname(&ਅਗਲਾ);
	ਜੇ (!new_ਨਿਸ਼ਾਨਾ)
		die("symref-create %s: missing <new-target>", refname);

	ਜੇ (*ਅਗਲਾ != ਸਤਰ_termination)
		die("symref-create %s: extra input: %s", refname, ਅਗਲਾ);

	tx_err = ref_transaction_create(transaction, refname, ਨਿਰਾ, new_ਨਿਸ਼ਾਨਾ,
					update_flags | create_reflog_flag,
					ਸੁਨੇਹਾ, &err);
	handle_ref_transaction_error(refname, ਨਿਰਾ, ਨਿਰਾ, new_ਨਿਸ਼ਾਨਾ, ਨਿਰਾ,
				     tx_err, &err, opts);

	update_flags = default_flags;
	ਮੁਕਤ(refname);
	ਮੁਕਤ(new_ਨਿਸ਼ਾਨਾ);
	strbuf_release(&err);
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_delete(ਬਣਤਰ ref_transaction *transaction,
			     ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ,
			     ਬਣਤਰ command_options *opts ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਅੱਖਰ *refname;
	ਬਣਤਰ object_id old_oid;
	ਪੂਰਨ have_old;

	refname = parse_refname(&ਅਗਲਾ);
	ਜੇ (!refname)
		die("delete: missing <ref>");

	ਜੇ (parse_ਅਗਲਾ_oid(&ਅਗਲਾ, ਸਿਰਾ, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	ਜੇ (*ਅਗਲਾ != ਸਤਰ_termination)
		die("delete %s: extra input: %s", refname, ਅਗਲਾ);

	ਜੇ (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : ਨਿਰਾ,
				   ਨਿਰਾ, update_flags, ਸੁਨੇਹਾ, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ਮੁਕਤ(refname);
	strbuf_release(&err);
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_symref_delete(ਬਣਤਰ ref_transaction *transaction,
				    ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ ਅਣਵਰਤਿਆ,
				    ਬਣਤਰ command_options *opts ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਅੱਖਰ *refname, *old_ਨਿਸ਼ਾਨਾ;

	ਜੇ (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&ਅਗਲਾ);
	ਜੇ (!refname)
		die("symref-delete: missing <ref>");

	old_ਨਿਸ਼ਾਨਾ = parse_ਅਗਲਾ_refname(&ਅਗਲਾ);

	ਜੇ (*ਅਗਲਾ != ਸਤਰ_termination)
		die("symref-delete %s: extra input: %s", refname, ਅਗਲਾ);

	ਜੇ (ref_transaction_delete(transaction, refname, ਨਿਰਾ,
				   old_ਨਿਸ਼ਾਨਾ, update_flags, ਸੁਨੇਹਾ, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ਮੁਕਤ(refname);
	ਮੁਕਤ(old_ਨਿਸ਼ਾਨਾ);
	strbuf_release(&err);
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_verify(ਬਣਤਰ ref_transaction *transaction,
			     ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ,
			     ਬਣਤਰ command_options *opts ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਅੱਖਰ *refname;
	ਬਣਤਰ object_id old_oid;

	refname = parse_refname(&ਅਗਲਾ);
	ਜੇ (!refname)
		die("verify: missing <ref>");

	ਜੇ (parse_ਅਗਲਾ_oid(&ਅਗਲਾ, ਸਿਰਾ, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	ਜੇ (*ਅਗਲਾ != ਸਤਰ_termination)
		die("verify %s: extra input: %s", refname, ਅਗਲਾ);

	ਜੇ (ref_transaction_verify(transaction, refname, &old_oid,
				   ਨਿਰਾ, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ਮੁਕਤ(refname);
	strbuf_release(&err);
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_symref_verify(ਬਣਤਰ ref_transaction *transaction,
				    ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ ਅਣਵਰਤਿਆ,
				    ਬਣਤਰ command_options *opts ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਬਣਤਰ object_id old_oid;
	ਅੱਖਰ *refname, *old_ਨਿਸ਼ਾਨਾ;

	ਜੇ (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&ਅਗਲਾ);
	ਜੇ (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_ਨਿਸ਼ਾਨਾ = parse_ਅਗਲਾ_refname(&ਅਗਲਾ);
	ਜੇ (!old_ਨਿਸ਼ਾਨਾ)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	ਜੇ (*ਅਗਲਾ != ਸਤਰ_termination)
		die("symref-verify %s: extra input: %s", refname, ਅਗਲਾ);

	ਜੇ (ref_transaction_verify(transaction, refname,
				   old_ਨਿਸ਼ਾਨਾ ? ਨਿਰਾ : &old_oid,
				   old_ਨਿਸ਼ਾਨਾ, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ਮੁਕਤ(refname);
	ਮੁਕਤ(old_ਨਿਸ਼ਾਨਾ);
	strbuf_release(&err);
}

ਸਥਿਰ ਖਾਲੀ report_ok(ਅਚਲ ਅੱਖਰ *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_option(ਬਣਤਰ ref_transaction *transaction ਅਣਵਰਤਿਆ,
			     ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ ਅਣਵਰਤਿਆ,
			     ਬਣਤਰ command_options *opts ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *rest;
	ਜੇ (skip_ਅਗੇਤਰ(ਅਗਲਾ, "no-deref", &rest) && *rest == ਸਤਰ_termination)
		update_flags |= REF_NO_DEREF;
	ਨਹੀਂਤਾਂ
		die("option unknown: %s", ਅਗਲਾ);
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_ਸ਼ੁਰੂ(ਬਣਤਰ ref_transaction *transaction ਅਣਵਰਤਿਆ,
			    ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ ਅਣਵਰਤਿਆ,
			    ਬਣਤਰ command_options *opts ਅਣਵਰਤਿਆ)
{
	ਜੇ (*ਅਗਲਾ != ਸਤਰ_termination)
		die("start: extra input: %s", ਅਗਲਾ);
	report_ok("start");
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_prepare(ਬਣਤਰ ref_transaction *transaction,
			      ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ ਅਣਵਰਤਿਆ,
			      ਬਣਤਰ command_options *opts ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf error = STRBUF_INIT;
	ਜੇ (*ਅਗਲਾ != ਸਤਰ_termination)
		die("prepare: extra input: %s", ਅਗਲਾ);
	ਜੇ (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_abort(ਬਣਤਰ ref_transaction *transaction,
			    ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ ਅਣਵਰਤਿਆ,
			    ਬਣਤਰ command_options *opts ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf error = STRBUF_INIT;
	ਜੇ (*ਅਗਲਾ != ਸਤਰ_termination)
		die("abort: extra input: %s", ਅਗਲਾ);
	ਜੇ (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

ਸਥਿਰ ਖਾਲੀ parse_cmd_commit(ਬਣਤਰ ref_transaction *transaction,
			     ਅਚਲ ਅੱਖਰ *ਅਗਲਾ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ ਅਣਵਰਤਿਆ,
			     ਬਣਤਰ command_options *opts ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf error = STRBUF_INIT;
	ਜੇ (*ਅਗਲਾ != ਸਤਰ_termination)
		die("commit: extra input: %s", ਅਗਲਾ);
	ਜੇ (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ਨਿਰਾ);

	report_ok("commit");
	ref_transaction_free(transaction);
}

ਗਿਣਤੀ update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ parse_cmd {
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਖਾਲੀ (*fn)(ਬਣਤਰ ref_transaction *, ਅਚਲ ਅੱਖਰ *, ਅਚਲ ਅੱਖਰ *,
		   ਬਣਤਰ command_options *);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਦਲੀਲਾਂ;
	ਗਿਣਤੀ update_refs_state state;
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
	{ "start",         parse_cmd_ਸ਼ੁਰੂ,         0, UPDATE_REFS_STARTED },
	{ "prepare",       parse_cmd_prepare,       0, UPDATE_REFS_PREPARED },
	{ "abort",         parse_cmd_abort,         0, UPDATE_REFS_CLOSED },
	{ "commit",        parse_cmd_commit,        0, UPDATE_REFS_CLOSED },
};

ਸਥਿਰ ਖਾਲੀ update_refs_stdin(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ strbuf ਨਿਵੇਸ਼ = STRBUF_INIT, err = STRBUF_INIT;
	ਗਿਣਤੀ update_refs_state state = UPDATE_REFS_OPEN;
	ਬਣਤਰ ref_transaction *transaction;
	ਪੂਰਨ i, j;

	ਬਣਤਰ command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	ਜੇ (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	ਜਦੋਂ (!strbuf_getwholeline(&ਨਿਵੇਸ਼, stdin, ਸਤਰ_termination)) {
		ਅਚਲ ਬਣਤਰ parse_cmd *cmd = ਨਿਰਾ;

		ਜੇ (*ਨਿਵੇਸ਼.buf == ਸਤਰ_termination)
			die("empty command in input");
		ਨਹੀਂਤਾਂ ਜੇ (ਖਾਲੀ_ਥਾਂ_ਹੈ(*ਨਿਵੇਸ਼.buf))
			die("whitespace before command: %s", ਨਿਵੇਸ਼.buf);

		ਲਈ (i = 0; i < ARRAY_ਮਾਪ(command); i++) {
			ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ = command[i].ਅਗੇਤਰ;
			ਅੱਖਰ c;

			ਜੇ (!starts_with(ਨਿਵੇਸ਼.buf, ਅਗੇਤਰ))
				ਜਾਰੀ;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].ਦਲੀਲਾਂ ? ' ' : ਸਤਰ_termination;
			ਜੇ (ਨਿਵੇਸ਼.buf[ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ)] != c)
				ਜਾਰੀ;

			cmd = &command[i];
			ਰੋਕੋ;
		}
		ਜੇ (!cmd)
			die("unknown command: %s", ਨਿਵੇਸ਼.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		ਲਈ (j = 1; ਸਤਰ_termination == '\0' && j < cmd->ਦਲੀਲਾਂ; j++)
			ਜੇ (strbuf_appendwholeline(&ਨਿਵੇਸ਼, stdin, ਸਤਰ_termination))
				ਰੋਕੋ;

		ਚੋਣ (state) {
		ਹਾਲਤ UPDATE_REFS_OPEN:
		ਹਾਲਤ UPDATE_REFS_STARTED:
			ਜੇ (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			ਜੇ (cmd->state >= state)
				state = cmd->state;
			ਰੋਕੋ;
		ਹਾਲਤ UPDATE_REFS_PREPARED:
			ਜੇ (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			ਰੋਕੋ;
		ਹਾਲਤ UPDATE_REFS_CLOSED:
			ਜੇ (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			ਜੇ (!transaction)
				die("%s", err.buf);

			ਰੋਕੋ;
		}

		cmd->fn(transaction, ਨਿਵੇਸ਼.buf + ਲੜੀ_ਲੰਬਾਈ(cmd->ਅਗੇਤਰ) + !!cmd->ਦਲੀਲਾਂ,
			ਨਿਵੇਸ਼.buf + ਨਿਵੇਸ਼.len, &opts);
	}

	ਚੋਣ (state) {
	ਹਾਲਤ UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		ਜੇ (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ਨਿਰਾ);
		ref_transaction_free(transaction);
		ਰੋਕੋ;
	ਹਾਲਤ UPDATE_REFS_STARTED:
	ਹਾਲਤ UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		ਜੇ (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		ਰੋਕੋ;
	ਹਾਲਤ UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		ਰੋਕੋ;
	}

	strbuf_release(&err);
	strbuf_release(&ਨਿਵੇਸ਼);
}

ਪੂਰਨ cmd_update_ref(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *refname, *oldval;
	ਬਣਤਰ object_id oid, oldoid;
	ਪੂਰਨ delete = 0, no_deref = 0, read_stdin = 0, ਸਿਰਾ_null = 0;
	ਪੂਰਨ create_reflog = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = 0;

	ਬਣਤਰ option options[] = {
		OPT_STRING( 'm', ਨਿਰਾ, &ਸੁਨੇਹਾ, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', ਨਿਰਾ, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', ਨਿਰਾ, &ਸਿਰਾ_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_ਸਿਰਾ(),
	};

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, git_update_ref_usage,
			     0);
	ਜੇ (ਸੁਨੇਹਾ && !*ਸੁਨੇਹਾ)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	ਜੇ (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	ਜੇ (read_stdin) {
		ਜੇ (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		ਜੇ (ਸਿਰਾ_null)
			ਸਤਰ_termination = '\0';
		update_refs_stdin(flags);
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	ਜੇ (ਸਿਰਾ_null)
		usage_with_options(git_update_ref_usage, options);

	ਜੇ (delete) {
		ਜੇ (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} ਨਹੀਂਤਾਂ {
		ਅਚਲ ਅੱਖਰ *ਮੁੱਲ;
		ਜੇ (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		ਮੁੱਲ = argv[1];
		oldval = argv[2];
		ਜੇ (repo_get_oid_with_flags(the_repository, ਮੁੱਲ, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", ਮੁੱਲ);
	}

	ਜੇ (oldval) {
		ਜੇ (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		ਨਹੀਂਤਾਂ ਜੇ (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	ਜੇ (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		ਵਾਪਸ refs_delete_ref(get_main_ref_store(the_repository),
				       ਸੁਨੇਹਾ, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : ਨਿਰਾ,
				       default_flags);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ refs_update_ref(get_main_ref_store(the_repository),
				       ਸੁਨੇਹਾ, refname, &oid,
				       oldval ? &oldoid : ਨਿਰਾ,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
