#شامل "builtin.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "url.h"
#شامل "urlmatch.h"

ایستا ثابت نویسه * ثابت builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	پوچ
};

ایستا نویسه *component_arg;

ایستا ساختار option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_END(),
};

شمارش url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_PORT,
	URL_PATH,
};

ایستا تهی parse_or_die(ثابت نویسه *url, ساختار url_info *info)
{
	اگر (url_is_local_not_ssh(url)) {
		اگر (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		اگر (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	اگر (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

ایستا شمارش url_component get_component_or_die(ثابت نویسه *arg)
{
	اگر (!strcmp("path", arg))
		بازگشت URL_PATH;
	اگر (!strcmp("host", arg))
		بازگشت URL_HOST;
	اگر (!strcmp("scheme", arg))
		بازگشت URL_SCHEME;
	اگر (!strcmp("user", arg))
		بازگشت URL_USER;
	اگر (!strcmp("password", arg))
		بازگشت URL_PASSWORD;
	اگر (!strcmp("port", arg))
		بازگشت URL_PORT;
	die("invalid git URL component '%s'", arg);
}

ایستا نویسه *extract_component(شمارش url_component component,
			       ساختار url_info *info)
{
	size_t offset, length;

	انتخاب (component) {
	حالت URL_SCHEME:
		offset = 0;
		length = info->scheme_len;
		بشکن;
	حالت URL_USER:
		offset = info->user_off;
		length = info->user_len;
		بشکن;
	حالت URL_PASSWORD:
		offset = info->passwd_off;
		length = info->passwd_len;
		بشکن;
	حالت URL_HOST:
		offset = info->host_off;
		length = info->host_len;
		بشکن;
	حالت URL_PORT:
		offset = info->port_off;
		length = info->port_len;
		بشکن;
	حالت URL_PATH:
		offset = info->path_off;
		length = info->path_len;
		بشکن;
	حالت URL_NONE:
		بازگشت پوچ;
	}

	بازگشت xstrndup(info->url + offset, length);
}

صحیح cmd_url_parse(صحیح argc,
		  ثابت نویسه **argv,
		  ثابت نویسه *prefix,
		  ساختار repository *repo UNUSED)
{
	ساختار url_info info;
	شمارش url_component selected = URL_NONE;
	نویسه *extracted;
	صحیح i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	اگر (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	اگر (component_arg)
		selected = get_component_or_die(component_arg);

	برای (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		اگر (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			اگر (extracted) {
				puts(extracted);
				آزاد(extracted);
			}
		}

		آزاد(info.url);
	}

	بازگشت 0;
}
