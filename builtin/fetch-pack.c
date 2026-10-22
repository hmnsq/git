#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "object-file.h"
#شامل "pkt-line.h"
#شامل "fetch-pack.h"
#شامل "remote.h"
#شامل "connect.h"
#شامل "oid-array.h"
#شامل "protocol.h"

ساکن ثابت حرف fetch_pack_usage[] =
"git fetch-pack [--all] [--stdin] [--quiet | -q] [--keep | -k] [--thin] "
"[--include-tag] [--upload-pack=<git-upload-pack>] [--depth=<n>] "
"[--no-progress] [--diag-url] [-v] [<host>:]<directory> [<refs>...]";

ساکن خالی جوڑو_sought_اندراج(ساخت ref ***sought, صحیح *nr, صحیح *alloc,
			     ثابت حرف *name)
{
	ساخت ref *ref;
	ساخت object_id oid;
	ثابت حرف *p;

	اگر (!parse_oid_hex(name, &oid, &p)) {
		اگر (*p == ' ') {
			/* <oid> <ref>, find refname */
			name = p + 1;
		} ورنہ اگر (*p == '\0') {
			; /* <oid>, leave oid as name */
		} ورنہ {
			/* <ref>, clear cruft from oid */
			oidclr(&oid, the_repository->hash_algo);
		}
	} ورنہ {
		/* <ref>, clear cruft from get_oid_hex */
		oidclr(&oid, the_repository->hash_algo);
	}

	ref = alloc_ref(name);
	oidcpy(&ref->old_oid, &oid);
	(*nr)++;
	ALLOC_GROW(*sought, *nr, *alloc);
	(*sought)[*nr - 1] = ref;
}

