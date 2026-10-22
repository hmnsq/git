#include "builtin.h"
#include "gettext.h"
#include "parse-options.h"
#include "url.h"
#include "urlmatch.h"

static const char * const builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	NULL
};

static char *component_arg;

static struct option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_END(),
};

enum url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_PORT,
	URL_PATH,
};

static void parse_or_die(const char *url, struct url_info *info)
{
	если (url_is_local_not_ssh(url)) {
		если (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		если (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	если (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

static enum url_component get_component_or_die(const char *arg)
{
	если (!strcmp("path", arg))
		вернуть URL_PATH;
	если (!strcmp("host", arg))
		вернуть URL_HOST;
	если (!strcmp("scheme", arg))
		вернуть URL_SCHEME;
	если (!strcmp("user", arg))
		вернуть URL_USER;
	если (!strcmp("password", arg))
		вернуть URL_PASSWORD;
	если (!strcmp("port", arg))
		вернуть URL_PORT;
	die("invalid git URL component '%s'", arg);
}

static char *extract_component(enum url_component component,
			       struct url_info *info)
{
	size_t offset, length;

	switch (component) {
	case URL_SCHEME:
		offset = 0;
		length = info->scheme_len;
		break;
	case URL_USER:
		offset = info->user_off;
		length = info->user_len;
		break;
	case URL_PASSWORD:
		offset = info->passwd_off;
		length = info->passwd_len;
		break;
	case URL_HOST:
		offset = info->host_off;
		length = info->host_len;
		break;
	case URL_PORT:
		offset = info->port_off;
		length = info->port_len;
		break;
	case URL_PATH:
		offset = info->path_off;
		length = info->path_len;
		break;
	case URL_NONE:
		вернуть NULL;
	}

	вернуть xstrndup(info->url + offset, length);
}

целое cmd_url_parse(целое argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	struct url_info info;
	enum url_component selected = URL_NONE;
	char *extracted;
	целое i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	если (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	если (component_arg)
		selected = get_component_or_die(component_arg);

	для (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		если (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			если (extracted) {
				puts(extracted);
				free(extracted);
			}
		}

		free(info.url);
	}

	вернуть 0;
}
