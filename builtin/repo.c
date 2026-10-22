#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

#تعریف_کرو REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#تعریف_کرو REPO_STRUCTURE_USAGE \
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

تعريفنوع صحيح get_قدر_fn(بنية repository *repo, بنية strbuf *buf);

تعداد output_format {
	FORMAT_جدول,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

بنية repo_info_میدان {
	ثابت حرف *کلید;
	get_قدر_fn *get_قدر;
};

ساكن صحيح get_layout_bare(بنية repository *repo بن_ورتیا, بنية strbuf *buf)
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

ساكن صحيح get_رستہ_commondir_absolute(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *common_dir = repo_get_common_dir(repo);

	إذا (!common_dir)
		أرجع error(_("unable to get common directory"));

	format_رستہ(buf, common_dir, repo->اگیتر, رستہ_FORMAT_CANONICAL);
	أرجع 0;
}

ساكن صحيح get_رستہ_commondir_relative(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *common_dir = repo_get_common_dir(repo);

	إذا (!common_dir)
		أرجع error(_("unable to get common directory"));

	format_رستہ(buf, common_dir, repo->اگیتر, رستہ_FORMAT_RELATIVE);
	أرجع 0;
}

ساكن صحيح get_رستہ_gitdir_absolute(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *git_dir = repo_get_git_dir(repo);

	إذا (!git_dir)
		أرجع error(_("unable to get git directory"));

	format_رستہ(buf, git_dir, repo->اگیتر, رستہ_FORMAT_CANONICAL);
	أرجع 0;
}

ساكن صحيح get_رستہ_gitdir_relative(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *git_dir = repo_get_git_dir(repo);

	إذا (!git_dir)
		أرجع error(_("unable to get git directory"));

	format_رستہ(buf, git_dir, repo->اگیتر, رستہ_FORMAT_RELATIVE);
	أرجع 0;
}

ساكن صحيح get_references_format(بنية repository *repo, بنية strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_منزل_name(repo->ref_storage_format));
	أرجع 0;
}

/* repo_info_field keys must be in lexicographical order */
ساكن ثابت بنية repo_info_میدان repo_info_میدان[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_رستہ_commondir_absolute },
	{ "path.commondir.relative", get_رستہ_commondir_relative },
	{ "path.gitdir.absolute", get_رستہ_gitdir_absolute },
	{ "path.gitdir.relative", get_رستہ_gitdir_relative },
	{ "references.format", get_references_format },
};

ساكن صحيح repo_info_میدان_cmp(ثابت فراغ *va, ثابت فراغ *vb)
{
	ثابت بنية repo_info_میدان *a = va;
	ثابت بنية repo_info_میدان *b = vb;

	أرجع strcmp(a->کلید, b->کلید);
}

ساكن ثابت بنية repo_info_میدان *get_repo_info_میدان(ثابت حرف *کلید)
{
	ثابت بنية repo_info_میدان تلاش_کلید = { کلید, عدم };
	ثابت بنية repo_info_میدان *لبھیا_2 = bsearch(&تلاش_کلید,
						      repo_info_میدان,
						      ARRAY_ناپ(repo_info_میدان),
						      حجم(*لبھیا_2),
						      repo_info_میدان_cmp);

	أرجع لبھیا_2;
}

ساكن فراغ print_میدان(تعداد output_format format, ثابت حرف *کلید,
			ثابت حرف *قدر)
{
	بدل (format) {
	حالة FORMAT_NEWLINE_TERMINATED:
		لکھو("%s=", کلید);
		quote_c_style(قدر, عدم, stdout, 0);
		حرف_لکھو('\n');
		اكسر;
	حالة FORMAT_NUL_TERMINATED:
		لکھو("%s\n%s%c", کلید, قدر, '\0');
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
		ثابت بنية repo_info_میدان *میدان = get_repo_info_میدان(کلید);

		إذا (!میدان) {
			ret = error(_("key '%s' not found"), کلید);
			استمر;
		}

		strbuf_reset(&valbuf);
		میدان->get_قدر(repo, &valbuf);
		print_میدان(format, کلید, valbuf.buf);
	}

	strbuf_release(&valbuf);
	أرجع ret;
}

