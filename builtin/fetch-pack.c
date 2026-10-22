#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

ایستا ثابت نویسه fetch_pack_usage[] =
"git fetch-pack [--all] [--stdin] [--quiet | -q] [--keep | -k] [--thin] "
"[--include-tag] [--upload-pack=<git-upload-pack>] [--depth=<n>] "
"[--no-progress] [--diag-url] [-v] [<host>:]<directory> [<refs>...]";

ایستا تهی add_sought_entry(ساختار ref ***sought, صحیح *nr, صحیح *alloc,
			     ثابت نویسه *name)
{
	ساختار ref *ref;
	ساختار object_id oid;
	ثابت نویسه *p;

	اگر (!parse_oid_hex(name, &oid, &p)) {
		اگر (*p == ' ') {
			/* <oid> <ref>, find refname */
			name = p + 1;
		} وگرنه اگر (*p == '\0') {
			; /* <oid>, leave oid as name */
		} وگرنه {
			/* <ref>, clear cruft from oid */
			oidclr(&oid, the_repository->hash_algo);
		}
	} وگرنه {
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
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix UNUSED,
		   ساختار repository *repo)
{
	صحیح i, ret;
	ساختار ref *fetched_refs = پوچ, *remote_refs = پوچ;
	ثابت نویسه *dest = پوچ;
	ساختار ref **sought = پوچ;
	ساختار ref **sought_to_free = پوچ;
	صحیح nr_sought = 0, alloc_sought = 0;
	صحیح fd[2];
	ساختار string_list pack_lockfiles = STRING_LIST_INIT_DUP;
	ساختار string_list *pack_lockfiles_ptr = پوچ;
	ساختار child_process *conn;
	ساختار fetch_pack_args args;
	ساختار oid_array shallow = OID_ARRAY_INIT;
	ساختار string_list deepen_not = STRING_LIST_INIT_DUP;
	ساختار packet_reader reader;
	شمارش protocol_version version;

	packet_trace_identity("fetch-pack");

	memset(&args, 0, اندازه(args));
	list_objects_filter_init(&args.filter_options);
	args.uploadpack = "git-upload-pack";

	show_usage_if_asked(argc, argv, fetch_pack_usage);

	repo->fetch_if_missing = 0;

	برای (i = 1; i < argc && *argv[i] == '-'; i++) {
		ثابت نویسه *arg = argv[i];

		اگر (skip_prefix(arg, "--upload-pack=", &arg)) {
			args.uploadpack = arg;
			ادامه;
		}
		اگر (skip_prefix(arg, "--exec=", &arg)) {
			args.uploadpack = arg;
			ادامه;
		}
		اگر (!strcmp("--quiet", arg) || !strcmp("-q", arg)) {
			args.quiet = 1;
			ادامه;
		}
		اگر (!strcmp("--keep", arg) || !strcmp("-k", arg)) {
			args.lock_pack = args.keep_pack;
			args.keep_pack = 1;
			ادامه;
		}
		اگر (!strcmp("--thin", arg)) {
			args.use_thin_pack = 1;
			ادامه;
		}
		اگر (!strcmp("--include-tag", arg)) {
			args.include_tag = 1;
			ادامه;
		}
		اگر (!strcmp("--all", arg)) {
			args.fetch_all = 1;
			ادامه;
		}
		اگر (!strcmp("--stdin", arg)) {
			args.stdin_refs = 1;
			ادامه;
		}
		اگر (!strcmp("--diag-url", arg)) {
			args.diag_url = 1;
			ادامه;
		}
		اگر (!strcmp("-v", arg)) {
			args.verbose = 1;
			ادامه;
		}
		اگر (skip_prefix(arg, "--depth=", &arg)) {
			args.depth = strtol(arg, پوچ, 0);
			ادامه;
		}
		اگر (skip_prefix(arg, "--shallow-since=", &arg)) {
			args.deepen_since = xstrdup(arg);
			ادامه;
		}
		اگر (skip_prefix(arg, "--shallow-exclude=", &arg)) {
			string_list_append(&deepen_not, arg);
			ادامه;
		}
		اگر (!strcmp(arg, "--deepen-relative")) {
			args.deepen_relative = 1;
			ادامه;
		}
		اگر (!strcmp("--no-progress", arg)) {
			args.no_progress = 1;
			ادامه;
		}
		اگر (!strcmp("--stateless-rpc", arg)) {
			args.stateless_rpc = 1;
			ادامه;
		}
		اگر (!strcmp("--lock-pack", arg)) {
			args.lock_pack = 1;
			pack_lockfiles_ptr = &pack_lockfiles;
			ادامه;
		}
		اگر (!strcmp("--check-self-contained-and-connected", arg)) {
			args.check_self_contained_and_connected = 1;
			ادامه;
		}
		اگر (!strcmp("--cloning", arg)) {
			args.cloning = 1;
			ادامه;
		}
		اگر (!strcmp("--update-shallow", arg)) {
			args.update_shallow = 1;
			ادامه;
		}
		اگر (!strcmp("--from-promisor", arg)) {
			args.from_promisor = 1;
			ادامه;
		}
		اگر (!strcmp("--refetch", arg)) {
			args.refetch = 1;
			ادامه;
		}
		اگر (skip_prefix(arg, ("--filter="), &arg)) {
			parse_list_objects_filter(&args.filter_options, arg);
			ادامه;
		}
		اگر (!strcmp(arg, ("--no-filter"))) {
			list_objects_filter_set_no_filter(&args.filter_options);
			ادامه;
		}
		usage(fetch_pack_usage);
	}
	اگر (deepen_not.nr)
		args.deepen_not = &deepen_not;

	اگر (i < argc)
		dest = argv[i++];
	وگرنه
		usage(fetch_pack_usage);

	/*
	 * Copy refs from cmdline to growable list, then append any
	 * refs from the standard input:
	 */
	برای (; i < argc; i++)
		add_sought_entry(&sought, &nr_sought, &alloc_sought, argv[i]);
	اگر (args.stdin_refs) {
		اگر (args.stateless_rpc) {
			/* in stateless RPC mode we use pkt-line to read
			 * from stdin, until we get a flush packet
			 */
			برای (;;) {
				نویسه *سطر = packet_read_سطر(0, پوچ);
				اگر (!سطر)
					بشکن;
				add_sought_entry(&sought, &nr_sought,  &alloc_sought, سطر);
			}
		}
		وگرنه {
			/* read from stdin one ref per line, until EOF */
			ساختار strbuf سطر = STRBUF_INIT;
			وقتی (strbuf_getline_lf(&سطر, stdin) != EOF)
				add_sought_entry(&sought, &nr_sought, &alloc_sought, سطر.buf);
			strbuf_release(&سطر);
		}
	}

	اگر (args.stateless_rpc) {
		conn = پوچ;
		fd[0] = 0;
		fd[1] = 1;
	} وگرنه {
		صحیح flags = args.verbose ? CONNECT_VERBOSE : 0;
		اگر (args.diag_url)
			flags |= CONNECT_DIAG_URL;
		conn = git_connect(fd, dest, GIT_CONNECT_UPLOAD_PACK,
				   args.uploadpack, flags);
		اگر (!conn)
			بازگشت args.diag_url ? 0 : 1;
	}

	packet_reader_init(&reader, fd[0], پوچ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	version = discover_version(&reader);
	انتخاب (version) {
	حالت protocol_v2:
		get_remote_refs(fd[1], &reader, &remote_refs, 0, پوچ, پوچ,
				args.stateless_rpc);
		بشکن;
	حالت protocol_v1:
	حالت protocol_v0:
		get_remote_heads(&reader, &remote_refs, 0, پوچ, &shallow);
		بشکن;
	حالت protocol_unknown_version:
		BUG("unknown protocol version");
	}

	/*
	 * Create a shallow copy of `sought` so that we can free all of its entries.
	 * This is because `fetch_pack()` will modify the array to evict some
	 * entries, but won't free those.
	 */
	DUP_ARRAY(sought_to_free, sought, nr_sought);

	fetched_refs = fetch_pack(&args, fd, remote_refs, sought, nr_sought,
			 &shallow, pack_lockfiles_ptr, version);

	اگر (pack_lockfiles.nr) {
		صحیح i;

		printf("lock %s\n", pack_lockfiles.اقلام[0].string);
		fflush(stdout);
		برای (i = 1; i < pack_lockfiles.nr; i++)
			warning(_("Lockfile created but not reported: %s"),
				pack_lockfiles.اقلام[i].string);
	}
	اگر (args.check_self_contained_and_connected &&
	    args.self_contained_and_connected) {
		printf("connectivity-ok\n");
		fflush(stdout);
	}
	close(fd[0]);
	close(fd[1]);
	اگر (finish_connect(conn)) {
		ret = 1;
		برو cleanup;
	}

	ret = !fetched_refs;

	/*
	 * If the heads to pull were given, we should have consumed
	 * all of them by matching the remote.  Otherwise, 'git fetch
	 * remote no-such-ref' would silently succeed without issuing
	 * an error.
	 */
	ret |= report_unmatched_refs(sought, nr_sought);

	برای (ساختار ref *ref = fetched_refs; ref; ref = ref->next)
		printf("%s %s\n",
		       oid_to_hex(&ref->old_oid), ref->name);

cleanup:
	برای (size_t i = 0; i < nr_sought; i++)
		free_one_ref(sought_to_free[i]);
	آزاد(sought_to_free);
	آزاد(sought);
	free_refs(fetched_refs);
	free_refs(remote_refs);
	list_objects_filter_release(&args.filter_options);
	oid_array_clear(&shallow);
	string_list_clear(&pack_lockfiles, 0);
	بازگشت ret;
}
