/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

#تعریف_کرو DO_REVS		1
#تعریف_کرو DO_NOREV	2
#تعریف_کرو DO_FLAGS	4
#تعریف_کرو DO_NONFLAGS	8
ساکن صحیح filter = ~0;

ساکن ثابت حرف *def;

#تعریف_کرو معمول 0
#تعریف_کرو REVERSED 1
ساکن صحیح show_type = معمول;

#تعریف_کرو SHOW_SYMBOLIC_ASIS 1
#تعریف_کرو SHOW_SYMBOLIC_بھرا 2
ساکن صحیح symbolic;
ساکن صحیح abbrev;
ساکن صحیح abbrev_ref;
ساکن صحیح abbrev_ref_strict;
ساکن صحیح output_sq;

ساکن صحیح stuck_long;
ساکن ساخت ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
ساکن صحیح is_rev_دلیل(ثابت حرف *arg)
{
	ساکن ثابت حرف *rev_دلائل[] = {
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
		تہی
	};
	ثابت حرف **p = rev_دلائل;

	/* accept -<digit>, like traditional "head" */
	اگر ((*arg == '-') && ہندسہ_ہے(arg[1]))
		واپس 1;

	برائے (;;) {
		ثابت حرف *str = *p++;
		صحیح len;
		اگر (!str)
			واپس 0;
		len = لڑی_لمبائی(str);
		اگر (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !لڑی_موازنہ(arg, str, len)))
			واپس 1;
	}
}

