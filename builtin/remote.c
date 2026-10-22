#تعریف_کن USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "date.h"
#ضمن "gettext.h"
#ضمن "ident.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "transport.h"
#ضمن "remote.h"
#ضمن "string-list.h"
#ضمن "strbuf.h"
#ضمن "run-command.h"
#ضمن "rebase.h"
#ضمن "refs.h"
#ضمن "refspec.h"
#ضمن "odb.h"
#ضمن "strvec.h"
#ضمن "commit-reach.h"
#ضمن "progress.h"

ساكن ثابت حرف * ثابت builtin_remote_usage[] = {
	"git remote [-v | --verbose]",
	N_("git remote add [-t <branch>] [-m <master>] [-f] [--tags | --no-tags] [--mirror=<fetch|push>] <name> <url>"),
	N_("git remote rename [--[no-]progress] <old> <new>"),
	N_("git remote remove <name>"),
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	N_("git remote [-v | --verbose] show [-n] <name>"),
	N_("git remote prune [-n | --dry-run] <name>"),
	N_("git remote [-v | --verbose] update [-p | --prune] [(<group> | <remote>)...]"),
	N_("git remote set-branches [--add] <name> <branch>..."),
	N_("git remote get-url [--push] [--all] <name>"),
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	عدم
};

ساكن ثابت حرف * ثابت builtin_remote_جمع_کن_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	عدم
};

ساكن ثابت حرف * ثابت builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	عدم
};

ساكن ثابت حرف * ثابت builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	عدم
};

ساكن ثابت حرف * ثابت builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	عدم
};

ساكن ثابت حرف * ثابت builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	عدم
};

ساكن ثابت حرف * ثابت builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	عدم
};

ساكن ثابت حرف * ثابت builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	عدم
};

ساكن ثابت حرف * ثابت builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	عدم
};

ساكن ثابت حرف * ثابت builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	عدم
};

ساكن ثابت حرف * ثابت builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	عدم
};

#تعریف_کن GET_REF_حالتها (1<<0)
#تعریف_کن GET_سر_نامها (1<<1)
#تعریف_کن GET_افزودن_REF_حالتها (1<<2)

ساكن صحيح verbose;

