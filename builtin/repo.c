#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "path-walk.h"
#अन्तर्भाव "progress.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "ref-filter.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "shallow.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "utf8.h"

#define REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#define REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

स्थिर स्थिरांक अक्षर *स्थिरांक repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	रिक्त,
};

स्थिर स्थिरांक अक्षर *स्थिरांक repo_info_usage[] = {
	REPO_INFO_USAGE,
	रिक्त,
};

स्थिर स्थिरांक अक्षर *स्थिरांक repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	रिक्त,
};

प्रकारव्याख्या पूर्णांक get_मूल्य_fn(संरचना repository *repo, संरचना strbuf *buf);

गणना output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

संरचना repo_info_field {
	स्थिरांक अक्षर *किल्ली;
	get_मूल्य_fn *get_मूल्य;
};

स्थिर पूर्णांक get_layout_bare(संरचना repository *repo UNUSED, संरचना strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	परत 0;
}

स्थिर पूर्णांक get_layout_shallow(संरचना repository *repo, संरचना strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	परत 0;
}

स्थिर पूर्णांक get_object_format(संरचना repository *repo, संरचना strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	परत 0;
}

स्थिर पूर्णांक get_path_commondir_absolute(संरचना repository *repo, संरचना strbuf *buf)
{
	स्थिरांक अक्षर *common_dir = repo_get_common_dir(repo);

	जर (!common_dir)
		परत error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	परत 0;
}

स्थिर पूर्णांक get_path_commondir_relative(संरचना repository *repo, संरचना strbuf *buf)
{
	स्थिरांक अक्षर *common_dir = repo_get_common_dir(repo);

	जर (!common_dir)
		परत error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	परत 0;
}

स्थिर पूर्णांक get_path_gitdir_absolute(संरचना repository *repo, संरचना strbuf *buf)
{
	स्थिरांक अक्षर *git_dir = repo_get_git_dir(repo);

	जर (!git_dir)
		परत error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	परत 0;
}

स्थिर पूर्णांक get_path_gitdir_relative(संरचना repository *repo, संरचना strbuf *buf)
{
	स्थिरांक अक्षर *git_dir = repo_get_git_dir(repo);

	जर (!git_dir)
		परत error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	परत 0;
}

स्थिर पूर्णांक get_references_format(संरचना repository *repo, संरचना strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_to_name(repo->ref_storage_format));
	परत 0;
}

/* repo_info_field keys must be in lexicographical order */
स्थिर स्थिरांक संरचना repo_info_field repo_info_field[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_path_commondir_absolute },
	{ "path.commondir.relative", get_path_commondir_relative },
	{ "path.gitdir.absolute", get_path_gitdir_absolute },
	{ "path.gitdir.relative", get_path_gitdir_relative },
	{ "references.format", get_references_format },
};

स्थिर पूर्णांक repo_info_field_cmp(स्थिरांक शून्य *va, स्थिरांक शून्य *vb)
{
	स्थिरांक संरचना repo_info_field *a = va;
	स्थिरांक संरचना repo_info_field *b = vb;

	परत म_भेद(a->किल्ली, b->किल्ली);
}

स्थिर स्थिरांक संरचना repo_info_field *get_repo_info_field(स्थिरांक अक्षर *किल्ली)
{
	स्थिरांक संरचना repo_info_field शोध_किल्ली = { किल्ली, रिक्त };
	स्थिरांक संरचना repo_info_field *found = द्वा_खोज(&शोध_किल्ली,
						      repo_info_field,
						      ARRAY_SIZE(repo_info_field),
						      आकार(*found),
						      repo_info_field_cmp);

	परत found;
}

