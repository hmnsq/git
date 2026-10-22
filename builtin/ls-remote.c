#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "transport.h"
#ಅನ್ತರ್ಭಾವ "pkt-line.h"
#ಅನ್ತರ್ಭಾವ "ref-filter.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "wildmatch.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ls_remote_usage[] = {
	N_("git ls-remote [--branches] [--tags] [--refs] [--upload-pack=<exec>]\n"
	   "              [-q | --quiet] [--exit-code] [--get-url] [--sort=<key>]\n"
	   "              [--symref] [<repository> [<patterns>...]]"),
	ಖಾಲಿ
};

/*
 * Is there one among the list of patterns that match the tail part
 * of the path?
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಬಾಲ_ಹೊಂದಿಕೆ(ಸ್ಥಿರಾಂಕ ರಚನೆ strvec *ನಮೂನೆ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಅಕ್ಷರ *pathbuf;

	ಆದರೆ (!ನಮೂನೆ->nr)
		ಹಿಂತಿರುಗಿ 1; /* no restriction */

	pathbuf = xstrfmt("/%s", ಮಾರ್ಗ);
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ನಮೂನೆ->nr; i++) {
		ಆದರೆ (!wildmatch(ನಮೂನೆ->v[i], pathbuf, 0)) {
			ಬಿಡುಗಡೆ(pathbuf);
			ಹಿಂತಿರುಗಿ 1;
		}
	}
	ಬಿಡುಗಡೆ(pathbuf);
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_ls_remote(ಪೂರ್ಣಾಂಕ argc,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		  ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dest = ಖಾಲಿ;
	ಚಿಹ್ನೆರಹಿತ flags = 0;
	ಪೂರ್ಣಾಂಕ get_url = 0;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = 0;
	ಪೂರ್ಣಾಂಕ show_symref_ಗುರಿ = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *uploadpack = ಖಾಲಿ;
	ರಚನೆ strvec ನಮೂನೆ = STRVEC_INIT;
	ರಚನೆ transport_ls_refs_options transport_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ string_list server_options = STRING_LIST_INIT_DUP;

	ರಚನೆ remote *remote;
	ರಚನೆ transport *transport;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ref *ref;
	ರಚನೆ ref_array ref_array;
	ರಚನೆ ref_ವಿಂಗಡಣೆ *ವಿಂಗಡಣೆ;
	ರಚನೆ string_list ವಿಂಗಡಣೆ_options = STRING_LIST_INIT_DUP;

	ರಚನೆ option options[] = {
		OPT__QUIET(&quiet, N_("do not print remote URL")),
		OPT_STRING(0, "upload-pack", &uploadpack, N_("exec"),
			   N_("path of git-upload-pack on the remote host")),
		{
			.type = OPTION_STRING,
			.long_name = "exec",
			.ಮೌಲ್ಯ = &uploadpack,
			.argh = N_("exec"),
			.help = N_("path of git-upload-pack on the remote host"),
			.flags = PARSE_OPT_HIDDEN,
		},
		OPT_ಬಿಟ್('t', "tags", &flags, N_("limit to tags"), REF_ಟ್ಯಾಗುಗಳು),
		OPT_ಬಿಟ್('b', "branches", &flags, N_("limit to branches"), REF_BRANCHES),
		OPT_ಬಿಟ್_F('h', "heads", &flags,
			  N_("deprecated synonym for --branches"), REF_BRANCHES,
			  PARSE_OPT_HIDDEN),
		OPT_ಬಿಟ್(0, "refs", &flags, N_("do not show peeled tags"), REF_ಸಾಮಾನ್ಯ),
		OPT_BOOL(0, "get-url", &get_url,
			 N_("take url.<base>.insteadOf into account")),
		OPT_REF_ವಿಂಗಡಿಸು(&ವಿಂಗಡಣೆ_options),
		OPT_ಗಣ_INT_F(0, "exit-code", &ಸ್ಥಿತಿ_2,
			      N_("exit with exit code 2 if no matching refs are found"),
			      2, PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "symref", &show_symref_ಗುರಿ,
			 N_("show underlying ref in addition to the object pointed by it")),
		OPT_STRING_LIST('o', "server-option", &server_options, N_("server-specific"), N_("option to transmit")),
		OPT_ಕೊನೆ()
	};

	memset(&ref_array, 0, ಗಾತ್ರ(ref_array));

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, ls_remote_usage,
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
	ಆದರೆ (!the_repository->hash_algo)
		repo_ಗಣ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	packet_trace_ತಟಸ್ಥ("ls-remote");

	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < argc; i++)
		strvec_pushf(&ನಮೂನೆ, "*/%s", argv[i]);

	ಆದರೆ (flags & REF_ಟ್ಯಾಗುಗಳು)
		strvec_ತಳ್ಳು(&transport_options.ref_prefixes, "refs/tags/");
	ಆದರೆ (flags & REF_BRANCHES)
		strvec_ತಳ್ಳು(&transport_options.ref_prefixes, "refs/heads/");

	remote = remote_get(dest);
	ಆದರೆ (!remote) {
		ಆದರೆ (dest)
			die("bad repository '%s'", dest);
		die("No remote configured to list refs from.");
	}

	ಆದರೆ (get_url) {
		ಬರೆ("%s\n", remote->url.v[0]);
		ಹಿಂತಿರುಗಿ 0;
	}

	transport = transport_get(remote, ಖಾಲಿ);
	ಆದರೆ (uploadpack)
		transport_ಗಣ_option(transport, TRANS_OPT_UPLOADPACK, uploadpack);
	ಆದರೆ (server_options.nr)
		transport->server_options = &server_options;

	ref = transport_get_remote_refs(transport, &transport_options);
	ಆದರೆ (ref) {
		ಪೂರ್ಣಾಂಕ hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
		repo_ಗಣ_hash_algo(the_repository, hash_algo);
	}

	ಆದರೆ (!dest && !quiet)
		fprintf(stderr, "From %s\n", remote->url.v[0]);
	ಗಾಗಿ ( ; ref; ref = ref->ಮುಂದಿನ) {
		ರಚನೆ ref_array_ಅಂಶ *ಅಂಶ;
		ಆದರೆ (!check_ref_type(ref, flags))
			ಮುಂದುವರಿ;
		ಆದರೆ (!ಬಾಲ_ಹೊಂದಿಕೆ(&ನಮೂನೆ, ref->name))
			ಮುಂದುವರಿ;
		ಅಂಶ = ref_array_ತಳ್ಳು(&ref_array, ref->name, &ref->old_oid, ಖಾಲಿ);
		ಅಂಶ->symref = xstrdup_or_null(ref->symref);
	}

	ವಿಂಗಡಣೆ = ref_ವಿಂಗಡಣೆ_options(&ವಿಂಗಡಣೆ_options);
	ref_array_ವಿಂಗಡಿಸು(ವಿಂಗಡಣೆ, &ref_array);

	ಗಾಗಿ (i = 0; i < ref_array.nr; i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ ref_array_ಅಂಶ *ref = ref_array.ವಸ್ತುಗಳು[i];
		ಆದರೆ (show_symref_ಗುರಿ && ref->symref)
			ಬರೆ("ref: %s\t%s\n", ref->symref, ref->refname);
		ಬರೆ("%s\t%s\n", oid_ಗಮ್ಯ_hex(&ref->objectname), ref->refname);
		ಸ್ಥಿತಿ_2 = 0; /* we found something */
	}

	string_list_clear(&server_options, 0);
	ref_ವಿಂಗಡಣೆ_release(ವಿಂಗಡಣೆ);
	ref_array_clear(&ref_array);
	ಆದರೆ (transport_disconnect(transport))
		ಸ್ಥಿತಿ_2 = 1;
	transport_ls_refs_options_release(&transport_options);

	strvec_clear(&ನಮೂನೆ);
	string_list_clear(&server_options, 0);
	ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}
