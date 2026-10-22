#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "lockfile.h"
#شامل "commit.h"
#شامل "tag.h"
#شامل "refs.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "read-cache-ll.h"
#شامل "revision.h"
#شامل "diff.h"
#شامل "hashmap.h"
#شامل "setup.h"
#شامل "strvec.h"
#شامل "run-command.h"
#شامل "odb.h"
#شامل "list-objects.h"
#شامل "commit-slab.h"
#شامل "wildmatch.h"
#شامل "prio-queue.h"
#شامل "oidset.h"

#تعریف_کرو MAX_لیبل_فہرست	(FLAG_بٹ - 1)
#تعریف_کرو DEFAULT_CANDIDATES 10

define_commit_slab(commit_نام, ساخت commit_name *);

ساکن ثابت حرف * ثابت describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	تہی
};

ساکن صحیح debug;	/* Display lots of verbose info */
ساکن صحیح all;	/* Any valid ref can be used */
ساکن صحیح لیبل_فہرست;	/* Allow lightweight tags */
ساکن صحیح longformat;
ساکن صحیح پہلا_والد;
ساکن صحیح abbrev = -1; /* unspecified */
ساکن صحیح max_candidates = DEFAULT_CANDIDATES;
ساکن ساخت hashmap نام;
ساکن صحیح have_util;
ساکن ساخت string_list patterns = STRING_LIST_INIT_NODUP;
ساکن ساخت string_list exclude_patterns = STRING_LIST_INIT_NODUP;
ساکن صحیح always;
ساکن ثابت حرف *suffix, *dirty, *broken;
ساکن ساخت commit_نام commit_نام;

/* diff-index command arguments to check if working tree is dirty. */
ساکن ثابت حرف *diff_اشاریہ_دلائل[] = {
	"diff-index", "--quiet", "HEAD", "--", تہی
};

ساکن ثابت حرف *update_اشاریہ_دلائل[] = {
	"update-index", "--unmerged", "-q", "--refresh", تہی
};

ساخت commit_name {
	ساخت hashmap_اندراج اندراج_2;
	ساخت object_id peeled;
	ساخت لیبل *لیبل;
	غیرعلامتی prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	غیرعلامتی name_checked:1;
	غیرعلامتی misnamed:1;
	ساخت object_id oid;
	حرف *راستہ;
};

