#define USE_THE_REPOSITORY_VARIABLE

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

#define REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#define REPO_STRUCTURE_USAGE \
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

تعريفنوع صحيح get_درج_fn(بنية repository *repo, بنية strbuf *buf);

تعداد output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

بنية repo_info_field {
	ثابت حرف *مبودي;
	get_درج_fn *get_درج;
};

ساكن صحيح get_layout_bare(بنية repository *repo UNUSED, بنية strbuf *buf)
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

ساكن صحيح get_path_commondir_absolute(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *common_dir = repo_get_common_dir(repo);

	إذا (!common_dir)
		أرجع error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	أرجع 0;
}

ساكن صحيح get_path_commondir_relative(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *common_dir = repo_get_common_dir(repo);

	إذا (!common_dir)
		أرجع error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	أرجع 0;
}

ساكن صحيح get_path_gitdir_absolute(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *git_dir = repo_get_git_dir(repo);

	إذا (!git_dir)
		أرجع error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	أرجع 0;
}

ساكن صحيح get_path_gitdir_relative(بنية repository *repo, بنية strbuf *buf)
{
	ثابت حرف *git_dir = repo_get_git_dir(repo);

	إذا (!git_dir)
		أرجع error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	أرجع 0;
}

ساكن صحيح get_references_format(بنية repository *repo, بنية strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_to_name(repo->ref_storage_format));
	أرجع 0;
}

/* repo_info_field keys must be in lexicographical order */
ساكن ثابت بنية repo_info_field repo_info_field[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_path_commondir_absolute },
	{ "path.commondir.relative", get_path_commondir_relative },
	{ "path.gitdir.absolute", get_path_gitdir_absolute },
	{ "path.gitdir.relative", get_path_gitdir_relative },
	{ "references.format", get_references_format },
};

ساكن صحيح repo_info_field_cmp(ثابت فراغ *va, ثابت فراغ *vb)
{
	ثابت بنية repo_info_field *a = va;
	ثابت بنية repo_info_field *b = vb;

	أرجع strcmp(a->مبودي, b->مبودي);
}

ساكن ثابت بنية repo_info_field *get_repo_info_field(ثابت حرف *مبودي)
{
	ثابت بنية repo_info_field نيما_مبودي = { مبودي, عدم };
	ثابت بنية repo_info_field *found = bsearch(&نيما_مبودي,
						      repo_info_field,
						      ARRAY_SIZE(repo_info_field),
						      حجم(*found),
						      repo_info_field_cmp);

	أرجع found;
}

ساكن فراغ بغ_field(تعداد output_format format, ثابت حرف *مبودي,
			ثابت حرف *درج)
{
	بدل (format) {
	حالة FORMAT_NEWLINE_TERMINATED:
		printf("%s=", مبودي);
		quote_c_style(درج, عدم, stdout, 0);
		putchar('\n');
		اكسر;
	حالة FORMAT_NUL_TERMINATED:
		printf("%s\n%s%c", مبودي, درج, '\0');
		اكسر;
	افتراضي:
		BUG("not a valid output format: %d", format);
	}
}

ساكن صحيح بغ_fields(صحيح argc, ثابت حرف **argv,
			بنية repository *repo,
			تعداد output_format format)
{
	صحيح ret = 0;
	بنية strbuf valbuf = STRBUF_INIT;

	لكل (صحيح i = 0; i < argc; i++) {
		ثابت حرف *مبودي = argv[i];
		ثابت بنية repo_info_field *field = get_repo_info_field(مبودي);

		إذا (!field) {
			ret = error(_("key '%s' not found"), مبودي);
			استمر;
		}

		strbuf_reset(&valbuf);
		field->get_درج(repo, &valbuf);
		بغ_field(format, مبودي, valbuf.buf);
	}

	strbuf_release(&valbuf);
	أرجع ret;
}

ساكن صحيح بغ_all_fields(بنية repository *repo,
			    تعداد output_format format)
{
	بنية strbuf valbuf = STRBUF_INIT;

	لكل (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		ثابت بنية repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_درج(repo, &valbuf);
		بغ_field(format, field->مبودي, valbuf.buf);
	}

	strbuf_release(&valbuf);
	أرجع 0;
}

ساكن صحيح بغ_مبوداي(تعداد output_format format)
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

	لكل (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		ثابت بنية repo_info_field *field = &repo_info_field[i];
		printf("%s%c", field->مبودي, sep);
	}

	أرجع 0;
}

