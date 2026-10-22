#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "hex.h"
#ضمن "pack.h"
#ضمن "parse-options.h"

ساكن ثابت حرف *ثابت show_index_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	عدم
};

صحيح cmd_show_index(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *prefix,
		   بنية repository *repo UNUSED)
{
	صحيح i;
	unsigned nr;
	unsigned صحيح version;
	ساكن unsigned صحيح كولي_index[256];
	unsigned hashsz;
	ثابت حرف *hash_name = عدم;
	صحيح hash_algo;
	ثابت بنية option show_index_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, show_index_options, show_index_usage, 0);

	إذا (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		إذا (hash_algo == GIT_HASH_UNKNOWN)
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
	إذا (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	إذا (fread(كولي_index, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	إذا (كولي_index[0] == htonl(PACK_IDX_SIGNATURE)) {
		version = ntohl(كولي_index[1]);
		إذا (version < 2 || version > 2)
			die(_("unknown index version"));
		إذا (fread(كولي_index, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} وإلا {
		version = 1;
		إذا (fread(&كولي_index[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	لكل (i = 0; i < 256; i++) {
		unsigned n = ntohl(كولي_index[i]);
		إذا (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	إذا (version == 1) {
		لكل (i = 0; i < nr; i++) {
			unsigned صحيح offset, entry[(GIT_MAX_RAWSZ + 4) / حجم(unsigned صحيح)];

			إذا (fread(entry, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			offset = ntohl(entry[0]);
			printf("%u %s\n", offset, hash_to_hex((فراغ *)(entry+1)));
		}
	} وإلا {
		unsigned off64_nr = 0;
		بنية {
			بنية object_id oid;
			uint32_t crc;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		لكل (i = 0; i < nr; i++) {
			إذا (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		لكل (i = 0; i < nr; i++)
			إذا (fread(&entries[i].crc, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		لكل (i = 0; i < nr; i++)
			إذا (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		لكل (i = 0; i < nr; i++) {
			uint64_t offset;
			uint32_t off = ntohl(entries[i].off);
			إذا (!(off & 0x80000000)) {
				offset = off;
			} وإلا {
				uint32_t off64[2];
				إذا ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				إذا (fread(off64, 8, 1, stdin) != 1)
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
		حرر(entries);
	}
	أرجع 0;
}
