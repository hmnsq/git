/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"

#ضمن "abspath.h"
#ضمن "bisect.h"
#ضمن "config.h"
#ضمن "commit.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "hex.h"
#ضمن "refs.h"
#ضمن "quote.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "diff.h"
#ضمن "read-cache-ll.h"
#ضمن "repo-settings.h"
#ضمن "repository.h"
#ضمن "revision.h"
#ضمن "setup.h"
#ضمن "split-index.h"
#ضمن "submodule.h"
#ضمن "commit-reach.h"
#ضمن "shallow.h"
#ضمن "object-file-convert.h"

#define DO_REVS		1
#define DO_NOREV	2
#define DO_FLAGS	4
#define DO_NONFLAGS	8
ساكن صحيح filter = ~0;

ساكن ثابت حرف *def;

#define NORMAL 0
#define REVERSED 1
ساكن صحيح show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
ساكن صحيح symbolic;
ساكن صحيح abbrev;
ساكن صحيح abbrev_ref;
ساكن صحيح abbrev_ref_strict;
ساكن صحيح output_sq;

ساكن صحيح stuck_long;
ساكن بنية ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
ساكن صحيح is_rev_argument(ثابت حرف *arg)
{
	ساكن ثابت حرف *rev_args[] = {
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
		عدم
	};
	ثابت حرف **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	אם ((*arg == '-') && isdigit(arg[1]))
		החזר 1;

	עבור (;;) {
		ثابت حرف *str = *p++;
		صحيح len;
		אם (!str)
			החזר 0;
		len = strlen(str);
		אם (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			החזר 1;
	}
}

/* Output argument as a string, either SQ or normal */
ساكن فراغ show(ثابت حرف *arg)
{
	אם (output_sq) {
		صحيح sq = '\'', ch;

		putchar(sq);
		כלעוד ((ch = *arg++)) {
			אם (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	אחרת
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
ساكن فراغ show_with_type(صحيح type, ثابت حرف *arg)
{
	אם (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
ساكن فراغ show_rev(صحيح type, ثابت بنية object_id *oid, ثابت حرف *name)
{
	אם (!(filter & DO_REVS))
		החזר;
	def = عدم;

	אם ((symbolic || abbrev_ref) && name) {
		אם (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			بنية object_id discard;
			حرف *full;

			بدل (repo_dwim_ref(the_repository, name,
					      strlen(name), &discard, &full,
					      0)) {
			حالة 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				اكسر;
			حالة 1: /* happy */
				אם (abbrev_ref) {
					حرف *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					حرر(old);
				}
				show_with_type(type, full);
				اكسر;
			افتراضي: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				اكسر;
			}
			حرر(full);
		} אחרת {
			show_with_type(type, name);
		}
	}
	אחרת אם (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	אחרת
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
ساكن صحيح show_flag(ثابت حرف *arg)
{
	אם (!(filter & DO_FLAGS))
		החזר 0;
	אם (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		החזר 1;
	}
	החזר 0;
}

ساكن صحيح show_default(فراغ)
{
	ثابت حرف *s = def;

	אם (s) {
		بنية object_id oid;

		def = عدم;
		אם (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			החזר 1;
		}
	}
	החזר 0;
}

ساكن صحيح show_reference(ثابت بنية reference *ref, فراغ *cb_data UNUSED)
{
	אם (ref_excluded(&ref_excludes, ref->name))
		החזר 0;
	show_rev(NORMAL, ref->oid, ref->name);
	החזר 0;
}

ساكن صحيح anti_reference(ثابت بنية reference *ref, فراغ *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	החזר 0;
}

ساكن صحيح show_abbrev(ثابت بنية object_id *oid, فراغ *cb_data UNUSED)
{
	show_rev(NORMAL, oid, عدم);
	החזר 0;
}

ساكن فراغ show_datestring(ثابت حرف *flag, ثابت حرف *datestr)
{
	حرف *buffer;

	/* date handling requires both flags and revs */
	אם ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		החזר;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	حرر(buffer);
}

ساكن صحيح show_file(ثابت حرف *arg, صحيح output_prefix)
{
	show_default();
	אם ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		אם (output_prefix) {
			ثابت حرف *prefix = the_repository->prefix;
			حرف *fname = prefix_filename(prefix, arg);
			show(fname);
			حرر(fname);
		} אחרת
			show(arg);
		החזר 1;
	}
	החזר 0;
}

ساكن صحيح try_difference(ثابت حرف *arg)
{
	ثابت حرف *dotdot;
	بنية object_id start_oid;
	بنية object_id end_oid;
	ثابت حرف *end;
	ثابت حرف *start;
	حرف *to_free;
	صحيح symmetric;
	ساكن ثابت حرف head_by_default[] = "HEAD";

	אם (!(dotdot = strstr(arg, "..")))
		החזר 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	אם (!*end)
		end = head_by_default;
	אם (dotdot == arg)
		start = head_by_default;

	אם (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		حرر(to_free);
		החזר 0;
	}

	אם (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		אם (symmetric) {
			بنية commit_list *exclude = عدم;
			بنية commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			אם (!a || !b) {
				حرر(to_free);
				החזר 0;
			}
			אם (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			כלעוד (exclude) {
				بنية commit *commit = pop_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, عدم);
			}
		}
		حرر(to_free);
		החזר 1;
	}
	حرر(to_free);
	החזר 0;
}

ساكن صحيح try_parent_shorthands(ثابت حرف *arg)
{
	ثابت حرف *mark;
	بنية object_id oid;
	بنية commit *commit;
	بنية commit_list *parents;
	صحيح parent_number;
	صحيح include_rev = 0;
	صحيح include_parents = 0;
	صحيح exclude_parent = 0;
	حرف *to_free;

	אם ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		אם (mark[2])
			החזר 0;
	} אחרת אם ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		אם (mark[2])
			החזר 0;
	} אחרת אם ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		אם (mark[2]) {
			حرف *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			אם (*end != '\0' || !exclude_parent)
				החזר 0;
		}
	} אחרת
		החזר 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	אם (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		حرر(to_free);
		החזר 0;
	}

	אם (exclude_parent &&
	    exclude_parent > commit_list_count(commit->parents)) {
		حرر(to_free);
		החזר 0;
	}

	אם (include_rev)
		show_rev(NORMAL, &oid, arg);
	עבור (parents = commit->parents, parent_number = 1;
	     parents;
	     parents = parents->next, parent_number++) {
		حرف *name = عدم;

		אם (exclude_parent && parent_number != exclude_parent)
			استمر;

		אם (symbolic)
			name = xstrfmt("%s^%d", arg, parent_number);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		حرر(name);
	}

	حرر(to_free);
	החזר 1;
}

ساكن صحيح parseopt_dump(ثابت بنية option *o, ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *parsed = o->value;
	אם (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	אחרת אם (o->short_name && (o->long_name == عدم || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	אחרת
		strbuf_addf(parsed, " --%s", o->long_name);
	אם (arg) {
		אם (!stuck_long)
			strbuf_addch(parsed, ' ');
		אחרת אם (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	החזר 0;
}

ساكن ثابت حرف *skipspaces(ثابت حرف *s)
{
	כלעוד (isspace(*s))
		s++;
	החזר s;
}

ساكن حرف *findspace(ثابت حرف *s)
{
	עבור (; *s; s++)
		אם (isspace(*s))
			החזר (حرف*)s;
	החזר عدم;
}

ساكن صحيح cmd_parseopt(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix)
{
	صحيح keep_dashdash = 0, stop_at_non_option = 0;
	حرف ثابت * ثابت parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		عدم
	};
	بنية option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_END(),
	};
	بنية strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	بنية strvec longnames = STRVEC_INIT;
	بنية strvec usage = STRVEC_INIT;
	بنية option *opts = عدم;
	size_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	אם (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	עבור (;;) {
		strbuf_reset(&sb);
		אם (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		אם (!strcmp("--", sb.buf)) {
			אם (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			اكسر;
		}

		strvec_push(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	כלעוד (strbuf_getline(&sb, stdin) != EOF) {
		ثابت حرف *s;
		حرف *help;
		بنية option *o;

		אם (!sb.len)
			استمر;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, حجم(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		אם (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			استمر;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->value = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		אם (!s)
			s = help;

		אם (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		אם (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} אחרת אם (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (صحيح)(s - sb.buf), sb.buf);
		} אחרת {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (صحيح)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		כלעוד (s < help) {
			بدل (*s++) {
			حالة '=':
				o->flags &= ~PARSE_OPT_NOARG;
				استمر;
			حالة '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				استمر;
			حالة '!':
				o->flags |= PARSE_OPT_NONEG;
				استمر;
			حالة '*':
				o->flags |= PARSE_OPT_HIDDEN;
				استمر;
			}
			s--;
			اكسر;
		}

		אם (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, حجم(*opts));
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
	עבור (size_t i = 0; i < opts_nr; i++) {
		حرر((حرف *) opts[i].help);
		حرر((حرف *) opts[i].argh);
	}
	حرر(opts);
	החזר 0;
}

ساكن صحيح cmd_sq_quote(صحيح argc, ثابت حرف **argv)
{
	بنية strbuf buf = STRBUF_INIT;

	אם (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	החזר 0;
}

ساكن فراغ die_no_single_rev(صحيح quiet)
{
	אם (quiet)
		exit(1);
	אחרת
		die(_("Needed a single revision"));
}

ساكن ثابت حرف builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
ساكن صحيح opt_with_value(ثابت حرف *arg, ثابت حرف *opt, ثابت حرف **value)
{
	אם (skip_prefix(arg, opt, &arg)) {
		אם (!*arg) {
			*value = عدم;
			החזר 1;
		}
		אם (*arg++ == '=') {
			*value = arg;
			החזר 1;
		}
	}
	החזר 0;
}

ساكن فراغ handle_ref_opt(ثابت حرف *pattern, ثابت حرف *prefix)
{
	אם (pattern) {
		بنية refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, عدم, &opts);
	} אחרת {
		بنية refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = strlen(prefix),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, عدم, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

تعداد format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

تعداد default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

ساكن فراغ print_path(ثابت حرف *path, ثابت حرف *prefix,
		       تعداد format_type format, تعداد default_type def)
{
	بنية strbuf sb = STRBUF_INIT;
	تعداد path_format fmt;

	אם (format == FORMAT_DEFAULT) {
		بدل (def) {
		حالة DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			اكسر;
		حالة DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			اكسر;
		حالة DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			اكسر;
		حالة DEFAULT_UNMODIFIED:
		افتراضي:
			fmt = PATH_FORMAT_UNMODIFIED;
			اكسر;
		}
	} אחרת {
		بدل (format) {
		حالة FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			اكسر;
		حالة FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			اكسر;
		افتراضي:
			fmt = PATH_FORMAT_UNMODIFIED;
			اكسر;
		}
	}

	format_path(&sb, path, prefix, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

صحيح cmd_rev_parse(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *prefix,
		  بنية repository *repo UNUSED)
{
	صحيح i, as_is = 0, verify = 0, quiet = 0, revs_count = 0, type = 0;
	ثابت بنية git_hash_algo *output_algo = عدم;
	ثابت بنية git_hash_algo *compat = عدم;
	صحيح did_repo_setup = 0;
	صحيح has_dashdash = 0;
	صحيح output_prefix = 0;
	بنية object_id oid;
	unsigned صحيح flags = 0;
	ثابت حرف *name = عدم;
	بنية strbuf buf = STRBUF_INIT;
	صحيح seen_end_of_options = 0;
	تعداد format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	אם (argc > 1 && !strcmp("--parseopt", argv[1]))
		החזר cmd_parseopt(argc - 1, argv + 1, prefix);

	אם (argc > 1 && !strcmp("--sq-quote", argv[1]))
		החזר cmd_sq_quote(argc - 2, argv + 2);

	אם (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	עבור (i = 1; i < argc; i++) {
		אם (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			اكسر;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	אם (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, عدم);
		החזר 0;
	}

	עבור (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		אם (as_is) {
			אם (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			استمر;
		}

		אם (!seen_end_of_options) {
			אם (!strcmp(arg, "--local-env-vars")) {
				صحيح i;
				עבור (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				استمر;
			}
			אם (!strcmp(arg, "--resolve-git-dir")) {
				ثابت حرف *gitdir = argv[++i];
				אם (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				אם (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				استمر;
			}
		}

		/* The rest of the options require a git repository. */
		אם (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, عدم);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		אם (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			אם (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			استمر;
		}

		אם (!seen_end_of_options && *arg == '-') {
			אם (!strcmp(arg, "--git-path")) {
				אם (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				استمر;
			}
			אם (!strcmp(arg,"-n")) {
				אם (++i >= argc)
					die(_("-n requires an argument"));
				אם ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				استمر;
			}
			אם (starts_with(arg, "-n")) {
				אם ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				استمر;
			}
			אם (opt_with_value(arg, "--path-format", &arg)) {
				אם (!arg)
					die(_("--path-format requires an argument"));
				אם (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} אחרת אם (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} אחרת {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				استمر;
			}
			אם (!strcmp(arg, "--default")) {
				def = argv[++i];
				אם (!def)
					die(_("--default requires an argument"));
				استمر;
			}
			אם (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				אם (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				استمر;
			}
			אם (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				استمر;
			}
			אם (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				استمر;
			}
			אם (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				استمر;
			}
			אם (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				استمر;
			}
			אם (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				استمر;
			}
			אם (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				استمر;
			}
			אם (opt_with_value(arg, "--output-object-format", &arg)) {
				אם (!arg)
					die(_("no object format specified"));
				אם (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					استمر;
				}
				אחרת אם (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					استمر;
				}
				אחרת die(_("unsupported object format: %s"), arg);
			}
			אם (opt_with_value(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				אם (!arg)
					استمر;
				abbrev = strtoul(arg, عدم, 10);
				אם (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				אחרת אם ((صحيح)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				استمر;
			}
			אם (!strcmp(arg, "--sq")) {
				output_sq = 1;
				استمر;
			}
			אם (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				استمر;
			}
			אם (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				استمر;
			}
			אם (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				استمر;
			}
			אם (opt_with_value(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				אם (arg) {
					אם (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					אחרת אם (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					אחרת
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				استمر;
			}
			אם (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, عدم);
				clear_ref_exclusions(&ref_excludes);
				استمر;
			}
			אם (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, عدم);
				استمر;
			}
			אם (!strcmp(arg, "--bisect")) {
				حرف *prefix;
				حرف *term_bad = عدم;
				حرف *term_good = عدم;
				بنية refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&term_bad, &term_good);
				prefix = xstrfmt("refs/bisect/%s", term_bad);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, عدم, &opts);
				حرر(prefix);
				prefix = xstrfmt("refs/bisect/%s", term_good);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, عدم, &opts);
				حرر(prefix);
				حرر(term_good);
				حرر(term_bad);
				استمر;
			}
			אם (opt_with_value(arg, "--branches", &arg)) {
				אם (ref_excludes.hidden_refs_configured)
					החזר error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				استمر;
			}
			אם (opt_with_value(arg, "--tags", &arg)) {
				אם (ref_excludes.hidden_refs_configured)
					החזר error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				استمر;
			}
			אם (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, عدم);
				استمر;
			}
			אם (opt_with_value(arg, "--remotes", &arg)) {
				אם (ref_excludes.hidden_refs_configured)
					החזר error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				استمر;
			}
			אם (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				استمر;
			}
			אם (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				استمر;
			}
			אם (!strcmp(arg, "--show-toplevel")) {
				ثابت حرف *work_tree = repo_get_work_tree(the_repository);
				אם (work_tree)
					print_path(work_tree, prefix, format, DEFAULT_UNMODIFIED);
				אחרת
					die(_("this operation must be run in a work tree"));
				استمر;
			}
			אם (!strcmp(arg, "--show-superproject-working-tree")) {
				بنية strbuf superproject = STRBUF_INIT;
				אם (get_superproject_working_tree(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				استمر;
			}
			אם (!strcmp(arg, "--show-prefix")) {
				אם (prefix)
					puts(prefix);
				אחרת
					putchar('\n');
				استمر;
			}
			אם (!strcmp(arg, "--show-cdup")) {
				ثابت حرف *pfx = prefix;
				אם (!is_inside_work_tree(the_repository)) {
					ثابت حرف *work_tree =
						repo_get_work_tree(the_repository);
					אם (work_tree)
						printf("%s\n", work_tree);
					استمر;
				}
				כלעוד (pfx) {
					pfx = strchr(pfx, '/');
					אם (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				استمر;
			}
			אם (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				ثابت حرف *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				حرف *cwd;
				صحيح len;
				تعداد format_type wanted = format;
				אם (arg[2] == 'g') {	/* --git-dir */
					אם (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						استمر;
					}
					אם (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						استمر;
					}
				} אחרת {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					אם (!gitdir && !prefix)
						gitdir = ".git";
					אם (gitdir) {
						بنية strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						استمر;
					}
				}
				cwd = xgetcwd();
				len = strlen(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				حرر(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				استمر;
			}
			אם (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				استمر;
			}
			אם (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				استمر;
			}
			אם (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_tree(the_repository) ? "true"
						: "false");
				استمر;
			}
			אם (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				استمر;
			}
			אם (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				استمر;
			}
			אם (!strcmp(arg, "--shared-index-path")) {
				אם (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				אם (the_repository->index->split_index) {
					ثابت بنية object_id *oid = &the_repository->index->split_index->base_oid;
					ثابت حرف *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				استمر;
			}
			אם (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				استمر;
			}
			אם (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				استمر;
			}
			אם (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				استمر;
			}
			אם (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				استمر;
			}
			אם (opt_with_value(arg, "--show-object-format", &arg)) {
				ثابت حرف *val = arg ? arg : "storage";

				אם (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				אם (!strcmp(val, "compat")) {
					אם (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					אחרת
						putchar('\n');
				} אחרת {
					puts(the_hash_algo->name);
				}
				استمر;
			}
			אם (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				استمر;
			}
			אם (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				אם (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				استمر;
			}
			אם (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			استمر;
		}

		/* Not a flag argument */
		אם (try_difference(arg))
			استمر;
		אם (try_parent_shorthands(arg))
			استمر;
		name = arg;
		type = NORMAL;
		אם (*arg == '^') {
			name++;
			type = REVERSED;
		}
		אם (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			אם (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			אם (verify)
				revs_count++;
			אחרת
				show_rev(type, &oid, name);
			استمر;
		}
		אם (verify)
			die_no_single_rev(quiet);
		אם (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		אם (!show_file(arg, output_prefix))
			استمر;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	אם (verify) {
		אם (revs_count == 1) {
			show_rev(type, &oid, name);
			החזר 0;
		} אחרת אם (revs_count == 0 && show_default())
			החזר 0;
		die_no_single_rev(quiet);
	} אחרת
		show_default();
	החזר 0;
}
