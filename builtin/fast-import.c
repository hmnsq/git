#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "object.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "delta.h"
#अन्तर्भाव "pack.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "csum-file.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "mem-pool.h"
#अन्तर्भाव "commit-reach.h"
#अन्तर्भाव "khash.h"
#अन्तर्भाव "date.h"
#अन्तर्भाव "gpg-interface.h"
#अन्तर्भाव "parse-options.h"

#परिभाषय PACK_ID_द्वयङ्काः 16
#परिभाषय MAX_PACK_ID ((1<<PACK_ID_द्वयङ्काः)-1)
#परिभाषय गभीरता_द्वयङ्काः 13
#परिभाषय MAX_गभीरता ((1<<गभीरता_द्वयङ्काः)-1)

/*
 * We abuse the setuid bit on directories to mean "do not delta".
 */
#परिभाषय NO_DELTA S_ISUID

/*
 * The amount of additional space required in order to write an object into the
 * current pack. This is the hash lengths at the end of the pack, plus the
 * length of one object ID.
 */
#परिभाषय PACK_आकार_देहली (the_hash_algo->rawsz * 3)

संरचना object_प्रविष्टिः {
	संरचना pack_idx_प्रविष्टिः idx;
	संरचना hashmap_प्रविष्टिः ent;
	uint32_t type : TYPE_द्वयङ्काः,
		pack_id : PACK_ID_द्वयङ्काः,
		गभीरता : गभीरता_द्वयङ्काः;
};

स्थिर पूर्णाङ्क object_प्रविष्टिः_hashcmp(ध्रुव शून्य *map_दत्त अप्रयुक्तम्,
				ध्रुव संरचना hashmap_प्रविष्टिः *eptr,
				ध्रुव संरचना hashmap_प्रविष्टिः *प्रविष्टिः_or_कुञ्चिका,
				ध्रुव शून्य *keydata)
{
	ध्रुव संरचना object_id *oid = keydata;
	ध्रुव संरचना object_प्रविष्टिः *e1, *e2;

	e1 = container_सम्बन्धि(eptr, ध्रुव संरचना object_प्रविष्टिः, ent);
	यदि (oid)
		प्रत्यागम oidcmp(&e1->idx.oid, oid);

	e2 = container_सम्बन्धि(प्रविष्टिः_or_कुञ्चिका, ध्रुव संरचना object_प्रविष्टिः, ent);
	प्रत्यागम oidcmp(&e1->idx.oid, &e2->idx.oid);
}

संरचना object_प्रविष्टिः_pool {
	संरचना object_प्रविष्टिः_pool *अग्र_pool;
	संरचना object_प्रविष्टिः *अग्र_free;
	संरचना object_प्रविष्टिः *अन्तः;
	संरचना object_प्रविष्टिः entries[FLEX_ARRAY]; /*अधिकम्*/
};

संरचना mark_समुच्चयः {
	सङ्घ {
		संरचना object_id *oids[1024];
		संरचना object_प्रविष्टिः *marked[1024];
		संरचना mark_समुच्चयः *sets[1024];
	} दत्त;
	अचिह्नित पूर्णाङ्क सरणम्;
};

संरचना चरमः_object {
	संरचना strbuf दत्त;
	off_t विस्थापनम्;
	अचिह्नित पूर्णाङ्क गभीरता;
	अचिह्नित no_swap : 1;
};

संरचना atom_str {
	संरचना atom_str *अग्र_atom;
	अचिह्नित ह्रस्व str_len;
	वर्ण str_dat[FLEX_ARRAY]; /*अधिकम्*/
};

संरचना वृक्षः_content;
संरचना वृक्षः_प्रविष्टिः {
	संरचना वृक्षः_content *वृक्षः;
	संरचना atom_str *name;
	संरचना वृक्षः_प्रविष्टिः_ms {
		uint16_t बहुलकम्;
		संरचना object_id oid;
	} versions[2];
};

संरचना वृक्षः_content {
	अचिह्नित पूर्णाङ्क प्रविष्टिः_क्षमता; /* must match avail_tree_content */
	अचिह्नित पूर्णाङ्क प्रविष्टिः_गणनम्;
	अचिह्नित पूर्णाङ्क delta_गभीरता;
	संरचना वृक्षः_प्रविष्टिः *entries[FLEX_ARRAY]; /*अधिकम्*/
};

संरचना avail_वृक्षः_content {
	अचिह्नित पूर्णाङ्क प्रविष्टिः_क्षमता; /* must match tree_content */
	संरचना avail_वृक्षः_content *अग्र_avail;
};

संरचना branch {
	संरचना branch *सारणी_अग्र_branch;
	संरचना branch *सक्रिय_अग्र_branch;
	ध्रुव वर्ण *name;
	संरचना वृक्षः_प्रविष्टिः branch_वृक्षः;
	uintmax_t चरमः_commit;
	uintmax_t अङ्कः_notes;
	अचिह्नित सक्रिय : 1;
	अचिह्नित delete : 1;
	अचिह्नित pack_id : PACK_ID_द्वयङ्काः;
	संरचना object_id oid;
};

संरचना चिह्नकम् {
	संरचना चिह्नकम् *अग्र_चिह्नकम्;
	ध्रुव वर्ण *name;
	अचिह्नित पूर्णाङ्क pack_id;
	संरचना object_id oid;
};

संरचना hash_list {
	संरचना hash_list *अग्र;
	संरचना object_id oid;
};

प्रकारनिर्देश गणना {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

संरचना recent_command {
	संरचना recent_command *पूर्व;
	संरचना recent_command *अग्र;
	वर्ण *buf;
};

प्रकारनिर्देश शून्य (*mark_समुच्चयः_inserter_t)(संरचना mark_समुच्चयः **s, संरचना object_id *oid, uintmax_t mark);
प्रकारनिर्देश शून्य (*each_mark_fn_t)(uintmax_t mark, शून्य *obj, शून्य *cbp);

/* Configured limits on output */
स्थिर अचिह्नित दीर्घ max_गभीरता = 50;
स्थिर off_t max_packsize;
स्थिर पूर्णाङ्क unpack_सीमा = 100;
स्थिर पूर्णाङ्क force_update;

/* Stats and misc. counters */
स्थिर uintmax_t alloc_गणनम्;
स्थिर uintmax_t marks_समुच्चयः_गणनम्;
स्थिर uintmax_t object_गणनम्_by_type[1 << TYPE_द्वयङ्काः];
स्थिर uintmax_t duplicate_गणनम्_by_type[1 << TYPE_द्वयङ्काः];
स्थिर uintmax_t delta_गणनम्_by_type[1 << TYPE_द्वयङ्काः];
स्थिर uintmax_t delta_गणनम्_attempts_by_type[1 << TYPE_द्वयङ्काः];
स्थिर अचिह्नित दीर्घ object_गणनम्;
स्थिर अचिह्नित दीर्घ branch_गणनम्;
स्थिर अचिह्नित दीर्घ branch_आरोपय_गणनम्;
स्थिर पूर्णाङ्क failure;
स्थिर FILE *pack_धाराः;
स्थिर पूर्णाङ्क show_stats = 1;
स्थिर पूर्णाङ्क quiet;
स्थिर गणना sign_बहुलकम् signed_चिह्नकम्_बहुलकम् = SIGN_VERBATIM;
स्थिर गणना sign_बहुलकम् signed_commit_बहुलकम् = SIGN_VERBATIM;
स्थिर ध्रुव वर्ण *signed_commit_keyid;
स्थिर ध्रुव वर्ण *signed_चिह्नकम्_keyid;

/* Memory pools */
स्थिर संरचना mem_pool fi_mem_pool = {
	.खण्डम्_alloc = 2*1024*1024 - परिमाण(संरचना mp_खण्डम्),
};

/* Atom management */
स्थिर अचिह्नित पूर्णाङ्क atom_सारणी_sz = 4451;
स्थिर अचिह्नित पूर्णाङ्क atom_cnt;
स्थिर संरचना atom_str **atom_सारणी;

/* The .pack file being generated */
स्थिर संरचना pack_idx_option pack_idx_opts;
स्थिर अचिह्नित पूर्णाङ्क pack_id;
स्थिर संरचना hashfile *pack_file;
स्थिर संरचना packed_git *pack_दत्त;
स्थिर संरचना packed_git **all_packs;
स्थिर off_t pack_आकार;

/* Table of objects we've written. */
स्थिर अचिह्नित पूर्णाङ्क object_प्रविष्टिः_alloc = 5000;
स्थिर संरचना object_प्रविष्टिः_pool *खण्डानि;
स्थिर संरचना hashmap object_सारणी;
स्थिर संरचना mark_समुच्चयः *marks;
स्थिर वर्ण *export_marks_file;
स्थिर वर्ण *import_marks_file;
स्थिर पूर्णाङ्क import_marks_file_from_प्रवाहः;
स्थिर पूर्णाङ्क import_marks_file_ignore_missing;
स्थिर पूर्णाङ्क import_marks_file_समाप्तम्;
स्थिर पूर्णाङ्क relative_marks_paths;

/* Our last blob */
स्थिर संरचना चरमः_object चरमः_blob = {
	.दत्त = STRBUF_INIT,
 };

/* Tree management */
स्थिर अचिह्नित पूर्णाङ्क वृक्षः_प्रविष्टिः_alloc = 1000;
स्थिर शून्य *avail_वृक्षः_प्रविष्टिः;
स्थिर अचिह्नित पूर्णाङ्क avail_वृक्षः_सारणी_sz = 100;
स्थिर संरचना avail_वृक्षः_content **avail_वृक्षः_सारणी;
स्थिर आकार_t वृक्षः_प्रविष्टिः_allocd;
स्थिर संरचना strbuf old_वृक्षः = STRBUF_INIT;
स्थिर संरचना strbuf new_वृक्षः = STRBUF_INIT;

/* Branch data */
स्थिर अचिह्नित दीर्घ max_सक्रिय_branches = 5;
स्थिर अचिह्नित दीर्घ cur_सक्रिय_branches;
स्थिर अचिह्नित दीर्घ branch_सारणी_sz = 1039;
स्थिर संरचना branch **branch_सारणी;
स्थिर संरचना branch *सक्रिय_branches;

/* Tag data */
स्थिर संरचना चिह्नकम् *first_चिह्नकम्;
स्थिर संरचना चिह्नकम् *चरमः_चिह्नकम्;

/* Input stream parsing */
स्थिर whenspec_type whenspec = WHENSPEC_RAW;
स्थिर संरचना strbuf command_buf = STRBUF_INIT;
स्थिर पूर्णाङ्क unread_command_buf;
स्थिर संरचना recent_command cmd_hist = {
	.पूर्व = &cmd_hist,
	.अग्र = &cmd_hist,
};
स्थिर संरचना recent_command *cmd_पुच्छ = &cmd_hist;
स्थिर संरचना recent_command *rc_free;
स्थिर अचिह्नित पूर्णाङ्क cmd_save = 100;
स्थिर uintmax_t अग्र_mark;
स्थिर संरचना strbuf new_दत्त = STRBUF_INIT;
स्थिर पूर्णाङ्क require_explicit_termination;

/* Signal handling */
स्थिर चञ्चल sig_atomic_t checkpoint_requested;

/* Submodule marks */
स्थिर संरचना string_list sub_marks_from = STRING_LIST_INIT_DUP;
स्थिर संरचना string_list sub_marks_गन्तव्यम् = STRING_LIST_INIT_DUP;
स्थिर kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
स्थिर पूर्णाङ्क cat_blob_fd = STDOUT_FILENO;

/* Command state */
संरचना fast_import_state {
	पूर्णाङ्क argc;
	ध्रुव वर्ण **argv;
	ध्रुव वर्ण *उपसर्गः;
	पूर्णाङ्क आलोकितम्_दत्त_command;
	पूर्णाङ्क allow_unsafe_features;
	संरचना option *option;
};

स्थिर शून्य fast_import_state_init(संरचना fast_import_state *state,
				   पूर्णाङ्क argc, ध्रुव वर्ण **argv,
				   ध्रुव वर्ण *उपसर्गः, संरचना option *option)
{
	स्मृतिं_पूरय(state, 0, परिमाण(*state));
	state->argc = argc;
	state->argv = argv;
	state->उपसर्गः = उपसर्गः;
	state->option = option;
}

स्थिर शून्य parse_argv(संरचना fast_import_state *state);
स्थिर शून्य parse_get_mark(संरचना fast_import_state *state, ध्रुव वर्ण *p);
स्थिर शून्य parse_cat_blob(संरचना fast_import_state *state, ध्रुव वर्ण *p);
स्थिर शून्य parse_ls(संरचना fast_import_state *state,
		     ध्रुव वर्ण *p,
		     संरचना branch *b);

स्थिर शून्य for_each_mark(संरचना mark_समुच्चयः *m, uintmax_t आधारः, each_mark_fn_t callback, शून्य *p)
{
	uintmax_t k;
	यदि (m->सरणम्) {
		प्रति (k = 0; k < 1024; k++) {
			यदि (m->दत्त.sets[k])
				for_each_mark(m->दत्त.sets[k], आधारः + (k << m->सरणम्), callback, p);
		}
	} अन्यथा {
		प्रति (k = 0; k < 1024; k++) {
			यदि (m->दत्त.marked[k])
				callback(आधारः + k, m->दत्त.marked[k], p);
		}
	}
}

स्थिर शून्य dump_marks_fn(uintmax_t mark, शून्य *object, शून्य *cbp) {
	संरचना object_प्रविष्टिः *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_गन्तव्यम्_hex(&e->idx.oid));
}

