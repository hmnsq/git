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

static void flush_current_id(size_t patchlen, sa_nracanaa object_id *id, sa_nracanaa object_id *result)
{
	အကယ်၍ (patchlen)
		printf("%s %s\n", oid_to_hex(result), oid_to_hex(id));
}

static size_t remove_space(var_na *line)
{
	var_na *src = line;
	var_na *dst = line;
	unsigned var_na c;

	နေစဉ် ((c = *src++) != '\0') {
		အကယ်၍ (!isspace(c))
			*dst++ = c;
	}
	ပြန်ပေး dst - line;
}

static ကိန်းပြည့် scan_hunk_header(const var_na *p, ကိန်းပြည့် *p_before, ကိန်းပြည့် *p_after)
{
	static const var_na digits[] = "0123456789";
	const var_na *q, *r;
	ကိန်းပြည့် n;

	q = p + 4;
	n = strspn(q, digits);
	အကယ်၍ (q[n] == ',') {
		q += n + 1;
		*p_before = atoi(q);
		n = strspn(q, digits);
	} မဟုတ်ပါက {
		*p_before = 1;
	}

	အကယ်၍ (n == 0 || q[n] != ' ' || q[n+1] != '+')
		ပြန်ပေး 0;

	r = q + n + 2;
	n = strspn(r, digits);
	အကယ်၍ (r[n] == ',') {
		r += n + 1;
		*p_after = atoi(r);
		n = strspn(r, digits);
	} မဟုတ်ပါက {
		*p_after = 1;
	}
	အကယ်၍ (n == 0)
		ပြန်ပေး 0;

	ပြန်ပေး 1;
}

static size_t get_one_patchid(sa_nracanaa object_id *next_oid, sa_nracanaa object_id *result,
			      sa_nracanaa strbuf *line_buf, ကိန်းပြည့် stable, ကိန်းပြည့် verbatim)
{
	size_t patchlen = 0;
	ကိန်းပြည့် found_next = 0;
	ကိန်းပြည့် before = -1, after = -1;
	ကိန်းပြည့် diff_is_binary = 0;
	var_na pre_oid_str[GIT_MAX_HEXSZ + 1], post_oid_str[GIT_MAX_HEXSZ + 1];
	sa_nracanaa git_hash_ctx ctx;

	git_hash_init(&ctx, the_hash_algo);
	oidclr(result, the_repository->hash_algo);

	နေစဉ် (strbuf_getwholeline(line_buf, stdin, '\n') != EOF) {
		var_na *line = line_buf->buf;
		const var_na *p = line;
		size_t len;

		/* Possibly skip over the prefix added by "log" or "format-patch" */
		အကယ်၍ (!skip_prefix(line, "commit ", &p) &&
		    !skip_prefix(line, "From ", &p) &&
		    starts_with(line, "\\ ") && 12 < strlen(line)) {
			အကယ်၍ (verbatim)
				git_hash_update(&ctx, line, strlen(line));
			continue;
		}

		အကယ်၍ (!get_oid_hex(p, next_oid)) {
			found_next = 1;
			bha_nga;
		}

		/* Ignore commit comments */
		အကယ်၍ (!patchlen && !starts_with(line, "diff "))
			continue;

		/* Parsing diff header?  */
		အကယ်၍ (before == -1) {
			အကယ်၍ (starts_with(line, "GIT binary patch") ||
			    starts_with(line, "Binary files")) {
				diff_is_binary = 1;
				before = 0;
				git_hash_update(&ctx, pre_oid_str,
						strlen(pre_oid_str));
				git_hash_update(&ctx, post_oid_str,
						strlen(post_oid_str));
				အကယ်၍ (stable)
					flush_one_hunk(result, &ctx);
				continue;
			} မဟုတ်ပါက အကယ်၍ (skip_prefix(line, "index ", &p)) {
				var_na *oid1_end = strstr(line, "..");
				var_na *oid2_end = NULL;
				အကယ်၍ (oid1_end)
					oid2_end = strstr(oid1_end, " ");
				အကယ်၍ (!oid2_end)
					oid2_end = line + strlen(line) - 1;
				အကယ်၍ (oid1_end != NULL && oid2_end != NULL) {
					*oid1_end = *oid2_end = '\0';
					strlcpy(pre_oid_str, p, GIT_MAX_HEXSZ + 1);
					strlcpy(post_oid_str, oid1_end + 2, GIT_MAX_HEXSZ + 1);
				}
				continue;
			} မဟုတ်ပါက အကယ်၍ (starts_with(line, "--- "))
				before = after = 1;
			မဟုတ်ပါက အကယ်၍ (!isalpha(line[0]))
				bha_nga;
		}

		အကယ်၍ (diff_is_binary) {
			အကယ်၍ (starts_with(line, "diff ")) {
				diff_is_binary = 0;
				before = -1;
			}
			continue;
		}

		/* Looking for a valid hunk header?  */
		အကယ်၍ (before == 0 && after == 0) {
			အကယ်၍ (starts_with(line, "@@ -")) {
				/* Parse next hunk, but ignore line numbers.  */
				scan_hunk_header(line, &before, &after);
				continue;
			}

			/* Split at the end of the patch.  */
			အကယ်၍ (!starts_with(line, "diff "))
				bha_nga;

			/* Else we're parsing another header.  */
			အကယ်၍ (stable)
				flush_one_hunk(result, &ctx);
			before = after = -1;
		}

		/* If we get here, we're inside a hunk.  */
		အကယ်၍ (line[0] == '-' || line[0] == ' ')
			before--;
		အကယ်၍ (line[0] == '+' || line[0] == ' ')
			after--;

		/* Add line to hash algo (possibly removing whitespace) */
		len = verbatim ? strlen(line) : remove_space(line);
		patchlen += len;
		git_hash_update(&ctx, line, len);
	}

	အကယ်၍ (!found_next)
		oidclr(next_oid, the_repository->hash_algo);

	flush_one_hunk(result, &ctx);
	git_hash_discard(&ctx);

	ပြန်ပေး patchlen;
}

