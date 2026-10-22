#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن فراغ add_sought_entry(بنية ref ***sought, صحيح *nr, صحيح *alloc,
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
		   ثابت حرف *prefix UNUSED,
		   بنية repository *repo)
{
	صحيح i, ret;
	بنية ref *fetched_refs = عدم, *remote_refs = عدم;
	ثابت حرف *dest = عدم;
	بنية ref **sought = عدم;
	بنية ref **sought_to_free = عدم;
	صحيح nr_sought = 0, alloc_sought = 0;
	صحيح fd[2];
	بنية string_list pack_lockfiles = STRING_LIST_INIT_DUP;
	بنية string_list *pack_lockfiles_ptr = عدم;
	بنية child_process *conn;
	بنية fetch_pack_وسائط وسائط;
	بنية oid_array shallow = OID_ARRAY_INIT;
	بنية string_list deepen_not = STRING_LIST_INIT_DUP;
	بنية packet_reader reader;
	تعداد protocol_version version;

	packet_trace_identity("fetch-pack");

	memset(&وسائط, 0, حجم(وسائط));
	list_objects_filter_init(&وسائط.filter_options);
	وسائط.uploadpack = "git-upload-pack";

	show_usage_if_asked(argc, argv, fetch_pack_usage);

	repo->fetch_if_missing = 0;

	لكل (i = 1; i < argc && *argv[i] == '-'; i++) {
		ثابت حرف *arg = argv[i];

		إذا (skip_prefix(arg, "--upload-pack=", &arg)) {
			وسائط.uploadpack = arg;
			استمر;
		}
		إذا (skip_prefix(arg, "--exec=", &arg)) {
			وسائط.uploadpack = arg;
			استمر;
		}
		إذا (!strcmp("--quiet", arg) || !strcmp("-q", arg)) {
			وسائط.quiet = 1;
			استمر;
		}
		إذا (!strcmp("--keep", arg) || !strcmp("-k", arg)) {
			وسائط.lock_pack = وسائط.keep_pack;
			وسائط.keep_pack = 1;
			استمر;
		}
		إذا (!strcmp("--thin", arg)) {
			وسائط.use_thin_pack = 1;
			استمر;
		}
		إذا (!strcmp("--include-tag", arg)) {
			وسائط.include_tag = 1;
			استمر;
		}
		إذا (!strcmp("--all", arg)) {
			وسائط.fetch_all = 1;
			استمر;
		}
		إذا (!strcmp("--stdin", arg)) {
			وسائط.stdin_refs = 1;
			استمر;
		}
		إذا (!strcmp("--diag-url", arg)) {
			وسائط.diag_url = 1;
			استمر;
		}
		إذا (!strcmp("-v", arg)) {
			وسائط.verbose = 1;
			استمر;
		}
		إذا (skip_prefix(arg, "--depth=", &arg)) {
			وسائط.depth = strtol(arg, عدم, 0);
			استمر;
		}
		إذا (skip_prefix(arg, "--shallow-since=", &arg)) {
			وسائط.deepen_since = xstrdup(arg);
			استمر;
		}
		إذا (skip_prefix(arg, "--shallow-exclude=", &arg)) {
			string_list_append(&deepen_not, arg);
			استمر;
		}
		إذا (!strcmp(arg, "--deepen-relative")) {
			وسائط.deepen_relative = 1;
			استمر;
		}
		إذا (!strcmp("--no-progress", arg)) {
			وسائط.no_progress = 1;
			استمر;
		}
		إذا (!strcmp("--stateless-rpc", arg)) {
			وسائط.stateless_rpc = 1;
			استمر;
		}
		إذا (!strcmp("--lock-pack", arg)) {
			وسائط.lock_pack = 1;
			pack_lockfiles_ptr = &pack_lockfiles;
			استمر;
		}
		إذا (!strcmp("--check-self-contained-and-connected", arg)) {
			وسائط.check_self_contained_and_connected = 1;
			استمر;
		}
		إذا (!strcmp("--cloning", arg)) {
			وسائط.cloning = 1;
			استمر;
		}
		إذا (!strcmp("--update-shallow", arg)) {
			وسائط.update_shallow = 1;
			استمر;
		}
		إذا (!strcmp("--from-promisor", arg)) {
			وسائط.from_promisor = 1;
			استمر;
		}
		إذا (!strcmp("--refetch", arg)) {
			وسائط.refetch = 1;
			استمر;
		}
		إذا (skip_prefix(arg, ("--filter="), &arg)) {
			parse_list_objects_filter(&وسائط.filter_options, arg);
			استمر;
		}
		إذا (!strcmp(arg, ("--no-filter"))) {
			list_objects_filter_set_no_filter(&وسائط.filter_options);
			استمر;
		}
		usage(fetch_pack_usage);
	}
	إذا (deepen_not.nr)
		وسائط.deepen_not = &deepen_not;

	إذا (i < argc)
		dest = argv[i++];
	وإلا
		usage(fetch_pack_usage);

	/*
	 * Copy refs from cmdline to growable list, then append any
	 * refs from the standard input:
	 */
	لكل (; i < argc; i++)
		add_sought_entry(&sought, &nr_sought, &alloc_sought, argv[i]);
	إذا (وسائط.stdin_refs) {
		إذا (وسائط.stateless_rpc) {
			/* in stateless RPC mode we use pkt-line to read
			 * from stdin, until we get a flush packet
			 */
			لكل (;;) {
				حرف *سطر = packet_read_سطر(0, عدم);
				إذا (!سطر)
					اكسر;
				add_sought_entry(&sought, &nr_sought,  &alloc_sought, سطر);
			}
		}
		وإلا {
			/* read from stdin one ref per line, until EOF */
			بنية strbuf سطر = STRBUF_INIT;
			طالما (strbuf_getline_lf(&سطر, stdin) != EOF)
				add_sought_entry(&sought, &nr_sought, &alloc_sought, سطر.buf);
			strbuf_release(&سطر);
		}
	}

	إذا (وسائط.stateless_rpc) {
		conn = عدم;
		fd[0] = 0;
		fd[1] = 1;
	} وإلا {
		صحيح flags = وسائط.verbose ? CONNECT_VERBOSE : 0;
		إذا (وسائط.diag_url)
			flags |= CONNECT_DIAG_URL;
		conn = git_connect(fd, dest, GIT_CONNECT_UPLOAD_PACK,
				   وسائط.uploadpack, flags);
		إذا (!conn)
			أرجع وسائط.diag_url ? 0 : 1;
	}

	packet_reader_init(&reader, fd[0], عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	version = discover_version(&reader);
	بدل (version) {
	حالة protocol_v2:
		get_remote_refs(fd[1], &reader, &remote_refs, 0, عدم, عدم,
				وسائط.stateless_rpc);
		اكسر;
	حالة protocol_v1:
	حالة protocol_v0:
		get_remote_heads(&reader, &remote_refs, 0, عدم, &shallow);
		اكسر;
	حالة protocol_unknown_version:
		BUG("unknown protocol version");
	}

	/*
	 * Create a shallow copy of `sought` so that we can free all of its entries.
	 * This is because `fetch_pack()` will modify the array to evict some
	 * entries, but won't free those.
	 */
	DUP_ARRAY(sought_to_free, sought, nr_sought);

	fetched_refs = fetch_pack(&وسائط, fd, remote_refs, sought, nr_sought,
			 &shallow, pack_lockfiles_ptr, version);

	إذا (pack_lockfiles.nr) {
		صحيح i;

		printf("lock %s\n", pack_lockfiles.عناصر[0].string);
		fflush(stdout);
		لكل (i = 1; i < pack_lockfiles.nr; i++)
			warning(_("Lockfile created but not reported: %s"),
				pack_lockfiles.عناصر[i].string);
	}
	إذا (وسائط.check_self_contained_and_connected &&
	    وسائط.self_contained_and_connected) {
		printf("connectivity-ok\n");
		fflush(stdout);
	}
	close(fd[0]);
	close(fd[1]);
	إذا (finish_connect(conn)) {
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

	لكل (بنية ref *ref = fetched_refs; ref; ref = ref->next)
		printf("%s %s\n",
		       oid_to_hex(&ref->old_oid), ref->name);

cleanup:
	لكل (size_t i = 0; i < nr_sought; i++)
		free_one_ref(sought_to_free[i]);
	حرر(sought_to_free);
	حرر(sought);
	free_refs(fetched_refs);
	free_refs(remote_refs);
	list_objects_filter_release(&وسائط.filter_options);
	oid_array_clear(&shallow);
	string_list_clear(&pack_lockfiles, 0);
	أرجع ret;
}
