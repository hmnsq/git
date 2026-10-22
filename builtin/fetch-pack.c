#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "object-file.h"
#ضمن "pkt-line.h"
#ضمن "fetch-pack.h"
#ضمن "remote.h"
#ضمن "connect.h"
#ضمن "oid-array.h"
#ضمن "protocol.h"

ساكن ثابت حرف fetch_pack_usage[] =
"git fetch-pack [--all] [--stdin] [--quiet | -q] [--keep | -k] [--thin] "
"[--include-tag] [--upload-pack=<git-upload-pack>] [--depth=<n>] "
"[--no-progress] [--diag-url] [-v] [<host>:]<directory> [<refs>...]";

ساكن فراغ جوڙيو_sought_داخلا(بنية ref ***sought, صحيح *nr, صحيح *alloc,
			     ثابت حرف *name)
{
	بنية ref *ref;
	بنية object_id oid;
	ثابت حرف *p;

	إذا (!parse_oid_hex(name, &oid, &p)) {
		إذا (*p == ' ') {
			/* <oid> <ref>, find refname */
			name = p + 1;
		} وإلا إذا (*p == '\0') {
			; /* <oid>, leave oid as name */
		} وإلا {
			/* <ref>, clear cruft from oid */
			oidclr(&oid, the_repository->hash_algo);
		}
	} وإلا {
		/* <ref>, clear cruft from get_oid_hex */
		oidclr(&oid, the_repository->hash_algo);
	}

	ref = alloc_ref(name);
	oidcpy(&ref->old_oid, &oid);
	(*nr)++;
	ALLOC_GROW(*sought, *nr, *alloc);
	(*sought)[*nr - 1] = ref;
}