स्थिर शून्य print_field(गणना output_format format, स्थिरांक अक्षर *किल्ली,
			स्थिरांक अक्षर *मूल्य)
{
	निवड (format) {
	स्थिती FORMAT_NEWLINE_TERMINATED:
		म_लिखो("%s=", किल्ली);
		quote_c_style(मूल्य, रिक्त, मानक_निकास, 0);
		अक्षर_दो('\n');
		थांबा;
	स्थिती FORMAT_NUL_TERMINATED:
		म_लिखो("%s\n%s%c", किल्ली, मूल्य, '\0');
		थांबा;
	पूर्वनिर्धारित:
		BUG("not a valid output format: %d", format);
	}
}

स्थिर पूर्णांक print_fields(पूर्णांक argc, स्थिरांक अक्षर **argv,
			संरचना repository *repo,
			गणना output_format format)
{
	पूर्णांक ret = 0;
	संरचना strbuf valbuf = STRBUF_INIT;

	साठी (पूर्णांक i = 0; i < argc; i++) {
		स्थिरांक अक्षर *किल्ली = argv[i];
		स्थिरांक संरचना repo_info_field *field = get_repo_info_field(किल्ली);

		जर (!field) {
			ret = error(_("key '%s' not found"), किल्ली);
			सुरूठेवा;
		}

		strbuf_reset(&valbuf);
		field->get_मूल्य(repo, &valbuf);
		print_field(format, किल्ली, valbuf.buf);
	}

	strbuf_release(&valbuf);
	परत ret;
}

स्थिर पूर्णांक print_all_fields(संरचना repository *repo,
			    गणना output_format format)
{
	संरचना strbuf valbuf = STRBUF_INIT;

	साठी (माप_प्रकार i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		स्थिरांक संरचना repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_मूल्य(repo, &valbuf);
		print_field(format, field->किल्ली, valbuf.buf);
	}

	strbuf_release(&valbuf);
	परत 0;
}

