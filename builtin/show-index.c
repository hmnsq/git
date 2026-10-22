#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "pack.h"
#अन्तर्भाव "parse-options.h"

स्थिर स्थिरांक अक्षर *स्थिरांक show_index_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	रिक्त
};

पूर्णांक cmd_show_index(पूर्णांक argc,
		   स्थिरांक अक्षर **argv,
		   स्थिरांक अक्षर *prefix,
		   संरचना repository *repo UNUSED)
{
	पूर्णांक i;
	अचिन्हित nr;
	अचिन्हित पूर्णांक version;
	स्थिर अचिन्हित पूर्णांक शीर्ष_index[256];
	अचिन्हित hashsz;
	स्थिरांक अक्षर *hash_name = रिक्त;
	पूर्णांक hash_algo;
	स्थिरांक संरचना option show_index_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, show_index_options, show_index_usage, 0);

	जर (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		जर (hash_algo == GIT_HASH_UNKNOWN)
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
	जर (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	जर (ख_पढ़ो(शीर्ष_index, 2 * 4, 1, मानक_निवेश) != 1)
		die(_("unable to read header"));
	जर (शीर्ष_index[0] == htonl(PACK_IDX_SIGNATURE)) {
		version = ntohl(शीर्ष_index[1]);
		जर (version < 2 || version > 2)
			die(_("unknown index version"));
		जर (ख_पढ़ो(शीर्ष_index, 256 * 4, 1, मानक_निवेश) != 1)
			die(_("unable to read index"));
	} अन्यथा {
		version = 1;
		जर (ख_पढ़ो(&शीर्ष_index[2], 254 * 4, 1, मानक_निवेश) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	साठी (i = 0; i < 256; i++) {
		अचिन्हित n = ntohl(शीर्ष_index[i]);
		जर (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	जर (version == 1) {
		साठी (i = 0; i < nr; i++) {
			अचिन्हित पूर्णांक offset, entry[(GIT_MAX_RAWSZ + 4) / आकार(अचिन्हित पूर्णांक)];

			जर (ख_पढ़ो(entry, 4 + hashsz, 1, मानक_निवेश) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			offset = ntohl(entry[0]);
			म_लिखो("%u %s\n", offset, hash_to_hex((शून्य *)(entry+1)));
		}
	} अन्यथा {
		अचिन्हित off64_nr = 0;
		संरचना {
			संरचना object_id oid;
			uint32_t crc;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		साठी (i = 0; i < nr; i++) {
			जर (ख_पढ़ो(entries[i].oid.hash, hashsz, 1, मानक_निवेश) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		साठी (i = 0; i < nr; i++)
			जर (ख_पढ़ो(&entries[i].crc, 4, 1, मानक_निवेश) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		साठी (i = 0; i < nr; i++)
			जर (ख_पढ़ो(&entries[i].off, 4, 1, मानक_निवेश) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		साठी (i = 0; i < nr; i++) {
			uint64_t offset;
			uint32_t off = ntohl(entries[i].off);
			जर (!(off & 0x80000000)) {
				offset = off;
			} अन्यथा {
				uint32_t off64[2];
				जर ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				जर (ख_पढ़ो(off64, 8, 1, मानक_निवेश) != 1)
					die(_("unable to read 64b offset %u"), off64_nr);
				offset = (((uint64_t)ntohl(off64[0])) << 32) |
						     ntohl(off64[1]);
				off64_nr++;
			}
			म_लिखो("%" PRIuMAX " %s (%08"PRIx32")\n",
			       (uintmax_t) offset,
			       oid_to_hex(&entries[i].oid),
			       ntohl(entries[i].crc));
		}
		मुक्त(entries);
	}
	परत 0;
}
