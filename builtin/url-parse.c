#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "url.h"
#अन्तर्भाव "urlmatch.h"

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	रिक्त
};

स्थिर अक्षर *component_arg;

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

स्थिर शून्य parse_or_die(स्थिरांक अक्षर *url, संरचना url_info *info)
{
	जर (url_is_local_not_ssh(url)) {
		जर (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		जर (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	जर (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

स्थिर गणना url_component get_component_or_die(स्थिरांक अक्षर *arg)
{
	जर (!म_भेद("path", arg))
		परत URL_PATH;
	जर (!म_भेद("host", arg))
		परत URL_HOST;
	जर (!म_भेद("scheme", arg))
		परत URL_SCHEME;
	जर (!म_भेद("user", arg))
		परत URL_USER;
	जर (!म_भेद("password", arg))
		परत URL_PASSWORD;
	जर (!म_भेद("port", arg))
		परत URL_PORT;
	die("invalid git URL component '%s'", arg);
}

स्थिर अक्षर *extract_component(गणना url_component component,
			       संरचना url_info *info)
{
	माप_प्रकार offset, length;

	निवड (component) {
	स्थिती URL_SCHEME:
		offset = 0;
		length = info->scheme_len;
		थांबा;
	स्थिती URL_USER:
		offset = info->user_off;
		length = info->user_len;
		थांबा;
	स्थिती URL_PASSWORD:
		offset = info->passwd_off;
		length = info->passwd_len;
		थांबा;
	स्थिती URL_HOST:
		offset = info->host_off;
		length = info->host_len;
		थांबा;
	स्थिती URL_PORT:
		offset = info->port_off;
		length = info->port_len;
		थांबा;
	स्थिती URL_PATH:
		offset = info->path_off;
		length = info->path_len;
		थांबा;
	स्थिती URL_NONE:
		परत रिक्त;
	}

	परत xstrndup(info->url + offset, length);
}

पूर्णांक cmd_url_parse(पूर्णांक argc,
		  स्थिरांक अक्षर **argv,
		  स्थिरांक अक्षर *prefix,
		  संरचना repository *repo UNUSED)
{
	संरचना url_info info;
	गणना url_component selected = URL_NONE;
	अक्षर *extracted;
	पूर्णांक i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	जर (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	जर (component_arg)
		selected = get_component_or_die(component_arg);

	साठी (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		जर (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			जर (extracted) {
				माला_दो(extracted);
				मुक्त(extracted);
			}
		}

		मुक्त(info.url);
	}

	परत 0;
}
