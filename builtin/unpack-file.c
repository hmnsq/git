#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "hex.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"

ساكن حرف *create_عارضي_file(بنية object_id *oid)
{
	ساكن حرف رستو[50];
	فراغ *buf;
	تعداد object_type type;
	سائيز_t سائيز;
	صحيح fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &سائيز);
	إذا (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_منزل_hex(oid));

	xsnprintf(رستو, حجم(رستو), ".merge_file_XXXXXX");
	fd = xmkstemp(رستو);
	إذا (write_اندريون_ڀريل(fd, buf, سائيز) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	حرر(buf);
	أرجع رستو;
}

ساكن ثابت حرف usage_نياپو[] =
"git unpack-file <blob>";

صحيح cmd_unpack_file(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل,
		    بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية object_id oid;

	show_usage_if_asked(argc, argv, usage_نياپو);
	إذا (argc != 2)
		usage(usage_نياپو);
	إذا (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, عدم);

	puts(create_عارضي_file(&oid));
	أرجع 0;
}
