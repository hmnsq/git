#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

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

#تعریف_کړه MAX_نښانونه	(FLAG_بټونه - 1)
#تعریف_کړه DEFAULT_CANDIDATES 10

define_commit_slab(commit_نومونه, بنية commit_name *);

ساكن ثابت حرف * ثابت describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	عدم
};

ساكن صحيح debug;	/* Display lots of verbose info */
ساكن صحيح all;	/* Any valid ref can be used */
ساكن صحيح نښانونه;	/* Allow lightweight tags */
ساكن صحيح longformat;
ساكن صحيح لومړی_پلار;
ساكن صحيح abbrev = -1; /* unspecified */
ساكن صحيح max_candidates = DEFAULT_CANDIDATES;
ساكن بنية hashmap نومونه;
ساكن صحيح have_util;
ساكن بنية string_list patterns = STRING_LIST_INIT_NODUP;
ساكن بنية string_list exclude_patterns = STRING_LIST_INIT_NODUP;
ساكن صحيح always;
ساكن ثابت حرف *suffix, *dirty, *broken;
ساكن بنية commit_نومونه commit_نومونه;

/* diff-index command arguments to check if working tree is dirty. */
ساكن ثابت حرف *diff_شاخص_دلیلونه[] = {
	"diff-index", "--quiet", "HEAD", "--", عدم
};

ساكن ثابت حرف *update_شاخص_دلیلونه[] = {
	"update-index", "--unmerged", "-q", "--refresh", عدم
};

بنية commit_name {
	بنية hashmap_ثبت ثبت;
	بنية object_id peeled;
	بنية نښان *نښان;
	بینښه prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	بینښه name_checked:1;
	بینښه misnamed:1;
	بنية object_id oid;
	حرف *لار;
};

