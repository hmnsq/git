#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"

સ્થિર અક્ષર *create_temp_file(સંરચના object_id *oid)
{
	સ્થિર અક્ષર path[50];
	શૂન્ય *buf;
	ગણના object_type type;
	size_t size;
	પૂર્ણાંક fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &size);
	જો (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_to_hex(oid));

	xsnprintf(path, કદ(path), ".merge_file_XXXXXX");
	fd = xmkstemp(path);
	જો (write_in_full(fd, buf, size) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	મુક્ત(buf);
	પાછા path;
}

સ્થિર અચલ અક્ષર usage_સંદેશ[] =
"git unpack-file <blob>";

પૂર્ણાંક cmd_unpack_file(પૂર્ણાંક argc,
		    અચલ અક્ષર **argv,
		    અચલ અક્ષર *prefix UNUSED,
		    સંરચના repository *repo UNUSED)
{
	સંરચના object_id oid;

	show_usage_if_asked(argc, argv, usage_સંદેશ);
	જો (argc != 2)
		usage(usage_સંદેશ);
	જો (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, ખાલી);

	puts(create_temp_file(&oid));
	પાછા 0;
}
