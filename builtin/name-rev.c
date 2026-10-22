#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "pager.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "hash-lookup.h"
#अन्तर्भाव "commit-slab.h"
#अन्तर्भाव "commit-graph.h"
#अन्तर्भाव "wildmatch.h"
#अन्तर्भाव "mem-pool.h"
#अन्तर्भाव "pretty.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "notes.h"
#अन्तर्भाव "write-or-die.h"

/*
 * One day.  See the 'name a rev shortly after epoch' test in t6120 when
 * changing this value
 */
#परिभाषय CUTOFF_DATE_SLOP 86400

संरचना rev_name {
	ध्रुव वर्ण *tip_name;
	timestamp_t taggerdate;
	पूर्णाङ्क generation;
	पूर्णाङ्क distance;
	पूर्णाङ्क from_चिह्नकम्;
};

define_commit_slab(commit_rev_name, संरचना rev_name);

स्थिर timestamp_t generation_cutoff = GENERATION_सङ्ख्या_INFINITY;
स्थिर timestamp_t cutoff = TIME_MAX;
स्थिर संरचना commit_rev_name rev_नामानि;

/* Disable the cutoff checks entirely */
स्थिर शून्य disable_cutoff(शून्य)
{
	generation_cutoff = 0;
	cutoff = 0;
}

/* Cutoff searching any commits older than this one */
स्थिर शून्य समुच्चयः_commit_cutoff(संरचना commit *commit)
{

	यदि (cutoff > commit->date)
		cutoff = commit->date;

	यदि (generation_cutoff) {
		timestamp_t generation = commit_graph_generation(commit);

		यदि (generation_cutoff > generation)
			generation_cutoff = generation;
	}
}

/* adjust the commit date cutoff with a slop to allow for slightly incorrect
 * commit timestamps in case of clock skew.
 */
स्थिर शून्य adjust_cutoff_timestamp_for_slop(शून्य)
{
	यदि (cutoff) {
		/* check for underflow */
		यदि (cutoff > TIME_MIN + CUTOFF_DATE_SLOP)
			cutoff = cutoff - CUTOFF_DATE_SLOP;
		अन्यथा
			cutoff = TIME_MIN;
	}
}

/* Check if a commit is before the cutoff. Prioritize generation numbers
 * first, but use the commit timestamp if we lack generation data.
 */
स्थिर पूर्णाङ्क commit_is_before_cutoff(संरचना commit *commit)
{
	यदि (generation_cutoff < GENERATION_सङ्ख्या_INFINITY)
		प्रत्यागम generation_cutoff &&
			commit_graph_generation(commit) < generation_cutoff;

	प्रत्यागम commit->date < cutoff;
}

/* How many generations are maximally preferred over _one_ merge traversal? */
#परिभाषय MERGE_TRAVERSAL_WEIGHT 65535

स्थिर पूर्णाङ्क is_valid_rev_name(ध्रुव संरचना rev_name *name)
{
	प्रत्यागम name && name->tip_name;
}

स्थिर संरचना rev_name *get_commit_rev_name(ध्रुव संरचना commit *commit)
{
	संरचना rev_name *name = commit_rev_name_peek(&rev_नामानि, commit);

	प्रत्यागम is_valid_rev_name(name) ? name : रिक्त;
}

स्थिर पूर्णाङ्क effective_distance(पूर्णाङ्क distance, पूर्णाङ्क generation)
{
	प्रत्यागम distance + (generation > 0 ? MERGE_TRAVERSAL_WEIGHT : 0);
}

स्थिर पूर्णाङ्क is_better_name(संरचना rev_name *name,
			  timestamp_t taggerdate,
			  पूर्णाङ्क generation,
			  पूर्णाङ्क distance,
			  पूर्णाङ्क from_चिह्नकम्)
{
	पूर्णाङ्क name_distance = effective_distance(name->distance, name->generation);
	पूर्णाङ्क new_distance = effective_distance(distance, generation);

	/* If both are tags, we prefer the nearer one. */
	यदि (from_चिह्नकम् && name->from_चिह्नकम्)
		प्रत्यागम name_distance > new_distance;

	/* Favor a tag over a non-tag. */
	यदि (name->from_चिह्नकम् != from_चिह्नकम्)
		प्रत्यागम from_चिह्नकम्;

	/*
	 * We are now looking at two non-tags.  Tiebreak to favor
	 * shorter hops.
	 */
	यदि (name_distance != new_distance)
		प्रत्यागम name_distance > new_distance;

	/* ... or tiebreak to favor older date */
	यदि (name->taggerdate != taggerdate)
		प्रत्यागम name->taggerdate > taggerdate;

	/* keep the current one if we cannot decide */
	प्रत्यागम 0;
}

