#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "hex.h"
#include "object-file.h"
#include "object-name.h"
#include "odb.h"

static char *create_temp_file(struct object_id *oid)
{
	static char path[50];
	void *buf;
	enum object_type type;
	size_t size;
	정수 fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &size);
	만약 (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, sizeof(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	만약 (write_in_full(fd, buf, size) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	free(buf);
	반환 path;
}

static const char usage_msg[] =
"git unpack-file <blob>";

정수 cmd_unpack_file(정수 argc,
		    const char **argv,
		    const char *prefix UNUSED,
		    struct repository *repo UNUSED)
{
	struct object_id oid;

	show_usage_if_asked(argc, argv, usage_msg);
	만약 (argc != 2)
		usage(usage_msg);
	만약 (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, NULL);

	puts(create_temp_file(&oid));
	반환 0;
}