ساكن صحيح print_all_fields(بنية repository *repo,
			    تعداد output_format format)
{
	بنية strbuf valbuf = STRBUF_INIT;

	لكل (ناپ_t i = 0; i < ARRAY_ناپ(repo_info_میدان); i++) {
		ثابت بنية repo_info_میدان *میدان = &repo_info_میدان[i];

		strbuf_reset(&valbuf);
		میدان->get_قدر(repo, &valbuf);
		print_میدان(format, میدان->کلید, valbuf.buf);
	}

	strbuf_release(&valbuf);
	أرجع 0;
}

ساكن صحيح print_کلیدیں(تعداد output_format format)
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

	لكل (ناپ_t i = 0; i < ARRAY_ناپ(repo_info_میدان); i++) {
		ثابت بنية repo_info_میدان *میدان = &repo_info_میدان[i];
		لکھو("%s%c", میدان->کلید, sep);
	}

	أرجع 0;
}

ساكن صحيح parse_format_cb(ثابت بنية option *opt,
			   ثابت حرف *arg, صحيح unset بن_ورتیا)
{
	تعداد output_format *format = opt->قدر;

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

ساكن صحيح cmd_repo_info(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
			 بنية repository *repo)
{
	تعداد output_format format = FORMAT_NEWLINE_TERMINATED;
	صحيح all_کلیدیں = 0;
	صحيح show_کلیدیں = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', عدم, &format, عدم,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_کلیدیں, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_کلیدیں, N_("show keys")),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options, repo_info_usage, 0);

	إذا (show_کلیدیں && (all_کلیدیں || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	إذا (show_کلیدیں)
		أرجع print_کلیدیں(format);

	إذا (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	إذا (all_کلیدیں && argc)
		die(_("--all and <key> cannot be used together"));

	إذا (all_کلیدیں)
		أرجع print_all_fields(repo, format);
	وإلا
		أرجع print_fields(argc, argv, repo, format);
}

بنية object_انکڑے {
	بنية object_id oid;
	ناپ_t قدر;
};

بنية سبھ_توں_وڈا_objects {
	بنية object_انکڑے ٹیگ_ناپ;
	بنية object_انکڑے commit_ناپ;
	بنية object_انکڑے درخت_ناپ;
	بنية object_انکڑے blob_ناپ;

	بنية object_انکڑے ماپا_گنتی;
	بنية object_انکڑے درخت_entries;
};

بنية ref_stats {
	ناپ_t branches;
	ناپ_t remotes;
	ناپ_t ٹیگ_سوچی;
	ناپ_t others;
};

بنية object_اقدار {
	ناپ_t ٹیگ_سوچی;
	ناپ_t commits;
	ناپ_t trees;
	ناپ_t blobs;
};

بنية object_stats {
	بنية object_اقدار type_counts;
	بنية object_اقدار inflated_sizes;
	بنية object_اقدار disk_sizes;
	بنية سبھ_توں_وڈا_objects سبھ_توں_وڈا;
};

بنية repo_structure {
	بنية ref_stats refs;
	بنية object_stats objects;
};

بنية stats_جدول {
	بنية string_list صفاں;
	بنية string_list annotations;

	صحيح name_خانہ_چوڑائی;
	صحيح قدر_خانہ_چوڑائی;
	صحيح unit_خانہ_چوڑائی;
};

/*
 * Holds column data that gets stored for each row.
 */
بنية stats_جدول_اندراج {
	حرف *قدر;
	ثابت حرف *unit;
	ناپ_t اشاریہ;
	بنية object_id *oid;
};

ساكن فراغ stats_جدول_vaddf(بنية stats_جدول *جدول,
			      بنية stats_جدول_اندراج *اندراج,
			      ثابت حرف *format, va_list ap)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_شے *شے;
	حرف *formatted_name;
	صحيح name_چوڑائی;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, عدم);
	name_چوڑائی = utf8_strwidth(formatted_name);

	شے = string_list_append_nodup(&جدول->صفاں, formatted_name);
	شے->util = اندراج;

	إذا (name_چوڑائی > جدول->name_خانہ_چوڑائی)
		جدول->name_خانہ_چوڑائی = name_چوڑائی;
	إذا (!اندراج)
		أرجع;
	إذا (اندراج->oid) {
		اندراج->اشاریہ = جدول->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)اندراج->اشاریہ,
			    oid_منزل_hex(اندراج->oid));
		string_list_append_nodup(&جدول->annotations, strbuf_detach(&buf, عدم));
	}
	إذا (اندراج->قدر) {
		صحيح قدر_چوڑائی = utf8_strwidth(اندراج->قدر);
		إذا (قدر_چوڑائی > جدول->قدر_خانہ_چوڑائی)
			جدول->قدر_خانہ_چوڑائی = قدر_چوڑائی;
	}
	إذا (اندراج->unit) {
		صحيح unit_چوڑائی = utf8_strwidth(اندراج->unit);
		إذا (unit_چوڑائی > جدول->unit_خانہ_چوڑائی)
			جدول->unit_خانہ_چوڑائی = unit_چوڑائی;
	}

	strbuf_release(&buf);
}

