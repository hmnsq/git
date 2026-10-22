#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "url.h"
#ଅନ୍ତର୍ଭାଵ "urlmatch.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	ଖାଲି
};

ସ୍ଥିର ଅକ୍ଷର *component_arg;

ସ୍ଥିର ସଂରଚନା option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_END(),
};

ଗଣନା url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_PORT,
	URL_PATH,
};

ସ୍ଥିର ଶୂନ୍ୟ parse_or_die(ଅଚଳ ଅକ୍ଷର *url, ସଂରଚନା url_info *info)
{
	ଯଦି (url_is_local_not_ssh(url)) {
		ଯଦି (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		ଯଦି (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	ଯଦି (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

ସ୍ଥିର ଗଣନା url_component get_component_or_die(ଅଚଳ ଅକ୍ଷର *arg)
{
	ଯଦି (!strcmp("path", arg))
		ଫେରନ୍ତୁ URL_PATH;
	ଯଦି (!strcmp("host", arg))
		ଫେରନ୍ତୁ URL_HOST;
	ଯଦି (!strcmp("scheme", arg))
		ଫେରନ୍ତୁ URL_SCHEME;
	ଯଦି (!strcmp("user", arg))
		ଫେରନ୍ତୁ URL_USER;
	ଯଦି (!strcmp("password", arg))
		ଫେରନ୍ତୁ URL_PASSWORD;
	ଯଦି (!strcmp("port", arg))
		ଫେରନ୍ତୁ URL_PORT;
	die("invalid git URL component '%s'", arg);
}

ସ୍ଥିର ଅକ୍ଷର *extract_component(ଗଣନା url_component component,
			       ସଂରଚନା url_info *info)
{
	size_t offset, length;

	ବାଛ (component) {
	ସ୍ଥିତି URL_SCHEME:
		offset = 0;
		length = info->scheme_len;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି URL_USER:
		offset = info->user_off;
		length = info->user_len;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି URL_PASSWORD:
		offset = info->passwd_off;
		length = info->passwd_len;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି URL_HOST:
		offset = info->host_off;
		length = info->host_len;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି URL_PORT:
		offset = info->port_off;
		length = info->port_len;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି URL_PATH:
		offset = info->path_off;
		length = info->path_len;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି URL_NONE:
		ଫେରନ୍ତୁ ଖାଲି;
	}

	ଫେରନ୍ତୁ xstrndup(info->url + offset, length);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_url_parse(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		  ଅଚଳ ଅକ୍ଷର **argv,
		  ଅଚଳ ଅକ୍ଷର *prefix,
		  ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା url_info info;
	ଗଣନା url_component selected = URL_NONE;
	ଅକ୍ଷର *extracted;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	ଯଦି (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	ଯଦି (component_arg)
		selected = get_component_or_die(component_arg);

	ପାଇଁ (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		ଯଦି (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			ଯଦି (extracted) {
				puts(extracted);
				ମୁକ୍ତ(extracted);
			}
		}

		ମୁକ୍ତ(info.url);
	}

	ଫେରନ୍ତୁ 0;
}
