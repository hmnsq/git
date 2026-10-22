#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "config.h"
#include "diff.h"
#include "environment.h"
#include "gettext.h"
#include "hash.h"
#include "hex.h"
#include "parse-options.h"
#include "setup.h"

static void flush_current_id(size_t patchlen, struct object_id *id, struct object_id *result)
{
	nếu (patchlen)
		printf("%s %s\n", oid_to_hex(result), oid_to_hex(id));
}

static size_t remove_space(char *line)
{
	char *src = line;
	char *dst = line;
	unsigned char c;

	trong_khi ((c = *src++) != '\0') {
		nếu (!isspace(c))
			*dst++ = c;
	}
	trả_về dst - line;
}

static số_nguyên scan_hunk_header(const char *p, số_nguyên *p_before, số_nguyên *p_after)
{
	static const char digits[] = "0123456789";
	const char *q, *r;
	số_nguyên n;

	q = p + 4;
	n = strspn(q, digits);
	nếu (q[n] == ',') {
		q += n + 1;
		*p_before = atoi(q);
		n = strspn(q, digits);
	} không_thì {
		*p_before = 1;
	}

	nếu (n == 0 || q[n] != ' ' || q[n+1] != '+')
		trả_về 0;

	r = q + n + 2;
	n = strspn(r, digits);
	nếu (r[n] == ',') {
		r += n + 1;
		*p_after = atoi(r);
		n = strspn(r, digits);
	} không_thì {
		*p_after = 1;
	}
	nếu (n == 0)
		trả_về 0;

	trả_về 1;
}

static size_t get_one_patchid(struct object_id *next_oid, struct object_id *result,
			      struct strbuf *line_buf, số_nguyên stable, số_nguyên verbatim)
{
	size_t patchlen = 0;
	số_nguyên found_next = 0;
	số_nguyên before = -1, after = -1;
	số_nguyên diff_is_binary = 0;
	char pre_oid_str[GIT_MAX_HEXSZ + 1], post_oid_str[GIT_MAX_HEXSZ + 1];
	struct git_hash_ctx ctx;

	git_hash_init(&ctx, the_hash_algo);
	oidclr(result, the_repository->hash_algo);

	trong_khi (strbuf_getwholeline(line_buf, stdin, '\n') != EOF) {
		char *line = line_buf->buf;
		const char *p = line;
		size_t len;

		/* Possibly skip over the prefix added by "log" or "format-patch" */
		nếu (!skip_prefix(line, "commit ", &p) &&
		    !skip_prefix(line, "From ", &p) &&
		    starts_with(line, "\\ ") && 12 < strlen(line)) {
			nếu (verbatim)
				git_hash_update(&ctx, line, strlen(line));
			continue;
		}

		nếu (!get_oid_hex(p, next_oid)) {
			found_next = 1;
			break;
		}

		/* Ignore commit comments */
		nếu (!patchlen && !starts_with(line, "diff "))
			continue;

		/* Parsing diff header?  */
		nếu (before == -1) {
			nếu (starts_with(line, "GIT binary patch") ||
			    starts_with(line, "Binary files")) {
				diff_is_binary = 1;
				before = 0;
				git_hash_update(&ctx, pre_oid_str,
						strlen(pre_oid_str));
				git_hash_update(&ctx, post_oid_str,
						strlen(post_oid_str));
				nếu (stable)
					flush_one_hunk(result, &ctx);
				continue;
			} không_thì nếu (skip_prefix(line, "index ", &p)) {
				char *oid1_end = strstr(line, "..");
				char *oid2_end = NULL;
				nếu (oid1_end)
					oid2_end = strstr(oid1_end, " ");
				nếu (!oid2_end)
					oid2_end = line + strlen(line) - 1;
				nếu (oid1_end != NULL && oid2_end != NULL) {
					*oid1_end = *oid2_end = '\0';
					strlcpy(pre_oid_str, p, GIT_MAX_HEXSZ + 1);
					strlcpy(post_oid_str, oid1_end + 2, GIT_MAX_HEXSZ + 1);
				}
				continue;
			} không_thì nếu (starts_with(line, "--- "))
				before = after = 1;
			không_thì nếu (!isalpha(line[0]))
				break;
		}

		nếu (diff_is_binary) {
			nếu (starts_with(line, "diff ")) {
				diff_is_binary = 0;
				before = -1;
			}
			continue;
		}

		/* Looking for a valid hunk header?  */
		nếu (before == 0 && after == 0) {
			nếu (starts_with(line, "@@ -")) {
				/* Parse next hunk, but ignore line numbers.  */
				scan_hunk_header(line, &before, &after);
				continue;
			}

			/* Split at the end of the patch.  */
			nếu (!starts_with(line, "diff "))
				break;

			/* Else we're parsing another header.  */
			nếu (stable)
				flush_one_hunk(result, &ctx);
			before = after = -1;
		}

		/* If we get here, we're inside a hunk.  */
		nếu (line[0] == '-' || line[0] == ' ')
			before--;
		nếu (line[0] == '+' || line[0] == ' ')
			after--;

		/* Add line to hash algo (possibly removing whitespace) */
		len = verbatim ? strlen(line) : remove_space(line);
		patchlen += len;
		git_hash_update(&ctx, line, len);
	}

