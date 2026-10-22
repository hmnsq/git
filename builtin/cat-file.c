/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "convert.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "ident.h"
#అన్తర్భావ "list-objects-filter-options.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "userdiff.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "packfile.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "odb/streaming.h"
#అన్తర్భావ "replace-object.h"
#అన్తర్భావ "promisor-remote.h"
#అన్తర్భావ "mailmap.h"
#అన్తర్భావ "write-or-die.h"
#అన్తర్భావ "alias.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "transport.h"
#అన్తర్భావ "fetch-object-info.h"

/*
 * Maximum length for a remote URL. While no universal standard exists,
 * 8K is assumed to be a reasonable limit.
 */
#నిర్వచించు MAX_REMOTE_URL_LEN (8 * 1024)

/* Maximum number of objects allowed in a single remote-object-info request. */
#నిర్వచించు MAX_ALLOWED_OBJ_హద్దు 10000

/* Maximum input size permitted for the remote-object-info command. */
#నిర్వచించు MAX_REMOTE_OBJ_INFO_పంక్తి \
	(MAX_REMOTE_URL_LEN + MAX_ALLOWED_OBJ_హద్దు * (GIT_MAX_HEXSZ + 1))

గణన batch_బహుళకం {
	BATCH_బహుళకం_CONTENTS,
	BATCH_బహుళకం_INFO,
	BATCH_బహుళకం_వరుస_AND_DISPATCH,
};

నిర్మాణం batch_options {
	నిర్మాణం list_objects_filter_options objects_filter;
	పూర్ణాంకం enabled;
	పూర్ణాంకం follow_symlinks;
	గణన batch_బహుళకం batch_బహుళకం;
	పూర్ణాంకం బఫరు_output;
	పూర్ణాంకం all_objects;
	పూర్ణాంకం unordered;
	పూర్ణాంకం transform_బహుళకం; /* may be 'w' or 'c' for --filters or --textconv */
	అక్షరం ఆదానం_delim;
	అక్షరం output_delim;
	స్థిరరాశి అక్షరం *format;
};

స్థిర స్థిరరాశి అక్షరం *force_మార్గం;

స్థిర నిర్మాణం string_list mailmap = STRING_LIST_INIT_NODUP;
స్థిర పూర్ణాంకం use_mailmap;

స్థిర అక్షరం *replace_idents_using_mailmap(అక్షరం *, కొలత_t *);

/*
 * The mailmap is initialized with .strdup_strings set to 0,
 * but read_mailmap() sets the bit to 1 (this is true even when
 * not a single mailmap entry is read), so it can be used for
 * lazy loading.
 */
స్థిర శూన్య ఎక్కించు_mailmap(శూన్య)
{
	ఒకవేళ (mailmap.strdup_అక్షరశ్రేణులు)
		తిరిగి;

	read_mailmap(the_repository, &mailmap);
}

స్థిర అక్షరం *replace_idents_using_mailmap(అక్షరం *object_buf, కొలత_t *కొలత)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	స్థిరరాశి అక్షరం *శీర్షికలు[] = { "author ", "committer ", "tagger ", ఖాళీ };

	strbuf_attach(&sb, object_buf, *కొలత, *కొలత + 1);
	apply_mailmap_గమ్యం_శీర్షిక(&sb, శీర్షికలు, &mailmap);
	*కొలత = sb.len;
	తిరిగి strbuf_detach(&sb, ఖాళీ);
}