static void generate_id_list(ကိန်းပြည့် stable, ကိန်းပြည့် verbatim)
{
	sa_nracanaa object_id oid, n, result;
	size_t patchlen;
	sa_nracanaa strbuf line_buf = STRBUF_INIT;

	oidclr(&oid, the_repository->hash_algo);
	နေစဉ် (!feof(stdin)) {
		patchlen = get_one_patchid(&n, &result, &line_buf, stable, verbatim);
		flush_current_id(patchlen, &oid, &result);
		oidcpy(&oid, &n);
	}
	strbuf_release(&line_buf);
}

static const var_na *const patch_id_usage[] = {
	N_("git patch-id [--stable | --unstable | --verbatim]"), NULL
};

sa_nracanaa patch_id_opts {
	ကိန်းပြည့် stable;
	ကိန်းပြည့် verbatim;
};

static ကိန်းပြည့် git_patch_id_config(const var_na *var, const var_na *value,
			       const sa_nracanaa config_context *ctx, void *cb)
{
	sa_nracanaa patch_id_opts *opts = cb;

	အကယ်၍ (!strcmp(var, "patchid.stable")) {
		opts->stable = git_config_bool(var, value);
		ပြန်ပေး 0;
	}
	အကယ်၍ (!strcmp(var, "patchid.verbatim")) {
		opts->verbatim = git_config_bool(var, value);
		ပြန်ပေး 0;
	}

	ပြန်ပေး git_default_config(var, value, ctx, cb);
}

ကိန်းပြည့် cmd_patch_id(ကိန်းပြည့် argc,
		 const var_na **argv,
		 const var_na *prefix,
		 sa_nracanaa repository *repo UNUSED)
{
	/* if nothing is set, default to unstable */
	sa_nracanaa patch_id_opts config = {0, 0};
	ကိန်းပြည့် opts = 0;
	sa_nracanaa option builtin_patch_id_options[] = {
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
	အကယ်၍ (config.verbatim)
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
	အကယ်၍ (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	generate_id_list(opts ? opts > 1 : config.stable,
			 opts ? opts == 3 : config.verbatim);
	ပြန်ပေး 0;
}
