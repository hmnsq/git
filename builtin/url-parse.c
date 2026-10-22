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
	OPT_अन्तः(),
};

गणना url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_पत्तनम्,
	URL_मार्गः,
};

स्थिर शून्य parse_or_die(ध्रुव वर्ण *url, संरचना url_info *info)
{
	यदि (url_is_local_not_ssh(url)) {
		यदि (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		यदि (has_dos_drive_उपसर्गः(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	यदि (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

स्थिर गणना url_component get_component_or_die(ध्रुव वर्ण *arg)
{
	यदि (!वाक्यं_तुलय("path", arg))
		प्रत्यागम URL_मार्गः;
	यदि (!वाक्यं_तुलय("host", arg))
		प्रत्यागम URL_HOST;
	यदि (!वाक्यं_तुलय("scheme", arg))
		प्रत्यागम URL_SCHEME;
	यदि (!वाक्यं_तुलय("user", arg))
		प्रत्यागम URL_USER;
	यदि (!वाक्यं_तुलय("password", arg))
		प्रत्यागम URL_PASSWORD;
	यदि (!वाक्यं_तुलय("port", arg))
		प्रत्यागम URL_पत्तनम्;
	die("invalid git URL component '%s'", arg);
}

स्थिर वर्ण *extract_component(गणना url_component component,
			       संरचना url_info *info)
{
	आकार_t विस्थापनम्, दीर्घता;

	विकल्प (component) {
	पक्ष URL_SCHEME:
		विस्थापनम् = 0;
		दीर्घता = info->scheme_len;
		भङ्ग;
	पक्ष URL_USER:
		विस्थापनम् = info->user_off;
		दीर्घता = info->user_len;
		भङ्ग;
	पक्ष URL_PASSWORD:
		विस्थापनम् = info->passwd_off;
		दीर्घता = info->passwd_len;
		भङ्ग;
	पक्ष URL_HOST:
		विस्थापनम् = info->host_off;
		दीर्घता = info->host_len;
		भङ्ग;
	पक्ष URL_पत्तनम्:
		विस्थापनम् = info->पत्तनम्_off;
		दीर्घता = info->पत्तनम्_len;
		भङ्ग;
	पक्ष URL_मार्गः:
		विस्थापनम् = info->मार्गः_off;
		दीर्घता = info->मार्गः_len;
		भङ्ग;
	पक्ष URL_NONE:
		प्रत्यागम रिक्त;
	}

	प्रत्यागम xstrndup(info->url + विस्थापनम्, दीर्घता);
}

पूर्णाङ्क cmd_url_parse(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *उपसर्गः,
		  संरचना repository *repo अप्रयुक्तम्)
{
	संरचना url_info info;
	गणना url_component selected = URL_NONE;
	वर्ण *extracted;
	पूर्णाङ्क i;

	argc = parse_options(argc, argv, उपसर्गः, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	यदि (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	यदि (component_arg)
		selected = get_component_or_die(component_arg);

	प्रति (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		यदि (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			यदि (extracted) {
				पङ्क्तिं_लिख(extracted);
				मुक्त(extracted);
			}
		}

		मुक्त(info.url);
	}

	प्रत्यागम 0;
}
