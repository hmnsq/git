#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "transport.h"
#അന്തര്ഭാവ "pkt-line.h"
#അന്തര്ഭാവ "ref-filter.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "wildmatch.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം ls_remote_usage[] = {
	N_("git ls-remote [--branches] [--tags] [--refs] [--upload-pack=<exec>]\n"
	   "              [-q | --quiet] [--exit-code] [--get-url] [--sort=<key>]\n"
	   "              [--symref] [<repository> [<patterns>...]]"),
	ശൂന്യ
};

/*
 * Is there one among the list of patterns that match the tail part
 * of the path?
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ tail_match(സ്ഥിരം ഘടന strvec *pattern, സ്ഥിരം അക്ഷരം *path)
{
	അക്ഷരം *pathbuf;

	എങ്കിൽ (!pattern->nr)
		തിരികെ 1; /* no restriction */

	pathbuf = xstrfmt("/%s", path);
	വേണ്ടി (size_t i = 0; i < pattern->nr; i++) {
		എങ്കിൽ (!wildmatch(pattern->v[i], pathbuf, 0)) {
			സ്വതന്ത്രം(pathbuf);
			തിരികെ 1;
		}
	}
	സ്വതന്ത്രം(pathbuf);
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_ls_remote(പൂർണ്ണസംഖ്യ argc,
		  സ്ഥിരം അക്ഷരം **argv,
		  സ്ഥിരം അക്ഷരം *prefix,
		  ഘടന repository *repo UNUSED)
{
	സ്ഥിരം അക്ഷരം *dest = ശൂന്യ;
	unsigned flags = 0;
	പൂർണ്ണസംഖ്യ get_url = 0;
	പൂർണ്ണസംഖ്യ quiet = 0;
	പൂർണ്ണസംഖ്യ status = 0;
	പൂർണ്ണസംഖ്യ show_symref_target = 0;
	സ്ഥിരം അക്ഷരം *uploadpack = ശൂന്യ;
	ഘടന strvec pattern = STRVEC_INIT;
	ഘടന transport_ls_refs_options transport_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	പൂർണ്ണസംഖ്യ i;
	ഘടന string_list server_options = STRING_LIST_INIT_DUP;

	ഘടന remote *remote;
	ഘടന transport *transport;
	സ്ഥിരം ഘടന ref *ref;
	ഘടന ref_array ref_array;
	ഘടന ref_sorting *sorting;
	ഘടന string_list sorting_options = STRING_LIST_INIT_DUP;

	ഘടന option options[] = {
		OPT__QUIET(&quiet, N_("do not print remote URL")),
		OPT_STRING(0, "upload-pack", &uploadpack, N_("exec"),
			   N_("path of git-upload-pack on the remote host")),
		{
			.type = OPTION_STRING,
			.long_name = "exec",
			.മൂല്യം = &uploadpack,
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

	memset(&ref_array, 0, വലുപ്പം(ref_array));

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
	എങ്കിൽ (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	packet_trace_identity("ls-remote");

	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < argc; i++)
		strvec_pushf(&pattern, "*/%s", argv[i]);

	എങ്കിൽ (flags & REF_TAGS)
		strvec_തള്ളുക(&transport_options.ref_prefixes, "refs/tags/");
	എങ്കിൽ (flags & REF_BRANCHES)
		strvec_തള്ളുക(&transport_options.ref_prefixes, "refs/heads/");

	remote = remote_get(dest);
	എങ്കിൽ (!remote) {
		എങ്കിൽ (dest)
			die("bad repository '%s'", dest);
		die("No remote configured to list refs from.");
	}

	എങ്കിൽ (get_url) {
		printf("%s\n", remote->url.v[0]);
		തിരികെ 0;
	}

	transport = transport_get(remote, ശൂന്യ);
	എങ്കിൽ (uploadpack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK, uploadpack);
	എങ്കിൽ (server_options.nr)
		transport->server_options = &server_options;

	ref = transport_get_remote_refs(transport, &transport_options);
	എങ്കിൽ (ref) {
		പൂർണ്ണസംഖ്യ hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
		repo_set_hash_algo(the_repository, hash_algo);
	}

	എങ്കിൽ (!dest && !quiet)
		fprintf(stderr, "From %s\n", remote->url.v[0]);
	വേണ്ടി ( ; ref; ref = ref->next) {
		ഘടന ref_array_item *item;
		എങ്കിൽ (!check_ref_type(ref, flags))
			തുടരുക;
		എങ്കിൽ (!tail_match(&pattern, ref->name))
			തുടരുക;
		item = ref_array_തള്ളുക(&ref_array, ref->name, &ref->old_oid, ശൂന്യ);
		item->symref = xstrdup_or_null(ref->symref);
	}

	sorting = ref_sorting_options(&sorting_options);
	ref_array_sort(sorting, &ref_array);

	വേണ്ടി (i = 0; i < ref_array.nr; i++) {
		സ്ഥിരം ഘടന ref_array_item *ref = ref_array.ഇനങ്ങൾ[i];
		എങ്കിൽ (show_symref_target && ref->symref)
			printf("ref: %s\t%s\n", ref->symref, ref->refname);
		printf("%s\t%s\n", oid_to_hex(&ref->objectname), ref->refname);
		status = 0; /* we found something */
	}

	string_list_clear(&server_options, 0);
	ref_sorting_release(sorting);
	ref_array_clear(&ref_array);
	എങ്കിൽ (transport_disconnect(transport))
		status = 1;
	transport_ls_refs_options_release(&transport_options);

	strvec_clear(&pattern);
	string_list_clear(&server_options, 0);
	തിരികെ status;
}
