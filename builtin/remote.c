#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "date.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "ident.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "transport.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "rebase.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "refspec.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "commit-reach.h"
#ಅನ್ತರ್ಭಾವ "progress.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_remote_usage[] = {
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
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_remote_ಕೂಡಿಸು_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	ಖಾಲಿ
};

#ವ್ಯಾಖ್ಯಾನಿಸು GET_REF_ಸ್ಥಿತಿಗಳು (1<<0)
#ವ್ಯಾಖ್ಯಾನಿಸು GET_ತಲೆ_ಹೆಸರುಗಳು (1<<1)
#ವ್ಯಾಖ್ಯಾನಿಸು GET_ತಳ್ಳು_REF_ಸ್ಥಿತಿಗಳು (1<<2)

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verbose;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fetch_remote(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "fetch");
	ಆದರೆ (verbose)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "-v");
	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	ಆದರೆ (ಸತತ_command(&cmd))
		ಹಿಂತಿರುಗಿ error(_("Could not fetch %s"), name);
	ಹಿಂತಿರುಗಿ 0;
}

ಎಣಿಕೆ {
	ಟ್ಯಾಗುಗಳು_UNSET = 0,
	ಟ್ಯಾಗುಗಳು_DEFAULT = 1,
	ಟ್ಯಾಗುಗಳು_ಗಣ = 2
};

