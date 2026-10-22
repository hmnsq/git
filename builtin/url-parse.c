#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "url.h"
#ضمن "urlmatch.h"

ساكن ثابت حرف * ثابت builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	عدم
};

ساكن حرف *component_arg;

ساكن بنية option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_END(),
};

تعداد url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_PORT,
	URL_PATH,
};

ساكن فراغ parse_or_die(ثابت حرف *url, بنية url_info *info)
{
	ܐܢ (url_is_local_not_ssh(url)) {
		ܐܢ (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		ܐܢ (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	ܐܢ (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

ساكن تعداد url_component get_component_or_die(ثابت حرف *arg)
{
	ܐܢ (!strcmp("path", arg))
		ܦܢܐ URL_PATH;
	ܐܢ (!strcmp("host", arg))
		ܦܢܐ URL_HOST;
	ܐܢ (!strcmp("scheme", arg))
		ܦܢܐ URL_SCHEME;
	ܐܢ (!strcmp("user", arg))
		ܦܢܐ URL_USER;
	ܐܢ (!strcmp("password", arg))
		ܦܢܐ URL_PASSWORD;
	ܐܢ (!strcmp("port", arg))
		ܦܢܐ URL_PORT;
	die("invalid git URL component '%s'", arg);
}

ساكن حرف *extract_component(تعداد url_component component,
			       بنية url_info *info)
{
	size_t offset, length;

	بدل (component) {
	حالة URL_SCHEME:
		offset = 0;
		length = info->scheme_len;
		اكسر;
	حالة URL_USER:
		offset = info->user_off;
		length = info->user_len;
		اكسر;
	حالة URL_PASSWORD:
		offset = info->passwd_off;
		length = info->passwd_len;
		اكسر;
	حالة URL_HOST:
		offset = info->host_off;
		length = info->host_len;
		اكسر;
	حالة URL_PORT:
		offset = info->port_off;
		length = info->port_len;
		اكسر;
	حالة URL_PATH:
		offset = info->path_off;
		length = info->path_len;
		اكسر;
	حالة URL_NONE:
		ܦܢܐ عدم;
	}

	ܦܢܐ xstrndup(info->url + offset, length);
}

صحيح cmd_url_parse(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *prefix,
		  بنية repository *repo UNUSED)
{
	بنية url_info info;
	تعداد url_component selected = URL_NONE;
	حرف *extracted;
	صحيح i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	ܐܢ (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	ܐܢ (component_arg)
		selected = get_component_or_die(component_arg);

	لكل (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		ܐܢ (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			ܐܢ (extracted) {
				puts(extracted);
				حرر(extracted);
			}
		}

		حرر(info.url);
	}

	ܦܢܐ 0;
}
