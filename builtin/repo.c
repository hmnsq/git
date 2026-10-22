#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "path-walk.h"
#அந்தர்பாவ "progress.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "ref-filter.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "shallow.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "tree-walk.h"
#அந்தர்பாவ "utf8.h"

#define REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#define REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

நிலையான மாறிலி எழுத்து *மாறிலி repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	வெற்று,
};

நிலையான மாறிலி எழுத்து *மாறிலி repo_info_usage[] = {
	REPO_INFO_USAGE,
	வெற்று,
};

நிலையான மாறிலி எழுத்து *மாறிலி repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	வெற்று,
};

வகைவரை முழு get_மதிப்பு_fn(கட்டமைப்பு repository *repo, கட்டமைப்பு strbuf *buf);

எண்ணிக்கை output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

கட்டமைப்பு repo_info_field {
	மாறிலி எழுத்து *சாவி;
	get_மதிப்பு_fn *get_மதிப்பு;
};

நிலையான முழு get_layout_bare(கட்டமைப்பு repository *repo UNUSED, கட்டமைப்பு strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	திரும்பு 0;
}

நிலையான முழு get_layout_shallow(கட்டமைப்பு repository *repo, கட்டமைப்பு strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	திரும்பு 0;
}

நிலையான முழு get_object_format(கட்டமைப்பு repository *repo, கட்டமைப்பு strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	திரும்பு 0;
}

நிலையான முழு get_path_commondir_absolute(கட்டமைப்பு repository *repo, கட்டமைப்பு strbuf *buf)
{
	மாறிலி எழுத்து *common_dir = repo_get_common_dir(repo);

	எனில் (!common_dir)
		திரும்பு error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	திரும்பு 0;
}

நிலையான முழு get_path_commondir_relative(கட்டமைப்பு repository *repo, கட்டமைப்பு strbuf *buf)
{
	மாறிலி எழுத்து *common_dir = repo_get_common_dir(repo);

	எனில் (!common_dir)
		திரும்பு error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	திரும்பு 0;
}

நிலையான முழு get_path_gitdir_absolute(கட்டமைப்பு repository *repo, கட்டமைப்பு strbuf *buf)
{
	மாறிலி எழுத்து *git_dir = repo_get_git_dir(repo);

	எனில் (!git_dir)
		திரும்பு error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	திரும்பு 0;
}

நிலையான முழு get_path_gitdir_relative(கட்டமைப்பு repository *repo, கட்டமைப்பு strbuf *buf)
{
	மாறிலி எழுத்து *git_dir = repo_get_git_dir(repo);

	எனில் (!git_dir)
		திரும்பு error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	திரும்பு 0;
}

நிலையான முழு get_references_format(கட்டமைப்பு repository *repo, கட்டமைப்பு strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_to_name(repo->ref_storage_format));
	திரும்பு 0;
}

/* repo_info_field keys must be in lexicographical order */
நிலையான மாறிலி கட்டமைப்பு repo_info_field repo_info_field[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_path_commondir_absolute },
	{ "path.commondir.relative", get_path_commondir_relative },
	{ "path.gitdir.absolute", get_path_gitdir_absolute },
	{ "path.gitdir.relative", get_path_gitdir_relative },
	{ "references.format", get_references_format },
};

நிலையான முழு repo_info_field_cmp(மாறிலி வெறுமை *va, மாறிலி வெறுமை *vb)
{
	மாறிலி கட்டமைப்பு repo_info_field *a = va;
	மாறிலி கட்டமைப்பு repo_info_field *b = vb;

	திரும்பு strcmp(a->சாவி, b->சாவி);
}

நிலையான மாறிலி கட்டமைப்பு repo_info_field *get_repo_info_field(மாறிலி எழுத்து *சாவி)
{
	மாறிலி கட்டமைப்பு repo_info_field தேடல்_சாவி = { சாவி, வெற்று };
	மாறிலி கட்டமைப்பு repo_info_field *found = bsearch(&தேடல்_சாவி,
						      repo_info_field,
						      ARRAY_SIZE(repo_info_field),
						      அளவு(*found),
						      repo_info_field_cmp);

	திரும்பு found;
}

