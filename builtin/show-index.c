#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ show_ਸੂਚਕਾਂਕ_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	ਨਿਰਾ
};

ਪੂਰਨ cmd_show_ਸੂਚਕਾਂਕ(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i;
	ਬਿਨਾਨਿਸ਼ਾਨੀ nr;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੰਸਕਰਣ;
	ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਖਰ_ਸੂਚਕਾਂਕ[256];
	ਬਿਨਾਨਿਸ਼ਾਨੀ hashsz;
	ਅਚਲ ਅੱਖਰ *hash_name = ਨਿਰਾ;
	ਪੂਰਨ hash_algo;
	ਅਚਲ ਬਣਤਰ option show_ਸੂਚਕਾਂਕ_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, show_ਸੂਚਕਾਂਕ_options, show_ਸੂਚਕਾਂਕ_usage, 0);

	ਜੇ (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		ਜੇ (hash_algo == GIT_HASH_UNKNOWN)
			die(_("Unknown hash algorithm"));
		repo_ਸੈੱਟ_hash_algo(the_repository, hash_algo);
	}

	/*
	 * Fallback to SHA1 if we are running outside of a repository.
	 *
	 * TODO: If a future implementation of index file version encodes the hash
	 *       algorithm in its header, enable show-index to infer it from the
	 *       header rather than relying on repository context or a default fallback.
	 */
	ਜੇ (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_ਸੈੱਟ_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	ਜੇ (fread(ਸਿਖਰ_ਸੂਚਕਾਂਕ, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	ਜੇ (ਸਿਖਰ_ਸੂਚਕਾਂਕ[0] == htonl(PACK_IDX_SIGNATURE)) {
		ਸੰਸਕਰਣ = ntohl(ਸਿਖਰ_ਸੂਚਕਾਂਕ[1]);
		ਜੇ (ਸੰਸਕਰਣ < 2 || ਸੰਸਕਰਣ > 2)
			die(_("unknown index version"));
		ਜੇ (fread(ਸਿਖਰ_ਸੂਚਕਾਂਕ, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} ਨਹੀਂਤਾਂ {
		ਸੰਸਕਰਣ = 1;
		ਜੇ (fread(&ਸਿਖਰ_ਸੂਚਕਾਂਕ[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	ਲਈ (i = 0; i < 256; i++) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ n = ntohl(ਸਿਖਰ_ਸੂਚਕਾਂਕ[i]);
		ਜੇ (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	ਜੇ (ਸੰਸਕਰਣ == 1) {
		ਲਈ (i = 0; i < nr; i++) {
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ, ਇੰਦਰਾਜ[(GIT_MAX_RAWSZ + 4) / ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)];

			ਜੇ (fread(ਇੰਦਰਾਜ, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			ਵਿੱਥ = ntohl(ਇੰਦਰਾਜ[0]);
			ਲਿਖੋ("%u %s\n", ਵਿੱਥ, hash_ਮੰਜ਼ਿਲ_hex((ਖਾਲੀ *)(ਇੰਦਰਾਜ+1)));
		}
	} ਨਹੀਂਤਾਂ {
		ਬਿਨਾਨਿਸ਼ਾਨੀ off64_nr = 0;
		ਬਣਤਰ {
			ਬਣਤਰ object_id oid;
			uint32_t ਚੱਕਰੀ_ਜਾਂਚ;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		ਲਈ (i = 0; i < nr; i++) {
			ਜੇ (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		ਲਈ (i = 0; i < nr; i++)
			ਜੇ (fread(&entries[i].ਚੱਕਰੀ_ਜਾਂਚ, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		ਲਈ (i = 0; i < nr; i++)
			ਜੇ (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		ਲਈ (i = 0; i < nr; i++) {
			uint64_t ਵਿੱਥ;
			uint32_t off = ntohl(entries[i].off);
			ਜੇ (!(off & 0x80000000)) {
				ਵਿੱਥ = off;
			} ਨਹੀਂਤਾਂ {
				uint32_t off64[2];
				ਜੇ ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				ਜੇ (fread(off64, 8, 1, stdin) != 1)
					die(_("unable to read 64b offset %u"), off64_nr);
				ਵਿੱਥ = (((uint64_t)ntohl(off64[0])) << 32) |
						     ntohl(off64[1]);
				off64_nr++;
			}
			ਲਿਖੋ("%" PRIuMAX " %s (%08"PRIx32")\n",
			       (uintmax_t) ਵਿੱਥ,
			       oid_ਮੰਜ਼ਿਲ_hex(&entries[i].oid),
			       ntohl(entries[i].ਚੱਕਰੀ_ਜਾਂਚ));
		}
		ਮੁਕਤ(entries);
	}
	ਵਾਪਸ 0;
}
