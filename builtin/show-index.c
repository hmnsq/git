#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "pack.h"
#অন্তর্ভাব "parse-options.h"

স্থির ধ্রুবক অক্ষর *ধ্রুবক show_সূচক_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	ফাঁকা
};

পূর্ণসংখ্যা cmd_show_সূচক(পূর্ণসংখ্যা argc,
		   ধ্রুবক অক্ষর **argv,
		   ধ্রুবক অক্ষর *prefix,
		   কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা i;
	unsigned nr;
	unsigned পূর্ণসংখ্যা version;
	স্থির unsigned পূর্ণসংখ্যা শীর্ষ_সূচক[256];
	unsigned hashsz;
	ধ্রুবক অক্ষর *hash_name = ফাঁকা;
	পূর্ণসংখ্যা hash_algo;
	ধ্রুবক কাঠামো option show_সূচক_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, show_সূচক_options, show_সূচক_usage, 0);

	যদি (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		যদি (hash_algo == GIT_HASH_UNKNOWN)
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
	যদি (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	যদি (fread(শীর্ষ_সূচক, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	যদি (শীর্ষ_সূচক[0] == htonl(PACK_IDX_SIGNATURE)) {
		version = ntohl(শীর্ষ_সূচক[1]);
		যদি (version < 2 || version > 2)
			die(_("unknown index version"));
		যদি (fread(শীর্ষ_সূচক, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} অন্যথা {
		version = 1;
		যদি (fread(&শীর্ষ_সূচক[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	জন্য (i = 0; i < 256; i++) {
		unsigned n = ntohl(শীর্ষ_সূচক[i]);
		যদি (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	যদি (version == 1) {
		জন্য (i = 0; i < nr; i++) {
			unsigned পূর্ণসংখ্যা offset, entry[(GIT_MAX_RAWSZ + 4) / আকার(unsigned পূর্ণসংখ্যা)];

			যদি (fread(entry, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			offset = ntohl(entry[0]);
			printf("%u %s\n", offset, hash_to_hex((শূন্য *)(entry+1)));
		}
	} অন্যথা {
		unsigned off64_nr = 0;
		কাঠামো {
			কাঠামো object_id oid;
			uint32_t crc;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		জন্য (i = 0; i < nr; i++) {
			যদি (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		জন্য (i = 0; i < nr; i++)
			যদি (fread(&entries[i].crc, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		জন্য (i = 0; i < nr; i++)
			যদি (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		জন্য (i = 0; i < nr; i++) {
			uint64_t offset;
			uint32_t off = ntohl(entries[i].off);
			যদি (!(off & 0x80000000)) {
				offset = off;
			} অন্যথা {
				uint32_t off64[2];
				যদি ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				যদি (fread(off64, 8, 1, stdin) != 1)
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
		মুক্ত(entries);
	}
	ফেরত 0;
}
