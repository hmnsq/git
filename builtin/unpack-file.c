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
	माप_प्रकार size;
	पूर्णाङ्क fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &size);
	यदि (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, परिमाण(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	यदि (write_in_full(fd, buf, size) < 0)
		die_errno("unable to write temp-file");
	बंद(fd);
	मुक्त(buf);
	फर्काऊ path;
}

स्थिर अचर वर्ण usage_सन्देश[] =
"git unpack-file <blob>";

पूर्णाङ्क cmd_unpack_file(पूर्णाङ्क argc,
		    अचर वर्ण **argv,
		    अचर वर्ण *prefix UNUSED,
		    संरचना repository *repo UNUSED)
{
	संरचना object_id oid;

	show_usage_if_asked(argc, argv, usage_सन्देश);
	यदि (argc != 2)
		usage(usage_सन्देश);
	यदि (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, रिक्त);

	माला_दो(create_temp_file(&oid));
	फर्काऊ 0;
}
