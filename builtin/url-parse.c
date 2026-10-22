#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "url.h"
#અન્તર્ભાવ "urlmatch.h"

સ્થિર અચલ અક્ષર * અચલ builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	ખાલી
};

સ્થિર અક્ષર *component_arg;

સ્થિર સંરચના option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_END(),
};

ગણના url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_PORT,
	URL_PATH,
};

સ્થિર શૂન્ય parse_or_die(અચલ અક્ષર *url, સંરચના url_info *info)
{
	જો (url_is_local_not_ssh(url)) {
		જો (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		જો (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	જો (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

સ્થિર ગણના url_component get_component_or_die(અચલ અક્ષર *arg)
{
	જો (!strcmp("path", arg))
		પાછા URL_PATH;
	જો (!strcmp("host", arg))
		પાછા URL_HOST;
	જો (!strcmp("scheme", arg))
		પાછા URL_SCHEME;
	જો (!strcmp("user", arg))
		પાછા URL_USER;
	જો (!strcmp("password", arg))
		પાછા URL_PASSWORD;
	જો (!strcmp("port", arg))
		પાછા URL_PORT;
	die("invalid git URL component '%s'", arg);
}

સ્થિર અક્ષર *extract_component(ગણના url_component component,
			       સંરચના url_info *info)
{
	size_t offset, length;

	પસંદગી (component) {
	સ્થિતિ URL_SCHEME:
		offset = 0;
		length = info->scheme_len;
		તોડો;
	સ્થિતિ URL_USER:
		offset = info->user_off;
		length = info->user_len;
		તોડો;
	સ્થિતિ URL_PASSWORD:
		offset = info->passwd_off;
		length = info->passwd_len;
		તોડો;
	સ્થિતિ URL_HOST:
		offset = info->host_off;
		length = info->host_len;
		તોડો;
	સ્થિતિ URL_PORT:
		offset = info->port_off;
		length = info->port_len;
		તોડો;
	સ્થિતિ URL_PATH:
		offset = info->path_off;
		length = info->path_len;
		તોડો;
	સ્થિતિ URL_NONE:
		પાછા ખાલી;
	}

	પાછા xstrndup(info->url + offset, length);
}

પૂર્ણાંક cmd_url_parse(પૂર્ણાંક argc,
		  અચલ અક્ષર **argv,
		  અચલ અક્ષર *prefix,
		  સંરચના repository *repo UNUSED)
{
	સંરચના url_info info;
	ગણના url_component selected = URL_NONE;
	અક્ષર *extracted;
	પૂર્ણાંક i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	જો (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	જો (component_arg)
		selected = get_component_or_die(component_arg);

	માટે (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		જો (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			જો (extracted) {
				puts(extracted);
				મુક્ત(extracted);
			}
		}

		મુક્ત(info.url);
	}

	પાછા 0;
}
