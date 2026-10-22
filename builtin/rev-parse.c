/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"

#समावेश "abspath.h"
#समावेश "bisect.h"
#समावेश "config.h"
#समावेश "commit.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hash.h"
#समावेश "hex.h"
#समावेश "refs.h"
#समावेश "quote.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "diff.h"
#समावेश "read-cache-ll.h"
#समावेश "repo-settings.h"
#समावेश "repository.h"
#समावेश "revision.h"
#समावेश "setup.h"
#समावेश "split-index.h"
#समावेश "submodule.h"
#समावेश "commit-reach.h"
#समावेश "shallow.h"
#समावेश "object-file-convert.h"

#घोषणा DO_REVS		1
#घोषणा DO_NOREV	2
#घोषणा DO_FLAGS	4
#घोषणा DO_NONFLAGS	8
अटल पूर्णांक filter = ~0;

अटल स्थिर अक्षर *def;

#घोषणा NORMAL 0
#घोषणा REVERSED 1
अटल पूर्णांक show_type = NORMAL;

#घोषणा SHOW_SYMBOLIC_ASIS 1
#घोषणा SHOW_SYMBOLIC_FULL 2
अटल पूर्णांक symbolic;
अटल पूर्णांक abbrev;
अटल पूर्णांक abbrev_ref;
अटल पूर्णांक abbrev_ref_strict;
अटल पूर्णांक output_sq;

अटल पूर्णांक stuck_long;
अटल काष्ठा ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
अटल पूर्णांक is_rev_argument(स्थिर अक्षर *arg)
{
	अटल स्थिर अक्षर *rev_args[] = {
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
		शून्य
	};
	स्थिर अक्षर **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	अगर ((*arg == '-') && है_अंक(arg[1]))
		वापस 1;

	क्रम (;;) {
		स्थिर अक्षर *str = *p++;
		पूर्णांक len;
		अगर (!str)
			वापस 0;
		len = म_माप(str);
		अगर (!म_भेद(arg, str) ||
		    (str[len-1] == '=' && !म_भेदन(arg, str, len)))
			वापस 1;
	}
}

