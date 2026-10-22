#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree-walk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdiff-interface.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "help.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "merge-ort.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blob.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "merge-blobs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

ਸਥਿਰ ਪੂਰਨ ਸਤਰ_termination = '\n';

ਬਣਤਰ ਮਿਲਾਓ_list {
	ਬਣਤਰ ਮਿਲਾਓ_list *ਅਗਲਾ;
	ਬਣਤਰ ਮਿਲਾਓ_list *link;	/* other stages for this object */

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stage : 2;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
	ਬਣਤਰ blob *blob;
};

ਸਥਿਰ ਬਣਤਰ ਮਿਲਾਓ_list *ਮਿਲਾਓ_ਨਤੀਜਾ, **ਮਿਲਾਓ_ਨਤੀਜਾ_ਸਿਰਾ = &ਮਿਲਾਓ_ਨਤੀਜਾ;

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ਮਿਲਾਓ_ਇੰਦਰਾਜ(ਬਣਤਰ ਮਿਲਾਓ_list *ਇੰਦਰਾਜ)
{
	*ਮਿਲਾਓ_ਨਤੀਜਾ_ਸਿਰਾ = ਇੰਦਰਾਜ;
	ਮਿਲਾਓ_ਨਤੀਜਾ_ਸਿਰਾ = &ਇੰਦਰਾਜ->ਅਗਲਾ;
}

ਸਥਿਰ ਖਾਲੀ trivial_ਮਿਲਾਓ_trees(ਬਣਤਰ ਰੁੱਖ_desc t[3], ਅਚਲ ਅੱਖਰ *ਅਧਾਰ);

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *explanation(ਬਣਤਰ ਮਿਲਾਓ_list *ਇੰਦਰਾਜ)
{
	ਚੋਣ (ਇੰਦਰਾਜ->stage) {
	ਹਾਲਤ 0:
		ਵਾਪਸ "merged";
	ਹਾਲਤ 3:
		ਵਾਪਸ "added in remote";
	ਹਾਲਤ 2:
		ਜੇ (ਇੰਦਰਾਜ->link)
			ਵਾਪਸ "added in both";
		ਵਾਪਸ "added in local";
	}

	/* Existed in base */
	ਇੰਦਰਾਜ = ਇੰਦਰਾਜ->link;
	ਜੇ (!ਇੰਦਰਾਜ)
		ਵਾਪਸ "removed in both";

	ਜੇ (ਇੰਦਰਾਜ->link)
		ਵਾਪਸ "changed in both";

	ਜੇ (ਇੰਦਰਾਜ->stage == 3)
		ਵਾਪਸ "removed in local";
	ਵਾਪਸ "removed in remote";
}

ਸਥਿਰ ਖਾਲੀ *ਨਤੀਜਾ(ਬਣਤਰ ਮਿਲਾਓ_list *ਇੰਦਰਾਜ, ਮਾਪ_t *ਮਾਪ)
{
	ਗਿਣਤੀ object_type type;
	ਬਣਤਰ blob *ਅਧਾਰ, *our, *their;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ = ਇੰਦਰਾਜ->ਰਸਤਾ;

	ਜੇ (!ਇੰਦਰਾਜ->stage)
		ਵਾਪਸ odb_read_object(the_repository->objects,
				       &ਇੰਦਰਾਜ->blob->object.oid, &type,
				       ਮਾਪ);
	ਅਧਾਰ = ਨਿਰਾ;
	ਜੇ (ਇੰਦਰਾਜ->stage == 1) {
		ਅਧਾਰ = ਇੰਦਰਾਜ->blob;
		ਇੰਦਰਾਜ = ਇੰਦਰਾਜ->link;
	}
	our = ਨਿਰਾ;
	ਜੇ (ਇੰਦਰਾਜ && ਇੰਦਰਾਜ->stage == 2) {
		our = ਇੰਦਰਾਜ->blob;
		ਇੰਦਰਾਜ = ਇੰਦਰਾਜ->link;
	}
	their = ਨਿਰਾ;
	ਜੇ (ਇੰਦਰਾਜ)
		their = ਇੰਦਰਾਜ->blob;
	ਵਾਪਸ ਮਿਲਾਓ_blobs(the_repository->ਸੂਚਕਾਂਕ, ਰਸਤਾ,
			   ਅਧਾਰ, our, their, ਮਾਪ);
}

