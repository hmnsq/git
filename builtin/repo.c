#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

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

#ವ್ಯಾಖ್ಯಾನಿಸು REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#ವ್ಯಾಖ್ಯಾನಿಸು REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	ಖಾಲಿ,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ repo_info_usage[] = {
	REPO_INFO_USAGE,
	ಖಾಲಿ,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	ಖಾಲಿ,
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ get_ಮೌಲ್ಯ_fn(ರಚನೆ repository *repo, ರಚನೆ strbuf *buf);

ಎಣಿಕೆ output_format {
	FORMAT_ಕೋಷ್ಟಕ,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

ರಚನೆ repo_info_ಕ್ಷೇತ್ರ {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ;
	get_ಮೌಲ್ಯ_fn *get_ಮೌಲ್ಯ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_layout_bare(ರಚನೆ repository *repo ಬಳಸದ, ರಚನೆ strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_layout_shallow(ರಚನೆ repository *repo, ರಚನೆ strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_object_format(ರಚನೆ repository *repo, ರಚನೆ strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ಮಾರ್ಗ_commondir_absolute(ರಚನೆ repository *repo, ರಚನೆ strbuf *buf)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *common_dir = repo_get_common_dir(repo);

	ಆದರೆ (!common_dir)
		ಹಿಂತಿರುಗಿ error(_("unable to get common directory"));

	format_ಮಾರ್ಗ(buf, common_dir, repo->ಪೂರ್ವಪ್ರತ್ಯಯ, ಮಾರ್ಗ_FORMAT_CANONICAL);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ಮಾರ್ಗ_commondir_relative(ರಚನೆ repository *repo, ರಚನೆ strbuf *buf)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *common_dir = repo_get_common_dir(repo);

	ಆದರೆ (!common_dir)
		ಹಿಂತಿರುಗಿ error(_("unable to get common directory"));

	format_ಮಾರ್ಗ(buf, common_dir, repo->ಪೂರ್ವಪ್ರತ್ಯಯ, ಮಾರ್ಗ_FORMAT_RELATIVE);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ಮಾರ್ಗ_gitdir_absolute(ರಚನೆ repository *repo, ರಚನೆ strbuf *buf)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *git_dir = repo_get_git_dir(repo);

	ಆದರೆ (!git_dir)
		ಹಿಂತಿರುಗಿ error(_("unable to get git directory"));

	format_ಮಾರ್ಗ(buf, git_dir, repo->ಪೂರ್ವಪ್ರತ್ಯಯ, ಮಾರ್ಗ_FORMAT_CANONICAL);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ಮಾರ್ಗ_gitdir_relative(ರಚನೆ repository *repo, ರಚನೆ strbuf *buf)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *git_dir = repo_get_git_dir(repo);

	ಆದರೆ (!git_dir)
		ಹಿಂತಿರುಗಿ error(_("unable to get git directory"));

	format_ಮಾರ್ಗ(buf, git_dir, repo->ಪೂರ್ವಪ್ರತ್ಯಯ, ಮಾರ್ಗ_FORMAT_RELATIVE);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_references_format(ರಚನೆ repository *repo, ರಚನೆ strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_ಗಮ್ಯ_name(repo->ref_storage_format));
	ಹಿಂತಿರುಗಿ 0;
}

/* repo_info_field keys must be in lexicographical order */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ repo_info_ಕ್ಷೇತ್ರ repo_info_ಕ್ಷೇತ್ರ[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_ಮಾರ್ಗ_commondir_absolute },
	{ "path.commondir.relative", get_ಮಾರ್ಗ_commondir_relative },
	{ "path.gitdir.absolute", get_ಮಾರ್ಗ_gitdir_absolute },
	{ "path.gitdir.relative", get_ಮಾರ್ಗ_gitdir_relative },
	{ "references.format", get_references_format },
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ repo_info_ಕ್ಷೇತ್ರ_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *va, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *vb)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ repo_info_ಕ್ಷೇತ್ರ *a = va;
	ಸ್ಥಿರಾಂಕ ರಚನೆ repo_info_ಕ್ಷೇತ್ರ *b = vb;

	ಹಿಂತಿರುಗಿ strcmp(a->ಕೀಲಿ, b->ಕೀಲಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ repo_info_ಕ್ಷೇತ್ರ *get_repo_info_ಕ್ಷೇತ್ರ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ repo_info_ಕ್ಷೇತ್ರ ಹುಡುಕು_ಕೀಲಿ = { ಕೀಲಿ, ಖಾಲಿ };
	ಸ್ಥಿರಾಂಕ ರಚನೆ repo_info_ಕ್ಷೇತ್ರ *ಸಿಕ್ಕಿತು_2 = bsearch(&ಹುಡುಕು_ಕೀಲಿ,
						      repo_info_ಕ್ಷೇತ್ರ,
						      ARRAY_ಗಾತ್ರ(repo_info_ಕ್ಷೇತ್ರ),
						      ಗಾತ್ರ(*ಸಿಕ್ಕಿತು_2),
						      repo_info_ಕ್ಷೇತ್ರ_cmp);

	ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು_2;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_ಕ್ಷೇತ್ರ(ಎಣಿಕೆ output_format format, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
	ಆಯ್ಕೆ (format) {
	ಸ್ಥಿತಿ FORMAT_NEWLINE_TERMINATED:
		ಬರೆ("%s=", ಕೀಲಿ);
		quote_c_style(ಮೌಲ್ಯ, ಖಾಲಿ, stdout, 0);
		ಅಕ್ಷರ_ಬರೆ('\n');
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ FORMAT_NUL_TERMINATED:
		ಬರೆ("%s\n%s%c", ಕೀಲಿ, ಮೌಲ್ಯ, '\0');
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("not a valid output format: %d", format);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ print_fields(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			ರಚನೆ repository *repo,
			ಎಣಿಕೆ output_format format)
{
	ಪೂರ್ಣಾಂಕ ret = 0;
	ರಚನೆ strbuf valbuf = STRBUF_INIT;

	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ = argv[i];
		ಸ್ಥಿರಾಂಕ ರಚನೆ repo_info_ಕ್ಷೇತ್ರ *ಕ್ಷೇತ್ರ = get_repo_info_ಕ್ಷೇತ್ರ(ಕೀಲಿ);

		ಆದರೆ (!ಕ್ಷೇತ್ರ) {
			ret = error(_("key '%s' not found"), ಕೀಲಿ);
			ಮುಂದುವರಿ;
		}

		strbuf_reset(&valbuf);
		ಕ್ಷೇತ್ರ->get_ಮೌಲ್ಯ(repo, &valbuf);
		print_ಕ್ಷೇತ್ರ(format, ಕೀಲಿ, valbuf.buf);
	}

	strbuf_release(&valbuf);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ print_all_fields(ರಚನೆ repository *repo,
			    ಎಣಿಕೆ output_format format)
{
	ರಚನೆ strbuf valbuf = STRBUF_INIT;

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ARRAY_ಗಾತ್ರ(repo_info_ಕ್ಷೇತ್ರ); i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ repo_info_ಕ್ಷೇತ್ರ *ಕ್ಷೇತ್ರ = &repo_info_ಕ್ಷೇತ್ರ[i];

		strbuf_reset(&valbuf);
		ಕ್ಷೇತ್ರ->get_ಮೌಲ್ಯ(repo, &valbuf);
		print_ಕ್ಷೇತ್ರ(format, ಕ್ಷೇತ್ರ->ಕೀಲಿ, valbuf.buf);
	}

	strbuf_release(&valbuf);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ print_ಕೀಲಿಗಳು(ಎಣಿಕೆ output_format format)
{
	ಅಕ್ಷರ sep;

	ಆಯ್ಕೆ (format) {
	ಸ್ಥಿತಿ FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ FORMAT_NUL_TERMINATED:
		sep = '\0';
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ARRAY_ಗಾತ್ರ(repo_info_ಕ್ಷೇತ್ರ); i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ repo_info_ಕ್ಷೇತ್ರ *ಕ್ಷೇತ್ರ = &repo_info_ಕ್ಷೇತ್ರ[i];
		ಬರೆ("%s%c", ಕ್ಷೇತ್ರ->ಕೀಲಿ, sep);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_format_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset ಬಳಸದ)
{
	ಎಣಿಕೆ output_format *format = opt->ಮೌಲ್ಯ;

	ಆದರೆ (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "table"))
		*format = FORMAT_ಕೋಷ್ಟಕ;
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("invalid format '%s'"), arg);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_repo_info(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			 ರಚನೆ repository *repo)
{
	ಎಣಿಕೆ output_format format = FORMAT_NEWLINE_TERMINATED;
	ಪೂರ್ಣಾಂಕ all_ಕೀಲಿಗಳು = 0;
	ಪೂರ್ಣಾಂಕ show_ಕೀಲಿಗಳು = 0;
	ರಚನೆ option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ಖಾಲಿ, &format, ಖಾಲಿ,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_ಕೀಲಿಗಳು, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_ಕೀಲಿಗಳು, N_("show keys")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, repo_info_usage, 0);

	ಆದರೆ (show_ಕೀಲಿಗಳು && (all_ಕೀಲಿಗಳು || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	ಆದರೆ (show_ಕೀಲಿಗಳು)
		ಹಿಂತಿರುಗಿ print_ಕೀಲಿಗಳು(format);

	ಆದರೆ (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	ಆದರೆ (all_ಕೀಲಿಗಳು && argc)
		die(_("--all and <key> cannot be used together"));

	ಆದರೆ (all_ಕೀಲಿಗಳು)
		ಹಿಂತಿರುಗಿ print_all_fields(repo, format);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ print_fields(argc, argv, repo, format);
}

ರಚನೆ object_ದತ್ತಾಂಶ {
	ರಚನೆ object_id oid;
	ಗಾತ್ರ_t ಮೌಲ್ಯ;
};

ರಚನೆ ಅತಿದೊಡ್ಡ_objects {
	ರಚನೆ object_ದತ್ತಾಂಶ ಟ್ಯಾಗ್_ಗಾತ್ರ;
	ರಚನೆ object_ದತ್ತಾಂಶ commit_ಗಾತ್ರ;
	ರಚನೆ object_ದತ್ತಾಂಶ ವೃಕ್ಷ_ಗಾತ್ರ;
	ರಚನೆ object_ದತ್ತಾಂಶ blob_ಗಾತ್ರ;

	ರಚನೆ object_ದತ್ತಾಂಶ ಪೋಷಕ_ಎಣಿಕೆ;
	ರಚನೆ object_ದತ್ತಾಂಶ ವೃಕ್ಷ_entries;
};

ರಚನೆ ref_stats {
	ಗಾತ್ರ_t branches;
	ಗಾತ್ರ_t remotes;
	ಗಾತ್ರ_t ಟ್ಯಾಗುಗಳು;
	ಗಾತ್ರ_t others;
};

ರಚನೆ object_ಮೌಲ್ಯಗಳು {
	ಗಾತ್ರ_t ಟ್ಯಾಗುಗಳು;
	ಗಾತ್ರ_t commits;
	ಗಾತ್ರ_t trees;
	ಗಾತ್ರ_t blobs;
};

ರಚನೆ object_stats {
	ರಚನೆ object_ಮೌಲ್ಯಗಳು type_counts;
	ರಚನೆ object_ಮೌಲ್ಯಗಳು inflated_sizes;
	ರಚನೆ object_ಮೌಲ್ಯಗಳು disk_sizes;
	ರಚನೆ ಅತಿದೊಡ್ಡ_objects ಅತಿದೊಡ್ಡ;
};

ರಚನೆ repo_structure {
	ರಚನೆ ref_stats refs;
	ರಚನೆ object_stats objects;
};

ರಚನೆ stats_ಕೋಷ್ಟಕ {
	ರಚನೆ string_list ಅಡ್ಡಸಾಲುಗಳು;
	ರಚನೆ string_list annotations;

	ಪೂರ್ಣಾಂಕ name_ಲಂಬಸಾಲು_ಅಗಲ;
	ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ_ಲಂಬಸಾಲು_ಅಗಲ;
	ಪೂರ್ಣಾಂಕ unit_ಲಂಬಸಾಲು_ಅಗಲ;
};

/*
 * Holds column data that gets stored for each row.
 */
ರಚನೆ stats_ಕೋಷ್ಟಕ_ನಮೂದು {
	ಅಕ್ಷರ *ಮೌಲ್ಯ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *unit;
	ಗಾತ್ರ_t ಸೂಚ್ಯಂಕ;
	ರಚನೆ object_id *oid;
};

ಸ್ಥಿರ ಶೂನ್ಯ stats_ಕೋಷ್ಟಕ_vaddf(ರಚನೆ stats_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
			      ರಚನೆ stats_ಕೋಷ್ಟಕ_ನಮೂದು *ನಮೂದು,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, va_list ap)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಅಕ್ಷರ *formatted_name;
	ಪೂರ್ಣಾಂಕ name_ಅಗಲ;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, ಖಾಲಿ);
	name_ಅಗಲ = utf8_strwidth(formatted_name);

	ಅಂಶ = string_list_append_nodup(&ಕೋಷ್ಟಕ->ಅಡ್ಡಸಾಲುಗಳು, formatted_name);
	ಅಂಶ->util = ನಮೂದು;

	ಆದರೆ (name_ಅಗಲ > ಕೋಷ್ಟಕ->name_ಲಂಬಸಾಲು_ಅಗಲ)
		ಕೋಷ್ಟಕ->name_ಲಂಬಸಾಲು_ಅಗಲ = name_ಅಗಲ;
	ಆದರೆ (!ನಮೂದು)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (ನಮೂದು->oid) {
		ನಮೂದು->ಸೂಚ್ಯಂಕ = ಕೋಷ್ಟಕ->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)ನಮೂದು->ಸೂಚ್ಯಂಕ,
			    oid_ಗಮ್ಯ_hex(ನಮೂದು->oid));
		string_list_append_nodup(&ಕೋಷ್ಟಕ->annotations, strbuf_detach(&buf, ಖಾಲಿ));
	}
	ಆದರೆ (ನಮೂದು->ಮೌಲ್ಯ) {
		ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ_ಅಗಲ = utf8_strwidth(ನಮೂದು->ಮೌಲ್ಯ);
		ಆದರೆ (ಮೌಲ್ಯ_ಅಗಲ > ಕೋಷ್ಟಕ->ಮೌಲ್ಯ_ಲಂಬಸಾಲು_ಅಗಲ)
			ಕೋಷ್ಟಕ->ಮೌಲ್ಯ_ಲಂಬಸಾಲು_ಅಗಲ = ಮೌಲ್ಯ_ಅಗಲ;
	}
	ಆದರೆ (ನಮೂದು->unit) {
		ಪೂರ್ಣಾಂಕ unit_ಅಗಲ = utf8_strwidth(ನಮೂದು->unit);
		ಆದರೆ (unit_ಅಗಲ > ಕೋಷ್ಟಕ->unit_ಲಂಬಸಾಲು_ಅಗಲ)
			ಕೋಷ್ಟಕ->unit_ಲಂಬಸಾಲು_ಅಗಲ = unit_ಅಗಲ;
	}

	strbuf_release(&buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_ಕೋಷ್ಟಕ_addf(ರಚನೆ stats_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, ...)
{
	va_list ap;

	va_ಆರಂಭ(ap, format);
	stats_ಕೋಷ್ಟಕ_vaddf(ಕೋಷ್ಟಕ, ಖಾಲಿ, format, ap);
	va_ಕೊನೆ(ap);
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_ಕೋಷ್ಟಕ_ಎಣಿಕೆ_addf(ರಚನೆ stats_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಗಾತ್ರ_t ಮೌಲ್ಯ,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, ...)
{
	ರಚನೆ stats_ಕೋಷ್ಟಕ_ನಮೂದು *ನಮೂದು;
	va_list ap;

	CALLOC_ARRAY(ನಮೂದು, 1);
	humanise_ಎಣಿಕೆ(ಮೌಲ್ಯ, &ನಮೂದು->ಮೌಲ್ಯ, &ನಮೂದು->unit);

	va_ಆರಂಭ(ap, format);
	stats_ಕೋಷ್ಟಕ_vaddf(ಕೋಷ್ಟಕ, ನಮೂದು, format, ap);
	va_ಕೊನೆ(ap);
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_ಕೋಷ್ಟಕ_object_ಎಣಿಕೆ_addf(ರಚನೆ stats_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
					  ರಚನೆ object_id *oid, ಗಾತ್ರ_t ಮೌಲ್ಯ,
					  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, ...)
{
	ರಚನೆ stats_ಕೋಷ್ಟಕ_ನಮೂದು *ನಮೂದು;
	va_list ap;

	CALLOC_ARRAY(ನಮೂದು, 1);
	humanise_ಎಣಿಕೆ(ಮೌಲ್ಯ, &ನಮೂದು->ಮೌಲ್ಯ, &ನಮೂದು->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	ಆದರೆ (!is_null_oid(oid))
		ನಮೂದು->oid = oid;

	va_ಆರಂಭ(ap, format);
	stats_ಕೋಷ್ಟಕ_vaddf(ಕೋಷ್ಟಕ, ನಮೂದು, format, ap);
	va_ಕೊನೆ(ap);
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_ಕೋಷ್ಟಕ_ಗಾತ್ರ_addf(ರಚನೆ stats_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಗಾತ್ರ_t ಮೌಲ್ಯ,
				  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, ...)
{
	ರಚನೆ stats_ಕೋಷ್ಟಕ_ನಮೂದು *ನಮೂದು;
	va_list ap;

	CALLOC_ARRAY(ನಮೂದು, 1);
	humanise_ಬೈಟುಗಳು(ಮೌಲ್ಯ, &ನಮೂದು->ಮೌಲ್ಯ, &ನಮೂದು->unit, HUMANISE_COMPACT);

	va_ಆರಂಭ(ap, format);
	stats_ಕೋಷ್ಟಕ_vaddf(ಕೋಷ್ಟಕ, ನಮೂದು, format, ap);
	va_ಕೊನೆ(ap);
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_ಕೋಷ್ಟಕ_object_ಗಾತ್ರ_addf(ರಚನೆ stats_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
					 ರಚನೆ object_id *oid, ಗಾತ್ರ_t ಮೌಲ್ಯ,
					 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, ...)
{
	ರಚನೆ stats_ಕೋಷ್ಟಕ_ನಮೂದು *ನಮೂದು;
	va_list ap;

	CALLOC_ARRAY(ನಮೂದು, 1);
	humanise_ಬೈಟುಗಳು(ಮೌಲ್ಯ, &ನಮೂದು->ಮೌಲ್ಯ, &ನಮೂದು->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	ಆದರೆ (!is_null_oid(oid))
		ನಮೂದು->oid = oid;

	va_ಆರಂಭ(ap, format);
	stats_ಕೋಷ್ಟಕ_vaddf(ಕೋಷ್ಟಕ, ನಮೂದು, format, ap);
	va_ಕೊನೆ(ap);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಗಾತ್ರ_t get_ಒಟ್ಟು_reference_ಎಣಿಕೆ(ರಚನೆ ref_stats *stats)
{
	ಹಿಂತಿರುಗಿ stats->branches + stats->remotes + stats->ಟ್ಯಾಗುಗಳು + stats->others;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಗಾತ್ರ_t get_ಒಟ್ಟು_object_ಮೌಲ್ಯಗಳು(ರಚನೆ object_ಮೌಲ್ಯಗಳು *ಮೌಲ್ಯಗಳು)
{
	ಹಿಂತಿರುಗಿ ಮೌಲ್ಯಗಳು->ಟ್ಯಾಗುಗಳು + ಮೌಲ್ಯಗಳು->commits + ಮೌಲ್ಯಗಳು->trees + ಮೌಲ್ಯಗಳು->blobs;
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_ಕೋಷ್ಟಕ_ಸಿದ್ಧತೆ_structure(ರಚನೆ stats_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
					ರಚನೆ repo_structure *stats)
{
	ರಚನೆ object_stats *objects = &stats->objects;
	ರಚನೆ ref_stats *refs = &stats->refs;
	ಗಾತ್ರ_t inflated_object_ಒಟ್ಟು;
	ಗಾತ್ರ_t object_ಎಣಿಕೆ_ಒಟ್ಟು;
	ಗಾತ್ರ_t disk_object_ಒಟ್ಟು;
	ಗಾತ್ರ_t ref_ಒಟ್ಟು;

	ref_ಒಟ್ಟು = get_ಒಟ್ಟು_reference_ಎಣಿಕೆ(refs);
	stats_ಕೋಷ್ಟಕ_addf(ಕೋಷ್ಟಕ, "* %s", _("References"));
	stats_ಕೋಷ್ಟಕ_ಎಣಿಕೆ_addf(ಕೋಷ್ಟಕ, ref_ಒಟ್ಟು, "  * %s", _("Count"));
	stats_ಕೋಷ್ಟಕ_ಎಣಿಕೆ_addf(ಕೋಷ್ಟಕ, refs->branches, "    * %s", _("Branches"));
	stats_ಕೋಷ್ಟಕ_ಎಣಿಕೆ_addf(ಕೋಷ್ಟಕ, refs->ಟ್ಯಾಗುಗಳು, "    * %s", _("Tags"));
	stats_ಕೋಷ್ಟಕ_ಎಣಿಕೆ_addf(ಕೋಷ್ಟಕ, refs->remotes, "    * %s", _("Remotes"));
	stats_ಕೋಷ್ಟಕ_ಎಣಿಕೆ_addf(ಕೋಷ್ಟಕ, refs->others, "    * %s", _("Others"));

	object_ಎಣಿಕೆ_ಒಟ್ಟು = get_ಒಟ್ಟು_object_ಮೌಲ್ಯಗಳು(&objects->type_counts);
	stats_ಕೋಷ್ಟಕ_addf(ಕೋಷ್ಟಕ, "");
	stats_ಕೋಷ್ಟಕ_addf(ಕೋಷ್ಟಕ, "* %s", _("Reachable objects"));
	stats_ಕೋಷ್ಟಕ_ಎಣಿಕೆ_addf(ಕೋಷ್ಟಕ, object_ಎಣಿಕೆ_ಒಟ್ಟು, "  * %s", _("Count"));
	stats_ಕೋಷ್ಟಕ_ಎಣಿಕೆ_addf(ಕೋಷ್ಟಕ, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_ಕೋಷ್ಟಕ_ಎಣಿಕೆ_addf(ಕೋಷ್ಟಕ, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_ಕೋಷ್ಟಕ_ಎಣಿಕೆ_addf(ಕೋಷ್ಟಕ, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_ಕೋಷ್ಟಕ_ಎಣಿಕೆ_addf(ಕೋಷ್ಟಕ, objects->type_counts.ಟ್ಯಾಗುಗಳು,
			       "    * %s", _("Tags"));

	inflated_object_ಒಟ್ಟು = get_ಒಟ್ಟು_object_ಮೌಲ್ಯಗಳು(&objects->inflated_sizes);
	stats_ಕೋಷ್ಟಕ_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ, inflated_object_ಒಟ್ಟು,
			      "  * %s", _("Inflated size"));
	stats_ಕೋಷ್ಟಕ_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ, objects->inflated_sizes.commits,
			      "    * %s", _("Commits"));
	stats_ಕೋಷ್ಟಕ_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ, objects->inflated_sizes.trees,
			      "    * %s", _("Trees"));
	stats_ಕೋಷ್ಟಕ_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ, objects->inflated_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_ಕೋಷ್ಟಕ_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ, objects->inflated_sizes.ಟ್ಯಾಗುಗಳು,
			      "    * %s", _("Tags"));

	disk_object_ಒಟ್ಟು = get_ಒಟ್ಟು_object_ಮೌಲ್ಯಗಳು(&objects->disk_sizes);
	stats_ಕೋಷ್ಟಕ_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ, disk_object_ಒಟ್ಟು,
			      "  * %s", _("Disk size"));
	stats_ಕೋಷ್ಟಕ_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ, objects->disk_sizes.commits,
			      "    * %s", _("Commits"));
	stats_ಕೋಷ್ಟಕ_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ, objects->disk_sizes.trees,
			      "    * %s", _("Trees"));
	stats_ಕೋಷ್ಟಕ_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ, objects->disk_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_ಕೋಷ್ಟಕ_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ, objects->disk_sizes.ಟ್ಯಾಗುಗಳು,
			      "    * %s", _("Tags"));

	stats_ಕೋಷ್ಟಕ_addf(ಕೋಷ್ಟಕ, "");
	stats_ಕೋಷ್ಟಕ_addf(ಕೋಷ್ಟಕ, "* %s", _("Largest objects"));
	stats_ಕೋಷ್ಟಕ_addf(ಕೋಷ್ಟಕ, "  * %s", _("Commits"));
	stats_ಕೋಷ್ಟಕ_object_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ,
				     &objects->ಅತಿದೊಡ್ಡ.commit_ಗಾತ್ರ.oid,
				     objects->ಅತಿದೊಡ್ಡ.commit_ಗಾತ್ರ.ಮೌಲ್ಯ,
				     "    * %s", _("Maximum size"));
	stats_ಕೋಷ್ಟಕ_object_ಎಣಿಕೆ_addf(ಕೋಷ್ಟಕ,
				      &objects->ಅತಿದೊಡ್ಡ.ಪೋಷಕ_ಎಣಿಕೆ.oid,
				      objects->ಅತಿದೊಡ್ಡ.ಪೋಷಕ_ಎಣಿಕೆ.ಮೌಲ್ಯ,
				      "    * %s", _("Maximum parents"));
	stats_ಕೋಷ್ಟಕ_addf(ಕೋಷ್ಟಕ, "  * %s", _("Trees"));
	stats_ಕೋಷ್ಟಕ_object_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ,
				     &objects->ಅತಿದೊಡ್ಡ.ವೃಕ್ಷ_ಗಾತ್ರ.oid,
				     objects->ಅತಿದೊಡ್ಡ.ವೃಕ್ಷ_ಗಾತ್ರ.ಮೌಲ್ಯ,
				     "    * %s", _("Maximum size"));
	stats_ಕೋಷ್ಟಕ_object_ಎಣಿಕೆ_addf(ಕೋಷ್ಟಕ,
				      &objects->ಅತಿದೊಡ್ಡ.ವೃಕ್ಷ_entries.oid,
				      objects->ಅತಿದೊಡ್ಡ.ವೃಕ್ಷ_entries.ಮೌಲ್ಯ,
				      "    * %s", _("Maximum entries"));
	stats_ಕೋಷ್ಟಕ_addf(ಕೋಷ್ಟಕ, "  * %s", _("Blobs"));
	stats_ಕೋಷ್ಟಕ_object_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ,
				     &objects->ಅತಿದೊಡ್ಡ.blob_ಗಾತ್ರ.oid,
				     objects->ಅತಿದೊಡ್ಡ.blob_ಗಾತ್ರ.ಮೌಲ್ಯ,
				     "    * %s", _("Maximum size"));
	stats_ಕೋಷ್ಟಕ_addf(ಕೋಷ್ಟಕ, "  * %s", _("Tags"));
	stats_ಕೋಷ್ಟಕ_object_ಗಾತ್ರ_addf(ಕೋಷ್ಟಕ,
				     &objects->ಅತಿದೊಡ್ಡ.ಟ್ಯಾಗ್_ಗಾತ್ರ.oid,
				     objects->ಅತಿದೊಡ್ಡ.ಟ್ಯಾಗ್_ಗಾತ್ರ.ಮೌಲ್ಯ,
				     "    * %s", _("Maximum size"));
}

#ವ್ಯಾಖ್ಯಾನಿಸು ಸೂಚ್ಯಂಕ_ಅಗಲ 4

ಸ್ಥಿರ ಶೂನ್ಯ stats_ಕೋಷ್ಟಕ_print_structure(ಸ್ಥಿರಾಂಕ ರಚನೆ stats_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name_ಲಂಬಸಾಲು_title = _("Repository structure");
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ_ಲಂಬಸಾಲು_title = _("Value");
	ಪೂರ್ಣಾಂಕ title_name_ಅಗಲ = utf8_strwidth(name_ಲಂಬಸಾಲು_title);
	ಪೂರ್ಣಾಂಕ title_ಮೌಲ್ಯ_ಅಗಲ = utf8_strwidth(ಮೌಲ್ಯ_ಲಂಬಸಾಲು_title);
	ಪೂರ್ಣಾಂಕ name_ಲಂಬಸಾಲು_ಅಗಲ = ಕೋಷ್ಟಕ->name_ಲಂಬಸಾಲು_ಅಗಲ;
	ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ_ಲಂಬಸಾಲು_ಅಗಲ = ಕೋಷ್ಟಕ->ಮೌಲ್ಯ_ಲಂಬಸಾಲು_ಅಗಲ;
	ಪೂರ್ಣಾಂಕ unit_ಲಂಬಸಾಲು_ಅಗಲ = ಕೋಷ್ಟಕ->unit_ಲಂಬಸಾಲು_ಅಗಲ;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ರಚನೆ strbuf buf = STRBUF_INIT;

	ಆದರೆ (title_name_ಅಗಲ > name_ಲಂಬಸಾಲು_ಅಗಲ)
		name_ಲಂಬಸಾಲು_ಅಗಲ = title_name_ಅಗಲ;
	ಆದರೆ (title_ಮೌಲ್ಯ_ಅಗಲ > ಮೌಲ್ಯ_ಲಂಬಸಾಲು_ಅಗಲ + unit_ಲಂಬಸಾಲು_ಅಗಲ + 1)
		ಮೌಲ್ಯ_ಲಂಬಸಾಲು_ಅಗಲ = title_ಮೌಲ್ಯ_ಅಗಲ - unit_ಲಂಬಸಾಲು_ಅಗಲ;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_ಎಡ, name_ಲಂಬಸಾಲು_ಅಗಲ + ಸೂಚ್ಯಂಕ_ಅಗಲ,
			  name_ಲಂಬಸಾಲು_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_ಎಡ,
			  ಮೌಲ್ಯ_ಲಂಬಸಾಲು_ಅಗಲ + unit_ಲಂಬಸಾಲು_ಅಗಲ + 1, ಮೌಲ್ಯ_ಲಂಬಸಾಲು_title);
	strbuf_addstr(&buf, " |");
	ಬರೆ("%s\n", buf.buf);

	ಬರೆ("| ");
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < name_ಲಂಬಸಾಲು_ಅಗಲ + ಸೂಚ್ಯಂಕ_ಅಗಲ; i++)
		ಅಕ್ಷರ_ಬರೆ('-');
	ಬರೆ(" | ");
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಮೌಲ್ಯ_ಲಂಬಸಾಲು_ಅಗಲ + unit_ಲಂಬಸಾಲು_ಅಗಲ + 1; i++)
		ಅಕ್ಷರ_ಬರೆ('-');
	ಬರೆ(" |\n");

	for_each_string_list_ಅಂಶ(ಅಂಶ, &ಕೋಷ್ಟಕ->ಅಡ್ಡಸಾಲುಗಳು) {
		ರಚನೆ stats_ಕೋಷ್ಟಕ_ನಮೂದು *ನಮೂದು = ಅಂಶ->util;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ = "";
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *unit = "";

		ಆದರೆ (ನಮೂದು) {
			ಮೌಲ್ಯ = ನಮೂದು->ಮೌಲ್ಯ;
			ಆದರೆ (ನಮೂದು->unit)
				unit = ನಮೂದು->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_ಎಡ, name_ಲಂಬಸಾಲು_ಅಗಲ, ಅಂಶ->string);

		ಆದರೆ (ನಮೂದು && ನಮೂದು->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)ನಮೂದು->ಸೂಚ್ಯಂಕ);
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_addchars(&buf, ' ', ಸೂಚ್ಯಂಕ_ಅಗಲ);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_ಬಲ, ಮೌಲ್ಯ_ಲಂಬಸಾಲು_ಅಗಲ, ಮೌಲ್ಯ);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_ಎಡ, unit_ಲಂಬಸಾಲು_ಅಗಲ, unit);
		strbuf_addstr(&buf, " |");
		ಬರೆ("%s\n", buf.buf);
	}

	ಆದರೆ (ಕೋಷ್ಟಕ->annotations.nr) {
		ಬರೆ("\n");
		for_each_string_list_ಅಂಶ(ಅಂಶ, &ಕೋಷ್ಟಕ->annotations)
			ಬರೆ("%s\n", ಅಂಶ->string);
	}

	strbuf_release(&buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ stats_ಕೋಷ್ಟಕ_clear(ರಚನೆ stats_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ)
{
	ರಚನೆ stats_ಕೋಷ್ಟಕ_ನಮೂದು *ನಮೂದು;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

	for_each_string_list_ಅಂಶ(ಅಂಶ, &ಕೋಷ್ಟಕ->ಅಡ್ಡಸಾಲುಗಳು) {
		ನಮೂದು = ಅಂಶ->util;
		ಆದರೆ (ನಮೂದು)
			ಬಿಡುಗಡೆ(ನಮೂದು->ಮೌಲ್ಯ);
	}

	string_list_clear(&ಕೋಷ್ಟಕ->ಅಡ್ಡಸಾಲುಗಳು, 1);
	string_list_clear(&ಕೋಷ್ಟಕ->annotations, 1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ print_keyvalue(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಅಕ್ಷರ ಕೀಲಿ_delim, ಗಾತ್ರ_t ಮೌಲ್ಯ,
				  ಅಕ್ಷರ ಮೌಲ್ಯ_delim)
{
	ಬರೆ("%s%c%" PRIuMAX "%c", ಕೀಲಿ, ಕೀಲಿ_delim, (uintmax_t)ಮೌಲ್ಯ,
	       ಮೌಲ್ಯ_delim);
}

ಸ್ಥಿರ ಶೂನ್ಯ print_object_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಅಕ್ಷರ ಕೀಲಿ_delim,
			      ರಚನೆ object_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ, ಅಕ್ಷರ ಮೌಲ್ಯ_delim)
{
	print_keyvalue(ಕೀಲಿ, ಕೀಲಿ_delim, ದತ್ತಾಂಶ->ಮೌಲ್ಯ, ಮೌಲ್ಯ_delim);
	ಬರೆ("%s_oid%c%s%c", ಕೀಲಿ, ಕೀಲಿ_delim, oid_ಗಮ್ಯ_hex(&ದತ್ತಾಂಶ->oid),
	       ಮೌಲ್ಯ_delim);
}

ಸ್ಥಿರ ಶೂನ್ಯ structure_keyvalue_print(ರಚನೆ repo_structure *stats,
				     ಅಕ್ಷರ ಕೀಲಿ_delim, ಅಕ್ಷರ ಮೌಲ್ಯ_delim)
{
	print_keyvalue("references.branches.count", ಕೀಲಿ_delim,
		       stats->refs.branches, ಮೌಲ್ಯ_delim);
	print_keyvalue("references.tags.count", ಕೀಲಿ_delim,
		       stats->refs.ಟ್ಯಾಗುಗಳು, ಮೌಲ್ಯ_delim);
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
		       stats->objects.type_counts.ಟ್ಯಾಗುಗಳು, ಮೌಲ್ಯ_delim);

	print_keyvalue("objects.commits.inflated_size", ಕೀಲಿ_delim,
		       stats->objects.inflated_sizes.commits, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.trees.inflated_size", ಕೀಲಿ_delim,
		       stats->objects.inflated_sizes.trees, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.blobs.inflated_size", ಕೀಲಿ_delim,
		       stats->objects.inflated_sizes.blobs, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.tags.inflated_size", ಕೀಲಿ_delim,
		       stats->objects.inflated_sizes.ಟ್ಯಾಗುಗಳು, ಮೌಲ್ಯ_delim);

	print_keyvalue("objects.commits.disk_size", ಕೀಲಿ_delim,
		       stats->objects.disk_sizes.commits, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.trees.disk_size", ಕೀಲಿ_delim,
		       stats->objects.disk_sizes.trees, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.blobs.disk_size", ಕೀಲಿ_delim,
		       stats->objects.disk_sizes.blobs, ಮೌಲ್ಯ_delim);
	print_keyvalue("objects.tags.disk_size", ಕೀಲಿ_delim,
		       stats->objects.disk_sizes.ಟ್ಯಾಗುಗಳು, ಮೌಲ್ಯ_delim);

	print_object_ದತ್ತಾಂಶ("objects.commits.max_size", ಕೀಲಿ_delim,
			  &stats->objects.ಅತಿದೊಡ್ಡ.commit_ಗಾತ್ರ, ಮೌಲ್ಯ_delim);
	print_object_ದತ್ತಾಂಶ("objects.trees.max_size", ಕೀಲಿ_delim,
			  &stats->objects.ಅತಿದೊಡ್ಡ.ವೃಕ್ಷ_ಗಾತ್ರ, ಮೌಲ್ಯ_delim);
	print_object_ದತ್ತಾಂಶ("objects.blobs.max_size", ಕೀಲಿ_delim,
			  &stats->objects.ಅತಿದೊಡ್ಡ.blob_ಗಾತ್ರ, ಮೌಲ್ಯ_delim);
	print_object_ದತ್ತಾಂಶ("objects.tags.max_size", ಕೀಲಿ_delim,
			  &stats->objects.ಅತಿದೊಡ್ಡ.ಟ್ಯಾಗ್_ಗಾತ್ರ, ಮೌಲ್ಯ_delim);

	print_object_ದತ್ತಾಂಶ("objects.commits.max_parents", ಕೀಲಿ_delim,
			  &stats->objects.ಅತಿದೊಡ್ಡ.ಪೋಷಕ_ಎಣಿಕೆ, ಮೌಲ್ಯ_delim);
	print_object_ದತ್ತಾಂಶ("objects.trees.max_entries", ಕೀಲಿ_delim,
			  &stats->objects.ಅತಿದೊಡ್ಡ.ವೃಕ್ಷ_entries, ಮೌಲ್ಯ_delim);

	fflush(stdout);
}

ರಚನೆ ಎಣಿಕೆ_references_ದತ್ತಾಂಶ {
	ರಚನೆ ref_stats *stats;
	ರಚನೆ rev_info *revs;
	ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_references(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ ಎಣಿಕೆ_references_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb_ದತ್ತಾಂಶ;
	ರಚನೆ ref_stats *stats = ದತ್ತಾಂಶ->stats;
	ಗಾತ್ರ_t ref_ಎಣಿಕೆ;

	ಆಯ್ಕೆ (ref_ಬಗೆ_from_refname(ref->name)) {
	ಸ್ಥಿತಿ FILTER_REFS_BRANCHES:
		stats->branches++;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ FILTER_REFS_REMOTES:
		stats->remotes++;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ FILTER_REFS_ಟ್ಯಾಗುಗಳು:
		stats->ಟ್ಯಾಗುಗಳು++;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ FILTER_REFS_OTHERS:
		stats->others++;
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	ಕೂಡಿಸು_pending_oid(ದತ್ತಾಂಶ->revs, ಖಾಲಿ, ref->oid, 0);

	ref_ಎಣಿಕೆ = get_ಒಟ್ಟು_reference_ಎಣಿಕೆ(stats);
	display_ಪ್ರಗತಿ(ದತ್ತಾಂಶ->ಪ್ರಗತಿ, ref_ಎಣಿಕೆ);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ structure_ಎಣಿಕೆ_references(ರಚನೆ ref_stats *stats,
				       ರಚನೆ rev_info *revs,
				       ರಚನೆ repository *repo,
				       ಪೂರ್ಣಾಂಕ show_ಪ್ರಗತಿ)
{
	ರಚನೆ ಎಣಿಕೆ_references_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
		.stats = stats,
		.revs = revs,
	};

	ಆದರೆ (show_ಪ್ರಗತಿ)
		ದತ್ತಾಂಶ.ಪ್ರಗತಿ = ಆರಂಭ_delayed_ಪ್ರಗತಿ(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), ಎಣಿಕೆ_references, &ದತ್ತಾಂಶ);
	stop_ಪ್ರಗತಿ(&ದತ್ತಾಂಶ.ಪ್ರಗತಿ);
}

ರಚನೆ ಎಣಿಕೆ_objects_ದತ್ತಾಂಶ {
	ರಚನೆ object_database *odb;
	ರಚನೆ object_stats *stats;
	ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ;
};

ಸ್ಥಿರ ಶೂನ್ಯ check_ಅತಿದೊಡ್ಡ(ರಚನೆ object_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ, ರಚನೆ object_id *oid,
			  ಗಾತ್ರ_t ಮೌಲ್ಯ)
{
	ಆದರೆ (ಮೌಲ್ಯ > ದತ್ತಾಂಶ->ಮೌಲ್ಯ || is_null_oid(&ದತ್ತಾಂಶ->oid)) {
		oidcpy(&ದತ್ತಾಂಶ->oid, oid);
		ದತ್ತಾಂಶ->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
	}
}

ಸ್ಥಿರ ಗಾತ್ರ_t ಎಣಿಕೆ_ವೃಕ್ಷ_entries(ರಚನೆ object *obj)
{
	ರಚನೆ ವೃಕ್ಷ *t = object_as_type(obj, OBJ_ವೃಕ್ಷ, 0);
	ರಚನೆ name_ನಮೂದು ನಮೂದು;
	ರಚನೆ ವೃಕ್ಷ_desc desc;
	ಗಾತ್ರ_t ಎಣಿಕೆ_2 = 0;

	init_ವೃಕ್ಷ_desc(&desc, &t->object.oid, t->ಬಫರ್, t->ಗಾತ್ರ_2);
	ಆಗ (ವೃಕ್ಷ_ನಮೂದು(&desc, &ನಮೂದು))
		ಎಣಿಕೆ_2++;

	ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_objects(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ ಬಳಸದ, ರಚನೆ oid_array *oids,
			 ಎಣಿಕೆ object_type type, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ ಎಣಿಕೆ_objects_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb_ದತ್ತಾಂಶ;
	ರಚನೆ object_stats *stats = ದತ್ತಾಂಶ->stats;
	ಗಾತ್ರ_t object_ಎಣಿಕೆ;

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < oids->nr; i++) {
		ರಚನೆ object_info oi = OBJECT_INFO_INIT;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ inflated;
		ಗಾತ್ರ_t inflated_st = 0;
		ರಚನೆ commit *commit;
		ರಚನೆ object *obj;
		ಶೂನ್ಯ *content;
		off_t disk;
		ಪೂರ್ಣಾಂಕ eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		ಆದರೆ (odb_read_object_info_extended(ದತ್ತಾಂಶ->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_ಶೀಘ್ರ) < 0)
			ಮುಂದುವರಿ;
		inflated = cast_ಗಾತ್ರ_t_ಗಮ್ಯ_ulong(inflated_st);

		obj = parse_object_ಬಫರ್(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		ಆಯ್ಕೆ (type) {
		ಸ್ಥಿತಿ OBJ_ಟ್ಯಾಗ್:
			stats->type_counts.ಟ್ಯಾಗುಗಳು++;
			stats->inflated_sizes.ಟ್ಯಾಗುಗಳು += inflated;
			stats->disk_sizes.ಟ್ಯಾಗುಗಳು += disk;
			check_ಅತಿದೊಡ್ಡ(&stats->ಅತಿದೊಡ್ಡ.ಟ್ಯಾಗ್_ಗಾತ್ರ, &oids->oid[i],
				      inflated);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_ಅತಿದೊಡ್ಡ(&stats->ಅತಿದೊಡ್ಡ.commit_ಗಾತ್ರ, &oids->oid[i],
				      inflated);
			check_ಅತಿದೊಡ್ಡ(&stats->ಅತಿದೊಡ್ಡ.ಪೋಷಕ_ಎಣಿಕೆ, &oids->oid[i],
				      commit_list_ಎಣಿಕೆ(commit->parents));
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ OBJ_ವೃಕ್ಷ:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_ಅತಿದೊಡ್ಡ(&stats->ಅತಿದೊಡ್ಡ.ವೃಕ್ಷ_ಗಾತ್ರ, &oids->oid[i],
				      inflated);
			check_ಅತಿದೊಡ್ಡ(&stats->ಅತಿದೊಡ್ಡ.ವೃಕ್ಷ_entries, &oids->oid[i],
				      ಎಣಿಕೆ_ವೃಕ್ಷ_entries(obj));
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_ಅತಿದೊಡ್ಡ(&stats->ಅತಿದೊಡ್ಡ.blob_ಗಾತ್ರ, &oids->oid[i],
				      inflated);
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			BUG("invalid object type");
		}

		ಆದರೆ (!eaten)
			ಬಿಡುಗಡೆ(content);
	}

	object_ಎಣಿಕೆ = get_ಒಟ್ಟು_object_ಮೌಲ್ಯಗಳು(&stats->type_counts);
	display_ಪ್ರಗತಿ(ದತ್ತಾಂಶ->ಪ್ರಗತಿ, object_ಎಣಿಕೆ);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ structure_ಎಣಿಕೆ_objects(ರಚನೆ object_stats *stats,
				    ರಚನೆ rev_info *revs,
				    ರಚನೆ repository *repo, ಪೂರ್ಣಾಂಕ show_ಪ್ರಗತಿ)
{
	ರಚನೆ ಮಾರ್ಗ_walk_info info = ಮಾರ್ಗ_WALK_INFO_INIT;
	ರಚನೆ ಎಣಿಕೆ_objects_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.ಮಾರ್ಗ_fn = ಎಣಿಕೆ_objects;
	info.ಮಾರ್ಗ_fn_ದತ್ತಾಂಶ = &ದತ್ತಾಂಶ;

	ಆದರೆ (show_ಪ್ರಗತಿ)
		ದತ್ತಾಂಶ.ಪ್ರಗತಿ = ಆರಂಭ_delayed_ಪ್ರಗತಿ(repo, _("Counting objects"), 0);

	walk_objects_by_ಮಾರ್ಗ(&info);
	ಮಾರ್ಗ_walk_info_clear(&info);
	stop_ಪ್ರಗತಿ(&ದತ್ತಾಂಶ.ಪ್ರಗತಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_repo_structure(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			      ರಚನೆ repository *repo)
{
	ರಚನೆ stats_ಕೋಷ್ಟಕ ಕೋಷ್ಟಕ = {
		.ಅಡ್ಡಸಾಲುಗಳು = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	ಎಣಿಕೆ output_format format = FORMAT_ಕೋಷ್ಟಕ;
	ರಚನೆ repo_structure stats = { 0 };
	ರಚನೆ rev_info revs;
	ಪೂರ್ಣಾಂಕ show_ಪ್ರಗತಿ = -1;
	ರಚನೆ option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ಖಾಲಿ, &format, ಖಾಲಿ,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_ಪ್ರಗತಿ, N_("show progress")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, repo_structure_usage, 0);
	ಆದರೆ (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, ಪೂರ್ವಪ್ರತ್ಯಯ);

	ಆದರೆ (show_ಪ್ರಗತಿ < 0)
		show_ಪ್ರಗತಿ = isatty(2);

	structure_ಎಣಿಕೆ_references(&stats.refs, &revs, repo, show_ಪ್ರಗತಿ);
	structure_ಎಣಿಕೆ_objects(&stats.objects, &revs, repo, show_ಪ್ರಗತಿ);

	ಆಯ್ಕೆ (format) {
	ಸ್ಥಿತಿ FORMAT_ಕೋಷ್ಟಕ:
		stats_ಕೋಷ್ಟಕ_ಸಿದ್ಧತೆ_structure(&ಕೋಷ್ಟಕ, &stats);
		stats_ಕೋಷ್ಟಕ_print_structure(&ಕೋಷ್ಟಕ);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid output format");
	}

	stats_ಕೋಷ್ಟಕ_clear(&ಕೋಷ್ಟಕ);
	release_revisions(&revs);

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_repo(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	     ರಚನೆ repository *repo)
{
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, repo_usage, 0);

	ಹಿಂತಿರುಗಿ fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
}
