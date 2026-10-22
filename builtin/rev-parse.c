/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"

#include "abspath.h"
#include "bisect.h"
#include "config.h"
#include "commit.h"
#include "environment.h"
#include "gettext.h"
#include "hash.h"
#include "hex.h"
#include "refs.h"
#include "quote.h"
#include "object-name.h"
#include "parse-options.h"
#include "path.h"
#include "diff.h"
#include "read-cache-ll.h"
#include "repo-settings.h"
#include "repository.h"
#include "revision.h"
#include "setup.h"
#include "split-index.h"
#include "submodule.h"
#include "commit-reach.h"
#include "shallow.h"
#include "object-file-convert.h"

#define DO_REVS		1
#define DO_NOREV	2
#define DO_FLAGS	4
#define DO_NONFLAGS	8
static පූර්ණ_සංඛ්‍යා filter = ~0;

static const var_na *def;

#define NORMAL 0
#define REVERSED 1
static පූර්ණ_සංඛ්‍යා show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
static පූර්ණ_සංඛ්‍යා symbolic;
static පූර්ණ_සංඛ්‍යා abbrev;
static පූර්ණ_සංඛ්‍යා abbrev_ref;
static පූර්ණ_සංඛ්‍යා abbrev_ref_strict;
static පූර්ණ_සංඛ්‍යා output_sq;

