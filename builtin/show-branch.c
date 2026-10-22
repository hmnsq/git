#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "pretty.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "color.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"

#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "commit-slab.h"
#அந்தர்பாவ "date.h"
#அந்தர்பாவ "wildmatch.h"
#அந்தர்பாவ "prio-queue.h"

நிலையான மாறிலி எழுத்து*மாறிலி show_branch_usage[] = {
    N_("git show-branch [-a | --all] [-r | --remotes] [--topo-order | --date-order]\n"
       "                [--current] [--color[=<when>] | --no-color] [--sparse]\n"
       "                [--more=<n> | --list | --independent | --merge-base]\n"
       "                [--no-name | --sha1-name] [--topics]\n"
       "                [(<rev> | <glob>)...]"),
    N_("git show-branch (-g | --reflog)[=<n>[,<base>]] [--list] [<ref>]"),
    வெற்று
};

நிலையான எண்ணிக்கை git_colorbool showbranch_use_color = GIT_COLOR_UNKNOWN;

நிலையான கட்டமைப்பு strvec default_args = STRVEC_INIT;

/*
 * TODO: convert this use of commit->object.flags to commit-slab
 * instead to store a pointer to ref name directly. Then use the same
 * UNINTERESTING definition from revision.h here.
 */
#define UNINTERESTING	01

#define REV_SHIFT	 2
#define MAX_REVS	(FLAG_BITS - REV_SHIFT) /* should not exceed bits_per_int - REV_SHIFT */

#define DEFAULT_REFLOG	4

நிலையான மாறிலி எழுத்து *get_color_code(முழு idx)
{
	எனில் (want_color(showbranch_use_color))
		திரும்பு column_colors_ansi[idx % column_colors_ansi_max];
	திரும்பு "";
}

நிலையான மாறிலி எழுத்து *get_color_reset_code(வெறுமை)
{
	எனில் (want_color(showbranch_use_color))
		திரும்பு GIT_COLOR_RESET;
	திரும்பு "";
}

நிலையான கட்டமைப்பு commit *interesting(கட்டமைப்பு prio_queue *queue)
{
	கட்டமைப்பு commit *commit;
	prio_queue_for_each(queue, commit) {
		எனில் (!(commit->object.flags & UNINTERESTING))
			திரும்பு commit;
	}
	திரும்பு வெற்று;
}

கட்டமைப்பு commit_name {
	மாறிலி எழுத்து *head_name; /* which head's ancestor? */
	முழு generation; /* how many parents away from head_name */
};

define_commit_slab(commit_name_slab, கட்டமைப்பு commit_name *);
நிலையான கட்டமைப்பு commit_name_slab name_slab;

நிலையான கட்டமைப்பு commit_name *commit_to_name(கட்டமைப்பு commit *commit)
{
	திரும்பு *commit_name_slab_at(&name_slab, commit);
}


/* Name the commit as nth generation ancestor of head_name;
 * we count only the first-parent relationship for naming purposes.
 */
