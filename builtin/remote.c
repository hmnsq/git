#define USE_THE_REPOSITORY_VARIABLE

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

ساكن ثابت حرف * ثابت builtin_remote_add_usage[] = {
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

#define GET_REF_STATES (1<<0)
#define GET_HEAD_NAMES (1<<1)
#define GET_پاڵنان_REF_STATES (1<<2)

ساكن صحيح verbose;

ساكن صحيح fetch_remote(ثابت حرف *name)
{
	بنية child_process cmd = CHILD_PROCESS_INIT;

	strvec_پاڵنان(&cmd.args, "fetch");
	إذا (verbose)
		strvec_پاڵنان(&cmd.args, "-v");
	strvec_پاڵنان(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	إذا (run_command(&cmd))
		أرجع error(_("Could not fetch %s"), name);
	أرجع 0;
}

تعداد {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#define MIRROR_NONE 0
#define MIRROR_FETCH 1
#define MIRROR_پاڵنان 2
#define MIRROR_BOTH (MIRROR_FETCH|MIRROR_پاڵنان)

ساكن فراغ add_branch(ثابت حرف *کلیل, ثابت حرف *branchname,
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
	repo_config_set_multivar(the_repository, کلیل, tmp->buf, "^$", 0);
}

ساكن ثابت حرف mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

ساكن صحيح parse_mirror_opt(ثابت بنية option *opt, ثابت حرف *arg, صحيح not)
{
	unsigned *mirror = opt->بەها;
	إذا (not)
		*mirror = MIRROR_NONE;
	وإلا إذا (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	وإلا إذا (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	وإلا إذا (!strcmp(arg, "push"))
		*mirror = MIRROR_پاڵنان;
	وإلا
		أرجع error(_("unknown --mirror argument: %s"), arg);
	أرجع 0;
}

ساكن صحيح check_remote_collision(بنية remote *remote, فراغ *data)
{
	ثابت حرف *name = data;
	ثابت حرف *p;

	إذا (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	إذا (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	أرجع 0;
}

ساكن صحيح add(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
	       بنية repository *repo UNUSED)
{
	صحيح fetch = 0, fetch_tags = TAGS_DEFAULT;
	unsigned mirror = MIRROR_NONE;
	بنية string_list track = STRING_LIST_INIT_NODUP;
	ثابت حرف *master = عدم;
	بنية remote *remote;
	بنية strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ثابت حرف *name, *url;
	صحيح result = 0;

	بنية option options[] = {
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

	إذا (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	إذا (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	إذا (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	إذا (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		exit(3);
	}

	إذا (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (فراغ *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	إذا (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		إذا (track.nr == 0)
			string_list_append(&track, "*");
		لكل (size_t i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.کاڵاکان[i].string,
				   name, mirror, &buf2);
		}
	}

	إذا (mirror & MIRROR_پاڵنان) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	إذا (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	إذا (fetch && fetch_remote(name)) {
		result = 1;
		اذهب out;
	}

	إذا (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		إذا (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			result = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	أرجع result;
}

بنية branch_info {
	حرف *remote_name;
	بنية string_list merge;
	تعداد rebase_type rebase;
	حرف *پاڵنان_remote_name;
};

ساكن بنية string_list branch_list = STRING_LIST_INIT_DUP;

ساكن ثابت حرف *abbrev_ref(ثابت حرف *name, ثابت حرف *prefix)
{
	skip_prefix(name, prefix, &name);
	أرجع name;
}
#define abbrev_branch(name) abbrev_ref((name), "refs/heads/")

ساكن صحيح config_read_branches(ثابت حرف *کلیل, ثابت حرف *بەها,
				ثابت بنية config_context *ctx UNUSED,
				فراغ *data UNUSED)
{
	ثابت حرف *orig_کلیل = کلیل;
	حرف *name;
	بنية string_list_item *item;
	بنية branch_info *info;
	تعداد { REMOTE, MERGE, REBASE, پاڵنان_REMOTE } type;
	size_t کلیل_len;

	إذا (!starts_with(کلیل, "branch."))
		أرجع 0;

	کلیل += strlen("branch.");
	إذا (strip_suffix(کلیل, ".remote", &کلیل_len))
		type = REMOTE;
	وإلا إذا (strip_suffix(کلیل, ".merge", &کلیل_len))
		type = MERGE;
	وإلا إذا (strip_suffix(کلیل, ".rebase", &کلیل_len))
		type = REBASE;
	وإلا إذا (strip_suffix(کلیل, ".pushremote", &کلیل_len))
		type = پاڵنان_REMOTE;
	وإلا
		أرجع 0;

	name = xmemdupz(کلیل, کلیل_len);
	item = string_list_زیادکردن(&branch_list, name);

	إذا (!item->util)
		item->util = xcalloc(1, حجم(بنية branch_info));
	info = item->util;
	بدل (type) {
	حالة REMOTE:
		إذا (info->remote_name)
			warning(_("more than one %s"), orig_کلیل);
		info->remote_name = xstrdup(بەها);
		اكسر;
	حالة MERGE: {
		ثابت حرف *space = strchr(بەها, ' ');
		بەها = abbrev_branch(بەها);
		طالما (space) {
			حرف *merge;
			merge = xstrndup(بەها, space - بەها);
			string_list_append(&info->merge, merge);
			بەها = abbrev_branch(space + 1);
			space = strchr(بەها, ' ');
		}
		string_list_append(&info->merge, xstrdup(بەها));
		اكسر;
	}
	حالة REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_بەها(بەها);
		إذا (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, بەها);
		اكسر;
	حالة پاڵنان_REMOTE:
		إذا (info->پاڵنان_remote_name)
			warning(_("more than one %s"), orig_کلیل);
		info->پاڵنان_remote_name = xstrdup(بەها);
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

بنية ref_states {
	بنية remote *remote;
	بنية string_list new_refs, skipped, stale, tracked, heads, پاڵنان;
	صحيح queried;
};

#define REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.پاڵنان = STRING_LIST_INIT_DUP, \
}

ساكن صحيح get_ref_states(ثابت بنية ref *remote_refs, بنية ref_states *states)
{
	بنية ref *fetch_map = عدم, **tail = &fetch_map;
	بنية ref *ref, *stale_refs;
	صحيح i;

	لكل (i = 0; i < states->remote->fetch.nr; i++)
		إذا (get_fetch_map(remote_refs, &states->remote->fetch.کاڵاکان[i], &tail, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.کاڵاکان[i].raw);

	لكل (ref = fetch_map; ref; ref = ref->next) {
		إذا (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		وإلا إذا (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		وإلا
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	لكل (ref = stale_refs; ref; ref = ref->next) {
		بنية string_list_item *item =
			string_list_append(&states->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_sort(&states->new_refs);
	string_list_sort(&states->skipped);
	string_list_sort(&states->tracked);
	string_list_sort(&states->stale);

	أرجع 0;
}

بنية پاڵنان_info {
	حرف *dest;
	صحيح forced;
	تعداد {
		پاڵنان_STATUS_CREATE = 0,
		پاڵنان_STATUS_DELETE,
		پاڵنان_STATUS_UPTODATE,
		پاڵنان_STATUS_FASTFORWARD,
		پاڵنان_STATUS_OUTOFDATE,
		پاڵنان_STATUS_NOTQUERIED
	} status;
};

ساكن صحيح get_پاڵنان_ref_states(ثابت بنية ref *remote_refs,
	بنية ref_states *states)
{
	بنية remote *remote = states->remote;
	بنية ref *ref, *local_refs, *پاڵنان_map;
	إذا (remote->mirror)
		أرجع 0;

	local_refs = get_local_heads();
	پاڵنان_map = copy_ref_list(remote_refs);

	match_پاڵنان_refs(local_refs, &پاڵنان_map, &remote->پاڵنان, MATCH_REFS_NONE);

	لكل (ref = پاڵنان_map; ref; ref = ref->next) {
		بنية string_list_item *item;
		بنية پاڵنان_info *info;

		إذا (!ref->peer_ref)
			استمر;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->پاڵنان,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, حجم(بنية پاڵنان_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		إذا (is_null_oid(&ref->new_oid)) {
			info->status = پاڵنان_STATUS_DELETE;
		} وإلا إذا (oideq(&ref->old_oid, &ref->new_oid))
			info->status = پاڵنان_STATUS_UPTODATE;
		وإلا إذا (is_null_oid(&ref->old_oid))
			info->status = پاڵنان_STATUS_CREATE;
		وإلا إذا (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = پاڵنان_STATUS_FASTFORWARD;
		وإلا
			info->status = پاڵنان_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(پاڵنان_map);
	أرجع 0;
}

ساكن صحيح get_پاڵنان_ref_states_noquery(بنية ref_states *states)
{
	صحيح i;
	بنية remote *remote = states->remote;
	بنية string_list_item *item;
	بنية پاڵنان_info *info;

	إذا (remote->mirror)
		أرجع 0;

	إذا (!remote->پاڵنان.nr) {
		item = string_list_append(&states->پاڵنان, _("(matching)"));
		info = item->util = xcalloc(1, حجم(بنية پاڵنان_info));
		info->status = پاڵنان_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	لكل (i = 0; i < remote->پاڵنان.nr; i++) {
		ثابت بنية refspec_item *spec = &remote->پاڵنان.کاڵاکان[i];
		إذا (spec->matching)
			item = string_list_append(&states->پاڵنان, _("(matching)"));
		وإلا إذا (strlen(spec->src))
			item = string_list_append(&states->پاڵنان, spec->src);
		وإلا
			item = string_list_append(&states->پاڵنان, _("(delete)"));

		info = item->util = xcalloc(1, حجم(بنية پاڵنان_info));
		info->forced = spec->force;
		info->status = پاڵنان_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	أرجع 0;
}

ساكن صحيح get_head_names(ثابت بنية ref *remote_refs, بنية ref_states *states)
{
	بنية ref *ref, *matches;
	بنية ref *fetch_map = عدم, **fetch_map_tail = &fetch_map;
	بنية refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (حرف *) "refs/heads/*",
		.dst = (حرف *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	لكل (ref = matches; ref; ref = ref->next)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	أرجع 0;
}

بنية known_remote {
	بنية known_remote *next;
	بنية remote *remote;
};

بنية known_remotes {
	بنية remote *to_delete;
	بنية known_remote *list;
};

ساكن صحيح add_known_remote(بنية remote *remote, فراغ *cb_data)
{
	بنية known_remotes *all = cb_data;
	بنية known_remote *r;

	إذا (!strcmp(all->to_delete->name, remote->name))
		أرجع 0;

	r = xmalloc(حجم(*r));
	r->remote = remote;
	r->next = all->list;
	all->list = r;
	أرجع 0;
}

بنية branches_for_remote {
	بنية remote *remote;
	بنية string_list *branches, *skipped;
	بنية known_remotes *keep;
};

ساكن صحيح add_branch_for_removal(ثابت بنية reference *ref, فراغ *cb_data)
{
	بنية branches_for_remote *branches = cb_data;
	بنية refspec_item refspec;
	بنية known_remote *kr;

	memset(&refspec, 0, حجم(refspec));
	refspec.dst = (حرف *)ref->name;
	إذا (remote_find_tracking(branches->remote, &refspec))
		أرجع 0;
	حرر(refspec.src);

	/* don't delete a branch if another remote also uses it */
	لكل (kr = branches->keep->list; kr; kr = kr->next) {
		memset(&refspec, 0, حجم(refspec));
		refspec.dst = (حرف *)ref->name;
		إذا (!remote_find_tracking(kr->remote, &refspec)) {
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
	بنية progress *progress;
	بنية strbuf *err;
	uint32_t progress_nr;
	uint64_t index;
};

ساكن فراغ compute_renamed_ref(بنية rename_info *rename,
				ثابت حرف *refname,
				بنية strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, strlen("refs/remotes/"), strlen(rename->old_name),
		      rename->new_name, strlen(rename->new_name));
}

ساكن صحيح rename_one_reflog_entry(ثابت حرف *old_refname,
				   بنية object_id *old_oid,
				   بنية object_id *new_oid,
				   ثابت حرف *committer,
				   timestamp_t timestamp, صحيح tz,
				   ثابت حرف *پەیام, فراغ *cb_data)
{
	بنية rename_info *rename = cb_data;
	بنية strbuf new_refname = STRBUF_INIT;
	بنية strbuf identity = STRBUF_INIT;
	بنية strbuf name = STRBUF_INIT;
	بنية strbuf mail = STRBUF_INIT;
	بنية ident_split ident;
	ثابت حرف *date;
	صحيح error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	إذا (split_ident_هێڵ(&ident, committer, strlen(committer)) < 0) {
		error = -1;
		اذهب out;
	}

	strbuf_add(&name, ident.name_begin, ident.name_end - ident.name_begin);
	strbuf_add(&mail, ident.mail_begin, ident.mail_end - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_MODE(NORMAL));
	strbuf_addstr(&identity, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, identity.buf, پەیام,
					      rename->index++, rename->err);

out:
	strbuf_release(&new_refname);
	strbuf_release(&identity);
	strbuf_release(&name);
	strbuf_release(&mail);
	أرجع error;
}

ساكن صحيح rename_one_reflog(ثابت حرف *old_refname,
			     ثابت بنية object_id *old_oid,
			     بنية rename_info *rename)
{
	بنية strbuf new_refname = STRBUF_INIT;
	بنية strbuf message = STRBUF_INIT;
	صحيح error;

	إذا (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		أرجع 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, rename);
	إذا (error < 0)
		اذهب out;

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
	إذا (error < 0)
		أرجع error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	أرجع error;
}

ساكن صحيح rename_one_ref(ثابت بنية reference *ref, فراغ *cb_data)
{
	بنية strbuf new_referent = STRBUF_INIT;
	بنية strbuf new_refname = STRBUF_INIT;
	بنية rename_info *rename = cb_data;
	ثابت بنية object_id *oid = ref->oid;
	ثابت حرف *referent = ref->target;
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
		اذهب out;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : عدم, عدم,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       عدم, rename->err);
	إذا (error < 0)
		اذهب out;

	error = rename_one_reflog(ref->name, oid, rename);
	إذا (error < 0)
		اذهب out;

	display_progress(rename->progress, ++rename->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	أرجع error;
}

ساكن صحيح migrate_file(بنية remote *remote)
{
	بنية strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	لكل (size_t i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	لكل (صحيح i = 0; i < remote->پاڵنان.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->پاڵنان.کاڵاکان[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	لكل (صحيح i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.کاڵاکان[i].raw, "^$", 0);
#ifndef WITH_BREAKING_CHANGES
	إذا (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	وإلا إذا (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#endif /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	أرجع 0;
}

بنية پاڵنان_default_info
{
	ثابت حرف *old_name;
	تعداد config_scope scope;
	بنية strbuf origin;
	صحيح linenr;
};

ساكن صحيح config_read_پاڵنان_default(ثابت حرف *کلیل, ثابت حرف *بەها,
	ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت بنية کلیل_بەها_info *kvi = ctx->kvi;

	بنية پاڵنان_default_info* info = cb;
	إذا (strcmp(کلیل, "remote.pushdefault") ||
	    !بەها || strcmp(بەها, info->old_name))
		أرجع 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	أرجع 0;
}

ساكن فراغ handle_پاڵنان_default(ثابت حرف* old_name, ثابت حرف* new_name)
{
	بنية پاڵنان_default_info پاڵنان_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_پاڵنان_default, &پاڵنان_default);
	إذا (پاڵنان_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	وإلا إذا (پاڵنان_default.scope >= CONFIG_SCOPE_LOCAL) {
		صحيح result = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		إذا (new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		وإلا إذا (!new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} وإلا إذا (پاڵنان_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(پاڵنان_default.scope),
			پاڵنان_default.origin.buf, پاڵنان_default.linenr,
			old_name);
	}

	strbuf_release(&پاڵنان_default.origin);
}

ساكن ثابت حرف conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

ساكن صحيح mv(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
	      بنية repository *repo UNUSED)
{
	صحيح show_progress = isatty(2);
	بنية option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	بنية remote *oldremote, *newremote;
	بنية strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	بنية rename_info rename = {
		.err = &err,
	};
	صحيح refspecs_need_update = 0;
	صحيح result = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	إذا (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	إذا (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		exit(2);
	}

	إذا (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		أرجع migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	إذا (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		exit(3);
	}

	إذا (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	إذا (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		result = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		اذهب out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	لكل (صحيح i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!strstr(oldremote->fetch.کاڵاکان[i].raw,
						old_remote_context.buf);

	إذا (refspecs_need_update) {
		بنية refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		إذا (!rename.transaction)
			اذهب out;

		إذا (show_progress)
			rename.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.prefix = buf.buf;

		result = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		إذا (result < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		result = ref_transaction_prepare(rename.transaction, &err);
		إذا (result < 0) {
			error("renaming remote references failed: %s", err.buf);
			إذا (result == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(عدم);
		}
	}

	إذا (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_set_multivar(the_repository, buf.buf, عدم, عدم, CONFIG_FLAGS_MULTI_REPLACE);
		لكل (صحيح i = 0; i < oldremote->fetch.nr; i++) {
			حرف *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.کاڵاکان[i].raw);
			ptr = strstr(buf2.buf, old_remote_context.buf);
			إذا (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + strlen(":refs/remotes/"),
					      strlen(rename.old_name), rename.new_name,
					      strlen(rename.new_name));
			} وإلا
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_set_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	لكل (size_t i = 0; i < branch_list.nr; i++) {
		بنية string_list_item *item = branch_list.کاڵاکان + i;
		بنية branch_info *info = item->util;
		إذا (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
		إذا (info->پاڵنان_remote_name && !strcmp(info->پاڵنان_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
	}

	إذا (refspecs_need_update) {
		result = ref_transaction_commit(rename.transaction, &err);
		إذا (result < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_progress(&rename.progress);

		handle_پاڵنان_default(rename.old_name, rename.new_name);
	}

out:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	أرجع result;
}

ساكن صحيح rm(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
	      بنية repository *repo UNUSED)
{
	بنية option options[] = {
		OPT_END()
	};
	بنية remote *remote;
	بنية strbuf buf = STRBUF_INIT;
	بنية known_remotes known_remotes = { عدم, عدم };
	بنية string_list branches = STRING_LIST_INIT_DUP;
	بنية string_list skipped = STRING_LIST_INIT_DUP;
	بنية branches_for_remote cb_data;
	صحيح result;

	memset(&cb_data, 0, حجم(cb_data));
	cb_data.branches = &branches;
	cb_data.skipped = &skipped;
	cb_data.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	إذا (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		exit(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	لكل (size_t i = 0; i < branch_list.nr; i++) {
		بنية string_list_item *item = branch_list.کاڵاکان + i;
		بنية branch_info *info = item->util;
		إذا (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ثابت حرف *کلیلەکان[] = { "remote", "merge", عدم }, **k;
			لكل (k = کلیلەکان; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				result = repo_config_set_gently(the_repository, buf.buf, عدم);
				إذا (result && result != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		إذا (info->پاڵنان_remote_name && !strcmp(info->پاڵنان_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			result = repo_config_set_gently(the_repository, buf.buf, عدم);
			إذا (result && result != CONFIG_NOTHING_SET)
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

	إذا (!result)
		result = refs_delete_refs(get_main_ref_store(the_repository),
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
		لكل (size_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.کاڵاکان[i].string);
	}
	string_list_clear(&skipped, 0);

	إذا (!result) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		إذا (repo_config_rename_section(the_repository, buf.buf, عدم) < 1) {
			result = error(_("Could not remove config section '%s'"), buf.buf);
			اذهب out;
		}

		handle_پاڵنان_default(remote->name, عدم);
	}

out:
	لكل (بنية known_remote *r = known_remotes.list; r;) {
		بنية known_remote *next = r->next;
		حرر(r);
		r = next;
	}
	strbuf_release(&buf);
	أرجع result;
}

ساكن فراغ clear_پاڵنان_info(فراغ *util, ثابت حرف *string UNUSED)
{
	بنية پاڵنان_info *info = util;
	حرر(info->dest);
	حرر(info);
}

ساكن فراغ free_remote_ref_states(بنية ref_states *states)
{
	string_list_clear(&states->new_refs, 0);
	string_list_clear(&states->skipped, 0);
	string_list_clear(&states->stale, 1);
	string_list_clear(&states->tracked, 0);
	string_list_clear(&states->heads, 0);
	string_list_clear_func(&states->پاڵنان, clear_پاڵنان_info);
}

ساكن صحيح append_ref_to_tracked_list(ثابت بنية reference *ref, فراغ *cb_data)
{
	بنية ref_states *states = cb_data;
	بنية refspec_item refspec;

	إذا (ref->flags & REF_ISSYMREF)
		أرجع 0;

	memset(&refspec, 0, حجم(refspec));
	refspec.dst = (حرف *)ref->name;
	إذا (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		حرر(refspec.src);
	}

	أرجع 0;
}

ساكن صحيح get_remote_ref_states(ثابت حرف *name,
				 بنية ref_states *states,
				 صحيح query)
{
	states->remote = remote_get(name);
	إذا (!states->remote)
		أرجع error(_("No such remote: '%s'"), name);

	read_branches();

	إذا (query) {
		بنية transport *transport;
		ثابت بنية ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, عدم);

		states->queried = 1;
		إذا (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		إذا (query & GET_HEAD_NAMES)
			get_head_names(remote_refs, states);
		إذا (query & GET_پاڵنان_REF_STATES)
			get_پاڵنان_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} وإلا {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_پاڵنان_ref_states_noquery(states);
	}

	أرجع 0;
}

بنية show_info {
	بنية string_list list;
	بنية ref_states states;
	صحيح width, width2;
	صحيح any_rebase;
};

#define SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

ساكن صحيح add_remote_to_show_info(بنية string_list_item *item, فراغ *cb_data)
{
	بنية show_info *info = cb_data;
	صحيح n = strlen(item->string);
	إذا (n > info->width)
		info->width = n;
	string_list_زیادکردن(&info->list, item->string);
	أرجع 0;
}

ساكن صحيح show_remote_info_item(بنية string_list_item *item, فراغ *cb_data)
{
	بنية show_info *info = cb_data;
	بنية ref_states *states = &info->states;
	ثابت حرف *name = item->string;

	إذا (states->queried) {
		ثابت حرف *fmt = "%s";
		ثابت حرف *arg = "";
		إذا (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} وإلا إذا (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		وإلا إذا (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		وإلا إذا (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		وإلا
			arg = _(" ???");
		printf("    %-*s", info->width, name);
		printf(fmt, arg);
		printf("\n");
	} وإلا
		printf("    %s\n", name);

	أرجع 0;
}

ساكن صحيح add_local_to_show_info(بنية string_list_item *branch_item, فراغ *cb_data)
{
	بنية show_info *show_info = cb_data;
	بنية ref_states *states = &show_info->states;
	بنية branch_info *branch_info = branch_item->util;
	بنية string_list_item *item;
	صحيح n;

	إذا (!branch_info->merge.nr || !branch_info->remote_name ||
	    strcmp(states->remote->name, branch_info->remote_name))
		أرجع 0;
	إذا ((n = strlen(branch_item->string)) > show_info->width)
		show_info->width = n;
	إذا (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_زیادکردن(&show_info->list, branch_item->string);
	item->util = branch_info;

	أرجع 0;
}

ساكن صحيح show_local_info_item(بنية string_list_item *item, فراغ *cb_data)
{
	بنية show_info *show_info = cb_data;
	بنية branch_info *branch_info = item->util;
	بنية string_list *merge = &branch_info->merge;
	صحيح width = show_info->width + 4;

	إذا (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		أرجع 0;
	}

	printf("    %-*s ", show_info->width, item->string);
	إذا (branch_info->rebase >= REBASE_TRUE) {
		ثابت حرف *پەیام;
		إذا (branch_info->rebase == REBASE_INTERACTIVE)
			پەیام = _("rebases interactively onto remote %s");
		وإلا إذا (branch_info->rebase == REBASE_MERGES)
			پەیام = _("rebases interactively (with merges) onto "
				"remote %s");
		وإلا
			پەیام = _("rebases onto remote %s");
		printf_ln(پەیام, merge->کاڵاکان[0].string);
		أرجع 0;
	} وإلا إذا (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->کاڵاکان[0].string);
		width++;
	} وإلا {
		printf_ln(_("merges with remote %s"), merge->کاڵاکان[0].string);
	}
	لكل (size_t i = 1; i < merge->nr; i++)
		printf(_("%-*s    and with remote %s\n"), width, "",
		       merge->کاڵاکان[i].string);

	أرجع 0;
}

ساكن صحيح add_پاڵنان_to_show_info(بنية string_list_item *پاڵنان_item, فراغ *cb_data)
{
	بنية show_info *show_info = cb_data;
	بنية پاڵنان_info *پاڵنان_info = پاڵنان_item->util;
	بنية string_list_item *item;
	صحيح n;
	إذا ((n = strlen(پاڵنان_item->string)) > show_info->width)
		show_info->width = n;
	إذا ((n = strlen(پاڵنان_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, پاڵنان_item->string);
	item->util = پاڵنان_item->util;
	أرجع 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
ساكن صحيح cmp_string_with_پاڵنان(ثابت فراغ *va, ثابت فراغ *vb)
{
	ثابت بنية string_list_item *a = va;
	ثابت بنية string_list_item *b = vb;
	ثابت بنية پاڵنان_info *a_پاڵنان = a->util;
	ثابت بنية پاڵنان_info *b_پاڵنان = b->util;
	صحيح cmp = strcmp(a->string, b->string);
	أرجع cmp ? cmp : strcmp(a_پاڵنان->dest, b_پاڵنان->dest);
}

ساكن صحيح show_پاڵنان_info_item(بنية string_list_item *item, فراغ *cb_data)
{
	بنية show_info *show_info = cb_data;
	بنية پاڵنان_info *پاڵنان_info = item->util;
	ثابت حرف *src = item->string, *status = عدم;

	بدل (پاڵنان_info->status) {
	حالة پاڵنان_STATUS_CREATE:
		status = _("create");
		اكسر;
	حالة پاڵنان_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		اكسر;
	حالة پاڵنان_STATUS_UPTODATE:
		status = _("up to date");
		اكسر;
	حالة پاڵنان_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		اكسر;
	حالة پاڵنان_STATUS_OUTOFDATE:
		status = _("local out of date");
		اكسر;
	حالة پاڵنان_STATUS_NOTQUERIED:
		اكسر;
	}
	إذا (status) {
		إذا (پاڵنان_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, پاڵنان_info->dest, status);
		وإلا
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, پاڵنان_info->dest, status);
	} وإلا {
		إذا (پاڵنان_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       پاڵنان_info->dest);
		وإلا
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       پاڵنان_info->dest);
	}
	أرجع 0;
}

ساكن صحيح get_one_entry(بنية remote *remote, فراغ *priv)
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
	url = پاڵنان_url_of_remote(remote);
	لكل (size_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, عدم);
	}

	أرجع 0;
}

ساكن صحيح show_all(فراغ)
{
	بنية string_list list = STRING_LIST_INIT_DUP;
	صحيح result;

	result = for_each_remote(get_one_entry, &list);

	إذا (!result) {
		string_list_sort(&list);
		لكل (size_t i = 0; i < list.nr; i++) {
			بنية string_list_item *item = list.کاڵاکان + i;
			إذا (verbose)
				printf("%s\t%s\n", item->string,
					item->util ? (ثابت حرف *)item->util : "");
			وإلا {
				إذا (i && !strcmp((item - 1)->string, item->string))
					استمر;
				printf("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	أرجع result;
}

ساكن صحيح show(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		بنية repository *repo UNUSED)
{
	صحيح no_query = 0, result = 0, query_flag = 0;
	بنية option options[] = {
		OPT_BOOL('n', عدم, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	بنية show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	إذا (argc < 1)
		أرجع show_all();

	إذا (!no_query)
		query_flag = (GET_REF_STATES | GET_HEAD_NAMES | GET_پاڵنان_REF_STATES);

	لكل (; argc; argc--, argv++) {
		size_t i;
		بنية strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = پاڵنان_url_of_remote(info.states.remote);
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
		وإلا إذا (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		وإلا إذا (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.کاڵاکان[0].string);
		وإلا {
			printf(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			لكل (i = 0; i < info.states.heads.nr; i++)
				printf("    %s\n", info.states.heads.کاڵاکان[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		إذا (info.list.nr)
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
		إذا (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		إذا (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.پاڵنان, add_پاڵنان_to_show_info, &info);
		QSORT(info.list.کاڵاکان, info.list.nr, cmp_string_with_پاڵنان);
		إذا (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_پاڵنان_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	أرجع result;
}

ساكن فراغ report_set_head_auto(ثابت حرف *remote, ثابت حرف *head_name,
			بنية strbuf *b_local_head, صحيح was_detached) {
	بنية strbuf buf_prefix = STRBUF_INIT;
	ثابت حرف *prev_head = عدم;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_head->buf, buf_prefix.buf, &prev_head);

	إذا (prev_head && !strcmp(prev_head, head_name))
		printf(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, head_name);
	وإلا إذا (prev_head)
		printf(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_head, head_name);
	وإلا إذا (!b_local_head->len)
		printf(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, head_name);
	وإلا إذا (was_detached && b_local_head->len)
		printf(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	وإلا
		printf(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	strbuf_release(&buf_prefix);
}

ساكن صحيح set_head(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		    بنية repository *repo UNUSED)
{
	صحيح opt_a = 0, opt_d = 0, result = 0, was_detached;
	بنية strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		b_local_head = STRBUF_INIT;
	حرف *head_name = عدم;
	بنية ref_store *refs = get_main_ref_store(the_repository);
	بنية remote *remote;

	بنية option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	إذا (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_head, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	إذا (!opt_a && !opt_d && argc == 2) {
		head_name = xstrdup(argv[1]);
	} وإلا إذا (opt_a && !opt_d && argc == 1) {
		بنية ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_HEAD_NAMES);
		إذا (!states.heads.nr)
			result |= error(_("Cannot determine remote HEAD"));
		وإلا إذا (states.heads.nr > 1) {
			result |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			لكل (size_t i = 0; i < states.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], states.heads.کاڵاکان[i].string);
		} وإلا
			head_name = xstrdup(states.heads.کاڵاکان[0].string);
		free_remote_ref_states(&states);
	} وإلا إذا (opt_d && !opt_a && argc == 1) {
		إذا (refs_delete_ref(refs, عدم, b_head.buf, عدم, REF_NO_DEREF))
			result |= error(_("Could not delete %s"), b_head.buf);
	} وإلا
		usage_with_options(builtin_remote_sethead_usage, options);

	إذا (!head_name)
		اذهب cleanup;
	strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", argv[0], head_name);
	إذا (!refs_ref_exists(refs, b_remote_head.buf)) {
		result |= error(_("Not a valid ref: %s"), b_remote_head.buf);
		اذهب cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
			"remote set-head", &b_local_head, 0);
	إذا (was_detached == -1) {
		result |= error(_("Could not set up %s"), b_head.buf);
		اذهب cleanup;
	}
	إذا (opt_a)
		report_set_head_auto(argv[0], head_name, &b_local_head, was_detached);
	إذا (remote->follow_remote_head == FOLLOW_REMOTE_ALWAYS) {
		بنية strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_set(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	حرر(head_name);
	strbuf_release(&b_head);
	strbuf_release(&b_remote_head);
	strbuf_release(&b_local_head);
	أرجع result;
}

ساكن صحيح prune_remote(ثابت حرف *remote, صحيح dry_run)
{
	صحيح result = 0;
	بنية ref_states states = REF_STATES_INIT;
	بنية string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	بنية string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	إذا (!states.stale.nr) {
		free_remote_ref_states(&states);
		أرجع 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	إذا (!dry_run)
		result |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		ثابت حرف *refname = item->util;

		إذا (dry_run)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		وإلا
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_run, &refs_to_prune);

	string_list_clear(&refs_to_prune, 0);
	free_remote_ref_states(&states);
	أرجع result;
}

ساكن صحيح prune(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		 بنية repository *repo UNUSED)
{
	صحيح dry_run = 0, result = 0;
	بنية option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	إذا (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	لكل (; argc; argc--, argv++)
		result |= prune_remote(*argv, dry_run);

	أرجع result;
}

ساكن صحيح get_remote_default(ثابت حرف *کلیل, ثابت حرف *بەها UNUSED,
			      ثابت بنية config_context *ctx UNUSED,
			      فراغ *priv)
{
	إذا (strcmp(کلیل, "remotes.default") == 0) {
		صحيح *found = priv;
		*found = 1;
	}
	أرجع 0;
}

ساكن صحيح update(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		  بنية repository *repo UNUSED)
{
	صحيح i, prune = -1;
	بنية option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	بنية child_process cmd = CHILD_PROCESS_INIT;
	صحيح default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_پاڵنان(&cmd.args, "fetch");

	إذا (prune != -1)
		strvec_پاڵنان(&cmd.args, prune ? "--prune" : "--no-prune");
	إذا (verbose)
		strvec_پاڵنان(&cmd.args, "-v");
	strvec_پاڵنان(&cmd.args, "--multiple");
	إذا (argc < 2)
		strvec_پاڵنان(&cmd.args, "default");
	لكل (i = 1; i < argc; i++)
		strvec_پاڵنان(&cmd.args, argv[i]);

	إذا (strcmp(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		إذا (!default_defined) {
			strvec_دەرهێنان(&cmd.args);
			strvec_پاڵنان(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	أرجع run_command(&cmd);
}

ساكن صحيح remove_all_fetch_refspecs(ثابت حرف *کلیل)
{
	أرجع repo_config_set_multivar_gently(the_repository, کلیل, عدم, عدم,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

ساكن فراغ add_branches(بنية remote *remote, ثابت حرف **branches,
			 ثابت حرف *کلیل)
{
	ثابت حرف *remotename = remote->name;
	صحيح mirror = remote->mirror;
	بنية strbuf refspec = STRBUF_INIT;

	لكل (; *branches; branches++)
		add_branch(کلیل, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

ساكن صحيح set_remote_branches(ثابت حرف *remotename, ثابت حرف **branches,
				صحيح add_mode)
{
	بنية strbuf کلیل = STRBUF_INIT;
	بنية remote *remote;

	strbuf_addf(&کلیل, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	إذا (!add_mode && remove_all_fetch_refspecs(کلیل.buf)) {
		strbuf_release(&کلیل);
		أرجع 1;
	}
	add_branches(remote, branches, کلیل.buf);

	strbuf_release(&کلیل);
	أرجع 0;
}

ساكن صحيح set_branches(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			بنية repository *repo UNUSED)
{
	صحيح add_mode = 0;
	بنية option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	إذا (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = عدم;

	أرجع set_remote_branches(argv[0], argv + 1, add_mode);
}

ساكن صحيح get_url(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		   بنية repository *repo UNUSED)
{
	صحيح پاڵنان_mode = 0, all_mode = 0;
	ثابت حرف *remotename = عدم;
	بنية remote *remote;
	بنية strvec *url;
	بنية option options[] = {
		OPT_BOOL('\0', "push", &پاڵنان_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	إذا (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	url = پاڵنان_mode ? پاڵنان_url_of_remote(remote) : &remote->url;

	إذا (all_mode) {
		لكل (size_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} وإلا {
		printf_ln("%s", url->v[0]);
	}

	أرجع 0;
}

ساكن صحيح set_url(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		   بنية repository *repo UNUSED)
{
	صحيح پاڵنان_mode = 0, add_mode = 0, delete_mode = 0;
	صحيح matches = 0, negative_matches = 0;
	ثابت حرف *remotename = عدم;
	ثابت حرف *newurl = عدم;
	ثابت حرف *oldurl = عدم;
	بنية remote *remote;
	regex_t old_regex;
	بنية strvec *urlset;
	بنية strbuf name_buf = STRBUF_INIT;
	بنية option options[] = {
		OPT_BOOL('\0', "push", &پاڵنان_mode,
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

	إذا (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	إذا (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	إذا (argc > 3)
		oldurl = argv[3];

	إذا (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	إذا (پاڵنان_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} وإلا {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	إذا ((!oldurl && !delete_mode) || add_mode) {
		إذا (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		وإلا
			repo_config_set(the_repository, name_buf.buf, newurl);
		اذهب out;
	}

	/* Old URL specified. Demand that one matches. */
	إذا (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	لكل (size_t i = 0; i < urlset->nr; i++)
		إذا (!regexec(&old_regex, urlset->v[i], 0, عدم, 0))
			matches++;
		وإلا
			negative_matches++;
	إذا (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	إذا (delete_mode && !negative_matches && !پاڵنان_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	إذا (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	وإلا
		repo_config_set_multivar(the_repository, name_buf.buf, عدم, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	أرجع 0;
}

صحيح cmd_remote(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *prefix,
	       بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, عدم, PARSE_OPT_NOCOMPLETE),
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

	إذا (fn) {
		أرجع !!fn(argc, argv, prefix, repo);
	} وإلا {
		إذا (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		أرجع !!show_all();
	}
}
