#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "url.h"
#अन्तर्भाव "urlmatch.h"

स्थिर ध्रुव वर्ण * ध्रुव builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	रिक्त
};

स्थिर वर्ण *component_arg;

स्थिर संरचना option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_END(),
};

गणना url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_PORT,
	URL_PATH,
};

स्थिर शून्य parse_or_die(ध्रुव वर्ण *url, संरचना url_info *info)
{
	जइ (url_is_local_not_ssh(url)) {
		जइ (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		जइ (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	जइ (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

स्थिर गणना url_component get_component_or_die(ध्रुव वर्ण *arg)
{
	जइ (!strcmp("path", arg))
		प्रत्यागम URL_PATH;
	जइ (!strcmp("host", arg))
		प्रत्यागम URL_HOST;
	जइ (!strcmp("scheme", arg))
		प्रत्यागम URL_SCHEME;
	जइ (!strcmp("user", arg))
		प्रत्यागम URL_USER;
	जइ (!strcmp("password", arg))
		प्रत्यागम URL_PASSWORD;
	जइ (!strcmp("port", arg))
		प्रत्यागम URL_PORT;
	die("invalid git URL component '%s'", arg);
}

स्थिर वर्ण *extract_component(गणना url_component component,
			       संरचना url_info *info)
{
	size_t offset, length;

	विकल्प (component) {
	पक्ष URL_SCHEME:
		offset = 0;
		length = info->scheme_len;
		भङ्ग;
	पक्ष URL_USER:
		offset = info->user_off;
		length = info->user_len;
		भङ्ग;
	पक्ष URL_PASSWORD:
		offset = info->passwd_off;
		length = info->passwd_len;
		भङ्ग;
	पक्ष URL_HOST:
		offset = info->host_off;
		length = info->host_len;
		भङ्ग;
	पक्ष URL_PORT:
		offset = info->port_off;
		length = info->port_len;
		भङ्ग;
	पक्ष URL_PATH:
		offset = info->path_off;
		length = info->path_len;
		भङ्ग;
	पक्ष URL_NONE:
		प्रत्यागम रिक्त;
	}

	प्रत्यागम xstrndup(info->url + offset, length);
}

पूर्णाङ्क cmd_url_parse(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *prefix,
		  संरचना repository *repo UNUSED)
{
	संरचना url_info info;
	गणना url_component selected = URL_NONE;
	वर्ण *extracted;
	पूर्णाङ्क i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	जइ (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	जइ (component_arg)
		selected = get_component_or_die(component_arg);

	प्रति (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		जइ (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			जइ (extracted) {
				puts(extracted);
				मुक्त(extracted);
			}
		}

		मुक्त(info.url);
	}

	प्रत्यागम 0;
}
