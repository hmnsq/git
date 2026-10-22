#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "hex.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"

ساكن حرف *create_موقت_file(بنية object_id *oid)
{
	ساكن حرف مسیر[50];
	فراغ *buf;
	تعداد object_type type;
	اندازه_t اندازه;
	صحيح fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &اندازه);
	إذا (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_مقصد_hex(oid));

	xsnprintf(مسیر, حجم(مسیر), ".merge_file_XXXXXX");
	fd = xmkstemp(مسیر);
	إذا (write_درون_پر(fd, buf, اندازه) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	حرر(buf);
	أرجع مسیر;
}

ساكن ثابت حرف usage_پیام[] =
"git unpack-file <blob>";

صحيح cmd_unpack_file(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *پیشوند استفادهنشده,
		    بنية repository *repo استفادهنشده)
{
	بنية object_id oid;

	show_usage_if_asked(argc, argv, usage_پیام);
	إذا (argc != 2)
		usage(usage_پیام);
	إذا (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, عدم);

	puts(create_موقت_file(&oid));
	أرجع 0;
}
