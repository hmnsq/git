#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "commit.h"
#شامل "environment.h"
#شامل "hash.h"
#شامل "hex.h"
#شامل "odb.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "path-walk.h"
#شامل "progress.h"
#شامل "quote.h"
#شامل "ref-filter.h"
#شامل "refs.h"
#شامل "revision.h"
#شامل "setup.h"
#شامل "strbuf.h"
#شامل "string-list.h"
#شامل "shallow.h"
#شامل "tree.h"
#شامل "tree-walk.h"
#شامل "utf8.h"

#تعریف_کرو REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#تعریف_کرو REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

ساکن ثابت حرف *ثابت repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	تہی,
};

ساکن ثابت حرف *ثابت repo_info_usage[] = {
	REPO_INFO_USAGE,
	تہی,
};

ساکن ثابت حرف *ثابت repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	تہی,
};

قسمتعریف صحیح get_قدر_fn(ساخت repository *repo, ساخت strbuf *buf);

شمار output_format {
	FORMAT_جدول,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

ساخت repo_info_میدان {
	ثابت حرف *کلید;
	get_قدر_fn *get_قدر;
};

ساکن صحیح get_layout_bare(ساخت repository *repo غیر_مستعمل, ساخت strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	واپس 0;
}

ساکن صحیح get_layout_shallow(ساخت repository *repo, ساخت strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	واپس 0;
}

ساکن صحیح get_object_format(ساخت repository *repo, ساخت strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	واپس 0;
}

ساکن صحیح get_راستہ_commondir_absolute(ساخت repository *repo, ساخت strbuf *buf)
{
	ثابت حرف *common_dir = repo_get_common_dir(repo);

	اگر (!common_dir)
		واپس error(_("unable to get common directory"));

	format_راستہ(buf, common_dir, repo->سابقہ, راستہ_FORMAT_CANONICAL);
	واپس 0;
}

ساکن صحیح get_راستہ_commondir_relative(ساخت repository *repo, ساخت strbuf *buf)
{
	ثابت حرف *common_dir = repo_get_common_dir(repo);

	اگر (!common_dir)
		واپس error(_("unable to get common directory"));

	format_راستہ(buf, common_dir, repo->سابقہ, راستہ_FORMAT_RELATIVE);
	واپس 0;
}

ساکن صحیح get_راستہ_gitdir_absolute(ساخت repository *repo, ساخت strbuf *buf)
{
	ثابت حرف *git_dir = repo_get_git_dir(repo);

	اگر (!git_dir)
		واپس error(_("unable to get git directory"));

	format_راستہ(buf, git_dir, repo->سابقہ, راستہ_FORMAT_CANONICAL);
	واپس 0;
}

ساکن صحیح get_راستہ_gitdir_relative(ساخت repository *repo, ساخت strbuf *buf)
{
	ثابت حرف *git_dir = repo_get_git_dir(repo);

	اگر (!git_dir)
		واپس error(_("unable to get git directory"));

	format_راستہ(buf, git_dir, repo->سابقہ, راستہ_FORMAT_RELATIVE);
	واپس 0;
}

ساکن صحیح get_references_format(ساخت repository *repo, ساخت strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_مقصد_name(repo->ref_storage_format));
	واپس 0;
}

/* repo_info_field keys must be in lexicographical order */
ساکن ثابت ساخت repo_info_میدان repo_info_میدان[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_راستہ_commondir_absolute },
	{ "path.commondir.relative", get_راستہ_commondir_relative },
	{ "path.gitdir.absolute", get_راستہ_gitdir_absolute },
	{ "path.gitdir.relative", get_راستہ_gitdir_relative },
	{ "references.format", get_references_format },
};

ساکن صحیح repo_info_میدان_cmp(ثابت خالی *va, ثابت خالی *vb)
{
	ثابت ساخت repo_info_میدان *a = va;
	ثابت ساخت repo_info_میدان *b = vb;

	واپس strcmp(a->کلید, b->کلید);
}

