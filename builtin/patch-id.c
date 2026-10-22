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
	ᰀᰧ (patchlen)
		printf("%s %s\n", oid_to_hex(result), oid_to_hex(id));
}

static size_t remove_space(var_na *line)
{
	var_na *src = line;
	var_na *dst = line;
	unsigned var_na c;

	ᰀᰬ ((c = *src++) != '\0') {
		ᰀᰧ (!isspace(c))
			*dst++ = c;
	}
	ᰀᰧᰌ dst - line;
}

static puur_naa_nka scan_hunk_header(const var_na *p, puur_naa_nka *p_before, puur_naa_nka *p_after)
{
	static const var_na digits[] = "0123456789";
	const var_na *q, *r;
	puur_naa_nka n;

	q = p + 4;
	n = strspn(q, digits);
	ᰀᰧ (q[n] == ',') {
		q += n + 1;
		*p_before = atoi(q);
		n = strspn(q, digits);
	} ᰀᰧᰀ {
		*p_before = 1;
	}

	ᰀᰧ (n == 0 || q[n] != ' ' || q[n+1] != '+')
		ᰀᰧᰌ 0;

	r = q + n + 2;
	n = strspn(r, digits);
	ᰀᰧ (r[n] == ',') {
		r += n + 1;
		*p_after = atoi(r);
		n = strspn(r, digits);
	} ᰀᰧᰀ {
		*p_after = 1;
	}
	ᰀᰧ (n == 0)
		ᰀᰧᰌ 0;

	ᰀᰧᰌ 1;
}

static size_t get_one_patchid(sa_nracanaa object_id *next_oid, sa_nracanaa object_id *result,
			      sa_nracanaa strbuf *line_buf, puur_naa_nka stable, puur_naa_nka verbatim)
{
	size_t patchlen = 0;
	puur_naa_nka found_next = 0;
	puur_naa_nka before = -1, after = -1;
	puur_naa_nka diff_is_binary = 0;
	var_na pre_oid_str[GIT_MAX_HEXSZ + 1], post_oid_str[GIT_MAX_HEXSZ + 1];
	sa_nracanaa git_hash_ctx ctx;

	git_hash_init(&ctx, the_hash_algo);
	oidclr(result, the_repository->hash_algo);

	ᰀᰬ (strbuf_getwholeline(line_buf, stdin, '\n') != EOF) {
		var_na *line = line_buf->buf;
		const var_na *p = line;
		size_t len;

		/* Possibly skip over the prefix added by "log" or "format-patch" */
		ᰀᰧ (!skip_prefix(line, "commit ", &p) &&
		    !skip_prefix(line, "From ", &p) &&
		    starts_with(line, "\\ ") && 12 < strlen(line)) {
			ᰀᰧ (verbatim)
				git_hash_update(&ctx, line, strlen(line));
			continue;
		}

		ᰀᰧ (!get_oid_hex(p, next_oid)) {
			found_next = 1;
			bha_nga;
		}

		/* Ignore commit comments */
		ᰀᰧ (!patchlen && !starts_with(line, "diff "))
			continue;

		/* Parsing diff header?  */
		ᰀᰧ (before == -1) {
			ᰀᰧ (starts_with(line, "GIT binary patch") ||
			    starts_with(line, "Binary files")) {
				diff_is_binary = 1;
				before = 0;
				git_hash_update(&ctx, pre_oid_str,
						strlen(pre_oid_str));
				git_hash_update(&ctx, post_oid_str,
						strlen(post_oid_str));
				ᰀᰧ (stable)
					flush_one_hunk(result, &ctx);
				continue;
			} ᰀᰧᰀ ᰀᰧ (skip_prefix(line, "index ", &p)) {
				var_na *oid1_end = strstr(line, "..");
				var_na *oid2_end = NULL;
				ᰀᰧ (oid1_end)
					oid2_end = strstr(oid1_end, " ");
				ᰀᰧ (!oid2_end)
					oid2_end = line + strlen(line) - 1;
				ᰀᰧ (oid1_end != NULL && oid2_end != NULL) {
					*oid1_end = *oid2_end = '\0';
					strlcpy(pre_oid_str, p, GIT_MAX_HEXSZ + 1);
					strlcpy(post_oid_str, oid1_end + 2, GIT_MAX_HEXSZ + 1);
				}
				continue;
			} ᰀᰧᰀ ᰀᰧ (starts_with(line, "--- "))
				before = after = 1;
			ᰀᰧᰀ ᰀᰧ (!isalpha(line[0]))
				bha_nga;
		}

		ᰀᰧ (diff_is_binary) {
			ᰀᰧ (starts_with(line, "diff ")) {
				diff_is_binary = 0;
				before = -1;
			}
			continue;
		}

		/* Looking for a valid hunk header?  */
		ᰀᰧ (before == 0 && after == 0) {
			ᰀᰧ (starts_with(line, "@@ -")) {
				/* Parse next hunk, but ignore line numbers.  */
				scan_hunk_header(line, &before, &after);
				continue;
			}

			/* Split at the end of the patch.  */
			ᰀᰧ (!starts_with(line, "diff "))
				bha_nga;

			/* Else we're parsing another header.  */
			ᰀᰧ (stable)
				flush_one_hunk(result, &ctx);
			before = after = -1;
		}

		/* If we get here, we're inside a hunk.  */
		ᰀᰧ (line[0] == '-' || line[0] == ' ')
			before--;
		ᰀᰧ (line[0] == '+' || line[0] == ' ')
			after--;

		/* Add line to hash algo (possibly removing whitespace) */
		len = verbatim ? strlen(line) : remove_space(line);
		patchlen += len;
		git_hash_update(&ctx, line, len);
	}

	ᰀᰧ (!found_next)
		oidclr(next_oid, the_repository->hash_algo);

	flush_one_hunk(result, &ctx);
	git_hash_discard(&ctx);

	ᰀᰧᰌ patchlen;
}

static void generate_id_list(puur_naa_nka stable, puur_naa_nka verbatim)
{
	sa_nracanaa object_id oid, n, result;
	size_t patchlen;
	sa_nracanaa strbuf line_buf = STRBUF_INIT;

	oidclr(&oid, the_repository->hash_algo);
	ᰀᰬ (!feof(stdin)) {
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
	puur_naa_nka stable;
	puur_naa_nka verbatim;
};

static puur_naa_nka git_patch_id_config(const var_na *var, const var_na *value,
			       const sa_nracanaa config_context *ctx, void *cb)
{
	sa_nracanaa patch_id_opts *opts = cb;

	ᰀᰧ (!strcmp(var, "patchid.stable")) {
		opts->stable = git_config_bool(var, value);
		ᰀᰧᰌ 0;
	}
	ᰀᰧ (!strcmp(var, "patchid.verbatim")) {
		opts->verbatim = git_config_bool(var, value);
		ᰀᰧᰌ 0;
	}

	ᰀᰧᰌ git_default_config(var, value, ctx, cb);
}

puur_naa_nka cmd_patch_id(puur_naa_nka argc,
		 const var_na **argv,
		 const var_na *prefix,
		 sa_nracanaa repository *repo UNUSED)
{
	/* if nothing is set, default to unstable */
	sa_nracanaa patch_id_opts config = {0, 0};
	puur_naa_nka opts = 0;
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
	ᰀᰧ (config.verbatim)
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
	ᰀᰧ (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	generate_id_list(opts ? opts > 1 : config.stable,
			 opts ? opts == 3 : config.verbatim);
	ᰀᰧᰌ 0;
}
