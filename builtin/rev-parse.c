/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"

#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "bisect.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "repo-settings.h"
#અન્તર્ભાવ "repository.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "split-index.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "commit-reach.h"
#અન્તર્ભાવ "shallow.h"
#અન્તર્ભાવ "object-file-convert.h"

#define DO_REVS		1
#define DO_NOREV	2
#define DO_FLAGS	4
#define DO_NONFLAGS	8
સ્થિર પૂર્ણાંક filter = ~0;

સ્થિર અચલ અક્ષર *def;

#define NORMAL 0
#define REVERSED 1
સ્થિર પૂર્ણાંક show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
સ્થિર પૂર્ણાંક symbolic;
સ્થિર પૂર્ણાંક abbrev;
સ્થિર પૂર્ણાંક abbrev_ref;
સ્થિર પૂર્ણાંક abbrev_ref_strict;
સ્થિર પૂર્ણાંક output_sq;

સ્થિર પૂર્ણાંક stuck_long;
સ્થિર સંરચના ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
સ્થિર પૂર્ણાંક is_rev_argument(અચલ અક્ષર *arg)
{
	સ્થિર અચલ અક્ષર *rev_args[] = {
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
		ખાલી
	};
	અચલ અક્ષર **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	જો ((*arg == '-') && isdigit(arg[1]))
		પાછા 1;

	માટે (;;) {
		અચલ અક્ષર *str = *p++;
		પૂર્ણાંક len;
		જો (!str)
			પાછા 0;
		len = strlen(str);
		જો (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			પાછા 1;
	}
}

/* Output argument as a string, either SQ or normal */
સ્થિર શૂન્ય show(અચલ અક્ષર *arg)
{
	જો (output_sq) {
		પૂર્ણાંક sq = '\'', ch;

		putchar(sq);
		જ્યારે ((ch = *arg++)) {
			જો (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	નહીંતર
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
સ્થિર શૂન્ય show_with_type(પૂર્ણાંક type, અચલ અક્ષર *arg)
{
	જો (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
સ્થિર શૂન્ય show_rev(પૂર્ણાંક type, અચલ સંરચના object_id *oid, અચલ અક્ષર *name)
{
	જો (!(filter & DO_REVS))
		પાછા;
	def = ખાલી;

	જો ((symbolic || abbrev_ref) && name) {
		જો (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			સંરચના object_id discard;
			અક્ષર *full;

			પસંદગી (repo_dwim_ref(the_repository, name,
					      strlen(name), &discard, &full,
					      0)) {
			સ્થિતિ 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				તોડો;
			સ્થિતિ 1: /* happy */
				જો (abbrev_ref) {
					અક્ષર *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					મુક્ત(old);
				}
				show_with_type(type, full);
				તોડો;
			મૂળભૂત: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				તોડો;
			}
			મુક્ત(full);
		} નહીંતર {
			show_with_type(type, name);
		}
	}
	નહીંતર જો (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	નહીંતર
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
સ્થિર પૂર્ણાંક show_flag(અચલ અક્ષર *arg)
{
	જો (!(filter & DO_FLAGS))
		પાછા 0;
	જો (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		પાછા 1;
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક show_default(શૂન્ય)
{
	અચલ અક્ષર *s = def;

	જો (s) {
		સંરચના object_id oid;

		def = ખાલી;
		જો (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			પાછા 1;
		}
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક show_reference(અચલ સંરચના reference *ref, શૂન્ય *cb_data UNUSED)
{
	જો (ref_excluded(&ref_excludes, ref->name))
		પાછા 0;
	show_rev(NORMAL, ref->oid, ref->name);
	પાછા 0;
}

સ્થિર પૂર્ણાંક anti_reference(અચલ સંરચના reference *ref, શૂન્ય *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	પાછા 0;
}

સ્થિર પૂર્ણાંક show_abbrev(અચલ સંરચના object_id *oid, શૂન્ય *cb_data UNUSED)
{
	show_rev(NORMAL, oid, ખાલી);
	પાછા 0;
}

સ્થિર શૂન્ય show_datestring(અચલ અક્ષર *flag, અચલ અક્ષર *datestr)
{
	અક્ષર *buffer;

	/* date handling requires both flags and revs */
	જો ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		પાછા;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	મુક્ત(buffer);
}

સ્થિર પૂર્ણાંક show_file(અચલ અક્ષર *arg, પૂર્ણાંક output_prefix)
{
	show_default();
	જો ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		જો (output_prefix) {
			અચલ અક્ષર *prefix = the_repository->prefix;
			અક્ષર *fname = prefix_filename(prefix, arg);
			show(fname);
			મુક્ત(fname);
		} નહીંતર
			show(arg);
		પાછા 1;
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક try_difference(અચલ અક્ષર *arg)
{
	અચલ અક્ષર *dotdot;
	સંરચના object_id start_oid;
	સંરચના object_id end_oid;
	અચલ અક્ષર *end;
	અચલ અક્ષર *start;
	અક્ષર *to_free;
	પૂર્ણાંક symmetric;
	સ્થિર અચલ અક્ષર head_by_default[] = "HEAD";

	જો (!(dotdot = strstr(arg, "..")))
		પાછા 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	જો (!*end)
		end = head_by_default;
	જો (dotdot == arg)
		start = head_by_default;

	જો (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		મુક્ત(to_free);
		પાછા 0;
	}

	જો (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		જો (symmetric) {
			સંરચના commit_list *exclude = ખાલી;
			સંરચના commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			જો (!a || !b) {
				મુક્ત(to_free);
				પાછા 0;
			}
			જો (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			જ્યારે (exclude) {
				સંરચના commit *commit = કાઢો_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, ખાલી);
			}
		}
		મુક્ત(to_free);
		પાછા 1;
	}
	મુક્ત(to_free);
	પાછા 0;
}

સ્થિર પૂર્ણાંક try_parent_shorthands(અચલ અક્ષર *arg)
{
	અચલ અક્ષર *mark;
	સંરચના object_id oid;
	સંરચના commit *commit;
	સંરચના commit_list *parents;
	પૂર્ણાંક parent_સંખ્યા;
	પૂર્ણાંક include_rev = 0;
	પૂર્ણાંક include_parents = 0;
	પૂર્ણાંક exclude_parent = 0;
	અક્ષર *to_free;

	જો ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		જો (mark[2])
			પાછા 0;
	} નહીંતર જો ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		જો (mark[2])
			પાછા 0;
	} નહીંતર જો ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		જો (mark[2]) {
			અક્ષર *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			જો (*end != '\0' || !exclude_parent)
				પાછા 0;
		}
	} નહીંતર
		પાછા 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	જો (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		મુક્ત(to_free);
		પાછા 0;
	}

	જો (exclude_parent &&
	    exclude_parent > commit_list_ગણતરી(commit->parents)) {
		મુક્ત(to_free);
		પાછા 0;
	}

	જો (include_rev)
		show_rev(NORMAL, &oid, arg);
	માટે (parents = commit->parents, parent_સંખ્યા = 1;
	     parents;
	     parents = parents->next, parent_સંખ્યા++) {
		અક્ષર *name = ખાલી;

		જો (exclude_parent && parent_સંખ્યા != exclude_parent)
			ચાલુ;

		જો (symbolic)
			name = xstrfmt("%s^%d", arg, parent_સંખ્યા);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		મુક્ત(name);
	}

	મુક્ત(to_free);
	પાછા 1;
}

સ્થિર પૂર્ણાંક parseopt_dump(અચલ સંરચના option *o, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના strbuf *parsed = o->કિંમત;
	જો (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	નહીંતર જો (o->short_name && (o->long_name == ખાલી || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	નહીંતર
		strbuf_addf(parsed, " --%s", o->long_name);
	જો (arg) {
		જો (!stuck_long)
			strbuf_addch(parsed, ' ');
		નહીંતર જો (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	પાછા 0;
}

સ્થિર અચલ અક્ષર *skipspaces(અચલ અક્ષર *s)
{
	જ્યારે (isspace(*s))
		s++;
	પાછા s;
}

સ્થિર અક્ષર *findspace(અચલ અક્ષર *s)
{
	માટે (; *s; s++)
		જો (isspace(*s))
			પાછા (અક્ષર*)s;
	પાછા ખાલી;
}

સ્થિર પૂર્ણાંક cmd_parseopt(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix)
{
	પૂર્ણાંક keep_dashdash = 0, stop_at_non_option = 0;
	અક્ષર અચલ * અચલ parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		ખાલી
	};
	સંરચના option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_END(),
	};
	સંરચના strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	સંરચના strvec longnames = STRVEC_INIT;
	સંરચના strvec usage = STRVEC_INIT;
	સંરચના option *opts = ખાલી;
	size_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	જો (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	માટે (;;) {
		strbuf_reset(&sb);
		જો (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		જો (!strcmp("--", sb.buf)) {
			જો (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			તોડો;
		}

		strvec_ધકેલો(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	જ્યારે (strbuf_getline(&sb, stdin) != EOF) {
		અચલ અક્ષર *s;
		અક્ષર *help;
		સંરચના option *o;

		જો (!sb.len)
			ચાલુ;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, કદ(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		જો (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			ચાલુ;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->કિંમત = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		જો (!s)
			s = help;

		જો (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		જો (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} નહીંતર જો (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (પૂર્ણાંક)(s - sb.buf), sb.buf);
		} નહીંતર {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (પૂર્ણાંક)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		જ્યારે (s < help) {
			પસંદગી (*s++) {
			સ્થિતિ '=':
				o->flags &= ~PARSE_OPT_NOARG;
				ચાલુ;
			સ્થિતિ '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				ચાલુ;
			સ્થિતિ '!':
				o->flags |= PARSE_OPT_NONEG;
				ચાલુ;
			સ્થિતિ '*':
				o->flags |= PARSE_OPT_HIDDEN;
				ચાલુ;
			}
			s--;
			તોડો;
		}

		જો (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, કદ(*opts));
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
	માટે (size_t i = 0; i < opts_nr; i++) {
		મુક્ત((અક્ષર *) opts[i].help);
		મુક્ત((અક્ષર *) opts[i].argh);
	}
	મુક્ત(opts);
	પાછા 0;
}

સ્થિર પૂર્ણાંક cmd_sq_quote(પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
	સંરચના strbuf buf = STRBUF_INIT;

	જો (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	પાછા 0;
}

સ્થિર શૂન્ય die_no_single_rev(પૂર્ણાંક quiet)
{
	જો (quiet)
		exit(1);
	નહીંતર
		die(_("Needed a single revision"));
}

સ્થિર અચલ અક્ષર builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
સ્થિર પૂર્ણાંક opt_with_કિંમત(અચલ અક્ષર *arg, અચલ અક્ષર *opt, અચલ અક્ષર **કિંમત)
{
	જો (skip_prefix(arg, opt, &arg)) {
		જો (!*arg) {
			*કિંમત = ખાલી;
			પાછા 1;
		}
		જો (*arg++ == '=') {
			*કિંમત = arg;
			પાછા 1;
		}
	}
	પાછા 0;
}

સ્થિર શૂન્ય handle_ref_opt(અચલ અક્ષર *pattern, અચલ અક્ષર *prefix)
{
	જો (pattern) {
		સંરચના refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ખાલી, &opts);
	} નહીંતર {
		સંરચના refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = strlen(prefix),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ખાલી, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

ગણના format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

ગણના default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

સ્થિર શૂન્ય print_path(અચલ અક્ષર *path, અચલ અક્ષર *prefix,
		       ગણના format_type format, ગણના default_type def)
{
	સંરચના strbuf sb = STRBUF_INIT;
	ગણના path_format fmt;

	જો (format == FORMAT_DEFAULT) {
		પસંદગી (def) {
		સ્થિતિ DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			તોડો;
		સ્થિતિ DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			તોડો;
		સ્થિતિ DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			તોડો;
		સ્થિતિ DEFAULT_UNMODIFIED:
		મૂળભૂત:
			fmt = PATH_FORMAT_UNMODIFIED;
			તોડો;
		}
	} નહીંતર {
		પસંદગી (format) {
		સ્થિતિ FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			તોડો;
		સ્થિતિ FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			તોડો;
		મૂળભૂત:
			fmt = PATH_FORMAT_UNMODIFIED;
			તોડો;
		}
	}

	format_path(&sb, path, prefix, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

પૂર્ણાંક cmd_rev_parse(પૂર્ણાંક argc,
		  અચલ અક્ષર **argv,
		  અચલ અક્ષર *prefix,
		  સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i, as_is = 0, verify = 0, quiet = 0, revs_ગણતરી = 0, type = 0;
	અચલ સંરચના git_hash_algo *output_algo = ખાલી;
	અચલ સંરચના git_hash_algo *compat = ખાલી;
	પૂર્ણાંક did_repo_setup = 0;
	પૂર્ણાંક has_dashdash = 0;
	પૂર્ણાંક output_prefix = 0;
	સંરચના object_id oid;
	unsigned પૂર્ણાંક flags = 0;
	અચલ અક્ષર *name = ખાલી;
	સંરચના strbuf buf = STRBUF_INIT;
	પૂર્ણાંક seen_end_of_options = 0;
	ગણના format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	જો (argc > 1 && !strcmp("--parseopt", argv[1]))
		પાછા cmd_parseopt(argc - 1, argv + 1, prefix);

	જો (argc > 1 && !strcmp("--sq-quote", argv[1]))
		પાછા cmd_sq_quote(argc - 2, argv + 2);

	જો (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	માટે (i = 1; i < argc; i++) {
		જો (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			તોડો;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	જો (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, ખાલી);
		પાછા 0;
	}

	માટે (i = 1; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];

		જો (as_is) {
			જો (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			ચાલુ;
		}

		જો (!seen_end_of_options) {
			જો (!strcmp(arg, "--local-env-vars")) {
				પૂર્ણાંક i;
				માટે (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				ચાલુ;
			}
			જો (!strcmp(arg, "--resolve-git-dir")) {
				અચલ અક્ષર *gitdir = argv[++i];
				જો (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				જો (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				ચાલુ;
			}
		}

		/* The rest of the options require a git repository. */
		જો (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, ખાલી);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		જો (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			જો (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			ચાલુ;
		}

		જો (!seen_end_of_options && *arg == '-') {
			જો (!strcmp(arg, "--git-path")) {
				જો (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				ચાલુ;
			}
			જો (!strcmp(arg,"-n")) {
				જો (++i >= argc)
					die(_("-n requires an argument"));
				જો ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				ચાલુ;
			}
			જો (starts_with(arg, "-n")) {
				જો ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				ચાલુ;
			}
			જો (opt_with_કિંમત(arg, "--path-format", &arg)) {
				જો (!arg)
					die(_("--path-format requires an argument"));
				જો (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} નહીંતર જો (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} નહીંતર {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				ચાલુ;
			}
			જો (!strcmp(arg, "--default")) {
				def = argv[++i];
				જો (!def)
					die(_("--default requires an argument"));
				ચાલુ;
			}
			જો (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				જો (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				ચાલુ;
			}
			જો (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				ચાલુ;
			}
			જો (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				ચાલુ;
			}
			જો (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				ચાલુ;
			}
			જો (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				ચાલુ;
			}
			જો (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				ચાલુ;
			}
			જો (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				ચાલુ;
			}
			જો (opt_with_કિંમત(arg, "--output-object-format", &arg)) {
				જો (!arg)
					die(_("no object format specified"));
				જો (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					ચાલુ;
				}
				નહીંતર જો (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					ચાલુ;
				}
				નહીંતર die(_("unsupported object format: %s"), arg);
			}
			જો (opt_with_કિંમત(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				જો (!arg)
					ચાલુ;
				abbrev = strtoul(arg, ખાલી, 10);
				જો (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				નહીંતર જો ((પૂર્ણાંક)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				ચાલુ;
			}
			જો (!strcmp(arg, "--sq")) {
				output_sq = 1;
				ચાલુ;
			}
			જો (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				ચાલુ;
			}
			જો (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				ચાલુ;
			}
			જો (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				ચાલુ;
			}
			જો (opt_with_કિંમત(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				જો (arg) {
					જો (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					નહીંતર જો (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					નહીંતર
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				ચાલુ;
			}
			જો (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, ખાલી);
				clear_ref_exclusions(&ref_excludes);
				ચાલુ;
			}
			જો (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, ખાલી);
				ચાલુ;
			}
			જો (!strcmp(arg, "--bisect")) {
				અક્ષર *prefix;
				અક્ષર *term_bad = ખાલી;
				અક્ષર *term_good = ખાલી;
				સંરચના refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&term_bad, &term_good);
				prefix = xstrfmt("refs/bisect/%s", term_bad);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, ખાલી, &opts);
				મુક્ત(prefix);
				prefix = xstrfmt("refs/bisect/%s", term_good);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, ખાલી, &opts);
				મુક્ત(prefix);
				મુક્ત(term_good);
				મુક્ત(term_bad);
				ચાલુ;
			}
			જો (opt_with_કિંમત(arg, "--branches", &arg)) {
				જો (ref_excludes.hidden_refs_configured)
					પાછા error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				ચાલુ;
			}
			જો (opt_with_કિંમત(arg, "--tags", &arg)) {
				જો (ref_excludes.hidden_refs_configured)
					પાછા error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				ચાલુ;
			}
			જો (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, ખાલી);
				ચાલુ;
			}
			જો (opt_with_કિંમત(arg, "--remotes", &arg)) {
				જો (ref_excludes.hidden_refs_configured)
					પાછા error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				ચાલુ;
			}
			જો (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				ચાલુ;
			}
			જો (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				ચાલુ;
			}
			જો (!strcmp(arg, "--show-toplevel")) {
				અચલ અક્ષર *work_વૃક્ષ = repo_get_work_વૃક્ષ(the_repository);
				જો (work_વૃક્ષ)
					print_path(work_વૃક્ષ, prefix, format, DEFAULT_UNMODIFIED);
				નહીંતર
					die(_("this operation must be run in a work tree"));
				ચાલુ;
			}
			જો (!strcmp(arg, "--show-superproject-working-tree")) {
				સંરચના strbuf superproject = STRBUF_INIT;
				જો (get_superproject_working_વૃક્ષ(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				ચાલુ;
			}
			જો (!strcmp(arg, "--show-prefix")) {
				જો (prefix)
					puts(prefix);
				નહીંતર
					putchar('\n');
				ચાલુ;
			}
			જો (!strcmp(arg, "--show-cdup")) {
				અચલ અક્ષર *pfx = prefix;
				જો (!is_inside_work_વૃક્ષ(the_repository)) {
					અચલ અક્ષર *work_વૃક્ષ =
						repo_get_work_વૃક્ષ(the_repository);
					જો (work_વૃક્ષ)
						printf("%s\n", work_વૃક્ષ);
					ચાલુ;
				}
				જ્યારે (pfx) {
					pfx = strchr(pfx, '/');
					જો (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				ચાલુ;
			}
			જો (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				અચલ અક્ષર *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				અક્ષર *cwd;
				પૂર્ણાંક len;
				ગણના format_type wanted = format;
				જો (arg[2] == 'g') {	/* --git-dir */
					જો (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						ચાલુ;
					}
					જો (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						ચાલુ;
					}
				} નહીંતર {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					જો (!gitdir && !prefix)
						gitdir = ".git";
					જો (gitdir) {
						સંરચના strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						ચાલુ;
					}
				}
				cwd = xgetcwd();
				len = strlen(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				મુક્ત(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				ચાલુ;
			}
			જો (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				ચાલુ;
			}
			જો (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				ચાલુ;
			}
			જો (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_વૃક્ષ(the_repository) ? "true"
						: "false");
				ચાલુ;
			}
			જો (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				ચાલુ;
			}
			જો (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				ચાલુ;
			}
			જો (!strcmp(arg, "--shared-index-path")) {
				જો (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				જો (the_repository->index->split_index) {
					અચલ સંરચના object_id *oid = &the_repository->index->split_index->base_oid;
					અચલ અક્ષર *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				ચાલુ;
			}
			જો (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				ચાલુ;
			}
			જો (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				ચાલુ;
			}
			જો (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				ચાલુ;
			}
			જો (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				ચાલુ;
			}
			જો (opt_with_કિંમત(arg, "--show-object-format", &arg)) {
				અચલ અક્ષર *val = arg ? arg : "storage";

				જો (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				જો (!strcmp(val, "compat")) {
					જો (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					નહીંતર
						putchar('\n');
				} નહીંતર {
					puts(the_hash_algo->name);
				}
				ચાલુ;
			}
			જો (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				ચાલુ;
			}
			જો (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				જો (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				ચાલુ;
			}
			જો (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			ચાલુ;
		}

		/* Not a flag argument */
		જો (try_difference(arg))
			ચાલુ;
		જો (try_parent_shorthands(arg))
			ચાલુ;
		name = arg;
		type = NORMAL;
		જો (*arg == '^') {
			name++;
			type = REVERSED;
		}
		જો (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			જો (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			જો (verify)
				revs_ગણતરી++;
			નહીંતર
				show_rev(type, &oid, name);
			ચાલુ;
		}
		જો (verify)
			die_no_single_rev(quiet);
		જો (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		જો (!show_file(arg, output_prefix))
			ચાલુ;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	જો (verify) {
		જો (revs_ગણતરી == 1) {
			show_rev(type, &oid, name);
			પાછા 0;
		} નહીંતર જો (revs_ગણતરી == 0 && show_default())
			પાછા 0;
		die_no_single_rev(quiet);
	} નહીંતર
		show_default();
	પાછા 0;
}