ساكن صحيح parse_format_cb(ثابت بنية option *opt,
			   ثابت حرف *arg, صحيح unset UNUSED)
{
	تعداد output_format *format = opt->درج;

	إذا (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	وإلا إذا (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	وإلا إذا (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	وإلا إذا (!strcmp(arg, "table"))
		*format = FORMAT_TABLE;
	وإلا
		die(_("invalid format '%s'"), arg);

	أرجع 0;
}

ساكن صحيح cmd_repo_info(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			 بنية repository *repo)
{
	تعداد output_format format = FORMAT_NEWLINE_TERMINATED;
	صحيح all_مبوداي = 0;
	صحيح show_مبوداي = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', عدم, &format, عدم,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_مبوداي, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_مبوداي, N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	إذا (show_مبوداي && (all_مبوداي || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	إذا (show_مبوداي)
		أرجع بغ_مبوداي(format);

	إذا (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	إذا (all_مبوداي && argc)
		die(_("--all and <key> cannot be used together"));

	إذا (all_مبوداي)
		أرجع بغ_all_fields(repo, format);
	وإلا
		أرجع بغ_fields(argc, argv, repo, format);
}

بنية object_data {
	بنية object_id oid;
	size_t درج;
};

بنية largest_objects {
	بنية object_data tag_size;
	بنية object_data commit_size;
	بنية object_data بشيا_size;
	بنية object_data blob_size;

	بنية object_data parent_كدي;
	بنية object_data بشيا_entries;
};

بنية ref_stats {
	size_t branches;
	size_t remotes;
	size_t tags;
	size_t others;
};

بنية object_درجوجي {
	size_t tags;
	size_t commits;
	size_t trees;
	size_t blobs;
};

بنية object_stats {
	بنية object_درجوجي type_counts;
	بنية object_درجوجي inflated_sizes;
	بنية object_درجوجي disk_sizes;
	بنية largest_objects largest;
};

بنية repo_structure {
	بنية ref_stats refs;
	بنية object_stats objects;
};

بنية stats_table {
	بنية string_list rows;
	بنية string_list annotations;

	صحيح name_col_width;
	صحيح درج_col_width;
	صحيح unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
بنية stats_table_entry {
	حرف *درج;
	ثابت حرف *unit;
	size_t index;
	بنية object_id *oid;
};

ساكن فراغ stats_table_vaddf(بنية stats_table *table,
			      بنية stats_table_entry *entry,
			      ثابت حرف *format, va_list ap)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_item *item;
	حرف *formatted_name;
	صحيح name_width;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, عدم);
	name_width = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&table->rows, formatted_name);
	item->util = entry;

	إذا (name_width > table->name_col_width)
		table->name_col_width = name_width;
	إذا (!entry)
		أرجع;
	إذا (entry->oid) {
		entry->index = table->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)entry->index,
			    oid_to_hex(entry->oid));
		string_list_append_nodup(&table->annotations, strbuf_detach(&buf, عدم));
	}
	إذا (entry->درج) {
		صحيح درج_width = utf8_strwidth(entry->درج);
		إذا (درج_width > table->درج_col_width)
			table->درج_col_width = درج_width;
	}
	إذا (entry->unit) {
		صحيح unit_width = utf8_strwidth(entry->unit);
		إذا (unit_width > table->unit_col_width)
			table->unit_col_width = unit_width;
	}

	strbuf_release(&buf);
}

ساكن فراغ stats_table_addf(بنية stats_table *table, ثابت حرف *format, ...)
{
	va_list ap;

	va_start(ap, format);
	stats_table_vaddf(table, عدم, format, ap);
	va_end(ap);
}

