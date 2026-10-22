#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "transport.h"
#شامل "pkt-line.h"
#شامل "ref-filter.h"
#شامل "remote.h"
#شامل "parse-options.h"
#شامل "wildmatch.h"

ساکن ثابت حرف * ثابت ls_remote_usage[] = {
	N_("git ls-remote [--branches] [--tags] [--refs] [--upload-pack=<exec>]\n"
	   "              [-q | --quiet] [--exit-code] [--get-url] [--sort=<key>]\n"
	   "              [--symref] [<repository> [<patterns>...]]"),
	تہی
};

/*
 * Is there one among the list of patterns that match the tail part
 * of the path?
 */
ساکن صحیح دم_مطابقت(ثابت ساخت strvec *نمونہ, ثابت حرف *راستہ)
{
	حرف *pathbuf;

	اگر (!نمونہ->nr)
		واپس 1; /* no restriction */

	pathbuf = xstrfmt("/%s", راستہ);
	برائے (سائز_t i = 0; i < نمونہ->nr; i++) {
		اگر (!wildmatch(نمونہ->v[i], pathbuf, 0)) {
			آزاد(pathbuf);
			واپس 1;
		}
	}
	آزاد(pathbuf);
	واپس 0;
}

صحیح cmd_ls_remote(صحیح argc,
		  ثابت حرف **argv,
		  ثابت حرف *سابقہ,
		  ساخت repository *repo غیر_مستعمل)
{
	ثابت حرف *dest = تہی;
	غیرعلامتی flags = 0;
	صحیح get_url = 0;
	صحیح quiet = 0;
	صحیح حالت_2 = 0;
	صحیح show_symref_ہدف = 0;
	ثابت حرف *uploadpack = تہی;
	ساخت strvec نمونہ = STRVEC_INIT;
	ساخت transport_ls_refs_options transport_options =
		TRANSPORT_LS_REFS_OPTIONS_INIT;
	صحیح i;
	ساخت string_list server_options = STRING_LIST_INIT_DUP;

	ساخت remote *remote;
	ساخت transport *transport;
	ثابت ساخت ref *ref;
	ساخت ref_array ref_array;
	ساخت ref_ترتیب_کاری *ترتیب_کاری;
	ساخت string_list ترتیب_کاری_options = STRING_LIST_INIT_DUP;

	ساخت option options[] = {
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
		OPT_ثنائی_ہندسہ('t', "tags", &flags, N_("limit to tags"), REF_لیبل_فہرست),
		OPT_ثنائی_ہندسہ('b', "branches", &flags, N_("limit to branches"), REF_BRANCHES),
		OPT_ثنائی_ہندسہ_F('h', "heads", &flags,
			  N_("deprecated synonym for --branches"), REF_BRANCHES,
			  PARSE_OPT_HIDDEN),
		OPT_ثنائی_ہندسہ(0, "refs", &flags, N_("do not show peeled tags"), REF_معمول),
		OPT_BOOL(0, "get-url", &get_url,
			 N_("take url.<base>.insteadOf into account")),
		OPT_REF_ترتیب_دو(&ترتیب_کاری_options),
		OPT_سیٹ_INT_F(0, "exit-code", &حالت_2,
			      N_("exit with exit code 2 if no matching refs are found"),
			      2, PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "symref", &show_symref_ہدف,
			 N_("show underlying ref in addition to the object pointed by it")),
		OPT_STRING_LIST('o', "server-option", &server_options, N_("server-specific"), N_("option to transmit")),
		OPT_انتہا()
	};

	memset(&ref_array, 0, حجم(ref_array));

	argc = parse_options(argc, argv, سابقہ, options, ls_remote_usage,
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
		repo_سیٹ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	packet_trace_شناختی_عنصر("ls-remote");

	برائے (صحیح i = 1; i < argc; i++)
		strvec_pushf(&نمونہ, "*/%s", argv[i]);

	اگر (flags & REF_لیبل_فہرست)
		strvec_ڈالو(&transport_options.ref_prefixes, "refs/tags/");
	اگر (flags & REF_BRANCHES)
		strvec_ڈالو(&transport_options.ref_prefixes, "refs/heads/");

	remote = remote_get(dest);
	اگر (!remote) {
		اگر (dest)
			die("bad repository '%s'", dest);
		die("No remote configured to list refs from.");
	}

	اگر (get_url) {
		لکھو("%s\n", remote->url.v[0]);
		واپس 0;
	}

	transport = transport_get(remote, تہی);
	اگر (uploadpack)
		transport_سیٹ_option(transport, TRANS_OPT_UPLOADPACK, uploadpack);
	اگر (server_options.nr)
		transport->server_options = &server_options;

	ref = transport_get_remote_refs(transport, &transport_options);
	اگر (ref) {
		صحیح hash_algo = hash_algo_by_ptr(transport_get_hash_algo(transport));
		repo_سیٹ_hash_algo(the_repository, hash_algo);
	}

	اگر (!dest && !quiet)
		fprintf(stderr, "From %s\n", remote->url.v[0]);
	برائے ( ; ref; ref = ref->اگلا) {
		ساخت ref_array_شے *شے;
		اگر (!check_ref_type(ref, flags))
			جاری;
		اگر (!دم_مطابقت(&نمونہ, ref->name))
			جاری;
		شے = ref_array_ڈالو(&ref_array, ref->name, &ref->old_oid, تہی);
		شے->symref = xstrdup_or_null(ref->symref);
	}

	ترتیب_کاری = ref_ترتیب_کاری_options(&ترتیب_کاری_options);
	ref_array_ترتیب_دو(ترتیب_کاری, &ref_array);

	برائے (i = 0; i < ref_array.nr; i++) {
		ثابت ساخت ref_array_شے *ref = ref_array.اشیاء[i];
		اگر (show_symref_ہدف && ref->symref)
			لکھو("ref: %s\t%s\n", ref->symref, ref->refname);
		لکھو("%s\t%s\n", oid_مقصد_hex(&ref->objectname), ref->refname);
		حالت_2 = 0; /* we found something */
	}

	string_list_clear(&server_options, 0);
	ref_ترتیب_کاری_release(ترتیب_کاری);
	ref_array_clear(&ref_array);
	اگر (transport_disconnect(transport))
		حالت_2 = 1;
	transport_ls_refs_options_release(&transport_options);

	strvec_clear(&نمونہ);
	string_list_clear(&server_options, 0);
	واپس حالت_2;
}
