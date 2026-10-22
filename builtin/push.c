/*
 * "git push"
 */

#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "branch.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "refspec.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "remote.h"
#അന്തര്ഭാവ "transport.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "pkt-line.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "submodule-config.h"
#അന്തര്ഭാവ "send-pack.h"
#അന്തര്ഭാവ "trace2.h"
#അന്തര്ഭാവ "color.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം തള്ളുക_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	ശൂന്യ,
};

സ്ഥിതം എണ്ണൽ git_colorbool തള്ളുക_use_color = GIT_COLOR_UNKNOWN;
സ്ഥിതം അക്ഷരം തള്ളുക_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

എണ്ണൽ color_തള്ളുക {
	തള്ളുക_COLOR_RESET = 0,
	തള്ളുക_COLOR_ERROR = 1
};

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_തള്ളുക_color_slot(സ്ഥിരം അക്ഷരം *slot)
{
	എങ്കിൽ (!strcasecmp(slot, "reset"))
		തിരികെ തള്ളുക_COLOR_RESET;
	എങ്കിൽ (!strcasecmp(slot, "error"))
		തിരികെ തള്ളുക_COLOR_ERROR;
	തിരികെ -1;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *തള്ളുക_get_color(എണ്ണൽ color_തള്ളുക ix)
{
	എങ്കിൽ (want_color_stderr(തള്ളുക_use_color))
		തിരികെ തള്ളുക_colors[ix];
	തിരികെ "";
}

സ്ഥിതം പൂർണ്ണസംഖ്യ thin = 1;
സ്ഥിതം പൂർണ്ണസംഖ്യ deleterefs;
സ്ഥിതം സ്ഥിരം അക്ഷരം *receivepack;
സ്ഥിതം പൂർണ്ണസംഖ്യ verbosity;
സ്ഥിതം പൂർണ്ണസംഖ്യ progress = -1;
സ്ഥിതം പൂർണ്ണസംഖ്യ recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
സ്ഥിതം എണ്ണൽ transport_family family;

സ്ഥിതം ഘടന തള്ളുക_cas_option cas;

സ്ഥിതം ഘടന refspec rs;

സ്ഥിതം ഘടന string_list തള്ളുക_options_config = STRING_LIST_INIT_DUP;

സ്ഥിതം ശൂന്യം refspec_append_mapped(ഘടന refspec *refspec, സ്ഥിരം അക്ഷരം *ref,
				  ഘടന remote *remote, ഘടന ref *matched)
{
	സ്ഥിരം അക്ഷരം *branch_name;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	എങ്കിൽ (remote->തള്ളുക.nr) {
		ഘടന refspec_item query = {
			.src = matched->name,
		};

		എങ്കിൽ (!refspec_find_match(&remote->തള്ളുക, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			സ്വതന്ത്രം(query.dst);
			തിരികെ;
		}
	}

	എങ്കിൽ (cfg->തള്ളുക_default == തള്ളുക_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		ഘടന branch *branch = branch_get(branch_name);
		എങ്കിൽ (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			തിരികെ;
		}
	}

	refspec_append(refspec, ref);
}

സ്ഥിതം ശൂന്യം set_refspecs(സ്ഥിരം അക്ഷരം **refs, പൂർണ്ണസംഖ്യ nr, ഘടന remote *remote)
{
	ഘടന ref *local_refs = ശൂന്യ;
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < nr; i++) {
		സ്ഥിരം അക്ഷരം *ref = refs[i];
		എങ്കിൽ (!strcmp("tag", ref)) {
			എങ്കിൽ (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			എങ്കിൽ (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			അല്ലെങ്കിൽ
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} അല്ലെങ്കിൽ എങ്കിൽ (deleterefs) {
			എങ്കിൽ (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} അല്ലെങ്കിൽ എങ്കിൽ (!strchr(ref, ':')) {
			ഘടന ref *matched = ശൂന്യ;

			/* lazily grab local_refs */
			എങ്കിൽ (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			എങ്കിൽ (എണ്ണം_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			അല്ലെങ്കിൽ
				refspec_append_mapped(&rs, ref, remote, matched);
		} അല്ലെങ്കിൽ
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

സ്ഥിതം NORETURN ശൂന്യം die_തള്ളുക_simple(ഘടന branch *branch,
				     ഘടന remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	സ്ഥിരം അക്ഷരം *advice_pushdefault_maybe = "";
	സ്ഥിരം അക്ഷരം *advice_automergesimple_maybe = "";
	സ്ഥിരം അക്ഷരം *short_upstream = branch->merge[0]->src;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	skip_prefix(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	എങ്കിൽ (cfg->തള്ളുക_default == തള്ളുക_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	എങ്കിൽ (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

സ്ഥിതം സ്ഥിരം അക്ഷരം message_detached_head_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

സ്ഥിതം സ്ഥിരം അക്ഷരം *get_upstream_ref(പൂർണ്ണസംഖ്യ flags, ഘടന branch *branch, സ്ഥിരം അക്ഷരം *remote_name)
{
	എങ്കിൽ (branch->merge_nr == 0 && (flags & TRANSPORT_തള്ളുക_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		തിരികെ branch->refname;
	}

	എങ്കിൽ (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		സ്ഥിരം അക്ഷരം *advice_autosetup_maybe = "";
		എങ്കിൽ (!(flags & TRANSPORT_തള്ളുക_AUTO_UPSTREAM)) {
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
	എങ്കിൽ (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	തിരികെ branch->merge[0]->src;
}

സ്ഥിതം ശൂന്യം setup_default_തള്ളുക_refspecs(പൂർണ്ണസംഖ്യ *flags, ഘടന remote *remote)
{
	ഘടന branch *branch;
	സ്ഥിരം അക്ഷരം *dst;
	പൂർണ്ണസംഖ്യ same_remote;
	ഘടന repo_config_മൂല്യങ്ങൾ *cfg = repo_config_മൂല്യങ്ങൾ(the_repository);

	തിരഞ്ഞെടുപ്പ് (cfg->തള്ളുക_default) {
	അവസ്ഥ തള്ളുക_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		തിരികെ;

	അവസ്ഥ തള്ളുക_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		തിരികെ;
	സ്ഥിരസ്ഥിതി:
		നിർത്തുക;
	}

	branch = branch_get(ശൂന്യ);
	എങ്കിൽ (!branch)
		die(_(message_detached_head_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, ശൂന്യ));

	തിരഞ്ഞെടുപ്പ് (cfg->തള്ളുക_default) {
	സ്ഥിരസ്ഥിതി:
	അവസ്ഥ തള്ളുക_DEFAULT_UNSPECIFIED:
	അവസ്ഥ തള്ളുക_DEFAULT_SIMPLE:
		എങ്കിൽ (!same_remote)
			നിർത്തുക;
		എങ്കിൽ (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_തള്ളുക_simple(branch, remote);
		നിർത്തുക;

	അവസ്ഥ തള്ളുക_DEFAULT_UPSTREAM:
		എങ്കിൽ (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		നിർത്തുക;

	അവസ്ഥ തള്ളുക_DEFAULT_CURRENT:
		നിർത്തുക;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	എങ്കിൽ ((*flags & TRANSPORT_തള്ളുക_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_തള്ളുക_SET_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം message_advice_pull_before_തള്ളുക[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

സ്ഥിതം സ്ഥിരം അക്ഷരം message_advice_checkout_pull_തള്ളുക[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

സ്ഥിതം സ്ഥിരം അക്ഷരം message_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

സ്ഥിതം സ്ഥിരം അക്ഷരം message_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

സ്ഥിതം സ്ഥിരം അക്ഷരം message_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

സ്ഥിതം സ്ഥിരം അക്ഷരം message_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

സ്ഥിതം ശൂന്യം advise_pull_before_തള്ളുക(ശൂന്യം)
{
	എങ്കിൽ (!advice_enabled(ADVICE_തള്ളുക_NON_FF_CURRENT) || !advice_enabled(ADVICE_തള്ളുക_UPDATE_REJECTED))
		തിരികെ;
	advise(_(message_advice_pull_before_തള്ളുക));
}

സ്ഥിതം ശൂന്യം advise_checkout_pull_തള്ളുക(ശൂന്യം)
{
	എങ്കിൽ (!advice_enabled(ADVICE_തള്ളുക_NON_FF_MATCHING) || !advice_enabled(ADVICE_തള്ളുക_UPDATE_REJECTED))
		തിരികെ;
	advise(_(message_advice_checkout_pull_തള്ളുക));
}

സ്ഥിതം ശൂന്യം advise_ref_already_exists(ശൂന്യം)
{
	എങ്കിൽ (!advice_enabled(ADVICE_തള്ളുക_ALREADY_EXISTS) || !advice_enabled(ADVICE_തള്ളുക_UPDATE_REJECTED))
		തിരികെ;
	advise(_(message_advice_ref_already_exists));
}

സ്ഥിതം ശൂന്യം advise_ref_fetch_first(ശൂന്യം)
{
	എങ്കിൽ (!advice_enabled(ADVICE_തള്ളുക_FETCH_FIRST) || !advice_enabled(ADVICE_തള്ളുക_UPDATE_REJECTED))
		തിരികെ;
	advise(_(message_advice_ref_fetch_first));
}

സ്ഥിതം ശൂന്യം advise_ref_needs_force(ശൂന്യം)
{
	എങ്കിൽ (!advice_enabled(ADVICE_തള്ളുക_NEEDS_FORCE) || !advice_enabled(ADVICE_തള്ളുക_UPDATE_REJECTED))
		തിരികെ;
	advise(_(message_advice_ref_needs_force));
}

സ്ഥിതം ശൂന്യം advise_ref_needs_update(ശൂന്യം)
{
	എങ്കിൽ (!advice_enabled(ADVICE_തള്ളുക_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_തള്ളുക_UPDATE_REJECTED))
		തിരികെ;
	advise(_(message_advice_ref_needs_update));
}

സ്ഥിതം പൂർണ്ണസംഖ്യ തള്ളുക_with_options(ഘടന transport *transport, ഘടന refspec *rs,
			     പൂർണ്ണസംഖ്യ flags)
{
	പൂർണ്ണസംഖ്യ err;
	unsigned പൂർണ്ണസംഖ്യ reject_reasons;
	അക്ഷരം *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	എങ്കിൽ (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : ശൂന്യ);

	എങ്കിൽ (!is_empty_cas(&cas)) {
		എങ്കിൽ (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	എങ്കിൽ (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_തള്ളുക(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	എങ്കിൽ (err != 0) {
		fprintf(stderr, "%s", തള്ളുക_get_color(തള്ളുക_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", തള്ളുക_get_color(തള്ളുക_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	സ്വതന്ത്രം(anon_url);
	എങ്കിൽ (!err)
		തിരികെ 0;

	എങ്കിൽ (reject_reasons & REJECT_NON_FF_HEAD) {
		advise_pull_before_തള്ളുക();
	} അല്ലെങ്കിൽ എങ്കിൽ (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_തള്ളുക();
	} അല്ലെങ്കിൽ എങ്കിൽ (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} അല്ലെങ്കിൽ എങ്കിൽ (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} അല്ലെങ്കിൽ എങ്കിൽ (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} അല്ലെങ്കിൽ എങ്കിൽ (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_തള്ളുക(പൂർണ്ണസംഖ്യ flags,
		   സ്ഥിരം ഘടന string_list *തള്ളുക_options,
		   ഘടന remote *remote)
{
	പൂർണ്ണസംഖ്യ errs;
	ഘടന strvec *url;
	ഘടന refspec *തള്ളുക_refspec = &rs;

	എങ്കിൽ (തള്ളുക_options->nr)
		flags |= TRANSPORT_തള്ളുക_OPTIONS;

	എങ്കിൽ (!തള്ളുക_refspec->nr && !(flags & TRANSPORT_തള്ളുക_ALL)) {
		എങ്കിൽ (remote->തള്ളുക.nr) {
			തള്ളുക_refspec = &remote->തള്ളുക;
		} അല്ലെങ്കിൽ എങ്കിൽ (!(flags & TRANSPORT_തള്ളുക_MIRROR))
			setup_default_തള്ളുക_refspecs(&flags, remote);
	}
	errs = 0;
	url = തള്ളുക_url_of_remote(remote);
	വേണ്ടി (size_t i = 0; i < url->nr; i++) {
		ഘടന transport *transport =
			transport_get(remote, url->v[i]);
		എങ്കിൽ (flags & TRANSPORT_തള്ളുക_OPTIONS)
			transport->തള്ളുക_options = തള്ളുക_options;
		എങ്കിൽ (തള്ളുക_with_options(transport, തള്ളുക_refspec, flags))
			errs++;
	}
	തിരികെ !!errs;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_recurse_submodules(സ്ഥിരം ഘടന option *opt,
				   സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *recurse_submodules = opt->മൂല്യം;

	എങ്കിൽ (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	അല്ലെങ്കിൽ {
		എങ്കിൽ (!strcmp(arg, "only-is-on-demand")) {
			എങ്കിൽ (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} അല്ലെങ്കിൽ {
			*recurse_submodules = parse_തള്ളുക_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം set_തള്ളുക_cert_flags(പൂർണ്ണസംഖ്യ *flags, പൂർണ്ണസംഖ്യ v)
{
	തിരഞ്ഞെടുപ്പ് (v) {
	അവസ്ഥ SEND_PACK_തള്ളുക_CERT_NEVER:
		*flags &= ~(TRANSPORT_തള്ളുക_CERT_ALWAYS | TRANSPORT_തള്ളുക_CERT_IF_ASKED);
		നിർത്തുക;
	അവസ്ഥ SEND_PACK_തള്ളുക_CERT_ALWAYS:
		*flags |= TRANSPORT_തള്ളുക_CERT_ALWAYS;
		*flags &= ~TRANSPORT_തള്ളുക_CERT_IF_ASKED;
		നിർത്തുക;
	അവസ്ഥ SEND_PACK_തള്ളുക_CERT_IF_ASKED:
		*flags |= TRANSPORT_തള്ളുക_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_തള്ളുക_CERT_ALWAYS;
		നിർത്തുക;
	}
}


സ്ഥിതം പൂർണ്ണസംഖ്യ git_തള്ളുക_config(സ്ഥിരം അക്ഷരം *k, സ്ഥിരം അക്ഷരം *v,
			   സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	സ്ഥിരം അക്ഷരം *slot_name;
	പൂർണ്ണസംഖ്യ *flags = cb;

	എങ്കിൽ (!strcmp(k, "push.followtags")) {
		എങ്കിൽ (git_config_bool(k, v))
			*flags |= TRANSPORT_തള്ളുക_FOLLOW_TAGS;
		അല്ലെങ്കിൽ
			*flags &= ~TRANSPORT_തള്ളുക_FOLLOW_TAGS;
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "push.autosetupremote")) {
		എങ്കിൽ (git_config_bool(k, v))
			*flags |= TRANSPORT_തള്ളുക_AUTO_UPSTREAM;
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "push.gpgsign")) {
		തിരഞ്ഞെടുപ്പ് (git_parse_maybe_bool(v)) {
		അവസ്ഥ 0:
			set_തള്ളുക_cert_flags(flags, SEND_PACK_തള്ളുക_CERT_NEVER);
			നിർത്തുക;
		അവസ്ഥ 1:
			set_തള്ളുക_cert_flags(flags, SEND_PACK_തള്ളുക_CERT_ALWAYS);
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			എങ്കിൽ (!strcasecmp(v, "if-asked"))
				set_തള്ളുക_cert_flags(flags, SEND_PACK_തള്ളുക_CERT_IF_ASKED);
			അല്ലെങ്കിൽ
				തിരികെ error(_("invalid value for '%s'"), k);
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_തള്ളുക_recurse_submodules_arg(k, v);
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "submodule.recurse")) {
		പൂർണ്ണസംഖ്യ val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "push.pushoption")) {
		തിരികെ parse_transport_option(k, v, &തള്ളുക_options_config);
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "color.push")) {
		തള്ളുക_use_color = git_config_colorbool(k, v);
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(k, "color.push.", &slot_name)) {
		പൂർണ്ണസംഖ്യ slot = parse_തള്ളുക_color_slot(slot_name);
		എങ്കിൽ (slot < 0)
			തിരികെ 0;
		എങ്കിൽ (!v)
			തിരികെ config_error_nonbool(k);
		തിരികെ color_parse(v, തള്ളുക_colors[slot]);
	} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(k, "push.useforceifincludes")) {
		എങ്കിൽ (git_config_bool(k, v))
			*flags |= TRANSPORT_തള്ളുക_FORCE_IF_INCLUDES;
		അല്ലെങ്കിൽ
			*flags &= ~TRANSPORT_തള്ളുക_FORCE_IF_INCLUDES;
		തിരികെ 0;
	}

	തിരികെ git_default_config(k, v, ctx, ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ തള്ളുക_multiple(ഘടന string_list *list,
			 സ്ഥിരം ഘടന string_list *തള്ളുക_options,
			 പൂർണ്ണസംഖ്യ flags,
			 പൂർണ്ണസംഖ്യ tags,
			 സ്ഥിരം അക്ഷരം **refspecs,
			 പൂർണ്ണസംഖ്യ refspec_nr)
{
	പൂർണ്ണസംഖ്യ result = 0;
	size_t i;
	ഘടന strvec argv = STRVEC_INIT;

	strvec_തള്ളുക(&argv, "push");

	എങ്കിൽ (flags & TRANSPORT_തള്ളുക_FORCE)
		strvec_തള്ളുക(&argv, "--force");
	എങ്കിൽ (flags & TRANSPORT_തള്ളുക_DRY_RUN)
		strvec_തള്ളുക(&argv, "--dry-run");
	എങ്കിൽ (flags & TRANSPORT_തള്ളുക_PORCELAIN)
		strvec_തള്ളുക(&argv, "--porcelain");
	എങ്കിൽ (flags & TRANSPORT_തള്ളുക_PRUNE)
		strvec_തള്ളുക(&argv, "--prune");
	എങ്കിൽ (flags & TRANSPORT_തള്ളുക_NO_HOOK)
		strvec_തള്ളുക(&argv, "--no-verify");
	എങ്കിൽ (flags & TRANSPORT_തള്ളുക_FOLLOW_TAGS)
		strvec_തള്ളുക(&argv, "--follow-tags");
	എങ്കിൽ (flags & TRANSPORT_തള്ളുക_SET_UPSTREAM)
		strvec_തള്ളുക(&argv, "--set-upstream");
	എങ്കിൽ (flags & TRANSPORT_തള്ളുക_FORCE_IF_INCLUDES)
		strvec_തള്ളുക(&argv, "--force-if-includes");
	എങ്കിൽ (flags & TRANSPORT_തള്ളുക_ALL)
		strvec_തള്ളുക(&argv, "--all");
	എങ്കിൽ (flags & TRANSPORT_തള്ളുക_MIRROR)
		strvec_തള്ളുക(&argv, "--mirror");

	എങ്കിൽ (flags & TRANSPORT_തള്ളുക_CERT_ALWAYS)
		strvec_തള്ളുക(&argv, "--signed=yes");
	അല്ലെങ്കിൽ എങ്കിൽ (flags & TRANSPORT_തള്ളുക_CERT_IF_ASKED)
		strvec_തള്ളുക(&argv, "--signed=if-asked");
	എങ്കിൽ (!thin)
		strvec_തള്ളുക(&argv, "--no-thin");

	എങ്കിൽ (deleterefs)
		strvec_തള്ളുക(&argv, "--delete");

	എങ്കിൽ (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	എങ്കിൽ (verbosity >= 2)
		strvec_തള്ളുക(&argv, "-v");
	എങ്കിൽ (verbosity >= 1)
		strvec_തള്ളുക(&argv, "-v");
	അല്ലെങ്കിൽ എങ്കിൽ (verbosity < 0)
		strvec_തള്ളുക(&argv, "-q");
	എങ്കിൽ (progress > 0)
		strvec_തള്ളുക(&argv, "--progress");
	അല്ലെങ്കിൽ എങ്കിൽ (progress == 0)
		strvec_തള്ളുക(&argv, "--no-progress");

	എങ്കിൽ (family == TRANSPORT_FAMILY_IPV4)
		strvec_തള്ളുക(&argv, "--ipv4");
	അല്ലെങ്കിൽ എങ്കിൽ (family == TRANSPORT_FAMILY_IPV6)
		strvec_തള്ളുക(&argv, "--ipv6");

	എങ്കിൽ (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_തള്ളുക(&argv, "--recurse-submodules=check");
	അല്ലെങ്കിൽ എങ്കിൽ (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_തള്ളുക(&argv, "--recurse-submodules=on-demand");
	അല്ലെങ്കിൽ എങ്കിൽ (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_തള്ളുക(&argv, "--recurse-submodules=only");
	അല്ലെങ്കിൽ എങ്കിൽ (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_തള്ളുക(&argv, "--recurse-submodules=no");


	എങ്കിൽ (tags)
		strvec_തള്ളുക(&argv, "--tags");

	വേണ്ടി (i = 0; i < തള്ളുക_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     തള്ളുക_options->ഇനങ്ങൾ[i].string);

	വേണ്ടി (i = 0; i < cas.nr; i++) {
		എങ്കിൽ (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} അല്ലെങ്കിൽ എങ്കിൽ (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} അല്ലെങ്കിൽ {
			strvec_തള്ളുക(&argv, "--force-with-lease");
		}
	}

	വേണ്ടി (i = 0; i < list->nr; i++) {
		സ്ഥിരം അക്ഷരം *name = list->ഇനങ്ങൾ[i].string;
		ഘടന child_process cmd = CHILD_PROCESS_INIT;
		പൂർണ്ണസംഖ്യ j;

		strvec_pushv(&cmd.args, argv.v);
		strvec_തള്ളുക(&cmd.args, name);

		വേണ്ടി (j = 0; j < refspec_nr; j++)
			strvec_തള്ളുക(&cmd.args, refspecs[j]);

		എങ്കിൽ (verbosity >= 0)
			printf(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		എങ്കിൽ (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			result = 1;
		}
	}

	strvec_clear(&argv);
	തിരികെ result;
}

സ്ഥിതം ശൂന്യം die_if_repo_looks_like_ref(സ്ഥിരം അക്ഷരം *repo)
{
	സ്ഥിരം അക്ഷരം *slash = strchr(repo, '/');
	ഘടന strbuf name = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ code;

	എങ്കിൽ (!slash || !slash[1] || file_exists(repo))
		തിരികെ;

	strbuf_add(&name, repo, slash - repo);
	എങ്കിൽ (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		തിരികെ;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_തള്ളുക_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	exit(code);
}

പൂർണ്ണസംഖ്യ cmd_തള്ളുക(പൂർണ്ണസംഖ്യ argc,
	     സ്ഥിരം അക്ഷരം **argv,
	     സ്ഥിരം അക്ഷരം *prefix,
	     ഘടന repository *repository UNUSED)
{
	പൂർണ്ണസംഖ്യ flags = 0;
	പൂർണ്ണസംഖ്യ tags = 0;
	പൂർണ്ണസംഖ്യ തള്ളുക_cert = -1;
	പൂർണ്ണസംഖ്യ rc = 0;
	പൂർണ്ണസംഖ്യ base_flags;
	സ്ഥിരം അക്ഷരം *repo = ശൂന്യ;	/* default repository */
	ഘടന string_list തള്ളുക_options_cmdline = STRING_LIST_INIT_DUP;
	ഘടന string_list remote_group = STRING_LIST_INIT_DUP;
	ഘടന string_list *തള്ളുക_options;
	സ്ഥിരം ഘടന string_list_item *item;

	ഘടന option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_BIT( 0 , "all", &flags, N_("push all branches"), TRANSPORT_തള്ളുക_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_BIT( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_തള്ളുക_MIRROR|TRANSPORT_തള്ളുക_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &tags, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_BIT('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_തള്ളുക_DRY_RUN),
		OPT_BIT( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_തള്ളുക_PORCELAIN),
		OPT_BIT('f', "force", &flags, N_("force updates"), TRANSPORT_തള്ളുക_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_തള്ളുക_cas_option),
		OPT_BIT(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_തള്ളുക_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_BIT('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_തള്ളുക_SET_UPSTREAM),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_BIT(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_തള്ളുക_PRUNE),
		OPT_BIT(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_തള്ളുക_NO_HOOK),
		OPT_BIT(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_തള്ളുക_FOLLOW_TAGS),
		OPT_CALLBACK_F(0, "signed", &തള്ളുക_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_തള്ളുക_signed),
		OPT_BIT(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_തള്ളുക_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &തള്ളുക_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_END()
	};

	packet_trace_identity("push");
	repo_config(the_repository, git_തള്ളുക_config, &flags);
	argc = parse_options(argc, argv, prefix, options, തള്ളുക_usage, 0);
	തള്ളുക_options = (തള്ളുക_options_cmdline.nr
		? &തള്ളുക_options_cmdline
		: &തള്ളുക_options_config);
	set_തള്ളുക_cert_flags(&flags, തള്ളുക_cert);

	refspec_init_തള്ളുക(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  tags, "--tags",
				  flags & TRANSPORT_തള്ളുക_ALL, "--all/--branches",
				  flags & TRANSPORT_തള്ളുക_MIRROR, "--mirror");
	എങ്കിൽ (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	എങ്കിൽ (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	അല്ലെങ്കിൽ എങ്കിൽ (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	അല്ലെങ്കിൽ എങ്കിൽ (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	എങ്കിൽ (argc > 0)
		repo = argv[0];

	എങ്കിൽ (repo) {
		എങ്കിൽ (!add_remote_or_group(repo, &remote_group)) {
			ഘടന remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			എങ്കിൽ (advice_enabled(ADVICE_തള്ളുക_REPO_LOOKS_LIKE_REF))
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
			എങ്കിൽ (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_group, r->name);
		}
	} അല്ലെങ്കിൽ {
		ഘടന remote *r = pushremote_get(ശൂന്യ);
		എങ്കിൽ (!r)
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
		string_list_append(&remote_group, r->name);
	}

	എങ്കിൽ (!is_empty_cas(&cas) && (flags & TRANSPORT_തള്ളുക_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, തള്ളുക_options)
		എങ്കിൽ (strchr(item->string, '\n'))
			die(_("push options must not have new line characters"));

	എങ്കിൽ (remote_group.nr == 1) {
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
		base_flags = flags;
		{
			പൂർണ്ണസംഖ്യ inner_flags = base_flags;
			ഘടന remote *r = pushremote_get(remote_group.ഇനങ്ങൾ[0].string);
			എങ്കിൽ (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.ഇനങ്ങൾ[0].string);

			എങ്കിൽ (r->mirror)
				inner_flags |= (TRANSPORT_തള്ളുക_MIRROR|TRANSPORT_തള്ളുക_FORCE);

			എങ്കിൽ (inner_flags & TRANSPORT_തള്ളുക_ALL) {
				എങ്കിൽ (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			എങ്കിൽ (inner_flags & TRANSPORT_തള്ളുക_MIRROR) {
				എങ്കിൽ (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (ഘടന refspec) REFSPEC_INIT_തള്ളുക(the_hash_algo);

			എങ്കിൽ (tags)
				refspec_append(&rs, "refs/tags/*");
			എങ്കിൽ (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_തള്ളുക(inner_flags, തള്ളുക_options, r);
		}
	} അല്ലെങ്കിൽ {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		എങ്കിൽ (flags & TRANSPORT_തള്ളുക_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = തള്ളുക_multiple(&remote_group, തള്ളുക_options, flags,
				   tags,
				   argc > 1 ? argv + 1 : ശൂന്യ,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&തള്ളുക_options_cmdline, 0);
	string_list_clear(&തള്ളുക_options_config, 0);
	string_list_clear(&remote_group, 0);
	clear_cas_option(&cas);

	എങ്കിൽ (rc == -1)
		usage_with_options(തള്ളുക_usage, options);
	അല്ലെങ്കിൽ
		തിരികെ rc;
}
