#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "delta.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "pack.h"
#अन्तर्भाव "csum-file.h"
#अन्तर्भाव "blob.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "progress.h"
#अन्तर्भाव "fsck.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "thread-utils.h"
#अन्तर्भाव "packfile.h"
#अन्तर्भाव "pack-revindex.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "odb/streaming.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "oidset.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "tree-walk.h"
#अन्तर्भाव "promisor-remote.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "strvec.h"

स्थिर ध्रुव वर्ण सूचकांक_pack_usage[] =
"git index-pack [-v] [-o <index-file>] [--keep | --keep=<msg>] [--[no-]rev-index] [--verify] [--strict[=<msg-id>=<severity>...]] [--fsck-objects[=<msg-id>=<severity>...]] (<pack-file> | --stdin [--fix-thin] [<pack-file>])";

संरचना object_प्रविष्टिः {
	संरचना pack_idx_प्रविष्टिः idx;
	आकार_t आकार;
	अचिह्नित वर्ण hdr_आकार;
	चिह्नित वर्ण type;
	चिह्नित वर्ण real_type;
};

संरचना object_stat {
	अचिह्नित delta_गभीरता;
	पूर्णाङ्क आधारः_object_no;
};

संरचना आधारः_दत्त {
	/* Initialized by make_base(). */
	संरचना आधारः_दत्त *आधारः;
	संरचना object_प्रविष्टिः *obj;
	पूर्णाङ्क ref_first, ref_चरमः;
	पूर्णाङ्क ofs_first, ofs_चरमः;
	/*
	 * Threads should increment retain_data if they are about to call
	 * patch_delta() using this struct's data as a base, and decrement this
	 * when they are done. While retain_data is nonzero, this struct's data
	 * will not be freed even if the delta base cache limit is exceeded.
	 */
	पूर्णाङ्क retain_दत्त;
	/*
	 * The number of direct children that have not been fully processed
	 * (entered work_head, entered done_head, left done_head). When this
	 * number reaches zero, this struct base_data can be freed.
	 */
	पूर्णाङ्क children_remaining;

	/* Not initialized by make_base(). */
	संरचना list_शीर्ष list;
	शून्य *दत्त;
	आकार_t आकार;
};

/*
 * Stack of struct base_data that have unprocessed children.
 * threaded_second_pass() uses this as a source of work (the other being the
 * objects array).
 *
 * Guarded by work_mutex.
 */
स्थिर LIST_शीर्ष(work_शीर्ष);

/*
 * Stack of struct base_data that have children, all of whom have been
 * processed or are being processed, and at least one child is being processed.
 * These struct base_data must be kept around until the last child is
 * processed.
 *
 * Guarded by work_mutex.
 */
स्थिर LIST_शीर्ष(समाप्तम्_शीर्ष);

/*
 * All threads share one delta base cache.
 *
 * base_cache_used is guarded by work_mutex, and base_cache_limit is read-only
 * in a thread.
 */
स्थिर आकार_t आधारः_cache_प्रयुक्तम्;
स्थिर आकार_t आधारः_cache_सीमा;

संरचना thread_local_दत्त {
	pthread_t thread;
	पूर्णाङ्क pack_fd;
};

/* Remember to update object flag allocation in object.h */
#परिभाषय FLAG_LINK (1u<<20)
#परिभाषय FLAG_CHECKED (1u<<21)

संरचना ofs_delta_प्रविष्टिः {
	off_t विस्थापनम्;
	पूर्णाङ्क obj_no;
};

संरचना ref_delta_प्रविष्टिः {
	संरचना object_id oid;
	पूर्णाङ्क obj_no;
};

स्थिर संरचना object_प्रविष्टिः *objects;
स्थिर संरचना object_stat *obj_stat;
स्थिर संरचना ofs_delta_प्रविष्टिः *ofs_deltas;
स्थिर संरचना ref_delta_प्रविष्टिः *ref_deltas;
स्थिर संरचना thread_local_दत्त nothread_दत्त;
स्थिर पूर्णाङ्क nr_objects;
स्थिर पूर्णाङ्क nr_ofs_deltas;
स्थिर पूर्णाङ्क nr_ref_deltas;
स्थिर पूर्णाङ्क ref_deltas_alloc;
स्थिर पूर्णाङ्क nr_resolved_deltas;
स्थिर पूर्णाङ्क nr_तन्तवः;

स्थिर पूर्णाङ्क from_stdin;
स्थिर पूर्णाङ्क strict;
स्थिर पूर्णाङ्क do_fsck_object;
स्थिर संरचना fsck_options fsck_options;
स्थिर पूर्णाङ्क verbose;
स्थिर ध्रुव वर्ण *प्रगतिः_title;
स्थिर पूर्णाङ्क show_resolving_प्रगतिः;
स्थिर पूर्णाङ्क show_stat;
स्थिर पूर्णाङ्क check_self_contained_and_connected;

स्थिर संरचना प्रगतिः *प्रगतिः;

स्थिर अचिह्नित वर्ण निवेशः_बफर[DEFAULT_IO_बफर_आकार];
स्थिर अचिह्नित पूर्णाङ्क निवेशः_विस्थापनम्, निवेशः_len;
स्थिर off_t consumed_अष्टकानि;
स्थिर off_t max_निवेशः_आकार;
स्थिर अचिह्नित गभीरतमम्_delta;
स्थिर संरचना git_hash_ctx निवेशः_ctx;
स्थिर uint32_t निवेशः_चक्रीयपरीक्षा_32;
स्थिर पूर्णाङ्क निवेशः_fd, output_fd;
स्थिर ध्रुव वर्ण *curr_pack;

/*
 * outgoing_links is guarded by read_mutex, and record_outgoing_links is
 * read-only in a thread.
 */
स्थिर संरचना oidset outgoing_links = OIDSET_INIT;
स्थिर पूर्णाङ्क लेखः_outgoing_links;

स्थिर संरचना thread_local_दत्त *thread_दत्त;
स्थिर पूर्णाङ्क nr_dispatched;
स्थिर पूर्णाङ्क तन्तवः_सक्रिय;

स्थिर pthread_mutex_t read_mutex;
#परिभाषय read_तालकम्()		तालकम्_mutex(&read_mutex)
#परिभाषय read_unlock()		unlock_mutex(&read_mutex)

स्थिर pthread_mutex_t गणक_mutex;
#परिभाषय गणक_तालकम्()		तालकम्_mutex(&गणक_mutex)
#परिभाषय गणक_unlock()	unlock_mutex(&गणक_mutex)

स्थिर pthread_mutex_t work_mutex;
#परिभाषय work_तालकम्()		तालकम्_mutex(&work_mutex)
#परिभाषय work_unlock()		unlock_mutex(&work_mutex)

स्थिर pthread_mutex_t गभीरतमम्_delta_mutex;
#परिभाषय गभीरतमम्_delta_तालकम्()	तालकम्_mutex(&गभीरतमम्_delta_mutex)
#परिभाषय गभीरतमम्_delta_unlock()	unlock_mutex(&गभीरतमम्_delta_mutex)

स्थिर pthread_कुञ्चिका_t कुञ्चिका;

स्थिर अन्तःस्थ शून्य तालकम्_mutex(pthread_mutex_t *mutex)
{
	यदि (तन्तवः_सक्रिय)
		pthread_mutex_तालकम्(mutex);
}

स्थिर अन्तःस्थ शून्य unlock_mutex(pthread_mutex_t *mutex)
{
	यदि (तन्तवः_सक्रिय)
		pthread_mutex_unlock(mutex);
}

/*
 * Mutex and conditional variable can't be statically-initialized on Windows.
 */
स्थिर शून्य init_thread(शून्य)
{
	पूर्णाङ्क i;
	init_recursive_mutex(&read_mutex);
	pthread_mutex_init(&गणक_mutex, रिक्त);
	pthread_mutex_init(&work_mutex, रिक्त);
	यदि (show_stat)
		pthread_mutex_init(&गभीरतमम्_delta_mutex, रिक्त);
	pthread_कुञ्चिका_create(&कुञ्चिका, रिक्त);
	CALLOC_ARRAY(thread_दत्त, nr_तन्तवः);
	प्रति (i = 0; i < nr_तन्तवः; i++) {
		thread_दत्त[i].pack_fd = xopen(curr_pack, O_RDONLY);
	}

	तन्तवः_सक्रिय = 1;
}

स्थिर शून्य cleanup_thread(शून्य)
{
	पूर्णाङ्क i;
	यदि (!तन्तवः_सक्रिय)
		प्रत्यागम;
	तन्तवः_सक्रिय = 0;
	pthread_mutex_destroy(&read_mutex);
	pthread_mutex_destroy(&गणक_mutex);
	pthread_mutex_destroy(&work_mutex);
	यदि (show_stat)
		pthread_mutex_destroy(&गभीरतमम्_delta_mutex);
	प्रति (i = 0; i < nr_तन्तवः; i++)
		close(thread_दत्त[i].pack_fd);
	pthread_कुञ्चिका_delete(कुञ्चिका);
	मुक्त(thread_दत्त);
}

स्थिर पूर्णाङ्क mark_link(संरचना object *obj, गणना object_type type,
		     शून्य *दत्त अप्रयुक्तम्,
		     संरचना fsck_options *options अप्रयुक्तम्)
{
	यदि (!obj)
		प्रत्यागम -1;

	यदि (type != OBJ_ANY && obj->type != type)
		die(_("object type mismatch at %s"), oid_गन्तव्यम्_hex(&obj->oid));

	obj->flags |= FLAG_LINK;
	प्रत्यागम 0;
}

/* The content of each linked object must have been checked
   or it must be already present in the object database */
स्थिर अचिह्नित check_object(संरचना object *obj)
{
	यदि (!obj)
		प्रत्यागम 0;

	यदि (!(obj->flags & FLAG_LINK))
		प्रत्यागम 0;

	यदि (!(obj->flags & FLAG_CHECKED)) {
		आकार_t आकार;
		पूर्णाङ्क type = odb_read_object_info(the_repository->objects,
						&obj->oid, &आकार);
		यदि (type <= 0)
			die(_("did not receive expected object %s"),
			      oid_गन्तव्यम्_hex(&obj->oid));
		यदि (type != obj->type)
			die(_("object %s: expected type %s, found %s"),
			    oid_गन्तव्यम्_hex(&obj->oid),
			    type_name(obj->type), type_name(type));
		obj->flags |= FLAG_CHECKED;
		प्रत्यागम 1;
	}

	प्रत्यागम 0;
}

स्थिर अचिह्नित check_objects(शून्य)
{
	अचिह्नित i, max, foreign_nr = 0;

	max = get_max_object_सूचकांक(the_repository);

	यदि (verbose)
		प्रगतिः = start_delayed_प्रगतिः(the_repository,
						  _("Checking objects"), max);

	प्रति (i = 0; i < max; i++) {
		foreign_nr += check_object(get_अनुक्रमितम्_object(the_repository, i));
		display_प्रगतिः(प्रगतिः, i + 1);
	}

	stop_प्रगतिः(&प्रगतिः);
	प्रत्यागम foreign_nr;
}


