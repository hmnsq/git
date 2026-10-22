/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "bisect.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "repo-settings.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "repository.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "split-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "shallow.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file-convert.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DO_REVS		1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DO_NOREV	2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DO_FLAGS	4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DO_NONFLAGS	8
ਸਥਿਰ ਪੂਰਨ filter = ~0;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *def;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਾਧਾਰਨ 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REVERSED 1
ਸਥਿਰ ਪੂਰਨ show_type = ਸਾਧਾਰਨ;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SHOW_SYMBOLIC_ASIS 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SHOW_SYMBOLIC_ਭਰਿਆ 2
ਸਥਿਰ ਪੂਰਨ symbolic;
ਸਥਿਰ ਪੂਰਨ abbrev;
ਸਥਿਰ ਪੂਰਨ abbrev_ref;
ਸਥਿਰ ਪੂਰਨ abbrev_ref_strict;
ਸਥਿਰ ਪੂਰਨ output_sq;

ਸਥਿਰ ਪੂਰਨ stuck_long;
ਸਥਿਰ ਬਣਤਰ ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
ਸਥਿਰ ਪੂਰਨ is_rev_ਦਲੀਲ(ਅਚਲ ਅੱਖਰ *arg)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *rev_ਦਲੀਲਾਂ[] = {
		"--all",
		"--bisect",
		"--dense",
		"--branches=",
		"--branches",
		"--header",
		"--ignore-missing",
		"--max-age=",
		"--max-count=",
		"--min-age=",
		"--no-merges",
		"--min-parents=",
		"--no-min-parents",
		"--max-parents=",
		"--no-max-parents",
		"--objects",
		"--objects-edge",
		"--parents",
		"--pretty",
		"--remotes=",
		"--remotes",
		"--glob=",
		"--sparse",
		"--tags=",
		"--tags",
		"--topo-order",
		"--date-order",
		"--unpacked",
		ਨਿਰਾ
	};
	ਅਚਲ ਅੱਖਰ **p = rev_ਦਲੀਲਾਂ;

	/* accept -<digit>, like traditional "head" */
	ਜੇ ((*arg == '-') && ਅੰਕ_ਹੈ(arg[1]))
		ਵਾਪਸ 1;

	ਲਈ (;;) {
		ਅਚਲ ਅੱਖਰ *str = *p++;
		ਪੂਰਨ len;
		ਜੇ (!str)
			ਵਾਪਸ 0;
		len = ਲੜੀ_ਲੰਬਾਈ(str);
		ਜੇ (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !ਲੜੀ_ਤੁਲਨਾ(arg, str, len)))
			ਵਾਪਸ 1;
	}
}