ਸਥਿਰ ਖਾਲੀ *origin(ਬਣਤਰ ਮਿਲਾਓ_list *ਇੰਦਰਾਜ, ਮਾਪ_t *ਮਾਪ)
{
	ਗਿਣਤੀ object_type type;
	ਜਦੋਂ (ਇੰਦਰਾਜ) {
		ਜੇ (ਇੰਦਰਾਜ->stage == 2)
			ਵਾਪਸ odb_read_object(the_repository->objects,
					       &ਇੰਦਰਾਜ->blob->object.oid,
					       &type, ਮਾਪ);
		ਇੰਦਰਾਜ = ਇੰਦਰਾਜ->link;
	}
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ show_outf(ਖਾਲੀ *priv ਅਣਵਰਤਿਆ, mmfile_t *mb, ਪੂਰਨ nbuf)
{
	ਪੂਰਨ i;
	ਲਈ (i = 0; i < nbuf; i++)
		ਲਿਖੋ("%.*s", (ਪੂਰਨ) mb[i].ਮਾਪ, mb[i].ptr);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ show_diff(ਬਣਤਰ ਮਿਲਾਓ_list *ਇੰਦਰਾਜ)
{
	ਮਾਪ_t ਮਾਪ;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .ਬਾਹਰਲਾ_ਸਤਰ = show_outf };

	memset(&xpp, 0, ਆਕਾਰ(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, ਆਕਾਰ(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(ਇੰਦਰਾਜ, &ਮਾਪ);
	ਜੇ (!src.ptr)
		ਮਾਪ = 0;
	src.ਮਾਪ = ਮਾਪ;
	dst.ptr = ਨਤੀਜਾ(ਇੰਦਰਾਜ, &ਮਾਪ);
	ਜੇ (!dst.ptr)
		ਮਾਪ = 0;
	dst.ਮਾਪ = ਮਾਪ;
	ਜੇ (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	ਮੁਕਤ(src.ptr);
	ਮੁਕਤ(dst.ptr);
}

ਸਥਿਰ ਖਾਲੀ show_ਨਤੀਜਾ_list(ਬਣਤਰ ਮਿਲਾਓ_list *ਇੰਦਰਾਜ)
{
	ਲਿਖੋ("%s\n", explanation(ਇੰਦਰਾਜ));
	ਕਰੋ {
		ਬਣਤਰ ਮਿਲਾਓ_list *link = ਇੰਦਰਾਜ->link;
		ਸਥਿਰ ਅਚਲ ਅੱਖਰ *desc[4] = { "result", "base", "our", "their" };
		ਲਿਖੋ("  %-6s %o %s %s\n", desc[ਇੰਦਰਾਜ->stage], ਇੰਦਰਾਜ->ਬਹੁਲਕ, oid_ਮੰਜ਼ਿਲ_hex(&ਇੰਦਰਾਜ->blob->object.oid), ਇੰਦਰਾਜ->ਰਸਤਾ);
		ਇੰਦਰਾਜ = link;
	} ਜਦੋਂ (ਇੰਦਰਾਜ);
}

ਸਥਿਰ ਖਾਲੀ show_ਨਤੀਜਾ(ਖਾਲੀ)
{
	ਬਣਤਰ ਮਿਲਾਓ_list *walk;

	walk = ਮਿਲਾਓ_ਨਤੀਜਾ;
	ਜਦੋਂ (walk) {
		show_ਨਤੀਜਾ_list(walk);
		show_diff(walk);
		walk = walk->ਅਗਲਾ;
	}
}

/* An empty entry never compares same, not even to another empty entry */
ਸਥਿਰ ਪੂਰਨ same_ਇੰਦਰਾਜ(ਬਣਤਰ name_ਇੰਦਰਾਜ *a, ਬਣਤਰ name_ਇੰਦਰਾਜ *b)
{
	ਵਾਪਸ	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->ਬਹੁਲਕ == b->ਬਹੁਲਕ;
}

ਸਥਿਰ ਪੂਰਨ both_ਖਾਲੀ_ਥਾਵਾਂ(ਬਣਤਰ name_ਇੰਦਰਾਜ *a, ਬਣਤਰ name_ਇੰਦਰਾਜ *b)
{
	ਵਾਪਸ is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

ਸਥਿਰ ਬਣਤਰ ਮਿਲਾਓ_list *create_ਇੰਦਰਾਜ(ਬਿਨਾਨਿਸ਼ਾਨੀ stage, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ, ਅਚਲ ਬਣਤਰ object_id *oid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਣਤਰ ਮਿਲਾਓ_list *res = xcalloc(1, ਆਕਾਰ(*res));

	res->stage = stage;
	res->ਰਸਤਾ = ਰਸਤਾ;
	res->ਬਹੁਲਕ = ਬਹੁਲਕ;
	res->blob = lookup_blob(the_repository, oid);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਅੱਖਰ *traverse_ਰਸਤਾ(ਅਚਲ ਬਣਤਰ traverse_info *info, ਅਚਲ ਬਣਤਰ name_ਇੰਦਰਾਜ *n)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_ਰਸਤਾ(&buf, info, n->ਰਸਤਾ, n->pathlen);
	ਵਾਪਸ strbuf_detach(&buf, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ resolve(ਅਚਲ ਬਣਤਰ traverse_info *info, ਬਣਤਰ name_ਇੰਦਰਾਜ *ours, ਬਣਤਰ name_ਇੰਦਰਾਜ *ਨਤੀਜਾ)
{
	ਬਣਤਰ ਮਿਲਾਓ_list *orig, *final;
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;

	/* If it's already ours, don't bother showing it */
	ਜੇ (!ours)
		ਵਾਪਸ;

	ਰਸਤਾ = traverse_ਰਸਤਾ(info, ਨਤੀਜਾ);
	orig = create_ਇੰਦਰਾਜ(2, ours->ਬਹੁਲਕ, &ours->oid, ਰਸਤਾ);
	final = create_ਇੰਦਰਾਜ(0, ਨਤੀਜਾ->ਬਹੁਲਕ, &ਨਤੀਜਾ->oid, ਰਸਤਾ);

	final->link = orig;

	ਜੋੜੋ_ਮਿਲਾਓ_ਇੰਦਰਾਜ(final);
}

ਸਥਿਰ ਖਾਲੀ unresolved_directory(ਅਚਲ ਬਣਤਰ traverse_info *info,
				 ਬਣਤਰ name_ਇੰਦਰਾਜ n[3])
{
	ਬਣਤਰ repository *r = the_repository;
	ਅੱਖਰ *newbase;
	ਬਣਤਰ name_ਇੰਦਰਾਜ *p;
	ਬਣਤਰ ਰੁੱਖ_desc t[3];
	ਖਾਲੀ *buf0, *buf1, *buf2;

	ਲਈ (p = n; p < n + 3; p++) {
		ਜੇ (p->ਬਹੁਲਕ && S_ISDIR(p->ਬਹੁਲਕ))
			ਰੋਕੋ;
	}
	ਜੇ (n + 3 <= p)
		ਵਾਪਸ; /* there is no tree here */

	newbase = traverse_ਰਸਤਾ(info, p);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਇੰਦਰਾਜ_OID(e) (((e)->ਬਹੁਲਕ && S_ISDIR((e)->ਬਹੁਲਕ)) ? &(e)->oid : ਨਿਰਾ)
	buf0 = fill_ਰੁੱਖ_descriptor(r, t + 0, ਇੰਦਰਾਜ_OID(n + 0));
	buf1 = fill_ਰੁੱਖ_descriptor(r, t + 1, ਇੰਦਰਾਜ_OID(n + 1));
	buf2 = fill_ਰੁੱਖ_descriptor(r, t + 2, ਇੰਦਰਾਜ_OID(n + 2));
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਇੰਦਰਾਜ_OID

	trivial_ਮਿਲਾਓ_trees(t, newbase);

	ਮੁਕਤ(buf0);
	ਮੁਕਤ(buf1);
	ਮੁਕਤ(buf2);
	ਮੁਕਤ(newbase);
}


ਸਥਿਰ ਬਣਤਰ ਮਿਲਾਓ_list *link_ਇੰਦਰਾਜ(ਬਿਨਾਨਿਸ਼ਾਨੀ stage, ਅਚਲ ਬਣਤਰ traverse_info *info, ਬਣਤਰ name_ਇੰਦਰਾਜ *n, ਬਣਤਰ ਮਿਲਾਓ_list *ਇੰਦਰਾਜ)
{
	ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
	ਬਣਤਰ ਮਿਲਾਓ_list *link;

	ਜੇ (!n->ਬਹੁਲਕ)
		ਵਾਪਸ ਇੰਦਰਾਜ;
	ਜੇ (ਇੰਦਰਾਜ)
		ਰਸਤਾ = ਇੰਦਰਾਜ->ਰਸਤਾ;
	ਨਹੀਂਤਾਂ
		ਰਸਤਾ = traverse_ਰਸਤਾ(info, n);
	link = create_ਇੰਦਰਾਜ(stage, n->ਬਹੁਲਕ, &n->oid, ਰਸਤਾ);
	link->link = ਇੰਦਰਾਜ;
	ਵਾਪਸ link;
}

ਸਥਿਰ ਖਾਲੀ unresolved(ਅਚਲ ਬਣਤਰ traverse_info *info, ਬਣਤਰ name_ਇੰਦਰਾਜ n[3])
{
	ਬਣਤਰ ਮਿਲਾਓ_list *ਇੰਦਰਾਜ = ਨਿਰਾ;
	ਪੂਰਨ i;
	ਬਿਨਾਨਿਸ਼ਾਨੀ dirmask = 0, ਨਕਾਬ = 0;

	ਲਈ (i = 0; i < 3; i++) {
		ਨਕਾਬ |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		ਜੇ (!n[i].ਬਹੁਲਕ || S_ISDIR(n[i].ਬਹੁਲਕ))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	ਜੇ (dirmask == ਨਕਾਬ)
		ਵਾਪਸ;

	ਜੇ (n[2].ਬਹੁਲਕ && !S_ISDIR(n[2].ਬਹੁਲਕ))
		ਇੰਦਰਾਜ = link_ਇੰਦਰਾਜ(3, info, n + 2, ਇੰਦਰਾਜ);
	ਜੇ (n[1].ਬਹੁਲਕ && !S_ISDIR(n[1].ਬਹੁਲਕ))
		ਇੰਦਰਾਜ = link_ਇੰਦਰਾਜ(2, info, n + 1, ਇੰਦਰਾਜ);
	ਜੇ (n[0].ਬਹੁਲਕ && !S_ISDIR(n[0].ਬਹੁਲਕ))
		ਇੰਦਰਾਜ = link_ਇੰਦਰਾਜ(1, info, n + 0, ਇੰਦਰਾਜ);

	ਜੋੜੋ_ਮਿਲਾਓ_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ);
}

/*
 * Merge two trees together (t[1] and t[2]), using a common base (t[0])
 * as the origin.
 *
 * This walks the (sorted) trees in lock-step, checking every possible
 * name. Note that directories automatically sort differently from other
 * files (see "base_name_compare"), so you'll never see file/directory
 * conflicts, because they won't ever compare the same.
 *
 * IOW, if a directory changes to a filename, it will automatically be
 * seen as the directory going away, and the filename being created.
 *
 * Think of this as a three-way diff.
 *
 * The output will be either:
 *  - successful merge
 *	 "0 mode sha1 filename"
 *    NOTE NOTE NOTE! FIXME! We really really need to walk the index
 *    in parallel with this too!
 *
 *  - conflict:
 *	"1 mode sha1 filename"
 *	"2 mode sha1 filename"
 *	"3 mode sha1 filename"
 *    where not all of the 1/2/3 lines may exist, of course.
 *
 * The successful merge rules are the same as for the three-way merge
 * in git-read-tree.
 */
ਸਥਿਰ ਪੂਰਨ threeway_callback(ਪੂਰਨ n ਅਣਵਰਤਿਆ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨਕਾਬ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ dirmask ਅਣਵਰਤਿਆ,
			     ਬਣਤਰ name_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ, ਬਣਤਰ traverse_info *info)
{
	/* Same in both? */
	ਜੇ (same_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ+1, ਇੰਦਰਾਜ+2) || both_ਖਾਲੀ_ਥਾਵਾਂ(ਇੰਦਰਾਜ+1, ਇੰਦਰਾਜ+2)) {
		/* Modified, added or removed identically */
		resolve(info, ਨਿਰਾ, ਇੰਦਰਾਜ+1);
		ਵਾਪਸ ਨਕਾਬ;
	}

	ਜੇ (same_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ+0, ਇੰਦਰਾਜ+1)) {
		ਜੇ (!is_null_oid(&ਇੰਦਰਾਜ[2].oid) && !S_ISDIR(ਇੰਦਰਾਜ[2].ਬਹੁਲਕ)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, ਇੰਦਰਾਜ+1, ਇੰਦਰਾਜ+2);
			ਵਾਪਸ ਨਕਾਬ;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	ਜੇ (same_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ+0, ਇੰਦਰਾਜ+2) || both_ਖਾਲੀ_ਥਾਵਾਂ(ਇੰਦਰਾਜ+0, ਇੰਦਰਾਜ+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, ਨਿਰਾ, ਇੰਦਰਾਜ+1);
		ਵਾਪਸ ਨਕਾਬ;
	}

	unresolved(info, ਇੰਦਰਾਜ);
	ਵਾਪਸ ਨਕਾਬ;
}

ਸਥਿਰ ਖਾਲੀ trivial_ਮਿਲਾਓ_trees(ਬਣਤਰ ਰੁੱਖ_desc t[3], ਅਚਲ ਅੱਖਰ *ਅਧਾਰ)
{
	ਬਣਤਰ traverse_info info;

	ਤਿਆਰੀ_traverse_info(&info, ਅਧਾਰ);
	info.fn = threeway_callback;
	traverse_trees(the_repository->ਸੂਚਕਾਂਕ, 3, t, &info);
}

ਸਥਿਰ ਖਾਲੀ *get_ਰੁੱਖ_descriptor(ਬਣਤਰ repository *r,
				 ਬਣਤਰ ਰੁੱਖ_desc *desc,
				 ਅਚਲ ਅੱਖਰ *rev)
{
	ਬਣਤਰ object_id oid;
	ਖਾਲੀ *buf;

	ਜੇ (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_ਰੁੱਖ_descriptor(r, desc, &oid);
	ਜੇ (!buf)
		die("%s is not a tree", rev);
	ਵਾਪਸ buf;
}

ਸਥਿਰ ਪੂਰਨ trivial_ਮਿਲਾਓ(ਅਚਲ ਅੱਖਰ *ਅਧਾਰ,
			 ਅਚਲ ਅੱਖਰ *branch1,
			 ਅਚਲ ਅੱਖਰ *branch2)
{
	ਬਣਤਰ repository *r = the_repository;
	ਬਣਤਰ ਰੁੱਖ_desc t[3];
	ਖਾਲੀ *buf1, *buf2, *buf3;

	buf1 = get_ਰੁੱਖ_descriptor(r, t+0, ਅਧਾਰ);
	buf2 = get_ਰੁੱਖ_descriptor(r, t+1, branch1);
	buf3 = get_ਰੁੱਖ_descriptor(r, t+2, branch2);
	trivial_ਮਿਲਾਓ_trees(t, "");
	ਮੁਕਤ(buf1);
	ਮੁਕਤ(buf2);
	ਮੁਕਤ(buf3);

	show_ਨਤੀਜਾ();
	ਵਾਪਸ 0;
}

ਗਿਣਤੀ ਬਹੁਲਕ {
	ਬਹੁਲਕ_UNKNOWN,
	ਬਹੁਲਕ_TRIVIAL,
	ਬਹੁਲਕ_REAL,
};

ਬਣਤਰ ਮਿਲਾਓ_ਰੁੱਖ_options {
	ਪੂਰਨ ਬਹੁਲਕ;
	ਪੂਰਨ allow_unrelated_histories;
	ਪੂਰਨ show_messages;
	ਪੂਰਨ name_only;
	ਪੂਰਨ use_stdin;
	ਬਣਤਰ ਮਿਲਾਓ_options ਮਿਲਾਓ_options;
};

ਸਥਿਰ ਪੂਰਨ real_ਮਿਲਾਓ(ਬਣਤਰ ਮਿਲਾਓ_ਰੁੱਖ_options *o,
		      ਅਚਲ ਅੱਖਰ *ਮਿਲਾਓ_ਅਧਾਰ,
		      ਅਚਲ ਅੱਖਰ *branch1, ਅਚਲ ਅੱਖਰ *branch2,
		      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਬਣਤਰ commit *parent1, *parent2;
	ਬਣਤਰ commit_list *ਮਿਲਾਓ_bases = ਨਿਰਾ;
	ਬਣਤਰ ਮਿਲਾਓ_ਨਤੀਜਾ ਨਤੀਜਾ = { 0 };
	ਪੂਰਨ show_messages = o->show_messages;
	ਬਣਤਰ ਮਿਲਾਓ_options opt;

	copy_ਮਿਲਾਓ_options(&opt, &o->ਮਿਲਾਓ_options);
	opt.show_rename_ਤਰੱਕੀ = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	ਜੇ (ਮਿਲਾਓ_ਅਧਾਰ) {
		ਬਣਤਰ ਰੁੱਖ *ਅਧਾਰ_ਰੁੱਖ, *parent1_ਰੁੱਖ, *parent2_ਰੁੱਖ;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		ਬਣਤਰ object_id ਅਧਾਰ_oid, ਸਿਰ_oid, ਮਿਲਾਓ_oid;

		ਜੇ (repo_get_oid_treeish(the_repository, ਮਿਲਾਓ_ਅਧਾਰ, &ਅਧਾਰ_oid))
			die(_("could not parse as tree '%s'"), ਮਿਲਾਓ_ਅਧਾਰ);
		ਅਧਾਰ_ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository,
						     &ਅਧਾਰ_oid);
		ਜੇ (!ਅਧਾਰ_ਰੁੱਖ)
			die(_("unable to read tree (%s)"), oid_ਮੰਜ਼ਿਲ_hex(&ਅਧਾਰ_oid));
		ਜੇ (repo_get_oid_treeish(the_repository, branch1, &ਸਿਰ_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository,
							&ਸਿਰ_oid);
		ਜੇ (!parent1_ਰੁੱਖ)
			die(_("unable to read tree (%s)"), oid_ਮੰਜ਼ਿਲ_hex(&ਸਿਰ_oid));
		ਜੇ (repo_get_oid_treeish(the_repository, branch2, &ਮਿਲਾਓ_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository,
							&ਮਿਲਾਓ_oid);
		ਜੇ (!parent2_ਰੁੱਖ)
			die(_("unable to read tree (%s)"), oid_ਮੰਜ਼ਿਲ_hex(&ਮਿਲਾਓ_oid));

		opt.ancestor = ਮਿਲਾਓ_ਅਧਾਰ;
		ਮਿਲਾਓ_incore_nonrecursive(&opt, ਅਧਾਰ_ਰੁੱਖ, parent1_ਰੁੱਖ, parent2_ਰੁੱਖ, &ਨਤੀਜਾ);
	} ਨਹੀਂਤਾਂ {
		parent1 = get_ਮਿਲਾਓ_ਮਾਪਾ(branch1);
		ਜੇ (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_ਮਿਲਾਓ_ਮਾਪਾ(branch2);
		ਜੇ (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		ਜੇ (repo_get_ਮਿਲਾਓ_bases(the_repository, parent1,
					 parent2, &ਮਿਲਾਓ_bases) < 0)
			ਬਾਹਰ_ਨਿਕਲੋ(128);
		ਜੇ (!ਮਿਲਾਓ_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		ਮਿਲਾਓ_bases = commit_list_ਉਲਟ(ਮਿਲਾਓ_bases);
		ਮਿਲਾਓ_incore_recursive(&opt, ਮਿਲਾਓ_bases, parent1, parent2, &ਨਤੀਜਾ);
		commit_list_free(ਮਿਲਾਓ_bases);
	}

	ਜੇ (ਨਤੀਜਾ.clean < 0)
		die(_("failure to merge"));

	ਜੇ (o->ਮਿਲਾਓ_options.mergeability_only)
		ਜਾਓ cleanup;

	ਜੇ (show_messages == -1)
		show_messages = !ਨਤੀਜਾ.clean;

	ਜੇ (o->use_stdin)
		ਲਿਖੋ("%d%c", ਨਤੀਜਾ.clean, ਸਤਰ_termination);
	ਲਿਖੋ("%s%c", oid_ਮੰਜ਼ਿਲ_hex(&ਨਤੀਜਾ.ਰੁੱਖ->object.oid), ਸਤਰ_termination);
	ਜੇ (!ਨਤੀਜਾ.clean) {
		ਬਣਤਰ string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ਅਚਲ ਅੱਖਰ *ਆਖਰੀ = ਨਿਰਾ;

		ਮਿਲਾਓ_get_conflicted_files(&ਨਤੀਜਾ, &conflicted_files);
		ਲਈ (ਮਾਪ_t i = 0; i < conflicted_files.nr; i++) {
			ਅਚਲ ਅੱਖਰ *name = conflicted_files.ਚੀਜ਼ਾਂ[i].string;
			ਬਣਤਰ stage_info *c = conflicted_files.ਚੀਜ਼ਾਂ[i].util;
			ਜੇ (!o->name_only)
				ਲਿਖੋ("%06o %s %d\t",
				       c->ਬਹੁਲਕ, oid_ਮੰਜ਼ਿਲ_hex(&c->oid), c->stage);
			ਨਹੀਂਤਾਂ ਜੇ (ਆਖਰੀ && !strcmp(ਆਖਰੀ, name))
				ਜਾਰੀ;
			write_name_quoted_relative(
				name, ਅਗੇਤਰ, stdout, ਸਤਰ_termination);
			ਆਖਰੀ = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	ਜੇ (show_messages) {
		ਅੱਖਰ_ਲਿਖੋ(ਸਤਰ_termination);
		ਮਿਲਾਓ_display_update_messages(&opt, ਸਤਰ_termination == '\0',
					      &ਨਤੀਜਾ);
	}
	ਜੇ (o->use_stdin)
		ਅੱਖਰ_ਲਿਖੋ(ਸਤਰ_termination);

cleanup:
	ਮਿਲਾਓ_finalize(&opt, &ਨਤੀਜਾ);
	clear_ਮਿਲਾਓ_options(&opt);
	ਵਾਪਸ !ਨਤੀਜਾ.clean; /* result.clean < 0 handled above */
}

ਪੂਰਨ cmd_ਮਿਲਾਓ_ਰੁੱਖ(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ ਮਿਲਾਓ_ਰੁੱਖ_options o = { .show_messages = -1 };
	ਬਣਤਰ strvec xopts = STRVEC_INIT;
	ਪੂਰਨ expected_ਬਚਿਆ_argc;
	ਪੂਰਨ original_argc;
	ਅਚਲ ਅੱਖਰ *ਮਿਲਾਓ_ਅਧਾਰ = ਨਿਰਾ;
	ਪੂਰਨ ret;
	ਪੂਰਨ quiet = 0;

	ਅਚਲ ਅੱਖਰ * ਅਚਲ ਮਿਲਾਓ_ਰੁੱਖ_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		ਨਿਰਾ
	};
	ਬਣਤਰ option mt_options[] = {
		OPT_CMDMODE(0, "write-tree", &o.ਬਹੁਲਕ,
			    N_("do a real merge instead of a trivial merge"),
			    ਬਹੁਲਕ_REAL),
		OPT_CMDMODE(0, "trivial-merge", &o.ਬਹੁਲਕ,
			    N_("do a trivial merge only"), ਬਹੁਲਕ_TRIVIAL),
		OPT_BOOL(0, "messages", &o.show_messages,
			 N_("also show informational/conflict messages")),
		OPT_BOOL_F(0, "quiet",
			   &quiet,
			   N_("suppress all output; only exit status wanted"),
			   PARSE_OPT_NONEG),
		OPT_ਸੈੱਟ_INT('z', ਨਿਰਾ, &ਸਤਰ_termination,
			    N_("separate paths with the NUL character"), '\0'),
		OPT_BOOL_F(0, "name-only",
			   &o.name_only,
			   N_("list filenames without modes/oids/stages"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "allow-unrelated-histories",
			   &o.allow_unrelated_histories,
			   N_("allow merging unrelated histories"),
			   PARSE_OPT_NONEG),
		OPT_BOOL_F(0, "stdin",
			   &o.use_stdin,
			   N_("perform multiple merges, one per line of input"),
			   PARSE_OPT_NONEG),
		OPT_STRING(0, "merge-base",
			   &ਮਿਲਾਓ_ਅਧਾਰ,
			   N_("tree-ish"),
			   N_("specify a merge-base for the merge")),
		OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
			N_("option for selected merge strategy")),
		OPT_ਸਿਰਾ()
	};

	/* Init merge options */
	init_basic_ਮਿਲਾਓ_options(&o.ਮਿਲਾਓ_options, the_repository);

	/* Parse arguments */
	original_argc = argc - 1; /* ignoring argv[0] */
	argc = parse_options(argc, argv, ਅਗੇਤਰ, mt_options,
			     ਮਿਲਾਓ_ਰੁੱਖ_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	ਜੇ (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.ਮਿਲਾਓ_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !ਸਤਰ_termination, "-z");

	ਜੇ (xopts.nr && o.ਬਹੁਲਕ == ਬਹੁਲਕ_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	ਲਈ (ਮਾਪ_t x = 0; x < xopts.nr; x++)
		ਜੇ (parse_ਮਿਲਾਓ_opt(&o.ਮਿਲਾਓ_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	ਜੇ (o.use_stdin) {
		ਬਣਤਰ strbuf buf = STRBUF_INIT;

		ਜੇ (o.ਬਹੁਲਕ == ਬਹੁਲਕ_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		ਜੇ (ਮਿਲਾਓ_ਅਧਾਰ)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		ਸਤਰ_termination = '\0';
		ਜਦੋਂ (strbuf_getline_lf(&buf, stdin) != EOF) {
			ਬਣਤਰ string_list ਵੰਡੋ = STRING_LIST_INIT_NODUP;
			ਅਚਲ ਅੱਖਰ *ਨਿਵੇਸ਼_ਮਿਲਾਓ_ਅਧਾਰ = ਨਿਰਾ;

			string_list_ਵੰਡੋ_ਵਿੱਚ_ਰੱਖੋ_f(&ਵੰਡੋ, buf.buf, " ", -1,
						     STRING_LIST_ਵੰਡੋ_TRIM);

			ਜੇ (ਵੰਡੋ.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			ਜੇ (!strcmp(ਵੰਡੋ.ਚੀਜ਼ਾਂ[1].string, "--")) {
				ਨਿਵੇਸ਼_ਮਿਲਾਓ_ਅਧਾਰ = ਵੰਡੋ.ਚੀਜ਼ਾਂ[0].string;
			}

			ਜੇ (ਨਿਵੇਸ਼_ਮਿਲਾਓ_ਅਧਾਰ && ਵੰਡੋ.nr == 4) {
				real_ਮਿਲਾਓ(&o, ਨਿਵੇਸ਼_ਮਿਲਾਓ_ਅਧਾਰ,
					   ਵੰਡੋ.ਚੀਜ਼ਾਂ[2].string, ਵੰਡੋ.ਚੀਜ਼ਾਂ[3].string,
					   ਅਗੇਤਰ);
			} ਨਹੀਂਤਾਂ ਜੇ (!ਨਿਵੇਸ਼_ਮਿਲਾਓ_ਅਧਾਰ && ਵੰਡੋ.nr == 2) {
				real_ਮਿਲਾਓ(&o, ਨਿਰਾ,
					   ਵੰਡੋ.ਚੀਜ਼ਾਂ[0].string, ਵੰਡੋ.ਚੀਜ਼ਾਂ[1].string,
					   ਅਗੇਤਰ);
			} ਨਹੀਂਤਾਂ {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&ਵੰਡੋ, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Figure out which mode to use */
	ਚੋਣ (o.ਬਹੁਲਕ) {
	ਮੂਲ:
		BUG("unexpected command mode %d", o.ਬਹੁਲਕ);
	ਹਾਲਤ ਬਹੁਲਕ_UNKNOWN:
		ਚੋਣ (argc) {
		ਮੂਲ:
			usage_with_options(ਮਿਲਾਓ_ਰੁੱਖ_usage, mt_options);
		ਹਾਲਤ 2:
			o.ਬਹੁਲਕ = ਬਹੁਲਕ_REAL;
			ਰੋਕੋ;
		ਹਾਲਤ 3:
			o.ਬਹੁਲਕ = ਬਹੁਲਕ_TRIVIAL;
			ਰੋਕੋ;
		}
		expected_ਬਚਿਆ_argc = argc;
		ਰੋਕੋ;
	ਹਾਲਤ ਬਹੁਲਕ_REAL:
		expected_ਬਚਿਆ_argc = 2;
		ਰੋਕੋ;
	ਹਾਲਤ ਬਹੁਲਕ_TRIVIAL:
		expected_ਬਚਿਆ_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		ਰੋਕੋ;
	}
	ਜੇ (o.ਬਹੁਲਕ == ਬਹੁਲਕ_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	ਜੇ (argc != expected_ਬਚਿਆ_argc)
		usage_with_options(ਮਿਲਾਓ_ਰੁੱਖ_usage, mt_options);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	/* Do the relevant type of merge */
	ਜੇ (o.ਬਹੁਲਕ == ਬਹੁਲਕ_REAL)
		ret = real_ਮਿਲਾਓ(&o, ਮਿਲਾਓ_ਅਧਾਰ, argv[0], argv[1], ਅਗੇਤਰ);
	ਨਹੀਂਤਾਂ
		ret = trivial_ਮਿਲਾਓ(argv[0], argv[1], argv[2]);

ਬਾਹਰਲਾ:
	strvec_clear(&xopts);
	ਵਾਪਸ ret;
}
