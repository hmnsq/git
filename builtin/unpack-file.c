#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "hex.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"

ساكن حرف *create_temp_file(بنية object_id *oid)
{
	ساكن حرف path[50];
	فراغ *buf;
	تعداد object_type type;
	size_t size;
	صحيح fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &size);
	إذا (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, حجم(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	إذا (write_in_full(fd, buf, size) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	حرر(buf);
	أرجع path;
}

ساكن ثابت حرف usage_ڤسنن[] =
"git unpack-file <blob>";

صحيح cmd_unpack_file(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *prefix UNUSED,
		    بنية repository *repo UNUSED)
{
	بنية object_id oid;

	show_usage_if_asked(argc, argv, usage_ڤسنن);
	إذا (argc != 2)
		usage(usage_ڤسنن);
	إذا (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, عدم);

	puts(create_temp_file(&oid));
	أرجع 0;
}
