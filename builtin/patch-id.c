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
	如果 (patchlen)
		printf("%s %s\n", oid_to_hex(result), oid_to_hex(id));
}

static size_t remove_space(char *line)
{
	char *src = line;
	char *dst = line;
	unsigned char c;

	当 ((c = *src++) != '\0') {
		如果 (!isspace(c))
			*dst++ = c;
	}
	返回 dst - line;
}

static 整数 scan_hunk_header(const char *p, 整数 *p_before, 整数 *p_after)
{
	static const char digits[] = "0123456789";
	const char *q, *r;
	整数 n;

	q = p + 4;
	n = strspn(q, digits);
	如果 (q[n] == ',') {
		q += n + 1;
		*p_before = atoi(q);
		n = strspn(q, digits);
	} 否则 {
		*p_before = 1;
	}

	如果 (n == 0 || q[n] != ' ' || q[n+1] != '+')
		返回 0;

	r = q + n + 2;
	n = strspn(r, digits);
	如果 (r[n] == ',') {
		r += n + 1;
		*p_after = atoi(r);
		n = strspn(r, digits);
	} 否则 {
		*p_after = 1;
	}
	如果 (n == 0)
		返回 0;

	返回 1;
}

static size_t get_one_patchid(struct object_id *next_oid, struct object_id *result,
			      struct strbuf *line_buf, 整数 stable, 整数 verbatim)
{
	size_t patchlen = 0;
	整数 found_next = 0;
	整数 before = -1, after = -1;
	整数 diff_is_binary = 0;
	char pre_oid_str[GIT_MAX_HEXSZ + 1], post_oid_str[GIT_MAX_HEXSZ + 1];
	struct git_hash_ctx ctx;

	git_hash_init(&ctx, the_hash_algo);
	oidclr(result, the_repository->hash_algo);

	当 (strbuf_getwholeline(line_buf, stdin, '\n') != EOF) {
		char *line = line_buf->buf;
		const char *p = line;
		size_t len;

		/* Possibly skip over the prefix added by "log" or "format-patch" */
		如果 (!skip_prefix(line, "commit ", &p) &&
		    !skip_prefix(line, "From ", &p) &&
		    starts_with(line, "\\ ") && 12 < strlen(line)) {
			如果 (verbatim)
				git_hash_update(&ctx, line, strlen(line));
			continue;
		}

		如果 (!get_oid_hex(p, next_oid)) {
			found_next = 1;
			break;
		}

		/* Ignore commit comments */
		如果 (!patchlen && !starts_with(line, "diff "))
			continue;

		/* Parsing diff header?  */
		如果 (before == -1) {
			如果 (starts_with(line, "GIT binary patch") ||
			    starts_with(line, "Binary files")) {
				diff_is_binary = 1;
				before = 0;
				git_hash_update(&ctx, pre_oid_str,
						strlen(pre_oid_str));
				git_hash_update(&ctx, post_oid_str,
						strlen(post_oid_str));
				如果 (stable)
					flush_one_hunk(result, &ctx);
				continue;
			} 否则 如果 (skip_prefix(line, "index ", &p)) {
				char *oid1_end = strstr(line, "..");
				char *oid2_end = NULL;
				如果 (oid1_end)
					oid2_end = strstr(oid1_end, " ");
				如果 (!oid2_end)
					oid2_end = line + strlen(line) - 1;
				如果 (oid1_end != NULL && oid2_end != NULL) {
					*oid1_end = *oid2_end = '\0';
					strlcpy(pre_oid_str, p, GIT_MAX_HEXSZ + 1);
					strlcpy(post_oid_str, oid1_end + 2, GIT_MAX_HEXSZ + 1);
				}
				continue;
			} 否则 如果 (starts_with(line, "--- "))
				before = after = 1;
			否则 如果 (!isalpha(line[0]))
				break;
		}

		如果 (diff_is_binary) {
			如果 (starts_with(line, "diff ")) {
				diff_is_binary = 0;
				before = -1;
			}
			continue;
		}

		/* Looking for a valid hunk header?  */
		如果 (before == 0 && after == 0) {
			如果 (starts_with(line, "@@ -")) {
				/* Parse next hunk, but ignore line numbers.  */
				scan_hunk_header(line, &before, &after);
				continue;
			}

			/* Split at the end of the patch.  */
			如果 (!starts_with(line, "diff "))
				break;

			/* Else we're parsing another header.  */
			如果 (stable)
				flush_one_hunk(result, &ctx);
			before = after = -1;
		}

		/* If we get here, we're inside a hunk.  */
		如果 (line[0] == '-' || line[0] == ' ')
			before--;
		如果 (line[0] == '+' || line[0] == ' ')
			after--;

		/* Add line to hash algo (possibly removing whitespace) */
		len = verbatim ? strlen(line) : remove_space(line);
		patchlen += len;
		git_hash_update(&ctx, line, len);
	}

	如果 (!found_next)
		oidclr(next_oid, the_repository->hash_algo);

	flush_one_hunk(result, &ctx);
	git_hash_discard(&ctx);

	返回 patchlen;
}

static void generate_id_list(整数 stable, 整数 verbatim)
{
	struct object_id oid, n, result;
	size_t patchlen;
	struct strbuf line_buf = STRBUF_INIT;

	oidclr(&oid, the_repository->hash_algo);
	当 (!feof(stdin)) {
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
	整数 stable;
	整数 verbatim;
};

static 整数 git_patch_id_config(const char *var, const char *value,
			       const struct config_context *ctx, void *cb)
{
	struct patch_id_opts *opts = cb;

	如果 (!strcmp(var, "patchid.stable")) {
		opts->stable = git_config_bool(var, value);
		返回 0;
	}
	如果 (!strcmp(var, "patchid.verbatim")) {
		opts->verbatim = git_config_bool(var, value);
		返回 0;
	}

	返回 git_default_config(var, value, ctx, cb);
}

整数 cmd_patch_id(整数 argc,
		 const char **argv,
		 const char *prefix,
		 struct repository *repo UNUSED)
{
	/* if nothing is set, default to unstable */
	struct patch_id_opts config = {0, 0};
	整数 opts = 0;
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
	如果 (config.verbatim)
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
	如果 (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	generate_id_list(opts ? opts > 1 : config.stable,
			 opts ? opts == 3 : config.verbatim);
	返回 0;
}