நிலையான வெறுமை name_commit(கட்டமைப்பு commit *commit, மாறிலி எழுத்து *head_name, முழு nth)
{
	கட்டமைப்பு commit_name *name;

	name = *commit_name_slab_at(&name_slab, commit);
	எனில் (!name) {
		name = xmalloc(அளவு(*name));
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
நிலையான வெறுமை name_parent(கட்டமைப்பு commit *commit, கட்டமைப்பு commit *parent)
{
	கட்டமைப்பு commit_name *commit_name = commit_to_name(commit);
	கட்டமைப்பு commit_name *parent_name = commit_to_name(parent);
	எனில் (!commit_name)
		திரும்பு;
	எனில் (!parent_name ||
	    commit_name->generation + 1 < parent_name->generation)
		name_commit(parent, commit_name->head_name,
			    commit_name->generation + 1);
}

நிலையான முழு name_first_parent_chain(கட்டமைப்பு commit *c)
{
	முழு i = 0;
	போது (c) {
		கட்டமைப்பு commit *p;
		எனில் (!commit_to_name(c))
			நிறுத்து;
		எனில் (!c->parents)
			நிறுத்து;
		p = c->parents->item;
		எனில் (!commit_to_name(p)) {
			name_parent(c, p);
			i++;
		}
		இல்லையெனில்
			நிறுத்து;
		c = p;
	}
	திரும்பு i;
}

நிலையான வெறுமை name_commits(கட்டமைப்பு commit_list *list,
			 கட்டமைப்பு commit **rev,
			 எழுத்து **ref_name,
			 முழு இலக்கம்_rev)
{
	கட்டமைப்பு commit_list *cl;
	கட்டமைப்பு commit *c;
	முழு i;

	/* First give names to the given heads */
	க்கு (cl = list; cl; cl = cl->next) {
		c = cl->item;
		எனில் (commit_to_name(c))
			தொடர்;
		க்கு (i = 0; i < இலக்கம்_rev; i++) {
			எனில் (rev[i] == c) {
				name_commit(c, ref_name[i], 0);
				நிறுத்து;
			}
		}
	}

	/* Then commits on the first parent ancestry chain */
	செய் {
		i = 0;
		க்கு (cl = list; cl; cl = cl->next) {
			i += name_first_parent_chain(cl->item);
		}
	} போது (i);

	/* Finally, any unnamed commits */
	செய் {
		i = 0;
		க்கு (cl = list; cl; cl = cl->next) {
			கட்டமைப்பு commit_list *parents;
			கட்டமைப்பு commit_name *n;
			முழு nth;
			c = cl->item;
			எனில் (!commit_to_name(c))
				தொடர்;
			n = commit_to_name(c);
			parents = c->parents;
			nth = 0;
			போது (parents) {
				கட்டமைப்பு commit *p = parents->item;
				கட்டமைப்பு strbuf newname = STRBUF_INIT;
				parents = parents->next;
				nth++;
				எனில் (commit_to_name(p))
					தொடர்;
				தேர்வு (n->generation) {
				நிலை 0:
					strbuf_addstr(&newname, n->head_name);
					நிறுத்து;
				நிலை 1:
					strbuf_addf(&newname, "%s^", n->head_name);
					நிறுத்து;
				இயல்பு:
					strbuf_addf(&newname, "%s~%d",
						    n->head_name, n->generation);
					நிறுத்து;
				}
				எனில் (nth == 1)
					strbuf_addch(&newname, '^');
				இல்லையெனில்
					strbuf_addf(&newname, "^%d", nth);
				name_commit(p, strbuf_detach(&newname, வெற்று), 0);
				i++;
				name_first_parent_chain(p);
			}
		}
	} போது (i);
}

நிலையான முழு mark_seen(கட்டமைப்பு commit *commit, கட்டமைப்பு commit_list **seen_p)
{
	எனில் (!commit->object.flags) {
		commit_list_செருகு(commit, seen_p);
		திரும்பு 1;
	}
	திரும்பு 0;
}

நிலையான வெறுமை join_revs(கட்டமைப்பு prio_queue *queue,
		      கட்டமைப்பு commit_list **seen_p,
		      முழு இலக்கம்_rev, முழு extra)
{
	முழு all_mask = ((1u << (REV_SHIFT + இலக்கம்_rev)) - 1);
	முழு all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	கட்டமைப்பு commit *commit;

	போது ((commit = prio_queue_peek(queue))) {
		கட்டமைப்பு commit_list *parents;
		முழு still_interesting = !!interesting(queue);
		முழு flags = commit->object.flags & all_mask;

		எனில் (!still_interesting && extra <= 0)
			நிறுத்து;

		prio_queue_get(queue);

		mark_seen(commit, seen_p);
		எனில் ((flags & all_revs) == all_revs)
			flags |= UNINTERESTING;
		parents = commit->parents;

		போது (parents) {
			கட்டமைப்பு commit *p = parents->item;
			முழு this_flag = p->object.flags;
			parents = parents->next;
			எனில் ((this_flag & flags) == flags)
				தொடர்;
			repo_parse_commit(the_repository, p);
			எனில் (mark_seen(p, seen_p) && !still_interesting)
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
	க்கு (;;) {
		முழு changed = 0;
		கட்டமைப்பு commit_list *s;
		க்கு (s = *seen_p; s; s = s->next) {
			கட்டமைப்பு commit *c = s->item;
			கட்டமைப்பு commit_list *parents;

			எனில் (((c->object.flags & all_revs) != all_revs) &&
			    !(c->object.flags & UNINTERESTING))
				தொடர்;

			/* The current commit is either a merge base or
			 * already uninteresting one.  Mark its parents
			 * as uninteresting commits _only_ if they are
			 * already parsed.  No reason to find new ones
			 * here.
			 */
			parents = c->parents;
			போது (parents) {
				கட்டமைப்பு commit *p = parents->item;
				parents = parents->next;
				எனில் (!(p->object.flags & UNINTERESTING)) {
					p->object.flags |= UNINTERESTING;
					changed = 1;
				}
			}
		}
		எனில் (!changed)
			நிறுத்து;
	}
}

நிலையான வெறுமை show_one_commit(கட்டமைப்பு commit *commit, முழு no_name)
{
	கட்டமைப்பு strbuf pretty = STRBUF_INIT;
	மாறிலி எழுத்து *pretty_str = "(unavailable)";
	கட்டமைப்பு commit_name *name = commit_to_name(commit);

	எனில் (commit->object.parsed) {
		pp_commit_easy(CMIT_FMT_ONELINE, commit, &pretty);
		pretty_str = pretty.buf;
	}
	skip_prefix(pretty_str, "[PATCH] ", &pretty_str);

	எனில் (!no_name) {
		எனில் (name && name->head_name) {
			printf("[%s", name->head_name);
			எனில் (name->generation) {
				எனில் (name->generation == 1)
					printf("^");
				இல்லையெனில்
					printf("~%d", name->generation);
			}
			printf("] ");
		}
		இல்லையெனில்
			printf("[%s] ",
			       repo_find_unique_abbrev(the_repository, &commit->object.oid,
						       DEFAULT_ABBREV));
	}
	puts(pretty_str);
	strbuf_release(&pretty);
}

நிலையான எழுத்து *ref_name[MAX_REVS + 1];
நிலையான முழு ref_name_cnt;

நிலையான மாறிலி எழுத்து *find_digit_prefix(மாறிலி எழுத்து *s, முழு *v)
{
	மாறிலி எழுத்து *p;
	முழு ver;
	எழுத்து ch;

	க்கு (p = s, ver = 0;
	     '0' <= (ch = *p) && ch <= '9';
	     p++)
		ver = ver * 10 + ch - '0';
	*v = ver;
	திரும்பு p;
}


நிலையான முழு version_cmp(மாறிலி எழுத்து *a, மாறிலி எழுத்து *b)
{
	போது (1) {
		முழு va, vb;

		a = find_digit_prefix(a, &va);
		b = find_digit_prefix(b, &vb);
		எனில் (va != vb)
			திரும்பு va - vb;

		போது (1) {
			முழு ca = *a;
			முழு cb = *b;
			எனில் ('0' <= ca && ca <= '9')
				ca = 0;
			எனில் ('0' <= cb && cb <= '9')
				cb = 0;
			எனில் (ca != cb)
				திரும்பு ca - cb;
			எனில் (!ca)
				நிறுத்து;
			a++;
			b++;
		}
		எனில் (!*a && !*b)
			திரும்பு 0;
	}
}

நிலையான முழு compare_ref_name(மாறிலி வெறுமை *a_, மாறிலி வெறுமை *b_)
{
	மாறிலி எழுத்து * மாறிலி*a = a_, * மாறிலி*b = b_;
	திரும்பு version_cmp(*a, *b);
}

நிலையான வெறுமை sort_ref_range(முழு bottom, முழு உச்சி)
{
	QSORT(ref_name + bottom, உச்சி - bottom, compare_ref_name);
}

நிலையான முழு append_ref(மாறிலி எழுத்து *refname, மாறிலி கட்டமைப்பு object_id *oid,
		      முழு allow_dups)
{
	கட்டமைப்பு commit *commit = lookup_commit_reference_gently(the_repository,
							       oid, 1);
	முழு i;

	எனில் (!commit)
		திரும்பு 0;

	எனில் (!allow_dups) {
		/* Avoid adding the same thing twice */
		க்கு (i = 0; i < ref_name_cnt; i++)
			எனில் (!strcmp(refname, ref_name[i]))
				திரும்பு 0;
	}
	எனில் (MAX_REVS <= ref_name_cnt) {
		warning(Q_("ignoring %s; cannot handle more than %d ref",
			   "ignoring %s; cannot handle more than %d refs",
			   MAX_REVS), refname, MAX_REVS);
		திரும்பு 0;
	}
	ref_name[ref_name_cnt++] = xstrdup(refname);
	ref_name[ref_name_cnt] = வெற்று;
	திரும்பு 0;
}

நிலையான முழு append_head_ref(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data UNUSED)
{
	கட்டமைப்பு object_id tmp;
	முழு ofs = 11;
	எனில் (!starts_with(ref->name, "refs/heads/"))
		திரும்பு 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	எனில் (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	திரும்பு append_ref(ref->name + ofs, ref->oid, 0);
}

நிலையான முழு append_remote_ref(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data UNUSED)
{
	கட்டமைப்பு object_id tmp;
	முழு ofs = 13;
	எனில் (!starts_with(ref->name, "refs/remotes/"))
		திரும்பு 0;
	/* If both heads/foo and tags/foo exists, get_sha1 would
	 * get confused.
	 */
	எனில் (repo_get_oid(the_repository, ref->name + ofs, &tmp) || !oideq(&tmp, ref->oid))
		ofs = 5;
	திரும்பு append_ref(ref->name + ofs, ref->oid, 0);
}

நிலையான முழு append_tag_ref(மாறிலி எழுத்து *refname, மாறிலி கட்டமைப்பு object_id *oid,
			  முழு flag UNUSED, வெறுமை *cb_data UNUSED)
{
	எனில் (!starts_with(refname, "refs/tags/"))
		திரும்பு 0;
	திரும்பு append_ref(refname + 5, oid, 0);
}

நிலையான மாறிலி எழுத்து *match_ref_pattern = வெற்று;
நிலையான முழு match_ref_slash = 0;

நிலையான முழு append_matching_ref(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	/* we want to allow pattern hold/<asterisk> to show all
	 * branches under refs/heads/hold/, and v0.99.9? to show
	 * refs/tags/v0.99.9a and friends.
	 */
	மாறிலி எழுத்து *tail;
	முழு slash = மொத்தம்_slashes(ref->name);
	க்கு (tail = ref->name; *tail && match_ref_slash < slash; )
		எனில் (*tail++ == '/')
			slash--;
	எனில் (!*tail)
		திரும்பு 0;
	எனில் (wildmatch(match_ref_pattern, tail, 0))
		திரும்பு 0;
	எனில் (starts_with(ref->name, "refs/heads/"))
		திரும்பு append_head_ref(ref, cb_data);
	எனில் (starts_with(ref->name, "refs/tags/"))
		திரும்பு append_tag_ref(ref->name, ref->oid, ref->flags, cb_data);
	திரும்பு append_ref(ref->name, ref->oid, 0);
}

நிலையான வெறுமை snarf_refs(முழு head, முழு remotes)
{
	எனில் (head) {
		முழு orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_head_ref, வெற்று);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
	எனில் (remotes) {
		முழு orig_cnt = ref_name_cnt;

		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_remote_ref, வெற்று);
		sort_ref_range(orig_cnt, ref_name_cnt);
	}
}

நிலையான முழு rev_is_head(மாறிலி எழுத்து *head, மாறிலி எழுத்து *name)
{
	எனில் (!head)
		திரும்பு 0;
	skip_prefix(head, "refs/heads/", &head);
	எனில் (!skip_prefix(name, "refs/heads/", &name))
		skip_prefix(name, "heads/", &name);
	திரும்பு !strcmp(head, name);
}

நிலையான முழு show_merge_base(மாறிலி கட்டமைப்பு commit_list *seen, முழு இலக்கம்_rev)
{
	முழு all_mask = ((1u << (REV_SHIFT + இலக்கம்_rev)) - 1);
	முழு all_revs = all_mask & ~((1u << REV_SHIFT) - 1);
	முழு exit_status = 1;

	க்கு (மாறிலி கட்டமைப்பு commit_list *s = seen; s; s = s->next) {
		கட்டமைப்பு commit *commit = s->item;
		முழு flags = commit->object.flags & all_mask;
		எனில் (!(flags & UNINTERESTING) &&
		    ((flags & all_revs) == all_revs)) {
			puts(oid_to_hex(&commit->object.oid));
			exit_status = 0;
			commit->object.flags |= UNINTERESTING;
		}
	}
	திரும்பு exit_status;
}

நிலையான முழு show_independent(கட்டமைப்பு commit **rev,
			    முழு இலக்கம்_rev,
			    unsigned முழு *rev_mask)
{
	முழு i;

	க்கு (i = 0; i < இலக்கம்_rev; i++) {
		கட்டமைப்பு commit *commit = rev[i];
		unsigned முழு flag = rev_mask[i];

		எனில் (commit->object.flags == flag)
			puts(oid_to_hex(&commit->object.oid));
		commit->object.flags |= UNINTERESTING;
	}
	திரும்பு 0;
}

நிலையான வெறுமை append_one_rev(மாறிலி எழுத்து *av)
{
	கட்டமைப்பு object_id revkey;
	எனில் (!repo_get_oid(the_repository, av, &revkey)) {
		append_ref(av, &revkey, 0);
		திரும்பு;
	}
	எனில் (strpbrk(av, "*?[")) {
		/* glob style match */
		முழு saved_matches = ref_name_cnt;

		match_ref_pattern = av;
		match_ref_slash = மொத்தம்_slashes(av);
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_matching_ref, வெற்று);
		எனில் (saved_matches == ref_name_cnt &&
		    ref_name_cnt < MAX_REVS)
			error(_("no matching refs with %s"), av);
		sort_ref_range(saved_matches, ref_name_cnt);
		திரும்பு;
	}
	die("bad sha1 reference %s", av);
}

நிலையான முழு git_show_branch_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
				  மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(var, "showbranch.default")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		/*
		 * default_arg is now passed to parse_options(), so we need to
		 * mimic the real argv a bit better.
		 */
		எனில் (!default_args.nr)
			strvec_தள்ளு(&default_args, "show-branch");
		strvec_தள்ளு(&default_args, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "color.showbranch")) {
		showbranch_use_color = git_config_colorbool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (git_color_config(var, மதிப்பு, cb) < 0)
		திரும்பு -1;

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

நிலையான முழு omit_in_dense(கட்டமைப்பு commit *commit, கட்டமைப்பு commit **rev, முழு n)
{
	/* If the commit is tip of the named branches, do not
	 * omit it.
	 * Otherwise, if it is a merge that is reachable from only one
	 * tip, it is not that interesting.
	 */
	முழு i, flag, மொத்தம்;
	க்கு (i = 0; i < n; i++)
		எனில் (rev[i] == commit)
			திரும்பு 0;
	flag = commit->object.flags;
	க்கு (i = மொத்தம் = 0; i < n; i++) {
		எனில் (flag & (1u << (i + REV_SHIFT)))
			மொத்தம்++;
	}
	எனில் (மொத்தம் == 1)
		திரும்பு 1;
	திரும்பு 0;
}

நிலையான முழு reflog = 0;

நிலையான முழு parse_reflog_param(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			      முழு unset)
{
	எழுத்து *ep;
	மாறிலி எழுத்து **base = (மாறிலி எழுத்து **)opt->மதிப்பு;
	BUG_ON_OPT_NEG(unset);
	எனில் (!arg)
		arg = "";
	reflog = strtoul(arg, &ep, 10);
	எனில் (*ep == ',')
		*base = ep + 1;
	இல்லையெனில் எனில் (*ep)
		திரும்பு error("unrecognized reflog param '%s'", arg);
	இல்லையெனில்
		*base = வெற்று;
	எனில் (reflog <= 0)
		reflog = DEFAULT_REFLOG;
	திரும்பு 0;
}

முழு cmd_show_branch(முழு ac,
		மாறிலி எழுத்து **av,
		மாறிலி எழுத்து *prefix,
		கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு commit *rev[MAX_REVS], *commit;
	எழுத்து *reflog_செய்தி[MAX_REVS] = {0};
	கட்டமைப்பு commit_list *seen = வெற்று;
	கட்டமைப்பு prio_queue queue = { compare_commits_by_commit_date };
	unsigned முழு rev_mask[MAX_REVS];
	முழு இலக்கம்_rev, i, extra = 0;
	முழு all_heads = 0, all_remotes = 0;
	முழு all_mask, all_revs;
	எண்ணிக்கை rev_sort_order sort_order = REV_SORT_IN_GRAPH_ORDER;
	எழுத்து *head;
	கட்டமைப்பு object_id head_oid;
	முழு merge_base = 0;
	முழு independent = 0;
	முழு no_name = 0;
	முழு sha1_name = 0;
	முழு shown_merge_point = 0;
	முழு with_current_branch = 0;
	முழு head_at = -1;
	முழு topics = 0;
	முழு sparse = 0;
	மாறிலி எழுத்து *reflog_base = வெற்று;
	கட்டமைப்பு option builtin_show_branch_options[] = {
		OPT_BOOL('a', "all", &all_heads,
			 N_("show remote-tracking and local branches")),
		OPT_BOOL('r', "remotes", &all_remotes,
			 N_("show remote-tracking branches")),
		OPT__COLOR(&showbranch_use_color,
			    N_("color '*!+-' corresponding to the branch")),
		{
			.type = OPTION_INTEGER,
			.long_name = "more",
			.மதிப்பு = &extra,
			.precision = அளவு(extra),
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
	மாறிலி எழுத்து **args_copy = வெற்று;
	முழு ret;

	init_commit_name_slab(&name_slab);

	repo_config(the_repository, git_show_branch_config, வெற்று);

	/* If nothing is specified, try the default first */
	எனில் (ac == 1 && default_args.nr) {
		DUP_ARRAY(args_copy, default_args.v, default_args.nr);
		ac = default_args.nr;
		av = args_copy;
	}

	ac = parse_options(ac, av, prefix, builtin_show_branch_options,
			   show_branch_usage, PARSE_OPT_STOP_AT_NON_OPTION);
	எனில் (all_heads)
		all_remotes = 1;

	எனில் (extra || reflog) {
		/* "listing" mode is incompatible with
		 * independent nor merge-base modes.
		 */
		எனில் (independent || merge_base)
			usage_with_options(show_branch_usage,
					   builtin_show_branch_options);
		எனில் (reflog && ((0 < extra) || all_heads || all_remotes))
			/*
			 * Asking for --more in reflog mode does not
			 * make sense.  --list is Ok.
			 *
			 * Also --all and --remotes do not make sense either.
			 */
			die(_("options '%s' and '%s' cannot be used together"), "--reflog",
				"--all/--remotes/--independent/--merge-base");
	}

	எனில் (with_current_branch && reflog)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--reflog", "--current");

	/* If nothing is specified, show all branches by default */
	எனில் (ac <= topics && all_heads + all_remotes == 0)
		all_heads = 1;

	எனில் (reflog) {
		கட்டமைப்பு object_id oid;
		எழுத்து *ref;
		முழு base = 0;
		unsigned முழு flags = 0;

		எனில் (ac == 0) {
			நிலையான மாறிலி எழுத்து *fake_av[2];

			fake_av[0] = refs_resolve_refdup(get_main_ref_store(the_repository),
							 "HEAD",
							 RESOLVE_REF_READING,
							 &oid,
							 வெற்று);
			fake_av[1] = வெற்று;
			av = fake_av;
			ac = 1;
			எனில் (!*av)
				die(_("no branches given, and HEAD is not valid"));
		}
		எனில் (ac != 1)
			die(_("--reflog option needs one branch name"));

		எனில் (MAX_REVS < reflog)
			die(Q_("only %d entry can be shown at one time.",
			       "only %d entries can be shown at one time.",
			       MAX_REVS), MAX_REVS);
		எனில் (!repo_dwim_ref(the_repository, *av, strlen(*av), &oid,
				   &ref, 0))
			die(_("no such ref %s"), *av);

		/* Has the base been specified? */
		எனில் (reflog_base) {
			எழுத்து *ep;
			base = strtoul(reflog_base, &ep, 10);
			எனில் (*ep) {
				/* Ah, that is a date spec... */
				timestamp_t at;
				at = approxidate(reflog_base);
				read_ref_at(get_main_ref_store(the_repository),
					    ref, flags, at, -1, &oid, வெற்று,
					    வெற்று, வெற்று, &base);
			}
		}

		க்கு (i = 0; i < reflog; i++) {
			எழுத்து *logmsg = வெற்று;
			எழுத்து *nth_desc;
			மாறிலி எழுத்து *செய்தி;
			எழுத்து *end;
			timestamp_t timestamp;
			முழு tz;

			எனில் (read_ref_at(get_main_ref_store(the_repository),
					ref, flags, 0, base + i, &oid, &logmsg,
					&timestamp, &tz, வெற்று)) {
				விடுவி(logmsg);
				reflog = i;
				நிறுத்து;
			}

			end = strchr(logmsg, '\n');
			எனில் (end)
				*end = '\0';

			செய்தி = (*logmsg == '\0') ? "(none)" : logmsg;
			reflog_செய்தி[i] = xstrfmt("(%s) %s",
						show_date(timestamp, tz,
							  DATE_MODE(RELATIVE)),
						செய்தி);
			விடுவி(logmsg);

			nth_desc = xstrfmt("%s@{%d}", *av, base+i);
			append_ref(nth_desc, &oid, 1);
			விடுவி(nth_desc);
		}
		விடுவி(ref);
	}
	இல்லையெனில் {
		போது (0 < ac) {
			append_one_rev(*av);
			ac--; av++;
		}
		எனில் (all_heads + all_remotes)
			snarf_refs(all_heads, all_remotes);
	}

	head = refs_resolve_refdup(get_main_ref_store(the_repository), "HEAD",
				   RESOLVE_REF_READING,
				   &head_oid, வெற்று);

	எனில் (with_current_branch && head) {
		முழு has_head = 0;
		க்கு (i = 0; !has_head && i < ref_name_cnt; i++) {
			/* We are only interested in adding the branch
			 * HEAD points at.
			 */
			எனில் (rev_is_head(head, ref_name[i]))
				has_head++;
		}
		எனில் (!has_head) {
			மாறிலி எழுத்து *name = head;
			skip_prefix(name, "refs/heads/", &name);
			append_one_rev(name);
		}
	}

	எனில் (!ref_name_cnt) {
		fprintf(stderr, "No revs to be shown.\n");
		ret = 0;
		செல் out;
	}

	க்கு (இலக்கம்_rev = 0; ref_name[இலக்கம்_rev]; இலக்கம்_rev++) {
		கட்டமைப்பு object_id revkey;
		unsigned முழு flag = 1u << (இலக்கம்_rev + REV_SHIFT);

		எனில் (MAX_REVS <= இலக்கம்_rev)
			die(Q_("cannot handle more than %d rev.",
			       "cannot handle more than %d revs.",
			       MAX_REVS), MAX_REVS);
		எனில் (repo_get_oid(the_repository, ref_name[இலக்கம்_rev], &revkey))
			die(_("'%s' is not a valid ref."), ref_name[இலக்கம்_rev]);
		commit = lookup_commit_reference(the_repository, &revkey);
		எனில் (!commit)
			die(_("cannot find commit %s (%s)"),
			    ref_name[இலக்கம்_rev], oid_to_hex(&revkey));
		repo_parse_commit(the_repository, commit);
		mark_seen(commit, &seen);

		/* rev#0 uses bit REV_SHIFT, rev#1 uses bit REV_SHIFT+1,
		 * and so on.  REV_SHIFT bits from bit 0 are used for
		 * internal bookkeeping.
		 */
		commit->object.flags |= flag;
		எனில் (commit->object.flags == flag)
			prio_queue_put(&queue, commit);
		rev[இலக்கம்_rev] = commit;
	}
	க்கு (i = 0; i < இலக்கம்_rev; i++)
		rev_mask[i] = rev[i]->object.flags;

	எனில் (0 <= extra)
		join_revs(&queue, &seen, இலக்கம்_rev, extra);

	commit_list_sort_by_date(&seen);

	எனில் (merge_base) {
		ret = show_merge_base(seen, இலக்கம்_rev);
		செல் out;
	}

	எனில் (independent) {
		ret = show_independent(rev, இலக்கம்_rev, rev_mask);
		செல் out;
	}

	/* Show list; --more=-1 means list-only */
	எனில் (1 < இலக்கம்_rev || extra < 0) {
		க்கு (i = 0; i < இலக்கம்_rev; i++) {
			முழு j;
			முழு is_head = rev_is_head(head, ref_name[i]) &&
				      oideq(&head_oid, &rev[i]->object.oid);
			எனில் (extra < 0)
				printf("%c [%s] ",
				       is_head ? '*' : ' ', ref_name[i]);
			இல்லையெனில் {
				க்கு (j = 0; j < i; j++)
					putchar(' ');
				printf("%s%c%s [%s] ",
				       get_color_code(i),
				       is_head ? '*' : '!',
				       get_color_reset_code(), ref_name[i]);
			}

			எனில் (!reflog) {
				/* header lines never need name */
				show_one_commit(rev[i], 1);
			}
			இல்லையெனில்
				puts(reflog_செய்தி[i]);

			எனில் (is_head)
				head_at = i;
		}
		எனில் (0 <= extra) {
			க்கு (i = 0; i < இலக்கம்_rev; i++)
				putchar('-');
			putchar('\n');
		}
	}
	எனில் (extra < 0) {
		ret = 0;
		செல் out;
	}

	/* Sort topologically */
	sort_in_topological_order(&seen, sort_order);

	/* Give names to commits */
	எனில் (!sha1_name && !no_name)
		name_commits(seen, rev, ref_name, இலக்கம்_rev);

	all_mask = ((1u << (REV_SHIFT + இலக்கம்_rev)) - 1);
	all_revs = all_mask & ~((1u << REV_SHIFT) - 1);

	க்கு (கட்டமைப்பு commit_list *l = seen; l; l = l->next) {
		கட்டமைப்பு commit *commit = l->item;
		முழு this_flag = commit->object.flags;
		முழு is_merge_point = ((this_flag & all_revs) == all_revs);

		shown_merge_point |= is_merge_point;

		எனில் (1 < இலக்கம்_rev) {
			முழு is_merge = !!(commit->parents &&
					  commit->parents->next);
			எனில் (topics &&
			    !is_merge_point &&
			    (this_flag & (1u << REV_SHIFT)))
				தொடர்;
			எனில் (!sparse && is_merge &&
			    omit_in_dense(commit, rev, இலக்கம்_rev))
				தொடர்;
			க்கு (i = 0; i < இலக்கம்_rev; i++) {
				முழு mark;
				எனில் (!(this_flag & (1u << (i + REV_SHIFT))))
					mark = ' ';
				இல்லையெனில் எனில் (is_merge)
					mark = '-';
				இல்லையெனில் எனில் (i == head_at)
					mark = '*';
				இல்லையெனில்
					mark = '+';
				எனில் (mark == ' ')
					putchar(mark);
				இல்லையெனில்
					printf("%s%c%s",
					       get_color_code(i),
					       mark, get_color_reset_code());
			}
			putchar(' ');
		}
		show_one_commit(commit, no_name);

		எனில் (shown_merge_point && --extra < 0)
			நிறுத்து;
	}

	ret = 0;

out:
	க்கு (size_t i = 0; i < ARRAY_SIZE(reflog_செய்தி); i++)
		விடுவி(reflog_செய்தி[i]);
	commit_list_free(seen);
	clear_prio_queue(&queue);
	விடுவி(args_copy);
	விடுவி(head);
	திரும்பு ret;
}
