#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "hashmap.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "list-objects.h"
#ಅನ್ತರ್ಭಾವ "commit-slab.h"
#ಅನ್ತರ್ಭಾವ "wildmatch.h"
#ಅನ್ತರ್ಭಾವ "prio-queue.h"
#ಅನ್ತರ್ಭಾವ "oidset.h"

#ವ್ಯಾಖ್ಯಾನಿಸು MAX_ಟ್ಯಾಗುಗಳು	(FLAG_ಬಿಟ್ಟುಗಳು - 1)
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_CANDIDATES 10

define_commit_slab(commit_ಹೆಸರುಗಳು, ರಚನೆ commit_name *);

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ debug;	/* Display lots of verbose info */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ all;	/* Any valid ref can be used */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗುಗಳು;	/* Allow lightweight tags */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ longformat;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮೊದಲ_ಪೋಷಕ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ abbrev = -1; /* unspecified */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ max_candidates = DEFAULT_CANDIDATES;
ಸ್ಥಿರ ರಚನೆ hashmap ಹೆಸರುಗಳು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ have_util;
ಸ್ಥಿರ ರಚನೆ string_list patterns = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ರಚನೆ string_list exclude_patterns = STRING_LIST_INIT_NODUP;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ always;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix, *dirty, *broken;
ಸ್ಥಿರ ರಚನೆ commit_ಹೆಸರುಗಳು commit_ಹೆಸರುಗಳು;

