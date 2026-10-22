#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "object-file.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "odb.h"

স্থির ৱর্ণ *create_temp_file(সংরচনা object_id *oid)
{
	স্থির ৱর্ণ path[50];
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
	ঘূৰাই path;
}

স্থির ধ্রুৱ ৱর্ণ usage_বাৰ্তা[] =
"git unpack-file <blob>";

পূর্ণাঙ্ক cmd_unpack_file(পূর্ণাঙ্ক argc,
		    ধ্রুৱ ৱর্ণ **argv,
		    ধ্রুৱ ৱর্ণ *prefix UNUSED,
		    সংরচনা repository *repo UNUSED)
{
	সংরচনা object_id oid;

	show_usage_if_asked(argc, argv, usage_বাৰ্তা);
	যদি (argc != 2)
		usage(usage_বাৰ্তা);
	যদি (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, রিক্ত);

	puts(create_temp_file(&oid));
	ঘূৰাই 0;
}
