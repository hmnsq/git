#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

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

#تعریف_کړه REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#تعریف_کړه REPO_STRUCTURE_USAGE \
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

تعريفنوع صحيح get_ارزښت_fn(بنية repository *repo, بنية strbuf *buf);

تعداد output_format {
	FORMAT_جدول,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

بنية repo_info_ډګر {
	ثابت حرف *کلی;
	get_ارزښت_fn *get_ارزښت;
};

ساكن صحيح get_layout_bare(بنية repository *repo ناکارول_شوی, بنية strbuf *buf)
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

ساكن صحيح get_لار_commondir_absolute(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *common_dir = repo_get_common_dir(repo);

	إذا (!common_dir)
		أرجع error(_("unable to get common directory"));

	format_لار(buf, common_dir, repo->مختاړی, لار_FORMAT_CANONICAL);
	أرجع 0;
}

ساكن صحيح get_لار_commondir_relative(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *common_dir = repo_get_common_dir(repo);

	إذا (!common_dir)
		أرجع error(_("unable to get common directory"));

	format_لار(buf, common_dir, repo->مختاړی, لار_FORMAT_RELATIVE);
	أرجع 0;
}

ساكن صحيح get_لار_gitdir_absolute(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *git_dir = repo_get_git_dir(repo);

	إذا (!git_dir)
		أرجع error(_("unable to get git directory"));

	format_لار(buf, git_dir, repo->مختاړی, لار_FORMAT_CANONICAL);
	أرجع 0;
}

ساكن صحيح get_لار_gitdir_relative(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *git_dir = repo_get_git_dir(repo);

	إذا (!git_dir)
		أرجع error(_("unable to get git directory"));

	format_لار(buf, git_dir, repo->مختاړی, لار_FORMAT_RELATIVE);
	أرجع 0;
}

ساكن صحيح get_references_format(بنية repository *repo, بنية strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_منزل_name(repo->ref_storage_format));
	أرجع 0;
}

/* repo_info_field keys must be in lexicographical order */
ساكن ثابت بنية repo_info_ډګر repo_info_ډګر[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_لار_commondir_absolute },
	{ "path.commondir.relative", get_لار_commondir_relative },
	{ "path.gitdir.absolute", get_لار_gitdir_absolute },
	{ "path.gitdir.relative", get_لار_gitdir_relative },
	{ "references.format", get_references_format },
};

ساكن صحيح repo_info_ډګر_cmp(ثابت فراغ *va, ثابت فراغ *vb)
{
	ثابت بنية repo_info_ډګر *a = va;
	ثابت بنية repo_info_ډګر *b = vb;

	أرجع strcmp(a->کلی, b->کلی);
}

ساكن ثابت بنية repo_info_ډګر *get_repo_info_ډګر(ثابت حرف *کلی)
{
	ثابت بنية repo_info_ډګر پلټنه_کلی = { کلی, عدم };
	ثابت بنية repo_info_ډګر *وموندل_2 = bsearch(&پلټنه_کلی,
						      repo_info_ډګر,
						      ARRAY_اندازه(repo_info_ډګر),
						      حجم(*وموندل_2),
						      repo_info_ډګر_cmp);

	أرجع وموندل_2;
}

ساكن فراغ print_ډګر(تعداد output_format format, ثابت حرف *کلی,
			ثابت حرف *ارزښت)
{
	بدل (format) {
	حالة FORMAT_NEWLINE_TERMINATED:
		چاپ_کړه("%s=", کلی);
		quote_c_style(ارزښت, عدم, stdout, 0);
		توری_چاپ_کړه('\n');
		اكسر;
	حالة FORMAT_NUL_TERMINATED:
		چاپ_کړه("%s\n%s%c", کلی, ارزښت, '\0');
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
		ثابت حرف *کلی = argv[i];
		ثابت بنية repo_info_ډګر *ډګر = get_repo_info_ډګر(کلی);

		إذا (!ډګر) {
			ret = error(_("key '%s' not found"), کلی);
			استمر;
		}

		strbuf_reset(&valbuf);
		ډګر->get_ارزښت(repo, &valbuf);
		print_ډګر(format, کلی, valbuf.buf);
	}

	strbuf_release(&valbuf);
	أرجع ret;
}

