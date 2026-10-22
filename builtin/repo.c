#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "path-walk.h"
#ଅନ୍ତର୍ଭାଵ "progress.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "ref-filter.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "shallow.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "tree-walk.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"

#define REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#define REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	ଖାଲି,
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ repo_info_usage[] = {
	REPO_INFO_USAGE,
	ଖାଲି,
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	ଖାଲି,
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ମୂଲ୍ୟ_fn(ସଂରଚନା repository *repo, ସଂରଚନା strbuf *buf);

ଗଣନା output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

ସଂରଚନା repo_info_field {
	ଅଚଳ ଅକ୍ଷର *ଚାବି;
	get_ମୂଲ୍ୟ_fn *get_ମୂଲ୍ୟ;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_layout_bare(ସଂରଚନା repository *repo UNUSED, ସଂରଚନା strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_layout_shallow(ସଂରଚନା repository *repo, ସଂରଚନା strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_object_format(ସଂରଚନା repository *repo, ସଂରଚନା strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_path_commondir_absolute(ସଂରଚନା repository *repo, ସଂରଚନା strbuf *buf)
{
	ଅଚଳ ଅକ୍ଷର *common_dir = repo_get_common_dir(repo);

	ଯଦି (!common_dir)
		ଫେରନ୍ତୁ error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_path_commondir_relative(ସଂରଚନା repository *repo, ସଂରଚନା strbuf *buf)
{
	ଅଚଳ ଅକ୍ଷର *common_dir = repo_get_common_dir(repo);

	ଯଦି (!common_dir)
		ଫେରନ୍ତୁ error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_path_gitdir_absolute(ସଂରଚନା repository *repo, ସଂରଚନା strbuf *buf)
{
	ଅଚଳ ଅକ୍ଷର *git_dir = repo_get_git_dir(repo);

	ଯଦି (!git_dir)
		ଫେରନ୍ତୁ error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_path_gitdir_relative(ସଂରଚନା repository *repo, ସଂରଚନା strbuf *buf)
{
	ଅଚଳ ଅକ୍ଷର *git_dir = repo_get_git_dir(repo);

	ଯଦି (!git_dir)
		ଫେରନ୍ତୁ error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_references_format(ସଂରଚନା repository *repo, ସଂରଚନା strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_to_name(repo->ref_storage_format));
	ଫେରନ୍ତୁ 0;
}

/* repo_info_field keys must be in lexicographical order */
ସ୍ଥିର ଅଚଳ ସଂରଚନା repo_info_field repo_info_field[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_path_commondir_absolute },
	{ "path.commondir.relative", get_path_commondir_relative },
	{ "path.gitdir.absolute", get_path_gitdir_absolute },
	{ "path.gitdir.relative", get_path_gitdir_relative },
	{ "references.format", get_references_format },
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା repo_info_field_cmp(ଅଚଳ ଶୂନ୍ୟ *va, ଅଚଳ ଶୂନ୍ୟ *vb)
{
	ଅଚଳ ସଂରଚନା repo_info_field *a = va;
	ଅଚଳ ସଂରଚନା repo_info_field *b = vb;

	ଫେରନ୍ତୁ strcmp(a->ଚାବି, b->ଚାବି);
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା repo_info_field *get_repo_info_field(ଅଚଳ ଅକ୍ଷର *ଚାବି)
{
	ଅଚଳ ସଂରଚନା repo_info_field ସନ୍ଧାନ_ଚାବି = { ଚାବି, ଖାଲି };
	ଅଚଳ ସଂରଚନା repo_info_field *found = bsearch(&ସନ୍ଧାନ_ଚାବି,
						      repo_info_field,
						      ARRAY_SIZE(repo_info_field),
						      ଆକାର(*found),
						      repo_info_field_cmp);

	ଫେରନ୍ତୁ found;
}

ସ୍ଥିର ଶୂନ୍ୟ print_field(ଗଣନା output_format format, ଅଚଳ ଅକ୍ଷର *ଚାବି,
			ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ବାଛ (format) {
	ସ୍ଥିତି FORMAT_NEWLINE_TERMINATED:
		printf("%s=", ଚାବି);
		quote_c_style(ମୂଲ୍ୟ, ଖାଲି, stdout, 0);
		putchar('\n');
		ଭାଙ୍ଗ;
	ସ୍ଥିତି FORMAT_NUL_TERMINATED:
		printf("%s\n%s%c", ଚାବି, ମୂଲ୍ୟ, '\0');
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("not a valid output format: %d", format);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_fields(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv,
			ସଂରଚନା repository *repo,
			ଗଣନା output_format format)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ସଂରଚନା strbuf valbuf = STRBUF_INIT;

	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *ଚାବି = argv[i];
		ଅଚଳ ସଂରଚନା repo_info_field *field = get_repo_info_field(ଚାବି);

		ଯଦି (!field) {
			ret = error(_("key '%s' not found"), ଚାବି);
			ଜାରି;
		}

		strbuf_reset(&valbuf);
		field->get_ମୂଲ୍ୟ(repo, &valbuf);
		print_field(format, ଚାବି, valbuf.buf);
	}

	strbuf_release(&valbuf);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_all_fields(ସଂରଚନା repository *repo,
			    ଗଣନା output_format format)
{
	ସଂରଚନା strbuf valbuf = STRBUF_INIT;

	ପାଇଁ (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		ଅଚଳ ସଂରଚନା repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_ମୂଲ୍ୟ(repo, &valbuf);
		print_field(format, field->ଚାବି, valbuf.buf);
	}

	strbuf_release(&valbuf);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_ଚାବିଗୁଡ଼ିକ(ଗଣନା output_format format)
{
	ଅକ୍ଷର sep;

	ବାଛ (format) {
	ସ୍ଥିତି FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		ଭାଙ୍ଗ;
	ସ୍ଥିତି FORMAT_NUL_TERMINATED:
		sep = '\0';
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	ପାଇଁ (size_t i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		ଅଚଳ ସଂରଚନା repo_info_field *field = &repo_info_field[i];
		printf("%s%c", field->ଚାବି, sep);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_format_cb(ଅଚଳ ସଂରଚନା option *opt,
			   ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset UNUSED)
{
	ଗଣନା output_format *format = opt->ମୂଲ୍ୟ;

	ଯଦି (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "table"))
		*format = FORMAT_TABLE;
	ଅନ୍ୟଥା
		die(_("invalid format '%s'"), arg);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_repo_info(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା repository *repo)
{
	ଗଣନା output_format format = FORMAT_NEWLINE_TERMINATED;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_ଚାବିଗୁଡ଼ିକ = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ଚାବିଗୁଡ଼ିକ = 0;
	ସଂରଚନା option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ଖାଲି, &format, ଖାଲି,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_ଚାବିଗୁଡ଼ିକ, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_ଚାବିଗୁଡ଼ିକ, N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	ଯଦି (show_ଚାବିଗୁଡ଼ିକ && (all_ଚାବିଗୁଡ଼ିକ || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	ଯଦି (show_ଚାବିଗୁଡ଼ିକ)
		ଫେରନ୍ତୁ print_ଚାବିଗୁଡ଼ିକ(format);

	ଯଦି (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	ଯଦି (all_ଚାବିଗୁଡ଼ିକ && argc)
		die(_("--all and <key> cannot be used together"));

	ଯଦି (all_ଚାବିଗୁଡ଼ିକ)
		ଫେରନ୍ତୁ print_all_fields(repo, format);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ print_fields(argc, argv, repo, format);
}

ସଂରଚନା object_data {
	ସଂରଚନା object_id oid;
	size_t ମୂଲ୍ୟ;
};

ସଂରଚନା largest_objects {
	ସଂରଚନା object_data tag_size;
	ସଂରଚନା object_data commit_size;
	ସଂରଚନା object_data ବୃକ୍ଷ_size;
	ସଂରଚନା object_data blob_size;

	ସଂରଚନା object_data parent_ଗଣନା;
	ସଂରଚନା object_data ବୃକ୍ଷ_entries;
};

ସଂରଚନା ref_stats {
	size_t branches;
	size_t remotes;
	size_t tags;
	size_t others;
};

ସଂରଚନା object_ମୂଲ୍ୟଗୁଡ଼ିକ {
	size_t tags;
	size_t commits;
	size_t trees;
	size_t blobs;
};

ସଂରଚନା object_stats {
	ସଂରଚନା object_ମୂଲ୍ୟଗୁଡ଼ିକ type_counts;
	ସଂରଚନା object_ମୂଲ୍ୟଗୁଡ଼ିକ inflated_sizes;
	ସଂରଚନା object_ମୂଲ୍ୟଗୁଡ଼ିକ disk_sizes;
	ସଂରଚନା largest_objects largest;
};

ସଂରଚନା repo_structure {
	ସଂରଚନା ref_stats refs;
	ସଂରଚନା object_stats objects;
};

ସଂରଚନା stats_table {
	ସଂରଚନା string_list rows;
	ସଂରଚନା string_list annotations;

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_col_width;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ_col_width;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
ସଂରଚନା stats_table_entry {
	ଅକ୍ଷର *ମୂଲ୍ୟ;
	ଅଚଳ ଅକ୍ଷର *unit;
	size_t index;
	ସଂରଚନା object_id *oid;
};

ସ୍ଥିର ଶୂନ୍ୟ stats_table_vaddf(ସଂରଚନା stats_table *table,
			      ସଂରଚନା stats_table_entry *entry,
			      ଅଚଳ ଅକ୍ଷର *format, va_list ap)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା string_list_item *item;
	ଅକ୍ଷର *formatted_name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_width;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, ଖାଲି);
	name_width = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&table->rows, formatted_name);
	item->util = entry;

	ଯଦି (name_width > table->name_col_width)
		table->name_col_width = name_width;
	ଯଦି (!entry)
		ଫେରନ୍ତୁ;
	ଯଦି (entry->oid) {
		entry->index = table->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)entry->index,
			    oid_to_hex(entry->oid));
		string_list_append_nodup(&table->annotations, strbuf_detach(&buf, ଖାଲି));
	}
	ଯଦି (entry->ମୂଲ୍ୟ) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ_width = utf8_strwidth(entry->ମୂଲ୍ୟ);
		ଯଦି (ମୂଲ୍ୟ_width > table->ମୂଲ୍ୟ_col_width)
			table->ମୂଲ୍ୟ_col_width = ମୂଲ୍ୟ_width;
	}
	ଯଦି (entry->unit) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା unit_width = utf8_strwidth(entry->unit);
		ଯଦି (unit_width > table->unit_col_width)
			table->unit_col_width = unit_width;
	}

	strbuf_release(&buf);
}

ସ୍ଥିର ଶୂନ୍ୟ stats_table_addf(ସଂରଚନା stats_table *table, ଅଚଳ ଅକ୍ଷର *format, ...)
{
	va_list ap;

	va_start(ap, format);
	stats_table_vaddf(table, ଖାଲି, format, ap);
	va_end(ap);
}

ସ୍ଥିର ଶୂନ୍ୟ stats_table_ଗଣନା_addf(ସଂରଚନା stats_table *table, size_t ମୂଲ୍ୟ,
				   ଅଚଳ ଅକ୍ଷର *format, ...)
{
	ସଂରଚନା stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_ଗଣନା(ମୂଲ୍ୟ, &entry->ମୂଲ୍ୟ, &entry->unit);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ସ୍ଥିର ଶୂନ୍ୟ stats_table_object_ଗଣନା_addf(ସଂରଚନା stats_table *table,
					  ସଂରଚନା object_id *oid, size_t ମୂଲ୍ୟ,
					  ଅଚଳ ଅକ୍ଷର *format, ...)
{
	ସଂରଚନା stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_ଗଣନା(ମୂଲ୍ୟ, &entry->ମୂଲ୍ୟ, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	ଯଦି (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ସ୍ଥିର ଶୂନ୍ୟ stats_table_size_addf(ସଂରଚନା stats_table *table, size_t ମୂଲ୍ୟ,
				  ଅଚଳ ଅକ୍ଷର *format, ...)
{
	ସଂରଚନା stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(ମୂଲ୍ୟ, &entry->ମୂଲ୍ୟ, &entry->unit, HUMANISE_COMPACT);

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ସ୍ଥିର ଶୂନ୍ୟ stats_table_object_size_addf(ସଂରଚନା stats_table *table,
					 ସଂରଚନା object_id *oid, size_t ମୂଲ୍ୟ,
					 ଅଚଳ ଅକ୍ଷର *format, ...)
{
	ସଂରଚନା stats_table_entry *entry;
	va_list ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(ମୂଲ୍ୟ, &entry->ମୂଲ୍ୟ, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	ଯଦି (!is_null_oid(oid))
		entry->oid = oid;

	va_start(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	va_end(ap);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t get_total_reference_ଗଣନା(ସଂରଚନା ref_stats *stats)
{
	ଫେରନ୍ତୁ stats->branches + stats->remotes + stats->tags + stats->others;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t get_total_object_ମୂଲ୍ୟଗୁଡ଼ିକ(ସଂରଚନା object_ମୂଲ୍ୟଗୁଡ଼ିକ *ମୂଲ୍ୟଗୁଡ଼ିକ)
{
	ଫେରନ୍ତୁ ମୂଲ୍ୟଗୁଡ଼ିକ->tags + ମୂଲ୍ୟଗୁଡ଼ିକ->commits + ମୂଲ୍ୟଗୁଡ଼ିକ->trees + ମୂଲ୍ୟଗୁଡ଼ିକ->blobs;
}

ସ୍ଥିର ଶୂନ୍ୟ stats_table_setup_structure(ସଂରଚନା stats_table *table,
					ସଂରଚନା repo_structure *stats)
{
	ସଂରଚନା object_stats *objects = &stats->objects;
	ସଂରଚନା ref_stats *refs = &stats->refs;
	size_t inflated_object_total;
	size_t object_ଗଣନା_total;
	size_t disk_object_total;
	size_t ref_total;

	ref_total = get_total_reference_ଗଣନା(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_ଗଣନା_addf(table, ref_total, "  * %s", _("Count"));
	stats_table_ଗଣନା_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_ଗଣନା_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_ଗଣନା_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_ଗଣନା_addf(table, refs->others, "    * %s", _("Others"));

	object_ଗଣନା_total = get_total_object_ମୂଲ୍ୟଗୁଡ଼ିକ(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_ଗଣନା_addf(table, object_ଗଣନା_total, "  * %s", _("Count"));
	stats_table_ଗଣନା_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_ଗଣନା_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_ଗଣନା_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_ଗଣନା_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_total = get_total_object_ମୂଲ୍ୟଗୁଡ଼ିକ(&objects->inflated_sizes);
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

	disk_object_total = get_total_object_ମୂଲ୍ୟଗୁଡ଼ିକ(&objects->disk_sizes);
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
				     objects->largest.commit_size.ମୂଲ୍ୟ,
				     "    * %s", _("Maximum size"));
	stats_table_object_ଗଣନା_addf(table,
				      &objects->largest.parent_ଗଣନା.oid,
				      objects->largest.parent_ଗଣନା.ମୂଲ୍ୟ,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_size_addf(table,
				     &objects->largest.ବୃକ୍ଷ_size.oid,
				     objects->largest.ବୃକ୍ଷ_size.ମୂଲ୍ୟ,
				     "    * %s", _("Maximum size"));
	stats_table_object_ଗଣନା_addf(table,
				      &objects->largest.ବୃକ୍ଷ_entries.oid,
				      objects->largest.ବୃକ୍ଷ_entries.ମୂଲ୍ୟ,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_size_addf(table,
				     &objects->largest.blob_size.oid,
				     objects->largest.blob_size.ମୂଲ୍ୟ,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_size_addf(table,
				     &objects->largest.tag_size.oid,
				     objects->largest.tag_size.ମୂଲ୍ୟ,
				     "    * %s", _("Maximum size"));
}

#define INDEX_WIDTH 4

ସ୍ଥିର ଶୂନ୍ୟ stats_table_print_structure(ଅଚଳ ସଂରଚନା stats_table *table)
{
	ଅଚଳ ଅକ୍ଷର *name_col_title = _("Repository structure");
	ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ_col_title = _("Value");
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା title_name_width = utf8_strwidth(name_col_title);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା title_ମୂଲ୍ୟ_width = utf8_strwidth(ମୂଲ୍ୟ_col_title);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_col_width = table->name_col_width;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ_col_width = table->ମୂଲ୍ୟ_col_width;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା unit_col_width = table->unit_col_width;
	ସଂରଚନା string_list_item *item;
	ସଂରଚନା strbuf buf = STRBUF_INIT;

	ଯଦି (title_name_width > name_col_width)
		name_col_width = title_name_width;
	ଯଦି (title_ମୂଲ୍ୟ_width > ମୂଲ୍ୟ_col_width + unit_col_width + 1)
		ମୂଲ୍ୟ_col_width = title_ମୂଲ୍ୟ_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_ବାମ, name_col_width + INDEX_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_ବାମ,
			  ମୂଲ୍ୟ_col_width + unit_col_width + 1, ମୂଲ୍ୟ_col_title);
	strbuf_addstr(&buf, " |");
	printf("%s\n", buf.buf);

	printf("| ");
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < name_col_width + INDEX_WIDTH; i++)
		putchar('-');
	printf(" | ");
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ମୂଲ୍ୟ_col_width + unit_col_width + 1; i++)
		putchar('-');
	printf(" |\n");

	for_each_string_list_item(item, &table->rows) {
		ସଂରଚନା stats_table_entry *entry = item->util;
		ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ = "";
		ଅଚଳ ଅକ୍ଷର *unit = "";

		ଯଦି (entry) {
			ମୂଲ୍ୟ = entry->ମୂଲ୍ୟ;
			ଯଦି (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_ବାମ, name_col_width, item->string);

		ଯଦି (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->index);
		ଅନ୍ୟଥା
			strbuf_addchars(&buf, ' ', INDEX_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_ଡାହାଣ, ମୂଲ୍ୟ_col_width, ମୂଲ୍ୟ);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_ବାମ, unit_col_width, unit);
		strbuf_addstr(&buf, " |");
		printf("%s\n", buf.buf);
	}

	ଯଦି (table->annotations.nr) {
		printf("\n");
		for_each_string_list_item(item, &table->annotations)
			printf("%s\n", item->string);
	}

	strbuf_release(&buf);
}

ସ୍ଥିର ଶୂନ୍ୟ stats_table_clear(ସଂରଚନା stats_table *table)
{
	ସଂରଚନା stats_table_entry *entry;
	ସଂରଚନା string_list_item *item;

	for_each_string_list_item(item, &table->rows) {
		entry = item->util;
		ଯଦି (entry)
			ମୁକ୍ତ(entry->ମୂଲ୍ୟ);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ print_keyvalue(ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅକ୍ଷର ଚାବି_delim, size_t ମୂଲ୍ୟ,
				  ଅକ୍ଷର ମୂଲ୍ୟ_delim)
{
	printf("%s%c%" PRIuMAX "%c", ଚାବି, ଚାବି_delim, (uintmax_t)ମୂଲ୍ୟ,
	       ମୂଲ୍ୟ_delim);
}

ସ୍ଥିର ଶୂନ୍ୟ print_object_data(ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅକ୍ଷର ଚାବି_delim,
			      ସଂରଚନା object_data *data, ଅକ୍ଷର ମୂଲ୍ୟ_delim)
{
	print_keyvalue(ଚାବି, ଚାବି_delim, data->ମୂଲ୍ୟ, ମୂଲ୍ୟ_delim);
	printf("%s_oid%c%s%c", ଚାବି, ଚାବି_delim, oid_to_hex(&data->oid),
	       ମୂଲ୍ୟ_delim);
}

ସ୍ଥିର ଶୂନ୍ୟ structure_keyvalue_print(ସଂରଚନା repo_structure *stats,
				     ଅକ୍ଷର ଚାବି_delim, ଅକ୍ଷର ମୂଲ୍ୟ_delim)
{
	print_keyvalue("references.branches.count", ଚାବି_delim,
		       stats->refs.branches, ମୂଲ୍ୟ_delim);
	print_keyvalue("references.tags.count", ଚାବି_delim,
		       stats->refs.tags, ମୂଲ୍ୟ_delim);
	print_keyvalue("references.remotes.count", ଚାବି_delim,
		       stats->refs.remotes, ମୂଲ୍ୟ_delim);
	print_keyvalue("references.others.count", ଚାବି_delim,
		       stats->refs.others, ମୂଲ୍ୟ_delim);

	print_keyvalue("objects.commits.count", ଚାବି_delim,
		       stats->objects.type_counts.commits, ମୂଲ୍ୟ_delim);
	print_keyvalue("objects.trees.count", ଚାବି_delim,
		       stats->objects.type_counts.trees, ମୂଲ୍ୟ_delim);
	print_keyvalue("objects.blobs.count", ଚାବି_delim,
		       stats->objects.type_counts.blobs, ମୂଲ୍ୟ_delim);
	print_keyvalue("objects.tags.count", ଚାବି_delim,
		       stats->objects.type_counts.tags, ମୂଲ୍ୟ_delim);

	print_keyvalue("objects.commits.inflated_size", ଚାବି_delim,
		       stats->objects.inflated_sizes.commits, ମୂଲ୍ୟ_delim);
	print_keyvalue("objects.trees.inflated_size", ଚାବି_delim,
		       stats->objects.inflated_sizes.trees, ମୂଲ୍ୟ_delim);
	print_keyvalue("objects.blobs.inflated_size", ଚାବି_delim,
		       stats->objects.inflated_sizes.blobs, ମୂଲ୍ୟ_delim);
	print_keyvalue("objects.tags.inflated_size", ଚାବି_delim,
		       stats->objects.inflated_sizes.tags, ମୂଲ୍ୟ_delim);

	print_keyvalue("objects.commits.disk_size", ଚାବି_delim,
		       stats->objects.disk_sizes.commits, ମୂଲ୍ୟ_delim);
	print_keyvalue("objects.trees.disk_size", ଚାବି_delim,
		       stats->objects.disk_sizes.trees, ମୂଲ୍ୟ_delim);
	print_keyvalue("objects.blobs.disk_size", ଚାବି_delim,
		       stats->objects.disk_sizes.blobs, ମୂଲ୍ୟ_delim);
	print_keyvalue("objects.tags.disk_size", ଚାବି_delim,
		       stats->objects.disk_sizes.tags, ମୂଲ୍ୟ_delim);

	print_object_data("objects.commits.max_size", ଚାବି_delim,
			  &stats->objects.largest.commit_size, ମୂଲ୍ୟ_delim);
	print_object_data("objects.trees.max_size", ଚାବି_delim,
			  &stats->objects.largest.ବୃକ୍ଷ_size, ମୂଲ୍ୟ_delim);
	print_object_data("objects.blobs.max_size", ଚାବି_delim,
			  &stats->objects.largest.blob_size, ମୂଲ୍ୟ_delim);
	print_object_data("objects.tags.max_size", ଚାବି_delim,
			  &stats->objects.largest.tag_size, ମୂଲ୍ୟ_delim);

	print_object_data("objects.commits.max_parents", ଚାବି_delim,
			  &stats->objects.largest.parent_ଗଣନା, ମୂଲ୍ୟ_delim);
	print_object_data("objects.trees.max_entries", ଚାବି_delim,
			  &stats->objects.largest.ବୃକ୍ଷ_entries, ମୂଲ୍ୟ_delim);

	fflush(stdout);
}

ସଂରଚନା ଗଣନା_references_data {
	ସଂରଚନା ref_stats *stats;
	ସଂରଚନା rev_info *revs;
	ସଂରଚନା progress *progress;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_references(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା ଗଣନା_references_data *data = cb_data;
	ସଂରଚନା ref_stats *stats = data->stats;
	size_t ref_ଗଣନା;

	ବାଛ (ref_kind_from_refname(ref->name)) {
	ସ୍ଥିତି FILTER_REFS_BRANCHES:
		stats->branches++;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି FILTER_REFS_REMOTES:
		stats->remotes++;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି FILTER_REFS_TAGS:
		stats->tags++;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି FILTER_REFS_OTHERS:
		stats->others++;
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	add_pending_oid(data->revs, ଖାଲି, ref->oid, 0);

	ref_ଗଣନା = get_total_reference_ଗଣନା(stats);
	display_progress(data->progress, ref_ଗଣନା);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ structure_ଗଣନା_references(ସଂରଚନା ref_stats *stats,
				       ସଂରଚନା rev_info *revs,
				       ସଂରଚନା repository *repo,
				       ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_progress)
{
	ସଂରଚନା ଗଣନା_references_data data = {
		.stats = stats,
		.revs = revs,
	};

	ଯଦି (show_progress)
		data.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), ଗଣନା_references, &data);
	stop_progress(&data.progress);
}

ସଂରଚନା ଗଣନା_objects_data {
	ସଂରଚନା object_database *odb;
	ସଂରଚନା object_stats *stats;
	ସଂରଚନା progress *progress;
};

ସ୍ଥିର ଶୂନ୍ୟ check_largest(ସଂରଚନା object_data *data, ସଂରଚନା object_id *oid,
			  size_t ମୂଲ୍ୟ)
{
	ଯଦି (ମୂଲ୍ୟ > data->ମୂଲ୍ୟ || is_null_oid(&data->oid)) {
		oidcpy(&data->oid, oid);
		data->ମୂଲ୍ୟ = ମୂଲ୍ୟ;
	}
}

ସ୍ଥିର size_t ଗଣନା_ବୃକ୍ଷ_entries(ସଂରଚନା object *obj)
{
	ସଂରଚନା ବୃକ୍ଷ *t = object_as_type(obj, OBJ_ବୃକ୍ଷ, 0);
	ସଂରଚନା name_entry entry;
	ସଂରଚନା ବୃକ୍ଷ_desc desc;
	size_t ଗଣନା_2 = 0;

	init_ବୃକ୍ଷ_desc(&desc, &t->object.oid, t->buffer, t->size);
	ଯେତେବେଳେ (ବୃକ୍ଷ_entry(&desc, &entry))
		ଗଣନା_2++;

	ଫେରନ୍ତୁ ଗଣନା_2;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_objects(ଅଚଳ ଅକ୍ଷର *path UNUSED, ସଂରଚନା oid_array *oids,
			 ଗଣନା object_type type, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା ଗଣନା_objects_data *data = cb_data;
	ସଂରଚନା object_stats *stats = data->stats;
	size_t object_ଗଣନା;

	ପାଇଁ (size_t i = 0; i < oids->nr; i++) {
		ସଂରଚନା object_info oi = OBJECT_INFO_INIT;
		unsigned long inflated;
		size_t inflated_st = 0;
		ସଂରଚନା commit *commit;
		ସଂରଚନା object *obj;
		ଶୂନ୍ୟ *content;
		off_t disk;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		ଯଦି (odb_read_object_info_extended(data->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_QUICK) < 0)
			ଜାରି;
		inflated = cast_size_t_to_ulong(inflated_st);

		obj = parse_object_buffer(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		ବାଛ (type) {
		ସ୍ଥିତି OBJ_TAG:
			stats->type_counts.tags++;
			stats->inflated_sizes.tags += inflated;
			stats->disk_sizes.tags += disk;
			check_largest(&stats->largest.tag_size, &oids->oid[i],
				      inflated);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_ଗଣନା, &oids->oid[i],
				      commit_list_ଗଣନା(commit->parents));
			ଭାଙ୍ଗ;
		ସ୍ଥିତି OBJ_ବୃକ୍ଷ:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.ବୃକ୍ଷ_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.ବୃକ୍ଷ_entries, &oids->oid[i],
				      ଗଣନା_ବୃକ୍ଷ_entries(obj));
			ଭାଙ୍ଗ;
		ସ୍ଥିତି OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_size, &oids->oid[i],
				      inflated);
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			BUG("invalid object type");
		}

		ଯଦି (!eaten)
			ମୁକ୍ତ(content);
	}

	object_ଗଣନା = get_total_object_ମୂଲ୍ୟଗୁଡ଼ିକ(&stats->type_counts);
	display_progress(data->progress, object_ଗଣନା);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ structure_ଗଣନା_objects(ସଂରଚନା object_stats *stats,
				    ସଂରଚନା rev_info *revs,
				    ସଂରଚନା repository *repo, ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_progress)
{
	ସଂରଚନା path_walk_info info = PATH_WALK_INFO_INIT;
	ସଂରଚନା ଗଣନା_objects_data data = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = ଗଣନା_objects;
	info.path_fn_data = &data;

	ଯଦି (show_progress)
		data.progress = start_delayed_progress(repo, _("Counting objects"), 0);

	walk_objects_by_path(&info);
	path_walk_info_clear(&info);
	stop_progress(&data.progress);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_repo_structure(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			      ସଂରଚନା repository *repo)
{
	ସଂରଚନା stats_table table = {
		.rows = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	ଗଣନା output_format format = FORMAT_TABLE;
	ସଂରଚନା repo_structure stats = { 0 };
	ସଂରଚନା rev_info revs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_progress = -1;
	ସଂରଚନା option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ଖାଲି, &format, ଖାଲି,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_progress, N_("show progress")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_structure_usage, 0);
	ଯଦି (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, prefix);

	ଯଦି (show_progress < 0)
		show_progress = isatty(2);

	structure_ଗଣନା_references(&stats.refs, &revs, repo, show_progress);
	structure_ଗଣନା_objects(&stats.objects, &revs, repo, show_progress);

	ବାଛ (format) {
	ସ୍ଥିତି FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_print_structure(&table);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		ଭାଙ୍ଗ;
	ସ୍ଥିତି FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid output format");
	}

	stats_table_clear(&table);
	release_revisions(&revs);

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_repo(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
	     ସଂରଚନା repository *repo)
{
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_usage, 0);

	ଫେରନ୍ତୁ fn(argc, argv, prefix, repo);
}
