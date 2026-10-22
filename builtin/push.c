/*
 * "git push"
 */

#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "refspec.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "remote.h"
#अन्तर्भाव "transport.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "pkt-line.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "submodule-config.h"
#अन्तर्भाव "send-pack.h"
#अन्तर्भाव "trace2.h"
#अन्तर्भाव "color.h"

स्थिर स्थिरांक अक्षर * स्थिरांक ढकला_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	रिक्त,
};

स्थिर गणना git_colorbool ढकला_use_color = GIT_COLOR_UNKNOWN;
स्थिर अक्षर ढकला_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

गणना color_ढकला {
	ढकला_COLOR_RESET = 0,
	ढकला_COLOR_ERROR = 1
};

स्थिर पूर्णांक parse_ढकला_color_slot(स्थिरांक अक्षर *slot)
{
	जर (!strcasecmp(slot, "reset"))
		परत ढकला_COLOR_RESET;
	जर (!strcasecmp(slot, "error"))
		परत ढकला_COLOR_ERROR;
	परत -1;
}

स्थिर स्थिरांक अक्षर *ढकला_get_color(गणना color_ढकला ix)
{
	जर (want_color_stderr(ढकला_use_color))
		परत ढकला_colors[ix];
	परत "";
}

स्थिर पूर्णांक thin = 1;
स्थिर पूर्णांक deleterefs;
स्थिर स्थिरांक अक्षर *receivepack;
स्थिर पूर्णांक verbosity;
स्थिर पूर्णांक progress = -1;
स्थिर पूर्णांक recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
स्थिर गणना transport_family family;

स्थिर संरचना ढकला_cas_option cas;

स्थिर संरचना refspec rs;

स्थिर संरचना string_list ढकला_options_config = STRING_LIST_INIT_DUP;

स्थिर शून्य refspec_append_mapped(संरचना refspec *refspec, स्थिरांक अक्षर *ref,
				  संरचना remote *remote, संरचना ref *matched)
{
	स्थिरांक अक्षर *branch_name;
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);

	जर (remote->ढकला.nr) {
		संरचना refspec_item query = {
			.src = matched->name,
		};

		जर (!refspec_find_match(&remote->ढकला, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			मुक्त(query.dst);
			परत;
		}
	}

	जर (cfg->ढकला_default == ढकला_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		संरचना branch *branch = branch_get(branch_name);
		जर (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			परत;
		}
	}

	refspec_append(refspec, ref);
}