స్థిర పూర్ణాంకం filter_object(స్థిరరాశి అక్షరం *మార్గం, గుర్తులేని బహుళకం,
			 స్థిరరాశి నిర్మాణం object_id *oid,
			 అక్షరం **buf, కొలత_t *కొలత)
{
	గణన object_type type;

	*buf = odb_read_object(the_repository->objects, oid, &type, కొలత);
	ఒకవేళ (!*buf)
		తిరిగి error(_("cannot read object %s '%s'"),
			     oid_గమ్యం_hex(oid), మార్గం);
	ఒకవేళ ((type == OBJ_BLOB) && S_ISREG(బహుళకం)) {
		నిర్మాణం strbuf strbuf = STRBUF_INIT;
		నిర్మాణం checkout_metadata meta;

		init_checkout_metadata(&meta, ఖాళీ, ఖాళీ, oid);
		ఒకవేళ (convert_గమ్యం_working_వృక్షం(the_repository->సూచీ, మార్గం, *buf, *కొలత, &strbuf, &meta)) {
			విడుదల(*buf);
			*కొలత = strbuf.len;
			*buf = strbuf_detach(&strbuf, ఖాళీ);
		}
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం ప్రవాహం_blob(స్థిరరాశి నిర్మాణం object_id *oid)
{
	ఒకవేళ (odb_ప్రవాహం_blob_గమ్యం_fd(the_repository->objects, 1, oid, ఖాళీ, 0))
		die("unable to stream %s to stdout", oid_గమ్యం_hex(oid));
	తిరిగి 0;
}

స్థిర పూర్ణాంకం cat_one_file(పూర్ణాంకం opt, స్థిరరాశి అక్షరం *exp_type, స్థిరరాశి అక్షరం *obj_name)
{
	పూర్ణాంకం ret;
	నిర్మాణం object_id oid;
	గణన object_type type;
	అక్షరం *buf;
	కొలత_t కొలత;
	నిర్మాణం object_context obj_context = {0};
	నిర్మాణం object_info oi = OBJECT_INFO_INIT;
	గుర్తులేని flags = OBJECT_INFO_LOOKUP_REPLACE;
	గుర్తులేని get_oid_flags =
		GET_OID_రికార్డు_మార్గం |
		GET_OID_ONLY_గమ్యం_DIE |
		GET_OID_HASH_ANY;
	స్థిరరాశి అక్షరం *మార్గం = force_మార్గం;
	స్థిరరాశి పూర్ణాంకం opt_cw = (opt == 'c' || opt == 'w');
	ఒకవేళ (!మార్గం && opt_cw)
		get_oid_flags |= GET_OID_REQUIRE_మార్గం;

	ఒకవేళ (get_oid_with_context(the_repository, obj_name, get_oid_flags, &oid,
				 &obj_context))
		die("Not a valid object name %s", obj_name);

	ఒకవేళ (!మార్గం)
		మార్గం = obj_context.మార్గం;
	ఒకవేళ (obj_context.బహుళకం == S_IFINVALID)
		obj_context.బహుళకం = 0100644;

	buf = ఖాళీ;
	ఎంపిక (opt) {
	స్థితి 't':
		oi.typep = &type;
		ఒకవేళ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");
		రాయి("%s\n", type_name(type));
		ret = 0;
		వెళ్ళు cleanup;

	స్థితి 's':
		oi.sizep = &కొలత;

		ఒకవేళ (use_mailmap) {
			oi.typep = &type;
			oi.contentp = (శూన్య**)&buf;
		}

		ఒకవేళ (odb_read_object_info_extended(the_repository->objects, &oid, &oi, flags) < 0)
			die("git cat-file: could not get object info");

		ఒకవేళ (use_mailmap && (type == OBJ_COMMIT || type == OBJ_ట్యాగ్))
			buf = replace_idents_using_mailmap(buf, &కొలత);

		రాయి("%"PRIuMAX"\n", (uintmax_t)కొలత);
		ret = 0;
		వెళ్ళు cleanup;

	స్థితి 'e':
		ret = !odb_has_object(the_repository->objects, &oid,
				      ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR);
		వెళ్ళు cleanup;

	స్థితి 'w':

		ఒకవేళ (filter_object(మార్గం, obj_context.బహుళకం,
				  &oid, &buf, &కొలత)) {
			ret = -1;
			వెళ్ళు cleanup;
		}
		ఆపు;

	స్థితి 'c':
	{
		గుర్తులేని దీర్ఘ కొలత_ul = 0;
		పూర్ణాంకం textconv_ret = textconv_object(the_repository, మార్గం,
						   obj_context.బహుళకం, &oid, 1,
						   &buf, &కొలత_ul);
		కొలత = కొలత_ul;
		ఒకవేళ (textconv_ret)
			ఆపు;
	}
		/* else fallthrough */

	స్థితి 'p':
		type = odb_read_object_info(the_repository->objects, &oid, ఖాళీ);
		ఒకవేళ (type < 0)
			die("Not a valid object name %s", obj_name);

		/* custom pretty-print here */
		ఒకవేళ (type == OBJ_వృక్షం) {
			స్థిరరాశి అక్షరం *ls_వాదనలు[3] = { ఖాళీ };
			ls_వాదనలు[0] =  "ls-tree";
			ls_వాదనలు[1] =  obj_name;
			ret = cmd_ls_వృక్షం(2, ls_వాదనలు, ఖాళీ, the_repository);
			వెళ్ళు cleanup;
		}

		ఒకవేళ (type == OBJ_BLOB) {
			ret = ప్రవాహం_blob(&oid);
			వెళ్ళు cleanup;
		}
		buf = odb_read_object(the_repository->objects, &oid,
				      &type, &కొలత);
		ఒకవేళ (!buf)
			die("Cannot read object %s", obj_name);

		ఒకవేళ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &కొలత);

		/* otherwise just spit out the data */
		ఆపు;

	స్థితి 0:
	{
		గణన object_type exp_type_id = type_from_string(exp_type);

		ఒకవేళ (exp_type_id == OBJ_BLOB) {
			నిర్మాణం object_id blob_oid;
			ఒకవేళ (odb_read_object_info(the_repository->objects,
						 &oid, ఖాళీ) == OBJ_ట్యాగ్) {
				అక్షరం *బఫరు = odb_read_object(the_repository->objects,
							       &oid, &type, &కొలత);
				స్థిరరాశి అక్షరం *లక్ష్యం;

				ఒకవేళ (!బఫరు)
					die(_("unable to read %s"), oid_గమ్యం_hex(&oid));

				ఒకవేళ (!skip_ఉపసర్గ(బఫరు, "object ", &లక్ష్యం) ||
				    get_oid_hex_algop(లక్ష్యం, &blob_oid,
						      &hash_algos[oid.algo]))
					die("%s not a valid tag", oid_గమ్యం_hex(&oid));
				విడుదల(బఫరు);
			} లేకపోతే
				oidcpy(&blob_oid, &oid);

			ఒకవేళ (odb_read_object_info(the_repository->objects,
						 &blob_oid, ఖాళీ) == OBJ_BLOB) {
				ret = ప్రవాహం_blob(&blob_oid);
				వెళ్ళు cleanup;
			}
			/*
			 * we attempted to dereference a tag to a blob
			 * and failed; there may be new dereference
			 * mechanisms this code is not aware of.
			 * fall-back to the usual case.
			 */
		}
		buf = odb_read_object_peeled(the_repository->objects, &oid,
					     exp_type_id, &కొలత, ఖాళీ);

		ఒకవేళ (use_mailmap)
			buf = replace_idents_using_mailmap(buf, &కొలత);
		ఆపు;
	}
	అప్రమేయ:
		die("git cat-file: unknown option: %s", exp_type);
	}

	ఒకవేళ (!buf)
		die("git cat-file %s: bad file", obj_name);

	write_or_die(1, buf, కొలత);
	ret = 0;
cleanup:
	విడుదల(buf);
	object_context_release(&obj_context);
	తిరిగి ret;
}

నిర్మాణం expand_దత్తాంశం {
	నిర్మాణం object_id oid;
	గణన object_type type;
	కొలత_t కొలత;
	గుర్తులేని పొట్టి బహుళకం;
	off_t disk_కొలత;
	స్థిరరాశి అక్షరం *rest;
	నిర్మాణం object_id delta_పునాది_oid;

	/*
	 * If mark_query is true, we do not expand anything, but rather
	 * just mark the object_info with items we wish to query.
	 */
	పూర్ణాంకం mark_query;

	/*
	 * Whether to split the input on whitespace before feeding it to
	 * get_sha1; this is decided during the mark_query phase based on
	 * whether we have a %(rest) token in our format.
	 */
	పూర్ణాంకం విభజించు_on_whitespace;

	/*
	 * After a mark_query run, this object_info is set up to be
	 * passed to odb_read_object_info_extended. It will point to the data
	 * elements above, so you can retrieve the response from there.
	 */
	నిర్మాణం object_info info;

	/*
	 * This flag will be true if the requested batch format and options
	 * don't require us to call oid_object_info, which can then be
	 * optimized out.
	 */
	గుర్తులేని skip_object_info : 1;

	/*
	 * Flags about when an object info is being fetched from remote.
	 */
	గుర్తులేని is_remote:1;

	/*
	 * List of atoms (i.e. "objectsize") that the server supports. Built
	 * from the server's object-info advertised capabilities.
	 */
	నిర్మాణం string_list remote_allowed_atoms;
};

#నిర్వచించు EXPAND_దత్తాంశం_INIT  {  \
	.బహుళకం = S_IFINVALID, \
	.type = OBJ_BAD,     \
	.remote_allowed_atoms = STRING_LIST_INIT_NODUP, \
}

స్థిర పూర్ణాంకం is_atom(స్థిరరాశి అక్షరం *atom, స్థిరరాశి అక్షరం *s, పూర్ణాంకం slen)
{
	పూర్ణాంకం alen = అక్షరశ్రేణి_పొడవు(atom);
	తిరిగి alen == slen && !memcmp(atom, s, alen);
}

