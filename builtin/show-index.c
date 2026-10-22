#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "pack.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ show_ಸೂಚ್ಯಂಕ_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	ಖಾಲಿ
};

ಪೂರ್ಣಾಂಕ cmd_show_ಸೂಚ್ಯಂಕ(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i;
	ಚಿಹ್ನೆರಹಿತ nr;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ;
	ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ತುದಿ_ಸೂಚ್ಯಂಕ[256];
	ಚಿಹ್ನೆರಹಿತ hashsz;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hash_name = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ hash_algo;
	ಸ್ಥಿರಾಂಕ ರಚನೆ option show_ಸೂಚ್ಯಂಕ_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, show_ಸೂಚ್ಯಂಕ_options, show_ಸೂಚ್ಯಂಕ_usage, 0);

	ಆದರೆ (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		ಆದರೆ (hash_algo == GIT_HASH_UNKNOWN)
			die(_("Unknown hash algorithm"));
		repo_ಗಣ_hash_algo(the_repository, hash_algo);
	}

	/*
	 * Fallback to SHA1 if we are running outside of a repository.
	 *
	 * TODO: If a future implementation of index file version encodes the hash
	 *       algorithm in its header, enable show-index to infer it from the
	 *       header rather than relying on repository context or a default fallback.
	 */
	ಆದರೆ (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_ಗಣ_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	ಆದರೆ (fread(ತುದಿ_ಸೂಚ್ಯಂಕ, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	ಆದರೆ (ತುದಿ_ಸೂಚ್ಯಂಕ[0] == htonl(PACK_IDX_SIGNATURE)) {
		ಆವೃತ್ತಿ = ntohl(ತುದಿ_ಸೂಚ್ಯಂಕ[1]);
		ಆದರೆ (ಆವೃತ್ತಿ < 2 || ಆವೃತ್ತಿ > 2)
			die(_("unknown index version"));
		ಆದರೆ (fread(ತುದಿ_ಸೂಚ್ಯಂಕ, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆವೃತ್ತಿ = 1;
		ಆದರೆ (fread(&ತುದಿ_ಸೂಚ್ಯಂಕ[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	ಗಾಗಿ (i = 0; i < 256; i++) {
		ಚಿಹ್ನೆರಹಿತ n = ntohl(ತುದಿ_ಸೂಚ್ಯಂಕ[i]);
		ಆದರೆ (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	ಆದರೆ (ಆವೃತ್ತಿ == 1) {
		ಗಾಗಿ (i = 0; i < nr; i++) {
			ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ, ನಮೂದು[(GIT_MAX_RAWSZ + 4) / ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)];

			ಆದರೆ (fread(ನಮೂದು, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			ಸ್ಥಾನಾಂತರ = ntohl(ನಮೂದು[0]);
			ಬರೆ("%u %s\n", ಸ್ಥಾನಾಂತರ, hash_ಗಮ್ಯ_hex((ಶೂನ್ಯ *)(ನಮೂದು+1)));
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಚಿಹ್ನೆರಹಿತ off64_nr = 0;
		ರಚನೆ {
			ರಚನೆ object_id oid;
			uint32_t ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		ಗಾಗಿ (i = 0; i < nr; i++) {
			ಆದರೆ (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		ಗಾಗಿ (i = 0; i < nr; i++)
			ಆದರೆ (fread(&entries[i].ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		ಗಾಗಿ (i = 0; i < nr; i++)
			ಆದರೆ (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		ಗಾಗಿ (i = 0; i < nr; i++) {
			uint64_t ಸ್ಥಾನಾಂತರ;
			uint32_t off = ntohl(entries[i].off);
			ಆದರೆ (!(off & 0x80000000)) {
				ಸ್ಥಾನಾಂತರ = off;
			} ಇಲ್ಲದಿದ್ದರೆ {
				uint32_t off64[2];
				ಆದರೆ ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				ಆದರೆ (fread(off64, 8, 1, stdin) != 1)
					die(_("unable to read 64b offset %u"), off64_nr);
				ಸ್ಥಾನಾಂತರ = (((uint64_t)ntohl(off64[0])) << 32) |
						     ntohl(off64[1]);
				off64_nr++;
			}
			ಬರೆ("%" PRIuMAX " %s (%08"PRIx32")\n",
			       (uintmax_t) ಸ್ಥಾನಾಂತರ,
			       oid_ಗಮ್ಯ_hex(&entries[i].oid),
			       ntohl(entries[i].ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ));
		}
		ಬಿಡುಗಡೆ(entries);
	}
	ಹಿಂತಿರುಗಿ 0;
}
