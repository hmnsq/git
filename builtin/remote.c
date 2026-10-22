#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

ساکن ثابت حرف * ثابت builtin_remote_usage[] = {
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
	تہی
};

ساکن ثابت حرف * ثابت builtin_remote_جوڑو_usage[] = {
	N_("git remote add [<options>] <name> <url>"),
	تہی
};

ساکن ثابت حرف * ثابت builtin_remote_rename_usage[] = {
	N_("git remote rename [--[no-]progress] <old> <new>"),
	تہی
};

ساکن ثابت حرف * ثابت builtin_remote_rm_usage[] = {
	N_("git remote remove <name>"),
	تہی
};

ساکن ثابت حرف * ثابت builtin_remote_sethead_usage[] = {
	N_("git remote set-head <name> (-a | --auto | -d | --delete | <branch>)"),
	تہی
};

ساکن ثابت حرف * ثابت builtin_remote_setbranches_usage[] = {
	N_("git remote set-branches <name> <branch>..."),
	N_("git remote set-branches --add <name> <branch>..."),
	تہی
};

ساکن ثابت حرف * ثابت builtin_remote_show_usage[] = {
	N_("git remote show [<options>] <name>"),
	تہی
};

ساکن ثابت حرف * ثابت builtin_remote_prune_usage[] = {
	N_("git remote prune [<options>] <name>"),
	تہی
};

ساکن ثابت حرف * ثابت builtin_remote_update_usage[] = {
	N_("git remote update [<options>] [<group> | <remote>]..."),
	تہی
};

ساکن ثابت حرف * ثابت builtin_remote_geturl_usage[] = {
	N_("git remote get-url [--push] [--all] <name>"),
	تہی
};

ساکن ثابت حرف * ثابت builtin_remote_seturl_usage[] = {
	N_("git remote set-url [--push] <name> <newurl> [<oldurl>]"),
	N_("git remote set-url --add <name> <newurl>"),
	N_("git remote set-url --delete <name> <url>"),
	تہی
};

#تعریف_کرو GET_REF_حالتیں (1<<0)
#تعریف_کرو GET_سر_نام (1<<1)
#تعریف_کرو GET_ڈالو_REF_حالتیں (1<<2)

ساکن صحیح verbose;

ساکن صحیح fetch_remote(ثابت حرف *name)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

	strvec_ڈالو(&cmd.دلائل, "fetch");
	اگر (verbose)
		strvec_ڈالو(&cmd.دلائل, "-v");
	strvec_ڈالو(&cmd.دلائل, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	اگر (تسلسل_command(&cmd))
		واپس error(_("Could not fetch %s"), name);
	واپس 0;
}

شمار {
	لیبل_فہرست_UNSET = 0,
	لیبل_فہرست_DEFAULT = 1,
	لیبل_فہرست_سیٹ = 2
};

#تعریف_کرو MIRROR_NONE 0
#تعریف_کرو MIRROR_FETCH 1
#تعریف_کرو MIRROR_ڈالو 2
#تعریف_کرو MIRROR_BOTH (MIRROR_FETCH|MIRROR_ڈالو)

ساکن خالی جوڑو_branch(ثابت حرف *کلید, ثابت حرف *branchname,
		       ثابت حرف *remotename, صحیح mirror, ساخت strbuf *tmp)
{
	strbuf_reset(tmp);
	strbuf_addch(tmp, '+');
	اگر (mirror)
		strbuf_addf(tmp, "refs/%s:refs/%s",
				branchname, branchname);
	ورنہ
		strbuf_addf(tmp, "refs/heads/%s:refs/remotes/%s/%s",
				branchname, remotename, branchname);
	repo_config_سیٹ_multivar(the_repository, کلید, tmp->buf, "^$", 0);
}

ساکن ثابت حرف mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