static පූර්ණ_සංඛ්‍යා stuck_long;
static sa_nracanaa ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
static පූර්ණ_සංඛ්‍යා is_rev_argument(const var_na *arg)
{
	static const var_na *rev_args[] = {
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
		NULL
	};
	const var_na **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	නම් ((*arg == '-') && isdigit(arg[1]))
		ආපසු 1;

	සඳහා (;;) {
		const var_na *str = *p++;
		පූර්ණ_සංඛ්‍යා len;
		නම් (!str)
			ආපසු 0;
		len = strlen(str);
		නම් (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			ආපසු 1;
	}
}

/* Output argument as a string, either SQ or normal */
static void show(const var_na *arg)
{
	නම් (output_sq) {
		පූර්ණ_සංඛ්‍යා sq = '\'', ch;

		putchar(sq);
		අතරතුර ((ch = *arg++)) {
			නම් (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	නැතහොත්
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
static void show_with_type(පූර්ණ_සංඛ්‍යා type, const var_na *arg)
{
	නම් (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
static void show_rev(පූර්ණ_සංඛ්‍යා type, const sa_nracanaa object_id *oid, const var_na *name)
{
	නම් (!(filter & DO_REVS))
		ආපසු;
	def = NULL;

	නම් ((symbolic || abbrev_ref) && name) {
		නම් (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			sa_nracanaa object_id discard;
			var_na *full;

			switch (repo_dwim_ref(the_repository, name,
					      strlen(name), &discard, &full,
					      0)) {
			pak_sa 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				bha_nga;
			pak_sa 1: /* happy */
				නම් (abbrev_ref) {
					var_na *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					free(old);
				}
				show_with_type(type, full);
				bha_nga;
			default: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				bha_nga;
			}
			free(full);
		} නැතහොත් {
			show_with_type(type, name);
		}
	}
	නැතහොත් නම් (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	නැතහොත්
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
static පූර්ණ_සංඛ්‍යා show_flag(const var_na *arg)
{
	නම් (!(filter & DO_FLAGS))
		ආපසු 0;
	නම් (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		ආපසු 1;
	}
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා show_default(void)
{
	const var_na *s = def;

	නම් (s) {
		sa_nracanaa object_id oid;

		def = NULL;
		නම් (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			ආපසු 1;
		}
	}
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා show_reference(const sa_nracanaa reference *ref, void *cb_data UNUSED)
{
	නම් (ref_excluded(&ref_excludes, ref->name))
		ආපසු 0;
	show_rev(NORMAL, ref->oid, ref->name);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා anti_reference(const sa_nracanaa reference *ref, void *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා show_abbrev(const sa_nracanaa object_id *oid, void *cb_data UNUSED)
{
	show_rev(NORMAL, oid, NULL);
	ආපසු 0;
}

static void show_datestring(const var_na *flag, const var_na *datestr)
{
	var_na *buffer;

	/* date handling requires both flags and revs */
	නම් ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		ආපසු;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	free(buffer);
}

static පූර්ණ_සංඛ්‍යා show_file(const var_na *arg, පූර්ණ_සංඛ්‍යා output_prefix)
{
	show_default();
	නම් ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		නම් (output_prefix) {
			const var_na *prefix = the_repository->prefix;
			var_na *fname = prefix_filename(prefix, arg);
			show(fname);
			free(fname);
		} නැතහොත්
			show(arg);
		ආපසු 1;
	}
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා try_difference(const var_na *arg)
{
	const var_na *dotdot;
	sa_nracanaa object_id start_oid;
	sa_nracanaa object_id end_oid;
	const var_na *end;
	const var_na *start;
	var_na *to_free;
	පූර්ණ_සංඛ්‍යා symmetric;
	static const var_na head_by_default[] = "HEAD";

	නම් (!(dotdot = strstr(arg, "..")))
		ආපසු 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	නම් (!*end)
		end = head_by_default;
	නම් (dotdot == arg)
		start = head_by_default;

	නම් (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		free(to_free);
		ආපසු 0;
	}

	නම් (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		නම් (symmetric) {
			sa_nracanaa commit_list *exclude = NULL;
			sa_nracanaa commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			නම් (!a || !b) {
				free(to_free);
				ආපසු 0;
			}
			නම් (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			අතරතුර (exclude) {
				sa_nracanaa commit *commit = pop_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, NULL);
			}
		}
		free(to_free);
		ආපසු 1;
	}
	free(to_free);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා try_parent_shorthands(const var_na *arg)
{
	const var_na *mark;
	sa_nracanaa object_id oid;
	sa_nracanaa commit *commit;
	sa_nracanaa commit_list *parents;
	පූර්ණ_සංඛ්‍යා parent_number;
	පූර්ණ_සංඛ්‍යා include_rev = 0;
	පූර්ණ_සංඛ්‍යා include_parents = 0;
	පූර්ණ_සංඛ්‍යා exclude_parent = 0;
	var_na *to_free;

	නම් ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		නම් (mark[2])
			ආපසු 0;
	} නැතහොත් නම් ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		නම් (mark[2])
			ආපසු 0;
	} නැතහොත් නම් ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		නම් (mark[2]) {
			var_na *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			නම් (*end != '\0' || !exclude_parent)
				ආපසු 0;
		}
	} නැතහොත්
		ආපසු 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	නම් (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		free(to_free);
		ආපසු 0;
	}

	නම් (exclude_parent &&
	    exclude_parent > commit_list_count(commit->parents)) {
		free(to_free);
		ආපසු 0;
	}

	නම් (include_rev)
		show_rev(NORMAL, &oid, arg);
	සඳහා (parents = commit->parents, parent_number = 1;
	     parents;
	     parents = parents->next, parent_number++) {
		var_na *name = NULL;

		නම් (exclude_parent && parent_number != exclude_parent)
			continue;

		නම් (symbolic)
			name = xstrfmt("%s^%d", arg, parent_number);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		free(name);
	}

	free(to_free);
	ආපසු 1;
}

static පූර්ණ_සංඛ්‍යා parseopt_dump(const sa_nracanaa option *o, const var_na *arg, පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa strbuf *parsed = o->value;
	නම් (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	නැතහොත් නම් (o->short_name && (o->long_name == NULL || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	නැතහොත්
		strbuf_addf(parsed, " --%s", o->long_name);
	නම් (arg) {
		නම් (!stuck_long)
			strbuf_addch(parsed, ' ');
		නැතහොත් නම් (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	ආපසු 0;
}

static const var_na *skipspaces(const var_na *s)
{
	අතරතුර (isspace(*s))
		s++;
	ආපසු s;
}

static var_na *findspace(const var_na *s)
{
	සඳහා (; *s; s++)
		නම් (isspace(*s))
			ආපසු (var_na*)s;
	ආපසු NULL;
}

static පූර්ණ_සංඛ්‍යා cmd_parseopt(පූර්ණ_සංඛ්‍යා argc, const var_na **argv, const var_na *prefix)
{
	පූර්ණ_සංඛ්‍යා keep_dashdash = 0, stop_at_non_option = 0;
	var_na const * const parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		NULL
	};
	sa_nracanaa option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_END(),
	};
	sa_nracanaa strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	sa_nracanaa strvec longnames = STRVEC_INIT;
	sa_nracanaa strvec usage = STRVEC_INIT;
	sa_nracanaa option *opts = NULL;
	size_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	නම් (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	සඳහා (;;) {
		strbuf_reset(&sb);
		නම් (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		නම් (!strcmp("--", sb.buf)) {
			නම් (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			bha_nga;
		}

		strvec_push(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	අතරතුර (strbuf_getline(&sb, stdin) != EOF) {
		const var_na *s;
		var_na *help;
		sa_nracanaa option *o;

		නම් (!sb.len)
			continue;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, parimaa_na(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		නම් (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			continue;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->value = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		නම් (!s)
			s = help;

		නම් (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		නම් (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} නැතහොත් නම් (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (පූර්ණ_සංඛ්‍යා)(s - sb.buf), sb.buf);
		} නැතහොත් {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (පූර්ණ_සංඛ්‍යා)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		අතරතුර (s < help) {
			switch (*s++) {
			pak_sa '=':
				o->flags &= ~PARSE_OPT_NOARG;
				continue;
			pak_sa '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				continue;
			pak_sa '!':
				o->flags |= PARSE_OPT_NONEG;
				continue;
			pak_sa '*':
				o->flags |= PARSE_OPT_HIDDEN;
				continue;
			}
			s--;
			bha_nga;
		}

		නම් (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, parimaa_na(*opts));
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
	සඳහා (size_t i = 0; i < opts_nr; i++) {
		free((var_na *) opts[i].help);
		free((var_na *) opts[i].argh);
	}
	free(opts);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා cmd_sq_quote(පූර්ණ_සංඛ්‍යා argc, const var_na **argv)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;

	නම් (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	ආපසු 0;
}

static void die_no_single_rev(පූර්ණ_සංඛ්‍යා quiet)
{
	නම් (quiet)
		exit(1);
	නැතහොත්
		die(_("Needed a single revision"));
}

static const var_na builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
static පූර්ණ_සංඛ්‍යා opt_with_value(const var_na *arg, const var_na *opt, const var_na **value)
{
	නම් (skip_prefix(arg, opt, &arg)) {
		නම් (!*arg) {
			*value = NULL;
			ආපසු 1;
		}
		නම් (*arg++ == '=') {
			*value = arg;
			ආපසු 1;
		}
	}
	ආපසු 0;
}

static void handle_ref_opt(const var_na *pattern, const var_na *prefix)
{
	නම් (pattern) {
		sa_nracanaa refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, NULL, &opts);
	} නැතහොත් {
		sa_nracanaa refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = strlen(prefix),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, NULL, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

ga_nanaa format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

ga_nanaa default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

static void print_path(const var_na *path, const var_na *prefix,
		       ga_nanaa format_type format, ga_nanaa default_type def)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	ga_nanaa path_format fmt;

	නම් (format == FORMAT_DEFAULT) {
		switch (def) {
		pak_sa DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			bha_nga;
		pak_sa DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			bha_nga;
		pak_sa DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			bha_nga;
		pak_sa DEFAULT_UNMODIFIED:
		default:
			fmt = PATH_FORMAT_UNMODIFIED;
			bha_nga;
		}
	} නැතහොත් {
		switch (format) {
		pak_sa FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			bha_nga;
		pak_sa FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			bha_nga;
		default:
			fmt = PATH_FORMAT_UNMODIFIED;
			bha_nga;
		}
	}

	format_path(&sb, path, prefix, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

පූර්ණ_සංඛ්‍යා cmd_rev_parse(පූර්ණ_සංඛ්‍යා argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	පූර්ණ_සංඛ්‍යා i, as_is = 0, verify = 0, quiet = 0, revs_count = 0, type = 0;
	const sa_nracanaa git_hash_algo *output_algo = NULL;
	const sa_nracanaa git_hash_algo *compat = NULL;
	පූර්ණ_සංඛ්‍යා did_repo_setup = 0;
	පූර්ණ_සංඛ්‍යා has_dashdash = 0;
	පූර්ණ_සංඛ්‍යා output_prefix = 0;
	sa_nracanaa object_id oid;
	unsigned පූර්ණ_සංඛ්‍යා flags = 0;
	const var_na *name = NULL;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	පූර්ණ_සංඛ්‍යා seen_end_of_options = 0;
	ga_nanaa format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	නම් (argc > 1 && !strcmp("--parseopt", argv[1]))
		ආපසු cmd_parseopt(argc - 1, argv + 1, prefix);

	නම් (argc > 1 && !strcmp("--sq-quote", argv[1]))
		ආපසු cmd_sq_quote(argc - 2, argv + 2);

	නම් (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	සඳහා (i = 1; i < argc; i++) {
		නම් (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			bha_nga;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	නම් (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, NULL);
		ආපසු 0;
	}

	සඳහා (i = 1; i < argc; i++) {
		const var_na *arg = argv[i];

		නම් (as_is) {
			නම් (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			continue;
		}

		නම් (!seen_end_of_options) {
			නම් (!strcmp(arg, "--local-env-vars")) {
				පූර්ණ_සංඛ්‍යා i;
				සඳහා (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				continue;
			}
			නම් (!strcmp(arg, "--resolve-git-dir")) {
				const var_na *gitdir = argv[++i];
				නම් (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				නම් (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				continue;
			}
		}

		/* The rest of the options require a git repository. */
		නම් (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, NULL);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		නම් (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			නම් (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			continue;
		}

		නම් (!seen_end_of_options && *arg == '-') {
			නම් (!strcmp(arg, "--git-path")) {
				නම් (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				continue;
			}
			නම් (!strcmp(arg,"-n")) {
				නම් (++i >= argc)
					die(_("-n requires an argument"));
				නම් ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				continue;
			}
			නම් (starts_with(arg, "-n")) {
				නම් ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				continue;
			}
			නම් (opt_with_value(arg, "--path-format", &arg)) {
				නම් (!arg)
					die(_("--path-format requires an argument"));
				නම් (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} නැතහොත් නම් (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} නැතහොත් {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				continue;
			}
			නම් (!strcmp(arg, "--default")) {
				def = argv[++i];
				නම් (!def)
					die(_("--default requires an argument"));
				continue;
			}
			නම් (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				නම් (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				continue;
			}
			නම් (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				continue;
			}
			නම් (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				continue;
			}
			නම් (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				continue;
			}
			නම් (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				continue;
			}
			නම් (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				continue;
			}
			නම් (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				continue;
			}
			නම් (opt_with_value(arg, "--output-object-format", &arg)) {
				නම් (!arg)
					die(_("no object format specified"));
				නම් (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					continue;
				}
				නැතහොත් නම් (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					continue;
				}
				නැතහොත් die(_("unsupported object format: %s"), arg);
			}
			නම් (opt_with_value(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				නම් (!arg)
					continue;
				abbrev = strtoul(arg, NULL, 10);
				නම් (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				නැතහොත් නම් ((පූර්ණ_සංඛ්‍යා)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				continue;
			}
			නම් (!strcmp(arg, "--sq")) {
				output_sq = 1;
				continue;
			}
			නම් (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				continue;
			}
			නම් (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				continue;
			}
			නම් (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				continue;
			}
			නම් (opt_with_value(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				නම් (arg) {
					නම් (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					නැතහොත් නම් (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					නැතහොත්
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				continue;
			}
			නම් (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, NULL);
				clear_ref_exclusions(&ref_excludes);
				continue;
			}
			නම් (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, NULL);
				continue;
			}
			නම් (!strcmp(arg, "--bisect")) {
				var_na *prefix;
				var_na *term_bad = NULL;
				var_na *term_good = NULL;
				sa_nracanaa refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&term_bad, &term_good);
				prefix = xstrfmt("refs/bisect/%s", term_bad);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, NULL, &opts);
				free(prefix);
				prefix = xstrfmt("refs/bisect/%s", term_good);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, NULL, &opts);
				free(prefix);
				free(term_good);
				free(term_bad);
				continue;
			}
			නම් (opt_with_value(arg, "--branches", &arg)) {
				නම් (ref_excludes.hidden_refs_configured)
					ආපසු error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				continue;
			}
			නම් (opt_with_value(arg, "--tags", &arg)) {
				නම් (ref_excludes.hidden_refs_configured)
					ආපසු error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				continue;
			}
			නම් (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, NULL);
				continue;
			}
			නම් (opt_with_value(arg, "--remotes", &arg)) {
				නම් (ref_excludes.hidden_refs_configured)
					ආපසු error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				continue;
			}
			නම් (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				continue;
			}
			නම් (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				continue;
			}
			නම් (!strcmp(arg, "--show-toplevel")) {
				const var_na *work_tree = repo_get_work_tree(the_repository);
				නම් (work_tree)
					print_path(work_tree, prefix, format, DEFAULT_UNMODIFIED);
				නැතහොත්
					die(_("this operation must be run in a work tree"));
				continue;
			}
			නම් (!strcmp(arg, "--show-superproject-working-tree")) {
				sa_nracanaa strbuf superproject = STRBUF_INIT;
				නම් (get_superproject_working_tree(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				continue;
			}
			නම් (!strcmp(arg, "--show-prefix")) {
				නම් (prefix)
					puts(prefix);
				නැතහොත්
					putchar('\n');
				continue;
			}
			නම් (!strcmp(arg, "--show-cdup")) {
				const var_na *pfx = prefix;
				නම් (!is_inside_work_tree(the_repository)) {
					const var_na *work_tree =
						repo_get_work_tree(the_repository);
					නම් (work_tree)
						printf("%s\n", work_tree);
					continue;
				}
				අතරතුර (pfx) {
					pfx = strchr(pfx, '/');
					නම් (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				continue;
			}
			නම් (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				const var_na *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				var_na *cwd;
				පූර්ණ_සංඛ්‍යා len;
				ga_nanaa format_type wanted = format;
				නම් (arg[2] == 'g') {	/* --git-dir */
					නම් (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						continue;
					}
					නම් (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						continue;
					}
				} නැතහොත් {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					නම් (!gitdir && !prefix)
						gitdir = ".git";
					නම් (gitdir) {
						sa_nracanaa strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						continue;
					}
				}
				cwd = xgetcwd();
				len = strlen(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				free(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				continue;
			}
			නම් (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				continue;
			}
			නම් (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				continue;
			}
			නම් (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_tree(the_repository) ? "true"
						: "false");
				continue;
			}
			නම් (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				continue;
			}
			නම් (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				continue;
			}
			නම් (!strcmp(arg, "--shared-index-path")) {
				නම් (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				නම් (the_repository->index->split_index) {
					const sa_nracanaa object_id *oid = &the_repository->index->split_index->base_oid;
					const var_na *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				continue;
			}
			නම් (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				continue;
			}
			නම් (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				continue;
			}
			නම් (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				continue;
			}
			නම් (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				continue;
			}
			නම් (opt_with_value(arg, "--show-object-format", &arg)) {
				const var_na *val = arg ? arg : "storage";

				නම් (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				නම් (!strcmp(val, "compat")) {
					නම් (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					නැතහොත්
						putchar('\n');
				} නැතහොත් {
					puts(the_hash_algo->name);
				}
				continue;
			}
			නම් (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				continue;
			}
			නම් (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				නම් (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				continue;
			}
			නම් (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			continue;
		}

		/* Not a flag argument */
		නම් (try_difference(arg))
			continue;
		නම් (try_parent_shorthands(arg))
			continue;
		name = arg;
		type = NORMAL;
		නම් (*arg == '^') {
			name++;
			type = REVERSED;
		}
		නම් (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			නම් (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			නම් (verify)
				revs_count++;
			නැතහොත්
				show_rev(type, &oid, name);
			continue;
		}
		නම් (verify)
			die_no_single_rev(quiet);
		නම් (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		නම් (!show_file(arg, output_prefix))
			continue;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	නම් (verify) {
		නම් (revs_count == 1) {
			show_rev(type, &oid, name);
			ආපසු 0;
		} නැතහොත් නම් (revs_count == 0 && show_default())
			ආපසු 0;
		die_no_single_rev(quiet);
	} නැතහොත්
		show_default();
	ආපසු 0;
}
