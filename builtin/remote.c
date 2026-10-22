#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

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

ساكن ثابت حرف * ثابت builtin_remote_جمع_کړه_usage[] = {
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

#تعریف_کړه GET_REF_حالتونه (1<<0)
#تعریف_کړه GET_سر_نومونه (1<<1)
#تعریف_کړه GET_ټېل_REF_حالتونه (1<<2)

ساكن صحيح verbose;

ساكن صحيح fetch_remote(ثابت حرف *name)
{
	بنية ماشوم_process cmd = ماشوم_PROCESS_INIT;

	strvec_ټېل(&cmd.دلیلونه, "fetch");
	إذا (verbose)
		strvec_ټېل(&cmd.دلیلونه, "-v");
	strvec_ټېل(&cmd.دلیلونه, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	إذا (پرلپسې_command(&cmd))
		أرجع error(_("Could not fetch %s"), name);
	أرجع 0;
}

تعداد {
	نښانونه_UNSET = 0,
	نښانونه_DEFAULT = 1,
	نښانونه_سېټ = 2
};

#تعریف_کړه MIRROR_NONE 0
#تعریف_کړه MIRROR_FETCH 1
#تعریف_کړه MIRROR_ټېل 2
#تعریف_کړه MIRROR_BOTH (MIRROR_FETCH|MIRROR_ټېل)

ساكن فراغ جمع_کړه_branch(ثابت حرف *کلی, ثابت حرف *branchname,
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
	repo_config_سېټ_multivar(the_repository, کلی, tmp->buf, "^$", 0);
}

ساكن ثابت حرف mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

ساكن صحيح parse_mirror_opt(ثابت بنية option *opt, ثابت حرف *arg, صحيح not)
{
	بینښه *mirror = opt->ارزښت;
	إذا (not)
		*mirror = MIRROR_NONE;
	وإلا إذا (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	وإلا إذا (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	وإلا إذا (!strcmp(arg, "push"))
		*mirror = MIRROR_ټېل;
	وإلا
		أرجع error(_("unknown --mirror argument: %s"), arg);
	أرجع 0;
}

ساكن صحيح check_remote_collision(بنية remote *remote, فراغ *معلومات)
{
	ثابت حرف *name = معلومات;
	ثابت حرف *p;

	إذا (skip_مختاړی(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	إذا (skip_مختاړی(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	أرجع 0;
}

ساكن صحيح جمع_کړه(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
	       بنية repository *repo ناکارول_شوی)
{
	صحيح fetch = 0, fetch_نښانونه = نښانونه_DEFAULT;
	بینښه mirror = MIRROR_NONE;
	بنية string_list track = STRING_LIST_INIT_NODUP;
	ثابت حرف *master = عدم;
	بنية remote *remote;
	بنية strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ثابت حرف *name, *url;
	صحيح پایله = 0;

	بنية option options[] = {
		OPT_BOOL('f', "fetch", &fetch, N_("fetch the remote branches")),
		OPT_سېټ_INT(0, "tags", &fetch_نښانونه,
			    N_("import all tags and associated objects when fetching\n"
			       "or do not fetch any tag at all (--no-tags)"),
			    نښانونه_سېټ),
		OPT_STRING_LIST('t', "track", &track, N_("branch"),
				N_("branch(es) to track")),
		OPT_STRING('m', "master", &master, N_("branch"), N_("master branch")),
		OPT_CALLBACK_F(0, "mirror", &mirror, "(push|fetch)",
			N_("set up remote as a mirror to push to or fetch from"),
			PARSE_OPT_OPTARG | PARSE_OPT_COMP_ARG, parse_mirror_opt),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options,
			     builtin_remote_جمع_کړه_usage, 0);

	إذا (argc != 2)
		usage_with_options(builtin_remote_جمع_کړه_usage, options);

	إذا (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	إذا (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	إذا (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		وتل(3);
	}

	إذا (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (فراغ *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_سېټ(the_repository, buf.buf, url);

	إذا (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		إذا (track.nr == 0)
			string_list_append(&track, "*");
		لكل (اندازه_t i = 0; i < track.nr; i++) {
			جمع_کړه_branch(buf.buf, track.توکي[i].string,
				   name, mirror, &buf2);
		}
	}

	إذا (mirror & MIRROR_ټېل) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_سېټ(the_repository, buf.buf, "true");
	}

	إذا (fetch_نښانونه != نښانونه_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_سېټ(the_repository, buf.buf,
				fetch_نښانونه == نښانونه_سېټ ? "--tags" : "--no-tags");
	}

	إذا (fetch && fetch_remote(name)) {
		پایله = 1;
		اذهب بهر;
	}

	إذا (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		إذا (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			پایله = error(_("Could not setup master '%s'"), master);
	}

بهر:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	أرجع پایله;
}

بنية branch_info {
	حرف *remote_name;
	بنية string_list یوځای;
	تعداد rebase_type rebase;
	حرف *ټېل_remote_name;
};

ساكن بنية string_list branch_list = STRING_LIST_INIT_DUP;

ساكن ثابت حرف *abbrev_ref(ثابت حرف *name, ثابت حرف *مختاړی)
{
	skip_مختاړی(name, مختاړی, &name);
	أرجع name;
}
#تعریف_کړه abbrev_branch(name) abbrev_ref((name), "refs/heads/")

ساكن صحيح config_read_branches(ثابت حرف *کلی, ثابت حرف *ارزښت,
				ثابت بنية config_context *ctx ناکارول_شوی,
				فراغ *معلومات ناکارول_شوی)
{
	ثابت حرف *orig_کلی = کلی;
	حرف *name;
	بنية string_list_توکی *توکی;
	بنية branch_info *info;
	تعداد { REMOTE, یوځای_2, REBASE, ټېل_REMOTE } type;
	اندازه_t کلی_len;

	إذا (!starts_with(کلی, "branch."))
		أرجع 0;

	کلی += مزي_اوږدوالی("branch.");
	إذا (strip_suffix(کلی, ".remote", &کلی_len))
		type = REMOTE;
	وإلا إذا (strip_suffix(کلی, ".merge", &کلی_len))
		type = یوځای_2;
	وإلا إذا (strip_suffix(کلی, ".rebase", &کلی_len))
		type = REBASE;
	وإلا إذا (strip_suffix(کلی, ".pushremote", &کلی_len))
		type = ټېل_REMOTE;
	وإلا
		أرجع 0;

	name = xmemdupz(کلی, کلی_len);
	توکی = string_list_ورننباسه(&branch_list, name);

	إذا (!توکی->util)
		توکی->util = xcalloc(1, حجم(بنية branch_info));
	info = توکی->util;
	بدل (type) {
	حالة REMOTE:
		إذا (info->remote_name)
			warning(_("more than one %s"), orig_کلی);
		info->remote_name = xstrdup(ارزښت);
		اكسر;
	حالة یوځای_2: {
		ثابت حرف *space = strchr(ارزښت, ' ');
		ارزښت = abbrev_branch(ارزښت);
		طالما (space) {
			حرف *یوځای;
			یوځای = xstrndup(ارزښت, space - ارزښت);
			string_list_append(&info->یوځای, یوځای);
			ارزښت = abbrev_branch(space + 1);
			space = strchr(ارزښت, ' ');
		}
		string_list_append(&info->یوځای, xstrdup(ارزښت));
		اكسر;
	}
	حالة REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_ارزښت(ارزښت);
		إذا (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, ارزښت);
		اكسر;
	حالة ټېل_REMOTE:
		إذا (info->ټېل_remote_name)
			warning(_("more than one %s"), orig_کلی);
		info->ټېل_remote_name = xstrdup(ارزښت);
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

بنية ref_حالتونه {
	بنية remote *remote;
	بنية string_list new_refs, skipped, stale, tracked, heads, ټېل;
	صحيح queried;
};

#تعریف_کړه REF_حالتونه_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.ټېل = STRING_LIST_INIT_DUP, \
}

ساكن صحيح get_ref_حالتونه(ثابت بنية ref *remote_refs, بنية ref_حالتونه *حالتونه)
{
	بنية ref *fetch_map = عدم, **لکۍ = &fetch_map;
	بنية ref *ref, *stale_refs;
	صحيح i;

	لكل (i = 0; i < حالتونه->remote->fetch.nr; i++)
		إذا (get_fetch_map(remote_refs, &حالتونه->remote->fetch.توکي[i], &لکۍ, 1))
			die(_("Could not get fetch map for refspec %s"),
				حالتونه->remote->fetch.توکي[i].raw);

	لكل (ref = fetch_map; ref; ref = ref->بل) {
		إذا (refname_matches_negative_refspec_توکی(ref->name, &حالتونه->remote->fetch))
			string_list_append(&حالتونه->skipped, abbrev_branch(ref->name));
		وإلا إذا (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&حالتونه->new_refs, abbrev_branch(ref->name));
		وإلا
			string_list_append(&حالتونه->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&حالتونه->remote->fetch, fetch_map);
	لكل (ref = stale_refs; ref; ref = ref->بل) {
		بنية string_list_توکی *توکی =
			string_list_append(&حالتونه->stale, abbrev_branch(ref->name));
		توکی->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_ترتیبول(&حالتونه->new_refs);
	string_list_ترتیبول(&حالتونه->skipped);
	string_list_ترتیبول(&حالتونه->tracked);
	string_list_ترتیبول(&حالتونه->stale);

	أرجع 0;
}

بنية ټېل_info {
	حرف *dest;
	صحيح forced;
	تعداد {
		ټېل_حالت_CREATE = 0,
		ټېل_حالت_DELETE,
		ټېل_حالت_UPTODATE,
		ټېل_حالت_FASTFORWARD,
		ټېل_حالت_OUTOFDATE,
		ټېل_حالت_NOTQUERIED
	} حالت;
};

ساكن صحيح get_ټېل_ref_حالتونه(ثابت بنية ref *remote_refs,
	بنية ref_حالتونه *حالتونه)
{
	بنية remote *remote = حالتونه->remote;
	بنية ref *ref, *local_refs, *ټېل_map;
	إذا (remote->mirror)
		أرجع 0;

	local_refs = get_local_heads();
	ټېل_map = copy_ref_list(remote_refs);

	سمون_ټېل_refs(local_refs, &ټېل_map, &remote->ټېل, سمون_REFS_NONE);

	لكل (ref = ټېل_map; ref; ref = ref->بل) {
		بنية string_list_توکی *توکی;
		بنية ټېل_info *info;

		إذا (!ref->peer_ref)
			استمر;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		توکی = string_list_append(&حالتونه->ټېل,
					  abbrev_branch(ref->peer_ref->name));
		توکی->util = xcalloc(1, حجم(بنية ټېل_info));
		info = توکی->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		إذا (is_null_oid(&ref->new_oid)) {
			info->حالت = ټېل_حالت_DELETE;
		} وإلا إذا (oideq(&ref->old_oid, &ref->new_oid))
			info->حالت = ټېل_حالت_UPTODATE;
		وإلا إذا (is_null_oid(&ref->old_oid))
			info->حالت = ټېل_حالت_CREATE;
		وإلا إذا (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->حالت = ټېل_حالت_FASTFORWARD;
		وإلا
			info->حالت = ټېل_حالت_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(ټېل_map);
	أرجع 0;
}

ساكن صحيح get_ټېل_ref_حالتونه_noquery(بنية ref_حالتونه *حالتونه)
{
	صحيح i;
	بنية remote *remote = حالتونه->remote;
	بنية string_list_توکی *توکی;
	بنية ټېل_info *info;

	إذا (remote->mirror)
		أرجع 0;

	إذا (!remote->ټېل.nr) {
		توکی = string_list_append(&حالتونه->ټېل, _("(matching)"));
		info = توکی->util = xcalloc(1, حجم(بنية ټېل_info));
		info->حالت = ټېل_حالت_NOTQUERIED;
		info->dest = xstrdup(توکی->string);
	}
	لكل (i = 0; i < remote->ټېل.nr; i++) {
		ثابت بنية refspec_توکی *spec = &remote->ټېل.توکي[i];
		إذا (spec->matching)
			توکی = string_list_append(&حالتونه->ټېل, _("(matching)"));
		وإلا إذا (مزي_اوږدوالی(spec->src))
			توکی = string_list_append(&حالتونه->ټېل, spec->src);
		وإلا
			توکی = string_list_append(&حالتونه->ټېل, _("(delete)"));

		info = توکی->util = xcalloc(1, حجم(بنية ټېل_info));
		info->forced = spec->force;
		info->حالت = ټېل_حالت_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : توکی->string);
	}
	أرجع 0;
}

ساكن صحيح get_سر_نومونه(ثابت بنية ref *remote_refs, بنية ref_حالتونه *حالتونه)
{
	بنية ref *ref, *matches;
	بنية ref *fetch_map = عدم, **fetch_map_لکۍ = &fetch_map;
	بنية refspec_توکی refspec = {
		.force = 0,
		.نمونه = 1,
		.src = (حرف *) "refs/heads/*",
		.dst = (حرف *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_لکۍ, 0);
	matches = guess_remote_سر(پیدا_کړه_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_سر_ALL);
	لكل (ref = matches; ref; ref = ref->بل)
		string_list_append(&حالتونه->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	أرجع 0;
}

بنية معلوم_remote {
	بنية معلوم_remote *بل;
	بنية remote *remote;
};

بنية معلوم_remotes {
	بنية remote *منزل_delete;
	بنية معلوم_remote *list;
};

ساكن صحيح جمع_کړه_معلوم_remote(بنية remote *remote, فراغ *cb_معلومات)
{
	بنية معلوم_remotes *all = cb_معلومات;
	بنية معلوم_remote *r;

	إذا (!strcmp(all->منزل_delete->name, remote->name))
		أرجع 0;

	r = xmalloc(حجم(*r));
	r->remote = remote;
	r->بل = all->list;
	all->list = r;
	أرجع 0;
}

بنية branches_for_remote {
	بنية remote *remote;
	بنية string_list *branches, *skipped;
	بنية معلوم_remotes *keep;
};

ساكن صحيح جمع_کړه_branch_for_removal(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية branches_for_remote *branches = cb_معلومات;
	بنية refspec_توکی refspec;
	بنية معلوم_remote *kr;

	memset(&refspec, 0, حجم(refspec));
	refspec.dst = (حرف *)ref->name;
	إذا (remote_پیدا_کړه_tracking(branches->remote, &refspec))
		أرجع 0;
	حرر(refspec.src);

	/* don't delete a branch if another remote also uses it */
	لكل (kr = branches->keep->list; kr; kr = kr->بل) {
		memset(&refspec, 0, حجم(refspec));
		refspec.dst = (حرف *)ref->name;
		إذا (!remote_پیدا_کړه_tracking(kr->remote, &refspec)) {
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
	بنية پرمختګ *پرمختګ;
	بنية strbuf *err;
	uint32_t پرمختګ_nr;
	uint64_t شاخص;
};

ساكن فراغ compute_renamed_ref(بنية rename_info *rename,
				ثابت حرف *refname,
				بنية strbuf *بهر)
{
	strbuf_reset(بهر);
	strbuf_addstr(بهر, refname);
	strbuf_splice(بهر, مزي_اوږدوالی("refs/remotes/"), مزي_اوږدوالی(rename->old_name),
		      rename->new_name, مزي_اوږدوالی(rename->new_name));
}

ساكن صحيح rename_one_reflog_ثبت(ثابت حرف *old_refname,
				   بنية object_id *old_oid,
				   بنية object_id *new_oid,
				   ثابت حرف *committer,
				   timestamp_t timestamp, صحيح tz,
				   ثابت حرف *پیغام, فراغ *cb_معلومات)
{
	بنية rename_info *rename = cb_معلومات;
	بنية strbuf new_refname = STRBUF_INIT;
	بنية strbuf بې_اغېزه_عنصر = STRBUF_INIT;
	بنية strbuf name = STRBUF_INIT;
	بنية strbuf mail = STRBUF_INIT;
	بنية ident_وېشل ident;
	ثابت حرف *date;
	صحيح error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	إذا (وېشل_ident_کرښه(&ident, committer, مزي_اوږدوالی(committer)) < 0) {
		error = -1;
		اذهب بهر;
	}

	strbuf_جمع_کړه(&name, ident.name_begin, ident.name_پای_ټکی - ident.name_begin);
	strbuf_جمع_کړه(&mail, ident.mail_begin, ident.mail_پای_ټکی - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_ډېرپېښ(نورمال));
	strbuf_addstr(&بې_اغېزه_عنصر, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, بې_اغېزه_عنصر.buf, پیغام,
					      rename->شاخص++, rename->err);

بهر:
	strbuf_release(&new_refname);
	strbuf_release(&بې_اغېزه_عنصر);
	strbuf_release(&name);
	strbuf_release(&mail);
	أرجع error;
}

ساكن صحيح rename_one_reflog(ثابت حرف *old_refname,
			     ثابت بنية object_id *old_oid,
			     بنية rename_info *rename)
{
	بنية strbuf new_refname = STRBUF_INIT;
	بنية strbuf پیغام_2 = STRBUF_INIT;
	صحيح error;

	إذا (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		أرجع 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_ثبت, rename);
	إذا (error < 0)
		اذهب بهر;

	compute_renamed_ref(rename, old_refname, &new_refname);

	/*
	 * Manually write the reflog entry for the now-renamed ref. We cannot
	 * rely on `rename_one_ref()` to do this for us as that would screw
	 * over order in which reflog entries are being written.
	 *
	 * Furthermore, we only append the entry in case the reference
	 * resolves. Missing references shouldn't have reflogs anyway.
	 */
	strbuf_addf(&پیغام_2, "remote: renamed %s to %s", old_refname,
		    new_refname.buf);

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      old_oid, old_oid, git_committer_info(0),
					      پیغام_2.buf, rename->شاخص++, rename->err);
	إذا (error < 0)
		أرجع error;

بهر:
	strbuf_release(&new_refname);
	strbuf_release(&پیغام_2);
	أرجع error;
}

ساكن صحيح rename_one_ref(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية strbuf new_referent = STRBUF_INIT;
	بنية strbuf new_refname = STRBUF_INIT;
	بنية rename_info *rename = cb_معلومات;
	ثابت بنية object_id *oid = ref->oid;
	ثابت حرف *referent = ref->موخه;
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
		اذهب بهر;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : عدم, عدم,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       عدم, rename->err);
	إذا (error < 0)
		اذهب بهر;

	error = rename_one_reflog(ref->name, oid, rename);
	إذا (error < 0)
		اذهب بهر;

	display_پرمختګ(rename->پرمختګ, ++rename->پرمختګ_nr);

بهر:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	أرجع error;
}

ساكن صحيح migrate_file(بنية remote *remote)
{
	بنية strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	لكل (اندازه_t i = 0; i < remote->url.nr; i++)
		repo_config_سېټ_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	لكل (صحيح i = 0; i < remote->ټېل.nr; i++)
		repo_config_سېټ_multivar(the_repository, buf.buf, remote->ټېل.توکي[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	لكل (صحيح i = 0; i < remote->fetch.nr; i++)
		repo_config_سېټ_multivar(the_repository, buf.buf, remote->fetch.توکي[i].raw, "^$", 0);
#که_نه_تعریف_شوی WITH_BREAKING_CHANGES
	إذا (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_لار_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	وإلا إذا (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_لار_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#د_که_پای /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	أرجع 0;
}

بنية ټېل_default_info
{
	ثابت حرف *old_name;
	تعداد config_scope scope;
	بنية strbuf origin;
	صحيح linenr;
};

ساكن صحيح config_read_ټېل_default(ثابت حرف *کلی, ثابت حرف *ارزښت,
	ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت بنية کلی_ارزښت_info *kvi = ctx->kvi;

	بنية ټېل_default_info* info = cb;
	إذا (strcmp(کلی, "remote.pushdefault") ||
	    !ارزښت || strcmp(ارزښت, info->old_name))
		أرجع 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	أرجع 0;
}

ساكن فراغ handle_ټېل_default(ثابت حرف* old_name, ثابت حرف* new_name)
{
	بنية ټېل_default_info ټېل_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_ټېل_default, &ټېل_default);
	إذا (ټېل_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	وإلا إذا (ټېل_default.scope >= CONFIG_SCOPE_LOCAL) {
		صحيح پایله = repo_config_سېټ_gently(the_repository, "remote.pushDefault",
						    new_name);
		إذا (new_name && پایله && پایله != CONFIG_NOTHING_سېټ)
			die(_("could not set '%s'"), "remote.pushDefault");
		وإلا إذا (!new_name && پایله && پایله != CONFIG_NOTHING_سېټ)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} وإلا إذا (ټېل_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(ټېل_default.scope),
			ټېل_default.origin.buf, ټېل_default.linenr,
			old_name);
	}

	strbuf_release(&ټېل_default.origin);
}

ساكن ثابت حرف conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

ساكن صحيح mv(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
	      بنية repository *repo ناکارول_شوی)
{
	صحيح show_پرمختګ = isatty(2);
	بنية option options[] = {
		OPT_BOOL(0, "progress", &show_پرمختګ, N_("force progress reporting")),
		OPT_پای_ټکی()
	};
	بنية remote *oldremote, *newremote;
	بنية strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	بنية rename_info rename = {
		.err = &err,
	};
	صحيح refspecs_need_update = 0;
	صحيح پایله = 0;

	argc = parse_options(argc, argv, مختاړی, options,
			     builtin_remote_rename_usage, 0);

	إذا (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	إذا (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		وتل(2);
	}

	إذا (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		أرجع migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	إذا (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		وتل(3);
	}

	إذا (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	إذا (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		پایله = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		اذهب بهر;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	لكل (صحيح i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!مزي_پلټنه(oldremote->fetch.توکي[i].raw,
						old_remote_context.buf);

	إذا (refspecs_need_update) {
		بنية refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		إذا (!rename.transaction)
			اذهب بهر;

		إذا (show_پرمختګ)
			rename.پرمختګ = پیل_delayed_پرمختګ(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.مختاړی = buf.buf;

		پایله = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		إذا (پایله < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		پایله = ref_transaction_prepare(rename.transaction, &err);
		إذا (پایله < 0) {
			error("renaming remote references failed: %s", err.buf);
			إذا (پایله == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(عدم);
		}
	}

	إذا (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_سېټ_multivar(the_repository, buf.buf, عدم, عدم, CONFIG_FLAGS_MULTI_REPLACE);
		لكل (صحيح i = 0; i < oldremote->fetch.nr; i++) {
			حرف *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.توکي[i].raw);
			ptr = مزي_پلټنه(buf2.buf, old_remote_context.buf);
			إذا (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + مزي_اوږدوالی(":refs/remotes/"),
					      مزي_اوږدوالی(rename.old_name), rename.new_name,
					      مزي_اوږدوالی(rename.new_name));
			} وإلا
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_سېټ_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	لكل (اندازه_t i = 0; i < branch_list.nr; i++) {
		بنية string_list_توکی *توکی = branch_list.توکي + i;
		بنية branch_info *info = توکی->util;
		إذا (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", توکی->string);
			repo_config_سېټ(the_repository, buf.buf, rename.new_name);
		}
		إذا (info->ټېل_remote_name && !strcmp(info->ټېل_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", توکی->string);
			repo_config_سېټ(the_repository, buf.buf, rename.new_name);
		}
	}

	إذا (refspecs_need_update) {
		پایله = ref_transaction_commit(rename.transaction, &err);
		إذا (پایله < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_پرمختګ(&rename.پرمختګ);

		handle_ټېل_default(rename.old_name, rename.new_name);
	}

بهر:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	أرجع پایله;
}

ساكن صحيح rm(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
	      بنية repository *repo ناکارول_شوی)
{
	بنية option options[] = {
		OPT_پای_ټکی()
	};
	بنية remote *remote;
	بنية strbuf buf = STRBUF_INIT;
	بنية معلوم_remotes معلوم_remotes = { عدم, عدم };
	بنية string_list branches = STRING_LIST_INIT_DUP;
	بنية string_list skipped = STRING_LIST_INIT_DUP;
	بنية branches_for_remote cb_معلومات;
	صحيح پایله;

	memset(&cb_معلومات, 0, حجم(cb_معلومات));
	cb_معلومات.branches = &branches;
	cb_معلومات.skipped = &skipped;
	cb_معلومات.keep = &معلوم_remotes;

	argc = parse_options(argc, argv, مختاړی, options,
			     builtin_remote_rm_usage, 0);
	إذا (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		وتل(2);
	}

	معلوم_remotes.منزل_delete = remote;
	for_each_remote(جمع_کړه_معلوم_remote, &معلوم_remotes);

	read_branches();
	لكل (اندازه_t i = 0; i < branch_list.nr; i++) {
		بنية string_list_توکی *توکی = branch_list.توکي + i;
		بنية branch_info *info = توکی->util;
		إذا (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ثابت حرف *کلیګانې[] = { "remote", "merge", عدم }, **k;
			لكل (k = کلیګانې; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						توکی->string, *k);
				پایله = repo_config_سېټ_gently(the_repository, buf.buf, عدم);
				إذا (پایله && پایله != CONFIG_NOTHING_سېټ)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		إذا (info->ټېل_remote_name && !strcmp(info->ټېل_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", توکی->string);
			پایله = repo_config_سېټ_gently(the_repository, buf.buf, عدم);
			إذا (پایله && پایله != CONFIG_NOTHING_سېټ)
				die(_("could not unset '%s'"), buf.buf);
		}
	}

	/*
	 * We cannot just pass a function to for_each_ref() which deletes
	 * the branches one by one, since for_each_ref() relies on cached
	 * refs, which are invalidated when deleting a branch.
	 */
	cb_معلومات.remote = remote;
	پایله = refs_for_each_ref(get_main_ref_store(the_repository),
				   جمع_کړه_branch_for_removal, &cb_معلومات);
	strbuf_release(&buf);

	إذا (!پایله)
		پایله = refs_delete_refs(get_main_ref_store(the_repository),
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
				skipped.توکي[i].string);
	}
	string_list_clear(&skipped, 0);

	إذا (!پایله) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		إذا (repo_config_rename_section(the_repository, buf.buf, عدم) < 1) {
			پایله = error(_("Could not remove config section '%s'"), buf.buf);
			اذهب بهر;
		}

		handle_ټېل_default(remote->name, عدم);
	}

بهر:
	لكل (بنية معلوم_remote *r = معلوم_remotes.list; r;) {
		بنية معلوم_remote *بل = r->بل;
		حرر(r);
		r = بل;
	}
	strbuf_release(&buf);
	أرجع پایله;
}

ساكن فراغ clear_ټېل_info(فراغ *util, ثابت حرف *string ناکارول_شوی)
{
	بنية ټېل_info *info = util;
	حرر(info->dest);
	حرر(info);
}

ساكن فراغ free_remote_ref_حالتونه(بنية ref_حالتونه *حالتونه)
{
	string_list_clear(&حالتونه->new_refs, 0);
	string_list_clear(&حالتونه->skipped, 0);
	string_list_clear(&حالتونه->stale, 1);
	string_list_clear(&حالتونه->tracked, 0);
	string_list_clear(&حالتونه->heads, 0);
	string_list_clear_func(&حالتونه->ټېل, clear_ټېل_info);
}

ساكن صحيح append_ref_منزل_tracked_list(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية ref_حالتونه *حالتونه = cb_معلومات;
	بنية refspec_توکی refspec;

	إذا (ref->flags & REF_ISSYMREF)
		أرجع 0;

	memset(&refspec, 0, حجم(refspec));
	refspec.dst = (حرف *)ref->name;
	إذا (!remote_پیدا_کړه_tracking(حالتونه->remote, &refspec)) {
		string_list_append(&حالتونه->tracked, abbrev_branch(refspec.src));
		حرر(refspec.src);
	}

	أرجع 0;
}

ساكن صحيح get_remote_ref_حالتونه(ثابت حرف *name,
				 بنية ref_حالتونه *حالتونه,
				 صحيح query)
{
	حالتونه->remote = remote_get(name);
	إذا (!حالتونه->remote)
		أرجع error(_("No such remote: '%s'"), name);

	read_branches();

	إذا (query) {
		بنية transport *transport;
		ثابت بنية ref *remote_refs;

		transport = transport_get(حالتونه->remote, حالتونه->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, عدم);

		حالتونه->queried = 1;
		إذا (query & GET_REF_حالتونه)
			get_ref_حالتونه(remote_refs, حالتونه);
		إذا (query & GET_سر_نومونه)
			get_سر_نومونه(remote_refs, حالتونه);
		إذا (query & GET_ټېل_REF_حالتونه)
			get_ټېل_ref_حالتونه(remote_refs, حالتونه);
		transport_disconnect(transport);
	} وإلا {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_منزل_tracked_list, حالتونه);
		string_list_ترتیبول(&حالتونه->tracked);
		get_ټېل_ref_حالتونه_noquery(حالتونه);
	}

	أرجع 0;
}

بنية show_info {
	بنية string_list list;
	بنية ref_حالتونه حالتونه;
	صحيح پلنوالی, width2;
	صحيح any_rebase;
};

#تعریف_کړه SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.حالتونه = REF_حالتونه_INIT, \
}

ساكن صحيح جمع_کړه_remote_منزل_show_info(بنية string_list_توکی *توکی, فراغ *cb_معلومات)
{
	بنية show_info *info = cb_معلومات;
	صحيح n = مزي_اوږدوالی(توکی->string);
	إذا (n > info->پلنوالی)
		info->پلنوالی = n;
	string_list_ورننباسه(&info->list, توکی->string);
	أرجع 0;
}

ساكن صحيح show_remote_info_توکی(بنية string_list_توکی *توکی, فراغ *cb_معلومات)
{
	بنية show_info *info = cb_معلومات;
	بنية ref_حالتونه *حالتونه = &info->حالتونه;
	ثابت حرف *name = توکی->string;

	إذا (حالتونه->queried) {
		ثابت حرف *fmt = "%s";
		ثابت حرف *arg = "";
		إذا (string_list_has_string(&حالتونه->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = حالتونه->remote->name;
		} وإلا إذا (string_list_has_string(&حالتونه->tracked, name))
			arg = _(" tracked");
		وإلا إذا (string_list_has_string(&حالتونه->skipped, name))
			arg = _(" skipped");
		وإلا إذا (string_list_has_string(&حالتونه->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		وإلا
			arg = _(" ???");
		چاپ_کړه("    %-*s", info->پلنوالی, name);
		چاپ_کړه(fmt, arg);
		چاپ_کړه("\n");
	} وإلا
		چاپ_کړه("    %s\n", name);

	أرجع 0;
}

ساكن صحيح جمع_کړه_local_منزل_show_info(بنية string_list_توکی *branch_توکی, فراغ *cb_معلومات)
{
	بنية show_info *show_info = cb_معلومات;
	بنية ref_حالتونه *حالتونه = &show_info->حالتونه;
	بنية branch_info *branch_info = branch_توکی->util;
	بنية string_list_توکی *توکی;
	صحيح n;

	إذا (!branch_info->یوځای.nr || !branch_info->remote_name ||
	    strcmp(حالتونه->remote->name, branch_info->remote_name))
		أرجع 0;
	إذا ((n = مزي_اوږدوالی(branch_توکی->string)) > show_info->پلنوالی)
		show_info->پلنوالی = n;
	إذا (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	توکی = string_list_ورننباسه(&show_info->list, branch_توکی->string);
	توکی->util = branch_info;

	أرجع 0;
}

ساكن صحيح show_local_info_توکی(بنية string_list_توکی *توکی, فراغ *cb_معلومات)
{
	بنية show_info *show_info = cb_معلومات;
	بنية branch_info *branch_info = توکی->util;
	بنية string_list *یوځای = &branch_info->یوځای;
	صحيح پلنوالی = show_info->پلنوالی + 4;

	إذا (branch_info->rebase >= REBASE_TRUE && branch_info->یوځای.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			توکی->string);
		أرجع 0;
	}

	چاپ_کړه("    %-*s ", show_info->پلنوالی, توکی->string);
	إذا (branch_info->rebase >= REBASE_TRUE) {
		ثابت حرف *پیغام;
		إذا (branch_info->rebase == REBASE_INTERACTIVE)
			پیغام = _("rebases interactively onto remote %s");
		وإلا إذا (branch_info->rebase == REBASE_MERGES)
			پیغام = _("rebases interactively (with merges) onto "
				"remote %s");
		وإلا
			پیغام = _("rebases onto remote %s");
		printf_ln(پیغام, یوځای->توکي[0].string);
		أرجع 0;
	} وإلا إذا (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), یوځای->توکي[0].string);
		پلنوالی++;
	} وإلا {
		printf_ln(_("merges with remote %s"), یوځای->توکي[0].string);
	}
	لكل (اندازه_t i = 1; i < یوځای->nr; i++)
		چاپ_کړه(_("%-*s    and with remote %s\n"), پلنوالی, "",
		       یوځای->توکي[i].string);

	أرجع 0;
}

ساكن صحيح جمع_کړه_ټېل_منزل_show_info(بنية string_list_توکی *ټېل_توکی, فراغ *cb_معلومات)
{
	بنية show_info *show_info = cb_معلومات;
	بنية ټېل_info *ټېل_info = ټېل_توکی->util;
	بنية string_list_توکی *توکی;
	صحيح n;
	إذا ((n = مزي_اوږدوالی(ټېل_توکی->string)) > show_info->پلنوالی)
		show_info->پلنوالی = n;
	إذا ((n = مزي_اوږدوالی(ټېل_info->dest)) > show_info->width2)
		show_info->width2 = n;
	توکی = string_list_append(&show_info->list, ټېل_توکی->string);
	توکی->util = ټېل_توکی->util;
	أرجع 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
ساكن صحيح cmp_string_with_ټېل(ثابت فراغ *va, ثابت فراغ *vb)
{
	ثابت بنية string_list_توکی *a = va;
	ثابت بنية string_list_توکی *b = vb;
	ثابت بنية ټېل_info *a_ټېل = a->util;
	ثابت بنية ټېل_info *b_ټېل = b->util;
	صحيح cmp = strcmp(a->string, b->string);
	أرجع cmp ? cmp : strcmp(a_ټېل->dest, b_ټېل->dest);
}

ساكن صحيح show_ټېل_info_توکی(بنية string_list_توکی *توکی, فراغ *cb_معلومات)
{
	بنية show_info *show_info = cb_معلومات;
	بنية ټېل_info *ټېل_info = توکی->util;
	ثابت حرف *src = توکی->string, *حالت = عدم;

	بدل (ټېل_info->حالت) {
	حالة ټېل_حالت_CREATE:
		حالت = _("create");
		اكسر;
	حالة ټېل_حالت_DELETE:
		حالت = _("delete");
		src = _("(none)");
		اكسر;
	حالة ټېل_حالت_UPTODATE:
		حالت = _("up to date");
		اكسر;
	حالة ټېل_حالت_FASTFORWARD:
		حالت = _("fast-forwardable");
		اكسر;
	حالة ټېل_حالت_OUTOFDATE:
		حالت = _("local out of date");
		اكسر;
	حالة ټېل_حالت_NOTQUERIED:
		اكسر;
	}
	إذا (حالت) {
		إذا (ټېل_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->پلنوالی, src,
			       show_info->width2, ټېل_info->dest, حالت);
		وإلا
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->پلنوالی, src,
			       show_info->width2, ټېل_info->dest, حالت);
	} وإلا {
		إذا (ټېل_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->پلنوالی, src,
			       ټېل_info->dest);
		وإلا
			printf_ln(_("    %-*s pushes to %s"), show_info->پلنوالی, src,
			       ټېل_info->dest);
	}
	أرجع 0;
}

ساكن صحيح get_one_ثبت(بنية remote *remote, فراغ *priv)
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
	url = ټېل_url_د_remote(remote);
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
	صحيح پایله;

	پایله = for_each_remote(get_one_ثبت, &list);

	إذا (!پایله) {
		string_list_ترتیبول(&list);
		لكل (اندازه_t i = 0; i < list.nr; i++) {
			بنية string_list_توکی *توکی = list.توکي + i;
			إذا (verbose)
				چاپ_کړه("%s\t%s\n", توکی->string,
					توکی->util ? (ثابت حرف *)توکی->util : "");
			وإلا {
				إذا (i && !strcmp((توکی - 1)->string, توکی->string))
					استمر;
				چاپ_کړه("%s\n", توکی->string);
			}
		}
	}
	string_list_clear(&list, 1);
	أرجع پایله;
}

ساكن صحيح show(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		بنية repository *repo ناکارول_شوی)
{
	صحيح no_query = 0, پایله = 0, query_flag = 0;
	بنية option options[] = {
		OPT_BOOL('n', عدم, &no_query, N_("do not query remotes")),
		OPT_پای_ټکی()
	};
	بنية show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, مختاړی, options,
			     builtin_remote_show_usage,
			     0);

	إذا (argc < 1)
		أرجع show_all();

	إذا (!no_query)
		query_flag = (GET_REF_حالتونه | GET_سر_نومونه | GET_ټېل_REF_حالتونه);

	لكل (; argc; argc--, argv++) {
		اندازه_t i;
		بنية strvec *url;

		get_remote_ref_حالتونه(*argv, &info.حالتونه, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.حالتونه.remote->url.v[0]);
		url = ټېل_url_د_remote(info.حالتونه.remote);
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
		وإلا إذا (!info.حالتونه.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		وإلا إذا (info.حالتونه.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.حالتونه.heads.توکي[0].string);
		وإلا {
			چاپ_کړه(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			لكل (i = 0; i < info.حالتونه.heads.nr; i++)
				چاپ_کړه("    %s\n", info.حالتونه.heads.توکي[i].string);
		}

		/* remote branch info */
		info.پلنوالی = 0;
		for_each_string_list(&info.حالتونه.new_refs, جمع_کړه_remote_منزل_show_info, &info);
		for_each_string_list(&info.حالتونه.skipped, جمع_کړه_remote_منزل_show_info, &info);
		for_each_string_list(&info.حالتونه.tracked, جمع_کړه_remote_منزل_show_info, &info);
		for_each_string_list(&info.حالتونه.stale, جمع_کړه_remote_منزل_show_info, &info);
		إذا (info.list.nr)
			printf_ln(Q_("  Remote branch:%s",
				     "  Remote branches:%s",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_remote_info_توکی, &info);
		string_list_clear(&info.list, 0);

		/* git pull info */
		info.پلنوالی = 0;
		info.any_rebase = 0;
		for_each_string_list(&branch_list, جمع_کړه_local_منزل_show_info, &info);
		إذا (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_توکی, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		إذا (info.حالتونه.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.پلنوالی = info.width2 = 0;
		for_each_string_list(&info.حالتونه.ټېل, جمع_کړه_ټېل_منزل_show_info, &info);
		QSORT(info.list.توکي, info.list.nr, cmp_string_with_ټېل);
		إذا (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_ټېل_info_توکی, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_حالتونه(&info.حالتونه);
	}

	أرجع پایله;
}

ساكن فراغ report_سېټ_سر_auto(ثابت حرف *remote, ثابت حرف *سر_name,
			بنية strbuf *b_local_سر, صحيح was_detached) {
	بنية strbuf buf_مختاړی = STRBUF_INIT;
	ثابت حرف *prev_سر = عدم;

	strbuf_addf(&buf_مختاړی, "refs/remotes/%s/", remote);
	skip_مختاړی(b_local_سر->buf, buf_مختاړی.buf, &prev_سر);

	إذا (prev_سر && !strcmp(prev_سر, سر_name))
		چاپ_کړه(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, سر_name);
	وإلا إذا (prev_سر)
		چاپ_کړه(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_سر, سر_name);
	وإلا إذا (!b_local_سر->len)
		چاپ_کړه(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, سر_name);
	وإلا إذا (was_detached && b_local_سر->len)
		چاپ_کړه(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_سر->buf, سر_name);
	وإلا
		چاپ_کړه(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_سر->buf, سر_name);
	strbuf_release(&buf_مختاړی);
}

ساكن صحيح سېټ_سر(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		    بنية repository *repo ناکارول_شوی)
{
	صحيح opt_a = 0, opt_d = 0, پایله = 0, was_detached;
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
		OPT_پای_ټکی()
	};
	argc = parse_options(argc, argv, مختاړی, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	إذا (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_سر, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	إذا (!opt_a && !opt_d && argc == 2) {
		سر_name = xstrdup(argv[1]);
	} وإلا إذا (opt_a && !opt_d && argc == 1) {
		بنية ref_حالتونه حالتونه = REF_حالتونه_INIT;
		get_remote_ref_حالتونه(argv[0], &حالتونه, GET_سر_نومونه);
		إذا (!حالتونه.heads.nr)
			پایله |= error(_("Cannot determine remote HEAD"));
		وإلا إذا (حالتونه.heads.nr > 1) {
			پایله |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			لكل (اندازه_t i = 0; i < حالتونه.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], حالتونه.heads.توکي[i].string);
		} وإلا
			سر_name = xstrdup(حالتونه.heads.توکي[0].string);
		free_remote_ref_حالتونه(&حالتونه);
	} وإلا إذا (opt_d && !opt_a && argc == 1) {
		إذا (refs_delete_ref(refs, عدم, b_سر.buf, عدم, REF_NO_DEREF))
			پایله |= error(_("Could not delete %s"), b_سر.buf);
	} وإلا
		usage_with_options(builtin_remote_sethead_usage, options);

	إذا (!سر_name)
		اذهب cleanup;
	strbuf_addf(&b_remote_سر, "refs/remotes/%s/%s", argv[0], سر_name);
	إذا (!refs_ref_exists(refs, b_remote_سر.buf)) {
		پایله |= error(_("Not a valid ref: %s"), b_remote_سر.buf);
		اذهب cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_سر.buf, b_remote_سر.buf,
			"remote set-head", &b_local_سر, 0);
	إذا (was_detached == -1) {
		پایله |= error(_("Could not set up %s"), b_سر.buf);
		اذهب cleanup;
	}
	إذا (opt_a)
		report_سېټ_سر_auto(argv[0], سر_name, &b_local_سر, was_detached);
	إذا (remote->follow_remote_سر == FOLLOW_REMOTE_ALWAYS) {
		بنية strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_سېټ(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	حرر(سر_name);
	strbuf_release(&b_سر);
	strbuf_release(&b_remote_سر);
	strbuf_release(&b_local_سر);
	أرجع پایله;
}

ساكن صحيح prune_remote(ثابت حرف *remote, صحيح dry_پرلپسې)
{
	صحيح پایله = 0;
	بنية ref_حالتونه حالتونه = REF_حالتونه_INIT;
	بنية string_list refs_منزل_prune = STRING_LIST_INIT_NODUP;
	بنية string_list_توکی *توکی;

	get_remote_ref_حالتونه(remote, &حالتونه, GET_REF_حالتونه);

	إذا (!حالتونه.stale.nr) {
		free_remote_ref_حالتونه(&حالتونه);
		أرجع 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), حالتونه.remote->url.v[0]);

	for_each_string_list_توکی(توکی, &حالتونه.stale)
		string_list_append(&refs_منزل_prune, توکی->util);
	string_list_ترتیبول(&refs_منزل_prune);

	إذا (!dry_پرلپسې)
		پایله |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_منزل_prune, 0);

	for_each_string_list_توکی(توکی, &حالتونه.stale) {
		ثابت حرف *refname = توکی->util;

		إذا (dry_پرلپسې)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		وإلا
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_پرلپسې, &refs_منزل_prune);

	string_list_clear(&refs_منزل_prune, 0);
	free_remote_ref_حالتونه(&حالتونه);
	أرجع پایله;
}

ساكن صحيح prune(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		 بنية repository *repo ناکارول_شوی)
{
	صحيح dry_پرلپسې = 0, پایله = 0;
	بنية option options[] = {
		OPT_DRY_پرلپسې(&dry_پرلپسې, N_("dry run")),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options,
			     builtin_remote_prune_usage, 0);

	إذا (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	لكل (; argc; argc--, argv++)
		پایله |= prune_remote(*argv, dry_پرلپسې);

	أرجع پایله;
}

ساكن صحيح get_remote_default(ثابت حرف *کلی, ثابت حرف *ارزښت ناکارول_شوی,
			      ثابت بنية config_context *ctx ناکارول_شوی,
			      فراغ *priv)
{
	إذا (strcmp(کلی, "remotes.default") == 0) {
		صحيح *وموندل_2 = priv;
		*وموندل_2 = 1;
	}
	أرجع 0;
}

ساكن صحيح update(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		  بنية repository *repo ناکارول_شوی)
{
	صحيح i, prune = -1;
	بنية option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_پای_ټکی()
	};
	بنية ماشوم_process cmd = ماشوم_PROCESS_INIT;
	صحيح default_defined = 0;

	argc = parse_options(argc, argv, مختاړی, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_ټېل(&cmd.دلیلونه, "fetch");

	إذا (prune != -1)
		strvec_ټېل(&cmd.دلیلونه, prune ? "--prune" : "--no-prune");
	إذا (verbose)
		strvec_ټېل(&cmd.دلیلونه, "-v");
	strvec_ټېل(&cmd.دلیلونه, "--multiple");
	إذا (argc < 2)
		strvec_ټېل(&cmd.دلیلونه, "default");
	لكل (i = 1; i < argc; i++)
		strvec_ټېل(&cmd.دلیلونه, argv[i]);

	إذا (strcmp(cmd.دلیلونه.v[cmd.دلیلونه.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		إذا (!default_defined) {
			strvec_راوباسه(&cmd.دلیلونه);
			strvec_ټېل(&cmd.دلیلونه, "--all");
		}
	}

	cmd.git_cmd = 1;
	أرجع پرلپسې_command(&cmd);
}

ساكن صحيح remove_all_fetch_refspecs(ثابت حرف *کلی)
{
	أرجع repo_config_سېټ_multivar_gently(the_repository, کلی, عدم, عدم,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

ساكن فراغ جمع_کړه_branches(بنية remote *remote, ثابت حرف **branches,
			 ثابت حرف *کلی)
{
	ثابت حرف *remotename = remote->name;
	صحيح mirror = remote->mirror;
	بنية strbuf refspec = STRBUF_INIT;

	لكل (; *branches; branches++)
		جمع_کړه_branch(کلی, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

ساكن صحيح سېټ_remote_branches(ثابت حرف *remotename, ثابت حرف **branches,
				صحيح جمع_کړه_ډېرپېښ)
{
	بنية strbuf کلی = STRBUF_INIT;
	بنية remote *remote;

	strbuf_addf(&کلی, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		وتل(2);
	}

	إذا (!جمع_کړه_ډېرپېښ && remove_all_fetch_refspecs(کلی.buf)) {
		strbuf_release(&کلی);
		أرجع 1;
	}
	جمع_کړه_branches(remote, branches, کلی.buf);

	strbuf_release(&کلی);
	أرجع 0;
}

ساكن صحيح سېټ_branches(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			بنية repository *repo ناکارول_شوی)
{
	صحيح جمع_کړه_ډېرپېښ = 0;
	بنية option options[] = {
		OPT_BOOL('\0', "add", &جمع_کړه_ډېرپېښ, N_("add branch")),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options,
			     builtin_remote_setbranches_usage, 0);
	إذا (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = عدم;

	أرجع سېټ_remote_branches(argv[0], argv + 1, جمع_کړه_ډېرپېښ);
}

ساكن صحيح get_url(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		   بنية repository *repo ناکارول_شوی)
{
	صحيح ټېل_ډېرپېښ = 0, all_ډېرپېښ = 0;
	ثابت حرف *remotename = عدم;
	بنية remote *remote;
	بنية strvec *url;
	بنية option options[] = {
		OPT_BOOL('\0', "push", &ټېل_ډېرپېښ,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_ډېرپېښ,
			 N_("return all URLs")),
		OPT_پای_ټکی()
	};
	argc = parse_options(argc, argv, مختاړی, options,
			     builtin_remote_geturl_usage, 0);

	إذا (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		وتل(2);
	}

	url = ټېل_ډېرپېښ ? ټېل_url_د_remote(remote) : &remote->url;

	إذا (all_ډېرپېښ) {
		لكل (اندازه_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} وإلا {
		printf_ln("%s", url->v[0]);
	}

	أرجع 0;
}

ساكن صحيح سېټ_url(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		   بنية repository *repo ناکارول_شوی)
{
	صحيح ټېل_ډېرپېښ = 0, جمع_کړه_ډېرپېښ = 0, delete_ډېرپېښ = 0;
	صحيح matches = 0, negative_matches = 0;
	ثابت حرف *remotename = عدم;
	ثابت حرف *newurl = عدم;
	ثابت حرف *oldurl = عدم;
	بنية remote *remote;
	regex_t old_regex;
	بنية strvec *urlset;
	بنية strbuf name_buf = STRBUF_INIT;
	بنية option options[] = {
		OPT_BOOL('\0', "push", &ټېل_ډېرپېښ,
			 N_("manipulate push URLs")),
		OPT_BOOL('\0', "add", &جمع_کړه_ډېرپېښ,
			 N_("add URL")),
		OPT_BOOL('\0', "delete", &delete_ډېرپېښ,
			    N_("delete URLs")),
		OPT_پای_ټکی()
	};
	argc = parse_options(argc, argv, مختاړی, options,
			     builtin_remote_seturl_usage,
			     PARSE_OPT_KEEP_ARGV0);

	إذا (جمع_کړه_ډېرپېښ && delete_ډېرپېښ)
		die(_("--add --delete doesn't make sense"));

	إذا (argc < 3 || argc > 4 || ((جمع_کړه_ډېرپېښ || delete_ډېرپېښ) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	إذا (argc > 3)
		oldurl = argv[3];

	إذا (delete_ډېرپېښ)
		oldurl = newurl;

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		وتل(2);
	}

	إذا (ټېل_ډېرپېښ) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} وإلا {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	إذا ((!oldurl && !delete_ډېرپېښ) || جمع_کړه_ډېرپېښ) {
		إذا (جمع_کړه_ډېرپېښ)
			repo_config_سېټ_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		وإلا
			repo_config_سېټ(the_repository, name_buf.buf, newurl);
		اذهب بهر;
	}

	/* Old URL specified. Demand that one matches. */
	إذا (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	لكل (اندازه_t i = 0; i < urlset->nr; i++)
		إذا (!regexec(&old_regex, urlset->v[i], 0, عدم, 0))
			matches++;
		وإلا
			negative_matches++;
	إذا (!delete_ډېرپېښ && !matches)
		die(_("No such URL found: %s"), oldurl);
	إذا (delete_ډېرپېښ && !negative_matches && !ټېل_ډېرپېښ)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	إذا (!delete_ډېرپېښ)
		repo_config_سېټ_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	وإلا
		repo_config_سېټ_multivar(the_repository, name_buf.buf, عدم, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
بهر:
	strbuf_release(&name_buf);
	أرجع 0;
}

صحيح cmd_remote(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *مختاړی,
	       بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, جمع_کړه),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, عدم, PARSE_OPT_NOCOMPLETE),
		OPT_SUBCOMMAND("remove", &fn, rm),
		OPT_SUBCOMMAND("set-head", &fn, سېټ_سر),
		OPT_SUBCOMMAND("set-branches", &fn, سېټ_branches),
		OPT_SUBCOMMAND("get-url", &fn, get_url),
		OPT_SUBCOMMAND("set-url", &fn, سېټ_url),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("update", &fn, update),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, builtin_remote_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	إذا (fn) {
		أرجع !!fn(argc, argv, مختاړی, repo);
	} وإلا {
		إذا (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		أرجع !!show_all();
	}
}
