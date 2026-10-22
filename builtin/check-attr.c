#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "attr.h"
#include "environment.h"
#include "gettext.h"
#include "object-name.h"
#include "quote.h"
#include "setup.h"
#include "parse-options.h"
#include "write-or-die.h"

static số_nguyên all_attrs;
static số_nguyên cached_attrs;
static số_nguyên stdin_paths;
static char *source;
static const char * const check_attr_usage[] = {
N_("git check-attr [--source <tree-ish>] [-a | --all | <attr>...] [--] <pathname>..."),
N_("git check-attr --stdin [-z] [--source <tree-ish>] [-a | --all | <attr>...]"),
NULL
};

static số_nguyên nul_term_line;

static const struct option check_attr_options[] = {
	OPT_BOOL('a', "all", &all_attrs, N_("report all attributes set on file")),
	OPT_BOOL(0,  "cached", &cached_attrs, N_("use .gitattributes only from the index")),
	OPT_BOOL(0 , "stdin", &stdin_paths, N_("read file names from stdin")),
	OPT_BOOL('z', NULL, &nul_term_line,
		 N_("terminate input and output records by a NUL character")),
	OPT_STRING(0, "source", &source, N_("<tree-ish>"), N_("which tree-ish to check attributes at")),
	OPT_END()
};

static void output_attr(struct attr_check *check, const char *file)
{
	số_nguyên j;
	số_nguyên cnt = check->nr;

	cho (j = 0; j < cnt; j++) {
		const char *value = check->items[j].value;

		nếu (ATTR_TRUE(value))
			value = "set";
		không_thì nếu (ATTR_FALSE(value))
			value = "unset";
		không_thì nếu (ATTR_UNSET(value))
			value = "unspecified";

		nếu (nul_term_line) {
			printf("%s%c" /* path */
			       "%s%c" /* attrname */
			       "%s%c" /* attrvalue */,
			       file, 0,
			       git_attr_name(check->items[j].attr), 0, value, 0);
		} không_thì {
			quote_c_style(file, NULL, stdout, 0);
			printf(": %s: %s\n",
			       git_attr_name(check->items[j].attr), value);
		}
	}
}

static void check_attr(const char *prefix, struct attr_check *check,
		       số_nguyên collect_all,
		       const char *file)

{
	char *full_path =
		prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, file);

	nếu (collect_all) {
		git_all_attrs(the_repository->index, full_path, check);
	} không_thì {
		git_check_attr(the_repository->index, full_path, check);
	}
	output_attr(check, file);

	free(full_path);
}

static void check_attr_stdin_paths(const char *prefix, struct attr_check *check,
				   số_nguyên collect_all)
{
	struct strbuf buf = STRBUF_INIT;
	struct strbuf unquoted = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;
	trong_khi (getline_fn(&buf, stdin) != EOF) {
		nếu (!nul_term_line && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			nếu (unquote_c_style(&unquoted, buf.buf, NULL))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		check_attr(prefix, check, collect_all, buf.buf);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

static NORETURN void error_with_usage(const char *msg)
{
	error("%s", msg);
	usage_with_options(check_attr_usage, check_attr_options);
}

số_nguyên cmd_check_attr(số_nguyên argc,
		   const char **argv,
		   const char *prefix,
		   struct repository *repo UNUSED)
{
	struct attr_check *check;
	struct object_id initialized_oid;
	số_nguyên cnt, i, doubledash, filei;

	nếu (!is_bare_repository(the_repository))
		setup_work_tree(the_repository);

	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, check_attr_options,
			     check_attr_usage, PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	nếu (repo_read_index(the_repository) < 0) {
		die("invalid cache");
	}

	nếu (cached_attrs)
		git_attr_set_direction(GIT_ATTR_INDEX);

	doubledash = -1;
	cho (i = 0; doubledash < 0 && i < argc; i++) {
		nếu (!strcmp(argv[i], "--"))
			doubledash = i;
	}

	/* Process --all and/or attribute arguments: */
	nếu (all_attrs) {
		nếu (doubledash >= 1)
			error_with_usage("Attributes and --all both specified");

		cnt = 0;
		filei = doubledash + 1;
	} không_thì nếu (doubledash == 0) {
		error_with_usage("No attribute specified");
	} không_thì nếu (doubledash < 0) {
		nếu (!argc)
			error_with_usage("No attribute specified");

		nếu (stdin_paths) {
			/* Treat all arguments as attribute names. */
			cnt = argc;
			filei = argc;
		} không_thì {
			/* Treat exactly one argument as an attribute name. */
			cnt = 1;
			filei = 1;
		}
	} không_thì {
		cnt = doubledash;
		filei = doubledash + 1;
	}

	/* Check file argument(s): */
	nếu (stdin_paths) {
		nếu (filei < argc)
			error_with_usage("Can't specify files with --stdin");
	} không_thì {
		nếu (filei >= argc)
			error_with_usage("No file specified");
	}

	check = attr_check_alloc();
	nếu (!all_attrs) {
		cho (i = 0; i < cnt; i++) {
			const struct git_attr *a = git_attr(argv[i]);

			nếu (!a)
				trả_về error("%s: not a valid attribute name",
					     argv[i]);
			attr_check_append(check, a);
		}
	}

	nếu (source) {
		nếu (repo_get_oid_tree(the_repository, source, &initialized_oid))
			die("%s: not a valid tree-ish source", source);
		set_git_attr_source(source);
	}

	nếu (stdin_paths)
		check_attr_stdin_paths(prefix, check, all_attrs);
	không_thì {
		cho (i = filei; i < argc; i++)
			check_attr(prefix, check, all_attrs, argv[i]);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}

	attr_check_free(check);
	trả_về 0;
}
