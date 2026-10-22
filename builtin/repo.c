#define USE_THE_REPOSITORY_VARIABLE

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

#define REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#define REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

ایستا ثابت نویسه *ثابت repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	پوچ,
};

ایستا ثابت نویسه *ثابت repo_info_usage[] = {
	REPO_INFO_USAGE,
	پوچ,
};

ایستا ثابت نویسه *ثابت repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	پوچ,
};

تعریفنوع صحیح get_مقدار_fn(ساختار repository *repo, ساختار strbuf *buf);

شمارش output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

ساختار repo_info_field {
	ثابت نویسه *کلید;
	get_مقدار_fn *get_مقدار;
};

ایستا صحیح get_layout_bare(ساختار repository *repo UNUSED, ساختار strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	بازگشت 0;
}

ایستا صحیح get_layout_shallow(ساختار repository *repo, ساختار strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	بازگشت 0;
}

ایستا صحیح get_object_format(ساختار repository *repo, ساختار strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	بازگشت 0;
}

ایستا صحیح get_path_commondir_absolute(ساختار repository *repo, ساختار strbuf *buf)
{
	ثابت نویسه *common_dir = repo_get_common_dir(repo);

	اگر (!common_dir)
		بازگشت error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	بازگشت 0;
}

ایستا صحیح get_path_commondir_relative(ساختار repository *repo, ساختار strbuf *buf)
{
	ثابت نویسه *common_dir = repo_get_common_dir(repo);

	اگر (!common_dir)
		بازگشت error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	بازگشت 0;
}

ایستا صحیح get_path_gitdir_absolute(ساختار repository *repo, ساختار strbuf *buf)
{
	ثابت نویسه *git_dir = repo_get_git_dir(repo);

	اگر (!git_dir)
		بازگشت error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	بازگشت 0;
}

ایستا صحیح get_path_gitdir_relative(ساختار repository *repo, ساختار strbuf *buf)
{
	ثابت نویسه *git_dir = repo_get_git_dir(repo);

	اگر (!git_dir)
		بازگشت error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	بازگشت 0;
}

ایستا صحیح get_references_format(ساختار repository *repo, ساختار strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_to_name(repo->ref_storage_format));
	بازگشت 0;
}

/* repo_info_field keys must be in lexicographical order */
ایستا ثابت ساختار repo_info_field repo_info_field[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_path_commondir_absolute },
	{ "path.commondir.relative", get_path_commondir_relative },
	{ "path.gitdir.absolute", get_path_gitdir_absolute },
	{ "path.gitdir.relative", get_path_gitdir_relative },
	{ "references.format", get_references_format },
};

ایستا صحیح repo_info_field_cmp(ثابت تهی *va, ثابت تهی *vb)
{
	ثابت ساختار repo_info_field *a = va;
	ثابت ساختار repo_info_field *b = vb;

	بازگشت strcmp(a->کلید, b->کلید);
}

ایستا ثابت ساختار repo_info_field *get_repo_info_field(ثابت نویسه *کلید)
{
	ثابت ساختار repo_info_field جستجو_کلید = { کلید, پوچ };
	ثابت ساختار repo_info_field *found = bsearch(&جستجو_کلید,
						      repo_info_field,
						      ARRAY_SIZE(repo_info_field),
						      اندازه(*found),
						      repo_info_field_cmp);

	بازگشت found;
}

ایستا تهی print_field(شمارش output_format format, ثابت نویسه *کلید,
			ثابت نویسه *مقدار)
{
	انتخاب (format) {
	حالت FORMAT_NEWLINE_TERMINATED:
		printf("%s=", کلید);
		quote_c_style(مقدار, پوچ, stdout, 0);
		putchar('\n');
		بشکن;
	حالت FORMAT_NUL_TERMINATED:
		printf("%s\n%s%c", کلید, مقدار, '\0');
		بشکن;
	پیشفرض:
		BUG("not a valid output format: %d", format);
	}
}