స్థిర పూర్ణాంకం expand_atom(నిర్మాణం strbuf *sb, స్థిరరాశి అక్షరం *atom, పూర్ణాంకం len,
		       నిర్మాణం expand_దత్తాంశం *దత్తాంశం)
{
	ఒకవేళ (దత్తాంశం->is_remote) {
		కొలత_t i;
		కొరకు (i = 0; i < దత్తాంశం->remote_allowed_atoms.nr; i++)
			ఒకవేళ (is_atom(దత్తాంశం->remote_allowed_atoms.వస్తువులు[i].string,
				    atom, len))
				ఆపు;
		ఒకవేళ (i == దత్తాంశం->remote_allowed_atoms.nr)
			తిరిగి 1;
	}

	ఒకవేళ (is_atom("objectname", atom, len)) {
		ఒకవేళ (!దత్తాంశం->mark_query)
			strbuf_కూడు_oid_hex(sb, &దత్తాంశం->oid);
	} లేకపోతే ఒకవేళ (is_atom("objecttype", atom, len)) {
		ఒకవేళ (దత్తాంశం->mark_query) {
			దత్తాంశం->info.typep = &దత్తాంశం->type;
		} లేకపోతే {
			స్థిరరాశి అక్షరం *t = type_name(దత్తాంశం->type);
			strbuf_addstr(sb, t ? t : "");
		}
	} లేకపోతే ఒకవేళ (is_atom("objectsize", atom, len)) {
		ఒకవేళ (దత్తాంశం->mark_query)
			దత్తాంశం->info.sizep = &దత్తాంశం->కొలత;
		లేకపోతే
			strbuf_కూడు_uint(sb, దత్తాంశం->కొలత);
	} లేకపోతే ఒకవేళ (is_atom("objectsize:disk", atom, len)) {
		ఒకవేళ (దత్తాంశం->mark_query)
			దత్తాంశం->info.disk_sizep = &దత్తాంశం->disk_కొలత;
		లేకపోతే
			strbuf_కూడు_uint(sb, దత్తాంశం->disk_కొలత);
	} లేకపోతే ఒకవేళ (is_atom("rest", atom, len)) {
		ఒకవేళ (దత్తాంశం->mark_query)
			దత్తాంశం->విభజించు_on_whitespace = 1;
		లేకపోతే ఒకవేళ (దత్తాంశం->rest)
			strbuf_addstr(sb, దత్తాంశం->rest);
	} లేకపోతే ఒకవేళ (is_atom("deltabase", atom, len)) {
		ఒకవేళ (దత్తాంశం->mark_query)
			దత్తాంశం->info.delta_పునాది_oid = &దత్తాంశం->delta_పునాది_oid;
		లేకపోతే
			strbuf_కూడు_oid_hex(sb, &దత్తాంశం->delta_పునాది_oid);
	} లేకపోతే ఒకవేళ (is_atom("objectmode", atom, len)) {
		ఒకవేళ (!దత్తాంశం->mark_query && !(S_IFINVALID == దత్తాంశం->బహుళకం))
			strbuf_addf(sb, "%06o", దత్తాంశం->బహుళకం);
	} లేకపోతే
		తిరిగి 0;
	తిరిగి 1;
}

స్థిర శూన్య expand_format(నిర్మాణం strbuf *sb, స్థిరరాశి అక్షరం *ప్రారంభం,
			  నిర్మాణం expand_దత్తాంశం *దత్తాంశం)
{
	అయితే (strbuf_expand_అడుగు(sb, &ప్రారంభం)) {
		స్థిరరాశి అక్షరం *చివర;

		ఒకవేళ (skip_ఉపసర్గ(ప్రారంభం, "%", &ప్రారంభం) || *ప్రారంభం != '(')
			strbuf_addch(sb, '%');
		లేకపోతే ఒకవేళ ((చివర = strchr(ప్రారంభం + 1, ')')) &&
			 expand_atom(sb, ప్రారంభం + 1, చివర - ప్రారంభం - 1, దత్తాంశం))
			ప్రారంభం = చివర + 1;
		లేకపోతే
			strbuf_expand_bad_format(ప్రారంభం, "cat-file");
	}
}

స్థిర శూన్య batch_write(నిర్మాణం batch_options *opt, స్థిరరాశి శూన్య *దత్తాంశం, పూర్ణాంకం len)
{
	ఒకవేళ (opt->బఫరు_output) {
		ఒకవేళ (fwrite(దత్తాంశం, 1, len, stdout) != len)
			die_errno("unable to write to stdout");
	} లేకపోతే
		write_or_die(1, దత్తాంశం, len);
}

స్థిర శూన్య print_object_or_die(నిర్మాణం batch_options *opt, నిర్మాణం expand_దత్తాంశం *దత్తాంశం)
{
	స్థిరరాశి నిర్మాణం object_id *oid = &దత్తాంశం->oid;

	ప్రతిజ్ఞా(దత్తాంశం->info.typep);

	ఒకవేళ (దత్తాంశం->type == OBJ_BLOB) {
		ఒకవేళ (opt->బఫరు_output)
			fflush(stdout);
		ఒకవేళ (opt->transform_బహుళకం) {
			అక్షరం *contents;
			కొలత_t కొలత;

			ఒకవేళ (!దత్తాంశం->rest)
				die("missing path for '%s'", oid_గమ్యం_hex(oid));

			ఒకవేళ (opt->transform_బహుళకం == 'w') {
				ఒకవేళ (filter_object(దత్తాంశం->rest, 0100644, oid,
						  &contents, &కొలత))
					die("could not convert '%s' %s",
					    oid_గమ్యం_hex(oid), దత్తాంశం->rest);
			} లేకపోతే ఒకవేళ (opt->transform_బహుళకం == 'c') {
				గణన object_type type;
				గుర్తులేని దీర్ఘ కొలత_ul = 0;
				ఒకవేళ (textconv_object(the_repository,
						    దత్తాంశం->rest, 0100644, oid,
						    1, &contents, &కొలత_ul))
					కొలత = కొలత_ul;
				లేకపోతే
					contents = odb_read_object(the_repository->objects,
								   oid, &type, &కొలత);
				ఒకవేళ (!contents)
					die("could not convert '%s' %s",
					    oid_గమ్యం_hex(oid), దత్తాంశం->rest);
			} లేకపోతే
				BUG("invalid transform_mode: %c", opt->transform_బహుళకం);
			batch_write(opt, contents, కొలత);
			విడుదల(contents);
		} లేకపోతే {
			ప్రవాహం_blob(oid);
		}
	}
	లేకపోతే {
		గణన object_type type;
		కొలత_t కొలత;
		శూన్య *contents;

		contents = odb_read_object(the_repository->objects, oid,
					   &type, &కొలత);
		ఒకవేళ (!contents)
			die("object %s disappeared", oid_గమ్యం_hex(oid));

		ఒకవేళ (use_mailmap)
			contents = replace_idents_using_mailmap(contents, &కొలత);

		ఒకవేళ (type != దత్తాంశం->type)
			die("object %s changed type!?", oid_గమ్యం_hex(oid));
		ఒకవేళ (దత్తాంశం->info.sizep && కొలత != దత్తాంశం->కొలత && !use_mailmap)
			die("object %s changed size!?", oid_గమ్యం_hex(oid));

		batch_write(opt, contents, కొలత);
		విడుదల(contents);
	}
}

స్థిర శూన్య print_default_format(నిర్మాణం strbuf *scratch, నిర్మాణం expand_దత్తాంశం *దత్తాంశం,
				 నిర్మాణం batch_options *opt)
{
	strbuf_కూడు_oid_hex(scratch, &దత్తాంశం->oid);
	strbuf_addch(scratch, ' ');
	strbuf_addstr(scratch, type_name(దత్తాంశం->type));
	strbuf_addch(scratch, ' ');
	strbuf_కూడు_uint(scratch, దత్తాంశం->కొలత);
	strbuf_addch(scratch, opt->output_delim);
}

స్థిర శూన్య report_object_స్థితి(నిర్మాణం batch_options *opt,
				 స్థిరరాశి అక్షరం *obj_name,
				 స్థిరరాశి నిర్మాణం object_id *oid,
				 స్థిరరాశి అక్షరం *స్థితి_2)
{
	రాయి("%s %s%c", obj_name ? obj_name : oid_గమ్యం_hex(oid),
	       స్థితి_2, opt->output_delim);
	fflush(stdout);
}

/*
 * If "pack" is non-NULL, then "offset" is the byte offset within the pack from
 * which the object may be accessed (though note that we may also rely on
 * data->oid, too). If "pack" is NULL, then offset is ignored.
 */
