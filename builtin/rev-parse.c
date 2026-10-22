/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

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

#تعریف_کړه DO_REVS		1
#تعریف_کړه DO_NOREV	2
#تعریف_کړه DO_FLAGS	4
#تعریف_کړه DO_NONFLAGS	8
ساكن صحيح filter = ~0;

ساكن ثابت حرف *def;

#تعریف_کړه نورمال 0
#تعریف_کړه REVERSED 1
ساكن صحيح show_type = نورمال;

#تعریف_کړه SHOW_SYMBOLIC_ASIS 1
#تعریف_کړه SHOW_SYMBOLIC_ډک 2
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
	ساكن ثابت حرف *rev_دلیلونه[] = {
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
	ثابت حرف **p = rev_دلیلونه;

	/* accept -<digit>, like traditional "head" */
	إذا ((*arg == '-') && که_عدد(arg[1]))
		أرجع 1;

	لكل (;;) {
		ثابت حرف *str = *p++;
		صحيح len;
		إذا (!str)
			أرجع 0;
		len = مزي_اوږدوالی(str);
		إذا (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !مزي_پرتله(arg, str, len)))
			أرجع 1;
	}
}

/* Output argument as a string, either SQ or normal */
ساكن فراغ show(ثابت حرف *arg)
{
	إذا (output_sq) {
		صحيح sq = '\'', ch;

		توری_چاپ_کړه(sq);
		طالما ((ch = *arg++)) {
			إذا (ch == sq)
				fputs("'\\'", stdout);
			توری_چاپ_کړه(ch);
		}
		توری_چاپ_کړه(sq);
		توری_چاپ_کړه(' ');
	}
	وإلا
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
ساكن فراغ show_with_type(صحيح type, ثابت حرف *arg)
{
	إذا (type != show_type)
		توری_چاپ_کړه('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
ساكن فراغ show_rev(صحيح type, ثابت بنية object_id *oid, ثابت حرف *name)
{
	إذا (!(filter & DO_REVS))
		أرجع;
	def = عدم;

	إذا ((symbolic || abbrev_ref) && name) {
		إذا (symbolic == SHOW_SYMBOLIC_ډک || abbrev_ref) {
			بنية object_id discard;
			حرف *ډک;

			بدل (repo_dwim_ref(the_repository, name,
					      مزي_اوږدوالی(name), &discard, &ډک,
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
					حرف *old = ډک;
					ډک = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    ډک,
									    abbrev_ref_strict);
					حرر(old);
				}
				show_with_type(type, ډک);
				اكسر;
			افتراضي: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				اكسر;
			}
			حرر(ډک);
		} وإلا {
			show_with_type(type, name);
		}
	}
	وإلا إذا (abbrev)
		show_with_type(type,
			       repo_پیدا_کړه_unique_abbrev(the_repository, oid, abbrev));
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
			show_rev(نورمال, &oid, s);
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن صحيح show_reference(ثابت بنية reference *ref, فراغ *cb_معلومات ناکارول_شوی)
{
	إذا (ref_excluded(&ref_excludes, ref->name))
		أرجع 0;
	show_rev(نورمال, ref->oid, ref->name);
	أرجع 0;
}

ساكن صحيح anti_reference(ثابت بنية reference *ref, فراغ *cb_معلومات ناکارول_شوی)
{
	show_rev(REVERSED, ref->oid, ref->name);
	أرجع 0;
}

ساكن صحيح show_abbrev(ثابت بنية object_id *oid, فراغ *cb_معلومات ناکارول_شوی)
{
	show_rev(نورمال, oid, عدم);
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

ساكن صحيح show_file(ثابت حرف *arg, صحيح output_مختاړی)
{
	show_default();
	إذا ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		إذا (output_مختاړی) {
			ثابت حرف *مختاړی = the_repository->مختاړی;
			حرف *fname = مختاړی_filename(مختاړی, arg);
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
	بنية object_id پیل_oid;
	بنية object_id پای_ټکی_oid;
	ثابت حرف *پای_ټکی;
	ثابت حرف *پیل;
	حرف *منزل_free;
	صحيح متناظر;
	ساكن ثابت حرف سر_by_default[] = "HEAD";

	إذا (!(dotdot = مزي_پلټنه(arg, "..")))
		أرجع 0;
	پیل = منزل_free = xmemdupz(arg, dotdot - arg);
	پای_ټکی = dotdot + 2;
	متناظر = (*پای_ټکی == '.');
	پای_ټکی += متناظر;

	إذا (!*پای_ټکی)
		پای_ټکی = سر_by_default;
	إذا (dotdot == arg)
		پیل = سر_by_default;

	إذا (پیل == سر_by_default && پای_ټکی == سر_by_default &&
	    !متناظر) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		حرر(منزل_free);
		أرجع 0;
	}

	إذا (!repo_get_oid_committish(the_repository, پیل, &پیل_oid) && !repo_get_oid_committish(the_repository, پای_ټکی, &پای_ټکی_oid)) {
		show_rev(نورمال, &پای_ټکی_oid, پای_ټکی);
		show_rev(متناظر ? نورمال : REVERSED, &پیل_oid, پیل);
		إذا (متناظر) {
			بنية commit_list *exclude = عدم;
			بنية commit *a, *b;
			a = lookup_commit_reference(the_repository, &پیل_oid);
			b = lookup_commit_reference(the_repository, &پای_ټکی_oid);
			إذا (!a || !b) {
				حرر(منزل_free);
				أرجع 0;
			}
			إذا (repo_get_یوځای_bases(the_repository, a, b, &exclude) < 0)
				وتل(128);
			طالما (exclude) {
				بنية commit *commit = راوباسه_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, عدم);
			}
		}
		حرر(منزل_free);
		أرجع 1;
	}
	حرر(منزل_free);
	أرجع 0;
}

ساكن صحيح try_پلار_shorthands(ثابت حرف *arg)
{
	ثابت حرف *mark;
	بنية object_id oid;
	بنية commit *commit;
	بنية commit_list *parents;
	صحيح پلار_شمېره;
	صحيح include_rev = 0;
	صحيح include_parents = 0;
	صحيح exclude_پلار = 0;
	حرف *منزل_free;

	إذا ((mark = مزي_پلټنه(arg, "^!"))) {
		include_rev = 1;
		إذا (mark[2])
			أرجع 0;
	} وإلا إذا ((mark = مزي_پلټنه(arg, "^@"))) {
		include_parents = 1;
		إذا (mark[2])
			أرجع 0;
	} وإلا إذا ((mark = مزي_پلټنه(arg, "^-"))) {
		include_rev = 1;
		exclude_پلار = 1;

		إذا (mark[2]) {
			حرف *پای_ټکی;
			exclude_پلار = strtoul(mark + 2, &پای_ټکی, 10);
			إذا (*پای_ټکی != '\0' || !exclude_پلار)
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

	إذا (exclude_پلار &&
	    exclude_پلار > commit_list_شمېر(commit->parents)) {
		حرر(منزل_free);
		أرجع 0;
	}

	إذا (include_rev)
		show_rev(نورمال, &oid, arg);
	لكل (parents = commit->parents, پلار_شمېره = 1;
	     parents;
	     parents = parents->بل, پلار_شمېره++) {
		حرف *name = عدم;

		إذا (exclude_پلار && پلار_شمېره != exclude_پلار)
			استمر;

		إذا (symbolic)
			name = xstrfmt("%s^%d", arg, پلار_شمېره);
		show_rev(include_parents ? نورمال : REVERSED,
			 &parents->توکی->object.oid, name);
		حرر(name);
	}

	حرر(منزل_free);
	أرجع 1;
}

ساكن صحيح parseopt_dump(ثابت بنية option *o, ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *parsed = o->ارزښت;
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
	طالما (که_تشه(*s))
		s++;
	أرجع s;
}

ساكن حرف *findspace(ثابت حرف *s)
{
	لكل (; *s; s++)
		إذا (که_تشه(*s))
			أرجع (حرف*)s;
	أرجع عدم;
}

ساكن صحيح cmd_parseopt(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی)
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
		OPT_پای_ټکی(),
	};
	بنية strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	بنية strvec longnames = STRVEC_INIT;
	بنية strvec usage = STRVEC_INIT;
	بنية option *opts = عدم;
	اندازه_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, مختاړی, parseopt_opts, parseopt_usage,
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

		strvec_ټېل(&usage, sb.buf);
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
			o->type = OPTION_ډله;
			o->help = xstrdup(skipspaces(sb.buf));
			استمر;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->ارزښت = &parsed;
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
	argc = parse_options(argc, argv, مختاړی, opts, usage.v,
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
	لكل (اندازه_t i = 0; i < opts_nr; i++) {
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
	چاپ_کړه("%s\n", buf.buf);
	strbuf_release(&buf);

	أرجع 0;
}

ساكن فراغ die_no_single_rev(صحيح quiet)
{
	إذا (quiet)
		وتل(1);
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
ساكن صحيح opt_with_ارزښت(ثابت حرف *arg, ثابت حرف *opt, ثابت حرف **ارزښت)
{
	إذا (skip_مختاړی(arg, opt, &arg)) {
		إذا (!*arg) {
			*ارزښت = عدم;
			أرجع 1;
		}
		إذا (*arg++ == '=') {
			*ارزښت = arg;
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن فراغ handle_ref_opt(ثابت حرف *نمونه, ثابت حرف *مختاړی)
{
	إذا (نمونه) {
		بنية refs_for_each_ref_options opts = {
			.نمونه = نمونه,
			.مختاړی = مختاړی,
			.trim_مختاړی = مختاړی ? مزي_اوږدوالی(مختاړی) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, عدم, &opts);
	} وإلا {
		بنية refs_for_each_ref_options opts = {
			.مختاړی = مختاړی,
			.trim_مختاړی = مزي_اوږدوالی(مختاړی),
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

ساكن فراغ print_لار(ثابت حرف *لار, ثابت حرف *مختاړی,
		       تعداد format_type format, تعداد default_type def)
{
	بنية strbuf sb = STRBUF_INIT;
	تعداد لار_format fmt;

	إذا (format == FORMAT_DEFAULT) {
		بدل (def) {
		حالة DEFAULT_RELATIVE:
			fmt = لار_FORMAT_RELATIVE;
			اكسر;
		حالة DEFAULT_RELATIVE_IF_SHARED:
			fmt = لار_FORMAT_RELATIVE_IF_SHARED;
			اكسر;
		حالة DEFAULT_CANONICAL:
			fmt = لار_FORMAT_CANONICAL;
			اكسر;
		حالة DEFAULT_UNMODIFIED:
		افتراضي:
			fmt = لار_FORMAT_UNMODIFIED;
			اكسر;
		}
	} وإلا {
		بدل (format) {
		حالة FORMAT_RELATIVE:
			fmt = لار_FORMAT_RELATIVE;
			اكسر;
		حالة FORMAT_CANONICAL:
			fmt = لار_FORMAT_CANONICAL;
			اكسر;
		افتراضي:
			fmt = لار_FORMAT_UNMODIFIED;
			اكسر;
		}
	}

	format_لار(&sb, لار, مختاړی, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

صحيح cmd_rev_parse(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *مختاړی,
		  بنية repository *repo ناکارول_شوی)
{
	صحيح i, as_is = 0, verify = 0, quiet = 0, revs_شمېر = 0, type = 0;
	ثابت بنية git_hash_algo *output_algo = عدم;
	ثابت بنية git_hash_algo *compat = عدم;
	صحيح did_repo_تیاری = 0;
	صحيح has_dashdash = 0;
	صحيح output_مختاړی = 0;
	بنية object_id oid;
	بینښه صحيح flags = 0;
	ثابت حرف *name = عدم;
	بنية strbuf buf = STRBUF_INIT;
	صحيح کتل_شوی_پای_ټکی_د_options = 0;
	تعداد format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	إذا (argc > 1 && !strcmp("--parseopt", argv[1]))
		أرجع cmd_parseopt(argc - 1, argv + 1, مختاړی);

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
			إذا (show_file(arg, output_مختاړی) && as_is < 2)
				verify_filename(the_repository, مختاړی, arg, 0);
			استمر;
		}

		إذا (!کتل_شوی_پای_ټکی_د_options) {
			إذا (!strcmp(arg, "--local-env-vars")) {
				صحيح i;
				لكل (i = 0; local_repo_env[i]; i++)
					چاپ_کړه("%s\n", local_repo_env[i]);
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
			مختاړی = تیاری_git_directory(the_repository);
			repo_config(the_repository, git_default_config, عدم);
			did_repo_تیاری = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_ډک_شاخص = 0;
			compat = the_repository->compat_hash_algo;
		}

		إذا (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			إذا (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			استمر;
		}

		إذا (!کتل_شوی_پای_ټکی_د_options && *arg == '-') {
			إذا (!strcmp(arg, "--git-path")) {
				إذا (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_لار(repo_git_لار_replace(the_repository, &buf,
								 "%s", argv[i + 1]), مختاړی,
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
			إذا (opt_with_ارزښت(arg, "--path-format", &arg)) {
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
				مختاړی = argv[++i];
				إذا (!مختاړی)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->مختاړی);
				the_repository->مختاړی = xstrdup(مختاړی);
				output_مختاړی = 1;
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
			إذا (opt_with_ارزښت(arg, "--output-object-format", &arg)) {
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
			إذا (opt_with_ارزښت(arg, "--short", &arg)) {
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
				symbolic = SHOW_SYMBOLIC_ډک;
				استمر;
			}
			إذا (opt_with_ارزښت(arg, "--abbrev-ref", &arg)) {
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
			إذا (skip_مختاړی(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, عدم);
				استمر;
			}
			إذا (!strcmp(arg, "--bisect")) {
				حرف *مختاړی;
				حرف *غړی_bad = عدم;
				حرف *غړی_good = عدم;
				بنية refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&غړی_bad, &غړی_good);
				مختاړی = xstrfmt("refs/bisect/%s", غړی_bad);
				opts.مختاړی = مختاړی;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, عدم, &opts);
				حرر(مختاړی);
				مختاړی = xstrfmt("refs/bisect/%s", غړی_good);
				opts.مختاړی = مختاړی;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, عدم, &opts);
				حرر(مختاړی);
				حرر(غړی_good);
				حرر(غړی_bad);
				استمر;
			}
			إذا (opt_with_ارزښت(arg, "--branches", &arg)) {
				إذا (ref_excludes.hidden_refs_configured)
					أرجع error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				استمر;
			}
			إذا (opt_with_ارزښت(arg, "--tags", &arg)) {
				إذا (ref_excludes.hidden_refs_configured)
					أرجع error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				استمر;
			}
			إذا (skip_مختاړی(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, عدم);
				استمر;
			}
			إذا (opt_with_ارزښت(arg, "--remotes", &arg)) {
				إذا (ref_excludes.hidden_refs_configured)
					أرجع error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				استمر;
			}
			إذا (skip_مختاړی(arg, "--exclude=", &arg)) {
				جمع_کړه_ref_exclusion(&ref_excludes, arg);
				استمر;
			}
			إذا (skip_مختاړی(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				استمر;
			}
			إذا (!strcmp(arg, "--show-toplevel")) {
				ثابت حرف *work_ونه = repo_get_work_ونه(the_repository);
				إذا (work_ونه)
					print_لار(work_ونه, مختاړی, format, DEFAULT_UNMODIFIED);
				وإلا
					die(_("this operation must be run in a work tree"));
				استمر;
			}
			إذا (!strcmp(arg, "--show-superproject-working-tree")) {
				بنية strbuf superproject = STRBUF_INIT;
				إذا (get_superproject_working_ونه(&superproject))
					print_لار(superproject.buf, مختاړی, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				استمر;
			}
			إذا (!strcmp(arg, "--show-prefix")) {
				إذا (مختاړی)
					puts(مختاړی);
				وإلا
					توری_چاپ_کړه('\n');
				استمر;
			}
			إذا (!strcmp(arg, "--show-cdup")) {
				ثابت حرف *pfx = مختاړی;
				إذا (!is_inside_work_ونه(the_repository)) {
					ثابت حرف *work_ونه =
						repo_get_work_ونه(the_repository);
					إذا (work_ونه)
						چاپ_کړه("%s\n", work_ونه);
					استمر;
				}
				طالما (pfx) {
					pfx = strchr(pfx, '/');
					إذا (pfx) {
						pfx++;
						چاپ_کړه("../");
					}
				}
				توری_چاپ_کړه('\n');
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
						print_لار(gitdir, مختاړی, format, DEFAULT_UNMODIFIED);
						استمر;
					}
					إذا (!مختاړی) {
						print_لار(".git", مختاړی, format, DEFAULT_UNMODIFIED);
						استمر;
					}
				} وإلا {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					إذا (!gitdir && !مختاړی)
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
				len = مزي_اوږدوالی(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				حرر(cwd);
				print_لار(buf.buf, مختاړی, wanted, DEFAULT_CANONICAL);
				استمر;
			}
			إذا (!strcmp(arg, "--git-common-dir")) {
				print_لار(repo_get_common_dir(the_repository), مختاړی, format, DEFAULT_RELATIVE_IF_SHARED);
				استمر;
			}
			إذا (!strcmp(arg, "--is-inside-git-dir")) {
				چاپ_کړه("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				استمر;
			}
			إذا (!strcmp(arg, "--is-inside-work-tree")) {
				چاپ_کړه("%s\n", is_inside_work_ونه(the_repository) ? "true"
						: "false");
				استمر;
			}
			إذا (!strcmp(arg, "--is-bare-repository")) {
				چاپ_کړه("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				استمر;
			}
			إذا (!strcmp(arg, "--is-shallow-repository")) {
				چاپ_کړه("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				استمر;
			}
			إذا (!strcmp(arg, "--shared-index-path")) {
				إذا (repo_read_شاخص(the_repository) < 0)
					die(_("Could not read the index"));
				إذا (the_repository->شاخص->وېشل_شاخص) {
					ثابت بنية object_id *oid = &the_repository->شاخص->وېشل_شاخص->بنسټ_oid;
					ثابت حرف *لار = repo_git_لار_replace(the_repository, &buf, "sharedindex.%s", oid_منزل_hex(oid));
					print_لار(لار, مختاړی, format, DEFAULT_RELATIVE);
				}
				استمر;
			}
			إذا (skip_مختاړی(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				استمر;
			}
			إذا (skip_مختاړی(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				استمر;
			}
			إذا (skip_مختاړی(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				استمر;
			}
			إذا (skip_مختاړی(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				استمر;
			}
			إذا (opt_with_ارزښت(arg, "--show-object-format", &arg)) {
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
						توری_چاپ_کړه('\n');
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
				کتل_شوی_پای_ټکی_د_options = 1;
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
		إذا (try_پلار_shorthands(arg))
			استمر;
		name = arg;
		type = نورمال;
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
				revs_شمېر++;
			وإلا
				show_rev(type, &oid, name);
			استمر;
		}
		إذا (verify)
			die_no_single_rev(quiet);
		إذا (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		إذا (!show_file(arg, output_مختاړی))
			استمر;
		verify_filename(the_repository, مختاړی, arg, 1);
	}
	strbuf_release(&buf);
	إذا (verify) {
		إذا (revs_شمېر == 1) {
			show_rev(type, &oid, name);
			أرجع 0;
		} وإلا إذا (revs_شمېر == 0 && show_default())
			أرجع 0;
		die_no_single_rev(quiet);
	} وإلا
		show_default();
	أرجع 0;
}
