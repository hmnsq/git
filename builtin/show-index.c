#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "hex.h"
#شامل "pack.h"
#شامل "parse-options.h"

ساکن ثابت حرف *ثابت show_اشاریہ_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	تہی
};

صحیح cmd_show_اشاریہ(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *سابقہ,
		   ساخت repository *repo غیر_مستعمل)
{
	صحیح i;
	غیرعلامتی nr;
	غیرعلامتی صحیح نسخہ;
	ساکن غیرعلامتی صحیح چوٹی_اشاریہ[256];
	غیرعلامتی hashsz;
	ثابت حرف *hash_name = تہی;
	صحیح hash_algo;
	ثابت ساخت option show_اشاریہ_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, show_اشاریہ_options, show_اشاریہ_usage, 0);

	اگر (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		اگر (hash_algo == GIT_HASH_UNKNOWN)
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
	اگر (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_سیٹ_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	اگر (fread(چوٹی_اشاریہ, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	اگر (چوٹی_اشاریہ[0] == htonl(PACK_IDX_SIGNATURE)) {
		نسخہ = ntohl(چوٹی_اشاریہ[1]);
		اگر (نسخہ < 2 || نسخہ > 2)
			die(_("unknown index version"));
		اگر (fread(چوٹی_اشاریہ, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} ورنہ {
		نسخہ = 1;
		اگر (fread(&چوٹی_اشاریہ[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	برائے (i = 0; i < 256; i++) {
		غیرعلامتی n = ntohl(چوٹی_اشاریہ[i]);
		اگر (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	اگر (نسخہ == 1) {
		برائے (i = 0; i < nr; i++) {
			غیرعلامتی صحیح انحراف, اندراج_2[(GIT_MAX_RAWSZ + 4) / حجم(غیرعلامتی صحیح)];

			اگر (fread(اندراج_2, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			انحراف = ntohl(اندراج_2[0]);
			لکھو("%u %s\n", انحراف, hash_مقصد_hex((خالی *)(اندراج_2+1)));
		}
	} ورنہ {
		غیرعلامتی off64_nr = 0;
		ساخت {
			ساخت object_id oid;
			uint32_t چکری_جانچ;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		برائے (i = 0; i < nr; i++) {
			اگر (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		برائے (i = 0; i < nr; i++)
			اگر (fread(&entries[i].چکری_جانچ, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		برائے (i = 0; i < nr; i++)
			اگر (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		برائے (i = 0; i < nr; i++) {
			uint64_t انحراف;
			uint32_t off = ntohl(entries[i].off);
			اگر (!(off & 0x80000000)) {
				انحراف = off;
			} ورنہ {
				uint32_t off64[2];
				اگر ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				اگر (fread(off64, 8, 1, stdin) != 1)
					die(_("unable to read 64b offset %u"), off64_nr);
				انحراف = (((uint64_t)ntohl(off64[0])) << 32) |
						     ntohl(off64[1]);
				off64_nr++;
			}
			لکھو("%" PRIuMAX " %s (%08"PRIx32")\n",
			       (uintmax_t) انحراف,
			       oid_مقصد_hex(&entries[i].oid),
			       ntohl(entries[i].چکری_جانچ));
		}
		آزاد(entries);
	}
	واپس 0;
}