/* diff-index command arguments to check if working tree is dirty. */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *diff_ಸೂಚ್ಯಂಕ_ವಾದಗಳು[] = {
	"diff-index", "--quiet", "HEAD", "--", ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *update_ಸೂಚ್ಯಂಕ_ವಾದಗಳು[] = {
	"update-index", "--unmerged", "-q", "--refresh", ಖಾಲಿ
};

ರಚನೆ commit_name {
	ರಚನೆ hashmap_ನಮೂದು ನಮೂದು;
	ರಚನೆ object_id peeled;
	ರಚನೆ ಟ್ಯಾಗ್ *ಟ್ಯಾಗ್;
	ಚಿಹ್ನೆರಹಿತ prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	ಚಿಹ್ನೆರಹಿತ name_checked:1;
	ಚಿಹ್ನೆರಹಿತ misnamed:1;
	ರಚನೆ object_id oid;
	ಅಕ್ಷರ *ಮಾರ್ಗ;
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *prio_ಹೆಸರುಗಳು[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ commit_name_neq(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *cmp_ದತ್ತಾಂಶ ಬಳಸದ,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *eptr,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ hashmap_ನಮೂದು *ನಮೂದು_or_ಕೀಲಿ,
			   ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *peeled)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ commit_name *cn1, *cn2;

	cn1 = container_ದ(eptr, ಸ್ಥಿರಾಂಕ ರಚನೆ commit_name, ನಮೂದು);
	cn2 = container_ದ(ನಮೂದು_or_ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ರಚನೆ commit_name, ನಮೂದು);

	ಹಿಂತಿರುಗಿ !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ commit_name *ಕಂಡುಹಿಡಿ_commit_name(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *peeled)
{
	ಹಿಂತಿರುಗಿ hashmap_get_ನಮೂದು_from_hash(&ಹೆಸರುಗಳು, oidhash(peeled), peeled,
						ರಚನೆ commit_name, ನಮೂದು);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ replace_name(ರಚನೆ commit_name *e,
			       ಪೂರ್ಣಾಂಕ prio,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid,
			       ರಚನೆ ಟ್ಯಾಗ್ **ಟ್ಯಾಗ್)
{
	ಆದರೆ (!e || e->prio < prio)
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		ರಚನೆ ಟ್ಯಾಗ್ *t;

		ಆದರೆ (!e->ಟ್ಯಾಗ್) {
			t = lookup_ಟ್ಯಾಗ್(the_repository, &e->oid);
			ಆದರೆ (!t || parse_ಟ್ಯಾಗ್(the_repository, t))
				ಹಿಂತಿರುಗಿ 1;
			e->ಟ್ಯಾಗ್ = t;
		}

		t = lookup_ಟ್ಯಾಗ್(the_repository, oid);
		ಆದರೆ (!t || parse_ಟ್ಯಾಗ್(the_repository, t))
			ಹಿಂತಿರುಗಿ 0;
		*ಟ್ಯಾಗ್ = t;

		ಆದರೆ (e->ಟ್ಯಾಗ್->date < t->date)
			ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_ಗಮ್ಯ_ತಿಳಿದ_ಹೆಸರುಗಳು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *peeled,
			       ಪೂರ್ಣಾಂಕ prio,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ರಚನೆ commit_name *e = ಕಂಡುಹಿಡಿ_commit_name(peeled);
	ರಚನೆ ಟ್ಯಾಗ್ *ಟ್ಯಾಗ್ = ಖಾಲಿ;
	ಆದರೆ (replace_name(e, prio, oid, &ಟ್ಯಾಗ್)) {
		ಆದರೆ (!e) {
			e = xmalloc(ಗಾತ್ರ(ರಚನೆ commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_ನಮೂದು_init(&e->ನಮೂದು, oidhash(peeled));
			hashmap_ಕೂಡಿಸು(&ಹೆಸರುಗಳು, &e->ನಮೂದು);
			e->ಮಾರ್ಗ = ಖಾಲಿ;
		}
		e->ಟ್ಯಾಗ್ = ಟ್ಯಾಗ್;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		ಬಿಡುಗಡೆ(e->ಮಾರ್ಗ);
		e->ಮಾರ್ಗ = xstrdup(ಮಾರ್ಗ);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_name(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ is_ಟ್ಯಾಗ್ = 0;
	ರಚನೆ object_id peeled;
	ಪೂರ್ಣಾಂಕ is_annotated, prio;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ_ಗಮ್ಯ_ಹೊಂದಿಕೆ = ಖಾಲಿ;

	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ref->name, "refs/tags/", &ಮಾರ್ಗ_ಗಮ್ಯ_ಹೊಂದಿಕೆ)) {
		is_ಟ್ಯಾಗ್ = 1;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (all) {
		ಆದರೆ ((exclude_patterns.nr || patterns.nr) &&
		    !skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ref->name, "refs/heads/", &ಮಾರ್ಗ_ಗಮ್ಯ_ಹೊಂದಿಕೆ) &&
		    !skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ref->name, "refs/remotes/", &ಮಾರ್ಗ_ಗಮ್ಯ_ಹೊಂದಿಕೆ)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			ಹಿಂತಿರುಗಿ 0;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* Reject anything outside refs/tags/ unless --all */
		ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	ಆದರೆ (exclude_patterns.nr) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

		for_each_string_list_ಅಂಶ(ಅಂಶ, &exclude_patterns) {
			ಆದರೆ (!wildmatch(ಅಂಶ->string, ಮಾರ್ಗ_ಗಮ್ಯ_ಹೊಂದಿಕೆ, 0))
				ಹಿಂತಿರುಗಿ 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	ಆದರೆ (patterns.nr) {
		ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು_2 = 0;
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

		for_each_string_list_ಅಂಶ(ಅಂಶ, &patterns) {
			ಆದರೆ (!wildmatch(ಅಂಶ->string, ಮಾರ್ಗ_ಗಮ್ಯ_ಹೊಂದಿಕೆ, 0)) {
				ಸಿಕ್ಕಿತು_2 = 1;
				ನಿಲ್ಲಿಸು;
			}
		}

		ಆದರೆ (!ಸಿಕ್ಕಿತು_2)
			ಹಿಂತಿರುಗಿ 0;
	}

	/* Is it annotated? */
	ಆದರೆ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} ಇಲ್ಲದಿದ್ದರೆ {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	ಆದರೆ (is_annotated)
		prio = 2;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_ಟ್ಯಾಗ್)
		prio = 1;
	ಇಲ್ಲದಿದ್ದರೆ
		prio = 0;

	ಕೂಡಿಸು_ಗಮ್ಯ_ತಿಳಿದ_ಹೆಸರುಗಳು(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ possible_ಟ್ಯಾಗ್ {
	ರಚನೆ commit_name *name;
	ಪೂರ್ಣಾಂಕ ಆಳ;
	ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು_ಕ್ರಮ;
	ಚಿಹ್ನೆರಹಿತ flag_within;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compare_pt(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a_, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b_)
{
	ರಚನೆ possible_ಟ್ಯಾಗ್ *a = (ರಚನೆ possible_ಟ್ಯಾಗ್ *)a_;
	ರಚನೆ possible_ಟ್ಯಾಗ್ *b = (ರಚನೆ possible_ಟ್ಯಾಗ್ *)b_;
	ಆದರೆ (a->ಆಳ != b->ಆಳ)
		ಹಿಂತಿರುಗಿ a->ಆಳ - b->ಆಳ;
	ಆದರೆ (a->ಸಿಕ್ಕಿತು_ಕ್ರಮ != b->ಸಿಕ್ಕಿತು_ಕ್ರಮ)
		ಹಿಂತಿರುಗಿ a->ಸಿಕ್ಕಿತು_ಕ್ರಮ - b->ಸಿಕ್ಕಿತು_ಕ್ರಮ;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಕ್ತಾಯ_ಆಳ_computation(ರಚನೆ prio_ಸರತಿ *ಸರತಿ,
					      ರಚನೆ possible_ಟ್ಯಾಗ್ *ಉತ್ತಮ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ನೋಡಿದ_commits = 0;
	ರಚನೆ oidset unflagged = OIDSET_INIT;
	ರಚನೆ commit *c;

	prio_ಸರತಿ_for_each(ಸರತಿ, c) {
		ಆದರೆ (!(c->object.flags & ಉತ್ತಮ->flag_within))
			oidset_ಸೇರಿಸು(&unflagged, &c->object.oid);
	}

	ಆಗ ((c = prio_ಸರತಿ_get(ಸರತಿ))) {
		ರಚನೆ commit_list *parents = c->parents;
		ನೋಡಿದ_commits++;
		ಆದರೆ (c->object.flags & ಉತ್ತಮ->flag_within) {
			ಆದರೆ (!oidset_ಗಾತ್ರ(&unflagged))
				ನಿಲ್ಲಿಸು;
		} ಇಲ್ಲದಿದ್ದರೆ {
			oidset_remove(&unflagged, &c->object.oid);
			ಉತ್ತಮ->ಆಳ++;
		}
		ಆಗ (parents) {
			ಚಿಹ್ನೆರಹಿತ ನೋಡಿದ, flag_before, flag_after;
			ರಚನೆ commit *p = parents->ಅಂಶ;
			repo_parse_commit(the_repository, p);
			ನೋಡಿದ = p->object.flags & ನೋಡಿದ_2;
			ಆದರೆ (!ನೋಡಿದ)
				prio_ಸರತಿ_put(ಸರತಿ, p);
			flag_before = p->object.flags & ಉತ್ತಮ->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & ಉತ್ತಮ->flag_within;
			ಆದರೆ (!ನೋಡಿದ && !flag_after)
				oidset_ಸೇರಿಸು(&unflagged, &p->object.oid);
			ಆದರೆ (ನೋಡಿದ && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->ಮುಂದಿನ;
		}
	}
	oidset_clear(&unflagged);
	ಹಿಂತಿರುಗಿ ನೋಡಿದ_commits;
}

ಸ್ಥಿರ ಶೂನ್ಯ append_name(ರಚನೆ commit_name *n, ರಚನೆ strbuf *dst)
{
	ಆದರೆ (n->prio == 2 && !n->ಟ್ಯಾಗ್) {
		n->ಟ್ಯಾಗ್ = lookup_ಟ್ಯಾಗ್(the_repository, &n->oid);
		ಆದರೆ (!n->ಟ್ಯಾಗ್ || parse_ಟ್ಯಾಗ್(the_repository, n->ಟ್ಯಾಗ್))
			die(_("annotated tag %s not available"), n->ಮಾರ್ಗ);
	}
	ಆದರೆ (n->ಟ್ಯಾಗ್ && !n->name_checked) {
		ಆದರೆ (strcmp(n->ಟ್ಯಾಗ್->ಟ್ಯಾಗ್, all ? n->ಮಾರ್ಗ + 5 : n->ಮಾರ್ಗ)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->ಮಾರ್ಗ, n->ಟ್ಯಾಗ್->ಟ್ಯಾಗ್);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	ಆದರೆ (n->ಟ್ಯಾಗ್) {
		ಆದರೆ (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->ಟ್ಯಾಗ್->ಟ್ಯಾಗ್);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addstr(dst, n->ಮಾರ್ಗ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ append_suffix(ಪೂರ್ಣಾಂಕ ಆಳ, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", ಆಳ,
		    repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, oid, abbrev));
}

ಸ್ಥಿರ ಶೂನ್ಯ describe_commit(ರಚನೆ commit *cmit, ರಚನೆ strbuf *dst)
{
	ರಚನೆ commit *c, *gave_ಮೇಲೆ_on = ಖಾಲಿ;
	ರಚನೆ prio_ಸರತಿ ಸರತಿ = { compare_commits_by_commit_date };
	ರಚನೆ commit_name *n;
	ರಚನೆ possible_ಟ್ಯಾಗ್ all_matches[MAX_ಟ್ಯಾಗುಗಳು];
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಹೊಂದಿಕೆ_cnt = 0, annotated_cnt = 0, cur_ಹೊಂದಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ನೋಡಿದ_commits = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ unannotated_cnt = 0;

	n = ಕಂಡುಹಿಡಿ_commit_name(&cmit->object.oid);
	ಆದರೆ (n && (ಟ್ಯಾಗುಗಳು || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		ಆದರೆ (n->misnamed || longformat)
			append_suffix(0, n->ಟ್ಯಾಗ್ ? get_tagged_oid(n->ಟ್ಯಾಗ್) : &cmit->object.oid, dst);
		ಆದರೆ (suffix)
			strbuf_addstr(dst, suffix);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_ಗಮ್ಯ_hex(&cmit->object.oid));
	ಆದರೆ (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	ಆದರೆ (!have_util) {
		ರಚನೆ hashmap_iter iter;
		ರಚನೆ commit *c;
		ರಚನೆ commit_name *n;

		init_commit_ಹೆಸರುಗಳು(&commit_ಹೆಸರುಗಳು);
		hashmap_for_each_ನಮೂದು(&ಹೆಸರುಗಳು, &iter, n,
					ನಮೂದು /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			ಆದರೆ (c)
				*commit_ಹೆಸರುಗಳು_at(&commit_ಹೆಸರುಗಳು, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = ನೋಡಿದ_2;
	prio_ಸರತಿ_put(&ಸರತಿ, cmit);
	ಆಗ ((c = prio_ಸರತಿ_get(&ಸರತಿ))) {
		ರಚನೆ commit_list *parents = c->parents;
		ರಚನೆ commit_name **ಸ್ಥಾನ;

		ನೋಡಿದ_commits++;

		ಆದರೆ (ಹೊಂದಿಕೆ_cnt == max_candidates ||
		    ಹೊಂದಿಕೆ_cnt == hashmap_get_ಗಾತ್ರ(&ಹೆಸರುಗಳು)) {
			gave_ಮೇಲೆ_on = c;
			ನಿಲ್ಲಿಸು;
		}

		ಸ್ಥಾನ = commit_ಹೆಸರುಗಳು_peek(&commit_ಹೆಸರುಗಳು, c);
		n = ಸ್ಥಾನ ? *ಸ್ಥಾನ : ಖಾಲಿ;
		ಆದರೆ (n) {
			ಆದರೆ (!ಟ್ಯಾಗುಗಳು && !all && n->prio < 2) {
				unannotated_cnt++;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಹೊಂದಿಕೆ_cnt < max_candidates) {
				ರಚನೆ possible_ಟ್ಯಾಗ್ *t = &all_matches[ಹೊಂದಿಕೆ_cnt++];
				t->name = n;
				t->ಆಳ = ನೋಡಿದ_commits - 1;
				t->flag_within = 1u << ಹೊಂದಿಕೆ_cnt;
				t->ಸಿಕ್ಕಿತು_ಕ್ರಮ = ಹೊಂದಿಕೆ_cnt;
				c->object.flags |= t->flag_within;
				ಆದರೆ (n->prio == 2)
					annotated_cnt++;
			}
		}
		ಗಾಗಿ (cur_ಹೊಂದಿಕೆ = 0; cur_ಹೊಂದಿಕೆ < ಹೊಂದಿಕೆ_cnt; cur_ಹೊಂದಿಕೆ++) {
			ರಚನೆ possible_ಟ್ಯಾಗ್ *t = &all_matches[cur_ಹೊಂದಿಕೆ];
			ಆದರೆ (!(c->object.flags & t->flag_within))
				t->ಆಳ++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		ಆದರೆ (annotated_cnt && !prio_ಸರತಿ_ಗಾತ್ರ(&ಸರತಿ)) {
			ಪೂರ್ಣಾಂಕ ಉತ್ತಮ_ಆಳ = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
			ಚಿಹ್ನೆರಹಿತ ಉತ್ತಮ_within = 0;
			ಗಾಗಿ (cur_ಹೊಂದಿಕೆ = 0; cur_ಹೊಂದಿಕೆ < ಹೊಂದಿಕೆ_cnt; cur_ಹೊಂದಿಕೆ++) {
				ರಚನೆ possible_ಟ್ಯಾಗ್ *t = &all_matches[cur_ಹೊಂದಿಕೆ];
				ಆದರೆ (t->ಆಳ < ಉತ್ತಮ_ಆಳ) {
					ಉತ್ತಮ_ಆಳ = t->ಆಳ;
					ಉತ್ತಮ_within = t->flag_within;
				} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (t->ಆಳ == ಉತ್ತಮ_ಆಳ) {
					ಉತ್ತಮ_within |= t->flag_within;
				}
			}
			ಆದರೆ ((c->object.flags & ಉತ್ತಮ_within) == ಉತ್ತಮ_within) {
				ಆದರೆ (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_ಗಮ್ಯ_hex(&c->object.oid));
				ನಿಲ್ಲಿಸು;
			}
		}
		ಆಗ (parents) {
			ರಚನೆ commit *p = parents->ಅಂಶ;
			repo_parse_commit(the_repository, p);
			ಆದರೆ (!(p->object.flags & ನೋಡಿದ_2))
				prio_ಸರತಿ_put(&ಸರತಿ, p);
			p->object.flags |= c->object.flags;
			parents = parents->ಮುಂದಿನ;

			ಆದರೆ (ಮೊದಲ_ಪೋಷಕ)
				ನಿಲ್ಲಿಸು;
		}
	}

	ಆದರೆ (!ಹೊಂದಿಕೆ_cnt) {
		ರಚನೆ object_id *cmit_oid = &cmit->object.oid;
		ಆದರೆ (always) {
			strbuf_ಕೂಡಿಸು_unique_abbrev(dst, cmit_oid, abbrev);
			ಆದರೆ (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_ಸರತಿ(&ಸರತಿ);
			ಹಿಂತಿರುಗಿ;
		}
		ಆದರೆ (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_ಗಮ್ಯ_hex(cmit_oid));
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_ಗಮ್ಯ_hex(cmit_oid));
	}

	QSORT(all_matches, ಹೊಂದಿಕೆ_cnt, compare_pt);

	ಆದರೆ (gave_ಮೇಲೆ_on) {
		prio_ಸರತಿ_put(&ಸರತಿ, gave_ಮೇಲೆ_on);
		ನೋಡಿದ_commits--;
	}
	ನೋಡಿದ_commits += ಮುಕ್ತಾಯ_ಆಳ_computation(&ಸರತಿ, &all_matches[0]);
	clear_prio_ಸರತಿ(&ಸರತಿ);

	ಆದರೆ (debug) {
		ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ label_ಅಗಲ = -1;
		ಆದರೆ (label_ಅಗಲ < 0) {
			ಪೂರ್ಣಾಂಕ i, w;
			ಗಾಗಿ (i = 0; i < ARRAY_ಗಾತ್ರ(prio_ಹೆಸರುಗಳು); i++) {
				w = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(_(prio_ಹೆಸರುಗಳು[i]));
				ಆದರೆ (label_ಅಗಲ < w)
					label_ಅಗಲ = w;
			}
		}
		ಗಾಗಿ (cur_ಹೊಂದಿಕೆ = 0; cur_ಹೊಂದಿಕೆ < ಹೊಂದಿಕೆ_cnt; cur_ಹೊಂದಿಕೆ++) {
			ರಚನೆ possible_ಟ್ಯಾಗ್ *t = &all_matches[cur_ಹೊಂದಿಕೆ];
			fprintf(stderr, " %-*s %8d %s\n",
				label_ಅಗಲ, _(prio_ಹೆಸರುಗಳು[t->name->prio]),
				t->ಆಳ, t->name->ಮಾರ್ಗ);
		}
		fprintf(stderr, _("traversed %lu commits\n"), ನೋಡಿದ_commits);
		ಆದರೆ (gave_ಮೇಲೆ_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_ಗಮ್ಯ_hex(&gave_ಮೇಲೆ_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	ಆದರೆ (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].ಆಳ, &cmit->object.oid, dst);
	ಆದರೆ (suffix)
		strbuf_addstr(dst, suffix);
}

ರಚನೆ process_commit_ದತ್ತಾಂಶ {
	ರಚನೆ commit *current_commit;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *looking_for;
	ರಚನೆ strbuf *dst;
	ರಚನೆ rev_info *revs;
};

ಸ್ಥಿರ ಶೂನ್ಯ process_commit(ರಚನೆ commit *commit, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ process_commit_ದತ್ತಾಂಶ *pcd = ದತ್ತಾಂಶ;
	pcd->current_commit = commit;
}

ಸ್ಥಿರ ಶೂನ್ಯ process_object(ರಚನೆ object *obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ process_commit_ದತ್ತಾಂಶ *pcd = ದತ್ತಾಂಶ;

	ಆದರೆ (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		ಆದರೆ (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", ಮಾರ್ಗ);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = ಖಾಲಿ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ describe_blob(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ strbuf *dst)
{
	ರಚನೆ rev_info revs;
	ರಚನೆ strvec ವಾದಗಳು = STRVEC_INIT;
	ರಚನೆ object_id ತಲೆ_oid;
	ರಚನೆ process_commit_ದತ್ತಾಂಶ pcd = { ಖಾಲಿ, oid, dst, &revs};

	ಆದರೆ (repo_get_oid(the_repository, "HEAD", &ತಲೆ_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_ಗಮ್ಯ_hex(oid));

	strvec_pushl(&ವಾದಗಳು, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_ಗಮ್ಯ_hex(&ತಲೆ_oid),
		     ಖಾಲಿ);

	repo_init_revisions(the_repository, &revs, ಖಾಲಿ);
	ಸಿದ್ಧತೆ_revisions_from_strvec(&ವಾದಗಳು, &revs, ಖಾಲಿ);
	ಆದರೆ (ವಾದಗಳು.nr > 1)
		BUG("setup_revisions could not handle all args?");

	ಆದರೆ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&ವಾದಗಳು);

	ಆದರೆ (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_ಗಮ್ಯ_hex(oid));
}

ಸ್ಥಿರ ಶೂನ್ಯ describe(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_one)
{
	ರಚನೆ object_id oid;
	ರಚನೆ commit *cmit;
	ರಚನೆ strbuf sb = STRBUF_INIT;

	ಆದರೆ (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	ಆದರೆ (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	ಆದರೆ (cmit)
		describe_commit(cmit, &sb);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (odb_read_object_info(the_repository->objects,
				      &oid, ಖಾಲಿ) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	ಆದರೆ (!ಕೊನೆಯ_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_exact_ಹೊಂದಿಕೆ(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
				    ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *val = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_describe(ಪೂರ್ಣಾಂಕ argc,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo ಬಳಸದ )
{
	ರಚನೆ refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	ಪೂರ್ಣಾಂಕ ಒಳಗೊಂಡಿದೆ = 0;
	ರಚನೆ option options[] = {
		OPT_BOOL(0, "contains",   &ಒಳಗೊಂಡಿದೆ, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &ಟ್ಯಾಗುಗಳು, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &ಮೊದಲ_ಪೋಷಕ, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, ಖಾಲಿ,
			       N_("only output exact matches"),
			       PARSE_OPT_NOARG, option_parse_exact_ಹೊಂದಿಕೆ),
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
			.ಮೌಲ್ಯ = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.ಮೌಲ್ಯ = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_ಕೊನೆ(),
	};

	repo_config(the_repository, git_default_config, ಖಾಲಿ);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, describe_usage, 0);
	ಆದರೆ (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	ಆದರೆ (max_candidates < 0)
		max_candidates = 0;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (max_candidates > MAX_ಟ್ಯಾಗುಗಳು)
		max_candidates = MAX_ಟ್ಯಾಗುಗಳು;

	save_commit_ಬಫರ್ = 0;

	ಆದರೆ (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	ಆದರೆ (ಒಳಗೊಂಡಿದೆ) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
		ರಚನೆ strvec ವಾದಗಳು;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv_copy;
		ಪೂರ್ಣಾಂಕ ret;

		strvec_init(&ವಾದಗಳು);
		strvec_pushl(&ವಾದಗಳು, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     ಖಾಲಿ);
		ಆದರೆ (always)
			strvec_ತಳ್ಳು(&ವಾದಗಳು, "--always");
		ಆದರೆ (!all)
			strvec_ತಳ್ಳು(&ವಾದಗಳು, "--tags");

		for_each_string_list_ಅಂಶ(ಅಂಶ, &patterns)
			strvec_pushf(&ವಾದಗಳು, "--refs=refs/tags/%s", ಅಂಶ->string);
		for_each_string_list_ಅಂಶ(ಅಂಶ, &exclude_patterns)
			strvec_pushf(&ವಾದಗಳು, "--exclude=refs/tags/%s", ಅಂಶ->string);

		ಆದರೆ (all) {
			for_each_string_list_ಅಂಶ(ಅಂಶ, &patterns)
				strvec_pushf(&ವಾದಗಳು, "--refs=refs/heads/%s", ಅಂಶ->string);
			for_each_string_list_ಅಂಶ(ಅಂಶ, &exclude_patterns)
				strvec_pushf(&ವಾದಗಳು, "--exclude=refs/heads/%s", ಅಂಶ->string);
			for_each_string_list_ಅಂಶ(ಅಂಶ, &patterns)
				strvec_pushf(&ವಾದಗಳು, "--refs=refs/remotes/%s", ಅಂಶ->string);
			for_each_string_list_ಅಂಶ(ಅಂಶ, &exclude_patterns)
				strvec_pushf(&ವಾದಗಳು, "--exclude=refs/remotes/%s", ಅಂಶ->string);
		}

		ಆದರೆ (argc)
			strvec_pushv(&ವಾದಗಳು, argv);
		ಇಲ್ಲದಿದ್ದರೆ
			strvec_ತಳ್ಳು(&ವಾದಗಳು, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, ವಾದಗಳು.nr + 1);
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ವಾದಗಳು.nr; i++)
			argv_copy[i] = ವಾದಗಳು.v[i];
		argv_copy[ವಾದಗಳು.nr] = ಖಾಲಿ;

		ret = cmd_name_rev(ವಾದಗಳು.nr, argv_copy, ಪೂರ್ವಪ್ರತ್ಯಯ, the_repository);

		strvec_clear(&ವಾದಗಳು);
		ಬಿಡುಗಡೆ(argv_copy);
		ಹಿಂತಿರುಗಿ ret;
	}

	ಆದರೆ (!all)
		for_each_ref_opts.ಪೂರ್ವಪ್ರತ್ಯಯ = "refs/tags/";

	hashmap_init(&ಹೆಸರುಗಳು, commit_name_neq, ಖಾಲಿ, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, ಖಾಲಿ, &for_each_ref_opts);
	ಆದರೆ (!hashmap_get_ಗಾತ್ರ(&ಹೆಸರುಗಳು) && !always)
		die(_("No names found, cannot describe anything."));

	ಆದರೆ (argc == 0) {
		ಆದರೆ (broken) {
			ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

			strvec_pushv(&cp.ವಾದಗಳು, update_ಸೂಚ್ಯಂಕ_ವಾದಗಳು);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			ಸತತ_command(&cp);

			ಮಗು_process_init(&cp);
			strvec_pushv(&cp.ವಾದಗಳು, diff_ಸೂಚ್ಯಂಕ_ವಾದಗಳು);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			ಆದರೆ (!dirty)
				dirty = "-dirty";

			ಆಯ್ಕೆ (ಸತತ_command(&cp)) {
			ಸ್ಥಿತಿ 0:
				suffix = ಖಾಲಿ;
				ನಿಲ್ಲಿಸು;
			ಸ್ಥಿತಿ 1:
				suffix = dirty;
				ನಿಲ್ಲಿಸು;
			ಪೂರ್ವನಿಯೋಜಿತ:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dirty) {
			ರಚನೆ ಬೀಗ_file ಸೂಚ್ಯಂಕ_ಬೀಗ = ಬೀಗ_INIT;
			ರಚನೆ rev_info revs;
			ಪೂರ್ಣಾಂಕ fd;

			ಸಿದ್ಧತೆ_work_ವೃಕ್ಷ(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;
			repo_read_ಸೂಚ್ಯಂಕ(the_repository);
			refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, REFRESH_QUIET|REFRESH_UNMERGED,
				      ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);
			fd = repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository,
						    &ಸೂಚ್ಯಂಕ_ಬೀಗ, 0);
			ಆದರೆ (0 <= fd)
				repo_update_ಸೂಚ್ಯಂಕ_if_able(the_repository, &ಸೂಚ್ಯಂಕ_ಬೀಗ);

			repo_init_revisions(the_repository, &revs, ಪೂರ್ವಪ್ರತ್ಯಯ);

			ಆದರೆ (ಸಿದ್ಧತೆ_revisions(ARRAY_ಗಾತ್ರ(diff_ಸೂಚ್ಯಂಕ_ವಾದಗಳು) - 1,
					    diff_ಸೂಚ್ಯಂಕ_ವಾದಗಳು, &revs, ಖಾಲಿ) != 1)
				BUG("malformed internal diff-index command line");
			ಸತತ_diff_ಸೂಚ್ಯಂಕ(&revs, 0);

			ಆದರೆ (!diff_ಫಲಿತಾಂಶ_ಸಂಕೇತ(&revs))
				suffix = ಖಾಲಿ;
			ಇಲ್ಲದಿದ್ದರೆ
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆಗ (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	ಹಿಂತಿರುಗಿ 0;
}
