#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "object.h"
#అన్తర్భావ "blob.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "delta.h"
#అన్తర్భావ "pack.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "csum-file.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "packfile.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "mem-pool.h"
#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "khash.h"
#అన్తర్భావ "date.h"
#అన్తర్భావ "gpg-interface.h"
#అన్తర్భావ "parse-options.h"

#నిర్వచించు PACK_ID_బిట్లు 16
#నిర్వచించు MAX_PACK_ID ((1<<PACK_ID_బిట్లు)-1)
#నిర్వచించు లోతు_బిట్లు 13
#నిర్వచించు MAX_లోతు ((1<<లోతు_బిట్లు)-1)

/*
 * We abuse the setuid bit on directories to mean "do not delta".
 */
#నిర్వచించు NO_DELTA S_ISUID

/*
 * The amount of additional space required in order to write an object into the
 * current pack. This is the hash lengths at the end of the pack, plus the
 * length of one object ID.
 */
#నిర్వచించు PACK_కొలత_పరిమితి (the_hash_algo->rawsz * 3)

నిర్మాణం object_నమోదు {
	నిర్మాణం pack_idx_నమోదు idx;
	నిర్మాణం hashmap_నమోదు ent;
	uint32_t type : TYPE_బిట్లు,
		pack_id : PACK_ID_బిట్లు,
		లోతు : లోతు_బిట్లు;
};

స్థిర పూర్ణాంకం object_నమోదు_hashcmp(స్థిరరాశి శూన్య *map_దత్తాంశం వాడని,
				స్థిరరాశి నిర్మాణం hashmap_నమోదు *eptr,
				స్థిరరాశి నిర్మాణం hashmap_నమోదు *నమోదు_or_కీలకం,
				స్థిరరాశి శూన్య *keydata)
{
	స్థిరరాశి నిర్మాణం object_id *oid = keydata;
	స్థిరరాశి నిర్మాణం object_నమోదు *e1, *e2;

	e1 = container_యొక్క(eptr, స్థిరరాశి నిర్మాణం object_నమోదు, ent);
	ఒకవేళ (oid)
		తిరిగి oidcmp(&e1->idx.oid, oid);

	e2 = container_యొక్క(నమోదు_or_కీలకం, స్థిరరాశి నిర్మాణం object_నమోదు, ent);
	తిరిగి oidcmp(&e1->idx.oid, &e2->idx.oid);
}

నిర్మాణం object_నమోదు_pool {
	నిర్మాణం object_నమోదు_pool *తదుపరి_pool;
	నిర్మాణం object_నమోదు *తదుపరి_free;
	నిర్మాణం object_నమోదు *చివర;
	నిర్మాణం object_నమోదు entries[FLEX_ARRAY]; /*మరిన్ని*/
};

నిర్మాణం mark_సమితి {
	సంఘం {
		నిర్మాణం object_id *oids[1024];
		నిర్మాణం object_నమోదు *marked[1024];
		నిర్మాణం mark_సమితి *sets[1024];
	} దత్తాంశం;
	గుర్తులేని పూర్ణాంకం జరుపు;
};

నిర్మాణం చివరి_object {
	నిర్మాణం strbuf దత్తాంశం;
	off_t స్థానభ్రంశం;
	గుర్తులేని పూర్ణాంకం లోతు;
	గుర్తులేని no_మార్పిడి : 1;
};

నిర్మాణం atom_str {
	నిర్మాణం atom_str *తదుపరి_atom;
	గుర్తులేని పొట్టి str_len;
	అక్షరం str_dat[FLEX_ARRAY]; /*మరిన్ని*/
};

నిర్మాణం వృక్షం_content;
నిర్మాణం వృక్షం_నమోదు {
	నిర్మాణం వృక్షం_content *వృక్షం;
	నిర్మాణం atom_str *name;
	నిర్మాణం వృక్షం_నమోదు_ms {
		uint16_t బహుళకం;
		నిర్మాణం object_id oid;
	} versions[2];
};

నిర్మాణం వృక్షం_content {
	గుర్తులేని పూర్ణాంకం నమోదు_సామర్థ్యం; /* must match avail_tree_content */
	గుర్తులేని పూర్ణాంకం నమోదు_లెక్క;
	గుర్తులేని పూర్ణాంకం delta_లోతు;
	నిర్మాణం వృక్షం_నమోదు *entries[FLEX_ARRAY]; /*మరిన్ని*/
};

నిర్మాణం avail_వృక్షం_content {
	గుర్తులేని పూర్ణాంకం నమోదు_సామర్థ్యం; /* must match tree_content */
	నిర్మాణం avail_వృక్షం_content *తదుపరి_avail;
};

నిర్మాణం branch {
	నిర్మాణం branch *పట్టిక_తదుపరి_branch;
	నిర్మాణం branch *చురుకైన_తదుపరి_branch;
	స్థిరరాశి అక్షరం *name;
	నిర్మాణం వృక్షం_నమోదు branch_వృక్షం;
	uintmax_t చివరి_commit;
	uintmax_t అంకె_notes;
	గుర్తులేని చురుకైన : 1;
	గుర్తులేని delete : 1;
	గుర్తులేని pack_id : PACK_ID_బిట్లు;
	నిర్మాణం object_id oid;
};

నిర్మాణం ట్యాగ్ {
	నిర్మాణం ట్యాగ్ *తదుపరి_ట్యాగ్;
	స్థిరరాశి అక్షరం *name;
	గుర్తులేని పూర్ణాంకం pack_id;
	నిర్మాణం object_id oid;
};

నిర్మాణం hash_list {
	నిర్మాణం hash_list *తదుపరి;
	నిర్మాణం object_id oid;
};

రకనిర్వచనం గణన {
	WHENSPEC_RAW = 1,
	WHENSPEC_RAW_PERMISSIVE,
	WHENSPEC_RFC2822,
	WHENSPEC_NOW
} whenspec_type;

నిర్మాణం recent_command {
	నిర్మాణం recent_command *prev;
	నిర్మాణం recent_command *తదుపరి;
	అక్షరం *buf;
};

రకనిర్వచనం శూన్య (*mark_సమితి_inserter_t)(నిర్మాణం mark_సమితి **s, నిర్మాణం object_id *oid, uintmax_t mark);
రకనిర్వచనం శూన్య (*each_mark_fn_t)(uintmax_t mark, శూన్య *obj, శూన్య *cbp);

/* Configured limits on output */
స్థిర గుర్తులేని దీర్ఘ max_లోతు = 50;
స్థిర off_t max_packsize;
స్థిర పూర్ణాంకం unpack_హద్దు = 100;
స్థిర పూర్ణాంకం force_update;

/* Stats and misc. counters */
స్థిర uintmax_t alloc_లెక్క;
స్థిర uintmax_t marks_సమితి_లెక్క;
స్థిర uintmax_t object_లెక్క_by_type[1 << TYPE_బిట్లు];
స్థిర uintmax_t duplicate_లెక్క_by_type[1 << TYPE_బిట్లు];
స్థిర uintmax_t delta_లెక్క_by_type[1 << TYPE_బిట్లు];
స్థిర uintmax_t delta_లెక్క_attempts_by_type[1 << TYPE_బిట్లు];
స్థిర గుర్తులేని దీర్ఘ object_లెక్క;
స్థిర గుర్తులేని దీర్ఘ branch_లెక్క;
స్థిర గుర్తులేని దీర్ఘ branch_ఎక్కించు_లెక్క;
స్థిర పూర్ణాంకం failure;
స్థిర FILE *pack_అంచులు;
స్థిర పూర్ణాంకం show_stats = 1;
స్థిర పూర్ణాంకం quiet;
స్థిర గణన sign_బహుళకం signed_ట్యాగ్_బహుళకం = SIGN_VERBATIM;
స్థిర గణన sign_బహుళకం signed_commit_బహుళకం = SIGN_VERBATIM;
స్థిర స్థిరరాశి అక్షరం *signed_commit_keyid;
స్థిర స్థిరరాశి అక్షరం *signed_ట్యాగ్_keyid;

/* Memory pools */
స్థిర నిర్మాణం mem_pool fi_mem_pool = {
	.ఖండం_alloc = 2*1024*1024 - పరిమాణం(నిర్మాణం mp_ఖండం),
};

/* Atom management */
స్థిర గుర్తులేని పూర్ణాంకం atom_పట్టిక_sz = 4451;
స్థిర గుర్తులేని పూర్ణాంకం atom_cnt;
స్థిర నిర్మాణం atom_str **atom_పట్టిక;

/* The .pack file being generated */
స్థిర నిర్మాణం pack_idx_option pack_idx_opts;
స్థిర గుర్తులేని పూర్ణాంకం pack_id;
స్థిర నిర్మాణం hashfile *pack_file;
స్థిర నిర్మాణం packed_git *pack_దత్తాంశం;
స్థిర నిర్మాణం packed_git **all_packs;
స్థిర off_t pack_కొలత;

/* Table of objects we've written. */
స్థిర గుర్తులేని పూర్ణాంకం object_నమోదు_alloc = 5000;
స్థిర నిర్మాణం object_నమోదు_pool *ఖండాలు;
స్థిర నిర్మాణం hashmap object_పట్టిక;
స్థిర నిర్మాణం mark_సమితి *marks;
స్థిర అక్షరం *export_marks_file;
స్థిర అక్షరం *import_marks_file;
స్థిర పూర్ణాంకం import_marks_file_from_ప్రవాహం;
స్థిర పూర్ణాంకం import_marks_file_ignore_missing;
స్థిర పూర్ణాంకం import_marks_file_పూర్తి;
స్థిర పూర్ణాంకం relative_marks_paths;

/* Our last blob */
స్థిర నిర్మాణం చివరి_object చివరి_blob = {
	.దత్తాంశం = STRBUF_INIT,
 };

/* Tree management */
స్థిర గుర్తులేని పూర్ణాంకం వృక్షం_నమోదు_alloc = 1000;
స్థిర శూన్య *avail_వృక్షం_నమోదు;
స్థిర గుర్తులేని పూర్ణాంకం avail_వృక్షం_పట్టిక_sz = 100;
స్థిర నిర్మాణం avail_వృక్షం_content **avail_వృక్షం_పట్టిక;
స్థిర కొలత_t వృక్షం_నమోదు_allocd;
స్థిర నిర్మాణం strbuf old_వృక్షం = STRBUF_INIT;
స్థిర నిర్మాణం strbuf new_వృక్షం = STRBUF_INIT;

/* Branch data */
స్థిర గుర్తులేని దీర్ఘ max_చురుకైన_branches = 5;
స్థిర గుర్తులేని దీర్ఘ cur_చురుకైన_branches;
స్థిర గుర్తులేని దీర్ఘ branch_పట్టిక_sz = 1039;
స్థిర నిర్మాణం branch **branch_పట్టిక;
స్థిర నిర్మాణం branch *చురుకైన_branches;

/* Tag data */
స్థిర నిర్మాణం ట్యాగ్ *మొదటి_ట్యాగ్;
స్థిర నిర్మాణం ట్యాగ్ *చివరి_ట్యాగ్;

/* Input stream parsing */
స్థిర whenspec_type whenspec = WHENSPEC_RAW;
స్థిర నిర్మాణం strbuf command_buf = STRBUF_INIT;
స్థిర పూర్ణాంకం unread_command_buf;
స్థిర నిర్మాణం recent_command cmd_hist = {
	.prev = &cmd_hist,
	.తదుపరి = &cmd_hist,
};
స్థిర నిర్మాణం recent_command *cmd_తోక = &cmd_hist;
స్థిర నిర్మాణం recent_command *rc_free;
స్థిర గుర్తులేని పూర్ణాంకం cmd_save = 100;
స్థిర uintmax_t తదుపరి_mark;
స్థిర నిర్మాణం strbuf new_దత్తాంశం = STRBUF_INIT;
స్థిర పూర్ణాంకం require_explicit_termination;

/* Signal handling */
స్థిర అస్థిర sig_atomic_t checkpoint_requested;

/* Submodule marks */
స్థిర నిర్మాణం string_list sub_marks_from = STRING_LIST_INIT_DUP;
స్థిర నిర్మాణం string_list sub_marks_గమ్యం = STRING_LIST_INIT_DUP;
స్థిర kh_oid_map_t *sub_oid_map;

/* Where to write output of cat-blob commands */
స్థిర పూర్ణాంకం cat_blob_fd = STDOUT_FILENO;

/* Command state */
నిర్మాణం fast_import_state {
	పూర్ణాంకం argc;
	స్థిరరాశి అక్షరం **argv;
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	పూర్ణాంకం చూసిన_దత్తాంశం_command;
	పూర్ణాంకం allow_unsafe_features;
	నిర్మాణం option *option;
};

స్థిర శూన్య fast_import_state_init(నిర్మాణం fast_import_state *state,
				   పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv,
				   స్థిరరాశి అక్షరం *ఉపసర్గ, నిర్మాణం option *option)
{
	memset(state, 0, పరిమాణం(*state));
	state->argc = argc;
	state->argv = argv;
	state->ఉపసర్గ = ఉపసర్గ;
	state->option = option;
}

స్థిర శూన్య parse_argv(నిర్మాణం fast_import_state *state);
స్థిర శూన్య parse_get_mark(నిర్మాణం fast_import_state *state, స్థిరరాశి అక్షరం *p);
స్థిర శూన్య parse_cat_blob(నిర్మాణం fast_import_state *state, స్థిరరాశి అక్షరం *p);
స్థిర శూన్య parse_ls(నిర్మాణం fast_import_state *state,
		     స్థిరరాశి అక్షరం *p,
		     నిర్మాణం branch *b);

స్థిర శూన్య for_each_mark(నిర్మాణం mark_సమితి *m, uintmax_t పునాది, each_mark_fn_t callback, శూన్య *p)
{
	uintmax_t k;
	ఒకవేళ (m->జరుపు) {
		కొరకు (k = 0; k < 1024; k++) {
			ఒకవేళ (m->దత్తాంశం.sets[k])
				for_each_mark(m->దత్తాంశం.sets[k], పునాది + (k << m->జరుపు), callback, p);
		}
	} లేకపోతే {
		కొరకు (k = 0; k < 1024; k++) {
			ఒకవేళ (m->దత్తాంశం.marked[k])
				callback(పునాది + k, m->దత్తాంశం.marked[k], p);
		}
	}
}

స్థిర శూన్య dump_marks_fn(uintmax_t mark, శూన్య *object, శూన్య *cbp) {
	నిర్మాణం object_నమోదు *e = object;
	FILE *f = cbp;

	fprintf(f, ":%" PRIuMAX " %s\n", mark, oid_గమ్యం_hex(&e->idx.oid));
}

స్థిర శూన్య write_branch_report(FILE *rpt, నిర్మాణం branch *b)
{
	fprintf(rpt, "%s:\n", b->name);

	fprintf(rpt, "  status      :");
	ఒకవేళ (b->చురుకైన)
		fputs(" active", rpt);
	ఒకవేళ (b->branch_వృక్షం.వృక్షం)
		fputs(" loaded", rpt);
	ఒకవేళ (is_null_oid(&b->branch_వృక్షం.versions[1].oid))
		fputs(" dirty", rpt);
	fputc('\n', rpt);

	fprintf(rpt, "  tip commit  : %s\n", oid_గమ్యం_hex(&b->oid));
	fprintf(rpt, "  old tree    : %s\n",
		oid_గమ్యం_hex(&b->branch_వృక్షం.versions[0].oid));
	fprintf(rpt, "  cur tree    : %s\n",
		oid_గమ్యం_hex(&b->branch_వృక్షం.versions[1].oid));
	fprintf(rpt, "  commit clock: %" PRIuMAX "\n", b->చివరి_commit);

	fputs("  last pack   : ", rpt);
	ఒకవేళ (b->pack_id < MAX_PACK_ID)
		fprintf(rpt, "%u", b->pack_id);
	fputc('\n', rpt);

	fputc('\n', rpt);
}

స్థిర శూన్య write_crash_report(స్థిరరాశి అక్షరం *err)
{
	అక్షరం *loc = repo_git_మార్గం(the_repository, "fast_import_crash_%"PRIuMAX, (uintmax_t) getpid());
	FILE *rpt = fopen(loc, "w");
	నిర్మాణం branch *b;
	గుర్తులేని దీర్ఘ lu;
	నిర్మాణం recent_command *rc;

	ఒకవేళ (!rpt) {
		error_errno(_("can't write crash report %s"), loc);
		విడుదల(loc);
		తిరిగి;
	}

	fprintf(stderr, _("fast-import: dumping crash report to %s\n"), loc);

	fprintf(rpt, "fast-import crash report:\n");
	fprintf(rpt, "    fast-import process: %"PRIuMAX"\n", (uintmax_t) getpid());
	fprintf(rpt, "    parent process     : %"PRIuMAX"\n", (uintmax_t) getppid());
	fprintf(rpt, "    at %s\n", show_date(సమయం(ఖాళీ), 0, DATE_బహుళకం(ISO8601)));
	fputc('\n', rpt);

	fputs("fatal: ", rpt);
	fputs(err, rpt);
	fputc('\n', rpt);

	fputc('\n', rpt);
	fputs("Most Recent Commands Before Crash\n", rpt);
	fputs("---------------------------------\n", rpt);
	కొరకు (rc = cmd_hist.తదుపరి; rc != &cmd_hist; rc = rc->తదుపరి) {
		ఒకవేళ (rc->తదుపరి == &cmd_hist)
			fputs("* ", rpt);
		లేకపోతే
			fputs("  ", rpt);
		fputs(rc->buf, rpt);
		fputc('\n', rpt);
	}

	fputc('\n', rpt);
	fputs("Active Branch LRU\n", rpt);
	fputs("-----------------\n", rpt);
	fprintf(rpt, "    active_branches = %lu cur, %lu max\n",
		cur_చురుకైన_branches,
		max_చురుకైన_branches);
	fputc('\n', rpt);
	fputs("  pos  clock name\n", rpt);
	fputs("  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n", rpt);
	కొరకు (b = చురుకైన_branches, lu = 0; b; b = b->చురుకైన_తదుపరి_branch)
		fprintf(rpt, "  %2lu) %6" PRIuMAX" %s\n",
			++lu, b->చివరి_commit, b->name);

	fputc('\n', rpt);
	fputs("Inactive Branches\n", rpt);
	fputs("-----------------\n", rpt);
	కొరకు (lu = 0; lu < branch_పట్టిక_sz; lu++) {
		కొరకు (b = branch_పట్టిక[lu]; b; b = b->పట్టిక_తదుపరి_branch)
			write_branch_report(rpt, b);
	}

	ఒకవేళ (మొదటి_ట్యాగ్) {
		నిర్మాణం ట్యాగ్ *tg;
		fputc('\n', rpt);
		fputs("Annotated Tags\n", rpt);
		fputs("--------------\n", rpt);
		కొరకు (tg = మొదటి_ట్యాగ్; tg; tg = tg->తదుపరి_ట్యాగ్) {
			fputs(oid_గమ్యం_hex(&tg->oid), rpt);
			fputc(' ', rpt);
			fputs(tg->name, rpt);
			fputc('\n', rpt);
		}
	}

	fputc('\n', rpt);
	fputs("Marks\n", rpt);
	fputs("-----\n", rpt);
	ఒకవేళ (export_marks_file)
		fprintf(rpt, "  exported to %s\n", export_marks_file);
	లేకపోతే
		for_each_mark(marks, 0, dump_marks_fn, rpt);

	fputc('\n', rpt);
	fputs("-------------------\n", rpt);
	fputs("END OF CRASH REPORT\n", rpt);
	fclose(rpt);
	విడుదల(loc);
}

స్థిర శూన్య చివర_packfile(శూన్య);
స్థిర శూన్య unkeep_all_packs(శూన్య);
స్థిర శూన్య dump_marks(శూన్య);

స్థిర NORETURN శూన్య die_nicely(స్థిరరాశి అక్షరం *err, va_list params)
{
	va_list cp;
	స్థిర పూర్ణాంకం zombie;
	report_fn die_సందేశం_fn = get_die_సందేశం_routine();

	va_copy(cp, params);
	die_సందేశం_fn(err, params);

	ఒకవేళ (!zombie) {
		అక్షరం సందేశం_2[2 * మార్గం_MAX];

		zombie = 1;
		vsnprintf(సందేశం_2, పరిమాణం(సందేశం_2), err, cp);
		write_crash_report(సందేశం_2);
		చివర_packfile();
		unkeep_all_packs();
		dump_marks();
	}
	నిష్క్రమించు(128);
}

#నిర్వచితం_కాకపోతే SIGUSR1	/* Windows, for example */

స్థిర శూన్య సమితి_checkpoint_signal(శూన్య)
{
}

#లేకపోతే

స్థిర శూన్య checkpoint_signal(పూర్ణాంకం signo వాడని)
{
	checkpoint_requested = 1;
}

స్థిర శూన్య సమితి_checkpoint_signal(శూన్య)
{
	నిర్మాణం sigaction sa;

	memset(&sa, 0, పరిమాణం(sa));
	sa.sa_handler = checkpoint_signal;
	sigemptyset(&sa.sa_ముసుగు);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, ఖాళీ);
}

#అయితే_ముగింపు

స్థిర శూన్య alloc_objects(గుర్తులేని పూర్ణాంకం cnt)
{
	నిర్మాణం object_నమోదు_pool *b;

	b = xmalloc(పరిమాణం(నిర్మాణం object_నమోదు_pool)
		+ cnt * పరిమాణం(నిర్మాణం object_నమోదు));
	b->తదుపరి_pool = ఖండాలు;
	b->తదుపరి_free = b->entries;
	b->చివర = b->entries + cnt;
	ఖండాలు = b;
	alloc_లెక్క += cnt;
}

స్థిర నిర్మాణం object_నమోదు *new_object(నిర్మాణం object_id *oid)
{
	నిర్మాణం object_నమోదు *e;

	ఒకవేళ (ఖండాలు->తదుపరి_free == ఖండాలు->చివర)
		alloc_objects(object_నమోదు_alloc);

	e = ఖండాలు->తదుపరి_free++;
	oidcpy(&e->idx.oid, oid);
	తిరిగి e;
}

స్థిర నిర్మాణం object_నమోదు *కనుగొను_object(నిర్మాణం object_id *oid)
{
	తిరిగి hashmap_get_నమోదు_from_hash(&object_పట్టిక, oidhash(oid), oid,
					   నిర్మాణం object_నమోదు, ent);
}

స్థిర నిర్మాణం object_నమోదు *చొప్పించు_object(నిర్మాణం object_id *oid)
{
	నిర్మాణం object_నమోదు *e;
	గుర్తులేని పూర్ణాంకం hash = oidhash(oid);

	e = hashmap_get_నమోదు_from_hash(&object_పట్టిక, hash, oid,
					నిర్మాణం object_నమోదు, ent);
	ఒకవేళ (!e) {
		e = new_object(oid);
		e->idx.స్థానభ్రంశం = 0;
		hashmap_నమోదు_init(&e->ent, hash);
		hashmap_కూడు(&object_పట్టిక, &e->ent);
	}

	తిరిగి e;
}

