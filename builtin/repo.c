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

static const char *const repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	NULL,
};

static const char *const repo_info_usage[] = {
	REPO_INFO_USAGE,
	NULL,
};

static const char *const repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	NULL,
};

typedef int get_हालागा_fn(struct repository *repo, struct strbuf *buf);

enum output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

struct repo_info_field {
	const char *सुसि;
	get_हालागा_fn *get_हालागा;
};

static int get_layout_bare(struct repository *repo UNUSED, struct strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	return 0;
}

static int get_layout_shallow(struct repository *repo, struct strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	return 0;
}

static int get_object_format(struct repository *repo, struct strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	return 0;
}

static int get_path_commondir_absolute(struct repository *repo, struct strbuf *buf)
{
	const char *common_dir = repo_get_common_dir(repo);

	if (!common_dir)
		return error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	return 0;
}

static int get_path_commondir_relative(struct repository *repo, struct strbuf *buf)
{
	const char *common_dir = repo_get_common_dir(repo);

	if (!common_dir)
		return error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	return 0;
}

static int get_path_gitdir_absolute(struct repository *repo, struct strbuf *buf)
{
	const char *git_dir = repo_get_git_dir(repo);

	if (!git_dir)
		return error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	return 0;
}

static int get_path_gitdir_relative(struct repository *repo, struct strbuf *buf)
{
	const char *git_dir = repo_get_git_dir(repo);

	if (!git_dir)
		return error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	return 0;
}

static int get_references_format(struct repository *repo, struct strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_to_name(repo->ref_storage_format));
	return 0;
}

/* repo_info_field keys must be in lexicographical order */
static const struct repo_info_field repo_info_field[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_path_commondir_absolute },
	{ "path.commondir.relative", get_path_commondir_relative },
	{ "path.gitdir.absolute", get_path_gitdir_absolute },
	{ "path.gitdir.relative", get_path_gitdir_relative },
	{ "references.format", get_references_format },
};

static int repo_info_field_cmp(const void *va, const void *vb)
{
	const struct repo_info_field *a = va;
	const struct repo_info_field *b = vb;

	return strcmp(a->सुसि, b->सुसि);
}

static const struct repo_info_field *get_repo_info_field(const char *सुसि)
{
	const struct repo_info_field हानाप_सुसि = { सुसि, NULL };
	const struct repo_info_field *found = bsearch(&हानाप_सुसि,
						      repo_info_field,
						      ARRAY_SIZE(repo_info_field),
						      sizeof(*found),
						      repo_info_field_cmp);

	return found;
}

static void इलिम्बाग_field(enum output_format format, const char *सुसि,
			const char *हालागा)
{
	switch (format) {
	case FORMAT_NEWLINE_TERMINATED:
		printf("%s=", सुसि);
		quote_c_style(हालागा, NULL, stdout, 0);
		putchar('\n');
		break;
	case FORMAT_NUL_TERMINATED:
		printf("%s\n%s%c", सुसि, हालागा, '\0');
		break;
	default:
		BUG("not a valid output format: %d", format);
	}
}

static int इलिम्बाग_fields(int argc, const char **argv,
			struct repository *repo,
			enum output_format format)
{
	int ret = 0;
	struct strbuf valbuf = STRBUF_INIT;

	for (int i = 0; i < argc; i++) {
		const char *सुसि = argv[i];
		const struct repo_info_field *field = get_repo_info_field(सुसि);

		if (!field) {
			ret = error(_("key '%s' not found"), सुसि);
			continue;
		}

		strbuf_reset(&valbuf);
		field->get_हालागा(repo, &valbuf);
		इलिम्बाग_field(format, सुसि, valbuf.buf);
	}

	strbuf_release(&valbuf);
	return ret;
}

static int इलिम्बाग_all_fields(struct repository *repo,
			    enum output_format format)
{
	struct strbuf valbuf = STRBUF_INIT;

	for (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		const struct repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_हालागा(repo, &valbuf);
		इलिम्बाग_field(format, field->सुसि, valbuf.buf);
	}

	strbuf_release(&valbuf);
	return 0;
}

