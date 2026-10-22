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
static цяло verbose;
static unsigned long loose, packed, packed_loose;
static off_t loose_size;

static const char *bits_to_msg(unsigned seen_bits)
{
	switch (seen_bits) {
	case 0:
		върни "no corresponding .idx or .pack";
	case PACKDIR_FILE_GARBAGE:
		върни "garbage found";
	case PACKDIR_FILE_PACK:
		върни "no corresponding .idx";
	case PACKDIR_FILE_IDX:
		върни "no corresponding .pack";
	case PACKDIR_FILE_PACK|PACKDIR_FILE_IDX:
	default:
		върни NULL;
	}
}

static void real_report_garbage(unsigned seen_bits, const char *path)
{
	struct stat st;
	const char *desc = bits_to_msg(seen_bits);

	ако (!desc)
		върни;

	ако (!stat(path, &st))
		size_garbage += st.st_size;
	warning("%s: %s", desc, path);
	garbage++;
}

static void loose_garbage(const char *path)
{
	ако (verbose)
		report_garbage(PACKDIR_FILE_GARBAGE, path);
}

static цяло count_loose(const struct object_id *oid, const char *path,
		       void *data UNUSED)
{
	struct stat st;

	ако (lstat(path, &st) || !S_ISREG(st.st_mode))
		loose_garbage(path);
	иначе {
		loose_size += on_disk_bytes(st);
		loose++;
		ако (verbose && has_object_pack(the_repository, oid))
			packed_loose++;
	}
	върни 0;
}

static цяло count_cruft(const char *basename UNUSED, const char *path,
		       void *data UNUSED)
{
	loose_garbage(path);
	върни 0;
}

static цяло print_alternate(struct odb_files_dir *alternate, void *data UNUSED)
{
	printf("alternate: ");
	quote_c_style(alternate->abspath, NULL, stdout, 0);
	putchar('\n');
	върни 0;
}

static char const * const count_objects_usage[] = {
	"git count-objects [-v] [-H | --human-readable]",
	NULL
};

цяло cmd_count_objects(цяло argc,
		      const char **argv,
		      const char *prefix,
		      struct repository *repo UNUSED)
{
	цяло human_readable = 0;
	struct option opts[] = {
		OPT__VERBOSE(&verbose, N_("be verbose")),
		OPT_BOOL('H', "human-readable", &human_readable,
			 N_("print sizes in human readable format")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, opts, count_objects_usage, 0);
	/* we do not take arguments other than flags for now */
	ако (argc)
		usage_with_options(count_objects_usage, opts);
	ако (verbose) {
		report_garbage = real_report_garbage;
		report_linked_checkout_garbage(the_repository);
	}

	for_each_loose_file_in_source(the_repository->objects->source,
				      count_loose, count_cruft, NULL, NULL);

	ако (verbose) {
		struct packed_git *p;
		unsigned long num_pack = 0;
		off_t size_pack = 0;
		struct strbuf loose_buf = STRBUF_INIT;
		struct strbuf pack_buf = STRBUF_INIT;
		struct strbuf garbage_buf = STRBUF_INIT;

		repo_for_each_pack(the_repository, p) {
			ако (!p->pack_local)
				continue;
			ако (open_pack_index(p))
				continue;
			packed += p->num_objects;
			size_pack += p->pack_size + p->index_size;
			num_pack++;
		}

		ако (human_readable) {
			strbuf_humanise_bytes(&loose_buf, loose_size);
			strbuf_humanise_bytes(&pack_buf, size_pack);
			strbuf_humanise_bytes(&garbage_buf, size_garbage);
		} иначе {
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
	} иначе {
		struct strbuf buf = STRBUF_INIT;
		ако (human_readable)
			strbuf_humanise_bytes(&buf, loose_size);
		иначе
			strbuf_addf(&buf, "%lu kilobytes",
				    (unsigned long)(loose_size / 1024));
		printf("%lu objects, %s\n", loose, buf.buf);
		strbuf_release(&buf);
	}
	върни 0;
}