స్థిర శూన్య invalidate_pack_id(గుర్తులేని పూర్ణాంకం id)
{
	గుర్తులేని దీర్ఘ lu;
	నిర్మాణం ట్యాగ్ *t;
	నిర్మాణం hashmap_iter iter;
	నిర్మాణం object_నమోదు *e;

	hashmap_for_each_నమోదు(&object_పట్టిక, &iter, e, ent) {
		ఒకవేళ (e->pack_id == id)
			e->pack_id = MAX_PACK_ID;
	}

	కొరకు (lu = 0; lu < branch_పట్టిక_sz; lu++) {
		నిర్మాణం branch *b;

		కొరకు (b = branch_పట్టిక[lu]; b; b = b->పట్టిక_తదుపరి_branch)
			ఒకవేళ (b->pack_id == id)
				b->pack_id = MAX_PACK_ID;
	}

	కొరకు (t = మొదటి_ట్యాగ్; t; t = t->తదుపరి_ట్యాగ్)
		ఒకవేళ (t->pack_id == id)
			t->pack_id = MAX_PACK_ID;
}

స్థిర గుర్తులేని పూర్ణాంకం hc_str(స్థిరరాశి అక్షరం *s, కొలత_t len)
{
	గుర్తులేని పూర్ణాంకం r = 0;
	అయితే (len-- > 0)
		r = r * 31 + *s++;
	తిరిగి r;
}

స్థిర శూన్య చొప్పించు_mark(నిర్మాణం mark_సమితి **శిఖరం, uintmax_t idnum, నిర్మాణం object_నమోదు *oe)
{
	నిర్మాణం mark_సమితి *s = *శిఖరం;

	అయితే ((idnum >> s->జరుపు) >= 1024) {
		s = mem_pool_calloc(&fi_mem_pool, 1, పరిమాణం(నిర్మాణం mark_సమితి));
		s->జరుపు = (*శిఖరం)->జరుపు + 10;
		s->దత్తాంశం.sets[0] = *శిఖరం;
		*శిఖరం = s;
	}
	అయితే (s->జరుపు) {
		uintmax_t i = idnum >> s->జరుపు;
		idnum -= i << s->జరుపు;
		ఒకవేళ (!s->దత్తాంశం.sets[i]) {
			s->దత్తాంశం.sets[i] = mem_pool_calloc(&fi_mem_pool, 1, పరిమాణం(నిర్మాణం mark_సమితి));
			s->దత్తాంశం.sets[i]->జరుపు = s->జరుపు - 10;
		}
		s = s->దత్తాంశం.sets[i];
	}
	ఒకవేళ (!s->దత్తాంశం.marked[idnum])
		marks_సమితి_లెక్క++;
	s->దత్తాంశం.marked[idnum] = oe;
}

స్థిర శూన్య *కనుగొను_mark(నిర్మాణం mark_సమితి *s, uintmax_t idnum)
{
	uintmax_t orig_idnum = idnum;
	నిర్మాణం object_నమోదు *oe = ఖాళీ;
	ఒకవేళ ((idnum >> s->జరుపు) < 1024) {
		అయితే (s && s->జరుపు) {
			uintmax_t i = idnum >> s->జరుపు;
			idnum -= i << s->జరుపు;
			s = s->దత్తాంశం.sets[i];
		}
		ఒకవేళ (s)
			oe = s->దత్తాంశం.marked[idnum];
	}
	ఒకవేళ (!oe)
		die(_("mark :%" PRIuMAX " not declared"), orig_idnum);
	తిరిగి oe;
}

స్థిర నిర్మాణం atom_str *గమ్యం_atom(స్థిరరాశి అక్షరం *s, గుర్తులేని పొట్టి len)
{
	గుర్తులేని పూర్ణాంకం hc = hc_str(s, len) % atom_పట్టిక_sz;
	నిర్మాణం atom_str *c;

	కొరకు (c = atom_పట్టిక[hc]; c; c = c->తదుపరి_atom)
		ఒకవేళ (c->str_len == len && !అక్షరశ్రేణి_పోలిక(s, c->str_dat, len))
			తిరిగి c;

	c = mem_pool_alloc(&fi_mem_pool, పరిమాణం(నిర్మాణం atom_str) + len + 1);
	c->str_len = len;
	జ్ఞాపకం_నకలు(c->str_dat, s, len);
	c->str_dat[len] = 0;
	c->తదుపరి_atom = atom_పట్టిక[hc];
	atom_పట్టిక[hc] = c;
	atom_cnt++;
	తిరిగి c;
}

స్థిర నిర్మాణం branch *lookup_branch(స్థిరరాశి అక్షరం *name)
{
	గుర్తులేని పూర్ణాంకం hc = hc_str(name, అక్షరశ్రేణి_పొడవు(name)) % branch_పట్టిక_sz;
	నిర్మాణం branch *b;

	కొరకు (b = branch_పట్టిక[hc]; b; b = b->పట్టిక_తదుపరి_branch)
		ఒకవేళ (!strcmp(name, b->name))
			తిరిగి b;
	తిరిగి ఖాళీ;
}

స్థిర నిర్మాణం branch *new_branch(స్థిరరాశి అక్షరం *name)
{
	గుర్తులేని పూర్ణాంకం hc = hc_str(name, అక్షరశ్రేణి_పొడవు(name)) % branch_పట్టిక_sz;
	నిర్మాణం branch *b = lookup_branch(name);

	ఒకవేళ (b)
		die(_("invalid attempt to create duplicate branch: %s"), name);
	ఒకవేళ (check_refname_format(name, REFNAME_ALLOW_ONELEVEL))
		die(_("branch name doesn't conform to Git standards: %s"), name);

	b = mem_pool_calloc(&fi_mem_pool, 1, పరిమాణం(నిర్మాణం branch));
	b->name = mem_pool_strdup(&fi_mem_pool, name);
	b->పట్టిక_తదుపరి_branch = branch_పట్టిక[hc];
	b->branch_వృక్షం.versions[0].బహుళకం = S_IFDIR;
	b->branch_వృక్షం.versions[1].బహుళకం = S_IFDIR;
	b->అంకె_notes = 0;
	b->చురుకైన = 0;
	b->pack_id = MAX_PACK_ID;
	branch_పట్టిక[hc] = b;
	branch_లెక్క++;
	తిరిగి b;
}

స్థిర గుర్తులేని పూర్ణాంకం hc_entries(గుర్తులేని పూర్ణాంకం cnt)
{
	cnt = cnt & 7 ? (cnt / 8) + 1 : cnt / 8;
	తిరిగి cnt < avail_వృక్షం_పట్టిక_sz ? cnt : avail_వృక్షం_పట్టిక_sz - 1;
}

స్థిర నిర్మాణం వృక్షం_content *new_వృక్షం_content(గుర్తులేని పూర్ణాంకం cnt)
{
	నిర్మాణం avail_వృక్షం_content *f, *l = ఖాళీ;
	నిర్మాణం వృక్షం_content *t;
	గుర్తులేని పూర్ణాంకం hc = hc_entries(cnt);

	కొరకు (f = avail_వృక్షం_పట్టిక[hc]; f; l = f, f = f->తదుపరి_avail)
		ఒకవేళ (f->నమోదు_సామర్థ్యం >= cnt)
			ఆపు;

	ఒకవేళ (f) {
		ఒకవేళ (l)
			l->తదుపరి_avail = f->తదుపరి_avail;
		లేకపోతే
			avail_వృక్షం_పట్టిక[hc] = f->తదుపరి_avail;
	} లేకపోతే {
		cnt = cnt & 7 ? ((cnt / 8) + 1) * 8 : cnt;
		f = mem_pool_alloc(&fi_mem_pool, పరిమాణం(*t) + పరిమాణం(t->entries[0]) * cnt);
		f->నమోదు_సామర్థ్యం = cnt;
	}

	t = (నిర్మాణం వృక్షం_content*)f;
	t->నమోదు_లెక్క = 0;
	t->delta_లోతు = 0;
	తిరిగి t;
}

స్థిర శూన్య release_వృక్షం_నమోదు(నిర్మాణం వృక్షం_నమోదు *e);
స్థిర శూన్య release_వృక్షం_content(నిర్మాణం వృక్షం_content *t)
{
	నిర్మాణం avail_వృక్షం_content *f = (నిర్మాణం avail_వృక్షం_content*)t;
	గుర్తులేని పూర్ణాంకం hc = hc_entries(f->నమోదు_సామర్థ్యం);
	f->తదుపరి_avail = avail_వృక్షం_పట్టిక[hc];
	avail_వృక్షం_పట్టిక[hc] = f;
}

స్థిర శూన్య release_వృక్షం_content_recursive(నిర్మాణం వృక్షం_content *t)
{
	గుర్తులేని పూర్ణాంకం i;
	కొరకు (i = 0; i < t->నమోదు_లెక్క; i++)
		release_వృక్షం_నమోదు(t->entries[i]);
	release_వృక్షం_content(t);
}

స్థిర నిర్మాణం వృక్షం_content *grow_వృక్షం_content(
	నిర్మాణం వృక్షం_content *t,
	పూర్ణాంకం amt)
{
	నిర్మాణం వృక్షం_content *r = new_వృక్షం_content(t->నమోదు_లెక్క + amt);
	r->నమోదు_లెక్క = t->నమోదు_లెక్క;
	r->delta_లోతు = t->delta_లోతు;
	COPY_ARRAY(r->entries, t->entries, t->నమోదు_లెక్క);
	release_వృక్షం_content(t);
	తిరిగి r;
}

స్థిర నిర్మాణం వృక్షం_నమోదు *new_వృక్షం_నమోదు(శూన్య)
{
	నిర్మాణం వృక్షం_నమోదు *e;

	ఒకవేళ (!avail_వృక్షం_నమోదు) {
		గుర్తులేని పూర్ణాంకం n = వృక్షం_నమోదు_alloc;
		వృక్షం_నమోదు_allocd += n * పరిమాణం(నిర్మాణం వృక్షం_నమోదు);
		ALLOC_ARRAY(e, n);
		avail_వృక్షం_నమోదు = e;
		అయితే (n-- > 1) {
			*((శూన్య**)e) = e + 1;
			e++;
		}
		*((శూన్య**)e) = ఖాళీ;
	}

	e = avail_వృక్షం_నమోదు;
	avail_వృక్షం_నమోదు = *((శూన్య**)e);
	తిరిగి e;
}

స్థిర శూన్య release_వృక్షం_నమోదు(నిర్మాణం వృక్షం_నమోదు *e)
{
	ఒకవేళ (e->వృక్షం)
		release_వృక్షం_content_recursive(e->వృక్షం);
	*((శూన్య**)e) = avail_వృక్షం_నమోదు;
	avail_వృక్షం_నమోదు = e;
}

స్థిర నిర్మాణం వృక్షం_content *dup_వృక్షం_content(నిర్మాణం వృక్షం_content *s)
{
	నిర్మాణం వృక్షం_content *d;
	నిర్మాణం వృక్షం_నమోదు *a, *b;
	గుర్తులేని పూర్ణాంకం i;

	ఒకవేళ (!s)
		తిరిగి ఖాళీ;
	d = new_వృక్షం_content(s->నమోదు_లెక్క);
	కొరకు (i = 0; i < s->నమోదు_లెక్క; i++) {
		a = s->entries[i];
		b = new_వృక్షం_నమోదు();
		జ్ఞాపకం_నకలు(b, a, పరిమాణం(*a));
		ఒకవేళ (a->వృక్షం && is_null_oid(&b->versions[1].oid))
			b->వృక్షం = dup_వృక్షం_content(a->వృక్షం);
		లేకపోతే
			b->వృక్షం = ఖాళీ;
		d->entries[i] = b;
	}
	d->నమోదు_లెక్క = s->నమోదు_లెక్క;
	d->delta_లోతు = s->delta_లోతు;

	తిరిగి d;
}

స్థిర శూన్య ప్రారంభం_packfile(శూన్య)
{
	నిర్మాణం strbuf tmp_file = STRBUF_INIT;
	నిర్మాణం packed_git *p;
	పూర్ణాంకం pack_fd;

	pack_fd = odb_mkstemp(the_repository->objects, &tmp_file,
			      "pack/tmp_pack_XXXXXX");
	FLEX_ALLOC_STR(p, pack_name, tmp_file.buf);
	strbuf_release(&tmp_file);

	p->pack_fd = pack_fd;
	p->do_not_close = 1;
	p->repo = the_repository;
	pack_file = hashfd(the_repository->hash_algo, pack_fd, p->pack_name);

	pack_దత్తాంశం = p;
	pack_కొలత = write_pack_శీర్షిక(pack_file, 0);
	object_లెక్క = 0;

	REALLOC_ARRAY(all_packs, pack_id + 1);
	all_packs[pack_id] = p;
}

స్థిర స్థిరరాశి అక్షరం *create_సూచీ(శూన్య)
{
	స్థిరరాశి అక్షరం *tmpfile;
	నిర్మాణం pack_idx_నమోదు **idx, **c, **చివరి;
	నిర్మాణం object_నమోదు *e;
	నిర్మాణం object_నమోదు_pool *o;

	/* Build the table of object IDs. */
	ALLOC_ARRAY(idx, object_లెక్క);
	c = idx;
	కొరకు (o = ఖండాలు; o; o = o->తదుపరి_pool)
		కొరకు (e = o->తదుపరి_free; e-- != o->entries;)
			ఒకవేళ (pack_id == e->pack_id)
				*c++ = &e->idx;
	చివరి = idx + object_లెక్క;
	ఒకవేళ (c != చివరి)
		die(_("internal consistency error creating the index"));

	tmpfile = write_idx_file(the_repository, ఖాళీ, idx, object_లెక్క,
				 &pack_idx_opts, pack_దత్తాంశం->hash);
	విడుదల(idx);
	తిరిగి tmpfile;
}

స్థిర అక్షరం *keep_pack(స్థిరరాశి అక్షరం *curr_సూచీ_name)
{
	స్థిర స్థిరరాశి అక్షరం *keep_సందేశం = "fast-import";
	నిర్మాణం strbuf name = STRBUF_INIT;
	పూర్ణాంకం keep_fd;

	odb_pack_name(pack_దత్తాంశం->repo, &name, pack_దత్తాంశం->hash, "keep");
	keep_fd = సురక్షిత_create_file_with_leading_directories(pack_దత్తాంశం->repo,
							    name.buf);
	ఒకవేళ (keep_fd < 0)
		die_errno(_("cannot create keep file"));
	write_or_die(keep_fd, keep_సందేశం, అక్షరశ్రేణి_పొడవు(keep_సందేశం));
	ఒకవేళ (close(keep_fd))
		die_errno(_("failed to write keep file"));

	odb_pack_name(pack_దత్తాంశం->repo, &name, pack_దత్తాంశం->hash, "pack");
	ఒకవేళ (finalize_object_file(pack_దత్తాంశం->repo, pack_దత్తాంశం->pack_name, name.buf))
		die(_("cannot store pack file"));

	odb_pack_name(pack_దత్తాంశం->repo, &name, pack_దత్తాంశం->hash, "idx");
	ఒకవేళ (finalize_object_file(pack_దత్తాంశం->repo, curr_సూచీ_name, name.buf))
		die(_("cannot store index file"));
	విడుదల((శూన్య *)curr_సూచీ_name);
	తిరిగి strbuf_detach(&name, ఖాళీ);
}

స్థిర శూన్య unkeep_all_packs(శూన్య)
{
	నిర్మాణం strbuf name = STRBUF_INIT;
	పూర్ణాంకం k;

	కొరకు (k = 0; k < pack_id; k++) {
		నిర్మాణం packed_git *p = all_packs[k];
		odb_pack_name(p->repo, &name, p->hash, "keep");
		unlink_or_warn(name.buf);
	}
	strbuf_release(&name);
}

స్థిర పూర్ణాంకం loosen_small_pack(స్థిరరాశి నిర్మాణం packed_git *p)
{
	నిర్మాణం బిడ్డ_process unpack = బిడ్డ_PROCESS_INIT;

	ఒకవేళ (lseek(p->pack_fd, 0, SEEK_సమితి) < 0)
		die_errno(_("failed seeking to start of '%s'"), p->pack_name);

	unpack.లోపల = p->pack_fd;
	unpack.git_cmd = 1;
	unpack.stdout_గమ్యం_stderr = 1;
	strvec_నెట్టు(&unpack.వాదనలు, "unpack-objects");
	ఒకవేళ (!show_stats)
		strvec_నెట్టు(&unpack.వాదనలు, "-q");

	తిరిగి నిరంతరం_command(&unpack);
}

స్థిర శూన్య చివర_packfile(శూన్య)
{
	స్థిర పూర్ణాంకం running;

	ఒకవేళ (running || !pack_దత్తాంశం)
		తిరిగి;

	running = 1;
	clear_delta_పునాది_cache();
	ఒకవేళ (object_లెక్క) {
		నిర్మాణం odb_ఆధారం_files *files = odb_ఆధారం_files_downcast(pack_దత్తాంశం->repo->objects->ఆధారం);
		నిర్మాణం packed_git *new_p;
		నిర్మాణం object_id cur_pack_oid;
		అక్షరం *idx_name;
		పూర్ణాంకం i;
		నిర్మాణం branch *b;
		నిర్మాణం ట్యాగ్ *t;

		close_pack_windows(pack_దత్తాంశం);
		finalize_hashfile(pack_file, cur_pack_oid.hash, FSYNC_COMPONENT_PACK, 0);
		fixup_pack_శీర్షిక_footer(the_hash_algo, pack_దత్తాంశం->pack_fd,
					 pack_దత్తాంశం->hash, pack_దత్తాంశం->pack_name,
					 object_లెక్క, cur_pack_oid.hash,
					 pack_కొలత);

		ఒకవేళ (object_లెక్క <= unpack_హద్దు) {
			ఒకవేళ (!loosen_small_pack(pack_దత్తాంశం)) {
				invalidate_pack_id(pack_id);
				వెళ్ళు discard_pack;
			}
		}

		close(pack_దత్తాంశం->pack_fd);
		idx_name = keep_pack(create_సూచీ());

		/* Register the packfile with core git's machinery. */
		new_p = packfile_store_ఎక్కించు_pack(files->dirs->packed, idx_name, 1);
		ఒకవేళ (!new_p)
			die(_("core Git rejected index %s"), idx_name);
		all_packs[pack_id] = new_p;
		విడుదల(idx_name);

		/* Print the boundary */
		ఒకవేళ (pack_అంచులు) {
			fprintf(pack_అంచులు, "%s:", new_p->pack_name);
			కొరకు (i = 0; i < branch_పట్టిక_sz; i++) {
				కొరకు (b = branch_పట్టిక[i]; b; b = b->పట్టిక_తదుపరి_branch) {
					ఒకవేళ (b->pack_id == pack_id)
						fprintf(pack_అంచులు, " %s",
							oid_గమ్యం_hex(&b->oid));
				}
			}
			కొరకు (t = మొదటి_ట్యాగ్; t; t = t->తదుపరి_ట్యాగ్) {
				ఒకవేళ (t->pack_id == pack_id)
					fprintf(pack_అంచులు, " %s",
						oid_గమ్యం_hex(&t->oid));
			}
			fputc('\n', pack_అంచులు);
			fflush(pack_అంచులు);
		}

		pack_id++;
	}
	లేకపోతే {
discard_pack:
		close(pack_దత్తాంశం->pack_fd);
		unlink_or_warn(pack_దత్తాంశం->pack_name);
	}
	FREE_AND_NULL(pack_దత్తాంశం);
	running = 0;

	/* We can't carry a delta across packfiles. */
	strbuf_release(&చివరి_blob.దత్తాంశం);
	చివరి_blob.స్థానభ్రంశం = 0;
	చివరి_blob.లోతు = 0;
}

స్థిర శూన్య చక్రం_packfile(శూన్య)
{
	చివర_packfile();
	ప్రారంభం_packfile();
}

