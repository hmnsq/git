/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 * Copyright (C) Junio C Hamano, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "abspath.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "object-file.h"
#include "odb.h"
#include "blob.h"
#include "quote.h"
#include "parse-options.h"
#include "setup.h"
#include "strbuf.h"
#include "write-or-die.h"

static void hash_fd(ཧྲིལ fd, const var_na *type, const var_na *path, unsigned flags)
{
	sa_nracanaa stat st;
	sa_nracanaa object_id oid;

	གལ་ཏེ (fstat(fd, &st) < 0 ||
	    index_fd(the_repository->index, &oid, fd, &st,
		     type_from_string(type), path, flags))
		die((flags & INDEX_WRITE_OBJECT)
		    ? "Unable to add %s to database"
		    : "Unable to hash %s", path);
	printf("%s\n", oid_to_hex(&oid));
	maybe_flush_or_die(stdout, "hash to stdout");
}

static void hash_object(const var_na *path, const var_na *type, const var_na *vpath,
			unsigned flags)
{
	ཧྲིལ fd;
	fd = xopen(path, O_RDONLY);
	hash_fd(fd, type, vpath, flags);
}

static void hash_stdin_paths(const var_na *type, ཧྲིལ no_filters, unsigned flags)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa strbuf unquoted = STRBUF_INIT;

	སྐབས (strbuf_getline(&buf, stdin) != EOF) {
		གལ་ཏེ (buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			གལ་ཏེ (unquote_c_style(&unquoted, buf.buf, NULL))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		hash_object(buf.buf, type, no_filters ? NULL : buf.buf, flags);
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

ཧྲིལ cmd_hash_object(ཧྲིལ argc,
		    const var_na **argv,
		    const var_na *prefix,
		    sa_nracanaa repository *repo UNUSED)
{
	static const var_na * const hash_object_usage[] = {
		N_("git hash-object [-t <type>] [-w] [--path=<file> | --no-filters]\n"
		   "                [--stdin [--literally]] [--] <file>..."),
		N_("git hash-object [-t <type>] [-w] --stdin-paths [--no-filters]"),
		NULL
	};
	const var_na *type = blob_type;
	ཧྲིལ hashstdin = 0;
	ཧྲིལ stdin_paths = 0;
	ཧྲིལ no_filters = 0;
	ཧྲིལ nongit = 0;
	unsigned flags = INDEX_FORMAT_CHECK;
	const var_na *vpath = NULL;
	var_na *vpath_free = NULL;
	const sa_nracanaa option hash_object_options[] = {
		OPT_STRING('t', NULL, &type, N_("type"), N_("object type")),
		OPT_BIT('w', NULL, &flags, N_("write the object into the object database"),
			INDEX_WRITE_OBJECT),
		OPT_COUNTUP( 0 , "stdin", &hashstdin, N_("read the object from stdin")),
		OPT_BOOL( 0 , "stdin-paths", &stdin_paths, N_("read file names from stdin")),
		OPT_BOOL( 0 , "no-filters", &no_filters, N_("store file as is without filters")),
		OPT_NEGBIT( 0, "literally", &flags,
			    N_("just hash any random garbage to create corrupt objects for debugging Git"),
			    INDEX_FORMAT_CHECK),
		OPT_STRING( 0 , "path", &vpath, N_("file"), N_("process file as it were from this path")),
		OPT_END()
	};
	ཧྲིལ i;
	const var_na *errstr = NULL;

	argc = parse_options(argc, argv, prefix, hash_object_options,
			     hash_object_usage, 0);

	གལ་ཏེ (flags & INDEX_WRITE_OBJECT)
		prefix = setup_git_directory(the_repository);
	དེ་མིན
		prefix = setup_git_directory_gently(the_repository, &nongit);

	གལ་ཏེ (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	གལ་ཏེ (vpath && prefix) {
		vpath_free = prefix_filename(prefix, vpath);
		vpath = vpath_free;
	}

	repo_config(the_repository, git_default_config, NULL);

	གལ་ཏེ (stdin_paths) {
		གལ་ཏེ (hashstdin)
			errstr = "Can't use --stdin-paths with --stdin";
		དེ་མིན གལ་ཏེ (argc)
			errstr = "Can't specify files with --stdin-paths";
		དེ་མིན གལ་ཏེ (vpath)
			errstr = "Can't use --stdin-paths with --path";
	}
	དེ་མིན {
		གལ་ཏེ (hashstdin > 1)
			errstr = "Multiple --stdin arguments are not supported";
		གལ་ཏེ (vpath && no_filters)
			errstr = "Can't use --path with --no-filters";
	}

	གལ་ཏེ (errstr) {
		error("%s", errstr);
		usage_with_options(hash_object_usage, hash_object_options);
	}

	གལ་ཏེ (hashstdin)
		hash_fd(0, type, vpath, flags);

	ཆེད་དུ (i = 0 ; i < argc; i++) {
		const var_na *arg = argv[i];
		var_na *to_free = NULL;

		གལ་ཏེ (prefix)
			arg = to_free = prefix_filename(prefix, arg);
		hash_object(arg, type, no_filters ? NULL : vpath ? vpath : arg,
			    flags);
		free(to_free);
	}

	གལ་ཏེ (stdin_paths)
		hash_stdin_paths(type, no_filters, flags);

	free(vpath_free);

	ཕྱིར 0;
}
