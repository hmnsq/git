#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "date.h"
#شامل "gettext.h"
#شامل "ident.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "transport.h"
#شامل "remote.h"
#شامل "string-list.h"
#شامل "strbuf.h"
#شامل "run-command.h"
#شامل "rebase.h"
#شامل "refs.h"
#شامل "refspec.h"
#شامل "odb.h"
#شامل "strvec.h"
#شامل "commit-reach.h"
#شامل "progress.h"

ایستا ثابت نویسه * ثابت builtin_remote_usage[] = {
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
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_remote_add_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	پوچ
};

#define GET_REF_STATES (1<<0)
#define GET_HEAD_NAMES (1<<1)
#define GET_افزودن_REF_STATES (1<<2)

ایستا صحیح verbose;

ایستا صحیح fetch_remote(ثابت نویسه *name)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;

	strvec_افزودن(&cmd.args, "fetch");
	اگر (verbose)
		strvec_افزودن(&cmd.args, "-v");
	strvec_افزودن(&cmd.args, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	اگر (run_command(&cmd))
		بازگشت error(_("Could not fetch %s"), name);
	بازگشت 0;
}

شمارش {
	TAGS_UNSET = 0,
	TAGS_DEFAULT = 1,
	TAGS_SET = 2
};

#define MIRROR_NONE 0
#define MIRROR_FETCH 1
#define MIRROR_افزودن 2
#define MIRROR_BOTH (MIRROR_FETCH|MIRROR_افزودن)