स्थिर शून्य set_refspecs(स्थिरांक अक्षर **refs, पूर्णांक nr, संरचना remote *remote)
{
	संरचना ref *local_refs = रिक्त;
	पूर्णांक i;

	साठी (i = 0; i < nr; i++) {
		स्थिरांक अक्षर *ref = refs[i];
		जर (!म_भेद("tag", ref)) {
			जर (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			जर (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			अन्यथा
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} अन्यथा जर (deleterefs) {
			जर (म_अक्षर(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} अन्यथा जर (!म_अक्षर(ref, ':')) {
			संरचना ref *matched = रिक्त;

			/* lazily grab local_refs */
			जर (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			जर (मोजणी_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			अन्यथा
				refspec_append_mapped(&rs, ref, remote, matched);
		} अन्यथा
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

स्थिर NORETURN शून्य die_ढकला_simple(संरचना branch *branch,
				     संरचना remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	स्थिरांक अक्षर *advice_pushdefault_maybe = "";
	स्थिरांक अक्षर *advice_automergesimple_maybe = "";
	स्थिरांक अक्षर *short_upstream = branch->merge[0]->src;
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);

	skip_prefix(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	जर (cfg->ढकला_default == ढकला_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	जर (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

स्थिर स्थिरांक अक्षर message_detached_head_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

स्थिर स्थिरांक अक्षर *get_upstream_ref(पूर्णांक flags, संरचना branch *branch, स्थिरांक अक्षर *remote_name)
{
	जर (branch->merge_nr == 0 && (flags & TRANSPORT_ढकला_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		परत branch->refname;
	}

	जर (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		स्थिरांक अक्षर *advice_autosetup_maybe = "";
		जर (!(flags & TRANSPORT_ढकला_AUTO_UPSTREAM)) {
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
	जर (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	परत branch->merge[0]->src;
}

स्थिर शून्य setup_default_ढकला_refspecs(पूर्णांक *flags, संरचना remote *remote)
{
	संरचना branch *branch;
	स्थिरांक अक्षर *dst;
	पूर्णांक same_remote;
	संरचना repo_config_मूल्ये *cfg = repo_config_मूल्ये(the_repository);

	निवड (cfg->ढकला_default) {
	स्थिती ढकला_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		परत;

	स्थिती ढकला_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		परत;
	पूर्वनिर्धारित:
		थांबा;
	}

	branch = branch_get(रिक्त);
	जर (!branch)
		die(_(message_detached_head_die), remote->name);

	dst = branch->refname;
	same_remote = !म_भेद(remote->name, remote_for_branch(branch, रिक्त));

	निवड (cfg->ढकला_default) {
	पूर्वनिर्धारित:
	स्थिती ढकला_DEFAULT_UNSPECIFIED:
	स्थिती ढकला_DEFAULT_SIMPLE:
		जर (!same_remote)
			थांबा;
		जर (म_भेद(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_ढकला_simple(branch, remote);
		थांबा;

	स्थिती ढकला_DEFAULT_UPSTREAM:
		जर (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		थांबा;

	स्थिती ढकला_DEFAULT_CURRENT:
		थांबा;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	जर ((*flags & TRANSPORT_ढकला_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_ढकला_SET_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

स्थिर स्थिरांक अक्षर message_advice_pull_before_ढकला[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

स्थिर स्थिरांक अक्षर message_advice_checkout_pull_ढकला[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

स्थिर स्थिरांक अक्षर message_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

स्थिर स्थिरांक अक्षर message_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

स्थिर स्थिरांक अक्षर message_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

स्थिर स्थिरांक अक्षर message_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

स्थिर शून्य advise_pull_before_ढकला(शून्य)
{
	जर (!advice_enabled(ADVICE_ढकला_NON_FF_CURRENT) || !advice_enabled(ADVICE_ढकला_UPDATE_REJECTED))
		परत;
	advise(_(message_advice_pull_before_ढकला));
}

स्थिर शून्य advise_checkout_pull_ढकला(शून्य)
{
	जर (!advice_enabled(ADVICE_ढकला_NON_FF_MATCHING) || !advice_enabled(ADVICE_ढकला_UPDATE_REJECTED))
		परत;
	advise(_(message_advice_checkout_pull_ढकला));
}

स्थिर शून्य advise_ref_already_exists(शून्य)
{
	जर (!advice_enabled(ADVICE_ढकला_ALREADY_EXISTS) || !advice_enabled(ADVICE_ढकला_UPDATE_REJECTED))
		परत;
	advise(_(message_advice_ref_already_exists));
}

स्थिर शून्य advise_ref_fetch_first(शून्य)
{
	जर (!advice_enabled(ADVICE_ढकला_FETCH_FIRST) || !advice_enabled(ADVICE_ढकला_UPDATE_REJECTED))
		परत;
	advise(_(message_advice_ref_fetch_first));
}

स्थिर शून्य advise_ref_needs_force(शून्य)
{
	जर (!advice_enabled(ADVICE_ढकला_NEEDS_FORCE) || !advice_enabled(ADVICE_ढकला_UPDATE_REJECTED))
		परत;
	advise(_(message_advice_ref_needs_force));
}

स्थिर शून्य advise_ref_needs_update(शून्य)
{
	जर (!advice_enabled(ADVICE_ढकला_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_ढकला_UPDATE_REJECTED))
		परत;
	advise(_(message_advice_ref_needs_update));
}

स्थिर पूर्णांक ढकला_with_options(संरचना transport *transport, संरचना refspec *rs,
			     पूर्णांक flags)
{
	पूर्णांक err;
	अचिन्हित पूर्णांक reject_reasons;
	अक्षर *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	जर (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : रिक्त);

	जर (!is_empty_cas(&cas)) {
		जर (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	जर (verbosity > 0)
		ख_लिखो(मानक_त्रुटि, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_ढकला(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	जर (err != 0) {
		ख_लिखो(मानक_त्रुटि, "%s", ढकला_get_color(ढकला_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		ख_लिखो(मानक_त्रुटि, "%s", ढकला_get_color(ढकला_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	मुक्त(anon_url);
	जर (!err)
		परत 0;

	जर (reject_reasons & REJECT_NON_FF_HEAD) {
		advise_pull_before_ढकला();
	} अन्यथा जर (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_ढकला();
	} अन्यथा जर (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} अन्यथा जर (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} अन्यथा जर (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} अन्यथा जर (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	परत 1;
}

स्थिर पूर्णांक do_ढकला(पूर्णांक flags,
		   स्थिरांक संरचना string_list *ढकला_options,
		   संरचना remote *remote)
{
	पूर्णांक errs;
	संरचना strvec *url;
	संरचना refspec *ढकला_refspec = &rs;

	जर (ढकला_options->nr)
		flags |= TRANSPORT_ढकला_OPTIONS;

	जर (!ढकला_refspec->nr && !(flags & TRANSPORT_ढकला_ALL)) {
		जर (remote->ढकला.nr) {
			ढकला_refspec = &remote->ढकला;
		} अन्यथा जर (!(flags & TRANSPORT_ढकला_MIRROR))
			setup_default_ढकला_refspecs(&flags, remote);
	}
	errs = 0;
	url = ढकला_url_of_remote(remote);
	साठी (माप_प्रकार i = 0; i < url->nr; i++) {
		संरचना transport *transport =
			transport_get(remote, url->v[i]);
		जर (flags & TRANSPORT_ढकला_OPTIONS)
			transport->ढकला_options = ढकला_options;
		जर (ढकला_with_options(transport, ढकला_refspec, flags))
			errs++;
	}
	परत !!errs;
}

स्थिर पूर्णांक option_parse_recurse_submodules(स्थिरांक संरचना option *opt,
				   स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *recurse_submodules = opt->मूल्य;

	जर (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	अन्यथा {
		जर (!म_भेद(arg, "only-is-on-demand")) {
			जर (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} अन्यथा {
			*recurse_submodules = parse_ढकला_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	परत 0;
}

स्थिर शून्य set_ढकला_cert_flags(पूर्णांक *flags, पूर्णांक v)
{
	निवड (v) {
	स्थिती SEND_PACK_ढकला_CERT_NEVER:
		*flags &= ~(TRANSPORT_ढकला_CERT_ALWAYS | TRANSPORT_ढकला_CERT_IF_ASKED);
		थांबा;
	स्थिती SEND_PACK_ढकला_CERT_ALWAYS:
		*flags |= TRANSPORT_ढकला_CERT_ALWAYS;
		*flags &= ~TRANSPORT_ढकला_CERT_IF_ASKED;
		थांबा;
	स्थिती SEND_PACK_ढकला_CERT_IF_ASKED:
		*flags |= TRANSPORT_ढकला_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_ढकला_CERT_ALWAYS;
		थांबा;
	}
}


स्थिर पूर्णांक git_ढकला_config(स्थिरांक अक्षर *k, स्थिरांक अक्षर *v,
			   स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	स्थिरांक अक्षर *slot_name;
	पूर्णांक *flags = cb;

	जर (!म_भेद(k, "push.followtags")) {
		जर (git_config_bool(k, v))
			*flags |= TRANSPORT_ढकला_FOLLOW_TAGS;
		अन्यथा
			*flags &= ~TRANSPORT_ढकला_FOLLOW_TAGS;
		परत 0;
	} अन्यथा जर (!म_भेद(k, "push.autosetupremote")) {
		जर (git_config_bool(k, v))
			*flags |= TRANSPORT_ढकला_AUTO_UPSTREAM;
		परत 0;
	} अन्यथा जर (!म_भेद(k, "push.gpgsign")) {
		निवड (git_parse_maybe_bool(v)) {
		स्थिती 0:
			set_ढकला_cert_flags(flags, SEND_PACK_ढकला_CERT_NEVER);
			थांबा;
		स्थिती 1:
			set_ढकला_cert_flags(flags, SEND_PACK_ढकला_CERT_ALWAYS);
			थांबा;
		पूर्वनिर्धारित:
			जर (!strcasecmp(v, "if-asked"))
				set_ढकला_cert_flags(flags, SEND_PACK_ढकला_CERT_IF_ASKED);
			अन्यथा
				परत error(_("invalid value for '%s'"), k);
		}
	} अन्यथा जर (!म_भेद(k, "push.recursesubmodules")) {
		recurse_submodules = parse_ढकला_recurse_submodules_arg(k, v);
	} अन्यथा जर (!म_भेद(k, "submodule.recurse")) {
		पूर्णांक val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} अन्यथा जर (!म_भेद(k, "push.pushoption")) {
		परत parse_transport_option(k, v, &ढकला_options_config);
	} अन्यथा जर (!म_भेद(k, "color.push")) {
		ढकला_use_color = git_config_colorbool(k, v);
		परत 0;
	} अन्यथा जर (skip_prefix(k, "color.push.", &slot_name)) {
		पूर्णांक slot = parse_ढकला_color_slot(slot_name);
		जर (slot < 0)
			परत 0;
		जर (!v)
			परत config_error_nonbool(k);
		परत color_parse(v, ढकला_colors[slot]);
	} अन्यथा जर (!म_भेद(k, "push.useforceifincludes")) {
		जर (git_config_bool(k, v))
			*flags |= TRANSPORT_ढकला_FORCE_IF_INCLUDES;
		अन्यथा
			*flags &= ~TRANSPORT_ढकला_FORCE_IF_INCLUDES;
		परत 0;
	}

	परत git_default_config(k, v, ctx, रिक्त);
}

स्थिर पूर्णांक ढकला_multiple(संरचना string_list *list,
			 स्थिरांक संरचना string_list *ढकला_options,
			 पूर्णांक flags,
			 पूर्णांक tags,
			 स्थिरांक अक्षर **refspecs,
			 पूर्णांक refspec_nr)
{
	पूर्णांक result = 0;
	माप_प्रकार i;
	संरचना strvec argv = STRVEC_INIT;

	strvec_ढकला(&argv, "push");

	जर (flags & TRANSPORT_ढकला_FORCE)
		strvec_ढकला(&argv, "--force");
	जर (flags & TRANSPORT_ढकला_DRY_RUN)
		strvec_ढकला(&argv, "--dry-run");
	जर (flags & TRANSPORT_ढकला_PORCELAIN)
		strvec_ढकला(&argv, "--porcelain");
	जर (flags & TRANSPORT_ढकला_PRUNE)
		strvec_ढकला(&argv, "--prune");
	जर (flags & TRANSPORT_ढकला_NO_HOOK)
		strvec_ढकला(&argv, "--no-verify");
	जर (flags & TRANSPORT_ढकला_FOLLOW_TAGS)
		strvec_ढकला(&argv, "--follow-tags");
	जर (flags & TRANSPORT_ढकला_SET_UPSTREAM)
		strvec_ढकला(&argv, "--set-upstream");
	जर (flags & TRANSPORT_ढकला_FORCE_IF_INCLUDES)
		strvec_ढकला(&argv, "--force-if-includes");
	जर (flags & TRANSPORT_ढकला_ALL)
		strvec_ढकला(&argv, "--all");
	जर (flags & TRANSPORT_ढकला_MIRROR)
		strvec_ढकला(&argv, "--mirror");

	जर (flags & TRANSPORT_ढकला_CERT_ALWAYS)
		strvec_ढकला(&argv, "--signed=yes");
	अन्यथा जर (flags & TRANSPORT_ढकला_CERT_IF_ASKED)
		strvec_ढकला(&argv, "--signed=if-asked");
	जर (!thin)
		strvec_ढकला(&argv, "--no-thin");

	जर (deleterefs)
		strvec_ढकला(&argv, "--delete");

	जर (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	जर (verbosity >= 2)
		strvec_ढकला(&argv, "-v");
	जर (verbosity >= 1)
		strvec_ढकला(&argv, "-v");
	अन्यथा जर (verbosity < 0)
		strvec_ढकला(&argv, "-q");
	जर (progress > 0)
		strvec_ढकला(&argv, "--progress");
	अन्यथा जर (progress == 0)
		strvec_ढकला(&argv, "--no-progress");

	जर (family == TRANSPORT_FAMILY_IPV4)
		strvec_ढकला(&argv, "--ipv4");
	अन्यथा जर (family == TRANSPORT_FAMILY_IPV6)
		strvec_ढकला(&argv, "--ipv6");

	जर (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_ढकला(&argv, "--recurse-submodules=check");
	अन्यथा जर (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ढकला(&argv, "--recurse-submodules=on-demand");
	अन्यथा जर (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_ढकला(&argv, "--recurse-submodules=only");
	अन्यथा जर (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ढकला(&argv, "--recurse-submodules=no");


	जर (tags)
		strvec_ढकला(&argv, "--tags");

	साठी (i = 0; i < ढकला_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     ढकला_options->वस्तुसंच[i].string);

	साठी (i = 0; i < cas.nr; i++) {
		जर (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} अन्यथा जर (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} अन्यथा {
			strvec_ढकला(&argv, "--force-with-lease");
		}
	}

	साठी (i = 0; i < list->nr; i++) {
		स्थिरांक अक्षर *name = list->वस्तुसंच[i].string;
		संरचना child_process cmd = CHILD_PROCESS_INIT;
		पूर्णांक j;

		strvec_pushv(&cmd.args, argv.v);
		strvec_ढकला(&cmd.args, name);

		साठी (j = 0; j < refspec_nr; j++)
			strvec_ढकला(&cmd.args, refspecs[j]);

		जर (verbosity >= 0)
			म_लिखो(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		जर (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			result = 1;
		}
	}

	strvec_clear(&argv);
	परत result;
}

स्थिर शून्य die_if_repo_looks_like_ref(स्थिरांक अक्षर *repo)
{
	स्थिरांक अक्षर *slash = म_अक्षर(repo, '/');
	संरचना strbuf name = STRBUF_INIT;
	पूर्णांक code;

	जर (!slash || !slash[1] || file_exists(repo))
		परत;

	strbuf_add(&name, repo, slash - repo);
	जर (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		परत;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_ढकला_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	निकास(code);
}

पूर्णांक cmd_ढकला(पूर्णांक argc,
	     स्थिरांक अक्षर **argv,
	     स्थिरांक अक्षर *prefix,
	     संरचना repository *repository UNUSED)
{
	पूर्णांक flags = 0;
	पूर्णांक tags = 0;
	पूर्णांक ढकला_cert = -1;
	पूर्णांक rc = 0;
	पूर्णांक base_flags;
	स्थिरांक अक्षर *repo = रिक्त;	/* default repository */
	संरचना string_list ढकला_options_cmdline = STRING_LIST_INIT_DUP;
	संरचना string_list remote_group = STRING_LIST_INIT_DUP;
	संरचना string_list *ढकला_options;
	स्थिरांक संरचना string_list_item *item;

	संरचना option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_BIT( 0 , "all", &flags, N_("push all branches"), TRANSPORT_ढकला_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_BIT( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_ढकला_MIRROR|TRANSPORT_ढकला_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &tags, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_BIT('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_ढकला_DRY_RUN),
		OPT_BIT( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_ढकला_PORCELAIN),
		OPT_BIT('f', "force", &flags, N_("force updates"), TRANSPORT_ढकला_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_ढकला_cas_option),
		OPT_BIT(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_ढकला_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_BIT('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_ढकला_SET_UPSTREAM),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_BIT(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_ढकला_PRUNE),
		OPT_BIT(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_ढकला_NO_HOOK),
		OPT_BIT(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_ढकला_FOLLOW_TAGS),
		OPT_CALLBACK_F(0, "signed", &ढकला_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_ढकला_signed),
		OPT_BIT(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_ढकला_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &ढकला_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_END()
	};

	packet_trace_identity("push");
	repo_config(the_repository, git_ढकला_config, &flags);
	argc = parse_options(argc, argv, prefix, options, ढकला_usage, 0);
	ढकला_options = (ढकला_options_cmdline.nr
		? &ढकला_options_cmdline
		: &ढकला_options_config);
	set_ढकला_cert_flags(&flags, ढकला_cert);

	refspec_init_ढकला(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  tags, "--tags",
				  flags & TRANSPORT_ढकला_ALL, "--all/--branches",
				  flags & TRANSPORT_ढकला_MIRROR, "--mirror");
	जर (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	जर (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	अन्यथा जर (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	अन्यथा जर (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	जर (argc > 0)
		repo = argv[0];

	जर (repo) {
		जर (!add_remote_or_group(repo, &remote_group)) {
			संरचना remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			जर (advice_enabled(ADVICE_ढकला_REPO_LOOKS_LIKE_REF))
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
			जर (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_group, r->name);
		}
	} अन्यथा {
		संरचना remote *r = pushremote_get(रिक्त);
		जर (!r)
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

	जर (!is_empty_cas(&cas) && (flags & TRANSPORT_ढकला_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, ढकला_options)
		जर (म_अक्षर(item->string, '\n'))
			die(_("push options must not have new line characters"));

	जर (remote_group.nr == 1) {
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
			पूर्णांक inner_flags = base_flags;
			संरचना remote *r = pushremote_get(remote_group.वस्तुसंच[0].string);
			जर (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.वस्तुसंच[0].string);

			जर (r->mirror)
				inner_flags |= (TRANSPORT_ढकला_MIRROR|TRANSPORT_ढकला_FORCE);

			जर (inner_flags & TRANSPORT_ढकला_ALL) {
				जर (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			जर (inner_flags & TRANSPORT_ढकला_MIRROR) {
				जर (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (संरचना refspec) REFSPEC_INIT_ढकला(the_hash_algo);

			जर (tags)
				refspec_append(&rs, "refs/tags/*");
			जर (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_ढकला(inner_flags, ढकला_options, r);
		}
	} अन्यथा {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		जर (flags & TRANSPORT_ढकला_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = ढकला_multiple(&remote_group, ढकला_options, flags,
				   tags,
				   argc > 1 ? argv + 1 : रिक्त,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&ढकला_options_cmdline, 0);
	string_list_clear(&ढकला_options_config, 0);
	string_list_clear(&remote_group, 0);
	clear_cas_option(&cas);

	जर (rc == -1)
		usage_with_options(ढकला_usage, options);
	अन्यथा
		परत rc;
}
