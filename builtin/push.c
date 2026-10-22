/*
 * "git push"
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE

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

स्थिर ध्रुव वर्ण * ध्रुव प्रक्षिप_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	रिक्त,
};

स्थिर गणना git_colorbool प्रक्षिप_use_color = GIT_COLOR_UNKNOWN;
स्थिर वर्ण प्रक्षिप_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

गणना color_प्रक्षिप {
	प्रक्षिप_COLOR_RESET = 0,
	प्रक्षिप_COLOR_ERROR = 1
};

स्थिर पूर्णाङ्क parse_प्रक्षिप_color_slot(ध्रुव वर्ण *slot)
{
	यदि (!strcasecmp(slot, "reset"))
		प्रत्यागम प्रक्षिप_COLOR_RESET;
	यदि (!strcasecmp(slot, "error"))
		प्रत्यागम प्रक्षिप_COLOR_ERROR;
	प्रत्यागम -1;
}

स्थिर ध्रुव वर्ण *प्रक्षिप_get_color(गणना color_प्रक्षिप ix)
{
	यदि (want_color_stderr(प्रक्षिप_use_color))
		प्रत्यागम प्रक्षिप_colors[ix];
	प्रत्यागम "";
}

स्थिर पूर्णाङ्क thin = 1;
स्थिर पूर्णाङ्क deleterefs;
स्थिर ध्रुव वर्ण *receivepack;
स्थिर पूर्णाङ्क verbosity;
स्थिर पूर्णाङ्क प्रगतिः = -1;
स्थिर पूर्णाङ्क recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
स्थिर गणना transport_family family;

स्थिर संरचना प्रक्षिप_cas_option cas;

स्थिर संरचना refspec rs;

स्थिर संरचना string_list प्रक्षिप_options_config = STRING_LIST_INIT_DUP;

स्थिर शून्य refspec_append_mapped(संरचना refspec *refspec, ध्रुव वर्ण *ref,
				  संरचना remote *remote, संरचना ref *matched)
{
	ध्रुव वर्ण *branch_name;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	यदि (remote->प्रक्षिप.nr) {
		संरचना refspec_item query = {
			.src = matched->name,
		};

		यदि (!refspec_find_मेलनम्(&remote->प्रक्षिप, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			मुक्त(query.dst);
			प्रत्यागम;
		}
	}

	यदि (cfg->प्रक्षिप_default == प्रक्षिप_DEFAULT_UPSTREAM &&
	    skip_उपसर्गः(matched->name, "refs/heads/", &branch_name)) {
		संरचना branch *branch = branch_get(branch_name);
		यदि (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			प्रत्यागम;
		}
	}

	refspec_append(refspec, ref);
}

स्थिर शून्य समुच्चयः_refspecs(ध्रुव वर्ण **refs, पूर्णाङ्क nr, संरचना remote *remote)
{
	संरचना ref *local_refs = रिक्त;
	पूर्णाङ्क i;

	प्रति (i = 0; i < nr; i++) {
		ध्रुव वर्ण *ref = refs[i];
		यदि (!वाक्यं_तुलय("tag", ref)) {
			यदि (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			यदि (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			अन्यथा
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} अन्यथा यदि (deleterefs) {
			यदि (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} अन्यथा यदि (!strchr(ref, ':')) {
			संरचना ref *matched = रिक्त;

			/* lazily grab local_refs */
			यदि (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			यदि (गणनम्_refspec_मेलनम्(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			अन्यथा
				refspec_append_mapped(&rs, ref, remote, matched);
		} अन्यथा
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

स्थिर NORETURN शून्य die_प्रक्षिप_simple(संरचना branch *branch,
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
	ध्रुव वर्ण *advice_pushdefault_maybe = "";
	ध्रुव वर्ण *advice_automergesimple_maybe = "";
	ध्रुव वर्ण *short_upstream = branch->merge[0]->src;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	skip_उपसर्गः(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	यदि (cfg->प्रक्षिप_default == प्रक्षिप_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	यदि (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

स्थिर ध्रुव वर्ण सन्देशः_detached_शीर्ष_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

स्थिर ध्रुव वर्ण *get_upstream_ref(पूर्णाङ्क flags, संरचना branch *branch, ध्रुव वर्ण *remote_name)
{
	यदि (branch->merge_nr == 0 && (flags & TRANSPORT_प्रक्षिप_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		प्रत्यागम branch->refname;
	}

	यदि (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		ध्रुव वर्ण *advice_autosetup_maybe = "";
		यदि (!(flags & TRANSPORT_प्रक्षिप_AUTO_UPSTREAM)) {
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
	यदि (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	प्रत्यागम branch->merge[0]->src;
}

स्थिर शून्य सज्जता_default_प्रक्षिप_refspecs(पूर्णाङ्क *flags, संरचना remote *remote)
{
	संरचना branch *branch;
	ध्रुव वर्ण *dst;
	पूर्णाङ्क same_remote;
	संरचना repo_config_मानानि *cfg = repo_config_मानानि(the_repository);

	विकल्प (cfg->प्रक्षिप_default) {
	पक्ष प्रक्षिप_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		प्रत्यागम;

	पक्ष प्रक्षिप_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		प्रत्यागम;
	स्वभाव:
		भङ्ग;
	}

	branch = branch_get(रिक्त);
	यदि (!branch)
		die(_(सन्देशः_detached_शीर्ष_die), remote->name);

	dst = branch->refname;
	same_remote = !वाक्यं_तुलय(remote->name, remote_for_branch(branch, रिक्त));

	विकल्प (cfg->प्रक्षिप_default) {
	स्वभाव:
	पक्ष प्रक्षिप_DEFAULT_UNSPECIFIED:
	पक्ष प्रक्षिप_DEFAULT_SIMPLE:
		यदि (!same_remote)
			भङ्ग;
		यदि (वाक्यं_तुलय(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_प्रक्षिप_simple(branch, remote);
		भङ्ग;

	पक्ष प्रक्षिप_DEFAULT_UPSTREAM:
		यदि (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		भङ्ग;

	पक्ष प्रक्षिप_DEFAULT_CURRENT:
		भङ्ग;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	यदि ((*flags & TRANSPORT_प्रक्षिप_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_प्रक्षिप_समुच्चयः_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

स्थिर ध्रुव वर्ण सन्देशः_advice_pull_before_प्रक्षिप[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

स्थिर ध्रुव वर्ण सन्देशः_advice_checkout_pull_प्रक्षिप[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

स्थिर ध्रुव वर्ण सन्देशः_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

स्थिर ध्रुव वर्ण सन्देशः_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

स्थिर ध्रुव वर्ण सन्देशः_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

स्थिर ध्रुव वर्ण सन्देशः_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

स्थिर शून्य advise_pull_before_प्रक्षिप(शून्य)
{
	यदि (!advice_enabled(ADVICE_प्रक्षिप_NON_FF_CURRENT) || !advice_enabled(ADVICE_प्रक्षिप_UPDATE_REJECTED))
		प्रत्यागम;
	advise(_(सन्देशः_advice_pull_before_प्रक्षिप));
}

स्थिर शून्य advise_checkout_pull_प्रक्षिप(शून्य)
{
	यदि (!advice_enabled(ADVICE_प्रक्षिप_NON_FF_MATCHING) || !advice_enabled(ADVICE_प्रक्षिप_UPDATE_REJECTED))
		प्रत्यागम;
	advise(_(सन्देशः_advice_checkout_pull_प्रक्षिप));
}

स्थिर शून्य advise_ref_already_exists(शून्य)
{
	यदि (!advice_enabled(ADVICE_प्रक्षिप_ALREADY_EXISTS) || !advice_enabled(ADVICE_प्रक्षिप_UPDATE_REJECTED))
		प्रत्यागम;
	advise(_(सन्देशः_advice_ref_already_exists));
}

स्थिर शून्य advise_ref_fetch_first(शून्य)
{
	यदि (!advice_enabled(ADVICE_प्रक्षिप_FETCH_FIRST) || !advice_enabled(ADVICE_प्रक्षिप_UPDATE_REJECTED))
		प्रत्यागम;
	advise(_(सन्देशः_advice_ref_fetch_first));
}

स्थिर शून्य advise_ref_needs_force(शून्य)
{
	यदि (!advice_enabled(ADVICE_प्रक्षिप_NEEDS_FORCE) || !advice_enabled(ADVICE_प्रक्षिप_UPDATE_REJECTED))
		प्रत्यागम;
	advise(_(सन्देशः_advice_ref_needs_force));
}

स्थिर शून्य advise_ref_needs_update(शून्य)
{
	यदि (!advice_enabled(ADVICE_प्रक्षिप_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_प्रक्षिप_UPDATE_REJECTED))
		प्रत्यागम;
	advise(_(सन्देशः_advice_ref_needs_update));
}

स्थिर पूर्णाङ्क प्रक्षिप_with_options(संरचना transport *transport, संरचना refspec *rs,
			     पूर्णाङ्क flags)
{
	पूर्णाङ्क err;
	अचिह्नित पूर्णाङ्क reject_reasons;
	वर्ण *anon_url = transport_anonymize_url(transport->url);

	transport_समुच्चयः_verbosity(transport, verbosity, प्रगतिः);
	transport->family = family;

	यदि (receivepack)
		transport_समुच्चयः_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_समुच्चयः_option(transport, TRANS_OPT_THIN, thin ? "yes" : रिक्त);

	यदि (!is_रिक्तानि_cas(&cas)) {
		यदि (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	यदि (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_प्रक्षिप(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	यदि (err != 0) {
		fprintf(stderr, "%s", प्रक्षिप_get_color(प्रक्षिप_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", प्रक्षिप_get_color(प्रक्षिप_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	मुक्त(anon_url);
	यदि (!err)
		प्रत्यागम 0;

	यदि (reject_reasons & REJECT_NON_FF_शीर्ष) {
		advise_pull_before_प्रक्षिप();
	} अन्यथा यदि (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_प्रक्षिप();
	} अन्यथा यदि (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} अन्यथा यदि (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} अन्यथा यदि (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} अन्यथा यदि (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क do_प्रक्षिप(पूर्णाङ्क flags,
		   ध्रुव संरचना string_list *प्रक्षिप_options,
		   संरचना remote *remote)
{
	पूर्णाङ्क errs;
	संरचना strvec *url;
	संरचना refspec *प्रक्षिप_refspec = &rs;

	यदि (प्रक्षिप_options->nr)
		flags |= TRANSPORT_प्रक्षिप_OPTIONS;

	यदि (!प्रक्षिप_refspec->nr && !(flags & TRANSPORT_प्रक्षिप_ALL)) {
		यदि (remote->प्रक्षिप.nr) {
			प्रक्षिप_refspec = &remote->प्रक्षिप;
		} अन्यथा यदि (!(flags & TRANSPORT_प्रक्षिप_MIRROR))
			सज्जता_default_प्रक्षिप_refspecs(&flags, remote);
	}
	errs = 0;
	url = प्रक्षिप_url_सम्बन्धि_remote(remote);
	प्रति (आकार_t i = 0; i < url->nr; i++) {
		संरचना transport *transport =
			transport_get(remote, url->v[i]);
		यदि (flags & TRANSPORT_प्रक्षिप_OPTIONS)
			transport->प्रक्षिप_options = प्रक्षिप_options;
		यदि (प्रक्षिप_with_options(transport, प्रक्षिप_refspec, flags))
			errs++;
	}
	प्रत्यागम !!errs;
}

स्थिर पूर्णाङ्क option_parse_recurse_submodules(ध्रुव संरचना option *opt,
				   ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *recurse_submodules = opt->मानम्;

	यदि (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	अन्यथा {
		यदि (!वाक्यं_तुलय(arg, "only-is-on-demand")) {
			यदि (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} अन्यथा {
			*recurse_submodules = parse_प्रक्षिप_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	प्रत्यागम 0;
}

स्थिर शून्य समुच्चयः_प्रक्षिप_cert_flags(पूर्णाङ्क *flags, पूर्णाङ्क v)
{
	विकल्प (v) {
	पक्ष SEND_PACK_प्रक्षिप_CERT_NEVER:
		*flags &= ~(TRANSPORT_प्रक्षिप_CERT_ALWAYS | TRANSPORT_प्रक्षिप_CERT_IF_ASKED);
		भङ्ग;
	पक्ष SEND_PACK_प्रक्षिप_CERT_ALWAYS:
		*flags |= TRANSPORT_प्रक्षिप_CERT_ALWAYS;
		*flags &= ~TRANSPORT_प्रक्षिप_CERT_IF_ASKED;
		भङ्ग;
	पक्ष SEND_PACK_प्रक्षिप_CERT_IF_ASKED:
		*flags |= TRANSPORT_प्रक्षिप_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_प्रक्षिप_CERT_ALWAYS;
		भङ्ग;
	}
}


स्थिर पूर्णाङ्क git_प्रक्षिप_config(ध्रुव वर्ण *k, ध्रुव वर्ण *v,
			   ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	ध्रुव वर्ण *slot_name;
	पूर्णाङ्क *flags = cb;

	यदि (!वाक्यं_तुलय(k, "push.followtags")) {
		यदि (git_config_bool(k, v))
			*flags |= TRANSPORT_प्रक्षिप_FOLLOW_चिह्नकानि;
		अन्यथा
			*flags &= ~TRANSPORT_प्रक्षिप_FOLLOW_चिह्नकानि;
		प्रत्यागम 0;
	} अन्यथा यदि (!वाक्यं_तुलय(k, "push.autosetupremote")) {
		यदि (git_config_bool(k, v))
			*flags |= TRANSPORT_प्रक्षिप_AUTO_UPSTREAM;
		प्रत्यागम 0;
	} अन्यथा यदि (!वाक्यं_तुलय(k, "push.gpgsign")) {
		विकल्प (git_parse_maybe_bool(v)) {
		पक्ष 0:
			समुच्चयः_प्रक्षिप_cert_flags(flags, SEND_PACK_प्रक्षिप_CERT_NEVER);
			भङ्ग;
		पक्ष 1:
			समुच्चयः_प्रक्षिप_cert_flags(flags, SEND_PACK_प्रक्षिप_CERT_ALWAYS);
			भङ्ग;
		स्वभाव:
			यदि (!strcasecmp(v, "if-asked"))
				समुच्चयः_प्रक्षिप_cert_flags(flags, SEND_PACK_प्रक्षिप_CERT_IF_ASKED);
			अन्यथा
				प्रत्यागम error(_("invalid value for '%s'"), k);
		}
	} अन्यथा यदि (!वाक्यं_तुलय(k, "push.recursesubmodules")) {
		recurse_submodules = parse_प्रक्षिप_recurse_submodules_arg(k, v);
	} अन्यथा यदि (!वाक्यं_तुलय(k, "submodule.recurse")) {
		पूर्णाङ्क val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} अन्यथा यदि (!वाक्यं_तुलय(k, "push.pushoption")) {
		प्रत्यागम parse_transport_option(k, v, &प्रक्षिप_options_config);
	} अन्यथा यदि (!वाक्यं_तुलय(k, "color.push")) {
		प्रक्षिप_use_color = git_config_colorbool(k, v);
		प्रत्यागम 0;
	} अन्यथा यदि (skip_उपसर्गः(k, "color.push.", &slot_name)) {
		पूर्णाङ्क slot = parse_प्रक्षिप_color_slot(slot_name);
		यदि (slot < 0)
			प्रत्यागम 0;
		यदि (!v)
			प्रत्यागम config_error_nonbool(k);
		प्रत्यागम color_parse(v, प्रक्षिप_colors[slot]);
	} अन्यथा यदि (!वाक्यं_तुलय(k, "push.useforceifincludes")) {
		यदि (git_config_bool(k, v))
			*flags |= TRANSPORT_प्रक्षिप_FORCE_IF_INCLUDES;
		अन्यथा
			*flags &= ~TRANSPORT_प्रक्षिप_FORCE_IF_INCLUDES;
		प्रत्यागम 0;
	}

	प्रत्यागम git_default_config(k, v, ctx, रिक्त);
}

स्थिर पूर्णाङ्क प्रक्षिप_गुणजः(संरचना string_list *list,
			 ध्रुव संरचना string_list *प्रक्षिप_options,
			 पूर्णाङ्क flags,
			 पूर्णाङ्क चिह्नकानि,
			 ध्रुव वर्ण **refspecs,
			 पूर्णाङ्क refspec_nr)
{
	पूर्णाङ्क परिणाम = 0;
	आकार_t i;
	संरचना strvec argv = STRVEC_INIT;

	strvec_प्रक्षिप(&argv, "push");

	यदि (flags & TRANSPORT_प्रक्षिप_FORCE)
		strvec_प्रक्षिप(&argv, "--force");
	यदि (flags & TRANSPORT_प्रक्षिप_DRY_सततम्)
		strvec_प्रक्षिप(&argv, "--dry-run");
	यदि (flags & TRANSPORT_प्रक्षिप_PORCELAIN)
		strvec_प्रक्षिप(&argv, "--porcelain");
	यदि (flags & TRANSPORT_प्रक्षिप_PRUNE)
		strvec_प्रक्षिप(&argv, "--prune");
	यदि (flags & TRANSPORT_प्रक्षिप_NO_HOOK)
		strvec_प्रक्षिप(&argv, "--no-verify");
	यदि (flags & TRANSPORT_प्रक्षिप_FOLLOW_चिह्नकानि)
		strvec_प्रक्षिप(&argv, "--follow-tags");
	यदि (flags & TRANSPORT_प्रक्षिप_समुच्चयः_UPSTREAM)
		strvec_प्रक्षिप(&argv, "--set-upstream");
	यदि (flags & TRANSPORT_प्रक्षिप_FORCE_IF_INCLUDES)
		strvec_प्रक्षिप(&argv, "--force-if-includes");
	यदि (flags & TRANSPORT_प्रक्षिप_ALL)
		strvec_प्रक्षिप(&argv, "--all");
	यदि (flags & TRANSPORT_प्रक्षिप_MIRROR)
		strvec_प्रक्षिप(&argv, "--mirror");

	यदि (flags & TRANSPORT_प्रक्षिप_CERT_ALWAYS)
		strvec_प्रक्षिप(&argv, "--signed=yes");
	अन्यथा यदि (flags & TRANSPORT_प्रक्षिप_CERT_IF_ASKED)
		strvec_प्रक्षिप(&argv, "--signed=if-asked");
	यदि (!thin)
		strvec_प्रक्षिप(&argv, "--no-thin");

	यदि (deleterefs)
		strvec_प्रक्षिप(&argv, "--delete");

	यदि (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	यदि (verbosity >= 2)
		strvec_प्रक्षिप(&argv, "-v");
	यदि (verbosity >= 1)
		strvec_प्रक्षिप(&argv, "-v");
	अन्यथा यदि (verbosity < 0)
		strvec_प्रक्षिप(&argv, "-q");
	यदि (प्रगतिः > 0)
		strvec_प्रक्षिप(&argv, "--progress");
	अन्यथा यदि (प्रगतिः == 0)
		strvec_प्रक्षिप(&argv, "--no-progress");

	यदि (family == TRANSPORT_FAMILY_IPV4)
		strvec_प्रक्षिप(&argv, "--ipv4");
	अन्यथा यदि (family == TRANSPORT_FAMILY_IPV6)
		strvec_प्रक्षिप(&argv, "--ipv6");

	यदि (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_प्रक्षिप(&argv, "--recurse-submodules=check");
	अन्यथा यदि (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_प्रक्षिप(&argv, "--recurse-submodules=on-demand");
	अन्यथा यदि (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_प्रक्षिप(&argv, "--recurse-submodules=only");
	अन्यथा यदि (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_प्रक्षिप(&argv, "--recurse-submodules=no");


	यदि (चिह्नकानि)
		strvec_प्रक्षिप(&argv, "--tags");

	प्रति (i = 0; i < प्रक्षिप_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     प्रक्षिप_options->वस्तूनि[i].string);

	प्रति (i = 0; i < cas.nr; i++) {
		यदि (cas.प्रविष्टिः[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.प्रविष्टिः[i].refname);
		} अन्यथा यदि (!is_null_oid(&cas.प्रविष्टिः[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.प्रविष्टिः[i].refname,
				     oid_गन्तव्यम्_hex(&cas.प्रविष्टिः[i].expect));
		} अन्यथा {
			strvec_प्रक्षिप(&argv, "--force-with-lease");
		}
	}

	प्रति (i = 0; i < list->nr; i++) {
		ध्रुव वर्ण *name = list->वस्तूनि[i].string;
		संरचना child_process cmd = CHILD_PROCESS_INIT;
		पूर्णाङ्क j;

		strvec_pushv(&cmd.तर्काः, argv.v);
		strvec_प्रक्षिप(&cmd.तर्काः, name);

		प्रति (j = 0; j < refspec_nr; j++)
			strvec_प्रक्षिप(&cmd.तर्काः, refspecs[j]);

		यदि (verbosity >= 0)
			प्रारूप_लिख(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		यदि (सततम्_command(&cmd)) {
			error(_("could not push to %s"), name);
			परिणाम = 1;
		}
	}

	strvec_clear(&argv);
	प्रत्यागम परिणाम;
}

स्थिर शून्य die_if_repo_looks_like_ref(ध्रुव वर्ण *repo)
{
	ध्रुव वर्ण *slash = strchr(repo, '/');
	संरचना strbuf name = STRBUF_INIT;
	पूर्णाङ्क संहिता;

	यदि (!slash || !slash[1] || file_exists(repo))
		प्रत्यागम;

	strbuf_सङ्कलय(&name, repo, slash - repo);
	यदि (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		प्रत्यागम;
	}

	संहिता = die_सन्देशः(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_प्रक्षिप_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	निर्गच्छ(संहिता);
}

पूर्णाङ्क cmd_प्रक्षिप(पूर्णाङ्क argc,
	     ध्रुव वर्ण **argv,
	     ध्रुव वर्ण *उपसर्गः,
	     संरचना repository *repository अप्रयुक्तम्)
{
	पूर्णाङ्क flags = 0;
	पूर्णाङ्क चिह्नकानि = 0;
	पूर्णाङ्क प्रक्षिप_cert = -1;
	पूर्णाङ्क rc = 0;
	पूर्णाङ्क आधारः_flags;
	ध्रुव वर्ण *repo = रिक्त;	/* default repository */
	संरचना string_list प्रक्षिप_options_cmdline = STRING_LIST_INIT_DUP;
	संरचना string_list remote_गणः = STRING_LIST_INIT_DUP;
	संरचना string_list *प्रक्षिप_options;
	ध्रुव संरचना string_list_item *item;

	संरचना option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_BIT( 0 , "all", &flags, N_("push all branches"), TRANSPORT_प्रक्षिप_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_BIT( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_प्रक्षिप_MIRROR|TRANSPORT_प्रक्षिप_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &चिह्नकानि, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_BIT('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_प्रक्षिप_DRY_सततम्),
		OPT_BIT( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_प्रक्षिप_PORCELAIN),
		OPT_BIT('f', "force", &flags, N_("force updates"), TRANSPORT_प्रक्षिप_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_प्रक्षिप_cas_option),
		OPT_BIT(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_प्रक्षिप_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_BIT('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_प्रक्षिप_समुच्चयः_UPSTREAM),
		OPT_BOOL(0, "progress", &प्रगतिः, N_("force progress reporting")),
		OPT_BIT(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_प्रक्षिप_PRUNE),
		OPT_BIT(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_प्रक्षिप_NO_HOOK),
		OPT_BIT(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_प्रक्षिप_FOLLOW_चिह्नकानि),
		OPT_CALLBACK_F(0, "signed", &प्रक्षिप_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_प्रक्षिप_signed),
		OPT_BIT(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_प्रक्षिप_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &प्रक्षिप_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_अन्तः()
	};

	packet_trace_तटस्थः("push");
	repo_config(the_repository, git_प्रक्षिप_config, &flags);
	argc = parse_options(argc, argv, उपसर्गः, options, प्रक्षिप_usage, 0);
	प्रक्षिप_options = (प्रक्षिप_options_cmdline.nr
		? &प्रक्षिप_options_cmdline
		: &प्रक्षिप_options_config);
	समुच्चयः_प्रक्षिप_cert_flags(&flags, प्रक्षिप_cert);

	refspec_init_प्रक्षिप(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  चिह्नकानि, "--tags",
				  flags & TRANSPORT_प्रक्षिप_ALL, "--all/--branches",
				  flags & TRANSPORT_प्रक्षिप_MIRROR, "--mirror");
	यदि (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	यदि (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	अन्यथा यदि (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	अन्यथा यदि (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	यदि (argc > 0)
		repo = argv[0];

	यदि (repo) {
		यदि (!सङ्कलय_remote_or_गणः(repo, &remote_गणः)) {
			संरचना remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			यदि (advice_enabled(ADVICE_प्रक्षिप_REPO_LOOKS_LIKE_REF))
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
			यदि (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_गणः, r->name);
		}
	} अन्यथा {
		संरचना remote *r = pushremote_get(रिक्त);
		यदि (!r)
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
		string_list_append(&remote_गणः, r->name);
	}

	यदि (!is_रिक्तानि_cas(&cas) && (flags & TRANSPORT_प्रक्षिप_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, प्रक्षिप_options)
		यदि (strchr(item->string, '\n'))
			die(_("push options must not have new line characters"));

	यदि (remote_गणः.nr == 1) {
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
		आधारः_flags = flags;
		{
			पूर्णाङ्क inner_flags = आधारः_flags;
			संरचना remote *r = pushremote_get(remote_गणः.वस्तूनि[0].string);
			यदि (!r)
				die(_("no such remote or remote group: %s"),
				    remote_गणः.वस्तूनि[0].string);

			यदि (r->mirror)
				inner_flags |= (TRANSPORT_प्रक्षिप_MIRROR|TRANSPORT_प्रक्षिप_FORCE);

			यदि (inner_flags & TRANSPORT_प्रक्षिप_ALL) {
				यदि (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			यदि (inner_flags & TRANSPORT_प्रक्षिप_MIRROR) {
				यदि (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (संरचना refspec) REFSPEC_INIT_प्रक्षिप(the_hash_algo);

			यदि (चिह्नकानि)
				refspec_append(&rs, "refs/tags/*");
			यदि (argc > 0)
				समुच्चयः_refspecs(argv + 1, argc - 1, r);

			rc = do_प्रक्षिप(inner_flags, प्रक्षिप_options, r);
		}
	} अन्यथा {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		यदि (flags & TRANSPORT_प्रक्षिप_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = प्रक्षिप_गुणजः(&remote_गणः, प्रक्षिप_options, flags,
				   चिह्नकानि,
				   argc > 1 ? argv + 1 : रिक्त,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&प्रक्षिप_options_cmdline, 0);
	string_list_clear(&प्रक्षिप_options_config, 0);
	string_list_clear(&remote_गणः, 0);
	clear_cas_option(&cas);

	यदि (rc == -1)
		usage_with_options(प्रक्षिप_usage, options);
	अन्यथा
		प्रत्यागम rc;
}