ساکن صحیح parse_mirror_opt(ثابت ساخت option *opt, ثابت حرف *arg, صحیح not)
{
	غیرعلامتی *mirror = opt->قدر;
	اگر (not)
		*mirror = MIRROR_NONE;
	ورنہ اگر (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	ورنہ اگر (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	ورنہ اگر (!strcmp(arg, "push"))
		*mirror = MIRROR_ڈالو;
	ورنہ
		واپس error(_("unknown --mirror argument: %s"), arg);
	واپس 0;
}

ساکن صحیح check_remote_collision(ساخت remote *remote, خالی *ڈیٹا)
{
	ثابت حرف *name = ڈیٹا;
	ثابت حرف *p;

	اگر (skip_سابقہ(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	اگر (skip_سابقہ(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	واپس 0;
}

ساکن صحیح جوڑو(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
	       ساخت repository *repo غیر_مستعمل)
{
	صحیح fetch = 0, fetch_لیبل_فہرست = لیبل_فہرست_DEFAULT;
	غیرعلامتی mirror = MIRROR_NONE;
	ساخت string_list track = STRING_LIST_INIT_NODUP;
	ثابت حرف *master = تہی;
	ساخت remote *remote;
	ساخت strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ثابت حرف *name, *url;
	صحیح نتیجہ = 0;

	ساخت option options[] = {
		OPT_BOOL('f', "fetch", &fetch, N_("fetch the remote branches")),
		OPT_سیٹ_INT(0, "tags", &fetch_لیبل_فہرست,
			    N_("import all tags and associated objects when fetching\n"
			       "or do not fetch any tag at all (--no-tags)"),
			    لیبل_فہرست_سیٹ),
		OPT_STRING_LIST('t', "track", &track, N_("branch"),
				N_("branch(es) to track")),
		OPT_STRING('m', "master", &master, N_("branch"), N_("master branch")),
		OPT_CALLBACK_F(0, "mirror", &mirror, "(push|fetch)",
			N_("set up remote as a mirror to push to or fetch from"),
			PARSE_OPT_OPTARG | PARSE_OPT_COMP_ARG, parse_mirror_opt),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_remote_جوڑو_usage, 0);

	اگر (argc != 2)
		usage_with_options(builtin_remote_جوڑو_usage, options);

	اگر (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	اگر (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	اگر (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		باہر_نکلو(3);
	}

	اگر (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (خالی *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_سیٹ(the_repository, buf.buf, url);

	اگر (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		اگر (track.nr == 0)
			string_list_append(&track, "*");
		برائے (سائز_t i = 0; i < track.nr; i++) {
			جوڑو_branch(buf.buf, track.اشیاء[i].string,
				   name, mirror, &buf2);
		}
	}

	اگر (mirror & MIRROR_ڈالو) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_سیٹ(the_repository, buf.buf, "true");
	}

	اگر (fetch_لیبل_فہرست != لیبل_فہرست_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_سیٹ(the_repository, buf.buf,
				fetch_لیبل_فہرست == لیبل_فہرست_سیٹ ? "--tags" : "--no-tags");
	}

	اگر (fetch && fetch_remote(name)) {
		نتیجہ = 1;
		جاؤ برآمد;
	}

	اگر (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		اگر (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			نتیجہ = error(_("Could not setup master '%s'"), master);
	}

برآمد:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	واپس نتیجہ;
}

ساخت branch_info {
	حرف *remote_name;
	ساخت string_list ملاؤ;
	شمار rebase_type rebase;
	حرف *ڈالو_remote_name;
};

ساکن ساخت string_list branch_list = STRING_LIST_INIT_DUP;

ساکن ثابت حرف *abbrev_ref(ثابت حرف *name, ثابت حرف *سابقہ)
{
	skip_سابقہ(name, سابقہ, &name);
	واپس name;
}
#تعریف_کرو abbrev_branch(name) abbrev_ref((name), "refs/heads/")

ساکن صحیح config_read_branches(ثابت حرف *کلید, ثابت حرف *قدر,
				ثابت ساخت config_context *ctx غیر_مستعمل,
				خالی *ڈیٹا غیر_مستعمل)
{
	ثابت حرف *orig_کلید = کلید;
	حرف *name;
	ساخت string_list_شے *شے;
	ساخت branch_info *info;
	شمار { REMOTE, ملاؤ_2, REBASE, ڈالو_REMOTE } type;
	سائز_t کلید_len;

	اگر (!starts_with(کلید, "branch."))
		واپس 0;

	کلید += لڑی_لمبائی("branch.");
	اگر (strip_suffix(کلید, ".remote", &کلید_len))
		type = REMOTE;
	ورنہ اگر (strip_suffix(کلید, ".merge", &کلید_len))
		type = ملاؤ_2;
	ورنہ اگر (strip_suffix(کلید, ".rebase", &کلید_len))
		type = REBASE;
	ورنہ اگر (strip_suffix(کلید, ".pushremote", &کلید_len))
		type = ڈالو_REMOTE;
	ورنہ
		واپس 0;

	name = xmemdupz(کلید, کلید_len);
	شے = string_list_داخل_کرو(&branch_list, name);

	اگر (!شے->util)
		شے->util = xcalloc(1, حجم(ساخت branch_info));
	info = شے->util;
	انتخاب (type) {
	حالت REMOTE:
		اگر (info->remote_name)
			warning(_("more than one %s"), orig_کلید);
		info->remote_name = xstrdup(قدر);
		توڑو;
	حالت ملاؤ_2: {
		ثابت حرف *space = strchr(قدر, ' ');
		قدر = abbrev_branch(قدر);
		جب (space) {
			حرف *ملاؤ;
			ملاؤ = xstrndup(قدر, space - قدر);
			string_list_append(&info->ملاؤ, ملاؤ);
			قدر = abbrev_branch(space + 1);
			space = strchr(قدر, ' ');
		}
		string_list_append(&info->ملاؤ, xstrdup(قدر));
		توڑو;
	}
	حالت REBASE:
		/*
		 * Consider invalid values as false and check the
		 * truth value with >= REBASE_TRUE.
		 */
		info->rebase = rebase_parse_قدر(قدر);
		اگر (info->rebase == REBASE_INVALID)
			warning(_("unhandled branch.%s.rebase=%s; assuming "
				  "'true'"), name, قدر);
		توڑو;
	حالت ڈالو_REMOTE:
		اگر (info->ڈالو_remote_name)
			warning(_("more than one %s"), orig_کلید);
		info->ڈالو_remote_name = xstrdup(قدر);
		توڑو;
	طےشدہ:
		BUG("unexpected type=%d", type);
	}

	آزاد(name);
	واپس 0;
}

ساکن خالی read_branches(خالی)
{
	اگر (branch_list.nr)
		واپس;
	repo_config(the_repository, config_read_branches, تہی);
}

ساخت ref_حالتیں {
	ساخت remote *remote;
	ساخت string_list new_refs, skipped, stale, tracked, heads, ڈالو;
	صحیح queried;
};

#تعریف_کرو REF_حالتیں_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.ڈالو = STRING_LIST_INIT_DUP, \
}

ساکن صحیح get_ref_حالتیں(ثابت ساخت ref *remote_refs, ساخت ref_حالتیں *حالتیں)
{
	ساخت ref *fetch_map = تہی, **دم = &fetch_map;
	ساخت ref *ref, *stale_refs;
	صحیح i;

	برائے (i = 0; i < حالتیں->remote->fetch.nr; i++)
		اگر (get_fetch_map(remote_refs, &حالتیں->remote->fetch.اشیاء[i], &دم, 1))
			die(_("Could not get fetch map for refspec %s"),
				حالتیں->remote->fetch.اشیاء[i].raw);

	برائے (ref = fetch_map; ref; ref = ref->اگلا) {
		اگر (refname_matches_negative_refspec_شے(ref->name, &حالتیں->remote->fetch))
			string_list_append(&حالتیں->skipped, abbrev_branch(ref->name));
		ورنہ اگر (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&حالتیں->new_refs, abbrev_branch(ref->name));
		ورنہ
			string_list_append(&حالتیں->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&حالتیں->remote->fetch, fetch_map);
	برائے (ref = stale_refs; ref; ref = ref->اگلا) {
		ساخت string_list_شے *شے =
			string_list_append(&حالتیں->stale, abbrev_branch(ref->name));
		شے->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_ترتیب_دو(&حالتیں->new_refs);
	string_list_ترتیب_دو(&حالتیں->skipped);
	string_list_ترتیب_دو(&حالتیں->tracked);
	string_list_ترتیب_دو(&حالتیں->stale);

	واپس 0;
}

ساخت ڈالو_info {
	حرف *dest;
	صحیح forced;
	شمار {
		ڈالو_حالت_CREATE = 0,
		ڈالو_حالت_DELETE,
		ڈالو_حالت_UPTODATE,
		ڈالو_حالت_FASTFORWARD,
		ڈالو_حالت_OUTOFDATE,
		ڈالو_حالت_NOTQUERIED
	} حالت_2;
};

ساکن صحیح get_ڈالو_ref_حالتیں(ثابت ساخت ref *remote_refs,
	ساخت ref_حالتیں *حالتیں)
{
	ساخت remote *remote = حالتیں->remote;
	ساخت ref *ref, *local_refs, *ڈالو_map;
	اگر (remote->mirror)
		واپس 0;

	local_refs = get_local_heads();
	ڈالو_map = copy_ref_list(remote_refs);

	مطابقت_ڈالو_refs(local_refs, &ڈالو_map, &remote->ڈالو, مطابقت_REFS_NONE);

	برائے (ref = ڈالو_map; ref; ref = ref->اگلا) {
		ساخت string_list_شے *شے;
		ساخت ڈالو_info *info;

		اگر (!ref->peer_ref)
			جاری;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		شے = string_list_append(&حالتیں->ڈالو,
					  abbrev_branch(ref->peer_ref->name));
		شے->util = xcalloc(1, حجم(ساخت ڈالو_info));
		info = شے->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		اگر (is_null_oid(&ref->new_oid)) {
			info->حالت_2 = ڈالو_حالت_DELETE;
		} ورنہ اگر (oideq(&ref->old_oid, &ref->new_oid))
			info->حالت_2 = ڈالو_حالت_UPTODATE;
		ورنہ اگر (is_null_oid(&ref->old_oid))
			info->حالت_2 = ڈالو_حالت_CREATE;
		ورنہ اگر (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->حالت_2 = ڈالو_حالت_FASTFORWARD;
		ورنہ
			info->حالت_2 = ڈالو_حالت_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(ڈالو_map);
	واپس 0;
}

ساکن صحیح get_ڈالو_ref_حالتیں_noquery(ساخت ref_حالتیں *حالتیں)
{
	صحیح i;
	ساخت remote *remote = حالتیں->remote;
	ساخت string_list_شے *شے;
	ساخت ڈالو_info *info;

	اگر (remote->mirror)
		واپس 0;

	اگر (!remote->ڈالو.nr) {
		شے = string_list_append(&حالتیں->ڈالو, _("(matching)"));
		info = شے->util = xcalloc(1, حجم(ساخت ڈالو_info));
		info->حالت_2 = ڈالو_حالت_NOTQUERIED;
		info->dest = xstrdup(شے->string);
	}
	برائے (i = 0; i < remote->ڈالو.nr; i++) {
		ثابت ساخت refspec_شے *spec = &remote->ڈالو.اشیاء[i];
		اگر (spec->matching)
			شے = string_list_append(&حالتیں->ڈالو, _("(matching)"));
		ورنہ اگر (لڑی_لمبائی(spec->src))
			شے = string_list_append(&حالتیں->ڈالو, spec->src);
		ورنہ
			شے = string_list_append(&حالتیں->ڈالو, _("(delete)"));

		info = شے->util = xcalloc(1, حجم(ساخت ڈالو_info));
		info->forced = spec->force;
		info->حالت_2 = ڈالو_حالت_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : شے->string);
	}
	واپس 0;
}

ساکن صحیح get_سر_نام(ثابت ساخت ref *remote_refs, ساخت ref_حالتیں *حالتیں)
{
	ساخت ref *ref, *matches;
	ساخت ref *fetch_map = تہی, **fetch_map_دم = &fetch_map;
	ساخت refspec_شے refspec = {
		.force = 0,
		.نمونہ = 1,
		.src = (حرف *) "refs/heads/*",
		.dst = (حرف *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_دم, 0);
	matches = guess_remote_سر(ڈھونڈو_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_سر_ALL);
	برائے (ref = matches; ref; ref = ref->اگلا)
		string_list_append(&حالتیں->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	واپس 0;
}

ساخت معلوم_remote {
	ساخت معلوم_remote *اگلا;
	ساخت remote *remote;
};

ساخت معلوم_remotes {
	ساخت remote *مقصد_delete;
	ساخت معلوم_remote *list;
};

ساکن صحیح جوڑو_معلوم_remote(ساخت remote *remote, خالی *cb_ڈیٹا)
{
	ساخت معلوم_remotes *all = cb_ڈیٹا;
	ساخت معلوم_remote *r;

	اگر (!strcmp(all->مقصد_delete->name, remote->name))
		واپس 0;

	r = xmalloc(حجم(*r));
	r->remote = remote;
	r->اگلا = all->list;
	all->list = r;
	واپس 0;
}

ساخت branches_for_remote {
	ساخت remote *remote;
	ساخت string_list *branches, *skipped;
	ساخت معلوم_remotes *keep;
};

ساکن صحیح جوڑو_branch_for_removal(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	ساخت branches_for_remote *branches = cb_ڈیٹا;
	ساخت refspec_شے refspec;
	ساخت معلوم_remote *kr;

	memset(&refspec, 0, حجم(refspec));
	refspec.dst = (حرف *)ref->name;
	اگر (remote_ڈھونڈو_tracking(branches->remote, &refspec))
		واپس 0;
	آزاد(refspec.src);

	/* don't delete a branch if another remote also uses it */
	برائے (kr = branches->keep->list; kr; kr = kr->اگلا) {
		memset(&refspec, 0, حجم(refspec));
		refspec.dst = (حرف *)ref->name;
		اگر (!remote_ڈھونڈو_tracking(kr->remote, &refspec)) {
			آزاد(refspec.src);
			واپس 0;
		}
	}

	/* don't delete non-remote-tracking refs */
	اگر (!starts_with(ref->name, "refs/remotes/")) {
		/* advise user how to delete local branches */
		اگر (starts_with(ref->name, "refs/heads/"))
			string_list_append(branches->skipped,
					   abbrev_branch(ref->name));
		/* silently skip over other non-remote refs */
		واپس 0;
	}

	string_list_append(branches->branches, ref->name);

	واپس 0;
}

ساخت rename_info {
	ثابت حرف *old_name;
	ثابت حرف *new_name;
	ساخت ref_transaction *transaction;
	ساخت پیش_رفت *پیش_رفت;
	ساخت strbuf *err;
	uint32_t پیش_رفت_nr;
	uint64_t اشاریہ;
};

ساکن خالی compute_renamed_ref(ساخت rename_info *rename,
				ثابت حرف *refname,
				ساخت strbuf *برآمد)
{
	strbuf_reset(برآمد);
	strbuf_addstr(برآمد, refname);
	strbuf_splice(برآمد, لڑی_لمبائی("refs/remotes/"), لڑی_لمبائی(rename->old_name),
		      rename->new_name, لڑی_لمبائی(rename->new_name));
}

ساکن صحیح rename_one_reflog_اندراج(ثابت حرف *old_refname,
				   ساخت object_id *old_oid,
				   ساخت object_id *new_oid,
				   ثابت حرف *committer,
				   timestamp_t timestamp, صحیح tz,
				   ثابت حرف *پیغام, خالی *cb_ڈیٹا)
{
	ساخت rename_info *rename = cb_ڈیٹا;
	ساخت strbuf new_refname = STRBUF_INIT;
	ساخت strbuf شناختی_عنصر = STRBUF_INIT;
	ساخت strbuf name = STRBUF_INIT;
	ساخت strbuf mail = STRBUF_INIT;
	ساخت ident_بانٹو ident;
	ثابت حرف *date;
	صحیح error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	اگر (بانٹو_ident_سطر(&ident, committer, لڑی_لمبائی(committer)) < 0) {
		error = -1;
		جاؤ برآمد;
	}

	strbuf_جوڑو(&name, ident.name_begin, ident.name_انتہا - ident.name_begin);
	strbuf_جوڑو(&mail, ident.mail_begin, ident.mail_انتہا - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_منوال(معمول));
	strbuf_addstr(&شناختی_عنصر, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, شناختی_عنصر.buf, پیغام,
					      rename->اشاریہ++, rename->err);

برآمد:
	strbuf_release(&new_refname);
	strbuf_release(&شناختی_عنصر);
	strbuf_release(&name);
	strbuf_release(&mail);
	واپس error;
}

ساکن صحیح rename_one_reflog(ثابت حرف *old_refname,
			     ثابت ساخت object_id *old_oid,
			     ساخت rename_info *rename)
{
	ساخت strbuf new_refname = STRBUF_INIT;
	ساخت strbuf پیغام_2 = STRBUF_INIT;
	صحیح error;

	اگر (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		واپس 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_اندراج, rename);
	اگر (error < 0)
		جاؤ برآمد;

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
					      پیغام_2.buf, rename->اشاریہ++, rename->err);
	اگر (error < 0)
		واپس error;

برآمد:
	strbuf_release(&new_refname);
	strbuf_release(&پیغام_2);
	واپس error;
}

ساکن صحیح rename_one_ref(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	ساخت strbuf new_referent = STRBUF_INIT;
	ساخت strbuf new_refname = STRBUF_INIT;
	ساخت rename_info *rename = cb_ڈیٹا;
	ثابت ساخت object_id *oid = ref->oid;
	ثابت حرف *referent = ref->ہدف;
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
						   تہی, تہی);
		compute_renamed_ref(rename, referent, &new_referent);
		oid = تہی;
	}

	error = ref_transaction_delete(rename->transaction, ref->name,
				       oid, referent, REF_NO_DEREF, تہی, rename->err);
	اگر (error < 0)
		جاؤ برآمد;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : تہی, تہی,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       تہی, rename->err);
	اگر (error < 0)
		جاؤ برآمد;

	error = rename_one_reflog(ref->name, oid, rename);
	اگر (error < 0)
		جاؤ برآمد;

	display_پیش_رفت(rename->پیش_رفت, ++rename->پیش_رفت_nr);

برآمد:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	واپس error;
}

ساکن صحیح migrate_file(ساخت remote *remote)
{
	ساخت strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	برائے (سائز_t i = 0; i < remote->url.nr; i++)
		repo_config_سیٹ_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	برائے (صحیح i = 0; i < remote->ڈالو.nr; i++)
		repo_config_سیٹ_multivar(the_repository, buf.buf, remote->ڈالو.اشیاء[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	برائے (صحیح i = 0; i < remote->fetch.nr; i++)
		repo_config_سیٹ_multivar(the_repository, buf.buf, remote->fetch.اشیاء[i].raw, "^$", 0);
#اگر_غیر_تعریف_شدہ WITH_BREAKING_CHANGES
	اگر (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_راستہ_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	ورنہ اگر (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_راستہ_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#اگر_ختم /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	واپس 0;
}

ساخت ڈالو_default_info
{
	ثابت حرف *old_name;
	شمار config_scope scope;
	ساخت strbuf origin;
	صحیح linenr;
};

ساکن صحیح config_read_ڈالو_default(ثابت حرف *کلید, ثابت حرف *قدر,
	ثابت ساخت config_context *ctx, خالی *cb)
{
	ثابت ساخت کلید_قدر_info *kvi = ctx->kvi;

	ساخت ڈالو_default_info* info = cb;
	اگر (strcmp(کلید, "remote.pushdefault") ||
	    !قدر || strcmp(قدر, info->old_name))
		واپس 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	واپس 0;
}

ساکن خالی handle_ڈالو_default(ثابت حرف* old_name, ثابت حرف* new_name)
{
	ساخت ڈالو_default_info ڈالو_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_ڈالو_default, &ڈالو_default);
	اگر (ڈالو_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	ورنہ اگر (ڈالو_default.scope >= CONFIG_SCOPE_LOCAL) {
		صحیح نتیجہ = repo_config_سیٹ_gently(the_repository, "remote.pushDefault",
						    new_name);
		اگر (new_name && نتیجہ && نتیجہ != CONFIG_NOTHING_سیٹ)
			die(_("could not set '%s'"), "remote.pushDefault");
		ورنہ اگر (!new_name && نتیجہ && نتیجہ != CONFIG_NOTHING_سیٹ)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} ورنہ اگر (ڈالو_default.scope >= CONFIG_SCOPE_SYSTEM) {
		/* warn */
		warning(_("The %s configuration remote.pushDefault in:\n"
			  "\t%s:%d\n"
			  "now names the non-existent remote '%s'"),
			config_scope_name(ڈالو_default.scope),
			ڈالو_default.origin.buf, ڈالو_default.linenr,
			old_name);
	}

	strbuf_release(&ڈالو_default.origin);
}

ساکن ثابت حرف conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

ساکن صحیح mv(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
	      ساخت repository *repo غیر_مستعمل)
{
	صحیح show_پیش_رفت = isatty(2);
	ساخت option options[] = {
		OPT_BOOL(0, "progress", &show_پیش_رفت, N_("force progress reporting")),
		OPT_انتہا()
	};
	ساخت remote *oldremote, *newremote;
	ساخت strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	ساخت rename_info rename = {
		.err = &err,
	};
	صحیح refspecs_need_update = 0;
	صحیح نتیجہ = 0;

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_remote_rename_usage, 0);

	اگر (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	اگر (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		باہر_نکلو(2);
	}

	اگر (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		واپس migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	اگر (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		باہر_نکلو(3);
	}

	اگر (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	اگر (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		نتیجہ = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		جاؤ برآمد;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	برائے (صحیح i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!لڑی_تلاش(oldremote->fetch.اشیاء[i].raw,
						old_remote_context.buf);

	اگر (refspecs_need_update) {
		ساخت refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		اگر (!rename.transaction)
			جاؤ برآمد;

		اگر (show_پیش_رفت)
			rename.پیش_رفت = آغاز_delayed_پیش_رفت(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.سابقہ = buf.buf;

		نتیجہ = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		اگر (نتیجہ < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		نتیجہ = ref_transaction_prepare(rename.transaction, &err);
		اگر (نتیجہ < 0) {
			error("renaming remote references failed: %s", err.buf);
			اگر (نتیجہ == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(تہی);
		}
	}

	اگر (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_سیٹ_multivar(the_repository, buf.buf, تہی, تہی, CONFIG_FLAGS_MULTI_REPLACE);
		برائے (صحیح i = 0; i < oldremote->fetch.nr; i++) {
			حرف *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.اشیاء[i].raw);
			ptr = لڑی_تلاش(buf2.buf, old_remote_context.buf);
			اگر (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + لڑی_لمبائی(":refs/remotes/"),
					      لڑی_لمبائی(rename.old_name), rename.new_name,
					      لڑی_لمبائی(rename.new_name));
			} ورنہ
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_سیٹ_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	برائے (سائز_t i = 0; i < branch_list.nr; i++) {
		ساخت string_list_شے *شے = branch_list.اشیاء + i;
		ساخت branch_info *info = شے->util;
		اگر (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", شے->string);
			repo_config_سیٹ(the_repository, buf.buf, rename.new_name);
		}
		اگر (info->ڈالو_remote_name && !strcmp(info->ڈالو_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", شے->string);
			repo_config_سیٹ(the_repository, buf.buf, rename.new_name);
		}
	}

	اگر (refspecs_need_update) {
		نتیجہ = ref_transaction_commit(rename.transaction, &err);
		اگر (نتیجہ < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_پیش_رفت(&rename.پیش_رفت);

		handle_ڈالو_default(rename.old_name, rename.new_name);
	}

برآمد:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	واپس نتیجہ;
}

ساکن صحیح rm(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
	      ساخت repository *repo غیر_مستعمل)
{
	ساخت option options[] = {
		OPT_انتہا()
	};
	ساخت remote *remote;
	ساخت strbuf buf = STRBUF_INIT;
	ساخت معلوم_remotes معلوم_remotes = { تہی, تہی };
	ساخت string_list branches = STRING_LIST_INIT_DUP;
	ساخت string_list skipped = STRING_LIST_INIT_DUP;
	ساخت branches_for_remote cb_ڈیٹا;
	صحیح نتیجہ;

	memset(&cb_ڈیٹا, 0, حجم(cb_ڈیٹا));
	cb_ڈیٹا.branches = &branches;
	cb_ڈیٹا.skipped = &skipped;
	cb_ڈیٹا.keep = &معلوم_remotes;

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_remote_rm_usage, 0);
	اگر (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	اگر (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		باہر_نکلو(2);
	}

	معلوم_remotes.مقصد_delete = remote;
	for_each_remote(جوڑو_معلوم_remote, &معلوم_remotes);

	read_branches();
	برائے (سائز_t i = 0; i < branch_list.nr; i++) {
		ساخت string_list_شے *شے = branch_list.اشیاء + i;
		ساخت branch_info *info = شے->util;
		اگر (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ثابت حرف *کلیدیں[] = { "remote", "merge", تہی }, **k;
			برائے (k = کلیدیں; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						شے->string, *k);
				نتیجہ = repo_config_سیٹ_gently(the_repository, buf.buf, تہی);
				اگر (نتیجہ && نتیجہ != CONFIG_NOTHING_سیٹ)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		اگر (info->ڈالو_remote_name && !strcmp(info->ڈالو_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", شے->string);
			نتیجہ = repo_config_سیٹ_gently(the_repository, buf.buf, تہی);
			اگر (نتیجہ && نتیجہ != CONFIG_NOTHING_سیٹ)
				die(_("could not unset '%s'"), buf.buf);
		}
	}

	/*
	 * We cannot just pass a function to for_each_ref() which deletes
	 * the branches one by one, since for_each_ref() relies on cached
	 * refs, which are invalidated when deleting a branch.
	 */
	cb_ڈیٹا.remote = remote;
	نتیجہ = refs_for_each_ref(get_main_ref_store(the_repository),
				   جوڑو_branch_for_removal, &cb_ڈیٹا);
	strbuf_release(&buf);

	اگر (!نتیجہ)
		نتیجہ = refs_delete_refs(get_main_ref_store(the_repository),
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
		برائے (سائز_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.اشیاء[i].string);
	}
	string_list_clear(&skipped, 0);

	اگر (!نتیجہ) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		اگر (repo_config_rename_section(the_repository, buf.buf, تہی) < 1) {
			نتیجہ = error(_("Could not remove config section '%s'"), buf.buf);
			جاؤ برآمد;
		}

		handle_ڈالو_default(remote->name, تہی);
	}

برآمد:
	برائے (ساخت معلوم_remote *r = معلوم_remotes.list; r;) {
		ساخت معلوم_remote *اگلا = r->اگلا;
		آزاد(r);
		r = اگلا;
	}
	strbuf_release(&buf);
	واپس نتیجہ;
}

ساکن خالی clear_ڈالو_info(خالی *util, ثابت حرف *string غیر_مستعمل)
{
	ساخت ڈالو_info *info = util;
	آزاد(info->dest);
	آزاد(info);
}

ساکن خالی free_remote_ref_حالتیں(ساخت ref_حالتیں *حالتیں)
{
	string_list_clear(&حالتیں->new_refs, 0);
	string_list_clear(&حالتیں->skipped, 0);
	string_list_clear(&حالتیں->stale, 1);
	string_list_clear(&حالتیں->tracked, 0);
	string_list_clear(&حالتیں->heads, 0);
	string_list_clear_func(&حالتیں->ڈالو, clear_ڈالو_info);
}

ساکن صحیح append_ref_مقصد_tracked_list(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	ساخت ref_حالتیں *حالتیں = cb_ڈیٹا;
	ساخت refspec_شے refspec;

	اگر (ref->flags & REF_ISSYMREF)
		واپس 0;

	memset(&refspec, 0, حجم(refspec));
	refspec.dst = (حرف *)ref->name;
	اگر (!remote_ڈھونڈو_tracking(حالتیں->remote, &refspec)) {
		string_list_append(&حالتیں->tracked, abbrev_branch(refspec.src));
		آزاد(refspec.src);
	}

	واپس 0;
}

ساکن صحیح get_remote_ref_حالتیں(ثابت حرف *name,
				 ساخت ref_حالتیں *حالتیں,
				 صحیح query)
{
	حالتیں->remote = remote_get(name);
	اگر (!حالتیں->remote)
		واپس error(_("No such remote: '%s'"), name);

	read_branches();

	اگر (query) {
		ساخت transport *transport;
		ثابت ساخت ref *remote_refs;

		transport = transport_get(حالتیں->remote, حالتیں->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, تہی);

		حالتیں->queried = 1;
		اگر (query & GET_REF_حالتیں)
			get_ref_حالتیں(remote_refs, حالتیں);
		اگر (query & GET_سر_نام)
			get_سر_نام(remote_refs, حالتیں);
		اگر (query & GET_ڈالو_REF_حالتیں)
			get_ڈالو_ref_حالتیں(remote_refs, حالتیں);
		transport_disconnect(transport);
	} ورنہ {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_مقصد_tracked_list, حالتیں);
		string_list_ترتیب_دو(&حالتیں->tracked);
		get_ڈالو_ref_حالتیں_noquery(حالتیں);
	}

	واپس 0;
}

ساخت show_info {
	ساخت string_list list;
	ساخت ref_حالتیں حالتیں;
	صحیح چوڑائی, width2;
	صحیح any_rebase;
};

#تعریف_کرو SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.حالتیں = REF_حالتیں_INIT, \
}

ساکن صحیح جوڑو_remote_مقصد_show_info(ساخت string_list_شے *شے, خالی *cb_ڈیٹا)
{
	ساخت show_info *info = cb_ڈیٹا;
	صحیح n = لڑی_لمبائی(شے->string);
	اگر (n > info->چوڑائی)
		info->چوڑائی = n;
	string_list_داخل_کرو(&info->list, شے->string);
	واپس 0;
}

ساکن صحیح show_remote_info_شے(ساخت string_list_شے *شے, خالی *cb_ڈیٹا)
{
	ساخت show_info *info = cb_ڈیٹا;
	ساخت ref_حالتیں *حالتیں = &info->حالتیں;
	ثابت حرف *name = شے->string;

	اگر (حالتیں->queried) {
		ثابت حرف *fmt = "%s";
		ثابت حرف *arg = "";
		اگر (string_list_has_string(&حالتیں->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = حالتیں->remote->name;
		} ورنہ اگر (string_list_has_string(&حالتیں->tracked, name))
			arg = _(" tracked");
		ورنہ اگر (string_list_has_string(&حالتیں->skipped, name))
			arg = _(" skipped");
		ورنہ اگر (string_list_has_string(&حالتیں->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		ورنہ
			arg = _(" ???");
		لکھو("    %-*s", info->چوڑائی, name);
		لکھو(fmt, arg);
		لکھو("\n");
	} ورنہ
		لکھو("    %s\n", name);

	واپس 0;
}

ساکن صحیح جوڑو_local_مقصد_show_info(ساخت string_list_شے *branch_شے, خالی *cb_ڈیٹا)
{
	ساخت show_info *show_info = cb_ڈیٹا;
	ساخت ref_حالتیں *حالتیں = &show_info->حالتیں;
	ساخت branch_info *branch_info = branch_شے->util;
	ساخت string_list_شے *شے;
	صحیح n;

	اگر (!branch_info->ملاؤ.nr || !branch_info->remote_name ||
	    strcmp(حالتیں->remote->name, branch_info->remote_name))
		واپس 0;
	اگر ((n = لڑی_لمبائی(branch_شے->string)) > show_info->چوڑائی)
		show_info->چوڑائی = n;
	اگر (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	شے = string_list_داخل_کرو(&show_info->list, branch_شے->string);
	شے->util = branch_info;

	واپس 0;
}

ساکن صحیح show_local_info_شے(ساخت string_list_شے *شے, خالی *cb_ڈیٹا)
{
	ساخت show_info *show_info = cb_ڈیٹا;
	ساخت branch_info *branch_info = شے->util;
	ساخت string_list *ملاؤ = &branch_info->ملاؤ;
	صحیح چوڑائی = show_info->چوڑائی + 4;

	اگر (branch_info->rebase >= REBASE_TRUE && branch_info->ملاؤ.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			شے->string);
		واپس 0;
	}

	لکھو("    %-*s ", show_info->چوڑائی, شے->string);
	اگر (branch_info->rebase >= REBASE_TRUE) {
		ثابت حرف *پیغام;
		اگر (branch_info->rebase == REBASE_INTERACTIVE)
			پیغام = _("rebases interactively onto remote %s");
		ورنہ اگر (branch_info->rebase == REBASE_MERGES)
			پیغام = _("rebases interactively (with merges) onto "
				"remote %s");
		ورنہ
			پیغام = _("rebases onto remote %s");
		printf_ln(پیغام, ملاؤ->اشیاء[0].string);
		واپس 0;
	} ورنہ اگر (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), ملاؤ->اشیاء[0].string);
		چوڑائی++;
	} ورنہ {
		printf_ln(_("merges with remote %s"), ملاؤ->اشیاء[0].string);
	}
	برائے (سائز_t i = 1; i < ملاؤ->nr; i++)
		لکھو(_("%-*s    and with remote %s\n"), چوڑائی, "",
		       ملاؤ->اشیاء[i].string);

	واپس 0;
}

ساکن صحیح جوڑو_ڈالو_مقصد_show_info(ساخت string_list_شے *ڈالو_شے, خالی *cb_ڈیٹا)
{
	ساخت show_info *show_info = cb_ڈیٹا;
	ساخت ڈالو_info *ڈالو_info = ڈالو_شے->util;
	ساخت string_list_شے *شے;
	صحیح n;
	اگر ((n = لڑی_لمبائی(ڈالو_شے->string)) > show_info->چوڑائی)
		show_info->چوڑائی = n;
	اگر ((n = لڑی_لمبائی(ڈالو_info->dest)) > show_info->width2)
		show_info->width2 = n;
	شے = string_list_append(&show_info->list, ڈالو_شے->string);
	شے->util = ڈالو_شے->util;
	واپس 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
ساکن صحیح cmp_string_with_ڈالو(ثابت خالی *va, ثابت خالی *vb)
{
	ثابت ساخت string_list_شے *a = va;
	ثابت ساخت string_list_شے *b = vb;
	ثابت ساخت ڈالو_info *a_ڈالو = a->util;
	ثابت ساخت ڈالو_info *b_ڈالو = b->util;
	صحیح cmp = strcmp(a->string, b->string);
	واپس cmp ? cmp : strcmp(a_ڈالو->dest, b_ڈالو->dest);
}

ساکن صحیح show_ڈالو_info_شے(ساخت string_list_شے *شے, خالی *cb_ڈیٹا)
{
	ساخت show_info *show_info = cb_ڈیٹا;
	ساخت ڈالو_info *ڈالو_info = شے->util;
	ثابت حرف *src = شے->string, *حالت_2 = تہی;

	انتخاب (ڈالو_info->حالت_2) {
	حالت ڈالو_حالت_CREATE:
		حالت_2 = _("create");
		توڑو;
	حالت ڈالو_حالت_DELETE:
		حالت_2 = _("delete");
		src = _("(none)");
		توڑو;
	حالت ڈالو_حالت_UPTODATE:
		حالت_2 = _("up to date");
		توڑو;
	حالت ڈالو_حالت_FASTFORWARD:
		حالت_2 = _("fast-forwardable");
		توڑو;
	حالت ڈالو_حالت_OUTOFDATE:
		حالت_2 = _("local out of date");
		توڑو;
	حالت ڈالو_حالت_NOTQUERIED:
		توڑو;
	}
	اگر (حالت_2) {
		اگر (ڈالو_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->چوڑائی, src,
			       show_info->width2, ڈالو_info->dest, حالت_2);
		ورنہ
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->چوڑائی, src,
			       show_info->width2, ڈالو_info->dest, حالت_2);
	} ورنہ {
		اگر (ڈالو_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->چوڑائی, src,
			       ڈالو_info->dest);
		ورنہ
			printf_ln(_("    %-*s pushes to %s"), show_info->چوڑائی, src,
			       ڈالو_info->dest);
	}
	واپس 0;
}

ساکن صحیح get_one_اندراج(ساخت remote *remote, خالی *priv)
{
	ساخت string_list *list = priv;
	ساخت strbuf remote_info_buf = STRBUF_INIT;
	ساخت strvec *url;

	اگر (remote->url.nr > 0) {
		ساخت strbuf promisor_config = STRBUF_INIT;
		ثابت حرف *partial_clone_filter = تہی;

		strbuf_addf(&promisor_config, "remote.%s.partialclonefilter", remote->name);
		strbuf_addf(&remote_info_buf, "%s (fetch)", remote->url.v[0]);
		اگر (!repo_config_get_string_tmp(the_repository, promisor_config.buf, &partial_clone_filter))
			strbuf_addf(&remote_info_buf, " [%s]", partial_clone_filter);

		strbuf_release(&promisor_config);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, تہی);
	} ورنہ
		string_list_append(list, remote->name)->util = تہی;
	url = ڈالو_url_کا_remote(remote);
	برائے (سائز_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, تہی);
	}

	واپس 0;
}

ساکن صحیح show_all(خالی)
{
	ساخت string_list list = STRING_LIST_INIT_DUP;
	صحیح نتیجہ;

	نتیجہ = for_each_remote(get_one_اندراج, &list);

	اگر (!نتیجہ) {
		string_list_ترتیب_دو(&list);
		برائے (سائز_t i = 0; i < list.nr; i++) {
			ساخت string_list_شے *شے = list.اشیاء + i;
			اگر (verbose)
				لکھو("%s\t%s\n", شے->string,
					شے->util ? (ثابت حرف *)شے->util : "");
			ورنہ {
				اگر (i && !strcmp((شے - 1)->string, شے->string))
					جاری;
				لکھو("%s\n", شے->string);
			}
		}
	}
	string_list_clear(&list, 1);
	واپس نتیجہ;
}

ساکن صحیح show(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		ساخت repository *repo غیر_مستعمل)
{
	صحیح no_query = 0, نتیجہ = 0, query_flag = 0;
	ساخت option options[] = {
		OPT_BOOL('n', تہی, &no_query, N_("do not query remotes")),
		OPT_انتہا()
	};
	ساخت show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_remote_show_usage,
			     0);

	اگر (argc < 1)
		واپس show_all();

	اگر (!no_query)
		query_flag = (GET_REF_حالتیں | GET_سر_نام | GET_ڈالو_REF_حالتیں);

	برائے (; argc; argc--, argv++) {
		سائز_t i;
		ساخت strvec *url;

		get_remote_ref_حالتیں(*argv, &info.حالتیں, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.حالتیں.remote->url.v[0]);
		url = ڈالو_url_کا_remote(info.حالتیں.remote);
		برائے (i = 0; i < url->nr; i++)
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
		ورنہ اگر (!info.حالتیں.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		ورنہ اگر (info.حالتیں.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.حالتیں.heads.اشیاء[0].string);
		ورنہ {
			لکھو(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			برائے (i = 0; i < info.حالتیں.heads.nr; i++)
				لکھو("    %s\n", info.حالتیں.heads.اشیاء[i].string);
		}

		/* remote branch info */
		info.چوڑائی = 0;
		for_each_string_list(&info.حالتیں.new_refs, جوڑو_remote_مقصد_show_info, &info);
		for_each_string_list(&info.حالتیں.skipped, جوڑو_remote_مقصد_show_info, &info);
		for_each_string_list(&info.حالتیں.tracked, جوڑو_remote_مقصد_show_info, &info);
		for_each_string_list(&info.حالتیں.stale, جوڑو_remote_مقصد_show_info, &info);
		اگر (info.list.nr)
			printf_ln(Q_("  Remote branch:%s",
				     "  Remote branches:%s",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_remote_info_شے, &info);
		string_list_clear(&info.list, 0);

		/* git pull info */
		info.چوڑائی = 0;
		info.any_rebase = 0;
		for_each_string_list(&branch_list, جوڑو_local_مقصد_show_info, &info);
		اگر (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_شے, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		اگر (info.حالتیں.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.چوڑائی = info.width2 = 0;
		for_each_string_list(&info.حالتیں.ڈالو, جوڑو_ڈالو_مقصد_show_info, &info);
		QSORT(info.list.اشیاء, info.list.nr, cmp_string_with_ڈالو);
		اگر (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_ڈالو_info_شے, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_حالتیں(&info.حالتیں);
	}

	واپس نتیجہ;
}

ساکن خالی report_سیٹ_سر_auto(ثابت حرف *remote, ثابت حرف *سر_name,
			ساخت strbuf *b_local_سر, صحیح was_detached) {
	ساخت strbuf buf_سابقہ = STRBUF_INIT;
	ثابت حرف *پچھلا_سر = تہی;

	strbuf_addf(&buf_سابقہ, "refs/remotes/%s/", remote);
	skip_سابقہ(b_local_سر->buf, buf_سابقہ.buf, &پچھلا_سر);

	اگر (پچھلا_سر && !strcmp(پچھلا_سر, سر_name))
		لکھو(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, سر_name);
	ورنہ اگر (پچھلا_سر)
		لکھو(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, پچھلا_سر, سر_name);
	ورنہ اگر (!b_local_سر->len)
		لکھو(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, سر_name);
	ورنہ اگر (was_detached && b_local_سر->len)
		لکھو(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_سر->buf, سر_name);
	ورنہ
		لکھو(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_سر->buf, سر_name);
	strbuf_release(&buf_سابقہ);
}

ساکن صحیح سیٹ_سر(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		    ساخت repository *repo غیر_مستعمل)
{
	صحیح opt_a = 0, opt_d = 0, نتیجہ = 0, was_detached;
	ساخت strbuf b_سر = STRBUF_INIT, b_remote_سر = STRBUF_INIT,
		b_local_سر = STRBUF_INIT;
	حرف *سر_name = تہی;
	ساخت ref_store *refs = get_main_ref_store(the_repository);
	ساخت remote *remote;

	ساخت option options[] = {
		OPT_BOOL('a', "auto", &opt_a,
			 N_("set refs/remotes/<name>/HEAD according to remote")),
		OPT_BOOL('d', "delete", &opt_d,
			 N_("delete refs/remotes/<name>/HEAD")),
		OPT_انتہا()
	};
	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	اگر (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_سر, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	اگر (!opt_a && !opt_d && argc == 2) {
		سر_name = xstrdup(argv[1]);
	} ورنہ اگر (opt_a && !opt_d && argc == 1) {
		ساخت ref_حالتیں حالتیں = REF_حالتیں_INIT;
		get_remote_ref_حالتیں(argv[0], &حالتیں, GET_سر_نام);
		اگر (!حالتیں.heads.nr)
			نتیجہ |= error(_("Cannot determine remote HEAD"));
		ورنہ اگر (حالتیں.heads.nr > 1) {
			نتیجہ |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			برائے (سائز_t i = 0; i < حالتیں.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], حالتیں.heads.اشیاء[i].string);
		} ورنہ
			سر_name = xstrdup(حالتیں.heads.اشیاء[0].string);
		free_remote_ref_حالتیں(&حالتیں);
	} ورنہ اگر (opt_d && !opt_a && argc == 1) {
		اگر (refs_delete_ref(refs, تہی, b_سر.buf, تہی, REF_NO_DEREF))
			نتیجہ |= error(_("Could not delete %s"), b_سر.buf);
	} ورنہ
		usage_with_options(builtin_remote_sethead_usage, options);

	اگر (!سر_name)
		جاؤ cleanup;
	strbuf_addf(&b_remote_سر, "refs/remotes/%s/%s", argv[0], سر_name);
	اگر (!refs_ref_exists(refs, b_remote_سر.buf)) {
		نتیجہ |= error(_("Not a valid ref: %s"), b_remote_سر.buf);
		جاؤ cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_سر.buf, b_remote_سر.buf,
			"remote set-head", &b_local_سر, 0);
	اگر (was_detached == -1) {
		نتیجہ |= error(_("Could not set up %s"), b_سر.buf);
		جاؤ cleanup;
	}
	اگر (opt_a)
		report_سیٹ_سر_auto(argv[0], سر_name, &b_local_سر, was_detached);
	اگر (remote->follow_remote_سر == FOLLOW_REMOTE_ALWAYS) {
		ساخت strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_سیٹ(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	آزاد(سر_name);
	strbuf_release(&b_سر);
	strbuf_release(&b_remote_سر);
	strbuf_release(&b_local_سر);
	واپس نتیجہ;
}

ساکن صحیح prune_remote(ثابت حرف *remote, صحیح dry_تسلسل)
{
	صحیح نتیجہ = 0;
	ساخت ref_حالتیں حالتیں = REF_حالتیں_INIT;
	ساخت string_list refs_مقصد_prune = STRING_LIST_INIT_NODUP;
	ساخت string_list_شے *شے;

	get_remote_ref_حالتیں(remote, &حالتیں, GET_REF_حالتیں);

	اگر (!حالتیں.stale.nr) {
		free_remote_ref_حالتیں(&حالتیں);
		واپس 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), حالتیں.remote->url.v[0]);

	for_each_string_list_شے(شے, &حالتیں.stale)
		string_list_append(&refs_مقصد_prune, شے->util);
	string_list_ترتیب_دو(&refs_مقصد_prune);

	اگر (!dry_تسلسل)
		نتیجہ |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_مقصد_prune, 0);

	for_each_string_list_شے(شے, &حالتیں.stale) {
		ثابت حرف *refname = شے->util;

		اگر (dry_تسلسل)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		ورنہ
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_تسلسل, &refs_مقصد_prune);

	string_list_clear(&refs_مقصد_prune, 0);
	free_remote_ref_حالتیں(&حالتیں);
	واپس نتیجہ;
}

ساکن صحیح prune(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		 ساخت repository *repo غیر_مستعمل)
{
	صحیح dry_تسلسل = 0, نتیجہ = 0;
	ساخت option options[] = {
		OPT_DRY_تسلسل(&dry_تسلسل, N_("dry run")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_remote_prune_usage, 0);

	اگر (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	برائے (; argc; argc--, argv++)
		نتیجہ |= prune_remote(*argv, dry_تسلسل);

	واپس نتیجہ;
}

ساکن صحیح get_remote_default(ثابت حرف *کلید, ثابت حرف *قدر غیر_مستعمل,
			      ثابت ساخت config_context *ctx غیر_مستعمل,
			      خالی *priv)
{
	اگر (strcmp(کلید, "remotes.default") == 0) {
		صحیح *ملا_2 = priv;
		*ملا_2 = 1;
	}
	واپس 0;
}

ساکن صحیح update(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		  ساخت repository *repo غیر_مستعمل)
{
	صحیح i, prune = -1;
	ساخت option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_انتہا()
	};
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;
	صحیح default_defined = 0;

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_ڈالو(&cmd.دلائل, "fetch");

	اگر (prune != -1)
		strvec_ڈالو(&cmd.دلائل, prune ? "--prune" : "--no-prune");
	اگر (verbose)
		strvec_ڈالو(&cmd.دلائل, "-v");
	strvec_ڈالو(&cmd.دلائل, "--multiple");
	اگر (argc < 2)
		strvec_ڈالو(&cmd.دلائل, "default");
	برائے (i = 1; i < argc; i++)
		strvec_ڈالو(&cmd.دلائل, argv[i]);

	اگر (strcmp(cmd.دلائل.v[cmd.دلائل.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		اگر (!default_defined) {
			strvec_نکالو(&cmd.دلائل);
			strvec_ڈالو(&cmd.دلائل, "--all");
		}
	}

	cmd.git_cmd = 1;
	واپس تسلسل_command(&cmd);
}

ساکن صحیح remove_all_fetch_refspecs(ثابت حرف *کلید)
{
	واپس repo_config_سیٹ_multivar_gently(the_repository, کلید, تہی, تہی,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

ساکن خالی جوڑو_branches(ساخت remote *remote, ثابت حرف **branches,
			 ثابت حرف *کلید)
{
	ثابت حرف *remotename = remote->name;
	صحیح mirror = remote->mirror;
	ساخت strbuf refspec = STRBUF_INIT;

	برائے (; *branches; branches++)
		جوڑو_branch(کلید, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

ساکن صحیح سیٹ_remote_branches(ثابت حرف *remotename, ثابت حرف **branches,
				صحیح جوڑو_منوال)
{
	ساخت strbuf کلید = STRBUF_INIT;
	ساخت remote *remote;

	strbuf_addf(&کلید, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	اگر (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		باہر_نکلو(2);
	}

	اگر (!جوڑو_منوال && remove_all_fetch_refspecs(کلید.buf)) {
		strbuf_release(&کلید);
		واپس 1;
	}
	جوڑو_branches(remote, branches, کلید.buf);

	strbuf_release(&کلید);
	واپس 0;
}

ساکن صحیح سیٹ_branches(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			ساخت repository *repo غیر_مستعمل)
{
	صحیح جوڑو_منوال = 0;
	ساخت option options[] = {
		OPT_BOOL('\0', "add", &جوڑو_منوال, N_("add branch")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_remote_setbranches_usage, 0);
	اگر (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = تہی;

	واپس سیٹ_remote_branches(argv[0], argv + 1, جوڑو_منوال);
}

ساکن صحیح get_url(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		   ساخت repository *repo غیر_مستعمل)
{
	صحیح ڈالو_منوال = 0, all_منوال = 0;
	ثابت حرف *remotename = تہی;
	ساخت remote *remote;
	ساخت strvec *url;
	ساخت option options[] = {
		OPT_BOOL('\0', "push", &ڈالو_منوال,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_منوال,
			 N_("return all URLs")),
		OPT_انتہا()
	};
	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_remote_geturl_usage, 0);

	اگر (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	اگر (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		باہر_نکلو(2);
	}

	url = ڈالو_منوال ? ڈالو_url_کا_remote(remote) : &remote->url;

	اگر (all_منوال) {
		برائے (سائز_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} ورنہ {
		printf_ln("%s", url->v[0]);
	}

	واپس 0;
}

ساکن صحیح سیٹ_url(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		   ساخت repository *repo غیر_مستعمل)
{
	صحیح ڈالو_منوال = 0, جوڑو_منوال = 0, delete_منوال = 0;
	صحیح matches = 0, negative_matches = 0;
	ثابت حرف *remotename = تہی;
	ثابت حرف *newurl = تہی;
	ثابت حرف *oldurl = تہی;
	ساخت remote *remote;
	regex_t old_regex;
	ساخت strvec *urlset;
	ساخت strbuf name_buf = STRBUF_INIT;
	ساخت option options[] = {
		OPT_BOOL('\0', "push", &ڈالو_منوال,
			 N_("manipulate push URLs")),
		OPT_BOOL('\0', "add", &جوڑو_منوال,
			 N_("add URL")),
		OPT_BOOL('\0', "delete", &delete_منوال,
			    N_("delete URLs")),
		OPT_انتہا()
	};
	argc = parse_options(argc, argv, سابقہ, options,
			     builtin_remote_seturl_usage,
			     PARSE_OPT_KEEP_ARGV0);

	اگر (جوڑو_منوال && delete_منوال)
		die(_("--add --delete doesn't make sense"));

	اگر (argc < 3 || argc > 4 || ((جوڑو_منوال || delete_منوال) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	اگر (argc > 3)
		oldurl = argv[3];

	اگر (delete_منوال)
		oldurl = newurl;

	remote = remote_get(remotename);
	اگر (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		باہر_نکلو(2);
	}

	اگر (ڈالو_منوال) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} ورنہ {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	اگر ((!oldurl && !delete_منوال) || جوڑو_منوال) {
		اگر (جوڑو_منوال)
			repo_config_سیٹ_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		ورنہ
			repo_config_سیٹ(the_repository, name_buf.buf, newurl);
		جاؤ برآمد;
	}

	/* Old URL specified. Demand that one matches. */
	اگر (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	برائے (سائز_t i = 0; i < urlset->nr; i++)
		اگر (!regexec(&old_regex, urlset->v[i], 0, تہی, 0))
			matches++;
		ورنہ
			negative_matches++;
	اگر (!delete_منوال && !matches)
		die(_("No such URL found: %s"), oldurl);
	اگر (delete_منوال && !negative_matches && !ڈالو_منوال)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	اگر (!delete_منوال)
		repo_config_سیٹ_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	ورنہ
		repo_config_سیٹ_multivar(the_repository, name_buf.buf, تہی, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
برآمد:
	strbuf_release(&name_buf);
	واپس 0;
}

صحیح cmd_remote(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo)
{
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, جوڑو),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, تہی, PARSE_OPT_NOCOMPLETE),
		OPT_SUBCOMMAND("remove", &fn, rm),
		OPT_SUBCOMMAND("set-head", &fn, سیٹ_سر),
		OPT_SUBCOMMAND("set-branches", &fn, سیٹ_branches),
		OPT_SUBCOMMAND("get-url", &fn, get_url),
		OPT_SUBCOMMAND("set-url", &fn, سیٹ_url),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("update", &fn, update),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, builtin_remote_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	اگر (fn) {
		واپس !!fn(argc, argv, سابقہ, repo);
	} ورنہ {
		اگر (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		واپس !!show_all();
	}
}