స్థిర శూన్య batch_object_write(స్థిరరాశి అక్షరం *obj_name,
			       నిర్మాణం strbuf *scratch,
			       నిర్మాణం batch_options *opt,
			       నిర్మాణం expand_దత్తాంశం *దత్తాంశం,
			       నిర్మాణం packed_git *pack,
			       off_t స్థానభ్రంశం)
{
	ఒకవేళ (!దత్తాంశం->skip_object_info) {
		పూర్ణాంకం ret;

		ఒకవేళ (use_mailmap ||
		    opt->objects_filter.choice == LOFC_BLOB_NONE ||
		    opt->objects_filter.choice == LOFC_BLOB_హద్దు ||
		    opt->objects_filter.choice == LOFC_OBJECT_TYPE)
			దత్తాంశం->info.typep = &దత్తాంశం->type;
		ఒకవేళ (opt->objects_filter.choice == LOFC_BLOB_హద్దు)
			దత్తాంశం->info.sizep = &దత్తాంశం->కొలత;

		ఒకవేళ (pack)
			ret = packed_object_info(ఖాళీ, pack, స్థానభ్రంశం, &దత్తాంశం->info);
		లేకపోతే
			ret = odb_read_object_info_extended(the_repository->objects,
							    &దత్తాంశం->oid, &దత్తాంశం->info,
							    OBJECT_INFO_LOOKUP_REPLACE);
		ఒకవేళ (ret < 0) {
			ఒకవేళ (దత్తాంశం->బహుళకం == S_IFGITLINK)
				report_object_స్థితి(opt, ఖాళీ, &దత్తాంశం->oid, "submodule");
			లేకపోతే
				report_object_స్థితి(opt, obj_name, &దత్తాంశం->oid, "missing");
			తిరిగి;
		}

		ఎంపిక (opt->objects_filter.choice) {
		స్థితి LOFC_DISABLED:
			ఆపు;
		స్థితి LOFC_BLOB_NONE:
			ఒకవేళ (దత్తాంశం->type == OBJ_BLOB) {
				ఒకవేళ (!opt->all_objects)
					report_object_స్థితి(opt, obj_name,
							     &దత్తాంశం->oid, "excluded");
				తిరిగి;
			}
			ఆపు;
		స్థితి LOFC_BLOB_హద్దు:
			ఒకవేళ (దత్తాంశం->type == OBJ_BLOB &&
			    దత్తాంశం->కొలత >= opt->objects_filter.blob_హద్దు_విలువ) {
				ఒకవేళ (!opt->all_objects)
					report_object_స్థితి(opt, obj_name,
							     &దత్తాంశం->oid, "excluded");
				తిరిగి;
			}
			ఆపు;
		స్థితి LOFC_OBJECT_TYPE:
			ఒకవేళ (దత్తాంశం->type != opt->objects_filter.object_type) {
				ఒకవేళ (!opt->all_objects)
					report_object_స్థితి(opt, obj_name,
							     &దత్తాంశం->oid, "excluded");
				తిరిగి;
			}
			ఆపు;
		అప్రమేయ:
			BUG("unsupported objects filter");
		}

		ఒకవేళ (use_mailmap && (దత్తాంశం->type == OBJ_COMMIT || దత్తాంశం->type == OBJ_ట్యాగ్)) {
			అక్షరం *buf = ఖాళీ;

			buf = odb_read_object(the_repository->objects, &దత్తాంశం->oid,
					      &దత్తాంశం->type, &దత్తాంశం->కొలత);
			ఒకవేళ (!buf)
				die(_("unable to read %s"), oid_గమ్యం_hex(&దత్తాంశం->oid));
			buf = replace_idents_using_mailmap(buf, &దత్తాంశం->కొలత);

			విడుదల(buf);
		}
	}

	strbuf_reset(scratch);

	ఒకవేళ (!opt->format) {
		print_default_format(scratch, దత్తాంశం, opt);
	} లేకపోతే {
		expand_format(scratch, opt->format, దత్తాంశం);
		strbuf_addch(scratch, opt->output_delim);
	}

	batch_write(opt, scratch->buf, scratch->len);

	ఒకవేళ (opt->batch_బహుళకం == BATCH_బహుళకం_CONTENTS) {
		print_object_or_die(opt, దత్తాంశం);
		batch_write(opt, &opt->output_delim, 1);
	}
}