ساكن صحيح fetch_remote(ثابت حرف *name)
{
	بنية فرزند_process cmd = فرزند_PROCESS_INIT;

	strvec_افزودن(&cmd.آرگومانها, "fetch");
	إذا (verbose)
		strvec_افزودن(&cmd.آرگومانها, "-v");
	strvec_افزودن(&cmd.آرگومانها, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	إذا (پیاپی_command(&cmd))
		أرجع error(_("Could not fetch %s"), name);
	أرجع 0;
}

تعداد {
	برچسبها_UNSET = 0,
	برچسبها_DEFAULT = 1,
	برچسبها_مجموعه = 2
};

#تعریف_کن MIRROR_NONE 0
#تعریف_کن MIRROR_FETCH 1
#تعریف_کن MIRROR_افزودن 2
#تعریف_کن MIRROR_BOTH (MIRROR_FETCH|MIRROR_افزودن)

ساكن فراغ جمع_کن_branch(ثابت حرف *کلید, ثابت حرف *branchname,
		       ثابت حرف *remotename, صحيح mirror, بنية strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	إذا (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	وإلا
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_مجموعه_multivar(the_repository, کلید, tmp->buf, "^$", 0);
}

ساكن ثابت حرف mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

ساكن صحيح parse_mirror_opt(ثابت بنية option *opt, ثابت حرف *arg, صحيح not)
{
	بیعلامت *mirror = opt->مقدار;
	إذا (not)
		*mirror = MIRROR_NONE;
	وإلا إذا (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	وإلا إذا (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	وإلا إذا (!strcmp(arg, "push"))
		*mirror = MIRROR_افزودن;
	وإلا
		أرجع error(_("unknown --mirror argument: %s"), arg);
	أرجع 0;
}

ساكن صحيح check_remote_collision(بنية remote *remote, فراغ *داده)
{
	ثابت حرف *name = داده;
	ثابت حرف *p;

	إذا (skip_پیشوند(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	إذا (skip_پیشوند(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	أرجع 0;
}

ساكن صحيح جمع_کن(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
	       بنية repository *repo استفادهنشده)
{
	صحيح fetch = 0, fetch_برچسبها = برچسبها_DEFAULT;
	بیعلامت mirror = MIRROR_NONE;
	بنية string_list track = STRING_LIST_INIT_NODUP;
	ثابت حرف *master = عدم;
	بنية remote *remote;
	بنية strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ثابت حرف *name, *url;
	صحيح نتیجه = 0;

	بنية option options[] = {
		OPT_BOOL('f', "fetch", &fetch, N_("fetch the remote branches")),
		OPT_مجموعه_INT(0, "tags", &fetch_برچسبها,
			    N_("import all tags and associated objects when fetching\n"
			       "or do not fetch any tag at all (--no-tags)"),
			    برچسبها_مجموعه),
		OPT_STRING_LIST('t', "track", &track, N_("branch"),
				N_("branch(es) to track")),
		OPT_STRING('m', "master", &master, N_("branch"), N_("master branch")),
		OPT_CALLBACK_F(0, "mirror", &mirror, "(push|fetch)",
			N_("set up remote as a mirror to push to or fetch from"),
			PARSE_OPT_OPTARG | PARSE_OPT_COMP_ARG, parse_mirror_opt),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_remote_جمع_کن_usage, 0);

	إذا (argc != 2)
		usage_with_options(builtin_remote_جمع_کن_usage, options);

	إذا (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	إذا (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	إذا (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		خروج(3);
	}

	إذا (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (فراغ *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_مجموعه(the_repository, buf.buf, url);

	إذا (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		إذا (track.nr == 0)
			string_list_append(&track, "*");
		لكل (اندازه_t i = 0; i < track.nr; i++) {
			جمع_کن_branch(buf.buf, track.اقلام[i].string,
				   name, mirror, &buf2);
		}
	}

	إذا (mirror & MIRROR_افزودن) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_مجموعه(the_repository, buf.buf, "true");
	}

	إذا (fetch_برچسبها != برچسبها_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_مجموعه(the_repository, buf.buf,
				fetch_برچسبها == برچسبها_مجموعه ? "--tags" : "--no-tags");
	}

	إذا (fetch && fetch_remote(name)) {
		نتیجه = 1;
		اذهب بیرون;
	}

	إذا (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		إذا (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			نتیجه = error(_("Could not setup master '%s'"), master);
	}

بیرون:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	أرجع نتیجه;
}

بنية branch_info {
	حرف *remote_name;
	بنية string_list ادغام;
	تعداد rebase_type rebase;
	حرف *افزودن_remote_name;
};

ساكن بنية string_list branch_list = STRING_LIST_INIT_DUP;

ساكن ثابت حرف *abbrev_ref(ثابت حرف *name, ثابت حرف *پیشوند)
{
	skip_پیشوند(name, پیشوند, &name);
	أرجع name;
}
#تعریف_کن abbrev_branch(name) abbrev_ref((name), "refs/heads/")

ساكن صحيح config_read_branches(ثابت حرف *کلید, ثابت حرف *مقدار,
				ثابت بنية config_context *ctx استفادهنشده,
				فراغ *داده استفادهنشده)
{
	ثابت حرف *orig_کلید = کلید;
	حرف *name;
	بنية string_list_قلم *قلم;
	بنية branch_info *info;
	تعداد { REMOTE, ادغام_2, REBASE, افزودن_REMOTE } type;
	اندازه_t کلید_len;

	إذا (!starts_with(کلید, "branch."))
		أرجع 0;

	کلید += طول_رشته("branch.");
	إذا (strip_suffix(کلید, ".remote", &کلید_len))
		type = REMOTE;
	وإلا إذا (strip_suffix(کلید, ".merge", &کلید_len))
		type = ادغام_2;
	وإلا إذا (strip_suffix(کلید, ".rebase", &کلید_len))
		type = REBASE;
	وإلا إذا (strip_suffix(کلید, ".pushremote", &کلید_len))
		type = افزودن_REMOTE;
	وإلا
		أرجع 0;

	name = xmemdupz(کلید, کلید_len);
	قلم = string_list_درج(&branch_list, name);

	إذا (!قلم->util)
		قلم->util = xcalloc(1, حجم(بنية branch_info));
	info = قلم->util;
	بدل (type) {
	حالة REMOTE:
		إذا (info->remote_name)
			warning(_("more than one %s"), orig_کلید);
		info->remote_name = xstrdup(مقدار);
		اكسر;
	حالة ادغام_2: {
		ثابت حرف *space = strchr(مقدار, ' ');
		مقدار = abbrev_branch(مقدار);
		طالما (space) {
			حرف *ادغام;
			ادغام = xstrndup(مقدار, space - مقدار);
			string_list_append(&info->ادغام, ادغام);
			مقدار = abbrev_branch(space + 1);
			space = strchr(مقدار, ' ');
		}
		string_list_append(&info->ادغام, xstrdup(مقدار));
		اكسر;
	}
	حالة REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_مقدار(مقدار);
		إذا (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, مقدار);
		اكسر;
	حالة افزودن_REMOTE:
		إذا (info->افزودن_remote_name)
			warning(_("more than one %s"), orig_کلید);
		info->افزودن_remote_name = xstrdup(مقدار);
		اكسر;
	افتراضي:
		BUG("unexpected type=%d", type);
	}

	حرر(name);
	أرجع 0;
}

ساكن فراغ read_branches(فراغ)
{
	إذا (branch_list.nr)
		أرجع;
	repo_config(the_repository, config_read_branches, عدم);
}

بنية ref_حالتها {
	بنية remote *remote;
	بنية string_list new_refs, skipped, stale, tracked, heads, افزودن;
	صحيح queried;
};

#تعریف_کن REF_حالتها_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.افزودن = STRING_LIST_INIT_DUP, \
}

ساكن صحيح get_ref_حالتها(ثابت بنية ref *remote_refs, بنية ref_حالتها *حالتها)
{
	بنية ref *fetch_map = عدم, **دم = &fetch_map;
	بنية ref *ref, *stale_refs;
	صحيح i;

	لكل (i = 0; i < حالتها->remote->fetch.nr; i++)
		إذا (get_fetch_map(remote_refs, &حالتها->remote->fetch.اقلام[i], &دم, 1))
			die(_("Could not get fetch map for refspec %s"),
				حالتها->remote->fetch.اقلام[i].raw);

	لكل (ref = fetch_map; ref; ref = ref->بعدی) {
		إذا (refname_matches_negative_refspec_قلم(ref->name, &حالتها->remote->fetch))
			string_list_append(&حالتها->skipped, abbrev_branch(ref->name));
		وإلا إذا (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&حالتها->new_refs, abbrev_branch(ref->name));
		وإلا
			string_list_append(&حالتها->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&حالتها->remote->fetch, fetch_map);
	لكل (ref = stale_refs; ref; ref = ref->بعدی) {
		بنية string_list_قلم *قلم =
			string_list_append(&حالتها->stale, abbrev_branch(ref->name));
		قلم->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_مرتب_سازی(&حالتها->new_refs);
	string_list_مرتب_سازی(&حالتها->skipped);
	string_list_مرتب_سازی(&حالتها->tracked);
	string_list_مرتب_سازی(&حالتها->stale);

	أرجع 0;
}

بنية افزودن_info {
	حرف *dest;
	صحيح forced;
	تعداد {
		افزودن_وضعیت_CREATE = 0,
		افزودن_وضعیت_DELETE,
		افزودن_وضعیت_UPTODATE,
		افزودن_وضعیت_FASTFORWARD,
		افزودن_وضعیت_OUTOFDATE,
		افزودن_وضعیت_NOTQUERIED
	} وضعیت;
};

ساكن صحيح get_افزودن_ref_حالتها(ثابت بنية ref *remote_refs,
	بنية ref_حالتها *حالتها)
{
	بنية remote *remote = حالتها->remote;
	بنية ref *ref, *local_refs, *افزودن_map;
	إذا (remote->mirror)
		أرجع 0;

	local_refs = get_local_heads();
	افزودن_map = copy_ref_list(remote_refs);

	تطابق_افزودن_refs(local_refs, &افزودن_map, &remote->افزودن, تطابق_REFS_NONE);

	لكل (ref = افزودن_map; ref; ref = ref->بعدی) {
		بنية string_list_قلم *قلم;
		بنية افزودن_info *info;

		إذا (!ref->peer_ref)
			استمر;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		قلم = string_list_append(&حالتها->افزودن,
					  abbrev_branch(ref->peer_ref->name));
		قلم->util = xcalloc(1, حجم(بنية افزودن_info));
		info = قلم->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		إذا (is_null_oid(&ref->new_oid)) {
			info->وضعیت = افزودن_وضعیت_DELETE;
		} وإلا إذا (oideq(&ref->old_oid, &ref->new_oid))
			info->وضعیت = افزودن_وضعیت_UPTODATE;
		وإلا إذا (is_null_oid(&ref->old_oid))
			info->وضعیت = افزودن_وضعیت_CREATE;
		وإلا إذا (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->وضعیت = افزودن_وضعیت_FASTFORWARD;
		وإلا
			info->وضعیت = افزودن_وضعیت_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(افزودن_map);
	أرجع 0;
}

ساكن صحيح get_افزودن_ref_حالتها_noquery(بنية ref_حالتها *حالتها)
{
	صحيح i;
	بنية remote *remote = حالتها->remote;
	بنية string_list_قلم *قلم;
	بنية افزودن_info *info;

	إذا (remote->mirror)
		أرجع 0;

	إذا (!remote->افزودن.nr) {
		قلم = string_list_append(&حالتها->افزودن, _("(matching)"));
		info = قلم->util = xcalloc(1, حجم(بنية افزودن_info));
		info->وضعیت = افزودن_وضعیت_NOTQUERIED;
		info->dest = xstrdup(قلم->string);
	}
	لكل (i = 0; i < remote->افزودن.nr; i++) {
		ثابت بنية refspec_قلم *spec = &remote->افزودن.اقلام[i];
		إذا (spec->matching)
			قلم = string_list_append(&حالتها->افزودن, _("(matching)"));
		وإلا إذا (طول_رشته(spec->src))
			قلم = string_list_append(&حالتها->افزودن, spec->src);
		وإلا
			قلم = string_list_append(&حالتها->افزودن, _("(delete)"));

		info = قلم->util = xcalloc(1, حجم(بنية افزودن_info));
		info->forced = spec->force;
		info->وضعیت = افزودن_وضعیت_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : قلم->string);
	}
	أرجع 0;
}

ساكن صحيح get_سر_نامها(ثابت بنية ref *remote_refs, بنية ref_حالتها *حالتها)
{
	بنية ref *ref, *matches;
	بنية ref *fetch_map = عدم, **fetch_map_دم = &fetch_map;
	بنية refspec_قلم refspec = {
		.force = 0,
		.الگو = 1,
		.src = (حرف *) "refs/heads/*",
		.dst = (حرف *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_دم, 0);
	matches = guess_remote_سر(پیدا_کن_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_سر_ALL);
	لكل (ref = matches; ref; ref = ref->بعدی)
		string_list_append(&حالتها->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	أرجع 0;
}

بنية معلوم_remote {
	بنية معلوم_remote *بعدی;
	بنية remote *remote;
};

بنية معلوم_remotes {
	بنية remote *مقصد_delete;
	بنية معلوم_remote *list;
};

ساكن صحيح جمع_کن_معلوم_remote(بنية remote *remote, فراغ *cb_داده)
{
	بنية معلوم_remotes *all = cb_داده;
	بنية معلوم_remote *r;

	إذا (!strcmp(all->مقصد_delete->name, remote->name))
		أرجع 0;

	r = xmalloc(حجم(*r));
	r->remote = remote;
	r->بعدی = all->list;
	all->list = r;
	أرجع 0;
}

بنية branches_for_remote {
	بنية remote *remote;
	بنية string_list *branches, *skipped;
	بنية معلوم_remotes *keep;
};

ساكن صحيح جمع_کن_branch_for_removal(ثابت بنية reference *ref, فراغ *cb_داده)
{
	بنية branches_for_remote *branches = cb_داده;
	بنية refspec_قلم refspec;
	بنية معلوم_remote *kr;

	memset(&refspec, 0, حجم(refspec));
	refspec.dst = (حرف *)ref->name;
	إذا (remote_پیدا_کن_tracking(branches->remote, &refspec))
		أرجع 0;
	حرر(refspec.src);

	/* don't delete a branch if another remote also uses it */
	لكل (kr = branches->keep->list; kr; kr = kr->بعدی) {
		memset(&refspec, 0, حجم(refspec));
		refspec.dst = (حرف *)ref->name;
		إذا (!remote_پیدا_کن_tracking(kr->remote, &refspec)) {
			حرر(refspec.src);
			أرجع 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	إذا (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		إذا (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		أرجع 0;
	}

	string_list_append(branches->branches, ref->name);

	أرجع 0;
}

بنية rename_info {
	ثابت حرف *old_name;
	ثابت حرف *new_name;
	بنية ref_transaction *transaction;
	بنية پیشرفت *پیشرفت;
	بنية strbuf *err;
	uint32_t پیشرفت_nr;
	uint64_t شاخص;
};

ساكن فراغ compute_renamed_ref(بنية rename_info *rename,
				ثابت حرف *refname,
				بنية strbuf *بیرون)
{
	strbuf_reset(بیرون);
	strbuf_addstr(بیرون, refname);
	strbuf_splice(بیرون, طول_رشته("refs/remotes/"), طول_رشته(rename->old_name),
		      rename->new_name, طول_رشته(rename->new_name));
}

ساكن صحيح rename_one_reflog_مدخل(ثابت حرف *old_refname,
				   بنية object_id *old_oid,
				   بنية object_id *new_oid,
				   ثابت حرف *committer,
				   timestamp_t timestamp, صحيح tz,
				   ثابت حرف *پیام, فراغ *cb_داده)
{
	بنية rename_info *rename = cb_داده;
	بنية strbuf new_refname = STRBUF_INIT;
	بنية strbuf عنصر_خنثی = STRBUF_INIT;
	بنية strbuf name = STRBUF_INIT;
	بنية strbuf mail = STRBUF_INIT;
	بنية ident_تقسیم ident;
	ثابت حرف *date;
	صحيح error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	إذا (تقسیم_ident_سطر(&ident, committer, طول_رشته(committer)) < 0) {
		error = -1;
		اذهب بیرون;
	}

	strbuf_جمع_کن(&name, ident.name_begin, ident.name_انتها - ident.name_begin);
	strbuf_جمع_کن(&mail, ident.mail_begin, ident.mail_انتها - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_نما(نرمال));
	strbuf_addstr(&عنصر_خنثی, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, عنصر_خنثی.buf, پیام,
					      rename->شاخص++, rename->err);

بیرون:
	strbuf_release(&new_refname);
	strbuf_release(&عنصر_خنثی);
	strbuf_release(&name);
	strbuf_release(&mail);
	أرجع error;
}

ساكن صحيح rename_one_reflog(ثابت حرف *old_refname,
			     ثابت بنية object_id *old_oid,
			     بنية rename_info *rename)
{
	بنية strbuf new_refname = STRBUF_INIT;
	بنية strbuf پیام_2 = STRBUF_INIT;
	صحيح error;

	إذا (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		أرجع 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_مدخل, rename);
	إذا (error < 0)
		اذهب بیرون;

	compute_renamed_ref(rename, old_refname, &new_refname);

	/*
	 * Manually write the reflog entry for the now-renamed ref. We cannot
	 * rely on `rename_one_ref()` to do this for us as that would screw
	 * over order in which reflog entries are being written.
	 *
	 * Furthermore, we only append the entry in case the reference
	 * resolves. Missing references shouldn't have reflogs anyway.
	 */
	strbuf_addf(&پیام_2, "remote: renamed %s to %s", old_refname,
		    new_refname.buf);

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      old_oid, old_oid, git_committer_info(0),
					      پیام_2.buf, rename->شاخص++, rename->err);
	إذا (error < 0)
		أرجع error;

بیرون:
	strbuf_release(&new_refname);
	strbuf_release(&پیام_2);
	أرجع error;
}

ساكن صحيح rename_one_ref(ثابت بنية reference *ref, فراغ *cb_داده)
{
	بنية strbuf new_referent = STRBUF_INIT;
	بنية strbuf new_refname = STRBUF_INIT;
	بنية rename_info *rename = cb_داده;
	ثابت بنية object_id *oid = ref->oid;
	ثابت حرف *referent = ref->هدف;
	صحيح error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	إذا (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   عدم, عدم);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = عدم;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, عدم, rename->err);
	إذا (error < 0)
		اذهب بیرون;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : عدم, عدم,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       عدم, rename->err);
	إذا (error < 0)
		اذهب بیرون;

	error = rename_one_reflog(ref->name, oid, rename);
	إذا (error < 0)
		اذهب بیرون;

	display_پیشرفت(rename->پیشرفت, ++rename->پیشرفت_nr);

بیرون:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	أرجع error;
}

ساكن صحيح migrate_file(بنية remote *remote)
{
	بنية strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	لكل (اندازه_t i = 0; i < remote->url.nr; i++)
		repo_config_مجموعه_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	لكل (صحيح i = 0; i < remote->افزودن.nr; i++)
		repo_config_مجموعه_multivar(the_repository, buf.buf, remote->افزودن.اقلام[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	لكل (صحيح i = 0; i < remote->fetch.nr; i++)
		repo_config_مجموعه_multivar(the_repository, buf.buf, remote->fetch.اقلام[i].raw, "^$", 0);
#اگر_تعریف_نشده WITH_BREAKING_CHANGES
	إذا (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_مسیر_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	وإلا إذا (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_مسیر_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#پایان_اگر /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	أرجع 0;
}

بنية افزودن_default_info
{
	ثابت حرف *old_name;
	تعداد config_scope scope;
	بنية strbuf origin;
	صحيح linenr;
};

ساكن صحيح config_read_افزودن_default(ثابت حرف *کلید, ثابت حرف *مقدار,
	ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت بنية کلید_مقدار_info *kvi = ctx->kvi;

	بنية افزودن_default_info* info = cb;
	إذا (strcmp(کلید, "remote.pushdefault") ||
	    !مقدار || strcmp(مقدار, info->old_name))
		أرجع 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	أرجع 0;
}

ساكن فراغ handle_افزودن_default(ثابت حرف* old_name, ثابت حرف* new_name)
{
	بنية افزودن_default_info افزودن_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_افزودن_default, &افزودن_default);
	إذا (افزودن_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	وإلا إذا (افزودن_default.scope >= CONFIG_SCOPE_LOCAL) {
		صحيح نتیجه = repo_config_مجموعه_gently(the_repository, "remote.pushDefault",
						    new_name);
		إذا (new_name && نتیجه && نتیجه != CONFIG_NOTHING_مجموعه)
			die(_("could not set '%s'"), "remote.pushDefault");
		وإلا إذا (!new_name && نتیجه && نتیجه != CONFIG_NOTHING_مجموعه)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} وإلا إذا (افزودن_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(افزودن_default.scope),
			افزودن_default.origin.buf, افزودن_default.linenr,
			old_name);
	}

	strbuf_release(&افزودن_default.origin);
}

ساكن ثابت حرف conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

ساكن صحيح mv(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
	      بنية repository *repo استفادهنشده)
{
	صحيح show_پیشرفت = isatty(2);
	بنية option options[] = {
		OPT_BOOL(0, "progress", &show_پیشرفت, N_("force progress reporting")),
		OPT_انتها()
	};
	بنية remote *oldremote, *newremote;
	بنية strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	بنية rename_info rename = {
		.err = &err,
	};
	صحيح refspecs_need_update = 0;
	صحيح نتیجه = 0;

	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_remote_rename_usage, 0);

	إذا (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	إذا (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		خروج(2);
	}

	إذا (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		أرجع migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	إذا (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		خروج(3);
	}

	إذا (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	إذا (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		نتیجه = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		اذهب بیرون;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	لكل (صحيح i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!جستجوی_رشته(oldremote->fetch.اقلام[i].raw,
						old_remote_context.buf);

	إذا (refspecs_need_update) {
		بنية refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		إذا (!rename.transaction)
			اذهب بیرون;

		إذا (show_پیشرفت)
			rename.پیشرفت = شروع_delayed_پیشرفت(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.پیشوند = buf.buf;

		نتیجه = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		إذا (نتیجه < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		نتیجه = ref_transaction_prepare(rename.transaction, &err);
		إذا (نتیجه < 0) {
			error("renaming remote references failed: %s", err.buf);
			إذا (نتیجه == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(عدم);
		}
	}

	إذا (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_مجموعه_multivar(the_repository, buf.buf, عدم, عدم, CONFIG_FLAGS_MULTI_REPLACE);
		لكل (صحيح i = 0; i < oldremote->fetch.nr; i++) {
			حرف *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.اقلام[i].raw);
			ptr = جستجوی_رشته(buf2.buf, old_remote_context.buf);
			إذا (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + طول_رشته(":refs/remotes/"),
					      طول_رشته(rename.old_name), rename.new_name,
					      طول_رشته(rename.new_name));
			} وإلا
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_مجموعه_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	لكل (اندازه_t i = 0; i < branch_list.nr; i++) {
		بنية string_list_قلم *قلم = branch_list.اقلام + i;
		بنية branch_info *info = قلم->util;
		إذا (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", قلم->string);
			repo_config_مجموعه(the_repository, buf.buf, rename.new_name);
		}
		إذا (info->افزودن_remote_name && !strcmp(info->افزودن_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", قلم->string);
			repo_config_مجموعه(the_repository, buf.buf, rename.new_name);
		}
	}

	إذا (refspecs_need_update) {
		نتیجه = ref_transaction_commit(rename.transaction, &err);
		إذا (نتیجه < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_پیشرفت(&rename.پیشرفت);

		handle_افزودن_default(rename.old_name, rename.new_name);
	}

بیرون:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	أرجع نتیجه;
}

ساكن صحيح rm(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
	      بنية repository *repo استفادهنشده)
{
	بنية option options[] = {
		OPT_انتها()
	};
	بنية remote *remote;
	بنية strbuf buf = STRBUF_INIT;
	بنية معلوم_remotes معلوم_remotes = { عدم, عدم };
	بنية string_list branches = STRING_LIST_INIT_DUP;
	بنية string_list skipped = STRING_LIST_INIT_DUP;
	بنية branches_for_remote cb_داده;
	صحيح نتیجه;

	memset(&cb_داده, 0, حجم(cb_داده));
	cb_داده.branches = &branches;
	cb_داده.skipped = &skipped;
	cb_داده.keep = &معلوم_remotes;

	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_remote_rm_usage, 0);
	إذا (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		خروج(2);
	}

	معلوم_remotes.مقصد_delete = remote;
	for_each_remote(جمع_کن_معلوم_remote, &معلوم_remotes);

	read_branches();
	لكل (اندازه_t i = 0; i < branch_list.nr; i++) {
		بنية string_list_قلم *قلم = branch_list.اقلام + i;
		بنية branch_info *info = قلم->util;
		إذا (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ثابت حرف *کلیدها[] = { "remote", "merge", عدم }, **k;
			لكل (k = کلیدها; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						قلم->string, *k);
				نتیجه = repo_config_مجموعه_gently(the_repository, buf.buf, عدم);
				إذا (نتیجه && نتیجه != CONFIG_NOTHING_مجموعه)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		إذا (info->افزودن_remote_name && !strcmp(info->افزودن_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", قلم->string);
			نتیجه = repo_config_مجموعه_gently(the_repository, buf.buf, عدم);
			إذا (نتیجه && نتیجه != CONFIG_NOTHING_مجموعه)
				die(_("could not unset '%s'"), buf.buf);
		}
	}

	/*
	 * We cannot just pass a function to for_each_ref() which deletes
	 * the branches one by one, since for_each_ref() relies on cached
	 * refs, which are invalidated when deleting a branch.
	 */
	cb_داده.remote = remote;
	نتیجه = refs_for_each_ref(get_main_ref_store(the_repository),
				   جمع_کن_branch_for_removal, &cb_داده);
	strbuf_release(&buf);

	إذا (!نتیجه)
		نتیجه = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	إذا (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		لكل (اندازه_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.اقلام[i].string);
	}
	string_list_clear(&skipped, 0);

	إذا (!نتیجه) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		إذا (repo_config_rename_section(the_repository, buf.buf, عدم) < 1) {
			نتیجه = error(_("Could not remove config section '%s'"), buf.buf);
			اذهب بیرون;
		}

		handle_افزودن_default(remote->name, عدم);
	}

بیرون:
	لكل (بنية معلوم_remote *r = معلوم_remotes.list; r;) {
		بنية معلوم_remote *بعدی = r->بعدی;
		حرر(r);
		r = بعدی;
	}
	strbuf_release(&buf);
	أرجع نتیجه;
}

ساكن فراغ clear_افزودن_info(فراغ *util, ثابت حرف *string استفادهنشده)
{
	بنية افزودن_info *info = util;
	حرر(info->dest);
	حرر(info);
}

ساكن فراغ free_remote_ref_حالتها(بنية ref_حالتها *حالتها)
{
	string_list_clear(&حالتها->new_refs, 0);
	string_list_clear(&حالتها->skipped, 0);
	string_list_clear(&حالتها->stale, 1);
	string_list_clear(&حالتها->tracked, 0);
	string_list_clear(&حالتها->heads, 0);
	string_list_clear_func(&حالتها->افزودن, clear_افزودن_info);
}

ساكن صحيح append_ref_مقصد_tracked_list(ثابت بنية reference *ref, فراغ *cb_داده)
{
	بنية ref_حالتها *حالتها = cb_داده;
	بنية refspec_قلم refspec;

	إذا (ref->flags & REF_ISSYMREF)
		أرجع 0;

	memset(&refspec, 0, حجم(refspec));
	refspec.dst = (حرف *)ref->name;
	إذا (!remote_پیدا_کن_tracking(حالتها->remote, &refspec)) {
		string_list_append(&حالتها->tracked, abbrev_branch(refspec.src));
		حرر(refspec.src);
	}

	أرجع 0;
}

ساكن صحيح get_remote_ref_حالتها(ثابت حرف *name,
				 بنية ref_حالتها *حالتها,
				 صحيح query)
{
	حالتها->remote = remote_get(name);
	إذا (!حالتها->remote)
		أرجع error(_("No such remote: '%s'"), name);

	read_branches();

	إذا (query) {
		بنية transport *transport;
		ثابت بنية ref *remote_refs;

		transport = transport_get(حالتها->remote, حالتها->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, عدم);

		حالتها->queried = 1;
		إذا (query & GET_REF_حالتها)
			get_ref_حالتها(remote_refs, حالتها);
		إذا (query & GET_سر_نامها)
			get_سر_نامها(remote_refs, حالتها);
		إذا (query & GET_افزودن_REF_حالتها)
			get_افزودن_ref_حالتها(remote_refs, حالتها);
		transport_disconnect(transport);
	} وإلا {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_مقصد_tracked_list, حالتها);
		string_list_مرتب_سازی(&حالتها->tracked);
		get_افزودن_ref_حالتها_noquery(حالتها);
	}

	أرجع 0;
}

بنية show_info {
	بنية string_list list;
	بنية ref_حالتها حالتها;
	صحيح عرض, width2;
	صحيح any_rebase;
};

#تعریف_کن SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.حالتها = REF_حالتها_INIT, \
}

ساكن صحيح جمع_کن_remote_مقصد_show_info(بنية string_list_قلم *قلم, فراغ *cb_داده)
{
	بنية show_info *info = cb_داده;
	صحيح n = طول_رشته(قلم->string);
	إذا (n > info->عرض)
		info->عرض = n;
	string_list_درج(&info->list, قلم->string);
	أرجع 0;
}

ساكن صحيح show_remote_info_قلم(بنية string_list_قلم *قلم, فراغ *cb_داده)
{
	بنية show_info *info = cb_داده;
	بنية ref_حالتها *حالتها = &info->حالتها;
	ثابت حرف *name = قلم->string;

	إذا (حالتها->queried) {
		ثابت حرف *fmt = "%s";
		ثابت حرف *arg = "";
		إذا (string_list_has_string(&حالتها->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = حالتها->remote->name;
		} وإلا إذا (string_list_has_string(&حالتها->tracked, name))
			arg = _(" tracked");
		وإلا إذا (string_list_has_string(&حالتها->skipped, name))
			arg = _(" skipped");
		وإلا إذا (string_list_has_string(&حالتها->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		وإلا
			arg = _(" ???");
		چاپ_کن("    %-*s", info->عرض, name);
		چاپ_کن(fmt, arg);
		چاپ_کن("\n");
	} وإلا
		چاپ_کن("    %s\n", name);

	أرجع 0;
}

ساكن صحيح جمع_کن_local_مقصد_show_info(بنية string_list_قلم *branch_قلم, فراغ *cb_داده)
{
	بنية show_info *show_info = cb_داده;
	بنية ref_حالتها *حالتها = &show_info->حالتها;
	بنية branch_info *branch_info = branch_قلم->util;
	بنية string_list_قلم *قلم;
	صحيح n;

	إذا (!branch_info->ادغام.nr || !branch_info->remote_name ||
	    strcmp(حالتها->remote->name, branch_info->remote_name))
		أرجع 0;
	إذا ((n = طول_رشته(branch_قلم->string)) > show_info->عرض)
		show_info->عرض = n;
	إذا (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	قلم = string_list_درج(&show_info->list, branch_قلم->string);
	قلم->util = branch_info;

	أرجع 0;
}

ساكن صحيح show_local_info_قلم(بنية string_list_قلم *قلم, فراغ *cb_داده)
{
	بنية show_info *show_info = cb_داده;
	بنية branch_info *branch_info = قلم->util;
	بنية string_list *ادغام = &branch_info->ادغام;
	صحيح عرض = show_info->عرض + 4;

	إذا (branch_info->rebase >= REBASE_TRUE && branch_info->ادغام.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			قلم->string);
		أرجع 0;
	}

	چاپ_کن("    %-*s ", show_info->عرض, قلم->string);
	إذا (branch_info->rebase >= REBASE_TRUE) {
		ثابت حرف *پیام;
		إذا (branch_info->rebase == REBASE_INTERACTIVE)
			پیام = _("rebases interactively onto remote %s");
		وإلا إذا (branch_info->rebase == REBASE_MERGES)
			پیام = _("rebases interactively (with merges) onto "
				"remote %s");
		وإلا
			پیام = _("rebases onto remote %s");
		printf_ln(پیام, ادغام->اقلام[0].string);
		أرجع 0;
	} وإلا إذا (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), ادغام->اقلام[0].string);
		عرض++;
	} وإلا {
		printf_ln(_("merges with remote %s"), ادغام->اقلام[0].string);
	}
	لكل (اندازه_t i = 1; i < ادغام->nr; i++)
		چاپ_کن(_("%-*s    and with remote %s\n"), عرض, "",
		       ادغام->اقلام[i].string);

	أرجع 0;
}

ساكن صحيح جمع_کن_افزودن_مقصد_show_info(بنية string_list_قلم *افزودن_قلم, فراغ *cb_داده)
{
	بنية show_info *show_info = cb_داده;
	بنية افزودن_info *افزودن_info = افزودن_قلم->util;
	بنية string_list_قلم *قلم;
	صحيح n;
	إذا ((n = طول_رشته(افزودن_قلم->string)) > show_info->عرض)
		show_info->عرض = n;
	إذا ((n = طول_رشته(افزودن_info->dest)) > show_info->width2)
		show_info->width2 = n;
	قلم = string_list_append(&show_info->list, افزودن_قلم->string);
	قلم->util = افزودن_قلم->util;
	أرجع 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
ساكن صحيح cmp_string_with_افزودن(ثابت فراغ *va, ثابت فراغ *vb)
{
	ثابت بنية string_list_قلم *a = va;
	ثابت بنية string_list_قلم *b = vb;
	ثابت بنية افزودن_info *a_افزودن = a->util;
	ثابت بنية افزودن_info *b_افزودن = b->util;
	صحيح cmp = strcmp(a->string, b->string);
	أرجع cmp ? cmp : strcmp(a_افزودن->dest, b_افزودن->dest);
}

ساكن صحيح show_افزودن_info_قلم(بنية string_list_قلم *قلم, فراغ *cb_داده)
{
	بنية show_info *show_info = cb_داده;
	بنية افزودن_info *افزودن_info = قلم->util;
	ثابت حرف *src = قلم->string, *وضعیت = عدم;

	بدل (افزودن_info->وضعیت) {
	حالة افزودن_وضعیت_CREATE:
		وضعیت = _("create");
		اكسر;
	حالة افزودن_وضعیت_DELETE:
		وضعیت = _("delete");
		src = _("(none)");
		اكسر;
	حالة افزودن_وضعیت_UPTODATE:
		وضعیت = _("up to date");
		اكسر;
	حالة افزودن_وضعیت_FASTFORWARD:
		وضعیت = _("fast-forwardable");
		اكسر;
	حالة افزودن_وضعیت_OUTOFDATE:
		وضعیت = _("local out of date");
		اكسر;
	حالة افزودن_وضعیت_NOTQUERIED:
		اكسر;
	}
	إذا (وضعیت) {
		إذا (افزودن_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->عرض, src,
			       show_info->width2, افزودن_info->dest, وضعیت);
		وإلا
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->عرض, src,
			       show_info->width2, افزودن_info->dest, وضعیت);
	} وإلا {
		إذا (افزودن_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->عرض, src,
			       افزودن_info->dest);
		وإلا
			printf_ln(_("    %-*s pushes to %s"), show_info->عرض, src,
			       افزودن_info->dest);
	}
	أرجع 0;
}

ساكن صحيح get_one_مدخل(بنية remote *remote, فراغ *priv)
{
	بنية string_list *list = priv;
	بنية strbuf remote_info_buf = STRBUF_INIT;
	بنية strvec *url;

	إذا (remote->url.nr > 0) {
		بنية strbuf promisor_config = STRBUF_INIT;
		ثابت حرف *partial_clone_filter = عدم;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		إذا (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, عدم);
	} وإلا
		string_list_append(list, remote->name)->util = عدم;
	url = افزودن_url_از_remote(remote);
	لكل (اندازه_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, عدم);
	}

	أرجع 0;
}

ساكن صحيح show_all(فراغ)
{
	بنية string_list list = STRING_LIST_INIT_DUP;
	صحيح نتیجه;

	نتیجه = for_each_remote(get_one_مدخل, &list);

	إذا (!نتیجه) {
		string_list_مرتب_سازی(&list);
		لكل (اندازه_t i = 0; i < list.nr; i++) {
			بنية string_list_قلم *قلم = list.اقلام + i;
			إذا (verbose)
				چاپ_کن("%s\t%s\n", قلم->string,
					قلم->util ? (ثابت حرف *)قلم->util : "");
			وإلا {
				إذا (i && !strcmp((قلم - 1)->string, قلم->string))
					استمر;
				چاپ_کن("%s\n", قلم->string);
			}
		}
	}
	string_list_clear(&list, 1);
	أرجع نتیجه;
}

ساكن صحيح show(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		بنية repository *repo استفادهنشده)
{
	صحيح no_query = 0, نتیجه = 0, query_flag = 0;
	بنية option options[] = {
		OPT_BOOL('n', عدم, &no_query, N_("do not query remotes")),
		OPT_انتها()
	};
	بنية show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_remote_show_usage,
			     0);

	إذا (argc < 1)
		أرجع show_all();

	إذا (!no_query)
		query_flag = (GET_REF_حالتها | GET_سر_نامها | GET_افزودن_REF_حالتها);

	لكل (; argc; argc--, argv++) {
		اندازه_t i;
		بنية strvec *url;

		get_remote_ref_حالتها(*argv, &info.حالتها, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.حالتها.remote->url.v[0]);
		url = افزودن_url_از_remote(info.حالتها.remote);
		لكل (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		إذا (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		إذا (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		وإلا إذا (!info.حالتها.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		وإلا إذا (info.حالتها.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.حالتها.heads.اقلام[0].string);
		وإلا {
			چاپ_کن(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			لكل (i = 0; i < info.حالتها.heads.nr; i++)
				چاپ_کن("    %s\n", info.حالتها.heads.اقلام[i].string);
		}

		/* remote branch info */
		info.عرض = 0;
		for_each_string_list(&info.حالتها.new_refs, جمع_کن_remote_مقصد_show_info, &info);
		for_each_string_list(&info.حالتها.skipped, جمع_کن_remote_مقصد_show_info, &info);
		for_each_string_list(&info.حالتها.tracked, جمع_کن_remote_مقصد_show_info, &info);
		for_each_string_list(&info.حالتها.stale, جمع_کن_remote_مقصد_show_info, &info);
		إذا (info.list.nr)
			printf_ln(Q_("  Remote branch:%s",
				     "  Remote branches:%s",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_remote_info_قلم, &info);
		string_list_clear(&info.list, 0);

		/* git pull info */
		info.عرض = 0;
		info.any_rebase = 0;
		for_each_string_list(&branch_list, جمع_کن_local_مقصد_show_info, &info);
		إذا (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_قلم, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		إذا (info.حالتها.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.عرض = info.width2 = 0;
		for_each_string_list(&info.حالتها.افزودن, جمع_کن_افزودن_مقصد_show_info, &info);
		QSORT(info.list.اقلام, info.list.nr, cmp_string_with_افزودن);
		إذا (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_افزودن_info_قلم, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_حالتها(&info.حالتها);
	}

	أرجع نتیجه;
}

ساكن فراغ report_مجموعه_سر_auto(ثابت حرف *remote, ثابت حرف *سر_name,
			بنية strbuf *b_local_سر, صحيح was_detached) {
	بنية strbuf buf_پیشوند = STRBUF_INIT;
	ثابت حرف *prev_سر = عدم;

	strbuf_addf(&buf_پیشوند, "refs/remotes/%s/", remote);
	skip_پیشوند(b_local_سر->buf, buf_پیشوند.buf, &prev_سر);

	إذا (prev_سر && !strcmp(prev_سر, سر_name))
		چاپ_کن(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, سر_name);
	وإلا إذا (prev_سر)
		چاپ_کن(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_سر, سر_name);
	وإلا إذا (!b_local_سر->len)
		چاپ_کن(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, سر_name);
	وإلا إذا (was_detached && b_local_سر->len)
		چاپ_کن(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_سر->buf, سر_name);
	وإلا
		چاپ_کن(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_سر->buf, سر_name);
	strbuf_release(&buf_پیشوند);
}

ساكن صحيح مجموعه_سر(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		    بنية repository *repo استفادهنشده)
{
	صحيح opt_a = 0, opt_d = 0, نتیجه = 0, was_detached;
	بنية strbuf b_سر = STRBUF_INIT, b_remote_سر = STRBUF_INIT,
		b_local_سر = STRBUF_INIT;
	حرف *سر_name = عدم;
	بنية ref_store *refs = get_main_ref_store(the_repository);
	بنية remote *remote;

	بنية option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_انتها()
	};
	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	إذا (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_سر, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	إذا (!opt_a && !opt_d && argc == 2) {
		سر_name = xstrdup(argv[1]);
	} وإلا إذا (opt_a && !opt_d && argc == 1) {
		بنية ref_حالتها حالتها = REF_حالتها_INIT;
		get_remote_ref_حالتها(argv[0], &حالتها, GET_سر_نامها);
		إذا (!حالتها.heads.nr)
			نتیجه |= error(_("Cannot determine remote HEAD"));
		وإلا إذا (حالتها.heads.nr > 1) {
			نتیجه |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			لكل (اندازه_t i = 0; i < حالتها.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], حالتها.heads.اقلام[i].string);
		} وإلا
			سر_name = xstrdup(حالتها.heads.اقلام[0].string);
		free_remote_ref_حالتها(&حالتها);
	} وإلا إذا (opt_d && !opt_a && argc == 1) {
		إذا (refs_delete_ref(refs, عدم, b_سر.buf, عدم, REF_NO_DEREF))
			نتیجه |= error(_("Could not delete %s"), b_سر.buf);
	} وإلا
		usage_with_options(builtin_remote_sethead_usage, options);

	إذا (!سر_name)
		اذهب cleanup;
	strbuf_addf(&b_remote_سر, "refs/remotes/%s/%s", argv[0], سر_name);
	إذا (!refs_ref_exists(refs, b_remote_سر.buf)) {
		نتیجه |= error(_("Not a valid ref: %s"), b_remote_سر.buf);
		اذهب cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_سر.buf, b_remote_سر.buf,
			"remote set-head", &b_local_سر, 0);
	إذا (was_detached == -1) {
		نتیجه |= error(_("Could not set up %s"), b_سر.buf);
		اذهب cleanup;
	}
	إذا (opt_a)
		report_مجموعه_سر_auto(argv[0], سر_name, &b_local_سر, was_detached);
	إذا (remote->follow_remote_سر == FOLLOW_REMOTE_ALWAYS) {
		بنية strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_مجموعه(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	حرر(سر_name);
	strbuf_release(&b_سر);
	strbuf_release(&b_remote_سر);
	strbuf_release(&b_local_سر);
	أرجع نتیجه;
}

ساكن صحيح prune_remote(ثابت حرف *remote, صحيح dry_پیاپی)
{
	صحيح نتیجه = 0;
	بنية ref_حالتها حالتها = REF_حالتها_INIT;
	بنية string_list refs_مقصد_prune = STRING_LIST_INIT_NODUP;
	بنية string_list_قلم *قلم;

	get_remote_ref_حالتها(remote, &حالتها, GET_REF_حالتها);

	إذا (!حالتها.stale.nr) {
		free_remote_ref_حالتها(&حالتها);
		أرجع 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), حالتها.remote->url.v[0]);

	for_each_string_list_قلم(قلم, &حالتها.stale)
		string_list_append(&refs_مقصد_prune, قلم->util);
	string_list_مرتب_سازی(&refs_مقصد_prune);

	إذا (!dry_پیاپی)
		نتیجه |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_مقصد_prune, 0);

	for_each_string_list_قلم(قلم, &حالتها.stale) {
		ثابت حرف *refname = قلم->util;

		إذا (dry_پیاپی)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		وإلا
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_پیاپی, &refs_مقصد_prune);

	string_list_clear(&refs_مقصد_prune, 0);
	free_remote_ref_حالتها(&حالتها);
	أرجع نتیجه;
}

ساكن صحيح prune(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		 بنية repository *repo استفادهنشده)
{
	صحيح dry_پیاپی = 0, نتیجه = 0;
	بنية option options[] = {
		OPT_DRY_پیاپی(&dry_پیاپی, N_("dry run")),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_remote_prune_usage, 0);

	إذا (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	لكل (; argc; argc--, argv++)
		نتیجه |= prune_remote(*argv, dry_پیاپی);

	أرجع نتیجه;
}

ساكن صحيح get_remote_default(ثابت حرف *کلید, ثابت حرف *مقدار استفادهنشده,
			      ثابت بنية config_context *ctx استفادهنشده,
			      فراغ *priv)
{
	إذا (strcmp(کلید, "remotes.default") == 0) {
		صحيح *یافت_2 = priv;
		*یافت_2 = 1;
	}
	أرجع 0;
}

ساكن صحيح update(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		  بنية repository *repo استفادهنشده)
{
	صحيح i, prune = -1;
	بنية option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_انتها()
	};
	بنية فرزند_process cmd = فرزند_PROCESS_INIT;
	صحيح default_defined = 0;

	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_افزودن(&cmd.آرگومانها, "fetch");

	إذا (prune != -1)
		strvec_افزودن(&cmd.آرگومانها, prune ? "--prune" : "--no-prune");
	إذا (verbose)
		strvec_افزودن(&cmd.آرگومانها, "-v");
	strvec_افزودن(&cmd.آرگومانها, "--multiple");
	إذا (argc < 2)
		strvec_افزودن(&cmd.آرگومانها, "default");
	لكل (i = 1; i < argc; i++)
		strvec_افزودن(&cmd.آرگومانها, argv[i]);

	إذا (strcmp(cmd.آرگومانها.v[cmd.آرگومانها.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		إذا (!default_defined) {
			strvec_برداشت(&cmd.آرگومانها);
			strvec_افزودن(&cmd.آرگومانها, "--all");
		}
	}

	cmd.git_cmd = 1;
	أرجع پیاپی_command(&cmd);
}

ساكن صحيح remove_all_fetch_refspecs(ثابت حرف *کلید)
{
	أرجع repo_config_مجموعه_multivar_gently(the_repository, کلید, عدم, عدم,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

ساكن فراغ جمع_کن_branches(بنية remote *remote, ثابت حرف **branches,
			 ثابت حرف *کلید)
{
	ثابت حرف *remotename = remote->name;
	صحيح mirror = remote->mirror;
	بنية strbuf refspec = STRBUF_INIT;

	لكل (; *branches; branches++)
		جمع_کن_branch(کلید, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

ساكن صحيح مجموعه_remote_branches(ثابت حرف *remotename, ثابت حرف **branches,
				صحيح جمع_کن_نما)
{
	بنية strbuf کلید = STRBUF_INIT;
	بنية remote *remote;

	strbuf_addf(&کلید, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		خروج(2);
	}

	إذا (!جمع_کن_نما && remove_all_fetch_refspecs(کلید.buf)) {
		strbuf_release(&کلید);
		أرجع 1;
	}
	جمع_کن_branches(remote, branches, کلید.buf);

	strbuf_release(&کلید);
	أرجع 0;
}

ساكن صحيح مجموعه_branches(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
			بنية repository *repo استفادهنشده)
{
	صحيح جمع_کن_نما = 0;
	بنية option options[] = {
		OPT_BOOL('\0', "add", &جمع_کن_نما, N_("add branch")),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_remote_setbranches_usage, 0);
	إذا (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = عدم;

	أرجع مجموعه_remote_branches(argv[0], argv + 1, جمع_کن_نما);
}

ساكن صحيح get_url(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		   بنية repository *repo استفادهنشده)
{
	صحيح افزودن_نما = 0, all_نما = 0;
	ثابت حرف *remotename = عدم;
	بنية remote *remote;
	بنية strvec *url;
	بنية option options[] = {
		OPT_BOOL('\0', "push", &افزودن_نما,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_نما,
			 N_("return all URLs")),
		OPT_انتها()
	};
	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_remote_geturl_usage, 0);

	إذا (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		خروج(2);
	}

	url = افزودن_نما ? افزودن_url_از_remote(remote) : &remote->url;

	إذا (all_نما) {
		لكل (اندازه_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} وإلا {
		printf_ln("%s", url->v[0]);
	}

	أرجع 0;
}

ساكن صحيح مجموعه_url(صحيح argc, ثابت حرف **argv, ثابت حرف *پیشوند,
		   بنية repository *repo استفادهنشده)
{
	صحيح افزودن_نما = 0, جمع_کن_نما = 0, delete_نما = 0;
	صحيح matches = 0, negative_matches = 0;
	ثابت حرف *remotename = عدم;
	ثابت حرف *newurl = عدم;
	ثابت حرف *oldurl = عدم;
	بنية remote *remote;
	regex_t old_regex;
	بنية strvec *urlset;
	بنية strbuf name_buf = STRBUF_INIT;
	بنية option options[] = {
		OPT_BOOL('\0', "push", &افزودن_نما,
			 N_("manipulate push URLs")),
		OPT_BOOL('\0', "add", &جمع_کن_نما,
			 N_("add URL")),
		OPT_BOOL('\0', "delete", &delete_نما,
			    N_("delete URLs")),
		OPT_انتها()
	};
	argc = parse_options(argc, argv, پیشوند, options,
			     builtin_remote_seturl_usage,
			     PARSE_OPT_KEEP_ARGV0);

	إذا (جمع_کن_نما && delete_نما)
		die(_("--add --delete doesn't make sense"));

	إذا (argc < 3 || argc > 4 || ((جمع_کن_نما || delete_نما) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	إذا (argc > 3)
		oldurl = argv[3];

	إذا (delete_نما)
		oldurl = newurl;

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		خروج(2);
	}

	إذا (افزودن_نما) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} وإلا {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	إذا ((!oldurl && !delete_نما) || جمع_کن_نما) {
		إذا (جمع_کن_نما)
			repo_config_مجموعه_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		وإلا
			repo_config_مجموعه(the_repository, name_buf.buf, newurl);
		اذهب بیرون;
	}

	/* Old URL specified. Demand that one matches. */
	إذا (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	لكل (اندازه_t i = 0; i < urlset->nr; i++)
		إذا (!regexec(&old_regex, urlset->v[i], 0, عدم, 0))
			matches++;
		وإلا
			negative_matches++;
	إذا (!delete_نما && !matches)
		die(_("No such URL found: %s"), oldurl);
	إذا (delete_نما && !negative_matches && !افزودن_نما)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	إذا (!delete_نما)
		repo_config_مجموعه_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	وإلا
		repo_config_مجموعه_multivar(the_repository, name_buf.buf, عدم, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
بیرون:
	strbuf_release(&name_buf);
	أرجع 0;
}

صحيح cmd_remote(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *پیشوند,
	       بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, جمع_کن),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, عدم, PARSE_OPT_NOCOMPLETE),
		OPT_SUBCOMMAND("remove", &fn, rm),
		OPT_SUBCOMMAND("set-head", &fn, مجموعه_سر),
		OPT_SUBCOMMAND("set-branches", &fn, مجموعه_branches),
		OPT_SUBCOMMAND("get-url", &fn, get_url),
		OPT_SUBCOMMAND("set-url", &fn, مجموعه_url),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("update", &fn, update),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, options, builtin_remote_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	إذا (fn) {
		أرجع !!fn(argc, argv, پیشوند, repo);
	} وإلا {
		إذا (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		أرجع !!show_all();
	}
}
