#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE

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

#وصف_ڪريو REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#وصف_ڪريو REPO_STRUCTURE_USAGE \
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

بنية repo_info_ميدان {
	ثابت حرف *ڪنجي;
	get_قدر_fn *get_قدر;
};

ساكن صحيح get_layout_bare(بنية repository *repo اڻ_استعمال_ٿيل, بنية strbuf *buf)
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

ساكن صحيح get_رستو_commondir_absolute(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *common_dir = repo_get_common_dir(repo);

	إذا (!common_dir)
		أرجع error(_("unable to get common directory"));

	format_رستو(buf, common_dir, repo->اڳياڙي, رستو_FORMAT_CANONICAL);
	أرجع 0;
}

ساكن صحيح get_رستو_commondir_relative(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *common_dir = repo_get_common_dir(repo);

	إذا (!common_dir)
		أرجع error(_("unable to get common directory"));

	format_رستو(buf, common_dir, repo->اڳياڙي, رستو_FORMAT_RELATIVE);
	أرجع 0;
}

ساكن صحيح get_رستو_gitdir_absolute(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *git_dir = repo_get_git_dir(repo);

	إذا (!git_dir)
		أرجع error(_("unable to get git directory"));

	format_رستو(buf, git_dir, repo->اڳياڙي, رستو_FORMAT_CANONICAL);
	أرجع 0;
}

ساكن صحيح get_رستو_gitdir_relative(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *git_dir = repo_get_git_dir(repo);

	إذا (!git_dir)
		أرجع error(_("unable to get git directory"));

	format_رستو(buf, git_dir, repo->اڳياڙي, رستو_FORMAT_RELATIVE);
	أرجع 0;
}

ساكن صحيح get_references_format(بنية repository *repo, بنية strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_منزل_name(repo->ref_storage_format));
	أرجع 0;
}

/* repo_info_field keys must be in lexicographical order */
ساكن ثابت بنية repo_info_ميدان repo_info_ميدان[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_رستو_commondir_absolute },
	{ "path.commondir.relative", get_رستو_commondir_relative },
	{ "path.gitdir.absolute", get_رستو_gitdir_absolute },
	{ "path.gitdir.relative", get_رستو_gitdir_relative },
	{ "references.format", get_references_format },
};

ساكن صحيح repo_info_ميدان_cmp(ثابت فراغ *va, ثابت فراغ *vb)
{
	ثابت بنية repo_info_ميدان *a = va;
	ثابت بنية repo_info_ميدان *b = vb;

	أرجع strcmp(a->ڪنجي, b->ڪنجي);
}

ساكن ثابت بنية repo_info_ميدان *get_repo_info_ميدان(ثابت حرف *ڪنجي)
{
	ثابت بنية repo_info_ميدان ڳولا_ڪنجي = { ڪنجي, عدم };
	ثابت بنية repo_info_ميدان *مليو_2 = bsearch(&ڳولا_ڪنجي,
						      repo_info_ميدان,
						      ARRAY_سائيز(repo_info_ميدان),
						      حجم(*مليو_2),
						      repo_info_ميدان_cmp);

	أرجع مليو_2;
}

ساكن فراغ print_ميدان(تعداد output_format format, ثابت حرف *ڪنجي,
			ثابت حرف *قدر)
{
	بدل (format) {
	حالة FORMAT_NEWLINE_TERMINATED:
		لکو("%s=", ڪنجي);
		quote_c_style(قدر, عدم, stdout, 0);
		اکر_لکو('\n');
		اكسر;
	حالة FORMAT_NUL_TERMINATED:
		لکو("%s\n%s%c", ڪنجي, قدر, '\0');
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
		ثابت حرف *ڪنجي = argv[i];
		ثابت بنية repo_info_ميدان *ميدان = get_repo_info_ميدان(ڪنجي);

		إذا (!ميدان) {
			ret = error(_("key '%s' not found"), ڪنجي);
			استمر;
		}

		strbuf_reset(&valbuf);
		ميدان->get_قدر(repo, &valbuf);
		print_ميدان(format, ڪنجي, valbuf.buf);
	}

	strbuf_release(&valbuf);
	أرجع ret;
}

