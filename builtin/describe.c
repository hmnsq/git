#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

#وصف_ڪريو MAX_ٽيگ_فهرست	(FLAG_بٽ - 1)
#وصف_ڪريو DEFAULT_CANDIDATES 10

define_commit_slab(commit_نالا, بنية commit_name *);

ساكن ثابت حرف * ثابت describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	عدم
};

ساكن صحيح debug;	/* Display lots of verbose info */
ساكن صحيح all;	/* Any valid ref can be used */
ساكن صحيح ٽيگ_فهرست;	/* Allow lightweight tags */
ساكن صحيح longformat;
ساكن صحيح پهريون_والد;
ساكن صحيح abbrev = -1; /* unspecified */
ساكن صحيح max_candidates = DEFAULT_CANDIDATES;
ساكن بنية hashmap نالا;
ساكن صحيح have_util;
ساكن بنية string_list patterns = STRING_LIST_INIT_NODUP;
ساكن بنية string_list exclude_patterns = STRING_LIST_INIT_NODUP;
ساكن صحيح always;
ساكن ثابت حرف *suffix, *dirty, *broken;
ساكن بنية commit_نالا commit_نالا;

/* diff-index command arguments to check if working tree is dirty. */
ساكن ثابت حرف *diff_اشاريو_دليلون[] = {
	"diff-index", "--quiet", "HEAD", "--", عدم
};

ساكن ثابت حرف *update_اشاريو_دليلون[] = {
	"update-index", "--unmerged", "-q", "--refresh", عدم
};

بنية commit_name {
	بنية hashmap_داخلا داخلا;
	بنية object_id peeled;
	بنية ٽيگ *ٽيگ;
	بنانشان prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	بنانشان name_checked:1;
	بنانشان misnamed:1;
	بنية object_id oid;
	حرف *رستو;
};