ساكن فراغ stats_table_كدي_addf(بنية stats_table *table, size_t درج,
				   ثابت حرف *format, ...)
{
	بنية stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_كدي(درج, &entry->درج, &entry->unit);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ساكن فراغ stats_table_object_كدي_addf(بنية stats_table *table,
					  بنية object_id *oid, size_t درج,
					  ثابت حرف *format, ...)
{
	بنية stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_كدي(درج, &entry->درج, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	إذا (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ساكن فراغ stats_table_size_addf(بنية stats_table *table, size_t درج,
				  ثابت حرف *format, ...)
{
	بنية stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(درج, &entry->درج, &entry->unit, HUMANISE_COMPACT);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ساكن فراغ stats_table_object_size_addf(بنية stats_table *table,
					 بنية object_id *oid, size_t درج,
					 ثابت حرف *format, ...)
{
	بنية stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(درج, &entry->درج, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	إذا (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ساكن مضمن size_t get_total_reference_كدي(بنية ref_stats *stats)
{
	أرجع stats->branches + stats->remotes + stats->tags + stats->others;
}

ساكن مضمن size_t get_total_object_درجوجي(بنية object_درجوجي *درجوجي)
{
	أرجع درجوجي->tags + درجوجي->commits + درجوجي->trees + درجوجي->blobs;
}

ساكن فراغ stats_table_setup_structure(بنية stats_table *table,
					بنية repo_structure *stats)
{
	بنية object_stats *objects = &stats->objects;
	بنية ref_stats *refs = &stats->refs;
	size_t inflated_object_total;
	size_t object_كدي_total;
	size_t disk_object_total;
	size_t ref_total;

	ref_total = get_total_reference_كدي(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_كدي_addf(table, ref_total, "  * %s", _("Count"));
	stats_table_كدي_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_كدي_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_كدي_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_كدي_addf(table, refs->others, "    * %s", _("Others"));

	object_كدي_total = get_total_object_درجوجي(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_كدي_addf(table, object_كدي_total, "  * %s", _("Count"));
	stats_table_كدي_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_كدي_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_كدي_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_كدي_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_total = get_total_object_درجوجي(&objects->inflated_sizes);
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

	disk_object_total = get_total_object_درجوجي(&objects->disk_sizes);
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
				     objects->largest.commit_size.درج,
				     "    * %s", _("Maximum size"));
	stats_table_object_كدي_addf(table,
				      &objects->largest.parent_كدي.oid,
				      objects->largest.parent_كدي.درج,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_size_addf(table,
				     &objects->largest.بشيا_size.oid,
				     objects->largest.بشيا_size.درج,
				     "    * %s", _("Maximum size"));
	stats_table_object_كدي_addf(table,
				      &objects->largest.بشيا_entries.oid,
				      objects->largest.بشيا_entries.درج,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_size_addf(table,
				     &objects->largest.blob_size.oid,
				     objects->largest.blob_size.درج,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_size_addf(table,
				     &objects->largest.tag_size.oid,
				     objects->largest.tag_size.درج,
				     "    * %s", _("Maximum size"));
}

#define INDEX_WIDTH 4

ساكن فراغ stats_table_بغ_structure(ثابت بنية stats_table *table)
{
	ثابت حرف *name_col_title = _("Repository structure");
	ثابت حرف *درج_col_title = _("Value");
	صحيح title_name_width = utf8_strwidth(name_col_title);
	صحيح title_درج_width = utf8_strwidth(درج_col_title);
	صحيح name_col_width = table->name_col_width;
	صحيح درج_col_width = table->درج_col_width;
	صحيح unit_col_width = table->unit_col_width;
	بنية string_list_item *item;
	بنية strbuf buf = STRBUF_INIT;

	إذا (title_name_width > name_col_width)
		name_col_width = title_name_width;
	إذا (title_درج_width > درج_col_width + unit_col_width + 1)
		درج_col_width = title_درج_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_هغو, name_col_width + INDEX_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_هغو,
			  درج_col_width + unit_col_width + 1, درج_col_title);
	strbuf_addstr(&buf, " |");
	printf("%s\n", buf.buf);

	printf("| ");
	لكل (صحيح i = 0; i < name_col_width + INDEX_WIDTH; i++)
		putchar('-');
	printf(" | ");
	لكل (صحيح i = 0; i < درج_col_width + unit_col_width + 1; i++)
		putchar('-');
	printf(" |\n");

	for_each_string_list_item(item, &table->rows) {
		بنية stats_table_entry *entry = item->util;
		ثابت حرف *درج = "";
		ثابت حرف *unit = "";

		إذا (entry) {
			درج = entry->درج;
			إذا (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_هغو, name_col_width, item->string);

		إذا (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->index);
		وإلا
			strbuf_addchars(&buf, ' ', INDEX_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_دام, درج_col_width, درج);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_هغو, unit_col_width, unit);
		strbuf_addstr(&buf, " |");
		printf("%s\n", buf.buf);
	}

	إذا (table->annotations.nr) {
		printf("\n");
		for_each_string_list_item(item, &table->annotations)
			printf("%s\n", item->string);
	}

	strbuf_release(&buf);
}

ساكن فراغ stats_table_clear(بنية stats_table *table)
{
	بنية stats_table_entry *entry;
	بنية string_list_item *item;

	for_each_string_list_item(item, &table->rows) {
		entry = item->util;
		إذا (entry)
			حرر(entry->درج);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

ساكن مضمن فراغ بغ_keyvalue(ثابت حرف *مبودي, حرف مبودي_delim, size_t درج,
				  حرف درج_delim)
{
	printf("%s%c%" PRIuMAX "%c", مبودي, مبودي_delim, (uintmax_t)درج,
	       درج_delim);
}

ساكن فراغ بغ_object_data(ثابت حرف *مبودي, حرف مبودي_delim,
			      بنية object_data *data, حرف درج_delim)
{
	بغ_keyvalue(مبودي, مبودي_delim, data->درج, درج_delim);
	printf("%s_oid%c%s%c", مبودي, مبودي_delim, oid_to_hex(&data->oid),
	       درج_delim);
}

ساكن فراغ structure_keyvalue_بغ(بنية repo_structure *stats,
				     حرف مبودي_delim, حرف درج_delim)
{
	بغ_keyvalue("references.branches.count", مبودي_delim,
		       stats->refs.branches, درج_delim);
	بغ_keyvalue("references.tags.count", مبودي_delim,
		       stats->refs.tags, درج_delim);
	بغ_keyvalue("references.remotes.count", مبودي_delim,
		       stats->refs.remotes, درج_delim);
	بغ_keyvalue("references.others.count", مبودي_delim,
		       stats->refs.others, درج_delim);

	بغ_keyvalue("objects.commits.count", مبودي_delim,
		       stats->objects.type_counts.commits, درج_delim);
	بغ_keyvalue("objects.trees.count", مبودي_delim,
		       stats->objects.type_counts.trees, درج_delim);
	بغ_keyvalue("objects.blobs.count", مبودي_delim,
		       stats->objects.type_counts.blobs, درج_delim);
	بغ_keyvalue("objects.tags.count", مبودي_delim,
		       stats->objects.type_counts.tags, درج_delim);

	بغ_keyvalue("objects.commits.inflated_size", مبودي_delim,
		       stats->objects.inflated_sizes.commits, درج_delim);
	بغ_keyvalue("objects.trees.inflated_size", مبودي_delim,
		       stats->objects.inflated_sizes.trees, درج_delim);
	بغ_keyvalue("objects.blobs.inflated_size", مبودي_delim,
		       stats->objects.inflated_sizes.blobs, درج_delim);
	بغ_keyvalue("objects.tags.inflated_size", مبودي_delim,
		       stats->objects.inflated_sizes.tags, درج_delim);

	بغ_keyvalue("objects.commits.disk_size", مبودي_delim,
		       stats->objects.disk_sizes.commits, درج_delim);
	بغ_keyvalue("objects.trees.disk_size", مبودي_delim,
		       stats->objects.disk_sizes.trees, درج_delim);
	بغ_keyvalue("objects.blobs.disk_size", مبودي_delim,
		       stats->objects.disk_sizes.blobs, درج_delim);
	بغ_keyvalue("objects.tags.disk_size", مبودي_delim,
		       stats->objects.disk_sizes.tags, درج_delim);

	بغ_object_data("objects.commits.max_size", مبودي_delim,
			  &stats->objects.largest.commit_size, درج_delim);
	بغ_object_data("objects.trees.max_size", مبودي_delim,
			  &stats->objects.largest.بشيا_size, درج_delim);
	بغ_object_data("objects.blobs.max_size", مبودي_delim,
			  &stats->objects.largest.blob_size, درج_delim);
	بغ_object_data("objects.tags.max_size", مبودي_delim,
			  &stats->objects.largest.tag_size, درج_delim);

	بغ_object_data("objects.commits.max_parents", مبودي_delim,
			  &stats->objects.largest.parent_كدي, درج_delim);
	بغ_object_data("objects.trees.max_entries", مبودي_delim,
			  &stats->objects.largest.بشيا_entries, درج_delim);

	fflush(stdout);
}

بنية كدي_references_data {
	بنية ref_stats *stats;
	بنية rev_info *revs;
	بنية progress *progress;
};

ساكن صحيح كدي_references(ثابت بنية reference *ref, فراغ *cb_data)
{
	بنية كدي_references_data *data = cb_data;
	بنية ref_stats *stats = data->stats;
	size_t ref_كدي;

	بدل (ref_kind_from_refname(ref->name)) {
	حالة FILTER_REFS_BRANCHES:
		stats->branches++;
		اكسر;
	حالة FILTER_REFS_REMOTES:
		stats->remotes++;
		اكسر;
	حالة FILTER_REFS_TAGS:
		stats->tags++;
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
	add_pending_oid(data->revs, عدم, ref->oid, 0);

	ref_كدي = get_total_reference_كدي(stats);
	display_progress(data->progress, ref_كدي);

	أرجع 0;
}

ساكن فراغ structure_كدي_references(بنية ref_stats *stats,
				       بنية rev_info *revs,
				       بنية repository *repo,
				       صحيح show_progress)
{
	بنية كدي_references_data data = {
		.stats = stats,
		.revs = revs,
	};

	إذا (show_progress)
		data.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), كدي_references, &data);
	stop_progress(&data.progress);
}

بنية كدي_objects_data {
	بنية object_database *odb;
	بنية object_stats *stats;
	بنية progress *progress;
};

ساكن فراغ check_largest(بنية object_data *data, بنية object_id *oid,
			  size_t درج)
{
	إذا (درج > data->درج || is_null_oid(&data->oid)) {
		oidcpy(&data->oid, oid);
		data->درج = درج;
	}
}

ساكن size_t كدي_بشيا_entries(بنية object *obj)
{
	بنية بشيا *t = object_as_type(obj, OBJ_بشيا, 0);
	بنية name_entry entry;
	بنية بشيا_desc desc;
	size_t كدي = 0;

	init_بشيا_desc(&desc, &t->object.oid, t->buffer, t->size);
	طالما (بشيا_entry(&desc, &entry))
		كدي++;

	أرجع كدي;
}

ساكن صحيح كدي_objects(ثابت حرف *path UNUSED, بنية oid_array *oids,
			 تعداد object_type type, فراغ *cb_data)
{
	بنية كدي_objects_data *data = cb_data;
	بنية object_stats *stats = data->stats;
	size_t object_كدي;

	لكل (size_t i = 0; i < oids->nr; i++) {
		بنية object_info oi = OBJECT_INFO_INIT;
		unsigned long inflated;
		size_t inflated_st = 0;
		بنية commit *commit;
		بنية object *obj;
		فراغ *content;
		off_t disk;
		صحيح eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		إذا (odb_read_object_info_extended(data->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_QUICK) < 0)
			استمر;
		inflated = cast_size_t_to_ulong(inflated_st);

		obj = parse_object_buffer(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		بدل (type) {
		حالة OBJ_TAG:
			stats->type_counts.tags++;
			stats->inflated_sizes.tags += inflated;
			stats->disk_sizes.tags += disk;
			check_largest(&stats->largest.tag_size, &oids->oid[i],
				      inflated);
			اكسر;
		حالة OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_كدي, &oids->oid[i],
				      commit_list_كدي(commit->parents));
			اكسر;
		حالة OBJ_بشيا:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.بشيا_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.بشيا_entries, &oids->oid[i],
				      كدي_بشيا_entries(obj));
			اكسر;
		حالة OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_size, &oids->oid[i],
				      inflated);
			اكسر;
		افتراضي:
			BUG("invalid object type");
		}

		إذا (!eaten)
			حرر(content);
	}

	object_كدي = get_total_object_درجوجي(&stats->type_counts);
	display_progress(data->progress, object_كدي);

	أرجع 0;
}

ساكن فراغ structure_كدي_objects(بنية object_stats *stats,
				    بنية rev_info *revs,
				    بنية repository *repo, صحيح show_progress)
{
	بنية path_walk_info info = PATH_WALK_INFO_INIT;
	بنية كدي_objects_data data = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = كدي_objects;
	info.path_fn_data = &data;

	إذا (show_progress)
		data.progress = start_delayed_progress(repo, _("Counting objects"), 0);

	walk_objects_by_path(&info);
	path_walk_info_clear(&info);
	stop_progress(&data.progress);
}

ساكن صحيح cmd_repo_structure(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			      بنية repository *repo)
{
	بنية stats_table table = {
		.rows = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	تعداد output_format format = FORMAT_TABLE;
	بنية repo_structure stats = { 0 };
	بنية rev_info revs;
	صحيح show_progress = -1;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', عدم, &format, عدم,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_progress, N_("show progress")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_structure_usage, 0);
	إذا (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, prefix);

	إذا (show_progress < 0)
		show_progress = isatty(2);

	structure_كدي_references(&stats.refs, &revs, repo, show_progress);
	structure_كدي_objects(&stats.objects, &revs, repo, show_progress);

	بدل (format) {
	حالة FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_بغ_structure(&table);
		اكسر;
	حالة FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_بغ(&stats, '=', '\n');
		اكسر;
	حالة FORMAT_NUL_TERMINATED:
		structure_keyvalue_بغ(&stats, '\n', '\0');
		اكسر;
	افتراضي:
		BUG("invalid output format");
	}

	stats_table_clear(&table);
	release_revisions(&revs);

	أرجع 0;
}

صحيح cmd_repo(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
	     بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_usage, 0);

	أرجع fn(argc, argv, prefix, repo);
}
