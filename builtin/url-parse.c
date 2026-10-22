#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "url.h"
#अन्तर्भाव "urlmatch.h"

स्थिर अचर वर्ण * अचर builtin_url_parse_usage[] = {
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

स्थिर शून्य parse_or_die(अचर वर्ण *url, संरचना url_info *info)
{
	यदि (url_is_local_not_ssh(url)) {
		यदि (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		यदि (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	यदि (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

स्थिर गणना url_component get_component_or_die(अचर वर्ण *arg)
{
	यदि (!म_भेद("path", arg))
		फर्काऊ URL_PATH;
	यदि (!म_भेद("host", arg))
		फर्काऊ URL_HOST;
	यदि (!म_भेद("scheme", arg))
		फर्काऊ URL_SCHEME;
	यदि (!म_भेद("user", arg))
		फर्काऊ URL_USER;
	यदि (!म_भेद("password", arg))
		फर्काऊ URL_PASSWORD;
	यदि (!म_भेद("port", arg))
		फर्काऊ URL_PORT;
	die("invalid git URL component '%s'", arg);
}

स्थिर वर्ण *extract_component(गणना url_component component,
			       संरचना url_info *info)
{
	माप_प्रकार offset, length;

	विकल्प (component) {
	पक्ष URL_SCHEME:
		offset = 0;
		length = info->scheme_len;
		रोक;
	पक्ष URL_USER:
		offset = info->user_off;
		length = info->user_len;
		रोक;
	पक्ष URL_PASSWORD:
		offset = info->passwd_off;
		length = info->passwd_len;
		रोक;
	पक्ष URL_HOST:
		offset = info->host_off;
		length = info->host_len;
		रोक;
	पक्ष URL_PORT:
		offset = info->port_off;
		length = info->port_len;
		रोक;
	पक्ष URL_PATH:
		offset = info->path_off;
		length = info->path_len;
		रोक;
	पक्ष URL_NONE:
		फर्काऊ रिक्त;
	}

	फर्काऊ xstrndup(info->url + offset, length);
}

पूर्णाङ्क cmd_url_parse(पूर्णाङ्क argc,
		  अचर वर्ण **argv,
		  अचर वर्ण *prefix,
		  संरचना repository *repo UNUSED)
{
	संरचना url_info info;
	गणना url_component selected = URL_NONE;
	वर्ण *extracted;
	पूर्णाङ्क i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	यदि (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	यदि (component_arg)
		selected = get_component_or_die(component_arg);

	लागि (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		यदि (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			यदि (extracted) {
				माला_दो(extracted);
				मुक्त(extracted);
			}
		}

		मुक्त(info.url);
	}

	फर्काऊ 0;
}