स्थिर संरचना rev_name *create_or_update_name(संरचना commit *commit,
					      timestamp_t taggerdate,
					      पूर्णाङ्क generation, पूर्णाङ्क distance,
					      पूर्णाङ्क from_चिह्नकम्)
{
	संरचना rev_name *name = commit_rev_name_at(&rev_नामानि, commit);

	यदि (is_valid_rev_name(name) &&
	    !is_better_name(name, taggerdate, generation, distance, from_चिह्नकम्))
		प्रत्यागम रिक्त;

	name->taggerdate = taggerdate;
	name->generation = generation;
	name->distance = distance;
	name->from_चिह्नकम् = from_चिह्नकम्;

	प्रत्यागम name;
}

स्थिर वर्ण *get_parent_name(ध्रुव संरचना rev_name *name, पूर्णाङ्क parent_सङ्ख्या,
			     संरचना mem_pool *string_pool)
{
	आकार_t len;

	strip_suffix(name->tip_name, "^0", &len);
	यदि (name->generation > 0) {
		प्रत्यागम mem_pool_strfmt(string_pool, "%.*s~%d^%d",
				       (पूर्णाङ्क)len, name->tip_name,
				       name->generation, parent_सङ्ख्या);
	} अन्यथा {
		प्रत्यागम mem_pool_strfmt(string_pool, "%.*s^%d",
				       (पूर्णाङ्क)len, name->tip_name, parent_सङ्ख्या);
	}
}

स्थिर शून्य name_rev(संरचना commit *start_commit,
		ध्रुव वर्ण *tip_name, timestamp_t taggerdate,
		पूर्णाङ्क from_चिह्नकम्, पूर्णाङ्क deref, संरचना mem_pool *string_pool)
{
	संरचना commit_राशिः राशिः = COMMIT_राशिः_INIT;
	संरचना commit *commit;
	संरचना commit_राशिः parents_गन्तव्यम्_queue = COMMIT_राशिः_INIT;
	संरचना rev_name *start_name;

	repo_parse_commit(the_repository, start_commit);
	यदि (commit_is_before_cutoff(start_commit))
		प्रत्यागम;

	start_name = create_or_update_name(start_commit, taggerdate, 0, 0,
					   from_चिह्नकम्);
	यदि (!start_name)
		प्रत्यागम;
	यदि (deref)
		start_name->tip_name = mem_pool_strfmt(string_pool, "%s^0",
						       tip_name);
	अन्यथा
		start_name->tip_name = mem_pool_strdup(string_pool, tip_name);

	commit_राशिः_प्रक्षिप(&राशिः, start_commit);

	यावत् ((commit = commit_राशिः_निष्कासय(&राशिः))) {
		संरचना rev_name *name = get_commit_rev_name(commit);
		संरचना commit_list *parents;
		पूर्णाङ्क parent_सङ्ख्या = 1;

		parents_गन्तव्यम्_queue.nr = 0;

		प्रति (parents = commit->parents;
				parents;
				parents = parents->अग्र, parent_सङ्ख्या++) {
			संरचना commit *parent = parents->item;
			संरचना rev_name *parent_name;
			पूर्णाङ्क generation, distance;

			repo_parse_commit(the_repository, parent);
			यदि (commit_is_before_cutoff(parent))
				अनुवर्त;

			यदि (parent_सङ्ख्या > 1) {
				generation = 0;
				distance = name->distance + MERGE_TRAVERSAL_WEIGHT;
			} अन्यथा {
				generation = name->generation + 1;
				distance = name->distance + 1;
			}

			parent_name = create_or_update_name(parent, taggerdate,
							    generation,
							    distance, from_चिह्नकम्);
			यदि (parent_name) {
				यदि (parent_सङ्ख्या > 1)
					parent_name->tip_name =
						get_parent_name(name,
								parent_सङ्ख्या,
								string_pool);
				अन्यथा
					parent_name->tip_name = name->tip_name;
				commit_राशिः_प्रक्षिप(&parents_गन्तव्यम्_queue, parent);
			}
		}

		/* The first parent must come out first from the stack */
		यावत् (parents_गन्तव्यम्_queue.nr)
			commit_राशिः_प्रक्षिप(&राशिः,
					  commit_राशिः_निष्कासय(&parents_गन्तव्यम्_queue));
	}

	commit_राशिः_clear(&राशिः);
	commit_राशिः_clear(&parents_गन्तव्यम्_queue);
}