/* Discard current buffer used content. */
स्थिर शून्य flush(शून्य)
{
	यदि (निवेशः_विस्थापनम्) {
		यदि (output_fd >= 0)
			write_or_die(output_fd, निवेशः_बफर, निवेशः_विस्थापनम्);
		git_hash_update(&निवेशः_ctx, निवेशः_बफर, निवेशः_विस्थापनम्);
		memmove(निवेशः_बफर, निवेशः_बफर + निवेशः_विस्थापनम्, निवेशः_len);
		निवेशः_विस्थापनम् = 0;
	}
}

/*
 * Make sure at least "min" bytes are available in the buffer, and
 * return the pointer to the buffer.
 */
स्थिर शून्य *fill(पूर्णाङ्क min)
{
	यदि (min <= निवेशः_len)
		प्रत्यागम निवेशः_बफर + निवेशः_विस्थापनम्;
	यदि (min > परिमाण(निवेशः_बफर))
		die(Q_("cannot fill %d byte",
		       "cannot fill %d bytes",
		       min),
		    min);
	flush();
	कुरु {
		ssize_t ret = xread(निवेशः_fd, निवेशः_बफर + निवेशः_len,
				परिमाण(निवेशः_बफर) - निवेशः_len);
		यदि (ret <= 0) {
			यदि (!ret)
				die(_("early EOF"));
			die_errno(_("read error on input"));
		}
		निवेशः_len += ret;
		यदि (from_stdin)
			display_throughput(प्रगतिः, consumed_अष्टकानि + निवेशः_len);
	} यावत् (निवेशः_len < min);
	प्रत्यागम निवेशः_बफर;
}

स्थिर शून्य use(पूर्णाङ्क अष्टकानि)
{
	यदि (अष्टकानि > निवेशः_len)
		die(_("used more bytes than were available"));
	निवेशः_चक्रीयपरीक्षा_32 = चक्रीयपरीक्षा_32(निवेशः_चक्रीयपरीक्षा_32, निवेशः_बफर + निवेशः_विस्थापनम्, अष्टकानि);
	निवेशः_len -= अष्टकानि;
	निवेशः_विस्थापनम् += अष्टकानि;

	/* make sure off_t is sufficiently large not to wrap */
	यदि (signed_सङ्कलय_overflows(consumed_अष्टकानि, अष्टकानि))
		die(_("pack too large for current definition of off_t"));
	consumed_अष्टकानि += अष्टकानि;
	यदि (max_निवेशः_आकार && consumed_अष्टकानि > max_निवेशः_आकार) {
		संरचना strbuf आकार_सीमा = STRBUF_INIT;
		strbuf_humanise_अष्टकानि(&आकार_सीमा, max_निवेशः_आकार);
		die(_("pack exceeds maximum allowed size (%s)"),
		    आकार_सीमा.buf);
	}
}

स्थिर ध्रुव वर्ण *open_pack_file(ध्रुव वर्ण *pack_name)
{
	यदि (from_stdin) {
		निवेशः_fd = 0;
		यदि (!pack_name) {
			संरचना strbuf tmp_file = STRBUF_INIT;
			output_fd = odb_mkstemp(the_repository->objects, &tmp_file,
						"pack/tmp_pack_XXXXXX");
			pack_name = strbuf_detach(&tmp_file, रिक्त);
		} अन्यथा {
			output_fd = xopen(pack_name, O_CREAT|O_EXCL|O_RDWR, 0600);
		}
		nothread_दत्त.pack_fd = output_fd;
	} अन्यथा {
		निवेशः_fd = xopen(pack_name, O_RDONLY);
		output_fd = -1;
		nothread_दत्त.pack_fd = निवेशः_fd;
	}
	git_hash_init(&निवेशः_ctx, the_hash_algo);
	प्रत्यागम pack_name;
}

स्थिर शून्य parse_pack_शीर्षकम्(शून्य)
{
	अचिह्नित वर्ण *hdr = fill(परिमाण(संरचना pack_शीर्षकम्));

	/* Header consistency check */
	यदि (get_be32(hdr) != PACK_SIGNATURE)
		die(_("pack signature mismatch"));
	hdr += 4;
	यदि (!pack_संस्करणम्_ok_native(get_be32(hdr)))
		die(_("pack version %"PRIu32" unsupported"),
		    get_be32(hdr));
	hdr += 4;

	nr_objects = get_be32(hdr);
	use(परिमाण(संरचना pack_शीर्षकम्));
}

__attribute__((format (प्रारूप_लिख, 2, 3)))
स्थिर NORETURN शून्य bad_object(off_t विस्थापनम्, ध्रुव वर्ण *format, ...)
{
	va_list params;
	वर्ण buf[1024];

	va_start(params, format);
	vsnprintf(buf, परिमाण(buf), format, params);
	va_अन्तः(params);
	die(_("pack has bad object at offset %"PRIuMAX": %s"),
	    (uintmax_t)विस्थापनम्, buf);
}

स्थिर अन्तःस्थ संरचना thread_local_दत्त *get_thread_दत्त(शून्य)
{
	यदि (HAVE_तन्तवः) {
		यदि (तन्तवः_सक्रिय)
			प्रत्यागम pthread_getspecific(कुञ्चिका);
		प्रतिज्ञा(!तन्तवः_सक्रिय &&
		       "This should only be reached when all threads are gone");
	}
	प्रत्यागम &nothread_दत्त;
}

स्थिर शून्य समुच्चयः_thread_दत्त(संरचना thread_local_दत्त *दत्त)
{
	यदि (तन्तवः_सक्रिय)
		pthread_setspecific(कुञ्चिका, दत्त);
}

स्थिर शून्य free_आधारः_दत्त(संरचना आधारः_दत्त *c)
{
	यदि (c->दत्त) {
		FREE_AND_NULL(c->दत्त);
		आधारः_cache_प्रयुक्तम् -= c->आकार;
	}
}

स्थिर शून्य prune_आधारः_दत्त(संरचना आधारः_दत्त *retain)
{
	संरचना list_शीर्ष *pos;

	यदि (आधारः_cache_प्रयुक्तम् <= आधारः_cache_सीमा)
		प्रत्यागम;

	list_for_each_पूर्व(pos, &समाप्तम्_शीर्ष) {
		संरचना आधारः_दत्त *b = list_प्रविष्टिः(pos, संरचना आधारः_दत्त, list);
		यदि (b->retain_दत्त || b == retain)
			अनुवर्त;
		यदि (b->दत्त) {
			free_आधारः_दत्त(b);
			यदि (आधारः_cache_प्रयुक्तम् <= आधारः_cache_सीमा)
				प्रत्यागम;
		}
	}

	list_for_each_पूर्व(pos, &work_शीर्ष) {
		संरचना आधारः_दत्त *b = list_प्रविष्टिः(pos, संरचना आधारः_दत्त, list);
		यदि (b->retain_दत्त || b == retain)
			अनुवर्त;
		यदि (b->दत्त) {
			free_आधारः_दत्त(b);
			यदि (आधारः_cache_प्रयुक्तम् <= आधारः_cache_सीमा)
				प्रत्यागम;
		}
	}
}

स्थिर पूर्णाङ्क is_delta_type(गणना object_type type)
{
	प्रत्यागम (type == OBJ_REF_DELTA || type == OBJ_OFS_DELTA);
}

स्थिर शून्य *unpack_प्रविष्टिः_दत्त(off_t विस्थापनम्, आकार_t आकार,
			       गणना object_type type, संरचना object_id *oid)
{
	स्थिर वर्ण fixed_buf[8192];
	पूर्णाङ्क स्थितिः;
	git_zstream प्रवाहः;
	शून्य *buf;
	संरचना git_hash_ctx c;
	वर्ण hdr[32];
	पूर्णाङ्क hdrlen;

	यदि (!is_delta_type(type)) {
		hdrlen = format_object_शीर्षकम्(hdr, परिमाण(hdr), type, आकार);
		git_hash_init(&c, the_hash_algo);
		git_hash_update(&c, hdr, hdrlen);
	} अन्यथा
		oid = रिक्त;
	यदि (type == OBJ_BLOB &&
	    आकार > repo_settings_get_big_file_देहली(the_repository))
		buf = fixed_buf;
	अन्यथा
		buf = xmallocz(आकार);

	स्मृतिं_पूरय(&प्रवाहः, 0, परिमाण(प्रवाहः));
	git_inflate_init(&प्रवाहः);
	प्रवाहः.अग्र_बहिर्गमनम् = buf;
	प्रवाहः.avail_बहिर्गमनम् = buf == fixed_buf ? परिमाण(fixed_buf) : आकार;

	कुरु {
		अचिह्नित वर्ण *चरमः_बहिर्गमनम् = प्रवाहः.अग्र_बहिर्गमनम्;
		प्रवाहः.अग्र_अन्तर् = fill(1);
		प्रवाहः.avail_अन्तर् = निवेशः_len;
		स्थितिः = git_inflate(&प्रवाहः, 0);
		use(निवेशः_len - प्रवाहः.avail_अन्तर्);
		यदि (oid)
			git_hash_update(&c, चरमः_बहिर्गमनम्, प्रवाहः.अग्र_बहिर्गमनम् - चरमः_बहिर्गमनम्);
		यदि (buf == fixed_buf) {
			प्रवाहः.अग्र_बहिर्गमनम् = buf;
			प्रवाहः.avail_बहिर्गमनम् = परिमाण(fixed_buf);
		}
	} यावत् (स्थितिः == Z_OK);
	यदि (प्रवाहः.योग_बहिर्गमनम् != आकार || स्थितिः != Z_प्रवाहः_अन्तः)
		bad_object(विस्थापनम्, _("inflate returned %d"), स्थितिः);
	git_inflate_अन्तः(&प्रवाहः);
	यदि (oid)
		git_hash_final_oid(oid, &c);
	प्रत्यागम buf == fixed_buf ? रिक्त : buf;
}

