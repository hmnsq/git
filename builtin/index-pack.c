#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "delta.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "pack.h"
#شامل "csum-file.h"
#شامل "blob.h"
#شامل "commit.h"
#شامل "tag.h"
#شامل "tree.h"
#شامل "progress.h"
#شامل "fsck.h"
#شامل "strbuf.h"
#شامل "thread-utils.h"
#شامل "packfile.h"
#شامل "pack-revindex.h"
#شامل "object-file.h"
#شامل "odb.h"
#شامل "odb/streaming.h"
#شامل "oid-array.h"
#شامل "oidset.h"
#شامل "path.h"
#شامل "replace-object.h"
#شامل "tree-walk.h"
#شامل "promisor-remote.h"
#شامل "run-command.h"
#شامل "setup.h"
#شامل "strvec.h"

ساکن ثابت حرف اشاریہ_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

ساخت object_اندراج {
	ساخت pack_idx_اندراج idx;
	سائز_t سائز;
	غیرعلامتی حرف hdr_سائز;
	علامتی حرف type;
	علامتی حرف real_type;
};

ساخت object_stat {
	غیرعلامتی delta_گہرائی;
	صحیح بنیاد_object_no;
};

ساخت بنیاد_ڈیٹا {
	/* Initialized by make_base(). */
	ساخت بنیاد_ڈیٹا *بنیاد;
	ساخت object_اندراج *obj;
	صحیح ref_پہلا, ref_آخری;
	صحیح ofs_پہلا, ofs_آخری;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	صحیح retain_ڈیٹا;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	صحیح children_بقایا;

	/* Not initialized by make_base(). */
	ساخت list_سر list;
	خالی *ڈیٹا;
	سائز_t سائز;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
ساکن LIST_سر(work_سر);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
ساکن LIST_سر(مکمل_سر);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
ساکن سائز_t بنیاد_cache_مستعمل;
ساکن سائز_t بنیاد_cache_حد_بندی;

ساخت thread_local_ڈیٹا {
	pthread_t thread;
	صحیح pack_fd;
};

/* Remember to update object flag allocation in object.h */
#تعریف_کرو FLAG_LINK (1u<<20)
#تعریف_کرو FLAG_CHECKED (1u<<21)

ساخت ofs_delta_اندراج {
	off_t انحراف;
	صحیح obj_no;
};

ساخت ref_delta_اندراج {
	ساخت object_id oid;
	صحیح obj_no;
};

ساکن ساخت object_اندراج *objects;
ساکن ساخت object_stat *obj_stat;
ساکن ساخت ofs_delta_اندراج *ofs_deltas;
ساکن ساخت ref_delta_اندراج *ref_deltas;
ساکن ساخت thread_local_ڈیٹا nothread_ڈیٹا;
ساکن صحیح nr_objects;
ساکن صحیح nr_ofs_deltas;
ساکن صحیح nr_ref_deltas;
ساکن صحیح ref_deltas_alloc;
ساکن صحیح nr_resolved_deltas;
ساکن صحیح nr_دھاگے;

ساکن صحیح from_stdin;
ساکن صحیح strict;
ساکن صحیح do_fsck_object;
ساکن ساخت fsck_options fsck_options;
ساکن صحیح verbose;
ساکن ثابت حرف *پیش_رفت_title;
ساکن صحیح show_resolving_پیش_رفت;
ساکن صحیح show_stat;
ساکن صحیح check_self_contained_and_connected;

ساکن ساخت پیش_رفت *پیش_رفت;

ساکن غیرعلامتی حرف داخلہ_بفر[DEFAULT_IO_بفر_سائز];
ساکن غیرعلامتی صحیح داخلہ_انحراف, داخلہ_len;
ساکن off_t consumed_بائٹیں;
ساکن off_t max_داخلہ_سائز;
ساکن غیرعلامتی گہرا_ترین_delta;
ساکن ساخت git_hash_ctx داخلہ_ctx;
ساکن uint32_t داخلہ_چکری_جانچ_32;
ساکن صحیح داخلہ_fd, output_fd;
ساکن ثابت حرف *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
ساکن ساخت oidset outgoing_links = OIDSET_INIT;
ساکن صحیح ریکارڈ_outgoing_links;

ساکن ساخت thread_local_ڈیٹا *thread_ڈیٹا;
ساکن صحیح nr_dispatched;
ساکن صحیح دھاگے_فعال;

ساکن pthread_mutex_t read_mutex;
#تعریف_کرو read_تالا()		تالا_mutex(&read_mutex)
#تعریف_کرو read_unlock()		unlock_mutex(&read_mutex)

ساکن pthread_mutex_t شمار_mutex;
#تعریف_کرو شمار_تالا()		تالا_mutex(&شمار_mutex)
#تعریف_کرو شمار_unlock()	unlock_mutex(&شمار_mutex)

ساکن pthread_mutex_t work_mutex;
#تعریف_کرو work_تالا()		تالا_mutex(&work_mutex)
#تعریف_کرو work_unlock()		unlock_mutex(&work_mutex)

ساکن pthread_mutex_t گہرا_ترین_delta_mutex;
#تعریف_کرو گہرا_ترین_delta_تالا()	تالا_mutex(&گہرا_ترین_delta_mutex)
#تعریف_کرو گہرا_ترین_delta_unlock()	unlock_mutex(&گہرا_ترین_delta_mutex)

ساکن pthread_کلید_t کلید;

ساکن اندرونی خالی تالا_mutex(pthread_mutex_t *mutex)
{
	اگر (دھاگے_فعال)
		pthread_mutex_تالا(mutex);
}

ساکن اندرونی خالی unlock_mutex(pthread_mutex_t *mutex)
{
	اگر (دھاگے_فعال)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
ساکن خالی init_thread(خالی)
{
	صحیح i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&شمار_mutex, تہی);
	pthread_mutex_init(&work_mutex, تہی);
	اگر (show_stat)
		pthread_mutex_init(&گہرا_ترین_delta_mutex, تہی);
	pthread_کلید_create(&کلید, تہی);
	CALLOC_ARRAY(thread_ڈیٹا, nr_دھاگے);
	برائے (i = 0; i < nr_دھاگے; i++) {
		thread_ڈیٹا[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	دھاگے_فعال = 1;
}

ساکن خالی cleanup_thread(خالی)
{
	صحیح i;
	اگر (!دھاگے_فعال)
		واپس;
	دھاگے_فعال = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&شمار_mutex);
	pthread_mutex_destroy(&work_mutex);
	اگر (show_stat)
		pthread_mutex_destroy(&گہرا_ترین_delta_mutex);
	برائے (i = 0; i < nr_دھاگے; i++)
		close(thread_ڈیٹا[i].pack_fd);
	pthread_کلید_delete(کلید);
	آزاد(thread_ڈیٹا);
}

ساکن صحیح mark_link(ساخت object *obj, شمار object_type type,
		     خالی *ڈیٹا غیر_مستعمل,
		     ساخت fsck_options *options غیر_مستعمل)
{
	اگر (!obj)
		واپس -1;

	اگر (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_مقصد_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	واپس 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
ساکن غیرعلامتی check_object(ساخت object *obj)
{
	اگر (!obj)
		واپس 0;

	اگر (!(obj->flags & FLAG_LINK))
		واپس 0;

	اگر (!(obj->flags & FLAG_CHECKED)) {
		سائز_t سائز;
		صحیح type = odb_read_object_info(the_repository->objects,
						&obj->oid, &سائز);
		اگر (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_مقصد_hex(&obj->oid));
		اگر (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_مقصد_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		واپس 1;
	}

	واپس 0;
}

ساکن غیرعلامتی check_objects(خالی)
{
	غیرعلامتی i, max, foreign_nr = 0;

	max = get_max_object_اشاریہ(the_repository);

	اگر (verbose)
		پیش_رفت = آغاز_delayed_پیش_رفت(the_repository,
						  _("Checking objects"), max);

	برائے (i = 0; i < max; i++) {
		foreign_nr += check_object(get_اشاریہ_دار_object(the_repository, i));
		display_پیش_رفت(پیش_رفت, i + 1);
	}

	stop_پیش_رفت(&پیش_رفت);
	واپس foreign_nr;
}


/* Discard current buffer used content. */
ساکن خالی flush(خالی)
{
	اگر (داخلہ_انحراف) {
		اگر (output_fd >= 0)
			write_or_die(output_fd, داخلہ_بفر, داخلہ_انحراف);
		git_hash_update(&داخلہ_ctx, داخلہ_بفر, داخلہ_انحراف);
		memmove(داخلہ_بفر, داخلہ_بفر + داخلہ_انحراف, داخلہ_len);
		داخلہ_انحراف = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
ساکن خالی *fill(صحیح min)
{
	اگر (min <= داخلہ_len)
		واپس داخلہ_بفر + داخلہ_انحراف;
	اگر (min > حجم(داخلہ_بفر))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	کرو {
		ssize_t ret = xread(داخلہ_fd, داخلہ_بفر + داخلہ_len,
				حجم(داخلہ_بفر) - داخلہ_len);
		اگر (ret <= 0) {
			اگر (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		داخلہ_len += ret;
		اگر (from_stdin)
			display_throughput(پیش_رفت, consumed_بائٹیں + داخلہ_len);
	} جب (داخلہ_len < min);
	واپس داخلہ_بفر;
}

ساکن خالی use(صحیح بائٹیں)
{
	اگر (بائٹیں > داخلہ_len)
		die(_("used more bytes than were available"));
	داخلہ_چکری_جانچ_32 = چکری_جانچ_32(داخلہ_چکری_جانچ_32, داخلہ_بفر + داخلہ_انحراف, بائٹیں);
	داخلہ_len -= بائٹیں;
	داخلہ_انحراف += بائٹیں;

	/* make sure off_t is sufficiently large not to wrap */
	اگر (signed_جوڑو_overflows(consumed_بائٹیں, بائٹیں))
		die(_("pack too large for current definition of off_t"));
	consumed_بائٹیں += بائٹیں;
	اگر (max_داخلہ_سائز && consumed_بائٹیں > max_داخلہ_سائز) {
		ساخت strbuf سائز_حد_بندی = STRBUF_INIT;
		strbuf_humanise_بائٹیں(&سائز_حد_بندی, max_داخلہ_سائز);
		die(_("pack exceeds maximum allowed size (%s)"),
		    سائز_حد_بندی.buf);
	}
}

ساکن ثابت حرف *open_pack_file(ثابت حرف *pack_name)
{
	اگر (from_stdin) {
		داخلہ_fd = 0;
		اگر (!pack_name) {
			ساخت strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, تہی);
		} ورنہ {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_ڈیٹا.pack_fd = output_fd;
	} ورنہ {
		داخلہ_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_ڈیٹا.pack_fd = داخلہ_fd;
	}
	git_hash_init(&داخلہ_ctx, the_hash_algo);
	واپس pack_name;
}

ساکن خالی parse_pack_سرخی(خالی)
{
	غیرعلامتی حرف *hdr = fill(حجم(ساخت pack_سرخی));

	/* Header consistency check */
	اگر (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	اگر (!pack_نسخہ_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(حجم(ساخت pack_سرخی));
}

__attribute__((format (لکھو, 2, 3)))
ساکن NORETURN خالی bad_object(off_t انحراف, ثابت حرف *format, ...)
{
	va_list params;
	حرف buf[1024];

	va_آغاز(params, format);
	vsnprintf(buf, حجم(buf), format, params);
	va_انتہا(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)انحراف, buf);
}

ساکن اندرونی ساخت thread_local_ڈیٹا *get_thread_ڈیٹا(خالی)
{
	اگر (HAVE_دھاگے) {
		اگر (دھاگے_فعال)
			واپس pthread_getspecific(کلید);
		أكد(!دھاگے_فعال &&
		       "This should only be reached when all threads are gone");
	}
	واپس &nothread_ڈیٹا;
}

ساکن خالی سیٹ_thread_ڈیٹا(ساخت thread_local_ڈیٹا *ڈیٹا)
{
	اگر (دھاگے_فعال)
		pthread_setspecific(کلید, ڈیٹا);
}

ساکن خالی free_بنیاد_ڈیٹا(ساخت بنیاد_ڈیٹا *c)
{
	اگر (c->ڈیٹا) {
		FREE_AND_NULL(c->ڈیٹا);
		بنیاد_cache_مستعمل -= c->سائز;
	}
}

ساکن خالی prune_بنیاد_ڈیٹا(ساخت بنیاد_ڈیٹا *retain)
{
	ساخت list_سر *pos;

	اگر (بنیاد_cache_مستعمل <= بنیاد_cache_حد_بندی)
		واپس;

	list_for_each_پچھلا(pos, &مکمل_سر) {
		ساخت بنیاد_ڈیٹا *b = list_اندراج(pos, ساخت بنیاد_ڈیٹا, list);
		اگر (b->retain_ڈیٹا || b == retain)
			جاری;
		اگر (b->ڈیٹا) {
			free_بنیاد_ڈیٹا(b);
			اگر (بنیاد_cache_مستعمل <= بنیاد_cache_حد_بندی)
				واپس;
		}
	}

	list_for_each_پچھلا(pos, &work_سر) {
		ساخت بنیاد_ڈیٹا *b = list_اندراج(pos, ساخت بنیاد_ڈیٹا, list);
		اگر (b->retain_ڈیٹا || b == retain)
			جاری;
		اگر (b->ڈیٹا) {
			free_بنیاد_ڈیٹا(b);
			اگر (بنیاد_cache_مستعمل <= بنیاد_cache_حد_بندی)
				واپس;
		}
	}
}

ساکن صحیح is_delta_type(شمار object_type type)
{
	واپس (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

ساکن خالی *unpack_اندراج_ڈیٹا(off_t انحراف, سائز_t سائز,
			       شمار object_type type, ساخت object_id *oid)
{
	ساکن حرف fixed_buf[8192];
	صحیح حالت_2;
	git_zstream دھارا;
	خالی *buf;
	ساخت git_hash_ctx c;
	حرف hdr[32];
	صحیح hdrlen;

	اگر (!is_delta_type(type)) {
		hdrlen = format_object_سرخی(hdr, حجم(hdr), type, سائز);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} ورنہ
		oid = تہی;
	اگر (type == OBJ_BLOB &&
	    سائز > repo_settings_get_big_file_حد(the_repository))
		buf = fixed_buf;
	ورنہ
		buf = xmallocz(سائز);

	memset(&دھارا, 0, حجم(دھارا));
	git_inflate_init(&دھارا);
	دھارا.اگلا_برآمد = buf;
	دھارا.avail_برآمد = buf == fixed_buf ? حجم(fixed_buf) : سائز;

	کرو {
		غیرعلامتی حرف *آخری_برآمد = دھارا.اگلا_برآمد;
		دھارا.اگلا_اندر = fill(1);
		دھارا.avail_اندر = داخلہ_len;
		حالت_2 = git_inflate(&دھارا, 0);
		use(داخلہ_len - دھارا.avail_اندر);
		اگر (oid)
			git_hash_update(&c, آخری_برآمد, دھارا.اگلا_برآمد - آخری_برآمد);
		اگر (buf == fixed_buf) {
			دھارا.اگلا_برآمد = buf;
			دھارا.avail_برآمد = حجم(fixed_buf);
		}
	} جب (حالت_2 == Z_OK);
	اگر (دھارا.کل_برآمد != سائز || حالت_2 != Z_دھارا_انتہا)
		bad_object(انحراف, _("inflate returned %d"), حالت_2);
	git_inflate_انتہا(&دھارا);
	اگر (oid)
		git_hash_final_oid(oid, &c);
	واپس buf == fixed_buf ? تہی : buf;
}

ساکن خالی *unpack_raw_اندراج(ساخت object_اندراج *obj,
			      off_t *ofs_انحراف,
			      ساخت object_id *ref_oid,
			      ساخت object_id *oid)
{
	غیرعلامتی حرف *p;
	سائز_t سائز, c;
	off_t بنیاد_انحراف;
	غیرعلامتی سرکاؤ;
	خالی *ڈیٹا;

	obj->idx.انحراف = consumed_بائٹیں;
	داخلہ_چکری_جانچ_32 = چکری_جانچ_32(0, تہی, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	سائز = (c & 15);
	سرکاؤ = 4;
	جب (c & 0x80) {
		اگر ((bitsizeof(سائز_t) - 7) < سرکاؤ)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		سائز += (c & 0x7f) << سرکاؤ;
		سرکاؤ += 7;
	}
	obj->سائز = سائز;

	انتخاب (obj->type) {
	حالت OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		توڑو;
	حالت OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		بنیاد_انحراف = c & 127;
		جب (c & 128) {
			بنیاد_انحراف += 1;
			اگر (!بنیاد_انحراف || MSB(بنیاد_انحراف, 7))
				bad_object(obj->idx.انحراف, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			بنیاد_انحراف = (بنیاد_انحراف << 7) + (c & 127);
		}
		*ofs_انحراف = obj->idx.انحراف - بنیاد_انحراف;
		اگر (*ofs_انحراف <= 0 || *ofs_انحراف >= obj->idx.انحراف)
			bad_object(obj->idx.انحراف, _("delta base offset is out of bound"));
		توڑو;
	حالت OBJ_COMMIT:
	حالت OBJ_درخت:
	حالت OBJ_BLOB:
	حالت OBJ_لیبل:
		توڑو;
	طےشدہ:
		bad_object(obj->idx.انحراف, _("unknown object type %d"), obj->type);
	}
	obj->hdr_سائز = consumed_بائٹیں - obj->idx.انحراف;

	ڈیٹا = unpack_اندراج_ڈیٹا(obj->idx.انحراف, obj->سائز, obj->type, oid);
	obj->idx.چکری_جانچ_32 = داخلہ_چکری_جانچ_32;
	واپس ڈیٹا;
}

ساکن خالی *unpack_ڈیٹا(ساخت object_اندراج *obj,
			 صحیح (*consume)(ثابت غیرعلامتی حرف *, غیرعلامتی طویل, خالی *),
			 خالی *cb_ڈیٹا)
{
	off_t from = obj[0].idx.انحراف + obj[0].hdr_سائز;
	off_t len = obj[1].idx.انحراف - from;
	غیرعلامتی حرف *ڈیٹا, *inbuf;
	git_zstream دھارا;
	صحیح حالت_2;

	ڈیٹا = xmallocz(consume ? 64*1024 : obj->سائز);
	inbuf = xmalloc((len < 64*1024) ? (صحیح)len : 64*1024);

	memset(&دھارا, 0, حجم(دھارا));
	git_inflate_init(&دھارا);
	دھارا.اگلا_برآمد = ڈیٹا;
	دھارا.avail_برآمد = consume ? 64*1024 : obj->سائز;

	کرو {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_ڈیٹا()->pack_fd, inbuf, n, from);
		اگر (n < 0)
			die_errno(_("cannot pread pack file"));
		اگر (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		دھارا.اگلا_اندر = inbuf;
		دھارا.avail_اندر = n;
		اگر (!consume)
			حالت_2 = git_inflate(&دھارا, 0);
		ورنہ {
			کرو {
				حالت_2 = git_inflate(&دھارا, 0);
				اگر (consume(ڈیٹا, دھارا.اگلا_برآمد - ڈیٹا, cb_ڈیٹا)) {
					آزاد(inbuf);
					آزاد(ڈیٹا);
					واپس تہی;
				}
				دھارا.اگلا_برآمد = ڈیٹا;
				دھارا.avail_برآمد = 64*1024;
			} جب (حالت_2 == Z_OK && دھارا.avail_اندر);
		}
	} جب (len && حالت_2 == Z_OK && !دھارا.avail_اندر);

	/* This has been inflated OK when first encountered, so... */
	اگر (حالت_2 != Z_دھارا_انتہا || دھارا.کل_برآمد != obj->سائز)
		die(_("serious inflate inconsistency"));

	git_inflate_انتہا(&دھارا);
	آزاد(inbuf);
	اگر (consume) {
		FREE_AND_NULL(ڈیٹا);
	}
	واپس ڈیٹا;
}

ساکن خالی *get_ڈیٹا_from_pack(ساخت object_اندراج *obj)
{
	واپس unpack_ڈیٹا(obj, تہی, تہی);
}

ساکن صحیح compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   شمار object_type type1,
				   شمار object_type type2)
{
	صحیح cmp = type1 - type2;
	اگر (cmp)
		واپس cmp;
	واپس offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

ساکن صحیح ڈھونڈو_ofs_delta(ثابت off_t انحراف)
{
	صحیح پہلا = 0, آخری = nr_ofs_deltas;

	جب (پہلا < آخری) {
		صحیح اگلا = پہلا + (آخری - پہلا) / 2;
		ساخت ofs_delta_اندراج *delta = &ofs_deltas[اگلا];
		صحیح cmp;

		cmp = compare_ofs_delta_bases(انحراف, delta->انحراف,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		اگر (!cmp)
			واپس اگلا;
		اگر (cmp < 0) {
			آخری = اگلا;
			جاری;
		}
		پہلا = اگلا+1;
	}
	واپس -پہلا-1;
}

ساکن خالی ڈھونڈو_ofs_delta_children(off_t انحراف,
				    صحیح *پہلا_اشاریہ, صحیح *آخری_اشاریہ)
{
	صحیح پہلا = ڈھونڈو_ofs_delta(انحراف);
	صحیح آخری = پہلا;
	صحیح انتہا = nr_ofs_deltas - 1;

	اگر (پہلا < 0) {
		*پہلا_اشاریہ = 0;
		*آخری_اشاریہ = -1;
		واپس;
	}
	جب (پہلا > 0 && ofs_deltas[پہلا - 1].انحراف == انحراف)
		--پہلا;
	جب (آخری < انتہا && ofs_deltas[آخری + 1].انحراف == انحراف)
		++آخری;
	*پہلا_اشاریہ = پہلا;
	*آخری_اشاریہ = آخری;
}

ساکن صحیح compare_ref_delta_bases(ثابت ساخت object_id *oid1,
				   ثابت ساخت object_id *oid2,
				   شمار object_type type1,
				   شمار object_type type2)
{
	صحیح cmp = type1 - type2;
	اگر (cmp)
		واپس cmp;
	واپس oidcmp(oid1, oid2);
}

ساکن صحیح ڈھونڈو_ref_delta(ثابت ساخت object_id *oid)
{
	صحیح پہلا = 0, آخری = nr_ref_deltas;

	جب (پہلا < آخری) {
		صحیح اگلا = پہلا + (آخری - پہلا) / 2;
		ساخت ref_delta_اندراج *delta = &ref_deltas[اگلا];
		صحیح cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		اگر (!cmp)
			واپس اگلا;
		اگر (cmp < 0) {
			آخری = اگلا;
			جاری;
		}
		پہلا = اگلا+1;
	}
	واپس -پہلا-1;
}

ساکن خالی ڈھونڈو_ref_delta_children(ثابت ساخت object_id *oid,
				    صحیح *پہلا_اشاریہ, صحیح *آخری_اشاریہ)
{
	صحیح پہلا = ڈھونڈو_ref_delta(oid);
	صحیح آخری = پہلا;
	صحیح انتہا = nr_ref_deltas - 1;

	اگر (پہلا < 0) {
		*پہلا_اشاریہ = 0;
		*آخری_اشاریہ = -1;
		واپس;
	}
	جب (پہلا > 0 && oideq(&ref_deltas[پہلا - 1].oid, oid))
		--پہلا;
	جب (آخری < انتہا && oideq(&ref_deltas[آخری + 1].oid, oid))
		++آخری;
	*پہلا_اشاریہ = پہلا;
	*آخری_اشاریہ = آخری;
}

ساخت compare_ڈیٹا {
	ساخت object_اندراج *اندراج_2;
	ساخت odb_دھارا *st;
	غیرعلامتی حرف *buf;
	غیرعلامتی طویل buf_سائز;
};

ساکن صحیح compare_objects(ثابت غیرعلامتی حرف *buf, غیرعلامتی طویل سائز,
			   خالی *cb_ڈیٹا)
{
	ساخت compare_ڈیٹا *ڈیٹا = cb_ڈیٹا;

	اگر (ڈیٹا->buf_سائز < سائز) {
		آزاد(ڈیٹا->buf);
		ڈیٹا->buf = xmalloc(سائز);
		ڈیٹا->buf_سائز = سائز;
	}

	جب (سائز) {
		ssize_t len = odb_دھارا_read(ڈیٹا->st, ڈیٹا->buf, سائز);
		اگر (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_مقصد_hex(&ڈیٹا->اندراج_2->idx.oid));
		اگر (len < 0)
			die(_("unable to read %s"),
			    oid_مقصد_hex(&ڈیٹا->اندراج_2->idx.oid));
		اگر (memcmp(buf, ڈیٹا->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_مقصد_hex(&ڈیٹا->اندراج_2->idx.oid));
		سائز -= len;
		buf += len;
	}
	واپس 0;
}

ساکن صحیح check_collison(ساخت object_اندراج *اندراج_2)
{
	ساخت compare_ڈیٹا ڈیٹا;

	اگر (اندراج_2->سائز <= repo_settings_get_big_file_حد(the_repository) ||
	    اندراج_2->type != OBJ_BLOB)
		واپس -1;

	memset(&ڈیٹا, 0, حجم(ڈیٹا));
	ڈیٹا.اندراج_2 = اندراج_2;
	ڈیٹا.st = odb_دھارا_from_object(the_repository->objects, &اندراج_2->idx.oid, تہی);
	اگر (!ڈیٹا.st)
		واپس -1;
	اگر (ڈیٹا.st->سائز != اندراج_2->سائز || ڈیٹا.st->type != اندراج_2->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_مقصد_hex(&اندراج_2->idx.oid));
	unpack_ڈیٹا(اندراج_2, compare_objects, &ڈیٹا);
	odb_دھارا_close(ڈیٹا.st);
	آزاد(ڈیٹا.buf);
	واپس 0;
}

ساکن خالی ریکارڈ_outgoing_link(ثابت ساخت object_id *oid)
{
	oidset_داخل_کرو(&outgoing_links, oid);
}

ساکن خالی maybe_ریکارڈ_name_اندراج(ثابت ساخت name_اندراج *اندراج_2)
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
	اگر (S_ISDIR(اندراج_2->منوال))
		ریکارڈ_outgoing_link(&اندراج_2->oid);
}

ساکن خالی do_ریکارڈ_outgoing_links(ساخت object *obj)
{
	اگر (obj->type == OBJ_درخت) {
		ساخت درخت *درخت = (ساخت درخت *)obj;
		ساخت درخت_desc desc;
		ساخت name_اندراج اندراج_2;
		اگر (init_درخت_desc_gently(&desc, &درخت->object.oid,
					  درخت->بفر, درخت->سائز, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			واپس;
		جب (درخت_اندراج_gently(&desc, &اندراج_2))
			maybe_ریکارڈ_name_اندراج(&اندراج_2);
	} ورنہ اگر (obj->type == OBJ_COMMIT) {
		ساخت commit *commit = (ساخت commit *) obj;
		ساخت commit_list *parents = commit->parents;

		ریکارڈ_outgoing_link(get_commit_درخت_oid(commit));
		برائے (; parents; parents = parents->اگلا)
			ریکارڈ_outgoing_link(&parents->شے->object.oid);
	} ورنہ اگر (obj->type == OBJ_لیبل) {
		ساخت لیبل *لیبل = (ساخت لیبل *) obj;
		ریکارڈ_outgoing_link(get_tagged_oid(لیبل));
	}
}

ساکن خالی sha1_object(ثابت خالی *ڈیٹا, ساخت object_اندراج *obj_اندراج,
			غیرعلامتی طویل سائز, شمار object_type type,
			ثابت ساخت object_id *oid)
{
	خالی *new_ڈیٹا = تہی;
	صحیح collision_test_needed = 0;

	أكد(ڈیٹا || obj_اندراج);

	اگر (startup_info->have_repository) {
		read_تالا();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	اگر (collision_test_needed && !ڈیٹا) {
		read_تالا();
		اگر (!check_collison(obj_اندراج))
			collision_test_needed = 0;
		read_unlock();
	}
	اگر (collision_test_needed) {
		خالی *has_ڈیٹا;
		شمار object_type has_type;
		سائز_t has_سائز;
		read_تالا();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_سائز);
		اگر (has_type < 0)
			die(_("cannot read existing object info %s"), oid_مقصد_hex(oid));
		اگر (has_type != type || has_سائز != سائز)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_مقصد_hex(oid));
		has_ڈیٹا = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_سائز);
		read_unlock();
		اگر (!ڈیٹا)
			ڈیٹا = new_ڈیٹا = get_ڈیٹا_from_pack(obj_اندراج);
		اگر (!has_ڈیٹا)
			die(_("cannot read existing object %s"), oid_مقصد_hex(oid));
		اگر (سائز != has_سائز || type != has_type ||
		    memcmp(ڈیٹا, has_ڈیٹا, سائز) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_مقصد_hex(oid));
		آزاد(has_ڈیٹا);
	}

	اگر (strict || do_fsck_object || ریکارڈ_outgoing_links) {
		read_تالا();
		اگر (type == OBJ_BLOB) {
			ساخت blob *blob = lookup_blob(the_repository, oid);
			اگر (blob)
				blob->object.flags |= FLAG_CHECKED;
			ورنہ
				die(_("invalid blob object %s"), oid_مقصد_hex(oid));
			اگر (do_fsck_object &&
			    fsck_object(&blob->object, (خالی *)ڈیٹا, سائز, &fsck_options))
				die(_("fsck error in packed object"));
		} ورنہ {
			ساخت object *obj;
			صحیح eaten;
			خالی *buf = (خالی *) ڈیٹا;

			أكد(ڈیٹا && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_بفر(the_repository, oid, type,
						  سائز, buf,
						  &eaten);
			اگر (!obj)
				die(_("invalid %s"), type_name(type));
			اگر (do_fsck_object &&
			    fsck_object(obj, buf, سائز, &fsck_options))
				die(_("fsck error in packed object"));
			اگر (strict && fsck_walk(obj, تہی, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_مقصد_hex(&obj->oid));
			اگر (ریکارڈ_outgoing_links)
				do_ریکارڈ_outgoing_links(obj);

			اگر (obj->type == OBJ_درخت) {
				ساخت درخت *شے = (ساخت درخت *) obj;
				شے->بفر = تہی;
				obj->parsed = 0;
			}
			اگر (obj->type == OBJ_COMMIT) {
				ساخت commit *commit = (ساخت commit *) obj;
				اگر (detach_commit_بفر(commit, تہی) != ڈیٹا)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	آزاد(new_ڈیٹا);
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
ساکن خالی *get_بنیاد_ڈیٹا(ساخت بنیاد_ڈیٹا *c)
{
	اگر (!c->ڈیٹا) {
		ساخت object_اندراج *obj = c->obj;
		ساخت بنیاد_ڈیٹا **delta = تہی;
		صحیح delta_nr = 0, delta_alloc = 0;

		جب (is_delta_type(c->obj->type) && !c->ڈیٹا) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->بنیاد;
		}
		اگر (!delta_nr) {
			c->ڈیٹا = get_ڈیٹا_from_pack(obj);
			c->سائز = obj->سائز;
			بنیاد_cache_مستعمل += c->سائز;
			prune_بنیاد_ڈیٹا(c);
		}
		برائے (; delta_nr > 0; delta_nr--) {
			خالی *بنیاد, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			بنیاد = get_بنیاد_ڈیٹا(c->بنیاد);
			raw = get_ڈیٹا_from_pack(obj);
			c->ڈیٹا = patch_delta(
				بنیاد, c->بنیاد->سائز,
				raw, obj->سائز,
				&c->سائز);
			آزاد(raw);
			اگر (!c->ڈیٹا)
				bad_object(obj->idx.انحراف, _("failed to apply delta"));
			بنیاد_cache_مستعمل += c->سائز;
			prune_بنیاد_ڈیٹا(c);
		}
		آزاد(delta);
	}
	واپس c->ڈیٹا;
}

ساکن ساخت بنیاد_ڈیٹا *make_بنیاد(ساخت object_اندراج *obj,
				   ساخت بنیاد_ڈیٹا *والد)
{
	ساخت بنیاد_ڈیٹا *بنیاد = xcalloc(1, حجم(ساخت بنیاد_ڈیٹا));
	بنیاد->بنیاد = والد;
	بنیاد->obj = obj;
	ڈھونڈو_ref_delta_children(&obj->idx.oid,
				&بنیاد->ref_پہلا, &بنیاد->ref_آخری);
	ڈھونڈو_ofs_delta_children(obj->idx.انحراف,
				&بنیاد->ofs_پہلا, &بنیاد->ofs_آخری);
	بنیاد->children_بقایا = بنیاد->ref_آخری - بنیاد->ref_پہلا +
		بنیاد->ofs_آخری - بنیاد->ofs_پہلا + 2;
	واپس بنیاد;
}

ساکن ساخت بنیاد_ڈیٹا *resolve_delta(ساخت object_اندراج *delta_obj,
				       ساخت بنیاد_ڈیٹا *بنیاد)
{
	خالی *delta_ڈیٹا, *نتیجہ_ڈیٹا;
	ساخت بنیاد_ڈیٹا *نتیجہ;
	سائز_t نتیجہ_سائز;

	اگر (show_stat) {
		صحیح i = delta_obj - objects;
		صحیح j = بنیاد->obj - objects;
		obj_stat[i].delta_گہرائی = obj_stat[j].delta_گہرائی + 1;
		گہرا_ترین_delta_تالا();
		اگر (گہرا_ترین_delta < obj_stat[i].delta_گہرائی)
			گہرا_ترین_delta = obj_stat[i].delta_گہرائی;
		گہرا_ترین_delta_unlock();
		obj_stat[i].بنیاد_object_no = j;
	}
	delta_ڈیٹا = get_ڈیٹا_from_pack(delta_obj);
	أكد(بنیاد->ڈیٹا);
	نتیجہ_ڈیٹا = patch_delta(بنیاد->ڈیٹا, بنیاد->سائز,
				  delta_ڈیٹا, delta_obj->سائز, &نتیجہ_سائز);
	آزاد(delta_ڈیٹا);
	اگر (!نتیجہ_ڈیٹا)
		bad_object(delta_obj->idx.انحراف, _("failed to apply delta"));
	hash_object_file(the_hash_algo, نتیجہ_ڈیٹا, نتیجہ_سائز,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(نتیجہ_ڈیٹا, تہی, نتیجہ_سائز, delta_obj->real_type,
		    &delta_obj->idx.oid);

	نتیجہ = make_بنیاد(delta_obj, بنیاد);
	نتیجہ->ڈیٹا = نتیجہ_ڈیٹا;
	نتیجہ->سائز = نتیجہ_سائز;

	شمار_تالا();
	nr_resolved_deltas++;
	شمار_unlock();

	واپس نتیجہ;
}

ساکن صحیح compare_ofs_delta_اندراج(ثابت خالی *a, ثابت خالی *b)
{
	ثابت ساخت ofs_delta_اندراج *delta_a = a;
	ثابت ساخت ofs_delta_اندراج *delta_b = b;

	واپس delta_a->انحراف < delta_b->انحراف ? -1 :
	       delta_a->انحراف > delta_b->انحراف ?  1 :
	       0;
}

ساکن صحیح compare_ref_delta_اندراج(ثابت خالی *a, ثابت خالی *b)
{
	ثابت ساخت ref_delta_اندراج *delta_a = a;
	ثابت ساخت ref_delta_اندراج *delta_b = b;

	واپس oidcmp(&delta_a->oid, &delta_b->oid);
}

ساکن خالی *threaded_دوسرا_pass(خالی *ڈیٹا)
{
	اگر (ڈیٹا)
		سیٹ_thread_ڈیٹا(ڈیٹا);
	برائے (;;) {
		ساخت بنیاد_ڈیٹا *والد = تہی;
		ساخت object_اندراج *بچہ_obj = تہی;
		ساخت بنیاد_ڈیٹا *بچہ = تہی;

		شمار_تالا();
		display_پیش_رفت(پیش_رفت, nr_resolved_deltas);
		شمار_unlock();

		work_تالا();
		اگر (list_خالی_خانے(&work_سر)) {
			/*
			 * Take an object from the object array.
			 */
			جب (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			اگر (nr_dispatched >= nr_objects) {
				work_unlock();
				توڑو;
			}
			بچہ_obj = &objects[nr_dispatched++];
		} ورنہ {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			والد = list_پہلا_اندراج(&work_سر, ساخت بنیاد_ڈیٹا,
						  list);

			جب (والد->ref_پہلا <= والد->ref_آخری) {
				صحیح انحراف = ref_deltas[والد->ref_پہلا++].obj_no;
				بچہ_obj = objects + انحراف;
				اگر (بچہ_obj->real_type != OBJ_REF_DELTA) {
					بچہ_obj = تہی;
					جاری;
				}
				بچہ_obj->real_type = والد->obj->real_type;
				توڑو;
			}

			اگر (!بچہ_obj && والد->ofs_پہلا <= والد->ofs_آخری) {
				بچہ_obj = objects +
					ofs_deltas[والد->ofs_پہلا++].obj_no;
				أكد(بچہ_obj->real_type == OBJ_OFS_DELTA);
				بچہ_obj->real_type = والد->obj->real_type;
			}

			اگر (والد->ref_پہلا > والد->ref_آخری &&
			    والد->ofs_پہلا > والد->ofs_آخری) {
				/*
				 * This parent has run out of children, so move
				 * it to done_head.
				 */
				list_del(&والد->list);
				list_جوڑو(&والد->list, &مکمل_سر);
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
			get_بنیاد_ڈیٹا(والد);
			والد->retain_ڈیٹا++;
		}
		work_unlock();

		اگر (بچہ_obj) {
			اگر (والد) {
				بچہ = resolve_delta(بچہ_obj, والد);
				اگر (!بچہ->children_بقایا)
					FREE_AND_NULL(بچہ->ڈیٹا);
			} ورنہ{
				بچہ = make_بنیاد(بچہ_obj, تہی);
				اگر (بچہ->children_بقایا) {
					/*
					 * Since this child has its own delta children,
					 * we will need this data in the future.
					 * Inflate now so that future iterations will
					 * have access to this object's data while
					 * outside the work mutex.
					 */
					بچہ->ڈیٹا = get_ڈیٹا_from_pack(بچہ_obj);
					بچہ->سائز = بچہ_obj->سائز;
				}
			}
		}

		work_تالا();
		اگر (والد)
			والد->retain_ڈیٹا--;

		اگر (بچہ && بچہ->ڈیٹا) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_جوڑو(&بچہ->list, &work_سر);
			بنیاد_cache_مستعمل += بچہ->سائز;
			prune_بنیاد_ڈیٹا(تہی);
		} ورنہ اگر (بچہ) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			ساخت بنیاد_ڈیٹا *p = والد;

			جب (p) {
				ساخت بنیاد_ڈیٹا *اگلا_p;

				p->children_بقایا--;
				اگر (p->children_بقایا)
					توڑو;

				اگلا_p = p->بنیاد;
				free_بنیاد_ڈیٹا(p);
				list_del(&p->list);
				آزاد(p);

				p = اگلا_p;
			}
			FREE_AND_NULL(بچہ);
		}
		work_unlock();
	}
	واپس تہی;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
ساکن خالی parse_pack_objects(غیرعلامتی حرف *hash)
{
	صحیح i, nr_delays = 0;
	ساخت ofs_delta_اندراج *ofs_delta = ofs_deltas;
	ساخت object_id ref_delta_oid;
	ساخت stat st;
	ساخت git_hash_ctx tmp_ctx;

	اگر (verbose)
		پیش_رفت = آغاز_پیش_رفت(
				the_repository,
				پیش_رفت_title ? پیش_رفت_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	برائے (i = 0; i < nr_objects; i++) {
		ساخت object_اندراج *obj = &objects[i];
		خالی *ڈیٹا = unpack_raw_اندراج(obj, &ofs_delta->انحراف,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		اگر (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} ورنہ اگر (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} ورنہ اگر (!ڈیٹا) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} ورنہ
			sha1_object(ڈیٹا, تہی, obj->سائز, obj->type,
				    &obj->idx.oid);
		آزاد(ڈیٹا);
		display_پیش_رفت(پیش_رفت, i+1);
	}
	objects[i].idx.انحراف = consumed_بائٹیں;
	stop_پیش_رفت(&پیش_رفت);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &داخلہ_ctx);
	git_hash_final(hash, &tmp_ctx);
	اگر (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	اگر (fstat(داخلہ_fd, &st))
		die_errno(_("cannot fstat packfile"));
	اگر (S_ISREG(st.st_منوال) &&
			lseek(داخلہ_fd, 0, SEEK_CUR) - داخلہ_len != st.st_سائز)
		die(_("pack has junk at the end"));

	برائے (i = 0; i < nr_objects; i++) {
		ساخت object_اندراج *obj = &objects[i];
		اگر (obj->real_type != OBJ_BAD)
			جاری;
		obj->real_type = obj->type;
		sha1_object(تہی, obj, obj->سائز, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	اگر (nr_delays)
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
ساکن خالی resolve_deltas(ساخت pack_idx_option *opts)
{
	صحیح i;

	اگر (!nr_ofs_deltas && !nr_ref_deltas)
		واپس;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_اندراج);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_اندراج);

	اگر (verbose || show_resolving_پیش_رفت)
		پیش_رفت = آغاز_پیش_رفت(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	بنیاد_cache_حد_بندی = opts->delta_بنیاد_cache_حد_بندی * nr_دھاگے;
	اگر (nr_دھاگے > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		برائے (i = 0; i < nr_دھاگے; i++) {
			صحیح ret = pthread_create(&thread_ڈیٹا[i].thread, تہی,
						 threaded_دوسرا_pass, thread_ڈیٹا + i);
			اگر (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		برائے (i = 0; i < nr_دھاگے; i++)
			pthread_join(thread_ڈیٹا[i].thread, تہی);
		cleanup_thread();
		واپس;
	}
	threaded_دوسرا_pass(&nothread_ڈیٹا);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
ساکن خالی fix_unresolved_deltas(ساخت hashfile *f);
ساکن خالی conclude_pack(صحیح fix_thin_pack, ثابت حرف *curr_pack, غیرعلامتی حرف *pack_hash)
{
	اگر (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_پیش_رفت(&پیش_رفت);
		/* Flush remaining pack final hash. */
		flush();
		واپس;
	}

	اگر (fix_thin_pack) {
		ساخت hashfile *f;
		غیرعلامتی حرف read_hash[GIT_MAX_RAWSZ], دم_hash[GIT_MAX_RAWSZ];
		ساخت strbuf پیغام = STRBUF_INIT;
		صحیح nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		صحیح nr_objects_initial = nr_objects;
		اگر (nr_unresolved <= 0)
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
		stop_پیش_رفت_پیغام(&پیش_رفت, پیغام.buf);
		strbuf_release(&پیغام);
		finalize_hashfile(f, دم_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_سرخی_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_بائٹیں-the_hash_algo->rawsz);
		اگر (!hasheq(read_hash, دم_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	اگر (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

ساکن صحیح write_compressed(ساخت hashfile *f, خالی *اندر, غیرعلامتی صحیح سائز)
{
	git_zstream دھارا;
	صحیح حالت_2;
	غیرعلامتی حرف outbuf[4096];
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	git_deflate_init(&دھارا, cfg->zlib_compression_level);
	دھارا.اگلا_اندر = اندر;
	دھارا.avail_اندر = سائز;

	کرو {
		دھارا.اگلا_برآمد = outbuf;
		دھارا.avail_برآمد = حجم(outbuf);
		حالت_2 = git_deflate(&دھارا, Z_اختتام);
		hashwrite(f, outbuf, حجم(outbuf) - دھارا.avail_برآمد);
	} جب (حالت_2 == Z_OK);

	اگر (حالت_2 != Z_دھارا_انتہا)
		die(_("unable to deflate appended object (%d)"), حالت_2);
	سائز = دھارا.کل_برآمد;
	git_deflate_انتہا(&دھارا);
	واپس سائز;
}

ساکن ساخت object_اندراج *append_obj_مقصد_pack(ساخت hashfile *f,
			       ثابت غیرعلامتی حرف *sha1, خالی *buf,
			       غیرعلامتی طویل سائز, شمار object_type type)
{
	ساخت object_اندراج *obj = &objects[nr_objects++];
	غیرعلامتی حرف سرخی[10];
	غیرعلامتی طویل s = سائز;
	صحیح n = 0;
	غیرعلامتی حرف c = (type << 4) | (s & 15);
	s >>= 4;
	جب (s) {
		سرخی[n++] = c | 0x80;
		c = s & 0x7f;
		s >>= 7;
	}
	سرخی[n++] = c;
	چکری_جانچ_32_begin(f);
	hashwrite(f, سرخی, n);
	obj[0].سائز = سائز;
	obj[0].hdr_سائز = n;
	obj[0].type = type;
	obj[0].real_type = type;
	obj[1].idx.انحراف = obj[0].idx.انحراف + n;
	obj[1].idx.انحراف += write_compressed(f, buf, سائز);
	obj[0].idx.چکری_جانچ_32 = چکری_جانچ_32_انتہا(f);
	hashflush(f);
	oidread(&obj->idx.oid, sha1, the_repository->hash_algo);
	واپس obj;
}

ساکن صحیح delta_pos_compare(ثابت خالی *_a, ثابت خالی *_b)
{
	ساخت ref_delta_اندراج *a = *(ساخت ref_delta_اندراج **)_a;
	ساخت ref_delta_اندراج *b = *(ساخت ref_delta_اندراج **)_b;
	واپس a->obj_no - b->obj_no;
}

ساکن خالی fix_unresolved_deltas(ساخت hashfile *f)
{
	ساخت ref_delta_اندراج **sorted_by_pos;
	صحیح i;

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
	برائے (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	اگر (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		ساخت oid_array مقصد_fetch = OID_ARRAY_INIT;
		برائے (i = 0; i < nr_ref_deltas; i++) {
			ساخت ref_delta_اندراج *d = sorted_by_pos[i];
			اگر (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, تہی,
							   OBJECT_INFO_FOR_PREFETCH))
				جاری;
			oid_array_append(&مقصد_fetch, &d->oid);
		}
		promisor_remote_get_براہ_راست(the_repository,
					   مقصد_fetch.oid, مقصد_fetch.nr);
		oid_array_clear(&مقصد_fetch);
	}

	برائے (i = 0; i < nr_ref_deltas; i++) {
		ساخت ref_delta_اندراج *d = sorted_by_pos[i];
		شمار object_type type;
		خالی *ڈیٹا;
		سائز_t سائز;

		اگر (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			جاری;
		ڈیٹا = odb_read_object(the_repository->objects, &d->oid,
				       &type, &سائز);
		اگر (!ڈیٹا)
			جاری;

		اگر (check_object_signature(the_repository, &d->oid, ڈیٹا, سائز,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_مقصد_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_مقصد_pack(f, d->oid.hash, ڈیٹا, سائز, type);
		آزاد(ڈیٹا);
		threaded_دوسرا_pass(تہی);

		display_پیش_رفت(پیش_رفت, nr_resolved_deltas);
	}
	آزاد(sorted_by_pos);
}

ساکن ثابت حرف *derive_filename(ثابت حرف *pack_name, ثابت حرف *strip,
				   ثابت حرف *suffix, ساخت strbuf *buf)
{
	سائز_t len;
	اگر (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_جوڑو(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	واپس buf->buf;
}

ساکن خالی write_special_file(ثابت حرف *suffix, ثابت حرف *پیغام,
			       ثابت حرف *pack_name, ثابت غیرعلامتی حرف *hash,
			       ثابت حرف **report)
{
	ساخت strbuf name_buf = STRBUF_INIT;
	ثابت حرف *filename;
	صحیح fd;
	صحیح پیغام_len = لڑی_لمبائی(پیغام);

	اگر (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	ورنہ
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = محفوظ_create_file_with_leading_directories(the_repository, filename);
	اگر (fd < 0) {
		اگر (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} ورنہ {
		اگر (پیغام_len > 0) {
			write_or_die(fd, پیغام, پیغام_len);
			write_or_die(fd, "\n", 1);
		}
		اگر (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		اگر (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

ساکن خالی rename_tmp_packfile(ثابت حرف **final_name,
				ثابت حرف *curr_name,
				ساخت strbuf *name, غیرعلامتی حرف *hash,
				ثابت حرف *ext, صحیح make_read_only_if_same)
{
	اگر (!*final_name || strcmp(*final_name, curr_name)) {
		اگر (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		اگر (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} ورنہ اگر (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

ساکن خالی final(ثابت حرف *final_pack_name, ثابت حرف *curr_pack_name,
		  ثابت حرف *final_اشاریہ_name, ثابت حرف *curr_اشاریہ_name,
		  ثابت حرف *final_rev_اشاریہ_name, ثابت حرف *curr_rev_اشاریہ_name,
		  ثابت حرف *keep_پیغام, ثابت حرف *promisor_پیغام,
		  غیرعلامتی حرف *hash)
{
	ثابت حرف *report = "pack";
	ساخت strbuf pack_name = STRBUF_INIT;
	ساخت strbuf اشاریہ_name = STRBUF_INIT;
	ساخت strbuf rev_اشاریہ_name = STRBUF_INIT;

	اگر (!from_stdin) {
		close(داخلہ_fd);
	} ورنہ {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		اگر (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	اگر (keep_پیغام)
		write_special_file("keep", keep_پیغام, final_pack_name, hash,
				   &report);
	اگر (promisor_پیغام)
		write_special_file("promisor", promisor_پیغام, final_pack_name,
				   hash, تہی);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	اگر (curr_rev_اشاریہ_name)
		rename_tmp_packfile(&final_rev_اشاریہ_name, curr_rev_اشاریہ_name,
				    &rev_اشاریہ_name, hash, "rev", 1);
	rename_tmp_packfile(&final_اشاریہ_name, curr_اشاریہ_name, &اشاریہ_name,
			    hash, "idx", 1);

	اگر (do_fsck_object && startup_info->have_repository) {
		ساخت odb_ماخذ_files *files =
			odb_ماخذ_files_downcast(the_repository->objects->ماخذ);
		packfile_store_بھرو_pack(files->dirs->packed, final_اشاریہ_name, 0);
	}

	اگر (!from_stdin) {
		لکھو("%s\n", hash_مقصد_hex(hash));
	} ورنہ {
		ساخت strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_مقصد_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_اندر_بھرا(1, داخلہ_بفر + داخلہ_انحراف, داخلہ_len);
	}

	strbuf_release(&rev_اشاریہ_name);
	strbuf_release(&اشاریہ_name);
	strbuf_release(&pack_name);
}

ساکن صحیح git_اشاریہ_pack_config(ثابت حرف *k, ثابت حرف *v,
				 ثابت ساخت config_context *ctx, خالی *cb)
{
	ساخت pack_idx_option *opts = cb;

	اگر (!strcmp(k, "pack.indexversion")) {
		opts->نسخہ = git_config_int(k, v, ctx->kvi);
		اگر (opts->نسخہ > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->نسخہ);
		واپس 0;
	}
	اگر (!strcmp(k, "pack.threads")) {
		nr_دھاگے = git_config_int(k, v, ctx->kvi);
		اگر (nr_دھاگے < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_دھاگے);
		اگر (!HAVE_دھاگے && nr_دھاگے != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_دھاگے = 1;
		}
		واپس 0;
	}
	اگر (!strcmp(k, "pack.writereverseindex")) {
		اگر (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		ورنہ
			opts->flags &= ~WRITE_REV;
	}
	اگر (!strcmp(k, "core.deltabasecachelimit")) {
		opts->delta_بنیاد_cache_حد_بندی = git_config_ulong(k, v, ctx->kvi);
		واپس 0;
	}
	واپس git_default_config(k, v, ctx, cb);
}

ساکن صحیح cmp_uint32(ثابت خالی *a_, ثابت خالی *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	واپس (a < b) ? -1 : (a != b);
}

ساکن خالی read_v2_anomalous_offsets(ساخت packed_git *p,
				      ساخت pack_idx_option *opts)
{
	ثابت uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((ثابت uint32_t *)((ثابت uint8_t *)p->اشاریہ_ڈیٹا + p->چکری_جانچ_انحراف))
		+ (سائز_t)p->عدد_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->عدد_objects;

	برائے (i = 0; i < p->عدد_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		اگر (!(off & 0x80000000))
			جاری;
		off = off & 0x7fffffff;
		check_pack_اشاریہ_ptr(p, &idx2[off * 2]);
		اگر (idx2[off * 2])
			جاری;
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

ساکن خالی read_idx_option(ساخت pack_idx_option *opts, ثابت حرف *pack_name)
{
	ساخت packed_git *p = جوڑو_packed_git(the_repository, pack_name,
					      لڑی_لمبائی(pack_name), 1);

	اگر (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	اگر (open_pack_اشاریہ(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->نسخہ = p->اشاریہ_نسخہ;

	اگر (opts->نسخہ == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_اشاریہ(p);
	آزاد(p);
}

ساکن خالی show_pack_info(صحیح stat_only)
{
	صحیح i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	غیرعلامتی طویل *chain_histogram = تہی;

	اگر (گہرا_ترین_delta)
		CALLOC_ARRAY(chain_histogram, گہرا_ترین_delta);

	برائے (i = 0; i < nr_objects; i++) {
		ساخت object_اندراج *obj = &objects[i];

		اگر (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_گہرائی - 1]++;
		اگر (stat_only)
			جاری;
		لکھو("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_مقصد_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->سائز,
		       (uintmax_t)(obj[1].idx.انحراف - obj->idx.انحراف),
		       (uintmax_t)obj->idx.انحراف);
		اگر (is_delta_type(obj->type)) {
			ساخت object_اندراج *bobj = &objects[obj_stat[i].بنیاد_object_no];
			لکھو(" %u %s", obj_stat[i].delta_گہرائی,
			       oid_مقصد_hex(&bobj->idx.oid));
		}
		حرف_لکھو('\n');
	}

	اگر (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	برائے (i = 0; i < گہرا_ترین_delta; i++) {
		اگر (!chain_histogram[i])
			جاری;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	آزاد(chain_histogram);
}

ساکن خالی repack_local_links(خالی)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;
	FILE *برآمد;
	ساخت strbuf سطر = STRBUF_INIT;
	ساخت oidset_iter iter;
	ساخت object_id *oid;
	حرف *بنیاد_name = تہی;

	اگر (!oidset_سائز(&outgoing_links))
		واپس;

	oidset_iter_init(&outgoing_links, &iter);
	جب ((oid = oidset_iter_اگلا(&iter))) {
		ساخت odb_ماخذ_info ماخذ_info;
		ساخت object_info info = {
			.ماخذ_infop = &ماخذ_info,
		};

		اگر (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			جاری;
		اگر (ماخذ_info.ماخذ->type == ODB_ماخذ_PACKED &&
		    ماخذ_info.u.packed.pack->pack_promisor)
			جاری;

		اگر (!cmd.دلائل.nr) {
			بنیاد_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_ڈالو(&cmd.دلائل, "pack-objects");
			strvec_ڈالو(&cmd.دلائل,
				    "--exclude-promisor-objects-best-effort");
			strvec_ڈالو(&cmd.دلائل, بنیاد_name);
			cmd.git_cmd = 1;
			cmd.اندر = -1;
			cmd.برآمد = -1;
			اگر (آغاز_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		اگر (write_اندر_بھرا(cmd.اندر, oid_مقصد_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_اندر_بھرا(cmd.اندر, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	اگر (!cmd.دلائل.nr)
		واپس;

	close(cmd.اندر);

	برآمد = xfdopen(cmd.برآمد, "r");
	جب (strbuf_getline_lf(&سطر, برآمد) != EOF) {
		غیرعلامتی حرف ثنائی[GIT_MAX_RAWSZ];
		اگر (سطر.len != the_hash_algo->hexsz ||
		    !hex_مقصد_بائٹیں(ثنائی, سطر.buf, سطر.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", تہی, ثنائی, تہی);
	}

	fclose(برآمد);
	اگر (اختتام_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&سطر);
	آزاد(بنیاد_name);
}

صحیح cmd_اشاریہ_pack(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *سابقہ,
		   ساخت repository *repo)
{
	صحیح i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_اشاریہ;
	ثابت حرف *curr_اشاریہ;
	حرف *curr_rev_اشاریہ = تہی;
	ثابت حرف *اشاریہ_name = تہی, *pack_name = تہی, *rev_اشاریہ_name = تہی;
	ثابت حرف *keep_پیغام = تہی;
	ثابت حرف *promisor_پیغام = تہی;
	ساخت strbuf اشاریہ_name_buf = STRBUF_INIT;
	ساخت strbuf rev_اشاریہ_name_buf = STRBUF_INIT;
	ساخت pack_idx_اندراج **idx_objects;
	ساخت pack_idx_option opts;
	غیرعلامتی حرف pack_hash[GIT_MAX_RAWSZ];
	غیرعلامتی foreign_nr = 1;	/* zero is a "good" value, assume bad */
	صحیح report_انتہا_کا_داخلہ = 0;
	صحیح hash_algo = 0;

	show_usage_if_asked(argc, argv, اشاریہ_pack_usage);

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
	repo_config(the_repository, git_اشاریہ_pack_config, &opts);
	اگر (سابقہ && chdir(سابقہ))
		die(_("Cannot come back to cwd"));

	اگر (git_env_bool(GIT_TEST_NO_WRITE_REV_اشاریہ, 0))
		rev_اشاریہ = 0;
	ورنہ
		rev_اشاریہ = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	برائے (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		اگر (*arg == '-') {
			اگر (!strcmp(arg, "--stdin")) {
				from_stdin = 1;
			} ورنہ اگر (!strcmp(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} ورنہ اگر (skip_مقصد_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_سیٹ_پیغام_types(&fsck_options, arg);
			} ورنہ اگر (!strcmp(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} ورنہ اگر (skip_مقصد_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_سیٹ_پیغام_types(&fsck_options, arg);
			} ورنہ اگر (!strcmp(arg, "--verify")) {
				verify = 1;
			} ورنہ اگر (!strcmp(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} ورنہ اگر (!strcmp(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} ورنہ اگر (skip_مقصد_optional_arg(arg, "--keep", &keep_پیغام)) {
				; /* nothing to do */
			} ورنہ اگر (skip_مقصد_optional_arg(arg, "--promisor", &promisor_پیغام)) {
				ریکارڈ_outgoing_links = 1;
			} ورنہ اگر (starts_with(arg, "--threads=")) {
				حرف *انتہا;
				nr_دھاگے = strtoul(arg+10, &انتہا, 0);
				اگر (!arg[10] || *انتہا || nr_دھاگے < 0)
					usage(اشاریہ_pack_usage);
				اگر (!HAVE_دھاگے && nr_دھاگے != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_دھاگے = 1;
				}
			} ورنہ اگر (skip_سابقہ(arg, "--pack_header=", &arg)) {
				اگر (parse_pack_سرخی_option(arg,
							     داخلہ_بفر,
							     &داخلہ_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} ورنہ اگر (!strcmp(arg, "-v")) {
				verbose = 1;
			} ورنہ اگر (!strcmp(arg, "--progress-title")) {
				اگر (پیش_رفت_title || (i+1) >= argc)
					usage(اشاریہ_pack_usage);
				پیش_رفت_title = argv[++i];
			} ورنہ اگر (!strcmp(arg, "--show-resolving-progress")) {
				show_resolving_پیش_رفت = 1;
			} ورنہ اگر (!strcmp(arg, "--report-end-of-input")) {
				report_انتہا_کا_داخلہ = 1;
			} ورنہ اگر (!strcmp(arg, "-o")) {
				اگر (اشاریہ_name || (i+1) >= argc)
					usage(اشاریہ_pack_usage);
				اشاریہ_name = argv[++i];
			} ورنہ اگر (starts_with(arg, "--index-version=")) {
				حرف *c;
				opts.نسخہ = strtoul(arg + 16, &c, 10);
				اگر (opts.نسخہ > 2)
					die(_("bad %s"), arg);
				اگر (*c == ',')
					opts.off32_حد_بندی = strtoul(c+1, &c, 0);
				اگر (*c || opts.off32_حد_بندی & 0x80000000)
					die(_("bad %s"), arg);
			} ورنہ اگر (skip_سابقہ(arg, "--max-input-size=", &arg)) {
				max_داخلہ_سائز = strtoumax(arg, تہی, 10);
			} ورنہ اگر (skip_سابقہ(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				اگر (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_سیٹ_hash_algo(the_repository, hash_algo);
			} ورنہ اگر (!strcmp(arg, "--rev-index")) {
				rev_اشاریہ = 1;
			} ورنہ اگر (!strcmp(arg, "--no-rev-index")) {
				rev_اشاریہ = 0;
			} ورنہ
				usage(اشاریہ_pack_usage);
			جاری;
		}

		اگر (pack_name)
			usage(اشاریہ_pack_usage);
		pack_name = arg;
	}

	اگر (!pack_name && !from_stdin)
		usage(اشاریہ_pack_usage);
	اگر (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	اگر (promisor_پیغام && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	اگر (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	اگر (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	اگر (!اشاریہ_name && pack_name)
		اشاریہ_name = derive_filename(pack_name, "pack", "idx", &اشاریہ_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	اگر (!the_repository->hash_algo)
		repo_سیٹ_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	اگر (rev_اشاریہ) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		اگر (اشاریہ_name)
			rev_اشاریہ_name = derive_filename(اشاریہ_name,
							 "idx", "rev",
							 &rev_اشاریہ_name_buf);
	}

	اگر (verify) {
		اگر (!اشاریہ_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, اشاریہ_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	اگر (strict)
		opts.flags |= WRITE_IDX_STRICT;

	اگر (HAVE_دھاگے && !nr_دھاگے) {
		nr_دھاگے = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		اگر (nr_دھاگے < 4)
			; /* too few cores to consider capping */
		ورنہ اگر (nr_دھاگے < 6)
			nr_دھاگے = 3; /* historic cap */
		ورنہ اگر (nr_دھاگے < 40)
			nr_دھاگے /= 2;
		ورنہ
			nr_دھاگے = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_سرخی();
	CALLOC_ARRAY(objects, st_جوڑو(nr_objects, 1));
	اگر (show_stat)
		CALLOC_ARRAY(obj_stat, st_جوڑو(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	اگر (report_انتہا_کا_داخلہ)
		write_اندر_بھرا(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	آزاد(ofs_deltas);
	آزاد(ref_deltas);
	اگر (strict)
		foreign_nr = check_objects();

	اگر (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	برائے (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_اشاریہ = write_idx_file(the_repository, اشاریہ_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	اگر (rev_اشاریہ)
		curr_rev_اشاریہ = write_rev_file(the_repository, rev_اشاریہ_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	آزاد(idx_objects);

	اگر (!verify)
		final(pack_name, curr_pack,
		      اشاریہ_name, curr_اشاریہ,
		      rev_اشاریہ_name, curr_rev_اشاریہ,
		      keep_پیغام, promisor_پیغام,
		      pack_hash);
	ورنہ
		close(داخلہ_fd);

	اگر (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		اگر (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		اگر (fsck_اختتام(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	آزاد(opts.anomaly);
	آزاد(objects);
	strbuf_release(&اشاریہ_name_buf);
	strbuf_release(&rev_اشاریہ_name_buf);
	اگر (!pack_name)
		آزاد((خالی *) curr_pack);
	اگر (!اشاریہ_name)
		آزاد((خالی *) curr_اشاریہ);
	آزاد(curr_rev_اشاریہ);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	اگر (check_self_contained_and_connected && foreign_nr)
		واپس 1;

	واپس 0;
}