स्थिर पूर्णाङ्क subpath_matches(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *filter)
{
	ध्रुव वर्ण *subpath = मार्गः;

	यावत् (subpath) {
		यदि (!wildmatch(filter, subpath, 0))
			प्रत्यागम subpath - मार्गः;
		subpath = strchr(subpath, '/');
		यदि (subpath)
			subpath++;
	}
	प्रत्यागम -1;
}

संरचना name_ref_दत्त {
	पूर्णाङ्क चिह्नकानि_only;
	पूर्णाङ्क name_only;
	संरचना string_list ref_filters;
	संरचना string_list exclude_filters;
};

संरचना pretty_format {
	संरचना pretty_print_context ctx;
	संरचना userformat_want want;
};

गणना command_type {
	NAME_REV = 1,
	FORMAT_REV = 2,
};

गणना stdin_बहुलकम् {
    TEXT = 1,
    REVS = 2,
};

संरचना command {
	गणना command_type type;
	सङ्घ {
		पूर्णाङ्क name_only;
		संरचना pretty_format *pretty_format;
	} u;
};

स्थिर शून्य init_name_rev_command(संरचना command *cmd,
				  पूर्णाङ्क name_only)
{
	cmd->type = NAME_REV;
	cmd->u.name_only = name_only;
}

स्थिर शून्य init_format_rev_command(संरचना command *cmd,
				    संरचना pretty_format *pretty_format)
{
	cmd->type = FORMAT_REV;
	cmd->u.pretty_format = pretty_format;
}

स्थिर संरचना tip_सारणी {
	संरचना tip_सारणी_प्रविष्टिः {
		संरचना object_id oid;
		ध्रुव वर्ण *refname;
		संरचना commit *commit;
		timestamp_t taggerdate;
		अचिह्नित पूर्णाङ्क from_चिह्नकम्:1;
		अचिह्नित पूर्णाङ्क deref:1;
	} *सारणी;
	पूर्णाङ्क nr;
	पूर्णाङ्क alloc;
	पूर्णाङ्क sorted;
} tip_सारणी;

स्थिर शून्य सङ्कलय_गन्तव्यम्_tip_सारणी(ध्रुव संरचना object_id *oid, ध्रुव वर्ण *refname,
			     पूर्णाङ्क shorten_unambiguous, संरचना commit *commit,
			     timestamp_t taggerdate, पूर्णाङ्क from_चिह्नकम्, पूर्णाङ्क deref)
{
	वर्ण *short_refname = रिक्त;

	यदि (shorten_unambiguous)
		short_refname = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
							     refname, 0);
	अन्यथा यदि (skip_उपसर्गः(refname, "refs/heads/", &refname))
		; /* refname already advanced */
	अन्यथा
		skip_उपसर्गः(refname, "refs/", &refname);

	ALLOC_GROW(tip_सारणी.सारणी, tip_सारणी.nr + 1, tip_सारणी.alloc);
	oidcpy(&tip_सारणी.सारणी[tip_सारणी.nr].oid, oid);
	tip_सारणी.सारणी[tip_सारणी.nr].refname = short_refname ?
		short_refname : xstrdup(refname);
	tip_सारणी.सारणी[tip_सारणी.nr].commit = commit;
	tip_सारणी.सारणी[tip_सारणी.nr].taggerdate = taggerdate;
	tip_सारणी.सारणी[tip_सारणी.nr].from_चिह्नकम् = from_चिह्नकम्;
	tip_सारणी.सारणी[tip_सारणी.nr].deref = deref;
	tip_सारणी.nr++;
	tip_सारणी.sorted = 0;
}

स्थिर पूर्णाङ्क tipcmp(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	ध्रुव संरचना tip_सारणी_प्रविष्टिः *a = a_, *b = b_;
	प्रत्यागम oidcmp(&a->oid, &b->oid);
}

स्थिर पूर्णाङ्क cmp_by_चिह्नकम्_and_age(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	ध्रुव संरचना tip_सारणी_प्रविष्टिः *a = a_, *b = b_;
	पूर्णाङ्क cmp;

	/* Prefer tags. */
	cmp = b->from_चिह्नकम् - a->from_चिह्नकम्;
	यदि (cmp)
		प्रत्यागम cmp;

	/* Older is better. */
	यदि (a->taggerdate < b->taggerdate)
		प्रत्यागम -1;
	प्रत्यागम a->taggerdate != b->taggerdate;
}

