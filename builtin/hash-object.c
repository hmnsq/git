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

static void hash_fd(số_nguyên fd, const char *type, const char *path, unsigned flags)
{
	struct stat st;
	struct object_id oid;

	nếu (fstat(fd, &st) < 0 ||
	    index_fd(the_repository->index, &oid, fd, &st,
		     type_from_string(type), path, flags))
		die((flags & INDEX_WRITE_OBJECT)
		    ? "Unable to add %s to database"
		    : "Unable to hash %s", path);
	printf("%s\n", oid_to_hex(&oid));
	maybe_flush_or_die(stdout, "hash to stdout");
}

static void hash_object(const char *path, const char *type, const char *vpath,
			unsigned flags)
{
	số_nguyên fd;
	fd = xopen(path, O_RDONLY);
	hash_fd(fd, type, vpath, flags);
}

static void hash_stdin_paths(const char *type, số_nguyên no_filters, unsigned flags)
{
	struct strbuf buf = STRBUF_INIT;
	struct strbuf unquoted = STRBUF_INIT;

	trong_khi (strbuf_getline(&buf, stdin) != EOF) {
		nếu (buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			nếu (unquote_c_style(&unquoted, buf.buf, NULL))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		hash_object(buf.buf, type, no_filters ? NULL : buf.buf, flags);
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

số_nguyên cmd_hash_object(số_nguyên argc,
		    const char **argv,
		    const char *prefix,
		    struct repository *repo UNUSED)
{
	static const char * const hash_object_usage[] = {
		N_("git hash-object [-t <type>] [-w] [--path=<file> | --no-filters]\n"
		   "                [--stdin [--literally]] [--] <file>..."),
		N_("git hash-object [-t <type>] [-w] --stdin-paths [--no-filters]"),
		NULL
	};
	const char *type = blob_type;
	số_nguyên hashstdin = 0;
	số_nguyên stdin_paths = 0;
	số_nguyên no_filters = 0;
	số_nguyên nongit = 0;
	unsigned flags = INDEX_FORMAT_CHECK;
	const char *vpath = NULL;
	char *vpath_free = NULL;
	const struct option hash_object_options[] = {
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
	số_nguyên i;
	const char *errstr = NULL;

	argc = parse_options(argc, argv, prefix, hash_object_options,
			     hash_object_usage, 0);

	nếu (flags & INDEX_WRITE_OBJECT)
		prefix = setup_git_directory(the_repository);
	không_thì
		prefix = setup_git_directory_gently(the_repository, &nongit);

	nếu (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	nếu (vpath && prefix) {
		vpath_free = prefix_filename(prefix, vpath);
		vpath = vpath_free;
	}

	repo_config(the_repository, git_default_config, NULL);

	nếu (stdin_paths) {
		nếu (hashstdin)
			errstr = "Can't use --stdin-paths with --stdin";
		không_thì nếu (argc)
			errstr = "Can't specify files with --stdin-paths";
		không_thì nếu (vpath)
			errstr = "Can't use --stdin-paths with --path";
	}
	không_thì {
		nếu (hashstdin > 1)
			errstr = "Multiple --stdin arguments are not supported";
		nếu (vpath && no_filters)
			errstr = "Can't use --path with --no-filters";
	}

	nếu (errstr) {
		error("%s", errstr);
		usage_with_options(hash_object_usage, hash_object_options);
	}

	nếu (hashstdin)
		hash_fd(0, type, vpath, flags);

	cho (i = 0 ; i < argc; i++) {
		const char *arg = argv[i];
		char *to_free = NULL;

		nếu (prefix)
			arg = to_free = prefix_filename(prefix, arg);
		hash_object(arg, type, no_filters ? NULL : vpath ? vpath : arg,
			    flags);
		free(to_free);
	}

	nếu (stdin_paths)
		hash_stdin_paths(type, no_filters, flags);

	free(vpath_free);

	trả_về 0;
}