ساكن ثابت حرف *prio_نالا[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

ساكن صحيح commit_name_neq(ثابت فراغ *cmp_معلومات اڻ_استعمال_ٿيل,
			   ثابت بنية hashmap_داخلا *eptr,
			   ثابت بنية hashmap_داخلا *داخلا_or_ڪنجي,
			   ثابت فراغ *peeled)
{
	ثابت بنية commit_name *cn1, *cn2;

	cn1 = container_جو(eptr, ثابت بنية commit_name, داخلا);
	cn2 = container_جو(داخلا_or_ڪنجي, ثابت بنية commit_name, داخلا);

	أرجع !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

ساكن مضمن بنية commit_name *ڳوليو_commit_name(ثابت بنية object_id *peeled)
{
	أرجع hashmap_get_داخلا_from_hash(&نالا, oidhash(peeled), peeled,
						بنية commit_name, داخلا);
}

ساكن صحيح replace_name(بنية commit_name *e,
			       صحيح prio,
			       ثابت بنية object_id *oid,
			       بنية ٽيگ **ٽيگ)
{
	إذا (!e || e->prio < prio)
		أرجع 1;

	إذا (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		بنية ٽيگ *t;

		إذا (!e->ٽيگ) {
			t = lookup_ٽيگ(the_repository, &e->oid);
			إذا (!t || parse_ٽيگ(the_repository, t))
				أرجع 1;
			e->ٽيگ = t;
		}

		t = lookup_ٽيگ(the_repository, oid);
		إذا (!t || parse_ٽيگ(the_repository, t))
			أرجع 0;
		*ٽيگ = t;

		إذا (e->ٽيگ->date < t->date)
			أرجع 1;
	}

	أرجع 0;
}

ساكن فراغ جوڙيو_منزل_معلوم_نالا(ثابت حرف *رستو,
			       ثابت بنية object_id *peeled,
			       صحيح prio,
			       ثابت بنية object_id *oid)
{
	بنية commit_name *e = ڳوليو_commit_name(peeled);
	بنية ٽيگ *ٽيگ = عدم;
	إذا (replace_name(e, prio, oid, &ٽيگ)) {
		إذا (!e) {
			e = xmalloc(حجم(بنية commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_داخلا_init(&e->داخلا, oidhash(peeled));
			hashmap_جوڙيو(&نالا, &e->داخلا);
			e->رستو = عدم;
		}
		e->ٽيگ = ٽيگ;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		حرر(e->رستو);
		e->رستو = xstrdup(رستو);
	}
}

ساكن صحيح get_name(ثابت بنية reference *ref, فراغ *cb_معلومات اڻ_استعمال_ٿيل)
{
	صحيح is_ٽيگ = 0;
	بنية object_id peeled;
	صحيح is_annotated, prio;
	ثابت حرف *رستو_منزل_ميلاپ = عدم;

	إذا (skip_اڳياڙي(ref->name, "refs/tags/", &رستو_منزل_ميلاپ)) {
		is_ٽيگ = 1;
	} وإلا إذا (all) {
		إذا ((exclude_patterns.nr || patterns.nr) &&
		    !skip_اڳياڙي(ref->name, "refs/heads/", &رستو_منزل_ميلاپ) &&
		    !skip_اڳياڙي(ref->name, "refs/remotes/", &رستو_منزل_ميلاپ)) {
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
		بنية string_list_شي *شي;

		for_each_string_list_شي(شي, &exclude_patterns) {
			إذا (!wildmatch(شي->string, رستو_منزل_ميلاپ, 0))
				أرجع 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	إذا (patterns.nr) {
		صحيح مليو_2 = 0;
		بنية string_list_شي *شي;

		for_each_string_list_شي(شي, &patterns) {
			إذا (!wildmatch(شي->string, رستو_منزل_ميلاپ, 0)) {
				مليو_2 = 1;
				اكسر;
			}
		}

		إذا (!مليو_2)
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
	وإلا إذا (is_ٽيگ)
		prio = 1;
	وإلا
		prio = 0;

	جوڙيو_منزل_معلوم_نالا(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	أرجع 0;
}

بنية possible_ٽيگ {
	بنية commit_name *name;
	صحيح اونهائي;
	صحيح مليو_ترتيب;
	بنانشان flag_within;
};

ساكن صحيح compare_pt(ثابت فراغ *a_, ثابت فراغ *b_)
{
	بنية possible_ٽيگ *a = (بنية possible_ٽيگ *)a_;
	بنية possible_ٽيگ *b = (بنية possible_ٽيگ *)b_;
	إذا (a->اونهائي != b->اونهائي)
		أرجع a->اونهائي - b->اونهائي;
	إذا (a->مليو_ترتيب != b->مليو_ترتيب)
		أرجع a->مليو_ترتيب - b->مليو_ترتيب;
	أرجع 0;
}

ساكن بنانشان ڊگهو پڄاڻي_اونهائي_computation(بنية prio_قطار *قطار,
					      بنية possible_ٽيگ *بهترين)
{
	بنانشان ڊگهو ڏٺل_commits = 0;
	بنية oidset unflagged = OIDSET_INIT;
	بنية commit *c;

	prio_قطار_for_each(قطار, c) {
		إذا (!(c->object.flags & بهترين->flag_within))
			oidset_وجهو(&unflagged, &c->object.oid);
	}

	طالما ((c = prio_قطار_get(قطار))) {
		بنية commit_list *parents = c->parents;
		ڏٺل_commits++;
		إذا (c->object.flags & بهترين->flag_within) {
			إذا (!oidset_سائيز(&unflagged))
				اكسر;
		} وإلا {
			oidset_remove(&unflagged, &c->object.oid);
			بهترين->اونهائي++;
		}
		طالما (parents) {
			بنانشان ڏٺل, flag_before, flag_after;
			بنية commit *p = parents->شي;
			repo_parse_commit(the_repository, p);
			ڏٺل = p->object.flags & ڏٺل_2;
			إذا (!ڏٺل)
				prio_قطار_put(قطار, p);
			flag_before = p->object.flags & بهترين->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & بهترين->flag_within;
			إذا (!ڏٺل && !flag_after)
				oidset_وجهو(&unflagged, &p->object.oid);
			إذا (ڏٺل && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->ايندڙ;
		}
	}
	oidset_clear(&unflagged);
	أرجع ڏٺل_commits;
}

ساكن فراغ append_name(بنية commit_name *n, بنية strbuf *dst)
{
	إذا (n->prio == 2 && !n->ٽيگ) {
		n->ٽيگ = lookup_ٽيگ(the_repository, &n->oid);
		إذا (!n->ٽيگ || parse_ٽيگ(the_repository, n->ٽيگ))
			die(_("annotated tag %s not available"), n->رستو);
	}
	إذا (n->ٽيگ && !n->name_checked) {
		إذا (strcmp(n->ٽيگ->ٽيگ, all ? n->رستو + 5 : n->رستو)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->رستو, n->ٽيگ->ٽيگ);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	إذا (n->ٽيگ) {
		إذا (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->ٽيگ->ٽيگ);
	} وإلا {
		strbuf_addstr(dst, n->رستو);
	}
}

ساكن فراغ append_suffix(صحيح اونهائي, ثابت بنية object_id *oid, بنية strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", اونهائي,
		    repo_ڳوليو_unique_abbrev(the_repository, oid, abbrev));
}

ساكن فراغ describe_commit(بنية commit *cmit, بنية strbuf *dst)
{
	بنية commit *c, *gave_مٿي_on = عدم;
	بنية prio_قطار قطار = { compare_commits_by_commit_date };
	بنية commit_name *n;
	بنية possible_ٽيگ all_matches[MAX_ٽيگ_فهرست];
	بنانشان صحيح ميلاپ_cnt = 0, annotated_cnt = 0, cur_ميلاپ;
	بنانشان ڊگهو ڏٺل_commits = 0;
	بنانشان صحيح unannotated_cnt = 0;

	n = ڳوليو_commit_name(&cmit->object.oid);
	إذا (n && (ٽيگ_فهرست || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		إذا (n->misnamed || longformat)
			append_suffix(0, n->ٽيگ ? get_tagged_oid(n->ٽيگ) : &cmit->object.oid, dst);
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

		init_commit_نالا(&commit_نالا);
		hashmap_for_each_داخلا(&نالا, &iter, n,
					داخلا /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			إذا (c)
				*commit_نالا_at(&commit_نالا, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = ڏٺل_2;
	prio_قطار_put(&قطار, cmit);
	طالما ((c = prio_قطار_get(&قطار))) {
		بنية commit_list *parents = c->parents;
		بنية commit_name **جڳهه;

		ڏٺل_commits++;

		إذا (ميلاپ_cnt == max_candidates ||
		    ميلاپ_cnt == hashmap_get_سائيز(&نالا)) {
			gave_مٿي_on = c;
			اكسر;
		}

		جڳهه = commit_نالا_peek(&commit_نالا, c);
		n = جڳهه ? *جڳهه : عدم;
		إذا (n) {
			إذا (!ٽيگ_فهرست && !all && n->prio < 2) {
				unannotated_cnt++;
			} وإلا إذا (ميلاپ_cnt < max_candidates) {
				بنية possible_ٽيگ *t = &all_matches[ميلاپ_cnt++];
				t->name = n;
				t->اونهائي = ڏٺل_commits - 1;
				t->flag_within = 1u << ميلاپ_cnt;
				t->مليو_ترتيب = ميلاپ_cnt;
				c->object.flags |= t->flag_within;
				إذا (n->prio == 2)
					annotated_cnt++;
			}
		}
		لكل (cur_ميلاپ = 0; cur_ميلاپ < ميلاپ_cnt; cur_ميلاپ++) {
			بنية possible_ٽيگ *t = &all_matches[cur_ميلاپ];
			إذا (!(c->object.flags & t->flag_within))
				t->اونهائي++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		إذا (annotated_cnt && !prio_قطار_سائيز(&قطار)) {
			صحيح بهترين_اونهائي = وڌ_کان_وڌ_عدد;
			بنانشان بهترين_within = 0;
			لكل (cur_ميلاپ = 0; cur_ميلاپ < ميلاپ_cnt; cur_ميلاپ++) {
				بنية possible_ٽيگ *t = &all_matches[cur_ميلاپ];
				إذا (t->اونهائي < بهترين_اونهائي) {
					بهترين_اونهائي = t->اونهائي;
					بهترين_within = t->flag_within;
				} وإلا إذا (t->اونهائي == بهترين_اونهائي) {
					بهترين_within |= t->flag_within;
				}
			}
			إذا ((c->object.flags & بهترين_within) == بهترين_within) {
				إذا (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_منزل_hex(&c->object.oid));
				اكسر;
			}
		}
		طالما (parents) {
			بنية commit *p = parents->شي;
			repo_parse_commit(the_repository, p);
			إذا (!(p->object.flags & ڏٺل_2))
				prio_قطار_put(&قطار, p);
			p->object.flags |= c->object.flags;
			parents = parents->ايندڙ;

			إذا (پهريون_والد)
				اكسر;
		}
	}

	إذا (!ميلاپ_cnt) {
		بنية object_id *cmit_oid = &cmit->object.oid;
		إذا (always) {
			strbuf_جوڙيو_unique_abbrev(dst, cmit_oid, abbrev);
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

	QSORT(all_matches, ميلاپ_cnt, compare_pt);

	إذا (gave_مٿي_on) {
		prio_قطار_put(&قطار, gave_مٿي_on);
		ڏٺل_commits--;
	}
	ڏٺل_commits += پڄاڻي_اونهائي_computation(&قطار, &all_matches[0]);
	clear_prio_قطار(&قطار);

	إذا (debug) {
		ساكن صحيح label_ويڪر = -1;
		إذا (label_ويڪر < 0) {
			صحيح i, w;
			لكل (i = 0; i < ARRAY_سائيز(prio_نالا); i++) {
				w = لڙي_ڊيگهه(_(prio_نالا[i]));
				إذا (label_ويڪر < w)
					label_ويڪر = w;
			}
		}
		لكل (cur_ميلاپ = 0; cur_ميلاپ < ميلاپ_cnt; cur_ميلاپ++) {
			بنية possible_ٽيگ *t = &all_matches[cur_ميلاپ];
			fprintf(stderr, " %-*s %8d %s\n",
				label_ويڪر, _(prio_نالا[t->name->prio]),
				t->اونهائي, t->name->رستو);
		}
		fprintf(stderr, _("traversed %lu commits\n"), ڏٺل_commits);
		إذا (gave_مٿي_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_منزل_hex(&gave_مٿي_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	إذا (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].اونهائي, &cmit->object.oid, dst);
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

ساكن فراغ process_object(بنية object *obj, ثابت حرف *رستو, فراغ *معلومات)
{
	بنية process_commit_معلومات *pcd = معلومات;

	إذا (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		إذا (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", رستو);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = عدم;
	}
}

ساكن فراغ describe_blob(ثابت بنية object_id *oid, بنية strbuf *dst)
{
	بنية rev_info revs;
	بنية strvec دليلون = STRVEC_INIT;
	بنية object_id سر_oid;
	بنية process_commit_معلومات pcd = { عدم, oid, dst, &revs};

	إذا (repo_get_oid(the_repository, "HEAD", &سر_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_منزل_hex(oid));

	strvec_pushl(&دليلون, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_منزل_hex(&سر_oid),
		     عدم);

	repo_init_revisions(the_repository, &revs, عدم);
	تياري_revisions_from_strvec(&دليلون, &revs, عدم);
	إذا (دليلون.nr > 1)
		BUG("setup_revisions could not handle all args?");

	إذا (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&دليلون);

	إذا (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_منزل_hex(oid));
}

ساكن فراغ describe(ثابت حرف *arg, صحيح آخري_one)
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

	إذا (!آخري_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

ساكن صحيح option_parse_exact_ميلاپ(ثابت بنية option *opt, ثابت حرف *arg,
				    صحيح unset)
{
	صحيح *val = opt->قدر;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	أرجع 0;
}

صحيح cmd_describe(صحيح argc,
		 ثابت حرف **argv,
		 ثابت حرف *اڳياڙي,
		 بنية repository *repo اڻ_استعمال_ٿيل )
{
	بنية refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	صحيح شامل_آهي = 0;
	بنية option options[] = {
		OPT_BOOL(0, "contains",   &شامل_آهي, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &ٽيگ_فهرست, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &پهريون_والد, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, عدم,
			       N_("only output exact matches"),
			       PARSE_OPT_NOARG, option_parse_exact_ميلاپ),
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
		OPT_پڇاڙي(),
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, اڳياڙي, options, describe_usage, 0);
	إذا (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	إذا (max_candidates < 0)
		max_candidates = 0;
	وإلا إذا (max_candidates > MAX_ٽيگ_فهرست)
		max_candidates = MAX_ٽيگ_فهرست;

	save_commit_بفر = 0;

	إذا (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	إذا (شامل_آهي) {
		بنية string_list_شي *شي;
		بنية strvec دليلون;
		ثابت حرف **argv_copy;
		صحيح ret;

		strvec_init(&دليلون);
		strvec_pushl(&دليلون, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     عدم);
		إذا (always)
			strvec_ڌڪو(&دليلون, "--always");
		إذا (!all)
			strvec_ڌڪو(&دليلون, "--tags");

		for_each_string_list_شي(شي, &patterns)
			strvec_pushf(&دليلون, "--refs=refs/tags/%s", شي->string);
		for_each_string_list_شي(شي, &exclude_patterns)
			strvec_pushf(&دليلون, "--exclude=refs/tags/%s", شي->string);

		إذا (all) {
			for_each_string_list_شي(شي, &patterns)
				strvec_pushf(&دليلون, "--refs=refs/heads/%s", شي->string);
			for_each_string_list_شي(شي, &exclude_patterns)
				strvec_pushf(&دليلون, "--exclude=refs/heads/%s", شي->string);
			for_each_string_list_شي(شي, &patterns)
				strvec_pushf(&دليلون, "--refs=refs/remotes/%s", شي->string);
			for_each_string_list_شي(شي, &exclude_patterns)
				strvec_pushf(&دليلون, "--exclude=refs/remotes/%s", شي->string);
		}

		إذا (argc)
			strvec_pushv(&دليلون, argv);
		وإلا
			strvec_ڌڪو(&دليلون, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, دليلون.nr + 1);
		لكل (سائيز_t i = 0; i < دليلون.nr; i++)
			argv_copy[i] = دليلون.v[i];
		argv_copy[دليلون.nr] = عدم;

		ret = cmd_name_rev(دليلون.nr, argv_copy, اڳياڙي, the_repository);

		strvec_clear(&دليلون);
		حرر(argv_copy);
		أرجع ret;
	}

	إذا (!all)
		for_each_ref_opts.اڳياڙي = "refs/tags/";

	hashmap_init(&نالا, commit_name_neq, عدم, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, عدم, &for_each_ref_opts);
	إذا (!hashmap_get_سائيز(&نالا) && !always)
		die(_("No names found, cannot describe anything."));

	إذا (argc == 0) {
		إذا (broken) {
			بنية ٻار_process cp = ٻار_PROCESS_INIT;

			strvec_pushv(&cp.دليلون, update_اشاريو_دليلون);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			لاڳيتو_command(&cp);

			ٻار_process_init(&cp);
			strvec_pushv(&cp.دليلون, diff_اشاريو_دليلون);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			إذا (!dirty)
				dirty = "-dirty";

			بدل (لاڳيتو_command(&cp)) {
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
			بنية تالو_file اشاريو_تالو = تالو_INIT;
			بنية rev_info revs;
			صحيح fd;

			تياري_work_وڻ(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_ڀريل_اشاريو = 0;
			repo_read_اشاريو(the_repository);
			refresh_اشاريو(the_repository->اشاريو, REFRESH_QUIET|REFRESH_UNMERGED,
				      عدم, عدم, عدم);
			fd = repo_hold_locked_اشاريو(the_repository,
						    &اشاريو_تالو, 0);
			إذا (0 <= fd)
				repo_update_اشاريو_if_able(the_repository, &اشاريو_تالو);

			repo_init_revisions(the_repository, &revs, اڳياڙي);

			إذا (تياري_revisions(ARRAY_سائيز(diff_اشاريو_دليلون) - 1,
					    diff_اشاريو_دليلون, &revs, عدم) != 1)
				BUG("malformed internal diff-index command line");
			لاڳيتو_diff_اشاريو(&revs, 0);

			إذا (!diff_نتيجو_ڪوڊ(&revs))
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
