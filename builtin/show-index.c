#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "pack.h"
#అన్తర్భావ "parse-options.h"

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి show_సూచీ_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	ఖాళీ
};

పూర్ణాంకం cmd_show_సూచీ(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i;
	గుర్తులేని nr;
	గుర్తులేని పూర్ణాంకం సంస్కరణ;
	స్థిర గుర్తులేని పూర్ణాంకం శిఖరం_సూచీ[256];
	గుర్తులేని hashsz;
	స్థిరరాశి అక్షరం *hash_name = ఖాళీ;
	పూర్ణాంకం hash_algo;
	స్థిరరాశి నిర్మాణం option show_సూచీ_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, show_సూచీ_options, show_సూచీ_usage, 0);

	ఒకవేళ (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		ఒకవేళ (hash_algo == GIT_HASH_UNKNOWN)
			die(_("Unknown hash algorithm"));
		repo_సమితి_hash_algo(the_repository, hash_algo);
	}

	/*
	 * Fallback to SHA1 if we are running outside of a repository.
	 *
	 * TODO: If a future implementation of index file version encodes the hash
	 *       algorithm in its header, enable show-index to infer it from the
	 *       header rather than relying on repository context or a default fallback.
	 */
	ఒకవేళ (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_సమితి_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	ఒకవేళ (fread(శిఖరం_సూచీ, 2 * 4, 1, stdin) != 1)
		die(_("unable to read header"));
	ఒకవేళ (శిఖరం_సూచీ[0] == htonl(PACK_IDX_SIGNATURE)) {
		సంస్కరణ = ntohl(శిఖరం_సూచీ[1]);
		ఒకవేళ (సంస్కరణ < 2 || సంస్కరణ > 2)
			die(_("unknown index version"));
		ఒకవేళ (fread(శిఖరం_సూచీ, 256 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	} లేకపోతే {
		సంస్కరణ = 1;
		ఒకవేళ (fread(&శిఖరం_సూచీ[2], 254 * 4, 1, stdin) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	కొరకు (i = 0; i < 256; i++) {
		గుర్తులేని n = ntohl(శిఖరం_సూచీ[i]);
		ఒకవేళ (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	ఒకవేళ (సంస్కరణ == 1) {
		కొరకు (i = 0; i < nr; i++) {
			గుర్తులేని పూర్ణాంకం స్థానభ్రంశం, నమోదు_2[(GIT_MAX_RAWSZ + 4) / పరిమాణం(గుర్తులేని పూర్ణాంకం)];

			ఒకవేళ (fread(నమోదు_2, 4 + hashsz, 1, stdin) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			స్థానభ్రంశం = ntohl(నమోదు_2[0]);
			రాయి("%u %s\n", స్థానభ్రంశం, hash_గమ్యం_hex((శూన్య *)(నమోదు_2+1)));
		}
	} లేకపోతే {
		గుర్తులేని off64_nr = 0;
		నిర్మాణం {
			నిర్మాణం object_id oid;
			uint32_t చక్రీయ_తనిఖీ;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		కొరకు (i = 0; i < nr; i++) {
			ఒకవేళ (fread(entries[i].oid.hash, hashsz, 1, stdin) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		కొరకు (i = 0; i < nr; i++)
			ఒకవేళ (fread(&entries[i].చక్రీయ_తనిఖీ, 4, 1, stdin) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		కొరకు (i = 0; i < nr; i++)
			ఒకవేళ (fread(&entries[i].off, 4, 1, stdin) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		కొరకు (i = 0; i < nr; i++) {
			uint64_t స్థానభ్రంశం;
			uint32_t off = ntohl(entries[i].off);
			ఒకవేళ (!(off & 0x80000000)) {
				స్థానభ్రంశం = off;
			} లేకపోతే {
				uint32_t off64[2];
				ఒకవేళ ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				ఒకవేళ (fread(off64, 8, 1, stdin) != 1)
					die(_("unable to read 64b offset %u"), off64_nr);
				స్థానభ్రంశం = (((uint64_t)ntohl(off64[0])) << 32) |
						     ntohl(off64[1]);
				off64_nr++;
			}
			రాయి("%" PRIuMAX " %s (%08"PRIx32")\n",
			       (uintmax_t) స్థానభ్రంశం,
			       oid_గమ్యం_hex(&entries[i].oid),
			       ntohl(entries[i].చక్రీయ_తనిఖీ));
		}
		విడుదల(entries);
	}
	తిరిగి 0;
}
