/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"

#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "bisect.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "repo-settings.h"
#ଅନ୍ତର୍ଭାଵ "repository.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "split-index.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"
#ଅନ୍ତର୍ଭାଵ "shallow.h"
#ଅନ୍ତର୍ଭାଵ "object-file-convert.h"

#define DO_REVS		1
#define DO_NOREV	2
#define DO_FLAGS	4
#define DO_NONFLAGS	8
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା filter = ~0;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *def;

#define NORMAL 0
#define REVERSED 1
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା symbolic;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା abbrev;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା abbrev_ref;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା abbrev_ref_strict;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_sq;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stuck_long;
ସ୍ଥିର ସଂରଚନା ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_rev_argument(ଅଚଳ ଅକ୍ଷର *arg)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *rev_args[] = {
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
		ଖାଲି
	};
	ଅଚଳ ଅକ୍ଷର **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	ଯଦି ((*arg == '-') && isdigit(arg[1]))
		ଫେରନ୍ତୁ 1;

	ପାଇଁ (;;) {
		ଅଚଳ ଅକ୍ଷର *str = *p++;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
		ଯଦି (!str)
			ଫେରନ୍ତୁ 0;
		len = strlen(str);
		ଯଦି (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			ଫେରନ୍ତୁ 1;
	}
}

