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

ཧྲིལ cmd_show_index(ཧྲིལ argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ i;
	unsigned nr;
	unsigned ཧྲིལ version;
	static unsigned ཧྲིལ top_index[256];
	unsigned hashsz;
	const var_na *hash_name = NULL;
	ཧྲིལ hash_algo;
	const sa_nracanaa option show_index_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, show_index_options, show_index_usage, 0);

	གལ་སྲིད (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		གལ་སྲིད (hash_algo == GIT_HASH_UNKNOWN)
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
	གལ་སྲིད (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	གལ་སྲིད (fread(top_index, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	གལ་སྲིད (top_index[0] == htonl(PACK_IDX_SIGNATURE)) {
		version = ntohl(top_index[1]);
		གལ་སྲིད (version < 2 || version > 2)
			die(_("unknown index version"));
		གལ་སྲིད (fread(top_index, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} དེ་མིན {
		version = 1;
		གལ་སྲིད (fread(&top_index[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	དོན་ལུ (i = 0; i < 256; i++) {
		unsigned n = ntohl(top_index[i]);
		གལ་སྲིད (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	གལ་སྲིད (version == 1) {
		དོན་ལུ (i = 0; i < nr; i++) {
			unsigned ཧྲིལ offset, entry[(GIT_MAX_RAWSZ + 4) / parimaa_na(unsigned ཧྲིལ)];

			གལ་སྲིད (fread(entry, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			offset = ntohl(entry[0]);
			printf("%u %s\n", offset, hash_to_hex((void *)(entry+1)));
		}
	} དེ་མིན {
		unsigned off64_nr = 0;
		sa_nracanaa {
			sa_nracanaa object_id oid;
			uint32_t crc;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		དོན་ལུ (i = 0; i < nr; i++) {
			གལ་སྲིད (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		དོན་ལུ (i = 0; i < nr; i++)
			གལ་སྲིད (fread(&entries[i].crc, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		དོན་ལུ (i = 0; i < nr; i++)
			གལ་སྲིད (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		དོན་ལུ (i = 0; i < nr; i++) {
			uint64_t offset;
			uint32_t off = ntohl(entries[i].off);
			གལ་སྲིད (!(off & 0x80000000)) {
				offset = off;
			} དེ་མིན {
				uint32_t off64[2];
				གལ་སྲིད ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				གལ་སྲིད (fread(off64, 8, 1, stdin) != 1)
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
	ལོག 0;
}
