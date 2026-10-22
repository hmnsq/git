/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 * Copyright (C) Junio C Hamano, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "abspath.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "blob.h"
#شامل "quote.h"
#شامل "parse-options.h"
#شامل "setup.h"
#شامل "strbuf.h"
#شامل "write-or-die.h"

ایستا تهی hash_fd(صحیح fd, ثابت نویسه *type, ثابت نویسه *path, unsigned flags)
{
	ساختار stat st;
	ساختار object_id oid;

	اگر (fstat(fd, &st) < 0 ||
	    index_fd(the_repository->index, &oid, fd, &st,
		     type_from_string(type), path, flags))
		die((flags & INDEX_WRITE_OBJECT)
		    ? "Unable to add %s to database"
		    : "Unable to hash %s", path);
	printf("%s\n", oid_to_hex(&oid));
	maybe_flush_or_die(stdout, "hash to stdout");
}

ایستا تهی hash_object(ثابت نویسه *path, ثابت نویسه *type, ثابت نویسه *vpath,
			unsigned flags)
{
	صحیح fd;
	fd = xopen(path, O_RDONLY);
	hash_fd(fd, type, vpath, flags);
}

ایستا تهی hash_stdin_paths(ثابت نویسه *type, صحیح no_filters, unsigned flags)
{
	ساختار strbuf buf = STRBUF_INIT;
	ساختار strbuf unquoted = STRBUF_INIT;

	وقتی (strbuf_getline(&buf, stdin) != EOF) {
		اگر (buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			اگر (unquote_c_style(&unquoted, buf.buf, پوچ))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		hash_object(buf.buf, type, no_filters ? پوچ : buf.buf, flags);
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

صحیح cmd_hash_object(صحیح argc,
		    ثابت نویسه **argv,
		    ثابت نویسه *prefix,
		    ساختار repository *repo UNUSED)
{
	ایستا ثابت نویسه * ثابت hash_object_usage[] = {
		N_("git hash-object [-t <type>] [-w] [--path=<file> | --no-filters]\n"
		   "                [--stdin [--literally]] [--] <file>..."),
		N_("git hash-object [-t <type>] [-w] --stdin-paths [--no-filters]"),
		پوچ
	};
	ثابت نویسه *type = blob_type;
	صحیح hashstdin = 0;
	صحیح stdin_paths = 0;
	صحیح no_filters = 0;
	صحیح nongit = 0;
	unsigned flags = INDEX_FORMAT_CHECK;
	ثابت نویسه *vpath = پوچ;
	نویسه *vpath_free = پوچ;
	ثابت ساختار option hash_object_options[] = {
		OPT_STRING('t', پوچ, &type, N_("type"), N_("object type")),
		OPT_BIT('w', پوچ, &flags, N_("write the object into the object database"),
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
	صحیح i;
	ثابت نویسه *errstr = پوچ;

	argc = parse_options(argc, argv, prefix, hash_object_options,
			     hash_object_usage, 0);

	اگر (flags & INDEX_WRITE_OBJECT)
		prefix = setup_git_directory(the_repository);
	وگرنه
		prefix = setup_git_directory_gently(the_repository, &nongit);

	اگر (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	اگر (vpath && prefix) {
		vpath_free = prefix_filename(prefix, vpath);
		vpath = vpath_free;
	}

	repo_config(the_repository, git_default_config, پوچ);

	اگر (stdin_paths) {
		اگر (hashstdin)
			errstr = "Can't use --stdin-paths with --stdin";
		وگرنه اگر (argc)
			errstr = "Can't specify files with --stdin-paths";
		وگرنه اگر (vpath)
			errstr = "Can't use --stdin-paths with --path";
	}
	وگرنه {
		اگر (hashstdin > 1)
			errstr = "Multiple --stdin arguments are not supported";
		اگر (vpath && no_filters)
			errstr = "Can't use --path with --no-filters";
	}

	اگر (errstr) {
		error("%s", errstr);
		usage_with_options(hash_object_usage, hash_object_options);
	}

	اگر (hashstdin)
		hash_fd(0, type, vpath, flags);

	برای (i = 0 ; i < argc; i++) {
		ثابت نویسه *arg = argv[i];
		نویسه *to_free = پوچ;

		اگر (prefix)
			arg = to_free = prefix_filename(prefix, arg);
		hash_object(arg, type, no_filters ? پوچ : vpath ? vpath : arg,
			    flags);
		آزاد(to_free);
	}

	اگر (stdin_paths)
		hash_stdin_paths(type, no_filters, flags);

	آزاد(vpath_free);

	بازگشت 0;
}
