#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "hashmap.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "list-objects.h"
#అన్తర్భావ "commit-slab.h"
#అన్తర్భావ "wildmatch.h"
#అన్తర్భావ "prio-queue.h"
#అన్తర్భావ "oidset.h"

#నిర్వచించు MAX_ట్యాగులు	(FLAG_బిట్లు - 1)
#నిర్వచించు DEFAULT_CANDIDATES 10

define_commit_slab(commit_పేర్లు, నిర్మాణం commit_name *);

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	ఖాళీ
};

స్థిర పూర్ణాంకం debug;	/* Display lots of verbose info */
స్థిర పూర్ణాంకం all;	/* Any valid ref can be used */
స్థిర పూర్ణాంకం ట్యాగులు;	/* Allow lightweight tags */
స్థిర పూర్ణాంకం longformat;
స్థిర పూర్ణాంకం మొదటి_మాతృ;
స్థిర పూర్ణాంకం abbrev = -1; /* unspecified */
స్థిర పూర్ణాంకం max_candidates = DEFAULT_CANDIDATES;
స్థిర నిర్మాణం hashmap పేర్లు;
స్థిర పూర్ణాంకం have_util;
స్థిర నిర్మాణం string_list patterns = STRING_LIST_INIT_NODUP;
స్థిర నిర్మాణం string_list exclude_patterns = STRING_LIST_INIT_NODUP;
స్థిర పూర్ణాంకం always;
స్థిర స్థిరరాశి అక్షరం *suffix, *dirty, *broken;
స్థిర నిర్మాణం commit_పేర్లు commit_పేర్లు;