	nếu (!found_next)
		oidclr(next_oid, the_repository->hash_algo);

	flush_one_hunk(result, &ctx);
	git_hash_discard(&ctx);

	trả_về patchlen;
}

static void generate_id_list(số_nguyên stable, số_nguyên verbatim)
{
	struct object_id oid, n, result;
	size_t patchlen;
	struct strbuf line_buf = STRBUF_INIT;

	oidclr(&oid, the_repository->hash_algo);
	trong_khi (!feof(stdin)) {
		patchlen = get_one_patchid(&n, &result, &line_buf, stable, verbatim);
		flush_current_id(patchlen, &oid, &result);
		oidcpy(&oid, &n);
	}
	strbuf_release(&line_buf);
}

static const char *const patch_id_usage[] = {
	N_("git patch-id [--stable | --unstable | --verbatim]"), NULL
};

struct patch_id_opts {
	số_nguyên stable;
	số_nguyên verbatim;
};

static số_nguyên git_patch_id_config(const char *var, const char *value,
			       const struct config_context *ctx, void *cb)
{
	struct patch_id_opts *opts = cb;

	nếu (!strcmp(var, "patchid.stable")) {
		opts->stable = git_config_bool(var, value);
		trả_về 0;
	}
	nếu (!strcmp(var, "patchid.verbatim")) {
		opts->verbatim = git_config_bool(var, value);
		trả_về 0;
	}

	trả_về git_default_config(var, value, ctx, cb);
}

số_nguyên cmd_patch_id(số_nguyên argc,
		 const char **argv,
		 const char *prefix,
		 struct repository *repo UNUSED)
{
	/* if nothing is set, default to unstable */
	struct patch_id_opts config = {0, 0};
	số_nguyên opts = 0;
	struct option builtin_patch_id_options[] = {
		OPT_CMDMODE(0, "unstable", &opts,
		    N_("use the unstable patch ID algorithm"), 1),
		OPT_CMDMODE(0, "stable", &opts,
		    N_("use the stable patch ID algorithm"), 2),
		OPT_CMDMODE(0, "verbatim", &opts,
			N_("don't strip whitespace from the patch"), 3),
		OPT_END()
	};

	repo_config(the_repository, git_patch_id_config, &config);

	/* verbatim implies stable */
	nếu (config.verbatim)
		config.stable = 1;

	argc = parse_options(argc, argv, prefix, builtin_patch_id_options,
			     patch_id_usage, 0);

	/*
	 * We rely on `the_hash_algo` to compute patch IDs. This is dubious as
	 * it means that the hash algorithm now depends on the object hash of
	 * the repository, even though git-patch-id(1) clearly defines that
	 * patch IDs always use SHA1.
	 *
	 * NEEDSWORK: This hack should be removed in favor of converting
	 * the code that computes patch IDs to always use SHA1.
	 */
	nếu (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	generate_id_list(opts ? opts > 1 : config.stable,
			 opts ? opts == 3 : config.verbatim);
	trả_về 0;
}
