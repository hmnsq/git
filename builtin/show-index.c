#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "hex.h"
#ضمن "pack.h"
#ضمن "parse-options.h"

ساكن ثابت حرف *ثابت show_اشاریہ_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	عدم
};

صحيح cmd_show_اشاریہ(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *اگیتر,
		   بنية repository *repo بن_ورتیا)
{
	صحيح i;
	بنانشانی nr;
	بنانشانی صحيح نسخہ;
	ساكن بنانشانی صحيح چوٹی_اشاریہ[256];
	بنانشانی hashsz;
	ثابت حرف *hash_name = عدم;
	صحيح hash_algo;
	ثابت بنية option show_اشاریہ_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, show_اشاریہ_options, show_اشاریہ_usage, 0);

	إذا (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		إذا (hash_algo == GIT_HASH_UNKNOWN)
			die(_("Unknown hash algorithm"));
		repo_سیٹ_hash_algo(the_repository, hash_algo);
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
		repo_سیٹ_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	إذا (fread(چوٹی_اشاریہ, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	إذا (چوٹی_اشاریہ[0] == htonl(PACK_IDX_SIGNATURE)) {
		نسخہ = ntohl(چوٹی_اشاریہ[1]);
		إذا (نسخہ < 2 || نسخہ > 2)
			die(_("unknown index version"));
		إذا (fread(چوٹی_اشاریہ, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} وإلا {
		نسخہ = 1;
		إذا (fread(&چوٹی_اشاریہ[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	لكل (i = 0; i < 256; i++) {
		بنانشانی n = ntohl(چوٹی_اشاریہ[i]);
		إذا (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	إذا (نسخہ == 1) {
		لكل (i = 0; i < nr; i++) {
			بنانشانی صحيح وتھ, اندراج[(GIT_MAX_RAWSZ + 4) / حجم(بنانشانی صحيح)];

			إذا (fread(اندراج, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			وتھ = ntohl(اندراج[0]);
			لکھو("%u %s\n", وتھ, hash_منزل_hex((فراغ *)(اندراج+1)));
		}
	} وإلا {
		بنانشانی off64_nr = 0;
		بنية {
			بنية object_id oid;
			uint32_t چکری_جانچ;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		لكل (i = 0; i < nr; i++) {
			إذا (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		لكل (i = 0; i < nr; i++)
			إذا (fread(&entries[i].چکری_جانچ, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		لكل (i = 0; i < nr; i++)
			إذا (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		لكل (i = 0; i < nr; i++) {
			uint64_t وتھ;
			uint32_t off = ntohl(entries[i].off);
			إذا (!(off & 0x80000000)) {
				وتھ = off;
			} وإلا {
				uint32_t off64[2];
				إذا ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				إذا (fread(off64, 8, 1, stdin) != 1)
					die(_("unable to read 64b offset %u"), off64_nr);
				وتھ = (((uint64_t)ntohl(off64[0])) << 32) |
						     ntohl(off64[1]);
				off64_nr++;
			}
			لکھو("%" PRIuMAX " %s (%08"PRIx32")\n",
			       (uintmax_t) وتھ,
			       oid_منزل_hex(&entries[i].oid),
			       ntohl(entries[i].چکری_جانچ));
		}
		حرر(entries);
	}
	أرجع 0;
}
