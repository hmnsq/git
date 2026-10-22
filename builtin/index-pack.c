#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "delta.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "pack.h"
#ضمن "csum-file.h"
#ضمن "blob.h"
#ضمن "commit.h"
#ضمن "tag.h"
#ضمن "tree.h"
#ضمن "progress.h"
#ضمن "fsck.h"
#ضمن "strbuf.h"
#ضمن "thread-utils.h"
#ضمن "packfile.h"
#ضمن "pack-revindex.h"
#ضمن "object-file.h"
#ضمن "odb.h"
#ضمن "odb/streaming.h"
#ضمن "oid-array.h"
#ضمن "oidset.h"
#ضمن "path.h"
#ضمن "replace-object.h"
#ضمن "tree-walk.h"
#ضمن "promisor-remote.h"
#ضمن "run-command.h"
#ضمن "setup.h"
#ضمن "strvec.h"

ساكن ثابت حرف شاخص_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

بنية object_ثبت {
	بنية pack_idx_ثبت idx;
	اندازه_t اندازه;
	بینښه حرف hdr_اندازه;
	نښهدار حرف type;
	نښهدار حرف real_type;
};

بنية object_stat {
	بینښه delta_ژوروالی;
	صحيح بنسټ_object_no;
};

بنية بنسټ_معلومات {
	/* Initialized by make_base(). */
	بنية بنسټ_معلومات *بنسټ;
	بنية object_ثبت *obj;
	صحيح ref_لومړی, ref_وروستی;
	صحيح ofs_لومړی, ofs_وروستی;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	صحيح retain_معلومات;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	صحيح children_پاتې_شوی;

	/* Not initialized by make_base(). */
	بنية list_سر list;
	فراغ *معلومات;
	اندازه_t اندازه;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
ساكن LIST_سر(work_سر);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
ساكن LIST_سر(بشپړ_سر);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
ساكن اندازه_t بنسټ_cache_کارول_شوی;
ساكن اندازه_t بنسټ_cache_حد;

بنية thread_local_معلومات {
	pthread_t thread;
	صحيح pack_fd;
};

/* Remember to update object flag allocation in object.h */
#تعریف_کړه FLAG_LINK (1u<<20)
#تعریف_کړه FLAG_CHECKED (1u<<21)

بنية ofs_delta_ثبت {
	off_t دننه_واټن;
	صحيح obj_no;
};

بنية ref_delta_ثبت {
	بنية object_id oid;
	صحيح obj_no;
};

ساكن بنية object_ثبت *objects;
ساكن بنية object_stat *obj_stat;
ساكن بنية ofs_delta_ثبت *ofs_deltas;
ساكن بنية ref_delta_ثبت *ref_deltas;
ساكن بنية thread_local_معلومات nothread_معلومات;
ساكن صحيح nr_objects;
ساكن صحيح nr_ofs_deltas;
ساكن صحيح nr_ref_deltas;
ساكن صحيح ref_deltas_alloc;
ساكن صحيح nr_resolved_deltas;
ساكن صحيح nr_تارونه;

ساكن صحيح from_stdin;
ساكن صحيح strict;
ساكن صحيح do_fsck_object;
ساكن بنية fsck_options fsck_options;
ساكن صحيح verbose;
ساكن ثابت حرف *پرمختګ_title;
ساكن صحيح show_resolving_پرمختګ;
ساكن صحيح show_stat;
ساكن صحيح check_self_contained_and_connected;

ساكن بنية پرمختګ *پرمختګ;

ساكن بینښه حرف ننوت_بفر[DEFAULT_IO_بفر_اندازه];
ساكن بینښه صحيح ننوت_دننه_واټن, ننوت_len;
ساكن off_t consumed_بایټونه;
ساكن off_t max_ننوت_اندازه;
ساكن بینښه تر_ټولو_ژور_delta;
ساكن بنية git_hash_ctx ننوت_ctx;
ساكن uint32_t ننوت_دوراني_ازموینه_32;
ساكن صحيح ننوت_fd, output_fd;
ساكن ثابت حرف *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
ساكن بنية oidset outgoing_links = OIDSET_INIT;
ساكن صحيح ریکارډ_outgoing_links;

ساكن بنية thread_local_معلومات *thread_معلومات;
ساكن صحيح nr_dispatched;
ساكن صحيح تارونه_فعال;

ساكن pthread_mutex_t read_mutex;
#تعریف_کړه read_قلف()		قلف_mutex(&read_mutex)
#تعریف_کړه read_unlock()		unlock_mutex(&read_mutex)

ساكن pthread_mutex_t counter_mutex;
#تعریف_کړه counter_قلف()		قلف_mutex(&counter_mutex)
#تعریف_کړه counter_unlock()	unlock_mutex(&counter_mutex)

ساكن pthread_mutex_t work_mutex;
#تعریف_کړه work_قلف()		قلف_mutex(&work_mutex)
#تعریف_کړه work_unlock()		unlock_mutex(&work_mutex)

ساكن pthread_mutex_t تر_ټولو_ژور_delta_mutex;
#تعریف_کړه تر_ټولو_ژور_delta_قلف()	قلف_mutex(&تر_ټولو_ژور_delta_mutex)
#تعریف_کړه تر_ټولو_ژور_delta_unlock()	unlock_mutex(&تر_ټولو_ژور_delta_mutex)

ساكن pthread_کلی_t کلی;

ساكن مضمن فراغ قلف_mutex(pthread_mutex_t *mutex)
{
	إذا (تارونه_فعال)
		pthread_mutex_قلف(mutex);
}

ساكن مضمن فراغ unlock_mutex(pthread_mutex_t *mutex)
{
	إذا (تارونه_فعال)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ساكن فراغ init_thread(فراغ)
{
	صحيح i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&counter_mutex, عدم);
	pthread_mutex_init(&work_mutex, عدم);
	إذا (show_stat)
		pthread_mutex_init(&تر_ټولو_ژور_delta_mutex, عدم);
	pthread_کلی_create(&کلی, عدم);
	CALLOC_ARRAY(thread_معلومات, nr_تارونه);
	لكل (i = 0; i < nr_تارونه; i++) {
		thread_معلومات[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	تارونه_فعال = 1;
}

ساكن فراغ cleanup_thread(فراغ)
{
	صحيح i;
	إذا (!تارونه_فعال)
		أرجع;
	تارونه_فعال = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	إذا (show_stat)
		pthread_mutex_destroy(&تر_ټولو_ژور_delta_mutex);
	لكل (i = 0; i < nr_تارونه; i++)
		close(thread_معلومات[i].pack_fd);
	pthread_کلی_delete(کلی);
	حرر(thread_معلومات);
}

ساكن صحيح mark_link(بنية object *obj, تعداد object_type type,
		     فراغ *معلومات ناکارول_شوی,
		     بنية fsck_options *options ناکارول_شوی)
{
	إذا (!obj)
		أرجع -1;

	إذا (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_منزل_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	أرجع 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
ساكن بینښه check_object(بنية object *obj)
{
	إذا (!obj)
		أرجع 0;

	إذا (!(obj->flags & FLAG_LINK))
		أرجع 0;

	إذا (!(obj->flags & FLAG_CHECKED)) {
		اندازه_t اندازه;
		صحيح type = odb_read_object_info(the_repository->objects,
						&obj->oid, &اندازه);
		إذا (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_منزل_hex(&obj->oid));
		إذا (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_منزل_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		أرجع 1;
	}

	أرجع 0;
}

ساكن بینښه check_objects(فراغ)
{
	بینښه i, max, foreign_nr = 0;

	max = get_max_object_شاخص(the_repository);

	إذا (verbose)
		پرمختګ = پیل_delayed_پرمختګ(the_repository,
						  _("Checking objects"), max);

	لكل (i = 0; i < max; i++) {
		foreign_nr += check_object(get_شاخصي_object(the_repository, i));
		display_پرمختګ(پرمختګ, i + 1);
	}

	stop_پرمختګ(&پرمختګ);
	أرجع foreign_nr;
}


/* Discard current buffer used content. */
ساكن فراغ flush(فراغ)
{
	إذا (ننوت_دننه_واټن) {
		إذا (output_fd >= 0)
			write_or_die(output_fd, ننوت_بفر, ننوت_دننه_واټن);
		git_hash_update(&ننوت_ctx, ننوت_بفر, ننوت_دننه_واټن);
		memmove(ننوت_بفر, ننوت_بفر + ننوت_دننه_واټن, ننوت_len);
		ننوت_دننه_واټن = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ساكن فراغ *fill(صحيح min)
{
	إذا (min <= ننوت_len)
		أرجع ننوت_بفر + ننوت_دننه_واټن;
	إذا (min > حجم(ننوت_بفر))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	افعل {
		ssize_t ret = xread(ننوت_fd, ننوت_بفر + ننوت_len,
				حجم(ننوت_بفر) - ننوت_len);
		إذا (ret <= 0) {
			إذا (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		ننوت_len += ret;
		إذا (from_stdin)
			display_throughput(پرمختګ, consumed_بایټونه + ننوت_len);
	} طالما (ننوت_len < min);
	أرجع ننوت_بفر;
}

ساكن فراغ use(صحيح بایټونه)
{
	إذا (بایټونه > ننوت_len)
		die(_("used more bytes than were available"));
	ننوت_دوراني_ازموینه_32 = دوراني_ازموینه_32(ننوت_دوراني_ازموینه_32, ننوت_بفر + ننوت_دننه_واټن, بایټونه);
	ننوت_len -= بایټونه;
	ننوت_دننه_واټن += بایټونه;

	/* make sure off_t is sufficiently large not to wrap */
	إذا (signed_جمع_کړه_overflows(consumed_بایټونه, بایټونه))
		die(_("pack too large for current definition of off_t"));
	consumed_بایټونه += بایټونه;
	إذا (max_ننوت_اندازه && consumed_بایټونه > max_ننوت_اندازه) {
		بنية strbuf اندازه_حد = STRBUF_INIT;
		strbuf_humanise_بایټونه(&اندازه_حد, max_ننوت_اندازه);
		die(_("pack exceeds maximum allowed size (%s)"),
		    اندازه_حد.buf);
	}
}

ساكن ثابت حرف *open_pack_file(ثابت حرف *pack_name)
{
	إذا (from_stdin) {
		ننوت_fd = 0;
		إذا (!pack_name) {
			بنية strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, عدم);
		} وإلا {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_معلومات.pack_fd = output_fd;
	} وإلا {
		ننوت_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_معلومات.pack_fd = ننوت_fd;
	}
	git_hash_init(&ننوت_ctx, the_hash_algo);
	أرجع pack_name;
}

ساكن فراغ parse_pack_سرلیک(فراغ)
{
	بینښه حرف *hdr = fill(حجم(بنية pack_سرلیک));

	/* Header consistency check */
	إذا (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	إذا (!pack_نسخه_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(حجم(بنية pack_سرلیک));
}

__attribute__((format (چاپ_کړه, 2, 3)))
ساكن NORETURN فراغ bad_object(off_t دننه_واټن, ثابت حرف *format, ...)
{
	va_list params;
	حرف buf[1024];

	va_پیل(params, format);
	vsnprintf(buf, حجم(buf), format, params);
	va_پای_ټکی(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)دننه_واټن, buf);
}

ساكن مضمن بنية thread_local_معلومات *get_thread_معلومات(فراغ)
{
	إذا (HAVE_تارونه) {
		إذا (تارونه_فعال)
			أرجع pthread_getspecific(کلی);
		أكد(!تارونه_فعال &&
		       "This should only be reached when all threads are gone");
	}
	أرجع &nothread_معلومات;
}

ساكن فراغ سېټ_thread_معلومات(بنية thread_local_معلومات *معلومات)
{
	إذا (تارونه_فعال)
		pthread_setspecific(کلی, معلومات);
}

ساكن فراغ free_بنسټ_معلومات(بنية بنسټ_معلومات *c)
{
	إذا (c->معلومات) {
		FREE_AND_NULL(c->معلومات);
		بنسټ_cache_کارول_شوی -= c->اندازه;
	}
}

ساكن فراغ prune_بنسټ_معلومات(بنية بنسټ_معلومات *retain)
{
	بنية list_سر *pos;

	إذا (بنسټ_cache_کارول_شوی <= بنسټ_cache_حد)
		أرجع;

	list_for_each_prev(pos, &بشپړ_سر) {
		بنية بنسټ_معلومات *b = list_ثبت(pos, بنية بنسټ_معلومات, list);
		إذا (b->retain_معلومات || b == retain)
			استمر;
		إذا (b->معلومات) {
			free_بنسټ_معلومات(b);
			إذا (بنسټ_cache_کارول_شوی <= بنسټ_cache_حد)
				أرجع;
		}
	}

	list_for_each_prev(pos, &work_سر) {
		بنية بنسټ_معلومات *b = list_ثبت(pos, بنية بنسټ_معلومات, list);
		إذا (b->retain_معلومات || b == retain)
			استمر;
		إذا (b->معلومات) {
			free_بنسټ_معلومات(b);
			إذا (بنسټ_cache_کارول_شوی <= بنسټ_cache_حد)
				أرجع;
		}
	}
}

ساكن صحيح is_delta_type(تعداد object_type type)
{
	أرجع (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

ساكن فراغ *unpack_ثبت_معلومات(off_t دننه_واټن, اندازه_t اندازه,
			       تعداد object_type type, بنية object_id *oid)
{
	ساكن حرف fixed_buf[8192];
	صحيح حالت;
	git_zstream بهیر;
	فراغ *buf;
	بنية git_hash_ctx c;
	حرف hdr[32];
	صحيح hdrlen;

	إذا (!is_delta_type(type)) {
		hdrlen = format_object_سرلیک(hdr, حجم(hdr), type, اندازه);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} وإلا
		oid = عدم;
	إذا (type == OBJ_BLOB &&
	    اندازه > repo_settings_get_big_file_پوله(the_repository))
		buf = fixed_buf;
	وإلا
		buf = xmallocz(اندازه);

	memset(&بهیر, 0, حجم(بهیر));
	git_inflate_init(&بهیر);
	بهیر.بل_بهر = buf;
	بهیر.avail_بهر = buf == fixed_buf ? حجم(fixed_buf) : اندازه;

	افعل {
		بینښه حرف *وروستی_بهر = بهیر.بل_بهر;
		بهیر.بل_دننه = fill(1);
		بهیر.avail_دننه = ننوت_len;
		حالت = git_inflate(&بهیر, 0);
		use(ننوت_len - بهیر.avail_دننه);
		إذا (oid)
			git_hash_update(&c, وروستی_بهر, بهیر.بل_بهر - وروستی_بهر);
		إذا (buf == fixed_buf) {
			بهیر.بل_بهر = buf;
			بهیر.avail_بهر = حجم(fixed_buf);
		}
	} طالما (حالت == Z_OK);
	إذا (بهیر.ټول_بهر != اندازه || حالت != Z_بهیر_پای_ټکی)
		bad_object(دننه_واټن, _("inflate returned %d"), حالت);
	git_inflate_پای_ټکی(&بهیر);
	إذا (oid)
		git_hash_final_oid(oid, &c);
	أرجع buf == fixed_buf ? عدم : buf;
}

ساكن فراغ *unpack_raw_ثبت(بنية object_ثبت *obj,
			      off_t *ofs_دننه_واټن,
			      بنية object_id *ref_oid,
			      بنية object_id *oid)
{
	بینښه حرف *p;
	اندازه_t اندازه, c;
	off_t بنسټ_دننه_واټن;
	بینښه ښويدنه;
	فراغ *معلومات;

	obj->idx.دننه_واټن = consumed_بایټونه;
	ننوت_دوراني_ازموینه_32 = دوراني_ازموینه_32(0, عدم, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	اندازه = (c & 15);
	ښويدنه = 4;
	طالما (c & 0x80) {
		إذا ((bitsizeof(اندازه_t) - 7) < ښويدنه)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		اندازه += (c & 0x7f) << ښويدنه;
		ښويدنه += 7;
	}
	obj->اندازه = اندازه;

	بدل (obj->type) {
	حالة OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		اكسر;
	حالة OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		بنسټ_دننه_واټن = c & 127;
		طالما (c & 128) {
			بنسټ_دننه_واټن += 1;
			إذا (!بنسټ_دننه_واټن || MSB(بنسټ_دننه_واټن, 7))
				bad_object(obj->idx.دننه_واټن, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			بنسټ_دننه_واټن = (بنسټ_دننه_واټن << 7) + (c & 127);
		}
		*ofs_دننه_واټن = obj->idx.دننه_واټن - بنسټ_دننه_واټن;
		إذا (*ofs_دننه_واټن <= 0 || *ofs_دننه_واټن >= obj->idx.دننه_واټن)
			bad_object(obj->idx.دننه_واټن, _("delta base offset is out of bound"));
		اكسر;
	حالة OBJ_COMMIT:
	حالة OBJ_ونه:
	حالة OBJ_BLOB:
	حالة OBJ_نښان:
		اكسر;
	افتراضي:
		bad_object(obj->idx.دننه_واټن, _("unknown object type %d"), obj->type);
	}
	obj->hdr_اندازه = consumed_بایټونه - obj->idx.دننه_واټن;

	معلومات = unpack_ثبت_معلومات(obj->idx.دننه_واټن, obj->اندازه, obj->type, oid);
	obj->idx.دوراني_ازموینه_32 = ننوت_دوراني_ازموینه_32;
	أرجع معلومات;
}

ساكن فراغ *unpack_معلومات(بنية object_ثبت *obj,
			 صحيح (*consume)(ثابت بینښه حرف *, بینښه اوږد, فراغ *),
			 فراغ *cb_معلومات)
{
	off_t from = obj[0].idx.دننه_واټن + obj[0].hdr_اندازه;
	off_t len = obj[1].idx.دننه_واټن - from;
	بینښه حرف *معلومات, *inbuf;
	git_zstream بهیر;
	صحيح حالت;

	معلومات = xmallocz(consume ? 64*1024 : obj->اندازه);
	inbuf = xmalloc((len < 64*1024) ? (صحيح)len : 64*1024);

	memset(&بهیر, 0, حجم(بهیر));
	git_inflate_init(&بهیر);
	بهیر.بل_بهر = معلومات;
	بهیر.avail_بهر = consume ? 64*1024 : obj->اندازه;

	افعل {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_معلومات()->pack_fd, inbuf, n, from);
		إذا (n < 0)
			die_errno(_("cannot pread pack file"));
		إذا (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		بهیر.بل_دننه = inbuf;
		بهیر.avail_دننه = n;
		إذا (!consume)
			حالت = git_inflate(&بهیر, 0);
		وإلا {
			افعل {
				حالت = git_inflate(&بهیر, 0);
				إذا (consume(معلومات, بهیر.بل_بهر - معلومات, cb_معلومات)) {
					حرر(inbuf);
					حرر(معلومات);
					أرجع عدم;
				}
				بهیر.بل_بهر = معلومات;
				بهیر.avail_بهر = 64*1024;
			} طالما (حالت == Z_OK && بهیر.avail_دننه);
		}
	} طالما (len && حالت == Z_OK && !بهیر.avail_دننه);

	/* This has been inflated OK when first encountered, so... */
	إذا (حالت != Z_بهیر_پای_ټکی || بهیر.ټول_بهر != obj->اندازه)
		die(_("serious inflate inconsistency"));

	git_inflate_پای_ټکی(&بهیر);
	حرر(inbuf);
	إذا (consume) {
		FREE_AND_NULL(معلومات);
	}
	أرجع معلومات;
}

ساكن فراغ *get_معلومات_from_pack(بنية object_ثبت *obj)
{
	أرجع unpack_معلومات(obj, عدم, عدم);
}

ساكن صحيح compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   تعداد object_type type1,
				   تعداد object_type type2)
{
	صحيح cmp = type1 - type2;
	إذا (cmp)
		أرجع cmp;
	أرجع offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

ساكن صحيح پیدا_کړه_ofs_delta(ثابت off_t دننه_واټن)
{
	صحيح لومړی = 0, وروستی = nr_ofs_deltas;

	طالما (لومړی < وروستی) {
		صحيح بل = لومړی + (وروستی - لومړی) / 2;
		بنية ofs_delta_ثبت *delta = &ofs_deltas[بل];
		صحيح cmp;

		cmp = compare_ofs_delta_bases(دننه_واټن, delta->دننه_واټن,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		إذا (!cmp)
			أرجع بل;
		إذا (cmp < 0) {
			وروستی = بل;
			استمر;
		}
		لومړی = بل+1;
	}
	أرجع -لومړی-1;
}

ساكن فراغ پیدا_کړه_ofs_delta_children(off_t دننه_واټن,
				    صحيح *لومړی_شاخص, صحيح *وروستی_شاخص)
{
	صحيح لومړی = پیدا_کړه_ofs_delta(دننه_واټن);
	صحيح وروستی = لومړی;
	صحيح پای_ټکی = nr_ofs_deltas - 1;

	إذا (لومړی < 0) {
		*لومړی_شاخص = 0;
		*وروستی_شاخص = -1;
		أرجع;
	}
	طالما (لومړی > 0 && ofs_deltas[لومړی - 1].دننه_واټن == دننه_واټن)
		--لومړی;
	طالما (وروستی < پای_ټکی && ofs_deltas[وروستی + 1].دننه_واټن == دننه_واټن)
		++وروستی;
	*لومړی_شاخص = لومړی;
	*وروستی_شاخص = وروستی;
}

ساكن صحيح compare_ref_delta_bases(ثابت بنية object_id *oid1,
				   ثابت بنية object_id *oid2,
				   تعداد object_type type1,
				   تعداد object_type type2)
{
	صحيح cmp = type1 - type2;
	إذا (cmp)
		أرجع cmp;
	أرجع oidcmp(oid1, oid2);
}

ساكن صحيح پیدا_کړه_ref_delta(ثابت بنية object_id *oid)
{
	صحيح لومړی = 0, وروستی = nr_ref_deltas;

	طالما (لومړی < وروستی) {
		صحيح بل = لومړی + (وروستی - لومړی) / 2;
		بنية ref_delta_ثبت *delta = &ref_deltas[بل];
		صحيح cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		إذا (!cmp)
			أرجع بل;
		إذا (cmp < 0) {
			وروستی = بل;
			استمر;
		}
		لومړی = بل+1;
	}
	أرجع -لومړی-1;
}

ساكن فراغ پیدا_کړه_ref_delta_children(ثابت بنية object_id *oid,
				    صحيح *لومړی_شاخص, صحيح *وروستی_شاخص)
{
	صحيح لومړی = پیدا_کړه_ref_delta(oid);
	صحيح وروستی = لومړی;
	صحيح پای_ټکی = nr_ref_deltas - 1;

	إذا (لومړی < 0) {
		*لومړی_شاخص = 0;
		*وروستی_شاخص = -1;
		أرجع;
	}
	طالما (لومړی > 0 && oideq(&ref_deltas[لومړی - 1].oid, oid))
		--لومړی;
	طالما (وروستی < پای_ټکی && oideq(&ref_deltas[وروستی + 1].oid, oid))
		++وروستی;
	*لومړی_شاخص = لومړی;
	*وروستی_شاخص = وروستی;
}

بنية compare_معلومات {
	بنية object_ثبت *ثبت;
	بنية odb_بهیر *st;
	بینښه حرف *buf;
	بینښه اوږد buf_اندازه;
};

ساكن صحيح compare_objects(ثابت بینښه حرف *buf, بینښه اوږد اندازه,
			   فراغ *cb_معلومات)
{
	بنية compare_معلومات *معلومات = cb_معلومات;

	إذا (معلومات->buf_اندازه < اندازه) {
		حرر(معلومات->buf);
		معلومات->buf = xmalloc(اندازه);
		معلومات->buf_اندازه = اندازه;
	}

	طالما (اندازه) {
		ssize_t len = odb_بهیر_read(معلومات->st, معلومات->buf, اندازه);
		إذا (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_منزل_hex(&معلومات->ثبت->idx.oid));
		إذا (len < 0)
			die(_("unable to read %s"),
			    oid_منزل_hex(&معلومات->ثبت->idx.oid));
		إذا (memcmp(buf, معلومات->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_منزل_hex(&معلومات->ثبت->idx.oid));
		اندازه -= len;
		buf += len;
	}
	أرجع 0;
}

ساكن صحيح check_collison(بنية object_ثبت *ثبت)
{
	بنية compare_معلومات معلومات;

	إذا (ثبت->اندازه <= repo_settings_get_big_file_پوله(the_repository) ||
	    ثبت->type != OBJ_BLOB)
		أرجع -1;

	memset(&معلومات, 0, حجم(معلومات));
	معلومات.ثبت = ثبت;
	معلومات.st = odb_بهیر_from_object(the_repository->objects, &ثبت->idx.oid, عدم);
	إذا (!معلومات.st)
		أرجع -1;
	إذا (معلومات.st->اندازه != ثبت->اندازه || معلومات.st->type != ثبت->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_منزل_hex(&ثبت->idx.oid));
	unpack_معلومات(ثبت, compare_objects, &معلومات);
	odb_بهیر_close(معلومات.st);
	حرر(معلومات.buf);
	أرجع 0;
}

ساكن فراغ ریکارډ_outgoing_link(ثابت بنية object_id *oid)
{
	oidset_ورننباسه(&outgoing_links, oid);
}

ساكن فراغ maybe_ریکارډ_name_ثبت(ثابت بنية name_ثبت *ثبت)
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
	إذا (S_ISDIR(ثبت->ډېرپېښ))
		ریکارډ_outgoing_link(&ثبت->oid);
}

ساكن فراغ do_ریکارډ_outgoing_links(بنية object *obj)
{
	إذا (obj->type == OBJ_ونه) {
		بنية ونه *ونه = (بنية ونه *)obj;
		بنية ونه_desc desc;
		بنية name_ثبت ثبت;
		إذا (init_ونه_desc_gently(&desc, &ونه->object.oid,
					  ونه->بفر, ونه->اندازه, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			أرجع;
		طالما (ونه_ثبت_gently(&desc, &ثبت))
			maybe_ریکارډ_name_ثبت(&ثبت);
	} وإلا إذا (obj->type == OBJ_COMMIT) {
		بنية commit *commit = (بنية commit *) obj;
		بنية commit_list *parents = commit->parents;

		ریکارډ_outgoing_link(get_commit_ونه_oid(commit));
		لكل (; parents; parents = parents->بل)
			ریکارډ_outgoing_link(&parents->توکی->object.oid);
	} وإلا إذا (obj->type == OBJ_نښان) {
		بنية نښان *نښان = (بنية نښان *) obj;
		ریکارډ_outgoing_link(get_tagged_oid(نښان));
	}
}

ساكن فراغ sha1_object(ثابت فراغ *معلومات, بنية object_ثبت *obj_ثبت,
			بینښه اوږد اندازه, تعداد object_type type,
			ثابت بنية object_id *oid)
{
	فراغ *new_معلومات = عدم;
	صحيح collision_test_needed = 0;

	أكد(معلومات || obj_ثبت);

	إذا (startup_info->have_repository) {
		read_قلف();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	إذا (collision_test_needed && !معلومات) {
		read_قلف();
		إذا (!check_collison(obj_ثبت))
			collision_test_needed = 0;
		read_unlock();
	}
	إذا (collision_test_needed) {
		فراغ *has_معلومات;
		تعداد object_type has_type;
		اندازه_t has_اندازه;
		read_قلف();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_اندازه);
		إذا (has_type < 0)
			die(_("cannot read existing object info %s"), oid_منزل_hex(oid));
		إذا (has_type != type || has_اندازه != اندازه)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_منزل_hex(oid));
		has_معلومات = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_اندازه);
		read_unlock();
		إذا (!معلومات)
			معلومات = new_معلومات = get_معلومات_from_pack(obj_ثبت);
		إذا (!has_معلومات)
			die(_("cannot read existing object %s"), oid_منزل_hex(oid));
		إذا (اندازه != has_اندازه || type != has_type ||
		    memcmp(معلومات, has_معلومات, اندازه) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_منزل_hex(oid));
		حرر(has_معلومات);
	}

	إذا (strict || do_fsck_object || ریکارډ_outgoing_links) {
		read_قلف();
		إذا (type == OBJ_BLOB) {
			بنية blob *blob = lookup_blob(the_repository, oid);
			إذا (blob)
				blob->object.flags |= FLAG_CHECKED;
			وإلا
				die(_("invalid blob object %s"), oid_منزل_hex(oid));
			إذا (do_fsck_object &&
			    fsck_object(&blob->object, (فراغ *)معلومات, اندازه, &fsck_options))
				die(_("fsck error in packed object"));
		} وإلا {
			بنية object *obj;
			صحيح eaten;
			فراغ *buf = (فراغ *) معلومات;

			أكد(معلومات && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_بفر(the_repository, oid, type,
						  اندازه, buf,
						  &eaten);
			إذا (!obj)
				die(_("invalid %s"), type_name(type));
			إذا (do_fsck_object &&
			    fsck_object(obj, buf, اندازه, &fsck_options))
				die(_("fsck error in packed object"));
			إذا (strict && fsck_walk(obj, عدم, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_منزل_hex(&obj->oid));
			إذا (ریکارډ_outgoing_links)
				do_ریکارډ_outgoing_links(obj);

			إذا (obj->type == OBJ_ونه) {
				بنية ونه *توکی = (بنية ونه *) obj;
				توکی->بفر = عدم;
				obj->parsed = 0;
			}
			إذا (obj->type == OBJ_COMMIT) {
				بنية commit *commit = (بنية commit *) obj;
				إذا (detach_commit_بفر(commit, عدم) != معلومات)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	حرر(new_معلومات);
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
ساكن فراغ *get_بنسټ_معلومات(بنية بنسټ_معلومات *c)
{
	إذا (!c->معلومات) {
		بنية object_ثبت *obj = c->obj;
		بنية بنسټ_معلومات **delta = عدم;
		صحيح delta_nr = 0, delta_alloc = 0;

		طالما (is_delta_type(c->obj->type) && !c->معلومات) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->بنسټ;
		}
		إذا (!delta_nr) {
			c->معلومات = get_معلومات_from_pack(obj);
			c->اندازه = obj->اندازه;
			بنسټ_cache_کارول_شوی += c->اندازه;
			prune_بنسټ_معلومات(c);
		}
		لكل (; delta_nr > 0; delta_nr--) {
			فراغ *بنسټ, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			بنسټ = get_بنسټ_معلومات(c->بنسټ);
			raw = get_معلومات_from_pack(obj);
			c->معلومات = patch_delta(
				بنسټ, c->بنسټ->اندازه,
				raw, obj->اندازه,
				&c->اندازه);
			حرر(raw);
			إذا (!c->معلومات)
				bad_object(obj->idx.دننه_واټن, _("failed to apply delta"));
			بنسټ_cache_کارول_شوی += c->اندازه;
			prune_بنسټ_معلومات(c);
		}
		حرر(delta);
	}
	أرجع c->معلومات;
}

ساكن بنية بنسټ_معلومات *make_بنسټ(بنية object_ثبت *obj,
				   بنية بنسټ_معلومات *پلار)
{
	بنية بنسټ_معلومات *بنسټ = xcalloc(1, حجم(بنية بنسټ_معلومات));
	بنسټ->بنسټ = پلار;
	بنسټ->obj = obj;
	پیدا_کړه_ref_delta_children(&obj->idx.oid,
				&بنسټ->ref_لومړی, &بنسټ->ref_وروستی);
	پیدا_کړه_ofs_delta_children(obj->idx.دننه_واټن,
				&بنسټ->ofs_لومړی, &بنسټ->ofs_وروستی);
	بنسټ->children_پاتې_شوی = بنسټ->ref_وروستی - بنسټ->ref_لومړی +
		بنسټ->ofs_وروستی - بنسټ->ofs_لومړی + 2;
	أرجع بنسټ;
}

ساكن بنية بنسټ_معلومات *resolve_delta(بنية object_ثبت *delta_obj,
				       بنية بنسټ_معلومات *بنسټ)
{
	فراغ *delta_معلومات, *پایله_معلومات;
	بنية بنسټ_معلومات *پایله;
	اندازه_t پایله_اندازه;

	إذا (show_stat) {
		صحيح i = delta_obj - objects;
		صحيح j = بنسټ->obj - objects;
		obj_stat[i].delta_ژوروالی = obj_stat[j].delta_ژوروالی + 1;
		تر_ټولو_ژور_delta_قلف();
		إذا (تر_ټولو_ژور_delta < obj_stat[i].delta_ژوروالی)
			تر_ټولو_ژور_delta = obj_stat[i].delta_ژوروالی;
		تر_ټولو_ژور_delta_unlock();
		obj_stat[i].بنسټ_object_no = j;
	}
	delta_معلومات = get_معلومات_from_pack(delta_obj);
	أكد(بنسټ->معلومات);
	پایله_معلومات = patch_delta(بنسټ->معلومات, بنسټ->اندازه,
				  delta_معلومات, delta_obj->اندازه, &پایله_اندازه);
	حرر(delta_معلومات);
	إذا (!پایله_معلومات)
		bad_object(delta_obj->idx.دننه_واټن, _("failed to apply delta"));
	hash_object_file(the_hash_algo, پایله_معلومات, پایله_اندازه,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(پایله_معلومات, عدم, پایله_اندازه, delta_obj->real_type,
		    &delta_obj->idx.oid);

	پایله = make_بنسټ(delta_obj, بنسټ);
	پایله->معلومات = پایله_معلومات;
	پایله->اندازه = پایله_اندازه;

	counter_قلف();
	nr_resolved_deltas++;
	counter_unlock();

	أرجع پایله;
}

ساكن صحيح compare_ofs_delta_ثبت(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية ofs_delta_ثبت *delta_a = a;
	ثابت بنية ofs_delta_ثبت *delta_b = b;

	أرجع delta_a->دننه_واټن < delta_b->دننه_واټن ? -1 :
	       delta_a->دننه_واټن > delta_b->دننه_واټن ?  1 :
	       0;
}

ساكن صحيح compare_ref_delta_ثبت(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية ref_delta_ثبت *delta_a = a;
	ثابت بنية ref_delta_ثبت *delta_b = b;

	أرجع oidcmp(&delta_a->oid, &delta_b->oid);
}

ساكن فراغ *threaded_دوهم_pass(فراغ *معلومات)
{
	إذا (معلومات)
		سېټ_thread_معلومات(معلومات);
	لكل (;;) {
		بنية بنسټ_معلومات *پلار = عدم;
		بنية object_ثبت *ماشوم_obj = عدم;
		بنية بنسټ_معلومات *ماشوم = عدم;

		counter_قلف();
		display_پرمختګ(پرمختګ, nr_resolved_deltas);
		counter_unlock();

		work_قلف();
		إذا (list_تشه(&work_سر)) {
			/*
			 * Take an object from the object array.
			 */
			طالما (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			إذا (nr_dispatched >= nr_objects) {
				work_unlock();
				اكسر;
			}
			ماشوم_obj = &objects[nr_dispatched++];
		} وإلا {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			پلار = list_لومړی_ثبت(&work_سر, بنية بنسټ_معلومات,
						  list);

			طالما (پلار->ref_لومړی <= پلار->ref_وروستی) {
				صحيح دننه_واټن = ref_deltas[پلار->ref_لومړی++].obj_no;
				ماشوم_obj = objects + دننه_واټن;
				إذا (ماشوم_obj->real_type != OBJ_REF_DELTA) {
					ماشوم_obj = عدم;
					استمر;
				}
				ماشوم_obj->real_type = پلار->obj->real_type;
				اكسر;
			}

			إذا (!ماشوم_obj && پلار->ofs_لومړی <= پلار->ofs_وروستی) {
				ماشوم_obj = objects +
					ofs_deltas[پلار->ofs_لومړی++].obj_no;
				أكد(ماشوم_obj->real_type == OBJ_OFS_DELTA);
				ماشوم_obj->real_type = پلار->obj->real_type;
			}

			إذا (پلار->ref_لومړی > پلار->ref_وروستی &&
			    پلار->ofs_لومړی > پلار->ofs_وروستی) {
				/*
				 * This parent has run out of children, so move
				 * it to done_head.
				 */
				list_del(&پلار->list);
				list_جمع_کړه(&پلار->list, &بشپړ_سر);
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
			get_بنسټ_معلومات(پلار);
			پلار->retain_معلومات++;
		}
		work_unlock();

		إذا (ماشوم_obj) {
			إذا (پلار) {
				ماشوم = resolve_delta(ماشوم_obj, پلار);
				إذا (!ماشوم->children_پاتې_شوی)
					FREE_AND_NULL(ماشوم->معلومات);
			} وإلا{
				ماشوم = make_بنسټ(ماشوم_obj, عدم);
				إذا (ماشوم->children_پاتې_شوی) {
					/*
					 * Since this child has its own delta children,
					 * we will need this data in the future.
					 * Inflate now so that future iterations will
					 * have access to this object's data while
					 * outside the work mutex.
					 */
					ماشوم->معلومات = get_معلومات_from_pack(ماشوم_obj);
					ماشوم->اندازه = ماشوم_obj->اندازه;
				}
			}
		}

		work_قلف();
		إذا (پلار)
			پلار->retain_معلومات--;

		إذا (ماشوم && ماشوم->معلومات) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_جمع_کړه(&ماشوم->list, &work_سر);
			بنسټ_cache_کارول_شوی += ماشوم->اندازه;
			prune_بنسټ_معلومات(عدم);
		} وإلا إذا (ماشوم) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			بنية بنسټ_معلومات *p = پلار;

			طالما (p) {
				بنية بنسټ_معلومات *بل_p;

				p->children_پاتې_شوی--;
				إذا (p->children_پاتې_شوی)
					اكسر;

				بل_p = p->بنسټ;
				free_بنسټ_معلومات(p);
				list_del(&p->list);
				حرر(p);

				p = بل_p;
			}
			FREE_AND_NULL(ماشوم);
		}
		work_unlock();
	}
	أرجع عدم;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
ساكن فراغ parse_pack_objects(بینښه حرف *hash)
{
	صحيح i, nr_delays = 0;
	بنية ofs_delta_ثبت *ofs_delta = ofs_deltas;
	بنية object_id ref_delta_oid;
	بنية stat st;
	بنية git_hash_ctx tmp_ctx;

	إذا (verbose)
		پرمختګ = پیل_پرمختګ(
				the_repository,
				پرمختګ_title ? پرمختګ_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	لكل (i = 0; i < nr_objects; i++) {
		بنية object_ثبت *obj = &objects[i];
		فراغ *معلومات = unpack_raw_ثبت(obj, &ofs_delta->دننه_واټن,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		إذا (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} وإلا إذا (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} وإلا إذا (!معلومات) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} وإلا
			sha1_object(معلومات, عدم, obj->اندازه, obj->type,
				    &obj->idx.oid);
		حرر(معلومات);
		display_پرمختګ(پرمختګ, i+1);
	}
	objects[i].idx.دننه_واټن = consumed_بایټونه;
	stop_پرمختګ(&پرمختګ);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &ننوت_ctx);
	git_hash_final(hash, &tmp_ctx);
	إذا (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	إذا (fstat(ننوت_fd, &st))
		die_errno(_("cannot fstat packfile"));
	إذا (S_ISREG(st.st_ډېرپېښ) &&
			lseek(ننوت_fd, 0, SEEK_CUR) - ننوت_len != st.st_اندازه)
		die(_("pack has junk at the end"));

	لكل (i = 0; i < nr_objects; i++) {
		بنية object_ثبت *obj = &objects[i];
		إذا (obj->real_type != OBJ_BAD)
			استمر;
		obj->real_type = obj->type;
		sha1_object(عدم, obj, obj->اندازه, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	إذا (nr_delays)
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
ساكن فراغ resolve_deltas(بنية pack_idx_option *opts)
{
	صحيح i;

	إذا (!nr_ofs_deltas && !nr_ref_deltas)
		أرجع;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_ثبت);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_ثبت);

	إذا (verbose || show_resolving_پرمختګ)
		پرمختګ = پیل_پرمختګ(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	بنسټ_cache_حد = opts->delta_بنسټ_cache_حد * nr_تارونه;
	إذا (nr_تارونه > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		لكل (i = 0; i < nr_تارونه; i++) {
			صحيح ret = pthread_create(&thread_معلومات[i].thread, عدم,
						 threaded_دوهم_pass, thread_معلومات + i);
			إذا (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		لكل (i = 0; i < nr_تارونه; i++)
			pthread_join(thread_معلومات[i].thread, عدم);
		cleanup_thread();
		أرجع;
	}
	threaded_دوهم_pass(&nothread_معلومات);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
ساكن فراغ fix_unresolved_deltas(بنية hashfile *f);
ساكن فراغ conclude_pack(صحيح fix_thin_pack, ثابت حرف *curr_pack, بینښه حرف *pack_hash)
{
	إذا (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_پرمختګ(&پرمختګ);
		/* Flush remaining pack final hash. */
		flush();
		أرجع;
	}

	إذا (fix_thin_pack) {
		بنية hashfile *f;
		بینښه حرف read_hash[GIT_MAX_RAWSZ], لکۍ_hash[GIT_MAX_RAWSZ];
		بنية strbuf پیغام = STRBUF_INIT;
		صحيح nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		صحيح nr_objects_initial = nr_objects;
		إذا (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * حجم(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&پیغام, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_پرمختګ_پیغام(&پرمختګ, پیغام.buf);
		strbuf_release(&پیغام);
		finalize_hashfile(f, لکۍ_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_سرلیک_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_بایټونه-the_hash_algo->rawsz);
		إذا (!hasheq(read_hash, لکۍ_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	إذا (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

ساكن صحيح write_compressed(بنية hashfile *f, فراغ *دننه, بینښه صحيح اندازه)
{
	git_zstream بهیر;
	صحيح حالت;
	بینښه حرف outbuf[4096];
	بنية repo_config_ارزښتونه *cfg = repo_config_ارزښتونه(the_repository);

	git_deflate_init(&بهیر, cfg->zlib_compression_level);
	بهیر.بل_دننه = دننه;
	بهیر.avail_دننه = اندازه;

	افعل {
		بهیر.بل_بهر = outbuf;
		بهیر.avail_بهر = حجم(outbuf);
		حالت = git_deflate(&بهیر, Z_پای);
		hashwrite(f, outbuf, حجم(outbuf) - بهیر.avail_بهر);
	} طالما (حالت == Z_OK);

	إذا (حالت != Z_بهیر_پای_ټکی)
		die(_("unable to deflate appended object (%d)"), حالت);
	اندازه = بهیر.ټول_بهر;
	git_deflate_پای_ټکی(&بهیر);
	أرجع اندازه;
}

ساكن بنية object_ثبت *append_obj_منزل_pack(بنية hashfile *f,
			       ثابت بینښه حرف *sha1, فراغ *buf,
			       بینښه اوږد اندازه, تعداد object_type type)
{
	بنية object_ثبت *obj = &objects[nr_objects++];
	بینښه حرف سرلیک[10];
	بینښه اوږد s = اندازه;
	صحيح n = 0;
	بینښه حرف c = (type << 4) | (s & 15);
	s >>= 4;
	طالما (s) {
		سرلیک[n++] = c | 0x80;
		c = s & 0x7f;
		s >>= 7;
	}
	سرلیک[n++] = c;
	دوراني_ازموینه_32_begin(f);
	hashwrite(f, سرلیک, n);
	obj[0].اندازه = اندازه;
	obj[0].hdr_اندازه = n;
	obj[0].type = type;
	obj[0].real_type = type;
	obj[1].idx.دننه_واټن = obj[0].idx.دننه_واټن + n;
	obj[1].idx.دننه_واټن += write_compressed(f, buf, اندازه);
	obj[0].idx.دوراني_ازموینه_32 = دوراني_ازموینه_32_پای_ټکی(f);
	hashflush(f);
	oidread(&obj->idx.oid, sha1, the_repository->hash_algo);
	أرجع obj;
}

ساكن صحيح delta_pos_compare(ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية ref_delta_ثبت *a = *(بنية ref_delta_ثبت **)_a;
	بنية ref_delta_ثبت *b = *(بنية ref_delta_ثبت **)_b;
	أرجع a->obj_no - b->obj_no;
}

ساكن فراغ fix_unresolved_deltas(بنية hashfile *f)
{
	بنية ref_delta_ثبت **sorted_by_pos;
	صحيح i;

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
	لكل (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	إذا (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		بنية oid_array منزل_fetch = OID_ARRAY_INIT;
		لكل (i = 0; i < nr_ref_deltas; i++) {
			بنية ref_delta_ثبت *d = sorted_by_pos[i];
			إذا (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, عدم,
							   OBJECT_INFO_FOR_PREFETCH))
				استمر;
			oid_array_append(&منزل_fetch, &d->oid);
		}
		promisor_remote_get_مستقیم(the_repository,
					   منزل_fetch.oid, منزل_fetch.nr);
		oid_array_clear(&منزل_fetch);
	}

	لكل (i = 0; i < nr_ref_deltas; i++) {
		بنية ref_delta_ثبت *d = sorted_by_pos[i];
		تعداد object_type type;
		فراغ *معلومات;
		اندازه_t اندازه;

		إذا (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			استمر;
		معلومات = odb_read_object(the_repository->objects, &d->oid,
				       &type, &اندازه);
		إذا (!معلومات)
			استمر;

		إذا (check_object_signature(the_repository, &d->oid, معلومات, اندازه,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_منزل_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_منزل_pack(f, d->oid.hash, معلومات, اندازه, type);
		حرر(معلومات);
		threaded_دوهم_pass(عدم);

		display_پرمختګ(پرمختګ, nr_resolved_deltas);
	}
	حرر(sorted_by_pos);
}

ساكن ثابت حرف *derive_filename(ثابت حرف *pack_name, ثابت حرف *strip,
				   ثابت حرف *suffix, بنية strbuf *buf)
{
	اندازه_t len;
	إذا (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_جمع_کړه(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	أرجع buf->buf;
}

ساكن فراغ write_special_file(ثابت حرف *suffix, ثابت حرف *پیغام,
			       ثابت حرف *pack_name, ثابت بینښه حرف *hash,
			       ثابت حرف **report)
{
	بنية strbuf name_buf = STRBUF_INIT;
	ثابت حرف *filename;
	صحيح fd;
	صحيح پیغام_len = مزي_اوږدوالی(پیغام);

	إذا (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	وإلا
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = خوندي_create_file_with_leading_directories(the_repository, filename);
	إذا (fd < 0) {
		إذا (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} وإلا {
		إذا (پیغام_len > 0) {
			write_or_die(fd, پیغام, پیغام_len);
			write_or_die(fd, "\n", 1);
		}
		إذا (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		إذا (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

ساكن فراغ rename_tmp_packfile(ثابت حرف **final_name,
				ثابت حرف *curr_name,
				بنية strbuf *name, بینښه حرف *hash,
				ثابت حرف *ext, صحيح make_read_only_if_same)
{
	إذا (!*final_name || strcmp(*final_name, curr_name)) {
		إذا (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		إذا (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} وإلا إذا (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

ساكن فراغ final(ثابت حرف *final_pack_name, ثابت حرف *curr_pack_name,
		  ثابت حرف *final_شاخص_name, ثابت حرف *curr_شاخص_name,
		  ثابت حرف *final_rev_شاخص_name, ثابت حرف *curr_rev_شاخص_name,
		  ثابت حرف *keep_پیغام, ثابت حرف *promisor_پیغام,
		  بینښه حرف *hash)
{
	ثابت حرف *report = "pack";
	بنية strbuf pack_name = STRBUF_INIT;
	بنية strbuf شاخص_name = STRBUF_INIT;
	بنية strbuf rev_شاخص_name = STRBUF_INIT;

	إذا (!from_stdin) {
		close(ننوت_fd);
	} وإلا {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		إذا (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	إذا (keep_پیغام)
		write_special_file("keep", keep_پیغام, final_pack_name, hash,
				   &report);
	إذا (promisor_پیغام)
		write_special_file("promisor", promisor_پیغام, final_pack_name,
				   hash, عدم);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	إذا (curr_rev_شاخص_name)
		rename_tmp_packfile(&final_rev_شاخص_name, curr_rev_شاخص_name,
				    &rev_شاخص_name, hash, "rev", 1);
	rename_tmp_packfile(&final_شاخص_name, curr_شاخص_name, &شاخص_name,
			    hash, "idx", 1);

	إذا (do_fsck_object && startup_info->have_repository) {
		بنية odb_سرچینه_files *files =
			odb_سرچینه_files_downcast(the_repository->objects->سرچینه);
		packfile_store_بارول_pack(files->dirs->packed, final_شاخص_name, 0);
	}

	إذا (!from_stdin) {
		چاپ_کړه("%s\n", hash_منزل_hex(hash));
	} وإلا {
		بنية strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_منزل_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_دننه_ډک(1, ننوت_بفر + ننوت_دننه_واټن, ننوت_len);
	}

	strbuf_release(&rev_شاخص_name);
	strbuf_release(&شاخص_name);
	strbuf_release(&pack_name);
}

ساكن صحيح git_شاخص_pack_config(ثابت حرف *k, ثابت حرف *v,
				 ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية pack_idx_option *opts = cb;

	إذا (!strcmp(k, "pack.indexversion")) {
		opts->نسخه = git_config_int(k, v, ctx->kvi);
		إذا (opts->نسخه > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->نسخه);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.threads")) {
		nr_تارونه = git_config_int(k, v, ctx->kvi);
		إذا (nr_تارونه < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_تارونه);
		إذا (!HAVE_تارونه && nr_تارونه != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_تارونه = 1;
		}
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.writereverseindex")) {
		إذا (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		وإلا
			opts->flags &= ~WRITE_REV;
	}
	إذا (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_بنسټ_cache_حد = git_config_ulong(k, v, ctx->kvi);
		أرجع 0;
	}
	أرجع git_default_config(k, v, ctx, cb);
}

ساكن صحيح cmp_uint32(ثابت فراغ *a_, ثابت فراغ *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	أرجع (a < b) ? -1 : (a != b);
}

ساكن فراغ read_v2_anomalous_offsets(بنية packed_git *p,
				      بنية pack_idx_option *opts)
{
	ثابت uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((ثابت uint32_t *)((ثابت uint8_t *)p->شاخص_معلومات + p->دوراني_ازموینه_دننه_واټن))
		+ (اندازه_t)p->عدد_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->عدد_objects;

	لكل (i = 0; i < p->عدد_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		إذا (!(off & 0x80000000))
			استمر;
		off = off & 0x7fffffff;
		check_pack_شاخص_ptr(p, &idx2[off * 2]);
		إذا (idx2[off * 2])
			استمر;
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

ساكن فراغ read_idx_option(بنية pack_idx_option *opts, ثابت حرف *pack_name)
{
	بنية packed_git *p = جمع_کړه_packed_git(the_repository, pack_name,
					      مزي_اوږدوالی(pack_name), 1);

	إذا (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	إذا (open_pack_شاخص(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->نسخه = p->شاخص_نسخه;

	إذا (opts->نسخه == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_شاخص(p);
	حرر(p);
}

ساكن فراغ show_pack_info(صحيح stat_only)
{
	صحيح i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	بینښه اوږد *chain_histogram = عدم;

	إذا (تر_ټولو_ژور_delta)
		CALLOC_ARRAY(chain_histogram, تر_ټولو_ژور_delta);

	لكل (i = 0; i < nr_objects; i++) {
		بنية object_ثبت *obj = &objects[i];

		إذا (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_ژوروالی - 1]++;
		إذا (stat_only)
			استمر;
		چاپ_کړه("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_منزل_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->اندازه,
		       (uintmax_t)(obj[1].idx.دننه_واټن - obj->idx.دننه_واټن),
		       (uintmax_t)obj->idx.دننه_واټن);
		إذا (is_delta_type(obj->type)) {
			بنية object_ثبت *bobj = &objects[obj_stat[i].بنسټ_object_no];
			چاپ_کړه(" %u %s", obj_stat[i].delta_ژوروالی,
			       oid_منزل_hex(&bobj->idx.oid));
		}
		توری_چاپ_کړه('\n');
	}

	إذا (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	لكل (i = 0; i < تر_ټولو_ژور_delta; i++) {
		إذا (!chain_histogram[i])
			استمر;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	حرر(chain_histogram);
}

ساكن فراغ repack_local_links(فراغ)
{
	بنية ماشوم_process cmd = ماشوم_PROCESS_INIT;
	FILE *بهر;
	بنية strbuf کرښه = STRBUF_INIT;
	بنية oidset_iter iter;
	بنية object_id *oid;
	حرف *بنسټ_name = عدم;

	إذا (!oidset_اندازه(&outgoing_links))
		أرجع;

	oidset_iter_init(&outgoing_links, &iter);
	طالما ((oid = oidset_iter_بل(&iter))) {
		بنية odb_سرچینه_info سرچینه_info;
		بنية object_info info = {
			.سرچینه_infop = &سرچینه_info,
		};

		إذا (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			استمر;
		إذا (سرچینه_info.سرچینه->type == ODB_سرچینه_PACKED &&
		    سرچینه_info.u.packed.pack->pack_promisor)
			استمر;

		إذا (!cmd.دلیلونه.nr) {
			بنسټ_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_ټېل(&cmd.دلیلونه, "pack-objects");
			strvec_ټېل(&cmd.دلیلونه,
				    "--exclude-promisor-objects-best-effort");
			strvec_ټېل(&cmd.دلیلونه, بنسټ_name);
			cmd.git_cmd = 1;
			cmd.دننه = -1;
			cmd.بهر = -1;
			إذا (پیل_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		إذا (write_دننه_ډک(cmd.دننه, oid_منزل_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_دننه_ډک(cmd.دننه, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	إذا (!cmd.دلیلونه.nr)
		أرجع;

	close(cmd.دننه);

	بهر = xfdopen(cmd.بهر, "r");
	طالما (strbuf_getline_lf(&کرښه, بهر) != EOF) {
		بینښه حرف دوګونی[GIT_MAX_RAWSZ];
		إذا (کرښه.len != the_hash_algo->hexsz ||
		    !hex_منزل_بایټونه(دوګونی, کرښه.buf, کرښه.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", عدم, دوګونی, عدم);
	}

	fclose(بهر);
	إذا (پای_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&کرښه);
	حرر(بنسټ_name);
}

صحيح cmd_شاخص_pack(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *مختاړی,
		   بنية repository *repo)
{
	صحيح i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_شاخص;
	ثابت حرف *curr_شاخص;
	حرف *curr_rev_شاخص = عدم;
	ثابت حرف *شاخص_name = عدم, *pack_name = عدم, *rev_شاخص_name = عدم;
	ثابت حرف *keep_پیغام = عدم;
	ثابت حرف *promisor_پیغام = عدم;
	بنية strbuf شاخص_name_buf = STRBUF_INIT;
	بنية strbuf rev_شاخص_name_buf = STRBUF_INIT;
	بنية pack_idx_ثبت **idx_objects;
	بنية pack_idx_option opts;
	بینښه حرف pack_hash[GIT_MAX_RAWSZ];
	بینښه foreign_nr = 1;	/* zero is a "good" value, assume bad */
	صحيح report_پای_ټکی_د_ننوت = 0;
	صحيح hash_algo = 0;

	show_usage_if_asked(argc, argv, شاخص_pack_usage);

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
	repo_config(the_repository, git_شاخص_pack_config, &opts);
	إذا (مختاړی && chdir(مختاړی))
		die(_("Cannot come back to cwd"));

	إذا (git_env_bool(GIT_TEST_NO_WRITE_REV_شاخص, 0))
		rev_شاخص = 0;
	وإلا
		rev_شاخص = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	لكل (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		إذا (*arg == '-') {
			إذا (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} وإلا إذا (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} وإلا إذا (skip_منزل_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_سېټ_پیغام_types(&fsck_options, arg);
			} وإلا إذا (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} وإلا إذا (skip_منزل_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_سېټ_پیغام_types(&fsck_options, arg);
			} وإلا إذا (!strcmp(arg, "--verify")) {
				verify = 1;
			} وإلا إذا (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} وإلا إذا (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} وإلا إذا (skip_منزل_optional_arg(arg, "--keep", &keep_پیغام)) {
				; /* nothing to do */
			} وإلا إذا (skip_منزل_optional_arg(arg, "--promisor", &promisor_پیغام)) {
				ریکارډ_outgoing_links = 1;
			} وإلا إذا (starts_with(arg, "--threads=")) {
				حرف *پای_ټکی;
				nr_تارونه = strtoul(arg+10, &پای_ټکی, 0);
				إذا (!arg[10] || *پای_ټکی || nr_تارونه < 0)
					usage(شاخص_pack_usage);
				إذا (!HAVE_تارونه && nr_تارونه != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_تارونه = 1;
				}
			} وإلا إذا (skip_مختاړی(arg, "--pack_header=", &arg)) {
				إذا (parse_pack_سرلیک_option(arg,
							     ننوت_بفر,
							     &ننوت_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} وإلا إذا (!strcmp(arg, "-v")) {
				verbose = 1;
			} وإلا إذا (!strcmp(arg, "--progress-title")) {
				إذا (پرمختګ_title || (i+1) >= argc)
					usage(شاخص_pack_usage);
				پرمختګ_title = argv[++i];
			} وإلا إذا (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_پرمختګ = 1;
			} وإلا إذا (!strcmp(arg, "--report-end-of-input")) {
				report_پای_ټکی_د_ننوت = 1;
			} وإلا إذا (!strcmp(arg, "-o")) {
				إذا (شاخص_name || (i+1) >= argc)
					usage(شاخص_pack_usage);
				شاخص_name = argv[++i];
			} وإلا إذا (starts_with(arg, "--index-version=")) {
				حرف *c;
				opts.نسخه = strtoul(arg + 16, &c, 10);
				إذا (opts.نسخه > 2)
					die(_("bad %s"), arg);
				إذا (*c == ',')
					opts.off32_حد = strtoul(c+1, &c, 0);
				إذا (*c || opts.off32_حد & 0x80000000)
					die(_("bad %s"), arg);
			} وإلا إذا (skip_مختاړی(arg, "--max-input-size=", &arg)) {
				max_ننوت_اندازه = strtoumax(arg, عدم, 10);
			} وإلا إذا (skip_مختاړی(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				إذا (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_سېټ_hash_algo(the_repository, hash_algo);
			} وإلا إذا (!strcmp(arg, "--rev-index")) {
				rev_شاخص = 1;
			} وإلا إذا (!strcmp(arg, "--no-rev-index")) {
				rev_شاخص = 0;
			} وإلا
				usage(شاخص_pack_usage);
			استمر;
		}

		إذا (pack_name)
			usage(شاخص_pack_usage);
		pack_name = arg;
	}

	إذا (!pack_name && !from_stdin)
		usage(شاخص_pack_usage);
	إذا (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	إذا (promisor_پیغام && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	إذا (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	إذا (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	إذا (!شاخص_name && pack_name)
		شاخص_name = derive_filename(pack_name, "pack", "idx", &شاخص_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	إذا (!the_repository->hash_algo)
		repo_سېټ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	إذا (rev_شاخص) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		إذا (شاخص_name)
			rev_شاخص_name = derive_filename(شاخص_name,
							 "idx", "rev",
							 &rev_شاخص_name_buf);
	}

	إذا (verify) {
		إذا (!شاخص_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, شاخص_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	إذا (strict)
		opts.flags |= WRITE_IDX_STRICT;

	إذا (HAVE_تارونه && !nr_تارونه) {
		nr_تارونه = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		إذا (nr_تارونه < 4)
			; /* too few cores to consider capping */
		وإلا إذا (nr_تارونه < 6)
			nr_تارونه = 3; /* historic cap */
		وإلا إذا (nr_تارونه < 40)
			nr_تارونه /= 2;
		وإلا
			nr_تارونه = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_سرلیک();
	CALLOC_ARRAY(objects, st_جمع_کړه(nr_objects, 1));
	إذا (show_stat)
		CALLOC_ARRAY(obj_stat, st_جمع_کړه(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	إذا (report_پای_ټکی_د_ننوت)
		write_دننه_ډک(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	حرر(ofs_deltas);
	حرر(ref_deltas);
	إذا (strict)
		foreign_nr = check_objects();

	إذا (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	لكل (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_شاخص = write_idx_file(the_repository, شاخص_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	إذا (rev_شاخص)
		curr_rev_شاخص = write_rev_file(the_repository, rev_شاخص_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	حرر(idx_objects);

	إذا (!verify)
		final(pack_name, curr_pack,
		      شاخص_name, curr_شاخص,
		      rev_شاخص_name, curr_rev_شاخص,
		      keep_پیغام, promisor_پیغام,
		      pack_hash);
	وإلا
		close(ننوت_fd);

	إذا (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		إذا (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		إذا (fsck_پای(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	حرر(opts.anomaly);
	حرر(objects);
	strbuf_release(&شاخص_name_buf);
	strbuf_release(&rev_شاخص_name_buf);
	إذا (!pack_name)
		حرر((فراغ *) curr_pack);
	إذا (!شاخص_name)
		حرر((فراغ *) curr_شاخص);
	حرر(curr_rev_شاخص);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	إذا (check_self_contained_and_connected && foreign_nr)
		أرجع 1;

	أرجع 0;
}