صحیح cmd_fetch_pack(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *سابقہ غیر_مستعمل,
		   ساخت repository *repo)
{
	صحیح i, ret;
	ساخت ref *fetched_refs = تہی, *remote_refs = تہی;
	ثابت حرف *dest = تہی;
	ساخت ref **sought = تہی;
	ساخت ref **sought_مقصد_free = تہی;
	صحیح nr_sought = 0, alloc_sought = 0;
	صحیح fd[2];
	ساخت string_list pack_lockfiles = STRING_LIST_INIT_DUP;
	ساخت string_list *pack_lockfiles_ptr = تہی;
	ساخت بچہ_process *conn;
	ساخت fetch_pack_دلائل دلائل;
	ساخت oid_array shallow = OID_ARRAY_INIT;
	ساخت string_list deepen_not = STRING_LIST_INIT_DUP;
	ساخت packet_reader reader;
	شمار protocol_نسخہ نسخہ;

	packet_trace_شناختی_عنصر("fetch-pack");

	memset(&دلائل, 0, حجم(دلائل));
	list_objects_filter_init(&دلائل.filter_options);
	دلائل.uploadpack = "git-upload-pack";

	show_usage_if_asked(argc, argv, fetch_pack_usage);

	repo->fetch_if_missing = 0;

	برائے (i = 1; i < argc && *argv[i] == '-'; i++) {
		ثابت حرف *arg = argv[i];

		اگر (skip_سابقہ(arg, "--upload-pack=", &arg)) {
			دلائل.uploadpack = arg;
			جاری;
		}
		اگر (skip_سابقہ(arg, "--exec=", &arg)) {
			دلائل.uploadpack = arg;
			جاری;
		}
		اگر (!strcmp("--quiet", arg) || !strcmp("-q", arg)) {
			دلائل.quiet = 1;
			جاری;
		}
		اگر (!strcmp("--keep", arg) || !strcmp("-k", arg)) {
			دلائل.تالا_pack = دلائل.keep_pack;
			دلائل.keep_pack = 1;
			جاری;
		}
		اگر (!strcmp("--thin", arg)) {
			دلائل.use_thin_pack = 1;
			جاری;
		}
		اگر (!strcmp("--include-tag", arg)) {
			دلائل.include_لیبل = 1;
			جاری;
		}
		اگر (!strcmp("--all", arg)) {
			دلائل.fetch_all = 1;
			جاری;
		}
		اگر (!strcmp("--stdin", arg)) {
			دلائل.stdin_refs = 1;
			جاری;
		}
		اگر (!strcmp("--diag-url", arg)) {
			دلائل.diag_url = 1;
			جاری;
		}
		اگر (!strcmp("-v", arg)) {
			دلائل.verbose = 1;
			جاری;
		}
		اگر (skip_سابقہ(arg, "--depth=", &arg)) {
			دلائل.گہرائی = strtol(arg, تہی, 0);
			جاری;
		}
		اگر (skip_سابقہ(arg, "--shallow-since=", &arg)) {
			دلائل.deepen_since = xstrdup(arg);
			جاری;
		}
		اگر (skip_سابقہ(arg, "--shallow-exclude=", &arg)) {
			string_list_append(&deepen_not, arg);
			جاری;
		}
		اگر (!strcmp(arg, "--deepen-relative")) {
			دلائل.deepen_relative = 1;
			جاری;
		}
		اگر (!strcmp("--no-progress", arg)) {
			دلائل.no_پیش_رفت = 1;
			جاری;
		}
		اگر (!strcmp("--stateless-rpc", arg)) {
			دلائل.stateless_rpc = 1;
			جاری;
		}
		اگر (!strcmp("--lock-pack", arg)) {
			دلائل.تالا_pack = 1;
			pack_lockfiles_ptr = &pack_lockfiles;
			جاری;
		}
		اگر (!strcmp("--check-self-contained-and-connected", arg)) {
			دلائل.check_self_contained_and_connected = 1;
			جاری;
		}
		اگر (!strcmp("--cloning", arg)) {
			دلائل.cloning = 1;
			جاری;
		}
		اگر (!strcmp("--update-shallow", arg)) {
			دلائل.update_shallow = 1;
			جاری;
		}
		اگر (!strcmp("--from-promisor", arg)) {
			دلائل.from_promisor = 1;
			جاری;
		}
		اگر (!strcmp("--refetch", arg)) {
			دلائل.refetch = 1;
			جاری;
		}
		اگر (skip_سابقہ(arg, ("--filter="), &arg)) {
			parse_list_objects_filter(&دلائل.filter_options, arg);
			جاری;
		}
		اگر (!strcmp(arg, ("--no-filter"))) {
			list_objects_filter_سیٹ_no_filter(&دلائل.filter_options);
			جاری;
		}
		usage(fetch_pack_usage);
	}
	اگر (deepen_not.nr)
		دلائل.deepen_not = &deepen_not;

	اگر (i < argc)
		dest = argv[i++];
	ورنہ
		usage(fetch_pack_usage);

	/*
	 * Copy refs from cmdline to growable list, then append any
	 * refs from the standard input:
	 */
	برائے (; i < argc; i++)
		جوڑو_sought_اندراج(&sought, &nr_sought, &alloc_sought, argv[i]);
	اگر (دلائل.stdin_refs) {
		اگر (دلائل.stateless_rpc) {
			/* in stateless RPC mode we use pkt-line to read
			 * from stdin, until we get a flush packet
			 */
			برائے (;;) {
				حرف *سطر = packet_read_سطر(0, تہی);
				اگر (!سطر)
					توڑو;
				جوڑو_sought_اندراج(&sought, &nr_sought,  &alloc_sought, سطر);
			}
		}
		ورنہ {
			/* read from stdin one ref per line, until EOF */
			ساخت strbuf سطر = STRBUF_INIT;
			جب (strbuf_getline_lf(&سطر, stdin) != EOF)
				جوڑو_sought_اندراج(&sought, &nr_sought, &alloc_sought, سطر.buf);
			strbuf_release(&سطر);
		}
	}

	اگر (دلائل.stateless_rpc) {
		conn = تہی;
		fd[0] = 0;
		fd[1] = 1;
	} ورنہ {
		صحیح flags = دلائل.verbose ? CONNECT_VERBOSE : 0;
		اگر (دلائل.diag_url)
			flags |= CONNECT_DIAG_URL;
		conn = git_connect(fd, dest, GIT_CONNECT_UPLOAD_PACK,
				   دلائل.uploadpack, flags);
		اگر (!conn)
			واپس دلائل.diag_url ? 0 : 1;
	}

	packet_reader_init(&reader, fd[0], تہی, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	نسخہ = discover_نسخہ(&reader);
	انتخاب (نسخہ) {
	حالت protocol_v2:
		get_remote_refs(fd[1], &reader, &remote_refs, 0, تہی, تہی,
				دلائل.stateless_rpc);
		توڑو;
	حالت protocol_v1:
	حالت protocol_v0:
		get_remote_heads(&reader, &remote_refs, 0, تہی, &shallow);
		توڑو;
	حالت protocol_unknown_نسخہ:
		BUG("unknown protocol version");
	}

	/*
	 * Create a shallow copy of `sought` so that we can free all of its entries.
	 * This is because `fetch_pack()` will modify the array to evict some
	 * entries, but won't free those.
	 */
	DUP_ARRAY(sought_مقصد_free, sought, nr_sought);

	fetched_refs = fetch_pack(&دلائل, fd, remote_refs, sought, nr_sought,
			 &shallow, pack_lockfiles_ptr, نسخہ);

	اگر (pack_lockfiles.nr) {
		صحیح i;

		لکھو("lock %s\n", pack_lockfiles.اشیاء[0].string);
		fflush(stdout);
		برائے (i = 1; i < pack_lockfiles.nr; i++)
			warning(_("Lockfile created but not reported: %s"),
				pack_lockfiles.اشیاء[i].string);
	}
	اگر (دلائل.check_self_contained_and_connected &&
	    دلائل.self_contained_and_connected) {
		لکھو("connectivity-ok\n");
		fflush(stdout);
	}
	close(fd[0]);
	close(fd[1]);
	اگر (اختتام_connect(conn)) {
		ret = 1;
		جاؤ cleanup;
	}

	ret = !fetched_refs;

	/*
	 * If the heads to pull were given, we should have consumed
	 * all of them by matching the remote.  Otherwise, 'git fetch
	 * remote no-such-ref' would silently succeed without issuing
	 * an error.
	 */
	ret |= report_unmatched_refs(sought, nr_sought);

	برائے (ساخت ref *ref = fetched_refs; ref; ref = ref->اگلا)
		لکھو("%s %s\n",
		       oid_مقصد_hex(&ref->old_oid), ref->name);

cleanup:
	برائے (سائز_t i = 0; i < nr_sought; i++)
		free_one_ref(sought_مقصد_free[i]);
	آزاد(sought_مقصد_free);
	آزاد(sought);
	free_refs(fetched_refs);
	free_refs(remote_refs);
	list_objects_filter_release(&دلائل.filter_options);
	oid_array_clear(&shallow);
	string_list_clear(&pack_lockfiles, 0);
	واپس ret;
}
