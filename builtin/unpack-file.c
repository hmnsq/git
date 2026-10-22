#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"

സ്ഥിതം അക്ഷരം *create_temp_file(ഘടന object_id *oid)
{
	സ്ഥിതം അക്ഷരം path[50];
	ശൂന്യം *buf;
	എണ്ണൽ object_type type;
	size_t size;
	പൂർണ്ണസംഖ്യ fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &size);
	എങ്കിൽ (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, വലുപ്പം(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	എങ്കിൽ (write_in_full(fd, buf, size) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	സ്വതന്ത്രം(buf);
	തിരികെ path;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം usage_സന്ദേശം[] =
"git unpack-file <blob>";

പൂർണ്ണസംഖ്യ cmd_unpack_file(പൂർണ്ണസംഖ്യ argc,
		    സ്ഥിരം അക്ഷരം **argv,
		    സ്ഥിരം അക്ഷരം *prefix UNUSED,
		    ഘടന repository *repo UNUSED)
{
	ഘടന object_id oid;

	show_usage_if_asked(argc, argv, usage_സന്ദേശം);
	എങ്കിൽ (argc != 2)
		usage(usage_സന്ദേശം);
	എങ്കിൽ (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, ശൂന്യ);

	puts(create_temp_file(&oid));
	തിരികെ 0;
}
