#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "commit.h"
#समावेश "environment.h"
#समावेश "hash.h"
#समावेश "hex.h"
#समावेश "odb.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "path-walk.h"
#समावेश "progress.h"
#समावेश "quote.h"
#समावेश "ref-filter.h"
#समावेश "refs.h"
#समावेश "revision.h"
#समावेश "setup.h"
#समावेश "strbuf.h"
#समावेश "string-list.h"
#समावेश "shallow.h"
#समावेश "tree.h"
#समावेश "tree-walk.h"
#समावेश "utf8.h"

#घोषणा REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#घोषणा REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

अटल स्थिर अक्षर *स्थिर repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	शून्य,
};

अटल स्थिर अक्षर *स्थिर repo_info_usage[] = {
	REPO_INFO_USAGE,
	शून्य,
};

अटल स्थिर अक्षर *स्थिर repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	शून्य,
};

प्रकार पूर्णांक get_मूल्य_fn(काष्ठा repository *repo, काष्ठा strbuf *buf);

क्रमागत output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

काष्ठा repo_info_field {
	स्थिर अक्षर *कुंजी;
	get_मूल्य_fn *get_मूल्य;
};

अटल पूर्णांक get_layout_bare(काष्ठा repository *repo UNUSED, काष्ठा strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	वापस 0;
}

अटल पूर्णांक get_layout_shallow(काष्ठा repository *repo, काष्ठा strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	वापस 0;
}

अटल पूर्णांक get_object_format(काष्ठा repository *repo, काष्ठा strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	वापस 0;
}

अटल पूर्णांक get_path_commondir_absolute(काष्ठा repository *repo, काष्ठा strbuf *buf)
{
	स्थिर अक्षर *common_dir = repo_get_common_dir(repo);

	अगर (!common_dir)
		वापस error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	वापस 0;
}

अटल पूर्णांक get_path_commondir_relative(काष्ठा repository *repo, काष्ठा strbuf *buf)
{
	स्थिर अक्षर *common_dir = repo_get_common_dir(repo);

	अगर (!common_dir)
		वापस error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	वापस 0;
}

अटल पूर्णांक get_path_gitdir_absolute(काष्ठा repository *repo, काष्ठा strbuf *buf)
{
	स्थिर अक्षर *git_dir = repo_get_git_dir(repo);

	अगर (!git_dir)
		वापस error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	वापस 0;
}

अटल पूर्णांक get_path_gitdir_relative(काष्ठा repository *repo, काष्ठा strbuf *buf)
{
	स्थिर अक्षर *git_dir = repo_get_git_dir(repo);

	अगर (!git_dir)
		वापस error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	वापस 0;
}

अटल पूर्णांक get_references_format(काष्ठा repository *repo, काष्ठा strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_to_name(repo->ref_storage_format));
	वापस 0;
}

/* repo_info_field keys must be in lexicographical order */
अटल स्थिर काष्ठा repo_info_field repo_info_field[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_path_commondir_absolute },
	{ "path.commondir.relative", get_path_commondir_relative },
	{ "path.gitdir.absolute", get_path_gitdir_absolute },
	{ "path.gitdir.relative", get_path_gitdir_relative },
	{ "references.format", get_references_format },
};

अटल पूर्णांक repo_info_field_cmp(स्थिर व्योम *va, स्थिर व्योम *vb)
{
	स्थिर काष्ठा repo_info_field *a = va;
	स्थिर काष्ठा repo_info_field *b = vb;

	वापस म_भेद(a->कुंजी, b->कुंजी);
}

अटल स्थिर काष्ठा repo_info_field *get_repo_info_field(स्थिर अक्षर *कुंजी)
{
	स्थिर काष्ठा repo_info_field खोज_कुंजी = { कुंजी, शून्य };
	स्थिर काष्ठा repo_info_field *found = द्वा_खोज(&खोज_कुंजी,
						      repo_info_field,
						      ARRAY_आकार(repo_info_field),
						      माप(*found),
						      repo_info_field_cmp);

	वापस found;
}

