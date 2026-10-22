#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "tree-walk.h"
#ಅನ್ತರ್ಭಾವ "xdiff-interface.h"
#ಅನ್ತರ್ಭಾವ "help.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "merge-ort.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "blob.h"
#ಅನ್ತರ್ಭಾವ "merge-blobs.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಾಲು_termination = '\n';

ರಚನೆ ವಿಲೀನ_list {
	ರಚನೆ ವಿಲೀನ_list *ಮುಂದಿನ;
	ರಚನೆ ವಿಲೀನ_list *link;	/* other stages for this object */

	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ stage : 2;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
	ರಚನೆ blob *blob;
};

ಸ್ಥಿರ ರಚನೆ ವಿಲೀನ_list *ವಿಲೀನ_ಫಲಿತಾಂಶ, **ವಿಲೀನ_ಫಲಿತಾಂಶ_ಕೊನೆ = &ವಿಲೀನ_ಫಲಿತಾಂಶ;

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_ವಿಲೀನ_ನಮೂದು(ರಚನೆ ವಿಲೀನ_list *ನಮೂದು)
{
	*ವಿಲೀನ_ಫಲಿತಾಂಶ_ಕೊನೆ = ನಮೂದು;
	ವಿಲೀನ_ಫಲಿತಾಂಶ_ಕೊನೆ = &ನಮೂದು->ಮುಂದಿನ;
}

ಸ್ಥಿರ ಶೂನ್ಯ trivial_ವಿಲೀನ_trees(ರಚನೆ ವೃಕ್ಷ_desc t[3], ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ);

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *explanation(ರಚನೆ ವಿಲೀನ_list *ನಮೂದು)
{
	ಆಯ್ಕೆ (ನಮೂದು->stage) {
	ಸ್ಥಿತಿ 0:
		ಹಿಂತಿರುಗಿ "merged";
	ಸ್ಥಿತಿ 3:
		ಹಿಂತಿರುಗಿ "added in remote";
	ಸ್ಥಿತಿ 2:
		ಆದರೆ (ನಮೂದು->link)
			ಹಿಂತಿರುಗಿ "added in both";
		ಹಿಂತಿರುಗಿ "added in local";
	}

	/* Existed in base */
	ನಮೂದು = ನಮೂದು->link;
	ಆದರೆ (!ನಮೂದು)
		ಹಿಂತಿರುಗಿ "removed in both";

	ಆದರೆ (ನಮೂದು->link)
		ಹಿಂತಿರುಗಿ "changed in both";

	ಆದರೆ (ನಮೂದು->stage == 3)
		ಹಿಂತಿರುಗಿ "removed in local";
	ಹಿಂತಿರುಗಿ "removed in remote";
}

ಸ್ಥಿರ ಶೂನ್ಯ *ಫಲಿತಾಂಶ(ರಚನೆ ವಿಲೀನ_list *ನಮೂದು, ಗಾತ್ರ_t *ಗಾತ್ರ_2)
{
	ಎಣಿಕೆ object_type type;
	ರಚನೆ blob *ತಳ, *our, *their;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = ನಮೂದು->ಮಾರ್ಗ;

	ಆದರೆ (!ನಮೂದು->stage)
		ಹಿಂತಿರುಗಿ odb_read_object(the_repository->objects,
				       &ನಮೂದು->blob->object.oid, &type,
				       ಗಾತ್ರ_2);
	ತಳ = ಖಾಲಿ;
	ಆದರೆ (ನಮೂದು->stage == 1) {
		ತಳ = ನಮೂದು->blob;
		ನಮೂದು = ನಮೂದು->link;
	}
	our = ಖಾಲಿ;
	ಆದರೆ (ನಮೂದು && ನಮೂದು->stage == 2) {
		our = ನಮೂದು->blob;
		ನಮೂದು = ನಮೂದು->link;
	}
	their = ಖಾಲಿ;
	ಆದರೆ (ನಮೂದು)
		their = ನಮೂದು->blob;
	ಹಿಂತಿರುಗಿ ವಿಲೀನ_blobs(the_repository->ಸೂಚ್ಯಂಕ, ಮಾರ್ಗ,
			   ತಳ, our, their, ಗಾತ್ರ_2);
}

