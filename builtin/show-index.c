#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "pack.h"
#அந்தர்பாவ "parse-options.h"

நிலையான மாறிலி எழுத்து *மாறிலி show_index_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	வெற்று
};

முழு cmd_show_index(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	முழு i;
	unsigned nr;
	unsigned முழு version;
	நிலையான unsigned முழு உச்சி_index[256];
	unsigned hashsz;
	மாறிலி எழுத்து *hash_name = வெற்று;
	முழு hash_algo;
	மாறிலி கட்டமைப்பு option show_index_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, show_index_options, show_index_usage, 0);

	எனில் (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		எனில் (hash_algo == GIT_HASH_UNKNOWN)
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
	எனில் (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	எனில் (fread(உச்சி_index, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	எனில் (உச்சி_index[0] == htonl(PACK_IDX_SIGNATURE)) {
		version = ntohl(உச்சி_index[1]);
		எனில் (version < 2 || version > 2)
			die(_("unknown index version"));
		எனில் (fread(உச்சி_index, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} இல்லையெனில் {
		version = 1;
		எனில் (fread(&உச்சி_index[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	க்கு (i = 0; i < 256; i++) {
		unsigned n = ntohl(உச்சி_index[i]);
		எனில் (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	எனில் (version == 1) {
		க்கு (i = 0; i < nr; i++) {
			unsigned முழு offset, entry[(GIT_MAX_RAWSZ + 4) / அளவு(unsigned முழு)];

			எனில் (fread(entry, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			offset = ntohl(entry[0]);
			printf("%u %s\n", offset, hash_to_hex((வெறுமை *)(entry+1)));
		}
	} இல்லையெனில் {
		unsigned off64_nr = 0;
		கட்டமைப்பு {
			கட்டமைப்பு object_id oid;
			uint32_t crc;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		க்கு (i = 0; i < nr; i++) {
			எனில் (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		க்கு (i = 0; i < nr; i++)
			எனில் (fread(&entries[i].crc, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		க்கு (i = 0; i < nr; i++)
			எனில் (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		க்கு (i = 0; i < nr; i++) {
			uint64_t offset;
			uint32_t off = ntohl(entries[i].off);
			எனில் (!(off & 0x80000000)) {
				offset = off;
			} இல்லையெனில் {
				uint32_t off64[2];
				எனில் ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				எனில் (fread(off64, 8, 1, stdin) != 1)
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
		விடுவி(entries);
	}
	திரும்பு 0;
}