ساكن صحيح print_all_fields(بنية repository *repo,
			    تعداد output_format format)
{
	بنية strbuf valbuf = STRBUF_INIT;

	لكل (اندازه_t i = 0; i < ARRAY_اندازه(repo_info_ډګر); i++) {
		ثابت بنية repo_info_ډګر *ډګر = &repo_info_ډګر[i];

		strbuf_reset(&valbuf);
		ډګر->get_ارزښت(repo, &valbuf);
		print_ډګر(format, ډګر->کلی, valbuf.buf);
	}

	strbuf_release(&valbuf);
	أرجع 0;
}

ساكن صحيح print_کلیګانې(تعداد output_format format)
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

	لكل (اندازه_t i = 0; i < ARRAY_اندازه(repo_info_ډګر); i++) {
		ثابت بنية repo_info_ډګر *ډګر = &repo_info_ډګر[i];
		چاپ_کړه("%s%c", ډګر->کلی, sep);
	}

	أرجع 0;
}

ساكن صحيح parse_format_cb(ثابت بنية option *opt,
			   ثابت حرف *arg, صحيح unset ناکارول_شوی)
{
	تعداد output_format *format = opt->ارزښت;

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

ساكن صحيح cmd_repo_info(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			 بنية repository *repo)
{
	تعداد output_format format = FORMAT_NEWLINE_TERMINATED;
	صحيح all_کلیګانې = 0;
	صحيح show_کلیګانې = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', عدم, &format, عدم,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_کلیګانې, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_کلیګانې, N_("show keys")),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, repo_info_usage, 0);

	إذا (show_کلیګانې && (all_کلیګانې || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	إذا (show_کلیګانې)
		أرجع print_کلیګانې(format);

	إذا (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	إذا (all_کلیګانې && argc)
		die(_("--all and <key> cannot be used together"));

	إذا (all_کلیګانې)
		أرجع print_all_fields(repo, format);
	وإلا
		أرجع print_fields(argc, argv, repo, format);
}

بنية object_معلومات {
	بنية object_id oid;
	اندازه_t ارزښت;
};

بنية تر_ټولو_لوی_objects {
	بنية object_معلومات نښان_اندازه;
	بنية object_معلومات commit_اندازه;
	بنية object_معلومات ونه_اندازه;
	بنية object_معلومات blob_اندازه;

	بنية object_معلومات پلار_شمېر;
	بنية object_معلومات ونه_entries;
};

بنية ref_stats {
	اندازه_t branches;
	اندازه_t remotes;
	اندازه_t نښانونه;
	اندازه_t others;
};

بنية object_ارزښتونه {
	اندازه_t نښانونه;
	اندازه_t commits;
	اندازه_t trees;
	اندازه_t blobs;
};

بنية object_stats {
	بنية object_ارزښتونه type_counts;
	بنية object_ارزښتونه inflated_sizes;
	بنية object_ارزښتونه disk_sizes;
	بنية تر_ټولو_لوی_objects تر_ټولو_لوی;
};

بنية repo_structure {
	بنية ref_stats refs;
	بنية object_stats objects;
};

بنية stats_جدول {
	بنية string_list لیکې;
	بنية string_list annotations;

	صحيح name_ستون_پلنوالی;
	صحيح ارزښت_ستون_پلنوالی;
	صحيح unit_ستون_پلنوالی;
};

/*
 * Holds column data that gets stored for each row.
 */
بنية stats_جدول_ثبت {
	حرف *ارزښت;
	ثابت حرف *unit;
	اندازه_t شاخص;
	بنية object_id *oid;
};

ساكن فراغ stats_جدول_vaddf(بنية stats_جدول *جدول,
			      بنية stats_جدول_ثبت *ثبت,
			      ثابت حرف *format, va_list ap)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_توکی *توکی;
	حرف *formatted_name;
	صحيح name_پلنوالی;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, عدم);
	name_پلنوالی = utf8_strwidth(formatted_name);

	توکی = string_list_append_nodup(&جدول->لیکې, formatted_name);
	توکی->util = ثبت;

	إذا (name_پلنوالی > جدول->name_ستون_پلنوالی)
		جدول->name_ستون_پلنوالی = name_پلنوالی;
	إذا (!ثبت)
		أرجع;
	إذا (ثبت->oid) {
		ثبت->شاخص = جدول->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)ثبت->شاخص,
			    oid_منزل_hex(ثبت->oid));
		string_list_append_nodup(&جدول->annotations, strbuf_detach(&buf, عدم));
	}
	إذا (ثبت->ارزښت) {
		صحيح ارزښت_پلنوالی = utf8_strwidth(ثبت->ارزښت);
		إذا (ارزښت_پلنوالی > جدول->ارزښت_ستون_پلنوالی)
			جدول->ارزښت_ستون_پلنوالی = ارزښت_پلنوالی;
	}
	إذا (ثبت->unit) {
		صحيح unit_پلنوالی = utf8_strwidth(ثبت->unit);
		إذا (unit_پلنوالی > جدول->unit_ستون_پلنوالی)
			جدول->unit_ستون_پلنوالی = unit_پلنوالی;
	}

	strbuf_release(&buf);
}