स्थिर पूर्णाङ्क name_ref(ध्रुव संरचना reference *ref, शून्य *cb_दत्त)
{
	संरचना object *o = parse_object(the_repository, ref->oid);
	संरचना name_ref_दत्त *दत्त = cb_दत्त;
	पूर्णाङ्क can_abbreviate_output = दत्त->चिह्नकानि_only && दत्त->name_only;
	पूर्णाङ्क deref = 0;
	पूर्णाङ्क from_चिह्नकम् = 0;
	संरचना commit *commit = रिक्त;
	timestamp_t taggerdate = TIME_MAX;

	यदि (दत्त->चिह्नकानि_only && !starts_with(ref->name, "refs/tags/"))
		प्रत्यागम 0;

	यदि (दत्त->exclude_filters.nr) {
		संरचना string_list_item *item;

		for_each_string_list_item(item, &दत्त->exclude_filters) {
			यदि (subpath_matches(ref->name, item->string) >= 0)
				प्रत्यागम 0;
		}
	}

	यदि (दत्त->ref_filters.nr) {
		संरचना string_list_item *item;
		पूर्णाङ्क matched = 0;

		/* See if any of the patterns match. */
		for_each_string_list_item(item, &दत्त->ref_filters) {
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
			विकल्प (subpath_matches(ref->name, item->string)) {
			पक्ष -1: /* did not match */
				भङ्ग;
			पक्ष 0: /* matched fully */
				matched = 1;
				भङ्ग;
			स्वभाव: /* matched subpath */
				matched = 1;
				can_abbreviate_output = 1;
				भङ्ग;
			}
		}

		/* If none of the patterns matched, stop now */
		यदि (!matched)
			प्रत्यागम 0;
	}

	यावत् (o && o->type == OBJ_चिह्नकम्) {
		संरचना चिह्नकम् *t = (संरचना चिह्नकम् *) o;
		यदि (!t->tagged)
			भङ्ग; /* broken repository */
		o = parse_object(the_repository, &t->tagged->oid);
		deref = 1;
		taggerdate = t->date;
	}
	यदि (o && o->type == OBJ_COMMIT) {
		commit = (संरचना commit *)o;
		from_चिह्नकम् = starts_with(ref->name, "refs/tags/");
		यदि (taggerdate == TIME_MAX)
			taggerdate = commit->date;
	}

	सङ्कलय_गन्तव्यम्_tip_सारणी(ref->oid, ref->name, can_abbreviate_output,
			 commit, taggerdate, from_चिह्नकम्, deref);
	प्रत्यागम 0;
}

स्थिर शून्य name_tips(संरचना mem_pool *string_pool)
{
	पूर्णाङ्क i;

	/*
	 * Try to set better names first, so that worse ones spread
	 * less.
	 */
	QSORT(tip_सारणी.सारणी, tip_सारणी.nr, cmp_by_चिह्नकम्_and_age);
	प्रति (i = 0; i < tip_सारणी.nr; i++) {
		संरचना tip_सारणी_प्रविष्टिः *e = &tip_सारणी.सारणी[i];
		यदि (e->commit) {
			name_rev(e->commit, e->refname, e->taggerdate,
				 e->from_चिह्नकम्, e->deref, string_pool);
		}
	}
}

स्थिर ध्रुव संरचना object_id *nth_tip_सारणी_ent(आकार_t ix, ध्रुव शून्य *सारणी_2)
{
	ध्रुव संरचना tip_सारणी_प्रविष्टिः *सारणी = सारणी_2;
	प्रत्यागम &सारणी[ix].oid;
}

स्थिर ध्रुव वर्ण *get_exact_ref_मेलनम्(ध्रुव संरचना object *o)
{
	पूर्णाङ्क प्राप्तम्_2;

	यदि (!tip_सारणी.सारणी || !tip_सारणी.nr)
		प्रत्यागम रिक्त;

	यदि (!tip_सारणी.sorted) {
		QSORT(tip_सारणी.सारणी, tip_सारणी.nr, tipcmp);
		tip_सारणी.sorted = 1;
	}

	प्राप्तम्_2 = oid_pos(&o->oid, tip_सारणी.सारणी, tip_सारणी.nr,
			nth_tip_सारणी_ent);
	यदि (0 <= प्राप्तम्_2)
		प्रत्यागम tip_सारणी.सारणी[प्राप्तम्_2].refname;
	प्रत्यागम रिक्त;
}