स्थिर पूर्णांक print_किल्ल्या(गणना output_format format)
{
	अक्षर sep;

	निवड (format) {
	स्थिती FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		थांबा;
	स्थिती FORMAT_NUL_TERMINATED:
		sep = '\0';
		थांबा;
	पूर्वनिर्धारित:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	साठी (माप_प्रकार i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		स्थिरांक संरचना repo_info_field *field = &repo_info_field[i];
		म_लिखो("%s%c", field->किल्ली, sep);
	}

	परत 0;
}

स्थिर पूर्णांक parse_format_cb(स्थिरांक संरचना option *opt,
			   स्थिरांक अक्षर *arg, पूर्णांक unset UNUSED)
{
	गणना output_format *format = opt->मूल्य;

	जर (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	अन्यथा जर (!म_भेद(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	अन्यथा जर (!म_भेद(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	अन्यथा जर (!म_भेद(arg, "table"))
		*format = FORMAT_TABLE;
	अन्यथा
		die(_("invalid format '%s'"), arg);

	परत 0;
}

स्थिर पूर्णांक cmd_repo_info(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
			 संरचना repository *repo)
{
	गणना output_format format = FORMAT_NEWLINE_TERMINATED;
	पूर्णांक all_किल्ल्या = 0;
	पूर्णांक show_किल्ल्या = 0;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', रिक्त, &format, रिक्त,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_किल्ल्या, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_किल्ल्या, N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	जर (show_किल्ल्या && (all_किल्ल्या || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	जर (show_किल्ल्या)
		परत print_किल्ल्या(format);

	जर (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	जर (all_किल्ल्या && argc)
		die(_("--all and <key> cannot be used together"));

	जर (all_किल्ल्या)
		परत print_all_fields(repo, format);
	अन्यथा
		परत print_fields(argc, argv, repo, format);
}

संरचना object_data {
	संरचना object_id oid;
	माप_प्रकार मूल्य;
};

संरचना largest_objects {
	संरचना object_data tag_size;
	संरचना object_data commit_size;
	संरचना object_data वृक्ष_size;
	संरचना object_data blob_size;

	संरचना object_data parent_मोजणी;
	संरचना object_data वृक्ष_entries;
};

संरचना ref_stats {
	माप_प्रकार branches;
	माप_प्रकार remotes;
	माप_प्रकार tags;
	माप_प्रकार others;
};

संरचना object_मूल्ये {
	माप_प्रकार tags;
	माप_प्रकार commits;
	माप_प्रकार trees;
	माप_प्रकार blobs;
};

संरचना object_stats {
	संरचना object_मूल्ये type_counts;
	संरचना object_मूल्ये inflated_sizes;
	संरचना object_मूल्ये disk_sizes;
	संरचना largest_objects largest;
};

संरचना repo_structure {
	संरचना ref_stats refs;
	संरचना object_stats objects;
};

संरचना stats_table {
	संरचना string_list rows;
	संरचना string_list annotations;

	पूर्णांक name_col_width;
	पूर्णांक मूल्य_col_width;
	पूर्णांक unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
संरचना stats_table_entry {
	अक्षर *मूल्य;
	स्थिरांक अक्षर *unit;
	माप_प्रकार index;
	संरचना object_id *oid;
};

स्थिर शून्य stats_table_vaddf(संरचना stats_table *table,
			      संरचना stats_table_entry *entry,
			      स्थिरांक अक्षर *format, बहु_सूची ap)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना string_list_item *item;
	अक्षर *formatted_name;
	पूर्णांक name_width;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, रिक्त);
	name_width = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&table->rows, formatted_name);
	item->util = entry;

	जर (name_width > table->name_col_width)
		table->name_col_width = name_width;
	जर (!entry)
		परत;
	जर (entry->oid) {
		entry->index = table->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)entry->index,
			    oid_to_hex(entry->oid));
		string_list_append_nodup(&table->annotations, strbuf_detach(&buf, रिक्त));
	}
	जर (entry->मूल्य) {
		पूर्णांक मूल्य_width = utf8_strwidth(entry->मूल्य);
		जर (मूल्य_width > table->मूल्य_col_width)
			table->मूल्य_col_width = मूल्य_width;
	}
	जर (entry->unit) {
		पूर्णांक unit_width = utf8_strwidth(entry->unit);
		जर (unit_width > table->unit_col_width)
			table->unit_col_width = unit_width;
	}

	strbuf_release(&buf);
}

स्थिर शून्य stats_table_addf(संरचना stats_table *table, स्थिरांक अक्षर *format, ...)
{
	बहु_सूची ap;

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, रिक्त, format, ap);
	बहु_पूर्ण(ap);
}

स्थिर शून्य stats_table_मोजणी_addf(संरचना stats_table *table, माप_प्रकार मूल्य,
				   स्थिरांक अक्षर *format, ...)
{
	संरचना stats_table_entry *entry;
	बहु_सूची ap;

	CALLOC_ARRAY(entry, 1);
	humanise_मोजणी(मूल्य, &entry->मूल्य, &entry->unit);

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	बहु_पूर्ण(ap);
}

स्थिर शून्य stats_table_object_मोजणी_addf(संरचना stats_table *table,
					  संरचना object_id *oid, माप_प्रकार मूल्य,
					  स्थिरांक अक्षर *format, ...)
{
	संरचना stats_table_entry *entry;
	बहु_सूची ap;

	CALLOC_ARRAY(entry, 1);
	humanise_मोजणी(मूल्य, &entry->मूल्य, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	जर (!is_null_oid(oid))
		entry->oid = oid;

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	बहु_पूर्ण(ap);
}

स्थिर शून्य stats_table_size_addf(संरचना stats_table *table, माप_प्रकार मूल्य,
				  स्थिरांक अक्षर *format, ...)
{
	संरचना stats_table_entry *entry;
	बहु_सूची ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(मूल्य, &entry->मूल्य, &entry->unit, HUMANISE_COMPACT);

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	बहु_पूर्ण(ap);
}

स्थिर शून्य stats_table_object_size_addf(संरचना stats_table *table,
					 संरचना object_id *oid, माप_प्रकार मूल्य,
					 स्थिरांक अक्षर *format, ...)
{
	संरचना stats_table_entry *entry;
	बहु_सूची ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(मूल्य, &entry->मूल्य, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	जर (!is_null_oid(oid))
		entry->oid = oid;

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	बहु_पूर्ण(ap);
}

स्थिर ओळीत माप_प्रकार get_total_reference_मोजणी(संरचना ref_stats *stats)
{
	परत stats->branches + stats->remotes + stats->tags + stats->others;
}

स्थिर ओळीत माप_प्रकार get_total_object_मूल्ये(संरचना object_मूल्ये *मूल्ये)
{
	परत मूल्ये->tags + मूल्ये->commits + मूल्ये->trees + मूल्ये->blobs;
}

स्थिर शून्य stats_table_setup_structure(संरचना stats_table *table,
					संरचना repo_structure *stats)
{
	संरचना object_stats *objects = &stats->objects;
	संरचना ref_stats *refs = &stats->refs;
	माप_प्रकार inflated_object_total;
	माप_प्रकार object_मोजणी_total;
	माप_प्रकार disk_object_total;
	माप_प्रकार ref_total;

	ref_total = get_total_reference_मोजणी(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_मोजणी_addf(table, ref_total, "  * %s", _("Count"));
	stats_table_मोजणी_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_मोजणी_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_मोजणी_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_मोजणी_addf(table, refs->others, "    * %s", _("Others"));

	object_मोजणी_total = get_total_object_मूल्ये(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_मोजणी_addf(table, object_मोजणी_total, "  * %s", _("Count"));
	stats_table_मोजणी_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_मोजणी_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_मोजणी_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_मोजणी_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_total = get_total_object_मूल्ये(&objects->inflated_sizes);
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

	disk_object_total = get_total_object_मूल्ये(&objects->disk_sizes);
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
				     objects->largest.commit_size.मूल्य,
				     "    * %s", _("Maximum size"));
	stats_table_object_मोजणी_addf(table,
				      &objects->largest.parent_मोजणी.oid,
				      objects->largest.parent_मोजणी.मूल्य,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_size_addf(table,
				     &objects->largest.वृक्ष_size.oid,
				     objects->largest.वृक्ष_size.मूल्य,
				     "    * %s", _("Maximum size"));
	stats_table_object_मोजणी_addf(table,
				      &objects->largest.वृक्ष_entries.oid,
				      objects->largest.वृक्ष_entries.मूल्य,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_size_addf(table,
				     &objects->largest.blob_size.oid,
				     objects->largest.blob_size.मूल्य,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_size_addf(table,
				     &objects->largest.tag_size.oid,
				     objects->largest.tag_size.मूल्य,
				     "    * %s", _("Maximum size"));
}

#define INDEX_WIDTH 4

स्थिर शून्य stats_table_print_structure(स्थिरांक संरचना stats_table *table)
{
	स्थिरांक अक्षर *name_col_title = _("Repository structure");
	स्थिरांक अक्षर *मूल्य_col_title = _("Value");
	पूर्णांक title_name_width = utf8_strwidth(name_col_title);
	पूर्णांक title_मूल्य_width = utf8_strwidth(मूल्य_col_title);
	पूर्णांक name_col_width = table->name_col_width;
	पूर्णांक मूल्य_col_width = table->मूल्य_col_width;
	पूर्णांक unit_col_width = table->unit_col_width;
	संरचना string_list_item *item;
	संरचना strbuf buf = STRBUF_INIT;

	जर (title_name_width > name_col_width)
		name_col_width = title_name_width;
	जर (title_मूल्य_width > मूल्य_col_width + unit_col_width + 1)
		मूल्य_col_width = title_मूल्य_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_डावा, name_col_width + INDEX_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_डावा,
			  मूल्य_col_width + unit_col_width + 1, मूल्य_col_title);
	strbuf_addstr(&buf, " |");
	म_लिखो("%s\n", buf.buf);

	म_लिखो("| ");
	साठी (पूर्णांक i = 0; i < name_col_width + INDEX_WIDTH; i++)
		अक्षर_दो('-');
	म_लिखो(" | ");
	साठी (पूर्णांक i = 0; i < मूल्य_col_width + unit_col_width + 1; i++)
		अक्षर_दो('-');
	म_लिखो(" |\n");

	for_each_string_list_item(item, &table->rows) {
		संरचना stats_table_entry *entry = item->util;
		स्थिरांक अक्षर *मूल्य = "";
		स्थिरांक अक्षर *unit = "";

		जर (entry) {
			मूल्य = entry->मूल्य;
			जर (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_डावा, name_col_width, item->string);

		जर (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->index);
		अन्यथा
			strbuf_addchars(&buf, ' ', INDEX_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_उजवा, मूल्य_col_width, मूल्य);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_डावा, unit_col_width, unit);
		strbuf_addstr(&buf, " |");
		म_लिखो("%s\n", buf.buf);
	}

	जर (table->annotations.nr) {
		म_लिखो("\n");
		for_each_string_list_item(item, &table->annotations)
			म_लिखो("%s\n", item->string);
	}

	strbuf_release(&buf);
}

स्थिर शून्य stats_table_clear(संरचना stats_table *table)
{
	संरचना stats_table_entry *entry;
	संरचना string_list_item *item;

	for_each_string_list_item(item, &table->rows) {
		entry = item->util;
		जर (entry)
			मुक्त(entry->मूल्य);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

स्थिर ओळीत शून्य print_keyvalue(स्थिरांक अक्षर *किल्ली, अक्षर किल्ली_delim, माप_प्रकार मूल्य,
				  अक्षर मूल्य_delim)
{
	म_लिखो("%s%c%" PRIuMAX "%c", किल्ली, किल्ली_delim, (uintmax_t)मूल्य,
	       मूल्य_delim);
}

स्थिर शून्य print_object_data(स्थिरांक अक्षर *किल्ली, अक्षर किल्ली_delim,
			      संरचना object_data *data, अक्षर मूल्य_delim)
{
	print_keyvalue(किल्ली, किल्ली_delim, data->मूल्य, मूल्य_delim);
	म_लिखो("%s_oid%c%s%c", किल्ली, किल्ली_delim, oid_to_hex(&data->oid),
	       मूल्य_delim);
}

स्थिर शून्य structure_keyvalue_print(संरचना repo_structure *stats,
				     अक्षर किल्ली_delim, अक्षर मूल्य_delim)
{
	print_keyvalue("references.branches.count", किल्ली_delim,
		       stats->refs.branches, मूल्य_delim);
	print_keyvalue("references.tags.count", किल्ली_delim,
		       stats->refs.tags, मूल्य_delim);
	print_keyvalue("references.remotes.count", किल्ली_delim,
		       stats->refs.remotes, मूल्य_delim);
	print_keyvalue("references.others.count", किल्ली_delim,
		       stats->refs.others, मूल्य_delim);

	print_keyvalue("objects.commits.count", किल्ली_delim,
		       stats->objects.type_counts.commits, मूल्य_delim);
	print_keyvalue("objects.trees.count", किल्ली_delim,
		       stats->objects.type_counts.trees, मूल्य_delim);
	print_keyvalue("objects.blobs.count", किल्ली_delim,
		       stats->objects.type_counts.blobs, मूल्य_delim);
	print_keyvalue("objects.tags.count", किल्ली_delim,
		       stats->objects.type_counts.tags, मूल्य_delim);

	print_keyvalue("objects.commits.inflated_size", किल्ली_delim,
		       stats->objects.inflated_sizes.commits, मूल्य_delim);
	print_keyvalue("objects.trees.inflated_size", किल्ली_delim,
		       stats->objects.inflated_sizes.trees, मूल्य_delim);
	print_keyvalue("objects.blobs.inflated_size", किल्ली_delim,
		       stats->objects.inflated_sizes.blobs, मूल्य_delim);
	print_keyvalue("objects.tags.inflated_size", किल्ली_delim,
		       stats->objects.inflated_sizes.tags, मूल्य_delim);

	print_keyvalue("objects.commits.disk_size", किल्ली_delim,
		       stats->objects.disk_sizes.commits, मूल्य_delim);
	print_keyvalue("objects.trees.disk_size", किल्ली_delim,
		       stats->objects.disk_sizes.trees, मूल्य_delim);
	print_keyvalue("objects.blobs.disk_size", किल्ली_delim,
		       stats->objects.disk_sizes.blobs, मूल्य_delim);
	print_keyvalue("objects.tags.disk_size", किल्ली_delim,
		       stats->objects.disk_sizes.tags, मूल्य_delim);

	print_object_data("objects.commits.max_size", किल्ली_delim,
			  &stats->objects.largest.commit_size, मूल्य_delim);
	print_object_data("objects.trees.max_size", किल्ली_delim,
			  &stats->objects.largest.वृक्ष_size, मूल्य_delim);
	print_object_data("objects.blobs.max_size", किल्ली_delim,
			  &stats->objects.largest.blob_size, मूल्य_delim);
	print_object_data("objects.tags.max_size", किल्ली_delim,
			  &stats->objects.largest.tag_size, मूल्य_delim);

	print_object_data("objects.commits.max_parents", किल्ली_delim,
			  &stats->objects.largest.parent_मोजणी, मूल्य_delim);
	print_object_data("objects.trees.max_entries", किल्ली_delim,
			  &stats->objects.largest.वृक्ष_entries, मूल्य_delim);

	ख_साफ(मानक_निकास);
}

संरचना मोजणी_references_data {
	संरचना ref_stats *stats;
	संरचना rev_info *revs;
	संरचना progress *progress;
};

स्थिर पूर्णांक मोजणी_references(स्थिरांक संरचना reference *ref, शून्य *cb_data)
{
	संरचना मोजणी_references_data *data = cb_data;
	संरचना ref_stats *stats = data->stats;
	माप_प्रकार ref_मोजणी;

	निवड (ref_kind_from_refname(ref->name)) {
	स्थिती FILTER_REFS_BRANCHES:
		stats->branches++;
		थांबा;
	स्थिती FILTER_REFS_REMOTES:
		stats->remotes++;
		थांबा;
	स्थिती FILTER_REFS_TAGS:
		stats->tags++;
		थांबा;
	स्थिती FILTER_REFS_OTHERS:
		stats->others++;
		थांबा;
	पूर्वनिर्धारित:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	add_pending_oid(data->revs, रिक्त, ref->oid, 0);

	ref_मोजणी = get_total_reference_मोजणी(stats);
	display_progress(data->progress, ref_मोजणी);

	परत 0;
}

स्थिर शून्य structure_मोजणी_references(संरचना ref_stats *stats,
				       संरचना rev_info *revs,
				       संरचना repository *repo,
				       पूर्णांक show_progress)
{
	संरचना मोजणी_references_data data = {
		.stats = stats,
		.revs = revs,
	};

	जर (show_progress)
		data.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), मोजणी_references, &data);
	stop_progress(&data.progress);
}

संरचना मोजणी_objects_data {
	संरचना object_database *odb;
	संरचना object_stats *stats;
	संरचना progress *progress;
};

स्थिर शून्य check_largest(संरचना object_data *data, संरचना object_id *oid,
			  माप_प्रकार मूल्य)
{
	जर (मूल्य > data->मूल्य || is_null_oid(&data->oid)) {
		oidcpy(&data->oid, oid);
		data->मूल्य = मूल्य;
	}
}

स्थिर माप_प्रकार मोजणी_वृक्ष_entries(संरचना object *obj)
{
	संरचना वृक्ष *t = object_as_type(obj, OBJ_वृक्ष, 0);
	संरचना name_entry entry;
	संरचना वृक्ष_desc desc;
	माप_प्रकार मोजणी = 0;

	init_वृक्ष_desc(&desc, &t->object.oid, t->buffer, t->size);
	पर्यंत (वृक्ष_entry(&desc, &entry))
		मोजणी++;

	परत मोजणी;
}

स्थिर पूर्णांक मोजणी_objects(स्थिरांक अक्षर *path UNUSED, संरचना oid_array *oids,
			 गणना object_type type, शून्य *cb_data)
{
	संरचना मोजणी_objects_data *data = cb_data;
	संरचना object_stats *stats = data->stats;
	माप_प्रकार object_मोजणी;

	साठी (माप_प्रकार i = 0; i < oids->nr; i++) {
		संरचना object_info oi = OBJECT_INFO_INIT;
		अचिन्हित दीर्घ inflated;
		माप_प्रकार inflated_st = 0;
		संरचना commit *commit;
		संरचना object *obj;
		शून्य *content;
		off_t disk;
		पूर्णांक eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		जर (odb_read_object_info_extended(data->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_QUICK) < 0)
			सुरूठेवा;
		inflated = cast_size_t_to_ulong(inflated_st);

		obj = parse_object_buffer(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		निवड (type) {
		स्थिती OBJ_TAG:
			stats->type_counts.tags++;
			stats->inflated_sizes.tags += inflated;
			stats->disk_sizes.tags += disk;
			check_largest(&stats->largest.tag_size, &oids->oid[i],
				      inflated);
			थांबा;
		स्थिती OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_मोजणी, &oids->oid[i],
				      commit_list_मोजणी(commit->parents));
			थांबा;
		स्थिती OBJ_वृक्ष:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.वृक्ष_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.वृक्ष_entries, &oids->oid[i],
				      मोजणी_वृक्ष_entries(obj));
			थांबा;
		स्थिती OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_size, &oids->oid[i],
				      inflated);
			थांबा;
		पूर्वनिर्धारित:
			BUG("invalid object type");
		}

		जर (!eaten)
			मुक्त(content);
	}

	object_मोजणी = get_total_object_मूल्ये(&stats->type_counts);
	display_progress(data->progress, object_मोजणी);

	परत 0;
}

स्थिर शून्य structure_मोजणी_objects(संरचना object_stats *stats,
				    संरचना rev_info *revs,
				    संरचना repository *repo, पूर्णांक show_progress)
{
	संरचना path_walk_info info = PATH_WALK_INFO_INIT;
	संरचना मोजणी_objects_data data = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = मोजणी_objects;
	info.path_fn_data = &data;

	जर (show_progress)
		data.progress = start_delayed_progress(repo, _("Counting objects"), 0);

	walk_objects_by_path(&info);
	path_walk_info_clear(&info);
	stop_progress(&data.progress);
}

स्थिर पूर्णांक cmd_repo_structure(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
			      संरचना repository *repo)
{
	संरचना stats_table table = {
		.rows = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	गणना output_format format = FORMAT_TABLE;
	संरचना repo_structure stats = { 0 };
	संरचना rev_info revs;
	पूर्णांक show_progress = -1;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', रिक्त, &format, रिक्त,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_progress, N_("show progress")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_structure_usage, 0);
	जर (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, prefix);

	जर (show_progress < 0)
		show_progress = isatty(2);

	structure_मोजणी_references(&stats.refs, &revs, repo, show_progress);
	structure_मोजणी_objects(&stats.objects, &revs, repo, show_progress);

	निवड (format) {
	स्थिती FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_print_structure(&table);
		थांबा;
	स्थिती FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		थांबा;
	स्थिती FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		थांबा;
	पूर्वनिर्धारित:
		BUG("invalid output format");
	}

	stats_table_clear(&table);
	release_revisions(&revs);

	परत 0;
}

पूर्णांक cmd_repo(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
	     संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_usage, 0);

	परत fn(argc, argv, prefix, repo);
}
