#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_update_ref_usage[] = {
	N_("git update-ref [<options>] -d <refname> [<old-oid>]"),
	N_("git update-ref [<options>]    <refname> <new-oid> [<old-oid>]"),
	N_("git update-ref [<options>] --stdin [-z] [--batch-updates]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸಾಲು_termination = '\n';
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ update_flags;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ default_flags;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ create_reflog_flag;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ;

ರಚನೆ command_options {
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
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *parse_arg(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ರಚನೆ strbuf *arg)
{
	ಆದರೆ (*ಮುಂದಿನ == '"') {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *orig = ಮುಂದಿನ;

		ಆದರೆ (unquote_c_style(arg, ಮುಂದಿನ, &ಮುಂದಿನ))
			die("badly quoted argument: %s", orig);
		ಆದರೆ (*ಮುಂದಿನ && !ಖಾಲಿಯೇ(*ಮುಂದಿನ))
			die("unexpected character after quoted argument: %s", orig);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆಗ (*ಮುಂದಿನ && !ಖಾಲಿಯೇ(*ಮುಂದಿನ))
			strbuf_addch(arg, *ಮುಂದಿನ++);
	}

	ಹಿಂತಿರುಗಿ ಮುಂದಿನ;
}

/*
 * Parse the reference name immediately after "command SP".  If not
 * -z, then handle C-quoting.  Return a pointer to a newly allocated
 * string containing the name of the reference, or NULL if there was
 * an error.  Update *next to point at the character that terminates
 * the argument.  Die if C-quoting is malformed or the reference name
 * is invalid.
 */
ಸ್ಥಿರ ಅಕ್ಷರ *parse_refname(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮುಂದಿನ)
{
	ರಚನೆ strbuf ref = STRBUF_INIT;

	ಆದರೆ (ಸಾಲು_termination) {
		/* Without -z, use the next argument */
		*ಮುಂದಿನ = parse_arg(*ಮುಂದಿನ, &ref);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&ref, *ಮುಂದಿನ);
		*ಮುಂದಿನ += ref.len;
	}

	ಆದರೆ (!ref.len) {
		strbuf_release(&ref);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ಆದರೆ (check_refname_format(ref.buf, REFNAME_ALLOW_ONELEVEL))
		die("invalid ref format: %s", ref.buf);

	ಹಿಂತಿರುಗಿ strbuf_detach(&ref, ಖಾಲಿ);
}

/*
 * Wrapper around parse_refname which skips the next delimiter.
 */
ಸ್ಥಿರ ಅಕ್ಷರ *parse_ಮುಂದಿನ_refname(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮುಂದಿನ)
{
	ಆದರೆ (ಸಾಲು_termination) {
		/* Without -z, consume SP and use next argument */
		ಆದರೆ (!**ಮುಂದಿನ || **ಮುಂದಿನ == ಸಾಲು_termination)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		ಆದರೆ (**ಮುಂದಿನ != ' ')
			die("expected SP but got: %s", *ಮುಂದಿನ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* With -z, read the next NUL-terminated line */
		ಆದರೆ (**ಮುಂದಿನ)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}
	/* Skip the delimiter */
	(*ಮುಂದಿನ)++;

	ಹಿಂತಿರುಗಿ parse_refname(ಮುಂದಿನ);
}

/*
 * Wrapper around parse_arg which skips the next delimiter.
 */
ಸ್ಥಿರ ಅಕ್ಷರ *parse_ಮುಂದಿನ_arg(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮುಂದಿನ)
{
	ರಚನೆ strbuf arg = STRBUF_INIT;

	ಆದರೆ (ಸಾಲು_termination) {
		/* Without -z, consume SP and use next argument */
		ಆದರೆ (!**ಮುಂದಿನ || **ಮುಂದಿನ == ಸಾಲು_termination)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		ಆದರೆ (**ಮುಂದಿನ != ' ')
			die("expected SP but got: %s", *ಮುಂದಿನ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* With -z, read the next NUL-terminated line */
		ಆದರೆ (**ಮುಂದಿನ)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}
	/* Skip the delimiter */
	(*ಮುಂದಿನ)++;

	ಆದರೆ (ಸಾಲು_termination) {
		/* Without -z, use the next argument */
		*ಮುಂದಿನ = parse_arg(*ಮುಂದಿನ, &arg);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* With -z, use everything up to the next NUL */
		strbuf_addstr(&arg, *ಮುಂದಿನ);
		*ಮುಂದಿನ += arg.len;
	}

	ಆದರೆ (arg.len)
		ಹಿಂತಿರುಗಿ strbuf_detach(&arg, ಖಾಲಿ);

	strbuf_release(&arg);
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/*
 * The value being parsed is <old-oid> (as opposed to <new-oid>; the
 * difference affects which error messages are generated):
 */
#ವ್ಯಾಖ್ಯಾನಿಸು PARSE_SHA1_OLD 0x01

/*
 * For backwards compatibility, accept an empty string for update's
 * <new-oid> in binary mode to be equivalent to specifying zeros.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು PARSE_SHA1_ALLOW_ಖಾಲಿಗಳು 0x02

/*
 * Parse an argument separator followed by the next argument, if any.
 * If there is an argument, convert it to a SHA-1, write it to sha1,
 * set *next to point at the character terminating the argument, and
 * return 0.  If there is no argument at all (not even the empty
 * string), return 1 and leave *next unchanged.  If the value is
 * provided but cannot be converted to a SHA-1, die.  flags can
 * include PARSE_SHA1_OLD and/or PARSE_SHA1_ALLOW_EMPTY.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_ಮುಂದಿನ_oid(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ,
			  ರಚನೆ object_id *oid,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *command, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
			  ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ strbuf arg = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಆದರೆ (*ಮುಂದಿನ == ಕೊನೆ)
		ಹೋಗು eof;

	ಆದರೆ (ಸಾಲು_termination) {
		/* Without -z, consume SP and use next argument */
		ಆದರೆ (!**ಮುಂದಿನ || **ಮುಂದಿನ == ಸಾಲು_termination)
			ಹಿಂತಿರುಗಿ 1;
		ಆದರೆ (**ಮುಂದಿನ != ' ')
			die("%s %s: expected SP but got: %s",
			    command, refname, *ಮುಂದಿನ);
		(*ಮುಂದಿನ)++;
		*ಮುಂದಿನ = parse_arg(*ಮುಂದಿನ, &arg);
		ಆದರೆ (arg.len) {
			ಆದರೆ (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				ಹೋಗು invalid;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* Without -z, an empty value means all zeros: */
			oidclr(oid, the_repository->hash_algo);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* With -z, read the next NUL-terminated line */
		ಆದರೆ (**ಮುಂದಿನ)
			die("%s %s: expected NUL but got: %s",
			    command, refname, *ಮುಂದಿನ);
		(*ಮುಂದಿನ)++;
		ಆದರೆ (*ಮುಂದಿನ == ಕೊನೆ)
			ಹೋಗು eof;
		strbuf_addstr(&arg, *ಮುಂದಿನ);
		*ಮುಂದಿನ += arg.len;

		ಆದರೆ (arg.len) {
			ಆದರೆ (repo_get_oid_with_flags(the_repository, arg.buf, oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				ಹೋಗು invalid;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags & PARSE_SHA1_ALLOW_ಖಾಲಿಗಳು) {
			/* With -z, treat an empty value as all zeros: */
			warning("%s %s: missing <new-oid>, treating as zero",
				command, refname);
			oidclr(oid, the_repository->hash_algo);
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * With -z, an empty non-required value means
			 * unspecified:
			 */
			ret = 1;
		}
	}

	strbuf_release(&arg);

	ಹಿಂತಿರುಗಿ ret;

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

ಸ್ಥಿರ ಶೂನ್ಯ print_rejected_refs(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
				ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_oid,
				ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *new_oid,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old_ಗುರಿ,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_ಗುರಿ,
				ಎಣಿಕೆ ref_transaction_error err,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *details,
				ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;

	ಆದರೆ (details && *details)
		error("%s", details);

	strbuf_addf(&sb, "rejected %s %s %s %s\n", refname,
		    new_oid ? oid_ಗಮ್ಯ_hex(new_oid) : new_ಗುರಿ,
		    old_oid ? oid_ಗಮ್ಯ_hex(old_oid) : old_ಗುರಿ,
		    ref_transaction_error_ಸಂದೇಶ(err));

	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
}

/*
 * Handle transaction errors. If we're using batches updates, we want to only
 * die for generic errors and print the remaining to the user.
 */
ಸ್ಥಿರ ಶೂನ್ಯ handle_ref_transaction_error(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
					 ರಚನೆ object_id *new_oid,
					 ರಚನೆ object_id *old_oid,
					 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_ಗುರಿ,
					 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old_ಗುರಿ,
					 ಎಣಿಕೆ ref_transaction_error tx_err,
					 ರಚನೆ strbuf *err,
					 ರಚನೆ command_options *opts)
{
	ಆದರೆ (!tx_err)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (tx_err != REF_TRANSACTION_ERROR_GENERIC && opts->allow_update_failures) {
		print_rejected_refs(refname, old_oid, new_oid, old_ಗುರಿ,
				    new_ಗುರಿ, tx_err, err->buf, ಖಾಲಿ);
		ಹಿಂತಿರುಗಿ;
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

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_update(ರಚನೆ ref_transaction *transaction,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ,
			     ರಚನೆ command_options *opts)
{
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಅಕ್ಷರ *refname;
	ರಚನೆ object_id new_oid, old_oid;
	ಎಣಿಕೆ ref_transaction_error tx_err;
	ಪೂರ್ಣಾಂಕ have_old;

	refname = parse_refname(&ಮುಂದಿನ);
	ಆದರೆ (!refname)
		die("update: missing <ref>");

	ಆದರೆ (parse_ಮುಂದಿನ_oid(&ಮುಂದಿನ, ಕೊನೆ, &new_oid, "update", refname,
			   PARSE_SHA1_ALLOW_ಖಾಲಿಗಳು))
		die("update %s: missing <new-oid>", refname);

	have_old = !parse_ಮುಂದಿನ_oid(&ಮುಂದಿನ, ಕೊನೆ, &old_oid, "update", refname,
				   PARSE_SHA1_OLD);

	ಆದರೆ (*ಮುಂದಿನ != ಸಾಲು_termination)
		die("update %s: extra input: %s", refname, ಮುಂದಿನ);

	tx_err = ref_transaction_update(transaction, refname,
					&new_oid, have_old ? &old_oid : ಖಾಲಿ,
					ಖಾಲಿ, ಖಾಲಿ,
					update_flags | create_reflog_flag,
					ಸಂದೇಶ, &err);
	handle_ref_transaction_error(refname, &new_oid, have_old ? &old_oid : ಖಾಲಿ,
				     ಖಾಲಿ, ಖಾಲಿ, tx_err, &err, opts);


	update_flags = default_flags;
	ಬಿಡುಗಡೆ(refname);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_symref_update(ರಚನೆ ref_transaction *transaction,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ ಬಳಸದ,
				    ರಚನೆ command_options *opts)
{
	ಅಕ್ಷರ *refname, *new_ಗುರಿ, *old_arg;
	ಎಣಿಕೆ ref_transaction_error tx_err;
	ಅಕ್ಷರ *old_ಗುರಿ = ಖಾಲಿ;
	ರಚನೆ strbuf err = STRBUF_INIT;
	ರಚನೆ object_id old_oid;
	ಪೂರ್ಣಾಂಕ have_old_oid = 0;

	refname = parse_refname(&ಮುಂದಿನ);
	ಆದರೆ (!refname)
		die("symref-update: missing <ref>");

	new_ಗುರಿ = parse_ಮುಂದಿನ_refname(&ಮುಂದಿನ);
	ಆದರೆ (!new_ಗುರಿ)
		die("symref-update %s: missing <new-target>", refname);

	old_arg = parse_ಮುಂದಿನ_arg(&ಮುಂದಿನ);
	ಆದರೆ (old_arg) {
		old_ಗುರಿ = parse_ಮುಂದಿನ_arg(&ಮುಂದಿನ);
		ಆದರೆ (!old_ಗುರಿ)
			die("symref-update %s: expected old value", refname);

		ಆದರೆ (!strcmp(old_arg, "oid")) {
			ಆದರೆ (repo_get_oid_with_flags(the_repository, old_ಗುರಿ, &old_oid,
						    GET_OID_SKIP_AMBIGUITY_CHECK))
				die("symref-update %s: invalid oid: %s", refname, old_ಗುರಿ);

			have_old_oid = 1;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(old_arg, "ref")) {
			ಆದರೆ (check_refname_format(old_ಗುರಿ, REFNAME_ALLOW_ONELEVEL))
				die("symref-update %s: invalid ref: %s", refname, old_ಗುರಿ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			die("symref-update %s: invalid arg '%s' for old value", refname, old_arg);
		}
	}

	ಆದರೆ (*ಮುಂದಿನ != ಸಾಲು_termination)
		die("symref-update %s: extra input: %s", refname, ಮುಂದಿನ);

	tx_err = ref_transaction_update(transaction, refname, ಖಾಲಿ,
					have_old_oid ? &old_oid : ಖಾಲಿ,
					new_ಗುರಿ,
					have_old_oid ? ಖಾಲಿ : old_ಗುರಿ,
					update_flags | create_reflog_flag,
					ಸಂದೇಶ, &err);
	handle_ref_transaction_error(refname, ಖಾಲಿ, have_old_oid ? &old_oid : ಖಾಲಿ,
				     new_ಗುರಿ, have_old_oid ? ಖಾಲಿ : old_ಗುರಿ,
				     tx_err, &err, opts);

	update_flags = default_flags;
	ಬಿಡುಗಡೆ(refname);
	ಬಿಡುಗಡೆ(old_arg);
	ಬಿಡುಗಡೆ(old_ಗುರಿ);
	ಬಿಡುಗಡೆ(new_ಗುರಿ);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_create(ರಚನೆ ref_transaction *transaction,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ,
			     ರಚನೆ command_options *opts)
{
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಅಕ್ಷರ *refname;
	ರಚನೆ object_id new_oid;
	ಎಣಿಕೆ ref_transaction_error tx_err;

	refname = parse_refname(&ಮುಂದಿನ);
	ಆದರೆ (!refname)
		die("create: missing <ref>");

	ಆದರೆ (parse_ಮುಂದಿನ_oid(&ಮುಂದಿನ, ಕೊನೆ, &new_oid, "create", refname, 0))
		die("create %s: missing <new-oid>", refname);

	ಆದರೆ (is_null_oid(&new_oid))
		die("create %s: zero <new-oid>", refname);

	ಆದರೆ (*ಮುಂದಿನ != ಸಾಲು_termination)
		die("create %s: extra input: %s", refname, ಮುಂದಿನ);

	tx_err = ref_transaction_create(transaction, refname, &new_oid, ಖಾಲಿ,
					update_flags | create_reflog_flag,
					ಸಂದೇಶ, &err);
	handle_ref_transaction_error(refname, &new_oid, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, tx_err,
				     &err, opts);

	update_flags = default_flags;
	ಬಿಡುಗಡೆ(refname);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_symref_create(ರಚನೆ ref_transaction *transaction,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ ಬಳಸದ,
				    ರಚನೆ command_options *opts)
{
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಅಕ್ಷರ *refname, *new_ಗುರಿ;
	ಎಣಿಕೆ ref_transaction_error tx_err;

	refname = parse_refname(&ಮುಂದಿನ);
	ಆದರೆ (!refname)
		die("symref-create: missing <ref>");

	new_ಗುರಿ = parse_ಮುಂದಿನ_refname(&ಮುಂದಿನ);
	ಆದರೆ (!new_ಗುರಿ)
		die("symref-create %s: missing <new-target>", refname);

	ಆದರೆ (*ಮುಂದಿನ != ಸಾಲು_termination)
		die("symref-create %s: extra input: %s", refname, ಮುಂದಿನ);

	tx_err = ref_transaction_create(transaction, refname, ಖಾಲಿ, new_ಗುರಿ,
					update_flags | create_reflog_flag,
					ಸಂದೇಶ, &err);
	handle_ref_transaction_error(refname, ಖಾಲಿ, ಖಾಲಿ, new_ಗುರಿ, ಖಾಲಿ,
				     tx_err, &err, opts);

	update_flags = default_flags;
	ಬಿಡುಗಡೆ(refname);
	ಬಿಡುಗಡೆ(new_ಗುರಿ);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_delete(ರಚನೆ ref_transaction *transaction,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ,
			     ರಚನೆ command_options *opts ಬಳಸದ)
{
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಅಕ್ಷರ *refname;
	ರಚನೆ object_id old_oid;
	ಪೂರ್ಣಾಂಕ have_old;

	refname = parse_refname(&ಮುಂದಿನ);
	ಆದರೆ (!refname)
		die("delete: missing <ref>");

	ಆದರೆ (parse_ಮುಂದಿನ_oid(&ಮುಂದಿನ, ಕೊನೆ, &old_oid, "delete", refname,
			   PARSE_SHA1_OLD)) {
		have_old = 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (is_null_oid(&old_oid))
			die("delete %s: zero <old-oid>", refname);
		have_old = 1;
	}

	ಆದರೆ (*ಮುಂದಿನ != ಸಾಲು_termination)
		die("delete %s: extra input: %s", refname, ಮುಂದಿನ);

	ಆದರೆ (ref_transaction_delete(transaction, refname,
				   have_old ? &old_oid : ಖಾಲಿ,
				   ಖಾಲಿ, update_flags, ಸಂದೇಶ, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ಬಿಡುಗಡೆ(refname);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_symref_delete(ರಚನೆ ref_transaction *transaction,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ ಬಳಸದ,
				    ರಚನೆ command_options *opts ಬಳಸದ)
{
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಅಕ್ಷರ *refname, *old_ಗುರಿ;

	ಆದರೆ (!(update_flags & REF_NO_DEREF))
		die("symref-delete: cannot operate with deref mode");

	refname = parse_refname(&ಮುಂದಿನ);
	ಆದರೆ (!refname)
		die("symref-delete: missing <ref>");

	old_ಗುರಿ = parse_ಮುಂದಿನ_refname(&ಮುಂದಿನ);

	ಆದರೆ (*ಮುಂದಿನ != ಸಾಲು_termination)
		die("symref-delete %s: extra input: %s", refname, ಮುಂದಿನ);

	ಆದರೆ (ref_transaction_delete(transaction, refname, ಖಾಲಿ,
				   old_ಗುರಿ, update_flags, ಸಂದೇಶ, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ಬಿಡುಗಡೆ(refname);
	ಬಿಡುಗಡೆ(old_ಗುರಿ);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_verify(ರಚನೆ ref_transaction *transaction,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ,
			     ರಚನೆ command_options *opts ಬಳಸದ)
{
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಅಕ್ಷರ *refname;
	ರಚನೆ object_id old_oid;

	refname = parse_refname(&ಮುಂದಿನ);
	ಆದರೆ (!refname)
		die("verify: missing <ref>");

	ಆದರೆ (parse_ಮುಂದಿನ_oid(&ಮುಂದಿನ, ಕೊನೆ, &old_oid, "verify", refname,
			   PARSE_SHA1_OLD))
		oidclr(&old_oid, the_repository->hash_algo);

	ಆದರೆ (*ಮುಂದಿನ != ಸಾಲು_termination)
		die("verify %s: extra input: %s", refname, ಮುಂದಿನ);

	ಆದರೆ (ref_transaction_verify(transaction, refname, &old_oid,
				   ಖಾಲಿ, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ಬಿಡುಗಡೆ(refname);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_symref_verify(ರಚನೆ ref_transaction *transaction,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ ಬಳಸದ,
				    ರಚನೆ command_options *opts ಬಳಸದ)
{
	ರಚನೆ strbuf err = STRBUF_INIT;
	ರಚನೆ object_id old_oid;
	ಅಕ್ಷರ *refname, *old_ಗುರಿ;

	ಆದರೆ (!(update_flags & REF_NO_DEREF))
		die("symref-verify: cannot operate with deref mode");

	refname = parse_refname(&ಮುಂದಿನ);
	ಆದರೆ (!refname)
		die("symref-verify: missing <ref>");

	/*
	 * old_ref is optional, if not provided, we need to ensure that the
	 * ref doesn't exist.
	 */
	old_ಗುರಿ = parse_ಮುಂದಿನ_refname(&ಮುಂದಿನ);
	ಆದರೆ (!old_ಗುರಿ)
		oidcpy(&old_oid, null_oid(the_hash_algo));

	ಆದರೆ (*ಮುಂದಿನ != ಸಾಲು_termination)
		die("symref-verify %s: extra input: %s", refname, ಮುಂದಿನ);

	ಆದರೆ (ref_transaction_verify(transaction, refname,
				   old_ಗುರಿ ? ಖಾಲಿ : &old_oid,
				   old_ಗುರಿ, update_flags, &err))
		die("%s", err.buf);

	update_flags = default_flags;
	ಬಿಡುಗಡೆ(refname);
	ಬಿಡುಗಡೆ(old_ಗುರಿ);
	strbuf_release(&err);
}

ಸ್ಥಿರ ಶೂನ್ಯ report_ok(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *command)
{
	fprintf(stdout, "%s: ok\n", command);
	fflush(stdout);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_option(ರಚನೆ ref_transaction *transaction ಬಳಸದ,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ ಬಳಸದ,
			     ರಚನೆ command_options *opts ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rest;
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ಮುಂದಿನ, "no-deref", &rest) && *rest == ಸಾಲು_termination)
		update_flags |= REF_NO_DEREF;
	ಇಲ್ಲದಿದ್ದರೆ
		die("option unknown: %s", ಮುಂದಿನ);
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_ಆರಂಭ(ರಚನೆ ref_transaction *transaction ಬಳಸದ,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ ಬಳಸದ,
			    ರಚನೆ command_options *opts ಬಳಸದ)
{
	ಆದರೆ (*ಮುಂದಿನ != ಸಾಲು_termination)
		die("start: extra input: %s", ಮುಂದಿನ);
	report_ok("start");
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_prepare(ರಚನೆ ref_transaction *transaction,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ ಬಳಸದ,
			      ರಚನೆ command_options *opts ಬಳಸದ)
{
	ರಚನೆ strbuf error = STRBUF_INIT;
	ಆದರೆ (*ಮುಂದಿನ != ಸಾಲು_termination)
		die("prepare: extra input: %s", ಮುಂದಿನ);
	ಆದರೆ (ref_transaction_prepare(transaction, &error))
		die("prepare: %s", error.buf);
	report_ok("prepare");
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_abort(ರಚನೆ ref_transaction *transaction,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ ಬಳಸದ,
			    ರಚನೆ command_options *opts ಬಳಸದ)
{
	ರಚನೆ strbuf error = STRBUF_INIT;
	ಆದರೆ (*ಮುಂದಿನ != ಸಾಲು_termination)
		die("abort: extra input: %s", ಮುಂದಿನ);
	ಆದರೆ (ref_transaction_abort(transaction, &error))
		die("abort: %s", error.buf);
	report_ok("abort");
}

ಸ್ಥಿರ ಶೂನ್ಯ parse_cmd_commit(ರಚನೆ ref_transaction *transaction,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮುಂದಿನ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ ಬಳಸದ,
			     ರಚನೆ command_options *opts ಬಳಸದ)
{
	ರಚನೆ strbuf error = STRBUF_INIT;
	ಆದರೆ (*ಮುಂದಿನ != ಸಾಲು_termination)
		die("commit: extra input: %s", ಮುಂದಿನ);
	ಆದರೆ (ref_transaction_commit(transaction, &error))
		die("commit: %s", error.buf);

	ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ಖಾಲಿ);

	report_ok("commit");
	ref_transaction_free(transaction);
}

ಎಣಿಕೆ update_refs_state {
	/* Non-transactional state open for updates. */
	UPDATE_REFS_OPEN,
	/* A transaction has been started. */
	UPDATE_REFS_STARTED,
	/* References are locked and ready for commit */
	UPDATE_REFS_PREPARED,
	/* Transaction has been committed or closed. */
	UPDATE_REFS_CLOSED,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ parse_cmd {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಶೂನ್ಯ (*fn)(ರಚನೆ ref_transaction *, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *,
		   ರಚನೆ command_options *);
	ಚಿಹ್ನೆರಹಿತ ವಾದಗಳು;
	ಎಣಿಕೆ update_refs_state state;
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
	{ "start",         parse_cmd_ಆರಂಭ,         0, UPDATE_REFS_STARTED },
	{ "prepare",       parse_cmd_prepare,       0, UPDATE_REFS_PREPARED },
	{ "abort",         parse_cmd_abort,         0, UPDATE_REFS_CLOSED },
	{ "commit",        parse_cmd_commit,        0, UPDATE_REFS_CLOSED },
};

ಸ್ಥಿರ ಶೂನ್ಯ update_refs_stdin(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ strbuf ಒಳಹರಿವು = STRBUF_INIT, err = STRBUF_INIT;
	ಎಣಿಕೆ update_refs_state state = UPDATE_REFS_OPEN;
	ರಚನೆ ref_transaction *transaction;
	ಪೂರ್ಣಾಂಕ i, j;

	ರಚನೆ command_options opts = {
		.allow_update_failures = flags & REF_TRANSACTION_ALLOW_FAILURE,
	};

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  flags, &err);
	ಆದರೆ (!transaction)
		die("%s", err.buf);

	/* Read each line dispatch its command */
	ಆಗ (!strbuf_getwholeline(&ಒಳಹರಿವು, stdin, ಸಾಲು_termination)) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ parse_cmd *cmd = ಖಾಲಿ;

		ಆದರೆ (*ಒಳಹರಿವು.buf == ಸಾಲು_termination)
			die("empty command in input");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಖಾಲಿಯೇ(*ಒಳಹರಿವು.buf))
			die("whitespace before command: %s", ಒಳಹರಿವು.buf);

		ಗಾಗಿ (i = 0; i < ARRAY_ಗಾತ್ರ(command); i++) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = command[i].ಪೂರ್ವಪ್ರತ್ಯಯ;
			ಅಕ್ಷರ c;

			ಆದರೆ (!starts_with(ಒಳಹರಿವು.buf, ಪೂರ್ವಪ್ರತ್ಯಯ))
				ಮುಂದುವರಿ;

			/*
			 * If the command has arguments, verify that it's
			 * followed by a space. Otherwise, it shall be followed
			 * by a line terminator.
			 */
			c = command[i].ವಾದಗಳು ? ' ' : ಸಾಲು_termination;
			ಆದರೆ (ಒಳಹರಿವು.buf[ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ)] != c)
				ಮುಂದುವರಿ;

			cmd = &command[i];
			ನಿಲ್ಲಿಸು;
		}
		ಆದರೆ (!cmd)
			die("unknown command: %s", ಒಳಹರಿವು.buf);

		/*
		 * Read additional arguments if NUL-terminated. Do not raise an
		 * error in case there is an early EOF to let the command
		 * handle missing arguments with a proper error message.
		 */
		ಗಾಗಿ (j = 1; ಸಾಲು_termination == '\0' && j < cmd->ವಾದಗಳು; j++)
			ಆದರೆ (strbuf_appendwholeline(&ಒಳಹರಿವು, stdin, ಸಾಲು_termination))
				ನಿಲ್ಲಿಸು;

		ಆಯ್ಕೆ (state) {
		ಸ್ಥಿತಿ UPDATE_REFS_OPEN:
		ಸ್ಥಿತಿ UPDATE_REFS_STARTED:
			ಆದರೆ (state == UPDATE_REFS_STARTED && cmd->state == UPDATE_REFS_STARTED)
				die("cannot restart ongoing transaction");
			/* Do not downgrade a transaction to a non-transaction. */
			ಆದರೆ (cmd->state >= state)
				state = cmd->state;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ UPDATE_REFS_PREPARED:
			ಆದರೆ (cmd->state != UPDATE_REFS_CLOSED)
				die("prepared transactions can only be closed");
			state = cmd->state;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ UPDATE_REFS_CLOSED:
			ಆದರೆ (cmd->state != UPDATE_REFS_STARTED)
				die("transaction is closed");

			/*
			 * Open a new transaction if we're currently closed and
			 * get a "start".
			 */
			state = cmd->state;
			transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
								  flags, &err);
			ಆದರೆ (!transaction)
				die("%s", err.buf);

			ನಿಲ್ಲಿಸು;
		}

		cmd->fn(transaction, ಒಳಹರಿವು.buf + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(cmd->ಪೂರ್ವಪ್ರತ್ಯಯ) + !!cmd->ವಾದಗಳು,
			ಒಳಹರಿವು.buf + ಒಳಹರಿವು.len, &opts);
	}

	ಆಯ್ಕೆ (state) {
	ಸ್ಥಿತಿ UPDATE_REFS_OPEN:
		/* Commit by default if no transaction was requested. */
		ಆದರೆ (ref_transaction_commit(transaction, &err))
			die("%s", err.buf);
		ref_transaction_for_each_rejected_update(transaction,
						 print_rejected_refs, ಖಾಲಿ);
		ref_transaction_free(transaction);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ UPDATE_REFS_STARTED:
	ಸ್ಥಿತಿ UPDATE_REFS_PREPARED:
		/* If using a transaction, we want to abort it. */
		ಆದರೆ (ref_transaction_abort(transaction, &err))
			die("%s", err.buf);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ UPDATE_REFS_CLOSED:
		/* Otherwise no need to do anything, the transaction was closed already. */
		ನಿಲ್ಲಿಸು;
	}

	strbuf_release(&err);
	strbuf_release(&ಒಳಹರಿವು);
}

ಪೂರ್ಣಾಂಕ cmd_update_ref(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname, *oldval;
	ರಚನೆ object_id oid, oldoid;
	ಪೂರ್ಣಾಂಕ delete = 0, no_deref = 0, read_stdin = 0, ಕೊನೆ_null = 0;
	ಪೂರ್ಣಾಂಕ create_reflog = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = 0;

	ರಚನೆ option options[] = {
		OPT_STRING( 'm', ಖಾಲಿ, &ಸಂದೇಶ, N_("reason"), N_("reason of the update")),
		OPT_BOOL('d', ಖಾಲಿ, &delete, N_("delete the reference")),
		OPT_BOOL( 0 , "no-deref", &no_deref,
					N_("update <refname> not the one it points to")),
		OPT_BOOL('z', ಖಾಲಿ, &ಕೊನೆ_null, N_("stdin has NUL-terminated arguments")),
		OPT_BOOL( 0 , "stdin", &read_stdin, N_("read updates from stdin")),
		OPT_BOOL( 0 , "create-reflog", &create_reflog, N_("create a reflog")),
		OPT_ಬಿಟ್('0', "batch-updates", &flags, N_("batch reference updates"),
			REF_TRANSACTION_ALLOW_FAILURE),
		OPT_ಕೊನೆ(),
	};

	repo_config(the_repository, git_default_config, ಖಾಲಿ);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_update_ref_usage,
			     0);
	ಆದರೆ (ಸಂದೇಶ && !*ಸಂದೇಶ)
		die("Refusing to perform update with empty message.");

	create_reflog_flag = create_reflog ? REF_FORCE_CREATE_REFLOG : 0;

	ಆದರೆ (no_deref) {
		default_flags = REF_NO_DEREF;
		update_flags = default_flags;
	}

	ಆದರೆ (read_stdin) {
		ಆದರೆ (delete || argc > 0)
			usage_with_options(git_update_ref_usage, options);
		ಆದರೆ (ಕೊನೆ_null)
			ಸಾಲು_termination = '\0';
		update_refs_stdin(flags);
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags & REF_TRANSACTION_ALLOW_FAILURE) {
		die("--batch-updates can only be used with --stdin");
	}

	ಆದರೆ (ಕೊನೆ_null)
		usage_with_options(git_update_ref_usage, options);

	ಆದರೆ (delete) {
		ಆದರೆ (argc < 1 || argc > 2)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		oldval = argv[1];
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ;
		ಆದರೆ (argc < 2 || argc > 3)
			usage_with_options(git_update_ref_usage, options);
		refname = argv[0];
		ಮೌಲ್ಯ = argv[1];
		oldval = argv[2];
		ಆದರೆ (repo_get_oid_with_flags(the_repository, ಮೌಲ್ಯ, &oid,
					    GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid SHA1", ಮೌಲ್ಯ);
	}

	ಆದರೆ (oldval) {
		ಆದರೆ (!*oldval)
			/*
			 * The empty string implies that the reference
			 * must not already exist:
			 */
			oidclr(&oldoid, the_repository->hash_algo);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (repo_get_oid_with_flags(the_repository, oldval, &oldoid,
						 GET_OID_SKIP_AMBIGUITY_CHECK))
			die("%s: not a valid old SHA1", oldval);
	}

	ಆದರೆ (delete)
		/*
		 * For purposes of backwards compatibility, we treat
		 * NULL_SHA1 as "don't care" here:
		 */
		ಹಿಂತಿರುಗಿ refs_delete_ref(get_main_ref_store(the_repository),
				       ಸಂದೇಶ, refname,
				       (oldval && !is_null_oid(&oldoid)) ? &oldoid : ಖಾಲಿ,
				       default_flags);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ refs_update_ref(get_main_ref_store(the_repository),
				       ಸಂದೇಶ, refname, &oid,
				       oldval ? &oldoid : ಖಾಲಿ,
				       default_flags | create_reflog_flag,
				       UPDATE_REFS_DIE_ON_ERR);
}
