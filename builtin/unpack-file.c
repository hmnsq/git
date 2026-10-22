#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"

ಸ್ಥಿರ ಅಕ್ಷರ *create_ತಾತ್ಕಾಲಿಕ_file(ರಚನೆ object_id *oid)
{
	ಸ್ಥಿರ ಅಕ್ಷರ ಮಾರ್ಗ[50];
	ಶೂನ್ಯ *buf;
	ಎಣಿಕೆ object_type type;
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
	ಪೂರ್ಣಾಂಕ fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &ಗಾತ್ರ_2);
	ಆದರೆ (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_ಗಮ್ಯ_hex(oid));

	xsnprintf(ಮಾರ್ಗ, ಗಾತ್ರ(ಮಾರ್ಗ), ".merge_file_XXXXXX");
	fd = xmkstemp(ಮಾರ್ಗ);
	ಆದರೆ (write_ಒಳ_ತುಂಬಿದ(fd, buf, ಗಾತ್ರ_2) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	ಬಿಡುಗಡೆ(buf);
	ಹಿಂತಿರುಗಿ ಮಾರ್ಗ;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ usage_ಸಂದೇಶ[] =
"git unpack-file <blob>";

ಪೂರ್ಣಾಂಕ cmd_unpack_file(ಪೂರ್ಣಾಂಕ argc,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
		    ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ object_id oid;

	show_usage_if_asked(argc, argv, usage_ಸಂದೇಶ);
	ಆದರೆ (argc != 2)
		usage(usage_ಸಂದೇಶ);
	ಆದರೆ (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	puts(create_ತಾತ್ಕಾಲಿಕ_file(&oid));
	ಹಿಂತಿರುಗಿ 0;
}
