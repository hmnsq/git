/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

#define DO_REVS		1
#define DO_NOREV	2
#define DO_FLAGS	4
#define DO_NONFLAGS	8
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ filter = ~0;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *def;

#define NORMAL 0
#define REVERSED 1
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ symbolic;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ abbrev;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ abbrev_ref;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ abbrev_ref_strict;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ output_sq;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stuck_long;
ಸ್ಥಿರ ಸಂರಚನಾ ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_rev_argument(ಧ್ರುವ ವರ್ಣ *arg)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *rev_args[] = {
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
		ರಿಕ್ತ
	};
	ಧ್ರುವ ವರ್ಣ **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	ಯದಿ ((*arg == '-') && isdigit(arg[1]))
		ಪ್ರತ್ಯಾಗಮ 1;

	ಪ್ರತಿ (;;) {
		ಧ್ರುವ ವರ್ಣ *str = *p++;
		ಪೂರ್ಣಾಙ್ಕ len;
		ಯದಿ (!str)
			ಪ್ರತ್ಯಾಗಮ 0;
		len = strlen(str);
		ಯದಿ (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			ಪ್ರತ್ಯಾಗಮ 1;
	}
}

/* Output argument as a string, either SQ or normal */
ಸ್ಥಿರ ಶೂನ್ಯ show(ಧ್ರುವ ವರ್ಣ *arg)
{
	ಯದಿ (output_sq) {
		ಪೂರ್ಣಾಙ್ಕ sq = '\'', ch;

		putchar(sq);
		ಯಾವತ್ ((ch = *arg++)) {
			ಯದಿ (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	ಅನ್ಯಥಾ
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
ಸ್ಥಿರ ಶೂನ್ಯ show_with_type(ಪೂರ್ಣಾಙ್ಕ type, ಧ್ರುವ ವರ್ಣ *arg)
{
	ಯದಿ (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
ಸ್ಥಿರ ಶೂನ್ಯ show_rev(ಪೂರ್ಣಾಙ್ಕ type, ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಧ್ರುವ ವರ್ಣ *name)
{
	ಯದಿ (!(filter & DO_REVS))
		ಪ್ರತ್ಯಾಗಮ;
	def = ರಿಕ್ತ;

	ಯದಿ ((symbolic || abbrev_ref) && name) {
		ಯದಿ (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			ಸಂರಚನಾ object_id discard;
			ವರ್ಣ *full;

			ವಿಕಲ್ಪ (repo_dwim_ref(the_repository, name,
					      strlen(name), &discard, &full,
					      0)) {
			ಪಕ್ಷ 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				ಭಙ್ಗ;
			ಪಕ್ಷ 1: /* happy */
				ಯದಿ (abbrev_ref) {
					ವರ್ಣ *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					ಮುಕ್ತ(old);
				}
				show_with_type(type, full);
				ಭಙ್ಗ;
			ಸ್ವಭಾವ: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				ಭಙ್ಗ;
			}
			ಮುಕ್ತ(full);
		} ಅನ್ಯಥಾ {
			show_with_type(type, name);
		}
	}
	ಅನ್ಯಥಾ ಯದಿ (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	ಅನ್ಯಥಾ
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_flag(ಧ್ರುವ ವರ್ಣ *arg)
{
	ಯದಿ (!(filter & DO_FLAGS))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		ಪ್ರತ್ಯಾಗಮ 1;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_default(ಶೂನ್ಯ)
{
	ಧ್ರುವ ವರ್ಣ *s = def;

	ಯದಿ (s) {
		ಸಂರಚನಾ object_id oid;

		def = ರಿಕ್ತ;
		ಯದಿ (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			ಪ್ರತ್ಯಾಗಮ 1;
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_reference(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data UNUSED)
{
	ಯದಿ (ref_excluded(&ref_excludes, ref->name))
		ಪ್ರತ್ಯಾಗಮ 0;
	show_rev(NORMAL, ref->oid, ref->name);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ anti_reference(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_abbrev(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಶೂನ್ಯ *cb_data UNUSED)
{
	show_rev(NORMAL, oid, ರಿಕ್ತ);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_datestring(ಧ್ರುವ ವರ್ಣ *flag, ಧ್ರುವ ವರ್ಣ *datestr)
{
	ವರ್ಣ *buffer;

	/* date handling requires both flags and revs */
	ಯದಿ ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		ಪ್ರತ್ಯಾಗಮ;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	ಮುಕ್ತ(buffer);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_file(ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ output_prefix)
{
	show_default();
	ಯದಿ ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		ಯದಿ (output_prefix) {
			ಧ್ರುವ ವರ್ಣ *prefix = the_repository->prefix;
			ವರ್ಣ *fname = prefix_filename(prefix, arg);
			show(fname);
			ಮುಕ್ತ(fname);
		} ಅನ್ಯಥಾ
			show(arg);
		ಪ್ರತ್ಯಾಗಮ 1;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ try_difference(ಧ್ರುವ ವರ್ಣ *arg)
{
	ಧ್ರುವ ವರ್ಣ *dotdot;
	ಸಂರಚನಾ object_id start_oid;
	ಸಂರಚನಾ object_id end_oid;
	ಧ್ರುವ ವರ್ಣ *end;
	ಧ್ರುವ ವರ್ಣ *start;
	ವರ್ಣ *to_free;
	ಪೂರ್ಣಾಙ್ಕ symmetric;
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ head_by_default[] = "HEAD";

	ಯದಿ (!(dotdot = strstr(arg, "..")))
		ಪ್ರತ್ಯಾಗಮ 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	ಯದಿ (!*end)
		end = head_by_default;
	ಯದಿ (dotdot == arg)
		start = head_by_default;

	ಯದಿ (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		ಮುಕ್ತ(to_free);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		ಯದಿ (symmetric) {
			ಸಂರಚನಾ commit_list *exclude = ರಿಕ್ತ;
			ಸಂರಚನಾ commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			ಯದಿ (!a || !b) {
				ಮುಕ್ತ(to_free);
				ಪ್ರತ್ಯಾಗಮ 0;
			}
			ಯದಿ (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			ಯಾವತ್ (exclude) {
				ಸಂರಚನಾ commit *commit = ತೆಗೆ_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, ರಿಕ್ತ);
			}
		}
		ಮುಕ್ತ(to_free);
		ಪ್ರತ್ಯಾಗಮ 1;
	}
	ಮುಕ್ತ(to_free);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ try_parent_shorthands(ಧ್ರುವ ವರ್ಣ *arg)
{
	ಧ್ರುವ ವರ್ಣ *mark;
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ commit *commit;
	ಸಂರಚನಾ commit_list *parents;
	ಪೂರ್ಣಾಙ್ಕ parent_ಸಂಖ್ಯೆ;
	ಪೂರ್ಣಾಙ್ಕ include_rev = 0;
	ಪೂರ್ಣಾಙ್ಕ include_parents = 0;
	ಪೂರ್ಣಾಙ್ಕ exclude_parent = 0;
	ವರ್ಣ *to_free;

	ಯದಿ ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		ಯದಿ (mark[2])
			ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		ಯದಿ (mark[2])
			ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		ಯದಿ (mark[2]) {
			ವರ್ಣ *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			ಯದಿ (*end != '\0' || !exclude_parent)
				ಪ್ರತ್ಯಾಗಮ 0;
		}
	} ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	ಯದಿ (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		ಮುಕ್ತ(to_free);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (exclude_parent &&
	    exclude_parent > commit_list_ಎಣಿಕೆ(commit->parents)) {
		ಮುಕ್ತ(to_free);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (include_rev)
		show_rev(NORMAL, &oid, arg);
	ಪ್ರತಿ (parents = commit->parents, parent_ಸಂಖ್ಯೆ = 1;
	     parents;
	     parents = parents->next, parent_ಸಂಖ್ಯೆ++) {
		ವರ್ಣ *name = ರಿಕ್ತ;

		ಯದಿ (exclude_parent && parent_ಸಂಖ್ಯೆ != exclude_parent)
			ಅನುವರ್ತ;

		ಯದಿ (symbolic)
			name = xstrfmt("%s^%d", arg, parent_ಸಂಖ್ಯೆ);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		ಮುಕ್ತ(name);
	}

	ಮುಕ್ತ(to_free);
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parseopt_dump(ಧ್ರುವ ಸಂರಚನಾ option *o, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ strbuf *parsed = o->ಮೌಲ್ಯ;
	ಯದಿ (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	ಅನ್ಯಥಾ ಯದಿ (o->short_name && (o->long_name == ರಿಕ್ತ || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	ಅನ್ಯಥಾ
		strbuf_addf(parsed, " --%s", o->long_name);
	ಯದಿ (arg) {
		ಯದಿ (!stuck_long)
			strbuf_addch(parsed, ' ');
		ಅನ್ಯಥಾ ಯದಿ (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *skipspaces(ಧ್ರುವ ವರ್ಣ *s)
{
	ಯಾವತ್ (isspace(*s))
		s++;
	ಪ್ರತ್ಯಾಗಮ s;
}

ಸ್ಥಿರ ವರ್ಣ *findspace(ಧ್ರುವ ವರ್ಣ *s)
{
	ಪ್ರತಿ (; *s; s++)
		ಯದಿ (isspace(*s))
			ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ*)s;
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_parseopt(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಪೂರ್ಣಾಙ್ಕ keep_dashdash = 0, stop_at_non_option = 0;
	ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		ರಿಕ್ತ
	};
	ಸಂರಚನಾ option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_END(),
	};
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	ಸಂರಚನಾ strvec longnames = STRVEC_INIT;
	ಸಂರಚನಾ strvec usage = STRVEC_INIT;
	ಸಂರಚನಾ option *opts = ರಿಕ್ತ;
	size_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	ಯದಿ (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	ಪ್ರತಿ (;;) {
		strbuf_reset(&sb);
		ಯದಿ (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		ಯದಿ (!strcmp("--", sb.buf)) {
			ಯದಿ (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			ಭಙ್ಗ;
		}

		strvec_ತಳ್ಳು(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	ಯಾವತ್ (strbuf_getline(&sb, stdin) != EOF) {
		ಧ್ರುವ ವರ್ಣ *s;
		ವರ್ಣ *help;
		ಸಂರಚನಾ option *o;

		ಯದಿ (!sb.len)
			ಅನುವರ್ತ;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, ಪರಿಮಾಣ(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		ಯದಿ (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			ಅನುವರ್ತ;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->ಮೌಲ್ಯ = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		ಯದಿ (!s)
			s = help;

		ಯದಿ (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		ಯದಿ (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} ಅನ್ಯಥಾ ಯದಿ (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (ಪೂರ್ಣಾಙ್ಕ)(s - sb.buf), sb.buf);
		} ಅನ್ಯಥಾ {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (ಪೂರ್ಣಾಙ್ಕ)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		ಯಾವತ್ (s < help) {
			ವಿಕಲ್ಪ (*s++) {
			ಪಕ್ಷ '=':
				o->flags &= ~PARSE_OPT_NOARG;
				ಅನುವರ್ತ;
			ಪಕ್ಷ '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				ಅನುವರ್ತ;
			ಪಕ್ಷ '!':
				o->flags |= PARSE_OPT_NONEG;
				ಅನುವರ್ತ;
			ಪಕ್ಷ '*':
				o->flags |= PARSE_OPT_HIDDEN;
				ಅನುವರ್ತ;
			}
			s--;
			ಭಙ್ಗ;
		}

		ಯದಿ (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, ಪರಿಮಾಣ(*opts));
	argc = parse_options(argc, argv, prefix, opts, usage.v,
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
	ಪ್ರತಿ (size_t i = 0; i < opts_nr; i++) {
		ಮುಕ್ತ((ವರ್ಣ *) opts[i].help);
		ಮುಕ್ತ((ವರ್ಣ *) opts[i].argh);
	}
	ಮುಕ್ತ(opts);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_sq_quote(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

	ಯದಿ (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ die_no_single_rev(ಪೂರ್ಣಾಙ್ಕ quiet)
{
	ಯದಿ (quiet)
		exit(1);
	ಅನ್ಯಥಾ
		die(_("Needed a single revision"));
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ opt_with_ಮೌಲ್ಯ(ಧ್ರುವ ವರ್ಣ *arg, ಧ್ರುವ ವರ್ಣ *opt, ಧ್ರುವ ವರ್ಣ **ಮೌಲ್ಯ)
{
	ಯದಿ (skip_prefix(arg, opt, &arg)) {
		ಯದಿ (!*arg) {
			*ಮೌಲ್ಯ = ರಿಕ್ತ;
			ಪ್ರತ್ಯಾಗಮ 1;
		}
		ಯದಿ (*arg++ == '=') {
			*ಮೌಲ್ಯ = arg;
			ಪ್ರತ್ಯಾಗಮ 1;
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_ref_opt(ಧ್ರುವ ವರ್ಣ *pattern, ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಯದಿ (pattern) {
		ಸಂರಚನಾ refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ರಿಕ್ತ, &opts);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = strlen(prefix),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ರಿಕ್ತ, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

ಗಣನಾ format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

ಗಣನಾ default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

ಸ್ಥಿರ ಶೂನ್ಯ print_path(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *prefix,
		       ಗಣನಾ format_type format, ಗಣನಾ default_type def)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಗಣನಾ path_format fmt;

	ಯದಿ (format == FORMAT_DEFAULT) {
		ವಿಕಲ್ಪ (def) {
		ಪಕ್ಷ DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			ಭಙ್ಗ;
		ಪಕ್ಷ DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			ಭಙ್ಗ;
		ಪಕ್ಷ DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			ಭಙ್ಗ;
		ಪಕ್ಷ DEFAULT_UNMODIFIED:
		ಸ್ವಭಾವ:
			fmt = PATH_FORMAT_UNMODIFIED;
			ಭಙ್ಗ;
		}
	} ಅನ್ಯಥಾ {
		ವಿಕಲ್ಪ (format) {
		ಪಕ್ಷ FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			ಭಙ್ಗ;
		ಪಕ್ಷ FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			fmt = PATH_FORMAT_UNMODIFIED;
			ಭಙ್ಗ;
		}
	}

	format_path(&sb, path, prefix, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

ಪೂರ್ಣಾಙ್ಕ cmd_rev_parse(ಪೂರ್ಣಾಙ್ಕ argc,
		  ಧ್ರುವ ವರ್ಣ **argv,
		  ಧ್ರುವ ವರ್ಣ *prefix,
		  ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ i, as_is = 0, verify = 0, quiet = 0, revs_ಎಣಿಕೆ = 0, type = 0;
	ಧ್ರುವ ಸಂರಚನಾ git_hash_algo *output_algo = ರಿಕ್ತ;
	ಧ್ರುವ ಸಂರಚನಾ git_hash_algo *compat = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ did_repo_setup = 0;
	ಪೂರ್ಣಾಙ್ಕ has_dashdash = 0;
	ಪೂರ್ಣಾಙ್ಕ output_prefix = 0;
	ಸಂರಚನಾ object_id oid;
	unsigned ಪೂರ್ಣಾಙ್ಕ flags = 0;
	ಧ್ರುವ ವರ್ಣ *name = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ seen_end_of_options = 0;
	ಗಣನಾ format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	ಯದಿ (argc > 1 && !strcmp("--parseopt", argv[1]))
		ಪ್ರತ್ಯಾಗಮ cmd_parseopt(argc - 1, argv + 1, prefix);

	ಯದಿ (argc > 1 && !strcmp("--sq-quote", argv[1]))
		ಪ್ರತ್ಯಾಗಮ cmd_sq_quote(argc - 2, argv + 2);

	ಯದಿ (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	ಪ್ರತಿ (i = 1; i < argc; i++) {
		ಯದಿ (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			ಭಙ್ಗ;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	ಯದಿ (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, ರಿಕ್ತ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತಿ (i = 1; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[i];

		ಯದಿ (as_is) {
			ಯದಿ (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			ಅನುವರ್ತ;
		}

		ಯದಿ (!seen_end_of_options) {
			ಯದಿ (!strcmp(arg, "--local-env-vars")) {
				ಪೂರ್ಣಾಙ್ಕ i;
				ಪ್ರತಿ (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--resolve-git-dir")) {
				ಧ್ರುವ ವರ್ಣ *gitdir = argv[++i];
				ಯದಿ (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				ಯದಿ (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				ಅನುವರ್ತ;
			}
		}

		/* The rest of the options require a git repository. */
		ಯದಿ (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, ರಿಕ್ತ);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		ಯದಿ (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			ಯದಿ (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			ಅನುವರ್ತ;
		}

		ಯದಿ (!seen_end_of_options && *arg == '-') {
			ಯದಿ (!strcmp(arg, "--git-path")) {
				ಯದಿ (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg,"-n")) {
				ಯದಿ (++i >= argc)
					die(_("-n requires an argument"));
				ಯದಿ ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				ಅನುವರ್ತ;
			}
			ಯದಿ (starts_with(arg, "-n")) {
				ಯದಿ ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				ಅನುವರ್ತ;
			}
			ಯದಿ (opt_with_ಮೌಲ್ಯ(arg, "--path-format", &arg)) {
				ಯದಿ (!arg)
					die(_("--path-format requires an argument"));
				ಯದಿ (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} ಅನ್ಯಥಾ {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--default")) {
				def = argv[++i];
				ಯದಿ (!def)
					die(_("--default requires an argument"));
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				ಯದಿ (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				ಅನುವರ್ತ;
			}
			ಯದಿ (opt_with_ಮೌಲ್ಯ(arg, "--output-object-format", &arg)) {
				ಯದಿ (!arg)
					die(_("no object format specified"));
				ಯದಿ (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					ಅನುವರ್ತ;
				}
				ಅನ್ಯಥಾ ಯದಿ (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					ಅನುವರ್ತ;
				}
				ಅನ್ಯಥಾ die(_("unsupported object format: %s"), arg);
			}
			ಯದಿ (opt_with_ಮೌಲ್ಯ(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				ಯದಿ (!arg)
					ಅನುವರ್ತ;
				abbrev = strtoul(arg, ರಿಕ್ತ, 10);
				ಯದಿ (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				ಅನ್ಯಥಾ ಯದಿ ((ಪೂರ್ಣಾಙ್ಕ)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--sq")) {
				output_sq = 1;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				ಅನುವರ್ತ;
			}
			ಯದಿ (opt_with_ಮೌಲ್ಯ(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				ಯದಿ (arg) {
					ಯದಿ (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					ಅನ್ಯಥಾ
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, ರಿಕ್ತ);
				clear_ref_exclusions(&ref_excludes);
				ಅನುವರ್ತ;
			}
			ಯದಿ (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, ರಿಕ್ತ);
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--bisect")) {
				ವರ್ಣ *prefix;
				ವರ್ಣ *term_bad = ರಿಕ್ತ;
				ವರ್ಣ *term_good = ರಿಕ್ತ;
				ಸಂರಚನಾ refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&term_bad, &term_good);
				prefix = xstrfmt("refs/bisect/%s", term_bad);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, ರಿಕ್ತ, &opts);
				ಮುಕ್ತ(prefix);
				prefix = xstrfmt("refs/bisect/%s", term_good);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, ರಿಕ್ತ, &opts);
				ಮುಕ್ತ(prefix);
				ಮುಕ್ತ(term_good);
				ಮುಕ್ತ(term_bad);
				ಅನುವರ್ತ;
			}
			ಯದಿ (opt_with_ಮೌಲ್ಯ(arg, "--branches", &arg)) {
				ಯದಿ (ref_excludes.hidden_refs_configured)
					ಪ್ರತ್ಯಾಗಮ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				ಅನುವರ್ತ;
			}
			ಯದಿ (opt_with_ಮೌಲ್ಯ(arg, "--tags", &arg)) {
				ಯದಿ (ref_excludes.hidden_refs_configured)
					ಪ್ರತ್ಯಾಗಮ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				ಅನುವರ್ತ;
			}
			ಯದಿ (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, ರಿಕ್ತ);
				ಅನುವರ್ತ;
			}
			ಯದಿ (opt_with_ಮೌಲ್ಯ(arg, "--remotes", &arg)) {
				ಯದಿ (ref_excludes.hidden_refs_configured)
					ಪ್ರತ್ಯಾಗಮ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				ಅನುವರ್ತ;
			}
			ಯದಿ (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				ಅನುವರ್ತ;
			}
			ಯದಿ (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--show-toplevel")) {
				ಧ್ರುವ ವರ್ಣ *work_ವೃಕ್ಷ = repo_get_work_ವೃಕ್ಷ(the_repository);
				ಯದಿ (work_ವೃಕ್ಷ)
					print_path(work_ವೃಕ್ಷ, prefix, format, DEFAULT_UNMODIFIED);
				ಅನ್ಯಥಾ
					die(_("this operation must be run in a work tree"));
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--show-superproject-working-tree")) {
				ಸಂರಚನಾ strbuf superproject = STRBUF_INIT;
				ಯದಿ (get_superproject_working_ವೃಕ್ಷ(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--show-prefix")) {
				ಯದಿ (prefix)
					puts(prefix);
				ಅನ್ಯಥಾ
					putchar('\n');
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--show-cdup")) {
				ಧ್ರುವ ವರ್ಣ *pfx = prefix;
				ಯದಿ (!is_inside_work_ವೃಕ್ಷ(the_repository)) {
					ಧ್ರುವ ವರ್ಣ *work_ವೃಕ್ಷ =
						repo_get_work_ವೃಕ್ಷ(the_repository);
					ಯದಿ (work_ವೃಕ್ಷ)
						printf("%s\n", work_ವೃಕ್ಷ);
					ಅನುವರ್ತ;
				}
				ಯಾವತ್ (pfx) {
					pfx = strchr(pfx, '/');
					ಯದಿ (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				ಧ್ರುವ ವರ್ಣ *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				ವರ್ಣ *cwd;
				ಪೂರ್ಣಾಙ್ಕ len;
				ಗಣನಾ format_type wanted = format;
				ಯದಿ (arg[2] == 'g') {	/* --git-dir */
					ಯದಿ (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						ಅನುವರ್ತ;
					}
					ಯದಿ (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						ಅನುವರ್ತ;
					}
				} ಅನ್ಯಥಾ {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					ಯದಿ (!gitdir && !prefix)
						gitdir = ".git";
					ಯದಿ (gitdir) {
						ಸಂರಚನಾ strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						ಅನುವರ್ತ;
					}
				}
				cwd = xgetcwd();
				len = strlen(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				ಮುಕ್ತ(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_ವೃಕ್ಷ(the_repository) ? "true"
						: "false");
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--shared-index-path")) {
				ಯದಿ (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				ಯದಿ (the_repository->index->split_index) {
					ಧ್ರುವ ಸಂರಚನಾ object_id *oid = &the_repository->index->split_index->base_oid;
					ಧ್ರುವ ವರ್ಣ *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				ಅನುವರ್ತ;
			}
			ಯದಿ (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				ಅನುವರ್ತ;
			}
			ಯದಿ (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				ಅನುವರ್ತ;
			}
			ಯದಿ (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				ಅನುವರ್ತ;
			}
			ಯದಿ (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				ಅನುವರ್ತ;
			}
			ಯದಿ (opt_with_ಮೌಲ್ಯ(arg, "--show-object-format", &arg)) {
				ಧ್ರುವ ವರ್ಣ *val = arg ? arg : "storage";

				ಯದಿ (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				ಯದಿ (!strcmp(val, "compat")) {
					ಯದಿ (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					ಅನ್ಯಥಾ
						putchar('\n');
				} ಅನ್ಯಥಾ {
					puts(the_hash_algo->name);
				}
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				ಅನುವರ್ತ;
			}
			ಯದಿ (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				ಯದಿ (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				ಅನುವರ್ತ;
			}
			ಯದಿ (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			ಅನುವರ್ತ;
		}

		/* Not a flag argument */
		ಯದಿ (try_difference(arg))
			ಅನುವರ್ತ;
		ಯದಿ (try_parent_shorthands(arg))
			ಅನುವರ್ತ;
		name = arg;
		type = NORMAL;
		ಯದಿ (*arg == '^') {
			name++;
			type = REVERSED;
		}
		ಯದಿ (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			ಯದಿ (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			ಯದಿ (verify)
				revs_ಎಣಿಕೆ++;
			ಅನ್ಯಥಾ
				show_rev(type, &oid, name);
			ಅನುವರ್ತ;
		}
		ಯದಿ (verify)
			die_no_single_rev(quiet);
		ಯದಿ (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		ಯದಿ (!show_file(arg, output_prefix))
			ಅನುವರ್ತ;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	ಯದಿ (verify) {
		ಯದಿ (revs_ಎಣಿಕೆ == 1) {
			show_rev(type, &oid, name);
			ಪ್ರತ್ಯಾಗಮ 0;
		} ಅನ್ಯಥಾ ಯದಿ (revs_ಎಣಿಕೆ == 0 && show_default())
			ಪ್ರತ್ಯಾಗಮ 0;
		die_no_single_rev(quiet);
	} ಅನ್ಯಥಾ
		show_default();
	ಪ್ರತ್ಯಾಗಮ 0;
}
