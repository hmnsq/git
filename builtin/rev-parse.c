/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"

#अन्तर्भाव "abspath.h"
#अन्तर्भाव "bisect.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "repo-settings.h"
#अन्तर्भाव "repository.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "split-index.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "shallow.h"
#अन्तर्भाव "object-file-convert.h"

#define DO_REVS		1
#define DO_NOREV	2
#define DO_FLAGS	4
#define DO_NONFLAGS	8
स्थिर पूर्णाङ्क filter = ~0;

स्थिर ध्रुव वर्ण *def;

#define NORMAL 0
#define REVERSED 1
स्थिर पूर्णाङ्क show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
स्थिर पूर्णाङ्क symbolic;
स्थिर पूर्णाङ्क abbrev;
स्थिर पूर्णाङ्क abbrev_ref;
स्थिर पूर्णाङ्क abbrev_ref_strict;
स्थिर पूर्णाङ्क output_sq;

स्थिर पूर्णाङ्क stuck_long;
स्थिर संरचना ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
स्थिर पूर्णाङ्क is_rev_argument(ध्रुव वर्ण *arg)
{
	स्थिर ध्रुव वर्ण *rev_args[] = {
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
		रिक्त
	};
	ध्रुव वर्ण **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	सचे ((*arg == '-') && isdigit(arg[1]))
		पटिनिवत्त 1;

	अत्थाय (;;) {
		ध्रुव वर्ण *str = *p++;
		पूर्णाङ्क len;
		सचे (!str)
			पटिनिवत्त 0;
		len = strlen(str);
		सचे (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			पटिनिवत्त 1;
	}
}

/* Output argument as a string, either SQ or normal */
स्थिर शून्य show(ध्रुव वर्ण *arg)
{
	सचे (output_sq) {
		पूर्णाङ्क sq = '\'', ch;

		putchar(sq);
		याव ((ch = *arg++)) {
			सचे (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	अञ्ञथा
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
स्थिर शून्य show_with_type(पूर्णाङ्क type, ध्रुव वर्ण *arg)
{
	सचे (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
स्थिर शून्य show_rev(पूर्णाङ्क type, ध्रुव संरचना object_id *oid, ध्रुव वर्ण *name)
{
	सचे (!(filter & DO_REVS))
		पटिनिवत्त;
	def = रिक्त;

	सचे ((symbolic || abbrev_ref) && name) {
		सचे (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			संरचना object_id discard;
			वर्ण *full;

			विकल्प (repo_dwim_ref(the_repository, name,
					      strlen(name), &discard, &full,
					      0)) {
			पक्ष 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				भङ्ग;
			पक्ष 1: /* happy */
				सचे (abbrev_ref) {
					वर्ण *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					मुक्त(old);
				}
				show_with_type(type, full);
				भङ्ग;
			स्वभाव: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				भङ्ग;
			}
			मुक्त(full);
		} अञ्ञथा {
			show_with_type(type, name);
		}
	}
	अञ्ञथा सचे (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	अञ्ञथा
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
स्थिर पूर्णाङ्क show_flag(ध्रुव वर्ण *arg)
{
	सचे (!(filter & DO_FLAGS))
		पटिनिवत्त 0;
	सचे (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		पटिनिवत्त 1;
	}
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क show_default(शून्य)
{
	ध्रुव वर्ण *s = def;

	सचे (s) {
		संरचना object_id oid;

		def = रिक्त;
		सचे (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			पटिनिवत्त 1;
		}
	}
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क show_reference(ध्रुव संरचना reference *ref, शून्य *cb_data UNUSED)
{
	सचे (ref_excluded(&ref_excludes, ref->name))
		पटिनिवत्त 0;
	show_rev(NORMAL, ref->oid, ref->name);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क anti_reference(ध्रुव संरचना reference *ref, शून्य *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क show_abbrev(ध्रुव संरचना object_id *oid, शून्य *cb_data UNUSED)
{
	show_rev(NORMAL, oid, रिक्त);
	पटिनिवत्त 0;
}

स्थिर शून्य show_datestring(ध्रुव वर्ण *flag, ध्रुव वर्ण *datestr)
{
	वर्ण *buffer;

	/* date handling requires both flags and revs */
	सचे ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		पटिनिवत्त;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	मुक्त(buffer);
}

स्थिर पूर्णाङ्क show_file(ध्रुव वर्ण *arg, पूर्णाङ्क output_prefix)
{
	show_default();
	सचे ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		सचे (output_prefix) {
			ध्रुव वर्ण *prefix = the_repository->prefix;
			वर्ण *fname = prefix_filename(prefix, arg);
			show(fname);
			मुक्त(fname);
		} अञ्ञथा
			show(arg);
		पटिनिवत्त 1;
	}
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क try_difference(ध्रुव वर्ण *arg)
{
	ध्रुव वर्ण *dotdot;
	संरचना object_id start_oid;
	संरचना object_id end_oid;
	ध्रुव वर्ण *end;
	ध्रुव वर्ण *start;
	वर्ण *to_free;
	पूर्णाङ्क symmetric;
	स्थिर ध्रुव वर्ण head_by_default[] = "HEAD";

	सचे (!(dotdot = strstr(arg, "..")))
		पटिनिवत्त 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	सचे (!*end)
		end = head_by_default;
	सचे (dotdot == arg)
		start = head_by_default;

	सचे (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		मुक्त(to_free);
		पटिनिवत्त 0;
	}

	सचे (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		सचे (symmetric) {
			संरचना commit_list *exclude = रिक्त;
			संरचना commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			सचे (!a || !b) {
				मुक्त(to_free);
				पटिनिवत्त 0;
			}
			सचे (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			याव (exclude) {
				संरचना commit *commit = निष्कासय_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, रिक्त);
			}
		}
		मुक्त(to_free);
		पटिनिवत्त 1;
	}
	मुक्त(to_free);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क try_parent_shorthands(ध्रुव वर्ण *arg)
{
	ध्रुव वर्ण *mark;
	संरचना object_id oid;
	संरचना commit *commit;
	संरचना commit_list *parents;
	पूर्णाङ्क parent_सङ्ख्या;
	पूर्णाङ्क include_rev = 0;
	पूर्णाङ्क include_parents = 0;
	पूर्णाङ्क exclude_parent = 0;
	वर्ण *to_free;

	सचे ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		सचे (mark[2])
			पटिनिवत्त 0;
	} अञ्ञथा सचे ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		सचे (mark[2])
			पटिनिवत्त 0;
	} अञ्ञथा सचे ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		सचे (mark[2]) {
			वर्ण *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			सचे (*end != '\0' || !exclude_parent)
				पटिनिवत्त 0;
		}
	} अञ्ञथा
		पटिनिवत्त 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	सचे (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		मुक्त(to_free);
		पटिनिवत्त 0;
	}

	सचे (exclude_parent &&
	    exclude_parent > commit_list_गणनम्(commit->parents)) {
		मुक्त(to_free);
		पटिनिवत्त 0;
	}

	सचे (include_rev)
		show_rev(NORMAL, &oid, arg);
	अत्थाय (parents = commit->parents, parent_सङ्ख्या = 1;
	     parents;
	     parents = parents->next, parent_सङ्ख्या++) {
		वर्ण *name = रिक्त;

		सचे (exclude_parent && parent_सङ्ख्या != exclude_parent)
			अनुवर्त;

		सचे (symbolic)
			name = xstrfmt("%s^%d", arg, parent_सङ्ख्या);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		मुक्त(name);
	}

	मुक्त(to_free);
	पटिनिवत्त 1;
}

स्थिर पूर्णाङ्क parseopt_dump(ध्रुव संरचना option *o, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना strbuf *parsed = o->मानम्;
	सचे (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	अञ्ञथा सचे (o->short_name && (o->long_name == रिक्त || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	अञ्ञथा
		strbuf_addf(parsed, " --%s", o->long_name);
	सचे (arg) {
		सचे (!stuck_long)
			strbuf_addch(parsed, ' ');
		अञ्ञथा सचे (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	पटिनिवत्त 0;
}

स्थिर ध्रुव वर्ण *skipspaces(ध्रुव वर्ण *s)
{
	याव (isspace(*s))
		s++;
	पटिनिवत्त s;
}

स्थिर वर्ण *findspace(ध्रुव वर्ण *s)
{
	अत्थाय (; *s; s++)
		सचे (isspace(*s))
			पटिनिवत्त (वर्ण*)s;
	पटिनिवत्त रिक्त;
}

स्थिर पूर्णाङ्क cmd_parseopt(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix)
{
	पूर्णाङ्क keep_dashdash = 0, stop_at_non_option = 0;
	वर्ण ध्रुव * ध्रुव parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		रिक्त
	};
	संरचना option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_END(),
	};
	संरचना strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	संरचना strvec longnames = STRVEC_INIT;
	संरचना strvec usage = STRVEC_INIT;
	संरचना option *opts = रिक्त;
	size_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	सचे (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	अत्थाय (;;) {
		strbuf_reset(&sb);
		सचे (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		सचे (!strcmp("--", sb.buf)) {
			सचे (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			भङ्ग;
		}

		strvec_प्रक्षिप(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	याव (strbuf_getline(&sb, stdin) != EOF) {
		ध्रुव वर्ण *s;
		वर्ण *help;
		संरचना option *o;

		सचे (!sb.len)
			अनुवर्त;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, परिमाण(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		सचे (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			अनुवर्त;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->मानम् = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		सचे (!s)
			s = help;

		सचे (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		सचे (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} अञ्ञथा सचे (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (पूर्णाङ्क)(s - sb.buf), sb.buf);
		} अञ्ञथा {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (पूर्णाङ्क)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		याव (s < help) {
			विकल्प (*s++) {
			पक्ष '=':
				o->flags &= ~PARSE_OPT_NOARG;
				अनुवर्त;
			पक्ष '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				अनुवर्त;
			पक्ष '!':
				o->flags |= PARSE_OPT_NONEG;
				अनुवर्त;
			पक्ष '*':
				o->flags |= PARSE_OPT_HIDDEN;
				अनुवर्त;
			}
			s--;
			भङ्ग;
		}

		सचे (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, परिमाण(*opts));
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
	अत्थाय (size_t i = 0; i < opts_nr; i++) {
		मुक्त((वर्ण *) opts[i].help);
		मुक्त((वर्ण *) opts[i].argh);
	}
	मुक्त(opts);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क cmd_sq_quote(पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	संरचना strbuf buf = STRBUF_INIT;

	सचे (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	पटिनिवत्त 0;
}

स्थिर शून्य die_no_single_rev(पूर्णाङ्क quiet)
{
	सचे (quiet)
		exit(1);
	अञ्ञथा
		die(_("Needed a single revision"));
}

स्थिर ध्रुव वर्ण builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
स्थिर पूर्णाङ्क opt_with_मानम्(ध्रुव वर्ण *arg, ध्रुव वर्ण *opt, ध्रुव वर्ण **मानम्)
{
	सचे (skip_prefix(arg, opt, &arg)) {
		सचे (!*arg) {
			*मानम् = रिक्त;
			पटिनिवत्त 1;
		}
		सचे (*arg++ == '=') {
			*मानम् = arg;
			पटिनिवत्त 1;
		}
	}
	पटिनिवत्त 0;
}

स्थिर शून्य handle_ref_opt(ध्रुव वर्ण *pattern, ध्रुव वर्ण *prefix)
{
	सचे (pattern) {
		संरचना refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, रिक्त, &opts);
	} अञ्ञथा {
		संरचना refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = strlen(prefix),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, रिक्त, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

गणना format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

गणना default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

स्थिर शून्य print_path(ध्रुव वर्ण *path, ध्रुव वर्ण *prefix,
		       गणना format_type format, गणना default_type def)
{
	संरचना strbuf sb = STRBUF_INIT;
	गणना path_format fmt;

	सचे (format == FORMAT_DEFAULT) {
		विकल्प (def) {
		पक्ष DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			भङ्ग;
		पक्ष DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			भङ्ग;
		पक्ष DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			भङ्ग;
		पक्ष DEFAULT_UNMODIFIED:
		स्वभाव:
			fmt = PATH_FORMAT_UNMODIFIED;
			भङ्ग;
		}
	} अञ्ञथा {
		विकल्प (format) {
		पक्ष FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			भङ्ग;
		पक्ष FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			भङ्ग;
		स्वभाव:
			fmt = PATH_FORMAT_UNMODIFIED;
			भङ्ग;
		}
	}

	format_path(&sb, path, prefix, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

पूर्णाङ्क cmd_rev_parse(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *prefix,
		  संरचना repository *repo UNUSED)
{
	पूर्णाङ्क i, as_is = 0, verify = 0, quiet = 0, revs_गणनम् = 0, type = 0;
	ध्रुव संरचना git_hash_algo *output_algo = रिक्त;
	ध्रुव संरचना git_hash_algo *compat = रिक्त;
	पूर्णाङ्क did_repo_setup = 0;
	पूर्णाङ्क has_dashdash = 0;
	पूर्णाङ्क output_prefix = 0;
	संरचना object_id oid;
	unsigned पूर्णाङ्क flags = 0;
	ध्रुव वर्ण *name = रिक्त;
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णाङ्क seen_end_of_options = 0;
	गणना format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	सचे (argc > 1 && !strcmp("--parseopt", argv[1]))
		पटिनिवत्त cmd_parseopt(argc - 1, argv + 1, prefix);

	सचे (argc > 1 && !strcmp("--sq-quote", argv[1]))
		पटिनिवत्त cmd_sq_quote(argc - 2, argv + 2);

	सचे (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	अत्थाय (i = 1; i < argc; i++) {
		सचे (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			भङ्ग;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	सचे (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, रिक्त);
		पटिनिवत्त 0;
	}

	अत्थाय (i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];

		सचे (as_is) {
			सचे (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			अनुवर्त;
		}

		सचे (!seen_end_of_options) {
			सचे (!strcmp(arg, "--local-env-vars")) {
				पूर्णाङ्क i;
				अत्थाय (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--resolve-git-dir")) {
				ध्रुव वर्ण *gitdir = argv[++i];
				सचे (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				सचे (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				अनुवर्त;
			}
		}

		/* The rest of the options require a git repository. */
		सचे (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, रिक्त);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		सचे (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			सचे (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			अनुवर्त;
		}

		सचे (!seen_end_of_options && *arg == '-') {
			सचे (!strcmp(arg, "--git-path")) {
				सचे (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				अनुवर्त;
			}
			सचे (!strcmp(arg,"-n")) {
				सचे (++i >= argc)
					die(_("-n requires an argument"));
				सचे ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				अनुवर्त;
			}
			सचे (starts_with(arg, "-n")) {
				सचे ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				अनुवर्त;
			}
			सचे (opt_with_मानम्(arg, "--path-format", &arg)) {
				सचे (!arg)
					die(_("--path-format requires an argument"));
				सचे (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} अञ्ञथा सचे (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} अञ्ञथा {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--default")) {
				def = argv[++i];
				सचे (!def)
					die(_("--default requires an argument"));
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				सचे (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				अनुवर्त;
			}
			सचे (opt_with_मानम्(arg, "--output-object-format", &arg)) {
				सचे (!arg)
					die(_("no object format specified"));
				सचे (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					अनुवर्त;
				}
				अञ्ञथा सचे (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					अनुवर्त;
				}
				अञ्ञथा die(_("unsupported object format: %s"), arg);
			}
			सचे (opt_with_मानम्(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				सचे (!arg)
					अनुवर्त;
				abbrev = strtoul(arg, रिक्त, 10);
				सचे (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				अञ्ञथा सचे ((पूर्णाङ्क)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--sq")) {
				output_sq = 1;
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				अनुवर्त;
			}
			सचे (opt_with_मानम्(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				सचे (arg) {
					सचे (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					अञ्ञथा सचे (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					अञ्ञथा
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, रिक्त);
				clear_ref_exclusions(&ref_excludes);
				अनुवर्त;
			}
			सचे (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, रिक्त);
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--bisect")) {
				वर्ण *prefix;
				वर्ण *term_bad = रिक्त;
				वर्ण *term_good = रिक्त;
				संरचना refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&term_bad, &term_good);
				prefix = xstrfmt("refs/bisect/%s", term_bad);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, रिक्त, &opts);
				मुक्त(prefix);
				prefix = xstrfmt("refs/bisect/%s", term_good);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, रिक्त, &opts);
				मुक्त(prefix);
				मुक्त(term_good);
				मुक्त(term_bad);
				अनुवर्त;
			}
			सचे (opt_with_मानम्(arg, "--branches", &arg)) {
				सचे (ref_excludes.hidden_refs_configured)
					पटिनिवत्त error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				अनुवर्त;
			}
			सचे (opt_with_मानम्(arg, "--tags", &arg)) {
				सचे (ref_excludes.hidden_refs_configured)
					पटिनिवत्त error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				अनुवर्त;
			}
			सचे (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, रिक्त);
				अनुवर्त;
			}
			सचे (opt_with_मानम्(arg, "--remotes", &arg)) {
				सचे (ref_excludes.hidden_refs_configured)
					पटिनिवत्त error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				अनुवर्त;
			}
			सचे (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				अनुवर्त;
			}
			सचे (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--show-toplevel")) {
				ध्रुव वर्ण *work_वृक्षः = repo_get_work_वृक्षः(the_repository);
				सचे (work_वृक्षः)
					print_path(work_वृक्षः, prefix, format, DEFAULT_UNMODIFIED);
				अञ्ञथा
					die(_("this operation must be run in a work tree"));
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--show-superproject-working-tree")) {
				संरचना strbuf superproject = STRBUF_INIT;
				सचे (get_superproject_working_वृक्षः(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--show-prefix")) {
				सचे (prefix)
					puts(prefix);
				अञ्ञथा
					putchar('\n');
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--show-cdup")) {
				ध्रुव वर्ण *pfx = prefix;
				सचे (!is_inside_work_वृक्षः(the_repository)) {
					ध्रुव वर्ण *work_वृक्षः =
						repo_get_work_वृक्षः(the_repository);
					सचे (work_वृक्षः)
						printf("%s\n", work_वृक्षः);
					अनुवर्त;
				}
				याव (pfx) {
					pfx = strchr(pfx, '/');
					सचे (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				ध्रुव वर्ण *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				वर्ण *cwd;
				पूर्णाङ्क len;
				गणना format_type wanted = format;
				सचे (arg[2] == 'g') {	/* --git-dir */
					सचे (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						अनुवर्त;
					}
					सचे (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						अनुवर्त;
					}
				} अञ्ञथा {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					सचे (!gitdir && !prefix)
						gitdir = ".git";
					सचे (gitdir) {
						संरचना strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						अनुवर्त;
					}
				}
				cwd = xgetcwd();
				len = strlen(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				मुक्त(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_वृक्षः(the_repository) ? "true"
						: "false");
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--shared-index-path")) {
				सचे (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				सचे (the_repository->index->split_index) {
					ध्रुव संरचना object_id *oid = &the_repository->index->split_index->base_oid;
					ध्रुव वर्ण *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				अनुवर्त;
			}
			सचे (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				अनुवर्त;
			}
			सचे (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				अनुवर्त;
			}
			सचे (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				अनुवर्त;
			}
			सचे (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				अनुवर्त;
			}
			सचे (opt_with_मानम्(arg, "--show-object-format", &arg)) {
				ध्रुव वर्ण *val = arg ? arg : "storage";

				सचे (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				सचे (!strcmp(val, "compat")) {
					सचे (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					अञ्ञथा
						putchar('\n');
				} अञ्ञथा {
					puts(the_hash_algo->name);
				}
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				अनुवर्त;
			}
			सचे (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				सचे (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				अनुवर्त;
			}
			सचे (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			अनुवर्त;
		}

		/* Not a flag argument */
		सचे (try_difference(arg))
			अनुवर्त;
		सचे (try_parent_shorthands(arg))
			अनुवर्त;
		name = arg;
		type = NORMAL;
		सचे (*arg == '^') {
			name++;
			type = REVERSED;
		}
		सचे (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			सचे (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			सचे (verify)
				revs_गणनम्++;
			अञ्ञथा
				show_rev(type, &oid, name);
			अनुवर्त;
		}
		सचे (verify)
			die_no_single_rev(quiet);
		सचे (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		सचे (!show_file(arg, output_prefix))
			अनुवर्त;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	सचे (verify) {
		सचे (revs_गणनम् == 1) {
			show_rev(type, &oid, name);
			पटिनिवत्त 0;
		} अञ्ञथा सचे (revs_गणनम् == 0 && show_default())
			पटिनिवत्त 0;
		die_no_single_rev(quiet);
	} अञ्ञथा
		show_default();
	पटिनिवत्त 0;
}
