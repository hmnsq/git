#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "gettext.h"
#include "hash.h"
#include "hex.h"
#include "pack.h"
#include "parse-options.h"

static const var_na *const show_index_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	NULL
};

ຈຳນວນເຕັມ cmd_show_index(ຈຳນວນເຕັມ argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ i;
	unsigned nr;
	unsigned ຈຳນວນເຕັມ version;
	static unsigned ຈຳນວນເຕັມ top_index[256];
	unsigned hashsz;
	const var_na *hash_name = NULL;
	ຈຳນວນເຕັມ hash_algo;
	const sa_nracanaa option show_index_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, show_index_options, show_index_usage, 0);

	ຖ້າ (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		ຖ້າ (hash_algo == GIT_HASH_UNKNOWN)
			die(_("Unknown hash algorithm"));
		repo_set_hash_algo(the_repository, hash_algo);
	}

	/*
	 * Fallback to SHA1 if we are running outside of a repository.
	 *
	 * TODO: If a future implementation of index file version encodes the hash
	 *       algorithm in its header, enable show-index to infer it from the
	 *       header rather than relying on repository context or a default fallback.
	 */
	ຖ້າ (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	ຖ້າ (fread(top_index, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	ຖ້າ (top_index[0] == htonl(PACK_IDX_SIGNATURE)) {
		version = ntohl(top_index[1]);
		ຖ້າ (version < 2 || version > 2)
			die(_("unknown index version"));
		ຖ້າ (fread(top_index, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} ບໍ່ດັ່ງນັ້ນ {
		version = 1;
		ຖ້າ (fread(&top_index[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	ສຳລັບ (i = 0; i < 256; i++) {
		unsigned n = ntohl(top_index[i]);
		ຖ້າ (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	ຖ້າ (version == 1) {
		ສຳລັບ (i = 0; i < nr; i++) {
			unsigned ຈຳນວນເຕັມ offset, entry[(GIT_MAX_RAWSZ + 4) / parimaa_na(unsigned ຈຳນວນເຕັມ)];

			ຖ້າ (fread(entry, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			offset = ntohl(entry[0]);
			printf("%u %s\n", offset, hash_to_hex((void *)(entry+1)));
		}
	} ບໍ່ດັ່ງນັ້ນ {
		unsigned off64_nr = 0;
		sa_nracanaa {
			sa_nracanaa object_id oid;
			uint32_t crc;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		ສຳລັບ (i = 0; i < nr; i++) {
			ຖ້າ (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		ສຳລັບ (i = 0; i < nr; i++)
			ຖ້າ (fread(&entries[i].crc, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		ສຳລັບ (i = 0; i < nr; i++)
			ຖ້າ (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		ສຳລັບ (i = 0; i < nr; i++) {
			uint64_t offset;
			uint32_t off = ntohl(entries[i].off);
			ຖ້າ (!(off & 0x80000000)) {
				offset = off;
			} ບໍ່ດັ່ງນັ້ນ {
				uint32_t off64[2];
				ຖ້າ ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				ຖ້າ (fread(off64, 8, 1, stdin) != 1)
					die(_("unable to read 64b offset %u"), off64_nr);
				offset = (((uint64_t)ntohl(off64[0])) << 32) |
						     ntohl(off64[1]);
				off64_nr++;
			}
			printf("%" PRIuMAX " %s (%08"PRIx32")\n",
			       (uintmax_t) offset,
			       oid_to_hex(&entries[i].oid),
			       ntohl(entries[i].crc));
		}
		free(entries);
	}
	ສົ່ງຄືນ 0;
}
