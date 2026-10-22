#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "pack.h"
#അന്തര്ഭാവ "parse-options.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം show_index_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	ശൂന്യ
};

പൂർണ്ണസംഖ്യ cmd_show_index(പൂർണ്ണസംഖ്യ argc,
		   സ്ഥിരം അക്ഷരം **argv,
		   സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i;
	unsigned nr;
	unsigned പൂർണ്ണസംഖ്യ version;
	സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ മുകൾ_index[256];
	unsigned hashsz;
	സ്ഥിരം അക്ഷരം *hash_name = ശൂന്യ;
	പൂർണ്ണസംഖ്യ hash_algo;
	സ്ഥിരം ഘടന option show_index_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, show_index_options, show_index_usage, 0);

	എങ്കിൽ (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		എങ്കിൽ (hash_algo == GIT_HASH_UNKNOWN)
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
	എങ്കിൽ (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	എങ്കിൽ (fread(മുകൾ_index, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	എങ്കിൽ (മുകൾ_index[0] == htonl(PACK_IDX_SIGNATURE)) {
		version = ntohl(മുകൾ_index[1]);
		എങ്കിൽ (version < 2 || version > 2)
			die(_("unknown index version"));
		എങ്കിൽ (fread(മുകൾ_index, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} അല്ലെങ്കിൽ {
		version = 1;
		എങ്കിൽ (fread(&മുകൾ_index[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	വേണ്ടി (i = 0; i < 256; i++) {
		unsigned n = ntohl(മുകൾ_index[i]);
		എങ്കിൽ (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	എങ്കിൽ (version == 1) {
		വേണ്ടി (i = 0; i < nr; i++) {
			unsigned പൂർണ്ണസംഖ്യ offset, entry[(GIT_MAX_RAWSZ + 4) / വലുപ്പം(unsigned പൂർണ്ണസംഖ്യ)];

			എങ്കിൽ (fread(entry, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			offset = ntohl(entry[0]);
			printf("%u %s\n", offset, hash_to_hex((ശൂന്യം *)(entry+1)));
		}
	} അല്ലെങ്കിൽ {
		unsigned off64_nr = 0;
		ഘടന {
			ഘടന object_id oid;
			uint32_t crc;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		വേണ്ടി (i = 0; i < nr; i++) {
			എങ്കിൽ (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		വേണ്ടി (i = 0; i < nr; i++)
			എങ്കിൽ (fread(&entries[i].crc, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		വേണ്ടി (i = 0; i < nr; i++)
			എങ്കിൽ (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		വേണ്ടി (i = 0; i < nr; i++) {
			uint64_t offset;
			uint32_t off = ntohl(entries[i].off);
			എങ്കിൽ (!(off & 0x80000000)) {
				offset = off;
			} അല്ലെങ്കിൽ {
				uint32_t off64[2];
				എങ്കിൽ ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				എങ്കിൽ (fread(off64, 8, 1, stdin) != 1)
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
		സ്വതന്ത്രം(entries);
	}
	തിരികെ 0;
}
