#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "path-walk.h"
#અન્તર્ભાવ "progress.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "ref-filter.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "shallow.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "tree-walk.h"
#અન્તર્ભાવ "utf8.h"

#define REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#define REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

સ્થિર અચલ અક્ષર *અચલ repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	ખાલી,
};

સ્થિર અચલ અક્ષર *અચલ repo_info_usage[] = {
	REPO_INFO_USAGE,
	ખાલી,
};

સ્થિર અચલ અક્ષર *અચલ repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	ખાલી,
};

પ્રકારવ્યાખ્યા પૂર્ણાંક get_કિંમત_fn(સંરચના repository *repo, સંરચના strbuf *buf);

ગણના output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

સંરચના repo_info_field {
	અચલ અક્ષર *ચાવી;
	get_કિંમત_fn *get_કિંમત;
};

સ્થિર પૂર્ણાંક get_layout_bare(સંરચના repository *repo UNUSED, સંરચના strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_layout_shallow(સંરચના repository *repo, સંરચના strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_object_format(સંરચના repository *repo, સંરચના strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_path_commondir_absolute(સંરચના repository *repo, સંરચના strbuf *buf)
{
	અચલ અક્ષર *common_dir = repo_get_common_dir(repo);

	જો (!common_dir)
		પાછા error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_path_commondir_relative(સંરચના repository *repo, સંરચના strbuf *buf)
{
	અચલ અક્ષર *common_dir = repo_get_common_dir(repo);

	જો (!common_dir)
		પાછા error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_path_gitdir_absolute(સંરચના repository *repo, સંરચના strbuf *buf)
{
	અચલ અક્ષર *git_dir = repo_get_git_dir(repo);

	જો (!git_dir)
		પાછા error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_path_gitdir_relative(સંરચના repository *repo, સંરચના strbuf *buf)
{
	અચલ અક્ષર *git_dir = repo_get_git_dir(repo);

	જો (!git_dir)
		પાછા error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_references_format(સંરચના repository *repo, સંરચના strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_to_name(repo->ref_storage_format));
	પાછા 0;
}

/* repo_info_field keys must be in lexicographical order */
સ્થિર અચલ સંરચના repo_info_field repo_info_field[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_path_commondir_absolute },
	{ "path.commondir.relative", get_path_commondir_relative },
	{ "path.gitdir.absolute", get_path_gitdir_absolute },
	{ "path.gitdir.relative", get_path_gitdir_relative },
	{ "references.format", get_references_format },
};

સ્થિર પૂર્ણાંક repo_info_field_cmp(અચલ શૂન્ય *va, અચલ શૂન્ય *vb)
{
	અચલ સંરચના repo_info_field *a = va;
	અચલ સંરચના repo_info_field *b = vb;

	પાછા strcmp(a->ચાવી, b->ચાવી);
}

સ્થિર અચલ સંરચના repo_info_field *get_repo_info_field(અચલ અક્ષર *ચાવી)
{
	અચલ સંરચના repo_info_field શોધ_ચાવી = { ચાવી, ખાલી };
	અચલ સંરચના repo_info_field *found = bsearch(&શોધ_ચાવી,
						      repo_info_field,
						      ARRAY_SIZE(repo_info_field),
						      કદ(*found),
						      repo_info_field_cmp);

	પાછા found;
}

સ્થિર શૂન્ય print_field(ગણના output_format format, અચલ અક્ષર *ચાવી,
			અચલ અક્ષર *કિંમત)
{
	પસંદગી (format) {
	સ્થિતિ FORMAT_NEWLINE_TERMINATED:
		printf("%s=", ચાવી);
		quote_c_style(કિંમત, ખાલી, stdout, 0);
		putchar('\n');
		તોડો;
	સ્થિતિ FORMAT_NUL_TERMINATED:
		printf("%s\n%s%c", ચાવી, કિંમત, '\0');
		તોડો;
	મૂળભૂત:
		BUG("not a valid output format: %d", format);
	}
}

સ્થિર પૂર્ણાંક print_fields(પૂર્ણાંક argc, અચલ અક્ષર **argv,
			સંરચના repository *repo,
			ગણના output_format format)
{
	પૂર્ણાંક ret = 0;
	સંરચના strbuf valbuf = STRBUF_INIT;

	માટે (પૂર્ણાંક i = 0; i < argc; i++) {
		અચલ અક્ષર *ચાવી = argv[i];
		અચલ સંરચના repo_info_field *field = get_repo_info_field(ચાવી);

		જો (!field) {
			ret = error(_("key '%s' not found"), ચાવી);
			ચાલુ;
		}

		strbuf_reset(&valbuf);
		field->get_કિંમત(repo, &valbuf);
		print_field(format, ચાવી, valbuf.buf);
	}

	strbuf_release(&valbuf);
	પાછા ret;
}

સ્થિર પૂર્ણાંક print_all_fields(સંરચના repository *repo,
			    ગણના output_format format)
{
	સંરચના strbuf valbuf = STRBUF_INIT;

	માટે (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		અચલ સંરચના repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_કિંમત(repo, &valbuf);
		print_field(format, field->ચાવી, valbuf.buf);
	}

	strbuf_release(&valbuf);
	પાછા 0;
}

સ્થિર પૂર્ણાંક print_ચાવીઓ(ગણના output_format format)
{
	અક્ષર sep;

	પસંદગી (format) {
	સ્થિતિ FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		તોડો;
	સ્થિતિ FORMAT_NUL_TERMINATED:
		sep = '\0';
		તોડો;
	મૂળભૂત:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	માટે (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		અચલ સંરચના repo_info_field *field = &repo_info_field[i];
		printf("%s%c", field->ચાવી, sep);
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_format_cb(અચલ સંરચના option *opt,
			   અચલ અક્ષર *arg, પૂર્ણાંક unset UNUSED)
{
	ગણના output_format *format = opt->કિંમત;

	જો (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	નહીંતર જો (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	નહીંતર જો (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	નહીંતર જો (!strcmp(arg, "table"))
		*format = FORMAT_TABLE;
	નહીંતર
		die(_("invalid format '%s'"), arg);

	પાછા 0;
}

સ્થિર પૂર્ણાંક cmd_repo_info(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			 સંરચના repository *repo)
{
	ગણના output_format format = FORMAT_NEWLINE_TERMINATED;
	પૂર્ણાંક all_ચાવીઓ = 0;
	પૂર્ણાંક show_ચાવીઓ = 0;
	સંરચના option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ખાલી, &format, ખાલી,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_ચાવીઓ, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_ચાવીઓ, N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	જો (show_ચાવીઓ && (all_ચાવીઓ || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	જો (show_ચાવીઓ)
		પાછા print_ચાવીઓ(format);

	જો (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	જો (all_ચાવીઓ && argc)
		die(_("--all and <key> cannot be used together"));

	જો (all_ચાવીઓ)
		પાછા print_all_fields(repo, format);
	નહીંતર
		પાછા print_fields(argc, argv, repo, format);
}

સંરચના object_data {
	સંરચના object_id oid;
	size_t કિંમત;
};

સંરચના largest_objects {
	સંરચના object_data tag_size;
	સંરચના object_data commit_size;
	સંરચના object_data વૃક્ષ_size;
	સંરચના object_data blob_size;

	સંરચના object_data parent_ગણતરી;
	સંરચના object_data વૃક્ષ_entries;
};

સંરચના ref_stats {
	size_t branches;
	size_t remotes;
	size_t tags;
	size_t others;
};

સંરચના object_કિંમતો {
	size_t tags;
	size_t commits;
	size_t trees;
	size_t blobs;
};

સંરચના object_stats {
	સંરચના object_કિંમતો type_counts;
	સંરચના object_કિંમતો inflated_sizes;
	સંરચના object_કિંમતો disk_sizes;
	સંરચના largest_objects largest;
};

સંરચના repo_structure {
	સંરચના ref_stats refs;
	સંરચના object_stats objects;
};

સંરચના stats_table {
	સંરચના string_list rows;
	સંરચના string_list annotations;

	પૂર્ણાંક name_col_width;
	પૂર્ણાંક કિંમત_col_width;
	પૂર્ણાંક unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
સંરચના stats_table_entry {
	અક્ષર *કિંમત;
	અચલ અક્ષર *unit;
	size_t index;
	સંરચના object_id *oid;
};

સ્થિર શૂન્ય stats_table_vaddf(સંરચના stats_table *table,
			      સંરચના stats_table_entry *entry,
			      અચલ અક્ષર *format, va_list ap)
{
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના string_list_item *item;
	અક્ષર *formatted_name;
	પૂર્ણાંક name_width;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, ખાલી);
	name_width = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&table->rows, formatted_name);
	item->util = entry;

	જો (name_width > table->name_col_width)
		table->name_col_width = name_width;
	જો (!entry)
		પાછા;
	જો (entry->oid) {
		entry->index = table->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)entry->index,
			    oid_to_hex(entry->oid));
		string_list_append_nodup(&table->annotations, strbuf_detach(&buf, ખાલી));
	}
	જો (entry->કિંમત) {
		પૂર્ણાંક કિંમત_width = utf8_strwidth(entry->કિંમત);
		જો (કિંમત_width > table->કિંમત_col_width)
			table->કિંમત_col_width = કિંમત_width;
	}
	જો (entry->unit) {
		પૂર્ણાંક unit_width = utf8_strwidth(entry->unit);
		જો (unit_width > table->unit_col_width)
			table->unit_col_width = unit_width;
	}

	strbuf_release(&buf);
}

સ્થિર શૂન્ય stats_table_addf(સંરચના stats_table *table, અચલ અક્ષર *format, ...)
{
	va_list ap;

	va_start(ap, format);
	stats_table_vaddf(table, ખાલી, format, ap);
	va_end(ap);
}

સ્થિર શૂન્ય stats_table_ગણતરી_addf(સંરચના stats_table *table, size_t કિંમત,
				   અચલ અક્ષર *format, ...)
{
	સંરચના stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_ગણતરી(કિંમત, &entry->કિંમત, &entry->unit);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

સ્થિર શૂન્ય stats_table_object_ગણતરી_addf(સંરચના stats_table *table,
					  સંરચના object_id *oid, size_t કિંમત,
					  અચલ અક્ષર *format, ...)
{
	સંરચના stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_ગણતરી(કિંમત, &entry->કિંમત, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	જો (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

સ્થિર શૂન્ય stats_table_size_addf(સંરચના stats_table *table, size_t કિંમત,
				  અચલ અક્ષર *format, ...)
{
	સંરચના stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(કિંમત, &entry->કિંમત, &entry->unit, HUMANISE_COMPACT);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

સ્થિર શૂન્ય stats_table_object_size_addf(સંરચના stats_table *table,
					 સંરચના object_id *oid, size_t કિંમત,
					 અચલ અક્ષર *format, ...)
{
	સંરચના stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(કિંમત, &entry->કિંમત, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	જો (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

સ્થિર અંતઃરેખ size_t get_total_reference_ગણતરી(સંરચના ref_stats *stats)
{
	પાછા stats->branches + stats->remotes + stats->tags + stats->others;
}

સ્થિર અંતઃરેખ size_t get_total_object_કિંમતો(સંરચના object_કિંમતો *કિંમતો)
{
	પાછા કિંમતો->tags + કિંમતો->commits + કિંમતો->trees + કિંમતો->blobs;
}

સ્થિર શૂન્ય stats_table_setup_structure(સંરચના stats_table *table,
					સંરચના repo_structure *stats)
{
	સંરચના object_stats *objects = &stats->objects;
	સંરચના ref_stats *refs = &stats->refs;
	size_t inflated_object_total;
	size_t object_ગણતરી_total;
	size_t disk_object_total;
	size_t ref_total;

	ref_total = get_total_reference_ગણતરી(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_ગણતરી_addf(table, ref_total, "  * %s", _("Count"));
	stats_table_ગણતરી_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_ગણતરી_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_ગણતરી_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_ગણતરી_addf(table, refs->others, "    * %s", _("Others"));

	object_ગણતરી_total = get_total_object_કિંમતો(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_ગણતરી_addf(table, object_ગણતરી_total, "  * %s", _("Count"));
	stats_table_ગણતરી_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_ગણતરી_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_ગણતરી_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_ગણતરી_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_total = get_total_object_કિંમતો(&objects->inflated_sizes);
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

	disk_object_total = get_total_object_કિંમતો(&objects->disk_sizes);
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
				     objects->largest.commit_size.કિંમત,
				     "    * %s", _("Maximum size"));
	stats_table_object_ગણતરી_addf(table,
				      &objects->largest.parent_ગણતરી.oid,
				      objects->largest.parent_ગણતરી.કિંમત,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_size_addf(table,
				     &objects->largest.વૃક્ષ_size.oid,
				     objects->largest.વૃક્ષ_size.કિંમત,
				     "    * %s", _("Maximum size"));
	stats_table_object_ગણતરી_addf(table,
				      &objects->largest.વૃક્ષ_entries.oid,
				      objects->largest.વૃક્ષ_entries.કિંમત,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_size_addf(table,
				     &objects->largest.blob_size.oid,
				     objects->largest.blob_size.કિંમત,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_size_addf(table,
				     &objects->largest.tag_size.oid,
				     objects->largest.tag_size.કિંમત,
				     "    * %s", _("Maximum size"));
}

#define INDEX_WIDTH 4

સ્થિર શૂન્ય stats_table_print_structure(અચલ સંરચના stats_table *table)
{
	અચલ અક્ષર *name_col_title = _("Repository structure");
	અચલ અક્ષર *કિંમત_col_title = _("Value");
	પૂર્ણાંક title_name_width = utf8_strwidth(name_col_title);
	પૂર્ણાંક title_કિંમત_width = utf8_strwidth(કિંમત_col_title);
	પૂર્ણાંક name_col_width = table->name_col_width;
	પૂર્ણાંક કિંમત_col_width = table->કિંમત_col_width;
	પૂર્ણાંક unit_col_width = table->unit_col_width;
	સંરચના string_list_item *item;
	સંરચના strbuf buf = STRBUF_INIT;

	જો (title_name_width > name_col_width)
		name_col_width = title_name_width;
	જો (title_કિંમત_width > કિંમત_col_width + unit_col_width + 1)
		કિંમત_col_width = title_કિંમત_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_ડાબું, name_col_width + INDEX_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_ડાબું,
			  કિંમત_col_width + unit_col_width + 1, કિંમત_col_title);
	strbuf_addstr(&buf, " |");
	printf("%s\n", buf.buf);

	printf("| ");
	માટે (પૂર્ણાંક i = 0; i < name_col_width + INDEX_WIDTH; i++)
		putchar('-');
	printf(" | ");
	માટે (પૂર્ણાંક i = 0; i < કિંમત_col_width + unit_col_width + 1; i++)
		putchar('-');
	printf(" |\n");

	for_each_string_list_item(item, &table->rows) {
		સંરચના stats_table_entry *entry = item->util;
		અચલ અક્ષર *કિંમત = "";
		અચલ અક્ષર *unit = "";

		જો (entry) {
			કિંમત = entry->કિંમત;
			જો (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_ડાબું, name_col_width, item->string);

		જો (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->index);
		નહીંતર
			strbuf_addchars(&buf, ' ', INDEX_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_જમણું, કિંમત_col_width, કિંમત);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_ડાબું, unit_col_width, unit);
		strbuf_addstr(&buf, " |");
		printf("%s\n", buf.buf);
	}

	જો (table->annotations.nr) {
		printf("\n");
		for_each_string_list_item(item, &table->annotations)
			printf("%s\n", item->string);
	}

	strbuf_release(&buf);
}

સ્થિર શૂન્ય stats_table_clear(સંરચના stats_table *table)
{
	સંરચના stats_table_entry *entry;
	સંરચના string_list_item *item;

	for_each_string_list_item(item, &table->rows) {
		entry = item->util;
		જો (entry)
			મુક્ત(entry->કિંમત);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

સ્થિર અંતઃરેખ શૂન્ય print_keyvalue(અચલ અક્ષર *ચાવી, અક્ષર ચાવી_delim, size_t કિંમત,
				  અક્ષર કિંમત_delim)
{
	printf("%s%c%" PRIuMAX "%c", ચાવી, ચાવી_delim, (uintmax_t)કિંમત,
	       કિંમત_delim);
}

સ્થિર શૂન્ય print_object_data(અચલ અક્ષર *ચાવી, અક્ષર ચાવી_delim,
			      સંરચના object_data *data, અક્ષર કિંમત_delim)
{
	print_keyvalue(ચાવી, ચાવી_delim, data->કિંમત, કિંમત_delim);
	printf("%s_oid%c%s%c", ચાવી, ચાવી_delim, oid_to_hex(&data->oid),
	       કિંમત_delim);
}

સ્થિર શૂન્ય structure_keyvalue_print(સંરચના repo_structure *stats,
				     અક્ષર ચાવી_delim, અક્ષર કિંમત_delim)
{
	print_keyvalue("references.branches.count", ચાવી_delim,
		       stats->refs.branches, કિંમત_delim);
	print_keyvalue("references.tags.count", ચાવી_delim,
		       stats->refs.tags, કિંમત_delim);
	print_keyvalue("references.remotes.count", ચાવી_delim,
		       stats->refs.remotes, કિંમત_delim);
	print_keyvalue("references.others.count", ચાવી_delim,
		       stats->refs.others, કિંમત_delim);

	print_keyvalue("objects.commits.count", ચાવી_delim,
		       stats->objects.type_counts.commits, કિંમત_delim);
	print_keyvalue("objects.trees.count", ચાવી_delim,
		       stats->objects.type_counts.trees, કિંમત_delim);
	print_keyvalue("objects.blobs.count", ચાવી_delim,
		       stats->objects.type_counts.blobs, કિંમત_delim);
	print_keyvalue("objects.tags.count", ચાવી_delim,
		       stats->objects.type_counts.tags, કિંમત_delim);

	print_keyvalue("objects.commits.inflated_size", ચાવી_delim,
		       stats->objects.inflated_sizes.commits, કિંમત_delim);
	print_keyvalue("objects.trees.inflated_size", ચાવી_delim,
		       stats->objects.inflated_sizes.trees, કિંમત_delim);
	print_keyvalue("objects.blobs.inflated_size", ચાવી_delim,
		       stats->objects.inflated_sizes.blobs, કિંમત_delim);
	print_keyvalue("objects.tags.inflated_size", ચાવી_delim,
		       stats->objects.inflated_sizes.tags, કિંમત_delim);

	print_keyvalue("objects.commits.disk_size", ચાવી_delim,
		       stats->objects.disk_sizes.commits, કિંમત_delim);
	print_keyvalue("objects.trees.disk_size", ચાવી_delim,
		       stats->objects.disk_sizes.trees, કિંમત_delim);
	print_keyvalue("objects.blobs.disk_size", ચાવી_delim,
		       stats->objects.disk_sizes.blobs, કિંમત_delim);
	print_keyvalue("objects.tags.disk_size", ચાવી_delim,
		       stats->objects.disk_sizes.tags, કિંમત_delim);

	print_object_data("objects.commits.max_size", ચાવી_delim,
			  &stats->objects.largest.commit_size, કિંમત_delim);
	print_object_data("objects.trees.max_size", ચાવી_delim,
			  &stats->objects.largest.વૃક્ષ_size, કિંમત_delim);
	print_object_data("objects.blobs.max_size", ચાવી_delim,
			  &stats->objects.largest.blob_size, કિંમત_delim);
	print_object_data("objects.tags.max_size", ચાવી_delim,
			  &stats->objects.largest.tag_size, કિંમત_delim);

	print_object_data("objects.commits.max_parents", ચાવી_delim,
			  &stats->objects.largest.parent_ગણતરી, કિંમત_delim);
	print_object_data("objects.trees.max_entries", ચાવી_delim,
			  &stats->objects.largest.વૃક્ષ_entries, કિંમત_delim);

	fflush(stdout);
}

સંરચના ગણતરી_references_data {
	સંરચના ref_stats *stats;
	સંરચના rev_info *revs;
	સંરચના progress *progress;
};

સ્થિર પૂર્ણાંક ગણતરી_references(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	સંરચના ગણતરી_references_data *data = cb_data;
	સંરચના ref_stats *stats = data->stats;
	size_t ref_ગણતરી;

	પસંદગી (ref_kind_from_refname(ref->name)) {
	સ્થિતિ FILTER_REFS_BRANCHES:
		stats->branches++;
		તોડો;
	સ્થિતિ FILTER_REFS_REMOTES:
		stats->remotes++;
		તોડો;
	સ્થિતિ FILTER_REFS_TAGS:
		stats->tags++;
		તોડો;
	સ્થિતિ FILTER_REFS_OTHERS:
		stats->others++;
		તોડો;
	મૂળભૂત:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	add_pending_oid(data->revs, ખાલી, ref->oid, 0);

	ref_ગણતરી = get_total_reference_ગણતરી(stats);
	display_progress(data->progress, ref_ગણતરી);

	પાછા 0;
}

સ્થિર શૂન્ય structure_ગણતરી_references(સંરચના ref_stats *stats,
				       સંરચના rev_info *revs,
				       સંરચના repository *repo,
				       પૂર્ણાંક show_progress)
{
	સંરચના ગણતરી_references_data data = {
		.stats = stats,
		.revs = revs,
	};

	જો (show_progress)
		data.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), ગણતરી_references, &data);
	stop_progress(&data.progress);
}

સંરચના ગણતરી_objects_data {
	સંરચના object_database *odb;
	સંરચના object_stats *stats;
	સંરચના progress *progress;
};

સ્થિર શૂન્ય check_largest(સંરચના object_data *data, સંરચના object_id *oid,
			  size_t કિંમત)
{
	જો (કિંમત > data->કિંમત || is_null_oid(&data->oid)) {
		oidcpy(&data->oid, oid);
		data->કિંમત = કિંમત;
	}
}

સ્થિર size_t ગણતરી_વૃક્ષ_entries(સંરચના object *obj)
{
	સંરચના વૃક્ષ *t = object_as_type(obj, OBJ_વૃક્ષ, 0);
	સંરચના name_entry entry;
	સંરચના વૃક્ષ_desc desc;
	size_t ગણતરી = 0;

	init_વૃક્ષ_desc(&desc, &t->object.oid, t->buffer, t->size);
	જ્યારે (વૃક્ષ_entry(&desc, &entry))
		ગણતરી++;

	પાછા ગણતરી;
}

સ્થિર પૂર્ણાંક ગણતરી_objects(અચલ અક્ષર *path UNUSED, સંરચના oid_array *oids,
			 ગણના object_type type, શૂન્ય *cb_data)
{
	સંરચના ગણતરી_objects_data *data = cb_data;
	સંરચના object_stats *stats = data->stats;
	size_t object_ગણતરી;

	માટે (size_t i = 0; i < oids->nr; i++) {
		સંરચના object_info oi = OBJECT_INFO_INIT;
		unsigned long inflated;
		size_t inflated_st = 0;
		સંરચના commit *commit;
		સંરચના object *obj;
		શૂન્ય *content;
		off_t disk;
		પૂર્ણાંક eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		જો (odb_read_object_info_extended(data->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_QUICK) < 0)
			ચાલુ;
		inflated = cast_size_t_to_ulong(inflated_st);

		obj = parse_object_buffer(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		પસંદગી (type) {
		સ્થિતિ OBJ_TAG:
			stats->type_counts.tags++;
			stats->inflated_sizes.tags += inflated;
			stats->disk_sizes.tags += disk;
			check_largest(&stats->largest.tag_size, &oids->oid[i],
				      inflated);
			તોડો;
		સ્થિતિ OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_ગણતરી, &oids->oid[i],
				      commit_list_ગણતરી(commit->parents));
			તોડો;
		સ્થિતિ OBJ_વૃક્ષ:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.વૃક્ષ_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.વૃક્ષ_entries, &oids->oid[i],
				      ગણતરી_વૃક્ષ_entries(obj));
			તોડો;
		સ્થિતિ OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_size, &oids->oid[i],
				      inflated);
			તોડો;
		મૂળભૂત:
			BUG("invalid object type");
		}

		જો (!eaten)
			મુક્ત(content);
	}

	object_ગણતરી = get_total_object_કિંમતો(&stats->type_counts);
	display_progress(data->progress, object_ગણતરી);

	પાછા 0;
}

સ્થિર શૂન્ય structure_ગણતરી_objects(સંરચના object_stats *stats,
				    સંરચના rev_info *revs,
				    સંરચના repository *repo, પૂર્ણાંક show_progress)
{
	સંરચના path_walk_info info = PATH_WALK_INFO_INIT;
	સંરચના ગણતરી_objects_data data = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = ગણતરી_objects;
	info.path_fn_data = &data;

	જો (show_progress)
		data.progress = start_delayed_progress(repo, _("Counting objects"), 0);

	walk_objects_by_path(&info);
	path_walk_info_clear(&info);
	stop_progress(&data.progress);
}

સ્થિર પૂર્ણાંક cmd_repo_structure(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			      સંરચના repository *repo)
{
	સંરચના stats_table table = {
		.rows = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	ગણના output_format format = FORMAT_TABLE;
	સંરચના repo_structure stats = { 0 };
	સંરચના rev_info revs;
	પૂર્ણાંક show_progress = -1;
	સંરચના option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ખાલી, &format, ખાલી,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_progress, N_("show progress")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_structure_usage, 0);
	જો (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, prefix);

	જો (show_progress < 0)
		show_progress = isatty(2);

	structure_ગણતરી_references(&stats.refs, &revs, repo, show_progress);
	structure_ગણતરી_objects(&stats.objects, &revs, repo, show_progress);

	પસંદગી (format) {
	સ્થિતિ FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_print_structure(&table);
		તોડો;
	સ્થિતિ FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		તોડો;
	સ્થિતિ FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		તોડો;
	મૂળભૂત:
		BUG("invalid output format");
	}

	stats_table_clear(&table);
	release_revisions(&revs);

	પાછા 0;
}

પૂર્ણાંક cmd_repo(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
	     સંરચના repository *repo)
{
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_usage, 0);

	પાછા fn(argc, argv, prefix, repo);
}