స్థిర పూర్ణాంకం store_object(
	గణన object_type type,
	నిర్మాణం strbuf *dat,
	నిర్మాణం చివరి_object *చివరి,
	నిర్మాణం object_id *oidout,
	uintmax_t mark)
{
	నిర్మాణం odb_ఆధారం_files *files = odb_ఆధారం_files_downcast(the_repository->objects->ఆధారం);
	శూన్య *బహిర్గతం, *delta;
	నిర్మాణం object_నమోదు *e;
	గుర్తులేని అక్షరం hdr[96];
	నిర్మాణం object_id oid;
	గుర్తులేని దీర్ఘ hdrlen, deltalen = 0;
	నిర్మాణం git_hash_ctx c;
	git_zstream s;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	hdrlen = format_object_శీర్షిక((అక్షరం *)hdr, పరిమాణం(hdr), type,
				      dat->len);
	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, hdr, hdrlen);
	git_hash_update(&c, dat->buf, dat->len);
	git_hash_final_oid(&oid, &c);
	ఒకవేళ (oidout)
		oidcpy(oidout, &oid);

	e = చొప్పించు_object(&oid);
	ఒకవేళ (mark)
		చొప్పించు_mark(&marks, mark, e);
	ఒకవేళ (e->idx.స్థానభ్రంశం) {
		duplicate_లెక్క_by_type[type]++;
		తిరిగి 1;
	}

	కొరకు (నిర్మాణం odb_files_dir *dir = files->dirs; dir; dir = dir->తదుపరి) {
		ఒకవేళ (!packfile_list_కనుగొను_oid(packfile_store_get_packs(dir->packed), &oid))
			కొనసాగు;
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.స్థానభ్రంశం = 1; /* just not zero! */
		duplicate_లెక్క_by_type[type]++;
		తిరిగి 1;
	}

	ఒకవేళ (చివరి && చివరి->దత్తాంశం.len && చివరి->దత్తాంశం.buf && చివరి->లోతు < max_లోతు
		&& dat->len > the_hash_algo->rawsz) {
		కొలత_t deltalen_st;

		delta_లెక్క_attempts_by_type[type]++;
		delta = diff_delta(చివరి->దత్తాంశం.buf, చివరి->దత్తాంశం.len,
			dat->buf, dat->len,
			&deltalen_st, dat->len - the_hash_algo->rawsz);
		deltalen = cast_కొలత_t_గమ్యం_ulong(deltalen_st);
	} లేకపోతే
		delta = ఖాళీ;

	git_deflate_init(&s, cfg->pack_compression_level);
	ఒకవేళ (delta) {
		s.తదుపరి_లోపల = delta;
		s.avail_లోపల = deltalen;
	} లేకపోతే {
		s.తదుపరి_లోపల = (శూన్య *)dat->buf;
		s.avail_లోపల = dat->len;
	}
	s.avail_బహిర్గతం = git_deflate_bound(&s, s.avail_లోపల);
	s.తదుపరి_బహిర్గతం = బహిర్గతం = xmalloc(s.avail_బహిర్గతం);
	అయితే (git_deflate(&s, Z_ముగింపు) == Z_OK)
		; /* nothing */
	git_deflate_చివర(&s);

	/* Determine if we should auto-checkpoint. */
	ఒకవేళ ((max_packsize
		&& (pack_కొలత + PACK_కొలత_పరిమితి + s.మొత్తం_బహిర్గతం) > max_packsize)
		|| (pack_కొలత + PACK_కొలత_పరిమితి + s.మొత్తం_బహిర్గతం) < pack_కొలత) {

		/* This new object needs to *not* have the current pack_id. */
		e->pack_id = pack_id + 1;
		చక్రం_packfile();

		/* We cannot carry a delta into the new pack. */
		ఒకవేళ (delta) {
			FREE_AND_NULL(delta);

			git_deflate_init(&s, cfg->pack_compression_level);
			s.తదుపరి_లోపల = (శూన్య *)dat->buf;
			s.avail_లోపల = dat->len;
			s.avail_బహిర్గతం = git_deflate_bound(&s, s.avail_లోపల);
			s.తదుపరి_బహిర్గతం = బహిర్గతం = xrealloc(బహిర్గతం, s.avail_బహిర్గతం);
			అయితే (git_deflate(&s, Z_ముగింపు) == Z_OK)
				; /* nothing */
			git_deflate_చివర(&s);
		}
	}

	e->type = type;
	e->pack_id = pack_id;
	e->idx.స్థానభ్రంశం = pack_కొలత;
	object_లెక్క++;
	object_లెక్క_by_type[type]++;

	చక్రీయ_తనిఖీ_32_begin(pack_file);

	ఒకవేళ (delta) {
		off_t ofs = e->idx.స్థానభ్రంశం - చివరి->స్థానభ్రంశం;
		గుర్తులేని pos = పరిమాణం(hdr) - 1;

		delta_లెక్క_by_type[type]++;
		e->లోతు = చివరి->లోతు + 1;

		hdrlen = encode_లోపల_pack_object_శీర్షిక(hdr, పరిమాణం(hdr),
						      OBJ_OFS_DELTA, deltalen);
		hashwrite(pack_file, hdr, hdrlen);
		pack_కొలత += hdrlen;

		hdr[pos] = ofs & 127;
		అయితే (ofs >>= 7)
			hdr[--pos] = 128 | (--ofs & 127);
		hashwrite(pack_file, hdr + pos, పరిమాణం(hdr) - pos);
		pack_కొలత += పరిమాణం(hdr) - pos;
	} లేకపోతే {
		e->లోతు = 0;
		hdrlen = encode_లోపల_pack_object_శీర్షిక(hdr, పరిమాణం(hdr),
						      type, dat->len);
		hashwrite(pack_file, hdr, hdrlen);
		pack_కొలత += hdrlen;
	}

	hashwrite(pack_file, బహిర్గతం, s.మొత్తం_బహిర్గతం);
	pack_కొలత += s.మొత్తం_బహిర్గతం;

	e->idx.చక్రీయ_తనిఖీ_32 = చక్రీయ_తనిఖీ_32_చివర(pack_file);

	విడుదల(బహిర్గతం);
	విడుదల(delta);
	ఒకవేళ (చివరి) {
		ఒకవేళ (చివరి->no_మార్పిడి) {
			చివరి->దత్తాంశం = *dat;
		} లేకపోతే {
			strbuf_మార్పిడి(&చివరి->దత్తాంశం, dat);
		}
		చివరి->స్థానభ్రంశం = e->idx.స్థానభ్రంశం;
		చివరి->లోతు = e->లోతు;
	}
	తిరిగి 0;
}

స్థిర శూన్య truncate_pack(నిర్మాణం hashfile_checkpoint *checkpoint)
{
	ఒకవేళ (hashfile_truncate(pack_file, checkpoint))
		die_errno(_("cannot truncate pack to skip duplicate"));
	pack_కొలత = checkpoint->స్థానభ్రంశం;
}

స్థిర శూన్య ప్రవాహం_blob(uintmax_t len, నిర్మాణం object_id *oidout, uintmax_t mark)
{
	నిర్మాణం odb_ఆధారం_files *files = odb_ఆధారం_files_downcast(the_repository->objects->ఆధారం);
	కొలత_t లోపల_sz = 64 * 1024, బహిర్గతం_sz = 64 * 1024;
	గుర్తులేని అక్షరం *లోపల_buf = xmalloc(లోపల_sz);
	గుర్తులేని అక్షరం *బహిర్గతం_buf = xmalloc(బహిర్గతం_sz);
	నిర్మాణం object_నమోదు *e;
	నిర్మాణం object_id oid;
	గుర్తులేని దీర్ఘ hdrlen;
	off_t స్థానభ్రంశం;
	నిర్మాణం git_hash_ctx c;
	git_zstream s;
	నిర్మాణం hashfile_checkpoint checkpoint;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);
	పూర్ణాంకం స్థితి_2 = Z_OK;

	/* Determine if we should auto-checkpoint. */
	ఒకవేళ ((max_packsize
		&& (pack_కొలత + PACK_కొలత_పరిమితి + len) > max_packsize)
		|| (pack_కొలత + PACK_కొలత_పరిమితి + len) < pack_కొలత)
		చక్రం_packfile();

	hashfile_checkpoint_init(pack_file, &checkpoint);
	hashfile_checkpoint(pack_file, &checkpoint);
	స్థానభ్రంశం = checkpoint.స్థానభ్రంశం;

	hdrlen = format_object_శీర్షిక((అక్షరం *)బహిర్గతం_buf, బహిర్గతం_sz, OBJ_BLOB, len);

	git_hash_init(&c, the_hash_algo);
	git_hash_update(&c, బహిర్గతం_buf, hdrlen);

	చక్రీయ_తనిఖీ_32_begin(pack_file);

	git_deflate_init(&s, cfg->pack_compression_level);

	hdrlen = encode_లోపల_pack_object_శీర్షిక(బహిర్గతం_buf, బహిర్గతం_sz, OBJ_BLOB, len);

	s.తదుపరి_బహిర్గతం = బహిర్గతం_buf + hdrlen;
	s.avail_బహిర్గతం = బహిర్గతం_sz - hdrlen;

	అయితే (స్థితి_2 != Z_ప్రవాహం_చివర) {
		ఒకవేళ (0 < len && !s.avail_లోపల) {
			కొలత_t cnt = లోపల_sz < len ? లోపల_sz : (కొలత_t)len;
			కొలత_t n = fread(లోపల_buf, 1, cnt, stdin);
			ఒకవేళ (!n && feof(stdin))
				die(_("EOF in data (%" PRIuMAX " bytes remaining)"), len);

			git_hash_update(&c, లోపల_buf, n);
			s.తదుపరి_లోపల = లోపల_buf;
			s.avail_లోపల = n;
			len -= n;
		}

		స్థితి_2 = git_deflate(&s, len ? 0 : Z_ముగింపు);

		ఒకవేళ (!s.avail_బహిర్గతం || స్థితి_2 == Z_ప్రవాహం_చివర) {
			కొలత_t n = s.తదుపరి_బహిర్గతం - బహిర్గతం_buf;
			hashwrite(pack_file, బహిర్గతం_buf, n);
			pack_కొలత += n;
			s.తదుపరి_బహిర్గతం = బహిర్గతం_buf;
			s.avail_బహిర్గతం = బహిర్గతం_sz;
		}

		ఎంపిక (స్థితి_2) {
		స్థితి Z_OK:
		స్థితి Z_BUF_ERROR:
		స్థితి Z_ప్రవాహం_చివర:
			కొనసాగు;
		అప్రమేయ:
			die(_("unexpected deflate failure: %d"), స్థితి_2);
		}
	}
	git_deflate_చివర(&s);
	git_hash_final_oid(&oid, &c);

	ఒకవేళ (oidout)
		oidcpy(oidout, &oid);

	e = చొప్పించు_object(&oid);

	ఒకవేళ (mark)
		చొప్పించు_mark(&marks, mark, e);

	ఒకవేళ (e->idx.స్థానభ్రంశం) {
		duplicate_లెక్క_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		వెళ్ళు బహిర్గతం;
	}

	కొరకు (నిర్మాణం odb_files_dir *dir = files->dirs; dir; dir = dir->తదుపరి) {
		ఒకవేళ (!packfile_list_కనుగొను_oid(packfile_store_get_packs(dir->packed), &oid))
			కొనసాగు;
		e->type = OBJ_BLOB;
		e->pack_id = MAX_PACK_ID;
		e->idx.స్థానభ్రంశం = 1; /* just not zero! */
		duplicate_లెక్క_by_type[OBJ_BLOB]++;
		truncate_pack(&checkpoint);
		వెళ్ళు బహిర్గతం;
	}

	e->లోతు = 0;
	e->type = OBJ_BLOB;
	e->pack_id = pack_id;
	e->idx.స్థానభ్రంశం = స్థానభ్రంశం;
	e->idx.చక్రీయ_తనిఖీ_32 = చక్రీయ_తనిఖీ_32_చివర(pack_file);
	object_లెక్క++;
	object_లెక్క_by_type[OBJ_BLOB]++;

బహిర్గతం:
	విడుదల(లోపల_buf);
	విడుదల(బహిర్గతం_buf);
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
స్థిర శూన్య *gfi_unpack_నమోదు(
	నిర్మాణం object_నమోదు *oe,
	గుర్తులేని దీర్ఘ *sizep)
{
	గణన object_type type;
	కొలత_t కొలత_st = 0;
	శూన్య *దత్తాంశం;
	నిర్మాణం packed_git *p = all_packs[oe->pack_id];
	ఒకవేళ (p == pack_దత్తాంశం && p->pack_కొలత < (pack_కొలత + the_hash_algo->rawsz)) {
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
		p->pack_కొలత = pack_కొలత + the_hash_algo->rawsz;
	}
	దత్తాంశం = unpack_నమోదు(the_repository, p, oe->idx.స్థానభ్రంశం, &type, &కొలత_st);
	ఒకవేళ (sizep)
		*sizep = cast_కొలత_t_గమ్యం_ulong(కొలత_st);
	తిరిగి దత్తాంశం;
}

స్థిర శూన్య ఎక్కించు_వృక్షం(నిర్మాణం వృక్షం_నమోదు *మూలం)
{
	నిర్మాణం object_id *oid = &మూలం->versions[1].oid;
	నిర్మాణం object_నమోదు *myoe;
	నిర్మాణం వృక్షం_content *t;
	గుర్తులేని దీర్ఘ కొలత;
	అక్షరం *buf;
	స్థిరరాశి అక్షరం *c;

	మూలం->వృక్షం = t = new_వృక్షం_content(8);
	ఒకవేళ (is_null_oid(oid))
		తిరిగి;

	myoe = కనుగొను_object(oid);
	ఒకవేళ (myoe && myoe->pack_id != MAX_PACK_ID) {
		ఒకవేళ (myoe->type != OBJ_వృక్షం)
			die(_("not a tree: %s"), oid_గమ్యం_hex(oid));
		t->delta_లోతు = myoe->లోతు;
		buf = gfi_unpack_నమోదు(myoe, &కొలత);
		ఒకవేళ (!buf)
			die(_("can't load tree %s"), oid_గమ్యం_hex(oid));
	} లేకపోతే {
		గణన object_type type;
		కొలత_t కొలత_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &కొలత_st);
		కొలత = cast_కొలత_t_గమ్యం_ulong(కొలత_st);
		ఒకవేళ (!buf || type != OBJ_వృక్షం)
			die(_("can't load tree %s"), oid_గమ్యం_hex(oid));
	}

	c = buf;
	అయితే (c != (buf + కొలత)) {
		నిర్మాణం వృక్షం_నమోదు *e = new_వృక్షం_నమోదు();

		ఒకవేళ (t->నమోదు_లెక్క == t->నమోదు_సామర్థ్యం)
			మూలం->వృక్షం = t = grow_వృక్షం_content(t, t->నమోదు_లెక్క);
		t->entries[t->నమోదు_లెక్క++] = e;

		e->వృక్షం = ఖాళీ;
		c = parse_బహుళకం(c, &e->versions[1].బహుళకం);
		ఒకవేళ (!c)
			die(_("corrupt mode in %s"), oid_గమ్యం_hex(oid));
		e->versions[0].బహుళకం = e->versions[1].బహుళకం;
		e->name = గమ్యం_atom(c, అక్షరశ్రేణి_పొడవు(c));
		c += e->name->str_len + 1;
		oidread(&e->versions[0].oid, (గుర్తులేని అక్షరం *)c,
			the_repository->hash_algo);
		oidread(&e->versions[1].oid, (గుర్తులేని అక్షరం *)c,
			the_repository->hash_algo);
		c += the_hash_algo->rawsz;
	}
	విడుదల(buf);
}

స్థిర పూర్ణాంకం tecmp0 (స్థిరరాశి శూన్య *_a, స్థిరరాశి శూన్య *_b)
{
	నిర్మాణం వృక్షం_నమోదు *a = *((నిర్మాణం వృక్షం_నమోదు**)_a);
	నిర్మాణం వృక్షం_నమోదు *b = *((నిర్మాణం వృక్షం_నమోదు**)_b);
	తిరిగి పునాది_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[0].బహుళకం,
		b->name->str_dat, b->name->str_len, b->versions[0].బహుళకం);
}

స్థిర పూర్ణాంకం tecmp1 (స్థిరరాశి శూన్య *_a, స్థిరరాశి శూన్య *_b)
{
	నిర్మాణం వృక్షం_నమోదు *a = *((నిర్మాణం వృక్షం_నమోదు**)_a);
	నిర్మాణం వృక్షం_నమోదు *b = *((నిర్మాణం వృక్షం_నమోదు**)_b);
	తిరిగి పునాది_name_compare(
		a->name->str_dat, a->name->str_len, a->versions[1].బహుళకం,
		b->name->str_dat, b->name->str_len, b->versions[1].బహుళకం);
}

స్థిర శూన్య mktree(నిర్మాణం వృక్షం_content *t, పూర్ణాంకం v, నిర్మాణం strbuf *b)
{
	కొలత_t maxlen = 0;
	గుర్తులేని పూర్ణాంకం i;

	ఒకవేళ (!v)
		QSORT(t->entries, t->నమోదు_లెక్క, tecmp0);
	లేకపోతే
		QSORT(t->entries, t->నమోదు_లెక్క, tecmp1);

	కొరకు (i = 0; i < t->నమోదు_లెక్క; i++) {
		ఒకవేళ (t->entries[i]->versions[v].బహుళకం)
			maxlen += t->entries[i]->name->str_len + 34;
	}

	strbuf_reset(b);
	strbuf_grow(b, maxlen);
	కొరకు (i = 0; i < t->నమోదు_లెక్క; i++) {
		నిర్మాణం వృక్షం_నమోదు *e = t->entries[i];
		ఒకవేళ (!e->versions[v].బహుళకం)
			కొనసాగు;
		strbuf_addf(b, "%o %s%c",
			(గుర్తులేని పూర్ణాంకం)(e->versions[v].బహుళకం & ~NO_DELTA),
			e->name->str_dat, '\0');
		strbuf_కూడు(b, e->versions[v].oid.hash, the_hash_algo->rawsz);
	}
}

స్థిర శూన్య store_వృక్షం(నిర్మాణం వృక్షం_నమోదు *మూలం)
{
	నిర్మాణం వృక్షం_content *t;
	గుర్తులేని పూర్ణాంకం i, j, del;
	నిర్మాణం చివరి_object lo = { STRBUF_INIT, 0, 0, /* no_swap */ 1 };
	నిర్మాణం object_నమోదు *le = ఖాళీ;

	ఒకవేళ (!is_null_oid(&మూలం->versions[1].oid))
		తిరిగి;

	ఒకవేళ (!మూలం->వృక్షం)
		ఎక్కించు_వృక్షం(మూలం);
	t = మూలం->వృక్షం;

	కొరకు (i = 0; i < t->నమోదు_లెక్క; i++) {
		ఒకవేళ (t->entries[i]->వృక్షం)
			store_వృక్షం(t->entries[i]);
	}

	ఒకవేళ (!(మూలం->versions[0].బహుళకం & NO_DELTA))
		le = కనుగొను_object(&మూలం->versions[0].oid);
	ఒకవేళ (S_ISDIR(మూలం->versions[0].బహుళకం) && le && le->pack_id == pack_id) {
		mktree(t, 0, &old_వృక్షం);
		lo.దత్తాంశం = old_వృక్షం;
		lo.స్థానభ్రంశం = le->idx.స్థానభ్రంశం;
		lo.లోతు = t->delta_లోతు;
	}

	mktree(t, 1, &new_వృక్షం);
	store_object(OBJ_వృక్షం, &new_వృక్షం, &lo, &మూలం->versions[1].oid, 0);

	t->delta_లోతు = lo.లోతు;
	కొరకు (i = 0, j = 0, del = 0; i < t->నమోదు_లెక్క; i++) {
		నిర్మాణం వృక్షం_నమోదు *e = t->entries[i];
		ఒకవేళ (e->versions[1].బహుళకం) {
			e->versions[0].బహుళకం = e->versions[1].బహుళకం;
			oidcpy(&e->versions[0].oid, &e->versions[1].oid);
			t->entries[j++] = e;
		} లేకపోతే {
			release_వృక్షం_నమోదు(e);
			del++;
		}
	}
	t->నమోదు_లెక్క -= del;
}

స్థిర శూన్య వృక్షం_content_replace(
	నిర్మాణం వృక్షం_నమోదు *మూలం,
	స్థిరరాశి నిర్మాణం object_id *oid,
	స్థిరరాశి uint16_t బహుళకం,
	నిర్మాణం వృక్షం_content *newtree)
{
	ఒకవేళ (!S_ISDIR(బహుళకం))
		die(_("root cannot be a non-directory"));
	oidclr(&మూలం->versions[0].oid, the_repository->hash_algo);
	oidcpy(&మూలం->versions[1].oid, oid);
	ఒకవేళ (మూలం->వృక్షం)
		release_వృక్షం_content_recursive(మూలం->వృక్షం);
	మూలం->వృక్షం = newtree;
}

స్థిర పూర్ణాంకం వృక్షం_content_సమితి(
	నిర్మాణం వృక్షం_నమోదు *మూలం,
	స్థిరరాశి అక్షరం *p,
	స్థిరరాశి నిర్మాణం object_id *oid,
	స్థిరరాశి uint16_t బహుళకం,
	నిర్మాణం వృక్షం_content *subtree)
{
	నిర్మాణం వృక్షం_content *t;
	స్థిరరాశి అక్షరం *slash1;
	గుర్తులేని పూర్ణాంకం i, n;
	నిర్మాణం వృక్షం_నమోదు *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	ఒకవేళ (!n)
		die(_("empty path component found in input"));
	ఒకవేళ (!*slash1 && !S_ISDIR(బహుళకం) && subtree)
		die(_("non-directories cannot have subtrees"));

	ఒకవేళ (!మూలం->వృక్షం)
		ఎక్కించు_వృక్షం(మూలం);
	t = మూలం->వృక్షం;
	కొరకు (i = 0; i < t->నమోదు_లెక్క; i++) {
		e = t->entries[i];
		ఒకవేళ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ఒకవేళ (!*slash1) {
				ఒకవేళ (!S_ISDIR(బహుళకం)
						&& e->versions[1].బహుళకం == బహుళకం
						&& oideq(&e->versions[1].oid, oid))
					తిరిగి 0;
				e->versions[1].బహుళకం = బహుళకం;
				oidcpy(&e->versions[1].oid, oid);
				ఒకవేళ (e->వృక్షం)
					release_వృక్షం_content_recursive(e->వృక్షం);
				e->వృక్షం = subtree;

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
				ఒకవేళ (S_ISDIR(e->versions[0].బహుళకం))
					e->versions[0].బహుళకం |= NO_DELTA;

				oidclr(&మూలం->versions[1].oid, the_repository->hash_algo);
				తిరిగి 1;
			}
			ఒకవేళ (!S_ISDIR(e->versions[1].బహుళకం)) {
				e->వృక్షం = new_వృక్షం_content(8);
				e->versions[1].బహుళకం = S_IFDIR;
			}
			ఒకవేళ (!e->వృక్షం)
				ఎక్కించు_వృక్షం(e);
			ఒకవేళ (వృక్షం_content_సమితి(e, slash1 + 1, oid, బహుళకం, subtree)) {
				oidclr(&మూలం->versions[1].oid, the_repository->hash_algo);
				తిరిగి 1;
			}
			తిరిగి 0;
		}
	}

	ఒకవేళ (t->నమోదు_లెక్క == t->నమోదు_సామర్థ్యం)
		మూలం->వృక్షం = t = grow_వృక్షం_content(t, t->నమోదు_లెక్క);
	e = new_వృక్షం_నమోదు();
	e->name = గమ్యం_atom(p, n);
	e->versions[0].బహుళకం = 0;
	oidclr(&e->versions[0].oid, the_repository->hash_algo);
	t->entries[t->నమోదు_లెక్క++] = e;
	ఒకవేళ (*slash1) {
		e->వృక్షం = new_వృక్షం_content(8);
		e->versions[1].బహుళకం = S_IFDIR;
		వృక్షం_content_సమితి(e, slash1 + 1, oid, బహుళకం, subtree);
	} లేకపోతే {
		e->వృక్షం = subtree;
		e->versions[1].బహుళకం = బహుళకం;
		oidcpy(&e->versions[1].oid, oid);
	}
	oidclr(&మూలం->versions[1].oid, the_repository->hash_algo);
	తిరిగి 1;
}

స్థిర పూర్ణాంకం వృక్షం_content_remove(
	నిర్మాణం వృక్షం_నమోదు *మూలం,
	స్థిరరాశి అక్షరం *p,
	నిర్మాణం వృక్షం_నమోదు *backup_ఆకు,
	పూర్ణాంకం allow_మూలం)
{
	నిర్మాణం వృక్షం_content *t;
	స్థిరరాశి అక్షరం *slash1;
	గుర్తులేని పూర్ణాంకం i, n;
	నిర్మాణం వృక్షం_నమోదు *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;

	ఒకవేళ (!మూలం->వృక్షం)
		ఎక్కించు_వృక్షం(మూలం);

	ఒకవేళ (!*p && allow_మూలం) {
		e = మూలం;
		వెళ్ళు del_నమోదు;
	}

	t = మూలం->వృక్షం;
	కొరకు (i = 0; i < t->నమోదు_లెక్క; i++) {
		e = t->entries[i];
		ఒకవేళ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ఒకవేళ (*slash1 && !S_ISDIR(e->versions[1].బహుళకం))
				/*
				 * If p names a file in some subdirectory, and a
				 * file or symlink matching the name of the
				 * parent directory of p exists, then p cannot
				 * exist and need not be deleted.
				 */
				తిరిగి 1;
			ఒకవేళ (!*slash1 || !S_ISDIR(e->versions[1].బహుళకం))
				వెళ్ళు del_నమోదు;
			ఒకవేళ (!e->వృక్షం)
				ఎక్కించు_వృక్షం(e);
			ఒకవేళ (వృక్షం_content_remove(e, slash1 + 1, backup_ఆకు, 0)) {
				కొరకు (n = 0; n < e->వృక్షం->నమోదు_లెక్క; n++) {
					ఒకవేళ (e->వృక్షం->entries[n]->versions[1].బహుళకం) {
						oidclr(&మూలం->versions[1].oid,
						       the_repository->hash_algo);
						తిరిగి 1;
					}
				}
				backup_ఆకు = ఖాళీ;
				వెళ్ళు del_నమోదు;
			}
			తిరిగి 0;
		}
	}
	తిరిగి 0;