ساكن ثابت حرف *prio_نومونه[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

ساكن صحيح commit_name_neq(ثابت فراغ *cmp_معلومات ناکارول_شوی,
			   ثابت بنية hashmap_ثبت *eptr,
			   ثابت بنية hashmap_ثبت *ثبت_or_کلی,
			   ثابت فراغ *peeled)
{
	ثابت بنية commit_name *cn1, *cn2;

	cn1 = container_د(eptr, ثابت بنية commit_name, ثبت);
	cn2 = container_د(ثبت_or_کلی, ثابت بنية commit_name, ثبت);

	أرجع !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

ساكن مضمن بنية commit_name *پیدا_کړه_commit_name(ثابت بنية object_id *peeled)
{
	أرجع hashmap_get_ثبت_from_hash(&نومونه, oidhash(peeled), peeled,
						بنية commit_name, ثبت);
}

ساكن صحيح replace_name(بنية commit_name *e,
			       صحيح prio,
			       ثابت بنية object_id *oid,
			       بنية نښان **نښان)
{
	إذا (!e || e->prio < prio)
		أرجع 1;

	إذا (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		بنية نښان *t;

		إذا (!e->نښان) {
			t = lookup_نښان(the_repository, &e->oid);
			إذا (!t || parse_نښان(the_repository, t))
				أرجع 1;
			e->نښان = t;
		}

		t = lookup_نښان(the_repository, oid);
		إذا (!t || parse_نښان(the_repository, t))
			أرجع 0;
		*نښان = t;

		إذا (e->نښان->date < t->date)
			أرجع 1;
	}

	أرجع 0;
}

ساكن فراغ جمع_کړه_منزل_معلوم_نومونه(ثابت حرف *لار,
			       ثابت بنية object_id *peeled,
			       صحيح prio,
			       ثابت بنية object_id *oid)
{
	بنية commit_name *e = پیدا_کړه_commit_name(peeled);
	بنية نښان *نښان = عدم;
	إذا (replace_name(e, prio, oid, &نښان)) {
		إذا (!e) {
			e = xmalloc(حجم(بنية commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_ثبت_init(&e->ثبت, oidhash(peeled));
			hashmap_جمع_کړه(&نومونه, &e->ثبت);
			e->لار = عدم;
		}
		e->نښان = نښان;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		حرر(e->لار);
		e->لار = xstrdup(لار);
	}
}

ساكن صحيح get_name(ثابت بنية reference *ref, فراغ *cb_معلومات ناکارول_شوی)
{
	صحيح is_نښان = 0;
	بنية object_id peeled;
	صحيح is_annotated, prio;
	ثابت حرف *لار_منزل_سمون = عدم;

	إذا (skip_مختاړی(ref->name, "refs/tags/", &لار_منزل_سمون)) {
		is_نښان = 1;
	} وإلا إذا (all) {
		إذا ((exclude_patterns.nr || patterns.nr) &&
		    !skip_مختاړی(ref->name, "refs/heads/", &لار_منزل_سمون) &&
		    !skip_مختاړی(ref->name, "refs/remotes/", &لار_منزل_سمون)) {
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
		بنية string_list_توکی *توکی;

		for_each_string_list_توکی(توکی, &exclude_patterns) {
			إذا (!wildmatch(توکی->string, لار_منزل_سمون, 0))
				أرجع 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	إذا (patterns.nr) {
		صحيح وموندل_2 = 0;
		بنية string_list_توکی *توکی;

		for_each_string_list_توکی(توکی, &patterns) {
			إذا (!wildmatch(توکی->string, لار_منزل_سمون, 0)) {
				وموندل_2 = 1;
				اكسر;
			}
		}

		إذا (!وموندل_2)
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
	وإلا إذا (is_نښان)
		prio = 1;
	وإلا
		prio = 0;

	جمع_کړه_منزل_معلوم_نومونه(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	أرجع 0;
}

بنية possible_نښان {
	بنية commit_name *name;
	صحيح ژوروالی;
	صحيح وموندل_ترتیب;
	بینښه flag_within;
};

ساكن صحيح compare_pt(ثابت فراغ *a_, ثابت فراغ *b_)
{
	بنية possible_نښان *a = (بنية possible_نښان *)a_;
	بنية possible_نښان *b = (بنية possible_نښان *)b_;
	إذا (a->ژوروالی != b->ژوروالی)
		أرجع a->ژوروالی - b->ژوروالی;
	إذا (a->وموندل_ترتیب != b->وموندل_ترتیب)
		أرجع a->وموندل_ترتیب - b->وموندل_ترتیب;
	أرجع 0;
}

ساكن بینښه اوږد پای_ژوروالی_computation(بنية prio_کتار *کتار,
					      بنية possible_نښان *غوره)
{
	بینښه اوږد کتل_شوی_commits = 0;
	بنية oidset unflagged = OIDSET_INIT;
	بنية commit *c;

	prio_کتار_for_each(کتار, c) {
		إذا (!(c->object.flags & غوره->flag_within))
			oidset_ورننباسه(&unflagged, &c->object.oid);
	}

	طالما ((c = prio_کتار_get(کتار))) {
		بنية commit_list *parents = c->parents;
		کتل_شوی_commits++;
		إذا (c->object.flags & غوره->flag_within) {
			إذا (!oidset_اندازه(&unflagged))
				اكسر;
		} وإلا {
			oidset_remove(&unflagged, &c->object.oid);
			غوره->ژوروالی++;
		}
		طالما (parents) {
			بینښه کتل_شوی, flag_before, flag_after;
			بنية commit *p = parents->توکی;
			repo_parse_commit(the_repository, p);
			کتل_شوی = p->object.flags & کتل_شوی_2;
			إذا (!کتل_شوی)
				prio_کتار_put(کتار, p);
			flag_before = p->object.flags & غوره->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & غوره->flag_within;
			إذا (!کتل_شوی && !flag_after)
				oidset_ورننباسه(&unflagged, &p->object.oid);
			إذا (کتل_شوی && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->بل;
		}
	}
	oidset_clear(&unflagged);
	أرجع کتل_شوی_commits;
}

ساكن فراغ append_name(بنية commit_name *n, بنية strbuf *dst)
{
	إذا (n->prio == 2 && !n->نښان) {
		n->نښان = lookup_نښان(the_repository, &n->oid);
		إذا (!n->نښان || parse_نښان(the_repository, n->نښان))
			die(_("annotated tag %s not available"), n->لار);
	}
	إذا (n->نښان && !n->name_checked) {
		إذا (strcmp(n->نښان->نښان, all ? n->لار + 5 : n->لار)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->لار, n->نښان->نښان);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	إذا (n->نښان) {
		إذا (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->نښان->نښان);
	} وإلا {
		strbuf_addstr(dst, n->لار);
	}
}

ساكن فراغ append_suffix(صحيح ژوروالی, ثابت بنية object_id *oid, بنية strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", ژوروالی,
		    repo_پیدا_کړه_unique_abbrev(the_repository, oid, abbrev));
}

ساكن فراغ describe_commit(بنية commit *cmit, بنية strbuf *dst)
{
	بنية commit *c, *gave_پورته_on = عدم;
	بنية prio_کتار کتار = { compare_commits_by_commit_date };
	بنية commit_name *n;
	بنية possible_نښان all_matches[MAX_نښانونه];
	بینښه صحيح سمون_cnt = 0, annotated_cnt = 0, cur_سمون;
	بینښه اوږد کتل_شوی_commits = 0;
	بینښه صحيح unannotated_cnt = 0;

	n = پیدا_کړه_commit_name(&cmit->object.oid);
	إذا (n && (نښانونه || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		إذا (n->misnamed || longformat)
			append_suffix(0, n->نښان ? get_tagged_oid(n->نښان) : &cmit->object.oid, dst);
		إذا (suffix)
			strbuf_addstr(dst, suffix);
		أرجع;
	}

	إذا (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_منزل_hex(&cmit->object.oid));
	إذا (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	إذا (!have_util) {
		بنية hashmap_iter iter;
		بنية commit *c;
		بنية commit_name *n;

		init_commit_نومونه(&commit_نومونه);
		hashmap_for_each_ثبت(&نومونه, &iter, n,
					ثبت /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			إذا (c)
				*commit_نومونه_at(&commit_نومونه, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = کتل_شوی_2;
	prio_کتار_put(&کتار, cmit);
	طالما ((c = prio_کتار_get(&کتار))) {
		بنية commit_list *parents = c->parents;
		بنية commit_name **خونه;

		کتل_شوی_commits++;

		إذا (سمون_cnt == max_candidates ||
		    سمون_cnt == hashmap_get_اندازه(&نومونه)) {
			gave_پورته_on = c;
			اكسر;
		}

		خونه = commit_نومونه_peek(&commit_نومونه, c);
		n = خونه ? *خونه : عدم;
		إذا (n) {
			إذا (!نښانونه && !all && n->prio < 2) {
				unannotated_cnt++;
			} وإلا إذا (سمون_cnt < max_candidates) {
				بنية possible_نښان *t = &all_matches[سمون_cnt++];
				t->name = n;
				t->ژوروالی = کتل_شوی_commits - 1;
				t->flag_within = 1u << سمون_cnt;
				t->وموندل_ترتیب = سمون_cnt;
				c->object.flags |= t->flag_within;
				إذا (n->prio == 2)
					annotated_cnt++;
			}
		}
		لكل (cur_سمون = 0; cur_سمون < سمون_cnt; cur_سمون++) {
			بنية possible_نښان *t = &all_matches[cur_سمون];
			إذا (!(c->object.flags & t->flag_within))
				t->ژوروالی++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		إذا (annotated_cnt && !prio_کتار_اندازه(&کتار)) {
			صحيح غوره_ژوروالی = لوی_عدد;
			بینښه غوره_within = 0;
			لكل (cur_سمون = 0; cur_سمون < سمون_cnt; cur_سمون++) {
				بنية possible_نښان *t = &all_matches[cur_سمون];
				إذا (t->ژوروالی < غوره_ژوروالی) {
					غوره_ژوروالی = t->ژوروالی;
					غوره_within = t->flag_within;
				} وإلا إذا (t->ژوروالی == غوره_ژوروالی) {
					غوره_within |= t->flag_within;
				}
			}
			إذا ((c->object.flags & غوره_within) == غوره_within) {
				إذا (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_منزل_hex(&c->object.oid));
				اكسر;
			}
		}
		طالما (parents) {
			بنية commit *p = parents->توکی;
			repo_parse_commit(the_repository, p);
			إذا (!(p->object.flags & کتل_شوی_2))
				prio_کتار_put(&کتار, p);
			p->object.flags |= c->object.flags;
			parents = parents->بل;

			إذا (لومړی_پلار)
				اكسر;
		}
	}

	إذا (!سمون_cnt) {
		بنية object_id *cmit_oid = &cmit->object.oid;
		إذا (always) {
			strbuf_جمع_کړه_unique_abbrev(dst, cmit_oid, abbrev);
			إذا (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_کتار(&کتار);
			أرجع;
		}
		إذا (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_منزل_hex(cmit_oid));
		وإلا
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_منزل_hex(cmit_oid));
	}

	QSORT(all_matches, سمون_cnt, compare_pt);

	إذا (gave_پورته_on) {
		prio_کتار_put(&کتار, gave_پورته_on);
		کتل_شوی_commits--;
	}
	کتل_شوی_commits += پای_ژوروالی_computation(&کتار, &all_matches[0]);
	clear_prio_کتار(&کتار);

	إذا (debug) {
		ساكن صحيح label_پلنوالی = -1;
		إذا (label_پلنوالی < 0) {
			صحيح i, w;
			لكل (i = 0; i < ARRAY_اندازه(prio_نومونه); i++) {
				w = مزي_اوږدوالی(_(prio_نومونه[i]));
				إذا (label_پلنوالی < w)
					label_پلنوالی = w;
			}
		}
		لكل (cur_سمون = 0; cur_سمون < سمون_cnt; cur_سمون++) {
			بنية possible_نښان *t = &all_matches[cur_سمون];
			fprintf(stderr, " %-*s %8d %s\n",
				label_پلنوالی, _(prio_نومونه[t->name->prio]),
				t->ژوروالی, t->name->لار);
		}
		fprintf(stderr, _("traversed %lu commits\n"), کتل_شوی_commits);
		إذا (gave_پورته_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_منزل_hex(&gave_پورته_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	إذا (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].ژوروالی, &cmit->object.oid, dst);
	إذا (suffix)
		strbuf_addstr(dst, suffix);
}

بنية process_commit_معلومات {
	بنية commit *current_commit;
	ثابت بنية object_id *looking_for;
	بنية strbuf *dst;
	بنية rev_info *revs;
};

ساكن فراغ process_commit(بنية commit *commit, فراغ *معلومات)
{
	بنية process_commit_معلومات *pcd = معلومات;
	pcd->current_commit = commit;
}

ساكن فراغ process_object(بنية object *obj, ثابت حرف *لار, فراغ *معلومات)
{
	بنية process_commit_معلومات *pcd = معلومات;

	إذا (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		إذا (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", لار);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = عدم;
	}
}

ساكن فراغ describe_blob(ثابت بنية object_id *oid, بنية strbuf *dst)
{
	بنية rev_info revs;
	بنية strvec دلیلونه = STRVEC_INIT;
	بنية object_id سر_oid;
	بنية process_commit_معلومات pcd = { عدم, oid, dst, &revs};

	إذا (repo_get_oid(the_repository, "HEAD", &سر_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_منزل_hex(oid));

	strvec_pushl(&دلیلونه, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_منزل_hex(&سر_oid),
		     عدم);

	repo_init_revisions(the_repository, &revs, عدم);
	تیاری_revisions_from_strvec(&دلیلونه, &revs, عدم);
	إذا (دلیلونه.nr > 1)
		BUG("setup_revisions could not handle all args?");

	إذا (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&دلیلونه);

	إذا (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_منزل_hex(oid));
}

ساكن فراغ describe(ثابت حرف *arg, صحيح وروستی_one)
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

	إذا (!وروستی_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

ساكن صحيح option_parse_exact_سمون(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	صحيح *val = opt->ارزښت;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	أرجع 0;
}

صحيح cmd_describe(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *مختاړی,
		 بنية repository *repo ناکارول_شوی )
{
	بنية refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	صحيح لري = 0;
	بنية option options[] = {
		OPT_BOOL(0, "contains",   &لري, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &نښانونه, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &لومړی_پلار, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, عدم,
			       N_("only output exact matches"),
			       PARSE_OPT_NOARG, option_parse_exact_سمون),
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
			.ارزښت = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.ارزښت = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_پای_ټکی(),
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, مختاړی, options, describe_usage, 0);
	إذا (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	إذا (max_candidates < 0)
		max_candidates = 0;
	وإلا إذا (max_candidates > MAX_نښانونه)
		max_candidates = MAX_نښانونه;

	save_commit_بفر = 0;

	إذا (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	إذا (لري) {
		بنية string_list_توکی *توکی;
		بنية strvec دلیلونه;
		ثابت حرف **argv_copy;
		صحيح ret;

		strvec_init(&دلیلونه);
		strvec_pushl(&دلیلونه, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     عدم);
		إذا (always)
			strvec_ټېل(&دلیلونه, "--always");
		إذا (!all)
			strvec_ټېل(&دلیلونه, "--tags");

		for_each_string_list_توکی(توکی, &patterns)
			strvec_pushf(&دلیلونه, "--refs=refs/tags/%s", توکی->string);
		for_each_string_list_توکی(توکی, &exclude_patterns)
			strvec_pushf(&دلیلونه, "--exclude=refs/tags/%s", توکی->string);

		إذا (all) {
			for_each_string_list_توکی(توکی, &patterns)
				strvec_pushf(&دلیلونه, "--refs=refs/heads/%s", توکی->string);
			for_each_string_list_توکی(توکی, &exclude_patterns)
				strvec_pushf(&دلیلونه, "--exclude=refs/heads/%s", توکی->string);
			for_each_string_list_توکی(توکی, &patterns)
				strvec_pushf(&دلیلونه, "--refs=refs/remotes/%s", توکی->string);
			for_each_string_list_توکی(توکی, &exclude_patterns)
				strvec_pushf(&دلیلونه, "--exclude=refs/remotes/%s", توکی->string);
		}

		إذا (argc)
			strvec_pushv(&دلیلونه, argv);
		وإلا
			strvec_ټېل(&دلیلونه, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, دلیلونه.nr + 1);
		لكل (اندازه_t i = 0; i < دلیلونه.nr; i++)
			argv_copy[i] = دلیلونه.v[i];
		argv_copy[دلیلونه.nr] = عدم;

		ret = cmd_name_rev(دلیلونه.nr, argv_copy, مختاړی, the_repository);

		strvec_clear(&دلیلونه);
		حرر(argv_copy);
		أرجع ret;
	}

	إذا (!all)
		for_each_ref_opts.مختاړی = "refs/tags/";

	hashmap_init(&نومونه, commit_name_neq, عدم, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, عدم, &for_each_ref_opts);
	إذا (!hashmap_get_اندازه(&نومونه) && !always)
		die(_("No names found, cannot describe anything."));

	إذا (argc == 0) {
		إذا (broken) {
			بنية ماشوم_process cp = ماشوم_PROCESS_INIT;

			strvec_pushv(&cp.دلیلونه, update_شاخص_دلیلونه);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			پرلپسې_command(&cp);

			ماشوم_process_init(&cp);
			strvec_pushv(&cp.دلیلونه, diff_شاخص_دلیلونه);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			إذا (!dirty)
				dirty = "-dirty";

			بدل (پرلپسې_command(&cp)) {
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
			بنية قلف_file شاخص_قلف = قلف_INIT;
			بنية rev_info revs;
			صحيح fd;

			تیاری_work_ونه(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_ډک_شاخص = 0;
			repo_read_شاخص(the_repository);
			refresh_شاخص(the_repository->شاخص, REFRESH_QUIET|REFRESH_UNMERGED,
				      عدم, عدم, عدم);
			fd = repo_hold_locked_شاخص(the_repository,
						    &شاخص_قلف, 0);
			إذا (0 <= fd)
				repo_update_شاخص_if_able(the_repository, &شاخص_قلف);

			repo_init_revisions(the_repository, &revs, مختاړی);

			إذا (تیاری_revisions(ARRAY_اندازه(diff_شاخص_دلیلونه) - 1,
					    diff_شاخص_دلیلونه, &revs, عدم) != 1)
				BUG("malformed internal diff-index command line");
			پرلپسې_diff_شاخص(&revs, 0);

			إذا (!diff_پایله_کوډ(&revs))
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
