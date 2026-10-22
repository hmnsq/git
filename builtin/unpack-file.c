#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "hex.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "odb.h"

ایستا نویسه *create_temp_file(ساختار object_id *oid)
{
	ایستا نویسه path[50];
	تهی *buf;
	شمارش object_type type;
	size_t size;
	صحیح fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &size);
	اگر (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, اندازه(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	اگر (write_in_full(fd, buf, size) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	آزاد(buf);
	بازگشت path;
}

ایستا ثابت نویسه usage_پیام[] =
"git unpack-file <blob>";

صحیح cmd_unpack_file(صحیح argc,
		    ثابت نویسه **argv,
		    ثابت نویسه *prefix UNUSED,
		    ساختار repository *repo UNUSED)
{
	ساختار object_id oid;

	show_usage_if_asked(argc, argv, usage_پیام);
	اگر (argc != 2)
		usage(usage_پیام);
	اگر (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, پوچ);

	puts(create_temp_file(&oid));
	بازگشت 0;
}
