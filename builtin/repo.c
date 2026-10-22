#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "path-walk.h"
#అన్తర్భావ "progress.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "ref-filter.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "shallow.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "tree-walk.h"
#అన్తర్భావ "utf8.h"

#నిర్వచించు REPO_INFO_USAGE \
	"git repo info [--format=(lines|nul) | -z] [--all | <key>...]", \
	"git repo info --keys [--format=(lines|nul) | -z]"

#నిర్వచించు REPO_STRUCTURE_USAGE \
	"git repo structure [--format=(table|lines|nul) | -z]"

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి repo_usage[] = {
	REPO_INFO_USAGE,
	REPO_STRUCTURE_USAGE,
	ఖాళీ,
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి repo_info_usage[] = {
	REPO_INFO_USAGE,
	ఖాళీ,
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి repo_structure_usage[] = {
	REPO_STRUCTURE_USAGE,
	ఖాళీ,
};

రకనిర్వచనం పూర్ణాంకం get_విలువ_fn(నిర్మాణం repository *repo, నిర్మాణం strbuf *buf);

గణన output_format {
	FORMAT_పట్టిక,
	FORMAT_NEWLINE_TERMINATED,
	FORMAT_NUL_TERMINATED,
};

నిర్మాణం repo_info_క్షేత్రం {
	స్థిరరాశి అక్షరం *కీలకం;
	get_విలువ_fn *get_విలువ;
};

స్థిర పూర్ణాంకం get_layout_bare(నిర్మాణం repository *repo వాడని, నిర్మాణం strbuf *buf)
{
	strbuf_addstr(buf, is_bare_repository(the_repository) ? "true" : "false");
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_layout_shallow(నిర్మాణం repository *repo, నిర్మాణం strbuf *buf)
{
	strbuf_addstr(buf,
		      is_repository_shallow(repo) ? "true" : "false");
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_object_format(నిర్మాణం repository *repo, నిర్మాణం strbuf *buf)
{
	strbuf_addstr(buf, repo->hash_algo->name);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_మార్గం_commondir_absolute(నిర్మాణం repository *repo, నిర్మాణం strbuf *buf)
{
	స్థిరరాశి అక్షరం *common_dir = repo_get_common_dir(repo);

	ఒకవేళ (!common_dir)
		తిరిగి error(_("unable to get common directory"));

	format_మార్గం(buf, common_dir, repo->ఉపసర్గ, మార్గం_FORMAT_CANONICAL);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_మార్గం_commondir_relative(నిర్మాణం repository *repo, నిర్మాణం strbuf *buf)
{
	స్థిరరాశి అక్షరం *common_dir = repo_get_common_dir(repo);

	ఒకవేళ (!common_dir)
		తిరిగి error(_("unable to get common directory"));

	format_మార్గం(buf, common_dir, repo->ఉపసర్గ, మార్గం_FORMAT_RELATIVE);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_మార్గం_gitdir_absolute(నిర్మాణం repository *repo, నిర్మాణం strbuf *buf)
{
	స్థిరరాశి అక్షరం *git_dir = repo_get_git_dir(repo);

	ఒకవేళ (!git_dir)
		తిరిగి error(_("unable to get git directory"));

	format_మార్గం(buf, git_dir, repo->ఉపసర్గ, మార్గం_FORMAT_CANONICAL);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_మార్గం_gitdir_relative(నిర్మాణం repository *repo, నిర్మాణం strbuf *buf)
{
	స్థిరరాశి అక్షరం *git_dir = repo_get_git_dir(repo);

	ఒకవేళ (!git_dir)
		తిరిగి error(_("unable to get git directory"));

	format_మార్గం(buf, git_dir, repo->ఉపసర్గ, మార్గం_FORMAT_RELATIVE);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_references_format(నిర్మాణం repository *repo, నిర్మాణం strbuf *buf)
{
	strbuf_addstr(buf,
		      ref_storage_format_గమ్యం_name(repo->ref_storage_format));
	తిరిగి 0;
}

/* repo_info_field keys must be in lexicographical order */
స్థిర స్థిరరాశి నిర్మాణం repo_info_క్షేత్రం repo_info_క్షేత్రం[] = {
	{ "layout.bare", get_layout_bare },
	{ "layout.shallow", get_layout_shallow },
	{ "object.format", get_object_format },
	{ "path.commondir.absolute", get_మార్గం_commondir_absolute },
	{ "path.commondir.relative", get_మార్గం_commondir_relative },
	{ "path.gitdir.absolute", get_మార్గం_gitdir_absolute },
	{ "path.gitdir.relative", get_మార్గం_gitdir_relative },
	{ "references.format", get_references_format },
};

స్థిర పూర్ణాంకం repo_info_క్షేత్రం_cmp(స్థిరరాశి శూన్య *va, స్థిరరాశి శూన్య *vb)
{
	స్థిరరాశి నిర్మాణం repo_info_క్షేత్రం *a = va;
	స్థిరరాశి నిర్మాణం repo_info_క్షేత్రం *b = vb;

	తిరిగి strcmp(a->కీలకం, b->కీలకం);
}

స్థిర స్థిరరాశి నిర్మాణం repo_info_క్షేత్రం *get_repo_info_క్షేత్రం(స్థిరరాశి అక్షరం *కీలకం)
{
	స్థిరరాశి నిర్మాణం repo_info_క్షేత్రం శోధన_కీలకం = { కీలకం, ఖాళీ };
	స్థిరరాశి నిర్మాణం repo_info_క్షేత్రం *దొరికింది_2 = bsearch(&శోధన_కీలకం,
						      repo_info_క్షేత్రం,
						      ARRAY_కొలత(repo_info_క్షేత్రం),
						      పరిమాణం(*దొరికింది_2),
						      repo_info_క్షేత్రం_cmp);

	తిరిగి దొరికింది_2;
}

స్థిర శూన్య print_క్షేత్రం(గణన output_format format, స్థిరరాశి అక్షరం *కీలకం,
			స్థిరరాశి అక్షరం *విలువ)
{
	ఎంపిక (format) {
	స్థితి FORMAT_NEWLINE_TERMINATED:
		రాయి("%s=", కీలకం);
		quote_c_style(విలువ, ఖాళీ, stdout, 0);
		అక్షరం_రాయి('\n');
		ఆపు;
	స్థితి FORMAT_NUL_TERMINATED:
		రాయి("%s\n%s%c", కీలకం, విలువ, '\0');
		ఆపు;
	అప్రమేయ:
		BUG("not a valid output format: %d", format);
	}
}

స్థిర పూర్ణాంకం print_fields(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
			నిర్మాణం repository *repo,
			గణన output_format format)
{
	పూర్ణాంకం ret = 0;
	నిర్మాణం strbuf valbuf = STRBUF_INIT;

	కొరకు (పూర్ణాంకం i = 0; i < argc; i++) {
		స్థిరరాశి అక్షరం *కీలకం = argv[i];
		స్థిరరాశి నిర్మాణం repo_info_క్షేత్రం *క్షేత్రం = get_repo_info_క్షేత్రం(కీలకం);

		ఒకవేళ (!క్షేత్రం) {
			ret = error(_("key '%s' not found"), కీలకం);
			కొనసాగు;
		}

		strbuf_reset(&valbuf);
		క్షేత్రం->get_విలువ(repo, &valbuf);
		print_క్షేత్రం(format, కీలకం, valbuf.buf);
	}

	strbuf_release(&valbuf);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం print_all_fields(నిర్మాణం repository *repo,
			    గణన output_format format)
{
	నిర్మాణం strbuf valbuf = STRBUF_INIT;

	కొరకు (కొలత_t i = 0; i < ARRAY_కొలత(repo_info_క్షేత్రం); i++) {
		స్థిరరాశి నిర్మాణం repo_info_క్షేత్రం *క్షేత్రం = &repo_info_క్షేత్రం[i];

		strbuf_reset(&valbuf);
		క్షేత్రం->get_విలువ(repo, &valbuf);
		print_క్షేత్రం(format, క్షేత్రం->కీలకం, valbuf.buf);
	}

	strbuf_release(&valbuf);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం print_కీలకాలు(గణన output_format format)
{
	అక్షరం sep;

	ఎంపిక (format) {
	స్థితి FORMAT_NEWLINE_TERMINATED:
		sep = '\n';
		ఆపు;
	స్థితి FORMAT_NUL_TERMINATED:
		sep = '\0';
		ఆపు;
	అప్రమేయ:
		die(_("--keys can only be used with --format=lines or --format=nul"));
	}

	కొరకు (కొలత_t i = 0; i < ARRAY_కొలత(repo_info_క్షేత్రం); i++) {
		స్థిరరాశి నిర్మాణం repo_info_క్షేత్రం *క్షేత్రం = &repo_info_క్షేత్రం[i];
		రాయి("%s%c", క్షేత్రం->కీలకం, sep);
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_format_cb(స్థిరరాశి నిర్మాణం option *opt,
			   స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset వాడని)
{
	గణన output_format *format = opt->విలువ;

	ఒకవేళ (opt->short_name == 'z')
		*format = FORMAT_NUL_TERMINATED;
	లేకపోతే ఒకవేళ (!strcmp(arg, "nul"))
		*format = FORMAT_NUL_TERMINATED;
	లేకపోతే ఒకవేళ (!strcmp(arg, "lines"))
		*format = FORMAT_NEWLINE_TERMINATED;
	లేకపోతే ఒకవేళ (!strcmp(arg, "table"))
		*format = FORMAT_పట్టిక;
	లేకపోతే
		die(_("invalid format '%s'"), arg);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం cmd_repo_info(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			 నిర్మాణం repository *repo)
{
	గణన output_format format = FORMAT_NEWLINE_TERMINATED;
	పూర్ణాంకం all_కీలకాలు = 0;
	పూర్ణాంకం show_కీలకాలు = 0;
	నిర్మాణం option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ఖాళీ, &format, ఖాళీ,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "all", &all_కీలకాలు, N_("print all keys/values")),
		OPT_BOOL(0, "keys", &show_కీలకాలు, N_("show keys")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, repo_info_usage, 0);

	ఒకవేళ (show_కీలకాలు && (all_కీలకాలు || argc))
		die(_("--keys cannot be used with a <key> or --all"));

	ఒకవేళ (show_కీలకాలు)
		తిరిగి print_కీలకాలు(format);

	ఒకవేళ (format != FORMAT_NEWLINE_TERMINATED && format != FORMAT_NUL_TERMINATED)
		die(_("unsupported output format"));

	ఒకవేళ (all_కీలకాలు && argc)
		die(_("--all and <key> cannot be used together"));

	ఒకవేళ (all_కీలకాలు)
		తిరిగి print_all_fields(repo, format);
	లేకపోతే
		తిరిగి print_fields(argc, argv, repo, format);
}

నిర్మాణం object_దత్తాంశం {
	నిర్మాణం object_id oid;
	కొలత_t విలువ;
};

నిర్మాణం అతిపెద్ద_objects {
	నిర్మాణం object_దత్తాంశం ట్యాగ్_కొలత;
	నిర్మాణం object_దత్తాంశం commit_కొలత;
	నిర్మాణం object_దత్తాంశం వృక్షం_కొలత;
	నిర్మాణం object_దత్తాంశం blob_కొలత;

	నిర్మాణం object_దత్తాంశం మాతృ_లెక్క;
	నిర్మాణం object_దత్తాంశం వృక్షం_entries;
};

నిర్మాణం ref_stats {
	కొలత_t branches;
	కొలత_t remotes;
	కొలత_t ట్యాగులు;
	కొలత_t others;
};

నిర్మాణం object_విలువలు {
	కొలత_t ట్యాగులు;
	కొలత_t commits;
	కొలత_t trees;
	కొలత_t blobs;
};

నిర్మాణం object_stats {
	నిర్మాణం object_విలువలు type_counts;
	నిర్మాణం object_విలువలు inflated_sizes;
	నిర్మాణం object_విలువలు disk_sizes;
	నిర్మాణం అతిపెద్ద_objects అతిపెద్ద;
};

నిర్మాణం repo_structure {
	నిర్మాణం ref_stats refs;
	నిర్మాణం object_stats objects;
};

నిర్మాణం stats_పట్టిక {
	నిర్మాణం string_list అడ్డువరుసలు;
	నిర్మాణం string_list annotations;

	పూర్ణాంకం name_నిలువు_వెడల్పు;
	పూర్ణాంకం విలువ_నిలువు_వెడల్పు;
	పూర్ణాంకం unit_నిలువు_వెడల్పు;
};

/*
 * Holds column data that gets stored for each row.
 */
నిర్మాణం stats_పట్టిక_నమోదు {
	అక్షరం *విలువ;
	స్థిరరాశి అక్షరం *unit;
	కొలత_t సూచీ;
	నిర్మాణం object_id *oid;
};

స్థిర శూన్య stats_పట్టిక_vaddf(నిర్మాణం stats_పట్టిక *పట్టిక,
			      నిర్మాణం stats_పట్టిక_నమోదు *నమోదు_2,
			      స్థిరరాశి అక్షరం *format, va_list ap)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం string_list_అంశం *అంశం;
	అక్షరం *formatted_name;
	పూర్ణాంకం name_వెడల్పు;

	strbuf_vaddf(&buf, format, ap);
	formatted_name = strbuf_detach(&buf, ఖాళీ);
	name_వెడల్పు = utf8_strwidth(formatted_name);

	అంశం = string_list_append_nodup(&పట్టిక->అడ్డువరుసలు, formatted_name);
	అంశం->util = నమోదు_2;

	ఒకవేళ (name_వెడల్పు > పట్టిక->name_నిలువు_వెడల్పు)
		పట్టిక->name_నిలువు_వెడల్పు = name_వెడల్పు;
	ఒకవేళ (!నమోదు_2)
		తిరిగి;
	ఒకవేళ (నమోదు_2->oid) {
		నమోదు_2->సూచీ = పట్టిక->annotations.nr + 1;
		strbuf_addf(&buf, "[%" PRIuMAX "] %s", (uintmax_t)నమోదు_2->సూచీ,
			    oid_గమ్యం_hex(నమోదు_2->oid));
		string_list_append_nodup(&పట్టిక->annotations, strbuf_detach(&buf, ఖాళీ));
	}
	ఒకవేళ (నమోదు_2->విలువ) {
		పూర్ణాంకం విలువ_వెడల్పు = utf8_strwidth(నమోదు_2->విలువ);
		ఒకవేళ (విలువ_వెడల్పు > పట్టిక->విలువ_నిలువు_వెడల్పు)
			పట్టిక->విలువ_నిలువు_వెడల్పు = విలువ_వెడల్పు;
	}
	ఒకవేళ (నమోదు_2->unit) {
		పూర్ణాంకం unit_వెడల్పు = utf8_strwidth(నమోదు_2->unit);
		ఒకవేళ (unit_వెడల్పు > పట్టిక->unit_నిలువు_వెడల్పు)
			పట్టిక->unit_నిలువు_వెడల్పు = unit_వెడల్పు;
	}

	strbuf_release(&buf);
}

స్థిర శూన్య stats_పట్టిక_addf(నిర్మాణం stats_పట్టిక *పట్టిక, స్థిరరాశి అక్షరం *format, ...)
{
	va_list ap;

	va_ప్రారంభం(ap, format);
	stats_పట్టిక_vaddf(పట్టిక, ఖాళీ, format, ap);
	va_చివర(ap);
}

స్థిర శూన్య stats_పట్టిక_లెక్క_addf(నిర్మాణం stats_పట్టిక *పట్టిక, కొలత_t విలువ,
				   స్థిరరాశి అక్షరం *format, ...)
{
	నిర్మాణం stats_పట్టిక_నమోదు *నమోదు_2;
	va_list ap;

	CALLOC_ARRAY(నమోదు_2, 1);
	humanise_లెక్క(విలువ, &నమోదు_2->విలువ, &నమోదు_2->unit);

	va_ప్రారంభం(ap, format);
	stats_పట్టిక_vaddf(పట్టిక, నమోదు_2, format, ap);
	va_చివర(ap);
}

స్థిర శూన్య stats_పట్టిక_object_లెక్క_addf(నిర్మాణం stats_పట్టిక *పట్టిక,
					  నిర్మాణం object_id *oid, కొలత_t విలువ,
					  స్థిరరాశి అక్షరం *format, ...)
{
	నిర్మాణం stats_పట్టిక_నమోదు *నమోదు_2;
	va_list ap;

	CALLOC_ARRAY(నమోదు_2, 1);
	humanise_లెక్క(విలువ, &నమోదు_2->విలువ, &నమోదు_2->unit);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	ఒకవేళ (!is_null_oid(oid))
		నమోదు_2->oid = oid;

	va_ప్రారంభం(ap, format);
	stats_పట్టిక_vaddf(పట్టిక, నమోదు_2, format, ap);
	va_చివర(ap);
}

స్థిర శూన్య stats_పట్టిక_కొలత_addf(నిర్మాణం stats_పట్టిక *పట్టిక, కొలత_t విలువ,
				  స్థిరరాశి అక్షరం *format, ...)
{
	నిర్మాణం stats_పట్టిక_నమోదు *నమోదు_2;
	va_list ap;

	CALLOC_ARRAY(నమోదు_2, 1);
	humanise_బైట్లు(విలువ, &నమోదు_2->విలువ, &నమోదు_2->unit, HUMANISE_COMPACT);

	va_ప్రారంభం(ap, format);
	stats_పట్టిక_vaddf(పట్టిక, నమోదు_2, format, ap);
	va_చివర(ap);
}

స్థిర శూన్య stats_పట్టిక_object_కొలత_addf(నిర్మాణం stats_పట్టిక *పట్టిక,
					 నిర్మాణం object_id *oid, కొలత_t విలువ,
					 స్థిరరాశి అక్షరం *format, ...)
{
	నిర్మాణం stats_పట్టిక_నమోదు *నమోదు_2;
	va_list ap;

	CALLOC_ARRAY(నమోదు_2, 1);
	humanise_బైట్లు(విలువ, &నమోదు_2->విలువ, &నమోదు_2->unit, HUMANISE_COMPACT);

	/*
	 * A NULL OID should not have a table annotation.
	 */
	ఒకవేళ (!is_null_oid(oid))
		నమోదు_2->oid = oid;

	va_ప్రారంభం(ap, format);
	stats_పట్టిక_vaddf(పట్టిక, నమోదు_2, format, ap);
	va_చివర(ap);
}

స్థిర అంతఃవరుస కొలత_t get_మొత్తం_reference_లెక్క(నిర్మాణం ref_stats *stats)
{
	తిరిగి stats->branches + stats->remotes + stats->ట్యాగులు + stats->others;
}

స్థిర అంతఃవరుస కొలత_t get_మొత్తం_object_విలువలు(నిర్మాణం object_విలువలు *విలువలు)
{
	తిరిగి విలువలు->ట్యాగులు + విలువలు->commits + విలువలు->trees + విలువలు->blobs;
}

స్థిర శూన్య stats_పట్టిక_అమరిక_structure(నిర్మాణం stats_పట్టిక *పట్టిక,
					నిర్మాణం repo_structure *stats)
{
	నిర్మాణం object_stats *objects = &stats->objects;
	నిర్మాణం ref_stats *refs = &stats->refs;
	కొలత_t inflated_object_మొత్తం;
	కొలత_t object_లెక్క_మొత్తం;
	కొలత_t disk_object_మొత్తం;
	కొలత_t ref_మొత్తం;

	ref_మొత్తం = get_మొత్తం_reference_లెక్క(refs);
	stats_పట్టిక_addf(పట్టిక, "* %s", _("References"));
	stats_పట్టిక_లెక్క_addf(పట్టిక, ref_మొత్తం, "  * %s", _("Count"));
	stats_పట్టిక_లెక్క_addf(పట్టిక, refs->branches, "    * %s", _("Branches"));
	stats_పట్టిక_లెక్క_addf(పట్టిక, refs->ట్యాగులు, "    * %s", _("Tags"));
	stats_పట్టిక_లెక్క_addf(పట్టిక, refs->remotes, "    * %s", _("Remotes"));
	stats_పట్టిక_లెక్క_addf(పట్టిక, refs->others, "    * %s", _("Others"));

	object_లెక్క_మొత్తం = get_మొత్తం_object_విలువలు(&objects->type_counts);
	stats_పట్టిక_addf(పట్టిక, "");
	stats_పట్టిక_addf(పట్టిక, "* %s", _("Reachable objects"));
	stats_పట్టిక_లెక్క_addf(పట్టిక, object_లెక్క_మొత్తం, "  * %s", _("Count"));
	stats_పట్టిక_లెక్క_addf(పట్టిక, objects->type_counts.commits,
			       "    * %s", _("Commits"));
	stats_పట్టిక_లెక్క_addf(పట్టిక, objects->type_counts.trees,
			       "    * %s", _("Trees"));
	stats_పట్టిక_లెక్క_addf(పట్టిక, objects->type_counts.blobs,
			       "    * %s", _("Blobs"));
	stats_పట్టిక_లెక్క_addf(పట్టిక, objects->type_counts.ట్యాగులు,
			       "    * %s", _("Tags"));

	inflated_object_మొత్తం = get_మొత్తం_object_విలువలు(&objects->inflated_sizes);
	stats_పట్టిక_కొలత_addf(పట్టిక, inflated_object_మొత్తం,
			      "  * %s", _("Inflated size"));
	stats_పట్టిక_కొలత_addf(పట్టిక, objects->inflated_sizes.commits,
			      "    * %s", _("Commits"));
	stats_పట్టిక_కొలత_addf(పట్టిక, objects->inflated_sizes.trees,
			      "    * %s", _("Trees"));
	stats_పట్టిక_కొలత_addf(పట్టిక, objects->inflated_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_పట్టిక_కొలత_addf(పట్టిక, objects->inflated_sizes.ట్యాగులు,
			      "    * %s", _("Tags"));

	disk_object_మొత్తం = get_మొత్తం_object_విలువలు(&objects->disk_sizes);
	stats_పట్టిక_కొలత_addf(పట్టిక, disk_object_మొత్తం,
			      "  * %s", _("Disk size"));
	stats_పట్టిక_కొలత_addf(పట్టిక, objects->disk_sizes.commits,
			      "    * %s", _("Commits"));
	stats_పట్టిక_కొలత_addf(పట్టిక, objects->disk_sizes.trees,
			      "    * %s", _("Trees"));
	stats_పట్టిక_కొలత_addf(పట్టిక, objects->disk_sizes.blobs,
			      "    * %s", _("Blobs"));
	stats_పట్టిక_కొలత_addf(పట్టిక, objects->disk_sizes.ట్యాగులు,
			      "    * %s", _("Tags"));

	stats_పట్టిక_addf(పట్టిక, "");
	stats_పట్టిక_addf(పట్టిక, "* %s", _("Largest objects"));
	stats_పట్టిక_addf(పట్టిక, "  * %s", _("Commits"));
	stats_పట్టిక_object_కొలత_addf(పట్టిక,
				     &objects->అతిపెద్ద.commit_కొలత.oid,
				     objects->అతిపెద్ద.commit_కొలత.విలువ,
				     "    * %s", _("Maximum size"));
	stats_పట్టిక_object_లెక్క_addf(పట్టిక,
				      &objects->అతిపెద్ద.మాతృ_లెక్క.oid,
				      objects->అతిపెద్ద.మాతృ_లెక్క.విలువ,
				      "    * %s", _("Maximum parents"));
	stats_పట్టిక_addf(పట్టిక, "  * %s", _("Trees"));
	stats_పట్టిక_object_కొలత_addf(పట్టిక,
				     &objects->అతిపెద్ద.వృక్షం_కొలత.oid,
				     objects->అతిపెద్ద.వృక్షం_కొలత.విలువ,
				     "    * %s", _("Maximum size"));
	stats_పట్టిక_object_లెక్క_addf(పట్టిక,
				      &objects->అతిపెద్ద.వృక్షం_entries.oid,
				      objects->అతిపెద్ద.వృక్షం_entries.విలువ,
				      "    * %s", _("Maximum entries"));
	stats_పట్టిక_addf(పట్టిక, "  * %s", _("Blobs"));
	stats_పట్టిక_object_కొలత_addf(పట్టిక,
				     &objects->అతిపెద్ద.blob_కొలత.oid,
				     objects->అతిపెద్ద.blob_కొలత.విలువ,
				     "    * %s", _("Maximum size"));
	stats_పట్టిక_addf(పట్టిక, "  * %s", _("Tags"));
	stats_పట్టిక_object_కొలత_addf(పట్టిక,
				     &objects->అతిపెద్ద.ట్యాగ్_కొలత.oid,
				     objects->అతిపెద్ద.ట్యాగ్_కొలత.విలువ,
				     "    * %s", _("Maximum size"));
}

#నిర్వచించు సూచీ_వెడల్పు 4

స్థిర శూన్య stats_పట్టిక_print_structure(స్థిరరాశి నిర్మాణం stats_పట్టిక *పట్టిక)
{
	స్థిరరాశి అక్షరం *name_నిలువు_title = _("Repository structure");
	స్థిరరాశి అక్షరం *విలువ_నిలువు_title = _("Value");
	పూర్ణాంకం title_name_వెడల్పు = utf8_strwidth(name_నిలువు_title);
	పూర్ణాంకం title_విలువ_వెడల్పు = utf8_strwidth(విలువ_నిలువు_title);
	పూర్ణాంకం name_నిలువు_వెడల్పు = పట్టిక->name_నిలువు_వెడల్పు;
	పూర్ణాంకం విలువ_నిలువు_వెడల్పు = పట్టిక->విలువ_నిలువు_వెడల్పు;
	పూర్ణాంకం unit_నిలువు_వెడల్పు = పట్టిక->unit_నిలువు_వెడల్పు;
	నిర్మాణం string_list_అంశం *అంశం;
	నిర్మాణం strbuf buf = STRBUF_INIT;

	ఒకవేళ (title_name_వెడల్పు > name_నిలువు_వెడల్పు)
		name_నిలువు_వెడల్పు = title_name_వెడల్పు;
	ఒకవేళ (title_విలువ_వెడల్పు > విలువ_నిలువు_వెడల్పు + unit_నిలువు_వెడల్పు + 1)
		విలువ_నిలువు_వెడల్పు = title_విలువ_వెడల్పు - unit_నిలువు_వెడల్పు;

	strbuf_addstr(&buf, "| ");
	strbuf_utf8_align(&buf, ALIGN_ఎడమ, name_నిలువు_వెడల్పు + సూచీ_వెడల్పు,
			  name_నిలువు_title);
	strbuf_addstr(&buf, " | ");
	strbuf_utf8_align(&buf, ALIGN_ఎడమ,
			  విలువ_నిలువు_వెడల్పు + unit_నిలువు_వెడల్పు + 1, విలువ_నిలువు_title);
	strbuf_addstr(&buf, " |");
	రాయి("%s\n", buf.buf);

	రాయి("| ");
	కొరకు (పూర్ణాంకం i = 0; i < name_నిలువు_వెడల్పు + సూచీ_వెడల్పు; i++)
		అక్షరం_రాయి('-');
	రాయి(" | ");
	కొరకు (పూర్ణాంకం i = 0; i < విలువ_నిలువు_వెడల్పు + unit_నిలువు_వెడల్పు + 1; i++)
		అక్షరం_రాయి('-');
	రాయి(" |\n");

	for_each_string_list_అంశం(అంశం, &పట్టిక->అడ్డువరుసలు) {
		నిర్మాణం stats_పట్టిక_నమోదు *నమోదు_2 = అంశం->util;
		స్థిరరాశి అక్షరం *విలువ = "";
		స్థిరరాశి అక్షరం *unit = "";

		ఒకవేళ (నమోదు_2) {
			విలువ = నమోదు_2->విలువ;
			ఒకవేళ (నమోదు_2->unit)
				unit = నమోదు_2->unit;
		}

		strbuf_reset(&buf);
		strbuf_addstr(&buf, "| ");
		strbuf_utf8_align(&buf, ALIGN_ఎడమ, name_నిలువు_వెడల్పు, అంశం->string);

		ఒకవేళ (నమోదు_2 && నమోదు_2->oid)
			strbuf_addf(&buf, " [%" PRIuMAX "]",
				    (uintmax_t)నమోదు_2->సూచీ);
		లేకపోతే
			strbuf_addchars(&buf, ' ', సూచీ_వెడల్పు);

		strbuf_addstr(&buf, " | ");
		strbuf_utf8_align(&buf, ALIGN_కుడి, విలువ_నిలువు_వెడల్పు, విలువ);
		strbuf_addch(&buf, ' ');
		strbuf_utf8_align(&buf, ALIGN_ఎడమ, unit_నిలువు_వెడల్పు, unit);
		strbuf_addstr(&buf, " |");
		రాయి("%s\n", buf.buf);
	}

	ఒకవేళ (పట్టిక->annotations.nr) {
		రాయి("\n");
		for_each_string_list_అంశం(అంశం, &పట్టిక->annotations)
			రాయి("%s\n", అంశం->string);
	}

	strbuf_release(&buf);
}

స్థిర శూన్య stats_పట్టిక_clear(నిర్మాణం stats_పట్టిక *పట్టిక)
{
	నిర్మాణం stats_పట్టిక_నమోదు *నమోదు_2;
	నిర్మాణం string_list_అంశం *అంశం;

	for_each_string_list_అంశం(అంశం, &పట్టిక->అడ్డువరుసలు) {
		నమోదు_2 = అంశం->util;
		ఒకవేళ (నమోదు_2)
			విడుదల(నమోదు_2->విలువ);
	}

	string_list_clear(&పట్టిక->అడ్డువరుసలు, 1);
	string_list_clear(&పట్టిక->annotations, 1);
}

స్థిర అంతఃవరుస శూన్య print_keyvalue(స్థిరరాశి అక్షరం *కీలకం, అక్షరం కీలకం_delim, కొలత_t విలువ,
				  అక్షరం విలువ_delim)
{
	రాయి("%s%c%" PRIuMAX "%c", కీలకం, కీలకం_delim, (uintmax_t)విలువ,
	       విలువ_delim);
}

స్థిర శూన్య print_object_దత్తాంశం(స్థిరరాశి అక్షరం *కీలకం, అక్షరం కీలకం_delim,
			      నిర్మాణం object_దత్తాంశం *దత్తాంశం, అక్షరం విలువ_delim)
{
	print_keyvalue(కీలకం, కీలకం_delim, దత్తాంశం->విలువ, విలువ_delim);
	రాయి("%s_oid%c%s%c", కీలకం, కీలకం_delim, oid_గమ్యం_hex(&దత్తాంశం->oid),
	       విలువ_delim);
}

స్థిర శూన్య structure_keyvalue_print(నిర్మాణం repo_structure *stats,
				     అక్షరం కీలకం_delim, అక్షరం విలువ_delim)
{
	print_keyvalue("references.branches.count", కీలకం_delim,
		       stats->refs.branches, విలువ_delim);
	print_keyvalue("references.tags.count", కీలకం_delim,
		       stats->refs.ట్యాగులు, విలువ_delim);
	print_keyvalue("references.remotes.count", కీలకం_delim,
		       stats->refs.remotes, విలువ_delim);
	print_keyvalue("references.others.count", కీలకం_delim,
		       stats->refs.others, విలువ_delim);

	print_keyvalue("objects.commits.count", కీలకం_delim,
		       stats->objects.type_counts.commits, విలువ_delim);
	print_keyvalue("objects.trees.count", కీలకం_delim,
		       stats->objects.type_counts.trees, విలువ_delim);
	print_keyvalue("objects.blobs.count", కీలకం_delim,
		       stats->objects.type_counts.blobs, విలువ_delim);
	print_keyvalue("objects.tags.count", కీలకం_delim,
		       stats->objects.type_counts.ట్యాగులు, విలువ_delim);

	print_keyvalue("objects.commits.inflated_size", కీలకం_delim,
		       stats->objects.inflated_sizes.commits, విలువ_delim);
	print_keyvalue("objects.trees.inflated_size", కీలకం_delim,
		       stats->objects.inflated_sizes.trees, విలువ_delim);
	print_keyvalue("objects.blobs.inflated_size", కీలకం_delim,
		       stats->objects.inflated_sizes.blobs, విలువ_delim);
	print_keyvalue("objects.tags.inflated_size", కీలకం_delim,
		       stats->objects.inflated_sizes.ట్యాగులు, విలువ_delim);

	print_keyvalue("objects.commits.disk_size", కీలకం_delim,
		       stats->objects.disk_sizes.commits, విలువ_delim);
	print_keyvalue("objects.trees.disk_size", కీలకం_delim,
		       stats->objects.disk_sizes.trees, విలువ_delim);
	print_keyvalue("objects.blobs.disk_size", కీలకం_delim,
		       stats->objects.disk_sizes.blobs, విలువ_delim);
	print_keyvalue("objects.tags.disk_size", కీలకం_delim,
		       stats->objects.disk_sizes.ట్యాగులు, విలువ_delim);

	print_object_దత్తాంశం("objects.commits.max_size", కీలకం_delim,
			  &stats->objects.అతిపెద్ద.commit_కొలత, విలువ_delim);
	print_object_దత్తాంశం("objects.trees.max_size", కీలకం_delim,
			  &stats->objects.అతిపెద్ద.వృక్షం_కొలత, విలువ_delim);
	print_object_దత్తాంశం("objects.blobs.max_size", కీలకం_delim,
			  &stats->objects.అతిపెద్ద.blob_కొలత, విలువ_delim);
	print_object_దత్తాంశం("objects.tags.max_size", కీలకం_delim,
			  &stats->objects.అతిపెద్ద.ట్యాగ్_కొలత, విలువ_delim);

	print_object_దత్తాంశం("objects.commits.max_parents", కీలకం_delim,
			  &stats->objects.అతిపెద్ద.మాతృ_లెక్క, విలువ_delim);
	print_object_దత్తాంశం("objects.trees.max_entries", కీలకం_delim,
			  &stats->objects.అతిపెద్ద.వృక్షం_entries, విలువ_delim);

	fflush(stdout);
}

నిర్మాణం లెక్క_references_దత్తాంశం {
	నిర్మాణం ref_stats *stats;
	నిర్మాణం rev_info *revs;
	నిర్మాణం పురోగతి *పురోగతి;
};

స్థిర పూర్ణాంకం లెక్క_references(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం లెక్క_references_దత్తాంశం *దత్తాంశం = cb_దత్తాంశం;
	నిర్మాణం ref_stats *stats = దత్తాంశం->stats;
	కొలత_t ref_లెక్క;

	ఎంపిక (ref_రకం_from_refname(ref->name)) {
	స్థితి FILTER_REFS_BRANCHES:
		stats->branches++;
		ఆపు;
	స్థితి FILTER_REFS_REMOTES:
		stats->remotes++;
		ఆపు;
	స్థితి FILTER_REFS_ట్యాగులు:
		stats->ట్యాగులు++;
		ఆపు;
	స్థితి FILTER_REFS_OTHERS:
		stats->others++;
		ఆపు;
	అప్రమేయ:
		BUG("unexpected reference type");
	}

	/*
	 * While iterating through references for counting, also add OIDs in
	 * preparation for the path walk.
	 */
	కూడు_pending_oid(దత్తాంశం->revs, ఖాళీ, ref->oid, 0);

	ref_లెక్క = get_మొత్తం_reference_లెక్క(stats);
	display_పురోగతి(దత్తాంశం->పురోగతి, ref_లెక్క);

	తిరిగి 0;
}

స్థిర శూన్య structure_లెక్క_references(నిర్మాణం ref_stats *stats,
				       నిర్మాణం rev_info *revs,
				       నిర్మాణం repository *repo,
				       పూర్ణాంకం show_పురోగతి)
{
	నిర్మాణం లెక్క_references_దత్తాంశం దత్తాంశం = {
		.stats = stats,
		.revs = revs,
	};

	ఒకవేళ (show_పురోగతి)
		దత్తాంశం.పురోగతి = ప్రారంభం_delayed_పురోగతి(repo,
						       _("Counting references"), 0);

	refs_for_each_ref(get_main_ref_store(repo), లెక్క_references, &దత్తాంశం);
	stop_పురోగతి(&దత్తాంశం.పురోగతి);
}

నిర్మాణం లెక్క_objects_దత్తాంశం {
	నిర్మాణం object_database *odb;
	నిర్మాణం object_stats *stats;
	నిర్మాణం పురోగతి *పురోగతి;
};

స్థిర శూన్య check_అతిపెద్ద(నిర్మాణం object_దత్తాంశం *దత్తాంశం, నిర్మాణం object_id *oid,
			  కొలత_t విలువ)
{
	ఒకవేళ (విలువ > దత్తాంశం->విలువ || is_null_oid(&దత్తాంశం->oid)) {
		oidcpy(&దత్తాంశం->oid, oid);
		దత్తాంశం->విలువ = విలువ;
	}
}

స్థిర కొలత_t లెక్క_వృక్షం_entries(నిర్మాణం object *obj)
{
	నిర్మాణం వృక్షం *t = object_as_type(obj, OBJ_వృక్షం, 0);
	నిర్మాణం name_నమోదు నమోదు_2;
	నిర్మాణం వృక్షం_desc desc;
	కొలత_t లెక్క = 0;

	init_వృక్షం_desc(&desc, &t->object.oid, t->బఫరు, t->కొలత);
	అయితే (వృక్షం_నమోదు(&desc, &నమోదు_2))
		లెక్క++;

	తిరిగి లెక్క;
}

స్థిర పూర్ణాంకం లెక్క_objects(స్థిరరాశి అక్షరం *మార్గం వాడని, నిర్మాణం oid_array *oids,
			 గణన object_type type, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం లెక్క_objects_దత్తాంశం *దత్తాంశం = cb_దత్తాంశం;
	నిర్మాణం object_stats *stats = దత్తాంశం->stats;
	కొలత_t object_లెక్క;

	కొరకు (కొలత_t i = 0; i < oids->nr; i++) {
		నిర్మాణం object_info oi = OBJECT_INFO_INIT;
		గుర్తులేని దీర్ఘ inflated;
		కొలత_t inflated_st = 0;
		నిర్మాణం commit *commit;
		నిర్మాణం object *obj;
		శూన్య *content;
		off_t disk;
		పూర్ణాంకం eaten;

		oi.sizep = &inflated_st;
		oi.disk_sizep = &disk;
		oi.contentp = &content;

		ఒకవేళ (odb_read_object_info_extended(దత్తాంశం->odb, &oids->oid[i], &oi,
						  OBJECT_INFO_SKIP_FETCH_OBJECT |
						  OBJECT_INFO_శీఘ్ర) < 0)
			కొనసాగు;
		inflated = cast_కొలత_t_గమ్యం_ulong(inflated_st);

		obj = parse_object_బఫరు(the_repository, &oids->oid[i], type,
					  inflated, content, &eaten);

		ఎంపిక (type) {
		స్థితి OBJ_ట్యాగ్:
			stats->type_counts.ట్యాగులు++;
			stats->inflated_sizes.ట్యాగులు += inflated;
			stats->disk_sizes.ట్యాగులు += disk;
			check_అతిపెద్ద(&stats->అతిపెద్ద.ట్యాగ్_కొలత, &oids->oid[i],
				      inflated);
			ఆపు;
		స్థితి OBJ_COMMIT:
			commit = object_as_type(obj, OBJ_COMMIT, 0);
			stats->type_counts.commits++;
			stats->inflated_sizes.commits += inflated;
			stats->disk_sizes.commits += disk;
			check_అతిపెద్ద(&stats->అతిపెద్ద.commit_కొలత, &oids->oid[i],
				      inflated);
			check_అతిపెద్ద(&stats->అతిపెద్ద.మాతృ_లెక్క, &oids->oid[i],
				      commit_list_లెక్క(commit->parents));
			ఆపు;
		స్థితి OBJ_వృక్షం:
			stats->type_counts.trees++;
			stats->inflated_sizes.trees += inflated;
			stats->disk_sizes.trees += disk;
			check_అతిపెద్ద(&stats->అతిపెద్ద.వృక్షం_కొలత, &oids->oid[i],
				      inflated);
			check_అతిపెద్ద(&stats->అతిపెద్ద.వృక్షం_entries, &oids->oid[i],
				      లెక్క_వృక్షం_entries(obj));
			ఆపు;
		స్థితి OBJ_BLOB:
			stats->type_counts.blobs++;
			stats->inflated_sizes.blobs += inflated;
			stats->disk_sizes.blobs += disk;
			check_అతిపెద్ద(&stats->అతిపెద్ద.blob_కొలత, &oids->oid[i],
				      inflated);
			ఆపు;
		అప్రమేయ:
			BUG("invalid object type");
		}

		ఒకవేళ (!eaten)
			విడుదల(content);
	}

	object_లెక్క = get_మొత్తం_object_విలువలు(&stats->type_counts);
	display_పురోగతి(దత్తాంశం->పురోగతి, object_లెక్క);

	తిరిగి 0;
}

స్థిర శూన్య structure_లెక్క_objects(నిర్మాణం object_stats *stats,
				    నిర్మాణం rev_info *revs,
				    నిర్మాణం repository *repo, పూర్ణాంకం show_పురోగతి)
{
	నిర్మాణం మార్గం_walk_info info = మార్గం_WALK_INFO_INIT;
	నిర్మాణం లెక్క_objects_దత్తాంశం దత్తాంశం = {
		.odb = repo->objects,
		.stats = stats,
	};

	info.revs = revs;
	info.మార్గం_fn = లెక్క_objects;
	info.మార్గం_fn_దత్తాంశం = &దత్తాంశం;

	ఒకవేళ (show_పురోగతి)
		దత్తాంశం.పురోగతి = ప్రారంభం_delayed_పురోగతి(repo, _("Counting objects"), 0);

	walk_objects_by_మార్గం(&info);
	మార్గం_walk_info_clear(&info);
	stop_పురోగతి(&దత్తాంశం.పురోగతి);
}

స్థిర పూర్ణాంకం cmd_repo_structure(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			      నిర్మాణం repository *repo)
{
	నిర్మాణం stats_పట్టిక పట్టిక = {
		.అడ్డువరుసలు = STRING_LIST_INIT_DUP,
		.annotations = STRING_LIST_INIT_DUP,
	};
	గణన output_format format = FORMAT_పట్టిక;
	నిర్మాణం repo_structure stats = { 0 };
	నిర్మాణం rev_info revs;
	పూర్ణాంకం show_పురోగతి = -1;
	నిర్మాణం option options[] = {
		OPT_CALLBACK_F(0, "format", &format, N_("format"),
			       N_("output format"),
			       PARSE_OPT_NONEG, parse_format_cb),
		OPT_CALLBACK_F('z', ఖాళీ, &format, ఖాళీ,
			       N_("synonym for --format=nul"),
			       PARSE_OPT_NONEG | PARSE_OPT_NOARG,
			       parse_format_cb),
		OPT_BOOL(0, "progress", &show_పురోగతి, N_("show progress")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, repo_structure_usage, 0);
	ఒకవేళ (argc)
		usage(_("too many arguments"));

	repo_init_revisions(repo, &revs, ఉపసర్గ);

	ఒకవేళ (show_పురోగతి < 0)
		show_పురోగతి = isatty(2);

	structure_లెక్క_references(&stats.refs, &revs, repo, show_పురోగతి);
	structure_లెక్క_objects(&stats.objects, &revs, repo, show_పురోగతి);

	ఎంపిక (format) {
	స్థితి FORMAT_పట్టిక:
		stats_పట్టిక_అమరిక_structure(&పట్టిక, &stats);
		stats_పట్టిక_print_structure(&పట్టిక);
		ఆపు;
	స్థితి FORMAT_NEWLINE_TERMINATED:
		structure_keyvalue_print(&stats, '=', '\n');
		ఆపు;
	స్థితి FORMAT_NUL_TERMINATED:
		structure_keyvalue_print(&stats, '\n', '\0');
		ఆపు;
	అప్రమేయ:
		BUG("invalid output format");
	}

	stats_పట్టిక_clear(&పట్టిక);
	release_revisions(&revs);

	తిరిగి 0;
}

పూర్ణాంకం cmd_repo(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
	     నిర్మాణం repository *repo)
{
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_SUBCOMMAND("info", &fn, cmd_repo_info),
		OPT_SUBCOMMAND("structure", &fn, cmd_repo_structure),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, repo_usage, 0);

	తిరిగి fn(argc, argv, ఉపసర్గ, repo);
}
