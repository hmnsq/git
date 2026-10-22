#تعریف_کن USE_THE_REPOSITORY_VARIABLE
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
ساكن صحيح دم_تطابق(ثابت بنية strvec *الگو, ثابت حرف *مسیر)
{
	حرف *pathbuf;

	إذا (!الگو->nr)
		أرجع 1; /* no restriction */

	pathbuf = xstrfmt("/%s", مسیر);
	لكل (اندازه_t i = 0; i < الگو->nr; i++) {
		إذا (!wildmatch(الگو->v[i], pathbuf, 0)) {
			حرر(pathbuf);
			أرجع 1;
		}
	}
	حرر(pathbuf);
	أرجع 0;
}

صحيح cmd_ls_remote(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *پیشوند,
		  بنية repository *repo استفادهنشده)
{
	ثابت حرف *dest = عدم;
	بیعلامت flags = 0;
	صحيح get_url = 0;
	صحيح quiet = 0;
	صحيح وضعیت = 0;
	صحيح show_symref_هدف = 0;
	ثابت حرف *uploadpack = عدم;
	بنية strvec الگو = STRVEC_INIT;
	بنية transport_ls_refs_options transport_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	صحيح i;
	بنية string_list server_options = STRING_LIST_INIT_DUP;

	بنية remote *remote;
	بنية transport *transport;
	ثابت بنية ref *ref;
	بنية ref_array ref_array;
	بنية ref_ترتیبدهی *ترتیبدهی;
	بنية string_list ترتیبدهی_options = STRING_LIST_INIT_DUP;

	بنية option options[] = {
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
		OPT_بیت('t', "tags", &flags, N_("limit to tags"), REF_برچسبها),
		OPT_بیت('b', "branches", &flags, N_("limit to branches"), REF_BRANCHES),
		OPT_بیت_F('h', "heads", &flags,
			  N_("deprecated synonym for --branches"), REF_BRANCHES,
			  PARSE_OPT_HIDDEN),
		OPT_بیت(0, "refs", &flags, N_("do not show peeled tags"), REF_نرمال),
		OPT_BOOL(0, "get-url", &get_url,
			 N_("take url.<base>.insteadOf into account")),
		OPT_REF_مرتب_سازی(&ترتیبدهی_options),
		OPT_مجموعه_INT_F(0, "exit-code", &وضعیت,
			      N_("exit with exit code 2 if no matching refs are found"),
			      2, PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "symref", &show_symref_هدف,
			 N_("show underlying ref in addition to the object pointed by it")),
		OPT_STRING_LIST('o', "server-option", &server_options, N_("server-specific"), N_("option to transmit")),
		OPT_انتها()
	};

	memset(&ref_array, 0, حجم(ref_array));

	argc = parse_options(argc, argv, پیشوند, options, ls_remote_usage,
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
	إذا (!the_repository->hash_algo)
		repo_مجموعه_hash_algo(the_repository, GIT_HASH_DEFAULT);

	packet_trace_عنصر_خنثی("ls-remote");

	لكل (صحيح i = 1; i < argc; i++)
		strvec_pushf(&الگو, "*/%s", argv[i]);

	إذا (flags & REF_برچسبها)
		strvec_افزودن(&transport_options.ref_prefixes, "refs/tags/");
	إذا (flags & REF_BRANCHES)
		strvec_افزودن(&transport_options.ref_prefixes, "refs/heads/");

	remote = remote_get(dest);
	إذا (!remote) {
		إذا (dest)
			die("bad repository '%s'", dest);
		die("No remote configured to list refs from.");
	}

	إذا (get_url) {
		چاپ_کن("%s\n", remote->url.v[0]);
		أرجع 0;
	}

	transport = transport_get(remote, عدم);
	إذا (uploadpack)
		transport_مجموعه_option(transport, TRANS_OPT_UPLOADPACK, uploadpack);
	إذا (server_options.nr)
		transport->server_options = &server_options;

	ref = transport_get_remote_refs(transport, &transport_options);
	إذا (ref) {
		صحيح hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
		repo_مجموعه_hash_algo(the_repository, hash_algo);
	}

	إذا (!dest && !quiet)
		fprintf(stderr, "From %s\n", remote->url.v[0]);
	لكل ( ; ref; ref = ref->بعدی) {
		بنية ref_array_قلم *قلم;
		إذا (!check_ref_type(ref, flags))
			استمر;
		إذا (!دم_تطابق(&الگو, ref->name))
			استمر;
		قلم = ref_array_افزودن(&ref_array, ref->name, &ref->old_oid, عدم);
		قلم->symref = xstrdup_or_null(ref->symref);
	}

	ترتیبدهی = ref_ترتیبدهی_options(&ترتیبدهی_options);
	ref_array_مرتب_سازی(ترتیبدهی, &ref_array);

	لكل (i = 0; i < ref_array.nr; i++) {
		ثابت بنية ref_array_قلم *ref = ref_array.اقلام[i];
		إذا (show_symref_هدف && ref->symref)
			چاپ_کن("ref: %s\t%s\n", ref->symref, ref->refname);
		چاپ_کن("%s\t%s\n", oid_مقصد_hex(&ref->objectname), ref->refname);
		وضعیت = 0; /* we found something */
	}

	string_list_clear(&server_options, 0);
	ref_ترتیبدهی_release(ترتیبدهی);
	ref_array_clear(&ref_array);
	إذا (transport_disconnect(transport))
		وضعیت = 1;
	transport_ls_refs_options_release(&transport_options);

	strvec_clear(&الگو);
	string_list_clear(&server_options, 0);
	أرجع وضعیت;
}
