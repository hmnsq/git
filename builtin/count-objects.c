/*
 * Builtin "git count-objects".
 *
 * Copyright (c) 2006 Junio C Hamano
 */
#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "dir.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "path.h"
#ضمن "parse-options.h"
#ضمن "quote.h"
#ضمن "packfile.h"
#ضمن "object-file.h"

ساكن بیعلامت دراز garbage;
ساكن off_t اندازه_garbage;
ساكن صحيح verbose;
ساكن بیعلامت دراز loose, packed, packed_loose;
ساكن off_t loose_اندازه;

ساكن ثابت حرف *بیتها_مقصد_پیام(بیعلامت دیده_بیتها)
{
	بدل (دیده_بیتها) {
	حالة 0:
		أرجع "no corresponding .idx or .pack";
	حالة PACKDIR_FILE_GARBAGE:
		أرجع "garbage found";
	حالة PACKDIR_FILE_PACK:
		أرجع "no corresponding .idx";
	حالة PACKDIR_FILE_IDX:
		أرجع "no corresponding .pack";
	حالة PACKDIR_FILE_PACK|PACKDIR_FILE_IDX:
	افتراضي:
		أرجع عدم;
	}
}

ساكن فراغ real_report_garbage(بیعلامت دیده_بیتها, ثابت حرف *مسیر)
{
	بنية stat st;
	ثابت حرف *desc = بیتها_مقصد_پیام(دیده_بیتها);

	إذا (!desc)
		أرجع;

	إذا (!stat(مسیر, &st))
		اندازه_garbage += st.st_اندازه;
	warning("%s: %s", desc, مسیر);
	garbage++;
}

ساكن فراغ loose_garbage(ثابت حرف *مسیر)
{
	إذا (verbose)
		report_garbage(PACKDIR_FILE_GARBAGE, مسیر);
}

ساكن صحيح شمار_loose(ثابت بنية object_id *oid, ثابت حرف *مسیر,
		       فراغ *داده استفادهنشده)
{
	بنية stat st;

	إذا (lstat(مسیر, &st) || !S_ISREG(st.st_نما))
		loose_garbage(مسیر);
	وإلا {
		loose_اندازه += on_disk_بایتها(st);
		loose++;
		إذا (verbose && has_object_pack(the_repository, oid))
			packed_loose++;
	}
	أرجع 0;
}

ساكن صحيح شمار_cruft(ثابت حرف *basename استفادهنشده, ثابت حرف *مسیر,
		       فراغ *داده استفادهنشده)
{
	loose_garbage(مسیر);
	أرجع 0;
}

ساكن صحيح print_alternate(بنية odb_files_dir *alternate, فراغ *داده استفادهنشده)
{
	چاپ_کن("alternate: ");
	quote_c_style(alternate->abspath, عدم, stdout, 0);
	حرف_چاپ_کن('\n');
	أرجع 0;
}

ساكن حرف ثابت * ثابت شمار_objects_usage[] = {
	"git count-objects [-v] [-H | --human-readable]",
	عدم
};

صحيح cmd_شمار_objects(صحيح argc,
		      ثابت حرف **argv,
		      ثابت حرف *پیشوند,
		      بنية repository *repo استفادهنشده)
{
	صحيح human_readable = 0;
	بنية option opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL('H', "human-readable", &human_readable,
			 N_("print sizes in human readable format")),
		OPT_انتها(),
	};

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, پیشوند, opts, شمار_objects_usage, 0);
	/* we do not take arguments other than flags for now */
	إذا (argc)
		usage_with_options(شمار_objects_usage, opts);
	إذا (verbose) {
		report_garbage = real_report_garbage;
		report_linked_checkout_garbage(the_repository);
	}

	for_each_loose_file_درون_منبع(the_repository->objects->منبع,
				      شمار_loose, شمار_cruft, عدم, عدم);

	إذا (verbose) {
		بنية packed_git *p;
		بیعلامت دراز رقم_pack = 0;
		off_t اندازه_pack = 0;
		بنية strbuf loose_buf = STRBUF_INIT;
		بنية strbuf pack_buf = STRBUF_INIT;
		بنية strbuf garbage_buf = STRBUF_INIT;

		repo_for_each_pack(the_repository, p) {
			إذا (!p->pack_local)
				استمر;
			إذا (open_pack_شاخص(p))
				استمر;
			packed += p->رقم_objects;
			اندازه_pack += p->pack_اندازه + p->شاخص_اندازه;
			رقم_pack++;
		}

		إذا (human_readable) {
			strbuf_humanise_بایتها(&loose_buf, loose_اندازه);
			strbuf_humanise_بایتها(&pack_buf, اندازه_pack);
			strbuf_humanise_بایتها(&garbage_buf, اندازه_garbage);
		} وإلا {
			strbuf_addf(&loose_buf, "%lu",
				    (بیعلامت دراز)(loose_اندازه / 1024));
			strbuf_addf(&pack_buf, "%lu",
				    (بیعلامت دراز)(اندازه_pack / 1024));
			strbuf_addf(&garbage_buf, "%lu",
				    (بیعلامت دراز)(اندازه_garbage / 1024));
		}

		چاپ_کن("count: %lu\n", loose);
		چاپ_کن("size: %s\n", loose_buf.buf);
		چاپ_کن("in-pack: %lu\n", packed);
		چاپ_کن("packs: %lu\n", رقم_pack);
		چاپ_کن("size-pack: %s\n", pack_buf.buf);
		چاپ_کن("prune-packable: %lu\n", packed_loose);
		چاپ_کن("garbage: %lu\n", garbage);
		چاپ_کن("size-garbage: %s\n", garbage_buf.buf);
		odb_for_each_alternate(the_repository->objects, print_alternate, عدم);
		strbuf_release(&loose_buf);
		strbuf_release(&pack_buf);
		strbuf_release(&garbage_buf);
	} وإلا {
		بنية strbuf buf = STRBUF_INIT;
		إذا (human_readable)
			strbuf_humanise_بایتها(&buf, loose_اندازه);
		وإلا
			strbuf_addf(&buf, "%lu kilobytes",
				    (بیعلامت دراز)(loose_اندازه / 1024));
		چاپ_کن("%lu objects, %s\n", loose, buf.buf);
		strbuf_release(&buf);
	}
	أرجع 0;
}
