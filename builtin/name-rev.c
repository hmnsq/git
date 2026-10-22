#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "pager.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "hash-lookup.h"
#అన్తర్భావ "commit-slab.h"
#అన్తర్భావ "commit-graph.h"
#అన్తర్భావ "wildmatch.h"
#అన్తర్భావ "mem-pool.h"
#అన్తర్భావ "pretty.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "notes.h"
#అన్తర్భావ "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#నిర్వచించు CUTOFF_DATE_SLOP 86400

నిర్మాణం rev_name {
	స్థిరరాశి అక్షరం *tip_name;
	timestamp_t taggerdate;
	పూర్ణాంకం generation;
	పూర్ణాంకం దూరం;
	పూర్ణాంకం from_ట్యాగ్;
};

define_commit_slab(commit_rev_name, నిర్మాణం rev_name);

స్థిర timestamp_t generation_cutoff = GENERATION_సంఖ్య_INFINITY;
స్థిర timestamp_t cutoff = సమయం_MAX;
స్థిర నిర్మాణం commit_rev_name rev_పేర్లు;

/* Disable the cutoff checks entirely */
స్థిర శూన్య disable_cutoff(శూన్య)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
స్థిర శూన్య సమితి_commit_cutoff(నిర్మాణం commit *commit)
{

	ఒకవేళ (cutoff > commit->date)
		cutoff = commit->date;

	ఒకవేళ (generation_cutoff) {
		timestamp_t generation = commit_లేఖాచిత్రం_generation(commit);

		ఒకవేళ (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
స్థిర శూన్య adjust_cutoff_timestamp_for_slop(శూన్య)
{
	ఒకవేళ (cutoff) {
		/* check for underflow */
		ఒకవేళ (cutoff > సమయం_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		లేకపోతే
			cutoff = సమయం_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
స్థిర పూర్ణాంకం commit_is_before_cutoff(నిర్మాణం commit *commit)
{
	ఒకవేళ (generation_cutoff < GENERATION_సంఖ్య_INFINITY)
		తిరిగి generation_cutoff &&
			commit_లేఖాచిత్రం_generation(commit) < generation_cutoff;

	తిరిగి commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#నిర్వచించు విలీనం_TRAVERSAL_బరువు 65535

స్థిర పూర్ణాంకం is_valid_rev_name(స్థిరరాశి నిర్మాణం rev_name *name)
{
	తిరిగి name && name->tip_name;
}

స్థిర నిర్మాణం rev_name *get_commit_rev_name(స్థిరరాశి నిర్మాణం commit *commit)
{
	నిర్మాణం rev_name *name = commit_rev_name_peek(&rev_పేర్లు, commit);

	తిరిగి is_valid_rev_name(name) ? name : ఖాళీ;
}

స్థిర పూర్ణాంకం effective_దూరం(పూర్ణాంకం దూరం, పూర్ణాంకం generation)
{
	తిరిగి దూరం + (generation > 0 ? విలీనం_TRAVERSAL_బరువు : 0);
}

స్థిర పూర్ణాంకం is_better_name(నిర్మాణం rev_name *name,
			  timestamp_t taggerdate,
			  పూర్ణాంకం generation,
			  పూర్ణాంకం దూరం,
			  పూర్ణాంకం from_ట్యాగ్)
{
	పూర్ణాంకం name_దూరం = effective_దూరం(name->దూరం, name->generation);
	పూర్ణాంకం new_దూరం = effective_దూరం(దూరం, generation);

	/* If both are tags, we prefer the nearer one. */
	ఒకవేళ (from_ట్యాగ్ && name->from_ట్యాగ్)
		తిరిగి name_దూరం > new_దూరం;

	/* Favor a tag over a non-tag. */
	ఒకవేళ (name->from_ట్యాగ్ != from_ట్యాగ్)
		తిరిగి from_ట్యాగ్;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	ఒకవేళ (name_దూరం != new_దూరం)
		తిరిగి name_దూరం > new_దూరం;

	/* ... or tiebreak to favor older date */
	ఒకవేళ (name->taggerdate != taggerdate)
		తిరిగి name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	తిరిగి 0;
}

స్థిర నిర్మాణం rev_name *create_or_update_name(నిర్మాణం commit *commit,
					      timestamp_t taggerdate,
					      పూర్ణాంకం generation, పూర్ణాంకం దూరం,
					      పూర్ణాంకం from_ట్యాగ్)
{
	నిర్మాణం rev_name *name = commit_rev_name_at(&rev_పేర్లు, commit);

	ఒకవేళ (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, దూరం, from_ట్యాగ్))
		తిరిగి ఖాళీ;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->దూరం = దూరం;
	name->from_ట్యాగ్ = from_ట్యాగ్;

	తిరిగి name;
}

స్థిర అక్షరం *get_మాతృ_name(స్థిరరాశి నిర్మాణం rev_name *name, పూర్ణాంకం మాతృ_సంఖ్య,
			     నిర్మాణం mem_pool *string_pool)
{
	కొలత_t len;

	strip_suffix(name->tip_name, "^0", &len);
	ఒకవేళ (name->generation > 0) {
		తిరిగి mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (పూర్ణాంకం)len, name->tip_name,
				       name->generation, మాతృ_సంఖ్య);
	} లేకపోతే {
		తిరిగి mem_pool_strfmt(string_pool, "%.*s^%d",
				       (పూర్ణాంకం)len, name->tip_name, మాతృ_సంఖ్య);
	}
}

స్థిర శూన్య name_rev(నిర్మాణం commit *ప్రారంభం_commit,
		స్థిరరాశి అక్షరం *tip_name, timestamp_t taggerdate,
		పూర్ణాంకం from_ట్యాగ్, పూర్ణాంకం deref, నిర్మాణం mem_pool *string_pool)
{
	నిర్మాణం commit_దొంతర దొంతర = COMMIT_దొంతర_INIT;
	నిర్మాణం commit *commit;
	నిర్మాణం commit_దొంతర parents_గమ్యం_వరుస = COMMIT_దొంతర_INIT;
	నిర్మాణం rev_name *ప్రారంభం_name;

	repo_parse_commit(the_repository, ప్రారంభం_commit);
	ఒకవేళ (commit_is_before_cutoff(ప్రారంభం_commit))
		తిరిగి;

	ప్రారంభం_name = create_or_update_name(ప్రారంభం_commit, taggerdate, 0, 0,
					   from_ట్యాగ్);
	ఒకవేళ (!ప్రారంభం_name)
		తిరిగి;
	ఒకవేళ (deref)
		ప్రారంభం_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	లేకపోతే
		ప్రారంభం_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_దొంతర_నెట్టు(&దొంతర, ప్రారంభం_commit);

	అయితే ((commit = commit_దొంతర_తీయి(&దొంతర))) {
		నిర్మాణం rev_name *name = get_commit_rev_name(commit);
		నిర్మాణం commit_list *parents;
		పూర్ణాంకం మాతృ_సంఖ్య = 1;

		parents_గమ్యం_వరుస.nr = 0;

		కొరకు (parents = commit->parents;
				parents;
				parents = parents->తదుపరి, మాతృ_సంఖ్య++) {
			నిర్మాణం commit *మాతృ = parents->అంశం;
			నిర్మాణం rev_name *మాతృ_name;
			పూర్ణాంకం generation, దూరం;

			repo_parse_commit(the_repository, మాతృ);
			ఒకవేళ (commit_is_before_cutoff(మాతృ))
				కొనసాగు;

			ఒకవేళ (మాతృ_సంఖ్య > 1) {
				generation = 0;
				దూరం = name->దూరం + విలీనం_TRAVERSAL_బరువు;
			} లేకపోతే {
				generation = name->generation + 1;
				దూరం = name->దూరం + 1;
			}

			మాతృ_name = create_or_update_name(మాతృ, taggerdate,
							    generation,
							    దూరం, from_ట్యాగ్);
			ఒకవేళ (మాతృ_name) {
				ఒకవేళ (మాతృ_సంఖ్య > 1)
					మాతృ_name->tip_name =
						get_మాతృ_name(name,
								మాతృ_సంఖ్య,
								string_pool);
				లేకపోతే
					మాతృ_name->tip_name = name->tip_name;
				commit_దొంతర_నెట్టు(&parents_గమ్యం_వరుస, మాతృ);
			}
		}

		/* The first parent must come out first from the stack */
		అయితే (parents_గమ్యం_వరుస.nr)
			commit_దొంతర_నెట్టు(&దొంతర,
					  commit_దొంతర_తీయి(&parents_గమ్యం_వరుస));
	}

	commit_దొంతర_clear(&దొంతర);
	commit_దొంతర_clear(&parents_గమ్యం_వరుస);
}

స్థిర పూర్ణాంకం subpath_matches(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *filter)
{
	స్థిరరాశి అక్షరం *subpath = మార్గం;

	అయితే (subpath) {
		ఒకవేళ (!wildmatch(filter, subpath, 0))
			తిరిగి subpath - మార్గం;
		subpath = strchr(subpath, '/');
		ఒకవేళ (subpath)
			subpath++;
	}
	తిరిగి -1;
}

నిర్మాణం name_ref_దత్తాంశం {
	పూర్ణాంకం ట్యాగులు_only;
	పూర్ణాంకం name_only;
	నిర్మాణం string_list ref_filters;
	నిర్మాణం string_list exclude_filters;
};

నిర్మాణం pretty_format {
	నిర్మాణం pretty_print_context ctx;
	నిర్మాణం userformat_want want;
};

గణన command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

గణన stdin_బహుళకం {
    పాఠ్యం_2 = 1,
    REVS = 2,
};

నిర్మాణం command {
	గణన command_type type;
	సంఘం {
		పూర్ణాంకం name_only;
		నిర్మాణం pretty_format *pretty_format;
	} u;
};

స్థిర శూన్య init_name_rev_command(నిర్మాణం command *cmd,
				  పూర్ణాంకం name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

స్థిర శూన్య init_format_rev_command(నిర్మాణం command *cmd,
				    నిర్మాణం pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

స్థిర నిర్మాణం tip_పట్టిక {
	నిర్మాణం tip_పట్టిక_నమోదు {
		నిర్మాణం object_id oid;
		స్థిరరాశి అక్షరం *refname;
		నిర్మాణం commit *commit;
		timestamp_t taggerdate;
		గుర్తులేని పూర్ణాంకం from_ట్యాగ్:1;
		గుర్తులేని పూర్ణాంకం deref:1;
	} *పట్టిక;
	పూర్ణాంకం nr;
	పూర్ణాంకం alloc;
	పూర్ణాంకం sorted;
} tip_పట్టిక;

స్థిర శూన్య కూడు_గమ్యం_tip_పట్టిక(స్థిరరాశి నిర్మాణం object_id *oid, స్థిరరాశి అక్షరం *refname,
			     పూర్ణాంకం shorten_unambiguous, నిర్మాణం commit *commit,
			     timestamp_t taggerdate, పూర్ణాంకం from_ట్యాగ్, పూర్ణాంకం deref)
{
	అక్షరం *short_refname = ఖాళీ;

	ఒకవేళ (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	లేకపోతే ఒకవేళ (skip_ఉపసర్గ(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	లేకపోతే
		skip_ఉపసర్గ(refname, "refs/", &refname);

	ALLOC_GROW(tip_పట్టిక.పట్టిక, tip_పట్టిక.nr + 1, tip_పట్టిక.alloc);
	oidcpy(&tip_పట్టిక.పట్టిక[tip_పట్టిక.nr].oid, oid);
	tip_పట్టిక.పట్టిక[tip_పట్టిక.nr].refname = short_refname ?
		short_refname : xstrdup(refname);
	tip_పట్టిక.పట్టిక[tip_పట్టిక.nr].commit = commit;
	tip_పట్టిక.పట్టిక[tip_పట్టిక.nr].taggerdate = taggerdate;
	tip_పట్టిక.పట్టిక[tip_పట్టిక.nr].from_ట్యాగ్ = from_ట్యాగ్;
	tip_పట్టిక.పట్టిక[tip_పట్టిక.nr].deref = deref;
	tip_పట్టిక.nr++;
	tip_పట్టిక.sorted = 0;
}

స్థిర పూర్ణాంకం tipcmp(స్థిరరాశి శూన్య *a_, స్థిరరాశి శూన్య *b_)
{
	స్థిరరాశి నిర్మాణం tip_పట్టిక_నమోదు *a = a_, *b = b_;
	తిరిగి oidcmp(&a->oid, &b->oid);
}

స్థిర పూర్ణాంకం cmp_by_ట్యాగ్_and_age(స్థిరరాశి శూన్య *a_, స్థిరరాశి శూన్య *b_)
{
	స్థిరరాశి నిర్మాణం tip_పట్టిక_నమోదు *a = a_, *b = b_;
	పూర్ణాంకం cmp;

	/* Prefer tags. */
	cmp = b->from_ట్యాగ్ - a->from_ట్యాగ్;
	ఒకవేళ (cmp)
		తిరిగి cmp;

	/* Older is better. */
	ఒకవేళ (a->taggerdate < b->taggerdate)
		తిరిగి -1;
	తిరిగి a->taggerdate != b->taggerdate;
}

స్థిర పూర్ణాంకం name_ref(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం object *o = parse_object(the_repository, ref->oid);
	నిర్మాణం name_ref_దత్తాంశం *దత్తాంశం = cb_దత్తాంశం;
	పూర్ణాంకం can_abbreviate_output = దత్తాంశం->ట్యాగులు_only && దత్తాంశం->name_only;
	పూర్ణాంకం deref = 0;
	పూర్ణాంకం from_ట్యాగ్ = 0;
	నిర్మాణం commit *commit = ఖాళీ;
	timestamp_t taggerdate = సమయం_MAX;

	ఒకవేళ (దత్తాంశం->ట్యాగులు_only && !starts_with(ref->name, "refs/tags/"))
		తిరిగి 0;

	ఒకవేళ (దత్తాంశం->exclude_filters.nr) {
		నిర్మాణం string_list_అంశం *అంశం;

		for_each_string_list_అంశం(అంశం, &దత్తాంశం->exclude_filters) {
			ఒకవేళ (subpath_matches(ref->name, అంశం->string) >= 0)
				తిరిగి 0;
		}
	}

	ఒకవేళ (దత్తాంశం->ref_filters.nr) {
		నిర్మాణం string_list_అంశం *అంశం;
		పూర్ణాంకం matched = 0;

		/* See if any of the patterns match. */
		for_each_string_list_అంశం(అంశం, &దత్తాంశం->ref_filters) {
			/*
			 * Check all patterns even after finding a match, so
			 * that we can see if a match with a subpath exists.
			 * When a user asked for 'refs/tags/v*' and 'v1.*',
			 * both of which match, the user is showing her
			 * willingness to accept a shortened output by having
			 * the 'v1.*' in the acceptable refnames, so we
			 * shouldn't stop when seeing 'refs/tags/v1.4' matches
			 * 'refs/tags/v*'.  We should show it as 'v1.4'.
			 */
			ఎంపిక (subpath_matches(ref->name, అంశం->string)) {
			స్థితి -1: /* did not match */
				ఆపు;
			స్థితి 0: /* matched fully */
				matched = 1;
				ఆపు;
			అప్రమేయ: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				ఆపు;
			}
		}

		/* If none of the patterns matched, stop now */
		ఒకవేళ (!matched)
			తిరిగి 0;
	}

	అయితే (o && o->type == OBJ_ట్యాగ్) {
		నిర్మాణం ట్యాగ్ *t = (నిర్మాణం ట్యాగ్ *) o;
		ఒకవేళ (!t->tagged)
			ఆపు; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	ఒకవేళ (o && o->type == OBJ_COMMIT) {
		commit = (నిర్మాణం commit *)o;
		from_ట్యాగ్ = starts_with(ref->name, "refs/tags/");
		ఒకవేళ (taggerdate == సమయం_MAX)
			taggerdate = commit->date;
	}

	కూడు_గమ్యం_tip_పట్టిక(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_ట్యాగ్, deref);
	తిరిగి 0;
}

స్థిర శూన్య name_tips(నిర్మాణం mem_pool *string_pool)
{
	పూర్ణాంకం i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_పట్టిక.పట్టిక, tip_పట్టిక.nr, cmp_by_ట్యాగ్_and_age);
	కొరకు (i = 0; i < tip_పట్టిక.nr; i++) {
		నిర్మాణం tip_పట్టిక_నమోదు *e = &tip_పట్టిక.పట్టిక[i];
		ఒకవేళ (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_ట్యాగ్, e->deref, string_pool);
		}
	}
}

స్థిర స్థిరరాశి నిర్మాణం object_id *nth_tip_పట్టిక_ent(కొలత_t ix, స్థిరరాశి శూన్య *పట్టిక_2)
{
	స్థిరరాశి నిర్మాణం tip_పట్టిక_నమోదు *పట్టిక = పట్టిక_2;
	తిరిగి &పట్టిక[ix].oid;
}

స్థిర స్థిరరాశి అక్షరం *get_exact_ref_సరిపోలిక(స్థిరరాశి నిర్మాణం object *o)
{
	పూర్ణాంకం దొరికింది_2;

	ఒకవేళ (!tip_పట్టిక.పట్టిక || !tip_పట్టిక.nr)
		తిరిగి ఖాళీ;

	ఒకవేళ (!tip_పట్టిక.sorted) {
		QSORT(tip_పట్టిక.పట్టిక, tip_పట్టిక.nr, tipcmp);
		tip_పట్టిక.sorted = 1;
	}

	దొరికింది_2 = oid_pos(&o->oid, tip_పట్టిక.పట్టిక, tip_పట్టిక.nr,
			nth_tip_పట్టిక_ent);
	ఒకవేళ (0 <= దొరికింది_2)
		తిరిగి tip_పట్టిక.పట్టిక[దొరికింది_2].refname;
	తిరిగి ఖాళీ;
}

/* may return a constant string or use "buf" as scratch space */
స్థిర స్థిరరాశి అక్షరం *get_rev_name(స్థిరరాశి నిర్మాణం object *o, నిర్మాణం strbuf *buf)
{
	నిర్మాణం rev_name *n;
	స్థిరరాశి నిర్మాణం commit *c;

	ఒకవేళ (o->type != OBJ_COMMIT)
		తిరిగి get_exact_ref_సరిపోలిక(o);
	c = (స్థిరరాశి నిర్మాణం commit *) o;
	n = get_commit_rev_name(c);
	ఒకవేళ (!n)
		తిరిగి ఖాళీ;

	ఒకవేళ (!n->generation) {
		తిరిగి n->tip_name;
	} లేకపోతే {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		తిరిగి buf->buf;
	}
}

స్థిర స్థిరరాశి అక్షరం *get_format_rev(స్థిరరాశి నిర్మాణం commit *c,
				  నిర్మాణం pretty_format *format_ctx,
				  నిర్మాణం strbuf *buf)
{
	strbuf_reset(buf);

	ఒకవేళ (format_ctx->want.notes) {
		నిర్మాణం strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_సందేశం = strbuf_detach(&notebuf, ఖాళీ);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_సందేశం);

	తిరిగి buf->buf;
}

స్థిర శూన్య show_name(స్థిరరాశి నిర్మాణం object *obj,
		      స్థిరరాశి అక్షరం *caller_name,
		      పూర్ణాంకం always, పూర్ణాంకం allow_undefined, పూర్ణాంకం name_only)
{
	స్థిరరాశి అక్షరం *name;
	స్థిరరాశి నిర్మాణం object_id *oid = &obj->oid;
	నిర్మాణం strbuf buf = STRBUF_INIT;

	ఒకవేళ (!name_only)
		రాయి("%s ", caller_name ? caller_name : oid_గమ్యం_hex(oid));
	name = get_rev_name(obj, &buf);
	ఒకవేళ (name)
		రాయి("%s\n", name);
	లేకపోతే ఒకవేళ (allow_undefined)
		రాయి("undefined\n");
	లేకపోతే ఒకవేళ (always)
		రాయి("%s\n",
		       repo_కనుగొను_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	లేకపోతే
		die("cannot describe '%s'", oid_గమ్యం_hex(oid));
	strbuf_release(&buf);
}

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	ఖాళీ
};

స్థిర శూన్య name_rev_పంక్తి(అక్షరం *p, నిర్మాణం command *cmd)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	పూర్ణాంకం counter = 0;
	అక్షరం *p_ప్రారంభం;
	స్థిరరాశి గుర్తులేని hexsz = the_hash_algo->hexsz;

	కొరకు (p_ప్రారంభం = p; *p; p++) {
#నిర్వచించు ishex(x) (అంకెనా((x)) || ((x) >= 'a' && (x) <= 'f'))
		ఒకవేళ (!ishex(*p)) {
			counter = 0;
		} లేకపోతే ఒకవేళ (++counter == hexsz &&
			   !ishex(*(p + 1))) {
			నిర్మాణం object_id oid;
			స్థిరరాశి అక్షరం *name = ఖాళీ;
			అక్షరం c = *(p + 1);
			పూర్ణాంకం p_len = p - p_ప్రారంభం + 1;
			నిర్మాణం object *o = ఖాళీ;
			పూర్ణాంకం oid_ret = 1;

			counter = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			ఎంపిక (cmd->type) {
			స్థితి NAME_REV:
				ఒకవేళ (!oid_ret)
					o = lookup_object(the_repository, &oid);
				ఒకవేళ (o)
					name = get_rev_name(o, &buf);
				ఒకవేళ (!name)
					కొనసాగు;
				ఒకవేళ (cmd->u.name_only)
					రాయి("%.*s%s", p_len - hexsz, p_ప్రారంభం, name);
				లేకపోతే
					రాయి("%.*s (%s)", p_len, p_ప్రారంభం, name);
				ఆపు;
			స్థితి FORMAT_REV:
				ఒకవేళ (!oid_ret)
					o = parse_object(the_repository, &oid);
				ఒకవేళ (o && o->type == OBJ_COMMIT)
					name = get_format_rev((స్థిరరాశి నిర్మాణం commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				ఒకవేళ (name)
					రాయి("%.*s%s", p_len - hexsz, p_ప్రారంభం, name);
				లేకపోతే
					రాయి("%.*s", p_len, p_ప్రారంభం);
				ఆపు;
			అప్రమేయ:
				BUG("uncovered case: %d", cmd->type);
			}

			p_ప్రారంభం = p + 1;
		}
	}

	/* flush */
	ఒకవేళ (p_ప్రారంభం != p)
		fwrite(p_ప్రారంభం, p - p_ప్రారంభం, 1, stdout);

	strbuf_release(&buf);
}

పూర్ణాంకం cmd_name_rev(పూర్ణాంకం argc,
		 స్థిరరాశి అక్షరం **argv,
		 స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం mem_pool string_pool;
	నిర్మాణం object_array revs = OBJECT_ARRAY_INIT;

#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
	పూర్ణాంకం transform_stdin = 0;
#అయితే_ముగింపు
	పూర్ణాంకం all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_ట్యాగ్ = 0;
	నిర్మాణం name_ref_దత్తాంశం దత్తాంశం = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	నిర్మాణం command cmd;
	నిర్మాణం option opts[] = {
		OPT_BOOL(0, "name-only", &దత్తాంశం.name_only, N_("print only ref-based names (no object names)")),
		OPT_BOOL(0, "tags", &దత్తాంశం.ట్యాగులు_only, N_("only use tags to name the commits")),
		OPT_STRING_LIST(0, "refs", &దత్తాంశం.ref_filters, N_("pattern"),
				N_("only use refs matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &దత్తాంశం.exclude_filters, N_("pattern"),
				N_("ignore refs matching <pattern>")),
		OPT_సమూహం(""),
		OPT_BOOL(0, "all", &all, N_("list all commits reachable from all refs")),
#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
		OPT_BOOL_F(0,
			   "stdin",
			   &transform_stdin,
			   N_("deprecated: use --annotate-stdin instead"),
			   PARSE_OPT_HIDDEN),
#అయితే_ముగింపు /* WITH_BREAKING_CHANGES */
		OPT_BOOL(0, "annotate-stdin", &annotate_stdin, N_("annotate text from stdin")),
		OPT_BOOL(0, "undefined", &allow_undefined, N_("allow to print `undefined` names (default)")),
		OPT_BOOL(0, "always", &always,
			 N_("show abbreviated commit object as fallback")),
		OPT_HIDDEN_BOOL(0, "peel-tag", &peel_ట్యాగ్,
				N_("dereference tags in the input (internal use)")),
		OPT_చివర(),
	};

	mem_pool_init(&string_pool, 0);
	init_commit_rev_name(&rev_పేర్లు);
	repo_config(the_repository, git_default_config, ఖాళీ);
	argc = parse_options(argc, argv, ఉపసర్గ, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, దత్తాంశం.name_only);

#నిర్వచితం_కాకపోతే WITH_BREAKING_CHANGES
	ఒకవేళ (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#అయితే_ముగింపు

	ఒకవేళ (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	ఒకవేళ (all || annotate_stdin)
		disable_cutoff();

	కొరకు (; argc; argc--, argv++) {
		నిర్మాణం object_id oid;
		నిర్మాణం object *object;
		నిర్మాణం commit *commit;

		ఒకవేళ (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			కొనసాగు;
		}

		commit = ఖాళీ;
		object = parse_object(the_repository, &oid);
		ఒకవేళ (object) {
			నిర్మాణం object *peeled = deref_ట్యాగ్(the_repository,
							  object, *argv, 0);
			ఒకవేళ (peeled && peeled->type == OBJ_COMMIT)
				commit = (నిర్మాణం commit *)peeled;
		}

		ఒకవేళ (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			కొనసాగు;
		}

		ఒకవేళ (commit)
			సమితి_commit_cutoff(commit);

		ఒకవేళ (peel_ట్యాగ్) {
			ఒకవేళ (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				కొనసాగు;
			}
			object = (నిర్మాణం object *)commit;
		}
		కూడు_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &దత్తాంశం);
	name_tips(&string_pool);

	ఒకవేళ (annotate_stdin) {
		నిర్మాణం strbuf sb = STRBUF_INIT;

		అయితే (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_పంక్తి(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} లేకపోతే ఒకవేళ (all) {
		పూర్ణాంకం i, max;

		max = get_max_object_సూచీ(the_repository);
		కొరకు (i = 0; i < max; i++) {
			నిర్మాణం object *obj = get_సూచీకృత_object(the_repository, i);
			ఒకవేళ (!obj || obj->type != OBJ_COMMIT)
				కొనసాగు;
			show_name(obj, ఖాళీ,
				  always, allow_undefined, దత్తాంశం.name_only);
		}
	} లేకపోతే {
		పూర్ణాంకం i;
		కొరకు (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].అంశం, revs.objects[i].name,
				  always, allow_undefined, దత్తాంశం.name_only);
	}

	string_list_clear(&దత్తాంశం.ref_filters, 0);
	string_list_clear(&దత్తాంశం.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	తిరిగి 0;
}

నిర్మాణం format_nul_దత్తాంశం {
	bool nul_ఆదానం;
	bool nul_output;
};

స్థిర పూర్ణాంకం format_nul_cb(స్థిరరాశి నిర్మాణం option *option,
			 స్థిరరాశి అక్షరం *arg,
			 పూర్ణాంకం unset)
{
	నిర్మాణం format_nul_దత్తాంశం *దత్తాంశం = option->విలువ;
	దత్తాంశం->nul_ఆదానం = 1;
	దత్తాంశం->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	తిరిగి 0;
}

స్థిర గణన stdin_బహుళకం parse_stdin_బహుళకం(స్థిరరాశి అక్షరం *stdin_బహుళకం)
{
	ఒకవేళ (!strcmp(stdin_బహుళకం, "text"))
		తిరిగి పాఠ్యం_2;
	లేకపోతే ఒకవేళ (!strcmp(stdin_బహుళకం, "revs") ||
		 !strcmp(stdin_బహుళకం, "rev"))
		తిరిగి REVS;
	లేకపోతే
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	ఖాళీ
};

పూర్ణాంకం cmd_format_rev(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	స్థిరరాశి అక్షరం *format = ఖాళీ;
	గణన stdin_బహుళకం stdin_బహుళకం;
	స్థిరరాశి అక్షరం *stdin_బహుళకం_arg = ఖాళీ;
	నిర్మాణం format_nul_దత్తాంశం nul_దత్తాంశం = { 0, 0 };
	అక్షరం output_terminator;
	strbuf_getline_fn getline_fn;
	నిర్మాణం display_notes_opt format_notes_opt;
	నిర్మాణం rev_info format_rev = REV_INFO_INIT;
	నిర్మాణం pretty_format format_pp = { 0 };
	నిర్మాణం string_list notes = STRING_LIST_INIT_NODUP;
	నిర్మాణం strbuf scratch_buf = STRBUF_INIT;
	నిర్మాణం command cmd;
	నిర్మాణం option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_బహుళకం_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_దత్తాంశం, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_దత్తాంశం.nul_ఆదానం,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_దత్తాంశం.nul_output,
			 N_("Use NUL for output termination")),
		OPT_చివర(),
	};

	argc = parse_options(argc, argv, ఉపసర్గ, opts, format_rev_usage, 0);

	ఒకవేళ (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	ఒకవేళ (!format)
		die(_("'%s' is required"), "--format");
	ఒకవేళ (!stdin_బహుళకం_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_దత్తాంశం.nul_ఆదానం ? strbuf_getline_nul : strbuf_getline_lf;
	output_terminator = nul_దత్తాంశం.nul_output ? '\0' : '\n';

	init_display_notes(&format_notes_opt);
	stdin_బహుళకం = parse_stdin_బహుళకం(stdin_బహుళకం_arg);

	get_commit_format(format, &format_rev);
	format_pp.ctx.rev = &format_rev;
	format_pp.ctx.fmt = format_rev.commit_format;
	format_pp.ctx.abbrev = format_rev.abbrev;
	format_pp.ctx.date_బహుళకం_explicit = format_rev.date_బహుళకం_explicit;
	format_pp.ctx.date_బహుళకం = format_rev.date_బహుళకం;
	format_pp.ctx.color = GIT_COLOR_AUTO;

	userformat_కనుగొను_requirements(format,
				     &format_pp.want);
	ఒకవేళ (format_pp.want.notes) {
		పూర్ణాంకం ignore_show_notes = 0;
		నిర్మాణం string_list_అంశం *n;

		for_each_string_list_అంశం(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		ఎక్కించు_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	ఎంపిక (stdin_బహుళకం) {
	స్థితి పాఠ్యం_2:
		అయితే (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_పంక్తి(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			రాయి("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		ఆపు;
	స్థితి REVS:
		అయితే (getline_fn(&scratch_buf, stdin) != EOF) {
			నిర్మాణం object_id oid;
			నిర్మాణం object *object;
			నిర్మాణం object *peeled;

			ఒకవేళ (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				కొనసాగు;
			}

			object = parse_object(the_repository, &oid);
			ఒకవేళ (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				కొనసాగు;
			}

			peeled = deref_ట్యాగ్(the_repository, object, scratch_buf.buf, 0);
			ఒకవేళ (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				కొనసాగు;
			}

			get_format_rev((నిర్మాణం commit *)peeled,
				       &format_pp, &scratch_buf);
			రాయి("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		ఆపు;
	అప్రమేయ:
		BUG("uncovered case: %d", stdin_బహుళకం);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	తిరిగి 0;
}
