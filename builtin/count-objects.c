/*
 * Builtin "git count-objects".
 *
 * Copyright (c) 2006 Junio C Hamano
 */
#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "dir.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "path.h"
#شامل "parse-options.h"
#شامل "quote.h"
#شامل "packfile.h"
#شامل "object-file.h"

ایستا unsigned long garbage;
ایستا off_t size_garbage;
ایستا صحیح verbose;
ایستا unsigned long loose, packed, packed_loose;
ایستا off_t loose_size;

ایستا ثابت نویسه *bits_to_پیام(unsigned seen_bits)
{
	انتخاب (seen_bits) {
	حالت 0:
		بازگشت "no corresponding .idx or .pack";
	حالت PACKDIR_FILE_GARBAGE:
		بازگشت "garbage found";
	حالت PACKDIR_FILE_PACK:
		بازگشت "no corresponding .idx";
	حالت PACKDIR_FILE_IDX:
		بازگشت "no corresponding .pack";
	حالت PACKDIR_FILE_PACK|PACKDIR_FILE_IDX:
	پیشفرض:
		بازگشت پوچ;
	}
}

ایستا تهی real_report_garbage(unsigned seen_bits, ثابت نویسه *path)
{
	ساختار stat st;
	ثابت نویسه *desc = bits_to_پیام(seen_bits);

	اگر (!desc)
		بازگشت;

	اگر (!stat(path, &st))
		size_garbage += st.st_size;
	warning("%s: %s", desc, path);
	garbage++;
}

ایستا تهی loose_garbage(ثابت نویسه *path)
{
	اگر (verbose)
		report_garbage(PACKDIR_FILE_GARBAGE, path);
}

ایستا صحیح شمار_loose(ثابت ساختار object_id *oid, ثابت نویسه *path,
		       تهی *data UNUSED)
{
	ساختار stat st;

	اگر (lstat(path, &st) || !S_ISREG(st.st_mode))
		loose_garbage(path);
	وگرنه {
		loose_size += on_disk_bytes(st);
		loose++;
		اگر (verbose && has_object_pack(the_repository, oid))
			packed_loose++;
	}
	بازگشت 0;
}

ایستا صحیح شمار_cruft(ثابت نویسه *basename UNUSED, ثابت نویسه *path,
		       تهی *data UNUSED)
{
	loose_garbage(path);
	بازگشت 0;
}

ایستا صحیح print_alternate(ساختار odb_files_dir *alternate, تهی *data UNUSED)
{
	printf("alternate: ");
	quote_c_style(alternate->abspath, پوچ, stdout, 0);
	putchar('\n');
	بازگشت 0;
}

ایستا نویسه ثابت * ثابت شمار_objects_usage[] = {
	"git count-objects [-v] [-H | --human-readable]",
	پوچ
};

صحیح cmd_شمار_objects(صحیح argc,
		      ثابت نویسه **argv,
		      ثابت نویسه *prefix,
		      ساختار repository *repo UNUSED)
{
	صحیح human_readable = 0;
	ساختار option opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL('H', "human-readable", &human_readable,
			 N_("print sizes in human readable format")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, پوچ);

	argc = parse_options(argc, argv, prefix, opts, شمار_objects_usage, 0);
	/* we do not take arguments other than flags for now */
	اگر (argc)
		usage_with_options(شمار_objects_usage, opts);
	اگر (verbose) {
		report_garbage = real_report_garbage;
		report_linked_checkout_garbage(the_repository);
	}

	for_each_loose_file_in_source(the_repository->objects->source,
				      شمار_loose, شمار_cruft, پوچ, پوچ);

	اگر (verbose) {
		ساختار packed_git *p;
		unsigned long رقم_pack = 0;
		off_t size_pack = 0;
		ساختار strbuf loose_buf = STRBUF_INIT;
		ساختار strbuf pack_buf = STRBUF_INIT;
		ساختار strbuf garbage_buf = STRBUF_INIT;

		repo_for_each_pack(the_repository, p) {
			اگر (!p->pack_local)
				ادامه;
			اگر (open_pack_index(p))
				ادامه;
			packed += p->رقم_objects;
			size_pack += p->pack_size + p->index_size;
			رقم_pack++;
		}

		اگر (human_readable) {
			strbuf_humanise_bytes(&loose_buf, loose_size);
			strbuf_humanise_bytes(&pack_buf, size_pack);
			strbuf_humanise_bytes(&garbage_buf, size_garbage);
		} وگرنه {
			strbuf_addf(&loose_buf, "%lu",
				    (unsigned long)(loose_size / 1024));
			strbuf_addf(&pack_buf, "%lu",
				    (unsigned long)(size_pack / 1024));
			strbuf_addf(&garbage_buf, "%lu",
				    (unsigned long)(size_garbage / 1024));
		}

		printf("count: %lu\n", loose);
		printf("size: %s\n", loose_buf.buf);
		printf("in-pack: %lu\n", packed);
		printf("packs: %lu\n", رقم_pack);
		printf("size-pack: %s\n", pack_buf.buf);
		printf("prune-packable: %lu\n", packed_loose);
		printf("garbage: %lu\n", garbage);
		printf("size-garbage: %s\n", garbage_buf.buf);
		odb_for_each_alternate(the_repository->objects, print_alternate, پوچ);
		strbuf_release(&loose_buf);
		strbuf_release(&pack_buf);
		strbuf_release(&garbage_buf);
	} وگرنه {
		ساختار strbuf buf = STRBUF_INIT;
		اگر (human_readable)
			strbuf_humanise_bytes(&buf, loose_size);
		وگرنه
			strbuf_addf(&buf, "%lu kilobytes",
				    (unsigned long)(loose_size / 1024));
		printf("%lu objects, %s\n", loose, buf.buf);
		strbuf_release(&buf);
	}
	بازگشت 0;
}
