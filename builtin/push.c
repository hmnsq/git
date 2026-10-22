/*
 * "git push"
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "branch.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "refspec.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "remote.h"
#అన్తర్భావ "transport.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "pkt-line.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "submodule-config.h"
#అన్తర్భావ "send-pack.h"
#అన్తర్భావ "trace2.h"
#అన్తర్భావ "color.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి నెట్టు_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	ఖాళీ,
};

స్థిర గణన git_colorbool నెట్టు_use_color = GIT_COLOR_UNKNOWN;
స్థిర అక్షరం నెట్టు_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

గణన color_నెట్టు {
	నెట్టు_COLOR_RESET = 0,
	నెట్టు_COLOR_ERROR = 1
};

స్థిర పూర్ణాంకం parse_నెట్టు_color_స్థానం(స్థిరరాశి అక్షరం *స్థానం)
{
	ఒకవేళ (!strcasecmp(స్థానం, "reset"))
		తిరిగి నెట్టు_COLOR_RESET;
	ఒకవేళ (!strcasecmp(స్థానం, "error"))
		తిరిగి నెట్టు_COLOR_ERROR;
	తిరిగి -1;
}

స్థిర స్థిరరాశి అక్షరం *నెట్టు_get_color(గణన color_నెట్టు ix)
{
	ఒకవేళ (want_color_stderr(నెట్టు_use_color))
		తిరిగి నెట్టు_colors[ix];
	తిరిగి "";
}

స్థిర పూర్ణాంకం thin = 1;
స్థిర పూర్ణాంకం deleterefs;
స్థిర స్థిరరాశి అక్షరం *receivepack;
స్థిర పూర్ణాంకం verbosity;
స్థిర పూర్ణాంకం పురోగతి = -1;
స్థిర పూర్ణాంకం recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
స్థిర గణన transport_family family;

స్థిర నిర్మాణం నెట్టు_cas_option cas;

స్థిర నిర్మాణం refspec rs;

స్థిర నిర్మాణం string_list నెట్టు_options_config = STRING_LIST_INIT_DUP;

స్థిర శూన్య refspec_append_mapped(నిర్మాణం refspec *refspec, స్థిరరాశి అక్షరం *ref,
				  నిర్మాణం remote *remote, నిర్మాణం ref *matched)
{
	స్థిరరాశి అక్షరం *branch_name;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	ఒకవేళ (remote->నెట్టు.nr) {
		నిర్మాణం refspec_అంశం query = {
			.src = matched->name,
		};

		ఒకవేళ (!refspec_కనుగొను_సరిపోలిక(&remote->నెట్టు, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			విడుదల(query.dst);
			తిరిగి;
		}
	}

	ఒకవేళ (cfg->నెట్టు_default == నెట్టు_DEFAULT_UPSTREAM &&
	    skip_ఉపసర్గ(matched->name, "refs/heads/", &branch_name)) {
		నిర్మాణం branch *branch = branch_get(branch_name);
		ఒకవేళ (branch->విలీనం_nr == 1 && branch->విలీనం[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->విలీనం[0]->src);
			తిరిగి;
		}
	}

	refspec_append(refspec, ref);
}

స్థిర శూన్య సమితి_refspecs(స్థిరరాశి అక్షరం **refs, పూర్ణాంకం nr, నిర్మాణం remote *remote)
{
	నిర్మాణం ref *local_refs = ఖాళీ;
	పూర్ణాంకం i;

	కొరకు (i = 0; i < nr; i++) {
		స్థిరరాశి అక్షరం *ref = refs[i];
		ఒకవేళ (!strcmp("tag", ref)) {
			ఒకవేళ (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			ఒకవేళ (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			లేకపోతే
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} లేకపోతే ఒకవేళ (deleterefs) {
			ఒకవేళ (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} లేకపోతే ఒకవేళ (!strchr(ref, ':')) {
			నిర్మాణం ref *matched = ఖాళీ;

			/* lazily grab local_refs */
			ఒకవేళ (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			ఒకవేళ (లెక్క_refspec_సరిపోలిక(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			లేకపోతే
				refspec_append_mapped(&rs, ref, remote, matched);
		} లేకపోతే
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

స్థిర NORETURN శూన్య die_నెట్టు_simple(నిర్మాణం branch *branch,
				     నిర్మాణం remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	స్థిరరాశి అక్షరం *advice_pushdefault_maybe = "";
	స్థిరరాశి అక్షరం *advice_automergesimple_maybe = "";
	స్థిరరాశి అక్షరం *short_upstream = branch->విలీనం[0]->src;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	skip_ఉపసర్గ(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	ఒకవేళ (cfg->నెట్టు_default == నెట్టు_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	ఒకవేళ (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

స్థిర స్థిరరాశి అక్షరం సందేశం_detached_తల_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

స్థిర స్థిరరాశి అక్షరం *get_upstream_ref(పూర్ణాంకం flags, నిర్మాణం branch *branch, స్థిరరాశి అక్షరం *remote_name)
{
	ఒకవేళ (branch->విలీనం_nr == 0 && (flags & TRANSPORT_నెట్టు_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		తిరిగి branch->refname;
	}

	ఒకవేళ (!branch->విలీనం_nr || !branch->విలీనం || !branch->remote_name) {
		స్థిరరాశి అక్షరం *advice_autosetup_maybe = "";
		ఒకవేళ (!(flags & TRANSPORT_నెట్టు_AUTO_UPSTREAM)) {
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
	ఒకవేళ (branch->విలీనం_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	తిరిగి branch->విలీనం[0]->src;
}

స్థిర శూన్య అమరిక_default_నెట్టు_refspecs(పూర్ణాంకం *flags, నిర్మాణం remote *remote)
{
	నిర్మాణం branch *branch;
	స్థిరరాశి అక్షరం *dst;
	పూర్ణాంకం same_remote;
	నిర్మాణం repo_config_విలువలు *cfg = repo_config_విలువలు(the_repository);

	ఎంపిక (cfg->నెట్టు_default) {
	స్థితి నెట్టు_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		తిరిగి;

	స్థితి నెట్టు_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		తిరిగి;
	అప్రమేయ:
		ఆపు;
	}

	branch = branch_get(ఖాళీ);
	ఒకవేళ (!branch)
		die(_(సందేశం_detached_తల_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, ఖాళీ));

	ఎంపిక (cfg->నెట్టు_default) {
	అప్రమేయ:
	స్థితి నెట్టు_DEFAULT_UNSPECIFIED:
	స్థితి నెట్టు_DEFAULT_SIMPLE:
		ఒకవేళ (!same_remote)
			ఆపు;
		ఒకవేళ (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_నెట్టు_simple(branch, remote);
		ఆపు;

	స్థితి నెట్టు_DEFAULT_UPSTREAM:
		ఒకవేళ (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		ఆపు;

	స్థితి నెట్టు_DEFAULT_CURRENT:
		ఆపు;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	ఒకవేళ ((*flags & TRANSPORT_నెట్టు_AUTO_UPSTREAM) && branch->విలీనం_nr == 0)
		*flags |= TRANSPORT_నెట్టు_సమితి_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

స్థిర స్థిరరాశి అక్షరం సందేశం_advice_pull_before_నెట్టు[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

స్థిర స్థిరరాశి అక్షరం సందేశం_advice_checkout_pull_నెట్టు[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

స్థిర స్థిరరాశి అక్షరం సందేశం_advice_ref_fetch_మొదటి[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

స్థిర స్థిరరాశి అక్షరం సందేశం_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

స్థిర స్థిరరాశి అక్షరం సందేశం_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

స్థిర స్థిరరాశి అక్షరం సందేశం_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

స్థిర శూన్య advise_pull_before_నెట్టు(శూన్య)
{
	ఒకవేళ (!advice_enabled(ADVICE_నెట్టు_NON_FF_CURRENT) || !advice_enabled(ADVICE_నెట్టు_UPDATE_REJECTED))
		తిరిగి;
	advise(_(సందేశం_advice_pull_before_నెట్టు));
}

స్థిర శూన్య advise_checkout_pull_నెట్టు(శూన్య)
{
	ఒకవేళ (!advice_enabled(ADVICE_నెట్టు_NON_FF_MATCHING) || !advice_enabled(ADVICE_నెట్టు_UPDATE_REJECTED))
		తిరిగి;
	advise(_(సందేశం_advice_checkout_pull_నెట్టు));
}

స్థిర శూన్య advise_ref_already_exists(శూన్య)
{
	ఒకవేళ (!advice_enabled(ADVICE_నెట్టు_ALREADY_EXISTS) || !advice_enabled(ADVICE_నెట్టు_UPDATE_REJECTED))
		తిరిగి;
	advise(_(సందేశం_advice_ref_already_exists));
}

స్థిర శూన్య advise_ref_fetch_మొదటి(శూన్య)
{
	ఒకవేళ (!advice_enabled(ADVICE_నెట్టు_FETCH_మొదటి) || !advice_enabled(ADVICE_నెట్టు_UPDATE_REJECTED))
		తిరిగి;
	advise(_(సందేశం_advice_ref_fetch_మొదటి));
}

స్థిర శూన్య advise_ref_needs_force(శూన్య)
{
	ఒకవేళ (!advice_enabled(ADVICE_నెట్టు_NEEDS_FORCE) || !advice_enabled(ADVICE_నెట్టు_UPDATE_REJECTED))
		తిరిగి;
	advise(_(సందేశం_advice_ref_needs_force));
}

స్థిర శూన్య advise_ref_needs_update(శూన్య)
{
	ఒకవేళ (!advice_enabled(ADVICE_నెట్టు_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_నెట్టు_UPDATE_REJECTED))
		తిరిగి;
	advise(_(సందేశం_advice_ref_needs_update));
}

స్థిర పూర్ణాంకం నెట్టు_with_options(నిర్మాణం transport *transport, నిర్మాణం refspec *rs,
			     పూర్ణాంకం flags)
{
	పూర్ణాంకం err;
	గుర్తులేని పూర్ణాంకం reject_reasons;
	అక్షరం *anon_url = transport_anonymize_url(transport->url);

	transport_సమితి_verbosity(transport, verbosity, పురోగతి);
	transport->family = family;

	ఒకవేళ (receivepack)
		transport_సమితి_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_సమితి_option(transport, TRANS_OPT_THIN, thin ? "yes" : ఖాళీ);

	ఒకవేళ (!is_ఖాళీలు_cas(&cas)) {
		ఒకవేళ (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	ఒకవేళ (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_నెట్టు(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	ఒకవేళ (err != 0) {
		fprintf(stderr, "%s", నెట్టు_get_color(నెట్టు_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", నెట్టు_get_color(నెట్టు_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	విడుదల(anon_url);
	ఒకవేళ (!err)
		తిరిగి 0;

	ఒకవేళ (reject_reasons & REJECT_NON_FF_తల) {
		advise_pull_before_నెట్టు();
	} లేకపోతే ఒకవేళ (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_నెట్టు();
	} లేకపోతే ఒకవేళ (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} లేకపోతే ఒకవేళ (reject_reasons & REJECT_FETCH_మొదటి) {
		advise_ref_fetch_మొదటి();
	} లేకపోతే ఒకవేళ (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} లేకపోతే ఒకవేళ (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	తిరిగి 1;
}

స్థిర పూర్ణాంకం do_నెట్టు(పూర్ణాంకం flags,
		   స్థిరరాశి నిర్మాణం string_list *నెట్టు_options,
		   నిర్మాణం remote *remote)
{
	పూర్ణాంకం errs;
	నిర్మాణం strvec *url;
	నిర్మాణం refspec *నెట్టు_refspec = &rs;

	ఒకవేళ (నెట్టు_options->nr)
		flags |= TRANSPORT_నెట్టు_OPTIONS;

	ఒకవేళ (!నెట్టు_refspec->nr && !(flags & TRANSPORT_నెట్టు_ALL)) {
		ఒకవేళ (remote->నెట్టు.nr) {
			నెట్టు_refspec = &remote->నెట్టు;
		} లేకపోతే ఒకవేళ (!(flags & TRANSPORT_నెట్టు_MIRROR))
			అమరిక_default_నెట్టు_refspecs(&flags, remote);
	}
	errs = 0;
	url = నెట్టు_url_యొక్క_remote(remote);
	కొరకు (కొలత_t i = 0; i < url->nr; i++) {
		నిర్మాణం transport *transport =
			transport_get(remote, url->v[i]);
		ఒకవేళ (flags & TRANSPORT_నెట్టు_OPTIONS)
			transport->నెట్టు_options = నెట్టు_options;
		ఒకవేళ (నెట్టు_with_options(transport, నెట్టు_refspec, flags))
			errs++;
	}
	తిరిగి !!errs;
}

స్థిర పూర్ణాంకం option_parse_recurse_submodules(స్థిరరాశి నిర్మాణం option *opt,
				   స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *recurse_submodules = opt->విలువ;

	ఒకవేళ (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	లేకపోతే {
		ఒకవేళ (!strcmp(arg, "only-is-on-demand")) {
			ఒకవేళ (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} లేకపోతే {
			*recurse_submodules = parse_నెట్టు_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	తిరిగి 0;
}

స్థిర శూన్య సమితి_నెట్టు_cert_flags(పూర్ణాంకం *flags, పూర్ణాంకం v)
{
	ఎంపిక (v) {
	స్థితి SEND_PACK_నెట్టు_CERT_NEVER:
		*flags &= ~(TRANSPORT_నెట్టు_CERT_ALWAYS | TRANSPORT_నెట్టు_CERT_IF_ASKED);
		ఆపు;
	స్థితి SEND_PACK_నెట్టు_CERT_ALWAYS:
		*flags |= TRANSPORT_నెట్టు_CERT_ALWAYS;
		*flags &= ~TRANSPORT_నెట్టు_CERT_IF_ASKED;
		ఆపు;
	స్థితి SEND_PACK_నెట్టు_CERT_IF_ASKED:
		*flags |= TRANSPORT_నెట్టు_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_నెట్టు_CERT_ALWAYS;
		ఆపు;
	}
}


స్థిర పూర్ణాంకం git_నెట్టు_config(స్థిరరాశి అక్షరం *k, స్థిరరాశి అక్షరం *v,
			   స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	స్థిరరాశి అక్షరం *స్థానం_name;
	పూర్ణాంకం *flags = cb;

	ఒకవేళ (!strcmp(k, "push.followtags")) {
		ఒకవేళ (git_config_bool(k, v))
			*flags |= TRANSPORT_నెట్టు_FOLLOW_ట్యాగులు;
		లేకపోతే
			*flags &= ~TRANSPORT_నెట్టు_FOLLOW_ట్యాగులు;
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (!strcmp(k, "push.autosetupremote")) {
		ఒకవేళ (git_config_bool(k, v))
			*flags |= TRANSPORT_నెట్టు_AUTO_UPSTREAM;
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (!strcmp(k, "push.gpgsign")) {
		ఎంపిక (git_parse_maybe_bool(v)) {
		స్థితి 0:
			సమితి_నెట్టు_cert_flags(flags, SEND_PACK_నెట్టు_CERT_NEVER);
			ఆపు;
		స్థితి 1:
			సమితి_నెట్టు_cert_flags(flags, SEND_PACK_నెట్టు_CERT_ALWAYS);
			ఆపు;
		అప్రమేయ:
			ఒకవేళ (!strcasecmp(v, "if-asked"))
				సమితి_నెట్టు_cert_flags(flags, SEND_PACK_నెట్టు_CERT_IF_ASKED);
			లేకపోతే
				తిరిగి error(_("invalid value for '%s'"), k);
		}
	} లేకపోతే ఒకవేళ (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_నెట్టు_recurse_submodules_arg(k, v);
	} లేకపోతే ఒకవేళ (!strcmp(k, "submodule.recurse")) {
		పూర్ణాంకం val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} లేకపోతే ఒకవేళ (!strcmp(k, "push.pushoption")) {
		తిరిగి parse_transport_option(k, v, &నెట్టు_options_config);
	} లేకపోతే ఒకవేళ (!strcmp(k, "color.push")) {
		నెట్టు_use_color = git_config_colorbool(k, v);
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(k, "color.push.", &స్థానం_name)) {
		పూర్ణాంకం స్థానం = parse_నెట్టు_color_స్థానం(స్థానం_name);
		ఒకవేళ (స్థానం < 0)
			తిరిగి 0;
		ఒకవేళ (!v)
			తిరిగి config_error_nonbool(k);
		తిరిగి color_parse(v, నెట్టు_colors[స్థానం]);
	} లేకపోతే ఒకవేళ (!strcmp(k, "push.useforceifincludes")) {
		ఒకవేళ (git_config_bool(k, v))
			*flags |= TRANSPORT_నెట్టు_FORCE_IF_INCLUDES;
		లేకపోతే
			*flags &= ~TRANSPORT_నెట్టు_FORCE_IF_INCLUDES;
		తిరిగి 0;
	}

	తిరిగి git_default_config(k, v, ctx, ఖాళీ);
}

స్థిర పూర్ణాంకం నెట్టు_గుణిజం(నిర్మాణం string_list *list,
			 స్థిరరాశి నిర్మాణం string_list *నెట్టు_options,
			 పూర్ణాంకం flags,
			 పూర్ణాంకం ట్యాగులు,
			 స్థిరరాశి అక్షరం **refspecs,
			 పూర్ణాంకం refspec_nr)
{
	పూర్ణాంకం ఫలితం = 0;
	కొలత_t i;
	నిర్మాణం strvec argv = STRVEC_INIT;

	strvec_నెట్టు(&argv, "push");

	ఒకవేళ (flags & TRANSPORT_నెట్టు_FORCE)
		strvec_నెట్టు(&argv, "--force");
	ఒకవేళ (flags & TRANSPORT_నెట్టు_DRY_నిరంతరం)
		strvec_నెట్టు(&argv, "--dry-run");
	ఒకవేళ (flags & TRANSPORT_నెట్టు_PORCELAIN)
		strvec_నెట్టు(&argv, "--porcelain");
	ఒకవేళ (flags & TRANSPORT_నెట్టు_PRUNE)
		strvec_నెట్టు(&argv, "--prune");
	ఒకవేళ (flags & TRANSPORT_నెట్టు_NO_HOOK)
		strvec_నెట్టు(&argv, "--no-verify");
	ఒకవేళ (flags & TRANSPORT_నెట్టు_FOLLOW_ట్యాగులు)
		strvec_నెట్టు(&argv, "--follow-tags");
	ఒకవేళ (flags & TRANSPORT_నెట్టు_సమితి_UPSTREAM)
		strvec_నెట్టు(&argv, "--set-upstream");
	ఒకవేళ (flags & TRANSPORT_నెట్టు_FORCE_IF_INCLUDES)
		strvec_నెట్టు(&argv, "--force-if-includes");
	ఒకవేళ (flags & TRANSPORT_నెట్టు_ALL)
		strvec_నెట్టు(&argv, "--all");
	ఒకవేళ (flags & TRANSPORT_నెట్టు_MIRROR)
		strvec_నెట్టు(&argv, "--mirror");

	ఒకవేళ (flags & TRANSPORT_నెట్టు_CERT_ALWAYS)
		strvec_నెట్టు(&argv, "--signed=yes");
	లేకపోతే ఒకవేళ (flags & TRANSPORT_నెట్టు_CERT_IF_ASKED)
		strvec_నెట్టు(&argv, "--signed=if-asked");
	ఒకవేళ (!thin)
		strvec_నెట్టు(&argv, "--no-thin");

	ఒకవేళ (deleterefs)
		strvec_నెట్టు(&argv, "--delete");

	ఒకవేళ (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	ఒకవేళ (verbosity >= 2)
		strvec_నెట్టు(&argv, "-v");
	ఒకవేళ (verbosity >= 1)
		strvec_నెట్టు(&argv, "-v");
	లేకపోతే ఒకవేళ (verbosity < 0)
		strvec_నెట్టు(&argv, "-q");
	ఒకవేళ (పురోగతి > 0)
		strvec_నెట్టు(&argv, "--progress");
	లేకపోతే ఒకవేళ (పురోగతి == 0)
		strvec_నెట్టు(&argv, "--no-progress");

	ఒకవేళ (family == TRANSPORT_FAMILY_IPV4)
		strvec_నెట్టు(&argv, "--ipv4");
	లేకపోతే ఒకవేళ (family == TRANSPORT_FAMILY_IPV6)
		strvec_నెట్టు(&argv, "--ipv6");

	ఒకవేళ (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_నెట్టు(&argv, "--recurse-submodules=check");
	లేకపోతే ఒకవేళ (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_నెట్టు(&argv, "--recurse-submodules=on-demand");
	లేకపోతే ఒకవేళ (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_నెట్టు(&argv, "--recurse-submodules=only");
	లేకపోతే ఒకవేళ (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_నెట్టు(&argv, "--recurse-submodules=no");


	ఒకవేళ (ట్యాగులు)
		strvec_నెట్టు(&argv, "--tags");

	కొరకు (i = 0; i < నెట్టు_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     నెట్టు_options->వస్తువులు[i].string);

	కొరకు (i = 0; i < cas.nr; i++) {
		ఒకవేళ (cas.నమోదు_2[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.నమోదు_2[i].refname);
		} లేకపోతే ఒకవేళ (!is_null_oid(&cas.నమోదు_2[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.నమోదు_2[i].refname,
				     oid_గమ్యం_hex(&cas.నమోదు_2[i].expect));
		} లేకపోతే {
			strvec_నెట్టు(&argv, "--force-with-lease");
		}
	}

	కొరకు (i = 0; i < list->nr; i++) {
		స్థిరరాశి అక్షరం *name = list->వస్తువులు[i].string;
		నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;
		పూర్ణాంకం j;

		strvec_pushv(&cmd.వాదనలు, argv.v);
		strvec_నెట్టు(&cmd.వాదనలు, name);

		కొరకు (j = 0; j < refspec_nr; j++)
			strvec_నెట్టు(&cmd.వాదనలు, refspecs[j]);

		ఒకవేళ (verbosity >= 0)
			రాయి(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		ఒకవేళ (నిరంతరం_command(&cmd)) {
			error(_("could not push to %s"), name);
			ఫలితం = 1;
		}
	}

	strvec_clear(&argv);
	తిరిగి ఫలితం;
}

స్థిర శూన్య die_if_repo_looks_like_ref(స్థిరరాశి అక్షరం *repo)
{
	స్థిరరాశి అక్షరం *slash = strchr(repo, '/');
	నిర్మాణం strbuf name = STRBUF_INIT;
	పూర్ణాంకం సంకేతం_2;

	ఒకవేళ (!slash || !slash[1] || file_exists(repo))
		తిరిగి;

	strbuf_కూడు(&name, repo, slash - repo);
	ఒకవేళ (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		తిరిగి;
	}

	సంకేతం_2 = die_సందేశం(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_నెట్టు_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	నిష్క్రమించు(సంకేతం_2);
}

పూర్ణాంకం cmd_నెట్టు(పూర్ణాంకం argc,
	     స్థిరరాశి అక్షరం **argv,
	     స్థిరరాశి అక్షరం *ఉపసర్గ,
	     నిర్మాణం repository *repository వాడని)
{
	పూర్ణాంకం flags = 0;
	పూర్ణాంకం ట్యాగులు = 0;
	పూర్ణాంకం నెట్టు_cert = -1;
	పూర్ణాంకం rc = 0;
	పూర్ణాంకం పునాది_flags;
	స్థిరరాశి అక్షరం *repo = ఖాళీ;	/* default repository */
	నిర్మాణం string_list నెట్టు_options_cmdline = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list remote_సమూహం = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list *నెట్టు_options;
	స్థిరరాశి నిర్మాణం string_list_అంశం *అంశం;

	నిర్మాణం option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_బిట్టు( 0 , "all", &flags, N_("push all branches"), TRANSPORT_నెట్టు_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_బిట్టు( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_నెట్టు_MIRROR|TRANSPORT_నెట్టు_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &ట్యాగులు, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_బిట్టు('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_నెట్టు_DRY_నిరంతరం),
		OPT_బిట్టు( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_నెట్టు_PORCELAIN),
		OPT_బిట్టు('f', "force", &flags, N_("force updates"), TRANSPORT_నెట్టు_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_నెట్టు_cas_option),
		OPT_బిట్టు(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_నెట్టు_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_బిట్టు('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_నెట్టు_సమితి_UPSTREAM),
		OPT_BOOL(0, "progress", &పురోగతి, N_("force progress reporting")),
		OPT_బిట్టు(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_నెట్టు_PRUNE),
		OPT_బిట్టు(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_నెట్టు_NO_HOOK),
		OPT_బిట్టు(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_నెట్టు_FOLLOW_ట్యాగులు),
		OPT_CALLBACK_F(0, "signed", &నెట్టు_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_నెట్టు_signed),
		OPT_బిట్టు(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_నెట్టు_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &నెట్టు_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_చివర()
	};

	packet_trace_తటస్థం("push");
	repo_config(the_repository, git_నెట్టు_config, &flags);
	argc = parse_options(argc, argv, ఉపసర్గ, options, నెట్టు_usage, 0);
	నెట్టు_options = (నెట్టు_options_cmdline.nr
		? &నెట్టు_options_cmdline
		: &నెట్టు_options_config);
	సమితి_నెట్టు_cert_flags(&flags, నెట్టు_cert);

	refspec_init_నెట్టు(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  ట్యాగులు, "--tags",
				  flags & TRANSPORT_నెట్టు_ALL, "--all/--branches",
				  flags & TRANSPORT_నెట్టు_MIRROR, "--mirror");
	ఒకవేళ (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	ఒకవేళ (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	లేకపోతే ఒకవేళ (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	లేకపోతే ఒకవేళ (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	ఒకవేళ (argc > 0)
		repo = argv[0];

	ఒకవేళ (repo) {
		ఒకవేళ (!కూడు_remote_or_సమూహం(repo, &remote_సమూహం)) {
			నిర్మాణం remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			ఒకవేళ (advice_enabled(ADVICE_నెట్టు_REPO_LOOKS_LIKE_REF))
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
			ఒకవేళ (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_సమూహం, r->name);
		}
	} లేకపోతే {
		నిర్మాణం remote *r = pushremote_get(ఖాళీ);
		ఒకవేళ (!r)
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
		string_list_append(&remote_సమూహం, r->name);
	}

	ఒకవేళ (!is_ఖాళీలు_cas(&cas) && (flags & TRANSPORT_నెట్టు_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_అంశం(అంశం, నెట్టు_options)
		ఒకవేళ (strchr(అంశం->string, '\n'))
			die(_("push options must not have new line characters"));

	ఒకవేళ (remote_సమూహం.nr == 1) {
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
		పునాది_flags = flags;
		{
			పూర్ణాంకం inner_flags = పునాది_flags;
			నిర్మాణం remote *r = pushremote_get(remote_సమూహం.వస్తువులు[0].string);
			ఒకవేళ (!r)
				die(_("no such remote or remote group: %s"),
				    remote_సమూహం.వస్తువులు[0].string);

			ఒకవేళ (r->mirror)
				inner_flags |= (TRANSPORT_నెట్టు_MIRROR|TRANSPORT_నెట్టు_FORCE);

			ఒకవేళ (inner_flags & TRANSPORT_నెట్టు_ALL) {
				ఒకవేళ (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			ఒకవేళ (inner_flags & TRANSPORT_నెట్టు_MIRROR) {
				ఒకవేళ (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (నిర్మాణం refspec) REFSPEC_INIT_నెట్టు(the_hash_algo);

			ఒకవేళ (ట్యాగులు)
				refspec_append(&rs, "refs/tags/*");
			ఒకవేళ (argc > 0)
				సమితి_refspecs(argv + 1, argc - 1, r);

			rc = do_నెట్టు(inner_flags, నెట్టు_options, r);
		}
	} లేకపోతే {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		ఒకవేళ (flags & TRANSPORT_నెట్టు_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = నెట్టు_గుణిజం(&remote_సమూహం, నెట్టు_options, flags,
				   ట్యాగులు,
				   argc > 1 ? argv + 1 : ఖాళీ,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&నెట్టు_options_cmdline, 0);
	string_list_clear(&నెట్టు_options_config, 0);
	string_list_clear(&remote_సమూహం, 0);
	clear_cas_option(&cas);

	ఒకవేళ (rc == -1)
		usage_with_options(నెట్టు_usage, options);
	లేకపోతే
		తిరిగి rc;
}
