/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 * Copyright (C) Junio C Hamano, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "blob.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "write-or-die.h"

நிலையான வெறுமை hash_fd(முழு fd, மாறிலி எழுத்து *type, மாறிலி எழுத்து *path, unsigned flags)
{
	கட்டமைப்பு stat st;
	கட்டமைப்பு object_id oid;

	எனில் (fstat(fd, &st) < 0 ||
	    index_fd(the_repository->index, &oid, fd, &st,
		     type_from_string(type), path, flags))
		die((flags & INDEX_WRITE_OBJECT)
		    ? "Unable to add %s to database"
		    : "Unable to hash %s", path);
	printf("%s\n", oid_to_hex(&oid));
	maybe_flush_or_die(stdout, "hash to stdout");
}

நிலையான வெறுமை hash_object(மாறிலி எழுத்து *path, மாறிலி எழுத்து *type, மாறிலி எழுத்து *vpath,
			unsigned flags)
{
	முழு fd;
	fd = xopen(path, O_RDONLY);
	hash_fd(fd, type, vpath, flags);
}

நிலையான வெறுமை hash_stdin_paths(மாறிலி எழுத்து *type, முழு no_filters, unsigned flags)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு strbuf unquoted = STRBUF_INIT;

	போது (strbuf_getline(&buf, stdin) != EOF) {
		எனில் (buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			எனில் (unquote_c_style(&unquoted, buf.buf, வெற்று))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		hash_object(buf.buf, type, no_filters ? வெற்று : buf.buf, flags);
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

முழு cmd_hash_object(முழு argc,
		    மாறிலி எழுத்து **argv,
		    மாறிலி எழுத்து *prefix,
		    கட்டமைப்பு repository *repo UNUSED)
{
	நிலையான மாறிலி எழுத்து * மாறிலி hash_object_usage[] = {
		N_("git hash-object [-t <type>] [-w] [--path=<file> | --no-filters]\n"
		   "                [--stdin [--literally]] [--] <file>..."),
		N_("git hash-object [-t <type>] [-w] --stdin-paths [--no-filters]"),
		வெற்று
	};
	மாறிலி எழுத்து *type = blob_type;
	முழு hashstdin = 0;
	முழு stdin_paths = 0;
	முழு no_filters = 0;
	முழு nongit = 0;
	unsigned flags = INDEX_FORMAT_CHECK;
	மாறிலி எழுத்து *vpath = வெற்று;
	எழுத்து *vpath_free = வெற்று;
	மாறிலி கட்டமைப்பு option hash_object_options[] = {
		OPT_STRING('t', வெற்று, &type, N_("type"), N_("object type")),
		OPT_BIT('w', வெற்று, &flags, N_("write the object into the object database"),
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
	முழு i;
	மாறிலி எழுத்து *errstr = வெற்று;

	argc = parse_options(argc, argv, prefix, hash_object_options,
			     hash_object_usage, 0);

	எனில் (flags & INDEX_WRITE_OBJECT)
		prefix = setup_git_directory(the_repository);
	இல்லையெனில்
		prefix = setup_git_directory_gently(the_repository, &nongit);

	எனில் (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	எனில் (vpath && prefix) {
		vpath_free = prefix_filename(prefix, vpath);
		vpath = vpath_free;
	}

	repo_config(the_repository, git_default_config, வெற்று);

	எனில் (stdin_paths) {
		எனில் (hashstdin)
			errstr = "Can't use --stdin-paths with --stdin";
		இல்லையெனில் எனில் (argc)
			errstr = "Can't specify files with --stdin-paths";
		இல்லையெனில் எனில் (vpath)
			errstr = "Can't use --stdin-paths with --path";
	}
	இல்லையெனில் {
		எனில் (hashstdin > 1)
			errstr = "Multiple --stdin arguments are not supported";
		எனில் (vpath && no_filters)
			errstr = "Can't use --path with --no-filters";
	}

	எனில் (errstr) {
		error("%s", errstr);
		usage_with_options(hash_object_usage, hash_object_options);
	}

	எனில் (hashstdin)
		hash_fd(0, type, vpath, flags);

	க்கு (i = 0 ; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];
		எழுத்து *to_free = வெற்று;

		எனில் (prefix)
			arg = to_free = prefix_filename(prefix, arg);
		hash_object(arg, type, no_filters ? வெற்று : vpath ? vpath : arg,
			    flags);
		விடுவி(to_free);
	}

	எனில் (stdin_paths)
		hash_stdin_paths(type, no_filters, flags);

	விடுவி(vpath_free);

	திரும்பு 0;
}