/* may return a constant string or use "buf" as scratch space */
स्थिर ध्रुव वर्ण *get_rev_name(ध्रुव संरचना object *o, संरचना strbuf *buf)
{
	संरचना rev_name *n;
	ध्रुव संरचना commit *c;

	यदि (o->type != OBJ_COMMIT)
		प्रत्यागम get_exact_ref_मेलनम्(o);
	c = (ध्रुव संरचना commit *) o;
	n = get_commit_rev_name(c);
	यदि (!n)
		प्रत्यागम रिक्त;

	यदि (!n->generation) {
		प्रत्यागम n->tip_name;
	} अन्यथा {
		strbuf_reset(buf);
		strbuf_addstr(buf, n->tip_name);
		strbuf_strip_suffix(buf, "^0");
		strbuf_addf(buf, "~%d", n->generation);
		प्रत्यागम buf->buf;
	}
}

स्थिर ध्रुव वर्ण *get_format_rev(ध्रुव संरचना commit *c,
				  संरचना pretty_format *format_ctx,
				  संरचना strbuf *buf)
{
	strbuf_reset(buf);

	यदि (format_ctx->want.notes) {
		संरचना strbuf notebuf = STRBUF_INIT;

		format_display_notes(&c->object.oid, &notebuf,
				     get_log_output_encoding(),
				     format_ctx->ctx.fmt == CMIT_FMT_USERFORMAT);
		format_ctx->ctx.notes_सन्देशः = strbuf_detach(&notebuf, रिक्त);
	}

	pretty_print_commit(&format_ctx->ctx, c, buf);
	FREE_AND_NULL(format_ctx->ctx.notes_सन्देशः);

	प्रत्यागम buf->buf;
}

स्थिर शून्य show_name(ध्रुव संरचना object *obj,
		      ध्रुव वर्ण *caller_name,
		      पूर्णाङ्क always, पूर्णाङ्क allow_undefined, पूर्णाङ्क name_only)
{
	ध्रुव वर्ण *name;
	ध्रुव संरचना object_id *oid = &obj->oid;
	संरचना strbuf buf = STRBUF_INIT;

	यदि (!name_only)
		प्रारूप_लिख("%s ", caller_name ? caller_name : oid_गन्तव्यम्_hex(oid));
	name = get_rev_name(obj, &buf);
	यदि (name)
		प्रारूप_लिख("%s\n", name);
	अन्यथा यदि (allow_undefined)
		प्रारूप_लिख("undefined\n");
	अन्यथा यदि (always)
		प्रारूप_लिख("%s\n",
		       repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));
	अन्यथा
		die("cannot describe '%s'", oid_गन्तव्यम्_hex(oid));
	strbuf_release(&buf);
}

स्थिर वर्ण ध्रुव * ध्रुव name_rev_usage[] = {
	N_("git name-rev [<options>] <commit>..."),
	N_("git name-rev [<options>] --all"),
	N_("git name-rev [<options>] --annotate-stdin"),
	रिक्त
};

स्थिर शून्य name_rev_पङ्क्तिः(वर्ण *p, संरचना command *cmd)
{
	संरचना strbuf buf = STRBUF_INIT;
	पूर्णाङ्क गणक = 0;
	वर्ण *p_start;
	ध्रुव अचिह्नित hexsz = the_hash_algo->hexsz;

	प्रति (p_start = p; *p; p++) {
#परिभाषय ishex(x) (अङ्कः_वा((x)) || ((x) >= 'a' && (x) <= 'f'))
		यदि (!ishex(*p)) {
			गणक = 0;
		} अन्यथा यदि (++गणक == hexsz &&
			   !ishex(*(p + 1))) {
			संरचना object_id oid;
			ध्रुव वर्ण *name = रिक्त;
			वर्ण c = *(p + 1);
			पूर्णाङ्क p_len = p - p_start + 1;
			संरचना object *o = रिक्त;
			पूर्णाङ्क oid_ret = 1;

			गणक = 0;

			*(p + 1) = 0;
			oid_ret = repo_get_oid(the_repository, p - (hexsz - 1), &oid);
			*(p + 1) = c;

			विकल्प (cmd->type) {
			पक्ष NAME_REV:
				यदि (!oid_ret)
					o = lookup_object(the_repository, &oid);
				यदि (o)
					name = get_rev_name(o, &buf);
				यदि (!name)
					अनुवर्त;
				यदि (cmd->u.name_only)
					प्रारूप_लिख("%.*s%s", p_len - hexsz, p_start, name);
				अन्यथा
					प्रारूप_लिख("%.*s (%s)", p_len, p_start, name);
				भङ्ग;
			पक्ष FORMAT_REV:
				यदि (!oid_ret)
					o = parse_object(the_repository, &oid);
				यदि (o && o->type == OBJ_COMMIT)
					name = get_format_rev((ध्रुव संरचना commit *)o,
							      cmd->u.pretty_format,
							      &buf);
				यदि (name)
					प्रारूप_लिख("%.*s%s", p_len - hexsz, p_start, name);
				अन्यथा
					प्रारूप_लिख("%.*s", p_len, p_start);
				भङ्ग;
			स्वभाव:
				BUG("uncovered case: %d", cmd->type);
			}

			p_start = p + 1;
		}
	}

	/* flush */
	यदि (p_start != p)
		fwrite(p_start, p - p_start, 1, stdout);

	strbuf_release(&buf);
}

