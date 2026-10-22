/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"

#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "bisect.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "repo-settings.h"
#ಅನ್ತರ್ಭಾವ "repository.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "split-index.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "shallow.h"
#ಅನ್ತರ್ಭಾವ "object-file-convert.h"

#ವ್ಯಾಖ್ಯಾನಿಸು DO_REVS		1
#ವ್ಯಾಖ್ಯಾನಿಸು DO_NOREV	2
#ವ್ಯಾಖ್ಯಾನಿಸು DO_FLAGS	4
#ವ್ಯಾಖ್ಯಾನಿಸು DO_NONFLAGS	8
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ filter = ~0;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *def;

#ವ್ಯಾಖ್ಯಾನಿಸು ಸಾಮಾನ್ಯ 0
#ವ್ಯಾಖ್ಯಾನಿಸು REVERSED 1
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_type = ಸಾಮಾನ್ಯ;

#ವ್ಯಾಖ್ಯಾನಿಸು SHOW_SYMBOLIC_ASIS 1
#ವ್ಯಾಖ್ಯಾನಿಸು SHOW_SYMBOLIC_ತುಂಬಿದ 2
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ symbolic;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ abbrev;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ abbrev_ref;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ abbrev_ref_strict;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ output_sq;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stuck_long;
ಸ್ಥಿರ ರಚನೆ ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_rev_ವಾದ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rev_ವಾದಗಳು[] = {
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
		ಖಾಲಿ
	};
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **p = rev_ವಾದಗಳು;

	/* accept -<digit>, like traditional "head" */
	ಆದರೆ ((*arg == '-') && ಅಂಕಿಯೇ(arg[1]))
		ಹಿಂತಿರುಗಿ 1;

	ಗಾಗಿ (;;) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str = *p++;
		ಪೂರ್ಣಾಂಕ len;
		ಆದರೆ (!str)
			ಹಿಂತಿರುಗಿ 0;
		len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(str);
		ಆದರೆ (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(arg, str, len)))
			ಹಿಂತಿರುಗಿ 1;
	}
}

