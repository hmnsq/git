#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن ثابت حرف اشاريو_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

بنية object_داخلا {
	بنية pack_idx_داخلا idx;
	سائيز_t سائيز;
	بنانشان حرف hdr_سائيز;
	نشانوارو حرف type;
	نشانوارو حرف real_type;
};

بنية object_stat {
	بنانشان delta_اونهائي;
	صحيح بنياد_object_no;
};

بنية بنياد_معلومات {
	/* Initialized by make_base(). */
	بنية بنياد_معلومات *بنياد;
	بنية object_داخلا *obj;
	صحيح ref_پهريون, ref_آخري;
	صحيح ofs_پهريون, ofs_آخري;
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
	صحيح children_بچيل;

	/* Not initialized by make_base(). */
	بنية list_سر list;
	فراغ *معلومات;
	سائيز_t سائيز;
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
ساكن LIST_سر(پورو_سر);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
ساكن سائيز_t بنياد_cache_استعمال_ٿيل;
ساكن سائيز_t بنياد_cache_حد_بندي;

بنية thread_local_معلومات {
	pthread_t thread;
	صحيح pack_fd;
};

/* Remember to update object flag allocation in object.h */
#وصف_ڪريو FLAG_LINK (1u<<20)
#وصف_ڪريو FLAG_CHECKED (1u<<21)

بنية ofs_delta_داخلا {
	off_t وٿي;
	صحيح obj_no;
};

بنية ref_delta_داخلا {
	بنية object_id oid;
	صحيح obj_no;
};

ساكن بنية object_داخلا *objects;
ساكن بنية object_stat *obj_stat;
ساكن بنية ofs_delta_داخلا *ofs_deltas;
ساكن بنية ref_delta_داخلا *ref_deltas;
ساكن بنية thread_local_معلومات nothread_معلومات;
ساكن صحيح nr_objects;
ساكن صحيح nr_ofs_deltas;
ساكن صحيح nr_ref_deltas;
ساكن صحيح ref_deltas_alloc;
ساكن صحيح nr_resolved_deltas;
ساكن صحيح nr_ڌاڳا;

ساكن صحيح from_stdin;
ساكن صحيح strict;
ساكن صحيح do_fsck_object;
ساكن بنية fsck_options fsck_options;
ساكن صحيح verbose;
ساكن ثابت حرف *ترقي_title;
ساكن صحيح show_resolving_ترقي;
ساكن صحيح show_stat;
ساكن صحيح check_self_contained_and_connected;

ساكن بنية ترقي *ترقي;

ساكن بنانشان حرف داخلا_بفر[DEFAULT_IO_بفر_سائيز];
ساكن بنانشان صحيح داخلا_وٿي, داخلا_len;
ساكن off_t consumed_بائيٽون;
ساكن off_t max_داخلا_سائيز;
ساكن بنانشان سڀ_کان_اونهو_delta;
ساكن بنية git_hash_ctx داخلا_ctx;
ساكن uint32_t داخلا_چڪري_جاچ_32;
ساكن صحيح داخلا_fd, output_fd;
ساكن ثابت حرف *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
ساكن بنية oidset outgoing_links = OIDSET_INIT;
ساكن صحيح رڪارڊ_outgoing_links;

ساكن بنية thread_local_معلومات *thread_معلومات;
ساكن صحيح nr_dispatched;
ساكن صحيح ڌاڳا_سرگرم;

ساكن pthread_mutex_t read_mutex;
#وصف_ڪريو read_تالو()		تالو_mutex(&read_mutex)
#وصف_ڪريو read_unlock()		unlock_mutex(&read_mutex)

ساكن pthread_mutex_t counter_mutex;
#وصف_ڪريو counter_تالو()		تالو_mutex(&counter_mutex)
#وصف_ڪريو counter_unlock()	unlock_mutex(&counter_mutex)

ساكن pthread_mutex_t work_mutex;
#وصف_ڪريو work_تالو()		تالو_mutex(&work_mutex)
#وصف_ڪريو work_unlock()		unlock_mutex(&work_mutex)

ساكن pthread_mutex_t سڀ_کان_اونهو_delta_mutex;
#وصف_ڪريو سڀ_کان_اونهو_delta_تالو()	تالو_mutex(&سڀ_کان_اونهو_delta_mutex)
#وصف_ڪريو سڀ_کان_اونهو_delta_unlock()	unlock_mutex(&سڀ_کان_اونهو_delta_mutex)

ساكن pthread_ڪنجي_t ڪنجي;

ساكن مضمن فراغ تالو_mutex(pthread_mutex_t *mutex)
{
	إذا (ڌاڳا_سرگرم)
		pthread_mutex_تالو(mutex);
}

ساكن مضمن فراغ unlock_mutex(pthread_mutex_t *mutex)
{
	إذا (ڌاڳا_سرگرم)
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
		pthread_mutex_init(&سڀ_کان_اونهو_delta_mutex, عدم);
	pthread_ڪنجي_create(&ڪنجي, عدم);
	CALLOC_ARRAY(thread_معلومات, nr_ڌاڳا);
	لكل (i = 0; i < nr_ڌاڳا; i++) {
		thread_معلومات[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	ڌاڳا_سرگرم = 1;
}

ساكن فراغ cleanup_thread(فراغ)
{
	صحيح i;
	إذا (!ڌاڳا_سرگرم)
		أرجع;
	ڌاڳا_سرگرم = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	إذا (show_stat)
		pthread_mutex_destroy(&سڀ_کان_اونهو_delta_mutex);
	لكل (i = 0; i < nr_ڌاڳا; i++)
		close(thread_معلومات[i].pack_fd);
	pthread_ڪنجي_delete(ڪنجي);
	حرر(thread_معلومات);
}

ساكن صحيح mark_link(بنية object *obj, تعداد object_type type,
		     فراغ *معلومات اڻ_استعمال_ٿيل,
		     بنية fsck_options *options اڻ_استعمال_ٿيل)
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
ساكن بنانشان check_object(بنية object *obj)
{
	إذا (!obj)
		أرجع 0;

	إذا (!(obj->flags & FLAG_LINK))
		أرجع 0;

	إذا (!(obj->flags & FLAG_CHECKED)) {
		سائيز_t سائيز;
		صحيح type = odb_read_object_info(the_repository->objects,
						&obj->oid, &سائيز);
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

ساكن بنانشان check_objects(فراغ)
{
	بنانشان i, max, foreign_nr = 0;

	max = get_max_object_اشاريو(the_repository);

	إذا (verbose)
		ترقي = شروعات_delayed_ترقي(the_repository,
						  _("Checking objects"), max);

	لكل (i = 0; i < max; i++) {
		foreign_nr += check_object(get_اشاريي_وارو_object(the_repository, i));
		display_ترقي(ترقي, i + 1);
	}

	stop_ترقي(&ترقي);
	أرجع foreign_nr;
}


/* Discard current buffer used content. */
ساكن فراغ flush(فراغ)
{
	إذا (داخلا_وٿي) {
		إذا (output_fd >= 0)
			write_or_die(output_fd, داخلا_بفر, داخلا_وٿي);
		git_hash_update(&داخلا_ctx, داخلا_بفر, داخلا_وٿي);
		memmove(داخلا_بفر, داخلا_بفر + داخلا_وٿي, داخلا_len);
		داخلا_وٿي = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ساكن فراغ *fill(صحيح min)
{
	إذا (min <= داخلا_len)
		أرجع داخلا_بفر + داخلا_وٿي;
	إذا (min > حجم(داخلا_بفر))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	افعل {
		ssize_t ret = xread(داخلا_fd, داخلا_بفر + داخلا_len,
				حجم(داخلا_بفر) - داخلا_len);
		إذا (ret <= 0) {
			إذا (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		داخلا_len += ret;
		إذا (from_stdin)
			display_throughput(ترقي, consumed_بائيٽون + داخلا_len);
	} طالما (داخلا_len < min);
	أرجع داخلا_بفر;
}

ساكن فراغ use(صحيح بائيٽون)
{
	إذا (بائيٽون > داخلا_len)
		die(_("used more bytes than were available"));
	داخلا_چڪري_جاچ_32 = چڪري_جاچ_32(داخلا_چڪري_جاچ_32, داخلا_بفر + داخلا_وٿي, بائيٽون);
	داخلا_len -= بائيٽون;
	داخلا_وٿي += بائيٽون;

	/* make sure off_t is sufficiently large not to wrap */
	إذا (signed_جوڙيو_overflows(consumed_بائيٽون, بائيٽون))
		die(_("pack too large for current definition of off_t"));
	consumed_بائيٽون += بائيٽون;
	إذا (max_داخلا_سائيز && consumed_بائيٽون > max_داخلا_سائيز) {
		بنية strbuf سائيز_حد_بندي = STRBUF_INIT;
		strbuf_humanise_بائيٽون(&سائيز_حد_بندي, max_داخلا_سائيز);
		die(_("pack exceeds maximum allowed size (%s)"),
		    سائيز_حد_بندي.buf);
	}
}

ساكن ثابت حرف *open_pack_file(ثابت حرف *pack_name)
{
	إذا (from_stdin) {
		داخلا_fd = 0;
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
		داخلا_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_معلومات.pack_fd = داخلا_fd;
	}
	git_hash_init(&داخلا_ctx, the_hash_algo);
	أرجع pack_name;
}

ساكن فراغ parse_pack_سرو(فراغ)
{
	بنانشان حرف *hdr = fill(حجم(بنية pack_سرو));

	/* Header consistency check */
	إذا (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	إذا (!pack_نسخو_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(حجم(بنية pack_سرو));
}

__attribute__((format (لکو, 2, 3)))
ساكن NORETURN فراغ bad_object(off_t وٿي, ثابت حرف *format, ...)
{
	va_list params;
	حرف buf[1024];

	va_شروعات(params, format);
	vsnprintf(buf, حجم(buf), format, params);
	va_پڇاڙي(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)وٿي, buf);
}

ساكن مضمن بنية thread_local_معلومات *get_thread_معلومات(فراغ)
{
	إذا (HAVE_ڌاڳا) {
		إذا (ڌاڳا_سرگرم)
			أرجع pthread_getspecific(ڪنجي);
		أكد(!ڌاڳا_سرگرم &&
		       "This should only be reached when all threads are gone");
	}
	أرجع &nothread_معلومات;
}

ساكن فراغ سيٽ_thread_معلومات(بنية thread_local_معلومات *معلومات)
{
	إذا (ڌاڳا_سرگرم)
		pthread_setspecific(ڪنجي, معلومات);
}

ساكن فراغ free_بنياد_معلومات(بنية بنياد_معلومات *c)
{
	إذا (c->معلومات) {
		FREE_AND_NULL(c->معلومات);
		بنياد_cache_استعمال_ٿيل -= c->سائيز;
	}
}

ساكن فراغ prune_بنياد_معلومات(بنية بنياد_معلومات *retain)
{
	بنية list_سر *pos;

	إذا (بنياد_cache_استعمال_ٿيل <= بنياد_cache_حد_بندي)
		أرجع;

	list_for_each_prev(pos, &پورو_سر) {
		بنية بنياد_معلومات *b = list_داخلا(pos, بنية بنياد_معلومات, list);
		إذا (b->retain_معلومات || b == retain)
			استمر;
		إذا (b->معلومات) {
			free_بنياد_معلومات(b);
			إذا (بنياد_cache_استعمال_ٿيل <= بنياد_cache_حد_بندي)
				أرجع;
		}
	}

	list_for_each_prev(pos, &work_سر) {
		بنية بنياد_معلومات *b = list_داخلا(pos, بنية بنياد_معلومات, list);
		إذا (b->retain_معلومات || b == retain)
			استمر;
		إذا (b->معلومات) {
			free_بنياد_معلومات(b);
			إذا (بنياد_cache_استعمال_ٿيل <= بنياد_cache_حد_بندي)
				أرجع;
		}
	}
}

ساكن صحيح is_delta_type(تعداد object_type type)
{
	أرجع (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

ساكن فراغ *unpack_داخلا_معلومات(off_t وٿي, سائيز_t سائيز,
			       تعداد object_type type, بنية object_id *oid)
{
	ساكن حرف fixed_buf[8192];
	صحيح حالت;
	git_zstream وهڪرو;
	فراغ *buf;
	بنية git_hash_ctx c;
	حرف hdr[32];
	صحيح hdrlen;

	إذا (!is_delta_type(type)) {
		hdrlen = format_object_سرو(hdr, حجم(hdr), type, سائيز);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} وإلا
		oid = عدم;
	إذا (type == OBJ_BLOB &&
	    سائيز > repo_settings_get_big_file_حد(the_repository))
		buf = fixed_buf;
	وإلا
		buf = xmallocz(سائيز);

	memset(&وهڪرو, 0, حجم(وهڪرو));
	git_inflate_init(&وهڪرو);
	وهڪرو.ايندڙ_ٻاهريون = buf;
	وهڪرو.avail_ٻاهريون = buf == fixed_buf ? حجم(fixed_buf) : سائيز;

	افعل {
		بنانشان حرف *آخري_ٻاهريون = وهڪرو.ايندڙ_ٻاهريون;
		وهڪرو.ايندڙ_اندريون = fill(1);
		وهڪرو.avail_اندريون = داخلا_len;
		حالت = git_inflate(&وهڪرو, 0);
		use(داخلا_len - وهڪرو.avail_اندريون);
		إذا (oid)
			git_hash_update(&c, آخري_ٻاهريون, وهڪرو.ايندڙ_ٻاهريون - آخري_ٻاهريون);
		إذا (buf == fixed_buf) {
			وهڪرو.ايندڙ_ٻاهريون = buf;
			وهڪرو.avail_ٻاهريون = حجم(fixed_buf);
		}
	} طالما (حالت == Z_OK);
	إذا (وهڪرو.ڪل_ٻاهريون != سائيز || حالت != Z_وهڪرو_پڇاڙي)
		bad_object(وٿي, _("inflate returned %d"), حالت);
	git_inflate_پڇاڙي(&وهڪرو);
	إذا (oid)
		git_hash_final_oid(oid, &c);
	أرجع buf == fixed_buf ? عدم : buf;
}

ساكن فراغ *unpack_raw_داخلا(بنية object_داخلا *obj,
			      off_t *ofs_وٿي,
			      بنية object_id *ref_oid,
			      بنية object_id *oid)
{
	بنانشان حرف *p;
	سائيز_t سائيز, c;
	off_t بنياد_وٿي;
	بنانشان سرڪايو;
	فراغ *معلومات;

	obj->idx.وٿي = consumed_بائيٽون;
	داخلا_چڪري_جاچ_32 = چڪري_جاچ_32(0, عدم, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	سائيز = (c & 15);
	سرڪايو = 4;
	طالما (c & 0x80) {
		إذا ((bitsizeof(سائيز_t) - 7) < سرڪايو)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		سائيز += (c & 0x7f) << سرڪايو;
		سرڪايو += 7;
	}
	obj->سائيز = سائيز;

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
		بنياد_وٿي = c & 127;
		طالما (c & 128) {
			بنياد_وٿي += 1;
			إذا (!بنياد_وٿي || MSB(بنياد_وٿي, 7))
				bad_object(obj->idx.وٿي, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			بنياد_وٿي = (بنياد_وٿي << 7) + (c & 127);
		}
		*ofs_وٿي = obj->idx.وٿي - بنياد_وٿي;
		إذا (*ofs_وٿي <= 0 || *ofs_وٿي >= obj->idx.وٿي)
			bad_object(obj->idx.وٿي, _("delta base offset is out of bound"));
		اكسر;
	حالة OBJ_COMMIT:
	حالة OBJ_وڻ:
	حالة OBJ_BLOB:
	حالة OBJ_ٽيگ:
		اكسر;
	افتراضي:
		bad_object(obj->idx.وٿي, _("unknown object type %d"), obj->type);
	}
	obj->hdr_سائيز = consumed_بائيٽون - obj->idx.وٿي;

	معلومات = unpack_داخلا_معلومات(obj->idx.وٿي, obj->سائيز, obj->type, oid);
	obj->idx.چڪري_جاچ_32 = داخلا_چڪري_جاچ_32;
	أرجع معلومات;
}

ساكن فراغ *unpack_معلومات(بنية object_داخلا *obj,
			 صحيح (*consume)(ثابت بنانشان حرف *, بنانشان ڊگهو, فراغ *),
			 فراغ *cb_معلومات)
{
	off_t from = obj[0].idx.وٿي + obj[0].hdr_سائيز;
	off_t len = obj[1].idx.وٿي - from;
	بنانشان حرف *معلومات, *inbuf;
	git_zstream وهڪرو;
	صحيح حالت;

	معلومات = xmallocz(consume ? 64*1024 : obj->سائيز);
	inbuf = xmalloc((len < 64*1024) ? (صحيح)len : 64*1024);

	memset(&وهڪرو, 0, حجم(وهڪرو));
	git_inflate_init(&وهڪرو);
	وهڪرو.ايندڙ_ٻاهريون = معلومات;
	وهڪرو.avail_ٻاهريون = consume ? 64*1024 : obj->سائيز;

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
		وهڪرو.ايندڙ_اندريون = inbuf;
		وهڪرو.avail_اندريون = n;
		إذا (!consume)
			حالت = git_inflate(&وهڪرو, 0);
		وإلا {
			افعل {
				حالت = git_inflate(&وهڪرو, 0);
				إذا (consume(معلومات, وهڪرو.ايندڙ_ٻاهريون - معلومات, cb_معلومات)) {
					حرر(inbuf);
					حرر(معلومات);
					أرجع عدم;
				}
				وهڪرو.ايندڙ_ٻاهريون = معلومات;
				وهڪرو.avail_ٻاهريون = 64*1024;
			} طالما (حالت == Z_OK && وهڪرو.avail_اندريون);
		}
	} طالما (len && حالت == Z_OK && !وهڪرو.avail_اندريون);

	/* This has been inflated OK when first encountered, so... */
	إذا (حالت != Z_وهڪرو_پڇاڙي || وهڪرو.ڪل_ٻاهريون != obj->سائيز)
		die(_("serious inflate inconsistency"));

	git_inflate_پڇاڙي(&وهڪرو);
	حرر(inbuf);
	إذا (consume) {
		FREE_AND_NULL(معلومات);
	}
	أرجع معلومات;
}

ساكن فراغ *get_معلومات_from_pack(بنية object_داخلا *obj)
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

ساكن صحيح ڳوليو_ofs_delta(ثابت off_t وٿي)
{
	صحيح پهريون = 0, آخري = nr_ofs_deltas;

	طالما (پهريون < آخري) {
		صحيح ايندڙ = پهريون + (آخري - پهريون) / 2;
		بنية ofs_delta_داخلا *delta = &ofs_deltas[ايندڙ];
		صحيح cmp;

		cmp = compare_ofs_delta_bases(وٿي, delta->وٿي,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		إذا (!cmp)
			أرجع ايندڙ;
		إذا (cmp < 0) {
			آخري = ايندڙ;
			استمر;
		}
		پهريون = ايندڙ+1;
	}
	أرجع -پهريون-1;
}

ساكن فراغ ڳوليو_ofs_delta_children(off_t وٿي,
				    صحيح *پهريون_اشاريو, صحيح *آخري_اشاريو)
{
	صحيح پهريون = ڳوليو_ofs_delta(وٿي);
	صحيح آخري = پهريون;
	صحيح پڇاڙي = nr_ofs_deltas - 1;

	إذا (پهريون < 0) {
		*پهريون_اشاريو = 0;
		*آخري_اشاريو = -1;
		أرجع;
	}
	طالما (پهريون > 0 && ofs_deltas[پهريون - 1].وٿي == وٿي)
		--پهريون;
	طالما (آخري < پڇاڙي && ofs_deltas[آخري + 1].وٿي == وٿي)
		++آخري;
	*پهريون_اشاريو = پهريون;
	*آخري_اشاريو = آخري;
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

ساكن صحيح ڳوليو_ref_delta(ثابت بنية object_id *oid)
{
	صحيح پهريون = 0, آخري = nr_ref_deltas;

	طالما (پهريون < آخري) {
		صحيح ايندڙ = پهريون + (آخري - پهريون) / 2;
		بنية ref_delta_داخلا *delta = &ref_deltas[ايندڙ];
		صحيح cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		إذا (!cmp)
			أرجع ايندڙ;
		إذا (cmp < 0) {
			آخري = ايندڙ;
			استمر;
		}
		پهريون = ايندڙ+1;
	}
	أرجع -پهريون-1;
}

ساكن فراغ ڳوليو_ref_delta_children(ثابت بنية object_id *oid,
				    صحيح *پهريون_اشاريو, صحيح *آخري_اشاريو)
{
	صحيح پهريون = ڳوليو_ref_delta(oid);
	صحيح آخري = پهريون;
	صحيح پڇاڙي = nr_ref_deltas - 1;

	إذا (پهريون < 0) {
		*پهريون_اشاريو = 0;
		*آخري_اشاريو = -1;
		أرجع;
	}
	طالما (پهريون > 0 && oideq(&ref_deltas[پهريون - 1].oid, oid))
		--پهريون;
	طالما (آخري < پڇاڙي && oideq(&ref_deltas[آخري + 1].oid, oid))
		++آخري;
	*پهريون_اشاريو = پهريون;
	*آخري_اشاريو = آخري;
}

بنية compare_معلومات {
	بنية object_داخلا *داخلا;
	بنية odb_وهڪرو *st;
	بنانشان حرف *buf;
	بنانشان ڊگهو buf_سائيز;
};

ساكن صحيح compare_objects(ثابت بنانشان حرف *buf, بنانشان ڊگهو سائيز,
			   فراغ *cb_معلومات)
{
	بنية compare_معلومات *معلومات = cb_معلومات;

	إذا (معلومات->buf_سائيز < سائيز) {
		حرر(معلومات->buf);
		معلومات->buf = xmalloc(سائيز);
		معلومات->buf_سائيز = سائيز;
	}

	طالما (سائيز) {
		ssize_t len = odb_وهڪرو_read(معلومات->st, معلومات->buf, سائيز);
		إذا (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_منزل_hex(&معلومات->داخلا->idx.oid));
		إذا (len < 0)
			die(_("unable to read %s"),
			    oid_منزل_hex(&معلومات->داخلا->idx.oid));
		إذا (memcmp(buf, معلومات->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_منزل_hex(&معلومات->داخلا->idx.oid));
		سائيز -= len;
		buf += len;
	}
	أرجع 0;
}

ساكن صحيح check_collison(بنية object_داخلا *داخلا)
{
	بنية compare_معلومات معلومات;

	إذا (داخلا->سائيز <= repo_settings_get_big_file_حد(the_repository) ||
	    داخلا->type != OBJ_BLOB)
		أرجع -1;

	memset(&معلومات, 0, حجم(معلومات));
	معلومات.داخلا = داخلا;
	معلومات.st = odb_وهڪرو_from_object(the_repository->objects, &داخلا->idx.oid, عدم);
	إذا (!معلومات.st)
		أرجع -1;
	إذا (معلومات.st->سائيز != داخلا->سائيز || معلومات.st->type != داخلا->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_منزل_hex(&داخلا->idx.oid));
	unpack_معلومات(داخلا, compare_objects, &معلومات);
	odb_وهڪرو_close(معلومات.st);
	حرر(معلومات.buf);
	أرجع 0;
}

ساكن فراغ رڪارڊ_outgoing_link(ثابت بنية object_id *oid)
{
	oidset_وجهو(&outgoing_links, oid);
}

ساكن فراغ maybe_رڪارڊ_name_داخلا(ثابت بنية name_داخلا *داخلا)
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
	إذا (S_ISDIR(داخلا->گهڻائي))
		رڪارڊ_outgoing_link(&داخلا->oid);
}

ساكن فراغ do_رڪارڊ_outgoing_links(بنية object *obj)
{
	إذا (obj->type == OBJ_وڻ) {
		بنية وڻ *وڻ = (بنية وڻ *)obj;
		بنية وڻ_desc desc;
		بنية name_داخلا داخلا;
		إذا (init_وڻ_desc_gently(&desc, &وڻ->object.oid,
					  وڻ->بفر, وڻ->سائيز, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			أرجع;
		طالما (وڻ_داخلا_gently(&desc, &داخلا))
			maybe_رڪارڊ_name_داخلا(&داخلا);
	} وإلا إذا (obj->type == OBJ_COMMIT) {
		بنية commit *commit = (بنية commit *) obj;
		بنية commit_list *parents = commit->parents;

		رڪارڊ_outgoing_link(get_commit_وڻ_oid(commit));
		لكل (; parents; parents = parents->ايندڙ)
			رڪارڊ_outgoing_link(&parents->شي->object.oid);
	} وإلا إذا (obj->type == OBJ_ٽيگ) {
		بنية ٽيگ *ٽيگ = (بنية ٽيگ *) obj;
		رڪارڊ_outgoing_link(get_tagged_oid(ٽيگ));
	}
}

ساكن فراغ sha1_object(ثابت فراغ *معلومات, بنية object_داخلا *obj_داخلا,
			بنانشان ڊگهو سائيز, تعداد object_type type,
			ثابت بنية object_id *oid)
{
	فراغ *new_معلومات = عدم;
	صحيح collision_test_needed = 0;

	أكد(معلومات || obj_داخلا);

	إذا (startup_info->have_repository) {
		read_تالو();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	إذا (collision_test_needed && !معلومات) {
		read_تالو();
		إذا (!check_collison(obj_داخلا))
			collision_test_needed = 0;
		read_unlock();
	}
	إذا (collision_test_needed) {
		فراغ *has_معلومات;
		تعداد object_type has_type;
		سائيز_t has_سائيز;
		read_تالو();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_سائيز);
		إذا (has_type < 0)
			die(_("cannot read existing object info %s"), oid_منزل_hex(oid));
		إذا (has_type != type || has_سائيز != سائيز)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_منزل_hex(oid));
		has_معلومات = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_سائيز);
		read_unlock();
		إذا (!معلومات)
			معلومات = new_معلومات = get_معلومات_from_pack(obj_داخلا);
		إذا (!has_معلومات)
			die(_("cannot read existing object %s"), oid_منزل_hex(oid));
		إذا (سائيز != has_سائيز || type != has_type ||
		    memcmp(معلومات, has_معلومات, سائيز) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_منزل_hex(oid));
		حرر(has_معلومات);
	}

	إذا (strict || do_fsck_object || رڪارڊ_outgoing_links) {
		read_تالو();
		إذا (type == OBJ_BLOB) {
			بنية blob *blob = lookup_blob(the_repository, oid);
			إذا (blob)
				blob->object.flags |= FLAG_CHECKED;
			وإلا
				die(_("invalid blob object %s"), oid_منزل_hex(oid));
			إذا (do_fsck_object &&
			    fsck_object(&blob->object, (فراغ *)معلومات, سائيز, &fsck_options))
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
						  سائيز, buf,
						  &eaten);
			إذا (!obj)
				die(_("invalid %s"), type_name(type));
			إذا (do_fsck_object &&
			    fsck_object(obj, buf, سائيز, &fsck_options))
				die(_("fsck error in packed object"));
			إذا (strict && fsck_walk(obj, عدم, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_منزل_hex(&obj->oid));
			إذا (رڪارڊ_outgoing_links)
				do_رڪارڊ_outgoing_links(obj);

			إذا (obj->type == OBJ_وڻ) {
				بنية وڻ *شي = (بنية وڻ *) obj;
				شي->بفر = عدم;
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
ساكن فراغ *get_بنياد_معلومات(بنية بنياد_معلومات *c)
{
	إذا (!c->معلومات) {
		بنية object_داخلا *obj = c->obj;
		بنية بنياد_معلومات **delta = عدم;
		صحيح delta_nr = 0, delta_alloc = 0;

		طالما (is_delta_type(c->obj->type) && !c->معلومات) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->بنياد;
		}
		إذا (!delta_nr) {
			c->معلومات = get_معلومات_from_pack(obj);
			c->سائيز = obj->سائيز;
			بنياد_cache_استعمال_ٿيل += c->سائيز;
			prune_بنياد_معلومات(c);
		}
		لكل (; delta_nr > 0; delta_nr--) {
			فراغ *بنياد, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			بنياد = get_بنياد_معلومات(c->بنياد);
			raw = get_معلومات_from_pack(obj);
			c->معلومات = patch_delta(
				بنياد, c->بنياد->سائيز,
				raw, obj->سائيز,
				&c->سائيز);
			حرر(raw);
			إذا (!c->معلومات)
				bad_object(obj->idx.وٿي, _("failed to apply delta"));
			بنياد_cache_استعمال_ٿيل += c->سائيز;
			prune_بنياد_معلومات(c);
		}
		حرر(delta);
	}
	أرجع c->معلومات;
}

ساكن بنية بنياد_معلومات *make_بنياد(بنية object_داخلا *obj,
				   بنية بنياد_معلومات *والد)
{
	بنية بنياد_معلومات *بنياد = xcalloc(1, حجم(بنية بنياد_معلومات));
	بنياد->بنياد = والد;
	بنياد->obj = obj;
	ڳوليو_ref_delta_children(&obj->idx.oid,
				&بنياد->ref_پهريون, &بنياد->ref_آخري);
	ڳوليو_ofs_delta_children(obj->idx.وٿي,
				&بنياد->ofs_پهريون, &بنياد->ofs_آخري);
	بنياد->children_بچيل = بنياد->ref_آخري - بنياد->ref_پهريون +
		بنياد->ofs_آخري - بنياد->ofs_پهريون + 2;
	أرجع بنياد;
}

ساكن بنية بنياد_معلومات *resolve_delta(بنية object_داخلا *delta_obj,
				       بنية بنياد_معلومات *بنياد)
{
	فراغ *delta_معلومات, *نتيجو_معلومات;
	بنية بنياد_معلومات *نتيجو;
	سائيز_t نتيجو_سائيز;

	إذا (show_stat) {
		صحيح i = delta_obj - objects;
		صحيح j = بنياد->obj - objects;
		obj_stat[i].delta_اونهائي = obj_stat[j].delta_اونهائي + 1;
		سڀ_کان_اونهو_delta_تالو();
		إذا (سڀ_کان_اونهو_delta < obj_stat[i].delta_اونهائي)
			سڀ_کان_اونهو_delta = obj_stat[i].delta_اونهائي;
		سڀ_کان_اونهو_delta_unlock();
		obj_stat[i].بنياد_object_no = j;
	}
	delta_معلومات = get_معلومات_from_pack(delta_obj);
	أكد(بنياد->معلومات);
	نتيجو_معلومات = patch_delta(بنياد->معلومات, بنياد->سائيز,
				  delta_معلومات, delta_obj->سائيز, &نتيجو_سائيز);
	حرر(delta_معلومات);
	إذا (!نتيجو_معلومات)
		bad_object(delta_obj->idx.وٿي, _("failed to apply delta"));
	hash_object_file(the_hash_algo, نتيجو_معلومات, نتيجو_سائيز,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(نتيجو_معلومات, عدم, نتيجو_سائيز, delta_obj->real_type,
		    &delta_obj->idx.oid);

	نتيجو = make_بنياد(delta_obj, بنياد);
	نتيجو->معلومات = نتيجو_معلومات;
	نتيجو->سائيز = نتيجو_سائيز;

	counter_تالو();
	nr_resolved_deltas++;
	counter_unlock();

	أرجع نتيجو;
}

ساكن صحيح compare_ofs_delta_داخلا(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية ofs_delta_داخلا *delta_a = a;
	ثابت بنية ofs_delta_داخلا *delta_b = b;

	أرجع delta_a->وٿي < delta_b->وٿي ? -1 :
	       delta_a->وٿي > delta_b->وٿي ?  1 :
	       0;
}

ساكن صحيح compare_ref_delta_داخلا(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية ref_delta_داخلا *delta_a = a;
	ثابت بنية ref_delta_داخلا *delta_b = b;

	أرجع oidcmp(&delta_a->oid, &delta_b->oid);
}

ساكن فراغ *threaded_ٻيو_pass(فراغ *معلومات)
{
	إذا (معلومات)
		سيٽ_thread_معلومات(معلومات);
	لكل (;;) {
		بنية بنياد_معلومات *والد = عدم;
		بنية object_داخلا *ٻار_obj = عدم;
		بنية بنياد_معلومات *ٻار = عدم;

		counter_تالو();
		display_ترقي(ترقي, nr_resolved_deltas);
		counter_unlock();

		work_تالو();
		إذا (list_خالي_جايون(&work_سر)) {
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
			ٻار_obj = &objects[nr_dispatched++];
		} وإلا {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			والد = list_پهريون_داخلا(&work_سر, بنية بنياد_معلومات,
						  list);

			طالما (والد->ref_پهريون <= والد->ref_آخري) {
				صحيح وٿي = ref_deltas[والد->ref_پهريون++].obj_no;
				ٻار_obj = objects + وٿي;
				إذا (ٻار_obj->real_type != OBJ_REF_DELTA) {
					ٻار_obj = عدم;
					استمر;
				}
				ٻار_obj->real_type = والد->obj->real_type;
				اكسر;
			}

			إذا (!ٻار_obj && والد->ofs_پهريون <= والد->ofs_آخري) {
				ٻار_obj = objects +
					ofs_deltas[والد->ofs_پهريون++].obj_no;
				أكد(ٻار_obj->real_type == OBJ_OFS_DELTA);
				ٻار_obj->real_type = والد->obj->real_type;
			}

			إذا (والد->ref_پهريون > والد->ref_آخري &&
			    والد->ofs_پهريون > والد->ofs_آخري) {
				/*
				 * This parent has run out of children, so move
				 * it to done_head.
				 */
				list_del(&والد->list);
				list_جوڙيو(&والد->list, &پورو_سر);
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
			get_بنياد_معلومات(والد);
			والد->retain_معلومات++;
		}
		work_unlock();

		إذا (ٻار_obj) {
			إذا (والد) {
				ٻار = resolve_delta(ٻار_obj, والد);
				إذا (!ٻار->children_بچيل)
					FREE_AND_NULL(ٻار->معلومات);
			} وإلا{
				ٻار = make_بنياد(ٻار_obj, عدم);
				إذا (ٻار->children_بچيل) {
					/*
					 * Since this child has its own delta children,
					 * we will need this data in the future.
					 * Inflate now so that future iterations will
					 * have access to this object's data while
					 * outside the work mutex.
					 */
					ٻار->معلومات = get_معلومات_from_pack(ٻار_obj);
					ٻار->سائيز = ٻار_obj->سائيز;
				}
			}
		}

		work_تالو();
		إذا (والد)
			والد->retain_معلومات--;

		إذا (ٻار && ٻار->معلومات) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_جوڙيو(&ٻار->list, &work_سر);
			بنياد_cache_استعمال_ٿيل += ٻار->سائيز;
			prune_بنياد_معلومات(عدم);
		} وإلا إذا (ٻار) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			بنية بنياد_معلومات *p = والد;

			طالما (p) {
				بنية بنياد_معلومات *ايندڙ_p;

				p->children_بچيل--;
				إذا (p->children_بچيل)
					اكسر;

				ايندڙ_p = p->بنياد;
				free_بنياد_معلومات(p);
				list_del(&p->list);
				حرر(p);

				p = ايندڙ_p;
			}
			FREE_AND_NULL(ٻار);
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
ساكن فراغ parse_pack_objects(بنانشان حرف *hash)
{
	صحيح i, nr_delays = 0;
	بنية ofs_delta_داخلا *ofs_delta = ofs_deltas;
	بنية object_id ref_delta_oid;
	بنية stat st;
	بنية git_hash_ctx tmp_ctx;

	إذا (verbose)
		ترقي = شروعات_ترقي(
				the_repository,
				ترقي_title ? ترقي_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	لكل (i = 0; i < nr_objects; i++) {
		بنية object_داخلا *obj = &objects[i];
		فراغ *معلومات = unpack_raw_داخلا(obj, &ofs_delta->وٿي,
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
			sha1_object(معلومات, عدم, obj->سائيز, obj->type,
				    &obj->idx.oid);
		حرر(معلومات);
		display_ترقي(ترقي, i+1);
	}
	objects[i].idx.وٿي = consumed_بائيٽون;
	stop_ترقي(&ترقي);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &داخلا_ctx);
	git_hash_final(hash, &tmp_ctx);
	إذا (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	إذا (fstat(داخلا_fd, &st))
		die_errno(_("cannot fstat packfile"));
	إذا (S_ISREG(st.st_گهڻائي) &&
			lseek(داخلا_fd, 0, SEEK_CUR) - داخلا_len != st.st_سائيز)
		die(_("pack has junk at the end"));

	لكل (i = 0; i < nr_objects; i++) {
		بنية object_داخلا *obj = &objects[i];
		إذا (obj->real_type != OBJ_BAD)
			استمر;
		obj->real_type = obj->type;
		sha1_object(عدم, obj, obj->سائيز, obj->type,
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
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_داخلا);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_داخلا);

	إذا (verbose || show_resolving_ترقي)
		ترقي = شروعات_ترقي(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	بنياد_cache_حد_بندي = opts->delta_بنياد_cache_حد_بندي * nr_ڌاڳا;
	إذا (nr_ڌاڳا > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		لكل (i = 0; i < nr_ڌاڳا; i++) {
			صحيح ret = pthread_create(&thread_معلومات[i].thread, عدم,
						 threaded_ٻيو_pass, thread_معلومات + i);
			إذا (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		لكل (i = 0; i < nr_ڌاڳا; i++)
			pthread_join(thread_معلومات[i].thread, عدم);
		cleanup_thread();
		أرجع;
	}
	threaded_ٻيو_pass(&nothread_معلومات);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
ساكن فراغ fix_unresolved_deltas(بنية hashfile *f);
ساكن فراغ conclude_pack(صحيح fix_thin_pack, ثابت حرف *curr_pack, بنانشان حرف *pack_hash)
{
	إذا (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_ترقي(&ترقي);
		/* Flush remaining pack final hash. */
		flush();
		أرجع;
	}

	إذا (fix_thin_pack) {
		بنية hashfile *f;
		بنانشان حرف read_hash[GIT_MAX_RAWSZ], پڇ_hash[GIT_MAX_RAWSZ];
		بنية strbuf نياپو = STRBUF_INIT;
		صحيح nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		صحيح nr_objects_initial = nr_objects;
		إذا (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * حجم(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&نياپو, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_ترقي_نياپو(&ترقي, نياپو.buf);
		strbuf_release(&نياپو);
		finalize_hashfile(f, پڇ_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_سرو_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_بائيٽون-the_hash_algo->rawsz);
		إذا (!hasheq(read_hash, پڇ_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	إذا (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

ساكن صحيح write_compressed(بنية hashfile *f, فراغ *اندريون, بنانشان صحيح سائيز)
{
	git_zstream وهڪرو;
	صحيح حالت;
	بنانشان حرف outbuf[4096];
	بنية repo_config_قدرون *cfg = repo_config_قدرون(the_repository);

	git_deflate_init(&وهڪرو, cfg->zlib_compression_level);
	وهڪرو.ايندڙ_اندريون = اندريون;
	وهڪرو.avail_اندريون = سائيز;

	افعل {
		وهڪرو.ايندڙ_ٻاهريون = outbuf;
		وهڪرو.avail_ٻاهريون = حجم(outbuf);
		حالت = git_deflate(&وهڪرو, Z_پڄاڻي);
		hashwrite(f, outbuf, حجم(outbuf) - وهڪرو.avail_ٻاهريون);
	} طالما (حالت == Z_OK);

	إذا (حالت != Z_وهڪرو_پڇاڙي)
		die(_("unable to deflate appended object (%d)"), حالت);
	سائيز = وهڪرو.ڪل_ٻاهريون;
	git_deflate_پڇاڙي(&وهڪرو);
	أرجع سائيز;
}

ساكن بنية object_داخلا *append_obj_منزل_pack(بنية hashfile *f,
			       ثابت بنانشان حرف *sha1, فراغ *buf,
			       بنانشان ڊگهو سائيز, تعداد object_type type)
{
	بنية object_داخلا *obj = &objects[nr_objects++];
	بنانشان حرف سرو[10];
	بنانشان ڊگهو s = سائيز;
	صحيح n = 0;
	بنانشان حرف c = (type << 4) | (s & 15);
	s >>= 4;
	طالما (s) {
		سرو[n++] = c | 0x80;
		c = s & 0x7f;
		s >>= 7;
	}
	سرو[n++] = c;
	چڪري_جاچ_32_begin(f);
	hashwrite(f, سرو, n);
	obj[0].سائيز = سائيز;
	obj[0].hdr_سائيز = n;
	obj[0].type = type;
	obj[0].real_type = type;
	obj[1].idx.وٿي = obj[0].idx.وٿي + n;
	obj[1].idx.وٿي += write_compressed(f, buf, سائيز);
	obj[0].idx.چڪري_جاچ_32 = چڪري_جاچ_32_پڇاڙي(f);
	hashflush(f);
	oidread(&obj->idx.oid, sha1, the_repository->hash_algo);
	أرجع obj;
}

ساكن صحيح delta_pos_compare(ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية ref_delta_داخلا *a = *(بنية ref_delta_داخلا **)_a;
	بنية ref_delta_داخلا *b = *(بنية ref_delta_داخلا **)_b;
	أرجع a->obj_no - b->obj_no;
}

ساكن فراغ fix_unresolved_deltas(بنية hashfile *f)
{
	بنية ref_delta_داخلا **sorted_by_pos;
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
			بنية ref_delta_داخلا *d = sorted_by_pos[i];
			إذا (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, عدم,
							   OBJECT_INFO_FOR_PREFETCH))
				استمر;
			oid_array_append(&منزل_fetch, &d->oid);
		}
		promisor_remote_get_سڌو(the_repository,
					   منزل_fetch.oid, منزل_fetch.nr);
		oid_array_clear(&منزل_fetch);
	}

	لكل (i = 0; i < nr_ref_deltas; i++) {
		بنية ref_delta_داخلا *d = sorted_by_pos[i];
		تعداد object_type type;
		فراغ *معلومات;
		سائيز_t سائيز;

		إذا (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			استمر;
		معلومات = odb_read_object(the_repository->objects, &d->oid,
				       &type, &سائيز);
		إذا (!معلومات)
			استمر;

		إذا (check_object_signature(the_repository, &d->oid, معلومات, سائيز,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_منزل_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_منزل_pack(f, d->oid.hash, معلومات, سائيز, type);
		حرر(معلومات);
		threaded_ٻيو_pass(عدم);

		display_ترقي(ترقي, nr_resolved_deltas);
	}
	حرر(sorted_by_pos);
}

ساكن ثابت حرف *derive_filename(ثابت حرف *pack_name, ثابت حرف *strip,
				   ثابت حرف *suffix, بنية strbuf *buf)
{
	سائيز_t len;
	إذا (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_جوڙيو(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	أرجع buf->buf;
}

ساكن فراغ write_special_file(ثابت حرف *suffix, ثابت حرف *نياپو,
			       ثابت حرف *pack_name, ثابت بنانشان حرف *hash,
			       ثابت حرف **report)
{
	بنية strbuf name_buf = STRBUF_INIT;
	ثابت حرف *filename;
	صحيح fd;
	صحيح نياپو_len = لڙي_ڊيگهه(نياپو);

	إذا (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	وإلا
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = محفوظ_create_file_with_leading_directories(the_repository, filename);
	إذا (fd < 0) {
		إذا (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} وإلا {
		إذا (نياپو_len > 0) {
			write_or_die(fd, نياپو, نياپو_len);
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
				بنية strbuf *name, بنانشان حرف *hash,
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
		  ثابت حرف *final_اشاريو_name, ثابت حرف *curr_اشاريو_name,
		  ثابت حرف *final_rev_اشاريو_name, ثابت حرف *curr_rev_اشاريو_name,
		  ثابت حرف *keep_نياپو, ثابت حرف *promisor_نياپو,
		  بنانشان حرف *hash)
{
	ثابت حرف *report = "pack";
	بنية strbuf pack_name = STRBUF_INIT;
	بنية strbuf اشاريو_name = STRBUF_INIT;
	بنية strbuf rev_اشاريو_name = STRBUF_INIT;

	إذا (!from_stdin) {
		close(داخلا_fd);
	} وإلا {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		إذا (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	إذا (keep_نياپو)
		write_special_file("keep", keep_نياپو, final_pack_name, hash,
				   &report);
	إذا (promisor_نياپو)
		write_special_file("promisor", promisor_نياپو, final_pack_name,
				   hash, عدم);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	إذا (curr_rev_اشاريو_name)
		rename_tmp_packfile(&final_rev_اشاريو_name, curr_rev_اشاريو_name,
				    &rev_اشاريو_name, hash, "rev", 1);
	rename_tmp_packfile(&final_اشاريو_name, curr_اشاريو_name, &اشاريو_name,
			    hash, "idx", 1);

	إذا (do_fsck_object && startup_info->have_repository) {
		بنية odb_ذريعو_files *files =
			odb_ذريعو_files_downcast(the_repository->objects->ذريعو);
		packfile_store_ڀريو_pack(files->dirs->packed, final_اشاريو_name, 0);
	}

	إذا (!from_stdin) {
		لکو("%s\n", hash_منزل_hex(hash));
	} وإلا {
		بنية strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_منزل_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_اندريون_ڀريل(1, داخلا_بفر + داخلا_وٿي, داخلا_len);
	}

	strbuf_release(&rev_اشاريو_name);
	strbuf_release(&اشاريو_name);
	strbuf_release(&pack_name);
}

ساكن صحيح git_اشاريو_pack_config(ثابت حرف *k, ثابت حرف *v,
				 ثابت بنية config_context *ctx, فراغ *cb)
{
	بنية pack_idx_option *opts = cb;

	إذا (!strcmp(k, "pack.indexversion")) {
		opts->نسخو = git_config_int(k, v, ctx->kvi);
		إذا (opts->نسخو > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->نسخو);
		أرجع 0;
	}
	إذا (!strcmp(k, "pack.threads")) {
		nr_ڌاڳا = git_config_int(k, v, ctx->kvi);
		إذا (nr_ڌاڳا < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_ڌاڳا);
		إذا (!HAVE_ڌاڳا && nr_ڌاڳا != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_ڌاڳا = 1;
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
		opts->delta_بنياد_cache_حد_بندي = git_config_ulong(k, v, ctx->kvi);
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
	idx1 = (((ثابت uint32_t *)((ثابت uint8_t *)p->اشاريو_معلومات + p->چڪري_جاچ_وٿي))
		+ (سائيز_t)p->انگ_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->انگ_objects;

	لكل (i = 0; i < p->انگ_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		إذا (!(off & 0x80000000))
			استمر;
		off = off & 0x7fffffff;
		check_pack_اشاريو_ptr(p, &idx2[off * 2]);
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
	بنية packed_git *p = جوڙيو_packed_git(the_repository, pack_name,
					      لڙي_ڊيگهه(pack_name), 1);

	إذا (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	إذا (open_pack_اشاريو(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->نسخو = p->اشاريو_نسخو;

	إذا (opts->نسخو == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_اشاريو(p);
	حرر(p);
}

ساكن فراغ show_pack_info(صحيح stat_only)
{
	صحيح i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	بنانشان ڊگهو *chain_histogram = عدم;

	إذا (سڀ_کان_اونهو_delta)
		CALLOC_ARRAY(chain_histogram, سڀ_کان_اونهو_delta);

	لكل (i = 0; i < nr_objects; i++) {
		بنية object_داخلا *obj = &objects[i];

		إذا (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_اونهائي - 1]++;
		إذا (stat_only)
			استمر;
		لکو("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_منزل_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->سائيز,
		       (uintmax_t)(obj[1].idx.وٿي - obj->idx.وٿي),
		       (uintmax_t)obj->idx.وٿي);
		إذا (is_delta_type(obj->type)) {
			بنية object_داخلا *bobj = &objects[obj_stat[i].بنياد_object_no];
			لکو(" %u %s", obj_stat[i].delta_اونهائي,
			       oid_منزل_hex(&bobj->idx.oid));
		}
		اکر_لکو('\n');
	}

	إذا (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	لكل (i = 0; i < سڀ_کان_اونهو_delta; i++) {
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
	بنية ٻار_process cmd = ٻار_PROCESS_INIT;
	FILE *ٻاهريون;
	بنية strbuf سٽ = STRBUF_INIT;
	بنية oidset_iter iter;
	بنية object_id *oid;
	حرف *بنياد_name = عدم;

	إذا (!oidset_سائيز(&outgoing_links))
		أرجع;

	oidset_iter_init(&outgoing_links, &iter);
	طالما ((oid = oidset_iter_ايندڙ(&iter))) {
		بنية odb_ذريعو_info ذريعو_info;
		بنية object_info info = {
			.ذريعو_infop = &ذريعو_info,
		};

		إذا (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			استمر;
		إذا (ذريعو_info.ذريعو->type == ODB_ذريعو_PACKED &&
		    ذريعو_info.u.packed.pack->pack_promisor)
			استمر;

		إذا (!cmd.دليلون.nr) {
			بنياد_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_ڌڪو(&cmd.دليلون, "pack-objects");
			strvec_ڌڪو(&cmd.دليلون,
				    "--exclude-promisor-objects-best-effort");
			strvec_ڌڪو(&cmd.دليلون, بنياد_name);
			cmd.git_cmd = 1;
			cmd.اندريون = -1;
			cmd.ٻاهريون = -1;
			إذا (شروعات_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		إذا (write_اندريون_ڀريل(cmd.اندريون, oid_منزل_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_اندريون_ڀريل(cmd.اندريون, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	إذا (!cmd.دليلون.nr)
		أرجع;

	close(cmd.اندريون);

	ٻاهريون = xfdopen(cmd.ٻاهريون, "r");
	طالما (strbuf_getline_lf(&سٽ, ٻاهريون) != EOF) {
		بنانشان حرف ٻٽو[GIT_MAX_RAWSZ];
		إذا (سٽ.len != the_hash_algo->hexsz ||
		    !hex_منزل_بائيٽون(ٻٽو, سٽ.buf, سٽ.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", عدم, ٻٽو, عدم);
	}

	fclose(ٻاهريون);
	إذا (پڄاڻي_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&سٽ);
	حرر(بنياد_name);
}

صحيح cmd_اشاريو_pack(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *اڳياڙي,
		   بنية repository *repo)
{
	صحيح i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_اشاريو;
	ثابت حرف *curr_اشاريو;
	حرف *curr_rev_اشاريو = عدم;
	ثابت حرف *اشاريو_name = عدم, *pack_name = عدم, *rev_اشاريو_name = عدم;
	ثابت حرف *keep_نياپو = عدم;
	ثابت حرف *promisor_نياپو = عدم;
	بنية strbuf اشاريو_name_buf = STRBUF_INIT;
	بنية strbuf rev_اشاريو_name_buf = STRBUF_INIT;
	بنية pack_idx_داخلا **idx_objects;
	بنية pack_idx_option opts;
	بنانشان حرف pack_hash[GIT_MAX_RAWSZ];
	بنانشان foreign_nr = 1;	/* zero is a "good" value, assume bad */
	صحيح report_پڇاڙي_جو_داخلا = 0;
	صحيح hash_algo = 0;

	show_usage_if_asked(argc, argv, اشاريو_pack_usage);

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
	repo_config(the_repository, git_اشاريو_pack_config, &opts);
	إذا (اڳياڙي && chdir(اڳياڙي))
		die(_("Cannot come back to cwd"));

	إذا (git_env_bool(GIT_TEST_NO_WRITE_REV_اشاريو, 0))
		rev_اشاريو = 0;
	وإلا
		rev_اشاريو = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

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
				fsck_سيٽ_نياپو_types(&fsck_options, arg);
			} وإلا إذا (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} وإلا إذا (skip_منزل_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_سيٽ_نياپو_types(&fsck_options, arg);
			} وإلا إذا (!strcmp(arg, "--verify")) {
				verify = 1;
			} وإلا إذا (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} وإلا إذا (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} وإلا إذا (skip_منزل_optional_arg(arg, "--keep", &keep_نياپو)) {
				; /* nothing to do */
			} وإلا إذا (skip_منزل_optional_arg(arg, "--promisor", &promisor_نياپو)) {
				رڪارڊ_outgoing_links = 1;
			} وإلا إذا (starts_with(arg, "--threads=")) {
				حرف *پڇاڙي;
				nr_ڌاڳا = strtoul(arg+10, &پڇاڙي, 0);
				إذا (!arg[10] || *پڇاڙي || nr_ڌاڳا < 0)
					usage(اشاريو_pack_usage);
				إذا (!HAVE_ڌاڳا && nr_ڌاڳا != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_ڌاڳا = 1;
				}
			} وإلا إذا (skip_اڳياڙي(arg, "--pack_header=", &arg)) {
				إذا (parse_pack_سرو_option(arg,
							     داخلا_بفر,
							     &داخلا_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} وإلا إذا (!strcmp(arg, "-v")) {
				verbose = 1;
			} وإلا إذا (!strcmp(arg, "--progress-title")) {
				إذا (ترقي_title || (i+1) >= argc)
					usage(اشاريو_pack_usage);
				ترقي_title = argv[++i];
			} وإلا إذا (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_ترقي = 1;
			} وإلا إذا (!strcmp(arg, "--report-end-of-input")) {
				report_پڇاڙي_جو_داخلا = 1;
			} وإلا إذا (!strcmp(arg, "-o")) {
				إذا (اشاريو_name || (i+1) >= argc)
					usage(اشاريو_pack_usage);
				اشاريو_name = argv[++i];
			} وإلا إذا (starts_with(arg, "--index-version=")) {
				حرف *c;
				opts.نسخو = strtoul(arg + 16, &c, 10);
				إذا (opts.نسخو > 2)
					die(_("bad %s"), arg);
				إذا (*c == ',')
					opts.off32_حد_بندي = strtoul(c+1, &c, 0);
				إذا (*c || opts.off32_حد_بندي & 0x80000000)
					die(_("bad %s"), arg);
			} وإلا إذا (skip_اڳياڙي(arg, "--max-input-size=", &arg)) {
				max_داخلا_سائيز = strtoumax(arg, عدم, 10);
			} وإلا إذا (skip_اڳياڙي(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				إذا (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_سيٽ_hash_algo(the_repository, hash_algo);
			} وإلا إذا (!strcmp(arg, "--rev-index")) {
				rev_اشاريو = 1;
			} وإلا إذا (!strcmp(arg, "--no-rev-index")) {
				rev_اشاريو = 0;
			} وإلا
				usage(اشاريو_pack_usage);
			استمر;
		}

		إذا (pack_name)
			usage(اشاريو_pack_usage);
		pack_name = arg;
	}

	إذا (!pack_name && !from_stdin)
		usage(اشاريو_pack_usage);
	إذا (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	إذا (promisor_نياپو && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	إذا (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	إذا (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	إذا (!اشاريو_name && pack_name)
		اشاريو_name = derive_filename(pack_name, "pack", "idx", &اشاريو_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	إذا (!the_repository->hash_algo)
		repo_سيٽ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	إذا (rev_اشاريو) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		إذا (اشاريو_name)
			rev_اشاريو_name = derive_filename(اشاريو_name,
							 "idx", "rev",
							 &rev_اشاريو_name_buf);
	}

	إذا (verify) {
		إذا (!اشاريو_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, اشاريو_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	إذا (strict)
		opts.flags |= WRITE_IDX_STRICT;

	إذا (HAVE_ڌاڳا && !nr_ڌاڳا) {
		nr_ڌاڳا = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		إذا (nr_ڌاڳا < 4)
			; /* too few cores to consider capping */
		وإلا إذا (nr_ڌاڳا < 6)
			nr_ڌاڳا = 3; /* historic cap */
		وإلا إذا (nr_ڌاڳا < 40)
			nr_ڌاڳا /= 2;
		وإلا
			nr_ڌاڳا = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_سرو();
	CALLOC_ARRAY(objects, st_جوڙيو(nr_objects, 1));
	إذا (show_stat)
		CALLOC_ARRAY(obj_stat, st_جوڙيو(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	إذا (report_پڇاڙي_جو_داخلا)
		write_اندريون_ڀريل(2, "\0", 1);
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
	curr_اشاريو = write_idx_file(the_repository, اشاريو_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	إذا (rev_اشاريو)
		curr_rev_اشاريو = write_rev_file(the_repository, rev_اشاريو_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	حرر(idx_objects);

	إذا (!verify)
		final(pack_name, curr_pack,
		      اشاريو_name, curr_اشاريو,
		      rev_اشاريو_name, curr_rev_اشاريو,
		      keep_نياپو, promisor_نياپو,
		      pack_hash);
	وإلا
		close(داخلا_fd);

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

		إذا (fsck_پڄاڻي(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	حرر(opts.anomaly);
	حرر(objects);
	strbuf_release(&اشاريو_name_buf);
	strbuf_release(&rev_اشاريو_name_buf);
	إذا (!pack_name)
		حرر((فراغ *) curr_pack);
	إذا (!اشاريو_name)
		حرر((فراغ *) curr_اشاريو);
	حرر(curr_rev_اشاريو);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	إذا (check_self_contained_and_connected && foreign_nr)
		أرجع 1;

	أرجع 0;
}