ایستا تهی add_branch(ثابت نویسه *کلید, ثابت نویسه *branchname,
		       ثابت نویسه *remotename, صحیح mirror, ساختار strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	اگر (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	وگرنه
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_set_multivar(the_repository, کلید, tmp->buf, "^$", 0);
}

ایستا ثابت نویسه mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

ایستا صحیح parse_mirror_opt(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح not)
{
	unsigned *mirror = opt->مقدار;
	اگر (not)
		*mirror = MIRROR_NONE;
	وگرنه اگر (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	وگرنه اگر (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	وگرنه اگر (!strcmp(arg, "push"))
		*mirror = MIRROR_افزودن;
	وگرنه
		بازگشت error(_("unknown --mirror argument: %s"), arg);
	بازگشت 0;
}

ایستا صحیح check_remote_collision(ساختار remote *remote, تهی *data)
{
	ثابت نویسه *name = data;
	ثابت نویسه *p;

	اگر (skip_prefix(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	اگر (skip_prefix(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	بازگشت 0;
}

ایستا صحیح add(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
	       ساختار repository *repo UNUSED)
{
	صحیح fetch = 0, fetch_tags = TAGS_DEFAULT;
	unsigned mirror = MIRROR_NONE;
	ساختار string_list track = STRING_LIST_INIT_NODUP;
	ثابت نویسه *master = پوچ;
	ساختار remote *remote;
	ساختار strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ثابت نویسه *name, *url;
	صحیح result = 0;

	ساختار option options[] = {
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

	اگر (argc != 2)
		usage_with_options(builtin_remote_add_usage, options);

	اگر (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	اگر (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	اگر (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		exit(3);
	}

	اگر (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (تهی *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_set(the_repository, buf.buf, url);

	اگر (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		اگر (track.nr == 0)
			string_list_append(&track, "*");
		برای (size_t i = 0; i < track.nr; i++) {
			add_branch(buf.buf, track.اقلام[i].string,
				   name, mirror, &buf2);
		}
	}

	اگر (mirror & MIRROR_افزودن) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_set(the_repository, buf.buf, "true");
	}

	اگر (fetch_tags != TAGS_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_set(the_repository, buf.buf,
				fetch_tags == TAGS_SET ? "--tags" : "--no-tags");
	}

	اگر (fetch && fetch_remote(name)) {
		result = 1;
		برو out;
	}

	اگر (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		اگر (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			result = error(_("Could not setup master '%s'"), master);
	}

out:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	بازگشت result;
}

ساختار branch_info {
	نویسه *remote_name;
	ساختار string_list merge;
	شمارش rebase_type rebase;
	نویسه *افزودن_remote_name;
};

ایستا ساختار string_list branch_list = STRING_LIST_INIT_DUP;

ایستا ثابت نویسه *abbrev_ref(ثابت نویسه *name, ثابت نویسه *prefix)
{
	skip_prefix(name, prefix, &name);
	بازگشت name;
}
#define abbrev_branch(name) abbrev_ref((name), "refs/heads/")

ایستا صحیح config_read_branches(ثابت نویسه *کلید, ثابت نویسه *مقدار,
				ثابت ساختار config_context *ctx UNUSED,
				تهی *data UNUSED)
{
	ثابت نویسه *orig_کلید = کلید;
	نویسه *name;
	ساختار string_list_item *item;
	ساختار branch_info *info;
	شمارش { REMOTE, MERGE, REBASE, افزودن_REMOTE } type;
	size_t کلید_len;

	اگر (!starts_with(کلید, "branch."))
		بازگشت 0;

	کلید += strlen("branch.");
	اگر (strip_suffix(کلید, ".remote", &کلید_len))
		type = REMOTE;
	وگرنه اگر (strip_suffix(کلید, ".merge", &کلید_len))
		type = MERGE;
	وگرنه اگر (strip_suffix(کلید, ".rebase", &کلید_len))
		type = REBASE;
	وگرنه اگر (strip_suffix(کلید, ".pushremote", &کلید_len))
		type = افزودن_REMOTE;
	وگرنه
		بازگشت 0;

	name = xmemdupz(کلید, کلید_len);
	item = string_list_درج(&branch_list, name);

	اگر (!item->util)
		item->util = xcalloc(1, اندازه(ساختار branch_info));
	info = item->util;
	انتخاب (type) {
	حالت REMOTE:
		اگر (info->remote_name)
			warning(_("more than one %s"), orig_کلید);
		info->remote_name = xstrdup(مقدار);
		بشکن;
	حالت MERGE: {
		ثابت نویسه *space = strchr(مقدار, ' ');
		مقدار = abbrev_branch(مقدار);
		وقتی (space) {
			نویسه *merge;
			merge = xstrndup(مقدار, space - مقدار);
			string_list_append(&info->merge, merge);
			مقدار = abbrev_branch(space + 1);
			space = strchr(مقدار, ' ');
		}
		string_list_append(&info->merge, xstrdup(مقدار));
		بشکن;
	}
	حالت REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_مقدار(مقدار);
		اگر (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, مقدار);
		بشکن;
	حالت افزودن_REMOTE:
		اگر (info->افزودن_remote_name)
			warning(_("more than one %s"), orig_کلید);
		info->افزودن_remote_name = xstrdup(مقدار);
		بشکن;
	پیشفرض:
		BUG("unexpected type=%d", type);
	}

	آزاد(name);
	بازگشت 0;
}

ایستا تهی read_branches(تهی)
{
	اگر (branch_list.nr)
		بازگشت;
	repo_config(the_repository, config_read_branches, پوچ);
}

ساختار ref_states {
	ساختار remote *remote;
	ساختار string_list new_refs, skipped, stale, tracked, heads, افزودن;
	صحیح queried;
};

#define REF_STATES_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.افزودن = STRING_LIST_INIT_DUP, \
}

ایستا صحیح get_ref_states(ثابت ساختار ref *remote_refs, ساختار ref_states *states)
{
	ساختار ref *fetch_map = پوچ, **tail = &fetch_map;
	ساختار ref *ref, *stale_refs;
	صحیح i;

	برای (i = 0; i < states->remote->fetch.nr; i++)
		اگر (get_fetch_map(remote_refs, &states->remote->fetch.اقلام[i], &tail, 1))
			die(_("Could not get fetch map for refspec %s"),
				states->remote->fetch.اقلام[i].raw);

	برای (ref = fetch_map; ref; ref = ref->next) {
		اگر (refname_matches_negative_refspec_item(ref->name, &states->remote->fetch))
			string_list_append(&states->skipped, abbrev_branch(ref->name));
		وگرنه اگر (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&states->new_refs, abbrev_branch(ref->name));
		وگرنه
			string_list_append(&states->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&states->remote->fetch, fetch_map);
	برای (ref = stale_refs; ref; ref = ref->next) {
		ساختار string_list_item *item =
			string_list_append(&states->stale, abbrev_branch(ref->name));
		item->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_sort(&states->new_refs);
	string_list_sort(&states->skipped);
	string_list_sort(&states->tracked);
	string_list_sort(&states->stale);

	بازگشت 0;
}

ساختار افزودن_info {
	نویسه *dest;
	صحیح forced;
	شمارش {
		افزودن_STATUS_CREATE = 0,
		افزودن_STATUS_DELETE,
		افزودن_STATUS_UPTODATE,
		افزودن_STATUS_FASTFORWARD,
		افزودن_STATUS_OUTOFDATE,
		افزودن_STATUS_NOTQUERIED
	} status;
};

ایستا صحیح get_افزودن_ref_states(ثابت ساختار ref *remote_refs,
	ساختار ref_states *states)
{
	ساختار remote *remote = states->remote;
	ساختار ref *ref, *local_refs, *افزودن_map;
	اگر (remote->mirror)
		بازگشت 0;

	local_refs = get_local_heads();
	افزودن_map = copy_ref_list(remote_refs);

	match_افزودن_refs(local_refs, &افزودن_map, &remote->افزودن, MATCH_REFS_NONE);

	برای (ref = افزودن_map; ref; ref = ref->next) {
		ساختار string_list_item *item;
		ساختار افزودن_info *info;

		اگر (!ref->peer_ref)
			ادامه;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		item = string_list_append(&states->افزودن,
					  abbrev_branch(ref->peer_ref->name));
		item->util = xcalloc(1, اندازه(ساختار افزودن_info));
		info = item->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		اگر (is_null_oid(&ref->new_oid)) {
			info->status = افزودن_STATUS_DELETE;
		} وگرنه اگر (oideq(&ref->old_oid, &ref->new_oid))
			info->status = افزودن_STATUS_UPTODATE;
		وگرنه اگر (is_null_oid(&ref->old_oid))
			info->status = افزودن_STATUS_CREATE;
		وگرنه اگر (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->status = افزودن_STATUS_FASTFORWARD;
		وگرنه
			info->status = افزودن_STATUS_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(افزودن_map);
	بازگشت 0;
}

ایستا صحیح get_افزودن_ref_states_noquery(ساختار ref_states *states)
{
	صحیح i;
	ساختار remote *remote = states->remote;
	ساختار string_list_item *item;
	ساختار افزودن_info *info;

	اگر (remote->mirror)
		بازگشت 0;

	اگر (!remote->افزودن.nr) {
		item = string_list_append(&states->افزودن, _("(matching)"));
		info = item->util = xcalloc(1, اندازه(ساختار افزودن_info));
		info->status = افزودن_STATUS_NOTQUERIED;
		info->dest = xstrdup(item->string);
	}
	برای (i = 0; i < remote->افزودن.nr; i++) {
		ثابت ساختار refspec_item *spec = &remote->افزودن.اقلام[i];
		اگر (spec->matching)
			item = string_list_append(&states->افزودن, _("(matching)"));
		وگرنه اگر (strlen(spec->src))
			item = string_list_append(&states->افزودن, spec->src);
		وگرنه
			item = string_list_append(&states->افزودن, _("(delete)"));

		info = item->util = xcalloc(1, اندازه(ساختار افزودن_info));
		info->forced = spec->force;
		info->status = افزودن_STATUS_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : item->string);
	}
	بازگشت 0;
}

ایستا صحیح get_head_names(ثابت ساختار ref *remote_refs, ساختار ref_states *states)
{
	ساختار ref *ref, *matches;
	ساختار ref *fetch_map = پوچ, **fetch_map_tail = &fetch_map;
	ساختار refspec_item refspec = {
		.force = 0,
		.pattern = 1,
		.src = (نویسه *) "refs/heads/*",
		.dst = (نویسه *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_tail, 0);
	matches = guess_remote_head(find_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_HEAD_ALL);
	برای (ref = matches; ref; ref = ref->next)
		string_list_append(&states->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	بازگشت 0;
}

ساختار known_remote {
	ساختار known_remote *next;
	ساختار remote *remote;
};

ساختار known_remotes {
	ساختار remote *to_delete;
	ساختار known_remote *list;
};

ایستا صحیح add_known_remote(ساختار remote *remote, تهی *cb_data)
{
	ساختار known_remotes *all = cb_data;
	ساختار known_remote *r;

	اگر (!strcmp(all->to_delete->name, remote->name))
		بازگشت 0;

	r = xmalloc(اندازه(*r));
	r->remote = remote;
	r->next = all->list;
	all->list = r;
	بازگشت 0;
}

ساختار branches_for_remote {
	ساختار remote *remote;
	ساختار string_list *branches, *skipped;
	ساختار known_remotes *keep;
};

ایستا صحیح add_branch_for_removal(ثابت ساختار reference *ref, تهی *cb_data)
{
	ساختار branches_for_remote *branches = cb_data;
	ساختار refspec_item refspec;
	ساختار known_remote *kr;

	memset(&refspec, 0, اندازه(refspec));
	refspec.dst = (نویسه *)ref->name;
	اگر (remote_find_tracking(branches->remote, &refspec))
		بازگشت 0;
	آزاد(refspec.src);

	/* don't delete a branch if another remote also uses it */
	برای (kr = branches->keep->list; kr; kr = kr->next) {
		memset(&refspec, 0, اندازه(refspec));
		refspec.dst = (نویسه *)ref->name;
		اگر (!remote_find_tracking(kr->remote, &refspec)) {
			آزاد(refspec.src);
			بازگشت 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	اگر (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		اگر (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		بازگشت 0;
	}

	string_list_append(branches->branches, ref->name);

	بازگشت 0;
}

ساختار rename_info {
	ثابت نویسه *old_name;
	ثابت نویسه *new_name;
	ساختار ref_transaction *transaction;
	ساختار progress *progress;
	ساختار strbuf *err;
	uint32_t progress_nr;
	uint64_t index;
};

ایستا تهی compute_renamed_ref(ساختار rename_info *rename,
				ثابت نویسه *refname,
				ساختار strbuf *out)
{
	strbuf_reset(out);
	strbuf_addstr(out, refname);
	strbuf_splice(out, strlen("refs/remotes/"), strlen(rename->old_name),
		      rename->new_name, strlen(rename->new_name));
}

ایستا صحیح rename_one_reflog_entry(ثابت نویسه *old_refname,
				   ساختار object_id *old_oid,
				   ساختار object_id *new_oid,
				   ثابت نویسه *committer,
				   timestamp_t timestamp, صحیح tz,
				   ثابت نویسه *پیام, تهی *cb_data)
{
	ساختار rename_info *rename = cb_data;
	ساختار strbuf new_refname = STRBUF_INIT;
	ساختار strbuf identity = STRBUF_INIT;
	ساختار strbuf name = STRBUF_INIT;
	ساختار strbuf mail = STRBUF_INIT;
	ساختار ident_split ident;
	ثابت نویسه *date;
	صحیح error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	اگر (split_ident_سطر(&ident, committer, strlen(committer)) < 0) {
		error = -1;
		برو out;
	}

	strbuf_add(&name, ident.name_begin, ident.name_end - ident.name_begin);
	strbuf_add(&mail, ident.mail_begin, ident.mail_end - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_MODE(NORMAL));
	strbuf_addstr(&identity, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, identity.buf, پیام,
					      rename->index++, rename->err);

out:
	strbuf_release(&new_refname);
	strbuf_release(&identity);
	strbuf_release(&name);
	strbuf_release(&mail);
	بازگشت error;
}

ایستا صحیح rename_one_reflog(ثابت نویسه *old_refname,
			     ثابت ساختار object_id *old_oid,
			     ساختار rename_info *rename)
{
	ساختار strbuf new_refname = STRBUF_INIT;
	ساختار strbuf message = STRBUF_INIT;
	صحیح error;

	اگر (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		بازگشت 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_entry, rename);
	اگر (error < 0)
		برو out;

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
	اگر (error < 0)
		بازگشت error;

out:
	strbuf_release(&new_refname);
	strbuf_release(&message);
	بازگشت error;
}

ایستا صحیح rename_one_ref(ثابت ساختار reference *ref, تهی *cb_data)
{
	ساختار strbuf new_referent = STRBUF_INIT;
	ساختار strbuf new_refname = STRBUF_INIT;
	ساختار rename_info *rename = cb_data;
	ثابت ساختار object_id *oid = ref->oid;
	ثابت نویسه *referent = ref->target;
	صحیح error;

	compute_renamed_ref(rename, ref->name, &new_refname);

	اگر (ref->flags & REF_ISSYMREF) {
		/*
		 * Stupidly enough `referent` is not pointing to the immediate
		 * target of a symref, but it's the recursively resolved value.
		 * So symrefs pointing to symrefs would be misresolved, and
		 * unborn symrefs don't have any value for the `referent` at all.
		 */
		referent = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
						   ref->name, RESOLVE_REF_NO_RECURSE,
						   پوچ, پوچ);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = پوچ;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, پوچ, rename->err);
	اگر (error < 0)
		برو out;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : پوچ, پوچ,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       پوچ, rename->err);
	اگر (error < 0)
		برو out;

	error = rename_one_reflog(ref->name, oid, rename);
	اگر (error < 0)
		برو out;

	display_progress(rename->progress, ++rename->progress_nr);

out:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	بازگشت error;
}

ایستا صحیح migrate_file(ساختار remote *remote)
{
	ساختار strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	برای (size_t i = 0; i < remote->url.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	برای (صحیح i = 0; i < remote->افزودن.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->افزودن.اقلام[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	برای (صحیح i = 0; i < remote->fetch.nr; i++)
		repo_config_set_multivar(the_repository, buf.buf, remote->fetch.اقلام[i].raw, "^$", 0);
#ifndef WITH_BREAKING_CHANGES
	اگر (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	وگرنه اگر (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_path_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#endif /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	بازگشت 0;
}

ساختار افزودن_default_info
{
	ثابت نویسه *old_name;
	شمارش config_scope scope;
	ساختار strbuf origin;
	صحیح linenr;
};

ایستا صحیح config_read_افزودن_default(ثابت نویسه *کلید, ثابت نویسه *مقدار,
	ثابت ساختار config_context *ctx, تهی *cb)
{
	ثابت ساختار کلید_مقدار_info *kvi = ctx->kvi;

	ساختار افزودن_default_info* info = cb;
	اگر (strcmp(کلید, "remote.pushdefault") ||
	    !مقدار || strcmp(مقدار, info->old_name))
		بازگشت 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	بازگشت 0;
}

ایستا تهی handle_افزودن_default(ثابت نویسه* old_name, ثابت نویسه* new_name)
{
	ساختار افزودن_default_info افزودن_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_افزودن_default, &افزودن_default);
	اگر (افزودن_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	وگرنه اگر (افزودن_default.scope >= CONFIG_SCOPE_LOCAL) {
		صحیح result = repo_config_set_gently(the_repository, "remote.pushDefault",
						    new_name);
		اگر (new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not set '%s'"), "remote.pushDefault");
		وگرنه اگر (!new_name && result && result != CONFIG_NOTHING_SET)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} وگرنه اگر (افزودن_default.scope >= CONFIG_SCOPE_SYSTEM) {
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

ایستا ثابت نویسه conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

ایستا صحیح mv(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
	      ساختار repository *repo UNUSED)
{
	صحیح show_progress = isatty(2);
	ساختار option options[] = {
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_END()
	};
	ساختار remote *oldremote, *newremote;
	ساختار strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	ساختار rename_info rename = {
		.err = &err,
	};
	صحیح refspecs_need_update = 0;
	صحیح result = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rename_usage, 0);

	اگر (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	اگر (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		exit(2);
	}

	اگر (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		بازگشت migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	اگر (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		exit(3);
	}

	اگر (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	اگر (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		result = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		برو out;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	برای (صحیح i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!strstr(oldremote->fetch.اقلام[i].raw,
						old_remote_context.buf);

	اگر (refspecs_need_update) {
		ساختار refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		اگر (!rename.transaction)
			برو out;

		اگر (show_progress)
			rename.progress = start_delayed_progress(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.prefix = buf.buf;

		result = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		اگر (result < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		result = ref_transaction_prepare(rename.transaction, &err);
		اگر (result < 0) {
			error("renaming remote references failed: %s", err.buf);
			اگر (result == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(پوچ);
		}
	}

	اگر (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_set_multivar(the_repository, buf.buf, پوچ, پوچ, CONFIG_FLAGS_MULTI_REPLACE);
		برای (صحیح i = 0; i < oldremote->fetch.nr; i++) {
			نویسه *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.اقلام[i].raw);
			ptr = strstr(buf2.buf, old_remote_context.buf);
			اگر (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + strlen(":refs/remotes/"),
					      strlen(rename.old_name), rename.new_name,
					      strlen(rename.new_name));
			} وگرنه
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_set_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	برای (size_t i = 0; i < branch_list.nr; i++) {
		ساختار string_list_item *item = branch_list.اقلام + i;
		ساختار branch_info *info = item->util;
		اگر (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
		اگر (info->افزودن_remote_name && !strcmp(info->افزودن_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", item->string);
			repo_config_set(the_repository, buf.buf, rename.new_name);
		}
	}

	اگر (refspecs_need_update) {
		result = ref_transaction_commit(rename.transaction, &err);
		اگر (result < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_progress(&rename.progress);

		handle_افزودن_default(rename.old_name, rename.new_name);
	}

out:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	بازگشت result;
}

ایستا صحیح rm(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
	      ساختار repository *repo UNUSED)
{
	ساختار option options[] = {
		OPT_END()
	};
	ساختار remote *remote;
	ساختار strbuf buf = STRBUF_INIT;
	ساختار known_remotes known_remotes = { پوچ, پوچ };
	ساختار string_list branches = STRING_LIST_INIT_DUP;
	ساختار string_list skipped = STRING_LIST_INIT_DUP;
	ساختار branches_for_remote cb_data;
	صحیح result;

	memset(&cb_data, 0, اندازه(cb_data));
	cb_data.branches = &branches;
	cb_data.skipped = &skipped;
	cb_data.keep = &known_remotes;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_rm_usage, 0);
	اگر (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	اگر (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		exit(2);
	}

	known_remotes.to_delete = remote;
	for_each_remote(add_known_remote, &known_remotes);

	read_branches();
	برای (size_t i = 0; i < branch_list.nr; i++) {
		ساختار string_list_item *item = branch_list.اقلام + i;
		ساختار branch_info *info = item->util;
		اگر (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ثابت نویسه *کلیدها[] = { "remote", "merge", پوچ }, **k;
			برای (k = کلیدها; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						item->string, *k);
				result = repo_config_set_gently(the_repository, buf.buf, پوچ);
				اگر (result && result != CONFIG_NOTHING_SET)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		اگر (info->افزودن_remote_name && !strcmp(info->افزودن_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", item->string);
			result = repo_config_set_gently(the_repository, buf.buf, پوچ);
			اگر (result && result != CONFIG_NOTHING_SET)
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

	اگر (!result)
		result = refs_delete_refs(get_main_ref_store(the_repository),
					  "remote: remove", &branches,
					  REF_NO_DEREF);
	string_list_clear(&branches, 0);

	اگر (skipped.nr) {
		fprintf_ln(stderr,
			   Q_("Note: A branch outside the refs/remotes/ hierarchy was not removed;\n"
			      "to delete it, use:",
			      "Note: Some branches outside the refs/remotes/ hierarchy were not removed;\n"
			      "to delete them, use:",
			      skipped.nr));
		برای (size_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.اقلام[i].string);
	}
	string_list_clear(&skipped, 0);

	اگر (!result) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		اگر (repo_config_rename_section(the_repository, buf.buf, پوچ) < 1) {
			result = error(_("Could not remove config section '%s'"), buf.buf);
			برو out;
		}

		handle_افزودن_default(remote->name, پوچ);
	}

out:
	برای (ساختار known_remote *r = known_remotes.list; r;) {
		ساختار known_remote *next = r->next;
		آزاد(r);
		r = next;
	}
	strbuf_release(&buf);
	بازگشت result;
}

ایستا تهی clear_افزودن_info(تهی *util, ثابت نویسه *string UNUSED)
{
	ساختار افزودن_info *info = util;
	آزاد(info->dest);
	آزاد(info);
}

ایستا تهی free_remote_ref_states(ساختار ref_states *states)
{
	string_list_clear(&states->new_refs, 0);
	string_list_clear(&states->skipped, 0);
	string_list_clear(&states->stale, 1);
	string_list_clear(&states->tracked, 0);
	string_list_clear(&states->heads, 0);
	string_list_clear_func(&states->افزودن, clear_افزودن_info);
}

ایستا صحیح append_ref_to_tracked_list(ثابت ساختار reference *ref, تهی *cb_data)
{
	ساختار ref_states *states = cb_data;
	ساختار refspec_item refspec;

	اگر (ref->flags & REF_ISSYMREF)
		بازگشت 0;

	memset(&refspec, 0, اندازه(refspec));
	refspec.dst = (نویسه *)ref->name;
	اگر (!remote_find_tracking(states->remote, &refspec)) {
		string_list_append(&states->tracked, abbrev_branch(refspec.src));
		آزاد(refspec.src);
	}

	بازگشت 0;
}

ایستا صحیح get_remote_ref_states(ثابت نویسه *name,
				 ساختار ref_states *states,
				 صحیح query)
{
	states->remote = remote_get(name);
	اگر (!states->remote)
		بازگشت error(_("No such remote: '%s'"), name);

	read_branches();

	اگر (query) {
		ساختار transport *transport;
		ثابت ساختار ref *remote_refs;

		transport = transport_get(states->remote, states->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, پوچ);

		states->queried = 1;
		اگر (query & GET_REF_STATES)
			get_ref_states(remote_refs, states);
		اگر (query & GET_HEAD_NAMES)
			get_head_names(remote_refs, states);
		اگر (query & GET_افزودن_REF_STATES)
			get_افزودن_ref_states(remote_refs, states);
		transport_disconnect(transport);
	} وگرنه {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_to_tracked_list, states);
		string_list_sort(&states->tracked);
		get_افزودن_ref_states_noquery(states);
	}

	بازگشت 0;
}

ساختار show_info {
	ساختار string_list list;
	ساختار ref_states states;
	صحیح width, width2;
	صحیح any_rebase;
};

#define SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.states = REF_STATES_INIT, \
}

ایستا صحیح add_remote_to_show_info(ساختار string_list_item *item, تهی *cb_data)
{
	ساختار show_info *info = cb_data;
	صحیح n = strlen(item->string);
	اگر (n > info->width)
		info->width = n;
	string_list_درج(&info->list, item->string);
	بازگشت 0;
}

ایستا صحیح show_remote_info_item(ساختار string_list_item *item, تهی *cb_data)
{
	ساختار show_info *info = cb_data;
	ساختار ref_states *states = &info->states;
	ثابت نویسه *name = item->string;

	اگر (states->queried) {
		ثابت نویسه *fmt = "%s";
		ثابت نویسه *arg = "";
		اگر (string_list_has_string(&states->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = states->remote->name;
		} وگرنه اگر (string_list_has_string(&states->tracked, name))
			arg = _(" tracked");
		وگرنه اگر (string_list_has_string(&states->skipped, name))
			arg = _(" skipped");
		وگرنه اگر (string_list_has_string(&states->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		وگرنه
			arg = _(" ???");
		printf("    %-*s", info->width, name);
		printf(fmt, arg);
		printf("\n");
	} وگرنه
		printf("    %s\n", name);

	بازگشت 0;
}

ایستا صحیح add_local_to_show_info(ساختار string_list_item *branch_item, تهی *cb_data)
{
	ساختار show_info *show_info = cb_data;
	ساختار ref_states *states = &show_info->states;
	ساختار branch_info *branch_info = branch_item->util;
	ساختار string_list_item *item;
	صحیح n;

	اگر (!branch_info->merge.nr || !branch_info->remote_name ||
	    strcmp(states->remote->name, branch_info->remote_name))
		بازگشت 0;
	اگر ((n = strlen(branch_item->string)) > show_info->width)
		show_info->width = n;
	اگر (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	item = string_list_درج(&show_info->list, branch_item->string);
	item->util = branch_info;

	بازگشت 0;
}

ایستا صحیح show_local_info_item(ساختار string_list_item *item, تهی *cb_data)
{
	ساختار show_info *show_info = cb_data;
	ساختار branch_info *branch_info = item->util;
	ساختار string_list *merge = &branch_info->merge;
	صحیح width = show_info->width + 4;

	اگر (branch_info->rebase >= REBASE_TRUE && branch_info->merge.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			item->string);
		بازگشت 0;
	}

	printf("    %-*s ", show_info->width, item->string);
	اگر (branch_info->rebase >= REBASE_TRUE) {
		ثابت نویسه *پیام;
		اگر (branch_info->rebase == REBASE_INTERACTIVE)
			پیام = _("rebases interactively onto remote %s");
		وگرنه اگر (branch_info->rebase == REBASE_MERGES)
			پیام = _("rebases interactively (with merges) onto "
				"remote %s");
		وگرنه
			پیام = _("rebases onto remote %s");
		printf_ln(پیام, merge->اقلام[0].string);
		بازگشت 0;
	} وگرنه اگر (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), merge->اقلام[0].string);
		width++;
	} وگرنه {
		printf_ln(_("merges with remote %s"), merge->اقلام[0].string);
	}
	برای (size_t i = 1; i < merge->nr; i++)
		printf(_("%-*s    and with remote %s\n"), width, "",
		       merge->اقلام[i].string);

	بازگشت 0;
}

ایستا صحیح add_افزودن_to_show_info(ساختار string_list_item *افزودن_item, تهی *cb_data)
{
	ساختار show_info *show_info = cb_data;
	ساختار افزودن_info *افزودن_info = افزودن_item->util;
	ساختار string_list_item *item;
	صحیح n;
	اگر ((n = strlen(افزودن_item->string)) > show_info->width)
		show_info->width = n;
	اگر ((n = strlen(افزودن_info->dest)) > show_info->width2)
		show_info->width2 = n;
	item = string_list_append(&show_info->list, افزودن_item->string);
	item->util = افزودن_item->util;
	بازگشت 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
ایستا صحیح cmp_string_with_افزودن(ثابت تهی *va, ثابت تهی *vb)
{
	ثابت ساختار string_list_item *a = va;
	ثابت ساختار string_list_item *b = vb;
	ثابت ساختار افزودن_info *a_افزودن = a->util;
	ثابت ساختار افزودن_info *b_افزودن = b->util;
	صحیح cmp = strcmp(a->string, b->string);
	بازگشت cmp ? cmp : strcmp(a_افزودن->dest, b_افزودن->dest);
}

ایستا صحیح show_افزودن_info_item(ساختار string_list_item *item, تهی *cb_data)
{
	ساختار show_info *show_info = cb_data;
	ساختار افزودن_info *افزودن_info = item->util;
	ثابت نویسه *src = item->string, *status = پوچ;

	انتخاب (افزودن_info->status) {
	حالت افزودن_STATUS_CREATE:
		status = _("create");
		بشکن;
	حالت افزودن_STATUS_DELETE:
		status = _("delete");
		src = _("(none)");
		بشکن;
	حالت افزودن_STATUS_UPTODATE:
		status = _("up to date");
		بشکن;
	حالت افزودن_STATUS_FASTFORWARD:
		status = _("fast-forwardable");
		بشکن;
	حالت افزودن_STATUS_OUTOFDATE:
		status = _("local out of date");
		بشکن;
	حالت افزودن_STATUS_NOTQUERIED:
		بشکن;
	}
	اگر (status) {
		اگر (افزودن_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->width, src,
			       show_info->width2, افزودن_info->dest, status);
		وگرنه
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->width, src,
			       show_info->width2, افزودن_info->dest, status);
	} وگرنه {
		اگر (افزودن_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->width, src,
			       افزودن_info->dest);
		وگرنه
			printf_ln(_("    %-*s pushes to %s"), show_info->width, src,
			       افزودن_info->dest);
	}
	بازگشت 0;
}

ایستا صحیح get_one_entry(ساختار remote *remote, تهی *priv)
{
	ساختار string_list *list = priv;
	ساختار strbuf remote_info_buf = STRBUF_INIT;
	ساختار strvec *url;

	اگر (remote->url.nr > 0) {
		ساختار strbuf promisor_config = STRBUF_INIT;
		ثابت نویسه *partial_clone_filter = پوچ;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		اگر (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, پوچ);
	} وگرنه
		string_list_append(list, remote->name)->util = پوچ;
	url = افزودن_url_of_remote(remote);
	برای (size_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, پوچ);
	}

	بازگشت 0;
}

ایستا صحیح show_all(تهی)
{
	ساختار string_list list = STRING_LIST_INIT_DUP;
	صحیح result;

	result = for_each_remote(get_one_entry, &list);

	اگر (!result) {
		string_list_sort(&list);
		برای (size_t i = 0; i < list.nr; i++) {
			ساختار string_list_item *item = list.اقلام + i;
			اگر (verbose)
				printf("%s\t%s\n", item->string,
					item->util ? (ثابت نویسه *)item->util : "");
			وگرنه {
				اگر (i && !strcmp((item - 1)->string, item->string))
					ادامه;
				printf("%s\n", item->string);
			}
		}
	}
	string_list_clear(&list, 1);
	بازگشت result;
}

ایستا صحیح show(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		ساختار repository *repo UNUSED)
{
	صحیح no_query = 0, result = 0, query_flag = 0;
	ساختار option options[] = {
		OPT_BOOL('n', پوچ, &no_query, N_("do not query remotes")),
		OPT_END()
	};
	ساختار show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_show_usage,
			     0);

	اگر (argc < 1)
		بازگشت show_all();

	اگر (!no_query)
		query_flag = (GET_REF_STATES | GET_HEAD_NAMES | GET_افزودن_REF_STATES);

	برای (; argc; argc--, argv++) {
		size_t i;
		ساختار strvec *url;

		get_remote_ref_states(*argv, &info.states, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.states.remote->url.v[0]);
		url = افزودن_url_of_remote(info.states.remote);
		برای (i = 0; i < url->nr; i++)
			/*
			 * TRANSLATORS: the colon ':' should align
			 * with the one in " Fetch URL: %s"
			 * translation.
			 */
			printf_ln(_("  Push  URL: %s"), url->v[i]);
		اگر (!i)
			printf_ln(_("  Push  URL: %s"), _("(no URL)"));
		اگر (no_query)
			printf_ln(_("  HEAD branch: %s"), _("(not queried)"));
		وگرنه اگر (!info.states.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		وگرنه اگر (info.states.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.states.heads.اقلام[0].string);
		وگرنه {
			printf(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			برای (i = 0; i < info.states.heads.nr; i++)
				printf("    %s\n", info.states.heads.اقلام[i].string);
		}

		/* remote branch info */
		info.width = 0;
		for_each_string_list(&info.states.new_refs, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.skipped, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.tracked, add_remote_to_show_info, &info);
		for_each_string_list(&info.states.stale, add_remote_to_show_info, &info);
		اگر (info.list.nr)
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
		اگر (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_item, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		اگر (info.states.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.width = info.width2 = 0;
		for_each_string_list(&info.states.افزودن, add_افزودن_to_show_info, &info);
		QSORT(info.list.اقلام, info.list.nr, cmp_string_with_افزودن);
		اگر (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_افزودن_info_item, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_states(&info.states);
	}

	بازگشت result;
}

ایستا تهی report_set_head_auto(ثابت نویسه *remote, ثابت نویسه *head_name,
			ساختار strbuf *b_local_head, صحیح was_detached) {
	ساختار strbuf buf_prefix = STRBUF_INIT;
	ثابت نویسه *prev_head = پوچ;

	strbuf_addf(&buf_prefix, "refs/remotes/%s/", remote);
	skip_prefix(b_local_head->buf, buf_prefix.buf, &prev_head);

	اگر (prev_head && !strcmp(prev_head, head_name))
		printf(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, head_name);
	وگرنه اگر (prev_head)
		printf(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_head, head_name);
	وگرنه اگر (!b_local_head->len)
		printf(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, head_name);
	وگرنه اگر (was_detached && b_local_head->len)
		printf(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	وگرنه
		printf(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_head->buf, head_name);
	strbuf_release(&buf_prefix);
}

ایستا صحیح set_head(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		    ساختار repository *repo UNUSED)
{
	صحیح opt_a = 0, opt_d = 0, result = 0, was_detached;
	ساختار strbuf b_head = STRBUF_INIT, b_remote_head = STRBUF_INIT,
		b_local_head = STRBUF_INIT;
	نویسه *head_name = پوچ;
	ساختار ref_store *refs = get_main_ref_store(the_repository);
	ساختار remote *remote;

	ساختار option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	اگر (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_head, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	اگر (!opt_a && !opt_d && argc == 2) {
		head_name = xstrdup(argv[1]);
	} وگرنه اگر (opt_a && !opt_d && argc == 1) {
		ساختار ref_states states = REF_STATES_INIT;
		get_remote_ref_states(argv[0], &states, GET_HEAD_NAMES);
		اگر (!states.heads.nr)
			result |= error(_("Cannot determine remote HEAD"));
		وگرنه اگر (states.heads.nr > 1) {
			result |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			برای (size_t i = 0; i < states.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], states.heads.اقلام[i].string);
		} وگرنه
			head_name = xstrdup(states.heads.اقلام[0].string);
		free_remote_ref_states(&states);
	} وگرنه اگر (opt_d && !opt_a && argc == 1) {
		اگر (refs_delete_ref(refs, پوچ, b_head.buf, پوچ, REF_NO_DEREF))
			result |= error(_("Could not delete %s"), b_head.buf);
	} وگرنه
		usage_with_options(builtin_remote_sethead_usage, options);

	اگر (!head_name)
		برو cleanup;
	strbuf_addf(&b_remote_head, "refs/remotes/%s/%s", argv[0], head_name);
	اگر (!refs_ref_exists(refs, b_remote_head.buf)) {
		result |= error(_("Not a valid ref: %s"), b_remote_head.buf);
		برو cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_head.buf, b_remote_head.buf,
			"remote set-head", &b_local_head, 0);
	اگر (was_detached == -1) {
		result |= error(_("Could not set up %s"), b_head.buf);
		برو cleanup;
	}
	اگر (opt_a)
		report_set_head_auto(argv[0], head_name, &b_local_head, was_detached);
	اگر (remote->follow_remote_head == FOLLOW_REMOTE_ALWAYS) {
		ساختار strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_set(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	آزاد(head_name);
	strbuf_release(&b_head);
	strbuf_release(&b_remote_head);
	strbuf_release(&b_local_head);
	بازگشت result;
}

ایستا صحیح prune_remote(ثابت نویسه *remote, صحیح dry_run)
{
	صحیح result = 0;
	ساختار ref_states states = REF_STATES_INIT;
	ساختار string_list refs_to_prune = STRING_LIST_INIT_NODUP;
	ساختار string_list_item *item;

	get_remote_ref_states(remote, &states, GET_REF_STATES);

	اگر (!states.stale.nr) {
		free_remote_ref_states(&states);
		بازگشت 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), states.remote->url.v[0]);

	for_each_string_list_item(item, &states.stale)
		string_list_append(&refs_to_prune, item->util);
	string_list_sort(&refs_to_prune);

	اگر (!dry_run)
		result |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_to_prune, 0);

	for_each_string_list_item(item, &states.stale) {
		ثابت نویسه *refname = item->util;

		اگر (dry_run)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		وگرنه
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_run, &refs_to_prune);

	string_list_clear(&refs_to_prune, 0);
	free_remote_ref_states(&states);
	بازگشت result;
}

ایستا صحیح prune(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		 ساختار repository *repo UNUSED)
{
	صحیح dry_run = 0, result = 0;
	ساختار option options[] = {
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_prune_usage, 0);

	اگر (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	برای (; argc; argc--, argv++)
		result |= prune_remote(*argv, dry_run);

	بازگشت result;
}

ایستا صحیح get_remote_default(ثابت نویسه *کلید, ثابت نویسه *مقدار UNUSED,
			      ثابت ساختار config_context *ctx UNUSED,
			      تهی *priv)
{
	اگر (strcmp(کلید, "remotes.default") == 0) {
		صحیح *found = priv;
		*found = 1;
	}
	بازگشت 0;
}

ایستا صحیح update(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		  ساختار repository *repo UNUSED)
{
	صحیح i, prune = -1;
	ساختار option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_END()
	};
	ساختار child_process cmd = CHILD_PROCESS_INIT;
	صحیح default_defined = 0;

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_افزودن(&cmd.args, "fetch");

	اگر (prune != -1)
		strvec_افزودن(&cmd.args, prune ? "--prune" : "--no-prune");
	اگر (verbose)
		strvec_افزودن(&cmd.args, "-v");
	strvec_افزودن(&cmd.args, "--multiple");
	اگر (argc < 2)
		strvec_افزودن(&cmd.args, "default");
	برای (i = 1; i < argc; i++)
		strvec_افزودن(&cmd.args, argv[i]);

	اگر (strcmp(cmd.args.v[cmd.args.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		اگر (!default_defined) {
			strvec_برداشت(&cmd.args);
			strvec_افزودن(&cmd.args, "--all");
		}
	}

	cmd.git_cmd = 1;
	بازگشت run_command(&cmd);
}

ایستا صحیح remove_all_fetch_refspecs(ثابت نویسه *کلید)
{
	بازگشت repo_config_set_multivar_gently(the_repository, کلید, پوچ, پوچ,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

ایستا تهی add_branches(ساختار remote *remote, ثابت نویسه **branches,
			 ثابت نویسه *کلید)
{
	ثابت نویسه *remotename = remote->name;
	صحیح mirror = remote->mirror;
	ساختار strbuf refspec = STRBUF_INIT;

	برای (; *branches; branches++)
		add_branch(کلید, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

ایستا صحیح set_remote_branches(ثابت نویسه *remotename, ثابت نویسه **branches,
				صحیح add_mode)
{
	ساختار strbuf کلید = STRBUF_INIT;
	ساختار remote *remote;

	strbuf_addf(&کلید, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	اگر (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	اگر (!add_mode && remove_all_fetch_refspecs(کلید.buf)) {
		strbuf_release(&کلید);
		بازگشت 1;
	}
	add_branches(remote, branches, کلید.buf);

	strbuf_release(&کلید);
	بازگشت 0;
}

ایستا صحیح set_branches(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			ساختار repository *repo UNUSED)
{
	صحیح add_mode = 0;
	ساختار option options[] = {
		OPT_BOOL('\0', "add", &add_mode, N_("add branch")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_setbranches_usage, 0);
	اگر (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = پوچ;

	بازگشت set_remote_branches(argv[0], argv + 1, add_mode);
}

ایستا صحیح get_url(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		   ساختار repository *repo UNUSED)
{
	صحیح افزودن_mode = 0, all_mode = 0;
	ثابت نویسه *remotename = پوچ;
	ساختار remote *remote;
	ساختار strvec *url;
	ساختار option options[] = {
		OPT_BOOL('\0', "push", &افزودن_mode,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_mode,
			 N_("return all URLs")),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options,
			     builtin_remote_geturl_usage, 0);

	اگر (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	اگر (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	url = افزودن_mode ? افزودن_url_of_remote(remote) : &remote->url;

	اگر (all_mode) {
		برای (size_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} وگرنه {
		printf_ln("%s", url->v[0]);
	}

	بازگشت 0;
}

ایستا صحیح set_url(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
		   ساختار repository *repo UNUSED)
{
	صحیح افزودن_mode = 0, add_mode = 0, delete_mode = 0;
	صحیح matches = 0, negative_matches = 0;
	ثابت نویسه *remotename = پوچ;
	ثابت نویسه *newurl = پوچ;
	ثابت نویسه *oldurl = پوچ;
	ساختار remote *remote;
	regex_t old_regex;
	ساختار strvec *urlset;
	ساختار strbuf name_buf = STRBUF_INIT;
	ساختار option options[] = {
		OPT_BOOL('\0', "push", &افزودن_mode,
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

	اگر (add_mode && delete_mode)
		die(_("--add --delete doesn't make sense"));

	اگر (argc < 3 || argc > 4 || ((add_mode || delete_mode) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	اگر (argc > 3)
		oldurl = argv[3];

	اگر (delete_mode)
		oldurl = newurl;

	remote = remote_get(remotename);
	اگر (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		exit(2);
	}

	اگر (افزودن_mode) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} وگرنه {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	اگر ((!oldurl && !delete_mode) || add_mode) {
		اگر (add_mode)
			repo_config_set_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		وگرنه
			repo_config_set(the_repository, name_buf.buf, newurl);
		برو out;
	}

	/* Old URL specified. Demand that one matches. */
	اگر (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	برای (size_t i = 0; i < urlset->nr; i++)
		اگر (!regexec(&old_regex, urlset->v[i], 0, پوچ, 0))
			matches++;
		وگرنه
			negative_matches++;
	اگر (!delete_mode && !matches)
		die(_("No such URL found: %s"), oldurl);
	اگر (delete_mode && !negative_matches && !افزودن_mode)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	اگر (!delete_mode)
		repo_config_set_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	وگرنه
		repo_config_set_multivar(the_repository, name_buf.buf, پوچ, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
out:
	strbuf_release(&name_buf);
	بازگشت 0;
}

صحیح cmd_remote(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repo)
{
	parse_opt_subcommand_fn *fn = پوچ;
	ساختار option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, پوچ, PARSE_OPT_NOCOMPLETE),
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

	اگر (fn) {
		بازگشت !!fn(argc, argv, prefix, repo);
	} وگرنه {
		اگر (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		بازگشت !!show_all();
	}
}
