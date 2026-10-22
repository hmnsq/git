#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

ساكن ثابت حرف * ثابت builtin_remote_جوڑو_usage[] = {
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

#تعریف_کرو GET_REF_حالتاں (1<<0)
#تعریف_کرو GET_سر_ناں (1<<1)
#تعریف_کرو GET_ڈالو_REF_حالتاں (1<<2)

ساكن صحيح verbose;

ساكن صحيح fetch_remote(ثابت حرف *name)
{
	بنية بچہ_process cmd = بچہ_PROCESS_INIT;

	strvec_ڈالو(&cmd.دلیلاں, "fetch");
	إذا (verbose)
		strvec_ڈالو(&cmd.دلیلاں, "-v");
	strvec_ڈالو(&cmd.دلیلاں, name);
	cmd.git_cmd = 1;
	printf_ln(_("Updating %s"), name);
	إذا (لگاتار_command(&cmd))
		أرجع error(_("Could not fetch %s"), name);
	أرجع 0;
}

تعداد {
	ٹیگ_سوچی_UNSET = 0,
	ٹیگ_سوچی_DEFAULT = 1,
	ٹیگ_سوچی_سیٹ = 2
};

#تعریف_کرو MIRROR_NONE 0
#تعریف_کرو MIRROR_FETCH 1
#تعریف_کرو MIRROR_ڈالو 2
#تعریف_کرو MIRROR_BOTH (MIRROR_FETCH|MIRROR_ڈالو)

ساكن فراغ جوڑو_branch(ثابت حرف *کلید, ثابت حرف *branchname,
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
	repo_config_سیٹ_multivar(the_repository, کلید, tmp->buf, "^$", 0);
}

ساكن ثابت حرف mirror_advice[] =
N_("--mirror is dangerous and deprecated; please\n"
   "\t use --mirror=fetch or --mirror=push instead");

ساكن صحيح parse_mirror_opt(ثابت بنية option *opt, ثابت حرف *arg, صحيح not)
{
	بنانشانی *mirror = opt->قدر;
	إذا (not)
		*mirror = MIRROR_NONE;
	وإلا إذا (!arg) {
		warning("%s", _(mirror_advice));
		*mirror = MIRROR_BOTH;
	}
	وإلا إذا (!strcmp(arg, "fetch"))
		*mirror = MIRROR_FETCH;
	وإلا إذا (!strcmp(arg, "push"))
		*mirror = MIRROR_ڈالو;
	وإلا
		أرجع error(_("unknown --mirror argument: %s"), arg);
	أرجع 0;
}

ساكن صحيح check_remote_collision(بنية remote *remote, فراغ *انکڑے)
{
	ثابت حرف *name = انکڑے;
	ثابت حرف *p;

	إذا (skip_اگیتر(name, remote->name, &p) && *p == '/')
		die(_("remote name '%s' is a subset of existing remote '%s'"),
		    name, remote->name);
	إذا (skip_اگیتر(remote->name, name, &p) && *p == '/')
		die(_("remote name '%s' is a superset of existing remote '%s'"),
		    name, remote->name);

	أرجع 0;
}

ساكن صحيح جوڑو(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
	       بنية repository *repo بن_ورتیا)
{
	صحيح fetch = 0, fetch_ٹیگ_سوچی = ٹیگ_سوچی_DEFAULT;
	بنانشانی mirror = MIRROR_NONE;
	بنية string_list track = STRING_LIST_INIT_NODUP;
	ثابت حرف *master = عدم;
	بنية remote *remote;
	بنية strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT;
	ثابت حرف *name, *url;
	صحيح نتیجہ = 0;

	بنية option options[] = {
		OPT_BOOL('f', "fetch", &fetch, N_("fetch the remote branches")),
		OPT_سیٹ_INT(0, "tags", &fetch_ٹیگ_سوچی,
			    N_("import all tags and associated objects when fetching\n"
			       "or do not fetch any tag at all (--no-tags)"),
			    ٹیگ_سوچی_سیٹ),
		OPT_STRING_LIST('t', "track", &track, N_("branch"),
				N_("branch(es) to track")),
		OPT_STRING('m', "master", &master, N_("branch"), N_("master branch")),
		OPT_CALLBACK_F(0, "mirror", &mirror, "(push|fetch)",
			N_("set up remote as a mirror to push to or fetch from"),
			PARSE_OPT_OPTARG | PARSE_OPT_COMP_ARG, parse_mirror_opt),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options,
			     builtin_remote_جوڑو_usage, 0);

	إذا (argc != 2)
		usage_with_options(builtin_remote_جوڑو_usage, options);

	إذا (mirror && master)
		die(_("specifying a master branch makes no sense with --mirror"));
	إذا (mirror && !(mirror & MIRROR_FETCH) && track.nr)
		die(_("specifying branches to track makes sense only with fetch mirrors"));

	name = argv[0];
	url = argv[1];

	remote = remote_get(name);
	إذا (remote_is_configured(remote, 1)) {
		error(_("remote %s already exists."), name);
		باہر_نکلو(3);
	}

	إذا (!valid_remote_name(name))
		die(_("'%s' is not a valid remote name"), name);

	for_each_remote(check_remote_collision, (فراغ *)name);

	strbuf_addf(&buf, "remote.%s.url", name);
	repo_config_سیٹ(the_repository, buf.buf, url);

	إذا (!mirror || mirror & MIRROR_FETCH) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", name);
		إذا (track.nr == 0)
			string_list_append(&track, "*");
		لكل (ناپ_t i = 0; i < track.nr; i++) {
			جوڑو_branch(buf.buf, track.اشیاء[i].string,
				   name, mirror, &buf2);
		}
	}

	إذا (mirror & MIRROR_ڈالو) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.mirror", name);
		repo_config_سیٹ(the_repository, buf.buf, "true");
	}

	إذا (fetch_ٹیگ_سوچی != ٹیگ_سوچی_DEFAULT) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.tagOpt", name);
		repo_config_سیٹ(the_repository, buf.buf,
				fetch_ٹیگ_سوچی == ٹیگ_سوچی_سیٹ ? "--tags" : "--no-tags");
	}

	إذا (fetch && fetch_remote(name)) {
		نتیجہ = 1;
		اذهب باہرلا;
	}

	إذا (master) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/HEAD", name);

		strbuf_reset(&buf2);
		strbuf_addf(&buf2, "refs/remotes/%s/%s", name, master);

		إذا (refs_update_symref(get_main_ref_store(the_repository), buf.buf, buf2.buf, "remote add"))
			نتیجہ = error(_("Could not setup master '%s'"), master);
	}

باہرلا:
	strbuf_release(&buf);
	strbuf_release(&buf2);
	string_list_clear(&track, 0);

	أرجع نتیجہ;
}

بنية branch_info {
	حرف *remote_name;
	بنية string_list ملاؤ;
	تعداد rebase_type rebase;
	حرف *ڈالو_remote_name;
};

ساكن بنية string_list branch_list = STRING_LIST_INIT_DUP;

ساكن ثابت حرف *abbrev_ref(ثابت حرف *name, ثابت حرف *اگیتر)
{
	skip_اگیتر(name, اگیتر, &name);
	أرجع name;
}
#تعریف_کرو abbrev_branch(name) abbrev_ref((name), "refs/heads/")