/* diff-index command arguments to check if working tree is dirty. */
స్థిర స్థిరరాశి అక్షరం *diff_సూచీ_వాదనలు[] = {
	"diff-index", "--quiet", "HEAD", "--", ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *update_సూచీ_వాదనలు[] = {
	"update-index", "--unmerged", "-q", "--refresh", ఖాళీ
};

నిర్మాణం commit_name {
	నిర్మాణం hashmap_నమోదు నమోదు_2;
	నిర్మాణం object_id peeled;
	నిర్మాణం ట్యాగ్ *ట్యాగ్;
	గుర్తులేని prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	గుర్తులేని name_checked:1;
	గుర్తులేని misnamed:1;
	నిర్మాణం object_id oid;
	అక్షరం *మార్గం;
};

స్థిర స్థిరరాశి అక్షరం *prio_పేర్లు[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

స్థిర పూర్ణాంకం commit_name_neq(స్థిరరాశి శూన్య *cmp_దత్తాంశం వాడని,
			   స్థిరరాశి నిర్మాణం hashmap_నమోదు *eptr,
			   స్థిరరాశి నిర్మాణం hashmap_నమోదు *నమోదు_or_కీలకం,
			   స్థిరరాశి శూన్య *peeled)
{
	స్థిరరాశి నిర్మాణం commit_name *cn1, *cn2;

	cn1 = container_యొక్క(eptr, స్థిరరాశి నిర్మాణం commit_name, నమోదు_2);
	cn2 = container_యొక్క(నమోదు_or_కీలకం, స్థిరరాశి నిర్మాణం commit_name, నమోదు_2);

	తిరిగి !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

స్థిర అంతఃవరుస నిర్మాణం commit_name *కనుగొను_commit_name(స్థిరరాశి నిర్మాణం object_id *peeled)
{
	తిరిగి hashmap_get_నమోదు_from_hash(&పేర్లు, oidhash(peeled), peeled,
						నిర్మాణం commit_name, నమోదు_2);
}

స్థిర పూర్ణాంకం replace_name(నిర్మాణం commit_name *e,
			       పూర్ణాంకం prio,
			       స్థిరరాశి నిర్మాణం object_id *oid,
			       నిర్మాణం ట్యాగ్ **ట్యాగ్)
{
	ఒకవేళ (!e || e->prio < prio)
		తిరిగి 1;

	ఒకవేళ (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		నిర్మాణం ట్యాగ్ *t;

		ఒకవేళ (!e->ట్యాగ్) {
			t = lookup_ట్యాగ్(the_repository, &e->oid);
			ఒకవేళ (!t || parse_ట్యాగ్(the_repository, t))
				తిరిగి 1;
			e->ట్యాగ్ = t;
		}

		t = lookup_ట్యాగ్(the_repository, oid);
		ఒకవేళ (!t || parse_ట్యాగ్(the_repository, t))
			తిరిగి 0;
		*ట్యాగ్ = t;

		ఒకవేళ (e->ట్యాగ్->date < t->date)
			తిరిగి 1;
	}

	తిరిగి 0;
}

స్థిర శూన్య కూడు_గమ్యం_తెలిసిన_పేర్లు(స్థిరరాశి అక్షరం *మార్గం,
			       స్థిరరాశి నిర్మాణం object_id *peeled,
			       పూర్ణాంకం prio,
			       స్థిరరాశి నిర్మాణం object_id *oid)
{
	నిర్మాణం commit_name *e = కనుగొను_commit_name(peeled);
	నిర్మాణం ట్యాగ్ *ట్యాగ్ = ఖాళీ;
	ఒకవేళ (replace_name(e, prio, oid, &ట్యాగ్)) {
		ఒకవేళ (!e) {
			e = xmalloc(పరిమాణం(నిర్మాణం commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_నమోదు_init(&e->నమోదు_2, oidhash(peeled));
			hashmap_కూడు(&పేర్లు, &e->నమోదు_2);
			e->మార్గం = ఖాళీ;
		}
		e->ట్యాగ్ = ట్యాగ్;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		విడుదల(e->మార్గం);
		e->మార్గం = xstrdup(మార్గం);
	}
}

స్థిర పూర్ణాంకం get_name(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం వాడని)
{
	పూర్ణాంకం is_ట్యాగ్ = 0;
	నిర్మాణం object_id peeled;
	పూర్ణాంకం is_annotated, prio;
	స్థిరరాశి అక్షరం *మార్గం_గమ్యం_సరిపోలిక = ఖాళీ;

	ఒకవేళ (skip_ఉపసర్గ(ref->name, "refs/tags/", &మార్గం_గమ్యం_సరిపోలిక)) {
		is_ట్యాగ్ = 1;
	} లేకపోతే ఒకవేళ (all) {
		ఒకవేళ ((exclude_patterns.nr || patterns.nr) &&
		    !skip_ఉపసర్గ(ref->name, "refs/heads/", &మార్గం_గమ్యం_సరిపోలిక) &&
		    !skip_ఉపసర్గ(ref->name, "refs/remotes/", &మార్గం_గమ్యం_సరిపోలిక)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			తిరిగి 0;
		}
	} లేకపోతే {
		/* Reject anything outside refs/tags/ unless --all */
		తిరిగి 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	ఒకవేళ (exclude_patterns.nr) {
		నిర్మాణం string_list_అంశం *అంశం;

		for_each_string_list_అంశం(అంశం, &exclude_patterns) {
			ఒకవేళ (!wildmatch(అంశం->string, మార్గం_గమ్యం_సరిపోలిక, 0))
				తిరిగి 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	ఒకవేళ (patterns.nr) {
		పూర్ణాంకం దొరికింది_2 = 0;
		నిర్మాణం string_list_అంశం *అంశం;

		for_each_string_list_అంశం(అంశం, &patterns) {
			ఒకవేళ (!wildmatch(అంశం->string, మార్గం_గమ్యం_సరిపోలిక, 0)) {
				దొరికింది_2 = 1;
				ఆపు;
			}
		}

		ఒకవేళ (!దొరికింది_2)
			తిరిగి 0;
	}

	/* Is it annotated? */
	ఒకవేళ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} లేకపోతే {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	ఒకవేళ (is_annotated)
		prio = 2;
	లేకపోతే ఒకవేళ (is_ట్యాగ్)
		prio = 1;
	లేకపోతే
		prio = 0;

	కూడు_గమ్యం_తెలిసిన_పేర్లు(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	తిరిగి 0;
}

నిర్మాణం possible_ట్యాగ్ {
	నిర్మాణం commit_name *name;
	పూర్ణాంకం లోతు;
	పూర్ణాంకం దొరికింది_క్రమం;
	గుర్తులేని flag_within;
};

స్థిర పూర్ణాంకం compare_pt(స్థిరరాశి శూన్య *a_, స్థిరరాశి శూన్య *b_)
{
	నిర్మాణం possible_ట్యాగ్ *a = (నిర్మాణం possible_ట్యాగ్ *)a_;
	నిర్మాణం possible_ట్యాగ్ *b = (నిర్మాణం possible_ట్యాగ్ *)b_;
	ఒకవేళ (a->లోతు != b->లోతు)
		తిరిగి a->లోతు - b->లోతు;
	ఒకవేళ (a->దొరికింది_క్రమం != b->దొరికింది_క్రమం)
		తిరిగి a->దొరికింది_క్రమం - b->దొరికింది_క్రమం;
	తిరిగి 0;
}

స్థిర గుర్తులేని దీర్ఘ ముగింపు_లోతు_computation(నిర్మాణం prio_వరుస *వరుస,
					      నిర్మాణం possible_ట్యాగ్ *ఉత్తమ)
{
	గుర్తులేని దీర్ఘ చూసిన_commits = 0;
	నిర్మాణం oidset unflagged = OIDSET_INIT;
	నిర్మాణం commit *c;

	prio_వరుస_for_each(వరుస, c) {
		ఒకవేళ (!(c->object.flags & ఉత్తమ->flag_within))
			oidset_చొప్పించు(&unflagged, &c->object.oid);
	}

	అయితే ((c = prio_వరుస_get(వరుస))) {
		నిర్మాణం commit_list *parents = c->parents;
		చూసిన_commits++;
		ఒకవేళ (c->object.flags & ఉత్తమ->flag_within) {
			ఒకవేళ (!oidset_కొలత(&unflagged))
				ఆపు;
		} లేకపోతే {
			oidset_remove(&unflagged, &c->object.oid);
			ఉత్తమ->లోతు++;
		}
		అయితే (parents) {
			గుర్తులేని చూసిన, flag_before, flag_after;
			నిర్మాణం commit *p = parents->అంశం;
			repo_parse_commit(the_repository, p);
			చూసిన = p->object.flags & చూసిన_2;
			ఒకవేళ (!చూసిన)
				prio_వరుస_put(వరుస, p);
			flag_before = p->object.flags & ఉత్తమ->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & ఉత్తమ->flag_within;
			ఒకవేళ (!చూసిన && !flag_after)
				oidset_చొప్పించు(&unflagged, &p->object.oid);
			ఒకవేళ (చూసిన && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->తదుపరి;
		}
	}
	oidset_clear(&unflagged);
	తిరిగి చూసిన_commits;
}

స్థిర శూన్య append_name(నిర్మాణం commit_name *n, నిర్మాణం strbuf *dst)
{
	ఒకవేళ (n->prio == 2 && !n->ట్యాగ్) {
		n->ట్యాగ్ = lookup_ట్యాగ్(the_repository, &n->oid);
		ఒకవేళ (!n->ట్యాగ్ || parse_ట్యాగ్(the_repository, n->ట్యాగ్))
			die(_("annotated tag %s not available"), n->మార్గం);
	}
	ఒకవేళ (n->ట్యాగ్ && !n->name_checked) {
		ఒకవేళ (strcmp(n->ట్యాగ్->ట్యాగ్, all ? n->మార్గం + 5 : n->మార్గం)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->మార్గం, n->ట్యాగ్->ట్యాగ్);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	ఒకవేళ (n->ట్యాగ్) {
		ఒకవేళ (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->ట్యాగ్->ట్యాగ్);
	} లేకపోతే {
		strbuf_addstr(dst, n->మార్గం);
	}
}

స్థిర శూన్య append_suffix(పూర్ణాంకం లోతు, స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", లోతు,
		    repo_కనుగొను_unique_abbrev(the_repository, oid, abbrev));
}

స్థిర శూన్య describe_commit(నిర్మాణం commit *cmit, నిర్మాణం strbuf *dst)
{
	నిర్మాణం commit *c, *gave_పైకి_on = ఖాళీ;
	నిర్మాణం prio_వరుస వరుస = { compare_commits_by_commit_date };
	నిర్మాణం commit_name *n;
	నిర్మాణం possible_ట్యాగ్ all_matches[MAX_ట్యాగులు];
	గుర్తులేని పూర్ణాంకం సరిపోలిక_cnt = 0, annotated_cnt = 0, cur_సరిపోలిక;
	గుర్తులేని దీర్ఘ చూసిన_commits = 0;
	గుర్తులేని పూర్ణాంకం unannotated_cnt = 0;

	n = కనుగొను_commit_name(&cmit->object.oid);
	ఒకవేళ (n && (ట్యాగులు || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		ఒకవేళ (n->misnamed || longformat)
			append_suffix(0, n->ట్యాగ్ ? get_tagged_oid(n->ట్యాగ్) : &cmit->object.oid, dst);
		ఒకవేళ (suffix)
			strbuf_addstr(dst, suffix);
		తిరిగి;
	}

	ఒకవేళ (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_గమ్యం_hex(&cmit->object.oid));
	ఒకవేళ (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	ఒకవేళ (!have_util) {
		నిర్మాణం hashmap_iter iter;
		నిర్మాణం commit *c;
		నిర్మాణం commit_name *n;

		init_commit_పేర్లు(&commit_పేర్లు);
		hashmap_for_each_నమోదు(&పేర్లు, &iter, n,
					నమోదు_2 /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			ఒకవేళ (c)
				*commit_పేర్లు_at(&commit_పేర్లు, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = చూసిన_2;
	prio_వరుస_put(&వరుస, cmit);
	అయితే ((c = prio_వరుస_get(&వరుస))) {
		నిర్మాణం commit_list *parents = c->parents;
		నిర్మాణం commit_name **స్థానం;

		చూసిన_commits++;

		ఒకవేళ (సరిపోలిక_cnt == max_candidates ||
		    సరిపోలిక_cnt == hashmap_get_కొలత(&పేర్లు)) {
			gave_పైకి_on = c;
			ఆపు;
		}

		స్థానం = commit_పేర్లు_peek(&commit_పేర్లు, c);
		n = స్థానం ? *స్థానం : ఖాళీ;
		ఒకవేళ (n) {
			ఒకవేళ (!ట్యాగులు && !all && n->prio < 2) {
				unannotated_cnt++;
			} లేకపోతే ఒకవేళ (సరిపోలిక_cnt < max_candidates) {
				నిర్మాణం possible_ట్యాగ్ *t = &all_matches[సరిపోలిక_cnt++];
				t->name = n;
				t->లోతు = చూసిన_commits - 1;
				t->flag_within = 1u << సరిపోలిక_cnt;
				t->దొరికింది_క్రమం = సరిపోలిక_cnt;
				c->object.flags |= t->flag_within;
				ఒకవేళ (n->prio == 2)
					annotated_cnt++;
			}
		}
		కొరకు (cur_సరిపోలిక = 0; cur_సరిపోలిక < సరిపోలిక_cnt; cur_సరిపోలిక++) {
			నిర్మాణం possible_ట్యాగ్ *t = &all_matches[cur_సరిపోలిక];
			ఒకవేళ (!(c->object.flags & t->flag_within))
				t->లోతు++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		ఒకవేళ (annotated_cnt && !prio_వరుస_కొలత(&వరుస)) {
			పూర్ణాంకం ఉత్తమ_లోతు = గరిష్ఠ_పూర్ణాంకం;
			గుర్తులేని ఉత్తమ_within = 0;
			కొరకు (cur_సరిపోలిక = 0; cur_సరిపోలిక < సరిపోలిక_cnt; cur_సరిపోలిక++) {
				నిర్మాణం possible_ట్యాగ్ *t = &all_matches[cur_సరిపోలిక];
				ఒకవేళ (t->లోతు < ఉత్తమ_లోతు) {
					ఉత్తమ_లోతు = t->లోతు;
					ఉత్తమ_within = t->flag_within;
				} లేకపోతే ఒకవేళ (t->లోతు == ఉత్తమ_లోతు) {
					ఉత్తమ_within |= t->flag_within;
				}
			}
			ఒకవేళ ((c->object.flags & ఉత్తమ_within) == ఉత్తమ_within) {
				ఒకవేళ (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_గమ్యం_hex(&c->object.oid));
				ఆపు;
			}
		}
		అయితే (parents) {
			నిర్మాణం commit *p = parents->అంశం;
			repo_parse_commit(the_repository, p);
			ఒకవేళ (!(p->object.flags & చూసిన_2))
				prio_వరుస_put(&వరుస, p);
			p->object.flags |= c->object.flags;
			parents = parents->తదుపరి;

			ఒకవేళ (మొదటి_మాతృ)
				ఆపు;
		}
	}

	ఒకవేళ (!సరిపోలిక_cnt) {
		నిర్మాణం object_id *cmit_oid = &cmit->object.oid;
		ఒకవేళ (always) {
			strbuf_కూడు_unique_abbrev(dst, cmit_oid, abbrev);
			ఒకవేళ (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_వరుస(&వరుస);
			తిరిగి;
		}
		ఒకవేళ (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_గమ్యం_hex(cmit_oid));
		లేకపోతే
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_గమ్యం_hex(cmit_oid));
	}

	QSORT(all_matches, సరిపోలిక_cnt, compare_pt);

	ఒకవేళ (gave_పైకి_on) {
		prio_వరుస_put(&వరుస, gave_పైకి_on);
		చూసిన_commits--;
	}
	చూసిన_commits += ముగింపు_లోతు_computation(&వరుస, &all_matches[0]);
	clear_prio_వరుస(&వరుస);

	ఒకవేళ (debug) {
		స్థిర పూర్ణాంకం label_వెడల్పు = -1;
		ఒకవేళ (label_వెడల్పు < 0) {
			పూర్ణాంకం i, w;
			కొరకు (i = 0; i < ARRAY_కొలత(prio_పేర్లు); i++) {
				w = అక్షరశ్రేణి_పొడవు(_(prio_పేర్లు[i]));
				ఒకవేళ (label_వెడల్పు < w)
					label_వెడల్పు = w;
			}
		}
		కొరకు (cur_సరిపోలిక = 0; cur_సరిపోలిక < సరిపోలిక_cnt; cur_సరిపోలిక++) {
			నిర్మాణం possible_ట్యాగ్ *t = &all_matches[cur_సరిపోలిక];
			fprintf(stderr, " %-*s %8d %s\n",
				label_వెడల్పు, _(prio_పేర్లు[t->name->prio]),
				t->లోతు, t->name->మార్గం);
		}
		fprintf(stderr, _("traversed %lu commits\n"), చూసిన_commits);
		ఒకవేళ (gave_పైకి_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_గమ్యం_hex(&gave_పైకి_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	ఒకవేళ (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].లోతు, &cmit->object.oid, dst);
	ఒకవేళ (suffix)
		strbuf_addstr(dst, suffix);
}

నిర్మాణం process_commit_దత్తాంశం {
	నిర్మాణం commit *current_commit;
	స్థిరరాశి నిర్మాణం object_id *looking_for;
	నిర్మాణం strbuf *dst;
	నిర్మాణం rev_info *revs;
};

స్థిర శూన్య process_commit(నిర్మాణం commit *commit, శూన్య *దత్తాంశం)
{
	నిర్మాణం process_commit_దత్తాంశం *pcd = దత్తాంశం;
	pcd->current_commit = commit;
}

స్థిర శూన్య process_object(నిర్మాణం object *obj, స్థిరరాశి అక్షరం *మార్గం, శూన్య *దత్తాంశం)
{
	నిర్మాణం process_commit_దత్తాంశం *pcd = దత్తాంశం;

	ఒకవేళ (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		ఒకవేళ (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", మార్గం);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = ఖాళీ;
	}
}

స్థిర శూన్య describe_blob(స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం strbuf *dst)
{
	నిర్మాణం rev_info revs;
	నిర్మాణం strvec వాదనలు = STRVEC_INIT;
	నిర్మాణం object_id తల_oid;
	నిర్మాణం process_commit_దత్తాంశం pcd = { ఖాళీ, oid, dst, &revs};

	ఒకవేళ (repo_get_oid(the_repository, "HEAD", &తల_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_గమ్యం_hex(oid));

	strvec_pushl(&వాదనలు, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_గమ్యం_hex(&తల_oid),
		     ఖాళీ);

	repo_init_revisions(the_repository, &revs, ఖాళీ);
	అమరిక_revisions_from_strvec(&వాదనలు, &revs, ఖాళీ);
	ఒకవేళ (వాదనలు.nr > 1)
		BUG("setup_revisions could not handle all args?");

	ఒకవేళ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&వాదనలు);

	ఒకవేళ (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_గమ్యం_hex(oid));
}

స్థిర శూన్య describe(స్థిరరాశి అక్షరం *arg, పూర్ణాంకం చివరి_one)
{
	నిర్మాణం object_id oid;
	నిర్మాణం commit *cmit;
	నిర్మాణం strbuf sb = STRBUF_INIT;

	ఒకవేళ (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	ఒకవేళ (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	ఒకవేళ (cmit)
		describe_commit(cmit, &sb);
	లేకపోతే ఒకవేళ (odb_read_object_info(the_repository->objects,
				      &oid, ఖాళీ) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	లేకపోతే
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	ఒకవేళ (!చివరి_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

స్థిర పూర్ణాంకం option_parse_exact_సరిపోలిక(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
				    పూర్ణాంకం unset)
{
	పూర్ణాంకం *val = opt->విలువ;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	తిరిగి 0;
}

పూర్ణాంకం cmd_describe(పూర్ణాంకం argc,
		 స్థిరరాశి అక్షరం **argv,
		 స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo వాడని )
{
	నిర్మాణం refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	పూర్ణాంకం కలిగియుంది = 0;
	నిర్మాణం option options[] = {
		OPT_BOOL(0, "contains",   &కలిగియుంది, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &ట్యాగులు, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &మొదటి_మాతృ, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, ఖాళీ,
			       N_("only output exact matches"),
			       PARSE_OPT_NOARG, option_parse_exact_సరిపోలిక),
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
			.విలువ = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.విలువ = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_చివర(),
	};

	repo_config(the_repository, git_default_config, ఖాళీ);
	argc = parse_options(argc, argv, ఉపసర్గ, options, describe_usage, 0);
	ఒకవేళ (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	ఒకవేళ (max_candidates < 0)
		max_candidates = 0;
	లేకపోతే ఒకవేళ (max_candidates > MAX_ట్యాగులు)
		max_candidates = MAX_ట్యాగులు;

	save_commit_బఫరు = 0;

	ఒకవేళ (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	ఒకవేళ (కలిగియుంది) {
		నిర్మాణం string_list_అంశం *అంశం;
		నిర్మాణం strvec వాదనలు;
		స్థిరరాశి అక్షరం **argv_copy;
		పూర్ణాంకం ret;

		strvec_init(&వాదనలు);
		strvec_pushl(&వాదనలు, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     ఖాళీ);
		ఒకవేళ (always)
			strvec_నెట్టు(&వాదనలు, "--always");
		ఒకవేళ (!all)
			strvec_నెట్టు(&వాదనలు, "--tags");

		for_each_string_list_అంశం(అంశం, &patterns)
			strvec_pushf(&వాదనలు, "--refs=refs/tags/%s", అంశం->string);
		for_each_string_list_అంశం(అంశం, &exclude_patterns)
			strvec_pushf(&వాదనలు, "--exclude=refs/tags/%s", అంశం->string);

		ఒకవేళ (all) {
			for_each_string_list_అంశం(అంశం, &patterns)
				strvec_pushf(&వాదనలు, "--refs=refs/heads/%s", అంశం->string);
			for_each_string_list_అంశం(అంశం, &exclude_patterns)
				strvec_pushf(&వాదనలు, "--exclude=refs/heads/%s", అంశం->string);
			for_each_string_list_అంశం(అంశం, &patterns)
				strvec_pushf(&వాదనలు, "--refs=refs/remotes/%s", అంశం->string);
			for_each_string_list_అంశం(అంశం, &exclude_patterns)
				strvec_pushf(&వాదనలు, "--exclude=refs/remotes/%s", అంశం->string);
		}

		ఒకవేళ (argc)
			strvec_pushv(&వాదనలు, argv);
		లేకపోతే
			strvec_నెట్టు(&వాదనలు, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, వాదనలు.nr + 1);
		కొరకు (కొలత_t i = 0; i < వాదనలు.nr; i++)
			argv_copy[i] = వాదనలు.v[i];
		argv_copy[వాదనలు.nr] = ఖాళీ;

		ret = cmd_name_rev(వాదనలు.nr, argv_copy, ఉపసర్గ, the_repository);

		strvec_clear(&వాదనలు);
		విడుదల(argv_copy);
		తిరిగి ret;
	}

	ఒకవేళ (!all)
		for_each_ref_opts.ఉపసర్గ = "refs/tags/";

	hashmap_init(&పేర్లు, commit_name_neq, ఖాళీ, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, ఖాళీ, &for_each_ref_opts);
	ఒకవేళ (!hashmap_get_కొలత(&పేర్లు) && !always)
		die(_("No names found, cannot describe anything."));

	ఒకవేళ (argc == 0) {
		ఒకవేళ (broken) {
			నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

			strvec_pushv(&cp.వాదనలు, update_సూచీ_వాదనలు);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			నిరంతరం_command(&cp);

			బిడ్డ_process_init(&cp);
			strvec_pushv(&cp.వాదనలు, diff_సూచీ_వాదనలు);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			ఒకవేళ (!dirty)
				dirty = "-dirty";

			ఎంపిక (నిరంతరం_command(&cp)) {
			స్థితి 0:
				suffix = ఖాళీ;
				ఆపు;
			స్థితి 1:
				suffix = dirty;
				ఆపు;
			అప్రమేయ:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} లేకపోతే ఒకవేళ (dirty) {
			నిర్మాణం తాళం_file సూచీ_తాళం = తాళం_INIT;
			నిర్మాణం rev_info revs;
			పూర్ణాంకం fd;

			అమరిక_work_వృక్షం(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_నిండిన_సూచీ = 0;
			repo_read_సూచీ(the_repository);
			refresh_సూచీ(the_repository->సూచీ, REFRESH_QUIET|REFRESH_UNMERGED,
				      ఖాళీ, ఖాళీ, ఖాళీ);
			fd = repo_hold_locked_సూచీ(the_repository,
						    &సూచీ_తాళం, 0);
			ఒకవేళ (0 <= fd)
				repo_update_సూచీ_if_able(the_repository, &సూచీ_తాళం);

			repo_init_revisions(the_repository, &revs, ఉపసర్గ);

			ఒకవేళ (అమరిక_revisions(ARRAY_కొలత(diff_సూచీ_వాదనలు) - 1,
					    diff_సూచీ_వాదనలు, &revs, ఖాళీ) != 1)
				BUG("malformed internal diff-index command line");
			నిరంతరం_diff_సూచీ(&revs, 0);

			ఒకవేళ (!diff_ఫలితం_సంకేతం(&revs))
				suffix = ఖాళీ;
			లేకపోతే
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} లేకపోతే ఒకవేళ (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} లేకపోతే ఒకవేళ (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} లేకపోతే {
		అయితే (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	తిరిగి 0;
}
