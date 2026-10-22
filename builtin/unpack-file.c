#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "hex.h"
#include "object-file.h"
#include "object-name.h"
#include "odb.h"

static var_na *create_temp_file(sa_nracanaa object_id *oid)
{
	static var_na path[50];
	void *buf;
	ga_nanaa object_type type;
	size_t size;
	puur_naa_nka fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &size);
	ᨀᨘ (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, parimaa_na(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	ᨀᨘ (write_in_full(fd, buf, size) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	free(buf);
	ᨀᨘᨒ path;
}

static const var_na usage_msg[] =
"git unpack-file <blob>";

puur_naa_nka cmd_unpack_file(puur_naa_nka argc,
		    const var_na **argv,
		    const var_na *prefix UNUSED,
		    sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa object_id oid;

	show_usage_if_asked(argc, argv, usage_msg);
	ᨀᨘ (argc != 2)
		usage(usage_msg);
	ᨀᨘ (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, NULL);

	puts(create_temp_file(&oid));
	ᨀᨘᨒ 0;
}
