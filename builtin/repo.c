#تعریف_کن USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "commit.h"
#ضمن "environment.h"
#ضمن "hash.h"
#ضمن "hex.h"
#ضمن "odb.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "path-walk.h"
#ضمن "progress.h"
#ضمن "quote.h"
#ضمن "ref-filter.h"
#ضمن "refs.h"
#ضمن "revision.h"
#ضمن "setup.h"
#ضمن "strbuf.h"
#ضمن "string-list.h"
#ضمن "shallow.h"
#ضمن "tree.h"
#ضمن "tree-walk.h"
#ضمن "utf8.h"

#تعریف_کن REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#تعریف_کن REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

ساكن ثابت حرف *ثابت repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	عدم,
};

ساكن ثابت حرف *ثابت repo_info_usage[] = {
	REPO_INFO_USAGE,
	عدم,
};

ساكن ثابت حرف *ثابت repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	عدم,
};

تعريفنوع صحيح get_مقدار_fn(بنية repository *repo, بنية strbuf *buf);

تعداد output_format {
	FORMAT_جدول,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

بنية repo_info_فیلد {
	ثابت حرف *کلید;
	get_مقدار_fn *get_مقدار;
};

ساكن صحيح get_layout_bare(بنية repository *repo استفادهنشده, بنية strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	أرجع 0;
}

ساكن صحيح get_layout_shallow(بنية repository *repo, بنية strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	أرجع 0;
}

ساكن صحيح get_object_format(بنية repository *repo, بنية strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	أرجع 0;
}

ساكن صحيح get_مسیر_commondir_absolute(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *common_dir = repo_get_common_dir(repo);

	إذا (!common_dir)
		أرجع error(_("unable to get common directory"));

	format_مسیر(buf, common_dir, repo->پیشوند, مسیر_FORMAT_CANONICAL);
	أرجع 0;
}

ساكن صحيح get_مسیر_commondir_relative(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *common_dir = repo_get_common_dir(repo);

	إذا (!common_dir)
		أرجع error(_("unable to get common directory"));

	format_مسیر(buf, common_dir, repo->پیشوند, مسیر_FORMAT_RELATIVE);
	أرجع 0;
}

ساكن صحيح get_مسیر_gitdir_absolute(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *git_dir = repo_get_git_dir(repo);

	إذا (!git_dir)
		أرجع error(_("unable to get git directory"));

	format_مسیر(buf, git_dir, repo->پیشوند, مسیر_FORMAT_CANONICAL);
	أرجع 0;
}

ساكن صحيح get_مسیر_gitdir_relative(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *git_dir = repo_get_git_dir(repo);

	إذا (!git_dir)
		أرجع error(_("unable to get git directory"));

	format_مسیر(buf, git_dir, repo->پیشوند, مسیر_FORMAT_RELATIVE);
	أرجع 0;
}

ساكن صحيح get_references_format(بنية repository *repo, بنية strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_مقصد_name(repo->ref_storage_format));
	أرجع 0;
}

/* repo_info_field keys must be in lexicographical order */
ساكن ثابت بنية repo_info_فیلد repo_info_فیلد[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_مسیر_commondir_absolute },
	{ "path.commondir.relative", get_مسیر_commondir_relative },
	{ "path.gitdir.absolute", get_مسیر_gitdir_absolute },
	{ "path.gitdir.relative", get_مسیر_gitdir_relative },
	{ "references.format", get_references_format },
};

ساكن صحيح repo_info_فیلد_cmp(ثابت فراغ *va, ثابت فراغ *vb)
{
	ثابت بنية repo_info_فیلد *a = va;
	ثابت بنية repo_info_فیلد *b = vb;

	أرجع strcmp(a->کلید, b->کلید);
}

ساكن ثابت بنية repo_info_فیلد *get_repo_info_فیلد(ثابت حرف *کلید)
{
	ثابت بنية repo_info_فیلد جستجو_کلید = { کلید, عدم };
	ثابت بنية repo_info_فیلد *یافت_2 = bsearch(&جستجو_کلید,
						      repo_info_فیلد,
						      ARRAY_اندازه(repo_info_فیلد),
						      حجم(*یافت_2),
						      repo_info_فیلد_cmp);

	أرجع یافت_2;
}

