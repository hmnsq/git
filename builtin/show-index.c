#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "gettext.h"
#समावेश "hash.h"
#समावेश "hex.h"
#समावेश "pack.h"
#समावेश "parse-options.h"

अटल स्थिर अक्षर *स्थिर show_सूचकांक_usage[] = {
	"git show-index [--object-format=<hash-algorithm>] < <pack-idx-file>",
	शून्य
};

पूर्णांक cmd_show_सूचकांक(पूर्णांक argc,
		   स्थिर अक्षर **argv,
		   स्थिर अक्षर *prefix,
		   काष्ठा repository *repo UNUSED)
{
	पूर्णांक i;
	अचिन्हित nr;
	अचिन्हित पूर्णांक version;
	अटल अचिन्हित पूर्णांक शीर्ष_सूचकांक[256];
	अचिन्हित hashsz;
	स्थिर अक्षर *hash_name = शून्य;
	पूर्णांक hash_algo;
	स्थिर काष्ठा option show_सूचकांक_options[] = {
		OPT_STRING(0, "object-format", &hash_name, N_("hash-algorithm"),
			   N_("specify the hash algorithm to use")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, show_सूचकांक_options, show_सूचकांक_usage, 0);

	अगर (hash_name) {
		hash_algo = hash_algo_by_name(hash_name);
		अगर (hash_algo == GIT_HASH_UNKNOWN)
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
	अगर (!the_hash_algo) {
		warning(_("assuming SHA-1; use --object-format to override"));
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);
	}

	hashsz = the_hash_algo->rawsz;

	अगर (ख_पढ़ो(शीर्ष_सूचकांक, 2 * 4, 1, मानक_निवेश) != 1)
		die(_("unable to read header"));
	अगर (शीर्ष_सूचकांक[0] == htonl(PACK_IDX_SIGNATURE)) {
		version = ntohl(शीर्ष_सूचकांक[1]);
		अगर (version < 2 || version > 2)
			die(_("unknown index version"));
		अगर (ख_पढ़ो(शीर्ष_सूचकांक, 256 * 4, 1, मानक_निवेश) != 1)
			die(_("unable to read index"));
	} अन्यथा {
		version = 1;
		अगर (ख_पढ़ो(&शीर्ष_सूचकांक[2], 254 * 4, 1, मानक_निवेश) != 1)
			die(_("unable to read index"));
	}
	nr = 0;
	क्रम (i = 0; i < 256; i++) {
		अचिन्हित n = ntohl(शीर्ष_सूचकांक[i]);
		अगर (n < nr)
			die(_("corrupt index file"));
		nr = n;
	}
	अगर (version == 1) {
		क्रम (i = 0; i < nr; i++) {
			अचिन्हित पूर्णांक offset, entry[(GIT_MAX_RAWSZ + 4) / माप(अचिन्हित पूर्णांक)];

			अगर (ख_पढ़ो(entry, 4 + hashsz, 1, मानक_निवेश) != 1)
				die(_("unable to read entry %u/%u"), i, nr);
			offset = ntohl(entry[0]);
			म_लिखो("%u %s\n", offset, hash_to_hex((व्योम *)(entry+1)));
		}
	} अन्यथा {
		अचिन्हित off64_nr = 0;
		काष्ठा {
			काष्ठा object_id oid;
			uint32_t crc;
			uint32_t off;
		} *entries;
		ALLOC_ARRAY(entries, nr);
		क्रम (i = 0; i < nr; i++) {
			अगर (ख_पढ़ो(entries[i].oid.hash, hashsz, 1, मानक_निवेश) != 1)
				die(_("unable to read sha1 %u/%u"), i, nr);
			entries[i].oid.algo = hash_algo_by_ptr(the_hash_algo);
		}
		क्रम (i = 0; i < nr; i++)
			अगर (ख_पढ़ो(&entries[i].crc, 4, 1, मानक_निवेश) != 1)
				die(_("unable to read crc %u/%u"), i, nr);
		क्रम (i = 0; i < nr; i++)
			अगर (ख_पढ़ो(&entries[i].off, 4, 1, मानक_निवेश) != 1)
				die(_("unable to read 32b offset %u/%u"), i, nr);
		क्रम (i = 0; i < nr; i++) {
			uint64_t offset;
			uint32_t off = ntohl(entries[i].off);
			अगर (!(off & 0x80000000)) {
				offset = off;
			} अन्यथा {
				uint32_t off64[2];
				अगर ((off & 0x7fffffff) != off64_nr)
					die(_("inconsistent 64b offset index"));
				अगर (ख_पढ़ो(off64, 8, 1, मानक_निवेश) != 1)
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
	वापस 0;
}