ساكن فراغ stats_جدول_addf(بنية stats_جدول *جدول, ثابت حرف *format, ...)
{
	va_list ap;

	va_پیل(ap, format);
	stats_جدول_vaddf(جدول, عدم, format, ap);
	va_پای_ټکی(ap);
}

ساكن فراغ stats_جدول_شمېر_addf(بنية stats_جدول *جدول, اندازه_t ارزښت,
				   ثابت حرف *format, ...)
{
	بنية stats_جدول_ثبت *ثبت;
	va_list ap;

	CALLOC_ARRAY(ثبت, 1);
	humanise_شمېر(ارزښت, &ثبت->ارزښت, &ثبت->unit);

	va_پیل(ap, format);
	stats_جدول_vaddf(جدول, ثبت, format, ap);
	va_پای_ټکی(ap);
}

ساكن فراغ stats_جدول_object_شمېر_addf(بنية stats_جدول *جدول,
					  بنية object_id *oid, اندازه_t ارزښت,
					  ثابت حرف *format, ...)
{
	بنية stats_جدول_ثبت *ثبت;
	va_list ap;

	CALLOC_ARRAY(ثبت, 1);
	humanise_شمېر(ارزښت, &ثبت->ارزښت, &ثبت->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	إذا (!is_null_oid(oid))
		ثبت->oid = oid;

	va_پیل(ap, format);
	stats_جدول_vaddf(جدول, ثبت, format, ap);
	va_پای_ټکی(ap);
}

ساكن فراغ stats_جدول_اندازه_addf(بنية stats_جدول *جدول, اندازه_t ارزښت,
				  ثابت حرف *format, ...)
{
	بنية stats_جدول_ثبت *ثبت;
	va_list ap;

	CALLOC_ARRAY(ثبت, 1);
	humanise_بایټونه(ارزښت, &ثبت->ارزښت, &ثبت->unit, HUMANISE_COMPACT);

	va_پیل(ap, format);
	stats_جدول_vaddf(جدول, ثبت, format, ap);
	va_پای_ټکی(ap);
}

ساكن فراغ stats_جدول_object_اندازه_addf(بنية stats_جدول *جدول,
					 بنية object_id *oid, اندازه_t ارزښت,
					 ثابت حرف *format, ...)
{
	بنية stats_جدول_ثبت *ثبت;
	va_list ap;

	CALLOC_ARRAY(ثبت, 1);
	humanise_بایټونه(ارزښت, &ثبت->ارزښت, &ثبت->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	إذا (!is_null_oid(oid))
		ثبت->oid = oid;

	va_پیل(ap, format);
	stats_جدول_vaddf(جدول, ثبت, format, ap);
	va_پای_ټکی(ap);
}

ساكن مضمن اندازه_t get_ټول_reference_شمېر(بنية ref_stats *stats)
{
	أرجع stats->branches + stats->remotes + stats->نښانونه + stats->others;
}

ساكن مضمن اندازه_t get_ټول_object_ارزښتونه(بنية object_ارزښتونه *ارزښتونه)
{
	أرجع ارزښتونه->نښانونه + ارزښتونه->commits + ارزښتونه->trees + ارزښتونه->blobs;
}

ساكن فراغ stats_جدول_تیاری_structure(بنية stats_جدول *جدول,
					بنية repo_structure *stats)
{
	بنية object_stats *objects = &stats->objects;
	بنية ref_stats *refs = &stats->refs;
	اندازه_t inflated_object_ټول;
	اندازه_t object_شمېر_ټول;
	اندازه_t disk_object_ټول;
	اندازه_t ref_ټول;

	ref_ټول = get_ټول_reference_شمېر(refs);
	stats_جدول_addf(جدول, "* %s", _("References"));
	stats_جدول_شمېر_addf(جدول, ref_ټول, "  * %s", _("Count"));
	stats_جدول_شمېر_addf(جدول, refs->branches, "    * %s", _("Branches"));
	stats_جدول_شمېر_addf(جدول, refs->نښانونه, "    * %s", _("Tags"));
	stats_جدول_شمېر_addf(جدول, refs->remotes, "    * %s", _("Remotes"));
	stats_جدول_شمېر_addf(جدول, refs->others, "    * %s", _("Others"));

	object_شمېر_ټول = get_ټول_object_ارزښتونه(&objects->type_counts);
	stats_جدول_addf(جدول, "");
	stats_جدول_addf(جدول, "* %s", _("Reachable objects"));
	stats_جدول_شمېر_addf(جدول, object_شمېر_ټول, "  * %s", _("Count"));
	stats_جدول_شمېر_addf(جدول, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_جدول_شمېر_addf(جدول, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_جدول_شمېر_addf(جدول, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_جدول_شمېر_addf(جدول, objects->type_counts.نښانونه,
			       "    * %s", _("Tags"));

	inflated_object_ټول = get_ټول_object_ارزښتونه(&objects->inflated_sizes);
	stats_جدول_اندازه_addf(جدول, inflated_object_ټول,
			      "  * %s", _("Inflated size"));
	stats_جدول_اندازه_addf(جدول, objects->inflated_sizes.commits,
			      "    * %s", _("Commits"));
	stats_جدول_اندازه_addf(جدول, objects->inflated_sizes.trees,
			      "    * %s", _("Trees"));
	stats_جدول_اندازه_addf(جدول, objects->inflated_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_جدول_اندازه_addf(جدول, objects->inflated_sizes.نښانونه,
			      "    * %s", _("Tags"));

	disk_object_ټول = get_ټول_object_ارزښتونه(&objects->disk_sizes);
	stats_جدول_اندازه_addf(جدول, disk_object_ټول,
			      "  * %s", _("Disk size"));
	stats_جدول_اندازه_addf(جدول, objects->disk_sizes.commits,
			      "    * %s", _("Commits"));
	stats_جدول_اندازه_addf(جدول, objects->disk_sizes.trees,
			      "    * %s", _("Trees"));
	stats_جدول_اندازه_addf(جدول, objects->disk_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_جدول_اندازه_addf(جدول, objects->disk_sizes.نښانونه,
			      "    * %s", _("Tags"));

	stats_جدول_addf(جدول, "");
	stats_جدول_addf(جدول, "* %s", _("Largest objects"));
	stats_جدول_addf(جدول, "  * %s", _("Commits"));
	stats_جدول_object_اندازه_addf(جدول,
				     &objects->تر_ټولو_لوی.commit_اندازه.oid,
				     objects->تر_ټولو_لوی.commit_اندازه.ارزښت,
				     "    * %s", _("Maximum size"));
	stats_جدول_object_شمېر_addf(جدول,
				      &objects->تر_ټولو_لوی.پلار_شمېر.oid,
				      objects->تر_ټولو_لوی.پلار_شمېر.ارزښت,
				      "    * %s", _("Maximum parents"));
	stats_جدول_addf(جدول, "  * %s", _("Trees"));
	stats_جدول_object_اندازه_addf(جدول,
				     &objects->تر_ټولو_لوی.ونه_اندازه.oid,
				     objects->تر_ټولو_لوی.ونه_اندازه.ارزښت,
				     "    * %s", _("Maximum size"));
	stats_جدول_object_شمېر_addf(جدول,
				      &objects->تر_ټولو_لوی.ونه_entries.oid,
				      objects->تر_ټولو_لوی.ونه_entries.ارزښت,
				      "    * %s", _("Maximum entries"));
	stats_جدول_addf(جدول, "  * %s", _("Blobs"));
	stats_جدول_object_اندازه_addf(جدول,
				     &objects->تر_ټولو_لوی.blob_اندازه.oid,
				     objects->تر_ټولو_لوی.blob_اندازه.ارزښت,
				     "    * %s", _("Maximum size"));
	stats_جدول_addf(جدول, "  * %s", _("Tags"));
	stats_جدول_object_اندازه_addf(جدول,
				     &objects->تر_ټولو_لوی.نښان_اندازه.oid,
				     objects->تر_ټولو_لوی.نښان_اندازه.ارزښت,
				     "    * %s", _("Maximum size"));
}

#تعریف_کړه شاخص_پلنوالی 4

ساكن فراغ stats_جدول_print_structure(ثابت بنية stats_جدول *جدول)
{
	ثابت حرف *name_ستون_title = _("Repository structure");
	ثابت حرف *ارزښت_ستون_title = _("Value");
	صحيح title_name_پلنوالی = utf8_strwidth(name_ستون_title);
	صحيح title_ارزښت_پلنوالی = utf8_strwidth(ارزښت_ستون_title);
	صحيح name_ستون_پلنوالی = جدول->name_ستون_پلنوالی;
	صحيح ارزښت_ستون_پلنوالی = جدول->ارزښت_ستون_پلنوالی;
	صحيح unit_ستون_پلنوالی = جدول->unit_ستون_پلنوالی;
	بنية string_list_توکی *توکی;
	بنية strbuf buf = STRBUF_INIT;

	إذا (title_name_پلنوالی > name_ستون_پلنوالی)
		name_ستون_پلنوالی = title_name_پلنوالی;
	إذا (title_ارزښت_پلنوالی > ارزښت_ستون_پلنوالی + unit_ستون_پلنوالی + 1)
		ارزښت_ستون_پلنوالی = title_ارزښت_پلنوالی - unit_ستون_پلنوالی;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_کیڼ, name_ستون_پلنوالی + شاخص_پلنوالی,
			  name_ستون_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_کیڼ,
			  ارزښت_ستون_پلنوالی + unit_ستون_پلنوالی + 1, ارزښت_ستون_title);
	strbuf_addstr(&buf, " |");
	چاپ_کړه("%s\n", buf.buf);

	چاپ_کړه("| ");
	لكل (صحيح i = 0; i < name_ستون_پلنوالی + شاخص_پلنوالی; i++)
		توری_چاپ_کړه('-');
	چاپ_کړه(" | ");
	لكل (صحيح i = 0; i < ارزښت_ستون_پلنوالی + unit_ستون_پلنوالی + 1; i++)
		توری_چاپ_کړه('-');
	چاپ_کړه(" |\n");

	for_each_string_list_توکی(توکی, &جدول->لیکې) {
		بنية stats_جدول_ثبت *ثبت = توکی->util;
		ثابت حرف *ارزښت = "";
		ثابت حرف *unit = "";

		إذا (ثبت) {
			ارزښت = ثبت->ارزښت;
			إذا (ثبت->unit)
				unit = ثبت->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_کیڼ, name_ستون_پلنوالی, توکی->string);

		إذا (ثبت && ثبت->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)ثبت->شاخص);
		وإلا
			strbuf_addchars(&buf, ' ', شاخص_پلنوالی);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_ښی, ارزښت_ستون_پلنوالی, ارزښت);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_کیڼ, unit_ستون_پلنوالی, unit);
		strbuf_addstr(&buf, " |");
		چاپ_کړه("%s\n", buf.buf);
	}

	إذا (جدول->annotations.nr) {
		چاپ_کړه("\n");
		for_each_string_list_توکی(توکی, &جدول->annotations)
			چاپ_کړه("%s\n", توکی->string);
	}

	strbuf_release(&buf);
}

ساكن فراغ stats_جدول_clear(بنية stats_جدول *جدول)
{
	بنية stats_جدول_ثبت *ثبت;
	بنية string_list_توکی *توکی;

	for_each_string_list_توکی(توکی, &جدول->لیکې) {
		ثبت = توکی->util;
		إذا (ثبت)
			حرر(ثبت->ارزښت);
	}

	string_list_clear(&جدول->لیکې, 1);
	string_list_clear(&جدول->annotations, 1);
}

ساكن مضمن فراغ print_keyvalue(ثابت حرف *کلی, حرف کلی_delim, اندازه_t ارزښت,
				  حرف ارزښت_delim)
{
	چاپ_کړه("%s%c%" PRIuMAX "%c", کلی, کلی_delim, (uintmax_t)ارزښت,
	       ارزښت_delim);
}

ساكن فراغ print_object_معلومات(ثابت حرف *کلی, حرف کلی_delim,
			      بنية object_معلومات *معلومات, حرف ارزښت_delim)
{
	print_keyvalue(کلی, کلی_delim, معلومات->ارزښت, ارزښت_delim);
	چاپ_کړه("%s_oid%c%s%c", کلی, کلی_delim, oid_منزل_hex(&معلومات->oid),
	       ارزښت_delim);
}

ساكن فراغ structure_keyvalue_print(بنية repo_structure *stats,
				     حرف کلی_delim, حرف ارزښت_delim)
{
	print_keyvalue("references.branches.count", کلی_delim,
		       stats->refs.branches, ارزښت_delim);
	print_keyvalue("references.tags.count", کلی_delim,
		       stats->refs.نښانونه, ارزښت_delim);
	print_keyvalue("references.remotes.count", کلی_delim,
		       stats->refs.remotes, ارزښت_delim);
	print_keyvalue("references.others.count", کلی_delim,
		       stats->refs.others, ارزښت_delim);

	print_keyvalue("objects.commits.count", کلی_delim,
		       stats->objects.type_counts.commits, ارزښت_delim);
	print_keyvalue("objects.trees.count", کلی_delim,
		       stats->objects.type_counts.trees, ارزښت_delim);
	print_keyvalue("objects.blobs.count", کلی_delim,
		       stats->objects.type_counts.blobs, ارزښت_delim);
	print_keyvalue("objects.tags.count", کلی_delim,
		       stats->objects.type_counts.نښانونه, ارزښت_delim);

	print_keyvalue("objects.commits.inflated_size", کلی_delim,
		       stats->objects.inflated_sizes.commits, ارزښت_delim);
	print_keyvalue("objects.trees.inflated_size", کلی_delim,
		       stats->objects.inflated_sizes.trees, ارزښت_delim);
	print_keyvalue("objects.blobs.inflated_size", کلی_delim,
		       stats->objects.inflated_sizes.blobs, ارزښت_delim);
	print_keyvalue("objects.tags.inflated_size", کلی_delim,
		       stats->objects.inflated_sizes.نښانونه, ارزښت_delim);

	print_keyvalue("objects.commits.disk_size", کلی_delim,
		       stats->objects.disk_sizes.commits, ارزښت_delim);
	print_keyvalue("objects.trees.disk_size", کلی_delim,
		       stats->objects.disk_sizes.trees, ارزښت_delim);
	print_keyvalue("objects.blobs.disk_size", کلی_delim,
		       stats->objects.disk_sizes.blobs, ارزښت_delim);
	print_keyvalue("objects.tags.disk_size", کلی_delim,
		       stats->objects.disk_sizes.نښانونه, ارزښت_delim);

	print_object_معلومات("objects.commits.max_size", کلی_delim,
			  &stats->objects.تر_ټولو_لوی.commit_اندازه, ارزښت_delim);
	print_object_معلومات("objects.trees.max_size", کلی_delim,
			  &stats->objects.تر_ټولو_لوی.ونه_اندازه, ارزښت_delim);
	print_object_معلومات("objects.blobs.max_size", کلی_delim,
			  &stats->objects.تر_ټولو_لوی.blob_اندازه, ارزښت_delim);
	print_object_معلومات("objects.tags.max_size", کلی_delim,
			  &stats->objects.تر_ټولو_لوی.نښان_اندازه, ارزښت_delim);

	print_object_معلومات("objects.commits.max_parents", کلی_delim,
			  &stats->objects.تر_ټولو_لوی.پلار_شمېر, ارزښت_delim);
	print_object_معلومات("objects.trees.max_entries", کلی_delim,
			  &stats->objects.تر_ټولو_لوی.ونه_entries, ارزښت_delim);

	fflush(stdout);
}

بنية شمېر_references_معلومات {
	بنية ref_stats *stats;
	بنية rev_info *revs;
	بنية پرمختګ *پرمختګ;
};

ساكن صحيح شمېر_references(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية شمېر_references_معلومات *معلومات = cb_معلومات;
	بنية ref_stats *stats = معلومات->stats;
	اندازه_t ref_شمېر;

	بدل (ref_ډول_from_refname(ref->name)) {
	حالة FILTER_REFS_BRANCHES:
		stats->branches++;
		اكسر;
	حالة FILTER_REFS_REMOTES:
		stats->remotes++;
		اكسر;
	حالة FILTER_REFS_نښانونه:
		stats->نښانونه++;
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
	جمع_کړه_pending_oid(معلومات->revs, عدم, ref->oid, 0);

	ref_شمېر = get_ټول_reference_شمېر(stats);
	display_پرمختګ(معلومات->پرمختګ, ref_شمېر);

	أرجع 0;
}

ساكن فراغ structure_شمېر_references(بنية ref_stats *stats,
				       بنية rev_info *revs,
				       بنية repository *repo,
				       صحيح show_پرمختګ)
{
	بنية شمېر_references_معلومات معلومات = {
		.stats = stats,
		.revs = revs,
	};

	إذا (show_پرمختګ)
		معلومات.پرمختګ = پیل_delayed_پرمختګ(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), شمېر_references, &معلومات);
	stop_پرمختګ(&معلومات.پرمختګ);
}

بنية شمېر_objects_معلومات {
	بنية object_database *odb;
	بنية object_stats *stats;
	بنية پرمختګ *پرمختګ;
};

ساكن فراغ check_تر_ټولو_لوی(بنية object_معلومات *معلومات, بنية object_id *oid,
			  اندازه_t ارزښت)
{
	إذا (ارزښت > معلومات->ارزښت || is_null_oid(&معلومات->oid)) {
		oidcpy(&معلومات->oid, oid);
		معلومات->ارزښت = ارزښت;
	}
}

ساكن اندازه_t شمېر_ونه_entries(بنية object *obj)
{
	بنية ونه *t = object_as_type(obj, OBJ_ونه, 0);
	بنية name_ثبت ثبت;
	بنية ونه_desc desc;
	اندازه_t شمېر = 0;

	init_ونه_desc(&desc, &t->object.oid, t->بفر, t->اندازه);
	طالما (ونه_ثبت(&desc, &ثبت))
		شمېر++;

	أرجع شمېر;
}

ساكن صحيح شمېر_objects(ثابت حرف *لار ناکارول_شوی, بنية oid_array *oids,
			 تعداد object_type type, فراغ *cb_معلومات)
{
	بنية شمېر_objects_معلومات *معلومات = cb_معلومات;
	بنية object_stats *stats = معلومات->stats;
	اندازه_t object_شمېر;

	لكل (اندازه_t i = 0; i < oids->nr; i++) {
		بنية object_info oi = OBJECT_INFO_INIT;
		بینښه اوږد inflated;
		اندازه_t inflated_st = 0;
		بنية commit *commit;
		بنية object *obj;
		فراغ *content;
		off_t disk;
		صحيح eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		إذا (odb_read_object_info_extended(معلومات->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_چټک) < 0)
			استمر;
		inflated = cast_اندازه_t_منزل_ulong(inflated_st);

		obj = parse_object_بفر(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		بدل (type) {
		حالة OBJ_نښان:
			stats->type_counts.نښانونه++;
			stats->inflated_sizes.نښانونه += inflated;
			stats->disk_sizes.نښانونه += disk;
			check_تر_ټولو_لوی(&stats->تر_ټولو_لوی.نښان_اندازه, &oids->oid[i],
				      inflated);
			اكسر;
		حالة OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_تر_ټولو_لوی(&stats->تر_ټولو_لوی.commit_اندازه, &oids->oid[i],
				      inflated);
			check_تر_ټولو_لوی(&stats->تر_ټولو_لوی.پلار_شمېر, &oids->oid[i],
				      commit_list_شمېر(commit->parents));
			اكسر;
		حالة OBJ_ونه:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_تر_ټولو_لوی(&stats->تر_ټولو_لوی.ونه_اندازه, &oids->oid[i],
				      inflated);
			check_تر_ټولو_لوی(&stats->تر_ټولو_لوی.ونه_entries, &oids->oid[i],
				      شمېر_ونه_entries(obj));
			اكسر;
		حالة OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_تر_ټولو_لوی(&stats->تر_ټولو_لوی.blob_اندازه, &oids->oid[i],
				      inflated);
			اكسر;
		افتراضي:
			BUG("invalid object type");
		}

		إذا (!eaten)
			حرر(content);
	}

	object_شمېر = get_ټول_object_ارزښتونه(&stats->type_counts);
	display_پرمختګ(معلومات->پرمختګ, object_شمېر);

	أرجع 0;
}

ساكن فراغ structure_شمېر_objects(بنية object_stats *stats,
				    بنية rev_info *revs,
				    بنية repository *repo, صحيح show_پرمختګ)
{
	بنية لار_walk_info info = لار_WALK_INFO_INIT;
	بنية شمېر_objects_معلومات معلومات = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.لار_fn = شمېر_objects;
	info.لار_fn_معلومات = &معلومات;

	إذا (show_پرمختګ)
		معلومات.پرمختګ = پیل_delayed_پرمختګ(repo, _("Counting objects"), 0);

	walk_objects_by_لار(&info);
	لار_walk_info_clear(&info);
	stop_پرمختګ(&معلومات.پرمختګ);
}

ساكن صحيح cmd_repo_structure(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			      بنية repository *repo)
{
	بنية stats_جدول جدول = {
		.لیکې = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	تعداد output_format format = FORMAT_جدول;
	بنية repo_structure stats = { 0 };
	بنية rev_info revs;
	صحيح show_پرمختګ = -1;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', عدم, &format, عدم,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_پرمختګ, N_("show progress")),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, repo_structure_usage, 0);
	إذا (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, مختاړی);

	إذا (show_پرمختګ < 0)
		show_پرمختګ = isatty(2);

	structure_شمېر_references(&stats.refs, &revs, repo, show_پرمختګ);
	structure_شمېر_objects(&stats.objects, &revs, repo, show_پرمختګ);

	بدل (format) {
	حالة FORMAT_جدول:
		stats_جدول_تیاری_structure(&جدول, &stats);
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

صحيح cmd_repo(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
	     بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, repo_usage, 0);

	أرجع fn(argc, argv, مختاړی, repo);
}
