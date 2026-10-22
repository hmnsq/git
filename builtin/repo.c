#define USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "path-walk.h"
#ಅನ್ತರ್ಭಾವ "progress.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "ref-filter.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "shallow.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"

#define REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#define REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	ರಿಕ್ತ,
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ repo_info_usage[] = {
	REPO_INFO_USAGE,
	ರಿಕ್ತ,
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	ರಿಕ್ತ,
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ get_ಮೌಲ್ಯ_fn(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ strbuf *buf);

ಗಣನಾ output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

ಸಂರಚನಾ repo_info_field {
	ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ;
	get_ಮೌಲ್ಯ_fn *get_ಮೌಲ್ಯ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_layout_bare(ಸಂರಚನಾ repository *repo UNUSED, ಸಂರಚನಾ strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_layout_shallow(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_object_format(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_path_commondir_absolute(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ strbuf *buf)
{
	ಧ್ರುವ ವರ್ಣ *common_dir = repo_get_common_dir(repo);

	ಯದಿ (!common_dir)
		ಪ್ರತ್ಯಾಗಮ error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_path_commondir_relative(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ strbuf *buf)
{
	ಧ್ರುವ ವರ್ಣ *common_dir = repo_get_common_dir(repo);

	ಯದಿ (!common_dir)
		ಪ್ರತ್ಯಾಗಮ error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_path_gitdir_absolute(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ strbuf *buf)
{
	ಧ್ರುವ ವರ್ಣ *git_dir = repo_get_git_dir(repo);

	ಯದಿ (!git_dir)
		ಪ್ರತ್ಯಾಗಮ error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_path_gitdir_relative(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ strbuf *buf)
{
	ಧ್ರುವ ವರ್ಣ *git_dir = repo_get_git_dir(repo);

	ಯದಿ (!git_dir)
		ಪ್ರತ್ಯಾಗಮ error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_references_format(ಸಂರಚನಾ repository *repo, ಸಂರಚನಾ strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_to_name(repo->ref_storage_format));
	ಪ್ರತ್ಯಾಗಮ 0;
}

/* repo_info_field keys must be in lexicographical order */
ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ repo_info_field repo_info_field[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_path_commondir_absolute },
	{ "path.commondir.relative", get_path_commondir_relative },
	{ "path.gitdir.absolute", get_path_gitdir_absolute },
	{ "path.gitdir.relative", get_path_gitdir_relative },
	{ "references.format", get_references_format },
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ repo_info_field_cmp(ಧ್ರುವ ಶೂನ್ಯ *va, ಧ್ರುವ ಶೂನ್ಯ *vb)
{
	ಧ್ರುವ ಸಂರಚನಾ repo_info_field *a = va;
	ಧ್ರುವ ಸಂರಚನಾ repo_info_field *b = vb;

	ಪ್ರತ್ಯಾಗಮ strcmp(a->ಕೀಲಿ, b->ಕೀಲಿ);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ repo_info_field *get_repo_info_field(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ)
{
	ಧ್ರುವ ಸಂರಚನಾ repo_info_field ಹುಡುಕು_ಕೀಲಿ = { ಕೀಲಿ, ರಿಕ್ತ };
	ಧ್ರುವ ಸಂರಚನಾ repo_info_field *found = bsearch(&ಹುಡುಕು_ಕೀಲಿ,
						      repo_info_field,
						      ARRAY_SIZE(repo_info_field),
						      ಪರಿಮಾಣ(*found),
						      repo_info_field_cmp);

	ಪ್ರತ್ಯಾಗಮ found;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_field(ಗಣನಾ output_format format, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ,
			ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
	ವಿಕಲ್ಪ (format) {
	ಪಕ್ಷ FORMAT_NEWLINE_TERMINATED:
		printf("%s=", ಕೀಲಿ);
		quote_c_style(ಮೌಲ್ಯ, ರಿಕ್ತ, stdout, 0);
		putchar('\n');
		ಭಙ್ಗ;
	ಪಕ್ಷ FORMAT_NUL_TERMINATED:
		printf("%s\n%s%c", ಕೀಲಿ, ಮೌಲ್ಯ, '\0');
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("not a valid output format: %d", format);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ print_fields(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv,
			ಸಂರಚನಾ repository *repo,
			ಗಣನಾ output_format format)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಸಂರಚನಾ strbuf valbuf = STRBUF_INIT;

	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ = argv[i];
		ಧ್ರುವ ಸಂರಚನಾ repo_info_field *field = get_repo_info_field(ಕೀಲಿ);

		ಯದಿ (!field) {
			ret = error(_("key '%s' not found"), ಕೀಲಿ);
			ಅನುವರ್ತ;
		}

		strbuf_reset(&valbuf);
		field->get_ಮೌಲ್ಯ(repo, &valbuf);
		print_field(format, ಕೀಲಿ, valbuf.buf);
	}

	strbuf_release(&valbuf);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ print_all_fields(ಸಂರಚನಾ repository *repo,
			    ಗಣನಾ output_format format)
{
	ಸಂರಚನಾ strbuf valbuf = STRBUF_INIT;

	ಪ್ರತಿ (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		ಧ್ರುವ ಸಂರಚನಾ repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_ಮೌಲ್ಯ(repo, &valbuf);
		print_field(format, field->ಕೀಲಿ, valbuf.buf);
	}

	strbuf_release(&valbuf);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ print_ಕೀಲಿಗಳು(ಗಣನಾ output_format format)
{
	ವರ್ಣ sep;

	ವಿಕಲ್ಪ (format) {
	ಪಕ್ಷ FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		ಭಙ್ಗ;
	ಪಕ್ಷ FORMAT_NUL_TERMINATED:
		sep = '\0';
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	ಪ್ರತಿ (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		ಧ್ರುವ ಸಂರಚನಾ repo_info_field *field = &repo_info_field[i];
		printf("%s%c", field->ಕೀಲಿ, sep);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_format_cb(ಧ್ರುವ ಸಂರಚನಾ option *opt,
			   ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset UNUSED)
{
	ಗಣನಾ output_format *format = opt->ಮೌಲ್ಯ;

	ಯದಿ (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "table"))
		*format = FORMAT_TABLE;
	ಅನ್ಯಥಾ
		die(_("invalid format '%s'"), arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_repo_info(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			 ಸಂರಚನಾ repository *repo)
{
	ಗಣನಾ output_format format = FORMAT_NEWLINE_TERMINATED;
	ಪೂರ್ಣಾಙ್ಕ all_ಕೀಲಿಗಳು = 0;
	ಪೂರ್ಣಾಙ್ಕ show_ಕೀಲಿಗಳು = 0;
	ಸಂರಚನಾ option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ರಿಕ್ತ, &format, ರಿಕ್ತ,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_ಕೀಲಿಗಳು, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_ಕೀಲಿಗಳು, N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	ಯದಿ (show_ಕೀಲಿಗಳು && (all_ಕೀಲಿಗಳು || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	ಯದಿ (show_ಕೀಲಿಗಳು)
		ಪ್ರತ್ಯಾಗಮ print_ಕೀಲಿಗಳು(format);

	ಯದಿ (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	ಯದಿ (all_ಕೀಲಿಗಳು && argc)
		die(_("--all and <key> cannot be used together"));

	ಯದಿ (all_ಕೀಲಿಗಳು)
		ಪ್ರತ್ಯಾಗಮ print_all_fields(repo, format);
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ print_fields(argc, argv, repo, format);
}

ಸಂರಚನಾ object_data {
	ಸಂರಚನಾ object_id oid;
	size_t ಮೌಲ್ಯ;
};

ಸಂರಚನಾ largest_objects {
	ಸಂರಚನಾ object_data tag_size;
	ಸಂರಚನಾ object_data commit_size;
	ಸಂರಚನಾ object_data ವೃಕ್ಷ_size;
	ಸಂರಚನಾ object_data blob_size;

	ಸಂರಚನಾ object_data parent_ಎಣಿಕೆ;
	ಸಂರಚನಾ object_data ವೃಕ್ಷ_entries;
};

ಸಂರಚನಾ ref_stats {
	size_t branches;
	size_t remotes;
	size_t tags;
	size_t others;
};

ಸಂರಚನಾ object_ಮೌಲ್ಯಗಳು {
	size_t tags;
	size_t commits;
	size_t trees;
	size_t blobs;
};

ಸಂರಚನಾ object_stats {
	ಸಂರಚನಾ object_ಮೌಲ್ಯಗಳು type_counts;
	ಸಂರಚನಾ object_ಮೌಲ್ಯಗಳು inflated_sizes;
	ಸಂರಚನಾ object_ಮೌಲ್ಯಗಳು disk_sizes;
	ಸಂರಚನಾ largest_objects largest;
};

ಸಂರಚನಾ repo_structure {
	ಸಂರಚನಾ ref_stats refs;
	ಸಂರಚನಾ object_stats objects;
};

ಸಂರಚನಾ stats_table {
	ಸಂರಚನಾ string_list rows;
	ಸಂರಚನಾ string_list annotations;

	ಪೂರ್ಣಾಙ್ಕ name_col_width;
	ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ_col_width;
	ಪೂರ್ಣಾಙ್ಕ unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
ಸಂರಚನಾ stats_table_entry {
	ವರ್ಣ *ಮೌಲ್ಯ;
	ಧ್ರುವ ವರ್ಣ *unit;
	size_t index;
	ಸಂರಚನಾ object_id *oid;
};

ಸ್ಥಿರ ಶೂನ್ಯ stats_table_vaddf(ಸಂರಚನಾ stats_table *table,
			      ಸಂರಚನಾ stats_table_entry *entry,
			      ಧ್ರುವ ವರ್ಣ *format, va_list ap)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ string_list_item *item;
	ವರ್ಣ *formatted_name;
	ಪೂರ್ಣಾಙ್ಕ name_width;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, ರಿಕ್ತ);
	name_width = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&table->rows, formatted_name);
	item->util = entry;

	ಯದಿ (name_width > table->name_col_width)
		table->name_col_width = name_width;
	ಯದಿ (!entry)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (entry->oid) {
		entry->index = table->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)entry->index,
			    oid_to_hex(entry->oid));
		string_list_append_nodup(&table->annotations, strbuf_detach(&buf, ರಿಕ್ತ));
	}
	ಯದಿ (entry->ಮೌಲ್ಯ) {
		ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ_width = utf8_strwidth(entry->ಮೌಲ್ಯ);
		ಯದಿ (ಮೌಲ್ಯ_width > table->ಮೌಲ್ಯ_col_width)
			table->ಮೌಲ್ಯ_col_width = ಮೌಲ್ಯ_width;
	}
	ಯದಿ (entry->unit) {
		ಪೂರ್ಣಾಙ್ಕ unit_width = utf8_strwidth(entry->unit);
		ಯದಿ (unit_width > table->unit_col_width)
			table->unit_col_width = unit_width;
	}

	strbuf_release(&buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_table_addf(ಸಂರಚನಾ stats_table *table, ಧ್ರುವ ವರ್ಣ *format, ...)
{
	va_list ap;

	va_start(ap, format);
	stats_table_vaddf(table, ರಿಕ್ತ, format, ap);
	va_end(ap);
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_table_ಎಣಿಕೆ_addf(ಸಂರಚನಾ stats_table *table, size_t ಮೌಲ್ಯ,
				   ಧ್ರುವ ವರ್ಣ *format, ...)
{
	ಸಂರಚನಾ stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_ಎಣಿಕೆ(ಮೌಲ್ಯ, &entry->ಮೌಲ್ಯ, &entry->unit);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_table_object_ಎಣಿಕೆ_addf(ಸಂರಚನಾ stats_table *table,
					  ಸಂರಚನಾ object_id *oid, size_t ಮೌಲ್ಯ,
					  ಧ್ರುವ ವರ್ಣ *format, ...)
{
	ಸಂರಚನಾ stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_ಎಣಿಕೆ(ಮೌಲ್ಯ, &entry->ಮೌಲ್ಯ, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	ಯದಿ (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_table_size_addf(ಸಂರಚನಾ stats_table *table, size_t ಮೌಲ್ಯ,
				  ಧ್ರುವ ವರ್ಣ *format, ...)
{
	ಸಂರಚನಾ stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(ಮೌಲ್ಯ, &entry->ಮೌಲ್ಯ, &entry->unit, HUMANISE_COMPACT);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_table_object_size_addf(ಸಂರಚನಾ stats_table *table,
					 ಸಂರಚನಾ object_id *oid, size_t ಮೌಲ್ಯ,
					 ಧ್ರುವ ವರ್ಣ *format, ...)
{
	ಸಂರಚನಾ stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(ಮೌಲ್ಯ, &entry->ಮೌಲ್ಯ, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	ಯದಿ (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t get_total_reference_ಎಣಿಕೆ(ಸಂರಚನಾ ref_stats *stats)
{
	ಪ್ರತ್ಯಾಗಮ stats->branches + stats->remotes + stats->tags + stats->others;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t get_total_object_ಮೌಲ್ಯಗಳು(ಸಂರಚನಾ object_ಮೌಲ್ಯಗಳು *ಮೌಲ್ಯಗಳು)
{
	ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯಗಳು->tags + ಮೌಲ್ಯಗಳು->commits + ಮೌಲ್ಯಗಳು->trees + ಮೌಲ್ಯಗಳು->blobs;
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_table_setup_structure(ಸಂರಚನಾ stats_table *table,
					ಸಂರಚನಾ repo_structure *stats)
{
	ಸಂರಚನಾ object_stats *objects = &stats->objects;
	ಸಂರಚನಾ ref_stats *refs = &stats->refs;
	size_t inflated_object_total;
	size_t object_ಎಣಿಕೆ_total;
	size_t disk_object_total;
	size_t ref_total;

	ref_total = get_total_reference_ಎಣಿಕೆ(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_ಎಣಿಕೆ_addf(table, ref_total, "  * %s", _("Count"));
	stats_table_ಎಣಿಕೆ_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_ಎಣಿಕೆ_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_ಎಣಿಕೆ_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_ಎಣಿಕೆ_addf(table, refs->others, "    * %s", _("Others"));

	object_ಎಣಿಕೆ_total = get_total_object_ಮೌಲ್ಯಗಳು(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_ಎಣಿಕೆ_addf(table, object_ಎಣಿಕೆ_total, "  * %s", _("Count"));
	stats_table_ಎಣಿಕೆ_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_ಎಣಿಕೆ_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_ಎಣಿಕೆ_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_ಎಣಿಕೆ_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_total = get_total_object_ಮೌಲ್ಯಗಳು(&objects->inflated_sizes);
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

	disk_object_total = get_total_object_ಮೌಲ್ಯಗಳು(&objects->disk_sizes);
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
				     objects->largest.commit_size.ಮೌಲ್ಯ,
				     "    * %s", _("Maximum size"));
	stats_table_object_ಎಣಿಕೆ_addf(table,
				      &objects->largest.parent_ಎಣಿಕೆ.oid,
				      objects->largest.parent_ಎಣಿಕೆ.ಮೌಲ್ಯ,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_size_addf(table,
				     &objects->largest.ವೃಕ್ಷ_size.oid,
				     objects->largest.ವೃಕ್ಷ_size.ಮೌಲ್ಯ,
				     "    * %s", _("Maximum size"));
	stats_table_object_ಎಣಿಕೆ_addf(table,
				      &objects->largest.ವೃಕ್ಷ_entries.oid,
				      objects->largest.ವೃಕ್ಷ_entries.ಮೌಲ್ಯ,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_size_addf(table,
				     &objects->largest.blob_size.oid,
				     objects->largest.blob_size.ಮೌಲ್ಯ,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_size_addf(table,
				     &objects->largest.tag_size.oid,
				     objects->largest.tag_size.ಮೌಲ್ಯ,
				     "    * %s", _("Maximum size"));
}

#define INDEX_WIDTH 4

ಸ್ಥಿರ ಶೂನ್ಯ stats_table_print_structure(ಧ್ರುವ ಸಂರಚನಾ stats_table *table)
{
	ಧ್ರುವ ವರ್ಣ *name_col_title = _("Repository structure");
	ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ_col_title = _("Value");
	ಪೂರ್ಣಾಙ್ಕ title_name_width = utf8_strwidth(name_col_title);
	ಪೂರ್ಣಾಙ್ಕ title_ಮೌಲ್ಯ_width = utf8_strwidth(ಮೌಲ್ಯ_col_title);
	ಪೂರ್ಣಾಙ್ಕ name_col_width = table->name_col_width;
	ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ_col_width = table->ಮೌಲ್ಯ_col_width;
	ಪೂರ್ಣಾಙ್ಕ unit_col_width = table->unit_col_width;
	ಸಂರಚನಾ string_list_item *item;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

	ಯದಿ (title_name_width > name_col_width)
		name_col_width = title_name_width;
	ಯದಿ (title_ಮೌಲ್ಯ_width > ಮೌಲ್ಯ_col_width + unit_col_width + 1)
		ಮೌಲ್ಯ_col_width = title_ಮೌಲ್ಯ_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_ಎಡ, name_col_width + INDEX_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_ಎಡ,
			  ಮೌಲ್ಯ_col_width + unit_col_width + 1, ಮೌಲ್ಯ_col_title);
	strbuf_addstr(&buf, " |");
	printf("%s\n", buf.buf);

	printf("| ");
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < name_col_width + INDEX_WIDTH; i++)
		putchar('-');
	printf(" | ");
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಮೌಲ್ಯ_col_width + unit_col_width + 1; i++)
		putchar('-');
	printf(" |\n");

	for_each_string_list_item(item, &table->rows) {
		ಸಂರಚನಾ stats_table_entry *entry = item->util;
		ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ = "";
		ಧ್ರುವ ವರ್ಣ *unit = "";

		ಯದಿ (entry) {
			ಮೌಲ್ಯ = entry->ಮೌಲ್ಯ;
			ಯದಿ (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_ಎಡ, name_col_width, item->string);

		ಯದಿ (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->index);
		ಅನ್ಯಥಾ
			strbuf_addchars(&buf, ' ', INDEX_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_ಬಲ, ಮೌಲ್ಯ_col_width, ಮೌಲ್ಯ);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_ಎಡ, unit_col_width, unit);
		strbuf_addstr(&buf, " |");
		printf("%s\n", buf.buf);
	}

	ಯದಿ (table->annotations.nr) {
		printf("\n");
		for_each_string_list_item(item, &table->annotations)
			printf("%s\n", item->string);
	}

	strbuf_release(&buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_table_clear(ಸಂರಚನಾ stats_table *table)
{
	ಸಂರಚನಾ stats_table_entry *entry;
	ಸಂರಚನಾ string_list_item *item;

	for_each_string_list_item(item, &table->rows) {
		entry = item->util;
		ಯದಿ (entry)
			ಮುಕ್ತ(entry->ಮೌಲ್ಯ);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ print_keyvalue(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ವರ್ಣ ಕೀಲಿ_delim, size_t ಮೌಲ್ಯ,
				  ವರ್ಣ ಮೌಲ್ಯ_delim)
{
	printf("%s%c%" PRIuMAX "%c", ಕೀಲಿ, ಕೀಲಿ_delim, (uintmax_t)ಮೌಲ್ಯ,
	       ಮೌಲ್ಯ_delim);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_object_data(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ವರ್ಣ ಕೀಲಿ_delim,
			      ಸಂರಚನಾ object_data *data, ವರ್ಣ ಮೌಲ್ಯ_delim)
{
	print_keyvalue(ಕೀಲಿ, ಕೀಲಿ_delim, data->ಮೌಲ್ಯ, ಮೌಲ್ಯ_delim);
	printf("%s_oid%c%s%c", ಕೀಲಿ, ಕೀಲಿ_delim, oid_to_hex(&data->oid),
	       ಮೌಲ್ಯ_delim);
}

ಸ್ಥಿರ ಶೂನ್ಯ structure_keyvalue_print(ಸಂರಚನಾ repo_structure *stats,
				     ವರ್ಣ ಕೀಲಿ_delim, ವರ್ಣ ಮೌಲ್ಯ_delim)
{
	print_keyvalue("references.branches.count", ಕೀಲಿ_delim,
		       stats->refs.branches, ಮೌಲ್ಯ_delim);
	print_keyvalue("references.tags.count", ಕೀಲಿ_delim,
		       stats->refs.tags, ಮೌಲ್ಯ_delim);
	print_keyvalue("references.remotes.count", ಕೀಲಿ_delim,
		       stats->refs.remotes, ಮೌಲ್ಯ_delim);
	print_keyvalue("references.others.count", ಕೀಲಿ_delim,
		       stats->refs.others, ಮೌಲ್ಯ_delim);

	print_keyvalue("objects.commits.count", ಕೀಲಿ_delim,
		       stats->objects.type_counts.commits, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.trees.count", ಕೀಲಿ_delim,
		       stats->objects.type_counts.trees, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.blobs.count", ಕೀಲಿ_delim,
		       stats->objects.type_counts.blobs, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.tags.count", ಕೀಲಿ_delim,
		       stats->objects.type_counts.tags, ಮೌಲ್ಯ_delim);

	print_keyvalue("objects.commits.inflated_size", ಕೀಲಿ_delim,
		       stats->objects.inflated_sizes.commits, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.trees.inflated_size", ಕೀಲಿ_delim,
		       stats->objects.inflated_sizes.trees, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.blobs.inflated_size", ಕೀಲಿ_delim,
		       stats->objects.inflated_sizes.blobs, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.tags.inflated_size", ಕೀಲಿ_delim,
		       stats->objects.inflated_sizes.tags, ಮೌಲ್ಯ_delim);

	print_keyvalue("objects.commits.disk_size", ಕೀಲಿ_delim,
		       stats->objects.disk_sizes.commits, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.trees.disk_size", ಕೀಲಿ_delim,
		       stats->objects.disk_sizes.trees, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.blobs.disk_size", ಕೀಲಿ_delim,
		       stats->objects.disk_sizes.blobs, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.tags.disk_size", ಕೀಲಿ_delim,
		       stats->objects.disk_sizes.tags, ಮೌಲ್ಯ_delim);

	print_object_data("objects.commits.max_size", ಕೀಲಿ_delim,
			  &stats->objects.largest.commit_size, ಮೌಲ್ಯ_delim);
	print_object_data("objects.trees.max_size", ಕೀಲಿ_delim,
			  &stats->objects.largest.ವೃಕ್ಷ_size, ಮೌಲ್ಯ_delim);
	print_object_data("objects.blobs.max_size", ಕೀಲಿ_delim,
			  &stats->objects.largest.blob_size, ಮೌಲ್ಯ_delim);
	print_object_data("objects.tags.max_size", ಕೀಲಿ_delim,
			  &stats->objects.largest.tag_size, ಮೌಲ್ಯ_delim);

	print_object_data("objects.commits.max_parents", ಕೀಲಿ_delim,
			  &stats->objects.largest.parent_ಎಣಿಕೆ, ಮೌಲ್ಯ_delim);
	print_object_data("objects.trees.max_entries", ಕೀಲಿ_delim,
			  &stats->objects.largest.ವೃಕ್ಷ_entries, ಮೌಲ್ಯ_delim);

	fflush(stdout);
}

ಸಂರಚನಾ ಎಣಿಕೆ_references_data {
	ಸಂರಚನಾ ref_stats *stats;
	ಸಂರಚನಾ rev_info *revs;
	ಸಂರಚನಾ progress *progress;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_references(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ ಎಣಿಕೆ_references_data *data = cb_data;
	ಸಂರಚನಾ ref_stats *stats = data->stats;
	size_t ref_ಎಣಿಕೆ;

	ವಿಕಲ್ಪ (ref_kind_from_refname(ref->name)) {
	ಪಕ್ಷ FILTER_REFS_BRANCHES:
		stats->branches++;
		ಭಙ್ಗ;
	ಪಕ್ಷ FILTER_REFS_REMOTES:
		stats->remotes++;
		ಭಙ್ಗ;
	ಪಕ್ಷ FILTER_REFS_TAGS:
		stats->tags++;
		ಭಙ್ಗ;
	ಪಕ್ಷ FILTER_REFS_OTHERS:
		stats->others++;
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	add_pending_oid(data->revs, ರಿಕ್ತ, ref->oid, 0);

	ref_ಎಣಿಕೆ = get_total_reference_ಎಣಿಕೆ(stats);
	display_progress(data->progress, ref_ಎಣಿಕೆ);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ structure_ಎಣಿಕೆ_references(ಸಂರಚನಾ ref_stats *stats,
				       ಸಂರಚನಾ rev_info *revs,
				       ಸಂರಚನಾ repository *repo,
				       ಪೂರ್ಣಾಙ್ಕ show_progress)
{
	ಸಂರಚನಾ ಎಣಿಕೆ_references_data data = {
		.stats = stats,
		.revs = revs,
	};

	ಯದಿ (show_progress)
		data.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), ಎಣಿಕೆ_references, &data);
	stop_progress(&data.progress);
}

ಸಂರಚನಾ ಎಣಿಕೆ_objects_data {
	ಸಂರಚನಾ object_database *odb;
	ಸಂರಚನಾ object_stats *stats;
	ಸಂರಚನಾ progress *progress;
};

ಸ್ಥಿರ ಶೂನ್ಯ check_largest(ಸಂರಚನಾ object_data *data, ಸಂರಚನಾ object_id *oid,
			  size_t ಮೌಲ್ಯ)
{
	ಯದಿ (ಮೌಲ್ಯ > data->ಮೌಲ್ಯ || is_null_oid(&data->oid)) {
		oidcpy(&data->oid, oid);
		data->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
	}
}

ಸ್ಥಿರ size_t ಎಣಿಕೆ_ವೃಕ್ಷ_entries(ಸಂರಚನಾ object *obj)
{
	ಸಂರಚನಾ ವೃಕ್ಷ *t = object_as_type(obj, OBJ_ವೃಕ್ಷ, 0);
	ಸಂರಚನಾ name_entry entry;
	ಸಂರಚನಾ ವೃಕ್ಷ_desc desc;
	size_t ಎಣಿಕೆ = 0;

	init_ವೃಕ್ಷ_desc(&desc, &t->object.oid, t->buffer, t->size);
	ಯಾವತ್ (ವೃಕ್ಷ_entry(&desc, &entry))
		ಎಣಿಕೆ++;

	ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_objects(ಧ್ರುವ ವರ್ಣ *path UNUSED, ಸಂರಚನಾ oid_array *oids,
			 ಗಣನಾ object_type type, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ ಎಣಿಕೆ_objects_data *data = cb_data;
	ಸಂರಚನಾ object_stats *stats = data->stats;
	size_t object_ಎಣಿಕೆ;

	ಪ್ರತಿ (size_t i = 0; i < oids->nr; i++) {
		ಸಂರಚನಾ object_info oi = OBJECT_INFO_INIT;
		unsigned long inflated;
		size_t inflated_st = 0;
		ಸಂರಚನಾ commit *commit;
		ಸಂರಚನಾ object *obj;
		ಶೂನ್ಯ *content;
		off_t disk;
		ಪೂರ್ಣಾಙ್ಕ eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		ಯದಿ (odb_read_object_info_extended(data->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_QUICK) < 0)
			ಅನುವರ್ತ;
		inflated = cast_size_t_to_ulong(inflated_st);

		obj = parse_object_buffer(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		ವಿಕಲ್ಪ (type) {
		ಪಕ್ಷ OBJ_TAG:
			stats->type_counts.tags++;
			stats->inflated_sizes.tags += inflated;
			stats->disk_sizes.tags += disk;
			check_largest(&stats->largest.tag_size, &oids->oid[i],
				      inflated);
			ಭಙ್ಗ;
		ಪಕ್ಷ OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_ಎಣಿಕೆ, &oids->oid[i],
				      commit_list_ಎಣಿಕೆ(commit->parents));
			ಭಙ್ಗ;
		ಪಕ್ಷ OBJ_ವೃಕ್ಷ:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.ವೃಕ್ಷ_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.ವೃಕ್ಷ_entries, &oids->oid[i],
				      ಎಣಿಕೆ_ವೃಕ್ಷ_entries(obj));
			ಭಙ್ಗ;
		ಪಕ್ಷ OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_size, &oids->oid[i],
				      inflated);
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			BUG("invalid object type");
		}

		ಯದಿ (!eaten)
			ಮುಕ್ತ(content);
	}

	object_ಎಣಿಕೆ = get_total_object_ಮೌಲ್ಯಗಳು(&stats->type_counts);
	display_progress(data->progress, object_ಎಣಿಕೆ);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ structure_ಎಣಿಕೆ_objects(ಸಂರಚನಾ object_stats *stats,
				    ಸಂರಚನಾ rev_info *revs,
				    ಸಂರಚನಾ repository *repo, ಪೂರ್ಣಾಙ್ಕ show_progress)
{
	ಸಂರಚನಾ path_walk_info info = PATH_WALK_INFO_INIT;
	ಸಂರಚನಾ ಎಣಿಕೆ_objects_data data = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = ಎಣಿಕೆ_objects;
	info.path_fn_data = &data;

	ಯದಿ (show_progress)
		data.progress = start_delayed_progress(repo, _("Counting objects"), 0);

	walk_objects_by_path(&info);
	path_walk_info_clear(&info);
	stop_progress(&data.progress);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_repo_structure(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			      ಸಂರಚನಾ repository *repo)
{
	ಸಂರಚನಾ stats_table table = {
		.rows = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	ಗಣನಾ output_format format = FORMAT_TABLE;
	ಸಂರಚನಾ repo_structure stats = { 0 };
	ಸಂರಚನಾ rev_info revs;
	ಪೂರ್ಣಾಙ್ಕ show_progress = -1;
	ಸಂರಚನಾ option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ರಿಕ್ತ, &format, ರಿಕ್ತ,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_progress, N_("show progress")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_structure_usage, 0);
	ಯದಿ (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, prefix);

	ಯದಿ (show_progress < 0)
		show_progress = isatty(2);

	structure_ಎಣಿಕೆ_references(&stats.refs, &revs, repo, show_progress);
	structure_ಎಣಿಕೆ_objects(&stats.objects, &revs, repo, show_progress);

	ವಿಕಲ್ಪ (format) {
	ಪಕ್ಷ FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_print_structure(&table);
		ಭಙ್ಗ;
	ಪಕ್ಷ FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		ಭಙ್ಗ;
	ಪಕ್ಷ FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("invalid output format");
	}

	stats_table_clear(&table);
	release_revisions(&revs);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_repo(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
	     ಸಂರಚನಾ repository *repo)
{
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_usage, 0);

	ಪ್ರತ್ಯಾಗಮ fn(argc, argv, prefix, repo);
}
