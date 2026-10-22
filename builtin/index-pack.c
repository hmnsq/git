#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "delta.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "pack.h"
#అన్తర్భావ "csum-file.h"
#అన్తర్భావ "blob.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "progress.h"
#అన్తర్భావ "fsck.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "thread-utils.h"
#అన్తర్భావ "packfile.h"
#అన్తర్భావ "pack-revindex.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/streaming.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "oidset.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "replace-object.h"
#అన్తర్భావ "tree-walk.h"
#అన్తర్భావ "promisor-remote.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "strvec.h"

స్థిర స్థిరరాశి అక్షరం సూచీ_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

నిర్మాణం object_నమోదు {
	నిర్మాణం pack_idx_నమోదు idx;
	కొలత_t కొలత;
	గుర్తులేని అక్షరం hdr_కొలత;
	గుర్తుగల అక్షరం type;
	గుర్తుగల అక్షరం real_type;
};

నిర్మాణం object_stat {
	గుర్తులేని delta_లోతు;
	పూర్ణాంకం పునాది_object_no;
};

నిర్మాణం పునాది_దత్తాంశం {
	/* Initialized by make_base(). */
	నిర్మాణం పునాది_దత్తాంశం *పునాది;
	నిర్మాణం object_నమోదు *obj;
	పూర్ణాంకం ref_మొదటి, ref_చివరి;
	పూర్ణాంకం ofs_మొదటి, ofs_చివరి;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	పూర్ణాంకం retain_దత్తాంశం;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	పూర్ణాంకం children_మిగిలిన;

	/* Not initialized by make_base(). */
	నిర్మాణం list_తల list;
	శూన్య *దత్తాంశం;
	కొలత_t కొలత;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
స్థిర LIST_తల(work_తల);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
స్థిర LIST_తల(పూర్తి_తల);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
స్థిర కొలత_t పునాది_cache_వాడిన;
స్థిర కొలత_t పునాది_cache_హద్దు;

నిర్మాణం thread_local_దత్తాంశం {
	pthread_t thread;
	పూర్ణాంకం pack_fd;
};

/* Remember to update object flag allocation in object.h */
#నిర్వచించు FLAG_LINK (1u<<20)
#నిర్వచించు FLAG_CHECKED (1u<<21)

నిర్మాణం ofs_delta_నమోదు {
	off_t స్థానభ్రంశం;
	పూర్ణాంకం obj_no;
};

నిర్మాణం ref_delta_నమోదు {
	నిర్మాణం object_id oid;
	పూర్ణాంకం obj_no;
};

స్థిర నిర్మాణం object_నమోదు *objects;
స్థిర నిర్మాణం object_stat *obj_stat;
స్థిర నిర్మాణం ofs_delta_నమోదు *ofs_deltas;
స్థిర నిర్మాణం ref_delta_నమోదు *ref_deltas;
స్థిర నిర్మాణం thread_local_దత్తాంశం nothread_దత్తాంశం;
స్థిర పూర్ణాంకం nr_objects;
స్థిర పూర్ణాంకం nr_ofs_deltas;
స్థిర పూర్ణాంకం nr_ref_deltas;
స్థిర పూర్ణాంకం ref_deltas_alloc;
స్థిర పూర్ణాంకం nr_resolved_deltas;
స్థిర పూర్ణాంకం nr_దారాలు;

స్థిర పూర్ణాంకం from_stdin;
స్థిర పూర్ణాంకం strict;
స్థిర పూర్ణాంకం do_fsck_object;
స్థిర నిర్మాణం fsck_options fsck_options;
స్థిర పూర్ణాంకం verbose;
స్థిర స్థిరరాశి అక్షరం *పురోగతి_title;
స్థిర పూర్ణాంకం show_resolving_పురోగతి;
స్థిర పూర్ణాంకం show_stat;
స్థిర పూర్ణాంకం check_self_contained_and_connected;

స్థిర నిర్మాణం పురోగతి *పురోగతి;

స్థిర గుర్తులేని అక్షరం ఆదానం_బఫరు[DEFAULT_IO_బఫరు_కొలత];
స్థిర గుర్తులేని పూర్ణాంకం ఆదానం_స్థానభ్రంశం, ఆదానం_len;
స్థిర off_t consumed_బైట్లు;
స్థిర off_t max_ఆదానం_కొలత;
స్థిర గుర్తులేని అత్యంతలోతు_delta;
స్థిర నిర్మాణం git_hash_ctx ఆదానం_ctx;
స్థిర uint32_t ఆదానం_చక్రీయ_తనిఖీ_32;
స్థిర పూర్ణాంకం ఆదానం_fd, output_fd;
స్థిర స్థిరరాశి అక్షరం *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
స్థిర నిర్మాణం oidset outgoing_links = OIDSET_INIT;
స్థిర పూర్ణాంకం రికార్డు_outgoing_links;

స్థిర నిర్మాణం thread_local_దత్తాంశం *thread_దత్తాంశం;
స్థిర పూర్ణాంకం nr_dispatched;
స్థిర పూర్ణాంకం దారాలు_చురుకైన;

స్థిర pthread_mutex_t read_mutex;
#నిర్వచించు read_తాళం()		తాళం_mutex(&read_mutex)
#నిర్వచించు read_unlock()		unlock_mutex(&read_mutex)

స్థిర pthread_mutex_t counter_mutex;
#నిర్వచించు counter_తాళం()		తాళం_mutex(&counter_mutex)
#నిర్వచించు counter_unlock()	unlock_mutex(&counter_mutex)

స్థిర pthread_mutex_t work_mutex;
#నిర్వచించు work_తాళం()		తాళం_mutex(&work_mutex)
#నిర్వచించు work_unlock()		unlock_mutex(&work_mutex)

స్థిర pthread_mutex_t అత్యంతలోతు_delta_mutex;
#నిర్వచించు అత్యంతలోతు_delta_తాళం()	తాళం_mutex(&అత్యంతలోతు_delta_mutex)
#నిర్వచించు అత్యంతలోతు_delta_unlock()	unlock_mutex(&అత్యంతలోతు_delta_mutex)

స్థిర pthread_కీలకం_t కీలకం;

స్థిర అంతఃవరుస శూన్య తాళం_mutex(pthread_mutex_t *mutex)
{
	ఒకవేళ (దారాలు_చురుకైన)
		pthread_mutex_తాళం(mutex);
}

స్థిర అంతఃవరుస శూన్య unlock_mutex(pthread_mutex_t *mutex)
{
	ఒకవేళ (దారాలు_చురుకైన)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
స్థిర శూన్య init_thread(శూన్య)
{
	పూర్ణాంకం i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, ఖాళీ);
	pthread_mutex_init(&work_mutex, ఖాళీ);
	ఒకవేళ (show_stat)
		pthread_mutex_init(&అత్యంతలోతు_delta_mutex, ఖాళీ);
	pthread_కీలకం_create(&కీలకం, ఖాళీ);
	CALLOC_ARRAY(thread_దత్తాంశం, nr_దారాలు);
	కొరకు (i = 0; i < nr_దారాలు; i++) {
		thread_దత్తాంశం[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	దారాలు_చురుకైన = 1;
}

స్థిర శూన్య cleanup_thread(శూన్య)
{
	పూర్ణాంకం i;
	ఒకవేళ (!దారాలు_చురుకైన)
		తిరిగి;
	దారాలు_చురుకైన = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	ఒకవేళ (show_stat)
		pthread_mutex_destroy(&అత్యంతలోతు_delta_mutex);
	కొరకు (i = 0; i < nr_దారాలు; i++)
		close(thread_దత్తాంశం[i].pack_fd);
	pthread_కీలకం_delete(కీలకం);
	విడుదల(thread_దత్తాంశం);
}

స్థిర పూర్ణాంకం mark_link(నిర్మాణం object *obj, గణన object_type type,
		     శూన్య *దత్తాంశం వాడని,
		     నిర్మాణం fsck_options *options వాడని)
{
	ఒకవేళ (!obj)
		తిరిగి -1;

	ఒకవేళ (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_గమ్యం_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	తిరిగి 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
స్థిర గుర్తులేని check_object(నిర్మాణం object *obj)
{
	ఒకవేళ (!obj)
		తిరిగి 0;

	ఒకవేళ (!(obj->flags & FLAG_LINK))
		తిరిగి 0;

	ఒకవేళ (!(obj->flags & FLAG_CHECKED)) {
		కొలత_t కొలత;
		పూర్ణాంకం type = odb_read_object_info(the_repository->objects,
						&obj->oid, &కొలత);
		ఒకవేళ (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_గమ్యం_hex(&obj->oid));
		ఒకవేళ (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_గమ్యం_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		తిరిగి 1;
	}

	తిరిగి 0;
}

స్థిర గుర్తులేని check_objects(శూన్య)
{
	గుర్తులేని i, max, foreign_nr = 0;

	max = get_max_object_సూచీ(the_repository);

	ఒకవేళ (verbose)
		పురోగతి = ప్రారంభం_delayed_పురోగతి(the_repository,
						  _("Checking objects"), max);

	కొరకు (i = 0; i < max; i++) {
		foreign_nr += check_object(get_సూచీకృత_object(the_repository, i));
		display_పురోగతి(పురోగతి, i + 1);
	}

	stop_పురోగతి(&పురోగతి);
	తిరిగి foreign_nr;
}


/* Discard current buffer used content. */
స్థిర శూన్య flush(శూన్య)
{
	ఒకవేళ (ఆదానం_స్థానభ్రంశం) {
		ఒకవేళ (output_fd >= 0)
			write_or_die(output_fd, ఆదానం_బఫరు, ఆదానం_స్థానభ్రంశం);
		git_hash_update(&ఆదానం_ctx, ఆదానం_బఫరు, ఆదానం_స్థానభ్రంశం);
		memmove(ఆదానం_బఫరు, ఆదానం_బఫరు + ఆదానం_స్థానభ్రంశం, ఆదానం_len);
		ఆదానం_స్థానభ్రంశం = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
స్థిర శూన్య *fill(పూర్ణాంకం min)
{
	ఒకవేళ (min <= ఆదానం_len)
		తిరిగి ఆదానం_బఫరు + ఆదానం_స్థానభ్రంశం;
	ఒకవేళ (min > పరిమాణం(ఆదానం_బఫరు))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	చేయి {
		ssize_t ret = xread(ఆదానం_fd, ఆదానం_బఫరు + ఆదానం_len,
				పరిమాణం(ఆదానం_బఫరు) - ఆదానం_len);
		ఒకవేళ (ret <= 0) {
			ఒకవేళ (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		ఆదానం_len += ret;
		ఒకవేళ (from_stdin)
			display_throughput(పురోగతి, consumed_బైట్లు + ఆదానం_len);
	} అయితే (ఆదానం_len < min);
	తిరిగి ఆదానం_బఫరు;
}

స్థిర శూన్య use(పూర్ణాంకం బైట్లు)
{
	ఒకవేళ (బైట్లు > ఆదానం_len)
		die(_("used more bytes than were available"));
	ఆదానం_చక్రీయ_తనిఖీ_32 = చక్రీయ_తనిఖీ_32(ఆదానం_చక్రీయ_తనిఖీ_32, ఆదానం_బఫరు + ఆదానం_స్థానభ్రంశం, బైట్లు);
	ఆదానం_len -= బైట్లు;
	ఆదానం_స్థానభ్రంశం += బైట్లు;

	/* make sure off_t is sufficiently large not to wrap */
	ఒకవేళ (signed_కూడు_overflows(consumed_బైట్లు, బైట్లు))
		die(_("pack too large for current definition of off_t"));
	consumed_బైట్లు += బైట్లు;
	ఒకవేళ (max_ఆదానం_కొలత && consumed_బైట్లు > max_ఆదానం_కొలత) {
		నిర్మాణం strbuf కొలత_హద్దు = STRBUF_INIT;
		strbuf_humanise_బైట్లు(&కొలత_హద్దు, max_ఆదానం_కొలత);
		die(_("pack exceeds maximum allowed size (%s)"),
		    కొలత_హద్దు.buf);
	}
}

స్థిర స్థిరరాశి అక్షరం *open_pack_file(స్థిరరాశి అక్షరం *pack_name)
{
	ఒకవేళ (from_stdin) {
		ఆదానం_fd = 0;
		ఒకవేళ (!pack_name) {
			నిర్మాణం strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, ఖాళీ);
		} లేకపోతే {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_దత్తాంశం.pack_fd = output_fd;
	} లేకపోతే {
		ఆదానం_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_దత్తాంశం.pack_fd = ఆదానం_fd;
	}
	git_hash_init(&ఆదానం_ctx, the_hash_algo);
	తిరిగి pack_name;
}

స్థిర శూన్య parse_pack_శీర్షిక(శూన్య)
{
	గుర్తులేని అక్షరం *hdr = fill(పరిమాణం(నిర్మాణం pack_శీర్షిక));

	/* Header consistency check */
	ఒకవేళ (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	ఒకవేళ (!pack_సంస్కరణ_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(పరిమాణం(నిర్మాణం pack_శీర్షిక));
}

__attribute__((format (రాయి, 2, 3)))
స్థిర NORETURN శూన్య bad_object(off_t స్థానభ్రంశం, స్థిరరాశి అక్షరం *format, ...)
{
	va_list params;
	అక్షరం buf[1024];

	va_ప్రారంభం(params, format);
	vsnprintf(buf, పరిమాణం(buf), format, params);
	va_చివర(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)స్థానభ్రంశం, buf);
}

స్థిర అంతఃవరుస నిర్మాణం thread_local_దత్తాంశం *get_thread_దత్తాంశం(శూన్య)
{
	ఒకవేళ (HAVE_దారాలు) {
		ఒకవేళ (దారాలు_చురుకైన)
			తిరిగి pthread_getspecific(కీలకం);
		ప్రతిజ్ఞా(!దారాలు_చురుకైన &&
		       "This should only be reached when all threads are gone");
	}
	తిరిగి &nothread_దత్తాంశం;
}

స్థిర శూన్య సమితి_thread_దత్తాంశం(నిర్మాణం thread_local_దత్తాంశం *దత్తాంశం)
{
	ఒకవేళ (దారాలు_చురుకైన)
		pthread_setspecific(కీలకం, దత్తాంశం);
}

స్థిర శూన్య free_పునాది_దత్తాంశం(నిర్మాణం పునాది_దత్తాంశం *c)
{
	ఒకవేళ (c->దత్తాంశం) {
		FREE_AND_NULL(c->దత్తాంశం);
		పునాది_cache_వాడిన -= c->కొలత;
	}
}

స్థిర శూన్య prune_పునాది_దత్తాంశం(నిర్మాణం పునాది_దత్తాంశం *retain)
{
	నిర్మాణం list_తల *pos;

	ఒకవేళ (పునాది_cache_వాడిన <= పునాది_cache_హద్దు)
		తిరిగి;

	list_for_each_prev(pos, &పూర్తి_తల) {
		నిర్మాణం పునాది_దత్తాంశం *b = list_నమోదు(pos, నిర్మాణం పునాది_దత్తాంశం, list);
		ఒకవేళ (b->retain_దత్తాంశం || b == retain)
			కొనసాగు;
		ఒకవేళ (b->దత్తాంశం) {
			free_పునాది_దత్తాంశం(b);
			ఒకవేళ (పునాది_cache_వాడిన <= పునాది_cache_హద్దు)
				తిరిగి;
		}
	}

	list_for_each_prev(pos, &work_తల) {
		నిర్మాణం పునాది_దత్తాంశం *b = list_నమోదు(pos, నిర్మాణం పునాది_దత్తాంశం, list);
		ఒకవేళ (b->retain_దత్తాంశం || b == retain)
			కొనసాగు;
		ఒకవేళ (b->దత్తాంశం) {
			free_పునాది_దత్తాంశం(b);
			ఒకవేళ (పునాది_cache_వాడిన <= పునాది_cache_హద్దు)
				తిరిగి;
		}
	}
}

స్థిర పూర్ణాంకం is_delta_type(గణన object_type type)
{
	తిరిగి (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

స్థిర శూన్య *unpack_నమోదు_దత్తాంశం(off_t స్థానభ్రంశం, కొలత_t కొలత,
			       గణన object_type type, నిర్మాణం object_id *oid)
{
	స్థిర అక్షరం fixed_buf[8192];
	పూర్ణాంకం స్థితి_2;
	git_zstream ప్రవాహం;
	శూన్య *buf;
	నిర్మాణం git_hash_ctx c;
	అక్షరం hdr[32];
	పూర్ణాంకం hdrlen;

	ఒకవేళ (!is_delta_type(type)) {
		hdrlen = format_object_శీర్షిక(hdr, పరిమాణం(hdr), type, కొలత);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} లేకపోతే
		oid = ఖాళీ;
	ఒకవేళ (type == OBJ_BLOB &&
	    కొలత > repo_settings_get_big_file_పరిమితి(the_repository))
		buf = fixed_buf;
	లేకపోతే
		buf = xmallocz(కొలత);

	memset(&ప్రవాహం, 0, పరిమాణం(ప్రవాహం));
	git_inflate_init(&ప్రవాహం);
	ప్రవాహం.తదుపరి_బహిర్గతం = buf;
	ప్రవాహం.avail_బహిర్గతం = buf == fixed_buf ? పరిమాణం(fixed_buf) : కొలత;

	చేయి {
		గుర్తులేని అక్షరం *చివరి_బహిర్గతం = ప్రవాహం.తదుపరి_బహిర్గతం;
		ప్రవాహం.తదుపరి_లోపల = fill(1);
		ప్రవాహం.avail_లోపల = ఆదానం_len;
		స్థితి_2 = git_inflate(&ప్రవాహం, 0);
		use(ఆదానం_len - ప్రవాహం.avail_లోపల);
		ఒకవేళ (oid)
			git_hash_update(&c, చివరి_బహిర్గతం, ప్రవాహం.తదుపరి_బహిర్గతం - చివరి_బహిర్గతం);
		ఒకవేళ (buf == fixed_buf) {
			ప్రవాహం.తదుపరి_బహిర్గతం = buf;
			ప్రవాహం.avail_బహిర్గతం = పరిమాణం(fixed_buf);
		}
	} అయితే (స్థితి_2 == Z_OK);
	ఒకవేళ (ప్రవాహం.మొత్తం_బహిర్గతం != కొలత || స్థితి_2 != Z_ప్రవాహం_చివర)
		bad_object(స్థానభ్రంశం, _("inflate returned %d"), స్థితి_2);
	git_inflate_చివర(&ప్రవాహం);
	ఒకవేళ (oid)
		git_hash_final_oid(oid, &c);
	తిరిగి buf == fixed_buf ? ఖాళీ : buf;
}

స్థిర శూన్య *unpack_raw_నమోదు(నిర్మాణం object_నమోదు *obj,
			      off_t *ofs_స్థానభ్రంశం,
			      నిర్మాణం object_id *ref_oid,
			      నిర్మాణం object_id *oid)
{
	గుర్తులేని అక్షరం *p;
	కొలత_t కొలత, c;
	off_t పునాది_స్థానభ్రంశం;
	గుర్తులేని జరుపు;
	శూన్య *దత్తాంశం;

	obj->idx.స్థానభ్రంశం = consumed_బైట్లు;
	ఆదానం_చక్రీయ_తనిఖీ_32 = చక్రీయ_తనిఖీ_32(0, ఖాళీ, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	కొలత = (c & 15);
	జరుపు = 4;
	అయితే (c & 0x80) {
		ఒకవేళ ((bitsizeof(కొలత_t) - 7) < జరుపు)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		కొలత += (c & 0x7f) << జరుపు;
		జరుపు += 7;
	}
	obj->కొలత = కొలత;

	ఎంపిక (obj->type) {
	స్థితి OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		ఆపు;
	స్థితి OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		పునాది_స్థానభ్రంశం = c & 127;
		అయితే (c & 128) {
			పునాది_స్థానభ్రంశం += 1;
			ఒకవేళ (!పునాది_స్థానభ్రంశం || MSB(పునాది_స్థానభ్రంశం, 7))
				bad_object(obj->idx.స్థానభ్రంశం, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			పునాది_స్థానభ్రంశం = (పునాది_స్థానభ్రంశం << 7) + (c & 127);
		}
		*ofs_స్థానభ్రంశం = obj->idx.స్థానభ్రంశం - పునాది_స్థానభ్రంశం;
		ఒకవేళ (*ofs_స్థానభ్రంశం <= 0 || *ofs_స్థానభ్రంశం >= obj->idx.స్థానభ్రంశం)
			bad_object(obj->idx.స్థానభ్రంశం, _("delta base offset is out of bound"));
		ఆపు;
	స్థితి OBJ_COMMIT:
	స్థితి OBJ_వృక్షం:
	స్థితి OBJ_BLOB:
	స్థితి OBJ_ట్యాగ్:
		ఆపు;
	అప్రమేయ:
		bad_object(obj->idx.స్థానభ్రంశం, _("unknown object type %d"), obj->type);
	}
	obj->hdr_కొలత = consumed_బైట్లు - obj->idx.స్థానభ్రంశం;

	దత్తాంశం = unpack_నమోదు_దత్తాంశం(obj->idx.స్థానభ్రంశం, obj->కొలత, obj->type, oid);
	obj->idx.చక్రీయ_తనిఖీ_32 = ఆదానం_చక్రీయ_తనిఖీ_32;
	తిరిగి దత్తాంశం;
}

స్థిర శూన్య *unpack_దత్తాంశం(నిర్మాణం object_నమోదు *obj,
			 పూర్ణాంకం (*consume)(స్థిరరాశి గుర్తులేని అక్షరం *, గుర్తులేని దీర్ఘ, శూన్య *),
			 శూన్య *cb_దత్తాంశం)
{
	off_t from = obj[0].idx.స్థానభ్రంశం + obj[0].hdr_కొలత;
	off_t len = obj[1].idx.స్థానభ్రంశం - from;
	గుర్తులేని అక్షరం *దత్తాంశం, *inbuf;
	git_zstream ప్రవాహం;
	పూర్ణాంకం స్థితి_2;

	దత్తాంశం = xmallocz(consume ? 64*1024 : obj->కొలత);
	inbuf = xmalloc((len < 64*1024) ? (పూర్ణాంకం)len : 64*1024);

	memset(&ప్రవాహం, 0, పరిమాణం(ప్రవాహం));
	git_inflate_init(&ప్రవాహం);
	ప్రవాహం.తదుపరి_బహిర్గతం = దత్తాంశం;
	ప్రవాహం.avail_బహిర్గతం = consume ? 64*1024 : obj->కొలత;

	చేయి {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_దత్తాంశం()->pack_fd, inbuf, n, from);
		ఒకవేళ (n < 0)
			die_errno(_("cannot pread pack file"));
		ఒకవేళ (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		ప్రవాహం.తదుపరి_లోపల = inbuf;
		ప్రవాహం.avail_లోపల = n;
		ఒకవేళ (!consume)
			స్థితి_2 = git_inflate(&ప్రవాహం, 0);
		లేకపోతే {
			చేయి {
				స్థితి_2 = git_inflate(&ప్రవాహం, 0);
				ఒకవేళ (consume(దత్తాంశం, ప్రవాహం.తదుపరి_బహిర్గతం - దత్తాంశం, cb_దత్తాంశం)) {
					విడుదల(inbuf);
					విడుదల(దత్తాంశం);
					తిరిగి ఖాళీ;
				}
				ప్రవాహం.తదుపరి_బహిర్గతం = దత్తాంశం;
				ప్రవాహం.avail_బహిర్గతం = 64*1024;
			} అయితే (స్థితి_2 == Z_OK && ప్రవాహం.avail_లోపల);
		}
	} అయితే (len && స్థితి_2 == Z_OK && !ప్రవాహం.avail_లోపల);

	/* This has been inflated OK when first encountered, so... */
	ఒకవేళ (స్థితి_2 != Z_ప్రవాహం_చివర || ప్రవాహం.మొత్తం_బహిర్గతం != obj->కొలత)
		die(_("serious inflate inconsistency"));

	git_inflate_చివర(&ప్రవాహం);
	విడుదల(inbuf);
	ఒకవేళ (consume) {
		FREE_AND_NULL(దత్తాంశం);
	}
	తిరిగి దత్తాంశం;
}

స్థిర శూన్య *get_దత్తాంశం_from_pack(నిర్మాణం object_నమోదు *obj)
{
	తిరిగి unpack_దత్తాంశం(obj, ఖాళీ, ఖాళీ);
}

స్థిర పూర్ణాంకం compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   గణన object_type type1,
				   గణన object_type type2)
{
	పూర్ణాంకం cmp = type1 - type2;
	ఒకవేళ (cmp)
		తిరిగి cmp;
	తిరిగి offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

స్థిర పూర్ణాంకం కనుగొను_ofs_delta(స్థిరరాశి off_t స్థానభ్రంశం)
{
	పూర్ణాంకం మొదటి = 0, చివరి = nr_ofs_deltas;

	అయితే (మొదటి < చివరి) {
		పూర్ణాంకం తదుపరి = మొదటి + (చివరి - మొదటి) / 2;
		నిర్మాణం ofs_delta_నమోదు *delta = &ofs_deltas[తదుపరి];
		పూర్ణాంకం cmp;

		cmp = compare_ofs_delta_bases(స్థానభ్రంశం, delta->స్థానభ్రంశం,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		ఒకవేళ (!cmp)
			తిరిగి తదుపరి;
		ఒకవేళ (cmp < 0) {
			చివరి = తదుపరి;
			కొనసాగు;
		}
		మొదటి = తదుపరి+1;
	}
	తిరిగి -మొదటి-1;
}

స్థిర శూన్య కనుగొను_ofs_delta_children(off_t స్థానభ్రంశం,
				    పూర్ణాంకం *మొదటి_సూచీ, పూర్ణాంకం *చివరి_సూచీ)
{
	పూర్ణాంకం మొదటి = కనుగొను_ofs_delta(స్థానభ్రంశం);
	పూర్ణాంకం చివరి = మొదటి;
	పూర్ణాంకం చివర = nr_ofs_deltas - 1;

	ఒకవేళ (మొదటి < 0) {
		*మొదటి_సూచీ = 0;
		*చివరి_సూచీ = -1;
		తిరిగి;
	}
	అయితే (మొదటి > 0 && ofs_deltas[మొదటి - 1].స్థానభ్రంశం == స్థానభ్రంశం)
		--మొదటి;
	అయితే (చివరి < చివర && ofs_deltas[చివరి + 1].స్థానభ్రంశం == స్థానభ్రంశం)
		++చివరి;
	*మొదటి_సూచీ = మొదటి;
	*చివరి_సూచీ = చివరి;
}

స్థిర పూర్ణాంకం compare_ref_delta_bases(స్థిరరాశి నిర్మాణం object_id *oid1,
				   స్థిరరాశి నిర్మాణం object_id *oid2,
				   గణన object_type type1,
				   గణన object_type type2)
{
	పూర్ణాంకం cmp = type1 - type2;
	ఒకవేళ (cmp)
		తిరిగి cmp;
	తిరిగి oidcmp(oid1, oid2);
}

స్థిర పూర్ణాంకం కనుగొను_ref_delta(స్థిరరాశి నిర్మాణం object_id *oid)
{
	పూర్ణాంకం మొదటి = 0, చివరి = nr_ref_deltas;

	అయితే (మొదటి < చివరి) {
		పూర్ణాంకం తదుపరి = మొదటి + (చివరి - మొదటి) / 2;
		నిర్మాణం ref_delta_నమోదు *delta = &ref_deltas[తదుపరి];
		పూర్ణాంకం cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		ఒకవేళ (!cmp)
			తిరిగి తదుపరి;
		ఒకవేళ (cmp < 0) {
			చివరి = తదుపరి;
			కొనసాగు;
		}
		మొదటి = తదుపరి+1;
	}
	తిరిగి -మొదటి-1;
}

స్థిర శూన్య కనుగొను_ref_delta_children(స్థిరరాశి నిర్మాణం object_id *oid,
				    పూర్ణాంకం *మొదటి_సూచీ, పూర్ణాంకం *చివరి_సూచీ)
{
	పూర్ణాంకం మొదటి = కనుగొను_ref_delta(oid);
	పూర్ణాంకం చివరి = మొదటి;
	పూర్ణాంకం చివర = nr_ref_deltas - 1;

	ఒకవేళ (మొదటి < 0) {
		*మొదటి_సూచీ = 0;
		*చివరి_సూచీ = -1;
		తిరిగి;
	}
	అయితే (మొదటి > 0 && oideq(&ref_deltas[మొదటి - 1].oid, oid))
		--మొదటి;
	అయితే (చివరి < చివర && oideq(&ref_deltas[చివరి + 1].oid, oid))
		++చివరి;
	*మొదటి_సూచీ = మొదటి;
	*చివరి_సూచీ = చివరి;
}

నిర్మాణం compare_దత్తాంశం {
	నిర్మాణం object_నమోదు *నమోదు_2;
	నిర్మాణం odb_ప్రవాహం *st;
	గుర్తులేని అక్షరం *buf;
	గుర్తులేని దీర్ఘ buf_కొలత;
};

స్థిర పూర్ణాంకం compare_objects(స్థిరరాశి గుర్తులేని అక్షరం *buf, గుర్తులేని దీర్ఘ కొలత,
			   శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం compare_దత్తాంశం *దత్తాంశం = cb_దత్తాంశం;

	ఒకవేళ (దత్తాంశం->buf_కొలత < కొలత) {
		విడుదల(దత్తాంశం->buf);
		దత్తాంశం->buf = xmalloc(కొలత);
		దత్తాంశం->buf_కొలత = కొలత;
	}

	అయితే (కొలత) {
		ssize_t len = odb_ప్రవాహం_read(దత్తాంశం->st, దత్తాంశం->buf, కొలత);
		ఒకవేళ (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_గమ్యం_hex(&దత్తాంశం->నమోదు_2->idx.oid));
		ఒకవేళ (len < 0)
			die(_("unable to read %s"),
			    oid_గమ్యం_hex(&దత్తాంశం->నమోదు_2->idx.oid));
		ఒకవేళ (memcmp(buf, దత్తాంశం->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_గమ్యం_hex(&దత్తాంశం->నమోదు_2->idx.oid));
		కొలత -= len;
		buf += len;
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం check_collison(నిర్మాణం object_నమోదు *నమోదు_2)
{
	నిర్మాణం compare_దత్తాంశం దత్తాంశం;

	ఒకవేళ (నమోదు_2->కొలత <= repo_settings_get_big_file_పరిమితి(the_repository) ||
	    నమోదు_2->type != OBJ_BLOB)
		తిరిగి -1;

	memset(&దత్తాంశం, 0, పరిమాణం(దత్తాంశం));
	దత్తాంశం.నమోదు_2 = నమోదు_2;
	దత్తాంశం.st = odb_ప్రవాహం_from_object(the_repository->objects, &నమోదు_2->idx.oid, ఖాళీ);
	ఒకవేళ (!దత్తాంశం.st)
		తిరిగి -1;
	ఒకవేళ (దత్తాంశం.st->కొలత != నమోదు_2->కొలత || దత్తాంశం.st->type != నమోదు_2->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_గమ్యం_hex(&నమోదు_2->idx.oid));
	unpack_దత్తాంశం(నమోదు_2, compare_objects, &దత్తాంశం);
	odb_ప్రవాహం_close(దత్తాంశం.st);
	విడుదల(దత్తాంశం.buf);
	తిరిగి 0;
}

స్థిర శూన్య రికార్డు_outgoing_link(స్థిరరాశి నిర్మాణం object_id *oid)
{
	oidset_చొప్పించు(&outgoing_links, oid);
}

స్థిర శూన్య maybe_రికార్డు_name_నమోదు(స్థిరరాశి నిర్మాణం name_నమోదు *నమోదు_2)
{
	/*
	 * Checking only trees here results in a significantly faster packfile
	 * indexing, but the drawback is that if the packfile to be indexed
	 * references a local blob only directly (that is, never through a
	 * local tree), that local blob is in danger of being garbage
	 * collected. Such a situation may arise if we push local commits,
	 * including one with a change to a blob in the root tree, and then the
	 * server incorporates them into its main branch through a "rebase" or
	 * "squash" merge strategy, and then we fetch the new main branch from
	 * the server.
	 *
	 * This situation has not been observed yet - we have only noticed
	 * missing commits, not missing trees or blobs. (In fact, if it were
	 * believed that only missing commits are problematic, one could argue
	 * that we should also exclude trees during the outgoing link check;
	 * but it is safer to include them.)
	 *
	 * Due to the rarity of the situation (it has not been observed to
	 * happen in real life), and because the "penalty" in such a situation
	 * is merely to refetch the missing blob when it's needed (and this
	 * happens only once - when refetched, the blob goes into a promisor
	 * pack, so it won't be GC-ed, the tradeoff seems worth it.
	*/
	ఒకవేళ (S_ISDIR(నమోదు_2->బహుళకం))
		రికార్డు_outgoing_link(&నమోదు_2->oid);
}

స్థిర శూన్య do_రికార్డు_outgoing_links(నిర్మాణం object *obj)
{
	ఒకవేళ (obj->type == OBJ_వృక్షం) {
		నిర్మాణం వృక్షం *వృక్షం = (నిర్మాణం వృక్షం *)obj;
		నిర్మాణం వృక్షం_desc desc;
		నిర్మాణం name_నమోదు నమోదు_2;
		ఒకవేళ (init_వృక్షం_desc_gently(&desc, &వృక్షం->object.oid,
					  వృక్షం->బఫరు, వృక్షం->కొలత, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			తిరిగి;
		అయితే (వృక్షం_నమోదు_gently(&desc, &నమోదు_2))
			maybe_రికార్డు_name_నమోదు(&నమోదు_2);
	} లేకపోతే ఒకవేళ (obj->type == OBJ_COMMIT) {
		నిర్మాణం commit *commit = (నిర్మాణం commit *) obj;
		నిర్మాణం commit_list *parents = commit->parents;

		రికార్డు_outgoing_link(get_commit_వృక్షం_oid(commit));
		కొరకు (; parents; parents = parents->తదుపరి)
			రికార్డు_outgoing_link(&parents->అంశం->object.oid);
	} లేకపోతే ఒకవేళ (obj->type == OBJ_ట్యాగ్) {
		నిర్మాణం ట్యాగ్ *ట్యాగ్ = (నిర్మాణం ట్యాగ్ *) obj;
		రికార్డు_outgoing_link(get_tagged_oid(ట్యాగ్));
	}
}

స్థిర శూన్య sha1_object(స్థిరరాశి శూన్య *దత్తాంశం, నిర్మాణం object_నమోదు *obj_నమోదు,
			గుర్తులేని దీర్ఘ కొలత, గణన object_type type,
			స్థిరరాశి నిర్మాణం object_id *oid)
{
	శూన్య *new_దత్తాంశం = ఖాళీ;
	పూర్ణాంకం collision_test_needed = 0;

	ప్రతిజ్ఞా(దత్తాంశం || obj_నమోదు);

	ఒకవేళ (startup_info->have_repository) {
		read_తాళం();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	ఒకవేళ (collision_test_needed && !దత్తాంశం) {
		read_తాళం();
		ఒకవేళ (!check_collison(obj_నమోదు))
			collision_test_needed = 0;
		read_unlock();
	}
	ఒకవేళ (collision_test_needed) {
		శూన్య *has_దత్తాంశం;
		గణన object_type has_type;
		కొలత_t has_కొలత;
		read_తాళం();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_కొలత);
		ఒకవేళ (has_type < 0)
			die(_("cannot read existing object info %s"), oid_గమ్యం_hex(oid));
		ఒకవేళ (has_type != type || has_కొలత != కొలత)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_గమ్యం_hex(oid));
		has_దత్తాంశం = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_కొలత);
		read_unlock();
		ఒకవేళ (!దత్తాంశం)
			దత్తాంశం = new_దత్తాంశం = get_దత్తాంశం_from_pack(obj_నమోదు);
		ఒకవేళ (!has_దత్తాంశం)
			die(_("cannot read existing object %s"), oid_గమ్యం_hex(oid));
		ఒకవేళ (కొలత != has_కొలత || type != has_type ||
		    memcmp(దత్తాంశం, has_దత్తాంశం, కొలత) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_గమ్యం_hex(oid));
		విడుదల(has_దత్తాంశం);
	}

	ఒకవేళ (strict || do_fsck_object || రికార్డు_outgoing_links) {
		read_తాళం();
		ఒకవేళ (type == OBJ_BLOB) {
			నిర్మాణం blob *blob = lookup_blob(the_repository, oid);
			ఒకవేళ (blob)
				blob->object.flags |= FLAG_CHECKED;
			లేకపోతే
				die(_("invalid blob object %s"), oid_గమ్యం_hex(oid));
			ఒకవేళ (do_fsck_object &&
			    fsck_object(&blob->object, (శూన్య *)దత్తాంశం, కొలత, &fsck_options))
				die(_("fsck error in packed object"));
		} లేకపోతే {
			నిర్మాణం object *obj;
			పూర్ణాంకం eaten;
			శూన్య *buf = (శూన్య *) దత్తాంశం;

			ప్రతిజ్ఞా(దత్తాంశం && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_బఫరు(the_repository, oid, type,
						  కొలత, buf,
						  &eaten);
			ఒకవేళ (!obj)
				die(_("invalid %s"), type_name(type));
			ఒకవేళ (do_fsck_object &&
			    fsck_object(obj, buf, కొలత, &fsck_options))
				die(_("fsck error in packed object"));
			ఒకవేళ (strict && fsck_walk(obj, ఖాళీ, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_గమ్యం_hex(&obj->oid));
			ఒకవేళ (రికార్డు_outgoing_links)
				do_రికార్డు_outgoing_links(obj);

			ఒకవేళ (obj->type == OBJ_వృక్షం) {
				నిర్మాణం వృక్షం *అంశం = (నిర్మాణం వృక్షం *) obj;
				అంశం->బఫరు = ఖాళీ;
				obj->parsed = 0;
			}
			ఒకవేళ (obj->type == OBJ_COMMIT) {
				నిర్మాణం commit *commit = (నిర్మాణం commit *) obj;
				ఒకవేళ (detach_commit_బఫరు(commit, ఖాళీ) != దత్తాంశం)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	విడుదల(new_దత్తాంశం);
}

/*
 * Ensure that this node has been reconstructed and return its contents.
 *
 * In the typical and best case, this node would already be reconstructed
 * (through the invocation to resolve_delta() in threaded_second_pass()) and it
 * would not be pruned. However, if pruning of this node was necessary due to
 * reaching delta_base_cache_limit, this function will find the closest
 * ancestor with reconstructed data that has not been pruned (or if there is
 * none, the ultimate base object), and reconstruct each node in the delta
 * chain in order to generate the reconstructed data for this node.
 */
స్థిర శూన్య *get_పునాది_దత్తాంశం(నిర్మాణం పునాది_దత్తాంశం *c)
{
	ఒకవేళ (!c->దత్తాంశం) {
		నిర్మాణం object_నమోదు *obj = c->obj;
		నిర్మాణం పునాది_దత్తాంశం **delta = ఖాళీ;
		పూర్ణాంకం delta_nr = 0, delta_alloc = 0;

		అయితే (is_delta_type(c->obj->type) && !c->దత్తాంశం) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->పునాది;
		}
		ఒకవేళ (!delta_nr) {
			c->దత్తాంశం = get_దత్తాంశం_from_pack(obj);
			c->కొలత = obj->కొలత;
			పునాది_cache_వాడిన += c->కొలత;
			prune_పునాది_దత్తాంశం(c);
		}
		కొరకు (; delta_nr > 0; delta_nr--) {
			శూన్య *పునాది, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			పునాది = get_పునాది_దత్తాంశం(c->పునాది);
			raw = get_దత్తాంశం_from_pack(obj);
			c->దత్తాంశం = patch_delta(
				పునాది, c->పునాది->కొలత,
				raw, obj->కొలత,
				&c->కొలత);
			విడుదల(raw);
			ఒకవేళ (!c->దత్తాంశం)
				bad_object(obj->idx.స్థానభ్రంశం, _("failed to apply delta"));
			పునాది_cache_వాడిన += c->కొలత;
			prune_పునాది_దత్తాంశం(c);
		}
		విడుదల(delta);
	}
	తిరిగి c->దత్తాంశం;
}

స్థిర నిర్మాణం పునాది_దత్తాంశం *make_పునాది(నిర్మాణం object_నమోదు *obj,
				   నిర్మాణం పునాది_దత్తాంశం *మాతృ)
{
	నిర్మాణం పునాది_దత్తాంశం *పునాది = xcalloc(1, పరిమాణం(నిర్మాణం పునాది_దత్తాంశం));
	పునాది->పునాది = మాతృ;
	పునాది->obj = obj;
	కనుగొను_ref_delta_children(&obj->idx.oid,
				&పునాది->ref_మొదటి, &పునాది->ref_చివరి);
	కనుగొను_ofs_delta_children(obj->idx.స్థానభ్రంశం,
				&పునాది->ofs_మొదటి, &పునాది->ofs_చివరి);
	పునాది->children_మిగిలిన = పునాది->ref_చివరి - పునాది->ref_మొదటి +
		పునాది->ofs_చివరి - పునాది->ofs_మొదటి + 2;
	తిరిగి పునాది;
}

స్థిర నిర్మాణం పునాది_దత్తాంశం *resolve_delta(నిర్మాణం object_నమోదు *delta_obj,
				       నిర్మాణం పునాది_దత్తాంశం *పునాది)
{
	శూన్య *delta_దత్తాంశం, *ఫలితం_దత్తాంశం;
	నిర్మాణం పునాది_దత్తాంశం *ఫలితం;
	కొలత_t ఫలితం_కొలత;

	ఒకవేళ (show_stat) {
		పూర్ణాంకం i = delta_obj - objects;
		పూర్ణాంకం j = పునాది->obj - objects;
		obj_stat[i].delta_లోతు = obj_stat[j].delta_లోతు + 1;
		అత్యంతలోతు_delta_తాళం();
		ఒకవేళ (అత్యంతలోతు_delta < obj_stat[i].delta_లోతు)
			అత్యంతలోతు_delta = obj_stat[i].delta_లోతు;
		అత్యంతలోతు_delta_unlock();
		obj_stat[i].పునాది_object_no = j;
	}
	delta_దత్తాంశం = get_దత్తాంశం_from_pack(delta_obj);
	ప్రతిజ్ఞా(పునాది->దత్తాంశం);
	ఫలితం_దత్తాంశం = patch_delta(పునాది->దత్తాంశం, పునాది->కొలత,
				  delta_దత్తాంశం, delta_obj->కొలత, &ఫలితం_కొలత);
	విడుదల(delta_దత్తాంశం);
	ఒకవేళ (!ఫలితం_దత్తాంశం)
		bad_object(delta_obj->idx.స్థానభ్రంశం, _("failed to apply delta"));
	hash_object_file(the_hash_algo, ఫలితం_దత్తాంశం, ఫలితం_కొలత,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(ఫలితం_దత్తాంశం, ఖాళీ, ఫలితం_కొలత, delta_obj->real_type,
		    &delta_obj->idx.oid);

	ఫలితం = make_పునాది(delta_obj, పునాది);
	ఫలితం->దత్తాంశం = ఫలితం_దత్తాంశం;
	ఫలితం->కొలత = ఫలితం_కొలత;

	counter_తాళం();
	nr_resolved_deltas++;
	counter_unlock();

	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం compare_ofs_delta_నమోదు(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
	స్థిరరాశి నిర్మాణం ofs_delta_నమోదు *delta_a = a;
	స్థిరరాశి నిర్మాణం ofs_delta_నమోదు *delta_b = b;

	తిరిగి delta_a->స్థానభ్రంశం < delta_b->స్థానభ్రంశం ? -1 :
	       delta_a->స్థానభ్రంశం > delta_b->స్థానభ్రంశం ?  1 :
	       0;
}

స్థిర పూర్ణాంకం compare_ref_delta_నమోదు(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
	స్థిరరాశి నిర్మాణం ref_delta_నమోదు *delta_a = a;
	స్థిరరాశి నిర్మాణం ref_delta_నమోదు *delta_b = b;

	తిరిగి oidcmp(&delta_a->oid, &delta_b->oid);
}

స్థిర శూన్య *threaded_రెండవ_pass(శూన్య *దత్తాంశం)
{
	ఒకవేళ (దత్తాంశం)
		సమితి_thread_దత్తాంశం(దత్తాంశం);
	కొరకు (;;) {
		నిర్మాణం పునాది_దత్తాంశం *మాతృ = ఖాళీ;
		నిర్మాణం object_నమోదు *బిడ్డ_obj = ఖాళీ;
		నిర్మాణం పునాది_దత్తాంశం *బిడ్డ = ఖాళీ;

		counter_తాళం();
		display_పురోగతి(పురోగతి, nr_resolved_deltas);
		counter_unlock();

		work_తాళం();
		ఒకవేళ (list_ఖాళీలు(&work_తల)) {
			/*
			 * Take an object from the object array.
			 */
			అయితే (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			ఒకవేళ (nr_dispatched >= nr_objects) {
				work_unlock();
				ఆపు;
			}
			బిడ్డ_obj = &objects[nr_dispatched++];
		} లేకపోతే {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			మాతృ = list_మొదటి_నమోదు(&work_తల, నిర్మాణం పునాది_దత్తాంశం,
						  list);

			అయితే (మాతృ->ref_మొదటి <= మాతృ->ref_చివరి) {
				పూర్ణాంకం స్థానభ్రంశం = ref_deltas[మాతృ->ref_మొదటి++].obj_no;
				బిడ్డ_obj = objects + స్థానభ్రంశం;
				ఒకవేళ (బిడ్డ_obj->real_type != OBJ_REF_DELTA) {
					బిడ్డ_obj = ఖాళీ;
					కొనసాగు;
				}
				బిడ్డ_obj->real_type = మాతృ->obj->real_type;
				ఆపు;
			}

			ఒకవేళ (!బిడ్డ_obj && మాతృ->ofs_మొదటి <= మాతృ->ofs_చివరి) {
				బిడ్డ_obj = objects +
					ofs_deltas[మాతృ->ofs_మొదటి++].obj_no;
				ప్రతిజ్ఞా(బిడ్డ_obj->real_type == OBJ_OFS_DELTA);
				బిడ్డ_obj->real_type = మాతృ->obj->real_type;
			}

			ఒకవేళ (మాతృ->ref_మొదటి > మాతృ->ref_చివరి &&
			    మాతృ->ofs_మొదటి > మాతృ->ofs_చివరి) {
				/*
				 * This parent has run out of children, so move
				 * it to done_head.
				 */
				list_del(&మాతృ->list);
				list_కూడు(&మాతృ->list, &పూర్తి_తల);
			}

			/*
			 * Ensure that the parent has data, since we will need
			 * it later.
			 *
			 * NEEDSWORK: If parent data needs to be reloaded, this
			 * prolongs the time that the current thread spends in
			 * the mutex. A mitigating factor is that parent data
			 * needs to be reloaded only if the delta base cache
			 * limit is exceeded, so in the typical case, this does
			 * not happen.
			 */
			get_పునాది_దత్తాంశం(మాతృ);
			మాతృ->retain_దత్తాంశం++;
		}
		work_unlock();

		ఒకవేళ (బిడ్డ_obj) {
			ఒకవేళ (మాతృ) {
				బిడ్డ = resolve_delta(బిడ్డ_obj, మాతృ);
				ఒకవేళ (!బిడ్డ->children_మిగిలిన)
					FREE_AND_NULL(బిడ్డ->దత్తాంశం);
			} లేకపోతే{
				బిడ్డ = make_పునాది(బిడ్డ_obj, ఖాళీ);
				ఒకవేళ (బిడ్డ->children_మిగిలిన) {
					/*
					 * Since this child has its own delta children,
					 * we will need this data in the future.
					 * Inflate now so that future iterations will
					 * have access to this object's data while
					 * outside the work mutex.
					 */
					బిడ్డ->దత్తాంశం = get_దత్తాంశం_from_pack(బిడ్డ_obj);
					బిడ్డ->కొలత = బిడ్డ_obj->కొలత;
				}
			}
		}

		work_తాళం();
		ఒకవేళ (మాతృ)
			మాతృ->retain_దత్తాంశం--;

		ఒకవేళ (బిడ్డ && బిడ్డ->దత్తాంశం) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_కూడు(&బిడ్డ->list, &work_తల);
			పునాది_cache_వాడిన += బిడ్డ->కొలత;
			prune_పునాది_దత్తాంశం(ఖాళీ);
		} లేకపోతే ఒకవేళ (బిడ్డ) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			నిర్మాణం పునాది_దత్తాంశం *p = మాతృ;

			అయితే (p) {
				నిర్మాణం పునాది_దత్తాంశం *తదుపరి_p;

				p->children_మిగిలిన--;
				ఒకవేళ (p->children_మిగిలిన)
					ఆపు;

				తదుపరి_p = p->పునాది;
				free_పునాది_దత్తాంశం(p);
				list_del(&p->list);
				విడుదల(p);

				p = తదుపరి_p;
			}
			FREE_AND_NULL(బిడ్డ);
		}
		work_unlock();
	}
	తిరిగి ఖాళీ;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
స్థిర శూన్య parse_pack_objects(గుర్తులేని అక్షరం *hash)
{
	పూర్ణాంకం i, nr_delays = 0;
	నిర్మాణం ofs_delta_నమోదు *ofs_delta = ofs_deltas;
	నిర్మాణం object_id ref_delta_oid;
	నిర్మాణం stat st;
	నిర్మాణం git_hash_ctx tmp_ctx;

	ఒకవేళ (verbose)
		పురోగతి = ప్రారంభం_పురోగతి(
				the_repository,
				పురోగతి_title ? పురోగతి_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	కొరకు (i = 0; i < nr_objects; i++) {
		నిర్మాణం object_నమోదు *obj = &objects[i];
		శూన్య *దత్తాంశం = unpack_raw_నమోదు(obj, &ofs_delta->స్థానభ్రంశం,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		ఒకవేళ (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} లేకపోతే ఒకవేళ (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} లేకపోతే ఒకవేళ (!దత్తాంశం) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} లేకపోతే
			sha1_object(దత్తాంశం, ఖాళీ, obj->కొలత, obj->type,
				    &obj->idx.oid);
		విడుదల(దత్తాంశం);
		display_పురోగతి(పురోగతి, i+1);
	}
	objects[i].idx.స్థానభ్రంశం = consumed_బైట్లు;
	stop_పురోగతి(&పురోగతి);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ఆదానం_ctx);
	git_hash_final(hash, &tmp_ctx);
	ఒకవేళ (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	ఒకవేళ (fstat(ఆదానం_fd, &st))
		die_errno(_("cannot fstat packfile"));
	ఒకవేళ (S_ISREG(st.st_బహుళకం) &&
			lseek(ఆదానం_fd, 0, SEEK_CUR) - ఆదానం_len != st.st_కొలత)
		die(_("pack has junk at the end"));

	కొరకు (i = 0; i < nr_objects; i++) {
		నిర్మాణం object_నమోదు *obj = &objects[i];
		ఒకవేళ (obj->real_type != OBJ_BAD)
			కొనసాగు;
		obj->real_type = obj->type;
		sha1_object(ఖాళీ, obj, obj->కొలత, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	ఒకవేళ (nr_delays)
		die(_("confusion beyond insanity in parse_pack_objects()"));
}

/*
 * Second pass:
 * - for all non-delta objects, look if it is used as a base for
 *   deltas;
 * - if used as a base, uncompress the object and apply all deltas,
 *   recursively checking if the resulting object is used as a base
 *   for some more deltas.
 */
స్థిర శూన్య resolve_deltas(నిర్మాణం pack_idx_option *opts)
{
	పూర్ణాంకం i;

	ఒకవేళ (!nr_ofs_deltas && !nr_ref_deltas)
		తిరిగి;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_నమోదు);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_నమోదు);

	ఒకవేళ (verbose || show_resolving_పురోగతి)
		పురోగతి = ప్రారంభం_పురోగతి(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	పునాది_cache_హద్దు = opts->delta_పునాది_cache_హద్దు * nr_దారాలు;
	ఒకవేళ (nr_దారాలు > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		కొరకు (i = 0; i < nr_దారాలు; i++) {
			పూర్ణాంకం ret = pthread_create(&thread_దత్తాంశం[i].thread, ఖాళీ,
						 threaded_రెండవ_pass, thread_దత్తాంశం + i);
			ఒకవేళ (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		కొరకు (i = 0; i < nr_దారాలు; i++)
			pthread_join(thread_దత్తాంశం[i].thread, ఖాళీ);
		cleanup_thread();
		తిరిగి;
	}
	threaded_రెండవ_pass(&nothread_దత్తాంశం);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
స్థిర శూన్య fix_unresolved_deltas(నిర్మాణం hashfile *f);
స్థిర శూన్య conclude_pack(పూర్ణాంకం fix_thin_pack, స్థిరరాశి అక్షరం *curr_pack, గుర్తులేని అక్షరం *pack_hash)
{
	ఒకవేళ (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_పురోగతి(&పురోగతి);
		/* Flush remaining pack final hash. */
		flush();
		తిరిగి;
	}

	ఒకవేళ (fix_thin_pack) {
		నిర్మాణం hashfile *f;
		గుర్తులేని అక్షరం read_hash[GIT_MAX_RAWSZ], తోక_hash[GIT_MAX_RAWSZ];
		నిర్మాణం strbuf సందేశం = STRBUF_INIT;
		పూర్ణాంకం nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		పూర్ణాంకం nr_objects_initial = nr_objects;
		ఒకవేళ (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * పరిమాణం(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&సందేశం, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_పురోగతి_సందేశం(&పురోగతి, సందేశం.buf);
		strbuf_release(&సందేశం);
		finalize_hashfile(f, తోక_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_శీర్షిక_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_బైట్లు-the_hash_algo->rawsz);
		ఒకవేళ (!hasheq(read_hash, తోక_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	ఒకవేళ (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

స్థిర పూర్ణాంకం write_compressed(నిర్మాణం hashfile *f, శూన్య *లోపల, గుర్తులేని పూర్ణాంకం కొలత)
{
	git_zstream ప్రవాహం;
	పూర్ణాంకం స్థితి_2;
	గుర్తులేని అక్షరం outbuf[4096];
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	git_deflate_init(&ప్రవాహం, cfg->zlib_compression_level);
	ప్రవాహం.తదుపరి_లోపల = లోపల;
	ప్రవాహం.avail_లోపల = కొలత;

	చేయి {
		ప్రవాహం.తదుపరి_బహిర్గతం = outbuf;
		ప్రవాహం.avail_బహిర్గతం = పరిమాణం(outbuf);
		స్థితి_2 = git_deflate(&ప్రవాహం, Z_ముగింపు);
		hashwrite(f, outbuf, పరిమాణం(outbuf) - ప్రవాహం.avail_బహిర్గతం);
	} అయితే (స్థితి_2 == Z_OK);

	ఒకవేళ (స్థితి_2 != Z_ప్రవాహం_చివర)
		die(_("unable to deflate appended object (%d)"), స్థితి_2);
	కొలత = ప్రవాహం.మొత్తం_బహిర్గతం;
	git_deflate_చివర(&ప్రవాహం);
	తిరిగి కొలత;
}

స్థిర నిర్మాణం object_నమోదు *append_obj_గమ్యం_pack(నిర్మాణం hashfile *f,
			       స్థిరరాశి గుర్తులేని అక్షరం *sha1, శూన్య *buf,
			       గుర్తులేని దీర్ఘ కొలత, గణన object_type type)
{
	నిర్మాణం object_నమోదు *obj = &objects[nr_objects++];
	గుర్తులేని అక్షరం శీర్షిక[10];
	గుర్తులేని దీర్ఘ s = కొలత;
	పూర్ణాంకం n = 0;
	గుర్తులేని అక్షరం c = (type << 4) | (s & 15);
	s >>= 4;
	అయితే (s) {
		శీర్షిక[n++] = c | 0x80;
		c = s & 0x7f;
		s >>= 7;
	}
	శీర్షిక[n++] = c;
	చక్రీయ_తనిఖీ_32_begin(f);
	hashwrite(f, శీర్షిక, n);
	obj[0].కొలత = కొలత;
	obj[0].hdr_కొలత = n;
	obj[0].type = type;
	obj[0].real_type = type;
	obj[1].idx.స్థానభ్రంశం = obj[0].idx.స్థానభ్రంశం + n;
	obj[1].idx.స్థానభ్రంశం += write_compressed(f, buf, కొలత);
	obj[0].idx.చక్రీయ_తనిఖీ_32 = చక్రీయ_తనిఖీ_32_చివర(f);
	hashflush(f);
	oidread(&obj->idx.oid, sha1, the_repository->hash_algo);
	తిరిగి obj;
}

స్థిర పూర్ణాంకం delta_pos_compare(స్థిరరాశి శూన్య *_a, స్థిరరాశి శూన్య *_b)
{
	నిర్మాణం ref_delta_నమోదు *a = *(నిర్మాణం ref_delta_నమోదు **)_a;
	నిర్మాణం ref_delta_నమోదు *b = *(నిర్మాణం ref_delta_నమోదు **)_b;
	తిరిగి a->obj_no - b->obj_no;
}

స్థిర శూన్య fix_unresolved_deltas(నిర్మాణం hashfile *f)
{
	నిర్మాణం ref_delta_నమోదు **sorted_by_pos;
	పూర్ణాంకం i;

	/*
	 * Since many unresolved deltas may well be themselves base objects
	 * for more unresolved deltas, we really want to include the
	 * smallest number of base objects that would cover as much delta
	 * as possible by picking the
	 * trunc deltas first, allowing for other deltas to resolve without
	 * additional base objects.  Since most base objects are to be found
	 * before deltas depending on them, a good heuristic is to start
	 * resolving deltas in the same order as their position in the pack.
	 */
	ALLOC_ARRAY(sorted_by_pos, nr_ref_deltas);
	కొరకు (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	ఒకవేళ (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		నిర్మాణం oid_array గమ్యం_fetch = OID_ARRAY_INIT;
		కొరకు (i = 0; i < nr_ref_deltas; i++) {
			నిర్మాణం ref_delta_నమోదు *d = sorted_by_pos[i];
			ఒకవేళ (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, ఖాళీ,
							   OBJECT_INFO_FOR_PREFETCH))
				కొనసాగు;
			oid_array_append(&గమ్యం_fetch, &d->oid);
		}
		promisor_remote_get_ప్రత్యక్ష(the_repository,
					   గమ్యం_fetch.oid, గమ్యం_fetch.nr);
		oid_array_clear(&గమ్యం_fetch);
	}

	కొరకు (i = 0; i < nr_ref_deltas; i++) {
		నిర్మాణం ref_delta_నమోదు *d = sorted_by_pos[i];
		గణన object_type type;
		శూన్య *దత్తాంశం;
		కొలత_t కొలత;

		ఒకవేళ (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			కొనసాగు;
		దత్తాంశం = odb_read_object(the_repository->objects, &d->oid,
				       &type, &కొలత);
		ఒకవేళ (!దత్తాంశం)
			కొనసాగు;

		ఒకవేళ (check_object_signature(the_repository, &d->oid, దత్తాంశం, కొలత,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_గమ్యం_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_గమ్యం_pack(f, d->oid.hash, దత్తాంశం, కొలత, type);
		విడుదల(దత్తాంశం);
		threaded_రెండవ_pass(ఖాళీ);

		display_పురోగతి(పురోగతి, nr_resolved_deltas);
	}
	విడుదల(sorted_by_pos);
}

స్థిర స్థిరరాశి అక్షరం *derive_filename(స్థిరరాశి అక్షరం *pack_name, స్థిరరాశి అక్షరం *strip,
				   స్థిరరాశి అక్షరం *suffix, నిర్మాణం strbuf *buf)
{
	కొలత_t len;
	ఒకవేళ (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_కూడు(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	తిరిగి buf->buf;
}

స్థిర శూన్య write_special_file(స్థిరరాశి అక్షరం *suffix, స్థిరరాశి అక్షరం *సందేశం,
			       స్థిరరాశి అక్షరం *pack_name, స్థిరరాశి గుర్తులేని అక్షరం *hash,
			       స్థిరరాశి అక్షరం **report)
{
	నిర్మాణం strbuf name_buf = STRBUF_INIT;
	స్థిరరాశి అక్షరం *filename;
	పూర్ణాంకం fd;
	పూర్ణాంకం సందేశం_len = అక్షరశ్రేణి_పొడవు(సందేశం);

	ఒకవేళ (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	లేకపోతే
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = సురక్షిత_create_file_with_leading_directories(the_repository, filename);
	ఒకవేళ (fd < 0) {
		ఒకవేళ (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} లేకపోతే {
		ఒకవేళ (సందేశం_len > 0) {
			write_or_die(fd, సందేశం, సందేశం_len);
			write_or_die(fd, "\n", 1);
		}
		ఒకవేళ (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		ఒకవేళ (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

స్థిర శూన్య rename_tmp_packfile(స్థిరరాశి అక్షరం **final_name,
				స్థిరరాశి అక్షరం *curr_name,
				నిర్మాణం strbuf *name, గుర్తులేని అక్షరం *hash,
				స్థిరరాశి అక్షరం *ext, పూర్ణాంకం make_read_only_if_same)
{
	ఒకవేళ (!*final_name || strcmp(*final_name, curr_name)) {
		ఒకవేళ (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		ఒకవేళ (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} లేకపోతే ఒకవేళ (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

స్థిర శూన్య final(స్థిరరాశి అక్షరం *final_pack_name, స్థిరరాశి అక్షరం *curr_pack_name,
		  స్థిరరాశి అక్షరం *final_సూచీ_name, స్థిరరాశి అక్షరం *curr_సూచీ_name,
		  స్థిరరాశి అక్షరం *final_rev_సూచీ_name, స్థిరరాశి అక్షరం *curr_rev_సూచీ_name,
		  స్థిరరాశి అక్షరం *keep_సందేశం, స్థిరరాశి అక్షరం *promisor_సందేశం,
		  గుర్తులేని అక్షరం *hash)
{
	స్థిరరాశి అక్షరం *report = "pack";
	నిర్మాణం strbuf pack_name = STRBUF_INIT;
	నిర్మాణం strbuf సూచీ_name = STRBUF_INIT;
	నిర్మాణం strbuf rev_సూచీ_name = STRBUF_INIT;

	ఒకవేళ (!from_stdin) {
		close(ఆదానం_fd);
	} లేకపోతే {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		ఒకవేళ (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	ఒకవేళ (keep_సందేశం)
		write_special_file("keep", keep_సందేశం, final_pack_name, hash,
				   &report);
	ఒకవేళ (promisor_సందేశం)
		write_special_file("promisor", promisor_సందేశం, final_pack_name,
				   hash, ఖాళీ);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	ఒకవేళ (curr_rev_సూచీ_name)
		rename_tmp_packfile(&final_rev_సూచీ_name, curr_rev_సూచీ_name,
				    &rev_సూచీ_name, hash, "rev", 1);
	rename_tmp_packfile(&final_సూచీ_name, curr_సూచీ_name, &సూచీ_name,
			    hash, "idx", 1);

	ఒకవేళ (do_fsck_object && startup_info->have_repository) {
		నిర్మాణం odb_ఆధారం_files *files =
			odb_ఆధారం_files_downcast(the_repository->objects->ఆధారం);
		packfile_store_ఎక్కించు_pack(files->dirs->packed, final_సూచీ_name, 0);
	}

	ఒకవేళ (!from_stdin) {
		రాయి("%s\n", hash_గమ్యం_hex(hash));
	} లేకపోతే {
		నిర్మాణం strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_గమ్యం_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_లోపల_నిండిన(1, ఆదానం_బఫరు + ఆదానం_స్థానభ్రంశం, ఆదానం_len);
	}

	strbuf_release(&rev_సూచీ_name);
	strbuf_release(&సూచీ_name);
	strbuf_release(&pack_name);
}

స్థిర పూర్ణాంకం git_సూచీ_pack_config(స్థిరరాశి అక్షరం *k, స్థిరరాశి అక్షరం *v,
				 స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	నిర్మాణం pack_idx_option *opts = cb;

	ఒకవేళ (!strcmp(k, "pack.indexversion")) {
		opts->సంస్కరణ = git_config_int(k, v, ctx->kvi);
		ఒకవేళ (opts->సంస్కరణ > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->సంస్కరణ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "pack.threads")) {
		nr_దారాలు = git_config_int(k, v, ctx->kvi);
		ఒకవేళ (nr_దారాలు < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_దారాలు);
		ఒకవేళ (!HAVE_దారాలు && nr_దారాలు != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_దారాలు = 1;
		}
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(k, "pack.writereverseindex")) {
		ఒకవేళ (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		లేకపోతే
			opts->flags &= ~WRITE_REV;
	}
	ఒకవేళ (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_పునాది_cache_హద్దు = git_config_ulong(k, v, ctx->kvi);
		తిరిగి 0;
	}
	తిరిగి git_default_config(k, v, ctx, cb);
}

స్థిర పూర్ణాంకం cmp_uint32(స్థిరరాశి శూన్య *a_, స్థిరరాశి శూన్య *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	తిరిగి (a < b) ? -1 : (a != b);
}

స్థిర శూన్య read_v2_anomalous_offsets(నిర్మాణం packed_git *p,
				      నిర్మాణం pack_idx_option *opts)
{
	స్థిరరాశి uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((స్థిరరాశి uint32_t *)((స్థిరరాశి uint8_t *)p->సూచీ_దత్తాంశం + p->చక్రీయ_తనిఖీ_స్థానభ్రంశం))
		+ (కొలత_t)p->అంకె_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->అంకె_objects;

	కొరకు (i = 0; i < p->అంకె_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		ఒకవేళ (!(off & 0x80000000))
			కొనసాగు;
		off = off & 0x7fffffff;
		check_pack_సూచీ_ptr(p, &idx2[off * 2]);
		ఒకవేళ (idx2[off * 2])
			కొనసాగు;
		/*
		 * The real offset is ntohl(idx2[off * 2]) in high 4
		 * octets, and ntohl(idx2[off * 2 + 1]) in low 4
		 * octets.  But idx2[off * 2] is Zero!!!
		 */
		ALLOC_GROW(opts->anomaly, opts->anomaly_nr + 1, opts->anomaly_alloc);
		opts->anomaly[opts->anomaly_nr++] = ntohl(idx2[off * 2 + 1]);
	}

	QSORT(opts->anomaly, opts->anomaly_nr, cmp_uint32);
}

స్థిర శూన్య read_idx_option(నిర్మాణం pack_idx_option *opts, స్థిరరాశి అక్షరం *pack_name)
{
	నిర్మాణం packed_git *p = కూడు_packed_git(the_repository, pack_name,
					      అక్షరశ్రేణి_పొడవు(pack_name), 1);

	ఒకవేళ (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	ఒకవేళ (open_pack_సూచీ(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->సంస్కరణ = p->సూచీ_సంస్కరణ;

	ఒకవేళ (opts->సంస్కరణ == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_సూచీ(p);
	విడుదల(p);
}

స్థిర శూన్య show_pack_info(పూర్ణాంకం stat_only)
{
	పూర్ణాంకం i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	గుర్తులేని దీర్ఘ *chain_histogram = ఖాళీ;

	ఒకవేళ (అత్యంతలోతు_delta)
		CALLOC_ARRAY(chain_histogram, అత్యంతలోతు_delta);

	కొరకు (i = 0; i < nr_objects; i++) {
		నిర్మాణం object_నమోదు *obj = &objects[i];

		ఒకవేళ (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_లోతు - 1]++;
		ఒకవేళ (stat_only)
			కొనసాగు;
		రాయి("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_గమ్యం_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->కొలత,
		       (uintmax_t)(obj[1].idx.స్థానభ్రంశం - obj->idx.స్థానభ్రంశం),
		       (uintmax_t)obj->idx.స్థానభ్రంశం);
		ఒకవేళ (is_delta_type(obj->type)) {
			నిర్మాణం object_నమోదు *bobj = &objects[obj_stat[i].పునాది_object_no];
			రాయి(" %u %s", obj_stat[i].delta_లోతు,
			       oid_గమ్యం_hex(&bobj->idx.oid));
		}
		అక్షరం_రాయి('\n');
	}

	ఒకవేళ (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	కొరకు (i = 0; i < అత్యంతలోతు_delta; i++) {
		ఒకవేళ (!chain_histogram[i])
			కొనసాగు;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	విడుదల(chain_histogram);
}

స్థిర శూన్య repack_local_links(శూన్య)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;
	FILE *బహిర్గతం;
	నిర్మాణం strbuf పంక్తి = STRBUF_INIT;
	నిర్మాణం oidset_iter iter;
	నిర్మాణం object_id *oid;
	అక్షరం *పునాది_name = ఖాళీ;

	ఒకవేళ (!oidset_కొలత(&outgoing_links))
		తిరిగి;

	oidset_iter_init(&outgoing_links, &iter);
	అయితే ((oid = oidset_iter_తదుపరి(&iter))) {
		నిర్మాణం odb_ఆధారం_info ఆధారం_info;
		నిర్మాణం object_info info = {
			.ఆధారం_infop = &ఆధారం_info,
		};

		ఒకవేళ (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			కొనసాగు;
		ఒకవేళ (ఆధారం_info.ఆధారం->type == ODB_ఆధారం_PACKED &&
		    ఆధారం_info.u.packed.pack->pack_promisor)
			కొనసాగు;

		ఒకవేళ (!cmd.వాదనలు.nr) {
			పునాది_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_నెట్టు(&cmd.వాదనలు, "pack-objects");
			strvec_నెట్టు(&cmd.వాదనలు,
				    "--exclude-promisor-objects-best-effort");
			strvec_నెట్టు(&cmd.వాదనలు, పునాది_name);
			cmd.git_cmd = 1;
			cmd.లోపల = -1;
			cmd.బహిర్గతం = -1;
			ఒకవేళ (ప్రారంభం_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		ఒకవేళ (write_లోపల_నిండిన(cmd.లోపల, oid_గమ్యం_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_లోపల_నిండిన(cmd.లోపల, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	ఒకవేళ (!cmd.వాదనలు.nr)
		తిరిగి;

	close(cmd.లోపల);

	బహిర్గతం = xfdopen(cmd.బహిర్గతం, "r");
	అయితే (strbuf_getline_lf(&పంక్తి, బహిర్గతం) != EOF) {
		గుర్తులేని అక్షరం ద్విమాన[GIT_MAX_RAWSZ];
		ఒకవేళ (పంక్తి.len != the_hash_algo->hexsz ||
		    !hex_గమ్యం_బైట్లు(ద్విమాన, పంక్తి.buf, పంక్తి.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", ఖాళీ, ద్విమాన, ఖాళీ);
	}

	fclose(బహిర్గతం);
	ఒకవేళ (ముగింపు_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&పంక్తి);
	విడుదల(పునాది_name);
}

పూర్ణాంకం cmd_సూచీ_pack(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo)
{
	పూర్ణాంకం i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_సూచీ;
	స్థిరరాశి అక్షరం *curr_సూచీ;
	అక్షరం *curr_rev_సూచీ = ఖాళీ;
	స్థిరరాశి అక్షరం *సూచీ_name = ఖాళీ, *pack_name = ఖాళీ, *rev_సూచీ_name = ఖాళీ;
	స్థిరరాశి అక్షరం *keep_సందేశం = ఖాళీ;
	స్థిరరాశి అక్షరం *promisor_సందేశం = ఖాళీ;
	నిర్మాణం strbuf సూచీ_name_buf = STRBUF_INIT;
	నిర్మాణం strbuf rev_సూచీ_name_buf = STRBUF_INIT;
	నిర్మాణం pack_idx_నమోదు **idx_objects;
	నిర్మాణం pack_idx_option opts;
	గుర్తులేని అక్షరం pack_hash[GIT_MAX_RAWSZ];
	గుర్తులేని foreign_nr = 1;	/* zero is a "good" value, assume bad */
	పూర్ణాంకం report_చివర_యొక్క_ఆదానం = 0;
	పూర్ణాంకం hash_algo = 0;

	show_usage_if_asked(argc, argv, సూచీ_pack_usage);

	/*
	 * index-pack never needs to fetch missing objects except when
	 * REF_DELTA bases are missing (which are explicitly handled). It only
	 * accesses the repo to do hash collision checks and to check which
	 * REF_DELTA bases need to be fetched.
	 */
	(repo ? repo : the_repository)->fetch_if_missing = 0;

	disable_replace_refs();

	fsck_options_init(&fsck_options, the_repository, FSCK_OPTIONS_MISSING_GITMODULES);
	fsck_options.walk = mark_link;

	reset_pack_idx_option(&opts);
	opts.flags |= WRITE_REV;
	repo_config(the_repository, git_సూచీ_pack_config, &opts);
	ఒకవేళ (ఉపసర్గ && chdir(ఉపసర్గ))
		die(_("Cannot come back to cwd"));

	ఒకవేళ (git_env_bool(GIT_TEST_NO_WRITE_REV_సూచీ, 0))
		rev_సూచీ = 0;
	లేకపోతే
		rev_సూచీ = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	కొరకు (i = 1; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];

		ఒకవేళ (*arg == '-') {
			ఒకవేళ (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} లేకపోతే ఒకవేళ (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} లేకపోతే ఒకవేళ (skip_గమ్యం_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_సమితి_సందేశం_types(&fsck_options, arg);
			} లేకపోతే ఒకవేళ (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} లేకపోతే ఒకవేళ (skip_గమ్యం_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_సమితి_సందేశం_types(&fsck_options, arg);
			} లేకపోతే ఒకవేళ (!strcmp(arg, "--verify")) {
				verify = 1;
			} లేకపోతే ఒకవేళ (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} లేకపోతే ఒకవేళ (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} లేకపోతే ఒకవేళ (skip_గమ్యం_optional_arg(arg, "--keep", &keep_సందేశం)) {
				; /* nothing to do */
			} లేకపోతే ఒకవేళ (skip_గమ్యం_optional_arg(arg, "--promisor", &promisor_సందేశం)) {
				రికార్డు_outgoing_links = 1;
			} లేకపోతే ఒకవేళ (starts_with(arg, "--threads=")) {
				అక్షరం *చివర;
				nr_దారాలు = strtoul(arg+10, &చివర, 0);
				ఒకవేళ (!arg[10] || *చివర || nr_దారాలు < 0)
					usage(సూచీ_pack_usage);
				ఒకవేళ (!HAVE_దారాలు && nr_దారాలు != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_దారాలు = 1;
				}
			} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(arg, "--pack_header=", &arg)) {
				ఒకవేళ (parse_pack_శీర్షిక_option(arg,
							     ఆదానం_బఫరు,
							     &ఆదానం_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} లేకపోతే ఒకవేళ (!strcmp(arg, "-v")) {
				verbose = 1;
			} లేకపోతే ఒకవేళ (!strcmp(arg, "--progress-title")) {
				ఒకవేళ (పురోగతి_title || (i+1) >= argc)
					usage(సూచీ_pack_usage);
				పురోగతి_title = argv[++i];
			} లేకపోతే ఒకవేళ (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_పురోగతి = 1;
			} లేకపోతే ఒకవేళ (!strcmp(arg, "--report-end-of-input")) {
				report_చివర_యొక్క_ఆదానం = 1;
			} లేకపోతే ఒకవేళ (!strcmp(arg, "-o")) {
				ఒకవేళ (సూచీ_name || (i+1) >= argc)
					usage(సూచీ_pack_usage);
				సూచీ_name = argv[++i];
			} లేకపోతే ఒకవేళ (starts_with(arg, "--index-version=")) {
				అక్షరం *c;
				opts.సంస్కరణ = strtoul(arg + 16, &c, 10);
				ఒకవేళ (opts.సంస్కరణ > 2)
					die(_("bad %s"), arg);
				ఒకవేళ (*c == ',')
					opts.off32_హద్దు = strtoul(c+1, &c, 0);
				ఒకవేళ (*c || opts.off32_హద్దు & 0x80000000)
					die(_("bad %s"), arg);
			} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(arg, "--max-input-size=", &arg)) {
				max_ఆదానం_కొలత = strtoumax(arg, ఖాళీ, 10);
			} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				ఒకవేళ (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_సమితి_hash_algo(the_repository, hash_algo);
			} లేకపోతే ఒకవేళ (!strcmp(arg, "--rev-index")) {
				rev_సూచీ = 1;
			} లేకపోతే ఒకవేళ (!strcmp(arg, "--no-rev-index")) {
				rev_సూచీ = 0;
			} లేకపోతే
				usage(సూచీ_pack_usage);
			కొనసాగు;
		}

		ఒకవేళ (pack_name)
			usage(సూచీ_pack_usage);
		pack_name = arg;
	}

	ఒకవేళ (!pack_name && !from_stdin)
		usage(సూచీ_pack_usage);
	ఒకవేళ (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	ఒకవేళ (promisor_సందేశం && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	ఒకవేళ (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	ఒకవేళ (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	ఒకవేళ (!సూచీ_name && pack_name)
		సూచీ_name = derive_filename(pack_name, "pack", "idx", &సూచీ_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	ఒకవేళ (!the_repository->hash_algo)
		repo_సమితి_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	ఒకవేళ (rev_సూచీ) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		ఒకవేళ (సూచీ_name)
			rev_సూచీ_name = derive_filename(సూచీ_name,
							 "idx", "rev",
							 &rev_సూచీ_name_buf);
	}

	ఒకవేళ (verify) {
		ఒకవేళ (!సూచీ_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, సూచీ_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	ఒకవేళ (strict)
		opts.flags |= WRITE_IDX_STRICT;

	ఒకవేళ (HAVE_దారాలు && !nr_దారాలు) {
		nr_దారాలు = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		ఒకవేళ (nr_దారాలు < 4)
			; /* too few cores to consider capping */
		లేకపోతే ఒకవేళ (nr_దారాలు < 6)
			nr_దారాలు = 3; /* historic cap */
		లేకపోతే ఒకవేళ (nr_దారాలు < 40)
			nr_దారాలు /= 2;
		లేకపోతే
			nr_దారాలు = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_శీర్షిక();
	CALLOC_ARRAY(objects, st_కూడు(nr_objects, 1));
	ఒకవేళ (show_stat)
		CALLOC_ARRAY(obj_stat, st_కూడు(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	ఒకవేళ (report_చివర_యొక్క_ఆదానం)
		write_లోపల_నిండిన(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	విడుదల(ofs_deltas);
	విడుదల(ref_deltas);
	ఒకవేళ (strict)
		foreign_nr = check_objects();

	ఒకవేళ (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	కొరకు (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_సూచీ = write_idx_file(the_repository, సూచీ_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	ఒకవేళ (rev_సూచీ)
		curr_rev_సూచీ = write_rev_file(the_repository, rev_సూచీ_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	విడుదల(idx_objects);

	ఒకవేళ (!verify)
		final(pack_name, curr_pack,
		      సూచీ_name, curr_సూచీ,
		      rev_సూచీ_name, curr_rev_సూచీ,
		      keep_సందేశం, promisor_సందేశం,
		      pack_hash);
	లేకపోతే
		close(ఆదానం_fd);

	ఒకవేళ (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		ఒకవేళ (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		ఒకవేళ (fsck_ముగింపు(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	విడుదల(opts.anomaly);
	విడుదల(objects);
	strbuf_release(&సూచీ_name_buf);
	strbuf_release(&rev_సూచీ_name_buf);
	ఒకవేళ (!pack_name)
		విడుదల((శూన్య *) curr_pack);
	ఒకవేళ (!సూచీ_name)
		విడుదల((శూన్య *) curr_సూచీ);
	విడుదల(curr_rev_సూచీ);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	ఒకవేళ (check_self_contained_and_connected && foreign_nr)
		తిరిగి 1;

	తిరిగి 0;
}
