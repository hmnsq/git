#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"

स्थिर वर्ण *create_temp_file(संरचना object_id *oid)
{
	स्थिर वर्ण path[50];
	शून्य *buf;
	गणना object_type type;
	size_t size;
	पूर्णाङ्क fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &size);
	सचे (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, परिमाण(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	सचे (write_in_full(fd, buf, size) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	मुक्त(buf);
	पटिनिवत्त path;
}

स्थिर ध्रुव वर्ण usage_सन्देशः[] =
"git unpack-file <blob>";

पूर्णाङ्क cmd_unpack_file(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *prefix UNUSED,
		    संरचना repository *repo UNUSED)
{
	संरचना object_id oid;

	show_usage_if_asked(argc, argv, usage_सन्देशः);
	सचे (argc != 2)
		usage(usage_सन्देशः);
	सचे (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, रिक्त);

	puts(create_temp_file(&oid));
	पटिनिवत्त 0;
}
