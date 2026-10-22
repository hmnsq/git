#परिभाषय USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"

स्थिर वर्ण *create_अस्थायी_file(संरचना object_id *oid)
{
	स्थिर वर्ण मार्गः[50];
	शून्य *buf;
	गणना object_type type;
	आकार_t आकार;
	पूर्णाङ्क fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &आकार);
	यदि (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_गन्तव्यम्_hex(oid));

	xsnprintf(मार्गः, परिमाण(मार्गः), ".merge_file_XXXXXX");
	fd = xmkstemp(मार्गः);
	यदि (write_अन्तर्_पूर्णम्(fd, buf, आकार) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	मुक्त(buf);
	प्रत्यागम मार्गः;
}

स्थिर ध्रुव वर्ण usage_सन्देशः[] =
"git unpack-file <blob>";

पूर्णाङ्क cmd_unpack_file(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
		    संरचना repository *repo अप्रयुक्तम्)
{
	संरचना object_id oid;

	show_usage_if_asked(argc, argv, usage_सन्देशः);
	यदि (argc != 2)
		usage(usage_सन्देशः);
	यदि (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, रिक्त);

	पङ्क्तिं_लिख(create_अस्थायी_file(&oid));
	प्रत्यागम 0;
}
