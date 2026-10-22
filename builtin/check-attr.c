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

static ကိန်းပြည့် all_attrs;
static ကိန်းပြည့် cached_attrs;
static ကိန်းပြည့် stdin_paths;
static var_na *source;
static const var_na * const check_attr_usage[] = {
N_("git check-attr [--source <tree-ish>] [-a | --all | <attr>...] [--] <pathname>..."),
N_("git check-attr --stdin [-z] [--source <tree-ish>] [-a | --all | <attr>...]"),
NULL
};

static ကိန်းပြည့် nul_term_line;

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
	ကိန်းပြည့် j;
	ကိန်းပြည့် cnt = check->nr;

	အတွက် (j = 0; j < cnt; j++) {
		const var_na *value = check->items[j].value;

		အကယ်၍ (ATTR_TRUE(value))
			value = "set";
		မဟုတ်ပါက အကယ်၍ (ATTR_FALSE(value))
			value = "unset";
		မဟုတ်ပါက အကယ်၍ (ATTR_UNSET(value))
			value = "unspecified";

		အကယ်၍ (nul_term_line) {
			printf("%s%c" /* path */
			       "%s%c" /* attrname */
			       "%s%c" /* attrvalue */,
			       file, 0,
			       git_attr_name(check->items[j].attr), 0, value, 0);
		} မဟုတ်ပါက {
			quote_c_style(file, NULL, stdout, 0);
			printf(": %s: %s\n",
			       git_attr_name(check->items[j].attr), value);
		}
	}
}

static void check_attr(const var_na *prefix, sa_nracanaa attr_check *check,
		       ကိန်းပြည့် collect_all,
		       const var_na *file)

{
	var_na *full_path =
		prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, file);

	အကယ်၍ (collect_all) {
		git_all_attrs(the_repository->index, full_path, check);
	} မဟုတ်ပါက {
		git_check_attr(the_repository->index, full_path, check);
	}
	output_attr(check, file);

	free(full_path);
}

static void check_attr_stdin_paths(const var_na *prefix, sa_nracanaa attr_check *check,
				   ကိန်းပြည့် collect_all)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa strbuf unquoted = STRBUF_INIT;
	strbuf_getline_fn getline_fn;

	getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;
	နေစဉ် (getline_fn(&buf, stdin) != EOF) {
		အကယ်၍ (!nul_term_line && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			အကယ်၍ (unquote_c_style(&unquoted, buf.buf, NULL))
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

ကိန်းပြည့် cmd_check_attr(ကိန်းပြည့် argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa attr_check *check;
	sa_nracanaa object_id initialized_oid;
	ကိန်းပြည့် cnt, i, doubledash, filei;

	အကယ်၍ (!is_bare_repository(the_repository))
		setup_work_tree(the_repository);

	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, check_attr_options,
			     check_attr_usage, PARSE_OPT_KEEP_DASHDASH);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	အကယ်၍ (repo_read_index(the_repository) < 0) {
		die("invalid cache");
	}

	အကယ်၍ (cached_attrs)
		git_attr_set_direction(GIT_ATTR_INDEX);

	doubledash = -1;
	အတွက် (i = 0; doubledash < 0 && i < argc; i++) {
		အကယ်၍ (!strcmp(argv[i], "--"))
			doubledash = i;
	}

	/* Process --all and/or attribute arguments: */
	အကယ်၍ (all_attrs) {
		အကယ်၍ (doubledash >= 1)
			error_with_usage("Attributes and --all both specified");

		cnt = 0;
		filei = doubledash + 1;
	} မဟုတ်ပါက အကယ်၍ (doubledash == 0) {
		error_with_usage("No attribute specified");
	} မဟုတ်ပါက အကယ်၍ (doubledash < 0) {
		အကယ်၍ (!argc)
			error_with_usage("No attribute specified");

		အကယ်၍ (stdin_paths) {
			/* Treat all arguments as attribute names. */
			cnt = argc;
			filei = argc;
		} မဟုတ်ပါက {
			/* Treat exactly one argument as an attribute name. */
			cnt = 1;
			filei = 1;
		}
	} မဟုတ်ပါက {
		cnt = doubledash;
		filei = doubledash + 1;
	}

	/* Check file argument(s): */
	အကယ်၍ (stdin_paths) {
		အကယ်၍ (filei < argc)
			error_with_usage("Can't specify files with --stdin");
	} မဟုတ်ပါက {
		အကယ်၍ (filei >= argc)
			error_with_usage("No file specified");
	}

	check = attr_check_alloc();
	အကယ်၍ (!all_attrs) {
		အတွက် (i = 0; i < cnt; i++) {
			const sa_nracanaa git_attr *a = git_attr(argv[i]);

			အကယ်၍ (!a)
				ပြန်ပေး error("%s: not a valid attribute name",
					     argv[i]);
			attr_check_append(check, a);
		}
	}

	အကယ်၍ (source) {
		အကယ်၍ (repo_get_oid_tree(the_repository, source, &initialized_oid))
			die("%s: not a valid tree-ish source", source);
		set_git_attr_source(source);
	}

	အကယ်၍ (stdin_paths)
		check_attr_stdin_paths(prefix, check, all_attrs);
	မဟုတ်ပါက {
		အတွက် (i = filei; i < argc; i++)
			check_attr(prefix, check, all_attrs, argv[i]);
		maybe_flush_or_die(stdout, "attribute to stdout");
	}

	attr_check_free(check);
	ပြန်ပေး 0;
}
