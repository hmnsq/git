#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

#تعریف_کرو MAX_ٹیگ_سوچی	(FLAG_بٹ - 1)
#تعریف_کرو DEFAULT_CANDIDATES 10

define_commit_slab(commit_ناں, بنية commit_name *);

ساكن ثابت حرف * ثابت describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	عدم
};

ساكن صحيح debug;	/* Display lots of verbose info */
ساكن صحيح all;	/* Any valid ref can be used */
ساكن صحيح ٹیگ_سوچی;	/* Allow lightweight tags */
ساكن صحيح longformat;
ساكن صحيح پہلا_ماپا;
ساكن صحيح abbrev = -1; /* unspecified */
ساكن صحيح max_candidates = DEFAULT_CANDIDATES;
ساكن بنية hashmap ناں;
ساكن صحيح have_util;
ساكن بنية string_list patterns = STRING_LIST_INIT_NODUP;
ساكن بنية string_list exclude_patterns = STRING_LIST_INIT_NODUP;
ساكن صحيح always;
ساكن ثابت حرف *suffix, *dirty, *broken;
ساكن بنية commit_ناں commit_ناں;

/* diff-index command arguments to check if working tree is dirty. */
ساكن ثابت حرف *diff_اشاریہ_دلیلاں[] = {
	"diff-index", "--quiet", "HEAD", "--", عدم
};

ساكن ثابت حرف *update_اشاریہ_دلیلاں[] = {
	"update-index", "--unmerged", "-q", "--refresh", عدم
};

بنية commit_name {
	بنية hashmap_اندراج اندراج;
	بنية object_id peeled;
	بنية ٹیگ *ٹیگ;
	بنانشانی prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	بنانشانی name_checked:1;
	بنانشانی misnamed:1;
	بنية object_id oid;
	حرف *رستہ;
};

