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
	ᯀᯮᯝ (patchlen)
		printf("%s %s\n", oid_to_hex(result), oid_to_hex(id));
}

static size_t remove_space(var_na *line)
{
	var_na *src = line;
	var_na *dst = line;
	unsigned var_na c;

	ᯀᯬᯝ ((c = *src++) != '\0') {
		ᯀᯮᯝ (!isspace(c))
			*dst++ = c;
	}
	ᯀᯮᯞ dst - line;
}

static puur_naa_nka scan_hunk_header(const var_na *p, puur_naa_nka *p_before, puur_naa_nka *p_after)
{
	static const var_na digits[] = "0123456789";
	const var_na *q, *r;
	puur_naa_nka n;

	q = p + 4;
	n = strspn(q, digits);
	ᯀᯮᯝ (q[n] == ',') {
		q += n + 1;
		*p_before = atoi(q);
		n = strspn(q, digits);
	} ᯀᯮᯝᯂᯪ {
		*p_before = 1;
	}

	ᯀᯮᯝ (n == 0 || q[n] != ' ' || q[n+1] != '+')
		ᯀᯮᯞ 0;

	r = q + n + 2;
	n = strspn(r, digits);
	ᯀᯮᯝ (r[n] == ',') {
		r += n + 1;
		*p_after = atoi(r);
		n = strspn(r, digits);
	} ᯀᯮᯝᯂᯪ {
		*p_after = 1;
	}
	ᯀᯮᯝ (n == 0)
		ᯀᯮᯞ 0;

	ᯀᯮᯞ 1;
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

	ᯀᯬᯝ (strbuf_getwholeline(line_buf, stdin, '\n') != EOF) {
		var_na *line = line_buf->buf;
		const var_na *p = line;
		size_t len;

		/* Possibly skip over the prefix added by "log" or "format-patch" */
		ᯀᯮᯝ (!skip_prefix(line, "commit ", &p) &&
		    !skip_prefix(line, "From ", &p) &&
		    starts_with(line, "\\ ") && 12 < strlen(line)) {
			ᯀᯮᯝ (verbatim)
				git_hash_update(&ctx, line, strlen(line));
			continue;
		}

		ᯀᯮᯝ (!get_oid_hex(p, next_oid)) {
			found_next = 1;
			bha_nga;
		}

		/* Ignore commit comments */
		ᯀᯮᯝ (!patchlen && !starts_with(line, "diff "))
			continue;

		/* Parsing diff header?  */
		ᯀᯮᯝ (before == -1) {
			ᯀᯮᯝ (starts_with(line, "GIT binary patch") ||
			    starts_with(line, "Binary files")) {
				diff_is_binary = 1;
				before = 0;
				git_hash_update(&ctx, pre_oid_str,
						strlen(pre_oid_str));
				git_hash_update(&ctx, post_oid_str,
						strlen(post_oid_str));
				ᯀᯮᯝ (stable)
					flush_one_hunk(result, &ctx);
				continue;
			} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (skip_prefix(line, "index ", &p)) {
				var_na *oid1_end = strstr(line, "..");
				var_na *oid2_end = NULL;
				ᯀᯮᯝ (oid1_end)
					oid2_end = strstr(oid1_end, " ");
				ᯀᯮᯝ (!oid2_end)
					oid2_end = line + strlen(line) - 1;
				ᯀᯮᯝ (oid1_end != NULL && oid2_end != NULL) {
					*oid1_end = *oid2_end = '\0';
					strlcpy(pre_oid_str, p, GIT_MAX_HEXSZ + 1);
					strlcpy(post_oid_str, oid1_end + 2, GIT_MAX_HEXSZ + 1);
				}
				continue;
			} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (starts_with(line, "--- "))
				before = after = 1;
			ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!isalpha(line[0]))
				bha_nga;
		}

		ᯀᯮᯝ (diff_is_binary) {
			ᯀᯮᯝ (starts_with(line, "diff ")) {
				diff_is_binary = 0;
				before = -1;
			}
			continue;
		}

		/* Looking for a valid hunk header?  */
		ᯀᯮᯝ (before == 0 && after == 0) {
			ᯀᯮᯝ (starts_with(line, "@@ -")) {
				/* Parse next hunk, but ignore line numbers.  */
				scan_hunk_header(line, &before, &after);
				continue;
			}

			/* Split at the end of the patch.  */
			ᯀᯮᯝ (!starts_with(line, "diff "))
				bha_nga;

			/* Else we're parsing another header.  */
			ᯀᯮᯝ (stable)
				flush_one_hunk(result, &ctx);
			before = after = -1;
		}

		/* If we get here, we're inside a hunk.  */
		ᯀᯮᯝ (line[0] == '-' || line[0] == ' ')
			before--;
		ᯀᯮᯝ (line[0] == '+' || line[0] == ' ')
			after--;

		/* Add line to hash algo (possibly removing whitespace) */
		len = verbatim ? strlen(line) : remove_space(line);
		patchlen += len;
		git_hash_update(&ctx, line, len);
	}

	ᯀᯮᯝ (!found_next)
		oidclr(next_oid, the_repository->hash_algo);

	flush_one_hunk(result, &ctx);
	git_hash_discard(&ctx);

	ᯀᯮᯞ patchlen;
}

static void generate_id_list(puur_naa_nka stable, puur_naa_nka verbatim)
{
	sa_nracanaa object_id oid, n, result;
	size_t patchlen;
	sa_nracanaa strbuf line_buf = STRBUF_INIT;

	oidclr(&oid, the_repository->hash_algo);
	ᯀᯬᯝ (!feof(stdin)) {
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

	ᯀᯮᯝ (!strcmp(var, "patchid.stable")) {
		opts->stable = git_config_bool(var, value);
		ᯀᯮᯞ 0;
	}
	ᯀᯮᯝ (!strcmp(var, "patchid.verbatim")) {
		opts->verbatim = git_config_bool(var, value);
		ᯀᯮᯞ 0;
	}

	ᯀᯮᯞ git_default_config(var, value, ctx, cb);
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
	ᯀᯮᯝ (config.verbatim)
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
	ᯀᯮᯝ (!the_hash_algo)
		repo_set_hash_algo(the_repository, GIT_HASH_DEFAULT);

	generate_id_list(opts ? opts > 1 : config.stable,
			 opts ? opts == 3 : config.verbatim);
	ᯀᯮᯞ 0;
}