ساكن فراغ stats_جدول_addf(بنية stats_جدول *جدول, ثابت حرف *format, ...)
{
	va_list ap;

	va_شروع(ap, format);
	stats_جدول_vaddf(جدول, عدم, format, ap);
	va_سرا(ap);
}

ساكن فراغ stats_جدول_گنتی_addf(بنية stats_جدول *جدول, ناپ_t قدر,
				   ثابت حرف *format, ...)
{
	بنية stats_جدول_اندراج *اندراج;
	va_list ap;

	CALLOC_ARRAY(اندراج, 1);
	humanise_گنتی(قدر, &اندراج->قدر, &اندراج->unit);

	va_شروع(ap, format);
	stats_جدول_vaddf(جدول, اندراج, format, ap);
	va_سرا(ap);
}

ساكن فراغ stats_جدول_object_گنتی_addf(بنية stats_جدول *جدول,
					  بنية object_id *oid, ناپ_t قدر,
					  ثابت حرف *format, ...)
{
	بنية stats_جدول_اندراج *اندراج;
	va_list ap;

	CALLOC_ARRAY(اندراج, 1);
	humanise_گنتی(قدر, &اندراج->قدر, &اندراج->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	إذا (!is_null_oid(oid))
		اندراج->oid = oid;

	va_شروع(ap, format);
	stats_جدول_vaddf(جدول, اندراج, format, ap);
	va_سرا(ap);
}

ساكن فراغ stats_جدول_ناپ_addf(بنية stats_جدول *جدول, ناپ_t قدر,
				  ثابت حرف *format, ...)
{
	بنية stats_جدول_اندراج *اندراج;
	va_list ap;

	CALLOC_ARRAY(اندراج, 1);
	humanise_بائٹاں(قدر, &اندراج->قدر, &اندراج->unit, HUMANISE_COMPACT);

	va_شروع(ap, format);
	stats_جدول_vaddf(جدول, اندراج, format, ap);
	va_سرا(ap);
}

ساكن فراغ stats_جدول_object_ناپ_addf(بنية stats_جدول *جدول,
					 بنية object_id *oid, ناپ_t قدر,
					 ثابت حرف *format, ...)
{
	بنية stats_جدول_اندراج *اندراج;
	va_list ap;

	CALLOC_ARRAY(اندراج, 1);
	humanise_بائٹاں(قدر, &اندراج->قدر, &اندراج->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	إذا (!is_null_oid(oid))
		اندراج->oid = oid;

	va_شروع(ap, format);
	stats_جدول_vaddf(جدول, اندراج, format, ap);
	va_سرا(ap);
}

ساكن مضمن ناپ_t get_کل_reference_گنتی(بنية ref_stats *stats)
{
	أرجع stats->branches + stats->remotes + stats->ٹیگ_سوچی + stats->others;
}

ساكن مضمن ناپ_t get_کل_object_اقدار(بنية object_اقدار *اقدار)
{
	أرجع اقدار->ٹیگ_سوچی + اقدار->commits + اقدار->trees + اقدار->blobs;
}

ساكن فراغ stats_جدول_تیاری_structure(بنية stats_جدول *جدول,
					بنية repo_structure *stats)
{
	بنية object_stats *objects = &stats->objects;
	بنية ref_stats *refs = &stats->refs;
	ناپ_t inflated_object_کل;
	ناپ_t object_گنتی_کل;
	ناپ_t disk_object_کل;
	ناپ_t ref_کل;

	ref_کل = get_کل_reference_گنتی(refs);
	stats_جدول_addf(جدول, "* %s", _("References"));
	stats_جدول_گنتی_addf(جدول, ref_کل, "  * %s", _("Count"));
	stats_جدول_گنتی_addf(جدول, refs->branches, "    * %s", _("Branches"));
	stats_جدول_گنتی_addf(جدول, refs->ٹیگ_سوچی, "    * %s", _("Tags"));
	stats_جدول_گنتی_addf(جدول, refs->remotes, "    * %s", _("Remotes"));
	stats_جدول_گنتی_addf(جدول, refs->others, "    * %s", _("Others"));

	object_گنتی_کل = get_کل_object_اقدار(&objects->type_counts);
	stats_جدول_addf(جدول, "");
	stats_جدول_addf(جدول, "* %s", _("Reachable objects"));
	stats_جدول_گنتی_addf(جدول, object_گنتی_کل, "  * %s", _("Count"));
	stats_جدول_گنتی_addf(جدول, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_جدول_گنتی_addf(جدول, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_جدول_گنتی_addf(جدول, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_جدول_گنتی_addf(جدول, objects->type_counts.ٹیگ_سوچی,
			       "    * %s", _("Tags"));

	inflated_object_کل = get_کل_object_اقدار(&objects->inflated_sizes);
	stats_جدول_ناپ_addf(جدول, inflated_object_کل,
			      "  * %s", _("Inflated size"));
	stats_جدول_ناپ_addf(جدول, objects->inflated_sizes.commits,
			      "    * %s", _("Commits"));
	stats_جدول_ناپ_addf(جدول, objects->inflated_sizes.trees,
			      "    * %s", _("Trees"));
	stats_جدول_ناپ_addf(جدول, objects->inflated_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_جدول_ناپ_addf(جدول, objects->inflated_sizes.ٹیگ_سوچی,
			      "    * %s", _("Tags"));

	disk_object_کل = get_کل_object_اقدار(&objects->disk_sizes);
	stats_جدول_ناپ_addf(جدول, disk_object_کل,
			      "  * %s", _("Disk size"));
	stats_جدول_ناپ_addf(جدول, objects->disk_sizes.commits,
			      "    * %s", _("Commits"));
	stats_جدول_ناپ_addf(جدول, objects->disk_sizes.trees,
			      "    * %s", _("Trees"));
	stats_جدول_ناپ_addf(جدول, objects->disk_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_جدول_ناپ_addf(جدول, objects->disk_sizes.ٹیگ_سوچی,
			      "    * %s", _("Tags"));

	stats_جدول_addf(جدول, "");
	stats_جدول_addf(جدول, "* %s", _("Largest objects"));
	stats_جدول_addf(جدول, "  * %s", _("Commits"));
	stats_جدول_object_ناپ_addf(جدول,
				     &objects->سبھ_توں_وڈا.commit_ناپ.oid,
				     objects->سبھ_توں_وڈا.commit_ناپ.قدر,
				     "    * %s", _("Maximum size"));
	stats_جدول_object_گنتی_addf(جدول,
				      &objects->سبھ_توں_وڈا.ماپا_گنتی.oid,
				      objects->سبھ_توں_وڈا.ماپا_گنتی.قدر,
				      "    * %s", _("Maximum parents"));
	stats_جدول_addf(جدول, "  * %s", _("Trees"));
	stats_جدول_object_ناپ_addf(جدول,
				     &objects->سبھ_توں_وڈا.درخت_ناپ.oid,
				     objects->سبھ_توں_وڈا.درخت_ناپ.قدر,
				     "    * %s", _("Maximum size"));
	stats_جدول_object_گنتی_addf(جدول,
				      &objects->سبھ_توں_وڈا.درخت_entries.oid,
				      objects->سبھ_توں_وڈا.درخت_entries.قدر,
				      "    * %s", _("Maximum entries"));
	stats_جدول_addf(جدول, "  * %s", _("Blobs"));
	stats_جدول_object_ناپ_addf(جدول,
				     &objects->سبھ_توں_وڈا.blob_ناپ.oid,
				     objects->سبھ_توں_وڈا.blob_ناپ.قدر,
				     "    * %s", _("Maximum size"));
	stats_جدول_addf(جدول, "  * %s", _("Tags"));
	stats_جدول_object_ناپ_addf(جدول,
				     &objects->سبھ_توں_وڈا.ٹیگ_ناپ.oid,
				     objects->سبھ_توں_وڈا.ٹیگ_ناپ.قدر,
				     "    * %s", _("Maximum size"));
}

#تعریف_کرو اشاریہ_چوڑائی 4

ساكن فراغ stats_جدول_print_structure(ثابت بنية stats_جدول *جدول)
{
	ثابت حرف *name_خانہ_title = _("Repository structure");
	ثابت حرف *قدر_خانہ_title = _("Value");
	صحيح title_name_چوڑائی = utf8_strwidth(name_خانہ_title);
	صحيح title_قدر_چوڑائی = utf8_strwidth(قدر_خانہ_title);
	صحيح name_خانہ_چوڑائی = جدول->name_خانہ_چوڑائی;
	صحيح قدر_خانہ_چوڑائی = جدول->قدر_خانہ_چوڑائی;
	صحيح unit_خانہ_چوڑائی = جدول->unit_خانہ_چوڑائی;
	بنية string_list_شے *شے;
	بنية strbuf buf = STRBUF_INIT;

	إذا (title_name_چوڑائی > name_خانہ_چوڑائی)
		name_خانہ_چوڑائی = title_name_چوڑائی;
	إذا (title_قدر_چوڑائی > قدر_خانہ_چوڑائی + unit_خانہ_چوڑائی + 1)
		قدر_خانہ_چوڑائی = title_قدر_چوڑائی - unit_خانہ_چوڑائی;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_بایاں, name_خانہ_چوڑائی + اشاریہ_چوڑائی,
			  name_خانہ_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_بایاں,
			  قدر_خانہ_چوڑائی + unit_خانہ_چوڑائی + 1, قدر_خانہ_title);
	strbuf_addstr(&buf, " |");
	لکھو("%s\n", buf.buf);

	لکھو("| ");
	لكل (صحيح i = 0; i < name_خانہ_چوڑائی + اشاریہ_چوڑائی; i++)
		حرف_لکھو('-');
	لکھو(" | ");
	لكل (صحيح i = 0; i < قدر_خانہ_چوڑائی + unit_خانہ_چوڑائی + 1; i++)
		حرف_لکھو('-');
	لکھو(" |\n");

	for_each_string_list_شے(شے, &جدول->صفاں) {
		بنية stats_جدول_اندراج *اندراج = شے->util;
		ثابت حرف *قدر = "";
		ثابت حرف *unit = "";

		إذا (اندراج) {
			قدر = اندراج->قدر;
			إذا (اندراج->unit)
				unit = اندراج->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_بایاں, name_خانہ_چوڑائی, شے->string);

		إذا (اندراج && اندراج->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)اندراج->اشاریہ);
		وإلا
			strbuf_addchars(&buf, ' ', اشاریہ_چوڑائی);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_دایاں, قدر_خانہ_چوڑائی, قدر);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_بایاں, unit_خانہ_چوڑائی, unit);
		strbuf_addstr(&buf, " |");
		لکھو("%s\n", buf.buf);
	}

	إذا (جدول->annotations.nr) {
		لکھو("\n");
		for_each_string_list_شے(شے, &جدول->annotations)
			لکھو("%s\n", شے->string);
	}

	strbuf_release(&buf);
}

ساكن فراغ stats_جدول_clear(بنية stats_جدول *جدول)
{
	بنية stats_جدول_اندراج *اندراج;
	بنية string_list_شے *شے;

	for_each_string_list_شے(شے, &جدول->صفاں) {
		اندراج = شے->util;
		إذا (اندراج)
			حرر(اندراج->قدر);
	}

	string_list_clear(&جدول->صفاں, 1);
	string_list_clear(&جدول->annotations, 1);
}

ساكن مضمن فراغ print_keyvalue(ثابت حرف *کلید, حرف کلید_delim, ناپ_t قدر,
				  حرف قدر_delim)
{
	لکھو("%s%c%" PRIuMAX "%c", کلید, کلید_delim, (uintmax_t)قدر,
	       قدر_delim);
}

ساكن فراغ print_object_انکڑے(ثابت حرف *کلید, حرف کلید_delim,
			      بنية object_انکڑے *انکڑے, حرف قدر_delim)
{
	print_keyvalue(کلید, کلید_delim, انکڑے->قدر, قدر_delim);
	لکھو("%s_oid%c%s%c", کلید, کلید_delim, oid_منزل_hex(&انکڑے->oid),
	       قدر_delim);
}

ساكن فراغ structure_keyvalue_print(بنية repo_structure *stats,
				     حرف کلید_delim, حرف قدر_delim)
{
	print_keyvalue("references.branches.count", کلید_delim,
		       stats->refs.branches, قدر_delim);
	print_keyvalue("references.tags.count", کلید_delim,
		       stats->refs.ٹیگ_سوچی, قدر_delim);
	print_keyvalue("references.remotes.count", کلید_delim,
		       stats->refs.remotes, قدر_delim);
	print_keyvalue("references.others.count", کلید_delim,
		       stats->refs.others, قدر_delim);

	print_keyvalue("objects.commits.count", کلید_delim,
		       stats->objects.type_counts.commits, قدر_delim);
	print_keyvalue("objects.trees.count", کلید_delim,
		       stats->objects.type_counts.trees, قدر_delim);
	print_keyvalue("objects.blobs.count", کلید_delim,
		       stats->objects.type_counts.blobs, قدر_delim);
	print_keyvalue("objects.tags.count", کلید_delim,
		       stats->objects.type_counts.ٹیگ_سوچی, قدر_delim);

	print_keyvalue("objects.commits.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.commits, قدر_delim);
	print_keyvalue("objects.trees.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.trees, قدر_delim);
	print_keyvalue("objects.blobs.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.blobs, قدر_delim);
	print_keyvalue("objects.tags.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.ٹیگ_سوچی, قدر_delim);

	print_keyvalue("objects.commits.disk_size", کلید_delim,
		       stats->objects.disk_sizes.commits, قدر_delim);
	print_keyvalue("objects.trees.disk_size", کلید_delim,
		       stats->objects.disk_sizes.trees, قدر_delim);
	print_keyvalue("objects.blobs.disk_size", کلید_delim,
		       stats->objects.disk_sizes.blobs, قدر_delim);
	print_keyvalue("objects.tags.disk_size", کلید_delim,
		       stats->objects.disk_sizes.ٹیگ_سوچی, قدر_delim);

	print_object_انکڑے("objects.commits.max_size", کلید_delim,
			  &stats->objects.سبھ_توں_وڈا.commit_ناپ, قدر_delim);
	print_object_انکڑے("objects.trees.max_size", کلید_delim,
			  &stats->objects.سبھ_توں_وڈا.درخت_ناپ, قدر_delim);
	print_object_انکڑے("objects.blobs.max_size", کلید_delim,
			  &stats->objects.سبھ_توں_وڈا.blob_ناپ, قدر_delim);
	print_object_انکڑے("objects.tags.max_size", کلید_delim,
			  &stats->objects.سبھ_توں_وڈا.ٹیگ_ناپ, قدر_delim);

	print_object_انکڑے("objects.commits.max_parents", کلید_delim,
			  &stats->objects.سبھ_توں_وڈا.ماپا_گنتی, قدر_delim);
	print_object_انکڑے("objects.trees.max_entries", کلید_delim,
			  &stats->objects.سبھ_توں_وڈا.درخت_entries, قدر_delim);

	fflush(stdout);
}

بنية گنتی_references_انکڑے {
	بنية ref_stats *stats;
	بنية rev_info *revs;
	بنية ترقی *ترقی;
};

ساكن صحيح گنتی_references(ثابت بنية reference *ref, فراغ *cb_انکڑے)
{
	بنية گنتی_references_انکڑے *انکڑے = cb_انکڑے;
	بنية ref_stats *stats = انکڑے->stats;
	ناپ_t ref_گنتی;

	بدل (ref_قسم_from_refname(ref->name)) {
	حالة FILTER_REFS_BRANCHES:
		stats->branches++;
		اكسر;
	حالة FILTER_REFS_REMOTES:
		stats->remotes++;
		اكسر;
	حالة FILTER_REFS_ٹیگ_سوچی:
		stats->ٹیگ_سوچی++;
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
	جوڑو_pending_oid(انکڑے->revs, عدم, ref->oid, 0);

	ref_گنتی = get_کل_reference_گنتی(stats);
	display_ترقی(انکڑے->ترقی, ref_گنتی);

	أرجع 0;
}

ساكن فراغ structure_گنتی_references(بنية ref_stats *stats,
				       بنية rev_info *revs,
				       بنية repository *repo,
				       صحيح show_ترقی)
{
	بنية گنتی_references_انکڑے انکڑے = {
		.stats = stats,
		.revs = revs,
	};

	إذا (show_ترقی)
		انکڑے.ترقی = شروع_delayed_ترقی(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), گنتی_references, &انکڑے);
	stop_ترقی(&انکڑے.ترقی);
}

بنية گنتی_objects_انکڑے {
	بنية object_database *odb;
	بنية object_stats *stats;
	بنية ترقی *ترقی;
};

ساكن فراغ check_سبھ_توں_وڈا(بنية object_انکڑے *انکڑے, بنية object_id *oid,
			  ناپ_t قدر)
{
	إذا (قدر > انکڑے->قدر || is_null_oid(&انکڑے->oid)) {
		oidcpy(&انکڑے->oid, oid);
		انکڑے->قدر = قدر;
	}
}

ساكن ناپ_t گنتی_درخت_entries(بنية object *obj)
{
	بنية درخت *t = object_as_type(obj, OBJ_درخت, 0);
	بنية name_اندراج اندراج;
	بنية درخت_desc desc;
	ناپ_t گنتی = 0;

	init_درخت_desc(&desc, &t->object.oid, t->بفر, t->ناپ);
	طالما (درخت_اندراج(&desc, &اندراج))
		گنتی++;

	أرجع گنتی;
}

ساكن صحيح گنتی_objects(ثابت حرف *رستہ بن_ورتیا, بنية oid_array *oids,
			 تعداد object_type type, فراغ *cb_انکڑے)
{
	بنية گنتی_objects_انکڑے *انکڑے = cb_انکڑے;
	بنية object_stats *stats = انکڑے->stats;
	ناپ_t object_گنتی;

	لكل (ناپ_t i = 0; i < oids->nr; i++) {
		بنية object_info oi = OBJECT_INFO_INIT;
		بنانشانی لما inflated;
		ناپ_t inflated_st = 0;
		بنية commit *commit;
		بنية object *obj;
		فراغ *content;
		off_t disk;
		صحيح eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		إذا (odb_read_object_info_extended(انکڑے->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_تیز) < 0)
			استمر;
		inflated = cast_ناپ_t_منزل_ulong(inflated_st);

		obj = parse_object_بفر(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		بدل (type) {
		حالة OBJ_ٹیگ:
			stats->type_counts.ٹیگ_سوچی++;
			stats->inflated_sizes.ٹیگ_سوچی += inflated;
			stats->disk_sizes.ٹیگ_سوچی += disk;
			check_سبھ_توں_وڈا(&stats->سبھ_توں_وڈا.ٹیگ_ناپ, &oids->oid[i],
				      inflated);
			اكسر;
		حالة OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_سبھ_توں_وڈا(&stats->سبھ_توں_وڈا.commit_ناپ, &oids->oid[i],
				      inflated);
			check_سبھ_توں_وڈا(&stats->سبھ_توں_وڈا.ماپا_گنتی, &oids->oid[i],
				      commit_list_گنتی(commit->parents));
			اكسر;
		حالة OBJ_درخت:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_سبھ_توں_وڈا(&stats->سبھ_توں_وڈا.درخت_ناپ, &oids->oid[i],
				      inflated);
			check_سبھ_توں_وڈا(&stats->سبھ_توں_وڈا.درخت_entries, &oids->oid[i],
				      گنتی_درخت_entries(obj));
			اكسر;
		حالة OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_سبھ_توں_وڈا(&stats->سبھ_توں_وڈا.blob_ناپ, &oids->oid[i],
				      inflated);
			اكسر;
		افتراضي:
			BUG("invalid object type");
		}

		إذا (!eaten)
			حرر(content);
	}

	object_گنتی = get_کل_object_اقدار(&stats->type_counts);
	display_ترقی(انکڑے->ترقی, object_گنتی);

	أرجع 0;
}

ساكن فراغ structure_گنتی_objects(بنية object_stats *stats,
				    بنية rev_info *revs,
				    بنية repository *repo, صحيح show_ترقی)
{
	بنية رستہ_walk_info info = رستہ_WALK_INFO_INIT;
	بنية گنتی_objects_انکڑے انکڑے = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.رستہ_fn = گنتی_objects;
	info.رستہ_fn_انکڑے = &انکڑے;

	إذا (show_ترقی)
		انکڑے.ترقی = شروع_delayed_ترقی(repo, _("Counting objects"), 0);

	walk_objects_by_رستہ(&info);
	رستہ_walk_info_clear(&info);
	stop_ترقی(&انکڑے.ترقی);
}

ساكن صحيح cmd_repo_structure(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
			      بنية repository *repo)
{
	بنية stats_جدول جدول = {
		.صفاں = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	تعداد output_format format = FORMAT_جدول;
	بنية repo_structure stats = { 0 };
	بنية rev_info revs;
	صحيح show_ترقی = -1;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', عدم, &format, عدم,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_ترقی, N_("show progress")),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options, repo_structure_usage, 0);
	إذا (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, اگیتر);

	إذا (show_ترقی < 0)
		show_ترقی = isatty(2);

	structure_گنتی_references(&stats.refs, &revs, repo, show_ترقی);
	structure_گنتی_objects(&stats.objects, &revs, repo, show_ترقی);

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

صحيح cmd_repo(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
	     بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options, repo_usage, 0);

	أرجع fn(argc, argv, اگیتر, repo);
}