ساكن صحيح print_all_fields(بنية repository *repo,
			    تعداد output_format format)
{
	بنية strbuf valbuf = STRBUF_INIT;

	لكل (سائيز_t i = 0; i < ARRAY_سائيز(repo_info_ميدان); i++) {
		ثابت بنية repo_info_ميدان *ميدان = &repo_info_ميدان[i];

		strbuf_reset(&valbuf);
		ميدان->get_قدر(repo, &valbuf);
		print_ميدان(format, ميدان->ڪنجي, valbuf.buf);
	}

	strbuf_release(&valbuf);
	أرجع 0;
}

ساكن صحيح print_ڪنجيون(تعداد output_format format)
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

	لكل (سائيز_t i = 0; i < ARRAY_سائيز(repo_info_ميدان); i++) {
		ثابت بنية repo_info_ميدان *ميدان = &repo_info_ميدان[i];
		لکو("%s%c", ميدان->ڪنجي, sep);
	}

	أرجع 0;
}

ساكن صحيح parse_format_cb(ثابت بنية option *opt,
			   ثابت حرف *arg, صحيح unset اڻ_استعمال_ٿيل)
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

ساكن صحيح cmd_repo_info(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
			 بنية repository *repo)
{
	تعداد output_format format = FORMAT_NEWLINE_TERMINATED;
	صحيح all_ڪنجيون = 0;
	صحيح show_ڪنجيون = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', عدم, &format, عدم,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_ڪنجيون, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_ڪنجيون, N_("show keys")),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options, repo_info_usage, 0);

	إذا (show_ڪنجيون && (all_ڪنجيون || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	إذا (show_ڪنجيون)
		أرجع print_ڪنجيون(format);

	إذا (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	إذا (all_ڪنجيون && argc)
		die(_("--all and <key> cannot be used together"));

	إذا (all_ڪنجيون)
		أرجع print_all_fields(repo, format);
	وإلا
		أرجع print_fields(argc, argv, repo, format);
}

بنية object_معلومات {
	بنية object_id oid;
	سائيز_t قدر;
};

بنية سڀ_کان_وڏو_objects {
	بنية object_معلومات ٽيگ_سائيز;
	بنية object_معلومات commit_سائيز;
	بنية object_معلومات وڻ_سائيز;
	بنية object_معلومات blob_سائيز;

	بنية object_معلومات والد_ڳڻپ;
	بنية object_معلومات وڻ_entries;
};

بنية ref_stats {
	سائيز_t branches;
	سائيز_t remotes;
	سائيز_t ٽيگ_فهرست;
	سائيز_t others;
};

بنية object_قدرون {
	سائيز_t ٽيگ_فهرست;
	سائيز_t commits;
	سائيز_t trees;
	سائيز_t blobs;
};

بنية object_stats {
	بنية object_قدرون type_counts;
	بنية object_قدرون inflated_sizes;
	بنية object_قدرون disk_sizes;
	بنية سڀ_کان_وڏو_objects سڀ_کان_وڏو;
};

بنية repo_structure {
	بنية ref_stats refs;
	بنية object_stats objects;
};

بنية stats_جدول {
	بنية string_list صفون;
	بنية string_list annotations;

	صحيح name_کانو_ويڪر;
	صحيح قدر_کانو_ويڪر;
	صحيح unit_کانو_ويڪر;
};

/*
 * Holds column data that gets stored for each row.
 */
بنية stats_جدول_داخلا {
	حرف *قدر;
	ثابت حرف *unit;
	سائيز_t اشاريو;
	بنية object_id *oid;
};

ساكن فراغ stats_جدول_vaddf(بنية stats_جدول *جدول,
			      بنية stats_جدول_داخلا *داخلا,
			      ثابت حرف *format, va_list ap)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_شي *شي;
	حرف *formatted_name;
	صحيح name_ويڪر;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, عدم);
	name_ويڪر = utf8_strwidth(formatted_name);

	شي = string_list_append_nodup(&جدول->صفون, formatted_name);
	شي->util = داخلا;

	إذا (name_ويڪر > جدول->name_کانو_ويڪر)
		جدول->name_کانو_ويڪر = name_ويڪر;
	إذا (!داخلا)
		أرجع;
	إذا (داخلا->oid) {
		داخلا->اشاريو = جدول->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)داخلا->اشاريو,
			    oid_منزل_hex(داخلا->oid));
		string_list_append_nodup(&جدول->annotations, strbuf_detach(&buf, عدم));
	}
	إذا (داخلا->قدر) {
		صحيح قدر_ويڪر = utf8_strwidth(داخلا->قدر);
		إذا (قدر_ويڪر > جدول->قدر_کانو_ويڪر)
			جدول->قدر_کانو_ويڪر = قدر_ويڪر;
	}
	إذا (داخلا->unit) {
		صحيح unit_ويڪر = utf8_strwidth(داخلا->unit);
		إذا (unit_ويڪر > جدول->unit_کانو_ويڪر)
			جدول->unit_کانو_ويڪر = unit_ويڪر;
	}

	strbuf_release(&buf);
}

