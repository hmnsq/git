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
	إذا ((*arg == '-') && isdigit(arg[1]))
		أرجع 1;

	لكل (;;) {
		ثابت حرف *str = *p++;
		صحيح len;
		إذا (!str)
			أرجع 0;
		len = strlen(str);
		إذا (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			أرجع 1;
	}
}

/* Output argument as a string, either SQ or normal */
ساكن فراغ show(ثابت حرف *arg)
{
	إذا (output_sq) {
		صحيح sq = '\'', ch;

		putchar(sq);
		طالما ((ch = *arg++)) {
			إذا (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	وإلا
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
ساكن فراغ show_with_type(صحيح type, ثابت حرف *arg)
{
	إذا (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
ساكن فراغ show_rev(صحيح type, ثابت بنية object_id *oid, ثابت حرف *name)
{
	إذا (!(filter & DO_REVS))
		أرجع;
	def = عدم;

	إذا ((symbolic || abbrev_ref) && name) {
		إذا (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
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
				إذا (abbrev_ref) {
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
		} وإلا {
			show_with_type(type, name);
		}
	}
	وإلا إذا (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	وإلا
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
ساكن صحيح show_flag(ثابت حرف *arg)
{
	إذا (!(filter & DO_FLAGS))
		أرجع 0;
	إذا (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		أرجع 1;
	}
	أرجع 0;
}

ساكن صحيح show_default(فراغ)
{
	ثابت حرف *s = def;

	إذا (s) {
		بنية object_id oid;

		def = عدم;
		إذا (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن صحيح show_reference(ثابت بنية reference *ref, فراغ *cb_data UNUSED)
{
	إذا (ref_excluded(&ref_excludes, ref->name))
		أرجع 0;
	show_rev(NORMAL, ref->oid, ref->name);
	أرجع 0;
}

ساكن صحيح anti_reference(ثابت بنية reference *ref, فراغ *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	أرجع 0;
}

ساكن صحيح show_abbrev(ثابت بنية object_id *oid, فراغ *cb_data UNUSED)
{
	show_rev(NORMAL, oid, عدم);
	أرجع 0;
}

ساكن فراغ show_datestring(ثابت حرف *flag, ثابت حرف *datestr)
{
	حرف *buffer;

	/* date handling requires both flags and revs */
	إذا ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		أرجع;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	حرر(buffer);
}

ساكن صحيح show_file(ثابت حرف *arg, صحيح output_prefix)
{
	show_default();
	إذا ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		إذا (output_prefix) {
			ثابت حرف *prefix = the_repository->prefix;
			حرف *fname = prefix_filename(prefix, arg);
			show(fname);
			حرر(fname);
		} وإلا
			show(arg);
		أرجع 1;
	}
	أرجع 0;
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

	إذا (!(dotdot = strstr(arg, "..")))
		أرجع 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	إذا (!*end)
		end = head_by_default;
	إذا (dotdot == arg)
		start = head_by_default;

	إذا (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		حرر(to_free);
		أرجع 0;
	}

	إذا (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		إذا (symmetric) {
			بنية commit_list *exclude = عدم;
			بنية commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			إذا (!a || !b) {
				حرر(to_free);
				أرجع 0;
			}
			إذا (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			طالما (exclude) {
				بنية commit *commit = دەرهێنان_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, عدم);
			}
		}
		حرر(to_free);
		أرجع 1;
	}
	حرر(to_free);
	أرجع 0;
}

ساكن صحيح try_parent_shorthands(ثابت حرف *arg)
{
	ثابت حرف *mark;
	بنية object_id oid;
	بنية commit *commit;
	بنية commit_list *parents;
	صحيح parent_ژمار;
	صحيح include_rev = 0;
	صحيح include_parents = 0;
	صحيح exclude_parent = 0;
	حرف *to_free;

	إذا ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		إذا (mark[2])
			أرجع 0;
	} وإلا إذا ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		إذا (mark[2])
			أرجع 0;
	} وإلا إذا ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		إذا (mark[2]) {
			حرف *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			إذا (*end != '\0' || !exclude_parent)
				أرجع 0;
		}
	} وإلا
		أرجع 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	إذا (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		حرر(to_free);
		أرجع 0;
	}

	إذا (exclude_parent &&
	    exclude_parent > commit_list_ژماردن(commit->parents)) {
		حرر(to_free);
		أرجع 0;
	}

	إذا (include_rev)
		show_rev(NORMAL, &oid, arg);
	لكل (parents = commit->parents, parent_ژمار = 1;
	     parents;
	     parents = parents->next, parent_ژمار++) {
		حرف *name = عدم;

		إذا (exclude_parent && parent_ژمار != exclude_parent)
			استمر;

		إذا (symbolic)
			name = xstrfmt("%s^%d", arg, parent_ژمار);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		حرر(name);
	}

	حرر(to_free);
	أرجع 1;
}

ساكن صحيح parseopt_dump(ثابت بنية option *o, ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *parsed = o->بەها;
	إذا (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	وإلا إذا (o->short_name && (o->long_name == عدم || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	وإلا
		strbuf_addf(parsed, " --%s", o->long_name);
	إذا (arg) {
		إذا (!stuck_long)
			strbuf_addch(parsed, ' ');
		وإلا إذا (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	أرجع 0;
}

ساكن ثابت حرف *skipspaces(ثابت حرف *s)
{
	طالما (isspace(*s))
		s++;
	أرجع s;
}

ساكن حرف *findspace(ثابت حرف *s)
{
	لكل (; *s; s++)
		إذا (isspace(*s))
			أرجع (حرف*)s;
	أرجع عدم;
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
	إذا (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	لكل (;;) {
		strbuf_reset(&sb);
		إذا (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		إذا (!strcmp("--", sb.buf)) {
			إذا (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			اكسر;
		}

		strvec_پاڵنان(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	طالما (strbuf_getline(&sb, stdin) != EOF) {
		ثابت حرف *s;
		حرف *help;
		بنية option *o;

		إذا (!sb.len)
			استمر;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, حجم(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		إذا (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			استمر;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->بەها = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		إذا (!s)
			s = help;

		إذا (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		إذا (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} وإلا إذا (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (صحيح)(s - sb.buf), sb.buf);
		} وإلا {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (صحيح)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		طالما (s < help) {
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

		إذا (s < help)
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
	لكل (size_t i = 0; i < opts_nr; i++) {
		حرر((حرف *) opts[i].help);
		حرر((حرف *) opts[i].argh);
	}
	حرر(opts);
	أرجع 0;
}

ساكن صحيح cmd_sq_quote(صحيح argc, ثابت حرف **argv)
{
	بنية strbuf buf = STRBUF_INIT;

	إذا (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	أرجع 0;
}

ساكن فراغ die_no_single_rev(صحيح quiet)
{
	إذا (quiet)
		exit(1);
	وإلا
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
ساكن صحيح opt_with_بەها(ثابت حرف *arg, ثابت حرف *opt, ثابت حرف **بەها)
{
	إذا (skip_prefix(arg, opt, &arg)) {
		إذا (!*arg) {
			*بەها = عدم;
			أرجع 1;
		}
		إذا (*arg++ == '=') {
			*بەها = arg;
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن فراغ handle_ref_opt(ثابت حرف *pattern, ثابت حرف *prefix)
{
	إذا (pattern) {
		بنية refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, عدم, &opts);
	} وإلا {
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

	إذا (format == FORMAT_DEFAULT) {
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
	} وإلا {
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
	صحيح i, as_is = 0, verify = 0, quiet = 0, revs_ژماردن = 0, type = 0;
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

	إذا (argc > 1 && !strcmp("--parseopt", argv[1]))
		أرجع cmd_parseopt(argc - 1, argv + 1, prefix);

	إذا (argc > 1 && !strcmp("--sq-quote", argv[1]))
		أرجع cmd_sq_quote(argc - 2, argv + 2);

	إذا (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	لكل (i = 1; i < argc; i++) {
		إذا (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			اكسر;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	إذا (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, عدم);
		أرجع 0;
	}

	لكل (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		إذا (as_is) {
			إذا (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			استمر;
		}

		إذا (!seen_end_of_options) {
			إذا (!strcmp(arg, "--local-env-vars")) {
				صحيح i;
				لكل (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				استمر;
			}
			إذا (!strcmp(arg, "--resolve-git-dir")) {
				ثابت حرف *gitdir = argv[++i];
				إذا (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				إذا (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				استمر;
			}
		}

		/* The rest of the options require a git repository. */
		إذا (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, عدم);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		إذا (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			إذا (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			استمر;
		}

		إذا (!seen_end_of_options && *arg == '-') {
			إذا (!strcmp(arg, "--git-path")) {
				إذا (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				استمر;
			}
			إذا (!strcmp(arg,"-n")) {
				إذا (++i >= argc)
					die(_("-n requires an argument"));
				إذا ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				استمر;
			}
			إذا (starts_with(arg, "-n")) {
				إذا ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				استمر;
			}
			إذا (opt_with_بەها(arg, "--path-format", &arg)) {
				إذا (!arg)
					die(_("--path-format requires an argument"));
				إذا (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} وإلا إذا (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} وإلا {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				استمر;
			}
			إذا (!strcmp(arg, "--default")) {
				def = argv[++i];
				إذا (!def)
					die(_("--default requires an argument"));
				استمر;
			}
			إذا (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				إذا (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				استمر;
			}
			إذا (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				استمر;
			}
			إذا (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				استمر;
			}
			إذا (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				استمر;
			}
			إذا (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				استمر;
			}
			إذا (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				استمر;
			}
			إذا (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				استمر;
			}
			إذا (opt_with_بەها(arg, "--output-object-format", &arg)) {
				إذا (!arg)
					die(_("no object format specified"));
				إذا (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					استمر;
				}
				وإلا إذا (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					استمر;
				}
				وإلا die(_("unsupported object format: %s"), arg);
			}
			إذا (opt_with_بەها(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				إذا (!arg)
					استمر;
				abbrev = strtoul(arg, عدم, 10);
				إذا (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				وإلا إذا ((صحيح)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				استمر;
			}
			إذا (!strcmp(arg, "--sq")) {
				output_sq = 1;
				استمر;
			}
			إذا (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				استمر;
			}
			إذا (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				استمر;
			}
			إذا (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				استمر;
			}
			إذا (opt_with_بەها(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				إذا (arg) {
					إذا (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					وإلا إذا (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					وإلا
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				استمر;
			}
			إذا (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, عدم);
				clear_ref_exclusions(&ref_excludes);
				استمر;
			}
			إذا (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, عدم);
				استمر;
			}
			إذا (!strcmp(arg, "--bisect")) {
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
			إذا (opt_with_بەها(arg, "--branches", &arg)) {
				إذا (ref_excludes.hidden_refs_configured)
					أرجع error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				استمر;
			}
			إذا (opt_with_بەها(arg, "--tags", &arg)) {
				إذا (ref_excludes.hidden_refs_configured)
					أرجع error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				استمر;
			}
			إذا (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, عدم);
				استمر;
			}
			إذا (opt_with_بەها(arg, "--remotes", &arg)) {
				إذا (ref_excludes.hidden_refs_configured)
					أرجع error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				استمر;
			}
			إذا (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				استمر;
			}
			إذا (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				استمر;
			}
			إذا (!strcmp(arg, "--show-toplevel")) {
				ثابت حرف *work_درەخت = repo_get_work_درەخت(the_repository);
				إذا (work_درەخت)
					print_path(work_درەخت, prefix, format, DEFAULT_UNMODIFIED);
				وإلا
					die(_("this operation must be run in a work tree"));
				استمر;
			}
			إذا (!strcmp(arg, "--show-superproject-working-tree")) {
				بنية strbuf superproject = STRBUF_INIT;
				إذا (get_superproject_working_درەخت(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				استمر;
			}
			إذا (!strcmp(arg, "--show-prefix")) {
				إذا (prefix)
					puts(prefix);
				وإلا
					putchar('\n');
				استمر;
			}
			إذا (!strcmp(arg, "--show-cdup")) {
				ثابت حرف *pfx = prefix;
				إذا (!is_inside_work_درەخت(the_repository)) {
					ثابت حرف *work_درەخت =
						repo_get_work_درەخت(the_repository);
					إذا (work_درەخت)
						printf("%s\n", work_درەخت);
					استمر;
				}
				طالما (pfx) {
					pfx = strchr(pfx, '/');
					إذا (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				استمر;
			}
			إذا (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				ثابت حرف *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				حرف *cwd;
				صحيح len;
				تعداد format_type wanted = format;
				إذا (arg[2] == 'g') {	/* --git-dir */
					إذا (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						استمر;
					}
					إذا (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						استمر;
					}
				} وإلا {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					إذا (!gitdir && !prefix)
						gitdir = ".git";
					إذا (gitdir) {
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
			إذا (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				استمر;
			}
			إذا (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				استمر;
			}
			إذا (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_درەخت(the_repository) ? "true"
						: "false");
				استمر;
			}
			إذا (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				استمر;
			}
			إذا (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				استمر;
			}
			إذا (!strcmp(arg, "--shared-index-path")) {
				إذا (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				إذا (the_repository->index->split_index) {
					ثابت بنية object_id *oid = &the_repository->index->split_index->base_oid;
					ثابت حرف *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				استمر;
			}
			إذا (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				استمر;
			}
			إذا (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				استمر;
			}
			إذا (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				استمر;
			}
			إذا (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				استمر;
			}
			إذا (opt_with_بەها(arg, "--show-object-format", &arg)) {
				ثابت حرف *val = arg ? arg : "storage";

				إذا (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				إذا (!strcmp(val, "compat")) {
					إذا (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					وإلا
						putchar('\n');
				} وإلا {
					puts(the_hash_algo->name);
				}
				استمر;
			}
			إذا (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				استمر;
			}
			إذا (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				إذا (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				استمر;
			}
			إذا (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			استمر;
		}

		/* Not a flag argument */
		إذا (try_difference(arg))
			استمر;
		إذا (try_parent_shorthands(arg))
			استمر;
		name = arg;
		type = NORMAL;
		إذا (*arg == '^') {
			name++;
			type = REVERSED;
		}
		إذا (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			إذا (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			إذا (verify)
				revs_ژماردن++;
			وإلا
				show_rev(type, &oid, name);
			استمر;
		}
		إذا (verify)
			die_no_single_rev(quiet);
		إذا (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		إذا (!show_file(arg, output_prefix))
			استمر;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	إذا (verify) {
		إذا (revs_ژماردن == 1) {
			show_rev(type, &oid, name);
			أرجع 0;
		} وإلا إذا (revs_ژماردن == 0 && show_default())
			أرجع 0;
		die_no_single_rev(quiet);
	} وإلا
		show_default();
	أرجع 0;
}
