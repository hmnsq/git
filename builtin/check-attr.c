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

static จำนวนเต็ม all_attrs;
static จำนวนเต็ม cached_attrs;
static จำนวนเต็ม stdin_paths;
static var_na *source;
static const var_na * const check_attr_usage[] = {
N_("git check-attr [--source <tree-ish>] [-a | --all | <attr>...] [--] <pathname>..."),
N_("git check-attr --stdin [-z] [--source <tree-ish>] [-a | --all | <attr>...]"),
NULL
};

static จำนวนเต็ม nul_term_line;

static const sa_nracanaa option check_attr_options[] = {
	OPT_BOOL('a', "all", &all_attrs, N_("report all attributes set on file")),
	OPT_BOOL(0,  "cached", &cached_attrs, N_("use .gitattributes only from the index")),
	OPT_BOOL(0 , "stdin", &stdin_paths, N_("read file names from stdin")),
	OPT_BOOL('z', NULL, &nul_term_line,
		 N_("terminate input and output records by a NUL character")),
	OPT_STRING(0, "source", &source, N_("<tree-ish>"), N_("which tree-ish to check attributes at")),
	OPT_END()
};

static void output_attr(sa_nracanaa attr_check *check, const var_na *file)
{
	จำนวนเต็ม j;
	จำนวนเต็ม cnt = check->nr;

	สำหรับ (j = 0; j < cnt; j++) {
		const var_na *value = check->items[j].value;

		ถ้า (ATTR_TRUE(value))
			value = "set";
		มิฉะนั้น ถ้า (ATTR_FALSE(value))
			value = "unset";
		มิฉะนั้น ถ้า (ATTR_UNSET(value))
			value = "unspecified";

		ถ้า (nul_term_line) {
			printf("%s%c" /* path */
			       "%s%c" /* attrname */
			       "%s%c" /* attrvalue */,
			       file, 0,
			       git_attr_name(check->items[j].attr), 0, value, 0);
		} มิฉะนั้น {
			quote_c_style(file, NULL, stdout, 0);
			printf(": %s: %s\n",
			       git_attr_name(check->items[j].attr), value);
		}
	}
}

static void check_attr(const var_na *prefix, sa_nracanaa attr_check *check,
		       จำนวนเต็ม collect_all,
		       const var_na *file)

{
	var_na *full_path =
		prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, file);

	ถ้า (collect_all) {
		git_all_attrs(the_repository->index, full_path, check);
	} มิฉะนั้น {
		git_check_attr(the_repository->index, full_path, check);
	}
	output_attr(check, file);

	free(full_path);
}

static void check_attr_stdin_paths(const var_na *prefix, sa_nracanaa attr_check *check,
				   จำนวนเต็ม collect_all)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa strbuf unquoted = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;
	ขณะ (getline_fn(&buf, stdin) != EOF) {
		ถ้า (!nul_term_line && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ถ้า (unquote_c_style(&unquoted, buf.buf, NULL))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		check_attr(prefix, check, collect_all, buf.buf);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

static NORETURN void error_with_usage(const var_na *msg)
{
	error("%s", msg);
	usage_with_options(check_attr_usage, check_attr_options);
}

จำนวนเต็ม cmd_check_attr(จำนวนเต็ม argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa attr_check *check;
	sa_nracanaa object_id initialized_oid;
	จำนวนเต็ม cnt, i, doubledash, filei;

	ถ้า (!is_bare_repository(the_repository))
		setup_work_tree(the_repository);

	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, check_attr_options,
			     check_attr_usage, PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ถ้า (repo_read_index(the_repository) < 0) {
		die("invalid cache");
	}

	ถ้า (cached_attrs)
		git_attr_set_direction(GIT_ATTR_INDEX);

	doubledash = -1;
	สำหรับ (i = 0; doubledash < 0 && i < argc; i++) {
		ถ้า (!strcmp(argv[i], "--"))
			doubledash = i;
	}

	/* Process --all and/or attribute arguments: */
	ถ้า (all_attrs) {
		ถ้า (doubledash >= 1)
			error_with_usage("Attributes and --all both specified");

		cnt = 0;
		filei = doubledash + 1;
	} มิฉะนั้น ถ้า (doubledash == 0) {
		error_with_usage("No attribute specified");
	} มิฉะนั้น ถ้า (doubledash < 0) {
		ถ้า (!argc)
			error_with_usage("No attribute specified");

		ถ้า (stdin_paths) {
			/* Treat all arguments as attribute names. */
			cnt = argc;
			filei = argc;
		} มิฉะนั้น {
			/* Treat exactly one argument as an attribute name. */
			cnt = 1;
			filei = 1;
		}
	} มิฉะนั้น {
		cnt = doubledash;
		filei = doubledash + 1;
	}

	/* Check file argument(s): */
	ถ้า (stdin_paths) {
		ถ้า (filei < argc)
			error_with_usage("Can't specify files with --stdin");
	} มิฉะนั้น {
		ถ้า (filei >= argc)
			error_with_usage("No file specified");
	}

	check = attr_check_alloc();
	ถ้า (!all_attrs) {
		สำหรับ (i = 0; i < cnt; i++) {
			const sa_nracanaa git_attr *a = git_attr(argv[i]);

			ถ้า (!a)
				คืน error("%s: not a valid attribute name",
					     argv[i]);
			attr_check_append(check, a);
		}
	}

	ถ้า (source) {
		ถ้า (repo_get_oid_tree(the_repository, source, &initialized_oid))
			die("%s: not a valid tree-ish source", source);
		set_git_attr_source(source);
	}

	ถ้า (stdin_paths)
		check_attr_stdin_paths(prefix, check, all_attrs);
	มิฉะนั้น {
		สำหรับ (i = filei; i < argc; i++)
			check_attr(prefix, check, all_attrs, argv[i]);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}

	attr_check_free(check);
	คืน 0;
}