ایستا صحیح print_fields(صحیح argc, ثابت نویسه **argv,
			ساختار repository *repo,
			شمارش output_format format)
{
	صحیح ret = 0;
	ساختار strbuf valbuf = STRBUF_INIT;

	برای (صحیح i = 0; i < argc; i++) {
		ثابت نویسه *کلید = argv[i];
		ثابت ساختار repo_info_field *field = get_repo_info_field(کلید);

		اگر (!field) {
			ret = error(_("key '%s' not found"), کلید);
			ادامه;
		}

		strbuf_reset(&valbuf);
		field->get_مقدار(repo, &valbuf);
		print_field(format, کلید, valbuf.buf);
	}

	strbuf_release(&valbuf);
	بازگشت ret;
}

ایستا صحیح print_all_fields(ساختار repository *repo,
			    شمارش output_format format)
{
	ساختار strbuf valbuf = STRBUF_INIT;

	برای (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		ثابت ساختار repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_مقدار(repo, &valbuf);
		print_field(format, field->کلید, valbuf.buf);
	}

	strbuf_release(&valbuf);
	بازگشت 0;
}

ایستا صحیح print_کلیدها(شمارش output_format format)
{
	نویسه sep;

	انتخاب (format) {
	حالت FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		بشکن;
	حالت FORMAT_NUL_TERMINATED:
		sep = '\0';
		بشکن;
	پیشفرض:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	برای (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		ثابت ساختار repo_info_field *field = &repo_info_field[i];
		printf("%s%c", field->کلید, sep);
	}

	بازگشت 0;
}

ایستا صحیح parse_format_cb(ثابت ساختار option *opt,
			   ثابت نویسه *arg, صحیح unset UNUSED)
{
	شمارش output_format *format = opt->مقدار;

	اگر (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	وگرنه اگر (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	وگرنه اگر (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	وگرنه اگر (!strcmp(arg, "table"))
		*format = FORMAT_TABLE;
	وگرنه
		die(_("invalid format '%s'"), arg);

	بازگشت 0;
}

ایستا صحیح cmd_repo_info(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			 ساختار repository *repo)
{
	شمارش output_format format = FORMAT_NEWLINE_TERMINATED;
	صحیح all_کلیدها = 0;
	صحیح show_کلیدها = 0;
	ساختار option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', پوچ, &format, پوچ,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_کلیدها, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_کلیدها, N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	اگر (show_کلیدها && (all_کلیدها || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	اگر (show_کلیدها)
		بازگشت print_کلیدها(format);

	اگر (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	اگر (all_کلیدها && argc)
		die(_("--all and <key> cannot be used together"));

	اگر (all_کلیدها)
		بازگشت print_all_fields(repo, format);
	وگرنه
		بازگشت print_fields(argc, argv, repo, format);
}

ساختار object_data {
	ساختار object_id oid;
	size_t مقدار;
};

ساختار largest_objects {
	ساختار object_data tag_size;
	ساختار object_data commit_size;
	ساختار object_data درخت_size;
	ساختار object_data blob_size;

	ساختار object_data parent_شمار;
	ساختار object_data درخت_entries;
};

ساختار ref_stats {
	size_t branches;
	size_t remotes;
	size_t tags;
	size_t others;
};

ساختار object_مقادیر {
	size_t tags;
	size_t commits;
	size_t trees;
	size_t blobs;
};

ساختار object_stats {
	ساختار object_مقادیر type_counts;
	ساختار object_مقادیر inflated_sizes;
	ساختار object_مقادیر disk_sizes;
	ساختار largest_objects largest;
};

ساختار repo_structure {
	ساختار ref_stats refs;
	ساختار object_stats objects;
};

ساختار stats_table {
	ساختار string_list rows;
	ساختار string_list annotations;

	صحیح name_col_width;
	صحیح مقدار_col_width;
	صحیح unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
ساختار stats_table_entry {
	نویسه *مقدار;
	ثابت نویسه *unit;
	size_t index;
	ساختار object_id *oid;
};

ایستا تهی stats_table_vaddf(ساختار stats_table *table,
			      ساختار stats_table_entry *entry,
			      ثابت نویسه *format, va_list ap)
{
	ساختار strbuf buf = STRBUF_INIT;
	ساختار string_list_item *item;
	نویسه *formatted_name;
	صحیح name_width;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, پوچ);
	name_width = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&table->rows, formatted_name);
	item->util = entry;

	اگر (name_width > table->name_col_width)
		table->name_col_width = name_width;
	اگر (!entry)
		بازگشت;
	اگر (entry->oid) {
		entry->index = table->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)entry->index,
			    oid_to_hex(entry->oid));
		string_list_append_nodup(&table->annotations, strbuf_detach(&buf, پوچ));
	}
	اگر (entry->مقدار) {
		صحیح مقدار_width = utf8_strwidth(entry->مقدار);
		اگر (مقدار_width > table->مقدار_col_width)
			table->مقدار_col_width = مقدار_width;
	}
	اگر (entry->unit) {
		صحیح unit_width = utf8_strwidth(entry->unit);
		اگر (unit_width > table->unit_col_width)
			table->unit_col_width = unit_width;
	}

	strbuf_release(&buf);
}

ایستا تهی stats_table_addf(ساختار stats_table *table, ثابت نویسه *format, ...)
{
	va_list ap;

	va_start(ap, format);
	stats_table_vaddf(table, پوچ, format, ap);
	va_end(ap);
}

ایستا تهی stats_table_شمار_addf(ساختار stats_table *table, size_t مقدار,
				   ثابت نویسه *format, ...)
{
	ساختار stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_شمار(مقدار, &entry->مقدار, &entry->unit);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ایستا تهی stats_table_object_شمار_addf(ساختار stats_table *table,
					  ساختار object_id *oid, size_t مقدار,
					  ثابت نویسه *format, ...)
{
	ساختار stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_شمار(مقدار, &entry->مقدار, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	اگر (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ایستا تهی stats_table_size_addf(ساختار stats_table *table, size_t مقدار,
				  ثابت نویسه *format, ...)
{
	ساختار stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(مقدار, &entry->مقدار, &entry->unit, HUMANISE_COMPACT);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ایستا تهی stats_table_object_size_addf(ساختار stats_table *table,
					 ساختار object_id *oid, size_t مقدار,
					 ثابت نویسه *format, ...)
{
	ساختار stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(مقدار, &entry->مقدار, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	اگر (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ایستا درونخطی size_t get_total_reference_شمار(ساختار ref_stats *stats)
{
	بازگشت stats->branches + stats->remotes + stats->tags + stats->others;
}

ایستا درونخطی size_t get_total_object_مقادیر(ساختار object_مقادیر *مقادیر)
{
	بازگشت مقادیر->tags + مقادیر->commits + مقادیر->trees + مقادیر->blobs;
}

ایستا تهی stats_table_setup_structure(ساختار stats_table *table,
					ساختار repo_structure *stats)
{
	ساختار object_stats *objects = &stats->objects;
	ساختار ref_stats *refs = &stats->refs;
	size_t inflated_object_total;
	size_t object_شمار_total;
	size_t disk_object_total;
	size_t ref_total;

	ref_total = get_total_reference_شمار(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_شمار_addf(table, ref_total, "  * %s", _("Count"));
	stats_table_شمار_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_شمار_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_شمار_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_شمار_addf(table, refs->others, "    * %s", _("Others"));

	object_شمار_total = get_total_object_مقادیر(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_شمار_addf(table, object_شمار_total, "  * %s", _("Count"));
	stats_table_شمار_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_شمار_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_شمار_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_شمار_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_total = get_total_object_مقادیر(&objects->inflated_sizes);
	stats_table_size_addf(table, inflated_object_total,
			      "  * %s", _("Inflated size"));
	stats_table_size_addf(table, objects->inflated_sizes.commits,
			      "    * %s", _("Commits"));
	stats_table_size_addf(table, objects->inflated_sizes.trees,
			      "    * %s", _("Trees"));
	stats_table_size_addf(table, objects->inflated_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_table_size_addf(table, objects->inflated_sizes.tags,
			      "    * %s", _("Tags"));

	disk_object_total = get_total_object_مقادیر(&objects->disk_sizes);
	stats_table_size_addf(table, disk_object_total,
			      "  * %s", _("Disk size"));
	stats_table_size_addf(table, objects->disk_sizes.commits,
			      "    * %s", _("Commits"));
	stats_table_size_addf(table, objects->disk_sizes.trees,
			      "    * %s", _("Trees"));
	stats_table_size_addf(table, objects->disk_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_table_size_addf(table, objects->disk_sizes.tags,
			      "    * %s", _("Tags"));

	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Largest objects"));
	stats_table_addf(table, "  * %s", _("Commits"));
	stats_table_object_size_addf(table,
				     &objects->largest.commit_size.oid,
				     objects->largest.commit_size.مقدار,
				     "    * %s", _("Maximum size"));
	stats_table_object_شمار_addf(table,
				      &objects->largest.parent_شمار.oid,
				      objects->largest.parent_شمار.مقدار,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_size_addf(table,
				     &objects->largest.درخت_size.oid,
				     objects->largest.درخت_size.مقدار,
				     "    * %s", _("Maximum size"));
	stats_table_object_شمار_addf(table,
				      &objects->largest.درخت_entries.oid,
				      objects->largest.درخت_entries.مقدار,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_size_addf(table,
				     &objects->largest.blob_size.oid,
				     objects->largest.blob_size.مقدار,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_size_addf(table,
				     &objects->largest.tag_size.oid,
				     objects->largest.tag_size.مقدار,
				     "    * %s", _("Maximum size"));
}

#define INDEX_WIDTH 4

ایستا تهی stats_table_print_structure(ثابت ساختار stats_table *table)
{
	ثابت نویسه *name_col_title = _("Repository structure");
	ثابت نویسه *مقدار_col_title = _("Value");
	صحیح title_name_width = utf8_strwidth(name_col_title);
	صحیح title_مقدار_width = utf8_strwidth(مقدار_col_title);
	صحیح name_col_width = table->name_col_width;
	صحیح مقدار_col_width = table->مقدار_col_width;
	صحیح unit_col_width = table->unit_col_width;
	ساختار string_list_item *item;
	ساختار strbuf buf = STRBUF_INIT;

	اگر (title_name_width > name_col_width)
		name_col_width = title_name_width;
	اگر (title_مقدار_width > مقدار_col_width + unit_col_width + 1)
		مقدار_col_width = title_مقدار_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_چپ, name_col_width + INDEX_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_چپ,
			  مقدار_col_width + unit_col_width + 1, مقدار_col_title);
	strbuf_addstr(&buf, " |");
	printf("%s\n", buf.buf);

	printf("| ");
	برای (صحیح i = 0; i < name_col_width + INDEX_WIDTH; i++)
		putchar('-');
	printf(" | ");
	برای (صحیح i = 0; i < مقدار_col_width + unit_col_width + 1; i++)
		putchar('-');
	printf(" |\n");

	for_each_string_list_item(item, &table->rows) {
		ساختار stats_table_entry *entry = item->util;
		ثابت نویسه *مقدار = "";
		ثابت نویسه *unit = "";

		اگر (entry) {
			مقدار = entry->مقدار;
			اگر (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_چپ, name_col_width, item->string);

		اگر (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->index);
		وگرنه
			strbuf_addchars(&buf, ' ', INDEX_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_راست, مقدار_col_width, مقدار);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_چپ, unit_col_width, unit);
		strbuf_addstr(&buf, " |");
		printf("%s\n", buf.buf);
	}

	اگر (table->annotations.nr) {
		printf("\n");
		for_each_string_list_item(item, &table->annotations)
			printf("%s\n", item->string);
	}

	strbuf_release(&buf);
}

ایستا تهی stats_table_clear(ساختار stats_table *table)
{
	ساختار stats_table_entry *entry;
	ساختار string_list_item *item;

	for_each_string_list_item(item, &table->rows) {
		entry = item->util;
		اگر (entry)
			آزاد(entry->مقدار);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

ایستا درونخطی تهی print_keyvalue(ثابت نویسه *کلید, نویسه کلید_delim, size_t مقدار,
				  نویسه مقدار_delim)
{
	printf("%s%c%" PRIuMAX "%c", کلید, کلید_delim, (uintmax_t)مقدار,
	       مقدار_delim);
}

ایستا تهی print_object_data(ثابت نویسه *کلید, نویسه کلید_delim,
			      ساختار object_data *data, نویسه مقدار_delim)
{
	print_keyvalue(کلید, کلید_delim, data->مقدار, مقدار_delim);
	printf("%s_oid%c%s%c", کلید, کلید_delim, oid_to_hex(&data->oid),
	       مقدار_delim);
}

ایستا تهی structure_keyvalue_print(ساختار repo_structure *stats,
				     نویسه کلید_delim, نویسه مقدار_delim)
{
	print_keyvalue("references.branches.count", کلید_delim,
		       stats->refs.branches, مقدار_delim);
	print_keyvalue("references.tags.count", کلید_delim,
		       stats->refs.tags, مقدار_delim);
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
		       stats->objects.type_counts.tags, مقدار_delim);

	print_keyvalue("objects.commits.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.commits, مقدار_delim);
	print_keyvalue("objects.trees.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.trees, مقدار_delim);
	print_keyvalue("objects.blobs.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.blobs, مقدار_delim);
	print_keyvalue("objects.tags.inflated_size", کلید_delim,
		       stats->objects.inflated_sizes.tags, مقدار_delim);

	print_keyvalue("objects.commits.disk_size", کلید_delim,
		       stats->objects.disk_sizes.commits, مقدار_delim);
	print_keyvalue("objects.trees.disk_size", کلید_delim,
		       stats->objects.disk_sizes.trees, مقدار_delim);
	print_keyvalue("objects.blobs.disk_size", کلید_delim,
		       stats->objects.disk_sizes.blobs, مقدار_delim);
	print_keyvalue("objects.tags.disk_size", کلید_delim,
		       stats->objects.disk_sizes.tags, مقدار_delim);

	print_object_data("objects.commits.max_size", کلید_delim,
			  &stats->objects.largest.commit_size, مقدار_delim);
	print_object_data("objects.trees.max_size", کلید_delim,
			  &stats->objects.largest.درخت_size, مقدار_delim);
	print_object_data("objects.blobs.max_size", کلید_delim,
			  &stats->objects.largest.blob_size, مقدار_delim);
	print_object_data("objects.tags.max_size", کلید_delim,
			  &stats->objects.largest.tag_size, مقدار_delim);

	print_object_data("objects.commits.max_parents", کلید_delim,
			  &stats->objects.largest.parent_شمار, مقدار_delim);
	print_object_data("objects.trees.max_entries", کلید_delim,
			  &stats->objects.largest.درخت_entries, مقدار_delim);

	fflush(stdout);
}

ساختار شمار_references_data {
	ساختار ref_stats *stats;
	ساختار rev_info *revs;
	ساختار progress *progress;
};

ایستا صحیح شمار_references(ثابت ساختار reference *ref, تهی *cb_data)
{
	ساختار شمار_references_data *data = cb_data;
	ساختار ref_stats *stats = data->stats;
	size_t ref_شمار;

	انتخاب (ref_kind_from_refname(ref->name)) {
	حالت FILTER_REFS_BRANCHES:
		stats->branches++;
		بشکن;
	حالت FILTER_REFS_REMOTES:
		stats->remotes++;
		بشکن;
	حالت FILTER_REFS_TAGS:
		stats->tags++;
		بشکن;
	حالت FILTER_REFS_OTHERS:
		stats->others++;
		بشکن;
	پیشفرض:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	add_pending_oid(data->revs, پوچ, ref->oid, 0);

	ref_شمار = get_total_reference_شمار(stats);
	display_progress(data->progress, ref_شمار);

	بازگشت 0;
}

ایستا تهی structure_شمار_references(ساختار ref_stats *stats,
				       ساختار rev_info *revs,
				       ساختار repository *repo,
				       صحیح show_progress)
{
	ساختار شمار_references_data data = {
		.stats = stats,
		.revs = revs,
	};

	اگر (show_progress)
		data.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), شمار_references, &data);
	stop_progress(&data.progress);
}

ساختار شمار_objects_data {
	ساختار object_database *odb;
	ساختار object_stats *stats;
	ساختار progress *progress;
};

ایستا تهی check_largest(ساختار object_data *data, ساختار object_id *oid,
			  size_t مقدار)
{
	اگر (مقدار > data->مقدار || is_null_oid(&data->oid)) {
		oidcpy(&data->oid, oid);
		data->مقدار = مقدار;
	}
}

ایستا size_t شمار_درخت_entries(ساختار object *obj)
{
	ساختار درخت *t = object_as_type(obj, OBJ_درخت, 0);
	ساختار name_entry entry;
	ساختار درخت_desc desc;
	size_t شمار = 0;

	init_درخت_desc(&desc, &t->object.oid, t->buffer, t->size);
	وقتی (درخت_entry(&desc, &entry))
		شمار++;

	بازگشت شمار;
}

ایستا صحیح شمار_objects(ثابت نویسه *path UNUSED, ساختار oid_array *oids,
			 شمارش object_type type, تهی *cb_data)
{
	ساختار شمار_objects_data *data = cb_data;
	ساختار object_stats *stats = data->stats;
	size_t object_شمار;

	برای (size_t i = 0; i < oids->nr; i++) {
		ساختار object_info oi = OBJECT_INFO_INIT;
		unsigned long inflated;
		size_t inflated_st = 0;
		ساختار commit *commit;
		ساختار object *obj;
		تهی *content;
		off_t disk;
		صحیح eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		اگر (odb_read_object_info_extended(data->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_QUICK) < 0)
			ادامه;
		inflated = cast_size_t_to_ulong(inflated_st);

		obj = parse_object_buffer(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		انتخاب (type) {
		حالت OBJ_TAG:
			stats->type_counts.tags++;
			stats->inflated_sizes.tags += inflated;
			stats->disk_sizes.tags += disk;
			check_largest(&stats->largest.tag_size, &oids->oid[i],
				      inflated);
			بشکن;
		حالت OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_شمار, &oids->oid[i],
				      commit_list_شمار(commit->parents));
			بشکن;
		حالت OBJ_درخت:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.درخت_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.درخت_entries, &oids->oid[i],
				      شمار_درخت_entries(obj));
			بشکن;
		حالت OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_size, &oids->oid[i],
				      inflated);
			بشکن;
		پیشفرض:
			BUG("invalid object type");
		}

		اگر (!eaten)
			آزاد(content);
	}

	object_شمار = get_total_object_مقادیر(&stats->type_counts);
	display_progress(data->progress, object_شمار);

	بازگشت 0;
}

ایستا تهی structure_شمار_objects(ساختار object_stats *stats,
				    ساختار rev_info *revs,
				    ساختار repository *repo, صحیح show_progress)
{
	ساختار path_walk_info info = PATH_WALK_INFO_INIT;
	ساختار شمار_objects_data data = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = شمار_objects;
	info.path_fn_data = &data;

	اگر (show_progress)
		data.progress = start_delayed_progress(repo, _("Counting objects"), 0);

	walk_objects_by_path(&info);
	path_walk_info_clear(&info);
	stop_progress(&data.progress);
}

ایستا صحیح cmd_repo_structure(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			      ساختار repository *repo)
{
	ساختار stats_table table = {
		.rows = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	شمارش output_format format = FORMAT_TABLE;
	ساختار repo_structure stats = { 0 };
	ساختار rev_info revs;
	صحیح show_progress = -1;
	ساختار option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', پوچ, &format, پوچ,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_progress, N_("show progress")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_structure_usage, 0);
	اگر (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, prefix);

	اگر (show_progress < 0)
		show_progress = isatty(2);

	structure_شمار_references(&stats.refs, &revs, repo, show_progress);
	structure_شمار_objects(&stats.objects, &revs, repo, show_progress);

	انتخاب (format) {
	حالت FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_print_structure(&table);
		بشکن;
	حالت FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		بشکن;
	حالت FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		بشکن;
	پیشفرض:
		BUG("invalid output format");
	}

	stats_table_clear(&table);
	release_revisions(&revs);

	بازگشت 0;
}

صحیح cmd_repo(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
	     ساختار repository *repo)
{
	parse_opt_subcommand_fn *fn = پوچ;
	ساختار option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_usage, 0);

	بازگشت fn(argc, argv, prefix, repo);
}
