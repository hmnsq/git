#شامل "builtin.h"
#شامل "config.h"
#شامل "diff.h"
#شامل "diffcore.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "hex.h"
#شامل "object.h"
#شامل "parse-options.h"
#شامل "revision.h"
#شامل "strbuf.h"

ساکن غیرعلامتی parse_منوال_or_die(ثابت حرف *منوال, ثابت حرف **انتہا)
{
	uint16_t ret;

	*انتہا = parse_منوال(منوال, &ret);
	اگر (!*انتہا)
		die(_("unable to parse mode: %s"), منوال);
	واپس ret;
}

ساکن خالی parse_oid_or_die(ثابت حرف *hex, ساخت object_id *oid,
			     ثابت حرف **انتہا, ثابت ساخت git_hash_algo *algop)
{
	اگر (parse_oid_hex_algop(hex, oid, انتہا, algop) || *(*انتہا)++ != ' ')
		die(_("unable to parse object id: %s"), hex);
}

صحیح cmd_diff_جوڑے(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		   ساخت repository *repo)
{
	ساخت strbuf راستہ_dst = STRBUF_INIT;
	ساخت strbuf راستہ = STRBUF_INIT;
	ساخت strbuf meta = STRBUF_INIT;
	ساخت option *parseopts;
	ساخت rev_info revs;
	صحیح سطر_رکن = '\0';
	صحیح ret;

	ثابت حرف * ثابت builtin_diff_جوڑے_usage[] = {
		N_("git diff-pairs -z [<diff-options>]"),
		تہی
	};
	ساخت option builtin_diff_جوڑے_options[] = {
		OPT_انتہا()
	};

	repo_init_revisions(repo, &revs, سابقہ);

	/*
	 * Diff options are usually parsed implicitly as part of
	 * setup_revisions(). Explicitly handle parsing to ensure options are
	 * printed in the usage message.
	 */
	parseopts = جوڑو_diff_options(builtin_diff_جوڑے_options, &revs.diffopt);
	show_usage_with_options_if_asked(argc, argv, builtin_diff_جوڑے_usage, parseopts);

	repo_config(repo, git_diff_basic_config, تہی);
	revs.diffopt.no_free = 1;
	revs.disable_stdin = 1;
	revs.abbrev = 0;
	revs.diff = 1;

	argc = parse_options(argc, argv, سابقہ, parseopts, builtin_diff_جوڑے_usage,
			     PARSE_OPT_KEEP_ARGV0 | PARSE_OPT_KEEP_DASHDASH);

	اگر (تیاری_revisions(argc, argv, &revs, تہی) > 1)
		usagef(_("unrecognized argument: %s"), argv[0]);

	/*
	 * With the -z option, both command input and raw output are
	 * NUL-delimited (this mode does not affect patch output). At present
	 * only NUL-delimited raw diff formatted input is supported.
	 */
	اگر (revs.diffopt.سطر_termination)
		usage(_("working without -z is not supported"));

	اگر (revs.prune_ڈیٹا.nr)
		usage(_("pathspec arguments not supported"));

	اگر (revs.pending.nr || revs.max_گنتی != -1 ||
	    revs.min_age != (timestamp_t)-1 ||
	    revs.max_age != (timestamp_t)-1)
		usage(_("revision arguments not allowed"));

	اگر (!revs.diffopt.output_format)
		revs.diffopt.output_format = DIFF_FORMAT_PATCH;

	/*
	 * If rename detection is not requested, use rename information from the
	 * raw diff formatted input. Setting skip_resolving_statuses ensures
	 * diffcore_std() does not mess with rename information already present
	 * in queued filepairs.
	 */
	اگر (!revs.diffopt.detect_rename)
		revs.diffopt.skip_resolving_statuses = 1;

	جب (1) {
		ساخت object_id oid_a, oid_b;
		ساخت diff_filepair *pair;
		غیرعلامتی منوال_a, منوال_b;
		ثابت حرف *p;
		حرف حالت_2;

		اگر (strbuf_getwholeline(&meta, stdin, سطر_رکن) == EOF)
			توڑو;

		p = meta.buf;
		اگر (!*p) {
			diffcore_std(&revs.diffopt);
			diff_flush(&revs.diffopt);
			/*
			 * When the diff queue is explicitly flushed, append a
			 * NUL byte to separate batches of diffs.
			 */
			fputc('\0', revs.diffopt.file);
			fflush(revs.diffopt.file);
			جاری;
		}

		اگر (*p != ':')
			die(_("invalid raw diff input"));
		p++;

		منوال_a = parse_منوال_or_die(p, &p);
		منوال_b = parse_منوال_or_die(p, &p);

		اگر (S_ISDIR(منوال_a) || S_ISDIR(منوال_b))
			die(_("tree objects not supported"));

		parse_oid_or_die(p, &oid_a, &p, repo->hash_algo);
		parse_oid_or_die(p, &oid_b, &p, repo->hash_algo);

		حالت_2 = *p++;

		اگر (strbuf_getwholeline(&راستہ, stdin, سطر_رکن) == EOF)
			die(_("got EOF while reading path"));

		انتخاب (حالت_2) {
		حالت DIFF_حالت_ADDED:
			pair = diff_قطار_addremove(&diff_queued_diff,
						    &revs.diffopt, '+', منوال_b,
						    &oid_b, 1, راستہ.buf, 0);
			اگر (pair)
				pair->حالت_2 = حالت_2;
			توڑو;

		حالت DIFF_حالت_DELETED:
			pair = diff_قطار_addremove(&diff_queued_diff,
						    &revs.diffopt, '-', منوال_a,
						    &oid_a, 1, راستہ.buf, 0);
			اگر (pair)
				pair->حالت_2 = حالت_2;
			توڑو;

		حالت DIFF_حالت_TYPE_بدلا:
		حالت DIFF_حالت_MODIFIED:
			pair = diff_قطار_change(&diff_queued_diff, &revs.diffopt,
						 منوال_a, منوال_b, &oid_a, &oid_b,
						 1, 1, راستہ.buf, 0, 0);
			اگر (pair)
				pair->حالت_2 = حالت_2;
			توڑو;

		حالت DIFF_حالت_RENAMED:
		حالت DIFF_حالت_COPIED: {
				ساخت diff_filespec *a, *b;
				غیرعلامتی صحیح score;

				اگر (strbuf_getwholeline(&راستہ_dst, stdin, سطر_رکن) == EOF)
					die(_("got EOF while reading destination path"));

				a = alloc_filespec(راستہ.buf);
				b = alloc_filespec(راستہ_dst.buf);
				fill_filespec(a, &oid_a, 1, منوال_a);
				fill_filespec(b, &oid_b, 1, منوال_b);

				pair = diff_قطار(&diff_queued_diff, a, b);

				اگر (strtoul_ui(p, 10, &score))
					die(_("unable to parse rename/copy score: %s"), p);

				pair->score = score * MAX_SCORE / 100;
				pair->حالت_2 = حالت_2;
				pair->renamed_pair = 1;
			}
			توڑو;

		طےشدہ:
			die(_("unknown diff status: %c"), حالت_2);
		}
	}

	revs.diffopt.no_free = 0;
	diffcore_std(&revs.diffopt);
	diff_flush(&revs.diffopt);
	ret = diff_نتیجہ_رمز(&revs);

	strbuf_release(&راستہ_dst);
	strbuf_release(&راستہ);
	strbuf_release(&meta);
	release_revisions(&revs);
	FREE_AND_NULL(parseopts);

	واپس ret;
}
