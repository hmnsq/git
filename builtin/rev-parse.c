/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"

#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "bisect.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "repo-settings.h"
#അന്തര്ഭാവ "repository.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "split-index.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "commit-reach.h"
#അന്തര്ഭാവ "shallow.h"
#അന്തര്ഭാവ "object-file-convert.h"

#define DO_REVS		1
#define DO_NOREV	2
#define DO_FLAGS	4
#define DO_NONFLAGS	8
സ്ഥിതം പൂർണ്ണസംഖ്യ filter = ~0;

സ്ഥിതം സ്ഥിരം അക്ഷരം *def;

#define NORMAL 0
#define REVERSED 1
സ്ഥിതം പൂർണ്ണസംഖ്യ show_type = NORMAL;

#define SHOW_SYMBOLIC_ASIS 1
#define SHOW_SYMBOLIC_FULL 2
സ്ഥിതം പൂർണ്ണസംഖ്യ symbolic;
സ്ഥിതം പൂർണ്ണസംഖ്യ abbrev;
സ്ഥിതം പൂർണ്ണസംഖ്യ abbrev_ref;
സ്ഥിതം പൂർണ്ണസംഖ്യ abbrev_ref_strict;
സ്ഥിതം പൂർണ്ണസംഖ്യ output_sq;

സ്ഥിതം പൂർണ്ണസംഖ്യ stuck_long;
സ്ഥിതം ഘടന ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ is_rev_argument(സ്ഥിരം അക്ഷരം *arg)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം *rev_args[] = {
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
		ശൂന്യ
	};
	സ്ഥിരം അക്ഷരം **p = rev_args;

	/* accept -<digit>, like traditional "head" */
	എങ്കിൽ ((*arg == '-') && isdigit(arg[1]))
		തിരികെ 1;

	വേണ്ടി (;;) {
		സ്ഥിരം അക്ഷരം *str = *p++;
		പൂർണ്ണസംഖ്യ len;
		എങ്കിൽ (!str)
			തിരികെ 0;
		len = strlen(str);
		എങ്കിൽ (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !strncmp(arg, str, len)))
			തിരികെ 1;
	}
}

