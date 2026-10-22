#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"

ସ୍ଥିର ଅକ୍ଷର *create_temp_file(ସଂରଚନା object_id *oid)
{
	ସ୍ଥିର ଅକ୍ଷର path[50];
	ଶୂନ୍ୟ *buf;
	ଗଣନା object_type type;
	size_t size;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &size);
	ଯଦି (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, ଆକାର(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	ଯଦି (write_in_full(fd, buf, size) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	ମୁକ୍ତ(buf);
	ଫେରନ୍ତୁ path;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର usage_ସନ୍ଦେଶ[] =
"git unpack-file <blob>";

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_unpack_file(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		    ଅଚଳ ଅକ୍ଷର **argv,
		    ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
		    ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା object_id oid;

	show_usage_if_asked(argc, argv, usage_ସନ୍ଦେଶ);
	ଯଦି (argc != 2)
		usage(usage_ସନ୍ଦେଶ);
	ଯଦି (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, ଖାଲି);

	puts(create_temp_file(&oid));
	ଫେରନ୍ତୁ 0;
}