/* Output argument as a string, either SQ or normal */
ସ୍ଥିର ଶୂନ୍ୟ show(ଅଚଳ ଅକ୍ଷର *arg)
{
	ଯଦି (output_sq) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା sq = '\'', ch;

		putchar(sq);
		ଯେତେବେଳେ ((ch = *arg++)) {
			ଯଦି (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	ଅନ୍ୟଥା
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
ସ୍ଥିର ଶୂନ୍ୟ show_with_type(ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ଅଚଳ ଅକ୍ଷର *arg)
{
	ଯଦି (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
ସ୍ଥିର ଶୂନ୍ୟ show_rev(ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ଅଚଳ ସଂରଚନା object_id *oid, ଅଚଳ ଅକ୍ଷର *name)
{
	ଯଦି (!(filter & DO_REVS))
		ଫେରନ୍ତୁ;
	def = ଖାଲି;

	ଯଦି ((symbolic || abbrev_ref) && name) {
		ଯଦି (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			ସଂରଚନା object_id discard;
			ଅକ୍ଷର *full;

			ବାଛ (repo_dwim_ref(the_repository, name,
					      strlen(name), &discard, &full,
					      0)) {
			ସ୍ଥିତି 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				ଭାଙ୍ଗ;
			ସ୍ଥିତି 1: /* happy */
				ଯଦି (abbrev_ref) {
					ଅକ୍ଷର *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					ମୁକ୍ତ(old);
				}
				show_with_type(type, full);
				ଭାଙ୍ଗ;
			ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				ଭାଙ୍ଗ;
			}
			ମୁକ୍ତ(full);
		} ଅନ୍ୟଥା {
			show_with_type(type, name);
		}
	}
	ଅନ୍ୟଥା ଯଦି (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	ଅନ୍ୟଥା
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_flag(ଅଚଳ ଅକ୍ଷର *arg)
{
	ଯଦି (!(filter & DO_FLAGS))
		ଫେରନ୍ତୁ 0;
	ଯଦି (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		ଫେରନ୍ତୁ 1;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_default(ଶୂନ୍ୟ)
{
	ଅଚଳ ଅକ୍ଷର *s = def;

	ଯଦି (s) {
		ସଂରଚନା object_id oid;

		def = ଖାଲି;
		ଯଦି (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			ଫେରନ୍ତୁ 1;
		}
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_reference(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data UNUSED)
{
	ଯଦି (ref_excluded(&ref_excludes, ref->name))
		ଫେରନ୍ତୁ 0;
	show_rev(NORMAL, ref->oid, ref->name);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା anti_reference(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_abbrev(ଅଚଳ ସଂରଚନା object_id *oid, ଶୂନ୍ୟ *cb_data UNUSED)
{
	show_rev(NORMAL, oid, ଖାଲି);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ show_datestring(ଅଚଳ ଅକ୍ଷର *flag, ଅଚଳ ଅକ୍ଷର *datestr)
{
	ଅକ୍ଷର *buffer;

	/* date handling requires both flags and revs */
	ଯଦି ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		ଫେରନ୍ତୁ;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	ମୁକ୍ତ(buffer);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_file(ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_prefix)
{
	show_default();
	ଯଦି ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		ଯଦି (output_prefix) {
			ଅଚଳ ଅକ୍ଷର *prefix = the_repository->prefix;
			ଅକ୍ଷର *fname = prefix_filename(prefix, arg);
			show(fname);
			ମୁକ୍ତ(fname);
		} ଅନ୍ୟଥା
			show(arg);
		ଫେରନ୍ତୁ 1;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_difference(ଅଚଳ ଅକ୍ଷର *arg)
{
	ଅଚଳ ଅକ୍ଷର *dotdot;
	ସଂରଚନା object_id start_oid;
	ସଂରଚନା object_id end_oid;
	ଅଚଳ ଅକ୍ଷର *end;
	ଅଚଳ ଅକ୍ଷର *start;
	ଅକ୍ଷର *to_free;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା symmetric;
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର head_by_default[] = "HEAD";

	ଯଦି (!(dotdot = strstr(arg, "..")))
		ଫେରନ୍ତୁ 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	ଯଦି (!*end)
		end = head_by_default;
	ଯଦି (dotdot == arg)
		start = head_by_default;

	ଯଦି (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		ମୁକ୍ତ(to_free);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		ଯଦି (symmetric) {
			ସଂରଚନା commit_list *exclude = ଖାଲି;
			ସଂରଚନା commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			ଯଦି (!a || !b) {
				ମୁକ୍ତ(to_free);
				ଫେରନ୍ତୁ 0;
			}
			ଯଦି (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			ଯେତେବେଳେ (exclude) {
				ସଂରଚନା commit *commit = କାଢ଼_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, ଖାଲି);
			}
		}
		ମୁକ୍ତ(to_free);
		ଫେରନ୍ତୁ 1;
	}
	ମୁକ୍ତ(to_free);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_parent_shorthands(ଅଚଳ ଅକ୍ଷର *arg)
{
	ଅଚଳ ଅକ୍ଷର *mark;
	ସଂରଚନା object_id oid;
	ସଂରଚନା commit *commit;
	ସଂରଚନା commit_list *parents;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent_ସଂଖ୍ୟା;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_rev = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_parents = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude_parent = 0;
	ଅକ୍ଷର *to_free;

	ଯଦି ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		ଯଦି (mark[2])
			ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		ଯଦି (mark[2])
			ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		ଯଦି (mark[2]) {
			ଅକ୍ଷର *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			ଯଦି (*end != '\0' || !exclude_parent)
				ଫେରନ୍ତୁ 0;
		}
	} ଅନ୍ୟଥା
		ଫେରନ୍ତୁ 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	ଯଦି (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		ମୁକ୍ତ(to_free);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (exclude_parent &&
	    exclude_parent > commit_list_ଗଣନା(commit->parents)) {
		ମୁକ୍ତ(to_free);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (include_rev)
		show_rev(NORMAL, &oid, arg);
	ପାଇଁ (parents = commit->parents, parent_ସଂଖ୍ୟା = 1;
	     parents;
	     parents = parents->next, parent_ସଂଖ୍ୟା++) {
		ଅକ୍ଷର *name = ଖାଲି;

		ଯଦି (exclude_parent && parent_ସଂଖ୍ୟା != exclude_parent)
			ଜାରି;

		ଯଦି (symbolic)
			name = xstrfmt("%s^%d", arg, parent_ସଂଖ୍ୟା);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		ମୁକ୍ତ(name);
	}

	ମୁକ୍ତ(to_free);
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parseopt_dump(ଅଚଳ ସଂରଚନା option *o, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା strbuf *parsed = o->ମୂଲ୍ୟ;
	ଯଦି (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	ଅନ୍ୟଥା ଯଦି (o->short_name && (o->long_name == ଖାଲି || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	ଅନ୍ୟଥା
		strbuf_addf(parsed, " --%s", o->long_name);
	ଯଦି (arg) {
		ଯଦି (!stuck_long)
			strbuf_addch(parsed, ' ');
		ଅନ୍ୟଥା ଯଦି (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *skipspaces(ଅଚଳ ଅକ୍ଷର *s)
{
	ଯେତେବେଳେ (isspace(*s))
		s++;
	ଫେରନ୍ତୁ s;
}

ସ୍ଥିର ଅକ୍ଷର *findspace(ଅଚଳ ଅକ୍ଷର *s)
{
	ପାଇଁ (; *s; s++)
		ଯଦି (isspace(*s))
			ଫେରନ୍ତୁ (ଅକ୍ଷର*)s;
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_parseopt(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_dashdash = 0, stop_at_non_option = 0;
	ଅକ୍ଷର ଅଚଳ * ଅଚଳ parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		ଖାଲି
	};
	ସଂରଚନା option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_END(),
	};
	ସଂରଚନା strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	ସଂରଚନା strvec longnames = STRVEC_INIT;
	ସଂରଚନା strvec usage = STRVEC_INIT;
	ସଂରଚନା option *opts = ଖାଲି;
	size_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	ଯଦି (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	ପାଇଁ (;;) {
		strbuf_reset(&sb);
		ଯଦି (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		ଯଦି (!strcmp("--", sb.buf)) {
			ଯଦି (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			ଭାଙ୍ଗ;
		}

		strvec_ଠେଲ(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	ଯେତେବେଳେ (strbuf_getline(&sb, stdin) != EOF) {
		ଅଚଳ ଅକ୍ଷର *s;
		ଅକ୍ଷର *help;
		ସଂରଚନା option *o;

		ଯଦି (!sb.len)
			ଜାରି;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, ଆକାର(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		ଯଦି (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			ଜାରି;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->ମୂଲ୍ୟ = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		ଯଦି (!s)
			s = help;

		ଯଦି (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		ଯଦି (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} ଅନ୍ୟଥା ଯଦି (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(s - sb.buf), sb.buf);
		} ଅନ୍ୟଥା {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		ଯେତେବେଳେ (s < help) {
			ବାଛ (*s++) {
			ସ୍ଥିତି '=':
				o->flags &= ~PARSE_OPT_NOARG;
				ଜାରି;
			ସ୍ଥିତି '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				ଜାରି;
			ସ୍ଥିତି '!':
				o->flags |= PARSE_OPT_NONEG;
				ଜାରି;
			ସ୍ଥିତି '*':
				o->flags |= PARSE_OPT_HIDDEN;
				ଜାରି;
			}
			s--;
			ଭାଙ୍ଗ;
		}

		ଯଦି (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, ଆକାର(*opts));
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
	ପାଇଁ (size_t i = 0; i < opts_nr; i++) {
		ମୁକ୍ତ((ଅକ୍ଷର *) opts[i].help);
		ମୁକ୍ତ((ଅକ୍ଷର *) opts[i].argh);
	}
	ମୁକ୍ତ(opts);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_sq_quote(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;

	ଯଦି (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ die_no_single_rev(ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet)
{
	ଯଦି (quiet)
		exit(1);
	ଅନ୍ୟଥା
		die(_("Needed a single revision"));
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_with_ମୂଲ୍ୟ(ଅଚଳ ଅକ୍ଷର *arg, ଅଚଳ ଅକ୍ଷର *opt, ଅଚଳ ଅକ୍ଷର **ମୂଲ୍ୟ)
{
	ଯଦି (skip_prefix(arg, opt, &arg)) {
		ଯଦି (!*arg) {
			*ମୂଲ୍ୟ = ଖାଲି;
			ଫେରନ୍ତୁ 1;
		}
		ଯଦି (*arg++ == '=') {
			*ମୂଲ୍ୟ = arg;
			ଫେରନ୍ତୁ 1;
		}
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ handle_ref_opt(ଅଚଳ ଅକ୍ଷର *pattern, ଅଚଳ ଅକ୍ଷର *prefix)
{
	ଯଦି (pattern) {
		ସଂରଚନା refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ଖାଲି, &opts);
	} ଅନ୍ୟଥା {
		ସଂରଚନା refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = strlen(prefix),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ଖାଲି, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

ଗଣନା format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

ଗଣନା default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

ସ୍ଥିର ଶୂନ୍ୟ print_path(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *prefix,
		       ଗଣନା format_type format, ଗଣନା default_type def)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଗଣନା path_format fmt;

	ଯଦି (format == FORMAT_DEFAULT) {
		ବାଛ (def) {
		ସ୍ଥିତି DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି DEFAULT_UNMODIFIED:
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			fmt = PATH_FORMAT_UNMODIFIED;
			ଭାଙ୍ଗ;
		}
	} ଅନ୍ୟଥା {
		ବାଛ (format) {
		ସ୍ଥିତି FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			fmt = PATH_FORMAT_UNMODIFIED;
			ଭାଙ୍ଗ;
		}
	}

	format_path(&sb, path, prefix, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_rev_parse(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		  ଅଚଳ ଅକ୍ଷର **argv,
		  ଅଚଳ ଅକ୍ଷର *prefix,
		  ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, as_is = 0, verify = 0, quiet = 0, revs_ଗଣନା = 0, type = 0;
	ଅଚଳ ସଂରଚନା git_hash_algo *output_algo = ଖାଲି;
	ଅଚଳ ସଂରଚନା git_hash_algo *compat = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା did_repo_setup = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_dashdash = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_prefix = 0;
	ସଂରଚନା object_id oid;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;
	ଅଚଳ ଅକ୍ଷର *name = ଖାଲି;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା seen_end_of_options = 0;
	ଗଣନା format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	ଯଦି (argc > 1 && !strcmp("--parseopt", argv[1]))
		ଫେରନ୍ତୁ cmd_parseopt(argc - 1, argv + 1, prefix);

	ଯଦି (argc > 1 && !strcmp("--sq-quote", argv[1]))
		ଫେରନ୍ତୁ cmd_sq_quote(argc - 2, argv + 2);

	ଯଦି (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	ପାଇଁ (i = 1; i < argc; i++) {
		ଯଦି (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			ଭାଙ୍ଗ;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	ଯଦି (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, ଖାଲି);
		ଫେରନ୍ତୁ 0;
	}

	ପାଇଁ (i = 1; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];

		ଯଦି (as_is) {
			ଯଦି (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			ଜାରି;
		}

		ଯଦି (!seen_end_of_options) {
			ଯଦି (!strcmp(arg, "--local-env-vars")) {
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
				ପାଇଁ (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--resolve-git-dir")) {
				ଅଚଳ ଅକ୍ଷର *gitdir = argv[++i];
				ଯଦି (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				ଯଦି (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				ଜାରି;
			}
		}

		/* The rest of the options require a git repository. */
		ଯଦି (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, ଖାଲି);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		ଯଦି (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			ଯଦି (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			ଜାରି;
		}

		ଯଦି (!seen_end_of_options && *arg == '-') {
			ଯଦି (!strcmp(arg, "--git-path")) {
				ଯଦି (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg,"-n")) {
				ଯଦି (++i >= argc)
					die(_("-n requires an argument"));
				ଯଦି ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				ଜାରି;
			}
			ଯଦି (starts_with(arg, "-n")) {
				ଯଦି ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				ଜାରି;
			}
			ଯଦି (opt_with_ମୂଲ୍ୟ(arg, "--path-format", &arg)) {
				ଯଦି (!arg)
					die(_("--path-format requires an argument"));
				ଯଦି (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} ଅନ୍ୟଥା {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--default")) {
				def = argv[++i];
				ଯଦି (!def)
					die(_("--default requires an argument"));
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				ଯଦି (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				ଜାରି;
			}
			ଯଦି (opt_with_ମୂଲ୍ୟ(arg, "--output-object-format", &arg)) {
				ଯଦି (!arg)
					die(_("no object format specified"));
				ଯଦି (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					ଜାରି;
				}
				ଅନ୍ୟଥା ଯଦି (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					ଜାରି;
				}
				ଅନ୍ୟଥା die(_("unsupported object format: %s"), arg);
			}
			ଯଦି (opt_with_ମୂଲ୍ୟ(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				ଯଦି (!arg)
					ଜାରି;
				abbrev = strtoul(arg, ଖାଲି, 10);
				ଯଦି (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				ଅନ୍ୟଥା ଯଦି ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--sq")) {
				output_sq = 1;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				ଜାରି;
			}
			ଯଦି (opt_with_ମୂଲ୍ୟ(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				ଯଦି (arg) {
					ଯଦି (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					ଅନ୍ୟଥା
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, ଖାଲି);
				clear_ref_exclusions(&ref_excludes);
				ଜାରି;
			}
			ଯଦି (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, ଖାଲି);
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--bisect")) {
				ଅକ୍ଷର *prefix;
				ଅକ୍ଷର *term_bad = ଖାଲି;
				ଅକ୍ଷର *term_good = ଖାଲି;
				ସଂରଚନା refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&term_bad, &term_good);
				prefix = xstrfmt("refs/bisect/%s", term_bad);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, ଖାଲି, &opts);
				ମୁକ୍ତ(prefix);
				prefix = xstrfmt("refs/bisect/%s", term_good);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, ଖାଲି, &opts);
				ମୁକ୍ତ(prefix);
				ମୁକ୍ତ(term_good);
				ମୁକ୍ତ(term_bad);
				ଜାରି;
			}
			ଯଦି (opt_with_ମୂଲ୍ୟ(arg, "--branches", &arg)) {
				ଯଦି (ref_excludes.hidden_refs_configured)
					ଫେରନ୍ତୁ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				ଜାରି;
			}
			ଯଦି (opt_with_ମୂଲ୍ୟ(arg, "--tags", &arg)) {
				ଯଦି (ref_excludes.hidden_refs_configured)
					ଫେରନ୍ତୁ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				ଜାରି;
			}
			ଯଦି (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, ଖାଲି);
				ଜାରି;
			}
			ଯଦି (opt_with_ମୂଲ୍ୟ(arg, "--remotes", &arg)) {
				ଯଦି (ref_excludes.hidden_refs_configured)
					ଫେରନ୍ତୁ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				ଜାରି;
			}
			ଯଦି (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				ଜାରି;
			}
			ଯଦି (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--show-toplevel")) {
				ଅଚଳ ଅକ୍ଷର *work_ବୃକ୍ଷ = repo_get_work_ବୃକ୍ଷ(the_repository);
				ଯଦି (work_ବୃକ୍ଷ)
					print_path(work_ବୃକ୍ଷ, prefix, format, DEFAULT_UNMODIFIED);
				ଅନ୍ୟଥା
					die(_("this operation must be run in a work tree"));
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--show-superproject-working-tree")) {
				ସଂରଚନା strbuf superproject = STRBUF_INIT;
				ଯଦି (get_superproject_working_ବୃକ୍ଷ(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--show-prefix")) {
				ଯଦି (prefix)
					puts(prefix);
				ଅନ୍ୟଥା
					putchar('\n');
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--show-cdup")) {
				ଅଚଳ ଅକ୍ଷର *pfx = prefix;
				ଯଦି (!is_inside_work_ବୃକ୍ଷ(the_repository)) {
					ଅଚଳ ଅକ୍ଷର *work_ବୃକ୍ଷ =
						repo_get_work_ବୃକ୍ଷ(the_repository);
					ଯଦି (work_ବୃକ୍ଷ)
						printf("%s\n", work_ବୃକ୍ଷ);
					ଜାରି;
				}
				ଯେତେବେଳେ (pfx) {
					pfx = strchr(pfx, '/');
					ଯଦି (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				ଅଚଳ ଅକ୍ଷର *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				ଅକ୍ଷର *cwd;
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
				ଗଣନା format_type wanted = format;
				ଯଦି (arg[2] == 'g') {	/* --git-dir */
					ଯଦି (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						ଜାରି;
					}
					ଯଦି (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						ଜାରି;
					}
				} ଅନ୍ୟଥା {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					ଯଦି (!gitdir && !prefix)
						gitdir = ".git";
					ଯଦି (gitdir) {
						ସଂରଚନା strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						ଜାରି;
					}
				}
				cwd = xgetcwd();
				len = strlen(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				ମୁକ୍ତ(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_ବୃକ୍ଷ(the_repository) ? "true"
						: "false");
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--shared-index-path")) {
				ଯଦି (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				ଯଦି (the_repository->index->split_index) {
					ଅଚଳ ସଂରଚନା object_id *oid = &the_repository->index->split_index->base_oid;
					ଅଚଳ ଅକ୍ଷର *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				ଜାରି;
			}
			ଯଦି (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				ଜାରି;
			}
			ଯଦି (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				ଜାରି;
			}
			ଯଦି (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				ଜାରି;
			}
			ଯଦି (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				ଜାରି;
			}
			ଯଦି (opt_with_ମୂଲ୍ୟ(arg, "--show-object-format", &arg)) {
				ଅଚଳ ଅକ୍ଷର *val = arg ? arg : "storage";

				ଯଦି (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				ଯଦି (!strcmp(val, "compat")) {
					ଯଦି (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					ଅନ୍ୟଥା
						putchar('\n');
				} ଅନ୍ୟଥା {
					puts(the_hash_algo->name);
				}
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				ଜାରି;
			}
			ଯଦି (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				ଯଦି (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				ଜାରି;
			}
			ଯଦି (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			ଜାରି;
		}

		/* Not a flag argument */
		ଯଦି (try_difference(arg))
			ଜାରି;
		ଯଦି (try_parent_shorthands(arg))
			ଜାରି;
		name = arg;
		type = NORMAL;
		ଯଦି (*arg == '^') {
			name++;
			type = REVERSED;
		}
		ଯଦି (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			ଯଦି (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			ଯଦି (verify)
				revs_ଗଣନା++;
			ଅନ୍ୟଥା
				show_rev(type, &oid, name);
			ଜାରି;
		}
		ଯଦି (verify)
			die_no_single_rev(quiet);
		ଯଦି (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		ଯଦି (!show_file(arg, output_prefix))
			ଜାରି;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	ଯଦି (verify) {
		ଯଦି (revs_ଗଣନା == 1) {
			show_rev(type, &oid, name);
			ଫେରନ୍ତୁ 0;
		} ଅନ୍ୟଥା ଯଦି (revs_ଗଣନା == 0 && show_default())
			ଫେରନ୍ତୁ 0;
		die_no_single_rev(quiet);
	} ଅନ୍ୟଥା
		show_default();
	ଫେରନ୍ତୁ 0;
}
