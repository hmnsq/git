#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "pkt-line.h"
#અન્તર્ભાવ "fetch-pack.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "connect.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "protocol.h"

સ્થિર અચલ અક્ષર fetch_pack_usage[] =
"git fetch-pack [--all] [--stdin] [--quiet | -q] [--keep | -k] [--thin] "
"[--include-tag] [--upload-pack=<git-upload-pack>] [--depth=<n>] "
"[--no-progress] [--diag-url] [-v] [<host>:]<directory> [<refs>...]";

સ્થિર શૂન્ય add_sought_entry(સંરચના ref ***sought, પૂર્ણાંક *nr, પૂર્ણાંક *alloc,
			     અચલ અક્ષર *name)
{
	સંરચના ref *ref;
	સંરચના object_id oid;
	અચલ અક્ષર *p;

	જો (!parse_oid_hex(name, &oid, &p)) {
		જો (*p == ' ') {
			/* <oid> <ref>, find refname */
			name = p + 1;
		} નહીંતર જો (*p == '\0') {
			; /* <oid>, leave oid as name */
		} નહીંતર {
			/* <ref>, clear cruft from oid */
			oidclr(&oid, the_repository->hash_algo);
		}
	} નહીંતર {
		/* <ref>, clear cruft from get_oid_hex */
		oidclr(&oid, the_repository->hash_algo);
	}

	ref = alloc_ref(name);
	oidcpy(&ref->old_oid, &oid);
	(*nr)++;
	ALLOC_GROW(*sought, *nr, *alloc);
	(*sought)[*nr - 1] = ref;
}

