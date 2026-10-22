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
	यदि ((*arg == '-') && है_अंक(arg[1]))
		प्रत्यागम 1;

	प्रति (;;) {
		ध्रुव वर्ण *str = *p++;
		पूर्णाङ्क len;
		यदि (!str)
			प्रत्यागम 0;
		len = म_माप(str);
		यदि (!म_भेद(arg, str) ||
		    (str[len-1] == '=' && !म_भेदन(arg, str, len)))
			प्रत्यागम 1;
	}
}

/* Output argument as a string, either SQ or normal */
स्थिर शून्य show(ध्रुव वर्ण *arg)
{
	यदि (output_sq) {
		पूर्णाङ्क sq = '\'', ch;

		अक्षर_दो(sq);
		यावत् ((ch = *arg++)) {
			यदि (ch == sq)
				ख_माला_दो("'\\'", मानक_निकास);
			अक्षर_दो(ch);
		}
		अक्षर_दो(sq);
		अक्षर_दो(' ');
	}
	अन्यथा
		माला_दो(arg);
}

/* Like show(), but with a negation prefix according to type */
स्थिर शून्य show_with_type(पूर्णाङ्क type, ध्रुव वर्ण *arg)
{
	यदि (type != show_type)
		अक्षर_दो('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
स्थिर शून्य show_rev(पूर्णाङ्क type, ध्रुव संरचना object_id *oid, ध्रुव वर्ण *name)
{
	यदि (!(filter & DO_REVS))
		प्रत्यागम;
	def = रिक्त;

	यदि ((symbolic || abbrev_ref) && name) {
		यदि (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			संरचना object_id discard;
			वर्ण *full;

			विकल्प (repo_dwim_ref(the_repository, name,
					      म_माप(name), &discard, &full,
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
				यदि (abbrev_ref) {
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
		} अन्यथा {
			show_with_type(type, name);
		}
	}
	अन्यथा यदि (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	अन्यथा
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
स्थिर पूर्णाङ्क show_flag(ध्रुव वर्ण *arg)
{
	यदि (!(filter & DO_FLAGS))
		प्रत्यागम 0;
	यदि (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		प्रत्यागम 1;
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_default(शून्य)
{
	ध्रुव वर्ण *s = def;

	यदि (s) {
		संरचना object_id oid;

		def = रिक्त;
		यदि (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			प्रत्यागम 1;
		}
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_reference(ध्रुव संरचना reference *ref, शून्य *cb_data UNUSED)
{
	यदि (ref_excluded(&ref_excludes, ref->name))
		प्रत्यागम 0;
	show_rev(NORMAL, ref->oid, ref->name);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क anti_reference(ध्रुव संरचना reference *ref, शून्य *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_abbrev(ध्रुव संरचना object_id *oid, शून्य *cb_data UNUSED)
{
	show_rev(NORMAL, oid, रिक्त);
	प्रत्यागम 0;
}

स्थिर शून्य show_datestring(ध्रुव वर्ण *flag, ध्रुव वर्ण *datestr)
{
	वर्ण *buffer;

	/* date handling requires both flags and revs */
	यदि ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		प्रत्यागम;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	मुक्त(buffer);
}

स्थिर पूर्णाङ्क show_file(ध्रुव वर्ण *arg, पूर्णाङ्क output_prefix)
{
	show_default();
	यदि ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		यदि (output_prefix) {
			ध्रुव वर्ण *prefix = the_repository->prefix;
			वर्ण *fname = prefix_filename(prefix, arg);
			show(fname);
			मुक्त(fname);
		} अन्यथा
			show(arg);
		प्रत्यागम 1;
	}
	प्रत्यागम 0;
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

	यदि (!(dotdot = म_माला(arg, "..")))
		प्रत्यागम 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	यदि (!*end)
		end = head_by_default;
	यदि (dotdot == arg)
		start = head_by_default;

	यदि (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		मुक्त(to_free);
		प्रत्यागम 0;
	}

	यदि (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		यदि (symmetric) {
			संरचना commit_list *exclude = रिक्त;
			संरचना commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			यदि (!a || !b) {
				मुक्त(to_free);
				प्रत्यागम 0;
			}
			यदि (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				निकास(128);
			यावत् (exclude) {
				संरचना commit *commit = निकालो_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, रिक्त);
			}
		}
		मुक्त(to_free);
		प्रत्यागम 1;
	}
	मुक्त(to_free);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क try_parent_shorthands(ध्रुव वर्ण *arg)
{
	ध्रुव वर्ण *mark;
	संरचना object_id oid;
	संरचना commit *commit;
	संरचना commit_list *parents;
	पूर्णाङ्क parent_संख्या;
	पूर्णाङ्क include_rev = 0;
	पूर्णाङ्क include_parents = 0;
	पूर्णाङ्क exclude_parent = 0;
	वर्ण *to_free;

	यदि ((mark = म_माला(arg, "^!"))) {
		include_rev = 1;
		यदि (mark[2])
			प्रत्यागम 0;
	} अन्यथा यदि ((mark = म_माला(arg, "^@"))) {
		include_parents = 1;
		यदि (mark[2])
			प्रत्यागम 0;
	} अन्यथा यदि ((mark = म_माला(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		यदि (mark[2]) {
			वर्ण *end;
			exclude_parent = म_से_अदीर्घ(mark + 2, &end, 10);
			यदि (*end != '\0' || !exclude_parent)
				प्रत्यागम 0;
		}
	} अन्यथा
		प्रत्यागम 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	यदि (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		मुक्त(to_free);
		प्रत्यागम 0;
	}

	यदि (exclude_parent &&
	    exclude_parent > commit_list_गिनती(commit->parents)) {
		मुक्त(to_free);
		प्रत्यागम 0;
	}

	यदि (include_rev)
		show_rev(NORMAL, &oid, arg);
	प्रति (parents = commit->parents, parent_संख्या = 1;
	     parents;
	     parents = parents->next, parent_संख्या++) {
		वर्ण *name = रिक्त;

		यदि (exclude_parent && parent_संख्या != exclude_parent)
			अनुवर्त;

		यदि (symbolic)
			name = xstrfmt("%s^%d", arg, parent_संख्या);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		मुक्त(name);
	}

	मुक्त(to_free);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क parseopt_dump(ध्रुव संरचना option *o, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना strbuf *parsed = o->मूल्य;
	यदि (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	अन्यथा यदि (o->short_name && (o->long_name == रिक्त || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	अन्यथा
		strbuf_addf(parsed, " --%s", o->long_name);
	यदि (arg) {
		यदि (!stuck_long)
			strbuf_addch(parsed, ' ');
		अन्यथा यदि (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *skipspaces(ध्रुव वर्ण *s)
{
	यावत् (है_खाली(*s))
		s++;
	प्रत्यागम s;
}

स्थिर वर्ण *findspace(ध्रुव वर्ण *s)
{
	प्रति (; *s; s++)
		यदि (है_खाली(*s))
			प्रत्यागम (वर्ण*)s;
	प्रत्यागम रिक्त;
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
	माप_प्रकार opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	यदि (argc < 1 || म_भेद(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	प्रति (;;) {
		strbuf_reset(&sb);
		यदि (strbuf_getline(&sb, मानक_निवेश) == खातापूर्ण)
			die(_("premature end of input"));
		यदि (!म_भेद("--", sb.buf)) {
			यदि (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			भङ्ग;
		}

		strvec_धकेलो(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	यावत् (strbuf_getline(&sb, मानक_निवेश) != खातापूर्ण) {
		ध्रुव वर्ण *s;
		वर्ण *help;
		संरचना option *o;

		यदि (!sb.len)
			अनुवर्त;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		स_रखो(opts + opts_nr, 0, परिमाण(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		यदि (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			अनुवर्त;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->मूल्य = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		यदि (!s)
			s = help;

		यदि (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		यदि (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} अन्यथा यदि (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (पूर्णाङ्क)(s - sb.buf), sb.buf);
		} अन्यथा {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (पूर्णाङ्क)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		यावत् (s < help) {
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

		यदि (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	स_रखो(opts + opts_nr, 0, परिमाण(*opts));
	argc = parse_options(argc, argv, prefix, opts, usage.v,
			(keep_dashdash ? PARSE_OPT_KEEP_DASHDASH : 0) |
			(stop_at_non_option ? PARSE_OPT_STOP_AT_NON_OPTION : 0) |
			PARSE_OPT_SHELL_EVAL);

	strbuf_addstr(&parsed, " --");
	sq_quote_argv(&parsed, argv);
	माला_दो(parsed.buf);

	strbuf_release(&parsed);
	strbuf_release(&sb);
	strvec_clear(&longnames);
	strvec_clear(&usage);
	प्रति (माप_प्रकार i = 0; i < opts_nr; i++) {
		मुक्त((वर्ण *) opts[i].help);
		मुक्त((वर्ण *) opts[i].argh);
	}
	मुक्त(opts);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cmd_sq_quote(पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	संरचना strbuf buf = STRBUF_INIT;

	यदि (argc)
		sq_quote_argv(&buf, argv);
	म_लिखो("%s\n", buf.buf);
	strbuf_release(&buf);

	प्रत्यागम 0;
}

स्थिर शून्य die_no_single_rev(पूर्णाङ्क quiet)
{
	यदि (quiet)
		निकास(1);
	अन्यथा
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
स्थिर पूर्णाङ्क opt_with_मूल्य(ध्रुव वर्ण *arg, ध्रुव वर्ण *opt, ध्रुव वर्ण **मूल्य)
{
	यदि (skip_prefix(arg, opt, &arg)) {
		यदि (!*arg) {
			*मूल्य = रिक्त;
			प्रत्यागम 1;
		}
		यदि (*arg++ == '=') {
			*मूल्य = arg;
			प्रत्यागम 1;
		}
	}
	प्रत्यागम 0;
}

स्थिर शून्य handle_ref_opt(ध्रुव वर्ण *pattern, ध्रुव वर्ण *prefix)
{
	यदि (pattern) {
		संरचना refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? म_माप(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, रिक्त, &opts);
	} अन्यथा {
		संरचना refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = म_माप(prefix),
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

	यदि (format == FORMAT_DEFAULT) {
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
	} अन्यथा {
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
	माला_दो(sb.buf);

	strbuf_release(&sb);
}

पूर्णाङ्क cmd_rev_parse(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *prefix,
		  संरचना repository *repo UNUSED)
{
	पूर्णाङ्क i, as_is = 0, verify = 0, quiet = 0, revs_गिनती = 0, type = 0;
	ध्रुव संरचना git_hash_algo *output_algo = रिक्त;
	ध्रुव संरचना git_hash_algo *compat = रिक्त;
	पूर्णाङ्क did_repo_setup = 0;
	पूर्णाङ्क has_dashdash = 0;
	पूर्णाङ्क output_prefix = 0;
	संरचना object_id oid;
	अचिन्हित पूर्णाङ्क flags = 0;
	ध्रुव वर्ण *name = रिक्त;
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णाङ्क seen_end_of_options = 0;
	गणना format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	यदि (argc > 1 && !म_भेद("--parseopt", argv[1]))
		प्रत्यागम cmd_parseopt(argc - 1, argv + 1, prefix);

	यदि (argc > 1 && !म_भेद("--sq-quote", argv[1]))
		प्रत्यागम cmd_sq_quote(argc - 2, argv + 2);

	यदि (argc > 1 && !म_भेद("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	प्रति (i = 1; i < argc; i++) {
		यदि (!म_भेद(argv[i], "--")) {
			has_dashdash = 1;
			भङ्ग;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	यदि (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, रिक्त);
		प्रत्यागम 0;
	}

	प्रति (i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];

		यदि (as_is) {
			यदि (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			अनुवर्त;
		}

		यदि (!seen_end_of_options) {
			यदि (!म_भेद(arg, "--local-env-vars")) {
				पूर्णाङ्क i;
				प्रति (i = 0; local_repo_env[i]; i++)
					म_लिखो("%s\n", local_repo_env[i]);
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--resolve-git-dir")) {
				ध्रुव वर्ण *gitdir = argv[++i];
				यदि (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				यदि (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				माला_दो(gitdir);
				अनुवर्त;
			}
		}

		/* The rest of the options require a git repository. */
		यदि (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, रिक्त);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		यदि (!म_भेद(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			यदि (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			अनुवर्त;
		}

		यदि (!seen_end_of_options && *arg == '-') {
			यदि (!म_भेद(arg, "--git-path")) {
				यदि (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				अनुवर्त;
			}
			यदि (!म_भेद(arg,"-n")) {
				यदि (++i >= argc)
					die(_("-n requires an argument"));
				यदि ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				अनुवर्त;
			}
			यदि (starts_with(arg, "-n")) {
				यदि ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				अनुवर्त;
			}
			यदि (opt_with_मूल्य(arg, "--path-format", &arg)) {
				यदि (!arg)
					die(_("--path-format requires an argument"));
				यदि (!म_भेद(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} अन्यथा यदि (!म_भेद(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} अन्यथा {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--default")) {
				def = argv[++i];
				यदि (!def)
					die(_("--default requires an argument"));
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--prefix")) {
				prefix = argv[++i];
				यदि (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--quiet") || !म_भेद(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				अनुवर्त;
			}
			यदि (opt_with_मूल्य(arg, "--output-object-format", &arg)) {
				यदि (!arg)
					die(_("no object format specified"));
				यदि (!म_भेद(arg, the_hash_algo->name) ||
				    !म_भेद(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					अनुवर्त;
				}
				अन्यथा यदि (compat && !म_भेद(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					अनुवर्त;
				}
				अन्यथा die(_("unsupported object format: %s"), arg);
			}
			यदि (opt_with_मूल्य(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				यदि (!arg)
					अनुवर्त;
				abbrev = म_से_अदीर्घ(arg, रिक्त, 10);
				यदि (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				अन्यथा यदि ((पूर्णाङ्क)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--sq")) {
				output_sq = 1;
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--not")) {
				show_type ^= REVERSED;
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				अनुवर्त;
			}
			यदि (opt_with_मूल्य(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				यदि (arg) {
					यदि (!म_भेद(arg, "strict"))
						abbrev_ref_strict = 1;
					अन्यथा यदि (!म_भेद(arg, "loose"))
						abbrev_ref_strict = 0;
					अन्यथा
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, रिक्त);
				clear_ref_exclusions(&ref_excludes);
				अनुवर्त;
			}
			यदि (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, रिक्त);
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--bisect")) {
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
			यदि (opt_with_मूल्य(arg, "--branches", &arg)) {
				यदि (ref_excludes.hidden_refs_configured)
					प्रत्यागम error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				अनुवर्त;
			}
			यदि (opt_with_मूल्य(arg, "--tags", &arg)) {
				यदि (ref_excludes.hidden_refs_configured)
					प्रत्यागम error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				अनुवर्त;
			}
			यदि (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, रिक्त);
				अनुवर्त;
			}
			यदि (opt_with_मूल्य(arg, "--remotes", &arg)) {
				यदि (ref_excludes.hidden_refs_configured)
					प्रत्यागम error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				अनुवर्त;
			}
			यदि (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				अनुवर्त;
			}
			यदि (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--show-toplevel")) {
				ध्रुव वर्ण *work_वृक्ष = repo_get_work_वृक्ष(the_repository);
				यदि (work_वृक्ष)
					print_path(work_वृक्ष, prefix, format, DEFAULT_UNMODIFIED);
				अन्यथा
					die(_("this operation must be run in a work tree"));
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--show-superproject-working-tree")) {
				संरचना strbuf superproject = STRBUF_INIT;
				यदि (get_superproject_working_वृक्ष(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--show-prefix")) {
				यदि (prefix)
					माला_दो(prefix);
				अन्यथा
					अक्षर_दो('\n');
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--show-cdup")) {
				ध्रुव वर्ण *pfx = prefix;
				यदि (!is_inside_work_वृक्ष(the_repository)) {
					ध्रुव वर्ण *work_वृक्ष =
						repo_get_work_वृक्ष(the_repository);
					यदि (work_वृक्ष)
						म_लिखो("%s\n", work_वृक्ष);
					अनुवर्त;
				}
				यावत् (pfx) {
					pfx = म_अक्षर(pfx, '/');
					यदि (pfx) {
						pfx++;
						म_लिखो("../");
					}
				}
				अक्षर_दो('\n');
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--git-dir") ||
			    !म_भेद(arg, "--absolute-git-dir")) {
				ध्रुव वर्ण *gitdir = दो_पर्या(GIT_DIR_ENVIRONMENT);
				वर्ण *cwd;
				पूर्णाङ्क len;
				गणना format_type wanted = format;
				यदि (arg[2] == 'g') {	/* --git-dir */
					यदि (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						अनुवर्त;
					}
					यदि (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						अनुवर्त;
					}
				} अन्यथा {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					यदि (!gitdir && !prefix)
						gitdir = ".git";
					यदि (gitdir) {
						संरचना strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						माला_दो(realpath.buf);
						strbuf_release(&realpath);
						अनुवर्त;
					}
				}
				cwd = xgetcwd();
				len = म_माप(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				मुक्त(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--is-inside-git-dir")) {
				म_लिखो("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--is-inside-work-tree")) {
				म_लिखो("%s\n", is_inside_work_वृक्ष(the_repository) ? "true"
						: "false");
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--is-bare-repository")) {
				म_लिखो("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--is-shallow-repository")) {
				म_लिखो("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--shared-index-path")) {
				यदि (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				यदि (the_repository->index->split_index) {
					ध्रुव संरचना object_id *oid = &the_repository->index->split_index->base_oid;
					ध्रुव वर्ण *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				अनुवर्त;
			}
			यदि (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				अनुवर्त;
			}
			यदि (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				अनुवर्त;
			}
			यदि (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				अनुवर्त;
			}
			यदि (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				अनुवर्त;
			}
			यदि (opt_with_मूल्य(arg, "--show-object-format", &arg)) {
				ध्रुव वर्ण *val = arg ? arg : "storage";

				यदि (म_भेद(val, "storage") &&
				    म_भेद(val, "compat") &&
				    म_भेद(val, "input") &&
				    म_भेद(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				यदि (!म_भेद(val, "compat")) {
					यदि (the_repository->compat_hash_algo)
						माला_दो(the_repository->compat_hash_algo->name);
					अन्यथा
						अक्षर_दो('\n');
				} अन्यथा {
					माला_दो(the_hash_algo->name);
				}
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--show-ref-format") || !म_भेद(arg, "--show-ref-storage-format")) {
				माला_दो(ref_storage_format_to_name(the_repository->ref_storage_format));
				अनुवर्त;
			}
			यदि (!म_भेद(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				यदि (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				अनुवर्त;
			}
			यदि (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			अनुवर्त;
		}

		/* Not a flag argument */
		यदि (try_difference(arg))
			अनुवर्त;
		यदि (try_parent_shorthands(arg))
			अनुवर्त;
		name = arg;
		type = NORMAL;
		यदि (*arg == '^') {
			name++;
			type = REVERSED;
		}
		यदि (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			यदि (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			यदि (verify)
				revs_गिनती++;
			अन्यथा
				show_rev(type, &oid, name);
			अनुवर्त;
		}
		यदि (verify)
			die_no_single_rev(quiet);
		यदि (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		यदि (!show_file(arg, output_prefix))
			अनुवर्त;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	यदि (verify) {
		यदि (revs_गिनती == 1) {
			show_rev(type, &oid, name);
			प्रत्यागम 0;
		} अन्यथा यदि (revs_गिनती == 0 && show_default())
			प्रत्यागम 0;
		die_no_single_rev(quiet);
	} अन्यथा
		show_default();
	प्रत्यागम 0;
}
