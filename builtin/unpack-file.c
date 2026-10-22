#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"

స్థిర అక్షరం *create_తాత్కాలిక_file(నిర్మాణం object_id *oid)
{
	స్థిర అక్షరం మార్గం[50];
	శూన్య *buf;
	గణన object_type type;
	కొలత_t కొలత;
	పూర్ణాంకం fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &కొలత);
	ఒకవేళ (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_గమ్యం_hex(oid));

	xsnprintf(మార్గం, పరిమాణం(మార్గం), ".merge_file_XXXXXX");
	fd = xmkstemp(మార్గం);
	ఒకవేళ (write_లోపల_నిండిన(fd, buf, కొలత) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	విడుదల(buf);
	తిరిగి మార్గం;
}

స్థిర స్థిరరాశి అక్షరం usage_సందేశం[] =
"git unpack-file <blob>";

పూర్ణాంకం cmd_unpack_file(పూర్ణాంకం argc,
		    స్థిరరాశి అక్షరం **argv,
		    స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
		    నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం object_id oid;

	show_usage_if_asked(argc, argv, usage_సందేశం);
	ఒకవేళ (argc != 2)
		usage(usage_సందేశం);
	ఒకవేళ (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, ఖాళీ);

	puts(create_తాత్కాలిక_file(&oid));
	తిరిగి 0;
}
