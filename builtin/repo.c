#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "commit.h"
#include "environment.h"
#include "hash.h"
#include "hex.h"
#include "odb.h"
#include "parse-options.h"
#include "path.h"
#include "path-walk.h"
#include "progress.h"
#include "quote.h"
#include "ref-filter.h"
#include "refs.h"
#include "revision.h"
#include "setup.h"
#include "strbuf.h"
#include "string-list.h"
#include "shallow.h"
#include "tree.h"
#include "tree-walk.h"
#include "utf8.h"

#define REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#define REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

static const var_na *const repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	NULL,
};

static const var_na *const repo_info_usage[] = {
	REPO_INFO_USAGE,
	NULL,
};

static const var_na *const repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	NULL,
};

typedef puur_naa_nka get_value_fn(sa_nracanaa repository *repo, sa_nracanaa strbuf *buf);

ga_nanaa output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

sa_nracanaa repo_info_field {
	const var_na *key;
	get_value_fn *get_value;
};

static puur_naa_nka get_layout_bare(sa_nracanaa repository *repo UNUSED, sa_nracanaa strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka get_layout_shallow(sa_nracanaa repository *repo, sa_nracanaa strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka get_object_format(sa_nracanaa repository *repo, sa_nracanaa strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka get_path_commondir_absolute(sa_nracanaa repository *repo, sa_nracanaa strbuf *buf)
{
	const var_na *common_dir = repo_get_common_dir(repo);

	ᱡᱩᱫᱤ (!common_dir)
		ᱨᱩᱣᱟᱹᱲ error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka get_path_commondir_relative(sa_nracanaa repository *repo, sa_nracanaa strbuf *buf)
{
	const var_na *common_dir = repo_get_common_dir(repo);

	ᱡᱩᱫᱤ (!common_dir)
		ᱨᱩᱣᱟᱹᱲ error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka get_path_gitdir_absolute(sa_nracanaa repository *repo, sa_nracanaa strbuf *buf)
{
	const var_na *git_dir = repo_get_git_dir(repo);

	ᱡᱩᱫᱤ (!git_dir)
		ᱨᱩᱣᱟᱹᱲ error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka get_path_gitdir_relative(sa_nracanaa repository *repo, sa_nracanaa strbuf *buf)
{
	const var_na *git_dir = repo_get_git_dir(repo);

	ᱡᱩᱫᱤ (!git_dir)
		ᱨᱩᱣᱟᱹᱲ error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka get_references_format(sa_nracanaa repository *repo, sa_nracanaa strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_to_name(repo->ref_storage_format));
	ᱨᱩᱣᱟᱹᱲ 0;
}

/* repo_info_field keys must be in lexicographical order */
static const sa_nracanaa repo_info_field repo_info_field[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_path_commondir_absolute },
	{ "path.commondir.relative", get_path_commondir_relative },
	{ "path.gitdir.absolute", get_path_gitdir_absolute },
	{ "path.gitdir.relative", get_path_gitdir_relative },
	{ "references.format", get_references_format },
};

static puur_naa_nka repo_info_field_cmp(const void *va, const void *vb)
{
	const sa_nracanaa repo_info_field *a = va;
	const sa_nracanaa repo_info_field *b = vb;

	ᱨᱩᱣᱟᱹᱲ strcmp(a->key, b->key);
}

static const sa_nracanaa repo_info_field *get_repo_info_field(const var_na *key)
{
	const sa_nracanaa repo_info_field search_key = { key, NULL };
	const sa_nracanaa repo_info_field *found = bsearch(&search_key,
						      repo_info_field,
						      ARRAY_SIZE(repo_info_field),
						      parimaa_na(*found),
						      repo_info_field_cmp);

	ᱨᱩᱣᱟᱹᱲ found;
}

static void print_field(ga_nanaa output_format format, const var_na *key,
			const var_na *value)
{
	switch (format) {
	pak_sa FORMAT_NEWLINE_TERMINATED:
		printf("%s=", key);
		quote_c_style(value, NULL, stdout, 0);
		putchar('\n');
		bha_nga;
	pak_sa FORMAT_NUL_TERMINATED:
		printf("%s\n%s%c", key, value, '\0');
		bha_nga;
	default:
		BUG("not a valid output format: %d", format);
	}
}

static puur_naa_nka print_fields(puur_naa_nka argc, const var_na **argv,
			sa_nracanaa repository *repo,
			ga_nanaa output_format format)
{
	puur_naa_nka ret = 0;
	sa_nracanaa strbuf valbuf = STRBUF_INIT;

	for (puur_naa_nka i = 0; i < argc; i++) {
		const var_na *key = argv[i];
		const sa_nracanaa repo_info_field *field = get_repo_info_field(key);

		ᱡᱩᱫᱤ (!field) {
			ret = error(_("key '%s' not found"), key);
			continue;
		}

		strbuf_reset(&valbuf);
		field->get_value(repo, &valbuf);
		print_field(format, key, valbuf.buf);
	}

	strbuf_release(&valbuf);
	ᱨᱩᱣᱟᱹᱲ ret;
}

static puur_naa_nka print_all_fields(sa_nracanaa repository *repo,
			    ga_nanaa output_format format)
{
	sa_nracanaa strbuf valbuf = STRBUF_INIT;

	for (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		const sa_nracanaa repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_value(repo, &valbuf);
		print_field(format, field->key, valbuf.buf);
	}

	strbuf_release(&valbuf);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka print_keys(ga_nanaa output_format format)
{
	var_na sep;

	switch (format) {
	pak_sa FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		bha_nga;
	pak_sa FORMAT_NUL_TERMINATED:
		sep = '\0';
		bha_nga;
	default:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	for (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		const sa_nracanaa repo_info_field *field = &repo_info_field[i];
		printf("%s%c", field->key, sep);
	}

	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka parse_format_cb(const sa_nracanaa option *opt,
			   const var_na *arg, puur_naa_nka unset UNUSED)
{
	ga_nanaa output_format *format = opt->value;

	ᱡᱩᱫᱤ (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "table"))
		*format = FORMAT_TABLE;
	ᱵᱟᱝ
		die(_("invalid format '%s'"), arg);

	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka cmd_repo_info(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			 sa_nracanaa repository *repo)
{
	ga_nanaa output_format format = FORMAT_NEWLINE_TERMINATED;
	puur_naa_nka all_keys = 0;
	puur_naa_nka show_keys = 0;
	sa_nracanaa option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', NULL, &format, NULL,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_keys, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_keys, N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	ᱡᱩᱫᱤ (show_keys && (all_keys || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	ᱡᱩᱫᱤ (show_keys)
		ᱨᱩᱣᱟᱹᱲ print_keys(format);

	ᱡᱩᱫᱤ (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	ᱡᱩᱫᱤ (all_keys && argc)
		die(_("--all and <key> cannot be used together"));

	ᱡᱩᱫᱤ (all_keys)
		ᱨᱩᱣᱟᱹᱲ print_all_fields(repo, format);
	ᱵᱟᱝ
		ᱨᱩᱣᱟᱹᱲ print_fields(argc, argv, repo, format);
}

sa_nracanaa object_data {
	sa_nracanaa object_id oid;
	size_t value;
};

sa_nracanaa largest_objects {
	sa_nracanaa object_data tag_size;
	sa_nracanaa object_data commit_size;
	sa_nracanaa object_data tree_size;
	sa_nracanaa object_data blob_size;

	sa_nracanaa object_data parent_count;
	sa_nracanaa object_data tree_entries;
};

sa_nracanaa ref_stats {
	size_t branches;
	size_t remotes;
	size_t tags;
	size_t others;
};

sa_nracanaa object_values {
	size_t tags;
	size_t commits;
	size_t trees;
	size_t blobs;
};

sa_nracanaa object_stats {
	sa_nracanaa object_values type_counts;
	sa_nracanaa object_values inflated_sizes;
	sa_nracanaa object_values disk_sizes;
	sa_nracanaa largest_objects largest;
};

sa_nracanaa repo_structure {
	sa_nracanaa ref_stats refs;
	sa_nracanaa object_stats objects;
};

sa_nracanaa stats_table {
	sa_nracanaa string_list rows;
	sa_nracanaa string_list annotations;

	puur_naa_nka name_col_width;
	puur_naa_nka value_col_width;
	puur_naa_nka unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
sa_nracanaa stats_table_entry {
	var_na *value;
	const var_na *unit;
	size_t index;
	sa_nracanaa object_id *oid;
};

static void stats_table_vaddf(sa_nracanaa stats_table *table,
			      sa_nracanaa stats_table_entry *entry,
			      const var_na *format, va_list ap)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa string_list_item *item;
	var_na *formatted_name;
	puur_naa_nka name_width;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, NULL);
	name_width = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&table->rows, formatted_name);
	item->util = entry;

	ᱡᱩᱫᱤ (name_width > table->name_col_width)
		table->name_col_width = name_width;
	ᱡᱩᱫᱤ (!entry)
		ᱨᱩᱣᱟᱹᱲ;
	ᱡᱩᱫᱤ (entry->oid) {
		entry->index = table->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)entry->index,
			    oid_to_hex(entry->oid));
		string_list_append_nodup(&table->annotations, strbuf_detach(&buf, NULL));
	}
	ᱡᱩᱫᱤ (entry->value) {
		puur_naa_nka value_width = utf8_strwidth(entry->value);
		ᱡᱩᱫᱤ (value_width > table->value_col_width)
			table->value_col_width = value_width;
	}
	ᱡᱩᱫᱤ (entry->unit) {
		puur_naa_nka unit_width = utf8_strwidth(entry->unit);
		ᱡᱩᱫᱤ (unit_width > table->unit_col_width)
			table->unit_col_width = unit_width;
	}

	strbuf_release(&buf);
}

static void stats_table_addf(sa_nracanaa stats_table *table, const var_na *format, ...)
{
	va_list ap;

	va_start(ap, format);
	stats_table_vaddf(table, NULL, format, ap);
	va_end(ap);
}

static void stats_table_count_addf(sa_nracanaa stats_table *table, size_t value,
				   const var_na *format, ...)
{
	sa_nracanaa stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_count(value, &entry->value, &entry->unit);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

static void stats_table_object_count_addf(sa_nracanaa stats_table *table,
					  sa_nracanaa object_id *oid, size_t value,
					  const var_na *format, ...)
{
	sa_nracanaa stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_count(value, &entry->value, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	ᱡᱩᱫᱤ (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

static void stats_table_size_addf(sa_nracanaa stats_table *table, size_t value,
				  const var_na *format, ...)
{
	sa_nracanaa stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(value, &entry->value, &entry->unit, HUMANISE_COMPACT);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

static void stats_table_object_size_addf(sa_nracanaa stats_table *table,
					 sa_nracanaa object_id *oid, size_t value,
					 const var_na *format, ...)
{
	sa_nracanaa stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(value, &entry->value, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	ᱡᱩᱫᱤ (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

static anta_hstha size_t get_total_reference_count(sa_nracanaa ref_stats *stats)
{
	ᱨᱩᱣᱟᱹᱲ stats->branches + stats->remotes + stats->tags + stats->others;
}

static anta_hstha size_t get_total_object_values(sa_nracanaa object_values *values)
{
	ᱨᱩᱣᱟᱹᱲ values->tags + values->commits + values->trees + values->blobs;
}

static void stats_table_setup_structure(sa_nracanaa stats_table *table,
					sa_nracanaa repo_structure *stats)
{
	sa_nracanaa object_stats *objects = &stats->objects;
	sa_nracanaa ref_stats *refs = &stats->refs;
	size_t inflated_object_total;
	size_t object_count_total;
	size_t disk_object_total;
	size_t ref_total;

	ref_total = get_total_reference_count(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_count_addf(table, ref_total, "  * %s", _("Count"));
	stats_table_count_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_count_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_count_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_count_addf(table, refs->others, "    * %s", _("Others"));

	object_count_total = get_total_object_values(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_count_addf(table, object_count_total, "  * %s", _("Count"));
	stats_table_count_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_count_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_count_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_count_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_total = get_total_object_values(&objects->inflated_sizes);
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

	disk_object_total = get_total_object_values(&objects->disk_sizes);
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
				     objects->largest.commit_size.value,
				     "    * %s", _("Maximum size"));
	stats_table_object_count_addf(table,
				      &objects->largest.parent_count.oid,
				      objects->largest.parent_count.value,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_size_addf(table,
				     &objects->largest.tree_size.oid,
				     objects->largest.tree_size.value,
				     "    * %s", _("Maximum size"));
	stats_table_object_count_addf(table,
				      &objects->largest.tree_entries.oid,
				      objects->largest.tree_entries.value,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_size_addf(table,
				     &objects->largest.blob_size.oid,
				     objects->largest.blob_size.value,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_size_addf(table,
				     &objects->largest.tag_size.oid,
				     objects->largest.tag_size.value,
				     "    * %s", _("Maximum size"));
}

#define INDEX_WIDTH 4

static void stats_table_print_structure(const sa_nracanaa stats_table *table)
{
	const var_na *name_col_title = _("Repository structure");
	const var_na *value_col_title = _("Value");
	puur_naa_nka title_name_width = utf8_strwidth(name_col_title);
	puur_naa_nka title_value_width = utf8_strwidth(value_col_title);
	puur_naa_nka name_col_width = table->name_col_width;
	puur_naa_nka value_col_width = table->value_col_width;
	puur_naa_nka unit_col_width = table->unit_col_width;
	sa_nracanaa string_list_item *item;
	sa_nracanaa strbuf buf = STRBUF_INIT;

	ᱡᱩᱫᱤ (title_name_width > name_col_width)
		name_col_width = title_name_width;
	ᱡᱩᱫᱤ (title_value_width > value_col_width + unit_col_width + 1)
		value_col_width = title_value_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_LEFT, name_col_width + INDEX_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_LEFT,
			  value_col_width + unit_col_width + 1, value_col_title);
	strbuf_addstr(&buf, " |");
	printf("%s\n", buf.buf);

	printf("| ");
	for (puur_naa_nka i = 0; i < name_col_width + INDEX_WIDTH; i++)
		putchar('-');
	printf(" | ");
	for (puur_naa_nka i = 0; i < value_col_width + unit_col_width + 1; i++)
		putchar('-');
	printf(" |\n");

	for_each_string_list_item(item, &table->rows) {
		sa_nracanaa stats_table_entry *entry = item->util;
		const var_na *value = "";
		const var_na *unit = "";

		ᱡᱩᱫᱤ (entry) {
			value = entry->value;
			ᱡᱩᱫᱤ (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_LEFT, name_col_width, item->string);

		ᱡᱩᱫᱤ (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->index);
		ᱵᱟᱝ
			strbuf_addchars(&buf, ' ', INDEX_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_RIGHT, value_col_width, value);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_LEFT, unit_col_width, unit);
		strbuf_addstr(&buf, " |");
		printf("%s\n", buf.buf);
	}

	ᱡᱩᱫᱤ (table->annotations.nr) {
		printf("\n");
		for_each_string_list_item(item, &table->annotations)
			printf("%s\n", item->string);
	}

	strbuf_release(&buf);
}

static void stats_table_clear(sa_nracanaa stats_table *table)
{
	sa_nracanaa stats_table_entry *entry;
	sa_nracanaa string_list_item *item;

	for_each_string_list_item(item, &table->rows) {
		entry = item->util;
		ᱡᱩᱫᱤ (entry)
			free(entry->value);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

static anta_hstha void print_keyvalue(const var_na *key, var_na key_delim, size_t value,
				  var_na value_delim)
{
	printf("%s%c%" PRIuMAX "%c", key, key_delim, (uintmax_t)value,
	       value_delim);
}

static void print_object_data(const var_na *key, var_na key_delim,
			      sa_nracanaa object_data *data, var_na value_delim)
{
	print_keyvalue(key, key_delim, data->value, value_delim);
	printf("%s_oid%c%s%c", key, key_delim, oid_to_hex(&data->oid),
	       value_delim);
}

static void structure_keyvalue_print(sa_nracanaa repo_structure *stats,
				     var_na key_delim, var_na value_delim)
{
	print_keyvalue("references.branches.count", key_delim,
		       stats->refs.branches, value_delim);
	print_keyvalue("references.tags.count", key_delim,
		       stats->refs.tags, value_delim);
	print_keyvalue("references.remotes.count", key_delim,
		       stats->refs.remotes, value_delim);
	print_keyvalue("references.others.count", key_delim,
		       stats->refs.others, value_delim);

	print_keyvalue("objects.commits.count", key_delim,
		       stats->objects.type_counts.commits, value_delim);
	print_keyvalue("objects.trees.count", key_delim,
		       stats->objects.type_counts.trees, value_delim);
	print_keyvalue("objects.blobs.count", key_delim,
		       stats->objects.type_counts.blobs, value_delim);
	print_keyvalue("objects.tags.count", key_delim,
		       stats->objects.type_counts.tags, value_delim);

	print_keyvalue("objects.commits.inflated_size", key_delim,
		       stats->objects.inflated_sizes.commits, value_delim);
	print_keyvalue("objects.trees.inflated_size", key_delim,
		       stats->objects.inflated_sizes.trees, value_delim);
	print_keyvalue("objects.blobs.inflated_size", key_delim,
		       stats->objects.inflated_sizes.blobs, value_delim);
	print_keyvalue("objects.tags.inflated_size", key_delim,
		       stats->objects.inflated_sizes.tags, value_delim);

	print_keyvalue("objects.commits.disk_size", key_delim,
		       stats->objects.disk_sizes.commits, value_delim);
	print_keyvalue("objects.trees.disk_size", key_delim,
		       stats->objects.disk_sizes.trees, value_delim);
	print_keyvalue("objects.blobs.disk_size", key_delim,
		       stats->objects.disk_sizes.blobs, value_delim);
	print_keyvalue("objects.tags.disk_size", key_delim,
		       stats->objects.disk_sizes.tags, value_delim);

	print_object_data("objects.commits.max_size", key_delim,
			  &stats->objects.largest.commit_size, value_delim);
	print_object_data("objects.trees.max_size", key_delim,
			  &stats->objects.largest.tree_size, value_delim);
	print_object_data("objects.blobs.max_size", key_delim,
			  &stats->objects.largest.blob_size, value_delim);
	print_object_data("objects.tags.max_size", key_delim,
			  &stats->objects.largest.tag_size, value_delim);

	print_object_data("objects.commits.max_parents", key_delim,
			  &stats->objects.largest.parent_count, value_delim);
	print_object_data("objects.trees.max_entries", key_delim,
			  &stats->objects.largest.tree_entries, value_delim);

	fflush(stdout);
}

sa_nracanaa count_references_data {
	sa_nracanaa ref_stats *stats;
	sa_nracanaa rev_info *revs;
	sa_nracanaa progress *progress;
};

static puur_naa_nka count_references(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa count_references_data *data = cb_data;
	sa_nracanaa ref_stats *stats = data->stats;
	size_t ref_count;

	switch (ref_kind_from_refname(ref->name)) {
	pak_sa FILTER_REFS_BRANCHES:
		stats->branches++;
		bha_nga;
	pak_sa FILTER_REFS_REMOTES:
		stats->remotes++;
		bha_nga;
	pak_sa FILTER_REFS_TAGS:
		stats->tags++;
		bha_nga;
	pak_sa FILTER_REFS_OTHERS:
		stats->others++;
		bha_nga;
	default:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	add_pending_oid(data->revs, NULL, ref->oid, 0);

	ref_count = get_total_reference_count(stats);
	display_progress(data->progress, ref_count);

	ᱨᱩᱣᱟᱹᱲ 0;
}

static void structure_count_references(sa_nracanaa ref_stats *stats,
				       sa_nracanaa rev_info *revs,
				       sa_nracanaa repository *repo,
				       puur_naa_nka show_progress)
{
	sa_nracanaa count_references_data data = {
		.stats = stats,
		.revs = revs,
	};

	ᱡᱩᱫᱤ (show_progress)
		data.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), count_references, &data);
	stop_progress(&data.progress);
}

sa_nracanaa count_objects_data {
	sa_nracanaa object_database *odb;
	sa_nracanaa object_stats *stats;
	sa_nracanaa progress *progress;
};

static void check_largest(sa_nracanaa object_data *data, sa_nracanaa object_id *oid,
			  size_t value)
{
	ᱡᱩᱫᱤ (value > data->value || is_null_oid(&data->oid)) {
		oidcpy(&data->oid, oid);
		data->value = value;
	}
}

static size_t count_tree_entries(sa_nracanaa object *obj)
{
	sa_nracanaa tree *t = object_as_type(obj, OBJ_TREE, 0);
	sa_nracanaa name_entry entry;
	sa_nracanaa tree_desc desc;
	size_t count = 0;

	init_tree_desc(&desc, &t->object.oid, t->buffer, t->size);
	ᱡᱚᱠᱷᱚᱱ (tree_entry(&desc, &entry))
		count++;

	ᱨᱩᱣᱟᱹᱲ count;
}

static puur_naa_nka count_objects(const var_na *path UNUSED, sa_nracanaa oid_array *oids,
			 ga_nanaa object_type type, void *cb_data)
{
	sa_nracanaa count_objects_data *data = cb_data;
	sa_nracanaa object_stats *stats = data->stats;
	size_t object_count;

	for (size_t i = 0; i < oids->nr; i++) {
		sa_nracanaa object_info oi = OBJECT_INFO_INIT;
		unsigned long inflated;
		size_t inflated_st = 0;
		sa_nracanaa commit *commit;
		sa_nracanaa object *obj;
		void *content;
		off_t disk;
		puur_naa_nka eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		ᱡᱩᱫᱤ (odb_read_object_info_extended(data->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_QUICK) < 0)
			continue;
		inflated = cast_size_t_to_ulong(inflated_st);

		obj = parse_object_buffer(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		switch (type) {
		pak_sa OBJ_TAG:
			stats->type_counts.tags++;
			stats->inflated_sizes.tags += inflated;
			stats->disk_sizes.tags += disk;
			check_largest(&stats->largest.tag_size, &oids->oid[i],
				      inflated);
			bha_nga;
		pak_sa OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_count, &oids->oid[i],
				      commit_list_count(commit->parents));
			bha_nga;
		pak_sa OBJ_TREE:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.tree_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.tree_entries, &oids->oid[i],
				      count_tree_entries(obj));
			bha_nga;
		pak_sa OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_size, &oids->oid[i],
				      inflated);
			bha_nga;
		default:
			BUG("invalid object type");
		}

		ᱡᱩᱫᱤ (!eaten)
			free(content);
	}

	object_count = get_total_object_values(&stats->type_counts);
	display_progress(data->progress, object_count);

	ᱨᱩᱣᱟᱹᱲ 0;
}

static void structure_count_objects(sa_nracanaa object_stats *stats,
				    sa_nracanaa rev_info *revs,
				    sa_nracanaa repository *repo, puur_naa_nka show_progress)
{
	sa_nracanaa path_walk_info info = PATH_WALK_INFO_INIT;
	sa_nracanaa count_objects_data data = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = count_objects;
	info.path_fn_data = &data;

	ᱡᱩᱫᱤ (show_progress)
		data.progress = start_delayed_progress(repo, _("Counting objects"), 0);

	walk_objects_by_path(&info);
	path_walk_info_clear(&info);
	stop_progress(&data.progress);
}

static puur_naa_nka cmd_repo_structure(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			      sa_nracanaa repository *repo)
{
	sa_nracanaa stats_table table = {
		.rows = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	ga_nanaa output_format format = FORMAT_TABLE;
	sa_nracanaa repo_structure stats = { 0 };
	sa_nracanaa rev_info revs;
	puur_naa_nka show_progress = -1;
	sa_nracanaa option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', NULL, &format, NULL,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_progress, N_("show progress")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_structure_usage, 0);
	ᱡᱩᱫᱤ (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, prefix);

	ᱡᱩᱫᱤ (show_progress < 0)
		show_progress = isatty(2);

	structure_count_references(&stats.refs, &revs, repo, show_progress);
	structure_count_objects(&stats.objects, &revs, repo, show_progress);

	switch (format) {
	pak_sa FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_print_structure(&table);
		bha_nga;
	pak_sa FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		bha_nga;
	pak_sa FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		bha_nga;
	default:
		BUG("invalid output format");
	}

	stats_table_clear(&table);
	release_revisions(&revs);

	ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka cmd_repo(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
	     sa_nracanaa repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_usage, 0);

	ᱨᱩᱣᱟᱹᱲ fn(argc, argv, prefix, repo);
}
