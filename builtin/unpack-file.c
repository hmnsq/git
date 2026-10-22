#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"

ಸ್ಥಿರ ವರ್ಣ *create_temp_file(ಸಂರಚನಾ object_id *oid)
{
	ಸ್ಥಿರ ವರ್ಣ path[50];
	ಶೂನ್ಯ *buf;
	ಗಣನಾ object_type type;
	size_t size;
	ಪೂರ್ಣಾಙ್ಕ fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &size);
	ಯದಿ (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, ಪರಿಮಾಣ(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	ಯದಿ (write_in_full(fd, buf, size) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	ಮುಕ್ತ(buf);
	ಪ್ರತ್ಯಾಗಮ path;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ usage_ಸಂದೇಶ[] =
"git unpack-file <blob>";

ಪೂರ್ಣಾಙ್ಕ cmd_unpack_file(ಪೂರ್ಣಾಙ್ಕ argc,
		    ಧ್ರುವ ವರ್ಣ **argv,
		    ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
		    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ object_id oid;

	show_usage_if_asked(argc, argv, usage_ಸಂದೇಶ);
	ಯದಿ (argc != 2)
		usage(usage_ಸಂದೇಶ);
	ಯದಿ (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	puts(create_temp_file(&oid));
	ಪ್ರತ್ಯಾಗಮ 0;
}