पूर्णाङ्क cmd_name_rev(पूर्णाङ्क argc,
		 ध्रुव वर्ण **argv,
		 ध्रुव वर्ण *उपसर्गः,
		 संरचना repository *repo अप्रयुक्तम्)
{
	संरचना mem_pool string_pool;
	संरचना object_array revs = OBJECT_ARRAY_INIT;

#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
	पूर्णाङ्क transform_stdin = 0;
#यदि_अन्तः
	पूर्णाङ्क all = 0, annotate_stdin = 0, allow_undefined = 1, always = 0, peel_चिह्नकम् = 0;
	संरचना name_ref_दत्त दत्त = { 0, 0, STRING_LIST_INIT_NODUP, STRING_LIST_INIT_NODUP };
	संरचना command cmd;
	संरचना option opts[] = {
		OPT_BOOL(0, "name-only", &दत्त.name_only, N_("print only ref-based names (no object names)")),
		OPT_BOOL(0, "tags", &दत्त.चिह्नकानि_only, N_("only use tags to name the commits")),
		OPT_STRING_LIST(0, "refs", &दत्त.ref_filters, N_("pattern"),
				N_("only use refs matching <pattern>")),
		OPT_STRING_LIST(0, "exclude", &दत्त.exclude_filters, N_("pattern"),
				N_("ignore refs matching <pattern>")),
		OPT_गणः(""),
		OPT_BOOL(0, "all", &all, N_("list all commits reachable from all refs")),
#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
		OPT_BOOL_F(0,
			   "stdin",
			   &transform_stdin,
			   N_("deprecated: use --annotate-stdin instead"),
			   PARSE_OPT_HIDDEN),
#यदि_अन्तः /* WITH_BREAKING_CHANGES */
		OPT_BOOL(0, "annotate-stdin", &annotate_stdin, N_("annotate text from stdin")),
		OPT_BOOL(0, "undefined", &allow_undefined, N_("allow to print `undefined` names (default)")),
		OPT_BOOL(0, "always", &always,
			 N_("show abbreviated commit object as fallback")),
		OPT_HIDDEN_BOOL(0, "peel-tag", &peel_चिह्नकम्,
				N_("dereference tags in the input (internal use)")),
		OPT_अन्तः(),
	};

	mem_pool_init(&string_pool, 0);
	init_commit_rev_name(&rev_नामानि);
	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, उपसर्गः, opts, name_rev_usage, 0);
	init_name_rev_command(&cmd, दत्त.name_only);

#यदि_अपरिभाषितम् WITH_BREAKING_CHANGES
	यदि (transform_stdin) {
		warning("--stdin is deprecated. Please use --annotate-stdin instead, "
					"which is functionally equivalent.\n"
					"This option will be removed in a future release.");
		annotate_stdin = 1;
	}
