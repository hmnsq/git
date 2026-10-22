/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 * Copyright (C) Junio C Hamano, 2005
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blob.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

ਸਥਿਰ ਖਾਲੀ hash_fd(ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *type, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ flags)
{
	ਬਣਤਰ stat st;
	ਬਣਤਰ object_id oid;

	ਜੇ (fstat(fd, &st) < 0 ||
	    ਸੂਚਕਾਂਕ_fd(the_repository->ਸੂਚਕਾਂਕ, &oid, fd, &st,
		     type_from_string(type), ਰਸਤਾ, flags))
		die((flags & ਸੂਚਕਾਂਕ_WRITE_OBJECT)
		    ? "Unable to add %s to database"
		    : "Unable to hash %s", ਰਸਤਾ);
	ਲਿਖੋ("%s\n", oid_ਮੰਜ਼ਿਲ_hex(&oid));
	maybe_flush_or_die(stdout, "hash to stdout");
}

ਸਥਿਰ ਖਾਲੀ hash_object(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *type, ਅਚਲ ਅੱਖਰ *vpath,
			ਬਿਨਾਨਿਸ਼ਾਨੀ flags)
{
	ਪੂਰਨ fd;
	fd = xopen(ਰਸਤਾ, O_RDONLY);
	hash_fd(fd, type, vpath, flags);
}

ਸਥਿਰ ਖਾਲੀ hash_stdin_paths(ਅਚਲ ਅੱਖਰ *type, ਪੂਰਨ no_filters, ਬਿਨਾਨਿਸ਼ਾਨੀ flags)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ strbuf unquoted = STRBUF_INIT;

	ਜਦੋਂ (strbuf_getline(&buf, stdin) != EOF) {
		ਜੇ (buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ਜੇ (unquote_c_style(&unquoted, buf.buf, ਨਿਰਾ))
				die("line is badly quoted");
			strbuf_ਅਦਲਾ_ਬਦਲੀ(&buf, &unquoted);
		}
		hash_object(buf.buf, type, no_filters ? ਨਿਰਾ : buf.buf, flags);
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

ਪੂਰਨ cmd_hash_object(ਪੂਰਨ argc,
		    ਅਚਲ ਅੱਖਰ **argv,
		    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ hash_object_usage[] = {
		N_("git hash-object [-t <type>] [-w] [--path=<file> | --no-filters]\n"
		   "                [--stdin [--literally]] [--] <file>..."),
		N_("git hash-object [-t <type>] [-w] --stdin-paths [--no-filters]"),
		ਨਿਰਾ
	};
	ਅਚਲ ਅੱਖਰ *type = blob_type;
	ਪੂਰਨ hashstdin = 0;
	ਪੂਰਨ stdin_paths = 0;
	ਪੂਰਨ no_filters = 0;
	ਪੂਰਨ nongit = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ flags = ਸੂਚਕਾਂਕ_FORMAT_CHECK;
	ਅਚਲ ਅੱਖਰ *vpath = ਨਿਰਾ;
	ਅੱਖਰ *vpath_free = ਨਿਰਾ;
	ਅਚਲ ਬਣਤਰ option hash_object_options[] = {
		OPT_STRING('t', ਨਿਰਾ, &type, N_("type"), N_("object type")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('w', ਨਿਰਾ, &flags, N_("write the object into the object database"),
			ਸੂਚਕਾਂਕ_WRITE_OBJECT),
		OPT_COUNTUP( 0 , "stdin", &hashstdin, N_("read the object from stdin")),
		OPT_BOOL( 0 , "stdin-paths", &stdin_paths, N_("read file names from stdin")),
		OPT_BOOL( 0 , "no-filters", &no_filters, N_("store file as is without filters")),
		OPT_NEGBIT( 0, "literally", &flags,
			    N_("just hash any random garbage to create corrupt objects for debugging Git"),
			    ਸੂਚਕਾਂਕ_FORMAT_CHECK),
		OPT_STRING( 0 , "path", &vpath, N_("file"), N_("process file as it were from this path")),
		OPT_ਸਿਰਾ()
	};
	ਪੂਰਨ i;
	ਅਚਲ ਅੱਖਰ *errstr = ਨਿਰਾ;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, hash_object_options,
			     hash_object_usage, 0);

	ਜੇ (flags & ਸੂਚਕਾਂਕ_WRITE_OBJECT)
		ਅਗੇਤਰ = ਤਿਆਰੀ_git_directory(the_repository);
	ਨਹੀਂਤਾਂ
		ਅਗੇਤਰ = ਤਿਆਰੀ_git_directory_gently(the_repository, &nongit);

	ਜੇ (nongit && !the_hash_algo)
		repo_ਸੈੱਟ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	ਜੇ (vpath && ਅਗੇਤਰ) {
		vpath_free = ਅਗੇਤਰ_filename(ਅਗੇਤਰ, vpath);
		vpath = vpath_free;
	}

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	ਜੇ (stdin_paths) {
		ਜੇ (hashstdin)
			errstr = "Can't use --stdin-paths with --stdin";
		ਨਹੀਂਤਾਂ ਜੇ (argc)
			errstr = "Can't specify files with --stdin-paths";
		ਨਹੀਂਤਾਂ ਜੇ (vpath)
			errstr = "Can't use --stdin-paths with --path";
	}
	ਨਹੀਂਤਾਂ {
		ਜੇ (hashstdin > 1)
			errstr = "Multiple --stdin arguments are not supported";
		ਜੇ (vpath && no_filters)
			errstr = "Can't use --path with --no-filters";
	}

	ਜੇ (errstr) {
		error("%s", errstr);
		usage_with_options(hash_object_usage, hash_object_options);
	}

	ਜੇ (hashstdin)
		hash_fd(0, type, vpath, flags);

	ਲਈ (i = 0 ; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];
		ਅੱਖਰ *ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;

		ਜੇ (ਅਗੇਤਰ)
			arg = ਮੰਜ਼ਿਲ_free = ਅਗੇਤਰ_filename(ਅਗੇਤਰ, arg);
		hash_object(arg, type, no_filters ? ਨਿਰਾ : vpath ? vpath : arg,
			    flags);
		ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);
	}

	ਜੇ (stdin_paths)
		hash_stdin_paths(type, no_filters, flags);

	ਮੁਕਤ(vpath_free);

	ਵਾਪਸ 0;
}
