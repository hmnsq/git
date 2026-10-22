/*
 * "git push"
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "advice.h"
#समावेश "branch.h"
#समावेश "config.h"
#समावेश "dir.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "refspec.h"
#समावेश "run-command.h"
#समावेश "remote.h"
#समावेश "transport.h"
#समावेश "parse-options.h"
#समावेश "pkt-line.h"
#समावेश "submodule.h"
#समावेश "submodule-config.h"
#समावेश "send-pack.h"
#समावेश "trace2.h"
#समावेश "color.h"

अटल स्थिर अक्षर * स्थिर धकेलो_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	शून्य,
};

अटल क्रमागत git_colorbool धकेलो_use_color = GIT_COLOR_UNKNOWN;
अटल अक्षर धकेलो_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

क्रमागत color_धकेलो {
	धकेलो_COLOR_RESET = 0,
	धकेलो_COLOR_ERROR = 1
};

अटल पूर्णांक parse_धकेलो_color_slot(स्थिर अक्षर *slot)
{
	अगर (!strcasecmp(slot, "reset"))
		वापस धकेलो_COLOR_RESET;
	अगर (!strcasecmp(slot, "error"))
		वापस धकेलो_COLOR_ERROR;
	वापस -1;
}

अटल स्थिर अक्षर *धकेलो_get_color(क्रमागत color_धकेलो ix)
{
	अगर (want_color_stderr(धकेलो_use_color))
		वापस धकेलो_colors[ix];
	वापस "";
}

अटल पूर्णांक thin = 1;
अटल पूर्णांक deleterefs;
अटल स्थिर अक्षर *receivepack;
अटल पूर्णांक verbosity;
अटल पूर्णांक progress = -1;
अटल पूर्णांक recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
अटल क्रमागत transport_family family;

अटल काष्ठा धकेलो_cas_option cas;

अटल काष्ठा refspec rs;

अटल काष्ठा string_list धकेलो_options_config = STRING_LIST_INIT_DUP;

अटल व्योम refspec_append_mapped(काष्ठा refspec *refspec, स्थिर अक्षर *ref,
				  काष्ठा remote *remote, काष्ठा ref *matched)
{
	स्थिर अक्षर *branch_name;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	अगर (remote->धकेलो.nr) {
		काष्ठा refspec_item query = {
			.src = matched->name,
		};

		अगर (!refspec_find_match(&remote->धकेलो, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			मुक्त(query.dst);
			वापस;
		}
	}

	अगर (cfg->धकेलो_default == धकेलो_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		काष्ठा branch *branch = branch_get(branch_name);
		अगर (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			वापस;
		}
	}

	refspec_append(refspec, ref);
}

अटल व्योम set_refspecs(स्थिर अक्षर **refs, पूर्णांक nr, काष्ठा remote *remote)
{
	काष्ठा ref *local_refs = शून्य;
	पूर्णांक i;

	क्रम (i = 0; i < nr; i++) {
		स्थिर अक्षर *ref = refs[i];
		अगर (!म_भेद("tag", ref)) {
			अगर (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			अगर (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			अन्यथा
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} अन्यथा अगर (deleterefs) {
			अगर (म_अक्षर(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} अन्यथा अगर (!म_अक्षर(ref, ':')) {
			काष्ठा ref *matched = शून्य;

			/* lazily grab local_refs */
			अगर (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			अगर (गिनती_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			अन्यथा
				refspec_append_mapped(&rs, ref, remote, matched);
		} अन्यथा
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

अटल NORETURN व्योम die_धकेलो_simple(काष्ठा branch *branch,
				     काष्ठा remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	स्थिर अक्षर *advice_pushdefault_maybe = "";
	स्थिर अक्षर *advice_automergesimple_maybe = "";
	स्थिर अक्षर *short_upstream = branch->merge[0]->src;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	skip_prefix(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	अगर (cfg->धकेलो_default == धकेलो_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	अगर (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

अटल स्थिर अक्षर message_detached_शीर्ष_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

अटल स्थिर अक्षर *get_upstream_ref(पूर्णांक flags, काष्ठा branch *branch, स्थिर अक्षर *remote_name)
{
	अगर (branch->merge_nr == 0 && (flags & TRANSPORT_धकेलो_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		वापस branch->refname;
	}

	अगर (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		स्थिर अक्षर *advice_autosetup_maybe = "";
		अगर (!(flags & TRANSPORT_धकेलो_AUTO_UPSTREAM)) {
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
	अगर (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	वापस branch->merge[0]->src;
}

अटल व्योम setup_default_धकेलो_refspecs(पूर्णांक *flags, काष्ठा remote *remote)
{
	काष्ठा branch *branch;
	स्थिर अक्षर *dst;
	पूर्णांक same_remote;
	काष्ठा repo_config_मूल्यावली *cfg = repo_config_मूल्यावली(the_repository);

	चयन (cfg->धकेलो_default) {
	हाल धकेलो_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		वापस;

	हाल धकेलो_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		वापस;
	शेष:
		अवरोध;
	}

	branch = branch_get(शून्य);
	अगर (!branch)
		die(_(message_detached_शीर्ष_die), remote->name);

	dst = branch->refname;
	same_remote = !म_भेद(remote->name, remote_for_branch(branch, शून्य));

	चयन (cfg->धकेलो_default) {
	शेष:
	हाल धकेलो_DEFAULT_UNSPECIFIED:
	हाल धकेलो_DEFAULT_SIMPLE:
		अगर (!same_remote)
			अवरोध;
		अगर (म_भेद(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_धकेलो_simple(branch, remote);
		अवरोध;

	हाल धकेलो_DEFAULT_UPSTREAM:
		अगर (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		अवरोध;

	हाल धकेलो_DEFAULT_CURRENT:
		अवरोध;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	अगर ((*flags & TRANSPORT_धकेलो_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_धकेलो_SET_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

अटल स्थिर अक्षर message_advice_pull_before_धकेलो[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

अटल स्थिर अक्षर message_advice_checkout_pull_धकेलो[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

अटल स्थिर अक्षर message_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

अटल स्थिर अक्षर message_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

अटल स्थिर अक्षर message_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

अटल स्थिर अक्षर message_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

अटल व्योम advise_pull_before_धकेलो(व्योम)
{
	अगर (!advice_enabled(ADVICE_धकेलो_NON_FF_CURRENT) || !advice_enabled(ADVICE_धकेलो_UPDATE_REJECTED))
		वापस;
	advise(_(message_advice_pull_before_धकेलो));
}

अटल व्योम advise_checkout_pull_धकेलो(व्योम)
{
	अगर (!advice_enabled(ADVICE_धकेलो_NON_FF_MATCHING) || !advice_enabled(ADVICE_धकेलो_UPDATE_REJECTED))
		वापस;
	advise(_(message_advice_checkout_pull_धकेलो));
}

अटल व्योम advise_ref_already_exists(व्योम)
{
	अगर (!advice_enabled(ADVICE_धकेलो_ALREADY_EXISTS) || !advice_enabled(ADVICE_धकेलो_UPDATE_REJECTED))
		वापस;
	advise(_(message_advice_ref_already_exists));
}

अटल व्योम advise_ref_fetch_first(व्योम)
{
	अगर (!advice_enabled(ADVICE_धकेलो_FETCH_FIRST) || !advice_enabled(ADVICE_धकेलो_UPDATE_REJECTED))
		वापस;
	advise(_(message_advice_ref_fetch_first));
}

अटल व्योम advise_ref_needs_force(व्योम)
{
	अगर (!advice_enabled(ADVICE_धकेलो_NEEDS_FORCE) || !advice_enabled(ADVICE_धकेलो_UPDATE_REJECTED))
		वापस;
	advise(_(message_advice_ref_needs_force));
}

अटल व्योम advise_ref_needs_update(व्योम)
{
	अगर (!advice_enabled(ADVICE_धकेलो_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_धकेलो_UPDATE_REJECTED))
		वापस;
	advise(_(message_advice_ref_needs_update));
}

अटल पूर्णांक धकेलो_with_options(काष्ठा transport *transport, काष्ठा refspec *rs,
			     पूर्णांक flags)
{
	पूर्णांक err;
	अचिन्हित पूर्णांक reject_reasons;
	अक्षर *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	अगर (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : शून्य);

	अगर (!is_empty_cas(&cas)) {
		अगर (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	अगर (verbosity > 0)
		ख_लिखो(मानक_त्रुटि, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_धकेलो(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	अगर (err != 0) {
		ख_लिखो(मानक_त्रुटि, "%s", धकेलो_get_color(धकेलो_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		ख_लिखो(मानक_त्रुटि, "%s", धकेलो_get_color(धकेलो_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	मुक्त(anon_url);
	अगर (!err)
		वापस 0;

	अगर (reject_reasons & REJECT_NON_FF_शीर्ष) {
		advise_pull_before_धकेलो();
	} अन्यथा अगर (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_धकेलो();
	} अन्यथा अगर (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} अन्यथा अगर (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} अन्यथा अगर (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} अन्यथा अगर (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	वापस 1;
}

अटल पूर्णांक do_धकेलो(पूर्णांक flags,
		   स्थिर काष्ठा string_list *धकेलो_options,
		   काष्ठा remote *remote)
{
	पूर्णांक errs;
	काष्ठा strvec *url;
	काष्ठा refspec *धकेलो_refspec = &rs;

	अगर (धकेलो_options->nr)
		flags |= TRANSPORT_धकेलो_OPTIONS;

	अगर (!धकेलो_refspec->nr && !(flags & TRANSPORT_धकेलो_ALL)) {
		अगर (remote->धकेलो.nr) {
			धकेलो_refspec = &remote->धकेलो;
		} अन्यथा अगर (!(flags & TRANSPORT_धकेलो_MIRROR))
			setup_default_धकेलो_refspecs(&flags, remote);
	}
	errs = 0;
	url = धकेलो_url_of_remote(remote);
	क्रम (माप_प्रकार i = 0; i < url->nr; i++) {
		काष्ठा transport *transport =
			transport_get(remote, url->v[i]);
		अगर (flags & TRANSPORT_धकेलो_OPTIONS)
			transport->धकेलो_options = धकेलो_options;
		अगर (धकेलो_with_options(transport, धकेलो_refspec, flags))
			errs++;
	}
	वापस !!errs;
}

अटल पूर्णांक option_parse_recurse_submodules(स्थिर काष्ठा option *opt,
				   स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *recurse_submodules = opt->मूल्य;

	अगर (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	अन्यथा {
		अगर (!म_भेद(arg, "only-is-on-demand")) {
			अगर (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} अन्यथा {
			*recurse_submodules = parse_धकेलो_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	वापस 0;
}

अटल व्योम set_धकेलो_cert_flags(पूर्णांक *flags, पूर्णांक v)
{
	चयन (v) {
	हाल SEND_PACK_धकेलो_CERT_NEVER:
		*flags &= ~(TRANSPORT_धकेलो_CERT_ALWAYS | TRANSPORT_धकेलो_CERT_IF_ASKED);
		अवरोध;
	हाल SEND_PACK_धकेलो_CERT_ALWAYS:
		*flags |= TRANSPORT_धकेलो_CERT_ALWAYS;
		*flags &= ~TRANSPORT_धकेलो_CERT_IF_ASKED;
		अवरोध;
	हाल SEND_PACK_धकेलो_CERT_IF_ASKED:
		*flags |= TRANSPORT_धकेलो_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_धकेलो_CERT_ALWAYS;
		अवरोध;
	}
}


अटल पूर्णांक git_धकेलो_config(स्थिर अक्षर *k, स्थिर अक्षर *v,
			   स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	स्थिर अक्षर *slot_name;
	पूर्णांक *flags = cb;

	अगर (!म_भेद(k, "push.followtags")) {
		अगर (git_config_bool(k, v))
			*flags |= TRANSPORT_धकेलो_FOLLOW_TAGS;
		अन्यथा
			*flags &= ~TRANSPORT_धकेलो_FOLLOW_TAGS;
		वापस 0;
	} अन्यथा अगर (!म_भेद(k, "push.autosetupremote")) {
		अगर (git_config_bool(k, v))
			*flags |= TRANSPORT_धकेलो_AUTO_UPSTREAM;
		वापस 0;
	} अन्यथा अगर (!म_भेद(k, "push.gpgsign")) {
		चयन (git_parse_maybe_bool(v)) {
		हाल 0:
			set_धकेलो_cert_flags(flags, SEND_PACK_धकेलो_CERT_NEVER);
			अवरोध;
		हाल 1:
			set_धकेलो_cert_flags(flags, SEND_PACK_धकेलो_CERT_ALWAYS);
			अवरोध;
		शेष:
			अगर (!strcasecmp(v, "if-asked"))
				set_धकेलो_cert_flags(flags, SEND_PACK_धकेलो_CERT_IF_ASKED);
			अन्यथा
				वापस error(_("invalid value for '%s'"), k);
		}
	} अन्यथा अगर (!म_भेद(k, "push.recursesubmodules")) {
		recurse_submodules = parse_धकेलो_recurse_submodules_arg(k, v);
	} अन्यथा अगर (!म_भेद(k, "submodule.recurse")) {
		पूर्णांक val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} अन्यथा अगर (!म_भेद(k, "push.pushoption")) {
		वापस parse_transport_option(k, v, &धकेलो_options_config);
	} अन्यथा अगर (!म_भेद(k, "color.push")) {
		धकेलो_use_color = git_config_colorbool(k, v);
		वापस 0;
	} अन्यथा अगर (skip_prefix(k, "color.push.", &slot_name)) {
		पूर्णांक slot = parse_धकेलो_color_slot(slot_name);
		अगर (slot < 0)
			वापस 0;
		अगर (!v)
			वापस config_error_nonbool(k);
		वापस color_parse(v, धकेलो_colors[slot]);
	} अन्यथा अगर (!म_भेद(k, "push.useforceifincludes")) {
		अगर (git_config_bool(k, v))
			*flags |= TRANSPORT_धकेलो_FORCE_IF_INCLUDES;
		अन्यथा
			*flags &= ~TRANSPORT_धकेलो_FORCE_IF_INCLUDES;
		वापस 0;
	}

	वापस git_default_config(k, v, ctx, शून्य);
}

अटल पूर्णांक धकेलो_multiple(काष्ठा string_list *list,
			 स्थिर काष्ठा string_list *धकेलो_options,
			 पूर्णांक flags,
			 पूर्णांक tags,
			 स्थिर अक्षर **refspecs,
			 पूर्णांक refspec_nr)
{
	पूर्णांक परिणाम = 0;
	माप_प्रकार i;
	काष्ठा strvec argv = STRVEC_INIT;

	strvec_धकेलो(&argv, "push");

	अगर (flags & TRANSPORT_धकेलो_FORCE)
		strvec_धकेलो(&argv, "--force");
	अगर (flags & TRANSPORT_धकेलो_DRY_RUN)
		strvec_धकेलो(&argv, "--dry-run");
	अगर (flags & TRANSPORT_धकेलो_PORCELAIN)
		strvec_धकेलो(&argv, "--porcelain");
	अगर (flags & TRANSPORT_धकेलो_PRUNE)
		strvec_धकेलो(&argv, "--prune");
	अगर (flags & TRANSPORT_धकेलो_NO_HOOK)
		strvec_धकेलो(&argv, "--no-verify");
	अगर (flags & TRANSPORT_धकेलो_FOLLOW_TAGS)
		strvec_धकेलो(&argv, "--follow-tags");
	अगर (flags & TRANSPORT_धकेलो_SET_UPSTREAM)
		strvec_धकेलो(&argv, "--set-upstream");
	अगर (flags & TRANSPORT_धकेलो_FORCE_IF_INCLUDES)
		strvec_धकेलो(&argv, "--force-if-includes");
	अगर (flags & TRANSPORT_धकेलो_ALL)
		strvec_धकेलो(&argv, "--all");
	अगर (flags & TRANSPORT_धकेलो_MIRROR)
		strvec_धकेलो(&argv, "--mirror");

	अगर (flags & TRANSPORT_धकेलो_CERT_ALWAYS)
		strvec_धकेलो(&argv, "--signed=yes");
	अन्यथा अगर (flags & TRANSPORT_धकेलो_CERT_IF_ASKED)
		strvec_धकेलो(&argv, "--signed=if-asked");
	अगर (!thin)
		strvec_धकेलो(&argv, "--no-thin");

	अगर (deleterefs)
		strvec_धकेलो(&argv, "--delete");

	अगर (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	अगर (verbosity >= 2)
		strvec_धकेलो(&argv, "-v");
	अगर (verbosity >= 1)
		strvec_धकेलो(&argv, "-v");
	अन्यथा अगर (verbosity < 0)
		strvec_धकेलो(&argv, "-q");
	अगर (progress > 0)
		strvec_धकेलो(&argv, "--progress");
	अन्यथा अगर (progress == 0)
		strvec_धकेलो(&argv, "--no-progress");

	अगर (family == TRANSPORT_FAMILY_IPV4)
		strvec_धकेलो(&argv, "--ipv4");
	अन्यथा अगर (family == TRANSPORT_FAMILY_IPV6)
		strvec_धकेलो(&argv, "--ipv6");

	अगर (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_धकेलो(&argv, "--recurse-submodules=check");
	अन्यथा अगर (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_धकेलो(&argv, "--recurse-submodules=on-demand");
	अन्यथा अगर (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_धकेलो(&argv, "--recurse-submodules=only");
	अन्यथा अगर (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_धकेलो(&argv, "--recurse-submodules=no");


	अगर (tags)
		strvec_धकेलो(&argv, "--tags");

	क्रम (i = 0; i < धकेलो_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     धकेलो_options->वस्तुएँ[i].string);

	क्रम (i = 0; i < cas.nr; i++) {
		अगर (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} अन्यथा अगर (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} अन्यथा {
			strvec_धकेलो(&argv, "--force-with-lease");
		}
	}

	क्रम (i = 0; i < list->nr; i++) {
		स्थिर अक्षर *name = list->वस्तुएँ[i].string;
		काष्ठा child_process cmd = CHILD_PROCESS_INIT;
		पूर्णांक j;

		strvec_pushv(&cmd.args, argv.v);
		strvec_धकेलो(&cmd.args, name);

		क्रम (j = 0; j < refspec_nr; j++)
			strvec_धकेलो(&cmd.args, refspecs[j]);

		अगर (verbosity >= 0)
			म_लिखो(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		अगर (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			परिणाम = 1;
		}
	}

	strvec_clear(&argv);
	वापस परिणाम;
}

अटल व्योम die_if_repo_looks_like_ref(स्थिर अक्षर *repo)
{
	स्थिर अक्षर *slash = म_अक्षर(repo, '/');
	काष्ठा strbuf name = STRBUF_INIT;
	पूर्णांक code;

	अगर (!slash || !slash[1] || file_exists(repo))
		वापस;

	strbuf_add(&name, repo, slash - repo);
	अगर (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		वापस;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_धकेलो_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	निकास(code);
}

पूर्णांक cmd_धकेलो(पूर्णांक argc,
	     स्थिर अक्षर **argv,
	     स्थिर अक्षर *prefix,
	     काष्ठा repository *repository UNUSED)
{
	पूर्णांक flags = 0;
	पूर्णांक tags = 0;
	पूर्णांक धकेलो_cert = -1;
	पूर्णांक rc = 0;
	पूर्णांक base_flags;
	स्थिर अक्षर *repo = शून्य;	/* default repository */
	काष्ठा string_list धकेलो_options_cmdline = STRING_LIST_INIT_DUP;
	काष्ठा string_list remote_group = STRING_LIST_INIT_DUP;
	काष्ठा string_list *धकेलो_options;
	स्थिर काष्ठा string_list_item *item;

	काष्ठा option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_BIT( 0 , "all", &flags, N_("push all branches"), TRANSPORT_धकेलो_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_BIT( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_धकेलो_MIRROR|TRANSPORT_धकेलो_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &tags, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_BIT('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_धकेलो_DRY_RUN),
		OPT_BIT( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_धकेलो_PORCELAIN),
		OPT_BIT('f', "force", &flags, N_("force updates"), TRANSPORT_धकेलो_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_धकेलो_cas_option),
		OPT_BIT(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_धकेलो_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_BIT('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_धकेलो_SET_UPSTREAM),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_BIT(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_धकेलो_PRUNE),
		OPT_BIT(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_धकेलो_NO_HOOK),
		OPT_BIT(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_धकेलो_FOLLOW_TAGS),
		OPT_CALLBACK_F(0, "signed", &धकेलो_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_धकेलो_signed),
		OPT_BIT(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_धकेलो_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &धकेलो_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_END()
	};

	packet_trace_identity("push");
	repo_config(the_repository, git_धकेलो_config, &flags);
	argc = parse_options(argc, argv, prefix, options, धकेलो_usage, 0);
	धकेलो_options = (धकेलो_options_cmdline.nr
		? &धकेलो_options_cmdline
		: &धकेलो_options_config);
	set_धकेलो_cert_flags(&flags, धकेलो_cert);

	refspec_init_धकेलो(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  tags, "--tags",
				  flags & TRANSPORT_धकेलो_ALL, "--all/--branches",
				  flags & TRANSPORT_धकेलो_MIRROR, "--mirror");
	अगर (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	अगर (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	अन्यथा अगर (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	अन्यथा अगर (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	अगर (argc > 0)
		repo = argv[0];

	अगर (repo) {
		अगर (!add_remote_or_group(repo, &remote_group)) {
			काष्ठा remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			अगर (advice_enabled(ADVICE_धकेलो_REPO_LOOKS_LIKE_REF))
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
			अगर (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_group, r->name);
		}
	} अन्यथा {
		काष्ठा remote *r = pushremote_get(शून्य);
		अगर (!r)
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

	अगर (!is_empty_cas(&cas) && (flags & TRANSPORT_धकेलो_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, धकेलो_options)
		अगर (म_अक्षर(item->string, '\n'))
			die(_("push options must not have new line characters"));

	अगर (remote_group.nr == 1) {
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
			काष्ठा remote *r = pushremote_get(remote_group.वस्तुएँ[0].string);
			अगर (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.वस्तुएँ[0].string);

			अगर (r->mirror)
				inner_flags |= (TRANSPORT_धकेलो_MIRROR|TRANSPORT_धकेलो_FORCE);

			अगर (inner_flags & TRANSPORT_धकेलो_ALL) {
				अगर (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			अगर (inner_flags & TRANSPORT_धकेलो_MIRROR) {
				अगर (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (काष्ठा refspec) REFSPEC_INIT_धकेलो(the_hash_algo);

			अगर (tags)
				refspec_append(&rs, "refs/tags/*");
			अगर (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_धकेलो(inner_flags, धकेलो_options, r);
		}
	} अन्यथा {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		अगर (flags & TRANSPORT_धकेलो_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = धकेलो_multiple(&remote_group, धकेलो_options, flags,
				   tags,
				   argc > 1 ? argv + 1 : शून्य,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&धकेलो_options_cmdline, 0);
	string_list_clear(&धकेलो_options_config, 0);
	string_list_clear(&remote_group, 0);
	clear_cas_option(&cas);

	अगर (rc == -1)
		usage_with_options(धकेलो_usage, options);
	अन्यथा
		वापस rc;
}
