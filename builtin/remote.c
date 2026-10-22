#define USE_THE_REPOSITORY_VARIABLE

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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_remote_usage[] = {
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
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_remote_add_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	ರಿಕ್ತ
};

#define GET_REF_STATES (1<<0)
#define GET_HEAD_NAMES (1<<1)
#define GET_ತಳ್ಳು_REF_STATES (1<<2)

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ verbose;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fetch_remote(ಧ್ರುವ ವರ್ಣ *name)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

	strvec_ತಳ್ಳು(&cmd.args, "fetch");
	ಯದಿ (verbose)
		strvec_ತಳ್ಳು(&cmd.args, "-v");
	strvec_ತಳ್ಳು(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	ಯದಿ (run_command(&cmd))
		ಪ್ರತ್ಯಾಗಮ error(_("Could not fetch %s"), name);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಗಣನಾ {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#define MIRROR_NONE 0
#define MIRROR_FETCH 1
#define MIRROR_ತಳ್ಳು 2
#define MIRROR_BOTH (MIRROR_FETCH|MIRROR_ತಳ್ಳು)

ಸ್ಥಿರ ಶೂನ್ಯ add_branch(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ *branchname,
		       ಧ್ರುವ ವರ್ಣ *remotename, ಪೂರ್ಣಾಙ್ಕ mirror, ಸಂರಚನಾ strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	ಯದಿ (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	ಅನ್ಯಥಾ
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_set_multivar(the_repository, ಕೀಲಿ, tmp->buf, "^$", 0);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_mirror_opt(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ not)
{
	unsigned *mirror = opt->ಮೌಲ್ಯ;
	ಯದಿ (not)
		*mirror = MIRROR_NONE;
	ಅನ್ಯಥಾ ಯದಿ (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "push"))
		*mirror = MIRROR_ತಳ್ಳು;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ error(_("unknown --mirror argument: %s"), arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_remote_collision(ಸಂರಚನಾ remote *remote, ಶೂನ್ಯ *data)
{
	ಧ್ರುವ ವರ್ಣ *name = data;
	ಧ್ರುವ ವರ್ಣ *p;

	ಯದಿ (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	ಯದಿ (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ fetch = 0, fetch_tags = TAGS_DEFAULT;
	unsigned mirror = MIRROR_NONE;
	ಸಂರಚನಾ string_list track = STRING_LIST_INIT_NODUP;
	ಧ್ರುವ ವರ್ಣ *master = ರಿಕ್ತ;
	ಸಂರಚನಾ remote *remote;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *name, *url;
	ಪೂರ್ಣಾಙ್ಕ result = 0;

	ಸಂರಚನಾ option options[] = {
		OPT_BOOL('f', "fetch", &fetch, N_("fetch the remote branches")),
		OPT_SET_INT(0, "tags", &fetch_tags,
			    N_("import all tags and associated objects when fetching\n"
			       "or do not fetch any tag at all (--no-tags)"),
			    TAGS_SET),
		OPT_STRING_LIST('t', "track", &track, N_("branch"),
				N_("branch(es) to track")),
		OPT_STRING('m', "master", &master, N_("branch"), N_("master branch")),
		OPT_CALLBACK_F(0, "mirror", &mirror, "(push|fetch)",
			N_("set up remote as a mirror to push to or fetch from"),
			PARSE_OPT_OPTARG | PARSE_OPT_COMP_ARG, parse_mirror_opt),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_add_usage, 0);

	ಯದಿ (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	ಯದಿ (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	ಯದಿ (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	ಯದಿ (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		exit(3);
	}

	ಯದಿ (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (ಶೂನ್ಯ *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	ಯದಿ (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		ಯದಿ (track.nr == 0)
			string_list_append(&track, "*");
		ಪ್ರತಿ (size_t i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.ವಸ್ತುಗಳು[i].string,
				   name, mirror, &buf2);
		}
	}

	ಯದಿ (mirror & MIRROR_ತಳ್ಳು) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	ಯದಿ (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	ಯದಿ (fetch && fetch_remote(name)) {
		result = 1;
		ಗಚ್ಛ out;
	}

	ಯದಿ (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		ಯದಿ (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			result = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	ಪ್ರತ್ಯಾಗಮ result;
}

ಸಂರಚನಾ branch_info {
	ವರ್ಣ *remote_name;
	ಸಂರಚನಾ string_list merge;
	ಗಣನಾ rebase_type rebase;
	ವರ್ಣ *ತಳ್ಳು_remote_name;
};

ಸ್ಥಿರ ಸಂರಚನಾ string_list branch_list = STRING_LIST_INIT_DUP;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *abbrev_ref(ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *prefix)
{
	skip_prefix(name, prefix, &name);
	ಪ್ರತ್ಯಾಗಮ name;
}
#define abbrev_branch(name) abbrev_ref((name), "refs/heads/")

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ config_read_branches(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
				ಧ್ರುವ ಸಂರಚನಾ config_context *ctx UNUSED,
				ಶೂನ್ಯ *data UNUSED)
{
	ಧ್ರುವ ವರ್ಣ *orig_ಕೀಲಿ = ಕೀಲಿ;
	ವರ್ಣ *name;
	ಸಂರಚನಾ string_list_item *item;
	ಸಂರಚನಾ branch_info *info;
	ಗಣನಾ { REMOTE, MERGE, REBASE, ತಳ್ಳು_REMOTE } type;
	size_t ಕೀಲಿ_len;

	ಯದಿ (!starts_with(ಕೀಲಿ, "branch."))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಕೀಲಿ += strlen("branch.");
	ಯದಿ (strip_suffix(ಕೀಲಿ, ".remote", &ಕೀಲಿ_len))
		type = REMOTE;
	ಅನ್ಯಥಾ ಯದಿ (strip_suffix(ಕೀಲಿ, ".merge", &ಕೀಲಿ_len))
		type = MERGE;
	ಅನ್ಯಥಾ ಯದಿ (strip_suffix(ಕೀಲಿ, ".rebase", &ಕೀಲಿ_len))
		type = REBASE;
	ಅನ್ಯಥಾ ಯದಿ (strip_suffix(ಕೀಲಿ, ".pushremote", &ಕೀಲಿ_len))
		type = ತಳ್ಳು_REMOTE;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ 0;

	name = xmemdupz(ಕೀಲಿ, ಕೀಲಿ_len);
	item = string_list_ಸೇರಿಸು(&branch_list, name);

	ಯದಿ (!item->util)
		item->util = xcalloc(1, ಪರಿಮಾಣ(ಸಂರಚನಾ branch_info));
	info = item->util;
	ವಿಕಲ್ಪ (type) {
	ಪಕ್ಷ REMOTE:
		ಯದಿ (info->remote_name)
			warning(_("more than one %s"), orig_ಕೀಲಿ);
		info->remote_name = xstrdup(ಮೌಲ್ಯ);
		ಭಙ್ಗ;
	ಪಕ್ಷ MERGE: {
		ಧ್ರುವ ವರ್ಣ *space = strchr(ಮೌಲ್ಯ, ' ');
		ಮೌಲ್ಯ = abbrev_branch(ಮೌಲ್ಯ);
		ಯಾವತ್ (space) {
			ವರ್ಣ *merge;
			merge = xstrndup(ಮೌಲ್ಯ, space - ಮೌಲ್ಯ);
			string_list_append(&info->merge, merge);
			ಮೌಲ್ಯ = abbrev_branch(space + 1);
			space = strchr(ಮೌಲ್ಯ, ' ');
		}
		string_list_append(&info->merge, xstrdup(ಮೌಲ್ಯ));
		ಭಙ್ಗ;
	}
	ಪಕ್ಷ REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_ಮೌಲ್ಯ(ಮೌಲ್ಯ);
		ಯದಿ (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, ಮೌಲ್ಯ);
		ಭಙ್ಗ;
	ಪಕ್ಷ ತಳ್ಳು_REMOTE:
		ಯದಿ (info->ತಳ್ಳು_remote_name)
			warning(_("more than one %s"), orig_ಕೀಲಿ);
		info->ತಳ್ಳು_remote_name = xstrdup(ಮೌಲ್ಯ);
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("unexpected type=%d", type);
	}

	ಮುಕ್ತ(name);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_branches(ಶೂನ್ಯ)
{
	ಯದಿ (branch_list.nr)
		ಪ್ರತ್ಯಾಗಮ;
	repo_config(the_repository, config_read_branches, ರಿಕ್ತ);
}

ಸಂರಚನಾ ref_states {
	ಸಂರಚನಾ remote *remote;
	ಸಂರಚನಾ string_list new_refs, skipped, stale, tracked, heads, ತಳ್ಳು;
	ಪೂರ್ಣಾಙ್ಕ queried;
};

#define REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.ತಳ್ಳು = STRING_LIST_INIT_DUP, \
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_ref_states(ಧ್ರುವ ಸಂರಚನಾ ref *remote_refs, ಸಂರಚನಾ ref_states *states)
{
	ಸಂರಚನಾ ref *fetch_map = ರಿಕ್ತ, **tail = &fetch_map;
	ಸಂರಚನಾ ref *ref, *stale_refs;
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < states->remote->fetch.nr; i++)
		ಯದಿ (get_fetch_map(remote_refs, &states->remote->fetch.ವಸ್ತುಗಳು[i], &tail, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.ವಸ್ತುಗಳು[i].raw);

	ಪ್ರತಿ (ref = fetch_map; ref; ref = ref->next) {
		ಯದಿ (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		ಅನ್ಯಥಾ ಯದಿ (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		ಅನ್ಯಥಾ
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	ಪ್ರತಿ (ref = stale_refs; ref; ref = ref->next) {
		ಸಂರಚನಾ string_list_item *item =
			string_list_append(&states->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_sort(&states->new_refs);
	string_list_sort(&states->skipped);
	string_list_sort(&states->tracked);
	string_list_sort(&states->stale);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ ತಳ್ಳು_info {
	ವರ್ಣ *dest;
	ಪೂರ್ಣಾಙ್ಕ forced;
	ಗಣನಾ {
		ತಳ್ಳು_STATUS_CREATE = 0,
		ತಳ್ಳು_STATUS_DELETE,
		ತಳ್ಳು_STATUS_UPTODATE,
		ತಳ್ಳು_STATUS_FASTFORWARD,
		ತಳ್ಳು_STATUS_OUTOFDATE,
		ತಳ್ಳು_STATUS_NOTQUERIED
	} status;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_ತಳ್ಳು_ref_states(ಧ್ರುವ ಸಂರಚನಾ ref *remote_refs,
	ಸಂರಚನಾ ref_states *states)
{
	ಸಂರಚನಾ remote *remote = states->remote;
	ಸಂರಚನಾ ref *ref, *local_refs, *ತಳ್ಳು_map;
	ಯದಿ (remote->mirror)
		ಪ್ರತ್ಯಾಗಮ 0;

	local_refs = get_local_heads();
	ತಳ್ಳು_map = copy_ref_list(remote_refs);

	match_ತಳ್ಳು_refs(local_refs, &ತಳ್ಳು_map, &remote->ತಳ್ಳು, MATCH_REFS_NONE);

	ಪ್ರತಿ (ref = ತಳ್ಳು_map; ref; ref = ref->next) {
		ಸಂರಚನಾ string_list_item *item;
		ಸಂರಚನಾ ತಳ್ಳು_info *info;

		ಯದಿ (!ref->peer_ref)
			ಅನುವರ್ತ;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->ತಳ್ಳು,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, ಪರಿಮಾಣ(ಸಂರಚನಾ ತಳ್ಳು_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		ಯದಿ (is_null_oid(&ref->new_oid)) {
			info->status = ತಳ್ಳು_STATUS_DELETE;
		} ಅನ್ಯಥಾ ಯದಿ (oideq(&ref->old_oid, &ref->new_oid))
			info->status = ತಳ್ಳು_STATUS_UPTODATE;
		ಅನ್ಯಥಾ ಯದಿ (is_null_oid(&ref->old_oid))
			info->status = ತಳ್ಳು_STATUS_CREATE;
		ಅನ್ಯಥಾ ಯದಿ (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = ತಳ್ಳು_STATUS_FASTFORWARD;
		ಅನ್ಯಥಾ
			info->status = ತಳ್ಳು_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(ತಳ್ಳು_map);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_ತಳ್ಳು_ref_states_noquery(ಸಂರಚನಾ ref_states *states)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ remote *remote = states->remote;
	ಸಂರಚನಾ string_list_item *item;
	ಸಂರಚನಾ ತಳ್ಳು_info *info;

	ಯದಿ (remote->mirror)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!remote->ತಳ್ಳು.nr) {
		item = string_list_append(&states->ತಳ್ಳು, _("(matching)"));
		info = item->util = xcalloc(1, ಪರಿಮಾಣ(ಸಂರಚನಾ ತಳ್ಳು_info));
		info->status = ತಳ್ಳು_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	ಪ್ರತಿ (i = 0; i < remote->ತಳ್ಳು.nr; i++) {
		ಧ್ರುವ ಸಂರಚನಾ refspec_item *spec = &remote->ತಳ್ಳು.ವಸ್ತುಗಳು[i];
		ಯದಿ (spec->matching)
			item = string_list_append(&states->ತಳ್ಳು, _("(matching)"));
		ಅನ್ಯಥಾ ಯದಿ (strlen(spec->src))
			item = string_list_append(&states->ತಳ್ಳು, spec->src);
		ಅನ್ಯಥಾ
			item = string_list_append(&states->ತಳ್ಳು, _("(delete)"));

		info = item->util = xcalloc(1, ಪರಿಮಾಣ(ಸಂರಚನಾ ತಳ್ಳು_info));
		info->forced = spec->force;
		info->status = ತಳ್ಳು_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_head_names(ಧ್ರುವ ಸಂರಚನಾ ref *remote_refs, ಸಂರಚನಾ ref_states *states)
{
	ಸಂರಚನಾ ref *ref, *matches;
	ಸಂರಚನಾ ref *fetch_map = ರಿಕ್ತ, **fetch_map_tail = &fetch_map;
	ಸಂರಚನಾ refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (ವರ್ಣ *) "refs/heads/*",
		.dst = (ವರ್ಣ *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	ಪ್ರತಿ (ref = matches; ref; ref = ref->next)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ known_remote {
	ಸಂರಚನಾ known_remote *next;
	ಸಂರಚನಾ remote *remote;
};

ಸಂರಚನಾ known_remotes {
	ಸಂರಚನಾ remote *to_delete;
	ಸಂರಚನಾ known_remote *list;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_known_remote(ಸಂರಚನಾ remote *remote, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ known_remotes *all = cb_data;
	ಸಂರಚನಾ known_remote *r;

	ಯದಿ (!strcmp(all->to_delete->name, remote->name))
		ಪ್ರತ್ಯಾಗಮ 0;

	r = xmalloc(ಪರಿಮಾಣ(*r));
	r->remote = remote;
	r->next = all->list;
	all->list = r;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ branches_for_remote {
	ಸಂರಚನಾ remote *remote;
	ಸಂರಚನಾ string_list *branches, *skipped;
	ಸಂರಚನಾ known_remotes *keep;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_branch_for_removal(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ branches_for_remote *branches = cb_data;
	ಸಂರಚನಾ refspec_item refspec;
	ಸಂರಚನಾ known_remote *kr;

	memset(&refspec, 0, ಪರಿಮಾಣ(refspec));
	refspec.dst = (ವರ್ಣ *)ref->name;
	ಯದಿ (remote_find_tracking(branches->remote, &refspec))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಮುಕ್ತ(refspec.src);

	/* don't delete a branch if another remote also uses it */
	ಪ್ರತಿ (kr = branches->keep->list; kr; kr = kr->next) {
		memset(&refspec, 0, ಪರಿಮಾಣ(refspec));
		refspec.dst = (ವರ್ಣ *)ref->name;
		ಯದಿ (!remote_find_tracking(kr->remote, &refspec)) {
			ಮುಕ್ತ(refspec.src);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	ಯದಿ (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		ಯದಿ (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	string_list_append(branches->branches, ref->name);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ rename_info {
	ಧ್ರುವ ವರ್ಣ *old_name;
	ಧ್ರುವ ವರ್ಣ *new_name;
	ಸಂರಚನಾ ref_transaction *transaction;
	ಸಂರಚನಾ progress *progress;
	ಸಂರಚನಾ strbuf *err;
	uint32_t progress_nr;
	uint64_t index;
};

ಸ್ಥಿರ ಶೂನ್ಯ compute_renamed_ref(ಸಂರಚನಾ rename_info *rename,
				ಧ್ರುವ ವರ್ಣ *refname,
				ಸಂರಚನಾ strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, strlen("refs/remotes/"), strlen(rename->old_name),
		      rename->new_name, strlen(rename->new_name));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rename_one_reflog_entry(ಧ್ರುವ ವರ್ಣ *old_refname,
				   ಸಂರಚನಾ object_id *old_oid,
				   ಸಂರಚನಾ object_id *new_oid,
				   ಧ್ರುವ ವರ್ಣ *committer,
				   timestamp_t timestamp, ಪೂರ್ಣಾಙ್ಕ tz,
				   ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ rename_info *rename = cb_data;
	ಸಂರಚನಾ strbuf new_refname = STRBUF_INIT;
	ಸಂರಚನಾ strbuf identity = STRBUF_INIT;
	ಸಂರಚನಾ strbuf name = STRBUF_INIT;
	ಸಂರಚನಾ strbuf mail = STRBUF_INIT;
	ಸಂರಚನಾ ident_split ident;
	ಧ್ರುವ ವರ್ಣ *date;
	ಪೂರ್ಣಾಙ್ಕ error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	ಯದಿ (split_ident_ಸಾಲು(&ident, committer, strlen(committer)) < 0) {
		error = -1;
		ಗಚ್ಛ out;
	}

	strbuf_add(&name, ident.name_begin, ident.name_end - ident.name_begin);
	strbuf_add(&mail, ident.mail_begin, ident.mail_end - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_MODE(NORMAL));
	strbuf_addstr(&identity, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, identity.buf, ಸಂದೇಶ,
					      rename->index++, rename->err);

out:
	strbuf_release(&new_refname);
	strbuf_release(&identity);
	strbuf_release(&name);
	strbuf_release(&mail);
	ಪ್ರತ್ಯಾಗಮ error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rename_one_reflog(ಧ್ರುವ ವರ್ಣ *old_refname,
			     ಧ್ರುವ ಸಂರಚನಾ object_id *old_oid,
			     ಸಂರಚನಾ rename_info *rename)
{
	ಸಂರಚನಾ strbuf new_refname = STRBUF_INIT;
	ಸಂರಚನಾ strbuf message = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ error;

	ಯದಿ (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		ಪ್ರತ್ಯಾಗಮ 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, rename);
	ಯದಿ (error < 0)
		ಗಚ್ಛ out;

	compute_renamed_ref(rename, old_refname, &new_refname);

	/*
	 * Manually write the reflog entry for the now-renamed ref. We cannot
	 * rely on `rename_one_ref()` to do this for us as that would screw
	 * over order in which reflog entries are being written.
	 *
	 * Furthermore, we only append the entry in case the reference
	 * resolves. Missing references shouldn't have reflogs anyway.
	 */
	strbuf_addf(&message, "remote: renamed %s to %s", old_refname,
		    new_refname.buf);

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      old_oid, old_oid, git_committer_info(0),
					      message.buf, rename->index++, rename->err);
	ಯದಿ (error < 0)
		ಪ್ರತ್ಯಾಗಮ error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	ಪ್ರತ್ಯಾಗಮ error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rename_one_ref(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ strbuf new_referent = STRBUF_INIT;
	ಸಂರಚನಾ strbuf new_refname = STRBUF_INIT;
	ಸಂರಚನಾ rename_info *rename = cb_data;
	ಧ್ರುವ ಸಂರಚನಾ object_id *oid = ref->oid;
	ಧ್ರುವ ವರ್ಣ *referent = ref->target;
	ಪೂರ್ಣಾಙ್ಕ error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	ಯದಿ (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   ರಿಕ್ತ, ರಿಕ್ತ);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = ರಿಕ್ತ;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, ರಿಕ್ತ, rename->err);
	ಯದಿ (error < 0)
		ಗಚ್ಛ out;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : ರಿಕ್ತ, ರಿಕ್ತ,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       ರಿಕ್ತ, rename->err);
	ಯದಿ (error < 0)
		ಗಚ್ಛ out;

	error = rename_one_reflog(ref->name, oid, rename);
	ಯದಿ (error < 0)
		ಗಚ್ಛ out;

	display_progress(rename->progress, ++rename->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	ಪ್ರತ್ಯಾಗಮ error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ migrate_file(ಸಂರಚನಾ remote *remote)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	ಪ್ರತಿ (size_t i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < remote->ತಳ್ಳು.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->ತಳ್ಳು.ವಸ್ತುಗಳು[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.ವಸ್ತುಗಳು[i].raw, "^$", 0);
#ifndef WITH_BREAKING_CHANGES
	ಯದಿ (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	ಅನ್ಯಥಾ ಯದಿ (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#endif /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ ತಳ್ಳು_default_info
{
	ಧ್ರುವ ವರ್ಣ *old_name;
	ಗಣನಾ config_scope scope;
	ಸಂರಚನಾ strbuf origin;
	ಪೂರ್ಣಾಙ್ಕ linenr;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ config_read_ತಳ್ಳು_default(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
	ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಧ್ರುವ ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ_info *kvi = ctx->kvi;

	ಸಂರಚನಾ ತಳ್ಳು_default_info* info = cb;
	ಯದಿ (strcmp(ಕೀಲಿ, "remote.pushdefault") ||
	    !ಮೌಲ್ಯ || strcmp(ಮೌಲ್ಯ, info->old_name))
		ಪ್ರತ್ಯಾಗಮ 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_ತಳ್ಳು_default(ಧ್ರುವ ವರ್ಣ* old_name, ಧ್ರುವ ವರ್ಣ* new_name)
{
	ಸಂರಚನಾ ತಳ್ಳು_default_info ತಳ್ಳು_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_ತಳ್ಳು_default, &ತಳ್ಳು_default);
	ಯದಿ (ತಳ್ಳು_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	ಅನ್ಯಥಾ ಯದಿ (ತಳ್ಳು_default.scope >= CONFIG_SCOPE_LOCAL) {
		ಪೂರ್ಣಾಙ್ಕ result = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		ಯದಿ (new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		ಅನ್ಯಥಾ ಯದಿ (!new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} ಅನ್ಯಥಾ ಯದಿ (ತಳ್ಳು_default.scope >= CONFIG_SCOPE_SYSTEM) {
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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mv(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
	      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ show_progress = isatty(2);
	ಸಂರಚನಾ option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	ಸಂರಚನಾ remote *oldremote, *newremote;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	ಸಂರಚನಾ rename_info rename = {
		.err = &err,
	};
	ಪೂರ್ಣಾಙ್ಕ refspecs_need_update = 0;
	ಪೂರ್ಣಾಙ್ಕ result = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	ಯದಿ (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	ಯದಿ (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		exit(2);
	}

	ಯದಿ (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		ಪ್ರತ್ಯಾಗಮ migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	ಯದಿ (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		exit(3);
	}

	ಯದಿ (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	ಯದಿ (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		result = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		ಗಚ್ಛ out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!strstr(oldremote->fetch.ವಸ್ತುಗಳು[i].raw,
						old_remote_context.buf);

	ಯದಿ (refspecs_need_update) {
		ಸಂರಚನಾ refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		ಯದಿ (!rename.transaction)
			ಗಚ್ಛ out;

		ಯದಿ (show_progress)
			rename.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.prefix = buf.buf;

		result = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		ಯದಿ (result < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		result = ref_transaction_prepare(rename.transaction, &err);
		ಯದಿ (result < 0) {
			error("renaming remote references failed: %s", err.buf);
			ಯದಿ (result == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(ರಿಕ್ತ);
		}
	}

	ಯದಿ (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_set_multivar(the_repository, buf.buf, ರಿಕ್ತ, ರಿಕ್ತ, CONFIG_FLAGS_MULTI_REPLACE);
		ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < oldremote->fetch.nr; i++) {
			ವರ್ಣ *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.ವಸ್ತುಗಳು[i].raw);
			ptr = strstr(buf2.buf, old_remote_context.buf);
			ಯದಿ (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + strlen(":refs/remotes/"),
					      strlen(rename.old_name), rename.new_name,
					      strlen(rename.new_name));
			} ಅನ್ಯಥಾ
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_set_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	ಪ್ರತಿ (size_t i = 0; i < branch_list.nr; i++) {
		ಸಂರಚನಾ string_list_item *item = branch_list.ವಸ್ತುಗಳು + i;
		ಸಂರಚನಾ branch_info *info = item->util;
		ಯದಿ (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
		ಯದಿ (info->ತಳ್ಳು_remote_name && !strcmp(info->ತಳ್ಳು_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
	}

	ಯದಿ (refspecs_need_update) {
		result = ref_transaction_commit(rename.transaction, &err);
		ಯದಿ (result < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_progress(&rename.progress);

		handle_ತಳ್ಳು_default(rename.old_name, rename.new_name);
	}

out:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rm(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
	      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};
	ಸಂರಚನಾ remote *remote;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ known_remotes known_remotes = { ರಿಕ್ತ, ರಿಕ್ತ };
	ಸಂರಚನಾ string_list branches = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ string_list skipped = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ branches_for_remote cb_data;
	ಪೂರ್ಣಾಙ್ಕ result;

	memset(&cb_data, 0, ಪರಿಮಾಣ(cb_data));
	cb_data.branches = &branches;
	cb_data.skipped = &skipped;
	cb_data.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	ಯದಿ (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	ಯದಿ (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		exit(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	ಪ್ರತಿ (size_t i = 0; i < branch_list.nr; i++) {
		ಸಂರಚನಾ string_list_item *item = branch_list.ವಸ್ತುಗಳು + i;
		ಸಂರಚನಾ branch_info *info = item->util;
		ಯದಿ (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ಧ್ರುವ ವರ್ಣ *ಕೀಲಿಗಳು[] = { "remote", "merge", ರಿಕ್ತ }, **k;
			ಪ್ರತಿ (k = ಕೀಲಿಗಳು; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				result = repo_config_set_gently(the_repository, buf.buf, ರಿಕ್ತ);
				ಯದಿ (result && result != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		ಯದಿ (info->ತಳ್ಳು_remote_name && !strcmp(info->ತಳ್ಳು_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			result = repo_config_set_gently(the_repository, buf.buf, ರಿಕ್ತ);
			ಯದಿ (result && result != CONFIG_NOTHING_SET)
				die(_("could not unset '%s'"), buf.buf);
		}
	}

	/*
	 * We cannot just pass a function to for_each_ref() which deletes
	 * the branches one by one, since for_each_ref() relies on cached
	 * refs, which are invalidated when deleting a branch.
	 */
	cb_data.remote = remote;
	result = refs_for_each_ref(get_main_ref_store(the_repository),
				   add_branch_for_removal, &cb_data);
	strbuf_release(&buf);

	ಯದಿ (!result)
		result = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	ಯದಿ (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		ಪ್ರತಿ (size_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.ವಸ್ತುಗಳು[i].string);
	}
	string_list_clear(&skipped, 0);

	ಯದಿ (!result) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		ಯದಿ (repo_config_rename_section(the_repository, buf.buf, ರಿಕ್ತ) < 1) {
			result = error(_("Could not remove config section '%s'"), buf.buf);
			ಗಚ್ಛ out;
		}

		handle_ತಳ್ಳು_default(remote->name, ರಿಕ್ತ);
	}

out:
	ಪ್ರತಿ (ಸಂರಚನಾ known_remote *r = known_remotes.list; r;) {
		ಸಂರಚನಾ known_remote *next = r->next;
		ಮುಕ್ತ(r);
		r = next;
	}
	strbuf_release(&buf);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಶೂನ್ಯ clear_ತಳ್ಳು_info(ಶೂನ್ಯ *util, ಧ್ರುವ ವರ್ಣ *string UNUSED)
{
	ಸಂರಚನಾ ತಳ್ಳು_info *info = util;
	ಮುಕ್ತ(info->dest);
	ಮುಕ್ತ(info);
}

ಸ್ಥಿರ ಶೂನ್ಯ free_remote_ref_states(ಸಂರಚನಾ ref_states *states)
{
	string_list_clear(&states->new_refs, 0);
	string_list_clear(&states->skipped, 0);
	string_list_clear(&states->stale, 1);
	string_list_clear(&states->tracked, 0);
	string_list_clear(&states->heads, 0);
	string_list_clear_func(&states->ತಳ್ಳು, clear_ತಳ್ಳು_info);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ append_ref_to_tracked_list(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ ref_states *states = cb_data;
	ಸಂರಚನಾ refspec_item refspec;

	ಯದಿ (ref->flags & REF_ISSYMREF)
		ಪ್ರತ್ಯಾಗಮ 0;

	memset(&refspec, 0, ಪರಿಮಾಣ(refspec));
	refspec.dst = (ವರ್ಣ *)ref->name;
	ಯದಿ (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		ಮುಕ್ತ(refspec.src);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_remote_ref_states(ಧ್ರುವ ವರ್ಣ *name,
				 ಸಂರಚನಾ ref_states *states,
				 ಪೂರ್ಣಾಙ್ಕ query)
{
	states->remote = remote_get(name);
	ಯದಿ (!states->remote)
		ಪ್ರತ್ಯಾಗಮ error(_("No such remote: '%s'"), name);

	read_branches();

	ಯದಿ (query) {
		ಸಂರಚನಾ transport *transport;
		ಧ್ರುವ ಸಂರಚನಾ ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, ರಿಕ್ತ);

		states->queried = 1;
		ಯದಿ (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		ಯದಿ (query & GET_HEAD_NAMES)
			get_head_names(remote_refs, states);
		ಯದಿ (query & GET_ತಳ್ಳು_REF_STATES)
			get_ತಳ್ಳು_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} ಅನ್ಯಥಾ {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_ತಳ್ಳು_ref_states_noquery(states);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ show_info {
	ಸಂರಚನಾ string_list list;
	ಸಂರಚನಾ ref_states states;
	ಪೂರ್ಣಾಙ್ಕ width, width2;
	ಪೂರ್ಣಾಙ್ಕ any_rebase;
};

#define SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_remote_to_show_info(ಸಂರಚನಾ string_list_item *item, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ show_info *info = cb_data;
	ಪೂರ್ಣಾಙ್ಕ n = strlen(item->string);
	ಯದಿ (n > info->width)
		info->width = n;
	string_list_ಸೇರಿಸು(&info->list, item->string);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_remote_info_item(ಸಂರಚನಾ string_list_item *item, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ show_info *info = cb_data;
	ಸಂರಚನಾ ref_states *states = &info->states;
	ಧ್ರುವ ವರ್ಣ *name = item->string;

	ಯದಿ (states->queried) {
		ಧ್ರುವ ವರ್ಣ *fmt = "%s";
		ಧ್ರುವ ವರ್ಣ *arg = "";
		ಯದಿ (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} ಅನ್ಯಥಾ ಯದಿ (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		ಅನ್ಯಥಾ ಯದಿ (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		ಅನ್ಯಥಾ ಯದಿ (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		ಅನ್ಯಥಾ
			arg = _(" ???");
		printf("    %-*s", info->width, name);
		printf(fmt, arg);
		printf("\n");
	} ಅನ್ಯಥಾ
		printf("    %s\n", name);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_local_to_show_info(ಸಂರಚನಾ string_list_item *branch_item, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ show_info *show_info = cb_data;
	ಸಂರಚನಾ ref_states *states = &show_info->states;
	ಸಂರಚನಾ branch_info *branch_info = branch_item->util;
	ಸಂರಚನಾ string_list_item *item;
	ಪೂರ್ಣಾಙ್ಕ n;

	ಯದಿ (!branch_info->merge.nr || !branch_info->remote_name ||
	    strcmp(states->remote->name, branch_info->remote_name))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ ((n = strlen(branch_item->string)) > show_info->width)
		show_info->width = n;
	ಯದಿ (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_ಸೇರಿಸು(&show_info->list, branch_item->string);
	item->util = branch_info;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_local_info_item(ಸಂರಚನಾ string_list_item *item, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ show_info *show_info = cb_data;
	ಸಂರಚನಾ branch_info *branch_info = item->util;
	ಸಂರಚನಾ string_list *merge = &branch_info->merge;
	ಪೂರ್ಣಾಙ್ಕ width = show_info->width + 4;

	ಯದಿ (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	printf("    %-*s ", show_info->width, item->string);
	ಯದಿ (branch_info->rebase >= REBASE_TRUE) {
		ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ;
		ಯದಿ (branch_info->rebase == REBASE_INTERACTIVE)
			ಸಂದೇಶ = _("rebases interactively onto remote %s");
		ಅನ್ಯಥಾ ಯದಿ (branch_info->rebase == REBASE_MERGES)
			ಸಂದೇಶ = _("rebases interactively (with merges) onto "
				"remote %s");
		ಅನ್ಯಥಾ
			ಸಂದೇಶ = _("rebases onto remote %s");
		printf_ln(ಸಂದೇಶ, merge->ವಸ್ತುಗಳು[0].string);
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->ವಸ್ತುಗಳು[0].string);
		width++;
	} ಅನ್ಯಥಾ {
		printf_ln(_("merges with remote %s"), merge->ವಸ್ತುಗಳು[0].string);
	}
	ಪ್ರತಿ (size_t i = 1; i < merge->nr; i++)
		printf(_("%-*s    and with remote %s\n"), width, "",
		       merge->ವಸ್ತುಗಳು[i].string);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_ತಳ್ಳು_to_show_info(ಸಂರಚನಾ string_list_item *ತಳ್ಳು_item, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ show_info *show_info = cb_data;
	ಸಂರಚನಾ ತಳ್ಳು_info *ತಳ್ಳು_info = ತಳ್ಳು_item->util;
	ಸಂರಚನಾ string_list_item *item;
	ಪೂರ್ಣಾಙ್ಕ n;
	ಯದಿ ((n = strlen(ತಳ್ಳು_item->string)) > show_info->width)
		show_info->width = n;
	ಯದಿ ((n = strlen(ತಳ್ಳು_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, ತಳ್ಳು_item->string);
	item->util = ತಳ್ಳು_item->util;
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp_string_with_ತಳ್ಳು(ಧ್ರುವ ಶೂನ್ಯ *va, ಧ್ರುವ ಶೂನ್ಯ *vb)
{
	ಧ್ರುವ ಸಂರಚನಾ string_list_item *a = va;
	ಧ್ರುವ ಸಂರಚನಾ string_list_item *b = vb;
	ಧ್ರುವ ಸಂರಚನಾ ತಳ್ಳು_info *a_ತಳ್ಳು = a->util;
	ಧ್ರುವ ಸಂರಚನಾ ತಳ್ಳು_info *b_ತಳ್ಳು = b->util;
	ಪೂರ್ಣಾಙ್ಕ cmp = strcmp(a->string, b->string);
	ಪ್ರತ್ಯಾಗಮ cmp ? cmp : strcmp(a_ತಳ್ಳು->dest, b_ತಳ್ಳು->dest);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_ತಳ್ಳು_info_item(ಸಂರಚನಾ string_list_item *item, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ show_info *show_info = cb_data;
	ಸಂರಚನಾ ತಳ್ಳು_info *ತಳ್ಳು_info = item->util;
	ಧ್ರುವ ವರ್ಣ *src = item->string, *status = ರಿಕ್ತ;

	ವಿಕಲ್ಪ (ತಳ್ಳು_info->status) {
	ಪಕ್ಷ ತಳ್ಳು_STATUS_CREATE:
		status = _("create");
		ಭಙ್ಗ;
	ಪಕ್ಷ ತಳ್ಳು_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		ಭಙ್ಗ;
	ಪಕ್ಷ ತಳ್ಳು_STATUS_UPTODATE:
		status = _("up to date");
		ಭಙ್ಗ;
	ಪಕ್ಷ ತಳ್ಳು_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		ಭಙ್ಗ;
	ಪಕ್ಷ ತಳ್ಳು_STATUS_OUTOFDATE:
		status = _("local out of date");
		ಭಙ್ಗ;
	ಪಕ್ಷ ತಳ್ಳು_STATUS_NOTQUERIED:
		ಭಙ್ಗ;
	}
	ಯದಿ (status) {
		ಯದಿ (ತಳ್ಳು_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, ತಳ್ಳು_info->dest, status);
		ಅನ್ಯಥಾ
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, ತಳ್ಳು_info->dest, status);
	} ಅನ್ಯಥಾ {
		ಯದಿ (ತಳ್ಳು_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       ತಳ್ಳು_info->dest);
		ಅನ್ಯಥಾ
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       ತಳ್ಳು_info->dest);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_one_entry(ಸಂರಚನಾ remote *remote, ಶೂನ್ಯ *priv)
{
	ಸಂರಚನಾ string_list *list = priv;
	ಸಂರಚನಾ strbuf remote_info_buf = STRBUF_INIT;
	ಸಂರಚನಾ strvec *url;

	ಯದಿ (remote->url.nr > 0) {
		ಸಂರಚನಾ strbuf promisor_config = STRBUF_INIT;
		ಧ್ರುವ ವರ್ಣ *partial_clone_filter = ರಿಕ್ತ;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		ಯದಿ (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ರಿಕ್ತ);
	} ಅನ್ಯಥಾ
		string_list_append(list, remote->name)->util = ರಿಕ್ತ;
	url = ತಳ್ಳು_url_of_remote(remote);
	ಪ್ರತಿ (size_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ರಿಕ್ತ);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_all(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ string_list list = STRING_LIST_INIT_DUP;
	ಪೂರ್ಣಾಙ್ಕ result;

	result = for_each_remote(get_one_entry, &list);

	ಯದಿ (!result) {
		string_list_sort(&list);
		ಪ್ರತಿ (size_t i = 0; i < list.nr; i++) {
			ಸಂರಚನಾ string_list_item *item = list.ವಸ್ತುಗಳು + i;
			ಯದಿ (verbose)
				printf("%s\t%s\n", item->string,
					item->util ? (ಧ್ರುವ ವರ್ಣ *)item->util : "");
			ಅನ್ಯಥಾ {
				ಯದಿ (i && !strcmp((item - 1)->string, item->string))
					ಅನುವರ್ತ;
				printf("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ no_query = 0, result = 0, query_flag = 0;
	ಸಂರಚನಾ option options[] = {
		OPT_BOOL('n', ರಿಕ್ತ, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	ಸಂರಚನಾ show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	ಯದಿ (argc < 1)
		ಪ್ರತ್ಯಾಗಮ show_all();

	ಯದಿ (!no_query)
		query_flag = (GET_REF_STATES | GET_HEAD_NAMES | GET_ತಳ್ಳು_REF_STATES);

	ಪ್ರತಿ (; argc; argc--, argv++) {
		size_t i;
		ಸಂರಚನಾ strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = ತಳ್ಳು_url_of_remote(info.states.remote);
		ಪ್ರತಿ (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		ಯದಿ (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		ಯದಿ (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		ಅನ್ಯಥಾ ಯದಿ (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		ಅನ್ಯಥಾ ಯದಿ (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.ವಸ್ತುಗಳು[0].string);
		ಅನ್ಯಥಾ {
			printf(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			ಪ್ರತಿ (i = 0; i < info.states.heads.nr; i++)
				printf("    %s\n", info.states.heads.ವಸ್ತುಗಳು[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		ಯದಿ (info.list.nr)
			printf_ln(Q_("  Remote branch:%s",
				     "  Remote branches:%s",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_remote_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git pull info */
		info.width = 0;
		info.any_rebase = 0;
		for_each_string_list(&branch_list, add_local_to_show_info, &info);
		ಯದಿ (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		ಯದಿ (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.ತಳ್ಳು, add_ತಳ್ಳು_to_show_info, &info);
		QSORT(info.list.ವಸ್ತುಗಳು, info.list.nr, cmp_string_with_ತಳ್ಳು);
		ಯದಿ (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_ತಳ್ಳು_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಶೂನ್ಯ report_set_head_auto(ಧ್ರುವ ವರ್ಣ *remote, ಧ್ರುವ ವರ್ಣ *head_name,
			ಸಂರಚನಾ strbuf *b_local_head, ಪೂರ್ಣಾಙ್ಕ was_detached) {
	ಸಂರಚನಾ strbuf buf_prefix = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *prev_head = ರಿಕ್ತ;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_head->buf, buf_prefix.buf, &prev_head);

	ಯದಿ (prev_head && !strcmp(prev_head, head_name))
		printf(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, head_name);
	ಅನ್ಯಥಾ ಯದಿ (prev_head)
		printf(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_head, head_name);
	ಅನ್ಯಥಾ ಯದಿ (!b_local_head->len)
		printf(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, head_name);
	ಅನ್ಯಥಾ ಯದಿ (was_detached && b_local_head->len)
		printf(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	ಅನ್ಯಥಾ
		printf(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	strbuf_release(&buf_prefix);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ set_head(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ opt_a = 0, opt_d = 0, result = 0, was_detached;
	ಸಂರಚನಾ strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		b_local_head = STRBUF_INIT;
	ವರ್ಣ *head_name = ರಿಕ್ತ;
	ಸಂರಚನಾ ref_store *refs = get_main_ref_store(the_repository);
	ಸಂರಚನಾ remote *remote;

	ಸಂರಚನಾ option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	ಯದಿ (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_head, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	ಯದಿ (!opt_a && !opt_d && argc == 2) {
		head_name = xstrdup(argv[1]);
	} ಅನ್ಯಥಾ ಯದಿ (opt_a && !opt_d && argc == 1) {
		ಸಂರಚನಾ ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_HEAD_NAMES);
		ಯದಿ (!states.heads.nr)
			result |= error(_("Cannot determine remote HEAD"));
		ಅನ್ಯಥಾ ಯದಿ (states.heads.nr > 1) {
			result |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			ಪ್ರತಿ (size_t i = 0; i < states.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], states.heads.ವಸ್ತುಗಳು[i].string);
		} ಅನ್ಯಥಾ
			head_name = xstrdup(states.heads.ವಸ್ತುಗಳು[0].string);
		free_remote_ref_states(&states);
	} ಅನ್ಯಥಾ ಯದಿ (opt_d && !opt_a && argc == 1) {
		ಯದಿ (refs_delete_ref(refs, ರಿಕ್ತ, b_head.buf, ರಿಕ್ತ, REF_NO_DEREF))
			result |= error(_("Could not delete %s"), b_head.buf);
	} ಅನ್ಯಥಾ
		usage_with_options(builtin_remote_sethead_usage, options);

	ಯದಿ (!head_name)
		ಗಚ್ಛ cleanup;
	strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", argv[0], head_name);
	ಯದಿ (!refs_ref_exists(refs, b_remote_head.buf)) {
		result |= error(_("Not a valid ref: %s"), b_remote_head.buf);
		ಗಚ್ಛ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
			"remote set-head", &b_local_head, 0);
	ಯದಿ (was_detached == -1) {
		result |= error(_("Could not set up %s"), b_head.buf);
		ಗಚ್ಛ cleanup;
	}
	ಯದಿ (opt_a)
		report_set_head_auto(argv[0], head_name, &b_local_head, was_detached);
	ಯದಿ (remote->follow_remote_head == FOLLOW_REMOTE_ALWAYS) {
		ಸಂರಚನಾ strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_set(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	ಮುಕ್ತ(head_name);
	strbuf_release(&b_head);
	strbuf_release(&b_remote_head);
	strbuf_release(&b_local_head);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prune_remote(ಧ್ರುವ ವರ್ಣ *remote, ಪೂರ್ಣಾಙ್ಕ dry_run)
{
	ಪೂರ್ಣಾಙ್ಕ result = 0;
	ಸಂರಚನಾ ref_states states = REF_STATES_INIT;
	ಸಂರಚನಾ string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	ಯದಿ (!states.stale.nr) {
		free_remote_ref_states(&states);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	ಯದಿ (!dry_run)
		result |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		ಧ್ರುವ ವರ್ಣ *refname = item->util;

		ಯದಿ (dry_run)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		ಅನ್ಯಥಾ
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_run, &refs_to_prune);

	string_list_clear(&refs_to_prune, 0);
	free_remote_ref_states(&states);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prune(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ dry_run = 0, result = 0;
	ಸಂರಚನಾ option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	ಯದಿ (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	ಪ್ರತಿ (; argc; argc--, argv++)
		result |= prune_remote(*argv, dry_run);

	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_remote_default(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ UNUSED,
			      ಧ್ರುವ ಸಂರಚನಾ config_context *ctx UNUSED,
			      ಶೂನ್ಯ *priv)
{
	ಯದಿ (strcmp(ಕೀಲಿ, "remotes.default") == 0) {
		ಪೂರ್ಣಾಙ್ಕ *found = priv;
		*found = 1;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ update(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		  ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ i, prune = -1;
	ಸಂರಚನಾ option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;
	ಪೂರ್ಣಾಙ್ಕ default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_ತಳ್ಳು(&cmd.args, "fetch");

	ಯದಿ (prune != -1)
		strvec_ತಳ್ಳು(&cmd.args, prune ? "--prune" : "--no-prune");
	ಯದಿ (verbose)
		strvec_ತಳ್ಳು(&cmd.args, "-v");
	strvec_ತಳ್ಳು(&cmd.args, "--multiple");
	ಯದಿ (argc < 2)
		strvec_ತಳ್ಳು(&cmd.args, "default");
	ಪ್ರತಿ (i = 1; i < argc; i++)
		strvec_ತಳ್ಳು(&cmd.args, argv[i]);

	ಯದಿ (strcmp(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		ಯದಿ (!default_defined) {
			strvec_ತೆಗೆ(&cmd.args);
			strvec_ತಳ್ಳು(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	ಪ್ರತ್ಯಾಗಮ run_command(&cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ remove_all_fetch_refspecs(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ)
{
	ಪ್ರತ್ಯಾಗಮ repo_config_set_multivar_gently(the_repository, ಕೀಲಿ, ರಿಕ್ತ, ರಿಕ್ತ,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

ಸ್ಥಿರ ಶೂನ್ಯ add_branches(ಸಂರಚನಾ remote *remote, ಧ್ರುವ ವರ್ಣ **branches,
			 ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ)
{
	ಧ್ರುವ ವರ್ಣ *remotename = remote->name;
	ಪೂರ್ಣಾಙ್ಕ mirror = remote->mirror;
	ಸಂರಚನಾ strbuf refspec = STRBUF_INIT;

	ಪ್ರತಿ (; *branches; branches++)
		add_branch(ಕೀಲಿ, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ set_remote_branches(ಧ್ರುವ ವರ್ಣ *remotename, ಧ್ರುವ ವರ್ಣ **branches,
				ಪೂರ್ಣಾಙ್ಕ add_mode)
{
	ಸಂರಚನಾ strbuf ಕೀಲಿ = STRBUF_INIT;
	ಸಂರಚನಾ remote *remote;

	strbuf_addf(&ಕೀಲಿ, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	ಯದಿ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	ಯದಿ (!add_mode && remove_all_fetch_refspecs(ಕೀಲಿ.buf)) {
		strbuf_release(&ಕೀಲಿ);
		ಪ್ರತ್ಯಾಗಮ 1;
	}
	add_branches(remote, branches, ಕೀಲಿ.buf);

	strbuf_release(&ಕೀಲಿ);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ set_branches(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ add_mode = 0;
	ಸಂರಚನಾ option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	ಯದಿ (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = ರಿಕ್ತ;

	ಪ್ರತ್ಯಾಗಮ set_remote_branches(argv[0], argv + 1, add_mode);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_url(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ ತಳ್ಳು_mode = 0, all_mode = 0;
	ಧ್ರುವ ವರ್ಣ *remotename = ರಿಕ್ತ;
	ಸಂರಚನಾ remote *remote;
	ಸಂರಚನಾ strvec *url;
	ಸಂರಚನಾ option options[] = {
		OPT_BOOL('\0', "push", &ತಳ್ಳು_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	ಯದಿ (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	ಯದಿ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	url = ತಳ್ಳು_mode ? ತಳ್ಳು_url_of_remote(remote) : &remote->url;

	ಯದಿ (all_mode) {
		ಪ್ರತಿ (size_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} ಅನ್ಯಥಾ {
		printf_ln("%s", url->v[0]);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ set_url(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ ತಳ್ಳು_mode = 0, add_mode = 0, delete_mode = 0;
	ಪೂರ್ಣಾಙ್ಕ matches = 0, negative_matches = 0;
	ಧ್ರುವ ವರ್ಣ *remotename = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *newurl = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *oldurl = ರಿಕ್ತ;
	ಸಂರಚನಾ remote *remote;
	regex_t old_regex;
	ಸಂರಚನಾ strvec *urlset;
	ಸಂರಚನಾ strbuf name_buf = STRBUF_INIT;
	ಸಂರಚನಾ option options[] = {
		OPT_BOOL('\0', "push", &ತಳ್ಳು_mode,
			 N_("manipulate push URLs")),
		OPT_BOOL('\0', "add", &add_mode,
			 N_("add URL")),
		OPT_BOOL('\0', "delete", &delete_mode,
			    N_("delete URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_seturl_usage,
			     PARSE_OPT_KEEP_ARGV0);

	ಯದಿ (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	ಯದಿ (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	ಯದಿ (argc > 3)
		oldurl = argv[3];

	ಯದಿ (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	ಯದಿ (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	ಯದಿ (ತಳ್ಳು_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} ಅನ್ಯಥಾ {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	ಯದಿ ((!oldurl && !delete_mode) || add_mode) {
		ಯದಿ (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		ಅನ್ಯಥಾ
			repo_config_set(the_repository, name_buf.buf, newurl);
		ಗಚ್ಛ out;
	}

	/* Old URL specified. Demand that one matches. */
	ಯದಿ (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	ಪ್ರತಿ (size_t i = 0; i < urlset->nr; i++)
		ಯದಿ (!regexec(&old_regex, urlset->v[i], 0, ರಿಕ್ತ, 0))
			matches++;
		ಅನ್ಯಥಾ
			negative_matches++;
	ಯದಿ (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	ಯದಿ (delete_mode && !negative_matches && !ತಳ್ಳು_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	ಯದಿ (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	ಅನ್ಯಥಾ
		repo_config_set_multivar(the_repository, name_buf.buf, ರಿಕ್ತ, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_remote(ಪೂರ್ಣಾಙ್ಕ argc,
	       ಧ್ರುವ ವರ್ಣ **argv,
	       ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo)
{
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, ರಿಕ್ತ, PARSE_OPT_NOCOMPLETE),
		OPT_SUBCOMMAND("remove", &fn, rm),
		OPT_SUBCOMMAND("set-head", &fn, set_head),
		OPT_SUBCOMMAND("set-branches", &fn, set_branches),
		OPT_SUBCOMMAND("get-url", &fn, get_url),
		OPT_SUBCOMMAND("set-url", &fn, set_url),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("update", &fn, update),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_remote_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	ಯದಿ (fn) {
		ಪ್ರತ್ಯಾಗಮ !!fn(argc, argv, prefix, repo);
	} ಅನ್ಯಥಾ {
		ಯದಿ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		ಪ್ರತ್ಯಾಗಮ !!show_all();
	}
}
