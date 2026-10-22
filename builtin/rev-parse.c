/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

#تعریف_کرو DO_REVS		1
#تعریف_کرو DO_NOREV	2
#تعریف_کرو DO_FLAGS	4
#تعریف_کرو DO_NONFLAGS	8
ساكن صحيح filter = ~0;

ساكن ثابت حرف *def;

#تعریف_کرو عام_2 0
#تعریف_کرو REVERSED 1
ساكن صحيح show_type = عام_2;

#تعریف_کرو SHOW_SYMBOLIC_ASIS 1
#تعریف_کرو SHOW_SYMBOLIC_بھریا 2
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
ساكن صحيح is_rev_دلیل(ثابت حرف *arg)
{
	ساكن ثابت حرف *rev_دلیلاں[] = {
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
	ثابت حرف **p = rev_دلیلاں;

	/* accept -<digit>, like traditional "head" */
	إذا ((*arg == '-') && ہندسہ_اے(arg[1]))
		أرجع 1;

	لكل (;;) {
		ثابت حرف *str = *p++;
		صحيح len;
		إذا (!str)
			أرجع 0;
		len = لڑی_لمبائی(str);
		إذا (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !لڑی_مقابلہ(arg, str, len)))
			أرجع 1;
	}
}

/* Output argument as a string, either SQ or normal */
ساكن فراغ show(ثابت حرف *arg)
{
	إذا (output_sq) {
		صحيح sq = '\'', ch;

		حرف_لکھو(sq);
		طالما ((ch = *arg++)) {
			إذا (ch == sq)
				fputs("'\\'", stdout);
			حرف_لکھو(ch);
		}
		حرف_لکھو(sq);
		حرف_لکھو(' ');
	}
	وإلا
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
ساكن فراغ show_with_type(صحيح type, ثابت حرف *arg)
{
	إذا (type != show_type)
		حرف_لکھو('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
ساكن فراغ show_rev(صحيح type, ثابت بنية object_id *oid, ثابت حرف *name)
{
	إذا (!(filter & DO_REVS))
		أرجع;
	def = عدم;

	إذا ((symbolic || abbrev_ref) && name) {
		إذا (symbolic == SHOW_SYMBOLIC_بھریا || abbrev_ref) {
			بنية object_id discard;
			حرف *بھریا;

			بدل (repo_dwim_ref(the_repository, name,
					      لڑی_لمبائی(name), &discard, &بھریا,
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
					حرف *old = بھریا;
					بھریا = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    بھریا,
									    abbrev_ref_strict);
					حرر(old);
				}
				show_with_type(type, بھریا);
				اكسر;
			افتراضي: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				اكسر;
			}
			حرر(بھریا);
		} وإلا {
			show_with_type(type, name);
		}
	}
	وإلا إذا (abbrev)
		show_with_type(type,
			       repo_لبھو_unique_abbrev(the_repository, oid, abbrev));
	وإلا
		show_with_type(type, oid_منزل_hex(oid));
}

/* Output a flag, only if filter allows it. */
ساكن صحيح show_flag(ثابت حرف *arg)
{
	إذا (!(filter & DO_FLAGS))
		أرجع 0;
	إذا (filter & (is_rev_دلیل(arg) ? DO_REVS : DO_NOREV)) {
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
			show_rev(عام_2, &oid, s);
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن صحيح show_reference(ثابت بنية reference *ref, فراغ *cb_انکڑے بن_ورتیا)
{
	إذا (ref_excluded(&ref_excludes, ref->name))
		أرجع 0;
	show_rev(عام_2, ref->oid, ref->name);
	أرجع 0;
}

ساكن صحيح anti_reference(ثابت بنية reference *ref, فراغ *cb_انکڑے بن_ورتیا)
{
	show_rev(REVERSED, ref->oid, ref->name);
	أرجع 0;
}

ساكن صحيح show_abbrev(ثابت بنية object_id *oid, فراغ *cb_انکڑے بن_ورتیا)
{
	show_rev(عام_2, oid, عدم);
	أرجع 0;
}

ساكن فراغ show_datestring(ثابت حرف *flag, ثابت حرف *datestr)
{
	حرف *بفر;

	/* date handling requires both flags and revs */
	إذا ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		أرجع;
	بفر = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(بفر);
	حرر(بفر);
}

ساكن صحيح show_file(ثابت حرف *arg, صحيح output_اگیتر)
{
	show_default();
	إذا ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		إذا (output_اگیتر) {
			ثابت حرف *اگیتر = the_repository->اگیتر;
			حرف *fname = اگیتر_filename(اگیتر, arg);
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
	بنية object_id شروع_oid;
	بنية object_id سرا_oid;
	ثابت حرف *سرا;
	ثابت حرف *شروع;
	حرف *منزل_free;
	صحيح متشاکل;
	ساكن ثابت حرف سر_by_default[] = "HEAD";

	إذا (!(dotdot = لڑی_کھوج(arg, "..")))
		أرجع 0;
	شروع = منزل_free = xmemdupz(arg, dotdot - arg);
	سرا = dotdot + 2;
	متشاکل = (*سرا == '.');
	سرا += متشاکل;

	إذا (!*سرا)
		سرا = سر_by_default;
	إذا (dotdot == arg)
		شروع = سر_by_default;

	إذا (شروع == سر_by_default && سرا == سر_by_default &&
	    !متشاکل) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		حرر(منزل_free);
		أرجع 0;
	}

	إذا (!repo_get_oid_committish(the_repository, شروع, &شروع_oid) && !repo_get_oid_committish(the_repository, سرا, &سرا_oid)) {
		show_rev(عام_2, &سرا_oid, سرا);
		show_rev(متشاکل ? عام_2 : REVERSED, &شروع_oid, شروع);
		إذا (متشاکل) {
			بنية commit_list *exclude = عدم;
			بنية commit *a, *b;
			a = lookup_commit_reference(the_repository, &شروع_oid);
			b = lookup_commit_reference(the_repository, &سرا_oid);
			إذا (!a || !b) {
				حرر(منزل_free);
				أرجع 0;
			}
			إذا (repo_get_ملاؤ_bases(the_repository, a, b, &exclude) < 0)
				باہر_نکلو(128);
			طالما (exclude) {
				بنية commit *commit = نکالو_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, عدم);
			}
		}
		حرر(منزل_free);
		أرجع 1;
	}
	حرر(منزل_free);
	أرجع 0;
}

ساكن صحيح try_ماپا_shorthands(ثابت حرف *arg)
{
	ثابت حرف *mark;
	بنية object_id oid;
	بنية commit *commit;
	بنية commit_list *parents;
	صحيح ماپا_نمبر;
	صحيح include_rev = 0;
	صحيح include_parents = 0;
	صحيح exclude_ماپا = 0;
	حرف *منزل_free;

	إذا ((mark = لڑی_کھوج(arg, "^!"))) {
		include_rev = 1;
		إذا (mark[2])
			أرجع 0;
	} وإلا إذا ((mark = لڑی_کھوج(arg, "^@"))) {
		include_parents = 1;
		إذا (mark[2])
			أرجع 0;
	} وإلا إذا ((mark = لڑی_کھوج(arg, "^-"))) {
		include_rev = 1;
		exclude_ماپا = 1;

		إذا (mark[2]) {
			حرف *سرا;
			exclude_ماپا = strtoul(mark + 2, &سرا, 10);
			إذا (*سرا != '\0' || !exclude_ماپا)
				أرجع 0;
		}
	} وإلا
		أرجع 0;

	arg = منزل_free = xmemdupz(arg, mark - arg);
	إذا (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		حرر(منزل_free);
		أرجع 0;
	}

	إذا (exclude_ماپا &&
	    exclude_ماپا > commit_list_گنتی(commit->parents)) {
		حرر(منزل_free);
		أرجع 0;
	}

	إذا (include_rev)
		show_rev(عام_2, &oid, arg);
	لكل (parents = commit->parents, ماپا_نمبر = 1;
	     parents;
	     parents = parents->اگلا, ماپا_نمبر++) {
		حرف *name = عدم;

		إذا (exclude_ماپا && ماپا_نمبر != exclude_ماپا)
			استمر;

		إذا (symbolic)
			name = xstrfmt("%s^%d", arg, ماپا_نمبر);
		show_rev(include_parents ? عام_2 : REVERSED,
			 &parents->شے->object.oid, name);
		حرر(name);
	}

	حرر(منزل_free);
	أرجع 1;
}

ساكن صحيح parseopt_dump(ثابت بنية option *o, ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *parsed = o->قدر;
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
	طالما (خلا_اے(*s))
		s++;
	أرجع s;
}

ساكن حرف *findspace(ثابت حرف *s)
{
	لكل (; *s; s++)
		إذا (خلا_اے(*s))
			أرجع (حرف*)s;
	أرجع عدم;
}

ساكن صحيح cmd_parseopt(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر)
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
		OPT_سرا(),
	};
	بنية strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	بنية strvec longnames = STRVEC_INIT;
	بنية strvec usage = STRVEC_INIT;
	بنية option *opts = عدم;
	ناپ_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, اگیتر, parseopt_opts, parseopt_usage,
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

		strvec_ڈالو(&usage, sb.buf);
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
			o->type = OPTION_گروہ;
			o->help = xstrdup(skipspaces(sb.buf));
			استمر;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->قدر = &parsed;
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
	argc = parse_options(argc, argv, اگیتر, opts, usage.v,
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
	لكل (ناپ_t i = 0; i < opts_nr; i++) {
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
	لکھو("%s\n", buf.buf);
	strbuf_release(&buf);

	أرجع 0;
}

ساكن فراغ die_no_single_rev(صحيح quiet)
{
	إذا (quiet)
		باہر_نکلو(1);
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
ساكن صحيح opt_with_قدر(ثابت حرف *arg, ثابت حرف *opt, ثابت حرف **قدر)
{
	إذا (skip_اگیتر(arg, opt, &arg)) {
		إذا (!*arg) {
			*قدر = عدم;
			أرجع 1;
		}
		إذا (*arg++ == '=') {
			*قدر = arg;
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن فراغ handle_ref_opt(ثابت حرف *نمونہ, ثابت حرف *اگیتر)
{
	إذا (نمونہ) {
		بنية refs_for_each_ref_options opts = {
			.نمونہ = نمونہ,
			.اگیتر = اگیتر,
			.trim_اگیتر = اگیتر ? لڑی_لمبائی(اگیتر) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, عدم, &opts);
	} وإلا {
		بنية refs_for_each_ref_options opts = {
			.اگیتر = اگیتر,
			.trim_اگیتر = لڑی_لمبائی(اگیتر),
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

ساكن فراغ print_رستہ(ثابت حرف *رستہ, ثابت حرف *اگیتر,
		       تعداد format_type format, تعداد default_type def)
{
	بنية strbuf sb = STRBUF_INIT;
	تعداد رستہ_format fmt;

	إذا (format == FORMAT_DEFAULT) {
		بدل (def) {
		حالة DEFAULT_RELATIVE:
			fmt = رستہ_FORMAT_RELATIVE;
			اكسر;
		حالة DEFAULT_RELATIVE_IF_SHARED:
			fmt = رستہ_FORMAT_RELATIVE_IF_SHARED;
			اكسر;
		حالة DEFAULT_CANONICAL:
			fmt = رستہ_FORMAT_CANONICAL;
			اكسر;
		حالة DEFAULT_UNMODIFIED:
		افتراضي:
			fmt = رستہ_FORMAT_UNMODIFIED;
			اكسر;
		}
	} وإلا {
		بدل (format) {
		حالة FORMAT_RELATIVE:
			fmt = رستہ_FORMAT_RELATIVE;
			اكسر;
		حالة FORMAT_CANONICAL:
			fmt = رستہ_FORMAT_CANONICAL;
			اكسر;
		افتراضي:
			fmt = رستہ_FORMAT_UNMODIFIED;
			اكسر;
		}
	}

	format_رستہ(&sb, رستہ, اگیتر, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

صحيح cmd_rev_parse(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *اگیتر,
		  بنية repository *repo بن_ورتیا)
{
	صحيح i, as_is = 0, verify = 0, quiet = 0, revs_گنتی = 0, type = 0;
	ثابت بنية git_hash_algo *output_algo = عدم;
	ثابت بنية git_hash_algo *compat = عدم;
	صحيح did_repo_تیاری = 0;
	صحيح has_dashdash = 0;
	صحيح output_اگیتر = 0;
	بنية object_id oid;
	بنانشانی صحيح flags = 0;
	ثابت حرف *name = عدم;
	بنية strbuf buf = STRBUF_INIT;
	صحيح تکیا_سرا_دا_options = 0;
	تعداد format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	إذا (argc > 1 && !strcmp("--parseopt", argv[1]))
		أرجع cmd_parseopt(argc - 1, argv + 1, اگیتر);

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
		تیاری_git_directory(the_repository);
		repo_config(the_repository, git_default_config, عدم);
		أرجع 0;
	}

	لكل (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		إذا (as_is) {
			إذا (show_file(arg, output_اگیتر) && as_is < 2)
				verify_filename(the_repository, اگیتر, arg, 0);
			استمر;
		}

		إذا (!تکیا_سرا_دا_options) {
			إذا (!strcmp(arg, "--local-env-vars")) {
				صحيح i;
				لكل (i = 0; local_repo_env[i]; i++)
					لکھو("%s\n", local_repo_env[i]);
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
		إذا (!did_repo_تیاری) {
			اگیتر = تیاری_git_directory(the_repository);
			repo_config(the_repository, git_default_config, عدم);
			did_repo_تیاری = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_بھریا_اشاریہ = 0;
			compat = the_repository->compat_hash_algo;
		}

		إذا (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			إذا (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			استمر;
		}

		إذا (!تکیا_سرا_دا_options && *arg == '-') {
			إذا (!strcmp(arg, "--git-path")) {
				إذا (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_رستہ(repo_git_رستہ_replace(the_repository, &buf,
								 "%s", argv[i + 1]), اگیتر,
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
			إذا (opt_with_قدر(arg, "--path-format", &arg)) {
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
				اگیتر = argv[++i];
				إذا (!اگیتر)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->اگیتر);
				the_repository->اگیتر = xstrdup(اگیتر);
				output_اگیتر = 1;
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
			إذا (opt_with_قدر(arg, "--output-object-format", &arg)) {
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
			إذا (opt_with_قدر(arg, "--short", &arg)) {
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
				symbolic = SHOW_SYMBOLIC_بھریا;
				استمر;
			}
			إذا (opt_with_قدر(arg, "--abbrev-ref", &arg)) {
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
			إذا (skip_اگیتر(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, عدم);
				استمر;
			}
			إذا (!strcmp(arg, "--bisect")) {
				حرف *اگیتر;
				حرف *رکن_bad = عدم;
				حرف *رکن_good = عدم;
				بنية refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&رکن_bad, &رکن_good);
				اگیتر = xstrfmt("refs/bisect/%s", رکن_bad);
				opts.اگیتر = اگیتر;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, عدم, &opts);
				حرر(اگیتر);
				اگیتر = xstrfmt("refs/bisect/%s", رکن_good);
				opts.اگیتر = اگیتر;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, عدم, &opts);
				حرر(اگیتر);
				حرر(رکن_good);
				حرر(رکن_bad);
				استمر;
			}
			إذا (opt_with_قدر(arg, "--branches", &arg)) {
				إذا (ref_excludes.hidden_refs_configured)
					أرجع error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				استمر;
			}
			إذا (opt_with_قدر(arg, "--tags", &arg)) {
				إذا (ref_excludes.hidden_refs_configured)
					أرجع error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				استمر;
			}
			إذا (skip_اگیتر(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, عدم);
				استمر;
			}
			إذا (opt_with_قدر(arg, "--remotes", &arg)) {
				إذا (ref_excludes.hidden_refs_configured)
					أرجع error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				استمر;
			}
			إذا (skip_اگیتر(arg, "--exclude=", &arg)) {
				جوڑو_ref_exclusion(&ref_excludes, arg);
				استمر;
			}
			إذا (skip_اگیتر(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				استمر;
			}
			إذا (!strcmp(arg, "--show-toplevel")) {
				ثابت حرف *work_درخت = repo_get_work_درخت(the_repository);
				إذا (work_درخت)
					print_رستہ(work_درخت, اگیتر, format, DEFAULT_UNMODIFIED);
				وإلا
					die(_("this operation must be run in a work tree"));
				استمر;
			}
			إذا (!strcmp(arg, "--show-superproject-working-tree")) {
				بنية strbuf superproject = STRBUF_INIT;
				إذا (get_superproject_working_درخت(&superproject))
					print_رستہ(superproject.buf, اگیتر, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				استمر;
			}
			إذا (!strcmp(arg, "--show-prefix")) {
				إذا (اگیتر)
					puts(اگیتر);
				وإلا
					حرف_لکھو('\n');
				استمر;
			}
			إذا (!strcmp(arg, "--show-cdup")) {
				ثابت حرف *pfx = اگیتر;
				إذا (!is_inside_work_درخت(the_repository)) {
					ثابت حرف *work_درخت =
						repo_get_work_درخت(the_repository);
					إذا (work_درخت)
						لکھو("%s\n", work_درخت);
					استمر;
				}
				طالما (pfx) {
					pfx = strchr(pfx, '/');
					إذا (pfx) {
						pfx++;
						لکھو("../");
					}
				}
				حرف_لکھو('\n');
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
						print_رستہ(gitdir, اگیتر, format, DEFAULT_UNMODIFIED);
						استمر;
					}
					إذا (!اگیتر) {
						print_رستہ(".git", اگیتر, format, DEFAULT_UNMODIFIED);
						استمر;
					}
				} وإلا {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					إذا (!gitdir && !اگیتر)
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
				len = لڑی_لمبائی(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				حرر(cwd);
				print_رستہ(buf.buf, اگیتر, wanted, DEFAULT_CANONICAL);
				استمر;
			}
			إذا (!strcmp(arg, "--git-common-dir")) {
				print_رستہ(repo_get_common_dir(the_repository), اگیتر, format, DEFAULT_RELATIVE_IF_SHARED);
				استمر;
			}
			إذا (!strcmp(arg, "--is-inside-git-dir")) {
				لکھو("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				استمر;
			}
			إذا (!strcmp(arg, "--is-inside-work-tree")) {
				لکھو("%s\n", is_inside_work_درخت(the_repository) ? "true"
						: "false");
				استمر;
			}
			إذا (!strcmp(arg, "--is-bare-repository")) {
				لکھو("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				استمر;
			}
			إذا (!strcmp(arg, "--is-shallow-repository")) {
				لکھو("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				استمر;
			}
			إذا (!strcmp(arg, "--shared-index-path")) {
				إذا (repo_read_اشاریہ(the_repository) < 0)
					die(_("Could not read the index"));
				إذا (the_repository->اشاریہ->ونڈو_اشاریہ) {
					ثابت بنية object_id *oid = &the_repository->اشاریہ->ونڈو_اشاریہ->ادھار_oid;
					ثابت حرف *رستہ = repo_git_رستہ_replace(the_repository, &buf, "sharedindex.%s", oid_منزل_hex(oid));
					print_رستہ(رستہ, اگیتر, format, DEFAULT_RELATIVE);
				}
				استمر;
			}
			إذا (skip_اگیتر(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				استمر;
			}
			إذا (skip_اگیتر(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				استمر;
			}
			إذا (skip_اگیتر(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				استمر;
			}
			إذا (skip_اگیتر(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				استمر;
			}
			إذا (opt_with_قدر(arg, "--show-object-format", &arg)) {
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
						حرف_لکھو('\n');
				} وإلا {
					puts(the_hash_algo->name);
				}
				استمر;
			}
			إذا (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_منزل_name(the_repository->ref_storage_format));
				استمر;
			}
			إذا (!strcmp(arg, "--end-of-options")) {
				تکیا_سرا_دا_options = 1;
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
		إذا (try_ماپا_shorthands(arg))
			استمر;
		name = arg;
		type = عام_2;
		إذا (*arg == '^') {
			name++;
			type = REVERSED;
		}
		إذا (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			إذا (output_algo)
				repo_oid_منزل_algop(the_repository, &oid,
						  output_algo, &oid);
			إذا (verify)
				revs_گنتی++;
			وإلا
				show_rev(type, &oid, name);
			استمر;
		}
		إذا (verify)
			die_no_single_rev(quiet);
		إذا (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		إذا (!show_file(arg, output_اگیتر))
			استمر;
		verify_filename(the_repository, اگیتر, arg, 1);
	}
	strbuf_release(&buf);
	إذا (verify) {
		إذا (revs_گنتی == 1) {
			show_rev(type, &oid, name);
			أرجع 0;
		} وإلا إذا (revs_گنتی == 0 && show_default())
			أرجع 0;
		die_no_single_rev(quiet);
	} وإلا
		show_default();
	أرجع 0;
}
