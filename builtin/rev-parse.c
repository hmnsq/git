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
static số_nguyên filter = ~0;

static const char *def;

#define NORMAL 0
#define REVERSED 1
static số_nguyên show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
static số_nguyên symbolic;
static số_nguyên abbrev;
static số_nguyên abbrev_ref;
static số_nguyên abbrev_ref_strict;
static số_nguyên output_sq;

static số_nguyên stuck_long;
static struct ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
static số_nguyên is_rev_argument(const char *arg)
{
	static const char *rev_args[] = {
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
	const char **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	nếu ((*arg == '-') && isdigit(arg[1]))
		trả_về 1;

	cho (;;) {
		const char *str = *p++;
		số_nguyên len;
		nếu (!str)
			trả_về 0;
		len = strlen(str);
		nếu (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			trả_về 1;
	}
}

/* Output argument as a string, either SQ or normal */
static void show(const char *arg)
{
	nếu (output_sq) {
		số_nguyên sq = '\'', ch;

		putchar(sq);
		trong_khi ((ch = *arg++)) {
			nếu (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	không_thì
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
static void show_with_type(số_nguyên type, const char *arg)
{
	nếu (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
static void show_rev(số_nguyên type, const struct object_id *oid, const char *name)
{
	nếu (!(filter & DO_REVS))
		trả_về;
	def = NULL;

	nếu ((symbolic || abbrev_ref) && name) {
		nếu (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			struct object_id discard;
			char *full;

			switch (repo_dwim_ref(the_repository, name,
					      strlen(name), &discard, &full,
					      0)) {
			case 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				break;
			case 1: /* happy */
				nếu (abbrev_ref) {
					char *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					free(old);
				}
				show_with_type(type, full);
				break;
			default: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				break;
			}
			free(full);
		} không_thì {
			show_with_type(type, name);
		}
	}
	không_thì nếu (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	không_thì
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
static số_nguyên show_flag(const char *arg)
{
	nếu (!(filter & DO_FLAGS))
		trả_về 0;
	nếu (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		trả_về 1;
	}
	trả_về 0;
}

static số_nguyên show_default(void)
{
	const char *s = def;

	nếu (s) {
		struct object_id oid;

		def = NULL;
		nếu (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			trả_về 1;
		}
	}
	trả_về 0;
}

static số_nguyên show_reference(const struct reference *ref, void *cb_data UNUSED)
{
	nếu (ref_excluded(&ref_excludes, ref->name))
		trả_về 0;
	show_rev(NORMAL, ref->oid, ref->name);
	trả_về 0;
}

static số_nguyên anti_reference(const struct reference *ref, void *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	trả_về 0;
}

static số_nguyên show_abbrev(const struct object_id *oid, void *cb_data UNUSED)
{
	show_rev(NORMAL, oid, NULL);
	trả_về 0;
}

static void show_datestring(const char *flag, const char *datestr)
{
	char *buffer;

	/* date handling requires both flags and revs */
	nếu ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		trả_về;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	free(buffer);
}

static số_nguyên show_file(const char *arg, số_nguyên output_prefix)
{
	show_default();
	nếu ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		nếu (output_prefix) {
			const char *prefix = the_repository->prefix;
			char *fname = prefix_filename(prefix, arg);
			show(fname);
			free(fname);
		} không_thì
			show(arg);
		trả_về 1;
	}
	trả_về 0;
}

static số_nguyên try_difference(const char *arg)
{
	const char *dotdot;
	struct object_id start_oid;
	struct object_id end_oid;
	const char *end;
	const char *start;
	char *to_free;
	số_nguyên symmetric;
	static const char head_by_default[] = "HEAD";

	nếu (!(dotdot = strstr(arg, "..")))
		trả_về 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	nếu (!*end)
		end = head_by_default;
	nếu (dotdot == arg)
		start = head_by_default;

	nếu (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		free(to_free);
		trả_về 0;
	}

	nếu (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		nếu (symmetric) {
			struct commit_list *exclude = NULL;
			struct commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			nếu (!a || !b) {
				free(to_free);
				trả_về 0;
			}
			nếu (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			trong_khi (exclude) {
				struct commit *commit = pop_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, NULL);
			}
		}
		free(to_free);
		trả_về 1;
	}
	free(to_free);
	trả_về 0;
}

static số_nguyên try_parent_shorthands(const char *arg)
{
	const char *mark;
	struct object_id oid;
	struct commit *commit;
	struct commit_list *parents;
	số_nguyên parent_number;
	số_nguyên include_rev = 0;
	số_nguyên include_parents = 0;
	số_nguyên exclude_parent = 0;
	char *to_free;

	nếu ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		nếu (mark[2])
			trả_về 0;
	} không_thì nếu ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		nếu (mark[2])
			trả_về 0;
	} không_thì nếu ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		nếu (mark[2]) {
			char *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			nếu (*end != '\0' || !exclude_parent)
				trả_về 0;
		}
	} không_thì
		trả_về 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	nếu (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		free(to_free);
		trả_về 0;
	}

	nếu (exclude_parent &&
	    exclude_parent > commit_list_count(commit->parents)) {
		free(to_free);
		trả_về 0;
	}

	nếu (include_rev)
		show_rev(NORMAL, &oid, arg);
	cho (parents = commit->parents, parent_number = 1;
	     parents;
	     parents = parents->next, parent_number++) {
		char *name = NULL;

		nếu (exclude_parent && parent_number != exclude_parent)
			continue;

		nếu (symbolic)
			name = xstrfmt("%s^%d", arg, parent_number);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		free(name);
	}

	free(to_free);
	trả_về 1;
}

static số_nguyên parseopt_dump(const struct option *o, const char *arg, số_nguyên unset)
{
	struct strbuf *parsed = o->value;
	nếu (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	không_thì nếu (o->short_name && (o->long_name == NULL || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	không_thì
		strbuf_addf(parsed, " --%s", o->long_name);
	nếu (arg) {
		nếu (!stuck_long)
			strbuf_addch(parsed, ' ');
		không_thì nếu (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	trả_về 0;
}

static const char *skipspaces(const char *s)
{
	trong_khi (isspace(*s))
		s++;
	trả_về s;
}

static char *findspace(const char *s)
{
	cho (; *s; s++)
		nếu (isspace(*s))
			trả_về (char*)s;
	trả_về NULL;
}

static số_nguyên cmd_parseopt(số_nguyên argc, const char **argv, const char *prefix)
{
	số_nguyên keep_dashdash = 0, stop_at_non_option = 0;
	char const * const parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		NULL
	};
	struct option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_END(),
	};
	struct strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	struct strvec longnames = STRVEC_INIT;
	struct strvec usage = STRVEC_INIT;
	struct option *opts = NULL;
	size_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	nếu (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	cho (;;) {
		strbuf_reset(&sb);
		nếu (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		nếu (!strcmp("--", sb.buf)) {
			nếu (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			break;
		}

		strvec_push(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	trong_khi (strbuf_getline(&sb, stdin) != EOF) {
		const char *s;
		char *help;
		struct option *o;

		nếu (!sb.len)
			continue;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, sizeof(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		nếu (!help || sb.buf == help) {
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
		nếu (!s)
			s = help;

		nếu (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		nếu (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} không_thì nếu (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (số_nguyên)(s - sb.buf), sb.buf);
		} không_thì {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (số_nguyên)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		trong_khi (s < help) {
			switch (*s++) {
			case '=':
				o->flags &= ~PARSE_OPT_NOARG;
				continue;
			case '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				continue;
			case '!':
				o->flags |= PARSE_OPT_NONEG;
				continue;
			case '*':
				o->flags |= PARSE_OPT_HIDDEN;
				continue;
			}
			s--;
			break;
		}

		nếu (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, sizeof(*opts));
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
	cho (size_t i = 0; i < opts_nr; i++) {
		free((char *) opts[i].help);
		free((char *) opts[i].argh);
	}
	free(opts);
	trả_về 0;
}

static số_nguyên cmd_sq_quote(số_nguyên argc, const char **argv)
{
	struct strbuf buf = STRBUF_INIT;

	nếu (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	trả_về 0;
}

static void die_no_single_rev(số_nguyên quiet)
{
	nếu (quiet)
		exit(1);
	không_thì
		die(_("Needed a single revision"));
}

static const char builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
static số_nguyên opt_with_value(const char *arg, const char *opt, const char **value)
{
	nếu (skip_prefix(arg, opt, &arg)) {
		nếu (!*arg) {
			*value = NULL;
			trả_về 1;
		}
		nếu (*arg++ == '=') {
			*value = arg;
			trả_về 1;
		}
	}
	trả_về 0;
}

static void handle_ref_opt(const char *pattern, const char *prefix)
{
	nếu (pattern) {
		struct refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, NULL, &opts);
	} không_thì {
		struct refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = strlen(prefix),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, NULL, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

enum format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

enum default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

static void print_path(const char *path, const char *prefix,
		       enum format_type format, enum default_type def)
{
	struct strbuf sb = STRBUF_INIT;
	enum path_format fmt;

	nếu (format == FORMAT_DEFAULT) {
		switch (def) {
		case DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			break;
		case DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			break;
		case DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			break;
		case DEFAULT_UNMODIFIED:
		default:
			fmt = PATH_FORMAT_UNMODIFIED;
			break;
		}
	} không_thì {
		switch (format) {
		case FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			break;
		case FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			break;
		default:
			fmt = PATH_FORMAT_UNMODIFIED;
			break;
		}
	}

	format_path(&sb, path, prefix, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

số_nguyên cmd_rev_parse(số_nguyên argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	số_nguyên i, as_is = 0, verify = 0, quiet = 0, revs_count = 0, type = 0;
	const struct git_hash_algo *output_algo = NULL;
	const struct git_hash_algo *compat = NULL;
	số_nguyên did_repo_setup = 0;
	số_nguyên has_dashdash = 0;
	số_nguyên output_prefix = 0;
	struct object_id oid;
	unsigned số_nguyên flags = 0;
	const char *name = NULL;
	struct strbuf buf = STRBUF_INIT;
	số_nguyên seen_end_of_options = 0;
	enum format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	nếu (argc > 1 && !strcmp("--parseopt", argv[1]))
		trả_về cmd_parseopt(argc - 1, argv + 1, prefix);

	nếu (argc > 1 && !strcmp("--sq-quote", argv[1]))
		trả_về cmd_sq_quote(argc - 2, argv + 2);

	nếu (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	cho (i = 1; i < argc; i++) {
		nếu (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			break;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	nếu (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, NULL);
		trả_về 0;
	}

	cho (i = 1; i < argc; i++) {
		const char *arg = argv[i];

		nếu (as_is) {
			nếu (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			continue;
		}

		nếu (!seen_end_of_options) {
			nếu (!strcmp(arg, "--local-env-vars")) {
				số_nguyên i;
				cho (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				continue;
			}
			nếu (!strcmp(arg, "--resolve-git-dir")) {
				const char *gitdir = argv[++i];
				nếu (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				nếu (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				continue;
			}
		}

		/* The rest of the options require a git repository. */
		nếu (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, NULL);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		nếu (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			nếu (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			continue;
		}

		nếu (!seen_end_of_options && *arg == '-') {
			nếu (!strcmp(arg, "--git-path")) {
				nếu (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				continue;
			}
			nếu (!strcmp(arg,"-n")) {
				nếu (++i >= argc)
					die(_("-n requires an argument"));
				nếu ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				continue;
			}
			nếu (starts_with(arg, "-n")) {
				nếu ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				continue;
			}
			nếu (opt_with_value(arg, "--path-format", &arg)) {
				nếu (!arg)
					die(_("--path-format requires an argument"));
				nếu (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} không_thì nếu (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} không_thì {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				continue;
			}
			nếu (!strcmp(arg, "--default")) {
				def = argv[++i];
				nếu (!def)
					die(_("--default requires an argument"));
				continue;
			}
			nếu (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				nếu (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				continue;
			}
			nếu (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				continue;
			}
			nếu (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				continue;
			}
			nếu (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				continue;
			}
			nếu (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				continue;
			}
			nếu (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				continue;
			}
			nếu (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				continue;
			}
			nếu (opt_with_value(arg, "--output-object-format", &arg)) {
				nếu (!arg)
					die(_("no object format specified"));
				nếu (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					continue;
				}
				không_thì nếu (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					continue;
				}
				không_thì die(_("unsupported object format: %s"), arg);
			}
			nếu (opt_with_value(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				nếu (!arg)
					continue;
				abbrev = strtoul(arg, NULL, 10);
				nếu (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				không_thì nếu ((số_nguyên)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				continue;
			}
			nếu (!strcmp(arg, "--sq")) {
				output_sq = 1;
				continue;
			}
			nếu (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				continue;
			}
			nếu (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				continue;
			}
			nếu (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				continue;
			}
			nếu (opt_with_value(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				nếu (arg) {
					nếu (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					không_thì nếu (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					không_thì
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				continue;
			}
			nếu (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, NULL);
				clear_ref_exclusions(&ref_excludes);
				continue;
			}
			nếu (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, NULL);
				continue;
			}
			nếu (!strcmp(arg, "--bisect")) {
				char *prefix;
				char *term_bad = NULL;
				char *term_good = NULL;
				struct refs_for_each_ref_options opts = { 0 };
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
			nếu (opt_with_value(arg, "--branches", &arg)) {
				nếu (ref_excludes.hidden_refs_configured)
					trả_về error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				continue;
			}
			nếu (opt_with_value(arg, "--tags", &arg)) {
				nếu (ref_excludes.hidden_refs_configured)
					trả_về error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				continue;
			}
			nếu (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, NULL);
				continue;
			}
			nếu (opt_with_value(arg, "--remotes", &arg)) {
				nếu (ref_excludes.hidden_refs_configured)
					trả_về error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				continue;
			}
			nếu (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				continue;
			}
			nếu (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				continue;
			}
			nếu (!strcmp(arg, "--show-toplevel")) {
				const char *work_tree = repo_get_work_tree(the_repository);
				nếu (work_tree)
					print_path(work_tree, prefix, format, DEFAULT_UNMODIFIED);
				không_thì
					die(_("this operation must be run in a work tree"));
				continue;
			}
			nếu (!strcmp(arg, "--show-superproject-working-tree")) {
				struct strbuf superproject = STRBUF_INIT;
				nếu (get_superproject_working_tree(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				continue;
			}
			nếu (!strcmp(arg, "--show-prefix")) {
				nếu (prefix)
					puts(prefix);
				không_thì
					putchar('\n');
				continue;
			}
			nếu (!strcmp(arg, "--show-cdup")) {
				const char *pfx = prefix;
				nếu (!is_inside_work_tree(the_repository)) {
					const char *work_tree =
						repo_get_work_tree(the_repository);
					nếu (work_tree)
						printf("%s\n", work_tree);
					continue;
				}
				trong_khi (pfx) {
					pfx = strchr(pfx, '/');
					nếu (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				continue;
			}
			nếu (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				const char *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				char *cwd;
				số_nguyên len;
				enum format_type wanted = format;
				nếu (arg[2] == 'g') {	/* --git-dir */
					nếu (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						continue;
					}
					nếu (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						continue;
					}
				} không_thì {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					nếu (!gitdir && !prefix)
						gitdir = ".git";
					nếu (gitdir) {
						struct strbuf realpath = STRBUF_INIT;
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
			nếu (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				continue;
			}
			nếu (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				continue;
			}
			nếu (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_tree(the_repository) ? "true"
						: "false");
				continue;
			}
			nếu (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				continue;
			}
			nếu (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				continue;
			}
			nếu (!strcmp(arg, "--shared-index-path")) {
				nếu (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				nếu (the_repository->index->split_index) {
					const struct object_id *oid = &the_repository->index->split_index->base_oid;
					const char *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				continue;
			}
			nếu (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				continue;
			}
			nếu (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				continue;
			}
			nếu (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				continue;
			}
			nếu (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				continue;
			}
			nếu (opt_with_value(arg, "--show-object-format", &arg)) {
				const char *val = arg ? arg : "storage";

				nếu (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				nếu (!strcmp(val, "compat")) {
					nếu (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					không_thì
						putchar('\n');
				} không_thì {
					puts(the_hash_algo->name);
				}
				continue;
			}
			nếu (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				continue;
			}
			nếu (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				nếu (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				continue;
			}
			nếu (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			continue;
		}

		/* Not a flag argument */
		nếu (try_difference(arg))
			continue;
		nếu (try_parent_shorthands(arg))
			continue;
		name = arg;
		type = NORMAL;
		nếu (*arg == '^') {
			name++;
			type = REVERSED;
		}
		nếu (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			nếu (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			nếu (verify)
				revs_count++;
			không_thì
				show_rev(type, &oid, name);
			continue;
		}
		nếu (verify)
			die_no_single_rev(quiet);
		nếu (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		nếu (!show_file(arg, output_prefix))
			continue;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	nếu (verify) {
		nếu (revs_count == 1) {
			show_rev(type, &oid, name);
			trả_về 0;
		} không_thì nếu (revs_count == 0 && show_default())
			trả_về 0;
		die_no_single_rev(quiet);
	} không_thì
		show_default();
	trả_về 0;
}
