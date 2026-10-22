/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"

#شامل "abspath.h"
#شامل "bisect.h"
#شامل "config.h"
#شامل "commit.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "hex.h"
#شامل "refs.h"
#شامل "quote.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "diff.h"
#شامل "read-cache-ll.h"
#شامل "repo-settings.h"
#شامل "repository.h"
#شامل "revision.h"
#شامل "setup.h"
#شامل "split-index.h"
#شامل "submodule.h"
#شامل "commit-reach.h"
#شامل "shallow.h"
#شامل "object-file-convert.h"

#define DO_REVS		1
#define DO_NOREV	2
#define DO_FLAGS	4
#define DO_NONFLAGS	8
ایستا صحیح filter = ~0;

ایستا ثابت نویسه *def;

#define NORMAL 0
#define REVERSED 1
ایستا صحیح show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
ایستا صحیح symbolic;
ایستا صحیح abbrev;
ایستا صحیح abbrev_ref;
ایستا صحیح abbrev_ref_strict;
ایستا صحیح output_sq;

ایستا صحیح stuck_long;
ایستا ساختار ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
ایستا صحیح is_rev_argument(ثابت نویسه *arg)
{
	ایستا ثابت نویسه *rev_args[] = {
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
		پوچ
	};
	ثابت نویسه **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	اگر ((*arg == '-') && isdigit(arg[1]))
		بازگشت 1;

	برای (;;) {
		ثابت نویسه *str = *p++;
		صحیح len;
		اگر (!str)
			بازگشت 0;
		len = strlen(str);
		اگر (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			بازگشت 1;
	}
}

/* Output argument as a string, either SQ or normal */
ایستا تهی show(ثابت نویسه *arg)
{
	اگر (output_sq) {
		صحیح sq = '\'', ch;

		putchar(sq);
		وقتی ((ch = *arg++)) {
			اگر (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	وگرنه
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
ایستا تهی show_with_type(صحیح type, ثابت نویسه *arg)
{
	اگر (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
ایستا تهی show_rev(صحیح type, ثابت ساختار object_id *oid, ثابت نویسه *name)
{
	اگر (!(filter & DO_REVS))
		بازگشت;
	def = پوچ;

	اگر ((symbolic || abbrev_ref) && name) {
		اگر (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			ساختار object_id discard;
			نویسه *full;

			انتخاب (repo_dwim_ref(the_repository, name,
					      strlen(name), &discard, &full,
					      0)) {
			حالت 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				بشکن;
			حالت 1: /* happy */
				اگر (abbrev_ref) {
					نویسه *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					آزاد(old);
				}
				show_with_type(type, full);
				بشکن;
			پیشفرض: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				بشکن;
			}
			آزاد(full);
		} وگرنه {
			show_with_type(type, name);
		}
	}
	وگرنه اگر (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	وگرنه
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
ایستا صحیح show_flag(ثابت نویسه *arg)
{
	اگر (!(filter & DO_FLAGS))
		بازگشت 0;
	اگر (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		بازگشت 1;
	}
	بازگشت 0;
}

ایستا صحیح show_default(تهی)
{
	ثابت نویسه *s = def;

	اگر (s) {
		ساختار object_id oid;

		def = پوچ;
		اگر (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			بازگشت 1;
		}
	}
	بازگشت 0;
}

ایستا صحیح show_reference(ثابت ساختار reference *ref, تهی *cb_data UNUSED)
{
	اگر (ref_excluded(&ref_excludes, ref->name))
		بازگشت 0;
	show_rev(NORMAL, ref->oid, ref->name);
	بازگشت 0;
}

ایستا صحیح anti_reference(ثابت ساختار reference *ref, تهی *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	بازگشت 0;
}

ایستا صحیح show_abbrev(ثابت ساختار object_id *oid, تهی *cb_data UNUSED)
{
	show_rev(NORMAL, oid, پوچ);
	بازگشت 0;
}

ایستا تهی show_datestring(ثابت نویسه *flag, ثابت نویسه *datestr)
{
	نویسه *buffer;

	/* date handling requires both flags and revs */
	اگر ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		بازگشت;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	آزاد(buffer);
}

ایستا صحیح show_file(ثابت نویسه *arg, صحیح output_prefix)
{
	show_default();
	اگر ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		اگر (output_prefix) {
			ثابت نویسه *prefix = the_repository->prefix;
			نویسه *fname = prefix_filename(prefix, arg);
			show(fname);
			آزاد(fname);
		} وگرنه
			show(arg);
		بازگشت 1;
	}
	بازگشت 0;
}

ایستا صحیح try_difference(ثابت نویسه *arg)
{
	ثابت نویسه *dotdot;
	ساختار object_id start_oid;
	ساختار object_id end_oid;
	ثابت نویسه *end;
	ثابت نویسه *start;
	نویسه *to_free;
	صحیح symmetric;
	ایستا ثابت نویسه head_by_default[] = "HEAD";

	اگر (!(dotdot = strstr(arg, "..")))
		بازگشت 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	اگر (!*end)
		end = head_by_default;
	اگر (dotdot == arg)
		start = head_by_default;

	اگر (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		آزاد(to_free);
		بازگشت 0;
	}

	اگر (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		اگر (symmetric) {
			ساختار commit_list *exclude = پوچ;
			ساختار commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			اگر (!a || !b) {
				آزاد(to_free);
				بازگشت 0;
			}
			اگر (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			وقتی (exclude) {
				ساختار commit *commit = برداشت_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, پوچ);
			}
		}
		آزاد(to_free);
		بازگشت 1;
	}
	آزاد(to_free);
	بازگشت 0;
}

ایستا صحیح try_parent_shorthands(ثابت نویسه *arg)
{
	ثابت نویسه *mark;
	ساختار object_id oid;
	ساختار commit *commit;
	ساختار commit_list *parents;
	صحیح parent_عدد;
	صحیح include_rev = 0;
	صحیح include_parents = 0;
	صحیح exclude_parent = 0;
	نویسه *to_free;

	اگر ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		اگر (mark[2])
			بازگشت 0;
	} وگرنه اگر ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		اگر (mark[2])
			بازگشت 0;
	} وگرنه اگر ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		اگر (mark[2]) {
			نویسه *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			اگر (*end != '\0' || !exclude_parent)
				بازگشت 0;
		}
	} وگرنه
		بازگشت 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	اگر (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		آزاد(to_free);
		بازگشت 0;
	}

	اگر (exclude_parent &&
	    exclude_parent > commit_list_شمار(commit->parents)) {
		آزاد(to_free);
		بازگشت 0;
	}

	اگر (include_rev)
		show_rev(NORMAL, &oid, arg);
	برای (parents = commit->parents, parent_عدد = 1;
	     parents;
	     parents = parents->next, parent_عدد++) {
		نویسه *name = پوچ;

		اگر (exclude_parent && parent_عدد != exclude_parent)
			ادامه;

		اگر (symbolic)
			name = xstrfmt("%s^%d", arg, parent_عدد);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		آزاد(name);
	}

	آزاد(to_free);
	بازگشت 1;
}

ایستا صحیح parseopt_dump(ثابت ساختار option *o, ثابت نویسه *arg, صحیح unset)
{
	ساختار strbuf *parsed = o->مقدار;
	اگر (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	وگرنه اگر (o->short_name && (o->long_name == پوچ || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	وگرنه
		strbuf_addf(parsed, " --%s", o->long_name);
	اگر (arg) {
		اگر (!stuck_long)
			strbuf_addch(parsed, ' ');
		وگرنه اگر (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	بازگشت 0;
}

ایستا ثابت نویسه *skipspaces(ثابت نویسه *s)
{
	وقتی (isspace(*s))
		s++;
	بازگشت s;
}

ایستا نویسه *findspace(ثابت نویسه *s)
{
	برای (; *s; s++)
		اگر (isspace(*s))
			بازگشت (نویسه*)s;
	بازگشت پوچ;
}

ایستا صحیح cmd_parseopt(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix)
{
	صحیح keep_dashdash = 0, stop_at_non_option = 0;
	نویسه ثابت * ثابت parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		پوچ
	};
	ساختار option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_END(),
	};
	ساختار strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	ساختار strvec longnames = STRVEC_INIT;
	ساختار strvec usage = STRVEC_INIT;
	ساختار option *opts = پوچ;
	size_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	اگر (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	برای (;;) {
		strbuf_reset(&sb);
		اگر (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		اگر (!strcmp("--", sb.buf)) {
			اگر (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			بشکن;
		}

		strvec_افزودن(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	وقتی (strbuf_getline(&sb, stdin) != EOF) {
		ثابت نویسه *s;
		نویسه *help;
		ساختار option *o;

		اگر (!sb.len)
			ادامه;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, اندازه(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		اگر (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			ادامه;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->مقدار = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		اگر (!s)
			s = help;

		اگر (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		اگر (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} وگرنه اگر (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (صحیح)(s - sb.buf), sb.buf);
		} وگرنه {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (صحیح)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		وقتی (s < help) {
			انتخاب (*s++) {
			حالت '=':
				o->flags &= ~PARSE_OPT_NOARG;
				ادامه;
			حالت '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				ادامه;
			حالت '!':
				o->flags |= PARSE_OPT_NONEG;
				ادامه;
			حالت '*':
				o->flags |= PARSE_OPT_HIDDEN;
				ادامه;
			}
			s--;
			بشکن;
		}

		اگر (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, اندازه(*opts));
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
	برای (size_t i = 0; i < opts_nr; i++) {
		آزاد((نویسه *) opts[i].help);
		آزاد((نویسه *) opts[i].argh);
	}
	آزاد(opts);
	بازگشت 0;
}

ایستا صحیح cmd_sq_quote(صحیح argc, ثابت نویسه **argv)
{
	ساختار strbuf buf = STRBUF_INIT;

	اگر (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	بازگشت 0;
}

ایستا تهی die_no_single_rev(صحیح quiet)
{
	اگر (quiet)
		exit(1);
	وگرنه
		die(_("Needed a single revision"));
}

ایستا ثابت نویسه builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
ایستا صحیح opt_with_مقدار(ثابت نویسه *arg, ثابت نویسه *opt, ثابت نویسه **مقدار)
{
	اگر (skip_prefix(arg, opt, &arg)) {
		اگر (!*arg) {
			*مقدار = پوچ;
			بازگشت 1;
		}
		اگر (*arg++ == '=') {
			*مقدار = arg;
			بازگشت 1;
		}
	}
	بازگشت 0;
}

ایستا تهی handle_ref_opt(ثابت نویسه *pattern, ثابت نویسه *prefix)
{
	اگر (pattern) {
		ساختار refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, پوچ, &opts);
	} وگرنه {
		ساختار refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = strlen(prefix),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, پوچ, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

شمارش format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

شمارش default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

ایستا تهی print_path(ثابت نویسه *path, ثابت نویسه *prefix,
		       شمارش format_type format, شمارش default_type def)
{
	ساختار strbuf sb = STRBUF_INIT;
	شمارش path_format fmt;

	اگر (format == FORMAT_DEFAULT) {
		انتخاب (def) {
		حالت DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			بشکن;
		حالت DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			بشکن;
		حالت DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			بشکن;
		حالت DEFAULT_UNMODIFIED:
		پیشفرض:
			fmt = PATH_FORMAT_UNMODIFIED;
			بشکن;
		}
	} وگرنه {
		انتخاب (format) {
		حالت FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			بشکن;
		حالت FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			بشکن;
		پیشفرض:
			fmt = PATH_FORMAT_UNMODIFIED;
			بشکن;
		}
	}

	format_path(&sb, path, prefix, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

صحیح cmd_rev_parse(صحیح argc,
		  ثابت نویسه **argv,
		  ثابت نویسه *prefix,
		  ساختار repository *repo UNUSED)
{
	صحیح i, as_is = 0, verify = 0, quiet = 0, revs_شمار = 0, type = 0;
	ثابت ساختار git_hash_algo *output_algo = پوچ;
	ثابت ساختار git_hash_algo *compat = پوچ;
	صحیح did_repo_setup = 0;
	صحیح has_dashdash = 0;
	صحیح output_prefix = 0;
	ساختار object_id oid;
	unsigned صحیح flags = 0;
	ثابت نویسه *name = پوچ;
	ساختار strbuf buf = STRBUF_INIT;
	صحیح seen_end_of_options = 0;
	شمارش format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	اگر (argc > 1 && !strcmp("--parseopt", argv[1]))
		بازگشت cmd_parseopt(argc - 1, argv + 1, prefix);

	اگر (argc > 1 && !strcmp("--sq-quote", argv[1]))
		بازگشت cmd_sq_quote(argc - 2, argv + 2);

	اگر (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	برای (i = 1; i < argc; i++) {
		اگر (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			بشکن;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	اگر (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, پوچ);
		بازگشت 0;
	}

	برای (i = 1; i < argc; i++) {
		ثابت نویسه *arg = argv[i];

		اگر (as_is) {
			اگر (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			ادامه;
		}

		اگر (!seen_end_of_options) {
			اگر (!strcmp(arg, "--local-env-vars")) {
				صحیح i;
				برای (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				ادامه;
			}
			اگر (!strcmp(arg, "--resolve-git-dir")) {
				ثابت نویسه *gitdir = argv[++i];
				اگر (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				اگر (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				ادامه;
			}
		}

		/* The rest of the options require a git repository. */
		اگر (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, پوچ);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		اگر (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			اگر (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			ادامه;
		}

		اگر (!seen_end_of_options && *arg == '-') {
			اگر (!strcmp(arg, "--git-path")) {
				اگر (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				ادامه;
			}
			اگر (!strcmp(arg,"-n")) {
				اگر (++i >= argc)
					die(_("-n requires an argument"));
				اگر ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				ادامه;
			}
			اگر (starts_with(arg, "-n")) {
				اگر ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				ادامه;
			}
			اگر (opt_with_مقدار(arg, "--path-format", &arg)) {
				اگر (!arg)
					die(_("--path-format requires an argument"));
				اگر (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} وگرنه اگر (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} وگرنه {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				ادامه;
			}
			اگر (!strcmp(arg, "--default")) {
				def = argv[++i];
				اگر (!def)
					die(_("--default requires an argument"));
				ادامه;
			}
			اگر (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				اگر (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				ادامه;
			}
			اگر (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				ادامه;
			}
			اگر (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				ادامه;
			}
			اگر (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				ادامه;
			}
			اگر (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				ادامه;
			}
			اگر (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				ادامه;
			}
			اگر (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				ادامه;
			}
			اگر (opt_with_مقدار(arg, "--output-object-format", &arg)) {
				اگر (!arg)
					die(_("no object format specified"));
				اگر (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					ادامه;
				}
				وگرنه اگر (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					ادامه;
				}
				وگرنه die(_("unsupported object format: %s"), arg);
			}
			اگر (opt_with_مقدار(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				اگر (!arg)
					ادامه;
				abbrev = strtoul(arg, پوچ, 10);
				اگر (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				وگرنه اگر ((صحیح)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				ادامه;
			}
			اگر (!strcmp(arg, "--sq")) {
				output_sq = 1;
				ادامه;
			}
			اگر (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				ادامه;
			}
			اگر (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				ادامه;
			}
			اگر (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				ادامه;
			}
			اگر (opt_with_مقدار(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				اگر (arg) {
					اگر (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					وگرنه اگر (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					وگرنه
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				ادامه;
			}
			اگر (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, پوچ);
				clear_ref_exclusions(&ref_excludes);
				ادامه;
			}
			اگر (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, پوچ);
				ادامه;
			}
			اگر (!strcmp(arg, "--bisect")) {
				نویسه *prefix;
				نویسه *term_bad = پوچ;
				نویسه *term_good = پوچ;
				ساختار refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&term_bad, &term_good);
				prefix = xstrfmt("refs/bisect/%s", term_bad);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, پوچ, &opts);
				آزاد(prefix);
				prefix = xstrfmt("refs/bisect/%s", term_good);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, پوچ, &opts);
				آزاد(prefix);
				آزاد(term_good);
				آزاد(term_bad);
				ادامه;
			}
			اگر (opt_with_مقدار(arg, "--branches", &arg)) {
				اگر (ref_excludes.hidden_refs_configured)
					بازگشت error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				ادامه;
			}
			اگر (opt_with_مقدار(arg, "--tags", &arg)) {
				اگر (ref_excludes.hidden_refs_configured)
					بازگشت error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				ادامه;
			}
			اگر (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, پوچ);
				ادامه;
			}
			اگر (opt_with_مقدار(arg, "--remotes", &arg)) {
				اگر (ref_excludes.hidden_refs_configured)
					بازگشت error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				ادامه;
			}
			اگر (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				ادامه;
			}
			اگر (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				ادامه;
			}
			اگر (!strcmp(arg, "--show-toplevel")) {
				ثابت نویسه *work_درخت = repo_get_work_درخت(the_repository);
				اگر (work_درخت)
					print_path(work_درخت, prefix, format, DEFAULT_UNMODIFIED);
				وگرنه
					die(_("this operation must be run in a work tree"));
				ادامه;
			}
			اگر (!strcmp(arg, "--show-superproject-working-tree")) {
				ساختار strbuf superproject = STRBUF_INIT;
				اگر (get_superproject_working_درخت(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				ادامه;
			}
			اگر (!strcmp(arg, "--show-prefix")) {
				اگر (prefix)
					puts(prefix);
				وگرنه
					putchar('\n');
				ادامه;
			}
			اگر (!strcmp(arg, "--show-cdup")) {
				ثابت نویسه *pfx = prefix;
				اگر (!is_inside_work_درخت(the_repository)) {
					ثابت نویسه *work_درخت =
						repo_get_work_درخت(the_repository);
					اگر (work_درخت)
						printf("%s\n", work_درخت);
					ادامه;
				}
				وقتی (pfx) {
					pfx = strchr(pfx, '/');
					اگر (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				ادامه;
			}
			اگر (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				ثابت نویسه *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				نویسه *cwd;
				صحیح len;
				شمارش format_type wanted = format;
				اگر (arg[2] == 'g') {	/* --git-dir */
					اگر (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						ادامه;
					}
					اگر (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						ادامه;
					}
				} وگرنه {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					اگر (!gitdir && !prefix)
						gitdir = ".git";
					اگر (gitdir) {
						ساختار strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						ادامه;
					}
				}
				cwd = xgetcwd();
				len = strlen(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				آزاد(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				ادامه;
			}
			اگر (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				ادامه;
			}
			اگر (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				ادامه;
			}
			اگر (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_درخت(the_repository) ? "true"
						: "false");
				ادامه;
			}
			اگر (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				ادامه;
			}
			اگر (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				ادامه;
			}
			اگر (!strcmp(arg, "--shared-index-path")) {
				اگر (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				اگر (the_repository->index->split_index) {
					ثابت ساختار object_id *oid = &the_repository->index->split_index->base_oid;
					ثابت نویسه *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				ادامه;
			}
			اگر (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				ادامه;
			}
			اگر (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				ادامه;
			}
			اگر (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				ادامه;
			}
			اگر (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				ادامه;
			}
			اگر (opt_with_مقدار(arg, "--show-object-format", &arg)) {
				ثابت نویسه *val = arg ? arg : "storage";

				اگر (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				اگر (!strcmp(val, "compat")) {
					اگر (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					وگرنه
						putchar('\n');
				} وگرنه {
					puts(the_hash_algo->name);
				}
				ادامه;
			}
			اگر (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				ادامه;
			}
			اگر (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				اگر (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				ادامه;
			}
			اگر (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			ادامه;
		}

		/* Not a flag argument */
		اگر (try_difference(arg))
			ادامه;
		اگر (try_parent_shorthands(arg))
			ادامه;
		name = arg;
		type = NORMAL;
		اگر (*arg == '^') {
			name++;
			type = REVERSED;
		}
		اگر (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			اگر (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			اگر (verify)
				revs_شمار++;
			وگرنه
				show_rev(type, &oid, name);
			ادامه;
		}
		اگر (verify)
			die_no_single_rev(quiet);
		اگر (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		اگر (!show_file(arg, output_prefix))
			ادامه;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	اگر (verify) {
		اگر (revs_شمار == 1) {
			show_rev(type, &oid, name);
			بازگشت 0;
		} وگرنه اگر (revs_شمار == 0 && show_default())
			بازگشت 0;
		die_no_single_rev(quiet);
	} وگرنه
		show_default();
	بازگشت 0;
}
