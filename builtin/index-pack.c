#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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

بنية object_مدخل {
	بنية pack_idx_مدخل idx;
	اندازه_t اندازه;
	بیعلامت حرف hdr_اندازه;
	علامتدار حرف type;
	علامتدار حرف real_type;
};

بنية object_stat {
	بیعلامت delta_عمق;
	صحيح پایه_object_no;
};

بنية پایه_داده {
	/* Initialized by make_base(). */
	بنية پایه_داده *پایه;
	بنية object_مدخل *obj;
	صحيح ref_اولی, ref_آخرین;
	صحيح ofs_اولی, ofs_آخرین;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	صحيح retain_داده;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	صحيح children_مانده;

	/* Not initialized by make_base(). */
	بنية list_سر list;
	فراغ *داده;
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
ساكن LIST_سر(انجامشده_سر);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
ساكن اندازه_t پایه_cache_استفاده_شده;
ساكن اندازه_t پایه_cache_حد;

بنية thread_local_داده {
	pthread_t thread;
	صحيح pack_fd;
};

/* Remember to update object flag allocation in object.h */
#تعریف_کن FLAG_LINK (1u<<20)
#تعریف_کن FLAG_CHECKED (1u<<21)

بنية ofs_delta_مدخل {
	off_t جابه_جایی;
	صحيح obj_no;
};

بنية ref_delta_مدخل {
	بنية object_id oid;
	صحيح obj_no;
};

ساكن بنية object_مدخل *objects;
ساكن بنية object_stat *obj_stat;
ساكن بنية ofs_delta_مدخل *ofs_deltas;
ساكن بنية ref_delta_مدخل *ref_deltas;
ساكن بنية thread_local_داده nothread_داده;
ساكن صحيح nr_objects;
ساكن صحيح nr_ofs_deltas;
ساكن صحيح nr_ref_deltas;
ساكن صحيح ref_deltas_alloc;
ساكن صحيح nr_resolved_deltas;
ساكن صحيح nr_نخها;

ساكن صحيح from_stdin;
ساكن صحيح strict;
ساكن صحيح do_fsck_object;
ساكن بنية fsck_options fsck_options;
ساكن صحيح verbose;
ساكن ثابت حرف *پیشرفت_title;
ساكن صحيح show_resolving_پیشرفت;
ساكن صحيح show_stat;
ساكن صحيح check_self_contained_and_connected;

ساكن بنية پیشرفت *پیشرفت;

ساكن بیعلامت حرف درون_داد_میانگیر[DEFAULT_IO_میانگیر_اندازه];
ساكن بیعلامت صحيح درون_داد_جابه_جایی, درون_داد_len;
ساكن off_t consumed_بایتها;
ساكن off_t max_درون_داد_اندازه;
ساكن بیعلامت عمیقترین_delta;
ساكن بنية git_hash_ctx درون_داد_ctx;
ساكن uint32_t درون_داد_بررسی_چرخهای_32;
ساكن صحيح درون_داد_fd, output_fd;
ساكن ثابت حرف *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
ساكن بنية oidset outgoing_links = OIDSET_INIT;
ساكن صحيح رکورد_outgoing_links;

ساكن بنية thread_local_داده *thread_داده;
ساكن صحيح nr_dispatched;
ساكن صحيح نخها_فعال;

ساكن pthread_mutex_t read_mutex;
#تعریف_کن read_قفل()		قفل_mutex(&read_mutex)
#تعریف_کن read_unlock()		unlock_mutex(&read_mutex)

ساكن pthread_mutex_t counter_mutex;
#تعریف_کن counter_قفل()		قفل_mutex(&counter_mutex)
#تعریف_کن counter_unlock()	unlock_mutex(&counter_mutex)

ساكن pthread_mutex_t work_mutex;
#تعریف_کن work_قفل()		قفل_mutex(&work_mutex)
#تعریف_کن work_unlock()		unlock_mutex(&work_mutex)

ساكن pthread_mutex_t عمیقترین_delta_mutex;
#تعریف_کن عمیقترین_delta_قفل()	قفل_mutex(&عمیقترین_delta_mutex)
#تعریف_کن عمیقترین_delta_unlock()	unlock_mutex(&عمیقترین_delta_mutex)

ساكن pthread_کلید_t کلید;

ساكن مضمن فراغ قفل_mutex(pthread_mutex_t *mutex)
{
	إذا (نخها_فعال)
		pthread_mutex_قفل(mutex);
}

ساكن مضمن فراغ unlock_mutex(pthread_mutex_t *mutex)
{
	إذا (نخها_فعال)
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
		pthread_mutex_init(&عمیقترین_delta_mutex, عدم);
	pthread_کلید_create(&کلید, عدم);
	CALLOC_ARRAY(thread_داده, nr_نخها);
	لكل (i = 0; i < nr_نخها; i++) {
		thread_داده[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	نخها_فعال = 1;
}

ساكن فراغ cleanup_thread(فراغ)
{
	صحيح i;
	إذا (!نخها_فعال)
		أرجع;
	نخها_فعال = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&counter_mutex);
	pthread_mutex_destroy(&work_mutex);
	إذا (show_stat)
		pthread_mutex_destroy(&عمیقترین_delta_mutex);
	لكل (i = 0; i < nr_نخها; i++)
		close(thread_داده[i].pack_fd);
	pthread_کلید_delete(کلید);
	حرر(thread_داده);
}

ساكن صحيح mark_link(بنية object *obj, تعداد object_type type,
		     فراغ *داده استفادهنشده,
		     بنية fsck_options *options استفادهنشده)
{
	إذا (!obj)
		أرجع -1;

	إذا (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_مقصد_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	أرجع 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
ساكن بیعلامت check_object(بنية object *obj)
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
			      oid_مقصد_hex(&obj->oid));
		إذا (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_مقصد_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		أرجع 1;
	}

	أرجع 0;
}

ساكن بیعلامت check_objects(فراغ)
{
	بیعلامت i, max, foreign_nr = 0;

	max = get_max_object_شاخص(the_repository);

	إذا (verbose)
		پیشرفت = شروع_delayed_پیشرفت(the_repository,
						  _("Checking objects"), max);

	لكل (i = 0; i < max; i++) {
		foreign_nr += check_object(get_نمایهای_object(the_repository, i));
		display_پیشرفت(پیشرفت, i + 1);
	}

	stop_پیشرفت(&پیشرفت);
	أرجع foreign_nr;
}


/* Discard current buffer used content. */
ساكن فراغ flush(فراغ)
{
	إذا (درون_داد_جابه_جایی) {
		إذا (output_fd >= 0)
			write_or_die(output_fd, درون_داد_میانگیر, درون_داد_جابه_جایی);
		git_hash_update(&درون_داد_ctx, درون_داد_میانگیر, درون_داد_جابه_جایی);
		memmove(درون_داد_میانگیر, درون_داد_میانگیر + درون_داد_جابه_جایی, درون_داد_len);
		درون_داد_جابه_جایی = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ساكن فراغ *fill(صحيح min)
{
	إذا (min <= درون_داد_len)
		أرجع درون_داد_میانگیر + درون_داد_جابه_جایی;
	إذا (min > حجم(درون_داد_میانگیر))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	افعل {
		ssize_t ret = xread(درون_داد_fd, درون_داد_میانگیر + درون_داد_len,
				حجم(درون_داد_میانگیر) - درون_داد_len);
		إذا (ret <= 0) {
			إذا (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		درون_داد_len += ret;
		إذا (from_stdin)
			display_throughput(پیشرفت, consumed_بایتها + درون_داد_len);
	} طالما (درون_داد_len < min);
	أرجع درون_داد_میانگیر;
}

ساكن فراغ use(صحيح بایتها)
{
	إذا (بایتها > درون_داد_len)
		die(_("used more bytes than were available"));
	درون_داد_بررسی_چرخهای_32 = بررسی_چرخهای_32(درون_داد_بررسی_چرخهای_32, درون_داد_میانگیر + درون_داد_جابه_جایی, بایتها);
	درون_داد_len -= بایتها;
	درون_داد_جابه_جایی += بایتها;

	/* make sure off_t is sufficiently large not to wrap */
	إذا (signed_جمع_کن_overflows(consumed_بایتها, بایتها))
		die(_("pack too large for current definition of off_t"));
	consumed_بایتها += بایتها;
	إذا (max_درون_داد_اندازه && consumed_بایتها > max_درون_داد_اندازه) {
		بنية strbuf اندازه_حد = STRBUF_INIT;
		strbuf_humanise_بایتها(&اندازه_حد, max_درون_داد_اندازه);
		die(_("pack exceeds maximum allowed size (%s)"),
		    اندازه_حد.buf);
	}
}

ساكن ثابت حرف *open_pack_file(ثابت حرف *pack_name)
{
	إذا (from_stdin) {
		درون_داد_fd = 0;
		إذا (!pack_name) {
			بنية strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, عدم);
		} وإلا {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_داده.pack_fd = output_fd;
	} وإلا {
		درون_داد_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_داده.pack_fd = درون_داد_fd;
	}
	git_hash_init(&درون_داد_ctx, the_hash_algo);
	أرجع pack_name;
}

ساكن فراغ parse_pack_سرآیند(فراغ)
{
	بیعلامت حرف *hdr = fill(حجم(بنية pack_سرآیند));

	/* Header consistency check */
	إذا (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	إذا (!pack_نسخه_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(حجم(بنية pack_سرآیند));
}

__attribute__((format (چاپ_کن, 2, 3)))
ساكن NORETURN فراغ bad_object(off_t جابه_جایی, ثابت حرف *format, ...)
{
	va_list params;
	حرف buf[1024];

	va_شروع(params, format);
	vsnprintf(buf, حجم(buf), format, params);
	va_انتها(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)جابه_جایی, buf);
}

ساكن مضمن بنية thread_local_داده *get_thread_داده(فراغ)
{
	إذا (HAVE_نخها) {
		إذا (نخها_فعال)
			أرجع pthread_getspecific(کلید);
		أكد(!نخها_فعال &&
		       "This should only be reached when all threads are gone");
	}
	أرجع &nothread_داده;
}

ساكن فراغ مجموعه_thread_داده(بنية thread_local_داده *داده)
{
	إذا (نخها_فعال)
		pthread_setspecific(کلید, داده);
}

ساكن فراغ free_پایه_داده(بنية پایه_داده *c)
{
	إذا (c->داده) {
		FREE_AND_NULL(c->داده);
		پایه_cache_استفاده_شده -= c->اندازه;
	}
}

ساكن فراغ prune_پایه_داده(بنية پایه_داده *retain)
{
	بنية list_سر *pos;

	إذا (پایه_cache_استفاده_شده <= پایه_cache_حد)
		أرجع;

	list_for_each_prev(pos, &انجامشده_سر) {
		بنية پایه_داده *b = list_مدخل(pos, بنية پایه_داده, list);
		إذا (b->retain_داده || b == retain)
			استمر;
		إذا (b->داده) {
			free_پایه_داده(b);
			إذا (پایه_cache_استفاده_شده <= پایه_cache_حد)
				أرجع;
		}
	}

	list_for_each_prev(pos, &work_سر) {
		بنية پایه_داده *b = list_مدخل(pos, بنية پایه_داده, list);
		إذا (b->retain_داده || b == retain)
			استمر;
		إذا (b->داده) {
			free_پایه_داده(b);
			إذا (پایه_cache_استفاده_شده <= پایه_cache_حد)
				أرجع;
		}
	}
}

ساكن صحيح is_delta_type(تعداد object_type type)
{
	أرجع (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

ساكن فراغ *unpack_مدخل_داده(off_t جابه_جایی, اندازه_t اندازه,
			       تعداد object_type type, بنية object_id *oid)
{
	ساكن حرف fixed_buf[8192];
	صحيح وضعیت;
	git_zstream جریان;
	فراغ *buf;
	بنية git_hash_ctx c;
	حرف hdr[32];
	صحيح hdrlen;

	إذا (!is_delta_type(type)) {
		hdrlen = format_object_سرآیند(hdr, حجم(hdr), type, اندازه);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} وإلا
		oid = عدم;
	إذا (type == OBJ_BLOB &&
	    اندازه > repo_settings_get_big_file_آستانه(the_repository))
		buf = fixed_buf;
	وإلا
		buf = xmallocz(اندازه);

	memset(&جریان, 0, حجم(جریان));
	git_inflate_init(&جریان);
	جریان.بعدی_بیرون = buf;
	جریان.avail_بیرون = buf == fixed_buf ? حجم(fixed_buf) : اندازه;

	افعل {
		بیعلامت حرف *آخرین_بیرون = جریان.بعدی_بیرون;
		جریان.بعدی_درون = fill(1);
		جریان.avail_درون = درون_داد_len;
		وضعیت = git_inflate(&جریان, 0);
		use(درون_داد_len - جریان.avail_درون);
		إذا (oid)
			git_hash_update(&c, آخرین_بیرون, جریان.بعدی_بیرون - آخرین_بیرون);
		إذا (buf == fixed_buf) {
			جریان.بعدی_بیرون = buf;
			جریان.avail_بیرون = حجم(fixed_buf);
		}
	} طالما (وضعیت == Z_OK);
	إذا (جریان.مجموع_بیرون != اندازه || وضعیت != Z_جریان_انتها)
		bad_object(جابه_جایی, _("inflate returned %d"), وضعیت);
	git_inflate_انتها(&جریان);
	إذا (oid)
		git_hash_final_oid(oid, &c);
	أرجع buf == fixed_buf ? عدم : buf;
}

ساكن فراغ *unpack_raw_مدخل(بنية object_مدخل *obj,
			      off_t *ofs_جابه_جایی,
			      بنية object_id *ref_oid,
			      بنية object_id *oid)
{
	بیعلامت حرف *p;
	اندازه_t اندازه, c;
	off_t پایه_جابه_جایی;
	بیعلامت لغزش;
	فراغ *داده;

	obj->idx.جابه_جایی = consumed_بایتها;
	درون_داد_بررسی_چرخهای_32 = بررسی_چرخهای_32(0, عدم, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	اندازه = (c & 15);
	لغزش = 4;
	طالما (c & 0x80) {
		إذا ((bitsizeof(اندازه_t) - 7) < لغزش)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		اندازه += (c & 0x7f) << لغزش;
		لغزش += 7;
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
		پایه_جابه_جایی = c & 127;
		طالما (c & 128) {
			پایه_جابه_جایی += 1;
			إذا (!پایه_جابه_جایی || MSB(پایه_جابه_جایی, 7))
				bad_object(obj->idx.جابه_جایی, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			پایه_جابه_جایی = (پایه_جابه_جایی << 7) + (c & 127);
		}
		*ofs_جابه_جایی = obj->idx.جابه_جایی - پایه_جابه_جایی;
		إذا (*ofs_جابه_جایی <= 0 || *ofs_جابه_جایی >= obj->idx.جابه_جایی)
			bad_object(obj->idx.جابه_جایی, _("delta base offset is out of bound"));
		اكسر;
	حالة OBJ_COMMIT:
	حالة OBJ_درخت:
	حالة OBJ_BLOB:
	حالة OBJ_برچسب:
		اكسر;
	افتراضي:
		bad_object(obj->idx.جابه_جایی, _("unknown object type %d"), obj->type);
	}
	obj->hdr_اندازه = consumed_بایتها - obj->idx.جابه_جایی;

	داده = unpack_مدخل_داده(obj->idx.جابه_جایی, obj->اندازه, obj->type, oid);
	obj->idx.بررسی_چرخهای_32 = درون_داد_بررسی_چرخهای_32;
	أرجع داده;
}

ساكن فراغ *unpack_داده(بنية object_مدخل *obj,
			 صحيح (*consume)(ثابت بیعلامت حرف *, بیعلامت دراز, فراغ *),
			 فراغ *cb_داده)
{
	off_t from = obj[0].idx.جابه_جایی + obj[0].hdr_اندازه;
	off_t len = obj[1].idx.جابه_جایی - from;
	بیعلامت حرف *داده, *inbuf;
	git_zstream جریان;
	صحيح وضعیت;

	داده = xmallocz(consume ? 64*1024 : obj->اندازه);
	inbuf = xmalloc((len < 64*1024) ? (صحيح)len : 64*1024);

	memset(&جریان, 0, حجم(جریان));
	git_inflate_init(&جریان);
	جریان.بعدی_بیرون = داده;
	جریان.avail_بیرون = consume ? 64*1024 : obj->اندازه;

	افعل {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_داده()->pack_fd, inbuf, n, from);
		إذا (n < 0)
			die_errno(_("cannot pread pack file"));
		إذا (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		جریان.بعدی_درون = inbuf;
		جریان.avail_درون = n;
		إذا (!consume)
			وضعیت = git_inflate(&جریان, 0);
		وإلا {
			افعل {
				وضعیت = git_inflate(&جریان, 0);
				إذا (consume(داده, جریان.بعدی_بیرون - داده, cb_داده)) {
					حرر(inbuf);
					حرر(داده);
					أرجع عدم;
				}
				جریان.بعدی_بیرون = داده;
				جریان.avail_بیرون = 64*1024;
			} طالما (وضعیت == Z_OK && جریان.avail_درون);
		}
	} طالما (len && وضعیت == Z_OK && !جریان.avail_درون);

	/* This has been inflated OK when first encountered, so... */
	إذا (وضعیت != Z_جریان_انتها || جریان.مجموع_بیرون != obj->اندازه)
		die(_("serious inflate inconsistency"));

	git_inflate_انتها(&جریان);
	حرر(inbuf);
	إذا (consume) {
		FREE_AND_NULL(داده);
	}
	أرجع داده;
}

ساكن فراغ *get_داده_from_pack(بنية object_مدخل *obj)
{
	أرجع unpack_داده(obj, عدم, عدم);
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

ساكن صحيح پیدا_کن_ofs_delta(ثابت off_t جابه_جایی)
{
	صحيح اولی = 0, آخرین = nr_ofs_deltas;

	طالما (اولی < آخرین) {
		صحيح بعدی = اولی + (آخرین - اولی) / 2;
		بنية ofs_delta_مدخل *delta = &ofs_deltas[بعدی];
		صحيح cmp;

		cmp = compare_ofs_delta_bases(جابه_جایی, delta->جابه_جایی,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		إذا (!cmp)
			أرجع بعدی;
		إذا (cmp < 0) {
			آخرین = بعدی;
			استمر;
		}
		اولی = بعدی+1;
	}
	أرجع -اولی-1;
}

ساكن فراغ پیدا_کن_ofs_delta_children(off_t جابه_جایی,
				    صحيح *اولی_شاخص, صحيح *آخرین_شاخص)
{
	صحيح اولی = پیدا_کن_ofs_delta(جابه_جایی);
	صحيح آخرین = اولی;
	صحيح انتها = nr_ofs_deltas - 1;

	إذا (اولی < 0) {
		*اولی_شاخص = 0;
		*آخرین_شاخص = -1;
		أرجع;
	}
	طالما (اولی > 0 && ofs_deltas[اولی - 1].جابه_جایی == جابه_جایی)
		--اولی;
	طالما (آخرین < انتها && ofs_deltas[آخرین + 1].جابه_جایی == جابه_جایی)
		++آخرین;
	*اولی_شاخص = اولی;
	*آخرین_شاخص = آخرین;
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

ساكن صحيح پیدا_کن_ref_delta(ثابت بنية object_id *oid)
{
	صحيح اولی = 0, آخرین = nr_ref_deltas;

	طالما (اولی < آخرین) {
		صحيح بعدی = اولی + (آخرین - اولی) / 2;
		بنية ref_delta_مدخل *delta = &ref_deltas[بعدی];
		صحيح cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		إذا (!cmp)
			أرجع بعدی;
		إذا (cmp < 0) {
			آخرین = بعدی;
			استمر;
		}
		اولی = بعدی+1;
	}
	أرجع -اولی-1;
}

ساكن فراغ پیدا_کن_ref_delta_children(ثابت بنية object_id *oid,
				    صحيح *اولی_شاخص, صحيح *آخرین_شاخص)
{
	صحيح اولی = پیدا_کن_ref_delta(oid);
	صحيح آخرین = اولی;
	صحيح انتها = nr_ref_deltas - 1;

	إذا (اولی < 0) {
		*اولی_شاخص = 0;
		*آخرین_شاخص = -1;
		أرجع;
	}
	طالما (اولی > 0 && oideq(&ref_deltas[اولی - 1].oid, oid))
		--اولی;
	طالما (آخرین < انتها && oideq(&ref_deltas[آخرین + 1].oid, oid))
		++آخرین;
	*اولی_شاخص = اولی;
	*آخرین_شاخص = آخرین;
}

بنية compare_داده {
	بنية object_مدخل *مدخل;
	بنية odb_جریان *st;
	بیعلامت حرف *buf;
	بیعلامت دراز buf_اندازه;
};

ساكن صحيح compare_objects(ثابت بیعلامت حرف *buf, بیعلامت دراز اندازه,
			   فراغ *cb_داده)
{
	بنية compare_داده *داده = cb_داده;

	إذا (داده->buf_اندازه < اندازه) {
		حرر(داده->buf);
		داده->buf = xmalloc(اندازه);
		داده->buf_اندازه = اندازه;
	}

	طالما (اندازه) {
		ssize_t len = odb_جریان_read(داده->st, داده->buf, اندازه);
		إذا (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_مقصد_hex(&داده->مدخل->idx.oid));
		إذا (len < 0)
			die(_("unable to read %s"),
			    oid_مقصد_hex(&داده->مدخل->idx.oid));
		إذا (memcmp(buf, داده->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_مقصد_hex(&داده->مدخل->idx.oid));
		اندازه -= len;
		buf += len;
	}
	أرجع 0;
}

ساكن صحيح check_collison(بنية object_مدخل *مدخل)
{
	بنية compare_داده داده;

	إذا (مدخل->اندازه <= repo_settings_get_big_file_آستانه(the_repository) ||
	    مدخل->type != OBJ_BLOB)
		أرجع -1;

	memset(&داده, 0, حجم(داده));
	داده.مدخل = مدخل;
	داده.st = odb_جریان_from_object(the_repository->objects, &مدخل->idx.oid, عدم);
	إذا (!داده.st)
		أرجع -1;
	إذا (داده.st->اندازه != مدخل->اندازه || داده.st->type != مدخل->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_مقصد_hex(&مدخل->idx.oid));
	unpack_داده(مدخل, compare_objects, &داده);
	odb_جریان_close(داده.st);
	حرر(داده.buf);
	أرجع 0;
}

ساكن فراغ رکورد_outgoing_link(ثابت بنية object_id *oid)
{
	oidset_درج(&outgoing_links, oid);
}

ساكن فراغ maybe_رکورد_name_مدخل(ثابت بنية name_مدخل *مدخل)
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
	إذا (S_ISDIR(مدخل->نما))
		رکورد_outgoing_link(&مدخل->oid);
}

ساكن فراغ do_رکورد_outgoing_links(بنية object *obj)
{
	إذا (obj->type == OBJ_درخت) {
		بنية درخت *درخت = (بنية درخت *)obj;
		بنية درخت_desc desc;
		بنية name_مدخل مدخل;
		إذا (init_درخت_desc_gently(&desc, &درخت->object.oid,
					  درخت->میانگیر, درخت->اندازه, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			أرجع;
		طالما (درخت_مدخل_gently(&desc, &مدخل))
			maybe_رکورد_name_مدخل(&مدخل);
	} وإلا إذا (obj->type == OBJ_COMMIT) {
		بنية commit *commit = (بنية commit *) obj;
		بنية commit_list *parents = commit->parents;

		رکورد_outgoing_link(get_commit_درخت_oid(commit));
		لكل (; parents; parents = parents->بعدی)
			رکورد_outgoing_link(&parents->قلم->object.oid);
	} وإلا إذا (obj->type == OBJ_برچسب) {
		بنية برچسب *برچسب = (بنية برچسب *) obj;
		رکورد_outgoing_link(get_tagged_oid(برچسب));
	}
}

ساكن فراغ sha1_object(ثابت فراغ *داده, بنية object_مدخل *obj_مدخل,
			بیعلامت دراز اندازه, تعداد object_type type,
			ثابت بنية object_id *oid)
{
	فراغ *new_داده = عدم;
	صحيح collision_test_needed = 0;

	أكد(داده || obj_مدخل);

	إذا (startup_info->have_repository) {
		read_قفل();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	إذا (collision_test_needed && !داده) {
		read_قفل();
		إذا (!check_collison(obj_مدخل))
			collision_test_needed = 0;
		read_unlock();
	}
	إذا (collision_test_needed) {
		فراغ *has_داده;
		تعداد object_type has_type;
		اندازه_t has_اندازه;
		read_قفل();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_اندازه);
		إذا (has_type < 0)
			die(_("cannot read existing object info %s"), oid_مقصد_hex(oid));
		إذا (has_type != type || has_اندازه != اندازه)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_مقصد_hex(oid));
		has_داده = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_اندازه);
		read_unlock();
		إذا (!داده)
			داده = new_داده = get_داده_from_pack(obj_مدخل);
		إذا (!has_داده)
			die(_("cannot read existing object %s"), oid_مقصد_hex(oid));
		إذا (اندازه != has_اندازه || type != has_type ||
		    memcmp(داده, has_داده, اندازه) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_مقصد_hex(oid));
		حرر(has_داده);
	}

	إذا (strict || do_fsck_object || رکورد_outgoing_links) {
		read_قفل();
		إذا (type == OBJ_BLOB) {
			بنية blob *blob = lookup_blob(the_repository, oid);
			إذا (blob)
				blob->object.flags |= FLAG_CHECKED;
			وإلا
				die(_("invalid blob object %s"), oid_مقصد_hex(oid));
			إذا (do_fsck_object &&
			    fsck_object(&blob->object, (فراغ *)داده, اندازه, &fsck_options))
				die(_("fsck error in packed object"));
		} وإلا {
			بنية object *obj;
			صحيح eaten;
			فراغ *buf = (فراغ *) داده;

			أكد(داده && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_میانگیر(the_repository, oid, type,
						  اندازه, buf,
						  &eaten);
			إذا (!obj)
				die(_("invalid %s"), type_name(type));
			إذا (do_fsck_object &&
			    fsck_object(obj, buf, اندازه, &fsck_options))
				die(_("fsck error in packed object"));
			إذا (strict && fsck_walk(obj, عدم, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_مقصد_hex(&obj->oid));
			إذا (رکورد_outgoing_links)
				do_رکورد_outgoing_links(obj);

			إذا (obj->type == OBJ_درخت) {
				بنية درخت *قلم = (بنية درخت *) obj;
				قلم->میانگیر = عدم;
				obj->parsed = 0;
			}
			إذا (obj->type == OBJ_COMMIT) {
				بنية commit *commit = (بنية commit *) obj;
				إذا (detach_commit_میانگیر(commit, عدم) != داده)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	حرر(new_داده);
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
ساكن فراغ *get_پایه_داده(بنية پایه_داده *c)
{
	إذا (!c->داده) {
		بنية object_مدخل *obj = c->obj;
		بنية پایه_داده **delta = عدم;
		صحيح delta_nr = 0, delta_alloc = 0;

		طالما (is_delta_type(c->obj->type) && !c->داده) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->پایه;
		}
		إذا (!delta_nr) {
			c->داده = get_داده_from_pack(obj);
			c->اندازه = obj->اندازه;
			پایه_cache_استفاده_شده += c->اندازه;
			prune_پایه_داده(c);
		}
		لكل (; delta_nr > 0; delta_nr--) {
			فراغ *پایه, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			پایه = get_پایه_داده(c->پایه);
			raw = get_داده_from_pack(obj);
			c->داده = patch_delta(
				پایه, c->پایه->اندازه,
				raw, obj->اندازه,
				&c->اندازه);
			حرر(raw);
			إذا (!c->داده)
				bad_object(obj->idx.جابه_جایی, _("failed to apply delta"));
			پایه_cache_استفاده_شده += c->اندازه;
			prune_پایه_داده(c);
		}
		حرر(delta);
	}
	أرجع c->داده;
}

ساكن بنية پایه_داده *make_پایه(بنية object_مدخل *obj,
				   بنية پایه_داده *والد)
{
	بنية پایه_داده *پایه = xcalloc(1, حجم(بنية پایه_داده));
	پایه->پایه = والد;
	پایه->obj = obj;
	پیدا_کن_ref_delta_children(&obj->idx.oid,
				&پایه->ref_اولی, &پایه->ref_آخرین);
	پیدا_کن_ofs_delta_children(obj->idx.جابه_جایی,
				&پایه->ofs_اولی, &پایه->ofs_آخرین);
	پایه->children_مانده = پایه->ref_آخرین - پایه->ref_اولی +
		پایه->ofs_آخرین - پایه->ofs_اولی + 2;
	أرجع پایه;
}

ساكن بنية پایه_داده *resolve_delta(بنية object_مدخل *delta_obj,
				       بنية پایه_داده *پایه)
{
	فراغ *delta_داده, *نتیجه_داده;
	بنية پایه_داده *نتیجه;
	اندازه_t نتیجه_اندازه;

	إذا (show_stat) {
		صحيح i = delta_obj - objects;
		صحيح j = پایه->obj - objects;
		obj_stat[i].delta_عمق = obj_stat[j].delta_عمق + 1;
		عمیقترین_delta_قفل();
		إذا (عمیقترین_delta < obj_stat[i].delta_عمق)
			عمیقترین_delta = obj_stat[i].delta_عمق;
		عمیقترین_delta_unlock();
		obj_stat[i].پایه_object_no = j;
	}
	delta_داده = get_داده_from_pack(delta_obj);
	أكد(پایه->داده);
	نتیجه_داده = patch_delta(پایه->داده, پایه->اندازه,
				  delta_داده, delta_obj->اندازه, &نتیجه_اندازه);
	حرر(delta_داده);
	إذا (!نتیجه_داده)
		bad_object(delta_obj->idx.جابه_جایی, _("failed to apply delta"));
	hash_object_file(the_hash_algo, نتیجه_داده, نتیجه_اندازه,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(نتیجه_داده, عدم, نتیجه_اندازه, delta_obj->real_type,
		    &delta_obj->idx.oid);

	نتیجه = make_پایه(delta_obj, پایه);
	نتیجه->داده = نتیجه_داده;
	نتیجه->اندازه = نتیجه_اندازه;

	counter_قفل();
	nr_resolved_deltas++;
	counter_unlock();

	أرجع نتیجه;
}

ساكن صحيح compare_ofs_delta_مدخل(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية ofs_delta_مدخل *delta_a = a;
	ثابت بنية ofs_delta_مدخل *delta_b = b;

	أرجع delta_a->جابه_جایی < delta_b->جابه_جایی ? -1 :
	       delta_a->جابه_جایی > delta_b->جابه_جایی ?  1 :
	       0;
}

ساكن صحيح compare_ref_delta_مدخل(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية ref_delta_مدخل *delta_a = a;
	ثابت بنية ref_delta_مدخل *delta_b = b;

	أرجع oidcmp(&delta_a->oid, &delta_b->oid);
}

ساكن فراغ *threaded_دومی_pass(فراغ *داده)
{
	إذا (داده)
		مجموعه_thread_داده(داده);
	لكل (;;) {
		بنية پایه_داده *والد = عدم;
		بنية object_مدخل *فرزند_obj = عدم;
		بنية پایه_داده *فرزند = عدم;

		counter_قفل();
		display_پیشرفت(پیشرفت, nr_resolved_deltas);
		counter_unlock();

		work_قفل();
		إذا (list_خالی(&work_سر)) {
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
			فرزند_obj = &objects[nr_dispatched++];
		} وإلا {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			والد = list_اولی_مدخل(&work_سر, بنية پایه_داده,
						  list);

			طالما (والد->ref_اولی <= والد->ref_آخرین) {
				صحيح جابه_جایی = ref_deltas[والد->ref_اولی++].obj_no;
				فرزند_obj = objects + جابه_جایی;
				إذا (فرزند_obj->real_type != OBJ_REF_DELTA) {
					فرزند_obj = عدم;
					استمر;
				}
				فرزند_obj->real_type = والد->obj->real_type;
				اكسر;
			}

			إذا (!فرزند_obj && والد->ofs_اولی <= والد->ofs_آخرین) {
				فرزند_obj = objects +
					ofs_deltas[والد->ofs_اولی++].obj_no;
				أكد(فرزند_obj->real_type == OBJ_OFS_DELTA);
				فرزند_obj->real_type = والد->obj->real_type;
			}

			إذا (والد->ref_اولی > والد->ref_آخرین &&
			    والد->ofs_اولی > والد->ofs_آخرین) {
				/*
				 * This parent has run out of children, so move
				 * it to done_head.
				 */
				list_del(&والد->list);
				list_جمع_کن(&والد->list, &انجامشده_سر);
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
			get_پایه_داده(والد);
			والد->retain_داده++;
		}
		work_unlock();

		إذا (فرزند_obj) {
			إذا (والد) {
				فرزند = resolve_delta(فرزند_obj, والد);
				إذا (!فرزند->children_مانده)
					FREE_AND_NULL(فرزند->داده);
			} وإلا{
				فرزند = make_پایه(فرزند_obj, عدم);
				إذا (فرزند->children_مانده) {
					/*
					 * Since this child has its own delta children,
					 * we will need this data in the future.
					 * Inflate now so that future iterations will
					 * have access to this object's data while
					 * outside the work mutex.
					 */
					فرزند->داده = get_داده_from_pack(فرزند_obj);
					فرزند->اندازه = فرزند_obj->اندازه;
				}
			}
		}

		work_قفل();
		إذا (والد)
			والد->retain_داده--;

		إذا (فرزند && فرزند->داده) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_جمع_کن(&فرزند->list, &work_سر);
			پایه_cache_استفاده_شده += فرزند->اندازه;
			prune_پایه_داده(عدم);
		} وإلا إذا (فرزند) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			بنية پایه_داده *p = والد;

			طالما (p) {
				بنية پایه_داده *بعدی_p;

				p->children_مانده--;
				إذا (p->children_مانده)
					اكسر;

				بعدی_p = p->پایه;
				free_پایه_داده(p);
				list_del(&p->list);
				حرر(p);

				p = بعدی_p;
			}
			FREE_AND_NULL(فرزند);
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
ساكن فراغ parse_pack_objects(بیعلامت حرف *hash)
{
	صحيح i, nr_delays = 0;
	بنية ofs_delta_مدخل *ofs_delta = ofs_deltas;
	بنية object_id ref_delta_oid;
	بنية stat st;
	بنية git_hash_ctx tmp_ctx;

	إذا (verbose)
		پیشرفت = شروع_پیشرفت(
				the_repository,
				پیشرفت_title ? پیشرفت_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	لكل (i = 0; i < nr_objects; i++) {
		بنية object_مدخل *obj = &objects[i];
		فراغ *داده = unpack_raw_مدخل(obj, &ofs_delta->جابه_جایی,
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
		} وإلا إذا (!داده) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} وإلا
			sha1_object(داده, عدم, obj->اندازه, obj->type,
				    &obj->idx.oid);
		حرر(داده);
		display_پیشرفت(پیشرفت, i+1);
	}
	objects[i].idx.جابه_جایی = consumed_بایتها;
	stop_پیشرفت(&پیشرفت);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &درون_داد_ctx);
	git_hash_final(hash, &tmp_ctx);
	إذا (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	إذا (fstat(درون_داد_fd, &st))
		die_errno(_("cannot fstat packfile"));
	إذا (S_ISREG(st.st_نما) &&
			lseek(درون_داد_fd, 0, SEEK_CUR) - درون_داد_len != st.st_اندازه)
		die(_("pack has junk at the end"));

	لكل (i = 0; i < nr_objects; i++) {
		بنية object_مدخل *obj = &objects[i];
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
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_مدخل);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_مدخل);

	إذا (verbose || show_resolving_پیشرفت)
		پیشرفت = شروع_پیشرفت(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	پایه_cache_حد = opts->delta_پایه_cache_حد * nr_نخها;
	إذا (nr_نخها > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		لكل (i = 0; i < nr_نخها; i++) {
			صحيح ret = pthread_create(&thread_داده[i].thread, عدم,
						 threaded_دومی_pass, thread_داده + i);
			إذا (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		لكل (i = 0; i < nr_نخها; i++)
			pthread_join(thread_داده[i].thread, عدم);
		cleanup_thread();
		أرجع;
	}
	threaded_دومی_pass(&nothread_داده);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
ساكن فراغ fix_unresolved_deltas(بنية hashfile *f);
ساكن فراغ conclude_pack(صحيح fix_thin_pack, ثابت حرف *curr_pack, بیعلامت حرف *pack_hash)
{
	إذا (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_پیشرفت(&پیشرفت);
		/* Flush remaining pack final hash. */
		flush();
		أرجع;
	}

	إذا (fix_thin_pack) {
		بنية hashfile *f;
		بیعلامت حرف read_hash[GIT_MAX_RAWSZ], دم_hash[GIT_MAX_RAWSZ];
		بنية strbuf پیام = STRBUF_INIT;
		صحيح nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		صحيح nr_objects_initial = nr_objects;
		إذا (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		memset(objects + nr_objects + 1, 0,
		       nr_unresolved * حجم(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&پیام, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_پیشرفت_پیام(&پیشرفت, پیام.buf);
		strbuf_release(&پیام);
		finalize_hashfile(f, دم_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_سرآیند_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_بایتها-the_hash_algo->rawsz);
		إذا (!hasheq(read_hash, دم_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	إذا (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

ساكن صحيح write_compressed(بنية hashfile *f, فراغ *درون, بیعلامت صحيح اندازه)
{
	git_zstream جریان;
	صحيح وضعیت;
	بیعلامت حرف outbuf[4096];
	بنية repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	git_deflate_init(&جریان, cfg->zlib_compression_level);
	جریان.بعدی_درون = درون;
	جریان.avail_درون = اندازه;

	افعل {
		جریان.بعدی_بیرون = outbuf;
		جریان.avail_بیرون = حجم(outbuf);
		وضعیت = git_deflate(&جریان, Z_پایان);
		hashwrite(f, outbuf, حجم(outbuf) - جریان.avail_بیرون);
	} طالما (وضعیت == Z_OK);

	إذا (وضعیت != Z_جریان_انتها)
		die(_("unable to deflate appended object (%d)"), وضعیت);
	اندازه = جریان.مجموع_بیرون;
	git_deflate_انتها(&جریان);
	أرجع اندازه;
}

ساكن بنية object_مدخل *append_obj_مقصد_pack(بنية hashfile *f,
			       ثابت بیعلامت حرف *sha1, فراغ *buf,
			       بیعلامت دراز اندازه, تعداد object_type type)
{
	بنية object_مدخل *obj = &objects[nr_objects++];
	بیعلامت حرف سرآیند[10];
	بیعلامت دراز s = اندازه;
	صحيح n = 0;
	بیعلامت حرف c = (type << 4) | (s & 15);
	s >>= 4;
	طالما (s) {
		سرآیند[n++] = c | 0x80;
		c = s & 0x7f;
		s >>= 7;
	}
	سرآیند[n++] = c;
	بررسی_چرخهای_32_begin(f);
	hashwrite(f, سرآیند, n);
	obj[0].اندازه = اندازه;
	obj[0].hdr_اندازه = n;
	obj[0].type = type;
	obj[0].real_type = type;
	obj[1].idx.جابه_جایی = obj[0].idx.جابه_جایی + n;
	obj[1].idx.جابه_جایی += write_compressed(f, buf, اندازه);
	obj[0].idx.بررسی_چرخهای_32 = بررسی_چرخهای_32_انتها(f);
	hashflush(f);
	oidread(&obj->idx.oid, sha1, the_repository->hash_algo);
	أرجع obj;
}

ساكن صحيح delta_pos_compare(ثابت فراغ *_a, ثابت فراغ *_b)
{
	بنية ref_delta_مدخل *a = *(بنية ref_delta_مدخل **)_a;
	بنية ref_delta_مدخل *b = *(بنية ref_delta_مدخل **)_b;
	أرجع a->obj_no - b->obj_no;
}

ساكن فراغ fix_unresolved_deltas(بنية hashfile *f)
{
	بنية ref_delta_مدخل **sorted_by_pos;
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
		بنية oid_array مقصد_fetch = OID_ARRAY_INIT;
		لكل (i = 0; i < nr_ref_deltas; i++) {
			بنية ref_delta_مدخل *d = sorted_by_pos[i];
			إذا (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, عدم,
							   OBJECT_INFO_FOR_PREFETCH))
				استمر;
			oid_array_append(&مقصد_fetch, &d->oid);
		}
		promisor_remote_get_مستقیم(the_repository,
					   مقصد_fetch.oid, مقصد_fetch.nr);
		oid_array_clear(&مقصد_fetch);
	}

	لكل (i = 0; i < nr_ref_deltas; i++) {
		بنية ref_delta_مدخل *d = sorted_by_pos[i];
		تعداد object_type type;
		فراغ *داده;
		اندازه_t اندازه;

		إذا (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			استمر;
		داده = odb_read_object(the_repository->objects, &d->oid,
				       &type, &اندازه);
		إذا (!داده)
			استمر;

		إذا (check_object_signature(the_repository, &d->oid, داده, اندازه,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_مقصد_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_مقصد_pack(f, d->oid.hash, داده, اندازه, type);
		حرر(داده);
		threaded_دومی_pass(عدم);

		display_پیشرفت(پیشرفت, nr_resolved_deltas);
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
	strbuf_جمع_کن(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	أرجع buf->buf;
}

ساكن فراغ write_special_file(ثابت حرف *suffix, ثابت حرف *پیام,
			       ثابت حرف *pack_name, ثابت بیعلامت حرف *hash,
			       ثابت حرف **report)
{
	بنية strbuf name_buf = STRBUF_INIT;
	ثابت حرف *filename;
	صحيح fd;
	صحيح پیام_len = طول_رشته(پیام);

	إذا (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	وإلا
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = امن_create_file_with_leading_directories(the_repository, filename);
	إذا (fd < 0) {
		إذا (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} وإلا {
		إذا (پیام_len > 0) {
			write_or_die(fd, پیام, پیام_len);
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
				بنية strbuf *name, بیعلامت حرف *hash,
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
		  ثابت حرف *keep_پیام, ثابت حرف *promisor_پیام,
		  بیعلامت حرف *hash)
{
	ثابت حرف *report = "pack";
	بنية strbuf pack_name = STRBUF_INIT;
	بنية strbuf شاخص_name = STRBUF_INIT;
	بنية strbuf rev_شاخص_name = STRBUF_INIT;

	إذا (!from_stdin) {
		close(درون_داد_fd);
	} وإلا {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		إذا (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	إذا (keep_پیام)
		write_special_file("keep", keep_پیام, final_pack_name, hash,
				   &report);
	إذا (promisor_پیام)
		write_special_file("promisor", promisor_پیام, final_pack_name,
				   hash, عدم);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	إذا (curr_rev_شاخص_name)
		rename_tmp_packfile(&final_rev_شاخص_name, curr_rev_شاخص_name,
				    &rev_شاخص_name, hash, "rev", 1);
	rename_tmp_packfile(&final_شاخص_name, curr_شاخص_name, &شاخص_name,
			    hash, "idx", 1);

	إذا (do_fsck_object && startup_info->have_repository) {
		بنية odb_منبع_files *files =
			odb_منبع_files_downcast(the_repository->objects->منبع);
		packfile_store_بارگیری_pack(files->dirs->packed, final_شاخص_name, 0);
	}

	إذا (!from_stdin) {
		چاپ_کن("%s\n", hash_مقصد_hex(hash));
	} وإلا {
		بنية strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_مقصد_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_درون_پر(1, درون_داد_میانگیر + درون_داد_جابه_جایی, درون_داد_len);
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
		nr_نخها = git_config_int(k, v, ctx->kvi);
		إذا (nr_نخها < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_نخها);
		إذا (!HAVE_نخها && nr_نخها != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_نخها = 1;
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
		opts->delta_پایه_cache_حد = git_config_ulong(k, v, ctx->kvi);
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
	idx1 = (((ثابت uint32_t *)((ثابت uint8_t *)p->شاخص_داده + p->بررسی_چرخهای_جابه_جایی))
		+ (اندازه_t)p->رقم_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->رقم_objects;

	لكل (i = 0; i < p->رقم_objects; i++) {
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
	بنية packed_git *p = جمع_کن_packed_git(the_repository, pack_name,
					      طول_رشته(pack_name), 1);

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
	بیعلامت دراز *chain_histogram = عدم;

	إذا (عمیقترین_delta)
		CALLOC_ARRAY(chain_histogram, عمیقترین_delta);

	لكل (i = 0; i < nr_objects; i++) {
		بنية object_مدخل *obj = &objects[i];

		إذا (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_عمق - 1]++;
		إذا (stat_only)
			استمر;
		چاپ_کن("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_مقصد_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->اندازه,
		       (uintmax_t)(obj[1].idx.جابه_جایی - obj->idx.جابه_جایی),
		       (uintmax_t)obj->idx.جابه_جایی);
		إذا (is_delta_type(obj->type)) {
			بنية object_مدخل *bobj = &objects[obj_stat[i].پایه_object_no];
			چاپ_کن(" %u %s", obj_stat[i].delta_عمق,
			       oid_مقصد_hex(&bobj->idx.oid));
		}
		حرف_چاپ_کن('\n');
	}

	إذا (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	لكل (i = 0; i < عمیقترین_delta; i++) {
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
	بنية فرزند_process cmd = فرزند_PROCESS_INIT;
	FILE *بیرون;
	بنية strbuf سطر = STRBUF_INIT;
	بنية oidset_iter iter;
	بنية object_id *oid;
	حرف *پایه_name = عدم;

	إذا (!oidset_اندازه(&outgoing_links))
		أرجع;

	oidset_iter_init(&outgoing_links, &iter);
	طالما ((oid = oidset_iter_بعدی(&iter))) {
		بنية odb_منبع_info منبع_info;
		بنية object_info info = {
			.منبع_infop = &منبع_info,
		};

		إذا (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			استمر;
		إذا (منبع_info.منبع->type == ODB_منبع_PACKED &&
		    منبع_info.u.packed.pack->pack_promisor)
			استمر;

		إذا (!cmd.آرگومانها.nr) {
			پایه_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_افزودن(&cmd.آرگومانها, "pack-objects");
			strvec_افزودن(&cmd.آرگومانها,
				    "--exclude-promisor-objects-best-effort");
			strvec_افزودن(&cmd.آرگومانها, پایه_name);
			cmd.git_cmd = 1;
			cmd.درون = -1;
			cmd.بیرون = -1;
			إذا (شروع_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		إذا (write_درون_پر(cmd.درون, oid_مقصد_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_درون_پر(cmd.درون, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	إذا (!cmd.آرگومانها.nr)
		أرجع;

	close(cmd.درون);

	بیرون = xfdopen(cmd.بیرون, "r");
	طالما (strbuf_getline_lf(&سطر, بیرون) != EOF) {
		بیعلامت حرف دودویی[GIT_MAX_RAWSZ];
		إذا (سطر.len != the_hash_algo->hexsz ||
		    !hex_مقصد_بایتها(دودویی, سطر.buf, سطر.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", عدم, دودویی, عدم);
	}

	fclose(بیرون);
	إذا (پایان_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&سطر);
	حرر(پایه_name);
}

صحيح cmd_شاخص_pack(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *پیشوند,
		   بنية repository *repo)
{
	صحيح i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_شاخص;
	ثابت حرف *curr_شاخص;
	حرف *curr_rev_شاخص = عدم;
	ثابت حرف *شاخص_name = عدم, *pack_name = عدم, *rev_شاخص_name = عدم;
	ثابت حرف *keep_پیام = عدم;
	ثابت حرف *promisor_پیام = عدم;
	بنية strbuf شاخص_name_buf = STRBUF_INIT;
	بنية strbuf rev_شاخص_name_buf = STRBUF_INIT;
	بنية pack_idx_مدخل **idx_objects;
	بنية pack_idx_option opts;
	بیعلامت حرف pack_hash[GIT_MAX_RAWSZ];
	بیعلامت foreign_nr = 1;	/* zero is a "good" value, assume bad */
	صحيح report_انتها_از_درون_داد = 0;
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
	إذا (پیشوند && chdir(پیشوند))
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
			} وإلا إذا (skip_مقصد_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_مجموعه_پیام_types(&fsck_options, arg);
			} وإلا إذا (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} وإلا إذا (skip_مقصد_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_مجموعه_پیام_types(&fsck_options, arg);
			} وإلا إذا (!strcmp(arg, "--verify")) {
				verify = 1;
			} وإلا إذا (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} وإلا إذا (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} وإلا إذا (skip_مقصد_optional_arg(arg, "--keep", &keep_پیام)) {
				; /* nothing to do */
			} وإلا إذا (skip_مقصد_optional_arg(arg, "--promisor", &promisor_پیام)) {
				رکورد_outgoing_links = 1;
			} وإلا إذا (starts_with(arg, "--threads=")) {
				حرف *انتها;
				nr_نخها = strtoul(arg+10, &انتها, 0);
				إذا (!arg[10] || *انتها || nr_نخها < 0)
					usage(شاخص_pack_usage);
				إذا (!HAVE_نخها && nr_نخها != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_نخها = 1;
				}
			} وإلا إذا (skip_پیشوند(arg, "--pack_header=", &arg)) {
				إذا (parse_pack_سرآیند_option(arg,
							     درون_داد_میانگیر,
							     &درون_داد_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} وإلا إذا (!strcmp(arg, "-v")) {
				verbose = 1;
			} وإلا إذا (!strcmp(arg, "--progress-title")) {
				إذا (پیشرفت_title || (i+1) >= argc)
					usage(شاخص_pack_usage);
				پیشرفت_title = argv[++i];
			} وإلا إذا (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_پیشرفت = 1;
			} وإلا إذا (!strcmp(arg, "--report-end-of-input")) {
				report_انتها_از_درون_داد = 1;
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
			} وإلا إذا (skip_پیشوند(arg, "--max-input-size=", &arg)) {
				max_درون_داد_اندازه = strtoumax(arg, عدم, 10);
			} وإلا إذا (skip_پیشوند(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				إذا (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_مجموعه_hash_algo(the_repository, hash_algo);
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
	إذا (promisor_پیام && pack_name)
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
		repo_مجموعه_hash_algo(the_repository, GIT_HASH_DEFAULT);

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

	إذا (HAVE_نخها && !nr_نخها) {
		nr_نخها = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		إذا (nr_نخها < 4)
			; /* too few cores to consider capping */
		وإلا إذا (nr_نخها < 6)
			nr_نخها = 3; /* historic cap */
		وإلا إذا (nr_نخها < 40)
			nr_نخها /= 2;
		وإلا
			nr_نخها = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_سرآیند();
	CALLOC_ARRAY(objects, st_جمع_کن(nr_objects, 1));
	إذا (show_stat)
		CALLOC_ARRAY(obj_stat, st_جمع_کن(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	إذا (report_انتها_از_درون_داد)
		write_درون_پر(2, "\0", 1);
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
		      keep_پیام, promisor_پیام,
		      pack_hash);
	وإلا
		close(درون_داد_fd);

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

		إذا (fsck_پایان(&fsck_options))
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
