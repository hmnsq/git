/*
 * "git push"
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "branch.h"
#ضمن "config.h"
#ضمن "dir.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "refspec.h"
#ضمن "run-command.h"
#ضمن "remote.h"
#ضمن "transport.h"
#ضمن "parse-options.h"
#ضمن "pkt-line.h"
#ضمن "submodule.h"
#ضمن "submodule-config.h"
#ضمن "send-pack.h"
#ضمن "trace2.h"
#ضمن "color.h"

ساكن ثابت حرف * ثابت ڈالو_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	عدم,
};

ساكن تعداد git_colorbool ڈالو_use_color = GIT_COLOR_UNKNOWN;
ساكن حرف ڈالو_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

تعداد color_ڈالو {
	ڈالو_COLOR_RESET = 0,
	ڈالو_COLOR_ERROR = 1
};

ساكن صحيح parse_ڈالو_color_تھاں(ثابت حرف *تھاں)
{
	إذا (!strcasecmp(تھاں, "reset"))
		أرجع ڈالو_COLOR_RESET;
	إذا (!strcasecmp(تھاں, "error"))
		أرجع ڈالو_COLOR_ERROR;
	أرجع -1;
}

ساكن ثابت حرف *ڈالو_get_color(تعداد color_ڈالو ix)
{
	إذا (want_color_stderr(ڈالو_use_color))
		أرجع ڈالو_colors[ix];
	أرجع "";
}

ساكن صحيح thin = 1;
ساكن صحيح deleterefs;
ساكن ثابت حرف *receivepack;
ساكن صحيح verbosity;
ساكن صحيح ترقی = -1;
ساكن صحيح recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ساكن تعداد transport_family family;

ساكن بنية ڈالو_cas_option cas;

ساكن بنية refspec rs;

ساكن بنية string_list ڈالو_options_config = STRING_LIST_INIT_DUP;

ساكن فراغ refspec_append_mapped(بنية refspec *refspec, ثابت حرف *ref,
				  بنية remote *remote, بنية ref *matched)
{
	ثابت حرف *branch_name;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	إذا (remote->ڈالو.nr) {
		بنية refspec_شے query = {
			.src = matched->name,
		};

		إذا (!refspec_لبھو_میل(&remote->ڈالو, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			حرر(query.dst);
			أرجع;
		}
	}

	إذا (cfg->ڈالو_default == ڈالو_DEFAULT_UPSTREAM &&
	    skip_اگیتر(matched->name, "refs/heads/", &branch_name)) {
		بنية branch *branch = branch_get(branch_name);
		إذا (branch->ملاؤ_nr == 1 && branch->ملاؤ[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->ملاؤ[0]->src);
			أرجع;
		}
	}

	refspec_append(refspec, ref);
}

ساكن فراغ سیٹ_refspecs(ثابت حرف **refs, صحيح nr, بنية remote *remote)
{
	بنية ref *local_refs = عدم;
	صحيح i;

	لكل (i = 0; i < nr; i++) {
		ثابت حرف *ref = refs[i];
		إذا (!strcmp("tag", ref)) {
			إذا (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			إذا (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			وإلا
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} وإلا إذا (deleterefs) {
			إذا (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} وإلا إذا (!strchr(ref, ':')) {
			بنية ref *matched = عدم;

			/* lazily grab local_refs */
			إذا (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			إذا (گنتی_refspec_میل(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			وإلا
				refspec_append_mapped(&rs, ref, remote, matched);
		} وإلا
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

ساكن NORETURN فراغ die_ڈالو_simple(بنية branch *branch,
				     بنية remote *remote)
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
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	skip_اگیتر(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	إذا (cfg->ڈالو_default == ڈالو_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	إذا (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

ساكن ثابت حرف سنیہا_detached_سر_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

ساكن ثابت حرف *get_upstream_ref(صحيح flags, بنية branch *branch, ثابت حرف *remote_name)
{
	إذا (branch->ملاؤ_nr == 0 && (flags & TRANSPORT_ڈالو_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		أرجع branch->refname;
	}

	إذا (!branch->ملاؤ_nr || !branch->ملاؤ || !branch->remote_name) {
		ثابت حرف *advice_autosetup_maybe = "";
		إذا (!(flags & TRANSPORT_ڈالو_AUTO_UPSTREAM)) {
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
	إذا (branch->ملاؤ_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	أرجع branch->ملاؤ[0]->src;
}

ساكن فراغ تیاری_default_ڈالو_refspecs(صحيح *flags, بنية remote *remote)
{
	بنية branch *branch;
	ثابت حرف *dst;
	صحيح same_remote;
	بنية repo_config_اقدار *cfg = repo_config_اقدار(the_repository);

	بدل (cfg->ڈالو_default) {
	حالة ڈالو_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		أرجع;

	حالة ڈالو_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		أرجع;
	افتراضي:
		اكسر;
	}

	branch = branch_get(عدم);
	إذا (!branch)
		die(_(سنیہا_detached_سر_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, عدم));

	بدل (cfg->ڈالو_default) {
	افتراضي:
	حالة ڈالو_DEFAULT_UNSPECIFIED:
	حالة ڈالو_DEFAULT_SIMPLE:
		إذا (!same_remote)
			اكسر;
		إذا (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_ڈالو_simple(branch, remote);
		اكسر;

	حالة ڈالو_DEFAULT_UPSTREAM:
		إذا (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		اكسر;

	حالة ڈالو_DEFAULT_CURRENT:
		اكسر;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	إذا ((*flags & TRANSPORT_ڈالو_AUTO_UPSTREAM) && branch->ملاؤ_nr == 0)
		*flags |= TRANSPORT_ڈالو_سیٹ_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

ساكن ثابت حرف سنیہا_advice_pull_before_ڈالو[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ساكن ثابت حرف سنیہا_advice_checkout_pull_ڈالو[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ساكن ثابت حرف سنیہا_advice_ref_fetch_پہلا[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ساكن ثابت حرف سنیہا_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

ساكن ثابت حرف سنیہا_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

ساكن ثابت حرف سنیہا_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ساكن فراغ advise_pull_before_ڈالو(فراغ)
{
	إذا (!advice_enabled(ADVICE_ڈالو_NON_FF_CURRENT) || !advice_enabled(ADVICE_ڈالو_UPDATE_REJECTED))
		أرجع;
	advise(_(سنیہا_advice_pull_before_ڈالو));
}

ساكن فراغ advise_checkout_pull_ڈالو(فراغ)
{
	إذا (!advice_enabled(ADVICE_ڈالو_NON_FF_MATCHING) || !advice_enabled(ADVICE_ڈالو_UPDATE_REJECTED))
		أرجع;
	advise(_(سنیہا_advice_checkout_pull_ڈالو));
}

ساكن فراغ advise_ref_already_exists(فراغ)
{
	إذا (!advice_enabled(ADVICE_ڈالو_ALREADY_EXISTS) || !advice_enabled(ADVICE_ڈالو_UPDATE_REJECTED))
		أرجع;
	advise(_(سنیہا_advice_ref_already_exists));
}

ساكن فراغ advise_ref_fetch_پہلا(فراغ)
{
	إذا (!advice_enabled(ADVICE_ڈالو_FETCH_پہلا) || !advice_enabled(ADVICE_ڈالو_UPDATE_REJECTED))
		أرجع;
	advise(_(سنیہا_advice_ref_fetch_پہلا));
}

ساكن فراغ advise_ref_needs_force(فراغ)
{
	إذا (!advice_enabled(ADVICE_ڈالو_NEEDS_FORCE) || !advice_enabled(ADVICE_ڈالو_UPDATE_REJECTED))
		أرجع;
	advise(_(سنیہا_advice_ref_needs_force));
}

ساكن فراغ advise_ref_needs_update(فراغ)
{
	إذا (!advice_enabled(ADVICE_ڈالو_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_ڈالو_UPDATE_REJECTED))
		أرجع;
	advise(_(سنیہا_advice_ref_needs_update));
}

ساكن صحيح ڈالو_with_options(بنية transport *transport, بنية refspec *rs,
			     صحيح flags)
{
	صحيح err;
	بنانشانی صحيح reject_reasons;
	حرف *anon_url = transport_anonymize_url(transport->url);

	transport_سیٹ_verbosity(transport, verbosity, ترقی);
	transport->family = family;

	إذا (receivepack)
		transport_سیٹ_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_سیٹ_option(transport, TRANS_OPT_THIN, thin ? "yes" : عدم);

	إذا (!is_خالی_تھاواں_cas(&cas)) {
		إذا (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	إذا (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_ڈالو(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	إذا (err != 0) {
		fprintf(stderr, "%s", ڈالو_get_color(ڈالو_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", ڈالو_get_color(ڈالو_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	حرر(anon_url);
	إذا (!err)
		أرجع 0;

	إذا (reject_reasons & REJECT_NON_FF_سر) {
		advise_pull_before_ڈالو();
	} وإلا إذا (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_ڈالو();
	} وإلا إذا (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} وإلا إذا (reject_reasons & REJECT_FETCH_پہلا) {
		advise_ref_fetch_پہلا();
	} وإلا إذا (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} وإلا إذا (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	أرجع 1;
}

ساكن صحيح do_ڈالو(صحيح flags,
		   ثابت بنية string_list *ڈالو_options,
		   بنية remote *remote)
{
	صحيح errs;
	بنية strvec *url;
	بنية refspec *ڈالو_refspec = &rs;

	إذا (ڈالو_options->nr)
		flags |= TRANSPORT_ڈالو_OPTIONS;

	إذا (!ڈالو_refspec->nr && !(flags & TRANSPORT_ڈالو_ALL)) {
		إذا (remote->ڈالو.nr) {
			ڈالو_refspec = &remote->ڈالو;
		} وإلا إذا (!(flags & TRANSPORT_ڈالو_MIRROR))
			تیاری_default_ڈالو_refspecs(&flags, remote);
	}
	errs = 0;
	url = ڈالو_url_دا_remote(remote);
	لكل (ناپ_t i = 0; i < url->nr; i++) {
		بنية transport *transport =
			transport_get(remote, url->v[i]);
		إذا (flags & TRANSPORT_ڈالو_OPTIONS)
			transport->ڈالو_options = ڈالو_options;
		إذا (ڈالو_with_options(transport, ڈالو_refspec, flags))
			errs++;
	}
	أرجع !!errs;
}

ساكن صحيح option_parse_recurse_submodules(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	صحيح *recurse_submodules = opt->قدر;

	إذا (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	وإلا {
		إذا (!strcmp(arg, "only-is-on-demand")) {
			إذا (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} وإلا {
			*recurse_submodules = parse_ڈالو_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	أرجع 0;
}

ساكن فراغ سیٹ_ڈالو_cert_flags(صحيح *flags, صحيح v)
{
	بدل (v) {
	حالة SEND_PACK_ڈالو_CERT_NEVER:
		*flags &= ~(TRANSPORT_ڈالو_CERT_ALWAYS | TRANSPORT_ڈالو_CERT_IF_ASKED);
		اكسر;
	حالة SEND_PACK_ڈالو_CERT_ALWAYS:
		*flags |= TRANSPORT_ڈالو_CERT_ALWAYS;
		*flags &= ~TRANSPORT_ڈالو_CERT_IF_ASKED;
		اكسر;
	حالة SEND_PACK_ڈالو_CERT_IF_ASKED:
		*flags |= TRANSPORT_ڈالو_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_ڈالو_CERT_ALWAYS;
		اكسر;
	}
}


ساكن صحيح git_ڈالو_config(ثابت حرف *k, ثابت حرف *v,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت حرف *تھاں_name;
	صحيح *flags = cb;

	إذا (!strcmp(k, "push.followtags")) {
		إذا (git_config_bool(k, v))
			*flags |= TRANSPORT_ڈالو_FOLLOW_ٹیگ_سوچی;
		وإلا
			*flags &= ~TRANSPORT_ڈالو_FOLLOW_ٹیگ_سوچی;
		أرجع 0;
	} وإلا إذا (!strcmp(k, "push.autosetupremote")) {
		إذا (git_config_bool(k, v))
			*flags |= TRANSPORT_ڈالو_AUTO_UPSTREAM;
		أرجع 0;
	} وإلا إذا (!strcmp(k, "push.gpgsign")) {
		بدل (git_parse_maybe_bool(v)) {
		حالة 0:
			سیٹ_ڈالو_cert_flags(flags, SEND_PACK_ڈالو_CERT_NEVER);
			اكسر;
		حالة 1:
			سیٹ_ڈالو_cert_flags(flags, SEND_PACK_ڈالو_CERT_ALWAYS);
			اكسر;
		افتراضي:
			إذا (!strcasecmp(v, "if-asked"))
				سیٹ_ڈالو_cert_flags(flags, SEND_PACK_ڈالو_CERT_IF_ASKED);
			وإلا
				أرجع error(_("invalid value for '%s'"), k);
		}
	} وإلا إذا (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_ڈالو_recurse_submodules_arg(k, v);
	} وإلا إذا (!strcmp(k, "submodule.recurse")) {
		صحيح val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} وإلا إذا (!strcmp(k, "push.pushoption")) {
		أرجع parse_transport_option(k, v, &ڈالو_options_config);
	} وإلا إذا (!strcmp(k, "color.push")) {
		ڈالو_use_color = git_config_colorbool(k, v);
		أرجع 0;
	} وإلا إذا (skip_اگیتر(k, "color.push.", &تھاں_name)) {
		صحيح تھاں = parse_ڈالو_color_تھاں(تھاں_name);
		إذا (تھاں < 0)
			أرجع 0;
		إذا (!v)
			أرجع config_error_nonbool(k);
		أرجع color_parse(v, ڈالو_colors[تھاں]);
	} وإلا إذا (!strcmp(k, "push.useforceifincludes")) {
		إذا (git_config_bool(k, v))
			*flags |= TRANSPORT_ڈالو_FORCE_IF_INCLUDES;
		وإلا
			*flags &= ~TRANSPORT_ڈالو_FORCE_IF_INCLUDES;
		أرجع 0;
	}

	أرجع git_default_config(k, v, ctx, عدم);
}

ساكن صحيح ڈالو_مضرب(بنية string_list *list,
			 ثابت بنية string_list *ڈالو_options,
			 صحيح flags,
			 صحيح ٹیگ_سوچی,
			 ثابت حرف **refspecs,
			 صحيح refspec_nr)
{
	صحيح نتیجہ = 0;
	ناپ_t i;
	بنية strvec argv = STRVEC_INIT;

	strvec_ڈالو(&argv, "push");

	إذا (flags & TRANSPORT_ڈالو_FORCE)
		strvec_ڈالو(&argv, "--force");
	إذا (flags & TRANSPORT_ڈالو_DRY_لگاتار)
		strvec_ڈالو(&argv, "--dry-run");
	إذا (flags & TRANSPORT_ڈالو_PORCELAIN)
		strvec_ڈالو(&argv, "--porcelain");
	إذا (flags & TRANSPORT_ڈالو_PRUNE)
		strvec_ڈالو(&argv, "--prune");
	إذا (flags & TRANSPORT_ڈالو_NO_HOOK)
		strvec_ڈالو(&argv, "--no-verify");
	إذا (flags & TRANSPORT_ڈالو_FOLLOW_ٹیگ_سوچی)
		strvec_ڈالو(&argv, "--follow-tags");
	إذا (flags & TRANSPORT_ڈالو_سیٹ_UPSTREAM)
		strvec_ڈالو(&argv, "--set-upstream");
	إذا (flags & TRANSPORT_ڈالو_FORCE_IF_INCLUDES)
		strvec_ڈالو(&argv, "--force-if-includes");
	إذا (flags & TRANSPORT_ڈالو_ALL)
		strvec_ڈالو(&argv, "--all");
	إذا (flags & TRANSPORT_ڈالو_MIRROR)
		strvec_ڈالو(&argv, "--mirror");

	إذا (flags & TRANSPORT_ڈالو_CERT_ALWAYS)
		strvec_ڈالو(&argv, "--signed=yes");
	وإلا إذا (flags & TRANSPORT_ڈالو_CERT_IF_ASKED)
		strvec_ڈالو(&argv, "--signed=if-asked");
	إذا (!thin)
		strvec_ڈالو(&argv, "--no-thin");

	إذا (deleterefs)
		strvec_ڈالو(&argv, "--delete");

	إذا (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	إذا (verbosity >= 2)
		strvec_ڈالو(&argv, "-v");
	إذا (verbosity >= 1)
		strvec_ڈالو(&argv, "-v");
	وإلا إذا (verbosity < 0)
		strvec_ڈالو(&argv, "-q");
	إذا (ترقی > 0)
		strvec_ڈالو(&argv, "--progress");
	وإلا إذا (ترقی == 0)
		strvec_ڈالو(&argv, "--no-progress");

	إذا (family == TRANSPORT_FAMILY_IPV4)
		strvec_ڈالو(&argv, "--ipv4");
	وإلا إذا (family == TRANSPORT_FAMILY_IPV6)
		strvec_ڈالو(&argv, "--ipv6");

	إذا (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_ڈالو(&argv, "--recurse-submodules=check");
	وإلا إذا (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ڈالو(&argv, "--recurse-submodules=on-demand");
	وإلا إذا (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_ڈالو(&argv, "--recurse-submodules=only");
	وإلا إذا (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ڈالو(&argv, "--recurse-submodules=no");


	إذا (ٹیگ_سوچی)
		strvec_ڈالو(&argv, "--tags");

	لكل (i = 0; i < ڈالو_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     ڈالو_options->اشیاء[i].string);

	لكل (i = 0; i < cas.nr; i++) {
		إذا (cas.اندراج[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.اندراج[i].refname);
		} وإلا إذا (!is_null_oid(&cas.اندراج[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.اندراج[i].refname,
				     oid_منزل_hex(&cas.اندراج[i].expect));
		} وإلا {
			strvec_ڈالو(&argv, "--force-with-lease");
		}
	}

	لكل (i = 0; i < list->nr; i++) {
		ثابت حرف *name = list->اشیاء[i].string;
		بنية بچہ_process cmd = بچہ_PROCESS_INIT;
		صحيح j;

		strvec_pushv(&cmd.دلیلاں, argv.v);
		strvec_ڈالو(&cmd.دلیلاں, name);

		لكل (j = 0; j < refspec_nr; j++)
			strvec_ڈالو(&cmd.دلیلاں, refspecs[j]);

		إذا (verbosity >= 0)
			لکھو(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		إذا (لگاتار_command(&cmd)) {
			error(_("could not push to %s"), name);
			نتیجہ = 1;
		}
	}

	strvec_clear(&argv);
	أرجع نتیجہ;
}

ساكن فراغ die_if_repo_looks_like_ref(ثابت حرف *repo)
{
	ثابت حرف *slash = strchr(repo, '/');
	بنية strbuf name = STRBUF_INIT;
	صحيح کوڈ;

	إذا (!slash || !slash[1] || file_exists(repo))
		أرجع;

	strbuf_جوڑو(&name, repo, slash - repo);
	إذا (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		أرجع;
	}

	کوڈ = die_سنیہا(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_ڈالو_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	باہر_نکلو(کوڈ);
}

صحيح cmd_ڈالو(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *اگیتر,
	     بنية repository *repository بن_ورتیا)
{
	صحيح flags = 0;
	صحيح ٹیگ_سوچی = 0;
	صحيح ڈالو_cert = -1;
	صحيح rc = 0;
	صحيح ادھار_flags;
	ثابت حرف *repo = عدم;	/* default repository */
	بنية string_list ڈالو_options_cmdline = STRING_LIST_INIT_DUP;
	بنية string_list remote_گروہ = STRING_LIST_INIT_DUP;
	بنية string_list *ڈالو_options;
	ثابت بنية string_list_شے *شے;

	بنية option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_ثنائی_ہندسہ( 0 , "all", &flags, N_("push all branches"), TRANSPORT_ڈالو_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_ثنائی_ہندسہ( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_ڈالو_MIRROR|TRANSPORT_ڈالو_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &ٹیگ_سوچی, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_ثنائی_ہندسہ('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_ڈالو_DRY_لگاتار),
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
		OPT_BOOL(0, "progress", &ترقی, N_("force progress reporting")),
		OPT_ثنائی_ہندسہ(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_ڈالو_PRUNE),
		OPT_ثنائی_ہندسہ(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_ڈالو_NO_HOOK),
		OPT_ثنائی_ہندسہ(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_ڈالو_FOLLOW_ٹیگ_سوچی),
		OPT_CALLBACK_F(0, "signed", &ڈالو_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_ڈالو_signed),
		OPT_ثنائی_ہندسہ(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_ڈالو_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &ڈالو_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_سرا()
	};

	packet_trace_پچھان_تت("push");
	repo_config(the_repository, git_ڈالو_config, &flags);
	argc = parse_options(argc, argv, اگیتر, options, ڈالو_usage, 0);
	ڈالو_options = (ڈالو_options_cmdline.nr
		? &ڈالو_options_cmdline
		: &ڈالو_options_config);
	سیٹ_ڈالو_cert_flags(&flags, ڈالو_cert);

	refspec_init_ڈالو(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  ٹیگ_سوچی, "--tags",
				  flags & TRANSPORT_ڈالو_ALL, "--all/--branches",
				  flags & TRANSPORT_ڈالو_MIRROR, "--mirror");
	إذا (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	إذا (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	وإلا إذا (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	وإلا إذا (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	إذا (argc > 0)
		repo = argv[0];

	إذا (repo) {
		إذا (!جوڑو_remote_or_گروہ(repo, &remote_گروہ)) {
			بنية remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			إذا (advice_enabled(ADVICE_ڈالو_REPO_LOOKS_LIKE_REF))
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
			إذا (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_گروہ, r->name);
		}
	} وإلا {
		بنية remote *r = pushremote_get(عدم);
		إذا (!r)
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

	إذا (!is_خالی_تھاواں_cas(&cas) && (flags & TRANSPORT_ڈالو_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_شے(شے, ڈالو_options)
		إذا (strchr(شے->string, '\n'))
			die(_("push options must not have new line characters"));

	إذا (remote_گروہ.nr == 1) {
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
		ادھار_flags = flags;
		{
			صحيح inner_flags = ادھار_flags;
			بنية remote *r = pushremote_get(remote_گروہ.اشیاء[0].string);
			إذا (!r)
				die(_("no such remote or remote group: %s"),
				    remote_گروہ.اشیاء[0].string);

			إذا (r->mirror)
				inner_flags |= (TRANSPORT_ڈالو_MIRROR|TRANSPORT_ڈالو_FORCE);

			إذا (inner_flags & TRANSPORT_ڈالو_ALL) {
				إذا (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			إذا (inner_flags & TRANSPORT_ڈالو_MIRROR) {
				إذا (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (بنية refspec) REFSPEC_INIT_ڈالو(the_hash_algo);

			إذا (ٹیگ_سوچی)
				refspec_append(&rs, "refs/tags/*");
			إذا (argc > 0)
				سیٹ_refspecs(argv + 1, argc - 1, r);

			rc = do_ڈالو(inner_flags, ڈالو_options, r);
		}
	} وإلا {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		إذا (flags & TRANSPORT_ڈالو_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = ڈالو_مضرب(&remote_گروہ, ڈالو_options, flags,
				   ٹیگ_سوچی,
				   argc > 1 ? argv + 1 : عدم,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&ڈالو_options_cmdline, 0);
	string_list_clear(&ڈالو_options_config, 0);
	string_list_clear(&remote_گروہ, 0);
	clear_cas_option(&cas);

	إذا (rc == -1)
		usage_with_options(ڈالو_usage, options);
	وإلا
		أرجع rc;
}
