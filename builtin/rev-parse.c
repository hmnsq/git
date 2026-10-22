/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"

#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "bisect.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "repo-settings.h"
#অন্তর্ভাব "repository.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "split-index.h"
#অন্তর্ভাব "submodule.h"
#অন্তর্ভাব "commit-reach.h"
#অন্তর্ভাব "shallow.h"
#অন্তর্ভাব "object-file-convert.h"

#define DO_REVS		1
#define DO_NOREV	2
#define DO_FLAGS	4
#define DO_NONFLAGS	8
স্থির পূর্ণাঙ্ক filter = ~0;

স্থির ধ্রুব বর্ণ *def;

#define NORMAL 0
#define REVERSED 1
স্থির পূর্ণাঙ্ক show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
স্থির পূর্ণাঙ্ক symbolic;
স্থির পূর্ণাঙ্ক abbrev;
স্থির পূর্ণাঙ্ক abbrev_ref;
স্থির পূর্ণাঙ্ক abbrev_ref_strict;
স্থির পূর্ণাঙ্ক output_sq;

স্থির পূর্ণাঙ্ক stuck_long;
স্থির সংরচনা ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
স্থির পূর্ণাঙ্ক is_rev_argument(ধ্রুব বর্ণ *arg)
{
	স্থির ধ্রুব বর্ণ *rev_args[] = {
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
		রিক্ত
	};
	ধ্রুব বর্ণ **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	যদি ((*arg == '-') && isdigit(arg[1]))
		প্রত্যাগম 1;

	প্রতি (;;) {
		ধ্রুব বর্ণ *str = *p++;
		পূর্ণাঙ্ক len;
		যদি (!str)
			প্রত্যাগম 0;
		len = strlen(str);
		যদি (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			প্রত্যাগম 1;
	}
}

/* Output argument as a string, either SQ or normal */
স্থির শূন্য show(ধ্রুব বর্ণ *arg)
{
	যদি (output_sq) {
		পূর্ণাঙ্ক sq = '\'', ch;

		putchar(sq);
		যাবত্ ((ch = *arg++)) {
			যদি (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	অন্যথা
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
স্থির শূন্য show_with_type(পূর্ণাঙ্ক type, ধ্রুব বর্ণ *arg)
{
	যদি (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
স্থির শূন্য show_rev(পূর্ণাঙ্ক type, ধ্রুব সংরচনা object_id *oid, ধ্রুব বর্ণ *name)
{
	যদি (!(filter & DO_REVS))
		প্রত্যাগম;
	def = রিক্ত;

	যদি ((symbolic || abbrev_ref) && name) {
		যদি (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			সংরচনা object_id discard;
			বর্ণ *full;

			বিকল্প (repo_dwim_ref(the_repository, name,
					      strlen(name), &discard, &full,
					      0)) {
			পক্ষ 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				ভঙ্গ;
			পক্ষ 1: /* happy */
				যদি (abbrev_ref) {
					বর্ণ *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					মুক্ত(old);
				}
				show_with_type(type, full);
				ভঙ্গ;
			স্বভাব: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				ভঙ্গ;
			}
			মুক্ত(full);
		} অন্যথা {
			show_with_type(type, name);
		}
	}
	অন্যথা যদি (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	অন্যথা
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
স্থির পূর্ণাঙ্ক show_flag(ধ্রুব বর্ণ *arg)
{
	যদি (!(filter & DO_FLAGS))
		প্রত্যাগম 0;
	যদি (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		প্রত্যাগম 1;
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক show_default(শূন্য)
{
	ধ্রুব বর্ণ *s = def;

	যদি (s) {
		সংরচনা object_id oid;

		def = রিক্ত;
		যদি (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			প্রত্যাগম 1;
		}
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক show_reference(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data UNUSED)
{
	যদি (ref_excluded(&ref_excludes, ref->name))
		প্রত্যাগম 0;
	show_rev(NORMAL, ref->oid, ref->name);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক anti_reference(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক show_abbrev(ধ্রুব সংরচনা object_id *oid, শূন্য *cb_data UNUSED)
{
	show_rev(NORMAL, oid, রিক্ত);
	প্রত্যাগম 0;
}

স্থির শূন্য show_datestring(ধ্রুব বর্ণ *flag, ধ্রুব বর্ণ *datestr)
{
	বর্ণ *buffer;

	/* date handling requires both flags and revs */
	যদি ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		প্রত্যাগম;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	মুক্ত(buffer);
}

স্থির পূর্ণাঙ্ক show_file(ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক output_prefix)
{
	show_default();
	যদি ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		যদি (output_prefix) {
			ধ্রুব বর্ণ *prefix = the_repository->prefix;
			বর্ণ *fname = prefix_filename(prefix, arg);
			show(fname);
			মুক্ত(fname);
		} অন্যথা
			show(arg);
		প্রত্যাগম 1;
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক try_difference(ধ্রুব বর্ণ *arg)
{
	ধ্রুব বর্ণ *dotdot;
	সংরচনা object_id start_oid;
	সংরচনা object_id end_oid;
	ধ্রুব বর্ণ *end;
	ধ্রুব বর্ণ *start;
	বর্ণ *to_free;
	পূর্ণাঙ্ক symmetric;
	স্থির ধ্রুব বর্ণ head_by_default[] = "HEAD";

	যদি (!(dotdot = strstr(arg, "..")))
		প্রত্যাগম 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	যদি (!*end)
		end = head_by_default;
	যদি (dotdot == arg)
		start = head_by_default;

	যদি (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		মুক্ত(to_free);
		প্রত্যাগম 0;
	}

	যদি (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		যদি (symmetric) {
			সংরচনা commit_list *exclude = রিক্ত;
			সংরচনা commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			যদি (!a || !b) {
				মুক্ত(to_free);
				প্রত্যাগম 0;
			}
			যদি (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			যাবত্ (exclude) {
				সংরচনা commit *commit = বের_করো_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, রিক্ত);
			}
		}
		মুক্ত(to_free);
		প্রত্যাগম 1;
	}
	মুক্ত(to_free);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক try_parent_shorthands(ধ্রুব বর্ণ *arg)
{
	ধ্রুব বর্ণ *mark;
	সংরচনা object_id oid;
	সংরচনা commit *commit;
	সংরচনা commit_list *parents;
	পূর্ণাঙ্ক parent_সংখ্যা;
	পূর্ণাঙ্ক include_rev = 0;
	পূর্ণাঙ্ক include_parents = 0;
	পূর্ণাঙ্ক exclude_parent = 0;
	বর্ণ *to_free;

	যদি ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		যদি (mark[2])
			প্রত্যাগম 0;
	} অন্যথা যদি ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		যদি (mark[2])
			প্রত্যাগম 0;
	} অন্যথা যদি ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		যদি (mark[2]) {
			বর্ণ *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			যদি (*end != '\0' || !exclude_parent)
				প্রত্যাগম 0;
		}
	} অন্যথা
		প্রত্যাগম 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	যদি (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		মুক্ত(to_free);
		প্রত্যাগম 0;
	}

	যদি (exclude_parent &&
	    exclude_parent > commit_list_গুনতি(commit->parents)) {
		মুক্ত(to_free);
		প্রত্যাগম 0;
	}

	যদি (include_rev)
		show_rev(NORMAL, &oid, arg);
	প্রতি (parents = commit->parents, parent_সংখ্যা = 1;
	     parents;
	     parents = parents->next, parent_সংখ্যা++) {
		বর্ণ *name = রিক্ত;

		যদি (exclude_parent && parent_সংখ্যা != exclude_parent)
			অনুবর্ত;

		যদি (symbolic)
			name = xstrfmt("%s^%d", arg, parent_সংখ্যা);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		মুক্ত(name);
	}

	মুক্ত(to_free);
	প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক parseopt_dump(ধ্রুব সংরচনা option *o, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা strbuf *parsed = o->মান;
	যদি (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	অন্যথা যদি (o->short_name && (o->long_name == রিক্ত || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	অন্যথা
		strbuf_addf(parsed, " --%s", o->long_name);
	যদি (arg) {
		যদি (!stuck_long)
			strbuf_addch(parsed, ' ');
		অন্যথা যদি (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	প্রত্যাগম 0;
}

স্থির ধ্রুব বর্ণ *skipspaces(ধ্রুব বর্ণ *s)
{
	যাবত্ (isspace(*s))
		s++;
	প্রত্যাগম s;
}

স্থির বর্ণ *findspace(ধ্রুব বর্ণ *s)
{
	প্রতি (; *s; s++)
		যদি (isspace(*s))
			প্রত্যাগম (বর্ণ*)s;
	প্রত্যাগম রিক্ত;
}

স্থির পূর্ণাঙ্ক cmd_parseopt(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix)
{
	পূর্ণাঙ্ক keep_dashdash = 0, stop_at_non_option = 0;
	বর্ণ ধ্রুব * ধ্রুব parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		রিক্ত
	};
	সংরচনা option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_END(),
	};
	সংরচনা strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	সংরচনা strvec longnames = STRVEC_INIT;
	সংরচনা strvec usage = STRVEC_INIT;
	সংরচনা option *opts = রিক্ত;
	size_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	যদি (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	প্রতি (;;) {
		strbuf_reset(&sb);
		যদি (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		যদি (!strcmp("--", sb.buf)) {
			যদি (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			ভঙ্গ;
		}

		strvec_ঠেলো(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	যাবত্ (strbuf_getline(&sb, stdin) != EOF) {
		ধ্রুব বর্ণ *s;
		বর্ণ *help;
		সংরচনা option *o;

		যদি (!sb.len)
			অনুবর্ত;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, পরিমাণ(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		যদি (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			অনুবর্ত;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->মান = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		যদি (!s)
			s = help;

		যদি (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		যদি (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} অন্যথা যদি (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (পূর্ণাঙ্ক)(s - sb.buf), sb.buf);
		} অন্যথা {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (পূর্ণাঙ্ক)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		যাবত্ (s < help) {
			বিকল্প (*s++) {
			পক্ষ '=':
				o->flags &= ~PARSE_OPT_NOARG;
				অনুবর্ত;
			পক্ষ '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				অনুবর্ত;
			পক্ষ '!':
				o->flags |= PARSE_OPT_NONEG;
				অনুবর্ত;
			পক্ষ '*':
				o->flags |= PARSE_OPT_HIDDEN;
				অনুবর্ত;
			}
			s--;
			ভঙ্গ;
		}

		যদি (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, পরিমাণ(*opts));
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
	প্রতি (size_t i = 0; i < opts_nr; i++) {
		মুক্ত((বর্ণ *) opts[i].help);
		মুক্ত((বর্ণ *) opts[i].argh);
	}
	মুক্ত(opts);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক cmd_sq_quote(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv)
{
	সংরচনা strbuf buf = STRBUF_INIT;

	যদি (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	প্রত্যাগম 0;
}

স্থির শূন্য die_no_single_rev(পূর্ণাঙ্ক quiet)
{
	যদি (quiet)
		exit(1);
	অন্যথা
		die(_("Needed a single revision"));
}

স্থির ধ্রুব বর্ণ builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
স্থির পূর্ণাঙ্ক opt_with_মান(ধ্রুব বর্ণ *arg, ধ্রুব বর্ণ *opt, ধ্রুব বর্ণ **মান)
{
	যদি (skip_prefix(arg, opt, &arg)) {
		যদি (!*arg) {
			*মান = রিক্ত;
			প্রত্যাগম 1;
		}
		যদি (*arg++ == '=') {
			*মান = arg;
			প্রত্যাগম 1;
		}
	}
	প্রত্যাগম 0;
}

স্থির শূন্য handle_ref_opt(ধ্রুব বর্ণ *pattern, ধ্রুব বর্ণ *prefix)
{
	যদি (pattern) {
		সংরচনা refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, রিক্ত, &opts);
	} অন্যথা {
		সংরচনা refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = strlen(prefix),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, রিক্ত, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

গণনা format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

গণনা default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

স্থির শূন্য print_path(ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *prefix,
		       গণনা format_type format, গণনা default_type def)
{
	সংরচনা strbuf sb = STRBUF_INIT;
	গণনা path_format fmt;

	যদি (format == FORMAT_DEFAULT) {
		বিকল্প (def) {
		পক্ষ DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			ভঙ্গ;
		পক্ষ DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			ভঙ্গ;
		পক্ষ DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			ভঙ্গ;
		পক্ষ DEFAULT_UNMODIFIED:
		স্বভাব:
			fmt = PATH_FORMAT_UNMODIFIED;
			ভঙ্গ;
		}
	} অন্যথা {
		বিকল্প (format) {
		পক্ষ FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			ভঙ্গ;
		পক্ষ FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			ভঙ্গ;
		স্বভাব:
			fmt = PATH_FORMAT_UNMODIFIED;
			ভঙ্গ;
		}
	}

	format_path(&sb, path, prefix, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

পূর্ণাঙ্ক cmd_rev_parse(পূর্ণাঙ্ক argc,
		  ধ্রুব বর্ণ **argv,
		  ধ্রুব বর্ণ *prefix,
		  সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক i, as_is = 0, verify = 0, quiet = 0, revs_গুনতি = 0, type = 0;
	ধ্রুব সংরচনা git_hash_algo *output_algo = রিক্ত;
	ধ্রুব সংরচনা git_hash_algo *compat = রিক্ত;
	পূর্ণাঙ্ক did_repo_setup = 0;
	পূর্ণাঙ্ক has_dashdash = 0;
	পূর্ণাঙ্ক output_prefix = 0;
	সংরচনা object_id oid;
	unsigned পূর্ণাঙ্ক flags = 0;
	ধ্রুব বর্ণ *name = রিক্ত;
	সংরচনা strbuf buf = STRBUF_INIT;
	পূর্ণাঙ্ক seen_end_of_options = 0;
	গণনা format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	যদি (argc > 1 && !strcmp("--parseopt", argv[1]))
		প্রত্যাগম cmd_parseopt(argc - 1, argv + 1, prefix);

	যদি (argc > 1 && !strcmp("--sq-quote", argv[1]))
		প্রত্যাগম cmd_sq_quote(argc - 2, argv + 2);

	যদি (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	প্রতি (i = 1; i < argc; i++) {
		যদি (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			ভঙ্গ;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	যদি (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, রিক্ত);
		প্রত্যাগম 0;
	}

	প্রতি (i = 1; i < argc; i++) {
		ধ্রুব বর্ণ *arg = argv[i];

		যদি (as_is) {
			যদি (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			অনুবর্ত;
		}

		যদি (!seen_end_of_options) {
			যদি (!strcmp(arg, "--local-env-vars")) {
				পূর্ণাঙ্ক i;
				প্রতি (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--resolve-git-dir")) {
				ধ্রুব বর্ণ *gitdir = argv[++i];
				যদি (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				যদি (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				অনুবর্ত;
			}
		}

		/* The rest of the options require a git repository. */
		যদি (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, রিক্ত);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		যদি (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			যদি (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			অনুবর্ত;
		}

		যদি (!seen_end_of_options && *arg == '-') {
			যদি (!strcmp(arg, "--git-path")) {
				যদি (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				অনুবর্ত;
			}
			যদি (!strcmp(arg,"-n")) {
				যদি (++i >= argc)
					die(_("-n requires an argument"));
				যদি ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				অনুবর্ত;
			}
			যদি (starts_with(arg, "-n")) {
				যদি ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				অনুবর্ত;
			}
			যদি (opt_with_মান(arg, "--path-format", &arg)) {
				যদি (!arg)
					die(_("--path-format requires an argument"));
				যদি (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} অন্যথা যদি (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} অন্যথা {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--default")) {
				def = argv[++i];
				যদি (!def)
					die(_("--default requires an argument"));
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				যদি (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				অনুবর্ত;
			}
			যদি (opt_with_মান(arg, "--output-object-format", &arg)) {
				যদি (!arg)
					die(_("no object format specified"));
				যদি (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					অনুবর্ত;
				}
				অন্যথা যদি (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					অনুবর্ত;
				}
				অন্যথা die(_("unsupported object format: %s"), arg);
			}
			যদি (opt_with_মান(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				যদি (!arg)
					অনুবর্ত;
				abbrev = strtoul(arg, রিক্ত, 10);
				যদি (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				অন্যথা যদি ((পূর্ণাঙ্ক)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--sq")) {
				output_sq = 1;
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				অনুবর্ত;
			}
			যদি (opt_with_মান(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				যদি (arg) {
					যদি (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					অন্যথা যদি (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					অন্যথা
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, রিক্ত);
				clear_ref_exclusions(&ref_excludes);
				অনুবর্ত;
			}
			যদি (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, রিক্ত);
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--bisect")) {
				বর্ণ *prefix;
				বর্ণ *term_bad = রিক্ত;
				বর্ণ *term_good = রিক্ত;
				সংরচনা refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&term_bad, &term_good);
				prefix = xstrfmt("refs/bisect/%s", term_bad);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, রিক্ত, &opts);
				মুক্ত(prefix);
				prefix = xstrfmt("refs/bisect/%s", term_good);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, রিক্ত, &opts);
				মুক্ত(prefix);
				মুক্ত(term_good);
				মুক্ত(term_bad);
				অনুবর্ত;
			}
			যদি (opt_with_মান(arg, "--branches", &arg)) {
				যদি (ref_excludes.hidden_refs_configured)
					প্রত্যাগম error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				অনুবর্ত;
			}
			যদি (opt_with_মান(arg, "--tags", &arg)) {
				যদি (ref_excludes.hidden_refs_configured)
					প্রত্যাগম error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				অনুবর্ত;
			}
			যদি (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, রিক্ত);
				অনুবর্ত;
			}
			যদি (opt_with_মান(arg, "--remotes", &arg)) {
				যদি (ref_excludes.hidden_refs_configured)
					প্রত্যাগম error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				অনুবর্ত;
			}
			যদি (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				অনুবর্ত;
			}
			যদি (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--show-toplevel")) {
				ধ্রুব বর্ণ *work_বৃক্ষ = repo_get_work_বৃক্ষ(the_repository);
				যদি (work_বৃক্ষ)
					print_path(work_বৃক্ষ, prefix, format, DEFAULT_UNMODIFIED);
				অন্যথা
					die(_("this operation must be run in a work tree"));
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--show-superproject-working-tree")) {
				সংরচনা strbuf superproject = STRBUF_INIT;
				যদি (get_superproject_working_বৃক্ষ(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--show-prefix")) {
				যদি (prefix)
					puts(prefix);
				অন্যথা
					putchar('\n');
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--show-cdup")) {
				ধ্রুব বর্ণ *pfx = prefix;
				যদি (!is_inside_work_বৃক্ষ(the_repository)) {
					ধ্রুব বর্ণ *work_বৃক্ষ =
						repo_get_work_বৃক্ষ(the_repository);
					যদি (work_বৃক্ষ)
						printf("%s\n", work_বৃক্ষ);
					অনুবর্ত;
				}
				যাবত্ (pfx) {
					pfx = strchr(pfx, '/');
					যদি (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				ধ্রুব বর্ণ *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				বর্ণ *cwd;
				পূর্ণাঙ্ক len;
				গণনা format_type wanted = format;
				যদি (arg[2] == 'g') {	/* --git-dir */
					যদি (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						অনুবর্ত;
					}
					যদি (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						অনুবর্ত;
					}
				} অন্যথা {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					যদি (!gitdir && !prefix)
						gitdir = ".git";
					যদি (gitdir) {
						সংরচনা strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						অনুবর্ত;
					}
				}
				cwd = xgetcwd();
				len = strlen(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				মুক্ত(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_বৃক্ষ(the_repository) ? "true"
						: "false");
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--shared-index-path")) {
				যদি (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				যদি (the_repository->index->split_index) {
					ধ্রুব সংরচনা object_id *oid = &the_repository->index->split_index->base_oid;
					ধ্রুব বর্ণ *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				অনুবর্ত;
			}
			যদি (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				অনুবর্ত;
			}
			যদি (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				অনুবর্ত;
			}
			যদি (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				অনুবর্ত;
			}
			যদি (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				অনুবর্ত;
			}
			যদি (opt_with_মান(arg, "--show-object-format", &arg)) {
				ধ্রুব বর্ণ *val = arg ? arg : "storage";

				যদি (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				যদি (!strcmp(val, "compat")) {
					যদি (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					অন্যথা
						putchar('\n');
				} অন্যথা {
					puts(the_hash_algo->name);
				}
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				অনুবর্ত;
			}
			যদি (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				যদি (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				অনুবর্ত;
			}
			যদি (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			অনুবর্ত;
		}

		/* Not a flag argument */
		যদি (try_difference(arg))
			অনুবর্ত;
		যদি (try_parent_shorthands(arg))
			অনুবর্ত;
		name = arg;
		type = NORMAL;
		যদি (*arg == '^') {
			name++;
			type = REVERSED;
		}
		যদি (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			যদি (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			যদি (verify)
				revs_গুনতি++;
			অন্যথা
				show_rev(type, &oid, name);
			অনুবর্ত;
		}
		যদি (verify)
			die_no_single_rev(quiet);
		যদি (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		যদি (!show_file(arg, output_prefix))
			অনুবর্ত;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	যদি (verify) {
		যদি (revs_গুনতি == 1) {
			show_rev(type, &oid, name);
			প্রত্যাগম 0;
		} অন্যথা যদি (revs_গুনতি == 0 && show_default())
			প্রত্যাগম 0;
		die_no_single_rev(quiet);
	} অন্যথা
		show_default();
	প্রত্যাগম 0;
}
