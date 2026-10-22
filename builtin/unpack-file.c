#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"

நிலையான எழுத்து *create_temp_file(கட்டமைப்பு object_id *oid)
{
	நிலையான எழுத்து path[50];
	வெறுமை *buf;
	எண்ணிக்கை object_type type;
	size_t size;
	முழு fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &size);
	எனில் (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, அளவு(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	எனில் (write_in_full(fd, buf, size) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	விடுவி(buf);
	திரும்பு path;
}

நிலையான மாறிலி எழுத்து usage_செய்தி[] =
"git unpack-file <blob>";

முழு cmd_unpack_file(முழு argc,
		    மாறிலி எழுத்து **argv,
		    மாறிலி எழுத்து *prefix UNUSED,
		    கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு object_id oid;

	show_usage_if_asked(argc, argv, usage_செய்தி);
	எனில் (argc != 2)
		usage(usage_செய்தி);
	எனில் (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, வெற்று);

	puts(create_temp_file(&oid));
	திரும்பு 0;
}