ساكن ثابت حرف *prio_ناں[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

ساكن صحيح commit_name_neq(ثابت فراغ *cmp_انکڑے بن_ورتیا,
			   ثابت بنية hashmap_اندراج *eptr,
			   ثابت بنية hashmap_اندراج *اندراج_or_کلید,
			   ثابت فراغ *peeled)
{
	ثابت بنية commit_name *cn1, *cn2;

	cn1 = container_دا(eptr, ثابت بنية commit_name, اندراج);
	cn2 = container_دا(اندراج_or_کلید, ثابت بنية commit_name, اندراج);

	أرجع !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

ساكن مضمن بنية commit_name *لبھو_commit_name(ثابت بنية object_id *peeled)
{
	أرجع hashmap_get_اندراج_from_hash(&ناں, oidhash(peeled), peeled,
						بنية commit_name, اندراج);
}

ساكن صحيح replace_name(بنية commit_name *e,
			       صحيح prio,
			       ثابت بنية object_id *oid,
			       بنية ٹیگ **ٹیگ)
{
	إذا (!e || e->prio < prio)
		أرجع 1;

	إذا (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		بنية ٹیگ *t;

		إذا (!e->ٹیگ) {
			t = lookup_ٹیگ(the_repository, &e->oid);
			إذا (!t || parse_ٹیگ(the_repository, t))
				أرجع 1;
			e->ٹیگ = t;
		}

		t = lookup_ٹیگ(the_repository, oid);
		إذا (!t || parse_ٹیگ(the_repository, t))
			أرجع 0;
		*ٹیگ = t;

		إذا (e->ٹیگ->date < t->date)
			أرجع 1;
	}

	أرجع 0;
}

ساكن فراغ جوڑو_منزل_معلوم_ناں(ثابت حرف *رستہ,
			       ثابت بنية object_id *peeled,
			       صحيح prio,
			       ثابت بنية object_id *oid)
{
	بنية commit_name *e = لبھو_commit_name(peeled);
	بنية ٹیگ *ٹیگ = عدم;
	إذا (replace_name(e, prio, oid, &ٹیگ)) {
		إذا (!e) {
			e = xmalloc(حجم(بنية commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_اندراج_init(&e->اندراج, oidhash(peeled));
			hashmap_جوڑو(&ناں, &e->اندراج);
			e->رستہ = عدم;
		}
		e->ٹیگ = ٹیگ;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		حرر(e->رستہ);
		e->رستہ = xstrdup(رستہ);
	}
}

ساكن صحيح get_name(ثابت بنية reference *ref, فراغ *cb_انکڑے بن_ورتیا)
{
	صحيح is_ٹیگ = 0;
	بنية object_id peeled;
	صحيح is_annotated, prio;
	ثابت حرف *رستہ_منزل_میل = عدم;

	إذا (skip_اگیتر(ref->name, "refs/tags/", &رستہ_منزل_میل)) {
		is_ٹیگ = 1;
	} وإلا إذا (all) {
		إذا ((exclude_patterns.nr || patterns.nr) &&
		    !skip_اگیتر(ref->name, "refs/heads/", &رستہ_منزل_میل) &&
		    !skip_اگیتر(ref->name, "refs/remotes/", &رستہ_منزل_میل)) {
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
		بنية string_list_شے *شے;

		for_each_string_list_شے(شے, &exclude_patterns) {
			إذا (!wildmatch(شے->string, رستہ_منزل_میل, 0))
				أرجع 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	إذا (patterns.nr) {
		صحيح لبھیا_2 = 0;
		بنية string_list_شے *شے;

		for_each_string_list_شے(شے, &patterns) {
			إذا (!wildmatch(شے->string, رستہ_منزل_میل, 0)) {
				لبھیا_2 = 1;
				اكسر;
			}
		}

		إذا (!لبھیا_2)
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
	وإلا إذا (is_ٹیگ)
		prio = 1;
	وإلا
		prio = 0;

	جوڑو_منزل_معلوم_ناں(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	أرجع 0;
}

بنية possible_ٹیگ {
	بنية commit_name *name;
	صحيح ڈونگھائی;
	صحيح لبھیا_ترتیب;
	بنانشانی flag_within;
};

ساكن صحيح compare_pt(ثابت فراغ *a_, ثابت فراغ *b_)
{
	بنية possible_ٹیگ *a = (بنية possible_ٹیگ *)a_;
	بنية possible_ٹیگ *b = (بنية possible_ٹیگ *)b_;
	إذا (a->ڈونگھائی != b->ڈونگھائی)
		أرجع a->ڈونگھائی - b->ڈونگھائی;
	إذا (a->لبھیا_ترتیب != b->لبھیا_ترتیب)
		أرجع a->لبھیا_ترتیب - b->لبھیا_ترتیب;
	أرجع 0;
}

ساكن بنانشانی لما انت_ڈونگھائی_computation(بنية prio_قطار *قطار,
					      بنية possible_ٹیگ *بہترین)
{
	بنانشانی لما تکیا_commits = 0;
	بنية oidset unflagged = OIDSET_INIT;
	بنية commit *c;

	prio_قطار_for_each(قطار, c) {
		إذا (!(c->object.flags & بہترین->flag_within))
			oidset_داخل_کرو(&unflagged, &c->object.oid);
	}

	طالما ((c = prio_قطار_get(قطار))) {
		بنية commit_list *parents = c->parents;
		تکیا_commits++;
		إذا (c->object.flags & بہترین->flag_within) {
			إذا (!oidset_ناپ(&unflagged))
				اكسر;
		} وإلا {
			oidset_remove(&unflagged, &c->object.oid);
			بہترین->ڈونگھائی++;
		}
		طالما (parents) {
			بنانشانی تکیا, flag_before, flag_after;
			بنية commit *p = parents->شے;
			repo_parse_commit(the_repository, p);
			تکیا = p->object.flags & تکیا_2;
			إذا (!تکیا)
				prio_قطار_put(قطار, p);
			flag_before = p->object.flags & بہترین->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & بہترین->flag_within;
			إذا (!تکیا && !flag_after)
				oidset_داخل_کرو(&unflagged, &p->object.oid);
			إذا (تکیا && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->اگلا;
		}
	}
	oidset_clear(&unflagged);
	أرجع تکیا_commits;
}

ساكن فراغ append_name(بنية commit_name *n, بنية strbuf *dst)
{
	إذا (n->prio == 2 && !n->ٹیگ) {
		n->ٹیگ = lookup_ٹیگ(the_repository, &n->oid);
		إذا (!n->ٹیگ || parse_ٹیگ(the_repository, n->ٹیگ))
			die(_("annotated tag %s not available"), n->رستہ);
	}
	إذا (n->ٹیگ && !n->name_checked) {
		إذا (strcmp(n->ٹیگ->ٹیگ, all ? n->رستہ + 5 : n->رستہ)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->رستہ, n->ٹیگ->ٹیگ);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	إذا (n->ٹیگ) {
		إذا (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->ٹیگ->ٹیگ);
	} وإلا {
		strbuf_addstr(dst, n->رستہ);
	}
}

ساكن فراغ append_suffix(صحيح ڈونگھائی, ثابت بنية object_id *oid, بنية strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", ڈونگھائی,
		    repo_لبھو_unique_abbrev(the_repository, oid, abbrev));
}

ساكن فراغ describe_commit(بنية commit *cmit, بنية strbuf *dst)
{
	بنية commit *c, *gave_اتے_on = عدم;
	بنية prio_قطار قطار = { compare_commits_by_commit_date };
	بنية commit_name *n;
	بنية possible_ٹیگ all_matches[MAX_ٹیگ_سوچی];
	بنانشانی صحيح میل_cnt = 0, annotated_cnt = 0, cur_میل;
	بنانشانی لما تکیا_commits = 0;
	بنانشانی صحيح unannotated_cnt = 0;

	n = لبھو_commit_name(&cmit->object.oid);
	إذا (n && (ٹیگ_سوچی || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		إذا (n->misnamed || longformat)
			append_suffix(0, n->ٹیگ ? get_tagged_oid(n->ٹیگ) : &cmit->object.oid, dst);
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

		init_commit_ناں(&commit_ناں);
		hashmap_for_each_اندراج(&ناں, &iter, n,
					اندراج /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			إذا (c)
				*commit_ناں_at(&commit_ناں, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = تکیا_2;
	prio_قطار_put(&قطار, cmit);
	طالما ((c = prio_قطار_get(&قطار))) {
		بنية commit_list *parents = c->parents;
		بنية commit_name **تھاں;

		تکیا_commits++;

		إذا (میل_cnt == max_candidates ||
		    میل_cnt == hashmap_get_ناپ(&ناں)) {
			gave_اتے_on = c;
			اكسر;
		}

		تھاں = commit_ناں_peek(&commit_ناں, c);
		n = تھاں ? *تھاں : عدم;
		إذا (n) {
			إذا (!ٹیگ_سوچی && !all && n->prio < 2) {
				unannotated_cnt++;
			} وإلا إذا (میل_cnt < max_candidates) {
				بنية possible_ٹیگ *t = &all_matches[میل_cnt++];
				t->name = n;
				t->ڈونگھائی = تکیا_commits - 1;
				t->flag_within = 1u << میل_cnt;
				t->لبھیا_ترتیب = میل_cnt;
				c->object.flags |= t->flag_within;
				إذا (n->prio == 2)
					annotated_cnt++;
			}
		}
		لكل (cur_میل = 0; cur_میل < میل_cnt; cur_میل++) {
			بنية possible_ٹیگ *t = &all_matches[cur_میل];
			إذا (!(c->object.flags & t->flag_within))
				t->ڈونگھائی++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		إذا (annotated_cnt && !prio_قطار_ناپ(&قطار)) {
			صحيح بہترین_ڈونگھائی = ودھ_توں_ودھ_عدد;
			بنانشانی بہترین_within = 0;
			لكل (cur_میل = 0; cur_میل < میل_cnt; cur_میل++) {
				بنية possible_ٹیگ *t = &all_matches[cur_میل];
				إذا (t->ڈونگھائی < بہترین_ڈونگھائی) {
					بہترین_ڈونگھائی = t->ڈونگھائی;
					بہترین_within = t->flag_within;
				} وإلا إذا (t->ڈونگھائی == بہترین_ڈونگھائی) {
					بہترین_within |= t->flag_within;
				}
			}
			إذا ((c->object.flags & بہترین_within) == بہترین_within) {
				إذا (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_منزل_hex(&c->object.oid));
				اكسر;
			}
		}
		طالما (parents) {
			بنية commit *p = parents->شے;
			repo_parse_commit(the_repository, p);
			إذا (!(p->object.flags & تکیا_2))
				prio_قطار_put(&قطار, p);
			p->object.flags |= c->object.flags;
			parents = parents->اگلا;

			إذا (پہلا_ماپا)
				اكسر;
		}
	}

	إذا (!میل_cnt) {
		بنية object_id *cmit_oid = &cmit->object.oid;
		إذا (always) {
			strbuf_جوڑو_unique_abbrev(dst, cmit_oid, abbrev);
			إذا (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_قطار(&قطار);
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

	QSORT(all_matches, میل_cnt, compare_pt);

	إذا (gave_اتے_on) {
		prio_قطار_put(&قطار, gave_اتے_on);
		تکیا_commits--;
	}
	تکیا_commits += انت_ڈونگھائی_computation(&قطار, &all_matches[0]);
	clear_prio_قطار(&قطار);

	إذا (debug) {
		ساكن صحيح label_چوڑائی = -1;
		إذا (label_چوڑائی < 0) {
			صحيح i, w;
			لكل (i = 0; i < ARRAY_ناپ(prio_ناں); i++) {
				w = لڑی_لمبائی(_(prio_ناں[i]));
				إذا (label_چوڑائی < w)
					label_چوڑائی = w;
			}
		}
		لكل (cur_میل = 0; cur_میل < میل_cnt; cur_میل++) {
			بنية possible_ٹیگ *t = &all_matches[cur_میل];
			fprintf(stderr, " %-*s %8d %s\n",
				label_چوڑائی, _(prio_ناں[t->name->prio]),
				t->ڈونگھائی, t->name->رستہ);
		}
		fprintf(stderr, _("traversed %lu commits\n"), تکیا_commits);
		إذا (gave_اتے_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_منزل_hex(&gave_اتے_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	إذا (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].ڈونگھائی, &cmit->object.oid, dst);
	إذا (suffix)
		strbuf_addstr(dst, suffix);
}

بنية process_commit_انکڑے {
	بنية commit *current_commit;
	ثابت بنية object_id *looking_for;
	بنية strbuf *dst;
	بنية rev_info *revs;
};

ساكن فراغ process_commit(بنية commit *commit, فراغ *انکڑے)
{
	بنية process_commit_انکڑے *pcd = انکڑے;
	pcd->current_commit = commit;
}

ساكن فراغ process_object(بنية object *obj, ثابت حرف *رستہ, فراغ *انکڑے)
{
	بنية process_commit_انکڑے *pcd = انکڑے;

	إذا (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		إذا (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", رستہ);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = عدم;
	}
}

ساكن فراغ describe_blob(ثابت بنية object_id *oid, بنية strbuf *dst)
{
	بنية rev_info revs;
	بنية strvec دلیلاں = STRVEC_INIT;
	بنية object_id سر_oid;
	بنية process_commit_انکڑے pcd = { عدم, oid, dst, &revs};

	إذا (repo_get_oid(the_repository, "HEAD", &سر_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_منزل_hex(oid));

	strvec_pushl(&دلیلاں, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_منزل_hex(&سر_oid),
		     عدم);

	repo_init_revisions(the_repository, &revs, عدم);
	تیاری_revisions_from_strvec(&دلیلاں, &revs, عدم);
	إذا (دلیلاں.nr > 1)
		BUG("setup_revisions could not handle all args?");

	إذا (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&دلیلاں);

	إذا (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_منزل_hex(oid));
}

ساكن فراغ describe(ثابت حرف *arg, صحيح آخری_one)
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

	إذا (!آخری_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

ساكن صحيح option_parse_exact_میل(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	صحيح *val = opt->قدر;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	أرجع 0;
}

صحيح cmd_describe(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *اگیتر,
		 بنية repository *repo بن_ورتیا )
{
	بنية refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	صحيح شامل_اے = 0;
	بنية option options[] = {
		OPT_BOOL(0, "contains",   &شامل_اے, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &ٹیگ_سوچی, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &پہلا_ماپا, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, عدم,
			       N_("only output exact matches"),
			       PARSE_OPT_NOARG, option_parse_exact_میل),
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
			.قدر = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.قدر = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_سرا(),
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, اگیتر, options, describe_usage, 0);
	إذا (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	إذا (max_candidates < 0)
		max_candidates = 0;
	وإلا إذا (max_candidates > MAX_ٹیگ_سوچی)
		max_candidates = MAX_ٹیگ_سوچی;

	save_commit_بفر = 0;

	إذا (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	إذا (شامل_اے) {
		بنية string_list_شے *شے;
		بنية strvec دلیلاں;
		ثابت حرف **argv_copy;
		صحيح ret;

		strvec_init(&دلیلاں);
		strvec_pushl(&دلیلاں, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     عدم);
		إذا (always)
			strvec_ڈالو(&دلیلاں, "--always");
		إذا (!all)
			strvec_ڈالو(&دلیلاں, "--tags");

		for_each_string_list_شے(شے, &patterns)
			strvec_pushf(&دلیلاں, "--refs=refs/tags/%s", شے->string);
		for_each_string_list_شے(شے, &exclude_patterns)
			strvec_pushf(&دلیلاں, "--exclude=refs/tags/%s", شے->string);

		إذا (all) {
			for_each_string_list_شے(شے, &patterns)
				strvec_pushf(&دلیلاں, "--refs=refs/heads/%s", شے->string);
			for_each_string_list_شے(شے, &exclude_patterns)
				strvec_pushf(&دلیلاں, "--exclude=refs/heads/%s", شے->string);
			for_each_string_list_شے(شے, &patterns)
				strvec_pushf(&دلیلاں, "--refs=refs/remotes/%s", شے->string);
			for_each_string_list_شے(شے, &exclude_patterns)
				strvec_pushf(&دلیلاں, "--exclude=refs/remotes/%s", شے->string);
		}

		إذا (argc)
			strvec_pushv(&دلیلاں, argv);
		وإلا
			strvec_ڈالو(&دلیلاں, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, دلیلاں.nr + 1);
		لكل (ناپ_t i = 0; i < دلیلاں.nr; i++)
			argv_copy[i] = دلیلاں.v[i];
		argv_copy[دلیلاں.nr] = عدم;

		ret = cmd_name_rev(دلیلاں.nr, argv_copy, اگیتر, the_repository);

		strvec_clear(&دلیلاں);
		حرر(argv_copy);
		أرجع ret;
	}

	إذا (!all)
		for_each_ref_opts.اگیتر = "refs/tags/";

	hashmap_init(&ناں, commit_name_neq, عدم, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, عدم, &for_each_ref_opts);
	إذا (!hashmap_get_ناپ(&ناں) && !always)
		die(_("No names found, cannot describe anything."));

	إذا (argc == 0) {
		إذا (broken) {
			بنية بچہ_process cp = بچہ_PROCESS_INIT;

			strvec_pushv(&cp.دلیلاں, update_اشاریہ_دلیلاں);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			لگاتار_command(&cp);

			بچہ_process_init(&cp);
			strvec_pushv(&cp.دلیلاں, diff_اشاریہ_دلیلاں);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			إذا (!dirty)
				dirty = "-dirty";

			بدل (لگاتار_command(&cp)) {
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
			بنية جندرا_file اشاریہ_جندرا = جندرا_INIT;
			بنية rev_info revs;
			صحيح fd;

			تیاری_work_درخت(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_بھریا_اشاریہ = 0;
			repo_read_اشاریہ(the_repository);
			refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET|REFRESH_UNMERGED,
				      عدم, عدم, عدم);
			fd = repo_hold_locked_اشاریہ(the_repository,
						    &اشاریہ_جندرا, 0);
			إذا (0 <= fd)
				repo_update_اشاریہ_if_able(the_repository, &اشاریہ_جندرا);

			repo_init_revisions(the_repository, &revs, اگیتر);

			إذا (تیاری_revisions(ARRAY_ناپ(diff_اشاریہ_دلیلاں) - 1,
					    diff_اشاریہ_دلیلاں, &revs, عدم) != 1)
				BUG("malformed internal diff-index command line");
			لگاتار_diff_اشاریہ(&revs, 0);

			إذا (!diff_نتیجہ_کوڈ(&revs))
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