/* Output argument as a string, either SQ or normal */
ساکن خالی show(ثابت حرف *arg)
{
	اگر (output_sq) {
		صحیح sq = '\'', ch;

		حرف_لکھو(sq);
		جب ((ch = *arg++)) {
			اگر (ch == sq)
				fputs("'\\'", stdout);
			حرف_لکھو(ch);
		}
		حرف_لکھو(sq);
		حرف_لکھو(' ');
	}
	ورنہ
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
ساکن خالی show_with_type(صحیح type, ثابت حرف *arg)
{
	اگر (type != show_type)
		حرف_لکھو('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
ساکن خالی show_rev(صحیح type, ثابت ساخت object_id *oid, ثابت حرف *name)
{
	اگر (!(filter & DO_REVS))
		واپس;
	def = تہی;

	اگر ((symbolic || abbrev_ref) && name) {
		اگر (symbolic == SHOW_SYMBOLIC_بھرا || abbrev_ref) {
			ساخت object_id discard;
			حرف *بھرا;

			انتخاب (repo_dwim_ref(the_repository, name,
					      لڑی_لمبائی(name), &discard, &بھرا,
					      0)) {
			حالت 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				توڑو;
			حالت 1: /* happy */
				اگر (abbrev_ref) {
					حرف *old = بھرا;
					بھرا = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    بھرا,
									    abbrev_ref_strict);
					آزاد(old);
				}
				show_with_type(type, بھرا);
				توڑو;
			طےشدہ: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				توڑو;
			}
			آزاد(بھرا);
		} ورنہ {
			show_with_type(type, name);
		}
	}
	ورنہ اگر (abbrev)
		show_with_type(type,
			       repo_ڈھونڈو_unique_abbrev(the_repository, oid, abbrev));
	ورنہ
		show_with_type(type, oid_مقصد_hex(oid));
}

/* Output a flag, only if filter allows it. */
ساکن صحیح show_flag(ثابت حرف *arg)
{
	اگر (!(filter & DO_FLAGS))
		واپس 0;
	اگر (filter & (is_rev_دلیل(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		واپس 1;
	}
	واپس 0;
}

ساکن صحیح show_default(خالی)
{
	ثابت حرف *s = def;

	اگر (s) {
		ساخت object_id oid;

		def = تہی;
		اگر (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(معمول, &oid, s);
			واپس 1;
		}
	}
	واپس 0;
}

ساکن صحیح show_reference(ثابت ساخت reference *ref, خالی *cb_ڈیٹا غیر_مستعمل)
{
	اگر (ref_excluded(&ref_excludes, ref->name))
		واپس 0;
	show_rev(معمول, ref->oid, ref->name);
	واپس 0;
}

ساکن صحیح anti_reference(ثابت ساخت reference *ref, خالی *cb_ڈیٹا غیر_مستعمل)
{
	show_rev(REVERSED, ref->oid, ref->name);
	واپس 0;
}

ساکن صحیح show_abbrev(ثابت ساخت object_id *oid, خالی *cb_ڈیٹا غیر_مستعمل)
{
	show_rev(معمول, oid, تہی);
	واپس 0;
}

ساکن خالی show_datestring(ثابت حرف *flag, ثابت حرف *datestr)
{
	حرف *بفر;

	/* date handling requires both flags and revs */
	اگر ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		واپس;
	بفر = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(بفر);
	آزاد(بفر);
}

ساکن صحیح show_file(ثابت حرف *arg, صحیح output_سابقہ)
{
	show_default();
	اگر ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		اگر (output_سابقہ) {
			ثابت حرف *سابقہ = the_repository->سابقہ;
			حرف *fname = سابقہ_filename(سابقہ, arg);
			show(fname);
			آزاد(fname);
		} ورنہ
			show(arg);
		واپس 1;
	}
	واپس 0;
}

ساکن صحیح try_difference(ثابت حرف *arg)
{
	ثابت حرف *dotdot;
	ساخت object_id آغاز_oid;
	ساخت object_id انتہا_oid;
	ثابت حرف *انتہا;
	ثابت حرف *آغاز;
	حرف *مقصد_free;
	صحیح متشاکل;
	ساکن ثابت حرف سر_by_default[] = "HEAD";

	اگر (!(dotdot = لڑی_تلاش(arg, "..")))
		واپس 0;
	آغاز = مقصد_free = xmemdupz(arg, dotdot - arg);
	انتہا = dotdot + 2;
	متشاکل = (*انتہا == '.');
	انتہا += متشاکل;

	اگر (!*انتہا)
		انتہا = سر_by_default;
	اگر (dotdot == arg)
		آغاز = سر_by_default;

	اگر (آغاز == سر_by_default && انتہا == سر_by_default &&
	    !متشاکل) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		آزاد(مقصد_free);
		واپس 0;
	}

	اگر (!repo_get_oid_committish(the_repository, آغاز, &آغاز_oid) && !repo_get_oid_committish(the_repository, انتہا, &انتہا_oid)) {
		show_rev(معمول, &انتہا_oid, انتہا);
		show_rev(متشاکل ? معمول : REVERSED, &آغاز_oid, آغاز);
		اگر (متشاکل) {
			ساخت commit_list *exclude = تہی;
			ساخت commit *a, *b;
			a = lookup_commit_reference(the_repository, &آغاز_oid);
			b = lookup_commit_reference(the_repository, &انتہا_oid);
			اگر (!a || !b) {
				آزاد(مقصد_free);
				واپس 0;
			}
			اگر (repo_get_ملاؤ_bases(the_repository, a, b, &exclude) < 0)
				باہر_نکلو(128);
			جب (exclude) {
				ساخت commit *commit = نکالو_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, تہی);
			}
		}
		آزاد(مقصد_free);
		واپس 1;
	}
	آزاد(مقصد_free);
	واپس 0;
}

ساکن صحیح try_والد_shorthands(ثابت حرف *arg)
{
	ثابت حرف *mark;
	ساخت object_id oid;
	ساخت commit *commit;
	ساخت commit_list *parents;
	صحیح والد_نمبر;
	صحیح include_rev = 0;
	صحیح include_parents = 0;
	صحیح exclude_والد = 0;
	حرف *مقصد_free;

	اگر ((mark = لڑی_تلاش(arg, "^!"))) {
		include_rev = 1;
		اگر (mark[2])
			واپس 0;
	} ورنہ اگر ((mark = لڑی_تلاش(arg, "^@"))) {
		include_parents = 1;
		اگر (mark[2])
			واپس 0;
	} ورنہ اگر ((mark = لڑی_تلاش(arg, "^-"))) {
		include_rev = 1;
		exclude_والد = 1;

		اگر (mark[2]) {
			حرف *انتہا;
			exclude_والد = strtoul(mark + 2, &انتہا, 10);
			اگر (*انتہا != '\0' || !exclude_والد)
				واپس 0;
		}
	} ورنہ
		واپس 0;

	arg = مقصد_free = xmemdupz(arg, mark - arg);
	اگر (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		آزاد(مقصد_free);
		واپس 0;
	}

	اگر (exclude_والد &&
	    exclude_والد > commit_list_گنتی(commit->parents)) {
		آزاد(مقصد_free);
		واپس 0;
	}

	اگر (include_rev)
		show_rev(معمول, &oid, arg);
	برائے (parents = commit->parents, والد_نمبر = 1;
	     parents;
	     parents = parents->اگلا, والد_نمبر++) {
		حرف *name = تہی;

		اگر (exclude_والد && والد_نمبر != exclude_والد)
			جاری;

		اگر (symbolic)
			name = xstrfmt("%s^%d", arg, والد_نمبر);
		show_rev(include_parents ? معمول : REVERSED,
			 &parents->شے->object.oid, name);
		آزاد(name);
	}

	آزاد(مقصد_free);
	واپس 1;
}

ساکن صحیح parseopt_dump(ثابت ساخت option *o, ثابت حرف *arg, صحیح unset)
{
	ساخت strbuf *parsed = o->قدر;
	اگر (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	ورنہ اگر (o->short_name && (o->long_name == تہی || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	ورنہ
		strbuf_addf(parsed, " --%s", o->long_name);
	اگر (arg) {
		اگر (!stuck_long)
			strbuf_addch(parsed, ' ');
		ورنہ اگر (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	واپس 0;
}

ساکن ثابت حرف *skipspaces(ثابت حرف *s)
{
	جب (خلا_ہے(*s))
		s++;
	واپس s;
}

ساکن حرف *findspace(ثابت حرف *s)
{
	برائے (; *s; s++)
		اگر (خلا_ہے(*s))
			واپس (حرف*)s;
	واپس تہی;
}

ساکن صحیح cmd_parseopt(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ)
{
	صحیح keep_dashdash = 0, stop_at_non_option = 0;
	حرف ثابت * ثابت parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		تہی
	};
	ساخت option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_انتہا(),
	};
	ساخت strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	ساخت strvec longnames = STRVEC_INIT;
	ساخت strvec usage = STRVEC_INIT;
	ساخت option *opts = تہی;
	سائز_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, سابقہ, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	اگر (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	برائے (;;) {
		strbuf_reset(&sb);
		اگر (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		اگر (!strcmp("--", sb.buf)) {
			اگر (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			توڑو;
		}

		strvec_ڈالو(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	جب (strbuf_getline(&sb, stdin) != EOF) {
		ثابت حرف *s;
		حرف *help;
		ساخت option *o;

		اگر (!sb.len)
			جاری;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, حجم(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		اگر (!help || sb.buf == help) {
			o->type = OPTION_گروہ;
			o->help = xstrdup(skipspaces(sb.buf));
			جاری;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->قدر = &parsed;
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
		} ورنہ اگر (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (صحیح)(s - sb.buf), sb.buf);
		} ورنہ {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (صحیح)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		جب (s < help) {
			انتخاب (*s++) {
			حالت '=':
				o->flags &= ~PARSE_OPT_NOARG;
				جاری;
			حالت '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				جاری;
			حالت '!':
				o->flags |= PARSE_OPT_NONEG;
				جاری;
			حالت '*':
				o->flags |= PARSE_OPT_HIDDEN;
				جاری;
			}
			s--;
			توڑو;
		}

		اگر (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, حجم(*opts));
	argc = parse_options(argc, argv, سابقہ, opts, usage.v,
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
	برائے (سائز_t i = 0; i < opts_nr; i++) {
		آزاد((حرف *) opts[i].help);
		آزاد((حرف *) opts[i].argh);
	}
	آزاد(opts);
	واپس 0;
}

ساکن صحیح cmd_sq_quote(صحیح argc, ثابت حرف **argv)
{
	ساخت strbuf buf = STRBUF_INIT;

	اگر (argc)
		sq_quote_argv(&buf, argv);
	لکھو("%s\n", buf.buf);
	strbuf_release(&buf);

	واپس 0;
}

ساکن خالی die_no_single_rev(صحیح quiet)
{
	اگر (quiet)
		باہر_نکلو(1);
	ورنہ
		die(_("Needed a single revision"));
}

ساکن ثابت حرف builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
ساکن صحیح opt_with_قدر(ثابت حرف *arg, ثابت حرف *opt, ثابت حرف **قدر)
{
	اگر (skip_سابقہ(arg, opt, &arg)) {
		اگر (!*arg) {
			*قدر = تہی;
			واپس 1;
		}
		اگر (*arg++ == '=') {
			*قدر = arg;
			واپس 1;
		}
	}
	واپس 0;
}

ساکن خالی handle_ref_opt(ثابت حرف *نمونہ, ثابت حرف *سابقہ)
{
	اگر (نمونہ) {
		ساخت refs_for_each_ref_options opts = {
			.نمونہ = نمونہ,
			.سابقہ = سابقہ,
			.trim_سابقہ = سابقہ ? لڑی_لمبائی(سابقہ) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, تہی, &opts);
	} ورنہ {
		ساخت refs_for_each_ref_options opts = {
			.سابقہ = سابقہ,
			.trim_سابقہ = لڑی_لمبائی(سابقہ),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, تہی, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

شمار format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

شمار default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

ساکن خالی print_راستہ(ثابت حرف *راستہ, ثابت حرف *سابقہ,
		       شمار format_type format, شمار default_type def)
{
	ساخت strbuf sb = STRBUF_INIT;
	شمار راستہ_format fmt;

	اگر (format == FORMAT_DEFAULT) {
		انتخاب (def) {
		حالت DEFAULT_RELATIVE:
			fmt = راستہ_FORMAT_RELATIVE;
			توڑو;
		حالت DEFAULT_RELATIVE_IF_SHARED:
			fmt = راستہ_FORMAT_RELATIVE_IF_SHARED;
			توڑو;
		حالت DEFAULT_CANONICAL:
			fmt = راستہ_FORMAT_CANONICAL;
			توڑو;
		حالت DEFAULT_UNMODIFIED:
		طےشدہ:
			fmt = راستہ_FORMAT_UNMODIFIED;
			توڑو;
		}
	} ورنہ {
		انتخاب (format) {
		حالت FORMAT_RELATIVE:
			fmt = راستہ_FORMAT_RELATIVE;
			توڑو;
		حالت FORMAT_CANONICAL:
			fmt = راستہ_FORMAT_CANONICAL;
			توڑو;
		طےشدہ:
			fmt = راستہ_FORMAT_UNMODIFIED;
			توڑو;
		}
	}

	format_راستہ(&sb, راستہ, سابقہ, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

صحیح cmd_rev_parse(صحیح argc,
		  ثابت حرف **argv,
		  ثابت حرف *سابقہ,
		  ساخت repository *repo غیر_مستعمل)
{
	صحیح i, as_is = 0, verify = 0, quiet = 0, revs_گنتی = 0, type = 0;
	ثابت ساخت git_hash_algo *output_algo = تہی;
	ثابت ساخت git_hash_algo *compat = تہی;
	صحیح did_repo_تیاری = 0;
	صحیح has_dashdash = 0;
	صحیح output_سابقہ = 0;
	ساخت object_id oid;
	غیرعلامتی صحیح flags = 0;
	ثابت حرف *name = تہی;
	ساخت strbuf buf = STRBUF_INIT;
	صحیح نظر_آیا_انتہا_کا_options = 0;
	شمار format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	اگر (argc > 1 && !strcmp("--parseopt", argv[1]))
		واپس cmd_parseopt(argc - 1, argv + 1, سابقہ);

	اگر (argc > 1 && !strcmp("--sq-quote", argv[1]))
		واپس cmd_sq_quote(argc - 2, argv + 2);

	اگر (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	برائے (i = 1; i < argc; i++) {
		اگر (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			توڑو;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	اگر (argc == 1) {
		تیاری_git_directory(the_repository);
		repo_config(the_repository, git_default_config, تہی);
		واپس 0;
	}

	برائے (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		اگر (as_is) {
			اگر (show_file(arg, output_سابقہ) && as_is < 2)
				verify_filename(the_repository, سابقہ, arg, 0);
			جاری;
		}

		اگر (!نظر_آیا_انتہا_کا_options) {
			اگر (!strcmp(arg, "--local-env-vars")) {
				صحیح i;
				برائے (i = 0; local_repo_env[i]; i++)
					لکھو("%s\n", local_repo_env[i]);
				جاری;
			}
			اگر (!strcmp(arg, "--resolve-git-dir")) {
				ثابت حرف *gitdir = argv[++i];
				اگر (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				اگر (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				جاری;
			}
		}

		/* The rest of the options require a git repository. */
		اگر (!did_repo_تیاری) {
			سابقہ = تیاری_git_directory(the_repository);
			repo_config(the_repository, git_default_config, تہی);
			did_repo_تیاری = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_بھرا_اشاریہ = 0;
			compat = the_repository->compat_hash_algo;
		}

		اگر (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			اگر (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			جاری;
		}

		اگر (!نظر_آیا_انتہا_کا_options && *arg == '-') {
			اگر (!strcmp(arg, "--git-path")) {
				اگر (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_راستہ(repo_git_راستہ_replace(the_repository, &buf,
								 "%s", argv[i + 1]), سابقہ,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				جاری;
			}
			اگر (!strcmp(arg,"-n")) {
				اگر (++i >= argc)
					die(_("-n requires an argument"));
				اگر ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				جاری;
			}
			اگر (starts_with(arg, "-n")) {
				اگر ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				جاری;
			}
			اگر (opt_with_قدر(arg, "--path-format", &arg)) {
				اگر (!arg)
					die(_("--path-format requires an argument"));
				اگر (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} ورنہ اگر (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} ورنہ {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				جاری;
			}
			اگر (!strcmp(arg, "--default")) {
				def = argv[++i];
				اگر (!def)
					die(_("--default requires an argument"));
				جاری;
			}
			اگر (!strcmp(arg, "--prefix")) {
				سابقہ = argv[++i];
				اگر (!سابقہ)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->سابقہ);
				the_repository->سابقہ = xstrdup(سابقہ);
				output_سابقہ = 1;
				جاری;
			}
			اگر (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				جاری;
			}
			اگر (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				جاری;
			}
			اگر (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				جاری;
			}
			اگر (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				جاری;
			}
			اگر (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				جاری;
			}
			اگر (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				جاری;
			}
			اگر (opt_with_قدر(arg, "--output-object-format", &arg)) {
				اگر (!arg)
					die(_("no object format specified"));
				اگر (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					جاری;
				}
				ورنہ اگر (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					جاری;
				}
				ورنہ die(_("unsupported object format: %s"), arg);
			}
			اگر (opt_with_قدر(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				اگر (!arg)
					جاری;
				abbrev = strtoul(arg, تہی, 10);
				اگر (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				ورنہ اگر ((صحیح)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				جاری;
			}
			اگر (!strcmp(arg, "--sq")) {
				output_sq = 1;
				جاری;
			}
			اگر (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				جاری;
			}
			اگر (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				جاری;
			}
			اگر (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_بھرا;
				جاری;
			}
			اگر (opt_with_قدر(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				اگر (arg) {
					اگر (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					ورنہ اگر (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					ورنہ
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				جاری;
			}
			اگر (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, تہی);
				clear_ref_exclusions(&ref_excludes);
				جاری;
			}
			اگر (skip_سابقہ(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, تہی);
				جاری;
			}
			اگر (!strcmp(arg, "--bisect")) {
				حرف *سابقہ;
				حرف *رکن_bad = تہی;
				حرف *رکن_good = تہی;
				ساخت refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&رکن_bad, &رکن_good);
				سابقہ = xstrfmt("refs/bisect/%s", رکن_bad);
				opts.سابقہ = سابقہ;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, تہی, &opts);
				آزاد(سابقہ);
				سابقہ = xstrfmt("refs/bisect/%s", رکن_good);
				opts.سابقہ = سابقہ;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, تہی, &opts);
				آزاد(سابقہ);
				آزاد(رکن_good);
				آزاد(رکن_bad);
				جاری;
			}
			اگر (opt_with_قدر(arg, "--branches", &arg)) {
				اگر (ref_excludes.hidden_refs_configured)
					واپس error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				جاری;
			}
			اگر (opt_with_قدر(arg, "--tags", &arg)) {
				اگر (ref_excludes.hidden_refs_configured)
					واپس error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				جاری;
			}
			اگر (skip_سابقہ(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, تہی);
				جاری;
			}
			اگر (opt_with_قدر(arg, "--remotes", &arg)) {
				اگر (ref_excludes.hidden_refs_configured)
					واپس error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				جاری;
			}
			اگر (skip_سابقہ(arg, "--exclude=", &arg)) {
				جوڑو_ref_exclusion(&ref_excludes, arg);
				جاری;
			}
			اگر (skip_سابقہ(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				جاری;
			}
			اگر (!strcmp(arg, "--show-toplevel")) {
				ثابت حرف *work_درخت = repo_get_work_درخت(the_repository);
				اگر (work_درخت)
					print_راستہ(work_درخت, سابقہ, format, DEFAULT_UNMODIFIED);
				ورنہ
					die(_("this operation must be run in a work tree"));
				جاری;
			}
			اگر (!strcmp(arg, "--show-superproject-working-tree")) {
				ساخت strbuf superproject = STRBUF_INIT;
				اگر (get_superproject_working_درخت(&superproject))
					print_راستہ(superproject.buf, سابقہ, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				جاری;
			}
			اگر (!strcmp(arg, "--show-prefix")) {
				اگر (سابقہ)
					puts(سابقہ);
				ورنہ
					حرف_لکھو('\n');
				جاری;
			}
			اگر (!strcmp(arg, "--show-cdup")) {
				ثابت حرف *pfx = سابقہ;
				اگر (!is_inside_work_درخت(the_repository)) {
					ثابت حرف *work_درخت =
						repo_get_work_درخت(the_repository);
					اگر (work_درخت)
						لکھو("%s\n", work_درخت);
					جاری;
				}
				جب (pfx) {
					pfx = strchr(pfx, '/');
					اگر (pfx) {
						pfx++;
						لکھو("../");
					}
				}
				حرف_لکھو('\n');
				جاری;
			}
			اگر (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				ثابت حرف *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				حرف *cwd;
				صحیح len;
				شمار format_type wanted = format;
				اگر (arg[2] == 'g') {	/* --git-dir */
					اگر (gitdir) {
						print_راستہ(gitdir, سابقہ, format, DEFAULT_UNMODIFIED);
						جاری;
					}
					اگر (!سابقہ) {
						print_راستہ(".git", سابقہ, format, DEFAULT_UNMODIFIED);
						جاری;
					}
				} ورنہ {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					اگر (!gitdir && !سابقہ)
						gitdir = ".git";
					اگر (gitdir) {
						ساخت strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						جاری;
					}
				}
				cwd = xgetcwd();
				len = لڑی_لمبائی(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				آزاد(cwd);
				print_راستہ(buf.buf, سابقہ, wanted, DEFAULT_CANONICAL);
				جاری;
			}
			اگر (!strcmp(arg, "--git-common-dir")) {
				print_راستہ(repo_get_common_dir(the_repository), سابقہ, format, DEFAULT_RELATIVE_IF_SHARED);
				جاری;
			}
			اگر (!strcmp(arg, "--is-inside-git-dir")) {
				لکھو("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				جاری;
			}
			اگر (!strcmp(arg, "--is-inside-work-tree")) {
				لکھو("%s\n", is_inside_work_درخت(the_repository) ? "true"
						: "false");
				جاری;
			}
			اگر (!strcmp(arg, "--is-bare-repository")) {
				لکھو("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				جاری;
			}
			اگر (!strcmp(arg, "--is-shallow-repository")) {
				لکھو("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				جاری;
			}
			اگر (!strcmp(arg, "--shared-index-path")) {
				اگر (repo_read_اشاریہ(the_repository) < 0)
					die(_("Could not read the index"));
				اگر (the_repository->اشاریہ->بانٹو_اشاریہ) {
					ثابت ساخت object_id *oid = &the_repository->اشاریہ->بانٹو_اشاریہ->بنیاد_oid;
					ثابت حرف *راستہ = repo_git_راستہ_replace(the_repository, &buf, "sharedindex.%s", oid_مقصد_hex(oid));
					print_راستہ(راستہ, سابقہ, format, DEFAULT_RELATIVE);
				}
				جاری;
			}
			اگر (skip_سابقہ(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				جاری;
			}
			اگر (skip_سابقہ(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				جاری;
			}
			اگر (skip_سابقہ(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				جاری;
			}
			اگر (skip_سابقہ(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				جاری;
			}
			اگر (opt_with_قدر(arg, "--show-object-format", &arg)) {
				ثابت حرف *val = arg ? arg : "storage";

				اگر (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				اگر (!strcmp(val, "compat")) {
					اگر (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					ورنہ
						حرف_لکھو('\n');
				} ورنہ {
					puts(the_hash_algo->name);
				}
				جاری;
			}
			اگر (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_مقصد_name(the_repository->ref_storage_format));
				جاری;
			}
			اگر (!strcmp(arg, "--end-of-options")) {
				نظر_آیا_انتہا_کا_options = 1;
				اگر (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				جاری;
			}
			اگر (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			جاری;
		}

		/* Not a flag argument */
		اگر (try_difference(arg))
			جاری;
		اگر (try_والد_shorthands(arg))
			جاری;
		name = arg;
		type = معمول;
		اگر (*arg == '^') {
			name++;
			type = REVERSED;
		}
		اگر (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			اگر (output_algo)
				repo_oid_مقصد_algop(the_repository, &oid,
						  output_algo, &oid);
			اگر (verify)
				revs_گنتی++;
			ورنہ
				show_rev(type, &oid, name);
			جاری;
		}
		اگر (verify)
			die_no_single_rev(quiet);
		اگر (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		اگر (!show_file(arg, output_سابقہ))
			جاری;
		verify_filename(the_repository, سابقہ, arg, 1);
	}
	strbuf_release(&buf);
	اگر (verify) {
		اگر (revs_گنتی == 1) {
			show_rev(type, &oid, name);
			واپس 0;
		} ورنہ اگر (revs_گنتی == 0 && show_default())
			واپس 0;
		die_no_single_rev(quiet);
	} ورنہ
		show_default();
	واپس 0;
}