del_నమోదు:
	ఒకవేళ (backup_ఆకు)
		జ్ఞాపకం_నకలు(backup_ఆకు, e, పరిమాణం(*backup_ఆకు));
	లేకపోతే ఒకవేళ (e->వృక్షం)
		release_వృక్షం_content_recursive(e->వృక్షం);
	e->వృక్షం = ఖాళీ;
	e->versions[1].బహుళకం = 0;
	oidclr(&e->versions[1].oid, the_repository->hash_algo);
	oidclr(&మూలం->versions[1].oid, the_repository->hash_algo);
	తిరిగి 1;
}

స్థిర పూర్ణాంకం వృక్షం_content_get(
	నిర్మాణం వృక్షం_నమోదు *మూలం,
	స్థిరరాశి అక్షరం *p,
	నిర్మాణం వృక్షం_నమోదు *ఆకు,
	పూర్ణాంకం allow_మూలం)
{
	నిర్మాణం వృక్షం_content *t;
	స్థిరరాశి అక్షరం *slash1;
	గుర్తులేని పూర్ణాంకం i, n;
	నిర్మాణం వృక్షం_నమోదు *e;

	slash1 = strchrnul(p, '/');
	n = slash1 - p;
	ఒకవేళ (!n && !allow_మూలం)
		die(_("empty path component found in input"));

	ఒకవేళ (!మూలం->వృక్షం)
		ఎక్కించు_వృక్షం(మూలం);

	ఒకవేళ (!n) {
		e = మూలం;
		వెళ్ళు దొరికింది_నమోదు;
	}

	t = మూలం->వృక్షం;
	కొరకు (i = 0; i < t->నమోదు_లెక్క; i++) {
		e = t->entries[i];
		ఒకవేళ (e->name->str_len == n && !fspathncmp(p, e->name->str_dat, n)) {
			ఒకవేళ (!*slash1)
				వెళ్ళు దొరికింది_నమోదు;
			ఒకవేళ (!S_ISDIR(e->versions[1].బహుళకం))
				తిరిగి 0;
			ఒకవేళ (!e->వృక్షం)
				ఎక్కించు_వృక్షం(e);
			తిరిగి వృక్షం_content_get(e, slash1 + 1, ఆకు, 0);
		}
	}
	తిరిగి 0;

దొరికింది_నమోదు:
	జ్ఞాపకం_నకలు(ఆకు, e, పరిమాణం(*ఆకు));
	ఒకవేళ (e->వృక్షం && is_null_oid(&e->versions[1].oid))
		ఆకు->వృక్షం = dup_వృక్షం_content(e->వృక్షం);
	లేకపోతే
		ఆకు->వృక్షం = ఖాళీ;
	తిరిగి 1;
}

స్థిర పూర్ణాంకం update_branch(నిర్మాణం branch *b)
{
	స్థిర స్థిరరాశి అక్షరం *సందేశం = "fast-import";
	నిర్మాణం ref_transaction *transaction;
	నిర్మాణం object_id old_oid;
	నిర్మాణం strbuf err = STRBUF_INIT;
	స్థిర స్థిరరాశి అక్షరం *replace_ఉపసర్గ = "refs/replace/";

	ఒకవేళ (starts_with(b->name, replace_ఉపసర్గ) &&
	    !strcmp(b->name + అక్షరశ్రేణి_పొడవు(replace_ఉపసర్గ),
		    oid_గమ్యం_hex(&b->oid))) {
		ఒకవేళ (!quiet)
			warning(_("dropping %s since it would point to "
				  "itself (i.e. to %s)"),
				b->name, oid_గమ్యం_hex(&b->oid));
		refs_delete_ref(get_main_ref_store(the_repository),
				ఖాళీ, b->name, ఖాళీ, 0);
		తిరిగి 0;
	}
	ఒకవేళ (is_null_oid(&b->oid)) {
		ఒకవేళ (b->delete)
			refs_delete_ref(get_main_ref_store(the_repository),
					ఖాళీ, b->name, ఖాళీ, 0);
		తిరిగి 0;
	}
	ఒకవేళ (refs_read_ref(get_main_ref_store(the_repository), b->name, &old_oid))
		oidclr(&old_oid, the_repository->hash_algo);
	ఒకవేళ (!force_update && !is_null_oid(&old_oid)) {
		నిర్మాణం commit *old_cmit, *new_cmit;
		పూర్ణాంకం ret;

		old_cmit = lookup_commit_reference_gently(the_repository,
							  &old_oid, 0);
		new_cmit = lookup_commit_reference_gently(the_repository,
							  &b->oid, 0);
		ఒకవేళ (!old_cmit || !new_cmit)
			తిరిగి error(_("branch %s is missing commits."), b->name);

		ret = repo_లోపల_విలీనం_bases(the_repository, old_cmit, new_cmit);
		ఒకవేళ (ret < 0)
			నిష్క్రమించు(128);
		ఒకవేళ (!ret) {
			warning(_("not updating %s"
				  " (new tip %s does not contain %s)"),
				b->name, oid_గమ్యం_hex(&b->oid),
				oid_గమ్యం_hex(&old_oid));
			తిరిగి -1;
		}
	}
	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ఒకవేళ (!transaction ||
	    ref_transaction_update(transaction, b->name, &b->oid, &old_oid,
				   ఖాళీ, ఖాళీ, 0, సందేశం, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ref_transaction_free(transaction);
		error("%s", err.buf);
		strbuf_release(&err);
		తిరిగి -1;
	}
	ref_transaction_free(transaction);
	strbuf_release(&err);
	తిరిగి 0;
}

స్థిర శూన్య dump_branches(శూన్య)
{
	గుర్తులేని పూర్ణాంకం i;
	నిర్మాణం branch *b;

	కొరకు (i = 0; i < branch_పట్టిక_sz; i++) {
		కొరకు (b = branch_పట్టిక[i]; b; b = b->పట్టిక_తదుపరి_branch)
			failure |= update_branch(b);
	}
}

స్థిర శూన్య dump_ట్యాగులు(శూన్య)
{
	స్థిర స్థిరరాశి అక్షరం *సందేశం = "fast-import";
	నిర్మాణం ట్యాగ్ *t;
	నిర్మాణం strbuf ref_name = STRBUF_INIT;
	నిర్మాణం strbuf err = STRBUF_INIT;
	నిర్మాణం ref_transaction *transaction;

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ఒకవేళ (!transaction) {
		failure |= error("%s", err.buf);
		వెళ్ళు cleanup;
	}
	కొరకు (t = మొదటి_ట్యాగ్; t; t = t->తదుపరి_ట్యాగ్) {
		strbuf_reset(&ref_name);
		strbuf_addf(&ref_name, "refs/tags/%s", t->name);

		ఒకవేళ (ref_transaction_update(transaction, ref_name.buf,
					   &t->oid, ఖాళీ, ఖాళీ, ఖాళీ,
					   0, సందేశం, &err)) {
			failure |= error("%s", err.buf);
			వెళ్ళు cleanup;
		}
	}
	ఒకవేళ (ref_transaction_commit(transaction, &err))
		failure |= error("%s", err.buf);

 cleanup:
	ref_transaction_free(transaction);
	strbuf_release(&ref_name);
	strbuf_release(&err);
}

స్థిర శూన్య dump_marks(శూన్య)
{
	నిర్మాణం తాళం_file mark_తాళం = తాళం_INIT;
	FILE *f;

	ఒకవేళ (!export_marks_file || (import_marks_file && !import_marks_file_పూర్తి))
		తిరిగి;

	ఒకవేళ (సురక్షిత_create_leading_directories_const(the_repository, export_marks_file)) {
		failure |= error_errno(_("unable to create leading directories of %s"),
				       export_marks_file);
		తిరిగి;
	}

	ఒకవేళ (hold_తాళం_file_for_update(&mark_తాళం, export_marks_file, 0) < 0) {
		failure |= error_errno(_("unable to write marks file %s"),
				       export_marks_file);
		తిరిగి;
	}

	f = fdopen_తాళం_file(&mark_తాళం, "w");
	ఒకవేళ (!f) {
		పూర్ణాంకం saved_errno = errno;
		rollback_తాళం_file(&mark_తాళం);
		failure |= error(_("unable to write marks file %s: %s"),
			export_marks_file, strerror(saved_errno));
		తిరిగి;
	}

	for_each_mark(marks, 0, dump_marks_fn, f);
	ఒకవేళ (commit_తాళం_file(&mark_తాళం)) {
		failure |= error_errno(_("unable to write file %s"),
				       export_marks_file);
		తిరిగి;
	}
}

స్థిర శూన్య చొప్పించు_object_నమోదు(నిర్మాణం mark_సమితి **s, నిర్మాణం object_id *oid, uintmax_t mark)
{
	నిర్మాణం object_నమోదు *e;
	e = కనుగొను_object(oid);
	ఒకవేళ (!e) {
		గణన object_type type = odb_read_object_info(the_repository->objects,
							     oid, ఖాళీ);
		ఒకవేళ (type < 0)
			die(_("object not found: %s"), oid_గమ్యం_hex(oid));
		e = చొప్పించు_object(oid);
		e->type = type;
		e->pack_id = MAX_PACK_ID;
		e->idx.స్థానభ్రంశం = 1; /* just not zero! */
	}
	చొప్పించు_mark(s, mark, e);
}

స్థిర శూన్య చొప్పించు_oid_నమోదు(నిర్మాణం mark_సమితి **s, నిర్మాణం object_id *oid, uintmax_t mark)
{
	చొప్పించు_mark(s, mark, xmemdupz(oid, పరిమాణం(*oid)));
}

స్థిర శూన్య read_mark_file(నిర్మాణం mark_సమితి **s, FILE *f, mark_సమితి_inserter_t inserter)
{
	అక్షరం పంక్తి[512];
	అయితే (fgets(పంక్తి, పరిమాణం(పంక్తి), f)) {
		uintmax_t mark;
		అక్షరం *చివర;
		నిర్మాణం object_id oid;

		/* Ensure SHA-1 objects are padded with zeros. */
		memset(oid.hash, 0, పరిమాణం(oid.hash));

		చివర = strchr(పంక్తి, '\n');
		ఒకవేళ (పంక్తి[0] != ':' || !చివర)
			die(_("corrupt mark line: %s"), పంక్తి);
		*చివర = 0;
		mark = strtoumax(పంక్తి + 1, &చివర, 10);
		ఒకవేళ (!mark || చివర == పంక్తి + 1
			|| *చివర != ' '
			|| get_oid_hex_any(చివర + 1, &oid) == GIT_HASH_UNKNOWN)
			die(_("corrupt mark line: %s"), పంక్తి);
		inserter(s, &oid, mark);
	}
}

స్థిర శూన్య read_marks(శూన్య)
{
	FILE *f = fopen(import_marks_file, "r");
	ఒకవేళ (f)
		;
	లేకపోతే ఒకవేళ (import_marks_file_ignore_missing && errno == ENOENT)
		వెళ్ళు పూర్తి; /* Marks file does not exist */
	లేకపోతే
		die_errno(_("cannot read '%s'"), import_marks_file);
	read_mark_file(&marks, f, చొప్పించు_object_నమోదు);
	fclose(f);
పూర్తి:
	import_marks_file_పూర్తి = 1;
}


స్థిర పూర్ణాంకం read_తదుపరి_command(నిర్మాణం fast_import_state *state)
{
	స్థిర పూర్ణాంకం stdin_eof = 0;

	ఒకవేళ (stdin_eof) {
		unread_command_buf = 0;
		తిరిగి EOF;
	}

	కొరకు (;;) {
		ఒకవేళ (unread_command_buf) {
			unread_command_buf = 0;
		} లేకపోతే {
			నిర్మాణం recent_command *rc;

			stdin_eof = strbuf_getline_lf(&command_buf, stdin);
			ఒకవేళ (stdin_eof)
				తిరిగి EOF;

			ఒకవేళ (!state->చూసిన_దత్తాంశం_command
				&& !starts_with(command_buf.buf, "feature ")
				&& !starts_with(command_buf.buf, "option ")) {
				parse_argv(state);
			}

			rc = rc_free;
			ఒకవేళ (rc)
				rc_free = rc->తదుపరి;
			లేకపోతే {
				rc = cmd_hist.తదుపరి;
				cmd_hist.తదుపరి = rc->తదుపరి;
				cmd_hist.తదుపరి->prev = &cmd_hist;
				విడుదల(rc->buf);
			}

			rc->buf = xstrdup(command_buf.buf);
			rc->prev = cmd_తోక;
			rc->తదుపరి = cmd_hist.prev;
			rc->prev->తదుపరి = rc;
			cmd_తోక = rc;
		}
		ఒకవేళ (command_buf.buf[0] == '#')
			కొనసాగు;
		తిరిగి 0;
	}
}

స్థిర శూన్య skip_optional_lf(శూన్య)
{
	పూర్ణాంకం పదం_char = fgetc(stdin);
	ఒకవేళ (పదం_char != '\n' && పదం_char != EOF)
		ungetc(పదం_char, stdin);
}

స్థిర శూన్య parse_mark(నిర్మాణం fast_import_state *state)
{
	స్థిరరాశి అక్షరం *v;
	ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "mark :", &v)) {
		తదుపరి_mark = strtoumax(v, ఖాళీ, 10);
		read_తదుపరి_command(state);
	}
	లేకపోతే
		తదుపరి_mark = 0;
}

స్థిర శూన్య parse_original_identifier(నిర్మాణం fast_import_state *state)
{
	స్థిరరాశి అక్షరం *v;
	ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "original-oid ", &v))
		read_తదుపరి_command(state);
}

స్థిర పూర్ణాంకం parse_దత్తాంశం(నిర్మాణం strbuf *sb, uintmax_t హద్దు, uintmax_t *len_res)
{
	స్థిరరాశి అక్షరం *దత్తాంశం;
	strbuf_reset(sb);

	ఒకవేళ (!skip_ఉపసర్గ(command_buf.buf, "data ", &దత్తాంశం))
		die(_("expected 'data n' command, found: %s"), command_buf.buf);

	ఒకవేళ (skip_ఉపసర్గ(దత్తాంశం, "<<", &దత్తాంశం)) {
		అక్షరం *పదం = xstrdup(దత్తాంశం);
		కొలత_t పదం_len = command_buf.len - (దత్తాంశం - command_buf.buf);

		కొరకు (;;) {
			ఒకవేళ (strbuf_getline_lf(&command_buf, stdin) == EOF)
				die(_("EOF in data (terminator '%s' not found)"), పదం);
			ఒకవేళ (పదం_len == command_buf.len
				&& !strcmp(పదం, command_buf.buf))
				ఆపు;
			strbuf_addbuf(sb, &command_buf);
			strbuf_addch(sb, '\n');
		}
		విడుదల(పదం);
	}
	లేకపోతే {
		uintmax_t len = strtoumax(దత్తాంశం, ఖాళీ, 10);
		కొలత_t n = 0, నిడివి = (కొలత_t)len;

		ఒకవేళ (హద్దు && హద్దు < len) {
			*len_res = len;
			తిరిగి 0;
		}
		ఒకవేళ (నిడివి < len)
			die(_("data is too large to use in this context"));

		అయితే (n < నిడివి) {
			కొలత_t s = strbuf_fread(sb, నిడివి - n, stdin);
			ఒకవేళ (!s && feof(stdin))
				die(_("EOF in data (%lu bytes remaining)"),
					(గుర్తులేని దీర్ఘ)(నిడివి - n));
			n += s;
		}
	}

	skip_optional_lf();
	తిరిగి 1;
}

స్థిర పూర్ణాంకం validate_raw_date(స్థిరరాశి అక్షరం *src, నిర్మాణం strbuf *ఫలితం, పూర్ణాంకం strict)
{
	స్థిరరాశి అక్షరం *orig_src = src;
	అక్షరం *endp;
	గుర్తులేని దీర్ఘ అంకె;

	errno = 0;

	అంకె = strtoul(src, &endp, 10);
	/*
	 * NEEDSWORK: perhaps check for reasonable values? For example, we
	 *            could error on values representing times more than a
	 *            day in the future.
	 */
	ఒకవేళ (errno || endp == src || *endp != ' ')
		తిరిగి -1;

	src = endp + 1;
	ఒకవేళ (*src != '-' && *src != '+')
		తిరిగి -1;

	అంకె = strtoul(src + 1, &endp, 10);
	/*
	 * NEEDSWORK: check for brokenness other than num > 1400, such as
	 *            (num % 100) >= 60, or ((num % 100) % 15) != 0 ?
	 */
	ఒకవేళ (errno || endp == src + 1 || *endp || /* did not parse */
	    (strict && (1400 < అంకె))             /* parsed a broken timezone */
	   )
		తిరిగి -1;

	strbuf_addstr(ఫలితం, orig_src);
	తిరిగి 0;
}