પૂર્ણાંક cmd_fetch_pack(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *prefix UNUSED,
		   સંરચના repository *repo)
{
	પૂર્ણાંક i, ret;
	સંરચના ref *fetched_refs = ખાલી, *remote_refs = ખાલી;
	અચલ અક્ષર *dest = ખાલી;
	સંરચના ref **sought = ખાલી;
	સંરચના ref **sought_to_free = ખાલી;
	પૂર્ણાંક nr_sought = 0, alloc_sought = 0;
	પૂર્ણાંક fd[2];
	સંરચના string_list pack_lockfiles = STRING_LIST_INIT_DUP;
	સંરચના string_list *pack_lockfiles_ptr = ખાલી;
	સંરચના child_process *conn;
	સંરચના fetch_pack_args args;
	સંરચના oid_array shallow = OID_ARRAY_INIT;
	સંરચના string_list deepen_not = STRING_LIST_INIT_DUP;
	સંરચના packet_reader reader;
	ગણના protocol_version version;

	packet_trace_identity("fetch-pack");

	memset(&args, 0, કદ(args));
	list_objects_filter_init(&args.filter_options);
	args.uploadpack = "git-upload-pack";

	show_usage_if_asked(argc, argv, fetch_pack_usage);

	repo->fetch_if_missing = 0;

	માટે (i = 1; i < argc && *argv[i] == '-'; i++) {
		અચલ અક્ષર *arg = argv[i];

		જો (skip_prefix(arg, "--upload-pack=", &arg)) {
			args.uploadpack = arg;
			ચાલુ;
		}
		જો (skip_prefix(arg, "--exec=", &arg)) {
			args.uploadpack = arg;
			ચાલુ;
		}
		જો (!strcmp("--quiet", arg) || !strcmp("-q", arg)) {
			args.quiet = 1;
			ચાલુ;
		}
		જો (!strcmp("--keep", arg) || !strcmp("-k", arg)) {
			args.lock_pack = args.keep_pack;
			args.keep_pack = 1;
			ચાલુ;
		}
		જો (!strcmp("--thin", arg)) {
			args.use_thin_pack = 1;
			ચાલુ;
		}
		જો (!strcmp("--include-tag", arg)) {
			args.include_tag = 1;
			ચાલુ;
		}
		જો (!strcmp("--all", arg)) {
			args.fetch_all = 1;
			ચાલુ;
		}
		જો (!strcmp("--stdin", arg)) {
			args.stdin_refs = 1;
			ચાલુ;
		}
		જો (!strcmp("--diag-url", arg)) {
			args.diag_url = 1;
			ચાલુ;
		}
		જો (!strcmp("-v", arg)) {
			args.verbose = 1;
			ચાલુ;
		}
		જો (skip_prefix(arg, "--depth=", &arg)) {
			args.depth = strtol(arg, ખાલી, 0);
			ચાલુ;
		}
		જો (skip_prefix(arg, "--shallow-since=", &arg)) {
			args.deepen_since = xstrdup(arg);
			ચાલુ;
		}
		જો (skip_prefix(arg, "--shallow-exclude=", &arg)) {
			string_list_append(&deepen_not, arg);
			ચાલુ;
		}
		જો (!strcmp(arg, "--deepen-relative")) {
			args.deepen_relative = 1;
			ચાલુ;
		}
		જો (!strcmp("--no-progress", arg)) {
			args.no_progress = 1;
			ચાલુ;
		}
		જો (!strcmp("--stateless-rpc", arg)) {
			args.stateless_rpc = 1;
			ચાલુ;
		}
		જો (!strcmp("--lock-pack", arg)) {
			args.lock_pack = 1;
			pack_lockfiles_ptr = &pack_lockfiles;
			ચાલુ;
		}
		જો (!strcmp("--check-self-contained-and-connected", arg)) {
			args.check_self_contained_and_connected = 1;
			ચાલુ;
		}
		જો (!strcmp("--cloning", arg)) {
			args.cloning = 1;
			ચાલુ;
		}
		જો (!strcmp("--update-shallow", arg)) {
			args.update_shallow = 1;
			ચાલુ;
		}
		જો (!strcmp("--from-promisor", arg)) {
			args.from_promisor = 1;
			ચાલુ;
		}
		જો (!strcmp("--refetch", arg)) {
			args.refetch = 1;
			ચાલુ;
		}
		જો (skip_prefix(arg, ("--filter="), &arg)) {
			parse_list_objects_filter(&args.filter_options, arg);
			ચાલુ;
		}
		જો (!strcmp(arg, ("--no-filter"))) {
			list_objects_filter_set_no_filter(&args.filter_options);
			ચાલુ;
		}
		usage(fetch_pack_usage);
	}
	જો (deepen_not.nr)
		args.deepen_not = &deepen_not;

	જો (i < argc)
		dest = argv[i++];
	નહીંતર
		usage(fetch_pack_usage);

	/*
	 * Copy refs from cmdline to growable list, then append any
	 * refs from the standard input:
	 */
	માટે (; i < argc; i++)
		add_sought_entry(&sought, &nr_sought, &alloc_sought, argv[i]);
	જો (args.stdin_refs) {
		જો (args.stateless_rpc) {
			/* in stateless RPC mode we use pkt-line to read
			 * from stdin, until we get a flush packet
			 */
			માટે (;;) {
				અક્ષર *લીટી = packet_read_લીટી(0, ખાલી);
				જો (!લીટી)
					તોડો;
				add_sought_entry(&sought, &nr_sought,  &alloc_sought, લીટી);
			}
		}
		નહીંતર {
			/* read from stdin one ref per line, until EOF */
			સંરચના strbuf લીટી = STRBUF_INIT;
			જ્યારે (strbuf_getline_lf(&લીટી, stdin) != EOF)
				add_sought_entry(&sought, &nr_sought, &alloc_sought, લીટી.buf);
			strbuf_release(&લીટી);
		}
	}

	જો (args.stateless_rpc) {
		conn = ખાલી;
		fd[0] = 0;
		fd[1] = 1;
	} નહીંતર {
		પૂર્ણાંક flags = args.verbose ? CONNECT_VERBOSE : 0;
		જો (args.diag_url)
			flags |= CONNECT_DIAG_URL;
		conn = git_connect(fd, dest, GIT_CONNECT_UPLOAD_PACK,
				   args.uploadpack, flags);
		જો (!conn)
			પાછા args.diag_url ? 0 : 1;
	}

	packet_reader_init(&reader, fd[0], ખાલી, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_GENTLE_ON_EOF |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	version = discover_version(&reader);
	પસંદગી (version) {
	સ્થિતિ protocol_v2:
		get_remote_refs(fd[1], &reader, &remote_refs, 0, ખાલી, ખાલી,
				args.stateless_rpc);
		તોડો;
	સ્થિતિ protocol_v1:
	સ્થિતિ protocol_v0:
		get_remote_heads(&reader, &remote_refs, 0, ખાલી, &shallow);
		તોડો;
	સ્થિતિ protocol_unknown_version:
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

	જો (pack_lockfiles.nr) {
		પૂર્ણાંક i;

		printf("lock %s\n", pack_lockfiles.વસ્તુઓ[0].string);
		fflush(stdout);
		માટે (i = 1; i < pack_lockfiles.nr; i++)
			warning(_("Lockfile created but not reported: %s"),
				pack_lockfiles.વસ્તુઓ[i].string);
	}
	જો (args.check_self_contained_and_connected &&
	    args.self_contained_and_connected) {
		printf("connectivity-ok\n");
		fflush(stdout);
	}
	close(fd[0]);
	close(fd[1]);
	જો (finish_connect(conn)) {
		ret = 1;
		જાઓ cleanup;
	}

	ret = !fetched_refs;

	/*
	 * If the heads to pull were given, we should have consumed
	 * all of them by matching the remote.  Otherwise, 'git fetch
	 * remote no-such-ref' would silently succeed without issuing
	 * an error.
	 */
	ret |= report_unmatched_refs(sought, nr_sought);

	માટે (સંરચના ref *ref = fetched_refs; ref; ref = ref->next)
		printf("%s %s\n",
		       oid_to_hex(&ref->old_oid), ref->name);

cleanup:
	માટે (size_t i = 0; i < nr_sought; i++)
		free_one_ref(sought_to_free[i]);
	મુક્ત(sought_to_free);
	મુક્ત(sought);
	free_refs(fetched_refs);
	free_refs(remote_refs);
	list_objects_filter_release(&args.filter_options);
	oid_array_clear(&shallow);
	string_list_clear(&pack_lockfiles, 0);
	પાછા ret;
}
