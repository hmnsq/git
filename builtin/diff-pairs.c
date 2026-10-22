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

ساكن بنانشان parse_گهڻائي_or_die(ثابت حرف *گهڻائي, ثابت حرف **پڇاڙي)
{
	uint16_t ret;

	*پڇاڙي = parse_گهڻائي(گهڻائي, &ret);
	إذا (!*پڇاڙي)
		die(_("unable to parse mode: %s"), گهڻائي);
	أرجع ret;
}

ساكن فراغ parse_oid_or_die(ثابت حرف *hex, بنية object_id *oid,
			     ثابت حرف **پڇاڙي, ثابت بنية git_hash_algo *algop)
{
	إذا (parse_oid_hex_algop(hex, oid, پڇاڙي, algop) || *(*پڇاڙي)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

صحيح cmd_diff_جوڙا(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		   بنية repository *repo)
{
	بنية strbuf رستو_dst = STRBUF_INIT;
	بنية strbuf رستو = STRBUF_INIT;
	بنية strbuf meta = STRBUF_INIT;
	بنية option *parseopts;
	بنية rev_info revs;
	صحيح سٽ_رڪن = '\0';
	صحيح ret;

	ثابت حرف * ثابت builtin_diff_جوڙا_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		عدم
	};
	بنية option builtin_diff_جوڙا_options[] = {
		OPT_پڇاڙي()
	};

	repo_init_revisions(repo, &revs, اڳياڙي);

	/*
	 * Diff options are usually parsed implicitly as part of
	 * setup_revisions(). Explicitly handle parsing to ensure options are
	 * printed in the usage message.
	 */
	parseopts = جوڙيو_diff_options(builtin_diff_جوڙا_options, &revs.diffopt);
	show_usage_with_options_if_asked(argc, argv, builtin_diff_جوڙا_usage, parseopts);

	repo_config(repo, git_diff_basic_config, عدم);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, اڳياڙي, parseopts, builtin_diff_جوڙا_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	إذا (تياري_revisions(argc, argv, &revs, عدم) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	إذا (revs.diffopt.سٽ_termination)
		usage(_("working without -z is not supported"));

	إذا (revs.prune_معلومات.nr)
		usage(_("pathspec arguments not supported"));

	إذا (revs.pending.nr || revs.max_ڳڻپ != -1 ||
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
		بنانشان گهڻائي_a, گهڻائي_b;
		ثابت حرف *p;
		حرف حالت;

		إذا (strbuf_getwholeline(&meta, stdin, سٽ_رڪن) == EOF)
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

		گهڻائي_a = parse_گهڻائي_or_die(p, &p);
		گهڻائي_b = parse_گهڻائي_or_die(p, &p);

		إذا (S_ISDIR(گهڻائي_a) || S_ISDIR(گهڻائي_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		حالت = *p++;

		إذا (strbuf_getwholeline(&رستو, stdin, سٽ_رڪن) == EOF)
			die(_("got EOF while reading path"));

		بدل (حالت) {
		حالة DIFF_حالت_ADDED:
			pair = diff_قطار_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', گهڻائي_b,
						    &oid_b, 1, رستو.buf, 0);
			إذا (pair)
				pair->حالت = حالت;
			اكسر;

		حالة DIFF_حالت_DELETED:
			pair = diff_قطار_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', گهڻائي_a,
						    &oid_a, 1, رستو.buf, 0);
			إذا (pair)
				pair->حالت = حالت;
			اكسر;

		حالة DIFF_حالت_TYPE_بدليو:
		حالة DIFF_حالت_MODIFIED:
			pair = diff_قطار_change(&diff_queued_diff, &revs.diffopt,
						 گهڻائي_a, گهڻائي_b, &oid_a, &oid_b,
						 1, 1, رستو.buf, 0, 0);
			إذا (pair)
				pair->حالت = حالت;
			اكسر;

		حالة DIFF_حالت_RENAMED:
		حالة DIFF_حالت_COPIED: {
				بنية diff_filespec *a, *b;
				بنانشان صحيح score;

				إذا (strbuf_getwholeline(&رستو_dst, stdin, سٽ_رڪن) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(رستو.buf);
				b = alloc_filespec(رستو_dst.buf);
				fill_filespec(a, &oid_a, 1, گهڻائي_a);
				fill_filespec(b, &oid_b, 1, گهڻائي_b);

				pair = diff_قطار(&diff_queued_diff, a, b);

				إذا (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->حالت = حالت;
				pair->renamed_pair = 1;
			}
			اكسر;

		افتراضي:
			die(_("unknown diff status: %c"), حالت);
		}
	}

	revs.diffopt.no_free = 0;
	diffcore_std(&revs.diffopt);
	diff_flush(&revs.diffopt);
	ret = diff_نتيجو_ڪوڊ(&revs);

	strbuf_release(&رستو_dst);
	strbuf_release(&رستو);
	strbuf_release(&meta);
	release_revisions(&revs);
	FREE_AND_NULL(parseopts);

	أرجع ret;
}
