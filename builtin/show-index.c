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

ကိန်းပြည့် cmd_show_index(ကိန်းပြည့် argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် i;
	unsigned nr;
	unsigned ကိန်းပြည့် version;
	static unsigned ကိန်းပြည့် top_index[256];
	unsigned hashsz;
	const var_na *hash_name = NULL;
	ကိန်းပြည့် hash_algo;
	const sa_nracanaa option show_index_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, show_index_options, show_index_usage, 0);

	အကယ်၍ (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		အကယ်၍ (hash_algo == GIT_HASH_UNKNOWN)
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
	အကယ်၍ (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	အကယ်၍ (fread(top_index, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	အကယ်၍ (top_index[0] == htonl(PACK_IDX_SIGNATURE)) {
		version = ntohl(top_index[1]);
		အကယ်၍ (version < 2 || version > 2)
			die(_("unknown index version"));
		အကယ်၍ (fread(top_index, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} မဟုတ်ပါက {
		version = 1;
		အကယ်၍ (fread(&top_index[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	အတွက် (i = 0; i < 256; i++) {
		unsigned n = ntohl(top_index[i]);
		အကယ်၍ (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	အကယ်၍ (version == 1) {
		အတွက် (i = 0; i < nr; i++) {
			unsigned ကိန်းပြည့် offset, entry[(GIT_MAX_RAWSZ + 4) / parimaa_na(unsigned ကိန်းပြည့်)];

			အကယ်၍ (fread(entry, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			offset = ntohl(entry[0]);
			printf("%u %s\n", offset, hash_to_hex((void *)(entry+1)));
		}
	} မဟုတ်ပါက {
		unsigned off64_nr = 0;
		sa_nracanaa {
			sa_nracanaa object_id oid;
			uint32_t crc;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		အတွက် (i = 0; i < nr; i++) {
			အကယ်၍ (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		အတွက် (i = 0; i < nr; i++)
			အကယ်၍ (fread(&entries[i].crc, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		အတွက် (i = 0; i < nr; i++)
			အကယ်၍ (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		အတွက် (i = 0; i < nr; i++) {
			uint64_t offset;
			uint32_t off = ntohl(entries[i].off);
			အကယ်၍ (!(off & 0x80000000)) {
				offset = off;
			} မဟုတ်ပါက {
				uint32_t off64[2];
				အကယ်၍ ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				အကယ်၍ (fread(off64, 8, 1, stdin) != 1)
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
	ပြန်ပေး 0;
}