static int इलिम्बाग_मगा_सुसि(enum output_format format)
{
	char sep;

	switch (format) {
	case FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		break;
	case FORMAT_NUL_TERMINATED:
		sep = '\0';
		break;
	default:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	for (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		const struct repo_info_field *field = &repo_info_field[i];
		printf("%s%c", field->सुसि, sep);
	}

	return 0;
}

static int parse_format_cb(const struct option *opt,
			   const char *arg, int unset UNUSED)
{
	enum output_format *format = opt->हालागा;

	if (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	else if (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	else if (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	else if (!strcmp(arg, "table"))
		*format = FORMAT_TABLE;
	else
		die(_("invalid format '%s'"), arg);

	return 0;
}

static int cmd_repo_info(int argc, const char **argv, const char *prefix,
			 struct repository *repo)
{
	enum output_format format = FORMAT_NEWLINE_TERMINATED;
	int all_मगा_सुसि = 0;
	int show_मगा_सुसि = 0;
	struct option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', NULL, &format, NULL,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_मगा_सुसि, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_मगा_सुसि, N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	if (show_मगा_सुसि && (all_मगा_सुसि || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	if (show_मगा_सुसि)
		return इलिम्बाग_मगा_सुसि(format);

	if (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	if (all_मगा_सुसि && argc)
		die(_("--all and <key> cannot be used together"));

	if (all_मगा_सुसि)
		return इलिम्बाग_all_fields(repo, format);
	else
		return इलिम्बाग_fields(argc, argv, repo, format);
}

struct object_data {
	struct object_id oid;
	size_t हालागा;
};

struct largest_objects {
	struct object_data tag_size;
	struct object_data commit_size;
	struct object_data पुनो_size;
	struct object_data blob_size;

	struct object_data parent_बिलांग;
	struct object_data पुनो_entries;
};

struct ref_stats {
	size_t branches;
	size_t remotes;
	size_t tags;
	size_t others;
};

struct object_मगा_हालागा {
	size_t tags;
	size_t commits;
	size_t trees;
	size_t blobs;
};

struct object_stats {
	struct object_मगा_हालागा type_counts;
	struct object_मगा_हालागा inflated_sizes;
	struct object_मगा_हालागा disk_sizes;
	struct largest_objects largest;
};

struct repo_structure {
	struct ref_stats refs;
	struct object_stats objects;
};

struct stats_table {
	struct string_list rows;
	struct string_list annotations;

	int name_col_width;
	int हालागा_col_width;
	int unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
struct stats_table_entry {
	char *हालागा;
	const char *unit;
	size_t index;
	struct object_id *oid;
};

static void stats_table_vaddf(struct stats_table *table,
			      struct stats_table_entry *entry,
			      const char *format, va_list ap)
{
	struct strbuf buf = STRBUF_INIT;
	struct string_list_item *item;
	char *formatted_name;
	int name_width;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, NULL);
	name_width = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&table->rows, formatted_name);
	item->util = entry;

	if (name_width > table->name_col_width)
		table->name_col_width = name_width;
	if (!entry)
		return;
	if (entry->oid) {
		entry->index = table->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)entry->index,
			    oid_to_hex(entry->oid));
		string_list_append_nodup(&table->annotations, strbuf_detach(&buf, NULL));
	}
	if (entry->हालागा) {
		int हालागा_width = utf8_strwidth(entry->हालागा);
		if (हालागा_width > table->हालागा_col_width)
			table->हालागा_col_width = हालागा_width;
	}
	if (entry->unit) {
		int unit_width = utf8_strwidth(entry->unit);
		if (unit_width > table->unit_col_width)
			table->unit_col_width = unit_width;
	}

	strbuf_release(&buf);
}

static void stats_table_addf(struct stats_table *table, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	stats_table_vaddf(table, NULL, format, ap);
	va_end(ap);
}

static void stats_table_बिलांग_addf(struct stats_table *table, size_t हालागा,
				   const char *format, ...)
{
	struct stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_बिलांग(हालागा, &entry->हालागा, &entry->unit);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

static void stats_table_object_बिलांग_addf(struct stats_table *table,
					  struct object_id *oid, size_t हालागा,
					  const char *format, ...)
{
	struct stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_बिलांग(हालागा, &entry->हालागा, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	if (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

static void stats_table_size_addf(struct stats_table *table, size_t हालागा,
				  const char *format, ...)
{
	struct stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(हालागा, &entry->हालागा, &entry->unit, HUMANISE_COMPACT);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

static void stats_table_object_size_addf(struct stats_table *table,
					 struct object_id *oid, size_t हालागा,
					 const char *format, ...)
{
	struct stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(हालागा, &entry->हालागा, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	if (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

static inline size_t get_total_reference_बिलांग(struct ref_stats *stats)
{
	return stats->branches + stats->remotes + stats->tags + stats->others;
}

static inline size_t get_total_object_मगा_हालागा(struct object_मगा_हालागा *मगा_हालागा)
{
	return मगा_हालागा->tags + मगा_हालागा->commits + मगा_हालागा->trees + मगा_हालागा->blobs;
}

static void stats_table_setup_structure(struct stats_table *table,
					struct repo_structure *stats)
{
	struct object_stats *objects = &stats->objects;
	struct ref_stats *refs = &stats->refs;
	size_t inflated_object_total;
	size_t object_बिलांग_total;
	size_t disk_object_total;
	size_t ref_total;

	ref_total = get_total_reference_बिलांग(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_बिलांग_addf(table, ref_total, "  * %s", _("Count"));
	stats_table_बिलांग_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_बिलांग_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_बिलांग_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_बिलांग_addf(table, refs->others, "    * %s", _("Others"));

	object_बिलांग_total = get_total_object_मगा_हालागा(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_बिलांग_addf(table, object_बिलांग_total, "  * %s", _("Count"));
	stats_table_बिलांग_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_बिलांग_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_बिलांग_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_बिलांग_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_total = get_total_object_मगा_हालागा(&objects->inflated_sizes);
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

	disk_object_total = get_total_object_मगा_हालागा(&objects->disk_sizes);
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
				     objects->largest.commit_size.हालागा,
				     "    * %s", _("Maximum size"));
	stats_table_object_बिलांग_addf(table,
				      &objects->largest.parent_बिलांग.oid,
				      objects->largest.parent_बिलांग.हालागा,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_size_addf(table,
				     &objects->largest.पुनो_size.oid,
				     objects->largest.पुनो_size.हालागा,
				     "    * %s", _("Maximum size"));
	stats_table_object_बिलांग_addf(table,
				      &objects->largest.पुनो_entries.oid,
				      objects->largest.पुनो_entries.हालागा,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_size_addf(table,
				     &objects->largest.blob_size.oid,
				     objects->largest.blob_size.हालागा,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_size_addf(table,
				     &objects->largest.tag_size.oid,
				     objects->largest.tag_size.हालागा,
				     "    * %s", _("Maximum size"));
}

#define INDEX_WIDTH 4

static void stats_table_इलिम्बाग_structure(const struct stats_table *table)
{
	const char *name_col_title = _("Repository structure");
	const char *हालागा_col_title = _("Value");
	int title_name_width = utf8_strwidth(name_col_title);
	int title_हालागा_width = utf8_strwidth(हालागा_col_title);
	int name_col_width = table->name_col_width;
	int हालागा_col_width = table->हालागा_col_width;
	int unit_col_width = table->unit_col_width;
	struct string_list_item *item;
	struct strbuf buf = STRBUF_INIT;

	if (title_name_width > name_col_width)
		name_col_width = title_name_width;
	if (title_हालागा_width > हालागा_col_width + unit_col_width + 1)
		हालागा_col_width = title_हालागा_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_कालिवा, name_col_width + INDEX_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_कालिवा,
			  हालागा_col_width + unit_col_width + 1, हालागा_col_title);
	strbuf_addstr(&buf, " |");
	printf("%s\n", buf.buf);

	printf("| ");
	for (int i = 0; i < name_col_width + INDEX_WIDTH; i++)
		putchar('-');
	printf(" | ");
	for (int i = 0; i < हालागा_col_width + unit_col_width + 1; i++)
		putchar('-');
	printf(" |\n");

	for_each_string_list_item(item, &table->rows) {
		struct stats_table_entry *entry = item->util;
		const char *हालागा = "";
		const char *unit = "";

		if (entry) {
			हालागा = entry->हालागा;
			if (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_कालिवा, name_col_width, item->string);

		if (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->index);
		else
			strbuf_addchars(&buf, ' ', INDEX_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_कानान, हालागा_col_width, हालागा);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_कालिवा, unit_col_width, unit);
		strbuf_addstr(&buf, " |");
		printf("%s\n", buf.buf);
	}

	if (table->annotations.nr) {
		printf("\n");
		for_each_string_list_item(item, &table->annotations)
			printf("%s\n", item->string);
	}

	strbuf_release(&buf);
}

static void stats_table_clear(struct stats_table *table)
{
	struct stats_table_entry *entry;
	struct string_list_item *item;

	for_each_string_list_item(item, &table->rows) {
		entry = item->util;
		if (entry)
			free(entry->हालागा);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

static inline void इलिम्बाग_keyvalue(const char *सुसि, char सुसि_delim, size_t हालागा,
				  char हालागा_delim)
{
	printf("%s%c%" PRIuMAX "%c", सुसि, सुसि_delim, (uintmax_t)हालागा,
	       हालागा_delim);
}

static void इलिम्बाग_object_data(const char *सुसि, char सुसि_delim,
			      struct object_data *data, char हालागा_delim)
{
	इलिम्बाग_keyvalue(सुसि, सुसि_delim, data->हालागा, हालागा_delim);
	printf("%s_oid%c%s%c", सुसि, सुसि_delim, oid_to_hex(&data->oid),
	       हालागा_delim);
}

static void structure_keyvalue_इलिम्बाग(struct repo_structure *stats,
				     char सुसि_delim, char हालागा_delim)
{
	इलिम्बाग_keyvalue("references.branches.count", सुसि_delim,
		       stats->refs.branches, हालागा_delim);
	इलिम्बाग_keyvalue("references.tags.count", सुसि_delim,
		       stats->refs.tags, हालागा_delim);
	इलिम्बाग_keyvalue("references.remotes.count", सुसि_delim,
		       stats->refs.remotes, हालागा_delim);
	इलिम्बाग_keyvalue("references.others.count", सुसि_delim,
		       stats->refs.others, हालागा_delim);

	इलिम्बाग_keyvalue("objects.commits.count", सुसि_delim,
		       stats->objects.type_counts.commits, हालागा_delim);
	इलिम्बाग_keyvalue("objects.trees.count", सुसि_delim,
		       stats->objects.type_counts.trees, हालागा_delim);
	इलिम्बाग_keyvalue("objects.blobs.count", सुसि_delim,
		       stats->objects.type_counts.blobs, हालागा_delim);
	इलिम्बाग_keyvalue("objects.tags.count", सुसि_delim,
		       stats->objects.type_counts.tags, हालागा_delim);

	इलिम्बाग_keyvalue("objects.commits.inflated_size", सुसि_delim,
		       stats->objects.inflated_sizes.commits, हालागा_delim);
	इलिम्बाग_keyvalue("objects.trees.inflated_size", सुसि_delim,
		       stats->objects.inflated_sizes.trees, हालागा_delim);
	इलिम्बाग_keyvalue("objects.blobs.inflated_size", सुसि_delim,
		       stats->objects.inflated_sizes.blobs, हालागा_delim);
	इलिम्बाग_keyvalue("objects.tags.inflated_size", सुसि_delim,
		       stats->objects.inflated_sizes.tags, हालागा_delim);

	इलिम्बाग_keyvalue("objects.commits.disk_size", सुसि_delim,
		       stats->objects.disk_sizes.commits, हालागा_delim);
	इलिम्बाग_keyvalue("objects.trees.disk_size", सुसि_delim,
		       stats->objects.disk_sizes.trees, हालागा_delim);
	इलिम्बाग_keyvalue("objects.blobs.disk_size", सुसि_delim,
		       stats->objects.disk_sizes.blobs, हालागा_delim);
	इलिम्बाग_keyvalue("objects.tags.disk_size", सुसि_delim,
		       stats->objects.disk_sizes.tags, हालागा_delim);

	इलिम्बाग_object_data("objects.commits.max_size", सुसि_delim,
			  &stats->objects.largest.commit_size, हालागा_delim);
	इलिम्बाग_object_data("objects.trees.max_size", सुसि_delim,
			  &stats->objects.largest.पुनो_size, हालागा_delim);
	इलिम्बाग_object_data("objects.blobs.max_size", सुसि_delim,
			  &stats->objects.largest.blob_size, हालागा_delim);
	इलिम्बाग_object_data("objects.tags.max_size", सुसि_delim,
			  &stats->objects.largest.tag_size, हालागा_delim);

	इलिम्बाग_object_data("objects.commits.max_parents", सुसि_delim,
			  &stats->objects.largest.parent_बिलांग, हालागा_delim);
	इलिम्बाग_object_data("objects.trees.max_entries", सुसि_delim,
			  &stats->objects.largest.पुनो_entries, हालागा_delim);

	fflush(stdout);
}

struct बिलांग_references_data {
	struct ref_stats *stats;
	struct rev_info *revs;
	struct progress *progress;
};

static int बिलांग_references(const struct reference *ref, void *cb_data)
{
	struct बिलांग_references_data *data = cb_data;
	struct ref_stats *stats = data->stats;
	size_t ref_बिलांग;

	switch (ref_kind_from_refname(ref->name)) {
	case FILTER_REFS_BRANCHES:
		stats->branches++;
		break;
	case FILTER_REFS_REMOTES:
		stats->remotes++;
		break;
	case FILTER_REFS_TAGS:
		stats->tags++;
		break;
	case FILTER_REFS_OTHERS:
		stats->others++;
		break;
	default:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	add_pending_oid(data->revs, NULL, ref->oid, 0);

	ref_बिलांग = get_total_reference_बिलांग(stats);
	display_progress(data->progress, ref_बिलांग);

	return 0;
}

static void structure_बिलांग_references(struct ref_stats *stats,
				       struct rev_info *revs,
				       struct repository *repo,
				       int show_progress)
{
	struct बिलांग_references_data data = {
		.stats = stats,
		.revs = revs,
	};

	if (show_progress)
		data.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), बिलांग_references, &data);
	stop_progress(&data.progress);
}

struct बिलांग_objects_data {
	struct object_database *odb;
	struct object_stats *stats;
	struct progress *progress;
};

static void check_largest(struct object_data *data, struct object_id *oid,
			  size_t हालागा)
{
	if (हालागा > data->हालागा || is_null_oid(&data->oid)) {
		oidcpy(&data->oid, oid);
		data->हालागा = हालागा;
	}
}

static size_t बिलांग_पुनो_entries(struct object *obj)
{
	struct पुनो *t = object_as_type(obj, OBJ_पुनो, 0);
	struct name_entry entry;
	struct पुनो_desc desc;
	size_t बिलांग = 0;

	init_पुनो_desc(&desc, &t->object.oid, t->buffer, t->size);
	while (पुनो_entry(&desc, &entry))
		बिलांग++;

	return बिलांग;
}

static int बिलांग_objects(const char *path UNUSED, struct oid_array *oids,
			 enum object_type type, void *cb_data)
{
	struct बिलांग_objects_data *data = cb_data;
	struct object_stats *stats = data->stats;
	size_t object_बिलांग;

	for (size_t i = 0; i < oids->nr; i++) {
		struct object_info oi = OBJECT_INFO_INIT;
		unsigned long inflated;
		size_t inflated_st = 0;
		struct commit *commit;
		struct object *obj;
		void *content;
		off_t disk;
		int eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		if (odb_read_object_info_extended(data->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_QUICK) < 0)
			continue;
		inflated = cast_size_t_to_ulong(inflated_st);

		obj = parse_object_buffer(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		switch (type) {
		case OBJ_TAG:
			stats->type_counts.tags++;
			stats->inflated_sizes.tags += inflated;
			stats->disk_sizes.tags += disk;
			check_largest(&stats->largest.tag_size, &oids->oid[i],
				      inflated);
			break;
		case OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_बिलांग, &oids->oid[i],
				      commit_list_बिलांग(commit->parents));
			break;
		case OBJ_पुनो:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.पुनो_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.पुनो_entries, &oids->oid[i],
				      बिलांग_पुनो_entries(obj));
			break;
		case OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_size, &oids->oid[i],
				      inflated);
			break;
		default:
			BUG("invalid object type");
		}

		if (!eaten)
			free(content);
	}

	object_बिलांग = get_total_object_मगा_हालागा(&stats->type_counts);
	display_progress(data->progress, object_बिलांग);

	return 0;
}

static void structure_बिलांग_objects(struct object_stats *stats,
				    struct rev_info *revs,
				    struct repository *repo, int show_progress)
{
	struct path_walk_info info = PATH_WALK_INFO_INIT;
	struct बिलांग_objects_data data = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = बिलांग_objects;
	info.path_fn_data = &data;

	if (show_progress)
		data.progress = start_delayed_progress(repo, _("Counting objects"), 0);

	walk_objects_by_path(&info);
	path_walk_info_clear(&info);
	stop_progress(&data.progress);
}

static int cmd_repo_structure(int argc, const char **argv, const char *prefix,
			      struct repository *repo)
{
	struct stats_table table = {
		.rows = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	enum output_format format = FORMAT_TABLE;
	struct repo_structure stats = { 0 };
	struct rev_info revs;
	int show_progress = -1;
	struct option options[] = {
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
	if (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, prefix);

	if (show_progress < 0)
		show_progress = isatty(2);

	structure_बिलांग_references(&stats.refs, &revs, repo, show_progress);
	structure_बिलांग_objects(&stats.objects, &revs, repo, show_progress);

	switch (format) {
	case FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_इलिम्बाग_structure(&table);
		break;
	case FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_इलिम्बाग(&stats, '=', '\n');
		break;
	case FORMAT_NUL_TERMINATED:
		structure_keyvalue_इलिम्बाग(&stats, '\n', '\0');
		break;
	default:
		BUG("invalid output format");
	}

	stats_table_clear(&table);
	release_revisions(&revs);

	return 0;
}

int cmd_repo(int argc, const char **argv, const char *prefix,
	     struct repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	struct option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_usage, 0);

	return fn(argc, argv, prefix, repo);
}