స్థిర శూన్య batch_one_object(స్థిరరాశి అక్షరం *obj_name,
			     నిర్మాణం strbuf *scratch,
			     నిర్మాణం batch_options *opt,
			     నిర్మాణం expand_దత్తాంశం *దత్తాంశం)
{
	నిర్మాణం object_context ctx = {0};
	పూర్ణాంకం flags =
		GET_OID_HASH_ANY |
		(opt->follow_symlinks ? GET_OID_FOLLOW_SYMLINKS : 0);
	గణన get_oid_ఫలితం ఫలితం;

	ఫలితం = get_oid_with_context(the_repository, obj_name,
				      flags, &దత్తాంశం->oid, &ctx);
	ఒకవేళ (ఫలితం != దొరికింది) {
		ఎంపిక (ఫలితం) {
		స్థితి MISSING_OBJECT:
			report_object_స్థితి(opt, obj_name, &దత్తాంశం->oid, "missing");
			ఆపు;
		స్థితి SHORT_NAME_AMBIGUOUS:
			report_object_స్థితి(opt, obj_name, &దత్తాంశం->oid, "ambiguous");
			ఆపు;
		స్థితి DANGLING_SYMLINK:
			రాయి("dangling %"PRIuMAX"%c%s%c",
			       (uintmax_t)అక్షరశ్రేణి_పొడవు(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ఆపు;
		స్థితి SYMLINK_LOOP:
			రాయి("loop %"PRIuMAX"%c%s%c",
			       (uintmax_t)అక్షరశ్రేణి_పొడవు(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ఆపు;
		స్థితి NOT_DIR:
			రాయి("notdir %"PRIuMAX"%c%s%c",
			       (uintmax_t)అక్షరశ్రేణి_పొడవు(obj_name),
			       opt->output_delim, obj_name, opt->output_delim);
			ఆపు;
		అప్రమేయ:
			BUG("unknown get_sha1_with_context result %d\n",
			       ఫలితం);
			ఆపు;
		}
		fflush(stdout);

		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (ctx.బహుళకం == 0) {
		రాయి("symlink %"PRIuMAX"%c%s%c",
		       (uintmax_t)ctx.symlink_మార్గం.len,
		       opt->output_delim, ctx.symlink_మార్గం.buf, opt->output_delim);
		fflush(stdout);
		వెళ్ళు బహిర్గతం;
	}

	దత్తాంశం->బహుళకం = ctx.బహుళకం;
	batch_object_write(obj_name, scratch, opt, దత్తాంశం, ఖాళీ, 0);

బహిర్గతం:
	object_context_release(&ctx);
}

స్థిర పూర్ణాంకం get_remote_info(పూర్ణాంకం argc,
			   స్థిరరాశి అక్షరం **argv,
			   నిర్మాణం fetch_object_info_results *results,
			   నిర్మాణం oid_array *object_info_oids)
{
	పూర్ణాంకం retval = 0;
	నిర్మాణం remote *remote = ఖాళీ;
	నిర్మాణం object_id oid;
	నిర్మాణం transport *gtransport;

	remote = remote_get(argv[0]);
	ఒకవేళ (!remote)
		die(_("must supply valid remote when using remote-object-info"));

	oid_array_clear(object_info_oids);
	కొరకు (కొలత_t i = 1; i < argc; i++) {
		ఒకవేళ (get_oid_hex(argv[i], &oid)) {
			కొలత_t len = అక్షరశ్రేణి_పొడవు(argv[i]);

			ఒకవేళ (len < the_hash_algo->hexsz && len >= 4) {
				కొలత_t j;
				కొరకు (j = 0; j < len; j++)
					ఒకవేళ (!isxdigit(argv[i][j]))
						ఆపు;
				ఒకవేళ (j == len)
					die(_("remote-object-info does not support "
					      "short oids, %d characters required"),
					    (పూర్ణాంకం)the_hash_algo->hexsz);
			}
			die(_("not a valid object name '%s'"), argv[i]);
		}
		oid_array_append(object_info_oids, &oid);
	}

	ఒకవేళ (!object_info_oids->nr)
		die(_("remote-object-info requires objects"));

	gtransport = transport_get(remote, ఖాళీ);

	ఒకవేళ (!gtransport->smart_options) {
		retval = -1;
		వెళ్ళు cleanup;
	}

	retval = transport_fetch_object_info(gtransport, object_info_oids,
					     results);
cleanup:
	transport_disconnect(gtransport);
	తిరిగి retval;
}

నిర్మాణం object_cb_దత్తాంశం {
	నిర్మాణం batch_options *opt;
	నిర్మాణం expand_దత్తాంశం *expand;
	నిర్మాణం oidset *చూసిన;
	నిర్మాణం strbuf *scratch;
};

స్థిర పూర్ణాంకం batch_object_cb(స్థిరరాశి నిర్మాణం object_id *oid, శూన్య *vdata)
{
	నిర్మాణం object_cb_దత్తాంశం *దత్తాంశం = vdata;
	oidcpy(&దత్తాంశం->expand->oid, oid);
	batch_object_write(ఖాళీ, దత్తాంశం->scratch, దత్తాంశం->opt, దత్తాంశం->expand,
			   ఖాళీ, 0);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం collect_object(స్థిరరాశి నిర్మాణం object_id *oid,
			  నిర్మాణం packed_git *pack వాడని,
			  off_t స్థానభ్రంశం వాడని,
			  శూన్య *దత్తాంశం)
{
	oid_array_append(దత్తాంశం, oid);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం batch_unordered_object(స్థిరరాశి నిర్మాణం object_id *oid,
				  నిర్మాణం packed_git *pack,
				  off_t స్థానభ్రంశం,
				  శూన్య *vdata)
{
	నిర్మాణం object_cb_దత్తాంశం *దత్తాంశం = vdata;

	ఒకవేళ (oidset_చొప్పించు(దత్తాంశం->చూసిన, oid))
		తిరిగి 0;

	oidcpy(&దత్తాంశం->expand->oid, oid);
	batch_object_write(ఖాళీ, దత్తాంశం->scratch, దత్తాంశం->opt, దత్తాంశం->expand,
			   pack, స్థానభ్రంశం);
	తిరిగి 0;
}

రకనిర్వచనం శూన్య (*parse_cmd_fn_t)(నిర్మాణం batch_options *, స్థిరరాశి అక్షరం *,
			       నిర్మాణం strbuf *, నిర్మాణం expand_దత్తాంశం *);

నిర్మాణం queued_cmd {
	parse_cmd_fn_t fn;
	అక్షరం *పంక్తి;
};

స్థిర శూన్య parse_cmd_contents(నిర్మాణం batch_options *opt,
			     స్థిరరాశి అక్షరం *పంక్తి,
			     నిర్మాణం strbuf *output,
			     నిర్మాణం expand_దత్తాంశం *దత్తాంశం)
{
	గణన object_type *saved_typep = దత్తాంశం->info.typep;
	దత్తాంశం->info.typep = &దత్తాంశం->type;
	opt->batch_బహుళకం = BATCH_బహుళకం_CONTENTS;
	batch_one_object(పంక్తి, output, opt, దత్తాంశం);
	దత్తాంశం->info.typep = saved_typep;
}

స్థిర శూన్య parse_cmd_info(నిర్మాణం batch_options *opt,
			   స్థిరరాశి అక్షరం *పంక్తి,
			   నిర్మాణం strbuf *output,
			   నిర్మాణం expand_దత్తాంశం *దత్తాంశం)
{
	opt->batch_బహుళకం = BATCH_బహుళకం_INFO;
	batch_one_object(పంక్తి, output, opt, దత్తాంశం);
}

స్థిర శూన్య parse_cmd_mailmap(నిర్మాణం batch_options *opt వాడని,
			      స్థిరరాశి అక్షరం *పంక్తి,
			      నిర్మాణం strbuf *output వాడని,
			      నిర్మాణం expand_దత్తాంశం *దత్తాంశం వాడని)
{
	use_mailmap = git_parse_maybe_bool(పంక్తి);

	ఒకవేళ (use_mailmap < 0)
		die(_("mailmap: invalid boolean '%s'"), పంక్తి);

	ఒకవేళ (use_mailmap)
		ఎక్కించు_mailmap();
}

స్థిర శూన్య parse_cmd_remote_object_info(నిర్మాణం batch_options *opt,
					 స్థిరరాశి అక్షరం *పంక్తి, నిర్మాణం strbuf *output,
					 నిర్మాణం expand_దత్తాంశం *దత్తాంశం)
{
	పూర్ణాంకం లెక్క;
	స్థిరరాశి అక్షరం **argv;
	అక్షరం *పంక్తి_గమ్యం_విభజించు;
	నిర్మాణం fetch_object_info_results results = FETCH_OBJECT_INFO_RESULTS_INIT;
	నిర్మాణం oid_array object_info_oids = OID_ARRAY_INIT;

	ఒకవేళ (అక్షరశ్రేణి_పొడవు(పంక్తి) >= MAX_REMOTE_OBJ_INFO_పంక్తి)
		die(_("remote-object-info command too long"));

	పంక్తి_గమ్యం_విభజించు = xstrdup(పంక్తి);
	లెక్క = విభజించు_cmdline(పంక్తి_గమ్యం_విభజించు, &argv);
	ఒకవేళ (లెక్క < 0)
		die(_("remote-object-info: failed to parse command line: %s"),
		    విభజించు_cmdline_strerror(లెక్క));
	ఒకవేళ (లెక్క - 1 > MAX_ALLOWED_OBJ_హద్దు)
		die(_("remote-object-info supports at most %d objects"),
		    MAX_ALLOWED_OBJ_హద్దు);

	ఒకవేళ (దత్తాంశం->info.sizep)
		results.కోరుతుంది_కొలత = 1;
	ఒకవేళ (దత్తాంశం->info.typep)
		results.కోరుతుంది_type = 1;

	ఒకవేళ (get_remote_info(లెక్క, argv, &results, &object_info_oids))
		die(_("failed to get object info from the remote: %s"), argv[0]);

	string_list_clear(&దత్తాంశం->remote_allowed_atoms, 0);
	string_list_append(&దత్తాంశం->remote_allowed_atoms, "objectname");
	ఒకవేళ (results.sizes)
		string_list_append(&దత్తాంశం->remote_allowed_atoms, "objectsize");
	ఒకవేళ (results.types)
		string_list_append(&దత్తాంశం->remote_allowed_atoms, "objecttype");

	దత్తాంశం->skip_object_info = 1;
	కొరకు (కొలత_t i = 0; i < results.nr; i++) {
		దత్తాంశం->oid = object_info_oids.oid[i];

		ఒకవేళ (results.unrecognized[i]) {
			report_object_స్థితి(opt, oid_గమ్యం_hex(&దత్తాంశం->oid),
					     &దత్తాంశం->oid, "missing");
			కొనసాగు;
		}

		/*
		 * When reaching here, it means remote-object-info can retrieve
		 * information from server without downloading them.
		 */
		ఒకవేళ (results.sizes)
			దత్తాంశం->కొలత = results.sizes[i];

		ఒకవేళ (results.types)
			దత్తాంశం->type = results.types[i];

		opt->batch_బహుళకం = BATCH_బహుళకం_INFO;
		దత్తాంశం->is_remote = 1;
		batch_object_write(argv[i + 1], output, opt, దత్తాంశం, ఖాళీ, 0);
		దత్తాంశం->is_remote = 0;
	}
	దత్తాంశం->skip_object_info = 0;

	free_fetch_object_info_results(&results);
	విడుదల(పంక్తి_గమ్యం_విభజించు);
	విడుదల(argv);
	oid_array_clear(&object_info_oids);
}

స్థిర శూన్య dispatch_calls(నిర్మాణం batch_options *opt,
		నిర్మాణం strbuf *output,
		నిర్మాణం expand_దత్తాంశం *దత్తాంశం,
		నిర్మాణం queued_cmd *cmd,
		కొలత_t nr)
{
	ఒకవేళ (!opt->బఫరు_output)
		die(_("flush is only for --buffer mode"));

	కొరకు (కొలత_t i = 0; i < nr; i++)
		cmd[i].fn(opt, cmd[i].పంక్తి, output, దత్తాంశం);

	fflush(stdout);
}

స్థిర శూన్య free_cmds(నిర్మాణం queued_cmd *cmd, కొలత_t *nr)
{
	కొరకు (కొలత_t i = 0; i < *nr; i++)
		FREE_AND_NULL(cmd[i].పంక్తి);

	*nr = 0;
}


స్థిర స్థిరరాశి నిర్మాణం parse_cmd {
	స్థిరరాశి అక్షరం *name;
	parse_cmd_fn_t fn;
	గుర్తులేని takes_వాదనలు;
} commands[] = {
	{ "contents", parse_cmd_contents, 1 },
	{ "flush", ఖాళీ, 0 },
	{ "info", parse_cmd_info, 1 },
	{ "mailmap", parse_cmd_mailmap, 1 },
	{ "remote-object-info", parse_cmd_remote_object_info, 1 },
};

స్థిర శూన్య batch_objects_command(నిర్మాణం batch_options *opt,
				    నిర్మాణం strbuf *output,
				    నిర్మాణం expand_దత్తాంశం *దత్తాంశం)
{
	నిర్మాణం strbuf ఆదానం = STRBUF_INIT;
	నిర్మాణం queued_cmd *queued_cmd = ఖాళీ;
	కొలత_t alloc = 0, nr = 0;

	అయితే (strbuf_getdelim_strip_crlf(&ఆదానం, stdin, opt->ఆదానం_delim) != EOF) {
		స్థిరరాశి నిర్మాణం parse_cmd *cmd = ఖాళీ;
		స్థిరరాశి అక్షరం *p = ఖాళీ, *cmd_చివర;
		నిర్మాణం queued_cmd call = {0};

		ఒకవేళ (!ఆదానం.len)
			die(_("empty command in input"));
		ఒకవేళ (ఖాళీనా(*ఆదానం.buf))
			die(_("whitespace before command: '%s'"), ఆదానం.buf);

		కొరకు (కొలత_t i = 0; i < ARRAY_కొలత(commands); i++) {
			ఒకవేళ (!skip_ఉపసర్గ(ఆదానం.buf, commands[i].name, &cmd_చివర))
				కొనసాగు;

			cmd = &commands[i];
			ఒకవేళ (cmd->takes_వాదనలు) {
				ఒకవేళ (*cmd_చివర != ' ')
					die(_("%s requires arguments"),
					    commands[i].name);

				p = cmd_చివర + 1;
			} లేకపోతే ఒకవేళ (*cmd_చివర) {
				die(_("%s takes no arguments"),
				    commands[i].name);
			}

			ఆపు;
		}

		ఒకవేళ (!cmd)
			die(_("unknown command: '%s'"), ఆదానం.buf);

		ఒకవేళ (!strcmp(cmd->name, "flush")) {
			dispatch_calls(opt, output, దత్తాంశం, queued_cmd, nr);
			free_cmds(queued_cmd, &nr);
		} లేకపోతే ఒకవేళ (!opt->బఫరు_output) {
			cmd->fn(opt, p, output, దత్తాంశం);
		} లేకపోతే {
			ALLOC_GROW(queued_cmd, nr + 1, alloc);
			call.fn = cmd->fn;
			call.పంక్తి = xstrdup_or_null(p);
			queued_cmd[nr++] = call;
		}
	}

	ఒకవేళ (opt->బఫరు_output &&
	    nr &&
	    !git_env_bool("GIT_TEST_CAT_FILE_NO_FLUSH_ON_EXIT", 0)) {
		dispatch_calls(opt, output, దత్తాంశం, queued_cmd, nr);
		free_cmds(queued_cmd, &nr);
	}

	free_cmds(queued_cmd, &nr);
	విడుదల(queued_cmd);
	strbuf_release(&ఆదానం);
}

#నిర్వచించు DEFAULT_FORMAT "%(objectname) %(objecttype) %(objectsize)"

రకనిర్వచనం పూర్ణాంకం (*for_each_object_fn)(స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం packed_git *pack,
				  off_t స్థానభ్రంశం, శూన్య *దత్తాంశం);

నిర్మాణం for_each_object_payload {
	for_each_object_fn callback;
	శూన్య *payload;
};

స్థిర పూర్ణాంకం batch_one_object_oi(స్థిరరాశి నిర్మాణం object_id *oid,
			       నిర్మాణం object_info *oi,
			       శూన్య *_payload)
{
	నిర్మాణం for_each_object_payload *payload = _payload;
	ఒకవేళ (oi && oi->ఆధారం_infop->ఆధారం->type == ODB_ఆధారం_PACKED)
		తిరిగి payload->callback(oid, oi->ఆధారం_infop->u.packed.pack,
					 oi->ఆధారం_infop->u.packed.స్థానభ్రంశం,
					 payload->payload);
	తిరిగి payload->callback(oid, ఖాళీ, 0, payload->payload);
}

స్థిర శూన్య batch_each_object(నిర్మాణం batch_options *opt,
			      for_each_object_fn callback,
			      గుర్తులేని flags,
			      శూన్య *_payload)
{
	నిర్మాణం for_each_object_payload payload = {
		.callback = callback,
		.payload = _payload,
	};
	నిర్మాణం odb_ఆధారం_info ఆధారం_info;
	నిర్మాణం object_info oi = {
		.ఆధారం_infop = &ఆధారం_info,
	};
	నిర్మాణం odb_for_each_object_options opts = {
		.flags = flags,
		.filter = &opt->objects_filter,
	};

	odb_for_each_object_ext(the_repository->objects, &oi,
				batch_one_object_oi, &payload, &opts);
}

స్థిర పూర్ణాంకం batch_objects(నిర్మాణం batch_options *opt)
{
	నిర్మాణం strbuf ఆదానం = STRBUF_INIT;
	నిర్మాణం strbuf output = STRBUF_INIT;
	నిర్మాణం expand_దత్తాంశం దత్తాంశం = EXPAND_దత్తాంశం_INIT;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);
	పూర్ణాంకం save_warning;
	పూర్ణాంకం retval = 0;

	/*
	 * Expand once with our special mark_query flag, which will prime the
	 * object_info to be handed to odb_read_object_info_extended for each
	 * object.
	 */
	దత్తాంశం.mark_query = 1;
	expand_format(&output,
		      opt->format ? opt->format : DEFAULT_FORMAT,
		      &దత్తాంశం);
	దత్తాంశం.mark_query = 0;
	strbuf_release(&output);
	ఒకవేళ (opt->transform_బహుళకం)
		దత్తాంశం.విభజించు_on_whitespace = 1;

	ఒకవేళ (opt->format && !strcmp(opt->format, DEFAULT_FORMAT))
		opt->format = ఖాళీ;
	/*
	 * If we are printing out the object, then always fill in the type,
	 * since we will want to decide whether or not to stream.
	 */
	ఒకవేళ (opt->batch_బహుళకం == BATCH_బహుళకం_CONTENTS)
		దత్తాంశం.info.typep = &దత్తాంశం.type;

	ఒకవేళ (opt->all_objects) {
		నిర్మాణం object_cb_దత్తాంశం cb;
		నిర్మాణం object_info ఖాళీలు = OBJECT_INFO_INIT;

		ఒకవేళ (!memcmp(&దత్తాంశం.info, &ఖాళీలు, పరిమాణం(ఖాళీలు)) &&
		    opt->objects_filter.choice == LOFC_DISABLED)
			దత్తాంశం.skip_object_info = 1;

		ఒకవేళ (repo_has_promisor_remote(the_repository))
			warning("This repository uses promisor remotes. Some objects may not be loaded.");

		disable_replace_refs();

		cb.opt = opt;
		cb.expand = &దత్తాంశం;
		cb.scratch = &output;

		ఒకవేళ (opt->unordered) {
			నిర్మాణం oidset చూసిన = OIDSET_INIT;

			cb.చూసిన = &చూసిన;

			batch_each_object(opt, batch_unordered_object,
					  ODB_FOR_EACH_OBJECT_PACK_క్రమం, &cb);

			oidset_clear(&చూసిన);
		} లేకపోతే {
			నిర్మాణం oid_array sa = OID_ARRAY_INIT;

			batch_each_object(opt, collect_object, 0, &sa);
			oid_array_for_each_unique(&sa, batch_object_cb, &cb);

			oid_array_clear(&sa);
		}

		strbuf_release(&output);
		తిరిగి 0;
	}

	/*
	 * We are going to call get_sha1 on a potentially very large number of
	 * objects. In most large cases, these will be actual object sha1s. The
	 * cost to double-check that each one is not also a ref (just so we can
	 * warn) ends up dwarfing the actual cost of the object lookups
	 * themselves. We can work around it by just turning off the warning.
	 */
	save_warning = cfg->warn_on_object_refname_ambiguity;
	cfg->warn_on_object_refname_ambiguity = 0;

	ఒకవేళ (opt->batch_బహుళకం == BATCH_బహుళకం_వరుస_AND_DISPATCH) {
		batch_objects_command(opt, &output, &దత్తాంశం);
		వెళ్ళు cleanup;
	}

	అయితే (strbuf_getdelim_strip_crlf(&ఆదానం, stdin, opt->ఆదానం_delim) != EOF) {
		ఒకవేళ (దత్తాంశం.విభజించు_on_whitespace) {
			/*
			 * Split at first whitespace, tying off the beginning
			 * of the string and saving the remainder (or NULL) in
			 * data.rest.
			 */
			అక్షరం *p = strpbrk(ఆదానం.buf, " \t");
			ఒకవేళ (p) {
				అయితే (*p && strchr(" \t", *p))
					*p++ = '\0';
			}
			దత్తాంశం.rest = p;
		}

		batch_one_object(ఆదానం.buf, &output, opt, &దత్తాంశం);
	}

 cleanup:
	strbuf_release(&ఆదానం);
	strbuf_release(&output);
	string_list_clear(&దత్తాంశం.remote_allowed_atoms, 0);
	cfg->warn_on_object_refname_ambiguity = save_warning;
	తిరిగి retval;
}

స్థిర పూర్ణాంకం git_cat_file_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			       స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (userdiff_config(var, విలువ) < 0)
		తిరిగి -1;

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

స్థిర పూర్ణాంకం batch_option_callback(స్థిరరాశి నిర్మాణం option *opt,
				 స్థిరరాశి అక్షరం *arg,
				 పూర్ణాంకం unset)
{
	నిర్మాణం batch_options *bo = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (bo->enabled) {
		తిరిగి error(_("only one batch option may be specified"));
	}

	bo->enabled = 1;

	ఒకవేళ (!strcmp(opt->long_name, "batch"))
		bo->batch_బహుళకం = BATCH_బహుళకం_CONTENTS;
	లేకపోతే ఒకవేళ (!strcmp(opt->long_name, "batch-check"))
		bo->batch_బహుళకం = BATCH_బహుళకం_INFO;
	లేకపోతే ఒకవేళ (!strcmp(opt->long_name, "batch-command"))
		bo->batch_బహుళకం = BATCH_బహుళకం_వరుస_AND_DISPATCH;
	లేకపోతే
		BUG("%s given to batch-option-callback", opt->long_name);

	bo->format = arg;

	తిరిగి 0;
}

పూర్ణాంకం cmd_cat_file(పూర్ణాంకం argc,
		 స్థిరరాశి అక్షరం **argv,
		 స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం opt = 0;
	పూర్ణాంకం opt_cw = 0;
	పూర్ణాంకం opt_epts = 0;
	స్థిరరాశి అక్షరం *exp_type = ఖాళీ, *obj_name = ఖాళీ;
	నిర్మాణం batch_options batch = {
		.objects_filter = LIST_OBJECTS_FILTER_INIT,
	};
	పూర్ణాంకం unknown_type = 0;
	పూర్ణాంకం ఆదానం_nul_terminated = 0;
	పూర్ణాంకం nul_terminated = 0;
	పూర్ణాంకం ret;

	స్థిరరాశి అక్షరం * స్థిరరాశి builtin_catfile_usage[] = {
		N_("git cat-file <type> <object>"),
		N_("git cat-file (-e | -p | -t | -s) <object>"),
		N_("git cat-file (--textconv | --filters)\n"
		   "             [<rev>:<path|tree-ish> | --path=<path|tree-ish> <rev>]"),
		N_("git cat-file (--batch | --batch-check | --batch-command) [--batch-all-objects]\n"
		   "             [--buffer] [--follow-symlinks] [--unordered]\n"
		   "             [--textconv | --filters] [-Z]"),
		ఖాళీ
	};
	స్థిరరాశి నిర్మాణం option options[] = {
		/* Simple queries */
		OPT_సమూహం(N_("Check object existence or emit object contents")),
		OPT_CMDMODE('e', ఖాళీ, &opt,
			    N_("check if <object> exists"), 'e'),
		OPT_CMDMODE('p', ఖాళీ, &opt, N_("pretty-print <object> content"), 'p'),

		OPT_సమూహం(N_("Emit [broken] object attributes")),
		OPT_CMDMODE('t', ఖాళీ, &opt, N_("show object type (one of 'blob', 'tree', 'commit', 'tag', ...)"), 't'),
		OPT_CMDMODE('s', ఖాళీ, &opt, N_("show object size"), 's'),
		OPT_HIDDEN_BOOL(0, "allow-unknown-type", &unknown_type,
			  N_("historical option -- no-op")),
		OPT_BOOL(0, "use-mailmap", &use_mailmap, N_("use mail map file")),
		OPT_ALIAS(0, "mailmap", "use-mailmap"),
		/* Batch mode */
		OPT_సమూహం(N_("Batch objects requested on stdin (or --batch-all-objects)")),
		OPT_CALLBACK_F(0, "batch", &batch, N_("format"),
			N_("show full <object> or <rev> contents"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CALLBACK_F(0, "batch-check", &batch, N_("format"),
			N_("like --batch, but don't emit <contents>"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_BOOL_F('z', ఖాళీ, &ఆదానం_nul_terminated, N_("stdin is NUL-terminated"),
			PARSE_OPT_HIDDEN),
		OPT_BOOL('Z', ఖాళీ, &nul_terminated, N_("stdin and stdout is NUL-terminated")),
		OPT_CALLBACK_F(0, "batch-command", &batch, N_("format"),
			N_("read commands from stdin"),
			PARSE_OPT_OPTARG | PARSE_OPT_NONEG,
			batch_option_callback),
		OPT_CMDMODE(0, "batch-all-objects", &opt,
			    N_("with --batch[-check]: ignores stdin, batches all known objects"), 'b'),
		/* Batch-specific options */
		OPT_సమూహం(N_("Change or optimize batch output")),
		OPT_BOOL(0, "buffer", &batch.బఫరు_output, N_("buffer --batch output")),
		OPT_BOOL(0, "follow-symlinks", &batch.follow_symlinks,
			 N_("follow in-tree symlinks")),
		OPT_BOOL(0, "unordered", &batch.unordered,
			 N_("do not order objects before emitting them")),
		/* Textconv options, stand-ole*/
		OPT_సమూహం(N_("Emit object (blob or tree) with conversion or filter (stand-alone, or with batch)")),
		OPT_CMDMODE(0, "textconv", &opt,
			    N_("run textconv on object's content"), 'c'),
		OPT_CMDMODE(0, "filters", &opt,
			    N_("run filters on object's content"), 'w'),
		OPT_STRING(0, "path", &force_మార్గం, N_("blob|tree"),
			   N_("use a <path> for (--textconv | --filters); Not with 'batch'")),
		OPT_PARSE_LIST_OBJECTS_FILTER(&batch.objects_filter),
		OPT_చివర()
	};

	repo_config(the_repository, git_cat_file_config, ఖాళీ);

	batch.బఫరు_output = -1;

	argc = parse_options(argc, argv, ఉపసర్గ, options, builtin_catfile_usage, 0);
	opt_cw = (opt == 'c' || opt == 'w');
	opt_epts = (opt == 'e' || opt == 'p' || opt == 't' || opt == 's');

	ఒకవేళ (use_mailmap)
		ఎక్కించు_mailmap();

	ఎంపిక (batch.objects_filter.choice) {
	స్థితి LOFC_DISABLED:
		ఆపు;
	స్థితి LOFC_BLOB_NONE:
	స్థితి LOFC_BLOB_హద్దు:
	స్థితి LOFC_OBJECT_TYPE:
		ఒకవేళ (!batch.enabled)
			usage(_("objects filter only supported in batch mode"));
		ఆపు;
	అప్రమేయ:
		usagef(_("objects filter not supported: '%s'"),
		       list_object_filter_config_name(batch.objects_filter.choice));
	}

	/* --batch-all-objects? */
	ఒకవేళ (opt == 'b')
		batch.all_objects = 1;

	/* Option compatibility */
	ఒకవేళ (force_మార్గం && !opt_cw)
		usage_సందేశం_optf(_("'%s=<%s>' needs '%s' or '%s'"),
			       builtin_catfile_usage, options,
			       "--path", _("path|tree-ish"), "--filters",
			       "--textconv");

	/* Option compatibility with batch mode */
	ఒకవేళ (batch.enabled)
		;
	లేకపోతే ఒకవేళ (batch.follow_symlinks)
		usage_సందేశం_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--follow-symlinks");
	లేకపోతే ఒకవేళ (batch.బఫరు_output >= 0)
		usage_సందేశం_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--buffer");
	లేకపోతే ఒకవేళ (batch.all_objects)
		usage_సందేశం_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "--batch-all-objects");
	లేకపోతే ఒకవేళ (ఆదానం_nul_terminated)
		usage_సందేశం_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-z");
	లేకపోతే ఒకవేళ (nul_terminated)
		usage_సందేశం_optf(_("'%s' requires a batch mode"), builtin_catfile_usage,
			       options, "-Z");

	batch.ఆదానం_delim = batch.output_delim = '\n';
	ఒకవేళ (ఆదానం_nul_terminated)
		batch.ఆదానం_delim = '\0';
	ఒకవేళ (nul_terminated)
		batch.ఆదానం_delim = batch.output_delim = '\0';

	/* Batch defaults */
	ఒకవేళ (batch.బఫరు_output < 0)
		batch.బఫరు_output = batch.all_objects;

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	/* Return early if we're in batch mode? */
	ఒకవేళ (batch.enabled) {
		ఒకవేళ (opt_cw)
			batch.transform_బహుళకం = opt;
		లేకపోతే ఒకవేళ (opt && opt != 'b')
			usage_సందేశం_optf(_("'-%c' is incompatible with batch mode"),
				       builtin_catfile_usage, options, opt);
		లేకపోతే ఒకవేళ (argc)
			usage_సందేశం_opt(_("batch modes take no arguments"),
				      builtin_catfile_usage, options);

		ret = batch_objects(&batch);
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (opt) {
		ఒకవేళ (!argc && opt == 'c')
			usage_సందేశం_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--textconv");
		లేకపోతే ఒకవేళ (!argc && opt == 'w')
			usage_సందేశం_optf(_("<rev> required with '%s'"),
				       builtin_catfile_usage, options,
				       "--filters");
		లేకపోతే ఒకవేళ (!argc && opt_epts)
			usage_సందేశం_optf(_("<object> required with '-%c'"),
				       builtin_catfile_usage, options, opt);
		లేకపోతే ఒకవేళ (argc == 1)
			obj_name = argv[0];
		లేకపోతే
			usage_సందేశం_opt(_("too many arguments"), builtin_catfile_usage,
				      options);
	} లేకపోతే ఒకవేళ (!argc) {
		usage_with_options(builtin_catfile_usage, options);
	} లేకపోతే ఒకవేళ (argc != 2) {
		usage_సందేశం_optf(_("only two arguments allowed in <type> <object> mode, not %d"),
			      builtin_catfile_usage, options, argc);
	} లేకపోతే ఒకవేళ (argc) {
		exp_type = argv[0];
		obj_name = argv[1];
	}

	ret = cat_one_file(opt, exp_type, obj_name);

బహిర్గతం:
	list_objects_filter_release(&batch.objects_filter);
	తిరిగి ret;
}
