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

تعريفنوع صحيح get_قىممەت_fn(بنية repository *repo, بنية strbuf *buf);

تعداد output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

بنية repo_info_field {
	ثابت حرف *ئاچقۇچ;
	get_قىممەت_fn *get_قىممەت;
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

	أرجع strcmp(a->ئاچقۇچ, b->ئاچقۇچ);
}

ساكن ثابت بنية repo_info_field *get_repo_info_field(ثابت حرف *ئاچقۇچ)
{
	ثابت بنية repo_info_field ئىزدەش_ئاچقۇچ = { ئاچقۇچ, عدم };
	ثابت بنية repo_info_field *found = bsearch(&ئىزدەش_ئاچقۇچ,
						      repo_info_field,
						      ARRAY_SIZE(repo_info_field),
						      حجم(*found),
						      repo_info_field_cmp);

	أرجع found;
}

ساكن فراغ print_field(تعداد output_format format, ثابت حرف *ئاچقۇچ,
			ثابت حرف *قىممەت)
{
	بدل (format) {
	حالة FORMAT_NEWLINE_TERMINATED:
		printf("%s=", ئاچقۇچ);
		quote_c_style(قىممەت, عدم, stdout, 0);
		putchar('\n');
		اكسر;
	حالة FORMAT_NUL_TERMINATED:
		printf("%s\n%s%c", ئاچقۇچ, قىممەت, '\0');
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
		ثابت حرف *ئاچقۇچ = argv[i];
		ثابت بنية repo_info_field *field = get_repo_info_field(ئاچقۇچ);

		إذا (!field) {
			ret = error(_("key '%s' not found"), ئاچقۇچ);
			استمر;
		}

		strbuf_reset(&valbuf);
		field->get_قىممەت(repo, &valbuf);
		print_field(format, ئاچقۇچ, valbuf.buf);
	}

	strbuf_release(&valbuf);
	أرجع ret;
}

ساكن صحيح print_all_fields(بنية repository *repo,
			    تعداد output_format format)
{
	بنية strbuf valbuf = STRBUF_INIT;

	لكل (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		ثابت بنية repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_قىممەت(repo, &valbuf);
		print_field(format, field->ئاچقۇچ, valbuf.buf);
	}

	strbuf_release(&valbuf);
	أرجع 0;
}

ساكن صحيح print_ئاچقۇچلار(تعداد output_format format)
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
		printf("%s%c", field->ئاچقۇچ, sep);
	}

	أرجع 0;
}