/* Output argument as a string, either SQ or normal */
സ്ഥിതം ശൂന്യം show(സ്ഥിരം അക്ഷരം *arg)
{
	എങ്കിൽ (output_sq) {
		പൂർണ്ണസംഖ്യ sq = '\'', ch;

		putchar(sq);
		ആകുമ്പോൾ ((ch = *arg++)) {
			എങ്കിൽ (ch == sq)
				fputs("'\\'", stdout);
			putchar(ch);
		}
		putchar(sq);
		putchar(' ');
	}
	അല്ലെങ്കിൽ
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
സ്ഥിതം ശൂന്യം show_with_type(പൂർണ്ണസംഖ്യ type, സ്ഥിരം അക്ഷരം *arg)
{
	എങ്കിൽ (type != show_type)
		putchar('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
സ്ഥിതം ശൂന്യം show_rev(പൂർണ്ണസംഖ്യ type, സ്ഥിരം ഘടന object_id *oid, സ്ഥിരം അക്ഷരം *name)
{
	എങ്കിൽ (!(filter & DO_REVS))
		തിരികെ;
	def = ശൂന്യ;

	എങ്കിൽ ((symbolic || abbrev_ref) && name) {
		എങ്കിൽ (symbolic == SHOW_SYMBOLIC_FULL || abbrev_ref) {
			ഘടന object_id discard;
			അക്ഷരം *full;

			തിരഞ്ഞെടുപ്പ് (repo_dwim_ref(the_repository, name,
					      strlen(name), &discard, &full,
					      0)) {
			അവസ്ഥ 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				നിർത്തുക;
			അവസ്ഥ 1: /* happy */
				എങ്കിൽ (abbrev_ref) {
					അക്ഷരം *old = full;
					full = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    full,
									    abbrev_ref_strict);
					സ്വതന്ത്രം(old);
				}
				show_with_type(type, full);
				നിർത്തുക;
			സ്ഥിരസ്ഥിതി: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				നിർത്തുക;
			}
			സ്വതന്ത്രം(full);
		} അല്ലെങ്കിൽ {
			show_with_type(type, name);
		}
	}
	അല്ലെങ്കിൽ എങ്കിൽ (abbrev)
		show_with_type(type,
			       repo_find_unique_abbrev(the_repository, oid, abbrev));
	അല്ലെങ്കിൽ
		show_with_type(type, oid_to_hex(oid));
}

/* Output a flag, only if filter allows it. */
സ്ഥിതം പൂർണ്ണസംഖ്യ show_flag(സ്ഥിരം അക്ഷരം *arg)
{
	എങ്കിൽ (!(filter & DO_FLAGS))
		തിരികെ 0;
	എങ്കിൽ (filter & (is_rev_argument(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		തിരികെ 1;
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_default(ശൂന്യം)
{
	സ്ഥിരം അക്ഷരം *s = def;

	എങ്കിൽ (s) {
		ഘടന object_id oid;

		def = ശൂന്യ;
		എങ്കിൽ (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(NORMAL, &oid, s);
			തിരികെ 1;
		}
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_reference(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data UNUSED)
{
	എങ്കിൽ (ref_excluded(&ref_excludes, ref->name))
		തിരികെ 0;
	show_rev(NORMAL, ref->oid, ref->name);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ anti_reference(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data UNUSED)
{
	show_rev(REVERSED, ref->oid, ref->name);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_abbrev(സ്ഥിരം ഘടന object_id *oid, ശൂന്യം *cb_data UNUSED)
{
	show_rev(NORMAL, oid, ശൂന്യ);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം show_datestring(സ്ഥിരം അക്ഷരം *flag, സ്ഥിരം അക്ഷരം *datestr)
{
	അക്ഷരം *buffer;

	/* date handling requires both flags and revs */
	എങ്കിൽ ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		തിരികെ;
	buffer = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(buffer);
	സ്വതന്ത്രം(buffer);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_file(സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ output_prefix)
{
	show_default();
	എങ്കിൽ ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		എങ്കിൽ (output_prefix) {
			സ്ഥിരം അക്ഷരം *prefix = the_repository->prefix;
			അക്ഷരം *fname = prefix_filename(prefix, arg);
			show(fname);
			സ്വതന്ത്രം(fname);
		} അല്ലെങ്കിൽ
			show(arg);
		തിരികെ 1;
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ try_difference(സ്ഥിരം അക്ഷരം *arg)
{
	സ്ഥിരം അക്ഷരം *dotdot;
	ഘടന object_id start_oid;
	ഘടന object_id end_oid;
	സ്ഥിരം അക്ഷരം *end;
	സ്ഥിരം അക്ഷരം *start;
	അക്ഷരം *to_free;
	പൂർണ്ണസംഖ്യ symmetric;
	സ്ഥിതം സ്ഥിരം അക്ഷരം head_by_default[] = "HEAD";

	എങ്കിൽ (!(dotdot = strstr(arg, "..")))
		തിരികെ 0;
	start = to_free = xmemdupz(arg, dotdot - arg);
	end = dotdot + 2;
	symmetric = (*end == '.');
	end += symmetric;

	എങ്കിൽ (!*end)
		end = head_by_default;
	എങ്കിൽ (dotdot == arg)
		start = head_by_default;

	എങ്കിൽ (start == head_by_default && end == head_by_default &&
	    !symmetric) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		സ്വതന്ത്രം(to_free);
		തിരികെ 0;
	}

	എങ്കിൽ (!repo_get_oid_committish(the_repository, start, &start_oid) && !repo_get_oid_committish(the_repository, end, &end_oid)) {
		show_rev(NORMAL, &end_oid, end);
		show_rev(symmetric ? NORMAL : REVERSED, &start_oid, start);
		എങ്കിൽ (symmetric) {
			ഘടന commit_list *exclude = ശൂന്യ;
			ഘടന commit *a, *b;
			a = lookup_commit_reference(the_repository, &start_oid);
			b = lookup_commit_reference(the_repository, &end_oid);
			എങ്കിൽ (!a || !b) {
				സ്വതന്ത്രം(to_free);
				തിരികെ 0;
			}
			എങ്കിൽ (repo_get_merge_bases(the_repository, a, b, &exclude) < 0)
				exit(128);
			ആകുമ്പോൾ (exclude) {
				ഘടന commit *commit = എടുക്കുക_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, ശൂന്യ);
			}
		}
		സ്വതന്ത്രം(to_free);
		തിരികെ 1;
	}
	സ്വതന്ത്രം(to_free);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ try_parent_shorthands(സ്ഥിരം അക്ഷരം *arg)
{
	സ്ഥിരം അക്ഷരം *mark;
	ഘടന object_id oid;
	ഘടന commit *commit;
	ഘടന commit_list *parents;
	പൂർണ്ണസംഖ്യ parent_സംഖ്യ;
	പൂർണ്ണസംഖ്യ include_rev = 0;
	പൂർണ്ണസംഖ്യ include_parents = 0;
	പൂർണ്ണസംഖ്യ exclude_parent = 0;
	അക്ഷരം *to_free;

	എങ്കിൽ ((mark = strstr(arg, "^!"))) {
		include_rev = 1;
		എങ്കിൽ (mark[2])
			തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ ((mark = strstr(arg, "^@"))) {
		include_parents = 1;
		എങ്കിൽ (mark[2])
			തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ ((mark = strstr(arg, "^-"))) {
		include_rev = 1;
		exclude_parent = 1;

		എങ്കിൽ (mark[2]) {
			അക്ഷരം *end;
			exclude_parent = strtoul(mark + 2, &end, 10);
			എങ്കിൽ (*end != '\0' || !exclude_parent)
				തിരികെ 0;
		}
	} അല്ലെങ്കിൽ
		തിരികെ 0;

	arg = to_free = xmemdupz(arg, mark - arg);
	എങ്കിൽ (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		സ്വതന്ത്രം(to_free);
		തിരികെ 0;
	}

	എങ്കിൽ (exclude_parent &&
	    exclude_parent > commit_list_എണ്ണം(commit->parents)) {
		സ്വതന്ത്രം(to_free);
		തിരികെ 0;
	}

	എങ്കിൽ (include_rev)
		show_rev(NORMAL, &oid, arg);
	വേണ്ടി (parents = commit->parents, parent_സംഖ്യ = 1;
	     parents;
	     parents = parents->next, parent_സംഖ്യ++) {
		അക്ഷരം *name = ശൂന്യ;

		എങ്കിൽ (exclude_parent && parent_സംഖ്യ != exclude_parent)
			തുടരുക;

		എങ്കിൽ (symbolic)
			name = xstrfmt("%s^%d", arg, parent_സംഖ്യ);
		show_rev(include_parents ? NORMAL : REVERSED,
			 &parents->item->object.oid, name);
		സ്വതന്ത്രം(name);
	}

	സ്വതന്ത്രം(to_free);
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parseopt_dump(സ്ഥിരം ഘടന option *o, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന strbuf *parsed = o->മൂല്യം;
	എങ്കിൽ (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	അല്ലെങ്കിൽ എങ്കിൽ (o->short_name && (o->long_name == ശൂന്യ || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	അല്ലെങ്കിൽ
		strbuf_addf(parsed, " --%s", o->long_name);
	എങ്കിൽ (arg) {
		എങ്കിൽ (!stuck_long)
			strbuf_addch(parsed, ' ');
		അല്ലെങ്കിൽ എങ്കിൽ (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *skipspaces(സ്ഥിരം അക്ഷരം *s)
{
	ആകുമ്പോൾ (isspace(*s))
		s++;
	തിരികെ s;
}

സ്ഥിതം അക്ഷരം *findspace(സ്ഥിരം അക്ഷരം *s)
{
	വേണ്ടി (; *s; s++)
		എങ്കിൽ (isspace(*s))
			തിരികെ (അക്ഷരം*)s;
	തിരികെ ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_parseopt(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix)
{
	പൂർണ്ണസംഖ്യ keep_dashdash = 0, stop_at_non_option = 0;
	അക്ഷരം സ്ഥിരം * സ്ഥിരം parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		ശൂന്യ
	};
	ഘടന option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_END(),
	};
	ഘടന strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	ഘടന strvec longnames = STRVEC_INIT;
	ഘടന strvec usage = STRVEC_INIT;
	ഘടന option *opts = ശൂന്യ;
	size_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, prefix, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	എങ്കിൽ (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	വേണ്ടി (;;) {
		strbuf_reset(&sb);
		എങ്കിൽ (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		എങ്കിൽ (!strcmp("--", sb.buf)) {
			എങ്കിൽ (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			നിർത്തുക;
		}

		strvec_തള്ളുക(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	ആകുമ്പോൾ (strbuf_getline(&sb, stdin) != EOF) {
		സ്ഥിരം അക്ഷരം *s;
		അക്ഷരം *help;
		ഘടന option *o;

		എങ്കിൽ (!sb.len)
			തുടരുക;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, വലുപ്പം(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		എങ്കിൽ (!help || sb.buf == help) {
			o->type = OPTION_GROUP;
			o->help = xstrdup(skipspaces(sb.buf));
			തുടരുക;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->മൂല്യം = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		എങ്കിൽ (!s)
			s = help;

		എങ്കിൽ (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		എങ്കിൽ (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} അല്ലെങ്കിൽ എങ്കിൽ (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (പൂർണ്ണസംഖ്യ)(s - sb.buf), sb.buf);
		} അല്ലെങ്കിൽ {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (പൂർണ്ണസംഖ്യ)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		ആകുമ്പോൾ (s < help) {
			തിരഞ്ഞെടുപ്പ് (*s++) {
			അവസ്ഥ '=':
				o->flags &= ~PARSE_OPT_NOARG;
				തുടരുക;
			അവസ്ഥ '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				തുടരുക;
			അവസ്ഥ '!':
				o->flags |= PARSE_OPT_NONEG;
				തുടരുക;
			അവസ്ഥ '*':
				o->flags |= PARSE_OPT_HIDDEN;
				തുടരുക;
			}
			s--;
			നിർത്തുക;
		}

		എങ്കിൽ (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, വലുപ്പം(*opts));
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
	വേണ്ടി (size_t i = 0; i < opts_nr; i++) {
		സ്വതന്ത്രം((അക്ഷരം *) opts[i].help);
		സ്വതന്ത്രം((അക്ഷരം *) opts[i].argh);
	}
	സ്വതന്ത്രം(opts);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_sq_quote(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	ഘടന strbuf buf = STRBUF_INIT;

	എങ്കിൽ (argc)
		sq_quote_argv(&buf, argv);
	printf("%s\n", buf.buf);
	strbuf_release(&buf);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം die_no_single_rev(പൂർണ്ണസംഖ്യ quiet)
{
	എങ്കിൽ (quiet)
		exit(1);
	അല്ലെങ്കിൽ
		die(_("Needed a single revision"));
}

സ്ഥിതം സ്ഥിരം അക്ഷരം builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ opt_with_മൂല്യം(സ്ഥിരം അക്ഷരം *arg, സ്ഥിരം അക്ഷരം *opt, സ്ഥിരം അക്ഷരം **മൂല്യം)
{
	എങ്കിൽ (skip_prefix(arg, opt, &arg)) {
		എങ്കിൽ (!*arg) {
			*മൂല്യം = ശൂന്യ;
			തിരികെ 1;
		}
		എങ്കിൽ (*arg++ == '=') {
			*മൂല്യം = arg;
			തിരികെ 1;
		}
	}
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം handle_ref_opt(സ്ഥിരം അക്ഷരം *pattern, സ്ഥിരം അക്ഷരം *prefix)
{
	എങ്കിൽ (pattern) {
		ഘടന refs_for_each_ref_options opts = {
			.pattern = pattern,
			.prefix = prefix,
			.trim_prefix = prefix ? strlen(prefix) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ശൂന്യ, &opts);
	} അല്ലെങ്കിൽ {
		ഘടന refs_for_each_ref_options opts = {
			.prefix = prefix,
			.trim_prefix = strlen(prefix),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ശൂന്യ, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

എണ്ണൽ format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

എണ്ണൽ default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

സ്ഥിതം ശൂന്യം print_path(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *prefix,
		       എണ്ണൽ format_type format, എണ്ണൽ default_type def)
{
	ഘടന strbuf sb = STRBUF_INIT;
	എണ്ണൽ path_format fmt;

	എങ്കിൽ (format == FORMAT_DEFAULT) {
		തിരഞ്ഞെടുപ്പ് (def) {
		അവസ്ഥ DEFAULT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			നിർത്തുക;
		അവസ്ഥ DEFAULT_RELATIVE_IF_SHARED:
			fmt = PATH_FORMAT_RELATIVE_IF_SHARED;
			നിർത്തുക;
		അവസ്ഥ DEFAULT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			നിർത്തുക;
		അവസ്ഥ DEFAULT_UNMODIFIED:
		സ്ഥിരസ്ഥിതി:
			fmt = PATH_FORMAT_UNMODIFIED;
			നിർത്തുക;
		}
	} അല്ലെങ്കിൽ {
		തിരഞ്ഞെടുപ്പ് (format) {
		അവസ്ഥ FORMAT_RELATIVE:
			fmt = PATH_FORMAT_RELATIVE;
			നിർത്തുക;
		അവസ്ഥ FORMAT_CANONICAL:
			fmt = PATH_FORMAT_CANONICAL;
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			fmt = PATH_FORMAT_UNMODIFIED;
			നിർത്തുക;
		}
	}

	format_path(&sb, path, prefix, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

പൂർണ്ണസംഖ്യ cmd_rev_parse(പൂർണ്ണസംഖ്യ argc,
		  സ്ഥിരം അക്ഷരം **argv,
		  സ്ഥിരം അക്ഷരം *prefix,
		  ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i, as_is = 0, verify = 0, quiet = 0, revs_എണ്ണം = 0, type = 0;
	സ്ഥിരം ഘടന git_hash_algo *output_algo = ശൂന്യ;
	സ്ഥിരം ഘടന git_hash_algo *compat = ശൂന്യ;
	പൂർണ്ണസംഖ്യ did_repo_setup = 0;
	പൂർണ്ണസംഖ്യ has_dashdash = 0;
	പൂർണ്ണസംഖ്യ output_prefix = 0;
	ഘടന object_id oid;
	unsigned പൂർണ്ണസംഖ്യ flags = 0;
	സ്ഥിരം അക്ഷരം *name = ശൂന്യ;
	ഘടന strbuf buf = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ seen_end_of_options = 0;
	എണ്ണൽ format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	എങ്കിൽ (argc > 1 && !strcmp("--parseopt", argv[1]))
		തിരികെ cmd_parseopt(argc - 1, argv + 1, prefix);

	എങ്കിൽ (argc > 1 && !strcmp("--sq-quote", argv[1]))
		തിരികെ cmd_sq_quote(argc - 2, argv + 2);

	എങ്കിൽ (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	വേണ്ടി (i = 1; i < argc; i++) {
		എങ്കിൽ (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			നിർത്തുക;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	എങ്കിൽ (argc == 1) {
		setup_git_directory(the_repository);
		repo_config(the_repository, git_default_config, ശൂന്യ);
		തിരികെ 0;
	}

	വേണ്ടി (i = 1; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];

		എങ്കിൽ (as_is) {
			എങ്കിൽ (show_file(arg, output_prefix) && as_is < 2)
				verify_filename(the_repository, prefix, arg, 0);
			തുടരുക;
		}

		എങ്കിൽ (!seen_end_of_options) {
			എങ്കിൽ (!strcmp(arg, "--local-env-vars")) {
				പൂർണ്ണസംഖ്യ i;
				വേണ്ടി (i = 0; local_repo_env[i]; i++)
					printf("%s\n", local_repo_env[i]);
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--resolve-git-dir")) {
				സ്ഥിരം അക്ഷരം *gitdir = argv[++i];
				എങ്കിൽ (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				എങ്കിൽ (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				തുടരുക;
			}
		}

		/* The rest of the options require a git repository. */
		എങ്കിൽ (!did_repo_setup) {
			prefix = setup_git_directory(the_repository);
			repo_config(the_repository, git_default_config, ശൂന്യ);
			did_repo_setup = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_full_index = 0;
			compat = the_repository->compat_hash_algo;
		}

		എങ്കിൽ (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			എങ്കിൽ (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			തുടരുക;
		}

		എങ്കിൽ (!seen_end_of_options && *arg == '-') {
			എങ്കിൽ (!strcmp(arg, "--git-path")) {
				എങ്കിൽ (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_path(repo_git_path_replace(the_repository, &buf,
								 "%s", argv[i + 1]), prefix,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg,"-n")) {
				എങ്കിൽ (++i >= argc)
					die(_("-n requires an argument"));
				എങ്കിൽ ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				തുടരുക;
			}
			എങ്കിൽ (starts_with(arg, "-n")) {
				എങ്കിൽ ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				തുടരുക;
			}
			എങ്കിൽ (opt_with_മൂല്യം(arg, "--path-format", &arg)) {
				എങ്കിൽ (!arg)
					die(_("--path-format requires an argument"));
				എങ്കിൽ (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} അല്ലെങ്കിൽ {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--default")) {
				def = argv[++i];
				എങ്കിൽ (!def)
					die(_("--default requires an argument"));
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--prefix")) {
				prefix = argv[++i];
				എങ്കിൽ (!prefix)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->prefix);
				the_repository->prefix = xstrdup(prefix);
				output_prefix = 1;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				തുടരുക;
			}
			എങ്കിൽ (opt_with_മൂല്യം(arg, "--output-object-format", &arg)) {
				എങ്കിൽ (!arg)
					die(_("no object format specified"));
				എങ്കിൽ (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					തുടരുക;
				}
				അല്ലെങ്കിൽ എങ്കിൽ (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					തുടരുക;
				}
				അല്ലെങ്കിൽ die(_("unsupported object format: %s"), arg);
			}
			എങ്കിൽ (opt_with_മൂല്യം(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				എങ്കിൽ (!arg)
					തുടരുക;
				abbrev = strtoul(arg, ശൂന്യ, 10);
				എങ്കിൽ (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				അല്ലെങ്കിൽ എങ്കിൽ ((പൂർണ്ണസംഖ്യ)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--sq")) {
				output_sq = 1;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_FULL;
				തുടരുക;
			}
			എങ്കിൽ (opt_with_മൂല്യം(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				എങ്കിൽ (arg) {
					എങ്കിൽ (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					അല്ലെങ്കിൽ
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, ശൂന്യ);
				clear_ref_exclusions(&ref_excludes);
				തുടരുക;
			}
			എങ്കിൽ (skip_prefix(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, ശൂന്യ);
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--bisect")) {
				അക്ഷരം *prefix;
				അക്ഷരം *term_bad = ശൂന്യ;
				അക്ഷരം *term_good = ശൂന്യ;
				ഘടന refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&term_bad, &term_good);
				prefix = xstrfmt("refs/bisect/%s", term_bad);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, ശൂന്യ, &opts);
				സ്വതന്ത്രം(prefix);
				prefix = xstrfmt("refs/bisect/%s", term_good);
				opts.prefix = prefix;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, ശൂന്യ, &opts);
				സ്വതന്ത്രം(prefix);
				സ്വതന്ത്രം(term_good);
				സ്വതന്ത്രം(term_bad);
				തുടരുക;
			}
			എങ്കിൽ (opt_with_മൂല്യം(arg, "--branches", &arg)) {
				എങ്കിൽ (ref_excludes.hidden_refs_configured)
					തിരികെ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				തുടരുക;
			}
			എങ്കിൽ (opt_with_മൂല്യം(arg, "--tags", &arg)) {
				എങ്കിൽ (ref_excludes.hidden_refs_configured)
					തിരികെ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				തുടരുക;
			}
			എങ്കിൽ (skip_prefix(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, ശൂന്യ);
				തുടരുക;
			}
			എങ്കിൽ (opt_with_മൂല്യം(arg, "--remotes", &arg)) {
				എങ്കിൽ (ref_excludes.hidden_refs_configured)
					തിരികെ error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				തുടരുക;
			}
			എങ്കിൽ (skip_prefix(arg, "--exclude=", &arg)) {
				add_ref_exclusion(&ref_excludes, arg);
				തുടരുക;
			}
			എങ്കിൽ (skip_prefix(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--show-toplevel")) {
				സ്ഥിരം അക്ഷരം *work_വൃക്ഷം = repo_get_work_വൃക്ഷം(the_repository);
				എങ്കിൽ (work_വൃക്ഷം)
					print_path(work_വൃക്ഷം, prefix, format, DEFAULT_UNMODIFIED);
				അല്ലെങ്കിൽ
					die(_("this operation must be run in a work tree"));
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--show-superproject-working-tree")) {
				ഘടന strbuf superproject = STRBUF_INIT;
				എങ്കിൽ (get_superproject_working_വൃക്ഷം(&superproject))
					print_path(superproject.buf, prefix, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--show-prefix")) {
				എങ്കിൽ (prefix)
					puts(prefix);
				അല്ലെങ്കിൽ
					putchar('\n');
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--show-cdup")) {
				സ്ഥിരം അക്ഷരം *pfx = prefix;
				എങ്കിൽ (!is_inside_work_വൃക്ഷം(the_repository)) {
					സ്ഥിരം അക്ഷരം *work_വൃക്ഷം =
						repo_get_work_വൃക്ഷം(the_repository);
					എങ്കിൽ (work_വൃക്ഷം)
						printf("%s\n", work_വൃക്ഷം);
					തുടരുക;
				}
				ആകുമ്പോൾ (pfx) {
					pfx = strchr(pfx, '/');
					എങ്കിൽ (pfx) {
						pfx++;
						printf("../");
					}
				}
				putchar('\n');
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				സ്ഥിരം അക്ഷരം *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				അക്ഷരം *cwd;
				പൂർണ്ണസംഖ്യ len;
				എണ്ണൽ format_type wanted = format;
				എങ്കിൽ (arg[2] == 'g') {	/* --git-dir */
					എങ്കിൽ (gitdir) {
						print_path(gitdir, prefix, format, DEFAULT_UNMODIFIED);
						തുടരുക;
					}
					എങ്കിൽ (!prefix) {
						print_path(".git", prefix, format, DEFAULT_UNMODIFIED);
						തുടരുക;
					}
				} അല്ലെങ്കിൽ {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					എങ്കിൽ (!gitdir && !prefix)
						gitdir = ".git";
					എങ്കിൽ (gitdir) {
						ഘടന strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						തുടരുക;
					}
				}
				cwd = xgetcwd();
				len = strlen(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				സ്വതന്ത്രം(cwd);
				print_path(buf.buf, prefix, wanted, DEFAULT_CANONICAL);
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--git-common-dir")) {
				print_path(repo_get_common_dir(the_repository), prefix, format, DEFAULT_RELATIVE_IF_SHARED);
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--is-inside-git-dir")) {
				printf("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--is-inside-work-tree")) {
				printf("%s\n", is_inside_work_വൃക്ഷം(the_repository) ? "true"
						: "false");
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--is-bare-repository")) {
				printf("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--is-shallow-repository")) {
				printf("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--shared-index-path")) {
				എങ്കിൽ (repo_read_index(the_repository) < 0)
					die(_("Could not read the index"));
				എങ്കിൽ (the_repository->index->split_index) {
					സ്ഥിരം ഘടന object_id *oid = &the_repository->index->split_index->base_oid;
					സ്ഥിരം അക്ഷരം *path = repo_git_path_replace(the_repository, &buf, "sharedindex.%s", oid_to_hex(oid));
					print_path(path, prefix, format, DEFAULT_RELATIVE);
				}
				തുടരുക;
			}
			എങ്കിൽ (skip_prefix(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				തുടരുക;
			}
			എങ്കിൽ (skip_prefix(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				തുടരുക;
			}
			എങ്കിൽ (skip_prefix(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				തുടരുക;
			}
			എങ്കിൽ (skip_prefix(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				തുടരുക;
			}
			എങ്കിൽ (opt_with_മൂല്യം(arg, "--show-object-format", &arg)) {
				സ്ഥിരം അക്ഷരം *val = arg ? arg : "storage";

				എങ്കിൽ (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				എങ്കിൽ (!strcmp(val, "compat")) {
					എങ്കിൽ (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					അല്ലെങ്കിൽ
						putchar('\n');
				} അല്ലെങ്കിൽ {
					puts(the_hash_algo->name);
				}
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_to_name(the_repository->ref_storage_format));
				തുടരുക;
			}
			എങ്കിൽ (!strcmp(arg, "--end-of-options")) {
				seen_end_of_options = 1;
				എങ്കിൽ (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				തുടരുക;
			}
			എങ്കിൽ (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			തുടരുക;
		}

		/* Not a flag argument */
		എങ്കിൽ (try_difference(arg))
			തുടരുക;
		എങ്കിൽ (try_parent_shorthands(arg))
			തുടരുക;
		name = arg;
		type = NORMAL;
		എങ്കിൽ (*arg == '^') {
			name++;
			type = REVERSED;
		}
		എങ്കിൽ (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			എങ്കിൽ (output_algo)
				repo_oid_to_algop(the_repository, &oid,
						  output_algo, &oid);
			എങ്കിൽ (verify)
				revs_എണ്ണം++;
			അല്ലെങ്കിൽ
				show_rev(type, &oid, name);
			തുടരുക;
		}
		എങ്കിൽ (verify)
			die_no_single_rev(quiet);
		എങ്കിൽ (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		എങ്കിൽ (!show_file(arg, output_prefix))
			തുടരുക;
		verify_filename(the_repository, prefix, arg, 1);
	}
	strbuf_release(&buf);
	എങ്കിൽ (verify) {
		എങ്കിൽ (revs_എണ്ണം == 1) {
			show_rev(type, &oid, name);
			തിരികെ 0;
		} അല്ലെങ്കിൽ എങ്കിൽ (revs_എണ്ണം == 0 && show_default())
			തിരികെ 0;
		die_no_single_rev(quiet);
	} അല്ലെങ്കിൽ
		show_default();
	തിരികെ 0;
}