अटल व्योम print_field(क्रमागत output_format format, स्थिर अक्षर *कुंजी,
			स्थिर अक्षर *मूल्य)
{
	चयन (format) {
	हाल FORMAT_NEWLINE_TERMINATED:
		म_लिखो("%s=", कुंजी);
		quote_c_style(मूल्य, शून्य, मानक_निकास, 0);
		अक्षर_दो('\n');
		अवरोध;
	हाल FORMAT_NUL_TERMINATED:
		म_लिखो("%s\n%s%c", कुंजी, मूल्य, '\0');
		अवरोध;
	शेष:
		BUG("not a valid output format: %d", format);
	}
}

अटल पूर्णांक print_fields(पूर्णांक argc, स्थिर अक्षर **argv,
			काष्ठा repository *repo,
			क्रमागत output_format format)
{
	पूर्णांक ret = 0;
	काष्ठा strbuf valbuf = STRBUF_INIT;

	क्रम (पूर्णांक i = 0; i < argc; i++) {
		स्थिर अक्षर *कुंजी = argv[i];
		स्थिर काष्ठा repo_info_field *field = get_repo_info_field(कुंजी);

		अगर (!field) {
			ret = error(_("key '%s' not found"), कुंजी);
			जारी;
		}

		strbuf_reset(&valbuf);
		field->get_मूल्य(repo, &valbuf);
		print_field(format, कुंजी, valbuf.buf);
	}

	strbuf_release(&valbuf);
	वापस ret;
}

अटल पूर्णांक print_all_fields(काष्ठा repository *repo,
			    क्रमागत output_format format)
{
	काष्ठा strbuf valbuf = STRBUF_INIT;

	क्रम (माप_प्रकार i = 0; i < ARRAY_आकार(repo_info_field); i++) {
		स्थिर काष्ठा repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_मूल्य(repo, &valbuf);
		print_field(format, field->कुंजी, valbuf.buf);
	}

	strbuf_release(&valbuf);
	वापस 0;
}

