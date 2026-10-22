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
	إذا (url_is_local_not_ssh(url)) {
		إذا (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		إذا (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	إذا (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

ساكن تعداد url_component get_component_or_die(ثابت حرف *arg)
{
	إذا (!strcmp("path", arg))
		أرجع URL_PATH;
	إذا (!strcmp("host", arg))
		أرجع URL_HOST;
	إذا (!strcmp("scheme", arg))
		أرجع URL_SCHEME;
	إذا (!strcmp("user", arg))
		أرجع URL_USER;
	إذا (!strcmp("password", arg))
		أرجع URL_PASSWORD;
	إذا (!strcmp("port", arg))
		أرجع URL_PORT;
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
		أرجع عدم;
	}

	أرجع xstrndup(info->url + offset, length);
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

	إذا (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	إذا (component_arg)
		selected = get_component_or_die(component_arg);

	لكل (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		إذا (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			إذا (extracted) {
				puts(extracted);
				حرر(extracted);
			}
		}

		حرر(info.url);
	}

	أرجع 0;
}
