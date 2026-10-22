/*
 * Builtin "git count-objects".
 *
 * Copyright (c) 2006 Junio C Hamano
 */
#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "dir.h"
#include "environment.h"
#include "gettext.h"
#include "path.h"
#include "parse-options.h"
#include "quote.h"
#include "packfile.h"
#include "object-file.h"

static unsigned long garbage;
static off_t size_garbage;
static puur_naa_nka verbose;
static unsigned long loose, packed, packed_loose;
static off_t loose_size;

static const var_na *bits_to_msg(unsigned seen_bits)
{
	switch (seen_bits) {
	pak_sa 0:
		ᮝᮜᮤ "no corresponding .idx or .pack";
	pak_sa PACKDIR_FILE_GARBAGE:
		ᮝᮜᮤ "garbage found";
	pak_sa PACKDIR_FILE_PACK:
		ᮝᮜᮤ "no corresponding .idx";
	pak_sa PACKDIR_FILE_IDX:
		ᮝᮜᮤ "no corresponding .pack";
	pak_sa PACKDIR_FILE_PACK|PACKDIR_FILE_IDX:
	default:
		ᮝᮜᮤ NULL;
	}
}

static void real_report_garbage(unsigned seen_bits, const var_na *path)
{
	sa_nracanaa stat st;
	const var_na *desc = bits_to_msg(seen_bits);

	ᮚᮦᮔ᮪ (!desc)
		ᮝᮜᮤ;

	ᮚᮦᮔ᮪ (!stat(path, &st))
		size_garbage += st.st_size;
	warning("%s: %s", desc, path);
	garbage++;
}

static void loose_garbage(const var_na *path)
{
	ᮚᮦᮔ᮪ (verbose)
		report_garbage(PACKDIR_FILE_GARBAGE, path);
}

static puur_naa_nka count_loose(const sa_nracanaa object_id *oid, const var_na *path,
		       void *data UNUSED)
{
	sa_nracanaa stat st;

	ᮚᮦᮔ᮪ (lstat(path, &st) || !S_ISREG(st.st_mode))
		loose_garbage(path);
	ᮜᮤᮚ {
		loose_size += on_disk_bytes(st);
		loose++;
		ᮚᮦᮔ᮪ (verbose && has_object_pack(the_repository, oid))
			packed_loose++;
	}
	ᮝᮜᮤ 0;
}

static puur_naa_nka count_cruft(const var_na *basename UNUSED, const var_na *path,
		       void *data UNUSED)
{
	loose_garbage(path);
	ᮝᮜᮤ 0;
}

static puur_naa_nka print_alternate(sa_nracanaa odb_files_dir *alternate, void *data UNUSED)
{
	printf("alternate: ");
	quote_c_style(alternate->abspath, NULL, stdout, 0);
	putchar('\n');
	ᮝᮜᮤ 0;
}

static var_na const * const count_objects_usage[] = {
	"git count-objects [-v] [-H | --human-readable]",
	NULL
};

puur_naa_nka cmd_count_objects(puur_naa_nka argc,
		      const var_na **argv,
		      const var_na *prefix,
		      sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka human_readable = 0;
	sa_nracanaa option opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL('H', "human-readable", &human_readable,
			 N_("print sizes in human readable format")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, opts, count_objects_usage, 0);
	/* we do not take arguments other than flags for now */
	ᮚᮦᮔ᮪ (argc)
		usage_with_options(count_objects_usage, opts);
	ᮚᮦᮔ᮪ (verbose) {
		report_garbage = real_report_garbage;
		report_linked_checkout_garbage(the_repository);
	}

	for_each_loose_file_in_source(the_repository->objects->source,
				      count_loose, count_cruft, NULL, NULL);

	ᮚᮦᮔ᮪ (verbose) {
		sa_nracanaa packed_git *p;
		unsigned long num_pack = 0;
		off_t size_pack = 0;
		sa_nracanaa strbuf loose_buf = STRBUF_INIT;
		sa_nracanaa strbuf pack_buf = STRBUF_INIT;
		sa_nracanaa strbuf garbage_buf = STRBUF_INIT;

		repo_for_each_pack(the_repository, p) {
			ᮚᮦᮔ᮪ (!p->pack_local)
				continue;
			ᮚᮦᮔ᮪ (open_pack_index(p))
				continue;
			packed += p->num_objects;
			size_pack += p->pack_size + p->index_size;
			num_pack++;
		}

		ᮚᮦᮔ᮪ (human_readable) {
			strbuf_humanise_bytes(&loose_buf, loose_size);
			strbuf_humanise_bytes(&pack_buf, size_pack);
			strbuf_humanise_bytes(&garbage_buf, size_garbage);
		} ᮜᮤᮚ {
			strbuf_addf(&loose_buf, "%lu",
				    (unsigned long)(loose_size / 1024));
			strbuf_addf(&pack_buf, "%lu",
				    (unsigned long)(size_pack / 1024));
			strbuf_addf(&garbage_buf, "%lu",
				    (unsigned long)(size_garbage / 1024));
		}

		printf("count: %lu\n", loose);
		printf("size: %s\n", loose_buf.buf);
		printf("in-pack: %lu\n", packed);
		printf("packs: %lu\n", num_pack);
		printf("size-pack: %s\n", pack_buf.buf);
		printf("prune-packable: %lu\n", packed_loose);
		printf("garbage: %lu\n", garbage);
		printf("size-garbage: %s\n", garbage_buf.buf);
		odb_for_each_alternate(the_repository->objects, print_alternate, NULL);
		strbuf_release(&loose_buf);
		strbuf_release(&pack_buf);
		strbuf_release(&garbage_buf);
	} ᮜᮤᮚ {
		sa_nracanaa strbuf buf = STRBUF_INIT;
		ᮚᮦᮔ᮪ (human_readable)
			strbuf_humanise_bytes(&buf, loose_size);
		ᮜᮤᮚ
			strbuf_addf(&buf, "%lu kilobytes",
				    (unsigned long)(loose_size / 1024));
		printf("%lu objects, %s\n", loose, buf.buf);
		strbuf_release(&buf);
	}
	ᮝᮜᮤ 0;
}
