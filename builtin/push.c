/*
 * "git push"
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "refspec.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "remote.h"
#ಅನ್ತರ್ಭಾವ "transport.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "pkt-line.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "submodule-config.h"
#ಅನ್ತರ್ಭಾವ "send-pack.h"
#ಅನ್ತರ್ಭಾವ "trace2.h"
#ಅನ್ತರ್ಭಾವ "color.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ತಳ್ಳು_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	ಖಾಲಿ,
};

ಸ್ಥಿರ ಎಣಿಕೆ git_colorbool ತಳ್ಳು_use_color = GIT_COLOR_UNKNOWN;
ಸ್ಥಿರ ಅಕ್ಷರ ತಳ್ಳು_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

ಎಣಿಕೆ color_ತಳ್ಳು {
	ತಳ್ಳು_COLOR_RESET = 0,
	ತಳ್ಳು_COLOR_ERROR = 1
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_ತಳ್ಳು_color_ಸ್ಥಾನ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಾನ)
{
	ಆದರೆ (!strcasecmp(ಸ್ಥಾನ, "reset"))
		ಹಿಂತಿರುಗಿ ತಳ್ಳು_COLOR_RESET;
	ಆದರೆ (!strcasecmp(ಸ್ಥಾನ, "error"))
		ಹಿಂತಿರುಗಿ ತಳ್ಳು_COLOR_ERROR;
	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ್ಳು_get_color(ಎಣಿಕೆ color_ತಳ್ಳು ix)
{
	ಆದರೆ (want_color_stderr(ತಳ್ಳು_use_color))
		ಹಿಂತಿರುಗಿ ತಳ್ಳು_colors[ix];
	ಹಿಂತಿರುಗಿ "";
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ thin = 1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ deleterefs;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *receivepack;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verbosity;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ = -1;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ಸ್ಥಿರ ಎಣಿಕೆ transport_family family;

ಸ್ಥಿರ ರಚನೆ ತಳ್ಳು_cas_option cas;

ಸ್ಥಿರ ರಚನೆ refspec rs;

ಸ್ಥಿರ ರಚನೆ string_list ತಳ್ಳು_options_config = STRING_LIST_INIT_DUP;

ಸ್ಥಿರ ಶೂನ್ಯ refspec_append_mapped(ರಚನೆ refspec *refspec, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref,
				  ರಚನೆ remote *remote, ರಚನೆ ref *matched)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch_name;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ಆದರೆ (remote->ತಳ್ಳು.nr) {
		ರಚನೆ refspec_ಅಂಶ query = {
			.src = matched->name,
		};

		ಆದರೆ (!refspec_ಕಂಡುಹಿಡಿ_ಹೊಂದಿಕೆ(&remote->ತಳ್ಳು, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			ಬಿಡುಗಡೆ(query.dst);
			ಹಿಂತಿರುಗಿ;
		}
	}

	ಆದರೆ (cfg->ತಳ್ಳು_default == ತಳ್ಳು_DEFAULT_UPSTREAM &&
	    skip_ಪೂರ್ವಪ್ರತ್ಯಯ(matched->name, "refs/heads/", &branch_name)) {
		ರಚನೆ branch *branch = branch_get(branch_name);
		ಆದರೆ (branch->ವಿಲೀನ_nr == 1 && branch->ವಿಲೀನ[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->ವಿಲೀನ[0]->src);
			ಹಿಂತಿರುಗಿ;
		}
	}

	refspec_append(refspec, ref);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_refspecs(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **refs, ಪೂರ್ಣಾಂಕ nr, ರಚನೆ remote *remote)
{
	ರಚನೆ ref *local_refs = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < nr; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref = refs[i];
		ಆದರೆ (!strcmp("tag", ref)) {
			ಆದರೆ (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			ಆದರೆ (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			ಇಲ್ಲದಿದ್ದರೆ
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (deleterefs) {
			ಆದರೆ (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strchr(ref, ':')) {
			ರಚನೆ ref *matched = ಖಾಲಿ;

			/* lazily grab local_refs */
			ಆದರೆ (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			ಆದರೆ (ಎಣಿಕೆ_refspec_ಹೊಂದಿಕೆ(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			ಇಲ್ಲದಿದ್ದರೆ
				refspec_append_mapped(&rs, ref, remote, matched);
		} ಇಲ್ಲದಿದ್ದರೆ
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

ಸ್ಥಿರ NORETURN ಶೂನ್ಯ die_ತಳ್ಳು_simple(ರಚನೆ branch *branch,
				     ರಚನೆ remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *advice_pushdefault_maybe = "";
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *advice_automergesimple_maybe = "";
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *short_upstream = branch->ವಿಲೀನ[0]->src;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	skip_ಪೂರ್ವಪ್ರತ್ಯಯ(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	ಆದರೆ (cfg->ತಳ್ಳು_default == ತಳ್ಳು_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	ಆದರೆ (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಸಂದೇಶ_detached_ತಲೆ_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *get_upstream_ref(ಪೂರ್ಣಾಂಕ flags, ರಚನೆ branch *branch, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *remote_name)
{
	ಆದರೆ (branch->ವಿಲೀನ_nr == 0 && (flags & TRANSPORT_ತಳ್ಳು_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		ಹಿಂತಿರುಗಿ branch->refname;
	}

	ಆದರೆ (!branch->ವಿಲೀನ_nr || !branch->ವಿಲೀನ || !branch->remote_name) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *advice_autosetup_maybe = "";
		ಆದರೆ (!(flags & TRANSPORT_ತಳ್ಳು_AUTO_UPSTREAM)) {
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
	ಆದರೆ (branch->ವಿಲೀನ_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	ಹಿಂತಿರುಗಿ branch->ವಿಲೀನ[0]->src;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸಿದ್ಧತೆ_default_ತಳ್ಳು_refspecs(ಪೂರ್ಣಾಂಕ *flags, ರಚನೆ remote *remote)
{
	ರಚನೆ branch *branch;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dst;
	ಪೂರ್ಣಾಂಕ same_remote;
	ರಚನೆ repo_config_ಮೌಲ್ಯಗಳು *cfg = repo_config_ಮೌಲ್ಯಗಳು(the_repository);

	ಆಯ್ಕೆ (cfg->ತಳ್ಳು_default) {
	ಸ್ಥಿತಿ ತಳ್ಳು_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		ಹಿಂತಿರುಗಿ;

	ಸ್ಥಿತಿ ತಳ್ಳು_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		ಹಿಂತಿರುಗಿ;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ನಿಲ್ಲಿಸು;
	}

	branch = branch_get(ಖಾಲಿ);
	ಆದರೆ (!branch)
		die(_(ಸಂದೇಶ_detached_ತಲೆ_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, ಖಾಲಿ));

	ಆಯ್ಕೆ (cfg->ತಳ್ಳು_default) {
	ಪೂರ್ವನಿಯೋಜಿತ:
	ಸ್ಥಿತಿ ತಳ್ಳು_DEFAULT_UNSPECIFIED:
	ಸ್ಥಿತಿ ತಳ್ಳು_DEFAULT_SIMPLE:
		ಆದರೆ (!same_remote)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_ತಳ್ಳು_simple(branch, remote);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ತಳ್ಳು_DEFAULT_UPSTREAM:
		ಆದರೆ (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ತಳ್ಳು_DEFAULT_CURRENT:
		ನಿಲ್ಲಿಸು;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	ಆದರೆ ((*flags & TRANSPORT_ತಳ್ಳು_AUTO_UPSTREAM) && branch->ವಿಲೀನ_nr == 0)
		*flags |= TRANSPORT_ತಳ್ಳು_ಗಣ_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಸಂದೇಶ_advice_pull_before_ತಳ್ಳು[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಸಂದೇಶ_advice_checkout_pull_ತಳ್ಳು[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಸಂದೇಶ_advice_ref_fetch_ಮೊದಲ[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಸಂದೇಶ_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಸಂದೇಶ_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಸಂದೇಶ_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ಸ್ಥಿರ ಶೂನ್ಯ advise_pull_before_ತಳ್ಳು(ಶೂನ್ಯ)
{
	ಆದರೆ (!advice_enabled(ADVICE_ತಳ್ಳು_NON_FF_CURRENT) || !advice_enabled(ADVICE_ತಳ್ಳು_UPDATE_REJECTED))
		ಹಿಂತಿರುಗಿ;
	advise(_(ಸಂದೇಶ_advice_pull_before_ತಳ್ಳು));
}

ಸ್ಥಿರ ಶೂನ್ಯ advise_checkout_pull_ತಳ್ಳು(ಶೂನ್ಯ)
{
	ಆದರೆ (!advice_enabled(ADVICE_ತಳ್ಳು_NON_FF_MATCHING) || !advice_enabled(ADVICE_ತಳ್ಳು_UPDATE_REJECTED))
		ಹಿಂತಿರುಗಿ;
	advise(_(ಸಂದೇಶ_advice_checkout_pull_ತಳ್ಳು));
}

ಸ್ಥಿರ ಶೂನ್ಯ advise_ref_already_exists(ಶೂನ್ಯ)
{
	ಆದರೆ (!advice_enabled(ADVICE_ತಳ್ಳು_ALREADY_EXISTS) || !advice_enabled(ADVICE_ತಳ್ಳು_UPDATE_REJECTED))
		ಹಿಂತಿರುಗಿ;
	advise(_(ಸಂದೇಶ_advice_ref_already_exists));
}

ಸ್ಥಿರ ಶೂನ್ಯ advise_ref_fetch_ಮೊದಲ(ಶೂನ್ಯ)
{
	ಆದರೆ (!advice_enabled(ADVICE_ತಳ್ಳು_FETCH_ಮೊದಲ) || !advice_enabled(ADVICE_ತಳ್ಳು_UPDATE_REJECTED))
		ಹಿಂತಿರುಗಿ;
	advise(_(ಸಂದೇಶ_advice_ref_fetch_ಮೊದಲ));
}

ಸ್ಥಿರ ಶೂನ್ಯ advise_ref_needs_force(ಶೂನ್ಯ)
{
	ಆದರೆ (!advice_enabled(ADVICE_ತಳ್ಳು_NEEDS_FORCE) || !advice_enabled(ADVICE_ತಳ್ಳು_UPDATE_REJECTED))
		ಹಿಂತಿರುಗಿ;
	advise(_(ಸಂದೇಶ_advice_ref_needs_force));
}

ಸ್ಥಿರ ಶೂನ್ಯ advise_ref_needs_update(ಶೂನ್ಯ)
{
	ಆದರೆ (!advice_enabled(ADVICE_ತಳ್ಳು_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_ತಳ್ಳು_UPDATE_REJECTED))
		ಹಿಂತಿರುಗಿ;
	advise(_(ಸಂದೇಶ_advice_ref_needs_update));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ತಳ್ಳು_with_options(ರಚನೆ transport *transport, ರಚನೆ refspec *rs,
			     ಪೂರ್ಣಾಂಕ flags)
{
	ಪೂರ್ಣಾಂಕ err;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ reject_reasons;
	ಅಕ್ಷರ *anon_url = transport_anonymize_url(transport->url);

	transport_ಗಣ_verbosity(transport, verbosity, ಪ್ರಗತಿ);
	transport->family = family;

	ಆದರೆ (receivepack)
		transport_ಗಣ_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_ಗಣ_option(transport, TRANS_OPT_THIN, thin ? "yes" : ಖಾಲಿ);

	ಆದರೆ (!is_ಖಾಲಿಗಳು_cas(&cas)) {
		ಆದರೆ (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	ಆದರೆ (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_ತಳ್ಳು(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	ಆದರೆ (err != 0) {
		fprintf(stderr, "%s", ತಳ್ಳು_get_color(ತಳ್ಳು_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", ತಳ್ಳು_get_color(ತಳ್ಳು_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	ಬಿಡುಗಡೆ(anon_url);
	ಆದರೆ (!err)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (reject_reasons & REJECT_NON_FF_ತಲೆ) {
		advise_pull_before_ತಳ್ಳು();
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_ತಳ್ಳು();
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (reject_reasons & REJECT_FETCH_ಮೊದಲ) {
		advise_ref_fetch_ಮೊದಲ();
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_ತಳ್ಳು(ಪೂರ್ಣಾಂಕ flags,
		   ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *ತಳ್ಳು_options,
		   ರಚನೆ remote *remote)
{
	ಪೂರ್ಣಾಂಕ errs;
	ರಚನೆ strvec *url;
	ರಚನೆ refspec *ತಳ್ಳು_refspec = &rs;

	ಆದರೆ (ತಳ್ಳು_options->nr)
		flags |= TRANSPORT_ತಳ್ಳು_OPTIONS;

	ಆದರೆ (!ತಳ್ಳು_refspec->nr && !(flags & TRANSPORT_ತಳ್ಳು_ALL)) {
		ಆದರೆ (remote->ತಳ್ಳು.nr) {
			ತಳ್ಳು_refspec = &remote->ತಳ್ಳು;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(flags & TRANSPORT_ತಳ್ಳು_MIRROR))
			ಸಿದ್ಧತೆ_default_ತಳ್ಳು_refspecs(&flags, remote);
	}
	errs = 0;
	url = ತಳ್ಳು_url_ದ_remote(remote);
	ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < url->nr; i++) {
		ರಚನೆ transport *transport =
			transport_get(remote, url->v[i]);
		ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_OPTIONS)
			transport->ತಳ್ಳು_options = ತಳ್ಳು_options;
		ಆದರೆ (ತಳ್ಳು_with_options(transport, ತಳ್ಳು_refspec, flags))
			errs++;
	}
	ಹಿಂತಿರುಗಿ !!errs;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_recurse_submodules(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *recurse_submodules = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (!strcmp(arg, "only-is-on-demand")) {
			ಆದರೆ (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			*recurse_submodules = parse_ತಳ್ಳು_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ತಳ್ಳು_cert_flags(ಪೂರ್ಣಾಂಕ *flags, ಪೂರ್ಣಾಂಕ v)
{
	ಆಯ್ಕೆ (v) {
	ಸ್ಥಿತಿ SEND_PACK_ತಳ್ಳು_CERT_NEVER:
		*flags &= ~(TRANSPORT_ತಳ್ಳು_CERT_ALWAYS | TRANSPORT_ತಳ್ಳು_CERT_IF_ASKED);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SEND_PACK_ತಳ್ಳು_CERT_ALWAYS:
		*flags |= TRANSPORT_ತಳ್ಳು_CERT_ALWAYS;
		*flags &= ~TRANSPORT_ತಳ್ಳು_CERT_IF_ASKED;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SEND_PACK_ತಳ್ಳು_CERT_IF_ASKED:
		*flags |= TRANSPORT_ತಳ್ಳು_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_ತಳ್ಳು_CERT_ALWAYS;
		ನಿಲ್ಲಿಸು;
	}
}


ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_ತಳ್ಳು_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *k, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಾನ_name;
	ಪೂರ್ಣಾಂಕ *flags = cb;

	ಆದರೆ (!strcmp(k, "push.followtags")) {
		ಆದರೆ (git_config_bool(k, v))
			*flags |= TRANSPORT_ತಳ್ಳು_FOLLOW_ಟ್ಯಾಗುಗಳು;
		ಇಲ್ಲದಿದ್ದರೆ
			*flags &= ~TRANSPORT_ತಳ್ಳು_FOLLOW_ಟ್ಯಾಗುಗಳು;
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "push.autosetupremote")) {
		ಆದರೆ (git_config_bool(k, v))
			*flags |= TRANSPORT_ತಳ್ಳು_AUTO_UPSTREAM;
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "push.gpgsign")) {
		ಆಯ್ಕೆ (git_parse_maybe_bool(v)) {
		ಸ್ಥಿತಿ 0:
			ಗಣ_ತಳ್ಳು_cert_flags(flags, SEND_PACK_ತಳ್ಳು_CERT_NEVER);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 1:
			ಗಣ_ತಳ್ಳು_cert_flags(flags, SEND_PACK_ತಳ್ಳು_CERT_ALWAYS);
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			ಆದರೆ (!strcasecmp(v, "if-asked"))
				ಗಣ_ತಳ್ಳು_cert_flags(flags, SEND_PACK_ತಳ್ಳು_CERT_IF_ASKED);
			ಇಲ್ಲದಿದ್ದರೆ
				ಹಿಂತಿರುಗಿ error(_("invalid value for '%s'"), k);
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_ತಳ್ಳು_recurse_submodules_arg(k, v);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "submodule.recurse")) {
		ಪೂರ್ಣಾಂಕ val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "push.pushoption")) {
		ಹಿಂತಿರುಗಿ parse_transport_option(k, v, &ತಳ್ಳು_options_config);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "color.push")) {
		ತಳ್ಳು_use_color = git_config_colorbool(k, v);
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(k, "color.push.", &ಸ್ಥಾನ_name)) {
		ಪೂರ್ಣಾಂಕ ಸ್ಥಾನ = parse_ತಳ್ಳು_color_ಸ್ಥಾನ(ಸ್ಥಾನ_name);
		ಆದರೆ (ಸ್ಥಾನ < 0)
			ಹಿಂತಿರುಗಿ 0;
		ಆದರೆ (!v)
			ಹಿಂತಿರುಗಿ config_error_nonbool(k);
		ಹಿಂತಿರುಗಿ color_parse(v, ತಳ್ಳು_colors[ಸ್ಥಾನ]);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(k, "push.useforceifincludes")) {
		ಆದರೆ (git_config_bool(k, v))
			*flags |= TRANSPORT_ತಳ್ಳು_FORCE_IF_INCLUDES;
		ಇಲ್ಲದಿದ್ದರೆ
			*flags &= ~TRANSPORT_ತಳ್ಳು_FORCE_IF_INCLUDES;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ git_default_config(k, v, ctx, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ತಳ್ಳು_ಅಪವರ್ತ್ಯ(ರಚನೆ string_list *list,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ string_list *ತಳ್ಳು_options,
			 ಪೂರ್ಣಾಂಕ flags,
			 ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗುಗಳು,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **refspecs,
			 ಪೂರ್ಣಾಂಕ refspec_nr)
{
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
	ಗಾತ್ರ_t i;
	ರಚನೆ strvec argv = STRVEC_INIT;

	strvec_ತಳ್ಳು(&argv, "push");

	ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_FORCE)
		strvec_ತಳ್ಳು(&argv, "--force");
	ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_DRY_ಸತತ)
		strvec_ತಳ್ಳು(&argv, "--dry-run");
	ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_PORCELAIN)
		strvec_ತಳ್ಳು(&argv, "--porcelain");
	ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_PRUNE)
		strvec_ತಳ್ಳು(&argv, "--prune");
	ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_NO_HOOK)
		strvec_ತಳ್ಳು(&argv, "--no-verify");
	ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_FOLLOW_ಟ್ಯಾಗುಗಳು)
		strvec_ತಳ್ಳು(&argv, "--follow-tags");
	ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_ಗಣ_UPSTREAM)
		strvec_ತಳ್ಳು(&argv, "--set-upstream");
	ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_FORCE_IF_INCLUDES)
		strvec_ತಳ್ಳು(&argv, "--force-if-includes");
	ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_ALL)
		strvec_ತಳ್ಳು(&argv, "--all");
	ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_MIRROR)
		strvec_ತಳ್ಳು(&argv, "--mirror");

	ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_CERT_ALWAYS)
		strvec_ತಳ್ಳು(&argv, "--signed=yes");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_CERT_IF_ASKED)
		strvec_ತಳ್ಳು(&argv, "--signed=if-asked");
	ಆದರೆ (!thin)
		strvec_ತಳ್ಳು(&argv, "--no-thin");

	ಆದರೆ (deleterefs)
		strvec_ತಳ್ಳು(&argv, "--delete");

	ಆದರೆ (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	ಆದರೆ (verbosity >= 2)
		strvec_ತಳ್ಳು(&argv, "-v");
	ಆದರೆ (verbosity >= 1)
		strvec_ತಳ್ಳು(&argv, "-v");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (verbosity < 0)
		strvec_ತಳ್ಳು(&argv, "-q");
	ಆದರೆ (ಪ್ರಗತಿ > 0)
		strvec_ತಳ್ಳು(&argv, "--progress");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಪ್ರಗತಿ == 0)
		strvec_ತಳ್ಳು(&argv, "--no-progress");

	ಆದರೆ (family == TRANSPORT_FAMILY_IPV4)
		strvec_ತಳ್ಳು(&argv, "--ipv4");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (family == TRANSPORT_FAMILY_IPV6)
		strvec_ತಳ್ಳು(&argv, "--ipv6");

	ಆದರೆ (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_ತಳ್ಳು(&argv, "--recurse-submodules=check");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ತಳ್ಳು(&argv, "--recurse-submodules=on-demand");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_ತಳ್ಳು(&argv, "--recurse-submodules=only");
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ತಳ್ಳು(&argv, "--recurse-submodules=no");


	ಆದರೆ (ಟ್ಯಾಗುಗಳು)
		strvec_ತಳ್ಳು(&argv, "--tags");

	ಗಾಗಿ (i = 0; i < ತಳ್ಳು_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     ತಳ್ಳು_options->ವಸ್ತುಗಳು[i].string);

	ಗಾಗಿ (i = 0; i < cas.nr; i++) {
		ಆದರೆ (cas.ನಮೂದು[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.ನಮೂದು[i].refname);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!is_null_oid(&cas.ನಮೂದು[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.ನಮೂದು[i].refname,
				     oid_ಗಮ್ಯ_hex(&cas.ನಮೂದು[i].expect));
		} ಇಲ್ಲದಿದ್ದರೆ {
			strvec_ತಳ್ಳು(&argv, "--force-with-lease");
		}
	}

	ಗಾಗಿ (i = 0; i < list->nr; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = list->ವಸ್ತುಗಳು[i].string;
		ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;
		ಪೂರ್ಣಾಂಕ j;

		strvec_pushv(&cmd.ವಾದಗಳು, argv.v);
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, name);

		ಗಾಗಿ (j = 0; j < refspec_nr; j++)
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, refspecs[j]);

		ಆದರೆ (verbosity >= 0)
			ಬರೆ(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		ಆದರೆ (ಸತತ_command(&cmd)) {
			error(_("could not push to %s"), name);
			ಫಲಿತಾಂಶ = 1;
		}
	}

	strvec_clear(&argv);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಶೂನ್ಯ die_if_repo_looks_like_ref(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *repo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *slash = strchr(repo, '/');
	ರಚನೆ strbuf name = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;

	ಆದರೆ (!slash || !slash[1] || file_exists(repo))
		ಹಿಂತಿರುಗಿ;

	strbuf_ಕೂಡಿಸು(&name, repo, slash - repo);
	ಆದರೆ (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		ಹಿಂತಿರುಗಿ;
	}

	ಸಂಕೇತ_2 = die_ಸಂದೇಶ(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_ತಳ್ಳು_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	ನಿರ್ಗಮಿಸು(ಸಂಕೇತ_2);
}

ಪೂರ್ಣಾಂಕ cmd_ತಳ್ಳು(ಪೂರ್ಣಾಂಕ argc,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	     ರಚನೆ repository *repository ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ flags = 0;
	ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗುಗಳು = 0;
	ಪೂರ್ಣಾಂಕ ತಳ್ಳು_cert = -1;
	ಪೂರ್ಣಾಂಕ rc = 0;
	ಪೂರ್ಣಾಂಕ ತಳ_flags;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *repo = ಖಾಲಿ;	/* default repository */
	ರಚನೆ string_list ತಳ್ಳು_options_cmdline = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list remote_ಗುಂಪು = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list *ತಳ್ಳು_options;
	ಸ್ಥಿರಾಂಕ ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

	ರಚನೆ option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_ಬಿಟ್( 0 , "all", &flags, N_("push all branches"), TRANSPORT_ತಳ್ಳು_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_ಬಿಟ್( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_ತಳ್ಳು_MIRROR|TRANSPORT_ತಳ್ಳು_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &ಟ್ಯಾಗುಗಳು, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_ಬಿಟ್('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_ತಳ್ಳು_DRY_ಸತತ),
		OPT_ಬಿಟ್( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_ತಳ್ಳು_PORCELAIN),
		OPT_ಬಿಟ್('f', "force", &flags, N_("force updates"), TRANSPORT_ತಳ್ಳು_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_ತಳ್ಳು_cas_option),
		OPT_ಬಿಟ್(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_ತಳ್ಳು_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_ಬಿಟ್('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_ತಳ್ಳು_ಗಣ_UPSTREAM),
		OPT_BOOL(0, "progress", &ಪ್ರಗತಿ, N_("force progress reporting")),
		OPT_ಬಿಟ್(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_ತಳ್ಳು_PRUNE),
		OPT_ಬಿಟ್(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_ತಳ್ಳು_NO_HOOK),
		OPT_ಬಿಟ್(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_ತಳ್ಳು_FOLLOW_ಟ್ಯಾಗುಗಳು),
		OPT_CALLBACK_F(0, "signed", &ತಳ್ಳು_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_ತಳ್ಳು_signed),
		OPT_ಬಿಟ್(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_ತಳ್ಳು_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &ತಳ್ಳು_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_ಕೊನೆ()
	};

	packet_trace_ತಟಸ್ಥ("push");
	repo_config(the_repository, git_ತಳ್ಳು_config, &flags);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, ತಳ್ಳು_usage, 0);
	ತಳ್ಳು_options = (ತಳ್ಳು_options_cmdline.nr
		? &ತಳ್ಳು_options_cmdline
		: &ತಳ್ಳು_options_config);
	ಗಣ_ತಳ್ಳು_cert_flags(&flags, ತಳ್ಳು_cert);

	refspec_init_ತಳ್ಳು(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  ಟ್ಯಾಗುಗಳು, "--tags",
				  flags & TRANSPORT_ತಳ್ಳು_ALL, "--all/--branches",
				  flags & TRANSPORT_ತಳ್ಳು_MIRROR, "--mirror");
	ಆದರೆ (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	ಆದರೆ (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	ಆದರೆ (argc > 0)
		repo = argv[0];

	ಆದರೆ (repo) {
		ಆದರೆ (!ಕೂಡಿಸು_remote_or_ಗುಂಪು(repo, &remote_ಗುಂಪು)) {
			ರಚನೆ remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			ಆದರೆ (advice_enabled(ADVICE_ತಳ್ಳು_REPO_LOOKS_LIKE_REF))
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
			ಆದರೆ (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_ಗುಂಪು, r->name);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ remote *r = pushremote_get(ಖಾಲಿ);
		ಆದರೆ (!r)
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
		string_list_append(&remote_ಗುಂಪು, r->name);
	}

	ಆದರೆ (!is_ಖಾಲಿಗಳು_cas(&cas) && (flags & TRANSPORT_ತಳ್ಳು_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_ಅಂಶ(ಅಂಶ, ತಳ್ಳು_options)
		ಆದರೆ (strchr(ಅಂಶ->string, '\n'))
			die(_("push options must not have new line characters"));

	ಆದರೆ (remote_ಗುಂಪು.nr == 1) {
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
		ತಳ_flags = flags;
		{
			ಪೂರ್ಣಾಂಕ inner_flags = ತಳ_flags;
			ರಚನೆ remote *r = pushremote_get(remote_ಗುಂಪು.ವಸ್ತುಗಳು[0].string);
			ಆದರೆ (!r)
				die(_("no such remote or remote group: %s"),
				    remote_ಗುಂಪು.ವಸ್ತುಗಳು[0].string);

			ಆದರೆ (r->mirror)
				inner_flags |= (TRANSPORT_ತಳ್ಳು_MIRROR|TRANSPORT_ತಳ್ಳು_FORCE);

			ಆದರೆ (inner_flags & TRANSPORT_ತಳ್ಳು_ALL) {
				ಆದರೆ (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			ಆದರೆ (inner_flags & TRANSPORT_ತಳ್ಳು_MIRROR) {
				ಆದರೆ (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (ರಚನೆ refspec) REFSPEC_INIT_ತಳ್ಳು(the_hash_algo);

			ಆದರೆ (ಟ್ಯಾಗುಗಳು)
				refspec_append(&rs, "refs/tags/*");
			ಆದರೆ (argc > 0)
				ಗಣ_refspecs(argv + 1, argc - 1, r);

			rc = do_ತಳ್ಳು(inner_flags, ತಳ್ಳು_options, r);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		ಆದರೆ (flags & TRANSPORT_ತಳ್ಳು_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = ತಳ್ಳು_ಅಪವರ್ತ್ಯ(&remote_ಗುಂಪು, ತಳ್ಳು_options, flags,
				   ಟ್ಯಾಗುಗಳು,
				   argc > 1 ? argv + 1 : ಖಾಲಿ,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&ತಳ್ಳು_options_cmdline, 0);
	string_list_clear(&ತಳ್ಳು_options_config, 0);
	string_list_clear(&remote_ಗುಂಪು, 0);
	clear_cas_option(&cas);

	ಆದರೆ (rc == -1)
		usage_with_options(ತಳ್ಳು_usage, options);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ rc;
}
