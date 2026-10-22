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
स्थिर पूर्णांक filter = ~0;

स्थिर स्थिरांक अक्षर *def;

#define NORMAL 0
#define REVERSED 1
स्थिर पूर्णांक show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
स्थिर पूर्णांक symbolic;
स्थिर पूर्णांक abbrev;
स्थिर पूर्णांक abbrev_ref;
स्थिर पूर्णांक abbrev_ref_strict;
स्थिर पूर्णांक output_sq;

स्थिर पूर्णांक stuck_long;
स्थिर संरचना ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
स्थिर पूर्णांक is_rev_argument(स्थिरांक अक्षर *arg)
{
	स्थिर स्थिरांक अक्षर *rev_args[] = {
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
	स्थिरांक अक्षर **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	जर ((*arg == '-') && है_अंक(arg[1]))
		परत 1;

	साठी (;;) {
		स्थिरांक अक्षर *str = *p++;
		पूर्णांक len;
		जर (!str)
			परत 0;
		len = म_माप(str);
		जर (!म_भेद(arg, str) ||
		    (str[len-1] == '=' && !म_भेदन(arg, str, len)))
			परत 1;
	}
}

/* Output argument as a string, either SQ or normal */
स्थिर शून्य show(स्थिरांक अक्षर *arg)
{
	जर (output_sq) {
		पूर्णांक sq = '\'', ch;

		अक्षर_दो(sq);
		पर्यंत ((ch = *arg++)) {
			जर (ch == sq)
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
स्थिर शून्य show_with_type(पूर्णांक type, स्थिरांक अक्षर *arg)
{
	जर (type != show_type)
		अक्षर_दो('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
स्थिर शून्य show_rev(पूर्णांक type, स्थिरांक संरचना object_id *oid, स्थिरांक अक्षर *name)
{
	जर (!(filter & DO_REVS))
		परत;
	def = रिक्त;

	जर ((symbolic || abbrev_ref) && name) {
		जर (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			संरचना object_id discard;
			अक्षर *full;

			निवड (repo_dwim_ref(the_repository, name,
					      म_माप(name), &discard, &full,
					      0)) {
			स्थिती 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				थांबा;
			स्थिती 1: /* happy */
				जर (abbrev_ref) {
					अक्षर *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					मुक्त(old);
				}
				show_with_type(type, full);
				थांबा;
			पूर्वनिर्धारित: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				थांबा;
			}
			मुक्त(full);
		} अन्यथा {
			show_with_type(type, name);
		}
	}
	अन्यथा जर (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	अन्यथा
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
स्थिर पूर्णांक show_flag(स्थिरांक अक्षर *arg)
{
	जर (!(filter & DO_FLAGS))
		परत 0;
	जर (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		परत 1;
	}
	परत 0;
}

स्थिर पूर्णांक show_default(शून्य)
{
	स्थिरांक अक्षर *s = def;

	जर (s) {
		संरचना object_id oid;

		def = रिक्त;
		जर (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			परत 1;
		}
	}
	परत 0;
}

स्थिर पूर्णांक show_reference(स्थिरांक संरचना reference *ref, शून्य *cb_data UNUSED)
{
	जर (ref_excluded(&ref_excludes, ref->name))
		परत 0;
	show_rev(NORMAL, ref->oid, ref->name);
	परत 0;
}

स्थिर पूर्णांक anti_reference(स्थिरांक संरचना reference *ref, शून्य *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	परत 0;
}

स्थिर पूर्णांक show_abbrev(स्थिरांक संरचना object_id *oid, शून्य *cb_data UNUSED)
{
	show_rev(NORMAL, oid, रिक्त);
	परत 0;
}

स्थिर शून्य show_datestring(स्थिरांक अक्षर *flag, स्थिरांक अक्षर *datestr)
{
	अक्षर *buffer;

	/* date handling requires both flags and revs */
	जर ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		परत;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	मुक्त(buffer);
}

स्थिर पूर्णांक show_file(स्थिरांक अक्षर *arg, पूर्णांक output_prefix)
{
	show_default();
	जर ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		जर (output_prefix) {
			स्थिरांक अक्षर *prefix = the_repository->prefix;
			अक्षर *fname = prefix_filename(prefix, arg);
			show(fname);
			मुक्त(fname);
		} अन्यथा
			show(arg);
		परत 1;
	}
	परत 0;
}

स्थिर पूर्णांक try_difference(स्थिरांक अक्षर *arg)
{
	स्थिरांक अक्षर *dotdot;
	संरचना object_id start_oid;
	संरचना object_id end_oid;
	स्थिरांक अक्षर *end;
	स्थिरांक अक्षर *start;
	अक्षर *to_free;
	पूर्णांक symmetric;
	स्थिर स्थिरांक अक्षर head_by_default[] = "HEAD";

	जर (!(dotdot = म_माला(arg, "..")))
		परत 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	जर (!*end)
		end = head_by_default;
	जर (dotdot == arg)
		start = head_by_default;

	जर (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		मुक्त(to_free);
		परत 0;
	}

	जर (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		जर (symmetric) {
			संरचना commit_list *exclude = रिक्त;
			संरचना commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			जर (!a || !b) {
				मुक्त(to_free);
				परत 0;
			}
			जर (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				निकास(128);
			पर्यंत (exclude) {
				संरचना commit *commit = काढा_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, रिक्त);
			}
		}
		मुक्त(to_free);
		परत 1;
	}
	मुक्त(to_free);
	परत 0;
}

स्थिर पूर्णांक try_parent_shorthands(स्थिरांक अक्षर *arg)
{
	स्थिरांक अक्षर *mark;
	संरचना object_id oid;
	संरचना commit *commit;
	संरचना commit_list *parents;
	पूर्णांक parent_संख्या;
	पूर्णांक include_rev = 0;
	पूर्णांक include_parents = 0;
	पूर्णांक exclude_parent = 0;
	अक्षर *to_free;

	जर ((mark = म_माला(arg, "^!"))) {
		include_rev = 1;
		जर (mark[2])
			परत 0;
	} अन्यथा जर ((mark = म_माला(arg, "^@"))) {
		include_parents = 1;
		जर (mark[2])
			परत 0;
	} अन्यथा जर ((mark = म_माला(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		जर (mark[2]) {
			अक्षर *end;
			exclude_parent = म_से_अदीर्घ(mark + 2, &end, 10);
			जर (*end != '\0' || !exclude_parent)
				परत 0;
		}
	} अन्यथा
		परत 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	जर (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		मुक्त(to_free);
		परत 0;
	}

	जर (exclude_parent &&
	    exclude_parent > commit_list_मोजणी(commit->parents)) {
		मुक्त(to_free);
		परत 0;
	}

	जर (include_rev)
		show_rev(NORMAL, &oid, arg);
	साठी (parents = commit->parents, parent_संख्या = 1;
	     parents;
	     parents = parents->next, parent_संख्या++) {
		अक्षर *name = रिक्त;

		जर (exclude_parent && parent_संख्या != exclude_parent)
			सुरूठेवा;

		जर (symbolic)
			name = xstrfmt("%s^%d", arg, parent_संख्या);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		मुक्त(name);
	}

	मुक्त(to_free);
	परत 1;
}

स्थिर पूर्णांक parseopt_dump(स्थिरांक संरचना option *o, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना strbuf *parsed = o->मूल्य;
	जर (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	अन्यथा जर (o->short_name && (o->long_name == रिक्त || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	अन्यथा
		strbuf_addf(parsed, " --%s", o->long_name);
	जर (arg) {
		जर (!stuck_long)
			strbuf_addch(parsed, ' ');
		अन्यथा जर (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	परत 0;
}

स्थिर स्थिरांक अक्षर *skipspaces(स्थिरांक अक्षर *s)
{
	पर्यंत (है_खाली(*s))
		s++;
	परत s;
}

स्थिर अक्षर *findspace(स्थिरांक अक्षर *s)
{
	साठी (; *s; s++)
		जर (है_खाली(*s))
			परत (अक्षर*)s;
	परत रिक्त;
}

स्थिर पूर्णांक cmd_parseopt(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix)
{
	पूर्णांक keep_dashdash = 0, stop_at_non_option = 0;
	अक्षर स्थिरांक * स्थिरांक parseopt_usage[] = {
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
	जर (argc < 1 || म_भेद(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	साठी (;;) {
		strbuf_reset(&sb);
		जर (strbuf_getline(&sb, मानक_निवेश) == खातापूर्ण)
			die(_("premature end of input"));
		जर (!म_भेद("--", sb.buf)) {
			जर (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			थांबा;
		}

		strvec_ढकला(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	पर्यंत (strbuf_getline(&sb, मानक_निवेश) != खातापूर्ण) {
		स्थिरांक अक्षर *s;
		अक्षर *help;
		संरचना option *o;

		जर (!sb.len)
			सुरूठेवा;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		स_रखो(opts + opts_nr, 0, आकार(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		जर (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			सुरूठेवा;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->मूल्य = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		जर (!s)
			s = help;

		जर (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		जर (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} अन्यथा जर (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (पूर्णांक)(s - sb.buf), sb.buf);
		} अन्यथा {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (पूर्णांक)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		पर्यंत (s < help) {
			निवड (*s++) {
			स्थिती '=':
				o->flags &= ~PARSE_OPT_NOARG;
				सुरूठेवा;
			स्थिती '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				सुरूठेवा;
			स्थिती '!':
				o->flags |= PARSE_OPT_NONEG;
				सुरूठेवा;
			स्थिती '*':
				o->flags |= PARSE_OPT_HIDDEN;
				सुरूठेवा;
			}
			s--;
			थांबा;
		}

		जर (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	स_रखो(opts + opts_nr, 0, आकार(*opts));
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
	साठी (माप_प्रकार i = 0; i < opts_nr; i++) {
		मुक्त((अक्षर *) opts[i].help);
		मुक्त((अक्षर *) opts[i].argh);
	}
	मुक्त(opts);
	परत 0;
}

स्थिर पूर्णांक cmd_sq_quote(पूर्णांक argc, स्थिरांक अक्षर **argv)
{
	संरचना strbuf buf = STRBUF_INIT;

	जर (argc)
		sq_quote_argv(&buf, argv);
	म_लिखो("%s\n", buf.buf);
	strbuf_release(&buf);

	परत 0;
}

स्थिर शून्य die_no_single_rev(पूर्णांक quiet)
{
	जर (quiet)
		निकास(1);
	अन्यथा
		die(_("Needed a single revision"));
}

स्थिर स्थिरांक अक्षर builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
स्थिर पूर्णांक opt_with_मूल्य(स्थिरांक अक्षर *arg, स्थिरांक अक्षर *opt, स्थिरांक अक्षर **मूल्य)
{
	जर (skip_prefix(arg, opt, &arg)) {
		जर (!*arg) {
			*मूल्य = रिक्त;
			परत 1;
		}
		जर (*arg++ == '=') {
			*मूल्य = arg;
			परत 1;
		}
	}
	परत 0;
}

स्थिर शून्य handle_ref_opt(स्थिरांक अक्षर *pattern, स्थिरांक अक्षर *prefix)
{
	जर (pattern) {
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

स्थिर शून्य print_path(स्थिरांक अक्षर *path, स्थिरांक अक्षर *prefix,
		       गणना format_type format, गणना default_type def)
{
	संरचना strbuf sb = STRBUF_INIT;
	गणना path_format fmt;

	जर (format == FORMAT_DEFAULT) {
		निवड (def) {
		स्थिती DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			थांबा;
		स्थिती DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			थांबा;
		स्थिती DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			थांबा;
		स्थिती DEFAULT_UNMODIFIED:
		पूर्वनिर्धारित:
			fmt = PATH_FORMAT_UNMODIFIED;
			थांबा;
		}
	} अन्यथा {
		निवड (format) {
		स्थिती FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			थांबा;
		स्थिती FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			थांबा;
		पूर्वनिर्धारित:
			fmt = PATH_FORMAT_UNMODIFIED;
			थांबा;
		}
	}

	format_path(&sb, path, prefix, fmt);
	माला_दो(sb.buf);

	strbuf_release(&sb);
}

पूर्णांक cmd_rev_parse(पूर्णांक argc,
		  स्थिरांक अक्षर **argv,
		  स्थिरांक अक्षर *prefix,
		  संरचना repository *repo UNUSED)
{
	पूर्णांक i, as_is = 0, verify = 0, quiet = 0, revs_मोजणी = 0, type = 0;
	स्थिरांक संरचना git_hash_algo *output_algo = रिक्त;
	स्थिरांक संरचना git_hash_algo *compat = रिक्त;
	पूर्णांक did_repo_setup = 0;
	पूर्णांक has_dashdash = 0;
	पूर्णांक output_prefix = 0;
	संरचना object_id oid;
	अचिन्हित पूर्णांक flags = 0;
	स्थिरांक अक्षर *name = रिक्त;
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णांक seen_end_of_options = 0;
	गणना format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	जर (argc > 1 && !म_भेद("--parseopt", argv[1]))
		परत cmd_parseopt(argc - 1, argv + 1, prefix);

	जर (argc > 1 && !म_भेद("--sq-quote", argv[1]))
		परत cmd_sq_quote(argc - 2, argv + 2);

	जर (argc > 1 && !म_भेद("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	साठी (i = 1; i < argc; i++) {
		जर (!म_भेद(argv[i], "--")) {
			has_dashdash = 1;
			थांबा;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	जर (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, रिक्त);
		परत 0;
	}

	साठी (i = 1; i < argc; i++) {
		स्थिरांक अक्षर *arg = argv[i];

		जर (as_is) {
			जर (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			सुरूठेवा;
		}

		जर (!seen_end_of_options) {
			जर (!म_भेद(arg, "--local-env-vars")) {
				पूर्णांक i;
				साठी (i = 0; local_repo_env[i]; i++)
					म_लिखो("%s\n", local_repo_env[i]);
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--resolve-git-dir")) {
				स्थिरांक अक्षर *gitdir = argv[++i];
				जर (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				जर (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				माला_दो(gitdir);
				सुरूठेवा;
			}
		}

		/* The rest of the options require a git repository. */
		जर (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, रिक्त);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		जर (!म_भेद(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			जर (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			सुरूठेवा;
		}

		जर (!seen_end_of_options && *arg == '-') {
			जर (!म_भेद(arg, "--git-path")) {
				जर (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				सुरूठेवा;
			}
			जर (!म_भेद(arg,"-n")) {
				जर (++i >= argc)
					die(_("-n requires an argument"));
				जर ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				सुरूठेवा;
			}
			जर (starts_with(arg, "-n")) {
				जर ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				सुरूठेवा;
			}
			जर (opt_with_मूल्य(arg, "--path-format", &arg)) {
				जर (!arg)
					die(_("--path-format requires an argument"));
				जर (!म_भेद(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} अन्यथा जर (!म_भेद(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} अन्यथा {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--default")) {
				def = argv[++i];
				जर (!def)
					die(_("--default requires an argument"));
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--prefix")) {
				prefix = argv[++i];
				जर (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--quiet") || !म_भेद(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				सुरूठेवा;
			}
			जर (opt_with_मूल्य(arg, "--output-object-format", &arg)) {
				जर (!arg)
					die(_("no object format specified"));
				जर (!म_भेद(arg, the_hash_algo->name) ||
				    !म_भेद(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					सुरूठेवा;
				}
				अन्यथा जर (compat && !म_भेद(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					सुरूठेवा;
				}
				अन्यथा die(_("unsupported object format: %s"), arg);
			}
			जर (opt_with_मूल्य(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				जर (!arg)
					सुरूठेवा;
				abbrev = म_से_अदीर्घ(arg, रिक्त, 10);
				जर (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				अन्यथा जर ((पूर्णांक)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--sq")) {
				output_sq = 1;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--not")) {
				show_type ^= REVERSED;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				सुरूठेवा;
			}
			जर (opt_with_मूल्य(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				जर (arg) {
					जर (!म_भेद(arg, "strict"))
						abbrev_ref_strict = 1;
					अन्यथा जर (!म_भेद(arg, "loose"))
						abbrev_ref_strict = 0;
					अन्यथा
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, रिक्त);
				clear_ref_exclusions(&ref_excludes);
				सुरूठेवा;
			}
			जर (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, रिक्त);
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--bisect")) {
				अक्षर *prefix;
				अक्षर *term_bad = रिक्त;
				अक्षर *term_good = रिक्त;
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
				सुरूठेवा;
			}
			जर (opt_with_मूल्य(arg, "--branches", &arg)) {
				जर (ref_excludes.hidden_refs_configured)
					परत error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				सुरूठेवा;
			}
			जर (opt_with_मूल्य(arg, "--tags", &arg)) {
				जर (ref_excludes.hidden_refs_configured)
					परत error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				सुरूठेवा;
			}
			जर (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, रिक्त);
				सुरूठेवा;
			}
			जर (opt_with_मूल्य(arg, "--remotes", &arg)) {
				जर (ref_excludes.hidden_refs_configured)
					परत error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				सुरूठेवा;
			}
			जर (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				सुरूठेवा;
			}
			जर (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--show-toplevel")) {
				स्थिरांक अक्षर *work_वृक्ष = repo_get_work_वृक्ष(the_repository);
				जर (work_वृक्ष)
					print_path(work_वृक्ष, prefix, format, DEFAULT_UNMODIFIED);
				अन्यथा
					die(_("this operation must be run in a work tree"));
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--show-superproject-working-tree")) {
				संरचना strbuf superproject = STRBUF_INIT;
				जर (get_superproject_working_वृक्ष(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--show-prefix")) {
				जर (prefix)
					माला_दो(prefix);
				अन्यथा
					अक्षर_दो('\n');
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--show-cdup")) {
				स्थिरांक अक्षर *pfx = prefix;
				जर (!is_inside_work_वृक्ष(the_repository)) {
					स्थिरांक अक्षर *work_वृक्ष =
						repo_get_work_वृक्ष(the_repository);
					जर (work_वृक्ष)
						म_लिखो("%s\n", work_वृक्ष);
					सुरूठेवा;
				}
				पर्यंत (pfx) {
					pfx = म_अक्षर(pfx, '/');
					जर (pfx) {
						pfx++;
						म_लिखो("../");
					}
				}
				अक्षर_दो('\n');
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--git-dir") ||
			    !म_भेद(arg, "--absolute-git-dir")) {
				स्थिरांक अक्षर *gitdir = दो_पर्या(GIT_DIR_ENVIRONMENT);
				अक्षर *cwd;
				पूर्णांक len;
				गणना format_type wanted = format;
				जर (arg[2] == 'g') {	/* --git-dir */
					जर (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						सुरूठेवा;
					}
					जर (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						सुरूठेवा;
					}
				} अन्यथा {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					जर (!gitdir && !prefix)
						gitdir = ".git";
					जर (gitdir) {
						संरचना strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						माला_दो(realpath.buf);
						strbuf_release(&realpath);
						सुरूठेवा;
					}
				}
				cwd = xgetcwd();
				len = म_माप(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				मुक्त(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--is-inside-git-dir")) {
				म_लिखो("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--is-inside-work-tree")) {
				म_लिखो("%s\n", is_inside_work_वृक्ष(the_repository) ? "true"
						: "false");
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--is-bare-repository")) {
				म_लिखो("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--is-shallow-repository")) {
				म_लिखो("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--shared-index-path")) {
				जर (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				जर (the_repository->index->split_index) {
					स्थिरांक संरचना object_id *oid = &the_repository->index->split_index->base_oid;
					स्थिरांक अक्षर *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				सुरूठेवा;
			}
			जर (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				सुरूठेवा;
			}
			जर (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				सुरूठेवा;
			}
			जर (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				सुरूठेवा;
			}
			जर (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				सुरूठेवा;
			}
			जर (opt_with_मूल्य(arg, "--show-object-format", &arg)) {
				स्थिरांक अक्षर *val = arg ? arg : "storage";

				जर (म_भेद(val, "storage") &&
				    म_भेद(val, "compat") &&
				    म_भेद(val, "input") &&
				    म_भेद(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				जर (!म_भेद(val, "compat")) {
					जर (the_repository->compat_hash_algo)
						माला_दो(the_repository->compat_hash_algo->name);
					अन्यथा
						अक्षर_दो('\n');
				} अन्यथा {
					माला_दो(the_hash_algo->name);
				}
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--show-ref-format") || !म_भेद(arg, "--show-ref-storage-format")) {
				माला_दो(ref_storage_format_to_name(the_repository->ref_storage_format));
				सुरूठेवा;
			}
			जर (!म_भेद(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				जर (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				सुरूठेवा;
			}
			जर (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			सुरूठेवा;
		}

		/* Not a flag argument */
		जर (try_difference(arg))
			सुरूठेवा;
		जर (try_parent_shorthands(arg))
			सुरूठेवा;
		name = arg;
		type = NORMAL;
		जर (*arg == '^') {
			name++;
			type = REVERSED;
		}
		जर (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			जर (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			जर (verify)
				revs_मोजणी++;
			अन्यथा
				show_rev(type, &oid, name);
			सुरूठेवा;
		}
		जर (verify)
			die_no_single_rev(quiet);
		जर (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		जर (!show_file(arg, output_prefix))
			सुरूठेवा;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	जर (verify) {
		जर (revs_मोजणी == 1) {
			show_rev(type, &oid, name);
			परत 0;
		} अन्यथा जर (revs_मोजणी == 0 && show_default())
			परत 0;
		die_no_single_rev(quiet);
	} अन्यथा
		show_default();
	परत 0;
}
