/*
 * Builtin "git count-objects".
 *
 * Copyright (c) 2006 Junio C Hamano
 */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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

ساکن غیرعلامتی طویل garbage;
ساکن off_t سائز_garbage;
ساکن صحیح verbose;
ساکن غیرعلامتی طویل loose, packed, packed_loose;
ساکن off_t loose_سائز;

ساکن ثابت حرف *بٹ_مقصد_پیغام(غیرعلامتی نظر_آیا_بٹ)
{
	انتخاب (نظر_آیا_بٹ) {
	حالت 0:
		واپس "no corresponding .idx or .pack";
	حالت PACKDIR_FILE_GARBAGE:
		واپس "garbage found";
	حالت PACKDIR_FILE_PACK:
		واپس "no corresponding .idx";
	حالت PACKDIR_FILE_IDX:
		واپس "no corresponding .pack";
	حالت PACKDIR_FILE_PACK|PACKDIR_FILE_IDX:
	طےشدہ:
		واپس تہی;
	}
}

ساکن خالی real_report_garbage(غیرعلامتی نظر_آیا_بٹ, ثابت حرف *راستہ)
{
	ساخت stat st;
	ثابت حرف *desc = بٹ_مقصد_پیغام(نظر_آیا_بٹ);

	اگر (!desc)
		واپس;

	اگر (!stat(راستہ, &st))
		سائز_garbage += st.st_سائز;
	warning("%s: %s", desc, راستہ);
	garbage++;
}

ساکن خالی loose_garbage(ثابت حرف *راستہ)
{
	اگر (verbose)
		report_garbage(PACKDIR_FILE_GARBAGE, راستہ);
}

ساکن صحیح گنتی_loose(ثابت ساخت object_id *oid, ثابت حرف *راستہ,
		       خالی *ڈیٹا غیر_مستعمل)
{
	ساخت stat st;

	اگر (lstat(راستہ, &st) || !S_ISREG(st.st_منوال))
		loose_garbage(راستہ);
	ورنہ {
		loose_سائز += on_disk_بائٹیں(st);
		loose++;
		اگر (verbose && has_object_pack(the_repository, oid))
			packed_loose++;
	}
	واپس 0;
}

ساکن صحیح گنتی_cruft(ثابت حرف *basename غیر_مستعمل, ثابت حرف *راستہ,
		       خالی *ڈیٹا غیر_مستعمل)
{
	loose_garbage(راستہ);
	واپس 0;
}

ساکن صحیح print_alternate(ساخت odb_files_dir *alternate, خالی *ڈیٹا غیر_مستعمل)
{
	لکھو("alternate: ");
	quote_c_style(alternate->abspath, تہی, stdout, 0);
	حرف_لکھو('\n');
	واپس 0;
}

ساکن حرف ثابت * ثابت گنتی_objects_usage[] = {
	"git count-objects [-v] [-H | --human-readable]",
	تہی
};

صحیح cmd_گنتی_objects(صحیح argc,
		      ثابت حرف **argv,
		      ثابت حرف *سابقہ,
		      ساخت repository *repo غیر_مستعمل)
{
	صحیح human_readable = 0;
	ساخت option opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL('H', "human-readable", &human_readable,
			 N_("print sizes in human readable format")),
		OPT_انتہا(),
	};

	repo_config(the_repository, git_default_config, تہی);

	argc = parse_options(argc, argv, سابقہ, opts, گنتی_objects_usage, 0);
	/* we do not take arguments other than flags for now */
	اگر (argc)
		usage_with_options(گنتی_objects_usage, opts);
	اگر (verbose) {
		report_garbage = real_report_garbage;
		report_linked_checkout_garbage(the_repository);
	}

	for_each_loose_file_اندر_ماخذ(the_repository->objects->ماخذ,
				      گنتی_loose, گنتی_cruft, تہی, تہی);

	اگر (verbose) {
		ساخت packed_git *p;
		غیرعلامتی طویل عدد_pack = 0;
		off_t سائز_pack = 0;
		ساخت strbuf loose_buf = STRBUF_INIT;
		ساخت strbuf pack_buf = STRBUF_INIT;
		ساخت strbuf garbage_buf = STRBUF_INIT;

		repo_for_each_pack(the_repository, p) {
			اگر (!p->pack_local)
				جاری;
			اگر (open_pack_اشاریہ(p))
				جاری;
			packed += p->عدد_objects;
			سائز_pack += p->pack_سائز + p->اشاریہ_سائز;
			عدد_pack++;
		}

		اگر (human_readable) {
			strbuf_humanise_بائٹیں(&loose_buf, loose_سائز);
			strbuf_humanise_بائٹیں(&pack_buf, سائز_pack);
			strbuf_humanise_بائٹیں(&garbage_buf, سائز_garbage);
		} ورنہ {
			strbuf_addf(&loose_buf, "%lu",
				    (غیرعلامتی طویل)(loose_سائز / 1024));
			strbuf_addf(&pack_buf, "%lu",
				    (غیرعلامتی طویل)(سائز_pack / 1024));
			strbuf_addf(&garbage_buf, "%lu",
				    (غیرعلامتی طویل)(سائز_garbage / 1024));
		}

		لکھو("count: %lu\n", loose);
		لکھو("size: %s\n", loose_buf.buf);
		لکھو("in-pack: %lu\n", packed);
		لکھو("packs: %lu\n", عدد_pack);
		لکھو("size-pack: %s\n", pack_buf.buf);
		لکھو("prune-packable: %lu\n", packed_loose);
		لکھو("garbage: %lu\n", garbage);
		لکھو("size-garbage: %s\n", garbage_buf.buf);
		odb_for_each_alternate(the_repository->objects, print_alternate, تہی);
		strbuf_release(&loose_buf);
		strbuf_release(&pack_buf);
		strbuf_release(&garbage_buf);
	} ورنہ {
		ساخت strbuf buf = STRBUF_INIT;
		اگر (human_readable)
			strbuf_humanise_بائٹیں(&buf, loose_سائز);
		ورنہ
			strbuf_addf(&buf, "%lu kilobytes",
				    (غیرعلامتی طویل)(loose_سائز / 1024));
		لکھو("%lu objects, %s\n", loose, buf.buf);
		strbuf_release(&buf);
	}
	واپس 0;
}
