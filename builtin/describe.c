#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hashmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list-objects.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-slab.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wildmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "prio-queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oidset.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_ਟੈਗ_ਸੂਚੀ	(FLAG_ਬਿੱਟ - 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_CANDIDATES 10

define_commit_slab(commit_ਨਾਂ, ਬਣਤਰ commit_name *);

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ describe_usage[] = {
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] [<commit-ish>...]"),
	N_("git describe [--all] [--tags] [--contains] [--abbrev=<n>] --dirty[=<mark>]"),
	N_("git describe <blob>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ debug;	/* Display lots of verbose info */
ਸਥਿਰ ਪੂਰਨ all;	/* Any valid ref can be used */
ਸਥਿਰ ਪੂਰਨ ਟੈਗ_ਸੂਚੀ;	/* Allow lightweight tags */
ਸਥਿਰ ਪੂਰਨ longformat;
ਸਥਿਰ ਪੂਰਨ ਪਹਿਲਾ_ਮਾਪਾ;
ਸਥਿਰ ਪੂਰਨ abbrev = -1; /* unspecified */
ਸਥਿਰ ਪੂਰਨ max_candidates = DEFAULT_CANDIDATES;
ਸਥਿਰ ਬਣਤਰ hashmap ਨਾਂ;
ਸਥਿਰ ਪੂਰਨ have_util;
ਸਥਿਰ ਬਣਤਰ string_list patterns = STRING_LIST_INIT_NODUP;
ਸਥਿਰ ਬਣਤਰ string_list exclude_patterns = STRING_LIST_INIT_NODUP;
ਸਥਿਰ ਪੂਰਨ always;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *suffix, *dirty, *broken;
ਸਥਿਰ ਬਣਤਰ commit_ਨਾਂ commit_ਨਾਂ;

/* diff-index command arguments to check if working tree is dirty. */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *diff_ਸੂਚਕਾਂਕ_ਦਲੀਲਾਂ[] = {
	"diff-index", "--quiet", "HEAD", "--", ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *update_ਸੂਚਕਾਂਕ_ਦਲੀਲਾਂ[] = {
	"update-index", "--unmerged", "-q", "--refresh", ਨਿਰਾ
};

ਬਣਤਰ commit_name {
	ਬਣਤਰ hashmap_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
	ਬਣਤਰ object_id peeled;
	ਬਣਤਰ ਟੈਗ *ਟੈਗ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ prio:2; /* annotated tag = 2, tag = 1, head = 0 */
	ਬਿਨਾਨਿਸ਼ਾਨੀ name_checked:1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ misnamed:1;
	ਬਣਤਰ object_id oid;
	ਅੱਖਰ *ਰਸਤਾ;
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *prio_ਨਾਂ[] = {
	N_("head"), N_("lightweight"), N_("annotated"),
};

ਸਥਿਰ ਪੂਰਨ commit_name_neq(ਅਚਲ ਖਾਲੀ *cmp_ਅੰਕੜੇ ਅਣਵਰਤਿਆ,
			   ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *eptr,
			   ਅਚਲ ਬਣਤਰ hashmap_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ_or_ਕੁੰਜੀ,
			   ਅਚਲ ਖਾਲੀ *peeled)
{
	ਅਚਲ ਬਣਤਰ commit_name *cn1, *cn2;

	cn1 = container_ਦਾ(eptr, ਅਚਲ ਬਣਤਰ commit_name, ਇੰਦਰਾਜ);
	cn2 = container_ਦਾ(ਇੰਦਰਾਜ_or_ਕੁੰਜੀ, ਅਚਲ ਬਣਤਰ commit_name, ਇੰਦਰਾਜ);

	ਵਾਪਸ !oideq(&cn1->peeled, peeled ? peeled : &cn2->peeled);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ commit_name *ਲੱਭੋ_commit_name(ਅਚਲ ਬਣਤਰ object_id *peeled)
{
	ਵਾਪਸ hashmap_get_ਇੰਦਰਾਜ_from_hash(&ਨਾਂ, oidhash(peeled), peeled,
						ਬਣਤਰ commit_name, ਇੰਦਰਾਜ);
}

ਸਥਿਰ ਪੂਰਨ replace_name(ਬਣਤਰ commit_name *e,
			       ਪੂਰਨ prio,
			       ਅਚਲ ਬਣਤਰ object_id *oid,
			       ਬਣਤਰ ਟੈਗ **ਟੈਗ)
{
	ਜੇ (!e || e->prio < prio)
		ਵਾਪਸ 1;

	ਜੇ (e->prio == 2 && prio == 2) {
		/* Multiple annotated tags point to the same commit.
		 * Select one to keep based upon their tagger date.
		 */
		ਬਣਤਰ ਟੈਗ *t;

		ਜੇ (!e->ਟੈਗ) {
			t = lookup_ਟੈਗ(the_repository, &e->oid);
			ਜੇ (!t || parse_ਟੈਗ(the_repository, t))
				ਵਾਪਸ 1;
			e->ਟੈਗ = t;
		}

		t = lookup_ਟੈਗ(the_repository, oid);
		ਜੇ (!t || parse_ਟੈਗ(the_repository, t))
			ਵਾਪਸ 0;
		*ਟੈਗ = t;

		ਜੇ (e->ਟੈਗ->date < t->date)
			ਵਾਪਸ 1;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਗਿਆਤ_ਨਾਂ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
			       ਅਚਲ ਬਣਤਰ object_id *peeled,
			       ਪੂਰਨ prio,
			       ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਬਣਤਰ commit_name *e = ਲੱਭੋ_commit_name(peeled);
	ਬਣਤਰ ਟੈਗ *ਟੈਗ = ਨਿਰਾ;
	ਜੇ (replace_name(e, prio, oid, &ਟੈਗ)) {
		ਜੇ (!e) {
			e = xmalloc(ਆਕਾਰ(ਬਣਤਰ commit_name));
			oidcpy(&e->peeled, peeled);
			hashmap_ਇੰਦਰਾਜ_init(&e->ਇੰਦਰਾਜ, oidhash(peeled));
			hashmap_ਜੋੜੋ(&ਨਾਂ, &e->ਇੰਦਰਾਜ);
			e->ਰਸਤਾ = ਨਿਰਾ;
		}
		e->ਟੈਗ = ਟੈਗ;
		e->prio = prio;
		e->name_checked = 0;
		e->misnamed = 0;
		oidcpy(&e->oid, oid);
		ਮੁਕਤ(e->ਰਸਤਾ);
		e->ਰਸਤਾ = xstrdup(ਰਸਤਾ);
	}
}

ਸਥਿਰ ਪੂਰਨ get_name(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ is_ਟੈਗ = 0;
	ਬਣਤਰ object_id peeled;
	ਪੂਰਨ is_annotated, prio;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ_ਮੰਜ਼ਿਲ_ਮੇਲ = ਨਿਰਾ;

	ਜੇ (skip_ਅਗੇਤਰ(ref->name, "refs/tags/", &ਰਸਤਾ_ਮੰਜ਼ਿਲ_ਮੇਲ)) {
		is_ਟੈਗ = 1;
	} ਨਹੀਂਤਾਂ ਜੇ (all) {
		ਜੇ ((exclude_patterns.nr || patterns.nr) &&
		    !skip_ਅਗੇਤਰ(ref->name, "refs/heads/", &ਰਸਤਾ_ਮੰਜ਼ਿਲ_ਮੇਲ) &&
		    !skip_ਅਗੇਤਰ(ref->name, "refs/remotes/", &ਰਸਤਾ_ਮੰਜ਼ਿਲ_ਮੇਲ)) {
			/* Only accept reference of known type if there are match/exclude patterns */
			ਵਾਪਸ 0;
		}
	} ਨਹੀਂਤਾਂ {
		/* Reject anything outside refs/tags/ unless --all */
		ਵਾਪਸ 0;
	}

	/*
	 * If we're given exclude patterns, first exclude any tag which match
	 * any of the exclude pattern.
	 */
	ਜੇ (exclude_patterns.nr) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &exclude_patterns) {
			ਜੇ (!wildmatch(ਵਸਤੂ->string, ਰਸਤਾ_ਮੰਜ਼ਿਲ_ਮੇਲ, 0))
				ਵਾਪਸ 0;
		}
	}

	/*
	 * If we're given patterns, accept only tags which match at least one
	 * pattern.
	 */
	ਜੇ (patterns.nr) {
		ਪੂਰਨ ਮਿਲਿਆ_2 = 0;
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &patterns) {
			ਜੇ (!wildmatch(ਵਸਤੂ->string, ਰਸਤਾ_ਮੰਜ਼ਿਲ_ਮੇਲ, 0)) {
				ਮਿਲਿਆ_2 = 1;
				ਰੋਕੋ;
			}
		}

		ਜੇ (!ਮਿਲਿਆ_2)
			ਵਾਪਸ 0;
	}

	/* Is it annotated? */
	ਜੇ (!reference_get_peeled_oid(the_repository, ref, &peeled)) {
		is_annotated = !oideq(ref->oid, &peeled);
	} ਨਹੀਂਤਾਂ {
		oidcpy(&peeled, ref->oid);
		is_annotated = 0;
	}

	/*
	 * By default, we only use annotated tags, but with --tags
	 * we fall back to lightweight ones (even without --tags,
	 * we still remember lightweight ones, only to give hints
	 * in an error message).  --all allows any refs to be used.
	 */
	ਜੇ (is_annotated)
		prio = 2;
	ਨਹੀਂਤਾਂ ਜੇ (is_ਟੈਗ)
		prio = 1;
	ਨਹੀਂਤਾਂ
		prio = 0;

	ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਗਿਆਤ_ਨਾਂ(all ? ref->name + 5 : ref->name + 10,
			   &peeled, prio, ref->oid);
	ਵਾਪਸ 0;
}

ਬਣਤਰ possible_ਟੈਗ {
	ਬਣਤਰ commit_name *name;
	ਪੂਰਨ ਡੂੰਘਾਈ;
	ਪੂਰਨ ਮਿਲਿਆ_ਕ੍ਰਮ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ flag_within;
};

ਸਥਿਰ ਪੂਰਨ compare_pt(ਅਚਲ ਖਾਲੀ *a_, ਅਚਲ ਖਾਲੀ *b_)
{
	ਬਣਤਰ possible_ਟੈਗ *a = (ਬਣਤਰ possible_ਟੈਗ *)a_;
	ਬਣਤਰ possible_ਟੈਗ *b = (ਬਣਤਰ possible_ਟੈਗ *)b_;
	ਜੇ (a->ਡੂੰਘਾਈ != b->ਡੂੰਘਾਈ)
		ਵਾਪਸ a->ਡੂੰਘਾਈ - b->ਡੂੰਘਾਈ;
	ਜੇ (a->ਮਿਲਿਆ_ਕ੍ਰਮ != b->ਮਿਲਿਆ_ਕ੍ਰਮ)
		ਵਾਪਸ a->ਮਿਲਿਆ_ਕ੍ਰਮ - b->ਮਿਲਿਆ_ਕ੍ਰਮ;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅੰਤ_ਡੂੰਘਾਈ_computation(ਬਣਤਰ prio_ਕਤਾਰ *ਕਤਾਰ,
					      ਬਣਤਰ possible_ਟੈਗ *ਉੱਤਮ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵੇਖਿਆ_commits = 0;
	ਬਣਤਰ oidset unflagged = OIDSET_INIT;
	ਬਣਤਰ commit *c;

	prio_ਕਤਾਰ_for_each(ਕਤਾਰ, c) {
		ਜੇ (!(c->object.flags & ਉੱਤਮ->flag_within))
			oidset_ਪਾਓ(&unflagged, &c->object.oid);
	}

	ਜਦੋਂ ((c = prio_ਕਤਾਰ_get(ਕਤਾਰ))) {
		ਬਣਤਰ commit_list *parents = c->parents;
		ਵੇਖਿਆ_commits++;
		ਜੇ (c->object.flags & ਉੱਤਮ->flag_within) {
			ਜੇ (!oidset_ਮਾਪ(&unflagged))
				ਰੋਕੋ;
		} ਨਹੀਂਤਾਂ {
			oidset_remove(&unflagged, &c->object.oid);
			ਉੱਤਮ->ਡੂੰਘਾਈ++;
		}
		ਜਦੋਂ (parents) {
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਵੇਖਿਆ, flag_before, flag_after;
			ਬਣਤਰ commit *p = parents->ਵਸਤੂ;
			repo_parse_commit(the_repository, p);
			ਵੇਖਿਆ = p->object.flags & ਵੇਖਿਆ_2;
			ਜੇ (!ਵੇਖਿਆ)
				prio_ਕਤਾਰ_put(ਕਤਾਰ, p);
			flag_before = p->object.flags & ਉੱਤਮ->flag_within;
			p->object.flags |= c->object.flags;
			flag_after = p->object.flags & ਉੱਤਮ->flag_within;
			ਜੇ (!ਵੇਖਿਆ && !flag_after)
				oidset_ਪਾਓ(&unflagged, &p->object.oid);
			ਜੇ (ਵੇਖਿਆ && !flag_before && flag_after)
				oidset_remove(&unflagged, &p->object.oid);
			parents = parents->ਅਗਲਾ;
		}
	}
	oidset_clear(&unflagged);
	ਵਾਪਸ ਵੇਖਿਆ_commits;
}

ਸਥਿਰ ਖਾਲੀ append_name(ਬਣਤਰ commit_name *n, ਬਣਤਰ strbuf *dst)
{
	ਜੇ (n->prio == 2 && !n->ਟੈਗ) {
		n->ਟੈਗ = lookup_ਟੈਗ(the_repository, &n->oid);
		ਜੇ (!n->ਟੈਗ || parse_ਟੈਗ(the_repository, n->ਟੈਗ))
			die(_("annotated tag %s not available"), n->ਰਸਤਾ);
	}
	ਜੇ (n->ਟੈਗ && !n->name_checked) {
		ਜੇ (strcmp(n->ਟੈਗ->ਟੈਗ, all ? n->ਰਸਤਾ + 5 : n->ਰਸਤਾ)) {
			warning(_("tag '%s' is externally known as '%s'"),
				n->ਰਸਤਾ, n->ਟੈਗ->ਟੈਗ);
			n->misnamed = 1;
		}
		n->name_checked = 1;
	}

	ਜੇ (n->ਟੈਗ) {
		ਜੇ (all)
			strbuf_addstr(dst, "tags/");
		strbuf_addstr(dst, n->ਟੈਗ->ਟੈਗ);
	} ਨਹੀਂਤਾਂ {
		strbuf_addstr(dst, n->ਰਸਤਾ);
	}
}

ਸਥਿਰ ਖਾਲੀ append_suffix(ਪੂਰਨ ਡੂੰਘਾਈ, ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ strbuf *dst)
{
	strbuf_addf(dst, "-%d-g%s", ਡੂੰਘਾਈ,
		    repo_ਲੱਭੋ_unique_abbrev(the_repository, oid, abbrev));
}

ਸਥਿਰ ਖਾਲੀ describe_commit(ਬਣਤਰ commit *cmit, ਬਣਤਰ strbuf *dst)
{
	ਬਣਤਰ commit *c, *gave_ਉੱਪਰ_on = ਨਿਰਾ;
	ਬਣਤਰ prio_ਕਤਾਰ ਕਤਾਰ = { compare_commits_by_commit_date };
	ਬਣਤਰ commit_name *n;
	ਬਣਤਰ possible_ਟੈਗ all_matches[MAX_ਟੈਗ_ਸੂਚੀ];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੇਲ_cnt = 0, annotated_cnt = 0, cur_ਮੇਲ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵੇਖਿਆ_commits = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ unannotated_cnt = 0;

	n = ਲੱਭੋ_commit_name(&cmit->object.oid);
	ਜੇ (n && (ਟੈਗ_ਸੂਚੀ || all || n->prio == 2)) {
		/*
		 * Exact match to an existing ref.
		 */
		append_name(n, dst);
		ਜੇ (n->misnamed || longformat)
			append_suffix(0, n->ਟੈਗ ? get_tagged_oid(n->ਟੈਗ) : &cmit->object.oid, dst);
		ਜੇ (suffix)
			strbuf_addstr(dst, suffix);
		ਵਾਪਸ;
	}

	ਜੇ (!max_candidates)
		die(_("no tag exactly matches '%s'"), oid_ਮੰਜ਼ਿਲ_hex(&cmit->object.oid));
	ਜੇ (debug)
		fprintf(stderr, _("No exact match on refs or tags, searching to describe\n"));

	ਜੇ (!have_util) {
		ਬਣਤਰ hashmap_iter iter;
		ਬਣਤਰ commit *c;
		ਬਣਤਰ commit_name *n;

		init_commit_ਨਾਂ(&commit_ਨਾਂ);
		hashmap_for_each_ਇੰਦਰਾਜ(&ਨਾਂ, &iter, n,
					ਇੰਦਰਾਜ /* member name */) {
			c = lookup_commit_reference_gently(the_repository,
							   &n->peeled, 1);
			ਜੇ (c)
				*commit_ਨਾਂ_at(&commit_ਨਾਂ, c) = n;
		}
		have_util = 1;
	}

	cmit->object.flags = ਵੇਖਿਆ_2;
	prio_ਕਤਾਰ_put(&ਕਤਾਰ, cmit);
	ਜਦੋਂ ((c = prio_ਕਤਾਰ_get(&ਕਤਾਰ))) {
		ਬਣਤਰ commit_list *parents = c->parents;
		ਬਣਤਰ commit_name **ਥਾਂ;

		ਵੇਖਿਆ_commits++;

		ਜੇ (ਮੇਲ_cnt == max_candidates ||
		    ਮੇਲ_cnt == hashmap_get_ਮਾਪ(&ਨਾਂ)) {
			gave_ਉੱਪਰ_on = c;
			ਰੋਕੋ;
		}

		ਥਾਂ = commit_ਨਾਂ_peek(&commit_ਨਾਂ, c);
		n = ਥਾਂ ? *ਥਾਂ : ਨਿਰਾ;
		ਜੇ (n) {
			ਜੇ (!ਟੈਗ_ਸੂਚੀ && !all && n->prio < 2) {
				unannotated_cnt++;
			} ਨਹੀਂਤਾਂ ਜੇ (ਮੇਲ_cnt < max_candidates) {
				ਬਣਤਰ possible_ਟੈਗ *t = &all_matches[ਮੇਲ_cnt++];
				t->name = n;
				t->ਡੂੰਘਾਈ = ਵੇਖਿਆ_commits - 1;
				t->flag_within = 1u << ਮੇਲ_cnt;
				t->ਮਿਲਿਆ_ਕ੍ਰਮ = ਮੇਲ_cnt;
				c->object.flags |= t->flag_within;
				ਜੇ (n->prio == 2)
					annotated_cnt++;
			}
		}
		ਲਈ (cur_ਮੇਲ = 0; cur_ਮੇਲ < ਮੇਲ_cnt; cur_ਮੇਲ++) {
			ਬਣਤਰ possible_ਟੈਗ *t = &all_matches[cur_ਮੇਲ];
			ਜੇ (!(c->object.flags & t->flag_within))
				t->ਡੂੰਘਾਈ++;
		}
		/* Stop if last remaining path already covered by best candidate(s) */
		ਜੇ (annotated_cnt && !prio_ਕਤਾਰ_ਮਾਪ(&ਕਤਾਰ)) {
			ਪੂਰਨ ਉੱਤਮ_ਡੂੰਘਾਈ = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਉੱਤਮ_within = 0;
			ਲਈ (cur_ਮੇਲ = 0; cur_ਮੇਲ < ਮੇਲ_cnt; cur_ਮੇਲ++) {
				ਬਣਤਰ possible_ਟੈਗ *t = &all_matches[cur_ਮੇਲ];
				ਜੇ (t->ਡੂੰਘਾਈ < ਉੱਤਮ_ਡੂੰਘਾਈ) {
					ਉੱਤਮ_ਡੂੰਘਾਈ = t->ਡੂੰਘਾਈ;
					ਉੱਤਮ_within = t->flag_within;
				} ਨਹੀਂਤਾਂ ਜੇ (t->ਡੂੰਘਾਈ == ਉੱਤਮ_ਡੂੰਘਾਈ) {
					ਉੱਤਮ_within |= t->flag_within;
				}
			}
			ਜੇ ((c->object.flags & ਉੱਤਮ_within) == ਉੱਤਮ_within) {
				ਜੇ (debug)
					fprintf(stderr, _("finished search at %s\n"),
						oid_ਮੰਜ਼ਿਲ_hex(&c->object.oid));
				ਰੋਕੋ;
			}
		}
		ਜਦੋਂ (parents) {
			ਬਣਤਰ commit *p = parents->ਵਸਤੂ;
			repo_parse_commit(the_repository, p);
			ਜੇ (!(p->object.flags & ਵੇਖਿਆ_2))
				prio_ਕਤਾਰ_put(&ਕਤਾਰ, p);
			p->object.flags |= c->object.flags;
			parents = parents->ਅਗਲਾ;

			ਜੇ (ਪਹਿਲਾ_ਮਾਪਾ)
				ਰੋਕੋ;
		}
	}

	ਜੇ (!ਮੇਲ_cnt) {
		ਬਣਤਰ object_id *cmit_oid = &cmit->object.oid;
		ਜੇ (always) {
			strbuf_ਜੋੜੋ_unique_abbrev(dst, cmit_oid, abbrev);
			ਜੇ (suffix)
				strbuf_addstr(dst, suffix);
			clear_prio_ਕਤਾਰ(&ਕਤਾਰ);
			ਵਾਪਸ;
		}
		ਜੇ (unannotated_cnt)
			die(_("No annotated tags can describe '%s'.\n"
			    "However, there were unannotated tags: try --tags."),
			    oid_ਮੰਜ਼ਿਲ_hex(cmit_oid));
		ਨਹੀਂਤਾਂ
			die(_("No tags can describe '%s'.\n"
			    "Try --always, or create some tags."),
			    oid_ਮੰਜ਼ਿਲ_hex(cmit_oid));
	}

	QSORT(all_matches, ਮੇਲ_cnt, compare_pt);

	ਜੇ (gave_ਉੱਪਰ_on) {
		prio_ਕਤਾਰ_put(&ਕਤਾਰ, gave_ਉੱਪਰ_on);
		ਵੇਖਿਆ_commits--;
	}
	ਵੇਖਿਆ_commits += ਅੰਤ_ਡੂੰਘਾਈ_computation(&ਕਤਾਰ, &all_matches[0]);
	clear_prio_ਕਤਾਰ(&ਕਤਾਰ);

	ਜੇ (debug) {
		ਸਥਿਰ ਪੂਰਨ label_ਚੌੜਾਈ = -1;
		ਜੇ (label_ਚੌੜਾਈ < 0) {
			ਪੂਰਨ i, w;
			ਲਈ (i = 0; i < ARRAY_ਮਾਪ(prio_ਨਾਂ); i++) {
				w = ਲੜੀ_ਲੰਬਾਈ(_(prio_ਨਾਂ[i]));
				ਜੇ (label_ਚੌੜਾਈ < w)
					label_ਚੌੜਾਈ = w;
			}
		}
		ਲਈ (cur_ਮੇਲ = 0; cur_ਮੇਲ < ਮੇਲ_cnt; cur_ਮੇਲ++) {
			ਬਣਤਰ possible_ਟੈਗ *t = &all_matches[cur_ਮੇਲ];
			fprintf(stderr, " %-*s %8d %s\n",
				label_ਚੌੜਾਈ, _(prio_ਨਾਂ[t->name->prio]),
				t->ਡੂੰਘਾਈ, t->name->ਰਸਤਾ);
		}
		fprintf(stderr, _("traversed %lu commits\n"), ਵੇਖਿਆ_commits);
		ਜੇ (gave_ਉੱਪਰ_on) {
			fprintf(stderr,
				_("found %i tags; gave up search at %s\n"),
				max_candidates,
				oid_ਮੰਜ਼ਿਲ_hex(&gave_ਉੱਪਰ_on->object.oid));
		}
	}

	append_name(all_matches[0].name, dst);
	ਜੇ (all_matches[0].name->misnamed || abbrev)
		append_suffix(all_matches[0].ਡੂੰਘਾਈ, &cmit->object.oid, dst);
	ਜੇ (suffix)
		strbuf_addstr(dst, suffix);
}

ਬਣਤਰ process_commit_ਅੰਕੜੇ {
	ਬਣਤਰ commit *current_commit;
	ਅਚਲ ਬਣਤਰ object_id *looking_for;
	ਬਣਤਰ strbuf *dst;
	ਬਣਤਰ rev_info *revs;
};

ਸਥਿਰ ਖਾਲੀ process_commit(ਬਣਤਰ commit *commit, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ process_commit_ਅੰਕੜੇ *pcd = ਅੰਕੜੇ;
	pcd->current_commit = commit;
}

ਸਥਿਰ ਖਾਲੀ process_object(ਬਣਤਰ object *obj, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ process_commit_ਅੰਕੜੇ *pcd = ਅੰਕੜੇ;

	ਜੇ (oideq(pcd->looking_for, &obj->oid) && !pcd->dst->len) {
		reset_revision_walk();
		ਜੇ (pcd->current_commit) {
			describe_commit(pcd->current_commit, pcd->dst);
			strbuf_addf(pcd->dst, ":%s", ਰਸਤਾ);
		}
		commit_list_free(pcd->revs->commits);
		pcd->revs->commits = ਨਿਰਾ;
	}
}

ਸਥਿਰ ਖਾਲੀ describe_blob(ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ strbuf *dst)
{
	ਬਣਤਰ rev_info revs;
	ਬਣਤਰ strvec ਦਲੀਲਾਂ = STRVEC_INIT;
	ਬਣਤਰ object_id ਸਿਰ_oid;
	ਬਣਤਰ process_commit_ਅੰਕੜੇ pcd = { ਨਿਰਾ, oid, dst, &revs};

	ਜੇ (repo_get_oid(the_repository, "HEAD", &ਸਿਰ_oid))
		die(_("cannot search for blob '%s' on an unborn branch"),
		    oid_ਮੰਜ਼ਿਲ_hex(oid));

	strvec_pushl(&ਦਲੀਲਾਂ, "internal: The first arg is not parsed",
		     "--objects", "--in-commit-order", "--reverse",
		     oid_ਮੰਜ਼ਿਲ_hex(&ਸਿਰ_oid),
		     ਨਿਰਾ);

	repo_init_revisions(the_repository, &revs, ਨਿਰਾ);
	ਤਿਆਰੀ_revisions_from_strvec(&ਦਲੀਲਾਂ, &revs, ਨਿਰਾ);
	ਜੇ (ਦਲੀਲਾਂ.nr > 1)
		BUG("setup_revisions could not handle all args?");

	ਜੇ (prepare_revision_walk(&revs))
		die("revision walk setup failed");

	traverse_commit_list(&revs, process_commit, process_object, &pcd);
	reset_revision_walk();
	release_revisions(&revs);
	strvec_clear(&ਦਲੀਲਾਂ);

	ਜੇ (!dst->len)
		die(_("blob '%s' not reachable from HEAD"), oid_ਮੰਜ਼ਿਲ_hex(oid));
}

ਸਥਿਰ ਖਾਲੀ describe(ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ ਆਖਰੀ_one)
{
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ commit *cmit;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	ਜੇ (debug)
		fprintf(stderr, _("describe %s\n"), arg);

	ਜੇ (repo_get_oid(the_repository, arg, &oid))
		die(_("Not a valid object name %s"), arg);
	cmit = lookup_commit_reference_gently(the_repository, &oid, 1);

	ਜੇ (cmit)
		describe_commit(cmit, &sb);
	ਨਹੀਂਤਾਂ ਜੇ (odb_read_object_info(the_repository->objects,
				      &oid, ਨਿਰਾ) == OBJ_BLOB)
		describe_blob(&oid, &sb);
	ਨਹੀਂਤਾਂ
		die(_("%s is neither a commit nor blob"), arg);

	puts(sb.buf);

	ਜੇ (!ਆਖਰੀ_one)
		clear_commit_marks(cmit, -1);

	strbuf_release(&sb);
}

ਸਥਿਰ ਪੂਰਨ option_parse_exact_ਮੇਲ(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
				    ਪੂਰਨ unset)
{
	ਪੂਰਨ *val = opt->ਮੁੱਲ;

	BUG_ON_OPT_ARG(arg);

	*val = unset ? DEFAULT_CANDIDATES : 0;
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_describe(ਪੂਰਨ argc,
		 ਅਚਲ ਅੱਖਰ **argv,
		 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ )
{
	ਬਣਤਰ refs_for_each_ref_options for_each_ref_opts = {
		.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
	};
	ਪੂਰਨ ਸ਼ਾਮਲ_ਹੈ = 0;
	ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "contains",   &ਸ਼ਾਮਲ_ਹੈ, N_("find the tag that comes after the commit")),
		OPT_BOOL(0, "debug",      &debug, N_("debug search strategy on stderr")),
		OPT_BOOL(0, "all",        &all, N_("use any ref")),
		OPT_BOOL(0, "tags",       &ਟੈਗ_ਸੂਚੀ, N_("use any tag, even unannotated")),
		OPT_BOOL(0, "long",       &longformat, N_("always use long format")),
		OPT_BOOL(0, "first-parent", &ਪਹਿਲਾ_ਮਾਪਾ, N_("only follow first parent")),
		OPT__ABBREV(&abbrev),
		OPT_CALLBACK_F(0, "exact-match", &max_candidates, ਨਿਰਾ,
			       N_("only output exact matches"),
			       PARSE_OPT_NOARG, option_parse_exact_ਮੇਲ),
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
			.ਮੁੱਲ = &dirty,
			.argh = N_("mark"),
			.help = N_("append <mark> on dirty working tree (default: \"-dirty\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-dirty",
		},
		{
			.type = OPTION_STRING,
			.long_name = "broken",
			.ਮੁੱਲ = &broken,
			.argh = N_("mark"),
			.help = N_("append <mark> on broken working tree (default: \"-broken\")"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "-broken",
		},
		OPT_ਸਿਰਾ(),
	};

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, describe_usage, 0);
	ਜੇ (abbrev < 0)
		abbrev = DEFAULT_ABBREV;

	ਜੇ (max_candidates < 0)
		max_candidates = 0;
	ਨਹੀਂਤਾਂ ਜੇ (max_candidates > MAX_ਟੈਗ_ਸੂਚੀ)
		max_candidates = MAX_ਟੈਗ_ਸੂਚੀ;

	save_commit_ਬਫਰ = 0;

	ਜੇ (longformat && abbrev == 0)
		die(_("options '%s' and '%s' cannot be used together"), "--long", "--abbrev=0");

	ਜੇ (ਸ਼ਾਮਲ_ਹੈ) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
		ਬਣਤਰ strvec ਦਲੀਲਾਂ;
		ਅਚਲ ਅੱਖਰ **argv_copy;
		ਪੂਰਨ ret;

		strvec_init(&ਦਲੀਲਾਂ);
		strvec_pushl(&ਦਲੀਲਾਂ, "name-rev",
			     "--peel-tag", "--name-only", "--no-undefined",
			     ਨਿਰਾ);
		ਜੇ (always)
			strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, "--always");
		ਜੇ (!all)
			strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, "--tags");

		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &patterns)
			strvec_pushf(&ਦਲੀਲਾਂ, "--refs=refs/tags/%s", ਵਸਤੂ->string);
		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &exclude_patterns)
			strvec_pushf(&ਦਲੀਲਾਂ, "--exclude=refs/tags/%s", ਵਸਤੂ->string);

		ਜੇ (all) {
			for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &patterns)
				strvec_pushf(&ਦਲੀਲਾਂ, "--refs=refs/heads/%s", ਵਸਤੂ->string);
			for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &exclude_patterns)
				strvec_pushf(&ਦਲੀਲਾਂ, "--exclude=refs/heads/%s", ਵਸਤੂ->string);
			for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &patterns)
				strvec_pushf(&ਦਲੀਲਾਂ, "--refs=refs/remotes/%s", ਵਸਤੂ->string);
			for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &exclude_patterns)
				strvec_pushf(&ਦਲੀਲਾਂ, "--exclude=refs/remotes/%s", ਵਸਤੂ->string);
		}

		ਜੇ (argc)
			strvec_pushv(&ਦਲੀਲਾਂ, argv);
		ਨਹੀਂਤਾਂ
			strvec_ਧੱਕੋ(&ਦਲੀਲਾਂ, "HEAD");

		/*
		 * `cmd_name_rev()` modifies the array, so we'd leak its
		 * contained strings if we didn't do a copy here.
		 */
		ALLOC_ARRAY(argv_copy, ਦਲੀਲਾਂ.nr + 1);
		ਲਈ (ਮਾਪ_t i = 0; i < ਦਲੀਲਾਂ.nr; i++)
			argv_copy[i] = ਦਲੀਲਾਂ.v[i];
		argv_copy[ਦਲੀਲਾਂ.nr] = ਨਿਰਾ;

		ret = cmd_name_rev(ਦਲੀਲਾਂ.nr, argv_copy, ਅਗੇਤਰ, the_repository);

		strvec_clear(&ਦਲੀਲਾਂ);
		ਮੁਕਤ(argv_copy);
		ਵਾਪਸ ret;
	}

	ਜੇ (!all)
		for_each_ref_opts.ਅਗੇਤਰ = "refs/tags/";

	hashmap_init(&ਨਾਂ, commit_name_neq, ਨਿਰਾ, 0);
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_name, ਨਿਰਾ, &for_each_ref_opts);
	ਜੇ (!hashmap_get_ਮਾਪ(&ਨਾਂ) && !always)
		die(_("No names found, cannot describe anything."));

	ਜੇ (argc == 0) {
		ਜੇ (broken) {
			ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

			strvec_pushv(&cp.ਦਲੀਲਾਂ, update_ਸੂਚਕਾਂਕ_ਦਲੀਲਾਂ);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;
			ਲੜੀਵਾਰ_command(&cp);

			ਬੱਚਾ_process_init(&cp);
			strvec_pushv(&cp.ਦਲੀਲਾਂ, diff_ਸੂਚਕਾਂਕ_ਦਲੀਲਾਂ);
			cp.git_cmd = 1;
			cp.no_stdin = 1;
			cp.no_stdout = 1;

			ਜੇ (!dirty)
				dirty = "-dirty";

			ਚੋਣ (ਲੜੀਵਾਰ_command(&cp)) {
			ਹਾਲਤ 0:
				suffix = ਨਿਰਾ;
				ਰੋਕੋ;
			ਹਾਲਤ 1:
				suffix = dirty;
				ਰੋਕੋ;
			ਮੂਲ:
				/* diff-index aborted abnormally */
				suffix = broken;
			}
		} ਨਹੀਂਤਾਂ ਜੇ (dirty) {
			ਬਣਤਰ ਤਾਲਾ_file ਸੂਚਕਾਂਕ_ਤਾਲਾ = ਤਾਲਾ_INIT;
			ਬਣਤਰ rev_info revs;
			ਪੂਰਨ fd;

			ਤਿਆਰੀ_work_ਰੁੱਖ(the_repository);
			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;
			repo_read_ਸੂਚਕਾਂਕ(the_repository);
			refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, REFRESH_QUIET|REFRESH_UNMERGED,
				      ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);
			fd = repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository,
						    &ਸੂਚਕਾਂਕ_ਤਾਲਾ, 0);
			ਜੇ (0 <= fd)
				repo_update_ਸੂਚਕਾਂਕ_if_able(the_repository, &ਸੂਚਕਾਂਕ_ਤਾਲਾ);

			repo_init_revisions(the_repository, &revs, ਅਗੇਤਰ);

			ਜੇ (ਤਿਆਰੀ_revisions(ARRAY_ਮਾਪ(diff_ਸੂਚਕਾਂਕ_ਦਲੀਲਾਂ) - 1,
					    diff_ਸੂਚਕਾਂਕ_ਦਲੀਲਾਂ, &revs, ਨਿਰਾ) != 1)
				BUG("malformed internal diff-index command line");
			ਲੜੀਵਾਰ_diff_ਸੂਚਕਾਂਕ(&revs, 0);

			ਜੇ (!diff_ਨਤੀਜਾ_ਕੋਡ(&revs))
				suffix = ਨਿਰਾ;
			ਨਹੀਂਤਾਂ
				suffix = dirty;
			release_revisions(&revs);
		}
		describe("HEAD", 1);
	} ਨਹੀਂਤਾਂ ਜੇ (dirty) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--dirty");
	} ਨਹੀਂਤਾਂ ਜੇ (broken) {
		die(_("option '%s' and commit-ishes cannot be used together"), "--broken");
	} ਨਹੀਂਤਾਂ {
		ਜਦੋਂ (argc-- > 0)
			describe(*argv++, argc == 0);
	}
	ਵਾਪਸ 0;
}