ساكن فراغ print_فیلد(تعداد output_format format, ثابت حرف *کلید,
			ثابت حرف *مقدار)
{
	بدل (format) {
	حالة FORMAT_NEWLINE_TERMINATED:
		چاپ_کن("%s=", کلید);
		quote_c_style(مقدار, عدم, stdout, 0);
		حرف_چاپ_کن('\n');
		اكسر;
	حالة FORMAT_NUL_TERMINATED:
		چاپ_کن("%s\n%s%c", کلید, مقدار, '\0');
		اكسر;
	افتراضي:
		BUG("not a valid output format: %d", format);
	}
}

ساكن صحيح print_fields(صحيح argc, ثابت حرف **argv,
			بنية repository *repo,
			تعداد output_format format)
{
	صحيح ret = 0;
	بنية strbuf valbuf = STRBUF_INIT;

	لكل (صحيح i = 0; i < argc; i++) {
		ثابت حرف *کلید = argv[i];
		ثابت بنية repo_info_فیلد *فیلد = get_repo_info_فیلد(کلید);

		إذا (!فیلد) {
			ret = error(_("key '%s' not found"), کلید);
			استمر;
		}

		strbuf_reset(&valbuf);
		فیلد->get_مقدار(repo, &valbuf);
		print_فیلد(format, کلید, valbuf.buf);
	}

	strbuf_release(&valbuf);
	أرجع ret;
}

ساكن صحيح print_all_fields(بنية repository *repo,
			    تعداد output_format format)
{
	بنية strbuf valbuf = STRBUF_INIT;

	لكل (اندازه_t i = 0; i < ARRAY_اندازه(repo_info_فیلد); i++) {
		ثابت بنية repo_info_فیلد *فیلد = &repo_info_فیلد[i];

		strbuf_reset(&valbuf);
		فیلد->get_مقدار(repo, &valbuf);
		print_فیلد(format, فیلد->کلید, valbuf.buf);
	}

	strbuf_release(&valbuf);
	أرجع 0;
}

