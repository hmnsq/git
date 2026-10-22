#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "lockfile.h"
#ضمن "commit.h"
#ضمن "tag.h"
#ضمن "refs.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "read-cache-ll.h"
#ضمن "revision.h"
#ضمن "diff.h"
#ضمن "hashmap.h"
#ضمن "setup.h"
#ضمن "strvec.h"
#ضمن "run-command.h"
#ضمن "odb.h"
#ضمن "list-objects.h"
#ضمن "commit-slab.h"
#ضمن "wildmatch.h"
#ضمن "prio-queue.h"
#ضمن "oidset.h"

#تعریف_کن MAX_برچسبها	(FLAG_بیتها - 1)
#تعریف_کن DEFAULT_CANDIDATES 10

define_commit_slab(commit_نامها, بنية commit_name *);

ساكن ثابت حرف * ثابت describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	عدم
};

ساكن صحيح debug;	/* Display lots of verbose info */
ساكن صحيح all;	/* Any valid ref can be used */
ساكن صحيح برچسبها;	/* Allow lightweight tags */
ساكن صحيح longformat;
ساكن صحيح اولی_والد;
ساكن صحيح abbrev = -1; /* unspecified */
ساكن صحيح max_candidates = DEFAULT_CANDIDATES;
ساكن بنية hashmap نامها;
ساكن صحيح have_util;
ساكن بنية string_list patterns = STRING_LIST_INIT_NODUP;
ساكن بنية string_list exclude_patterns = STRING_LIST_INIT_NODUP;
ساكن صحيح always;
ساكن ثابت حرف *suffix, *dirty, *broken;
ساكن بنية commit_نامها commit_نامها;

/* diff-index command arguments to check if working tree is dirty. */
ساكن ثابت حرف *diff_شاخص_آرگومانها[] = {
	"diff-index", "--quiet", "HEAD", "--", عدم
};

ساكن ثابت حرف *update_شاخص_آرگومانها[] = {
	"update-index", "--unmerged", "-q", "--refresh", عدم
};

بنية commit_name {
	بنية hashmap_مدخل مدخل;
	بنية object_id peeled;
	بنية برچسب *برچسب;
	بیعلامت prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	بیعلامت name_checked:1;
	بیعلامت misnamed:1;
	بنية object_id oid;
	حرف *مسیر;
};

