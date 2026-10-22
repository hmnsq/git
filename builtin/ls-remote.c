#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "transport.h"
#شامل "pkt-line.h"
#شامل "ref-filter.h"
#شامل "remote.h"
#شامل "parse-options.h"
#شامل "wildmatch.h"

ایستا ثابت نویسه * ثابت ls_remote_usage[] = {
	N_("git ls-remote [--branches] [--tags] [--refs] [--upload-pack=<exec>]\n"
	   "              [-q | --quiet] [--exit-code] [--get-url] [--sort=<key>]\n"
	   "              [--symref] [<repository> [<patterns>...]]"),
	پوچ
};

/*
 * Is there one among the list of patterns that match the tail part
 * of the path?
 */
ایستا صحیح tail_match(ثابت ساختار strvec *pattern, ثابت نویسه *path)
{
	نویسه *pathbuf;

	اگر (!pattern->nr)
		بازگشت 1; /* no restriction */

	pathbuf = xstrfmt("/%s", path);
	برای (size_t i = 0; i < pattern->nr; i++) {
		اگر (!wildmatch(pattern->v[i], pathbuf, 0)) {
			آزاد(pathbuf);
			بازگشت 1;
		}
	}
	آزاد(pathbuf);
	بازگشت 0;
}

صحیح cmd_ls_remote(صحیح argc,
		  ثابت نویسه **argv,
		  ثابت نویسه *prefix,
		  ساختار repository *repo UNUSED)
{
	ثابت نویسه *dest = پوچ;
	unsigned flags = 0;
	صحیح get_url = 0;
	صحیح quiet = 0;
	صحیح status = 0;
	صحیح show_symref_target = 0;
	ثابت نویسه *uploadpack = پوچ;
	ساختار strvec pattern = STRVEC_INIT;
	ساختار transport_ls_refs_options transport_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	صحیح i;
	ساختار string_list server_options = STRING_LIST_INIT_DUP;

	ساختار remote *remote;
	ساختار transport *transport;
	ثابت ساختار ref *ref;
	ساختار ref_array ref_array;
	ساختار ref_sorting *sorting;
	ساختار string_list sorting_options = STRING_LIST_INIT_DUP;

	ساختار option options[] = {
		OPT__QUIET(&quiet, N_("do not print remote URL")),
		OPT_STRING(0, "upload-pack", &uploadpack, N_("exec"),
			   N_("path of git-upload-pack on the remote host")),
		{
			.type = OPTION_STRING,
			.long_name = "exec",
			.مقدار = &uploadpack,
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

	memset(&ref_array, 0, اندازه(ref_array));

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
	اگر (!the_repository->hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	packet_trace_identity("ls-remote");

	برای (صحیح i = 1; i < argc; i++)
		strvec_pushf(&pattern, "*/%s", argv[i]);

	اگر (flags & REF_TAGS)
		strvec_افزودن(&transport_options.ref_prefixes, "refs/tags/");
	اگر (flags & REF_BRANCHES)
		strvec_افزودن(&transport_options.ref_prefixes, "refs/heads/");

	remote = remote_get(dest);
	اگر (!remote) {
		اگر (dest)
			die("bad repository '%s'", dest);
		die("No remote configured to list refs from.");
	}

	اگر (get_url) {
		printf("%s\n", remote->url.v[0]);
		بازگشت 0;
	}

	transport = transport_get(remote, پوچ);
	اگر (uploadpack)
		transport_set_option(transport, TRANS_OPT_UPLOADPACK, uploadpack);
	اگر (server_options.nr)
		transport->server_options = &server_options;

	ref = transport_get_remote_refs(transport, &transport_options);
	اگر (ref) {
		صحیح hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
		repo_set_hash_algo(the_repository, hash_algo);
	}

	اگر (!dest && !quiet)
		fprintf(stderr, "From %s\n", remote->url.v[0]);
	برای ( ; ref; ref = ref->next) {
		ساختار ref_array_item *item;
		اگر (!check_ref_type(ref, flags))
			ادامه;
		اگر (!tail_match(&pattern, ref->name))
			ادامه;
		item = ref_array_افزودن(&ref_array, ref->name, &ref->old_oid, پوچ);
		item->symref = xstrdup_or_null(ref->symref);
	}

	sorting = ref_sorting_options(&sorting_options);
	ref_array_sort(sorting, &ref_array);

	برای (i = 0; i < ref_array.nr; i++) {
		ثابت ساختار ref_array_item *ref = ref_array.اقلام[i];
		اگر (show_symref_target && ref->symref)
			printf("ref: %s\t%s\n", ref->symref, ref->refname);
		printf("%s\t%s\n", oid_to_hex(&ref->objectname), ref->refname);
		status = 0; /* we found something */
	}

	string_list_clear(&server_options, 0);
	ref_sorting_release(sorting);
	ref_array_clear(&ref_array);
	اگر (transport_disconnect(transport))
		status = 1;
	transport_ls_refs_options_release(&transport_options);

	strvec_clear(&pattern);
	string_list_clear(&server_options, 0);
	بازگشت status;
}