#यदि_अन्तः

	यदि (all + annotate_stdin + !!argc > 1) {
		error("Specify either a list, or --all, not both!");
		usage_with_options(name_rev_usage, opts);
	}
	यदि (all || annotate_stdin)
		disable_cutoff();

	प्रति (; argc; argc--, argv++) {
		संरचना object_id oid;
		संरचना object *object;
		संरचना commit *commit;

		यदि (repo_get_oid(the_repository, *argv, &oid)) {
			fprintf(stderr, "Could not get sha1 for %s. Skipping.\n",
					*argv);
			अनुवर्त;
		}

		commit = रिक्त;
		object = parse_object(the_repository, &oid);
		यदि (object) {
			संरचना object *peeled = deref_चिह्नकम्(the_repository,
							  object, *argv, 0);
			यदि (peeled && peeled->type == OBJ_COMMIT)
				commit = (संरचना commit *)peeled;
		}

		यदि (!object) {
			fprintf(stderr, "Could not get object for %s. Skipping.\n",
					*argv);
			अनुवर्त;
		}

		यदि (commit)
			समुच्चयः_commit_cutoff(commit);

		यदि (peel_चिह्नकम्) {
			यदि (!commit) {
				fprintf(stderr, "Could not get commit for %s. Skipping.\n",
					*argv);
				अनुवर्त;
			}
			object = (संरचना object *)commit;
		}
		सङ्कलय_object_array(object, *argv, &revs);
	}

	adjust_cutoff_timestamp_for_slop();

	refs_for_each_ref(get_main_ref_store(the_repository), name_ref, &दत्त);
	name_tips(&string_pool);

	यदि (annotate_stdin) {
		संरचना strbuf sb = STRBUF_INIT;

		यावत् (strbuf_getline(&sb, stdin) != EOF) {
			strbuf_addch(&sb, '\n');
			name_rev_पङ्क्तिः(sb.buf, &cmd);
		}
		strbuf_release(&sb);
	} अन्यथा यदि (all) {
		पूर्णाङ्क i, max;

		max = get_max_object_सूचकांक(the_repository);
		प्रति (i = 0; i < max; i++) {
			संरचना object *obj = get_अनुक्रमितम्_object(the_repository, i);
			यदि (!obj || obj->type != OBJ_COMMIT)
				अनुवर्त;
			show_name(obj, रिक्त,
				  always, allow_undefined, दत्त.name_only);
		}
	} अन्यथा {
		पूर्णाङ्क i;
		प्रति (i = 0; i < revs.nr; i++)
			show_name(revs.objects[i].item, revs.objects[i].name,
				  always, allow_undefined, दत्त.name_only);
	}

	string_list_clear(&दत्त.ref_filters, 0);
	string_list_clear(&दत्त.exclude_filters, 0);
	mem_pool_discard(&string_pool, 0);
	object_array_clear(&revs);
	प्रत्यागम 0;
}

संरचना format_nul_दत्त {
	bool nul_निवेशः;
	bool nul_output;
};

स्थिर पूर्णाङ्क format_nul_cb(ध्रुव संरचना option *option,
			 ध्रुव वर्ण *arg,
			 पूर्णाङ्क unset)
{
	संरचना format_nul_दत्त *दत्त = option->मानम्;
	दत्त->nul_निवेशः = 1;
	दत्त->nul_output = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	प्रत्यागम 0;
}

स्थिर गणना stdin_बहुलकम् parse_stdin_बहुलकम्(ध्रुव वर्ण *stdin_बहुलकम्)
{
	यदि (!वाक्यं_तुलय(stdin_बहुलकम्, "text"))
		प्रत्यागम TEXT;
	अन्यथा यदि (!वाक्यं_तुलय(stdin_बहुलकम्, "revs") ||
		 !वाक्यं_तुलय(stdin_बहुलकम्, "rev"))
		प्रत्यागम REVS;
	अन्यथा
		die(_("'%s' needs to be either text, revs, or rev"),
		    "--stdin-mode");
}

स्थिर वर्ण ध्रुव *ध्रुव format_rev_usage[] = {
	N_("(EXPERIMENTAL!) git format-rev --stdin-mode=<mode> "
	   "--format=<pretty> [--[no-]notes=<ref>] "
	   "[-z] [--[no-]null-output] [--[no-]null-input]"),
	रिक्त
};