நிலையான வெறுமை print_field(எண்ணிக்கை output_format format, மாறிலி எழுத்து *சாவி,
			மாறிலி எழுத்து *மதிப்பு)
{
	தேர்வு (format) {
	நிலை FORMAT_NEWLINE_TERMINATED:
		printf("%s=", சாவி);
		quote_c_style(மதிப்பு, வெற்று, stdout, 0);
		putchar('\n');
		நிறுத்து;
	நிலை FORMAT_NUL_TERMINATED:
		printf("%s\n%s%c", சாவி, மதிப்பு, '\0');
		நிறுத்து;
	இயல்பு:
		BUG("not a valid output format: %d", format);
	}
}

நிலையான முழு print_fields(முழு argc, மாறிலி எழுத்து **argv,
			கட்டமைப்பு repository *repo,
			எண்ணிக்கை output_format format)
{
	முழு ret = 0;
	கட்டமைப்பு strbuf valbuf = STRBUF_INIT;

	க்கு (முழு i = 0; i < argc; i++) {
		மாறிலி எழுத்து *சாவி = argv[i];
		மாறிலி கட்டமைப்பு repo_info_field *field = get_repo_info_field(சாவி);

		எனில் (!field) {
			ret = error(_("key '%s' not found"), சாவி);
			தொடர்;
		}

		strbuf_reset(&valbuf);
		field->get_மதிப்பு(repo, &valbuf);
		print_field(format, சாவி, valbuf.buf);
	}

	strbuf_release(&valbuf);
	திரும்பு ret;
}

நிலையான முழு print_all_fields(கட்டமைப்பு repository *repo,
			    எண்ணிக்கை output_format format)
{
	கட்டமைப்பு strbuf valbuf = STRBUF_INIT;

	க்கு (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		மாறிலி கட்டமைப்பு repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_மதிப்பு(repo, &valbuf);
		print_field(format, field->சாவி, valbuf.buf);
	}

	strbuf_release(&valbuf);
	திரும்பு 0;
}

