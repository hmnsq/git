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

स्थिर अचर वर्ण * अचर थप_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	रिक्त,
};

स्थिर गणना git_colorbool थप_use_color = GIT_COLOR_UNKNOWN;
स्थिर वर्ण थप_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

गणना color_थप {
	थप_COLOR_RESET = 0,
	थप_COLOR_ERROR = 1
};

स्थिर पूर्णाङ्क parse_थप_color_slot(अचर वर्ण *slot)
{
	यदि (!strcasecmp(slot, "reset"))
		फर्काऊ थप_COLOR_RESET;
	यदि (!strcasecmp(slot, "error"))
		फर्काऊ थप_COLOR_ERROR;
	फर्काऊ -1;
}

स्थिर अचर वर्ण *थप_get_color(गणना color_थप ix)
{
	यदि (want_color_stderr(थप_use_color))
		फर्काऊ थप_colors[ix];
	फर्काऊ "";
}

स्थिर पूर्णाङ्क thin = 1;
स्थिर पूर्णाङ्क deleterefs;
स्थिर अचर वर्ण *receivepack;
स्थिर पूर्णाङ्क verbosity;
स्थिर पूर्णाङ्क progress = -1;
स्थिर पूर्णाङ्क recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
स्थिर गणना transport_family family;

स्थिर संरचना थप_cas_option cas;

स्थिर संरचना refspec rs;

स्थिर संरचना string_list थप_options_config = STRING_LIST_INIT_DUP;

