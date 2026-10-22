#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "date.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "ident.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "transport.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "rebase.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "refspec.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "commit-reach.h"
#ଅନ୍ତର୍ଭାଵ "progress.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_remote_usage[] = {
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
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_remote_add_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	ଖାଲି
};

#define GET_REF_STATES (1<<0)
#define GET_HEAD_NAMES (1<<1)
#define GET_ଠେଲ_REF_STATES (1<<2)

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fetch_remote(ଅଚଳ ଅକ୍ଷର *name)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

	strvec_ଠେଲ(&cmd.args, "fetch");
	ଯଦି (verbose)
		strvec_ଠେଲ(&cmd.args, "-v");
	strvec_ଠେଲ(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	ଯଦି (run_command(&cmd))
		ଫେରନ୍ତୁ error(_("Could not fetch %s"), name);
	ଫେରନ୍ତୁ 0;
}

ଗଣନା {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#define MIRROR_NONE 0
#define MIRROR_FETCH 1
#define MIRROR_ଠେଲ 2
#define MIRROR_BOTH (MIRROR_FETCH|MIRROR_ଠେଲ)

ସ୍ଥିର ଶୂନ୍ୟ add_branch(ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ ଅକ୍ଷର *branchname,
		       ଅଚଳ ଅକ୍ଷର *remotename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mirror, ସଂରଚନା strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	ଯଦି (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	ଅନ୍ୟଥା
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_set_multivar(the_repository, ଚାବି, tmp->buf, "^$", 0);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_mirror_opt(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା not)
{
	unsigned *mirror = opt->ମୂଲ୍ୟ;
	ଯଦି (not)
		*mirror = MIRROR_NONE;
	ଅନ୍ୟଥା ଯଦି (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "push"))
		*mirror = MIRROR_ଠେଲ;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("unknown --mirror argument: %s"), arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_remote_collision(ସଂରଚନା remote *remote, ଶୂନ୍ୟ *data)
{
	ଅଚଳ ଅକ୍ଷର *name = data;
	ଅଚଳ ଅକ୍ଷର *p;

	ଯଦି (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	ଯଦି (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fetch = 0, fetch_tags = TAGS_DEFAULT;
	unsigned mirror = MIRROR_NONE;
	ସଂରଚନା string_list track = STRING_LIST_INIT_NODUP;
	ଅଚଳ ଅକ୍ଷର *master = ଖାଲି;
	ସଂରଚନା remote *remote;
	ସଂରଚନା strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *name, *url;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;

	ସଂରଚନା option options[] = {
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

	ଯଦି (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	ଯଦି (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	ଯଦି (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	ଯଦି (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		exit(3);
	}

	ଯଦି (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (ଶୂନ୍ୟ *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	ଯଦି (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		ଯଦି (track.nr == 0)
			string_list_append(&track, "*");
		ପାଇଁ (size_t i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.ବସ୍ତୁଗୁଡ଼ିକ[i].string,
				   name, mirror, &buf2);
		}
	}

	ଯଦି (mirror & MIRROR_ଠେଲ) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	ଯଦି (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	ଯଦି (fetch && fetch_remote(name)) {
		result = 1;
		ଯାଅ out;
	}

	ଯଦି (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		ଯଦି (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			result = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	ଫେରନ୍ତୁ result;
}

ସଂରଚନା branch_info {
	ଅକ୍ଷର *remote_name;
	ସଂରଚନା string_list merge;
	ଗଣନା rebase_type rebase;
	ଅକ୍ଷର *ଠେଲ_remote_name;
};

ସ୍ଥିର ସଂରଚନା string_list branch_list = STRING_LIST_INIT_DUP;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *abbrev_ref(ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *prefix)
{
	skip_prefix(name, prefix, &name);
	ଫେରନ୍ତୁ name;
}
#define abbrev_branch(name) abbrev_ref((name), "refs/heads/")

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା config_read_branches(ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
				ଅଚଳ ସଂରଚନା config_context *ctx UNUSED,
				ଶୂନ୍ୟ *data UNUSED)
{
	ଅଚଳ ଅକ୍ଷର *orig_ଚାବି = ଚାବି;
	ଅକ୍ଷର *name;
	ସଂରଚନା string_list_item *item;
	ସଂରଚନା branch_info *info;
	ଗଣନା { REMOTE, MERGE, REBASE, ଠେଲ_REMOTE } type;
	size_t ଚାବି_len;

	ଯଦି (!starts_with(ଚାବି, "branch."))
		ଫେରନ୍ତୁ 0;

	ଚାବି += strlen("branch.");
	ଯଦି (strip_suffix(ଚାବି, ".remote", &ଚାବି_len))
		type = REMOTE;
	ଅନ୍ୟଥା ଯଦି (strip_suffix(ଚାବି, ".merge", &ଚାବି_len))
		type = MERGE;
	ଅନ୍ୟଥା ଯଦି (strip_suffix(ଚାବି, ".rebase", &ଚାବି_len))
		type = REBASE;
	ଅନ୍ୟଥା ଯଦି (strip_suffix(ଚାବି, ".pushremote", &ଚାବି_len))
		type = ଠେଲ_REMOTE;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ 0;

	name = xmemdupz(ଚାବି, ଚାବି_len);
	item = string_list_ଭର୍ତ୍ତି(&branch_list, name);

	ଯଦି (!item->util)
		item->util = xcalloc(1, ଆକାର(ସଂରଚନା branch_info));
	info = item->util;
	ବାଛ (type) {
	ସ୍ଥିତି REMOTE:
		ଯଦି (info->remote_name)
			warning(_("more than one %s"), orig_ଚାବି);
		info->remote_name = xstrdup(ମୂଲ୍ୟ);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି MERGE: {
		ଅଚଳ ଅକ୍ଷର *space = strchr(ମୂଲ୍ୟ, ' ');
		ମୂଲ୍ୟ = abbrev_branch(ମୂଲ୍ୟ);
		ଯେତେବେଳେ (space) {
			ଅକ୍ଷର *merge;
			merge = xstrndup(ମୂଲ୍ୟ, space - ମୂଲ୍ୟ);
			string_list_append(&info->merge, merge);
			ମୂଲ୍ୟ = abbrev_branch(space + 1);
			space = strchr(ମୂଲ୍ୟ, ' ');
		}
		string_list_append(&info->merge, xstrdup(ମୂଲ୍ୟ));
		ଭାଙ୍ଗ;
	}
	ସ୍ଥିତି REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_ମୂଲ୍ୟ(ମୂଲ୍ୟ);
		ଯଦି (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, ମୂଲ୍ୟ);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି ଠେଲ_REMOTE:
		ଯଦି (info->ଠେଲ_remote_name)
			warning(_("more than one %s"), orig_ଚାବି);
		info->ଠେଲ_remote_name = xstrdup(ମୂଲ୍ୟ);
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unexpected type=%d", type);
	}

	ମୁକ୍ତ(name);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ read_branches(ଶୂନ୍ୟ)
{
	ଯଦି (branch_list.nr)
		ଫେରନ୍ତୁ;
	repo_config(the_repository, config_read_branches, ଖାଲି);
}

ସଂରଚନା ref_states {
	ସଂରଚନା remote *remote;
	ସଂରଚନା string_list new_refs, skipped, stale, tracked, heads, ଠେଲ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା queried;
};

#define REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.ଠେଲ = STRING_LIST_INIT_DUP, \
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ref_states(ଅଚଳ ସଂରଚନା ref *remote_refs, ସଂରଚନା ref_states *states)
{
	ସଂରଚନା ref *fetch_map = ଖାଲି, **tail = &fetch_map;
	ସଂରଚନା ref *ref, *stale_refs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < states->remote->fetch.nr; i++)
		ଯଦି (get_fetch_map(remote_refs, &states->remote->fetch.ବସ୍ତୁଗୁଡ଼ିକ[i], &tail, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.ବସ୍ତୁଗୁଡ଼ିକ[i].raw);

	ପାଇଁ (ref = fetch_map; ref; ref = ref->next) {
		ଯଦି (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		ଅନ୍ୟଥା ଯଦି (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		ଅନ୍ୟଥା
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	ପାଇଁ (ref = stale_refs; ref; ref = ref->next) {
		ସଂରଚନା string_list_item *item =
			string_list_append(&states->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_sort(&states->new_refs);
	string_list_sort(&states->skipped);
	string_list_sort(&states->tracked);
	string_list_sort(&states->stale);

	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା ଠେଲ_info {
	ଅକ୍ଷର *dest;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା forced;
	ଗଣନା {
		ଠେଲ_STATUS_CREATE = 0,
		ଠେଲ_STATUS_DELETE,
		ଠେଲ_STATUS_UPTODATE,
		ଠେଲ_STATUS_FASTFORWARD,
		ଠେଲ_STATUS_OUTOFDATE,
		ଠେଲ_STATUS_NOTQUERIED
	} status;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ଠେଲ_ref_states(ଅଚଳ ସଂରଚନା ref *remote_refs,
	ସଂରଚନା ref_states *states)
{
	ସଂରଚନା remote *remote = states->remote;
	ସଂରଚନା ref *ref, *local_refs, *ଠେଲ_map;
	ଯଦି (remote->mirror)
		ଫେରନ୍ତୁ 0;

	local_refs = get_local_heads();
	ଠେଲ_map = copy_ref_list(remote_refs);

	match_ଠେଲ_refs(local_refs, &ଠେଲ_map, &remote->ଠେଲ, MATCH_REFS_NONE);

	ପାଇଁ (ref = ଠେଲ_map; ref; ref = ref->next) {
		ସଂରଚନା string_list_item *item;
		ସଂରଚନା ଠେଲ_info *info;

		ଯଦି (!ref->peer_ref)
			ଜାରି;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->ଠେଲ,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, ଆକାର(ସଂରଚନା ଠେଲ_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		ଯଦି (is_null_oid(&ref->new_oid)) {
			info->status = ଠେଲ_STATUS_DELETE;
		} ଅନ୍ୟଥା ଯଦି (oideq(&ref->old_oid, &ref->new_oid))
			info->status = ଠେଲ_STATUS_UPTODATE;
		ଅନ୍ୟଥା ଯଦି (is_null_oid(&ref->old_oid))
			info->status = ଠେଲ_STATUS_CREATE;
		ଅନ୍ୟଥା ଯଦି (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = ଠେଲ_STATUS_FASTFORWARD;
		ଅନ୍ୟଥା
			info->status = ଠେଲ_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(ଠେଲ_map);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ଠେଲ_ref_states_noquery(ସଂରଚନା ref_states *states)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା remote *remote = states->remote;
	ସଂରଚନା string_list_item *item;
	ସଂରଚନା ଠେଲ_info *info;

	ଯଦି (remote->mirror)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!remote->ଠେଲ.nr) {
		item = string_list_append(&states->ଠେଲ, _("(matching)"));
		info = item->util = xcalloc(1, ଆକାର(ସଂରଚନା ଠେଲ_info));
		info->status = ଠେଲ_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	ପାଇଁ (i = 0; i < remote->ଠେଲ.nr; i++) {
		ଅଚଳ ସଂରଚନା refspec_item *spec = &remote->ଠେଲ.ବସ୍ତୁଗୁଡ଼ିକ[i];
		ଯଦି (spec->matching)
			item = string_list_append(&states->ଠେଲ, _("(matching)"));
		ଅନ୍ୟଥା ଯଦି (strlen(spec->src))
			item = string_list_append(&states->ଠେଲ, spec->src);
		ଅନ୍ୟଥା
			item = string_list_append(&states->ଠେଲ, _("(delete)"));

		info = item->util = xcalloc(1, ଆକାର(ସଂରଚନା ଠେଲ_info));
		info->forced = spec->force;
		info->status = ଠେଲ_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_head_names(ଅଚଳ ସଂରଚନା ref *remote_refs, ସଂରଚନା ref_states *states)
{
	ସଂରଚନା ref *ref, *matches;
	ସଂରଚନା ref *fetch_map = ଖାଲି, **fetch_map_tail = &fetch_map;
	ସଂରଚନା refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (ଅକ୍ଷର *) "refs/heads/*",
		.dst = (ଅକ୍ଷର *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	ପାଇଁ (ref = matches; ref; ref = ref->next)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା known_remote {
	ସଂରଚନା known_remote *next;
	ସଂରଚନା remote *remote;
};

ସଂରଚନା known_remotes {
	ସଂରଚନା remote *to_delete;
	ସଂରଚନା known_remote *list;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_known_remote(ସଂରଚନା remote *remote, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା known_remotes *all = cb_data;
	ସଂରଚନା known_remote *r;

	ଯଦି (!strcmp(all->to_delete->name, remote->name))
		ଫେରନ୍ତୁ 0;

	r = xmalloc(ଆକାର(*r));
	r->remote = remote;
	r->next = all->list;
	all->list = r;
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା branches_for_remote {
	ସଂରଚନା remote *remote;
	ସଂରଚନା string_list *branches, *skipped;
	ସଂରଚନା known_remotes *keep;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_branch_for_removal(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା branches_for_remote *branches = cb_data;
	ସଂରଚନା refspec_item refspec;
	ସଂରଚନା known_remote *kr;

	memset(&refspec, 0, ଆକାର(refspec));
	refspec.dst = (ଅକ୍ଷର *)ref->name;
	ଯଦି (remote_find_tracking(branches->remote, &refspec))
		ଫେରନ୍ତୁ 0;
	ମୁକ୍ତ(refspec.src);

	/* don't delete a branch if another remote also uses it */
	ପାଇଁ (kr = branches->keep->list; kr; kr = kr->next) {
		memset(&refspec, 0, ଆକାର(refspec));
		refspec.dst = (ଅକ୍ଷର *)ref->name;
		ଯଦି (!remote_find_tracking(kr->remote, &refspec)) {
			ମୁକ୍ତ(refspec.src);
			ଫେରନ୍ତୁ 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	ଯଦି (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		ଯଦି (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		ଫେରନ୍ତୁ 0;
	}

	string_list_append(branches->branches, ref->name);

	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା rename_info {
	ଅଚଳ ଅକ୍ଷର *old_name;
	ଅଚଳ ଅକ୍ଷର *new_name;
	ସଂରଚନା ref_transaction *transaction;
	ସଂରଚନା progress *progress;
	ସଂରଚନା strbuf *err;
	uint32_t progress_nr;
	uint64_t index;
};

ସ୍ଥିର ଶୂନ୍ୟ compute_renamed_ref(ସଂରଚନା rename_info *rename,
				ଅଚଳ ଅକ୍ଷର *refname,
				ସଂରଚନା strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, strlen("refs/remotes/"), strlen(rename->old_name),
		      rename->new_name, strlen(rename->new_name));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rename_one_reflog_entry(ଅଚଳ ଅକ୍ଷର *old_refname,
				   ସଂରଚନା object_id *old_oid,
				   ସଂରଚନା object_id *new_oid,
				   ଅଚଳ ଅକ୍ଷର *committer,
				   timestamp_t timestamp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା tz,
				   ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା rename_info *rename = cb_data;
	ସଂରଚନା strbuf new_refname = STRBUF_INIT;
	ସଂରଚନା strbuf identity = STRBUF_INIT;
	ସଂରଚନା strbuf name = STRBUF_INIT;
	ସଂରଚନା strbuf mail = STRBUF_INIT;
	ସଂରଚନା ident_split ident;
	ଅଚଳ ଅକ୍ଷର *date;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	ଯଦି (split_ident_ଧାଡ଼ି(&ident, committer, strlen(committer)) < 0) {
		error = -1;
		ଯାଅ out;
	}

	strbuf_add(&name, ident.name_begin, ident.name_end - ident.name_begin);
	strbuf_add(&mail, ident.mail_begin, ident.mail_end - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_MODE(NORMAL));
	strbuf_addstr(&identity, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, identity.buf, ସନ୍ଦେଶ,
					      rename->index++, rename->err);

out:
	strbuf_release(&new_refname);
	strbuf_release(&identity);
	strbuf_release(&name);
	strbuf_release(&mail);
	ଫେରନ୍ତୁ error;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rename_one_reflog(ଅଚଳ ଅକ୍ଷର *old_refname,
			     ଅଚଳ ସଂରଚନା object_id *old_oid,
			     ସଂରଚନା rename_info *rename)
{
	ସଂରଚନା strbuf new_refname = STRBUF_INIT;
	ସଂରଚନା strbuf message = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା error;

	ଯଦି (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		ଫେରନ୍ତୁ 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, rename);
	ଯଦି (error < 0)
		ଯାଅ out;

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
	ଯଦି (error < 0)
		ଫେରନ୍ତୁ error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	ଫେରନ୍ତୁ error;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rename_one_ref(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା strbuf new_referent = STRBUF_INIT;
	ସଂରଚନା strbuf new_refname = STRBUF_INIT;
	ସଂରଚନା rename_info *rename = cb_data;
	ଅଚଳ ସଂରଚନା object_id *oid = ref->oid;
	ଅଚଳ ଅକ୍ଷର *referent = ref->target;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	ଯଦି (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   ଖାଲି, ଖାଲି);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = ଖାଲି;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, ଖାଲି, rename->err);
	ଯଦି (error < 0)
		ଯାଅ out;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : ଖାଲି, ଖାଲି,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       ଖାଲି, rename->err);
	ଯଦି (error < 0)
		ଯାଅ out;

	error = rename_one_reflog(ref->name, oid, rename);
	ଯଦି (error < 0)
		ଯାଅ out;

	display_progress(rename->progress, ++rename->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	ଫେରନ୍ତୁ error;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା migrate_file(ସଂରଚନା remote *remote)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	ପାଇଁ (size_t i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < remote->ଠେଲ.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->ଠେଲ.ବସ୍ତୁଗୁଡ଼ିକ[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.ବସ୍ତୁଗୁଡ଼ିକ[i].raw, "^$", 0);
#ifndef WITH_BREAKING_CHANGES
	ଯଦି (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	ଅନ୍ୟଥା ଯଦି (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#endif /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା ଠେଲ_default_info
{
	ଅଚଳ ଅକ୍ଷର *old_name;
	ଗଣନା config_scope scope;
	ସଂରଚନା strbuf origin;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା linenr;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା config_read_ଠେଲ_default(ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
	ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଅଚଳ ସଂରଚନା ଚାବି_ମୂଲ୍ୟ_info *kvi = ctx->kvi;

	ସଂରଚନା ଠେଲ_default_info* info = cb;
	ଯଦି (strcmp(ଚାବି, "remote.pushdefault") ||
	    !ମୂଲ୍ୟ || strcmp(ମୂଲ୍ୟ, info->old_name))
		ଫେରନ୍ତୁ 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ handle_ଠେଲ_default(ଅଚଳ ଅକ୍ଷର* old_name, ଅଚଳ ଅକ୍ଷର* new_name)
{
	ସଂରଚନା ଠେଲ_default_info ଠେଲ_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_ଠେଲ_default, &ଠେଲ_default);
	ଯଦି (ଠେଲ_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	ଅନ୍ୟଥା ଯଦି (ଠେଲ_default.scope >= CONFIG_SCOPE_LOCAL) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		ଯଦି (new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		ଅନ୍ୟଥା ଯଦି (!new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} ଅନ୍ୟଥା ଯଦି (ଠେଲ_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(ଠେଲ_default.scope),
			ଠେଲ_default.origin.buf, ଠେଲ_default.linenr,
			old_name);
	}

	strbuf_release(&ଠେଲ_default.origin);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mv(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
	      ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_progress = isatty(2);
	ସଂରଚନା option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	ସଂରଚନା remote *oldremote, *newremote;
	ସଂରଚନା strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	ସଂରଚନା rename_info rename = {
		.err = &err,
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା refspecs_need_update = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	ଯଦି (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	ଯଦି (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		exit(2);
	}

	ଯଦି (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		ଫେରନ୍ତୁ migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	ଯଦି (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		exit(3);
	}

	ଯଦି (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	ଯଦି (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		result = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		ଯାଅ out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!strstr(oldremote->fetch.ବସ୍ତୁଗୁଡ଼ିକ[i].raw,
						old_remote_context.buf);

	ଯଦି (refspecs_need_update) {
		ସଂରଚନା refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		ଯଦି (!rename.transaction)
			ଯାଅ out;

		ଯଦି (show_progress)
			rename.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.prefix = buf.buf;

		result = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		ଯଦି (result < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		result = ref_transaction_prepare(rename.transaction, &err);
		ଯଦି (result < 0) {
			error("renaming remote references failed: %s", err.buf);
			ଯଦି (result == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(ଖାଲି);
		}
	}

	ଯଦି (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_set_multivar(the_repository, buf.buf, ଖାଲି, ଖାଲି, CONFIG_FLAGS_MULTI_REPLACE);
		ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < oldremote->fetch.nr; i++) {
			ଅକ୍ଷର *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.ବସ୍ତୁଗୁଡ଼ିକ[i].raw);
			ptr = strstr(buf2.buf, old_remote_context.buf);
			ଯଦି (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + strlen(":refs/remotes/"),
					      strlen(rename.old_name), rename.new_name,
					      strlen(rename.new_name));
			} ଅନ୍ୟଥା
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_set_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	ପାଇଁ (size_t i = 0; i < branch_list.nr; i++) {
		ସଂରଚନା string_list_item *item = branch_list.ବସ୍ତୁଗୁଡ଼ିକ + i;
		ସଂରଚନା branch_info *info = item->util;
		ଯଦି (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
		ଯଦି (info->ଠେଲ_remote_name && !strcmp(info->ଠେଲ_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
	}

	ଯଦି (refspecs_need_update) {
		result = ref_transaction_commit(rename.transaction, &err);
		ଯଦି (result < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_progress(&rename.progress);

		handle_ଠେଲ_default(rename.old_name, rename.new_name);
	}

out:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rm(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
	      ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option options[] = {
		OPT_END()
	};
	ସଂରଚନା remote *remote;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା known_remotes known_remotes = { ଖାଲି, ଖାଲି };
	ସଂରଚନା string_list branches = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list skipped = STRING_LIST_INIT_DUP;
	ସଂରଚନା branches_for_remote cb_data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;

	memset(&cb_data, 0, ଆକାର(cb_data));
	cb_data.branches = &branches;
	cb_data.skipped = &skipped;
	cb_data.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	ଯଦି (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	ଯଦି (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		exit(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	ପାଇଁ (size_t i = 0; i < branch_list.nr; i++) {
		ସଂରଚନା string_list_item *item = branch_list.ବସ୍ତୁଗୁଡ଼ିକ + i;
		ସଂରଚନା branch_info *info = item->util;
		ଯଦି (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ଅଚଳ ଅକ୍ଷର *ଚାବିଗୁଡ଼ିକ[] = { "remote", "merge", ଖାଲି }, **k;
			ପାଇଁ (k = ଚାବିଗୁଡ଼ିକ; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				result = repo_config_set_gently(the_repository, buf.buf, ଖାଲି);
				ଯଦି (result && result != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		ଯଦି (info->ଠେଲ_remote_name && !strcmp(info->ଠେଲ_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			result = repo_config_set_gently(the_repository, buf.buf, ଖାଲି);
			ଯଦି (result && result != CONFIG_NOTHING_SET)
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

	ଯଦି (!result)
		result = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	ଯଦି (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		ପାଇଁ (size_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.ବସ୍ତୁଗୁଡ଼ିକ[i].string);
	}
	string_list_clear(&skipped, 0);

	ଯଦି (!result) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		ଯଦି (repo_config_rename_section(the_repository, buf.buf, ଖାଲି) < 1) {
			result = error(_("Could not remove config section '%s'"), buf.buf);
			ଯାଅ out;
		}

		handle_ଠେଲ_default(remote->name, ଖାଲି);
	}

out:
	ପାଇଁ (ସଂରଚନା known_remote *r = known_remotes.list; r;) {
		ସଂରଚନା known_remote *next = r->next;
		ମୁକ୍ତ(r);
		r = next;
	}
	strbuf_release(&buf);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ଶୂନ୍ୟ clear_ଠେଲ_info(ଶୂନ୍ୟ *util, ଅଚଳ ଅକ୍ଷର *string UNUSED)
{
	ସଂରଚନା ଠେଲ_info *info = util;
	ମୁକ୍ତ(info->dest);
	ମୁକ୍ତ(info);
}

ସ୍ଥିର ଶୂନ୍ୟ free_remote_ref_states(ସଂରଚନା ref_states *states)
{
	string_list_clear(&states->new_refs, 0);
	string_list_clear(&states->skipped, 0);
	string_list_clear(&states->stale, 1);
	string_list_clear(&states->tracked, 0);
	string_list_clear(&states->heads, 0);
	string_list_clear_func(&states->ଠେଲ, clear_ଠେଲ_info);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା append_ref_to_tracked_list(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା ref_states *states = cb_data;
	ସଂରଚନା refspec_item refspec;

	ଯଦି (ref->flags & REF_ISSYMREF)
		ଫେରନ୍ତୁ 0;

	memset(&refspec, 0, ଆକାର(refspec));
	refspec.dst = (ଅକ୍ଷର *)ref->name;
	ଯଦି (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		ମୁକ୍ତ(refspec.src);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_remote_ref_states(ଅଚଳ ଅକ୍ଷର *name,
				 ସଂରଚନା ref_states *states,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା query)
{
	states->remote = remote_get(name);
	ଯଦି (!states->remote)
		ଫେରନ୍ତୁ error(_("No such remote: '%s'"), name);

	read_branches();

	ଯଦି (query) {
		ସଂରଚନା transport *transport;
		ଅଚଳ ସଂରଚନା ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, ଖାଲି);

		states->queried = 1;
		ଯଦି (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		ଯଦି (query & GET_HEAD_NAMES)
			get_head_names(remote_refs, states);
		ଯଦି (query & GET_ଠେଲ_REF_STATES)
			get_ଠେଲ_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} ଅନ୍ୟଥା {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_ଠେଲ_ref_states_noquery(states);
	}

	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା show_info {
	ସଂରଚନା string_list list;
	ସଂରଚନା ref_states states;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, width2;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା any_rebase;
};

#define SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_remote_to_show_info(ସଂରଚନା string_list_item *item, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା show_info *info = cb_data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = strlen(item->string);
	ଯଦି (n > info->width)
		info->width = n;
	string_list_ଭର୍ତ୍ତି(&info->list, item->string);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_remote_info_item(ସଂରଚନା string_list_item *item, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା show_info *info = cb_data;
	ସଂରଚନା ref_states *states = &info->states;
	ଅଚଳ ଅକ୍ଷର *name = item->string;

	ଯଦି (states->queried) {
		ଅଚଳ ଅକ୍ଷର *fmt = "%s";
		ଅଚଳ ଅକ୍ଷର *arg = "";
		ଯଦି (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} ଅନ୍ୟଥା ଯଦି (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		ଅନ୍ୟଥା ଯଦି (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		ଅନ୍ୟଥା ଯଦି (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		ଅନ୍ୟଥା
			arg = _(" ???");
		printf("    %-*s", info->width, name);
		printf(fmt, arg);
		printf("\n");
	} ଅନ୍ୟଥା
		printf("    %s\n", name);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_local_to_show_info(ସଂରଚନା string_list_item *branch_item, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା show_info *show_info = cb_data;
	ସଂରଚନା ref_states *states = &show_info->states;
	ସଂରଚନା branch_info *branch_info = branch_item->util;
	ସଂରଚନା string_list_item *item;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା n;

	ଯଦି (!branch_info->merge.nr || !branch_info->remote_name ||
	    strcmp(states->remote->name, branch_info->remote_name))
		ଫେରନ୍ତୁ 0;
	ଯଦି ((n = strlen(branch_item->string)) > show_info->width)
		show_info->width = n;
	ଯଦି (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_ଭର୍ତ୍ତି(&show_info->list, branch_item->string);
	item->util = branch_info;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_local_info_item(ସଂରଚନା string_list_item *item, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା show_info *show_info = cb_data;
	ସଂରଚନା branch_info *branch_info = item->util;
	ସଂରଚନା string_list *merge = &branch_info->merge;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = show_info->width + 4;

	ଯଦି (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		ଫେରନ୍ତୁ 0;
	}

	printf("    %-*s ", show_info->width, item->string);
	ଯଦି (branch_info->rebase >= REBASE_TRUE) {
		ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ;
		ଯଦି (branch_info->rebase == REBASE_INTERACTIVE)
			ସନ୍ଦେଶ = _("rebases interactively onto remote %s");
		ଅନ୍ୟଥା ଯଦି (branch_info->rebase == REBASE_MERGES)
			ସନ୍ଦେଶ = _("rebases interactively (with merges) onto "
				"remote %s");
		ଅନ୍ୟଥା
			ସନ୍ଦେଶ = _("rebases onto remote %s");
		printf_ln(ସନ୍ଦେଶ, merge->ବସ୍ତୁଗୁଡ଼ିକ[0].string);
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->ବସ୍ତୁଗୁଡ଼ିକ[0].string);
		width++;
	} ଅନ୍ୟଥା {
		printf_ln(_("merges with remote %s"), merge->ବସ୍ତୁଗୁଡ଼ିକ[0].string);
	}
	ପାଇଁ (size_t i = 1; i < merge->nr; i++)
		printf(_("%-*s    and with remote %s\n"), width, "",
		       merge->ବସ୍ତୁଗୁଡ଼ିକ[i].string);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_ଠେଲ_to_show_info(ସଂରଚନା string_list_item *ଠେଲ_item, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା show_info *show_info = cb_data;
	ସଂରଚନା ଠେଲ_info *ଠେଲ_info = ଠେଲ_item->util;
	ସଂରଚନା string_list_item *item;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା n;
	ଯଦି ((n = strlen(ଠେଲ_item->string)) > show_info->width)
		show_info->width = n;
	ଯଦି ((n = strlen(ଠେଲ_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, ଠେଲ_item->string);
	item->util = ଠେଲ_item->util;
	ଫେରନ୍ତୁ 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp_string_with_ଠେଲ(ଅଚଳ ଶୂନ୍ୟ *va, ଅଚଳ ଶୂନ୍ୟ *vb)
{
	ଅଚଳ ସଂରଚନା string_list_item *a = va;
	ଅଚଳ ସଂରଚନା string_list_item *b = vb;
	ଅଚଳ ସଂରଚନା ଠେଲ_info *a_ଠେଲ = a->util;
	ଅଚଳ ସଂରଚନା ଠେଲ_info *b_ଠେଲ = b->util;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = strcmp(a->string, b->string);
	ଫେରନ୍ତୁ cmp ? cmp : strcmp(a_ଠେଲ->dest, b_ଠେଲ->dest);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_ଠେଲ_info_item(ସଂରଚନା string_list_item *item, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା show_info *show_info = cb_data;
	ସଂରଚନା ଠେଲ_info *ଠେଲ_info = item->util;
	ଅଚଳ ଅକ୍ଷର *src = item->string, *status = ଖାଲି;

	ବାଛ (ଠେଲ_info->status) {
	ସ୍ଥିତି ଠେଲ_STATUS_CREATE:
		status = _("create");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି ଠେଲ_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି ଠେଲ_STATUS_UPTODATE:
		status = _("up to date");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି ଠେଲ_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି ଠେଲ_STATUS_OUTOFDATE:
		status = _("local out of date");
		ଭାଙ୍ଗ;
	ସ୍ଥିତି ଠେଲ_STATUS_NOTQUERIED:
		ଭାଙ୍ଗ;
	}
	ଯଦି (status) {
		ଯଦି (ଠେଲ_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, ଠେଲ_info->dest, status);
		ଅନ୍ୟଥା
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, ଠେଲ_info->dest, status);
	} ଅନ୍ୟଥା {
		ଯଦି (ଠେଲ_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       ଠେଲ_info->dest);
		ଅନ୍ୟଥା
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       ଠେଲ_info->dest);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_one_entry(ସଂରଚନା remote *remote, ଶୂନ୍ୟ *priv)
{
	ସଂରଚନା string_list *list = priv;
	ସଂରଚନା strbuf remote_info_buf = STRBUF_INIT;
	ସଂରଚନା strvec *url;

	ଯଦି (remote->url.nr > 0) {
		ସଂରଚନା strbuf promisor_config = STRBUF_INIT;
		ଅଚଳ ଅକ୍ଷର *partial_clone_filter = ଖାଲି;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		ଯଦି (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ଖାଲି);
	} ଅନ୍ୟଥା
		string_list_append(list, remote->name)->util = ଖାଲି;
	url = ଠେଲ_url_of_remote(remote);
	ପାଇଁ (size_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, ଖାଲି);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_all(ଶୂନ୍ୟ)
{
	ସଂରଚନା string_list list = STRING_LIST_INIT_DUP;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;

	result = for_each_remote(get_one_entry, &list);

	ଯଦି (!result) {
		string_list_sort(&list);
		ପାଇଁ (size_t i = 0; i < list.nr; i++) {
			ସଂରଚନା string_list_item *item = list.ବସ୍ତୁଗୁଡ଼ିକ + i;
			ଯଦି (verbose)
				printf("%s\t%s\n", item->string,
					item->util ? (ଅଚଳ ଅକ୍ଷର *)item->util : "");
			ଅନ୍ୟଥା {
				ଯଦି (i && !strcmp((item - 1)->string, item->string))
					ଜାରି;
				printf("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_query = 0, result = 0, query_flag = 0;
	ସଂରଚନା option options[] = {
		OPT_BOOL('n', ଖାଲି, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	ସଂରଚନା show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	ଯଦି (argc < 1)
		ଫେରନ୍ତୁ show_all();

	ଯଦି (!no_query)
		query_flag = (GET_REF_STATES | GET_HEAD_NAMES | GET_ଠେଲ_REF_STATES);

	ପାଇଁ (; argc; argc--, argv++) {
		size_t i;
		ସଂରଚନା strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = ଠେଲ_url_of_remote(info.states.remote);
		ପାଇଁ (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		ଯଦି (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		ଯଦି (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		ଅନ୍ୟଥା ଯଦି (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		ଅନ୍ୟଥା ଯଦି (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.ବସ୍ତୁଗୁଡ଼ିକ[0].string);
		ଅନ୍ୟଥା {
			printf(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			ପାଇଁ (i = 0; i < info.states.heads.nr; i++)
				printf("    %s\n", info.states.heads.ବସ୍ତୁଗୁଡ଼ିକ[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		ଯଦି (info.list.nr)
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
		ଯଦି (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		ଯଦି (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.ଠେଲ, add_ଠେଲ_to_show_info, &info);
		QSORT(info.list.ବସ୍ତୁଗୁଡ଼ିକ, info.list.nr, cmp_string_with_ଠେଲ);
		ଯଦି (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_ଠେଲ_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ଶୂନ୍ୟ report_set_head_auto(ଅଚଳ ଅକ୍ଷର *remote, ଅଚଳ ଅକ୍ଷର *head_name,
			ସଂରଚନା strbuf *b_local_head, ପୂର୍ଣ୍ଣସଂଖ୍ୟା was_detached) {
	ସଂରଚନା strbuf buf_prefix = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *prev_head = ଖାଲି;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_head->buf, buf_prefix.buf, &prev_head);

	ଯଦି (prev_head && !strcmp(prev_head, head_name))
		printf(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, head_name);
	ଅନ୍ୟଥା ଯଦି (prev_head)
		printf(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_head, head_name);
	ଅନ୍ୟଥା ଯଦି (!b_local_head->len)
		printf(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, head_name);
	ଅନ୍ୟଥା ଯଦି (was_detached && b_local_head->len)
		printf(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	ଅନ୍ୟଥା
		printf(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	strbuf_release(&buf_prefix);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_head(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		    ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା opt_a = 0, opt_d = 0, result = 0, was_detached;
	ସଂରଚନା strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		b_local_head = STRBUF_INIT;
	ଅକ୍ଷର *head_name = ଖାଲି;
	ସଂରଚନା ref_store *refs = get_main_ref_store(the_repository);
	ସଂରଚନା remote *remote;

	ସଂରଚନା option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	ଯଦି (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_head, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	ଯଦି (!opt_a && !opt_d && argc == 2) {
		head_name = xstrdup(argv[1]);
	} ଅନ୍ୟଥା ଯଦି (opt_a && !opt_d && argc == 1) {
		ସଂରଚନା ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_HEAD_NAMES);
		ଯଦି (!states.heads.nr)
			result |= error(_("Cannot determine remote HEAD"));
		ଅନ୍ୟଥା ଯଦି (states.heads.nr > 1) {
			result |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			ପାଇଁ (size_t i = 0; i < states.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], states.heads.ବସ୍ତୁଗୁଡ଼ିକ[i].string);
		} ଅନ୍ୟଥା
			head_name = xstrdup(states.heads.ବସ୍ତୁଗୁଡ଼ିକ[0].string);
		free_remote_ref_states(&states);
	} ଅନ୍ୟଥା ଯଦି (opt_d && !opt_a && argc == 1) {
		ଯଦି (refs_delete_ref(refs, ଖାଲି, b_head.buf, ଖାଲି, REF_NO_DEREF))
			result |= error(_("Could not delete %s"), b_head.buf);
	} ଅନ୍ୟଥା
		usage_with_options(builtin_remote_sethead_usage, options);

	ଯଦି (!head_name)
		ଯାଅ cleanup;
	strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", argv[0], head_name);
	ଯଦି (!refs_ref_exists(refs, b_remote_head.buf)) {
		result |= error(_("Not a valid ref: %s"), b_remote_head.buf);
		ଯାଅ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
			"remote set-head", &b_local_head, 0);
	ଯଦି (was_detached == -1) {
		result |= error(_("Could not set up %s"), b_head.buf);
		ଯାଅ cleanup;
	}
	ଯଦି (opt_a)
		report_set_head_auto(argv[0], head_name, &b_local_head, was_detached);
	ଯଦି (remote->follow_remote_head == FOLLOW_REMOTE_ALWAYS) {
		ସଂରଚନା strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_set(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	ମୁକ୍ତ(head_name);
	strbuf_release(&b_head);
	strbuf_release(&b_remote_head);
	strbuf_release(&b_local_head);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune_remote(ଅଚଳ ଅକ୍ଷର *remote, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	ସଂରଚନା ref_states states = REF_STATES_INIT;
	ସଂରଚନା string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	ସଂରଚନା string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	ଯଦି (!states.stale.nr) {
		free_remote_ref_states(&states);
		ଫେରନ୍ତୁ 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	ଯଦି (!dry_run)
		result |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		ଅଚଳ ଅକ୍ଷର *refname = item->util;

		ଯଦି (dry_run)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		ଅନ୍ୟଥା
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_run, &refs_to_prune);

	string_list_clear(&refs_to_prune, 0);
	free_remote_ref_states(&states);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run = 0, result = 0;
	ସଂରଚନା option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	ଯଦି (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	ପାଇଁ (; argc; argc--, argv++)
		result |= prune_remote(*argv, dry_run);

	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_remote_default(ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ UNUSED,
			      ଅଚଳ ସଂରଚନା config_context *ctx UNUSED,
			      ଶୂନ୍ୟ *priv)
{
	ଯଦି (strcmp(ଚାବି, "remotes.default") == 0) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା *found = priv;
		*found = 1;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା update(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		  ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, prune = -1;
	ସଂରଚନା option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_ଠେଲ(&cmd.args, "fetch");

	ଯଦି (prune != -1)
		strvec_ଠେଲ(&cmd.args, prune ? "--prune" : "--no-prune");
	ଯଦି (verbose)
		strvec_ଠେଲ(&cmd.args, "-v");
	strvec_ଠେଲ(&cmd.args, "--multiple");
	ଯଦି (argc < 2)
		strvec_ଠେଲ(&cmd.args, "default");
	ପାଇଁ (i = 1; i < argc; i++)
		strvec_ଠେଲ(&cmd.args, argv[i]);

	ଯଦି (strcmp(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		ଯଦି (!default_defined) {
			strvec_କାଢ଼(&cmd.args);
			strvec_ଠେଲ(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	ଫେରନ୍ତୁ run_command(&cmd);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା remove_all_fetch_refspecs(ଅଚଳ ଅକ୍ଷର *ଚାବି)
{
	ଫେରନ୍ତୁ repo_config_set_multivar_gently(the_repository, ଚାବି, ଖାଲି, ଖାଲି,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

ସ୍ଥିର ଶୂନ୍ୟ add_branches(ସଂରଚନା remote *remote, ଅଚଳ ଅକ୍ଷର **branches,
			 ଅଚଳ ଅକ୍ଷର *ଚାବି)
{
	ଅଚଳ ଅକ୍ଷର *remotename = remote->name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା mirror = remote->mirror;
	ସଂରଚନା strbuf refspec = STRBUF_INIT;

	ପାଇଁ (; *branches; branches++)
		add_branch(ଚାବି, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_remote_branches(ଅଚଳ ଅକ୍ଷର *remotename, ଅଚଳ ଅକ୍ଷର **branches,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_mode)
{
	ସଂରଚନା strbuf ଚାବି = STRBUF_INIT;
	ସଂରଚନା remote *remote;

	strbuf_addf(&ଚାବି, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	ଯଦି (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	ଯଦି (!add_mode && remove_all_fetch_refspecs(ଚାବି.buf)) {
		strbuf_release(&ଚାବି);
		ଫେରନ୍ତୁ 1;
	}
	add_branches(remote, branches, ଚାବି.buf);

	strbuf_release(&ଚାବି);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_branches(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_mode = 0;
	ସଂରଚନା option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	ଯଦି (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = ଖାଲି;

	ଫେରନ୍ତୁ set_remote_branches(argv[0], argv + 1, add_mode);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_url(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_mode = 0, all_mode = 0;
	ଅଚଳ ଅକ୍ଷର *remotename = ଖାଲି;
	ସଂରଚନା remote *remote;
	ସଂରଚନା strvec *url;
	ସଂରଚନା option options[] = {
		OPT_BOOL('\0', "push", &ଠେଲ_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	ଯଦି (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	ଯଦି (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	url = ଠେଲ_mode ? ଠେଲ_url_of_remote(remote) : &remote->url;

	ଯଦି (all_mode) {
		ପାଇଁ (size_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} ଅନ୍ୟଥା {
		printf_ln("%s", url->v[0]);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_url(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_mode = 0, add_mode = 0, delete_mode = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା matches = 0, negative_matches = 0;
	ଅଚଳ ଅକ୍ଷର *remotename = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *newurl = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *oldurl = ଖାଲି;
	ସଂରଚନା remote *remote;
	regex_t old_regex;
	ସଂରଚନା strvec *urlset;
	ସଂରଚନା strbuf name_buf = STRBUF_INIT;
	ସଂରଚନା option options[] = {
		OPT_BOOL('\0', "push", &ଠେଲ_mode,
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

	ଯଦି (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	ଯଦି (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	ଯଦି (argc > 3)
		oldurl = argv[3];

	ଯଦି (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	ଯଦି (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	ଯଦି (ଠେଲ_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} ଅନ୍ୟଥା {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	ଯଦି ((!oldurl && !delete_mode) || add_mode) {
		ଯଦି (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		ଅନ୍ୟଥା
			repo_config_set(the_repository, name_buf.buf, newurl);
		ଯାଅ out;
	}

	/* Old URL specified. Demand that one matches. */
	ଯଦି (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	ପାଇଁ (size_t i = 0; i < urlset->nr; i++)
		ଯଦି (!regexec(&old_regex, urlset->v[i], 0, ଖାଲି, 0))
			matches++;
		ଅନ୍ୟଥା
			negative_matches++;
	ଯଦି (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	ଯଦି (delete_mode && !negative_matches && !ଠେଲ_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	ଯଦି (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	ଅନ୍ୟଥା
		repo_config_set_multivar(the_repository, name_buf.buf, ଖାଲି, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_remote(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	       ଅଚଳ ଅକ୍ଷର **argv,
	       ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo)
{
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, ଖାଲି, PARSE_OPT_NOCOMPLETE),
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

	ଯଦି (fn) {
		ଫେରନ୍ତୁ !!fn(argc, argv, prefix, repo);
	} ଅନ୍ୟଥା {
		ଯଦି (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		ଫେରନ୍ତୁ !!show_all();
	}
}