ساکن ثابت ساخت repo_info_میدان *get_repo_info_میدان(ثابت حرف *کلید)
{
	ثابت ساخت repo_info_میدان تلاش_کلید = { کلید, تہی };
	ثابت ساخت repo_info_میدان *ملا_2 = bsearch(&تلاش_کلید,
						      repo_info_میدان,
						      ARRAY_سائز(repo_info_میدان),
						      حجم(*ملا_2),
						      repo_info_میدان_cmp);

	واپس ملا_2;
}

ساکن خالی print_میدان(شمار output_format format, ثابت حرف *کلید,
			ثابت حرف *قدر)
{
	انتخاب (format) {
	حالت FORMAT_NEWLINE_TERMINATED:
		لکھو("%s=", کلید);
		quote_c_style(قدر, تہی, stdout, 0);
		حرف_لکھو('\n');
		توڑو;
	حالت FORMAT_NUL_TERMINATED:
		لکھو("%s\n%s%c", کلید, قدر, '\0');
		توڑو;
	طےشدہ:
		BUG("not a valid output format: %d", format);
	}
}

ساکن صحیح print_fields(صحیح argc, ثابت حرف **argv,
			ساخت repository *repo,
			شمار output_format format)
{
	صحیح ret = 0;
	ساخت strbuf valbuf = STRBUF_INIT;

	برائے (صحیح i = 0; i < argc; i++) {
		ثابت حرف *کلید = argv[i];
		ثابت ساخت repo_info_میدان *میدان = get_repo_info_میدان(کلید);

		اگر (!میدان) {
			ret = error(_("key '%s' not found"), کلید);
			جاری;
		}

		strbuf_reset(&valbuf);
		میدان->get_قدر(repo, &valbuf);
		print_میدان(format, کلید, valbuf.buf);
	}

	strbuf_release(&valbuf);
	واپس ret;
}

ساکن صحیح print_all_fields(ساخت repository *repo,
			    شمار output_format format)
{
	ساخت strbuf valbuf = STRBUF_INIT;

	برائے (سائز_t i = 0; i < ARRAY_سائز(repo_info_میدان); i++) {
		ثابت ساخت repo_info_میدان *میدان = &repo_info_میدان[i];

		strbuf_reset(&valbuf);
		میدان->get_قدر(repo, &valbuf);
		print_میدان(format, میدان->کلید, valbuf.buf);
	}

	strbuf_release(&valbuf);
	واپس 0;
}

