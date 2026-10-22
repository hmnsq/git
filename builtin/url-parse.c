#شامل "builtin.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "url.h"
#شامل "urlmatch.h"

ساکن ثابت حرف * ثابت builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	تہی
};

ساکن حرف *component_arg;

ساکن ساخت option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_انتہا(),
};

شمار url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_پورٹ,
	URL_راستہ,
};

ساکن خالی parse_or_die(ثابت حرف *url, ساخت url_info *info)
{
	اگر (url_is_local_not_ssh(url)) {
		اگر (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		اگر (has_dos_drive_سابقہ(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	اگر (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

ساکن شمار url_component get_component_or_die(ثابت حرف *arg)
{
	اگر (!strcmp("path", arg))
		واپس URL_راستہ;
	اگر (!strcmp("host", arg))
		واپس URL_HOST;
	اگر (!strcmp("scheme", arg))
		واپس URL_SCHEME;
	اگر (!strcmp("user", arg))
		واپس URL_USER;
	اگر (!strcmp("password", arg))
		واپس URL_PASSWORD;
	اگر (!strcmp("port", arg))
		واپس URL_پورٹ;
	die("invalid git URL component '%s'", arg);
}

ساکن حرف *نکالو_component(شمار url_component component,
			       ساخت url_info *info)
{
	سائز_t انحراف, لمبائی;

	انتخاب (component) {
	حالت URL_SCHEME:
		انحراف = 0;
		لمبائی = info->scheme_len;
		توڑو;
	حالت URL_USER:
		انحراف = info->user_off;
		لمبائی = info->user_len;
		توڑو;
	حالت URL_PASSWORD:
		انحراف = info->passwd_off;
		لمبائی = info->passwd_len;
		توڑو;
	حالت URL_HOST:
		انحراف = info->host_off;
		لمبائی = info->host_len;
		توڑو;
	حالت URL_پورٹ:
		انحراف = info->پورٹ_off;
		لمبائی = info->پورٹ_len;
		توڑو;
	حالت URL_راستہ:
		انحراف = info->راستہ_off;
		لمبائی = info->راستہ_len;
		توڑو;
	حالت URL_NONE:
		واپس تہی;
	}

	واپس xstrndup(info->url + انحراف, لمبائی);
}

صحیح cmd_url_parse(صحیح argc,
		  ثابت حرف **argv,
		  ثابت حرف *سابقہ,
		  ساخت repository *repo غیر_مستعمل)
{
	ساخت url_info info;
	شمار url_component selected = URL_NONE;
	حرف *extracted;
	صحیح i;

	argc = parse_options(argc, argv, سابقہ, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	اگر (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	اگر (component_arg)
		selected = get_component_or_die(component_arg);

	برائے (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		اگر (selected != URL_NONE) {
			extracted = نکالو_component(selected, &info);
			اگر (extracted) {
				puts(extracted);
				آزاد(extracted);
			}
		}

		آزاد(info.url);
	}

	واپس 0;
}
