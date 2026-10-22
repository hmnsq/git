/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 * Copyright (C) Junio C Hamano, 2005
 */
#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "blob.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "write-or-die.h"

స్థిర శూన్య hash_fd(పూర్ణాంకం fd, స్థిరరాశి అక్షరం *type, స్థిరరాశి అక్షరం *మార్గం, గుర్తులేని flags)
{
	నిర్మాణం stat st;
	నిర్మాణం object_id oid;

	ఒకవేళ (fstat(fd, &st) < 0 ||
	    సూచీ_fd(the_repository->సూచీ, &oid, fd, &st,
		     type_from_string(type), మార్గం, flags))
		die((flags & సూచీ_WRITE_OBJECT)
		    ? "Unable to add %s to database"
		    : "Unable to hash %s", మార్గం);
	రాయి("%s\n", oid_గమ్యం_hex(&oid));
	maybe_flush_or_die(stdout, "hash to stdout");
}

స్థిర శూన్య hash_object(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *type, స్థిరరాశి అక్షరం *vpath,
			గుర్తులేని flags)
{
	పూర్ణాంకం fd;
	fd = xopen(మార్గం, O_RDONLY);
	hash_fd(fd, type, vpath, flags);
}

స్థిర శూన్య hash_stdin_paths(స్థిరరాశి అక్షరం *type, పూర్ణాంకం no_filters, గుర్తులేని flags)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం strbuf unquoted = STRBUF_INIT;

	అయితే (strbuf_getline(&buf, stdin) != EOF) {
		ఒకవేళ (buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ఒకవేళ (unquote_c_style(&unquoted, buf.buf, ఖాళీ))
				die("line is badly quoted");
			strbuf_మార్పిడి(&buf, &unquoted);
		}
		hash_object(buf.buf, type, no_filters ? ఖాళీ : buf.buf, flags);
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
}

పూర్ణాంకం cmd_hash_object(పూర్ణాంకం argc,
		    స్థిరరాశి అక్షరం **argv,
		    స్థిరరాశి అక్షరం *ఉపసర్గ,
		    నిర్మాణం repository *repo వాడని)
{
	స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి hash_object_usage[] = {
		N_("git hash-object [-t <type>] [-w] [--path=<file> | --no-filters]\n"
		   "                [--stdin [--literally]] [--] <file>..."),
		N_("git hash-object [-t <type>] [-w] --stdin-paths [--no-filters]"),
		ఖాళీ
	};
	స్థిరరాశి అక్షరం *type = blob_type;
	పూర్ణాంకం hashstdin = 0;
	పూర్ణాంకం stdin_paths = 0;
	పూర్ణాంకం no_filters = 0;
	పూర్ణాంకం nongit = 0;
	గుర్తులేని flags = సూచీ_FORMAT_CHECK;
	స్థిరరాశి అక్షరం *vpath = ఖాళీ;
	అక్షరం *vpath_free = ఖాళీ;
	స్థిరరాశి నిర్మాణం option hash_object_options[] = {
		OPT_STRING('t', ఖాళీ, &type, N_("type"), N_("object type")),
		OPT_బిట్టు('w', ఖాళీ, &flags, N_("write the object into the object database"),
			సూచీ_WRITE_OBJECT),
		OPT_COUNTUP( 0 , "stdin", &hashstdin, N_("read the object from stdin")),
		OPT_BOOL( 0 , "stdin-paths", &stdin_paths, N_("read file names from stdin")),
		OPT_BOOL( 0 , "no-filters", &no_filters, N_("store file as is without filters")),
		OPT_NEGBIT( 0, "literally", &flags,
			    N_("just hash any random garbage to create corrupt objects for debugging Git"),
			    సూచీ_FORMAT_CHECK),
		OPT_STRING( 0 , "path", &vpath, N_("file"), N_("process file as it were from this path")),
		OPT_చివర()
	};
	పూర్ణాంకం i;
	స్థిరరాశి అక్షరం *errstr = ఖాళీ;

	argc = parse_options(argc, argv, ఉపసర్గ, hash_object_options,
			     hash_object_usage, 0);

	ఒకవేళ (flags & సూచీ_WRITE_OBJECT)
		ఉపసర్గ = అమరిక_git_directory(the_repository);
	లేకపోతే
		ఉపసర్గ = అమరిక_git_directory_gently(the_repository, &nongit);

	ఒకవేళ (nongit && !the_hash_algo)
		repo_సమితి_hash_algo(the_repository, GIT_HASH_DEFAULT);

	ఒకవేళ (vpath && ఉపసర్గ) {
		vpath_free = ఉపసర్గ_filename(ఉపసర్గ, vpath);
		vpath = vpath_free;
	}

	repo_config(the_repository, git_default_config, ఖాళీ);

	ఒకవేళ (stdin_paths) {
		ఒకవేళ (hashstdin)
			errstr = "Can't use --stdin-paths with --stdin";
		లేకపోతే ఒకవేళ (argc)
			errstr = "Can't specify files with --stdin-paths";
		లేకపోతే ఒకవేళ (vpath)
			errstr = "Can't use --stdin-paths with --path";
	}
	లేకపోతే {
		ఒకవేళ (hashstdin > 1)
			errstr = "Multiple --stdin arguments are not supported";
		ఒకవేళ (vpath && no_filters)
			errstr = "Can't use --path with --no-filters";
	}

	ఒకవేళ (errstr) {
		error("%s", errstr);
		usage_with_options(hash_object_usage, hash_object_options);
	}

	ఒకవేళ (hashstdin)
		hash_fd(0, type, vpath, flags);

	కొరకు (i = 0 ; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];
		అక్షరం *గమ్యం_free = ఖాళీ;

		ఒకవేళ (ఉపసర్గ)
			arg = గమ్యం_free = ఉపసర్గ_filename(ఉపసర్గ, arg);
		hash_object(arg, type, no_filters ? ఖాళీ : vpath ? vpath : arg,
			    flags);
		విడుదల(గమ్యం_free);
	}

	ఒకవేళ (stdin_paths)
		hash_stdin_paths(type, no_filters, flags);

	విడుదల(vpath_free);

	తిరిగి 0;
}