صحيح cmd_fetch_pack(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل,
		   بنية repository *repo)
{
	صحيح i, ret;
	بنية ref *fetched_refs = عدم, *remote_refs = عدم;
	ثابت حرف *dest = عدم;
	بنية ref **sought = عدم;
	بنية ref **sought_منزل_free = عدم;
	صحيح nr_sought = 0, alloc_sought = 0;
	صحيح fd[2];
	بنية string_list pack_lockfiles = STRING_LIST_INIT_DUP;
	بنية string_list *pack_lockfiles_ptr = عدم;
	بنية ٻار_process *conn;
	بنية fetch_pack_دليلون دليلون;
	بنية oid_array shallow = OID_ARRAY_INIT;
	بنية string_list deepen_not = STRING_LIST_INIT_DUP;
	بنية packet_reader reader;
	تعداد protocol_نسخو نسخو;

	packet_trace_سڃاڻپ_عنصر("fetch-pack");

	memset(&دليلون, 0, حجم(دليلون));
	list_objects_filter_init(&دليلون.filter_options);
	دليلون.uploadpack = "git-upload-pack";

	show_usage_if_asked(argc, argv, fetch_pack_usage);

	repo->fetch_if_missing = 0;

	لكل (i = 1; i < argc && *argv[i] == '-'; i++) {
		ثابت حرف *arg = argv[i];

		إذا (skip_اڳياڙي(arg, "--upload-pack=", &arg)) {
			دليلون.uploadpack = arg;
			استمر;
		}
		إذا (skip_اڳياڙي(arg, "--exec=", &arg)) {
			دليلون.uploadpack = arg;
			استمر;
		}
		إذا (!strcmp("--quiet", arg) || !strcmp("-q", arg)) {
			دليلون.quiet = 1;
			استمر;
		}
		إذا (!strcmp("--keep", arg) || !strcmp("-k", arg)) {
			دليلون.تالو_pack = دليلون.keep_pack;
			دليلون.keep_pack = 1;
			استمر;
		}
		إذا (!strcmp("--thin", arg)) {
			دليلون.use_thin_pack = 1;
			استمر;
		}
		إذا (!strcmp("--include-tag", arg)) {
			دليلون.include_ٽيگ = 1;
			استمر;
		}
		إذا (!strcmp("--all", arg)) {
			دليلون.fetch_all = 1;
			استمر;
		}
		إذا (!strcmp("--stdin", arg)) {
			دليلون.stdin_refs = 1;
			استمر;
		}
		إذا (!strcmp("--diag-url", arg)) {
			دليلون.diag_url = 1;
			استمر;
		}
		إذا (!strcmp("-v", arg)) {
			دليلون.verbose = 1;
			استمر;
		}
		إذا (skip_اڳياڙي(arg, "--depth=", &arg)) {
			دليلون.اونهائي = strtol(arg, عدم, 0);
			استمر;
		}
		إذا (skip_اڳياڙي(arg, "--shallow-since=", &arg)) {
			دليلون.deepen_since = xstrdup(arg);
			استمر;
		}
		إذا (skip_اڳياڙي(arg, "--shallow-exclude=", &arg)) {
			string_list_append(&deepen_not, arg);
			استمر;
		}
		إذا (!strcmp(arg, "--deepen-relative")) {
			دليلون.deepen_relative = 1;
			استمر;
		}
		إذا (!strcmp("--no-progress", arg)) {
			دليلون.no_ترقي = 1;
			استمر;
		}
		إذا (!strcmp("--stateless-rpc", arg)) {
			دليلون.stateless_rpc = 1;
			استمر;
		}
		إذا (!strcmp("--lock-pack", arg)) {
			دليلون.تالو_pack = 1;
			pack_lockfiles_ptr = &pack_lockfiles;
			استمر;
		}
		إذا (!strcmp("--check-self-contained-and-connected", arg)) {
			دليلون.check_self_contained_and_connected = 1;
			استمر;
		}
		إذا (!strcmp("--cloning", arg)) {
			دليلون.cloning = 1;
			استمر;
		}
		إذا (!strcmp("--update-shallow", arg)) {
			دليلون.update_shallow = 1;
			استمر;
		}
		إذا (!strcmp("--from-promisor", arg)) {
			دليلون.from_promisor = 1;
			استمر;
		}
		إذا (!strcmp("--refetch", arg)) {
			دليلون.refetch = 1;
			استمر;
		}
		إذا (skip_اڳياڙي(arg, ("--filter="), &arg)) {
			parse_list_objects_filter(&دليلون.filter_options, arg);
			استمر;
		}
		إذا (!strcmp(arg, ("--no-filter"))) {
			list_objects_filter_سيٽ_no_filter(&دليلون.filter_options);
			استمر;
		}
		usage(fetch_pack_usage);
	}
	إذا (deepen_not.nr)
		دليلون.deepen_not = &deepen_not;

	إذا (i < argc)
		dest = argv[i++];
	وإلا
		usage(fetch_pack_usage);

	/*
	 * Copy refs from cmdline to growable list, then append any
	 * refs from the standard input:
	 */
	لكل (; i < argc; i++)
		جوڙيو_sought_داخلا(&sought, &nr_sought, &alloc_sought, argv[i]);
	إذا (دليلون.stdin_refs) {
		إذا (دليلون.stateless_rpc) {
			/* in stateless RPC mode we use pkt-line to read
			 * from stdin, until we get a flush packet
			 */
			لكل (;;) {
				حرف *سٽ = packet_read_سٽ(0, عدم);
				إذا (!سٽ)
					اكسر;
				جوڙيو_sought_داخلا(&sought, &nr_sought,  &alloc_sought, سٽ);
			}
		}
		وإلا {
			/* read from stdin one ref per line, until EOF */
			بنية strbuf سٽ = STRBUF_INIT;
			طالما (strbuf_getline_lf(&سٽ, stdin) != EOF)
				جوڙيو_sought_داخلا(&sought, &nr_sought, &alloc_sought, سٽ.buf);
			strbuf_release(&سٽ);
		}
	}

	إذا (دليلون.stateless_rpc) {
		conn = عدم;
		fd[0] = 0;
		fd[1] = 1;
	} وإلا {
		صحيح flags = دليلون.verbose ? CONNECT_VERBOSE : 0;
		إذا (دليلون.diag_url)
			flags |= CONNECT_DIAG_URL;
		conn = git_connect(fd, dest, GIT_CONNECT_UPLOAD_PACK,
				   دليلون.uploadpack, flags);
		إذا (!conn)
			أرجع دليلون.diag_url ? 0 : 1;
	}

	packet_reader_init(&reader, fd[0], عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	نسخو = discover_نسخو(&reader);
	بدل (نسخو) {
	حالة protocol_v2:
		get_remote_refs(fd[1], &reader, &remote_refs, 0, عدم, عدم,
				دليلون.stateless_rpc);
		اكسر;
	حالة protocol_v1:
	حالة protocol_v0:
		get_remote_heads(&reader, &remote_refs, 0, عدم, &shallow);
		اكسر;
	حالة protocol_unknown_نسخو:
		BUG("unknown protocol version");
	}

	/*
	 * Create a shallow copy of `sought` so that we can free all of its entries.
	 * This is because `fetch_pack()` will modify the array to evict some
	 * entries, but won't free those.
	 */
	DUP_ARRAY(sought_منزل_free, sought, nr_sought);

	fetched_refs = fetch_pack(&دليلون, fd, remote_refs, sought, nr_sought,
			 &shallow, pack_lockfiles_ptr, نسخو);

	إذا (pack_lockfiles.nr) {
		صحيح i;

		لکو("lock %s\n", pack_lockfiles.شيون[0].string);
		fflush(stdout);
		لكل (i = 1; i < pack_lockfiles.nr; i++)
			warning(_("Lockfile created but not reported: %s"),
				pack_lockfiles.شيون[i].string);
	}
	إذا (دليلون.check_self_contained_and_connected &&
	    دليلون.self_contained_and_connected) {
		لکو("connectivity-ok\n");
		fflush(stdout);
	}
	close(fd[0]);
	close(fd[1]);
	إذا (پڄاڻي_connect(conn)) {
		ret = 1;
		اذهب cleanup;
	}

	ret = !fetched_refs;

	/*
	 * If the heads to pull were given, we should have consumed
	 * all of them by matching the remote.  Otherwise, 'git fetch
	 * remote no-such-ref' would silently succeed without issuing
	 * an error.
	 */
	ret |= report_unmatched_refs(sought, nr_sought);

	لكل (بنية ref *ref = fetched_refs; ref; ref = ref->ايندڙ)
		لکو("%s %s\n",
		       oid_منزل_hex(&ref->old_oid), ref->name);

cleanup:
	لكل (سائيز_t i = 0; i < nr_sought; i++)
		free_one_ref(sought_منزل_free[i]);
	حرر(sought_منزل_free);
	حرر(sought);
	free_refs(fetched_refs);
	free_refs(remote_refs);
	list_objects_filter_release(&دليلون.filter_options);
	oid_array_clear(&shallow);
	string_list_clear(&pack_lockfiles, 0);
	أرجع ret;
}
