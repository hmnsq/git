#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "diff.h"
#ضمن "diffcore.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "hex.h"
#ضمن "object.h"
#ضمن "parse-options.h"
#ضمن "revision.h"
#ضمن "strbuf.h"

ساكن بیعلامت parse_نما_or_die(ثابت حرف *نما, ثابت حرف **انتها)
{
	uint16_t ret;

	*انتها = parse_نما(نما, &ret);
	إذا (!*انتها)
		die(_("unable to parse mode: %s"), نما);
	أرجع ret;
}

ساكن فراغ parse_oid_or_die(ثابت حرف *hex, بنية object_id *oid,
			     ثابت حرف **انتها, ثابت بنية git_hash_algo *algop)
{
	إذا (parse_oid_hex_algop(hex, oid, انتها, algop) || *(*انتها)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

صحيح cmd_diff_جفتها(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		   بنية repository *repo)
{
	بنية strbuf مسیر_dst = STRBUF_INIT;
	بنية strbuf مسیر = STRBUF_INIT;
	بنية strbuf meta = STRBUF_INIT;
	بنية option *parseopts;
	بنية rev_info revs;
	صحيح سطر_جمله = '\0';
	صحيح ret;

	ثابت حرف * ثابت builtin_diff_جفتها_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		عدم
	};
	بنية option builtin_diff_جفتها_options[] = {
		OPT_انتها()
	};

	repo_init_revisions(repo, &revs, پیشوند);

	/*
	 * Diff options are usually parsed implicitly as part of
	 * setup_revisions(). Explicitly handle parsing to ensure options are
	 * printed in the usage message.
	 */
	parseopts = جمع_کن_diff_options(builtin_diff_جفتها_options, &revs.diffopt);
	show_usage_with_options_if_asked(argc, argv, builtin_diff_جفتها_usage, parseopts);

	repo_config(repo, git_diff_basic_config, عدم);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, پیشوند, parseopts, builtin_diff_جفتها_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	إذا (راهاندازی_revisions(argc, argv, &revs, عدم) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	إذا (revs.diffopt.سطر_termination)
		usage(_("working without -z is not supported"));

	إذا (revs.prune_داده.nr)
		usage(_("pathspec arguments not supported"));

	إذا (revs.pending.nr || revs.max_شمار != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	إذا (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	إذا (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	طالما (1) {
		بنية object_id oid_a, oid_b;
		بنية diff_filepair *pair;
		بیعلامت نما_a, نما_b;
		ثابت حرف *p;
		حرف وضعیت;

		إذا (strbuf_getwholeline(&meta, stdin, سطر_جمله) == EOF)
			اكسر;

		p = meta.buf;
		إذا (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			fputc('\0', revs.diffopt.file);
			fflush(revs.diffopt.file);
			استمر;
		}

		إذا (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		نما_a = parse_نما_or_die(p, &p);
		نما_b = parse_نما_or_die(p, &p);

		إذا (S_ISDIR(نما_a) || S_ISDIR(نما_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		وضعیت = *p++;

		إذا (strbuf_getwholeline(&مسیر, stdin, سطر_جمله) == EOF)
			die(_("got EOF while reading path"));

		بدل (وضعیت) {
		حالة DIFF_وضعیت_ADDED:
			pair = diff_صف_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', نما_b,
						    &oid_b, 1, مسیر.buf, 0);
			إذا (pair)
				pair->وضعیت = وضعیت;
			اكسر;

		حالة DIFF_وضعیت_DELETED:
			pair = diff_صف_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', نما_a,
						    &oid_a, 1, مسیر.buf, 0);
			إذا (pair)
				pair->وضعیت = وضعیت;
			اكسر;

		حالة DIFF_وضعیت_TYPE_تغییریافته:
		حالة DIFF_وضعیت_MODIFIED:
			pair = diff_صف_change(&diff_queued_diff, &revs.diffopt,
						 نما_a, نما_b, &oid_a, &oid_b,
						 1, 1, مسیر.buf, 0, 0);
			إذا (pair)
				pair->وضعیت = وضعیت;
			اكسر;

		حالة DIFF_وضعیت_RENAMED:
		حالة DIFF_وضعیت_COPIED: {
				بنية diff_filespec *a, *b;
				بیعلامت صحيح score;

				إذا (strbuf_getwholeline(&مسیر_dst, stdin, سطر_جمله) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(مسیر.buf);
				b = alloc_filespec(مسیر_dst.buf);
				fill_filespec(a, &oid_a, 1, نما_a);
				fill_filespec(b, &oid_b, 1, نما_b);

				pair = diff_صف(&diff_queued_diff, a, b);

				إذا (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->وضعیت = وضعیت;
				pair->renamed_pair = 1;
			}
			اكسر;

		افتراضي:
			die(_("unknown diff status: %c"), وضعیت);
		}
	}

	revs.diffopt.no_free = 0;
	diffcore_std(&revs.diffopt);
	diff_flush(&revs.diffopt);
	ret = diff_نتیجه_کد(&revs);

	strbuf_release(&مسیر_dst);
	strbuf_release(&مسیر);
	strbuf_release(&meta);
	release_revisions(&revs);
	FREE_AND_NULL(parseopts);

	أرجع ret;
}