ساكن صحيح config_read_branches(ثابت حرف *کلید, ثابت حرف *قدر,
				ثابت بنية config_context *ctx بن_ورتیا,
				فراغ *انکڑے بن_ورتیا)
{
	ثابت حرف *orig_کلید = کلید;
	حرف *name;
	بنية string_list_شے *شے;
	بنية branch_info *info;
	تعداد { REMOTE, ملاؤ_2, REBASE, ڈالو_REMOTE } type;
	ناپ_t کلید_len;

	إذا (!starts_with(کلید, "branch."))
		أرجع 0;

	کلید += لڑی_لمبائی("branch.");
	إذا (strip_suffix(کلید, ".remote", &کلید_len))
		type = REMOTE;
	وإلا إذا (strip_suffix(کلید, ".merge", &کلید_len))
		type = ملاؤ_2;
	وإلا إذا (strip_suffix(کلید, ".rebase", &کلید_len))
		type = REBASE;
	وإلا إذا (strip_suffix(کلید, ".pushremote", &کلید_len))
		type = ڈالو_REMOTE;
	وإلا
		أرجع 0;

	name = xmemdupz(کلید, کلید_len);
	شے = string_list_داخل_کرو(&branch_list, name);

	إذا (!شے->util)
		شے->util = xcalloc(1, حجم(بنية branch_info));
	info = شے->util;
	بدل (type) {
	حالة REMOTE:
		إذا (info->remote_name)
			warning(_("more than one %s"), orig_کلید);
		info->remote_name = xstrdup(قدر);
		اكسر;
	حالة ملاؤ_2: {
		ثابت حرف *space = strchr(قدر, ' ');
		قدر = abbrev_branch(قدر);
		طالما (space) {
			حرف *ملاؤ;
			ملاؤ = xstrndup(قدر, space - قدر);
			string_list_append(&info->ملاؤ, ملاؤ);
			قدر = abbrev_branch(space + 1);
			space = strchr(قدر, ' ');
		}
		string_list_append(&info->ملاؤ, xstrdup(قدر));
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
	حالة ڈالو_REMOTE:
		إذا (info->ڈالو_remote_name)
			warning(_("more than one %s"), orig_کلید);
		info->ڈالو_remote_name = xstrdup(قدر);
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

بنية ref_حالتاں {
	بنية remote *remote;
	بنية string_list new_refs, skipped, stale, tracked, heads, ڈالو;
	صحيح queried;
};

#تعریف_کرو REF_حالتاں_INIT { \
	.new_refs = STRING_LIST_INIT_DUP, \
	.skipped = STRING_LIST_INIT_DUP, \
	.stale = STRING_LIST_INIT_DUP, \
	.tracked = STRING_LIST_INIT_DUP, \
	.heads = STRING_LIST_INIT_DUP, \
	.ڈالو = STRING_LIST_INIT_DUP, \
}

ساكن صحيح get_ref_حالتاں(ثابت بنية ref *remote_refs, بنية ref_حالتاں *حالتاں)
{
	بنية ref *fetch_map = عدم, **پوچھ = &fetch_map;
	بنية ref *ref, *stale_refs;
	صحيح i;

	لكل (i = 0; i < حالتاں->remote->fetch.nr; i++)
		إذا (get_fetch_map(remote_refs, &حالتاں->remote->fetch.اشیاء[i], &پوچھ, 1))
			die(_("Could not get fetch map for refspec %s"),
				حالتاں->remote->fetch.اشیاء[i].raw);

	لكل (ref = fetch_map; ref; ref = ref->اگلا) {
		إذا (refname_matches_negative_refspec_شے(ref->name, &حالتاں->remote->fetch))
			string_list_append(&حالتاں->skipped, abbrev_branch(ref->name));
		وإلا إذا (!ref->peer_ref || !refs_ref_exists(get_main_ref_store(the_repository), ref->peer_ref->name))
			string_list_append(&حالتاں->new_refs, abbrev_branch(ref->name));
		وإلا
			string_list_append(&حالتاں->tracked, abbrev_branch(ref->name));
	}
	stale_refs = get_stale_heads(&حالتاں->remote->fetch, fetch_map);
	لكل (ref = stale_refs; ref; ref = ref->اگلا) {
		بنية string_list_شے *شے =
			string_list_append(&حالتاں->stale, abbrev_branch(ref->name));
		شے->util = xstrdup(ref->name);
	}
	free_refs(stale_refs);
	free_refs(fetch_map);

	string_list_ترتیب_دیو(&حالتاں->new_refs);
	string_list_ترتیب_دیو(&حالتاں->skipped);
	string_list_ترتیب_دیو(&حالتاں->tracked);
	string_list_ترتیب_دیو(&حالتاں->stale);

	أرجع 0;
}

بنية ڈالو_info {
	حرف *dest;
	صحيح forced;
	تعداد {
		ڈالو_حالت_CREATE = 0,
		ڈالو_حالت_DELETE,
		ڈالو_حالت_UPTODATE,
		ڈالو_حالت_FASTFORWARD,
		ڈالو_حالت_OUTOFDATE,
		ڈالو_حالت_NOTQUERIED
	} حالت;
};

ساكن صحيح get_ڈالو_ref_حالتاں(ثابت بنية ref *remote_refs,
	بنية ref_حالتاں *حالتاں)
{
	بنية remote *remote = حالتاں->remote;
	بنية ref *ref, *local_refs, *ڈالو_map;
	إذا (remote->mirror)
		أرجع 0;

	local_refs = get_local_heads();
	ڈالو_map = copy_ref_list(remote_refs);

	میل_ڈالو_refs(local_refs, &ڈالو_map, &remote->ڈالو, میل_REFS_NONE);

	لكل (ref = ڈالو_map; ref; ref = ref->اگلا) {
		بنية string_list_شے *شے;
		بنية ڈالو_info *info;

		إذا (!ref->peer_ref)
			استمر;
		oidcpy(&ref->new_oid, &ref->peer_ref->new_oid);

		شے = string_list_append(&حالتاں->ڈالو,
					  abbrev_branch(ref->peer_ref->name));
		شے->util = xcalloc(1, حجم(بنية ڈالو_info));
		info = شے->util;
		info->forced = ref->force;
		info->dest = xstrdup(abbrev_branch(ref->name));

		إذا (is_null_oid(&ref->new_oid)) {
			info->حالت = ڈالو_حالت_DELETE;
		} وإلا إذا (oideq(&ref->old_oid, &ref->new_oid))
			info->حالت = ڈالو_حالت_UPTODATE;
		وإلا إذا (is_null_oid(&ref->old_oid))
			info->حالت = ڈالو_حالت_CREATE;
		وإلا إذا (odb_has_object(the_repository->objects, &ref->old_oid,
					ODB_HAS_OBJECT_RECHECK_PACKED | ODB_HAS_OBJECT_FETCH_PROMISOR) &&
			 ref_newer(&ref->new_oid, &ref->old_oid))
			info->حالت = ڈالو_حالت_FASTFORWARD;
		وإلا
			info->حالت = ڈالو_حالت_OUTOFDATE;
	}
	free_refs(local_refs);
	free_refs(ڈالو_map);
	أرجع 0;
}

ساكن صحيح get_ڈالو_ref_حالتاں_noquery(بنية ref_حالتاں *حالتاں)
{
	صحيح i;
	بنية remote *remote = حالتاں->remote;
	بنية string_list_شے *شے;
	بنية ڈالو_info *info;

	إذا (remote->mirror)
		أرجع 0;

	إذا (!remote->ڈالو.nr) {
		شے = string_list_append(&حالتاں->ڈالو, _("(matching)"));
		info = شے->util = xcalloc(1, حجم(بنية ڈالو_info));
		info->حالت = ڈالو_حالت_NOTQUERIED;
		info->dest = xstrdup(شے->string);
	}
	لكل (i = 0; i < remote->ڈالو.nr; i++) {
		ثابت بنية refspec_شے *spec = &remote->ڈالو.اشیاء[i];
		إذا (spec->matching)
			شے = string_list_append(&حالتاں->ڈالو, _("(matching)"));
		وإلا إذا (لڑی_لمبائی(spec->src))
			شے = string_list_append(&حالتاں->ڈالو, spec->src);
		وإلا
			شے = string_list_append(&حالتاں->ڈالو, _("(delete)"));

		info = شے->util = xcalloc(1, حجم(بنية ڈالو_info));
		info->forced = spec->force;
		info->حالت = ڈالو_حالت_NOTQUERIED;
		info->dest = xstrdup(spec->dst ? spec->dst : شے->string);
	}
	أرجع 0;
}

ساكن صحيح get_سر_ناں(ثابت بنية ref *remote_refs, بنية ref_حالتاں *حالتاں)
{
	بنية ref *ref, *matches;
	بنية ref *fetch_map = عدم, **fetch_map_پوچھ = &fetch_map;
	بنية refspec_شے refspec = {
		.force = 0,
		.نمونہ = 1,
		.src = (حرف *) "refs/heads/*",
		.dst = (حرف *) "refs/heads/*",
	};

	get_fetch_map(remote_refs, &refspec, &fetch_map_پوچھ, 0);
	matches = guess_remote_سر(لبھو_ref_by_name(remote_refs, "HEAD"),
				    fetch_map, REMOTE_GUESS_سر_ALL);
	لكل (ref = matches; ref; ref = ref->اگلا)
		string_list_append(&حالتاں->heads, abbrev_branch(ref->name));

	free_refs(fetch_map);
	free_refs(matches);
	أرجع 0;
}

بنية معلوم_remote {
	بنية معلوم_remote *اگلا;
	بنية remote *remote;
};

بنية معلوم_remotes {
	بنية remote *منزل_delete;
	بنية معلوم_remote *list;
};

ساكن صحيح جوڑو_معلوم_remote(بنية remote *remote, فراغ *cb_انکڑے)
{
	بنية معلوم_remotes *all = cb_انکڑے;
	بنية معلوم_remote *r;

	إذا (!strcmp(all->منزل_delete->name, remote->name))
		أرجع 0;

	r = xmalloc(حجم(*r));
	r->remote = remote;
	r->اگلا = all->list;
	all->list = r;
	أرجع 0;
}

بنية branches_for_remote {
	بنية remote *remote;
	بنية string_list *branches, *skipped;
	بنية معلوم_remotes *keep;
};

ساكن صحيح جوڑو_branch_for_removal(ثابت بنية reference *ref, فراغ *cb_انکڑے)
{
	بنية branches_for_remote *branches = cb_انکڑے;
	بنية refspec_شے refspec;
	بنية معلوم_remote *kr;

	memset(&refspec, 0, حجم(refspec));
	refspec.dst = (حرف *)ref->name;
	إذا (remote_لبھو_tracking(branches->remote, &refspec))
		أرجع 0;
	حرر(refspec.src);

	/* don't delete a branch if another remote also uses it */
	لكل (kr = branches->keep->list; kr; kr = kr->اگلا) {
		memset(&refspec, 0, حجم(refspec));
		refspec.dst = (حرف *)ref->name;
		إذا (!remote_لبھو_tracking(kr->remote, &refspec)) {
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
	بنية ترقی *ترقی;
	بنية strbuf *err;
	uint32_t ترقی_nr;
	uint64_t اشاریہ;
};

ساكن فراغ compute_renamed_ref(بنية rename_info *rename,
				ثابت حرف *refname,
				بنية strbuf *باہرلا)
{
	strbuf_reset(باہرلا);
	strbuf_addstr(باہرلا, refname);
	strbuf_splice(باہرلا, لڑی_لمبائی("refs/remotes/"), لڑی_لمبائی(rename->old_name),
		      rename->new_name, لڑی_لمبائی(rename->new_name));
}

ساكن صحيح rename_one_reflog_اندراج(ثابت حرف *old_refname,
				   بنية object_id *old_oid,
				   بنية object_id *new_oid,
				   ثابت حرف *committer,
				   timestamp_t timestamp, صحيح tz,
				   ثابت حرف *پیغام, فراغ *cb_انکڑے)
{
	بنية rename_info *rename = cb_انکڑے;
	بنية strbuf new_refname = STRBUF_INIT;
	بنية strbuf پچھان_تت = STRBUF_INIT;
	بنية strbuf name = STRBUF_INIT;
	بنية strbuf mail = STRBUF_INIT;
	بنية ident_ونڈو ident;
	ثابت حرف *date;
	صحيح error;

	compute_renamed_ref(rename, old_refname, &new_refname);

	إذا (ونڈو_ident_سطر(&ident, committer, لڑی_لمبائی(committer)) < 0) {
		error = -1;
		اذهب باہرلا;
	}

	strbuf_جوڑو(&name, ident.name_begin, ident.name_سرا - ident.name_begin);
	strbuf_جوڑو(&mail, ident.mail_begin, ident.mail_سرا - ident.mail_begin);

	date = show_date(timestamp, tz, DATE_منوال(عام_2));
	strbuf_addstr(&پچھان_تت, fmt_ident(name.buf, mail.buf,
					  WANT_BLANK_IDENT, date, 0));

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      new_oid, old_oid, پچھان_تت.buf, پیغام,
					      rename->اشاریہ++, rename->err);

باہرلا:
	strbuf_release(&new_refname);
	strbuf_release(&پچھان_تت);
	strbuf_release(&name);
	strbuf_release(&mail);
	أرجع error;
}

ساكن صحيح rename_one_reflog(ثابت حرف *old_refname,
			     ثابت بنية object_id *old_oid,
			     بنية rename_info *rename)
{
	بنية strbuf new_refname = STRBUF_INIT;
	بنية strbuf سنیہا = STRBUF_INIT;
	صحيح error;

	إذا (!refs_reflog_exists(get_main_ref_store(the_repository), old_refname))
		أرجع 0;

	error = refs_for_each_reflog_ent(get_main_ref_store(the_repository),
					 old_refname, rename_one_reflog_اندراج, rename);
	إذا (error < 0)
		اذهب باہرلا;

	compute_renamed_ref(rename, old_refname, &new_refname);

	/*
	 * Manually write the reflog entry for the now-renamed ref. We cannot
	 * rely on `rename_one_ref()` to do this for us as that would screw
	 * over order in which reflog entries are being written.
	 *
	 * Furthermore, we only append the entry in case the reference
	 * resolves. Missing references shouldn't have reflogs anyway.
	 */
	strbuf_addf(&سنیہا, "remote: renamed %s to %s", old_refname,
		    new_refname.buf);

	error = ref_transaction_update_reflog(rename->transaction, new_refname.buf,
					      old_oid, old_oid, git_committer_info(0),
					      سنیہا.buf, rename->اشاریہ++, rename->err);
	إذا (error < 0)
		أرجع error;

باہرلا:
	strbuf_release(&new_refname);
	strbuf_release(&سنیہا);
	أرجع error;
}

ساكن صحيح rename_one_ref(ثابت بنية reference *ref, فراغ *cb_انکڑے)
{
	بنية strbuf new_referent = STRBUF_INIT;
	بنية strbuf new_refname = STRBUF_INIT;
	بنية rename_info *rename = cb_انکڑے;
	ثابت بنية object_id *oid = ref->oid;
	ثابت حرف *referent = ref->نشانہ;
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
		اذهب باہرلا;

	error = ref_transaction_update(rename->transaction, new_refname.buf, oid, null_oid(the_hash_algo),
				       (ref->flags & REF_ISSYMREF) ? new_referent.buf : عدم, عدم,
				       REF_SKIP_CREATE_REFLOG | REF_NO_DEREF | REF_SKIP_OID_VERIFICATION,
				       عدم, rename->err);
	إذا (error < 0)
		اذهب باہرلا;

	error = rename_one_reflog(ref->name, oid, rename);
	إذا (error < 0)
		اذهب باہرلا;

	display_ترقی(rename->ترقی, ++rename->ترقی_nr);

باہرلا:
	strbuf_release(&new_referent);
	strbuf_release(&new_refname);
	أرجع error;
}

ساكن صحيح migrate_file(بنية remote *remote)
{
	بنية strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "remote.%s.url", remote->name);
	لكل (ناپ_t i = 0; i < remote->url.nr; i++)
		repo_config_سیٹ_multivar(the_repository, buf.buf, remote->url.v[i], "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.push", remote->name);
	لكل (صحيح i = 0; i < remote->ڈالو.nr; i++)
		repo_config_سیٹ_multivar(the_repository, buf.buf, remote->ڈالو.اشیاء[i].raw, "^$", 0);
	strbuf_reset(&buf);
	strbuf_addf(&buf, "remote.%s.fetch", remote->name);
	لكل (صحيح i = 0; i < remote->fetch.nr; i++)
		repo_config_سیٹ_multivar(the_repository, buf.buf, remote->fetch.اشیاء[i].raw, "^$", 0);
#جے_بنا_تعریف WITH_BREAKING_CHANGES
	إذا (remote->origin == REMOTE_REMOTES)
		unlink_or_warn(repo_git_رستہ_replace(the_repository, &buf,
						     "remotes/%s", remote->name));
	وإلا إذا (remote->origin == REMOTE_BRANCHES)
		unlink_or_warn(repo_git_رستہ_replace(the_repository, &buf,
						     "branches/%s", remote->name));
#جے_انت /* WITH_BREAKING_CHANGES */
	strbuf_release(&buf);

	أرجع 0;
}

بنية ڈالو_default_info
{
	ثابت حرف *old_name;
	تعداد config_scope scope;
	بنية strbuf origin;
	صحيح linenr;
};

ساكن صحيح config_read_ڈالو_default(ثابت حرف *کلید, ثابت حرف *قدر,
	ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت بنية کلید_قدر_info *kvi = ctx->kvi;

	بنية ڈالو_default_info* info = cb;
	إذا (strcmp(کلید, "remote.pushdefault") ||
	    !قدر || strcmp(قدر, info->old_name))
		أرجع 0;

	info->scope = kvi->scope;
	strbuf_reset(&info->origin);
	strbuf_addstr(&info->origin, config_origin_type_name(kvi->origin_type));
	info->linenr = kvi->linenr;

	أرجع 0;
}

ساكن فراغ handle_ڈالو_default(ثابت حرف* old_name, ثابت حرف* new_name)
{
	بنية ڈالو_default_info ڈالو_default = {
		.old_name = old_name,
		.scope = CONFIG_SCOPE_UNKNOWN,
		.origin = STRBUF_INIT,
		.linenr = -1,
	};
	repo_config(the_repository, config_read_ڈالو_default, &ڈالو_default);
	إذا (ڈالو_default.scope >= CONFIG_SCOPE_COMMAND)
		; /* pass */
	وإلا إذا (ڈالو_default.scope >= CONFIG_SCOPE_LOCAL) {
		صحيح نتیجہ = repo_config_سیٹ_gently(the_repository, "remote.pushDefault",
						    new_name);
		إذا (new_name && نتیجہ && نتیجہ != CONFIG_NOTHING_سیٹ)
			die(_("could not set '%s'"), "remote.pushDefault");
		وإلا إذا (!new_name && نتیجہ && نتیجہ != CONFIG_NOTHING_سیٹ)
			die(_("could not unset '%s'"), "remote.pushDefault");
	} وإلا إذا (ڈالو_default.scope >= CONFIG_SCOPE_SYSTEM) {
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

ساكن ثابت حرف conflicting_remote_refs_advice[] = N_(
	"The remote you are trying to rename has conflicting references in the\n"
	"new target refspec. This is most likely caused by you trying to nest\n"
	"a remote into itself, e.g. by renaming 'parent' into 'parent/child'\n"
	"or by unnesting a remote, e.g. the other way round.\n"
	"\n"
	"If that is the case, you can address this by first renaming the\n"
	"remote to a different name.\n");

ساكن صحيح mv(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
	      بنية repository *repo بن_ورتیا)
{
	صحيح show_ترقی = isatty(2);
	بنية option options[] = {
		OPT_BOOL(0, "progress", &show_ترقی, N_("force progress reporting")),
		OPT_سرا()
	};
	بنية remote *oldremote, *newremote;
	بنية strbuf buf = STRBUF_INIT, buf2 = STRBUF_INIT, buf3 = STRBUF_INIT,
		old_remote_context = STRBUF_INIT, err = STRBUF_INIT;
	بنية rename_info rename = {
		.err = &err,
	};
	صحيح refspecs_need_update = 0;
	صحيح نتیجہ = 0;

	argc = parse_options(argc, argv, اگیتر, options,
			     builtin_remote_rename_usage, 0);

	إذا (argc != 2)
		usage_with_options(builtin_remote_rename_usage, options);

	rename.old_name = argv[0];
	rename.new_name = argv[1];

	oldremote = remote_get(rename.old_name);
	إذا (!remote_is_configured(oldremote, 1)) {
		error(_("No such remote: '%s'"), rename.old_name);
		باہر_نکلو(2);
	}

	إذا (!strcmp(rename.old_name, rename.new_name) && oldremote->origin != REMOTE_CONFIG)
		أرجع migrate_file(oldremote);

	newremote = remote_get(rename.new_name);
	إذا (remote_is_configured(newremote, 1)) {
		error(_("remote %s already exists."), rename.new_name);
		باہر_نکلو(3);
	}

	إذا (!valid_remote_name(rename.new_name))
		die(_("'%s' is not a valid remote name"), rename.new_name);

	strbuf_addf(&buf, "remote.%s", rename.old_name);
	strbuf_addf(&buf2, "remote.%s", rename.new_name);
	إذا (repo_config_rename_section(the_repository, buf.buf, buf2.buf) < 1) {
		نتیجہ = error(_("Could not rename config section '%s' to '%s'"),
			       buf.buf, buf2.buf);
		اذهب باہرلا;
	}

	strbuf_addf(&old_remote_context, ":refs/remotes/%s/", rename.old_name);

	لكل (صحيح i = 0; i < oldremote->fetch.nr && !refspecs_need_update; i++)
		refspecs_need_update = !!لڑی_کھوج(oldremote->fetch.اشیاء[i].raw,
						old_remote_context.buf);

	إذا (refspecs_need_update) {
		بنية refs_for_each_ref_options opts = {
			.flags = REFS_FOR_EACH_INCLUDE_BROKEN,
		};
		rename.transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
							       0, &err);
		إذا (!rename.transaction)
			اذهب باہرلا;

		إذا (show_ترقی)
			rename.ترقی = شروع_delayed_ترقی(the_repository,
								 _("Renaming remote references"), 0);

		strbuf_reset(&buf);
		strbuf_addf(&buf, "refs/remotes/%s/", rename.old_name);
		opts.اگیتر = buf.buf;

		نتیجہ = refs_for_each_ref_ext(get_main_ref_store(the_repository),
					       rename_one_ref, &rename, &opts);
		إذا (نتیجہ < 0)
			die(_("queueing remote ref renames failed: %s"), rename.err->buf);

		نتیجہ = ref_transaction_prepare(rename.transaction, &err);
		إذا (نتیجہ < 0) {
			error("renaming remote references failed: %s", err.buf);
			إذا (نتیجہ == REF_TRANSACTION_ERROR_NAME_CONFLICT)
				advise(conflicting_remote_refs_advice);
			die(عدم);
		}
	}

	إذا (oldremote->fetch.nr) {
		strbuf_reset(&buf);
		strbuf_addf(&buf, "remote.%s.fetch", rename.new_name);
		repo_config_سیٹ_multivar(the_repository, buf.buf, عدم, عدم, CONFIG_FLAGS_MULTI_REPLACE);
		لكل (صحيح i = 0; i < oldremote->fetch.nr; i++) {
			حرف *ptr;

			strbuf_reset(&buf2);
			strbuf_addstr(&buf2, oldremote->fetch.اشیاء[i].raw);
			ptr = لڑی_کھوج(buf2.buf, old_remote_context.buf);
			إذا (ptr) {
				strbuf_splice(&buf2,
					      ptr-buf2.buf + لڑی_لمبائی(":refs/remotes/"),
					      لڑی_لمبائی(rename.old_name), rename.new_name,
					      لڑی_لمبائی(rename.new_name));
			} وإلا
				warning(_("Not updating non-default fetch refspec\n"
					  "\t%s\n"
					  "\tPlease update the configuration manually if necessary."),
					buf2.buf);

			repo_config_سیٹ_multivar(the_repository, buf.buf, buf2.buf, "^$", 0);
		}
	}

	read_branches();
	لكل (ناپ_t i = 0; i < branch_list.nr; i++) {
		بنية string_list_شے *شے = branch_list.اشیاء + i;
		بنية branch_info *info = شے->util;
		إذا (info->remote_name && !strcmp(info->remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.remote", شے->string);
			repo_config_سیٹ(the_repository, buf.buf, rename.new_name);
		}
		إذا (info->ڈالو_remote_name && !strcmp(info->ڈالو_remote_name, rename.old_name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushRemote", شے->string);
			repo_config_سیٹ(the_repository, buf.buf, rename.new_name);
		}
	}

	إذا (refspecs_need_update) {
		نتیجہ = ref_transaction_commit(rename.transaction, &err);
		إذا (نتیجہ < 0)
			die(_("renaming remote refs failed: %s"), rename.err->buf);

		stop_ترقی(&rename.ترقی);

		handle_ڈالو_default(rename.old_name, rename.new_name);
	}

باہرلا:
	ref_transaction_free(rename.transaction);
	strbuf_release(&old_remote_context);
	strbuf_release(&buf);
	strbuf_release(&buf2);
	strbuf_release(&buf3);
	strbuf_release(&err);
	أرجع نتیجہ;
}

ساكن صحيح rm(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
	      بنية repository *repo بن_ورتیا)
{
	بنية option options[] = {
		OPT_سرا()
	};
	بنية remote *remote;
	بنية strbuf buf = STRBUF_INIT;
	بنية معلوم_remotes معلوم_remotes = { عدم, عدم };
	بنية string_list branches = STRING_LIST_INIT_DUP;
	بنية string_list skipped = STRING_LIST_INIT_DUP;
	بنية branches_for_remote cb_انکڑے;
	صحيح نتیجہ;

	memset(&cb_انکڑے, 0, حجم(cb_انکڑے));
	cb_انکڑے.branches = &branches;
	cb_انکڑے.skipped = &skipped;
	cb_انکڑے.keep = &معلوم_remotes;

	argc = parse_options(argc, argv, اگیتر, options,
			     builtin_remote_rm_usage, 0);
	إذا (argc != 1)
		usage_with_options(builtin_remote_rm_usage, options);

	remote = remote_get(argv[0]);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote: '%s'"), argv[0]);
		باہر_نکلو(2);
	}

	معلوم_remotes.منزل_delete = remote;
	for_each_remote(جوڑو_معلوم_remote, &معلوم_remotes);

	read_branches();
	لكل (ناپ_t i = 0; i < branch_list.nr; i++) {
		بنية string_list_شے *شے = branch_list.اشیاء + i;
		بنية branch_info *info = شے->util;
		إذا (info->remote_name && !strcmp(info->remote_name, remote->name)) {
			ثابت حرف *کلیدیں[] = { "remote", "merge", عدم }, **k;
			لكل (k = کلیدیں; *k; k++) {
				strbuf_reset(&buf);
				strbuf_addf(&buf, "branch.%s.%s",
						شے->string, *k);
				نتیجہ = repo_config_سیٹ_gently(the_repository, buf.buf, عدم);
				إذا (نتیجہ && نتیجہ != CONFIG_NOTHING_سیٹ)
					die(_("could not unset '%s'"), buf.buf);
			}
		}
		إذا (info->ڈالو_remote_name && !strcmp(info->ڈالو_remote_name, remote->name)) {
			strbuf_reset(&buf);
			strbuf_addf(&buf, "branch.%s.pushremote", شے->string);
			نتیجہ = repo_config_سیٹ_gently(the_repository, buf.buf, عدم);
			إذا (نتیجہ && نتیجہ != CONFIG_NOTHING_سیٹ)
				die(_("could not unset '%s'"), buf.buf);
		}
	}

	/*
	 * We cannot just pass a function to for_each_ref() which deletes
	 * the branches one by one, since for_each_ref() relies on cached
	 * refs, which are invalidated when deleting a branch.
	 */
	cb_انکڑے.remote = remote;
	نتیجہ = refs_for_each_ref(get_main_ref_store(the_repository),
				   جوڑو_branch_for_removal, &cb_انکڑے);
	strbuf_release(&buf);

	إذا (!نتیجہ)
		نتیجہ = refs_delete_refs(get_main_ref_store(the_repository),
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
		لكل (ناپ_t i = 0; i < skipped.nr; i++)
			fprintf(stderr, "  git branch -d %s\n",
				skipped.اشیاء[i].string);
	}
	string_list_clear(&skipped, 0);

	إذا (!نتیجہ) {
		strbuf_addf(&buf, "remote.%s", remote->name);
		إذا (repo_config_rename_section(the_repository, buf.buf, عدم) < 1) {
			نتیجہ = error(_("Could not remove config section '%s'"), buf.buf);
			اذهب باہرلا;
		}

		handle_ڈالو_default(remote->name, عدم);
	}

باہرلا:
	لكل (بنية معلوم_remote *r = معلوم_remotes.list; r;) {
		بنية معلوم_remote *اگلا = r->اگلا;
		حرر(r);
		r = اگلا;
	}
	strbuf_release(&buf);
	أرجع نتیجہ;
}

ساكن فراغ clear_ڈالو_info(فراغ *util, ثابت حرف *string بن_ورتیا)
{
	بنية ڈالو_info *info = util;
	حرر(info->dest);
	حرر(info);
}

ساكن فراغ free_remote_ref_حالتاں(بنية ref_حالتاں *حالتاں)
{
	string_list_clear(&حالتاں->new_refs, 0);
	string_list_clear(&حالتاں->skipped, 0);
	string_list_clear(&حالتاں->stale, 1);
	string_list_clear(&حالتاں->tracked, 0);
	string_list_clear(&حالتاں->heads, 0);
	string_list_clear_func(&حالتاں->ڈالو, clear_ڈالو_info);
}

ساكن صحيح append_ref_منزل_tracked_list(ثابت بنية reference *ref, فراغ *cb_انکڑے)
{
	بنية ref_حالتاں *حالتاں = cb_انکڑے;
	بنية refspec_شے refspec;

	إذا (ref->flags & REF_ISSYMREF)
		أرجع 0;

	memset(&refspec, 0, حجم(refspec));
	refspec.dst = (حرف *)ref->name;
	إذا (!remote_لبھو_tracking(حالتاں->remote, &refspec)) {
		string_list_append(&حالتاں->tracked, abbrev_branch(refspec.src));
		حرر(refspec.src);
	}

	أرجع 0;
}

ساكن صحيح get_remote_ref_حالتاں(ثابت حرف *name,
				 بنية ref_حالتاں *حالتاں,
				 صحيح query)
{
	حالتاں->remote = remote_get(name);
	إذا (!حالتاں->remote)
		أرجع error(_("No such remote: '%s'"), name);

	read_branches();

	إذا (query) {
		بنية transport *transport;
		ثابت بنية ref *remote_refs;

		transport = transport_get(حالتاں->remote, حالتاں->remote->url.v[0]);
		remote_refs = transport_get_remote_refs(transport, عدم);

		حالتاں->queried = 1;
		إذا (query & GET_REF_حالتاں)
			get_ref_حالتاں(remote_refs, حالتاں);
		إذا (query & GET_سر_ناں)
			get_سر_ناں(remote_refs, حالتاں);
		إذا (query & GET_ڈالو_REF_حالتاں)
			get_ڈالو_ref_حالتاں(remote_refs, حالتاں);
		transport_disconnect(transport);
	} وإلا {
		refs_for_each_ref(get_main_ref_store(the_repository),
				  append_ref_منزل_tracked_list, حالتاں);
		string_list_ترتیب_دیو(&حالتاں->tracked);
		get_ڈالو_ref_حالتاں_noquery(حالتاں);
	}

	أرجع 0;
}

بنية show_info {
	بنية string_list list;
	بنية ref_حالتاں حالتاں;
	صحيح چوڑائی, width2;
	صحيح any_rebase;
};

#تعریف_کرو SHOW_INFO_INIT { \
	.list = STRING_LIST_INIT_DUP, \
	.حالتاں = REF_حالتاں_INIT, \
}

ساكن صحيح جوڑو_remote_منزل_show_info(بنية string_list_شے *شے, فراغ *cb_انکڑے)
{
	بنية show_info *info = cb_انکڑے;
	صحيح n = لڑی_لمبائی(شے->string);
	إذا (n > info->چوڑائی)
		info->چوڑائی = n;
	string_list_داخل_کرو(&info->list, شے->string);
	أرجع 0;
}

ساكن صحيح show_remote_info_شے(بنية string_list_شے *شے, فراغ *cb_انکڑے)
{
	بنية show_info *info = cb_انکڑے;
	بنية ref_حالتاں *حالتاں = &info->حالتاں;
	ثابت حرف *name = شے->string;

	إذا (حالتاں->queried) {
		ثابت حرف *fmt = "%s";
		ثابت حرف *arg = "";
		إذا (string_list_has_string(&حالتاں->new_refs, name)) {
			fmt = _(" new (next fetch will store in remotes/%s)");
			arg = حالتاں->remote->name;
		} وإلا إذا (string_list_has_string(&حالتاں->tracked, name))
			arg = _(" tracked");
		وإلا إذا (string_list_has_string(&حالتاں->skipped, name))
			arg = _(" skipped");
		وإلا إذا (string_list_has_string(&حالتاں->stale, name))
			arg = _(" stale (use 'git remote prune' to remove)");
		وإلا
			arg = _(" ???");
		لکھو("    %-*s", info->چوڑائی, name);
		لکھو(fmt, arg);
		لکھو("\n");
	} وإلا
		لکھو("    %s\n", name);

	أرجع 0;
}

ساكن صحيح جوڑو_local_منزل_show_info(بنية string_list_شے *branch_شے, فراغ *cb_انکڑے)
{
	بنية show_info *show_info = cb_انکڑے;
	بنية ref_حالتاں *حالتاں = &show_info->حالتاں;
	بنية branch_info *branch_info = branch_شے->util;
	بنية string_list_شے *شے;
	صحيح n;

	إذا (!branch_info->ملاؤ.nr || !branch_info->remote_name ||
	    strcmp(حالتاں->remote->name, branch_info->remote_name))
		أرجع 0;
	إذا ((n = لڑی_لمبائی(branch_شے->string)) > show_info->چوڑائی)
		show_info->چوڑائی = n;
	إذا (branch_info->rebase >= REBASE_TRUE)
		show_info->any_rebase = 1;

	شے = string_list_داخل_کرو(&show_info->list, branch_شے->string);
	شے->util = branch_info;

	أرجع 0;
}

ساكن صحيح show_local_info_شے(بنية string_list_شے *شے, فراغ *cb_انکڑے)
{
	بنية show_info *show_info = cb_انکڑے;
	بنية branch_info *branch_info = شے->util;
	بنية string_list *ملاؤ = &branch_info->ملاؤ;
	صحيح چوڑائی = show_info->چوڑائی + 4;

	إذا (branch_info->rebase >= REBASE_TRUE && branch_info->ملاؤ.nr > 1) {
		error(_("invalid branch.%s.merge; cannot rebase onto > 1 branch"),
			شے->string);
		أرجع 0;
	}

	لکھو("    %-*s ", show_info->چوڑائی, شے->string);
	إذا (branch_info->rebase >= REBASE_TRUE) {
		ثابت حرف *پیغام;
		إذا (branch_info->rebase == REBASE_INTERACTIVE)
			پیغام = _("rebases interactively onto remote %s");
		وإلا إذا (branch_info->rebase == REBASE_MERGES)
			پیغام = _("rebases interactively (with merges) onto "
				"remote %s");
		وإلا
			پیغام = _("rebases onto remote %s");
		printf_ln(پیغام, ملاؤ->اشیاء[0].string);
		أرجع 0;
	} وإلا إذا (show_info->any_rebase) {
		printf_ln(_(" merges with remote %s"), ملاؤ->اشیاء[0].string);
		چوڑائی++;
	} وإلا {
		printf_ln(_("merges with remote %s"), ملاؤ->اشیاء[0].string);
	}
	لكل (ناپ_t i = 1; i < ملاؤ->nr; i++)
		لکھو(_("%-*s    and with remote %s\n"), چوڑائی, "",
		       ملاؤ->اشیاء[i].string);

	أرجع 0;
}

ساكن صحيح جوڑو_ڈالو_منزل_show_info(بنية string_list_شے *ڈالو_شے, فراغ *cb_انکڑے)
{
	بنية show_info *show_info = cb_انکڑے;
	بنية ڈالو_info *ڈالو_info = ڈالو_شے->util;
	بنية string_list_شے *شے;
	صحيح n;
	إذا ((n = لڑی_لمبائی(ڈالو_شے->string)) > show_info->چوڑائی)
		show_info->چوڑائی = n;
	إذا ((n = لڑی_لمبائی(ڈالو_info->dest)) > show_info->width2)
		show_info->width2 = n;
	شے = string_list_append(&show_info->list, ڈالو_شے->string);
	شے->util = ڈالو_شے->util;
	أرجع 0;
}

/*
 * Sorting comparison for a string list that has push_info
 * structs in its util field
 */
ساكن صحيح cmp_string_with_ڈالو(ثابت فراغ *va, ثابت فراغ *vb)
{
	ثابت بنية string_list_شے *a = va;
	ثابت بنية string_list_شے *b = vb;
	ثابت بنية ڈالو_info *a_ڈالو = a->util;
	ثابت بنية ڈالو_info *b_ڈالو = b->util;
	صحيح cmp = strcmp(a->string, b->string);
	أرجع cmp ? cmp : strcmp(a_ڈالو->dest, b_ڈالو->dest);
}

ساكن صحيح show_ڈالو_info_شے(بنية string_list_شے *شے, فراغ *cb_انکڑے)
{
	بنية show_info *show_info = cb_انکڑے;
	بنية ڈالو_info *ڈالو_info = شے->util;
	ثابت حرف *src = شے->string, *حالت = عدم;

	بدل (ڈالو_info->حالت) {
	حالة ڈالو_حالت_CREATE:
		حالت = _("create");
		اكسر;
	حالة ڈالو_حالت_DELETE:
		حالت = _("delete");
		src = _("(none)");
		اكسر;
	حالة ڈالو_حالت_UPTODATE:
		حالت = _("up to date");
		اكسر;
	حالة ڈالو_حالت_FASTFORWARD:
		حالت = _("fast-forwardable");
		اكسر;
	حالة ڈالو_حالت_OUTOFDATE:
		حالت = _("local out of date");
		اكسر;
	حالة ڈالو_حالت_NOTQUERIED:
		اكسر;
	}
	إذا (حالت) {
		إذا (ڈالو_info->forced)
			printf_ln(_("    %-*s forces to %-*s (%s)"), show_info->چوڑائی, src,
			       show_info->width2, ڈالو_info->dest, حالت);
		وإلا
			printf_ln(_("    %-*s pushes to %-*s (%s)"), show_info->چوڑائی, src,
			       show_info->width2, ڈالو_info->dest, حالت);
	} وإلا {
		إذا (ڈالو_info->forced)
			printf_ln(_("    %-*s forces to %s"), show_info->چوڑائی, src,
			       ڈالو_info->dest);
		وإلا
			printf_ln(_("    %-*s pushes to %s"), show_info->چوڑائی, src,
			       ڈالو_info->dest);
	}
	أرجع 0;
}

ساكن صحيح get_one_اندراج(بنية remote *remote, فراغ *priv)
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
	url = ڈالو_url_دا_remote(remote);
	لكل (ناپ_t i = 0; i < url->nr; i++) {
		strbuf_addf(&remote_info_buf, "%s (push)", url->v[i]);
		string_list_append(list, remote->name)->util =
				strbuf_detach(&remote_info_buf, عدم);
	}

	أرجع 0;
}

ساكن صحيح show_all(فراغ)
{
	بنية string_list list = STRING_LIST_INIT_DUP;
	صحيح نتیجہ;

	نتیجہ = for_each_remote(get_one_اندراج, &list);

	إذا (!نتیجہ) {
		string_list_ترتیب_دیو(&list);
		لكل (ناپ_t i = 0; i < list.nr; i++) {
			بنية string_list_شے *شے = list.اشیاء + i;
			إذا (verbose)
				لکھو("%s\t%s\n", شے->string,
					شے->util ? (ثابت حرف *)شے->util : "");
			وإلا {
				إذا (i && !strcmp((شے - 1)->string, شے->string))
					استمر;
				لکھو("%s\n", شے->string);
			}
		}
	}
	string_list_clear(&list, 1);
	أرجع نتیجہ;
}

ساكن صحيح show(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		بنية repository *repo بن_ورتیا)
{
	صحيح no_query = 0, نتیجہ = 0, query_flag = 0;
	بنية option options[] = {
		OPT_BOOL('n', عدم, &no_query, N_("do not query remotes")),
		OPT_سرا()
	};
	بنية show_info info = SHOW_INFO_INIT;

	argc = parse_options(argc, argv, اگیتر, options,
			     builtin_remote_show_usage,
			     0);

	إذا (argc < 1)
		أرجع show_all();

	إذا (!no_query)
		query_flag = (GET_REF_حالتاں | GET_سر_ناں | GET_ڈالو_REF_حالتاں);

	لكل (; argc; argc--, argv++) {
		ناپ_t i;
		بنية strvec *url;

		get_remote_ref_حالتاں(*argv, &info.حالتاں, query_flag);

		printf_ln(_("* remote %s"), *argv);
		printf_ln(_("  Fetch URL: %s"), info.حالتاں.remote->url.v[0]);
		url = ڈالو_url_دا_remote(info.حالتاں.remote);
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
		وإلا إذا (!info.حالتاں.heads.nr)
			printf_ln(_("  HEAD branch: %s"), _("(unknown)"));
		وإلا إذا (info.حالتاں.heads.nr == 1)
			printf_ln(_("  HEAD branch: %s"), info.حالتاں.heads.اشیاء[0].string);
		وإلا {
			لکھو(_("  HEAD branch (remote HEAD is ambiguous,"
				 " may be one of the following):\n"));
			لكل (i = 0; i < info.حالتاں.heads.nr; i++)
				لکھو("    %s\n", info.حالتاں.heads.اشیاء[i].string);
		}

		/* remote branch info */
		info.چوڑائی = 0;
		for_each_string_list(&info.حالتاں.new_refs, جوڑو_remote_منزل_show_info, &info);
		for_each_string_list(&info.حالتاں.skipped, جوڑو_remote_منزل_show_info, &info);
		for_each_string_list(&info.حالتاں.tracked, جوڑو_remote_منزل_show_info, &info);
		for_each_string_list(&info.حالتاں.stale, جوڑو_remote_منزل_show_info, &info);
		إذا (info.list.nr)
			printf_ln(Q_("  Remote branch:%s",
				     "  Remote branches:%s",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_remote_info_شے, &info);
		string_list_clear(&info.list, 0);

		/* git pull info */
		info.چوڑائی = 0;
		info.any_rebase = 0;
		for_each_string_list(&branch_list, جوڑو_local_منزل_show_info, &info);
		إذا (info.list.nr)
			printf_ln(Q_("  Local branch configured for 'git pull':",
				     "  Local branches configured for 'git pull':",
				     info.list.nr));
		for_each_string_list(&info.list, show_local_info_شے, &info);
		string_list_clear(&info.list, 0);

		/* git push info */
		إذا (info.حالتاں.remote->mirror)
			printf_ln(_("  Local refs will be mirrored by 'git push'"));

		info.چوڑائی = info.width2 = 0;
		for_each_string_list(&info.حالتاں.ڈالو, جوڑو_ڈالو_منزل_show_info, &info);
		QSORT(info.list.اشیاء, info.list.nr, cmp_string_with_ڈالو);
		إذا (info.list.nr)
			printf_ln(Q_("  Local ref configured for 'git push'%s:",
				     "  Local refs configured for 'git push'%s:",
				     info.list.nr),
				  no_query ? _(" (status not queried)") : "");
		for_each_string_list(&info.list, show_ڈالو_info_شے, &info);
		string_list_clear(&info.list, 0);

		free_remote_ref_حالتاں(&info.حالتاں);
	}

	أرجع نتیجہ;
}

ساكن فراغ report_سیٹ_سر_auto(ثابت حرف *remote, ثابت حرف *سر_name,
			بنية strbuf *b_local_سر, صحيح was_detached) {
	بنية strbuf buf_اگیتر = STRBUF_INIT;
	ثابت حرف *prev_سر = عدم;

	strbuf_addf(&buf_اگیتر, "refs/remotes/%s/", remote);
	skip_اگیتر(b_local_سر->buf, buf_اگیتر.buf, &prev_سر);

	إذا (prev_سر && !strcmp(prev_سر, سر_name))
		لکھو(_("'%s/HEAD' is unchanged and points to '%s'\n"),
			remote, سر_name);
	وإلا إذا (prev_سر)
		لکھو(_("'%s/HEAD' has changed from '%s' and now points to '%s'\n"),
			remote, prev_سر, سر_name);
	وإلا إذا (!b_local_سر->len)
		لکھو(_("'%s/HEAD' is now created and points to '%s'\n"),
			remote, سر_name);
	وإلا إذا (was_detached && b_local_سر->len)
		لکھو(_("'%s/HEAD' was detached at '%s' and now points to '%s'\n"),
			remote, b_local_سر->buf, سر_name);
	وإلا
		لکھو(_("'%s/HEAD' used to point to '%s' "
			"(which is not a remote branch), but now points to '%s'\n"),
			remote, b_local_سر->buf, سر_name);
	strbuf_release(&buf_اگیتر);
}

ساكن صحيح سیٹ_سر(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		    بنية repository *repo بن_ورتیا)
{
	صحيح opt_a = 0, opt_d = 0, نتیجہ = 0, was_detached;
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
		OPT_سرا()
	};
	argc = parse_options(argc, argv, اگیتر, options,
			     builtin_remote_sethead_usage, 0);

	/* All modes require at least a remote name. */
	إذا (!argc)
		usage_with_options(builtin_remote_sethead_usage, options);

	strbuf_addf(&b_سر, "refs/remotes/%s/HEAD", argv[0]);
	remote = remote_get(argv[0]);

	إذا (!opt_a && !opt_d && argc == 2) {
		سر_name = xstrdup(argv[1]);
	} وإلا إذا (opt_a && !opt_d && argc == 1) {
		بنية ref_حالتاں حالتاں = REF_حالتاں_INIT;
		get_remote_ref_حالتاں(argv[0], &حالتاں, GET_سر_ناں);
		إذا (!حالتاں.heads.nr)
			نتیجہ |= error(_("Cannot determine remote HEAD"));
		وإلا إذا (حالتاں.heads.nr > 1) {
			نتیجہ |= error(_("Multiple remote HEAD branches. "
					  "Please choose one explicitly with:"));
			لكل (ناپ_t i = 0; i < حالتاں.heads.nr; i++)
				fprintf(stderr, "  git remote set-head %s %s\n",
					argv[0], حالتاں.heads.اشیاء[i].string);
		} وإلا
			سر_name = xstrdup(حالتاں.heads.اشیاء[0].string);
		free_remote_ref_حالتاں(&حالتاں);
	} وإلا إذا (opt_d && !opt_a && argc == 1) {
		إذا (refs_delete_ref(refs, عدم, b_سر.buf, عدم, REF_NO_DEREF))
			نتیجہ |= error(_("Could not delete %s"), b_سر.buf);
	} وإلا
		usage_with_options(builtin_remote_sethead_usage, options);

	إذا (!سر_name)
		اذهب cleanup;
	strbuf_addf(&b_remote_سر, "refs/remotes/%s/%s", argv[0], سر_name);
	إذا (!refs_ref_exists(refs, b_remote_سر.buf)) {
		نتیجہ |= error(_("Not a valid ref: %s"), b_remote_سر.buf);
		اذهب cleanup;
	}
	was_detached = refs_update_symref_extended(refs, b_سر.buf, b_remote_سر.buf,
			"remote set-head", &b_local_سر, 0);
	إذا (was_detached == -1) {
		نتیجہ |= error(_("Could not set up %s"), b_سر.buf);
		اذهب cleanup;
	}
	إذا (opt_a)
		report_سیٹ_سر_auto(argv[0], سر_name, &b_local_سر, was_detached);
	إذا (remote->follow_remote_سر == FOLLOW_REMOTE_ALWAYS) {
		بنية strbuf config_name = STRBUF_INIT;
		strbuf_addf(&config_name,
			"remote.%s.followremotehead", remote->name);
		repo_config_سیٹ(the_repository, config_name.buf, "warn");
		strbuf_release(&config_name);
	}

cleanup:
	حرر(سر_name);
	strbuf_release(&b_سر);
	strbuf_release(&b_remote_سر);
	strbuf_release(&b_local_سر);
	أرجع نتیجہ;
}

ساكن صحيح prune_remote(ثابت حرف *remote, صحيح dry_لگاتار)
{
	صحيح نتیجہ = 0;
	بنية ref_حالتاں حالتاں = REF_حالتاں_INIT;
	بنية string_list refs_منزل_prune = STRING_LIST_INIT_NODUP;
	بنية string_list_شے *شے;

	get_remote_ref_حالتاں(remote, &حالتاں, GET_REF_حالتاں);

	إذا (!حالتاں.stale.nr) {
		free_remote_ref_حالتاں(&حالتاں);
		أرجع 0;
	}

	printf_ln(_("Pruning %s"), remote);
	printf_ln(_("URL: %s"), حالتاں.remote->url.v[0]);

	for_each_string_list_شے(شے, &حالتاں.stale)
		string_list_append(&refs_منزل_prune, شے->util);
	string_list_ترتیب_دیو(&refs_منزل_prune);

	إذا (!dry_لگاتار)
		نتیجہ |= refs_delete_refs(get_main_ref_store(the_repository),
					   "remote: prune", &refs_منزل_prune, 0);

	for_each_string_list_شے(شے, &حالتاں.stale) {
		ثابت حرف *refname = شے->util;

		إذا (dry_لگاتار)
			printf_ln(_(" * [would prune] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
		وإلا
			printf_ln(_(" * [pruned] %s"),
			       abbrev_ref(refname, "refs/remotes/"));
	}

	refs_warn_dangling_symrefs(get_main_ref_store(the_repository),
				   stdout, " ", dry_لگاتار, &refs_منزل_prune);

	string_list_clear(&refs_منزل_prune, 0);
	free_remote_ref_حالتاں(&حالتاں);
	أرجع نتیجہ;
}

ساكن صحيح prune(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		 بنية repository *repo بن_ورتیا)
{
	صحيح dry_لگاتار = 0, نتیجہ = 0;
	بنية option options[] = {
		OPT_DRY_لگاتار(&dry_لگاتار, N_("dry run")),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options,
			     builtin_remote_prune_usage, 0);

	إذا (argc < 1)
		usage_with_options(builtin_remote_prune_usage, options);

	لكل (; argc; argc--, argv++)
		نتیجہ |= prune_remote(*argv, dry_لگاتار);

	أرجع نتیجہ;
}

ساكن صحيح get_remote_default(ثابت حرف *کلید, ثابت حرف *قدر بن_ورتیا,
			      ثابت بنية config_context *ctx بن_ورتیا,
			      فراغ *priv)
{
	إذا (strcmp(کلید, "remotes.default") == 0) {
		صحيح *لبھیا_2 = priv;
		*لبھیا_2 = 1;
	}
	أرجع 0;
}

ساكن صحيح update(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		  بنية repository *repo بن_ورتیا)
{
	صحيح i, prune = -1;
	بنية option options[] = {
		OPT_BOOL('p', "prune", &prune,
			 N_("prune remotes after fetching")),
		OPT_سرا()
	};
	بنية بچہ_process cmd = بچہ_PROCESS_INIT;
	صحيح default_defined = 0;

	argc = parse_options(argc, argv, اگیتر, options,
			     builtin_remote_update_usage,
			     PARSE_OPT_KEEP_ARGV0);

	strvec_ڈالو(&cmd.دلیلاں, "fetch");

	إذا (prune != -1)
		strvec_ڈالو(&cmd.دلیلاں, prune ? "--prune" : "--no-prune");
	إذا (verbose)
		strvec_ڈالو(&cmd.دلیلاں, "-v");
	strvec_ڈالو(&cmd.دلیلاں, "--multiple");
	إذا (argc < 2)
		strvec_ڈالو(&cmd.دلیلاں, "default");
	لكل (i = 1; i < argc; i++)
		strvec_ڈالو(&cmd.دلیلاں, argv[i]);

	إذا (strcmp(cmd.دلیلاں.v[cmd.دلیلاں.nr-1], "default") == 0) {
		repo_config(the_repository, get_remote_default, &default_defined);
		إذا (!default_defined) {
			strvec_نکالو(&cmd.دلیلاں);
			strvec_ڈالو(&cmd.دلیلاں, "--all");
		}
	}

	cmd.git_cmd = 1;
	أرجع لگاتار_command(&cmd);
}

ساكن صحيح remove_all_fetch_refspecs(ثابت حرف *کلید)
{
	أرجع repo_config_سیٹ_multivar_gently(the_repository, کلید, عدم, عدم,
					       CONFIG_FLAGS_MULTI_REPLACE);
}

ساكن فراغ جوڑو_branches(بنية remote *remote, ثابت حرف **branches,
			 ثابت حرف *کلید)
{
	ثابت حرف *remotename = remote->name;
	صحيح mirror = remote->mirror;
	بنية strbuf refspec = STRBUF_INIT;

	لكل (; *branches; branches++)
		جوڑو_branch(کلید, *branches, remotename, mirror, &refspec);

	strbuf_release(&refspec);
}

ساكن صحيح سیٹ_remote_branches(ثابت حرف *remotename, ثابت حرف **branches,
				صحيح جوڑو_منوال)
{
	بنية strbuf کلید = STRBUF_INIT;
	بنية remote *remote;

	strbuf_addf(&کلید, "remote.%s.fetch", remotename);

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		باہر_نکلو(2);
	}

	إذا (!جوڑو_منوال && remove_all_fetch_refspecs(کلید.buf)) {
		strbuf_release(&کلید);
		أرجع 1;
	}
	جوڑو_branches(remote, branches, کلید.buf);

	strbuf_release(&کلید);
	أرجع 0;
}

ساكن صحيح سیٹ_branches(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
			بنية repository *repo بن_ورتیا)
{
	صحيح جوڑو_منوال = 0;
	بنية option options[] = {
		OPT_BOOL('\0', "add", &جوڑو_منوال, N_("add branch")),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options,
			     builtin_remote_setbranches_usage, 0);
	إذا (argc == 0) {
		error(_("no remote specified"));
		usage_with_options(builtin_remote_setbranches_usage, options);
	}
	argv[argc] = عدم;

	أرجع سیٹ_remote_branches(argv[0], argv + 1, جوڑو_منوال);
}

ساكن صحيح get_url(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		   بنية repository *repo بن_ورتیا)
{
	صحيح ڈالو_منوال = 0, all_منوال = 0;
	ثابت حرف *remotename = عدم;
	بنية remote *remote;
	بنية strvec *url;
	بنية option options[] = {
		OPT_BOOL('\0', "push", &ڈالو_منوال,
			 N_("query push URLs rather than fetch URLs")),
		OPT_BOOL('\0', "all", &all_منوال,
			 N_("return all URLs")),
		OPT_سرا()
	};
	argc = parse_options(argc, argv, اگیتر, options,
			     builtin_remote_geturl_usage, 0);

	إذا (argc != 1)
		usage_with_options(builtin_remote_geturl_usage, options);

	remotename = argv[0];

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		باہر_نکلو(2);
	}

	url = ڈالو_منوال ? ڈالو_url_دا_remote(remote) : &remote->url;

	إذا (all_منوال) {
		لكل (ناپ_t i = 0; i < url->nr; i++)
			printf_ln("%s", url->v[i]);
	} وإلا {
		printf_ln("%s", url->v[0]);
	}

	أرجع 0;
}

ساكن صحيح سیٹ_url(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر,
		   بنية repository *repo بن_ورتیا)
{
	صحيح ڈالو_منوال = 0, جوڑو_منوال = 0, delete_منوال = 0;
	صحيح matches = 0, negative_matches = 0;
	ثابت حرف *remotename = عدم;
	ثابت حرف *newurl = عدم;
	ثابت حرف *oldurl = عدم;
	بنية remote *remote;
	regex_t old_regex;
	بنية strvec *urlset;
	بنية strbuf name_buf = STRBUF_INIT;
	بنية option options[] = {
		OPT_BOOL('\0', "push", &ڈالو_منوال,
			 N_("manipulate push URLs")),
		OPT_BOOL('\0', "add", &جوڑو_منوال,
			 N_("add URL")),
		OPT_BOOL('\0', "delete", &delete_منوال,
			    N_("delete URLs")),
		OPT_سرا()
	};
	argc = parse_options(argc, argv, اگیتر, options,
			     builtin_remote_seturl_usage,
			     PARSE_OPT_KEEP_ARGV0);

	إذا (جوڑو_منوال && delete_منوال)
		die(_("--add --delete doesn't make sense"));

	إذا (argc < 3 || argc > 4 || ((جوڑو_منوال || delete_منوال) && argc != 3))
		usage_with_options(builtin_remote_seturl_usage, options);

	remotename = argv[1];
	newurl = argv[2];
	إذا (argc > 3)
		oldurl = argv[3];

	إذا (delete_منوال)
		oldurl = newurl;

	remote = remote_get(remotename);
	إذا (!remote_is_configured(remote, 1)) {
		error(_("No such remote '%s'"), remotename);
		باہر_نکلو(2);
	}

	إذا (ڈالو_منوال) {
		strbuf_addf(&name_buf, "remote.%s.pushurl", remotename);
		urlset = &remote->pushurl;
	} وإلا {
		strbuf_addf(&name_buf, "remote.%s.url", remotename);
		urlset = &remote->url;
	}

	/* Special cases that add new entry. */
	إذا ((!oldurl && !delete_منوال) || جوڑو_منوال) {
		إذا (جوڑو_منوال)
			repo_config_سیٹ_multivar(the_repository, name_buf.buf, newurl,
						       "^$", 0);
		وإلا
			repo_config_سیٹ(the_repository, name_buf.buf, newurl);
		اذهب باہرلا;
	}

	/* Old URL specified. Demand that one matches. */
	إذا (regcomp(&old_regex, oldurl, REG_EXTENDED))
		die(_("Invalid old URL pattern: %s"), oldurl);

	لكل (ناپ_t i = 0; i < urlset->nr; i++)
		إذا (!regexec(&old_regex, urlset->v[i], 0, عدم, 0))
			matches++;
		وإلا
			negative_matches++;
	إذا (!delete_منوال && !matches)
		die(_("No such URL found: %s"), oldurl);
	إذا (delete_منوال && !negative_matches && !ڈالو_منوال)
		die(_("Will not delete all non-push URLs"));

	regfree(&old_regex);

	إذا (!delete_منوال)
		repo_config_سیٹ_multivar(the_repository, name_buf.buf, newurl, oldurl, 0);
	وإلا
		repo_config_سیٹ_multivar(the_repository, name_buf.buf, عدم, oldurl,
					 CONFIG_FLAGS_MULTI_REPLACE);
باہرلا:
	strbuf_release(&name_buf);
	أرجع 0;
}

صحيح cmd_remote(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *اگیتر,
	       بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT__VERBOSE(&verbose, N_("be verbose; must be placed before a subcommand")),
		OPT_SUBCOMMAND("add", &fn, جوڑو),
		OPT_SUBCOMMAND("rename", &fn, mv),
		OPT_SUBCOMMAND_F("rm", &fn, rm, عدم, PARSE_OPT_NOCOMPLETE),
		OPT_SUBCOMMAND("remove", &fn, rm),
		OPT_SUBCOMMAND("set-head", &fn, سیٹ_سر),
		OPT_SUBCOMMAND("set-branches", &fn, سیٹ_branches),
		OPT_SUBCOMMAND("get-url", &fn, get_url),
		OPT_SUBCOMMAND("set-url", &fn, سیٹ_url),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("update", &fn, update),
		OPT_سرا()
	};

	argc = parse_options(argc, argv, اگیتر, options, builtin_remote_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	إذا (fn) {
		أرجع !!fn(argc, argv, اگیتر, repo);
	} وإلا {
		إذا (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(builtin_remote_usage, options);
		}
		أرجع !!show_all();
	}
}
