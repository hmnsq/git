#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "pretty.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "color.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"

#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "commit-slab.h"
#ଅନ୍ତର୍ଭାଵ "date.h"
#ଅନ୍ତର୍ଭାଵ "wildmatch.h"
#ଅନ୍ତର୍ଭାଵ "prio-queue.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର*ଅଚଳ show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    ଖାଲି
};

ସ୍ଥିର ଗଣନା git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

ସ୍ଥିର ସଂରଚନା strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#define UNINTERESTING	01

#define REV_SHIFT	 2
#define MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#define DEFAULT_REFLOG	4

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_color_code(ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx)
{
	ଯଦି (want_color(showbranch_use_color))
		ଫେରନ୍ତୁ column_colors_ansi[idx % column_colors_ansi_max];
	ଫେରନ୍ତୁ "";
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_color_reset_code(ଶୂନ୍ୟ)
{
	ଯଦି (want_color(showbranch_use_color))
		ଫେରନ୍ତୁ GIT_COLOR_RESET;
	ଫେରନ୍ତୁ "";
}

ସ୍ଥିର ସଂରଚନା commit *interesting(ସଂରଚନା prio_queue *queue)
{
	ସଂରଚନା commit *commit;
	prio_queue_for_each(queue, commit) {
		ଯଦି (!(commit->object.flags & UNINTERESTING))
			ଫେରନ୍ତୁ commit;
	}
	ଫେରନ୍ତୁ ଖାଲି;
}

ସଂରଚନା commit_name {
	ଅଚଳ ଅକ୍ଷର *head_name; /* which head's ancestor? */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, ସଂରଚନା commit_name *);
ସ୍ଥିର ସଂରଚନା commit_name_slab name_slab;

ସ୍ଥିର ସଂରଚନା commit_name *commit_to_name(ସଂରଚନା commit *commit)
{
	ଫେରନ୍ତୁ *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
ସ୍ଥିର ଶୂନ୍ୟ name_commit(ସଂରଚନା commit *commit, ଅଚଳ ଅକ୍ଷର *head_name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nth)
{
	ସଂରଚନା commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	ଯଦି (!name) {
		name = xmalloc(ଆକାର(*name));
		*commit_name_slab_at(&name_slab, commit) = name;
	}
	name->head_name = head_name;
	name->generation = nth;
}

/* Parent is the first parent of the commit.  We may name it
 * as (n+1)th generation ancestor of the same head_name as
 * commit is nth generation ancestor of, if that generation
 * number is better than the name it already has.
 */
ସ୍ଥିର ଶୂନ୍ୟ name_parent(ସଂରଚନା commit *commit, ସଂରଚନା commit *parent)
{
	ସଂରଚନା commit_name *commit_name = commit_to_name(commit);
	ସଂରଚନା commit_name *parent_name = commit_to_name(parent);
	ଯଦି (!commit_name)
		ଫେରନ୍ତୁ;
	ଯଦି (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_first_parent_chain(ସଂରଚନା commit *c)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
	ଯେତେବେଳେ (c) {
		ସଂରଚନା commit *p;
		ଯଦି (!commit_to_name(c))
			ଭାଙ୍ଗ;
		ଯଦି (!c->parents)
			ଭାଙ୍ଗ;
		p = c->parents->item;
		ଯଦି (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		ଅନ୍ୟଥା
			ଭାଙ୍ଗ;
		c = p;
	}
	ଫେରନ୍ତୁ i;
}

ସ୍ଥିର ଶୂନ୍ୟ name_commits(ସଂରଚନା commit_list *list,
			 ସଂରଚନା commit **rev,
			 ଅକ୍ଷର **ref_name,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_rev)
{
	ସଂରଚନା commit_list *cl;
	ସଂରଚନା commit *c;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	/* First give names to the given heads */
	ପାଇଁ (cl = list; cl; cl = cl->next) {
		c = cl->item;
		ଯଦି (commit_to_name(c))
			ଜାରି;
		ପାଇଁ (i = 0; i < ଅଙ୍କ_rev; i++) {
			ଯଦି (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				ଭାଙ୍ଗ;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	କର {
		i = 0;
		ପାଇଁ (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} ଯେତେବେଳେ (i);

	/* Finally, any unnamed commits */
	କର {
		i = 0;
		ପାଇଁ (cl = list; cl; cl = cl->next) {
			ସଂରଚନା commit_list *parents;
			ସଂରଚନା commit_name *n;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା nth;
			c = cl->item;
			ଯଦି (!commit_to_name(c))
				ଜାରି;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			ଯେତେବେଳେ (parents) {
				ସଂରଚନା commit *p = parents->item;
				ସଂରଚନା strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				ଯଦି (commit_to_name(p))
					ଜାରି;
				ବାଛ (n->generation) {
				ସ୍ଥିତି 0:
					strbuf_addstr(&newname, n->head_name);
					ଭାଙ୍ଗ;
				ସ୍ଥିତି 1:
					strbuf_addf(&newname, "%s^", n->head_name);
					ଭାଙ୍ଗ;
				ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
					strbuf_addf(&newname, "%s~%d",
						    n->head_name, n->generation);
					ଭାଙ୍ଗ;
				}
				ଯଦି (nth == 1)
					strbuf_addch(&newname, '^');
				ଅନ୍ୟଥା
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, ଖାଲି), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} ଯେତେବେଳେ (i);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_seen(ସଂରଚନା commit *commit, ସଂରଚନା commit_list **seen_p)
{
	ଯଦି (!commit->object.flags) {
		commit_list_ଭର୍ତ୍ତି(commit, seen_p);
		ଫେରନ୍ତୁ 1;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ join_revs(ସଂରଚନା prio_queue *queue,
		      ସଂରଚନା commit_list **seen_p,
		      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_rev, ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_mask = ((1u << (REV_SHIFT + ଅଙ୍କ_rev)) - 1);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	ସଂରଚନା commit *commit;

	ଯେତେବେଳେ ((commit = prio_queue_peek(queue))) {
		ସଂରଚନା commit_list *parents;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା still_interesting = !!interesting(queue);
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = commit->object.flags & all_mask;

		ଯଦି (!still_interesting && extra <= 0)
			ଭାଙ୍ଗ;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		ଯଦି ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		ଯେତେବେଳେ (parents) {
			ସଂରଚନା commit *p = parents->item;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_flag = p->object.flags;
			parents = parents->next;
			ଯଦି ((this_flag & flags) == flags)
				ଜାରି;
			repo_parse_commit(the_repository, p);
			ଯଦି (mark_seen(p, seen_p) && !still_interesting)
				extra--;
			p->object.flags |= flags;
			prio_queue_put(queue, p);
		}
	}

	/*
	 * Postprocess to complete well-poisoning.
	 *
	 * At this point we have all the commits we have seen in
	 * seen_p list.  Mark anything that can be reached from
	 * uninteresting commits not interesting.
	 */
	ପାଇଁ (;;) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା changed = 0;
		ସଂରଚନା commit_list *s;
		ପାଇଁ (s = *seen_p; s; s = s->next) {
			ସଂରଚନା commit *c = s->item;
			ସଂରଚନା commit_list *parents;

			ଯଦି (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				ଜାରି;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			ଯେତେବେଳେ (parents) {
				ସଂରଚନା commit *p = parents->item;
				parents = parents->next;
				ଯଦି (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		ଯଦି (!changed)
			ଭାଙ୍ଗ;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ show_one_commit(ସଂରଚନା commit *commit, ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_name)
{
	ସଂରଚନା strbuf pretty = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *pretty_str = "(unavailable)";
	ସଂରଚନା commit_name *name = commit_to_name(commit);

	ଯଦି (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	ଯଦି (!no_name) {
		ଯଦି (name && name->head_name) {
			printf("[%s", name->head_name);
			ଯଦି (name->generation) {
				ଯଦି (name->generation == 1)
					printf("^");
				ଅନ୍ୟଥା
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		ଅନ୍ୟଥା
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

ସ୍ଥିର ଅକ୍ଷର *ref_name[MAX_REVS + 1];
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_name_cnt;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *find_digit_prefix(ଅଚଳ ଅକ୍ଷର *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *v)
{
	ଅଚଳ ଅକ୍ଷର *p;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ver;
	ଅକ୍ଷର ch;

	ପାଇଁ (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	ଫେରନ୍ତୁ p;
}


ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା version_cmp(ଅଚଳ ଅକ୍ଷର *a, ଅଚଳ ଅକ୍ଷର *b)
{
	ଯେତେବେଳେ (1) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		ଯଦି (va != vb)
			ଫେରନ୍ତୁ va - vb;

		ଯେତେବେଳେ (1) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା ca = *a;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା cb = *b;
			ଯଦି ('0' <= ca && ca <= '9')
				ca = 0;
			ଯଦି ('0' <= cb && cb <= '9')
				cb = 0;
			ଯଦି (ca != cb)
				ଫେରନ୍ତୁ ca - cb;
			ଯଦି (!ca)
				ଭାଙ୍ଗ;
			a++;
			b++;
		}
		ଯଦି (!*a && !*b)
			ଫେରନ୍ତୁ 0;
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compare_ref_name(ଅଚଳ ଶୂନ୍ୟ *a_, ଅଚଳ ଶୂନ୍ୟ *b_)
{
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ*a = a_, * ଅଚଳ*b = b_;
	ଫେରନ୍ତୁ version_cmp(*a, *b);
}

ସ୍ଥିର ଶୂନ୍ୟ sort_ref_range(ପୂର୍ଣ୍ଣସଂଖ୍ୟା bottom, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଶୀର୍ଷ)
{
	QSORT(ref_name + bottom, ଶୀର୍ଷ - bottom, compare_ref_name);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା append_ref(ଅଚଳ ଅକ୍ଷର *refname, ଅଚଳ ସଂରଚନା object_id *oid,
		      ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_dups)
{
	ସଂରଚନା commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (!commit)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!allow_dups) {
		/* Avoid adding the same thing twice */
		ପାଇଁ (i = 0; i < ref_name_cnt; i++)
			ଯଦି (!strcmp(refname, ref_name[i]))
				ଫେରନ୍ତୁ 0;
	}
	ଯଦି (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		ଫେରନ୍ତୁ 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = ଖାଲି;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା append_head_ref(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data UNUSED)
{
	ସଂରଚନା object_id tmp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ofs = 11;
	ଯଦି (!starts_with(ref->name, "refs/heads/"))
		ଫେରନ୍ତୁ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ଯଦି (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ଫେରନ୍ତୁ append_ref(ref->name + ofs, ref->oid, 0);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା append_remote_ref(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data UNUSED)
{
	ସଂରଚନା object_id tmp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ofs = 13;
	ଯଦି (!starts_with(ref->name, "refs/remotes/"))
		ଫେରନ୍ତୁ 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	ଯଦି (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	ଫେରନ୍ତୁ append_ref(ref->name + ofs, ref->oid, 0);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା append_tag_ref(ଅଚଳ ଅକ୍ଷର *refname, ଅଚଳ ସଂରଚନା object_id *oid,
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag UNUSED, ଶୂନ୍ୟ *cb_data UNUSED)
{
	ଯଦି (!starts_with(refname, "refs/tags/"))
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ append_ref(refname + 5, oid, 0);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *match_ref_pattern = ଖାଲି;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା match_ref_slash = 0;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା append_matching_ref(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	ଅଚଳ ଅକ୍ଷର *tail;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା slash = ଗଣନା_slashes(ref->name);
	ପାଇଁ (tail = ref->name; *tail && match_ref_slash < slash; )
		ଯଦି (*tail++ == '/')
			slash--;
	ଯଦି (!*tail)
		ଫେରନ୍ତୁ 0;
	ଯଦି (wildmatch(match_ref_pattern, tail, 0))
		ଫେରନ୍ତୁ 0;
	ଯଦି (starts_with(ref->name, "refs/heads/"))
		ଫେରନ୍ତୁ append_head_ref(ref, cb_data);
	ଯଦି (starts_with(ref->name, "refs/tags/"))
		ଫେରନ୍ତୁ append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	ଫେରନ୍ତୁ append_ref(ref->name, ref->oid, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ snarf_refs(ପୂର୍ଣ୍ଣସଂଖ୍ୟା head, ପୂର୍ଣ୍ଣସଂଖ୍ୟା remotes)
{
	ଯଦି (head) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, ଖାଲି);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	ଯଦି (remotes) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, ଖାଲି);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rev_is_head(ଅଚଳ ଅକ୍ଷର *head, ଅଚଳ ଅକ୍ଷର *name)
{
	ଯଦି (!head)
		ଫେରନ୍ତୁ 0;
	skip_prefix(head, "refs/heads/", &head);
	ଯଦି (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	ଫେରନ୍ତୁ !strcmp(head, name);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_merge_base(ଅଚଳ ସଂରଚନା commit_list *seen, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_rev)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_mask = ((1u << (REV_SHIFT + ଅଙ୍କ_rev)) - 1);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = 1;

	ପାଇଁ (ଅଚଳ ସଂରଚନା commit_list *s = seen; s; s = s->next) {
		ସଂରଚନା commit *commit = s->item;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = commit->object.flags & all_mask;
		ଯଦି (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	ଫେରନ୍ତୁ exit_status;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_independent(ସଂରଚନା commit **rev,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_rev,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rev_mask)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < ଅଙ୍କ_rev; i++) {
		ସଂରଚନା commit *commit = rev[i];
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag = rev_mask[i];

		ଯଦି (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ append_one_rev(ଅଚଳ ଅକ୍ଷର *av)
{
	ସଂରଚନା object_id revkey;
	ଯଦି (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		ଫେରନ୍ତୁ;
	}
	ଯଦି (strpbrk(av, "*?[")) {
		/* glob style match */
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = ଗଣନା_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, ଖାଲି);
		ଯଦି (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		ଫେରନ୍ତୁ;
	}
	die("bad sha1 reference %s", av);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_show_branch_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
				  ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(var, "showbranch.default")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		ଯଦି (!default_args.nr)
			strvec_ଠେଲ(&default_args, "show-branch");
		strvec_ଠେଲ(&default_args, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (git_color_config(var, ମୂଲ୍ୟ, cb) < 0)
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା omit_in_dense(ସଂରଚନା commit *commit, ସଂରଚନା commit **rev, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, flag, ଗଣନା_2;
	ପାଇଁ (i = 0; i < n; i++)
		ଯଦି (rev[i] == commit)
			ଫେରନ୍ତୁ 0;
	flag = commit->object.flags;
	ପାଇଁ (i = ଗଣନା_2 = 0; i < n; i++) {
		ଯଦି (flag & (1u << (i + REV_SHIFT)))
			ଗଣନା_2++;
	}
	ଯଦି (ଗଣନା_2 == 1)
		ଫେରନ୍ତୁ 1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା reflog = 0;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_reflog_param(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଅକ୍ଷର *ep;
	ଅଚଳ ଅକ୍ଷର **base = (ଅଚଳ ଅକ୍ଷର **)opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_NEG(unset);
	ଯଦି (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	ଯଦି (*ep == ',')
		*base = ep + 1;
	ଅନ୍ୟଥା ଯଦି (*ep)
		ଫେରନ୍ତୁ error("unrecognized reflog param '%s'", arg);
	ଅନ୍ୟଥା
		*base = ଖାଲି;
	ଯଦି (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_show_branch(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ac,
		ଅଚଳ ଅକ୍ଷର **av,
		ଅଚଳ ଅକ୍ଷର *prefix,
		ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା commit *rev[MAX_REVS], *commit;
	ଅକ୍ଷର *reflog_ସନ୍ଦେଶ[MAX_REVS] = {0};
	ସଂରଚନା commit_list *seen = ଖାଲି;
	ସଂରଚନା prio_queue queue = { compare_commits_by_commit_date };
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rev_mask[MAX_REVS];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_rev, i, extra = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_heads = 0, all_remotes = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା all_mask, all_revs;
	ଗଣନା rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	ଅକ୍ଷର *head;
	ସଂରଚନା object_id head_oid;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା merge_base = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା independent = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_name = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା sha1_name = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା shown_merge_point = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା with_current_branch = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା head_at = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା topics = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse = 0;
	ଅଚଳ ଅକ୍ଷର *reflog_base = ଖାଲି;
	ସଂରଚନା option builtin_show_branch_options[] = {
		OPT_BOOL('a', "all", &all_heads,
			 N_("show remote-tracking and local branches")),
		OPT_BOOL('r', "remotes", &all_remotes,
			 N_("show remote-tracking branches")),
		OPT__COLOR(&showbranch_use_color,
			    N_("color '*!+-' corresponding to the branch")),
		{
			.type = OPTION_INTEGER,
			.long_name = "more",
			.ମୂଲ୍ୟ = &extra,
			.precision = ଆକାର(extra),
			.argh = N_("n"),
			.help = N_("show <n> more commits after the common ancestor"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_SET_INT(0, "list", &extra, N_("synonym to more=-1"), -1),
		OPT_BOOL(0, "no-name", &no_name, N_("suppress naming strings")),
		OPT_BOOL(0, "current", &with_current_branch,
			 N_("include the current branch")),
		OPT_BOOL(0, "sha1-name", &sha1_name,
			 N_("name commits with their object names")),
		OPT_BOOL(0, "merge-base", &merge_base,
			 N_("show possible merge bases")),
		OPT_BOOL(0, "independent", &independent,
			    N_("show refs unreachable from any other ref")),
		OPT_SET_INT_F(0, "topo-order", &sort_order,
			      N_("show commits in topological order"),
			      REV_SORT_IN_GRAPH_ORDER, PARSE_OPT_NONEG),
		OPT_BOOL(0, "topics", &topics,
			 N_("show only commits not on the first branch")),
		OPT_SET_INT(0, "sparse", &sparse,
			    N_("show merges reachable from only one tip"), 1),
		OPT_SET_INT_F(0, "date-order", &sort_order,
			      N_("topologically sort, maintaining date order "
				 "where possible"),
			      REV_SORT_BY_COMMIT_DATE, PARSE_OPT_NONEG),
		OPT_CALLBACK_F('g', "reflog", &reflog_base, N_("<n>[,<base>]"),
			    N_("show <n> most recent ref-log entries starting at "
			       "base"),
			    PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			    parse_reflog_param),
		OPT_END()
	};
	ଅଚଳ ଅକ୍ଷର **args_copy = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, ଖାଲି);

	/* If nothing is specified, try the default first */
	ଯଦି (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	ଯଦି (all_heads)
		all_remotes = 1;

	ଯଦି (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		ଯଦି (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		ଯଦି (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	ଯଦି (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	ଯଦି (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	ଯଦି (reflog) {
		ସଂରଚନା object_id oid;
		ଅକ୍ଷର *ref;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା base = 0;
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;

		ଯଦି (ac == 0) {
			ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 ଖାଲି);
			fake_av[1] = ଖାଲି;
			av = fake_av;
			ac = 1;
			ଯଦି (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		ଯଦି (ac != 1)
			die(_("--reflog option needs one branch name"));

		ଯଦି (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		ଯଦି (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		ଯଦି (reflog_base) {
			ଅକ୍ଷର *ep;
			base = strtoul(reflog_base, &ep, 10);
			ଯଦି (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, ଖାଲି,
					    ଖାଲି, ଖାଲି, &base);
			}
		}

		ପାଇଁ (i = 0; i < reflog; i++) {
			ଅକ୍ଷର *logmsg = ଖାଲି;
			ଅକ୍ଷର *nth_desc;
			ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ;
			ଅକ୍ଷର *end;
			timestamp_t timestamp;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା tz;

			ଯଦି (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, ଖାଲି)) {
				ମୁକ୍ତ(logmsg);
				reflog = i;
				ଭାଙ୍ଗ;
			}

			end = strchr(logmsg, '\n');
			ଯଦି (end)
				*end = '\0';

			ସନ୍ଦେଶ = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_ସନ୍ଦେଶ[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_MODE(RELATIVE)),
						ସନ୍ଦେଶ);
			ମୁକ୍ତ(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, base+i);
			append_ref(nth_desc, &oid, 1);
			ମୁକ୍ତ(nth_desc);
		}
		ମୁକ୍ତ(ref);
	}
	ଅନ୍ୟଥା {
		ଯେତେବେଳେ (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		ଯଦି (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, ଖାଲି);

	ଯଦି (with_current_branch && head) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_head = 0;
		ପାଇଁ (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			ଯଦି (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		ଯଦି (!has_head) {
			ଅଚଳ ଅକ୍ଷର *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	ଯଦି (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		ଯାଅ out;
	}

	ପାଇଁ (ଅଙ୍କ_rev = 0; ref_name[ଅଙ୍କ_rev]; ଅଙ୍କ_rev++) {
		ସଂରଚନା object_id revkey;
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag = 1u << (ଅଙ୍କ_rev + REV_SHIFT);

		ଯଦି (MAX_REVS <= ଅଙ୍କ_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		ଯଦି (repo_get_oid(the_repository, ref_name[ଅଙ୍କ_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[ଅଙ୍କ_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		ଯଦି (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[ଅଙ୍କ_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		ଯଦି (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[ଅଙ୍କ_rev] = commit;
	}
	ପାଇଁ (i = 0; i < ଅଙ୍କ_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	ଯଦି (0 <= extra)
		join_revs(&queue, &seen, ଅଙ୍କ_rev, extra);

	commit_list_sort_by_date(&seen);

	ଯଦି (merge_base) {
		ret = show_merge_base(seen, ଅଙ୍କ_rev);
		ଯାଅ out;
	}

	ଯଦି (independent) {
		ret = show_independent(rev, ଅଙ୍କ_rev, rev_mask);
		ଯାଅ out;
	}

	/* Show list; --more=-1 means list-only */
	ଯଦି (1 < ଅଙ୍କ_rev || extra < 0) {
		ପାଇଁ (i = 0; i < ଅଙ୍କ_rev; i++) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			ଯଦି (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			ଅନ୍ୟଥା {
				ପାଇଁ (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			ଯଦି (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			ଅନ୍ୟଥା
				puts(reflog_ସନ୍ଦେଶ[i]);

			ଯଦି (is_head)
				head_at = i;
		}
		ଯଦି (0 <= extra) {
			ପାଇଁ (i = 0; i < ଅଙ୍କ_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	ଯଦି (extra < 0) {
		ret = 0;
		ଯାଅ out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	ଯଦି (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, ଅଙ୍କ_rev);

	all_mask = ((1u << (REV_SHIFT + ଅଙ୍କ_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	ପାଇଁ (ସଂରଚନା commit_list *l = seen; l; l = l->next) {
		ସଂରଚନା commit *commit = l->item;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_flag = commit->object.flags;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		ଯଦି (1 < ଅଙ୍କ_rev) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_merge = !!(commit->parents &&
					  commit->parents->next);
			ଯଦି (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				ଜାରି;
			ଯଦି (!sparse && is_merge &&
			    omit_in_dense(commit, rev, ଅଙ୍କ_rev))
				ଜାରି;
			ପାଇଁ (i = 0; i < ଅଙ୍କ_rev; i++) {
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark;
				ଯଦି (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				ଅନ୍ୟଥା ଯଦି (is_merge)
					mark = '-';
				ଅନ୍ୟଥା ଯଦି (i == head_at)
					mark = '*';
				ଅନ୍ୟଥା
					mark = '+';
				ଯଦି (mark == ' ')
					putchar(mark);
				ଅନ୍ୟଥା
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		ଯଦି (shown_merge_point && --extra < 0)
			ଭାଙ୍ଗ;
	}

	ret = 0;

out:
	ପାଇଁ (size_t i = 0; i < ARRAY_SIZE(reflog_ସନ୍ଦେଶ); i++)
		ମୁକ୍ତ(reflog_ସନ୍ଦେଶ[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	ମୁକ୍ତ(args_copy);
	ମୁକ୍ତ(head);
	ଫେରନ୍ତୁ ret;
}