ساكن فراغ stats_جدول_addf(بنية stats_جدول *جدول, ثابت حرف *format, ...)
{
	va_list ap;

	va_شروعات(ap, format);
	stats_جدول_vaddf(جدول, عدم, format, ap);
	va_پڇاڙي(ap);
}

ساكن فراغ stats_جدول_ڳڻپ_addf(بنية stats_جدول *جدول, سائيز_t قدر,
				   ثابت حرف *format, ...)
{
	بنية stats_جدول_داخلا *داخلا;
	va_list ap;

	CALLOC_ARRAY(داخلا, 1);
	humanise_ڳڻپ(قدر, &داخلا->قدر, &داخلا->unit);

	va_شروعات(ap, format);
	stats_جدول_vaddf(جدول, داخلا, format, ap);
	va_پڇاڙي(ap);
}

ساكن فراغ stats_جدول_object_ڳڻپ_addf(بنية stats_جدول *جدول,
					  بنية object_id *oid, سائيز_t قدر,
					  ثابت حرف *format, ...)
{
	بنية stats_جدول_داخلا *داخلا;
	va_list ap;

	CALLOC_ARRAY(داخلا, 1);
	humanise_ڳڻپ(قدر, &داخلا->قدر, &داخلا->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	إذا (!is_null_oid(oid))
		داخلا->oid = oid;

	va_شروعات(ap, format);
	stats_جدول_vaddf(جدول, داخلا, format, ap);
	va_پڇاڙي(ap);
}

ساكن فراغ stats_جدول_سائيز_addf(بنية stats_جدول *جدول, سائيز_t قدر,
				  ثابت حرف *format, ...)
{
	بنية stats_جدول_داخلا *داخلا;
	va_list ap;

	CALLOC_ARRAY(داخلا, 1);
	humanise_بائيٽون(قدر, &داخلا->قدر, &داخلا->unit, HUMANISE_COMPACT);

	va_شروعات(ap, format);
	stats_جدول_vaddf(جدول, داخلا, format, ap);
	va_پڇاڙي(ap);
}

ساكن فراغ stats_جدول_object_سائيز_addf(بنية stats_جدول *جدول,
					 بنية object_id *oid, سائيز_t قدر,
					 ثابت حرف *format, ...)
{
	بنية stats_جدول_داخلا *داخلا;
	va_list ap;

	CALLOC_ARRAY(داخلا, 1);
	humanise_بائيٽون(قدر, &داخلا->قدر, &داخلا->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	إذا (!is_null_oid(oid))
		داخلا->oid = oid;

	va_شروعات(ap, format);
	stats_جدول_vaddf(جدول, داخلا, format, ap);
	va_پڇاڙي(ap);
}

ساكن مضمن سائيز_t get_ڪل_reference_ڳڻپ(بنية ref_stats *stats)
{
	أرجع stats->branches + stats->remotes + stats->ٽيگ_فهرست + stats->others;
}

ساكن مضمن سائيز_t get_ڪل_object_قدرون(بنية object_قدرون *قدرون)
{
	أرجع قدرون->ٽيگ_فهرست + قدرون->commits + قدرون->trees + قدرون->blobs;
}

ساكن فراغ stats_جدول_تياري_structure(بنية stats_جدول *جدول,
					بنية repo_structure *stats)
{
	بنية object_stats *objects = &stats->objects;
	بنية ref_stats *refs = &stats->refs;
	سائيز_t inflated_object_ڪل;
	سائيز_t object_ڳڻپ_ڪل;
	سائيز_t disk_object_ڪل;
	سائيز_t ref_ڪل;

	ref_ڪل = get_ڪل_reference_ڳڻپ(refs);
	stats_جدول_addf(جدول, "* %s", _("References"));
	stats_جدول_ڳڻپ_addf(جدول, ref_ڪل, "  * %s", _("Count"));
	stats_جدول_ڳڻپ_addf(جدول, refs->branches, "    * %s", _("Branches"));
	stats_جدول_ڳڻپ_addf(جدول, refs->ٽيگ_فهرست, "    * %s", _("Tags"));
	stats_جدول_ڳڻپ_addf(جدول, refs->remotes, "    * %s", _("Remotes"));
	stats_جدول_ڳڻپ_addf(جدول, refs->others, "    * %s", _("Others"));

	object_ڳڻپ_ڪل = get_ڪل_object_قدرون(&objects->type_counts);
	stats_جدول_addf(جدول, "");
	stats_جدول_addf(جدول, "* %s", _("Reachable objects"));
	stats_جدول_ڳڻپ_addf(جدول, object_ڳڻپ_ڪل, "  * %s", _("Count"));
	stats_جدول_ڳڻپ_addf(جدول, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_جدول_ڳڻپ_addf(جدول, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_جدول_ڳڻپ_addf(جدول, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_جدول_ڳڻپ_addf(جدول, objects->type_counts.ٽيگ_فهرست,
			       "    * %s", _("Tags"));

	inflated_object_ڪل = get_ڪل_object_قدرون(&objects->inflated_sizes);
	stats_جدول_سائيز_addf(جدول, inflated_object_ڪل,
			      "  * %s", _("Inflated size"));
	stats_جدول_سائيز_addf(جدول, objects->inflated_sizes.commits,
			      "    * %s", _("Commits"));
	stats_جدول_سائيز_addf(جدول, objects->inflated_sizes.trees,
			      "    * %s", _("Trees"));
	stats_جدول_سائيز_addf(جدول, objects->inflated_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_جدول_سائيز_addf(جدول, objects->inflated_sizes.ٽيگ_فهرست,
			      "    * %s", _("Tags"));

	disk_object_ڪل = get_ڪل_object_قدرون(&objects->disk_sizes);
	stats_جدول_سائيز_addf(جدول, disk_object_ڪل,
			      "  * %s", _("Disk size"));
	stats_جدول_سائيز_addf(جدول, objects->disk_sizes.commits,
			      "    * %s", _("Commits"));
	stats_جدول_سائيز_addf(جدول, objects->disk_sizes.trees,
			      "    * %s", _("Trees"));
	stats_جدول_سائيز_addf(جدول, objects->disk_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_جدول_سائيز_addf(جدول, objects->disk_sizes.ٽيگ_فهرست,
			      "    * %s", _("Tags"));

	stats_جدول_addf(جدول, "");
	stats_جدول_addf(جدول, "* %s", _("Largest objects"));
	stats_جدول_addf(جدول, "  * %s", _("Commits"));
	stats_جدول_object_سائيز_addf(جدول,
				     &objects->سڀ_کان_وڏو.commit_سائيز.oid,
				     objects->سڀ_کان_وڏو.commit_سائيز.قدر,
				     "    * %s", _("Maximum size"));
	stats_جدول_object_ڳڻپ_addf(جدول,
				      &objects->سڀ_کان_وڏو.والد_ڳڻپ.oid,
				      objects->سڀ_کان_وڏو.والد_ڳڻپ.قدر,
				      "    * %s", _("Maximum parents"));
	stats_جدول_addf(جدول, "  * %s", _("Trees"));
	stats_جدول_object_سائيز_addf(جدول,
				     &objects->سڀ_کان_وڏو.وڻ_سائيز.oid,
				     objects->سڀ_کان_وڏو.وڻ_سائيز.قدر,
				     "    * %s", _("Maximum size"));
	stats_جدول_object_ڳڻپ_addf(جدول,
				      &objects->سڀ_کان_وڏو.وڻ_entries.oid,
				      objects->سڀ_کان_وڏو.وڻ_entries.قدر,
				      "    * %s", _("Maximum entries"));
	stats_جدول_addf(جدول, "  * %s", _("Blobs"));
	stats_جدول_object_سائيز_addf(جدول,
				     &objects->سڀ_کان_وڏو.blob_سائيز.oid,
				     objects->سڀ_کان_وڏو.blob_سائيز.قدر,
				     "    * %s", _("Maximum size"));
	stats_جدول_addf(جدول, "  * %s", _("Tags"));
	stats_جدول_object_سائيز_addf(جدول,
				     &objects->سڀ_کان_وڏو.ٽيگ_سائيز.oid,
				     objects->سڀ_کان_وڏو.ٽيگ_سائيز.قدر,
				     "    * %s", _("Maximum size"));
}

#وصف_ڪريو اشاريو_ويڪر 4

ساكن فراغ stats_جدول_print_structure(ثابت بنية stats_جدول *جدول)
{
	ثابت حرف *name_کانو_title = _("Repository structure");
	ثابت حرف *قدر_کانو_title = _("Value");
	صحيح title_name_ويڪر = utf8_strwidth(name_کانو_title);
	صحيح title_قدر_ويڪر = utf8_strwidth(قدر_کانو_title);
	صحيح name_کانو_ويڪر = جدول->name_کانو_ويڪر;
	صحيح قدر_کانو_ويڪر = جدول->قدر_کانو_ويڪر;
	صحيح unit_کانو_ويڪر = جدول->unit_کانو_ويڪر;
	بنية string_list_شي *شي;
	بنية strbuf buf = STRBUF_INIT;

	إذا (title_name_ويڪر > name_کانو_ويڪر)
		name_کانو_ويڪر = title_name_ويڪر;
	إذا (title_قدر_ويڪر > قدر_کانو_ويڪر + unit_کانو_ويڪر + 1)
		قدر_کانو_ويڪر = title_قدر_ويڪر - unit_کانو_ويڪر;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_کاٻو, name_کانو_ويڪر + اشاريو_ويڪر,
			  name_کانو_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_کاٻو,
			  قدر_کانو_ويڪر + unit_کانو_ويڪر + 1, قدر_کانو_title);
	strbuf_addstr(&buf, " |");
	لکو("%s\n", buf.buf);

	لکو("| ");
	لكل (صحيح i = 0; i < name_کانو_ويڪر + اشاريو_ويڪر; i++)
		اکر_لکو('-');
	لکو(" | ");
	لكل (صحيح i = 0; i < قدر_کانو_ويڪر + unit_کانو_ويڪر + 1; i++)
		اکر_لکو('-');
	لکو(" |\n");

	for_each_string_list_شي(شي, &جدول->صفون) {
		بنية stats_جدول_داخلا *داخلا = شي->util;
		ثابت حرف *قدر = "";
		ثابت حرف *unit = "";

		إذا (داخلا) {
			قدر = داخلا->قدر;
			إذا (داخلا->unit)
				unit = داخلا->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_کاٻو, name_کانو_ويڪر, شي->string);

		إذا (داخلا && داخلا->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)داخلا->اشاريو);
		وإلا
			strbuf_addchars(&buf, ' ', اشاريو_ويڪر);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_ساڄو, قدر_کانو_ويڪر, قدر);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_کاٻو, unit_کانو_ويڪر, unit);
		strbuf_addstr(&buf, " |");
		لکو("%s\n", buf.buf);
	}

	إذا (جدول->annotations.nr) {
		لکو("\n");
		for_each_string_list_شي(شي, &جدول->annotations)
			لکو("%s\n", شي->string);
	}

	strbuf_release(&buf);
}

ساكن فراغ stats_جدول_clear(بنية stats_جدول *جدول)
{
	بنية stats_جدول_داخلا *داخلا;
	بنية string_list_شي *شي;

	for_each_string_list_شي(شي, &جدول->صفون) {
		داخلا = شي->util;
		إذا (داخلا)
			حرر(داخلا->قدر);
	}

	string_list_clear(&جدول->صفون, 1);
	string_list_clear(&جدول->annotations, 1);
}

ساكن مضمن فراغ print_keyvalue(ثابت حرف *ڪنجي, حرف ڪنجي_delim, سائيز_t قدر,
				  حرف قدر_delim)
{
	لکو("%s%c%" PRIuMAX "%c", ڪنجي, ڪنجي_delim, (uintmax_t)قدر,
	       قدر_delim);
}

ساكن فراغ print_object_معلومات(ثابت حرف *ڪنجي, حرف ڪنجي_delim,
			      بنية object_معلومات *معلومات, حرف قدر_delim)
{
	print_keyvalue(ڪنجي, ڪنجي_delim, معلومات->قدر, قدر_delim);
	لکو("%s_oid%c%s%c", ڪنجي, ڪنجي_delim, oid_منزل_hex(&معلومات->oid),
	       قدر_delim);
}

ساكن فراغ structure_keyvalue_print(بنية repo_structure *stats,
				     حرف ڪنجي_delim, حرف قدر_delim)
{
	print_keyvalue("references.branches.count", ڪنجي_delim,
		       stats->refs.branches, قدر_delim);
	print_keyvalue("references.tags.count", ڪنجي_delim,
		       stats->refs.ٽيگ_فهرست, قدر_delim);
	print_keyvalue("references.remotes.count", ڪنجي_delim,
		       stats->refs.remotes, قدر_delim);
	print_keyvalue("references.others.count", ڪنجي_delim,
		       stats->refs.others, قدر_delim);

	print_keyvalue("objects.commits.count", ڪنجي_delim,
		       stats->objects.type_counts.commits, قدر_delim);
	print_keyvalue("objects.trees.count", ڪنجي_delim,
		       stats->objects.type_counts.trees, قدر_delim);
	print_keyvalue("objects.blobs.count", ڪنجي_delim,
		       stats->objects.type_counts.blobs, قدر_delim);
	print_keyvalue("objects.tags.count", ڪنجي_delim,
		       stats->objects.type_counts.ٽيگ_فهرست, قدر_delim);

	print_keyvalue("objects.commits.inflated_size", ڪنجي_delim,
		       stats->objects.inflated_sizes.commits, قدر_delim);
	print_keyvalue("objects.trees.inflated_size", ڪنجي_delim,
		       stats->objects.inflated_sizes.trees, قدر_delim);
	print_keyvalue("objects.blobs.inflated_size", ڪنجي_delim,
		       stats->objects.inflated_sizes.blobs, قدر_delim);
	print_keyvalue("objects.tags.inflated_size", ڪنجي_delim,
		       stats->objects.inflated_sizes.ٽيگ_فهرست, قدر_delim);

	print_keyvalue("objects.commits.disk_size", ڪنجي_delim,
		       stats->objects.disk_sizes.commits, قدر_delim);
	print_keyvalue("objects.trees.disk_size", ڪنجي_delim,
		       stats->objects.disk_sizes.trees, قدر_delim);
	print_keyvalue("objects.blobs.disk_size", ڪنجي_delim,
		       stats->objects.disk_sizes.blobs, قدر_delim);
	print_keyvalue("objects.tags.disk_size", ڪنجي_delim,
		       stats->objects.disk_sizes.ٽيگ_فهرست, قدر_delim);

	print_object_معلومات("objects.commits.max_size", ڪنجي_delim,
			  &stats->objects.سڀ_کان_وڏو.commit_سائيز, قدر_delim);
	print_object_معلومات("objects.trees.max_size", ڪنجي_delim,
			  &stats->objects.سڀ_کان_وڏو.وڻ_سائيز, قدر_delim);
	print_object_معلومات("objects.blobs.max_size", ڪنجي_delim,
			  &stats->objects.سڀ_کان_وڏو.blob_سائيز, قدر_delim);
	print_object_معلومات("objects.tags.max_size", ڪنجي_delim,
			  &stats->objects.سڀ_کان_وڏو.ٽيگ_سائيز, قدر_delim);

	print_object_معلومات("objects.commits.max_parents", ڪنجي_delim,
			  &stats->objects.سڀ_کان_وڏو.والد_ڳڻپ, قدر_delim);
	print_object_معلومات("objects.trees.max_entries", ڪنجي_delim,
			  &stats->objects.سڀ_کان_وڏو.وڻ_entries, قدر_delim);

	fflush(stdout);
}

بنية ڳڻپ_references_معلومات {
	بنية ref_stats *stats;
	بنية rev_info *revs;
	بنية ترقي *ترقي;
};

ساكن صحيح ڳڻپ_references(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية ڳڻپ_references_معلومات *معلومات = cb_معلومات;
	بنية ref_stats *stats = معلومات->stats;
	سائيز_t ref_ڳڻپ;

	بدل (ref_قسم_from_refname(ref->name)) {
	حالة FILTER_REFS_BRANCHES:
		stats->branches++;
		اكسر;
	حالة FILTER_REFS_REMOTES:
		stats->remotes++;
		اكسر;
	حالة FILTER_REFS_ٽيگ_فهرست:
		stats->ٽيگ_فهرست++;
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
	جوڙيو_pending_oid(معلومات->revs, عدم, ref->oid, 0);

	ref_ڳڻپ = get_ڪل_reference_ڳڻپ(stats);
	display_ترقي(معلومات->ترقي, ref_ڳڻپ);

	أرجع 0;
}

ساكن فراغ structure_ڳڻپ_references(بنية ref_stats *stats,
				       بنية rev_info *revs,
				       بنية repository *repo,
				       صحيح show_ترقي)
{
	بنية ڳڻپ_references_معلومات معلومات = {
		.stats = stats,
		.revs = revs,
	};

	إذا (show_ترقي)
		معلومات.ترقي = شروعات_delayed_ترقي(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), ڳڻپ_references, &معلومات);
	stop_ترقي(&معلومات.ترقي);
}

بنية ڳڻپ_objects_معلومات {
	بنية object_database *odb;
	بنية object_stats *stats;
	بنية ترقي *ترقي;
};

ساكن فراغ check_سڀ_کان_وڏو(بنية object_معلومات *معلومات, بنية object_id *oid,
			  سائيز_t قدر)
{
	إذا (قدر > معلومات->قدر || is_null_oid(&معلومات->oid)) {
		oidcpy(&معلومات->oid, oid);
		معلومات->قدر = قدر;
	}
}

ساكن سائيز_t ڳڻپ_وڻ_entries(بنية object *obj)
{
	بنية وڻ *t = object_as_type(obj, OBJ_وڻ, 0);
	بنية name_داخلا داخلا;
	بنية وڻ_desc desc;
	سائيز_t ڳڻپ = 0;

	init_وڻ_desc(&desc, &t->object.oid, t->بفر, t->سائيز);
	طالما (وڻ_داخلا(&desc, &داخلا))
		ڳڻپ++;

	أرجع ڳڻپ;
}

ساكن صحيح ڳڻپ_objects(ثابت حرف *رستو اڻ_استعمال_ٿيل, بنية oid_array *oids,
			 تعداد object_type type, فراغ *cb_معلومات)
{
	بنية ڳڻپ_objects_معلومات *معلومات = cb_معلومات;
	بنية object_stats *stats = معلومات->stats;
	سائيز_t object_ڳڻپ;

	لكل (سائيز_t i = 0; i < oids->nr; i++) {
		بنية object_info oi = OBJECT_INFO_INIT;
		بنانشان ڊگهو inflated;
		سائيز_t inflated_st = 0;
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
						  OBJECT_INFO_تيز) < 0)
			استمر;
		inflated = cast_سائيز_t_منزل_ulong(inflated_st);

		obj = parse_object_بفر(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		بدل (type) {
		حالة OBJ_ٽيگ:
			stats->type_counts.ٽيگ_فهرست++;
			stats->inflated_sizes.ٽيگ_فهرست += inflated;
			stats->disk_sizes.ٽيگ_فهرست += disk;
			check_سڀ_کان_وڏو(&stats->سڀ_کان_وڏو.ٽيگ_سائيز, &oids->oid[i],
				      inflated);
			اكسر;
		حالة OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_سڀ_کان_وڏو(&stats->سڀ_کان_وڏو.commit_سائيز, &oids->oid[i],
				      inflated);
			check_سڀ_کان_وڏو(&stats->سڀ_کان_وڏو.والد_ڳڻپ, &oids->oid[i],
				      commit_list_ڳڻپ(commit->parents));
			اكسر;
		حالة OBJ_وڻ:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_سڀ_کان_وڏو(&stats->سڀ_کان_وڏو.وڻ_سائيز, &oids->oid[i],
				      inflated);
			check_سڀ_کان_وڏو(&stats->سڀ_کان_وڏو.وڻ_entries, &oids->oid[i],
				      ڳڻپ_وڻ_entries(obj));
			اكسر;
		حالة OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_سڀ_کان_وڏو(&stats->سڀ_کان_وڏو.blob_سائيز, &oids->oid[i],
				      inflated);
			اكسر;
		افتراضي:
			BUG("invalid object type");
		}

		إذا (!eaten)
			حرر(content);
	}

	object_ڳڻپ = get_ڪل_object_قدرون(&stats->type_counts);
	display_ترقي(معلومات->ترقي, object_ڳڻپ);

	أرجع 0;
}

ساكن فراغ structure_ڳڻپ_objects(بنية object_stats *stats,
				    بنية rev_info *revs,
				    بنية repository *repo, صحيح show_ترقي)
{
	بنية رستو_walk_info info = رستو_WALK_INFO_INIT;
	بنية ڳڻپ_objects_معلومات معلومات = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.رستو_fn = ڳڻپ_objects;
	info.رستو_fn_معلومات = &معلومات;

	إذا (show_ترقي)
		معلومات.ترقي = شروعات_delayed_ترقي(repo, _("Counting objects"), 0);

	walk_objects_by_رستو(&info);
	رستو_walk_info_clear(&info);
	stop_ترقي(&معلومات.ترقي);
}

ساكن صحيح cmd_repo_structure(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
			      بنية repository *repo)
{
	بنية stats_جدول جدول = {
		.صفون = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	تعداد output_format format = FORMAT_جدول;
	بنية repo_structure stats = { 0 };
	بنية rev_info revs;
	صحيح show_ترقي = -1;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', عدم, &format, عدم,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_ترقي, N_("show progress")),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options, repo_structure_usage, 0);
	إذا (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, اڳياڙي);

	إذا (show_ترقي < 0)
		show_ترقي = isatty(2);

	structure_ڳڻپ_references(&stats.refs, &revs, repo, show_ترقي);
	structure_ڳڻپ_objects(&stats.objects, &revs, repo, show_ترقي);

	بدل (format) {
	حالة FORMAT_جدول:
		stats_جدول_تياري_structure(&جدول, &stats);
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

صحيح cmd_repo(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
	     بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options, repo_usage, 0);

	أرجع fn(argc, argv, اڳياڙي, repo);
}
