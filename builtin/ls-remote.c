#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "transport.h"
#அந்தர்பாவ "pkt-line.h"
#அந்தர்பாவ "ref-filter.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "wildmatch.h"

நிலையான மாறிலி எழுத்து * மாறிலி ls_remote_usage[] = {
	N_("git ls-remote [--branches] [--tags] [--refs] [--upload-pack=<exec>]\n"
	   "              [-q | --quiet] [--exit-code] [--get-url] [--sort=<key>]\n"
	   "              [--symref] [<repository> [<patterns>...]]"),
	வெற்று
};

/*
 * Is there one among the list of patterns that match the tail part
 * of the path?
 */
நிலையான முழு tail_match(மாறிலி கட்டமைப்பு strvec *pattern, மாறிலி எழுத்து *path)
{
	எழுத்து *pathbuf;

	எனில் (!pattern->nr)
		திரும்பு 1; /* no restriction */

	pathbuf = xstrfmt("/%s", path);
	க்கு (size_t i = 0; i < pattern->nr; i++) {
		எனில் (!wildmatch(pattern->v[i], pathbuf, 0)) {
			விடுவி(pathbuf);
			திரும்பு 1;
		}
	}
	விடுவி(pathbuf);
	திரும்பு 0;
}

முழு cmd_ls_remote(முழு argc,
		  மாறிலி எழுத்து **argv,
		  மாறிலி எழுத்து *prefix,
		  கட்டமைப்பு repository *repo UNUSED)
{
	மாறிலி எழுத்து *dest = வெற்று;
	unsigned flags = 0;
	முழு get_url = 0;
	முழு quiet = 0;
	முழு status = 0;
	முழு show_symref_target = 0;
	மாறிலி எழுத்து *uploadpack = வெற்று;
	கட்டமைப்பு strvec pattern = STRVEC_INIT;
	கட்டமைப்பு transport_ls_refs_options transport_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	முழு i;
	கட்டமைப்பு string_list server_options = STRING_LIST_INIT_DUP;

	கட்டமைப்பு remote *remote;
	கட்டமைப்பு transport *transport;
	மாறிலி கட்டமைப்பு ref *ref;
	கட்டமைப்பு ref_array ref_array;
	கட்டமைப்பு ref_sorting *sorting;
	கட்டமைப்பு string_list sorting_options = STRING_LIST_INIT_DUP;

	கட்டமைப்பு option options[] = {
		OPT__QUIET(&quiet, N_("do not print remote URL")),
		OPT_STRING(0, "upload-pack", &uploadpack, N_("exec"),
			   N_("path of git-upload-pack on the remote host")),
		{
			.type = OPTION_STRING,
			.long_name = "exec",
			.மதிப்பு = &uploadpack,
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

	memset(&ref_array, 0, அளவு(ref_array));

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
	எனில் (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	packet_trace_identity("ls-remote");

	க்கு (முழு i = 1; i < argc; i++)
		strvec_pushf(&pattern, "*/%s", argv[i]);

	எனில் (flags & REF_TAGS)
		strvec_தள்ளு(&transport_options.ref_prefixes, "refs/tags/");
	எனில் (flags & REF_BRANCHES)
		strvec_தள்ளு(&transport_options.ref_prefixes, "refs/heads/");

	remote = remote_get(dest);
	எனில் (!remote) {
		எனில் (dest)
			die("bad repository '%s'", dest);
		die("No remote configured to list refs from.");
	}

	எனில் (get_url) {
		printf("%s\n", remote->url.v[0]);
		திரும்பு 0;
	}

	transport = transport_get(remote, வெற்று);
	எனில் (uploadpack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK, uploadpack);
	எனில் (server_options.nr)
		transport->server_options = &server_options;

	ref = transport_get_remote_refs(transport, &transport_options);
	எனில் (ref) {
		முழு hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
		repo_set_hash_algo(the_repository, hash_algo);
	}

	எனில் (!dest && !quiet)
		fprintf(stderr, "From %s\n", remote->url.v[0]);
	க்கு ( ; ref; ref = ref->next) {
		கட்டமைப்பு ref_array_item *item;
		எனில் (!check_ref_type(ref, flags))
			தொடர்;
		எனில் (!tail_match(&pattern, ref->name))
			தொடர்;
		item = ref_array_தள்ளு(&ref_array, ref->name, &ref->old_oid, வெற்று);
		item->symref = xstrdup_or_null(ref->symref);
	}

	sorting = ref_sorting_options(&sorting_options);
	ref_array_sort(sorting, &ref_array);

	க்கு (i = 0; i < ref_array.nr; i++) {
		மாறிலி கட்டமைப்பு ref_array_item *ref = ref_array.பொருட்கள்[i];
		எனில் (show_symref_target && ref->symref)
			printf("ref: %s\t%s\n", ref->symref, ref->refname);
		printf("%s\t%s\n", oid_to_hex(&ref->objectname), ref->refname);
		status = 0; /* we found something */
	}

	string_list_clear(&server_options, 0);
	ref_sorting_release(sorting);
	ref_array_clear(&ref_array);
	எனில் (transport_disconnect(transport))
		status = 1;
	transport_ls_refs_options_release(&transport_options);

	strvec_clear(&pattern);
	string_list_clear(&server_options, 0);
	திரும்பு status;
}
