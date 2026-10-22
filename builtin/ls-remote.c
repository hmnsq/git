#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "transport.h"
#અન્તર્ભાવ "pkt-line.h"
#અન્તર્ભાવ "ref-filter.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "wildmatch.h"

સ્થિર અચલ અક્ષર * અચલ ls_remote_usage[] = {
	N_("git ls-remote [--branches] [--tags] [--refs] [--upload-pack=<exec>]\n"
	   "              [-q | --quiet] [--exit-code] [--get-url] [--sort=<key>]\n"
	   "              [--symref] [<repository> [<patterns>...]]"),
	ખાલી
};

/*
 * Is there one among the list of patterns that match the tail part
 * of the path?
 */
સ્થિર પૂર્ણાંક tail_match(અચલ સંરચના strvec *pattern, અચલ અક્ષર *path)
{
	અક્ષર *pathbuf;

	જો (!pattern->nr)
		પાછા 1; /* no restriction */

	pathbuf = xstrfmt("/%s", path);
	માટે (size_t i = 0; i < pattern->nr; i++) {
		જો (!wildmatch(pattern->v[i], pathbuf, 0)) {
			મુક્ત(pathbuf);
			પાછા 1;
		}
	}
	મુક્ત(pathbuf);
	પાછા 0;
}

પૂર્ણાંક cmd_ls_remote(પૂર્ણાંક argc,
		  અચલ અક્ષર **argv,
		  અચલ અક્ષર *prefix,
		  સંરચના repository *repo UNUSED)
{
	અચલ અક્ષર *dest = ખાલી;
	unsigned flags = 0;
	પૂર્ણાંક get_url = 0;
	પૂર્ણાંક quiet = 0;
	પૂર્ણાંક status = 0;
	પૂર્ણાંક show_symref_target = 0;
	અચલ અક્ષર *uploadpack = ખાલી;
	સંરચના strvec pattern = STRVEC_INIT;
	સંરચના transport_ls_refs_options transport_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	પૂર્ણાંક i;
	સંરચના string_list server_options = STRING_LIST_INIT_DUP;

	સંરચના remote *remote;
	સંરચના transport *transport;
	અચલ સંરચના ref *ref;
	સંરચના ref_array ref_array;
	સંરચના ref_sorting *sorting;
	સંરચના string_list sorting_options = STRING_LIST_INIT_DUP;

	સંરચના option options[] = {
		OPT__QUIET(&quiet, N_("do not print remote URL")),
		OPT_STRING(0, "upload-pack", &uploadpack, N_("exec"),
			   N_("path of git-upload-pack on the remote host")),
		{
			.type = OPTION_STRING,
			.long_name = "exec",
			.કિંમત = &uploadpack,
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

	memset(&ref_array, 0, કદ(ref_array));

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
	જો (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	packet_trace_identity("ls-remote");

	માટે (પૂર્ણાંક i = 1; i < argc; i++)
		strvec_pushf(&pattern, "*/%s", argv[i]);

	જો (flags & REF_TAGS)
		strvec_ધકેલો(&transport_options.ref_prefixes, "refs/tags/");
	જો (flags & REF_BRANCHES)
		strvec_ધકેલો(&transport_options.ref_prefixes, "refs/heads/");

	remote = remote_get(dest);
	જો (!remote) {
		જો (dest)
			die("bad repository '%s'", dest);
		die("No remote configured to list refs from.");
	}

	જો (get_url) {
		printf("%s\n", remote->url.v[0]);
		પાછા 0;
	}

	transport = transport_get(remote, ખાલી);
	જો (uploadpack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK, uploadpack);
	જો (server_options.nr)
		transport->server_options = &server_options;

	ref = transport_get_remote_refs(transport, &transport_options);
	જો (ref) {
		પૂર્ણાંક hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
		repo_set_hash_algo(the_repository, hash_algo);
	}

	જો (!dest && !quiet)
		fprintf(stderr, "From %s\n", remote->url.v[0]);
	માટે ( ; ref; ref = ref->next) {
		સંરચના ref_array_item *item;
		જો (!check_ref_type(ref, flags))
			ચાલુ;
		જો (!tail_match(&pattern, ref->name))
			ચાલુ;
		item = ref_array_ધકેલો(&ref_array, ref->name, &ref->old_oid, ખાલી);
		item->symref = xstrdup_or_null(ref->symref);
	}

	sorting = ref_sorting_options(&sorting_options);
	ref_array_sort(sorting, &ref_array);

	માટે (i = 0; i < ref_array.nr; i++) {
		અચલ સંરચના ref_array_item *ref = ref_array.વસ્તુઓ[i];
		જો (show_symref_target && ref->symref)
			printf("ref: %s\t%s\n", ref->symref, ref->refname);
		printf("%s\t%s\n", oid_to_hex(&ref->objectname), ref->refname);
		status = 0; /* we found something */
	}

	string_list_clear(&server_options, 0);
	ref_sorting_release(sorting);
	ref_array_clear(&ref_array);
	જો (transport_disconnect(transport))
		status = 1;
	transport_ls_refs_options_release(&transport_options);

	strvec_clear(&pattern);
	string_list_clear(&server_options, 0);
	પાછા status;
}
