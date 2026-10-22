#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "hashmap.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "list-objects.h"
#अन्तर्भाव "commit-slab.h"
#अन्तर्भाव "wildmatch.h"
#अन्तर्भाव "prio-queue.h"
#अन्तर्भाव "oidset.h"

#परिभाषय MAX_चिह्नकानि	(FLAG_द्वयङ्काः - 1)
#परिभाषय DEFAULT_CANDIDATES 10

define_commit_slab(commit_नामानि, संरचना commit_name *);

स्थिर ध्रुव वर्ण * ध्रुव describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	रिक्त
};

स्थिर पूर्णाङ्क debug;	/* Display lots of verbose info */
स्थिर पूर्णाङ्क all;	/* Any valid ref can be used */
स्थिर पूर्णाङ्क चिह्नकानि;	/* Allow lightweight tags */
स्थिर पूर्णाङ्क longformat;
स्थिर पूर्णाङ्क first_parent;
स्थिर पूर्णाङ्क abbrev = -1; /* unspecified */
स्थिर पूर्णाङ्क max_candidates = DEFAULT_CANDIDATES;
स्थिर संरचना hashmap नामानि;
स्थिर पूर्णाङ्क have_util;
स्थिर संरचना string_list patterns = STRING_LIST_INIT_NODUP;
स्थिर संरचना string_list exclude_patterns = STRING_LIST_INIT_NODUP;
स्थिर पूर्णाङ्क always;
स्थिर ध्रुव वर्ण *suffix, *dirty, *broken;
स्थिर संरचना commit_नामानि commit_नामानि;

/* diff-index command arguments to check if working tree is dirty. */
स्थिर ध्रुव वर्ण *diff_सूचकांक_तर्काः[] = {
	"diff-index", "--quiet", "HEAD", "--", रिक्त
};

स्थिर ध्रुव वर्ण *update_सूचकांक_तर्काः[] = {
	"update-index", "--unmerged", "-q", "--refresh", रिक्त
};

संरचना commit_name {
	संरचना hashmap_प्रविष्टिः प्रविष्टिः;
	संरचना object_id peeled;
	संरचना चिह्नकम् *चिह्नकम्;
	अचिह्नित prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	अचिह्नित name_checked:1;
	अचिह्नित misnamed:1;
	संरचना object_id oid;
	वर्ण *मार्गः;
};

