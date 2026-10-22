/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 * Copyright (C) Junio C Hamano, 2005
 */
#परिभाषय USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "write-or-die.h"

स्थिर शून्य hash_fd(पूर्णाङ्क fd, ध्रुव वर्ण *type, ध्रुव वर्ण *मार्गः, अचिह्नित flags)
{
	संरचना stat st;
	संरचना object_id oid;

	यदि (fstat(fd, &st) < 0 ||
	    सूचकांक_fd(the_repository->सूचकांक, &oid, fd, &st,
		     type_from_string(type), मार्गः, flags))
		die((flags & सूचकांक_WRITE_OBJECT)
		    ? "Unable to add %s to database"
		    : "Unable to hash %s", मार्गः);
	प्रारूप_लिख("%s\n", oid_गन्तव्यम्_hex(&oid));
	maybe_flush_or_die(stdout, "hash to stdout");
}

स्थिर शून्य hash_object(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *type, ध्रुव वर्ण *vpath,
			अचिह्नित flags)
{
	पूर्णाङ्क fd;
	fd = xopen(मार्गः, O_RDONLY);
	hash_fd(fd, type, vpath, flags);
}

स्थिर शून्य hash_stdin_paths(ध्रुव वर्ण *type, पूर्णाङ्क no_filters, अचिह्नित flags)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf unquoted = STRBUF_INIT;

	यावत् (strbuf_getline(&buf, stdin) != EOF) {
		यदि (buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			यदि (unquote_c_style(&unquoted, buf.buf, रिक्त))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		hash_object(buf.buf, type, no_filters ? रिक्त : buf.buf, flags);
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

पूर्णाङ्क cmd_hash_object(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *उपसर्गः,
		    संरचना repository *repo अप्रयुक्तम्)
{
	स्थिर ध्रुव वर्ण * ध्रुव hash_object_usage[] = {
		N_("git hash-object [-t <type>] [-w] [--path=<file> | --no-filters]\n"
		   "                [--stdin [--literally]] [--] <file>..."),
		N_("git hash-object [-t <type>] [-w] --stdin-paths [--no-filters]"),
		रिक्त
	};
	ध्रुव वर्ण *type = blob_type;
	पूर्णाङ्क hashstdin = 0;
	पूर्णाङ्क stdin_paths = 0;
	पूर्णाङ्क no_filters = 0;
	पूर्णाङ्क nongit = 0;
	अचिह्नित flags = सूचकांक_FORMAT_CHECK;
	ध्रुव वर्ण *vpath = रिक्त;
	वर्ण *vpath_free = रिक्त;
	ध्रुव संरचना option hash_object_options[] = {
		OPT_STRING('t', रिक्त, &type, N_("type"), N_("object type")),
		OPT_BIT('w', रिक्त, &flags, N_("write the object into the object database"),
			सूचकांक_WRITE_OBJECT),
		OPT_COUNTUP( 0 , "stdin", &hashstdin, N_("read the object from stdin")),
		OPT_BOOL( 0 , "stdin-paths", &stdin_paths, N_("read file names from stdin")),
		OPT_BOOL( 0 , "no-filters", &no_filters, N_("store file as is without filters")),
		OPT_NEGBIT( 0, "literally", &flags,
			    N_("just hash any random garbage to create corrupt objects for debugging Git"),
			    सूचकांक_FORMAT_CHECK),
		OPT_STRING( 0 , "path", &vpath, N_("file"), N_("process file as it were from this path")),
		OPT_अन्तः()
	};
	पूर्णाङ्क i;
	ध्रुव वर्ण *errstr = रिक्त;

	argc = parse_options(argc, argv, उपसर्गः, hash_object_options,
			     hash_object_usage, 0);

	यदि (flags & सूचकांक_WRITE_OBJECT)
		उपसर्गः = सज्जता_git_directory(the_repository);
	अन्यथा
		उपसर्गः = सज्जता_git_directory_gently(the_repository, &nongit);

	यदि (nongit && !the_hash_algo)
		repo_समुच्चयः_hash_algo(the_repository, GIT_HASH_DEFAULT);

	यदि (vpath && उपसर्गः) {
		vpath_free = उपसर्गः_filename(उपसर्गः, vpath);
		vpath = vpath_free;
	}

	repo_config(the_repository, git_default_config, रिक्त);

	यदि (stdin_paths) {
		यदि (hashstdin)
			errstr = "Can't use --stdin-paths with --stdin";
		अन्यथा यदि (argc)
			errstr = "Can't specify files with --stdin-paths";
		अन्यथा यदि (vpath)
			errstr = "Can't use --stdin-paths with --path";
	}
	अन्यथा {
		यदि (hashstdin > 1)
			errstr = "Multiple --stdin arguments are not supported";
		यदि (vpath && no_filters)
			errstr = "Can't use --path with --no-filters";
	}

	यदि (errstr) {
		error("%s", errstr);
		usage_with_options(hash_object_usage, hash_object_options);
	}

	यदि (hashstdin)
		hash_fd(0, type, vpath, flags);

	प्रति (i = 0 ; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		वर्ण *गन्तव्यम्_free = रिक्त;

		यदि (उपसर्गः)
			arg = गन्तव्यम्_free = उपसर्गः_filename(उपसर्गः, arg);
		hash_object(arg, type, no_filters ? रिक्त : vpath ? vpath : arg,
			    flags);
		मुक्त(गन्तव्यम्_free);
	}

	यदि (stdin_paths)
		hash_stdin_paths(type, no_filters, flags);

	मुक्त(vpath_free);

	प्रत्यागम 0;
}
