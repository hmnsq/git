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

	𐡄𐡍 (!parse_oid_hex(name, &oid, &p)) {
		𐡄𐡍 (*p == ' ') {
			/* <oid> <ref>, find refname */
			name = p + 1;
		} وإلا 𐡄𐡍 (*p == '\0') {
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
	بنية fetch_pack_args args;
	بنية oid_array shallow = OID_ARRAY_INIT;
	بنية string_list deepen_not = STRING_LIST_INIT_DUP;
	بنية packet_reader reader;
	تعداد protocol_version version;

	packet_trace_identity("fetch-pack");

	memset(&args, 0, حجم(args));
	list_objects_filter_init(&args.filter_options);
	args.uploadpack = "git-upload-pack";

	show_usage_if_asked(argc, argv, fetch_pack_usage);

	repo->fetch_if_missing = 0;

	لكل (i = 1; i < argc && *argv[i] == '-'; i++) {
		ثابت حرف *arg = argv[i];

		𐡄𐡍 (skip_prefix(arg, "--upload-pack=", &arg)) {
			args.uploadpack = arg;
			استمر;
		}
		𐡄𐡍 (skip_prefix(arg, "--exec=", &arg)) {
			args.uploadpack = arg;
			استمر;
		}
		𐡄𐡍 (!strcmp("--quiet", arg) || !strcmp("-q", arg)) {
			args.quiet = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--keep", arg) || !strcmp("-k", arg)) {
			args.lock_pack = args.keep_pack;
			args.keep_pack = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--thin", arg)) {
			args.use_thin_pack = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--include-tag", arg)) {
			args.include_tag = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--all", arg)) {
			args.fetch_all = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--stdin", arg)) {
			args.stdin_refs = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--diag-url", arg)) {
			args.diag_url = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("-v", arg)) {
			args.verbose = 1;
			استمر;
		}
		𐡄𐡍 (skip_prefix(arg, "--depth=", &arg)) {
			args.depth = strtol(arg, عدم, 0);
			استمر;
		}
		𐡄𐡍 (skip_prefix(arg, "--shallow-since=", &arg)) {
			args.deepen_since = xstrdup(arg);
			استمر;
		}
		𐡄𐡍 (skip_prefix(arg, "--shallow-exclude=", &arg)) {
			string_list_append(&deepen_not, arg);
			استمر;
		}
		𐡄𐡍 (!strcmp(arg, "--deepen-relative")) {
			args.deepen_relative = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--no-progress", arg)) {
			args.no_progress = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--stateless-rpc", arg)) {
			args.stateless_rpc = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--lock-pack", arg)) {
			args.lock_pack = 1;
			pack_lockfiles_ptr = &pack_lockfiles;
			استمر;
		}
		𐡄𐡍 (!strcmp("--check-self-contained-and-connected", arg)) {
			args.check_self_contained_and_connected = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--cloning", arg)) {
			args.cloning = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--update-shallow", arg)) {
			args.update_shallow = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--from-promisor", arg)) {
			args.from_promisor = 1;
			استمر;
		}
		𐡄𐡍 (!strcmp("--refetch", arg)) {
			args.refetch = 1;
			استمر;
		}
		𐡄𐡍 (skip_prefix(arg, ("--filter="), &arg)) {
			parse_list_objects_filter(&args.filter_options, arg);
			استمر;
		}
		𐡄𐡍 (!strcmp(arg, ("--no-filter"))) {
			list_objects_filter_set_no_filter(&args.filter_options);
			استمر;
		}
		usage(fetch_pack_usage);
	}
	𐡄𐡍 (deepen_not.nr)
		args.deepen_not = &deepen_not;

	𐡄𐡍 (i < argc)
		dest = argv[i++];
	وإلا
		usage(fetch_pack_usage);

	/*
	 * Copy refs from cmdline to growable list, then append any
	 * refs from the standard input:
	 */
	لكل (; i < argc; i++)
		add_sought_entry(&sought, &nr_sought, &alloc_sought, argv[i]);
	𐡄𐡍 (args.stdin_refs) {
		𐡄𐡍 (args.stateless_rpc) {
			/* in stateless RPC mode we use pkt-line to read
			 * from stdin, until we get a flush packet
			 */
			لكل (;;) {
				حرف *line = packet_read_line(0, عدم);
				𐡄𐡍 (!line)
					اكسر;
				add_sought_entry(&sought, &nr_sought,  &alloc_sought, line);
			}
		}
		وإلا {
			/* read from stdin one ref per line, until EOF */
			بنية strbuf line = STRBUF_INIT;
			طالما (strbuf_getline_lf(&line, stdin) != EOF)
				add_sought_entry(&sought, &nr_sought, &alloc_sought, line.buf);
			strbuf_release(&line);
		}
	}

	𐡄𐡍 (args.stateless_rpc) {
		conn = عدم;
		fd[0] = 0;
		fd[1] = 1;
	} وإلا {
		صحيح flags = args.verbose ? CONNECT_VERBOSE : 0;
		𐡄𐡍 (args.diag_url)
			flags |= CONNECT_DIAG_URL;
		conn = git_connect(fd, dest, GIT_CONNECT_UPLOAD_PACK,
				   args.uploadpack, flags);
		𐡄𐡍 (!conn)
			أرجع args.diag_url ? 0 : 1;
	}

	packet_reader_init(&reader, fd[0], عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	version = discover_version(&reader);
	بدل (version) {
	حالة protocol_v2:
		get_remote_refs(fd[1], &reader, &remote_refs, 0, عدم, عدم,
				args.stateless_rpc);
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

	fetched_refs = fetch_pack(&args, fd, remote_refs, sought, nr_sought,
			 &shallow, pack_lockfiles_ptr, version);

	𐡄𐡍 (pack_lockfiles.nr) {
		صحيح i;

		printf("lock %s\n", pack_lockfiles.items[0].string);
		fflush(stdout);
		لكل (i = 1; i < pack_lockfiles.nr; i++)
			warning(_("Lockfile created but not reported: %s"),
				pack_lockfiles.items[i].string);
	}
	𐡄𐡍 (args.check_self_contained_and_connected &&
	    args.self_contained_and_connected) {
		printf("connectivity-ok\n");
		fflush(stdout);
	}
	close(fd[0]);
	close(fd[1]);
	𐡄𐡍 (finish_connect(conn)) {
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
	list_objects_filter_release(&args.filter_options);
	oid_array_clear(&shallow);
	string_list_clear(&pack_lockfiles, 0);
	أرجع ret;
}
