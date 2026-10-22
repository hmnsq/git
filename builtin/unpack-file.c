#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"

স্থির অক্ষর *create_অস্থায়ী_file(কাঠামো object_id *oid)
{
	স্থির অক্ষর path[50];
	শূন্য *buf;
	গণনা object_type type;
	আকার_t আকার_2;
	পূর্ণসংখ্যা fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &আকার_2);
	যদি (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, আকার(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	যদি (write_in_full(fd, buf, আকার_2) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	মুক্ত(buf);
	ফেরত path;
}

স্থির ধ্রুবক অক্ষর usage_বার্তা[] =
"git unpack-file <blob>";

পূর্ণসংখ্যা cmd_unpack_file(পূর্ণসংখ্যা argc,
		    ধ্রুবক অক্ষর **argv,
		    ধ্রুবক অক্ষর *prefix UNUSED,
		    কাঠামো repository *repo UNUSED)
{
	কাঠামো object_id oid;

	show_usage_if_asked(argc, argv, usage_বার্তা);
	যদি (argc != 2)
		usage(usage_বার্তা);
	যদি (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, ফাঁকা);

	puts(create_অস্থায়ী_file(&oid));
	ফেরত 0;
}
