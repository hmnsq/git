#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "progress.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ref-filter.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "shallow.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	ਨਿਰਾ,
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ repo_info_usage[] = {
	REPO_INFO_USAGE,
	ਨਿਰਾ,
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	ਨਿਰਾ,
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ get_ਮੁੱਲ_fn(ਬਣਤਰ repository *repo, ਬਣਤਰ strbuf *buf);

ਗਿਣਤੀ output_format {
	FORMAT_ਸਾਰਣੀ,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

ਬਣਤਰ repo_info_ਖੇਤਰ {
	ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ;
	get_ਮੁੱਲ_fn *get_ਮੁੱਲ;
};

ਸਥਿਰ ਪੂਰਨ get_layout_bare(ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ, ਬਣਤਰ strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_layout_shallow(ਬਣਤਰ repository *repo, ਬਣਤਰ strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_object_format(ਬਣਤਰ repository *repo, ਬਣਤਰ strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_ਰਸਤਾ_commondir_absolute(ਬਣਤਰ repository *repo, ਬਣਤਰ strbuf *buf)
{
	ਅਚਲ ਅੱਖਰ *common_dir = repo_get_common_dir(repo);

	ਜੇ (!common_dir)
		ਵਾਪਸ error(_("unable to get common directory"));

	format_ਰਸਤਾ(buf, common_dir, repo->ਅਗੇਤਰ, ਰਸਤਾ_FORMAT_CANONICAL);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_ਰਸਤਾ_commondir_relative(ਬਣਤਰ repository *repo, ਬਣਤਰ strbuf *buf)
{
	ਅਚਲ ਅੱਖਰ *common_dir = repo_get_common_dir(repo);

	ਜੇ (!common_dir)
		ਵਾਪਸ error(_("unable to get common directory"));

	format_ਰਸਤਾ(buf, common_dir, repo->ਅਗੇਤਰ, ਰਸਤਾ_FORMAT_RELATIVE);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_ਰਸਤਾ_gitdir_absolute(ਬਣਤਰ repository *repo, ਬਣਤਰ strbuf *buf)
{
	ਅਚਲ ਅੱਖਰ *git_dir = repo_get_git_dir(repo);

	ਜੇ (!git_dir)
		ਵਾਪਸ error(_("unable to get git directory"));

	format_ਰਸਤਾ(buf, git_dir, repo->ਅਗੇਤਰ, ਰਸਤਾ_FORMAT_CANONICAL);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_ਰਸਤਾ_gitdir_relative(ਬਣਤਰ repository *repo, ਬਣਤਰ strbuf *buf)
{
	ਅਚਲ ਅੱਖਰ *git_dir = repo_get_git_dir(repo);

	ਜੇ (!git_dir)
		ਵਾਪਸ error(_("unable to get git directory"));

	format_ਰਸਤਾ(buf, git_dir, repo->ਅਗੇਤਰ, ਰਸਤਾ_FORMAT_RELATIVE);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_references_format(ਬਣਤਰ repository *repo, ਬਣਤਰ strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_ਮੰਜ਼ਿਲ_name(repo->ref_storage_format));
	ਵਾਪਸ 0;
}

/* repo_info_field keys must be in lexicographical order */
ਸਥਿਰ ਅਚਲ ਬਣਤਰ repo_info_ਖੇਤਰ repo_info_ਖੇਤਰ[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_ਰਸਤਾ_commondir_absolute },
	{ "path.commondir.relative", get_ਰਸਤਾ_commondir_relative },
	{ "path.gitdir.absolute", get_ਰਸਤਾ_gitdir_absolute },
	{ "path.gitdir.relative", get_ਰਸਤਾ_gitdir_relative },
	{ "references.format", get_references_format },
};

ਸਥਿਰ ਪੂਰਨ repo_info_ਖੇਤਰ_cmp(ਅਚਲ ਖਾਲੀ *va, ਅਚਲ ਖਾਲੀ *vb)
{
	ਅਚਲ ਬਣਤਰ repo_info_ਖੇਤਰ *a = va;
	ਅਚਲ ਬਣਤਰ repo_info_ਖੇਤਰ *b = vb;

	ਵਾਪਸ strcmp(a->ਕੁੰਜੀ, b->ਕੁੰਜੀ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ repo_info_ਖੇਤਰ *get_repo_info_ਖੇਤਰ(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ)
{
	ਅਚਲ ਬਣਤਰ repo_info_ਖੇਤਰ ਖੋਜ_ਕੁੰਜੀ = { ਕੁੰਜੀ, ਨਿਰਾ };
	ਅਚਲ ਬਣਤਰ repo_info_ਖੇਤਰ *ਮਿਲਿਆ_2 = bsearch(&ਖੋਜ_ਕੁੰਜੀ,
						      repo_info_ਖੇਤਰ,
						      ARRAY_ਮਾਪ(repo_info_ਖੇਤਰ),
						      ਆਕਾਰ(*ਮਿਲਿਆ_2),
						      repo_info_ਖੇਤਰ_cmp);

	ਵਾਪਸ ਮਿਲਿਆ_2;
}

ਸਥਿਰ ਖਾਲੀ print_ਖੇਤਰ(ਗਿਣਤੀ output_format format, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ,
			ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
	ਚੋਣ (format) {
	ਹਾਲਤ FORMAT_NEWLINE_TERMINATED:
		ਲਿਖੋ("%s=", ਕੁੰਜੀ);
		quote_c_style(ਮੁੱਲ, ਨਿਰਾ, stdout, 0);
		ਅੱਖਰ_ਲਿਖੋ('\n');
		ਰੋਕੋ;
	ਹਾਲਤ FORMAT_NUL_TERMINATED:
		ਲਿਖੋ("%s\n%s%c", ਕੁੰਜੀ, ਮੁੱਲ, '\0');
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("not a valid output format: %d", format);
	}
}

ਸਥਿਰ ਪੂਰਨ print_fields(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv,
			ਬਣਤਰ repository *repo,
			ਗਿਣਤੀ output_format format)
{
	ਪੂਰਨ ret = 0;
	ਬਣਤਰ strbuf valbuf = STRBUF_INIT;

	ਲਈ (ਪੂਰਨ i = 0; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ = argv[i];
		ਅਚਲ ਬਣਤਰ repo_info_ਖੇਤਰ *ਖੇਤਰ = get_repo_info_ਖੇਤਰ(ਕੁੰਜੀ);

		ਜੇ (!ਖੇਤਰ) {
			ret = error(_("key '%s' not found"), ਕੁੰਜੀ);
			ਜਾਰੀ;
		}

		strbuf_reset(&valbuf);
		ਖੇਤਰ->get_ਮੁੱਲ(repo, &valbuf);
		print_ਖੇਤਰ(format, ਕੁੰਜੀ, valbuf.buf);
	}

	strbuf_release(&valbuf);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ print_all_fields(ਬਣਤਰ repository *repo,
			    ਗਿਣਤੀ output_format format)
{
	ਬਣਤਰ strbuf valbuf = STRBUF_INIT;

	ਲਈ (ਮਾਪ_t i = 0; i < ARRAY_ਮਾਪ(repo_info_ਖੇਤਰ); i++) {
		ਅਚਲ ਬਣਤਰ repo_info_ਖੇਤਰ *ਖੇਤਰ = &repo_info_ਖੇਤਰ[i];

		strbuf_reset(&valbuf);
		ਖੇਤਰ->get_ਮੁੱਲ(repo, &valbuf);
		print_ਖੇਤਰ(format, ਖੇਤਰ->ਕੁੰਜੀ, valbuf.buf);
	}

	strbuf_release(&valbuf);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ print_ਕੁੰਜੀਆਂ(ਗਿਣਤੀ output_format format)
{
	ਅੱਖਰ sep;

	ਚੋਣ (format) {
	ਹਾਲਤ FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		ਰੋਕੋ;
	ਹਾਲਤ FORMAT_NUL_TERMINATED:
		sep = '\0';
		ਰੋਕੋ;
	ਮੂਲ:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	ਲਈ (ਮਾਪ_t i = 0; i < ARRAY_ਮਾਪ(repo_info_ਖੇਤਰ); i++) {
		ਅਚਲ ਬਣਤਰ repo_info_ਖੇਤਰ *ਖੇਤਰ = &repo_info_ਖੇਤਰ[i];
		ਲਿਖੋ("%s%c", ਖੇਤਰ->ਕੁੰਜੀ, sep);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_format_cb(ਅਚਲ ਬਣਤਰ option *opt,
			   ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset ਅਣਵਰਤਿਆ)
{
	ਗਿਣਤੀ output_format *format = opt->ਮੁੱਲ;

	ਜੇ (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "table"))
		*format = FORMAT_ਸਾਰਣੀ;
	ਨਹੀਂਤਾਂ
		die(_("invalid format '%s'"), arg);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cmd_repo_info(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ repository *repo)
{
	ਗਿਣਤੀ output_format format = FORMAT_NEWLINE_TERMINATED;
	ਪੂਰਨ all_ਕੁੰਜੀਆਂ = 0;
	ਪੂਰਨ show_ਕੁੰਜੀਆਂ = 0;
	ਬਣਤਰ option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ਨਿਰਾ, &format, ਨਿਰਾ,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_ਕੁੰਜੀਆਂ, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_ਕੁੰਜੀਆਂ, N_("show keys")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, repo_info_usage, 0);

	ਜੇ (show_ਕੁੰਜੀਆਂ && (all_ਕੁੰਜੀਆਂ || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	ਜੇ (show_ਕੁੰਜੀਆਂ)
		ਵਾਪਸ print_ਕੁੰਜੀਆਂ(format);

	ਜੇ (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	ਜੇ (all_ਕੁੰਜੀਆਂ && argc)
		die(_("--all and <key> cannot be used together"));

	ਜੇ (all_ਕੁੰਜੀਆਂ)
		ਵਾਪਸ print_all_fields(repo, format);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ print_fields(argc, argv, repo, format);
}

ਬਣਤਰ object_ਅੰਕੜੇ {
	ਬਣਤਰ object_id oid;
	ਮਾਪ_t ਮੁੱਲ;
};

ਬਣਤਰ ਸਭ_ਤੋਂ_ਵੱਡਾ_objects {
	ਬਣਤਰ object_ਅੰਕੜੇ ਟੈਗ_ਮਾਪ;
	ਬਣਤਰ object_ਅੰਕੜੇ commit_ਮਾਪ;
	ਬਣਤਰ object_ਅੰਕੜੇ ਰੁੱਖ_ਮਾਪ;
	ਬਣਤਰ object_ਅੰਕੜੇ blob_ਮਾਪ;

	ਬਣਤਰ object_ਅੰਕੜੇ ਮਾਪਾ_ਗਿਣਤੀ;
	ਬਣਤਰ object_ਅੰਕੜੇ ਰੁੱਖ_entries;
};

ਬਣਤਰ ref_stats {
	ਮਾਪ_t branches;
	ਮਾਪ_t remotes;
	ਮਾਪ_t ਟੈਗ_ਸੂਚੀ;
	ਮਾਪ_t others;
};

ਬਣਤਰ object_ਮੁੱਲਾਂ {
	ਮਾਪ_t ਟੈਗ_ਸੂਚੀ;
	ਮਾਪ_t commits;
	ਮਾਪ_t trees;
	ਮਾਪ_t blobs;
};

ਬਣਤਰ object_stats {
	ਬਣਤਰ object_ਮੁੱਲਾਂ type_counts;
	ਬਣਤਰ object_ਮੁੱਲਾਂ inflated_sizes;
	ਬਣਤਰ object_ਮੁੱਲਾਂ disk_sizes;
	ਬਣਤਰ ਸਭ_ਤੋਂ_ਵੱਡਾ_objects ਸਭ_ਤੋਂ_ਵੱਡਾ;
};

ਬਣਤਰ repo_structure {
	ਬਣਤਰ ref_stats refs;
	ਬਣਤਰ object_stats objects;
};

ਬਣਤਰ stats_ਸਾਰਣੀ {
	ਬਣਤਰ string_list ਪੰਕਤੀਆਂ;
	ਬਣਤਰ string_list annotations;

	ਪੂਰਨ name_ਖਾਨਾ_ਚੌੜਾਈ;
	ਪੂਰਨ ਮੁੱਲ_ਖਾਨਾ_ਚੌੜਾਈ;
	ਪੂਰਨ unit_ਖਾਨਾ_ਚੌੜਾਈ;
};

/*
 * Holds column data that gets stored for each row.
 */
ਬਣਤਰ stats_ਸਾਰਣੀ_ਇੰਦਰਾਜ {
	ਅੱਖਰ *ਮੁੱਲ;
	ਅਚਲ ਅੱਖਰ *unit;
	ਮਾਪ_t ਸੂਚਕਾਂਕ;
	ਬਣਤਰ object_id *oid;
};

ਸਥਿਰ ਖਾਲੀ stats_ਸਾਰਣੀ_vaddf(ਬਣਤਰ stats_ਸਾਰਣੀ *ਸਾਰਣੀ,
			      ਬਣਤਰ stats_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
			      ਅਚਲ ਅੱਖਰ *format, va_list ap)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਅੱਖਰ *formatted_name;
	ਪੂਰਨ name_ਚੌੜਾਈ;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, ਨਿਰਾ);
	name_ਚੌੜਾਈ = utf8_strwidth(formatted_name);

	ਵਸਤੂ = string_list_append_nodup(&ਸਾਰਣੀ->ਪੰਕਤੀਆਂ, formatted_name);
	ਵਸਤੂ->util = ਇੰਦਰਾਜ;

	ਜੇ (name_ਚੌੜਾਈ > ਸਾਰਣੀ->name_ਖਾਨਾ_ਚੌੜਾਈ)
		ਸਾਰਣੀ->name_ਖਾਨਾ_ਚੌੜਾਈ = name_ਚੌੜਾਈ;
	ਜੇ (!ਇੰਦਰਾਜ)
		ਵਾਪਸ;
	ਜੇ (ਇੰਦਰਾਜ->oid) {
		ਇੰਦਰਾਜ->ਸੂਚਕਾਂਕ = ਸਾਰਣੀ->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)ਇੰਦਰਾਜ->ਸੂਚਕਾਂਕ,
			    oid_ਮੰਜ਼ਿਲ_hex(ਇੰਦਰਾਜ->oid));
		string_list_append_nodup(&ਸਾਰਣੀ->annotations, strbuf_detach(&buf, ਨਿਰਾ));
	}
	ਜੇ (ਇੰਦਰਾਜ->ਮੁੱਲ) {
		ਪੂਰਨ ਮੁੱਲ_ਚੌੜਾਈ = utf8_strwidth(ਇੰਦਰਾਜ->ਮੁੱਲ);
		ਜੇ (ਮੁੱਲ_ਚੌੜਾਈ > ਸਾਰਣੀ->ਮੁੱਲ_ਖਾਨਾ_ਚੌੜਾਈ)
			ਸਾਰਣੀ->ਮੁੱਲ_ਖਾਨਾ_ਚੌੜਾਈ = ਮੁੱਲ_ਚੌੜਾਈ;
	}
	ਜੇ (ਇੰਦਰਾਜ->unit) {
		ਪੂਰਨ unit_ਚੌੜਾਈ = utf8_strwidth(ਇੰਦਰਾਜ->unit);
		ਜੇ (unit_ਚੌੜਾਈ > ਸਾਰਣੀ->unit_ਖਾਨਾ_ਚੌੜਾਈ)
			ਸਾਰਣੀ->unit_ਖਾਨਾ_ਚੌੜਾਈ = unit_ਚੌੜਾਈ;
	}

	strbuf_release(&buf);
}

ਸਥਿਰ ਖਾਲੀ stats_ਸਾਰਣੀ_addf(ਬਣਤਰ stats_ਸਾਰਣੀ *ਸਾਰਣੀ, ਅਚਲ ਅੱਖਰ *format, ...)
{
	va_list ap;

	va_ਸ਼ੁਰੂ(ap, format);
	stats_ਸਾਰਣੀ_vaddf(ਸਾਰਣੀ, ਨਿਰਾ, format, ap);
	va_ਸਿਰਾ(ap);
}

ਸਥਿਰ ਖਾਲੀ stats_ਸਾਰਣੀ_ਗਿਣਤੀ_addf(ਬਣਤਰ stats_ਸਾਰਣੀ *ਸਾਰਣੀ, ਮਾਪ_t ਮੁੱਲ,
				   ਅਚਲ ਅੱਖਰ *format, ...)
{
	ਬਣਤਰ stats_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
	va_list ap;

	CALLOC_ARRAY(ਇੰਦਰਾਜ, 1);
	humanise_ਗਿਣਤੀ(ਮੁੱਲ, &ਇੰਦਰਾਜ->ਮੁੱਲ, &ਇੰਦਰਾਜ->unit);

	va_ਸ਼ੁਰੂ(ap, format);
	stats_ਸਾਰਣੀ_vaddf(ਸਾਰਣੀ, ਇੰਦਰਾਜ, format, ap);
	va_ਸਿਰਾ(ap);
}

ਸਥਿਰ ਖਾਲੀ stats_ਸਾਰਣੀ_object_ਗਿਣਤੀ_addf(ਬਣਤਰ stats_ਸਾਰਣੀ *ਸਾਰਣੀ,
					  ਬਣਤਰ object_id *oid, ਮਾਪ_t ਮੁੱਲ,
					  ਅਚਲ ਅੱਖਰ *format, ...)
{
	ਬਣਤਰ stats_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
	va_list ap;

	CALLOC_ARRAY(ਇੰਦਰਾਜ, 1);
	humanise_ਗਿਣਤੀ(ਮੁੱਲ, &ਇੰਦਰਾਜ->ਮੁੱਲ, &ਇੰਦਰਾਜ->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	ਜੇ (!is_null_oid(oid))
		ਇੰਦਰਾਜ->oid = oid;

	va_ਸ਼ੁਰੂ(ap, format);
	stats_ਸਾਰਣੀ_vaddf(ਸਾਰਣੀ, ਇੰਦਰਾਜ, format, ap);
	va_ਸਿਰਾ(ap);
}

ਸਥਿਰ ਖਾਲੀ stats_ਸਾਰਣੀ_ਮਾਪ_addf(ਬਣਤਰ stats_ਸਾਰਣੀ *ਸਾਰਣੀ, ਮਾਪ_t ਮੁੱਲ,
				  ਅਚਲ ਅੱਖਰ *format, ...)
{
	ਬਣਤਰ stats_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
	va_list ap;

	CALLOC_ARRAY(ਇੰਦਰਾਜ, 1);
	humanise_ਬਾਈਟਾਂ(ਮੁੱਲ, &ਇੰਦਰਾਜ->ਮੁੱਲ, &ਇੰਦਰਾਜ->unit, HUMANISE_COMPACT);

	va_ਸ਼ੁਰੂ(ap, format);
	stats_ਸਾਰਣੀ_vaddf(ਸਾਰਣੀ, ਇੰਦਰਾਜ, format, ap);
	va_ਸਿਰਾ(ap);
}

ਸਥਿਰ ਖਾਲੀ stats_ਸਾਰਣੀ_object_ਮਾਪ_addf(ਬਣਤਰ stats_ਸਾਰਣੀ *ਸਾਰਣੀ,
					 ਬਣਤਰ object_id *oid, ਮਾਪ_t ਮੁੱਲ,
					 ਅਚਲ ਅੱਖਰ *format, ...)
{
	ਬਣਤਰ stats_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
	va_list ap;

	CALLOC_ARRAY(ਇੰਦਰਾਜ, 1);
	humanise_ਬਾਈਟਾਂ(ਮੁੱਲ, &ਇੰਦਰਾਜ->ਮੁੱਲ, &ਇੰਦਰਾਜ->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	ਜੇ (!is_null_oid(oid))
		ਇੰਦਰਾਜ->oid = oid;

	va_ਸ਼ੁਰੂ(ap, format);
	stats_ਸਾਰਣੀ_vaddf(ਸਾਰਣੀ, ਇੰਦਰਾਜ, format, ap);
	va_ਸਿਰਾ(ap);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਮਾਪ_t get_ਕੁੱਲ_reference_ਗਿਣਤੀ(ਬਣਤਰ ref_stats *stats)
{
	ਵਾਪਸ stats->branches + stats->remotes + stats->ਟੈਗ_ਸੂਚੀ + stats->others;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਮਾਪ_t get_ਕੁੱਲ_object_ਮੁੱਲਾਂ(ਬਣਤਰ object_ਮੁੱਲਾਂ *ਮੁੱਲਾਂ)
{
	ਵਾਪਸ ਮੁੱਲਾਂ->ਟੈਗ_ਸੂਚੀ + ਮੁੱਲਾਂ->commits + ਮੁੱਲਾਂ->trees + ਮੁੱਲਾਂ->blobs;
}

ਸਥਿਰ ਖਾਲੀ stats_ਸਾਰਣੀ_ਤਿਆਰੀ_structure(ਬਣਤਰ stats_ਸਾਰਣੀ *ਸਾਰਣੀ,
					ਬਣਤਰ repo_structure *stats)
{
	ਬਣਤਰ object_stats *objects = &stats->objects;
	ਬਣਤਰ ref_stats *refs = &stats->refs;
	ਮਾਪ_t inflated_object_ਕੁੱਲ;
	ਮਾਪ_t object_ਗਿਣਤੀ_ਕੁੱਲ;
	ਮਾਪ_t disk_object_ਕੁੱਲ;
	ਮਾਪ_t ref_ਕੁੱਲ;

	ref_ਕੁੱਲ = get_ਕੁੱਲ_reference_ਗਿਣਤੀ(refs);
	stats_ਸਾਰਣੀ_addf(ਸਾਰਣੀ, "* %s", _("References"));
	stats_ਸਾਰਣੀ_ਗਿਣਤੀ_addf(ਸਾਰਣੀ, ref_ਕੁੱਲ, "  * %s", _("Count"));
	stats_ਸਾਰਣੀ_ਗਿਣਤੀ_addf(ਸਾਰਣੀ, refs->branches, "    * %s", _("Branches"));
	stats_ਸਾਰਣੀ_ਗਿਣਤੀ_addf(ਸਾਰਣੀ, refs->ਟੈਗ_ਸੂਚੀ, "    * %s", _("Tags"));
	stats_ਸਾਰਣੀ_ਗਿਣਤੀ_addf(ਸਾਰਣੀ, refs->remotes, "    * %s", _("Remotes"));
	stats_ਸਾਰਣੀ_ਗਿਣਤੀ_addf(ਸਾਰਣੀ, refs->others, "    * %s", _("Others"));

	object_ਗਿਣਤੀ_ਕੁੱਲ = get_ਕੁੱਲ_object_ਮੁੱਲਾਂ(&objects->type_counts);
	stats_ਸਾਰਣੀ_addf(ਸਾਰਣੀ, "");
	stats_ਸਾਰਣੀ_addf(ਸਾਰਣੀ, "* %s", _("Reachable objects"));
	stats_ਸਾਰਣੀ_ਗਿਣਤੀ_addf(ਸਾਰਣੀ, object_ਗਿਣਤੀ_ਕੁੱਲ, "  * %s", _("Count"));
	stats_ਸਾਰਣੀ_ਗਿਣਤੀ_addf(ਸਾਰਣੀ, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_ਸਾਰਣੀ_ਗਿਣਤੀ_addf(ਸਾਰਣੀ, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_ਸਾਰਣੀ_ਗਿਣਤੀ_addf(ਸਾਰਣੀ, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_ਸਾਰਣੀ_ਗਿਣਤੀ_addf(ਸਾਰਣੀ, objects->type_counts.ਟੈਗ_ਸੂਚੀ,
			       "    * %s", _("Tags"));

	inflated_object_ਕੁੱਲ = get_ਕੁੱਲ_object_ਮੁੱਲਾਂ(&objects->inflated_sizes);
	stats_ਸਾਰਣੀ_ਮਾਪ_addf(ਸਾਰਣੀ, inflated_object_ਕੁੱਲ,
			      "  * %s", _("Inflated size"));
	stats_ਸਾਰਣੀ_ਮਾਪ_addf(ਸਾਰਣੀ, objects->inflated_sizes.commits,
			      "    * %s", _("Commits"));
	stats_ਸਾਰਣੀ_ਮਾਪ_addf(ਸਾਰਣੀ, objects->inflated_sizes.trees,
			      "    * %s", _("Trees"));
	stats_ਸਾਰਣੀ_ਮਾਪ_addf(ਸਾਰਣੀ, objects->inflated_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_ਸਾਰਣੀ_ਮਾਪ_addf(ਸਾਰਣੀ, objects->inflated_sizes.ਟੈਗ_ਸੂਚੀ,
			      "    * %s", _("Tags"));

	disk_object_ਕੁੱਲ = get_ਕੁੱਲ_object_ਮੁੱਲਾਂ(&objects->disk_sizes);
	stats_ਸਾਰਣੀ_ਮਾਪ_addf(ਸਾਰਣੀ, disk_object_ਕੁੱਲ,
			      "  * %s", _("Disk size"));
	stats_ਸਾਰਣੀ_ਮਾਪ_addf(ਸਾਰਣੀ, objects->disk_sizes.commits,
			      "    * %s", _("Commits"));
	stats_ਸਾਰਣੀ_ਮਾਪ_addf(ਸਾਰਣੀ, objects->disk_sizes.trees,
			      "    * %s", _("Trees"));
	stats_ਸਾਰਣੀ_ਮਾਪ_addf(ਸਾਰਣੀ, objects->disk_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_ਸਾਰਣੀ_ਮਾਪ_addf(ਸਾਰਣੀ, objects->disk_sizes.ਟੈਗ_ਸੂਚੀ,
			      "    * %s", _("Tags"));

	stats_ਸਾਰਣੀ_addf(ਸਾਰਣੀ, "");
	stats_ਸਾਰਣੀ_addf(ਸਾਰਣੀ, "* %s", _("Largest objects"));
	stats_ਸਾਰਣੀ_addf(ਸਾਰਣੀ, "  * %s", _("Commits"));
	stats_ਸਾਰਣੀ_object_ਮਾਪ_addf(ਸਾਰਣੀ,
				     &objects->ਸਭ_ਤੋਂ_ਵੱਡਾ.commit_ਮਾਪ.oid,
				     objects->ਸਭ_ਤੋਂ_ਵੱਡਾ.commit_ਮਾਪ.ਮੁੱਲ,
				     "    * %s", _("Maximum size"));
	stats_ਸਾਰਣੀ_object_ਗਿਣਤੀ_addf(ਸਾਰਣੀ,
				      &objects->ਸਭ_ਤੋਂ_ਵੱਡਾ.ਮਾਪਾ_ਗਿਣਤੀ.oid,
				      objects->ਸਭ_ਤੋਂ_ਵੱਡਾ.ਮਾਪਾ_ਗਿਣਤੀ.ਮੁੱਲ,
				      "    * %s", _("Maximum parents"));
	stats_ਸਾਰਣੀ_addf(ਸਾਰਣੀ, "  * %s", _("Trees"));
	stats_ਸਾਰਣੀ_object_ਮਾਪ_addf(ਸਾਰਣੀ,
				     &objects->ਸਭ_ਤੋਂ_ਵੱਡਾ.ਰੁੱਖ_ਮਾਪ.oid,
				     objects->ਸਭ_ਤੋਂ_ਵੱਡਾ.ਰੁੱਖ_ਮਾਪ.ਮੁੱਲ,
				     "    * %s", _("Maximum size"));
	stats_ਸਾਰਣੀ_object_ਗਿਣਤੀ_addf(ਸਾਰਣੀ,
				      &objects->ਸਭ_ਤੋਂ_ਵੱਡਾ.ਰੁੱਖ_entries.oid,
				      objects->ਸਭ_ਤੋਂ_ਵੱਡਾ.ਰੁੱਖ_entries.ਮੁੱਲ,
				      "    * %s", _("Maximum entries"));
	stats_ਸਾਰਣੀ_addf(ਸਾਰਣੀ, "  * %s", _("Blobs"));
	stats_ਸਾਰਣੀ_object_ਮਾਪ_addf(ਸਾਰਣੀ,
				     &objects->ਸਭ_ਤੋਂ_ਵੱਡਾ.blob_ਮਾਪ.oid,
				     objects->ਸਭ_ਤੋਂ_ਵੱਡਾ.blob_ਮਾਪ.ਮੁੱਲ,
				     "    * %s", _("Maximum size"));
	stats_ਸਾਰਣੀ_addf(ਸਾਰਣੀ, "  * %s", _("Tags"));
	stats_ਸਾਰਣੀ_object_ਮਾਪ_addf(ਸਾਰਣੀ,
				     &objects->ਸਭ_ਤੋਂ_ਵੱਡਾ.ਟੈਗ_ਮਾਪ.oid,
				     objects->ਸਭ_ਤੋਂ_ਵੱਡਾ.ਟੈਗ_ਮਾਪ.ਮੁੱਲ,
				     "    * %s", _("Maximum size"));
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੂਚਕਾਂਕ_ਚੌੜਾਈ 4

ਸਥਿਰ ਖਾਲੀ stats_ਸਾਰਣੀ_print_structure(ਅਚਲ ਬਣਤਰ stats_ਸਾਰਣੀ *ਸਾਰਣੀ)
{
	ਅਚਲ ਅੱਖਰ *name_ਖਾਨਾ_title = _("Repository structure");
	ਅਚਲ ਅੱਖਰ *ਮੁੱਲ_ਖਾਨਾ_title = _("Value");
	ਪੂਰਨ title_name_ਚੌੜਾਈ = utf8_strwidth(name_ਖਾਨਾ_title);
	ਪੂਰਨ title_ਮੁੱਲ_ਚੌੜਾਈ = utf8_strwidth(ਮੁੱਲ_ਖਾਨਾ_title);
	ਪੂਰਨ name_ਖਾਨਾ_ਚੌੜਾਈ = ਸਾਰਣੀ->name_ਖਾਨਾ_ਚੌੜਾਈ;
	ਪੂਰਨ ਮੁੱਲ_ਖਾਨਾ_ਚੌੜਾਈ = ਸਾਰਣੀ->ਮੁੱਲ_ਖਾਨਾ_ਚੌੜਾਈ;
	ਪੂਰਨ unit_ਖਾਨਾ_ਚੌੜਾਈ = ਸਾਰਣੀ->unit_ਖਾਨਾ_ਚੌੜਾਈ;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;

	ਜੇ (title_name_ਚੌੜਾਈ > name_ਖਾਨਾ_ਚੌੜਾਈ)
		name_ਖਾਨਾ_ਚੌੜਾਈ = title_name_ਚੌੜਾਈ;
	ਜੇ (title_ਮੁੱਲ_ਚੌੜਾਈ > ਮੁੱਲ_ਖਾਨਾ_ਚੌੜਾਈ + unit_ਖਾਨਾ_ਚੌੜਾਈ + 1)
		ਮੁੱਲ_ਖਾਨਾ_ਚੌੜਾਈ = title_ਮੁੱਲ_ਚੌੜਾਈ - unit_ਖਾਨਾ_ਚੌੜਾਈ;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_ਖੱਬਾ, name_ਖਾਨਾ_ਚੌੜਾਈ + ਸੂਚਕਾਂਕ_ਚੌੜਾਈ,
			  name_ਖਾਨਾ_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_ਖੱਬਾ,
			  ਮੁੱਲ_ਖਾਨਾ_ਚੌੜਾਈ + unit_ਖਾਨਾ_ਚੌੜਾਈ + 1, ਮੁੱਲ_ਖਾਨਾ_title);
	strbuf_addstr(&buf, " |");
	ਲਿਖੋ("%s\n", buf.buf);

	ਲਿਖੋ("| ");
	ਲਈ (ਪੂਰਨ i = 0; i < name_ਖਾਨਾ_ਚੌੜਾਈ + ਸੂਚਕਾਂਕ_ਚੌੜਾਈ; i++)
		ਅੱਖਰ_ਲਿਖੋ('-');
	ਲਿਖੋ(" | ");
	ਲਈ (ਪੂਰਨ i = 0; i < ਮੁੱਲ_ਖਾਨਾ_ਚੌੜਾਈ + unit_ਖਾਨਾ_ਚੌੜਾਈ + 1; i++)
		ਅੱਖਰ_ਲਿਖੋ('-');
	ਲਿਖੋ(" |\n");

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਸਾਰਣੀ->ਪੰਕਤੀਆਂ) {
		ਬਣਤਰ stats_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = ਵਸਤੂ->util;
		ਅਚਲ ਅੱਖਰ *ਮੁੱਲ = "";
		ਅਚਲ ਅੱਖਰ *unit = "";

		ਜੇ (ਇੰਦਰਾਜ) {
			ਮੁੱਲ = ਇੰਦਰਾਜ->ਮੁੱਲ;
			ਜੇ (ਇੰਦਰਾਜ->unit)
				unit = ਇੰਦਰਾਜ->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_ਖੱਬਾ, name_ਖਾਨਾ_ਚੌੜਾਈ, ਵਸਤੂ->string);

		ਜੇ (ਇੰਦਰਾਜ && ਇੰਦਰਾਜ->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)ਇੰਦਰਾਜ->ਸੂਚਕਾਂਕ);
		ਨਹੀਂਤਾਂ
			strbuf_addchars(&buf, ' ', ਸੂਚਕਾਂਕ_ਚੌੜਾਈ);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_ਸੱਜਾ, ਮੁੱਲ_ਖਾਨਾ_ਚੌੜਾਈ, ਮੁੱਲ);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_ਖੱਬਾ, unit_ਖਾਨਾ_ਚੌੜਾਈ, unit);
		strbuf_addstr(&buf, " |");
		ਲਿਖੋ("%s\n", buf.buf);
	}

	ਜੇ (ਸਾਰਣੀ->annotations.nr) {
		ਲਿਖੋ("\n");
		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਸਾਰਣੀ->annotations)
			ਲਿਖੋ("%s\n", ਵਸਤੂ->string);
	}

	strbuf_release(&buf);
}

ਸਥਿਰ ਖਾਲੀ stats_ਸਾਰਣੀ_clear(ਬਣਤਰ stats_ਸਾਰਣੀ *ਸਾਰਣੀ)
{
	ਬਣਤਰ stats_ਸਾਰਣੀ_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਸਾਰਣੀ->ਪੰਕਤੀਆਂ) {
		ਇੰਦਰਾਜ = ਵਸਤੂ->util;
		ਜੇ (ਇੰਦਰਾਜ)
			ਮੁਕਤ(ਇੰਦਰਾਜ->ਮੁੱਲ);
	}

	string_list_clear(&ਸਾਰਣੀ->ਪੰਕਤੀਆਂ, 1);
	string_list_clear(&ਸਾਰਣੀ->annotations, 1);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ print_keyvalue(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅੱਖਰ ਕੁੰਜੀ_delim, ਮਾਪ_t ਮੁੱਲ,
				  ਅੱਖਰ ਮੁੱਲ_delim)
{
	ਲਿਖੋ("%s%c%" PRIuMAX "%c", ਕੁੰਜੀ, ਕੁੰਜੀ_delim, (uintmax_t)ਮੁੱਲ,
	       ਮੁੱਲ_delim);
}

ਸਥਿਰ ਖਾਲੀ print_object_ਅੰਕੜੇ(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅੱਖਰ ਕੁੰਜੀ_delim,
			      ਬਣਤਰ object_ਅੰਕੜੇ *ਅੰਕੜੇ, ਅੱਖਰ ਮੁੱਲ_delim)
{
	print_keyvalue(ਕੁੰਜੀ, ਕੁੰਜੀ_delim, ਅੰਕੜੇ->ਮੁੱਲ, ਮੁੱਲ_delim);
	ਲਿਖੋ("%s_oid%c%s%c", ਕੁੰਜੀ, ਕੁੰਜੀ_delim, oid_ਮੰਜ਼ਿਲ_hex(&ਅੰਕੜੇ->oid),
	       ਮੁੱਲ_delim);
}

ਸਥਿਰ ਖਾਲੀ structure_keyvalue_print(ਬਣਤਰ repo_structure *stats,
				     ਅੱਖਰ ਕੁੰਜੀ_delim, ਅੱਖਰ ਮੁੱਲ_delim)
{
	print_keyvalue("references.branches.count", ਕੁੰਜੀ_delim,
		       stats->refs.branches, ਮੁੱਲ_delim);
	print_keyvalue("references.tags.count", ਕੁੰਜੀ_delim,
		       stats->refs.ਟੈਗ_ਸੂਚੀ, ਮੁੱਲ_delim);
	print_keyvalue("references.remotes.count", ਕੁੰਜੀ_delim,
		       stats->refs.remotes, ਮੁੱਲ_delim);
	print_keyvalue("references.others.count", ਕੁੰਜੀ_delim,
		       stats->refs.others, ਮੁੱਲ_delim);

	print_keyvalue("objects.commits.count", ਕੁੰਜੀ_delim,
		       stats->objects.type_counts.commits, ਮੁੱਲ_delim);
	print_keyvalue("objects.trees.count", ਕੁੰਜੀ_delim,
		       stats->objects.type_counts.trees, ਮੁੱਲ_delim);
	print_keyvalue("objects.blobs.count", ਕੁੰਜੀ_delim,
		       stats->objects.type_counts.blobs, ਮੁੱਲ_delim);
	print_keyvalue("objects.tags.count", ਕੁੰਜੀ_delim,
		       stats->objects.type_counts.ਟੈਗ_ਸੂਚੀ, ਮੁੱਲ_delim);

	print_keyvalue("objects.commits.inflated_size", ਕੁੰਜੀ_delim,
		       stats->objects.inflated_sizes.commits, ਮੁੱਲ_delim);
	print_keyvalue("objects.trees.inflated_size", ਕੁੰਜੀ_delim,
		       stats->objects.inflated_sizes.trees, ਮੁੱਲ_delim);
	print_keyvalue("objects.blobs.inflated_size", ਕੁੰਜੀ_delim,
		       stats->objects.inflated_sizes.blobs, ਮੁੱਲ_delim);
	print_keyvalue("objects.tags.inflated_size", ਕੁੰਜੀ_delim,
		       stats->objects.inflated_sizes.ਟੈਗ_ਸੂਚੀ, ਮੁੱਲ_delim);

	print_keyvalue("objects.commits.disk_size", ਕੁੰਜੀ_delim,
		       stats->objects.disk_sizes.commits, ਮੁੱਲ_delim);
	print_keyvalue("objects.trees.disk_size", ਕੁੰਜੀ_delim,
		       stats->objects.disk_sizes.trees, ਮੁੱਲ_delim);
	print_keyvalue("objects.blobs.disk_size", ਕੁੰਜੀ_delim,
		       stats->objects.disk_sizes.blobs, ਮੁੱਲ_delim);
	print_keyvalue("objects.tags.disk_size", ਕੁੰਜੀ_delim,
		       stats->objects.disk_sizes.ਟੈਗ_ਸੂਚੀ, ਮੁੱਲ_delim);

	print_object_ਅੰਕੜੇ("objects.commits.max_size", ਕੁੰਜੀ_delim,
			  &stats->objects.ਸਭ_ਤੋਂ_ਵੱਡਾ.commit_ਮਾਪ, ਮੁੱਲ_delim);
	print_object_ਅੰਕੜੇ("objects.trees.max_size", ਕੁੰਜੀ_delim,
			  &stats->objects.ਸਭ_ਤੋਂ_ਵੱਡਾ.ਰੁੱਖ_ਮਾਪ, ਮੁੱਲ_delim);
	print_object_ਅੰਕੜੇ("objects.blobs.max_size", ਕੁੰਜੀ_delim,
			  &stats->objects.ਸਭ_ਤੋਂ_ਵੱਡਾ.blob_ਮਾਪ, ਮੁੱਲ_delim);
	print_object_ਅੰਕੜੇ("objects.tags.max_size", ਕੁੰਜੀ_delim,
			  &stats->objects.ਸਭ_ਤੋਂ_ਵੱਡਾ.ਟੈਗ_ਮਾਪ, ਮੁੱਲ_delim);

	print_object_ਅੰਕੜੇ("objects.commits.max_parents", ਕੁੰਜੀ_delim,
			  &stats->objects.ਸਭ_ਤੋਂ_ਵੱਡਾ.ਮਾਪਾ_ਗਿਣਤੀ, ਮੁੱਲ_delim);
	print_object_ਅੰਕੜੇ("objects.trees.max_entries", ਕੁੰਜੀ_delim,
			  &stats->objects.ਸਭ_ਤੋਂ_ਵੱਡਾ.ਰੁੱਖ_entries, ਮੁੱਲ_delim);

	fflush(stdout);
}

ਬਣਤਰ ਗਿਣਤੀ_references_ਅੰਕੜੇ {
	ਬਣਤਰ ref_stats *stats;
	ਬਣਤਰ rev_info *revs;
	ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ;
};

ਸਥਿਰ ਪੂਰਨ ਗਿਣਤੀ_references(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ ਗਿਣਤੀ_references_ਅੰਕੜੇ *ਅੰਕੜੇ = cb_ਅੰਕੜੇ;
	ਬਣਤਰ ref_stats *stats = ਅੰਕੜੇ->stats;
	ਮਾਪ_t ref_ਗਿਣਤੀ;

	ਚੋਣ (ref_ਕਿਸਮ_from_refname(ref->name)) {
	ਹਾਲਤ FILTER_REFS_BRANCHES:
		stats->branches++;
		ਰੋਕੋ;
	ਹਾਲਤ FILTER_REFS_REMOTES:
		stats->remotes++;
		ਰੋਕੋ;
	ਹਾਲਤ FILTER_REFS_ਟੈਗ_ਸੂਚੀ:
		stats->ਟੈਗ_ਸੂਚੀ++;
		ਰੋਕੋ;
	ਹਾਲਤ FILTER_REFS_OTHERS:
		stats->others++;
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	ਜੋੜੋ_pending_oid(ਅੰਕੜੇ->revs, ਨਿਰਾ, ref->oid, 0);

	ref_ਗਿਣਤੀ = get_ਕੁੱਲ_reference_ਗਿਣਤੀ(stats);
	display_ਤਰੱਕੀ(ਅੰਕੜੇ->ਤਰੱਕੀ, ref_ਗਿਣਤੀ);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ structure_ਗਿਣਤੀ_references(ਬਣਤਰ ref_stats *stats,
				       ਬਣਤਰ rev_info *revs,
				       ਬਣਤਰ repository *repo,
				       ਪੂਰਨ show_ਤਰੱਕੀ)
{
	ਬਣਤਰ ਗਿਣਤੀ_references_ਅੰਕੜੇ ਅੰਕੜੇ = {
		.stats = stats,
		.revs = revs,
	};

	ਜੇ (show_ਤਰੱਕੀ)
		ਅੰਕੜੇ.ਤਰੱਕੀ = ਸ਼ੁਰੂ_delayed_ਤਰੱਕੀ(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), ਗਿਣਤੀ_references, &ਅੰਕੜੇ);
	stop_ਤਰੱਕੀ(&ਅੰਕੜੇ.ਤਰੱਕੀ);
}

ਬਣਤਰ ਗਿਣਤੀ_objects_ਅੰਕੜੇ {
	ਬਣਤਰ object_database *odb;
	ਬਣਤਰ object_stats *stats;
	ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ;
};

ਸਥਿਰ ਖਾਲੀ check_ਸਭ_ਤੋਂ_ਵੱਡਾ(ਬਣਤਰ object_ਅੰਕੜੇ *ਅੰਕੜੇ, ਬਣਤਰ object_id *oid,
			  ਮਾਪ_t ਮੁੱਲ)
{
	ਜੇ (ਮੁੱਲ > ਅੰਕੜੇ->ਮੁੱਲ || is_null_oid(&ਅੰਕੜੇ->oid)) {
		oidcpy(&ਅੰਕੜੇ->oid, oid);
		ਅੰਕੜੇ->ਮੁੱਲ = ਮੁੱਲ;
	}
}

ਸਥਿਰ ਮਾਪ_t ਗਿਣਤੀ_ਰੁੱਖ_entries(ਬਣਤਰ object *obj)
{
	ਬਣਤਰ ਰੁੱਖ *t = object_as_type(obj, OBJ_ਰੁੱਖ, 0);
	ਬਣਤਰ name_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
	ਬਣਤਰ ਰੁੱਖ_desc desc;
	ਮਾਪ_t ਗਿਣਤੀ_2 = 0;

	init_ਰੁੱਖ_desc(&desc, &t->object.oid, t->ਬਫਰ, t->ਮਾਪ);
	ਜਦੋਂ (ਰੁੱਖ_ਇੰਦਰਾਜ(&desc, &ਇੰਦਰਾਜ))
		ਗਿਣਤੀ_2++;

	ਵਾਪਸ ਗਿਣਤੀ_2;
}

ਸਥਿਰ ਪੂਰਨ ਗਿਣਤੀ_objects(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ ਅਣਵਰਤਿਆ, ਬਣਤਰ oid_array *oids,
			 ਗਿਣਤੀ object_type type, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ ਗਿਣਤੀ_objects_ਅੰਕੜੇ *ਅੰਕੜੇ = cb_ਅੰਕੜੇ;
	ਬਣਤਰ object_stats *stats = ਅੰਕੜੇ->stats;
	ਮਾਪ_t object_ਗਿਣਤੀ;

	ਲਈ (ਮਾਪ_t i = 0; i < oids->nr; i++) {
		ਬਣਤਰ object_info oi = OBJECT_INFO_INIT;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ inflated;
		ਮਾਪ_t inflated_st = 0;
		ਬਣਤਰ commit *commit;
		ਬਣਤਰ object *obj;
		ਖਾਲੀ *content;
		off_t disk;
		ਪੂਰਨ eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		ਜੇ (odb_read_object_info_extended(ਅੰਕੜੇ->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_ਤੇਜ਼) < 0)
			ਜਾਰੀ;
		inflated = cast_ਮਾਪ_t_ਮੰਜ਼ਿਲ_ulong(inflated_st);

		obj = parse_object_ਬਫਰ(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		ਚੋਣ (type) {
		ਹਾਲਤ OBJ_ਟੈਗ:
			stats->type_counts.ਟੈਗ_ਸੂਚੀ++;
			stats->inflated_sizes.ਟੈਗ_ਸੂਚੀ += inflated;
			stats->disk_sizes.ਟੈਗ_ਸੂਚੀ += disk;
			check_ਸਭ_ਤੋਂ_ਵੱਡਾ(&stats->ਸਭ_ਤੋਂ_ਵੱਡਾ.ਟੈਗ_ਮਾਪ, &oids->oid[i],
				      inflated);
			ਰੋਕੋ;
		ਹਾਲਤ OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_ਸਭ_ਤੋਂ_ਵੱਡਾ(&stats->ਸਭ_ਤੋਂ_ਵੱਡਾ.commit_ਮਾਪ, &oids->oid[i],
				      inflated);
			check_ਸਭ_ਤੋਂ_ਵੱਡਾ(&stats->ਸਭ_ਤੋਂ_ਵੱਡਾ.ਮਾਪਾ_ਗਿਣਤੀ, &oids->oid[i],
				      commit_list_ਗਿਣਤੀ(commit->parents));
			ਰੋਕੋ;
		ਹਾਲਤ OBJ_ਰੁੱਖ:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_ਸਭ_ਤੋਂ_ਵੱਡਾ(&stats->ਸਭ_ਤੋਂ_ਵੱਡਾ.ਰੁੱਖ_ਮਾਪ, &oids->oid[i],
				      inflated);
			check_ਸਭ_ਤੋਂ_ਵੱਡਾ(&stats->ਸਭ_ਤੋਂ_ਵੱਡਾ.ਰੁੱਖ_entries, &oids->oid[i],
				      ਗਿਣਤੀ_ਰੁੱਖ_entries(obj));
			ਰੋਕੋ;
		ਹਾਲਤ OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_ਸਭ_ਤੋਂ_ਵੱਡਾ(&stats->ਸਭ_ਤੋਂ_ਵੱਡਾ.blob_ਮਾਪ, &oids->oid[i],
				      inflated);
			ਰੋਕੋ;
		ਮੂਲ:
			BUG("invalid object type");
		}

		ਜੇ (!eaten)
			ਮੁਕਤ(content);
	}

	object_ਗਿਣਤੀ = get_ਕੁੱਲ_object_ਮੁੱਲਾਂ(&stats->type_counts);
	display_ਤਰੱਕੀ(ਅੰਕੜੇ->ਤਰੱਕੀ, object_ਗਿਣਤੀ);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ structure_ਗਿਣਤੀ_objects(ਬਣਤਰ object_stats *stats,
				    ਬਣਤਰ rev_info *revs,
				    ਬਣਤਰ repository *repo, ਪੂਰਨ show_ਤਰੱਕੀ)
{
	ਬਣਤਰ ਰਸਤਾ_walk_info info = ਰਸਤਾ_WALK_INFO_INIT;
	ਬਣਤਰ ਗਿਣਤੀ_objects_ਅੰਕੜੇ ਅੰਕੜੇ = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.ਰਸਤਾ_fn = ਗਿਣਤੀ_objects;
	info.ਰਸਤਾ_fn_ਅੰਕੜੇ = &ਅੰਕੜੇ;

	ਜੇ (show_ਤਰੱਕੀ)
		ਅੰਕੜੇ.ਤਰੱਕੀ = ਸ਼ੁਰੂ_delayed_ਤਰੱਕੀ(repo, _("Counting objects"), 0);

	walk_objects_by_ਰਸਤਾ(&info);
	ਰਸਤਾ_walk_info_clear(&info);
	stop_ਤਰੱਕੀ(&ਅੰਕੜੇ.ਤਰੱਕੀ);
}

ਸਥਿਰ ਪੂਰਨ cmd_repo_structure(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			      ਬਣਤਰ repository *repo)
{
	ਬਣਤਰ stats_ਸਾਰਣੀ ਸਾਰਣੀ = {
		.ਪੰਕਤੀਆਂ = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	ਗਿਣਤੀ output_format format = FORMAT_ਸਾਰਣੀ;
	ਬਣਤਰ repo_structure stats = { 0 };
	ਬਣਤਰ rev_info revs;
	ਪੂਰਨ show_ਤਰੱਕੀ = -1;
	ਬਣਤਰ option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ਨਿਰਾ, &format, ਨਿਰਾ,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_ਤਰੱਕੀ, N_("show progress")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, repo_structure_usage, 0);
	ਜੇ (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, ਅਗੇਤਰ);

	ਜੇ (show_ਤਰੱਕੀ < 0)
		show_ਤਰੱਕੀ = isatty(2);

	structure_ਗਿਣਤੀ_references(&stats.refs, &revs, repo, show_ਤਰੱਕੀ);
	structure_ਗਿਣਤੀ_objects(&stats.objects, &revs, repo, show_ਤਰੱਕੀ);

	ਚੋਣ (format) {
	ਹਾਲਤ FORMAT_ਸਾਰਣੀ:
		stats_ਸਾਰਣੀ_ਤਿਆਰੀ_structure(&ਸਾਰਣੀ, &stats);
		stats_ਸਾਰਣੀ_print_structure(&ਸਾਰਣੀ);
		ਰੋਕੋ;
	ਹਾਲਤ FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		ਰੋਕੋ;
	ਹਾਲਤ FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("invalid output format");
	}

	stats_ਸਾਰਣੀ_clear(&ਸਾਰਣੀ);
	release_revisions(&revs);

	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_repo(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	     ਬਣਤਰ repository *repo)
{
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, repo_usage, 0);

	ਵਾਪਸ fn(argc, argv, ਅਗੇਤਰ, repo);
}