स्थिर ध्रुव वर्ण *prio_नामानि[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

स्थिर पूर्णाङ्क commit_name_neq(ध्रुव शून्य *cmp_दत्त अप्रयुक्तम्,
			   ध्रुव संरचना hashmap_प्रविष्टिः *eptr,
			   ध्रुव संरचना hashmap_प्रविष्टिः *प्रविष्टिः_or_कुञ्चिका,
			   ध्रुव शून्य *peeled)
{
	ध्रुव संरचना commit_name *cn1, *cn2;

	cn1 = container_सम्बन्धि(eptr, ध्रुव संरचना commit_name, प्रविष्टिः);
	cn2 = container_सम्बन्धि(प्रविष्टिः_or_कुञ्चिका, ध्रुव संरचना commit_name, प्रविष्टिः);

	प्रत्यागम !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

स्थिर अन्तःस्थ संरचना commit_name *find_commit_name(ध्रुव संरचना object_id *peeled)
{
	प्रत्यागम hashmap_get_प्रविष्टिः_from_hash(&नामानि, oidhash(peeled), peeled,
						संरचना commit_name, प्रविष्टिः);
}

स्थिर पूर्णाङ्क replace_name(संरचना commit_name *e,
			       पूर्णाङ्क prio,
			       ध्रुव संरचना object_id *oid,
			       संरचना चिह्नकम् **चिह्नकम्)
{
	यदि (!e || e->prio < prio)
		प्रत्यागम 1;

	यदि (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		संरचना चिह्नकम् *t;

		यदि (!e->चिह्नकम्) {
			t = lookup_चिह्नकम्(the_repository, &e->oid);
			यदि (!t || parse_चिह्नकम्(the_repository, t))
				प्रत्यागम 1;
			e->चिह्नकम् = t;
		}

		t = lookup_चिह्नकम्(the_repository, oid);
		यदि (!t || parse_चिह्नकम्(the_repository, t))
			प्रत्यागम 0;
		*चिह्नकम् = t;

		यदि (e->चिह्नकम्->date < t->date)
			प्रत्यागम 1;
	}

	प्रत्यागम 0;
}

स्थिर शून्य सङ्कलय_गन्तव्यम्_ज्ञातम्_नामानि(ध्रुव वर्ण *मार्गः,
			       ध्रुव संरचना object_id *peeled,
			       पूर्णाङ्क prio,
			       ध्रुव संरचना object_id *oid)
{
	संरचना commit_name *e = find_commit_name(peeled);
	संरचना चिह्नकम् *चिह्नकम् = रिक्त;
	यदि (replace_name(e, prio, oid, &चिह्नकम्)) {
		यदि (!e) {
			e = xmalloc(परिमाण(संरचना commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_प्रविष्टिः_init(&e->प्रविष्टिः, oidhash(peeled));
			hashmap_सङ्कलय(&नामानि, &e->प्रविष्टिः);
			e->मार्गः = रिक्त;
		}
		e->चिह्नकम् = चिह्नकम्;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		मुक्त(e->मार्गः);
		e->मार्गः = xstrdup(मार्गः);
	}
}

स्थिर पूर्णाङ्क get_name(ध्रुव संरचना reference *ref, शून्य *cb_दत्त अप्रयुक्तम्)
{
	पूर्णाङ्क is_चिह्नकम् = 0;
	संरचना object_id peeled;
	पूर्णाङ्क is_annotated, prio;
	ध्रुव वर्ण *मार्गः_गन्तव्यम्_मेलनम् = रिक्त;

	यदि (skip_उपसर्गः(ref->name, "refs/tags/", &मार्गः_गन्तव्यम्_मेलनम्)) {
		is_चिह्नकम् = 1;
	} अन्यथा यदि (all) {
		यदि ((exclude_patterns.nr || patterns.nr) &&
		    !skip_उपसर्गः(ref->name, "refs/heads/", &मार्गः_गन्तव्यम्_मेलनम्) &&
		    !skip_उपसर्गः(ref->name, "refs/remotes/", &मार्गः_गन्तव्यम्_मेलनम्)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			प्रत्यागम 0;
		}
	} अन्यथा {
		/* Reject anything outside refs/tags/ unless --all */
		प्रत्यागम 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	यदि (exclude_patterns.nr) {
		संरचना string_list_item *item;

		for_each_string_list_item(item, &exclude_patterns) {
			यदि (!wildmatch(item->string, मार्गः_गन्तव्यम्_मेलनम्, 0))
				प्रत्यागम 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	यदि (patterns.nr) {
		पूर्णाङ्क प्राप्तम्_2 = 0;
		संरचना string_list_item *item;

		for_each_string_list_item(item, &patterns) {
			यदि (!wildmatch(item->string, मार्गः_गन्तव्यम्_मेलनम्, 0)) {
				प्राप्तम्_2 = 1;
				भङ्ग;
			}
		}

		यदि (!प्राप्तम्_2)
			प्रत्यागम 0;
	}

	/* Is it annotated? */
	यदि (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} अन्यथा {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	यदि (is_annotated)
		prio = 2;
	अन्यथा यदि (is_चिह्नकम्)
		prio = 1;
	अन्यथा
		prio = 0;

	सङ्कलय_गन्तव्यम्_ज्ञातम्_नामानि(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	प्रत्यागम 0;
}

संरचना possible_चिह्नकम् {
	संरचना commit_name *name;
	पूर्णाङ्क गभीरता;
	पूर्णाङ्क प्राप्तम्_क्रमः;
	अचिह्नित flag_within;
};

स्थिर पूर्णाङ्क compare_pt(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	संरचना possible_चिह्नकम् *a = (संरचना possible_चिह्नकम् *)a_;
	संरचना possible_चिह्नकम् *b = (संरचना possible_चिह्नकम् *)b_;
	यदि (a->गभीरता != b->गभीरता)
		प्रत्यागम a->गभीरता - b->गभीरता;
	यदि (a->प्राप्तम्_क्रमः != b->प्राप्तम्_क्रमः)
		प्रत्यागम a->प्राप्तम्_क्रमः - b->प्राप्तम्_क्रमः;
	प्रत्यागम 0;
}

स्थिर अचिह्नित दीर्घ finish_गभीरता_computation(संरचना prio_queue *queue,
					      संरचना possible_चिह्नकम् *best)
{
	अचिह्नित दीर्घ आलोकितम्_commits = 0;
	संरचना oidset unflagged = OIDSET_INIT;
	संरचना commit *c;

	prio_queue_for_each(queue, c) {
		यदि (!(c->object.flags & best->flag_within))
			oidset_निवेशय(&unflagged, &c->object.oid);
	}

	यावत् ((c = prio_queue_get(queue))) {
		संरचना commit_list *parents = c->parents;
		आलोकितम्_commits++;
		यदि (c->object.flags & best->flag_within) {
			यदि (!oidset_आकार(&unflagged))
				भङ्ग;
		} अन्यथा {
			oidset_remove(&unflagged, &c->object.oid);
			best->गभीरता++;
		}
		यावत् (parents) {
			अचिह्नित आलोकितम्, flag_before, flag_after;
			संरचना commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			आलोकितम् = p->object.flags & आलोकितम्_2;
			यदि (!आलोकितम्)
				prio_queue_put(queue, p);
			flag_before = p->object.flags & best->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & best->flag_within;
			यदि (!आलोकितम् && !flag_after)
				oidset_निवेशय(&unflagged, &p->object.oid);
			यदि (आलोकितम् && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->अग्र;
		}
	}
	oidset_clear(&unflagged);
	प्रत्यागम आलोकितम्_commits;
}

स्थिर शून्य append_name(संरचना commit_name *n, संरचना strbuf *dst)
{
	यदि (n->prio == 2 && !n->चिह्नकम्) {
		n->चिह्नकम् = lookup_चिह्नकम्(the_repository, &n->oid);
		यदि (!n->चिह्नकम् || parse_चिह्नकम्(the_repository, n->चिह्नकम्))
			die(_("annotated tag %s not available"), n->मार्गः);
	}
	यदि (n->चिह्नकम् && !n->name_checked) {
		यदि (वाक्यं_तुलय(n->चिह्नकम्->चिह्नकम्, all ? n->मार्गः + 5 : n->मार्गः)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->मार्गः, n->चिह्नकम्->चिह्नकम्);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	यदि (n->चिह्नकम्) {
		यदि (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->चिह्नकम्->चिह्नकम्);
	} अन्यथा {
		strbuf_addstr(dst, n->मार्गः);
	}
}

स्थिर शून्य append_suffix(पूर्णाङ्क गभीरता, ध्रुव संरचना object_id *oid, संरचना strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", गभीरता,
		    repo_find_unique_abbrev(the_repository, oid, abbrev));
}

स्थिर शून्य describe_commit(संरचना commit *cmit, संरचना strbuf *dst)
{
	संरचना commit *c, *gave_ऊर्ध्वम्_on = रिक्त;
	संरचना prio_queue queue = { compare_commits_by_commit_date };
	संरचना commit_name *n;
	संरचना possible_चिह्नकम् all_matches[MAX_चिह्नकानि];
	अचिह्नित पूर्णाङ्क मेलनम्_cnt = 0, annotated_cnt = 0, cur_मेलनम्;
	अचिह्नित दीर्घ आलोकितम्_commits = 0;
	अचिह्नित पूर्णाङ्क unannotated_cnt = 0;

	n = find_commit_name(&cmit->object.oid);
	यदि (n && (चिह्नकानि || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		यदि (n->misnamed || longformat)
			append_suffix(0, n->चिह्नकम् ? get_tagged_oid(n->चिह्नकम्) : &cmit->object.oid, dst);
		यदि (suffix)
			strbuf_addstr(dst, suffix);
		प्रत्यागम;
	}

	यदि (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_गन्तव्यम्_hex(&cmit->object.oid));
	यदि (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	यदि (!have_util) {
		संरचना hashmap_iter iter;
		संरचना commit *c;
		संरचना commit_name *n;

		init_commit_नामानि(&commit_नामानि);
		hashmap_for_each_प्रविष्टिः(&नामानि, &iter, n,
					प्रविष्टिः /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			यदि (c)
				*commit_नामानि_at(&commit_नामानि, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = आलोकितम्_2;
	prio_queue_put(&queue, cmit);
	यावत् ((c = prio_queue_get(&queue))) {
		संरचना commit_list *parents = c->parents;
		संरचना commit_name **slot;

		आलोकितम्_commits++;

		यदि (मेलनम्_cnt == max_candidates ||
		    मेलनम्_cnt == hashmap_get_आकार(&नामानि)) {
			gave_ऊर्ध्वम्_on = c;
			भङ्ग;
		}

		slot = commit_नामानि_peek(&commit_नामानि, c);
		n = slot ? *slot : रिक्त;
		यदि (n) {
			यदि (!चिह्नकानि && !all && n->prio < 2) {
				unannotated_cnt++;
			} अन्यथा यदि (मेलनम्_cnt < max_candidates) {
				संरचना possible_चिह्नकम् *t = &all_matches[मेलनम्_cnt++];
				t->name = n;
				t->गभीरता = आलोकितम्_commits - 1;
				t->flag_within = 1u << मेलनम्_cnt;
				t->प्राप्तम्_क्रमः = मेलनम्_cnt;
				c->object.flags |= t->flag_within;
				यदि (n->prio == 2)
					annotated_cnt++;
			}
		}
		प्रति (cur_मेलनम् = 0; cur_मेलनम् < मेलनम्_cnt; cur_मेलनम्++) {
			संरचना possible_चिह्नकम् *t = &all_matches[cur_मेलनम्];
			यदि (!(c->object.flags & t->flag_within))
				t->गभीरता++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		यदि (annotated_cnt && !prio_queue_आकार(&queue)) {
			पूर्णाङ्क best_गभीरता = अधिकतम_पूर्णाङ्कः;
			अचिह्नित best_within = 0;
			प्रति (cur_मेलनम् = 0; cur_मेलनम् < मेलनम्_cnt; cur_मेलनम्++) {
				संरचना possible_चिह्नकम् *t = &all_matches[cur_मेलनम्];
				यदि (t->गभीरता < best_गभीरता) {
					best_गभीरता = t->गभीरता;
					best_within = t->flag_within;
				} अन्यथा यदि (t->गभीरता == best_गभीरता) {
					best_within |= t->flag_within;
				}
			}
			यदि ((c->object.flags & best_within) == best_within) {
				यदि (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_गन्तव्यम्_hex(&c->object.oid));
				भङ्ग;
			}
		}
		यावत् (parents) {
			संरचना commit *p = parents->item;
			repo_parse_commit(the_repository, p);
			यदि (!(p->object.flags & आलोकितम्_2))
				prio_queue_put(&queue, p);
			p->object.flags |= c->object.flags;
			parents = parents->अग्र;

			यदि (first_parent)
				भङ्ग;
		}
	}

	यदि (!मेलनम्_cnt) {
		संरचना object_id *cmit_oid = &cmit->object.oid;
		यदि (always) {
			strbuf_सङ्कलय_unique_abbrev(dst, cmit_oid, abbrev);
			यदि (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_queue(&queue);
			प्रत्यागम;
		}
		यदि (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_गन्तव्यम्_hex(cmit_oid));
		अन्यथा
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_गन्तव्यम्_hex(cmit_oid));
	}

	QSORT(all_matches, मेलनम्_cnt, compare_pt);

	यदि (gave_ऊर्ध्वम्_on) {
		prio_queue_put(&queue, gave_ऊर्ध्वम्_on);
		आलोकितम्_commits--;
	}
	आलोकितम्_commits += finish_गभीरता_computation(&queue, &all_matches[0]);
	clear_prio_queue(&queue);

	यदि (debug) {
		स्थिर पूर्णाङ्क label_विस्तारः = -1;
		यदि (label_विस्तारः < 0) {
			पूर्णाङ्क i, w;
			प्रति (i = 0; i < ARRAY_आकार(prio_नामानि); i++) {
				w = वाक्यदैर्घ्यम्(_(prio_नामानि[i]));
				यदि (label_विस्तारः < w)
					label_विस्तारः = w;
			}
		}
		प्रति (cur_मेलनम् = 0; cur_मेलनम् < मेलनम्_cnt; cur_मेलनम्++) {
			संरचना possible_चिह्नकम् *t = &all_matches[cur_मेलनम्];
			fprintf(stderr, " %-*s %8d %s\n",
				label_विस्तारः, _(prio_नामानि[t->name->prio]),
				t->गभीरता, t->name->मार्गः);
		}
		fprintf(stderr, _("traversed %lu commits\n"), आलोकितम्_commits);
		यदि (gave_ऊर्ध्वम्_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_गन्तव्यम्_hex(&gave_ऊर्ध्वम्_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	यदि (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].गभीरता, &cmit->object.oid, dst);
	यदि (suffix)
		strbuf_addstr(dst, suffix);
}

संरचना process_commit_दत्त {
	संरचना commit *current_commit;
	ध्रुव संरचना object_id *looking_for;
	संरचना strbuf *dst;
	संरचना rev_info *revs;
};

स्थिर शून्य process_commit(संरचना commit *commit, शून्य *दत्त)
{
	संरचना process_commit_दत्त *pcd = दत्त;
	pcd->current_commit = commit;
}

स्थिर शून्य process_object(संरचना object *obj, ध्रुव वर्ण *मार्गः, शून्य *दत्त)
{
	संरचना process_commit_दत्त *pcd = दत्त;

	यदि (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		यदि (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", मार्गः);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = रिक्त;
	}
}

स्थिर शून्य describe_blob(ध्रुव संरचना object_id *oid, संरचना strbuf *dst)
{
	संरचना rev_info revs;
	संरचना strvec तर्काः = STRVEC_INIT;
	संरचना object_id शीर्ष_oid;
	संरचना process_commit_दत्त pcd = { रिक्त, oid, dst, &revs};

	यदि (repo_get_oid(the_repository, "HEAD", &शीर्ष_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_गन्तव्यम्_hex(oid));

	strvec_pushl(&तर्काः, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_गन्तव्यम्_hex(&शीर्ष_oid),
		     रिक्त);

	repo_init_revisions(the_repository, &revs, रिक्त);
	सज्जता_revisions_from_strvec(&तर्काः, &revs, रिक्त);
	यदि (तर्काः.nr > 1)
		BUG("setup_revisions could not handle all args?");

	यदि (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&तर्काः);

	यदि (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_गन्तव्यम्_hex(oid));
}

स्थिर शून्य describe(ध्रुव वर्ण *arg, पूर्णाङ्क चरमः_one)
{
	संरचना object_id oid;
	संरचना commit *cmit;
	संरचना strbuf sb = STRBUF_INIT;

	यदि (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	यदि (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	यदि (cmit)
		describe_commit(cmit, &sb);
	अन्यथा यदि (odb_read_object_info(the_repository->objects,
				      &oid, रिक्त) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	अन्यथा
		die(_("%s is neither a commit nor blob"), arg);

	पङ्क्तिं_लिख(sb.buf);

	यदि (!चरमः_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

स्थिर पूर्णाङ्क option_parse_exact_मेलनम्(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
				    पूर्णाङ्क unset)
{
	पूर्णाङ्क *val = opt->मानम्;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_describe(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *उपसर्गः,
		 संरचना repository *repo अप्रयुक्तम् )
{
	संरचना refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	पूर्णाङ्क अन्तर्भवति = 0;
	संरचना option options[] = {
		OPT_BOOL(0, "contains",   &अन्तर्भवति, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &चिह्नकानि, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &first_parent, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, रिक्त,
			       N_("only output exact matches"),
			       PARSE_OPT_NOARG, option_parse_exact_मेलनम्),
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
			.मानम् = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.मानम् = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_अन्तः(),
	};

	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, उपसर्गः, options, describe_usage, 0);
	यदि (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	यदि (max_candidates < 0)
		max_candidates = 0;
	अन्यथा यदि (max_candidates > MAX_चिह्नकानि)
		max_candidates = MAX_चिह्नकानि;

	save_commit_बफर = 0;

	यदि (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	यदि (अन्तर्भवति) {
		संरचना string_list_item *item;
		संरचना strvec तर्काः;
		ध्रुव वर्ण **argv_copy;
		पूर्णाङ्क ret;

		strvec_init(&तर्काः);
		strvec_pushl(&तर्काः, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     रिक्त);
		यदि (always)
			strvec_प्रक्षिप(&तर्काः, "--always");
		यदि (!all)
			strvec_प्रक्षिप(&तर्काः, "--tags");

		for_each_string_list_item(item, &patterns)
			strvec_pushf(&तर्काः, "--refs=refs/tags/%s", item->string);
		for_each_string_list_item(item, &exclude_patterns)
			strvec_pushf(&तर्काः, "--exclude=refs/tags/%s", item->string);

		यदि (all) {
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&तर्काः, "--refs=refs/heads/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&तर्काः, "--exclude=refs/heads/%s", item->string);
			for_each_string_list_item(item, &patterns)
				strvec_pushf(&तर्काः, "--refs=refs/remotes/%s", item->string);
			for_each_string_list_item(item, &exclude_patterns)
				strvec_pushf(&तर्काः, "--exclude=refs/remotes/%s", item->string);
		}

		यदि (argc)
			strvec_pushv(&तर्काः, argv);
		अन्यथा
			strvec_प्रक्षिप(&तर्काः, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, तर्काः.nr + 1);
		प्रति (आकार_t i = 0; i < तर्काः.nr; i++)
			argv_copy[i] = तर्काः.v[i];
		argv_copy[तर्काः.nr] = रिक्त;

		ret = cmd_name_rev(तर्काः.nr, argv_copy, उपसर्गः, the_repository);

		strvec_clear(&तर्काः);
		मुक्त(argv_copy);
		प्रत्यागम ret;
	}

	यदि (!all)
		for_each_ref_opts.उपसर्गः = "refs/tags/";

	hashmap_init(&नामानि, commit_name_neq, रिक्त, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, रिक्त, &for_each_ref_opts);
	यदि (!hashmap_get_आकार(&नामानि) && !always)
		die(_("No names found, cannot describe anything."));

	यदि (argc == 0) {
		यदि (broken) {
			संरचना child_process cp = CHILD_PROCESS_INIT;

			strvec_pushv(&cp.तर्काः, update_सूचकांक_तर्काः);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			सततम्_command(&cp);

			child_process_init(&cp);
			strvec_pushv(&cp.तर्काः, diff_सूचकांक_तर्काः);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			यदि (!dirty)
				dirty = "-dirty";

			विकल्प (सततम्_command(&cp)) {
			पक्ष 0:
				suffix = रिक्त;
				भङ्ग;
			पक्ष 1:
				suffix = dirty;
				भङ्ग;
			स्वभाव:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} अन्यथा यदि (dirty) {
			संरचना तालकम्_file सूचकांक_तालकम् = तालकम्_INIT;
			संरचना rev_info revs;
			पूर्णाङ्क fd;

			सज्जता_work_वृक्षः(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;
			repo_read_सूचकांक(the_repository);
			refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET|REFRESH_UNMERGED,
				      रिक्त, रिक्त, रिक्त);
			fd = repo_hold_locked_सूचकांक(the_repository,
						    &सूचकांक_तालकम्, 0);
			यदि (0 <= fd)
				repo_update_सूचकांक_if_able(the_repository, &सूचकांक_तालकम्);

			repo_init_revisions(the_repository, &revs, उपसर्गः);

			यदि (सज्जता_revisions(ARRAY_आकार(diff_सूचकांक_तर्काः) - 1,
					    diff_सूचकांक_तर्काः, &revs, रिक्त) != 1)
				BUG("malformed internal diff-index command line");
			सततम्_diff_सूचकांक(&revs, 0);

			यदि (!diff_परिणाम_संहिता(&revs))
				suffix = रिक्त;
			अन्यथा
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} अन्यथा यदि (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} अन्यथा यदि (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} अन्यथा {
		यावत् (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	प्रत्यागम 0;
}
