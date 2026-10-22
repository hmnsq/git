/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"

#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "bisect.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "repo-settings.h"
#அந்தர்பாவ "repository.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "split-index.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "commit-reach.h"
#அந்தர்பாவ "shallow.h"
#அந்தர்பாவ "object-file-convert.h"

#define DO_REVS		1
#define DO_NOREV	2
#define DO_FLAGS	4
#define DO_NONFLAGS	8
நிலையான முழு filter = ~0;

நிலையான மாறிலி எழுத்து *def;

#define NORMAL 0
#define REVERSED 1
நிலையான முழு show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
நிலையான முழு symbolic;
நிலையான முழு abbrev;
நிலையான முழு abbrev_ref;
நிலையான முழு abbrev_ref_strict;
நிலையான முழு output_sq;

நிலையான முழு stuck_long;
நிலையான கட்டமைப்பு ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
நிலையான முழு is_rev_argument(மாறிலி எழுத்து *arg)
{
	நிலையான மாறிலி எழுத்து *rev_args[] = {
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
		வெற்று
	};
	மாறிலி எழுத்து **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	எனில் ((*arg == '-') && isdigit(arg[1]))
		திரும்பு 1;

	க்கு (;;) {
		மாறிலி எழுத்து *str = *p++;
		முழு len;
		எனில் (!str)
			திரும்பு 0;
		len = strlen(str);
		எனில் (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			திரும்பு 1;
	}
}

/* Output argument as a string, either SQ or normal */
நிலையான வெறுமை show(மாறிலி எழுத்து *arg)
{
	எனில் (output_sq) {
		முழு sq = '\'', ch;

		putchar(sq);
		போது ((ch = *arg++)) {
			எனில் (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	இல்லையெனில்
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
நிலையான வெறுமை show_with_type(முழு type, மாறிலி எழுத்து *arg)
{
	எனில் (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
நிலையான வெறுமை show_rev(முழு type, மாறிலி கட்டமைப்பு object_id *oid, மாறிலி எழுத்து *name)
{
	எனில் (!(filter & DO_REVS))
		திரும்பு;
	def = வெற்று;

	எனில் ((symbolic || abbrev_ref) && name) {
		எனில் (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			கட்டமைப்பு object_id discard;
			எழுத்து *full;

			தேர்வு (repo_dwim_ref(the_repository, name,
					      strlen(name), &discard, &full,
					      0)) {
			நிலை 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				நிறுத்து;
			நிலை 1: /* happy */
				எனில் (abbrev_ref) {
					எழுத்து *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					விடுவி(old);
				}
				show_with_type(type, full);
				நிறுத்து;
			இயல்பு: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				நிறுத்து;
			}
			விடுவி(full);
		} இல்லையெனில் {
			show_with_type(type, name);
		}
	}
	இல்லையெனில் எனில் (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	இல்லையெனில்
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
நிலையான முழு show_flag(மாறிலி எழுத்து *arg)
{
	எனில் (!(filter & DO_FLAGS))
		திரும்பு 0;
	எனில் (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		திரும்பு 1;
	}
	திரும்பு 0;
}

நிலையான முழு show_default(வெறுமை)
{
	மாறிலி எழுத்து *s = def;

	எனில் (s) {
		கட்டமைப்பு object_id oid;

		def = வெற்று;
		எனில் (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			திரும்பு 1;
		}
	}
	திரும்பு 0;
}

நிலையான முழு show_reference(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data UNUSED)
{
	எனில் (ref_excluded(&ref_excludes, ref->name))
		திரும்பு 0;
	show_rev(NORMAL, ref->oid, ref->name);
	திரும்பு 0;
}

நிலையான முழு anti_reference(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	திரும்பு 0;
}

நிலையான முழு show_abbrev(மாறிலி கட்டமைப்பு object_id *oid, வெறுமை *cb_data UNUSED)
{
	show_rev(NORMAL, oid, வெற்று);
	திரும்பு 0;
}

நிலையான வெறுமை show_datestring(மாறிலி எழுத்து *flag, மாறிலி எழுத்து *datestr)
{
	எழுத்து *buffer;

	/* date handling requires both flags and revs */
	எனில் ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		திரும்பு;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	விடுவி(buffer);
}

நிலையான முழு show_file(மாறிலி எழுத்து *arg, முழு output_prefix)
{
	show_default();
	எனில் ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		எனில் (output_prefix) {
			மாறிலி எழுத்து *prefix = the_repository->prefix;
			எழுத்து *fname = prefix_filename(prefix, arg);
			show(fname);
			விடுவி(fname);
		} இல்லையெனில்
			show(arg);
		திரும்பு 1;
	}
	திரும்பு 0;
}

நிலையான முழு try_difference(மாறிலி எழுத்து *arg)
{
	மாறிலி எழுத்து *dotdot;
	கட்டமைப்பு object_id start_oid;
	கட்டமைப்பு object_id end_oid;
	மாறிலி எழுத்து *end;
	மாறிலி எழுத்து *start;
	எழுத்து *to_free;
	முழு symmetric;
	நிலையான மாறிலி எழுத்து head_by_default[] = "HEAD";

	எனில் (!(dotdot = strstr(arg, "..")))
		திரும்பு 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	எனில் (!*end)
		end = head_by_default;
	எனில் (dotdot == arg)
		start = head_by_default;

	எனில் (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		விடுவி(to_free);
		திரும்பு 0;
	}

	எனில் (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		எனில் (symmetric) {
			கட்டமைப்பு commit_list *exclude = வெற்று;
			கட்டமைப்பு commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			எனில் (!a || !b) {
				விடுவி(to_free);
				திரும்பு 0;
			}
			எனில் (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			போது (exclude) {
				கட்டமைப்பு commit *commit = எடு_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, வெற்று);
			}
		}
		விடுவி(to_free);
		திரும்பு 1;
	}
	விடுவி(to_free);
	திரும்பு 0;
}

நிலையான முழு try_parent_shorthands(மாறிலி எழுத்து *arg)
{
	மாறிலி எழுத்து *mark;
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு commit *commit;
	கட்டமைப்பு commit_list *parents;
	முழு parent_எண்;
	முழு include_rev = 0;
	முழு include_parents = 0;
	முழு exclude_parent = 0;
	எழுத்து *to_free;

	எனில் ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		எனில் (mark[2])
			திரும்பு 0;
	} இல்லையெனில் எனில் ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		எனில் (mark[2])
			திரும்பு 0;
	} இல்லையெனில் எனில் ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		எனில் (mark[2]) {
			எழுத்து *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			எனில் (*end != '\0' || !exclude_parent)
				திரும்பு 0;
		}
	} இல்லையெனில்
		திரும்பு 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	எனில் (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		விடுவி(to_free);
		திரும்பு 0;
	}

	எனில் (exclude_parent &&
	    exclude_parent > commit_list_மொத்தம்(commit->parents)) {
		விடுவி(to_free);
		திரும்பு 0;
	}

	எனில் (include_rev)
		show_rev(NORMAL, &oid, arg);
	க்கு (parents = commit->parents, parent_எண் = 1;
	     parents;
	     parents = parents->next, parent_எண்++) {
		எழுத்து *name = வெற்று;

		எனில் (exclude_parent && parent_எண் != exclude_parent)
			தொடர்;

		எனில் (symbolic)
			name = xstrfmt("%s^%d", arg, parent_எண்);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		விடுவி(name);
	}

	விடுவி(to_free);
	திரும்பு 1;
}

நிலையான முழு parseopt_dump(மாறிலி கட்டமைப்பு option *o, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு strbuf *parsed = o->மதிப்பு;
	எனில் (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	இல்லையெனில் எனில் (o->short_name && (o->long_name == வெற்று || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	இல்லையெனில்
		strbuf_addf(parsed, " --%s", o->long_name);
	எனில் (arg) {
		எனில் (!stuck_long)
			strbuf_addch(parsed, ' ');
		இல்லையெனில் எனில் (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *skipspaces(மாறிலி எழுத்து *s)
{
	போது (isspace(*s))
		s++;
	திரும்பு s;
}

நிலையான எழுத்து *findspace(மாறிலி எழுத்து *s)
{
	க்கு (; *s; s++)
		எனில் (isspace(*s))
			திரும்பு (எழுத்து*)s;
	திரும்பு வெற்று;
}

நிலையான முழு cmd_parseopt(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix)
{
	முழு keep_dashdash = 0, stop_at_non_option = 0;
	எழுத்து மாறிலி * மாறிலி parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		வெற்று
	};
	கட்டமைப்பு option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_END(),
	};
	கட்டமைப்பு strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	கட்டமைப்பு strvec longnames = STRVEC_INIT;
	கட்டமைப்பு strvec usage = STRVEC_INIT;
	கட்டமைப்பு option *opts = வெற்று;
	size_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	எனில் (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	க்கு (;;) {
		strbuf_reset(&sb);
		எனில் (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		எனில் (!strcmp("--", sb.buf)) {
			எனில் (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			நிறுத்து;
		}

		strvec_தள்ளு(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	போது (strbuf_getline(&sb, stdin) != EOF) {
		மாறிலி எழுத்து *s;
		எழுத்து *help;
		கட்டமைப்பு option *o;

		எனில் (!sb.len)
			தொடர்;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, அளவு(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		எனில் (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			தொடர்;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->மதிப்பு = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		எனில் (!s)
			s = help;

		எனில் (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		எனில் (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} இல்லையெனில் எனில் (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (முழு)(s - sb.buf), sb.buf);
		} இல்லையெனில் {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (முழு)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		போது (s < help) {
			தேர்வு (*s++) {
			நிலை '=':
				o->flags &= ~PARSE_OPT_NOARG;
				தொடர்;
			நிலை '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				தொடர்;
			நிலை '!':
				o->flags |= PARSE_OPT_NONEG;
				தொடர்;
			நிலை '*':
				o->flags |= PARSE_OPT_HIDDEN;
				தொடர்;
			}
			s--;
			நிறுத்து;
		}

		எனில் (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, அளவு(*opts));
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
	க்கு (size_t i = 0; i < opts_nr; i++) {
		விடுவி((எழுத்து *) opts[i].help);
		விடுவி((எழுத்து *) opts[i].argh);
	}
	விடுவி(opts);
	திரும்பு 0;
}

நிலையான முழு cmd_sq_quote(முழு argc, மாறிலி எழுத்து **argv)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;

	எனில் (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	திரும்பு 0;
}

நிலையான வெறுமை die_no_single_rev(முழு quiet)
{
	எனில் (quiet)
		exit(1);
	இல்லையெனில்
		die(_("Needed a single revision"));
}

நிலையான மாறிலி எழுத்து builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
நிலையான முழு opt_with_மதிப்பு(மாறிலி எழுத்து *arg, மாறிலி எழுத்து *opt, மாறிலி எழுத்து **மதிப்பு)
{
	எனில் (skip_prefix(arg, opt, &arg)) {
		எனில் (!*arg) {
			*மதிப்பு = வெற்று;
			திரும்பு 1;
		}
		எனில் (*arg++ == '=') {
			*மதிப்பு = arg;
			திரும்பு 1;
		}
	}
	திரும்பு 0;
}

நிலையான வெறுமை handle_ref_opt(மாறிலி எழுத்து *pattern, மாறிலி எழுத்து *prefix)
{
	எனில் (pattern) {
		கட்டமைப்பு refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, வெற்று, &opts);
	} இல்லையெனில் {
		கட்டமைப்பு refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = strlen(prefix),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, வெற்று, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

எண்ணிக்கை format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

எண்ணிக்கை default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

நிலையான வெறுமை print_path(மாறிலி எழுத்து *path, மாறிலி எழுத்து *prefix,
		       எண்ணிக்கை format_type format, எண்ணிக்கை default_type def)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	எண்ணிக்கை path_format fmt;

	எனில் (format == FORMAT_DEFAULT) {
		தேர்வு (def) {
		நிலை DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			நிறுத்து;
		நிலை DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			நிறுத்து;
		நிலை DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			நிறுத்து;
		நிலை DEFAULT_UNMODIFIED:
		இயல்பு:
			fmt = PATH_FORMAT_UNMODIFIED;
			நிறுத்து;
		}
	} இல்லையெனில் {
		தேர்வு (format) {
		நிலை FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			நிறுத்து;
		நிலை FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			நிறுத்து;
		இயல்பு:
			fmt = PATH_FORMAT_UNMODIFIED;
			நிறுத்து;
		}
	}

	format_path(&sb, path, prefix, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

முழு cmd_rev_parse(முழு argc,
		  மாறிலி எழுத்து **argv,
		  மாறிலி எழுத்து *prefix,
		  கட்டமைப்பு repository *repo UNUSED)
{
	முழு i, as_is = 0, verify = 0, quiet = 0, revs_மொத்தம் = 0, type = 0;
	மாறிலி கட்டமைப்பு git_hash_algo *output_algo = வெற்று;
	மாறிலி கட்டமைப்பு git_hash_algo *compat = வெற்று;
	முழு did_repo_setup = 0;
	முழு has_dashdash = 0;
	முழு output_prefix = 0;
	கட்டமைப்பு object_id oid;
	unsigned முழு flags = 0;
	மாறிலி எழுத்து *name = வெற்று;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	முழு seen_end_of_options = 0;
	எண்ணிக்கை format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	எனில் (argc > 1 && !strcmp("--parseopt", argv[1]))
		திரும்பு cmd_parseopt(argc - 1, argv + 1, prefix);

	எனில் (argc > 1 && !strcmp("--sq-quote", argv[1]))
		திரும்பு cmd_sq_quote(argc - 2, argv + 2);

	எனில் (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	க்கு (i = 1; i < argc; i++) {
		எனில் (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			நிறுத்து;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	எனில் (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, வெற்று);
		திரும்பு 0;
	}

	க்கு (i = 1; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];

		எனில் (as_is) {
			எனில் (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			தொடர்;
		}

		எனில் (!seen_end_of_options) {
			எனில் (!strcmp(arg, "--local-env-vars")) {
				முழு i;
				க்கு (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				தொடர்;
			}
			எனில் (!strcmp(arg, "--resolve-git-dir")) {
				மாறிலி எழுத்து *gitdir = argv[++i];
				எனில் (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				எனில் (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				தொடர்;
			}
		}

		/* The rest of the options require a git repository. */
		எனில் (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, வெற்று);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		எனில் (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			எனில் (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			தொடர்;
		}

		எனில் (!seen_end_of_options && *arg == '-') {
			எனில் (!strcmp(arg, "--git-path")) {
				எனில் (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				தொடர்;
			}
			எனில் (!strcmp(arg,"-n")) {
				எனில் (++i >= argc)
					die(_("-n requires an argument"));
				எனில் ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				தொடர்;
			}
			எனில் (starts_with(arg, "-n")) {
				எனில் ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				தொடர்;
			}
			எனில் (opt_with_மதிப்பு(arg, "--path-format", &arg)) {
				எனில் (!arg)
					die(_("--path-format requires an argument"));
				எனில் (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} இல்லையெனில் எனில் (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} இல்லையெனில் {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				தொடர்;
			}
			எனில் (!strcmp(arg, "--default")) {
				def = argv[++i];
				எனில் (!def)
					die(_("--default requires an argument"));
				தொடர்;
			}
			எனில் (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				எனில் (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				தொடர்;
			}
			எனில் (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				தொடர்;
			}
			எனில் (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				தொடர்;
			}
			எனில் (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				தொடர்;
			}
			எனில் (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				தொடர்;
			}
			எனில் (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				தொடர்;
			}
			எனில் (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				தொடர்;
			}
			எனில் (opt_with_மதிப்பு(arg, "--output-object-format", &arg)) {
				எனில் (!arg)
					die(_("no object format specified"));
				எனில் (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					தொடர்;
				}
				இல்லையெனில் எனில் (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					தொடர்;
				}
				இல்லையெனில் die(_("unsupported object format: %s"), arg);
			}
			எனில் (opt_with_மதிப்பு(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				எனில் (!arg)
					தொடர்;
				abbrev = strtoul(arg, வெற்று, 10);
				எனில் (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				இல்லையெனில் எனில் ((முழு)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				தொடர்;
			}
			எனில் (!strcmp(arg, "--sq")) {
				output_sq = 1;
				தொடர்;
			}
			எனில் (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				தொடர்;
			}
			எனில் (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				தொடர்;
			}
			எனில் (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				தொடர்;
			}
			எனில் (opt_with_மதிப்பு(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				எனில் (arg) {
					எனில் (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					இல்லையெனில் எனில் (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					இல்லையெனில்
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				தொடர்;
			}
			எனில் (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, வெற்று);
				clear_ref_exclusions(&ref_excludes);
				தொடர்;
			}
			எனில் (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, வெற்று);
				தொடர்;
			}
			எனில் (!strcmp(arg, "--bisect")) {
				எழுத்து *prefix;
				எழுத்து *term_bad = வெற்று;
				எழுத்து *term_good = வெற்று;
				கட்டமைப்பு refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&term_bad, &term_good);
				prefix = xstrfmt("refs/bisect/%s", term_bad);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, வெற்று, &opts);
				விடுவி(prefix);
				prefix = xstrfmt("refs/bisect/%s", term_good);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, வெற்று, &opts);
				விடுவி(prefix);
				விடுவி(term_good);
				விடுவி(term_bad);
				தொடர்;
			}
			எனில் (opt_with_மதிப்பு(arg, "--branches", &arg)) {
				எனில் (ref_excludes.hidden_refs_configured)
					திரும்பு error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				தொடர்;
			}
			எனில் (opt_with_மதிப்பு(arg, "--tags", &arg)) {
				எனில் (ref_excludes.hidden_refs_configured)
					திரும்பு error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				தொடர்;
			}
			எனில் (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, வெற்று);
				தொடர்;
			}
			எனில் (opt_with_மதிப்பு(arg, "--remotes", &arg)) {
				எனில் (ref_excludes.hidden_refs_configured)
					திரும்பு error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				தொடர்;
			}
			எனில் (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				தொடர்;
			}
			எனில் (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				தொடர்;
			}
			எனில் (!strcmp(arg, "--show-toplevel")) {
				மாறிலி எழுத்து *work_மரம் = repo_get_work_மரம்(the_repository);
				எனில் (work_மரம்)
					print_path(work_மரம், prefix, format, DEFAULT_UNMODIFIED);
				இல்லையெனில்
					die(_("this operation must be run in a work tree"));
				தொடர்;
			}
			எனில் (!strcmp(arg, "--show-superproject-working-tree")) {
				கட்டமைப்பு strbuf superproject = STRBUF_INIT;
				எனில் (get_superproject_working_மரம்(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				தொடர்;
			}
			எனில் (!strcmp(arg, "--show-prefix")) {
				எனில் (prefix)
					puts(prefix);
				இல்லையெனில்
					putchar('\n');
				தொடர்;
			}
			எனில் (!strcmp(arg, "--show-cdup")) {
				மாறிலி எழுத்து *pfx = prefix;
				எனில் (!is_inside_work_மரம்(the_repository)) {
					மாறிலி எழுத்து *work_மரம் =
						repo_get_work_மரம்(the_repository);
					எனில் (work_மரம்)
						printf("%s\n", work_மரம்);
					தொடர்;
				}
				போது (pfx) {
					pfx = strchr(pfx, '/');
					எனில் (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				தொடர்;
			}
			எனில் (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				மாறிலி எழுத்து *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				எழுத்து *cwd;
				முழு len;
				எண்ணிக்கை format_type wanted = format;
				எனில் (arg[2] == 'g') {	/* --git-dir */
					எனில் (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						தொடர்;
					}
					எனில் (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						தொடர்;
					}
				} இல்லையெனில் {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					எனில் (!gitdir && !prefix)
						gitdir = ".git";
					எனில் (gitdir) {
						கட்டமைப்பு strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						தொடர்;
					}
				}
				cwd = xgetcwd();
				len = strlen(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				விடுவி(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				தொடர்;
			}
			எனில் (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				தொடர்;
			}
			எனில் (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				தொடர்;
			}
			எனில் (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_மரம்(the_repository) ? "true"
						: "false");
				தொடர்;
			}
			எனில் (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				தொடர்;
			}
			எனில் (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				தொடர்;
			}
			எனில் (!strcmp(arg, "--shared-index-path")) {
				எனில் (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				எனில் (the_repository->index->split_index) {
					மாறிலி கட்டமைப்பு object_id *oid = &the_repository->index->split_index->base_oid;
					மாறிலி எழுத்து *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				தொடர்;
			}
			எனில் (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				தொடர்;
			}
			எனில் (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				தொடர்;
			}
			எனில் (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				தொடர்;
			}
			எனில் (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				தொடர்;
			}
			எனில் (opt_with_மதிப்பு(arg, "--show-object-format", &arg)) {
				மாறிலி எழுத்து *val = arg ? arg : "storage";

				எனில் (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				எனில் (!strcmp(val, "compat")) {
					எனில் (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					இல்லையெனில்
						putchar('\n');
				} இல்லையெனில் {
					puts(the_hash_algo->name);
				}
				தொடர்;
			}
			எனில் (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				தொடர்;
			}
			எனில் (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				எனில் (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				தொடர்;
			}
			எனில் (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			தொடர்;
		}

		/* Not a flag argument */
		எனில் (try_difference(arg))
			தொடர்;
		எனில் (try_parent_shorthands(arg))
			தொடர்;
		name = arg;
		type = NORMAL;
		எனில் (*arg == '^') {
			name++;
			type = REVERSED;
		}
		எனில் (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			எனில் (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			எனில் (verify)
				revs_மொத்தம்++;
			இல்லையெனில்
				show_rev(type, &oid, name);
			தொடர்;
		}
		எனில் (verify)
			die_no_single_rev(quiet);
		எனில் (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		எனில் (!show_file(arg, output_prefix))
			தொடர்;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	எனில் (verify) {
		எனில் (revs_மொத்தம் == 1) {
			show_rev(type, &oid, name);
			திரும்பு 0;
		} இல்லையெனில் எனில் (revs_மொத்தம் == 0 && show_default())
			திரும்பு 0;
		die_no_single_rev(quiet);
	} இல்லையெனில்
		show_default();
	திரும்பு 0;
}
