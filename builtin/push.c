/*
 * "git push"
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "transport.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pkt-line.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule-config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "send-pack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trace2.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "color.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ ਧੱਕੋ_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	ਨਿਰਾ,
};

ਸਥਿਰ ਗਿਣਤੀ git_colorbool ਧੱਕੋ_use_color = GIT_COLOR_UNKNOWN;
ਸਥਿਰ ਅੱਖਰ ਧੱਕੋ_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

ਗਿਣਤੀ color_ਧੱਕੋ {
	ਧੱਕੋ_COLOR_RESET = 0,
	ਧੱਕੋ_COLOR_ERROR = 1
};

ਸਥਿਰ ਪੂਰਨ parse_ਧੱਕੋ_color_ਥਾਂ(ਅਚਲ ਅੱਖਰ *ਥਾਂ)
{
	ਜੇ (!strcasecmp(ਥਾਂ, "reset"))
		ਵਾਪਸ ਧੱਕੋ_COLOR_RESET;
	ਜੇ (!strcasecmp(ਥਾਂ, "error"))
		ਵਾਪਸ ਧੱਕੋ_COLOR_ERROR;
	ਵਾਪਸ -1;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਧੱਕੋ_get_color(ਗਿਣਤੀ color_ਧੱਕੋ ix)
{
	ਜੇ (want_color_stderr(ਧੱਕੋ_use_color))
		ਵਾਪਸ ਧੱਕੋ_colors[ix];
	ਵਾਪਸ "";
}

ਸਥਿਰ ਪੂਰਨ thin = 1;
ਸਥਿਰ ਪੂਰਨ deleterefs;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *receivepack;
ਸਥਿਰ ਪੂਰਨ verbosity;
ਸਥਿਰ ਪੂਰਨ ਤਰੱਕੀ = -1;
ਸਥਿਰ ਪੂਰਨ recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ਸਥਿਰ ਗਿਣਤੀ transport_family family;

ਸਥਿਰ ਬਣਤਰ ਧੱਕੋ_cas_option cas;

ਸਥਿਰ ਬਣਤਰ refspec rs;

ਸਥਿਰ ਬਣਤਰ string_list ਧੱਕੋ_options_config = STRING_LIST_INIT_DUP;

ਸਥਿਰ ਖਾਲੀ refspec_append_mapped(ਬਣਤਰ refspec *refspec, ਅਚਲ ਅੱਖਰ *ref,
				  ਬਣਤਰ remote *remote, ਬਣਤਰ ref *matched)
{
	ਅਚਲ ਅੱਖਰ *branch_name;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	ਜੇ (remote->ਧੱਕੋ.nr) {
		ਬਣਤਰ refspec_ਵਸਤੂ query = {
			.src = matched->name,
		};

		ਜੇ (!refspec_ਲੱਭੋ_ਮੇਲ(&remote->ਧੱਕੋ, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			ਮੁਕਤ(query.dst);
			ਵਾਪਸ;
		}
	}

	ਜੇ (cfg->ਧੱਕੋ_default == ਧੱਕੋ_DEFAULT_UPSTREAM &&
	    skip_ਅਗੇਤਰ(matched->name, "refs/heads/", &branch_name)) {
		ਬਣਤਰ branch *branch = branch_get(branch_name);
		ਜੇ (branch->ਮਿਲਾਓ_nr == 1 && branch->ਮਿਲਾਓ[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->ਮਿਲਾਓ[0]->src);
			ਵਾਪਸ;
		}
	}

	refspec_append(refspec, ref);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_refspecs(ਅਚਲ ਅੱਖਰ **refs, ਪੂਰਨ nr, ਬਣਤਰ remote *remote)
{
	ਬਣਤਰ ref *local_refs = ਨਿਰਾ;
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < nr; i++) {
		ਅਚਲ ਅੱਖਰ *ref = refs[i];
		ਜੇ (!strcmp("tag", ref)) {
			ਜੇ (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			ਜੇ (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			ਨਹੀਂਤਾਂ
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} ਨਹੀਂਤਾਂ ਜੇ (deleterefs) {
			ਜੇ (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} ਨਹੀਂਤਾਂ ਜੇ (!strchr(ref, ':')) {
			ਬਣਤਰ ref *matched = ਨਿਰਾ;

			/* lazily grab local_refs */
			ਜੇ (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			ਜੇ (ਗਿਣਤੀ_refspec_ਮੇਲ(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			ਨਹੀਂਤਾਂ
				refspec_append_mapped(&rs, ref, remote, matched);
		} ਨਹੀਂਤਾਂ
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

ਸਥਿਰ NORETURN ਖਾਲੀ die_ਧੱਕੋ_simple(ਬਣਤਰ branch *branch,
				     ਬਣਤਰ remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	ਅਚਲ ਅੱਖਰ *advice_pushdefault_maybe = "";
	ਅਚਲ ਅੱਖਰ *advice_automergesimple_maybe = "";
	ਅਚਲ ਅੱਖਰ *short_upstream = branch->ਮਿਲਾਓ[0]->src;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	skip_ਅਗੇਤਰ(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	ਜੇ (cfg->ਧੱਕੋ_default == ਧੱਕੋ_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	ਜੇ (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਸੁਨੇਹਾ_detached_ਸਿਰ_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_upstream_ref(ਪੂਰਨ flags, ਬਣਤਰ branch *branch, ਅਚਲ ਅੱਖਰ *remote_name)
{
	ਜੇ (branch->ਮਿਲਾਓ_nr == 0 && (flags & TRANSPORT_ਧੱਕੋ_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		ਵਾਪਸ branch->refname;
	}

	ਜੇ (!branch->ਮਿਲਾਓ_nr || !branch->ਮਿਲਾਓ || !branch->remote_name) {
		ਅਚਲ ਅੱਖਰ *advice_autosetup_maybe = "";
		ਜੇ (!(flags & TRANSPORT_ਧੱਕੋ_AUTO_UPSTREAM)) {
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
	ਜੇ (branch->ਮਿਲਾਓ_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	ਵਾਪਸ branch->ਮਿਲਾਓ[0]->src;
}

ਸਥਿਰ ਖਾਲੀ ਤਿਆਰੀ_default_ਧੱਕੋ_refspecs(ਪੂਰਨ *flags, ਬਣਤਰ remote *remote)
{
	ਬਣਤਰ branch *branch;
	ਅਚਲ ਅੱਖਰ *dst;
	ਪੂਰਨ same_remote;
	ਬਣਤਰ repo_config_ਮੁੱਲਾਂ *cfg = repo_config_ਮੁੱਲਾਂ(the_repository);

	ਚੋਣ (cfg->ਧੱਕੋ_default) {
	ਹਾਲਤ ਧੱਕੋ_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		ਵਾਪਸ;

	ਹਾਲਤ ਧੱਕੋ_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		ਵਾਪਸ;
	ਮੂਲ:
		ਰੋਕੋ;
	}

	branch = branch_get(ਨਿਰਾ);
	ਜੇ (!branch)
		die(_(ਸੁਨੇਹਾ_detached_ਸਿਰ_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, ਨਿਰਾ));

	ਚੋਣ (cfg->ਧੱਕੋ_default) {
	ਮੂਲ:
	ਹਾਲਤ ਧੱਕੋ_DEFAULT_UNSPECIFIED:
	ਹਾਲਤ ਧੱਕੋ_DEFAULT_SIMPLE:
		ਜੇ (!same_remote)
			ਰੋਕੋ;
		ਜੇ (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_ਧੱਕੋ_simple(branch, remote);
		ਰੋਕੋ;

	ਹਾਲਤ ਧੱਕੋ_DEFAULT_UPSTREAM:
		ਜੇ (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		ਰੋਕੋ;

	ਹਾਲਤ ਧੱਕੋ_DEFAULT_CURRENT:
		ਰੋਕੋ;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	ਜੇ ((*flags & TRANSPORT_ਧੱਕੋ_AUTO_UPSTREAM) && branch->ਮਿਲਾਓ_nr == 0)
		*flags |= TRANSPORT_ਧੱਕੋ_ਸੈੱਟ_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਸੁਨੇਹਾ_advice_pull_before_ਧੱਕੋ[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਸੁਨੇਹਾ_advice_checkout_pull_ਧੱਕੋ[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਸੁਨੇਹਾ_advice_ref_fetch_ਪਹਿਲਾ[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਸੁਨੇਹਾ_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਸੁਨੇਹਾ_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਸੁਨੇਹਾ_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ਸਥਿਰ ਖਾਲੀ advise_pull_before_ਧੱਕੋ(ਖਾਲੀ)
{
	ਜੇ (!advice_enabled(ADVICE_ਧੱਕੋ_NON_FF_CURRENT) || !advice_enabled(ADVICE_ਧੱਕੋ_UPDATE_REJECTED))
		ਵਾਪਸ;
	advise(_(ਸੁਨੇਹਾ_advice_pull_before_ਧੱਕੋ));
}

ਸਥਿਰ ਖਾਲੀ advise_checkout_pull_ਧੱਕੋ(ਖਾਲੀ)
{
	ਜੇ (!advice_enabled(ADVICE_ਧੱਕੋ_NON_FF_MATCHING) || !advice_enabled(ADVICE_ਧੱਕੋ_UPDATE_REJECTED))
		ਵਾਪਸ;
	advise(_(ਸੁਨੇਹਾ_advice_checkout_pull_ਧੱਕੋ));
}

ਸਥਿਰ ਖਾਲੀ advise_ref_already_exists(ਖਾਲੀ)
{
	ਜੇ (!advice_enabled(ADVICE_ਧੱਕੋ_ALREADY_EXISTS) || !advice_enabled(ADVICE_ਧੱਕੋ_UPDATE_REJECTED))
		ਵਾਪਸ;
	advise(_(ਸੁਨੇਹਾ_advice_ref_already_exists));
}

ਸਥਿਰ ਖਾਲੀ advise_ref_fetch_ਪਹਿਲਾ(ਖਾਲੀ)
{
	ਜੇ (!advice_enabled(ADVICE_ਧੱਕੋ_FETCH_ਪਹਿਲਾ) || !advice_enabled(ADVICE_ਧੱਕੋ_UPDATE_REJECTED))
		ਵਾਪਸ;
	advise(_(ਸੁਨੇਹਾ_advice_ref_fetch_ਪਹਿਲਾ));
}

ਸਥਿਰ ਖਾਲੀ advise_ref_needs_force(ਖਾਲੀ)
{
	ਜੇ (!advice_enabled(ADVICE_ਧੱਕੋ_NEEDS_FORCE) || !advice_enabled(ADVICE_ਧੱਕੋ_UPDATE_REJECTED))
		ਵਾਪਸ;
	advise(_(ਸੁਨੇਹਾ_advice_ref_needs_force));
}

ਸਥਿਰ ਖਾਲੀ advise_ref_needs_update(ਖਾਲੀ)
{
	ਜੇ (!advice_enabled(ADVICE_ਧੱਕੋ_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_ਧੱਕੋ_UPDATE_REJECTED))
		ਵਾਪਸ;
	advise(_(ਸੁਨੇਹਾ_advice_ref_needs_update));
}

ਸਥਿਰ ਪੂਰਨ ਧੱਕੋ_with_options(ਬਣਤਰ transport *transport, ਬਣਤਰ refspec *rs,
			     ਪੂਰਨ flags)
{
	ਪੂਰਨ err;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ reject_reasons;
	ਅੱਖਰ *anon_url = transport_anonymize_url(transport->url);

	transport_ਸੈੱਟ_verbosity(transport, verbosity, ਤਰੱਕੀ);
	transport->family = family;

	ਜੇ (receivepack)
		transport_ਸੈੱਟ_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_ਸੈੱਟ_option(transport, TRANS_OPT_THIN, thin ? "yes" : ਨਿਰਾ);

	ਜੇ (!is_ਖਾਲੀ_ਥਾਵਾਂ_cas(&cas)) {
		ਜੇ (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	ਜੇ (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_ਧੱਕੋ(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	ਜੇ (err != 0) {
		fprintf(stderr, "%s", ਧੱਕੋ_get_color(ਧੱਕੋ_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", ਧੱਕੋ_get_color(ਧੱਕੋ_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	ਮੁਕਤ(anon_url);
	ਜੇ (!err)
		ਵਾਪਸ 0;

	ਜੇ (reject_reasons & REJECT_NON_FF_ਸਿਰ) {
		advise_pull_before_ਧੱਕੋ();
	} ਨਹੀਂਤਾਂ ਜੇ (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_ਧੱਕੋ();
	} ਨਹੀਂਤਾਂ ਜੇ (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} ਨਹੀਂਤਾਂ ਜੇ (reject_reasons & REJECT_FETCH_ਪਹਿਲਾ) {
		advise_ref_fetch_ਪਹਿਲਾ();
	} ਨਹੀਂਤਾਂ ਜੇ (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} ਨਹੀਂਤਾਂ ਜੇ (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ do_ਧੱਕੋ(ਪੂਰਨ flags,
		   ਅਚਲ ਬਣਤਰ string_list *ਧੱਕੋ_options,
		   ਬਣਤਰ remote *remote)
{
	ਪੂਰਨ errs;
	ਬਣਤਰ strvec *url;
	ਬਣਤਰ refspec *ਧੱਕੋ_refspec = &rs;

	ਜੇ (ਧੱਕੋ_options->nr)
		flags |= TRANSPORT_ਧੱਕੋ_OPTIONS;

	ਜੇ (!ਧੱਕੋ_refspec->nr && !(flags & TRANSPORT_ਧੱਕੋ_ALL)) {
		ਜੇ (remote->ਧੱਕੋ.nr) {
			ਧੱਕੋ_refspec = &remote->ਧੱਕੋ;
		} ਨਹੀਂਤਾਂ ਜੇ (!(flags & TRANSPORT_ਧੱਕੋ_MIRROR))
			ਤਿਆਰੀ_default_ਧੱਕੋ_refspecs(&flags, remote);
	}
	errs = 0;
	url = ਧੱਕੋ_url_ਦਾ_remote(remote);
	ਲਈ (ਮਾਪ_t i = 0; i < url->nr; i++) {
		ਬਣਤਰ transport *transport =
			transport_get(remote, url->v[i]);
		ਜੇ (flags & TRANSPORT_ਧੱਕੋ_OPTIONS)
			transport->ਧੱਕੋ_options = ਧੱਕੋ_options;
		ਜੇ (ਧੱਕੋ_with_options(transport, ਧੱਕੋ_refspec, flags))
			errs++;
	}
	ਵਾਪਸ !!errs;
}

ਸਥਿਰ ਪੂਰਨ option_parse_recurse_submodules(ਅਚਲ ਬਣਤਰ option *opt,
				   ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *recurse_submodules = opt->ਮੁੱਲ;

	ਜੇ (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	ਨਹੀਂਤਾਂ {
		ਜੇ (!strcmp(arg, "only-is-on-demand")) {
			ਜੇ (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} ਨਹੀਂਤਾਂ {
			*recurse_submodules = parse_ਧੱਕੋ_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ਧੱਕੋ_cert_flags(ਪੂਰਨ *flags, ਪੂਰਨ v)
{
	ਚੋਣ (v) {
	ਹਾਲਤ SEND_PACK_ਧੱਕੋ_CERT_NEVER:
		*flags &= ~(TRANSPORT_ਧੱਕੋ_CERT_ALWAYS | TRANSPORT_ਧੱਕੋ_CERT_IF_ASKED);
		ਰੋਕੋ;
	ਹਾਲਤ SEND_PACK_ਧੱਕੋ_CERT_ALWAYS:
		*flags |= TRANSPORT_ਧੱਕੋ_CERT_ALWAYS;
		*flags &= ~TRANSPORT_ਧੱਕੋ_CERT_IF_ASKED;
		ਰੋਕੋ;
	ਹਾਲਤ SEND_PACK_ਧੱਕੋ_CERT_IF_ASKED:
		*flags |= TRANSPORT_ਧੱਕੋ_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_ਧੱਕੋ_CERT_ALWAYS;
		ਰੋਕੋ;
	}
}


ਸਥਿਰ ਪੂਰਨ git_ਧੱਕੋ_config(ਅਚਲ ਅੱਖਰ *k, ਅਚਲ ਅੱਖਰ *v,
			   ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਅਚਲ ਅੱਖਰ *ਥਾਂ_name;
	ਪੂਰਨ *flags = cb;

	ਜੇ (!strcmp(k, "push.followtags")) {
		ਜੇ (git_config_bool(k, v))
			*flags |= TRANSPORT_ਧੱਕੋ_FOLLOW_ਟੈਗ_ਸੂਚੀ;
		ਨਹੀਂਤਾਂ
			*flags &= ~TRANSPORT_ਧੱਕੋ_FOLLOW_ਟੈਗ_ਸੂਚੀ;
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "push.autosetupremote")) {
		ਜੇ (git_config_bool(k, v))
			*flags |= TRANSPORT_ਧੱਕੋ_AUTO_UPSTREAM;
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "push.gpgsign")) {
		ਚੋਣ (git_parse_maybe_bool(v)) {
		ਹਾਲਤ 0:
			ਸੈੱਟ_ਧੱਕੋ_cert_flags(flags, SEND_PACK_ਧੱਕੋ_CERT_NEVER);
			ਰੋਕੋ;
		ਹਾਲਤ 1:
			ਸੈੱਟ_ਧੱਕੋ_cert_flags(flags, SEND_PACK_ਧੱਕੋ_CERT_ALWAYS);
			ਰੋਕੋ;
		ਮੂਲ:
			ਜੇ (!strcasecmp(v, "if-asked"))
				ਸੈੱਟ_ਧੱਕੋ_cert_flags(flags, SEND_PACK_ਧੱਕੋ_CERT_IF_ASKED);
			ਨਹੀਂਤਾਂ
				ਵਾਪਸ error(_("invalid value for '%s'"), k);
		}
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_ਧੱਕੋ_recurse_submodules_arg(k, v);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "submodule.recurse")) {
		ਪੂਰਨ val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "push.pushoption")) {
		ਵਾਪਸ parse_transport_option(k, v, &ਧੱਕੋ_options_config);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "color.push")) {
		ਧੱਕੋ_use_color = git_config_colorbool(k, v);
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(k, "color.push.", &ਥਾਂ_name)) {
		ਪੂਰਨ ਥਾਂ = parse_ਧੱਕੋ_color_ਥਾਂ(ਥਾਂ_name);
		ਜੇ (ਥਾਂ < 0)
			ਵਾਪਸ 0;
		ਜੇ (!v)
			ਵਾਪਸ config_error_nonbool(k);
		ਵਾਪਸ color_parse(v, ਧੱਕੋ_colors[ਥਾਂ]);
	} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(k, "push.useforceifincludes")) {
		ਜੇ (git_config_bool(k, v))
			*flags |= TRANSPORT_ਧੱਕੋ_FORCE_IF_INCLUDES;
		ਨਹੀਂਤਾਂ
			*flags &= ~TRANSPORT_ਧੱਕੋ_FORCE_IF_INCLUDES;
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ git_default_config(k, v, ctx, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ ਧੱਕੋ_ਗੁਣਜ(ਬਣਤਰ string_list *list,
			 ਅਚਲ ਬਣਤਰ string_list *ਧੱਕੋ_options,
			 ਪੂਰਨ flags,
			 ਪੂਰਨ ਟੈਗ_ਸੂਚੀ,
			 ਅਚਲ ਅੱਖਰ **refspecs,
			 ਪੂਰਨ refspec_nr)
{
	ਪੂਰਨ ਨਤੀਜਾ = 0;
	ਮਾਪ_t i;
	ਬਣਤਰ strvec argv = STRVEC_INIT;

	strvec_ਧੱਕੋ(&argv, "push");

	ਜੇ (flags & TRANSPORT_ਧੱਕੋ_FORCE)
		strvec_ਧੱਕੋ(&argv, "--force");
	ਜੇ (flags & TRANSPORT_ਧੱਕੋ_DRY_ਲੜੀਵਾਰ)
		strvec_ਧੱਕੋ(&argv, "--dry-run");
	ਜੇ (flags & TRANSPORT_ਧੱਕੋ_PORCELAIN)
		strvec_ਧੱਕੋ(&argv, "--porcelain");
	ਜੇ (flags & TRANSPORT_ਧੱਕੋ_PRUNE)
		strvec_ਧੱਕੋ(&argv, "--prune");
	ਜੇ (flags & TRANSPORT_ਧੱਕੋ_NO_HOOK)
		strvec_ਧੱਕੋ(&argv, "--no-verify");
	ਜੇ (flags & TRANSPORT_ਧੱਕੋ_FOLLOW_ਟੈਗ_ਸੂਚੀ)
		strvec_ਧੱਕੋ(&argv, "--follow-tags");
	ਜੇ (flags & TRANSPORT_ਧੱਕੋ_ਸੈੱਟ_UPSTREAM)
		strvec_ਧੱਕੋ(&argv, "--set-upstream");
	ਜੇ (flags & TRANSPORT_ਧੱਕੋ_FORCE_IF_INCLUDES)
		strvec_ਧੱਕੋ(&argv, "--force-if-includes");
	ਜੇ (flags & TRANSPORT_ਧੱਕੋ_ALL)
		strvec_ਧੱਕੋ(&argv, "--all");
	ਜੇ (flags & TRANSPORT_ਧੱਕੋ_MIRROR)
		strvec_ਧੱਕੋ(&argv, "--mirror");

	ਜੇ (flags & TRANSPORT_ਧੱਕੋ_CERT_ALWAYS)
		strvec_ਧੱਕੋ(&argv, "--signed=yes");
	ਨਹੀਂਤਾਂ ਜੇ (flags & TRANSPORT_ਧੱਕੋ_CERT_IF_ASKED)
		strvec_ਧੱਕੋ(&argv, "--signed=if-asked");
	ਜੇ (!thin)
		strvec_ਧੱਕੋ(&argv, "--no-thin");

	ਜੇ (deleterefs)
		strvec_ਧੱਕੋ(&argv, "--delete");

	ਜੇ (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	ਜੇ (verbosity >= 2)
		strvec_ਧੱਕੋ(&argv, "-v");
	ਜੇ (verbosity >= 1)
		strvec_ਧੱਕੋ(&argv, "-v");
	ਨਹੀਂਤਾਂ ਜੇ (verbosity < 0)
		strvec_ਧੱਕੋ(&argv, "-q");
	ਜੇ (ਤਰੱਕੀ > 0)
		strvec_ਧੱਕੋ(&argv, "--progress");
	ਨਹੀਂਤਾਂ ਜੇ (ਤਰੱਕੀ == 0)
		strvec_ਧੱਕੋ(&argv, "--no-progress");

	ਜੇ (family == TRANSPORT_FAMILY_IPV4)
		strvec_ਧੱਕੋ(&argv, "--ipv4");
	ਨਹੀਂਤਾਂ ਜੇ (family == TRANSPORT_FAMILY_IPV6)
		strvec_ਧੱਕੋ(&argv, "--ipv6");

	ਜੇ (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_ਧੱਕੋ(&argv, "--recurse-submodules=check");
	ਨਹੀਂਤਾਂ ਜੇ (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ਧੱਕੋ(&argv, "--recurse-submodules=on-demand");
	ਨਹੀਂਤਾਂ ਜੇ (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_ਧੱਕੋ(&argv, "--recurse-submodules=only");
	ਨਹੀਂਤਾਂ ਜੇ (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ਧੱਕੋ(&argv, "--recurse-submodules=no");


	ਜੇ (ਟੈਗ_ਸੂਚੀ)
		strvec_ਧੱਕੋ(&argv, "--tags");

	ਲਈ (i = 0; i < ਧੱਕੋ_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     ਧੱਕੋ_options->ਚੀਜ਼ਾਂ[i].string);

	ਲਈ (i = 0; i < cas.nr; i++) {
		ਜੇ (cas.ਇੰਦਰਾਜ[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.ਇੰਦਰਾਜ[i].refname);
		} ਨਹੀਂਤਾਂ ਜੇ (!is_null_oid(&cas.ਇੰਦਰਾਜ[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.ਇੰਦਰਾਜ[i].refname,
				     oid_ਮੰਜ਼ਿਲ_hex(&cas.ਇੰਦਰਾਜ[i].expect));
		} ਨਹੀਂਤਾਂ {
			strvec_ਧੱਕੋ(&argv, "--force-with-lease");
		}
	}

	ਲਈ (i = 0; i < list->nr; i++) {
		ਅਚਲ ਅੱਖਰ *name = list->ਚੀਜ਼ਾਂ[i].string;
		ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;
		ਪੂਰਨ j;

		strvec_pushv(&cmd.ਦਲੀਲਾਂ, argv.v);
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, name);

		ਲਈ (j = 0; j < refspec_nr; j++)
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, refspecs[j]);

		ਜੇ (verbosity >= 0)
			ਲਿਖੋ(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		ਜੇ (ਲੜੀਵਾਰ_command(&cmd)) {
			error(_("could not push to %s"), name);
			ਨਤੀਜਾ = 1;
		}
	}

	strvec_clear(&argv);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਖਾਲੀ die_if_repo_looks_like_ref(ਅਚਲ ਅੱਖਰ *repo)
{
	ਅਚਲ ਅੱਖਰ *slash = strchr(repo, '/');
	ਬਣਤਰ strbuf name = STRBUF_INIT;
	ਪੂਰਨ ਕੋਡ;

	ਜੇ (!slash || !slash[1] || file_exists(repo))
		ਵਾਪਸ;

	strbuf_ਜੋੜੋ(&name, repo, slash - repo);
	ਜੇ (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		ਵਾਪਸ;
	}

	ਕੋਡ = die_ਸੁਨੇਹਾ(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_ਧੱਕੋ_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	ਬਾਹਰ_ਨਿਕਲੋ(ਕੋਡ);
}

ਪੂਰਨ cmd_ਧੱਕੋ(ਪੂਰਨ argc,
	     ਅਚਲ ਅੱਖਰ **argv,
	     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	     ਬਣਤਰ repository *repository ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ flags = 0;
	ਪੂਰਨ ਟੈਗ_ਸੂਚੀ = 0;
	ਪੂਰਨ ਧੱਕੋ_cert = -1;
	ਪੂਰਨ rc = 0;
	ਪੂਰਨ ਅਧਾਰ_flags;
	ਅਚਲ ਅੱਖਰ *repo = ਨਿਰਾ;	/* default repository */
	ਬਣਤਰ string_list ਧੱਕੋ_options_cmdline = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list remote_ਸਮੂਹ = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list *ਧੱਕੋ_options;
	ਅਚਲ ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

	ਬਣਤਰ option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ( 0 , "all", &flags, N_("push all branches"), TRANSPORT_ਧੱਕੋ_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_ਧੱਕੋ_MIRROR|TRANSPORT_ਧੱਕੋ_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &ਟੈਗ_ਸੂਚੀ, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_ਧੱਕੋ_DRY_ਲੜੀਵਾਰ),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_ਧੱਕੋ_PORCELAIN),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('f', "force", &flags, N_("force updates"), TRANSPORT_ਧੱਕੋ_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_ਧੱਕੋ_cas_option),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_ਧੱਕੋ_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_ਧੱਕੋ_ਸੈੱਟ_UPSTREAM),
		OPT_BOOL(0, "progress", &ਤਰੱਕੀ, N_("force progress reporting")),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_ਧੱਕੋ_PRUNE),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_ਧੱਕੋ_NO_HOOK),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_ਧੱਕੋ_FOLLOW_ਟੈਗ_ਸੂਚੀ),
		OPT_CALLBACK_F(0, "signed", &ਧੱਕੋ_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_ਧੱਕੋ_signed),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_ਧੱਕੋ_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &ਧੱਕੋ_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_ਸਿਰਾ()
	};

	packet_trace_ਪਛਾਣ_ਤੱਤ("push");
	repo_config(the_repository, git_ਧੱਕੋ_config, &flags);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, ਧੱਕੋ_usage, 0);
	ਧੱਕੋ_options = (ਧੱਕੋ_options_cmdline.nr
		? &ਧੱਕੋ_options_cmdline
		: &ਧੱਕੋ_options_config);
	ਸੈੱਟ_ਧੱਕੋ_cert_flags(&flags, ਧੱਕੋ_cert);

	refspec_init_ਧੱਕੋ(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  ਟੈਗ_ਸੂਚੀ, "--tags",
				  flags & TRANSPORT_ਧੱਕੋ_ALL, "--all/--branches",
				  flags & TRANSPORT_ਧੱਕੋ_MIRROR, "--mirror");
	ਜੇ (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	ਜੇ (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	ਨਹੀਂਤਾਂ ਜੇ (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	ਨਹੀਂਤਾਂ ਜੇ (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	ਜੇ (argc > 0)
		repo = argv[0];

	ਜੇ (repo) {
		ਜੇ (!ਜੋੜੋ_remote_or_ਸਮੂਹ(repo, &remote_ਸਮੂਹ)) {
			ਬਣਤਰ remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			ਜੇ (advice_enabled(ADVICE_ਧੱਕੋ_REPO_LOOKS_LIKE_REF))
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
			ਜੇ (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_ਸਮੂਹ, r->name);
		}
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ remote *r = pushremote_get(ਨਿਰਾ);
		ਜੇ (!r)
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
		string_list_append(&remote_ਸਮੂਹ, r->name);
	}

	ਜੇ (!is_ਖਾਲੀ_ਥਾਵਾਂ_cas(&cas) && (flags & TRANSPORT_ਧੱਕੋ_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, ਧੱਕੋ_options)
		ਜੇ (strchr(ਵਸਤੂ->string, '\n'))
			die(_("push options must not have new line characters"));

	ਜੇ (remote_ਸਮੂਹ.nr == 1) {
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
		ਅਧਾਰ_flags = flags;
		{
			ਪੂਰਨ inner_flags = ਅਧਾਰ_flags;
			ਬਣਤਰ remote *r = pushremote_get(remote_ਸਮੂਹ.ਚੀਜ਼ਾਂ[0].string);
			ਜੇ (!r)
				die(_("no such remote or remote group: %s"),
				    remote_ਸਮੂਹ.ਚੀਜ਼ਾਂ[0].string);

			ਜੇ (r->mirror)
				inner_flags |= (TRANSPORT_ਧੱਕੋ_MIRROR|TRANSPORT_ਧੱਕੋ_FORCE);

			ਜੇ (inner_flags & TRANSPORT_ਧੱਕੋ_ALL) {
				ਜੇ (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			ਜੇ (inner_flags & TRANSPORT_ਧੱਕੋ_MIRROR) {
				ਜੇ (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (ਬਣਤਰ refspec) REFSPEC_INIT_ਧੱਕੋ(the_hash_algo);

			ਜੇ (ਟੈਗ_ਸੂਚੀ)
				refspec_append(&rs, "refs/tags/*");
			ਜੇ (argc > 0)
				ਸੈੱਟ_refspecs(argv + 1, argc - 1, r);

			rc = do_ਧੱਕੋ(inner_flags, ਧੱਕੋ_options, r);
		}
	} ਨਹੀਂਤਾਂ {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		ਜੇ (flags & TRANSPORT_ਧੱਕੋ_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = ਧੱਕੋ_ਗੁਣਜ(&remote_ਸਮੂਹ, ਧੱਕੋ_options, flags,
				   ਟੈਗ_ਸੂਚੀ,
				   argc > 1 ? argv + 1 : ਨਿਰਾ,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&ਧੱਕੋ_options_cmdline, 0);
	string_list_clear(&ਧੱਕੋ_options_config, 0);
	string_list_clear(&remote_ਸਮੂਹ, 0);
	clear_cas_option(&cas);

	ਜੇ (rc == -1)
		usage_with_options(ਧੱਕੋ_usage, options);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ rc;
}