स्थिर शून्य *unpack_raw_प्रविष्टिः(संरचना object_प्रविष्टिः *obj,
			      off_t *ofs_विस्थापनम्,
			      संरचना object_id *ref_oid,
			      संरचना object_id *oid)
{
	अचिह्नित वर्ण *p;
	आकार_t आकार, c;
	off_t आधारः_विस्थापनम्;
	अचिह्नित सरणम्;
	शून्य *दत्त;

	obj->idx.विस्थापनम् = consumed_अष्टकानि;
	निवेशः_चक्रीयपरीक्षा_32 = चक्रीयपरीक्षा_32(0, रिक्त, 0);

	p = fill(1);
	c = *p;
	use(1);
	obj->type = (c >> 4) & 7;
	आकार = (c & 15);
	सरणम् = 4;
	यावत् (c & 0x80) {
		यदि ((bitsizeof(आकार_t) - 7) < सरणम्)
			die(_("object size too large for this platform"));
		p = fill(1);
		c = *p;
		use(1);
		आकार += (c & 0x7f) << सरणम्;
		सरणम् += 7;
	}
	obj->आकार = आकार;

	विकल्प (obj->type) {
	पक्ष OBJ_REF_DELTA:
		oidread(ref_oid, fill(the_hash_algo->rawsz),
			the_repository->hash_algo);
		use(the_hash_algo->rawsz);
		भङ्ग;
	पक्ष OBJ_OFS_DELTA:
		p = fill(1);
		c = *p;
		use(1);
		आधारः_विस्थापनम् = c & 127;
		यावत् (c & 128) {
			आधारः_विस्थापनम् += 1;
			यदि (!आधारः_विस्थापनम् || MSB(आधारः_विस्थापनम्, 7))
				bad_object(obj->idx.विस्थापनम्, _("offset value overflow for delta base object"));
			p = fill(1);
			c = *p;
			use(1);
			आधारः_विस्थापनम् = (आधारः_विस्थापनम् << 7) + (c & 127);
		}
		*ofs_विस्थापनम् = obj->idx.विस्थापनम् - आधारः_विस्थापनम्;
		यदि (*ofs_विस्थापनम् <= 0 || *ofs_विस्थापनम् >= obj->idx.विस्थापनम्)
			bad_object(obj->idx.विस्थापनम्, _("delta base offset is out of bound"));
		भङ्ग;
	पक्ष OBJ_COMMIT:
	पक्ष OBJ_वृक्षः:
	पक्ष OBJ_BLOB:
	पक्ष OBJ_चिह्नकम्:
		भङ्ग;
	स्वभाव:
		bad_object(obj->idx.विस्थापनम्, _("unknown object type %d"), obj->type);
	}
	obj->hdr_आकार = consumed_अष्टकानि - obj->idx.विस्थापनम्;

	दत्त = unpack_प्रविष्टिः_दत्त(obj->idx.विस्थापनम्, obj->आकार, obj->type, oid);
	obj->idx.चक्रीयपरीक्षा_32 = निवेशः_चक्रीयपरीक्षा_32;
	प्रत्यागम दत्त;
}

स्थिर शून्य *unpack_दत्त(संरचना object_प्रविष्टिः *obj,
			 पूर्णाङ्क (*consume)(ध्रुव अचिह्नित वर्ण *, अचिह्नित दीर्घ, शून्य *),
			 शून्य *cb_दत्त)
{
	off_t from = obj[0].idx.विस्थापनम् + obj[0].hdr_आकार;
	off_t len = obj[1].idx.विस्थापनम् - from;
	अचिह्नित वर्ण *दत्त, *inbuf;
	git_zstream प्रवाहः;
	पूर्णाङ्क स्थितिः;

	दत्त = xmallocz(consume ? 64*1024 : obj->आकार);
	inbuf = xmalloc((len < 64*1024) ? (पूर्णाङ्क)len : 64*1024);

	स्मृतिं_पूरय(&प्रवाहः, 0, परिमाण(प्रवाहः));
	git_inflate_init(&प्रवाहः);
	प्रवाहः.अग्र_बहिर्गमनम् = दत्त;
	प्रवाहः.avail_बहिर्गमनम् = consume ? 64*1024 : obj->आकार;

	कुरु {
		ssize_t n = (len < 64*1024) ? (ssize_t)len : 64*1024;
		n = xpread(get_thread_दत्त()->pack_fd, inbuf, n, from);
		यदि (n < 0)
			die_errno(_("cannot pread pack file"));
		यदि (!n)
			die(Q_("premature end of pack file, %"PRIuMAX" byte missing",
			       "premature end of pack file, %"PRIuMAX" bytes missing",
			       len),
			    (uintmax_t)len);
		from += n;
		len -= n;
		प्रवाहः.अग्र_अन्तर् = inbuf;
		प्रवाहः.avail_अन्तर् = n;
		यदि (!consume)
			स्थितिः = git_inflate(&प्रवाहः, 0);
		अन्यथा {
			कुरु {
				स्थितिः = git_inflate(&प्रवाहः, 0);
				यदि (consume(दत्त, प्रवाहः.अग्र_बहिर्गमनम् - दत्त, cb_दत्त)) {
					मुक्त(inbuf);
					मुक्त(दत्त);
					प्रत्यागम रिक्त;
				}
				प्रवाहः.अग्र_बहिर्गमनम् = दत्त;
				प्रवाहः.avail_बहिर्गमनम् = 64*1024;
			} यावत् (स्थितिः == Z_OK && प्रवाहः.avail_अन्तर्);
		}
	} यावत् (len && स्थितिः == Z_OK && !प्रवाहः.avail_अन्तर्);

	/* This has been inflated OK when first encountered, so... */
	यदि (स्थितिः != Z_प्रवाहः_अन्तः || प्रवाहः.योग_बहिर्गमनम् != obj->आकार)
		die(_("serious inflate inconsistency"));

	git_inflate_अन्तः(&प्रवाहः);
	मुक्त(inbuf);
	यदि (consume) {
		FREE_AND_NULL(दत्त);
	}
	प्रत्यागम दत्त;
}

स्थिर शून्य *get_दत्त_from_pack(संरचना object_प्रविष्टिः *obj)
{
	प्रत्यागम unpack_दत्त(obj, रिक्त, रिक्त);
}

स्थिर पूर्णाङ्क compare_ofs_delta_bases(off_t offset1, off_t offset2,
				   गणना object_type type1,
				   गणना object_type type2)
{
	पूर्णाङ्क cmp = type1 - type2;
	यदि (cmp)
		प्रत्यागम cmp;
	प्रत्यागम offset1 < offset2 ? -1 :
	       offset1 > offset2 ?  1 :
	       0;
}

स्थिर पूर्णाङ्क find_ofs_delta(ध्रुव off_t विस्थापनम्)
{
	पूर्णाङ्क first = 0, चरमः = nr_ofs_deltas;

	यावत् (first < चरमः) {
		पूर्णाङ्क अग्र = first + (चरमः - first) / 2;
		संरचना ofs_delta_प्रविष्टिः *delta = &ofs_deltas[अग्र];
		पूर्णाङ्क cmp;

		cmp = compare_ofs_delta_bases(विस्थापनम्, delta->विस्थापनम्,
					      OBJ_OFS_DELTA,
					      objects[delta->obj_no].type);
		यदि (!cmp)
			प्रत्यागम अग्र;
		यदि (cmp < 0) {
			चरमः = अग्र;
			अनुवर्त;
		}
		first = अग्र+1;
	}
	प्रत्यागम -first-1;
}

स्थिर शून्य find_ofs_delta_children(off_t विस्थापनम्,
				    पूर्णाङ्क *first_सूचकांक, पूर्णाङ्क *चरमः_सूचकांक)
{
	पूर्णाङ्क first = find_ofs_delta(विस्थापनम्);
	पूर्णाङ्क चरमः = first;
	पूर्णाङ्क अन्तः = nr_ofs_deltas - 1;

	यदि (first < 0) {
		*first_सूचकांक = 0;
		*चरमः_सूचकांक = -1;
		प्रत्यागम;
	}
	यावत् (first > 0 && ofs_deltas[first - 1].विस्थापनम् == विस्थापनम्)
		--first;
	यावत् (चरमः < अन्तः && ofs_deltas[चरमः + 1].विस्थापनम् == विस्थापनम्)
		++चरमः;
	*first_सूचकांक = first;
	*चरमः_सूचकांक = चरमः;
}

स्थिर पूर्णाङ्क compare_ref_delta_bases(ध्रुव संरचना object_id *oid1,
				   ध्रुव संरचना object_id *oid2,
				   गणना object_type type1,
				   गणना object_type type2)
{
	पूर्णाङ्क cmp = type1 - type2;
	यदि (cmp)
		प्रत्यागम cmp;
	प्रत्यागम oidcmp(oid1, oid2);
}

स्थिर पूर्णाङ्क find_ref_delta(ध्रुव संरचना object_id *oid)
{
	पूर्णाङ्क first = 0, चरमः = nr_ref_deltas;

	यावत् (first < चरमः) {
		पूर्णाङ्क अग्र = first + (चरमः - first) / 2;
		संरचना ref_delta_प्रविष्टिः *delta = &ref_deltas[अग्र];
		पूर्णाङ्क cmp;

		cmp = compare_ref_delta_bases(oid, &delta->oid,
					      OBJ_REF_DELTA,
					      objects[delta->obj_no].type);
		यदि (!cmp)
			प्रत्यागम अग्र;
		यदि (cmp < 0) {
			चरमः = अग्र;
			अनुवर्त;
		}
		first = अग्र+1;
	}
	प्रत्यागम -first-1;
}

स्थिर शून्य find_ref_delta_children(ध्रुव संरचना object_id *oid,
				    पूर्णाङ्क *first_सूचकांक, पूर्णाङ्क *चरमः_सूचकांक)
{
	पूर्णाङ्क first = find_ref_delta(oid);
	पूर्णाङ्क चरमः = first;
	पूर्णाङ्क अन्तः = nr_ref_deltas - 1;

	यदि (first < 0) {
		*first_सूचकांक = 0;
		*चरमः_सूचकांक = -1;
		प्रत्यागम;
	}
	यावत् (first > 0 && oideq(&ref_deltas[first - 1].oid, oid))
		--first;
	यावत् (चरमः < अन्तः && oideq(&ref_deltas[चरमः + 1].oid, oid))
		++चरमः;
	*first_सूचकांक = first;
	*चरमः_सूचकांक = चरमः;
}

संरचना compare_दत्त {
	संरचना object_प्रविष्टिः *प्रविष्टिः;
	संरचना odb_प्रवाहः *st;
	अचिह्नित वर्ण *buf;
	अचिह्नित दीर्घ buf_आकार;
};