ساكن صحيح print_کلیدها(تعداد output_format format)
{
	حرف sep;

	بدل (format) {
	حالة FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		اكسر;
	حالة FORMAT_NUL_TERMINATED:
		sep = '\0';
		اكسر;
	افتراضي:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	لكل (اندازه_t i = 0; i < ARRAY_اندازه(repo_info_فیلد); i++) {
		ثابت بنية repo_info_فیلد *فیلد = &repo_info_فیلد[i];
		چاپ_کن("%s%c", فیلد->کلید, sep);
	}

	أرجع 0;
}

ساكن صحيح parse_format_cb(ثابت بنية option *opt,
			   ثابت حرف *arg, صحيح unset استفادهنشده)
{
	تعداد output_format *format = opt->مقدار;

	إذا (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	وإلا إذا (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	وإلا إذا (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	وإلا إذا (!strcmp(arg, "table"))
		*format = FORMAT_جدول;
	وإلا
		die(_("invalid format '%s'"), arg);

	أرجع 0;
}

ساكن صحيح cmd_repo_info(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			 بنية repository *repo)
{
	تعداد output_format format = FORMAT_NEWLINE_TERMINATED;
	صحيح all_کلیدها = 0;
	صحيح show_کلیدها = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', عدم, &format, عدم,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_کلیدها, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_کلیدها, N_("show keys")),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options, repo_info_usage, 0);

	إذا (show_کلیدها && (all_کلیدها || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	إذا (show_کلیدها)
		أرجع print_کلیدها(format);

	إذا (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	إذا (all_کلیدها && argc)
		die(_("--all and <key> cannot be used together"));

	إذا (all_کلیدها)
		أرجع print_all_fields(repo, format);
	وإلا
		أرجع print_fields(argc, argv, repo, format);
}

بنية object_داده {
	بنية object_id oid;
	اندازه_t مقدار;
};

بنية بزرگترین_objects {
	بنية object_داده برچسب_اندازه;
	بنية object_داده commit_اندازه;
	بنية object_داده درخت_اندازه;
	بنية object_داده blob_اندازه;

	بنية object_داده والد_شمار;
	بنية object_داده درخت_entries;
};

بنية ref_stats {
	اندازه_t branches;
	اندازه_t remotes;
	اندازه_t برچسبها;
	اندازه_t others;
};

بنية object_مقادیر {
	اندازه_t برچسبها;
	اندازه_t commits;
	اندازه_t trees;
	اندازه_t blobs;
};

بنية object_stats {
	بنية object_مقادیر type_counts;
	بنية object_مقادیر inflated_sizes;
	بنية object_مقادیر disk_sizes;
	بنية بزرگترین_objects بزرگترین;
};

بنية repo_structure {
	بنية ref_stats refs;
	بنية object_stats objects;
};

بنية stats_جدول {
	بنية string_list ردیفها;
	بنية string_list annotations;

	صحيح name_ستون_عرض;
	صحيح مقدار_ستون_عرض;
	صحيح unit_ستون_عرض;
};

/*
 * Holds column data that gets stored for each row.
 */
بنية stats_جدول_مدخل {
	حرف *مقدار;
	ثابت حرف *unit;
	اندازه_t شاخص;
	بنية object_id *oid;
};

ساكن فراغ stats_جدول_vaddf(بنية stats_جدول *جدول,
			      بنية stats_جدول_مدخل *مدخل,
			      ثابت حرف *format, va_list ap)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_قلم *قلم;
	حرف *formatted_name;
	صحيح name_عرض;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, عدم);
	name_عرض = utf8_strwidth(formatted_name);

	قلم = string_list_append_nodup(&جدول->ردیفها, formatted_name);
	قلم->util = مدخل;

	إذا (name_عرض > جدول->name_ستون_عرض)
		جدول->name_ستون_عرض = name_عرض;
	إذا (!مدخل)
		أرجع;
	إذا (مدخل->oid) {
		مدخل->شاخص = جدول->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)مدخل->شاخص,
			    oid_مقصد_hex(مدخل->oid));
		string_list_append_nodup(&جدول->annotations, strbuf_detach(&buf, عدم));
	}
	إذا (مدخل->مقدار) {
		صحيح مقدار_عرض = utf8_strwidth(مدخل->مقدار);
		إذا (مقدار_عرض > جدول->مقدار_ستون_عرض)
			جدول->مقدار_ستون_عرض = مقدار_عرض;
	}
	إذا (مدخل->unit) {
		صحيح unit_عرض = utf8_strwidth(مدخل->unit);
		إذا (unit_عرض > جدول->unit_ستون_عرض)
			جدول->unit_ستون_عرض = unit_عرض;
	}

	strbuf_release(&buf);
}

ساكن فراغ stats_جدول_addf(بنية stats_جدول *جدول, ثابت حرف *format, ...)
{
	va_list ap;

	va_شروع(ap, format);
	stats_جدول_vaddf(جدول, عدم, format, ap);
	va_انتها(ap);
}

ساكن فراغ stats_جدول_شمار_addf(بنية stats_جدول *جدول, اندازه_t مقدار,
				   ثابت حرف *format, ...)
{
	بنية stats_جدول_مدخل *مدخل;
	va_list ap;

	CALLOC_ARRAY(مدخل, 1);
	humanise_شمار(مقدار, &مدخل->مقدار, &مدخل->unit);

	va_شروع(ap, format);
	stats_جدول_vaddf(جدول, مدخل, format, ap);
	va_انتها(ap);
}

ساكن فراغ stats_جدول_object_شمار_addf(بنية stats_جدول *جدول,
					  بنية object_id *oid, اندازه_t مقدار,
					  ثابت حرف *format, ...)
{
	بنية stats_جدول_مدخل *مدخل;
	va_list ap;

	CALLOC_ARRAY(مدخل, 1);
	humanise_شمار(مقدار, &مدخل->مقدار, &مدخل->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	إذا (!is_null_oid(oid))
		مدخل->oid = oid;

	va_شروع(ap, format);
	stats_جدول_vaddf(جدول, مدخل, format, ap);
	va_انتها(ap);
}

ساكن فراغ stats_جدول_اندازه_addf(بنية stats_جدول *جدول, اندازه_t مقدار,
				  ثابت حرف *format, ...)
{
	بنية stats_جدول_مدخل *مدخل;
	va_list ap;

	CALLOC_ARRAY(مدخل, 1);
	humanise_بایتها(مقدار, &مدخل->مقدار, &مدخل->unit, HUMANISE_COMPACT);

	va_شروع(ap, format);
	stats_جدول_vaddf(جدول, مدخل, format, ap);
	va_انتها(ap);
}

ساكن فراغ stats_جدول_object_اندازه_addf(بنية stats_جدول *جدول,
					 بنية object_id *oid, اندازه_t مقدار,
					 ثابت حرف *format, ...)
{
	بنية stats_جدول_مدخل *مدخل;
	va_list ap;

	CALLOC_ARRAY(مدخل, 1);
	humanise_بایتها(مقدار, &مدخل->مقدار, &مدخل->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	إذا (!is_null_oid(oid))
		مدخل->oid = oid;

	va_شروع(ap, format);
	stats_جدول_vaddf(جدول, مدخل, format, ap);
	va_انتها(ap);
}

ساكن مضمن اندازه_t get_مجموع_reference_شمار(بنية ref_stats *stats)
{
	أرجع stats->branches + stats->remotes + stats->برچسبها + stats->others;
}

ساكن مضمن اندازه_t get_مجموع_object_مقادیر(بنية object_مقادیر *مقادیر)
{
	أرجع مقادیر->برچسبها + مقادیر->commits + مقادیر->trees + مقادیر->blobs;
}

ساكن فراغ stats_جدول_راهاندازی_structure(بنية stats_جدول *جدول,
					بنية repo_structure *stats)
{
	بنية object_stats *objects = &stats->objects;
	بنية ref_stats *refs = &stats->refs;
	اندازه_t inflated_object_مجموع;
	اندازه_t object_شمار_مجموع;
	اندازه_t disk_object_مجموع;
	اندازه_t ref_مجموع;

	ref_مجموع = get_مجموع_reference_شمار(refs);
	stats_جدول_addf(جدول, "* %s", _("References"));
	stats_جدول_شمار_addf(جدول, ref_مجموع, "  * %s", _("Count"));
	stats_جدول_شمار_addf(جدول, refs->branches, "    * %s", _("Branches"));
	stats_جدول_شمار_addf(جدول, refs->برچسبها, "    * %s", _("Tags"));
	stats_جدول_شمار_addf(جدول, refs->remotes, "    * %s", _("Remotes"));
	stats_جدول_شمار_addf(جدول, refs->others, "    * %s", _("Others"));

	object_شمار_مجموع = get_مجموع_object_مقادیر(&objects->type_counts);
	stats_جدول_addf(جدول, "");
	stats_جدول_addf(جدول, "* %s", _("Reachable objects"));
	stats_جدول_شمار_addf(جدول, object_شمار_مجموع, "  * %s", _("Count"));
	stats_جدول_شمار_addf(جدول, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_جدول_شمار_addf(جدول, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_جدول_شمار_addf(جدول, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_جدول_شمار_addf(جدول, objects->type_counts.برچسبها,
			       "    * %s", _("Tags"));

	inflated_object_مجموع = get_مجموع_object_مقادیر(&objects->inflated_sizes);
	stats_جدول_اندازه_addf(جدول, inflated_object_مجموع,
			      "  * %s", _("Inflated size"));
	stats_جدول_اندازه_addf(جدول, objects->inflated_sizes.commits,
			      "    * %s", _("Commits"));
	stats_جدول_اندازه_addf(جدول, objects->inflated_sizes.trees,
			      "    * %s", _("Trees"));
	stats_جدول_اندازه_addf(جدول, objects->inflated_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_جدول_اندازه_addf(جدول, objects->inflated_sizes.برچسبها,
			      "    * %s", _("Tags"));

	disk_object_مجموع = get_مجموع_object_مقادیر(&objects->disk_sizes);
	stats_جدول_اندازه_addf(جدول, disk_object_مجموع,
			      "  * %s", _("Disk size"));
	stats_جدول_اندازه_addf(جدول, objects->disk_sizes.commits,
			      "    * %s", _("Commits"));
	stats_جدول_اندازه_addf(جدول, objects->disk_sizes.trees,
			      "    * %s", _("Trees"));
	stats_جدول_اندازه_addf(جدول, objects->disk_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_جدول_اندازه_addf(جدول, objects->disk_sizes.برچسبها,
			      "    * %s", _("Tags"));

	stats_جدول_addf(جدول, "");
	stats_جدول_addf(جدول, "* %s", _("Largest objects"));
	stats_جدول_addf(جدول, "  * %s", _("Commits"));
	stats_جدول_object_اندازه_addf(جدول,
				     &objects->بزرگترین.commit_اندازه.oid,
				     objects->بزرگترین.commit_اندازه.مقدار,
				     "    * %s", _("Maximum size"));
	stats_جدول_object_شمار_addf(جدول,
				      &objects->بزرگترین.والد_شمار.oid,
				      objects->بزرگترین.والد_شمار.مقدار,
				      "    * %s", _("Maximum parents"));
	stats_جدول_addf(جدول, "  * %s", _("Trees"));
	stats_جدول_object_اندازه_addf(جدول,
				     &objects->بزرگترین.درخت_اندازه.oid,
				     objects->بزرگترین.درخت_اندازه.مقدار,
				     "    * %s", _("Maximum size"));
	stats_جدول_object_شمار_addf(جدول,
				      &objects->بزرگترین.درخت_entries.oid,
				      objects->بزرگترین.درخت_entries.مقدار,
				      "    * %s", _("Maximum entries"));
	stats_جدول_addf(جدول, "  * %s", _("Blobs"));
	stats_جدول_object_اندازه_addf(جدول,
				     &objects->بزرگترین.blob_اندازه.oid,
				     objects->بزرگترین.blob_اندازه.مقدار,
				     "    * %s", _("Maximum size"));
	stats_جدول_addf(جدول, "  * %s", _("Tags"));
	stats_جدول_object_اندازه_addf(جدول,
				     &objects->بزرگترین.برچسب_اندازه.oid,
				     objects->بزرگترین.برچسب_اندازه.مقدار,
				     "    * %s", _("Maximum size"));
}

#تعریف_کن شاخص_عرض 4

ساكن فراغ stats_جدول_print_structure(ثابت بنية stats_جدول *جدول)
{
	ثابت حرف *name_ستون_title = _("Repository structure");
	ثابت حرف *مقدار_ستون_title = _("Value");
	صحيح title_name_عرض = utf8_strwidth(name_ستون_title);
	صحيح title_مقدار_عرض = utf8_strwidth(مقدار_ستون_title);
	صحيح name_ستون_عرض = جدول->name_ستون_عرض;
	صحيح مقدار_ستون_عرض = جدول->مقدار_ستون_عرض;
	صحيح unit_ستون_عرض = جدول->unit_ستون_عرض;
	بنية string_list_قلم *قلم;
	بنية strbuf buf = STRBUF_INIT;

	إذا (title_name_عرض > name_ستون_عرض)
		name_ستون_عرض = title_name_عرض;
	إذا (title_مقدار_عرض > مقدار_ستون_عرض + unit_ستون_عرض + 1)
		مقدار_ستون_عرض = title_مقدار_عرض - unit_ستون_عرض;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_چپ, name_ستون_عرض + شاخص_عرض,
			  name_ستون_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_چپ,
			  مقدار_ستون_عرض + unit_ستون_عرض + 1, مقدار_ستون_title);
	strbuf_addstr(&buf, " |");
	چاپ_کن("%s\n", buf.buf);

	چاپ_کن("| ");
	لكل (صحيح i = 0; i < name_ستون_عرض + شاخص_عرض; i++)
		حرف_چاپ_کن('-');
	چاپ_کن(" | ");
	لكل (صحيح i = 0; i < مقدار_ستون_عرض + unit_ستون_عرض + 1; i++)
		حرف_چاپ_کن('-');
	چاپ_کن(" |\n");

	for_each_string_list_قلم(قلم, &جدول->ردیفها) {
		بنية stats_جدول_مدخل *مدخل = قلم->util;
		ثابت حرف *مقدار = "";
		ثابت حرف *unit = "";

		إذا (مدخل) {
			مقدار = مدخل->مقدار;
			إذا (مدخل->unit)
				unit = مدخل->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_چپ, name_ستون_عرض, قلم->string);

		إذا (مدخل && مدخل->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)مدخل->شاخص);
		وإلا
			strbuf_addchars(&buf, ' ', شاخص_عرض);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_راست, مقدار_ستون_عرض, مقدار);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_چپ, unit_ستون_عرض, unit);
		strbuf_addstr(&buf, " |");
		چاپ_کن("%s\n", buf.buf);
	}

	إذا (جدول->annotations.nr) {
		چاپ_کن("\n");
		for_each_string_list_قلم(قلم, &جدول->annotations)
			چاپ_کن("%s\n", قلم->string);
	}

	strbuf_release(&buf);
}

ساكن فراغ stats_جدول_clear(بنية stats_جدول *جدول)
{
	بنية stats_جدول_مدخل *مدخل;
	بنية string_list_قلم *قلم;

	for_each_string_list_قلم(قلم, &جدول->ردیفها) {
		مدخل = قلم->util;
		إذا (مدخل)
			حرر(مدخل->مقدار);
	}

	string_list_clear(&جدول->ردیفها, 1);
	string_list_clear(&جدول->annotations, 1);
}

ساكن مضمن فراغ print_keyvalue(ثابت حرف *کلید, حرف کلید_delim, اندازه_t مقدار,
				  حرف مقدار_delim)
{
	چاپ_کن("%s%c%" PRIuMAX "%c", کلید, کلید_delim, (uintmax_t)مقدار,
	       مقدار_delim);
}

ساكن فراغ print_object_داده(ثابت حرف *کلید, حرف کلید_delim,
			      بنية object_داده *داده, حرف مقدار_delim)
{
	print_keyvalue(کلید, کلید_delim, داده->مقدار, مقدار_delim);
	چاپ_کن("%s_oid%c%s%c", کلید, کلید_delim, oid_مقصد_hex(&داده->oid),
	       مقدار_delim);
}

ساكن فراغ structure_keyvalue_print(بنية repo_structure *stats,
				     حرف کلید_delim, حرف مقدار_delim)
{
	print_keyvalue("references.branches.count", کلید_delim,
		       stats->refs.branches, مقدار_delim);
	print_keyvalue("references.tags.count", کلید_delim,
		       stats->refs.برچسبها, مقدار_delim);
	print_keyvalue("references.remotes.count", کلید_delim,
		       stats->refs.remotes, مقدار_delim);
	print_keyvalue("references.others.count", کلید_delim,
		       stats->refs.others, مقدار_delim);

	print_keyvalue("objects.commits.count", کلید_delim,
		       stats->objects.type_counts.commits, مقدار_delim);
	print_keyvalue("objects.trees.count", کلید_delim,
		       stats->objects.type_counts.trees, مقدار_delim);
	print_keyvalue("objects.blobs.count", کلید_delim,
		       stats->objects.type_counts.blobs, مقدار_delim);
	print_keyvalue("objects.tags.count", کلید_delim,
		       stats->objects.type_counts.برچسبها, مقدار_delim);

	print_keyvalue("objects.commits.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.commits, مقدار_delim);
	print_keyvalue("objects.trees.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.trees, مقدار_delim);
	print_keyvalue("objects.blobs.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.blobs, مقدار_delim);
	print_keyvalue("objects.tags.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.برچسبها, مقدار_delim);

	print_keyvalue("objects.commits.disk_size", کلید_delim,
		       stats->objects.disk_sizes.commits, مقدار_delim);
	print_keyvalue("objects.trees.disk_size", کلید_delim,
		       stats->objects.disk_sizes.trees, مقدار_delim);
	print_keyvalue("objects.blobs.disk_size", کلید_delim,
		       stats->objects.disk_sizes.blobs, مقدار_delim);
	print_keyvalue("objects.tags.disk_size", کلید_delim,
		       stats->objects.disk_sizes.برچسبها, مقدار_delim);

	print_object_داده("objects.commits.max_size", کلید_delim,
			  &stats->objects.بزرگترین.commit_اندازه, مقدار_delim);
	print_object_داده("objects.trees.max_size", کلید_delim,
			  &stats->objects.بزرگترین.درخت_اندازه, مقدار_delim);
	print_object_داده("objects.blobs.max_size", کلید_delim,
			  &stats->objects.بزرگترین.blob_اندازه, مقدار_delim);
	print_object_داده("objects.tags.max_size", کلید_delim,
			  &stats->objects.بزرگترین.برچسب_اندازه, مقدار_delim);

	print_object_داده("objects.commits.max_parents", کلید_delim,
			  &stats->objects.بزرگترین.والد_شمار, مقدار_delim);
	print_object_داده("objects.trees.max_entries", کلید_delim,
			  &stats->objects.بزرگترین.درخت_entries, مقدار_delim);

	fflush(stdout);
}

بنية شمار_references_داده {
	بنية ref_stats *stats;
	بنية rev_info *revs;
	بنية پیشرفت *پیشرفت;
};

ساكن صحيح شمار_references(ثابت بنية reference *ref, فراغ *cb_داده)
{
	بنية شمار_references_داده *داده = cb_داده;
	بنية ref_stats *stats = داده->stats;
	اندازه_t ref_شمار;

	بدل (ref_نوع_from_refname(ref->name)) {
	حالة FILTER_REFS_BRANCHES:
		stats->branches++;
		اكسر;
	حالة FILTER_REFS_REMOTES:
		stats->remotes++;
		اكسر;
	حالة FILTER_REFS_برچسبها:
		stats->برچسبها++;
		اكسر;
	حالة FILTER_REFS_OTHERS:
		stats->others++;
		اكسر;
	افتراضي:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	جمع_کن_pending_oid(داده->revs, عدم, ref->oid, 0);

	ref_شمار = get_مجموع_reference_شمار(stats);
	display_پیشرفت(داده->پیشرفت, ref_شمار);

	أرجع 0;
}

ساكن فراغ structure_شمار_references(بنية ref_stats *stats,
				       بنية rev_info *revs,
				       بنية repository *repo,
				       صحيح show_پیشرفت)
{
	بنية شمار_references_داده داده = {
		.stats = stats,
		.revs = revs,
	};

	إذا (show_پیشرفت)
		داده.پیشرفت = شروع_delayed_پیشرفت(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), شمار_references, &داده);
	stop_پیشرفت(&داده.پیشرفت);
}

بنية شمار_objects_داده {
	بنية object_database *odb;
	بنية object_stats *stats;
	بنية پیشرفت *پیشرفت;
};

ساكن فراغ check_بزرگترین(بنية object_داده *داده, بنية object_id *oid,
			  اندازه_t مقدار)
{
	إذا (مقدار > داده->مقدار || is_null_oid(&داده->oid)) {
		oidcpy(&داده->oid, oid);
		داده->مقدار = مقدار;
	}
}

ساكن اندازه_t شمار_درخت_entries(بنية object *obj)
{
	بنية درخت *t = object_as_type(obj, OBJ_درخت, 0);
	بنية name_مدخل مدخل;
	بنية درخت_desc desc;
	اندازه_t شمار = 0;

	init_درخت_desc(&desc, &t->object.oid, t->میانگیر, t->اندازه);
	طالما (درخت_مدخل(&desc, &مدخل))
		شمار++;

	أرجع شمار;
}

ساكن صحيح شمار_objects(ثابت حرف *مسیر استفادهنشده, بنية oid_array *oids,
			 تعداد object_type type, فراغ *cb_داده)
{
	بنية شمار_objects_داده *داده = cb_داده;
	بنية object_stats *stats = داده->stats;
	اندازه_t object_شمار;

	لكل (اندازه_t i = 0; i < oids->nr; i++) {
		بنية object_info oi = OBJECT_INFO_INIT;
		بیعلامت دراز inflated;
		اندازه_t inflated_st = 0;
		بنية commit *commit;
		بنية object *obj;
		فراغ *content;
		off_t disk;
		صحيح eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		إذا (odb_read_object_info_extended(داده->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_سریع) < 0)
			استمر;
		inflated = cast_اندازه_t_مقصد_ulong(inflated_st);

		obj = parse_object_میانگیر(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		بدل (type) {
		حالة OBJ_برچسب:
			stats->type_counts.برچسبها++;
			stats->inflated_sizes.برچسبها += inflated;
			stats->disk_sizes.برچسبها += disk;
			check_بزرگترین(&stats->بزرگترین.برچسب_اندازه, &oids->oid[i],
				      inflated);
			اكسر;
		حالة OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_بزرگترین(&stats->بزرگترین.commit_اندازه, &oids->oid[i],
				      inflated);
			check_بزرگترین(&stats->بزرگترین.والد_شمار, &oids->oid[i],
				      commit_list_شمار(commit->parents));
			اكسر;
		حالة OBJ_درخت:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_بزرگترین(&stats->بزرگترین.درخت_اندازه, &oids->oid[i],
				      inflated);
			check_بزرگترین(&stats->بزرگترین.درخت_entries, &oids->oid[i],
				      شمار_درخت_entries(obj));
			اكسر;
		حالة OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_بزرگترین(&stats->بزرگترین.blob_اندازه, &oids->oid[i],
				      inflated);
			اكسر;
		افتراضي:
			BUG("invalid object type");
		}

		إذا (!eaten)
			حرر(content);
	}

	object_شمار = get_مجموع_object_مقادیر(&stats->type_counts);
	display_پیشرفت(داده->پیشرفت, object_شمار);

	أرجع 0;
}

ساكن فراغ structure_شمار_objects(بنية object_stats *stats,
				    بنية rev_info *revs,
				    بنية repository *repo, صحيح show_پیشرفت)
{
	بنية مسیر_walk_info info = مسیر_WALK_INFO_INIT;
	بنية شمار_objects_داده داده = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.مسیر_fn = شمار_objects;
	info.مسیر_fn_داده = &داده;

	إذا (show_پیشرفت)
		داده.پیشرفت = شروع_delayed_پیشرفت(repo, _("Counting objects"), 0);

	walk_objects_by_مسیر(&info);
	مسیر_walk_info_clear(&info);
	stop_پیشرفت(&داده.پیشرفت);
}

ساكن صحيح cmd_repo_structure(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			      بنية repository *repo)
{
	بنية stats_جدول جدول = {
		.ردیفها = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	تعداد output_format format = FORMAT_جدول;
	بنية repo_structure stats = { 0 };
	بنية rev_info revs;
	صحيح show_پیشرفت = -1;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', عدم, &format, عدم,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_پیشرفت, N_("show progress")),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options, repo_structure_usage, 0);
	إذا (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, پیشوند);

	إذا (show_پیشرفت < 0)
		show_پیشرفت = isatty(2);

	structure_شمار_references(&stats.refs, &revs, repo, show_پیشرفت);
	structure_شمار_objects(&stats.objects, &revs, repo, show_پیشرفت);

	بدل (format) {
	حالة FORMAT_جدول:
		stats_جدول_راهاندازی_structure(&جدول, &stats);
		stats_جدول_print_structure(&جدول);
		اكسر;
	حالة FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		اكسر;
	حالة FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		اكسر;
	افتراضي:
		BUG("invalid output format");
	}

	stats_جدول_clear(&جدول);
	release_revisions(&revs);

	أرجع 0;
}

صحيح cmd_repo(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
	     بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options, repo_usage, 0);

	أرجع fn(argc, argv, پیشوند, repo);
}
