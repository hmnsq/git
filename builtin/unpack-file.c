#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "hex.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"

ساكن حرف *create_لنډمهاله_file(بنية object_id *oid)
{
	ساكن حرف لار[50];
	فراغ *buf;
	تعداد object_type type;
	اندازه_t اندازه;
	صحيح fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &اندازه);
	إذا (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_منزل_hex(oid));

	xsnprintf(لار, حجم(لار), ".merge_file_XXXXXX");
	fd = xmkstemp(لار);
	إذا (write_دننه_ډک(fd, buf, اندازه) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	حرر(buf);
	أرجع لار;
}

ساكن ثابت حرف usage_پیغام[] =
"git unpack-file <blob>";

صحيح cmd_unpack_file(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *مختاړی ناکارول_شوی,
		    بنية repository *repo ناکارول_شوی)
{
	بنية object_id oid;

	show_usage_if_asked(argc, argv, usage_پیغام);
	إذا (argc != 2)
		usage(usage_پیغام);
	إذا (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, عدم);

	puts(create_لنډمهاله_file(&oid));
	أرجع 0;
}
