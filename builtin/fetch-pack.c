#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pkt-line.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fetch-pack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "connect.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "protocol.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ fetch_pack_usage[] =
"git fetch-pack [--all] [--stdin] [--quiet | -q] [--keep | -k] [--thin] "
"[--include-tag] [--upload-pack=<git-upload-pack>] [--depth=<n>] "
"[--no-progress] [--diag-url] [-v] [<host>:]<directory> [<refs>...]";

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_sought_ਇੰਦਰਾਜ(ਬਣਤਰ ref ***sought, ਪੂਰਨ *nr, ਪੂਰਨ *alloc,
			     ਅਚਲ ਅੱਖਰ *name)
{
	ਬਣਤਰ ref *ref;
	ਬਣਤਰ object_id oid;
	ਅਚਲ ਅੱਖਰ *p;

	ਜੇ (!parse_oid_hex(name, &oid, &p)) {
		ਜੇ (*p == ' ') {
			/* <oid> <ref>, find refname */
			name = p + 1;
		} ਨਹੀਂਤਾਂ ਜੇ (*p == '\0') {
			; /* <oid>, leave oid as name */
		} ਨਹੀਂਤਾਂ {
			/* <ref>, clear cruft from oid */
			oidclr(&oid, the_repository->hash_algo);
		}
	} ਨਹੀਂਤਾਂ {
		/* <ref>, clear cruft from get_oid_hex */
		oidclr(&oid, the_repository->hash_algo);
	}

	ref = alloc_ref(name);
	oidcpy(&ref->old_oid, &oid);
	(*nr)++;
	ALLOC_GROW(*sought, *nr, *alloc);
	(*sought)[*nr - 1] = ref;
}

