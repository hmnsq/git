/*
 * "git push"
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "advice.h"
#شامل "branch.h"
#شامل "config.h"
#شامل "dir.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "refspec.h"
#شامل "run-command.h"
#شامل "remote.h"
#شامل "transport.h"
#شامل "parse-options.h"
#شامل "pkt-line.h"
#شامل "submodule.h"
#شامل "submodule-config.h"
#شامل "send-pack.h"
#شامل "trace2.h"
#شامل "color.h"

ساکن ثابت حرف * ثابت ڈالو_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	تہی,
};

ساکن شمار git_colorbool ڈالو_use_color = GIT_COLOR_UNKNOWN;
ساکن حرف ڈالو_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

شمار color_ڈالو {
	ڈالو_COLOR_RESET = 0,
	ڈالو_COLOR_ERROR = 1
};

ساکن صحیح parse_ڈالو_color_خانہ(ثابت حرف *خانہ)
{
	اگر (!strcasecmp(خانہ, "reset"))
		واپس ڈالو_COLOR_RESET;
	اگر (!strcasecmp(خانہ, "error"))
		واپس ڈالو_COLOR_ERROR;
	واپس -1;
}

ساکن ثابت حرف *ڈالو_get_color(شمار color_ڈالو ix)
{
	اگر (want_color_stderr(ڈالو_use_color))
		واپس ڈالو_colors[ix];
	واپس "";
}

ساکن صحیح thin = 1;
ساکن صحیح deleterefs;
ساکن ثابت حرف *receivepack;
ساکن صحیح verbosity;
ساکن صحیح پیش_رفت = -1;
ساکن صحیح recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ساکن شمار transport_family family;

ساکن ساخت ڈالو_cas_option cas;

ساکن ساخت refspec rs;

ساکن ساخت string_list ڈالو_options_config = STRING_LIST_INIT_DUP;

ساکن خالی refspec_append_mapped(ساخت refspec *refspec, ثابت حرف *ref,
				  ساخت remote *remote, ساخت ref *matched)
{
	ثابت حرف *branch_name;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	اگر (remote->ڈالو.nr) {
		ساخت refspec_شے query = {
			.src = matched->name,
		};

		اگر (!refspec_ڈھونڈو_مطابقت(&remote->ڈالو, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			آزاد(query.dst);
			واپس;
		}
	}

	اگر (cfg->ڈالو_default == ڈالو_DEFAULT_UPSTREAM &&
	    skip_سابقہ(matched->name, "refs/heads/", &branch_name)) {
		ساخت branch *branch = branch_get(branch_name);
		اگر (branch->ملاؤ_nr == 1 && branch->ملاؤ[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->ملاؤ[0]->src);
			واپس;
		}
	}

	refspec_append(refspec, ref);
}

ساکن خالی سیٹ_refspecs(ثابت حرف **refs, صحیح nr, ساخت remote *remote)
{
	ساخت ref *local_refs = تہی;
	صحیح i;

	برائے (i = 0; i < nr; i++) {
		ثابت حرف *ref = refs[i];
		اگر (!strcmp("tag", ref)) {
			اگر (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			اگر (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			ورنہ
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} ورنہ اگر (deleterefs) {
			اگر (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} ورنہ اگر (!strchr(ref, ':')) {
			ساخت ref *matched = تہی;

			/* lazily grab local_refs */
			اگر (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			اگر (گنتی_refspec_مطابقت(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			ورنہ
				refspec_append_mapped(&rs, ref, remote, matched);
		} ورنہ
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

ساکن NORETURN خالی die_ڈالو_simple(ساخت branch *branch,
				     ساخت remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	ثابت حرف *advice_pushdefault_maybe = "";
	ثابت حرف *advice_automergesimple_maybe = "";
	ثابت حرف *short_upstream = branch->ملاؤ[0]->src;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	skip_سابقہ(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	اگر (cfg->ڈالو_default == ڈالو_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	اگر (cfg->branch_track != BRANCH_TRACK_SIMPLE)
		advice_automergesimple_maybe = _("\n"
				 "To avoid automatically configuring "
				 "an upstream branch when its name\n"
				 "won't match the local branch, see option "
				 "'simple' of branch.autoSetupMerge\n"
				 "in 'git help config'.\n");
	die(_("The upstream branch of your current branch does not match\n"
	      "the name of your current branch.  To push to the upstream branch\n"
	      "on the remote, use\n"
	      "\n"
	      "    git push %s HEAD:%s\n"
	      "\n"
	      "To push to the branch of the same name on the remote, use\n"
	      "\n"
	      "    git push %s HEAD\n"
	      "%s%s"),
	    remote->name, short_upstream,
	    remote->name, advice_pushdefault_maybe,
	    advice_automergesimple_maybe);
}

ساکن ثابت حرف پیغام_detached_سر_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

ساکن ثابت حرف *get_upstream_ref(صحیح flags, ساخت branch *branch, ثابت حرف *remote_name)
{
	اگر (branch->ملاؤ_nr == 0 && (flags & TRANSPORT_ڈالو_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		واپس branch->refname;
	}

	اگر (!branch->ملاؤ_nr || !branch->ملاؤ || !branch->remote_name) {
		ثابت حرف *advice_autosetup_maybe = "";
		اگر (!(flags & TRANSPORT_ڈالو_AUTO_UPSTREAM)) {
			advice_autosetup_maybe = _("\n"
					   "To have this happen automatically for "
					   "branches without a tracking\n"
					   "upstream, see 'push.autoSetupRemote' "
					   "in 'git help config'.\n");
		}
		die(_("The current branch %s has no upstream branch.\n"
		    "To push the current branch and set the remote as upstream, use\n"
		    "\n"
		    "    git push --set-upstream %s %s\n"
		    "%s"),
		    branch->name,
		    remote_name,
		    branch->name,
		    advice_autosetup_maybe);
	}
	اگر (branch->ملاؤ_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	واپس branch->ملاؤ[0]->src;
}

ساکن خالی تیاری_default_ڈالو_refspecs(صحیح *flags, ساخت remote *remote)
{
	ساخت branch *branch;
	ثابت حرف *dst;
	صحیح same_remote;
	ساخت repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	انتخاب (cfg->ڈالو_default) {
	حالت ڈالو_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		واپس;

	حالت ڈالو_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		واپس;
	طےشدہ:
		توڑو;
	}

	branch = branch_get(تہی);
	اگر (!branch)
		die(_(پیغام_detached_سر_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, تہی));

	انتخاب (cfg->ڈالو_default) {
	طےشدہ:
	حالت ڈالو_DEFAULT_UNSPECIFIED:
	حالت ڈالو_DEFAULT_SIMPLE:
		اگر (!same_remote)
			توڑو;
		اگر (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_ڈالو_simple(branch, remote);
		توڑو;

	حالت ڈالو_DEFAULT_UPSTREAM:
		اگر (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		توڑو;

	حالت ڈالو_DEFAULT_CURRENT:
		توڑو;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	اگر ((*flags & TRANSPORT_ڈالو_AUTO_UPSTREAM) && branch->ملاؤ_nr == 0)
		*flags |= TRANSPORT_ڈالو_سیٹ_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

ساکن ثابت حرف پیغام_advice_pull_before_ڈالو[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ساکن ثابت حرف پیغام_advice_checkout_pull_ڈالو[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ساکن ثابت حرف پیغام_advice_ref_fetch_پہلا[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ساکن ثابت حرف پیغام_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

ساکن ثابت حرف پیغام_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

ساکن ثابت حرف پیغام_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ساکن خالی advise_pull_before_ڈالو(خالی)
{
	اگر (!advice_enabled(ADVICE_ڈالو_NON_FF_CURRENT) || !advice_enabled(ADVICE_ڈالو_UPDATE_REJECTED))
		واپس;
	advise(_(پیغام_advice_pull_before_ڈالو));
}

ساکن خالی advise_checkout_pull_ڈالو(خالی)
{
	اگر (!advice_enabled(ADVICE_ڈالو_NON_FF_MATCHING) || !advice_enabled(ADVICE_ڈالو_UPDATE_REJECTED))
		واپس;
	advise(_(پیغام_advice_checkout_pull_ڈالو));
}

ساکن خالی advise_ref_already_exists(خالی)
{
	اگر (!advice_enabled(ADVICE_ڈالو_ALREADY_EXISTS) || !advice_enabled(ADVICE_ڈالو_UPDATE_REJECTED))
		واپس;
	advise(_(پیغام_advice_ref_already_exists));
}

ساکن خالی advise_ref_fetch_پہلا(خالی)
{
	اگر (!advice_enabled(ADVICE_ڈالو_FETCH_پہلا) || !advice_enabled(ADVICE_ڈالو_UPDATE_REJECTED))
		واپس;
	advise(_(پیغام_advice_ref_fetch_پہلا));
}

ساکن خالی advise_ref_needs_force(خالی)
{
	اگر (!advice_enabled(ADVICE_ڈالو_NEEDS_FORCE) || !advice_enabled(ADVICE_ڈالو_UPDATE_REJECTED))
		واپس;
	advise(_(پیغام_advice_ref_needs_force));
}

ساکن خالی advise_ref_needs_update(خالی)
{
	اگر (!advice_enabled(ADVICE_ڈالو_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_ڈالو_UPDATE_REJECTED))
		واپس;
	advise(_(پیغام_advice_ref_needs_update));
}

ساکن صحیح ڈالو_with_options(ساخت transport *transport, ساخت refspec *rs,
			     صحیح flags)
{
	صحیح err;
	غیرعلامتی صحیح reject_reasons;
	حرف *anon_url = transport_anonymize_url(transport->url);

	transport_سیٹ_verbosity(transport, verbosity, پیش_رفت);
	transport->family = family;

	اگر (receivepack)
		transport_سیٹ_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_سیٹ_option(transport, TRANS_OPT_THIN, thin ? "yes" : تہی);

	اگر (!is_خالی_خانے_cas(&cas)) {
		اگر (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	اگر (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_ڈالو(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	اگر (err != 0) {
		fprintf(stderr, "%s", ڈالو_get_color(ڈالو_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", ڈالو_get_color(ڈالو_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	آزاد(anon_url);
	اگر (!err)
		واپس 0;

	اگر (reject_reasons & REJECT_NON_FF_سر) {
		advise_pull_before_ڈالو();
	} ورنہ اگر (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_ڈالو();
	} ورنہ اگر (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} ورنہ اگر (reject_reasons & REJECT_FETCH_پہلا) {
		advise_ref_fetch_پہلا();
	} ورنہ اگر (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} ورنہ اگر (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	واپس 1;
}

ساکن صحیح do_ڈالو(صحیح flags,
		   ثابت ساخت string_list *ڈالو_options,
		   ساخت remote *remote)
{
	صحیح errs;
	ساخت strvec *url;
	ساخت refspec *ڈالو_refspec = &rs;

	اگر (ڈالو_options->nr)
		flags |= TRANSPORT_ڈالو_OPTIONS;

	اگر (!ڈالو_refspec->nr && !(flags & TRANSPORT_ڈالو_ALL)) {
		اگر (remote->ڈالو.nr) {
			ڈالو_refspec = &remote->ڈالو;
		} ورنہ اگر (!(flags & TRANSPORT_ڈالو_MIRROR))
			تیاری_default_ڈالو_refspecs(&flags, remote);
	}
	errs = 0;
	url = ڈالو_url_کا_remote(remote);
	برائے (سائز_t i = 0; i < url->nr; i++) {
		ساخت transport *transport =
			transport_get(remote, url->v[i]);
		اگر (flags & TRANSPORT_ڈالو_OPTIONS)
			transport->ڈالو_options = ڈالو_options;
		اگر (ڈالو_with_options(transport, ڈالو_refspec, flags))
			errs++;
	}
	واپس !!errs;
}

ساکن صحیح option_parse_recurse_submodules(ثابت ساخت option *opt,
				   ثابت حرف *arg, صحیح unset)
{
	صحیح *recurse_submodules = opt->قدر;

	اگر (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	ورنہ {
		اگر (!strcmp(arg, "only-is-on-demand")) {
			اگر (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} ورنہ {
			*recurse_submodules = parse_ڈالو_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	واپس 0;
}

ساکن خالی سیٹ_ڈالو_cert_flags(صحیح *flags, صحیح v)
{
	انتخاب (v) {
	حالت SEND_PACK_ڈالو_CERT_NEVER:
		*flags &= ~(TRANSPORT_ڈالو_CERT_ALWAYS | TRANSPORT_ڈالو_CERT_IF_ASKED);
		توڑو;
	حالت SEND_PACK_ڈالو_CERT_ALWAYS:
		*flags |= TRANSPORT_ڈالو_CERT_ALWAYS;
		*flags &= ~TRANSPORT_ڈالو_CERT_IF_ASKED;
		توڑو;
	حالت SEND_PACK_ڈالو_CERT_IF_ASKED:
		*flags |= TRANSPORT_ڈالو_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_ڈالو_CERT_ALWAYS;
		توڑو;
	}
}


ساکن صحیح git_ڈالو_config(ثابت حرف *k, ثابت حرف *v,
			   ثابت ساخت config_context *ctx, خالی *cb)
{
	ثابت حرف *خانہ_name;
	صحیح *flags = cb;

	اگر (!strcmp(k, "push.followtags")) {
		اگر (git_config_bool(k, v))
			*flags |= TRANSPORT_ڈالو_FOLLOW_لیبل_فہرست;
		ورنہ
			*flags &= ~TRANSPORT_ڈالو_FOLLOW_لیبل_فہرست;
		واپس 0;
	} ورنہ اگر (!strcmp(k, "push.autosetupremote")) {
		اگر (git_config_bool(k, v))
			*flags |= TRANSPORT_ڈالو_AUTO_UPSTREAM;
		واپس 0;
	} ورنہ اگر (!strcmp(k, "push.gpgsign")) {
		انتخاب (git_parse_maybe_bool(v)) {
		حالت 0:
			سیٹ_ڈالو_cert_flags(flags, SEND_PACK_ڈالو_CERT_NEVER);
			توڑو;
		حالت 1:
			سیٹ_ڈالو_cert_flags(flags, SEND_PACK_ڈالو_CERT_ALWAYS);
			توڑو;
		طےشدہ:
			اگر (!strcasecmp(v, "if-asked"))
				سیٹ_ڈالو_cert_flags(flags, SEND_PACK_ڈالو_CERT_IF_ASKED);
			ورنہ
				واپس error(_("invalid value for '%s'"), k);
		}
	} ورنہ اگر (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_ڈالو_recurse_submodules_arg(k, v);
	} ورنہ اگر (!strcmp(k, "submodule.recurse")) {
		صحیح val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} ورنہ اگر (!strcmp(k, "push.pushoption")) {
		واپس parse_transport_option(k, v, &ڈالو_options_config);
	} ورنہ اگر (!strcmp(k, "color.push")) {
		ڈالو_use_color = git_config_colorbool(k, v);
		واپس 0;
	} ورنہ اگر (skip_سابقہ(k, "color.push.", &خانہ_name)) {
		صحیح خانہ = parse_ڈالو_color_خانہ(خانہ_name);
		اگر (خانہ < 0)
			واپس 0;
		اگر (!v)
			واپس config_error_nonbool(k);
		واپس color_parse(v, ڈالو_colors[خانہ]);
	} ورنہ اگر (!strcmp(k, "push.useforceifincludes")) {
		اگر (git_config_bool(k, v))
			*flags |= TRANSPORT_ڈالو_FORCE_IF_INCLUDES;
		ورنہ
			*flags &= ~TRANSPORT_ڈالو_FORCE_IF_INCLUDES;
		واپس 0;
	}

	واپس git_default_config(k, v, ctx, تہی);
}

ساکن صحیح ڈالو_مضرب(ساخت string_list *list,
			 ثابت ساخت string_list *ڈالو_options,
			 صحیح flags,
			 صحیح لیبل_فہرست,
			 ثابت حرف **refspecs,
			 صحیح refspec_nr)
{
	صحیح نتیجہ = 0;
	سائز_t i;
	ساخت strvec argv = STRVEC_INIT;

	strvec_ڈالو(&argv, "push");

	اگر (flags & TRANSPORT_ڈالو_FORCE)
		strvec_ڈالو(&argv, "--force");
	اگر (flags & TRANSPORT_ڈالو_DRY_تسلسل)
		strvec_ڈالو(&argv, "--dry-run");
	اگر (flags & TRANSPORT_ڈالو_PORCELAIN)
		strvec_ڈالو(&argv, "--porcelain");
	اگر (flags & TRANSPORT_ڈالو_PRUNE)
		strvec_ڈالو(&argv, "--prune");
	اگر (flags & TRANSPORT_ڈالو_NO_HOOK)
		strvec_ڈالو(&argv, "--no-verify");
	اگر (flags & TRANSPORT_ڈالو_FOLLOW_لیبل_فہرست)
		strvec_ڈالو(&argv, "--follow-tags");
	اگر (flags & TRANSPORT_ڈالو_سیٹ_UPSTREAM)
		strvec_ڈالو(&argv, "--set-upstream");
	اگر (flags & TRANSPORT_ڈالو_FORCE_IF_INCLUDES)
		strvec_ڈالو(&argv, "--force-if-includes");
	اگر (flags & TRANSPORT_ڈالو_ALL)
		strvec_ڈالو(&argv, "--all");
	اگر (flags & TRANSPORT_ڈالو_MIRROR)
		strvec_ڈالو(&argv, "--mirror");

	اگر (flags & TRANSPORT_ڈالو_CERT_ALWAYS)
		strvec_ڈالو(&argv, "--signed=yes");
	ورنہ اگر (flags & TRANSPORT_ڈالو_CERT_IF_ASKED)
		strvec_ڈالو(&argv, "--signed=if-asked");
	اگر (!thin)
		strvec_ڈالو(&argv, "--no-thin");

	اگر (deleterefs)
		strvec_ڈالو(&argv, "--delete");

	اگر (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	اگر (verbosity >= 2)
		strvec_ڈالو(&argv, "-v");
	اگر (verbosity >= 1)
		strvec_ڈالو(&argv, "-v");
	ورنہ اگر (verbosity < 0)
		strvec_ڈالو(&argv, "-q");
	اگر (پیش_رفت > 0)
		strvec_ڈالو(&argv, "--progress");
	ورنہ اگر (پیش_رفت == 0)
		strvec_ڈالو(&argv, "--no-progress");

	اگر (family == TRANSPORT_FAMILY_IPV4)
		strvec_ڈالو(&argv, "--ipv4");
	ورنہ اگر (family == TRANSPORT_FAMILY_IPV6)
		strvec_ڈالو(&argv, "--ipv6");

	اگر (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_ڈالو(&argv, "--recurse-submodules=check");
	ورنہ اگر (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ڈالو(&argv, "--recurse-submodules=on-demand");
	ورنہ اگر (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_ڈالو(&argv, "--recurse-submodules=only");
	ورنہ اگر (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ڈالو(&argv, "--recurse-submodules=no");


	اگر (لیبل_فہرست)
		strvec_ڈالو(&argv, "--tags");

	برائے (i = 0; i < ڈالو_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     ڈالو_options->اشیاء[i].string);

	برائے (i = 0; i < cas.nr; i++) {
		اگر (cas.اندراج_2[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.اندراج_2[i].refname);
		} ورنہ اگر (!is_null_oid(&cas.اندراج_2[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.اندراج_2[i].refname,
				     oid_مقصد_hex(&cas.اندراج_2[i].expect));
		} ورنہ {
			strvec_ڈالو(&argv, "--force-with-lease");
		}
	}

	برائے (i = 0; i < list->nr; i++) {
		ثابت حرف *name = list->اشیاء[i].string;
		ساخت بچہ_process cmd = بچہ_PROCESS_INIT;
		صحیح j;

		strvec_pushv(&cmd.دلائل, argv.v);
		strvec_ڈالو(&cmd.دلائل, name);

		برائے (j = 0; j < refspec_nr; j++)
			strvec_ڈالو(&cmd.دلائل, refspecs[j]);

		اگر (verbosity >= 0)
			لکھو(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		اگر (تسلسل_command(&cmd)) {
			error(_("could not push to %s"), name);
			نتیجہ = 1;
		}
	}

	strvec_clear(&argv);
	واپس نتیجہ;
}

ساکن خالی die_if_repo_looks_like_ref(ثابت حرف *repo)
{
	ثابت حرف *slash = strchr(repo, '/');
	ساخت strbuf name = STRBUF_INIT;
	صحیح رمز;

	اگر (!slash || !slash[1] || file_exists(repo))
		واپس;

	strbuf_جوڑو(&name, repo, slash - repo);
	اگر (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		واپس;
	}

	رمز = die_پیغام(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_ڈالو_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	باہر_نکلو(رمز);
}

صحیح cmd_ڈالو(صحیح argc,
	     ثابت حرف **argv,
	     ثابت حرف *سابقہ,
	     ساخت repository *repository غیر_مستعمل)
{
	صحیح flags = 0;
	صحیح لیبل_فہرست = 0;
	صحیح ڈالو_cert = -1;
	صحیح rc = 0;
	صحیح بنیاد_flags;
	ثابت حرف *repo = تہی;	/* default repository */
	ساخت string_list ڈالو_options_cmdline = STRING_LIST_INIT_DUP;
	ساخت string_list remote_گروہ = STRING_LIST_INIT_DUP;
	ساخت string_list *ڈالو_options;
	ثابت ساخت string_list_شے *شے;

	ساخت option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_ثنائی_ہندسہ( 0 , "all", &flags, N_("push all branches"), TRANSPORT_ڈالو_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_ثنائی_ہندسہ( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_ڈالو_MIRROR|TRANSPORT_ڈالو_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &لیبل_فہرست, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_ثنائی_ہندسہ('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_ڈالو_DRY_تسلسل),
		OPT_ثنائی_ہندسہ( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_ڈالو_PORCELAIN),
		OPT_ثنائی_ہندسہ('f', "force", &flags, N_("force updates"), TRANSPORT_ڈالو_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_ڈالو_cas_option),
		OPT_ثنائی_ہندسہ(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_ڈالو_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_ثنائی_ہندسہ('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_ڈالو_سیٹ_UPSTREAM),
		OPT_BOOL(0, "progress", &پیش_رفت, N_("force progress reporting")),
		OPT_ثنائی_ہندسہ(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_ڈالو_PRUNE),
		OPT_ثنائی_ہندسہ(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_ڈالو_NO_HOOK),
		OPT_ثنائی_ہندسہ(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_ڈالو_FOLLOW_لیبل_فہرست),
		OPT_CALLBACK_F(0, "signed", &ڈالو_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_ڈالو_signed),
		OPT_ثنائی_ہندسہ(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_ڈالو_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &ڈالو_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_انتہا()
	};

	packet_trace_شناختی_عنصر("push");
	repo_config(the_repository, git_ڈالو_config, &flags);
	argc = parse_options(argc, argv, سابقہ, options, ڈالو_usage, 0);
	ڈالو_options = (ڈالو_options_cmdline.nr
		? &ڈالو_options_cmdline
		: &ڈالو_options_config);
	سیٹ_ڈالو_cert_flags(&flags, ڈالو_cert);

	refspec_init_ڈالو(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  لیبل_فہرست, "--tags",
				  flags & TRANSPORT_ڈالو_ALL, "--all/--branches",
				  flags & TRANSPORT_ڈالو_MIRROR, "--mirror");
	اگر (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	اگر (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	ورنہ اگر (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	ورنہ اگر (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	اگر (argc > 0)
		repo = argv[0];

	اگر (repo) {
		اگر (!جوڑو_remote_or_گروہ(repo, &remote_گروہ)) {
			ساخت remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			اگر (advice_enabled(ADVICE_ڈالو_REPO_LOOKS_LIKE_REF))
				die_if_repo_looks_like_ref(repo);

			/*
			 * Not a configured remote name or group name.
			 * Try treating it as a direct URL or path, e.g.
			 *   git push /tmp/foo.git
			 *   git push https://github.com/user/repo.git
			 * pushremote_get() creates an anonymous remote
			 * from the URL so the loop below can handle it
			 * identically to a named remote.
			 */
			r = pushremote_get(repo);
			اگر (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_گروہ, r->name);
		}
	} ورنہ {
		ساخت remote *r = pushremote_get(تہی);
		اگر (!r)
			die(_("No configured push destination.\n"
			    "Either specify the URL from the command-line or configure a remote repository using\n"
			    "\n"
			    "    git remote add <name> <url>\n"
			    "\n"
			    "and then push using the remote name\n"
			    "\n"
			    "    git push <name>\n"
			    "\n"
			    "To push to multiple remotes at once, configure a remote group using\n"
			    "\n"
			    "    git config remotes.<groupname> \"<remote1> <remote2>\"\n"
			    "\n"
			    "and then push using the group name\n"
			    "\n"
			    "    git push <groupname>\n"));
		string_list_append(&remote_گروہ, r->name);
	}

	اگر (!is_خالی_خانے_cas(&cas) && (flags & TRANSPORT_ڈالو_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_شے(شے, ڈالو_options)
		اگر (strchr(شے->string, '\n'))
			die(_("push options must not have new line characters"));

	اگر (remote_گروہ.nr == 1) {
		/*
		 * Single remote (the common case): run do_push() directly
		 * in this process.  The loop runs exactly once.
		 *
		 * Mirror detection and the --mirror/--all + refspec conflict
		 * checks are done here.  rs is rebuilt so that per-remote push
		 * mappings (remote.NAME.push config) are resolved against the
		 * correct remote.  inner_flags is a snapshot of flags so that a
		 * mirror remote cannot bleed TRANSPORT_PUSH_FORCE into any
		 * subsequent call.
		 */
		بنیاد_flags = flags;
		{
			صحیح inner_flags = بنیاد_flags;
			ساخت remote *r = pushremote_get(remote_گروہ.اشیاء[0].string);
			اگر (!r)
				die(_("no such remote or remote group: %s"),
				    remote_گروہ.اشیاء[0].string);

			اگر (r->mirror)
				inner_flags |= (TRANSPORT_ڈالو_MIRROR|TRANSPORT_ڈالو_FORCE);

			اگر (inner_flags & TRANSPORT_ڈالو_ALL) {
				اگر (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			اگر (inner_flags & TRANSPORT_ڈالو_MIRROR) {
				اگر (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (ساخت refspec) REFSPEC_INIT_ڈالو(the_hash_algo);

			اگر (لیبل_فہرست)
				refspec_append(&rs, "refs/tags/*");
			اگر (argc > 0)
				سیٹ_refspecs(argv + 1, argc - 1, r);

			rc = do_ڈالو(inner_flags, ڈالو_options, r);
		}
	} ورنہ {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		اگر (flags & TRANSPORT_ڈالو_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = ڈالو_مضرب(&remote_گروہ, ڈالو_options, flags,
				   لیبل_فہرست,
				   argc > 1 ? argv + 1 : تہی,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&ڈالو_options_cmdline, 0);
	string_list_clear(&ڈالو_options_config, 0);
	string_list_clear(&remote_گروہ, 0);
	clear_cas_option(&cas);

	اگر (rc == -1)
		usage_with_options(ڈالو_usage, options);
	ورنہ
		واپس rc;
}