/* Output argument as a string, either SQ or normal */
ਸਥਿਰ ਖਾਲੀ show(ਅਚਲ ਅੱਖਰ *arg)
{
	ਜੇ (output_sq) {
		ਪੂਰਨ sq = '\'', ch;

		ਅੱਖਰ_ਲਿਖੋ(sq);
		ਜਦੋਂ ((ch = *arg++)) {
			ਜੇ (ch == sq)
				fputs("'\\'", stdout);
			ਅੱਖਰ_ਲਿਖੋ(ch);
		}
		ਅੱਖਰ_ਲਿਖੋ(sq);
		ਅੱਖਰ_ਲਿਖੋ(' ');
	}
	ਨਹੀਂਤਾਂ
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
ਸਥਿਰ ਖਾਲੀ show_with_type(ਪੂਰਨ type, ਅਚਲ ਅੱਖਰ *arg)
{
	ਜੇ (type != show_type)
		ਅੱਖਰ_ਲਿਖੋ('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
ਸਥਿਰ ਖਾਲੀ show_rev(ਪੂਰਨ type, ਅਚਲ ਬਣਤਰ object_id *oid, ਅਚਲ ਅੱਖਰ *name)
{
	ਜੇ (!(filter & DO_REVS))
		ਵਾਪਸ;
	def = ਨਿਰਾ;

	ਜੇ ((symbolic || abbrev_ref) && name) {
		ਜੇ (symbolic == SHOW_SYMBOLIC_ਭਰਿਆ || abbrev_ref) {
			ਬਣਤਰ object_id discard;
			ਅੱਖਰ *ਭਰਿਆ;

			ਚੋਣ (repo_dwim_ref(the_repository, name,
					      ਲੜੀ_ਲੰਬਾਈ(name), &discard, &ਭਰਿਆ,
					      0)) {
			ਹਾਲਤ 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				ਰੋਕੋ;
			ਹਾਲਤ 1: /* happy */
				ਜੇ (abbrev_ref) {
					ਅੱਖਰ *old = ਭਰਿਆ;
					ਭਰਿਆ = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    ਭਰਿਆ,
									    abbrev_ref_strict);
					ਮੁਕਤ(old);
				}
				show_with_type(type, ਭਰਿਆ);
				ਰੋਕੋ;
			ਮੂਲ: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				ਰੋਕੋ;
			}
			ਮੁਕਤ(ਭਰਿਆ);
		} ਨਹੀਂਤਾਂ {
			show_with_type(type, name);
		}
	}
	ਨਹੀਂਤਾਂ ਜੇ (abbrev)
		show_with_type(type,
			       repo_ਲੱਭੋ_unique_abbrev(the_repository, oid, abbrev));
	ਨਹੀਂਤਾਂ
		show_with_type(type, oid_ਮੰਜ਼ਿਲ_hex(oid));
}

/* Output a flag, only if filter allows it. */
ਸਥਿਰ ਪੂਰਨ show_flag(ਅਚਲ ਅੱਖਰ *arg)
{
	ਜੇ (!(filter & DO_FLAGS))
		ਵਾਪਸ 0;
	ਜੇ (filter & (is_rev_ਦਲੀਲ(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		ਵਾਪਸ 1;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ show_default(ਖਾਲੀ)
{
	ਅਚਲ ਅੱਖਰ *s = def;

	ਜੇ (s) {
		ਬਣਤਰ object_id oid;

		def = ਨਿਰਾ;
		ਜੇ (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(ਸਾਧਾਰਨ, &oid, s);
			ਵਾਪਸ 1;
		}
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ show_reference(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਜੇ (ref_excluded(&ref_excludes, ref->name))
		ਵਾਪਸ 0;
	show_rev(ਸਾਧਾਰਨ, ref->oid, ref->name);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ anti_reference(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	show_rev(REVERSED, ref->oid, ref->name);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ show_abbrev(ਅਚਲ ਬਣਤਰ object_id *oid, ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	show_rev(ਸਾਧਾਰਨ, oid, ਨਿਰਾ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ show_datestring(ਅਚਲ ਅੱਖਰ *flag, ਅਚਲ ਅੱਖਰ *datestr)
{
	ਅੱਖਰ *ਬਫਰ;

	/* date handling requires both flags and revs */
	ਜੇ ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		ਵਾਪਸ;
	ਬਫਰ = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(ਬਫਰ);
	ਮੁਕਤ(ਬਫਰ);
}

ਸਥਿਰ ਪੂਰਨ show_file(ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ output_ਅਗੇਤਰ)
{
	show_default();
	ਜੇ ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		ਜੇ (output_ਅਗੇਤਰ) {
			ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ = the_repository->ਅਗੇਤਰ;
			ਅੱਖਰ *fname = ਅਗੇਤਰ_filename(ਅਗੇਤਰ, arg);
			show(fname);
			ਮੁਕਤ(fname);
		} ਨਹੀਂਤਾਂ
			show(arg);
		ਵਾਪਸ 1;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ try_difference(ਅਚਲ ਅੱਖਰ *arg)
{
	ਅਚਲ ਅੱਖਰ *dotdot;
	ਬਣਤਰ object_id ਸ਼ੁਰੂ_oid;
	ਬਣਤਰ object_id ਸਿਰਾ_oid;
	ਅਚਲ ਅੱਖਰ *ਸਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ਸ਼ੁਰੂ;
	ਅੱਖਰ *ਮੰਜ਼ਿਲ_free;
	ਪੂਰਨ ਸਮਮਿਤ;
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਸਿਰ_by_default[] = "HEAD";

	ਜੇ (!(dotdot = ਲੜੀ_ਖੋਜ(arg, "..")))
		ਵਾਪਸ 0;
	ਸ਼ੁਰੂ = ਮੰਜ਼ਿਲ_free = xmemdupz(arg, dotdot - arg);
	ਸਿਰਾ = dotdot + 2;
	ਸਮਮਿਤ = (*ਸਿਰਾ == '.');
	ਸਿਰਾ += ਸਮਮਿਤ;

	ਜੇ (!*ਸਿਰਾ)
		ਸਿਰਾ = ਸਿਰ_by_default;
	ਜੇ (dotdot == arg)
		ਸ਼ੁਰੂ = ਸਿਰ_by_default;

	ਜੇ (ਸ਼ੁਰੂ == ਸਿਰ_by_default && ਸਿਰਾ == ਸਿਰ_by_default &&
	    !ਸਮਮਿਤ) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
		ਵਾਪਸ 0;
	}

	ਜੇ (!repo_get_oid_committish(the_repository, ਸ਼ੁਰੂ, &ਸ਼ੁਰੂ_oid) && !repo_get_oid_committish(the_repository, ਸਿਰਾ, &ਸਿਰਾ_oid)) {
		show_rev(ਸਾਧਾਰਨ, &ਸਿਰਾ_oid, ਸਿਰਾ);
		show_rev(ਸਮਮਿਤ ? ਸਾਧਾਰਨ : REVERSED, &ਸ਼ੁਰੂ_oid, ਸ਼ੁਰੂ);
		ਜੇ (ਸਮਮਿਤ) {
			ਬਣਤਰ commit_list *exclude = ਨਿਰਾ;
			ਬਣਤਰ commit *a, *b;
			a = lookup_commit_reference(the_repository, &ਸ਼ੁਰੂ_oid);
			b = lookup_commit_reference(the_repository, &ਸਿਰਾ_oid);
			ਜੇ (!a || !b) {
				ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
				ਵਾਪਸ 0;
			}
			ਜੇ (repo_get_ਮਿਲਾਓ_bases(the_repository, a, b, &exclude) < 0)
				ਬਾਹਰ_ਨਿਕਲੋ(128);
			ਜਦੋਂ (exclude) {
				ਬਣਤਰ commit *commit = ਕੱਢੋ_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, ਨਿਰਾ);
			}
		}
		ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
		ਵਾਪਸ 1;
	}
	ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ try_ਮਾਪਾ_shorthands(ਅਚਲ ਅੱਖਰ *arg)
{
	ਅਚਲ ਅੱਖਰ *mark;
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ commit *commit;
	ਬਣਤਰ commit_list *parents;
	ਪੂਰਨ ਮਾਪਾ_ਸੰਖਿਆ;
	ਪੂਰਨ include_rev = 0;
	ਪੂਰਨ include_parents = 0;
	ਪੂਰਨ exclude_ਮਾਪਾ = 0;
	ਅੱਖਰ *ਮੰਜ਼ਿਲ_free;

	ਜੇ ((mark = ਲੜੀ_ਖੋਜ(arg, "^!"))) {
		include_rev = 1;
		ਜੇ (mark[2])
			ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ ((mark = ਲੜੀ_ਖੋਜ(arg, "^@"))) {
		include_parents = 1;
		ਜੇ (mark[2])
			ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ ((mark = ਲੜੀ_ਖੋਜ(arg, "^-"))) {
		include_rev = 1;
		exclude_ਮਾਪਾ = 1;

		ਜੇ (mark[2]) {
			ਅੱਖਰ *ਸਿਰਾ;
			exclude_ਮਾਪਾ = strtoul(mark + 2, &ਸਿਰਾ, 10);
			ਜੇ (*ਸਿਰਾ != '\0' || !exclude_ਮਾਪਾ)
				ਵਾਪਸ 0;
		}
	} ਨਹੀਂਤਾਂ
		ਵਾਪਸ 0;

	arg = ਮੰਜ਼ਿਲ_free = xmemdupz(arg, mark - arg);
	ਜੇ (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
		ਵਾਪਸ 0;
	}

	ਜੇ (exclude_ਮਾਪਾ &&
	    exclude_ਮਾਪਾ > commit_list_ਗਿਣਤੀ(commit->parents)) {
		ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
		ਵਾਪਸ 0;
	}

	ਜੇ (include_rev)
		show_rev(ਸਾਧਾਰਨ, &oid, arg);
	ਲਈ (parents = commit->parents, ਮਾਪਾ_ਸੰਖਿਆ = 1;
	     parents;
	     parents = parents->ਅਗਲਾ, ਮਾਪਾ_ਸੰਖਿਆ++) {
		ਅੱਖਰ *name = ਨਿਰਾ;

		ਜੇ (exclude_ਮਾਪਾ && ਮਾਪਾ_ਸੰਖਿਆ != exclude_ਮਾਪਾ)
			ਜਾਰੀ;

		ਜੇ (symbolic)
			name = xstrfmt("%s^%d", arg, ਮਾਪਾ_ਸੰਖਿਆ);
		show_rev(include_parents ? ਸਾਧਾਰਨ : REVERSED,
			 &parents->ਵਸਤੂ->object.oid, name);
		ਮੁਕਤ(name);
	}

	ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ parseopt_dump(ਅਚਲ ਬਣਤਰ option *o, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ strbuf *parsed = o->ਮੁੱਲ;
	ਜੇ (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	ਨਹੀਂਤਾਂ ਜੇ (o->short_name && (o->long_name == ਨਿਰਾ || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	ਨਹੀਂਤਾਂ
		strbuf_addf(parsed, " --%s", o->long_name);
	ਜੇ (arg) {
		ਜੇ (!stuck_long)
			strbuf_addch(parsed, ' ');
		ਨਹੀਂਤਾਂ ਜੇ (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *skipspaces(ਅਚਲ ਅੱਖਰ *s)
{
	ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ(*s))
		s++;
	ਵਾਪਸ s;
}

ਸਥਿਰ ਅੱਖਰ *findspace(ਅਚਲ ਅੱਖਰ *s)
{
	ਲਈ (; *s; s++)
		ਜੇ (ਖਾਲੀ_ਥਾਂ_ਹੈ(*s))
			ਵਾਪਸ (ਅੱਖਰ*)s;
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ cmd_parseopt(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਪੂਰਨ keep_dashdash = 0, stop_at_non_option = 0;
	ਅੱਖਰ ਅਚਲ * ਅਚਲ parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		ਨਿਰਾ
	};
	ਬਣਤਰ option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_ਸਿਰਾ(),
	};
	ਬਣਤਰ strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	ਬਣਤਰ strvec longnames = STRVEC_INIT;
	ਬਣਤਰ strvec usage = STRVEC_INIT;
	ਬਣਤਰ option *opts = ਨਿਰਾ;
	ਮਾਪ_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, ਅਗੇਤਰ, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	ਜੇ (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	ਲਈ (;;) {
		strbuf_reset(&sb);
		ਜੇ (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		ਜੇ (!strcmp("--", sb.buf)) {
			ਜੇ (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			ਰੋਕੋ;
		}

		strvec_ਧੱਕੋ(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	ਜਦੋਂ (strbuf_getline(&sb, stdin) != EOF) {
		ਅਚਲ ਅੱਖਰ *s;
		ਅੱਖਰ *help;
		ਬਣਤਰ option *o;

		ਜੇ (!sb.len)
			ਜਾਰੀ;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, ਆਕਾਰ(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		ਜੇ (!help || sb.buf == help) {
			o->type = OPTION_ਸਮੂਹ;
			o->help = xstrdup(skipspaces(sb.buf));
			ਜਾਰੀ;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->ਮੁੱਲ = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		ਜੇ (!s)
			s = help;

		ਜੇ (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		ਜੇ (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} ਨਹੀਂਤਾਂ ਜੇ (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (ਪੂਰਨ)(s - sb.buf), sb.buf);
		} ਨਹੀਂਤਾਂ {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (ਪੂਰਨ)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		ਜਦੋਂ (s < help) {
			ਚੋਣ (*s++) {
			ਹਾਲਤ '=':
				o->flags &= ~PARSE_OPT_NOARG;
				ਜਾਰੀ;
			ਹਾਲਤ '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				ਜਾਰੀ;
			ਹਾਲਤ '!':
				o->flags |= PARSE_OPT_NONEG;
				ਜਾਰੀ;
			ਹਾਲਤ '*':
				o->flags |= PARSE_OPT_HIDDEN;
				ਜਾਰੀ;
			}
			s--;
			ਰੋਕੋ;
		}

		ਜੇ (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, ਆਕਾਰ(*opts));
	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, usage.v,
			(keep_dashdash ? PARSE_OPT_KEEP_DASHDASH : 0) |
			(stop_at_non_option ? PARSE_OPT_STOP_AT_NON_OPTION : 0) |
			PARSE_OPT_SHELL_EVAL);

	strbuf_addstr(&parsed, " --");
	sq_quote_argv(&parsed, argv);
	puts(parsed.buf);

	strbuf_release(&parsed);
	strbuf_release(&sb);
	strvec_clear(&longnames);
	strvec_clear(&usage);
	ਲਈ (ਮਾਪ_t i = 0; i < opts_nr; i++) {
		ਮੁਕਤ((ਅੱਖਰ *) opts[i].help);
		ਮੁਕਤ((ਅੱਖਰ *) opts[i].argh);
	}
	ਮੁਕਤ(opts);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cmd_sq_quote(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;

	ਜੇ (argc)
		sq_quote_argv(&buf, argv);
	ਲਿਖੋ("%s\n", buf.buf);
	strbuf_release(&buf);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ die_no_single_rev(ਪੂਰਨ quiet)
{
	ਜੇ (quiet)
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	ਨਹੀਂਤਾਂ
		die(_("Needed a single revision"));
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
ਸਥਿਰ ਪੂਰਨ opt_with_ਮੁੱਲ(ਅਚਲ ਅੱਖਰ *arg, ਅਚਲ ਅੱਖਰ *opt, ਅਚਲ ਅੱਖਰ **ਮੁੱਲ)
{
	ਜੇ (skip_ਅਗੇਤਰ(arg, opt, &arg)) {
		ਜੇ (!*arg) {
			*ਮੁੱਲ = ਨਿਰਾ;
			ਵਾਪਸ 1;
		}
		ਜੇ (*arg++ == '=') {
			*ਮੁੱਲ = arg;
			ਵਾਪਸ 1;
		}
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ handle_ref_opt(ਅਚਲ ਅੱਖਰ *ਨਮੂਨਾ, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਜੇ (ਨਮੂਨਾ) {
		ਬਣਤਰ refs_for_each_ref_options opts = {
			.ਨਮੂਨਾ = ਨਮੂਨਾ,
			.ਅਗੇਤਰ = ਅਗੇਤਰ,
			.trim_ਅਗੇਤਰ = ਅਗੇਤਰ ? ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ਨਿਰਾ, &opts);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ refs_for_each_ref_options opts = {
			.ਅਗੇਤਰ = ਅਗੇਤਰ,
			.trim_ਅਗੇਤਰ = ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ਨਿਰਾ, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

ਗਿਣਤੀ format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

ਗਿਣਤੀ default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

ਸਥਿਰ ਖਾਲੀ print_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		       ਗਿਣਤੀ format_type format, ਗਿਣਤੀ default_type def)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਗਿਣਤੀ ਰਸਤਾ_format fmt;

	ਜੇ (format == FORMAT_DEFAULT) {
		ਚੋਣ (def) {
		ਹਾਲਤ DEFAULT_RELATIVE:
			fmt = ਰਸਤਾ_FORMAT_RELATIVE;
			ਰੋਕੋ;
		ਹਾਲਤ DEFAULT_RELATIVE_IF_SHARED:
			fmt = ਰਸਤਾ_FORMAT_RELATIVE_IF_SHARED;
			ਰੋਕੋ;
		ਹਾਲਤ DEFAULT_CANONICAL:
			fmt = ਰਸਤਾ_FORMAT_CANONICAL;
			ਰੋਕੋ;
		ਹਾਲਤ DEFAULT_UNMODIFIED:
		ਮੂਲ:
			fmt = ਰਸਤਾ_FORMAT_UNMODIFIED;
			ਰੋਕੋ;
		}
	} ਨਹੀਂਤਾਂ {
		ਚੋਣ (format) {
		ਹਾਲਤ FORMAT_RELATIVE:
			fmt = ਰਸਤਾ_FORMAT_RELATIVE;
			ਰੋਕੋ;
		ਹਾਲਤ FORMAT_CANONICAL:
			fmt = ਰਸਤਾ_FORMAT_CANONICAL;
			ਰੋਕੋ;
		ਮੂਲ:
			fmt = ਰਸਤਾ_FORMAT_UNMODIFIED;
			ਰੋਕੋ;
		}
	}

	format_ਰਸਤਾ(&sb, ਰਸਤਾ, ਅਗੇਤਰ, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

ਪੂਰਨ cmd_rev_parse(ਪੂਰਨ argc,
		  ਅਚਲ ਅੱਖਰ **argv,
		  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i, as_is = 0, verify = 0, quiet = 0, revs_ਗਿਣਤੀ = 0, type = 0;
	ਅਚਲ ਬਣਤਰ git_hash_algo *output_algo = ਨਿਰਾ;
	ਅਚਲ ਬਣਤਰ git_hash_algo *compat = ਨਿਰਾ;
	ਪੂਰਨ did_repo_ਤਿਆਰੀ = 0;
	ਪੂਰਨ has_dashdash = 0;
	ਪੂਰਨ output_ਅਗੇਤਰ = 0;
	ਬਣਤਰ object_id oid;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = 0;
	ਅਚਲ ਅੱਖਰ *name = ਨਿਰਾ;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਪੂਰਨ ਵੇਖਿਆ_ਸਿਰਾ_ਦਾ_options = 0;
	ਗਿਣਤੀ format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	ਜੇ (argc > 1 && !strcmp("--parseopt", argv[1]))
		ਵਾਪਸ cmd_parseopt(argc - 1, argv + 1, ਅਗੇਤਰ);

	ਜੇ (argc > 1 && !strcmp("--sq-quote", argv[1]))
		ਵਾਪਸ cmd_sq_quote(argc - 2, argv + 2);

	ਜੇ (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	ਲਈ (i = 1; i < argc; i++) {
		ਜੇ (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			ਰੋਕੋ;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	ਜੇ (argc == 1) {
		ਤਿਆਰੀ_git_directory(the_repository);
		repo_config(the_repository, git_default_config, ਨਿਰਾ);
		ਵਾਪਸ 0;
	}

	ਲਈ (i = 1; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];

		ਜੇ (as_is) {
			ਜੇ (show_file(arg, output_ਅਗੇਤਰ) && as_is < 2)
				verify_filename(the_repository, ਅਗੇਤਰ, arg, 0);
			ਜਾਰੀ;
		}

		ਜੇ (!ਵੇਖਿਆ_ਸਿਰਾ_ਦਾ_options) {
			ਜੇ (!strcmp(arg, "--local-env-vars")) {
				ਪੂਰਨ i;
				ਲਈ (i = 0; local_repo_env[i]; i++)
					ਲਿਖੋ("%s\n", local_repo_env[i]);
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--resolve-git-dir")) {
				ਅਚਲ ਅੱਖਰ *gitdir = argv[++i];
				ਜੇ (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				ਜੇ (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				ਜਾਰੀ;
			}
		}

		/* The rest of the options require a git repository. */
		ਜੇ (!did_repo_ਤਿਆਰੀ) {
			ਅਗੇਤਰ = ਤਿਆਰੀ_git_directory(the_repository);
			repo_config(the_repository, git_default_config, ਨਿਰਾ);
			did_repo_ਤਿਆਰੀ = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;
			compat = the_repository->compat_hash_algo;
		}

		ਜੇ (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			ਜੇ (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			ਜਾਰੀ;
		}

		ਜੇ (!ਵੇਖਿਆ_ਸਿਰਾ_ਦਾ_options && *arg == '-') {
			ਜੇ (!strcmp(arg, "--git-path")) {
				ਜੇ (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_ਰਸਤਾ(repo_git_ਰਸਤਾ_replace(the_repository, &buf,
								 "%s", argv[i + 1]), ਅਗੇਤਰ,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg,"-n")) {
				ਜੇ (++i >= argc)
					die(_("-n requires an argument"));
				ਜੇ ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				ਜਾਰੀ;
			}
			ਜੇ (starts_with(arg, "-n")) {
				ਜੇ ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				ਜਾਰੀ;
			}
			ਜੇ (opt_with_ਮੁੱਲ(arg, "--path-format", &arg)) {
				ਜੇ (!arg)
					die(_("--path-format requires an argument"));
				ਜੇ (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} ਨਹੀਂਤਾਂ {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--default")) {
				def = argv[++i];
				ਜੇ (!def)
					die(_("--default requires an argument"));
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--prefix")) {
				ਅਗੇਤਰ = argv[++i];
				ਜੇ (!ਅਗੇਤਰ)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->ਅਗੇਤਰ);
				the_repository->ਅਗੇਤਰ = xstrdup(ਅਗੇਤਰ);
				output_ਅਗੇਤਰ = 1;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				ਜਾਰੀ;
			}
			ਜੇ (opt_with_ਮੁੱਲ(arg, "--output-object-format", &arg)) {
				ਜੇ (!arg)
					die(_("no object format specified"));
				ਜੇ (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					ਜਾਰੀ;
				}
				ਨਹੀਂਤਾਂ ਜੇ (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					ਜਾਰੀ;
				}
				ਨਹੀਂਤਾਂ die(_("unsupported object format: %s"), arg);
			}
			ਜੇ (opt_with_ਮੁੱਲ(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				ਜੇ (!arg)
					ਜਾਰੀ;
				abbrev = strtoul(arg, ਨਿਰਾ, 10);
				ਜੇ (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				ਨਹੀਂਤਾਂ ਜੇ ((ਪੂਰਨ)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--sq")) {
				output_sq = 1;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_ਭਰਿਆ;
				ਜਾਰੀ;
			}
			ਜੇ (opt_with_ਮੁੱਲ(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				ਜੇ (arg) {
					ਜੇ (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					ਨਹੀਂਤਾਂ
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, ਨਿਰਾ);
				clear_ref_exclusions(&ref_excludes);
				ਜਾਰੀ;
			}
			ਜੇ (skip_ਅਗੇਤਰ(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, ਨਿਰਾ);
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--bisect")) {
				ਅੱਖਰ *ਅਗੇਤਰ;
				ਅੱਖਰ *ਪਦ_bad = ਨਿਰਾ;
				ਅੱਖਰ *ਪਦ_good = ਨਿਰਾ;
				ਬਣਤਰ refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&ਪਦ_bad, &ਪਦ_good);
				ਅਗੇਤਰ = xstrfmt("refs/bisect/%s", ਪਦ_bad);
				opts.ਅਗੇਤਰ = ਅਗੇਤਰ;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, ਨਿਰਾ, &opts);
				ਮੁਕਤ(ਅਗੇਤਰ);
				ਅਗੇਤਰ = xstrfmt("refs/bisect/%s", ਪਦ_good);
				opts.ਅਗੇਤਰ = ਅਗੇਤਰ;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, ਨਿਰਾ, &opts);
				ਮੁਕਤ(ਅਗੇਤਰ);
				ਮੁਕਤ(ਪਦ_good);
				ਮੁਕਤ(ਪਦ_bad);
				ਜਾਰੀ;
			}
			ਜੇ (opt_with_ਮੁੱਲ(arg, "--branches", &arg)) {
				ਜੇ (ref_excludes.hidden_refs_configured)
					ਵਾਪਸ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				ਜਾਰੀ;
			}
			ਜੇ (opt_with_ਮੁੱਲ(arg, "--tags", &arg)) {
				ਜੇ (ref_excludes.hidden_refs_configured)
					ਵਾਪਸ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				ਜਾਰੀ;
			}
			ਜੇ (skip_ਅਗੇਤਰ(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, ਨਿਰਾ);
				ਜਾਰੀ;
			}
			ਜੇ (opt_with_ਮੁੱਲ(arg, "--remotes", &arg)) {
				ਜੇ (ref_excludes.hidden_refs_configured)
					ਵਾਪਸ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				ਜਾਰੀ;
			}
			ਜੇ (skip_ਅਗੇਤਰ(arg, "--exclude=", &arg)) {
				ਜੋੜੋ_ref_exclusion(&ref_excludes, arg);
				ਜਾਰੀ;
			}
			ਜੇ (skip_ਅਗੇਤਰ(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--show-toplevel")) {
				ਅਚਲ ਅੱਖਰ *work_ਰੁੱਖ = repo_get_work_ਰੁੱਖ(the_repository);
				ਜੇ (work_ਰੁੱਖ)
					print_ਰਸਤਾ(work_ਰੁੱਖ, ਅਗੇਤਰ, format, DEFAULT_UNMODIFIED);
				ਨਹੀਂਤਾਂ
					die(_("this operation must be run in a work tree"));
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--show-superproject-working-tree")) {
				ਬਣਤਰ strbuf superproject = STRBUF_INIT;
				ਜੇ (get_superproject_working_ਰੁੱਖ(&superproject))
					print_ਰਸਤਾ(superproject.buf, ਅਗੇਤਰ, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--show-prefix")) {
				ਜੇ (ਅਗੇਤਰ)
					puts(ਅਗੇਤਰ);
				ਨਹੀਂਤਾਂ
					ਅੱਖਰ_ਲਿਖੋ('\n');
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--show-cdup")) {
				ਅਚਲ ਅੱਖਰ *pfx = ਅਗੇਤਰ;
				ਜੇ (!is_inside_work_ਰੁੱਖ(the_repository)) {
					ਅਚਲ ਅੱਖਰ *work_ਰੁੱਖ =
						repo_get_work_ਰੁੱਖ(the_repository);
					ਜੇ (work_ਰੁੱਖ)
						ਲਿਖੋ("%s\n", work_ਰੁੱਖ);
					ਜਾਰੀ;
				}
				ਜਦੋਂ (pfx) {
					pfx = strchr(pfx, '/');
					ਜੇ (pfx) {
						pfx++;
						ਲਿਖੋ("../");
					}
				}
				ਅੱਖਰ_ਲਿਖੋ('\n');
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				ਅਚਲ ਅੱਖਰ *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				ਅੱਖਰ *cwd;
				ਪੂਰਨ len;
				ਗਿਣਤੀ format_type wanted = format;
				ਜੇ (arg[2] == 'g') {	/* --git-dir */
					ਜੇ (gitdir) {
						print_ਰਸਤਾ(gitdir, ਅਗੇਤਰ, format, DEFAULT_UNMODIFIED);
						ਜਾਰੀ;
					}
					ਜੇ (!ਅਗੇਤਰ) {
						print_ਰਸਤਾ(".git", ਅਗੇਤਰ, format, DEFAULT_UNMODIFIED);
						ਜਾਰੀ;
					}
				} ਨਹੀਂਤਾਂ {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					ਜੇ (!gitdir && !ਅਗੇਤਰ)
						gitdir = ".git";
					ਜੇ (gitdir) {
						ਬਣਤਰ strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						ਜਾਰੀ;
					}
				}
				cwd = xgetcwd();
				len = ਲੜੀ_ਲੰਬਾਈ(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				ਮੁਕਤ(cwd);
				print_ਰਸਤਾ(buf.buf, ਅਗੇਤਰ, wanted, DEFAULT_CANONICAL);
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--git-common-dir")) {
				print_ਰਸਤਾ(repo_get_common_dir(the_repository), ਅਗੇਤਰ, format, DEFAULT_RELATIVE_IF_SHARED);
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--is-inside-git-dir")) {
				ਲਿਖੋ("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--is-inside-work-tree")) {
				ਲਿਖੋ("%s\n", is_inside_work_ਰੁੱਖ(the_repository) ? "true"
						: "false");
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--is-bare-repository")) {
				ਲਿਖੋ("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--is-shallow-repository")) {
				ਲਿਖੋ("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--shared-index-path")) {
				ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
					die(_("Could not read the index"));
				ਜੇ (the_repository->ਸੂਚਕਾਂਕ->ਵੰਡੋ_ਸੂਚਕਾਂਕ) {
					ਅਚਲ ਬਣਤਰ object_id *oid = &the_repository->ਸੂਚਕਾਂਕ->ਵੰਡੋ_ਸੂਚਕਾਂਕ->ਅਧਾਰ_oid;
					ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = repo_git_ਰਸਤਾ_replace(the_repository, &buf, "sharedindex.%s", oid_ਮੰਜ਼ਿਲ_hex(oid));
					print_ਰਸਤਾ(ਰਸਤਾ, ਅਗੇਤਰ, format, DEFAULT_RELATIVE);
				}
				ਜਾਰੀ;
			}
			ਜੇ (skip_ਅਗੇਤਰ(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				ਜਾਰੀ;
			}
			ਜੇ (skip_ਅਗੇਤਰ(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				ਜਾਰੀ;
			}
			ਜੇ (skip_ਅਗੇਤਰ(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				ਜਾਰੀ;
			}
			ਜੇ (skip_ਅਗੇਤਰ(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				ਜਾਰੀ;
			}
			ਜੇ (opt_with_ਮੁੱਲ(arg, "--show-object-format", &arg)) {
				ਅਚਲ ਅੱਖਰ *val = arg ? arg : "storage";

				ਜੇ (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				ਜੇ (!strcmp(val, "compat")) {
					ਜੇ (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					ਨਹੀਂਤਾਂ
						ਅੱਖਰ_ਲਿਖੋ('\n');
				} ਨਹੀਂਤਾਂ {
					puts(the_hash_algo->name);
				}
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_ਮੰਜ਼ਿਲ_name(the_repository->ref_storage_format));
				ਜਾਰੀ;
			}
			ਜੇ (!strcmp(arg, "--end-of-options")) {
				ਵੇਖਿਆ_ਸਿਰਾ_ਦਾ_options = 1;
				ਜੇ (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				ਜਾਰੀ;
			}
			ਜੇ (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			ਜਾਰੀ;
		}

		/* Not a flag argument */
		ਜੇ (try_difference(arg))
			ਜਾਰੀ;
		ਜੇ (try_ਮਾਪਾ_shorthands(arg))
			ਜਾਰੀ;
		name = arg;
		type = ਸਾਧਾਰਨ;
		ਜੇ (*arg == '^') {
			name++;
			type = REVERSED;
		}
		ਜੇ (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			ਜੇ (output_algo)
				repo_oid_ਮੰਜ਼ਿਲ_algop(the_repository, &oid,
						  output_algo, &oid);
			ਜੇ (verify)
				revs_ਗਿਣਤੀ++;
			ਨਹੀਂਤਾਂ
				show_rev(type, &oid, name);
			ਜਾਰੀ;
		}
		ਜੇ (verify)
			die_no_single_rev(quiet);
		ਜੇ (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		ਜੇ (!show_file(arg, output_ਅਗੇਤਰ))
			ਜਾਰੀ;
		verify_filename(the_repository, ਅਗੇਤਰ, arg, 1);
	}
	strbuf_release(&buf);
	ਜੇ (verify) {
		ਜੇ (revs_ਗਿਣਤੀ == 1) {
			show_rev(type, &oid, name);
			ਵਾਪਸ 0;
		} ਨਹੀਂਤਾਂ ਜੇ (revs_ਗਿਣਤੀ == 0 && show_default())
			ਵਾਪਸ 0;
		die_no_single_rev(quiet);
	} ਨਹੀਂਤਾਂ
		show_default();
	ਵਾਪਸ 0;
}
