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
	𐤀𐤌 (url_is_local_not_ssh(url)) {
		𐤀𐤌 (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		𐤀𐤌 (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	𐤀𐤌 (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

ساكن تعداد url_component get_component_or_die(ثابت حرف *arg)
{
	𐤀𐤌 (!strcmp("path", arg))
		أرجع URL_PATH;
	𐤀𐤌 (!strcmp("host", arg))
		أرجع URL_HOST;
	𐤀𐤌 (!strcmp("scheme", arg))
		أرجع URL_SCHEME;
	𐤀𐤌 (!strcmp("user", arg))
		أرجع URL_USER;
	𐤀𐤌 (!strcmp("password", arg))
		أرجع URL_PASSWORD;
	𐤀𐤌 (!strcmp("port", arg))
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

	𐤀𐤌 (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	𐤀𐤌 (component_arg)
		selected = get_component_or_die(component_arg);

	لكل (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		𐤀𐤌 (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			𐤀𐤌 (extracted) {
				puts(extracted);
				حرر(extracted);
			}
		}

		حرر(info.url);
	}

	أرجع 0;
}
