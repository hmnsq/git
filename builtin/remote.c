#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "date.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ident.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "transport.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rebase.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit-reach.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "progress.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_remote_usage[] = {
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
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_remote_ਜੋੜੋ_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	ਨਿਰਾ
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_REF_ਹਾਲਤਾਂ (1<<0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_ਸਿਰ_ਨਾਂ (1<<1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_ਧੱਕੋ_REF_ਹਾਲਤਾਂ (1<<2)

ਸਥਿਰ ਪੂਰਨ verbose;

ਸਥਿਰ ਪੂਰਨ fetch_remote(ਅਚਲ ਅੱਖਰ *name)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "fetch");
	ਜੇ (verbose)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "-v");
	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	ਜੇ (ਲੜੀਵਾਰ_command(&cmd))
		ਵਾਪਸ error(_("Could not fetch %s"), name);
	ਵਾਪਸ 0;
}

ਗਿਣਤੀ {
	ਟੈਗ_ਸੂਚੀ_UNSET = 0,
	ਟੈਗ_ਸੂਚੀ_DEFAULT = 1,
	ਟੈਗ_ਸੂਚੀ_ਸੈੱਟ = 2
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MIRROR_NONE 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MIRROR_FETCH 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MIRROR_ਧੱਕੋ 2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MIRROR_BOTH (MIRROR_FETCH|MIRROR_ਧੱਕੋ)

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_branch(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ *branchname,
		       ਅਚਲ ਅੱਖਰ *remotename, ਪੂਰਨ mirror, ਬਣਤਰ strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	ਜੇ (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	ਨਹੀਂਤਾਂ
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_ਸੈੱਟ_multivar(the_repository, ਕੁੰਜੀ, tmp->buf, "^$", 0);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

ਸਥਿਰ ਪੂਰਨ parse_mirror_opt(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ not)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ *mirror = opt->ਮੁੱਲ;
	ਜੇ (not)
		*mirror = MIRROR_NONE;
	ਨਹੀਂਤਾਂ ਜੇ (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "push"))
		*mirror = MIRROR_ਧੱਕੋ;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("unknown --mirror argument: %s"), arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ check_remote_collision(ਬਣਤਰ remote *remote, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਅਚਲ ਅੱਖਰ *name = ਅੰਕੜੇ;
	ਅਚਲ ਅੱਖਰ *p;

	ਜੇ (skip_ਅਗੇਤਰ(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	ਜੇ (skip_ਅਗੇਤਰ(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ fetch = 0, fetch_ਟੈਗ_ਸੂਚੀ = ਟੈਗ_ਸੂਚੀ_DEFAULT;
	ਬਿਨਾਨਿਸ਼ਾਨੀ mirror = MIRROR_NONE;
	ਬਣਤਰ string_list track = STRING_LIST_INIT_NODUP;
	ਅਚਲ ਅੱਖਰ *master = ਨਿਰਾ;
	ਬਣਤਰ remote *remote;
	ਬਣਤਰ strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *name, *url;
	ਪੂਰਨ ਨਤੀਜਾ = 0;

	ਬਣਤਰ option options[] = {
		OPT_BOOL('f', "fetch", &fetch, N_("fetch the remote branches")),
		OPT_ਸੈੱਟ_INT(0, "tags", &fetch_ਟੈਗ_ਸੂਚੀ,
			    N_("import all tags and associated objects when fetching\n"
			       "or do not fetch any tag at all (--no-tags)"),
			    ਟੈਗ_ਸੂਚੀ_ਸੈੱਟ),
		OPT_STRING_LIST('t', "track", &track, N_("branch"),
				N_("branch(es) to track")),
		OPT_STRING('m', "master", &master, N_("branch"), N_("master branch")),
		OPT_CALLBACK_F(0, "mirror", &mirror, "(push|fetch)",
			N_("set up remote as a mirror to push to or fetch from"),
			PARSE_OPT_OPTARG | PARSE_OPT_COMP_ARG, parse_mirror_opt),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_remote_ਜੋੜੋ_usage, 0);

	ਜੇ (argc != 2)
		usage_with_options(builtin_remote_ਜੋੜੋ_usage, options);

	ਜੇ (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	ਜੇ (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	ਜੇ (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		ਬਾਹਰ_ਨਿਕਲੋ(3);
	}

	ਜੇ (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (ਖਾਲੀ *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_ਸੈੱਟ(the_repository, buf.buf, url);

	ਜੇ (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		ਜੇ (track.nr == 0)
			string_list_append(&track, "*");
		ਲਈ (ਮਾਪ_t i = 0; i < track.nr; i++) {
			ਜੋੜੋ_branch(buf.buf, track.ਚੀਜ਼ਾਂ[i].string,
				   name, mirror, &buf2);
		}
	}

	ਜੇ (mirror & MIRROR_ਧੱਕੋ) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_ਸੈੱਟ(the_repository, buf.buf, "true");
	}

	ਜੇ (fetch_ਟੈਗ_ਸੂਚੀ != ਟੈਗ_ਸੂਚੀ_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_ਸੈੱਟ(the_repository, buf.buf,
				fetch_ਟੈਗ_ਸੂਚੀ == ਟੈਗ_ਸੂਚੀ_ਸੈੱਟ ? "--tags" : "--no-tags");
	}

	ਜੇ (fetch && fetch_remote(name)) {
		ਨਤੀਜਾ = 1;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		ਜੇ (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			ਨਤੀਜਾ = error(_("Could not setup master '%s'"), master);
	}

ਬਾਹਰਲਾ:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	ਵਾਪਸ ਨਤੀਜਾ;
}

ਬਣਤਰ branch_info {
	ਅੱਖਰ *remote_name;
	ਬਣਤਰ string_list ਮਿਲਾਓ;
	ਗਿਣਤੀ rebase_type rebase;
	ਅੱਖਰ *ਧੱਕੋ_remote_name;
};

ਸਥਿਰ ਬਣਤਰ string_list branch_list = STRING_LIST_INIT_DUP;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *abbrev_ref(ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	skip_ਅਗੇਤਰ(name, ਅਗੇਤਰ, &name);
	ਵਾਪਸ name;
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ abbrev_branch(name) abbrev_ref((name), "refs/heads/")

ਸਥਿਰ ਪੂਰਨ config_read_branches(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
				ਅਚਲ ਬਣਤਰ config_context *ctx ਅਣਵਰਤਿਆ,
				ਖਾਲੀ *ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *orig_ਕੁੰਜੀ = ਕੁੰਜੀ;
	ਅੱਖਰ *name;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਬਣਤਰ branch_info *info;
	ਗਿਣਤੀ { REMOTE, ਮਿਲਾਓ_2, REBASE, ਧੱਕੋ_REMOTE } type;
	ਮਾਪ_t ਕੁੰਜੀ_len;

	ਜੇ (!starts_with(ਕੁੰਜੀ, "branch."))
		ਵਾਪਸ 0;

	ਕੁੰਜੀ += ਲੜੀ_ਲੰਬਾਈ("branch.");
	ਜੇ (strip_suffix(ਕੁੰਜੀ, ".remote", &ਕੁੰਜੀ_len))
		type = REMOTE;
	ਨਹੀਂਤਾਂ ਜੇ (strip_suffix(ਕੁੰਜੀ, ".merge", &ਕੁੰਜੀ_len))
		type = ਮਿਲਾਓ_2;
	ਨਹੀਂਤਾਂ ਜੇ (strip_suffix(ਕੁੰਜੀ, ".rebase", &ਕੁੰਜੀ_len))
		type = REBASE;
	ਨਹੀਂਤਾਂ ਜੇ (strip_suffix(ਕੁੰਜੀ, ".pushremote", &ਕੁੰਜੀ_len))
		type = ਧੱਕੋ_REMOTE;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ 0;

	name = xmemdupz(ਕੁੰਜੀ, ਕੁੰਜੀ_len);
	ਵਸਤੂ = string_list_ਪਾਓ(&branch_list, name);

	ਜੇ (!ਵਸਤੂ->util)
		ਵਸਤੂ->util = xcalloc(1, ਆਕਾਰ(ਬਣਤਰ branch_info));
	info = ਵਸਤੂ->util;
	ਚੋਣ (type) {
	ਹਾਲਤ REMOTE:
		ਜੇ (info->remote_name)
			warning(_("more than one %s"), orig_ਕੁੰਜੀ);
		info->remote_name = xstrdup(ਮੁੱਲ);
		ਰੋਕੋ;
	ਹਾਲਤ ਮਿਲਾਓ_2: {
		ਅਚਲ ਅੱਖਰ *space = strchr(ਮੁੱਲ, ' ');
		ਮੁੱਲ = abbrev_branch(ਮੁੱਲ);
		ਜਦੋਂ (space) {
			ਅੱਖਰ *ਮਿਲਾਓ;
			ਮਿਲਾਓ = xstrndup(ਮੁੱਲ, space - ਮੁੱਲ);
			string_list_append(&info->ਮਿਲਾਓ, ਮਿਲਾਓ);
			ਮੁੱਲ = abbrev_branch(space + 1);
			space = strchr(ਮੁੱਲ, ' ');
		}
		string_list_append(&info->ਮਿਲਾਓ, xstrdup(ਮੁੱਲ));
		ਰੋਕੋ;
	}
	ਹਾਲਤ REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_ਮੁੱਲ(ਮੁੱਲ);
		ਜੇ (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, ਮੁੱਲ);
		ਰੋਕੋ;
	ਹਾਲਤ ਧੱਕੋ_REMOTE:
		ਜੇ (info->ਧੱਕੋ_remote_name)
			warning(_("more than one %s"), orig_ਕੁੰਜੀ);
		info->ਧੱਕੋ_remote_name = xstrdup(ਮੁੱਲ);
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("unexpected type=%d", type);
	}

	ਮੁਕਤ(name);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ read_branches(ਖਾਲੀ)
{
	ਜੇ (branch_list.nr)
		ਵਾਪਸ;
	repo_config(the_repository, config_read_branches, ਨਿਰਾ);
}

ਬਣਤਰ ref_ਹਾਲਤਾਂ {
	ਬਣਤਰ remote *remote;
	ਬਣਤਰ string_list new_refs, skipped, stale, tracked, heads, ਧੱਕੋ;
	ਪੂਰਨ queried;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REF_ਹਾਲਤਾਂ_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.ਧੱਕੋ = STRING_LIST_INIT_DUP, \
}

ਸਥਿਰ ਪੂਰਨ get_ref_ਹਾਲਤਾਂ(ਅਚਲ ਬਣਤਰ ref *remote_refs, ਬਣਤਰ ref_ਹਾਲਤਾਂ *ਹਾਲਤਾਂ)
{
	ਬਣਤਰ ref *fetch_map = ਨਿਰਾ, **ਪੂਛ = &fetch_map;
	ਬਣਤਰ ref *ref, *stale_refs;
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < ਹਾਲਤਾਂ->remote->fetch.nr; i++)
		ਜੇ (get_fetch_map(remote_refs, &ਹਾਲਤਾਂ->remote->fetch.ਚੀਜ਼ਾਂ[i], &ਪੂਛ, 1))
			die(_("Could not get fetch map for refspec %s"),
				ਹਾਲਤਾਂ->remote->fetch.ਚੀਜ਼ਾਂ[i].raw);

	ਲਈ (ref = fetch_map; ref; ref = ref->ਅਗਲਾ) {
		ਜੇ (refname_matches_negative_refspec_ਵਸਤੂ(ref->name, &ਹਾਲਤਾਂ->remote->fetch))
			string_list_append(&ਹਾਲਤਾਂ->skipped, abbrev_branch(ref->name));
		ਨਹੀਂਤਾਂ ਜੇ (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&ਹਾਲਤਾਂ->new_refs, abbrev_branch(ref->name));
		ਨਹੀਂਤਾਂ
			string_list_append(&ਹਾਲਤਾਂ->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&ਹਾਲਤਾਂ->remote->fetch, fetch_map);
	ਲਈ (ref = stale_refs; ref; ref = ref->ਅਗਲਾ) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ =
			string_list_append(&ਹਾਲਤਾਂ->stale, abbrev_branch(ref->name));
		ਵਸਤੂ->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_ਛਾਂਟੋ(&ਹਾਲਤਾਂ->new_refs);
	string_list_ਛਾਂਟੋ(&ਹਾਲਤਾਂ->skipped);
	string_list_ਛਾਂਟੋ(&ਹਾਲਤਾਂ->tracked);
	string_list_ਛਾਂਟੋ(&ਹਾਲਤਾਂ->stale);

	ਵਾਪਸ 0;
}

ਬਣਤਰ ਧੱਕੋ_info {
	ਅੱਖਰ *dest;
	ਪੂਰਨ forced;
	ਗਿਣਤੀ {
		ਧੱਕੋ_ਸਥਿਤੀ_CREATE = 0,
		ਧੱਕੋ_ਸਥਿਤੀ_DELETE,
		ਧੱਕੋ_ਸਥਿਤੀ_UPTODATE,
		ਧੱਕੋ_ਸਥਿਤੀ_FASTFORWARD,
		ਧੱਕੋ_ਸਥਿਤੀ_OUTOFDATE,
		ਧੱਕੋ_ਸਥਿਤੀ_NOTQUERIED
	} ਸਥਿਤੀ;
};

ਸਥਿਰ ਪੂਰਨ get_ਧੱਕੋ_ref_ਹਾਲਤਾਂ(ਅਚਲ ਬਣਤਰ ref *remote_refs,
	ਬਣਤਰ ref_ਹਾਲਤਾਂ *ਹਾਲਤਾਂ)
{
	ਬਣਤਰ remote *remote = ਹਾਲਤਾਂ->remote;
	ਬਣਤਰ ref *ref, *local_refs, *ਧੱਕੋ_map;
	ਜੇ (remote->mirror)
		ਵਾਪਸ 0;

	local_refs = get_local_heads();
	ਧੱਕੋ_map = copy_ref_list(remote_refs);

	ਮੇਲ_ਧੱਕੋ_refs(local_refs, &ਧੱਕੋ_map, &remote->ਧੱਕੋ, ਮੇਲ_REFS_NONE);

	ਲਈ (ref = ਧੱਕੋ_map; ref; ref = ref->ਅਗਲਾ) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
		ਬਣਤਰ ਧੱਕੋ_info *info;

		ਜੇ (!ref->peer_ref)
			ਜਾਰੀ;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		ਵਸਤੂ = string_list_append(&ਹਾਲਤਾਂ->ਧੱਕੋ,
					  abbrev_branch(ref->peer_ref->name));
		ਵਸਤੂ->util = xcalloc(1, ਆਕਾਰ(ਬਣਤਰ ਧੱਕੋ_info));
		info = ਵਸਤੂ->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		ਜੇ (is_null_oid(&ref->new_oid)) {
			info->ਸਥਿਤੀ = ਧੱਕੋ_ਸਥਿਤੀ_DELETE;
		} ਨਹੀਂਤਾਂ ਜੇ (oideq(&ref->old_oid, &ref->new_oid))
			info->ਸਥਿਤੀ = ਧੱਕੋ_ਸਥਿਤੀ_UPTODATE;
		ਨਹੀਂਤਾਂ ਜੇ (is_null_oid(&ref->old_oid))
			info->ਸਥਿਤੀ = ਧੱਕੋ_ਸਥਿਤੀ_CREATE;
		ਨਹੀਂਤਾਂ ਜੇ (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->ਸਥਿਤੀ = ਧੱਕੋ_ਸਥਿਤੀ_FASTFORWARD;
		ਨਹੀਂਤਾਂ
			info->ਸਥਿਤੀ = ਧੱਕੋ_ਸਥਿਤੀ_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(ਧੱਕੋ_map);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_ਧੱਕੋ_ref_ਹਾਲਤਾਂ_noquery(ਬਣਤਰ ref_ਹਾਲਤਾਂ *ਹਾਲਤਾਂ)
{
	ਪੂਰਨ i;
	ਬਣਤਰ remote *remote = ਹਾਲਤਾਂ->remote;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਬਣਤਰ ਧੱਕੋ_info *info;

	ਜੇ (remote->mirror)
		ਵਾਪਸ 0;

	ਜੇ (!remote->ਧੱਕੋ.nr) {
		ਵਸਤੂ = string_list_append(&ਹਾਲਤਾਂ->ਧੱਕੋ, _("(matching)"));
		info = ਵਸਤੂ->util = xcalloc(1, ਆਕਾਰ(ਬਣਤਰ ਧੱਕੋ_info));
		info->ਸਥਿਤੀ = ਧੱਕੋ_ਸਥਿਤੀ_NOTQUERIED;
		info->dest = xstrdup(ਵਸਤੂ->string);
	}
	ਲਈ (i = 0; i < remote->ਧੱਕੋ.nr; i++) {
		ਅਚਲ ਬਣਤਰ refspec_ਵਸਤੂ *spec = &remote->ਧੱਕੋ.ਚੀਜ਼ਾਂ[i];
		ਜੇ (spec->matching)
			ਵਸਤੂ = string_list_append(&ਹਾਲਤਾਂ->ਧੱਕੋ, _("(matching)"));
		ਨਹੀਂਤਾਂ ਜੇ (ਲੜੀ_ਲੰਬਾਈ(spec->src))
			ਵਸਤੂ = string_list_append(&ਹਾਲਤਾਂ->ਧੱਕੋ, spec->src);
		ਨਹੀਂਤਾਂ
			ਵਸਤੂ = string_list_append(&ਹਾਲਤਾਂ->ਧੱਕੋ, _("(delete)"));

		info = ਵਸਤੂ->util = xcalloc(1, ਆਕਾਰ(ਬਣਤਰ ਧੱਕੋ_info));
		info->forced = spec->force;
		info->ਸਥਿਤੀ = ਧੱਕੋ_ਸਥਿਤੀ_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : ਵਸਤੂ->string);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_ਸਿਰ_ਨਾਂ(ਅਚਲ ਬਣਤਰ ref *remote_refs, ਬਣਤਰ ref_ਹਾਲਤਾਂ *ਹਾਲਤਾਂ)
{
	ਬਣਤਰ ref *ref, *matches;
	ਬਣਤਰ ref *fetch_map = ਨਿਰਾ, **fetch_map_ਪੂਛ = &fetch_map;
	ਬਣਤਰ refspec_ਵਸਤੂ refspec = {
		.force = 0,
		.ਨਮੂਨਾ = 1,
		.src = (ਅੱਖਰ *) "refs/heads/*",
		.dst = (ਅੱਖਰ *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_ਪੂਛ, 0);
	matches = guess_remote_ਸਿਰ(ਲੱਭੋ_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_ਸਿਰ_ALL);
	ਲਈ (ref = matches; ref; ref = ref->ਅਗਲਾ)
		string_list_append(&ਹਾਲਤਾਂ->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	ਵਾਪਸ 0;
}

ਬਣਤਰ ਗਿਆਤ_remote {
	ਬਣਤਰ ਗਿਆਤ_remote *ਅਗਲਾ;
	ਬਣਤਰ remote *remote;
};

ਬਣਤਰ ਗਿਆਤ_remotes {
	ਬਣਤਰ remote *ਮੰਜ਼ਿਲ_delete;
	ਬਣਤਰ ਗਿਆਤ_remote *list;
};

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_ਗਿਆਤ_remote(ਬਣਤਰ remote *remote, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ ਗਿਆਤ_remotes *all = cb_ਅੰਕੜੇ;
	ਬਣਤਰ ਗਿਆਤ_remote *r;

	ਜੇ (!strcmp(all->ਮੰਜ਼ਿਲ_delete->name, remote->name))
		ਵਾਪਸ 0;

	r = xmalloc(ਆਕਾਰ(*r));
	r->remote = remote;
	r->ਅਗਲਾ = all->list;
	all->list = r;
	ਵਾਪਸ 0;
}

ਬਣਤਰ branches_for_remote {
	ਬਣਤਰ remote *remote;
	ਬਣਤਰ string_list *branches, *skipped;
	ਬਣਤਰ ਗਿਆਤ_remotes *keep;
};

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_branch_for_removal(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ branches_for_remote *branches = cb_ਅੰਕੜੇ;
	ਬਣਤਰ refspec_ਵਸਤੂ refspec;
	ਬਣਤਰ ਗਿਆਤ_remote *kr;

	memset(&refspec, 0, ਆਕਾਰ(refspec));
	refspec.dst = (ਅੱਖਰ *)ref->name;
	ਜੇ (remote_ਲੱਭੋ_tracking(branches->remote, &refspec))
		ਵਾਪਸ 0;
	ਮੁਕਤ(refspec.src);

	/* don't delete a branch if another remote also uses it */
	ਲਈ (kr = branches->keep->list; kr; kr = kr->ਅਗਲਾ) {
		memset(&refspec, 0, ਆਕਾਰ(refspec));
		refspec.dst = (ਅੱਖਰ *)ref->name;
		ਜੇ (!remote_ਲੱਭੋ_tracking(kr->remote, &refspec)) {
			ਮੁਕਤ(refspec.src);
			ਵਾਪਸ 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	ਜੇ (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		ਜੇ (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		ਵਾਪਸ 0;
	}

	string_list_append(branches->branches, ref->name);

	ਵਾਪਸ 0;
}

ਬਣਤਰ rename_info {
	ਅਚਲ ਅੱਖਰ *old_name;
	ਅਚਲ ਅੱਖਰ *new_name;
	ਬਣਤਰ ref_transaction *transaction;
	ਬਣਤਰ ਤਰੱਕੀ *ਤਰੱਕੀ;
	ਬਣਤਰ strbuf *err;
	uint32_t ਤਰੱਕੀ_nr;
	uint64_t ਸੂਚਕਾਂਕ;
};

ਸਥਿਰ ਖਾਲੀ compute_renamed_ref(ਬਣਤਰ rename_info *rename,
				ਅਚਲ ਅੱਖਰ *refname,
				ਬਣਤਰ strbuf *ਬਾਹਰਲਾ)
{
	strbuf_reset(ਬਾਹਰਲਾ);
	strbuf_addstr(ਬਾਹਰਲਾ, refname);
	strbuf_splice(ਬਾਹਰਲਾ, ਲੜੀ_ਲੰਬਾਈ("refs/remotes/"), ਲੜੀ_ਲੰਬਾਈ(rename->old_name),
		      rename->new_name, ਲੜੀ_ਲੰਬਾਈ(rename->new_name));
}

ਸਥਿਰ ਪੂਰਨ rename_one_reflog_ਇੰਦਰਾਜ(ਅਚਲ ਅੱਖਰ *old_refname,
				   ਬਣਤਰ object_id *old_oid,
				   ਬਣਤਰ object_id *new_oid,
				   ਅਚਲ ਅੱਖਰ *committer,
				   timestamp_t timestamp, ਪੂਰਨ tz,
				   ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ rename_info *rename = cb_ਅੰਕੜੇ;
	ਬਣਤਰ strbuf new_refname = STRBUF_INIT;
	ਬਣਤਰ strbuf ਪਛਾਣ_ਤੱਤ = STRBUF_INIT;
	ਬਣਤਰ strbuf name = STRBUF_INIT;
	ਬਣਤਰ strbuf mail = STRBUF_INIT;
	ਬਣਤਰ ident_ਵੰਡੋ ident;
	ਅਚਲ ਅੱਖਰ *date;
	ਪੂਰਨ error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	ਜੇ (ਵੰਡੋ_ident_ਸਤਰ(&ident, committer, ਲੜੀ_ਲੰਬਾਈ(committer)) < 0) {
		error = -1;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	strbuf_ਜੋੜੋ(&name, ident.name_begin, ident.name_ਸਿਰਾ - ident.name_begin);
	strbuf_ਜੋੜੋ(&mail, ident.mail_begin, ident.mail_ਸਿਰਾ - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_ਬਹੁਲਕ(ਸਾਧਾਰਨ));
	strbuf_addstr(&ਪਛਾਣ_ਤੱਤ, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, ਪਛਾਣ_ਤੱਤ.buf, ਸੁਨੇਹਾ,
					      rename->ਸੂਚਕਾਂਕ++, rename->err);

ਬਾਹਰਲਾ:
	strbuf_release(&new_refname);
	strbuf_release(&ਪਛਾਣ_ਤੱਤ);
	strbuf_release(&name);
	strbuf_release(&mail);
	ਵਾਪਸ error;
}

ਸਥਿਰ ਪੂਰਨ rename_one_reflog(ਅਚਲ ਅੱਖਰ *old_refname,
			     ਅਚਲ ਬਣਤਰ object_id *old_oid,
			     ਬਣਤਰ rename_info *rename)
{
	ਬਣਤਰ strbuf new_refname = STRBUF_INIT;
	ਬਣਤਰ strbuf ਸੁਨੇਹਾ_2 = STRBUF_INIT;
	ਪੂਰਨ error;

	ਜੇ (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		ਵਾਪਸ 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_ਇੰਦਰਾਜ, rename);
	ਜੇ (error < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	compute_renamed_ref(rename, old_refname, &new_refname);

	/*
	 * Manually write the reflog entry for the now-renamed ref. We cannot
	 * rely on `rename_one_ref()` to do this for us as that would screw
	 * over order in which reflog entries are being written.
	 *
	 * Furthermore, we only append the entry in case the reference
	 * resolves. Missing references shouldn't have reflogs anyway.
	 */
	strbuf_addf(&ਸੁਨੇਹਾ_2, "remote: renamed %s to %s", old_refname,
		    new_refname.buf);

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      old_oid, old_oid, git_committer_info(0),
					      ਸੁਨੇਹਾ_2.buf, rename->ਸੂਚਕਾਂਕ++, rename->err);
	ਜੇ (error < 0)
		ਵਾਪਸ error;

ਬਾਹਰਲਾ:
	strbuf_release(&new_refname);
	strbuf_release(&ਸੁਨੇਹਾ_2);
	ਵਾਪਸ error;
}

ਸਥਿਰ ਪੂਰਨ rename_one_ref(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ strbuf new_referent = STRBUF_INIT;
	ਬਣਤਰ strbuf new_refname = STRBUF_INIT;
	ਬਣਤਰ rename_info *rename = cb_ਅੰਕੜੇ;
	ਅਚਲ ਬਣਤਰ object_id *oid = ref->oid;
	ਅਚਲ ਅੱਖਰ *referent = ref->ਨਿਸ਼ਾਨਾ;
	ਪੂਰਨ error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	ਜੇ (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   ਨਿਰਾ, ਨਿਰਾ);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = ਨਿਰਾ;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, ਨਿਰਾ, rename->err);
	ਜੇ (error < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : ਨਿਰਾ, ਨਿਰਾ,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       ਨਿਰਾ, rename->err);
	ਜੇ (error < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	error = rename_one_reflog(ref->name, oid, rename);
	ਜੇ (error < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	display_ਤਰੱਕੀ(rename->ਤਰੱਕੀ, ++rename->ਤਰੱਕੀ_nr);

ਬਾਹਰਲਾ:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	ਵਾਪਸ error;
}

ਸਥਿਰ ਪੂਰਨ migrate_file(ਬਣਤਰ remote *remote)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	ਲਈ (ਮਾਪ_t i = 0; i < remote->url.nr; i++)
		repo_config_ਸੈੱਟ_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	ਲਈ (ਪੂਰਨ i = 0; i < remote->ਧੱਕੋ.nr; i++)
		repo_config_ਸੈੱਟ_multivar(the_repository, buf.buf, remote->ਧੱਕੋ.ਚੀਜ਼ਾਂ[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	ਲਈ (ਪੂਰਨ i = 0; i < remote->fetch.nr; i++)
		repo_config_ਸੈੱਟ_multivar(the_repository, buf.buf, remote->fetch.ਚੀਜ਼ਾਂ[i].raw, "^$", 0);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WITH_BREAKING_CHANGES
	ਜੇ (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_ਰਸਤਾ_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	ਨਹੀਂਤਾਂ ਜੇ (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_ਰਸਤਾ_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#ਜੇ_ਅੰਤ /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	ਵਾਪਸ 0;
}

ਬਣਤਰ ਧੱਕੋ_default_info
{
	ਅਚਲ ਅੱਖਰ *old_name;
	ਗਿਣਤੀ config_scope scope;
	ਬਣਤਰ strbuf origin;
	ਪੂਰਨ linenr;
};

ਸਥਿਰ ਪੂਰਨ config_read_ਧੱਕੋ_default(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
	ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਅਚਲ ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ_info *kvi = ctx->kvi;

	ਬਣਤਰ ਧੱਕੋ_default_info* info = cb;
	ਜੇ (strcmp(ਕੁੰਜੀ, "remote.pushdefault") ||
	    !ਮੁੱਲ || strcmp(ਮੁੱਲ, info->old_name))
		ਵਾਪਸ 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ handle_ਧੱਕੋ_default(ਅਚਲ ਅੱਖਰ* old_name, ਅਚਲ ਅੱਖਰ* new_name)
{
	ਬਣਤਰ ਧੱਕੋ_default_info ਧੱਕੋ_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_ਧੱਕੋ_default, &ਧੱਕੋ_default);
	ਜੇ (ਧੱਕੋ_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	ਨਹੀਂਤਾਂ ਜੇ (ਧੱਕੋ_default.scope >= CONFIG_SCOPE_LOCAL) {
		ਪੂਰਨ ਨਤੀਜਾ = repo_config_ਸੈੱਟ_gently(the_repository, "remote.pushDefault",
						    new_name);
		ਜੇ (new_name && ਨਤੀਜਾ && ਨਤੀਜਾ != CONFIG_NOTHING_ਸੈੱਟ)
			die(_("could not set '%s'"), "remote.pushDefault");
		ਨਹੀਂਤਾਂ ਜੇ (!new_name && ਨਤੀਜਾ && ਨਤੀਜਾ != CONFIG_NOTHING_ਸੈੱਟ)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} ਨਹੀਂਤਾਂ ਜੇ (ਧੱਕੋ_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(ਧੱਕੋ_default.scope),
			ਧੱਕੋ_default.origin.buf, ਧੱਕੋ_default.linenr,
			old_name);
	}

	strbuf_release(&ਧੱਕੋ_default.origin);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

ਸਥਿਰ ਪੂਰਨ mv(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ show_ਤਰੱਕੀ = isatty(2);
	ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "progress", &show_ਤਰੱਕੀ, N_("force progress reporting")),
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ remote *oldremote, *newremote;
	ਬਣਤਰ strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	ਬਣਤਰ rename_info rename = {
		.err = &err,
	};
	ਪੂਰਨ refspecs_need_update = 0;
	ਪੂਰਨ ਨਤੀਜਾ = 0;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_remote_rename_usage, 0);

	ਜੇ (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	ਜੇ (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		ਬਾਹਰ_ਨਿਕਲੋ(2);
	}

	ਜੇ (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		ਵਾਪਸ migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	ਜੇ (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		ਬਾਹਰ_ਨਿਕਲੋ(3);
	}

	ਜੇ (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	ਜੇ (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		ਨਤੀਜਾ = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	ਲਈ (ਪੂਰਨ i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!ਲੜੀ_ਖੋਜ(oldremote->fetch.ਚੀਜ਼ਾਂ[i].raw,
						old_remote_context.buf);

	ਜੇ (refspecs_need_update) {
		ਬਣਤਰ refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		ਜੇ (!rename.transaction)
			ਜਾਓ ਬਾਹਰਲਾ;

		ਜੇ (show_ਤਰੱਕੀ)
			rename.ਤਰੱਕੀ = ਸ਼ੁਰੂ_delayed_ਤਰੱਕੀ(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.ਅਗੇਤਰ = buf.buf;

		ਨਤੀਜਾ = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		ਜੇ (ਨਤੀਜਾ < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		ਨਤੀਜਾ = ref_transaction_prepare(rename.transaction, &err);
		ਜੇ (ਨਤੀਜਾ < 0) {
			error("renaming remote references failed: %s", err.buf);
			ਜੇ (ਨਤੀਜਾ == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(ਨਿਰਾ);
		}
	}

	ਜੇ (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_ਸੈੱਟ_multivar(the_repository, buf.buf, ਨਿਰਾ, ਨਿਰਾ, CONFIG_FLAGS_MULTI_REPLACE);
		ਲਈ (ਪੂਰਨ i = 0; i < oldremote->fetch.nr; i++) {
			ਅੱਖਰ *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.ਚੀਜ਼ਾਂ[i].raw);
			ptr = ਲੜੀ_ਖੋਜ(buf2.buf, old_remote_context.buf);
			ਜੇ (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + ਲੜੀ_ਲੰਬਾਈ(":refs/remotes/"),
					      ਲੜੀ_ਲੰਬਾਈ(rename.old_name), rename.new_name,
					      ਲੜੀ_ਲੰਬਾਈ(rename.new_name));
			} ਨਹੀਂਤਾਂ
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_ਸੈੱਟ_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	ਲਈ (ਮਾਪ_t i = 0; i < branch_list.nr; i++) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ = branch_list.ਚੀਜ਼ਾਂ + i;
		ਬਣਤਰ branch_info *info = ਵਸਤੂ->util;
		ਜੇ (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", ਵਸਤੂ->string);
			repo_config_ਸੈੱਟ(the_repository, buf.buf, rename.new_name);
		}
		ਜੇ (info->ਧੱਕੋ_remote_name && !strcmp(info->ਧੱਕੋ_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", ਵਸਤੂ->string);
			repo_config_ਸੈੱਟ(the_repository, buf.buf, rename.new_name);
		}
	}

	ਜੇ (refspecs_need_update) {
		ਨਤੀਜਾ = ref_transaction_commit(rename.transaction, &err);
		ਜੇ (ਨਤੀਜਾ < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_ਤਰੱਕੀ(&rename.ਤਰੱਕੀ);

		handle_ਧੱਕੋ_default(rename.old_name, rename.new_name);
	}

ਬਾਹਰਲਾ:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ rm(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ remote *remote;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ ਗਿਆਤ_remotes ਗਿਆਤ_remotes = { ਨਿਰਾ, ਨਿਰਾ };
	ਬਣਤਰ string_list branches = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list skipped = STRING_LIST_INIT_DUP;
	ਬਣਤਰ branches_for_remote cb_ਅੰਕੜੇ;
	ਪੂਰਨ ਨਤੀਜਾ;

	memset(&cb_ਅੰਕੜੇ, 0, ਆਕਾਰ(cb_ਅੰਕੜੇ));
	cb_ਅੰਕੜੇ.branches = &branches;
	cb_ਅੰਕੜੇ.skipped = &skipped;
	cb_ਅੰਕੜੇ.keep = &ਗਿਆਤ_remotes;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_remote_rm_usage, 0);
	ਜੇ (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	ਜੇ (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		ਬਾਹਰ_ਨਿਕਲੋ(2);
	}

	ਗਿਆਤ_remotes.ਮੰਜ਼ਿਲ_delete = remote;
	for_each_remote(ਜੋੜੋ_ਗਿਆਤ_remote, &ਗਿਆਤ_remotes);

	read_branches();
	ਲਈ (ਮਾਪ_t i = 0; i < branch_list.nr; i++) {
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ = branch_list.ਚੀਜ਼ਾਂ + i;
		ਬਣਤਰ branch_info *info = ਵਸਤੂ->util;
		ਜੇ (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀਆਂ[] = { "remote", "merge", ਨਿਰਾ }, **k;
			ਲਈ (k = ਕੁੰਜੀਆਂ; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						ਵਸਤੂ->string, *k);
				ਨਤੀਜਾ = repo_config_ਸੈੱਟ_gently(the_repository, buf.buf, ਨਿਰਾ);
				ਜੇ (ਨਤੀਜਾ && ਨਤੀਜਾ != CONFIG_NOTHING_ਸੈੱਟ)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		ਜੇ (info->ਧੱਕੋ_remote_name && !strcmp(info->ਧੱਕੋ_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", ਵਸਤੂ->string);
			ਨਤੀਜਾ = repo_config_ਸੈੱਟ_gently(the_repository, buf.buf, ਨਿਰਾ);
			ਜੇ (ਨਤੀਜਾ && ਨਤੀਜਾ != CONFIG_NOTHING_ਸੈੱਟ)
				die(_("could not unset '%s'"), buf.buf);
		}
	}

	/*
	 * We cannot just pass a function to for_each_ref() which deletes
	 * the branches one by one, since for_each_ref() relies on cached
	 * refs, which are invalidated when deleting a branch.
	 */
	cb_ਅੰਕੜੇ.remote = remote;
	ਨਤੀਜਾ = refs_for_each_ref(get_main_ref_store(the_repository),
				   ਜੋੜੋ_branch_for_removal, &cb_ਅੰਕੜੇ);
	strbuf_release(&buf);

	ਜੇ (!ਨਤੀਜਾ)
		ਨਤੀਜਾ = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	ਜੇ (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		ਲਈ (ਮਾਪ_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.ਚੀਜ਼ਾਂ[i].string);
	}
	string_list_clear(&skipped, 0);

	ਜੇ (!ਨਤੀਜਾ) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		ਜੇ (repo_config_rename_section(the_repository, buf.buf, ਨਿਰਾ) < 1) {
			ਨਤੀਜਾ = error(_("Could not remove config section '%s'"), buf.buf);
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		handle_ਧੱਕੋ_default(remote->name, ਨਿਰਾ);
	}

ਬਾਹਰਲਾ:
	ਲਈ (ਬਣਤਰ ਗਿਆਤ_remote *r = ਗਿਆਤ_remotes.list; r;) {
		ਬਣਤਰ ਗਿਆਤ_remote *ਅਗਲਾ = r->ਅਗਲਾ;
		ਮੁਕਤ(r);
		r = ਅਗਲਾ;
	}
	strbuf_release(&buf);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਖਾਲੀ clear_ਧੱਕੋ_info(ਖਾਲੀ *util, ਅਚਲ ਅੱਖਰ *string ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ ਧੱਕੋ_info *info = util;
	ਮੁਕਤ(info->dest);
	ਮੁਕਤ(info);
}

ਸਥਿਰ ਖਾਲੀ free_remote_ref_ਹਾਲਤਾਂ(ਬਣਤਰ ref_ਹਾਲਤਾਂ *ਹਾਲਤਾਂ)
{
	string_list_clear(&ਹਾਲਤਾਂ->new_refs, 0);
	string_list_clear(&ਹਾਲਤਾਂ->skipped, 0);
	string_list_clear(&ਹਾਲਤਾਂ->stale, 1);
	string_list_clear(&ਹਾਲਤਾਂ->tracked, 0);
	string_list_clear(&ਹਾਲਤਾਂ->heads, 0);
	string_list_clear_func(&ਹਾਲਤਾਂ->ਧੱਕੋ, clear_ਧੱਕੋ_info);
}

ਸਥਿਰ ਪੂਰਨ append_ref_ਮੰਜ਼ਿਲ_tracked_list(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ ref_ਹਾਲਤਾਂ *ਹਾਲਤਾਂ = cb_ਅੰਕੜੇ;
	ਬਣਤਰ refspec_ਵਸਤੂ refspec;

	ਜੇ (ref->flags & REF_ISSYMREF)
		ਵਾਪਸ 0;

	memset(&refspec, 0, ਆਕਾਰ(refspec));
	refspec.dst = (ਅੱਖਰ *)ref->name;
	ਜੇ (!remote_ਲੱਭੋ_tracking(ਹਾਲਤਾਂ->remote, &refspec)) {
		string_list_append(&ਹਾਲਤਾਂ->tracked, abbrev_branch(refspec.src));
		ਮੁਕਤ(refspec.src);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_remote_ref_ਹਾਲਤਾਂ(ਅਚਲ ਅੱਖਰ *name,
				 ਬਣਤਰ ref_ਹਾਲਤਾਂ *ਹਾਲਤਾਂ,
				 ਪੂਰਨ query)
{
	ਹਾਲਤਾਂ->remote = remote_get(name);
	ਜੇ (!ਹਾਲਤਾਂ->remote)
		ਵਾਪਸ error(_("No such remote: '%s'"), name);

	read_branches();

	ਜੇ (query) {
		ਬਣਤਰ transport *transport;
		ਅਚਲ ਬਣਤਰ ref *remote_refs;

		transport = transport_get(ਹਾਲਤਾਂ->remote, ਹਾਲਤਾਂ->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, ਨਿਰਾ);

		ਹਾਲਤਾਂ->queried = 1;
		ਜੇ (query & GET_REF_ਹਾਲਤਾਂ)
			get_ref_ਹਾਲਤਾਂ(remote_refs, ਹਾਲਤਾਂ);
		ਜੇ (query & GET_ਸਿਰ_ਨਾਂ)
			get_ਸਿਰ_ਨਾਂ(remote_refs, ਹਾਲਤਾਂ);
		ਜੇ (query & GET_ਧੱਕੋ_REF_ਹਾਲਤਾਂ)
			get_ਧੱਕੋ_ref_ਹਾਲਤਾਂ(remote_refs, ਹਾਲਤਾਂ);
		transport_disconnect(transport);
	} ਨਹੀਂਤਾਂ {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_ਮੰਜ਼ਿਲ_tracked_list, ਹਾਲਤਾਂ);
		string_list_ਛਾਂਟੋ(&ਹਾਲਤਾਂ->tracked);
		get_ਧੱਕੋ_ref_ਹਾਲਤਾਂ_noquery(ਹਾਲਤਾਂ);
	}

	ਵਾਪਸ 0;
}

ਬਣਤਰ show_info {
	ਬਣਤਰ string_list list;
	ਬਣਤਰ ref_ਹਾਲਤਾਂ ਹਾਲਤਾਂ;
	ਪੂਰਨ ਚੌੜਾਈ, width2;
	ਪੂਰਨ any_rebase;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.ਹਾਲਤਾਂ = REF_ਹਾਲਤਾਂ_INIT, \
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_remote_ਮੰਜ਼ਿਲ_show_info(ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ show_info *info = cb_ਅੰਕੜੇ;
	ਪੂਰਨ n = ਲੜੀ_ਲੰਬਾਈ(ਵਸਤੂ->string);
	ਜੇ (n > info->ਚੌੜਾਈ)
		info->ਚੌੜਾਈ = n;
	string_list_ਪਾਓ(&info->list, ਵਸਤੂ->string);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ show_remote_info_ਵਸਤੂ(ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ show_info *info = cb_ਅੰਕੜੇ;
	ਬਣਤਰ ref_ਹਾਲਤਾਂ *ਹਾਲਤਾਂ = &info->ਹਾਲਤਾਂ;
	ਅਚਲ ਅੱਖਰ *name = ਵਸਤੂ->string;

	ਜੇ (ਹਾਲਤਾਂ->queried) {
		ਅਚਲ ਅੱਖਰ *fmt = "%s";
		ਅਚਲ ਅੱਖਰ *arg = "";
		ਜੇ (string_list_has_string(&ਹਾਲਤਾਂ->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = ਹਾਲਤਾਂ->remote->name;
		} ਨਹੀਂਤਾਂ ਜੇ (string_list_has_string(&ਹਾਲਤਾਂ->tracked, name))
			arg = _(" tracked");
		ਨਹੀਂਤਾਂ ਜੇ (string_list_has_string(&ਹਾਲਤਾਂ->skipped, name))
			arg = _(" skipped");
		ਨਹੀਂਤਾਂ ਜੇ (string_list_has_string(&ਹਾਲਤਾਂ->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		ਨਹੀਂਤਾਂ
			arg = _(" ???");
		ਲਿਖੋ("    %-*s", info->ਚੌੜਾਈ, name);
		ਲਿਖੋ(fmt, arg);
		ਲਿਖੋ("\n");
	} ਨਹੀਂਤਾਂ
		ਲਿਖੋ("    %s\n", name);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_local_ਮੰਜ਼ਿਲ_show_info(ਬਣਤਰ string_list_ਵਸਤੂ *branch_ਵਸਤੂ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ show_info *show_info = cb_ਅੰਕੜੇ;
	ਬਣਤਰ ref_ਹਾਲਤਾਂ *ਹਾਲਤਾਂ = &show_info->ਹਾਲਤਾਂ;
	ਬਣਤਰ branch_info *branch_info = branch_ਵਸਤੂ->util;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਪੂਰਨ n;

	ਜੇ (!branch_info->ਮਿਲਾਓ.nr || !branch_info->remote_name ||
	    strcmp(ਹਾਲਤਾਂ->remote->name, branch_info->remote_name))
		ਵਾਪਸ 0;
	ਜੇ ((n = ਲੜੀ_ਲੰਬਾਈ(branch_ਵਸਤੂ->string)) > show_info->ਚੌੜਾਈ)
		show_info->ਚੌੜਾਈ = n;
	ਜੇ (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	ਵਸਤੂ = string_list_ਪਾਓ(&show_info->list, branch_ਵਸਤੂ->string);
	ਵਸਤੂ->util = branch_info;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ show_local_info_ਵਸਤੂ(ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ show_info *show_info = cb_ਅੰਕੜੇ;
	ਬਣਤਰ branch_info *branch_info = ਵਸਤੂ->util;
	ਬਣਤਰ string_list *ਮਿਲਾਓ = &branch_info->ਮਿਲਾਓ;
	ਪੂਰਨ ਚੌੜਾਈ = show_info->ਚੌੜਾਈ + 4;

	ਜੇ (branch_info->rebase >= REBASE_TRUE && branch_info->ਮਿਲਾਓ.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			ਵਸਤੂ->string);
		ਵਾਪਸ 0;
	}

	ਲਿਖੋ("    %-*s ", show_info->ਚੌੜਾਈ, ਵਸਤੂ->string);
	ਜੇ (branch_info->rebase >= REBASE_TRUE) {
		ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ;
		ਜੇ (branch_info->rebase == REBASE_INTERACTIVE)
			ਸੁਨੇਹਾ = _("rebases interactively onto remote %s");
		ਨਹੀਂਤਾਂ ਜੇ (branch_info->rebase == REBASE_MERGES)
			ਸੁਨੇਹਾ = _("rebases interactively (with merges) onto "
				"remote %s");
		ਨਹੀਂਤਾਂ
			ਸੁਨੇਹਾ = _("rebases onto remote %s");
		printf_ln(ਸੁਨੇਹਾ, ਮਿਲਾਓ->ਚੀਜ਼ਾਂ[0].string);
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), ਮਿਲਾਓ->ਚੀਜ਼ਾਂ[0].string);
		ਚੌੜਾਈ++;
	} ਨਹੀਂਤਾਂ {
		printf_ln(_("merges with remote %s"), ਮਿਲਾਓ->ਚੀਜ਼ਾਂ[0].string);
	}
	ਲਈ (ਮਾਪ_t i = 1; i < ਮਿਲਾਓ->nr; i++)
		ਲਿਖੋ(_("%-*s    and with remote %s\n"), ਚੌੜਾਈ, "",
		       ਮਿਲਾਓ->ਚੀਜ਼ਾਂ[i].string);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_ਧੱਕੋ_ਮੰਜ਼ਿਲ_show_info(ਬਣਤਰ string_list_ਵਸਤੂ *ਧੱਕੋ_ਵਸਤੂ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ show_info *show_info = cb_ਅੰਕੜੇ;
	ਬਣਤਰ ਧੱਕੋ_info *ਧੱਕੋ_info = ਧੱਕੋ_ਵਸਤੂ->util;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਪੂਰਨ n;
	ਜੇ ((n = ਲੜੀ_ਲੰਬਾਈ(ਧੱਕੋ_ਵਸਤੂ->string)) > show_info->ਚੌੜਾਈ)
		show_info->ਚੌੜਾਈ = n;
	ਜੇ ((n = ਲੜੀ_ਲੰਬਾਈ(ਧੱਕੋ_info->dest)) > show_info->width2)
		show_info->width2 = n;
	ਵਸਤੂ = string_list_append(&show_info->list, ਧੱਕੋ_ਵਸਤੂ->string);
	ਵਸਤੂ->util = ਧੱਕੋ_ਵਸਤੂ->util;
	ਵਾਪਸ 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
ਸਥਿਰ ਪੂਰਨ cmp_string_with_ਧੱਕੋ(ਅਚਲ ਖਾਲੀ *va, ਅਚਲ ਖਾਲੀ *vb)
{
	ਅਚਲ ਬਣਤਰ string_list_ਵਸਤੂ *a = va;
	ਅਚਲ ਬਣਤਰ string_list_ਵਸਤੂ *b = vb;
	ਅਚਲ ਬਣਤਰ ਧੱਕੋ_info *a_ਧੱਕੋ = a->util;
	ਅਚਲ ਬਣਤਰ ਧੱਕੋ_info *b_ਧੱਕੋ = b->util;
	ਪੂਰਨ cmp = strcmp(a->string, b->string);
	ਵਾਪਸ cmp ? cmp : strcmp(a_ਧੱਕੋ->dest, b_ਧੱਕੋ->dest);
}

ਸਥਿਰ ਪੂਰਨ show_ਧੱਕੋ_info_ਵਸਤੂ(ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ show_info *show_info = cb_ਅੰਕੜੇ;
	ਬਣਤਰ ਧੱਕੋ_info *ਧੱਕੋ_info = ਵਸਤੂ->util;
	ਅਚਲ ਅੱਖਰ *src = ਵਸਤੂ->string, *ਸਥਿਤੀ = ਨਿਰਾ;

	ਚੋਣ (ਧੱਕੋ_info->ਸਥਿਤੀ) {
	ਹਾਲਤ ਧੱਕੋ_ਸਥਿਤੀ_CREATE:
		ਸਥਿਤੀ = _("create");
		ਰੋਕੋ;
	ਹਾਲਤ ਧੱਕੋ_ਸਥਿਤੀ_DELETE:
		ਸਥਿਤੀ = _("delete");
		src = _("(none)");
		ਰੋਕੋ;
	ਹਾਲਤ ਧੱਕੋ_ਸਥਿਤੀ_UPTODATE:
		ਸਥਿਤੀ = _("up to date");
		ਰੋਕੋ;
	ਹਾਲਤ ਧੱਕੋ_ਸਥਿਤੀ_FASTFORWARD:
		ਸਥਿਤੀ = _("fast-forwardable");
		ਰੋਕੋ;
	ਹਾਲਤ ਧੱਕੋ_ਸਥਿਤੀ_OUTOFDATE:
		ਸਥਿਤੀ = _("local out of date");
		ਰੋਕੋ;
	ਹਾਲਤ ਧੱਕੋ_ਸਥਿਤੀ_NOTQUERIED:
		ਰੋਕੋ;
	}
	ਜੇ (ਸਥਿਤੀ) {
		ਜੇ (ਧੱਕੋ_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->ਚੌੜਾਈ, src,
			       show_info->width2, ਧੱਕੋ_info->dest, ਸਥਿਤੀ);
		ਨਹੀਂਤਾਂ
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->ਚੌੜਾਈ, src,
			       show_info->width2, ਧੱਕੋ_info->dest, ਸਥਿਤੀ);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (ਧੱਕੋ_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->ਚੌੜਾਈ, src,
			       ਧੱਕੋ_info->dest);
		ਨਹੀਂਤਾਂ
			printf_ln(_("    %-*s pushes to %s"), show_info->ਚੌੜਾਈ, src,
			       ਧੱਕੋ_info->dest);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_one_ਇੰਦਰਾਜ(ਬਣਤਰ remote *remote, ਖਾਲੀ *priv)
{
	ਬਣਤਰ string_list *list = priv;
	ਬਣਤਰ strbuf remote_info_buf = STRBUF_INIT;
	ਬਣਤਰ strvec *url;

	ਜੇ (remote->url.nr > 0) {
		ਬਣਤਰ strbuf promisor_config = STRBUF_INIT;
		ਅਚਲ ਅੱਖਰ *partial_clone_filter = ਨਿਰਾ;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		ਜੇ (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ਨਿਰਾ);
	} ਨਹੀਂਤਾਂ
		string_list_append(list, remote->name)->util = ਨਿਰਾ;
	url = ਧੱਕੋ_url_ਦਾ_remote(remote);
	ਲਈ (ਮਾਪ_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ਨਿਰਾ);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ show_all(ਖਾਲੀ)
{
	ਬਣਤਰ string_list list = STRING_LIST_INIT_DUP;
	ਪੂਰਨ ਨਤੀਜਾ;

	ਨਤੀਜਾ = for_each_remote(get_one_ਇੰਦਰਾਜ, &list);

	ਜੇ (!ਨਤੀਜਾ) {
		string_list_ਛਾਂਟੋ(&list);
		ਲਈ (ਮਾਪ_t i = 0; i < list.nr; i++) {
			ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ = list.ਚੀਜ਼ਾਂ + i;
			ਜੇ (verbose)
				ਲਿਖੋ("%s\t%s\n", ਵਸਤੂ->string,
					ਵਸਤੂ->util ? (ਅਚਲ ਅੱਖਰ *)ਵਸਤੂ->util : "");
			ਨਹੀਂਤਾਂ {
				ਜੇ (i && !strcmp((ਵਸਤੂ - 1)->string, ਵਸਤੂ->string))
					ਜਾਰੀ;
				ਲਿਖੋ("%s\n", ਵਸਤੂ->string);
			}
		}
	}
	string_list_clear(&list, 1);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ show(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ no_query = 0, ਨਤੀਜਾ = 0, query_flag = 0;
	ਬਣਤਰ option options[] = {
		OPT_BOOL('n', ਨਿਰਾ, &no_query, N_("do not query remotes")),
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_remote_show_usage,
			     0);

	ਜੇ (argc < 1)
		ਵਾਪਸ show_all();

	ਜੇ (!no_query)
		query_flag = (GET_REF_ਹਾਲਤਾਂ | GET_ਸਿਰ_ਨਾਂ | GET_ਧੱਕੋ_REF_ਹਾਲਤਾਂ);

	ਲਈ (; argc; argc--, argv++) {
		ਮਾਪ_t i;
		ਬਣਤਰ strvec *url;

		get_remote_ref_ਹਾਲਤਾਂ(*argv, &info.ਹਾਲਤਾਂ, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.ਹਾਲਤਾਂ.remote->url.v[0]);
		url = ਧੱਕੋ_url_ਦਾ_remote(info.ਹਾਲਤਾਂ.remote);
		ਲਈ (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		ਜੇ (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		ਜੇ (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		ਨਹੀਂਤਾਂ ਜੇ (!info.ਹਾਲਤਾਂ.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		ਨਹੀਂਤਾਂ ਜੇ (info.ਹਾਲਤਾਂ.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.ਹਾਲਤਾਂ.heads.ਚੀਜ਼ਾਂ[0].string);
		ਨਹੀਂਤਾਂ {
			ਲਿਖੋ(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			ਲਈ (i = 0; i < info.ਹਾਲਤਾਂ.heads.nr; i++)
				ਲਿਖੋ("    %s\n", info.ਹਾਲਤਾਂ.heads.ਚੀਜ਼ਾਂ[i].string);
		}

		/* remote branch info */
		info.ਚੌੜਾਈ = 0;
		for_each_string_list(&info.ਹਾਲਤਾਂ.new_refs, ਜੋੜੋ_remote_ਮੰਜ਼ਿਲ_show_info, &info);
		for_each_string_list(&info.ਹਾਲਤਾਂ.skipped, ਜੋੜੋ_remote_ਮੰਜ਼ਿਲ_show_info, &info);
		for_each_string_list(&info.ਹਾਲਤਾਂ.tracked, ਜੋੜੋ_remote_ਮੰਜ਼ਿਲ_show_info, &info);
		for_each_string_list(&info.ਹਾਲਤਾਂ.stale, ਜੋੜੋ_remote_ਮੰਜ਼ਿਲ_show_info, &info);
		ਜੇ (info.list.nr)
			printf_ln(Q_("  Remote branch:%s",
				     "  Remote branches:%s",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_remote_info_ਵਸਤੂ, &info);
		string_list_clear(&info.list, 0);

		/* git pull info */
		info.ਚੌੜਾਈ = 0;
		info.any_rebase = 0;
		for_each_string_list(&branch_list, ਜੋੜੋ_local_ਮੰਜ਼ਿਲ_show_info, &info);
		ਜੇ (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_ਵਸਤੂ, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		ਜੇ (info.ਹਾਲਤਾਂ.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.ਚੌੜਾਈ = info.width2 = 0;
		for_each_string_list(&info.ਹਾਲਤਾਂ.ਧੱਕੋ, ਜੋੜੋ_ਧੱਕੋ_ਮੰਜ਼ਿਲ_show_info, &info);
		QSORT(info.list.ਚੀਜ਼ਾਂ, info.list.nr, cmp_string_with_ਧੱਕੋ);
		ਜੇ (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_ਧੱਕੋ_info_ਵਸਤੂ, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_ਹਾਲਤਾਂ(&info.ਹਾਲਤਾਂ);
	}

	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਖਾਲੀ report_ਸੈੱਟ_ਸਿਰ_auto(ਅਚਲ ਅੱਖਰ *remote, ਅਚਲ ਅੱਖਰ *ਸਿਰ_name,
			ਬਣਤਰ strbuf *b_local_ਸਿਰ, ਪੂਰਨ was_detached) {
	ਬਣਤਰ strbuf buf_ਅਗੇਤਰ = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *prev_ਸਿਰ = ਨਿਰਾ;

	strbuf_addf(&buf_ਅਗੇਤਰ, "refs/remotes/%s/", remote);
	skip_ਅਗੇਤਰ(b_local_ਸਿਰ->buf, buf_ਅਗੇਤਰ.buf, &prev_ਸਿਰ);

	ਜੇ (prev_ਸਿਰ && !strcmp(prev_ਸਿਰ, ਸਿਰ_name))
		ਲਿਖੋ(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, ਸਿਰ_name);
	ਨਹੀਂਤਾਂ ਜੇ (prev_ਸਿਰ)
		ਲਿਖੋ(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_ਸਿਰ, ਸਿਰ_name);
	ਨਹੀਂਤਾਂ ਜੇ (!b_local_ਸਿਰ->len)
		ਲਿਖੋ(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, ਸਿਰ_name);
	ਨਹੀਂਤਾਂ ਜੇ (was_detached && b_local_ਸਿਰ->len)
		ਲਿਖੋ(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_ਸਿਰ->buf, ਸਿਰ_name);
	ਨਹੀਂਤਾਂ
		ਲਿਖੋ(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_ਸਿਰ->buf, ਸਿਰ_name);
	strbuf_release(&buf_ਅਗੇਤਰ);
}

ਸਥਿਰ ਪੂਰਨ ਸੈੱਟ_ਸਿਰ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ opt_a = 0, opt_d = 0, ਨਤੀਜਾ = 0, was_detached;
	ਬਣਤਰ strbuf b_ਸਿਰ = STRBUF_INIT, b_remote_ਸਿਰ = STRBUF_INIT,
		b_local_ਸਿਰ = STRBUF_INIT;
	ਅੱਖਰ *ਸਿਰ_name = ਨਿਰਾ;
	ਬਣਤਰ ref_store *refs = get_main_ref_store(the_repository);
	ਬਣਤਰ remote *remote;

	ਬਣਤਰ option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_ਸਿਰਾ()
	};
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	ਜੇ (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_ਸਿਰ, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	ਜੇ (!opt_a && !opt_d && argc == 2) {
		ਸਿਰ_name = xstrdup(argv[1]);
	} ਨਹੀਂਤਾਂ ਜੇ (opt_a && !opt_d && argc == 1) {
		ਬਣਤਰ ref_ਹਾਲਤਾਂ ਹਾਲਤਾਂ = REF_ਹਾਲਤਾਂ_INIT;
		get_remote_ref_ਹਾਲਤਾਂ(argv[0], &ਹਾਲਤਾਂ, GET_ਸਿਰ_ਨਾਂ);
		ਜੇ (!ਹਾਲਤਾਂ.heads.nr)
			ਨਤੀਜਾ |= error(_("Cannot determine remote HEAD"));
		ਨਹੀਂਤਾਂ ਜੇ (ਹਾਲਤਾਂ.heads.nr > 1) {
			ਨਤੀਜਾ |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			ਲਈ (ਮਾਪ_t i = 0; i < ਹਾਲਤਾਂ.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], ਹਾਲਤਾਂ.heads.ਚੀਜ਼ਾਂ[i].string);
		} ਨਹੀਂਤਾਂ
			ਸਿਰ_name = xstrdup(ਹਾਲਤਾਂ.heads.ਚੀਜ਼ਾਂ[0].string);
		free_remote_ref_ਹਾਲਤਾਂ(&ਹਾਲਤਾਂ);
	} ਨਹੀਂਤਾਂ ਜੇ (opt_d && !opt_a && argc == 1) {
		ਜੇ (refs_delete_ref(refs, ਨਿਰਾ, b_ਸਿਰ.buf, ਨਿਰਾ, REF_NO_DEREF))
			ਨਤੀਜਾ |= error(_("Could not delete %s"), b_ਸਿਰ.buf);
	} ਨਹੀਂਤਾਂ
		usage_with_options(builtin_remote_sethead_usage, options);

	ਜੇ (!ਸਿਰ_name)
		ਜਾਓ cleanup;
	strbuf_addf(&b_remote_ਸਿਰ, "refs/remotes/%s/%s", argv[0], ਸਿਰ_name);
	ਜੇ (!refs_ref_exists(refs, b_remote_ਸਿਰ.buf)) {
		ਨਤੀਜਾ |= error(_("Not a valid ref: %s"), b_remote_ਸਿਰ.buf);
		ਜਾਓ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_ਸਿਰ.buf, b_remote_ਸਿਰ.buf,
			"remote set-head", &b_local_ਸਿਰ, 0);
	ਜੇ (was_detached == -1) {
		ਨਤੀਜਾ |= error(_("Could not set up %s"), b_ਸਿਰ.buf);
		ਜਾਓ cleanup;
	}
	ਜੇ (opt_a)
		report_ਸੈੱਟ_ਸਿਰ_auto(argv[0], ਸਿਰ_name, &b_local_ਸਿਰ, was_detached);
	ਜੇ (remote->follow_remote_ਸਿਰ == FOLLOW_REMOTE_ALWAYS) {
		ਬਣਤਰ strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_ਸੈੱਟ(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	ਮੁਕਤ(ਸਿਰ_name);
	strbuf_release(&b_ਸਿਰ);
	strbuf_release(&b_remote_ਸਿਰ);
	strbuf_release(&b_local_ਸਿਰ);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ prune_remote(ਅਚਲ ਅੱਖਰ *remote, ਪੂਰਨ dry_ਲੜੀਵਾਰ)
{
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਬਣਤਰ ref_ਹਾਲਤਾਂ ਹਾਲਤਾਂ = REF_ਹਾਲਤਾਂ_INIT;
	ਬਣਤਰ string_list refs_ਮੰਜ਼ਿਲ_prune = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

	get_remote_ref_ਹਾਲਤਾਂ(remote, &ਹਾਲਤਾਂ, GET_REF_ਹਾਲਤਾਂ);

	ਜੇ (!ਹਾਲਤਾਂ.stale.nr) {
		free_remote_ref_ਹਾਲਤਾਂ(&ਹਾਲਤਾਂ);
		ਵਾਪਸ 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), ਹਾਲਤਾਂ.remote->url.v[0]);

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਹਾਲਤਾਂ.stale)
		string_list_append(&refs_ਮੰਜ਼ਿਲ_prune, ਵਸਤੂ->util);
	string_list_ਛਾਂਟੋ(&refs_ਮੰਜ਼ਿਲ_prune);

	ਜੇ (!dry_ਲੜੀਵਾਰ)
		ਨਤੀਜਾ |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_ਮੰਜ਼ਿਲ_prune, 0);

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &ਹਾਲਤਾਂ.stale) {
		ਅਚਲ ਅੱਖਰ *refname = ਵਸਤੂ->util;

		ਜੇ (dry_ਲੜੀਵਾਰ)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		ਨਹੀਂਤਾਂ
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_ਲੜੀਵਾਰ, &refs_ਮੰਜ਼ਿਲ_prune);

	string_list_clear(&refs_ਮੰਜ਼ਿਲ_prune, 0);
	free_remote_ref_ਹਾਲਤਾਂ(&ਹਾਲਤਾਂ);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ prune(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ dry_ਲੜੀਵਾਰ = 0, ਨਤੀਜਾ = 0;
	ਬਣਤਰ option options[] = {
		OPT_DRY_ਲੜੀਵਾਰ(&dry_ਲੜੀਵਾਰ, N_("dry run")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_remote_prune_usage, 0);

	ਜੇ (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	ਲਈ (; argc; argc--, argv++)
		ਨਤੀਜਾ |= prune_remote(*argv, dry_ਲੜੀਵਾਰ);

	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ get_remote_default(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ ਅਣਵਰਤਿਆ,
			      ਅਚਲ ਬਣਤਰ config_context *ctx ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ *priv)
{
	ਜੇ (strcmp(ਕੁੰਜੀ, "remotes.default") == 0) {
		ਪੂਰਨ *ਮਿਲਿਆ_2 = priv;
		*ਮਿਲਿਆ_2 = 1;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ update(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i, prune = -1;
	ਬਣਤਰ option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;
	ਪੂਰਨ default_defined = 0;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "fetch");

	ਜੇ (prune != -1)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, prune ? "--prune" : "--no-prune");
	ਜੇ (verbose)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "-v");
	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--multiple");
	ਜੇ (argc < 2)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "default");
	ਲਈ (i = 1; i < argc; i++)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, argv[i]);

	ਜੇ (strcmp(cmd.ਦਲੀਲਾਂ.v[cmd.ਦਲੀਲਾਂ.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		ਜੇ (!default_defined) {
			strvec_ਕੱਢੋ(&cmd.ਦਲੀਲਾਂ);
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--all");
		}
	}

	cmd.git_cmd = 1;
	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cmd);
}

ਸਥਿਰ ਪੂਰਨ remove_all_fetch_refspecs(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ)
{
	ਵਾਪਸ repo_config_ਸੈੱਟ_multivar_gently(the_repository, ਕੁੰਜੀ, ਨਿਰਾ, ਨਿਰਾ,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_branches(ਬਣਤਰ remote *remote, ਅਚਲ ਅੱਖਰ **branches,
			 ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ)
{
	ਅਚਲ ਅੱਖਰ *remotename = remote->name;
	ਪੂਰਨ mirror = remote->mirror;
	ਬਣਤਰ strbuf refspec = STRBUF_INIT;

	ਲਈ (; *branches; branches++)
		ਜੋੜੋ_branch(ਕੁੰਜੀ, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

ਸਥਿਰ ਪੂਰਨ ਸੈੱਟ_remote_branches(ਅਚਲ ਅੱਖਰ *remotename, ਅਚਲ ਅੱਖਰ **branches,
				ਪੂਰਨ ਜੋੜੋ_ਬਹੁਲਕ)
{
	ਬਣਤਰ strbuf ਕੁੰਜੀ = STRBUF_INIT;
	ਬਣਤਰ remote *remote;

	strbuf_addf(&ਕੁੰਜੀ, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	ਜੇ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		ਬਾਹਰ_ਨਿਕਲੋ(2);
	}

	ਜੇ (!ਜੋੜੋ_ਬਹੁਲਕ && remove_all_fetch_refspecs(ਕੁੰਜੀ.buf)) {
		strbuf_release(&ਕੁੰਜੀ);
		ਵਾਪਸ 1;
	}
	ਜੋੜੋ_branches(remote, branches, ਕੁੰਜੀ.buf);

	strbuf_release(&ਕੁੰਜੀ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਸੈੱਟ_branches(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ਜੋੜੋ_ਬਹੁਲਕ = 0;
	ਬਣਤਰ option options[] = {
		OPT_BOOL('\0', "add", &ਜੋੜੋ_ਬਹੁਲਕ, N_("add branch")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_remote_setbranches_usage, 0);
	ਜੇ (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = ਨਿਰਾ;

	ਵਾਪਸ ਸੈੱਟ_remote_branches(argv[0], argv + 1, ਜੋੜੋ_ਬਹੁਲਕ);
}

ਸਥਿਰ ਪੂਰਨ get_url(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ਧੱਕੋ_ਬਹੁਲਕ = 0, all_ਬਹੁਲਕ = 0;
	ਅਚਲ ਅੱਖਰ *remotename = ਨਿਰਾ;
	ਬਣਤਰ remote *remote;
	ਬਣਤਰ strvec *url;
	ਬਣਤਰ option options[] = {
		OPT_BOOL('\0', "push", &ਧੱਕੋ_ਬਹੁਲਕ,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_ਬਹੁਲਕ,
			 N_("return all URLs")),
		OPT_ਸਿਰਾ()
	};
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_remote_geturl_usage, 0);

	ਜੇ (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	ਜੇ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		ਬਾਹਰ_ਨਿਕਲੋ(2);
	}

	url = ਧੱਕੋ_ਬਹੁਲਕ ? ਧੱਕੋ_url_ਦਾ_remote(remote) : &remote->url;

	ਜੇ (all_ਬਹੁਲਕ) {
		ਲਈ (ਮਾਪ_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} ਨਹੀਂਤਾਂ {
		printf_ln("%s", url->v[0]);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਸੈੱਟ_url(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ਧੱਕੋ_ਬਹੁਲਕ = 0, ਜੋੜੋ_ਬਹੁਲਕ = 0, delete_ਬਹੁਲਕ = 0;
	ਪੂਰਨ matches = 0, negative_matches = 0;
	ਅਚਲ ਅੱਖਰ *remotename = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *newurl = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *oldurl = ਨਿਰਾ;
	ਬਣਤਰ remote *remote;
	regex_t old_regex;
	ਬਣਤਰ strvec *urlset;
	ਬਣਤਰ strbuf name_buf = STRBUF_INIT;
	ਬਣਤਰ option options[] = {
		OPT_BOOL('\0', "push", &ਧੱਕੋ_ਬਹੁਲਕ,
			 N_("manipulate push URLs")),
		OPT_BOOL('\0', "add", &ਜੋੜੋ_ਬਹੁਲਕ,
			 N_("add URL")),
		OPT_BOOL('\0', "delete", &delete_ਬਹੁਲਕ,
			    N_("delete URLs")),
		OPT_ਸਿਰਾ()
	};
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     builtin_remote_seturl_usage,
			     PARSE_OPT_KEEP_ARGV0);

	ਜੇ (ਜੋੜੋ_ਬਹੁਲਕ && delete_ਬਹੁਲਕ)
		die(_("--add --delete doesn't make sense"));

	ਜੇ (argc < 3 || argc > 4 || ((ਜੋੜੋ_ਬਹੁਲਕ || delete_ਬਹੁਲਕ) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	ਜੇ (argc > 3)
		oldurl = argv[3];

	ਜੇ (delete_ਬਹੁਲਕ)
		oldurl = newurl;

	remote = remote_get(remotename);
	ਜੇ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		ਬਾਹਰ_ਨਿਕਲੋ(2);
	}

	ਜੇ (ਧੱਕੋ_ਬਹੁਲਕ) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} ਨਹੀਂਤਾਂ {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	ਜੇ ((!oldurl && !delete_ਬਹੁਲਕ) || ਜੋੜੋ_ਬਹੁਲਕ) {
		ਜੇ (ਜੋੜੋ_ਬਹੁਲਕ)
			repo_config_ਸੈੱਟ_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		ਨਹੀਂਤਾਂ
			repo_config_ਸੈੱਟ(the_repository, name_buf.buf, newurl);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Old URL specified. Demand that one matches. */
	ਜੇ (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	ਲਈ (ਮਾਪ_t i = 0; i < urlset->nr; i++)
		ਜੇ (!regexec(&old_regex, urlset->v[i], 0, ਨਿਰਾ, 0))
			matches++;
		ਨਹੀਂਤਾਂ
			negative_matches++;
	ਜੇ (!delete_ਬਹੁਲਕ && !matches)
		die(_("No such URL found: %s"), oldurl);
	ਜੇ (delete_ਬਹੁਲਕ && !negative_matches && !ਧੱਕੋ_ਬਹੁਲਕ)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	ਜੇ (!delete_ਬਹੁਲਕ)
		repo_config_ਸੈੱਟ_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	ਨਹੀਂਤਾਂ
		repo_config_ਸੈੱਟ_multivar(the_repository, name_buf.buf, ਨਿਰਾ, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
ਬਾਹਰਲਾ:
	strbuf_release(&name_buf);
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_remote(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo)
{
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, ਜੋੜੋ),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, ਨਿਰਾ, PARSE_OPT_NOCOMPLETE),
		OPT_SUBCOMMAND("remove", &fn, rm),
		OPT_SUBCOMMAND("set-head", &fn, ਸੈੱਟ_ਸਿਰ),
		OPT_SUBCOMMAND("set-branches", &fn, ਸੈੱਟ_branches),
		OPT_SUBCOMMAND("get-url", &fn, get_url),
		OPT_SUBCOMMAND("set-url", &fn, ਸੈੱਟ_url),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("update", &fn, update),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, builtin_remote_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	ਜੇ (fn) {
		ਵਾਪਸ !!fn(argc, argv, ਅਗੇਤਰ, repo);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		ਵਾਪਸ !!show_all();
	}
}