స్థిర అక్షరం *parse_ident(స్థిరరాశి అక్షరం *buf)
{
	స్థిరరాశి అక్షరం *ltgt;
	కొలత_t name_len;
	నిర్మాణం strbuf ident = STRBUF_INIT;

	/* ensure there is a space delimiter even if there is no name */
	ఒకవేళ (*buf == '<')
		--buf;

	ltgt = buf + strcspn(buf, "<>");
	ఒకవేళ (*ltgt != '<')
		die(_("missing < in ident string: %s"), buf);
	ఒకవేళ (ltgt != buf && ltgt[-1] != ' ')
		die(_("missing space before < in ident string: %s"), buf);
	ltgt = ltgt + 1 + strcspn(ltgt + 1, "<>");
	ఒకవేళ (*ltgt != '>')
		die(_("missing > in ident string: %s"), buf);
	ltgt++;
	ఒకవేళ (*ltgt != ' ')
		die(_("missing space after > in ident string: %s"), buf);
	ltgt++;
	name_len = ltgt - buf;
	strbuf_కూడు(&ident, buf, name_len);

	ఎంపిక (whenspec) {
	స్థితి WHENSPEC_RAW:
		ఒకవేళ (validate_raw_date(ltgt, &ident, 1) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		ఆపు;
	స్థితి WHENSPEC_RAW_PERMISSIVE:
		ఒకవేళ (validate_raw_date(ltgt, &ident, 0) < 0)
			die(_("invalid raw date \"%s\" in ident: %s"), ltgt, buf);
		ఆపు;
	స్థితి WHENSPEC_RFC2822:
		ఒకవేళ (parse_date(ltgt, &ident) < 0)
			die(_("invalid rfc2822 date \"%s\" in ident: %s"), ltgt, buf);
		ఆపు;
	స్థితి WHENSPEC_NOW:
		ఒకవేళ (strcmp("now", ltgt))
			die(_("date in ident must be 'now': %s"), buf);
		datestamp(&ident);
		ఆపు;
	}

	తిరిగి strbuf_detach(&ident, ఖాళీ);
}

స్థిర శూన్య parse_and_store_blob(
	నిర్మాణం చివరి_object *చివరి,
	నిర్మాణం object_id *oidout,
	uintmax_t mark)
{
	స్థిర నిర్మాణం strbuf buf = STRBUF_INIT;
	uintmax_t len;

	ఒకవేళ (parse_దత్తాంశం(&buf, repo_settings_get_big_file_పరిమితి(the_repository), &len))
		store_object(OBJ_BLOB, &buf, చివరి, oidout, mark);
	లేకపోతే {
		ఒకవేళ (చివరి) {
			strbuf_release(&చివరి->దత్తాంశం);
			చివరి->స్థానభ్రంశం = 0;
			చివరి->లోతు = 0;
		}
		ప్రవాహం_blob(len, oidout, mark);
		skip_optional_lf();
	}
}

స్థిర శూన్య parse_new_blob(నిర్మాణం fast_import_state *state)
{
	read_తదుపరి_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	parse_and_store_blob(&చివరి_blob, ఖాళీ, తదుపరి_mark);
}

స్థిర శూన్య unload_one_branch(శూన్య)
{
	అయితే (cur_చురుకైన_branches
		&& cur_చురుకైన_branches >= max_చురుకైన_branches) {
		uintmax_t min_commit = ULONG_MAX;
		నిర్మాణం branch *e, *l = ఖాళీ, *p = ఖాళీ;

		కొరకు (e = చురుకైన_branches; e; e = e->చురుకైన_తదుపరి_branch) {
			ఒకవేళ (e->చివరి_commit < min_commit) {
				p = l;
				min_commit = e->చివరి_commit;
			}
			l = e;
		}

		ఒకవేళ (p) {
			e = p->చురుకైన_తదుపరి_branch;
			p->చురుకైన_తదుపరి_branch = e->చురుకైన_తదుపరి_branch;
		} లేకపోతే {
			e = చురుకైన_branches;
			చురుకైన_branches = e->చురుకైన_తదుపరి_branch;
		}
		e->చురుకైన = 0;
		e->చురుకైన_తదుపరి_branch = ఖాళీ;
		ఒకవేళ (e->branch_వృక్షం.వృక్షం) {
			release_వృక్షం_content_recursive(e->branch_వృక్షం.వృక్షం);
			e->branch_వృక్షం.వృక్షం = ఖాళీ;
		}
		cur_చురుకైన_branches--;
	}
}

స్థిర శూన్య ఎక్కించు_branch(నిర్మాణం branch *b)
{
	ఎక్కించు_వృక్షం(&b->branch_వృక్షం);
	ఒకవేళ (!b->చురుకైన) {
		b->చురుకైన = 1;
		b->చురుకైన_తదుపరి_branch = చురుకైన_branches;
		చురుకైన_branches = b;
		cur_చురుకైన_branches++;
		branch_ఎక్కించు_లెక్క++;
	}
}

స్థిర గుర్తులేని అక్షరం convert_అంకె_notes_గమ్యం_fanout(uintmax_t అంకె_notes)
{
	గుర్తులేని అక్షరం fanout = 0;
	అయితే ((అంకె_notes >>= 8))
		fanout++;
	తిరిగి fanout;
}

స్థిర శూన్య construct_మార్గం_with_fanout(స్థిరరాశి అక్షరం *hex_sha1,
		గుర్తులేని అక్షరం fanout, అక్షరం *మార్గం)
{
	గుర్తులేని పూర్ణాంకం i = 0, j = 0;
	ఒకవేళ (fanout >= the_hash_algo->rawsz)
		die(_("too large fanout (%u)"), fanout);
	అయితే (fanout) {
		మార్గం[i++] = hex_sha1[j++];
		మార్గం[i++] = hex_sha1[j++];
		మార్గం[i++] = '/';
		fanout--;
	}
	జ్ఞాపకం_నకలు(మార్గం + i, hex_sha1 + j, the_hash_algo->hexsz - j);
	మార్గం[i + the_hash_algo->hexsz - j] = '\0';
}

స్థిర uintmax_t do_change_note_fanout(
		నిర్మాణం వృక్షం_నమోదు *orig_మూలం, నిర్మాణం వృక్షం_నమోదు *మూలం,
		అక్షరం *hex_oid, గుర్తులేని పూర్ణాంకం hex_oid_len,
		అక్షరం *fullpath, గుర్తులేని పూర్ణాంకం fullpath_len,
		గుర్తులేని అక్షరం fanout)
{
	నిర్మాణం వృక్షం_content *t;
	నిర్మాణం వృక్షం_నమోదు *e, ఆకు;
	గుర్తులేని పూర్ణాంకం i, tmp_hex_oid_len, tmp_fullpath_len;
	uintmax_t అంకె_notes = 0;
	నిర్మాణం object_id oid;
	/* hex oid + '/' between each pair of hex digits + NUL */
	అక్షరం realpath[GIT_MAX_HEXSZ + ((GIT_MAX_HEXSZ / 2) - 1) + 1];
	స్థిరరాశి గుర్తులేని hexsz = the_hash_algo->hexsz;

	ఒకవేళ (!మూలం->వృక్షం)
		ఎక్కించు_వృక్షం(మూలం);
	t = మూలం->వృక్షం;

	కొరకు (i = 0; t && i < t->నమోదు_లెక్క; i++) {
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
		ఒకవేళ (!e->versions[1].బహుళకం ||
		    tmp_hex_oid_len > hexsz ||
		    e->name->str_len % 2)
			కొనసాగు;

		/* This _may_ be a note entry, or a subdir containing notes */
		జ్ఞాపకం_నకలు(hex_oid + hex_oid_len, e->name->str_dat,
		       e->name->str_len);
		ఒకవేళ (tmp_fullpath_len)
			fullpath[tmp_fullpath_len++] = '/';
		జ్ఞాపకం_నకలు(fullpath + tmp_fullpath_len, e->name->str_dat,
		       e->name->str_len);
		tmp_fullpath_len += e->name->str_len;
		fullpath[tmp_fullpath_len] = '\0';

		ఒకవేళ (tmp_hex_oid_len == hexsz && !get_oid_hex(hex_oid, &oid)) {
			/* This is a note entry */
			ఒకవేళ (fanout == 0xff) {
				/* Counting mode, no rename */
				అంకె_notes++;
				కొనసాగు;
			}
			construct_మార్గం_with_fanout(hex_oid, fanout, realpath);
			ఒకవేళ (!strcmp(fullpath, realpath)) {
				/* Note entry is in correct location */
				అంకె_notes++;
				కొనసాగు;
			}

			/* Rename fullpath to realpath */
			ఒకవేళ (!వృక్షం_content_remove(orig_మూలం, fullpath, &ఆకు, 0))
				die(_("failed to remove path %s"), fullpath);
			వృక్షం_content_సమితి(orig_మూలం, realpath,
				&ఆకు.versions[1].oid,
				ఆకు.versions[1].బహుళకం,
				ఆకు.వృక్షం);
		} లేకపోతే ఒకవేళ (S_ISDIR(e->versions[1].బహుళకం)) {
			/* This is a subdir that may contain note entries */
			అంకె_notes += do_change_note_fanout(orig_మూలం, e,
				hex_oid, tmp_hex_oid_len,
				fullpath, tmp_fullpath_len, fanout);
		}

		/* The above may have reallocated the current tree_content */
		t = మూలం->వృక్షం;
	}
	తిరిగి అంకె_notes;
}

స్థిర uintmax_t change_note_fanout(నిర్మాణం వృక్షం_నమోదు *మూలం,
		గుర్తులేని అక్షరం fanout)
{
	/*
	 * The size of path is due to one slash between every two hex digits,
	 * plus the terminating NUL.  Note that there is no slash at the end, so
	 * the number of slashes is one less than half the number of hex
	 * characters.
	 */
	అక్షరం hex_oid[GIT_MAX_HEXSZ], మార్గం[GIT_MAX_HEXSZ + (GIT_MAX_HEXSZ / 2) - 1 + 1];
	తిరిగి do_change_note_fanout(మూలం, మూలం, hex_oid, 0, మార్గం, 0, fanout);
}

స్థిర పూర్ణాంకం parse_mapped_oid_hex(స్థిరరాశి అక్షరం *hex, నిర్మాణం object_id *oid, స్థిరరాశి అక్షరం **చివర)
{
	పూర్ణాంకం algo;
	khiter_t it;

	/* Make SHA-1 object IDs have all-zero padding. */
	memset(oid->hash, 0, పరిమాణం(oid->hash));

	algo = parse_oid_hex_any(hex, oid, చివర);
	ఒకవేళ (algo == GIT_HASH_UNKNOWN)
		తిరిగి -1;

	it = kh_get_oid_map(sub_oid_map, *oid);
	/* No such object? */
	ఒకవేళ (it == kh_చివర(sub_oid_map)) {
		/* If we're using the same algorithm, pass it through. */
		ఒకవేళ (hash_algos[algo].format_id == the_hash_algo->format_id)
			తిరిగి 0;
		తిరిగి -1;
	}
	oidcpy(oid, kh_విలువ(sub_oid_map, it));
	తిరిగి 0;
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
స్థిర uintmax_t parse_mark_ref(స్థిరరాశి అక్షరం *p, అక్షరం **endptr)
{
	uintmax_t mark;

	ప్రతిజ్ఞా(*p == ':');
	p++;
	mark = strtoumax(p, endptr, 10);
	ఒకవేళ (*endptr == p)
		die(_("no value after ':' in mark: %s"), command_buf.buf);
	తిరిగి mark;
}

/*
 * Parse the mark reference, and complain if this is not the end of
 * the string.
 */
స్థిర uintmax_t parse_mark_ref_eol(స్థిరరాశి అక్షరం *p)
{
	అక్షరం *చివర;
	uintmax_t mark;

	mark = parse_mark_ref(p, &చివర);
	ఒకవేళ (*చివర != '\0')
		die(_("garbage after mark: %s"), command_buf.buf);
	తిరిగి mark;
}

/*
 * Parse the mark reference, demanding a trailing space. Update *p to
 * point to the first character after the space.
 */
స్థిర uintmax_t parse_mark_ref_space(స్థిరరాశి అక్షరం **p)
{
	uintmax_t mark;
	అక్షరం *చివర;

	mark = parse_mark_ref(*p, &చివర);
	ఒకవేళ (*చివర++ != ' ')
		die(_("missing space after mark: %s"), command_buf.buf);
	*p = చివర;
	తిరిగి mark;
}

/*
 * Parse the path string into the strbuf. The path can either be quoted with
 * escape sequences or unquoted without escape sequences. Unquoted strings may
 * contain spaces only if `is_last_field` is nonzero; otherwise, it stops
 * parsing at the first space.
 */
స్థిర శూన్య parse_మార్గం(నిర్మాణం strbuf *sb, స్థిరరాశి అక్షరం *p, స్థిరరాశి అక్షరం **endp,
		పూర్ణాంకం is_చివరి_క్షేత్రం, స్థిరరాశి అక్షరం *క్షేత్రం)
{
	ఒకవేళ (*p == '"') {
		ఒకవేళ (unquote_c_style(sb, p, endp))
			die(_("invalid %s: %s"), క్షేత్రం, command_buf.buf);
		ఒకవేళ (అక్షరశ్రేణి_పొడవు(sb->buf) != sb->len)
			die(_("NUL in %s: %s"), క్షేత్రం, command_buf.buf);
	} లేకపోతే {
		/*
		 * Unless we are parsing the last field of a line,
		 * SP is the end of this field.
		 */
		*endp = is_చివరి_క్షేత్రం
			? p + అక్షరశ్రేణి_పొడవు(p)
			: strchrnul(p, ' ');
		strbuf_కూడు(sb, p, *endp - p);
	}
}

/*
 * Parse the path string into the strbuf, and complain if this is not the end of
 * the string. Unquoted strings may contain spaces.
 */
స్థిర శూన్య parse_మార్గం_eol(నిర్మాణం strbuf *sb, స్థిరరాశి అక్షరం *p, స్థిరరాశి అక్షరం *క్షేత్రం)
{
	స్థిరరాశి అక్షరం *చివర;

	parse_మార్గం(sb, p, &చివర, 1, క్షేత్రం);
	ఒకవేళ (*చివర)
		die(_("garbage after %s: %s"), క్షేత్రం, command_buf.buf);
}

/*
 * Parse the path string into the strbuf, and ensure it is followed by a space.
 * Unquoted strings may not contain spaces. Update *endp to point to the first
 * character after the space.
 */
స్థిర శూన్య parse_మార్గం_space(నిర్మాణం strbuf *sb, స్థిరరాశి అక్షరం *p,
		స్థిరరాశి అక్షరం **endp, స్థిరరాశి అక్షరం *క్షేత్రం)
{
	parse_మార్గం(sb, p, endp, 0, క్షేత్రం);
	ఒకవేళ (**endp != ' ')
		die(_("missing space after %s: %s"), క్షేత్రం, command_buf.buf);
	(*endp)++;
}

స్థిర శూన్య file_change_m(నిర్మాణం fast_import_state *state,
			  స్థిరరాశి అక్షరం *p,
			  నిర్మాణం branch *b)
{
	స్థిర నిర్మాణం strbuf మార్గం = STRBUF_INIT;
	నిర్మాణం object_నమోదు *oe;
	నిర్మాణం object_id oid;
	uint16_t బహుళకం, inline_దత్తాంశం = 0;

	p = parse_బహుళకం(p, &బహుళకం);
	ఒకవేళ (!p)
		die(_("corrupt mode: %s"), command_buf.buf);
	ఎంపిక (బహుళకం) {
	స్థితి 0644:
	స్థితి 0755:
		బహుళకం |= S_IFREG;
	స్థితి S_IFREG | 0644:
	స్థితి S_IFREG | 0755:
	స్థితి S_IFLNK:
	స్థితి S_IFDIR:
	స్థితి S_IFGITLINK:
		/* ok */
		ఆపు;
	అప్రమేయ:
		die(_("corrupt mode: %s"), command_buf.buf);
	}

	ఒకవేళ (*p == ':') {
		oe = కనుగొను_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(p, "inline ", &p)) {
		inline_దత్తాంశం = 1;
		oe = ఖాళీ; /* not used with inline_data, but makes gcc happy */
	} లేకపోతే {
		ఒకవేళ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = కనుగొను_object(&oid);
		ఒకవేళ (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	strbuf_reset(&మార్గం);
	parse_మార్గం_eol(&మార్గం, p, "path");

	/* Git does not track empty, non-toplevel directories. */
	ఒకవేళ (S_ISDIR(బహుళకం) &&
	    is_ఖాళీలు_వృక్షం_oid(&oid, the_repository->hash_algo) &&
	    *మార్గం.buf) {
		వృక్షం_content_remove(&b->branch_వృక్షం, మార్గం.buf, ఖాళీ, 0);
		తిరిగి;
	}

	ఒకవేళ (S_ISGITLINK(బహుళకం)) {
		ఒకవేళ (inline_దత్తాంశం)
			die(_("Git links cannot be specified 'inline': %s"),
				command_buf.buf);
		లేకపోతే ఒకవేళ (oe) {
			ఒకవేళ (oe->type != OBJ_COMMIT)
				die(_("not a commit (actually a %s): %s"),
					type_name(oe->type), command_buf.buf);
		}
		/*
		 * Accept the sha1 without checking; it expected to be in
		 * another repository.
		 */
	} లేకపోతే ఒకవేళ (inline_దత్తాంశం) {
		ఒకవేళ (S_ISDIR(బహుళకం))
			die(_("directories cannot be specified 'inline': %s"),
				command_buf.buf);
		అయితే (read_తదుపరి_command(state) != EOF) {
			స్థిరరాశి అక్షరం *v;
			ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "cat-blob ", &v))
				parse_cat_blob(state, v);
			లేకపోతే {
				parse_and_store_blob(&చివరి_blob, &oid, 0);
				ఆపు;
			}
		}
	} లేకపోతే {
		గణన object_type expected = S_ISDIR(బహుళకం) ?
						OBJ_వృక్షం: OBJ_BLOB;
		గణన object_type type = oe ? oe->type :
					odb_read_object_info(the_repository->objects,
							     &oid, ఖాళీ);
		ఒకవేళ (type < 0)
			die(_("%s not found: %s"),
			    S_ISDIR(బహుళకం) ?  _("tree") : _("blob"),
			    command_buf.buf);
		ఒకవేళ (type != expected)
			die(_("not a %s (actually a %s): %s"),
				type_name(expected), type_name(type),
				command_buf.buf);
	}

	ఒకవేళ (!*మార్గం.buf) {
		వృక్షం_content_replace(&b->branch_వృక్షం, &oid, బహుళకం, ఖాళీ);
		తిరిగి;
	}

	ఒకవేళ (!verify_మార్గం(మార్గం.buf, బహుళకం))
		die(_("invalid path '%s'"), మార్గం.buf);
	వృక్షం_content_సమితి(&b->branch_వృక్షం, మార్గం.buf, &oid, బహుళకం, ఖాళీ);
}

స్థిర శూన్య file_change_d(స్థిరరాశి అక్షరం *p, నిర్మాణం branch *b)
{
	స్థిర నిర్మాణం strbuf మార్గం = STRBUF_INIT;

	strbuf_reset(&మార్గం);
	parse_మార్గం_eol(&మార్గం, p, "path");
	వృక్షం_content_remove(&b->branch_వృక్షం, మార్గం.buf, ఖాళీ, 1);
}

స్థిర శూన్య file_change_cr(స్థిరరాశి అక్షరం *p, నిర్మాణం branch *b, పూర్ణాంకం rename)
{
	స్థిర నిర్మాణం strbuf ఆధారం = STRBUF_INIT;
	స్థిర నిర్మాణం strbuf dest = STRBUF_INIT;
	నిర్మాణం వృక్షం_నమోదు ఆకు;

	strbuf_reset(&ఆధారం);
	parse_మార్గం_space(&ఆధారం, p, &p, "source");
	strbuf_reset(&dest);
	parse_మార్గం_eol(&dest, p, "dest");

	memset(&ఆకు, 0, పరిమాణం(ఆకు));
	ఒకవేళ (rename)
		వృక్షం_content_remove(&b->branch_వృక్షం, ఆధారం.buf, &ఆకు, 1);
	లేకపోతే
		వృక్షం_content_get(&b->branch_వృక్షం, ఆధారం.buf, &ఆకు, 1);
	ఒకవేళ (!ఆకు.versions[1].బహుళకం)
		die(_("path %s not in branch"), ఆధారం.buf);
	ఒకవేళ (!*dest.buf) {	/* C "path/to/subdir" "" */
		వృక్షం_content_replace(&b->branch_వృక్షం,
			&ఆకు.versions[1].oid,
			ఆకు.versions[1].బహుళకం,
			ఆకు.వృక్షం);
		తిరిగి;
	}
	ఒకవేళ (!verify_మార్గం(dest.buf, ఆకు.versions[1].బహుళకం))
		die(_("invalid path '%s'"), dest.buf);
	వృక్షం_content_సమితి(&b->branch_వృక్షం, dest.buf,
		&ఆకు.versions[1].oid,
		ఆకు.versions[1].బహుళకం,
		ఆకు.వృక్షం);
}

స్థిర శూన్య note_change_n(నిర్మాణం fast_import_state *state,
			  స్థిరరాశి అక్షరం *p,
			  నిర్మాణం branch *b,
			  గుర్తులేని అక్షరం *old_fanout)
{
	నిర్మాణం object_నమోదు *oe;
	నిర్మాణం branch *s;
	నిర్మాణం object_id oid, commit_oid;
	అక్షరం మార్గం[GIT_MAX_RAWSZ * 3];
	uint16_t inline_దత్తాంశం = 0;
	గుర్తులేని అక్షరం new_fanout;

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
	ఒకవేళ (b->అంకె_notes == 0 && *old_fanout == 0) {
		/* Invoke change_note_fanout() in "counting mode". */
		b->అంకె_notes = change_note_fanout(&b->branch_వృక్షం, 0xff);
		*old_fanout = convert_అంకె_notes_గమ్యం_fanout(b->అంకె_notes);
	}

	/* Now parse the notemodify command. */
	/* <dataref> or 'inline' */
	ఒకవేళ (*p == ':') {
		oe = కనుగొను_mark(marks, parse_mark_ref_space(&p));
		oidcpy(&oid, &oe->idx.oid);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(p, "inline ", &p)) {
		inline_దత్తాంశం = 1;
		oe = ఖాళీ; /* not used with inline_data, but makes gcc happy */
	} లేకపోతే {
		ఒకవేళ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		oe = కనుగొను_object(&oid);
		ఒకవేళ (*p++ != ' ')
			die(_("missing space after SHA1: %s"), command_buf.buf);
	}

	/* <commit-ish> */
	s = lookup_branch(p);
	ఒకవేళ (s) {
		ఒకవేళ (is_null_oid(&s->oid))
			die(_("can't add a note on empty branch."));
		oidcpy(&commit_oid, &s->oid);
	} లేకపోతే ఒకవేళ (*p == ':') {
		uintmax_t commit_mark = parse_mark_ref_eol(p);
		నిర్మాణం object_నమోదు *commit_oe = కనుగొను_mark(marks, commit_mark);
		ఒకవేళ (commit_oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), commit_mark);
		oidcpy(&commit_oid, &commit_oe->idx.oid);
	} లేకపోతే ఒకవేళ (!repo_get_oid(the_repository, p, &commit_oid)) {
		కొలత_t కొలత;
		అక్షరం *buf = odb_read_object_peeled(the_repository->objects,
						   &commit_oid, OBJ_COMMIT, &కొలత,
						   &commit_oid);
		ఒకవేళ (!buf || కొలత < the_hash_algo->hexsz + 6)
			die(_("not a valid commit: %s"), p);
		విడుదల(buf);
	} లేకపోతే
		die(_("invalid ref name or SHA1 expression: %s"), p);

	ఒకవేళ (inline_దత్తాంశం) {
		read_తదుపరి_command(state);
		parse_and_store_blob(&చివరి_blob, &oid, 0);
	} లేకపోతే ఒకవేళ (oe) {
		ఒకవేళ (oe->type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
				type_name(oe->type), command_buf.buf);
	} లేకపోతే ఒకవేళ (!is_null_oid(&oid)) {
		గణన object_type type = odb_read_object_info(the_repository->objects, &oid,
							ఖాళీ);
		ఒకవేళ (type < 0)
			die(_("blob not found: %s"), command_buf.buf);
		ఒకవేళ (type != OBJ_BLOB)
			die(_("not a blob (actually a %s): %s"),
			    type_name(type), command_buf.buf);
	}

	construct_మార్గం_with_fanout(oid_గమ్యం_hex(&commit_oid), *old_fanout, మార్గం);
	ఒకవేళ (వృక్షం_content_remove(&b->branch_వృక్షం, మార్గం, ఖాళీ, 0))
		b->అంకె_notes--;

	ఒకవేళ (is_null_oid(&oid))
		తిరిగి; /* nothing to insert */

	b->అంకె_notes++;
	new_fanout = convert_అంకె_notes_గమ్యం_fanout(b->అంకె_notes);
	construct_మార్గం_with_fanout(oid_గమ్యం_hex(&commit_oid), new_fanout, మార్గం);
	వృక్షం_content_సమితి(&b->branch_వృక్షం, మార్గం, &oid, S_IFREG | 0644, ఖాళీ);
}

స్థిర శూన్య file_change_deleteall(నిర్మాణం branch *b)
{
	release_వృక్షం_content_recursive(b->branch_వృక్షం.వృక్షం);
	oidclr(&b->branch_వృక్షం.versions[0].oid, the_repository->hash_algo);
	oidclr(&b->branch_వృక్షం.versions[1].oid, the_repository->hash_algo);
	ఎక్కించు_వృక్షం(&b->branch_వృక్షం);
	b->అంకె_notes = 0;
}

స్థిర శూన్య parse_from_commit(నిర్మాణం branch *b, అక్షరం *buf, గుర్తులేని దీర్ఘ కొలత)
{
	ఒకవేళ (!buf || కొలత < the_hash_algo->hexsz + 6)
		die(_("not a valid commit: %s"), oid_గమ్యం_hex(&b->oid));
	ఒకవేళ (memcmp("tree ", buf, 5)
		|| get_oid_hex(buf + 5, &b->branch_వృక్షం.versions[1].oid))
		die(_("the commit %s is corrupt"), oid_గమ్యం_hex(&b->oid));
	oidcpy(&b->branch_వృక్షం.versions[0].oid,
	       &b->branch_వృక్షం.versions[1].oid);
}

స్థిర శూన్య parse_from_existing(నిర్మాణం branch *b)
{
	ఒకవేళ (is_null_oid(&b->oid)) {
		oidclr(&b->branch_వృక్షం.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_వృక్షం.versions[1].oid, the_repository->hash_algo);
	} లేకపోతే {
		గుర్తులేని దీర్ఘ కొలత;
		కొలత_t కొలత_st = 0;
		అక్షరం *buf;

		buf = odb_read_object_peeled(the_repository->objects, &b->oid,
					     OBJ_COMMIT, &కొలత_st, &b->oid);
		కొలత = cast_కొలత_t_గమ్యం_ulong(కొలత_st);
		parse_from_commit(b, buf, కొలత);
		విడుదల(buf);
	}
}

స్థిర పూర్ణాంకం parse_objectish(నిర్మాణం fast_import_state *state,
			   నిర్మాణం branch *b,
			   స్థిరరాశి అక్షరం *objectish)
{
	నిర్మాణం branch *s;
	నిర్మాణం object_id oid;

	oidcpy(&oid, &b->branch_వృక్షం.versions[1].oid);

	s = lookup_branch(objectish);
	ఒకవేళ (b == s)
		die(_("can't create a branch from itself: %s"), b->name);
	లేకపోతే ఒకవేళ (s) {
		నిర్మాణం object_id *t = &s->branch_వృక్షం.versions[1].oid;
		oidcpy(&b->oid, &s->oid);
		oidcpy(&b->branch_వృక్షం.versions[0].oid, t);
		oidcpy(&b->branch_వృక్షం.versions[1].oid, t);
	} లేకపోతే ఒకవేళ (*objectish == ':') {
		uintmax_t idnum = parse_mark_ref_eol(objectish);
		నిర్మాణం object_నమోదు *oe = కనుగొను_mark(marks, idnum);
		ఒకవేళ (oe->type != OBJ_COMMIT)
			die(_("mark :%" PRIuMAX " not a commit"), idnum);
		ఒకవేళ (!oideq(&b->oid, &oe->idx.oid)) {
			oidcpy(&b->oid, &oe->idx.oid);
			ఒకవేళ (oe->pack_id != MAX_PACK_ID) {
				గుర్తులేని దీర్ఘ కొలత;
				అక్షరం *buf = gfi_unpack_నమోదు(oe, &కొలత);
				parse_from_commit(b, buf, కొలత);
				విడుదల(buf);
			} లేకపోతే
				parse_from_existing(b);
		}
	} లేకపోతే ఒకవేళ (!repo_get_oid(the_repository, objectish, &b->oid)) {
		parse_from_existing(b);
		ఒకవేళ (is_null_oid(&b->oid))
			b->delete = 1;
	}
	లేకపోతే
		die(_("invalid ref name or SHA1 expression: %s"), objectish);

	ఒకవేళ (b->branch_వృక్షం.వృక్షం && !oideq(&oid, &b->branch_వృక్షం.versions[1].oid)) {
		release_వృక్షం_content_recursive(b->branch_వృక్షం.వృక్షం);
		b->branch_వృక్షం.వృక్షం = ఖాళీ;
	}

	read_తదుపరి_command(state);
	తిరిగి 1;
}

స్థిర పూర్ణాంకం parse_from(నిర్మాణం fast_import_state *state, నిర్మాణం branch *b)
{
	స్థిరరాశి అక్షరం *from;

	ఒకవేళ (!skip_ఉపసర్గ(command_buf.buf, "from ", &from))
		తిరిగి 0;

	తిరిగి parse_objectish(state, b, from);
}

స్థిర పూర్ణాంకం parse_objectish_with_ఉపసర్గ(నిర్మాణం fast_import_state *state,
				       నిర్మాణం branch *b,
				       స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	స్థిరరాశి అక్షరం *పునాది;

	ఒకవేళ (!skip_ఉపసర్గ(command_buf.buf, ఉపసర్గ, &పునాది))
		తిరిగి 0;

	తిరిగి parse_objectish(state, b, పునాది);
}

స్థిర నిర్మాణం hash_list *parse_విలీనం(నిర్మాణం fast_import_state *state,
				     గుర్తులేని పూర్ణాంకం *లెక్క)
{
	నిర్మాణం hash_list *list = ఖాళీ, **తోక = &list, *n;
	స్థిరరాశి అక్షరం *from;
	నిర్మాణం branch *s;

	*లెక్క = 0;
	అయితే (skip_ఉపసర్గ(command_buf.buf, "merge ", &from)) {
		n = xmalloc(పరిమాణం(*n));
		s = lookup_branch(from);
		ఒకవేళ (s)
			oidcpy(&n->oid, &s->oid);
		లేకపోతే ఒకవేళ (*from == ':') {
			uintmax_t idnum = parse_mark_ref_eol(from);
			నిర్మాణం object_నమోదు *oe = కనుగొను_mark(marks, idnum);
			ఒకవేళ (oe->type != OBJ_COMMIT)
				die(_("mark :%" PRIuMAX " not a commit"), idnum);
			oidcpy(&n->oid, &oe->idx.oid);
		} లేకపోతే ఒకవేళ (!repo_get_oid(the_repository, from, &n->oid)) {
			కొలత_t కొలత;
			అక్షరం *buf = odb_read_object_peeled(the_repository->objects,
							   &n->oid, OBJ_COMMIT,
							   &కొలత, &n->oid);
			ఒకవేళ (!buf || కొలత < the_hash_algo->hexsz + 6)
				die(_("not a valid commit: %s"), from);
			విడుదల(buf);
		} లేకపోతే
			die(_("invalid ref name or SHA1 expression: %s"), from);

		n->తదుపరి = ఖాళీ;
		*తోక = n;
		తోక = &n->తదుపరి;

		(*లెక్క)++;
		read_తదుపరి_command(state);
	}
	తిరిగి list;
}

నిర్మాణం signature_దత్తాంశం {
	అక్షరం *hash_algo;      /* "sha1" or "sha256" */
	అక్షరం *sig_format;     /* "openpgp", "x509", "ssh", or "unknown" */
	నిర్మాణం strbuf దత్తాంశం;   /* The actual signature data */
};

స్థిర శూన్య parse_one_signature(నిర్మాణం fast_import_state *state,
				నిర్మాణం signature_దత్తాంశం *sig,
				స్థిరరాశి అక్షరం *v)
{
	అక్షరం *వాదనలు = xstrdup(v); /* Will be freed when sig->hash_algo is freed */
	అక్షరం *space = strchr(వాదనలు, ' ');

	ఒకవేళ (!space)
		die(_("expected gpgsig format: 'gpgsig <hash-algo> <signature-format>', "
		      "got 'gpgsig %s'"), వాదనలు);
	*space = '\0';

	sig->hash_algo = వాదనలు;
	sig->sig_format = space + 1;

	/* Validate hash algorithm */
	ఒకవేళ (strcmp(sig->hash_algo, "sha1") &&
	    strcmp(sig->hash_algo, "sha256"))
		die(_("unknown git hash algorithm in gpgsig: '%s'"), sig->hash_algo);

	/* Validate signature format */
	ఒకవేళ (!valid_signature_format(sig->sig_format))
		die(_("invalid signature format in gpgsig: '%s'"), sig->sig_format);
	ఒకవేళ (!strcmp(sig->sig_format, "unknown"))
		warning(_("'unknown' signature format in gpgsig"));

	/* Read signature data */
	read_తదుపరి_command(state);
	parse_దత్తాంశం(&sig->దత్తాంశం, 0, ఖాళీ);
}

స్థిర శూన్య discard_one_signature(నిర్మాణం fast_import_state *state)
{
	నిర్మాణం strbuf దత్తాంశం = STRBUF_INIT;

	read_తదుపరి_command(state);
	parse_దత్తాంశం(&దత్తాంశం, 0, ఖాళీ);
	strbuf_release(&దత్తాంశం);
}

స్థిర శూన్య కూడు_gpgsig_గమ్యం_commit(నిర్మాణం strbuf *commit_దత్తాంశం,
				 స్థిరరాశి అక్షరం *శీర్షిక,
				 నిర్మాణం signature_దత్తాంశం *sig)
{
	నిర్మాణం string_list siglines = STRING_LIST_INIT_NODUP;

	ఒకవేళ (!sig || !sig->hash_algo)
		తిరిగి;

	strbuf_addstr(commit_దత్తాంశం, శీర్షిక);
	string_list_విభజించు_లోపల_ఉంచు(&siglines, sig->దత్తాంశం.buf, "\n", -1);
	strbuf_కూడు_separated_string_list(commit_దత్తాంశం, "\n ", &siglines);
	strbuf_addch(commit_దత్తాంశం, '\n');
	string_list_clear(&siglines, 1);
	strbuf_release(&sig->దత్తాంశం);
	విడుదల(sig->hash_algo);
}

స్థిర శూన్య store_signature(నిర్మాణం signature_దత్తాంశం *stored_sig,
			    నిర్మాణం signature_దత్తాంశం *new_sig,
			    స్థిరరాశి అక్షరం *hash_type)
{
	ఒకవేళ (stored_sig->hash_algo) {
		warning(_("multiple %s signatures found, "
			  "ignoring additional signature"),
			hash_type);
		strbuf_release(&new_sig->దత్తాంశం);
		విడుదల(new_sig->hash_algo);
	} లేకపోతే {
		*stored_sig = *new_sig;
	}
}

స్థిర శూన్య import_one_signature(నిర్మాణం fast_import_state *state,
				 నిర్మాణం signature_దత్తాంశం *sig_sha1,
				 నిర్మాణం signature_దత్తాంశం *sig_sha256,
				 స్థిరరాశి అక్షరం *v)
{
	నిర్మాణం signature_దత్తాంశం sig = { ఖాళీ, ఖాళీ, STRBUF_INIT };

	parse_one_signature(state, &sig, v);

	ఒకవేళ (!strcmp(sig.hash_algo, "sha1"))
		store_signature(sig_sha1, &sig, "SHA-1");
	లేకపోతే ఒకవేళ (!strcmp(sig.hash_algo, "sha256"))
		store_signature(sig_sha256, &sig, "SHA-256");
	లేకపోతే
		die(_("parse_one_signature() returned unknown hash algo"));
}

స్థిర శూన్య finalize_commit_బఫరు(నిర్మాణం strbuf *new_దత్తాంశం,
				   నిర్మాణం signature_దత్తాంశం *sig_sha1,
				   నిర్మాణం signature_దత్తాంశం *sig_sha256,
				   నిర్మాణం strbuf *సందేశం)
{
	కూడు_gpgsig_గమ్యం_commit(new_దత్తాంశం, "gpgsig ", sig_sha1);
	కూడు_gpgsig_గమ్యం_commit(new_దత్తాంశం, "gpgsig-sha256 ", sig_sha256);

	strbuf_addch(new_దత్తాంశం, '\n');
	strbuf_addbuf(new_దత్తాంశం, సందేశం);
}

స్థిర శూన్య warn_invalid_signature(నిర్మాణం signature_check *check,
				   స్థిరరాశి అక్షరం *సందేశం, గణన sign_బహుళకం బహుళకం)
{
	స్థిరరాశి అక్షరం *signer = check->signer ? check->signer : _("unknown");
	స్థిరరాశి అక్షరం *subject;
	పూర్ణాంకం subject_len = కనుగొను_commit_subject(సందేశం, &subject);

	ఎంపిక (బహుళకం) {
	స్థితి SIGN_STRIP_IF_INVALID:
		ఒకవేళ (subject_len > 100)
			warning(_("stripping invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		లేకపోతే ఒకవేళ (subject_len > 0)
			warning(_("stripping invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		లేకపోతే
			warning(_("stripping invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		ఆపు;
	స్థితి SIGN_SIGN_IF_INVALID:
		ఒకవేళ (subject_len > 100)
			warning(_("replacing invalid signature for commit '%.100s...'\n"
				  "  allegedly by %s"), subject, signer);
		లేకపోతే ఒకవేళ (subject_len > 0)
			warning(_("replacing invalid signature for commit '%.*s'\n"
				  "  allegedly by %s"), subject_len, subject, signer);
		లేకపోతే
			warning(_("replacing invalid signature for commit\n"
				  "  allegedly by %s"), signer);
		ఆపు;
	అప్రమేయ:
		BUG("unsupported signing mode");
	}
}

స్థిర శూన్య handle_signature_if_invalid(నిర్మాణం strbuf *new_దత్తాంశం,
					నిర్మాణం signature_దత్తాంశం *sig_sha1,
					నిర్మాణం signature_దత్తాంశం *sig_sha256,
					నిర్మాణం strbuf *సందేశం,
					గణన sign_బహుళకం బహుళకం)
{
	నిర్మాణం strbuf tmp_buf = STRBUF_INIT;
	నిర్మాణం signature_check signature_check = { 0 };
	పూర్ణాంకం ret;

	/* Check signature in a temporary commit buffer */
	strbuf_addbuf(&tmp_buf, new_దత్తాంశం);
	finalize_commit_బఫరు(&tmp_buf, sig_sha1, sig_sha256, సందేశం);
	ret = verify_commit_బఫరు(tmp_buf.buf, tmp_buf.len, &signature_check);

	ఒకవేళ (ret) {
		ఒకవేళ (బహుళకం == SIGN_ABORT_IF_INVALID)
			die(_("aborting due to invalid signature"));

		warn_invalid_signature(&signature_check, సందేశం->buf, బహుళకం);

		ఒకవేళ (బహుళకం == SIGN_SIGN_IF_INVALID) {
			నిర్మాణం strbuf signature = STRBUF_INIT;
			నిర్మాణం strbuf payload = STRBUF_INIT;

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
			ఒకవేళ (the_repository->compat_hash_algo)
				die(_("signing commits in interoperability mode is unsupported"));

			strbuf_addstr(&payload, signature_check.payload);
			ఒకవేళ (sign_బఫరు(&payload, &signature, signed_commit_keyid,
					SIGN_బఫరు_USE_DEFAULT_కీలకం))
				die(_("failed to sign commit object"));
			కూడు_శీర్షిక_signature(new_దత్తాంశం, &signature, the_hash_algo);

			strbuf_release(&signature);
			strbuf_release(&payload);
		}

		finalize_commit_బఫరు(new_దత్తాంశం, ఖాళీ, ఖాళీ, సందేశం);
	} లేకపోతే {
		strbuf_మార్పిడి(new_దత్తాంశం, &tmp_buf);
	}

	signature_check_clear(&signature_check);
	strbuf_release(&tmp_buf);
}

స్థిర శూన్య parse_new_commit(నిర్మాణం fast_import_state *state, స్థిరరాశి అక్షరం *arg)
{
	స్థిర నిర్మాణం strbuf సందేశం = STRBUF_INIT;
	నిర్మాణం signature_దత్తాంశం sig_sha1 = { ఖాళీ, ఖాళీ, STRBUF_INIT };
	నిర్మాణం signature_దత్తాంశం sig_sha256 = { ఖాళీ, ఖాళీ, STRBUF_INIT };
	నిర్మాణం branch *b;
	అక్షరం *author = ఖాళీ;
	అక్షరం *committer = ఖాళీ;
	అక్షరం *encoding = ఖాళీ;
	నిర్మాణం hash_list *విలీనం_list = ఖాళీ;
	గుర్తులేని పూర్ణాంకం విలీనం_లెక్క;
	గుర్తులేని అక్షరం prev_fanout, new_fanout;
	స్థిరరాశి అక్షరం *v;

	b = lookup_branch(arg);
	ఒకవేళ (!b)
		b = new_branch(arg);

	read_తదుపరి_command(state);
	parse_mark(state);
	parse_original_identifier(state);
	ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "author ", &v)) {
		author = parse_ident(v);
		read_తదుపరి_command(state);
	}
	ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "committer ", &v)) {
		committer = parse_ident(v);
		read_తదుపరి_command(state);
	}
	ఒకవేళ (!committer)
		die(_("expected committer but didn't get one"));

	అయితే (skip_ఉపసర్గ(command_buf.buf, "gpgsig ", &v)) {
		ఎంపిక (signed_commit_బహుళకం) {

		/* First, modes that don't need the signature to be parsed */
		స్థితి SIGN_ABORT:
			die(_("encountered signed commit; use "
			      "--signed-commits=<mode> to handle it"));
		స్థితి SIGN_WARN_STRIP:
			warning(_("stripping a commit signature"));
			/* fallthru */
		స్థితి SIGN_STRIP:
			discard_one_signature(state);
			ఆపు;

		/* Second, modes that parse the signature */
		స్థితి SIGN_WARN_VERBATIM:
			warning(_("importing a commit signature verbatim"));
			/* fallthru */
		స్థితి SIGN_VERBATIM:
		స్థితి SIGN_STRIP_IF_INVALID:
		స్థితి SIGN_SIGN_IF_INVALID:
		స్థితి SIGN_ABORT_IF_INVALID:
			import_one_signature(state, &sig_sha1, &sig_sha256, v);
			ఆపు;

		/* Third, BUG */
		అప్రమేయ:
			BUG("invalid signed_commit_mode value %d", signed_commit_బహుళకం);
		}
		read_తదుపరి_command(state);
	}

	ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "encoding ", &v)) {
		encoding = xstrdup(v);
		read_తదుపరి_command(state);
	}
	parse_దత్తాంశం(&సందేశం, 0, ఖాళీ);
	read_తదుపరి_command(state);
	parse_from(state, b);
	విలీనం_list = parse_విలీనం(state, &విలీనం_లెక్క);

	/* ensure the branch is active/loaded */
	ఒకవేళ (!b->branch_వృక్షం.వృక్షం || !max_చురుకైన_branches) {
		unload_one_branch();
		ఎక్కించు_branch(b);
	}

	prev_fanout = convert_అంకె_notes_గమ్యం_fanout(b->అంకె_notes);

	/* file_change* */
	అయితే (command_buf.len > 0) {
		ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "M ", &v))
			file_change_m(state, v, b);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "D ", &v))
			file_change_d(v, b);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "R ", &v))
			file_change_cr(v, b, 1);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "C ", &v))
			file_change_cr(v, b, 0);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "N ", &v))
			note_change_n(state, v, b, &prev_fanout);
		లేకపోతే ఒకవేళ (!strcmp("deleteall", command_buf.buf))
			file_change_deleteall(b);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "ls ", &v))
			parse_ls(state, v, b);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(state, v);
		లేకపోతే {
			unread_command_buf = 1;
			ఆపు;
		}
		ఒకవేళ (read_తదుపరి_command(state) == EOF)
			ఆపు;
	}

	new_fanout = convert_అంకె_notes_గమ్యం_fanout(b->అంకె_notes);
	ఒకవేళ (new_fanout != prev_fanout)
		b->అంకె_notes = change_note_fanout(&b->branch_వృక్షం, new_fanout);

	/* build the tree and the commit */
	store_వృక్షం(&b->branch_వృక్షం);
	oidcpy(&b->branch_వృక్షం.versions[0].oid,
	       &b->branch_వృక్షం.versions[1].oid);

	strbuf_reset(&new_దత్తాంశం);
	strbuf_addf(&new_దత్తాంశం, "tree %s\n",
		oid_గమ్యం_hex(&b->branch_వృక్షం.versions[1].oid));
	ఒకవేళ (!is_null_oid(&b->oid))
		strbuf_addf(&new_దత్తాంశం, "parent %s\n",
			    oid_గమ్యం_hex(&b->oid));
	అయితే (విలీనం_list) {
		నిర్మాణం hash_list *తదుపరి = విలీనం_list->తదుపరి;
		strbuf_addf(&new_దత్తాంశం, "parent %s\n",
			    oid_గమ్యం_hex(&విలీనం_list->oid));
		విడుదల(విలీనం_list);
		విలీనం_list = తదుపరి;
	}
	strbuf_addf(&new_దత్తాంశం,
		"author %s\n"
		"committer %s\n",
		author ? author : committer, committer);
	ఒకవేళ (encoding)
		strbuf_addf(&new_దత్తాంశం,
			"encoding %s\n",
			encoding);

	ఒకవేళ ((signed_commit_బహుళకం == SIGN_STRIP_IF_INVALID ||
	     signed_commit_బహుళకం == SIGN_SIGN_IF_INVALID ||
	     signed_commit_బహుళకం == SIGN_ABORT_IF_INVALID) &&
	    (sig_sha1.hash_algo || sig_sha256.hash_algo))
		handle_signature_if_invalid(&new_దత్తాంశం, &sig_sha1, &sig_sha256,
					    &సందేశం, signed_commit_బహుళకం);
	లేకపోతే
		finalize_commit_బఫరు(&new_దత్తాంశం, &sig_sha1, &sig_sha256, &సందేశం);

	విడుదల(author);
	విడుదల(committer);
	విడుదల(encoding);

	ఒకవేళ (!store_object(OBJ_COMMIT, &new_దత్తాంశం, ఖాళీ, &b->oid, తదుపరి_mark))
		b->pack_id = pack_id;
	b->చివరి_commit = object_లెక్క_by_type[OBJ_COMMIT];
}

