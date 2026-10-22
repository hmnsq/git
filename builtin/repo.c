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

स्थिर अचर वर्ण *अचर repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	रिक्त,
};

स्थिर अचर वर्ण *अचर repo_info_usage[] = {
	REPO_INFO_USAGE,
	रिक्त,
};

स्थिर अचर वर्ण *अचर repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	रिक्त,
};

प्रकारनिर्देश पूर्णाङ्क get_मान_fn(संरचना repository *repo, संरचना strbuf *buf);

गणना output_format {
	FORMAT_TABLE,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

संरचना repo_info_field {
	अचर वर्ण *साँचो;
	get_मान_fn *get_मान;
};

स्थिर पूर्णाङ्क get_layout_bare(संरचना repository *repo UNUSED, संरचना strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क get_layout_shallow(संरचना repository *repo, संरचना strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क get_object_format(संरचना repository *repo, संरचना strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क get_path_commondir_absolute(संरचना repository *repo, संरचना strbuf *buf)
{
	अचर वर्ण *common_dir = repo_get_common_dir(repo);

	यदि (!common_dir)
		फर्काऊ error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क get_path_commondir_relative(संरचना repository *repo, संरचना strbuf *buf)
{
	अचर वर्ण *common_dir = repo_get_common_dir(repo);

	यदि (!common_dir)
		फर्काऊ error(_("unable to get common directory"));

	format_path(buf, common_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क get_path_gitdir_absolute(संरचना repository *repo, संरचना strbuf *buf)
{
	अचर वर्ण *git_dir = repo_get_git_dir(repo);

	यदि (!git_dir)
		फर्काऊ error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_CANONICAL);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क get_path_gitdir_relative(संरचना repository *repo, संरचना strbuf *buf)
{
	अचर वर्ण *git_dir = repo_get_git_dir(repo);

	यदि (!git_dir)
		फर्काऊ error(_("unable to get git directory"));

	format_path(buf, git_dir, repo->prefix, PATH_FORMAT_RELATIVE);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क get_references_format(संरचना repository *repo, संरचना strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_to_name(repo->ref_storage_format));
	फर्काऊ 0;
}

/* repo_info_field keys must be in lexicographical order */
स्थिर अचर संरचना repo_info_field repo_info_field[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_path_commondir_absolute },
	{ "path.commondir.relative", get_path_commondir_relative },
	{ "path.gitdir.absolute", get_path_gitdir_absolute },
	{ "path.gitdir.relative", get_path_gitdir_relative },
	{ "references.format", get_references_format },
};

स्थिर पूर्णाङ्क repo_info_field_cmp(अचर शून्य *va, अचर शून्य *vb)
{
	अचर संरचना repo_info_field *a = va;
	अचर संरचना repo_info_field *b = vb;

	फर्काऊ म_भेद(a->साँचो, b->साँचो);
}

स्थिर अचर संरचना repo_info_field *get_repo_info_field(अचर वर्ण *साँचो)
{
	अचर संरचना repo_info_field खोज_साँचो = { साँचो, रिक्त };
	अचर संरचना repo_info_field *found = द्वा_खोज(&खोज_साँचो,
						      repo_info_field,
						      ARRAY_SIZE(repo_info_field),
						      परिमाण(*found),
						      repo_info_field_cmp);

	फर्काऊ found;
}

स्थिर शून्य print_field(गणना output_format format, अचर वर्ण *साँचो,
			अचर वर्ण *मान)
{
	विकल्प (format) {
	पक्ष FORMAT_NEWLINE_TERMINATED:
		म_लिखो("%s=", साँचो);
		quote_c_style(मान, रिक्त, मानक_निकास, 0);
		अक्षर_दो('\n');
		रोक;
	पक्ष FORMAT_NUL_TERMINATED:
		म_लिखो("%s\n%s%c", साँचो, मान, '\0');
		रोक;
	स्वभाव:
		BUG("not a valid output format: %d", format);
	}
}

स्थिर पूर्णाङ्क print_fields(पूर्णाङ्क argc, अचर वर्ण **argv,
			संरचना repository *repo,
			गणना output_format format)
{
	पूर्णाङ्क ret = 0;
	संरचना strbuf valbuf = STRBUF_INIT;

	लागि (पूर्णाङ्क i = 0; i < argc; i++) {
		अचर वर्ण *साँचो = argv[i];
		अचर संरचना repo_info_field *field = get_repo_info_field(साँचो);

		यदि (!field) {
			ret = error(_("key '%s' not found"), साँचो);
			जारी;
		}

		strbuf_reset(&valbuf);
		field->get_मान(repo, &valbuf);
		print_field(format, साँचो, valbuf.buf);
	}

	strbuf_release(&valbuf);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क print_all_fields(संरचना repository *repo,
			    गणना output_format format)
{
	संरचना strbuf valbuf = STRBUF_INIT;

	लागि (माप_प्रकार i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		अचर संरचना repo_info_field *field = &repo_info_field[i];

		strbuf_reset(&valbuf);
		field->get_मान(repo, &valbuf);
		print_field(format, field->साँचो, valbuf.buf);
	}

	strbuf_release(&valbuf);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क print_साँचोहरू(गणना output_format format)
{
	वर्ण sep;

	विकल्प (format) {
	पक्ष FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		रोक;
	पक्ष FORMAT_NUL_TERMINATED:
		sep = '\0';
		रोक;
	स्वभाव:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	लागि (माप_प्रकार i = 0; i < ARRAY_SIZE(repo_info_field); i++) {
		अचर संरचना repo_info_field *field = &repo_info_field[i];
		म_लिखो("%s%c", field->साँचो, sep);
	}

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क parse_format_cb(अचर संरचना option *opt,
			   अचर वर्ण *arg, पूर्णाङ्क unset UNUSED)
{
	गणना output_format *format = opt->मान;

	यदि (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	अन्यथा यदि (!म_भेद(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	अन्यथा यदि (!म_भेद(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	अन्यथा यदि (!म_भेद(arg, "table"))
		*format = FORMAT_TABLE;
	अन्यथा
		die(_("invalid format '%s'"), arg);

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क cmd_repo_info(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			 संरचना repository *repo)
{
	गणना output_format format = FORMAT_NEWLINE_TERMINATED;
	पूर्णाङ्क all_साँचोहरू = 0;
	पूर्णाङ्क show_साँचोहरू = 0;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', रिक्त, &format, रिक्त,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_साँचोहरू, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_साँचोहरू, N_("show keys")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_info_usage, 0);

	यदि (show_साँचोहरू && (all_साँचोहरू || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	यदि (show_साँचोहरू)
		फर्काऊ print_साँचोहरू(format);

	यदि (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	यदि (all_साँचोहरू && argc)
		die(_("--all and <key> cannot be used together"));

	यदि (all_साँचोहरू)
		फर्काऊ print_all_fields(repo, format);
	अन्यथा
		फर्काऊ print_fields(argc, argv, repo, format);
}

संरचना object_data {
	संरचना object_id oid;
	माप_प्रकार मान;
};

संरचना largest_objects {
	संरचना object_data tag_size;
	संरचना object_data commit_size;
	संरचना object_data रूख_size;
	संरचना object_data blob_size;

	संरचना object_data parent_गन्ती;
	संरचना object_data रूख_entries;
};

संरचना ref_stats {
	माप_प्रकार branches;
	माप_प्रकार remotes;
	माप_प्रकार tags;
	माप_प्रकार others;
};

संरचना object_मानहरू {
	माप_प्रकार tags;
	माप_प्रकार commits;
	माप_प्रकार trees;
	माप_प्रकार blobs;
};

संरचना object_stats {
	संरचना object_मानहरू type_counts;
	संरचना object_मानहरू inflated_sizes;
	संरचना object_मानहरू disk_sizes;
	संरचना largest_objects largest;
};

संरचना repo_structure {
	संरचना ref_stats refs;
	संरचना object_stats objects;
};

संरचना stats_table {
	संरचना string_list rows;
	संरचना string_list annotations;

	पूर्णाङ्क name_col_width;
	पूर्णाङ्क मान_col_width;
	पूर्णाङ्क unit_col_width;
};

/*
 * Holds column data that gets stored for each row.
 */
संरचना stats_table_entry {
	वर्ण *मान;
	अचर वर्ण *unit;
	माप_प्रकार index;
	संरचना object_id *oid;
};

स्थिर शून्य stats_table_vaddf(संरचना stats_table *table,
			      संरचना stats_table_entry *entry,
			      अचर वर्ण *format, बहु_सूची ap)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना string_list_item *item;
	वर्ण *formatted_name;
	पूर्णाङ्क name_width;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, रिक्त);
	name_width = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&table->rows, formatted_name);
	item->util = entry;

	यदि (name_width > table->name_col_width)
		table->name_col_width = name_width;
	यदि (!entry)
		फर्काऊ;
	यदि (entry->oid) {
		entry->index = table->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)entry->index,
			    oid_to_hex(entry->oid));
		string_list_append_nodup(&table->annotations, strbuf_detach(&buf, रिक्त));
	}
	यदि (entry->मान) {
		पूर्णाङ्क मान_width = utf8_strwidth(entry->मान);
		यदि (मान_width > table->मान_col_width)
			table->मान_col_width = मान_width;
	}
	यदि (entry->unit) {
		पूर्णाङ्क unit_width = utf8_strwidth(entry->unit);
		यदि (unit_width > table->unit_col_width)
			table->unit_col_width = unit_width;
	}

	strbuf_release(&buf);
}

स्थिर शून्य stats_table_addf(संरचना stats_table *table, अचर वर्ण *format, ...)
{
	बहु_सूची ap;

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, रिक्त, format, ap);
	बहु_पूर्ण(ap);
}

स्थिर शून्य stats_table_गन्ती_addf(संरचना stats_table *table, माप_प्रकार मान,
				   अचर वर्ण *format, ...)
{
	संरचना stats_table_entry *entry;
	बहु_सूची ap;

	CALLOC_ARRAY(entry, 1);
	humanise_गन्ती(मान, &entry->मान, &entry->unit);

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	बहु_पूर्ण(ap);
}

स्थिर शून्य stats_table_object_गन्ती_addf(संरचना stats_table *table,
					  संरचना object_id *oid, माप_प्रकार मान,
					  अचर वर्ण *format, ...)
{
	संरचना stats_table_entry *entry;
	बहु_सूची ap;

	CALLOC_ARRAY(entry, 1);
	humanise_गन्ती(मान, &entry->मान, &entry->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	यदि (!is_null_oid(oid))
		entry->oid = oid;

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	बहु_पूर्ण(ap);
}

स्थिर शून्य stats_table_size_addf(संरचना stats_table *table, माप_प्रकार मान,
				  अचर वर्ण *format, ...)
{
	संरचना stats_table_entry *entry;
	बहु_सूची ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(मान, &entry->मान, &entry->unit, HUMANISE_COMPACT);

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	बहु_पूर्ण(ap);
}

स्थिर शून्य stats_table_object_size_addf(संरचना stats_table *table,
					 संरचना object_id *oid, माप_प्रकार मान,
					 अचर वर्ण *format, ...)
{
	संरचना stats_table_entry *entry;
	बहु_सूची ap;

	CALLOC_ARRAY(entry, 1);
	humanise_bytes(मान, &entry->मान, &entry->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	यदि (!is_null_oid(oid))
		entry->oid = oid;

	बहु_शुरू(ap, format);
	stats_table_vaddf(table, entry, format, ap);
	बहु_पूर्ण(ap);
}

स्थिर अन्तःस्थ माप_प्रकार get_total_reference_गन्ती(संरचना ref_stats *stats)
{
	फर्काऊ stats->branches + stats->remotes + stats->tags + stats->others;
}

स्थिर अन्तःस्थ माप_प्रकार get_total_object_मानहरू(संरचना object_मानहरू *मानहरू)
{
	फर्काऊ मानहरू->tags + मानहरू->commits + मानहरू->trees + मानहरू->blobs;
}

स्थिर शून्य stats_table_setup_structure(संरचना stats_table *table,
					संरचना repo_structure *stats)
{
	संरचना object_stats *objects = &stats->objects;
	संरचना ref_stats *refs = &stats->refs;
	माप_प्रकार inflated_object_total;
	माप_प्रकार object_गन्ती_total;
	माप_प्रकार disk_object_total;
	माप_प्रकार ref_total;

	ref_total = get_total_reference_गन्ती(refs);
	stats_table_addf(table, "* %s", _("References"));
	stats_table_गन्ती_addf(table, ref_total, "  * %s", _("Count"));
	stats_table_गन्ती_addf(table, refs->branches, "    * %s", _("Branches"));
	stats_table_गन्ती_addf(table, refs->tags, "    * %s", _("Tags"));
	stats_table_गन्ती_addf(table, refs->remotes, "    * %s", _("Remotes"));
	stats_table_गन्ती_addf(table, refs->others, "    * %s", _("Others"));

	object_गन्ती_total = get_total_object_मानहरू(&objects->type_counts);
	stats_table_addf(table, "");
	stats_table_addf(table, "* %s", _("Reachable objects"));
	stats_table_गन्ती_addf(table, object_गन्ती_total, "  * %s", _("Count"));
	stats_table_गन्ती_addf(table, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_table_गन्ती_addf(table, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_table_गन्ती_addf(table, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_table_गन्ती_addf(table, objects->type_counts.tags,
			       "    * %s", _("Tags"));

	inflated_object_total = get_total_object_मानहरू(&objects->inflated_sizes);
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

	disk_object_total = get_total_object_मानहरू(&objects->disk_sizes);
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
				     objects->largest.commit_size.मान,
				     "    * %s", _("Maximum size"));
	stats_table_object_गन्ती_addf(table,
				      &objects->largest.parent_गन्ती.oid,
				      objects->largest.parent_गन्ती.मान,
				      "    * %s", _("Maximum parents"));
	stats_table_addf(table, "  * %s", _("Trees"));
	stats_table_object_size_addf(table,
				     &objects->largest.रूख_size.oid,
				     objects->largest.रूख_size.मान,
				     "    * %s", _("Maximum size"));
	stats_table_object_गन्ती_addf(table,
				      &objects->largest.रूख_entries.oid,
				      objects->largest.रूख_entries.मान,
				      "    * %s", _("Maximum entries"));
	stats_table_addf(table, "  * %s", _("Blobs"));
	stats_table_object_size_addf(table,
				     &objects->largest.blob_size.oid,
				     objects->largest.blob_size.मान,
				     "    * %s", _("Maximum size"));
	stats_table_addf(table, "  * %s", _("Tags"));
	stats_table_object_size_addf(table,
				     &objects->largest.tag_size.oid,
				     objects->largest.tag_size.मान,
				     "    * %s", _("Maximum size"));
}

#define INDEX_WIDTH 4

स्थिर शून्य stats_table_print_structure(अचर संरचना stats_table *table)
{
	अचर वर्ण *name_col_title = _("Repository structure");
	अचर वर्ण *मान_col_title = _("Value");
	पूर्णाङ्क title_name_width = utf8_strwidth(name_col_title);
	पूर्णाङ्क title_मान_width = utf8_strwidth(मान_col_title);
	पूर्णाङ्क name_col_width = table->name_col_width;
	पूर्णाङ्क मान_col_width = table->मान_col_width;
	पूर्णाङ्क unit_col_width = table->unit_col_width;
	संरचना string_list_item *item;
	संरचना strbuf buf = STRBUF_INIT;

	यदि (title_name_width > name_col_width)
		name_col_width = title_name_width;
	यदि (title_मान_width > मान_col_width + unit_col_width + 1)
		मान_col_width = title_मान_width - unit_col_width;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_बायाँ, name_col_width + INDEX_WIDTH,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_बायाँ,
			  मान_col_width + unit_col_width + 1, मान_col_title);
	strbuf_addstr(&buf, " |");
	म_लिखो("%s\n", buf.buf);

	म_लिखो("| ");
	लागि (पूर्णाङ्क i = 0; i < name_col_width + INDEX_WIDTH; i++)
		अक्षर_दो('-');
	म_लिखो(" | ");
	लागि (पूर्णाङ्क i = 0; i < मान_col_width + unit_col_width + 1; i++)
		अक्षर_दो('-');
	म_लिखो(" |\n");

	for_each_string_list_item(item, &table->rows) {
		संरचना stats_table_entry *entry = item->util;
		अचर वर्ण *मान = "";
		अचर वर्ण *unit = "";

		यदि (entry) {
			मान = entry->मान;
			यदि (entry->unit)
				unit = entry->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_बायाँ, name_col_width, item->string);

		यदि (entry && entry->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)entry->index);
		अन्यथा
			strbuf_addchars(&buf, ' ', INDEX_WIDTH);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_दायाँ, मान_col_width, मान);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_बायाँ, unit_col_width, unit);
		strbuf_addstr(&buf, " |");
		म_लिखो("%s\n", buf.buf);
	}

	यदि (table->annotations.nr) {
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
		यदि (entry)
			मुक्त(entry->मान);
	}

	string_list_clear(&table->rows, 1);
	string_list_clear(&table->annotations, 1);
}

स्थिर अन्तःस्थ शून्य print_keyvalue(अचर वर्ण *साँचो, वर्ण साँचो_delim, माप_प्रकार मान,
				  वर्ण मान_delim)
{
	म_लिखो("%s%c%" PRIuMAX "%c", साँचो, साँचो_delim, (uintmax_t)मान,
	       मान_delim);
}

स्थिर शून्य print_object_data(अचर वर्ण *साँचो, वर्ण साँचो_delim,
			      संरचना object_data *data, वर्ण मान_delim)
{
	print_keyvalue(साँचो, साँचो_delim, data->मान, मान_delim);
	म_लिखो("%s_oid%c%s%c", साँचो, साँचो_delim, oid_to_hex(&data->oid),
	       मान_delim);
}

स्थिर शून्य structure_keyvalue_print(संरचना repo_structure *stats,
				     वर्ण साँचो_delim, वर्ण मान_delim)
{
	print_keyvalue("references.branches.count", साँचो_delim,
		       stats->refs.branches, मान_delim);
	print_keyvalue("references.tags.count", साँचो_delim,
		       stats->refs.tags, मान_delim);
	print_keyvalue("references.remotes.count", साँचो_delim,
		       stats->refs.remotes, मान_delim);
	print_keyvalue("references.others.count", साँचो_delim,
		       stats->refs.others, मान_delim);

	print_keyvalue("objects.commits.count", साँचो_delim,
		       stats->objects.type_counts.commits, मान_delim);
	print_keyvalue("objects.trees.count", साँचो_delim,
		       stats->objects.type_counts.trees, मान_delim);
	print_keyvalue("objects.blobs.count", साँचो_delim,
		       stats->objects.type_counts.blobs, मान_delim);
	print_keyvalue("objects.tags.count", साँचो_delim,
		       stats->objects.type_counts.tags, मान_delim);

	print_keyvalue("objects.commits.inflated_size", साँचो_delim,
		       stats->objects.inflated_sizes.commits, मान_delim);
	print_keyvalue("objects.trees.inflated_size", साँचो_delim,
		       stats->objects.inflated_sizes.trees, मान_delim);
	print_keyvalue("objects.blobs.inflated_size", साँचो_delim,
		       stats->objects.inflated_sizes.blobs, मान_delim);
	print_keyvalue("objects.tags.inflated_size", साँचो_delim,
		       stats->objects.inflated_sizes.tags, मान_delim);

	print_keyvalue("objects.commits.disk_size", साँचो_delim,
		       stats->objects.disk_sizes.commits, मान_delim);
	print_keyvalue("objects.trees.disk_size", साँचो_delim,
		       stats->objects.disk_sizes.trees, मान_delim);
	print_keyvalue("objects.blobs.disk_size", साँचो_delim,
		       stats->objects.disk_sizes.blobs, मान_delim);
	print_keyvalue("objects.tags.disk_size", साँचो_delim,
		       stats->objects.disk_sizes.tags, मान_delim);

	print_object_data("objects.commits.max_size", साँचो_delim,
			  &stats->objects.largest.commit_size, मान_delim);
	print_object_data("objects.trees.max_size", साँचो_delim,
			  &stats->objects.largest.रूख_size, मान_delim);
	print_object_data("objects.blobs.max_size", साँचो_delim,
			  &stats->objects.largest.blob_size, मान_delim);
	print_object_data("objects.tags.max_size", साँचो_delim,
			  &stats->objects.largest.tag_size, मान_delim);

	print_object_data("objects.commits.max_parents", साँचो_delim,
			  &stats->objects.largest.parent_गन्ती, मान_delim);
	print_object_data("objects.trees.max_entries", साँचो_delim,
			  &stats->objects.largest.रूख_entries, मान_delim);

	ख_साफ(मानक_निकास);
}

संरचना गन्ती_references_data {
	संरचना ref_stats *stats;
	संरचना rev_info *revs;
	संरचना progress *progress;
};

स्थिर पूर्णाङ्क गन्ती_references(अचर संरचना reference *ref, शून्य *cb_data)
{
	संरचना गन्ती_references_data *data = cb_data;
	संरचना ref_stats *stats = data->stats;
	माप_प्रकार ref_गन्ती;

	विकल्प (ref_kind_from_refname(ref->name)) {
	पक्ष FILTER_REFS_BRANCHES:
		stats->branches++;
		रोक;
	पक्ष FILTER_REFS_REMOTES:
		stats->remotes++;
		रोक;
	पक्ष FILTER_REFS_TAGS:
		stats->tags++;
		रोक;
	पक्ष FILTER_REFS_OTHERS:
		stats->others++;
		रोक;
	स्वभाव:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	add_pending_oid(data->revs, रिक्त, ref->oid, 0);

	ref_गन्ती = get_total_reference_गन्ती(stats);
	display_progress(data->progress, ref_गन्ती);

	फर्काऊ 0;
}

स्थिर शून्य structure_गन्ती_references(संरचना ref_stats *stats,
				       संरचना rev_info *revs,
				       संरचना repository *repo,
				       पूर्णाङ्क show_progress)
{
	संरचना गन्ती_references_data data = {
		.stats = stats,
		.revs = revs,
	};

	यदि (show_progress)
		data.progress = start_delayed_progress(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), गन्ती_references, &data);
	stop_progress(&data.progress);
}

संरचना गन्ती_objects_data {
	संरचना object_database *odb;
	संरचना object_stats *stats;
	संरचना progress *progress;
};

स्थिर शून्य check_largest(संरचना object_data *data, संरचना object_id *oid,
			  माप_प्रकार मान)
{
	यदि (मान > data->मान || is_null_oid(&data->oid)) {
		oidcpy(&data->oid, oid);
		data->मान = मान;
	}
}

स्थिर माप_प्रकार गन्ती_रूख_entries(संरचना object *obj)
{
	संरचना रूख *t = object_as_type(obj, OBJ_रूख, 0);
	संरचना name_entry entry;
	संरचना रूख_desc desc;
	माप_प्रकार गन्ती = 0;

	init_रूख_desc(&desc, &t->object.oid, t->buffer, t->size);
	जबसम्म (रूख_entry(&desc, &entry))
		गन्ती++;

	फर्काऊ गन्ती;
}

स्थिर पूर्णाङ्क गन्ती_objects(अचर वर्ण *path UNUSED, संरचना oid_array *oids,
			 गणना object_type type, शून्य *cb_data)
{
	संरचना गन्ती_objects_data *data = cb_data;
	संरचना object_stats *stats = data->stats;
	माप_प्रकार object_गन्ती;

	लागि (माप_प्रकार i = 0; i < oids->nr; i++) {
		संरचना object_info oi = OBJECT_INFO_INIT;
		अचिन्हित दीर्घ inflated;
		माप_प्रकार inflated_st = 0;
		संरचना commit *commit;
		संरचना object *obj;
		शून्य *content;
		off_t disk;
		पूर्णाङ्क eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		यदि (odb_read_object_info_extended(data->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_QUICK) < 0)
			जारी;
		inflated = cast_size_t_to_ulong(inflated_st);

		obj = parse_object_buffer(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		विकल्प (type) {
		पक्ष OBJ_TAG:
			stats->type_counts.tags++;
			stats->inflated_sizes.tags += inflated;
			stats->disk_sizes.tags += disk;
			check_largest(&stats->largest.tag_size, &oids->oid[i],
				      inflated);
			रोक;
		पक्ष OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_गन्ती, &oids->oid[i],
				      commit_list_गन्ती(commit->parents));
			रोक;
		पक्ष OBJ_रूख:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.रूख_size, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.रूख_entries, &oids->oid[i],
				      गन्ती_रूख_entries(obj));
			रोक;
		पक्ष OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_size, &oids->oid[i],
				      inflated);
			रोक;
		स्वभाव:
			BUG("invalid object type");
		}

		यदि (!eaten)
			मुक्त(content);
	}

	object_गन्ती = get_total_object_मानहरू(&stats->type_counts);
	display_progress(data->progress, object_गन्ती);

	फर्काऊ 0;
}

स्थिर शून्य structure_गन्ती_objects(संरचना object_stats *stats,
				    संरचना rev_info *revs,
				    संरचना repository *repo, पूर्णाङ्क show_progress)
{
	संरचना path_walk_info info = PATH_WALK_INFO_INIT;
	संरचना गन्ती_objects_data data = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.path_fn = गन्ती_objects;
	info.path_fn_data = &data;

	यदि (show_progress)
		data.progress = start_delayed_progress(repo, _("Counting objects"), 0);

	walk_objects_by_path(&info);
	path_walk_info_clear(&info);
	stop_progress(&data.progress);
}

स्थिर पूर्णाङ्क cmd_repo_structure(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
			      संरचना repository *repo)
{
	संरचना stats_table table = {
		.rows = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	गणना output_format format = FORMAT_TABLE;
	संरचना repo_structure stats = { 0 };
	संरचना rev_info revs;
	पूर्णाङ्क show_progress = -1;
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
	यदि (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, prefix);

	यदि (show_progress < 0)
		show_progress = isatty(2);

	structure_गन्ती_references(&stats.refs, &revs, repo, show_progress);
	structure_गन्ती_objects(&stats.objects, &revs, repo, show_progress);

	विकल्प (format) {
	पक्ष FORMAT_TABLE:
		stats_table_setup_structure(&table, &stats);
		stats_table_print_structure(&table);
		रोक;
	पक्ष FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		रोक;
	पक्ष FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		रोक;
	स्वभाव:
		BUG("invalid output format");
	}

	stats_table_clear(&table);
	release_revisions(&revs);

	फर्काऊ 0;
}

पूर्णाङ्क cmd_repo(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix,
	     संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, repo_usage, 0);

	फर्काऊ fn(argc, argv, prefix, repo);
}
