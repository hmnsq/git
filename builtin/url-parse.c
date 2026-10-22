#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "url.h"
#അന്തര്ഭാവ "urlmatch.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	ശൂന്യ
};

സ്ഥിതം അക്ഷരം *component_arg;

സ്ഥിതം ഘടന option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_END(),
};

എണ്ണൽ url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_PORT,
	URL_PATH,
};

സ്ഥിതം ശൂന്യം parse_or_die(സ്ഥിരം അക്ഷരം *url, ഘടന url_info *info)
{
	എങ്കിൽ (url_is_local_not_ssh(url)) {
		എങ്കിൽ (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		എങ്കിൽ (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	എങ്കിൽ (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

സ്ഥിതം എണ്ണൽ url_component get_component_or_die(സ്ഥിരം അക്ഷരം *arg)
{
	എങ്കിൽ (!strcmp("path", arg))
		തിരികെ URL_PATH;
	എങ്കിൽ (!strcmp("host", arg))
		തിരികെ URL_HOST;
	എങ്കിൽ (!strcmp("scheme", arg))
		തിരികെ URL_SCHEME;
	എങ്കിൽ (!strcmp("user", arg))
		തിരികെ URL_USER;
	എങ്കിൽ (!strcmp("password", arg))
		തിരികെ URL_PASSWORD;
	എങ്കിൽ (!strcmp("port", arg))
		തിരികെ URL_PORT;
	die("invalid git URL component '%s'", arg);
}

സ്ഥിതം അക്ഷരം *extract_component(എണ്ണൽ url_component component,
			       ഘടന url_info *info)
{
	size_t offset, length;

	തിരഞ്ഞെടുപ്പ് (component) {
	അവസ്ഥ URL_SCHEME:
		offset = 0;
		length = info->scheme_len;
		നിർത്തുക;
	അവസ്ഥ URL_USER:
		offset = info->user_off;
		length = info->user_len;
		നിർത്തുക;
	അവസ്ഥ URL_PASSWORD:
		offset = info->passwd_off;
		length = info->passwd_len;
		നിർത്തുക;
	അവസ്ഥ URL_HOST:
		offset = info->host_off;
		length = info->host_len;
		നിർത്തുക;
	അവസ്ഥ URL_PORT:
		offset = info->port_off;
		length = info->port_len;
		നിർത്തുക;
	അവസ്ഥ URL_PATH:
		offset = info->path_off;
		length = info->path_len;
		നിർത്തുക;
	അവസ്ഥ URL_NONE:
		തിരികെ ശൂന്യ;
	}

	തിരികെ xstrndup(info->url + offset, length);
}

പൂർണ്ണസംഖ്യ cmd_url_parse(പൂർണ്ണസംഖ്യ argc,
		  സ്ഥിരം അക്ഷരം **argv,
		  സ്ഥിരം അക്ഷരം *prefix,
		  ഘടന repository *repo UNUSED)
{
	ഘടന url_info info;
	എണ്ണൽ url_component selected = URL_NONE;
	അക്ഷരം *extracted;
	പൂർണ്ണസംഖ്യ i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	എങ്കിൽ (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	എങ്കിൽ (component_arg)
		selected = get_component_or_die(component_arg);

	വേണ്ടി (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		എങ്കിൽ (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			എങ്കിൽ (extracted) {
				puts(extracted);
				സ്വതന്ത്രം(extracted);
			}
		}

		സ്വതന്ത്രം(info.url);
	}

	തിരികെ 0;
}
