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
	якщо (url_is_local_not_ssh(url)) {
		якщо (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		якщо (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	якщо (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

static enum url_component get_component_or_die(const char *arg)
{
	якщо (!strcmp("path", arg))
		повернути URL_PATH;
	якщо (!strcmp("host", arg))
		повернути URL_HOST;
	якщо (!strcmp("scheme", arg))
		повернути URL_SCHEME;
	якщо (!strcmp("user", arg))
		повернути URL_USER;
	якщо (!strcmp("password", arg))
		повернути URL_PASSWORD;
	якщо (!strcmp("port", arg))
		повернути URL_PORT;
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
		повернути NULL;
	}

	повернути xstrndup(info->url + offset, length);
}

ціле cmd_url_parse(ціле argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	struct url_info info;
	enum url_component selected = URL_NONE;
	char *extracted;
	ціле i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	якщо (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	якщо (component_arg)
		selected = get_component_or_die(component_arg);

	для (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		якщо (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			якщо (extracted) {
				puts(extracted);
				free(extracted);
			}
		}

		free(info.url);
	}

	повернути 0;
}
