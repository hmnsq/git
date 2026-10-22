#परिभाषय USE_THE_REPOSITORY_VARIABLE

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

#परिभाषय REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#परिभाषय REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

स्थिर ध्रुव वर्ण *ध्रुव repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	रिक्त,
};

स्थिर ध्रुव वर्ण *ध्रुव repo_info_usage[] = {
	REPO_INFO_USAGE,
	रिक्त,
};

स्थिर ध्रुव वर्ण *ध्रुव repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	रिक्त,
};

प्रकारनिर्देश पूर्णाङ्क get_मानम्_fn(संरचना repository *repo, संरचना strbuf *buf);

गणना output_format {
	FORMAT_सारणी,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

संरचना repo_info_क्षेत्रम् {
	ध्रुव वर्ण *कुञ्चिका;
	get_मानम्_fn *get_मानम्;
};

स्थिर पूर्णाङ्क get_layout_bare(संरचना repository *repo अप्रयुक्तम्, संरचना strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_layout_shallow(संरचना repository *repo, संरचना strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_object_format(संरचना repository *repo, संरचना strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_मार्गः_commondir_absolute(संरचना repository *repo, संरचना strbuf *buf)
{
	ध्रुव वर्ण *common_dir = repo_get_common_dir(repo);

	यदि (!common_dir)
		प्रत्यागम error(_("unable to get common directory"));

	format_मार्गः(buf, common_dir, repo->उपसर्गः, मार्गः_FORMAT_CANONICAL);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_मार्गः_commondir_relative(संरचना repository *repo, संरचना strbuf *buf)
{
	ध्रुव वर्ण *common_dir = repo_get_common_dir(repo);

	यदि (!common_dir)
		प्रत्यागम error(_("unable to get common directory"));

	format_मार्गः(buf, common_dir, repo->उपसर्गः, मार्गः_FORMAT_RELATIVE);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_मार्गः_gitdir_absolute(संरचना repository *repo, संरचना strbuf *buf)
{
	ध्रुव वर्ण *git_dir = repo_get_git_dir(repo);

	यदि (!git_dir)
		प्रत्यागम error(_("unable to get git directory"));

	format_मार्गः(buf, git_dir, repo->उपसर्गः, मार्गः_FORMAT_CANONICAL);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_मार्गः_gitdir_relative(संरचना repository *repo, संरचना strbuf *buf)
{
	ध्रुव वर्ण *git_dir = repo_get_git_dir(repo);

	यदि (!git_dir)
		प्रत्यागम error(_("unable to get git directory"));

	format_मार्गः(buf, git_dir, repo->उपसर्गः, मार्गः_FORMAT_RELATIVE);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_references_format(संरचना repository *repo, संरचना strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_गन्तव्यम्_name(repo->ref_storage_format));
	प्रत्यागम 0;
}

/* repo_info_field keys must be in lexicographical order */
स्थिर ध्रुव संरचना repo_info_क्षेत्रम् repo_info_क्षेत्रम्[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_मार्गः_commondir_absolute },
	{ "path.commondir.relative", get_मार्गः_commondir_relative },
	{ "path.gitdir.absolute", get_मार्गः_gitdir_absolute },
	{ "path.gitdir.relative", get_मार्गः_gitdir_relative },
	{ "references.format", get_references_format },
};

स्थिर पूर्णाङ्क repo_info_क्षेत्रम्_cmp(ध्रुव शून्य *va, ध्रुव शून्य *vb)
{
	ध्रुव संरचना repo_info_क्षेत्रम् *a = va;
	ध्रुव संरचना repo_info_क्षेत्रम् *b = vb;

	प्रत्यागम वाक्यं_तुलय(a->कुञ्चिका, b->कुञ्चिका);
}

स्थिर ध्रुव संरचना repo_info_क्षेत्रम् *get_repo_info_क्षेत्रम्(ध्रुव वर्ण *कुञ्चिका)
{
	ध्रुव संरचना repo_info_क्षेत्रम् अन्वेषणम्_कुञ्चिका = { कुञ्चिका, रिक्त };
	ध्रुव संरचना repo_info_क्षेत्रम् *प्राप्तम्_2 = bsearch(&अन्वेषणम्_कुञ्चिका,
						      repo_info_क्षेत्रम्,
						      ARRAY_आकार(repo_info_क्षेत्रम्),
						      परिमाण(*प्राप्तम्_2),
						      repo_info_क्षेत्रम्_cmp);

	प्रत्यागम प्राप्तम्_2;
}

स्थिर शून्य print_क्षेत्रम्(गणना output_format format, ध्रुव वर्ण *कुञ्चिका,
			ध्रुव वर्ण *मानम्)
{
	विकल्प (format) {
	पक्ष FORMAT_NEWLINE_TERMINATED:
		प्रारूप_लिख("%s=", कुञ्चिका);
		quote_c_style(मानम्, रिक्त, stdout, 0);
		अक्षरं_लिख('\n');
		भङ्ग;
	पक्ष FORMAT_NUL_TERMINATED:
		प्रारूप_लिख("%s\n%s%c", कुञ्चिका, मानम्, '\0');
		भङ्ग;
	स्वभाव:
		BUG("not a valid output format: %d", format);
	}
}

स्थिर पूर्णाङ्क print_fields(पूर्णाङ्क argc, ध्रुव वर्ण **argv,
			संरचना repository *repo,
			गणना output_format format)
{
	पूर्णाङ्क ret = 0;
	संरचना strbuf valbuf = STRBUF_INIT;

	प्रति (पूर्णाङ्क i = 0; i < argc; i++) {
		ध्रुव वर्ण *कुञ्चिका = argv[i];
		ध्रुव संरचना repo_info_क्षेत्रम् *क्षेत्रम् = get_repo_info_क्षेत्रम्(कुञ्चिका);

		यदि (!क्षेत्रम्) {
			ret = error(_("key '%s' not found"), कुञ्चिका);
			अनुवर्त;
		}

		strbuf_reset(&valbuf);
		क्षेत्रम्->get_मानम्(repo, &valbuf);
		print_क्षेत्रम्(format, कुञ्चिका, valbuf.buf);
	}

	strbuf_release(&valbuf);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क print_all_fields(संरचना repository *repo,
			    गणना output_format format)
{
	संरचना strbuf valbuf = STRBUF_INIT;

	प्रति (आकार_t i = 0; i < ARRAY_आकार(repo_info_क्षेत्रम्); i++) {
		ध्रुव संरचना repo_info_क्षेत्रम् *क्षेत्रम् = &repo_info_क्षेत्रम्[i];

		strbuf_reset(&valbuf);
		क्षेत्रम्->get_मानम्(repo, &valbuf);
		print_क्षेत्रम्(format, क्षेत्रम्->कुञ्चिका, valbuf.buf);
	}

	strbuf_release(&valbuf);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क print_कुञ्चिकाः(गणना output_format format)
{
	वर्ण sep;

	विकल्प (format) {
	पक्ष FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		भङ्ग;
	पक्ष FORMAT_NUL_TERMINATED:
		sep = '\0';
		भङ्ग;
	स्वभाव:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	प्रति (आकार_t i = 0; i < ARRAY_आकार(repo_info_क्षेत्रम्); i++) {
		ध्रुव संरचना repo_info_क्षेत्रम् *क्षेत्रम् = &repo_info_क्षेत्रम्[i];
		प्रारूप_लिख("%s%c", क्षेत्रम्->कुञ्चिका, sep);
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_format_cb(ध्रुव संरचना option *opt,
			   ध्रुव वर्ण *arg, पूर्णाङ्क unset अप्रयुक्तम्)
{
	गणना output_format *format = opt->मानम्;

	यदि (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "table"))
		*format = FORMAT_सारणी;
	अन्यथा
		die(_("invalid format '%s'"), arg);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cmd_repo_info(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			 संरचना repository *repo)
{
	गणना output_format format = FORMAT_NEWLINE_TERMINATED;
	पूर्णाङ्क all_कुञ्चिकाः = 0;
	पूर्णाङ्क show_कुञ्चिकाः = 0;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', रिक्त, &format, रिक्त,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_कुञ्चिकाः, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_कुञ्चिकाः, N_("show keys")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options, repo_info_usage, 0);

	यदि (show_कुञ्चिकाः && (all_कुञ्चिकाः || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	यदि (show_कुञ्चिकाः)
		प्रत्यागम print_कुञ्चिकाः(format);

	यदि (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	यदि (all_कुञ्चिकाः && argc)
		die(_("--all and <key> cannot be used together"));

	यदि (all_कुञ्चिकाः)
		प्रत्यागम print_all_fields(repo, format);
	अन्यथा
		प्रत्यागम print_fields(argc, argv, repo, format);
}

संरचना object_दत्त {
	संरचना object_id oid;
	आकार_t मानम्;
};

संरचना largest_objects {
	संरचना object_दत्त चिह्नकम्_आकार;
	संरचना object_दत्त commit_आकार;
	संरचना object_दत्त वृक्षः_आकार;
	संरचना object_दत्त blob_आकार;

	संरचना object_दत्त parent_गणनम्;
	संरचना object_दत्त वृक्षः_entries;
};

संरचना ref_stats {
	आकार_t branches;
	आकार_t remotes;
	आकार_t चिह्नकानि;
	आकार_t others;
};

संरचना object_मानानि {
	आकार_t चिह्नकानि;
	आकार_t commits;
	आकार_t trees;
	आकार_t blobs;
};

संरचना object_stats {
	संरचना object_मानानि type_counts;
	संरचना object_मानानि inflated_sizes;
	संरचना object_मानानि disk_sizes;
	संरचना largest_objects largest;
};

संरचना repo_structure {
	संरचना ref_stats refs;
	संरचना object_stats objects;
};

संरचना stats_सारणी {
	संरचना string_list पङ्क्तिकाः;
	संरचना string_list annotations;

	पूर्णाङ्क name_col_विस्तारः;
	पूर्णाङ्क मानम्_col_विस्तारः;
	पूर्णाङ्क unit_col_विस्तारः;
};

/*
 * Holds column data that gets stored for each row.
 */
संरचना stats_सारणी_प्रविष्टिः {
	वर्ण *मानम्;
	ध्रुव वर्ण *unit;
	आकार_t सूचकांक;
	संरचना object_id *oid;
};

स्थिर शून्य stats_सारणी_vaddf(संरचना stats_सारणी *सारणी,
			      संरचना stats_सारणी_प्रविष्टिः *प्रविष्टिः,
			      ध्रुव वर्ण *format, va_list ap)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना string_list_item *item;
	वर्ण *formatted_name;
	पूर्णाङ्क name_विस्तारः;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, रिक्त);
	name_विस्तारः = utf8_strwidth(formatted_name);

	item = string_list_append_nodup(&सारणी->पङ्क्तिकाः, formatted_name);
	item->util = प्रविष्टिः;

	यदि (name_विस्तारः > सारणी->name_col_विस्तारः)
		सारणी->name_col_विस्तारः = name_विस्तारः;
	यदि (!प्रविष्टिः)
		प्रत्यागम;
	यदि (प्रविष्टिः->oid) {
		प्रविष्टिः->सूचकांक = सारणी->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)प्रविष्टिः->सूचकांक,
			    oid_गन्तव्यम्_hex(प्रविष्टिः->oid));
		string_list_append_nodup(&सारणी->annotations, strbuf_detach(&buf, रिक्त));
	}
	यदि (प्रविष्टिः->मानम्) {
		पूर्णाङ्क मानम्_विस्तारः = utf8_strwidth(प्रविष्टिः->मानम्);
		यदि (मानम्_विस्तारः > सारणी->मानम्_col_विस्तारः)
			सारणी->मानम्_col_विस्तारः = मानम्_विस्तारः;
	}
	यदि (प्रविष्टिः->unit) {
		पूर्णाङ्क unit_विस्तारः = utf8_strwidth(प्रविष्टिः->unit);
		यदि (unit_विस्तारः > सारणी->unit_col_विस्तारः)
			सारणी->unit_col_विस्तारः = unit_विस्तारः;
	}

	strbuf_release(&buf);
}

स्थिर शून्य stats_सारणी_addf(संरचना stats_सारणी *सारणी, ध्रुव वर्ण *format, ...)
{
	va_list ap;

	va_start(ap, format);
	stats_सारणी_vaddf(सारणी, रिक्त, format, ap);
	va_अन्तः(ap);
}

स्थिर शून्य stats_सारणी_गणनम्_addf(संरचना stats_सारणी *सारणी, आकार_t मानम्,
				   ध्रुव वर्ण *format, ...)
{
	संरचना stats_सारणी_प्रविष्टिः *प्रविष्टिः;
	va_list ap;

	CALLOC_ARRAY(प्रविष्टिः, 1);
	humanise_गणनम्(मानम्, &प्रविष्टिः->मानम्, &प्रविष्टिः->unit);

	va_start(ap, format);
	stats_सारणी_vaddf(सारणी, प्रविष्टिः, format, ap);
	va_अन्तः(ap);
}

स्थिर शून्य stats_सारणी_object_गणनम्_addf(संरचना stats_सारणी *सारणी,
					  संरचना object_id *oid, आकार_t मानम्,
					  ध्रुव वर्ण *format, ...)
{
	संरचना stats_सारणी_प्रविष्टिः *प्रविष्टिः;
	va_list ap;

	CALLOC_ARRAY(प्रविष्टिः, 1);
	humanise_गणनम्(मानम्, &प्रविष्टिः->मानम्, &प्रविष्टिः->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	यदि (!is_null_oid(oid))
		प्रविष्टिः->oid = oid;

	va_start(ap, format);
	stats_सारणी_vaddf(सारणी, प्रविष्टिः, format, ap);
	va_अन्तः(ap);
}

स्थिर शून्य stats_सारणी_आकार_addf(संरचना stats_सारणी *सारणी, आकार_t मानम्,
				  ध्रुव वर्ण *format, ...)
{
	संरचना stats_सारणी_प्रविष्टिः *प्रविष्टिः;
	va_list ap;

	CALLOC_ARRAY(प्रविष्टिः, 1);
	humanise_अष्टकानि(मानम्, &प्रविष्टिः->मानम्, &प्रविष्टिः->unit, HUMANISE_COMPACT);

	va_start(ap, format);
	stats_सारणी_vaddf(सारणी, प्रविष्टिः, format, ap);
	va_अन्तः(ap);
}

स्थिर शून्य stats_सारणी_object_आकार_addf(संरचना stats_सारणी *सारणी,
					 संरचना object_id *oid, आकार_t मानम्,
					 ध्रुव वर्ण *format, ...)
{
	संरचना stats_सारणी_प्रविष्टिः *प्रविष्टिः;
	va_list ap;

	CALLOC_ARRAY(प्रविष्टिः, 1);
	humanise_अष्टकानि(मानम्, &प्रविष्टिः->मानम्, &प्रविष्टिः->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	यदि (!is_null_oid(oid))
		प्रविष्टिः->oid = oid;

	va_start(ap, format);
	stats_सारणी_vaddf(सारणी, प्रविष्टिः, format, ap);
	va_अन्तः(ap);
}

स्थिर अन्तःस्थ आकार_t get_योग_reference_गणनम्(संरचना ref_stats *stats)
{
	प्रत्यागम stats->branches + stats->remotes + stats->चिह्नकानि + stats->others;
}

स्थिर अन्तःस्थ आकार_t get_योग_object_मानानि(संरचना object_मानानि *मानानि)
{
	प्रत्यागम मानानि->चिह्नकानि + मानानि->commits + मानानि->trees + मानानि->blobs;
}

स्थिर शून्य stats_सारणी_सज्जता_structure(संरचना stats_सारणी *सारणी,
					संरचना repo_structure *stats)
{
	संरचना object_stats *objects = &stats->objects;
	संरचना ref_stats *refs = &stats->refs;
	आकार_t inflated_object_योग;
	आकार_t object_गणनम्_योग;
	आकार_t disk_object_योग;
	आकार_t ref_योग;

	ref_योग = get_योग_reference_गणनम्(refs);
	stats_सारणी_addf(सारणी, "* %s", _("References"));
	stats_सारणी_गणनम्_addf(सारणी, ref_योग, "  * %s", _("Count"));
	stats_सारणी_गणनम्_addf(सारणी, refs->branches, "    * %s", _("Branches"));
	stats_सारणी_गणनम्_addf(सारणी, refs->चिह्नकानि, "    * %s", _("Tags"));
	stats_सारणी_गणनम्_addf(सारणी, refs->remotes, "    * %s", _("Remotes"));
	stats_सारणी_गणनम्_addf(सारणी, refs->others, "    * %s", _("Others"));

	object_गणनम्_योग = get_योग_object_मानानि(&objects->type_counts);
	stats_सारणी_addf(सारणी, "");
	stats_सारणी_addf(सारणी, "* %s", _("Reachable objects"));
	stats_सारणी_गणनम्_addf(सारणी, object_गणनम्_योग, "  * %s", _("Count"));
	stats_सारणी_गणनम्_addf(सारणी, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_सारणी_गणनम्_addf(सारणी, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_सारणी_गणनम्_addf(सारणी, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_सारणी_गणनम्_addf(सारणी, objects->type_counts.चिह्नकानि,
			       "    * %s", _("Tags"));

	inflated_object_योग = get_योग_object_मानानि(&objects->inflated_sizes);
	stats_सारणी_आकार_addf(सारणी, inflated_object_योग,
			      "  * %s", _("Inflated size"));
	stats_सारणी_आकार_addf(सारणी, objects->inflated_sizes.commits,
			      "    * %s", _("Commits"));
	stats_सारणी_आकार_addf(सारणी, objects->inflated_sizes.trees,
			      "    * %s", _("Trees"));
	stats_सारणी_आकार_addf(सारणी, objects->inflated_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_सारणी_आकार_addf(सारणी, objects->inflated_sizes.चिह्नकानि,
			      "    * %s", _("Tags"));

	disk_object_योग = get_योग_object_मानानि(&objects->disk_sizes);
	stats_सारणी_आकार_addf(सारणी, disk_object_योग,
			      "  * %s", _("Disk size"));
	stats_सारणी_आकार_addf(सारणी, objects->disk_sizes.commits,
			      "    * %s", _("Commits"));
	stats_सारणी_आकार_addf(सारणी, objects->disk_sizes.trees,
			      "    * %s", _("Trees"));
	stats_सारणी_आकार_addf(सारणी, objects->disk_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_सारणी_आकार_addf(सारणी, objects->disk_sizes.चिह्नकानि,
			      "    * %s", _("Tags"));

	stats_सारणी_addf(सारणी, "");
	stats_सारणी_addf(सारणी, "* %s", _("Largest objects"));
	stats_सारणी_addf(सारणी, "  * %s", _("Commits"));
	stats_सारणी_object_आकार_addf(सारणी,
				     &objects->largest.commit_आकार.oid,
				     objects->largest.commit_आकार.मानम्,
				     "    * %s", _("Maximum size"));
	stats_सारणी_object_गणनम्_addf(सारणी,
				      &objects->largest.parent_गणनम्.oid,
				      objects->largest.parent_गणनम्.मानम्,
				      "    * %s", _("Maximum parents"));
	stats_सारणी_addf(सारणी, "  * %s", _("Trees"));
	stats_सारणी_object_आकार_addf(सारणी,
				     &objects->largest.वृक्षः_आकार.oid,
				     objects->largest.वृक्षः_आकार.मानम्,
				     "    * %s", _("Maximum size"));
	stats_सारणी_object_गणनम्_addf(सारणी,
				      &objects->largest.वृक्षः_entries.oid,
				      objects->largest.वृक्षः_entries.मानम्,
				      "    * %s", _("Maximum entries"));
	stats_सारणी_addf(सारणी, "  * %s", _("Blobs"));
	stats_सारणी_object_आकार_addf(सारणी,
				     &objects->largest.blob_आकार.oid,
				     objects->largest.blob_आकार.मानम्,
				     "    * %s", _("Maximum size"));
	stats_सारणी_addf(सारणी, "  * %s", _("Tags"));
	stats_सारणी_object_आकार_addf(सारणी,
				     &objects->largest.चिह्नकम्_आकार.oid,
				     objects->largest.चिह्नकम्_आकार.मानम्,
				     "    * %s", _("Maximum size"));
}

#परिभाषय सूचकांक_विस्तारः 4

स्थिर शून्य stats_सारणी_print_structure(ध्रुव संरचना stats_सारणी *सारणी)
{
	ध्रुव वर्ण *name_col_title = _("Repository structure");
	ध्रुव वर्ण *मानम्_col_title = _("Value");
	पूर्णाङ्क title_name_विस्तारः = utf8_strwidth(name_col_title);
	पूर्णाङ्क title_मानम्_विस्तारः = utf8_strwidth(मानम्_col_title);
	पूर्णाङ्क name_col_विस्तारः = सारणी->name_col_विस्तारः;
	पूर्णाङ्क मानम्_col_विस्तारः = सारणी->मानम्_col_विस्तारः;
	पूर्णाङ्क unit_col_विस्तारः = सारणी->unit_col_विस्तारः;
	संरचना string_list_item *item;
	संरचना strbuf buf = STRBUF_INIT;

	यदि (title_name_विस्तारः > name_col_विस्तारः)
		name_col_विस्तारः = title_name_विस्तारः;
	यदि (title_मानम्_विस्तारः > मानम्_col_विस्तारः + unit_col_विस्तारः + 1)
		मानम्_col_विस्तारः = title_मानम्_विस्तारः - unit_col_विस्तारः;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_वामः, name_col_विस्तारः + सूचकांक_विस्तारः,
			  name_col_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_वामः,
			  मानम्_col_विस्तारः + unit_col_विस्तारः + 1, मानम्_col_title);
	strbuf_addstr(&buf, " |");
	प्रारूप_लिख("%s\n", buf.buf);

	प्रारूप_लिख("| ");
	प्रति (पूर्णाङ्क i = 0; i < name_col_विस्तारः + सूचकांक_विस्तारः; i++)
		अक्षरं_लिख('-');
	प्रारूप_लिख(" | ");
	प्रति (पूर्णाङ्क i = 0; i < मानम्_col_विस्तारः + unit_col_विस्तारः + 1; i++)
		अक्षरं_लिख('-');
	प्रारूप_लिख(" |\n");

	for_each_string_list_item(item, &सारणी->पङ्क्तिकाः) {
		संरचना stats_सारणी_प्रविष्टिः *प्रविष्टिः = item->util;
		ध्रुव वर्ण *मानम् = "";
		ध्रुव वर्ण *unit = "";

		यदि (प्रविष्टिः) {
			मानम् = प्रविष्टिः->मानम्;
			यदि (प्रविष्टिः->unit)
				unit = प्रविष्टिः->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_वामः, name_col_विस्तारः, item->string);

		यदि (प्रविष्टिः && प्रविष्टिः->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)प्रविष्टिः->सूचकांक);
		अन्यथा
			strbuf_addchars(&buf, ' ', सूचकांक_विस्तारः);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_दक्षिणः, मानम्_col_विस्तारः, मानम्);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_वामः, unit_col_विस्तारः, unit);
		strbuf_addstr(&buf, " |");
		प्रारूप_लिख("%s\n", buf.buf);
	}

	यदि (सारणी->annotations.nr) {
		प्रारूप_लिख("\n");
		for_each_string_list_item(item, &सारणी->annotations)
			प्रारूप_लिख("%s\n", item->string);
	}

	strbuf_release(&buf);
}

स्थिर शून्य stats_सारणी_clear(संरचना stats_सारणी *सारणी)
{
	संरचना stats_सारणी_प्रविष्टिः *प्रविष्टिः;
	संरचना string_list_item *item;

	for_each_string_list_item(item, &सारणी->पङ्क्तिकाः) {
		प्रविष्टिः = item->util;
		यदि (प्रविष्टिः)
			मुक्त(प्रविष्टिः->मानम्);
	}

	string_list_clear(&सारणी->पङ्क्तिकाः, 1);
	string_list_clear(&सारणी->annotations, 1);
}

स्थिर अन्तःस्थ शून्य print_keyvalue(ध्रुव वर्ण *कुञ्चिका, वर्ण कुञ्चिका_delim, आकार_t मानम्,
				  वर्ण मानम्_delim)
{
	प्रारूप_लिख("%s%c%" PRIuMAX "%c", कुञ्चिका, कुञ्चिका_delim, (uintmax_t)मानम्,
	       मानम्_delim);
}

स्थिर शून्य print_object_दत्त(ध्रुव वर्ण *कुञ्चिका, वर्ण कुञ्चिका_delim,
			      संरचना object_दत्त *दत्त, वर्ण मानम्_delim)
{
	print_keyvalue(कुञ्चिका, कुञ्चिका_delim, दत्त->मानम्, मानम्_delim);
	प्रारूप_लिख("%s_oid%c%s%c", कुञ्चिका, कुञ्चिका_delim, oid_गन्तव्यम्_hex(&दत्त->oid),
	       मानम्_delim);
}

स्थिर शून्य structure_keyvalue_print(संरचना repo_structure *stats,
				     वर्ण कुञ्चिका_delim, वर्ण मानम्_delim)
{
	print_keyvalue("references.branches.count", कुञ्चिका_delim,
		       stats->refs.branches, मानम्_delim);
	print_keyvalue("references.tags.count", कुञ्चिका_delim,
		       stats->refs.चिह्नकानि, मानम्_delim);
	print_keyvalue("references.remotes.count", कुञ्चिका_delim,
		       stats->refs.remotes, मानम्_delim);
	print_keyvalue("references.others.count", कुञ्चिका_delim,
		       stats->refs.others, मानम्_delim);

	print_keyvalue("objects.commits.count", कुञ्चिका_delim,
		       stats->objects.type_counts.commits, मानम्_delim);
	print_keyvalue("objects.trees.count", कुञ्चिका_delim,
		       stats->objects.type_counts.trees, मानम्_delim);
	print_keyvalue("objects.blobs.count", कुञ्चिका_delim,
		       stats->objects.type_counts.blobs, मानम्_delim);
	print_keyvalue("objects.tags.count", कुञ्चिका_delim,
		       stats->objects.type_counts.चिह्नकानि, मानम्_delim);

	print_keyvalue("objects.commits.inflated_size", कुञ्चिका_delim,
		       stats->objects.inflated_sizes.commits, मानम्_delim);
	print_keyvalue("objects.trees.inflated_size", कुञ्चिका_delim,
		       stats->objects.inflated_sizes.trees, मानम्_delim);
	print_keyvalue("objects.blobs.inflated_size", कुञ्चिका_delim,
		       stats->objects.inflated_sizes.blobs, मानम्_delim);
	print_keyvalue("objects.tags.inflated_size", कुञ्चिका_delim,
		       stats->objects.inflated_sizes.चिह्नकानि, मानम्_delim);

	print_keyvalue("objects.commits.disk_size", कुञ्चिका_delim,
		       stats->objects.disk_sizes.commits, मानम्_delim);
	print_keyvalue("objects.trees.disk_size", कुञ्चिका_delim,
		       stats->objects.disk_sizes.trees, मानम्_delim);
	print_keyvalue("objects.blobs.disk_size", कुञ्चिका_delim,
		       stats->objects.disk_sizes.blobs, मानम्_delim);
	print_keyvalue("objects.tags.disk_size", कुञ्चिका_delim,
		       stats->objects.disk_sizes.चिह्नकानि, मानम्_delim);

	print_object_दत्त("objects.commits.max_size", कुञ्चिका_delim,
			  &stats->objects.largest.commit_आकार, मानम्_delim);
	print_object_दत्त("objects.trees.max_size", कुञ्चिका_delim,
			  &stats->objects.largest.वृक्षः_आकार, मानम्_delim);
	print_object_दत्त("objects.blobs.max_size", कुञ्चिका_delim,
			  &stats->objects.largest.blob_आकार, मानम्_delim);
	print_object_दत्त("objects.tags.max_size", कुञ्चिका_delim,
			  &stats->objects.largest.चिह्नकम्_आकार, मानम्_delim);

	print_object_दत्त("objects.commits.max_parents", कुञ्चिका_delim,
			  &stats->objects.largest.parent_गणनम्, मानम्_delim);
	print_object_दत्त("objects.trees.max_entries", कुञ्चिका_delim,
			  &stats->objects.largest.वृक्षः_entries, मानम्_delim);

	fflush(stdout);
}

संरचना गणनम्_references_दत्त {
	संरचना ref_stats *stats;
	संरचना rev_info *revs;
	संरचना प्रगतिः *प्रगतिः;
};

स्थिर पूर्णाङ्क गणनम्_references(ध्रुव संरचना reference *ref, शून्य *cb_दत्त)
{
	संरचना गणनम्_references_दत्त *दत्त = cb_दत्त;
	संरचना ref_stats *stats = दत्त->stats;
	आकार_t ref_गणनम्;

	विकल्प (ref_विधा_from_refname(ref->name)) {
	पक्ष FILTER_REFS_BRANCHES:
		stats->branches++;
		भङ्ग;
	पक्ष FILTER_REFS_REMOTES:
		stats->remotes++;
		भङ्ग;
	पक्ष FILTER_REFS_चिह्नकानि:
		stats->चिह्नकानि++;
		भङ्ग;
	पक्ष FILTER_REFS_OTHERS:
		stats->others++;
		भङ्ग;
	स्वभाव:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	सङ्कलय_pending_oid(दत्त->revs, रिक्त, ref->oid, 0);

	ref_गणनम् = get_योग_reference_गणनम्(stats);
	display_प्रगतिः(दत्त->प्रगतिः, ref_गणनम्);

	प्रत्यागम 0;
}

स्थिर शून्य structure_गणनम्_references(संरचना ref_stats *stats,
				       संरचना rev_info *revs,
				       संरचना repository *repo,
				       पूर्णाङ्क show_प्रगतिः)
{
	संरचना गणनम्_references_दत्त दत्त = {
		.stats = stats,
		.revs = revs,
	};

	यदि (show_प्रगतिः)
		दत्त.प्रगतिः = start_delayed_प्रगतिः(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), गणनम्_references, &दत्त);
	stop_प्रगतिः(&दत्त.प्रगतिः);
}

संरचना गणनम्_objects_दत्त {
	संरचना object_database *odb;
	संरचना object_stats *stats;
	संरचना प्रगतिः *प्रगतिः;
};

स्थिर शून्य check_largest(संरचना object_दत्त *दत्त, संरचना object_id *oid,
			  आकार_t मानम्)
{
	यदि (मानम् > दत्त->मानम् || is_null_oid(&दत्त->oid)) {
		oidcpy(&दत्त->oid, oid);
		दत्त->मानम् = मानम्;
	}
}

स्थिर आकार_t गणनम्_वृक्षः_entries(संरचना object *obj)
{
	संरचना वृक्षः *t = object_as_type(obj, OBJ_वृक्षः, 0);
	संरचना name_प्रविष्टिः प्रविष्टिः;
	संरचना वृक्षः_desc desc;
	आकार_t गणनम् = 0;

	init_वृक्षः_desc(&desc, &t->object.oid, t->बफर, t->आकार);
	यावत् (वृक्षः_प्रविष्टिः(&desc, &प्रविष्टिः))
		गणनम्++;

	प्रत्यागम गणनम्;
}

स्थिर पूर्णाङ्क गणनम्_objects(ध्रुव वर्ण *मार्गः अप्रयुक्तम्, संरचना oid_array *oids,
			 गणना object_type type, शून्य *cb_दत्त)
{
	संरचना गणनम्_objects_दत्त *दत्त = cb_दत्त;
	संरचना object_stats *stats = दत्त->stats;
	आकार_t object_गणनम्;

	प्रति (आकार_t i = 0; i < oids->nr; i++) {
		संरचना object_info oi = OBJECT_INFO_INIT;
		अचिह्नित दीर्घ inflated;
		आकार_t inflated_st = 0;
		संरचना commit *commit;
		संरचना object *obj;
		शून्य *content;
		off_t disk;
		पूर्णाङ्क eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		यदि (odb_read_object_info_extended(दत्त->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_शीघ्र) < 0)
			अनुवर्त;
		inflated = cast_आकार_t_गन्तव्यम्_ulong(inflated_st);

		obj = parse_object_बफर(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		विकल्प (type) {
		पक्ष OBJ_चिह्नकम्:
			stats->type_counts.चिह्नकानि++;
			stats->inflated_sizes.चिह्नकानि += inflated;
			stats->disk_sizes.चिह्नकानि += disk;
			check_largest(&stats->largest.चिह्नकम्_आकार, &oids->oid[i],
				      inflated);
			भङ्ग;
		पक्ष OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_largest(&stats->largest.commit_आकार, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.parent_गणनम्, &oids->oid[i],
				      commit_list_गणनम्(commit->parents));
			भङ्ग;
		पक्ष OBJ_वृक्षः:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_largest(&stats->largest.वृक्षः_आकार, &oids->oid[i],
				      inflated);
			check_largest(&stats->largest.वृक्षः_entries, &oids->oid[i],
				      गणनम्_वृक्षः_entries(obj));
			भङ्ग;
		पक्ष OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_largest(&stats->largest.blob_आकार, &oids->oid[i],
				      inflated);
			भङ्ग;
		स्वभाव:
			BUG("invalid object type");
		}

		यदि (!eaten)
			मुक्त(content);
	}

	object_गणनम् = get_योग_object_मानानि(&stats->type_counts);
	display_प्रगतिः(दत्त->प्रगतिः, object_गणनम्);

	प्रत्यागम 0;
}

स्थिर शून्य structure_गणनम्_objects(संरचना object_stats *stats,
				    संरचना rev_info *revs,
				    संरचना repository *repo, पूर्णाङ्क show_प्रगतिः)
{
	संरचना मार्गः_walk_info info = मार्गः_WALK_INFO_INIT;
	संरचना गणनम्_objects_दत्त दत्त = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.मार्गः_fn = गणनम्_objects;
	info.मार्गः_fn_दत्त = &दत्त;

	यदि (show_प्रगतिः)
		दत्त.प्रगतिः = start_delayed_प्रगतिः(repo, _("Counting objects"), 0);

	walk_objects_by_मार्गः(&info);
	मार्गः_walk_info_clear(&info);
	stop_प्रगतिः(&दत्त.प्रगतिः);
}

स्थिर पूर्णाङ्क cmd_repo_structure(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			      संरचना repository *repo)
{
	संरचना stats_सारणी सारणी = {
		.पङ्क्तिकाः = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	गणना output_format format = FORMAT_सारणी;
	संरचना repo_structure stats = { 0 };
	संरचना rev_info revs;
	पूर्णाङ्क show_प्रगतिः = -1;
	संरचना option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', रिक्त, &format, रिक्त,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_प्रगतिः, N_("show progress")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options, repo_structure_usage, 0);
	यदि (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, उपसर्गः);

	यदि (show_प्रगतिः < 0)
		show_प्रगतिः = isatty(2);

	structure_गणनम्_references(&stats.refs, &revs, repo, show_प्रगतिः);
	structure_गणनम्_objects(&stats.objects, &revs, repo, show_प्रगतिः);

	विकल्प (format) {
	पक्ष FORMAT_सारणी:
		stats_सारणी_सज्जता_structure(&सारणी, &stats);
		stats_सारणी_print_structure(&सारणी);
		भङ्ग;
	पक्ष FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		भङ्ग;
	पक्ष FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		भङ्ग;
	स्वभाव:
		BUG("invalid output format");
	}

	stats_सारणी_clear(&सारणी);
	release_revisions(&revs);

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_repo(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
	     संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options, repo_usage, 0);

	प्रत्यागम fn(argc, argv, उपसर्गः, repo);
}