स्थिर शून्य refspec_append_mapped(संरचना refspec *refspec, अचर वर्ण *ref,
				  संरचना remote *remote, संरचना ref *matched)
{
	अचर वर्ण *branch_name;
	संरचना repo_config_मानहरू *cfg = repo_config_मानहरू(the_repository);

	यदि (remote->थप.nr) {
		संरचना refspec_item query = {
			.src = matched->name,
		};

		यदि (!refspec_find_match(&remote->थप, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			मुक्त(query.dst);
			फर्काऊ;
		}
	}

	यदि (cfg->थप_default == थप_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		संरचना branch *branch = branch_get(branch_name);
		यदि (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			फर्काऊ;
		}
	}

	refspec_append(refspec, ref);
}

स्थिर शून्य set_refspecs(अचर वर्ण **refs, पूर्णाङ्क nr, संरचना remote *remote)
{
	संरचना ref *local_refs = रिक्त;
	पूर्णाङ्क i;

	लागि (i = 0; i < nr; i++) {
		अचर वर्ण *ref = refs[i];
		यदि (!म_भेद("tag", ref)) {
			यदि (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			यदि (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			अन्यथा
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} अन्यथा यदि (deleterefs) {
			यदि (म_अक्षर(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} अन्यथा यदि (!म_अक्षर(ref, ':')) {
			संरचना ref *matched = रिक्त;

			/* lazily grab local_refs */
			यदि (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			यदि (गन्ती_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			अन्यथा
				refspec_append_mapped(&rs, ref, remote, matched);
		} अन्यथा
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

स्थिर NORETURN शून्य die_थप_simple(संरचना branch *branch,
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
	अचर वर्ण *advice_pushdefault_maybe = "";
	अचर वर्ण *advice_automergesimple_maybe = "";
	अचर वर्ण *short_upstream = branch->merge[0]->src;
	संरचना repo_config_मानहरू *cfg = repo_config_मानहरू(the_repository);

	skip_prefix(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	यदि (cfg->थप_default == थप_DEFAULT_UNSPECIFIED)
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

स्थिर अचर वर्ण message_detached_head_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

स्थिर अचर वर्ण *get_upstream_ref(पूर्णाङ्क flags, संरचना branch *branch, अचर वर्ण *remote_name)
{
	यदि (branch->merge_nr == 0 && (flags & TRANSPORT_थप_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		फर्काऊ branch->refname;
	}

	यदि (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		अचर वर्ण *advice_autosetup_maybe = "";
		यदि (!(flags & TRANSPORT_थप_AUTO_UPSTREAM)) {
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

	फर्काऊ branch->merge[0]->src;
}

स्थिर शून्य setup_default_थप_refspecs(पूर्णाङ्क *flags, संरचना remote *remote)
{
	संरचना branch *branch;
	अचर वर्ण *dst;
	पूर्णाङ्क same_remote;
	संरचना repo_config_मानहरू *cfg = repo_config_मानहरू(the_repository);

	विकल्प (cfg->थप_default) {
	पक्ष थप_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		फर्काऊ;

	पक्ष थप_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		फर्काऊ;
	स्वभाव:
		रोक;
	}

	branch = branch_get(रिक्त);
	यदि (!branch)
		die(_(message_detached_head_die), remote->name);

	dst = branch->refname;
	same_remote = !म_भेद(remote->name, remote_for_branch(branch, रिक्त));

	विकल्प (cfg->थप_default) {
	स्वभाव:
	पक्ष थप_DEFAULT_UNSPECIFIED:
	पक्ष थप_DEFAULT_SIMPLE:
		यदि (!same_remote)
			रोक;
		यदि (म_भेद(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_थप_simple(branch, remote);
		रोक;

	पक्ष थप_DEFAULT_UPSTREAM:
		यदि (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		रोक;

	पक्ष थप_DEFAULT_CURRENT:
		रोक;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	यदि ((*flags & TRANSPORT_थप_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_थप_SET_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

स्थिर अचर वर्ण message_advice_pull_before_थप[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

स्थिर अचर वर्ण message_advice_checkout_pull_थप[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

स्थिर अचर वर्ण message_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

स्थिर अचर वर्ण message_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

स्थिर अचर वर्ण message_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

स्थिर अचर वर्ण message_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

स्थिर शून्य advise_pull_before_थप(शून्य)
{
	यदि (!advice_enabled(ADVICE_थप_NON_FF_CURRENT) || !advice_enabled(ADVICE_थप_UPDATE_REJECTED))
		फर्काऊ;
	advise(_(message_advice_pull_before_थप));
}

स्थिर शून्य advise_checkout_pull_थप(शून्य)
{
	यदि (!advice_enabled(ADVICE_थप_NON_FF_MATCHING) || !advice_enabled(ADVICE_थप_UPDATE_REJECTED))
		फर्काऊ;
	advise(_(message_advice_checkout_pull_थप));
}

स्थिर शून्य advise_ref_already_exists(शून्य)
{
	यदि (!advice_enabled(ADVICE_थप_ALREADY_EXISTS) || !advice_enabled(ADVICE_थप_UPDATE_REJECTED))
		फर्काऊ;
	advise(_(message_advice_ref_already_exists));
}

स्थिर शून्य advise_ref_fetch_first(शून्य)
{
	यदि (!advice_enabled(ADVICE_थप_FETCH_FIRST) || !advice_enabled(ADVICE_थप_UPDATE_REJECTED))
		फर्काऊ;
	advise(_(message_advice_ref_fetch_first));
}

स्थिर शून्य advise_ref_needs_force(शून्य)
{
	यदि (!advice_enabled(ADVICE_थप_NEEDS_FORCE) || !advice_enabled(ADVICE_थप_UPDATE_REJECTED))
		फर्काऊ;
	advise(_(message_advice_ref_needs_force));
}

स्थिर शून्य advise_ref_needs_update(शून्य)
{
	यदि (!advice_enabled(ADVICE_थप_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_थप_UPDATE_REJECTED))
		फर्काऊ;
	advise(_(message_advice_ref_needs_update));
}

स्थिर पूर्णाङ्क थप_with_options(संरचना transport *transport, संरचना refspec *rs,
			     पूर्णाङ्क flags)
{
	पूर्णाङ्क err;
	अचिन्हित पूर्णाङ्क reject_reasons;
	वर्ण *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	यदि (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : रिक्त);

	यदि (!is_empty_cas(&cas)) {
		यदि (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	यदि (verbosity > 0)
		ख_लिखो(मानक_त्रुटि, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_थप(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	यदि (err != 0) {
		ख_लिखो(मानक_त्रुटि, "%s", थप_get_color(थप_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		ख_लिखो(मानक_त्रुटि, "%s", थप_get_color(थप_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	मुक्त(anon_url);
	यदि (!err)
		फर्काऊ 0;

	यदि (reject_reasons & REJECT_NON_FF_HEAD) {
		advise_pull_before_थप();
	} अन्यथा यदि (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_थप();
	} अन्यथा यदि (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} अन्यथा यदि (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} अन्यथा यदि (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} अन्यथा यदि (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	फर्काऊ 1;
}

स्थिर पूर्णाङ्क do_थप(पूर्णाङ्क flags,
		   अचर संरचना string_list *थप_options,
		   संरचना remote *remote)
{
	पूर्णाङ्क errs;
	संरचना strvec *url;
	संरचना refspec *थप_refspec = &rs;

	यदि (थप_options->nr)
		flags |= TRANSPORT_थप_OPTIONS;

	यदि (!थप_refspec->nr && !(flags & TRANSPORT_थप_ALL)) {
		यदि (remote->थप.nr) {
			थप_refspec = &remote->थप;
		} अन्यथा यदि (!(flags & TRANSPORT_थप_MIRROR))
			setup_default_थप_refspecs(&flags, remote);
	}
	errs = 0;
	url = थप_url_of_remote(remote);
	लागि (माप_प्रकार i = 0; i < url->nr; i++) {
		संरचना transport *transport =
			transport_get(remote, url->v[i]);
		यदि (flags & TRANSPORT_थप_OPTIONS)
			transport->थप_options = थप_options;
		यदि (थप_with_options(transport, थप_refspec, flags))
			errs++;
	}
	फर्काऊ !!errs;
}

स्थिर पूर्णाङ्क option_parse_recurse_submodules(अचर संरचना option *opt,
				   अचर वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *recurse_submodules = opt->मान;

	यदि (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	अन्यथा {
		यदि (!म_भेद(arg, "only-is-on-demand")) {
			यदि (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} अन्यथा {
			*recurse_submodules = parse_थप_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	फर्काऊ 0;
}

स्थिर शून्य set_थप_cert_flags(पूर्णाङ्क *flags, पूर्णाङ्क v)
{
	विकल्प (v) {
	पक्ष SEND_PACK_थप_CERT_NEVER:
		*flags &= ~(TRANSPORT_थप_CERT_ALWAYS | TRANSPORT_थप_CERT_IF_ASKED);
		रोक;
	पक्ष SEND_PACK_थप_CERT_ALWAYS:
		*flags |= TRANSPORT_थप_CERT_ALWAYS;
		*flags &= ~TRANSPORT_थप_CERT_IF_ASKED;
		रोक;
	पक्ष SEND_PACK_थप_CERT_IF_ASKED:
		*flags |= TRANSPORT_थप_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_थप_CERT_ALWAYS;
		रोक;
	}
}


स्थिर पूर्णाङ्क git_थप_config(अचर वर्ण *k, अचर वर्ण *v,
			   अचर संरचना config_context *ctx, शून्य *cb)
{
	अचर वर्ण *slot_name;
	पूर्णाङ्क *flags = cb;

	यदि (!म_भेद(k, "push.followtags")) {
		यदि (git_config_bool(k, v))
			*flags |= TRANSPORT_थप_FOLLOW_TAGS;
		अन्यथा
			*flags &= ~TRANSPORT_थप_FOLLOW_TAGS;
		फर्काऊ 0;
	} अन्यथा यदि (!म_भेद(k, "push.autosetupremote")) {
		यदि (git_config_bool(k, v))
			*flags |= TRANSPORT_थप_AUTO_UPSTREAM;
		फर्काऊ 0;
	} अन्यथा यदि (!म_भेद(k, "push.gpgsign")) {
		विकल्प (git_parse_maybe_bool(v)) {
		पक्ष 0:
			set_थप_cert_flags(flags, SEND_PACK_थप_CERT_NEVER);
			रोक;
		पक्ष 1:
			set_थप_cert_flags(flags, SEND_PACK_थप_CERT_ALWAYS);
			रोक;
		स्वभाव:
			यदि (!strcasecmp(v, "if-asked"))
				set_थप_cert_flags(flags, SEND_PACK_थप_CERT_IF_ASKED);
			अन्यथा
				फर्काऊ error(_("invalid value for '%s'"), k);
		}
	} अन्यथा यदि (!म_भेद(k, "push.recursesubmodules")) {
		recurse_submodules = parse_थप_recurse_submodules_arg(k, v);
	} अन्यथा यदि (!म_भेद(k, "submodule.recurse")) {
		पूर्णाङ्क val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} अन्यथा यदि (!म_भेद(k, "push.pushoption")) {
		फर्काऊ parse_transport_option(k, v, &थप_options_config);
	} अन्यथा यदि (!म_भेद(k, "color.push")) {
		थप_use_color = git_config_colorbool(k, v);
		फर्काऊ 0;
	} अन्यथा यदि (skip_prefix(k, "color.push.", &slot_name)) {
		पूर्णाङ्क slot = parse_थप_color_slot(slot_name);
		यदि (slot < 0)
			फर्काऊ 0;
		यदि (!v)
			फर्काऊ config_error_nonbool(k);
		फर्काऊ color_parse(v, थप_colors[slot]);
	} अन्यथा यदि (!म_भेद(k, "push.useforceifincludes")) {
		यदि (git_config_bool(k, v))
			*flags |= TRANSPORT_थप_FORCE_IF_INCLUDES;
		अन्यथा
			*flags &= ~TRANSPORT_थप_FORCE_IF_INCLUDES;
		फर्काऊ 0;
	}

	फर्काऊ git_default_config(k, v, ctx, रिक्त);
}

स्थिर पूर्णाङ्क थप_multiple(संरचना string_list *list,
			 अचर संरचना string_list *थप_options,
			 पूर्णाङ्क flags,
			 पूर्णाङ्क tags,
			 अचर वर्ण **refspecs,
			 पूर्णाङ्क refspec_nr)
{
	पूर्णाङ्क result = 0;
	माप_प्रकार i;
	संरचना strvec argv = STRVEC_INIT;

	strvec_थप(&argv, "push");

	यदि (flags & TRANSPORT_थप_FORCE)
		strvec_थप(&argv, "--force");
	यदि (flags & TRANSPORT_थप_DRY_RUN)
		strvec_थप(&argv, "--dry-run");
	यदि (flags & TRANSPORT_थप_PORCELAIN)
		strvec_थप(&argv, "--porcelain");
	यदि (flags & TRANSPORT_थप_PRUNE)
		strvec_थप(&argv, "--prune");
	यदि (flags & TRANSPORT_थप_NO_HOOK)
		strvec_थप(&argv, "--no-verify");
	यदि (flags & TRANSPORT_थप_FOLLOW_TAGS)
		strvec_थप(&argv, "--follow-tags");
	यदि (flags & TRANSPORT_थप_SET_UPSTREAM)
		strvec_थप(&argv, "--set-upstream");
	यदि (flags & TRANSPORT_थप_FORCE_IF_INCLUDES)
		strvec_थप(&argv, "--force-if-includes");
	यदि (flags & TRANSPORT_थप_ALL)
		strvec_थप(&argv, "--all");
	यदि (flags & TRANSPORT_थप_MIRROR)
		strvec_थप(&argv, "--mirror");

	यदि (flags & TRANSPORT_थप_CERT_ALWAYS)
		strvec_थप(&argv, "--signed=yes");
	अन्यथा यदि (flags & TRANSPORT_थप_CERT_IF_ASKED)
		strvec_थप(&argv, "--signed=if-asked");
	यदि (!thin)
		strvec_थप(&argv, "--no-thin");

	यदि (deleterefs)
		strvec_थप(&argv, "--delete");

	यदि (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	यदि (verbosity >= 2)
		strvec_थप(&argv, "-v");
	यदि (verbosity >= 1)
		strvec_थप(&argv, "-v");
	अन्यथा यदि (verbosity < 0)
		strvec_थप(&argv, "-q");
	यदि (progress > 0)
		strvec_थप(&argv, "--progress");
	अन्यथा यदि (progress == 0)
		strvec_थप(&argv, "--no-progress");

	यदि (family == TRANSPORT_FAMILY_IPV4)
		strvec_थप(&argv, "--ipv4");
	अन्यथा यदि (family == TRANSPORT_FAMILY_IPV6)
		strvec_थप(&argv, "--ipv6");

	यदि (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_थप(&argv, "--recurse-submodules=check");
	अन्यथा यदि (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_थप(&argv, "--recurse-submodules=on-demand");
	अन्यथा यदि (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_थप(&argv, "--recurse-submodules=only");
	अन्यथा यदि (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_थप(&argv, "--recurse-submodules=no");


	यदि (tags)
		strvec_थप(&argv, "--tags");

	लागि (i = 0; i < थप_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     थप_options->वस्तुहरू[i].string);

	लागि (i = 0; i < cas.nr; i++) {
		यदि (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} अन्यथा यदि (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} अन्यथा {
			strvec_थप(&argv, "--force-with-lease");
		}
	}

	लागि (i = 0; i < list->nr; i++) {
		अचर वर्ण *name = list->वस्तुहरू[i].string;
		संरचना child_process cmd = CHILD_PROCESS_INIT;
		पूर्णाङ्क j;

		strvec_pushv(&cmd.args, argv.v);
		strvec_थप(&cmd.args, name);

		लागि (j = 0; j < refspec_nr; j++)
			strvec_थप(&cmd.args, refspecs[j]);

		यदि (verbosity >= 0)
			म_लिखो(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		यदि (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			result = 1;
		}
	}

	strvec_clear(&argv);
	फर्काऊ result;
}

स्थिर शून्य die_if_repo_looks_like_ref(अचर वर्ण *repo)
{
	अचर वर्ण *slash = म_अक्षर(repo, '/');
	संरचना strbuf name = STRBUF_INIT;
	पूर्णाङ्क code;

	यदि (!slash || !slash[1] || file_exists(repo))
		फर्काऊ;

	strbuf_add(&name, repo, slash - repo);
	यदि (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		फर्काऊ;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_थप_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	निकास(code);
}

पूर्णाङ्क cmd_थप(पूर्णाङ्क argc,
	     अचर वर्ण **argv,
	     अचर वर्ण *prefix,
	     संरचना repository *repository UNUSED)
{
	पूर्णाङ्क flags = 0;
	पूर्णाङ्क tags = 0;
	पूर्णाङ्क थप_cert = -1;
	पूर्णाङ्क rc = 0;
	पूर्णाङ्क base_flags;
	अचर वर्ण *repo = रिक्त;	/* default repository */
	संरचना string_list थप_options_cmdline = STRING_LIST_INIT_DUP;
	संरचना string_list remote_group = STRING_LIST_INIT_DUP;
	संरचना string_list *थप_options;
	अचर संरचना string_list_item *item;

	संरचना option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_BIT( 0 , "all", &flags, N_("push all branches"), TRANSPORT_थप_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_BIT( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_थप_MIRROR|TRANSPORT_थप_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &tags, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_BIT('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_थप_DRY_RUN),
		OPT_BIT( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_थप_PORCELAIN),
		OPT_BIT('f', "force", &flags, N_("force updates"), TRANSPORT_थप_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_थप_cas_option),
		OPT_BIT(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_थप_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_BIT('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_थप_SET_UPSTREAM),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_BIT(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_थप_PRUNE),
		OPT_BIT(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_थप_NO_HOOK),
		OPT_BIT(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_थप_FOLLOW_TAGS),
		OPT_CALLBACK_F(0, "signed", &थप_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_थप_signed),
		OPT_BIT(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_थप_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &थप_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_END()
	};

	packet_trace_identity("push");
	repo_config(the_repository, git_थप_config, &flags);
	argc = parse_options(argc, argv, prefix, options, थप_usage, 0);
	थप_options = (थप_options_cmdline.nr
		? &थप_options_cmdline
		: &थप_options_config);
	set_थप_cert_flags(&flags, थप_cert);

	refspec_init_थप(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  tags, "--tags",
				  flags & TRANSPORT_थप_ALL, "--all/--branches",
				  flags & TRANSPORT_थप_MIRROR, "--mirror");
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
		यदि (!add_remote_or_group(repo, &remote_group)) {
			संरचना remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			यदि (advice_enabled(ADVICE_थप_REPO_LOOKS_LIKE_REF))
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
			string_list_append(&remote_group, r->name);
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
		string_list_append(&remote_group, r->name);
	}

	यदि (!is_empty_cas(&cas) && (flags & TRANSPORT_थप_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, थप_options)
		यदि (म_अक्षर(item->string, '\n'))
			die(_("push options must not have new line characters"));

	यदि (remote_group.nr == 1) {
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
			पूर्णाङ्क inner_flags = base_flags;
			संरचना remote *r = pushremote_get(remote_group.वस्तुहरू[0].string);
			यदि (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.वस्तुहरू[0].string);

			यदि (r->mirror)
				inner_flags |= (TRANSPORT_थप_MIRROR|TRANSPORT_थप_FORCE);

			यदि (inner_flags & TRANSPORT_थप_ALL) {
				यदि (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			यदि (inner_flags & TRANSPORT_थप_MIRROR) {
				यदि (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (संरचना refspec) REFSPEC_INIT_थप(the_hash_algo);

			यदि (tags)
				refspec_append(&rs, "refs/tags/*");
			यदि (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_थप(inner_flags, थप_options, r);
		}
	} अन्यथा {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		यदि (flags & TRANSPORT_थप_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = थप_multiple(&remote_group, थप_options, flags,
				   tags,
				   argc > 1 ? argv + 1 : रिक्त,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&थप_options_cmdline, 0);
	string_list_clear(&थप_options_config, 0);
	string_list_clear(&remote_group, 0);
	clear_cas_option(&cas);

	यदि (rc == -1)
		usage_with_options(थप_usage, options);
	अन्यथा
		फर्काऊ rc;
}
