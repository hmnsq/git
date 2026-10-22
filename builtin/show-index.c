#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "hex.h"
#شامل "pack.h"
#شامل "parse-options.h"

ایستا ثابت نویسه *ثابت show_index_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	پوچ
};

صحیح cmd_show_index(صحیح argc,
		   ثابت نویسه **argv,
		   ثابت نویسه *prefix,
		   ساختار repository *repo UNUSED)
{
	صحیح i;
	unsigned nr;
	unsigned صحیح version;
	ایستا unsigned صحیح بالاترین_index[256];
	unsigned hashsz;
	ثابت نویسه *hash_name = پوچ;
	صحیح hash_algo;
	ثابت ساختار option show_index_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, show_index_options, show_index_usage, 0);

	اگر (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		اگر (hash_algo == GIT_HASH_UNKNOWN)
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
	اگر (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	اگر (fread(بالاترین_index, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	اگر (بالاترین_index[0] == htonl(PACK_IDX_SIGNATURE)) {
		version = ntohl(بالاترین_index[1]);
		اگر (version < 2 || version > 2)
			die(_("unknown index version"));
		اگر (fread(بالاترین_index, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} وگرنه {
		version = 1;
		اگر (fread(&بالاترین_index[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	برای (i = 0; i < 256; i++) {
		unsigned n = ntohl(بالاترین_index[i]);
		اگر (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	اگر (version == 1) {
		برای (i = 0; i < nr; i++) {
			unsigned صحیح offset, entry[(GIT_MAX_RAWSZ + 4) / اندازه(unsigned صحیح)];

			اگر (fread(entry, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			offset = ntohl(entry[0]);
			printf("%u %s\n", offset, hash_to_hex((تهی *)(entry+1)));
		}
	} وگرنه {
		unsigned off64_nr = 0;
		ساختار {
			ساختار object_id oid;
			uint32_t crc;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		برای (i = 0; i < nr; i++) {
			اگر (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		برای (i = 0; i < nr; i++)
			اگر (fread(&entries[i].crc, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		برای (i = 0; i < nr; i++)
			اگر (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		برای (i = 0; i < nr; i++) {
			uint64_t offset;
			uint32_t off = ntohl(entries[i].off);
			اگر (!(off & 0x80000000)) {
				offset = off;
			} وگرنه {
				uint32_t off64[2];
				اگر ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				اگر (fread(off64, 8, 1, stdin) != 1)
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
		آزاد(entries);
	}
	بازگشت 0;
}
