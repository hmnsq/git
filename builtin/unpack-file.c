#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"

স্থির বর্ণ *create_temp_file(সংরচনা object_id *oid)
{
	স্থির বর্ণ path[50];
	শূন্য *buf;
	গণনা object_type type;
	size_t size;
	পূর্ণাঙ্ক fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &size);
	যদি (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, পরিমাণ(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	যদি (write_in_full(fd, buf, size) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	মুক্ত(buf);
	প্রত্যাগম path;
}

স্থির ধ্রুব বর্ণ usage_বার্তা[] =
"git unpack-file <blob>";

পূর্ণাঙ্ক cmd_unpack_file(পূর্ণাঙ্ক argc,
		    ধ্রুব বর্ণ **argv,
		    ধ্রুব বর্ণ *prefix UNUSED,
		    সংরচনা repository *repo UNUSED)
{
	সংরচনা object_id oid;

	show_usage_if_asked(argc, argv, usage_বার্তা);
	যদি (argc != 2)
		usage(usage_বার্তা);
	যদি (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, রিক্ত);

	puts(create_temp_file(&oid));
	প্রত্যাগম 0;
}
