#అన్తర్భావ "builtin.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "url.h"
#అన్తర్భావ "urlmatch.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	ఖాళీ
};

స్థిర అక్షరం *component_arg;

స్థిర నిర్మాణం option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_చివర(),
};

గణన url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_పోర్టు,
	URL_మార్గం,
};

స్థిర శూన్య parse_or_die(స్థిరరాశి అక్షరం *url, నిర్మాణం url_info *info)
{
	ఒకవేళ (url_is_local_not_ssh(url)) {
		ఒకవేళ (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		ఒకవేళ (has_dos_drive_ఉపసర్గ(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	ఒకవేళ (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

స్థిర గణన url_component get_component_or_die(స్థిరరాశి అక్షరం *arg)
{
	ఒకవేళ (!strcmp("path", arg))
		తిరిగి URL_మార్గం;
	ఒకవేళ (!strcmp("host", arg))
		తిరిగి URL_HOST;
	ఒకవేళ (!strcmp("scheme", arg))
		తిరిగి URL_SCHEME;
	ఒకవేళ (!strcmp("user", arg))
		తిరిగి URL_USER;
	ఒకవేళ (!strcmp("password", arg))
		తిరిగి URL_PASSWORD;
	ఒకవేళ (!strcmp("port", arg))
		తిరిగి URL_పోర్టు;
	die("invalid git URL component '%s'", arg);
}

స్థిర అక్షరం *వెలికితీయి_component(గణన url_component component,
			       నిర్మాణం url_info *info)
{
	కొలత_t స్థానభ్రంశం, నిడివి;

	ఎంపిక (component) {
	స్థితి URL_SCHEME:
		స్థానభ్రంశం = 0;
		నిడివి = info->scheme_len;
		ఆపు;
	స్థితి URL_USER:
		స్థానభ్రంశం = info->user_off;
		నిడివి = info->user_len;
		ఆపు;
	స్థితి URL_PASSWORD:
		స్థానభ్రంశం = info->passwd_off;
		నిడివి = info->passwd_len;
		ఆపు;
	స్థితి URL_HOST:
		స్థానభ్రంశం = info->host_off;
		నిడివి = info->host_len;
		ఆపు;
	స్థితి URL_పోర్టు:
		స్థానభ్రంశం = info->పోర్టు_off;
		నిడివి = info->పోర్టు_len;
		ఆపు;
	స్థితి URL_మార్గం:
		స్థానభ్రంశం = info->మార్గం_off;
		నిడివి = info->మార్గం_len;
		ఆపు;
	స్థితి URL_NONE:
		తిరిగి ఖాళీ;
	}

	తిరిగి xstrndup(info->url + స్థానభ్రంశం, నిడివి);
}

పూర్ణాంకం cmd_url_parse(పూర్ణాంకం argc,
		  స్థిరరాశి అక్షరం **argv,
		  స్థిరరాశి అక్షరం *ఉపసర్గ,
		  నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం url_info info;
	గణన url_component selected = URL_NONE;
	అక్షరం *extracted;
	పూర్ణాంకం i;

	argc = parse_options(argc, argv, ఉపసర్గ, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	ఒకవేళ (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	ఒకవేళ (component_arg)
		selected = get_component_or_die(component_arg);

	కొరకు (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		ఒకవేళ (selected != URL_NONE) {
			extracted = వెలికితీయి_component(selected, &info);
			ఒకవేళ (extracted) {
				puts(extracted);
				విడుదల(extracted);
			}
		}

		విడుదల(info.url);
	}

	తిరిగి 0;
}
