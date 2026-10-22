#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE

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

ساكن ثابت حرف * ثابت builtin_remote_جوڙيو_usage[] = {
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

#وصف_ڪريو GET_REF_حالتون (1<<0)
#وصف_ڪريو GET_سر_نالا (1<<1)
#وصف_ڪريو GET_ڌڪو_REF_حالتون (1<<2)

ساكن صحيح verbose;

ساكن صحيح fetch_remote(ثابت حرف *name)
{
	بنية ٻار_process cmd = ٻار_PROCESS_INIT;

	strvec_ڌڪو(&cmd.دليلون, "fetch");
	إذا (verbose)
		strvec_ڌڪو(&cmd.دليلون, "-v");
	strvec_ڌڪو(&cmd.دليلون, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	إذا (لاڳيتو_command(&cmd))
		أرجع error(_("Could not fetch %s"), name);
	أرجع 0;
}

تعداد {
	ٽيگ_فهرست_UNSET = 0,
	ٽيگ_فهرست_DEFAULT = 1,
	ٽيگ_فهرست_سيٽ = 2
};

#وصف_ڪريو MIRROR_NONE 0
#وصف_ڪريو MIRROR_FETCH 1
#وصف_ڪريو MIRROR_ڌڪو 2
#وصف_ڪريو MIRROR_BOTH (MIRROR_FETCH|MIRROR_ڌڪو)

ساكن فراغ جوڙيو_branch(ثابت حرف *ڪنجي, ثابت حرف *branchname,
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
	repo_config_سيٽ_multivar(the_repository, ڪنجي, tmp->buf, "^$", 0);
}

ساكن ثابت حرف mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

ساكن صحيح parse_mirror_opt(ثابت بنية option *opt, ثابت حرف *arg, صحيح not)
{
	بنانشان *mirror = opt->قدر;
	إذا (not)
		*mirror = MIRROR_NONE;
	وإلا إذا (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	وإلا إذا (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	وإلا إذا (!strcmp(arg, "push"))
		*mirror = MIRROR_ڌڪو;
	وإلا
		أرجع error(_("unknown --mirror argument: %s"), arg);
	أرجع 0;
}

ساكن صحيح check_remote_collision(بنية remote *remote, فراغ *معلومات)
{
	ثابت حرف *name = معلومات;
	ثابت حرف *p;

	إذا (skip_اڳياڙي(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	إذا (skip_اڳياڙي(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	أرجع 0;
}

ساكن صحيح جوڙيو(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
	       بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح fetch = 0, fetch_ٽيگ_فهرست = ٽيگ_فهرست_DEFAULT;
	بنانشان mirror = MIRROR_NONE;
	بنية string_list track = STRING_LIST_INIT_NODUP;
	ثابت حرف *master = عدم;
	بنية remote *remote;
	بنية strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ثابت حرف *name, *url;
	صحيح نتيجو = 0;

	بنية option options[] = {
		OPT_BOOL('f', "fetch", &fetch, N_("fetch the remote branches")),
		OPT_سيٽ_INT(0, "tags", &fetch_ٽيگ_فهرست,
			    N_("import all tags and associated objects when fetching\n"
			       "or do not fetch any tag at all (--no-tags)"),
			    ٽيگ_فهرست_سيٽ),
		OPT_STRING_LIST('t', "track", &track, N_("branch"),
				N_("branch(es) to track")),
		OPT_STRING('m', "master", &master, N_("branch"), N_("master branch")),
		OPT_CALLBACK_F(0, "mirror", &mirror, "(push|fetch)",
			N_("set up remote as a mirror to push to or fetch from"),
			PARSE_OPT_OPTARG | PARSE_OPT_COMP_ARG, parse_mirror_opt),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_remote_جوڙيو_usage, 0);

	إذا (argc != 2)
		usage_with_options(builtin_remote_جوڙيو_usage, options);

	إذا (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	إذا (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	إذا (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		ٻاهر_نڪرو(3);
	}

	إذا (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (فراغ *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_سيٽ(the_repository, buf.buf, url);

	إذا (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		إذا (track.nr == 0)
			string_list_append(&track, "*");
		لكل (سائيز_t i = 0; i < track.nr; i++) {
			جوڙيو_branch(buf.buf, track.شيون[i].string,
				   name, mirror, &buf2);
		}
	}

	إذا (mirror & MIRROR_ڌڪو) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_سيٽ(the_repository, buf.buf, "true");
	}

	إذا (fetch_ٽيگ_فهرست != ٽيگ_فهرست_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_سيٽ(the_repository, buf.buf,
				fetch_ٽيگ_فهرست == ٽيگ_فهرست_سيٽ ? "--tags" : "--no-tags");
	}

	إذا (fetch && fetch_remote(name)) {
		نتيجو = 1;
		اذهب ٻاهريون;
	}

	إذا (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		إذا (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			نتيجو = error(_("Could not setup master '%s'"), master);
	}

ٻاهريون:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	أرجع نتيجو;
}

بنية branch_info {
	حرف *remote_name;
	بنية string_list ملايو;
	تعداد rebase_type rebase;
	حرف *ڌڪو_remote_name;
};

ساكن بنية string_list branch_list = STRING_LIST_INIT_DUP;

ساكن ثابت حرف *abbrev_ref(ثابت حرف *name, ثابت حرف *اڳياڙي)
{
	skip_اڳياڙي(name, اڳياڙي, &name);
	أرجع name;
}
#وصف_ڪريو abbrev_branch(name) abbrev_ref((name), "refs/heads/")

ساكن صحيح config_read_branches(ثابت حرف *ڪنجي, ثابت حرف *قدر,
				ثابت بنية config_context *ctx اڻ_استعمال_ٿيل,
				فراغ *معلومات اڻ_استعمال_ٿيل)
{
	ثابت حرف *orig_ڪنجي = ڪنجي;
	حرف *name;
	بنية string_list_شي *شي;
	بنية branch_info *info;
	تعداد { REMOTE, ملايو_2, REBASE, ڌڪو_REMOTE } type;
	سائيز_t ڪنجي_len;

	إذا (!starts_with(ڪنجي, "branch."))
		أرجع 0;

	ڪنجي += لڙي_ڊيگهه("branch.");
	إذا (strip_suffix(ڪنجي, ".remote", &ڪنجي_len))
		type = REMOTE;
	وإلا إذا (strip_suffix(ڪنجي, ".merge", &ڪنجي_len))
		type = ملايو_2;
	وإلا إذا (strip_suffix(ڪنجي, ".rebase", &ڪنجي_len))
		type = REBASE;
	وإلا إذا (strip_suffix(ڪنجي, ".pushremote", &ڪنجي_len))
		type = ڌڪو_REMOTE;
	وإلا
		أرجع 0;

	name = xmemdupz(ڪنجي, ڪنجي_len);
	شي = string_list_وجهو(&branch_list, name);

	إذا (!شي->util)
		شي->util = xcalloc(1, حجم(بنية branch_info));
	info = شي->util;
	بدل (type) {
	حالة REMOTE:
		إذا (info->remote_name)
			warning(_("more than one %s"), orig_ڪنجي);
		info->remote_name = xstrdup(قدر);
		اكسر;
	حالة ملايو_2: {
		ثابت حرف *space = strchr(قدر, ' ');
		قدر = abbrev_branch(قدر);
		طالما (space) {
			حرف *ملايو;
			ملايو = xstrndup(قدر, space - قدر);
			string_list_append(&info->ملايو, ملايو);
			قدر = abbrev_branch(space + 1);
			space = strchr(قدر, ' ');
		}
		string_list_append(&info->ملايو, xstrdup(قدر));
		اكسر;
	}
	حالة REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_قدر(قدر);
		إذا (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, قدر);
		اكسر;
	حالة ڌڪو_REMOTE:
		إذا (info->ڌڪو_remote_name)
			warning(_("more than one %s"), orig_ڪنجي);
		info->ڌڪو_remote_name = xstrdup(قدر);
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

بنية ref_حالتون {
	بنية remote *remote;
	بنية string_list new_refs, skipped, stale, tracked, heads, ڌڪو;
	صحيح queried;
};

#وصف_ڪريو REF_حالتون_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.ڌڪو = STRING_LIST_INIT_DUP, \
}

ساكن صحيح get_ref_حالتون(ثابت بنية ref *remote_refs, بنية ref_حالتون *حالتون)
{
	بنية ref *fetch_map = عدم, **پڇ = &fetch_map;
	بنية ref *ref, *stale_refs;
	صحيح i;

	لكل (i = 0; i < حالتون->remote->fetch.nr; i++)
		إذا (get_fetch_map(remote_refs, &حالتون->remote->fetch.شيون[i], &پڇ, 1))
			die(_("Could not get fetch map for refspec %s"),
				حالتون->remote->fetch.شيون[i].raw);

	لكل (ref = fetch_map; ref; ref = ref->ايندڙ) {
		إذا (refname_matches_negative_refspec_شي(ref->name, &حالتون->remote->fetch))
			string_list_append(&حالتون->skipped, abbrev_branch(ref->name));
		وإلا إذا (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&حالتون->new_refs, abbrev_branch(ref->name));
		وإلا
			string_list_append(&حالتون->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&حالتون->remote->fetch, fetch_map);
	لكل (ref = stale_refs; ref; ref = ref->ايندڙ) {
		بنية string_list_شي *شي =
			string_list_append(&حالتون->stale, abbrev_branch(ref->name));
		شي->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_ترتيب_ڏيو(&حالتون->new_refs);
	string_list_ترتيب_ڏيو(&حالتون->skipped);
	string_list_ترتيب_ڏيو(&حالتون->tracked);
	string_list_ترتيب_ڏيو(&حالتون->stale);

	أرجع 0;
}

بنية ڌڪو_info {
	حرف *dest;
	صحيح forced;
	تعداد {
		ڌڪو_حالت_CREATE = 0,
		ڌڪو_حالت_DELETE,
		ڌڪو_حالت_UPTODATE,
		ڌڪو_حالت_FASTFORWARD,
		ڌڪو_حالت_OUTOFDATE,
		ڌڪو_حالت_NOTQUERIED
	} حالت;
};

ساكن صحيح get_ڌڪو_ref_حالتون(ثابت بنية ref *remote_refs,
	بنية ref_حالتون *حالتون)
{
	بنية remote *remote = حالتون->remote;
	بنية ref *ref, *local_refs, *ڌڪو_map;
	إذا (remote->mirror)
		أرجع 0;

	local_refs = get_local_heads();
	ڌڪو_map = copy_ref_list(remote_refs);

	ميلاپ_ڌڪو_refs(local_refs, &ڌڪو_map, &remote->ڌڪو, ميلاپ_REFS_NONE);

	لكل (ref = ڌڪو_map; ref; ref = ref->ايندڙ) {
		بنية string_list_شي *شي;
		بنية ڌڪو_info *info;

		إذا (!ref->peer_ref)
			استمر;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		شي = string_list_append(&حالتون->ڌڪو,
					  abbrev_branch(ref->peer_ref->name));
		شي->util = xcalloc(1, حجم(بنية ڌڪو_info));
		info = شي->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		إذا (is_null_oid(&ref->new_oid)) {
			info->حالت = ڌڪو_حالت_DELETE;
		} وإلا إذا (oideq(&ref->old_oid, &ref->new_oid))
			info->حالت = ڌڪو_حالت_UPTODATE;
		وإلا إذا (is_null_oid(&ref->old_oid))
			info->حالت = ڌڪو_حالت_CREATE;
		وإلا إذا (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->حالت = ڌڪو_حالت_FASTFORWARD;
		وإلا
			info->حالت = ڌڪو_حالت_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(ڌڪو_map);
	أرجع 0;
}

ساكن صحيح get_ڌڪو_ref_حالتون_noquery(بنية ref_حالتون *حالتون)
{
	صحيح i;
	بنية remote *remote = حالتون->remote;
	بنية string_list_شي *شي;
	بنية ڌڪو_info *info;

	إذا (remote->mirror)
		أرجع 0;

	إذا (!remote->ڌڪو.nr) {
		شي = string_list_append(&حالتون->ڌڪو, _("(matching)"));
		info = شي->util = xcalloc(1, حجم(بنية ڌڪو_info));
		info->حالت = ڌڪو_حالت_NOTQUERIED;
		info->dest = xstrdup(شي->string);
	}
	لكل (i = 0; i < remote->ڌڪو.nr; i++) {
		ثابت بنية refspec_شي *spec = &remote->ڌڪو.شيون[i];
		إذا (spec->matching)
			شي = string_list_append(&حالتون->ڌڪو, _("(matching)"));
		وإلا إذا (لڙي_ڊيگهه(spec->src))
			شي = string_list_append(&حالتون->ڌڪو, spec->src);
		وإلا
			شي = string_list_append(&حالتون->ڌڪو, _("(delete)"));

		info = شي->util = xcalloc(1, حجم(بنية ڌڪو_info));
		info->forced = spec->force;
		info->حالت = ڌڪو_حالت_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : شي->string);
	}
	أرجع 0;
}

ساكن صحيح get_سر_نالا(ثابت بنية ref *remote_refs, بنية ref_حالتون *حالتون)
{
	بنية ref *ref, *matches;
	بنية ref *fetch_map = عدم, **fetch_map_پڇ = &fetch_map;
	بنية refspec_شي refspec = {
		.force = 0,
		.نمونو = 1,
		.src = (حرف *) "refs/heads/*",
		.dst = (حرف *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_پڇ, 0);
	matches = guess_remote_سر(ڳوليو_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_سر_ALL);
	لكل (ref = matches; ref; ref = ref->ايندڙ)
		string_list_append(&حالتون->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	أرجع 0;
}

بنية معلوم_remote {
	بنية معلوم_remote *ايندڙ;
	بنية remote *remote;
};

بنية معلوم_remotes {
	بنية remote *منزل_delete;
	بنية معلوم_remote *list;
};

ساكن صحيح جوڙيو_معلوم_remote(بنية remote *remote, فراغ *cb_معلومات)
{
	بنية معلوم_remotes *all = cb_معلومات;
	بنية معلوم_remote *r;

	إذا (!strcmp(all->منزل_delete->name, remote->name))
		أرجع 0;

	r = xmalloc(حجم(*r));
	r->remote = remote;
	r->ايندڙ = all->list;
	all->list = r;
	أرجع 0;
}

بنية branches_for_remote {
	بنية remote *remote;
	بنية string_list *branches, *skipped;
	بنية معلوم_remotes *keep;
};

ساكن صحيح جوڙيو_branch_for_removal(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية branches_for_remote *branches = cb_معلومات;
	بنية refspec_شي refspec;
	بنية معلوم_remote *kr;

	memset(&refspec, 0, حجم(refspec));
	refspec.dst = (حرف *)ref->name;
	إذا (remote_ڳوليو_tracking(branches->remote, &refspec))
		أرجع 0;
	حرر(refspec.src);

	/* don't delete a branch if another remote also uses it */
	لكل (kr = branches->keep->list; kr; kr = kr->ايندڙ) {
		memset(&refspec, 0, حجم(refspec));
		refspec.dst = (حرف *)ref->name;
		إذا (!remote_ڳوليو_tracking(kr->remote, &refspec)) {
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
	بنية ترقي *ترقي;
	بنية strbuf *err;
	uint32_t ترقي_nr;
	uint64_t اشاريو;
};

ساكن فراغ compute_renamed_ref(بنية rename_info *rename,
				ثابت حرف *refname,
				بنية strbuf *ٻاهريون)
{
	strbuf_reset(ٻاهريون);
	strbuf_addstr(ٻاهريون, refname);
	strbuf_splice(ٻاهريون, لڙي_ڊيگهه("refs/remotes/"), لڙي_ڊيگهه(rename->old_name),
		      rename->new_name, لڙي_ڊيگهه(rename->new_name));
}

ساكن صحيح rename_one_reflog_داخلا(ثابت حرف *old_refname,
				   بنية object_id *old_oid,
				   بنية object_id *new_oid,
				   ثابت حرف *committer,
				   timestamp_t timestamp, صحيح tz,
				   ثابت حرف *نياپو, فراغ *cb_معلومات)
{
	بنية rename_info *rename = cb_معلومات;
	بنية strbuf new_refname = STRBUF_INIT;
	بنية strbuf سڃاڻپ_عنصر = STRBUF_INIT;
	بنية strbuf name = STRBUF_INIT;
	بنية strbuf mail = STRBUF_INIT;
	بنية ident_ورهايو ident;
	ثابت حرف *date;
	صحيح error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	إذا (ورهايو_ident_سٽ(&ident, committer, لڙي_ڊيگهه(committer)) < 0) {
		error = -1;
		اذهب ٻاهريون;
	}

	strbuf_جوڙيو(&name, ident.name_begin, ident.name_پڇاڙي - ident.name_begin);
	strbuf_جوڙيو(&mail, ident.mail_begin, ident.mail_پڇاڙي - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_گهڻائي(عام_2));
	strbuf_addstr(&سڃاڻپ_عنصر, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, سڃاڻپ_عنصر.buf, نياپو,
					      rename->اشاريو++, rename->err);

ٻاهريون:
	strbuf_release(&new_refname);
	strbuf_release(&سڃاڻپ_عنصر);
	strbuf_release(&name);
	strbuf_release(&mail);
	أرجع error;
}

ساكن صحيح rename_one_reflog(ثابت حرف *old_refname,
			     ثابت بنية object_id *old_oid,
			     بنية rename_info *rename)
{
	بنية strbuf new_refname = STRBUF_INIT;
	بنية strbuf پيغام = STRBUF_INIT;
	صحيح error;

	إذا (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		أرجع 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_داخلا, rename);
	إذا (error < 0)
		اذهب ٻاهريون;

	compute_renamed_ref(rename, old_refname, &new_refname);

	/*
	 * Manually write the reflog entry for the now-renamed ref. We cannot
	 * rely on `rename_one_ref()` to do this for us as that would screw
	 * over order in which reflog entries are being written.
	 *
	 * Furthermore, we only append the entry in case the reference
	 * resolves. Missing references shouldn't have reflogs anyway.
	 */
	strbuf_addf(&پيغام, "remote: renamed %s to %s", old_refname,
		    new_refname.buf);

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      old_oid, old_oid, git_committer_info(0),
					      پيغام.buf, rename->اشاريو++, rename->err);
	إذا (error < 0)
		أرجع error;

ٻاهريون:
	strbuf_release(&new_refname);
	strbuf_release(&پيغام);
	أرجع error;
}

ساكن صحيح rename_one_ref(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية strbuf new_referent = STRBUF_INIT;
	بنية strbuf new_refname = STRBUF_INIT;
	بنية rename_info *rename = cb_معلومات;
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
		اذهب ٻاهريون;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : عدم, عدم,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       عدم, rename->err);
	إذا (error < 0)
		اذهب ٻاهريون;

	error = rename_one_reflog(ref->name, oid, rename);
	إذا (error < 0)
		اذهب ٻاهريون;

	display_ترقي(rename->ترقي, ++rename->ترقي_nr);

ٻاهريون:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	أرجع error;
}

ساكن صحيح migrate_file(بنية remote *remote)
{
	بنية strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	لكل (سائيز_t i = 0; i < remote->url.nr; i++)
		repo_config_سيٽ_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	لكل (صحيح i = 0; i < remote->ڌڪو.nr; i++)
		repo_config_سيٽ_multivar(the_repository, buf.buf, remote->ڌڪو.شيون[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	لكل (صحيح i = 0; i < remote->fetch.nr; i++)
		repo_config_سيٽ_multivar(the_repository, buf.buf, remote->fetch.شيون[i].raw, "^$", 0);
#جيڪڏهن_وصف_نه_ٿيل WITH_BREAKING_CHANGES
	إذا (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_رستو_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	وإلا إذا (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_رستو_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#جيڪڏهن_پورو /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	أرجع 0;
}

بنية ڌڪو_default_info
{
	ثابت حرف *old_name;
	تعداد config_scope scope;
	بنية strbuf origin;
	صحيح linenr;
};

ساكن صحيح config_read_ڌڪو_default(ثابت حرف *ڪنجي, ثابت حرف *قدر,
	ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت بنية ڪنجي_قدر_info *kvi = ctx->kvi;

	بنية ڌڪو_default_info* info = cb;
	إذا (strcmp(ڪنجي, "remote.pushdefault") ||
	    !قدر || strcmp(قدر, info->old_name))
		أرجع 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	أرجع 0;
}

ساكن فراغ handle_ڌڪو_default(ثابت حرف* old_name, ثابت حرف* new_name)
{
	بنية ڌڪو_default_info ڌڪو_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_ڌڪو_default, &ڌڪو_default);
	إذا (ڌڪو_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	وإلا إذا (ڌڪو_default.scope >= CONFIG_SCOPE_LOCAL) {
		صحيح نتيجو = repo_config_سيٽ_gently(the_repository, "remote.pushDefault",
						    new_name);
		إذا (new_name && نتيجو && نتيجو != CONFIG_NOTHING_سيٽ)
			die(_("could not set '%s'"), "remote.pushDefault");
		وإلا إذا (!new_name && نتيجو && نتيجو != CONFIG_NOTHING_سيٽ)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} وإلا إذا (ڌڪو_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(ڌڪو_default.scope),
			ڌڪو_default.origin.buf, ڌڪو_default.linenr,
			old_name);
	}

	strbuf_release(&ڌڪو_default.origin);
}

ساكن ثابت حرف conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

ساكن صحيح mv(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
	      بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح show_ترقي = isatty(2);
	بنية option options[] = {
		OPT_BOOL(0, "progress", &show_ترقي, N_("force progress reporting")),
		OPT_پڇاڙي()
	};
	بنية remote *oldremote, *newremote;
	بنية strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	بنية rename_info rename = {
		.err = &err,
	};
	صحيح refspecs_need_update = 0;
	صحيح نتيجو = 0;

	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_remote_rename_usage, 0);

	إذا (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	إذا (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		ٻاهر_نڪرو(2);
	}

	إذا (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		أرجع migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	إذا (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		ٻاهر_نڪرو(3);
	}

	إذا (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	إذا (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		نتيجو = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		اذهب ٻاهريون;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	لكل (صحيح i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!لڙي_ڳولا(oldremote->fetch.شيون[i].raw,
						old_remote_context.buf);

	إذا (refspecs_need_update) {
		بنية refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		إذا (!rename.transaction)
			اذهب ٻاهريون;

		إذا (show_ترقي)
			rename.ترقي = شروعات_delayed_ترقي(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.اڳياڙي = buf.buf;

		نتيجو = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		إذا (نتيجو < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		نتيجو = ref_transaction_prepare(rename.transaction, &err);
		إذا (نتيجو < 0) {
			error("renaming remote references failed: %s", err.buf);
			إذا (نتيجو == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(عدم);
		}
	}

	إذا (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_سيٽ_multivar(the_repository, buf.buf, عدم, عدم, CONFIG_FLAGS_MULTI_REPLACE);
		لكل (صحيح i = 0; i < oldremote->fetch.nr; i++) {
			حرف *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.شيون[i].raw);
			ptr = لڙي_ڳولا(buf2.buf, old_remote_context.buf);
			إذا (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + لڙي_ڊيگهه(":refs/remotes/"),
					      لڙي_ڊيگهه(rename.old_name), rename.new_name,
					      لڙي_ڊيگهه(rename.new_name));
			} وإلا
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_سيٽ_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	لكل (سائيز_t i = 0; i < branch_list.nr; i++) {
		بنية string_list_شي *شي = branch_list.شيون + i;
		بنية branch_info *info = شي->util;
		إذا (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", شي->string);
			repo_config_سيٽ(the_repository, buf.buf, rename.new_name);
		}
		إذا (info->ڌڪو_remote_name && !strcmp(info->ڌڪو_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", شي->string);
			repo_config_سيٽ(the_repository, buf.buf, rename.new_name);
		}
	}

	إذا (refspecs_need_update) {
		نتيجو = ref_transaction_commit(rename.transaction, &err);
		إذا (نتيجو < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_ترقي(&rename.ترقي);

		handle_ڌڪو_default(rename.old_name, rename.new_name);
	}

ٻاهريون:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	أرجع نتيجو;
}

ساكن صحيح rm(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
	      بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية option options[] = {
		OPT_پڇاڙي()
	};
	بنية remote *remote;
	بنية strbuf buf = STRBUF_INIT;
	بنية معلوم_remotes معلوم_remotes = { عدم, عدم };
	بنية string_list branches = STRING_LIST_INIT_DUP;
	بنية string_list skipped = STRING_LIST_INIT_DUP;
	بنية branches_for_remote cb_معلومات;
	صحيح نتيجو;

	memset(&cb_معلومات, 0, حجم(cb_معلومات));
	cb_معلومات.branches = &branches;
	cb_معلومات.skipped = &skipped;
	cb_معلومات.keep = &معلوم_remotes;

	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_remote_rm_usage, 0);
	إذا (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		ٻاهر_نڪرو(2);
	}

	معلوم_remotes.منزل_delete = remote;
	for_each_remote(جوڙيو_معلوم_remote, &معلوم_remotes);

	read_branches();
	لكل (سائيز_t i = 0; i < branch_list.nr; i++) {
		بنية string_list_شي *شي = branch_list.شيون + i;
		بنية branch_info *info = شي->util;
		إذا (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ثابت حرف *ڪنجيون[] = { "remote", "merge", عدم }, **k;
			لكل (k = ڪنجيون; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						شي->string, *k);
				نتيجو = repo_config_سيٽ_gently(the_repository, buf.buf, عدم);
				إذا (نتيجو && نتيجو != CONFIG_NOTHING_سيٽ)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		إذا (info->ڌڪو_remote_name && !strcmp(info->ڌڪو_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", شي->string);
			نتيجو = repo_config_سيٽ_gently(the_repository, buf.buf, عدم);
			إذا (نتيجو && نتيجو != CONFIG_NOTHING_سيٽ)
				die(_("could not unset '%s'"), buf.buf);
		}
	}

	/*
	 * We cannot just pass a function to for_each_ref() which deletes
	 * the branches one by one, since for_each_ref() relies on cached
	 * refs, which are invalidated when deleting a branch.
	 */
	cb_معلومات.remote = remote;
	نتيجو = refs_for_each_ref(get_main_ref_store(the_repository),
				   جوڙيو_branch_for_removal, &cb_معلومات);
	strbuf_release(&buf);

	إذا (!نتيجو)
		نتيجو = refs_delete_refs(get_main_ref_store(the_repository),
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
		لكل (سائيز_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.شيون[i].string);
	}
	string_list_clear(&skipped, 0);

	إذا (!نتيجو) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		إذا (repo_config_rename_section(the_repository, buf.buf, عدم) < 1) {
			نتيجو = error(_("Could not remove config section '%s'"), buf.buf);
			اذهب ٻاهريون;
		}

		handle_ڌڪو_default(remote->name, عدم);
	}

ٻاهريون:
	لكل (بنية معلوم_remote *r = معلوم_remotes.list; r;) {
		بنية معلوم_remote *ايندڙ = r->ايندڙ;
		حرر(r);
		r = ايندڙ;
	}
	strbuf_release(&buf);
	أرجع نتيجو;
}

ساكن فراغ clear_ڌڪو_info(فراغ *util, ثابت حرف *string اڻ_استعمال_ٿيل)
{
	بنية ڌڪو_info *info = util;
	حرر(info->dest);
	حرر(info);
}

ساكن فراغ free_remote_ref_حالتون(بنية ref_حالتون *حالتون)
{
	string_list_clear(&حالتون->new_refs, 0);
	string_list_clear(&حالتون->skipped, 0);
	string_list_clear(&حالتون->stale, 1);
	string_list_clear(&حالتون->tracked, 0);
	string_list_clear(&حالتون->heads, 0);
	string_list_clear_func(&حالتون->ڌڪو, clear_ڌڪو_info);
}

ساكن صحيح append_ref_منزل_tracked_list(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية ref_حالتون *حالتون = cb_معلومات;
	بنية refspec_شي refspec;

	إذا (ref->flags & REF_ISSYMREF)
		أرجع 0;

	memset(&refspec, 0, حجم(refspec));
	refspec.dst = (حرف *)ref->name;
	إذا (!remote_ڳوليو_tracking(حالتون->remote, &refspec)) {
		string_list_append(&حالتون->tracked, abbrev_branch(refspec.src));
		حرر(refspec.src);
	}

	أرجع 0;
}

ساكن صحيح get_remote_ref_حالتون(ثابت حرف *name,
				 بنية ref_حالتون *حالتون,
				 صحيح query)
{
	حالتون->remote = remote_get(name);
	إذا (!حالتون->remote)
		أرجع error(_("No such remote: '%s'"), name);

	read_branches();

	إذا (query) {
		بنية transport *transport;
		ثابت بنية ref *remote_refs;

		transport = transport_get(حالتون->remote, حالتون->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, عدم);

		حالتون->queried = 1;
		إذا (query & GET_REF_حالتون)
			get_ref_حالتون(remote_refs, حالتون);
		إذا (query & GET_سر_نالا)
			get_سر_نالا(remote_refs, حالتون);
		إذا (query & GET_ڌڪو_REF_حالتون)
			get_ڌڪو_ref_حالتون(remote_refs, حالتون);
		transport_disconnect(transport);
	} وإلا {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_منزل_tracked_list, حالتون);
		string_list_ترتيب_ڏيو(&حالتون->tracked);
		get_ڌڪو_ref_حالتون_noquery(حالتون);
	}

	أرجع 0;
}

بنية show_info {
	بنية string_list list;
	بنية ref_حالتون حالتون;
	صحيح ويڪر, width2;
	صحيح any_rebase;
};

#وصف_ڪريو SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.حالتون = REF_حالتون_INIT, \
}

ساكن صحيح جوڙيو_remote_منزل_show_info(بنية string_list_شي *شي, فراغ *cb_معلومات)
{
	بنية show_info *info = cb_معلومات;
	صحيح n = لڙي_ڊيگهه(شي->string);
	إذا (n > info->ويڪر)
		info->ويڪر = n;
	string_list_وجهو(&info->list, شي->string);
	أرجع 0;
}

ساكن صحيح show_remote_info_شي(بنية string_list_شي *شي, فراغ *cb_معلومات)
{
	بنية show_info *info = cb_معلومات;
	بنية ref_حالتون *حالتون = &info->حالتون;
	ثابت حرف *name = شي->string;

	إذا (حالتون->queried) {
		ثابت حرف *fmt = "%s";
		ثابت حرف *arg = "";
		إذا (string_list_has_string(&حالتون->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = حالتون->remote->name;
		} وإلا إذا (string_list_has_string(&حالتون->tracked, name))
			arg = _(" tracked");
		وإلا إذا (string_list_has_string(&حالتون->skipped, name))
			arg = _(" skipped");
		وإلا إذا (string_list_has_string(&حالتون->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		وإلا
			arg = _(" ???");
		لکو("    %-*s", info->ويڪر, name);
		لکو(fmt, arg);
		لکو("\n");
	} وإلا
		لکو("    %s\n", name);

	أرجع 0;
}

ساكن صحيح جوڙيو_local_منزل_show_info(بنية string_list_شي *branch_شي, فراغ *cb_معلومات)
{
	بنية show_info *show_info = cb_معلومات;
	بنية ref_حالتون *حالتون = &show_info->حالتون;
	بنية branch_info *branch_info = branch_شي->util;
	بنية string_list_شي *شي;
	صحيح n;

	إذا (!branch_info->ملايو.nr || !branch_info->remote_name ||
	    strcmp(حالتون->remote->name, branch_info->remote_name))
		أرجع 0;
	إذا ((n = لڙي_ڊيگهه(branch_شي->string)) > show_info->ويڪر)
		show_info->ويڪر = n;
	إذا (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	شي = string_list_وجهو(&show_info->list, branch_شي->string);
	شي->util = branch_info;

	أرجع 0;
}

ساكن صحيح show_local_info_شي(بنية string_list_شي *شي, فراغ *cb_معلومات)
{
	بنية show_info *show_info = cb_معلومات;
	بنية branch_info *branch_info = شي->util;
	بنية string_list *ملايو = &branch_info->ملايو;
	صحيح ويڪر = show_info->ويڪر + 4;

	إذا (branch_info->rebase >= REBASE_TRUE && branch_info->ملايو.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			شي->string);
		أرجع 0;
	}

	لکو("    %-*s ", show_info->ويڪر, شي->string);
	إذا (branch_info->rebase >= REBASE_TRUE) {
		ثابت حرف *نياپو;
		إذا (branch_info->rebase == REBASE_INTERACTIVE)
			نياپو = _("rebases interactively onto remote %s");
		وإلا إذا (branch_info->rebase == REBASE_MERGES)
			نياپو = _("rebases interactively (with merges) onto "
				"remote %s");
		وإلا
			نياپو = _("rebases onto remote %s");
		printf_ln(نياپو, ملايو->شيون[0].string);
		أرجع 0;
	} وإلا إذا (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), ملايو->شيون[0].string);
		ويڪر++;
	} وإلا {
		printf_ln(_("merges with remote %s"), ملايو->شيون[0].string);
	}
	لكل (سائيز_t i = 1; i < ملايو->nr; i++)
		لکو(_("%-*s    and with remote %s\n"), ويڪر, "",
		       ملايو->شيون[i].string);

	أرجع 0;
}

ساكن صحيح جوڙيو_ڌڪو_منزل_show_info(بنية string_list_شي *ڌڪو_شي, فراغ *cb_معلومات)
{
	بنية show_info *show_info = cb_معلومات;
	بنية ڌڪو_info *ڌڪو_info = ڌڪو_شي->util;
	بنية string_list_شي *شي;
	صحيح n;
	إذا ((n = لڙي_ڊيگهه(ڌڪو_شي->string)) > show_info->ويڪر)
		show_info->ويڪر = n;
	إذا ((n = لڙي_ڊيگهه(ڌڪو_info->dest)) > show_info->width2)
		show_info->width2 = n;
	شي = string_list_append(&show_info->list, ڌڪو_شي->string);
	شي->util = ڌڪو_شي->util;
	أرجع 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
ساكن صحيح cmp_string_with_ڌڪو(ثابت فراغ *va, ثابت فراغ *vb)
{
	ثابت بنية string_list_شي *a = va;
	ثابت بنية string_list_شي *b = vb;
	ثابت بنية ڌڪو_info *a_ڌڪو = a->util;
	ثابت بنية ڌڪو_info *b_ڌڪو = b->util;
	صحيح cmp = strcmp(a->string, b->string);
	أرجع cmp ? cmp : strcmp(a_ڌڪو->dest, b_ڌڪو->dest);
}

ساكن صحيح show_ڌڪو_info_شي(بنية string_list_شي *شي, فراغ *cb_معلومات)
{
	بنية show_info *show_info = cb_معلومات;
	بنية ڌڪو_info *ڌڪو_info = شي->util;
	ثابت حرف *src = شي->string, *حالت = عدم;

	بدل (ڌڪو_info->حالت) {
	حالة ڌڪو_حالت_CREATE:
		حالت = _("create");
		اكسر;
	حالة ڌڪو_حالت_DELETE:
		حالت = _("delete");
		src = _("(none)");
		اكسر;
	حالة ڌڪو_حالت_UPTODATE:
		حالت = _("up to date");
		اكسر;
	حالة ڌڪو_حالت_FASTFORWARD:
		حالت = _("fast-forwardable");
		اكسر;
	حالة ڌڪو_حالت_OUTOFDATE:
		حالت = _("local out of date");
		اكسر;
	حالة ڌڪو_حالت_NOTQUERIED:
		اكسر;
	}
	إذا (حالت) {
		إذا (ڌڪو_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->ويڪر, src,
			       show_info->width2, ڌڪو_info->dest, حالت);
		وإلا
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->ويڪر, src,
			       show_info->width2, ڌڪو_info->dest, حالت);
	} وإلا {
		إذا (ڌڪو_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->ويڪر, src,
			       ڌڪو_info->dest);
		وإلا
			printf_ln(_("    %-*s pushes to %s"), show_info->ويڪر, src,
			       ڌڪو_info->dest);
	}
	أرجع 0;
}

ساكن صحيح get_one_داخلا(بنية remote *remote, فراغ *priv)
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
	url = ڌڪو_url_جو_remote(remote);
	لكل (سائيز_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, عدم);
	}

	أرجع 0;
}

ساكن صحيح show_all(فراغ)
{
	بنية string_list list = STRING_LIST_INIT_DUP;
	صحيح نتيجو;

	نتيجو = for_each_remote(get_one_داخلا, &list);

	إذا (!نتيجو) {
		string_list_ترتيب_ڏيو(&list);
		لكل (سائيز_t i = 0; i < list.nr; i++) {
			بنية string_list_شي *شي = list.شيون + i;
			إذا (verbose)
				لکو("%s\t%s\n", شي->string,
					شي->util ? (ثابت حرف *)شي->util : "");
			وإلا {
				إذا (i && !strcmp((شي - 1)->string, شي->string))
					استمر;
				لکو("%s\n", شي->string);
			}
		}
	}
	string_list_clear(&list, 1);
	أرجع نتيجو;
}

ساكن صحيح show(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح no_query = 0, نتيجو = 0, query_flag = 0;
	بنية option options[] = {
		OPT_BOOL('n', عدم, &no_query, N_("do not query remotes")),
		OPT_پڇاڙي()
	};
	بنية show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_remote_show_usage,
			     0);

	إذا (argc < 1)
		أرجع show_all();

	إذا (!no_query)
		query_flag = (GET_REF_حالتون | GET_سر_نالا | GET_ڌڪو_REF_حالتون);

	لكل (; argc; argc--, argv++) {
		سائيز_t i;
		بنية strvec *url;

		get_remote_ref_حالتون(*argv, &info.حالتون, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.حالتون.remote->url.v[0]);
		url = ڌڪو_url_جو_remote(info.حالتون.remote);
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
		وإلا إذا (!info.حالتون.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		وإلا إذا (info.حالتون.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.حالتون.heads.شيون[0].string);
		وإلا {
			لکو(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			لكل (i = 0; i < info.حالتون.heads.nr; i++)
				لکو("    %s\n", info.حالتون.heads.شيون[i].string);
		}

		/* remote branch info */
		info.ويڪر = 0;
		for_each_string_list(&info.حالتون.new_refs, جوڙيو_remote_منزل_show_info, &info);
		for_each_string_list(&info.حالتون.skipped, جوڙيو_remote_منزل_show_info, &info);
		for_each_string_list(&info.حالتون.tracked, جوڙيو_remote_منزل_show_info, &info);
		for_each_string_list(&info.حالتون.stale, جوڙيو_remote_منزل_show_info, &info);
		إذا (info.list.nr)
			printf_ln(Q_("  Remote branch:%s",
				     "  Remote branches:%s",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_remote_info_شي, &info);
		string_list_clear(&info.list, 0);

		/* git pull info */
		info.ويڪر = 0;
		info.any_rebase = 0;
		for_each_string_list(&branch_list, جوڙيو_local_منزل_show_info, &info);
		إذا (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_شي, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		إذا (info.حالتون.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.ويڪر = info.width2 = 0;
		for_each_string_list(&info.حالتون.ڌڪو, جوڙيو_ڌڪو_منزل_show_info, &info);
		QSORT(info.list.شيون, info.list.nr, cmp_string_with_ڌڪو);
		إذا (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_ڌڪو_info_شي, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_حالتون(&info.حالتون);
	}

	أرجع نتيجو;
}

ساكن فراغ report_سيٽ_سر_auto(ثابت حرف *remote, ثابت حرف *سر_name,
			بنية strbuf *b_local_سر, صحيح was_detached) {
	بنية strbuf buf_اڳياڙي = STRBUF_INIT;
	ثابت حرف *prev_سر = عدم;

	strbuf_addf(&buf_اڳياڙي, "refs/remotes/%s/", remote);
	skip_اڳياڙي(b_local_سر->buf, buf_اڳياڙي.buf, &prev_سر);

	إذا (prev_سر && !strcmp(prev_سر, سر_name))
		لکو(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, سر_name);
	وإلا إذا (prev_سر)
		لکو(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_سر, سر_name);
	وإلا إذا (!b_local_سر->len)
		لکو(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, سر_name);
	وإلا إذا (was_detached && b_local_سر->len)
		لکو(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_سر->buf, سر_name);
	وإلا
		لکو(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_سر->buf, سر_name);
	strbuf_release(&buf_اڳياڙي);
}

ساكن صحيح سيٽ_سر(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		    بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح opt_a = 0, opt_d = 0, نتيجو = 0, was_detached;
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
		OPT_پڇاڙي()
	};
	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	إذا (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_سر, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	إذا (!opt_a && !opt_d && argc == 2) {
		سر_name = xstrdup(argv[1]);
	} وإلا إذا (opt_a && !opt_d && argc == 1) {
		بنية ref_حالتون حالتون = REF_حالتون_INIT;
		get_remote_ref_حالتون(argv[0], &حالتون, GET_سر_نالا);
		إذا (!حالتون.heads.nr)
			نتيجو |= error(_("Cannot determine remote HEAD"));
		وإلا إذا (حالتون.heads.nr > 1) {
			نتيجو |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			لكل (سائيز_t i = 0; i < حالتون.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], حالتون.heads.شيون[i].string);
		} وإلا
			سر_name = xstrdup(حالتون.heads.شيون[0].string);
		free_remote_ref_حالتون(&حالتون);
	} وإلا إذا (opt_d && !opt_a && argc == 1) {
		إذا (refs_delete_ref(refs, عدم, b_سر.buf, عدم, REF_NO_DEREF))
			نتيجو |= error(_("Could not delete %s"), b_سر.buf);
	} وإلا
		usage_with_options(builtin_remote_sethead_usage, options);

	إذا (!سر_name)
		اذهب cleanup;
	strbuf_addf(&b_remote_سر, "refs/remotes/%s/%s", argv[0], سر_name);
	إذا (!refs_ref_exists(refs, b_remote_سر.buf)) {
		نتيجو |= error(_("Not a valid ref: %s"), b_remote_سر.buf);
		اذهب cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_سر.buf, b_remote_سر.buf,
			"remote set-head", &b_local_سر, 0);
	إذا (was_detached == -1) {
		نتيجو |= error(_("Could not set up %s"), b_سر.buf);
		اذهب cleanup;
	}
	إذا (opt_a)
		report_سيٽ_سر_auto(argv[0], سر_name, &b_local_سر, was_detached);
	إذا (remote->follow_remote_سر == FOLLOW_REMOTE_ALWAYS) {
		بنية strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_سيٽ(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	حرر(سر_name);
	strbuf_release(&b_سر);
	strbuf_release(&b_remote_سر);
	strbuf_release(&b_local_سر);
	أرجع نتيجو;
}

ساكن صحيح prune_remote(ثابت حرف *remote, صحيح dry_لاڳيتو)
{
	صحيح نتيجو = 0;
	بنية ref_حالتون حالتون = REF_حالتون_INIT;
	بنية string_list refs_منزل_prune = STRING_LIST_INIT_NODUP;
	بنية string_list_شي *شي;

	get_remote_ref_حالتون(remote, &حالتون, GET_REF_حالتون);

	إذا (!حالتون.stale.nr) {
		free_remote_ref_حالتون(&حالتون);
		أرجع 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), حالتون.remote->url.v[0]);

	for_each_string_list_شي(شي, &حالتون.stale)
		string_list_append(&refs_منزل_prune, شي->util);
	string_list_ترتيب_ڏيو(&refs_منزل_prune);

	إذا (!dry_لاڳيتو)
		نتيجو |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_منزل_prune, 0);

	for_each_string_list_شي(شي, &حالتون.stale) {
		ثابت حرف *refname = شي->util;

		إذا (dry_لاڳيتو)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		وإلا
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_لاڳيتو, &refs_منزل_prune);

	string_list_clear(&refs_منزل_prune, 0);
	free_remote_ref_حالتون(&حالتون);
	أرجع نتيجو;
}

ساكن صحيح prune(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		 بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح dry_لاڳيتو = 0, نتيجو = 0;
	بنية option options[] = {
		OPT_DRY_لاڳيتو(&dry_لاڳيتو, N_("dry run")),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_remote_prune_usage, 0);

	إذا (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	لكل (; argc; argc--, argv++)
		نتيجو |= prune_remote(*argv, dry_لاڳيتو);

	أرجع نتيجو;
}

ساكن صحيح get_remote_default(ثابت حرف *ڪنجي, ثابت حرف *قدر اڻ_استعمال_ٿيل,
			      ثابت بنية config_context *ctx اڻ_استعمال_ٿيل,
			      فراغ *priv)
{
	إذا (strcmp(ڪنجي, "remotes.default") == 0) {
		صحيح *مليو_2 = priv;
		*مليو_2 = 1;
	}
	أرجع 0;
}

ساكن صحيح update(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		  بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح i, prune = -1;
	بنية option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_پڇاڙي()
	};
	بنية ٻار_process cmd = ٻار_PROCESS_INIT;
	صحيح default_defined = 0;

	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_ڌڪو(&cmd.دليلون, "fetch");

	إذا (prune != -1)
		strvec_ڌڪو(&cmd.دليلون, prune ? "--prune" : "--no-prune");
	إذا (verbose)
		strvec_ڌڪو(&cmd.دليلون, "-v");
	strvec_ڌڪو(&cmd.دليلون, "--multiple");
	إذا (argc < 2)
		strvec_ڌڪو(&cmd.دليلون, "default");
	لكل (i = 1; i < argc; i++)
		strvec_ڌڪو(&cmd.دليلون, argv[i]);

	إذا (strcmp(cmd.دليلون.v[cmd.دليلون.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		إذا (!default_defined) {
			strvec_ڪڍو(&cmd.دليلون);
			strvec_ڌڪو(&cmd.دليلون, "--all");
		}
	}

	cmd.git_cmd = 1;
	أرجع لاڳيتو_command(&cmd);
}

ساكن صحيح remove_all_fetch_refspecs(ثابت حرف *ڪنجي)
{
	أرجع repo_config_سيٽ_multivar_gently(the_repository, ڪنجي, عدم, عدم,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

ساكن فراغ جوڙيو_branches(بنية remote *remote, ثابت حرف **branches,
			 ثابت حرف *ڪنجي)
{
	ثابت حرف *remotename = remote->name;
	صحيح mirror = remote->mirror;
	بنية strbuf refspec = STRBUF_INIT;

	لكل (; *branches; branches++)
		جوڙيو_branch(ڪنجي, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

ساكن صحيح سيٽ_remote_branches(ثابت حرف *remotename, ثابت حرف **branches,
				صحيح جوڙيو_گهڻائي)
{
	بنية strbuf ڪنجي = STRBUF_INIT;
	بنية remote *remote;

	strbuf_addf(&ڪنجي, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		ٻاهر_نڪرو(2);
	}

	إذا (!جوڙيو_گهڻائي && remove_all_fetch_refspecs(ڪنجي.buf)) {
		strbuf_release(&ڪنجي);
		أرجع 1;
	}
	جوڙيو_branches(remote, branches, ڪنجي.buf);

	strbuf_release(&ڪنجي);
	أرجع 0;
}

ساكن صحيح سيٽ_branches(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
			بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح جوڙيو_گهڻائي = 0;
	بنية option options[] = {
		OPT_BOOL('\0', "add", &جوڙيو_گهڻائي, N_("add branch")),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_remote_setbranches_usage, 0);
	إذا (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = عدم;

	أرجع سيٽ_remote_branches(argv[0], argv + 1, جوڙيو_گهڻائي);
}

ساكن صحيح get_url(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		   بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح ڌڪو_گهڻائي = 0, all_گهڻائي = 0;
	ثابت حرف *remotename = عدم;
	بنية remote *remote;
	بنية strvec *url;
	بنية option options[] = {
		OPT_BOOL('\0', "push", &ڌڪو_گهڻائي,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_گهڻائي,
			 N_("return all URLs")),
		OPT_پڇاڙي()
	};
	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_remote_geturl_usage, 0);

	إذا (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		ٻاهر_نڪرو(2);
	}

	url = ڌڪو_گهڻائي ? ڌڪو_url_جو_remote(remote) : &remote->url;

	إذا (all_گهڻائي) {
		لكل (سائيز_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} وإلا {
		printf_ln("%s", url->v[0]);
	}

	أرجع 0;
}

ساكن صحيح سيٽ_url(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي,
		   بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح ڌڪو_گهڻائي = 0, جوڙيو_گهڻائي = 0, delete_گهڻائي = 0;
	صحيح matches = 0, negative_matches = 0;
	ثابت حرف *remotename = عدم;
	ثابت حرف *newurl = عدم;
	ثابت حرف *oldurl = عدم;
	بنية remote *remote;
	regex_t old_regex;
	بنية strvec *urlset;
	بنية strbuf name_buf = STRBUF_INIT;
	بنية option options[] = {
		OPT_BOOL('\0', "push", &ڌڪو_گهڻائي,
			 N_("manipulate push URLs")),
		OPT_BOOL('\0', "add", &جوڙيو_گهڻائي,
			 N_("add URL")),
		OPT_BOOL('\0', "delete", &delete_گهڻائي,
			    N_("delete URLs")),
		OPT_پڇاڙي()
	};
	argc = parse_options(argc, argv, اڳياڙي, options,
			     builtin_remote_seturl_usage,
			     PARSE_OPT_KEEP_ARGV0);

	إذا (جوڙيو_گهڻائي && delete_گهڻائي)
		die(_("--add --delete doesn't make sense"));

	إذا (argc < 3 || argc > 4 || ((جوڙيو_گهڻائي || delete_گهڻائي) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	إذا (argc > 3)
		oldurl = argv[3];

	إذا (delete_گهڻائي)
		oldurl = newurl;

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		ٻاهر_نڪرو(2);
	}

	إذا (ڌڪو_گهڻائي) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} وإلا {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	إذا ((!oldurl && !delete_گهڻائي) || جوڙيو_گهڻائي) {
		إذا (جوڙيو_گهڻائي)
			repo_config_سيٽ_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		وإلا
			repo_config_سيٽ(the_repository, name_buf.buf, newurl);
		اذهب ٻاهريون;
	}

	/* Old URL specified. Demand that one matches. */
	إذا (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	لكل (سائيز_t i = 0; i < urlset->nr; i++)
		إذا (!regexec(&old_regex, urlset->v[i], 0, عدم, 0))
			matches++;
		وإلا
			negative_matches++;
	إذا (!delete_گهڻائي && !matches)
		die(_("No such URL found: %s"), oldurl);
	إذا (delete_گهڻائي && !negative_matches && !ڌڪو_گهڻائي)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	إذا (!delete_گهڻائي)
		repo_config_سيٽ_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	وإلا
		repo_config_سيٽ_multivar(the_repository, name_buf.buf, عدم, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
ٻاهريون:
	strbuf_release(&name_buf);
	أرجع 0;
}

صحيح cmd_remote(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *اڳياڙي,
	       بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, جوڙيو),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, عدم, PARSE_OPT_NOCOMPLETE),
		OPT_SUBCOMMAND("remove", &fn, rm),
		OPT_SUBCOMMAND("set-head", &fn, سيٽ_سر),
		OPT_SUBCOMMAND("set-branches", &fn, سيٽ_branches),
		OPT_SUBCOMMAND("get-url", &fn, get_url),
		OPT_SUBCOMMAND("set-url", &fn, سيٽ_url),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("update", &fn, update),
		OPT_پڇاڙي()
	};

	argc = parse_options(argc, argv, اڳياڙي, options, builtin_remote_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	إذا (fn) {
		أرجع !!fn(argc, argv, اڳياڙي, repo);
	} وإلا {
		إذا (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		أرجع !!show_all();
	}
}