ساكن ثابت حرف *prio_نامها[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

ساكن صحيح commit_name_neq(ثابت فراغ *cmp_داده استفادهنشده,
			   ثابت بنية hashmap_مدخل *eptr,
			   ثابت بنية hashmap_مدخل *مدخل_or_کلید,
			   ثابت فراغ *peeled)
{
	ثابت بنية commit_name *cn1, *cn2;

	cn1 = container_از(eptr, ثابت بنية commit_name, مدخل);
	cn2 = container_از(مدخل_or_کلید, ثابت بنية commit_name, مدخل);

	أرجع !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

ساكن مضمن بنية commit_name *پیدا_کن_commit_name(ثابت بنية object_id *peeled)
{
	أرجع hashmap_get_مدخل_from_hash(&نامها, oidhash(peeled), peeled,
						بنية commit_name, مدخل);
}

ساكن صحيح replace_name(بنية commit_name *e,
			       صحيح prio,
			       ثابت بنية object_id *oid,
			       بنية برچسب **برچسب)
{
	إذا (!e || e->prio < prio)
		أرجع 1;

	إذا (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		بنية برچسب *t;

		إذا (!e->برچسب) {
			t = lookup_برچسب(the_repository, &e->oid);
			إذا (!t || parse_برچسب(the_repository, t))
				أرجع 1;
			e->برچسب = t;
		}

		t = lookup_برچسب(the_repository, oid);
		إذا (!t || parse_برچسب(the_repository, t))
			أرجع 0;
		*برچسب = t;

		إذا (e->برچسب->date < t->date)
			أرجع 1;
	}

	أرجع 0;
}

ساكن فراغ جمع_کن_مقصد_معلوم_نامها(ثابت حرف *مسیر,
			       ثابت بنية object_id *peeled,
			       صحيح prio,
			       ثابت بنية object_id *oid)
{
	بنية commit_name *e = پیدا_کن_commit_name(peeled);
	بنية برچسب *برچسب = عدم;
	إذا (replace_name(e, prio, oid, &برچسب)) {
		إذا (!e) {
			e = xmalloc(حجم(بنية commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_مدخل_init(&e->مدخل, oidhash(peeled));
			hashmap_جمع_کن(&نامها, &e->مدخل);
			e->مسیر = عدم;
		}
		e->برچسب = برچسب;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		حرر(e->مسیر);
		e->مسیر = xstrdup(مسیر);
	}
}

ساكن صحيح get_name(ثابت بنية reference *ref, فراغ *cb_داده استفادهنشده)
{
	صحيح is_برچسب = 0;
	بنية object_id peeled;
	صحيح is_annotated, prio;
	ثابت حرف *مسیر_مقصد_تطابق = عدم;

	إذا (skip_پیشوند(ref->name, "refs/tags/", &مسیر_مقصد_تطابق)) {
		is_برچسب = 1;
	} وإلا إذا (all) {
		إذا ((exclude_patterns.nr || patterns.nr) &&
		    !skip_پیشوند(ref->name, "refs/heads/", &مسیر_مقصد_تطابق) &&
		    !skip_پیشوند(ref->name, "refs/remotes/", &مسیر_مقصد_تطابق)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			أرجع 0;
		}
	} وإلا {
		/* Reject anything outside refs/tags/ unless --all */
		أرجع 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	إذا (exclude_patterns.nr) {
		بنية string_list_قلم *قلم;

		for_each_string_list_قلم(قلم, &exclude_patterns) {
			إذا (!wildmatch(قلم->string, مسیر_مقصد_تطابق, 0))
				أرجع 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	إذا (patterns.nr) {
		صحيح یافت_2 = 0;
		بنية string_list_قلم *قلم;

		for_each_string_list_قلم(قلم, &patterns) {
			إذا (!wildmatch(قلم->string, مسیر_مقصد_تطابق, 0)) {
				یافت_2 = 1;
				اكسر;
			}
		}

		إذا (!یافت_2)
			أرجع 0;
	}

	/* Is it annotated? */
	إذا (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} وإلا {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	إذا (is_annotated)
		prio = 2;
	وإلا إذا (is_برچسب)
		prio = 1;
	وإلا
		prio = 0;

	جمع_کن_مقصد_معلوم_نامها(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	أرجع 0;
}

بنية possible_برچسب {
	بنية commit_name *name;
	صحيح عمق;
	صحيح یافت_ترتیب;
	بیعلامت flag_within;
};

ساكن صحيح compare_pt(ثابت فراغ *a_, ثابت فراغ *b_)
{
	بنية possible_برچسب *a = (بنية possible_برچسب *)a_;
	بنية possible_برچسب *b = (بنية possible_برچسب *)b_;
	إذا (a->عمق != b->عمق)
		أرجع a->عمق - b->عمق;
	إذا (a->یافت_ترتیب != b->یافت_ترتیب)
		أرجع a->یافت_ترتیب - b->یافت_ترتیب;
	أرجع 0;
}

ساكن بیعلامت دراز پایان_عمق_computation(بنية prio_صف *صف,
					      بنية possible_برچسب *بهترین)
{
	بیعلامت دراز دیده_commits = 0;
	بنية oidset unflagged = OIDSET_INIT;
	بنية commit *c;

	prio_صف_for_each(صف, c) {
		إذا (!(c->object.flags & بهترین->flag_within))
			oidset_درج(&unflagged, &c->object.oid);
	}

	طالما ((c = prio_صف_get(صف))) {
		بنية commit_list *parents = c->parents;
		دیده_commits++;
		إذا (c->object.flags & بهترین->flag_within) {
			إذا (!oidset_اندازه(&unflagged))
				اكسر;
		} وإلا {
			oidset_remove(&unflagged, &c->object.oid);
			بهترین->عمق++;
		}
		طالما (parents) {
			بیعلامت دیده, flag_before, flag_after;
			بنية commit *p = parents->قلم;
			repo_parse_commit(the_repository, p);
			دیده = p->object.flags & دیده_2;
			إذا (!دیده)
				prio_صف_put(صف, p);
			flag_before = p->object.flags & بهترین->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & بهترین->flag_within;
			إذا (!دیده && !flag_after)
				oidset_درج(&unflagged, &p->object.oid);
			إذا (دیده && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->بعدی;
		}
	}
	oidset_clear(&unflagged);
	أرجع دیده_commits;
}

ساكن فراغ append_name(بنية commit_name *n, بنية strbuf *dst)
{
	إذا (n->prio == 2 && !n->برچسب) {
		n->برچسب = lookup_برچسب(the_repository, &n->oid);
		إذا (!n->برچسب || parse_برچسب(the_repository, n->برچسب))
			die(_("annotated tag %s not available"), n->مسیر);
	}
	إذا (n->برچسب && !n->name_checked) {
		إذا (strcmp(n->برچسب->برچسب, all ? n->مسیر + 5 : n->مسیر)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->مسیر, n->برچسب->برچسب);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	إذا (n->برچسب) {
		إذا (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->برچسب->برچسب);
	} وإلا {
		strbuf_addstr(dst, n->مسیر);
	}
}

ساكن فراغ append_suffix(صحيح عمق, ثابت بنية object_id *oid, بنية strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", عمق,
		    repo_پیدا_کن_unique_abbrev(the_repository, oid, abbrev));
}

ساكن فراغ describe_commit(بنية commit *cmit, بنية strbuf *dst)
{
	بنية commit *c, *gave_به_بالا_on = عدم;
	بنية prio_صف صف = { compare_commits_by_commit_date };
	بنية commit_name *n;
	بنية possible_برچسب all_matches[MAX_برچسبها];
	بیعلامت صحيح تطابق_cnt = 0, annotated_cnt = 0, cur_تطابق;
	بیعلامت دراز دیده_commits = 0;
	بیعلامت صحيح unannotated_cnt = 0;

	n = پیدا_کن_commit_name(&cmit->object.oid);
	إذا (n && (برچسبها || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		إذا (n->misnamed || longformat)
			append_suffix(0, n->برچسب ? get_tagged_oid(n->برچسب) : &cmit->object.oid, dst);
		إذا (suffix)
			strbuf_addstr(dst, suffix);
		أرجع;
	}

	إذا (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_مقصد_hex(&cmit->object.oid));
	إذا (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	إذا (!have_util) {
		بنية hashmap_iter iter;
		بنية commit *c;
		بنية commit_name *n;

		init_commit_نامها(&commit_نامها);
		hashmap_for_each_مدخل(&نامها, &iter, n,
					مدخل /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			إذا (c)
				*commit_نامها_at(&commit_نامها, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = دیده_2;
	prio_صف_put(&صف, cmit);
	طالما ((c = prio_صف_get(&صف))) {
		بنية commit_list *parents = c->parents;
		بنية commit_name **جایگاه;

		دیده_commits++;

		إذا (تطابق_cnt == max_candidates ||
		    تطابق_cnt == hashmap_get_اندازه(&نامها)) {
			gave_به_بالا_on = c;
			اكسر;
		}

		جایگاه = commit_نامها_peek(&commit_نامها, c);
		n = جایگاه ? *جایگاه : عدم;
		إذا (n) {
			إذا (!برچسبها && !all && n->prio < 2) {
				unannotated_cnt++;
			} وإلا إذا (تطابق_cnt < max_candidates) {
				بنية possible_برچسب *t = &all_matches[تطابق_cnt++];
				t->name = n;
				t->عمق = دیده_commits - 1;
				t->flag_within = 1u << تطابق_cnt;
				t->یافت_ترتیب = تطابق_cnt;
				c->object.flags |= t->flag_within;
				إذا (n->prio == 2)
					annotated_cnt++;
			}
		}
		لكل (cur_تطابق = 0; cur_تطابق < تطابق_cnt; cur_تطابق++) {
			بنية possible_برچسب *t = &all_matches[cur_تطابق];
			إذا (!(c->object.flags & t->flag_within))
				t->عمق++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		إذا (annotated_cnt && !prio_صف_اندازه(&صف)) {
			صحيح بهترین_عمق = بیشترین_صحیح;
			بیعلامت بهترین_within = 0;
			لكل (cur_تطابق = 0; cur_تطابق < تطابق_cnt; cur_تطابق++) {
				بنية possible_برچسب *t = &all_matches[cur_تطابق];
				إذا (t->عمق < بهترین_عمق) {
					بهترین_عمق = t->عمق;
					بهترین_within = t->flag_within;
				} وإلا إذا (t->عمق == بهترین_عمق) {
					بهترین_within |= t->flag_within;
				}
			}
			إذا ((c->object.flags & بهترین_within) == بهترین_within) {
				إذا (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_مقصد_hex(&c->object.oid));
				اكسر;
			}
		}
		طالما (parents) {
			بنية commit *p = parents->قلم;
			repo_parse_commit(the_repository, p);
			إذا (!(p->object.flags & دیده_2))
				prio_صف_put(&صف, p);
			p->object.flags |= c->object.flags;
			parents = parents->بعدی;

			إذا (اولی_والد)
				اكسر;
		}
	}

	إذا (!تطابق_cnt) {
		بنية object_id *cmit_oid = &cmit->object.oid;
		إذا (always) {
			strbuf_جمع_کن_unique_abbrev(dst, cmit_oid, abbrev);
			إذا (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_صف(&صف);
			أرجع;
		}
		إذا (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_مقصد_hex(cmit_oid));
		وإلا
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_مقصد_hex(cmit_oid));
	}

	QSORT(all_matches, تطابق_cnt, compare_pt);

	إذا (gave_به_بالا_on) {
		prio_صف_put(&صف, gave_به_بالا_on);
		دیده_commits--;
	}
	دیده_commits += پایان_عمق_computation(&صف, &all_matches[0]);
	clear_prio_صف(&صف);

	إذا (debug) {
		ساكن صحيح label_عرض = -1;
		إذا (label_عرض < 0) {
			صحيح i, w;
			لكل (i = 0; i < ARRAY_اندازه(prio_نامها); i++) {
				w = طول_رشته(_(prio_نامها[i]));
				إذا (label_عرض < w)
					label_عرض = w;
			}
		}
		لكل (cur_تطابق = 0; cur_تطابق < تطابق_cnt; cur_تطابق++) {
			بنية possible_برچسب *t = &all_matches[cur_تطابق];
			fprintf(stderr, " %-*s %8d %s\n",
				label_عرض, _(prio_نامها[t->name->prio]),
				t->عمق, t->name->مسیر);
		}
		fprintf(stderr, _("traversed %lu commits\n"), دیده_commits);
		إذا (gave_به_بالا_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_مقصد_hex(&gave_به_بالا_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	إذا (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].عمق, &cmit->object.oid, dst);
	إذا (suffix)
		strbuf_addstr(dst, suffix);
}

بنية process_commit_داده {
	بنية commit *current_commit;
	ثابت بنية object_id *looking_for;
	بنية strbuf *dst;
	بنية rev_info *revs;
};

ساكن فراغ process_commit(بنية commit *commit, فراغ *داده)
{
	بنية process_commit_داده *pcd = داده;
	pcd->current_commit = commit;
}

ساكن فراغ process_object(بنية object *obj, ثابت حرف *مسیر, فراغ *داده)
{
	بنية process_commit_داده *pcd = داده;

	إذا (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		إذا (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", مسیر);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = عدم;
	}
}

ساكن فراغ describe_blob(ثابت بنية object_id *oid, بنية strbuf *dst)
{
	بنية rev_info revs;
	بنية strvec آرگومانها = STRVEC_INIT;
	بنية object_id سر_oid;
	بنية process_commit_داده pcd = { عدم, oid, dst, &revs};

	إذا (repo_get_oid(the_repository, "HEAD", &سر_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_مقصد_hex(oid));

	strvec_pushl(&آرگومانها, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_مقصد_hex(&سر_oid),
		     عدم);

	repo_init_revisions(the_repository, &revs, عدم);
	راهاندازی_revisions_from_strvec(&آرگومانها, &revs, عدم);
	إذا (آرگومانها.nr > 1)
		BUG("setup_revisions could not handle all args?");

	إذا (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&آرگومانها);

	إذا (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_مقصد_hex(oid));
}

ساكن فراغ describe(ثابت حرف *arg, صحيح آخرین_one)
{
	بنية object_id oid;
	بنية commit *cmit;
	بنية strbuf sb = STRBUF_INIT;

	إذا (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	إذا (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	إذا (cmit)
		describe_commit(cmit, &sb);
	وإلا إذا (odb_read_object_info(the_repository->objects,
				      &oid, عدم) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	وإلا
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	إذا (!آخرین_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

ساكن صحيح option_parse_exact_تطابق(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	صحيح *val = opt->مقدار;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	أرجع 0;
}

صحيح cmd_describe(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *پیشوند,
		 بنية repository *repo استفادهنشده )
{
	بنية refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	صحيح شامل_است = 0;
	بنية option options[] = {
		OPT_BOOL(0, "contains",   &شامل_است, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &برچسبها, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &اولی_والد, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, عدم,
			       N_("only output exact matches"),
			       PARSE_OPT_NOARG, option_parse_exact_تطابق),
		OPT_INTEGER(0, "candidates", &max_candidates,
			    N_("consider <n> most recent tags (default: 10)")),
		OPT_STRING_LIST(0, "match", &patterns, N_("pattern"),
			   N_("only consider tags matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &exclude_patterns, N_("pattern"),
			   N_("do not consider tags matching <pattern>")),
		OPT_BOOL(0, "always",        &always,
			N_("show abbreviated commit object as fallback")),
		{
			.type = OPTION_STRING,
			.long_name = "dirty",
			.مقدار = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.مقدار = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_انتها(),
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, پیشوند, options, describe_usage, 0);
	إذا (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	إذا (max_candidates < 0)
		max_candidates = 0;
	وإلا إذا (max_candidates > MAX_برچسبها)
		max_candidates = MAX_برچسبها;

	save_commit_میانگیر = 0;

	إذا (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	إذا (شامل_است) {
		بنية string_list_قلم *قلم;
		بنية strvec آرگومانها;
		ثابت حرف **argv_copy;
		صحيح ret;

		strvec_init(&آرگومانها);
		strvec_pushl(&آرگومانها, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     عدم);
		إذا (always)
			strvec_افزودن(&آرگومانها, "--always");
		إذا (!all)
			strvec_افزودن(&آرگومانها, "--tags");

		for_each_string_list_قلم(قلم, &patterns)
			strvec_pushf(&آرگومانها, "--refs=refs/tags/%s", قلم->string);
		for_each_string_list_قلم(قلم, &exclude_patterns)
			strvec_pushf(&آرگومانها, "--exclude=refs/tags/%s", قلم->string);

		إذا (all) {
			for_each_string_list_قلم(قلم, &patterns)
				strvec_pushf(&آرگومانها, "--refs=refs/heads/%s", قلم->string);
			for_each_string_list_قلم(قلم, &exclude_patterns)
				strvec_pushf(&آرگومانها, "--exclude=refs/heads/%s", قلم->string);
			for_each_string_list_قلم(قلم, &patterns)
				strvec_pushf(&آرگومانها, "--refs=refs/remotes/%s", قلم->string);
			for_each_string_list_قلم(قلم, &exclude_patterns)
				strvec_pushf(&آرگومانها, "--exclude=refs/remotes/%s", قلم->string);
		}

		إذا (argc)
			strvec_pushv(&آرگومانها, argv);
		وإلا
			strvec_افزودن(&آرگومانها, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, آرگومانها.nr + 1);
		لكل (اندازه_t i = 0; i < آرگومانها.nr; i++)
			argv_copy[i] = آرگومانها.v[i];
		argv_copy[آرگومانها.nr] = عدم;

		ret = cmd_name_rev(آرگومانها.nr, argv_copy, پیشوند, the_repository);

		strvec_clear(&آرگومانها);
		حرر(argv_copy);
		أرجع ret;
	}

	إذا (!all)
		for_each_ref_opts.پیشوند = "refs/tags/";

	hashmap_init(&نامها, commit_name_neq, عدم, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, عدم, &for_each_ref_opts);
	إذا (!hashmap_get_اندازه(&نامها) && !always)
		die(_("No names found, cannot describe anything."));

	إذا (argc == 0) {
		إذا (broken) {
			بنية فرزند_process cp = فرزند_PROCESS_INIT;

			strvec_pushv(&cp.آرگومانها, update_شاخص_آرگومانها);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			پیاپی_command(&cp);

			فرزند_process_init(&cp);
			strvec_pushv(&cp.آرگومانها, diff_شاخص_آرگومانها);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			إذا (!dirty)
				dirty = "-dirty";

			بدل (پیاپی_command(&cp)) {
			حالة 0:
				suffix = عدم;
				اكسر;
			حالة 1:
				suffix = dirty;
				اكسر;
			افتراضي:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} وإلا إذا (dirty) {
			بنية قفل_file شاخص_قفل = قفل_INIT;
			بنية rev_info revs;
			صحيح fd;

			راهاندازی_work_درخت(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_پر_شاخص = 0;
			repo_read_شاخص(the_repository);
			refresh_شاخص(the_repository->شاخص, REFRESH_QUIET|REFRESH_UNMERGED,
				      عدم, عدم, عدم);
			fd = repo_hold_locked_شاخص(the_repository,
						    &شاخص_قفل, 0);
			إذا (0 <= fd)
				repo_update_شاخص_if_able(the_repository, &شاخص_قفل);

			repo_init_revisions(the_repository, &revs, پیشوند);

			إذا (راهاندازی_revisions(ARRAY_اندازه(diff_شاخص_آرگومانها) - 1,
					    diff_شاخص_آرگومانها, &revs, عدم) != 1)
				BUG("malformed internal diff-index command line");
			پیاپی_diff_شاخص(&revs, 0);

			إذا (!diff_نتیجه_کد(&revs))
				suffix = عدم;
			وإلا
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} وإلا إذا (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} وإلا إذا (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} وإلا {
		طالما (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	أرجع 0;
}