ಸ್ಥಿರ ಶೂನ್ಯ *origin(ರಚನೆ ವಿಲೀನ_list *ನಮೂದು, ಗಾತ್ರ_t *ಗಾತ್ರ_2)
{
	ಎಣಿಕೆ object_type type;
	ಆಗ (ನಮೂದು) {
		ಆದರೆ (ನಮೂದು->stage == 2)
			ಹಿಂತಿರುಗಿ odb_read_object(the_repository->objects,
					       &ನಮೂದು->blob->object.oid,
					       &type, ಗಾತ್ರ_2);
		ನಮೂದು = ನಮೂದು->link;
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_outf(ಶೂನ್ಯ *priv ಬಳಸದ, mmfile_t *mb, ಪೂರ್ಣಾಂಕ nbuf)
{
	ಪೂರ್ಣಾಂಕ i;
	ಗಾಗಿ (i = 0; i < nbuf; i++)
		ಬರೆ("%.*s", (ಪೂರ್ಣಾಂಕ) mb[i].ಗಾತ್ರ_2, mb[i].ptr);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ show_diff(ರಚನೆ ವಿಲೀನ_list *ನಮೂದು)
{
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
	mmfile_t src, dst;
	xpparam_t xpp;
	xdemitconf_t xecfg;
	xdemitcb_t ecb = { .ಹೊರಹೋಗು_ಸಾಲು = show_outf };

	memset(&xpp, 0, ಗಾತ್ರ(xpp));
	xpp.flags = 0;
	memset(&xecfg, 0, ಗಾತ್ರ(xecfg));
	xecfg.ctxlen = 3;

	src.ptr = origin(ನಮೂದು, &ಗಾತ್ರ_2);
	ಆದರೆ (!src.ptr)
		ಗಾತ್ರ_2 = 0;
	src.ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
	dst.ptr = ಫಲಿತಾಂಶ(ನಮೂದು, &ಗಾತ್ರ_2);
	ಆದರೆ (!dst.ptr)
		ಗಾತ್ರ_2 = 0;
	dst.ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
	ಆದರೆ (xdi_diff(&src, &dst, &xpp, &xecfg, &ecb))
		die("unable to generate diff");
	ಬಿಡುಗಡೆ(src.ptr);
	ಬಿಡುಗಡೆ(dst.ptr);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ಫಲಿತಾಂಶ_list(ರಚನೆ ವಿಲೀನ_list *ನಮೂದು)
{
	ಬರೆ("%s\n", explanation(ನಮೂದು));
	ಮಾಡು {
		ರಚನೆ ವಿಲೀನ_list *link = ನಮೂದು->link;
		ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *desc[4] = { "result", "base", "our", "their" };
		ಬರೆ("  %-6s %o %s %s\n", desc[ನಮೂದು->stage], ನಮೂದು->ಬಹುಲಕ, oid_ಗಮ್ಯ_hex(&ನಮೂದು->blob->object.oid), ನಮೂದು->ಮಾರ್ಗ);
		ನಮೂದು = link;
	} ಆಗ (ನಮೂದು);
}

ಸ್ಥಿರ ಶೂನ್ಯ show_ಫಲಿತಾಂಶ(ಶೂನ್ಯ)
{
	ರಚನೆ ವಿಲೀನ_list *walk;

	walk = ವಿಲೀನ_ಫಲಿತಾಂಶ;
	ಆಗ (walk) {
		show_ಫಲಿತಾಂಶ_list(walk);
		show_diff(walk);
		walk = walk->ಮುಂದಿನ;
	}
}

/* An empty entry never compares same, not even to another empty entry */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ same_ನಮೂದು(ರಚನೆ name_ನಮೂದು *a, ರಚನೆ name_ನಮೂದು *b)
{
	ಹಿಂತಿರುಗಿ	!is_null_oid(&a->oid) &&
		!is_null_oid(&b->oid) &&
		oideq(&a->oid, &b->oid) &&
		a->ಬಹುಲಕ == b->ಬಹುಲಕ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ both_ಖಾಲಿಗಳು(ರಚನೆ name_ನಮೂದು *a, ರಚನೆ name_ನಮೂದು *b)
{
	ಹಿಂತಿರುಗಿ is_null_oid(&a->oid) && is_null_oid(&b->oid);
}

ಸ್ಥಿರ ರಚನೆ ವಿಲೀನ_list *create_ನಮೂದು(ಚಿಹ್ನೆರಹಿತ stage, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ರಚನೆ ವಿಲೀನ_list *res = xcalloc(1, ಗಾತ್ರ(*res));

	res->stage = stage;
	res->ಮಾರ್ಗ = ಮಾರ್ಗ;
	res->ಬಹುಲಕ = ಬಹುಲಕ;
	res->blob = lookup_blob(the_repository, oid);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಅಕ್ಷರ *traverse_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ರಚನೆ traverse_info *info, ಸ್ಥಿರಾಂಕ ರಚನೆ name_ನಮೂದು *n)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	strbuf_make_traverse_ಮಾರ್ಗ(&buf, info, n->ಮಾರ್ಗ, n->pathlen);
	ಹಿಂತಿರುಗಿ strbuf_detach(&buf, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ resolve(ಸ್ಥಿರಾಂಕ ರಚನೆ traverse_info *info, ರಚನೆ name_ನಮೂದು *ours, ರಚನೆ name_ನಮೂದು *ಫಲಿತಾಂಶ)
{
	ರಚನೆ ವಿಲೀನ_list *orig, *final;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;

	/* If it's already ours, don't bother showing it */
	ಆದರೆ (!ours)
		ಹಿಂತಿರುಗಿ;

	ಮಾರ್ಗ = traverse_ಮಾರ್ಗ(info, ಫಲಿತಾಂಶ);
	orig = create_ನಮೂದು(2, ours->ಬಹುಲಕ, &ours->oid, ಮಾರ್ಗ);
	final = create_ನಮೂದು(0, ಫಲಿತಾಂಶ->ಬಹುಲಕ, &ಫಲಿತಾಂಶ->oid, ಮಾರ್ಗ);

	final->link = orig;

	ಕೂಡಿಸು_ವಿಲೀನ_ನಮೂದು(final);
}

ಸ್ಥಿರ ಶೂನ್ಯ unresolved_directory(ಸ್ಥಿರಾಂಕ ರಚನೆ traverse_info *info,
				 ರಚನೆ name_ನಮೂದು n[3])
{
	ರಚನೆ repository *r = the_repository;
	ಅಕ್ಷರ *newbase;
	ರಚನೆ name_ನಮೂದು *p;
	ರಚನೆ ವೃಕ್ಷ_desc t[3];
	ಶೂನ್ಯ *buf0, *buf1, *buf2;

	ಗಾಗಿ (p = n; p < n + 3; p++) {
		ಆದರೆ (p->ಬಹುಲಕ && S_ISDIR(p->ಬಹುಲಕ))
			ನಿಲ್ಲಿಸು;
	}
	ಆದರೆ (n + 3 <= p)
		ಹಿಂತಿರುಗಿ; /* there is no tree here */

	newbase = traverse_ಮಾರ್ಗ(info, p);

#ವ್ಯಾಖ್ಯಾನಿಸು ನಮೂದು_OID(e) (((e)->ಬಹುಲಕ && S_ISDIR((e)->ಬಹುಲಕ)) ? &(e)->oid : ಖಾಲಿ)
	buf0 = fill_ವೃಕ್ಷ_descriptor(r, t + 0, ನಮೂದು_OID(n + 0));
	buf1 = fill_ವೃಕ್ಷ_descriptor(r, t + 1, ನಮೂದು_OID(n + 1));
	buf2 = fill_ವೃಕ್ಷ_descriptor(r, t + 2, ನಮೂದು_OID(n + 2));
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ನಮೂದು_OID

	trivial_ವಿಲೀನ_trees(t, newbase);

	ಬಿಡುಗಡೆ(buf0);
	ಬಿಡುಗಡೆ(buf1);
	ಬಿಡುಗಡೆ(buf2);
	ಬಿಡುಗಡೆ(newbase);
}


ಸ್ಥಿರ ರಚನೆ ವಿಲೀನ_list *link_ನಮೂದು(ಚಿಹ್ನೆರಹಿತ stage, ಸ್ಥಿರಾಂಕ ರಚನೆ traverse_info *info, ರಚನೆ name_ನಮೂದು *n, ರಚನೆ ವಿಲೀನ_list *ನಮೂದು)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
	ರಚನೆ ವಿಲೀನ_list *link;

	ಆದರೆ (!n->ಬಹುಲಕ)
		ಹಿಂತಿರುಗಿ ನಮೂದು;
	ಆದರೆ (ನಮೂದು)
		ಮಾರ್ಗ = ನಮೂದು->ಮಾರ್ಗ;
	ಇಲ್ಲದಿದ್ದರೆ
		ಮಾರ್ಗ = traverse_ಮಾರ್ಗ(info, n);
	link = create_ನಮೂದು(stage, n->ಬಹುಲಕ, &n->oid, ಮಾರ್ಗ);
	link->link = ನಮೂದು;
	ಹಿಂತಿರುಗಿ link;
}

ಸ್ಥಿರ ಶೂನ್ಯ unresolved(ಸ್ಥಿರಾಂಕ ರಚನೆ traverse_info *info, ರಚನೆ name_ನಮೂದು n[3])
{
	ರಚನೆ ವಿಲೀನ_list *ನಮೂದು = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ i;
	ಚಿಹ್ನೆರಹಿತ dirmask = 0, ಮುಖವಾಡ = 0;

	ಗಾಗಿ (i = 0; i < 3; i++) {
		ಮುಖವಾಡ |= (1 << i);
		/*
		 * Treat missing entries as directories so that we return
		 * after unresolved_directory has handled this.
		 */
		ಆದರೆ (!n[i].ಬಹುಲಕ || S_ISDIR(n[i].ಬಹುಲಕ))
			dirmask |= (1 << i);
	}

	unresolved_directory(info, n);

	ಆದರೆ (dirmask == ಮುಖವಾಡ)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (n[2].ಬಹುಲಕ && !S_ISDIR(n[2].ಬಹುಲಕ))
		ನಮೂದು = link_ನಮೂದು(3, info, n + 2, ನಮೂದು);
	ಆದರೆ (n[1].ಬಹುಲಕ && !S_ISDIR(n[1].ಬಹುಲಕ))
		ನಮೂದು = link_ನಮೂದು(2, info, n + 1, ನಮೂದು);
	ಆದರೆ (n[0].ಬಹುಲಕ && !S_ISDIR(n[0].ಬಹುಲಕ))
		ನಮೂದು = link_ನಮೂದು(1, info, n + 0, ನಮೂದು);

	ಕೂಡಿಸು_ವಿಲೀನ_ನಮೂದು(ನಮೂದು);
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ threeway_callback(ಪೂರ್ಣಾಂಕ n ಬಳಸದ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಖವಾಡ,
			     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ dirmask ಬಳಸದ,
			     ರಚನೆ name_ನಮೂದು *ನಮೂದು, ರಚನೆ traverse_info *info)
{
	/* Same in both? */
	ಆದರೆ (same_ನಮೂದು(ನಮೂದು+1, ನಮೂದು+2) || both_ಖಾಲಿಗಳು(ನಮೂದು+1, ನಮೂದು+2)) {
		/* Modified, added or removed identically */
		resolve(info, ಖಾಲಿ, ನಮೂದು+1);
		ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
	}

	ಆದರೆ (same_ನಮೂದು(ನಮೂದು+0, ನಮೂದು+1)) {
		ಆದರೆ (!is_null_oid(&ನಮೂದು[2].oid) && !S_ISDIR(ನಮೂದು[2].ಬಹುಲಕ)) {
			/* We did not touch, they modified -- take theirs */
			resolve(info, ನಮೂದು+1, ನಮೂದು+2);
			ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
		}
		/*
		 * If we did not touch a directory but they made it
		 * into a file, we fall through and unresolved()
		 * recurses down.  Likewise for the opposite case.
		 */
	}

	ಆದರೆ (same_ನಮೂದು(ನಮೂದು+0, ನಮೂದು+2) || both_ಖಾಲಿಗಳು(ನಮೂದು+0, ನಮೂದು+2)) {
		/* We added, modified or removed, they did not touch -- take ours */
		resolve(info, ಖಾಲಿ, ನಮೂದು+1);
		ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
	}

	unresolved(info, ನಮೂದು);
	ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
}

ಸ್ಥಿರ ಶೂನ್ಯ trivial_ವಿಲೀನ_trees(ರಚನೆ ವೃಕ್ಷ_desc t[3], ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ)
{
	ರಚನೆ traverse_info info;

	ಸಿದ್ಧತೆ_traverse_info(&info, ತಳ);
	info.fn = threeway_callback;
	traverse_trees(the_repository->ಸೂಚ್ಯಂಕ, 3, t, &info);
}

ಸ್ಥಿರ ಶೂನ್ಯ *get_ವೃಕ್ಷ_descriptor(ರಚನೆ repository *r,
				 ರಚನೆ ವೃಕ್ಷ_desc *desc,
				 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rev)
{
	ರಚನೆ object_id oid;
	ಶೂನ್ಯ *buf;

	ಆದರೆ (repo_get_oid(r, rev, &oid))
		die("unknown rev %s", rev);
	buf = fill_ವೃಕ್ಷ_descriptor(r, desc, &oid);
	ಆದರೆ (!buf)
		die("%s is not a tree", rev);
	ಹಿಂತಿರುಗಿ buf;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ trivial_ವಿಲೀನ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch1,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch2)
{
	ರಚನೆ repository *r = the_repository;
	ರಚನೆ ವೃಕ್ಷ_desc t[3];
	ಶೂನ್ಯ *buf1, *buf2, *buf3;

	buf1 = get_ವೃಕ್ಷ_descriptor(r, t+0, ತಳ);
	buf2 = get_ವೃಕ್ಷ_descriptor(r, t+1, branch1);
	buf3 = get_ವೃಕ್ಷ_descriptor(r, t+2, branch2);
	trivial_ವಿಲೀನ_trees(t, "");
	ಬಿಡುಗಡೆ(buf1);
	ಬಿಡುಗಡೆ(buf2);
	ಬಿಡುಗಡೆ(buf3);

	show_ಫಲಿತಾಂಶ();
	ಹಿಂತಿರುಗಿ 0;
}

ಎಣಿಕೆ ಬಹುಲಕ {
	ಬಹುಲಕ_UNKNOWN,
	ಬಹುಲಕ_TRIVIAL,
	ಬಹುಲಕ_REAL,
};

ರಚನೆ ವಿಲೀನ_ವೃಕ್ಷ_options {
	ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;
	ಪೂರ್ಣಾಂಕ allow_unrelated_histories;
	ಪೂರ್ಣಾಂಕ show_messages;
	ಪೂರ್ಣಾಂಕ name_only;
	ಪೂರ್ಣಾಂಕ use_stdin;
	ರಚನೆ ವಿಲೀನ_options ವಿಲೀನ_options;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ real_ವಿಲೀನ(ರಚನೆ ವಿಲೀನ_ವೃಕ್ಷ_options *o,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ವಿಲೀನ_ತಳ,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch1, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch2,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ರಚನೆ commit *parent1, *parent2;
	ರಚನೆ commit_list *ವಿಲೀನ_bases = ಖಾಲಿ;
	ರಚನೆ ವಿಲೀನ_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ = { 0 };
	ಪೂರ್ಣಾಂಕ show_messages = o->show_messages;
	ರಚನೆ ವಿಲೀನ_options opt;

	copy_ವಿಲೀನ_options(&opt, &o->ವಿಲೀನ_options);
	opt.show_rename_ಪ್ರಗತಿ = 0;

	opt.branch1 = branch1;
	opt.branch2 = branch2;

	ಆದರೆ (ವಿಲೀನ_ತಳ) {
		ರಚನೆ ವೃಕ್ಷ *ತಳ_ವೃಕ್ಷ, *parent1_ವೃಕ್ಷ, *parent2_ವೃಕ್ಷ;

		/*
		 * We actually only need the trees because we already
		 * have a merge base.
		 */
		ರಚನೆ object_id ತಳ_oid, ತಲೆ_oid, ವಿಲೀನ_oid;

		ಆದರೆ (repo_get_oid_treeish(the_repository, ವಿಲೀನ_ತಳ, &ತಳ_oid))
			die(_("could not parse as tree '%s'"), ವಿಲೀನ_ತಳ);
		ತಳ_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository,
						     &ತಳ_oid);
		ಆದರೆ (!ತಳ_ವೃಕ್ಷ)
			die(_("unable to read tree (%s)"), oid_ಗಮ್ಯ_hex(&ತಳ_oid));
		ಆದರೆ (repo_get_oid_treeish(the_repository, branch1, &ತಲೆ_oid))
			die(_("could not parse as tree '%s'"), branch1);
		parent1_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository,
							&ತಲೆ_oid);
		ಆದರೆ (!parent1_ವೃಕ್ಷ)
			die(_("unable to read tree (%s)"), oid_ಗಮ್ಯ_hex(&ತಲೆ_oid));
		ಆದರೆ (repo_get_oid_treeish(the_repository, branch2, &ವಿಲೀನ_oid))
			die(_("could not parse as tree '%s'"), branch2);
		parent2_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository,
							&ವಿಲೀನ_oid);
		ಆದರೆ (!parent2_ವೃಕ್ಷ)
			die(_("unable to read tree (%s)"), oid_ಗಮ್ಯ_hex(&ವಿಲೀನ_oid));

		opt.ancestor = ವಿಲೀನ_ತಳ;
		ವಿಲೀನ_incore_nonrecursive(&opt, ತಳ_ವೃಕ್ಷ, parent1_ವೃಕ್ಷ, parent2_ವೃಕ್ಷ, &ಫಲಿತಾಂಶ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		parent1 = get_ವಿಲೀನ_ಪೋಷಕ(branch1);
		ಆದರೆ (!parent1)
			help_unknown_ref(branch1, "merge-tree",
					 _("not something we can merge"));

		parent2 = get_ವಿಲೀನ_ಪೋಷಕ(branch2);
		ಆದರೆ (!parent2)
			help_unknown_ref(branch2, "merge-tree",
					 _("not something we can merge"));

		/*
		 * Get the merge bases, in reverse order; see comment above
		 * merge_incore_recursive in merge-ort.h
		 */
		ಆದರೆ (repo_get_ವಿಲೀನ_bases(the_repository, parent1,
					 parent2, &ವಿಲೀನ_bases) < 0)
			ನಿರ್ಗಮಿಸು(128);
		ಆದರೆ (!ವಿಲೀನ_bases && !o->allow_unrelated_histories)
			die(_("refusing to merge unrelated histories"));
		ವಿಲೀನ_bases = commit_list_ಹಿಮ್ಮುಖ(ವಿಲೀನ_bases);
		ವಿಲೀನ_incore_recursive(&opt, ವಿಲೀನ_bases, parent1, parent2, &ಫಲಿತಾಂಶ);
		commit_list_free(ವಿಲೀನ_bases);
	}

	ಆದರೆ (ಫಲಿತಾಂಶ.clean < 0)
		die(_("failure to merge"));

	ಆದರೆ (o->ವಿಲೀನ_options.mergeability_only)
		ಹೋಗು cleanup;

	ಆದರೆ (show_messages == -1)
		show_messages = !ಫಲಿತಾಂಶ.clean;

	ಆದರೆ (o->use_stdin)
		ಬರೆ("%d%c", ಫಲಿತಾಂಶ.clean, ಸಾಲು_termination);
	ಬರೆ("%s%c", oid_ಗಮ್ಯ_hex(&ಫಲಿತಾಂಶ.ವೃಕ್ಷ->object.oid), ಸಾಲು_termination);
	ಆದರೆ (!ಫಲಿತಾಂಶ.clean) {
		ರಚನೆ string_list conflicted_files = STRING_LIST_INIT_NODUP;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆಯ = ಖಾಲಿ;

		ವಿಲೀನ_get_conflicted_files(&ಫಲಿತಾಂಶ, &conflicted_files);
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < conflicted_files.nr; i++) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = conflicted_files.ವಸ್ತುಗಳು[i].string;
			ರಚನೆ stage_info *c = conflicted_files.ವಸ್ತುಗಳು[i].util;
			ಆದರೆ (!o->name_only)
				ಬರೆ("%06o %s %d\t",
				       c->ಬಹುಲಕ, oid_ಗಮ್ಯ_hex(&c->oid), c->stage);
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೊನೆಯ && !strcmp(ಕೊನೆಯ, name))
				ಮುಂದುವರಿ;
			write_name_quoted_relative(
				name, ಪೂರ್ವಪ್ರತ್ಯಯ, stdout, ಸಾಲು_termination);
			ಕೊನೆಯ = name;
		}
		string_list_clear(&conflicted_files, 1);
	}
	ಆದರೆ (show_messages) {
		ಅಕ್ಷರ_ಬರೆ(ಸಾಲು_termination);
		ವಿಲೀನ_display_update_messages(&opt, ಸಾಲು_termination == '\0',
					      &ಫಲಿತಾಂಶ);
	}
	ಆದರೆ (o->use_stdin)
		ಅಕ್ಷರ_ಬರೆ(ಸಾಲು_termination);

cleanup:
	ವಿಲೀನ_finalize(&opt, &ಫಲಿತಾಂಶ);
	clear_ವಿಲೀನ_options(&opt);
	ಹಿಂತಿರುಗಿ !ಫಲಿತಾಂಶ.clean; /* result.clean < 0 handled above */
}

ಪೂರ್ಣಾಂಕ cmd_ವಿಲೀನ_ವೃಕ್ಷ(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ ವಿಲೀನ_ವೃಕ್ಷ_options o = { .show_messages = -1 };
	ರಚನೆ strvec xopts = STRVEC_INIT;
	ಪೂರ್ಣಾಂಕ expected_ಉಳಿದ_argc;
	ಪೂರ್ಣಾಂಕ original_argc;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ವಿಲೀನ_ತಳ = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ret;
	ಪೂರ್ಣಾಂಕ quiet = 0;

	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ವಿಲೀನ_ವೃಕ್ಷ_usage[] = {
		N_("git merge-tree [--write-tree] [<options>] <branch1> <branch2>"),
		N_("git merge-tree [--trivial-merge] <base-tree> <branch1> <branch2>"),
		ಖಾಲಿ
	};
	ರಚನೆ option mt_options[] = {
		OPT_CMDMODE(0, "write-tree", &o.ಬಹುಲಕ,
			    N_("do a real merge instead of a trivial merge"),
			    ಬಹುಲಕ_REAL),
		OPT_CMDMODE(0, "trivial-merge", &o.ಬಹುಲಕ,
			    N_("do a trivial merge only"), ಬಹುಲಕ_TRIVIAL),
		OPT_BOOL(0, "messages", &o.show_messages,
			 N_("also show informational/conflict messages")),
		OPT_BOOL_F(0, "quiet",
			   &quiet,
			   N_("suppress all output; only exit status wanted"),
			   PARSE_OPT_NONEG),
		OPT_ಗಣ_INT('z', ಖಾಲಿ, &ಸಾಲು_termination,
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
			   &ವಿಲೀನ_ತಳ,
			   N_("tree-ish"),
			   N_("specify a merge-base for the merge")),
		OPT_STRVEC('X', "strategy-option", &xopts, N_("option=value"),
			N_("option for selected merge strategy")),
		OPT_ಕೊನೆ()
	};

	/* Init merge options */
	init_basic_ವಿಲೀನ_options(&o.ವಿಲೀನ_options, the_repository);

	/* Parse arguments */
	original_argc = argc - 1; /* ignoring argv[0] */
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, mt_options,
			     ವಿಲೀನ_ವೃಕ್ಷ_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	ಆದರೆ (quiet && o.show_messages == -1)
		o.show_messages = 0;
	o.ವಿಲೀನ_options.mergeability_only = quiet;
	die_for_incompatible_opt2(quiet, "--quiet", o.show_messages, "--messages");
	die_for_incompatible_opt2(quiet, "--quiet", o.name_only, "--name-only");
	die_for_incompatible_opt2(quiet, "--quiet", o.use_stdin, "--stdin");
	die_for_incompatible_opt2(quiet, "--quiet", !ಸಾಲು_termination, "-z");

	ಆದರೆ (xopts.nr && o.ಬಹುಲಕ == ಬಹುಲಕ_TRIVIAL)
		die(_("--trivial-merge is incompatible with all other options"));
	ಗಾಗಿ (ಗಾತ್ರ_t x = 0; x < xopts.nr; x++)
		ಆದರೆ (parse_ವಿಲೀನ_opt(&o.ವಿಲೀನ_options, xopts.v[x]))
			die(_("unknown strategy option: -X%s"), xopts.v[x]);

	/* Handle --stdin */
	ಆದರೆ (o.use_stdin) {
		ರಚನೆ strbuf buf = STRBUF_INIT;

		ಆದರೆ (o.ಬಹುಲಕ == ಬಹುಲಕ_TRIVIAL)
			die(_("--trivial-merge is incompatible with all other options"));
		ಆದರೆ (ವಿಲೀನ_ತಳ)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--merge-base", "--stdin");
		ಸಾಲು_termination = '\0';
		ಆಗ (strbuf_getline_lf(&buf, stdin) != EOF) {
			ರಚನೆ string_list ವಿಭಜಿಸು = STRING_LIST_INIT_NODUP;
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಒಳಹರಿವು_ವಿಲೀನ_ತಳ = ಖಾಲಿ;

			string_list_ವಿಭಜಿಸು_ಒಳ_ಇರಿಸು_f(&ವಿಭಜಿಸು, buf.buf, " ", -1,
						     STRING_LIST_ವಿಭಜಿಸು_TRIM);

			ಆದರೆ (ವಿಭಜಿಸು.nr < 2)
				die(_("malformed input line: '%s'."), buf.buf);

			/* parse the merge-base */
			ಆದರೆ (!strcmp(ವಿಭಜಿಸು.ವಸ್ತುಗಳು[1].string, "--")) {
				ಒಳಹರಿವು_ವಿಲೀನ_ತಳ = ವಿಭಜಿಸು.ವಸ್ತುಗಳು[0].string;
			}

			ಆದರೆ (ಒಳಹರಿವು_ವಿಲೀನ_ತಳ && ವಿಭಜಿಸು.nr == 4) {
				real_ವಿಲೀನ(&o, ಒಳಹರಿವು_ವಿಲೀನ_ತಳ,
					   ವಿಭಜಿಸು.ವಸ್ತುಗಳು[2].string, ವಿಭಜಿಸು.ವಸ್ತುಗಳು[3].string,
					   ಪೂರ್ವಪ್ರತ್ಯಯ);
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಒಳಹರಿವು_ವಿಲೀನ_ತಳ && ವಿಭಜಿಸು.nr == 2) {
				real_ವಿಲೀನ(&o, ಖಾಲಿ,
					   ವಿಭಜಿಸು.ವಸ್ತುಗಳು[0].string, ವಿಭಜಿಸು.ವಸ್ತುಗಳು[1].string,
					   ಪೂರ್ವಪ್ರತ್ಯಯ);
			} ಇಲ್ಲದಿದ್ದರೆ {
				die(_("malformed input line: '%s'."), buf.buf);
			}
			maybe_flush_or_die(stdout, "stdout");

			string_list_clear(&ವಿಭಜಿಸು, 0);
		}
		strbuf_release(&buf);

		ret = 0;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Figure out which mode to use */
	ಆಯ್ಕೆ (o.ಬಹುಲಕ) {
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unexpected command mode %d", o.ಬಹುಲಕ);
	ಸ್ಥಿತಿ ಬಹುಲಕ_UNKNOWN:
		ಆಯ್ಕೆ (argc) {
		ಪೂರ್ವನಿಯೋಜಿತ:
			usage_with_options(ವಿಲೀನ_ವೃಕ್ಷ_usage, mt_options);
		ಸ್ಥಿತಿ 2:
			o.ಬಹುಲಕ = ಬಹುಲಕ_REAL;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 3:
			o.ಬಹುಲಕ = ಬಹುಲಕ_TRIVIAL;
			ನಿಲ್ಲಿಸು;
		}
		expected_ಉಳಿದ_argc = argc;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ ಬಹುಲಕ_REAL:
		expected_ಉಳಿದ_argc = 2;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ ಬಹುಲಕ_TRIVIAL:
		expected_ಉಳಿದ_argc = 3;
		/* Removal of `--trivial-merge` is expected */
		original_argc--;
		ನಿಲ್ಲಿಸು;
	}
	ಆದರೆ (o.ಬಹುಲಕ == ಬಹುಲಕ_TRIVIAL && argc < original_argc)
		die(_("--trivial-merge is incompatible with all other options"));

	ಆದರೆ (argc != expected_ಉಳಿದ_argc)
		usage_with_options(ವಿಲೀನ_ವೃಕ್ಷ_usage, mt_options);

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	/* Do the relevant type of merge */
	ಆದರೆ (o.ಬಹುಲಕ == ಬಹುಲಕ_REAL)
		ret = real_ವಿಲೀನ(&o, ವಿಲೀನ_ತಳ, argv[0], argv[1], ಪೂರ್ವಪ್ರತ್ಯಯ);
	ಇಲ್ಲದಿದ್ದರೆ
		ret = trivial_ವಿಲೀನ(argv[0], argv[1], argv[2]);

ಹೊರಹೋಗು:
	strvec_clear(&xopts);
	ಹಿಂತಿರುಗಿ ret;
}
