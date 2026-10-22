#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "transport.h"
#समावेश "pkt-line.h"
#समावेश "ref-filter.h"
#समावेश "remote.h"
#समावेश "parse-options.h"
#समावेश "wildmatch.h"

अटल स्थिर अक्षर * स्थिर ls_remote_usage[] = {
	N_("git ls-remote [--branches] [--tags] [--refs] [--upload-pack=<exec>]\n"
	   "              [-q | --quiet] [--exit-code] [--get-url] [--sort=<key>]\n"
	   "              [--symref] [<repository> [<patterns>...]]"),
	शून्य
};

/*
 * Is there one among the list of patterns that match the tail part
 * of the path?
 */
अटल पूर्णांक पुच्छ_match(स्थिर काष्ठा strvec *pattern, स्थिर अक्षर *path)
{
	अक्षर *pathbuf;

	अगर (!pattern->nr)
		वापस 1; /* no restriction */

	pathbuf = xstrfmt("/%s", path);
	क्रम (माप_प्रकार i = 0; i < pattern->nr; i++) {
		अगर (!wildmatch(pattern->v[i], pathbuf, 0)) {
			मुक्त(pathbuf);
			वापस 1;
		}
	}
	मुक्त(pathbuf);
	वापस 0;
}

पूर्णांक cmd_ls_remote(पूर्णांक argc,
		  स्थिर अक्षर **argv,
		  स्थिर अक्षर *prefix,
		  काष्ठा repository *repo UNUSED)
{
	स्थिर अक्षर *dest = शून्य;
	अचिन्हित flags = 0;
	पूर्णांक get_url = 0;
	पूर्णांक quiet = 0;
	पूर्णांक status = 0;
	पूर्णांक show_symref_target = 0;
	स्थिर अक्षर *uploadpack = शून्य;
	काष्ठा strvec pattern = STRVEC_INIT;
	काष्ठा transport_ls_refs_options transport_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	पूर्णांक i;
	काष्ठा string_list server_options = STRING_LIST_INIT_DUP;

	काष्ठा remote *remote;
	काष्ठा transport *transport;
	स्थिर काष्ठा ref *ref;
	काष्ठा ref_array ref_array;
	काष्ठा ref_sorting *sorting;
	काष्ठा string_list sorting_options = STRING_LIST_INIT_DUP;

	काष्ठा option options[] = {
		OPT__QUIET(&quiet, N_("do not print remote URL")),
		OPT_STRING(0, "upload-pack", &uploadpack, N_("exec"),
			   N_("path of git-upload-pack on the remote host")),
		{
			.type = OPTION_STRING,
			.long_name = "exec",
			.मूल्य = &uploadpack,
			.argh = N_("exec"),
			.help = N_("path of git-upload-pack on the remote host"),
			.flags = PARSE_OPT_HIDDEN,
		},
		OPT_BIT('t', "tags", &flags, N_("limit to tags"), REF_TAGS),
		OPT_BIT('b', "branches", &flags, N_("limit to branches"), REF_BRANCHES),
		OPT_BIT_F('h', "heads", &flags,
			  N_("deprecated synonym for --branches"), REF_BRANCHES,
			  PARSE_OPT_HIDDEN),
		OPT_BIT(0, "refs", &flags, N_("do not show peeled tags"), REF_NORMAL),
		OPT_BOOL(0, "get-url", &get_url,
			 N_("take url.<base>.insteadOf into account")),
		OPT_REF_SORT(&sorting_options),
		OPT_SET_INT_F(0, "exit-code", &status,
			      N_("exit with exit code 2 if no matching refs are found"),
			      2, PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "symref", &show_symref_target,
			 N_("show underlying ref in addition to the object pointed by it")),
		OPT_STRING_LIST('o', "server-option", &server_options, N_("server-specific"), N_("option to transmit")),
		OPT_END()
	};

	स_रखो(&ref_array, 0, माप(ref_array));

	argc = parse_options(argc, argv, prefix, options, ls_remote_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	dest = argv[0];

	/*
	 * TODO: This is buggy, but required for transport helpers. When a
	 * transport helper advertises a "refspec", then we'd add that to a
	 * list of refspecs via `refspec_append()`, which transitively depends
	 * on `the_hash_algo`. Thus, when the hash algorithm isn't properly set
	 * up, this would lead to a segfault.
	 *
	 * We really should fix this in the transport helper logic such that we
	 * lazily parse refspec capabilities _after_ we have learned about the
	 * remote's object format. Otherwise, we may end up misparsing refspecs
	 * depending on what object hash the remote uses.
	 */
	अगर (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	packet_trace_identity("ls-remote");

	क्रम (पूर्णांक i = 1; i < argc; i++)
		strvec_pushf(&pattern, "*/%s", argv[i]);

	अगर (flags & REF_TAGS)
		strvec_धकेलो(&transport_options.ref_prefixes, "refs/tags/");
	अगर (flags & REF_BRANCHES)
		strvec_धकेलो(&transport_options.ref_prefixes, "refs/heads/");

	remote = remote_get(dest);
	अगर (!remote) {
		अगर (dest)
			die("bad repository '%s'", dest);
		die("No remote configured to list refs from.");
	}

	अगर (get_url) {
		म_लिखो("%s\n", remote->url.v[0]);
		वापस 0;
	}

	transport = transport_get(remote, शून्य);
	अगर (uploadpack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK, uploadpack);
	अगर (server_options.nr)
		transport->server_options = &server_options;

	ref = transport_get_remote_refs(transport, &transport_options);
	अगर (ref) {
		पूर्णांक hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
		repo_set_hash_algo(the_repository, hash_algo);
	}

	अगर (!dest && !quiet)
		ख_लिखो(मानक_त्रुटि, "From %s\n", remote->url.v[0]);
	क्रम ( ; ref; ref = ref->अग्र) {
		काष्ठा ref_array_item *item;
		अगर (!check_ref_type(ref, flags))
			जारी;
		अगर (!पुच्छ_match(&pattern, ref->name))
			जारी;
		item = ref_array_धकेलो(&ref_array, ref->name, &ref->old_oid, शून्य);
		item->symref = xstrdup_or_null(ref->symref);
	}

	sorting = ref_sorting_options(&sorting_options);
	ref_array_sort(sorting, &ref_array);

	क्रम (i = 0; i < ref_array.nr; i++) {
		स्थिर काष्ठा ref_array_item *ref = ref_array.वस्तुएँ[i];
		अगर (show_symref_target && ref->symref)
			म_लिखो("ref: %s\t%s\n", ref->symref, ref->refname);
		म_लिखो("%s\t%s\n", oid_to_hex(&ref->objectname), ref->refname);
		status = 0; /* we found something */
	}

	string_list_clear(&server_options, 0);
	ref_sorting_release(sorting);
	ref_array_clear(&ref_array);
	अगर (transport_disconnect(transport))
		status = 1;
	transport_ls_refs_options_release(&transport_options);

	strvec_clear(&pattern);
	string_list_clear(&server_options, 0);
	वापस status;
}