ਪੂਰਨ cmd_fetch_pack(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
		   ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ i, ret;
	ਬਣਤਰ ref *fetched_refs = ਨਿਰਾ, *remote_refs = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *dest = ਨਿਰਾ;
	ਬਣਤਰ ref **sought = ਨਿਰਾ;
	ਬਣਤਰ ref **sought_ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;
	ਪੂਰਨ nr_sought = 0, alloc_sought = 0;
	ਪੂਰਨ fd[2];
	ਬਣਤਰ string_list pack_lockfiles = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list *pack_lockfiles_ptr = ਨਿਰਾ;
	ਬਣਤਰ ਬੱਚਾ_process *conn;
	ਬਣਤਰ fetch_pack_ਦਲੀਲਾਂ ਦਲੀਲਾਂ;
	ਬਣਤਰ oid_array shallow = OID_ARRAY_INIT;
	ਬਣਤਰ string_list deepen_not = STRING_LIST_INIT_DUP;
	ਬਣਤਰ packet_reader reader;
	ਗਿਣਤੀ protocol_ਸੰਸਕਰਣ ਸੰਸਕਰਣ;

	packet_trace_ਪਛਾਣ_ਤੱਤ("fetch-pack");

	memset(&ਦਲੀਲਾਂ, 0, ਆਕਾਰ(ਦਲੀਲਾਂ));
	list_objects_filter_init(&ਦਲੀਲਾਂ.filter_options);
	ਦਲੀਲਾਂ.uploadpack = "git-upload-pack";

	show_usage_if_asked(argc, argv, fetch_pack_usage);

	repo->fetch_if_missing = 0;

	ਲਈ (i = 1; i < argc && *argv[i] == '-'; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];

		ਜੇ (skip_ਅਗੇਤਰ(arg, "--upload-pack=", &arg)) {
			ਦਲੀਲਾਂ.uploadpack = arg;
			ਜਾਰੀ;
		}
		ਜੇ (skip_ਅਗੇਤਰ(arg, "--exec=", &arg)) {
			ਦਲੀਲਾਂ.uploadpack = arg;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--quiet", arg) || !strcmp("-q", arg)) {
			ਦਲੀਲਾਂ.quiet = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--keep", arg) || !strcmp("-k", arg)) {
			ਦਲੀਲਾਂ.ਤਾਲਾ_pack = ਦਲੀਲਾਂ.keep_pack;
			ਦਲੀਲਾਂ.keep_pack = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--thin", arg)) {
			ਦਲੀਲਾਂ.use_thin_pack = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--include-tag", arg)) {
			ਦਲੀਲਾਂ.include_ਟੈਗ = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--all", arg)) {
			ਦਲੀਲਾਂ.fetch_all = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--stdin", arg)) {
			ਦਲੀਲਾਂ.stdin_refs = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--diag-url", arg)) {
			ਦਲੀਲਾਂ.diag_url = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("-v", arg)) {
			ਦਲੀਲਾਂ.verbose = 1;
			ਜਾਰੀ;
		}
		ਜੇ (skip_ਅਗੇਤਰ(arg, "--depth=", &arg)) {
			ਦਲੀਲਾਂ.ਡੂੰਘਾਈ = strtol(arg, ਨਿਰਾ, 0);
			ਜਾਰੀ;
		}
		ਜੇ (skip_ਅਗੇਤਰ(arg, "--shallow-since=", &arg)) {
			ਦਲੀਲਾਂ.deepen_since = xstrdup(arg);
			ਜਾਰੀ;
		}
		ਜੇ (skip_ਅਗੇਤਰ(arg, "--shallow-exclude=", &arg)) {
			string_list_append(&deepen_not, arg);
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, "--deepen-relative")) {
			ਦਲੀਲਾਂ.deepen_relative = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--no-progress", arg)) {
			ਦਲੀਲਾਂ.no_ਤਰੱਕੀ = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--stateless-rpc", arg)) {
			ਦਲੀਲਾਂ.stateless_rpc = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--lock-pack", arg)) {
			ਦਲੀਲਾਂ.ਤਾਲਾ_pack = 1;
			pack_lockfiles_ptr = &pack_lockfiles;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--check-self-contained-and-connected", arg)) {
			ਦਲੀਲਾਂ.check_self_contained_and_connected = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--cloning", arg)) {
			ਦਲੀਲਾਂ.cloning = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--update-shallow", arg)) {
			ਦਲੀਲਾਂ.update_shallow = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--from-promisor", arg)) {
			ਦਲੀਲਾਂ.from_promisor = 1;
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp("--refetch", arg)) {
			ਦਲੀਲਾਂ.refetch = 1;
			ਜਾਰੀ;
		}
		ਜੇ (skip_ਅਗੇਤਰ(arg, ("--filter="), &arg)) {
			parse_list_objects_filter(&ਦਲੀਲਾਂ.filter_options, arg);
			ਜਾਰੀ;
		}
		ਜੇ (!strcmp(arg, ("--no-filter"))) {
			list_objects_filter_ਸੈੱਟ_no_filter(&ਦਲੀਲਾਂ.filter_options);
			ਜਾਰੀ;
		}
		usage(fetch_pack_usage);
	}
	ਜੇ (deepen_not.nr)
		ਦਲੀਲਾਂ.deepen_not = &deepen_not;

	ਜੇ (i < argc)
		dest = argv[i++];
	ਨਹੀਂਤਾਂ
		usage(fetch_pack_usage);

	/*
	 * Copy refs from cmdline to growable list, then append any
	 * refs from the standard input:
	 */
	ਲਈ (; i < argc; i++)
		ਜੋੜੋ_sought_ਇੰਦਰਾਜ(&sought, &nr_sought, &alloc_sought, argv[i]);
	ਜੇ (ਦਲੀਲਾਂ.stdin_refs) {
		ਜੇ (ਦਲੀਲਾਂ.stateless_rpc) {
			/* in stateless RPC mode we use pkt-line to read
			 * from stdin, until we get a flush packet
			 */
			ਲਈ (;;) {
				ਅੱਖਰ *ਸਤਰ = packet_read_ਸਤਰ(0, ਨਿਰਾ);
				ਜੇ (!ਸਤਰ)
					ਰੋਕੋ;
				ਜੋੜੋ_sought_ਇੰਦਰਾਜ(&sought, &nr_sought,  &alloc_sought, ਸਤਰ);
			}
		}
		ਨਹੀਂਤਾਂ {
			/* read from stdin one ref per line, until EOF */
			ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;
			ਜਦੋਂ (strbuf_getline_lf(&ਸਤਰ, stdin) != EOF)
				ਜੋੜੋ_sought_ਇੰਦਰਾਜ(&sought, &nr_sought, &alloc_sought, ਸਤਰ.buf);
			strbuf_release(&ਸਤਰ);
		}
	}

	ਜੇ (ਦਲੀਲਾਂ.stateless_rpc) {
		conn = ਨਿਰਾ;
		fd[0] = 0;
		fd[1] = 1;
	} ਨਹੀਂਤਾਂ {
		ਪੂਰਨ flags = ਦਲੀਲਾਂ.verbose ? CONNECT_VERBOSE : 0;
		ਜੇ (ਦਲੀਲਾਂ.diag_url)
			flags |= CONNECT_DIAG_URL;
		conn = git_connect(fd, dest, GIT_CONNECT_UPLOAD_PACK,
				   ਦਲੀਲਾਂ.uploadpack, flags);
		ਜੇ (!conn)
			ਵਾਪਸ ਦਲੀਲਾਂ.diag_url ? 0 : 1;
	}

	packet_reader_init(&reader, fd[0], ਨਿਰਾ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ਸੰਸਕਰਣ = discover_ਸੰਸਕਰਣ(&reader);
	ਚੋਣ (ਸੰਸਕਰਣ) {
	ਹਾਲਤ protocol_v2:
		get_remote_refs(fd[1], &reader, &remote_refs, 0, ਨਿਰਾ, ਨਿਰਾ,
				ਦਲੀਲਾਂ.stateless_rpc);
		ਰੋਕੋ;
	ਹਾਲਤ protocol_v1:
	ਹਾਲਤ protocol_v0:
		get_remote_heads(&reader, &remote_refs, 0, ਨਿਰਾ, &shallow);
		ਰੋਕੋ;
	ਹਾਲਤ protocol_unknown_ਸੰਸਕਰਣ:
		BUG("unknown protocol version");
	}

	/*
	 * Create a shallow copy of `sought` so that we can free all of its entries.
	 * This is because `fetch_pack()` will modify the array to evict some
	 * entries, but won't free those.
	 */
	DUP_ARRAY(sought_ਮੰਜ਼ਿਲ_free, sought, nr_sought);

	fetched_refs = fetch_pack(&ਦਲੀਲਾਂ, fd, remote_refs, sought, nr_sought,
			 &shallow, pack_lockfiles_ptr, ਸੰਸਕਰਣ);

	ਜੇ (pack_lockfiles.nr) {
		ਪੂਰਨ i;

		ਲਿਖੋ("lock %s\n", pack_lockfiles.ਚੀਜ਼ਾਂ[0].string);
		fflush(stdout);
		ਲਈ (i = 1; i < pack_lockfiles.nr; i++)
			warning(_("Lockfile created but not reported: %s"),
				pack_lockfiles.ਚੀਜ਼ਾਂ[i].string);
	}
	ਜੇ (ਦਲੀਲਾਂ.check_self_contained_and_connected &&
	    ਦਲੀਲਾਂ.self_contained_and_connected) {
		ਲਿਖੋ("connectivity-ok\n");
		fflush(stdout);
	}
	close(fd[0]);
	close(fd[1]);
	ਜੇ (ਅੰਤ_connect(conn)) {
		ret = 1;
		ਜਾਓ cleanup;
	}

	ret = !fetched_refs;

	/*
	 * If the heads to pull were given, we should have consumed
	 * all of them by matching the remote.  Otherwise, 'git fetch
	 * remote no-such-ref' would silently succeed without issuing
	 * an error.
	 */
	ret |= report_unmatched_refs(sought, nr_sought);

	ਲਈ (ਬਣਤਰ ref *ref = fetched_refs; ref; ref = ref->ਅਗਲਾ)
		ਲਿਖੋ("%s %s\n",
		       oid_ਮੰਜ਼ਿਲ_hex(&ref->old_oid), ref->name);

cleanup:
	ਲਈ (ਮਾਪ_t i = 0; i < nr_sought; i++)
		free_one_ref(sought_ਮੰਜ਼ਿਲ_free[i]);
	ਮੁਕਤ(sought_ਮੰਜ਼ਿਲ_free);
	ਮੁਕਤ(sought);
	free_refs(fetched_refs);
	free_refs(remote_refs);
	list_objects_filter_release(&ਦਲੀਲਾਂ.filter_options);
	oid_array_clear(&shallow);
	string_list_clear(&pack_lockfiles, 0);
	ਵਾਪਸ ret;
}
