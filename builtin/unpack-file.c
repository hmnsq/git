#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"

ਸਥਿਰ ਅੱਖਰ *create_ਆਰਜ਼ੀ_file(ਬਣਤਰ object_id *oid)
{
	ਸਥਿਰ ਅੱਖਰ ਰਸਤਾ[50];
	ਖਾਲੀ *buf;
	ਗਿਣਤੀ object_type type;
	ਮਾਪ_t ਮਾਪ;
	ਪੂਰਨ fd;

	buf = odb_read_object(the_repository->objects, oid, &type, &ਮਾਪ);
	ਜੇ (!buf || type != OBJ_BLOB)
		die("unable to read blob object %s", oid_ਮੰਜ਼ਿਲ_hex(oid));

	xsnprintf(ਰਸਤਾ, ਆਕਾਰ(ਰਸਤਾ), ".merge_file_XXXXXX");
	fd = xmkstemp(ਰਸਤਾ);
	ਜੇ (write_ਵਿੱਚ_ਭਰਿਆ(fd, buf, ਮਾਪ) < 0)
		die_errno("unable to write temp-file");
	close(fd);
	ਮੁਕਤ(buf);
	ਵਾਪਸ ਰਸਤਾ;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ usage_ਸੁਨੇਹਾ[] =
"git unpack-file <blob>";

ਪੂਰਨ cmd_unpack_file(ਪੂਰਨ argc,
		    ਅਚਲ ਅੱਖਰ **argv,
		    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
		    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ object_id oid;

	show_usage_if_asked(argc, argv, usage_ਸੁਨੇਹਾ);
	ਜੇ (argc != 2)
		usage(usage_ਸੁਨੇਹਾ);
	ਜੇ (repo_get_oid(the_repository, argv[1], &oid))
		die("Not a valid object name %s", argv[1]);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	puts(create_ਆਰਜ਼ੀ_file(&oid));
	ਵਾਪਸ 0;
}
