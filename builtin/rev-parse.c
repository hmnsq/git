/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

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

#परिभाषय DO_REVS		1
#परिभाषय DO_NOREV	2
#परिभाषय DO_FLAGS	4
#परिभाषय DO_NONFLAGS	8
स्थिर पूर्णाङ्क filter = ~0;

स्थिर ध्रुव वर्ण *def;

#परिभाषय प्रसामान्यम् 0
#परिभाषय REVERSED 1
स्थिर पूर्णाङ्क show_type = प्रसामान्यम्;

#परिभाषय SHOW_SYMBOLIC_ASIS 1
#परिभाषय SHOW_SYMBOLIC_पूर्णम् 2
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
स्थिर पूर्णाङ्क is_rev_तर्कः(ध्रुव वर्ण *arg)
{
	स्थिर ध्रुव वर्ण *rev_तर्काः[] = {
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
	ध्रुव वर्ण **p = rev_तर्काः;

	/* accept -<digit>, like traditional "head" */
	यदि ((*arg == '-') && अङ्कः_वा(arg[1]))
		प्रत्यागम 1;

	प्रति (;;) {
		ध्रुव वर्ण *str = *p++;
		पूर्णाङ्क len;
		यदि (!str)
			प्रत्यागम 0;
		len = वाक्यदैर्घ्यम्(str);
		यदि (!वाक्यं_तुलय(arg, str) ||
		    (str[len-1] == '=' && !वाक्ये_तुलय(arg, str, len)))
			प्रत्यागम 1;
	}
}

/* Output argument as a string, either SQ or normal */
स्थिर शून्य show(ध्रुव वर्ण *arg)
{
	यदि (output_sq) {
		पूर्णाङ्क sq = '\'', ch;

		अक्षरं_लिख(sq);
		यावत् ((ch = *arg++)) {
			यदि (ch == sq)
				fputs("'\\'", stdout);
			अक्षरं_लिख(ch);
		}
		अक्षरं_लिख(sq);
		अक्षरं_लिख(' ');
	}
	अन्यथा
		पङ्क्तिं_लिख(arg);
}

/* Like show(), but with a negation prefix according to type */
स्थिर शून्य show_with_type(पूर्णाङ्क type, ध्रुव वर्ण *arg)
{
	यदि (type != show_type)
		अक्षरं_लिख('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
स्थिर शून्य show_rev(पूर्णाङ्क type, ध्रुव संरचना object_id *oid, ध्रुव वर्ण *name)
{
	यदि (!(filter & DO_REVS))
		प्रत्यागम;
	def = रिक्त;

	यदि ((symbolic || abbrev_ref) && name) {
		यदि (symbolic == SHOW_SYMBOLIC_पूर्णम् || abbrev_ref) {
			संरचना object_id discard;
			वर्ण *पूर्णम्;

			विकल्प (repo_dwim_ref(the_repository, name,
					      वाक्यदैर्घ्यम्(name), &discard, &पूर्णम्,
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
					वर्ण *old = पूर्णम्;
					पूर्णम् = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    पूर्णम्,
									    abbrev_ref_strict);
					मुक्त(old);
				}
				show_with_type(type, पूर्णम्);
				भङ्ग;
			स्वभाव: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				भङ्ग;
			}
			मुक्त(पूर्णम्);
		} अन्यथा {
			show_with_type(type, name);
		}
	}
	अन्यथा यदि (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	अन्यथा
		show_with_type(type, oid_गन्तव्यम्_hex(oid));
}

/* Output a flag, only if filter allows it. */
स्थिर पूर्णाङ्क show_flag(ध्रुव वर्ण *arg)
{
	यदि (!(filter & DO_FLAGS))
		प्रत्यागम 0;
	यदि (filter & (is_rev_तर्कः(arg) ? DO_REVS : DO_NOREV)) {
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
			show_rev(प्रसामान्यम्, &oid, s);
			प्रत्यागम 1;
		}
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_reference(ध्रुव संरचना reference *ref, शून्य *cb_दत्त अप्रयुक्तम्)
{
	यदि (ref_excluded(&ref_excludes, ref->name))
		प्रत्यागम 0;
	show_rev(प्रसामान्यम्, ref->oid, ref->name);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क anti_reference(ध्रुव संरचना reference *ref, शून्य *cb_दत्त अप्रयुक्तम्)
{
	show_rev(REVERSED, ref->oid, ref->name);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show_abbrev(ध्रुव संरचना object_id *oid, शून्य *cb_दत्त अप्रयुक्तम्)
{
	show_rev(प्रसामान्यम्, oid, रिक्त);
	प्रत्यागम 0;
}

स्थिर शून्य show_datestring(ध्रुव वर्ण *flag, ध्रुव वर्ण *datestr)
{
	वर्ण *बफर;

	/* date handling requires both flags and revs */
	यदि ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		प्रत्यागम;
	बफर = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(बफर);
	मुक्त(बफर);
}

स्थिर पूर्णाङ्क show_file(ध्रुव वर्ण *arg, पूर्णाङ्क output_उपसर्गः)
{
	show_default();
	यदि ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		यदि (output_उपसर्गः) {
			ध्रुव वर्ण *उपसर्गः = the_repository->उपसर्गः;
			वर्ण *fname = उपसर्गः_filename(उपसर्गः, arg);
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
	संरचना object_id अन्तः_oid;
	ध्रुव वर्ण *अन्तः;
	ध्रुव वर्ण *start;
	वर्ण *गन्तव्यम्_free;
	पूर्णाङ्क सममितम्;
	स्थिर ध्रुव वर्ण शीर्ष_by_default[] = "HEAD";

	यदि (!(dotdot = वाक्ये_अन्विष(arg, "..")))
		प्रत्यागम 0;
	start = गन्तव्यम्_free = xmemdupz(arg, dotdot - arg);
	अन्तः = dotdot + 2;
	सममितम् = (*अन्तः == '.');
	अन्तः += सममितम्;

	यदि (!*अन्तः)
		अन्तः = शीर्ष_by_default;
	यदि (dotdot == arg)
		start = शीर्ष_by_default;

	यदि (start == शीर्ष_by_default && अन्तः == शीर्ष_by_default &&
	    !सममितम्) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		मुक्त(गन्तव्यम्_free);
		प्रत्यागम 0;
	}

	यदि (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, अन्तः, &अन्तः_oid)) {
		show_rev(प्रसामान्यम्, &अन्तः_oid, अन्तः);
		show_rev(सममितम् ? प्रसामान्यम् : REVERSED, &start_oid, start);
		यदि (सममितम्) {
			संरचना commit_list *exclude = रिक्त;
			संरचना commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &अन्तः_oid);
			यदि (!a || !b) {
				मुक्त(गन्तव्यम्_free);
				प्रत्यागम 0;
			}
			यदि (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				निर्गच्छ(128);
			यावत् (exclude) {
				संरचना commit *commit = निष्कासय_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, रिक्त);
			}
		}
		मुक्त(गन्तव्यम्_free);
		प्रत्यागम 1;
	}
	मुक्त(गन्तव्यम्_free);
	प्रत्यागम 0;
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
	वर्ण *गन्तव्यम्_free;

	यदि ((mark = वाक्ये_अन्विष(arg, "^!"))) {
		include_rev = 1;
		यदि (mark[2])
			प्रत्यागम 0;
	} अन्यथा यदि ((mark = वाक्ये_अन्विष(arg, "^@"))) {
		include_parents = 1;
		यदि (mark[2])
			प्रत्यागम 0;
	} अन्यथा यदि ((mark = वाक्ये_अन्विष(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		यदि (mark[2]) {
			वर्ण *अन्तः;
			exclude_parent = strtoul(mark + 2, &अन्तः, 10);
			यदि (*अन्तः != '\0' || !exclude_parent)
				प्रत्यागम 0;
		}
	} अन्यथा
		प्रत्यागम 0;

	arg = गन्तव्यम्_free = xmemdupz(arg, mark - arg);
	यदि (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		मुक्त(गन्तव्यम्_free);
		प्रत्यागम 0;
	}

	यदि (exclude_parent &&
	    exclude_parent > commit_list_गणनम्(commit->parents)) {
		मुक्त(गन्तव्यम्_free);
		प्रत्यागम 0;
	}

	यदि (include_rev)
		show_rev(प्रसामान्यम्, &oid, arg);
	प्रति (parents = commit->parents, parent_सङ्ख्या = 1;
	     parents;
	     parents = parents->अग्र, parent_सङ्ख्या++) {
		वर्ण *name = रिक्त;

		यदि (exclude_parent && parent_सङ्ख्या != exclude_parent)
			अनुवर्त;

		यदि (symbolic)
			name = xstrfmt("%s^%d", arg, parent_सङ्ख्या);
		show_rev(include_parents ? प्रसामान्यम् : REVERSED,
			 &parents->item->object.oid, name);
		मुक्त(name);
	}

	मुक्त(गन्तव्यम्_free);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क parseopt_dump(ध्रुव संरचना option *o, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना strbuf *parsed = o->मानम्;
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
	यावत् (रिक्तम्_वा(*s))
		s++;
	प्रत्यागम s;
}

स्थिर वर्ण *findspace(ध्रुव वर्ण *s)
{
	प्रति (; *s; s++)
		यदि (रिक्तम्_वा(*s))
			प्रत्यागम (वर्ण*)s;
	प्रत्यागम रिक्त;
}

स्थिर पूर्णाङ्क cmd_parseopt(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः)
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
		OPT_अन्तः(),
	};
	संरचना strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	संरचना strvec longnames = STRVEC_INIT;
	संरचना strvec usage = STRVEC_INIT;
	संरचना option *opts = रिक्त;
	आकार_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, उपसर्गः, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	यदि (argc < 1 || वाक्यं_तुलय(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	प्रति (;;) {
		strbuf_reset(&sb);
		यदि (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		यदि (!वाक्यं_तुलय("--", sb.buf)) {
			यदि (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			भङ्ग;
		}

		strvec_प्रक्षिप(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	यावत् (strbuf_getline(&sb, stdin) != EOF) {
		ध्रुव वर्ण *s;
		वर्ण *help;
		संरचना option *o;

		यदि (!sb.len)
			अनुवर्त;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		स्मृतिं_पूरय(opts + opts_nr, 0, परिमाण(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		यदि (!help || sb.buf == help) {
			o->type = OPTION_गणः;
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
	स्मृतिं_पूरय(opts + opts_nr, 0, परिमाण(*opts));
	argc = parse_options(argc, argv, उपसर्गः, opts, usage.v,
			(keep_dashdash ? PARSE_OPT_KEEP_DASHDASH : 0) |
			(stop_at_non_option ? PARSE_OPT_STOP_AT_NON_OPTION : 0) |
			PARSE_OPT_SHELL_EVAL);

	strbuf_addstr(&parsed, " --");
	sq_quote_argv(&parsed, argv);
	पङ्क्तिं_लिख(parsed.buf);

	strbuf_release(&parsed);
	strbuf_release(&sb);
	strvec_clear(&longnames);
	strvec_clear(&usage);
	प्रति (आकार_t i = 0; i < opts_nr; i++) {
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
	प्रारूप_लिख("%s\n", buf.buf);
	strbuf_release(&buf);

	प्रत्यागम 0;
}

स्थिर शून्य die_no_single_rev(पूर्णाङ्क quiet)
{
	यदि (quiet)
		निर्गच्छ(1);
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
स्थिर पूर्णाङ्क opt_with_मानम्(ध्रुव वर्ण *arg, ध्रुव वर्ण *opt, ध्रुव वर्ण **मानम्)
{
	यदि (skip_उपसर्गः(arg, opt, &arg)) {
		यदि (!*arg) {
			*मानम् = रिक्त;
			प्रत्यागम 1;
		}
		यदि (*arg++ == '=') {
			*मानम् = arg;
			प्रत्यागम 1;
		}
	}
	प्रत्यागम 0;
}

स्थिर शून्य handle_ref_opt(ध्रुव वर्ण *आदर्शः, ध्रुव वर्ण *उपसर्गः)
{
	यदि (आदर्शः) {
		संरचना refs_for_each_ref_options opts = {
			.आदर्शः = आदर्शः,
			.उपसर्गः = उपसर्गः,
			.trim_उपसर्गः = उपसर्गः ? वाक्यदैर्घ्यम्(उपसर्गः) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, रिक्त, &opts);
	} अन्यथा {
		संरचना refs_for_each_ref_options opts = {
			.उपसर्गः = उपसर्गः,
			.trim_उपसर्गः = वाक्यदैर्घ्यम्(उपसर्गः),
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

स्थिर शून्य print_मार्गः(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *उपसर्गः,
		       गणना format_type format, गणना default_type def)
{
	संरचना strbuf sb = STRBUF_INIT;
	गणना मार्गः_format fmt;

	यदि (format == FORMAT_DEFAULT) {
		विकल्प (def) {
		पक्ष DEFAULT_RELATIVE:
			fmt = मार्गः_FORMAT_RELATIVE;
			भङ्ग;
		पक्ष DEFAULT_RELATIVE_IF_SHARED:
			fmt = मार्गः_FORMAT_RELATIVE_IF_SHARED;
			भङ्ग;
		पक्ष DEFAULT_CANONICAL:
			fmt = मार्गः_FORMAT_CANONICAL;
			भङ्ग;
		पक्ष DEFAULT_UNMODIFIED:
		स्वभाव:
			fmt = मार्गः_FORMAT_UNMODIFIED;
			भङ्ग;
		}
	} अन्यथा {
		विकल्प (format) {
		पक्ष FORMAT_RELATIVE:
			fmt = मार्गः_FORMAT_RELATIVE;
			भङ्ग;
		पक्ष FORMAT_CANONICAL:
			fmt = मार्गः_FORMAT_CANONICAL;
			भङ्ग;
		स्वभाव:
			fmt = मार्गः_FORMAT_UNMODIFIED;
			भङ्ग;
		}
	}

	format_मार्गः(&sb, मार्गः, उपसर्गः, fmt);
	पङ्क्तिं_लिख(sb.buf);

	strbuf_release(&sb);
}

पूर्णाङ्क cmd_rev_parse(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *उपसर्गः,
		  संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क i, as_is = 0, verify = 0, quiet = 0, revs_गणनम् = 0, type = 0;
	ध्रुव संरचना git_hash_algo *output_algo = रिक्त;
	ध्रुव संरचना git_hash_algo *compat = रिक्त;
	पूर्णाङ्क did_repo_सज्जता = 0;
	पूर्णाङ्क has_dashdash = 0;
	पूर्णाङ्क output_उपसर्गः = 0;
	संरचना object_id oid;
	अचिह्नित पूर्णाङ्क flags = 0;
	ध्रुव वर्ण *name = रिक्त;
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णाङ्क आलोकितम्_अन्तः_सम्बन्धि_options = 0;
	गणना format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	यदि (argc > 1 && !वाक्यं_तुलय("--parseopt", argv[1]))
		प्रत्यागम cmd_parseopt(argc - 1, argv + 1, उपसर्गः);

	यदि (argc > 1 && !वाक्यं_तुलय("--sq-quote", argv[1]))
		प्रत्यागम cmd_sq_quote(argc - 2, argv + 2);

	यदि (argc > 1 && !वाक्यं_तुलय("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	प्रति (i = 1; i < argc; i++) {
		यदि (!वाक्यं_तुलय(argv[i], "--")) {
			has_dashdash = 1;
			भङ्ग;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	यदि (argc == 1) {
		सज्जता_git_directory(the_repository);
		repo_config(the_repository, git_default_config, रिक्त);
		प्रत्यागम 0;
	}

	प्रति (i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];

		यदि (as_is) {
			यदि (show_file(arg, output_उपसर्गः) && as_is < 2)
				verify_filename(the_repository, उपसर्गः, arg, 0);
			अनुवर्त;
		}

		यदि (!आलोकितम्_अन्तः_सम्बन्धि_options) {
			यदि (!वाक्यं_तुलय(arg, "--local-env-vars")) {
				पूर्णाङ्क i;
				प्रति (i = 0; local_repo_env[i]; i++)
					प्रारूप_लिख("%s\n", local_repo_env[i]);
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--resolve-git-dir")) {
				ध्रुव वर्ण *gitdir = argv[++i];
				यदि (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				यदि (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				पङ्क्तिं_लिख(gitdir);
				अनुवर्त;
			}
		}

		/* The rest of the options require a git repository. */
		यदि (!did_repo_सज्जता) {
			उपसर्गः = सज्जता_git_directory(the_repository);
			repo_config(the_repository, git_default_config, रिक्त);
			did_repo_सज्जता = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;
			compat = the_repository->compat_hash_algo;
		}

		यदि (!वाक्यं_तुलय(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			यदि (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			अनुवर्त;
		}

		यदि (!आलोकितम्_अन्तः_सम्बन्धि_options && *arg == '-') {
			यदि (!वाक्यं_तुलय(arg, "--git-path")) {
				यदि (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_मार्गः(repo_git_मार्गः_replace(the_repository, &buf,
								 "%s", argv[i + 1]), उपसर्गः,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg,"-n")) {
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
			यदि (opt_with_मानम्(arg, "--path-format", &arg)) {
				यदि (!arg)
					die(_("--path-format requires an argument"));
				यदि (!वाक्यं_तुलय(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} अन्यथा यदि (!वाक्यं_तुलय(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} अन्यथा {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--default")) {
				def = argv[++i];
				यदि (!def)
					die(_("--default requires an argument"));
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--prefix")) {
				उपसर्गः = argv[++i];
				यदि (!उपसर्गः)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->उपसर्गः);
				the_repository->उपसर्गः = xstrdup(उपसर्गः);
				output_उपसर्गः = 1;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--quiet") || !वाक्यं_तुलय(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				अनुवर्त;
			}
			यदि (opt_with_मानम्(arg, "--output-object-format", &arg)) {
				यदि (!arg)
					die(_("no object format specified"));
				यदि (!वाक्यं_तुलय(arg, the_hash_algo->name) ||
				    !वाक्यं_तुलय(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					अनुवर्त;
				}
				अन्यथा यदि (compat && !वाक्यं_तुलय(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					अनुवर्त;
				}
				अन्यथा die(_("unsupported object format: %s"), arg);
			}
			यदि (opt_with_मानम्(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				यदि (!arg)
					अनुवर्त;
				abbrev = strtoul(arg, रिक्त, 10);
				यदि (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				अन्यथा यदि ((पूर्णाङ्क)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--sq")) {
				output_sq = 1;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--not")) {
				show_type ^= REVERSED;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_पूर्णम्;
				अनुवर्त;
			}
			यदि (opt_with_मानम्(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				यदि (arg) {
					यदि (!वाक्यं_तुलय(arg, "strict"))
						abbrev_ref_strict = 1;
					अन्यथा यदि (!वाक्यं_तुलय(arg, "loose"))
						abbrev_ref_strict = 0;
					अन्यथा
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, रिक्त);
				clear_ref_exclusions(&ref_excludes);
				अनुवर्त;
			}
			यदि (skip_उपसर्गः(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, रिक्त);
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--bisect")) {
				वर्ण *उपसर्गः;
				वर्ण *पदांशः_bad = रिक्त;
				वर्ण *पदांशः_good = रिक्त;
				संरचना refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&पदांशः_bad, &पदांशः_good);
				उपसर्गः = xstrfmt("refs/bisect/%s", पदांशः_bad);
				opts.उपसर्गः = उपसर्गः;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, रिक्त, &opts);
				मुक्त(उपसर्गः);
				उपसर्गः = xstrfmt("refs/bisect/%s", पदांशः_good);
				opts.उपसर्गः = उपसर्गः;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, रिक्त, &opts);
				मुक्त(उपसर्गः);
				मुक्त(पदांशः_good);
				मुक्त(पदांशः_bad);
				अनुवर्त;
			}
			यदि (opt_with_मानम्(arg, "--branches", &arg)) {
				यदि (ref_excludes.hidden_refs_configured)
					प्रत्यागम error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				अनुवर्त;
			}
			यदि (opt_with_मानम्(arg, "--tags", &arg)) {
				यदि (ref_excludes.hidden_refs_configured)
					प्रत्यागम error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				अनुवर्त;
			}
			यदि (skip_उपसर्गः(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, रिक्त);
				अनुवर्त;
			}
			यदि (opt_with_मानम्(arg, "--remotes", &arg)) {
				यदि (ref_excludes.hidden_refs_configured)
					प्रत्यागम error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				अनुवर्त;
			}
			यदि (skip_उपसर्गः(arg, "--exclude=", &arg)) {
				सङ्कलय_ref_exclusion(&ref_excludes, arg);
				अनुवर्त;
			}
			यदि (skip_उपसर्गः(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--show-toplevel")) {
				ध्रुव वर्ण *work_वृक्षः = repo_get_work_वृक्षः(the_repository);
				यदि (work_वृक्षः)
					print_मार्गः(work_वृक्षः, उपसर्गः, format, DEFAULT_UNMODIFIED);
				अन्यथा
					die(_("this operation must be run in a work tree"));
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--show-superproject-working-tree")) {
				संरचना strbuf superproject = STRBUF_INIT;
				यदि (get_superproject_working_वृक्षः(&superproject))
					print_मार्गः(superproject.buf, उपसर्गः, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--show-prefix")) {
				यदि (उपसर्गः)
					पङ्क्तिं_लिख(उपसर्गः);
				अन्यथा
					अक्षरं_लिख('\n');
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--show-cdup")) {
				ध्रुव वर्ण *pfx = उपसर्गः;
				यदि (!is_inside_work_वृक्षः(the_repository)) {
					ध्रुव वर्ण *work_वृक्षः =
						repo_get_work_वृक्षः(the_repository);
					यदि (work_वृक्षः)
						प्रारूप_लिख("%s\n", work_वृक्षः);
					अनुवर्त;
				}
				यावत् (pfx) {
					pfx = strchr(pfx, '/');
					यदि (pfx) {
						pfx++;
						प्रारूप_लिख("../");
					}
				}
				अक्षरं_लिख('\n');
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--git-dir") ||
			    !वाक्यं_तुलय(arg, "--absolute-git-dir")) {
				ध्रुव वर्ण *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				वर्ण *cwd;
				पूर्णाङ्क len;
				गणना format_type wanted = format;
				यदि (arg[2] == 'g') {	/* --git-dir */
					यदि (gitdir) {
						print_मार्गः(gitdir, उपसर्गः, format, DEFAULT_UNMODIFIED);
						अनुवर्त;
					}
					यदि (!उपसर्गः) {
						print_मार्गः(".git", उपसर्गः, format, DEFAULT_UNMODIFIED);
						अनुवर्त;
					}
				} अन्यथा {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					यदि (!gitdir && !उपसर्गः)
						gitdir = ".git";
					यदि (gitdir) {
						संरचना strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						पङ्क्तिं_लिख(realpath.buf);
						strbuf_release(&realpath);
						अनुवर्त;
					}
				}
				cwd = xgetcwd();
				len = वाक्यदैर्घ्यम्(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				मुक्त(cwd);
				print_मार्गः(buf.buf, उपसर्गः, wanted, DEFAULT_CANONICAL);
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--git-common-dir")) {
				print_मार्गः(repo_get_common_dir(the_repository), उपसर्गः, format, DEFAULT_RELATIVE_IF_SHARED);
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--is-inside-git-dir")) {
				प्रारूप_लिख("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--is-inside-work-tree")) {
				प्रारूप_लिख("%s\n", is_inside_work_वृक्षः(the_repository) ? "true"
						: "false");
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--is-bare-repository")) {
				प्रारूप_लिख("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--is-shallow-repository")) {
				प्रारूप_लिख("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--shared-index-path")) {
				यदि (repo_read_सूचकांक(the_repository) < 0)
					die(_("Could not read the index"));
				यदि (the_repository->सूचकांक->विभज_सूचकांक) {
					ध्रुव संरचना object_id *oid = &the_repository->सूचकांक->विभज_सूचकांक->आधारः_oid;
					ध्रुव वर्ण *मार्गः = repo_git_मार्गः_replace(the_repository, &buf, "sharedindex.%s", oid_गन्तव्यम्_hex(oid));
					print_मार्गः(मार्गः, उपसर्गः, format, DEFAULT_RELATIVE);
				}
				अनुवर्त;
			}
			यदि (skip_उपसर्गः(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				अनुवर्त;
			}
			यदि (skip_उपसर्गः(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				अनुवर्त;
			}
			यदि (skip_उपसर्गः(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				अनुवर्त;
			}
			यदि (skip_उपसर्गः(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				अनुवर्त;
			}
			यदि (opt_with_मानम्(arg, "--show-object-format", &arg)) {
				ध्रुव वर्ण *val = arg ? arg : "storage";

				यदि (वाक्यं_तुलय(val, "storage") &&
				    वाक्यं_तुलय(val, "compat") &&
				    वाक्यं_तुलय(val, "input") &&
				    वाक्यं_तुलय(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				यदि (!वाक्यं_तुलय(val, "compat")) {
					यदि (the_repository->compat_hash_algo)
						पङ्क्तिं_लिख(the_repository->compat_hash_algo->name);
					अन्यथा
						अक्षरं_लिख('\n');
				} अन्यथा {
					पङ्क्तिं_लिख(the_hash_algo->name);
				}
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--show-ref-format") || !वाक्यं_तुलय(arg, "--show-ref-storage-format")) {
				पङ्क्तिं_लिख(ref_storage_format_गन्तव्यम्_name(the_repository->ref_storage_format));
				अनुवर्त;
			}
			यदि (!वाक्यं_तुलय(arg, "--end-of-options")) {
				आलोकितम्_अन्तः_सम्बन्धि_options = 1;
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
		type = प्रसामान्यम्;
		यदि (*arg == '^') {
			name++;
			type = REVERSED;
		}
		यदि (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			यदि (output_algo)
				repo_oid_गन्तव्यम्_algop(the_repository, &oid,
						  output_algo, &oid);
			यदि (verify)
				revs_गणनम्++;
			अन्यथा
				show_rev(type, &oid, name);
			अनुवर्त;
		}
		यदि (verify)
			die_no_single_rev(quiet);
		यदि (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		यदि (!show_file(arg, output_उपसर्गः))
			अनुवर्त;
		verify_filename(the_repository, उपसर्गः, arg, 1);
	}
	strbuf_release(&buf);
	यदि (verify) {
		यदि (revs_गणनम् == 1) {
			show_rev(type, &oid, name);
			प्रत्यागम 0;
		} अन्यथा यदि (revs_गणनम् == 0 && show_default())
			प्रत्यागम 0;
		die_no_single_rev(quiet);
	} अन्यथा
		show_default();
	प्रत्यागम 0;
}
