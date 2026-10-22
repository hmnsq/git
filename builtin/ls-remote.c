#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "transport.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pkt-line.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ref-filter.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wildmatch.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ ls_remote_usage[] = {
	N_("git ls-remote [--branches] [--tags] [--refs] [--upload-pack=<exec>]\n"
	   "              [-q | --quiet] [--exit-code] [--get-url] [--sort=<key>]\n"
	   "              [--symref] [<repository> [<patterns>...]]"),
	ਨਿਰਾ
};

/*
 * Is there one among the list of patterns that match the tail part
 * of the path?
 */
ਸਥਿਰ ਪੂਰਨ ਪੂਛ_ਮੇਲ(ਅਚਲ ਬਣਤਰ strvec *ਨਮੂਨਾ, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਅੱਖਰ *pathbuf;

	ਜੇ (!ਨਮੂਨਾ->nr)
		ਵਾਪਸ 1; /* no restriction */

	pathbuf = xstrfmt("/%s", ਰਸਤਾ);
	ਲਈ (ਮਾਪ_t i = 0; i < ਨਮੂਨਾ->nr; i++) {
		ਜੇ (!wildmatch(ਨਮੂਨਾ->v[i], pathbuf, 0)) {
			ਮੁਕਤ(pathbuf);
			ਵਾਪਸ 1;
		}
	}
	ਮੁਕਤ(pathbuf);
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_ls_remote(ਪੂਰਨ argc,
		  ਅਚਲ ਅੱਖਰ **argv,
		  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *dest = ਨਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ flags = 0;
	ਪੂਰਨ get_url = 0;
	ਪੂਰਨ quiet = 0;
	ਪੂਰਨ ਸਥਿਤੀ = 0;
	ਪੂਰਨ show_symref_ਨਿਸ਼ਾਨਾ = 0;
	ਅਚਲ ਅੱਖਰ *uploadpack = ਨਿਰਾ;
	ਬਣਤਰ strvec ਨਮੂਨਾ = STRVEC_INIT;
	ਬਣਤਰ transport_ls_refs_options transport_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	ਪੂਰਨ i;
	ਬਣਤਰ string_list server_options = STRING_LIST_INIT_DUP;

	ਬਣਤਰ remote *remote;
	ਬਣਤਰ transport *transport;
	ਅਚਲ ਬਣਤਰ ref *ref;
	ਬਣਤਰ ref_array ref_array;
	ਬਣਤਰ ref_ਛਾਂਟੀ *ਛਾਂਟੀ;
	ਬਣਤਰ string_list ਛਾਂਟੀ_options = STRING_LIST_INIT_DUP;

	ਬਣਤਰ option options[] = {
		OPT__QUIET(&quiet, N_("do not print remote URL")),
		OPT_STRING(0, "upload-pack", &uploadpack, N_("exec"),
			   N_("path of git-upload-pack on the remote host")),
		{
			.type = OPTION_STRING,
			.long_name = "exec",
			.ਮੁੱਲ = &uploadpack,
			.argh = N_("exec"),
			.help = N_("path of git-upload-pack on the remote host"),
			.flags = PARSE_OPT_HIDDEN,
		},
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('t', "tags", &flags, N_("limit to tags"), REF_ਟੈਗ_ਸੂਚੀ),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('b', "branches", &flags, N_("limit to branches"), REF_BRANCHES),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ_F('h', "heads", &flags,
			  N_("deprecated synonym for --branches"), REF_BRANCHES,
			  PARSE_OPT_HIDDEN),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "refs", &flags, N_("do not show peeled tags"), REF_ਸਾਧਾਰਨ),
		OPT_BOOL(0, "get-url", &get_url,
			 N_("take url.<base>.insteadOf into account")),
		OPT_REF_ਛਾਂਟੋ(&ਛਾਂਟੀ_options),
		OPT_ਸੈੱਟ_INT_F(0, "exit-code", &ਸਥਿਤੀ,
			      N_("exit with exit code 2 if no matching refs are found"),
			      2, PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "symref", &show_symref_ਨਿਸ਼ਾਨਾ,
			 N_("show underlying ref in addition to the object pointed by it")),
		OPT_STRING_LIST('o', "server-option", &server_options, N_("server-specific"), N_("option to transmit")),
		OPT_ਸਿਰਾ()
	};

	memset(&ref_array, 0, ਆਕਾਰ(ref_array));

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, ls_remote_usage,
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
	ਜੇ (!the_repository->hash_algo)
		repo_ਸੈੱਟ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	packet_trace_ਪਛਾਣ_ਤੱਤ("ls-remote");

	ਲਈ (ਪੂਰਨ i = 1; i < argc; i++)
		strvec_pushf(&ਨਮੂਨਾ, "*/%s", argv[i]);

	ਜੇ (flags & REF_ਟੈਗ_ਸੂਚੀ)
		strvec_ਧੱਕੋ(&transport_options.ref_prefixes, "refs/tags/");
	ਜੇ (flags & REF_BRANCHES)
		strvec_ਧੱਕੋ(&transport_options.ref_prefixes, "refs/heads/");

	remote = remote_get(dest);
	ਜੇ (!remote) {
		ਜੇ (dest)
			die("bad repository '%s'", dest);
		die("No remote configured to list refs from.");
	}

	ਜੇ (get_url) {
		ਲਿਖੋ("%s\n", remote->url.v[0]);
		ਵਾਪਸ 0;
	}

	transport = transport_get(remote, ਨਿਰਾ);
	ਜੇ (uploadpack)
		transport_ਸੈੱਟ_option(transport, TRANS_OPT_UPLOADPACK, uploadpack);
	ਜੇ (server_options.nr)
		transport->server_options = &server_options;

	ref = transport_get_remote_refs(transport, &transport_options);
	ਜੇ (ref) {
		ਪੂਰਨ hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
		repo_ਸੈੱਟ_hash_algo(the_repository, hash_algo);
	}

	ਜੇ (!dest && !quiet)
		fprintf(stderr, "From %s\n", remote->url.v[0]);
	ਲਈ ( ; ref; ref = ref->ਅਗਲਾ) {
		ਬਣਤਰ ref_array_ਵਸਤੂ *ਵਸਤੂ;
		ਜੇ (!check_ref_type(ref, flags))
			ਜਾਰੀ;
		ਜੇ (!ਪੂਛ_ਮੇਲ(&ਨਮੂਨਾ, ref->name))
			ਜਾਰੀ;
		ਵਸਤੂ = ref_array_ਧੱਕੋ(&ref_array, ref->name, &ref->old_oid, ਨਿਰਾ);
		ਵਸਤੂ->symref = xstrdup_or_null(ref->symref);
	}

	ਛਾਂਟੀ = ref_ਛਾਂਟੀ_options(&ਛਾਂਟੀ_options);
	ref_array_ਛਾਂਟੋ(ਛਾਂਟੀ, &ref_array);

	ਲਈ (i = 0; i < ref_array.nr; i++) {
		ਅਚਲ ਬਣਤਰ ref_array_ਵਸਤੂ *ref = ref_array.ਚੀਜ਼ਾਂ[i];
		ਜੇ (show_symref_ਨਿਸ਼ਾਨਾ && ref->symref)
			ਲਿਖੋ("ref: %s\t%s\n", ref->symref, ref->refname);
		ਲਿਖੋ("%s\t%s\n", oid_ਮੰਜ਼ਿਲ_hex(&ref->objectname), ref->refname);
		ਸਥਿਤੀ = 0; /* we found something */
	}

	string_list_clear(&server_options, 0);
	ref_ਛਾਂਟੀ_release(ਛਾਂਟੀ);
	ref_array_clear(&ref_array);
	ਜੇ (transport_disconnect(transport))
		ਸਥਿਤੀ = 1;
	transport_ls_refs_options_release(&transport_options);

	strvec_clear(&ਨਮੂਨਾ);
	string_list_clear(&server_options, 0);
	ਵਾਪਸ ਸਥਿਤੀ;
}