స్థిర శూన్య handle_ట్యాగ్_signature_if_invalid(నిర్మాణం strbuf *buf,
					    నిర్మాణం strbuf *సందేశం,
					    కొలత_t sig_స్థానభ్రంశం)
{
	నిర్మాణం strbuf signature = STRBUF_INIT;
	నిర్మాణం strbuf payload = STRBUF_INIT;
	నిర్మాణం signature_check sigc = { 0 };

	strbuf_addbuf(&payload, buf);
	strbuf_addch(&payload, '\n');
	strbuf_కూడు(&payload, సందేశం->buf, sig_స్థానభ్రంశం);
	strbuf_కూడు(&signature, సందేశం->buf + sig_స్థానభ్రంశం, సందేశం->len - sig_స్థానభ్రంశం);

	sigc.payload_type = SIGNATURE_PAYLOAD_ట్యాగ్;
	sigc.payload = strbuf_detach(&payload, &sigc.payload_len);

	ఒకవేళ (!check_signature(&sigc, signature.buf, signature.len))
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (signed_ట్యాగ్_బహుళకం == SIGN_ABORT_IF_INVALID)
		die(_("aborting due to invalid signature"));

	strbuf_setlen(సందేశం, sig_స్థానభ్రంశం);

	ఒకవేళ (signed_ట్యాగ్_బహుళకం == SIGN_SIGN_IF_INVALID) {
		strbuf_attach(&payload, sigc.payload, sigc.payload_len,
			      sigc.payload_len + 1);
		sigc.payload = ఖాళీ;
		strbuf_reset(&signature);

		ఒకవేళ (sign_బఫరు(&payload, &signature, signed_ట్యాగ్_keyid,
				SIGN_బఫరు_USE_DEFAULT_కీలకం))
			die(_("failed to sign tag object"));

		strbuf_addbuf(సందేశం, &signature);
	}

బహిర్గతం:
	signature_check_clear(&sigc);
	strbuf_release(&signature);
	strbuf_release(&payload);
}

స్థిర శూన్య handle_ట్యాగ్_signature(నిర్మాణం strbuf *buf, నిర్మాణం strbuf *సందేశం, స్థిరరాశి అక్షరం *name)
{
	కొలత_t sig_స్థానభ్రంశం = parse_signed_బఫరు(సందేశం->buf, సందేశం->len);

	/* If there is no signature, there is nothing to do. */
	ఒకవేళ (sig_స్థానభ్రంశం >= సందేశం->len)
		తిరిగి;

	ఎంపిక (signed_ట్యాగ్_బహుళకం) {

	/* First, modes that don't change anything */
	స్థితి SIGN_WARN_VERBATIM:
		warning(_("importing a tag signature verbatim for tag '%s'"), name);
		/* fallthru */
	స్థితి SIGN_VERBATIM:
		/* Nothing to do, the signature will be put into the imported tag. */
		ఆపు;

	/* Second, modes that remove the signature */
	స్థితి SIGN_WARN_STRIP:
		warning(_("stripping a tag signature for tag '%s'"), name);
		/* fallthru */
	స్థితి SIGN_STRIP:
		/* Truncate the buffer to remove the signature */
		strbuf_setlen(సందేశం, sig_స్థానభ్రంశం);
		ఆపు;
	స్థితి SIGN_ABORT_IF_INVALID:
	స్థితి SIGN_SIGN_IF_INVALID:
	స్థితి SIGN_STRIP_IF_INVALID:
		handle_ట్యాగ్_signature_if_invalid(buf, సందేశం, sig_స్థానభ్రంశం);
		ఆపు;

	/* Third, aborting modes */
	స్థితి SIGN_ABORT:
		die(_("encountered signed tag; use "
		      "--signed-tags=<mode> to handle it"));
	అప్రమేయ:
		BUG("invalid signed_tag_mode value %d from tag '%s'",
		    signed_ట్యాగ్_బహుళకం, name);
	}
}

