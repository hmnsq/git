#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "hex.h"
#ضمن "pack.h"
#ضمن "parse-options.h"

ساكن ثابت حرف *ثابت show_شاخص_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	عدم
};

صحيح cmd_show_شاخص(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *مختاړی,
		   بنية repository *repo ناکارول_شوی)
{
	صحيح i;
	بینښه nr;
	بینښه صحيح نسخه;
	ساكن بینښه صحيح سر_شاخص[256];
	بینښه hashsz;
	ثابت حرف *hash_name = عدم;
	صحيح hash_algo;
	ثابت بنية option show_شاخص_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, show_شاخص_options, show_شاخص_usage, 0);

	إذا (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		إذا (hash_algo == GIT_HASH_UNKNOWN)
			die(_("Unknown hash algorithm"));
		repo_سېټ_hash_algo(the_repository, hash_algo);
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
		repo_سېټ_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	إذا (fread(سر_شاخص, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	إذا (سر_شاخص[0] == htonl(PACK_IDX_SIGNATURE)) {
		نسخه = ntohl(سر_شاخص[1]);
		إذا (نسخه < 2 || نسخه > 2)
			die(_("unknown index version"));
		إذا (fread(سر_شاخص, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} وإلا {
		نسخه = 1;
		إذا (fread(&سر_شاخص[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	لكل (i = 0; i < 256; i++) {
		بینښه n = ntohl(سر_شاخص[i]);
		إذا (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	إذا (نسخه == 1) {
		لكل (i = 0; i < nr; i++) {
			بینښه صحيح دننه_واټن, ثبت[(GIT_MAX_RAWSZ + 4) / حجم(بینښه صحيح)];

			إذا (fread(ثبت, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			دننه_واټن = ntohl(ثبت[0]);
			چاپ_کړه("%u %s\n", دننه_واټن, hash_منزل_hex((فراغ *)(ثبت+1)));
		}
	} وإلا {
		بینښه off64_nr = 0;
		بنية {
			بنية object_id oid;
			uint32_t دوراني_ازموینه;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		لكل (i = 0; i < nr; i++) {
			إذا (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		لكل (i = 0; i < nr; i++)
			إذا (fread(&entries[i].دوراني_ازموینه, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		لكل (i = 0; i < nr; i++)
			إذا (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		لكل (i = 0; i < nr; i++) {
			uint64_t دننه_واټن;
			uint32_t off = ntohl(entries[i].off);
			إذا (!(off & 0x80000000)) {
				دننه_واټن = off;
			} وإلا {
				uint32_t off64[2];
				إذا ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				إذا (fread(off64, 8, 1, stdin) != 1)
					die(_("unable to read 64b offset %u"), off64_nr);
				دننه_واټن = (((uint64_t)ntohl(off64[0])) << 32) |
						     ntohl(off64[1]);
				off64_nr++;
			}
			چاپ_کړه("%" PRIuMAX " %s (%08"PRIx32")\n",
			       (uintmax_t) دننه_واټن,
			       oid_منزل_hex(&entries[i].oid),
			       ntohl(entries[i].دوراني_ازموینه));
		}
		حرر(entries);
	}
	أرجع 0;
}
