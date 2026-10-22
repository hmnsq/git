#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "transport.h"
#ضمن "pkt-line.h"
#ضمن "ref-filter.h"
#ضمن "remote.h"
#ضمن "parse-options.h"
#ضمن "wildmatch.h"

ساكن ثابت حرف * ثابت ls_remote_usage[] = {
	N_("git ls-remote [--branches] [--tags] [--refs] [--upload-pack=<exec>]\n"
	   "              [-q | --quiet] [--exit-code] [--get-url] [--sort=<key>]\n"
	   "              [--symref] [<repository> [<patterns>...]]"),
	عدم
};

/*
 * Is there one among the list of patterns that match the tail part
 * of the path?
 */
ساكن صحيح tail_match(ثابت بنية strvec *pattern, ثابت حرف *path)
{
	حرف *pathbuf;

	𐡄𐡍 (!pattern->nr)
		أرجع 1; /* no restriction */

	pathbuf = xstrfmt("/%s", path);
	لكل (size_t i = 0; i < pattern->nr; i++) {
		𐡄𐡍 (!wildmatch(pattern->v[i], pathbuf, 0)) {
			حرر(pathbuf);
			أرجع 1;
		}
	}
	حرر(pathbuf);
	أرجع 0;
}

صحيح cmd_ls_remote(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *prefix,
		  بنية repository *repo UNUSED)
{
	ثابت حرف *dest = عدم;
	unsigned flags = 0;
	صحيح get_url = 0;
	صحيح quiet = 0;
	صحيح status = 0;
	صحيح show_symref_target = 0;
	ثابت حرف *uploadpack = عدم;
	بنية strvec pattern = STRVEC_INIT;
	بنية transport_ls_refs_options transport_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	صحيح i;
	بنية string_list server_options = STRING_LIST_INIT_DUP;

	بنية remote *remote;
	بنية transport *transport;
	ثابت بنية ref *ref;
	بنية ref_array ref_array;
	بنية ref_sorting *sorting;
	بنية string_list sorting_options = STRING_LIST_INIT_DUP;

	بنية option options[] = {
		OPT__QUIET(&quiet, N_("do not print remote URL")),
		OPT_STRING(0, "upload-pack", &uploadpack, N_("exec"),
			   N_("path of git-upload-pack on the remote host")),
		{
			.type = OPTION_STRING,
			.long_name = "exec",
			.value = &uploadpack,
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

	memset(&ref_array, 0, حجم(ref_array));

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
	𐡄𐡍 (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	packet_trace_identity("ls-remote");

	لكل (صحيح i = 1; i < argc; i++)
		strvec_pushf(&pattern, "*/%s", argv[i]);

	𐡄𐡍 (flags & REF_TAGS)
		strvec_push(&transport_options.ref_prefixes, "refs/tags/");
	𐡄𐡍 (flags & REF_BRANCHES)
		strvec_push(&transport_options.ref_prefixes, "refs/heads/");

	remote = remote_get(dest);
	𐡄𐡍 (!remote) {
		𐡄𐡍 (dest)
			die("bad repository '%s'", dest);
		die("No remote configured to list refs from.");
	}

	𐡄𐡍 (get_url) {
		printf("%s\n", remote->url.v[0]);
		أرجع 0;
	}

	transport = transport_get(remote, عدم);
	𐡄𐡍 (uploadpack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK, uploadpack);
	𐡄𐡍 (server_options.nr)
		transport->server_options = &server_options;

	ref = transport_get_remote_refs(transport, &transport_options);
	𐡄𐡍 (ref) {
		صحيح hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
		repo_set_hash_algo(the_repository, hash_algo);
	}

	𐡄𐡍 (!dest && !quiet)
		fprintf(stderr, "From %s\n", remote->url.v[0]);
	لكل ( ; ref; ref = ref->next) {
		بنية ref_array_item *item;
		𐡄𐡍 (!check_ref_type(ref, flags))
			استمر;
		𐡄𐡍 (!tail_match(&pattern, ref->name))
			استمر;
		item = ref_array_push(&ref_array, ref->name, &ref->old_oid, عدم);
		item->symref = xstrdup_or_null(ref->symref);
	}

	sorting = ref_sorting_options(&sorting_options);
	ref_array_sort(sorting, &ref_array);

	لكل (i = 0; i < ref_array.nr; i++) {
		ثابت بنية ref_array_item *ref = ref_array.items[i];
		𐡄𐡍 (show_symref_target && ref->symref)
			printf("ref: %s\t%s\n", ref->symref, ref->refname);
		printf("%s\t%s\n", oid_to_hex(&ref->objectname), ref->refname);
		status = 0; /* we found something */
	}

	string_list_clear(&server_options, 0);
	ref_sorting_release(sorting);
	ref_array_clear(&ref_array);
	𐡄𐡍 (transport_disconnect(transport))
		status = 1;
	transport_ls_refs_options_release(&transport_options);

	strvec_clear(&pattern);
	string_list_clear(&server_options, 0);
	أرجع status;
}