ساکن ثابت حرف *prio_نام[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

ساکن صحیح commit_name_neq(ثابت خالی *cmp_ڈیٹا غیر_مستعمل,
			   ثابت ساخت hashmap_اندراج *eptr,
			   ثابت ساخت hashmap_اندراج *اندراج_or_کلید,
			   ثابت خالی *peeled)
{
	ثابت ساخت commit_name *cn1, *cn2;

	cn1 = container_کا(eptr, ثابت ساخت commit_name, اندراج_2);
	cn2 = container_کا(اندراج_or_کلید, ثابت ساخت commit_name, اندراج_2);

	واپس !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

ساکن اندرونی ساخت commit_name *ڈھونڈو_commit_name(ثابت ساخت object_id *peeled)
{
	واپس hashmap_get_اندراج_from_hash(&نام, oidhash(peeled), peeled,
						ساخت commit_name, اندراج_2);
}

ساکن صحیح replace_name(ساخت commit_name *e,
			       صحیح prio,
			       ثابت ساخت object_id *oid,
			       ساخت لیبل **لیبل)
{
	اگر (!e || e->prio < prio)
		واپس 1;

	اگر (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		ساخت لیبل *t;

		اگر (!e->لیبل) {
			t = lookup_لیبل(the_repository, &e->oid);
			اگر (!t || parse_لیبل(the_repository, t))
				واپس 1;
			e->لیبل = t;
		}

		t = lookup_لیبل(the_repository, oid);
		اگر (!t || parse_لیبل(the_repository, t))
			واپس 0;
		*لیبل = t;

		اگر (e->لیبل->date < t->date)
			واپس 1;
	}

	واپس 0;
}

ساکن خالی جوڑو_مقصد_معلوم_نام(ثابت حرف *راستہ,
			       ثابت ساخت object_id *peeled,
			       صحیح prio,
			       ثابت ساخت object_id *oid)
{
	ساخت commit_name *e = ڈھونڈو_commit_name(peeled);
	ساخت لیبل *لیبل = تہی;
	اگر (replace_name(e, prio, oid, &لیبل)) {
		اگر (!e) {
			e = xmalloc(حجم(ساخت commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_اندراج_init(&e->اندراج_2, oidhash(peeled));
			hashmap_جوڑو(&نام, &e->اندراج_2);
			e->راستہ = تہی;
		}
		e->لیبل = لیبل;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		آزاد(e->راستہ);
		e->راستہ = xstrdup(راستہ);
	}
}

ساکن صحیح get_name(ثابت ساخت reference *ref, خالی *cb_ڈیٹا غیر_مستعمل)
{
	صحیح is_لیبل = 0;
	ساخت object_id peeled;
	صحیح is_annotated, prio;
	ثابت حرف *راستہ_مقصد_مطابقت = تہی;

	اگر (skip_سابقہ(ref->name, "refs/tags/", &راستہ_مقصد_مطابقت)) {
		is_لیبل = 1;
	} ورنہ اگر (all) {
		اگر ((exclude_patterns.nr || patterns.nr) &&
		    !skip_سابقہ(ref->name, "refs/heads/", &راستہ_مقصد_مطابقت) &&
		    !skip_سابقہ(ref->name, "refs/remotes/", &راستہ_مقصد_مطابقت)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			واپس 0;
		}
	} ورنہ {
		/* Reject anything outside refs/tags/ unless --all */
		واپس 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	اگر (exclude_patterns.nr) {
		ساخت string_list_شے *شے;

		for_each_string_list_شے(شے, &exclude_patterns) {
			اگر (!wildmatch(شے->string, راستہ_مقصد_مطابقت, 0))
				واپس 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	اگر (patterns.nr) {
		صحیح ملا_2 = 0;
		ساخت string_list_شے *شے;

		for_each_string_list_شے(شے, &patterns) {
			اگر (!wildmatch(شے->string, راستہ_مقصد_مطابقت, 0)) {
				ملا_2 = 1;
				توڑو;
			}
		}

		اگر (!ملا_2)
			واپس 0;
	}

	/* Is it annotated? */
	اگر (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} ورنہ {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	اگر (is_annotated)
		prio = 2;
	ورنہ اگر (is_لیبل)
		prio = 1;
	ورنہ
		prio = 0;

	جوڑو_مقصد_معلوم_نام(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	واپس 0;
}

ساخت possible_لیبل {
	ساخت commit_name *name;
	صحیح گہرائی;
	صحیح ملا_ترتیب;
	غیرعلامتی flag_within;
};

ساکن صحیح compare_pt(ثابت خالی *a_, ثابت خالی *b_)
{
	ساخت possible_لیبل *a = (ساخت possible_لیبل *)a_;
	ساخت possible_لیبل *b = (ساخت possible_لیبل *)b_;
	اگر (a->گہرائی != b->گہرائی)
		واپس a->گہرائی - b->گہرائی;
	اگر (a->ملا_ترتیب != b->ملا_ترتیب)
		واپس a->ملا_ترتیب - b->ملا_ترتیب;
	واپس 0;
}

ساکن غیرعلامتی طویل اختتام_گہرائی_computation(ساخت prio_قطار *قطار,
					      ساخت possible_لیبل *بہترین)
{
	غیرعلامتی طویل نظر_آیا_commits = 0;
	ساخت oidset unflagged = OIDSET_INIT;
	ساخت commit *c;

	prio_قطار_for_each(قطار, c) {
		اگر (!(c->object.flags & بہترین->flag_within))
			oidset_داخل_کرو(&unflagged, &c->object.oid);
	}

	جب ((c = prio_قطار_get(قطار))) {
		ساخت commit_list *parents = c->parents;
		نظر_آیا_commits++;
		اگر (c->object.flags & بہترین->flag_within) {
			اگر (!oidset_سائز(&unflagged))
				توڑو;
		} ورنہ {
			oidset_remove(&unflagged, &c->object.oid);
			بہترین->گہرائی++;
		}
		جب (parents) {
			غیرعلامتی نظر_آیا, flag_before, flag_after;
			ساخت commit *p = parents->شے;
			repo_parse_commit(the_repository, p);
			نظر_آیا = p->object.flags & نظر_آیا_2;
			اگر (!نظر_آیا)
				prio_قطار_put(قطار, p);
			flag_before = p->object.flags & بہترین->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & بہترین->flag_within;
			اگر (!نظر_آیا && !flag_after)
				oidset_داخل_کرو(&unflagged, &p->object.oid);
			اگر (نظر_آیا && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->اگلا;
		}
	}
	oidset_clear(&unflagged);
	واپس نظر_آیا_commits;
}

ساکن خالی append_name(ساخت commit_name *n, ساخت strbuf *dst)
{
	اگر (n->prio == 2 && !n->لیبل) {
		n->لیبل = lookup_لیبل(the_repository, &n->oid);
		اگر (!n->لیبل || parse_لیبل(the_repository, n->لیبل))
			die(_("annotated tag %s not available"), n->راستہ);
	}
	اگر (n->لیبل && !n->name_checked) {
		اگر (strcmp(n->لیبل->لیبل, all ? n->راستہ + 5 : n->راستہ)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->راستہ, n->لیبل->لیبل);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	اگر (n->لیبل) {
		اگر (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->لیبل->لیبل);
	} ورنہ {
		strbuf_addstr(dst, n->راستہ);
	}
}

ساکن خالی append_suffix(صحیح گہرائی, ثابت ساخت object_id *oid, ساخت strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", گہرائی,
		    repo_ڈھونڈو_unique_abbrev(the_repository, oid, abbrev));
}

ساکن خالی describe_commit(ساخت commit *cmit, ساخت strbuf *dst)
{
	ساخت commit *c, *gave_اوپر_on = تہی;
	ساخت prio_قطار قطار = { compare_commits_by_commit_date };
	ساخت commit_name *n;
	ساخت possible_لیبل all_matches[MAX_لیبل_فہرست];
	غیرعلامتی صحیح مطابقت_cnt = 0, annotated_cnt = 0, cur_مطابقت;
	غیرعلامتی طویل نظر_آیا_commits = 0;
	غیرعلامتی صحیح unannotated_cnt = 0;

	n = ڈھونڈو_commit_name(&cmit->object.oid);
	اگر (n && (لیبل_فہرست || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		اگر (n->misnamed || longformat)
			append_suffix(0, n->لیبل ? get_tagged_oid(n->لیبل) : &cmit->object.oid, dst);
		اگر (suffix)
			strbuf_addstr(dst, suffix);
		واپس;
	}

	اگر (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_مقصد_hex(&cmit->object.oid));
	اگر (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	اگر (!have_util) {
		ساخت hashmap_iter iter;
		ساخت commit *c;
		ساخت commit_name *n;

		init_commit_نام(&commit_نام);
		hashmap_for_each_اندراج(&نام, &iter, n,
					اندراج_2 /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			اگر (c)
				*commit_نام_at(&commit_نام, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = نظر_آیا_2;
	prio_قطار_put(&قطار, cmit);
	جب ((c = prio_قطار_get(&قطار))) {
		ساخت commit_list *parents = c->parents;
		ساخت commit_name **خانہ;

		نظر_آیا_commits++;

		اگر (مطابقت_cnt == max_candidates ||
		    مطابقت_cnt == hashmap_get_سائز(&نام)) {
			gave_اوپر_on = c;
			توڑو;
		}

		خانہ = commit_نام_peek(&commit_نام, c);
		n = خانہ ? *خانہ : تہی;
		اگر (n) {
			اگر (!لیبل_فہرست && !all && n->prio < 2) {
				unannotated_cnt++;
			} ورنہ اگر (مطابقت_cnt < max_candidates) {
				ساخت possible_لیبل *t = &all_matches[مطابقت_cnt++];
				t->name = n;
				t->گہرائی = نظر_آیا_commits - 1;
				t->flag_within = 1u << مطابقت_cnt;
				t->ملا_ترتیب = مطابقت_cnt;
				c->object.flags |= t->flag_within;
				اگر (n->prio == 2)
					annotated_cnt++;
			}
		}
		برائے (cur_مطابقت = 0; cur_مطابقت < مطابقت_cnt; cur_مطابقت++) {
			ساخت possible_لیبل *t = &all_matches[cur_مطابقت];
			اگر (!(c->object.flags & t->flag_within))
				t->گہرائی++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		اگر (annotated_cnt && !prio_قطار_سائز(&قطار)) {
			صحیح بہترین_گہرائی = زیادہ_سے_زیادہ_عدد;
			غیرعلامتی بہترین_within = 0;
			برائے (cur_مطابقت = 0; cur_مطابقت < مطابقت_cnt; cur_مطابقت++) {
				ساخت possible_لیبل *t = &all_matches[cur_مطابقت];
				اگر (t->گہرائی < بہترین_گہرائی) {
					بہترین_گہرائی = t->گہرائی;
					بہترین_within = t->flag_within;
				} ورنہ اگر (t->گہرائی == بہترین_گہرائی) {
					بہترین_within |= t->flag_within;
				}
			}
			اگر ((c->object.flags & بہترین_within) == بہترین_within) {
				اگر (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_مقصد_hex(&c->object.oid));
				توڑو;
			}
		}
		جب (parents) {
			ساخت commit *p = parents->شے;
			repo_parse_commit(the_repository, p);
			اگر (!(p->object.flags & نظر_آیا_2))
				prio_قطار_put(&قطار, p);
			p->object.flags |= c->object.flags;
			parents = parents->اگلا;

			اگر (پہلا_والد)
				توڑو;
		}
	}

	اگر (!مطابقت_cnt) {
		ساخت object_id *cmit_oid = &cmit->object.oid;
		اگر (always) {
			strbuf_جوڑو_unique_abbrev(dst, cmit_oid, abbrev);
			اگر (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_قطار(&قطار);
			واپس;
		}
		اگر (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_مقصد_hex(cmit_oid));
		ورنہ
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_مقصد_hex(cmit_oid));
	}

	QSORT(all_matches, مطابقت_cnt, compare_pt);

	اگر (gave_اوپر_on) {
		prio_قطار_put(&قطار, gave_اوپر_on);
		نظر_آیا_commits--;
	}
	نظر_آیا_commits += اختتام_گہرائی_computation(&قطار, &all_matches[0]);
	clear_prio_قطار(&قطار);

	اگر (debug) {
		ساکن صحیح label_چوڑائی = -1;
		اگر (label_چوڑائی < 0) {
			صحیح i, w;
			برائے (i = 0; i < ARRAY_سائز(prio_نام); i++) {
				w = لڑی_لمبائی(_(prio_نام[i]));
				اگر (label_چوڑائی < w)
					label_چوڑائی = w;
			}
		}
		برائے (cur_مطابقت = 0; cur_مطابقت < مطابقت_cnt; cur_مطابقت++) {
			ساخت possible_لیبل *t = &all_matches[cur_مطابقت];
			fprintf(stderr, " %-*s %8d %s\n",
				label_چوڑائی, _(prio_نام[t->name->prio]),
				t->گہرائی, t->name->راستہ);
		}
		fprintf(stderr, _("traversed %lu commits\n"), نظر_آیا_commits);
		اگر (gave_اوپر_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_مقصد_hex(&gave_اوپر_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	اگر (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].گہرائی, &cmit->object.oid, dst);
	اگر (suffix)
		strbuf_addstr(dst, suffix);
}

ساخت process_commit_ڈیٹا {
	ساخت commit *current_commit;
	ثابت ساخت object_id *looking_for;
	ساخت strbuf *dst;
	ساخت rev_info *revs;
};

ساکن خالی process_commit(ساخت commit *commit, خالی *ڈیٹا)
{
	ساخت process_commit_ڈیٹا *pcd = ڈیٹا;
	pcd->current_commit = commit;
}

ساکن خالی process_object(ساخت object *obj, ثابت حرف *راستہ, خالی *ڈیٹا)
{
	ساخت process_commit_ڈیٹا *pcd = ڈیٹا;

	اگر (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		اگر (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", راستہ);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = تہی;
	}
}

ساکن خالی describe_blob(ثابت ساخت object_id *oid, ساخت strbuf *dst)
{
	ساخت rev_info revs;
	ساخت strvec دلائل = STRVEC_INIT;
	ساخت object_id سر_oid;
	ساخت process_commit_ڈیٹا pcd = { تہی, oid, dst, &revs};

	اگر (repo_get_oid(the_repository, "HEAD", &سر_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_مقصد_hex(oid));

	strvec_pushl(&دلائل, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_مقصد_hex(&سر_oid),
		     تہی);

	repo_init_revisions(the_repository, &revs, تہی);
	تیاری_revisions_from_strvec(&دلائل, &revs, تہی);
	اگر (دلائل.nr > 1)
		BUG("setup_revisions could not handle all args?");

	اگر (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&دلائل);

	اگر (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_مقصد_hex(oid));
}

ساکن خالی describe(ثابت حرف *arg, صحیح آخری_one)
{
	ساخت object_id oid;
	ساخت commit *cmit;
	ساخت strbuf sb = STRBUF_INIT;

	اگر (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	اگر (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	اگر (cmit)
		describe_commit(cmit, &sb);
	ورنہ اگر (odb_read_object_info(the_repository->objects,
				      &oid, تہی) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	ورنہ
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	اگر (!آخری_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

ساکن صحیح option_parse_exact_مطابقت(ثابت ساخت option *opt, ثابت حرف *arg,
				    صحیح unset)
{
	صحیح *val = opt->قدر;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	واپس 0;
}

صحیح cmd_describe(صحیح argc,
		 ثابت حرف **argv,
		 ثابت حرف *سابقہ,
		 ساخت repository *repo غیر_مستعمل )
{
	ساخت refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	صحیح شامل_ہے = 0;
	ساخت option options[] = {
		OPT_BOOL(0, "contains",   &شامل_ہے, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &لیبل_فہرست, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &پہلا_والد, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, تہی,
			       N_("only output exact matches"),
			       PARSE_OPT_NOARG, option_parse_exact_مطابقت),
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
		OPT_انتہا(),
	};

	repo_config(the_repository, git_default_config, تہی);
	argc = parse_options(argc, argv, سابقہ, options, describe_usage, 0);
	اگر (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	اگر (max_candidates < 0)
		max_candidates = 0;
	ورنہ اگر (max_candidates > MAX_لیبل_فہرست)
		max_candidates = MAX_لیبل_فہرست;

	save_commit_بفر = 0;

	اگر (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	اگر (شامل_ہے) {
		ساخت string_list_شے *شے;
		ساخت strvec دلائل;
		ثابت حرف **argv_copy;
		صحیح ret;

		strvec_init(&دلائل);
		strvec_pushl(&دلائل, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     تہی);
		اگر (always)
			strvec_ڈالو(&دلائل, "--always");
		اگر (!all)
			strvec_ڈالو(&دلائل, "--tags");

		for_each_string_list_شے(شے, &patterns)
			strvec_pushf(&دلائل, "--refs=refs/tags/%s", شے->string);
		for_each_string_list_شے(شے, &exclude_patterns)
			strvec_pushf(&دلائل, "--exclude=refs/tags/%s", شے->string);

		اگر (all) {
			for_each_string_list_شے(شے, &patterns)
				strvec_pushf(&دلائل, "--refs=refs/heads/%s", شے->string);
			for_each_string_list_شے(شے, &exclude_patterns)
				strvec_pushf(&دلائل, "--exclude=refs/heads/%s", شے->string);
			for_each_string_list_شے(شے, &patterns)
				strvec_pushf(&دلائل, "--refs=refs/remotes/%s", شے->string);
			for_each_string_list_شے(شے, &exclude_patterns)
				strvec_pushf(&دلائل, "--exclude=refs/remotes/%s", شے->string);
		}

		اگر (argc)
			strvec_pushv(&دلائل, argv);
		ورنہ
			strvec_ڈالو(&دلائل, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, دلائل.nr + 1);
		برائے (سائز_t i = 0; i < دلائل.nr; i++)
			argv_copy[i] = دلائل.v[i];
		argv_copy[دلائل.nr] = تہی;

		ret = cmd_name_rev(دلائل.nr, argv_copy, سابقہ, the_repository);

		strvec_clear(&دلائل);
		آزاد(argv_copy);
		واپس ret;
	}

	اگر (!all)
		for_each_ref_opts.سابقہ = "refs/tags/";

	hashmap_init(&نام, commit_name_neq, تہی, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, تہی, &for_each_ref_opts);
	اگر (!hashmap_get_سائز(&نام) && !always)
		die(_("No names found, cannot describe anything."));

	اگر (argc == 0) {
		اگر (broken) {
			ساخت بچہ_process cp = بچہ_PROCESS_INIT;

			strvec_pushv(&cp.دلائل, update_اشاریہ_دلائل);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			تسلسل_command(&cp);

			بچہ_process_init(&cp);
			strvec_pushv(&cp.دلائل, diff_اشاریہ_دلائل);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			اگر (!dirty)
				dirty = "-dirty";

			انتخاب (تسلسل_command(&cp)) {
			حالت 0:
				suffix = تہی;
				توڑو;
			حالت 1:
				suffix = dirty;
				توڑو;
			طےشدہ:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} ورنہ اگر (dirty) {
			ساخت تالا_file اشاریہ_تالا = تالا_INIT;
			ساخت rev_info revs;
			صحیح fd;

			تیاری_work_درخت(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_بھرا_اشاریہ = 0;
			repo_read_اشاریہ(the_repository);
			refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET|REFRESH_UNMERGED,
				      تہی, تہی, تہی);
			fd = repo_hold_locked_اشاریہ(the_repository,
						    &اشاریہ_تالا, 0);
			اگر (0 <= fd)
				repo_update_اشاریہ_if_able(the_repository, &اشاریہ_تالا);

			repo_init_revisions(the_repository, &revs, سابقہ);

			اگر (تیاری_revisions(ARRAY_سائز(diff_اشاریہ_دلائل) - 1,
					    diff_اشاریہ_دلائل, &revs, تہی) != 1)
				BUG("malformed internal diff-index command line");
			تسلسل_diff_اشاریہ(&revs, 0);

			اگر (!diff_نتیجہ_رمز(&revs))
				suffix = تہی;
			ورنہ
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} ورنہ اگر (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} ورنہ اگر (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} ورنہ {
		جب (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	واپس 0;
}
