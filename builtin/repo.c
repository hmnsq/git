#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "path-walk.h"
#അന്തര്ഭാവ "progress.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "ref-filter.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "shallow.h"
#അന്തര്ഭാവ "tree.h"
#അന്തര്ഭാവ "tree-walk.h"
#അന്തര്ഭാവ "utf8.h"

#define REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#define REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	ശൂന്യ,
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം repo_info_usage[] = {
	REPO_INFO_USAGE,
	ശൂന്യ,
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	ശൂന്യ,
};

തരനിർവചനം പൂർണ്ണസംഖ്യ get_മൂല്യം_fn(ഘടന repository *repo, ഘടന strbuf *buf);

എണ്ണൽ output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

ഘടന repo_info_field {
	സ്ഥിരം അക്ഷരം *താക്കോൽ;
	get_മൂല്യം_fn *get_മൂല്യം;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ get_layout_bare(ഘടന repository *repo UNUSED, ഘടന strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_layout_shallow(ഘടന repository *repo, ഘടന strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_object_format(ഘടന repository *repo, ഘടന strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_path_commondir_absolute(ഘടന repository *repo, ഘടന strbuf *buf)
{
	സ്ഥിരം അക്ഷരം *common_dir = repo_get_common_dir(repo);

	എങ്കിൽ (!common_dir)
		തിരികെ error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_path_commondir_relative(ഘടന repository *repo, ഘടന strbuf *buf)
{
	സ്ഥിരം അക്ഷരം *common_dir = repo_get_common_dir(repo);

	എങ്കിൽ (!common_dir)
		തിരികെ error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_path_gitdir_absolute(ഘടന repository *repo, ഘടന strbuf *buf)
{
	സ്ഥിരം അക്ഷരം *git_dir = repo_get_git_dir(repo);

	എങ്കിൽ (!git_dir)
		തിരികെ error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_path_gitdir_relative(ഘടന repository *repo, ഘടന strbuf *buf)
{
	സ്ഥിരം അക്ഷരം *git_dir = repo_get_git_dir(repo);

	എങ്കിൽ (!git_dir)
		തിരികെ error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_references_format(ഘടന repository *repo, ഘടന strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_to_name(repo->ref_storage_format));
	തിരികെ 0;
}

/* repo_info_field keys must be in lexicographical order */
സ്ഥിതം സ്ഥിരം ഘടന repo_info_field repo_info_field[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_path_commondir_absolute },
	{ "path.commondir.relative", get_path_commondir_relative },
	{ "path.gitdir.absolute", get_path_gitdir_absolute },
	{ "path.gitdir.relative", get_path_gitdir_relative },
	{ "references.format", get_references_format },
};

സ്ഥിതം പൂർണ്ണസംഖ്യ repo_info_field_cmp(സ്ഥിരം ശൂന്യം *va, സ്ഥിരം ശൂന്യം *vb)
{
	സ്ഥിരം ഘടന repo_info_field *a = va;
	സ്ഥിരം ഘടന repo_info_field *b = vb;

	തിരികെ strcmp(a->താക്കോൽ, b->താക്കോൽ);
}

സ്ഥിതം സ്ഥിരം ഘടന repo_info_field *get_repo_info_field(സ്ഥിരം അക്ഷരം *താക്കോൽ)
{
	സ്ഥിരം ഘടന repo_info_field തിരയൽ_താക്കോൽ = { താക്കോൽ, ശൂന്യ };
	സ്ഥിരം ഘടന repo_info_field *found = bsearch(&തിരയൽ_താക്കോൽ,
						      repo_info_field,
						      ARRAY_SIZE(repo_info_field),
						      വലുപ്പം(*found),
						      repo_info_field_cmp);

	തിരികെ found;
}

സ്ഥിതം ശൂന്യം print_field(എണ്ണൽ output_format format, സ്ഥിരം അക്ഷരം *താക്കോൽ,
			സ്ഥിരം അക്ഷരം *മൂല്യം)
{
	തിരഞ്ഞെടുപ്പ് (format) {
	അവസ്ഥ FORMAT_NEWLINE_TERMINATED:
		printf("%s=", താക്കോൽ);
		quote_c_style(മൂല്യം, ശൂന്യ, stdout, 0);
		putchar('\n');
		നിർത്തുക;
	അവസ്ഥ FORMAT_NUL_TERMINATED:
		printf("%s\n%s%c", താക്കോൽ, മൂല്യം, '\0');
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("not a valid output format: %d", format);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ print_fields(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv,
			ഘടന repository *repo,
			എണ്ണൽ output_format format)
{
	പൂർണ്ണസംഖ്യ ret = 0;
	ഘടന strbuf valbuf = STRBUF_INIT;

	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *താക്കോൽ = argv[i];
		സ്ഥിരം ഘടന repo_info_field *field = get_repo_info_field(താക്കോൽ);

		എങ്കിൽ (!field) {
			ret = error(_("key '%s' not found"), താക്കോൽ);
			തുടരുക;
		}

		strbuf_reset(&valbuf);
		field->get_മൂല്യം(repo, &valbuf);
		print_field(format, താക്കോൽ, valbuf.buf);
	}

	strbuf_release(&valbuf);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ print_all_fields(ഘടന repository *repo,
			    എണ്ണൽ output_format format)
{
	ഘടന strbuf valbuf = STRBUF_INIT;

	വേണ്ടി (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		സ്ഥിരം ഘടന repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_മൂല്യം(repo, &valbuf);
		print_field(format, field->താക്കോൽ, valbuf.buf);
	}

	strbuf_release(&valbuf);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ print_താക്കോലുകൾ(എണ്ണൽ output_format format)
{
	അക്ഷരം sep;

	തിരഞ്ഞെടുപ്പ് (format) {
	അവസ്ഥ FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		നിർത്തുക;
	അവസ്ഥ FORMAT_NUL_TERMINATED:
		sep = '\0';
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	വേണ്ടി (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		സ്ഥിരം ഘടന repo_info_field *field = &repo_info_field[i];
		printf("%s%c", field->താക്കോൽ, sep);
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_format_cb(സ്ഥിരം ഘടന option *opt,
			   സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset UNUSED)
{
	എണ്ണൽ output_format *format = opt->മൂല്യം;

	എങ്കിൽ (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "table"))
		*format = FORMAT_TABLE;
	അല്ലെങ്കിൽ
		die(_("invalid format '%s'"), arg);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_repo_info(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			 ഘടന repository *repo)
{
	എണ്ണൽ output_format format = FORMAT_NEWLINE_TERMINATED;
	പൂർണ്ണസംഖ്യ all_താക്കോലുകൾ = 0;
	പൂർണ്ണസംഖ്യ show_താക്കോലുകൾ = 0;
	ഘടന option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ശൂന്യ, &format, ശൂന്യ,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_താക്കോലുകൾ, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_താക്കോലുകൾ, N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	എങ്കിൽ (show_താക്കോലുകൾ && (all_താക്കോലുകൾ || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	എങ്കിൽ (show_താക്കോലുകൾ)
		തിരികെ print_താക്കോലുകൾ(format);

	എങ്കിൽ (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	എങ്കിൽ (all_താക്കോലുകൾ && argc)
		die(_("--all and <key> cannot be used together"));

	എങ്കിൽ (all_താക്കോലുകൾ)
		തിരികെ print_all_fields(repo, format);
	അല്ലെങ്കിൽ
		തിരികെ print_fields(argc, argv, repo, format);
}

ഘടന object_data {
	ഘടന object_id oid;
	size_t മൂല്യം;
};

ഘടന largest_objects {
	ഘടന object_data tag_size;
	ഘടന object_data commit_size;
	ഘടന object_data വൃക്ഷം_size;
	ഘടന object_data blob_size;

	ഘടന object_data parent_എണ്ണം;
	ഘടന object_data വൃക്ഷം_entries;
};

ഘടന ref_stats {
	size_t branches;
	size_t remotes;
	size_t tags;
	size_t others;
};

ഘടന object_മൂല്യങ്ങൾ {
	size_t tags;
	size_t commits;
	size_t trees;
	size_t blobs;
};

ഘടന object_stats {
	ഘടന object_മൂല്യങ്ങൾ type_counts;
	ഘടന object_മൂല്യങ്ങൾ inflated_sizes;
	ഘടന object_മൂല്യങ്ങൾ disk_sizes;
	ഘടന largest_objects largest;
};

ഘടന repo_structure {
	ഘടന ref_stats refs;
	ഘടന object_stats objects;
};

ഘടന stats_table {
	ഘടന string_list rows;
	ഘടന string_list annotations;

	പൂർണ്ണസംഖ്യ name_col_width;
	പൂർണ്ണസംഖ്യ മൂല്യം_col_width;
	പൂർണ്ണസംഖ്യ unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
ഘടന stats_table_entry {
	അക്ഷരം *മൂല്യം;
	സ്ഥിരം അക്ഷരം *unit;
	size_t index;
	ഘടന object_id *oid;
};

സ്ഥിതം ശൂന്യം stats_table_vaddf(ഘടന stats_table *table,
			      ഘടന stats_table_entry *entry,
			      സ്ഥിരം അക്ഷരം *format, va_list ap)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന string_list_item *item;
	അക്ഷരം *formatted_name;
	പൂർണ്ണസംഖ്യ name_width;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, ശൂന്യ);
	name_width = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&table->rows, formatted_name);
	item->util = entry;

	എങ്കിൽ (name_width > table->name_col_width)
		table->name_col_width = name_width;
	എങ്കിൽ (!entry)
		തിരികെ;
	എങ്കിൽ (entry->oid) {
		entry->index = table->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)entry->index,
			    oid_to_hex(entry->oid));
		string_list_append_nodup(&table->annotations, strbuf_detach(&buf, ശൂന്യ));
	}
	എങ്കിൽ (entry->മൂല്യം) {
		പൂർണ്ണസംഖ്യ മൂല്യം_width = utf8_strwidth(entry->മൂല്യം);
		എങ്കിൽ (മൂല്യം_width > table->മൂല്യം_col_width)
			table->മൂല്യം_col_width = മൂല്യം_width;
	}
	എങ്കിൽ (entry->unit) {
		പൂർണ്ണസംഖ്യ unit_width = utf8_strwidth(entry->unit);
		എങ്കിൽ (unit_width > table->unit_col_width)
			table->unit_col_width = unit_width;
	}

	strbuf_release(&buf);
}

സ്ഥിതം ശൂന്യം stats_table_addf(ഘടന stats_table *table, സ്ഥിരം അക്ഷരം *format, ...)
{
	va_list ap;

	va_start(ap, format);
	stats_table_vaddf(table, ശൂന്യ, format, ap);
	va_end(ap);
}

സ്ഥിതം ശൂന്യം stats_table_എണ്ണം_addf(ഘടന stats_table *table, size_t മൂല്യം,
				   സ്ഥിരം അക്ഷരം *format, ...)
{
	ഘടന stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_എണ്ണം(മൂല്യം, &entry->മൂല്യം, &entry->unit);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

സ്ഥിതം ശൂന്യം stats_table_object_എണ്ണം_addf(ഘടന stats_table *table,
					  ഘടന object_id *oid, size_t മൂല്യം,
					  സ്ഥിരം അക്ഷരം *format, ...)
{
	ഘടന stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_എണ്ണം(മൂല്യം, &entry->മൂല്യം, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	എങ്കിൽ (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

സ്ഥിതം ശൂന്യം stats_table_size_addf(ഘടന stats_table *table, size_t മൂല്യം,
				  സ്ഥിരം അക്ഷരം *format, ...)
{
	ഘടന stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(മൂല്യം, &entry->മൂല്യം, &entry->unit, HUMANISE_COMPACT);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

സ്ഥിതം ശൂന്യം stats_table_object_size_addf(ഘടന stats_table *table,
					 ഘടന object_id *oid, size_t മൂല്യം,
					 സ്ഥിരം അക്ഷരം *format, ...)
{
	ഘടന stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(മൂല്യം, &entry->മൂല്യം, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	എങ്കിൽ (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

സ്ഥിതം വരിയിൽ size_t get_total_reference_എണ്ണം(ഘടന ref_stats *stats)
{
	തിരികെ stats->branches + stats->remotes + stats->tags + stats->others;
}

സ്ഥിതം വരിയിൽ size_t get_total_object_മൂല്യങ്ങൾ(ഘടന object_മൂല്യങ്ങൾ *മൂല്യങ്ങൾ)
{
	തിരികെ മൂല്യങ്ങൾ->tags + മൂല്യങ്ങൾ->commits + മൂല്യങ്ങൾ->trees + മൂല്യങ്ങൾ->blobs;
}

സ്ഥിതം ശൂന്യം stats_table_setup_structure(ഘടന stats_table *table,
					ഘടന repo_structure *stats)
{
	ഘടന object_stats *objects = &stats->objects;
	ഘടന ref_stats *refs = &stats->refs;
	size_t inflated_object_total;
	size_t object_എണ്ണം_total;
	size_t disk_object_total;
	size_t ref_total;

	ref_total = get_total_reference_എണ്ണം(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_എണ്ണം_addf(table, ref_total, "  * %s", _("Count"));
	stats_table_എണ്ണം_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_എണ്ണം_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_എണ്ണം_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_എണ്ണം_addf(table, refs->others, "    * %s", _("Others"));

	object_എണ്ണം_total = get_total_object_മൂല്യങ്ങൾ(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_എണ്ണം_addf(table, object_എണ്ണം_total, "  * %s", _("Count"));
	stats_table_എണ്ണം_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_എണ്ണം_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_എണ്ണം_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_എണ്ണം_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_total = get_total_object_മൂല്യങ്ങൾ(&objects->inflated_sizes);
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

	disk_object_total = get_total_object_മൂല്യങ്ങൾ(&objects->disk_sizes);
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
				     objects->largest.commit_size.മൂല്യം,
				     "    * %s", _("Maximum size"));
	stats_table_object_എണ്ണം_addf(table,
				      &objects->largest.parent_എണ്ണം.oid,
				      objects->largest.parent_എണ്ണം.മൂല്യം,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_size_addf(table,
				     &objects->largest.വൃക്ഷം_size.oid,
				     objects->largest.വൃക്ഷം_size.മൂല്യം,
				     "    * %s", _("Maximum size"));
	stats_table_object_എണ്ണം_addf(table,
				      &objects->largest.വൃക്ഷം_entries.oid,
				      objects->largest.വൃക്ഷം_entries.മൂല്യം,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_size_addf(table,
				     &objects->largest.blob_size.oid,
				     objects->largest.blob_size.മൂല്യം,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_size_addf(table,
				     &objects->largest.tag_size.oid,
				     objects->largest.tag_size.മൂല്യം,
				     "    * %s", _("Maximum size"));
}

#define INDEX_WIDTH 4

സ്ഥിതം ശൂന്യം stats_table_print_structure(സ്ഥിരം ഘടന stats_table *table)
{
	സ്ഥിരം അക്ഷരം *name_col_title = _("Repository structure");
	സ്ഥിരം അക്ഷരം *മൂല്യം_col_title = _("Value");
	പൂർണ്ണസംഖ്യ title_name_width = utf8_strwidth(name_col_title);
	പൂർണ്ണസംഖ്യ title_മൂല്യം_width = utf8_strwidth(മൂല്യം_col_title);
	പൂർണ്ണസംഖ്യ name_col_width = table->name_col_width;
	പൂർണ്ണസംഖ്യ മൂല്യം_col_width = table->മൂല്യം_col_width;
	പൂർണ്ണസംഖ്യ unit_col_width = table->unit_col_width;
	ഘടന string_list_item *item;
	ഘടന strbuf buf = STRBUF_INIT;

	എങ്കിൽ (title_name_width > name_col_width)
		name_col_width = title_name_width;
	എങ്കിൽ (title_മൂല്യം_width > മൂല്യം_col_width + unit_col_width + 1)
		മൂല്യം_col_width = title_മൂല്യം_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_ഇടത്, name_col_width + INDEX_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_ഇടത്,
			  മൂല്യം_col_width + unit_col_width + 1, മൂല്യം_col_title);
	strbuf_addstr(&buf, " |");
	printf("%s\n", buf.buf);

	printf("| ");
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < name_col_width + INDEX_WIDTH; i++)
		putchar('-');
	printf(" | ");
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < മൂല്യം_col_width + unit_col_width + 1; i++)
		putchar('-');
	printf(" |\n");

	for_each_string_list_item(item, &table->rows) {
		ഘടന stats_table_entry *entry = item->util;
		സ്ഥിരം അക്ഷരം *മൂല്യം = "";
		സ്ഥിരം അക്ഷരം *unit = "";

		എങ്കിൽ (entry) {
			മൂല്യം = entry->മൂല്യം;
			എങ്കിൽ (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_ഇടത്, name_col_width, item->string);

		എങ്കിൽ (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->index);
		അല്ലെങ്കിൽ
			strbuf_addchars(&buf, ' ', INDEX_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_വലത്, മൂല്യം_col_width, മൂല്യം);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_ഇടത്, unit_col_width, unit);
		strbuf_addstr(&buf, " |");
		printf("%s\n", buf.buf);
	}

	എങ്കിൽ (table->annotations.nr) {
		printf("\n");
		for_each_string_list_item(item, &table->annotations)
			printf("%s\n", item->string);
	}

	strbuf_release(&buf);
}

സ്ഥിതം ശൂന്യം stats_table_clear(ഘടന stats_table *table)
{
	ഘടന stats_table_entry *entry;
	ഘടന string_list_item *item;

	for_each_string_list_item(item, &table->rows) {
		entry = item->util;
		എങ്കിൽ (entry)
			സ്വതന്ത്രം(entry->മൂല്യം);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

സ്ഥിതം വരിയിൽ ശൂന്യം print_keyvalue(സ്ഥിരം അക്ഷരം *താക്കോൽ, അക്ഷരം താക്കോൽ_delim, size_t മൂല്യം,
				  അക്ഷരം മൂല്യം_delim)
{
	printf("%s%c%" PRIuMAX "%c", താക്കോൽ, താക്കോൽ_delim, (uintmax_t)മൂല്യം,
	       മൂല്യം_delim);
}

സ്ഥിതം ശൂന്യം print_object_data(സ്ഥിരം അക്ഷരം *താക്കോൽ, അക്ഷരം താക്കോൽ_delim,
			      ഘടന object_data *data, അക്ഷരം മൂല്യം_delim)
{
	print_keyvalue(താക്കോൽ, താക്കോൽ_delim, data->മൂല്യം, മൂല്യം_delim);
	printf("%s_oid%c%s%c", താക്കോൽ, താക്കോൽ_delim, oid_to_hex(&data->oid),
	       മൂല്യം_delim);
}

സ്ഥിതം ശൂന്യം structure_keyvalue_print(ഘടന repo_structure *stats,
				     അക്ഷരം താക്കോൽ_delim, അക്ഷരം മൂല്യം_delim)
{
	print_keyvalue("references.branches.count", താക്കോൽ_delim,
		       stats->refs.branches, മൂല്യം_delim);
	print_keyvalue("references.tags.count", താക്കോൽ_delim,
		       stats->refs.tags, മൂല്യം_delim);
	print_keyvalue("references.remotes.count", താക്കോൽ_delim,
		       stats->refs.remotes, മൂല്യം_delim);
	print_keyvalue("references.others.count", താക്കോൽ_delim,
		       stats->refs.others, മൂല്യം_delim);

	print_keyvalue("objects.commits.count", താക്കോൽ_delim,
		       stats->objects.type_counts.commits, മൂല്യം_delim);
	print_keyvalue("objects.trees.count", താക്കോൽ_delim,
		       stats->objects.type_counts.trees, മൂല്യം_delim);
	print_keyvalue("objects.blobs.count", താക്കോൽ_delim,
		       stats->objects.type_counts.blobs, മൂല്യം_delim);
	print_keyvalue("objects.tags.count", താക്കോൽ_delim,
		       stats->objects.type_counts.tags, മൂല്യം_delim);

	print_keyvalue("objects.commits.inflated_size", താക്കോൽ_delim,
		       stats->objects.inflated_sizes.commits, മൂല്യം_delim);
	print_keyvalue("objects.trees.inflated_size", താക്കോൽ_delim,
		       stats->objects.inflated_sizes.trees, മൂല്യം_delim);
	print_keyvalue("objects.blobs.inflated_size", താക്കോൽ_delim,
		       stats->objects.inflated_sizes.blobs, മൂല്യം_delim);
	print_keyvalue("objects.tags.inflated_size", താക്കോൽ_delim,
		       stats->objects.inflated_sizes.tags, മൂല്യം_delim);

	print_keyvalue("objects.commits.disk_size", താക്കോൽ_delim,
		       stats->objects.disk_sizes.commits, മൂല്യം_delim);
	print_keyvalue("objects.trees.disk_size", താക്കോൽ_delim,
		       stats->objects.disk_sizes.trees, മൂല്യം_delim);
	print_keyvalue("objects.blobs.disk_size", താക്കോൽ_delim,
		       stats->objects.disk_sizes.blobs, മൂല്യം_delim);
	print_keyvalue("objects.tags.disk_size", താക്കോൽ_delim,
		       stats->objects.disk_sizes.tags, മൂല്യം_delim);

	print_object_data("objects.commits.max_size", താക്കോൽ_delim,
			  &stats->objects.largest.commit_size, മൂല്യം_delim);
	print_object_data("objects.trees.max_size", താക്കോൽ_delim,
			  &stats->objects.largest.വൃക്ഷം_size, മൂല്യം_delim);
	print_object_data("objects.blobs.max_size", താക്കോൽ_delim,
			  &stats->objects.largest.blob_size, മൂല്യം_delim);
	print_object_data("objects.tags.max_size", താക്കോൽ_delim,
			  &stats->objects.largest.tag_size, മൂല്യം_delim);

	print_object_data("objects.commits.max_parents", താക്കോൽ_delim,
			  &stats->objects.largest.parent_എണ്ണം, മൂല്യം_delim);
	print_object_data("objects.trees.max_entries", താക്കോൽ_delim,
			  &stats->objects.largest.വൃക്ഷം_entries, മൂല്യം_delim);

	fflush(stdout);
}

ഘടന എണ്ണം_references_data {
	ഘടന ref_stats *stats;
	ഘടന rev_info *revs;
	ഘടന progress *progress;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ എണ്ണം_references(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	ഘടന എണ്ണം_references_data *data = cb_data;
	ഘടന ref_stats *stats = data->stats;
	size_t ref_എണ്ണം;

	തിരഞ്ഞെടുപ്പ് (ref_kind_from_refname(ref->name)) {
	അവസ്ഥ FILTER_REFS_BRANCHES:
		stats->branches++;
		നിർത്തുക;
	അവസ്ഥ FILTER_REFS_REMOTES:
		stats->remotes++;
		നിർത്തുക;
	അവസ്ഥ FILTER_REFS_TAGS:
		stats->tags++;
		നിർത്തുക;
	അവസ്ഥ FILTER_REFS_OTHERS:
		stats->others++;
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	add_pending_oid(data->revs, ശൂന്യ, ref->oid, 0);

	ref_എണ്ണം = get_total_reference_എണ്ണം(stats);
	display_progress(data->progress, ref_എണ്ണം);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം structure_എണ്ണം_references(ഘടന ref_stats *stats,
				       ഘടന rev_info *revs,
				       ഘടന repository *repo,
				       പൂർണ്ണസംഖ്യ show_progress)
{
	ഘടന എണ്ണം_references_data data = {
		.stats = stats,
		.revs = revs,
	};

	എങ്കിൽ (show_progress)
		data.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), എണ്ണം_references, &data);
	stop_progress(&data.progress);
}

ഘടന എണ്ണം_objects_data {
	ഘടന object_database *odb;
	ഘടന object_stats *stats;
	ഘടന progress *progress;
};

സ്ഥിതം ശൂന്യം check_largest(ഘടന object_data *data, ഘടന object_id *oid,
			  size_t മൂല്യം)
{
	എങ്കിൽ (മൂല്യം > data->മൂല്യം || is_null_oid(&data->oid)) {
		oidcpy(&data->oid, oid);
		data->മൂല്യം = മൂല്യം;
	}
}

സ്ഥിതം size_t എണ്ണം_വൃക്ഷം_entries(ഘടന object *obj)
{
	ഘടന വൃക്ഷം *t = object_as_type(obj, OBJ_വൃക്ഷം, 0);
	ഘടന name_entry entry;
	ഘടന വൃക്ഷം_desc desc;
	size_t എണ്ണം = 0;

	init_വൃക്ഷം_desc(&desc, &t->object.oid, t->buffer, t->size);
	ആകുമ്പോൾ (വൃക്ഷം_entry(&desc, &entry))
		എണ്ണം++;

	തിരികെ എണ്ണം;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ എണ്ണം_objects(സ്ഥിരം അക്ഷരം *path UNUSED, ഘടന oid_array *oids,
			 എണ്ണൽ object_type type, ശൂന്യം *cb_data)
{
	ഘടന എണ്ണം_objects_data *data = cb_data;
	ഘടന object_stats *stats = data->stats;
	size_t object_എണ്ണം;

	വേണ്ടി (size_t i = 0; i < oids->nr; i++) {
		ഘടന object_info oi = OBJECT_INFO_INIT;
		unsigned long inflated;
		size_t inflated_st = 0;
		ഘടന commit *commit;
		ഘടന object *obj;
		ശൂന്യം *content;
		off_t disk;
		പൂർണ്ണസംഖ്യ eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		എങ്കിൽ (odb_read_object_info_extended(data->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_QUICK) < 0)
			തുടരുക;
		inflated = cast_size_t_to_ulong(inflated_st);

		obj = parse_object_buffer(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		തിരഞ്ഞെടുപ്പ് (type) {
		അവസ്ഥ OBJ_TAG:
			stats->type_counts.tags++;
			stats->inflated_sizes.tags += inflated;
			stats->disk_sizes.tags += disk;
			check_largest(&stats->largest.tag_size, &oids->oid[i],
				      inflated);
			നിർത്തുക;
		അവസ്ഥ OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_എണ്ണം, &oids->oid[i],
				      commit_list_എണ്ണം(commit->parents));
			നിർത്തുക;
		അവസ്ഥ OBJ_വൃക്ഷം:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.വൃക്ഷം_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.വൃക്ഷം_entries, &oids->oid[i],
				      എണ്ണം_വൃക്ഷം_entries(obj));
			നിർത്തുക;
		അവസ്ഥ OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_size, &oids->oid[i],
				      inflated);
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			BUG("invalid object type");
		}

		എങ്കിൽ (!eaten)
			സ്വതന്ത്രം(content);
	}

	object_എണ്ണം = get_total_object_മൂല്യങ്ങൾ(&stats->type_counts);
	display_progress(data->progress, object_എണ്ണം);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം structure_എണ്ണം_objects(ഘടന object_stats *stats,
				    ഘടന rev_info *revs,
				    ഘടന repository *repo, പൂർണ്ണസംഖ്യ show_progress)
{
	ഘടന path_walk_info info = PATH_WALK_INFO_INIT;
	ഘടന എണ്ണം_objects_data data = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = എണ്ണം_objects;
	info.path_fn_data = &data;

	എങ്കിൽ (show_progress)
		data.progress = start_delayed_progress(repo, _("Counting objects"), 0);

	walk_objects_by_path(&info);
	path_walk_info_clear(&info);
	stop_progress(&data.progress);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_repo_structure(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			      ഘടന repository *repo)
{
	ഘടന stats_table table = {
		.rows = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	എണ്ണൽ output_format format = FORMAT_TABLE;
	ഘടന repo_structure stats = { 0 };
	ഘടന rev_info revs;
	പൂർണ്ണസംഖ്യ show_progress = -1;
	ഘടന option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ശൂന്യ, &format, ശൂന്യ,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_progress, N_("show progress")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_structure_usage, 0);
	എങ്കിൽ (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, prefix);

	എങ്കിൽ (show_progress < 0)
		show_progress = isatty(2);

	structure_എണ്ണം_references(&stats.refs, &revs, repo, show_progress);
	structure_എണ്ണം_objects(&stats.objects, &revs, repo, show_progress);

	തിരഞ്ഞെടുപ്പ് (format) {
	അവസ്ഥ FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_print_structure(&table);
		നിർത്തുക;
	അവസ്ഥ FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		നിർത്തുക;
	അവസ്ഥ FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("invalid output format");
	}

	stats_table_clear(&table);
	release_revisions(&revs);

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_repo(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
	     ഘടന repository *repo)
{
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_usage, 0);

	തിരികെ fn(argc, argv, prefix, repo);
}