अटल पूर्णांक print_कुंजियाँ(क्रमागत output_format format)
{
	अक्षर sep;

	चयन (format) {
	हाल FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		अवरोध;
	हाल FORMAT_NUL_TERMINATED:
		sep = '\0';
		अवरोध;
	शेष:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	क्रम (माप_प्रकार i = 0; i < ARRAY_आकार(repo_info_field); i++) {
		स्थिर काष्ठा repo_info_field *field = &repo_info_field[i];
		म_लिखो("%s%c", field->कुंजी, sep);
	}

	वापस 0;
}

अटल पूर्णांक parse_format_cb(स्थिर काष्ठा option *opt,
			   स्थिर अक्षर *arg, पूर्णांक unset UNUSED)
{
	क्रमागत output_format *format = opt->मूल्य;

	अगर (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	अन्यथा अगर (!म_भेद(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	अन्यथा अगर (!म_भेद(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	अन्यथा अगर (!म_भेद(arg, "table"))
		*format = FORMAT_TABLE;
	अन्यथा
		die(_("invalid format '%s'"), arg);

	वापस 0;
}

अटल पूर्णांक cmd_repo_info(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			 काष्ठा repository *repo)
{
	क्रमागत output_format format = FORMAT_NEWLINE_TERMINATED;
	पूर्णांक all_कुंजियाँ = 0;
	पूर्णांक show_कुंजियाँ = 0;
	काष्ठा option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', शून्य, &format, शून्य,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_कुंजियाँ, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_कुंजियाँ, N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	अगर (show_कुंजियाँ && (all_कुंजियाँ || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	अगर (show_कुंजियाँ)
		वापस print_कुंजियाँ(format);

	अगर (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	अगर (all_कुंजियाँ && argc)
		die(_("--all and <key> cannot be used together"));

	अगर (all_कुंजियाँ)
		वापस print_all_fields(repo, format);
	अन्यथा
		वापस print_fields(argc, argv, repo, format);
}

काष्ठा object_तथ्य {
	काष्ठा object_id oid;
	माप_प्रकार मूल्य;
};

काष्ठा largest_objects {
	काष्ठा object_तथ्य tag_आकार;
	काष्ठा object_तथ्य commit_आकार;
	काष्ठा object_तथ्य वृक्ष_आकार;
	काष्ठा object_तथ्य blob_आकार;

	काष्ठा object_तथ्य parent_गिनती;
	काष्ठा object_तथ्य वृक्ष_entries;
};

काष्ठा ref_stats {
	माप_प्रकार branches;
	माप_प्रकार remotes;
	माप_प्रकार tags;
	माप_प्रकार others;
};

काष्ठा object_मूल्यावली {
	माप_प्रकार tags;
	माप_प्रकार commits;
	माप_प्रकार trees;
	माप_प्रकार blobs;
};

काष्ठा object_stats {
	काष्ठा object_मूल्यावली type_counts;
	काष्ठा object_मूल्यावली inflated_sizes;
	काष्ठा object_मूल्यावली disk_sizes;
	काष्ठा largest_objects largest;
};

काष्ठा repo_structure {
	काष्ठा ref_stats refs;
	काष्ठा object_stats objects;
};

काष्ठा stats_table {
	काष्ठा string_list rows;
	काष्ठा string_list annotations;

	पूर्णांक name_col_width;
	पूर्णांक मूल्य_col_width;
	पूर्णांक unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
काष्ठा stats_table_entry {
	अक्षर *मूल्य;
	स्थिर अक्षर *unit;
	माप_प्रकार सूचकांक;
	काष्ठा object_id *oid;
};

अटल व्योम stats_table_vaddf(काष्ठा stats_table *table,
			      काष्ठा stats_table_entry *entry,
			      स्थिर अक्षर *format, बहु_सूची ap)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा string_list_item *item;
	अक्षर *formatted_name;
	पूर्णांक name_width;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, शून्य);
	name_width = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&table->rows, formatted_name);
	item->util = entry;

	अगर (name_width > table->name_col_width)
		table->name_col_width = name_width;
	अगर (!entry)
		वापस;
	अगर (entry->oid) {
		entry->सूचकांक = table->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)entry->सूचकांक,
			    oid_to_hex(entry->oid));
		string_list_append_nodup(&table->annotations, strbuf_detach(&buf, शून्य));
	}
	अगर (entry->मूल्य) {
		पूर्णांक मूल्य_width = utf8_strwidth(entry->मूल्य);
		अगर (मूल्य_width > table->मूल्य_col_width)
			table->मूल्य_col_width = मूल्य_width;
	}
	अगर (entry->unit) {
		पूर्णांक unit_width = utf8_strwidth(entry->unit);
		अगर (unit_width > table->unit_col_width)
			table->unit_col_width = unit_width;
	}

	strbuf_release(&buf);
}

अटल व्योम stats_table_addf(काष्ठा stats_table *table, स्थिर अक्षर *format, ...)
{
	बहु_सूची ap;

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, शून्य, format, ap);
	बहु_पूर्ण(ap);
}

अटल व्योम stats_table_गिनती_addf(काष्ठा stats_table *table, माप_प्रकार मूल्य,
				   स्थिर अक्षर *format, ...)
{
	काष्ठा stats_table_entry *entry;
	बहु_सूची ap;

	CALLOC_ARRAY(entry, 1);
	humanise_गिनती(मूल्य, &entry->मूल्य, &entry->unit);

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	बहु_पूर्ण(ap);
}

अटल व्योम stats_table_object_गिनती_addf(काष्ठा stats_table *table,
					  काष्ठा object_id *oid, माप_प्रकार मूल्य,
					  स्थिर अक्षर *format, ...)
{
	काष्ठा stats_table_entry *entry;
	बहु_सूची ap;

	CALLOC_ARRAY(entry, 1);
	humanise_गिनती(मूल्य, &entry->मूल्य, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	अगर (!is_null_oid(oid))
		entry->oid = oid;

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	बहु_पूर्ण(ap);
}

अटल व्योम stats_table_आकार_addf(काष्ठा stats_table *table, माप_प्रकार मूल्य,
				  स्थिर अक्षर *format, ...)
{
	काष्ठा stats_table_entry *entry;
	बहु_सूची ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(मूल्य, &entry->मूल्य, &entry->unit, HUMANISE_COMPACT);

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	बहु_पूर्ण(ap);
}

अटल व्योम stats_table_object_आकार_addf(काष्ठा stats_table *table,
					 काष्ठा object_id *oid, माप_प्रकार मूल्य,
					 स्थिर अक्षर *format, ...)
{
	काष्ठा stats_table_entry *entry;
	बहु_सूची ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(मूल्य, &entry->मूल्य, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	अगर (!is_null_oid(oid))
		entry->oid = oid;

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	बहु_पूर्ण(ap);
}

अटल अंतरभूत माप_प्रकार get_योग_reference_गिनती(काष्ठा ref_stats *stats)
{
	वापस stats->branches + stats->remotes + stats->tags + stats->others;
}

अटल अंतरभूत माप_प्रकार get_योग_object_मूल्यावली(काष्ठा object_मूल्यावली *मूल्यावली)
{
	वापस मूल्यावली->tags + मूल्यावली->commits + मूल्यावली->trees + मूल्यावली->blobs;
}

अटल व्योम stats_table_setup_structure(काष्ठा stats_table *table,
					काष्ठा repo_structure *stats)
{
	काष्ठा object_stats *objects = &stats->objects;
	काष्ठा ref_stats *refs = &stats->refs;
	माप_प्रकार inflated_object_योग;
	माप_प्रकार object_गिनती_योग;
	माप_प्रकार disk_object_योग;
	माप_प्रकार ref_योग;

	ref_योग = get_योग_reference_गिनती(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_गिनती_addf(table, ref_योग, "  * %s", _("Count"));
	stats_table_गिनती_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_गिनती_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_गिनती_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_गिनती_addf(table, refs->others, "    * %s", _("Others"));

	object_गिनती_योग = get_योग_object_मूल्यावली(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_गिनती_addf(table, object_गिनती_योग, "  * %s", _("Count"));
	stats_table_गिनती_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_गिनती_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_गिनती_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_गिनती_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_योग = get_योग_object_मूल्यावली(&objects->inflated_sizes);
	stats_table_आकार_addf(table, inflated_object_योग,
			      "  * %s", _("Inflated size"));
	stats_table_आकार_addf(table, objects->inflated_sizes.commits,
			      "    * %s", _("Commits"));
	stats_table_आकार_addf(table, objects->inflated_sizes.trees,
			      "    * %s", _("Trees"));
	stats_table_आकार_addf(table, objects->inflated_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_table_आकार_addf(table, objects->inflated_sizes.tags,
			      "    * %s", _("Tags"));

	disk_object_योग = get_योग_object_मूल्यावली(&objects->disk_sizes);
	stats_table_आकार_addf(table, disk_object_योग,
			      "  * %s", _("Disk size"));
	stats_table_आकार_addf(table, objects->disk_sizes.commits,
			      "    * %s", _("Commits"));
	stats_table_आकार_addf(table, objects->disk_sizes.trees,
			      "    * %s", _("Trees"));
	stats_table_आकार_addf(table, objects->disk_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_table_आकार_addf(table, objects->disk_sizes.tags,
			      "    * %s", _("Tags"));

	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Largest objects"));
	stats_table_addf(table, "  * %s", _("Commits"));
	stats_table_object_आकार_addf(table,
				     &objects->largest.commit_आकार.oid,
				     objects->largest.commit_आकार.मूल्य,
				     "    * %s", _("Maximum size"));
	stats_table_object_गिनती_addf(table,
				      &objects->largest.parent_गिनती.oid,
				      objects->largest.parent_गिनती.मूल्य,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_आकार_addf(table,
				     &objects->largest.वृक्ष_आकार.oid,
				     objects->largest.वृक्ष_आकार.मूल्य,
				     "    * %s", _("Maximum size"));
	stats_table_object_गिनती_addf(table,
				      &objects->largest.वृक्ष_entries.oid,
				      objects->largest.वृक्ष_entries.मूल्य,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_आकार_addf(table,
				     &objects->largest.blob_आकार.oid,
				     objects->largest.blob_आकार.मूल्य,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_आकार_addf(table,
				     &objects->largest.tag_आकार.oid,
				     objects->largest.tag_आकार.मूल्य,
				     "    * %s", _("Maximum size"));
}

#घोषणा सूचकांक_WIDTH 4

अटल व्योम stats_table_print_structure(स्थिर काष्ठा stats_table *table)
{
	स्थिर अक्षर *name_col_title = _("Repository structure");
	स्थिर अक्षर *मूल्य_col_title = _("Value");
	पूर्णांक title_name_width = utf8_strwidth(name_col_title);
	पूर्णांक title_मूल्य_width = utf8_strwidth(मूल्य_col_title);
	पूर्णांक name_col_width = table->name_col_width;
	पूर्णांक मूल्य_col_width = table->मूल्य_col_width;
	पूर्णांक unit_col_width = table->unit_col_width;
	काष्ठा string_list_item *item;
	काष्ठा strbuf buf = STRBUF_INIT;

	अगर (title_name_width > name_col_width)
		name_col_width = title_name_width;
	अगर (title_मूल्य_width > मूल्य_col_width + unit_col_width + 1)
		मूल्य_col_width = title_मूल्य_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_बायाँ, name_col_width + सूचकांक_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_बायाँ,
			  मूल्य_col_width + unit_col_width + 1, मूल्य_col_title);
	strbuf_addstr(&buf, " |");
	म_लिखो("%s\n", buf.buf);

	म_लिखो("| ");
	क्रम (पूर्णांक i = 0; i < name_col_width + सूचकांक_WIDTH; i++)
		अक्षर_दो('-');
	म_लिखो(" | ");
	क्रम (पूर्णांक i = 0; i < मूल्य_col_width + unit_col_width + 1; i++)
		अक्षर_दो('-');
	म_लिखो(" |\n");

	for_each_string_list_item(item, &table->rows) {
		काष्ठा stats_table_entry *entry = item->util;
		स्थिर अक्षर *मूल्य = "";
		स्थिर अक्षर *unit = "";

		अगर (entry) {
			मूल्य = entry->मूल्य;
			अगर (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_बायाँ, name_col_width, item->string);

		अगर (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->सूचकांक);
		अन्यथा
			strbuf_addchars(&buf, ' ', सूचकांक_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_दायाँ, मूल्य_col_width, मूल्य);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_बायाँ, unit_col_width, unit);
		strbuf_addstr(&buf, " |");
		म_लिखो("%s\n", buf.buf);
	}

	अगर (table->annotations.nr) {
		म_लिखो("\n");
		for_each_string_list_item(item, &table->annotations)
			म_लिखो("%s\n", item->string);
	}

	strbuf_release(&buf);
}

अटल व्योम stats_table_clear(काष्ठा stats_table *table)
{
	काष्ठा stats_table_entry *entry;
	काष्ठा string_list_item *item;

	for_each_string_list_item(item, &table->rows) {
		entry = item->util;
		अगर (entry)
			मुक्त(entry->मूल्य);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

अटल अंतरभूत व्योम print_keyvalue(स्थिर अक्षर *कुंजी, अक्षर कुंजी_delim, माप_प्रकार मूल्य,
				  अक्षर मूल्य_delim)
{
	म_लिखो("%s%c%" PRIuMAX "%c", कुंजी, कुंजी_delim, (uintmax_t)मूल्य,
	       मूल्य_delim);
}

अटल व्योम print_object_तथ्य(स्थिर अक्षर *कुंजी, अक्षर कुंजी_delim,
			      काष्ठा object_तथ्य *तथ्य, अक्षर मूल्य_delim)
{
	print_keyvalue(कुंजी, कुंजी_delim, तथ्य->मूल्य, मूल्य_delim);
	म_लिखो("%s_oid%c%s%c", कुंजी, कुंजी_delim, oid_to_hex(&तथ्य->oid),
	       मूल्य_delim);
}

अटल व्योम structure_keyvalue_print(काष्ठा repo_structure *stats,
				     अक्षर कुंजी_delim, अक्षर मूल्य_delim)
{
	print_keyvalue("references.branches.count", कुंजी_delim,
		       stats->refs.branches, मूल्य_delim);
	print_keyvalue("references.tags.count", कुंजी_delim,
		       stats->refs.tags, मूल्य_delim);
	print_keyvalue("references.remotes.count", कुंजी_delim,
		       stats->refs.remotes, मूल्य_delim);
	print_keyvalue("references.others.count", कुंजी_delim,
		       stats->refs.others, मूल्य_delim);

	print_keyvalue("objects.commits.count", कुंजी_delim,
		       stats->objects.type_counts.commits, मूल्य_delim);
	print_keyvalue("objects.trees.count", कुंजी_delim,
		       stats->objects.type_counts.trees, मूल्य_delim);
	print_keyvalue("objects.blobs.count", कुंजी_delim,
		       stats->objects.type_counts.blobs, मूल्य_delim);
	print_keyvalue("objects.tags.count", कुंजी_delim,
		       stats->objects.type_counts.tags, मूल्य_delim);

	print_keyvalue("objects.commits.inflated_size", कुंजी_delim,
		       stats->objects.inflated_sizes.commits, मूल्य_delim);
	print_keyvalue("objects.trees.inflated_size", कुंजी_delim,
		       stats->objects.inflated_sizes.trees, मूल्य_delim);
	print_keyvalue("objects.blobs.inflated_size", कुंजी_delim,
		       stats->objects.inflated_sizes.blobs, मूल्य_delim);
	print_keyvalue("objects.tags.inflated_size", कुंजी_delim,
		       stats->objects.inflated_sizes.tags, मूल्य_delim);

	print_keyvalue("objects.commits.disk_size", कुंजी_delim,
		       stats->objects.disk_sizes.commits, मूल्य_delim);
	print_keyvalue("objects.trees.disk_size", कुंजी_delim,
		       stats->objects.disk_sizes.trees, मूल्य_delim);
	print_keyvalue("objects.blobs.disk_size", कुंजी_delim,
		       stats->objects.disk_sizes.blobs, मूल्य_delim);
	print_keyvalue("objects.tags.disk_size", कुंजी_delim,
		       stats->objects.disk_sizes.tags, मूल्य_delim);

	print_object_तथ्य("objects.commits.max_size", कुंजी_delim,
			  &stats->objects.largest.commit_आकार, मूल्य_delim);
	print_object_तथ्य("objects.trees.max_size", कुंजी_delim,
			  &stats->objects.largest.वृक्ष_आकार, मूल्य_delim);
	print_object_तथ्य("objects.blobs.max_size", कुंजी_delim,
			  &stats->objects.largest.blob_आकार, मूल्य_delim);
	print_object_तथ्य("objects.tags.max_size", कुंजी_delim,
			  &stats->objects.largest.tag_आकार, मूल्य_delim);

	print_object_तथ्य("objects.commits.max_parents", कुंजी_delim,
			  &stats->objects.largest.parent_गिनती, मूल्य_delim);
	print_object_तथ्य("objects.trees.max_entries", कुंजी_delim,
			  &stats->objects.largest.वृक्ष_entries, मूल्य_delim);

	ख_साफ(मानक_निकास);
}

काष्ठा गिनती_references_तथ्य {
	काष्ठा ref_stats *stats;
	काष्ठा rev_info *revs;
	काष्ठा progress *progress;
};

अटल पूर्णांक गिनती_references(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	काष्ठा गिनती_references_तथ्य *तथ्य = cb_तथ्य;
	काष्ठा ref_stats *stats = तथ्य->stats;
	माप_प्रकार ref_गिनती;

	चयन (ref_kind_from_refname(ref->name)) {
	हाल FILTER_REFS_BRANCHES:
		stats->branches++;
		अवरोध;
	हाल FILTER_REFS_REMOTES:
		stats->remotes++;
		अवरोध;
	हाल FILTER_REFS_TAGS:
		stats->tags++;
		अवरोध;
	हाल FILTER_REFS_OTHERS:
		stats->others++;
		अवरोध;
	शेष:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	add_pending_oid(तथ्य->revs, शून्य, ref->oid, 0);

	ref_गिनती = get_योग_reference_गिनती(stats);
	display_progress(तथ्य->progress, ref_गिनती);

	वापस 0;
}

अटल व्योम structure_गिनती_references(काष्ठा ref_stats *stats,
				       काष्ठा rev_info *revs,
				       काष्ठा repository *repo,
				       पूर्णांक show_progress)
{
	काष्ठा गिनती_references_तथ्य तथ्य = {
		.stats = stats,
		.revs = revs,
	};

	अगर (show_progress)
		तथ्य.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), गिनती_references, &तथ्य);
	stop_progress(&तथ्य.progress);
}

काष्ठा गिनती_objects_तथ्य {
	काष्ठा object_database *odb;
	काष्ठा object_stats *stats;
	काष्ठा progress *progress;
};

अटल व्योम check_largest(काष्ठा object_तथ्य *तथ्य, काष्ठा object_id *oid,
			  माप_प्रकार मूल्य)
{
	अगर (मूल्य > तथ्य->मूल्य || is_null_oid(&तथ्य->oid)) {
		oidcpy(&तथ्य->oid, oid);
		तथ्य->मूल्य = मूल्य;
	}
}

अटल माप_प्रकार गिनती_वृक्ष_entries(काष्ठा object *obj)
{
	काष्ठा वृक्ष *t = object_as_type(obj, OBJ_वृक्ष, 0);
	काष्ठा name_entry entry;
	काष्ठा वृक्ष_desc desc;
	माप_प्रकार गिनती = 0;

	init_वृक्ष_desc(&desc, &t->object.oid, t->बफर, t->आकार);
	जबतक (वृक्ष_entry(&desc, &entry))
		गिनती++;

	वापस गिनती;
}

अटल पूर्णांक गिनती_objects(स्थिर अक्षर *path UNUSED, काष्ठा oid_array *oids,
			 क्रमागत object_type type, व्योम *cb_तथ्य)
{
	काष्ठा गिनती_objects_तथ्य *तथ्य = cb_तथ्य;
	काष्ठा object_stats *stats = तथ्य->stats;
	माप_प्रकार object_गिनती;

	क्रम (माप_प्रकार i = 0; i < oids->nr; i++) {
		काष्ठा object_info oi = OBJECT_INFO_INIT;
		अचिन्हित दीर्घ inflated;
		माप_प्रकार inflated_st = 0;
		काष्ठा commit *commit;
		काष्ठा object *obj;
		व्योम *content;
		off_t disk;
		पूर्णांक eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		अगर (odb_read_object_info_extended(तथ्य->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_QUICK) < 0)
			जारी;
		inflated = cast_आकार_t_to_ulong(inflated_st);

		obj = parse_object_बफर(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		चयन (type) {
		हाल OBJ_TAG:
			stats->type_counts.tags++;
			stats->inflated_sizes.tags += inflated;
			stats->disk_sizes.tags += disk;
			check_largest(&stats->largest.tag_आकार, &oids->oid[i],
				      inflated);
			अवरोध;
		हाल OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_आकार, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_गिनती, &oids->oid[i],
				      commit_list_गिनती(commit->parents));
			अवरोध;
		हाल OBJ_वृक्ष:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.वृक्ष_आकार, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.वृक्ष_entries, &oids->oid[i],
				      गिनती_वृक्ष_entries(obj));
			अवरोध;
		हाल OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_आकार, &oids->oid[i],
				      inflated);
			अवरोध;
		शेष:
			BUG("invalid object type");
		}

		अगर (!eaten)
			मुक्त(content);
	}

	object_गिनती = get_योग_object_मूल्यावली(&stats->type_counts);
	display_progress(तथ्य->progress, object_गिनती);

	वापस 0;
}

अटल व्योम structure_गिनती_objects(काष्ठा object_stats *stats,
				    काष्ठा rev_info *revs,
				    काष्ठा repository *repo, पूर्णांक show_progress)
{
	काष्ठा path_walk_info info = PATH_WALK_INFO_INIT;
	काष्ठा गिनती_objects_तथ्य तथ्य = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = गिनती_objects;
	info.path_fn_तथ्य = &तथ्य;

	अगर (show_progress)
		तथ्य.progress = start_delayed_progress(repo, _("Counting objects"), 0);

	walk_objects_by_path(&info);
	path_walk_info_clear(&info);
	stop_progress(&तथ्य.progress);
}

अटल पूर्णांक cmd_repo_structure(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
			      काष्ठा repository *repo)
{
	काष्ठा stats_table table = {
		.rows = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	क्रमागत output_format format = FORMAT_TABLE;
	काष्ठा repo_structure stats = { 0 };
	काष्ठा rev_info revs;
	पूर्णांक show_progress = -1;
	काष्ठा option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', शून्य, &format, शून्य,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_progress, N_("show progress")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_structure_usage, 0);
	अगर (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, prefix);

	अगर (show_progress < 0)
		show_progress = isatty(2);

	structure_गिनती_references(&stats.refs, &revs, repo, show_progress);
	structure_गिनती_objects(&stats.objects, &revs, repo, show_progress);

	चयन (format) {
	हाल FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_print_structure(&table);
		अवरोध;
	हाल FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		अवरोध;
	हाल FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		अवरोध;
	शेष:
		BUG("invalid output format");
	}

	stats_table_clear(&table);
	release_revisions(&revs);

	वापस 0;
}

पूर्णांक cmd_repo(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
	     काष्ठा repository *repo)
{
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_usage, 0);

	वापस fn(argc, argv, prefix, repo);
}