/* Output argument as a string, either SQ or normal */
अटल व्योम show(स्थिर अक्षर *arg)
{
	अगर (output_sq) {
		पूर्णांक sq = '\'', ch;

		अक्षर_दो(sq);
		जबतक ((ch = *arg++)) {
			अगर (ch == sq)
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
अटल व्योम show_with_type(पूर्णांक type, स्थिर अक्षर *arg)
{
	अगर (type != show_type)
		अक्षर_दो('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
अटल व्योम show_rev(पूर्णांक type, स्थिर काष्ठा object_id *oid, स्थिर अक्षर *name)
{
	अगर (!(filter & DO_REVS))
		वापस;
	def = शून्य;

	अगर ((symbolic || abbrev_ref) && name) {
		अगर (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			काष्ठा object_id discard;
			अक्षर *full;

			चयन (repo_dwim_ref(the_repository, name,
					      म_माप(name), &discard, &full,
					      0)) {
			हाल 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				अवरोध;
			हाल 1: /* happy */
				अगर (abbrev_ref) {
					अक्षर *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					मुक्त(old);
				}
				show_with_type(type, full);
				अवरोध;
			शेष: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				अवरोध;
			}
			मुक्त(full);
		} अन्यथा {
			show_with_type(type, name);
		}
	}
	अन्यथा अगर (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	अन्यथा
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
अटल पूर्णांक show_flag(स्थिर अक्षर *arg)
{
	अगर (!(filter & DO_FLAGS))
		वापस 0;
	अगर (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		वापस 1;
	}
	वापस 0;
}

अटल पूर्णांक show_default(व्योम)
{
	स्थिर अक्षर *s = def;

	अगर (s) {
		काष्ठा object_id oid;

		def = शून्य;
		अगर (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			वापस 1;
		}
	}
	वापस 0;
}

अटल पूर्णांक show_reference(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य UNUSED)
{
	अगर (ref_excluded(&ref_excludes, ref->name))
		वापस 0;
	show_rev(NORMAL, ref->oid, ref->name);
	वापस 0;
}

अटल पूर्णांक anti_reference(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	वापस 0;
}

अटल पूर्णांक show_abbrev(स्थिर काष्ठा object_id *oid, व्योम *cb_तथ्य UNUSED)
{
	show_rev(NORMAL, oid, शून्य);
	वापस 0;
}

अटल व्योम show_datestring(स्थिर अक्षर *flag, स्थिर अक्षर *datestr)
{
	अक्षर *बफर;

	/* date handling requires both flags and revs */
	अगर ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		वापस;
	बफर = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(बफर);
	मुक्त(बफर);
}

अटल पूर्णांक show_file(स्थिर अक्षर *arg, पूर्णांक output_prefix)
{
	show_default();
	अगर ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		अगर (output_prefix) {
			स्थिर अक्षर *prefix = the_repository->prefix;
			अक्षर *fname = prefix_filename(prefix, arg);
			show(fname);
			मुक्त(fname);
		} अन्यथा
			show(arg);
		वापस 1;
	}
	वापस 0;
}

अटल पूर्णांक try_difference(स्थिर अक्षर *arg)
{
	स्थिर अक्षर *dotdot;
	काष्ठा object_id start_oid;
	काष्ठा object_id end_oid;
	स्थिर अक्षर *end;
	स्थिर अक्षर *start;
	अक्षर *to_free;
	पूर्णांक symmetric;
	अटल स्थिर अक्षर शीर्ष_by_default[] = "HEAD";

	अगर (!(dotdot = म_माला(arg, "..")))
		वापस 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	अगर (!*end)
		end = शीर्ष_by_default;
	अगर (dotdot == arg)
		start = शीर्ष_by_default;

	अगर (start == शीर्ष_by_default && end == शीर्ष_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		मुक्त(to_free);
		वापस 0;
	}

	अगर (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		अगर (symmetric) {
			काष्ठा commit_list *exclude = शून्य;
			काष्ठा commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			अगर (!a || !b) {
				मुक्त(to_free);
				वापस 0;
			}
			अगर (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				निकास(128);
			जबतक (exclude) {
				काष्ठा commit *commit = निकालो_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, शून्य);
			}
		}
		मुक्त(to_free);
		वापस 1;
	}
	मुक्त(to_free);
	वापस 0;
}

अटल पूर्णांक try_parent_shorthands(स्थिर अक्षर *arg)
{
	स्थिर अक्षर *mark;
	काष्ठा object_id oid;
	काष्ठा commit *commit;
	काष्ठा commit_list *parents;
	पूर्णांक parent_संख्या;
	पूर्णांक include_rev = 0;
	पूर्णांक include_parents = 0;
	पूर्णांक exclude_parent = 0;
	अक्षर *to_free;

	अगर ((mark = म_माला(arg, "^!"))) {
		include_rev = 1;
		अगर (mark[2])
			वापस 0;
	} अन्यथा अगर ((mark = म_माला(arg, "^@"))) {
		include_parents = 1;
		अगर (mark[2])
			वापस 0;
	} अन्यथा अगर ((mark = म_माला(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		अगर (mark[2]) {
			अक्षर *end;
			exclude_parent = म_से_अदीर्घ(mark + 2, &end, 10);
			अगर (*end != '\0' || !exclude_parent)
				वापस 0;
		}
	} अन्यथा
		वापस 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	अगर (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		मुक्त(to_free);
		वापस 0;
	}

	अगर (exclude_parent &&
	    exclude_parent > commit_list_गिनती(commit->parents)) {
		मुक्त(to_free);
		वापस 0;
	}

	अगर (include_rev)
		show_rev(NORMAL, &oid, arg);
	क्रम (parents = commit->parents, parent_संख्या = 1;
	     parents;
	     parents = parents->अग्र, parent_संख्या++) {
		अक्षर *name = शून्य;

		अगर (exclude_parent && parent_संख्या != exclude_parent)
			जारी;

		अगर (symbolic)
			name = xstrfmt("%s^%d", arg, parent_संख्या);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		मुक्त(name);
	}

	मुक्त(to_free);
	वापस 1;
}

अटल पूर्णांक parseopt_dump(स्थिर काष्ठा option *o, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा strbuf *parsed = o->मूल्य;
	अगर (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	अन्यथा अगर (o->short_name && (o->long_name == शून्य || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	अन्यथा
		strbuf_addf(parsed, " --%s", o->long_name);
	अगर (arg) {
		अगर (!stuck_long)
			strbuf_addch(parsed, ' ');
		अन्यथा अगर (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	वापस 0;
}

अटल स्थिर अक्षर *skipspaces(स्थिर अक्षर *s)
{
	जबतक (है_खाली(*s))
		s++;
	वापस s;
}

अटल अक्षर *findspace(स्थिर अक्षर *s)
{
	क्रम (; *s; s++)
		अगर (है_खाली(*s))
			वापस (अक्षर*)s;
	वापस शून्य;
}

अटल पूर्णांक cmd_parseopt(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix)
{
	पूर्णांक keep_dashdash = 0, stop_at_non_option = 0;
	अक्षर स्थिर * स्थिर parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		शून्य
	};
	काष्ठा option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_END(),
	};
	काष्ठा strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	काष्ठा strvec longnames = STRVEC_INIT;
	काष्ठा strvec usage = STRVEC_INIT;
	काष्ठा option *opts = शून्य;
	माप_प्रकार opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	अगर (argc < 1 || म_भेद(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	क्रम (;;) {
		strbuf_reset(&sb);
		अगर (strbuf_getline(&sb, मानक_निवेश) == खातापूर्ण)
			die(_("premature end of input"));
		अगर (!म_भेद("--", sb.buf)) {
			अगर (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			अवरोध;
		}

		strvec_धकेलो(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	जबतक (strbuf_getline(&sb, मानक_निवेश) != खातापूर्ण) {
		स्थिर अक्षर *s;
		अक्षर *help;
		काष्ठा option *o;

		अगर (!sb.len)
			जारी;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		स_रखो(opts + opts_nr, 0, माप(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		अगर (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			जारी;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->मूल्य = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		अगर (!s)
			s = help;

		अगर (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		अगर (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} अन्यथा अगर (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (पूर्णांक)(s - sb.buf), sb.buf);
		} अन्यथा {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (पूर्णांक)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		जबतक (s < help) {
			चयन (*s++) {
			हाल '=':
				o->flags &= ~PARSE_OPT_NOARG;
				जारी;
			हाल '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				जारी;
			हाल '!':
				o->flags |= PARSE_OPT_NONEG;
				जारी;
			हाल '*':
				o->flags |= PARSE_OPT_HIDDEN;
				जारी;
			}
			s--;
			अवरोध;
		}

		अगर (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	स_रखो(opts + opts_nr, 0, माप(*opts));
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
	क्रम (माप_प्रकार i = 0; i < opts_nr; i++) {
		मुक्त((अक्षर *) opts[i].help);
		मुक्त((अक्षर *) opts[i].argh);
	}
	मुक्त(opts);
	वापस 0;
}

अटल पूर्णांक cmd_sq_quote(पूर्णांक argc, स्थिर अक्षर **argv)
{
	काष्ठा strbuf buf = STRBUF_INIT;

	अगर (argc)
		sq_quote_argv(&buf, argv);
	म_लिखो("%s\n", buf.buf);
	strbuf_release(&buf);

	वापस 0;
}

अटल व्योम die_no_single_rev(पूर्णांक quiet)
{
	अगर (quiet)
		निकास(1);
	अन्यथा
		die(_("Needed a single revision"));
}

अटल स्थिर अक्षर builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
अटल पूर्णांक opt_with_मूल्य(स्थिर अक्षर *arg, स्थिर अक्षर *opt, स्थिर अक्षर **मूल्य)
{
	अगर (skip_prefix(arg, opt, &arg)) {
		अगर (!*arg) {
			*मूल्य = शून्य;
			वापस 1;
		}
		अगर (*arg++ == '=') {
			*मूल्य = arg;
			वापस 1;
		}
	}
	वापस 0;
}

अटल व्योम handle_ref_opt(स्थिर अक्षर *pattern, स्थिर अक्षर *prefix)
{
	अगर (pattern) {
		काष्ठा refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? म_माप(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, शून्य, &opts);
	} अन्यथा {
		काष्ठा refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = म_माप(prefix),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, शून्य, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

क्रमागत format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

क्रमागत default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

अटल व्योम print_path(स्थिर अक्षर *path, स्थिर अक्षर *prefix,
		       क्रमागत format_type format, क्रमागत default_type def)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	क्रमागत path_format fmt;

	अगर (format == FORMAT_DEFAULT) {
		चयन (def) {
		हाल DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			अवरोध;
		हाल DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			अवरोध;
		हाल DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			अवरोध;
		हाल DEFAULT_UNMODIFIED:
		शेष:
			fmt = PATH_FORMAT_UNMODIFIED;
			अवरोध;
		}
	} अन्यथा {
		चयन (format) {
		हाल FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			अवरोध;
		हाल FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			अवरोध;
		शेष:
			fmt = PATH_FORMAT_UNMODIFIED;
			अवरोध;
		}
	}

	format_path(&sb, path, prefix, fmt);
	माला_दो(sb.buf);

	strbuf_release(&sb);
}

पूर्णांक cmd_rev_parse(पूर्णांक argc,
		  स्थिर अक्षर **argv,
		  स्थिर अक्षर *prefix,
		  काष्ठा repository *repo UNUSED)
{
	पूर्णांक i, as_is = 0, verify = 0, quiet = 0, revs_गिनती = 0, type = 0;
	स्थिर काष्ठा git_hash_algo *output_algo = शून्य;
	स्थिर काष्ठा git_hash_algo *compat = शून्य;
	पूर्णांक did_repo_setup = 0;
	पूर्णांक has_dashdash = 0;
	पूर्णांक output_prefix = 0;
	काष्ठा object_id oid;
	अचिन्हित पूर्णांक flags = 0;
	स्थिर अक्षर *name = शून्य;
	काष्ठा strbuf buf = STRBUF_INIT;
	पूर्णांक seen_end_of_options = 0;
	क्रमागत format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	अगर (argc > 1 && !म_भेद("--parseopt", argv[1]))
		वापस cmd_parseopt(argc - 1, argv + 1, prefix);

	अगर (argc > 1 && !म_भेद("--sq-quote", argv[1]))
		वापस cmd_sq_quote(argc - 2, argv + 2);

	अगर (argc > 1 && !म_भेद("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	क्रम (i = 1; i < argc; i++) {
		अगर (!म_भेद(argv[i], "--")) {
			has_dashdash = 1;
			अवरोध;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	अगर (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, शून्य);
		वापस 0;
	}

	क्रम (i = 1; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];

		अगर (as_is) {
			अगर (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			जारी;
		}

		अगर (!seen_end_of_options) {
			अगर (!म_भेद(arg, "--local-env-vars")) {
				पूर्णांक i;
				क्रम (i = 0; local_repo_env[i]; i++)
					म_लिखो("%s\n", local_repo_env[i]);
				जारी;
			}
			अगर (!म_भेद(arg, "--resolve-git-dir")) {
				स्थिर अक्षर *gitdir = argv[++i];
				अगर (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				अगर (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				माला_दो(gitdir);
				जारी;
			}
		}

		/* The rest of the options require a git repository. */
		अगर (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, शून्य);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_सूचकांक = 0;
			compat = the_repository->compat_hash_algo;
		}

		अगर (!म_भेद(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			अगर (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			जारी;
		}

		अगर (!seen_end_of_options && *arg == '-') {
			अगर (!म_भेद(arg, "--git-path")) {
				अगर (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				जारी;
			}
			अगर (!म_भेद(arg,"-n")) {
				अगर (++i >= argc)
					die(_("-n requires an argument"));
				अगर ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				जारी;
			}
			अगर (starts_with(arg, "-n")) {
				अगर ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				जारी;
			}
			अगर (opt_with_मूल्य(arg, "--path-format", &arg)) {
				अगर (!arg)
					die(_("--path-format requires an argument"));
				अगर (!म_भेद(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} अन्यथा अगर (!म_भेद(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} अन्यथा {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				जारी;
			}
			अगर (!म_भेद(arg, "--default")) {
				def = argv[++i];
				अगर (!def)
					die(_("--default requires an argument"));
				जारी;
			}
			अगर (!म_भेद(arg, "--prefix")) {
				prefix = argv[++i];
				अगर (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				जारी;
			}
			अगर (!म_भेद(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				जारी;
			}
			अगर (!म_भेद(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				जारी;
			}
			अगर (!म_भेद(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				जारी;
			}
			अगर (!म_भेद(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				जारी;
			}
			अगर (!म_भेद(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				जारी;
			}
			अगर (!म_भेद(arg, "--quiet") || !म_भेद(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				जारी;
			}
			अगर (opt_with_मूल्य(arg, "--output-object-format", &arg)) {
				अगर (!arg)
					die(_("no object format specified"));
				अगर (!म_भेद(arg, the_hash_algo->name) ||
				    !म_भेद(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					जारी;
				}
				अन्यथा अगर (compat && !म_भेद(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					जारी;
				}
				अन्यथा die(_("unsupported object format: %s"), arg);
			}
			अगर (opt_with_मूल्य(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				अगर (!arg)
					जारी;
				abbrev = म_से_अदीर्घ(arg, शून्य, 10);
				अगर (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				अन्यथा अगर ((पूर्णांक)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				जारी;
			}
			अगर (!म_भेद(arg, "--sq")) {
				output_sq = 1;
				जारी;
			}
			अगर (!म_भेद(arg, "--not")) {
				show_type ^= REVERSED;
				जारी;
			}
			अगर (!म_भेद(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				जारी;
			}
			अगर (!म_भेद(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				जारी;
			}
			अगर (opt_with_मूल्य(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				अगर (arg) {
					अगर (!म_भेद(arg, "strict"))
						abbrev_ref_strict = 1;
					अन्यथा अगर (!म_भेद(arg, "loose"))
						abbrev_ref_strict = 0;
					अन्यथा
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				जारी;
			}
			अगर (!म_भेद(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, शून्य);
				clear_ref_exclusions(&ref_excludes);
				जारी;
			}
			अगर (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, शून्य);
				जारी;
			}
			अगर (!म_भेद(arg, "--bisect")) {
				अक्षर *prefix;
				अक्षर *term_bad = शून्य;
				अक्षर *term_good = शून्य;
				काष्ठा refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&term_bad, &term_good);
				prefix = xstrfmt("refs/bisect/%s", term_bad);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, शून्य, &opts);
				मुक्त(prefix);
				prefix = xstrfmt("refs/bisect/%s", term_good);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, शून्य, &opts);
				मुक्त(prefix);
				मुक्त(term_good);
				मुक्त(term_bad);
				जारी;
			}
			अगर (opt_with_मूल्य(arg, "--branches", &arg)) {
				अगर (ref_excludes.hidden_refs_configured)
					वापस error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				जारी;
			}
			अगर (opt_with_मूल्य(arg, "--tags", &arg)) {
				अगर (ref_excludes.hidden_refs_configured)
					वापस error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				जारी;
			}
			अगर (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, शून्य);
				जारी;
			}
			अगर (opt_with_मूल्य(arg, "--remotes", &arg)) {
				अगर (ref_excludes.hidden_refs_configured)
					वापस error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				जारी;
			}
			अगर (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				जारी;
			}
			अगर (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				जारी;
			}
			अगर (!म_भेद(arg, "--show-toplevel")) {
				स्थिर अक्षर *work_वृक्ष = repo_get_work_वृक्ष(the_repository);
				अगर (work_वृक्ष)
					print_path(work_वृक्ष, prefix, format, DEFAULT_UNMODIFIED);
				अन्यथा
					die(_("this operation must be run in a work tree"));
				जारी;
			}
			अगर (!म_भेद(arg, "--show-superproject-working-tree")) {
				काष्ठा strbuf superproject = STRBUF_INIT;
				अगर (get_superproject_working_वृक्ष(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				जारी;
			}
			अगर (!म_भेद(arg, "--show-prefix")) {
				अगर (prefix)
					माला_दो(prefix);
				अन्यथा
					अक्षर_दो('\n');
				जारी;
			}
			अगर (!म_भेद(arg, "--show-cdup")) {
				स्थिर अक्षर *pfx = prefix;
				अगर (!is_inside_work_वृक्ष(the_repository)) {
					स्थिर अक्षर *work_वृक्ष =
						repo_get_work_वृक्ष(the_repository);
					अगर (work_वृक्ष)
						म_लिखो("%s\n", work_वृक्ष);
					जारी;
				}
				जबतक (pfx) {
					pfx = म_अक्षर(pfx, '/');
					अगर (pfx) {
						pfx++;
						म_लिखो("../");
					}
				}
				अक्षर_दो('\n');
				जारी;
			}
			अगर (!म_भेद(arg, "--git-dir") ||
			    !म_भेद(arg, "--absolute-git-dir")) {
				स्थिर अक्षर *gitdir = दो_पर्या(GIT_DIR_ENVIRONMENT);
				अक्षर *cwd;
				पूर्णांक len;
				क्रमागत format_type wanted = format;
				अगर (arg[2] == 'g') {	/* --git-dir */
					अगर (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						जारी;
					}
					अगर (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						जारी;
					}
				} अन्यथा {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					अगर (!gitdir && !prefix)
						gitdir = ".git";
					अगर (gitdir) {
						काष्ठा strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						माला_दो(realpath.buf);
						strbuf_release(&realpath);
						जारी;
					}
				}
				cwd = xgetcwd();
				len = म_माप(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				मुक्त(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				जारी;
			}
			अगर (!म_भेद(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				जारी;
			}
			अगर (!म_भेद(arg, "--is-inside-git-dir")) {
				म_लिखो("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				जारी;
			}
			अगर (!म_भेद(arg, "--is-inside-work-tree")) {
				म_लिखो("%s\n", is_inside_work_वृक्ष(the_repository) ? "true"
						: "false");
				जारी;
			}
			अगर (!म_भेद(arg, "--is-bare-repository")) {
				म_लिखो("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				जारी;
			}
			अगर (!म_भेद(arg, "--is-shallow-repository")) {
				म_लिखो("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				जारी;
			}
			अगर (!म_भेद(arg, "--shared-index-path")) {
				अगर (repo_read_सूचकांक(the_repository) < 0)
					die(_("Could not read the index"));
				अगर (the_repository->सूचकांक->split_सूचकांक) {
					स्थिर काष्ठा object_id *oid = &the_repository->सूचकांक->split_सूचकांक->base_oid;
					स्थिर अक्षर *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				जारी;
			}
			अगर (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				जारी;
			}
			अगर (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				जारी;
			}
			अगर (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				जारी;
			}
			अगर (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				जारी;
			}
			अगर (opt_with_मूल्य(arg, "--show-object-format", &arg)) {
				स्थिर अक्षर *val = arg ? arg : "storage";

				अगर (म_भेद(val, "storage") &&
				    म_भेद(val, "compat") &&
				    म_भेद(val, "input") &&
				    म_भेद(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				अगर (!म_भेद(val, "compat")) {
					अगर (the_repository->compat_hash_algo)
						माला_दो(the_repository->compat_hash_algo->name);
					अन्यथा
						अक्षर_दो('\n');
				} अन्यथा {
					माला_दो(the_hash_algo->name);
				}
				जारी;
			}
			अगर (!म_भेद(arg, "--show-ref-format") || !म_भेद(arg, "--show-ref-storage-format")) {
				माला_दो(ref_storage_format_to_name(the_repository->ref_storage_format));
				जारी;
			}
			अगर (!म_भेद(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				अगर (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				जारी;
			}
			अगर (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			जारी;
		}

		/* Not a flag argument */
		अगर (try_difference(arg))
			जारी;
		अगर (try_parent_shorthands(arg))
			जारी;
		name = arg;
		type = NORMAL;
		अगर (*arg == '^') {
			name++;
			type = REVERSED;
		}
		अगर (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			अगर (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			अगर (verify)
				revs_गिनती++;
			अन्यथा
				show_rev(type, &oid, name);
			जारी;
		}
		अगर (verify)
			die_no_single_rev(quiet);
		अगर (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		अगर (!show_file(arg, output_prefix))
			जारी;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	अगर (verify) {
		अगर (revs_गिनती == 1) {
			show_rev(type, &oid, name);
			वापस 0;
		} अन्यथा अगर (revs_गिनती == 0 && show_default())
			वापस 0;
		die_no_single_rev(quiet);
	} अन्यथा
		show_default();
	वापस 0;
}