ساكن صحيح parse_format_cb(ثابت بنية option *opt,
			   ثابت حرف *arg, صحيح unset UNUSED)
{
	تعداد output_format *format = opt->قىممەت;

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
	صحيح all_ئاچقۇچلار = 0;
	صحيح show_ئاچقۇچلار = 0;
	بنية option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', عدم, &format, عدم,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_ئاچقۇچلار, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_ئاچقۇچلار, N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	إذا (show_ئاچقۇچلار && (all_ئاچقۇچلار || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	إذا (show_ئاچقۇچلار)
		أرجع print_ئاچقۇچلار(format);

	إذا (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	إذا (all_ئاچقۇچلار && argc)
		die(_("--all and <key> cannot be used together"));

	إذا (all_ئاچقۇچلار)
		أرجع print_all_fields(repo, format);
	وإلا
		أرجع print_fields(argc, argv, repo, format);
}

بنية object_data {
	بنية object_id oid;
	size_t قىممەت;
};

بنية largest_objects {
	بنية object_data tag_size;
	بنية object_data commit_size;
	بنية object_data دەرەخ_size;
	بنية object_data blob_size;

	بنية object_data parent_سانى;
	بنية object_data دەرەخ_entries;
};

بنية ref_stats {
	size_t branches;
	size_t remotes;
	size_t tags;
	size_t others;
};

بنية object_قىممەتلەر {
	size_t tags;
	size_t commits;
	size_t trees;
	size_t blobs;
};

بنية object_stats {
	بنية object_قىممەتلەر type_counts;
	بنية object_قىممەتلەر inflated_sizes;
	بنية object_قىممەتلەر disk_sizes;
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
	صحيح قىممەت_col_width;
	صحيح unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
بنية stats_table_entry {
	حرف *قىممەت;
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
	إذا (entry->قىممەت) {
		صحيح قىممەت_width = utf8_strwidth(entry->قىممەت);
		إذا (قىممەت_width > table->قىممەت_col_width)
			table->قىممەت_col_width = قىممەت_width;
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

ساكن فراغ stats_table_سانى_addf(بنية stats_table *table, size_t قىممەت,
				   ثابت حرف *format, ...)
{
	بنية stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_سانى(قىممەت, &entry->قىممەت, &entry->unit);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ساكن فراغ stats_table_object_سانى_addf(بنية stats_table *table,
					  بنية object_id *oid, size_t قىممەت,
					  ثابت حرف *format, ...)
{
	بنية stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_سانى(قىممەت, &entry->قىممەت, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	إذا (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ساكن فراغ stats_table_size_addf(بنية stats_table *table, size_t قىممەت,
				  ثابت حرف *format, ...)
{
	بنية stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(قىممەت, &entry->قىممەت, &entry->unit, HUMANISE_COMPACT);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ساكن فراغ stats_table_object_size_addf(بنية stats_table *table,
					 بنية object_id *oid, size_t قىممەت,
					 ثابت حرف *format, ...)
{
	بنية stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(قىممەت, &entry->قىممەت, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	إذا (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ساكن مضمن size_t get_total_reference_سانى(بنية ref_stats *stats)
{
	أرجع stats->branches + stats->remotes + stats->tags + stats->others;
}

ساكن مضمن size_t get_total_object_قىممەتلەر(بنية object_قىممەتلەر *قىممەتلەر)
{
	أرجع قىممەتلەر->tags + قىممەتلەر->commits + قىممەتلەر->trees + قىممەتلەر->blobs;
}

ساكن فراغ stats_table_setup_structure(بنية stats_table *table,
					بنية repo_structure *stats)
{
	بنية object_stats *objects = &stats->objects;
	بنية ref_stats *refs = &stats->refs;
	size_t inflated_object_total;
	size_t object_سانى_total;
	size_t disk_object_total;
	size_t ref_total;

	ref_total = get_total_reference_سانى(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_سانى_addf(table, ref_total, "  * %s", _("Count"));
	stats_table_سانى_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_سانى_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_سانى_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_سانى_addf(table, refs->others, "    * %s", _("Others"));

	object_سانى_total = get_total_object_قىممەتلەر(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_سانى_addf(table, object_سانى_total, "  * %s", _("Count"));
	stats_table_سانى_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_سانى_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_سانى_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_سانى_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_total = get_total_object_قىممەتلەر(&objects->inflated_sizes);
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

	disk_object_total = get_total_object_قىممەتلەر(&objects->disk_sizes);
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
				     objects->largest.commit_size.قىممەت,
				     "    * %s", _("Maximum size"));
	stats_table_object_سانى_addf(table,
				      &objects->largest.parent_سانى.oid,
				      objects->largest.parent_سانى.قىممەت,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_size_addf(table,
				     &objects->largest.دەرەخ_size.oid,
				     objects->largest.دەرەخ_size.قىممەت,
				     "    * %s", _("Maximum size"));
	stats_table_object_سانى_addf(table,
				      &objects->largest.دەرەخ_entries.oid,
				      objects->largest.دەرەخ_entries.قىممەت,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_size_addf(table,
				     &objects->largest.blob_size.oid,
				     objects->largest.blob_size.قىممەت,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_size_addf(table,
				     &objects->largest.tag_size.oid,
				     objects->largest.tag_size.قىممەت,
				     "    * %s", _("Maximum size"));
}

#define INDEX_WIDTH 4

ساكن فراغ stats_table_print_structure(ثابت بنية stats_table *table)
{
	ثابت حرف *name_col_title = _("Repository structure");
	ثابت حرف *قىممەت_col_title = _("Value");
	صحيح title_name_width = utf8_strwidth(name_col_title);
	صحيح title_قىممەت_width = utf8_strwidth(قىممەت_col_title);
	صحيح name_col_width = table->name_col_width;
	صحيح قىممەت_col_width = table->قىممەت_col_width;
	صحيح unit_col_width = table->unit_col_width;
	بنية string_list_item *item;
	بنية strbuf buf = STRBUF_INIT;

	إذا (title_name_width > name_col_width)
		name_col_width = title_name_width;
	إذا (title_قىممەت_width > قىممەت_col_width + unit_col_width + 1)
		قىممەت_col_width = title_قىممەت_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_سول, name_col_width + INDEX_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_سول,
			  قىممەت_col_width + unit_col_width + 1, قىممەت_col_title);
	strbuf_addstr(&buf, " |");
	printf("%s\n", buf.buf);

	printf("| ");
	لكل (صحيح i = 0; i < name_col_width + INDEX_WIDTH; i++)
		putchar('-');
	printf(" | ");
	لكل (صحيح i = 0; i < قىممەت_col_width + unit_col_width + 1; i++)
		putchar('-');
	printf(" |\n");

	for_each_string_list_item(item, &table->rows) {
		بنية stats_table_entry *entry = item->util;
		ثابت حرف *قىممەت = "";
		ثابت حرف *unit = "";

		إذا (entry) {
			قىممەت = entry->قىممەت;
			إذا (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_سول, name_col_width, item->string);

		إذا (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->index);
		وإلا
			strbuf_addchars(&buf, ' ', INDEX_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_ئوڭ, قىممەت_col_width, قىممەت);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_سول, unit_col_width, unit);
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
			حرر(entry->قىممەت);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

ساكن مضمن فراغ print_keyvalue(ثابت حرف *ئاچقۇچ, حرف ئاچقۇچ_delim, size_t قىممەت,
				  حرف قىممەت_delim)
{
	printf("%s%c%" PRIuMAX "%c", ئاچقۇچ, ئاچقۇچ_delim, (uintmax_t)قىممەت,
	       قىممەت_delim);
}

ساكن فراغ print_object_data(ثابت حرف *ئاچقۇچ, حرف ئاچقۇچ_delim,
			      بنية object_data *data, حرف قىممەت_delim)
{
	print_keyvalue(ئاچقۇچ, ئاچقۇچ_delim, data->قىممەت, قىممەت_delim);
	printf("%s_oid%c%s%c", ئاچقۇچ, ئاچقۇچ_delim, oid_to_hex(&data->oid),
	       قىممەت_delim);
}

ساكن فراغ structure_keyvalue_print(بنية repo_structure *stats,
				     حرف ئاچقۇچ_delim, حرف قىممەت_delim)
{
	print_keyvalue("references.branches.count", ئاچقۇچ_delim,
		       stats->refs.branches, قىممەت_delim);
	print_keyvalue("references.tags.count", ئاچقۇچ_delim,
		       stats->refs.tags, قىممەت_delim);
	print_keyvalue("references.remotes.count", ئاچقۇچ_delim,
		       stats->refs.remotes, قىممەت_delim);
	print_keyvalue("references.others.count", ئاچقۇچ_delim,
		       stats->refs.others, قىممەت_delim);

	print_keyvalue("objects.commits.count", ئاچقۇچ_delim,
		       stats->objects.type_counts.commits, قىممەت_delim);
	print_keyvalue("objects.trees.count", ئاچقۇچ_delim,
		       stats->objects.type_counts.trees, قىممەت_delim);
	print_keyvalue("objects.blobs.count", ئاچقۇچ_delim,
		       stats->objects.type_counts.blobs, قىممەت_delim);
	print_keyvalue("objects.tags.count", ئاچقۇچ_delim,
		       stats->objects.type_counts.tags, قىممەت_delim);

	print_keyvalue("objects.commits.inflated_size", ئاچقۇچ_delim,
		       stats->objects.inflated_sizes.commits, قىممەت_delim);
	print_keyvalue("objects.trees.inflated_size", ئاچقۇچ_delim,
		       stats->objects.inflated_sizes.trees, قىممەت_delim);
	print_keyvalue("objects.blobs.inflated_size", ئاچقۇچ_delim,
		       stats->objects.inflated_sizes.blobs, قىممەت_delim);
	print_keyvalue("objects.tags.inflated_size", ئاچقۇچ_delim,
		       stats->objects.inflated_sizes.tags, قىممەت_delim);

	print_keyvalue("objects.commits.disk_size", ئاچقۇچ_delim,
		       stats->objects.disk_sizes.commits, قىممەت_delim);
	print_keyvalue("objects.trees.disk_size", ئاچقۇچ_delim,
		       stats->objects.disk_sizes.trees, قىممەت_delim);
	print_keyvalue("objects.blobs.disk_size", ئاچقۇچ_delim,
		       stats->objects.disk_sizes.blobs, قىممەت_delim);
	print_keyvalue("objects.tags.disk_size", ئاچقۇچ_delim,
		       stats->objects.disk_sizes.tags, قىممەت_delim);

	print_object_data("objects.commits.max_size", ئاچقۇچ_delim,
			  &stats->objects.largest.commit_size, قىممەت_delim);
	print_object_data("objects.trees.max_size", ئاچقۇچ_delim,
			  &stats->objects.largest.دەرەخ_size, قىممەت_delim);
	print_object_data("objects.blobs.max_size", ئاچقۇچ_delim,
			  &stats->objects.largest.blob_size, قىممەت_delim);
	print_object_data("objects.tags.max_size", ئاچقۇچ_delim,
			  &stats->objects.largest.tag_size, قىممەت_delim);

	print_object_data("objects.commits.max_parents", ئاچقۇچ_delim,
			  &stats->objects.largest.parent_سانى, قىممەت_delim);
	print_object_data("objects.trees.max_entries", ئاچقۇچ_delim,
			  &stats->objects.largest.دەرەخ_entries, قىممەت_delim);

	fflush(stdout);
}

بنية سانى_references_data {
	بنية ref_stats *stats;
	بنية rev_info *revs;
	بنية progress *progress;
};

ساكن صحيح سانى_references(ثابت بنية reference *ref, فراغ *cb_data)
{
	بنية سانى_references_data *data = cb_data;
	بنية ref_stats *stats = data->stats;
	size_t ref_سانى;

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

	ref_سانى = get_total_reference_سانى(stats);
	display_progress(data->progress, ref_سانى);

	أرجع 0;
}

ساكن فراغ structure_سانى_references(بنية ref_stats *stats,
				       بنية rev_info *revs,
				       بنية repository *repo,
				       صحيح show_progress)
{
	بنية سانى_references_data data = {
		.stats = stats,
		.revs = revs,
	};

	إذا (show_progress)
		data.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), سانى_references, &data);
	stop_progress(&data.progress);
}

بنية سانى_objects_data {
	بنية object_database *odb;
	بنية object_stats *stats;
	بنية progress *progress;
};

ساكن فراغ check_largest(بنية object_data *data, بنية object_id *oid,
			  size_t قىممەت)
{
	إذا (قىممەت > data->قىممەت || is_null_oid(&data->oid)) {
		oidcpy(&data->oid, oid);
		data->قىممەت = قىممەت;
	}
}

ساكن size_t سانى_دەرەخ_entries(بنية object *obj)
{
	بنية دەرەخ *t = object_as_type(obj, OBJ_دەرەخ, 0);
	بنية name_entry entry;
	بنية دەرەخ_desc desc;
	size_t سانى = 0;

	init_دەرەخ_desc(&desc, &t->object.oid, t->buffer, t->size);
	طالما (دەرەخ_entry(&desc, &entry))
		سانى++;

	أرجع سانى;
}

ساكن صحيح سانى_objects(ثابت حرف *path UNUSED, بنية oid_array *oids,
			 تعداد object_type type, فراغ *cb_data)
{
	بنية سانى_objects_data *data = cb_data;
	بنية object_stats *stats = data->stats;
	size_t object_سانى;

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
			check_largest(&stats->largest.parent_سانى, &oids->oid[i],
				      commit_list_سانى(commit->parents));
			اكسر;
		حالة OBJ_دەرەخ:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.دەرەخ_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.دەرەخ_entries, &oids->oid[i],
				      سانى_دەرەخ_entries(obj));
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

	object_سانى = get_total_object_قىممەتلەر(&stats->type_counts);
	display_progress(data->progress, object_سانى);

	أرجع 0;
}

ساكن فراغ structure_سانى_objects(بنية object_stats *stats,
				    بنية rev_info *revs,
				    بنية repository *repo, صحيح show_progress)
{
	بنية path_walk_info info = PATH_WALK_INFO_INIT;
	بنية سانى_objects_data data = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = سانى_objects;
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

	structure_سانى_references(&stats.refs, &revs, repo, show_progress);
	structure_سانى_objects(&stats.objects, &revs, repo, show_progress);

	بدل (format) {
	حالة FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_print_structure(&table);
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
