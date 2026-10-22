/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 * Copyright (C) Junio C Hamano, 2005
 */
#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "object-file.h"
#समावेश "odb.h"
#समावेश "blob.h"
#समावेश "quote.h"
#समावेश "parse-options.h"
#समावेश "setup.h"
#समावेश "strbuf.h"
#समावेश "write-or-die.h"

अटल व्योम hash_fd(पूर्णांक fd, स्थिर अक्षर *type, स्थिर अक्षर *path, अचिन्हित flags)
{
	काष्ठा stat st;
	काष्ठा object_id oid;

	अगर (ख_स्थिति(fd, &st) < 0 ||
	    सूचकांक_fd(the_repository->सूचकांक, &oid, fd, &st,
		     type_from_string(type), path, flags))
		die((flags & सूचकांक_WRITE_OBJECT)
		    ? "Unable to add %s to database"
		    : "Unable to hash %s", path);
	म_लिखो("%s\n", oid_to_hex(&oid));
	maybe_flush_or_die(मानक_निकास, "hash to stdout");
}

अटल व्योम hash_object(स्थिर अक्षर *path, स्थिर अक्षर *type, स्थिर अक्षर *vpath,
			अचिन्हित flags)
{
	पूर्णांक fd;
	fd = xopen(path, O_RDONLY);
	hash_fd(fd, type, vpath, flags);
}

अटल व्योम hash_stdin_paths(स्थिर अक्षर *type, पूर्णांक no_filters, अचिन्हित flags)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा strbuf unquoted = STRBUF_INIT;

	जबतक (strbuf_getline(&buf, मानक_निवेश) != खातापूर्ण) {
		अगर (buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			अगर (unquote_c_style(&unquoted, buf.buf, शून्य))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		hash_object(buf.buf, type, no_filters ? शून्य : buf.buf, flags);
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

पूर्णांक cmd_hash_object(पूर्णांक argc,
		    स्थिर अक्षर **argv,
		    स्थिर अक्षर *prefix,
		    काष्ठा repository *repo UNUSED)
{
	अटल स्थिर अक्षर * स्थिर hash_object_usage[] = {
		N_("git hash-object [-t <type>] [-w] [--path=<file> | --no-filters]\n"
		   "                [--stdin [--literally]] [--] <file>..."),
		N_("git hash-object [-t <type>] [-w] --stdin-paths [--no-filters]"),
		शून्य
	};
	स्थिर अक्षर *type = blob_type;
	पूर्णांक hashstdin = 0;
	पूर्णांक stdin_paths = 0;
	पूर्णांक no_filters = 0;
	पूर्णांक nongit = 0;
	अचिन्हित flags = सूचकांक_FORMAT_CHECK;
	स्थिर अक्षर *vpath = शून्य;
	अक्षर *vpath_free = शून्य;
	स्थिर काष्ठा option hash_object_options[] = {
		OPT_STRING('t', शून्य, &type, N_("type"), N_("object type")),
		OPT_BIT('w', शून्य, &flags, N_("write the object into the object database"),
			सूचकांक_WRITE_OBJECT),
		OPT_COUNTUP( 0 , "stdin", &hashstdin, N_("read the object from stdin")),
		OPT_BOOL( 0 , "stdin-paths", &stdin_paths, N_("read file names from stdin")),
		OPT_BOOL( 0 , "no-filters", &no_filters, N_("store file as is without filters")),
		OPT_NEGBIT( 0, "literally", &flags,
			    N_("just hash any random garbage to create corrupt objects for debugging Git"),
			    सूचकांक_FORMAT_CHECK),
		OPT_STRING( 0 , "path", &vpath, N_("file"), N_("process file as it were from this path")),
		OPT_END()
	};
	पूर्णांक i;
	स्थिर अक्षर *errstr = शून्य;

	argc = parse_options(argc, argv, prefix, hash_object_options,
			     hash_object_usage, 0);

	अगर (flags & सूचकांक_WRITE_OBJECT)
		prefix = setup_git_directory(the_repository);
	अन्यथा
		prefix = setup_git_directory_gently(the_repository, &nongit);

	अगर (nongit && !the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	अगर (vpath && prefix) {
		vpath_free = prefix_filename(prefix, vpath);
		vpath = vpath_free;
	}

	repo_config(the_repository, git_default_config, शून्य);

	अगर (stdin_paths) {
		अगर (hashstdin)
			errstr = "Can't use --stdin-paths with --stdin";
		अन्यथा अगर (argc)
			errstr = "Can't specify files with --stdin-paths";
		अन्यथा अगर (vpath)
			errstr = "Can't use --stdin-paths with --path";
	}
	अन्यथा {
		अगर (hashstdin > 1)
			errstr = "Multiple --stdin arguments are not supported";
		अगर (vpath && no_filters)
			errstr = "Can't use --path with --no-filters";
	}

	अगर (errstr) {
		error("%s", errstr);
		usage_with_options(hash_object_usage, hash_object_options);
	}

	अगर (hashstdin)
		hash_fd(0, type, vpath, flags);

	क्रम (i = 0 ; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];
		अक्षर *to_free = शून्य;

		अगर (prefix)
			arg = to_free = prefix_filename(prefix, arg);
		hash_object(arg, type, no_filters ? शून्य : vpath ? vpath : arg,
			    flags);
		मुक्त(to_free);
	}

	अगर (stdin_paths)
		hash_stdin_paths(type, no_filters, flags);

	मुक्त(vpath_free);

	वापस 0;
}