/* Output argument as a string, either SQ or normal */
ಸ್ಥಿರ ಶೂನ್ಯ show(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಆದರೆ (output_sq) {
		ಪೂರ್ಣಾಂಕ sq = '\'', ch;

		ಅಕ್ಷರ_ಬರೆ(sq);
		ಆಗ ((ch = *arg++)) {
			ಆದರೆ (ch == sq)
				fputs("'\\'", stdout);
			ಅಕ್ಷರ_ಬರೆ(ch);
		}
		ಅಕ್ಷರ_ಬರೆ(sq);
		ಅಕ್ಷರ_ಬರೆ(' ');
	}
	ಇಲ್ಲದಿದ್ದರೆ
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
ಸ್ಥಿರ ಶೂನ್ಯ show_with_type(ಪೂರ್ಣಾಂಕ type, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಆದರೆ (type != show_type)
		ಅಕ್ಷರ_ಬರೆ('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
ಸ್ಥಿರ ಶೂನ್ಯ show_rev(ಪೂರ್ಣಾಂಕ type, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಆದರೆ (!(filter & DO_REVS))
		ಹಿಂತಿರುಗಿ;
	def = ಖಾಲಿ;

	ಆದರೆ ((symbolic || abbrev_ref) && name) {
		ಆದರೆ (symbolic == SHOW_SYMBOLIC_ತುಂಬಿದ || abbrev_ref) {
			ರಚನೆ object_id discard;
			ಅಕ್ಷರ *ತುಂಬಿದ;

			ಆಯ್ಕೆ (repo_dwim_ref(the_repository, name,
					      ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name), &discard, &ತುಂಬಿದ,
					      0)) {
			ಸ್ಥಿತಿ 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				ನಿಲ್ಲಿಸು;
			ಸ್ಥಿತಿ 1: /* happy */
				ಆದರೆ (abbrev_ref) {
					ಅಕ್ಷರ *old = ತುಂಬಿದ;
					ತುಂಬಿದ = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    ತುಂಬಿದ,
									    abbrev_ref_strict);
					ಬಿಡುಗಡೆ(old);
				}
				show_with_type(type, ತುಂಬಿದ);
				ನಿಲ್ಲಿಸು;
			ಪೂರ್ವನಿಯೋಜಿತ: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				ನಿಲ್ಲಿಸು;
			}
			ಬಿಡುಗಡೆ(ತುಂಬಿದ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			show_with_type(type, name);
		}
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (abbrev)
		show_with_type(type,
			       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, oid, abbrev));
	ಇಲ್ಲದಿದ್ದರೆ
		show_with_type(type, oid_ಗಮ್ಯ_hex(oid));
}

/* Output a flag, only if filter allows it. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_flag(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಆದರೆ (!(filter & DO_FLAGS))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (filter & (is_rev_ವಾದ(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		ಹಿಂತಿರುಗಿ 1;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_default(ಶೂನ್ಯ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s = def;

	ಆದರೆ (s) {
		ರಚನೆ object_id oid;

		def = ಖಾಲಿ;
		ಆದರೆ (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(ಸಾಮಾನ್ಯ, &oid, s);
			ಹಿಂತಿರುಗಿ 1;
		}
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_reference(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಆದರೆ (ref_excluded(&ref_excludes, ref->name))
		ಹಿಂತಿರುಗಿ 0;
	show_rev(ಸಾಮಾನ್ಯ, ref->oid, ref->name);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ anti_reference(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	show_rev(REVERSED, ref->oid, ref->name);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_abbrev(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	show_rev(ಸಾಮಾನ್ಯ, oid, ಖಾಲಿ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_datestring(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *flag, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *datestr)
{
	ಅಕ್ಷರ *ಬಫರ್;

	/* date handling requires both flags and revs */
	ಆದರೆ ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		ಹಿಂತಿರುಗಿ;
	ಬಫರ್ = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(ಬಫರ್);
	ಬಿಡುಗಡೆ(ಬಫರ್);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ output_ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	show_default();
	ಆದರೆ ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		ಆದರೆ (output_ಪೂರ್ವಪ್ರತ್ಯಯ) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = the_repository->ಪೂರ್ವಪ್ರತ್ಯಯ;
			ಅಕ್ಷರ *fname = ಪೂರ್ವಪ್ರತ್ಯಯ_filename(ಪೂರ್ವಪ್ರತ್ಯಯ, arg);
			show(fname);
			ಬಿಡುಗಡೆ(fname);
		} ಇಲ್ಲದಿದ್ದರೆ
			show(arg);
		ಹಿಂತಿರುಗಿ 1;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ try_difference(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dotdot;
	ರಚನೆ object_id ಆರಂಭ_oid;
	ರಚನೆ object_id ಕೊನೆ_oid;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಆರಂಭ;
	ಅಕ್ಷರ *ಗಮ್ಯ_free;
	ಪೂರ್ಣಾಂಕ ಸಮಮಿತ;
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ತಲೆ_by_default[] = "HEAD";

	ಆದರೆ (!(dotdot = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(arg, "..")))
		ಹಿಂತಿರುಗಿ 0;
	ಆರಂಭ = ಗಮ್ಯ_free = xmemdupz(arg, dotdot - arg);
	ಕೊನೆ = dotdot + 2;
	ಸಮಮಿತ = (*ಕೊನೆ == '.');
	ಕೊನೆ += ಸಮಮಿತ;

	ಆದರೆ (!*ಕೊನೆ)
		ಕೊನೆ = ತಲೆ_by_default;
	ಆದರೆ (dotdot == arg)
		ಆರಂಭ = ತಲೆ_by_default;

	ಆದರೆ (ಆರಂಭ == ತಲೆ_by_default && ಕೊನೆ == ತಲೆ_by_default &&
	    !ಸಮಮಿತ) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!repo_get_oid_committish(the_repository, ಆರಂಭ, &ಆರಂಭ_oid) && !repo_get_oid_committish(the_repository, ಕೊನೆ, &ಕೊನೆ_oid)) {
		show_rev(ಸಾಮಾನ್ಯ, &ಕೊನೆ_oid, ಕೊನೆ);
		show_rev(ಸಮಮಿತ ? ಸಾಮಾನ್ಯ : REVERSED, &ಆರಂಭ_oid, ಆರಂಭ);
		ಆದರೆ (ಸಮಮಿತ) {
			ರಚನೆ commit_list *exclude = ಖಾಲಿ;
			ರಚನೆ commit *a, *b;
			a = lookup_commit_reference(the_repository, &ಆರಂಭ_oid);
			b = lookup_commit_reference(the_repository, &ಕೊನೆ_oid);
			ಆದರೆ (!a || !b) {
				ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
				ಹಿಂತಿರುಗಿ 0;
			}
			ಆದರೆ (repo_get_ವಿಲೀನ_bases(the_repository, a, b, &exclude) < 0)
				ನಿರ್ಗಮಿಸು(128);
			ಆಗ (exclude) {
				ರಚನೆ commit *commit = ತೆಗೆ_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, ಖಾಲಿ);
			}
		}
		ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
		ಹಿಂತಿರುಗಿ 1;
	}
	ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ try_ಪೋಷಕ_shorthands(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mark;
	ರಚನೆ object_id oid;
	ರಚನೆ commit *commit;
	ರಚನೆ commit_list *parents;
	ಪೂರ್ಣಾಂಕ ಪೋಷಕ_ಸಂಖ್ಯೆ;
	ಪೂರ್ಣಾಂಕ include_rev = 0;
	ಪೂರ್ಣಾಂಕ include_parents = 0;
	ಪೂರ್ಣಾಂಕ exclude_ಪೋಷಕ = 0;
	ಅಕ್ಷರ *ಗಮ್ಯ_free;

	ಆದರೆ ((mark = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(arg, "^!"))) {
		include_rev = 1;
		ಆದರೆ (mark[2])
			ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((mark = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(arg, "^@"))) {
		include_parents = 1;
		ಆದರೆ (mark[2])
			ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((mark = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(arg, "^-"))) {
		include_rev = 1;
		exclude_ಪೋಷಕ = 1;

		ಆದರೆ (mark[2]) {
			ಅಕ್ಷರ *ಕೊನೆ;
			exclude_ಪೋಷಕ = strtoul(mark + 2, &ಕೊನೆ, 10);
			ಆದರೆ (*ಕೊನೆ != '\0' || !exclude_ಪೋಷಕ)
				ಹಿಂತಿರುಗಿ 0;
		}
	} ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ 0;

	arg = ಗಮ್ಯ_free = xmemdupz(arg, mark - arg);
	ಆದರೆ (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (exclude_ಪೋಷಕ &&
	    exclude_ಪೋಷಕ > commit_list_ಎಣಿಕೆ(commit->parents)) {
		ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (include_rev)
		show_rev(ಸಾಮಾನ್ಯ, &oid, arg);
	ಗಾಗಿ (parents = commit->parents, ಪೋಷಕ_ಸಂಖ್ಯೆ = 1;
	     parents;
	     parents = parents->ಮುಂದಿನ, ಪೋಷಕ_ಸಂಖ್ಯೆ++) {
		ಅಕ್ಷರ *name = ಖಾಲಿ;

		ಆದರೆ (exclude_ಪೋಷಕ && ಪೋಷಕ_ಸಂಖ್ಯೆ != exclude_ಪೋಷಕ)
			ಮುಂದುವರಿ;

		ಆದರೆ (symbolic)
			name = xstrfmt("%s^%d", arg, ಪೋಷಕ_ಸಂಖ್ಯೆ);
		show_rev(include_parents ? ಸಾಮಾನ್ಯ : REVERSED,
			 &parents->ಅಂಶ->object.oid, name);
		ಬಿಡುಗಡೆ(name);
	}

	ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parseopt_dump(ಸ್ಥಿರಾಂಕ ರಚನೆ option *o, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ strbuf *parsed = o->ಮೌಲ್ಯ;
	ಆದರೆ (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (o->short_name && (o->long_name == ಖಾಲಿ || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addf(parsed, " --%s", o->long_name);
	ಆದರೆ (arg) {
		ಆದರೆ (!stuck_long)
			strbuf_addch(parsed, ' ');
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *skipspaces(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	ಆಗ (ಖಾಲಿಯೇ(*s))
		s++;
	ಹಿಂತಿರುಗಿ s;
}

ಸ್ಥಿರ ಅಕ್ಷರ *findspace(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	ಗಾಗಿ (; *s; s++)
		ಆದರೆ (ಖಾಲಿಯೇ(*s))
			ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ*)s;
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_parseopt(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಪೂರ್ಣಾಂಕ keep_dashdash = 0, stop_at_non_option = 0;
	ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		ಖಾಲಿ
	};
	ರಚನೆ option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_ಕೊನೆ(),
	};
	ರಚನೆ strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	ರಚನೆ strvec longnames = STRVEC_INIT;
	ರಚನೆ strvec usage = STRVEC_INIT;
	ರಚನೆ option *opts = ಖಾಲಿ;
	ಗಾತ್ರ_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	ಆದರೆ (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	ಗಾಗಿ (;;) {
		strbuf_reset(&sb);
		ಆದರೆ (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		ಆದರೆ (!strcmp("--", sb.buf)) {
			ಆದರೆ (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			ನಿಲ್ಲಿಸು;
		}

		strvec_ತಳ್ಳು(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	ಆಗ (strbuf_getline(&sb, stdin) != EOF) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s;
		ಅಕ್ಷರ *help;
		ರಚನೆ option *o;

		ಆದರೆ (!sb.len)
			ಮುಂದುವರಿ;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, ಗಾತ್ರ(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		ಆದರೆ (!help || sb.buf == help) {
			o->type = OPTION_ಗುಂಪು;
			o->help = xstrdup(skipspaces(sb.buf));
			ಮುಂದುವರಿ;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->ಮೌಲ್ಯ = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		ಆದರೆ (!s)
			s = help;

		ಆದರೆ (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		ಆದರೆ (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (ಪೂರ್ಣಾಂಕ)(s - sb.buf), sb.buf);
		} ಇಲ್ಲದಿದ್ದರೆ {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (ಪೂರ್ಣಾಂಕ)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		ಆಗ (s < help) {
			ಆಯ್ಕೆ (*s++) {
			ಸ್ಥಿತಿ '=':
				o->flags &= ~PARSE_OPT_NOARG;
				ಮುಂದುವರಿ;
			ಸ್ಥಿತಿ '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				ಮುಂದುವರಿ;
			ಸ್ಥಿತಿ '!':
				o->flags |= PARSE_OPT_NONEG;
				ಮುಂದುವರಿ;
			ಸ್ಥಿತಿ '*':
				o->flags |= PARSE_OPT_HIDDEN;
				ಮುಂದುವರಿ;
			}
			s--;
			ನಿಲ್ಲಿಸು;
		}

		ಆದರೆ (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, ಗಾತ್ರ(*opts));
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, opts, usage.v,
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
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < opts_nr; i++) {
		ಬಿಡುಗಡೆ((ಅಕ್ಷರ *) opts[i].help);
		ಬಿಡುಗಡೆ((ಅಕ್ಷರ *) opts[i].argh);
	}
	ಬಿಡುಗಡೆ(opts);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_sq_quote(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;

	ಆದರೆ (argc)
		sq_quote_argv(&buf, argv);
	ಬರೆ("%s\n", buf.buf);
	strbuf_release(&buf);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ die_no_single_rev(ಪೂರ್ಣಾಂಕ quiet)
{
	ಆದರೆ (quiet)
		ನಿರ್ಗಮಿಸು(1);
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("Needed a single revision"));
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ opt_with_ಮೌಲ್ಯ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮೌಲ್ಯ)
{
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, opt, &arg)) {
		ಆದರೆ (!*arg) {
			*ಮೌಲ್ಯ = ಖಾಲಿ;
			ಹಿಂತಿರುಗಿ 1;
		}
		ಆದರೆ (*arg++ == '=') {
			*ಮೌಲ್ಯ = arg;
			ಹಿಂತಿರುಗಿ 1;
		}
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_ref_opt(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ನಮೂನೆ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಆದರೆ (ನಮೂನೆ) {
		ರಚನೆ refs_for_each_ref_options opts = {
			.ನಮೂನೆ = ನಮೂನೆ,
			.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ,
			.trim_ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ಖಾಲಿ, &opts);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ refs_for_each_ref_options opts = {
			.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ,
			.trim_ಪೂರ್ವಪ್ರತ್ಯಯ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ಖಾಲಿ, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

ಎಣಿಕೆ format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

ಎಣಿಕೆ default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

ಸ್ಥಿರ ಶೂನ್ಯ print_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		       ಎಣಿಕೆ format_type format, ಎಣಿಕೆ default_type def)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಎಣಿಕೆ ಮಾರ್ಗ_format fmt;

	ಆದರೆ (format == FORMAT_DEFAULT) {
		ಆಯ್ಕೆ (def) {
		ಸ್ಥಿತಿ DEFAULT_RELATIVE:
			fmt = ಮಾರ್ಗ_FORMAT_RELATIVE;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ DEFAULT_RELATIVE_IF_SHARED:
			fmt = ಮಾರ್ಗ_FORMAT_RELATIVE_IF_SHARED;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ DEFAULT_CANONICAL:
			fmt = ಮಾರ್ಗ_FORMAT_CANONICAL;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ DEFAULT_UNMODIFIED:
		ಪೂರ್ವನಿಯೋಜಿತ:
			fmt = ಮಾರ್ಗ_FORMAT_UNMODIFIED;
			ನಿಲ್ಲಿಸು;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆಯ್ಕೆ (format) {
		ಸ್ಥಿತಿ FORMAT_RELATIVE:
			fmt = ಮಾರ್ಗ_FORMAT_RELATIVE;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ FORMAT_CANONICAL:
			fmt = ಮಾರ್ಗ_FORMAT_CANONICAL;
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			fmt = ಮಾರ್ಗ_FORMAT_UNMODIFIED;
			ನಿಲ್ಲಿಸು;
		}
	}

	format_ಮಾರ್ಗ(&sb, ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

ಪೂರ್ಣಾಂಕ cmd_rev_parse(ಪೂರ್ಣಾಂಕ argc,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		  ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i, as_is = 0, verify = 0, quiet = 0, revs_ಎಣಿಕೆ = 0, type = 0;
	ಸ್ಥಿರಾಂಕ ರಚನೆ git_hash_algo *output_algo = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ git_hash_algo *compat = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ did_repo_ಸಿದ್ಧತೆ = 0;
	ಪೂರ್ಣಾಂಕ has_dashdash = 0;
	ಪೂರ್ಣಾಂಕ output_ಪೂರ್ವಪ್ರತ್ಯಯ = 0;
	ರಚನೆ object_id oid;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = ಖಾಲಿ;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ನೋಡಿದ_ಕೊನೆ_ದ_options = 0;
	ಎಣಿಕೆ format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	ಆದರೆ (argc > 1 && !strcmp("--parseopt", argv[1]))
		ಹಿಂತಿರುಗಿ cmd_parseopt(argc - 1, argv + 1, ಪೂರ್ವಪ್ರತ್ಯಯ);

	ಆದರೆ (argc > 1 && !strcmp("--sq-quote", argv[1]))
		ಹಿಂತಿರುಗಿ cmd_sq_quote(argc - 2, argv + 2);

	ಆದರೆ (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	ಗಾಗಿ (i = 1; i < argc; i++) {
		ಆದರೆ (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			ನಿಲ್ಲಿಸು;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	ಆದರೆ (argc == 1) {
		ಸಿದ್ಧತೆ_git_directory(the_repository);
		repo_config(the_repository, git_default_config, ಖಾಲಿ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಗಾಗಿ (i = 1; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];

		ಆದರೆ (as_is) {
			ಆದರೆ (show_file(arg, output_ಪೂರ್ವಪ್ರತ್ಯಯ) && as_is < 2)
				verify_filename(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, arg, 0);
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (!ನೋಡಿದ_ಕೊನೆ_ದ_options) {
			ಆದರೆ (!strcmp(arg, "--local-env-vars")) {
				ಪೂರ್ಣಾಂಕ i;
				ಗಾಗಿ (i = 0; local_repo_env[i]; i++)
					ಬರೆ("%s\n", local_repo_env[i]);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--resolve-git-dir")) {
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *gitdir = argv[++i];
				ಆದರೆ (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				ಆದರೆ (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				ಮುಂದುವರಿ;
			}
		}

		/* The rest of the options require a git repository. */
		ಆದರೆ (!did_repo_ಸಿದ್ಧತೆ) {
			ಪೂರ್ವಪ್ರತ್ಯಯ = ಸಿದ್ಧತೆ_git_directory(the_repository);
			repo_config(the_repository, git_default_config, ಖಾಲಿ);
			did_repo_ಸಿದ್ಧತೆ = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;
			compat = the_repository->compat_hash_algo;
		}

		ಆದರೆ (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			ಆದರೆ (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (!ನೋಡಿದ_ಕೊನೆ_ದ_options && *arg == '-') {
			ಆದರೆ (!strcmp(arg, "--git-path")) {
				ಆದರೆ (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_ಮಾರ್ಗ(repo_git_ಮಾರ್ಗ_replace(the_repository, &buf,
								 "%s", argv[i + 1]), ಪೂರ್ವಪ್ರತ್ಯಯ,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg,"-n")) {
				ಆದರೆ (++i >= argc)
					die(_("-n requires an argument"));
				ಆದರೆ ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (starts_with(arg, "-n")) {
				ಆದರೆ ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (opt_with_ಮೌಲ್ಯ(arg, "--path-format", &arg)) {
				ಆದರೆ (!arg)
					die(_("--path-format requires an argument"));
				ಆದರೆ (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} ಇಲ್ಲದಿದ್ದರೆ {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--default")) {
				def = argv[++i];
				ಆದರೆ (!def)
					die(_("--default requires an argument"));
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--prefix")) {
				ಪೂರ್ವಪ್ರತ್ಯಯ = argv[++i];
				ಆದರೆ (!ಪೂರ್ವಪ್ರತ್ಯಯ)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->ಪೂರ್ವಪ್ರತ್ಯಯ);
				the_repository->ಪೂರ್ವಪ್ರತ್ಯಯ = xstrdup(ಪೂರ್ವಪ್ರತ್ಯಯ);
				output_ಪೂರ್ವಪ್ರತ್ಯಯ = 1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (opt_with_ಮೌಲ್ಯ(arg, "--output-object-format", &arg)) {
				ಆದರೆ (!arg)
					die(_("no object format specified"));
				ಆದರೆ (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					ಮುಂದುವರಿ;
				}
				ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					ಮುಂದುವರಿ;
				}
				ಇಲ್ಲದಿದ್ದರೆ die(_("unsupported object format: %s"), arg);
			}
			ಆದರೆ (opt_with_ಮೌಲ್ಯ(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				ಆದರೆ (!arg)
					ಮುಂದುವರಿ;
				abbrev = strtoul(arg, ಖಾಲಿ, 10);
				ಆದರೆ (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((ಪೂರ್ಣಾಂಕ)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--sq")) {
				output_sq = 1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_ತುಂಬಿದ;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (opt_with_ಮೌಲ್ಯ(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				ಆದರೆ (arg) {
					ಆದರೆ (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					ಇಲ್ಲದಿದ್ದರೆ
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, ಖಾಲಿ);
				clear_ref_exclusions(&ref_excludes);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, ಖಾಲಿ);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--bisect")) {
				ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
				ಅಕ್ಷರ *ಪದ_bad = ಖಾಲಿ;
				ಅಕ್ಷರ *ಪದ_good = ಖಾಲಿ;
				ರಚನೆ refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&ಪದ_bad, &ಪದ_good);
				ಪೂರ್ವಪ್ರತ್ಯಯ = xstrfmt("refs/bisect/%s", ಪದ_bad);
				opts.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, ಖಾಲಿ, &opts);
				ಬಿಡುಗಡೆ(ಪೂರ್ವಪ್ರತ್ಯಯ);
				ಪೂರ್ವಪ್ರತ್ಯಯ = xstrfmt("refs/bisect/%s", ಪದ_good);
				opts.ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, ಖಾಲಿ, &opts);
				ಬಿಡುಗಡೆ(ಪೂರ್ವಪ್ರತ್ಯಯ);
				ಬಿಡುಗಡೆ(ಪದ_good);
				ಬಿಡುಗಡೆ(ಪದ_bad);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (opt_with_ಮೌಲ್ಯ(arg, "--branches", &arg)) {
				ಆದರೆ (ref_excludes.hidden_refs_configured)
					ಹಿಂತಿರುಗಿ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (opt_with_ಮೌಲ್ಯ(arg, "--tags", &arg)) {
				ಆದರೆ (ref_excludes.hidden_refs_configured)
					ಹಿಂತಿರುಗಿ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, ಖಾಲಿ);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (opt_with_ಮೌಲ್ಯ(arg, "--remotes", &arg)) {
				ಆದರೆ (ref_excludes.hidden_refs_configured)
					ಹಿಂತಿರುಗಿ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--exclude=", &arg)) {
				ಕೂಡಿಸು_ref_exclusion(&ref_excludes, arg);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--show-toplevel")) {
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *work_ವೃಕ್ಷ = repo_get_work_ವೃಕ್ಷ(the_repository);
				ಆದರೆ (work_ವೃಕ್ಷ)
					print_ಮಾರ್ಗ(work_ವೃಕ್ಷ, ಪೂರ್ವಪ್ರತ್ಯಯ, format, DEFAULT_UNMODIFIED);
				ಇಲ್ಲದಿದ್ದರೆ
					die(_("this operation must be run in a work tree"));
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--show-superproject-working-tree")) {
				ರಚನೆ strbuf superproject = STRBUF_INIT;
				ಆದರೆ (get_superproject_working_ವೃಕ್ಷ(&superproject))
					print_ಮಾರ್ಗ(superproject.buf, ಪೂರ್ವಪ್ರತ್ಯಯ, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--show-prefix")) {
				ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ)
					puts(ಪೂರ್ವಪ್ರತ್ಯಯ);
				ಇಲ್ಲದಿದ್ದರೆ
					ಅಕ್ಷರ_ಬರೆ('\n');
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--show-cdup")) {
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pfx = ಪೂರ್ವಪ್ರತ್ಯಯ;
				ಆದರೆ (!is_inside_work_ವೃಕ್ಷ(the_repository)) {
					ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *work_ವೃಕ್ಷ =
						repo_get_work_ವೃಕ್ಷ(the_repository);
					ಆದರೆ (work_ವೃಕ್ಷ)
						ಬರೆ("%s\n", work_ವೃಕ್ಷ);
					ಮುಂದುವರಿ;
				}
				ಆಗ (pfx) {
					pfx = strchr(pfx, '/');
					ಆದರೆ (pfx) {
						pfx++;
						ಬರೆ("../");
					}
				}
				ಅಕ್ಷರ_ಬರೆ('\n');
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				ಅಕ್ಷರ *cwd;
				ಪೂರ್ಣಾಂಕ len;
				ಎಣಿಕೆ format_type wanted = format;
				ಆದರೆ (arg[2] == 'g') {	/* --git-dir */
					ಆದರೆ (gitdir) {
						print_ಮಾರ್ಗ(gitdir, ಪೂರ್ವಪ್ರತ್ಯಯ, format, DEFAULT_UNMODIFIED);
						ಮುಂದುವರಿ;
					}
					ಆದರೆ (!ಪೂರ್ವಪ್ರತ್ಯಯ) {
						print_ಮಾರ್ಗ(".git", ಪೂರ್ವಪ್ರತ್ಯಯ, format, DEFAULT_UNMODIFIED);
						ಮುಂದುವರಿ;
					}
				} ಇಲ್ಲದಿದ್ದರೆ {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					ಆದರೆ (!gitdir && !ಪೂರ್ವಪ್ರತ್ಯಯ)
						gitdir = ".git";
					ಆದರೆ (gitdir) {
						ರಚನೆ strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						ಮುಂದುವರಿ;
					}
				}
				cwd = xgetcwd();
				len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				ಬಿಡುಗಡೆ(cwd);
				print_ಮಾರ್ಗ(buf.buf, ಪೂರ್ವಪ್ರತ್ಯಯ, wanted, DEFAULT_CANONICAL);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--git-common-dir")) {
				print_ಮಾರ್ಗ(repo_get_common_dir(the_repository), ಪೂರ್ವಪ್ರತ್ಯಯ, format, DEFAULT_RELATIVE_IF_SHARED);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--is-inside-git-dir")) {
				ಬರೆ("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--is-inside-work-tree")) {
				ಬರೆ("%s\n", is_inside_work_ವೃಕ್ಷ(the_repository) ? "true"
						: "false");
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--is-bare-repository")) {
				ಬರೆ("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--is-shallow-repository")) {
				ಬರೆ("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--shared-index-path")) {
				ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
					die(_("Could not read the index"));
				ಆದರೆ (the_repository->ಸೂಚ್ಯಂಕ->ವಿಭಜಿಸು_ಸೂಚ್ಯಂಕ) {
					ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid = &the_repository->ಸೂಚ್ಯಂಕ->ವಿಭಜಿಸು_ಸೂಚ್ಯಂಕ->ತಳ_oid;
					ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = repo_git_ಮಾರ್ಗ_replace(the_repository, &buf, "sharedindex.%s", oid_ಗಮ್ಯ_hex(oid));
					print_ಮಾರ್ಗ(ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ, format, DEFAULT_RELATIVE);
				}
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (opt_with_ಮೌಲ್ಯ(arg, "--show-object-format", &arg)) {
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *val = arg ? arg : "storage";

				ಆದರೆ (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				ಆದರೆ (!strcmp(val, "compat")) {
					ಆದರೆ (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					ಇಲ್ಲದಿದ್ದರೆ
						ಅಕ್ಷರ_ಬರೆ('\n');
				} ಇಲ್ಲದಿದ್ದರೆ {
					puts(the_hash_algo->name);
				}
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_ಗಮ್ಯ_name(the_repository->ref_storage_format));
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (!strcmp(arg, "--end-of-options")) {
				ನೋಡಿದ_ಕೊನೆ_ದ_options = 1;
				ಆದರೆ (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			ಮುಂದುವರಿ;
		}

		/* Not a flag argument */
		ಆದರೆ (try_difference(arg))
			ಮುಂದುವರಿ;
		ಆದರೆ (try_ಪೋಷಕ_shorthands(arg))
			ಮುಂದುವರಿ;
		name = arg;
		type = ಸಾಮಾನ್ಯ;
		ಆದರೆ (*arg == '^') {
			name++;
			type = REVERSED;
		}
		ಆದರೆ (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			ಆದರೆ (output_algo)
				repo_oid_ಗಮ್ಯ_algop(the_repository, &oid,
						  output_algo, &oid);
			ಆದರೆ (verify)
				revs_ಎಣಿಕೆ++;
			ಇಲ್ಲದಿದ್ದರೆ
				show_rev(type, &oid, name);
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (verify)
			die_no_single_rev(quiet);
		ಆದರೆ (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		ಆದರೆ (!show_file(arg, output_ಪೂರ್ವಪ್ರತ್ಯಯ))
			ಮುಂದುವರಿ;
		verify_filename(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ, arg, 1);
	}
	strbuf_release(&buf);
	ಆದರೆ (verify) {
		ಆದರೆ (revs_ಎಣಿಕೆ == 1) {
			show_rev(type, &oid, name);
			ಹಿಂತಿರುಗಿ 0;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (revs_ಎಣಿಕೆ == 0 && show_default())
			ಹಿಂತಿರುಗಿ 0;
		die_no_single_rev(quiet);
	} ಇಲ್ಲದಿದ್ದರೆ
		show_default();
	ಹಿಂತಿರುಗಿ 0;
}