நிலையான முழு print_சாவிகள்(எண்ணிக்கை output_format format)
{
	எழுத்து sep;

	தேர்வு (format) {
	நிலை FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		நிறுத்து;
	நிலை FORMAT_NUL_TERMINATED:
		sep = '\0';
		நிறுத்து;
	இயல்பு:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	க்கு (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		மாறிலி கட்டமைப்பு repo_info_field *field = &repo_info_field[i];
		printf("%s%c", field->சாவி, sep);
	}

	திரும்பு 0;
}

நிலையான முழு parse_format_cb(மாறிலி கட்டமைப்பு option *opt,
			   மாறிலி எழுத்து *arg, முழு unset UNUSED)
{
	எண்ணிக்கை output_format *format = opt->மதிப்பு;

	எனில் (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	இல்லையெனில் எனில் (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	இல்லையெனில் எனில் (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	இல்லையெனில் எனில் (!strcmp(arg, "table"))
		*format = FORMAT_TABLE;
	இல்லையெனில்
		die(_("invalid format '%s'"), arg);

	திரும்பு 0;
}

நிலையான முழு cmd_repo_info(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு repository *repo)
{
	எண்ணிக்கை output_format format = FORMAT_NEWLINE_TERMINATED;
	முழு all_சாவிகள் = 0;
	முழு show_சாவிகள் = 0;
	கட்டமைப்பு option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', வெற்று, &format, வெற்று,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_சாவிகள், N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_சாவிகள், N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	எனில் (show_சாவிகள் && (all_சாவிகள் || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	எனில் (show_சாவிகள்)
		திரும்பு print_சாவிகள்(format);

	எனில் (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	எனில் (all_சாவிகள் && argc)
		die(_("--all and <key> cannot be used together"));

	எனில் (all_சாவிகள்)
		திரும்பு print_all_fields(repo, format);
	இல்லையெனில்
		திரும்பு print_fields(argc, argv, repo, format);
}

கட்டமைப்பு object_data {
	கட்டமைப்பு object_id oid;
	size_t மதிப்பு;
};

கட்டமைப்பு largest_objects {
	கட்டமைப்பு object_data tag_size;
	கட்டமைப்பு object_data commit_size;
	கட்டமைப்பு object_data மரம்_size;
	கட்டமைப்பு object_data blob_size;

	கட்டமைப்பு object_data parent_மொத்தம்;
	கட்டமைப்பு object_data மரம்_entries;
};

கட்டமைப்பு ref_stats {
	size_t branches;
	size_t remotes;
	size_t tags;
	size_t others;
};

கட்டமைப்பு object_மதிப்புகள் {
	size_t tags;
	size_t commits;
	size_t trees;
	size_t blobs;
};

கட்டமைப்பு object_stats {
	கட்டமைப்பு object_மதிப்புகள் type_counts;
	கட்டமைப்பு object_மதிப்புகள் inflated_sizes;
	கட்டமைப்பு object_மதிப்புகள் disk_sizes;
	கட்டமைப்பு largest_objects largest;
};

கட்டமைப்பு repo_structure {
	கட்டமைப்பு ref_stats refs;
	கட்டமைப்பு object_stats objects;
};

கட்டமைப்பு stats_table {
	கட்டமைப்பு string_list rows;
	கட்டமைப்பு string_list annotations;

	முழு name_col_width;
	முழு மதிப்பு_col_width;
	முழு unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
கட்டமைப்பு stats_table_entry {
	எழுத்து *மதிப்பு;
	மாறிலி எழுத்து *unit;
	size_t index;
	கட்டமைப்பு object_id *oid;
};

நிலையான வெறுமை stats_table_vaddf(கட்டமைப்பு stats_table *table,
			      கட்டமைப்பு stats_table_entry *entry,
			      மாறிலி எழுத்து *format, va_list ap)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு string_list_item *item;
	எழுத்து *formatted_name;
	முழு name_width;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, வெற்று);
	name_width = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&table->rows, formatted_name);
	item->util = entry;

	எனில் (name_width > table->name_col_width)
		table->name_col_width = name_width;
	எனில் (!entry)
		திரும்பு;
	எனில் (entry->oid) {
		entry->index = table->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)entry->index,
			    oid_to_hex(entry->oid));
		string_list_append_nodup(&table->annotations, strbuf_detach(&buf, வெற்று));
	}
	எனில் (entry->மதிப்பு) {
		முழு மதிப்பு_width = utf8_strwidth(entry->மதிப்பு);
		எனில் (மதிப்பு_width > table->மதிப்பு_col_width)
			table->மதிப்பு_col_width = மதிப்பு_width;
	}
	எனில் (entry->unit) {
		முழு unit_width = utf8_strwidth(entry->unit);
		எனில் (unit_width > table->unit_col_width)
			table->unit_col_width = unit_width;
	}

	strbuf_release(&buf);
}

நிலையான வெறுமை stats_table_addf(கட்டமைப்பு stats_table *table, மாறிலி எழுத்து *format, ...)
{
	va_list ap;

	va_start(ap, format);
	stats_table_vaddf(table, வெற்று, format, ap);
	va_end(ap);
}

நிலையான வெறுமை stats_table_மொத்தம்_addf(கட்டமைப்பு stats_table *table, size_t மதிப்பு,
				   மாறிலி எழுத்து *format, ...)
{
	கட்டமைப்பு stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_மொத்தம்(மதிப்பு, &entry->மதிப்பு, &entry->unit);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

நிலையான வெறுமை stats_table_object_மொத்தம்_addf(கட்டமைப்பு stats_table *table,
					  கட்டமைப்பு object_id *oid, size_t மதிப்பு,
					  மாறிலி எழுத்து *format, ...)
{
	கட்டமைப்பு stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_மொத்தம்(மதிப்பு, &entry->மதிப்பு, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	எனில் (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

நிலையான வெறுமை stats_table_size_addf(கட்டமைப்பு stats_table *table, size_t மதிப்பு,
				  மாறிலி எழுத்து *format, ...)
{
	கட்டமைப்பு stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(மதிப்பு, &entry->மதிப்பு, &entry->unit, HUMANISE_COMPACT);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

நிலையான வெறுமை stats_table_object_size_addf(கட்டமைப்பு stats_table *table,
					 கட்டமைப்பு object_id *oid, size_t மதிப்பு,
					 மாறிலி எழுத்து *format, ...)
{
	கட்டமைப்பு stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(மதிப்பு, &entry->மதிப்பு, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	எனில் (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

நிலையான உள்வரி size_t get_total_reference_மொத்தம்(கட்டமைப்பு ref_stats *stats)
{
	திரும்பு stats->branches + stats->remotes + stats->tags + stats->others;
}

நிலையான உள்வரி size_t get_total_object_மதிப்புகள்(கட்டமைப்பு object_மதிப்புகள் *மதிப்புகள்)
{
	திரும்பு மதிப்புகள்->tags + மதிப்புகள்->commits + மதிப்புகள்->trees + மதிப்புகள்->blobs;
}

நிலையான வெறுமை stats_table_setup_structure(கட்டமைப்பு stats_table *table,
					கட்டமைப்பு repo_structure *stats)
{
	கட்டமைப்பு object_stats *objects = &stats->objects;
	கட்டமைப்பு ref_stats *refs = &stats->refs;
	size_t inflated_object_total;
	size_t object_மொத்தம்_total;
	size_t disk_object_total;
	size_t ref_total;

	ref_total = get_total_reference_மொத்தம்(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_மொத்தம்_addf(table, ref_total, "  * %s", _("Count"));
	stats_table_மொத்தம்_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_மொத்தம்_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_மொத்தம்_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_மொத்தம்_addf(table, refs->others, "    * %s", _("Others"));

	object_மொத்தம்_total = get_total_object_மதிப்புகள்(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_மொத்தம்_addf(table, object_மொத்தம்_total, "  * %s", _("Count"));
	stats_table_மொத்தம்_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_மொத்தம்_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_மொத்தம்_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_மொத்தம்_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_total = get_total_object_மதிப்புகள்(&objects->inflated_sizes);
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

	disk_object_total = get_total_object_மதிப்புகள்(&objects->disk_sizes);
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
				     objects->largest.commit_size.மதிப்பு,
				     "    * %s", _("Maximum size"));
	stats_table_object_மொத்தம்_addf(table,
				      &objects->largest.parent_மொத்தம்.oid,
				      objects->largest.parent_மொத்தம்.மதிப்பு,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_size_addf(table,
				     &objects->largest.மரம்_size.oid,
				     objects->largest.மரம்_size.மதிப்பு,
				     "    * %s", _("Maximum size"));
	stats_table_object_மொத்தம்_addf(table,
				      &objects->largest.மரம்_entries.oid,
				      objects->largest.மரம்_entries.மதிப்பு,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_size_addf(table,
				     &objects->largest.blob_size.oid,
				     objects->largest.blob_size.மதிப்பு,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_size_addf(table,
				     &objects->largest.tag_size.oid,
				     objects->largest.tag_size.மதிப்பு,
				     "    * %s", _("Maximum size"));
}

#define INDEX_WIDTH 4

நிலையான வெறுமை stats_table_print_structure(மாறிலி கட்டமைப்பு stats_table *table)
{
	மாறிலி எழுத்து *name_col_title = _("Repository structure");
	மாறிலி எழுத்து *மதிப்பு_col_title = _("Value");
	முழு title_name_width = utf8_strwidth(name_col_title);
	முழு title_மதிப்பு_width = utf8_strwidth(மதிப்பு_col_title);
	முழு name_col_width = table->name_col_width;
	முழு மதிப்பு_col_width = table->மதிப்பு_col_width;
	முழு unit_col_width = table->unit_col_width;
	கட்டமைப்பு string_list_item *item;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;

	எனில் (title_name_width > name_col_width)
		name_col_width = title_name_width;
	எனில் (title_மதிப்பு_width > மதிப்பு_col_width + unit_col_width + 1)
		மதிப்பு_col_width = title_மதிப்பு_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_இடது, name_col_width + INDEX_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_இடது,
			  மதிப்பு_col_width + unit_col_width + 1, மதிப்பு_col_title);
	strbuf_addstr(&buf, " |");
	printf("%s\n", buf.buf);

	printf("| ");
	க்கு (முழு i = 0; i < name_col_width + INDEX_WIDTH; i++)
		putchar('-');
	printf(" | ");
	க்கு (முழு i = 0; i < மதிப்பு_col_width + unit_col_width + 1; i++)
		putchar('-');
	printf(" |\n");

	for_each_string_list_item(item, &table->rows) {
		கட்டமைப்பு stats_table_entry *entry = item->util;
		மாறிலி எழுத்து *மதிப்பு = "";
		மாறிலி எழுத்து *unit = "";

		எனில் (entry) {
			மதிப்பு = entry->மதிப்பு;
			எனில் (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_இடது, name_col_width, item->string);

		எனில் (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->index);
		இல்லையெனில்
			strbuf_addchars(&buf, ' ', INDEX_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_வலது, மதிப்பு_col_width, மதிப்பு);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_இடது, unit_col_width, unit);
		strbuf_addstr(&buf, " |");
		printf("%s\n", buf.buf);
	}

	எனில் (table->annotations.nr) {
		printf("\n");
		for_each_string_list_item(item, &table->annotations)
			printf("%s\n", item->string);
	}

	strbuf_release(&buf);
}

நிலையான வெறுமை stats_table_clear(கட்டமைப்பு stats_table *table)
{
	கட்டமைப்பு stats_table_entry *entry;
	கட்டமைப்பு string_list_item *item;

	for_each_string_list_item(item, &table->rows) {
		entry = item->util;
		எனில் (entry)
			விடுவி(entry->மதிப்பு);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

நிலையான உள்வரி வெறுமை print_keyvalue(மாறிலி எழுத்து *சாவி, எழுத்து சாவி_delim, size_t மதிப்பு,
				  எழுத்து மதிப்பு_delim)
{
	printf("%s%c%" PRIuMAX "%c", சாவி, சாவி_delim, (uintmax_t)மதிப்பு,
	       மதிப்பு_delim);
}

நிலையான வெறுமை print_object_data(மாறிலி எழுத்து *சாவி, எழுத்து சாவி_delim,
			      கட்டமைப்பு object_data *data, எழுத்து மதிப்பு_delim)
{
	print_keyvalue(சாவி, சாவி_delim, data->மதிப்பு, மதிப்பு_delim);
	printf("%s_oid%c%s%c", சாவி, சாவி_delim, oid_to_hex(&data->oid),
	       மதிப்பு_delim);
}

நிலையான வெறுமை structure_keyvalue_print(கட்டமைப்பு repo_structure *stats,
				     எழுத்து சாவி_delim, எழுத்து மதிப்பு_delim)
{
	print_keyvalue("references.branches.count", சாவி_delim,
		       stats->refs.branches, மதிப்பு_delim);
	print_keyvalue("references.tags.count", சாவி_delim,
		       stats->refs.tags, மதிப்பு_delim);
	print_keyvalue("references.remotes.count", சாவி_delim,
		       stats->refs.remotes, மதிப்பு_delim);
	print_keyvalue("references.others.count", சாவி_delim,
		       stats->refs.others, மதிப்பு_delim);

	print_keyvalue("objects.commits.count", சாவி_delim,
		       stats->objects.type_counts.commits, மதிப்பு_delim);
	print_keyvalue("objects.trees.count", சாவி_delim,
		       stats->objects.type_counts.trees, மதிப்பு_delim);
	print_keyvalue("objects.blobs.count", சாவி_delim,
		       stats->objects.type_counts.blobs, மதிப்பு_delim);
	print_keyvalue("objects.tags.count", சாவி_delim,
		       stats->objects.type_counts.tags, மதிப்பு_delim);

	print_keyvalue("objects.commits.inflated_size", சாவி_delim,
		       stats->objects.inflated_sizes.commits, மதிப்பு_delim);
	print_keyvalue("objects.trees.inflated_size", சாவி_delim,
		       stats->objects.inflated_sizes.trees, மதிப்பு_delim);
	print_keyvalue("objects.blobs.inflated_size", சாவி_delim,
		       stats->objects.inflated_sizes.blobs, மதிப்பு_delim);
	print_keyvalue("objects.tags.inflated_size", சாவி_delim,
		       stats->objects.inflated_sizes.tags, மதிப்பு_delim);

	print_keyvalue("objects.commits.disk_size", சாவி_delim,
		       stats->objects.disk_sizes.commits, மதிப்பு_delim);
	print_keyvalue("objects.trees.disk_size", சாவி_delim,
		       stats->objects.disk_sizes.trees, மதிப்பு_delim);
	print_keyvalue("objects.blobs.disk_size", சாவி_delim,
		       stats->objects.disk_sizes.blobs, மதிப்பு_delim);
	print_keyvalue("objects.tags.disk_size", சாவி_delim,
		       stats->objects.disk_sizes.tags, மதிப்பு_delim);

	print_object_data("objects.commits.max_size", சாவி_delim,
			  &stats->objects.largest.commit_size, மதிப்பு_delim);
	print_object_data("objects.trees.max_size", சாவி_delim,
			  &stats->objects.largest.மரம்_size, மதிப்பு_delim);
	print_object_data("objects.blobs.max_size", சாவி_delim,
			  &stats->objects.largest.blob_size, மதிப்பு_delim);
	print_object_data("objects.tags.max_size", சாவி_delim,
			  &stats->objects.largest.tag_size, மதிப்பு_delim);

	print_object_data("objects.commits.max_parents", சாவி_delim,
			  &stats->objects.largest.parent_மொத்தம், மதிப்பு_delim);
	print_object_data("objects.trees.max_entries", சாவி_delim,
			  &stats->objects.largest.மரம்_entries, மதிப்பு_delim);

	fflush(stdout);
}

கட்டமைப்பு மொத்தம்_references_data {
	கட்டமைப்பு ref_stats *stats;
	கட்டமைப்பு rev_info *revs;
	கட்டமைப்பு progress *progress;
};

நிலையான முழு மொத்தம்_references(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	கட்டமைப்பு மொத்தம்_references_data *data = cb_data;
	கட்டமைப்பு ref_stats *stats = data->stats;
	size_t ref_மொத்தம்;

	தேர்வு (ref_kind_from_refname(ref->name)) {
	நிலை FILTER_REFS_BRANCHES:
		stats->branches++;
		நிறுத்து;
	நிலை FILTER_REFS_REMOTES:
		stats->remotes++;
		நிறுத்து;
	நிலை FILTER_REFS_TAGS:
		stats->tags++;
		நிறுத்து;
	நிலை FILTER_REFS_OTHERS:
		stats->others++;
		நிறுத்து;
	இயல்பு:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	add_pending_oid(data->revs, வெற்று, ref->oid, 0);

	ref_மொத்தம் = get_total_reference_மொத்தம்(stats);
	display_progress(data->progress, ref_மொத்தம்);

	திரும்பு 0;
}

நிலையான வெறுமை structure_மொத்தம்_references(கட்டமைப்பு ref_stats *stats,
				       கட்டமைப்பு rev_info *revs,
				       கட்டமைப்பு repository *repo,
				       முழு show_progress)
{
	கட்டமைப்பு மொத்தம்_references_data data = {
		.stats = stats,
		.revs = revs,
	};

	எனில் (show_progress)
		data.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), மொத்தம்_references, &data);
	stop_progress(&data.progress);
}

கட்டமைப்பு மொத்தம்_objects_data {
	கட்டமைப்பு object_database *odb;
	கட்டமைப்பு object_stats *stats;
	கட்டமைப்பு progress *progress;
};

நிலையான வெறுமை check_largest(கட்டமைப்பு object_data *data, கட்டமைப்பு object_id *oid,
			  size_t மதிப்பு)
{
	எனில் (மதிப்பு > data->மதிப்பு || is_null_oid(&data->oid)) {
		oidcpy(&data->oid, oid);
		data->மதிப்பு = மதிப்பு;
	}
}

நிலையான size_t மொத்தம்_மரம்_entries(கட்டமைப்பு object *obj)
{
	கட்டமைப்பு மரம் *t = object_as_type(obj, OBJ_மரம், 0);
	கட்டமைப்பு name_entry entry;
	கட்டமைப்பு மரம்_desc desc;
	size_t மொத்தம் = 0;

	init_மரம்_desc(&desc, &t->object.oid, t->buffer, t->size);
	போது (மரம்_entry(&desc, &entry))
		மொத்தம்++;

	திரும்பு மொத்தம்;
}

நிலையான முழு மொத்தம்_objects(மாறிலி எழுத்து *path UNUSED, கட்டமைப்பு oid_array *oids,
			 எண்ணிக்கை object_type type, வெறுமை *cb_data)
{
	கட்டமைப்பு மொத்தம்_objects_data *data = cb_data;
	கட்டமைப்பு object_stats *stats = data->stats;
	size_t object_மொத்தம்;

	க்கு (size_t i = 0; i < oids->nr; i++) {
		கட்டமைப்பு object_info oi = OBJECT_INFO_INIT;
		unsigned long inflated;
		size_t inflated_st = 0;
		கட்டமைப்பு commit *commit;
		கட்டமைப்பு object *obj;
		வெறுமை *content;
		off_t disk;
		முழு eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		எனில் (odb_read_object_info_extended(data->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_QUICK) < 0)
			தொடர்;
		inflated = cast_size_t_to_ulong(inflated_st);

		obj = parse_object_buffer(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		தேர்வு (type) {
		நிலை OBJ_TAG:
			stats->type_counts.tags++;
			stats->inflated_sizes.tags += inflated;
			stats->disk_sizes.tags += disk;
			check_largest(&stats->largest.tag_size, &oids->oid[i],
				      inflated);
			நிறுத்து;
		நிலை OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_மொத்தம், &oids->oid[i],
				      commit_list_மொத்தம்(commit->parents));
			நிறுத்து;
		நிலை OBJ_மரம்:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.மரம்_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.மரம்_entries, &oids->oid[i],
				      மொத்தம்_மரம்_entries(obj));
			நிறுத்து;
		நிலை OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_size, &oids->oid[i],
				      inflated);
			நிறுத்து;
		இயல்பு:
			BUG("invalid object type");
		}

		எனில் (!eaten)
			விடுவி(content);
	}

	object_மொத்தம் = get_total_object_மதிப்புகள்(&stats->type_counts);
	display_progress(data->progress, object_மொத்தம்);

	திரும்பு 0;
}

நிலையான வெறுமை structure_மொத்தம்_objects(கட்டமைப்பு object_stats *stats,
				    கட்டமைப்பு rev_info *revs,
				    கட்டமைப்பு repository *repo, முழு show_progress)
{
	கட்டமைப்பு path_walk_info info = PATH_WALK_INFO_INIT;
	கட்டமைப்பு மொத்தம்_objects_data data = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = மொத்தம்_objects;
	info.path_fn_data = &data;

	எனில் (show_progress)
		data.progress = start_delayed_progress(repo, _("Counting objects"), 0);

	walk_objects_by_path(&info);
	path_walk_info_clear(&info);
	stop_progress(&data.progress);
}

நிலையான முழு cmd_repo_structure(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			      கட்டமைப்பு repository *repo)
{
	கட்டமைப்பு stats_table table = {
		.rows = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	எண்ணிக்கை output_format format = FORMAT_TABLE;
	கட்டமைப்பு repo_structure stats = { 0 };
	கட்டமைப்பு rev_info revs;
	முழு show_progress = -1;
	கட்டமைப்பு option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', வெற்று, &format, வெற்று,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_progress, N_("show progress")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_structure_usage, 0);
	எனில் (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, prefix);

	எனில் (show_progress < 0)
		show_progress = isatty(2);

	structure_மொத்தம்_references(&stats.refs, &revs, repo, show_progress);
	structure_மொத்தம்_objects(&stats.objects, &revs, repo, show_progress);

	தேர்வு (format) {
	நிலை FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_print_structure(&table);
		நிறுத்து;
	நிலை FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		நிறுத்து;
	நிலை FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		நிறுத்து;
	இயல்பு:
		BUG("invalid output format");
	}

	stats_table_clear(&table);
	release_revisions(&revs);

	திரும்பு 0;
}

முழு cmd_repo(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
	     கட்டமைப்பு repository *repo)
{
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_usage, 0);

	திரும்பு fn(argc, argv, prefix, repo);
}
