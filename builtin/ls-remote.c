#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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
ساكن صحيح پوچھ_میل(ثابت بنية strvec *نمونہ, ثابت حرف *رستہ)
{
	حرف *pathbuf;

	إذا (!نمونہ->nr)
		أرجع 1; /* no restriction */

	pathbuf = xstrfmt("/%s", رستہ);
	لكل (ناپ_t i = 0; i < نمونہ->nr; i++) {
		إذا (!wildmatch(نمونہ->v[i], pathbuf, 0)) {
			حرر(pathbuf);
			أرجع 1;
		}
	}
	حرر(pathbuf);
	أرجع 0;
}

صحيح cmd_ls_remote(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *اگیتر,
		  بنية repository *repo بن_ورتیا)
{
	ثابت حرف *dest = عدم;
	بنانشانی flags = 0;
	صحيح get_url = 0;
	صحيح quiet = 0;
	صحيح حالت = 0;
	صحيح show_symref_نشانہ = 0;
	ثابت حرف *uploadpack = عدم;
	بنية strvec نمونہ = STRVEC_INIT;
	بنية transport_ls_refs_options transport_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	صحيح i;
	بنية string_list server_options = STRING_LIST_INIT_DUP;

	بنية remote *remote;
	بنية transport *transport;
	ثابت بنية ref *ref;
	بنية ref_array ref_array;
	بنية ref_چھانٹی *چھانٹی;
	بنية string_list چھانٹی_options = STRING_LIST_INIT_DUP;

	بنية option options[] = {
		OPT__QUIET(&quiet, N_("do not print remote URL")),
		OPT_STRING(0, "upload-pack", &uploadpack, N_("exec"),
			   N_("path of git-upload-pack on the remote host")),
		{
			.type = OPTION_STRING,
			.long_name = "exec",
			.قدر = &uploadpack,
			.argh = N_("exec"),
			.help = N_("path of git-upload-pack on the remote host"),
			.flags = PARSE_OPT_HIDDEN,
		},
		OPT_ثنائی_ہندسہ('t', "tags", &flags, N_("limit to tags"), REF_ٹیگ_سوچی),
		OPT_ثنائی_ہندسہ('b', "branches", &flags, N_("limit to branches"), REF_BRANCHES),
		OPT_ثنائی_ہندسہ_F('h', "heads", &flags,
			  N_("deprecated synonym for --branches"), REF_BRANCHES,
			  PARSE_OPT_HIDDEN),
		OPT_ثنائی_ہندسہ(0, "refs", &flags, N_("do not show peeled tags"), REF_عام),
		OPT_BOOL(0, "get-url", &get_url,
			 N_("take url.<base>.insteadOf into account")),
		OPT_REF_ترتیب_دیو(&چھانٹی_options),
		OPT_سیٹ_INT_F(0, "exit-code", &حالت,
			      N_("exit with exit code 2 if no matching refs are found"),
			      2, PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "symref", &show_symref_نشانہ,
			 N_("show underlying ref in addition to the object pointed by it")),
		OPT_STRING_LIST('o', "server-option", &server_options, N_("server-specific"), N_("option to transmit")),
		OPT_سرا()
	};

	memset(&ref_array, 0, حجم(ref_array));

	argc = parse_options(argc, argv, اگیتر, options, ls_remote_usage,
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
		repo_سیٹ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	packet_trace_پچھان_تت("ls-remote");

	لكل (صحيح i = 1; i < argc; i++)
		strvec_pushf(&نمونہ, "*/%s", argv[i]);

	إذا (flags & REF_ٹیگ_سوچی)
		strvec_ڈالو(&transport_options.ref_prefixes, "refs/tags/");
	إذا (flags & REF_BRANCHES)
		strvec_ڈالو(&transport_options.ref_prefixes, "refs/heads/");

	remote = remote_get(dest);
	إذا (!remote) {
		إذا (dest)
			die("bad repository '%s'", dest);
		die("No remote configured to list refs from.");
	}

	إذا (get_url) {
		لکھو("%s\n", remote->url.v[0]);
		أرجع 0;
	}

	transport = transport_get(remote, عدم);
	إذا (uploadpack)
		transport_سیٹ_option(transport, TRANS_OPT_UPLOADPACK, uploadpack);
	إذا (server_options.nr)
		transport->server_options = &server_options;

	ref = transport_get_remote_refs(transport, &transport_options);
	إذا (ref) {
		صحيح hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
		repo_سیٹ_hash_algo(the_repository, hash_algo);
	}

	إذا (!dest && !quiet)
		fprintf(stderr, "From %s\n", remote->url.v[0]);
	لكل ( ; ref; ref = ref->اگلا) {
		بنية ref_array_شے *شے;
		إذا (!check_ref_type(ref, flags))
			استمر;
		إذا (!پوچھ_میل(&نمونہ, ref->name))
			استمر;
		شے = ref_array_ڈالو(&ref_array, ref->name, &ref->old_oid, عدم);
		شے->symref = xstrdup_or_null(ref->symref);
	}

	چھانٹی = ref_چھانٹی_options(&چھانٹی_options);
	ref_array_ترتیب_دیو(چھانٹی, &ref_array);

	لكل (i = 0; i < ref_array.nr; i++) {
		ثابت بنية ref_array_شے *ref = ref_array.اشیاء[i];
		إذا (show_symref_نشانہ && ref->symref)
			لکھو("ref: %s\t%s\n", ref->symref, ref->refname);
		لکھو("%s\t%s\n", oid_منزل_hex(&ref->objectname), ref->refname);
		حالت = 0; /* we found something */
	}

	string_list_clear(&server_options, 0);
	ref_چھانٹی_release(چھانٹی);
	ref_array_clear(&ref_array);
	إذا (transport_disconnect(transport))
		حالت = 1;
	transport_ls_refs_options_release(&transport_options);

	strvec_clear(&نمونہ);
	string_list_clear(&server_options, 0);
	أرجع حالت;
}
