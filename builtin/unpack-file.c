#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "hex.h"
#समावेश "object-file.h"
#समावेश "object-name.h"
#समावेश "odb.h"

अटल अक्षर *create_अस्थायी_file(काष्ठा object_id *oid)
{
	अटल अक्षर path[50];
	व्योम *buf;
	क्रमागत object_type type;
	माप_प्रकार आकार;
	पूर्णांक fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &आकार);
	अगर (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, माप(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	अगर (write_in_full(fd, buf, आकार) < 0)
		die_errno("unable to write temp-file");
	बंद(fd);
	मुक्त(buf);
	वापस path;
}

अटल स्थिर अक्षर usage_संदेश[] =
"git unpack-file <blob>";

पूर्णांक cmd_unpack_file(पूर्णांक argc,
		    स्थिर अक्षर **argv,
		    स्थिर अक्षर *prefix UNUSED,
		    काष्ठा repository *repo UNUSED)
{
	काष्ठा object_id oid;

	show_usage_if_asked(argc, argv, usage_संदेश);
	अगर (argc != 2)
		usage(usage_संदेश);
	अगर (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, शून्य);

	माला_दो(create_अस्थायी_file(&oid));
	वापस 0;
}
