#समावेश "builtin.h"
#समावेश "gettext.h"
#समावेश "parse-options.h"
#समावेश "url.h"
#समावेश "urlmatch.h"

अटल स्थिर अक्षर * स्थिर builtin_url_parse_usage[] = {
	N_("git url-parse [-c <component>] [--] <url>..."),
	शून्य
};

अटल अक्षर *component_arg;

अटल काष्ठा option builtin_url_parse_options[] = {
	OPT_STRING('c', "component", &component_arg, N_("component"),
		N_("which URL component to extract")),
	OPT_END(),
};

क्रमागत url_component {
	URL_NONE = 0,
	URL_SCHEME,
	URL_USER,
	URL_PASSWORD,
	URL_HOST,
	URL_PORT,
	URL_PATH,
};

अटल व्योम parse_or_die(स्थिर अक्षर *url, काष्ठा url_info *info)
{
	अगर (url_is_local_not_ssh(url)) {
		अगर (*url == '/')
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file://%s'", url, url);
		अगर (has_dos_drive_prefix(url))
			die("'%s' is not a URL; if you meant a local "
			    "repository, use 'file:///%s'", url, url);
		die("'%s' is not a URL; if you meant a local repository, "
		    "use a 'file://' URL with an absolute path", url);
	}
	अगर (!url_parse(url, info))
		die("invalid git URL '%s': %s", url, info->err);
}

अटल क्रमागत url_component get_component_or_die(स्थिर अक्षर *arg)
{
	अगर (!म_भेद("path", arg))
		वापस URL_PATH;
	अगर (!म_भेद("host", arg))
		वापस URL_HOST;
	अगर (!म_भेद("scheme", arg))
		वापस URL_SCHEME;
	अगर (!म_भेद("user", arg))
		वापस URL_USER;
	अगर (!म_भेद("password", arg))
		वापस URL_PASSWORD;
	अगर (!म_भेद("port", arg))
		वापस URL_PORT;
	die("invalid git URL component '%s'", arg);
}

अटल अक्षर *extract_component(क्रमागत url_component component,
			       काष्ठा url_info *info)
{
	माप_प्रकार offset, लंबाई;

	चयन (component) {
	हाल URL_SCHEME:
		offset = 0;
		लंबाई = info->scheme_len;
		अवरोध;
	हाल URL_USER:
		offset = info->user_off;
		लंबाई = info->user_len;
		अवरोध;
	हाल URL_PASSWORD:
		offset = info->passwd_off;
		लंबाई = info->passwd_len;
		अवरोध;
	हाल URL_HOST:
		offset = info->host_off;
		लंबाई = info->host_len;
		अवरोध;
	हाल URL_PORT:
		offset = info->port_off;
		लंबाई = info->port_len;
		अवरोध;
	हाल URL_PATH:
		offset = info->path_off;
		लंबाई = info->path_len;
		अवरोध;
	हाल URL_NONE:
		वापस शून्य;
	}

	वापस xstrndup(info->url + offset, लंबाई);
}

पूर्णांक cmd_url_parse(पूर्णांक argc,
		  स्थिर अक्षर **argv,
		  स्थिर अक्षर *prefix,
		  काष्ठा repository *repo UNUSED)
{
	काष्ठा url_info info;
	क्रमागत url_component selected = URL_NONE;
	अक्षर *extracted;
	पूर्णांक i;

	argc = parse_options(argc, argv, prefix, builtin_url_parse_options,
			     builtin_url_parse_usage, 0);

	अगर (argc == 0)
		usage_with_options(builtin_url_parse_usage,
				   builtin_url_parse_options);

	अगर (component_arg)
		selected = get_component_or_die(component_arg);

	क्रम (i = 0; i < argc; i++) {
		parse_or_die(argv[i], &info);

		अगर (selected != URL_NONE) {
			extracted = extract_component(selected, &info);
			अगर (extracted) {
				माला_दो(extracted);
				मुक्त(extracted);
			}
		}

		मुक्त(info.url);
	}

	वापस 0;
}