पूर्णाङ्क cmd_format_rev(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *उपसर्गः,
		   संरचना repository *repo अप्रयुक्तम्)
{
	ध्रुव वर्ण *format = रिक्त;
	गणना stdin_बहुलकम् stdin_बहुलकम्;
	ध्रुव वर्ण *stdin_बहुलकम्_arg = रिक्त;
	संरचना format_nul_दत्त nul_दत्त = { 0, 0 };
	वर्ण output_terminator;
	strbuf_getline_fn getline_fn;
	संरचना display_notes_opt format_notes_opt;
	संरचना rev_info format_rev = REV_INFO_INIT;
	संरचना pretty_format format_pp = { 0 };
	संरचना string_list notes = STRING_LIST_INIT_NODUP;
	संरचना strbuf scratch_buf = STRBUF_INIT;
	संरचना command cmd;
	संरचना option opts[] = {
		OPT_STRING(0, "format", &format, N_("format"),
			   N_("pretty format to use")),
		OPT_STRING(0, "stdin-mode", &stdin_बहुलकम्_arg, N_("stdin-mode"),
			   N_("how revs are processed")),
		OPT_STRING_LIST(0, "notes", &notes, N_("notes"),
				N_("display notes for pretty format")),
		OPT_CALLBACK_F('z', "null", &nul_दत्त, N_("z"),
			       N_("Use NUL for input and output termination"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG, format_nul_cb),
		OPT_BOOL(0, "null-input", &nul_दत्त.nul_निवेशः,
			 N_("Use NUL for input termination")),
		OPT_BOOL(0, "null-output", &nul_दत्त.nul_output,
			 N_("Use NUL for output termination")),
		OPT_अन्तः(),
	};

	argc = parse_options(argc, argv, उपसर्गः, opts, format_rev_usage, 0);

	यदि (argc > 0) {
		error(_("too many arguments"));
		usage_with_options(format_rev_usage, opts);
	}

	यदि (!format)
		die(_("'%s' is required"), "--format");
	यदि (!stdin_बहुलकम्_arg)
		die(_("'%s' is required"), "--stdin-mode");

	getline_fn = nul_दत्त.nul_निवेशः ? strbuf_getline_nul : strbuf_getline_lf;
	output_terminator = nul_दत्त.nul_output ? '\0' : '\n';

	init_display_notes(&format_notes_opt);
	stdin_बहुलकम् = parse_stdin_बहुलकम्(stdin_बहुलकम्_arg);

	get_commit_format(format, &format_rev);
	format_pp.ctx.rev = &format_rev;
	format_pp.ctx.fmt = format_rev.commit_format;
	format_pp.ctx.abbrev = format_rev.abbrev;
	format_pp.ctx.date_बहुलकम्_explicit = format_rev.date_बहुलकम्_explicit;
	format_pp.ctx.date_बहुलकम् = format_rev.date_बहुलकम्;
	format_pp.ctx.color = GIT_COLOR_AUTO;

	userformat_find_requirements(format,
				     &format_pp.want);
	यदि (format_pp.want.notes) {
		पूर्णाङ्क ignore_show_notes = 0;
		संरचना string_list_item *n;

		for_each_string_list_item(n, &notes)
			enable_ref_display_notes(&format_notes_opt,
						 &ignore_show_notes,
						 n->string);
		आरोपय_display_notes(&format_notes_opt);
	}

	init_format_rev_command(&cmd, &format_pp);

	विकल्प (stdin_बहुलकम्) {
	पक्ष TEXT:
		यावत् (getline_fn(&scratch_buf, stdin) != EOF) {
			name_rev_पङ्क्तिः(scratch_buf.buf, &cmd);
			/*
			 * We do not pass on the terminator to name_rev_line,
			 * unlike name-rev.
			 */
			प्रारूप_लिख("%c", output_terminator);
			maybe_flush_or_die(stdout, "stdout");
		}
		भङ्ग;
	पक्ष REVS:
		यावत् (getline_fn(&scratch_buf, stdin) != EOF) {
			संरचना object_id oid;
			संरचना object *object;
			संरचना object *peeled;

			यदि (repo_get_oid(the_repository, scratch_buf.buf, &oid)) {
				fprintf(stderr, "Could not get object name for %s. Skipping.\n",
					scratch_buf.buf);
				अनुवर्त;
			}

			object = parse_object(the_repository, &oid);
			यदि (!object) {
				fprintf(stderr, "Could not get object for %s. Skipping.\n",
					scratch_buf.buf);
				अनुवर्त;
			}

			peeled = deref_चिह्नकम्(the_repository, object, scratch_buf.buf, 0);
			यदि (!peeled || peeled->type != OBJ_COMMIT) {
				fprintf(stderr,
					"Could not get commit for %s. Skipping.\n",
					scratch_buf.buf);
				अनुवर्त;
			}

			get_format_rev((संरचना commit *)peeled,
				       &format_pp, &scratch_buf);
			प्रारूप_लिख("%s%c", scratch_buf.buf, output_terminator);
			maybe_flush_or_die(stdout, "stdout");
			strbuf_release(&scratch_buf);
		}
		भङ्ग;
	स्वभाव:
		BUG("uncovered case: %d", stdin_बहुलकम्);
	}

	strbuf_release(&scratch_buf);
	string_list_clear(&notes, 0);
	release_display_notes(&format_notes_opt);
	प्रत्यागम 0;
}
