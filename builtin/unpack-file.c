#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "hex.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "odb.h"

ساکن حرف *create_عارضی_file(ساخت object_id *oid)
{
	ساکن حرف راستہ[50];
	خالی *buf;
	شمار object_type type;
	سائز_t سائز;
	صحیح fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &سائز);
	اگر (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_مقصد_hex(oid));

	xsnprintf(راستہ, حجم(راستہ), ".merge_file_XXXXXX");
	fd = xmkstemp(راستہ);
	اگر (write_اندر_بھرا(fd, buf, سائز) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	آزاد(buf);
	واپس راستہ;
}

ساکن ثابت حرف usage_پیغام[] =
"git unpack-file <blob>";

صحیح cmd_unpack_file(صحیح argc,
		    ثابت حرف **argv,
		    ثابت حرف *سابقہ غیر_مستعمل,
		    ساخت repository *repo غیر_مستعمل)
{
	ساخت object_id oid;

	show_usage_if_asked(argc, argv, usage_پیغام);
	اگر (argc != 2)
		usage(usage_پیغام);
	اگر (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, تہی);

	puts(create_عارضی_file(&oid));
	واپس 0;
}