స్థిర శూన్య parse_new_ట్యాగ్(నిర్మాణం fast_import_state *state, స్థిరరాశి అక్షరం *arg)
{
	స్థిర నిర్మాణం strbuf సందేశం = STRBUF_INIT;
	స్థిరరాశి అక్షరం *from;
	అక్షరం *tagger;
	నిర్మాణం branch *s;
	నిర్మాణం ట్యాగ్ *t;
	uintmax_t from_mark = 0;
	నిర్మాణం object_id oid;
	గణన object_type type;
	స్థిరరాశి అక్షరం *v;

	t = mem_pool_calloc(&fi_mem_pool, 1, పరిమాణం(నిర్మాణం ట్యాగ్));
	t->name = mem_pool_strdup(&fi_mem_pool, arg);
	ఒకవేళ (చివరి_ట్యాగ్)
		చివరి_ట్యాగ్->తదుపరి_ట్యాగ్ = t;
	లేకపోతే
		మొదటి_ట్యాగ్ = t;
	చివరి_ట్యాగ్ = t;
	read_తదుపరి_command(state);
	parse_mark(state);

	/* from ... */
	ఒకవేళ (!skip_ఉపసర్గ(command_buf.buf, "from ", &from))
		die(_("expected 'from' command, got '%s'"), command_buf.buf);
	s = lookup_branch(from);
	ఒకవేళ (s) {
		ఒకవేళ (is_null_oid(&s->oid))
			die(_("can't tag an empty branch."));
		oidcpy(&oid, &s->oid);
		type = OBJ_COMMIT;
	} లేకపోతే ఒకవేళ (*from == ':') {
		నిర్మాణం object_నమోదు *oe;
		from_mark = parse_mark_ref_eol(from);
		oe = కనుగొను_mark(marks, from_mark);
		type = oe->type;
		oidcpy(&oid, &oe->idx.oid);
	} లేకపోతే ఒకవేళ (!repo_get_oid(the_repository, from, &oid)) {
		నిర్మాణం object_నమోదు *oe = కనుగొను_object(&oid);
		ఒకవేళ (!oe) {
			type = odb_read_object_info(the_repository->objects,
						    &oid, ఖాళీ);
			ఒకవేళ (type < 0)
				die(_("not a valid object: %s"), from);
		} లేకపోతే
			type = oe->type;
	} లేకపోతే
		die(_("invalid ref name or SHA1 expression: %s"), from);
	read_తదుపరి_command(state);

	/* original-oid ... */
	parse_original_identifier(state);

	/* tagger ... */
	ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "tagger ", &v)) {
		tagger = parse_ident(v);
		read_తదుపరి_command(state);
	} లేకపోతే
		tagger = ఖాళీ;

	/* tag payload/message */
	parse_దత్తాంశం(&సందేశం, 0, ఖాళీ);

	/* build the tag object */
	strbuf_reset(&new_దత్తాంశం);

	strbuf_addf(&new_దత్తాంశం,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n",
		    oid_గమ్యం_hex(&oid), type_name(type), t->name);
	ఒకవేళ (tagger)
		strbuf_addf(&new_దత్తాంశం,
			    "tagger %s\n", tagger);

	handle_ట్యాగ్_signature(&new_దత్తాంశం, &సందేశం, t->name);

	strbuf_addch(&new_దత్తాంశం, '\n');
	strbuf_addbuf(&new_దత్తాంశం, &సందేశం);
	విడుదల(tagger);

	ఒకవేళ (store_object(OBJ_ట్యాగ్, &new_దత్తాంశం, ఖాళీ, &t->oid, తదుపరి_mark))
		t->pack_id = MAX_PACK_ID;
	లేకపోతే
		t->pack_id = pack_id;
}

స్థిర శూన్య parse_reset_branch(నిర్మాణం fast_import_state *state, స్థిరరాశి అక్షరం *arg)
{
	నిర్మాణం branch *b;
	స్థిరరాశి అక్షరం *ట్యాగ్_name;

	b = lookup_branch(arg);
	ఒకవేళ (b) {
		oidclr(&b->oid, the_repository->hash_algo);
		oidclr(&b->branch_వృక్షం.versions[0].oid, the_repository->hash_algo);
		oidclr(&b->branch_వృక్షం.versions[1].oid, the_repository->hash_algo);
		ఒకవేళ (b->branch_వృక్షం.వృక్షం) {
			release_వృక్షం_content_recursive(b->branch_వృక్షం.వృక్షం);
			b->branch_వృక్షం.వృక్షం = ఖాళీ;
		}
	}
	లేకపోతే
		b = new_branch(arg);
	read_తదుపరి_command(state);
	parse_from(state, b);
	ఒకవేళ (b->delete && skip_ఉపసర్గ(b->name, "refs/tags/", &ట్యాగ్_name)) {
		/*
		 * Elsewhere, we call dump_branches() before dump_tags(),
		 * and dump_branches() will handle ref deletions first, so
		 * in order to make sure the deletion actually takes effect,
		 * we need to remove the tag from our list of tags to update.
		 *
		 * NEEDSWORK: replace list of tags with hashmap for faster
		 * deletion?
		 */
		నిర్మాణం ట్యాగ్ *t, *prev = ఖాళీ;
		కొరకు (t = మొదటి_ట్యాగ్; t; t = t->తదుపరి_ట్యాగ్) {
			ఒకవేళ (!strcmp(t->name, ట్యాగ్_name))
				ఆపు;
			prev = t;
		}
		ఒకవేళ (t) {
			ఒకవేళ (prev)
				prev->తదుపరి_ట్యాగ్ = t->తదుపరి_ట్యాగ్;
			లేకపోతే
				మొదటి_ట్యాగ్ = t->తదుపరి_ట్యాగ్;
			ఒకవేళ (!t->తదుపరి_ట్యాగ్)
				చివరి_ట్యాగ్ = prev;
			/* There is no mem_pool_free(t) function to call. */
		}
	}
	ఒకవేళ (command_buf.len > 0)
		unread_command_buf = 1;
}

స్థిర శూన్య cat_blob_write(స్థిరరాశి అక్షరం *buf, గుర్తులేని దీర్ఘ కొలత)
{
	ఒకవేళ (write_లోపల_నిండిన(cat_blob_fd, buf, కొలత) < 0)
		die_errno(_("write to frontend failed"));
}

స్థిర శూన్య cat_blob(నిర్మాణం object_నమోదు *oe, నిర్మాణం object_id *oid)
{
	నిర్మాణం strbuf పంక్తి = STRBUF_INIT;
	నిర్మాణం iovec iov[3];
	గుర్తులేని దీర్ఘ కొలత;
	గణన object_type type = 0;
	అక్షరం *buf;

	ఒకవేళ (!oe || oe->pack_id == MAX_PACK_ID) {
		కొలత_t కొలత_st = 0;
		buf = odb_read_object(the_repository->objects, oid, &type,
				      &కొలత_st);
		కొలత = cast_కొలత_t_గమ్యం_ulong(కొలత_st);
	} లేకపోతే {
		type = oe->type;
		buf = gfi_unpack_నమోదు(oe, &కొలత);
	}

	/*
	 * Output based on batch_one_object() from cat-file.c.
	 */
	ఒకవేళ (type <= 0) {
		strbuf_reset(&పంక్తి);
		strbuf_addf(&పంక్తి, "%s missing\n", oid_గమ్యం_hex(oid));
		cat_blob_write(పంక్తి.buf, పంక్తి.len);
		strbuf_release(&పంక్తి);
		విడుదల(buf);
		తిరిగి;
	}
	ఒకవేళ (!buf)
		die(_("can't read object %s"), oid_గమ్యం_hex(oid));
	ఒకవేళ (type != OBJ_BLOB)
		die(_("object %s is a %s but a blob was expected."),
		    oid_గమ్యం_hex(oid), type_name(type));
	strbuf_reset(&పంక్తి);
	strbuf_addf(&పంక్తి, "%s %s %"PRIuMAX"\n", oid_గమ్యం_hex(oid),
		    type_name(type), (uintmax_t)కొలత);

	/*
	 * Write the header, the payload and the trailing newline with a
	 * single writev(3p) call instead of three separate write(3p) calls.
	 */
	iov[0].iov_పునాది = పంక్తి.buf;
	iov[0].iov_len = పంక్తి.len;
	iov[1].iov_పునాది = buf;
	iov[1].iov_len = కొలత;
	iov[2].iov_పునాది = (శూన్య *) "\n";
	iov[2].iov_len = 1;

	ఒకవేళ (writev_లోపల_నిండిన(cat_blob_fd, iov, ARRAY_కొలత(iov)) < 0)
		die_errno(_("write to frontend failed"));
	strbuf_release(&పంక్తి);
	ఒకవేళ (oe && oe->pack_id == pack_id) {
		చివరి_blob.స్థానభ్రంశం = oe->idx.స్థానభ్రంశం;
		strbuf_attach(&చివరి_blob.దత్తాంశం, buf, కొలత, కొలత + 1);
		చివరి_blob.లోతు = oe->లోతు;
	} లేకపోతే
		విడుదల(buf);
}

స్థిర శూన్య parse_get_mark(నిర్మాణం fast_import_state *state వాడని,
			   స్థిరరాశి అక్షరం *p)
{
	నిర్మాణం object_నమోదు *oe;
	అక్షరం output[GIT_MAX_HEXSZ + 2];

	/* get-mark SP <object> LF */
	ఒకవేళ (*p != ':')
		die(_("not a mark: %s"), p);

	oe = కనుగొను_mark(marks, parse_mark_ref_eol(p));
	ఒకవేళ (!oe)
		die(_("unknown mark: %s"), command_buf.buf);

	xsnprintf(output, పరిమాణం(output), "%s\n", oid_గమ్యం_hex(&oe->idx.oid));
	cat_blob_write(output, the_hash_algo->hexsz + 1);
}

స్థిర శూన్య parse_cat_blob(నిర్మాణం fast_import_state *state వాడని,
			   స్థిరరాశి అక్షరం *p)
{
	నిర్మాణం object_నమోదు *oe;
	నిర్మాణం object_id oid;

	/* cat-blob SP <object> LF */
	ఒకవేళ (*p == ':') {
		oe = కనుగొను_mark(marks, parse_mark_ref_eol(p));
		ఒకవేళ (!oe)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &oe->idx.oid);
	} లేకపోతే {
		ఒకవేళ (parse_mapped_oid_hex(p, &oid, &p))
			die(_("invalid dataref: %s"), command_buf.buf);
		ఒకవేళ (*p)
			die(_("garbage after SHA1: %s"), command_buf.buf);
		oe = కనుగొను_object(&oid);
	}

	cat_blob(oe, &oid);
}

స్థిర నిర్మాణం object_నమోదు *dereference(నిర్మాణం object_నమోదు *oe,
					నిర్మాణం object_id *oid)
{
	గుర్తులేని దీర్ఘ కొలత;
	అక్షరం *buf = ఖాళీ;
	స్థిరరాశి గుర్తులేని hexsz = the_hash_algo->hexsz;

	ఒకవేళ (!oe) {
		గణన object_type type = odb_read_object_info(the_repository->objects,
							     oid, ఖాళీ);
		ఒకవేళ (type < 0)
			die(_("object not found: %s"), oid_గమ్యం_hex(oid));
		/* cache it! */
		oe = చొప్పించు_object(oid);
		oe->type = type;
		oe->pack_id = MAX_PACK_ID;
		oe->idx.స్థానభ్రంశం = 1;
	}
	ఎంపిక (oe->type) {
	స్థితి OBJ_వృక్షం:	/* easy case. */
		తిరిగి oe;
	స్థితి OBJ_COMMIT:
	స్థితి OBJ_ట్యాగ్:
		ఆపు;
	అప్రమేయ:
		die(_("not a tree-ish: %s"), command_buf.buf);
	}

	ఒకవేళ (oe->pack_id != MAX_PACK_ID) {	/* in a pack being written */
		buf = gfi_unpack_నమోదు(oe, &కొలత);
	} లేకపోతే {
		గణన object_type వాడని_2;
		కొలత_t కొలత_st = 0;
		buf = odb_read_object(the_repository->objects, oid,
				      &వాడని_2, &కొలత_st);
		కొలత = cast_కొలత_t_గమ్యం_ulong(కొలత_st);
	}
	ఒకవేళ (!buf)
		die(_("can't load object %s"), oid_గమ్యం_hex(oid));

	/* Peel one layer. */
	ఎంపిక (oe->type) {
	స్థితి OBJ_ట్యాగ్:
		ఒకవేళ (కొలత < hexsz + అక్షరశ్రేణి_పొడవు("object ") ||
		    get_oid_hex(buf + అక్షరశ్రేణి_పొడవు("object "), oid))
			die(_("invalid SHA1 in tag: %s"), command_buf.buf);
		ఆపు;
	స్థితి OBJ_COMMIT:
		ఒకవేళ (కొలత < hexsz + అక్షరశ్రేణి_పొడవు("tree ") ||
		    get_oid_hex(buf + అక్షరశ్రేణి_పొడవు("tree "), oid))
			die(_("invalid SHA1 in commit: %s"), command_buf.buf);
	}

	విడుదల(buf);
	తిరిగి కనుగొను_object(oid);
}

స్థిర శూన్య చొప్పించు_mapped_mark(uintmax_t mark, శూన్య *object, శూన్య *cbp)
{
	నిర్మాణం object_id *fromoid = object;
	నిర్మాణం object_id *tooid = కనుగొను_mark(cbp, mark);
	పూర్ణాంకం ret;
	khiter_t it;

	it = kh_put_oid_map(sub_oid_map, *fromoid, &ret);
	/* We've already seen this object. */
	ఒకవేళ (ret == 0)
		తిరిగి;
	kh_విలువ(sub_oid_map, it) = tooid;
}

స్థిర శూన్య build_mark_map_one(నిర్మాణం mark_సమితి *from, నిర్మాణం mark_సమితి *గమ్యం)
{
	for_each_mark(from, 0, చొప్పించు_mapped_mark, గమ్యం);
}

స్థిర శూన్య build_mark_map(నిర్మాణం string_list *from, నిర్మాణం string_list *గమ్యం)
{
	నిర్మాణం string_list_అంశం *fromp, *శిఖరం;

	sub_oid_map = kh_init_oid_map();

	for_each_string_list_అంశం(fromp, from) {
		శిఖరం = string_list_lookup(గమ్యం, fromp->string);
		ఒకవేళ (!fromp->util) {
			die(_("missing from marks for submodule '%s'"), fromp->string);
		} లేకపోతే ఒకవేళ (!శిఖరం || !శిఖరం->util) {
			die(_("missing to marks for submodule '%s'"), fromp->string);
		}
		build_mark_map_one(fromp->util, శిఖరం->util);
	}
}

స్థిర నిర్మాణం object_నమోదు *parse_treeish_dataref(స్థిరరాశి అక్షరం **p)
{
	నిర్మాణం object_id oid;
	నిర్మాణం object_నమోదు *e;

	ఒకవేళ (**p == ':') {	/* <mark> */
		e = కనుగొను_mark(marks, parse_mark_ref_space(p));
		ఒకవేళ (!e)
			die(_("unknown mark: %s"), command_buf.buf);
		oidcpy(&oid, &e->idx.oid);
	} లేకపోతే {	/* <sha1> */
		ఒకవేళ (parse_mapped_oid_hex(*p, &oid, p))
			die(_("invalid dataref: %s"), command_buf.buf);
		e = కనుగొను_object(&oid);
		ఒకవేళ (*(*p)++ != ' ')
			die(_("missing space after tree-ish: %s"), command_buf.buf);
	}

	అయితే (!e || e->type != OBJ_వృక్షం)
		e = dereference(e, &oid);
	తిరిగి e;
}

స్థిర శూన్య print_ls(పూర్ణాంకం బహుళకం, స్థిరరాశి గుర్తులేని అక్షరం *hash, స్థిరరాశి అక్షరం *మార్గం)
{
	స్థిర నిర్మాణం strbuf పంక్తి = STRBUF_INIT;

	/* See show_tree(). */
	స్థిరరాశి అక్షరం *type =
		S_ISGITLINK(బహుళకం) ? commit_type :
		S_ISDIR(బహుళకం) ? వృక్షం_type :
		blob_type;

	ఒకవేళ (!బహుళకం) {
		/* missing SP path LF */
		strbuf_reset(&పంక్తి);
		strbuf_addstr(&పంక్తి, "missing ");
		quote_c_style(మార్గం, &పంక్తి, ఖాళీ, 0);
		strbuf_addch(&పంక్తి, '\n');
	} లేకపోతే {
		/* mode SP type SP object_name TAB path LF */
		strbuf_reset(&పంక్తి);
		strbuf_addf(&పంక్తి, "%06o %s %s\t",
				బహుళకం & ~NO_DELTA, type, hash_గమ్యం_hex(hash));
		quote_c_style(మార్గం, &పంక్తి, ఖాళీ, 0);
		strbuf_addch(&పంక్తి, '\n');
	}
	cat_blob_write(పంక్తి.buf, పంక్తి.len);
}

స్థిర శూన్య parse_ls(నిర్మాణం fast_import_state *state వాడని,
		     స్థిరరాశి అక్షరం *p,
		     నిర్మాణం branch *b)
{
	స్థిర నిర్మాణం strbuf మార్గం = STRBUF_INIT;
	నిర్మాణం వృక్షం_నమోదు *మూలం = ఖాళీ;
	నిర్మాణం వృక్షం_నమోదు ఆకు = {ఖాళీ};

	/* ls SP (<tree-ish> SP)? <path> */
	ఒకవేళ (*p == '"') {
		ఒకవేళ (!b)
			die(_("not in a commit: %s"), command_buf.buf);
		మూలం = &b->branch_వృక్షం;
	} లేకపోతే {
		నిర్మాణం object_నమోదు *e = parse_treeish_dataref(&p);
		మూలం = new_వృక్షం_నమోదు();
		oidcpy(&మూలం->versions[1].oid, &e->idx.oid);
		ఒకవేళ (!is_null_oid(&మూలం->versions[1].oid))
			మూలం->versions[1].బహుళకం = S_IFDIR;
		ఎక్కించు_వృక్షం(మూలం);
	}
	strbuf_reset(&మార్గం);
	parse_మార్గం_eol(&మార్గం, p, "path");
	వృక్షం_content_get(మూలం, మార్గం.buf, &ఆకు, 1);
	/*
	 * A directory in preparation would have a sha1 of zero
	 * until it is saved.  Save, for simplicity.
	 */
	ఒకవేళ (S_ISDIR(ఆకు.versions[1].బహుళకం))
		store_వృక్షం(&ఆకు);

	print_ls(ఆకు.versions[1].బహుళకం, ఆకు.versions[1].oid.hash, మార్గం.buf);
	ఒకవేళ (ఆకు.వృక్షం)
		release_వృక్షం_content_recursive(ఆకు.వృక్షం);
	ఒకవేళ (!b || మూలం != &b->branch_వృక్షం)
		release_వృక్షం_నమోదు(మూలం);
}

స్థిర శూన్య checkpoint(శూన్య)
{
	checkpoint_requested = 0;
	ఒకవేళ (object_లెక్క) {
		చక్రం_packfile();
	}
	dump_branches();
	dump_ట్యాగులు();
	dump_marks();
}

స్థిర శూన్య parse_checkpoint(నిర్మాణం fast_import_state *state వాడని)
{
	checkpoint_requested = 1;
	skip_optional_lf();
}

స్థిర శూన్య parse_పురోగతి(నిర్మాణం fast_import_state *state వాడని)
{
	fwrite(command_buf.buf, 1, command_buf.len, stdout);
	fputc('\n', stdout);
	fflush(stdout);
	skip_optional_lf();
}

స్థిర శూన్య parse_alias(నిర్మాణం fast_import_state *state)
{
	నిర్మాణం object_నమోదు *e;
	నిర్మాణం branch b;

	skip_optional_lf();
	read_తదుపరి_command(state);

	/* mark ... */
	parse_mark(state);
	ఒకవేళ (!తదుపరి_mark)
		die(_("expected 'mark' command, got %s"), command_buf.buf);

	/* to ... */
	memset(&b, 0, పరిమాణం(b));
	ఒకవేళ (!parse_objectish_with_ఉపసర్గ(state, &b, "to "))
		die(_("expected 'to' command, got %s"), command_buf.buf);
	e = కనుగొను_object(&b.oid);
	ప్రతిజ్ఞా(e);
	చొప్పించు_mark(&marks, తదుపరి_mark, e);
}

స్థిర అక్షరం* make_fast_import_మార్గం(నిర్మాణం fast_import_state *state,
				   స్థిరరాశి అక్షరం *మార్గం)
{
	ఒకవేళ (!relative_marks_paths || is_absolute_మార్గం(మార్గం))
		తిరిగి ఉపసర్గ_filename(state->ఉపసర్గ, మార్గం);
	తిరిగి repo_git_మార్గం(the_repository, "info/fast-import/%s", మార్గం);
}

స్థిర శూన్య option_import_marks(నిర్మాణం fast_import_state *state,
				స్థిరరాశి అక్షరం *marks,
				పూర్ణాంకం from_ప్రవాహం,
				పూర్ణాంకం ignore_missing)
{
	ఒకవేళ (import_marks_file) {
		ఒకవేళ (from_ప్రవాహం)
			die(_("only one import-marks command allowed per stream"));

		/* read previous mark file */
		ఒకవేళ(!import_marks_file_from_ప్రవాహం)
			read_marks();
	}

	విడుదల(import_marks_file);
	import_marks_file = make_fast_import_మార్గం(state, marks);
	import_marks_file_from_ప్రవాహం = from_ప్రవాహం;
	import_marks_file_ignore_missing = ignore_missing;
}

స్థిర శూన్య option_date_format(స్థిరరాశి అక్షరం *fmt)
{
	ఒకవేళ (!strcmp(fmt, "raw"))
		whenspec = WHENSPEC_RAW;
	లేకపోతే ఒకవేళ (!strcmp(fmt, "raw-permissive"))
		whenspec = WHENSPEC_RAW_PERMISSIVE;
	లేకపోతే ఒకవేళ (!strcmp(fmt, "rfc2822"))
		whenspec = WHENSPEC_RFC2822;
	లేకపోతే ఒకవేళ (!strcmp(fmt, "now"))
		whenspec = WHENSPEC_NOW;
	లేకపోతే
		die(_("unknown --date-format argument %s"), fmt);
}

స్థిర గుర్తులేని దీర్ఘ ulong_arg(స్థిరరాశి అక్షరం *option, స్థిరరాశి అక్షరం *arg)
{
	అక్షరం *endptr;
	గుర్తులేని దీర్ఘ rv = strtoul(arg, &endptr, 0);
	ఒకవేళ (strchr(arg, '-') || endptr == arg || *endptr)
		die(_("%s: argument must be a non-negative integer"), option);
	తిరిగి rv;
}

స్థిర శూన్య option_లోతు(స్థిరరాశి అక్షరం *లోతు)
{
	max_లోతు = ulong_arg("--depth", లోతు);
	ఒకవేళ (max_లోతు > MAX_లోతు)
		die(_("--depth cannot exceed %u"), MAX_లోతు);
}

స్థిర శూన్య option_చురుకైన_branches(స్థిరరాశి అక్షరం *branches)
{
	max_చురుకైన_branches = ulong_arg("--active-branches", branches);
}

స్థిర శూన్య option_export_marks(నిర్మాణం fast_import_state *state,
				స్థిరరాశి అక్షరం *marks)
{
	విడుదల(export_marks_file);
	export_marks_file = make_fast_import_మార్గం(state, marks);
}