स्थिर पूर्णाङ्क compare_objects(ध्रुव अचिह्नित वर्ण *buf, अचिह्नित दीर्घ आकार,
			   शून्य *cb_दत्त)
{
	संरचना compare_दत्त *दत्त = cb_दत्त;

	यदि (दत्त->buf_आकार < आकार) {
		मुक्त(दत्त->buf);
		दत्त->buf = xmalloc(आकार);
		दत्त->buf_आकार = आकार;
	}

	यावत् (आकार) {
		ssize_t len = odb_प्रवाहः_read(दत्त->st, दत्त->buf, आकार);
		यदि (len == 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_गन्तव्यम्_hex(&दत्त->प्रविष्टिः->idx.oid));
		यदि (len < 0)
			die(_("unable to read %s"),
			    oid_गन्तव्यम्_hex(&दत्त->प्रविष्टिः->idx.oid));
		यदि (memcmp(buf, दत्त->buf, len))
			die(_("SHA1 COLLISION FOUND WITH %s !"),
			    oid_गन्तव्यम्_hex(&दत्त->प्रविष्टिः->idx.oid));
		आकार -= len;
		buf += len;
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क check_collison(संरचना object_प्रविष्टिः *प्रविष्टिः)
{
	संरचना compare_दत्त दत्त;

	यदि (प्रविष्टिः->आकार <= repo_settings_get_big_file_देहली(the_repository) ||
	    प्रविष्टिः->type != OBJ_BLOB)
		प्रत्यागम -1;

	स्मृतिं_पूरय(&दत्त, 0, परिमाण(दत्त));
	दत्त.प्रविष्टिः = प्रविष्टिः;
	दत्त.st = odb_प्रवाहः_from_object(the_repository->objects, &प्रविष्टिः->idx.oid, रिक्त);
	यदि (!दत्त.st)
		प्रत्यागम -1;
	यदि (दत्त.st->आकार != प्रविष्टिः->आकार || दत्त.st->type != प्रविष्टिः->type)
		die(_("SHA1 COLLISION FOUND WITH %s !"),
		    oid_गन्तव्यम्_hex(&प्रविष्टिः->idx.oid));
	unpack_दत्त(प्रविष्टिः, compare_objects, &दत्त);
	odb_प्रवाहः_close(दत्त.st);
	मुक्त(दत्त.buf);
	प्रत्यागम 0;
}

स्थिर शून्य लेखः_outgoing_link(ध्रुव संरचना object_id *oid)
{
	oidset_निवेशय(&outgoing_links, oid);
}

स्थिर शून्य maybe_लेखः_name_प्रविष्टिः(ध्रुव संरचना name_प्रविष्टिः *प्रविष्टिः)
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
	यदि (S_ISDIR(प्रविष्टिः->बहुलकम्))
		लेखः_outgoing_link(&प्रविष्टिः->oid);
}

स्थिर शून्य do_लेखः_outgoing_links(संरचना object *obj)
{
	यदि (obj->type == OBJ_वृक्षः) {
		संरचना वृक्षः *वृक्षः = (संरचना वृक्षः *)obj;
		संरचना वृक्षः_desc desc;
		संरचना name_प्रविष्टिः प्रविष्टिः;
		यदि (init_वृक्षः_desc_gently(&desc, &वृक्षः->object.oid,
					  वृक्षः->बफर, वृक्षः->आकार, 0))
			/*
			 * Error messages are given when packs are
			 * verified, so do not print any here.
			 */
			प्रत्यागम;
		यावत् (वृक्षः_प्रविष्टिः_gently(&desc, &प्रविष्टिः))
			maybe_लेखः_name_प्रविष्टिः(&प्रविष्टिः);
	} अन्यथा यदि (obj->type == OBJ_COMMIT) {
		संरचना commit *commit = (संरचना commit *) obj;
		संरचना commit_list *parents = commit->parents;

		लेखः_outgoing_link(get_commit_वृक्षः_oid(commit));
		प्रति (; parents; parents = parents->अग्र)
			लेखः_outgoing_link(&parents->item->object.oid);
	} अन्यथा यदि (obj->type == OBJ_चिह्नकम्) {
		संरचना चिह्नकम् *चिह्नकम् = (संरचना चिह्नकम् *) obj;
		लेखः_outgoing_link(get_tagged_oid(चिह्नकम्));
	}
}

स्थिर शून्य sha1_object(ध्रुव शून्य *दत्त, संरचना object_प्रविष्टिः *obj_प्रविष्टिः,
			अचिह्नित दीर्घ आकार, गणना object_type type,
			ध्रुव संरचना object_id *oid)
{
	शून्य *new_दत्त = रिक्त;
	पूर्णाङ्क collision_test_needed = 0;

	प्रतिज्ञा(दत्त || obj_प्रविष्टिः);

	यदि (startup_info->have_repository) {
		read_तालकम्();
		collision_test_needed = odb_has_object(the_repository->objects, oid,
						       ODB_HAS_OBJECT_FETCH_PROMISOR);
		read_unlock();
	}

	यदि (collision_test_needed && !दत्त) {
		read_तालकम्();
		यदि (!check_collison(obj_प्रविष्टिः))
			collision_test_needed = 0;
		read_unlock();
	}
	यदि (collision_test_needed) {
		शून्य *has_दत्त;
		गणना object_type has_type;
		आकार_t has_आकार;
		read_तालकम्();
		has_type = odb_read_object_info(the_repository->objects, oid, &has_आकार);
		यदि (has_type < 0)
			die(_("cannot read existing object info %s"), oid_गन्तव्यम्_hex(oid));
		यदि (has_type != type || has_आकार != आकार)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_गन्तव्यम्_hex(oid));
		has_दत्त = odb_read_object(the_repository->objects, oid,
					   &has_type, &has_आकार);
		read_unlock();
		यदि (!दत्त)
			दत्त = new_दत्त = get_दत्त_from_pack(obj_प्रविष्टिः);
		यदि (!has_दत्त)
			die(_("cannot read existing object %s"), oid_गन्तव्यम्_hex(oid));
		यदि (आकार != has_आकार || type != has_type ||
		    memcmp(दत्त, has_दत्त, आकार) != 0)
			die(_("SHA1 COLLISION FOUND WITH %s !"), oid_गन्तव्यम्_hex(oid));
		मुक्त(has_दत्त);
	}

	यदि (strict || do_fsck_object || लेखः_outgoing_links) {
		read_तालकम्();
		यदि (type == OBJ_BLOB) {
			संरचना blob *blob = lookup_blob(the_repository, oid);
			यदि (blob)
				blob->object.flags |= FLAG_CHECKED;
			अन्यथा
				die(_("invalid blob object %s"), oid_गन्तव्यम्_hex(oid));
			यदि (do_fsck_object &&
			    fsck_object(&blob->object, (शून्य *)दत्त, आकार, &fsck_options))
				die(_("fsck error in packed object"));
		} अन्यथा {
			संरचना object *obj;
			पूर्णाङ्क eaten;
			शून्य *buf = (शून्य *) दत्त;

			प्रतिज्ञा(दत्त && "data can only be NULL for large _blobs_");

			/*
			 * we do not need to free the memory here, as the
			 * buf is deleted by the caller.
			 */
			obj = parse_object_बफर(the_repository, oid, type,
						  आकार, buf,
						  &eaten);
			यदि (!obj)
				die(_("invalid %s"), type_name(type));
			यदि (do_fsck_object &&
			    fsck_object(obj, buf, आकार, &fsck_options))
				die(_("fsck error in packed object"));
			यदि (strict && fsck_walk(obj, रिक्त, &fsck_options))
				die(_("Not all child objects of %s are reachable"), oid_गन्तव्यम्_hex(&obj->oid));
			यदि (लेखः_outgoing_links)
				do_लेखः_outgoing_links(obj);

			यदि (obj->type == OBJ_वृक्षः) {
				संरचना वृक्षः *item = (संरचना वृक्षः *) obj;
				item->बफर = रिक्त;
				obj->parsed = 0;
			}
			यदि (obj->type == OBJ_COMMIT) {
				संरचना commit *commit = (संरचना commit *) obj;
				यदि (detach_commit_बफर(commit, रिक्त) != दत्त)
					BUG("parse_object_buffer transmogrified our buffer");
			}
			obj->flags |= FLAG_CHECKED;
		}
		read_unlock();
	}

	मुक्त(new_दत्त);
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
स्थिर शून्य *get_आधारः_दत्त(संरचना आधारः_दत्त *c)
{
	यदि (!c->दत्त) {
		संरचना object_प्रविष्टिः *obj = c->obj;
		संरचना आधारः_दत्त **delta = रिक्त;
		पूर्णाङ्क delta_nr = 0, delta_alloc = 0;

		यावत् (is_delta_type(c->obj->type) && !c->दत्त) {
			ALLOC_GROW(delta, delta_nr + 1, delta_alloc);
			delta[delta_nr++] = c;
			c = c->आधारः;
		}
		यदि (!delta_nr) {
			c->दत्त = get_दत्त_from_pack(obj);
			c->आकार = obj->आकार;
			आधारः_cache_प्रयुक्तम् += c->आकार;
			prune_आधारः_दत्त(c);
		}
		प्रति (; delta_nr > 0; delta_nr--) {
			शून्य *आधारः, *raw;
			c = delta[delta_nr - 1];
			obj = c->obj;
			आधारः = get_आधारः_दत्त(c->आधारः);
			raw = get_दत्त_from_pack(obj);
			c->दत्त = patch_delta(
				आधारः, c->आधारः->आकार,
				raw, obj->आकार,
				&c->आकार);
			मुक्त(raw);
			यदि (!c->दत्त)
				bad_object(obj->idx.विस्थापनम्, _("failed to apply delta"));
			आधारः_cache_प्रयुक्तम् += c->आकार;
			prune_आधारः_दत्त(c);
		}
		मुक्त(delta);
	}
	प्रत्यागम c->दत्त;
}

स्थिर संरचना आधारः_दत्त *make_आधारः(संरचना object_प्रविष्टिः *obj,
				   संरचना आधारः_दत्त *parent)
{
	संरचना आधारः_दत्त *आधारः = xcalloc(1, परिमाण(संरचना आधारः_दत्त));
	आधारः->आधारः = parent;
	आधारः->obj = obj;
	find_ref_delta_children(&obj->idx.oid,
				&आधारः->ref_first, &आधारः->ref_चरमः);
	find_ofs_delta_children(obj->idx.विस्थापनम्,
				&आधारः->ofs_first, &आधारः->ofs_चरमः);
	आधारः->children_remaining = आधारः->ref_चरमः - आधारः->ref_first +
		आधारः->ofs_चरमः - आधारः->ofs_first + 2;
	प्रत्यागम आधारः;
}

स्थिर संरचना आधारः_दत्त *resolve_delta(संरचना object_प्रविष्टिः *delta_obj,
				       संरचना आधारः_दत्त *आधारः)
{
	शून्य *delta_दत्त, *परिणाम_दत्त;
	संरचना आधारः_दत्त *परिणाम;
	आकार_t परिणाम_आकार;

	यदि (show_stat) {
		पूर्णाङ्क i = delta_obj - objects;
		पूर्णाङ्क j = आधारः->obj - objects;
		obj_stat[i].delta_गभीरता = obj_stat[j].delta_गभीरता + 1;
		गभीरतमम्_delta_तालकम्();
		यदि (गभीरतमम्_delta < obj_stat[i].delta_गभीरता)
			गभीरतमम्_delta = obj_stat[i].delta_गभीरता;
		गभीरतमम्_delta_unlock();
		obj_stat[i].आधारः_object_no = j;
	}
	delta_दत्त = get_दत्त_from_pack(delta_obj);
	प्रतिज्ञा(आधारः->दत्त);
	परिणाम_दत्त = patch_delta(आधारः->दत्त, आधारः->आकार,
				  delta_दत्त, delta_obj->आकार, &परिणाम_आकार);
	मुक्त(delta_दत्त);
	यदि (!परिणाम_दत्त)
		bad_object(delta_obj->idx.विस्थापनम्, _("failed to apply delta"));
	hash_object_file(the_hash_algo, परिणाम_दत्त, परिणाम_आकार,
			 delta_obj->real_type, &delta_obj->idx.oid);
	sha1_object(परिणाम_दत्त, रिक्त, परिणाम_आकार, delta_obj->real_type,
		    &delta_obj->idx.oid);

	परिणाम = make_आधारः(delta_obj, आधारः);
	परिणाम->दत्त = परिणाम_दत्त;
	परिणाम->आकार = परिणाम_आकार;

	गणक_तालकम्();
	nr_resolved_deltas++;
	गणक_unlock();

	प्रत्यागम परिणाम;
}

स्थिर पूर्णाङ्क compare_ofs_delta_प्रविष्टिः(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	ध्रुव संरचना ofs_delta_प्रविष्टिः *delta_a = a;
	ध्रुव संरचना ofs_delta_प्रविष्टिः *delta_b = b;

	प्रत्यागम delta_a->विस्थापनम् < delta_b->विस्थापनम् ? -1 :
	       delta_a->विस्थापनम् > delta_b->विस्थापनम् ?  1 :
	       0;
}

स्थिर पूर्णाङ्क compare_ref_delta_प्रविष्टिः(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	ध्रुव संरचना ref_delta_प्रविष्टिः *delta_a = a;
	ध्रुव संरचना ref_delta_प्रविष्टिः *delta_b = b;

	प्रत्यागम oidcmp(&delta_a->oid, &delta_b->oid);
}

स्थिर शून्य *threaded_second_pass(शून्य *दत्त)
{
	यदि (दत्त)
		समुच्चयः_thread_दत्त(दत्त);
	प्रति (;;) {
		संरचना आधारः_दत्त *parent = रिक्त;
		संरचना object_प्रविष्टिः *child_obj = रिक्त;
		संरचना आधारः_दत्त *child = रिक्त;

		गणक_तालकम्();
		display_प्रगतिः(प्रगतिः, nr_resolved_deltas);
		गणक_unlock();

		work_तालकम्();
		यदि (list_रिक्तानि(&work_शीर्ष)) {
			/*
			 * Take an object from the object array.
			 */
			यावत् (nr_dispatched < nr_objects &&
			       is_delta_type(objects[nr_dispatched].type))
				nr_dispatched++;
			यदि (nr_dispatched >= nr_objects) {
				work_unlock();
				भङ्ग;
			}
			child_obj = &objects[nr_dispatched++];
		} अन्यथा {
			/*
			 * Peek at the top of the stack, and take a child from
			 * it.
			 */
			parent = list_first_प्रविष्टिः(&work_शीर्ष, संरचना आधारः_दत्त,
						  list);

			यावत् (parent->ref_first <= parent->ref_चरमः) {
				पूर्णाङ्क विस्थापनम् = ref_deltas[parent->ref_first++].obj_no;
				child_obj = objects + विस्थापनम्;
				यदि (child_obj->real_type != OBJ_REF_DELTA) {
					child_obj = रिक्त;
					अनुवर्त;
				}
				child_obj->real_type = parent->obj->real_type;
				भङ्ग;
			}

			यदि (!child_obj && parent->ofs_first <= parent->ofs_चरमः) {
				child_obj = objects +
					ofs_deltas[parent->ofs_first++].obj_no;
				प्रतिज्ञा(child_obj->real_type == OBJ_OFS_DELTA);
				child_obj->real_type = parent->obj->real_type;
			}

			यदि (parent->ref_first > parent->ref_चरमः &&
			    parent->ofs_first > parent->ofs_चरमः) {
				/*
				 * This parent has run out of children, so move
				 * it to done_head.
				 */
				list_del(&parent->list);
				list_सङ्कलय(&parent->list, &समाप्तम्_शीर्ष);
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
			get_आधारः_दत्त(parent);
			parent->retain_दत्त++;
		}
		work_unlock();

		यदि (child_obj) {
			यदि (parent) {
				child = resolve_delta(child_obj, parent);
				यदि (!child->children_remaining)
					FREE_AND_NULL(child->दत्त);
			} अन्यथा{
				child = make_आधारः(child_obj, रिक्त);
				यदि (child->children_remaining) {
					/*
					 * Since this child has its own delta children,
					 * we will need this data in the future.
					 * Inflate now so that future iterations will
					 * have access to this object's data while
					 * outside the work mutex.
					 */
					child->दत्त = get_दत्त_from_pack(child_obj);
					child->आकार = child_obj->आकार;
				}
			}
		}

		work_तालकम्();
		यदि (parent)
			parent->retain_दत्त--;

		यदि (child && child->दत्त) {
			/*
			 * This child has its own children, so add it to
			 * work_head.
			 */
			list_सङ्कलय(&child->list, &work_शीर्ष);
			आधारः_cache_प्रयुक्तम् += child->आकार;
			prune_आधारः_दत्त(रिक्त);
		} अन्यथा यदि (child) {
			/*
			 * This child does not have its own children. It may be
			 * the last descendant of its ancestors; free those
			 * that we can.
			 */
			संरचना आधारः_दत्त *p = parent;

			यावत् (p) {
				संरचना आधारः_दत्त *अग्र_p;

				p->children_remaining--;
				यदि (p->children_remaining)
					भङ्ग;

				अग्र_p = p->आधारः;
				free_आधारः_दत्त(p);
				list_del(&p->list);
				मुक्त(p);

				p = अग्र_p;
			}
			FREE_AND_NULL(child);
		}
		work_unlock();
	}
	प्रत्यागम रिक्त;
}

/*
 * First pass:
 * - find locations of all objects;
 * - calculate SHA1 of all non-delta objects;
 * - remember base (SHA1 or offset) for all deltas.
 */
स्थिर शून्य parse_pack_objects(अचिह्नित वर्ण *hash)
{
	पूर्णाङ्क i, nr_delays = 0;
	संरचना ofs_delta_प्रविष्टिः *ofs_delta = ofs_deltas;
	संरचना object_id ref_delta_oid;
	संरचना stat st;
	संरचना git_hash_ctx tmp_ctx;

	यदि (verbose)
		प्रगतिः = start_प्रगतिः(
				the_repository,
				प्रगतिः_title ? प्रगतिः_title :
				from_stdin ? _("Receiving objects") : _("Indexing objects"),
				nr_objects);
	प्रति (i = 0; i < nr_objects; i++) {
		संरचना object_प्रविष्टिः *obj = &objects[i];
		शून्य *दत्त = unpack_raw_प्रविष्टिः(obj, &ofs_delta->विस्थापनम्,
					      &ref_delta_oid,
					      &obj->idx.oid);
		obj->real_type = obj->type;
		यदि (obj->type == OBJ_OFS_DELTA) {
			nr_ofs_deltas++;
			ofs_delta->obj_no = i;
			ofs_delta++;
		} अन्यथा यदि (obj->type == OBJ_REF_DELTA) {
			ALLOC_GROW(ref_deltas, nr_ref_deltas + 1, ref_deltas_alloc);
			oidcpy(&ref_deltas[nr_ref_deltas].oid, &ref_delta_oid);
			ref_deltas[nr_ref_deltas].obj_no = i;
			nr_ref_deltas++;
		} अन्यथा यदि (!दत्त) {
			/* large blobs, check later */
			obj->real_type = OBJ_BAD;
			nr_delays++;
		} अन्यथा
			sha1_object(दत्त, रिक्त, obj->आकार, obj->type,
				    &obj->idx.oid);
		मुक्त(दत्त);
		display_प्रगतिः(प्रगतिः, i+1);
	}
	objects[i].idx.विस्थापनम् = consumed_अष्टकानि;
	stop_प्रगतिः(&प्रगतिः);

	/* Check pack integrity */
	flush();
	git_hash_init(&tmp_ctx, the_hash_algo);
	git_hash_clone(&tmp_ctx, &निवेशः_ctx);
	git_hash_final(hash, &tmp_ctx);
	यदि (!hasheq(fill(the_hash_algo->rawsz), hash, the_repository->hash_algo))
		die(_("pack is corrupted (SHA1 mismatch)"));
	use(the_hash_algo->rawsz);

	/* If input_fd is a file, we should have reached its end now. */
	यदि (fstat(निवेशः_fd, &st))
		die_errno(_("cannot fstat packfile"));
	यदि (S_ISREG(st.st_बहुलकम्) &&
			lseek(निवेशः_fd, 0, SEEK_CUR) - निवेशः_len != st.st_आकार)
		die(_("pack has junk at the end"));

	प्रति (i = 0; i < nr_objects; i++) {
		संरचना object_प्रविष्टिः *obj = &objects[i];
		यदि (obj->real_type != OBJ_BAD)
			अनुवर्त;
		obj->real_type = obj->type;
		sha1_object(रिक्त, obj, obj->आकार, obj->type,
			    &obj->idx.oid);
		nr_delays--;
	}
	यदि (nr_delays)
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
स्थिर शून्य resolve_deltas(संरचना pack_idx_option *opts)
{
	पूर्णाङ्क i;

	यदि (!nr_ofs_deltas && !nr_ref_deltas)
		प्रत्यागम;

	/* Sort deltas by base SHA1/offset for fast searching */
	QSORT(ofs_deltas, nr_ofs_deltas, compare_ofs_delta_प्रविष्टिः);
	QSORT(ref_deltas, nr_ref_deltas, compare_ref_delta_प्रविष्टिः);

	यदि (verbose || show_resolving_प्रगतिः)
		प्रगतिः = start_प्रगतिः(the_repository,
					  _("Resolving deltas"),
					  nr_ref_deltas + nr_ofs_deltas);

	nr_dispatched = 0;
	आधारः_cache_सीमा = opts->delta_आधारः_cache_सीमा * nr_तन्तवः;
	यदि (nr_तन्तवः > 1 || getenv("GIT_FORCE_THREADS")) {
		init_thread();
		प्रति (i = 0; i < nr_तन्तवः; i++) {
			पूर्णाङ्क ret = pthread_create(&thread_दत्त[i].thread, रिक्त,
						 threaded_second_pass, thread_दत्त + i);
			यदि (ret)
				die(_("unable to create thread: %s"),
				    strerror(ret));
		}
		प्रति (i = 0; i < nr_तन्तवः; i++)
			pthread_join(thread_दत्त[i].thread, रिक्त);
		cleanup_thread();
		प्रत्यागम;
	}
	threaded_second_pass(&nothread_दत्त);
}

/*
 * Third pass:
 * - append objects to convert thin pack to full pack if required
 * - write the final pack hash
 */
स्थिर शून्य fix_unresolved_deltas(संरचना hashfile *f);
स्थिर शून्य conclude_pack(पूर्णाङ्क fix_thin_pack, ध्रुव वर्ण *curr_pack, अचिह्नित वर्ण *pack_hash)
{
	यदि (nr_ref_deltas + nr_ofs_deltas == nr_resolved_deltas) {
		stop_प्रगतिः(&प्रगतिः);
		/* Flush remaining pack final hash. */
		flush();
		प्रत्यागम;
	}

	यदि (fix_thin_pack) {
		संरचना hashfile *f;
		अचिह्नित वर्ण read_hash[GIT_MAX_RAWSZ], पुच्छ_hash[GIT_MAX_RAWSZ];
		संरचना strbuf सन्देशः = STRBUF_INIT;
		पूर्णाङ्क nr_unresolved = nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas;
		पूर्णाङ्क nr_objects_initial = nr_objects;
		यदि (nr_unresolved <= 0)
			die(_("confusion beyond insanity"));
		REALLOC_ARRAY(objects, nr_objects + nr_unresolved + 1);
		स्मृतिं_पूरय(objects + nr_objects + 1, 0,
		       nr_unresolved * परिमाण(*objects));
		f = hashfd(the_repository->hash_algo, output_fd, curr_pack);
		fix_unresolved_deltas(f);
		strbuf_addf(&सन्देशः, Q_("completed with %d local object",
				     "completed with %d local objects",
				     nr_objects - nr_objects_initial),
			    nr_objects - nr_objects_initial);
		stop_प्रगतिः_सन्देशः(&प्रगतिः, सन्देशः.buf);
		strbuf_release(&सन्देशः);
		finalize_hashfile(f, पुच्छ_hash, FSYNC_COMPONENT_PACK, 0);
		hashcpy(read_hash, pack_hash, the_repository->hash_algo);
		fixup_pack_शीर्षकम्_footer(the_hash_algo, output_fd, pack_hash,
					 curr_pack, nr_objects,
					 read_hash, consumed_अष्टकानि-the_hash_algo->rawsz);
		यदि (!hasheq(read_hash, पुच्छ_hash, the_repository->hash_algo))
			die(_("Unexpected tail checksum for %s "
			      "(disk corruption?)"), curr_pack);
	}
	यदि (nr_ofs_deltas + nr_ref_deltas != nr_resolved_deltas)
		die(Q_("pack has %d unresolved delta",
		       "pack has %d unresolved deltas",
		       nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas),
		    nr_ofs_deltas + nr_ref_deltas - nr_resolved_deltas);
}

स्थिर पूर्णाङ्क write_compressed(संरचना hashfile *f, शून्य *अन्तर्, अचिह्नित पूर्णाङ्क आकार)
{
	git_zstream प्रवाहः;
	पूर्णाङ्क स्थितिः;
	अचिह्नित वर्ण outbuf[4096];
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	git_deflate_init(&प्रवाहः, cfg->zlib_compression_level);
	प्रवाहः.अग्र_अन्तर् = अन्तर्;
	प्रवाहः.avail_अन्तर् = आकार;

	कुरु {
		प्रवाहः.अग्र_बहिर्गमनम् = outbuf;
		प्रवाहः.avail_बहिर्गमनम् = परिमाण(outbuf);
		स्थितिः = git_deflate(&प्रवाहः, Z_FINISH);
		hashwrite(f, outbuf, परिमाण(outbuf) - प्रवाहः.avail_बहिर्गमनम्);
	} यावत् (स्थितिः == Z_OK);

	यदि (स्थितिः != Z_प्रवाहः_अन्तः)
		die(_("unable to deflate appended object (%d)"), स्थितिः);
	आकार = प्रवाहः.योग_बहिर्गमनम्;
	git_deflate_अन्तः(&प्रवाहः);
	प्रत्यागम आकार;
}

स्थिर संरचना object_प्रविष्टिः *append_obj_गन्तव्यम्_pack(संरचना hashfile *f,
			       ध्रुव अचिह्नित वर्ण *sha1, शून्य *buf,
			       अचिह्नित दीर्घ आकार, गणना object_type type)
{
	संरचना object_प्रविष्टिः *obj = &objects[nr_objects++];
	अचिह्नित वर्ण शीर्षकम्[10];
	अचिह्नित दीर्घ s = आकार;
	पूर्णाङ्क n = 0;
	अचिह्नित वर्ण c = (type << 4) | (s & 15);
	s >>= 4;
	यावत् (s) {
		शीर्षकम्[n++] = c | 0x80;
		c = s & 0x7f;
		s >>= 7;
	}
	शीर्षकम्[n++] = c;
	चक्रीयपरीक्षा_32_begin(f);
	hashwrite(f, शीर्षकम्, n);
	obj[0].आकार = आकार;
	obj[0].hdr_आकार = n;
	obj[0].type = type;
	obj[0].real_type = type;
	obj[1].idx.विस्थापनम् = obj[0].idx.विस्थापनम् + n;
	obj[1].idx.विस्थापनम् += write_compressed(f, buf, आकार);
	obj[0].idx.चक्रीयपरीक्षा_32 = चक्रीयपरीक्षा_32_अन्तः(f);
	hashflush(f);
	oidread(&obj->idx.oid, sha1, the_repository->hash_algo);
	प्रत्यागम obj;
}

स्थिर पूर्णाङ्क delta_pos_compare(ध्रुव शून्य *_a, ध्रुव शून्य *_b)
{
	संरचना ref_delta_प्रविष्टिः *a = *(संरचना ref_delta_प्रविष्टिः **)_a;
	संरचना ref_delta_प्रविष्टिः *b = *(संरचना ref_delta_प्रविष्टिः **)_b;
	प्रत्यागम a->obj_no - b->obj_no;
}

स्थिर शून्य fix_unresolved_deltas(संरचना hashfile *f)
{
	संरचना ref_delta_प्रविष्टिः **sorted_by_pos;
	पूर्णाङ्क i;

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
	प्रति (i = 0; i < nr_ref_deltas; i++)
		sorted_by_pos[i] = &ref_deltas[i];
	QSORT(sorted_by_pos, nr_ref_deltas, delta_pos_compare);

	यदि (repo_has_promisor_remote(the_repository)) {
		/*
		 * Prefetch the delta bases.
		 */
		संरचना oid_array गन्तव्यम्_fetch = OID_ARRAY_INIT;
		प्रति (i = 0; i < nr_ref_deltas; i++) {
			संरचना ref_delta_प्रविष्टिः *d = sorted_by_pos[i];
			यदि (!odb_read_object_info_extended(the_repository->objects,
							   &d->oid, रिक्त,
							   OBJECT_INFO_FOR_PREFETCH))
				अनुवर्त;
			oid_array_append(&गन्तव्यम्_fetch, &d->oid);
		}
		promisor_remote_get_प्रत्यक्षम्(the_repository,
					   गन्तव्यम्_fetch.oid, गन्तव्यम्_fetch.nr);
		oid_array_clear(&गन्तव्यम्_fetch);
	}

	प्रति (i = 0; i < nr_ref_deltas; i++) {
		संरचना ref_delta_प्रविष्टिः *d = sorted_by_pos[i];
		गणना object_type type;
		शून्य *दत्त;
		आकार_t आकार;

		यदि (objects[d->obj_no].real_type != OBJ_REF_DELTA)
			अनुवर्त;
		दत्त = odb_read_object(the_repository->objects, &d->oid,
				       &type, &आकार);
		यदि (!दत्त)
			अनुवर्त;

		यदि (check_object_signature(the_repository, &d->oid, दत्त, आकार,
					   type) < 0)
			die(_("local object %s is corrupt"), oid_गन्तव्यम्_hex(&d->oid));

		/*
		 * Add this as an object to the objects array and call
		 * threaded_second_pass() (which will pick up the added
		 * object).
		 */
		append_obj_गन्तव्यम्_pack(f, d->oid.hash, दत्त, आकार, type);
		मुक्त(दत्त);
		threaded_second_pass(रिक्त);

		display_प्रगतिः(प्रगतिः, nr_resolved_deltas);
	}
	मुक्त(sorted_by_pos);
}

स्थिर ध्रुव वर्ण *derive_filename(ध्रुव वर्ण *pack_name, ध्रुव वर्ण *strip,
				   ध्रुव वर्ण *suffix, संरचना strbuf *buf)
{
	आकार_t len;
	यदि (!strip_suffix(pack_name, strip, &len) || !len ||
	    pack_name[len - 1] != '.')
		die(_("packfile name '%s' does not end with '.%s'"),
		    pack_name, strip);
	strbuf_सङ्कलय(buf, pack_name, len);
	strbuf_addstr(buf, suffix);
	प्रत्यागम buf->buf;
}

स्थिर शून्य write_special_file(ध्रुव वर्ण *suffix, ध्रुव वर्ण *सन्देशः,
			       ध्रुव वर्ण *pack_name, ध्रुव अचिह्नित वर्ण *hash,
			       ध्रुव वर्ण **report)
{
	संरचना strbuf name_buf = STRBUF_INIT;
	ध्रुव वर्ण *filename;
	पूर्णाङ्क fd;
	पूर्णाङ्क सन्देशः_len = वाक्यदैर्घ्यम्(सन्देशः);

	यदि (pack_name)
		filename = derive_filename(pack_name, "pack", suffix, &name_buf);
	अन्यथा
		filename = odb_pack_name(the_repository, &name_buf, hash, suffix);

	fd = safe_create_file_with_leading_directories(the_repository, filename);
	यदि (fd < 0) {
		यदि (errno != EEXIST)
			die_errno(_("cannot write %s file '%s'"),
				  suffix, filename);
	} अन्यथा {
		यदि (सन्देशः_len > 0) {
			write_or_die(fd, सन्देशः, सन्देशः_len);
			write_or_die(fd, "\n", 1);
		}
		यदि (close(fd) != 0)
			die_errno(_("cannot close written %s file '%s'"),
				  suffix, filename);
		यदि (report)
			*report = suffix;
	}
	strbuf_release(&name_buf);
}

स्थिर शून्य rename_tmp_packfile(ध्रुव वर्ण **final_name,
				ध्रुव वर्ण *curr_name,
				संरचना strbuf *name, अचिह्नित वर्ण *hash,
				ध्रुव वर्ण *ext, पूर्णाङ्क make_read_only_if_same)
{
	यदि (!*final_name || वाक्यं_तुलय(*final_name, curr_name)) {
		यदि (!*final_name)
			*final_name = odb_pack_name(the_repository, name, hash, ext);
		यदि (finalize_object_file(the_repository, curr_name, *final_name))
			die(_("unable to rename temporary '*.%s' file to '%s'"),
			    ext, *final_name);
	} अन्यथा यदि (make_read_only_if_same) {
		chmod(*final_name, 0444);
	}
}

स्थिर शून्य final(ध्रुव वर्ण *final_pack_name, ध्रुव वर्ण *curr_pack_name,
		  ध्रुव वर्ण *final_सूचकांक_name, ध्रुव वर्ण *curr_सूचकांक_name,
		  ध्रुव वर्ण *final_rev_सूचकांक_name, ध्रुव वर्ण *curr_rev_सूचकांक_name,
		  ध्रुव वर्ण *keep_सन्देशः, ध्रुव वर्ण *promisor_सन्देशः,
		  अचिह्नित वर्ण *hash)
{
	ध्रुव वर्ण *report = "pack";
	संरचना strbuf pack_name = STRBUF_INIT;
	संरचना strbuf सूचकांक_name = STRBUF_INIT;
	संरचना strbuf rev_सूचकांक_name = STRBUF_INIT;

	यदि (!from_stdin) {
		close(निवेशः_fd);
	} अन्यथा {
		fsync_component_or_die(FSYNC_COMPONENT_PACK, output_fd, curr_pack_name);
		यदि (close(output_fd))
			die_errno(_("error while closing pack file"));
	}

	यदि (keep_सन्देशः)
		write_special_file("keep", keep_सन्देशः, final_pack_name, hash,
				   &report);
	यदि (promisor_सन्देशः)
		write_special_file("promisor", promisor_सन्देशः, final_pack_name,
				   hash, रिक्त);

	rename_tmp_packfile(&final_pack_name, curr_pack_name, &pack_name,
			    hash, "pack", from_stdin);
	यदि (curr_rev_सूचकांक_name)
		rename_tmp_packfile(&final_rev_सूचकांक_name, curr_rev_सूचकांक_name,
				    &rev_सूचकांक_name, hash, "rev", 1);
	rename_tmp_packfile(&final_सूचकांक_name, curr_सूचकांक_name, &सूचकांक_name,
			    hash, "idx", 1);

	यदि (do_fsck_object && startup_info->have_repository) {
		संरचना odb_source_files *files =
			odb_source_files_downcast(the_repository->objects->source);
		packfile_store_आरोपय_pack(files->dirs->packed, final_सूचकांक_name, 0);
	}

	यदि (!from_stdin) {
		प्रारूप_लिख("%s\n", hash_गन्तव्यम्_hex(hash));
	} अन्यथा {
		संरचना strbuf buf = STRBUF_INIT;

		strbuf_addf(&buf, "%s\t%s\n", report, hash_गन्तव्यम्_hex(hash));
		write_or_die(1, buf.buf, buf.len);
		strbuf_release(&buf);

		/* Write the last part of the buffer to stdout */
		write_अन्तर्_पूर्णम्(1, निवेशः_बफर + निवेशः_विस्थापनम्, निवेशः_len);
	}

	strbuf_release(&rev_सूचकांक_name);
	strbuf_release(&सूचकांक_name);
	strbuf_release(&pack_name);
}

स्थिर पूर्णाङ्क git_सूचकांक_pack_config(ध्रुव वर्ण *k, ध्रुव वर्ण *v,
				 ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	संरचना pack_idx_option *opts = cb;

	यदि (!वाक्यं_तुलय(k, "pack.indexversion")) {
		opts->संस्करणम् = git_config_int(k, v, ctx->kvi);
		यदि (opts->संस्करणम् > 2)
			die(_("bad pack.indexVersion=%"PRIu32), opts->संस्करणम्);
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "pack.threads")) {
		nr_तन्तवः = git_config_int(k, v, ctx->kvi);
		यदि (nr_तन्तवः < 0)
			die(_("invalid number of threads specified (%d)"),
			    nr_तन्तवः);
		यदि (!HAVE_तन्तवः && nr_तन्तवः != 1) {
			warning(_("no threads support, ignoring %s"), k);
			nr_तन्तवः = 1;
		}
		प्रत्यागम 0;
	}
	यदि (!वाक्यं_तुलय(k, "pack.writereverseindex")) {
		यदि (git_config_bool(k, v))
			opts->flags |= WRITE_REV;
		अन्यथा
			opts->flags &= ~WRITE_REV;
	}
	यदि (!वाक्यं_तुलय(k, "core.deltabasecachelimit")) {
		opts->delta_आधारः_cache_सीमा = git_config_ulong(k, v, ctx->kvi);
		प्रत्यागम 0;
	}
	प्रत्यागम git_default_config(k, v, ctx, cb);
}

स्थिर पूर्णाङ्क cmp_uint32(ध्रुव शून्य *a_, ध्रुव शून्य *b_)
{
	uint32_t a = *((uint32_t *)a_);
	uint32_t b = *((uint32_t *)b_);

	प्रत्यागम (a < b) ? -1 : (a != b);
}

स्थिर शून्य read_v2_anomalous_offsets(संरचना packed_git *p,
				      संरचना pack_idx_option *opts)
{
	ध्रुव uint32_t *idx1, *idx2;
	uint32_t i;

	/* The address of the 4-byte offset table */
	idx1 = (((ध्रुव uint32_t *)((ध्रुव uint8_t *)p->सूचकांक_दत्त + p->crc_विस्थापनम्))
		+ (आकार_t)p->अङ्कः_objects /* CRC32 table */
		);

	/* The address of the 8-byte offset table */
	idx2 = idx1 + p->अङ्कः_objects;

	प्रति (i = 0; i < p->अङ्कः_objects; i++) {
		uint32_t off = ntohl(idx1[i]);
		यदि (!(off & 0x80000000))
			अनुवर्त;
		off = off & 0x7fffffff;
		check_pack_सूचकांक_ptr(p, &idx2[off * 2]);
		यदि (idx2[off * 2])
			अनुवर्त;
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

स्थिर शून्य read_idx_option(संरचना pack_idx_option *opts, ध्रुव वर्ण *pack_name)
{
	संरचना packed_git *p = सङ्कलय_packed_git(the_repository, pack_name,
					      वाक्यदैर्घ्यम्(pack_name), 1);

	यदि (!p)
		die(_("Cannot open existing pack file '%s'"), pack_name);
	यदि (open_pack_सूचकांक(p))
		die(_("Cannot open existing pack idx file for '%s'"), pack_name);

	/* Read the attributes from the existing idx file */
	opts->संस्करणम् = p->सूचकांक_संस्करणम्;

	यदि (opts->संस्करणम् == 2)
		read_v2_anomalous_offsets(p, opts);

	/*
	 * Get rid of the idx file as we do not need it anymore.
	 * NEEDSWORK: extract this bit from free_pack_by_name() in
	 * object-file.c, perhaps?  It shouldn't matter very much as we
	 * know we haven't installed this pack (hence we never have
	 * read anything from it).
	 */
	close_pack_सूचकांक(p);
	मुक्त(p);
}

स्थिर शून्य show_pack_info(पूर्णाङ्क stat_only)
{
	पूर्णाङ्क i, baseobjects = nr_objects - nr_ref_deltas - nr_ofs_deltas;
	अचिह्नित दीर्घ *chain_histogram = रिक्त;

	यदि (गभीरतमम्_delta)
		CALLOC_ARRAY(chain_histogram, गभीरतमम्_delta);

	प्रति (i = 0; i < nr_objects; i++) {
		संरचना object_प्रविष्टिः *obj = &objects[i];

		यदि (is_delta_type(obj->type))
			chain_histogram[obj_stat[i].delta_गभीरता - 1]++;
		यदि (stat_only)
			अनुवर्त;
		प्रारूप_लिख("%s %-6s %"PRIuMAX" %"PRIuMAX" %"PRIuMAX,
		       oid_गन्तव्यम्_hex(&obj->idx.oid),
		       type_name(obj->real_type), (uintmax_t)obj->आकार,
		       (uintmax_t)(obj[1].idx.विस्थापनम् - obj->idx.विस्थापनम्),
		       (uintmax_t)obj->idx.विस्थापनम्);
		यदि (is_delta_type(obj->type)) {
			संरचना object_प्रविष्टिः *bobj = &objects[obj_stat[i].आधारः_object_no];
			प्रारूप_लिख(" %u %s", obj_stat[i].delta_गभीरता,
			       oid_गन्तव्यम्_hex(&bobj->idx.oid));
		}
		अक्षरं_लिख('\n');
	}

	यदि (baseobjects)
		printf_ln(Q_("non delta: %d object",
			     "non delta: %d objects",
			     baseobjects),
			  baseobjects);
	प्रति (i = 0; i < गभीरतमम्_delta; i++) {
		यदि (!chain_histogram[i])
			अनुवर्त;
		printf_ln(Q_("chain length = %d: %lu object",
			     "chain length = %d: %lu objects",
			     chain_histogram[i]),
			  i + 1,
			  chain_histogram[i]);
	}
	मुक्त(chain_histogram);
}

स्थिर शून्य repack_local_links(शून्य)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	FILE *बहिर्गमनम्;
	संरचना strbuf पङ्क्तिः = STRBUF_INIT;
	संरचना oidset_iter iter;
	संरचना object_id *oid;
	वर्ण *आधारः_name = रिक्त;

	यदि (!oidset_आकार(&outgoing_links))
		प्रत्यागम;

	oidset_iter_init(&outgoing_links, &iter);
	यावत् ((oid = oidset_iter_अग्र(&iter))) {
		संरचना odb_source_info source_info;
		संरचना object_info info = {
			.source_infop = &source_info,
		};

		यदि (odb_read_object_info_extended(the_repository->objects, oid, &info, 0))
			/* Missing; assume it is a promisor object */
			अनुवर्त;
		यदि (source_info.source->type == ODB_SOURCE_PACKED &&
		    source_info.u.packed.pack->pack_promisor)
			अनुवर्त;

		यदि (!cmd.तर्काः.nr) {
			आधारः_name = mkpathdup(
				"%s/pack/pack",
				repo_get_object_directory(the_repository));
			strvec_प्रक्षिप(&cmd.तर्काः, "pack-objects");
			strvec_प्रक्षिप(&cmd.तर्काः,
				    "--exclude-promisor-objects-best-effort");
			strvec_प्रक्षिप(&cmd.तर्काः, आधारः_name);
			cmd.git_cmd = 1;
			cmd.अन्तर् = -1;
			cmd.बहिर्गमनम् = -1;
			यदि (start_command(&cmd))
				die(_("could not start pack-objects to repack local links"));
		}

		यदि (write_अन्तर्_पूर्णम्(cmd.अन्तर्, oid_गन्तव्यम्_hex(oid), the_hash_algo->hexsz) < 0 ||
		    write_अन्तर्_पूर्णम्(cmd.अन्तर्, "\n", 1) < 0)
			die(_("failed to feed local object to pack-objects"));
	}

	यदि (!cmd.तर्काः.nr)
		प्रत्यागम;

	close(cmd.अन्तर्);

	बहिर्गमनम् = xfdopen(cmd.बहिर्गमनम्, "r");
	यावत् (strbuf_getline_lf(&पङ्क्तिः, बहिर्गमनम्) != EOF) {
		अचिह्नित वर्ण द्विमान[GIT_MAX_RAWSZ];
		यदि (पङ्क्तिः.len != the_hash_algo->hexsz ||
		    !hex_गन्तव्यम्_अष्टकानि(द्विमान, पङ्क्तिः.buf, पङ्क्तिः.len))
			die(_("index-pack: Expecting full hex object ID lines only from pack-objects."));

		/*
		 * pack-objects creates the .pack and .idx files, but not the
		 * .promisor file. Create the .promisor file, which is empty.
		 */
		write_special_file("promisor", "", रिक्त, द्विमान, रिक्त);
	}

	fclose(बहिर्गमनम्);
	यदि (finish_command(&cmd))
		die(_("could not finish pack-objects to repack local links"));
	strbuf_release(&पङ्क्तिः);
	मुक्त(आधारः_name);
}

पूर्णाङ्क cmd_सूचकांक_pack(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *उपसर्गः,
		   संरचना repository *repo)
{
	पूर्णाङ्क i, fix_thin_pack = 0, verify = 0, stat_only = 0, rev_सूचकांक;
	ध्रुव वर्ण *curr_सूचकांक;
	वर्ण *curr_rev_सूचकांक = रिक्त;
	ध्रुव वर्ण *सूचकांक_name = रिक्त, *pack_name = रिक्त, *rev_सूचकांक_name = रिक्त;
	ध्रुव वर्ण *keep_सन्देशः = रिक्त;
	ध्रुव वर्ण *promisor_सन्देशः = रिक्त;
	संरचना strbuf सूचकांक_name_buf = STRBUF_INIT;
	संरचना strbuf rev_सूचकांक_name_buf = STRBUF_INIT;
	संरचना pack_idx_प्रविष्टिः **idx_objects;
	संरचना pack_idx_option opts;
	अचिह्नित वर्ण pack_hash[GIT_MAX_RAWSZ];
	अचिह्नित foreign_nr = 1;	/* zero is a "good" value, assume bad */
	पूर्णाङ्क report_अन्तः_सम्बन्धि_निवेशः = 0;
	पूर्णाङ्क hash_algo = 0;

	show_usage_if_asked(argc, argv, सूचकांक_pack_usage);

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
	repo_config(the_repository, git_सूचकांक_pack_config, &opts);
	यदि (उपसर्गः && chdir(उपसर्गः))
		die(_("Cannot come back to cwd"));

	यदि (git_env_bool(GIT_TEST_NO_WRITE_REV_सूचकांक, 0))
		rev_सूचकांक = 0;
	अन्यथा
		rev_सूचकांक = !!(opts.flags & (WRITE_REV_VERIFY | WRITE_REV));

	प्रति (i = 1; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];

		यदि (*arg == '-') {
			यदि (!वाक्यं_तुलय(arg, "--stdin")) {
				from_stdin = 1;
			} अन्यथा यदि (!वाक्यं_तुलय(arg, "--fix-thin")) {
				fix_thin_pack = 1;
			} अन्यथा यदि (skip_गन्तव्यम्_optional_arg(arg, "--strict", &arg)) {
				strict = 1;
				do_fsck_object = 1;
				fsck_समुच्चयः_सन्देशः_types(&fsck_options, arg);
			} अन्यथा यदि (!वाक्यं_तुलय(arg, "--check-self-contained-and-connected")) {
				strict = 1;
				check_self_contained_and_connected = 1;
			} अन्यथा यदि (skip_गन्तव्यम्_optional_arg(arg, "--fsck-objects", &arg)) {
				do_fsck_object = 1;
				fsck_समुच्चयः_सन्देशः_types(&fsck_options, arg);
			} अन्यथा यदि (!वाक्यं_तुलय(arg, "--verify")) {
				verify = 1;
			} अन्यथा यदि (!वाक्यं_तुलय(arg, "--verify-stat")) {
				verify = 1;
				show_stat = 1;
			} अन्यथा यदि (!वाक्यं_तुलय(arg, "--verify-stat-only")) {
				verify = 1;
				show_stat = 1;
				stat_only = 1;
			} अन्यथा यदि (skip_गन्तव्यम्_optional_arg(arg, "--keep", &keep_सन्देशः)) {
				; /* nothing to do */
			} अन्यथा यदि (skip_गन्तव्यम्_optional_arg(arg, "--promisor", &promisor_सन्देशः)) {
				लेखः_outgoing_links = 1;
			} अन्यथा यदि (starts_with(arg, "--threads=")) {
				वर्ण *अन्तः;
				nr_तन्तवः = strtoul(arg+10, &अन्तः, 0);
				यदि (!arg[10] || *अन्तः || nr_तन्तवः < 0)
					usage(सूचकांक_pack_usage);
				यदि (!HAVE_तन्तवः && nr_तन्तवः != 1) {
					warning(_("no threads support, ignoring %s"), arg);
					nr_तन्तवः = 1;
				}
			} अन्यथा यदि (skip_उपसर्गः(arg, "--pack_header=", &arg)) {
				यदि (parse_pack_शीर्षकम्_option(arg,
							     निवेशः_बफर,
							     &निवेशः_len) < 0)
					die(_("bad --pack_header: %s"), arg);
			} अन्यथा यदि (!वाक्यं_तुलय(arg, "-v")) {
				verbose = 1;
			} अन्यथा यदि (!वाक्यं_तुलय(arg, "--progress-title")) {
				यदि (प्रगतिः_title || (i+1) >= argc)
					usage(सूचकांक_pack_usage);
				प्रगतिः_title = argv[++i];
			} अन्यथा यदि (!वाक्यं_तुलय(arg, "--show-resolving-progress")) {
				show_resolving_प्रगतिः = 1;
			} अन्यथा यदि (!वाक्यं_तुलय(arg, "--report-end-of-input")) {
				report_अन्तः_सम्बन्धि_निवेशः = 1;
			} अन्यथा यदि (!वाक्यं_तुलय(arg, "-o")) {
				यदि (सूचकांक_name || (i+1) >= argc)
					usage(सूचकांक_pack_usage);
				सूचकांक_name = argv[++i];
			} अन्यथा यदि (starts_with(arg, "--index-version=")) {
				वर्ण *c;
				opts.संस्करणम् = strtoul(arg + 16, &c, 10);
				यदि (opts.संस्करणम् > 2)
					die(_("bad %s"), arg);
				यदि (*c == ',')
					opts.off32_सीमा = strtoul(c+1, &c, 0);
				यदि (*c || opts.off32_सीमा & 0x80000000)
					die(_("bad %s"), arg);
			} अन्यथा यदि (skip_उपसर्गः(arg, "--max-input-size=", &arg)) {
				max_निवेशः_आकार = strtoumax(arg, रिक्त, 10);
			} अन्यथा यदि (skip_उपसर्गः(arg, "--object-format=", &arg)) {
				hash_algo = hash_algo_by_name(arg);
				यदि (hash_algo == GIT_HASH_UNKNOWN)
					die(_("unknown hash algorithm '%s'"), arg);
				repo_समुच्चयः_hash_algo(the_repository, hash_algo);
			} अन्यथा यदि (!वाक्यं_तुलय(arg, "--rev-index")) {
				rev_सूचकांक = 1;
			} अन्यथा यदि (!वाक्यं_तुलय(arg, "--no-rev-index")) {
				rev_सूचकांक = 0;
			} अन्यथा
				usage(सूचकांक_pack_usage);
			अनुवर्त;
		}

		यदि (pack_name)
			usage(सूचकांक_pack_usage);
		pack_name = arg;
	}

	यदि (!pack_name && !from_stdin)
		usage(सूचकांक_pack_usage);
	यदि (fix_thin_pack && !from_stdin)
		die(_("the option '%s' requires '%s'"), "--fix-thin", "--stdin");
	यदि (promisor_सन्देशः && pack_name)
		die(_("--promisor cannot be used with a pack name"));
	यदि (from_stdin && !startup_info->have_repository)
		die(_("--stdin requires a git repository"));
	यदि (from_stdin && hash_algo)
		die(_("options '%s' and '%s' cannot be used together"), "--object-format", "--stdin");
	यदि (!सूचकांक_name && pack_name)
		सूचकांक_name = derive_filename(pack_name, "pack", "idx", &सूचकांक_name_buf);

	/*
	 * Packfiles and indices do not carry enough information to be able to
	 * identify their object hash. So when we are neither in a repository
	 * nor has the user told us which object hash to use we have no other
	 * choice but to guess the object hash.
	 */
	यदि (!the_repository->hash_algo)
		repo_समुच्चयः_hash_algo(the_repository, GIT_HASH_DEFAULT);

	opts.flags &= ~(WRITE_REV | WRITE_REV_VERIFY);
	यदि (rev_सूचकांक) {
		opts.flags |= verify ? WRITE_REV_VERIFY : WRITE_REV;
		यदि (सूचकांक_name)
			rev_सूचकांक_name = derive_filename(सूचकांक_name,
							 "idx", "rev",
							 &rev_सूचकांक_name_buf);
	}

	यदि (verify) {
		यदि (!सूचकांक_name)
			die(_("--verify with no packfile name given"));
		read_idx_option(&opts, सूचकांक_name);
		opts.flags |= WRITE_IDX_VERIFY | WRITE_IDX_STRICT;
	}
	यदि (strict)
		opts.flags |= WRITE_IDX_STRICT;

	यदि (HAVE_तन्तवः && !nr_तन्तवः) {
		nr_तन्तवः = online_cpus();
		/*
		 * Experiments show that going above 20 threads doesn't help,
		 * no matter how many cores you have. Below that, we tend to
		 * max at half the number of online_cpus(), presumably because
		 * half of those are hyperthreads rather than full cores. We'll
		 * never reduce the level below "3", though, to match a
		 * historical value that nobody complained about.
		 */
		यदि (nr_तन्तवः < 4)
			; /* too few cores to consider capping */
		अन्यथा यदि (nr_तन्तवः < 6)
			nr_तन्तवः = 3; /* historic cap */
		अन्यथा यदि (nr_तन्तवः < 40)
			nr_तन्तवः /= 2;
		अन्यथा
			nr_तन्तवः = 20; /* hard cap */
	}

	curr_pack = open_pack_file(pack_name);
	parse_pack_शीर्षकम्();
	CALLOC_ARRAY(objects, st_सङ्कलय(nr_objects, 1));
	यदि (show_stat)
		CALLOC_ARRAY(obj_stat, st_सङ्कलय(nr_objects, 1));
	CALLOC_ARRAY(ofs_deltas, nr_objects);
	parse_pack_objects(pack_hash);
	यदि (report_अन्तः_सम्बन्धि_निवेशः)
		write_अन्तर्_पूर्णम्(2, "\0", 1);
	resolve_deltas(&opts);
	conclude_pack(fix_thin_pack, curr_pack, pack_hash);
	मुक्त(ofs_deltas);
	मुक्त(ref_deltas);
	यदि (strict)
		foreign_nr = check_objects();

	यदि (show_stat)
		show_pack_info(stat_only);

	ALLOC_ARRAY(idx_objects, nr_objects);
	प्रति (i = 0; i < nr_objects; i++)
		idx_objects[i] = &objects[i].idx;
	curr_सूचकांक = write_idx_file(the_repository, सूचकांक_name, idx_objects,
				    nr_objects, &opts, pack_hash);
	यदि (rev_सूचकांक)
		curr_rev_सूचकांक = write_rev_file(the_repository, rev_सूचकांक_name,
						idx_objects, nr_objects,
						pack_hash, opts.flags);
	मुक्त(idx_objects);

	यदि (!verify)
		final(pack_name, curr_pack,
		      सूचकांक_name, curr_सूचकांक,
		      rev_सूचकांक_name, curr_rev_सूचकांक,
		      keep_सन्देशः, promisor_सन्देशः,
		      pack_hash);
	अन्यथा
		close(निवेशः_fd);

	यदि (do_fsck_object) {
		/*
		 * We cannot perform queued consistency checks when running
		 * outside of a repository because those require us to read
		 * from the object database, which is uninitialized.
		 *
		 * TODO: we may eventually set up an in-memory object database,
		 * which would allow us to perform these queued checks.
		 */
		यदि (!startup_info->have_repository &&
		    fsck_has_queued_checks(&fsck_options))
			die(_("cannot perform queued object checks outside "
			      "of a repository"));

		यदि (fsck_finish(&fsck_options))
			die(_("fsck error in pack objects"));
	}

	मुक्त(opts.anomaly);
	मुक्त(objects);
	strbuf_release(&सूचकांक_name_buf);
	strbuf_release(&rev_सूचकांक_name_buf);
	यदि (!pack_name)
		मुक्त((शून्य *) curr_pack);
	यदि (!सूचकांक_name)
		मुक्त((शून्य *) curr_सूचकांक);
	मुक्त(curr_rev_सूचकांक);

	repack_local_links();

	/*
	 * Let the caller know this pack is not self contained
	 */
	यदि (check_self_contained_and_connected && foreign_nr)
		प्रत्यागम 1;

	प्रत्यागम 0;
}
