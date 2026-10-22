#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "pack.h"
#અન્તર્ભાવ "parse-options.h"

સ્થિર અચલ અક્ષર *અચલ show_index_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	ખાલી
};

પૂર્ણાંક cmd_show_index(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *prefix,
		   સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i;
	unsigned nr;
	unsigned પૂર્ણાંક version;
	સ્થિર unsigned પૂર્ણાંક ટોચ_index[256];
	unsigned hashsz;
	અચલ અક્ષર *hash_name = ખાલી;
	પૂર્ણાંક hash_algo;
	અચલ સંરચના option show_index_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, show_index_options, show_index_usage, 0);

	જો (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		જો (hash_algo == GIT_HASH_UNKNOWN)
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
	જો (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	જો (fread(ટોચ_index, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	જો (ટોચ_index[0] == htonl(PACK_IDX_SIGNATURE)) {
		version = ntohl(ટોચ_index[1]);
		જો (version < 2 || version > 2)
			die(_("unknown index version"));
		જો (fread(ટોચ_index, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} નહીંતર {
		version = 1;
		જો (fread(&ટોચ_index[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	માટે (i = 0; i < 256; i++) {
		unsigned n = ntohl(ટોચ_index[i]);
		જો (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	જો (version == 1) {
		માટે (i = 0; i < nr; i++) {
			unsigned પૂર્ણાંક offset, entry[(GIT_MAX_RAWSZ + 4) / કદ(unsigned પૂર્ણાંક)];

			જો (fread(entry, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			offset = ntohl(entry[0]);
			printf("%u %s\n", offset, hash_to_hex((શૂન્ય *)(entry+1)));
		}
	} નહીંતર {
		unsigned off64_nr = 0;
		સંરચના {
			સંરચના object_id oid;
			uint32_t crc;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		માટે (i = 0; i < nr; i++) {
			જો (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		માટે (i = 0; i < nr; i++)
			જો (fread(&entries[i].crc, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		માટે (i = 0; i < nr; i++)
			જો (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		માટે (i = 0; i < nr; i++) {
			uint64_t offset;
			uint32_t off = ntohl(entries[i].off);
			જો (!(off & 0x80000000)) {
				offset = off;
			} નહીંતર {
				uint32_t off64[2];
				જો ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				જો (fread(off64, 8, 1, stdin) != 1)
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
		મુક્ત(entries);
	}
	પાછા 0;
}
