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
	nếu (url_is_local_not_ssh(url)) {
		nếu (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		nếu (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	nếu (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

static enum url_component get_component_or_die(const char *arg)
{
	nếu (!strcmp("path", arg))
		trả_về URL_PATH;
	nếu (!strcmp("host", arg))
		trả_về URL_HOST;
	nếu (!strcmp("scheme", arg))
		trả_về URL_SCHEME;
	nếu (!strcmp("user", arg))
		trả_về URL_USER;
	nếu (!strcmp("password", arg))
		trả_về URL_PASSWORD;
	nếu (!strcmp("port", arg))
		trả_về URL_PORT;
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
		trả_về NULL;
	}

	trả_về xstrndup(info->url + offset, length);
}

số_nguyên cmd_url_parse(số_nguyên argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	struct url_info info;
	enum url_component selected = URL_NONE;
	char *extracted;
	số_nguyên i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	nếu (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	nếu (component_arg)
		selected = get_component_or_die(component_arg);

	cho (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		nếu (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			nếu (extracted) {
				puts(extracted);
				free(extracted);
			}
		}

		free(info.url);
	}

	trả_về 0;
}