स्थिर शून्य write_branch_report(FILE *rpt, संरचना branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	यदि (b->सक्रिय)
		fputs(" active", rpt);
	यदि (b->branch_वृक्षः.वृक्षः)
		fputs(" loaded", rpt);
	यदि (is_null_oid(&b->branch_वृक्षः.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_गन्तव्यम्_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_गन्तव्यम्_hex(&b->branch_वृक्षः.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_गन्तव्यम्_hex(&b->branch_वृक्षः.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->चरमः_commit);

	fputs("  last pack   : ", rpt);
	यदि (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

स्थिर शून्य write_crash_report(ध्रुव वर्ण *err)
{
	वर्ण *loc = repo_git_मार्गः(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	संरचना branch *b;
	अचिह्नित दीर्घ lu;
	संरचना recent_command *rc;

	यदि (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		मुक्त(loc);
		प्रत्यागम;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(time(रिक्त), 0, DATE_बहुलकम्(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	प्रति (rc = cmd_hist.अग्र; rc != &cmd_hist; rc = rc->अग्र) {
		यदि (rc->अग्र == &cmd_hist)
			fputs("* ", rpt);
		अन्यथा
			fputs("  ", rpt);
		fputs(rc->buf, rpt);
		fputc('\n', rpt);
	}

	fputc('\n', rpt);
	fputs("Active Branch LRU\n", rpt);
	fputs("-----------------\n", rpt);
	fprintf(rpt, "    active_branches = %lu cur, %lu max\n",
		cur_सक्रिय_branches,
		max_सक्रिय_branches);
	fputc('\n', rpt);
	fputs("  pos  clock name\n", rpt);
	fputs("  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", rpt);
	प्रति (b = सक्रिय_branches, lu = 0; b; b = b->सक्रिय_अग्र_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->चरमः_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	प्रति (lu = 0; lu < branch_सारणी_sz; lu++) {
		प्रति (b = branch_सारणी[lu]; b; b = b->सारणी_अग्र_branch)
			write_branch_report(rpt, b);
	}

	यदि (first_चिह्नकम्) {
		संरचना चिह्नकम् *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		प्रति (tg = first_चिह्नकम्; tg; tg = tg->अग्र_चिह्नकम्) {
			fputs(oid_गन्तव्यम्_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	यदि (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	अन्यथा
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	मुक्त(loc);
}

स्थिर शून्य अन्तः_packfile(शून्य);
स्थिर शून्य unkeep_all_packs(शून्य);
स्थिर शून्य dump_marks(शून्य);

स्थिर NORETURN शून्य die_nicely(ध्रुव वर्ण *err, va_list params)
{
	va_list cp;
	स्थिर पूर्णाङ्क zombie;
	report_fn die_सन्देशः_fn = get_die_सन्देशः_routine();

	va_copy(cp, params);
	die_सन्देशः_fn(err, params);

	यदि (!zombie) {
		वर्ण सन्देशः_2[2 * मार्गः_MAX];

		zombie = 1;
		vsnprintf(सन्देशः_2, परिमाण(सन्देशः_2), err, cp);
		write_crash_report(सन्देशः_2);
		अन्तः_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	निर्गच्छ(128);
}

#यदि_अपरिभाषितम् SIGUSR1	/* Windows, for example */

स्थिर शून्य समुच्चयः_checkpoint_signal(शून्य)
{
}

#अन्यथा

स्थिर शून्य checkpoint_signal(पूर्णाङ्क signo अप्रयुक्तम्)
{
	checkpoint_requested = 1;
}

स्थिर शून्य समुच्चयः_checkpoint_signal(शून्य)
{
	संरचना sigaction sa;

	स्मृतिं_पूरय(&sa, 0, परिमाण(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_आवरणम्);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, रिक्त);
}

#यदि_अन्तः

स्थिर शून्य alloc_objects(अचिह्नित पूर्णाङ्क cnt)
{
	संरचना object_प्रविष्टिः_pool *b;

	b = xmalloc(परिमाण(संरचना object_प्रविष्टिः_pool)
		+ cnt * परिमाण(संरचना object_प्रविष्टिः));
	b->अग्र_pool = खण्डानि;
	b->अग्र_free = b->entries;
	b->अन्तः = b->entries + cnt;
	खण्डानि = b;
	alloc_गणनम् += cnt;
}

स्थिर संरचना object_प्रविष्टिः *new_object(संरचना object_id *oid)
{
	संरचना object_प्रविष्टिः *e;

	यदि (खण्डानि->अग्र_free == खण्डानि->अन्तः)
		alloc_objects(object_प्रविष्टिः_alloc);

	e = खण्डानि->अग्र_free++;
	oidcpy(&e->idx.oid, oid);
	प्रत्यागम e;
}

स्थिर संरचना object_प्रविष्टिः *find_object(संरचना object_id *oid)
{
	प्रत्यागम hashmap_get_प्रविष्टिः_from_hash(&object_सारणी, oidhash(oid), oid,
					   संरचना object_प्रविष्टिः, ent);
}

स्थिर संरचना object_प्रविष्टिः *निवेशय_object(संरचना object_id *oid)
{
	संरचना object_प्रविष्टिः *e;
	अचिह्नित पूर्णाङ्क hash = oidhash(oid);

	e = hashmap_get_प्रविष्टिः_from_hash(&object_सारणी, hash, oid,
					संरचना object_प्रविष्टिः, ent);
	यदि (!e) {
		e = new_object(oid);
		e->idx.विस्थापनम् = 0;
		hashmap_प्रविष्टिः_init(&e->ent, hash);
		hashmap_सङ्कलय(&object_सारणी, &e->ent);
	}

	प्रत्यागम e;
}

स्थिर शून्य invalidate_pack_id(अचिह्नित पूर्णाङ्क id)
{
	अचिह्नित दीर्घ lu;
	संरचना चिह्नकम् *t;
	संरचना hashmap_iter iter;
	संरचना object_प्रविष्टिः *e;

	hashmap_for_each_प्रविष्टिः(&object_सारणी, &iter, e, ent) {
		यदि (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	प्रति (lu = 0; lu < branch_सारणी_sz; lu++) {
		संरचना branch *b;

		प्रति (b = branch_सारणी[lu]; b; b = b->सारणी_अग्र_branch)
			यदि (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	प्रति (t = first_चिह्नकम्; t; t = t->अग्र_चिह्नकम्)
		यदि (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

स्थिर अचिह्नित पूर्णाङ्क hc_str(ध्रुव वर्ण *s, आकार_t len)
{
	अचिह्नित पूर्णाङ्क r = 0;
	यावत् (len-- > 0)
		r = r * 31 + *s++;
	प्रत्यागम r;
}

स्थिर शून्य निवेशय_mark(संरचना mark_समुच्चयः **शिखरम्, uintmax_t idnum, संरचना object_प्रविष्टिः *oe)
{
	संरचना mark_समुच्चयः *s = *शिखरम्;

	यावत् ((idnum >> s->सरणम्) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, परिमाण(संरचना mark_समुच्चयः));
		s->सरणम् = (*शिखरम्)->सरणम् + 10;
		s->दत्त.sets[0] = *शिखरम्;
		*शिखरम् = s;
	}
	यावत् (s->सरणम्) {
		uintmax_t i = idnum >> s->सरणम्;
		idnum -= i << s->सरणम्;
		यदि (!s->दत्त.sets[i]) {
			s->दत्त.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, परिमाण(संरचना mark_समुच्चयः));
			s->दत्त.sets[i]->सरणम् = s->सरणम् - 10;
		}
		s = s->दत्त.sets[i];
	}
	यदि (!s->दत्त.marked[idnum])
		marks_समुच्चयः_गणनम्++;
	s->दत्त.marked[idnum] = oe;
}

स्थिर शून्य *find_mark(संरचना mark_समुच्चयः *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	संरचना object_प्रविष्टिः *oe = रिक्त;
	यदि ((idnum >> s->सरणम्) < 1024) {
		यावत् (s && s->सरणम्) {
			uintmax_t i = idnum >> s->सरणम्;
			idnum -= i << s->सरणम्;
			s = s->दत्त.sets[i];
		}
		यदि (s)
			oe = s->दत्त.marked[idnum];
	}
	यदि (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	प्रत्यागम oe;
}

स्थिर संरचना atom_str *गन्तव्यम्_atom(ध्रुव वर्ण *s, अचिह्नित ह्रस्व len)
{
	अचिह्नित पूर्णाङ्क hc = hc_str(s, len) % atom_सारणी_sz;
	संरचना atom_str *c;

	प्रति (c = atom_सारणी[hc]; c; c = c->अग्र_atom)
		यदि (c->str_len == len && !वाक्ये_तुलय(s, c->str_dat, len))
			प्रत्यागम c;

	c = mem_pool_alloc(&fi_mem_pool, परिमाण(संरचना atom_str) + len + 1);
	c->str_len = len;
	स्मृतिं_प्रतिलिख(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->अग्र_atom = atom_सारणी[hc];
	atom_सारणी[hc] = c;
	atom_cnt++;
	प्रत्यागम c;
}

स्थिर संरचना branch *lookup_branch(ध्रुव वर्ण *name)
{
	अचिह्नित पूर्णाङ्क hc = hc_str(name, वाक्यदैर्घ्यम्(name)) % branch_सारणी_sz;
	संरचना branch *b;

	प्रति (b = branch_सारणी[hc]; b; b = b->सारणी_अग्र_branch)
		यदि (!वाक्यं_तुलय(name, b->name))
			प्रत्यागम b;
	प्रत्यागम रिक्त;
}

स्थिर संरचना branch *new_branch(ध्रुव वर्ण *name)
{
	अचिह्नित पूर्णाङ्क hc = hc_str(name, वाक्यदैर्घ्यम्(name)) % branch_सारणी_sz;
	संरचना branch *b = lookup_branch(name);

	यदि (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	यदि (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, परिमाण(संरचना branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->सारणी_अग्र_branch = branch_सारणी[hc];
	b->branch_वृक्षः.versions[0].बहुलकम् = S_IFDIR;
	b->branch_वृक्षः.versions[1].बहुलकम् = S_IFDIR;
	b->अङ्कः_notes = 0;
	b->सक्रिय = 0;
	b->pack_id = MAX_PACK_ID;
	branch_सारणी[hc] = b;
	branch_गणनम्++;
	प्रत्यागम b;
}

स्थिर अचिह्नित पूर्णाङ्क hc_entries(अचिह्नित पूर्णाङ्क cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	प्रत्यागम cnt < avail_वृक्षः_सारणी_sz ? cnt : avail_वृक्षः_सारणी_sz - 1;
}

स्थिर संरचना वृक्षः_content *new_वृक्षः_content(अचिह्नित पूर्णाङ्क cnt)
{
	संरचना avail_वृक्षः_content *f, *l = रिक्त;
	संरचना वृक्षः_content *t;
	अचिह्नित पूर्णाङ्क hc = hc_entries(cnt);

	प्रति (f = avail_वृक्षः_सारणी[hc]; f; l = f, f = f->अग्र_avail)
		यदि (f->प्रविष्टिः_क्षमता >= cnt)
			भङ्ग;

	यदि (f) {
		यदि (l)
			l->अग्र_avail = f->अग्र_avail;
		अन्यथा
			avail_वृक्षः_सारणी[hc] = f->अग्र_avail;
	} अन्यथा {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, परिमाण(*t) + परिमाण(t->entries[0]) * cnt);
		f->प्रविष्टिः_क्षमता = cnt;
	}

	t = (संरचना वृक्षः_content*)f;
	t->प्रविष्टिः_गणनम् = 0;
	t->delta_गभीरता = 0;
	प्रत्यागम t;
}

स्थिर शून्य release_वृक्षः_प्रविष्टिः(संरचना वृक्षः_प्रविष्टिः *e);
स्थिर शून्य release_वृक्षः_content(संरचना वृक्षः_content *t)
{
	संरचना avail_वृक्षः_content *f = (संरचना avail_वृक्षः_content*)t;
	अचिह्नित पूर्णाङ्क hc = hc_entries(f->प्रविष्टिः_क्षमता);
	f->अग्र_avail = avail_वृक्षः_सारणी[hc];
	avail_वृक्षः_सारणी[hc] = f;
}

स्थिर शून्य release_वृक्षः_content_recursive(संरचना वृक्षः_content *t)
{
	अचिह्नित पूर्णाङ्क i;
	प्रति (i = 0; i < t->प्रविष्टिः_गणनम्; i++)
		release_वृक्षः_प्रविष्टिः(t->entries[i]);
	release_वृक्षः_content(t);
}

स्थिर संरचना वृक्षः_content *grow_वृक्षः_content(
	संरचना वृक्षः_content *t,
	पूर्णाङ्क amt)
{
	संरचना वृक्षः_content *r = new_वृक्षः_content(t->प्रविष्टिः_गणनम् + amt);
	r->प्रविष्टिः_गणनम् = t->प्रविष्टिः_गणनम्;
	r->delta_गभीरता = t->delta_गभीरता;
	COPY_ARRAY(r->entries, t->entries, t->प्रविष्टिः_गणनम्);
	release_वृक्षः_content(t);
	प्रत्यागम r;
}

स्थिर संरचना वृक्षः_प्रविष्टिः *new_वृक्षः_प्रविष्टिः(शून्य)
{
	संरचना वृक्षः_प्रविष्टिः *e;

	यदि (!avail_वृक्षः_प्रविष्टिः) {
		अचिह्नित पूर्णाङ्क n = वृक्षः_प्रविष्टिः_alloc;
		वृक्षः_प्रविष्टिः_allocd += n * परिमाण(संरचना वृक्षः_प्रविष्टिः);
		ALLOC_ARRAY(e, n);
		avail_वृक्षः_प्रविष्टिः = e;
		यावत् (n-- > 1) {
			*((शून्य**)e) = e + 1;
			e++;
		}
		*((शून्य**)e) = रिक्त;
	}

	e = avail_वृक्षः_प्रविष्टिः;
	avail_वृक्षः_प्रविष्टिः = *((शून्य**)e);
	प्रत्यागम e;
}

स्थिर शून्य release_वृक्षः_प्रविष्टिः(संरचना वृक्षः_प्रविष्टिः *e)
{
	यदि (e->वृक्षः)
		release_वृक्षः_content_recursive(e->वृक्षः);
	*((शून्य**)e) = avail_वृक्षः_प्रविष्टिः;
	avail_वृक्षः_प्रविष्टिः = e;
}

स्थिर संरचना वृक्षः_content *dup_वृक्षः_content(संरचना वृक्षः_content *s)
{
	संरचना वृक्षः_content *d;
	संरचना वृक्षः_प्रविष्टिः *a, *b;
	अचिह्नित पूर्णाङ्क i;

	यदि (!s)
		प्रत्यागम रिक्त;
	d = new_वृक्षः_content(s->प्रविष्टिः_गणनम्);
	प्रति (i = 0; i < s->प्रविष्टिः_गणनम्; i++) {
		a = s->entries[i];
		b = new_वृक्षः_प्रविष्टिः();
		स्मृतिं_प्रतिलिख(b, a, परिमाण(*a));
		यदि (a->वृक्षः && is_null_oid(&b->versions[1].oid))
			b->वृक्षः = dup_वृक्षः_content(a->वृक्षः);
		अन्यथा
			b->वृक्षः = रिक्त;
		d->entries[i] = b;
	}
	d->प्रविष्टिः_गणनम् = s->प्रविष्टिः_गणनम्;
	d->delta_गभीरता = s->delta_गभीरता;

	प्रत्यागम d;
}

स्थिर शून्य start_packfile(शून्य)
{
	संरचना strbuf tmp_file = STRBUF_INIT;
	संरचना packed_git *p;
	पूर्णाङ्क pack_fd;

	pack_fd = odb_mkstemp(the_repository->objects, &tmp_file,
			      "pack/tmp_pack_XXXXXX");
	FLEX_ALLOC_STR(p, pack_name, tmp_file.buf);
	strbuf_release(&tmp_file);

	p->pack_fd = pack_fd;
	p->do_not_close = 1;
	p->repo = the_repository;
	pack_file = hashfd(the_repository->hash_algo, pack_fd, p->pack_name);

	pack_दत्त = p;
	pack_आकार = write_pack_शीर्षकम्(pack_file, 0);
	object_गणनम् = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

स्थिर ध्रुव वर्ण *create_सूचकांक(शून्य)
{
	ध्रुव वर्ण *tmpfile;
	संरचना pack_idx_प्रविष्टिः **idx, **c, **चरमः;
	संरचना object_प्रविष्टिः *e;
	संरचना object_प्रविष्टिः_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_गणनम्);
	c = idx;
	प्रति (o = खण्डानि; o; o = o->अग्र_pool)
		प्रति (e = o->अग्र_free; e-- != o->entries;)
			यदि (pack_id == e->pack_id)
				*c++ = &e->idx;
	चरमः = idx + object_गणनम्;
	यदि (c != चरमः)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, रिक्त, idx, object_गणनम्,
				 &pack_idx_opts, pack_दत्त->hash);
	मुक्त(idx);
	प्रत्यागम tmpfile;
}

स्थिर वर्ण *keep_pack(ध्रुव वर्ण *curr_सूचकांक_name)
{
	स्थिर ध्रुव वर्ण *keep_सन्देशः = "fast-import";
	संरचना strbuf name = STRBUF_INIT;
	पूर्णाङ्क keep_fd;

	odb_pack_name(pack_दत्त->repo, &name, pack_दत्त->hash, "keep");
	keep_fd = safe_create_file_with_leading_directories(pack_दत्त->repo,
							    name.buf);
	यदि (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_सन्देशः, वाक्यदैर्घ्यम्(keep_सन्देशः));
	यदि (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_दत्त->repo, &name, pack_दत्त->hash, "pack");
	यदि (finalize_object_file(pack_दत्त->repo, pack_दत्त->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_दत्त->repo, &name, pack_दत्त->hash, "idx");
	यदि (finalize_object_file(pack_दत्त->repo, curr_सूचकांक_name, name.buf))
		die(_("cannot store index file"));
	मुक्त((शून्य *)curr_सूचकांक_name);
	प्रत्यागम strbuf_detach(&name, रिक्त);
}

स्थिर शून्य unkeep_all_packs(शून्य)
{
	संरचना strbuf name = STRBUF_INIT;
	पूर्णाङ्क k;

	प्रति (k = 0; k < pack_id; k++) {
		संरचना packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

स्थिर पूर्णाङ्क loosen_small_pack(ध्रुव संरचना packed_git *p)
{
	संरचना child_process unpack = CHILD_PROCESS_INIT;

	यदि (lseek(p->pack_fd, 0, SEEK_समुच्चयः) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.अन्तर् = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_गन्तव्यम्_stderr = 1;
	strvec_प्रक्षिप(&unpack.तर्काः, "unpack-objects");
	यदि (!show_stats)
		strvec_प्रक्षिप(&unpack.तर्काः, "-q");

	प्रत्यागम सततम्_command(&unpack);
}

स्थिर शून्य अन्तः_packfile(शून्य)
{
	स्थिर पूर्णाङ्क running;

	यदि (running || !pack_दत्त)
		प्रत्यागम;

	running = 1;
	clear_delta_आधारः_cache();
	यदि (object_गणनम्) {
		संरचना odb_source_files *files = odb_source_files_downcast(pack_दत्त->repo->objects->source);
		संरचना packed_git *new_p;
		संरचना object_id cur_pack_oid;
		वर्ण *idx_name;
		पूर्णाङ्क i;
		संरचना branch *b;
		संरचना चिह्नकम् *t;

		close_pack_windows(pack_दत्त);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_शीर्षकम्_footer(the_hash_algo, pack_दत्त->pack_fd,
					 pack_दत्त->hash, pack_दत्त->pack_name,
					 object_गणनम्, cur_pack_oid.hash,
					 pack_आकार);

		यदि (object_गणनम् <= unpack_सीमा) {
			यदि (!loosen_small_pack(pack_दत्त)) {
				invalidate_pack_id(pack_id);
				गच्छ discard_pack;
			}
		}

		close(pack_दत्त->pack_fd);
		idx_name = keep_pack(create_सूचकांक());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_आरोपय_pack(files->dirs->packed, idx_name, 1);
		यदि (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		मुक्त(idx_name);

		/* Print the boundary */
		यदि (pack_धाराः) {
			fprintf(pack_धाराः, "%s:", new_p->pack_name);
			प्रति (i = 0; i < branch_सारणी_sz; i++) {
				प्रति (b = branch_सारणी[i]; b; b = b->सारणी_अग्र_branch) {
					यदि (b->pack_id == pack_id)
						fprintf(pack_धाराः, " %s",
							oid_गन्तव्यम्_hex(&b->oid));
				}
			}
			प्रति (t = first_चिह्नकम्; t; t = t->अग्र_चिह्नकम्) {
				यदि (t->pack_id == pack_id)
					fprintf(pack_धाराः, " %s",
						oid_गन्तव्यम्_hex(&t->oid));
			}
			fputc('\n', pack_धाराः);
			fflush(pack_धाराः);
		}

		pack_id++;
	}
	अन्यथा {
discard_pack:
		close(pack_दत्त->pack_fd);
		unlink_or_warn(pack_दत्त->pack_name);
	}
	FREE_AND_NULL(pack_दत्त);
	running = 0;

	/* We can't carry a delta across packfiles. */
	strbuf_release(&चरमः_blob.दत्त);
	चरमः_blob.विस्थापनम् = 0;
	चरमः_blob.गभीरता = 0;
}

स्थिर शून्य आवृत्तिः_packfile(शून्य)
{
	अन्तः_packfile();
	start_packfile();
}

स्थिर पूर्णाङ्क store_object(
	गणना object_type type,
	संरचना strbuf *dat,
	संरचना चरमः_object *चरमः,
	संरचना object_id *oidout,
	uintmax_t mark)
{
	संरचना odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	शून्य *बहिर्गमनम्, *delta;
	संरचना object_प्रविष्टिः *e;
	अचिह्नित वर्ण hdr[96];
	संरचना object_id oid;
	अचिह्नित दीर्घ hdrlen, deltalen = 0;
	संरचना git_hash_ctx c;
	git_zstream s;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	hdrlen = format_object_शीर्षकम्((वर्ण *)hdr, परिमाण(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	यदि (oidout)
		oidcpy(oidout, &oid);

	e = निवेशय_object(&oid);
	यदि (mark)
		निवेशय_mark(&marks, mark, e);
	यदि (e->idx.विस्थापनम्) {
		duplicate_गणनम्_by_type[type]++;
		प्रत्यागम 1;
	}

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
		यदि (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			अनुवर्त;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.विस्थापनम् = 1; /* just not zero! */
		duplicate_गणनम्_by_type[type]++;
		प्रत्यागम 1;
	}

	यदि (चरमः && चरमः->दत्त.len && चरमः->दत्त.buf && चरमः->गभीरता < max_गभीरता
		&& dat->len > the_hash_algo->rawsz) {
		आकार_t deltalen_st;

		delta_गणनम्_attempts_by_type[type]++;
		delta = diff_delta(चरमः->दत्त.buf, चरमः->दत्त.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_आकार_t_गन्तव्यम्_ulong(deltalen_st);
	} अन्यथा
		delta = रिक्त;

	git_deflate_init(&s, cfg->pack_compression_level);
	यदि (delta) {
		s.अग्र_अन्तर् = delta;
		s.avail_अन्तर् = deltalen;
	} अन्यथा {
		s.अग्र_अन्तर् = (शून्य *)dat->buf;
		s.avail_अन्तर् = dat->len;
	}
	s.avail_बहिर्गमनम् = git_deflate_bound(&s, s.avail_अन्तर्);
	s.अग्र_बहिर्गमनम् = बहिर्गमनम् = xmalloc(s.avail_बहिर्गमनम्);
	यावत् (git_deflate(&s, Z_FINISH) == Z_OK)
		; /* nothing */
	git_deflate_अन्तः(&s);

	/* Determine if we should auto-checkpoint. */
	यदि ((max_packsize
		&& (pack_आकार + PACK_आकार_देहली + s.योग_बहिर्गमनम्) > max_packsize)
		|| (pack_आकार + PACK_आकार_देहली + s.योग_बहिर्गमनम्) < pack_आकार) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		आवृत्तिः_packfile();

		/* We cannot carry a delta into the new pack. */
		यदि (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.अग्र_अन्तर् = (शून्य *)dat->buf;
			s.avail_अन्तर् = dat->len;
			s.avail_बहिर्गमनम् = git_deflate_bound(&s, s.avail_अन्तर्);
			s.अग्र_बहिर्गमनम् = बहिर्गमनम् = xrealloc(बहिर्गमनम्, s.avail_बहिर्गमनम्);
			यावत् (git_deflate(&s, Z_FINISH) == Z_OK)
				; /* nothing */
			git_deflate_अन्तः(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.विस्थापनम् = pack_आकार;
	object_गणनम्++;
	object_गणनम्_by_type[type]++;

	चक्रीयपरीक्षा_32_begin(pack_file);

	यदि (delta) {
		off_t ofs = e->idx.विस्थापनम् - चरमः->विस्थापनम्;
		अचिह्नित pos = परिमाण(hdr) - 1;

		delta_गणनम्_by_type[type]++;
		e->गभीरता = चरमः->गभीरता + 1;

		hdrlen = encode_अन्तर्_pack_object_शीर्षकम्(hdr, परिमाण(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_आकार += hdrlen;

		hdr[pos] = ofs & 127;
		यावत् (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, परिमाण(hdr) - pos);
		pack_आकार += परिमाण(hdr) - pos;
	} अन्यथा {
		e->गभीरता = 0;
		hdrlen = encode_अन्तर्_pack_object_शीर्षकम्(hdr, परिमाण(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_आकार += hdrlen;
	}

	hashwrite(pack_file, बहिर्गमनम्, s.योग_बहिर्गमनम्);
	pack_आकार += s.योग_बहिर्गमनम्;

	e->idx.चक्रीयपरीक्षा_32 = चक्रीयपरीक्षा_32_अन्तः(pack_file);

	मुक्त(बहिर्गमनम्);
	मुक्त(delta);
	यदि (चरमः) {
		यदि (चरमः->no_swap) {
			चरमः->दत्त = *dat;
		} अन्यथा {
			strbuf_swap(&चरमः->दत्त, dat);
		}
		चरमः->विस्थापनम् = e->idx.विस्थापनम्;
		चरमः->गभीरता = e->गभीरता;
	}
	प्रत्यागम 0;
}

स्थिर शून्य truncate_pack(संरचना hashfile_checkpoint *checkpoint)
{
	यदि (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_आकार = checkpoint->विस्थापनम्;
}

स्थिर शून्य प्रवाहः_blob(uintmax_t len, संरचना object_id *oidout, uintmax_t mark)
{
	संरचना odb_source_files *files = odb_source_files_downcast(the_repository->objects->source);
	आकार_t अन्तर्_sz = 64 * 1024, बहिर्गमनम्_sz = 64 * 1024;
	अचिह्नित वर्ण *अन्तर्_buf = xmalloc(अन्तर्_sz);
	अचिह्नित वर्ण *बहिर्गमनम्_buf = xmalloc(बहिर्गमनम्_sz);
	संरचना object_प्रविष्टिः *e;
	संरचना object_id oid;
	अचिह्नित दीर्घ hdrlen;
	off_t विस्थापनम्;
	संरचना git_hash_ctx c;
	git_zstream s;
	संरचना hashfile_checkpoint checkpoint;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);
	पूर्णाङ्क स्थितिः = Z_OK;

	/* Determine if we should auto-checkpoint. */
	यदि ((max_packsize
		&& (pack_आकार + PACK_आकार_देहली + len) > max_packsize)
		|| (pack_आकार + PACK_आकार_देहली + len) < pack_आकार)
		आवृत्तिः_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	विस्थापनम् = checkpoint.विस्थापनम्;

	hdrlen = format_object_शीर्षकम्((वर्ण *)बहिर्गमनम्_buf, बहिर्गमनम्_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, बहिर्गमनम्_buf, hdrlen);

	चक्रीयपरीक्षा_32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_अन्तर्_pack_object_शीर्षकम्(बहिर्गमनम्_buf, बहिर्गमनम्_sz, OBJ_BLOB, len);

	s.अग्र_बहिर्गमनम् = बहिर्गमनम्_buf + hdrlen;
	s.avail_बहिर्गमनम् = बहिर्गमनम्_sz - hdrlen;

	यावत् (स्थितिः != Z_प्रवाहः_अन्तः) {
		यदि (0 < len && !s.avail_अन्तर्) {
			आकार_t cnt = अन्तर्_sz < len ? अन्तर्_sz : (आकार_t)len;
			आकार_t n = fread(अन्तर्_buf, 1, cnt, stdin);
			यदि (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, अन्तर्_buf, n);
			s.अग्र_अन्तर् = अन्तर्_buf;
			s.avail_अन्तर् = n;
			len -= n;
		}

		स्थितिः = git_deflate(&s, len ? 0 : Z_FINISH);

		यदि (!s.avail_बहिर्गमनम् || स्थितिः == Z_प्रवाहः_अन्तः) {
			आकार_t n = s.अग्र_बहिर्गमनम् - बहिर्गमनम्_buf;
			hashwrite(pack_file, बहिर्गमनम्_buf, n);
			pack_आकार += n;
			s.अग्र_बहिर्गमनम् = बहिर्गमनम्_buf;
			s.avail_बहिर्गमनम् = बहिर्गमनम्_sz;
		}

		विकल्प (स्थितिः) {
		पक्ष Z_OK:
		पक्ष Z_BUF_ERROR:
		पक्ष Z_प्रवाहः_अन्तः:
			अनुवर्त;
		स्वभाव:
			die(_("unexpected deflate failure: %d"), स्थितिः);
		}
	}
	git_deflate_अन्तः(&s);
	git_hash_final_oid(&oid, &c);

	यदि (oidout)
		oidcpy(oidout, &oid);

	e = निवेशय_object(&oid);

	यदि (mark)
		निवेशय_mark(&marks, mark, e);

	यदि (e->idx.विस्थापनम्) {
		duplicate_गणनम्_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		गच्छ बहिर्गमनम्;
	}

	प्रति (संरचना odb_files_dir *dir = files->dirs; dir; dir = dir->अग्र) {
		यदि (!packfile_list_find_oid(packfile_store_get_packs(dir->packed), &oid))
			अनुवर्त;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.विस्थापनम् = 1; /* just not zero! */
		duplicate_गणनम्_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		गच्छ बहिर्गमनम्;
	}

	e->गभीरता = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.विस्थापनम् = विस्थापनम्;
	e->idx.चक्रीयपरीक्षा_32 = चक्रीयपरीक्षा_32_अन्तः(pack_file);
	object_गणनम्++;
	object_गणनम्_by_type[OBJ_BLOB]++;

बहिर्गमनम्:
	मुक्त(अन्तर्_buf);
	मुक्त(बहिर्गमनम्_buf);
	hashfile_checkpoint_release(&checkpoint);
}

/* All calls must be guarded by find_object() or find_mark() to
 * ensure the 'struct object_entry' passed was written by this
 * process instance.  We unpack the entry by the offset, avoiding
 * the need for the corresponding .idx file.  This unpacking rule
 * works because we only use OBJ_REF_DELTA within the packfiles
 * created by fast-import.
 *
 * oe must not be NULL.  Such an oe usually comes from giving
 * an unknown SHA-1 to find_object() or an undefined mark to
 * find_mark().  Callers must test for this condition and use
 * the standard read_sha1_file() when it happens.
 *
 * oe->pack_id must not be MAX_PACK_ID.  Such an oe is usually from
 * find_mark(), where the mark was reloaded from an existing marks
 * file and is referencing an object that this fast-import process
 * instance did not write out to a packfile.  Callers must test for
 * this condition and use read_sha1_file() instead.
 */
स्थिर शून्य *gfi_unpack_प्रविष्टिः(
	संरचना object_प्रविष्टिः *oe,
	अचिह्नित दीर्घ *sizep)
{
	गणना object_type type;
	आकार_t आकार_st = 0;
	शून्य *दत्त;
	संरचना packed_git *p = all_packs[oe->pack_id];
	यदि (p == pack_दत्त && p->pack_आकार < (pack_आकार + the_hash_algo->rawsz)) {
		/* The object is stored in the packfile we are writing to
		 * and we have modified it since the last time we scanned
		 * back to read a previously written object.  If an old
		 * window covered [p->pack_size, p->pack_size + rawsz) its
		 * data is stale and is not valid.  Closing all windows
		 * and updating the packfile length ensures we can read
		 * the newly written data.
		 */
		close_pack_windows(p);
		hashflush(pack_file);

		/* We have to offer rawsz bytes additional on the end of
		 * the packfile as the core unpacker code assumes the
		 * footer is present at the file end and must promise
		 * at least rawsz bytes within any window it maps.  But
		 * we don't actually create the footer here.
		 */
		p->pack_आकार = pack_आकार + the_hash_algo->rawsz;
	}
	दत्त = unpack_प्रविष्टिः(the_repository, p, oe->idx.विस्थापनम्, &type, &आकार_st);
	यदि (sizep)
		*sizep = cast_आकार_t_गन्तव्यम्_ulong(आकार_st);
	प्रत्यागम दत्त;
}

स्थिर शून्य आरोपय_वृक्षः(संरचना वृक्षः_प्रविष्टिः *मूलम्)
{
	संरचना object_id *oid = &मूलम्->versions[1].oid;
	संरचना object_प्रविष्टिः *myoe;
	संरचना वृक्षः_content *t;
	अचिह्नित दीर्घ आकार;
	वर्ण *buf;
	ध्रुव वर्ण *c;

	मूलम्->वृक्षः = t = new_वृक्षः_content(8);
	यदि (is_null_oid(oid))
		प्रत्यागम;

	myoe = find_object(oid);
	यदि (myoe && myoe->pack_id != MAX_PACK_ID) {
		यदि (myoe->type != OBJ_वृक्षः)
			die(_("not a tree: %s"), oid_गन्तव्यम्_hex(oid));
		t->delta_गभीरता = myoe->गभीरता;
		buf = gfi_unpack_प्रविष्टिः(myoe, &आकार);
		यदि (!buf)
			die(_("can't load tree %s"), oid_गन्तव्यम्_hex(oid));
	} अन्यथा {
		गणना object_type type;
		आकार_t आकार_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &आकार_st);
		आकार = cast_आकार_t_गन्तव्यम्_ulong(आकार_st);
		यदि (!buf || type != OBJ_वृक्षः)
			die(_("can't load tree %s"), oid_गन्तव्यम्_hex(oid));
	}

	c = buf;
	यावत् (c != (buf + आकार)) {
		संरचना वृक्षः_प्रविष्टिः *e = new_वृक्षः_प्रविष्टिः();

		यदि (t->प्रविष्टिः_गणनम् == t->प्रविष्टिः_क्षमता)
			मूलम्->वृक्षः = t = grow_वृक्षः_content(t, t->प्रविष्टिः_गणनम्);
		t->entries[t->प्रविष्टिः_गणनम्++] = e;

		e->वृक्षः = रिक्त;
		c = parse_बहुलकम्(c, &e->versions[1].बहुलकम्);
		यदि (!c)
			die(_("corrupt mode in %s"), oid_गन्तव्यम्_hex(oid));
		e->versions[0].बहुलकम् = e->versions[1].बहुलकम्;
		e->name = गन्तव्यम्_atom(c, वाक्यदैर्घ्यम्(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (अचिह्नित वर्ण *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (अचिह्नित वर्ण *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	मुक्त(buf);
}

स्थिर पूर्णाङ्क tecmp0 (ध्रुव शून्य *_a, ध्रुव शून्य *_b)
{
	संरचना वृक्षः_प्रविष्टिः *a = *((संरचना वृक्षः_प्रविष्टिः**)_a);
	संरचना वृक्षः_प्रविष्टिः *b = *((संरचना वृक्षः_प्रविष्टिः**)_b);
	प्रत्यागम आधारः_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].बहुलकम्,
		b->name->str_dat, b->name->str_len, b->versions[0].बहुलकम्);
}

स्थिर पूर्णाङ्क tecmp1 (ध्रुव शून्य *_a, ध्रुव शून्य *_b)
{
	संरचना वृक्षः_प्रविष्टिः *a = *((संरचना वृक्षः_प्रविष्टिः**)_a);
	संरचना वृक्षः_प्रविष्टिः *b = *((संरचना वृक्षः_प्रविष्टिः**)_b);
	प्रत्यागम आधारः_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].बहुलकम्,
		b->name->str_dat, b->name->str_len, b->versions[1].बहुलकम्);
}

स्थिर शून्य mktree(संरचना वृक्षः_content *t, पूर्णाङ्क v, संरचना strbuf *b)
{
	आकार_t maxlen = 0;
	अचिह्नित पूर्णाङ्क i;

	यदि (!v)
		QSORT(t->entries, t->प्रविष्टिः_गणनम्, tecmp0);
	अन्यथा
		QSORT(t->entries, t->प्रविष्टिः_गणनम्, tecmp1);

	प्रति (i = 0; i < t->प्रविष्टिः_गणनम्; i++) {
		यदि (t->entries[i]->versions[v].बहुलकम्)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	प्रति (i = 0; i < t->प्रविष्टिः_गणनम्; i++) {
		संरचना वृक्षः_प्रविष्टिः *e = t->entries[i];
		यदि (!e->versions[v].बहुलकम्)
			अनुवर्त;
		strbuf_addf(b, "%o %s%c",
			(अचिह्नित पूर्णाङ्क)(e->versions[v].बहुलकम् & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_सङ्कलय(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

स्थिर शून्य store_वृक्षः(संरचना वृक्षः_प्रविष्टिः *मूलम्)
{
	संरचना वृक्षः_content *t;
	अचिह्नित पूर्णाङ्क i, j, del;
	संरचना चरमः_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	संरचना object_प्रविष्टिः *le = रिक्त;

	यदि (!is_null_oid(&मूलम्->versions[1].oid))
		प्रत्यागम;

	यदि (!मूलम्->वृक्षः)
		आरोपय_वृक्षः(मूलम्);
	t = मूलम्->वृक्षः;

	प्रति (i = 0; i < t->प्रविष्टिः_गणनम्; i++) {
		यदि (t->entries[i]->वृक्षः)
			store_वृक्षः(t->entries[i]);
	}

	यदि (!(मूलम्->versions[0].बहुलकम् & NO_DELTA))
		le = find_object(&मूलम्->versions[0].oid);
	यदि (S_ISDIR(मूलम्->versions[0].बहुलकम्) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_वृक्षः);
		lo.दत्त = old_वृक्षः;
		lo.विस्थापनम् = le->idx.विस्थापनम्;
		lo.गभीरता = t->delta_गभीरता;
	}

	mktree(t, 1, &new_वृक्षः);
	store_object(OBJ_वृक्षः, &new_वृक्षः, &lo, &मूलम्->versions[1].oid, 0);

	t->delta_गभीरता = lo.गभीरता;
	प्रति (i = 0, j = 0, del = 0; i < t->प्रविष्टिः_गणनम्; i++) {
		संरचना वृक्षः_प्रविष्टिः *e = t->entries[i];
		यदि (e->versions[1].बहुलकम्) {
			e->versions[0].बहुलकम् = e->versions[1].बहुलकम्;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} अन्यथा {
			release_वृक्षः_प्रविष्टिः(e);
			del++;
		}
	}
	t->प्रविष्टिः_गणनम् -= del;
}

स्थिर शून्य वृक्षः_content_replace(
	संरचना वृक्षः_प्रविष्टिः *मूलम्,
	ध्रुव संरचना object_id *oid,
	ध्रुव uint16_t बहुलकम्,
	संरचना वृक्षः_content *newtree)
{
	यदि (!S_ISDIR(बहुलकम्))
		die(_("root cannot be a non-directory"));
	oidclr(&मूलम्->versions[0].oid, the_repository->hash_algo);
	oidcpy(&मूलम्->versions[1].oid, oid);
	यदि (मूलम्->वृक्षः)
		release_वृक्षः_content_recursive(मूलम्->वृक्षः);
	मूलम्->वृक्षः = newtree;
}

स्थिर पूर्णाङ्क वृक्षः_content_समुच्चयः(
	संरचना वृक्षः_प्रविष्टिः *मूलम्,
	ध्रुव वर्ण *p,
	ध्रुव संरचना object_id *oid,
	ध्रुव uint16_t बहुलकम्,
	संरचना वृक्षः_content *subtree)
{
	संरचना वृक्षः_content *t;
	ध्रुव वर्ण *slash1;
	अचिह्नित पूर्णाङ्क i, n;
	संरचना वृक्षः_प्रविष्टिः *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	यदि (!n)
		die(_("empty path component found in input"));
	यदि (!*slash1 && !S_ISDIR(बहुलकम्) && subtree)
		die(_("non-directories cannot have subtrees"));

	यदि (!मूलम्->वृक्षः)
		आरोपय_वृक्षः(मूलम्);
	t = मूलम्->वृक्षः;
	प्रति (i = 0; i < t->प्रविष्टिः_गणनम्; i++) {
		e = t->entries[i];
		यदि (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			यदि (!*slash1) {
				यदि (!S_ISDIR(बहुलकम्)
						&& e->versions[1].बहुलकम् == बहुलकम्
						&& oideq(&e->versions[1].oid, oid))
					प्रत्यागम 0;
				e->versions[1].बहुलकम् = बहुलकम्;
				oidcpy(&e->versions[1].oid, oid);
				यदि (e->वृक्षः)
					release_वृक्षः_content_recursive(e->वृक्षः);
				e->वृक्षः = subtree;

				/*
				 * We need to leave e->versions[0].sha1 alone
				 * to avoid modifying the preimage tree used
				 * when writing out the parent directory.
				 * But after replacing the subdir with a
				 * completely different one, it's not a good
				 * delta base any more, and besides, we've
				 * thrown away the tree entries needed to
				 * make a delta against it.
				 *
				 * So let's just explicitly disable deltas
				 * for the subtree.
				 */
				यदि (S_ISDIR(e->versions[0].बहुलकम्))
					e->versions[0].बहुलकम् |= NO_DELTA;

				oidclr(&मूलम्->versions[1].oid, the_repository->hash_algo);
				प्रत्यागम 1;
			}
			यदि (!S_ISDIR(e->versions[1].बहुलकम्)) {
				e->वृक्षः = new_वृक्षः_content(8);
				e->versions[1].बहुलकम् = S_IFDIR;
			}
			यदि (!e->वृक्षः)
				आरोपय_वृक्षः(e);
			यदि (वृक्षः_content_समुच्चयः(e, slash1 + 1, oid, बहुलकम्, subtree)) {
				oidclr(&मूलम्->versions[1].oid, the_repository->hash_algo);
				प्रत्यागम 1;
			}
			प्रत्यागम 0;
		}
	}

	यदि (t->प्रविष्टिः_गणनम् == t->प्रविष्टिः_क्षमता)
		मूलम्->वृक्षः = t = grow_वृक्षः_content(t, t->प्रविष्टिः_गणनम्);
	e = new_वृक्षः_प्रविष्टिः();
	e->name = गन्तव्यम्_atom(p, n);
	e->versions[0].बहुलकम् = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->प्रविष्टिः_गणनम्++] = e;
	यदि (*slash1) {
		e->वृक्षः = new_वृक्षः_content(8);
		e->versions[1].बहुलकम् = S_IFDIR;
		वृक्षः_content_समुच्चयः(e, slash1 + 1, oid, बहुलकम्, subtree);
	} अन्यथा {
		e->वृक्षः = subtree;
		e->versions[1].बहुलकम् = बहुलकम्;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&मूलम्->versions[1].oid, the_repository->hash_algo);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क वृक्षः_content_remove(
	संरचना वृक्षः_प्रविष्टिः *मूलम्,
	ध्रुव वर्ण *p,
	संरचना वृक्षः_प्रविष्टिः *backup_पर्णम्,
	पूर्णाङ्क allow_मूलम्)
{
	संरचना वृक्षः_content *t;
	ध्रुव वर्ण *slash1;
	अचिह्नित पूर्णाङ्क i, n;
	संरचना वृक्षः_प्रविष्टिः *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	यदि (!मूलम्->वृक्षः)
		आरोपय_वृक्षः(मूलम्);

	यदि (!*p && allow_मूलम्) {
		e = मूलम्;
		गच्छ del_प्रविष्टिः;
	}

	t = मूलम्->वृक्षः;
	प्रति (i = 0; i < t->प्रविष्टिः_गणनम्; i++) {
		e = t->entries[i];
		यदि (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			यदि (*slash1 && !S_ISDIR(e->versions[1].बहुलकम्))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				प्रत्यागम 1;
			यदि (!*slash1 || !S_ISDIR(e->versions[1].बहुलकम्))
				गच्छ del_प्रविष्टिः;
			यदि (!e->वृक्षः)
				आरोपय_वृक्षः(e);
			यदि (वृक्षः_content_remove(e, slash1 + 1, backup_पर्णम्, 0)) {
				प्रति (n = 0; n < e->वृक्षः->प्रविष्टिः_गणनम्; n++) {
					यदि (e->वृक्षः->entries[n]->versions[1].बहुलकम्) {
						oidclr(&मूलम्->versions[1].oid,
						       the_repository->hash_algo);
						प्रत्यागम 1;
					}
				}
				backup_पर्णम् = रिक्त;
				गच्छ del_प्रविष्टिः;
			}
			प्रत्यागम 0;
		}
	}
	प्रत्यागम 0;

del_प्रविष्टिः:
	यदि (backup_पर्णम्)
		स्मृतिं_प्रतिलिख(backup_पर्णम्, e, परिमाण(*backup_पर्णम्));
	अन्यथा यदि (e->वृक्षः)
		release_वृक्षः_content_recursive(e->वृक्षः);
	e->वृक्षः = रिक्त;
	e->versions[1].बहुलकम् = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&मूलम्->versions[1].oid, the_repository->hash_algo);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क वृक्षः_content_get(
	संरचना वृक्षः_प्रविष्टिः *मूलम्,
	ध्रुव वर्ण *p,
	संरचना वृक्षः_प्रविष्टिः *पर्णम्,
	पूर्णाङ्क allow_मूलम्)
{
	संरचना वृक्षः_content *t;
	ध्रुव वर्ण *slash1;
	अचिह्नित पूर्णाङ्क i, n;
	संरचना वृक्षः_प्रविष्टिः *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	यदि (!n && !allow_मूलम्)
		die(_("empty path component found in input"));

	यदि (!मूलम्->वृक्षः)
		आरोपय_वृक्षः(मूलम्);

	यदि (!n) {
		e = मूलम्;
		गच्छ प्राप्तम्_प्रविष्टिः;
	}

	t = मूलम्->वृक्षः;
	प्रति (i = 0; i < t->प्रविष्टिः_गणनम्; i++) {
		e = t->entries[i];
		यदि (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			यदि (!*slash1)
				गच्छ प्राप्तम्_प्रविष्टिः;
			यदि (!S_ISDIR(e->versions[1].बहुलकम्))
				प्रत्यागम 0;
			यदि (!e->वृक्षः)
				आरोपय_वृक्षः(e);
			प्रत्यागम वृक्षः_content_get(e, slash1 + 1, पर्णम्, 0);
		}
	}
	प्रत्यागम 0;

प्राप्तम्_प्रविष्टिः:
	स्मृतिं_प्रतिलिख(पर्णम्, e, परिमाण(*पर्णम्));
	यदि (e->वृक्षः && is_null_oid(&e->versions[1].oid))
		पर्णम्->वृक्षः = dup_वृक्षः_content(e->वृक्षः);
	अन्यथा
		पर्णम्->वृक्षः = रिक्त;
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क update_branch(संरचना branch *b)
{
	स्थिर ध्रुव वर्ण *सन्देशः = "fast-import";
	संरचना ref_transaction *transaction;
	संरचना object_id old_oid;
	संरचना strbuf err = STRBUF_INIT;
	स्थिर ध्रुव वर्ण *replace_उपसर्गः = "refs/replace/";

	यदि (starts_with(b->name, replace_उपसर्गः) &&
	    !वाक्यं_तुलय(b->name + वाक्यदैर्घ्यम्(replace_उपसर्गः),
		    oid_गन्तव्यम्_hex(&b->oid))) {
		यदि (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_गन्तव्यम्_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				रिक्त, b->name, रिक्त, 0);
		प्रत्यागम 0;
	}
	यदि (is_null_oid(&b->oid)) {
		यदि (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					रिक्त, b->name, रिक्त, 0);
		प्रत्यागम 0;
	}
	यदि (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	यदि (!force_update && !is_null_oid(&old_oid)) {
		संरचना commit *old_cmit, *new_cmit;
		पूर्णाङ्क ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		यदि (!old_cmit || !new_cmit)
			प्रत्यागम error(_("branch %s is missing commits."), b->name);

		ret = repo_अन्तर्_merge_bases(the_repository, old_cmit, new_cmit);
		यदि (ret < 0)
			निर्गच्छ(128);
		यदि (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_गन्तव्यम्_hex(&b->oid),
				oid_गन्तव्यम्_hex(&old_oid));
			प्रत्यागम -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	यदि (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   रिक्त, रिक्त, 0, सन्देशः, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		प्रत्यागम -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	प्रत्यागम 0;
}

स्थिर शून्य dump_branches(शून्य)
{
	अचिह्नित पूर्णाङ्क i;
	संरचना branch *b;

	प्रति (i = 0; i < branch_सारणी_sz; i++) {
		प्रति (b = branch_सारणी[i]; b; b = b->सारणी_अग्र_branch)
			failure |= update_branch(b);
	}
}

स्थिर शून्य dump_चिह्नकानि(शून्य)
{
	स्थिर ध्रुव वर्ण *सन्देशः = "fast-import";
	संरचना चिह्नकम् *t;
	संरचना strbuf ref_name = STRBUF_INIT;
	संरचना strbuf err = STRBUF_INIT;
	संरचना ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	यदि (!transaction) {
		failure |= error("%s", err.buf);
		गच्छ cleanup;
	}
	प्रति (t = first_चिह्नकम्; t; t = t->अग्र_चिह्नकम्) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		यदि (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, रिक्त, रिक्त, रिक्त,
					   0, सन्देशः, &err)) {
			failure |= error("%s", err.buf);
			गच्छ cleanup;
		}
	}
	यदि (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

स्थिर शून्य dump_marks(शून्य)
{
	संरचना तालकम्_file mark_तालकम् = तालकम्_INIT;
	FILE *f;

	यदि (!export_marks_file || (import_marks_file && !import_marks_file_समाप्तम्))
		प्रत्यागम;

	यदि (safe_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		प्रत्यागम;
	}

	यदि (hold_तालकम्_file_for_update(&mark_तालकम्, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		प्रत्यागम;
	}

	f = fdopen_तालकम्_file(&mark_तालकम्, "w");
	यदि (!f) {
		पूर्णाङ्क saved_errno = errno;
		rollback_तालकम्_file(&mark_तालकम्);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		प्रत्यागम;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	यदि (commit_तालकम्_file(&mark_तालकम्)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		प्रत्यागम;
	}
}

स्थिर शून्य निवेशय_object_प्रविष्टिः(संरचना mark_समुच्चयः **s, संरचना object_id *oid, uintmax_t mark)
{
	संरचना object_प्रविष्टिः *e;
	e = find_object(oid);
	यदि (!e) {
		गणना object_type type = odb_read_object_info(the_repository->objects,
							     oid, रिक्त);
		यदि (type < 0)
			die(_("object not found: %s"), oid_गन्तव्यम्_hex(oid));
		e = निवेशय_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.विस्थापनम् = 1; /* just not zero! */
	}
	निवेशय_mark(s, mark, e);
}

स्थिर शून्य निवेशय_oid_प्रविष्टिः(संरचना mark_समुच्चयः **s, संरचना object_id *oid, uintmax_t mark)
{
	निवेशय_mark(s, mark, xmemdupz(oid, परिमाण(*oid)));
}

स्थिर शून्य read_mark_file(संरचना mark_समुच्चयः **s, FILE *f, mark_समुच्चयः_inserter_t inserter)
{
	वर्ण पङ्क्तिः[512];
	यावत् (fgets(पङ्क्तिः, परिमाण(पङ्क्तिः), f)) {
		uintmax_t mark;
		वर्ण *अन्तः;
		संरचना object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		स्मृतिं_पूरय(oid.hash, 0, परिमाण(oid.hash));

		अन्तः = strchr(पङ्क्तिः, '\n');
		यदि (पङ्क्तिः[0] != ':' || !अन्तः)
			die(_("corrupt mark line: %s"), पङ्क्तिः);
		*अन्तः = 0;
		mark = strtoumax(पङ्क्तिः + 1, &अन्तः, 10);
		यदि (!mark || अन्तः == पङ्क्तिः + 1
			|| *अन्तः != ' '
			|| get_oid_hex_any(अन्तः + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), पङ्क्तिः);
		inserter(s, &oid, mark);
	}
}

स्थिर शून्य read_marks(शून्य)
{
	FILE *f = fopen(import_marks_file, "r");
	यदि (f)
		;
	अन्यथा यदि (import_marks_file_ignore_missing && errno == ENOENT)
		गच्छ समाप्तम्; /* Marks file does not exist */
	अन्यथा
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, निवेशय_object_प्रविष्टिः);
	fclose(f);
समाप्तम्:
	import_marks_file_समाप्तम् = 1;
}


स्थिर पूर्णाङ्क read_अग्र_command(संरचना fast_import_state *state)
{
	स्थिर पूर्णाङ्क stdin_eof = 0;

	यदि (stdin_eof) {
		unread_command_buf = 0;
		प्रत्यागम EOF;
	}

	प्रति (;;) {
		यदि (unread_command_buf) {
			unread_command_buf = 0;
		} अन्यथा {
			संरचना recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			यदि (stdin_eof)
				प्रत्यागम EOF;

			यदि (!state->आलोकितम्_दत्त_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			यदि (rc)
				rc_free = rc->अग्र;
			अन्यथा {
				rc = cmd_hist.अग्र;
				cmd_hist.अग्र = rc->अग्र;
				cmd_hist.अग्र->पूर्व = &cmd_hist;
				मुक्त(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->पूर्व = cmd_पुच्छ;
			rc->अग्र = cmd_hist.पूर्व;
			rc->पूर्व->अग्र = rc;
			cmd_पुच्छ = rc;
		}
		यदि (command_buf.buf[0] == '#')
			अनुवर्त;
		प्रत्यागम 0;
	}
}

स्थिर शून्य skip_optional_lf(शून्य)
{
	पूर्णाङ्क पदांशः_char = fgetc(stdin);
	यदि (पदांशः_char != '\n' && पदांशः_char != EOF)
		ungetc(पदांशः_char, stdin);
}

स्थिर शून्य parse_mark(संरचना fast_import_state *state)
{
	ध्रुव वर्ण *v;
	यदि (skip_उपसर्गः(command_buf.buf, "mark :", &v)) {
		अग्र_mark = strtoumax(v, रिक्त, 10);
		read_अग्र_command(state);
	}
	अन्यथा
		अग्र_mark = 0;
}

स्थिर शून्य parse_original_identifier(संरचना fast_import_state *state)
{
	ध्रुव वर्ण *v;
	यदि (skip_उपसर्गः(command_buf.buf, "original-oid ", &v))
		read_अग्र_command(state);
}

स्थिर पूर्णाङ्क parse_दत्त(संरचना strbuf *sb, uintmax_t सीमा, uintmax_t *len_res)
{
	ध्रुव वर्ण *दत्त;
	strbuf_reset(sb);

	यदि (!skip_उपसर्गः(command_buf.buf, "data ", &दत्त))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	यदि (skip_उपसर्गः(दत्त, "<<", &दत्त)) {
		वर्ण *पदांशः = xstrdup(दत्त);
		आकार_t पदांशः_len = command_buf.len - (दत्त - command_buf.buf);

		प्रति (;;) {
			यदि (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), पदांशः);
			यदि (पदांशः_len == command_buf.len
				&& !वाक्यं_तुलय(पदांशः, command_buf.buf))
				भङ्ग;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		मुक्त(पदांशः);
	}
	अन्यथा {
		uintmax_t len = strtoumax(दत्त, रिक्त, 10);
		आकार_t n = 0, दीर्घता = (आकार_t)len;

		यदि (सीमा && सीमा < len) {
			*len_res = len;
			प्रत्यागम 0;
		}
		यदि (दीर्घता < len)
			die(_("data is too large to use in this context"));

		यावत् (n < दीर्घता) {
			आकार_t s = strbuf_fread(sb, दीर्घता - n, stdin);
			यदि (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(अचिह्नित दीर्घ)(दीर्घता - n));
			n += s;
		}
	}

	skip_optional_lf();
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क validate_raw_date(ध्रुव वर्ण *src, संरचना strbuf *परिणाम, पूर्णाङ्क strict)
{
	ध्रुव वर्ण *orig_src = src;
	वर्ण *endp;
	अचिह्नित दीर्घ अङ्कः;

	errno = 0;

	अङ्कः = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	यदि (errno || endp == src || *endp != ' ')
		प्रत्यागम -1;

	src = endp + 1;
	यदि (*src != '-' && *src != '+')
		प्रत्यागम -1;

	अङ्कः = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	यदि (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < अङ्कः))             /* parsed a broken timezone */
	   )
		प्रत्यागम -1;

	strbuf_addstr(परिणाम, orig_src);
	प्रत्यागम 0;
}

स्थिर वर्ण *parse_ident(ध्रुव वर्ण *buf)
{
	ध्रुव वर्ण *ltgt;
	आकार_t name_len;
	संरचना strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	यदि (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	यदि (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	यदि (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	यदि (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	यदि (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_सङ्कलय(&ident, buf, name_len);

	विकल्प (whenspec) {
	पक्ष WHENSPEC_RAW:
		यदि (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		भङ्ग;
	पक्ष WHENSPEC_RAW_PERMISSIVE:
		यदि (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		भङ्ग;
	पक्ष WHENSPEC_RFC2822:
		यदि (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		भङ्ग;
	पक्ष WHENSPEC_NOW:
		यदि (वाक्यं_तुलय("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		भङ्ग;
	}

	प्रत्यागम strbuf_detach(&ident, रिक्त);
}

स्थिर शून्य parse_and_store_blob(
	संरचना चरमः_object *चरमः,
	संरचना object_id *oidout,
	uintmax_t mark)
{
	स्थिर संरचना strbuf buf = STRBUF_INIT;
	uintmax_t len;

	यदि (parse_दत्त(&buf, repo_settings_get_big_file_देहली(the_repository), &len))
		store_object(OBJ_BLOB, &buf, चरमः, oidout, mark);
	अन्यथा {
		यदि (चरमः) {
			strbuf_release(&चरमः->दत्त);
			चरमः->विस्थापनम् = 0;
			चरमः->गभीरता = 0;
		}
		प्रवाहः_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

स्थिर शून्य parse_new_blob(संरचना fast_import_state *state)
{
	read_अग्र_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&चरमः_blob, रिक्त, अग्र_mark);
}

स्थिर शून्य unload_one_branch(शून्य)
{
	यावत् (cur_सक्रिय_branches
		&& cur_सक्रिय_branches >= max_सक्रिय_branches) {
		uintmax_t min_commit = ULONG_MAX;
		संरचना branch *e, *l = रिक्त, *p = रिक्त;

		प्रति (e = सक्रिय_branches; e; e = e->सक्रिय_अग्र_branch) {
			यदि (e->चरमः_commit < min_commit) {
				p = l;
				min_commit = e->चरमः_commit;
			}
			l = e;
		}

		यदि (p) {
			e = p->सक्रिय_अग्र_branch;
			p->सक्रिय_अग्र_branch = e->सक्रिय_अग्र_branch;
		} अन्यथा {
			e = सक्रिय_branches;
			सक्रिय_branches = e->सक्रिय_अग्र_branch;
		}
		e->सक्रिय = 0;
		e->सक्रिय_अग्र_branch = रिक्त;
		यदि (e->branch_वृक्षः.वृक्षः) {
			release_वृक्षः_content_recursive(e->branch_वृक्षः.वृक्षः);
			e->branch_वृक्षः.वृक्षः = रिक्त;
		}
		cur_सक्रिय_branches--;
	}
}

स्थिर शून्य आरोपय_branch(संरचना branch *b)
{
	आरोपय_वृक्षः(&b->branch_वृक्षः);
	यदि (!b->सक्रिय) {
		b->सक्रिय = 1;
		b->सक्रिय_अग्र_branch = सक्रिय_branches;
		सक्रिय_branches = b;
		cur_सक्रिय_branches++;
		branch_आरोपय_गणनम्++;
	}
}

स्थिर अचिह्नित वर्ण convert_अङ्कः_notes_गन्तव्यम्_fanout(uintmax_t अङ्कः_notes)
{
	अचिह्नित वर्ण fanout = 0;
	यावत् ((अङ्कः_notes >>= 8))
		fanout++;
	प्रत्यागम fanout;
}

स्थिर शून्य construct_मार्गः_with_fanout(ध्रुव वर्ण *hex_sha1,
		अचिह्नित वर्ण fanout, वर्ण *मार्गः)
{
	अचिह्नित पूर्णाङ्क i = 0, j = 0;
	यदि (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	यावत् (fanout) {
		मार्गः[i++] = hex_sha1[j++];
		मार्गः[i++] = hex_sha1[j++];
		मार्गः[i++] = '/';
		fanout--;
	}
	स्मृतिं_प्रतिलिख(मार्गः + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	मार्गः[i + the_hash_algo->hexsz - j] = '\0';
}

स्थिर uintmax_t do_change_note_fanout(
		संरचना वृक्षः_प्रविष्टिः *orig_मूलम्, संरचना वृक्षः_प्रविष्टिः *मूलम्,
		वर्ण *hex_oid, अचिह्नित पूर्णाङ्क hex_oid_len,
		वर्ण *fullpath, अचिह्नित पूर्णाङ्क fullpath_len,
		अचिह्नित वर्ण fanout)
{
	संरचना वृक्षः_content *t;
	संरचना वृक्षः_प्रविष्टिः *e, पर्णम्;
	अचिह्नित पूर्णाङ्क i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t अङ्कः_notes = 0;
	संरचना object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	वर्ण realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	ध्रुव अचिह्नित hexsz = the_hash_algo->hexsz;

	यदि (!मूलम्->वृक्षः)
		आरोपय_वृक्षः(मूलम्);
	t = मूलम्->वृक्षः;

	प्रति (i = 0; t && i < t->प्रविष्टिः_गणनम्; i++) {
		e = t->entries[i];
		tmp_hex_oid_len = hex_oid_len + e->name->str_len;
		tmp_fullpath_len = fullpath_len;

		/*
		 * We're interested in EITHER existing note entries (entries
		 * with exactly 40 hex chars in path, not including directory
		 * separators), OR directory entries that may contain note
		 * entries (with < 40 hex chars in path).
		 * Also, each path component in a note entry must be a multiple
		 * of 2 chars.
		 */
		यदि (!e->versions[1].बहुलकम् ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			अनुवर्त;

		/* This _may_ be a note entry, or a subdir containing notes */
		स्मृतिं_प्रतिलिख(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		यदि (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		स्मृतिं_प्रतिलिख(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		यदि (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			यदि (fanout == 0xff) {
				/* Counting mode, no rename */
				अङ्कः_notes++;
				अनुवर्त;
			}
			construct_मार्गः_with_fanout(hex_oid, fanout, realpath);
			यदि (!वाक्यं_तुलय(fullpath, realpath)) {
				/* Note entry is in correct location */
				अङ्कः_notes++;
				अनुवर्त;
			}

			/* Rename fullpath to realpath */
			यदि (!वृक्षः_content_remove(orig_मूलम्, fullpath, &पर्णम्, 0))
				die(_("failed to remove path %s"), fullpath);
			वृक्षः_content_समुच्चयः(orig_मूलम्, realpath,
				&पर्णम्.versions[1].oid,
				पर्णम्.versions[1].बहुलकम्,
				पर्णम्.वृक्षः);
		} अन्यथा यदि (S_ISDIR(e->versions[1].बहुलकम्)) {
			/* This is a subdir that may contain note entries */
			अङ्कः_notes += do_change_note_fanout(orig_मूलम्, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = मूलम्->वृक्षः;
	}
	प्रत्यागम अङ्कः_notes;
}

स्थिर uintmax_t change_note_fanout(संरचना वृक्षः_प्रविष्टिः *मूलम्,
		अचिह्नित वर्ण fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	वर्ण hex_oid[GIT_MAX_HEXSZ], मार्गः[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	प्रत्यागम do_change_note_fanout(मूलम्, मूलम्, hex_oid, 0, मार्गः, 0, fanout);
}

स्थिर पूर्णाङ्क parse_mapped_oid_hex(ध्रुव वर्ण *hex, संरचना object_id *oid, ध्रुव वर्ण **अन्तः)
{
	पूर्णाङ्क algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	स्मृतिं_पूरय(oid->hash, 0, परिमाण(oid->hash));

	algo = parse_oid_hex_any(hex, oid, अन्तः);
	यदि (algo == GIT_HASH_UNKNOWN)
		प्रत्यागम -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	यदि (it == kh_अन्तः(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		यदि (hash_algos[algo].format_id == the_hash_algo->format_id)
			प्रत्यागम 0;
		प्रत्यागम -1;
	}
	oidcpy(oid, kh_मानम्(sub_oid_map, it));
	प्रत्यागम 0;
}

/*
 * Given a pointer into a string, parse a mark reference:
 *
 *   idnum ::= ':' bigint;
 *
 * Update *endptr to point to the first character after the value.
 *
 * Complain if the following character is not what is expected,
 * either a space or end of the string.
 */
स्थिर uintmax_t parse_mark_ref(ध्रुव वर्ण *p, वर्ण **endptr)
{
	uintmax_t mark;

	प्रतिज्ञा(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	यदि (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	प्रत्यागम mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
स्थिर uintmax_t parse_mark_ref_eol(ध्रुव वर्ण *p)
{
	वर्ण *अन्तः;
	uintmax_t mark;

	mark = parse_mark_ref(p, &अन्तः);
	यदि (*अन्तः != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	प्रत्यागम mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
स्थिर uintmax_t parse_mark_ref_space(ध्रुव वर्ण **p)
{
	uintmax_t mark;
	वर्ण *अन्तः;

	mark = parse_mark_ref(*p, &अन्तः);
	यदि (*अन्तः++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = अन्तः;
	प्रत्यागम mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
स्थिर शून्य parse_मार्गः(संरचना strbuf *sb, ध्रुव वर्ण *p, ध्रुव वर्ण **endp,
		पूर्णाङ्क is_चरमः_क्षेत्रम्, ध्रुव वर्ण *क्षेत्रम्)
{
	यदि (*p == '"') {
		यदि (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), क्षेत्रम्, command_buf.buf);
		यदि (वाक्यदैर्घ्यम्(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), क्षेत्रम्, command_buf.buf);
	} अन्यथा {
		/*
		 * Unless we are parsing the last field of a line,
		 * SP is the end of this field.
		 */
		*endp = is_चरमः_क्षेत्रम्
			? p + वाक्यदैर्घ्यम्(p)
			: strchrnul(p, ' ');
		strbuf_सङ्कलय(sb, p, *endp - p);
	}
}

/*
 * Parse the path string into the strbuf, and complain if this is not the end of
 * the string. Unquoted strings may contain spaces.
 */
स्थिर शून्य parse_मार्गः_eol(संरचना strbuf *sb, ध्रुव वर्ण *p, ध्रुव वर्ण *क्षेत्रम्)
{
	ध्रुव वर्ण *अन्तः;

	parse_मार्गः(sb, p, &अन्तः, 1, क्षेत्रम्);
	यदि (*अन्तः)
		die(_("garbage after %s: %s"), क्षेत्रम्, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
स्थिर शून्य parse_मार्गः_space(संरचना strbuf *sb, ध्रुव वर्ण *p,
		ध्रुव वर्ण **endp, ध्रुव वर्ण *क्षेत्रम्)
{
	parse_मार्गः(sb, p, endp, 0, क्षेत्रम्);
	यदि (**endp != ' ')
		die(_("missing space after %s: %s"), क्षेत्रम्, command_buf.buf);
	(*endp)++;
}

स्थिर शून्य file_change_m(संरचना fast_import_state *state,
			  ध्रुव वर्ण *p,
			  संरचना branch *b)
{
	स्थिर संरचना strbuf मार्गः = STRBUF_INIT;
	संरचना object_प्रविष्टिः *oe;
	संरचना object_id oid;
	uint16_t बहुलकम्, inline_दत्त = 0;

	p = parse_बहुलकम्(p, &बहुलकम्);
	यदि (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	विकल्प (बहुलकम्) {
	पक्ष 0644:
	पक्ष 0755:
		बहुलकम् |= S_IFREG;
	पक्ष S_IFREG | 0644:
	पक्ष S_IFREG | 0755:
	पक्ष S_IFLNK:
	पक्ष S_IFDIR:
	पक्ष S_IFGITLINK:
		/* ok */
		भङ्ग;
	स्वभाव:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	यदि (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा यदि (skip_उपसर्गः(p, "inline ", &p)) {
		inline_दत्त = 1;
		oe = रिक्त; /* not used with inline_data, but makes gcc happy */
	} अन्यथा {
		यदि (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		यदि (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&मार्गः);
	parse_मार्गः_eol(&मार्गः, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	यदि (S_ISDIR(बहुलकम्) &&
	    is_रिक्तानि_वृक्षः_oid(&oid, the_repository->hash_algo) &&
	    *मार्गः.buf) {
		वृक्षः_content_remove(&b->branch_वृक्षः, मार्गः.buf, रिक्त, 0);
		प्रत्यागम;
	}

	यदि (S_ISGITLINK(बहुलकम्)) {
		यदि (inline_दत्त)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		अन्यथा यदि (oe) {
			यदि (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} अन्यथा यदि (inline_दत्त) {
		यदि (S_ISDIR(बहुलकम्))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		यावत् (read_अग्र_command(state) != EOF) {
			ध्रुव वर्ण *v;
			यदि (skip_उपसर्गः(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			अन्यथा {
				parse_and_store_blob(&चरमः_blob, &oid, 0);
				भङ्ग;
			}
		}
	} अन्यथा {
		गणना object_type expected = S_ISDIR(बहुलकम्) ?
						OBJ_वृक्षः: OBJ_BLOB;
		गणना object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, रिक्त);
		यदि (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(बहुलकम्) ?  _("tree") : _("blob"),
			    command_buf.buf);
		यदि (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	यदि (!*मार्गः.buf) {
		वृक्षः_content_replace(&b->branch_वृक्षः, &oid, बहुलकम्, रिक्त);
		प्रत्यागम;
	}

	यदि (!verify_मार्गः(मार्गः.buf, बहुलकम्))
		die(_("invalid path '%s'"), मार्गः.buf);
	वृक्षः_content_समुच्चयः(&b->branch_वृक्षः, मार्गः.buf, &oid, बहुलकम्, रिक्त);
}

स्थिर शून्य file_change_d(ध्रुव वर्ण *p, संरचना branch *b)
{
	स्थिर संरचना strbuf मार्गः = STRBUF_INIT;

	strbuf_reset(&मार्गः);
	parse_मार्गः_eol(&मार्गः, p, "path");
	वृक्षः_content_remove(&b->branch_वृक्षः, मार्गः.buf, रिक्त, 1);
}

स्थिर शून्य file_change_cr(ध्रुव वर्ण *p, संरचना branch *b, पूर्णाङ्क rename)
{
	स्थिर संरचना strbuf source = STRBUF_INIT;
	स्थिर संरचना strbuf dest = STRBUF_INIT;
	संरचना वृक्षः_प्रविष्टिः पर्णम्;

	strbuf_reset(&source);
	parse_मार्गः_space(&source, p, &p, "source");
	strbuf_reset(&dest);
	parse_मार्गः_eol(&dest, p, "dest");

	स्मृतिं_पूरय(&पर्णम्, 0, परिमाण(पर्णम्));
	यदि (rename)
		वृक्षः_content_remove(&b->branch_वृक्षः, source.buf, &पर्णम्, 1);
	अन्यथा
		वृक्षः_content_get(&b->branch_वृक्षः, source.buf, &पर्णम्, 1);
	यदि (!पर्णम्.versions[1].बहुलकम्)
		die(_("path %s not in branch"), source.buf);
	यदि (!*dest.buf) {	/* C "path/to/subdir" "" */
		वृक्षः_content_replace(&b->branch_वृक्षः,
			&पर्णम्.versions[1].oid,
			पर्णम्.versions[1].बहुलकम्,
			पर्णम्.वृक्षः);
		प्रत्यागम;
	}
	यदि (!verify_मार्गः(dest.buf, पर्णम्.versions[1].बहुलकम्))
		die(_("invalid path '%s'"), dest.buf);
	वृक्षः_content_समुच्चयः(&b->branch_वृक्षः, dest.buf,
		&पर्णम्.versions[1].oid,
		पर्णम्.versions[1].बहुलकम्,
		पर्णम्.वृक्षः);
}

स्थिर शून्य note_change_n(संरचना fast_import_state *state,
			  ध्रुव वर्ण *p,
			  संरचना branch *b,
			  अचिह्नित वर्ण *old_fanout)
{
	संरचना object_प्रविष्टिः *oe;
	संरचना branch *s;
	संरचना object_id oid, commit_oid;
	वर्ण मार्गः[GIT_MAX_RAWSZ * 3];
	uint16_t inline_दत्त = 0;
	अचिह्नित वर्ण new_fanout;

	/*
	 * When loading a branch, we don't traverse its tree to count the real
	 * number of notes (too expensive to do this for all non-note refs).
	 * This means that recently loaded notes refs might incorrectly have
	 * b->num_notes == 0, and consequently, old_fanout might be wrong.
	 *
	 * Fix this by traversing the tree and counting the number of notes
	 * when b->num_notes == 0. If the notes tree is truly empty, the
	 * calculation should not take long.
	 */
	यदि (b->अङ्कः_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->अङ्कः_notes = change_note_fanout(&b->branch_वृक्षः, 0xff);
		*old_fanout = convert_अङ्कः_notes_गन्तव्यम्_fanout(b->अङ्कः_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	यदि (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा यदि (skip_उपसर्गः(p, "inline ", &p)) {
		inline_दत्त = 1;
		oe = रिक्त; /* not used with inline_data, but makes gcc happy */
	} अन्यथा {
		यदि (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = find_object(&oid);
		यदि (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	यदि (s) {
		यदि (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} अन्यथा यदि (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		संरचना object_प्रविष्टिः *commit_oe = find_mark(marks, commit_mark);
		यदि (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} अन्यथा यदि (!repo_get_oid(the_repository, p, &commit_oid)) {
		आकार_t आकार;
		वर्ण *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &आकार,
						   &commit_oid);
		यदि (!buf || आकार < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		मुक्त(buf);
	} अन्यथा
		die(_("invalid ref name or SHA1 expression: %s"), p);

	यदि (inline_दत्त) {
		read_अग्र_command(state);
		parse_and_store_blob(&चरमः_blob, &oid, 0);
	} अन्यथा यदि (oe) {
		यदि (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} अन्यथा यदि (!is_null_oid(&oid)) {
		गणना object_type type = odb_read_object_info(the_repository->objects, &oid,
							रिक्त);
		यदि (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		यदि (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_मार्गः_with_fanout(oid_गन्तव्यम्_hex(&commit_oid), *old_fanout, मार्गः);
	यदि (वृक्षः_content_remove(&b->branch_वृक्षः, मार्गः, रिक्त, 0))
		b->अङ्कः_notes--;

	यदि (is_null_oid(&oid))
		प्रत्यागम; /* nothing to insert */

	b->अङ्कः_notes++;
	new_fanout = convert_अङ्कः_notes_गन्तव्यम्_fanout(b->अङ्कः_notes);
	construct_मार्गः_with_fanout(oid_गन्तव्यम्_hex(&commit_oid), new_fanout, मार्गः);
	वृक्षः_content_समुच्चयः(&b->branch_वृक्षः, मार्गः, &oid, S_IFREG | 0644, रिक्त);
}

स्थिर शून्य file_change_deleteall(संरचना branch *b)
{
	release_वृक्षः_content_recursive(b->branch_वृक्षः.वृक्षः);
	oidclr(&b->branch_वृक्षः.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_वृक्षः.versions[1].oid, the_repository->hash_algo);
	आरोपय_वृक्षः(&b->branch_वृक्षः);
	b->अङ्कः_notes = 0;
}

स्थिर शून्य parse_from_commit(संरचना branch *b, वर्ण *buf, अचिह्नित दीर्घ आकार)
{
	यदि (!buf || आकार < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_गन्तव्यम्_hex(&b->oid));
	यदि (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_वृक्षः.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_गन्तव्यम्_hex(&b->oid));
	oidcpy(&b->branch_वृक्षः.versions[0].oid,
	       &b->branch_वृक्षः.versions[1].oid);
}

स्थिर शून्य parse_from_existing(संरचना branch *b)
{
	यदि (is_null_oid(&b->oid)) {
		oidclr(&b->branch_वृक्षः.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_वृक्षः.versions[1].oid, the_repository->hash_algo);
	} अन्यथा {
		अचिह्नित दीर्घ आकार;
		आकार_t आकार_st = 0;
		वर्ण *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &आकार_st, &b->oid);
		आकार = cast_आकार_t_गन्तव्यम्_ulong(आकार_st);
		parse_from_commit(b, buf, आकार);
		मुक्त(buf);
	}
}

स्थिर पूर्णाङ्क parse_objectish(संरचना fast_import_state *state,
			   संरचना branch *b,
			   ध्रुव वर्ण *objectish)
{
	संरचना branch *s;
	संरचना object_id oid;

	oidcpy(&oid, &b->branch_वृक्षः.versions[1].oid);

	s = lookup_branch(objectish);
	यदि (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	अन्यथा यदि (s) {
		संरचना object_id *t = &s->branch_वृक्षः.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_वृक्षः.versions[0].oid, t);
		oidcpy(&b->branch_वृक्षः.versions[1].oid, t);
	} अन्यथा यदि (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		संरचना object_प्रविष्टिः *oe = find_mark(marks, idnum);
		यदि (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		यदि (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			यदि (oe->pack_id != MAX_PACK_ID) {
				अचिह्नित दीर्घ आकार;
				वर्ण *buf = gfi_unpack_प्रविष्टिः(oe, &आकार);
				parse_from_commit(b, buf, आकार);
				मुक्त(buf);
			} अन्यथा
				parse_from_existing(b);
		}
	} अन्यथा यदि (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		यदि (is_null_oid(&b->oid))
			b->delete = 1;
	}
	अन्यथा
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	यदि (b->branch_वृक्षः.वृक्षः && !oideq(&oid, &b->branch_वृक्षः.versions[1].oid)) {
		release_वृक्षः_content_recursive(b->branch_वृक्षः.वृक्षः);
		b->branch_वृक्षः.वृक्षः = रिक्त;
	}

	read_अग्र_command(state);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क parse_from(संरचना fast_import_state *state, संरचना branch *b)
{
	ध्रुव वर्ण *from;

	यदि (!skip_उपसर्गः(command_buf.buf, "from ", &from))
		प्रत्यागम 0;

	प्रत्यागम parse_objectish(state, b, from);
}

स्थिर पूर्णाङ्क parse_objectish_with_उपसर्गः(संरचना fast_import_state *state,
				       संरचना branch *b,
				       ध्रुव वर्ण *उपसर्गः)
{
	ध्रुव वर्ण *आधारः;

	यदि (!skip_उपसर्गः(command_buf.buf, उपसर्गः, &आधारः))
		प्रत्यागम 0;

	प्रत्यागम parse_objectish(state, b, आधारः);
}

स्थिर संरचना hash_list *parse_merge(संरचना fast_import_state *state,
				     अचिह्नित पूर्णाङ्क *गणनम्)
{
	संरचना hash_list *list = रिक्त, **पुच्छ = &list, *n;
	ध्रुव वर्ण *from;
	संरचना branch *s;

	*गणनम् = 0;
	यावत् (skip_उपसर्गः(command_buf.buf, "merge ", &from)) {
		n = xmalloc(परिमाण(*n));
		s = lookup_branch(from);
		यदि (s)
			oidcpy(&n->oid, &s->oid);
		अन्यथा यदि (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			संरचना object_प्रविष्टिः *oe = find_mark(marks, idnum);
			यदि (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} अन्यथा यदि (!repo_get_oid(the_repository, from, &n->oid)) {
			आकार_t आकार;
			वर्ण *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &आकार, &n->oid);
			यदि (!buf || आकार < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			मुक्त(buf);
		} अन्यथा
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->अग्र = रिक्त;
		*पुच्छ = n;
		पुच्छ = &n->अग्र;

		(*गणनम्)++;
		read_अग्र_command(state);
	}
	प्रत्यागम list;
}

संरचना signature_दत्त {
	वर्ण *hash_algo;      /* "sha1" or "sha256" */
	वर्ण *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	संरचना strbuf दत्त;   /* The actual signature data */
};

स्थिर शून्य parse_one_signature(संरचना fast_import_state *state,
				संरचना signature_दत्त *sig,
				ध्रुव वर्ण *v)
{
	वर्ण *तर्काः = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	वर्ण *space = strchr(तर्काः, ' ');

	यदि (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), तर्काः);
	*space = '\0';

	sig->hash_algo = तर्काः;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	यदि (वाक्यं_तुलय(sig->hash_algo, "sha1") &&
	    वाक्यं_तुलय(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	यदि (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	यदि (!वाक्यं_तुलय(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_अग्र_command(state);
	parse_दत्त(&sig->दत्त, 0, रिक्त);
}

स्थिर शून्य discard_one_signature(संरचना fast_import_state *state)
{
	संरचना strbuf दत्त = STRBUF_INIT;

	read_अग्र_command(state);
	parse_दत्त(&दत्त, 0, रिक्त);
	strbuf_release(&दत्त);
}

स्थिर शून्य सङ्कलय_gpgsig_गन्तव्यम्_commit(संरचना strbuf *commit_दत्त,
				 ध्रुव वर्ण *शीर्षकम्,
				 संरचना signature_दत्त *sig)
{
	संरचना string_list siglines = STRING_LIST_INIT_NODUP;

	यदि (!sig || !sig->hash_algo)
		प्रत्यागम;

	strbuf_addstr(commit_दत्त, शीर्षकम्);
	string_list_विभज_अन्तर्_place(&siglines, sig->दत्त.buf, "\n", -1);
	strbuf_सङ्कलय_separated_string_list(commit_दत्त, "\n ", &siglines);
	strbuf_addch(commit_दत्त, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->दत्त);
	मुक्त(sig->hash_algo);
}

स्थिर शून्य store_signature(संरचना signature_दत्त *stored_sig,
			    संरचना signature_दत्त *new_sig,
			    ध्रुव वर्ण *hash_type)
{
	यदि (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->दत्त);
		मुक्त(new_sig->hash_algo);
	} अन्यथा {
		*stored_sig = *new_sig;
	}
}

स्थिर शून्य import_one_signature(संरचना fast_import_state *state,
				 संरचना signature_दत्त *sig_sha1,
				 संरचना signature_दत्त *sig_sha256,
				 ध्रुव वर्ण *v)
{
	संरचना signature_दत्त sig = { रिक्त, रिक्त, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	यदि (!वाक्यं_तुलय(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	अन्यथा यदि (!वाक्यं_तुलय(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	अन्यथा
		die(_("parse_one_signature() returned unknown hash algo"));
}

स्थिर शून्य finalize_commit_बफर(संरचना strbuf *new_दत्त,
				   संरचना signature_दत्त *sig_sha1,
				   संरचना signature_दत्त *sig_sha256,
				   संरचना strbuf *सन्देशः)
{
	सङ्कलय_gpgsig_गन्तव्यम्_commit(new_दत्त, "gpgsig ", sig_sha1);
	सङ्कलय_gpgsig_गन्तव्यम्_commit(new_दत्त, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_दत्त, '\n');
	strbuf_addbuf(new_दत्त, सन्देशः);
}

स्थिर शून्य warn_invalid_signature(संरचना signature_check *check,
				   ध्रुव वर्ण *सन्देशः, गणना sign_बहुलकम् बहुलकम्)
{
	ध्रुव वर्ण *signer = check->signer ? check->signer : _("unknown");
	ध्रुव वर्ण *subject;
	पूर्णाङ्क subject_len = find_commit_subject(सन्देशः, &subject);

	विकल्प (बहुलकम्) {
	पक्ष SIGN_STRIP_IF_INVALID:
		यदि (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		अन्यथा यदि (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		अन्यथा
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		भङ्ग;
	पक्ष SIGN_SIGN_IF_INVALID:
		यदि (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		अन्यथा यदि (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		अन्यथा
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		भङ्ग;
	स्वभाव:
		BUG("unsupported signing mode");
	}
}

स्थिर शून्य handle_signature_if_invalid(संरचना strbuf *new_दत्त,
					संरचना signature_दत्त *sig_sha1,
					संरचना signature_दत्त *sig_sha256,
					संरचना strbuf *सन्देशः,
					गणना sign_बहुलकम् बहुलकम्)
{
	संरचना strbuf tmp_buf = STRBUF_INIT;
	संरचना signature_check signature_check = { 0 };
	पूर्णाङ्क ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_दत्त);
	finalize_commit_बफर(&tmp_buf, sig_sha1, sig_sha256, सन्देशः);
	ret = verify_commit_बफर(tmp_buf.buf, tmp_buf.len, &signature_check);

	यदि (ret) {
		यदि (बहुलकम् == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, सन्देशः->buf, बहुलकम्);

		यदि (बहुलकम् == SIGN_SIGN_IF_INVALID) {
			संरचना strbuf signature = STRBUF_INIT;
			संरचना strbuf payload = STRBUF_INIT;

			/*
			 * NEEDSWORK: To properly support interoperability mode
			 * when signing commit signatures, the commit buffer
			 * must be provided in both the repository and
			 * compatibility object formats. As currently
			 * implemented, only the repository object format is
			 * considered meaning compatibility signatures cannot be
			 * generated. Thus, attempting to sign commit signatures
			 * in interoperability mode is currently unsupported.
			 */
			यदि (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			यदि (sign_बफर(&payload, &signature, signed_commit_keyid,
					SIGN_बफर_USE_DEFAULT_कुञ्चिका))
				die(_("failed to sign commit object"));
			सङ्कलय_शीर्षकम्_signature(new_दत्त, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_बफर(new_दत्त, रिक्त, रिक्त, सन्देशः);
	} अन्यथा {
		strbuf_swap(new_दत्त, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

स्थिर शून्य parse_new_commit(संरचना fast_import_state *state, ध्रुव वर्ण *arg)
{
	स्थिर संरचना strbuf सन्देशः = STRBUF_INIT;
	संरचना signature_दत्त sig_sha1 = { रिक्त, रिक्त, STRBUF_INIT };
	संरचना signature_दत्त sig_sha256 = { रिक्त, रिक्त, STRBUF_INIT };
	संरचना branch *b;
	वर्ण *author = रिक्त;
	वर्ण *committer = रिक्त;
	वर्ण *encoding = रिक्त;
	संरचना hash_list *merge_list = रिक्त;
	अचिह्नित पूर्णाङ्क merge_गणनम्;
	अचिह्नित वर्ण पूर्व_fanout, new_fanout;
	ध्रुव वर्ण *v;

	b = lookup_branch(arg);
	यदि (!b)
		b = new_branch(arg);

	read_अग्र_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	यदि (skip_उपसर्गः(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_अग्र_command(state);
	}
	यदि (skip_उपसर्गः(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_अग्र_command(state);
	}
	यदि (!committer)
		die(_("expected committer but didn't get one"));

	यावत् (skip_उपसर्गः(command_buf.buf, "gpgsig ", &v)) {
		विकल्प (signed_commit_बहुलकम्) {

		/* First, modes that don't need the signature to be parsed */
		पक्ष SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		पक्ष SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		पक्ष SIGN_STRIP:
			discard_one_signature(state);
			भङ्ग;

		/* Second, modes that parse the signature */
		पक्ष SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		पक्ष SIGN_VERBATIM:
		पक्ष SIGN_STRIP_IF_INVALID:
		पक्ष SIGN_SIGN_IF_INVALID:
		पक्ष SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			भङ्ग;

		/* Third, BUG */
		स्वभाव:
			BUG("invalid signed_commit_mode value %d", signed_commit_बहुलकम्);
		}
		read_अग्र_command(state);
	}

	यदि (skip_उपसर्गः(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_अग्र_command(state);
	}
	parse_दत्त(&सन्देशः, 0, रिक्त);
	read_अग्र_command(state);
	parse_from(state, b);
	merge_list = parse_merge(state, &merge_गणनम्);

	/* ensure the branch is active/loaded */
	यदि (!b->branch_वृक्षः.वृक्षः || !max_सक्रिय_branches) {
		unload_one_branch();
		आरोपय_branch(b);
	}

	पूर्व_fanout = convert_अङ्कः_notes_गन्तव्यम्_fanout(b->अङ्कः_notes);

	/* file_change* */
	यावत् (command_buf.len > 0) {
		यदि (skip_उपसर्गः(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &पूर्व_fanout);
		अन्यथा यदि (!वाक्यं_तुलय("deleteall", command_buf.buf))
			file_change_deleteall(b);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		अन्यथा {
			unread_command_buf = 1;
			भङ्ग;
		}
		यदि (read_अग्र_command(state) == EOF)
			भङ्ग;
	}

	new_fanout = convert_अङ्कः_notes_गन्तव्यम्_fanout(b->अङ्कः_notes);
	यदि (new_fanout != पूर्व_fanout)
		b->अङ्कः_notes = change_note_fanout(&b->branch_वृक्षः, new_fanout);

	/* build the tree and the commit */
	store_वृक्षः(&b->branch_वृक्षः);
	oidcpy(&b->branch_वृक्षः.versions[0].oid,
	       &b->branch_वृक्षः.versions[1].oid);

	strbuf_reset(&new_दत्त);
	strbuf_addf(&new_दत्त, "tree %s\n",
		oid_गन्तव्यम्_hex(&b->branch_वृक्षः.versions[1].oid));
	यदि (!is_null_oid(&b->oid))
		strbuf_addf(&new_दत्त, "parent %s\n",
			    oid_गन्तव्यम्_hex(&b->oid));
	यावत् (merge_list) {
		संरचना hash_list *अग्र = merge_list->अग्र;
		strbuf_addf(&new_दत्त, "parent %s\n",
			    oid_गन्तव्यम्_hex(&merge_list->oid));
		मुक्त(merge_list);
		merge_list = अग्र;
	}
	strbuf_addf(&new_दत्त,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	यदि (encoding)
		strbuf_addf(&new_दत्त,
			"encoding %s\n",
			encoding);

	यदि ((signed_commit_बहुलकम् == SIGN_STRIP_IF_INVALID ||
	     signed_commit_बहुलकम् == SIGN_SIGN_IF_INVALID ||
	     signed_commit_बहुलकम् == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_दत्त, &sig_sha1, &sig_sha256,
					    &सन्देशः, signed_commit_बहुलकम्);
	अन्यथा
		finalize_commit_बफर(&new_दत्त, &sig_sha1, &sig_sha256, &सन्देशः);

	मुक्त(author);
	मुक्त(committer);
	मुक्त(encoding);

	यदि (!store_object(OBJ_COMMIT, &new_दत्त, रिक्त, &b->oid, अग्र_mark))
		b->pack_id = pack_id;
	b->चरमः_commit = object_गणनम्_by_type[OBJ_COMMIT];
}

स्थिर शून्य handle_चिह्नकम्_signature_if_invalid(संरचना strbuf *buf,
					    संरचना strbuf *सन्देशः,
					    आकार_t sig_विस्थापनम्)
{
	संरचना strbuf signature = STRBUF_INIT;
	संरचना strbuf payload = STRBUF_INIT;
	संरचना signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_सङ्कलय(&payload, सन्देशः->buf, sig_विस्थापनम्);
	strbuf_सङ्कलय(&signature, सन्देशः->buf + sig_विस्थापनम्, सन्देशः->len - sig_विस्थापनम्);

	sigc.payload_type = SIGNATURE_PAYLOAD_चिह्नकम्;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	यदि (!check_signature(&sigc, signature.buf, signature.len))
		गच्छ बहिर्गमनम्;

	यदि (signed_चिह्नकम्_बहुलकम् == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(सन्देशः, sig_विस्थापनम्);

	यदि (signed_चिह्नकम्_बहुलकम् == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = रिक्त;
		strbuf_reset(&signature);

		यदि (sign_बफर(&payload, &signature, signed_चिह्नकम्_keyid,
				SIGN_बफर_USE_DEFAULT_कुञ्चिका))
			die(_("failed to sign tag object"));

		strbuf_addbuf(सन्देशः, &signature);
	}

बहिर्गमनम्:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

स्थिर शून्य handle_चिह्नकम्_signature(संरचना strbuf *buf, संरचना strbuf *सन्देशः, ध्रुव वर्ण *name)
{
	आकार_t sig_विस्थापनम् = parse_signed_बफर(सन्देशः->buf, सन्देशः->len);

	/* If there is no signature, there is nothing to do. */
	यदि (sig_विस्थापनम् >= सन्देशः->len)
		प्रत्यागम;

	विकल्प (signed_चिह्नकम्_बहुलकम्) {

	/* First, modes that don't change anything */
	पक्ष SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	पक्ष SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		भङ्ग;

	/* Second, modes that remove the signature */
	पक्ष SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	पक्ष SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(सन्देशः, sig_विस्थापनम्);
		भङ्ग;
	पक्ष SIGN_ABORT_IF_INVALID:
	पक्ष SIGN_SIGN_IF_INVALID:
	पक्ष SIGN_STRIP_IF_INVALID:
		handle_चिह्नकम्_signature_if_invalid(buf, सन्देशः, sig_विस्थापनम्);
		भङ्ग;

	/* Third, aborting modes */
	पक्ष SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	स्वभाव:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_चिह्नकम्_बहुलकम्, name);
	}
}

स्थिर शून्य parse_new_चिह्नकम्(संरचना fast_import_state *state, ध्रुव वर्ण *arg)
{
	स्थिर संरचना strbuf सन्देशः = STRBUF_INIT;
	ध्रुव वर्ण *from;
	वर्ण *tagger;
	संरचना branch *s;
	संरचना चिह्नकम् *t;
	uintmax_t from_mark = 0;
	संरचना object_id oid;
	गणना object_type type;
	ध्रुव वर्ण *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, परिमाण(संरचना चिह्नकम्));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	यदि (चरमः_चिह्नकम्)
		चरमः_चिह्नकम्->अग्र_चिह्नकम् = t;
	अन्यथा
		first_चिह्नकम् = t;
	चरमः_चिह्नकम् = t;
	read_अग्र_command(state);
	parse_mark(state);

	/* from ... */
	यदि (!skip_उपसर्गः(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	यदि (s) {
		यदि (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} अन्यथा यदि (*from == ':') {
		संरचना object_प्रविष्टिः *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = find_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा यदि (!repo_get_oid(the_repository, from, &oid)) {
		संरचना object_प्रविष्टिः *oe = find_object(&oid);
		यदि (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, रिक्त);
			यदि (type < 0)
				die(_("not a valid object: %s"), from);
		} अन्यथा
			type = oe->type;
	} अन्यथा
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_अग्र_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	यदि (skip_उपसर्गः(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_अग्र_command(state);
	} अन्यथा
		tagger = रिक्त;

	/* tag payload/message */
	parse_दत्त(&सन्देशः, 0, रिक्त);

	/* build the tag object */
	strbuf_reset(&new_दत्त);

	strbuf_addf(&new_दत्त,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_गन्तव्यम्_hex(&oid), type_name(type), t->name);
	यदि (tagger)
		strbuf_addf(&new_दत्त,
			    "tagger %s\n", tagger);

	handle_चिह्नकम्_signature(&new_दत्त, &सन्देशः, t->name);

	strbuf_addch(&new_दत्त, '\n');
	strbuf_addbuf(&new_दत्त, &सन्देशः);
	मुक्त(tagger);

	यदि (store_object(OBJ_चिह्नकम्, &new_दत्त, रिक्त, &t->oid, अग्र_mark))
		t->pack_id = MAX_PACK_ID;
	अन्यथा
		t->pack_id = pack_id;
}

स्थिर शून्य parse_reset_branch(संरचना fast_import_state *state, ध्रुव वर्ण *arg)
{
	संरचना branch *b;
	ध्रुव वर्ण *चिह्नकम्_name;

	b = lookup_branch(arg);
	यदि (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_वृक्षः.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_वृक्षः.versions[1].oid, the_repository->hash_algo);
		यदि (b->branch_वृक्षः.वृक्षः) {
			release_वृक्षः_content_recursive(b->branch_वृक्षः.वृक्षः);
			b->branch_वृक्षः.वृक्षः = रिक्त;
		}
	}
	अन्यथा
		b = new_branch(arg);
	read_अग्र_command(state);
	parse_from(state, b);
	यदि (b->delete && skip_उपसर्गः(b->name, "refs/tags/", &चिह्नकम्_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		संरचना चिह्नकम् *t, *पूर्व = रिक्त;
		प्रति (t = first_चिह्नकम्; t; t = t->अग्र_चिह्नकम्) {
			यदि (!वाक्यं_तुलय(t->name, चिह्नकम्_name))
				भङ्ग;
			पूर्व = t;
		}
		यदि (t) {
			यदि (पूर्व)
				पूर्व->अग्र_चिह्नकम् = t->अग्र_चिह्नकम्;
			अन्यथा
				first_चिह्नकम् = t->अग्र_चिह्नकम्;
			यदि (!t->अग्र_चिह्नकम्)
				चरमः_चिह्नकम् = पूर्व;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	यदि (command_buf.len > 0)
		unread_command_buf = 1;
}

स्थिर शून्य cat_blob_write(ध्रुव वर्ण *buf, अचिह्नित दीर्घ आकार)
{
	यदि (write_अन्तर्_पूर्णम्(cat_blob_fd, buf, आकार) < 0)
		die_errno(_("write to frontend failed"));
}

स्थिर शून्य cat_blob(संरचना object_प्रविष्टिः *oe, संरचना object_id *oid)
{
	संरचना strbuf पङ्क्तिः = STRBUF_INIT;
	संरचना iovec iov[3];
	अचिह्नित दीर्घ आकार;
	गणना object_type type = 0;
	वर्ण *buf;

	यदि (!oe || oe->pack_id == MAX_PACK_ID) {
		आकार_t आकार_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &आकार_st);
		आकार = cast_आकार_t_गन्तव्यम्_ulong(आकार_st);
	} अन्यथा {
		type = oe->type;
		buf = gfi_unpack_प्रविष्टिः(oe, &आकार);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	यदि (type <= 0) {
		strbuf_reset(&पङ्क्तिः);
		strbuf_addf(&पङ्क्तिः, "%s missing\n", oid_गन्तव्यम्_hex(oid));
		cat_blob_write(पङ्क्तिः.buf, पङ्क्तिः.len);
		strbuf_release(&पङ्क्तिः);
		मुक्त(buf);
		प्रत्यागम;
	}
	यदि (!buf)
		die(_("can't read object %s"), oid_गन्तव्यम्_hex(oid));
	यदि (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_गन्तव्यम्_hex(oid), type_name(type));
	strbuf_reset(&पङ्क्तिः);
	strbuf_addf(&पङ्क्तिः, "%s %s %"PRIuMAX"\n", oid_गन्तव्यम्_hex(oid),
		    type_name(type), (uintmax_t)आकार);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_आधारः = पङ्क्तिः.buf;
	iov[0].iov_len = पङ्क्तिः.len;
	iov[1].iov_आधारः = buf;
	iov[1].iov_len = आकार;
	iov[2].iov_आधारः = (शून्य *) "\n";
	iov[2].iov_len = 1;

	यदि (writev_अन्तर्_पूर्णम्(cat_blob_fd, iov, ARRAY_आकार(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&पङ्क्तिः);
	यदि (oe && oe->pack_id == pack_id) {
		चरमः_blob.विस्थापनम् = oe->idx.विस्थापनम्;
		strbuf_attach(&चरमः_blob.दत्त, buf, आकार, आकार + 1);
		चरमः_blob.गभीरता = oe->गभीरता;
	} अन्यथा
		मुक्त(buf);
}

स्थिर शून्य parse_get_mark(संरचना fast_import_state *state अप्रयुक्तम्,
			   ध्रुव वर्ण *p)
{
	संरचना object_प्रविष्टिः *oe;
	वर्ण output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	यदि (*p != ':')
		die(_("not a mark: %s"), p);

	oe = find_mark(marks, parse_mark_ref_eol(p));
	यदि (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, परिमाण(output), "%s\n", oid_गन्तव्यम्_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

स्थिर शून्य parse_cat_blob(संरचना fast_import_state *state अप्रयुक्तम्,
			   ध्रुव वर्ण *p)
{
	संरचना object_प्रविष्टिः *oe;
	संरचना object_id oid;

	/* cat-blob SP <object> LF */
	यदि (*p == ':') {
		oe = find_mark(marks, parse_mark_ref_eol(p));
		यदि (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} अन्यथा {
		यदि (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		यदि (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = find_object(&oid);
	}

	cat_blob(oe, &oid);
}

स्थिर संरचना object_प्रविष्टिः *dereference(संरचना object_प्रविष्टिः *oe,
					संरचना object_id *oid)
{
	अचिह्नित दीर्घ आकार;
	वर्ण *buf = रिक्त;
	ध्रुव अचिह्नित hexsz = the_hash_algo->hexsz;

	यदि (!oe) {
		गणना object_type type = odb_read_object_info(the_repository->objects,
							     oid, रिक्त);
		यदि (type < 0)
			die(_("object not found: %s"), oid_गन्तव्यम्_hex(oid));
		/* cache it! */
		oe = निवेशय_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.विस्थापनम् = 1;
	}
	विकल्प (oe->type) {
	पक्ष OBJ_वृक्षः:	/* easy case. */
		प्रत्यागम oe;
	पक्ष OBJ_COMMIT:
	पक्ष OBJ_चिह्नकम्:
		भङ्ग;
	स्वभाव:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	यदि (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_प्रविष्टिः(oe, &आकार);
	} अन्यथा {
		गणना object_type अप्रयुक्तम्_2;
		आकार_t आकार_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &अप्रयुक्तम्_2, &आकार_st);
		आकार = cast_आकार_t_गन्तव्यम्_ulong(आकार_st);
	}
	यदि (!buf)
		die(_("can't load object %s"), oid_गन्तव्यम्_hex(oid));

	/* Peel one layer. */
	विकल्प (oe->type) {
	पक्ष OBJ_चिह्नकम्:
		यदि (आकार < hexsz + वाक्यदैर्घ्यम्("object ") ||
		    get_oid_hex(buf + वाक्यदैर्घ्यम्("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		भङ्ग;
	पक्ष OBJ_COMMIT:
		यदि (आकार < hexsz + वाक्यदैर्घ्यम्("tree ") ||
		    get_oid_hex(buf + वाक्यदैर्घ्यम्("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	मुक्त(buf);
	प्रत्यागम find_object(oid);
}

स्थिर शून्य निवेशय_mapped_mark(uintmax_t mark, शून्य *object, शून्य *cbp)
{
	संरचना object_id *fromoid = object;
	संरचना object_id *tooid = find_mark(cbp, mark);
	पूर्णाङ्क ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	यदि (ret == 0)
		प्रत्यागम;
	kh_मानम्(sub_oid_map, it) = tooid;
}

स्थिर शून्य build_mark_map_one(संरचना mark_समुच्चयः *from, संरचना mark_समुच्चयः *गन्तव्यम्)
{
	for_each_mark(from, 0, निवेशय_mapped_mark, गन्तव्यम्);
}

स्थिर शून्य build_mark_map(संरचना string_list *from, संरचना string_list *गन्तव्यम्)
{
	संरचना string_list_item *fromp, *शिखरम्;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_item(fromp, from) {
		शिखरम् = string_list_lookup(गन्तव्यम्, fromp->string);
		यदि (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} अन्यथा यदि (!शिखरम् || !शिखरम्->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, शिखरम्->util);
	}
}

स्थिर संरचना object_प्रविष्टिः *parse_treeish_dataref(ध्रुव वर्ण **p)
{
	संरचना object_id oid;
	संरचना object_प्रविष्टिः *e;

	यदि (**p == ':') {	/* <mark> */
		e = find_mark(marks, parse_mark_ref_space(p));
		यदि (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} अन्यथा {	/* <sha1> */
		यदि (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = find_object(&oid);
		यदि (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	यावत् (!e || e->type != OBJ_वृक्षः)
		e = dereference(e, &oid);
	प्रत्यागम e;
}

स्थिर शून्य print_ls(पूर्णाङ्क बहुलकम्, ध्रुव अचिह्नित वर्ण *hash, ध्रुव वर्ण *मार्गः)
{
	स्थिर संरचना strbuf पङ्क्तिः = STRBUF_INIT;

	/* See show_tree(). */
	ध्रुव वर्ण *type =
		S_ISGITLINK(बहुलकम्) ? commit_type :
		S_ISDIR(बहुलकम्) ? वृक्षः_type :
		blob_type;

	यदि (!बहुलकम्) {
		/* missing SP path LF */
		strbuf_reset(&पङ्क्तिः);
		strbuf_addstr(&पङ्क्तिः, "missing ");
		quote_c_style(मार्गः, &पङ्क्तिः, रिक्त, 0);
		strbuf_addch(&पङ्क्तिः, '\n');
	} अन्यथा {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&पङ्क्तिः);
		strbuf_addf(&पङ्क्तिः, "%06o %s %s\t",
				बहुलकम् & ~NO_DELTA, type, hash_गन्तव्यम्_hex(hash));
		quote_c_style(मार्गः, &पङ्क्तिः, रिक्त, 0);
		strbuf_addch(&पङ्क्तिः, '\n');
	}
	cat_blob_write(पङ्क्तिः.buf, पङ्क्तिः.len);
}

स्थिर शून्य parse_ls(संरचना fast_import_state *state अप्रयुक्तम्,
		     ध्रुव वर्ण *p,
		     संरचना branch *b)
{
	स्थिर संरचना strbuf मार्गः = STRBUF_INIT;
	संरचना वृक्षः_प्रविष्टिः *मूलम् = रिक्त;
	संरचना वृक्षः_प्रविष्टिः पर्णम् = {रिक्त};

	/* ls SP (<tree-ish> SP)? <path> */
	यदि (*p == '"') {
		यदि (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		मूलम् = &b->branch_वृक्षः;
	} अन्यथा {
		संरचना object_प्रविष्टिः *e = parse_treeish_dataref(&p);
		मूलम् = new_वृक्षः_प्रविष्टिः();
		oidcpy(&मूलम्->versions[1].oid, &e->idx.oid);
		यदि (!is_null_oid(&मूलम्->versions[1].oid))
			मूलम्->versions[1].बहुलकम् = S_IFDIR;
		आरोपय_वृक्षः(मूलम्);
	}
	strbuf_reset(&मार्गः);
	parse_मार्गः_eol(&मार्गः, p, "path");
	वृक्षः_content_get(मूलम्, मार्गः.buf, &पर्णम्, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	यदि (S_ISDIR(पर्णम्.versions[1].बहुलकम्))
		store_वृक्षः(&पर्णम्);

	print_ls(पर्णम्.versions[1].बहुलकम्, पर्णम्.versions[1].oid.hash, मार्गः.buf);
	यदि (पर्णम्.वृक्षः)
		release_वृक्षः_content_recursive(पर्णम्.वृक्षः);
	यदि (!b || मूलम् != &b->branch_वृक्षः)
		release_वृक्षः_प्रविष्टिः(मूलम्);
}

स्थिर शून्य checkpoint(शून्य)
{
	checkpoint_requested = 0;
	यदि (object_गणनम्) {
		आवृत्तिः_packfile();
	}
	dump_branches();
	dump_चिह्नकानि();
	dump_marks();
}

स्थिर शून्य parse_checkpoint(संरचना fast_import_state *state अप्रयुक्तम्)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

स्थिर शून्य parse_प्रगतिः(संरचना fast_import_state *state अप्रयुक्तम्)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

स्थिर शून्य parse_alias(संरचना fast_import_state *state)
{
	संरचना object_प्रविष्टिः *e;
	संरचना branch b;

	skip_optional_lf();
	read_अग्र_command(state);

	/* mark ... */
	parse_mark(state);
	यदि (!अग्र_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	स्मृतिं_पूरय(&b, 0, परिमाण(b));
	यदि (!parse_objectish_with_उपसर्गः(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = find_object(&b.oid);
	प्रतिज्ञा(e);
	निवेशय_mark(&marks, अग्र_mark, e);
}

स्थिर वर्ण* make_fast_import_मार्गः(संरचना fast_import_state *state,
				   ध्रुव वर्ण *मार्गः)
{
	यदि (!relative_marks_paths || is_absolute_मार्गः(मार्गः))
		प्रत्यागम उपसर्गः_filename(state->उपसर्गः, मार्गः);
	प्रत्यागम repo_git_मार्गः(the_repository, "info/fast-import/%s", मार्गः);
}

स्थिर शून्य option_import_marks(संरचना fast_import_state *state,
				ध्रुव वर्ण *marks,
				पूर्णाङ्क from_प्रवाहः,
				पूर्णाङ्क ignore_missing)
{
	यदि (import_marks_file) {
		यदि (from_प्रवाहः)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		यदि(!import_marks_file_from_प्रवाहः)
			read_marks();
	}

	मुक्त(import_marks_file);
	import_marks_file = make_fast_import_मार्गः(state, marks);
	import_marks_file_from_प्रवाहः = from_प्रवाहः;
	import_marks_file_ignore_missing = ignore_missing;
}

स्थिर शून्य option_date_format(ध्रुव वर्ण *fmt)
{
	यदि (!वाक्यं_तुलय(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	अन्यथा यदि (!वाक्यं_तुलय(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	अन्यथा यदि (!वाक्यं_तुलय(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	अन्यथा यदि (!वाक्यं_तुलय(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	अन्यथा
		die(_("unknown --date-format argument %s"), fmt);
}

स्थिर अचिह्नित दीर्घ ulong_arg(ध्रुव वर्ण *option, ध्रुव वर्ण *arg)
{
	वर्ण *endptr;
	अचिह्नित दीर्घ rv = strtoul(arg, &endptr, 0);
	यदि (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	प्रत्यागम rv;
}

स्थिर शून्य option_गभीरता(ध्रुव वर्ण *गभीरता)
{
	max_गभीरता = ulong_arg("--depth", गभीरता);
	यदि (max_गभीरता > MAX_गभीरता)
		die(_("--depth cannot exceed %u"), MAX_गभीरता);
}

स्थिर शून्य option_सक्रिय_branches(ध्रुव वर्ण *branches)
{
	max_सक्रिय_branches = ulong_arg("--active-branches", branches);
}

स्थिर शून्य option_export_marks(संरचना fast_import_state *state,
				ध्रुव वर्ण *marks)
{
	मुक्त(export_marks_file);
	export_marks_file = make_fast_import_मार्गः(state, marks);
}

स्थिर शून्य option_cat_blob_fd(संरचना fast_import_state *state अप्रयुक्तम्,
			       ध्रुव वर्ण *fd)
{
	अचिह्नित दीर्घ n = ulong_arg("--cat-blob-fd", fd);
	यदि (n > (अचिह्नित दीर्घ) अधिकतम_पूर्णाङ्कः)
		die(_("--cat-blob-fd cannot exceed %d"), अधिकतम_पूर्णाङ्कः);
	cat_blob_fd = (पूर्णाङ्क) n;
}

स्थिर शून्य option_export_pack_धाराः(संरचना fast_import_state *state,
				     ध्रुव वर्ण *धाराः)
{
	वर्ण *fn = उपसर्गः_filename(state->उपसर्गः, धाराः);
	यदि (pack_धाराः)
		fclose(pack_धाराः);
	pack_धाराः = xfopen(fn, "a");
	मुक्त(fn);
}

स्थिर शून्य option_rewrite_submodules(संरचना fast_import_state *state,
				      ध्रुव वर्ण *arg,
				      संरचना string_list *list)
{
	संरचना mark_समुच्चयः *ms;
	FILE *fp;
	वर्ण *s = xstrdup(arg);
	वर्ण *f = strchr(s, ':');
	यदि (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = उपसर्गः_filename(state->उपसर्गः, f);
	fp = fopen(f, "r");
	यदि (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, निवेशय_oid_प्रविष्टिः);
	fclose(fp);
	मुक्त(f);

	string_list_निवेशय(list, s)->util = ms;

	मुक्त(s);
}

स्थिर शून्य option_max_pack_आकार(ध्रुव वर्ण *arg)
{
	अचिह्नित दीर्घ v;

	यदि (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	यदि (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} अन्यथा यदि (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

स्थिर शून्य option_big_file_देहली(ध्रुव वर्ण *arg)
{
	अचिह्नित दीर्घ v;

	यदि (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_समुच्चयः_big_file_देहली(the_repository, v);
}

स्थिर शून्य option_signed_commits(ध्रुव वर्ण *arg)
{
	यदि (parse_sign_बहुलकम्(arg, &signed_commit_बहुलकम्, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

स्थिर शून्य option_signed_चिह्नकानि(ध्रुव वर्ण *arg)
{
	यदि (parse_sign_बहुलकम्(arg, &signed_चिह्नकम्_बहुलकम्, &signed_चिह्नकम्_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

स्थिर शून्य option_quiet(शून्य)
{
	show_stats = 0;
	quiet = 1;
}

स्थिर पूर्णाङ्क parse_one_option(संरचना fast_import_state *state, ध्रुव वर्ण *option)
{
	यदि (skip_उपसर्गः(option, "max-pack-size=", &option)) {
		option_max_pack_आकार(option);
	} अन्यथा यदि (skip_उपसर्गः(option, "big-file-threshold=", &option)) {
		option_big_file_देहली(option);
	} अन्यथा यदि (skip_उपसर्गः(option, "depth=", &option)) {
		option_गभीरता(option);
	} अन्यथा यदि (skip_उपसर्गः(option, "active-branches=", &option)) {
		option_सक्रिय_branches(option);
	} अन्यथा यदि (skip_उपसर्गः(option, "export-pack-edges=", &option)) {
		option_export_pack_धाराः(state, option);
	} अन्यथा यदि (skip_उपसर्गः(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} अन्यथा यदि (skip_उपसर्गः(option, "signed-tags=", &option)) {
		option_signed_चिह्नकानि(option);
	} अन्यथा यदि (!वाक्यं_तुलय(option, "quiet")) {
		option_quiet();
	} अन्यथा यदि (!वाक्यं_तुलय(option, "stats")) {
		show_stats = 1;
	} अन्यथा यदि (!वाक्यं_तुलय(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} अन्यथा {
		प्रत्यागम 0;
	}

	प्रत्यागम 1;
}

स्थिर शून्य check_unsafe_feature(संरचना fast_import_state *state,
				 ध्रुव वर्ण *feature)
{
	यदि (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

स्थिर पूर्णाङ्क parse_one_feature(संरचना fast_import_state *state,
			     ध्रुव वर्ण *feature)
{
	ध्रुव वर्ण *arg;

	यदि (skip_उपसर्गः(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} अन्यथा यदि (skip_उपसर्गः(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} अन्यथा यदि (skip_उपसर्गः(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} अन्यथा यदि (skip_उपसर्गः(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} अन्यथा यदि (!वाक्यं_तुलय(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} अन्यथा यदि (skip_उपसर्गः(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_गन्तव्यम्);
	} अन्यथा यदि (skip_उपसर्गः(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} अन्यथा यदि (!वाक्यं_तुलय(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} अन्यथा यदि (!वाक्यं_तुलय(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} अन्यथा यदि (!वाक्यं_तुलय(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} अन्यथा यदि (!वाक्यं_तुलय(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} अन्यथा यदि (!वाक्यं_तुलय(feature, "done")) {
		require_explicit_termination = 1;
	} अन्यथा यदि (!वाक्यं_तुलय(feature, "force")) {
		force_update = 1;
	} अन्यथा यदि (!वाक्यं_तुलय(feature, "notes") || !वाक्यं_तुलय(feature, "ls")) {
		; /* do nothing; we have the feature */
	} अन्यथा {
		प्रत्यागम 0;
	}

	प्रत्यागम 1;
}

स्थिर शून्य parse_feature(संरचना fast_import_state *state, ध्रुव वर्ण *feature)
{
	यदि (state->आलोकितम्_दत्त_command)
		die(_("got feature command '%s' after data command"), feature);

	यदि (parse_one_feature(state, feature))
		प्रत्यागम;

	die(_("this version of fast-import does not support feature %s."), feature);
}

स्थिर शून्य parse_option(संरचना fast_import_state *state, ध्रुव वर्ण *option)
{
	यदि (state->आलोकितम्_दत्त_command)
		die(_("got option command '%s' after data command"), option);

	यदि (parse_one_option(state, option))
		प्रत्यागम;

	die(_("this version of fast-import does not support option: %s"), option);
}

स्थिर शून्य git_pack_config(शून्य)
{
	पूर्णाङ्क indexversion_मानम्;
	पूर्णाङ्क सीमा;
	अचिह्नित दीर्घ packsizelimit_मानम्;

	यदि (!repo_config_get_ulong(the_repository, "pack.depth", &max_गभीरता)) {
		यदि (max_गभीरता > MAX_गभीरता)
			max_गभीरता = MAX_गभीरता;
	}
	यदि (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_मानम्)) {
		pack_idx_opts.संस्करणम् = indexversion_मानम्;
		यदि (pack_idx_opts.संस्करणम् > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.संस्करणम्);
	}
	यदि (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_मानम्))
		max_packsize = packsizelimit_मानम्;

	यदि (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &सीमा))
		unpack_सीमा = सीमा;
	अन्यथा यदि (!repo_config_get_int(the_repository, "transfer.unpacklimit", &सीमा))
		unpack_सीमा = सीमा;

	repo_config(the_repository, git_default_config, रिक्त);
}

स्थिर ध्रुव वर्ण *ध्रुव fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	रिक्त
};

स्थिर शून्य parse_argv(संरचना fast_import_state *state)
{
	पूर्णाङ्क argc = parse_options(state->argc, state->argv, state->उपसर्गः,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	यदि (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->आलोकितम्_दत्त_command = 1;
	यदि (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_गन्तव्यम्);
}

स्थिर पूर्णाङ्क option_parse_date_format(ध्रुव संरचना option *opt अप्रयुक्तम्,
				    ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_export_pack_धाराः(ध्रुव संरचना option *opt,
					  ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_धाराः(opt->मानम्, arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_max_pack_आकार(ध्रुव संरचना option *opt अप्रयुक्तम्,
				      ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_आकार(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_big_file_देहली(ध्रुव संरचना option *opt अप्रयुक्तम्,
					   ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_देहली(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_signed_commits(ध्रुव संरचना option *opt अप्रयुक्तम्,
				       ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_signed_चिह्नकानि(ध्रुव संरचना option *opt अप्रयुक्तम्,
				    ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_चिह्नकानि(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_rewrite_submodules_from(ध्रुव संरचना option *opt,
						ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->मानम्, arg, &sub_marks_from);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_rewrite_submodules_गन्तव्यम्(ध्रुव संरचना option *opt,
					      ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->मानम्, arg, &sub_marks_गन्तव्यम्);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_cat_blob_fd(ध्रुव संरचना option *opt,
				    ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->मानम्, arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_import_marks(ध्रुव संरचना option *opt,
				     ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->मानम्, arg, 0, 0);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_import_marks_if_exists(ध्रुव संरचना option *opt,
					       ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->मानम्, arg, 0, 1);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_export_marks(ध्रुव संरचना option *opt,
				     ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->मानम्, arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_गभीरता(ध्रुव संरचना option *opt अप्रयुक्तम्,
			      ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_गभीरता(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_सक्रिय_branches(ध्रुव संरचना option *opt अप्रयुक्तम्,
					ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_सक्रिय_branches(arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क option_parse_quiet(ध्रुव संरचना option *opt अप्रयुक्तम्,
			      ध्रुव वर्ण *arg अप्रयुक्तम्, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_fast_import(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *उपसर्गः,
		    संरचना repository *repo)
{
	संरचना fast_import_state state;

	संरचना option fast_import_options[] = {
		OPT_गणः(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", रिक्त, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", रिक्त, रिक्त,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", रिक्त, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_आकार),
		OPT_CALLBACK_F(0, "big-file-threshold", रिक्त, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_देहली),
		OPT_CALLBACK_F(0, "depth", रिक्त, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_गभीरता),
		OPT_CALLBACK_F(0, "active-branches", रिक्त, N_("n"),
			       N_("maximum number of branches to maintain active"),
			       PARSE_OPT_NONEG, option_parse_सक्रिय_branches),
		OPT_गणः(N_("Marks")),
		OPT_CALLBACK_F(0, "import-marks", &state, N_("file"),
			       N_("import marks from <file>"),
			       PARSE_OPT_NONEG, option_parse_import_marks),
		OPT_CALLBACK_F(0, "import-marks-if-exists", &state, N_("file"),
			       N_("import marks from <file> if it exists"),
			       PARSE_OPT_NONEG, option_parse_import_marks_if_exists),
		OPT_CALLBACK_F(0, "export-marks", &state, N_("file"),
			       N_("dump marks to <file>"),
			       PARSE_OPT_NONEG, option_parse_export_marks),
		OPT_BOOL(0, "relative-marks", &relative_marks_paths,
			 N_("are --(import|export)-marks= paths relative to '.git/info/fast-import'?")),
		OPT_गणः(N_("Submodule rewrite")),
		OPT_CALLBACK_F(0, "rewrite-submodules-from", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> from <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_from),
		OPT_CALLBACK_F(0, "rewrite-submodules-to", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> to <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_गन्तव्यम्),
		OPT_गणः(N_("Signing")),
		OPT_CALLBACK_F(0, "signed-commits", रिक्त, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", रिक्त, N_("mode"),
			       N_("how to handle signed tags"),
			       PARSE_OPT_NONEG, option_parse_signed_चिह्नकानि),
		OPT_HIDDEN_गणः(N_("Advanced")),
		OPT_BOOL_F(0, "allow-unsafe-features", &state.allow_unsafe_features,
			   N_("allow unsafe mark commands from the stream"),
			   PARSE_OPT_HIDDEN | PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "export-pack-edges", &state, N_("file"),
			       N_("dump edge commits to <file>"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_export_pack_धाराः),
		OPT_CALLBACK_F(0, "cat-blob-fd", &state, N_("fd"),
			       N_("write some responses to <fd> instead of stdout"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_cat_blob_fd),
		OPT_अन्तः()
	};

	show_usage_with_options_if_asked(argc, argv, fast_import_usage, fast_import_options);

	fast_import_state_init(&state, argc, argv, उपसर्गः, fast_import_options);

	reset_pack_idx_option(&pack_idx_opts);
	git_pack_config();

	alloc_objects(object_प्रविष्टिः_alloc);
	strbuf_init(&command_buf, 0);
	CALLOC_ARRAY(atom_सारणी, atom_सारणी_sz);
	CALLOC_ARRAY(branch_सारणी, branch_सारणी_sz);
	CALLOC_ARRAY(avail_वृक्षः_सारणी, avail_वृक्षः_सारणी_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, परिमाण(संरचना mark_समुच्चयः));

	hashmap_init(&object_सारणी, object_प्रविष्टिः_hashcmp, रिक्त, 0);

	/*
	 * We don't parse most options until after we've seen the set of
	 * "feature" lines at the start of the stream (which allows the command
	 * line to override stream data). But we must do an early parse of any
	 * command-line options that impact how we interpret the feature lines.
	 *
	 * NEEDSWORK: This scan only matches the exact "--allow-unsafe-features"
	 * spelling and stops at the first argument that doesn't start with a
	 * dash. As parse_options() below also accepts unambiguous abbreviations
	 * and values separated by a space from their option, the two disagree
	 * for command lines like "--allow-unsafe" or "--depth 5
	 * --allow-unsafe-features": parse_options() accepts the option, but
	 * this scan doesn't see it, so unsafe features from the stream are
	 * still refused. This errs on the safe side, but should be fixed by
	 * teaching this scan about the options that take a value.
	 */
	प्रति (पूर्णाङ्क i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		यदि (*arg != '-' || !वाक्यं_तुलय(arg, "--"))
			भङ्ग;
		यदि (!वाक्यं_तुलय(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * परिमाण(*rc_free));
	प्रति (अचिह्नित पूर्णाङ्क i = 0; i < (cmd_save - 1); i++)
		rc_free[i].अग्र = &rc_free[i + 1];
	rc_free[cmd_save - 1].अग्र = रिक्त;

	start_packfile();
	समुच्चयः_die_routine(die_nicely);
	समुच्चयः_checkpoint_signal();
	यावत् (read_अग्र_command(&state) != EOF) {
		ध्रुव वर्ण *v;
		यदि (!वाक्यं_तुलय("blob", command_buf.buf))
			parse_new_blob(&state);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "tag ", &v))
			parse_new_चिह्नकम्(&state, v);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, रिक्त);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		अन्यथा यदि (!वाक्यं_तुलय("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		अन्यथा यदि (!वाक्यं_तुलय("done", command_buf.buf))
			भङ्ग;
		अन्यथा यदि (!वाक्यं_तुलय("alias", command_buf.buf))
			parse_alias(&state);
		अन्यथा यदि (starts_with(command_buf.buf, "progress "))
			parse_प्रगतिः(&state);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		अन्यथा यदि (skip_उपसर्गः(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		अन्यथा यदि (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		अन्यथा
			die(_("unsupported command: %s"), command_buf.buf);

		यदि (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	यदि (!state.आलोकितम्_दत्त_command)
		parse_argv(&state);

	यदि (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	अन्तः_packfile();

	dump_branches();
	dump_चिह्नकानि();
	unkeep_all_packs();
	dump_marks();

	यदि (pack_धाराः)
		fclose(pack_धाराः);

	यदि (show_stats) {
		uintmax_t योग_गणनम् = 0, duplicate_गणनम् = 0;
		प्रति (आकार_t i = 0; i < ARRAY_आकार(object_गणनम्_by_type); i++)
			योग_गणनम् += object_गणनम्_by_type[i];
		प्रति (आकार_t i = 0; i < ARRAY_आकार(duplicate_गणनम्_by_type); i++)
			duplicate_गणनम् += duplicate_गणनम्_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_गणनम्);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", योग_गणनम्, duplicate_गणनम्);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_गणनम्_by_type[OBJ_BLOB], duplicate_गणनम्_by_type[OBJ_BLOB], delta_गणनम्_by_type[OBJ_BLOB], delta_गणनम्_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_गणनम्_by_type[OBJ_वृक्षः], duplicate_गणनम्_by_type[OBJ_वृक्षः], delta_गणनम्_by_type[OBJ_वृक्षः], delta_गणनम्_attempts_by_type[OBJ_वृक्षः]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_गणनम्_by_type[OBJ_COMMIT], duplicate_गणनम्_by_type[OBJ_COMMIT], delta_गणनम्_by_type[OBJ_COMMIT], delta_गणनम्_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_गणनम्_by_type[OBJ_चिह्नकम्], duplicate_गणनम्_by_type[OBJ_चिह्नकम्], delta_गणनम्_by_type[OBJ_चिह्नकम्], delta_गणनम्_attempts_by_type[OBJ_चिह्नकम्]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_गणनम्, branch_आरोपय_गणनम्);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->सरणम्) * 1024, marks_समुच्चयः_गणनम्);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (वृक्षः_प्रविष्टिः_allocd + fi_mem_pool.pool_alloc + alloc_गणनम्*परिमाण(संरचना object_प्रविष्टिः))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (अचिह्नित दीर्घ)((वृक्षः_प्रविष्टिः_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_गणनम्*परिमाण(संरचना object_प्रविष्टिः))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	प्रत्यागम failure ? 1 : 0;
}