#ವ್ಯಾಖ್ಯಾನಿಸು MIRROR_NONE 0
#ವ್ಯಾಖ್ಯಾನಿಸು MIRROR_FETCH 1
#ವ್ಯಾಖ್ಯಾನಿಸು MIRROR_ತಳ್ಳು 2
#ವ್ಯಾಖ್ಯಾನಿಸು MIRROR_BOTH (MIRROR_FETCH|MIRROR_ತಳ್ಳು)

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_branch(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branchname,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remotename, ಪೂರ್ಣಾಂಕ mirror, ರಚನೆ strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	ಆದರೆ (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_ಗಣ_multivar(the_repository, ಕೀಲಿ, tmp->buf, "^$", 0);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_mirror_opt(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ not)
{
	ಚಿಹ್ನೆರಹಿತ *mirror = opt->ಮೌಲ್ಯ;
	ಆದರೆ (not)
		*mirror = MIRROR_NONE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "push"))
		*mirror = MIRROR_ತಳ್ಳು;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("unknown --mirror argument: %s"), arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_remote_collision(ರಚನೆ remote *remote, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = ದತ್ತಾಂಶ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;

	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ fetch = 0, fetch_ಟ್ಯಾಗುಗಳು = ಟ್ಯಾಗುಗಳು_DEFAULT;
	ಚಿಹ್ನೆರಹಿತ mirror = MIRROR_NONE;
	ರಚನೆ string_list track = STRING_LIST_INIT_NODUP;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *master = ಖಾಲಿ;
	ರಚನೆ remote *remote;
	ರಚನೆ strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, *url;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;

	ರಚನೆ option options[] = {
		OPT_BOOL('f', "fetch", &fetch, N_("fetch the remote branches")),
		OPT_ಗಣ_INT(0, "tags", &fetch_ಟ್ಯಾಗುಗಳು,
			    N_("import all tags and associated objects when fetching\n"
			       "or do not fetch any tag at all (--no-tags)"),
			    ಟ್ಯಾಗುಗಳು_ಗಣ),
		OPT_STRING_LIST('t', "track", &track, N_("branch"),
				N_("branch(es) to track")),
		OPT_STRING('m', "master", &master, N_("branch"), N_("master branch")),
		OPT_CALLBACK_F(0, "mirror", &mirror, "(push|fetch)",
			N_("set up remote as a mirror to push to or fetch from"),
			PARSE_OPT_OPTARG | PARSE_OPT_COMP_ARG, parse_mirror_opt),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_remote_ಕೂಡಿಸು_usage, 0);

	ಆದರೆ (argc != 2)
		usage_with_options(builtin_remote_ಕೂಡಿಸು_usage, options);

	ಆದರೆ (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	ಆದರೆ (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	ಆದರೆ (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		ನಿರ್ಗಮಿಸು(3);
	}

	ಆದರೆ (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (ಶೂನ್ಯ *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_ಗಣ(the_repository, buf.buf, url);

	ಆದರೆ (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		ಆದರೆ (track.nr == 0)
			string_list_append(&track, "*");
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < track.nr; i++) {
			ಕೂಡಿಸು_branch(buf.buf, track.ವಸ್ತುಗಳು[i].string,
				   name, mirror, &buf2);
		}
	}

	ಆದರೆ (mirror & MIRROR_ತಳ್ಳು) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_ಗಣ(the_repository, buf.buf, "true");
	}

	ಆದರೆ (fetch_ಟ್ಯಾಗುಗಳು != ಟ್ಯಾಗುಗಳು_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_ಗಣ(the_repository, buf.buf,
				fetch_ಟ್ಯಾಗುಗಳು == ಟ್ಯಾಗುಗಳು_ಗಣ ? "--tags" : "--no-tags");
	}

	ಆದರೆ (fetch && fetch_remote(name)) {
		ಫಲಿತಾಂಶ = 1;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		ಆದರೆ (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			ಫಲಿತಾಂಶ = error(_("Could not setup master '%s'"), master);
	}

ಹೊರಹೋಗು:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ರಚನೆ branch_info {
	ಅಕ್ಷರ *remote_name;
	ರಚನೆ string_list ವಿಲೀನ;
	ಎಣಿಕೆ rebase_type rebase;
	ಅಕ್ಷರ *ತಳ್ಳು_remote_name;
};

ಸ್ಥಿರ ರಚನೆ string_list branch_list = STRING_LIST_INIT_DUP;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *abbrev_ref(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	skip_ಪೂರ್ವಪ್ರತ್ಯಯ(name, ಪೂರ್ವಪ್ರತ್ಯಯ, &name);
	ಹಿಂತಿರುಗಿ name;
}
#ವ್ಯಾಖ್ಯಾನಿಸು abbrev_branch(name) abbrev_ref((name), "refs/heads/")

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ config_read_branches(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
				ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx ಬಳಸದ,
				ಶೂನ್ಯ *ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *orig_ಕೀಲಿ = ಕೀಲಿ;
	ಅಕ್ಷರ *name;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ರಚನೆ branch_info *info;
	ಎಣಿಕೆ { REMOTE, ವಿಲೀನ_2, REBASE, ತಳ್ಳು_REMOTE } type;
	ಗಾತ್ರ_t ಕೀಲಿ_len;

	ಆದರೆ (!starts_with(ಕೀಲಿ, "branch."))
		ಹಿಂತಿರುಗಿ 0;

	ಕೀಲಿ += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("branch.");
	ಆದರೆ (strip_suffix(ಕೀಲಿ, ".remote", &ಕೀಲಿ_len))
		type = REMOTE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strip_suffix(ಕೀಲಿ, ".merge", &ಕೀಲಿ_len))
		type = ವಿಲೀನ_2;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strip_suffix(ಕೀಲಿ, ".rebase", &ಕೀಲಿ_len))
		type = REBASE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strip_suffix(ಕೀಲಿ, ".pushremote", &ಕೀಲಿ_len))
		type = ತಳ್ಳು_REMOTE;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ 0;

	name = xmemdupz(ಕೀಲಿ, ಕೀಲಿ_len);
	ಅಂಶ = string_list_ಸೇರಿಸು(&branch_list, name);

	ಆದರೆ (!ಅಂಶ->util)
		ಅಂಶ->util = xcalloc(1, ಗಾತ್ರ(ರಚನೆ branch_info));
	info = ಅಂಶ->util;
	ಆಯ್ಕೆ (type) {
	ಸ್ಥಿತಿ REMOTE:
		ಆದರೆ (info->remote_name)
			warning(_("more than one %s"), orig_ಕೀಲಿ);
		info->remote_name = xstrdup(ಮೌಲ್ಯ);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ ವಿಲೀನ_2: {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *space = strchr(ಮೌಲ್ಯ, ' ');
		ಮೌಲ್ಯ = abbrev_branch(ಮೌಲ್ಯ);
		ಆಗ (space) {
			ಅಕ್ಷರ *ವಿಲೀನ;
			ವಿಲೀನ = xstrndup(ಮೌಲ್ಯ, space - ಮೌಲ್ಯ);
			string_list_append(&info->ವಿಲೀನ, ವಿಲೀನ);
			ಮೌಲ್ಯ = abbrev_branch(space + 1);
			space = strchr(ಮೌಲ್ಯ, ' ');
		}
		string_list_append(&info->ವಿಲೀನ, xstrdup(ಮೌಲ್ಯ));
		ನಿಲ್ಲಿಸು;
	}
	ಸ್ಥಿತಿ REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_ಮೌಲ್ಯ(ಮೌಲ್ಯ);
		ಆದರೆ (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, ಮೌಲ್ಯ);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ ತಳ್ಳು_REMOTE:
		ಆದರೆ (info->ತಳ್ಳು_remote_name)
			warning(_("more than one %s"), orig_ಕೀಲಿ);
		info->ತಳ್ಳು_remote_name = xstrdup(ಮೌಲ್ಯ);
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unexpected type=%d", type);
	}

	ಬಿಡುಗಡೆ(name);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_branches(ಶೂನ್ಯ)
{
	ಆದರೆ (branch_list.nr)
		ಹಿಂತಿರುಗಿ;
	repo_config(the_repository, config_read_branches, ಖಾಲಿ);
}

ರಚನೆ ref_ಸ್ಥಿತಿಗಳು {
	ರಚನೆ remote *remote;
	ರಚನೆ string_list new_refs, skipped, stale, tracked, heads, ತಳ್ಳು;
	ಪೂರ್ಣಾಂಕ queried;
};

#ವ್ಯಾಖ್ಯಾನಿಸು REF_ಸ್ಥಿತಿಗಳು_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.ತಳ್ಳು = STRING_LIST_INIT_DUP, \
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ref_ಸ್ಥಿತಿಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ ref *remote_refs, ರಚನೆ ref_ಸ್ಥಿತಿಗಳು *ಸ್ಥಿತಿಗಳು)
{
	ರಚನೆ ref *fetch_map = ಖಾಲಿ, **ಬಾಲ = &fetch_map;
	ರಚನೆ ref *ref, *stale_refs;
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < ಸ್ಥಿತಿಗಳು->remote->fetch.nr; i++)
		ಆದರೆ (get_fetch_map(remote_refs, &ಸ್ಥಿತಿಗಳು->remote->fetch.ವಸ್ತುಗಳು[i], &ಬಾಲ, 1))
			die(_("Could not get fetch map for refspec %s"),
				ಸ್ಥಿತಿಗಳು->remote->fetch.ವಸ್ತುಗಳು[i].raw);

	ಗಾಗಿ (ref = fetch_map; ref; ref = ref->ಮುಂದಿನ) {
		ಆದರೆ (refname_matches_negative_refspec_ಅಂಶ(ref->name, &ಸ್ಥಿತಿಗಳು->remote->fetch))
			string_list_append(&ಸ್ಥಿತಿಗಳು->skipped, abbrev_branch(ref->name));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&ಸ್ಥಿತಿಗಳು->new_refs, abbrev_branch(ref->name));
		ಇಲ್ಲದಿದ್ದರೆ
			string_list_append(&ಸ್ಥಿತಿಗಳು->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&ಸ್ಥಿತಿಗಳು->remote->fetch, fetch_map);
	ಗಾಗಿ (ref = stale_refs; ref; ref = ref->ಮುಂದಿನ) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ =
			string_list_append(&ಸ್ಥಿತಿಗಳು->stale, abbrev_branch(ref->name));
		ಅಂಶ->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_ವಿಂಗಡಿಸು(&ಸ್ಥಿತಿಗಳು->new_refs);
	string_list_ವಿಂಗಡಿಸು(&ಸ್ಥಿತಿಗಳು->skipped);
	string_list_ವಿಂಗಡಿಸು(&ಸ್ಥಿತಿಗಳು->tracked);
	string_list_ವಿಂಗಡಿಸು(&ಸ್ಥಿತಿಗಳು->stale);

	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ ತಳ್ಳು_info {
	ಅಕ್ಷರ *dest;
	ಪೂರ್ಣಾಂಕ forced;
	ಎಣಿಕೆ {
		ತಳ್ಳು_ಸ್ಥಿತಿ_CREATE = 0,
		ತಳ್ಳು_ಸ್ಥಿತಿ_DELETE,
		ತಳ್ಳು_ಸ್ಥಿತಿ_UPTODATE,
		ತಳ್ಳು_ಸ್ಥಿತಿ_FASTFORWARD,
		ತಳ್ಳು_ಸ್ಥಿತಿ_OUTOFDATE,
		ತಳ್ಳು_ಸ್ಥಿತಿ_NOTQUERIED
	} ಸ್ಥಿತಿ_2;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ತಳ್ಳು_ref_ಸ್ಥಿತಿಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ ref *remote_refs,
	ರಚನೆ ref_ಸ್ಥಿತಿಗಳು *ಸ್ಥಿತಿಗಳು)
{
	ರಚನೆ remote *remote = ಸ್ಥಿತಿಗಳು->remote;
	ರಚನೆ ref *ref, *local_refs, *ತಳ್ಳು_map;
	ಆದರೆ (remote->mirror)
		ಹಿಂತಿರುಗಿ 0;

	local_refs = get_local_heads();
	ತಳ್ಳು_map = copy_ref_list(remote_refs);

	ಹೊಂದಿಕೆ_ತಳ್ಳು_refs(local_refs, &ತಳ್ಳು_map, &remote->ತಳ್ಳು, ಹೊಂದಿಕೆ_REFS_NONE);

	ಗಾಗಿ (ref = ತಳ್ಳು_map; ref; ref = ref->ಮುಂದಿನ) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
		ರಚನೆ ತಳ್ಳು_info *info;

		ಆದರೆ (!ref->peer_ref)
			ಮುಂದುವರಿ;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		ಅಂಶ = string_list_append(&ಸ್ಥಿತಿಗಳು->ತಳ್ಳು,
					  abbrev_branch(ref->peer_ref->name));
		ಅಂಶ->util = xcalloc(1, ಗಾತ್ರ(ರಚನೆ ತಳ್ಳು_info));
		info = ಅಂಶ->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		ಆದರೆ (is_null_oid(&ref->new_oid)) {
			info->ಸ್ಥಿತಿ_2 = ತಳ್ಳು_ಸ್ಥಿತಿ_DELETE;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (oideq(&ref->old_oid, &ref->new_oid))
			info->ಸ್ಥಿತಿ_2 = ತಳ್ಳು_ಸ್ಥಿತಿ_UPTODATE;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_null_oid(&ref->old_oid))
			info->ಸ್ಥಿತಿ_2 = ತಳ್ಳು_ಸ್ಥಿತಿ_CREATE;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->ಸ್ಥಿತಿ_2 = ತಳ್ಳು_ಸ್ಥಿತಿ_FASTFORWARD;
		ಇಲ್ಲದಿದ್ದರೆ
			info->ಸ್ಥಿತಿ_2 = ತಳ್ಳು_ಸ್ಥಿತಿ_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(ತಳ್ಳು_map);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ತಳ್ಳು_ref_ಸ್ಥಿತಿಗಳು_noquery(ರಚನೆ ref_ಸ್ಥಿತಿಗಳು *ಸ್ಥಿತಿಗಳು)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ remote *remote = ಸ್ಥಿತಿಗಳು->remote;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ರಚನೆ ತಳ್ಳು_info *info;

	ಆದರೆ (remote->mirror)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!remote->ತಳ್ಳು.nr) {
		ಅಂಶ = string_list_append(&ಸ್ಥಿತಿಗಳು->ತಳ್ಳು, _("(matching)"));
		info = ಅಂಶ->util = xcalloc(1, ಗಾತ್ರ(ರಚನೆ ತಳ್ಳು_info));
		info->ಸ್ಥಿತಿ_2 = ತಳ್ಳು_ಸ್ಥಿತಿ_NOTQUERIED;
		info->dest = xstrdup(ಅಂಶ->string);
	}
	ಗಾಗಿ (i = 0; i < remote->ತಳ್ಳು.nr; i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ refspec_ಅಂಶ *spec = &remote->ತಳ್ಳು.ವಸ್ತುಗಳು[i];
		ಆದರೆ (spec->matching)
			ಅಂಶ = string_list_append(&ಸ್ಥಿತಿಗಳು->ತಳ್ಳು, _("(matching)"));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(spec->src))
			ಅಂಶ = string_list_append(&ಸ್ಥಿತಿಗಳು->ತಳ್ಳು, spec->src);
		ಇಲ್ಲದಿದ್ದರೆ
			ಅಂಶ = string_list_append(&ಸ್ಥಿತಿಗಳು->ತಳ್ಳು, _("(delete)"));

		info = ಅಂಶ->util = xcalloc(1, ಗಾತ್ರ(ರಚನೆ ತಳ್ಳು_info));
		info->forced = spec->force;
		info->ಸ್ಥಿತಿ_2 = ತಳ್ಳು_ಸ್ಥಿತಿ_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : ಅಂಶ->string);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ತಲೆ_ಹೆಸರುಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ ref *remote_refs, ರಚನೆ ref_ಸ್ಥಿತಿಗಳು *ಸ್ಥಿತಿಗಳು)
{
	ರಚನೆ ref *ref, *matches;
	ರಚನೆ ref *fetch_map = ಖಾಲಿ, **fetch_map_ಬಾಲ = &fetch_map;
	ರಚನೆ refspec_ಅಂಶ refspec = {
		.force = 0,
		.ನಮೂನೆ = 1,
		.src = (ಅಕ್ಷರ *) "refs/heads/*",
		.dst = (ಅಕ್ಷರ *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_ಬಾಲ, 0);
	matches = guess_remote_ತಲೆ(ಕಂಡುಹಿಡಿ_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_ತಲೆ_ALL);
	ಗಾಗಿ (ref = matches; ref; ref = ref->ಮುಂದಿನ)
		string_list_append(&ಸ್ಥಿತಿಗಳು->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ ತಿಳಿದ_remote {
	ರಚನೆ ತಿಳಿದ_remote *ಮುಂದಿನ;
	ರಚನೆ remote *remote;
};

ರಚನೆ ತಿಳಿದ_remotes {
	ರಚನೆ remote *ಗಮ್ಯ_delete;
	ರಚನೆ ತಿಳಿದ_remote *list;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_ತಿಳಿದ_remote(ರಚನೆ remote *remote, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ ತಿಳಿದ_remotes *all = cb_ದತ್ತಾಂಶ;
	ರಚನೆ ತಿಳಿದ_remote *r;

	ಆದರೆ (!strcmp(all->ಗಮ್ಯ_delete->name, remote->name))
		ಹಿಂತಿರುಗಿ 0;

	r = xmalloc(ಗಾತ್ರ(*r));
	r->remote = remote;
	r->ಮುಂದಿನ = all->list;
	all->list = r;
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ branches_for_remote {
	ರಚನೆ remote *remote;
	ರಚನೆ string_list *branches, *skipped;
	ರಚನೆ ತಿಳಿದ_remotes *keep;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_branch_for_removal(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ branches_for_remote *branches = cb_ದತ್ತಾಂಶ;
	ರಚನೆ refspec_ಅಂಶ refspec;
	ರಚನೆ ತಿಳಿದ_remote *kr;

	memset(&refspec, 0, ಗಾತ್ರ(refspec));
	refspec.dst = (ಅಕ್ಷರ *)ref->name;
	ಆದರೆ (remote_ಕಂಡುಹಿಡಿ_tracking(branches->remote, &refspec))
		ಹಿಂತಿರುಗಿ 0;
	ಬಿಡುಗಡೆ(refspec.src);

	/* don't delete a branch if another remote also uses it */
	ಗಾಗಿ (kr = branches->keep->list; kr; kr = kr->ಮುಂದಿನ) {
		memset(&refspec, 0, ಗಾತ್ರ(refspec));
		refspec.dst = (ಅಕ್ಷರ *)ref->name;
		ಆದರೆ (!remote_ಕಂಡುಹಿಡಿ_tracking(kr->remote, &refspec)) {
			ಬಿಡುಗಡೆ(refspec.src);
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	ಆದರೆ (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		ಆದರೆ (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		ಹಿಂತಿರುಗಿ 0;
	}

	string_list_append(branches->branches, ref->name);

	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ rename_info {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old_name;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_name;
	ರಚನೆ ref_transaction *transaction;
	ರಚನೆ ಪ್ರಗತಿ *ಪ್ರಗತಿ;
	ರಚನೆ strbuf *err;
	uint32_t ಪ್ರಗತಿ_nr;
	uint64_t ಸೂಚ್ಯಂಕ;
};

ಸ್ಥಿರ ಶೂನ್ಯ compute_renamed_ref(ರಚನೆ rename_info *rename,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname,
				ರಚನೆ strbuf *ಹೊರಹೋಗು)
{
	strbuf_reset(ಹೊರಹೋಗು);
	strbuf_addstr(ಹೊರಹೋಗು, refname);
	strbuf_splice(ಹೊರಹೋಗು, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("refs/remotes/"), ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(rename->old_name),
		      rename->new_name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(rename->new_name));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rename_one_reflog_ನಮೂದು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old_refname,
				   ರಚನೆ object_id *old_oid,
				   ರಚನೆ object_id *new_oid,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *committer,
				   timestamp_t timestamp, ಪೂರ್ಣಾಂಕ tz,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ rename_info *rename = cb_ದತ್ತಾಂಶ;
	ರಚನೆ strbuf new_refname = STRBUF_INIT;
	ರಚನೆ strbuf ತಟಸ್ಥ = STRBUF_INIT;
	ರಚನೆ strbuf name = STRBUF_INIT;
	ರಚನೆ strbuf mail = STRBUF_INIT;
	ರಚನೆ ident_ವಿಭಜಿಸು ident;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *date;
	ಪೂರ್ಣಾಂಕ error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	ಆದರೆ (ವಿಭಜಿಸು_ident_ಸಾಲು(&ident, committer, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(committer)) < 0) {
		error = -1;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	strbuf_ಕೂಡಿಸು(&name, ident.name_begin, ident.name_ಕೊನೆ - ident.name_begin);
	strbuf_ಕೂಡಿಸು(&mail, ident.mail_begin, ident.mail_ಕೊನೆ - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_ಬಹುಲಕ(ಸಾಮಾನ್ಯ));
	strbuf_addstr(&ತಟಸ್ಥ, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, ತಟಸ್ಥ.buf, ಸಂದೇಶ,
					      rename->ಸೂಚ್ಯಂಕ++, rename->err);

ಹೊರಹೋಗು:
	strbuf_release(&new_refname);
	strbuf_release(&ತಟಸ್ಥ);
	strbuf_release(&name);
	strbuf_release(&mail);
	ಹಿಂತಿರುಗಿ error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rename_one_reflog(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old_refname,
			     ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_oid,
			     ರಚನೆ rename_info *rename)
{
	ರಚನೆ strbuf new_refname = STRBUF_INIT;
	ರಚನೆ strbuf ಸಂದೇಶ_2 = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ error;

	ಆದರೆ (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		ಹಿಂತಿರುಗಿ 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_ನಮೂದು, rename);
	ಆದರೆ (error < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	compute_renamed_ref(rename, old_refname, &new_refname);

	/*
	 * Manually write the reflog entry for the now-renamed ref. We cannot
	 * rely on `rename_one_ref()` to do this for us as that would screw
	 * over order in which reflog entries are being written.
	 *
	 * Furthermore, we only append the entry in case the reference
	 * resolves. Missing references shouldn't have reflogs anyway.
	 */
	strbuf_addf(&ಸಂದೇಶ_2, "remote: renamed %s to %s", old_refname,
		    new_refname.buf);

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      old_oid, old_oid, git_committer_info(0),
					      ಸಂದೇಶ_2.buf, rename->ಸೂಚ್ಯಂಕ++, rename->err);
	ಆದರೆ (error < 0)
		ಹಿಂತಿರುಗಿ error;

ಹೊರಹೋಗು:
	strbuf_release(&new_refname);
	strbuf_release(&ಸಂದೇಶ_2);
	ಹಿಂತಿರುಗಿ error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rename_one_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ strbuf new_referent = STRBUF_INIT;
	ರಚನೆ strbuf new_refname = STRBUF_INIT;
	ರಚನೆ rename_info *rename = cb_ದತ್ತಾಂಶ;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid = ref->oid;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *referent = ref->ಗುರಿ;
	ಪೂರ್ಣಾಂಕ error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	ಆದರೆ (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   ಖಾಲಿ, ಖಾಲಿ);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = ಖಾಲಿ;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, ಖಾಲಿ, rename->err);
	ಆದರೆ (error < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : ಖಾಲಿ, ಖಾಲಿ,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       ಖಾಲಿ, rename->err);
	ಆದರೆ (error < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	error = rename_one_reflog(ref->name, oid, rename);
	ಆದರೆ (error < 0)
		ಹೋಗು ಹೊರಹೋಗು;

	display_ಪ್ರಗತಿ(rename->ಪ್ರಗತಿ, ++rename->ಪ್ರಗತಿ_nr);

ಹೊರಹೋಗು:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	ಹಿಂತಿರುಗಿ error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ migrate_file(ರಚನೆ remote *remote)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < remote->url.nr; i++)
		repo_config_ಗಣ_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < remote->ತಳ್ಳು.nr; i++)
		repo_config_ಗಣ_multivar(the_repository, buf.buf, remote->ತಳ್ಳು.ವಸ್ತುಗಳು[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < remote->fetch.nr; i++)
		repo_config_ಗಣ_multivar(the_repository, buf.buf, remote->fetch.ವಸ್ತುಗಳು[i].raw, "^$", 0);
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WITH_BREAKING_CHANGES
	ಆದರೆ (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_ಮಾರ್ಗ_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_ಮಾರ್ಗ_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#ಆದರೆ_ಅಂತ್ಯ /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ ತಳ್ಳು_default_info
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old_name;
	ಎಣಿಕೆ config_scope scope;
	ರಚನೆ strbuf origin;
	ಪೂರ್ಣಾಂಕ linenr;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ config_read_ತಳ್ಳು_default(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
	ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi = ctx->kvi;

	ರಚನೆ ತಳ್ಳು_default_info* info = cb;
	ಆದರೆ (strcmp(ಕೀಲಿ, "remote.pushdefault") ||
	    !ಮೌಲ್ಯ || strcmp(ಮೌಲ್ಯ, info->old_name))
		ಹಿಂತಿರುಗಿ 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_ತಳ್ಳು_default(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* old_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* new_name)
{
	ರಚನೆ ತಳ್ಳು_default_info ತಳ್ಳು_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_ತಳ್ಳು_default, &ತಳ್ಳು_default);
	ಆದರೆ (ತಳ್ಳು_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ತಳ್ಳು_default.scope >= CONFIG_SCOPE_LOCAL) {
		ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = repo_config_ಗಣ_gently(the_repository, "remote.pushDefault",
						    new_name);
		ಆದರೆ (new_name && ಫಲಿತಾಂಶ && ಫಲಿತಾಂಶ != CONFIG_NOTHING_ಗಣ)
			die(_("could not set '%s'"), "remote.pushDefault");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!new_name && ಫಲಿತಾಂಶ && ಫಲಿತಾಂಶ != CONFIG_NOTHING_ಗಣ)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ತಳ್ಳು_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(ತಳ್ಳು_default.scope),
			ತಳ್ಳು_default.origin.buf, ತಳ್ಳು_default.linenr,
			old_name);
	}

	strbuf_release(&ತಳ್ಳು_default.origin);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mv(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	      ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ show_ಪ್ರಗತಿ = isatty(2);
	ರಚನೆ option options[] = {
		OPT_BOOL(0, "progress", &show_ಪ್ರಗತಿ, N_("force progress reporting")),
		OPT_ಕೊನೆ()
	};
	ರಚನೆ remote *oldremote, *newremote;
	ರಚನೆ strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	ರಚನೆ rename_info rename = {
		.err = &err,
	};
	ಪೂರ್ಣಾಂಕ refspecs_need_update = 0;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_remote_rename_usage, 0);

	ಆದರೆ (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	ಆದರೆ (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		ನಿರ್ಗಮಿಸು(2);
	}

	ಆದರೆ (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		ಹಿಂತಿರುಗಿ migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	ಆದರೆ (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		ನಿರ್ಗಮಿಸು(3);
	}

	ಆದರೆ (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	ಆದರೆ (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		ಫಲಿತಾಂಶ = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(oldremote->fetch.ವಸ್ತುಗಳು[i].raw,
						old_remote_context.buf);

	ಆದರೆ (refspecs_need_update) {
		ರಚನೆ refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		ಆದರೆ (!rename.transaction)
			ಹೋಗು ಹೊರಹೋಗು;

		ಆದರೆ (show_ಪ್ರಗತಿ)
			rename.ಪ್ರಗತಿ = ಆರಂಭ_delayed_ಪ್ರಗತಿ(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.ಪೂರ್ವಪ್ರತ್ಯಯ = buf.buf;

		ಫಲಿತಾಂಶ = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		ಆದರೆ (ಫಲಿತಾಂಶ < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		ಫಲಿತಾಂಶ = ref_transaction_prepare(rename.transaction, &err);
		ಆದರೆ (ಫಲಿತಾಂಶ < 0) {
			error("renaming remote references failed: %s", err.buf);
			ಆದರೆ (ಫಲಿತಾಂಶ == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(ಖಾಲಿ);
		}
	}

	ಆದರೆ (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_ಗಣ_multivar(the_repository, buf.buf, ಖಾಲಿ, ಖಾಲಿ, CONFIG_FLAGS_MULTI_REPLACE);
		ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < oldremote->fetch.nr; i++) {
			ಅಕ್ಷರ *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.ವಸ್ತುಗಳು[i].raw);
			ptr = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(buf2.buf, old_remote_context.buf);
			ಆದರೆ (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(":refs/remotes/"),
					      ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(rename.old_name), rename.new_name,
					      ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(rename.new_name));
			} ಇಲ್ಲದಿದ್ದರೆ
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_ಗಣ_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < branch_list.nr; i++) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ = branch_list.ವಸ್ತುಗಳು + i;
		ರಚನೆ branch_info *info = ಅಂಶ->util;
		ಆದರೆ (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", ಅಂಶ->string);
			repo_config_ಗಣ(the_repository, buf.buf, rename.new_name);
		}
		ಆದರೆ (info->ತಳ್ಳು_remote_name && !strcmp(info->ತಳ್ಳು_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", ಅಂಶ->string);
			repo_config_ಗಣ(the_repository, buf.buf, rename.new_name);
		}
	}

	ಆದರೆ (refspecs_need_update) {
		ಫಲಿತಾಂಶ = ref_transaction_commit(rename.transaction, &err);
		ಆದರೆ (ಫಲಿತಾಂಶ < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_ಪ್ರಗತಿ(&rename.ಪ್ರಗತಿ);

		handle_ತಳ್ಳು_default(rename.old_name, rename.new_name);
	}

ಹೊರಹೋಗು:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rm(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	      ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};
	ರಚನೆ remote *remote;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ ತಿಳಿದ_remotes ತಿಳಿದ_remotes = { ಖಾಲಿ, ಖಾಲಿ };
	ರಚನೆ string_list branches = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list skipped = STRING_LIST_INIT_DUP;
	ರಚನೆ branches_for_remote cb_ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;

	memset(&cb_ದತ್ತಾಂಶ, 0, ಗಾತ್ರ(cb_ದತ್ತಾಂಶ));
	cb_ದತ್ತಾಂಶ.branches = &branches;
	cb_ದತ್ತಾಂಶ.skipped = &skipped;
	cb_ದತ್ತಾಂಶ.keep = &ತಿಳಿದ_remotes;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_remote_rm_usage, 0);
	ಆದರೆ (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	ಆದರೆ (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		ನಿರ್ಗಮಿಸು(2);
	}

	ತಿಳಿದ_remotes.ಗಮ್ಯ_delete = remote;
	for_each_remote(ಕೂಡಿಸು_ತಿಳಿದ_remote, &ತಿಳಿದ_remotes);

	read_branches();
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < branch_list.nr; i++) {
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ = branch_list.ವಸ್ತುಗಳು + i;
		ರಚನೆ branch_info *info = ಅಂಶ->util;
		ಆದರೆ (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿಗಳು[] = { "remote", "merge", ಖಾಲಿ }, **k;
			ಗಾಗಿ (k = ಕೀಲಿಗಳು; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						ಅಂಶ->string, *k);
				ಫಲಿತಾಂಶ = repo_config_ಗಣ_gently(the_repository, buf.buf, ಖಾಲಿ);
				ಆದರೆ (ಫಲಿತಾಂಶ && ಫಲಿತಾಂಶ != CONFIG_NOTHING_ಗಣ)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		ಆದರೆ (info->ತಳ್ಳು_remote_name && !strcmp(info->ತಳ್ಳು_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", ಅಂಶ->string);
			ಫಲಿತಾಂಶ = repo_config_ಗಣ_gently(the_repository, buf.buf, ಖಾಲಿ);
			ಆದರೆ (ಫಲಿತಾಂಶ && ಫಲಿತಾಂಶ != CONFIG_NOTHING_ಗಣ)
				die(_("could not unset '%s'"), buf.buf);
		}
	}

	/*
	 * We cannot just pass a function to for_each_ref() which deletes
	 * the branches one by one, since for_each_ref() relies on cached
	 * refs, which are invalidated when deleting a branch.
	 */
	cb_ದತ್ತಾಂಶ.remote = remote;
	ಫಲಿತಾಂಶ = refs_for_each_ref(get_main_ref_store(the_repository),
				   ಕೂಡಿಸು_branch_for_removal, &cb_ದತ್ತಾಂಶ);
	strbuf_release(&buf);

	ಆದರೆ (!ಫಲಿತಾಂಶ)
		ಫಲಿತಾಂಶ = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	ಆದರೆ (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.ವಸ್ತುಗಳು[i].string);
	}
	string_list_clear(&skipped, 0);

	ಆದರೆ (!ಫಲಿತಾಂಶ) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		ಆದರೆ (repo_config_rename_section(the_repository, buf.buf, ಖಾಲಿ) < 1) {
			ಫಲಿತಾಂಶ = error(_("Could not remove config section '%s'"), buf.buf);
			ಹೋಗು ಹೊರಹೋಗು;
		}

		handle_ತಳ್ಳು_default(remote->name, ಖಾಲಿ);
	}

ಹೊರಹೋಗು:
	ಗಾಗಿ (ರಚನೆ ತಿಳಿದ_remote *r = ತಿಳಿದ_remotes.list; r;) {
		ರಚನೆ ತಿಳಿದ_remote *ಮುಂದಿನ = r->ಮುಂದಿನ;
		ಬಿಡುಗಡೆ(r);
		r = ಮುಂದಿನ;
	}
	strbuf_release(&buf);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ clear_ತಳ್ಳು_info(ಶೂನ್ಯ *util, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string ಬಳಸದ)
{
	ರಚನೆ ತಳ್ಳು_info *info = util;
	ಬಿಡುಗಡೆ(info->dest);
	ಬಿಡುಗಡೆ(info);
}

ಸ್ಥಿರ ಶೂನ್ಯ free_remote_ref_ಸ್ಥಿತಿಗಳು(ರಚನೆ ref_ಸ್ಥಿತಿಗಳು *ಸ್ಥಿತಿಗಳು)
{
	string_list_clear(&ಸ್ಥಿತಿಗಳು->new_refs, 0);
	string_list_clear(&ಸ್ಥಿತಿಗಳು->skipped, 0);
	string_list_clear(&ಸ್ಥಿತಿಗಳು->stale, 1);
	string_list_clear(&ಸ್ಥಿತಿಗಳು->tracked, 0);
	string_list_clear(&ಸ್ಥಿತಿಗಳು->heads, 0);
	string_list_clear_func(&ಸ್ಥಿತಿಗಳು->ತಳ್ಳು, clear_ತಳ್ಳು_info);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ append_ref_ಗಮ್ಯ_tracked_list(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ ref_ಸ್ಥಿತಿಗಳು *ಸ್ಥಿತಿಗಳು = cb_ದತ್ತಾಂಶ;
	ರಚನೆ refspec_ಅಂಶ refspec;

	ಆದರೆ (ref->flags & REF_ISSYMREF)
		ಹಿಂತಿರುಗಿ 0;

	memset(&refspec, 0, ಗಾತ್ರ(refspec));
	refspec.dst = (ಅಕ್ಷರ *)ref->name;
	ಆದರೆ (!remote_ಕಂಡುಹಿಡಿ_tracking(ಸ್ಥಿತಿಗಳು->remote, &refspec)) {
		string_list_append(&ಸ್ಥಿತಿಗಳು->tracked, abbrev_branch(refspec.src));
		ಬಿಡುಗಡೆ(refspec.src);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_remote_ref_ಸ್ಥಿತಿಗಳು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
				 ರಚನೆ ref_ಸ್ಥಿತಿಗಳು *ಸ್ಥಿತಿಗಳು,
				 ಪೂರ್ಣಾಂಕ query)
{
	ಸ್ಥಿತಿಗಳು->remote = remote_get(name);
	ಆದರೆ (!ಸ್ಥಿತಿಗಳು->remote)
		ಹಿಂತಿರುಗಿ error(_("No such remote: '%s'"), name);

	read_branches();

	ಆದರೆ (query) {
		ರಚನೆ transport *transport;
		ಸ್ಥಿರಾಂಕ ರಚನೆ ref *remote_refs;

		transport = transport_get(ಸ್ಥಿತಿಗಳು->remote, ಸ್ಥಿತಿಗಳು->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, ಖಾಲಿ);

		ಸ್ಥಿತಿಗಳು->queried = 1;
		ಆದರೆ (query & GET_REF_ಸ್ಥಿತಿಗಳು)
			get_ref_ಸ್ಥಿತಿಗಳು(remote_refs, ಸ್ಥಿತಿಗಳು);
		ಆದರೆ (query & GET_ತಲೆ_ಹೆಸರುಗಳು)
			get_ತಲೆ_ಹೆಸರುಗಳು(remote_refs, ಸ್ಥಿತಿಗಳು);
		ಆದರೆ (query & GET_ತಳ್ಳು_REF_ಸ್ಥಿತಿಗಳು)
			get_ತಳ್ಳು_ref_ಸ್ಥಿತಿಗಳು(remote_refs, ಸ್ಥಿತಿಗಳು);
		transport_disconnect(transport);
	} ಇಲ್ಲದಿದ್ದರೆ {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_ಗಮ್ಯ_tracked_list, ಸ್ಥಿತಿಗಳು);
		string_list_ವಿಂಗಡಿಸು(&ಸ್ಥಿತಿಗಳು->tracked);
		get_ತಳ್ಳು_ref_ಸ್ಥಿತಿಗಳು_noquery(ಸ್ಥಿತಿಗಳು);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ show_info {
	ರಚನೆ string_list list;
	ರಚನೆ ref_ಸ್ಥಿತಿಗಳು ಸ್ಥಿತಿಗಳು;
	ಪೂರ್ಣಾಂಕ ಅಗಲ, width2;
	ಪೂರ್ಣಾಂಕ any_rebase;
};

#ವ್ಯಾಖ್ಯಾನಿಸು SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.ಸ್ಥಿತಿಗಳು = REF_ಸ್ಥಿತಿಗಳು_INIT, \
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_remote_ಗಮ್ಯ_show_info(ರಚನೆ string_list_ಅಂಶ *ಅಂಶ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ show_info *info = cb_ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ n = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಅಂಶ->string);
	ಆದರೆ (n > info->ಅಗಲ)
		info->ಅಗಲ = n;
	string_list_ಸೇರಿಸು(&info->list, ಅಂಶ->string);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_remote_info_ಅಂಶ(ರಚನೆ string_list_ಅಂಶ *ಅಂಶ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ show_info *info = cb_ದತ್ತಾಂಶ;
	ರಚನೆ ref_ಸ್ಥಿತಿಗಳು *ಸ್ಥಿತಿಗಳು = &info->ಸ್ಥಿತಿಗಳು;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = ಅಂಶ->string;

	ಆದರೆ (ಸ್ಥಿತಿಗಳು->queried) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt = "%s";
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = "";
		ಆದರೆ (string_list_has_string(&ಸ್ಥಿತಿಗಳು->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = ಸ್ಥಿತಿಗಳು->remote->name;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (string_list_has_string(&ಸ್ಥಿತಿಗಳು->tracked, name))
			arg = _(" tracked");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (string_list_has_string(&ಸ್ಥಿತಿಗಳು->skipped, name))
			arg = _(" skipped");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (string_list_has_string(&ಸ್ಥಿತಿಗಳು->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		ಇಲ್ಲದಿದ್ದರೆ
			arg = _(" ???");
		ಬರೆ("    %-*s", info->ಅಗಲ, name);
		ಬರೆ(fmt, arg);
		ಬರೆ("\n");
	} ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ("    %s\n", name);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_local_ಗಮ್ಯ_show_info(ರಚನೆ string_list_ಅಂಶ *branch_ಅಂಶ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ show_info *show_info = cb_ದತ್ತಾಂಶ;
	ರಚನೆ ref_ಸ್ಥಿತಿಗಳು *ಸ್ಥಿತಿಗಳು = &show_info->ಸ್ಥಿತಿಗಳು;
	ರಚನೆ branch_info *branch_info = branch_ಅಂಶ->util;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಪೂರ್ಣಾಂಕ n;

	ಆದರೆ (!branch_info->ವಿಲೀನ.nr || !branch_info->remote_name ||
	    strcmp(ಸ್ಥಿತಿಗಳು->remote->name, branch_info->remote_name))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ ((n = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(branch_ಅಂಶ->string)) > show_info->ಅಗಲ)
		show_info->ಅಗಲ = n;
	ಆದರೆ (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	ಅಂಶ = string_list_ಸೇರಿಸು(&show_info->list, branch_ಅಂಶ->string);
	ಅಂಶ->util = branch_info;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_local_info_ಅಂಶ(ರಚನೆ string_list_ಅಂಶ *ಅಂಶ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ show_info *show_info = cb_ದತ್ತಾಂಶ;
	ರಚನೆ branch_info *branch_info = ಅಂಶ->util;
	ರಚನೆ string_list *ವಿಲೀನ = &branch_info->ವಿಲೀನ;
	ಪೂರ್ಣಾಂಕ ಅಗಲ = show_info->ಅಗಲ + 4;

	ಆದರೆ (branch_info->rebase >= REBASE_TRUE && branch_info->ವಿಲೀನ.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			ಅಂಶ->string);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಬರೆ("    %-*s ", show_info->ಅಗಲ, ಅಂಶ->string);
	ಆದರೆ (branch_info->rebase >= REBASE_TRUE) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ;
		ಆದರೆ (branch_info->rebase == REBASE_INTERACTIVE)
			ಸಂದೇಶ = _("rebases interactively onto remote %s");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (branch_info->rebase == REBASE_MERGES)
			ಸಂದೇಶ = _("rebases interactively (with merges) onto "
				"remote %s");
		ಇಲ್ಲದಿದ್ದರೆ
			ಸಂದೇಶ = _("rebases onto remote %s");
		printf_ln(ಸಂದೇಶ, ವಿಲೀನ->ವಸ್ತುಗಳು[0].string);
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), ವಿಲೀನ->ವಸ್ತುಗಳು[0].string);
		ಅಗಲ++;
	} ಇಲ್ಲದಿದ್ದರೆ {
		printf_ln(_("merges with remote %s"), ವಿಲೀನ->ವಸ್ತುಗಳು[0].string);
	}
	ಗಾಗಿ (ಗಾತ್ರ_t i = 1; i < ವಿಲೀನ->nr; i++)
		ಬರೆ(_("%-*s    and with remote %s\n"), ಅಗಲ, "",
		       ವಿಲೀನ->ವಸ್ತುಗಳು[i].string);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_ತಳ್ಳು_ಗಮ್ಯ_show_info(ರಚನೆ string_list_ಅಂಶ *ತಳ್ಳು_ಅಂಶ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ show_info *show_info = cb_ದತ್ತಾಂಶ;
	ರಚನೆ ತಳ್ಳು_info *ತಳ್ಳು_info = ತಳ್ಳು_ಅಂಶ->util;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಪೂರ್ಣಾಂಕ n;
	ಆದರೆ ((n = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ತಳ್ಳು_ಅಂಶ->string)) > show_info->ಅಗಲ)
		show_info->ಅಗಲ = n;
	ಆದರೆ ((n = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ತಳ್ಳು_info->dest)) > show_info->width2)
		show_info->width2 = n;
	ಅಂಶ = string_list_append(&show_info->list, ತಳ್ಳು_ಅಂಶ->string);
	ಅಂಶ->util = ತಳ್ಳು_ಅಂಶ->util;
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmp_string_with_ತಳ್ಳು(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *va, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *vb)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list_ಅಂಶ *a = va;
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list_ಅಂಶ *b = vb;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ತಳ್ಳು_info *a_ತಳ್ಳು = a->util;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ತಳ್ಳು_info *b_ತಳ್ಳು = b->util;
	ಪೂರ್ಣಾಂಕ cmp = strcmp(a->string, b->string);
	ಹಿಂತಿರುಗಿ cmp ? cmp : strcmp(a_ತಳ್ಳು->dest, b_ತಳ್ಳು->dest);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_ತಳ್ಳು_info_ಅಂಶ(ರಚನೆ string_list_ಅಂಶ *ಅಂಶ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ show_info *show_info = cb_ದತ್ತಾಂಶ;
	ರಚನೆ ತಳ್ಳು_info *ತಳ್ಳು_info = ಅಂಶ->util;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src = ಅಂಶ->string, *ಸ್ಥಿತಿ_2 = ಖಾಲಿ;

	ಆಯ್ಕೆ (ತಳ್ಳು_info->ಸ್ಥಿತಿ_2) {
	ಸ್ಥಿತಿ ತಳ್ಳು_ಸ್ಥಿತಿ_CREATE:
		ಸ್ಥಿತಿ_2 = _("create");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ ತಳ್ಳು_ಸ್ಥಿತಿ_DELETE:
		ಸ್ಥಿತಿ_2 = _("delete");
		src = _("(none)");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ ತಳ್ಳು_ಸ್ಥಿತಿ_UPTODATE:
		ಸ್ಥಿತಿ_2 = _("up to date");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ ತಳ್ಳು_ಸ್ಥಿತಿ_FASTFORWARD:
		ಸ್ಥಿತಿ_2 = _("fast-forwardable");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ ತಳ್ಳು_ಸ್ಥಿತಿ_OUTOFDATE:
		ಸ್ಥಿತಿ_2 = _("local out of date");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ ತಳ್ಳು_ಸ್ಥಿತಿ_NOTQUERIED:
		ನಿಲ್ಲಿಸು;
	}
	ಆದರೆ (ಸ್ಥಿತಿ_2) {
		ಆದರೆ (ತಳ್ಳು_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->ಅಗಲ, src,
			       show_info->width2, ತಳ್ಳು_info->dest, ಸ್ಥಿತಿ_2);
		ಇಲ್ಲದಿದ್ದರೆ
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->ಅಗಲ, src,
			       show_info->width2, ತಳ್ಳು_info->dest, ಸ್ಥಿತಿ_2);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ತಳ್ಳು_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->ಅಗಲ, src,
			       ತಳ್ಳು_info->dest);
		ಇಲ್ಲದಿದ್ದರೆ
			printf_ln(_("    %-*s pushes to %s"), show_info->ಅಗಲ, src,
			       ತಳ್ಳು_info->dest);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_one_ನಮೂದು(ರಚನೆ remote *remote, ಶೂನ್ಯ *priv)
{
	ರಚನೆ string_list *list = priv;
	ರಚನೆ strbuf remote_info_buf = STRBUF_INIT;
	ರಚನೆ strvec *url;

	ಆದರೆ (remote->url.nr > 0) {
		ರಚನೆ strbuf promisor_config = STRBUF_INIT;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *partial_clone_filter = ಖಾಲಿ;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		ಆದರೆ (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ಖಾಲಿ);
	} ಇಲ್ಲದಿದ್ದರೆ
		string_list_append(list, remote->name)->util = ಖಾಲಿ;
	url = ತಳ್ಳು_url_ದ_remote(remote);
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ಖಾಲಿ);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_all(ಶೂನ್ಯ)
{
	ರಚನೆ string_list list = STRING_LIST_INIT_DUP;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;

	ಫಲಿತಾಂಶ = for_each_remote(get_one_ನಮೂದು, &list);

	ಆದರೆ (!ಫಲಿತಾಂಶ) {
		string_list_ವಿಂಗಡಿಸು(&list);
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < list.nr; i++) {
			ರಚನೆ string_list_ಅಂಶ *ಅಂಶ = list.ವಸ್ತುಗಳು + i;
			ಆದರೆ (verbose)
				ಬರೆ("%s\t%s\n", ಅಂಶ->string,
					ಅಂಶ->util ? (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)ಅಂಶ->util : "");
			ಇಲ್ಲದಿದ್ದರೆ {
				ಆದರೆ (i && !strcmp((ಅಂಶ - 1)->string, ಅಂಶ->string))
					ಮುಂದುವರಿ;
				ಬರೆ("%s\n", ಅಂಶ->string);
			}
		}
	}
	string_list_clear(&list, 1);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ no_query = 0, ಫಲಿತಾಂಶ = 0, query_flag = 0;
	ರಚನೆ option options[] = {
		OPT_BOOL('n', ಖಾಲಿ, &no_query, N_("do not query remotes")),
		OPT_ಕೊನೆ()
	};
	ರಚನೆ show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_remote_show_usage,
			     0);

	ಆದರೆ (argc < 1)
		ಹಿಂತಿರುಗಿ show_all();

	ಆದರೆ (!no_query)
		query_flag = (GET_REF_ಸ್ಥಿತಿಗಳು | GET_ತಲೆ_ಹೆಸರುಗಳು | GET_ತಳ್ಳು_REF_ಸ್ಥಿತಿಗಳು);

	ಗಾಗಿ (; argc; argc--, argv++) {
		ಗಾತ್ರ_t i;
		ರಚನೆ strvec *url;

		get_remote_ref_ಸ್ಥಿತಿಗಳು(*argv, &info.ಸ್ಥಿತಿಗಳು, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.ಸ್ಥಿತಿಗಳು.remote->url.v[0]);
		url = ತಳ್ಳು_url_ದ_remote(info.ಸ್ಥಿತಿಗಳು.remote);
		ಗಾಗಿ (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		ಆದರೆ (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		ಆದರೆ (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!info.ಸ್ಥಿತಿಗಳು.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (info.ಸ್ಥಿತಿಗಳು.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.ಸ್ಥಿತಿಗಳು.heads.ವಸ್ತುಗಳು[0].string);
		ಇಲ್ಲದಿದ್ದರೆ {
			ಬರೆ(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			ಗಾಗಿ (i = 0; i < info.ಸ್ಥಿತಿಗಳು.heads.nr; i++)
				ಬರೆ("    %s\n", info.ಸ್ಥಿತಿಗಳು.heads.ವಸ್ತುಗಳು[i].string);
		}

		/* remote branch info */
		info.ಅಗಲ = 0;
		for_each_string_list(&info.ಸ್ಥಿತಿಗಳು.new_refs, ಕೂಡಿಸು_remote_ಗಮ್ಯ_show_info, &info);
		for_each_string_list(&info.ಸ್ಥಿತಿಗಳು.skipped, ಕೂಡಿಸು_remote_ಗಮ್ಯ_show_info, &info);
		for_each_string_list(&info.ಸ್ಥಿತಿಗಳು.tracked, ಕೂಡಿಸು_remote_ಗಮ್ಯ_show_info, &info);
		for_each_string_list(&info.ಸ್ಥಿತಿಗಳು.stale, ಕೂಡಿಸು_remote_ಗಮ್ಯ_show_info, &info);
		ಆದರೆ (info.list.nr)
			printf_ln(Q_("  Remote branch:%s",
				     "  Remote branches:%s",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_remote_info_ಅಂಶ, &info);
		string_list_clear(&info.list, 0);

		/* git pull info */
		info.ಅಗಲ = 0;
		info.any_rebase = 0;
		for_each_string_list(&branch_list, ಕೂಡಿಸು_local_ಗಮ್ಯ_show_info, &info);
		ಆದರೆ (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_ಅಂಶ, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		ಆದರೆ (info.ಸ್ಥಿತಿಗಳು.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.ಅಗಲ = info.width2 = 0;
		for_each_string_list(&info.ಸ್ಥಿತಿಗಳು.ತಳ್ಳು, ಕೂಡಿಸು_ತಳ್ಳು_ಗಮ್ಯ_show_info, &info);
		QSORT(info.list.ವಸ್ತುಗಳು, info.list.nr, cmp_string_with_ತಳ್ಳು);
		ಆದರೆ (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_ತಳ್ಳು_info_ಅಂಶ, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_ಸ್ಥಿತಿಗಳು(&info.ಸ್ಥಿತಿಗಳು);
	}

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ report_ಗಣ_ತಲೆ_auto(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ_name,
			ರಚನೆ strbuf *b_local_ತಲೆ, ಪೂರ್ಣಾಂಕ was_detached) {
	ರಚನೆ strbuf buf_ಪೂರ್ವಪ್ರತ್ಯಯ = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *prev_ತಲೆ = ಖಾಲಿ;

	strbuf_addf(&buf_ಪೂರ್ವಪ್ರತ್ಯಯ, "refs/remotes/%s/", remote);
	skip_ಪೂರ್ವಪ್ರತ್ಯಯ(b_local_ತಲೆ->buf, buf_ಪೂರ್ವಪ್ರತ್ಯಯ.buf, &prev_ತಲೆ);

	ಆದರೆ (prev_ತಲೆ && !strcmp(prev_ತಲೆ, ತಲೆ_name))
		ಬರೆ(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, ತಲೆ_name);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (prev_ತಲೆ)
		ಬರೆ(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_ತಲೆ, ತಲೆ_name);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!b_local_ತಲೆ->len)
		ಬರೆ(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, ತಲೆ_name);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (was_detached && b_local_ತಲೆ->len)
		ಬರೆ(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_ತಲೆ->buf, ತಲೆ_name);
	ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_ತಲೆ->buf, ತಲೆ_name);
	strbuf_release(&buf_ಪೂರ್ವಪ್ರತ್ಯಯ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗಣ_ತಲೆ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		    ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ opt_a = 0, opt_d = 0, ಫಲಿತಾಂಶ = 0, was_detached;
	ರಚನೆ strbuf b_ತಲೆ = STRBUF_INIT, b_remote_ತಲೆ = STRBUF_INIT,
		b_local_ತಲೆ = STRBUF_INIT;
	ಅಕ್ಷರ *ತಲೆ_name = ಖಾಲಿ;
	ರಚನೆ ref_store *refs = get_main_ref_store(the_repository);
	ರಚನೆ remote *remote;

	ರಚನೆ option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_ಕೊನೆ()
	};
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	ಆದರೆ (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_ತಲೆ, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	ಆದರೆ (!opt_a && !opt_d && argc == 2) {
		ತಲೆ_name = xstrdup(argv[1]);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opt_a && !opt_d && argc == 1) {
		ರಚನೆ ref_ಸ್ಥಿತಿಗಳು ಸ್ಥಿತಿಗಳು = REF_ಸ್ಥಿತಿಗಳು_INIT;
		get_remote_ref_ಸ್ಥಿತಿಗಳು(argv[0], &ಸ್ಥಿತಿಗಳು, GET_ತಲೆ_ಹೆಸರುಗಳು);
		ಆದರೆ (!ಸ್ಥಿತಿಗಳು.heads.nr)
			ಫಲಿತಾಂಶ |= error(_("Cannot determine remote HEAD"));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸ್ಥಿತಿಗಳು.heads.nr > 1) {
			ಫಲಿತಾಂಶ |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ಸ್ಥಿತಿಗಳು.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], ಸ್ಥಿತಿಗಳು.heads.ವಸ್ತುಗಳು[i].string);
		} ಇಲ್ಲದಿದ್ದರೆ
			ತಲೆ_name = xstrdup(ಸ್ಥಿತಿಗಳು.heads.ವಸ್ತುಗಳು[0].string);
		free_remote_ref_ಸ್ಥಿತಿಗಳು(&ಸ್ಥಿತಿಗಳು);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opt_d && !opt_a && argc == 1) {
		ಆದರೆ (refs_delete_ref(refs, ಖಾಲಿ, b_ತಲೆ.buf, ಖಾಲಿ, REF_NO_DEREF))
			ಫಲಿತಾಂಶ |= error(_("Could not delete %s"), b_ತಲೆ.buf);
	} ಇಲ್ಲದಿದ್ದರೆ
		usage_with_options(builtin_remote_sethead_usage, options);

	ಆದರೆ (!ತಲೆ_name)
		ಹೋಗು cleanup;
	strbuf_addf(&b_remote_ತಲೆ, "refs/remotes/%s/%s", argv[0], ತಲೆ_name);
	ಆದರೆ (!refs_ref_exists(refs, b_remote_ತಲೆ.buf)) {
		ಫಲಿತಾಂಶ |= error(_("Not a valid ref: %s"), b_remote_ತಲೆ.buf);
		ಹೋಗು cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_ತಲೆ.buf, b_remote_ತಲೆ.buf,
			"remote set-head", &b_local_ತಲೆ, 0);
	ಆದರೆ (was_detached == -1) {
		ಫಲಿತಾಂಶ |= error(_("Could not set up %s"), b_ತಲೆ.buf);
		ಹೋಗು cleanup;
	}
	ಆದರೆ (opt_a)
		report_ಗಣ_ತಲೆ_auto(argv[0], ತಲೆ_name, &b_local_ತಲೆ, was_detached);
	ಆದರೆ (remote->follow_remote_ತಲೆ == FOLLOW_REMOTE_ALWAYS) {
		ರಚನೆ strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_ಗಣ(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	ಬಿಡುಗಡೆ(ತಲೆ_name);
	strbuf_release(&b_ತಲೆ);
	strbuf_release(&b_remote_ತಲೆ);
	strbuf_release(&b_local_ತಲೆ);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prune_remote(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote, ಪೂರ್ಣಾಂಕ dry_ಸತತ)
{
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ರಚನೆ ref_ಸ್ಥಿತಿಗಳು ಸ್ಥಿತಿಗಳು = REF_ಸ್ಥಿತಿಗಳು_INIT;
	ರಚನೆ string_list refs_ಗಮ್ಯ_prune = STRING_LIST_INIT_NODUP;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

	get_remote_ref_ಸ್ಥಿತಿಗಳು(remote, &ಸ್ಥಿತಿಗಳು, GET_REF_ಸ್ಥಿತಿಗಳು);

	ಆದರೆ (!ಸ್ಥಿತಿಗಳು.stale.nr) {
		free_remote_ref_ಸ್ಥಿತಿಗಳು(&ಸ್ಥಿತಿಗಳು);
		ಹಿಂತಿರುಗಿ 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), ಸ್ಥಿತಿಗಳು.remote->url.v[0]);

	for_each_string_list_ಅಂಶ(ಅಂಶ, &ಸ್ಥಿತಿಗಳು.stale)
		string_list_append(&refs_ಗಮ್ಯ_prune, ಅಂಶ->util);
	string_list_ವಿಂಗಡಿಸು(&refs_ಗಮ್ಯ_prune);

	ಆದರೆ (!dry_ಸತತ)
		ಫಲಿತಾಂಶ |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_ಗಮ್ಯ_prune, 0);

	for_each_string_list_ಅಂಶ(ಅಂಶ, &ಸ್ಥಿತಿಗಳು.stale) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *refname = ಅಂಶ->util;

		ಆದರೆ (dry_ಸತತ)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		ಇಲ್ಲದಿದ್ದರೆ
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_ಸತತ, &refs_ಗಮ್ಯ_prune);

	string_list_clear(&refs_ಗಮ್ಯ_prune, 0);
	free_remote_ref_ಸ್ಥಿತಿಗಳು(&ಸ್ಥಿತಿಗಳು);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prune(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ dry_ಸತತ = 0, ಫಲಿತಾಂಶ = 0;
	ರಚನೆ option options[] = {
		OPT_DRY_ಸತತ(&dry_ಸತತ, N_("dry run")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_remote_prune_usage, 0);

	ಆದರೆ (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	ಗಾಗಿ (; argc; argc--, argv++)
		ಫಲಿತಾಂಶ |= prune_remote(*argv, dry_ಸತತ);

	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_remote_default(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ ಬಳಸದ,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx ಬಳಸದ,
			      ಶೂನ್ಯ *priv)
{
	ಆದರೆ (strcmp(ಕೀಲಿ, "remotes.default") == 0) {
		ಪೂರ್ಣಾಂಕ *ಸಿಕ್ಕಿತು_2 = priv;
		*ಸಿಕ್ಕಿತು_2 = 1;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		  ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i, prune = -1;
	ರಚನೆ option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_ಕೊನೆ()
	};
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;
	ಪೂರ್ಣಾಂಕ default_defined = 0;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "fetch");

	ಆದರೆ (prune != -1)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, prune ? "--prune" : "--no-prune");
	ಆದರೆ (verbose)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "-v");
	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--multiple");
	ಆದರೆ (argc < 2)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "default");
	ಗಾಗಿ (i = 1; i < argc; i++)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, argv[i]);

	ಆದರೆ (strcmp(cmd.ವಾದಗಳು.v[cmd.ವಾದಗಳು.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		ಆದರೆ (!default_defined) {
			strvec_ತೆಗೆ(&cmd.ವಾದಗಳು);
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--all");
		}
	}

	cmd.git_cmd = 1;
	ಹಿಂತಿರುಗಿ ಸತತ_command(&cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ remove_all_fetch_refspecs(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ)
{
	ಹಿಂತಿರುಗಿ repo_config_ಗಣ_multivar_gently(the_repository, ಕೀಲಿ, ಖಾಲಿ, ಖಾಲಿ,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_branches(ರಚನೆ remote *remote, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **branches,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remotename = remote->name;
	ಪೂರ್ಣಾಂಕ mirror = remote->mirror;
	ರಚನೆ strbuf refspec = STRBUF_INIT;

	ಗಾಗಿ (; *branches; branches++)
		ಕೂಡಿಸು_branch(ಕೀಲಿ, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗಣ_remote_branches(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remotename, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **branches,
				ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_ಬಹುಲಕ)
{
	ರಚನೆ strbuf ಕೀಲಿ = STRBUF_INIT;
	ರಚನೆ remote *remote;

	strbuf_addf(&ಕೀಲಿ, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	ಆದರೆ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		ನಿರ್ಗಮಿಸು(2);
	}

	ಆದರೆ (!ಕೂಡಿಸು_ಬಹುಲಕ && remove_all_fetch_refspecs(ಕೀಲಿ.buf)) {
		strbuf_release(&ಕೀಲಿ);
		ಹಿಂತಿರುಗಿ 1;
	}
	ಕೂಡಿಸು_branches(remote, branches, ಕೀಲಿ.buf);

	strbuf_release(&ಕೀಲಿ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗಣ_branches(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_ಬಹುಲಕ = 0;
	ರಚನೆ option options[] = {
		OPT_BOOL('\0', "add", &ಕೂಡಿಸು_ಬಹುಲಕ, N_("add branch")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_remote_setbranches_usage, 0);
	ಆದರೆ (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ ಗಣ_remote_branches(argv[0], argv + 1, ಕೂಡಿಸು_ಬಹುಲಕ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_url(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ತಳ್ಳು_ಬಹುಲಕ = 0, all_ಬಹುಲಕ = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remotename = ಖಾಲಿ;
	ರಚನೆ remote *remote;
	ರಚನೆ strvec *url;
	ರಚನೆ option options[] = {
		OPT_BOOL('\0', "push", &ತಳ್ಳು_ಬಹುಲಕ,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_ಬಹುಲಕ,
			 N_("return all URLs")),
		OPT_ಕೊನೆ()
	};
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_remote_geturl_usage, 0);

	ಆದರೆ (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	ಆದರೆ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		ನಿರ್ಗಮಿಸು(2);
	}

	url = ತಳ್ಳು_ಬಹುಲಕ ? ತಳ್ಳು_url_ದ_remote(remote) : &remote->url;

	ಆದರೆ (all_ಬಹುಲಕ) {
		ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} ಇಲ್ಲದಿದ್ದರೆ {
		printf_ln("%s", url->v[0]);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗಣ_url(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ತಳ್ಳು_ಬಹುಲಕ = 0, ಕೂಡಿಸು_ಬಹುಲಕ = 0, delete_ಬಹುಲಕ = 0;
	ಪೂರ್ಣಾಂಕ matches = 0, negative_matches = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remotename = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *newurl = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *oldurl = ಖಾಲಿ;
	ರಚನೆ remote *remote;
	regex_t old_regex;
	ರಚನೆ strvec *urlset;
	ರಚನೆ strbuf name_buf = STRBUF_INIT;
	ರಚನೆ option options[] = {
		OPT_BOOL('\0', "push", &ತಳ್ಳು_ಬಹುಲಕ,
			 N_("manipulate push URLs")),
		OPT_BOOL('\0', "add", &ಕೂಡಿಸು_ಬಹುಲಕ,
			 N_("add URL")),
		OPT_BOOL('\0', "delete", &delete_ಬಹುಲಕ,
			    N_("delete URLs")),
		OPT_ಕೊನೆ()
	};
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     builtin_remote_seturl_usage,
			     PARSE_OPT_KEEP_ARGV0);

	ಆದರೆ (ಕೂಡಿಸು_ಬಹುಲಕ && delete_ಬಹುಲಕ)
		die(_("--add --delete doesn't make sense"));

	ಆದರೆ (argc < 3 || argc > 4 || ((ಕೂಡಿಸು_ಬಹುಲಕ || delete_ಬಹುಲಕ) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	ಆದರೆ (argc > 3)
		oldurl = argv[3];

	ಆದರೆ (delete_ಬಹುಲಕ)
		oldurl = newurl;

	remote = remote_get(remotename);
	ಆದರೆ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		ನಿರ್ಗಮಿಸು(2);
	}

	ಆದರೆ (ತಳ್ಳು_ಬಹುಲಕ) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	ಆದರೆ ((!oldurl && !delete_ಬಹುಲಕ) || ಕೂಡಿಸು_ಬಹುಲಕ) {
		ಆದರೆ (ಕೂಡಿಸು_ಬಹುಲಕ)
			repo_config_ಗಣ_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		ಇಲ್ಲದಿದ್ದರೆ
			repo_config_ಗಣ(the_repository, name_buf.buf, newurl);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Old URL specified. Demand that one matches. */
	ಆದರೆ (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < urlset->nr; i++)
		ಆದರೆ (!regexec(&old_regex, urlset->v[i], 0, ಖಾಲಿ, 0))
			matches++;
		ಇಲ್ಲದಿದ್ದರೆ
			negative_matches++;
	ಆದರೆ (!delete_ಬಹುಲಕ && !matches)
		die(_("No such URL found: %s"), oldurl);
	ಆದರೆ (delete_ಬಹುಲಕ && !negative_matches && !ತಳ್ಳು_ಬಹುಲಕ)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	ಆದರೆ (!delete_ಬಹುಲಕ)
		repo_config_ಗಣ_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	ಇಲ್ಲದಿದ್ದರೆ
		repo_config_ಗಣ_multivar(the_repository, name_buf.buf, ಖಾಲಿ, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
ಹೊರಹೋಗು:
	strbuf_release(&name_buf);
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_remote(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo)
{
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, ಕೂಡಿಸು),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, ಖಾಲಿ, PARSE_OPT_NOCOMPLETE),
		OPT_SUBCOMMAND("remove", &fn, rm),
		OPT_SUBCOMMAND("set-head", &fn, ಗಣ_ತಲೆ),
		OPT_SUBCOMMAND("set-branches", &fn, ಗಣ_branches),
		OPT_SUBCOMMAND("get-url", &fn, get_url),
		OPT_SUBCOMMAND("set-url", &fn, ಗಣ_url),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("update", &fn, update),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, builtin_remote_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	ಆದರೆ (fn) {
		ಹಿಂತಿರುಗಿ !!fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		ಹಿಂತಿರುಗಿ !!show_all();
	}
}
