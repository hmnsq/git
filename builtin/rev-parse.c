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
static puur_naa_nka filter = ~0;

static const var_na *def;

#define NORMAL 0
#define REVERSED 1
static puur_naa_nka show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
static puur_naa_nka symbolic;
static puur_naa_nka abbrev;
static puur_naa_nka abbrev_ref;
static puur_naa_nka abbrev_ref_strict;
static puur_naa_nka output_sq;

static puur_naa_nka stuck_long;
static sa_nracanaa ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
static puur_naa_nka is_rev_argument(const var_na *arg)
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
	ᱡᱩᱫᱤ ((*arg == '-') && isdigit(arg[1]))
		ᱨᱩᱣᱟᱹᱲ 1;

	for (;;) {
		const var_na *str = *p++;
		puur_naa_nka len;
		ᱡᱩᱫᱤ (!str)
			ᱨᱩᱣᱟᱹᱲ 0;
		len = strlen(str);
		ᱡᱩᱫᱤ (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			ᱨᱩᱣᱟᱹᱲ 1;
	}
}

/* Output argument as a string, either SQ or normal */
static void show(const var_na *arg)
{
	ᱡᱩᱫᱤ (output_sq) {
		puur_naa_nka sq = '\'', ch;

		putchar(sq);
		ᱡᱚᱠᱷᱚᱱ ((ch = *arg++)) {
			ᱡᱩᱫᱤ (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	ᱵᱟᱝ
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
static void show_with_type(puur_naa_nka type, const var_na *arg)
{
	ᱡᱩᱫᱤ (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
static void show_rev(puur_naa_nka type, const sa_nracanaa object_id *oid, const var_na *name)
{
	ᱡᱩᱫᱤ (!(filter & DO_REVS))
		ᱨᱩᱣᱟᱹᱲ;
	def = NULL;

	ᱡᱩᱫᱤ ((symbolic || abbrev_ref) && name) {
		ᱡᱩᱫᱤ (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
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
				ᱡᱩᱫᱤ (abbrev_ref) {
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
		} ᱵᱟᱝ {
			show_with_type(type, name);
		}
	}
	ᱵᱟᱝ ᱡᱩᱫᱤ (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	ᱵᱟᱝ
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
static puur_naa_nka show_flag(const var_na *arg)
{
	ᱡᱩᱫᱤ (!(filter & DO_FLAGS))
		ᱨᱩᱣᱟᱹᱲ 0;
	ᱡᱩᱫᱤ (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		ᱨᱩᱣᱟᱹᱲ 1;
	}
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka show_default(void)
{
	const var_na *s = def;

	ᱡᱩᱫᱤ (s) {
		sa_nracanaa object_id oid;

		def = NULL;
		ᱡᱩᱫᱤ (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			ᱨᱩᱣᱟᱹᱲ 1;
		}
	}
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka show_reference(const sa_nracanaa reference *ref, void *cb_data UNUSED)
{
	ᱡᱩᱫᱤ (ref_excluded(&ref_excludes, ref->name))
		ᱨᱩᱣᱟᱹᱲ 0;
	show_rev(NORMAL, ref->oid, ref->name);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka anti_reference(const sa_nracanaa reference *ref, void *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka show_abbrev(const sa_nracanaa object_id *oid, void *cb_data UNUSED)
{
	show_rev(NORMAL, oid, NULL);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static void show_datestring(const var_na *flag, const var_na *datestr)
{
	var_na *buffer;

	/* date handling requires both flags and revs */
	ᱡᱩᱫᱤ ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		ᱨᱩᱣᱟᱹᱲ;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	free(buffer);
}

static puur_naa_nka show_file(const var_na *arg, puur_naa_nka output_prefix)
{
	show_default();
	ᱡᱩᱫᱤ ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		ᱡᱩᱫᱤ (output_prefix) {
			const var_na *prefix = the_repository->prefix;
			var_na *fname = prefix_filename(prefix, arg);
			show(fname);
			free(fname);
		} ᱵᱟᱝ
			show(arg);
		ᱨᱩᱣᱟᱹᱲ 1;
	}
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka try_difference(const var_na *arg)
{
	const var_na *dotdot;
	sa_nracanaa object_id start_oid;
	sa_nracanaa object_id end_oid;
	const var_na *end;
	const var_na *start;
	var_na *to_free;
	puur_naa_nka symmetric;
	static const var_na head_by_default[] = "HEAD";

	ᱡᱩᱫᱤ (!(dotdot = strstr(arg, "..")))
		ᱨᱩᱣᱟᱹᱲ 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	ᱡᱩᱫᱤ (!*end)
		end = head_by_default;
	ᱡᱩᱫᱤ (dotdot == arg)
		start = head_by_default;

	ᱡᱩᱫᱤ (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		free(to_free);
		ᱨᱩᱣᱟᱹᱲ 0;
	}

	ᱡᱩᱫᱤ (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		ᱡᱩᱫᱤ (symmetric) {
			sa_nracanaa commit_list *exclude = NULL;
			sa_nracanaa commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			ᱡᱩᱫᱤ (!a || !b) {
				free(to_free);
				ᱨᱩᱣᱟᱹᱲ 0;
			}
			ᱡᱩᱫᱤ (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			ᱡᱚᱠᱷᱚᱱ (exclude) {
				sa_nracanaa commit *commit = pop_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, NULL);
			}
		}
		free(to_free);
		ᱨᱩᱣᱟᱹᱲ 1;
	}
	free(to_free);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka try_parent_shorthands(const var_na *arg)
{
	const var_na *mark;
	sa_nracanaa object_id oid;
	sa_nracanaa commit *commit;
	sa_nracanaa commit_list *parents;
	puur_naa_nka parent_number;
	puur_naa_nka include_rev = 0;
	puur_naa_nka include_parents = 0;
	puur_naa_nka exclude_parent = 0;
	var_na *to_free;

	ᱡᱩᱫᱤ ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		ᱡᱩᱫᱤ (mark[2])
			ᱨᱩᱣᱟᱹᱲ 0;
	} ᱵᱟᱝ ᱡᱩᱫᱤ ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		ᱡᱩᱫᱤ (mark[2])
			ᱨᱩᱣᱟᱹᱲ 0;
	} ᱵᱟᱝ ᱡᱩᱫᱤ ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		ᱡᱩᱫᱤ (mark[2]) {
			var_na *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			ᱡᱩᱫᱤ (*end != '\0' || !exclude_parent)
				ᱨᱩᱣᱟᱹᱲ 0;
		}
	} ᱵᱟᱝ
		ᱨᱩᱣᱟᱹᱲ 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	ᱡᱩᱫᱤ (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		free(to_free);
		ᱨᱩᱣᱟᱹᱲ 0;
	}

	ᱡᱩᱫᱤ (exclude_parent &&
	    exclude_parent > commit_list_count(commit->parents)) {
		free(to_free);
		ᱨᱩᱣᱟᱹᱲ 0;
	}

	ᱡᱩᱫᱤ (include_rev)
		show_rev(NORMAL, &oid, arg);
	for (parents = commit->parents, parent_number = 1;
	     parents;
	     parents = parents->next, parent_number++) {
		var_na *name = NULL;

		ᱡᱩᱫᱤ (exclude_parent && parent_number != exclude_parent)
			continue;

		ᱡᱩᱫᱤ (symbolic)
			name = xstrfmt("%s^%d", arg, parent_number);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		free(name);
	}

	free(to_free);
	ᱨᱩᱣᱟᱹᱲ 1;
}

static puur_naa_nka parseopt_dump(const sa_nracanaa option *o, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa strbuf *parsed = o->value;
	ᱡᱩᱫᱤ (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	ᱵᱟᱝ ᱡᱩᱫᱤ (o->short_name && (o->long_name == NULL || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	ᱵᱟᱝ
		strbuf_addf(parsed, " --%s", o->long_name);
	ᱡᱩᱫᱤ (arg) {
		ᱡᱩᱫᱤ (!stuck_long)
			strbuf_addch(parsed, ' ');
		ᱵᱟᱝ ᱡᱩᱫᱤ (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	ᱨᱩᱣᱟᱹᱲ 0;
}

static const var_na *skipspaces(const var_na *s)
{
	ᱡᱚᱠᱷᱚᱱ (isspace(*s))
		s++;
	ᱨᱩᱣᱟᱹᱲ s;
}

static var_na *findspace(const var_na *s)
{
	for (; *s; s++)
		ᱡᱩᱫᱤ (isspace(*s))
			ᱨᱩᱣᱟᱹᱲ (var_na*)s;
	ᱨᱩᱣᱟᱹᱲ NULL;
}

static puur_naa_nka cmd_parseopt(puur_naa_nka argc, const var_na **argv, const var_na *prefix)
{
	puur_naa_nka keep_dashdash = 0, stop_at_non_option = 0;
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
	ᱡᱩᱫᱤ (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	for (;;) {
		strbuf_reset(&sb);
		ᱡᱩᱫᱤ (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		ᱡᱩᱫᱤ (!strcmp("--", sb.buf)) {
			ᱡᱩᱫᱤ (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			bha_nga;
		}

		strvec_push(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	ᱡᱚᱠᱷᱚᱱ (strbuf_getline(&sb, stdin) != EOF) {
		const var_na *s;
		var_na *help;
		sa_nracanaa option *o;

		ᱡᱩᱫᱤ (!sb.len)
			continue;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, parimaa_na(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		ᱡᱩᱫᱤ (!help || sb.buf == help) {
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
		ᱡᱩᱫᱤ (!s)
			s = help;

		ᱡᱩᱫᱤ (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		ᱡᱩᱫᱤ (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} ᱵᱟᱝ ᱡᱩᱫᱤ (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (puur_naa_nka)(s - sb.buf), sb.buf);
		} ᱵᱟᱝ {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (puur_naa_nka)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		ᱡᱚᱠᱷᱚᱱ (s < help) {
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

		ᱡᱩᱫᱤ (s < help)
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
	for (size_t i = 0; i < opts_nr; i++) {
		free((var_na *) opts[i].help);
		free((var_na *) opts[i].argh);
	}
	free(opts);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka cmd_sq_quote(puur_naa_nka argc, const var_na **argv)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;

	ᱡᱩᱫᱤ (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	ᱨᱩᱣᱟᱹᱲ 0;
}

static void die_no_single_rev(puur_naa_nka quiet)
{
	ᱡᱩᱫᱤ (quiet)
		exit(1);
	ᱵᱟᱝ
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
static puur_naa_nka opt_with_value(const var_na *arg, const var_na *opt, const var_na **value)
{
	ᱡᱩᱫᱤ (skip_prefix(arg, opt, &arg)) {
		ᱡᱩᱫᱤ (!*arg) {
			*value = NULL;
			ᱨᱩᱣᱟᱹᱲ 1;
		}
		ᱡᱩᱫᱤ (*arg++ == '=') {
			*value = arg;
			ᱨᱩᱣᱟᱹᱲ 1;
		}
	}
	ᱨᱩᱣᱟᱹᱲ 0;
}

static void handle_ref_opt(const var_na *pattern, const var_na *prefix)
{
	ᱡᱩᱫᱤ (pattern) {
		sa_nracanaa refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, NULL, &opts);
	} ᱵᱟᱝ {
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

	ᱡᱩᱫᱤ (format == FORMAT_DEFAULT) {
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
	} ᱵᱟᱝ {
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

puur_naa_nka cmd_rev_parse(puur_naa_nka argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka i, as_is = 0, verify = 0, quiet = 0, revs_count = 0, type = 0;
	const sa_nracanaa git_hash_algo *output_algo = NULL;
	const sa_nracanaa git_hash_algo *compat = NULL;
	puur_naa_nka did_repo_setup = 0;
	puur_naa_nka has_dashdash = 0;
	puur_naa_nka output_prefix = 0;
	sa_nracanaa object_id oid;
	unsigned puur_naa_nka flags = 0;
	const var_na *name = NULL;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	puur_naa_nka seen_end_of_options = 0;
	ga_nanaa format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	ᱡᱩᱫᱤ (argc > 1 && !strcmp("--parseopt", argv[1]))
		ᱨᱩᱣᱟᱹᱲ cmd_parseopt(argc - 1, argv + 1, prefix);

	ᱡᱩᱫᱤ (argc > 1 && !strcmp("--sq-quote", argv[1]))
		ᱨᱩᱣᱟᱹᱲ cmd_sq_quote(argc - 2, argv + 2);

	ᱡᱩᱫᱤ (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	for (i = 1; i < argc; i++) {
		ᱡᱩᱫᱤ (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			bha_nga;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	ᱡᱩᱫᱤ (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, NULL);
		ᱨᱩᱣᱟᱹᱲ 0;
	}

	for (i = 1; i < argc; i++) {
		const var_na *arg = argv[i];

		ᱡᱩᱫᱤ (as_is) {
			ᱡᱩᱫᱤ (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			continue;
		}

		ᱡᱩᱫᱤ (!seen_end_of_options) {
			ᱡᱩᱫᱤ (!strcmp(arg, "--local-env-vars")) {
				puur_naa_nka i;
				for (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--resolve-git-dir")) {
				const var_na *gitdir = argv[++i];
				ᱡᱩᱫᱤ (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				ᱡᱩᱫᱤ (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				continue;
			}
		}

		/* The rest of the options require a git repository. */
		ᱡᱩᱫᱤ (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, NULL);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		ᱡᱩᱫᱤ (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			ᱡᱩᱫᱤ (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			continue;
		}

		ᱡᱩᱫᱤ (!seen_end_of_options && *arg == '-') {
			ᱡᱩᱫᱤ (!strcmp(arg, "--git-path")) {
				ᱡᱩᱫᱤ (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg,"-n")) {
				ᱡᱩᱫᱤ (++i >= argc)
					die(_("-n requires an argument"));
				ᱡᱩᱫᱤ ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				continue;
			}
			ᱡᱩᱫᱤ (starts_with(arg, "-n")) {
				ᱡᱩᱫᱤ ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				continue;
			}
			ᱡᱩᱫᱤ (opt_with_value(arg, "--path-format", &arg)) {
				ᱡᱩᱫᱤ (!arg)
					die(_("--path-format requires an argument"));
				ᱡᱩᱫᱤ (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} ᱵᱟᱝ {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--default")) {
				def = argv[++i];
				ᱡᱩᱫᱤ (!def)
					die(_("--default requires an argument"));
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				ᱡᱩᱫᱤ (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				continue;
			}
			ᱡᱩᱫᱤ (opt_with_value(arg, "--output-object-format", &arg)) {
				ᱡᱩᱫᱤ (!arg)
					die(_("no object format specified"));
				ᱡᱩᱫᱤ (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					continue;
				}
				ᱵᱟᱝ ᱡᱩᱫᱤ (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					continue;
				}
				ᱵᱟᱝ die(_("unsupported object format: %s"), arg);
			}
			ᱡᱩᱫᱤ (opt_with_value(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				ᱡᱩᱫᱤ (!arg)
					continue;
				abbrev = strtoul(arg, NULL, 10);
				ᱡᱩᱫᱤ (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				ᱵᱟᱝ ᱡᱩᱫᱤ ((puur_naa_nka)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--sq")) {
				output_sq = 1;
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				continue;
			}
			ᱡᱩᱫᱤ (opt_with_value(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				ᱡᱩᱫᱤ (arg) {
					ᱡᱩᱫᱤ (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					ᱵᱟᱝ
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, NULL);
				clear_ref_exclusions(&ref_excludes);
				continue;
			}
			ᱡᱩᱫᱤ (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, NULL);
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--bisect")) {
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
			ᱡᱩᱫᱤ (opt_with_value(arg, "--branches", &arg)) {
				ᱡᱩᱫᱤ (ref_excludes.hidden_refs_configured)
					ᱨᱩᱣᱟᱹᱲ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				continue;
			}
			ᱡᱩᱫᱤ (opt_with_value(arg, "--tags", &arg)) {
				ᱡᱩᱫᱤ (ref_excludes.hidden_refs_configured)
					ᱨᱩᱣᱟᱹᱲ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				continue;
			}
			ᱡᱩᱫᱤ (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, NULL);
				continue;
			}
			ᱡᱩᱫᱤ (opt_with_value(arg, "--remotes", &arg)) {
				ᱡᱩᱫᱤ (ref_excludes.hidden_refs_configured)
					ᱨᱩᱣᱟᱹᱲ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				continue;
			}
			ᱡᱩᱫᱤ (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				continue;
			}
			ᱡᱩᱫᱤ (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--show-toplevel")) {
				const var_na *work_tree = repo_get_work_tree(the_repository);
				ᱡᱩᱫᱤ (work_tree)
					print_path(work_tree, prefix, format, DEFAULT_UNMODIFIED);
				ᱵᱟᱝ
					die(_("this operation must be run in a work tree"));
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--show-superproject-working-tree")) {
				sa_nracanaa strbuf superproject = STRBUF_INIT;
				ᱡᱩᱫᱤ (get_superproject_working_tree(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--show-prefix")) {
				ᱡᱩᱫᱤ (prefix)
					puts(prefix);
				ᱵᱟᱝ
					putchar('\n');
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--show-cdup")) {
				const var_na *pfx = prefix;
				ᱡᱩᱫᱤ (!is_inside_work_tree(the_repository)) {
					const var_na *work_tree =
						repo_get_work_tree(the_repository);
					ᱡᱩᱫᱤ (work_tree)
						printf("%s\n", work_tree);
					continue;
				}
				ᱡᱚᱠᱷᱚᱱ (pfx) {
					pfx = strchr(pfx, '/');
					ᱡᱩᱫᱤ (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				const var_na *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				var_na *cwd;
				puur_naa_nka len;
				ga_nanaa format_type wanted = format;
				ᱡᱩᱫᱤ (arg[2] == 'g') {	/* --git-dir */
					ᱡᱩᱫᱤ (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						continue;
					}
					ᱡᱩᱫᱤ (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						continue;
					}
				} ᱵᱟᱝ {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					ᱡᱩᱫᱤ (!gitdir && !prefix)
						gitdir = ".git";
					ᱡᱩᱫᱤ (gitdir) {
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
			ᱡᱩᱫᱤ (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_tree(the_repository) ? "true"
						: "false");
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--shared-index-path")) {
				ᱡᱩᱫᱤ (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				ᱡᱩᱫᱤ (the_repository->index->split_index) {
					const sa_nracanaa object_id *oid = &the_repository->index->split_index->base_oid;
					const var_na *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				continue;
			}
			ᱡᱩᱫᱤ (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				continue;
			}
			ᱡᱩᱫᱤ (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				continue;
			}
			ᱡᱩᱫᱤ (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				continue;
			}
			ᱡᱩᱫᱤ (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				continue;
			}
			ᱡᱩᱫᱤ (opt_with_value(arg, "--show-object-format", &arg)) {
				const var_na *val = arg ? arg : "storage";

				ᱡᱩᱫᱤ (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				ᱡᱩᱫᱤ (!strcmp(val, "compat")) {
					ᱡᱩᱫᱤ (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					ᱵᱟᱝ
						putchar('\n');
				} ᱵᱟᱝ {
					puts(the_hash_algo->name);
				}
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				continue;
			}
			ᱡᱩᱫᱤ (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				ᱡᱩᱫᱤ (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				continue;
			}
			ᱡᱩᱫᱤ (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			continue;
		}

		/* Not a flag argument */
		ᱡᱩᱫᱤ (try_difference(arg))
			continue;
		ᱡᱩᱫᱤ (try_parent_shorthands(arg))
			continue;
		name = arg;
		type = NORMAL;
		ᱡᱩᱫᱤ (*arg == '^') {
			name++;
			type = REVERSED;
		}
		ᱡᱩᱫᱤ (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			ᱡᱩᱫᱤ (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			ᱡᱩᱫᱤ (verify)
				revs_count++;
			ᱵᱟᱝ
				show_rev(type, &oid, name);
			continue;
		}
		ᱡᱩᱫᱤ (verify)
			die_no_single_rev(quiet);
		ᱡᱩᱫᱤ (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		ᱡᱩᱫᱤ (!show_file(arg, output_prefix))
			continue;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	ᱡᱩᱫᱤ (verify) {
		ᱡᱩᱫᱤ (revs_count == 1) {
			show_rev(type, &oid, name);
			ᱨᱩᱣᱟᱹᱲ 0;
		} ᱵᱟᱝ ᱡᱩᱫᱤ (revs_count == 0 && show_default())
			ᱨᱩᱣᱟᱹᱲ 0;
		die_no_single_rev(quiet);
	} ᱵᱟᱝ
		show_default();
	ᱨᱩᱣᱟᱹᱲ 0;
}