స్థిర శూన్య option_cat_blob_fd(నిర్మాణం fast_import_state *state వాడని,
			       స్థిరరాశి అక్షరం *fd)
{
	గుర్తులేని దీర్ఘ n = ulong_arg("--cat-blob-fd", fd);
	ఒకవేళ (n > (గుర్తులేని దీర్ఘ) గరిష్ఠ_పూర్ణాంకం)
		die(_("--cat-blob-fd cannot exceed %d"), గరిష్ఠ_పూర్ణాంకం);
	cat_blob_fd = (పూర్ణాంకం) n;
}

స్థిర శూన్య option_export_pack_అంచులు(నిర్మాణం fast_import_state *state,
				     స్థిరరాశి అక్షరం *అంచులు)
{
	అక్షరం *fn = ఉపసర్గ_filename(state->ఉపసర్గ, అంచులు);
	ఒకవేళ (pack_అంచులు)
		fclose(pack_అంచులు);
	pack_అంచులు = xfopen(fn, "a");
	విడుదల(fn);
}

స్థిర శూన్య option_rewrite_submodules(నిర్మాణం fast_import_state *state,
				      స్థిరరాశి అక్షరం *arg,
				      నిర్మాణం string_list *list)
{
	నిర్మాణం mark_సమితి *ms;
	FILE *fp;
	అక్షరం *s = xstrdup(arg);
	అక్షరం *f = strchr(s, ':');
	ఒకవేళ (!f)
		die(_("expected format name:filename for submodule rewrite option"));
	*f = '\0';
	f++;
	CALLOC_ARRAY(ms, 1);

	f = ఉపసర్గ_filename(state->ఉపసర్గ, f);
	fp = fopen(f, "r");
	ఒకవేళ (!fp)
		die_errno(_("cannot read '%s'"), f);
	read_mark_file(&ms, fp, చొప్పించు_oid_నమోదు);
	fclose(fp);
	విడుదల(f);

	string_list_చొప్పించు(list, s)->util = ms;

	విడుదల(s);
}

స్థిర శూన్య option_max_pack_కొలత(స్థిరరాశి అక్షరం *arg)
{
	గుర్తులేని దీర్ఘ v;

	ఒకవేళ (!git_parse_ulong(arg, &v))
		die(_("--max-pack-size: argument must be a non-negative integer"));
	ఒకవేళ (v < 8192) {
		warning(_("max-pack-size is now in bytes, assuming --max-pack-size=%lum"), v);
		v *= 1024 * 1024;
	} లేకపోతే ఒకవేళ (v < 1024 * 1024) {
		warning(_("minimum max-pack-size is 1 MiB"));
		v = 1024 * 1024;
	}
	max_packsize = v;
}

స్థిర శూన్య option_big_file_పరిమితి(స్థిరరాశి అక్షరం *arg)
{
	గుర్తులేని దీర్ఘ v;

	ఒకవేళ (!git_parse_ulong(arg, &v))
		die(_("--big-file-threshold: argument must be a non-negative integer"));
	repo_settings_సమితి_big_file_పరిమితి(the_repository, v);
}

స్థిర శూన్య option_signed_commits(స్థిరరాశి అక్షరం *arg)
{
	ఒకవేళ (parse_sign_బహుళకం(arg, &signed_commit_బహుళకం, &signed_commit_keyid))
		usagef(_("unknown --signed-commits mode '%s'"), arg);
}

స్థిర శూన్య option_signed_ట్యాగులు(స్థిరరాశి అక్షరం *arg)
{
	ఒకవేళ (parse_sign_బహుళకం(arg, &signed_ట్యాగ్_బహుళకం, &signed_ట్యాగ్_keyid))
		usagef(_("unknown --signed-tags mode '%s'"), arg);
}

స్థిర శూన్య option_quiet(శూన్య)
{
	show_stats = 0;
	quiet = 1;
}

స్థిర పూర్ణాంకం parse_one_option(నిర్మాణం fast_import_state *state, స్థిరరాశి అక్షరం *option)
{
	ఒకవేళ (skip_ఉపసర్గ(option, "max-pack-size=", &option)) {
		option_max_pack_కొలత(option);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(option, "big-file-threshold=", &option)) {
		option_big_file_పరిమితి(option);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(option, "depth=", &option)) {
		option_లోతు(option);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(option, "active-branches=", &option)) {
		option_చురుకైన_branches(option);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(option, "export-pack-edges=", &option)) {
		option_export_pack_అంచులు(state, option);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(option, "signed-commits=", &option)) {
		option_signed_commits(option);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(option, "signed-tags=", &option)) {
		option_signed_ట్యాగులు(option);
	} లేకపోతే ఒకవేళ (!strcmp(option, "quiet")) {
		option_quiet();
	} లేకపోతే ఒకవేళ (!strcmp(option, "stats")) {
		show_stats = 1;
	} లేకపోతే ఒకవేళ (!strcmp(option, "allow-unsafe-features")) {
		; /* already handled during early option parsing */
	} లేకపోతే {
		తిరిగి 0;
	}

	తిరిగి 1;
}

స్థిర శూన్య check_unsafe_feature(నిర్మాణం fast_import_state *state,
				 స్థిరరాశి అక్షరం *feature)
{
	ఒకవేళ (!state->allow_unsafe_features)
		die(_("feature '%s' forbidden in input without --allow-unsafe-features"),
		    feature);
}

స్థిర పూర్ణాంకం parse_one_feature(నిర్మాణం fast_import_state *state,
			     స్థిరరాశి అక్షరం *feature)
{
	స్థిరరాశి అక్షరం *arg;

	ఒకవేళ (skip_ఉపసర్గ(feature, "date-format=", &arg)) {
		option_date_format(arg);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(feature, "import-marks=", &arg)) {
		check_unsafe_feature(state, "import-marks");
		option_import_marks(state, arg, 1, 0);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(feature, "import-marks-if-exists=", &arg)) {
		check_unsafe_feature(state, "import-marks-if-exists");
		option_import_marks(state, arg, 1, 1);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(feature, "export-marks=", &arg)) {
		check_unsafe_feature(state, feature);
		option_export_marks(state, arg);
	} లేకపోతే ఒకవేళ (!strcmp(feature, "alias")) {
		; /* Don't die - this feature is supported */
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(feature, "rewrite-submodules-to=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_గమ్యం);
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(feature, "rewrite-submodules-from=", &arg)) {
		option_rewrite_submodules(state, arg, &sub_marks_from);
	} లేకపోతే ఒకవేళ (!strcmp(feature, "get-mark")) {
		; /* Don't die - this feature is supported */
	} లేకపోతే ఒకవేళ (!strcmp(feature, "cat-blob")) {
		; /* Don't die - this feature is supported */
	} లేకపోతే ఒకవేళ (!strcmp(feature, "relative-marks")) {
		relative_marks_paths = 1;
	} లేకపోతే ఒకవేళ (!strcmp(feature, "no-relative-marks")) {
		relative_marks_paths = 0;
	} లేకపోతే ఒకవేళ (!strcmp(feature, "done")) {
		require_explicit_termination = 1;
	} లేకపోతే ఒకవేళ (!strcmp(feature, "force")) {
		force_update = 1;
	} లేకపోతే ఒకవేళ (!strcmp(feature, "notes") || !strcmp(feature, "ls")) {
		; /* do nothing; we have the feature */
	} లేకపోతే {
		తిరిగి 0;
	}

	తిరిగి 1;
}

స్థిర శూన్య parse_feature(నిర్మాణం fast_import_state *state, స్థిరరాశి అక్షరం *feature)
{
	ఒకవేళ (state->చూసిన_దత్తాంశం_command)
		die(_("got feature command '%s' after data command"), feature);

	ఒకవేళ (parse_one_feature(state, feature))
		తిరిగి;

	die(_("this version of fast-import does not support feature %s."), feature);
}

స్థిర శూన్య parse_option(నిర్మాణం fast_import_state *state, స్థిరరాశి అక్షరం *option)
{
	ఒకవేళ (state->చూసిన_దత్తాంశం_command)
		die(_("got option command '%s' after data command"), option);

	ఒకవేళ (parse_one_option(state, option))
		తిరిగి;

	die(_("this version of fast-import does not support option: %s"), option);
}

స్థిర శూన్య git_pack_config(శూన్య)
{
	పూర్ణాంకం indexversion_విలువ;
	పూర్ణాంకం హద్దు;
	గుర్తులేని దీర్ఘ packsizelimit_విలువ;

	ఒకవేళ (!repo_config_get_ulong(the_repository, "pack.depth", &max_లోతు)) {
		ఒకవేళ (max_లోతు > MAX_లోతు)
			max_లోతు = MAX_లోతు;
	}
	ఒకవేళ (!repo_config_get_int(the_repository, "pack.indexversion", &indexversion_విలువ)) {
		pack_idx_opts.సంస్కరణ = indexversion_విలువ;
		ఒకవేళ (pack_idx_opts.సంస్కరణ > 2)
			git_die_config(the_repository, "pack.indexversion",
				       "bad pack.indexVersion=%"PRIu32, pack_idx_opts.సంస్కరణ);
	}
	ఒకవేళ (!repo_config_get_ulong(the_repository, "pack.packsizelimit", &packsizelimit_విలువ))
		max_packsize = packsizelimit_విలువ;

	ఒకవేళ (!repo_config_get_int(the_repository, "fastimport.unpacklimit", &హద్దు))
		unpack_హద్దు = హద్దు;
	లేకపోతే ఒకవేళ (!repo_config_get_int(the_repository, "transfer.unpacklimit", &హద్దు))
		unpack_హద్దు = హద్దు;

	repo_config(the_repository, git_default_config, ఖాళీ);
}

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి fast_import_usage[] = {
	N_("git fast-import [<options>]"),
	ఖాళీ
};

స్థిర శూన్య parse_argv(నిర్మాణం fast_import_state *state)
{
	పూర్ణాంకం argc = parse_options(state->argc, state->argv, state->ఉపసర్గ,
				 state->option, fast_import_usage,
				 PARSE_OPT_KEEP_ARGV0);

	ఒకవేళ (argc > 1)
		usage_with_options(fast_import_usage, state->option);

	state->చూసిన_దత్తాంశం_command = 1;
	ఒకవేళ (import_marks_file)
		read_marks();
	build_mark_map(&sub_marks_from, &sub_marks_గమ్యం);
}

స్థిర పూర్ణాంకం option_parse_date_format(స్థిరరాశి నిర్మాణం option *opt వాడని,
				    స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_date_format(arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_export_pack_అంచులు(స్థిరరాశి నిర్మాణం option *opt,
					  స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_pack_అంచులు(opt->విలువ, arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_max_pack_కొలత(స్థిరరాశి నిర్మాణం option *opt వాడని,
				      స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_max_pack_కొలత(arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_big_file_పరిమితి(స్థిరరాశి నిర్మాణం option *opt వాడని,
					   స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_big_file_పరిమితి(arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_signed_commits(స్థిరరాశి నిర్మాణం option *opt వాడని,
				       స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_commits(arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_signed_ట్యాగులు(స్థిరరాశి నిర్మాణం option *opt వాడని,
				    స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_signed_ట్యాగులు(arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_rewrite_submodules_from(స్థిరరాశి నిర్మాణం option *opt,
						స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->విలువ, arg, &sub_marks_from);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_rewrite_submodules_గమ్యం(స్థిరరాశి నిర్మాణం option *opt,
					      స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_rewrite_submodules(opt->విలువ, arg, &sub_marks_గమ్యం);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_cat_blob_fd(స్థిరరాశి నిర్మాణం option *opt,
				    స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_cat_blob_fd(opt->విలువ, arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_import_marks(స్థిరరాశి నిర్మాణం option *opt,
				     స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->విలువ, arg, 0, 0);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_import_marks_if_exists(స్థిరరాశి నిర్మాణం option *opt,
					       స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_import_marks(opt->విలువ, arg, 0, 1);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_export_marks(స్థిరరాశి నిర్మాణం option *opt,
				     స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_export_marks(opt->విలువ, arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_లోతు(స్థిరరాశి నిర్మాణం option *opt వాడని,
			      స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_లోతు(arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_చురుకైన_branches(స్థిరరాశి నిర్మాణం option *opt వాడని,
					స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_చురుకైన_branches(arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం option_parse_quiet(స్థిరరాశి నిర్మాణం option *opt వాడని,
			      స్థిరరాశి అక్షరం *arg వాడని, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);
	option_quiet();
	తిరిగి 0;
}

పూర్ణాంకం cmd_fast_import(పూర్ణాంకం argc,
		    స్థిరరాశి అక్షరం **argv,
		    స్థిరరాశి అక్షరం *ఉపసర్గ,
		    నిర్మాణం repository *repo)
{
	నిర్మాణం fast_import_state state;

	నిర్మాణం option fast_import_options[] = {
		OPT_సమూహం(N_("Common")),
		OPT_CALLBACK_F(0, "date-format", ఖాళీ, N_("fmt"),
			       N_("format of the commit/tag dates"),
			       PARSE_OPT_NONEG, option_parse_date_format),
		OPT_BOOL_F(0, "stats", &show_stats,
			   N_("display some basic statistics (objects, packfiles and memory)"),
			   PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "quiet", ఖాళీ, ఖాళీ,
			       N_("disable the output shown by --stats"),
			       PARSE_OPT_NOARG | PARSE_OPT_NONEG,
			       option_parse_quiet),
		OPT_BOOL_F(0, "force", &force_update,
			   N_("force updating modified existing branches"), PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "done", &require_explicit_termination,
			   N_("require a terminating 'done' command"), PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "max-pack-size", ఖాళీ, N_("n"),
			       N_("maximum size of each output pack file"),
			       PARSE_OPT_NONEG, option_parse_max_pack_కొలత),
		OPT_CALLBACK_F(0, "big-file-threshold", ఖాళీ, N_("n"),
			       N_("maximum size of a blob that will be deltified"),
			       PARSE_OPT_NONEG, option_parse_big_file_పరిమితి),
		OPT_CALLBACK_F(0, "depth", ఖాళీ, N_("n"),
			       N_("maximum delta depth"),
			       PARSE_OPT_NONEG, option_parse_లోతు),
		OPT_CALLBACK_F(0, "active-branches", ఖాళీ, N_("n"),
			       N_("maximum number of branches to maintain active"),
			       PARSE_OPT_NONEG, option_parse_చురుకైన_branches),
		OPT_సమూహం(N_("Marks")),
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
		OPT_సమూహం(N_("Submodule rewrite")),
		OPT_CALLBACK_F(0, "rewrite-submodules-from", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> from <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_from),
		OPT_CALLBACK_F(0, "rewrite-submodules-to", &state, N_("name:filename"),
			       N_("rewrite object IDs for submodule <name> to <filename>"),
			       PARSE_OPT_NONEG, option_parse_rewrite_submodules_గమ్యం),
		OPT_సమూహం(N_("Signing")),
		OPT_CALLBACK_F(0, "signed-commits", ఖాళీ, N_("mode"),
			       N_("how to handle signed commits"),
			       PARSE_OPT_NONEG, option_parse_signed_commits),
		OPT_CALLBACK_F(0, "signed-tags", ఖాళీ, N_("mode"),
			       N_("how to handle signed tags"),
			       PARSE_OPT_NONEG, option_parse_signed_ట్యాగులు),
		OPT_HIDDEN_సమూహం(N_("Advanced")),
		OPT_BOOL_F(0, "allow-unsafe-features", &state.allow_unsafe_features,
			   N_("allow unsafe mark commands from the stream"),
			   PARSE_OPT_HIDDEN | PARSE_OPT_NONEG),
		OPT_CALLBACK_F(0, "export-pack-edges", &state, N_("file"),
			       N_("dump edge commits to <file>"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_export_pack_అంచులు),
		OPT_CALLBACK_F(0, "cat-blob-fd", &state, N_("fd"),
			       N_("write some responses to <fd> instead of stdout"),
			       PARSE_OPT_HIDDEN | PARSE_OPT_NONEG,
			       option_parse_cat_blob_fd),
		OPT_చివర()
	};

	show_usage_with_options_if_asked(argc, argv, fast_import_usage, fast_import_options);

	fast_import_state_init(&state, argc, argv, ఉపసర్గ, fast_import_options);

	reset_pack_idx_option(&pack_idx_opts);
	git_pack_config();

	alloc_objects(object_నమోదు_alloc);
	strbuf_init(&command_buf, 0);
	CALLOC_ARRAY(atom_పట్టిక, atom_పట్టిక_sz);
	CALLOC_ARRAY(branch_పట్టిక, branch_పట్టిక_sz);
	CALLOC_ARRAY(avail_వృక్షం_పట్టిక, avail_వృక్షం_పట్టిక_sz);
	marks = mem_pool_calloc(&fi_mem_pool, 1, పరిమాణం(నిర్మాణం mark_సమితి));

	hashmap_init(&object_పట్టిక, object_నమోదు_hashcmp, ఖాళీ, 0);

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
	కొరకు (పూర్ణాంకం i = 1; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];
		ఒకవేళ (*arg != '-' || !strcmp(arg, "--"))
			ఆపు;
		ఒకవేళ (!strcmp(arg, "--allow-unsafe-features"))
			state.allow_unsafe_features = 1;
	}

	rc_free = mem_pool_alloc(&fi_mem_pool, cmd_save * పరిమాణం(*rc_free));
	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < (cmd_save - 1); i++)
		rc_free[i].తదుపరి = &rc_free[i + 1];
	rc_free[cmd_save - 1].తదుపరి = ఖాళీ;

	ప్రారంభం_packfile();
	సమితి_die_routine(die_nicely);
	సమితి_checkpoint_signal();
	అయితే (read_తదుపరి_command(&state) != EOF) {
		స్థిరరాశి అక్షరం *v;
		ఒకవేళ (!strcmp("blob", command_buf.buf))
			parse_new_blob(&state);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "commit ", &v))
			parse_new_commit(&state, v);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "tag ", &v))
			parse_new_ట్యాగ్(&state, v);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "reset ", &v))
			parse_reset_branch(&state, v);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "ls ", &v))
			parse_ls(&state, v, ఖాళీ);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "cat-blob ", &v))
			parse_cat_blob(&state, v);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "get-mark ", &v))
			parse_get_mark(&state, v);
		లేకపోతే ఒకవేళ (!strcmp("checkpoint", command_buf.buf))
			parse_checkpoint(&state);
		లేకపోతే ఒకవేళ (!strcmp("done", command_buf.buf))
			ఆపు;
		లేకపోతే ఒకవేళ (!strcmp("alias", command_buf.buf))
			parse_alias(&state);
		లేకపోతే ఒకవేళ (starts_with(command_buf.buf, "progress "))
			parse_పురోగతి(&state);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "feature ", &v))
			parse_feature(&state, v);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(command_buf.buf, "option git ", &v))
			parse_option(&state, v);
		లేకపోతే ఒకవేళ (starts_with(command_buf.buf, "option "))
			/* ignore non-git options*/;
		లేకపోతే
			die(_("unsupported command: %s"), command_buf.buf);

		ఒకవేళ (checkpoint_requested)
			checkpoint();
	}

	/* argv hasn't been parsed yet, do so */
	ఒకవేళ (!state.చూసిన_దత్తాంశం_command)
		parse_argv(&state);

	ఒకవేళ (require_explicit_termination && feof(stdin))
		die(_("stream ends early"));

	చివర_packfile();

	dump_branches();
	dump_ట్యాగులు();
	unkeep_all_packs();
	dump_marks();

	ఒకవేళ (pack_అంచులు)
		fclose(pack_అంచులు);

	ఒకవేళ (show_stats) {
		uintmax_t మొత్తం_లెక్క = 0, duplicate_లెక్క = 0;
		కొరకు (కొలత_t i = 0; i < ARRAY_కొలత(object_లెక్క_by_type); i++)
			మొత్తం_లెక్క += object_లెక్క_by_type[i];
		కొరకు (కొలత_t i = 0; i < ARRAY_కొలత(duplicate_లెక్క_by_type); i++)
			duplicate_లెక్క += duplicate_లెక్క_by_type[i];

		fprintf(stderr, "%s statistics:\n", argv[0]);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "Alloc'd objects: %10" PRIuMAX "\n", alloc_లెక్క);
		fprintf(stderr, "Total objects:   %10" PRIuMAX " (%10" PRIuMAX " duplicates                  )\n", మొత్తం_లెక్క, duplicate_లెక్క);
		fprintf(stderr, "      blobs  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_లెక్క_by_type[OBJ_BLOB], duplicate_లెక్క_by_type[OBJ_BLOB], delta_లెక్క_by_type[OBJ_BLOB], delta_లెక్క_attempts_by_type[OBJ_BLOB]);
		fprintf(stderr, "      trees  :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_లెక్క_by_type[OBJ_వృక్షం], duplicate_లెక్క_by_type[OBJ_వృక్షం], delta_లెక్క_by_type[OBJ_వృక్షం], delta_లెక్క_attempts_by_type[OBJ_వృక్షం]);
		fprintf(stderr, "      commits:   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_లెక్క_by_type[OBJ_COMMIT], duplicate_లెక్క_by_type[OBJ_COMMIT], delta_లెక్క_by_type[OBJ_COMMIT], delta_లెక్క_attempts_by_type[OBJ_COMMIT]);
		fprintf(stderr, "      tags   :   %10" PRIuMAX " (%10" PRIuMAX " duplicates %10" PRIuMAX " deltas of %10" PRIuMAX" attempts)\n", object_లెక్క_by_type[OBJ_ట్యాగ్], duplicate_లెక్క_by_type[OBJ_ట్యాగ్], delta_లెక్క_by_type[OBJ_ట్యాగ్], delta_లెక్క_attempts_by_type[OBJ_ట్యాగ్]);
		fprintf(stderr, "Total branches:  %10lu (%10lu loads     )\n", branch_లెక్క, branch_ఎక్కించు_లెక్క);
		fprintf(stderr, "      marks:     %10" PRIuMAX " (%10" PRIuMAX " unique    )\n", (((uintmax_t)1) << marks->జరుపు) * 1024, marks_సమితి_లెక్క);
		fprintf(stderr, "      atoms:     %10u\n", atom_cnt);
		fprintf(stderr, "Memory total:    %10" PRIuMAX " KiB\n", (వృక్షం_నమోదు_allocd + fi_mem_pool.pool_alloc + alloc_లెక్క*పరిమాణం(నిర్మాణం object_నమోదు))/1024);
		fprintf(stderr, "       pools:    %10lu KiB\n", (గుర్తులేని దీర్ఘ)((వృక్షం_నమోదు_allocd + fi_mem_pool.pool_alloc) /1024));
		fprintf(stderr, "     objects:    %10" PRIuMAX " KiB\n", (alloc_లెక్క*పరిమాణం(నిర్మాణం object_నమోదు))/1024);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		pack_report(repo);
		fprintf(stderr, "---------------------------------------------------------------------\n");
		fprintf(stderr, "\n");
	}

	తిరిగి failure ? 1 : 0;
}
