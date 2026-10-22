#include "builtin.h"
#include "gettext.h"
#include "parse-options.h"
#include "url.h"
#include "urlmatch.h"

static const var_na * const builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	NULL
};

static var_na *component_arg;

static sa_nracanaa option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_END(),
};

ga_nanaa url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_PORT,
	URL_PATH,
};

static void parse_or_die(const var_na *url, sa_nracanaa url_info *info)
{
	ᮚᮦᮔ᮪ (url_is_local_not_ssh(url)) {
		ᮚᮦᮔ᮪ (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		ᮚᮦᮔ᮪ (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	ᮚᮦᮔ᮪ (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

static ga_nanaa url_component get_component_or_die(const var_na *arg)
{
	ᮚᮦᮔ᮪ (!strcmp("path", arg))
		ᮝᮜᮤ URL_PATH;
	ᮚᮦᮔ᮪ (!strcmp("host", arg))
		ᮝᮜᮤ URL_HOST;
	ᮚᮦᮔ᮪ (!strcmp("scheme", arg))
		ᮝᮜᮤ URL_SCHEME;
	ᮚᮦᮔ᮪ (!strcmp("user", arg))
		ᮝᮜᮤ URL_USER;
	ᮚᮦᮔ᮪ (!strcmp("password", arg))
		ᮝᮜᮤ URL_PASSWORD;
	ᮚᮦᮔ᮪ (!strcmp("port", arg))
		ᮝᮜᮤ URL_PORT;
	die("invalid git URL component '%s'", arg);
}

static var_na *extract_component(ga_nanaa url_component component,
			       sa_nracanaa url_info *info)
{
	size_t offset, length;

	switch (component) {
	pak_sa URL_SCHEME:
		offset = 0;
		length = info->scheme_len;
		bha_nga;
	pak_sa URL_USER:
		offset = info->user_off;
		length = info->user_len;
		bha_nga;
	pak_sa URL_PASSWORD:
		offset = info->passwd_off;
		length = info->passwd_len;
		bha_nga;
	pak_sa URL_HOST:
		offset = info->host_off;
		length = info->host_len;
		bha_nga;
	pak_sa URL_PORT:
		offset = info->port_off;
		length = info->port_len;
		bha_nga;
	pak_sa URL_PATH:
		offset = info->path_off;
		length = info->path_len;
		bha_nga;
	pak_sa URL_NONE:
		ᮝᮜᮤ NULL;
	}

	ᮝᮜᮤ xstrndup(info->url + offset, length);
}

puur_naa_nka cmd_url_parse(puur_naa_nka argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa url_info info;
	ga_nanaa url_component selected = URL_NONE;
	var_na *extracted;
	puur_naa_nka i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	ᮚᮦᮔ᮪ (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	ᮚᮦᮔ᮪ (component_arg)
		selected = get_component_or_die(component_arg);

	for (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		ᮚᮦᮔ᮪ (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			ᮚᮦᮔ᮪ (extracted) {
				puts(extracted);
				free(extracted);
			}
		}

		free(info.url);
	}

	ᮝᮜᮤ 0;
}
