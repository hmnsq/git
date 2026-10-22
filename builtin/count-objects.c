/*
 * Builtin "git count-objects".
 *
 * Copyright (c) 2006 Junio C Hamano
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "packfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ garbage;
ਸਥਿਰ off_t ਮਾਪ_garbage;
ਸਥਿਰ ਪੂਰਨ verbose;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ loose, packed, packed_loose;
ਸਥਿਰ off_t loose_ਮਾਪ;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੁਨੇਹਾ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਵੇਖਿਆ_ਬਿੱਟ)
{
	ਚੋਣ (ਵੇਖਿਆ_ਬਿੱਟ) {
	ਹਾਲਤ 0:
		ਵਾਪਸ "no corresponding .idx or .pack";
	ਹਾਲਤ PACKDIR_FILE_GARBAGE:
		ਵਾਪਸ "garbage found";
	ਹਾਲਤ PACKDIR_FILE_PACK:
		ਵਾਪਸ "no corresponding .idx";
	ਹਾਲਤ PACKDIR_FILE_IDX:
		ਵਾਪਸ "no corresponding .pack";
	ਹਾਲਤ PACKDIR_FILE_PACK|PACKDIR_FILE_IDX:
	ਮੂਲ:
		ਵਾਪਸ ਨਿਰਾ;
	}
}

ਸਥਿਰ ਖਾਲੀ real_report_garbage(ਬਿਨਾਨਿਸ਼ਾਨੀ ਵੇਖਿਆ_ਬਿੱਟ, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਣਤਰ stat st;
	ਅਚਲ ਅੱਖਰ *desc = ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੁਨੇਹਾ(ਵੇਖਿਆ_ਬਿੱਟ);

	ਜੇ (!desc)
		ਵਾਪਸ;

	ਜੇ (!stat(ਰਸਤਾ, &st))
		ਮਾਪ_garbage += st.st_ਮਾਪ;
	warning("%s: %s", desc, ਰਸਤਾ);
	garbage++;
}

ਸਥਿਰ ਖਾਲੀ loose_garbage(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਜੇ (verbose)
		report_garbage(PACKDIR_FILE_GARBAGE, ਰਸਤਾ);
}

ਸਥਿਰ ਪੂਰਨ ਗਿਣਤੀ_loose(ਅਚਲ ਬਣਤਰ object_id *oid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
		       ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ stat st;

	ਜੇ (lstat(ਰਸਤਾ, &st) || !S_ISREG(st.st_ਬਹੁਲਕ))
		loose_garbage(ਰਸਤਾ);
	ਨਹੀਂਤਾਂ {
		loose_ਮਾਪ += on_disk_ਬਾਈਟਾਂ(st);
		loose++;
		ਜੇ (verbose && has_object_pack(the_repository, oid))
			packed_loose++;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਗਿਣਤੀ_cruft(ਅਚਲ ਅੱਖਰ *basename ਅਣਵਰਤਿਆ, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
		       ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	loose_garbage(ਰਸਤਾ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ print_alternate(ਬਣਤਰ odb_files_dir *alternate, ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਲਿਖੋ("alternate: ");
	quote_c_style(alternate->abspath, ਨਿਰਾ, stdout, 0);
	ਅੱਖਰ_ਲਿਖੋ('\n');
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ ਗਿਣਤੀ_objects_usage[] = {
	"git count-objects [-v] [-H | --human-readable]",
	ਨਿਰਾ
};

ਪੂਰਨ cmd_ਗਿਣਤੀ_objects(ਪੂਰਨ argc,
		      ਅਚਲ ਅੱਖਰ **argv,
		      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ human_readable = 0;
	ਬਣਤਰ option opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL('H', "human-readable", &human_readable,
			 N_("print sizes in human readable format")),
		OPT_ਸਿਰਾ(),
	};

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, opts, ਗਿਣਤੀ_objects_usage, 0);
	/* we do not take arguments other than flags for now */
	ਜੇ (argc)
		usage_with_options(ਗਿਣਤੀ_objects_usage, opts);
	ਜੇ (verbose) {
		report_garbage = real_report_garbage;
		report_linked_checkout_garbage(the_repository);
	}

	for_each_loose_file_ਵਿੱਚ_ਸਰੋਤ(the_repository->objects->ਸਰੋਤ,
				      ਗਿਣਤੀ_loose, ਗਿਣਤੀ_cruft, ਨਿਰਾ, ਨਿਰਾ);

	ਜੇ (verbose) {
		ਬਣਤਰ packed_git *p;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅੰਕ_pack = 0;
		off_t ਮਾਪ_pack = 0;
		ਬਣਤਰ strbuf loose_buf = STRBUF_INIT;
		ਬਣਤਰ strbuf pack_buf = STRBUF_INIT;
		ਬਣਤਰ strbuf garbage_buf = STRBUF_INIT;

		repo_for_each_pack(the_repository, p) {
			ਜੇ (!p->pack_local)
				ਜਾਰੀ;
			ਜੇ (open_pack_ਸੂਚਕਾਂਕ(p))
				ਜਾਰੀ;
			packed += p->ਅੰਕ_objects;
			ਮਾਪ_pack += p->pack_ਮਾਪ + p->ਸੂਚਕਾਂਕ_ਮਾਪ;
			ਅੰਕ_pack++;
		}

		ਜੇ (human_readable) {
			strbuf_humanise_ਬਾਈਟਾਂ(&loose_buf, loose_ਮਾਪ);
			strbuf_humanise_ਬਾਈਟਾਂ(&pack_buf, ਮਾਪ_pack);
			strbuf_humanise_ਬਾਈਟਾਂ(&garbage_buf, ਮਾਪ_garbage);
		} ਨਹੀਂਤਾਂ {
			strbuf_addf(&loose_buf, "%lu",
				    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(loose_ਮਾਪ / 1024));
			strbuf_addf(&pack_buf, "%lu",
				    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(ਮਾਪ_pack / 1024));
			strbuf_addf(&garbage_buf, "%lu",
				    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(ਮਾਪ_garbage / 1024));
		}

		ਲਿਖੋ("count: %lu\n", loose);
		ਲਿਖੋ("size: %s\n", loose_buf.buf);
		ਲਿਖੋ("in-pack: %lu\n", packed);
		ਲਿਖੋ("packs: %lu\n", ਅੰਕ_pack);
		ਲਿਖੋ("size-pack: %s\n", pack_buf.buf);
		ਲਿਖੋ("prune-packable: %lu\n", packed_loose);
		ਲਿਖੋ("garbage: %lu\n", garbage);
		ਲਿਖੋ("size-garbage: %s\n", garbage_buf.buf);
		odb_for_each_alternate(the_repository->objects, print_alternate, ਨਿਰਾ);
		strbuf_release(&loose_buf);
		strbuf_release(&pack_buf);
		strbuf_release(&garbage_buf);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ strbuf buf = STRBUF_INIT;
		ਜੇ (human_readable)
			strbuf_humanise_ਬਾਈਟਾਂ(&buf, loose_ਮਾਪ);
		ਨਹੀਂਤਾਂ
			strbuf_addf(&buf, "%lu kilobytes",
				    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(loose_ਮਾਪ / 1024));
		ਲਿਖੋ("%lu objects, %s\n", loose, buf.buf);
		strbuf_release(&buf);
	}
	ਵਾਪਸ 0;
}
