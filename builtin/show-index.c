#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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
		   ثابت حرف *پیشوند,
		   بنية repository *repo استفادهنشده)
{
	صحيح i;
	بیعلامت nr;
	بیعلامت صحيح نسخه;
	ساكن بیعلامت صحيح بالاترین_شاخص[256];
	بیعلامت hashsz;
	ثابت حرف *hash_name = عدم;
	صحيح hash_algo;
	ثابت بنية option show_شاخص_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, show_شاخص_options, show_شاخص_usage, 0);

	إذا (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		إذا (hash_algo == GIT_HASH_UNKNOWN)
			die(_("Unknown hash algorithm"));
		repo_مجموعه_hash_algo(the_repository, hash_algo);
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
		repo_مجموعه_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	إذا (fread(بالاترین_شاخص, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	إذا (بالاترین_شاخص[0] == htonl(PACK_IDX_SIGNATURE)) {
		نسخه = ntohl(بالاترین_شاخص[1]);
		إذا (نسخه < 2 || نسخه > 2)
			die(_("unknown index version"));
		إذا (fread(بالاترین_شاخص, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} وإلا {
		نسخه = 1;
		إذا (fread(&بالاترین_شاخص[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	لكل (i = 0; i < 256; i++) {
		بیعلامت n = ntohl(بالاترین_شاخص[i]);
		إذا (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	إذا (نسخه == 1) {
		لكل (i = 0; i < nr; i++) {
			بیعلامت صحيح جابه_جایی, مدخل[(GIT_MAX_RAWSZ + 4) / حجم(بیعلامت صحيح)];

			إذا (fread(مدخل, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			جابه_جایی = ntohl(مدخل[0]);
			چاپ_کن("%u %s\n", جابه_جایی, hash_مقصد_hex((فراغ *)(مدخل+1)));
		}
	} وإلا {
		بیعلامت off64_nr = 0;
		بنية {
			بنية object_id oid;
			uint32_t بررسی_چرخهای;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		لكل (i = 0; i < nr; i++) {
			إذا (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		لكل (i = 0; i < nr; i++)
			إذا (fread(&entries[i].بررسی_چرخهای, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		لكل (i = 0; i < nr; i++)
			إذا (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		لكل (i = 0; i < nr; i++) {
			uint64_t جابه_جایی;
			uint32_t off = ntohl(entries[i].off);
			إذا (!(off & 0x80000000)) {
				جابه_جایی = off;
			} وإلا {
				uint32_t off64[2];
				إذا ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				إذا (fread(off64, 8, 1, stdin) != 1)
					die(_("unable to read 64b offset %u"), off64_nr);
				جابه_جایی = (((uint64_t)ntohl(off64[0])) << 32) |
						     ntohl(off64[1]);
				off64_nr++;
			}
			چاپ_کن("%" PRIuMAX " %s (%08"PRIx32")\n",
			       (uintmax_t) جابه_جایی,
			       oid_مقصد_hex(&entries[i].oid),
			       ntohl(entries[i].بررسی_چرخهای));
		}
		حرر(entries);
	}
	أرجع 0;
}