ساکن صحیح print_کلیدیں(شمار output_format format)
{
	حرف sep;

	انتخاب (format) {
	حالت FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		توڑو;
	حالت FORMAT_NUL_TERMINATED:
		sep = '\0';
		توڑو;
	طےشدہ:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	برائے (سائز_t i = 0; i < ARRAY_سائز(repo_info_میدان); i++) {
		ثابت ساخت repo_info_میدان *میدان = &repo_info_میدان[i];
		لکھو("%s%c", میدان->کلید, sep);
	}

	واپس 0;
}

ساکن صحیح parse_format_cb(ثابت ساخت option *opt,
			   ثابت حرف *arg, صحیح unset غیر_مستعمل)
{
	شمار output_format *format = opt->قدر;

	اگر (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	ورنہ اگر (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	ورنہ اگر (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	ورنہ اگر (!strcmp(arg, "table"))
		*format = FORMAT_جدول;
	ورنہ
		die(_("invalid format '%s'"), arg);

	واپس 0;
}

ساکن صحیح cmd_repo_info(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			 ساخت repository *repo)
{
	شمار output_format format = FORMAT_NEWLINE_TERMINATED;
	صحیح all_کلیدیں = 0;
	صحیح show_کلیدیں = 0;
	ساخت option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', تہی, &format, تہی,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_کلیدیں, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_کلیدیں, N_("show keys")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, repo_info_usage, 0);

	اگر (show_کلیدیں && (all_کلیدیں || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	اگر (show_کلیدیں)
		واپس print_کلیدیں(format);

	اگر (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	اگر (all_کلیدیں && argc)
		die(_("--all and <key> cannot be used together"));

	اگر (all_کلیدیں)
		واپس print_all_fields(repo, format);
	ورنہ
		واپس print_fields(argc, argv, repo, format);
}

ساخت object_ڈیٹا {
	ساخت object_id oid;
	سائز_t قدر;
};

ساخت سب_سے_بڑا_objects {
	ساخت object_ڈیٹا لیبل_سائز;
	ساخت object_ڈیٹا commit_سائز;
	ساخت object_ڈیٹا درخت_سائز;
	ساخت object_ڈیٹا blob_سائز;

	ساخت object_ڈیٹا والد_گنتی;
	ساخت object_ڈیٹا درخت_entries;
};

ساخت ref_stats {
	سائز_t branches;
	سائز_t remotes;
	سائز_t لیبل_فہرست;
	سائز_t others;
};

ساخت object_اقدار {
	سائز_t لیبل_فہرست;
	سائز_t commits;
	سائز_t trees;
	سائز_t blobs;
};

ساخت object_stats {
	ساخت object_اقدار type_counts;
	ساخت object_اقدار inflated_sizes;
	ساخت object_اقدار disk_sizes;
	ساخت سب_سے_بڑا_objects سب_سے_بڑا;
};

ساخت repo_structure {
	ساخت ref_stats refs;
	ساخت object_stats objects;
};

ساخت stats_جدول {
	ساخت string_list صفیں;
	ساخت string_list annotations;

	صحیح name_ستون_چوڑائی;
	صحیح قدر_ستون_چوڑائی;
	صحیح unit_ستون_چوڑائی;
};

/*
 * Holds column data that gets stored for each row.
 */
ساخت stats_جدول_اندراج {
	حرف *قدر;
	ثابت حرف *unit;
	سائز_t اشاریہ;
	ساخت object_id *oid;
};

ساکن خالی stats_جدول_vaddf(ساخت stats_جدول *جدول,
			      ساخت stats_جدول_اندراج *اندراج_2,
			      ثابت حرف *format, va_list ap)
{
	ساخت strbuf buf = STRBUF_INIT;
	ساخت string_list_شے *شے;
	حرف *formatted_name;
	صحیح name_چوڑائی;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, تہی);
	name_چوڑائی = utf8_strwidth(formatted_name);

	شے = string_list_append_nodup(&جدول->صفیں, formatted_name);
	شے->util = اندراج_2;

	اگر (name_چوڑائی > جدول->name_ستون_چوڑائی)
		جدول->name_ستون_چوڑائی = name_چوڑائی;
	اگر (!اندراج_2)
		واپس;
	اگر (اندراج_2->oid) {
		اندراج_2->اشاریہ = جدول->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)اندراج_2->اشاریہ,
			    oid_مقصد_hex(اندراج_2->oid));
		string_list_append_nodup(&جدول->annotations, strbuf_detach(&buf, تہی));
	}
	اگر (اندراج_2->قدر) {
		صحیح قدر_چوڑائی = utf8_strwidth(اندراج_2->قدر);
		اگر (قدر_چوڑائی > جدول->قدر_ستون_چوڑائی)
			جدول->قدر_ستون_چوڑائی = قدر_چوڑائی;
	}
	اگر (اندراج_2->unit) {
		صحیح unit_چوڑائی = utf8_strwidth(اندراج_2->unit);
		اگر (unit_چوڑائی > جدول->unit_ستون_چوڑائی)
			جدول->unit_ستون_چوڑائی = unit_چوڑائی;
	}

	strbuf_release(&buf);
}

ساکن خالی stats_جدول_addf(ساخت stats_جدول *جدول, ثابت حرف *format, ...)
{
	va_list ap;

	va_آغاز(ap, format);
	stats_جدول_vaddf(جدول, تہی, format, ap);
	va_انتہا(ap);
}

ساکن خالی stats_جدول_گنتی_addf(ساخت stats_جدول *جدول, سائز_t قدر,
				   ثابت حرف *format, ...)
{
	ساخت stats_جدول_اندراج *اندراج_2;
	va_list ap;

	CALLOC_ARRAY(اندراج_2, 1);
	humanise_گنتی(قدر, &اندراج_2->قدر, &اندراج_2->unit);

	va_آغاز(ap, format);
	stats_جدول_vaddf(جدول, اندراج_2, format, ap);
	va_انتہا(ap);
}

ساکن خالی stats_جدول_object_گنتی_addf(ساخت stats_جدول *جدول,
					  ساخت object_id *oid, سائز_t قدر,
					  ثابت حرف *format, ...)
{
	ساخت stats_جدول_اندراج *اندراج_2;
	va_list ap;

	CALLOC_ARRAY(اندراج_2, 1);
	humanise_گنتی(قدر, &اندراج_2->قدر, &اندراج_2->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	اگر (!is_null_oid(oid))
		اندراج_2->oid = oid;

	va_آغاز(ap, format);
	stats_جدول_vaddf(جدول, اندراج_2, format, ap);
	va_انتہا(ap);
}

ساکن خالی stats_جدول_سائز_addf(ساخت stats_جدول *جدول, سائز_t قدر,
				  ثابت حرف *format, ...)
{
	ساخت stats_جدول_اندراج *اندراج_2;
	va_list ap;

	CALLOC_ARRAY(اندراج_2, 1);
	humanise_بائٹیں(قدر, &اندراج_2->قدر, &اندراج_2->unit, HUMANISE_COMPACT);

	va_آغاز(ap, format);
	stats_جدول_vaddf(جدول, اندراج_2, format, ap);
	va_انتہا(ap);
}

ساکن خالی stats_جدول_object_سائز_addf(ساخت stats_جدول *جدول,
					 ساخت object_id *oid, سائز_t قدر,
					 ثابت حرف *format, ...)
{
	ساخت stats_جدول_اندراج *اندراج_2;
	va_list ap;

	CALLOC_ARRAY(اندراج_2, 1);
	humanise_بائٹیں(قدر, &اندراج_2->قدر, &اندراج_2->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	اگر (!is_null_oid(oid))
		اندراج_2->oid = oid;

	va_آغاز(ap, format);
	stats_جدول_vaddf(جدول, اندراج_2, format, ap);
	va_انتہا(ap);
}

ساکن اندرونی سائز_t get_کل_reference_گنتی(ساخت ref_stats *stats)
{
	واپس stats->branches + stats->remotes + stats->لیبل_فہرست + stats->others;
}

ساکن اندرونی سائز_t get_کل_object_اقدار(ساخت object_اقدار *اقدار)
{
	واپس اقدار->لیبل_فہرست + اقدار->commits + اقدار->trees + اقدار->blobs;
}

ساکن خالی stats_جدول_تیاری_structure(ساخت stats_جدول *جدول,
					ساخت repo_structure *stats)
{
	ساخت object_stats *objects = &stats->objects;
	ساخت ref_stats *refs = &stats->refs;
	سائز_t inflated_object_کل;
	سائز_t object_گنتی_کل;
	سائز_t disk_object_کل;
	سائز_t ref_کل;

	ref_کل = get_کل_reference_گنتی(refs);
	stats_جدول_addf(جدول, "* %s", _("References"));
	stats_جدول_گنتی_addf(جدول, ref_کل, "  * %s", _("Count"));
	stats_جدول_گنتی_addf(جدول, refs->branches, "    * %s", _("Branches"));
	stats_جدول_گنتی_addf(جدول, refs->لیبل_فہرست, "    * %s", _("Tags"));
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
	stats_جدول_گنتی_addf(جدول, objects->type_counts.لیبل_فہرست,
			       "    * %s", _("Tags"));

	inflated_object_کل = get_کل_object_اقدار(&objects->inflated_sizes);
	stats_جدول_سائز_addf(جدول, inflated_object_کل,
			      "  * %s", _("Inflated size"));
	stats_جدول_سائز_addf(جدول, objects->inflated_sizes.commits,
			      "    * %s", _("Commits"));
	stats_جدول_سائز_addf(جدول, objects->inflated_sizes.trees,
			      "    * %s", _("Trees"));
	stats_جدول_سائز_addf(جدول, objects->inflated_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_جدول_سائز_addf(جدول, objects->inflated_sizes.لیبل_فہرست,
			      "    * %s", _("Tags"));

	disk_object_کل = get_کل_object_اقدار(&objects->disk_sizes);
	stats_جدول_سائز_addf(جدول, disk_object_کل,
			      "  * %s", _("Disk size"));
	stats_جدول_سائز_addf(جدول, objects->disk_sizes.commits,
			      "    * %s", _("Commits"));
	stats_جدول_سائز_addf(جدول, objects->disk_sizes.trees,
			      "    * %s", _("Trees"));
	stats_جدول_سائز_addf(جدول, objects->disk_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_جدول_سائز_addf(جدول, objects->disk_sizes.لیبل_فہرست,
			      "    * %s", _("Tags"));

	stats_جدول_addf(جدول, "");
	stats_جدول_addf(جدول, "* %s", _("Largest objects"));
	stats_جدول_addf(جدول, "  * %s", _("Commits"));
	stats_جدول_object_سائز_addf(جدول,
				     &objects->سب_سے_بڑا.commit_سائز.oid,
				     objects->سب_سے_بڑا.commit_سائز.قدر,
				     "    * %s", _("Maximum size"));
	stats_جدول_object_گنتی_addf(جدول,
				      &objects->سب_سے_بڑا.والد_گنتی.oid,
				      objects->سب_سے_بڑا.والد_گنتی.قدر,
				      "    * %s", _("Maximum parents"));
	stats_جدول_addf(جدول, "  * %s", _("Trees"));
	stats_جدول_object_سائز_addf(جدول,
				     &objects->سب_سے_بڑا.درخت_سائز.oid,
				     objects->سب_سے_بڑا.درخت_سائز.قدر,
				     "    * %s", _("Maximum size"));
	stats_جدول_object_گنتی_addf(جدول,
				      &objects->سب_سے_بڑا.درخت_entries.oid,
				      objects->سب_سے_بڑا.درخت_entries.قدر,
				      "    * %s", _("Maximum entries"));
	stats_جدول_addf(جدول, "  * %s", _("Blobs"));
	stats_جدول_object_سائز_addf(جدول,
				     &objects->سب_سے_بڑا.blob_سائز.oid,
				     objects->سب_سے_بڑا.blob_سائز.قدر,
				     "    * %s", _("Maximum size"));
	stats_جدول_addf(جدول, "  * %s", _("Tags"));
	stats_جدول_object_سائز_addf(جدول,
				     &objects->سب_سے_بڑا.لیبل_سائز.oid,
				     objects->سب_سے_بڑا.لیبل_سائز.قدر,
				     "    * %s", _("Maximum size"));
}

#تعریف_کرو اشاریہ_چوڑائی 4

ساکن خالی stats_جدول_print_structure(ثابت ساخت stats_جدول *جدول)
{
	ثابت حرف *name_ستون_title = _("Repository structure");
	ثابت حرف *قدر_ستون_title = _("Value");
	صحیح title_name_چوڑائی = utf8_strwidth(name_ستون_title);
	صحیح title_قدر_چوڑائی = utf8_strwidth(قدر_ستون_title);
	صحیح name_ستون_چوڑائی = جدول->name_ستون_چوڑائی;
	صحیح قدر_ستون_چوڑائی = جدول->قدر_ستون_چوڑائی;
	صحیح unit_ستون_چوڑائی = جدول->unit_ستون_چوڑائی;
	ساخت string_list_شے *شے;
	ساخت strbuf buf = STRBUF_INIT;

	اگر (title_name_چوڑائی > name_ستون_چوڑائی)
		name_ستون_چوڑائی = title_name_چوڑائی;
	اگر (title_قدر_چوڑائی > قدر_ستون_چوڑائی + unit_ستون_چوڑائی + 1)
		قدر_ستون_چوڑائی = title_قدر_چوڑائی - unit_ستون_چوڑائی;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_بایاں, name_ستون_چوڑائی + اشاریہ_چوڑائی,
			  name_ستون_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_بایاں,
			  قدر_ستون_چوڑائی + unit_ستون_چوڑائی + 1, قدر_ستون_title);
	strbuf_addstr(&buf, " |");
	لکھو("%s\n", buf.buf);

	لکھو("| ");
	برائے (صحیح i = 0; i < name_ستون_چوڑائی + اشاریہ_چوڑائی; i++)
		حرف_لکھو('-');
	لکھو(" | ");
	برائے (صحیح i = 0; i < قدر_ستون_چوڑائی + unit_ستون_چوڑائی + 1; i++)
		حرف_لکھو('-');
	لکھو(" |\n");

	for_each_string_list_شے(شے, &جدول->صفیں) {
		ساخت stats_جدول_اندراج *اندراج_2 = شے->util;
		ثابت حرف *قدر = "";
		ثابت حرف *unit = "";

		اگر (اندراج_2) {
			قدر = اندراج_2->قدر;
			اگر (اندراج_2->unit)
				unit = اندراج_2->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_بایاں, name_ستون_چوڑائی, شے->string);

		اگر (اندراج_2 && اندراج_2->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)اندراج_2->اشاریہ);
		ورنہ
			strbuf_addchars(&buf, ' ', اشاریہ_چوڑائی);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_دایاں, قدر_ستون_چوڑائی, قدر);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_بایاں, unit_ستون_چوڑائی, unit);
		strbuf_addstr(&buf, " |");
		لکھو("%s\n", buf.buf);
	}

	اگر (جدول->annotations.nr) {
		لکھو("\n");
		for_each_string_list_شے(شے, &جدول->annotations)
			لکھو("%s\n", شے->string);
	}

	strbuf_release(&buf);
}

ساکن خالی stats_جدول_clear(ساخت stats_جدول *جدول)
{
	ساخت stats_جدول_اندراج *اندراج_2;
	ساخت string_list_شے *شے;

	for_each_string_list_شے(شے, &جدول->صفیں) {
		اندراج_2 = شے->util;
		اگر (اندراج_2)
			آزاد(اندراج_2->قدر);
	}

	string_list_clear(&جدول->صفیں, 1);
	string_list_clear(&جدول->annotations, 1);
}

ساکن اندرونی خالی print_keyvalue(ثابت حرف *کلید, حرف کلید_delim, سائز_t قدر,
				  حرف قدر_delim)
{
	لکھو("%s%c%" PRIuMAX "%c", کلید, کلید_delim, (uintmax_t)قدر,
	       قدر_delim);
}

ساکن خالی print_object_ڈیٹا(ثابت حرف *کلید, حرف کلید_delim,
			      ساخت object_ڈیٹا *ڈیٹا, حرف قدر_delim)
{
	print_keyvalue(کلید, کلید_delim, ڈیٹا->قدر, قدر_delim);
	لکھو("%s_oid%c%s%c", کلید, کلید_delim, oid_مقصد_hex(&ڈیٹا->oid),
	       قدر_delim);
}

ساکن خالی structure_keyvalue_print(ساخت repo_structure *stats,
				     حرف کلید_delim, حرف قدر_delim)
{
	print_keyvalue("references.branches.count", کلید_delim,
		       stats->refs.branches, قدر_delim);
	print_keyvalue("references.tags.count", کلید_delim,
		       stats->refs.لیبل_فہرست, قدر_delim);
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
		       stats->objects.type_counts.لیبل_فہرست, قدر_delim);

	print_keyvalue("objects.commits.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.commits, قدر_delim);
	print_keyvalue("objects.trees.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.trees, قدر_delim);
	print_keyvalue("objects.blobs.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.blobs, قدر_delim);
	print_keyvalue("objects.tags.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.لیبل_فہرست, قدر_delim);

	print_keyvalue("objects.commits.disk_size", کلید_delim,
		       stats->objects.disk_sizes.commits, قدر_delim);
	print_keyvalue("objects.trees.disk_size", کلید_delim,
		       stats->objects.disk_sizes.trees, قدر_delim);
	print_keyvalue("objects.blobs.disk_size", کلید_delim,
		       stats->objects.disk_sizes.blobs, قدر_delim);
	print_keyvalue("objects.tags.disk_size", کلید_delim,
		       stats->objects.disk_sizes.لیبل_فہرست, قدر_delim);

	print_object_ڈیٹا("objects.commits.max_size", کلید_delim,
			  &stats->objects.سب_سے_بڑا.commit_سائز, قدر_delim);
	print_object_ڈیٹا("objects.trees.max_size", کلید_delim,
			  &stats->objects.سب_سے_بڑا.درخت_سائز, قدر_delim);
	print_object_ڈیٹا("objects.blobs.max_size", کلید_delim,
			  &stats->objects.سب_سے_بڑا.blob_سائز, قدر_delim);
	print_object_ڈیٹا("objects.tags.max_size", کلید_delim,
			  &stats->objects.سب_سے_بڑا.لیبل_سائز, قدر_delim);

	print_object_ڈیٹا("objects.commits.max_parents", کلید_delim,
			  &stats->objects.سب_سے_بڑا.والد_گنتی, قدر_delim);
	print_object_ڈیٹا("objects.trees.max_entries", کلید_delim,
			  &stats->objects.سب_سے_بڑا.درخت_entries, قدر_delim);

	fflush(stdout);
}

ساخت گنتی_references_ڈیٹا {
	ساخت ref_stats *stats;
	ساخت rev_info *revs;
	ساخت پیش_رفت *پیش_رفت;
};

ساکن صحیح گنتی_references(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	ساخت گنتی_references_ڈیٹا *ڈیٹا = cb_ڈیٹا;
	ساخت ref_stats *stats = ڈیٹا->stats;
	سائز_t ref_گنتی;

	انتخاب (ref_قسم_from_refname(ref->name)) {
	حالت FILTER_REFS_BRANCHES:
		stats->branches++;
		توڑو;
	حالت FILTER_REFS_REMOTES:
		stats->remotes++;
		توڑو;
	حالت FILTER_REFS_لیبل_فہرست:
		stats->لیبل_فہرست++;
		توڑو;
	حالت FILTER_REFS_OTHERS:
		stats->others++;
		توڑو;
	طےشدہ:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	جوڑو_pending_oid(ڈیٹا->revs, تہی, ref->oid, 0);

	ref_گنتی = get_کل_reference_گنتی(stats);
	display_پیش_رفت(ڈیٹا->پیش_رفت, ref_گنتی);

	واپس 0;
}

ساکن خالی structure_گنتی_references(ساخت ref_stats *stats,
				       ساخت rev_info *revs,
				       ساخت repository *repo,
				       صحیح show_پیش_رفت)
{
	ساخت گنتی_references_ڈیٹا ڈیٹا = {
		.stats = stats,
		.revs = revs,
	};

	اگر (show_پیش_رفت)
		ڈیٹا.پیش_رفت = آغاز_delayed_پیش_رفت(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), گنتی_references, &ڈیٹا);
	stop_پیش_رفت(&ڈیٹا.پیش_رفت);
}

ساخت گنتی_objects_ڈیٹا {
	ساخت object_database *odb;
	ساخت object_stats *stats;
	ساخت پیش_رفت *پیش_رفت;
};

ساکن خالی check_سب_سے_بڑا(ساخت object_ڈیٹا *ڈیٹا, ساخت object_id *oid,
			  سائز_t قدر)
{
	اگر (قدر > ڈیٹا->قدر || is_null_oid(&ڈیٹا->oid)) {
		oidcpy(&ڈیٹا->oid, oid);
		ڈیٹا->قدر = قدر;
	}
}

ساکن سائز_t گنتی_درخت_entries(ساخت object *obj)
{
	ساخت درخت *t = object_as_type(obj, OBJ_درخت, 0);
	ساخت name_اندراج اندراج_2;
	ساخت درخت_desc desc;
	سائز_t گنتی = 0;

	init_درخت_desc(&desc, &t->object.oid, t->بفر, t->سائز);
	جب (درخت_اندراج(&desc, &اندراج_2))
		گنتی++;

	واپس گنتی;
}

ساکن صحیح گنتی_objects(ثابت حرف *راستہ غیر_مستعمل, ساخت oid_array *oids,
			 شمار object_type type, خالی *cb_ڈیٹا)
{
	ساخت گنتی_objects_ڈیٹا *ڈیٹا = cb_ڈیٹا;
	ساخت object_stats *stats = ڈیٹا->stats;
	سائز_t object_گنتی;

	برائے (سائز_t i = 0; i < oids->nr; i++) {
		ساخت object_info oi = OBJECT_INFO_INIT;
		غیرعلامتی طویل inflated;
		سائز_t inflated_st = 0;
		ساخت commit *commit;
		ساخت object *obj;
		خالی *content;
		off_t disk;
		صحیح eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		اگر (odb_read_object_info_extended(ڈیٹا->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_تیز) < 0)
			جاری;
		inflated = cast_سائز_t_مقصد_ulong(inflated_st);

		obj = parse_object_بفر(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		انتخاب (type) {
		حالت OBJ_لیبل:
			stats->type_counts.لیبل_فہرست++;
			stats->inflated_sizes.لیبل_فہرست += inflated;
			stats->disk_sizes.لیبل_فہرست += disk;
			check_سب_سے_بڑا(&stats->سب_سے_بڑا.لیبل_سائز, &oids->oid[i],
				      inflated);
			توڑو;
		حالت OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_سب_سے_بڑا(&stats->سب_سے_بڑا.commit_سائز, &oids->oid[i],
				      inflated);
			check_سب_سے_بڑا(&stats->سب_سے_بڑا.والد_گنتی, &oids->oid[i],
				      commit_list_گنتی(commit->parents));
			توڑو;
		حالت OBJ_درخت:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_سب_سے_بڑا(&stats->سب_سے_بڑا.درخت_سائز, &oids->oid[i],
				      inflated);
			check_سب_سے_بڑا(&stats->سب_سے_بڑا.درخت_entries, &oids->oid[i],
				      گنتی_درخت_entries(obj));
			توڑو;
		حالت OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_سب_سے_بڑا(&stats->سب_سے_بڑا.blob_سائز, &oids->oid[i],
				      inflated);
			توڑو;
		طےشدہ:
			BUG("invalid object type");
		}

		اگر (!eaten)
			آزاد(content);
	}

	object_گنتی = get_کل_object_اقدار(&stats->type_counts);
	display_پیش_رفت(ڈیٹا->پیش_رفت, object_گنتی);

	واپس 0;
}

ساکن خالی structure_گنتی_objects(ساخت object_stats *stats,
				    ساخت rev_info *revs,
				    ساخت repository *repo, صحیح show_پیش_رفت)
{
	ساخت راستہ_walk_info info = راستہ_WALK_INFO_INIT;
	ساخت گنتی_objects_ڈیٹا ڈیٹا = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.راستہ_fn = گنتی_objects;
	info.راستہ_fn_ڈیٹا = &ڈیٹا;

	اگر (show_پیش_رفت)
		ڈیٹا.پیش_رفت = آغاز_delayed_پیش_رفت(repo, _("Counting objects"), 0);

	walk_objects_by_راستہ(&info);
	راستہ_walk_info_clear(&info);
	stop_پیش_رفت(&ڈیٹا.پیش_رفت);
}

ساکن صحیح cmd_repo_structure(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			      ساخت repository *repo)
{
	ساخت stats_جدول جدول = {
		.صفیں = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	شمار output_format format = FORMAT_جدول;
	ساخت repo_structure stats = { 0 };
	ساخت rev_info revs;
	صحیح show_پیش_رفت = -1;
	ساخت option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', تہی, &format, تہی,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_پیش_رفت, N_("show progress")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, repo_structure_usage, 0);
	اگر (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, سابقہ);

	اگر (show_پیش_رفت < 0)
		show_پیش_رفت = isatty(2);

	structure_گنتی_references(&stats.refs, &revs, repo, show_پیش_رفت);
	structure_گنتی_objects(&stats.objects, &revs, repo, show_پیش_رفت);

	انتخاب (format) {
	حالت FORMAT_جدول:
		stats_جدول_تیاری_structure(&جدول, &stats);
		stats_جدول_print_structure(&جدول);
		توڑو;
	حالت FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		توڑو;
	حالت FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		توڑو;
	طےشدہ:
		BUG("invalid output format");
	}

	stats_جدول_clear(&جدول);
	release_revisions(&revs);

	واپس 0;
}

صحیح cmd_repo(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
	     ساخت repository *repo)
{
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, repo_usage, 0);

	واپس fn(argc, argv, سابقہ, repo);
}
