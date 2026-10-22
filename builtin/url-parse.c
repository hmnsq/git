#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "url.h"
#অন্তর্ভাব "urlmatch.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	ফাঁকা
};

স্থির অক্ষর *component_arg;

স্থির কাঠামো option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_END(),
};

গণনা url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_PORT,
	URL_PATH,
};

স্থির শূন্য parse_or_die(ধ্রুবক অক্ষর *url, কাঠামো url_info *info)
{
	যদি (url_is_local_not_ssh(url)) {
		যদি (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		যদি (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	যদি (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

স্থির গণনা url_component get_component_or_die(ধ্রুবক অক্ষর *arg)
{
	যদি (!strcmp("path", arg))
		ফেরত URL_PATH;
	যদি (!strcmp("host", arg))
		ফেরত URL_HOST;
	যদি (!strcmp("scheme", arg))
		ফেরত URL_SCHEME;
	যদি (!strcmp("user", arg))
		ফেরত URL_USER;
	যদি (!strcmp("password", arg))
		ফেরত URL_PASSWORD;
	যদি (!strcmp("port", arg))
		ফেরত URL_PORT;
	die("invalid git URL component '%s'", arg);
}

স্থির অক্ষর *extract_component(গণনা url_component component,
			       কাঠামো url_info *info)
{
	আকার_t offset, দৈর্ঘ্য;

	নির্বাচন (component) {
	ক্ষেত্র URL_SCHEME:
		offset = 0;
		দৈর্ঘ্য = info->scheme_len;
		বিরতি;
	ক্ষেত্র URL_USER:
		offset = info->user_off;
		দৈর্ঘ্য = info->user_len;
		বিরতি;
	ক্ষেত্র URL_PASSWORD:
		offset = info->passwd_off;
		দৈর্ঘ্য = info->passwd_len;
		বিরতি;
	ক্ষেত্র URL_HOST:
		offset = info->host_off;
		দৈর্ঘ্য = info->host_len;
		বিরতি;
	ক্ষেত্র URL_PORT:
		offset = info->port_off;
		দৈর্ঘ্য = info->port_len;
		বিরতি;
	ক্ষেত্র URL_PATH:
		offset = info->path_off;
		দৈর্ঘ্য = info->path_len;
		বিরতি;
	ক্ষেত্র URL_NONE:
		ফেরত ফাঁকা;
	}

	ফেরত xstrndup(info->url + offset, দৈর্ঘ্য);
}

পূর্ণসংখ্যা cmd_url_parse(পূর্ণসংখ্যা argc,
		  ধ্রুবক অক্ষর **argv,
		  ধ্রুবক অক্ষর *prefix,
		  কাঠামো repository *repo UNUSED)
{
	কাঠামো url_info info;
	গণনা url_component selected = URL_NONE;
	অক্ষর *extracted;
	পূর্ণসংখ্যা i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	যদি (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	যদি (component_arg)
		selected = get_component_or_die(component_arg);

	জন্য (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		যদি (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			যদি (extracted) {
				puts(extracted);
				মুক্ত(extracted);
			}
		}

		মুক্ত(info.url);
	}

	ফেরত 0;
}
