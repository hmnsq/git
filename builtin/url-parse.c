#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "url.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "urlmatch.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	ਨਿਰਾ
};

ਸਥਿਰ ਅੱਖਰ *component_arg;

ਸਥਿਰ ਬਣਤਰ option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_ਸਿਰਾ(),
};

ਗਿਣਤੀ url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_ਪੋਰਟ,
	URL_ਰਸਤਾ,
};

ਸਥਿਰ ਖਾਲੀ parse_or_die(ਅਚਲ ਅੱਖਰ *url, ਬਣਤਰ url_info *info)
{
	ਜੇ (url_is_local_not_ssh(url)) {
		ਜੇ (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		ਜੇ (has_dos_drive_ਅਗੇਤਰ(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	ਜੇ (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

ਸਥਿਰ ਗਿਣਤੀ url_component get_component_or_die(ਅਚਲ ਅੱਖਰ *arg)
{
	ਜੇ (!strcmp("path", arg))
		ਵਾਪਸ URL_ਰਸਤਾ;
	ਜੇ (!strcmp("host", arg))
		ਵਾਪਸ URL_HOST;
	ਜੇ (!strcmp("scheme", arg))
		ਵਾਪਸ URL_SCHEME;
	ਜੇ (!strcmp("user", arg))
		ਵਾਪਸ URL_USER;
	ਜੇ (!strcmp("password", arg))
		ਵਾਪਸ URL_PASSWORD;
	ਜੇ (!strcmp("port", arg))
		ਵਾਪਸ URL_ਪੋਰਟ;
	die("invalid git URL component '%s'", arg);
}

ਸਥਿਰ ਅੱਖਰ *ਕੱਢੋ_component(ਗਿਣਤੀ url_component component,
			       ਬਣਤਰ url_info *info)
{
	ਮਾਪ_t ਵਿੱਥ, ਲੰਮਾਈ;

	ਚੋਣ (component) {
	ਹਾਲਤ URL_SCHEME:
		ਵਿੱਥ = 0;
		ਲੰਮਾਈ = info->scheme_len;
		ਰੋਕੋ;
	ਹਾਲਤ URL_USER:
		ਵਿੱਥ = info->user_off;
		ਲੰਮਾਈ = info->user_len;
		ਰੋਕੋ;
	ਹਾਲਤ URL_PASSWORD:
		ਵਿੱਥ = info->passwd_off;
		ਲੰਮਾਈ = info->passwd_len;
		ਰੋਕੋ;
	ਹਾਲਤ URL_HOST:
		ਵਿੱਥ = info->host_off;
		ਲੰਮਾਈ = info->host_len;
		ਰੋਕੋ;
	ਹਾਲਤ URL_ਪੋਰਟ:
		ਵਿੱਥ = info->ਪੋਰਟ_off;
		ਲੰਮਾਈ = info->ਪੋਰਟ_len;
		ਰੋਕੋ;
	ਹਾਲਤ URL_ਰਸਤਾ:
		ਵਿੱਥ = info->ਰਸਤਾ_off;
		ਲੰਮਾਈ = info->ਰਸਤਾ_len;
		ਰੋਕੋ;
	ਹਾਲਤ URL_NONE:
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਵਾਪਸ xstrndup(info->url + ਵਿੱਥ, ਲੰਮਾਈ);
}

ਪੂਰਨ cmd_url_parse(ਪੂਰਨ argc,
		  ਅਚਲ ਅੱਖਰ **argv,
		  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ url_info info;
	ਗਿਣਤੀ url_component selected = URL_NONE;
	ਅੱਖਰ *extracted;
	ਪੂਰਨ i;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	ਜੇ (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	ਜੇ (component_arg)
		selected = get_component_or_die(component_arg);

	ਲਈ (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		ਜੇ (selected != URL_NONE) {
			extracted = ਕੱਢੋ_component(selected, &info);
			ਜੇ (extracted) {
				puts(extracted);
				ਮੁਕਤ(extracted);
			}
		}

		ਮੁਕਤ(info.url);
	}

	ਵਾਪਸ 0;
}
