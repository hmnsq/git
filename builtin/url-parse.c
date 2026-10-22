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
	OPT_انتها(),
};

تعداد url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_پورت,
	URL_مسیر,
};

ساكن فراغ parse_or_die(ثابت حرف *url, بنية url_info *info)
{
	إذا (url_is_local_not_ssh(url)) {
		إذا (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		إذا (has_dos_drive_پیشوند(url))
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
		أرجع URL_مسیر;
	إذا (!strcmp("host", arg))
		أرجع URL_HOST;
	إذا (!strcmp("scheme", arg))
		أرجع URL_SCHEME;
	إذا (!strcmp("user", arg))
		أرجع URL_USER;
	إذا (!strcmp("password", arg))
		أرجع URL_PASSWORD;
	إذا (!strcmp("port", arg))
		أرجع URL_پورت;
	die("invalid git URL component '%s'", arg);
}

ساكن حرف *استخراج_component(تعداد url_component component,
			       بنية url_info *info)
{
	اندازه_t جابه_جایی, درازی;

	بدل (component) {
	حالة URL_SCHEME:
		جابه_جایی = 0;
		درازی = info->scheme_len;
		اكسر;
	حالة URL_USER:
		جابه_جایی = info->user_off;
		درازی = info->user_len;
		اكسر;
	حالة URL_PASSWORD:
		جابه_جایی = info->passwd_off;
		درازی = info->passwd_len;
		اكسر;
	حالة URL_HOST:
		جابه_جایی = info->host_off;
		درازی = info->host_len;
		اكسر;
	حالة URL_پورت:
		جابه_جایی = info->پورت_off;
		درازی = info->پورت_len;
		اكسر;
	حالة URL_مسیر:
		جابه_جایی = info->مسیر_off;
		درازی = info->مسیر_len;
		اكسر;
	حالة URL_NONE:
		أرجع عدم;
	}

	أرجع xstrndup(info->url + جابه_جایی, درازی);
}

صحيح cmd_url_parse(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *پیشوند,
		  بنية repository *repo استفادهنشده)
{
	بنية url_info info;
	تعداد url_component selected = URL_NONE;
	حرف *extracted;
	صحيح i;

	argc = parse_options(argc, argv, پیشوند, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	إذا (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	إذا (component_arg)
		selected = get_component_or_die(component_arg);

	لكل (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		إذا (selected != URL_NONE) {
			extracted = استخراج_component(selected, &info);
			إذا (extracted) {
				puts(extracted);
				حرر(extracted);
			}
		}

		حرر(info.url);
	}

	أرجع 0;
}
