/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 * Copyright (C) Junio C Hamano, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "blob.h"
#ضمن "quote.h"
#ضمن "parse-options.h"
#ضمن "setup.h"
#ضمن "strbuf.h"
#ضمن "write-or-die.h"

ساكن فراغ hash_fd(صحيح fd, ثابت حرف *type, ثابت حرف *path, unsigned flags)
{
	بنية stat st;
	بنية object_id oid;

	إذا (fstat(fd, &st) < 0 ||
	    index_fd(the_repository->index, &oid, fd, &st,
		     type_from_string(type), path, flags))
		die((flags & INDEX_WRITE_OBJECT)
		    ? "Unable to add %s to database"
		    : "Unable to hash %s", path);
	printf("%s\n", oid_to_hex(&oid));
	maybe_flush_or_die(stdout, "hash to stdout");
}

ساكن فراغ hash_object(ثابت حرف *path, ثابت حرف *type, ثابت حرف *vpath,
			unsigned flags)
{
	صحيح fd;
	fd = xopen(path, O_RDONLY);
	hash_fd(fd, type, vpath, flags);
}

ساكن فراغ hash_stdin_paths(ثابت حرف *type, صحيح no_filters, unsigned flags)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf unquoted = STRBUF_INIT;

	بينما (strbuf_getline(&buf, stdin) != EOF) {
		إذا (buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			إذا (unquote_c_style(&unquoted, buf.buf, عدم))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		hash_object(buf.buf, type, no_filters ? عدم : buf.buf, flags);
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

صحيح cmd_hash_object(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *prefix,
		    بنية repository *repo UNUSED)
{
	ساكن ثابت حرف * ثابت hash_object_usage[] = {
		N_("git hash-object [-t <type>] [-w] [--path=<file> | --no-filters]\n"
		   "                [--stdin [--literally]] [--] <file>..."),
		N_("git hash-object [-t <type>] [-w] --stdin-paths [--no-filters]"),
		عدم
	};
	ثابت حرف *type = blob_type;
	صحيح hashstdin = 0;
	صحيح stdin_paths = 0;
	صحيح no_filters = 0;
	صحيح nongit = 0;
	unsigned flags = INDEX_FORMAT_CHECK;
	ثابت حرف *vpath = عدم;
	حرف *vpath_free = عدم;
	ثابت بنية option hash_object_options[] = {
		OPT_STRING('t', عدم, &type, N_("type"), N_("object type")),
		OPT_BIT('w', عدم, &flags, N_("write the object into the object database"),
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
	صحيح i;
	ثابت حرف *errstr = عدم;

	argc = parse_options(argc, argv, prefix, hash_object_options,
			     hash_object_usage, 0);

	إذا (flags & INDEX_WRITE_OBJECT)
		prefix = setup_git_directory(the_repository);
	وإلا
		prefix = setup_git_directory_gently(the_repository, &nongit);

	إذا (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	إذا (vpath && prefix) {
		vpath_free = prefix_filename(prefix, vpath);
		vpath = vpath_free;
	}

	repo_config(the_repository, git_default_config, عدم);

	إذا (stdin_paths) {
		إذا (hashstdin)
			errstr = "Can't use --stdin-paths with --stdin";
		وإلا إذا (argc)
			errstr = "Can't specify files with --stdin-paths";
		وإلا إذا (vpath)
			errstr = "Can't use --stdin-paths with --path";
	}
	وإلا {
		إذا (hashstdin > 1)
			errstr = "Multiple --stdin arguments are not supported";
		إذا (vpath && no_filters)
			errstr = "Can't use --path with --no-filters";
	}

	إذا (errstr) {
		error("%s", errstr);
		usage_with_options(hash_object_usage, hash_object_options);
	}

	إذا (hashstdin)
		hash_fd(0, type, vpath, flags);

	لكل (i = 0 ; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		حرف *to_free = عدم;

		إذا (prefix)
			arg = to_free = prefix_filename(prefix, arg);
		hash_object(arg, type, no_filters ? عدم : vpath ? vpath : arg,
			    flags);
		حرر(to_free);
	}

	إذا (stdin_paths)
		hash_stdin_paths(type, no_filters, flags);

	حرر(vpath_free);

	إرجاع 0;
}
