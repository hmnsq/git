/*
 * "git push"
 */

#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "branch.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "refspec.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "remote.h"
#અન્તર્ભાવ "transport.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "pkt-line.h"
#અન્તર્ભાવ "submodule.h"
#અન્તર્ભાવ "submodule-config.h"
#અન્તર્ભાવ "send-pack.h"
#અન્તર્ભાવ "trace2.h"
#અન્તર્ભાવ "color.h"

સ્થિર અચલ અક્ષર * અચલ ધકેલો_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	ખાલી,
};

સ્થિર ગણના git_colorbool ધકેલો_use_color = GIT_COLOR_UNKNOWN;
સ્થિર અક્ષર ધકેલો_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

ગણના color_ધકેલો {
	ધકેલો_COLOR_RESET = 0,
	ધકેલો_COLOR_ERROR = 1
};

સ્થિર પૂર્ણાંક parse_ધકેલો_color_slot(અચલ અક્ષર *slot)
{
	જો (!strcasecmp(slot, "reset"))
		પાછા ધકેલો_COLOR_RESET;
	જો (!strcasecmp(slot, "error"))
		પાછા ધકેલો_COLOR_ERROR;
	પાછા -1;
}

સ્થિર અચલ અક્ષર *ધકેલો_get_color(ગણના color_ધકેલો ix)
{
	જો (want_color_stderr(ધકેલો_use_color))
		પાછા ધકેલો_colors[ix];
	પાછા "";
}

સ્થિર પૂર્ણાંક thin = 1;
સ્થિર પૂર્ણાંક deleterefs;
સ્થિર અચલ અક્ષર *receivepack;
સ્થિર પૂર્ણાંક verbosity;
સ્થિર પૂર્ણાંક progress = -1;
સ્થિર પૂર્ણાંક recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
સ્થિર ગણના transport_family family;

સ્થિર સંરચના ધકેલો_cas_option cas;

સ્થિર સંરચના refspec rs;

સ્થિર સંરચના string_list ધકેલો_options_config = STRING_LIST_INIT_DUP;

સ્થિર શૂન્ય refspec_append_mapped(સંરચના refspec *refspec, અચલ અક્ષર *ref,
				  સંરચના remote *remote, સંરચના ref *matched)
{
	અચલ અક્ષર *branch_name;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	જો (remote->ધકેલો.nr) {
		સંરચના refspec_item query = {
			.src = matched->name,
		};

		જો (!refspec_find_match(&remote->ધકેલો, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			મુક્ત(query.dst);
			પાછા;
		}
	}

	જો (cfg->ધકેલો_default == ધકેલો_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		સંરચના branch *branch = branch_get(branch_name);
		જો (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			પાછા;
		}
	}

	refspec_append(refspec, ref);
}

સ્થિર શૂન્ય set_refspecs(અચલ અક્ષર **refs, પૂર્ણાંક nr, સંરચના remote *remote)
{
	સંરચના ref *local_refs = ખાલી;
	પૂર્ણાંક i;

	માટે (i = 0; i < nr; i++) {
		અચલ અક્ષર *ref = refs[i];
		જો (!strcmp("tag", ref)) {
			જો (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			જો (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			નહીંતર
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} નહીંતર જો (deleterefs) {
			જો (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} નહીંતર જો (!strchr(ref, ':')) {
			સંરચના ref *matched = ખાલી;

			/* lazily grab local_refs */
			જો (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			જો (ગણતરી_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			નહીંતર
				refspec_append_mapped(&rs, ref, remote, matched);
		} નહીંતર
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

સ્થિર NORETURN શૂન્ય die_ધકેલો_simple(સંરચના branch *branch,
				     સંરચના remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	અચલ અક્ષર *advice_pushdefault_maybe = "";
	અચલ અક્ષર *advice_automergesimple_maybe = "";
	અચલ અક્ષર *short_upstream = branch->merge[0]->src;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	skip_prefix(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	જો (cfg->ધકેલો_default == ધકેલો_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	જો (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

સ્થિર અચલ અક્ષર message_detached_head_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

સ્થિર અચલ અક્ષર *get_upstream_ref(પૂર્ણાંક flags, સંરચના branch *branch, અચલ અક્ષર *remote_name)
{
	જો (branch->merge_nr == 0 && (flags & TRANSPORT_ધકેલો_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		પાછા branch->refname;
	}

	જો (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		અચલ અક્ષર *advice_autosetup_maybe = "";
		જો (!(flags & TRANSPORT_ધકેલો_AUTO_UPSTREAM)) {
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
	જો (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	પાછા branch->merge[0]->src;
}

સ્થિર શૂન્ય setup_default_ધકેલો_refspecs(પૂર્ણાંક *flags, સંરચના remote *remote)
{
	સંરચના branch *branch;
	અચલ અક્ષર *dst;
	પૂર્ણાંક same_remote;
	સંરચના repo_config_કિંમતો *cfg = repo_config_કિંમતો(the_repository);

	પસંદગી (cfg->ધકેલો_default) {
	સ્થિતિ ધકેલો_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		પાછા;

	સ્થિતિ ધકેલો_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		પાછા;
	મૂળભૂત:
		તોડો;
	}

	branch = branch_get(ખાલી);
	જો (!branch)
		die(_(message_detached_head_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, ખાલી));

	પસંદગી (cfg->ધકેલો_default) {
	મૂળભૂત:
	સ્થિતિ ધકેલો_DEFAULT_UNSPECIFIED:
	સ્થિતિ ધકેલો_DEFAULT_SIMPLE:
		જો (!same_remote)
			તોડો;
		જો (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_ધકેલો_simple(branch, remote);
		તોડો;

	સ્થિતિ ધકેલો_DEFAULT_UPSTREAM:
		જો (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		તોડો;

	સ્થિતિ ધકેલો_DEFAULT_CURRENT:
		તોડો;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	જો ((*flags & TRANSPORT_ધકેલો_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_ધકેલો_SET_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

સ્થિર અચલ અક્ષર message_advice_pull_before_ધકેલો[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

સ્થિર અચલ અક્ષર message_advice_checkout_pull_ધકેલો[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

સ્થિર અચલ અક્ષર message_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

સ્થિર અચલ અક્ષર message_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

સ્થિર અચલ અક્ષર message_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

સ્થિર અચલ અક્ષર message_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

સ્થિર શૂન્ય advise_pull_before_ધકેલો(શૂન્ય)
{
	જો (!advice_enabled(ADVICE_ધકેલો_NON_FF_CURRENT) || !advice_enabled(ADVICE_ધકેલો_UPDATE_REJECTED))
		પાછા;
	advise(_(message_advice_pull_before_ધકેલો));
}

સ્થિર શૂન્ય advise_checkout_pull_ધકેલો(શૂન્ય)
{
	જો (!advice_enabled(ADVICE_ધકેલો_NON_FF_MATCHING) || !advice_enabled(ADVICE_ધકેલો_UPDATE_REJECTED))
		પાછા;
	advise(_(message_advice_checkout_pull_ધકેલો));
}

સ્થિર શૂન્ય advise_ref_already_exists(શૂન્ય)
{
	જો (!advice_enabled(ADVICE_ધકેલો_ALREADY_EXISTS) || !advice_enabled(ADVICE_ધકેલો_UPDATE_REJECTED))
		પાછા;
	advise(_(message_advice_ref_already_exists));
}

સ્થિર શૂન્ય advise_ref_fetch_first(શૂન્ય)
{
	જો (!advice_enabled(ADVICE_ધકેલો_FETCH_FIRST) || !advice_enabled(ADVICE_ધકેલો_UPDATE_REJECTED))
		પાછા;
	advise(_(message_advice_ref_fetch_first));
}

સ્થિર શૂન્ય advise_ref_needs_force(શૂન્ય)
{
	જો (!advice_enabled(ADVICE_ધકેલો_NEEDS_FORCE) || !advice_enabled(ADVICE_ધકેલો_UPDATE_REJECTED))
		પાછા;
	advise(_(message_advice_ref_needs_force));
}

સ્થિર શૂન્ય advise_ref_needs_update(શૂન્ય)
{
	જો (!advice_enabled(ADVICE_ધકેલો_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_ધકેલો_UPDATE_REJECTED))
		પાછા;
	advise(_(message_advice_ref_needs_update));
}

સ્થિર પૂર્ણાંક ધકેલો_with_options(સંરચના transport *transport, સંરચના refspec *rs,
			     પૂર્ણાંક flags)
{
	પૂર્ણાંક err;
	unsigned પૂર્ણાંક reject_reasons;
	અક્ષર *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	જો (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : ખાલી);

	જો (!is_empty_cas(&cas)) {
		જો (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	જો (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_ધકેલો(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	જો (err != 0) {
		fprintf(stderr, "%s", ધકેલો_get_color(ધકેલો_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", ધકેલો_get_color(ધકેલો_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	મુક્ત(anon_url);
	જો (!err)
		પાછા 0;

	જો (reject_reasons & REJECT_NON_FF_HEAD) {
		advise_pull_before_ધકેલો();
	} નહીંતર જો (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_ધકેલો();
	} નહીંતર જો (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} નહીંતર જો (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} નહીંતર જો (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} નહીંતર જો (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	પાછા 1;
}

સ્થિર પૂર્ણાંક do_ધકેલો(પૂર્ણાંક flags,
		   અચલ સંરચના string_list *ધકેલો_options,
		   સંરચના remote *remote)
{
	પૂર્ણાંક errs;
	સંરચના strvec *url;
	સંરચના refspec *ધકેલો_refspec = &rs;

	જો (ધકેલો_options->nr)
		flags |= TRANSPORT_ધકેલો_OPTIONS;

	જો (!ધકેલો_refspec->nr && !(flags & TRANSPORT_ધકેલો_ALL)) {
		જો (remote->ધકેલો.nr) {
			ધકેલો_refspec = &remote->ધકેલો;
		} નહીંતર જો (!(flags & TRANSPORT_ધકેલો_MIRROR))
			setup_default_ધકેલો_refspecs(&flags, remote);
	}
	errs = 0;
	url = ધકેલો_url_of_remote(remote);
	માટે (size_t i = 0; i < url->nr; i++) {
		સંરચના transport *transport =
			transport_get(remote, url->v[i]);
		જો (flags & TRANSPORT_ધકેલો_OPTIONS)
			transport->ધકેલો_options = ધકેલો_options;
		જો (ધકેલો_with_options(transport, ધકેલો_refspec, flags))
			errs++;
	}
	પાછા !!errs;
}

સ્થિર પૂર્ણાંક option_parse_recurse_submodules(અચલ સંરચના option *opt,
				   અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	પૂર્ણાંક *recurse_submodules = opt->કિંમત;

	જો (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	નહીંતર {
		જો (!strcmp(arg, "only-is-on-demand")) {
			જો (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} નહીંતર {
			*recurse_submodules = parse_ધકેલો_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	પાછા 0;
}

સ્થિર શૂન્ય set_ધકેલો_cert_flags(પૂર્ણાંક *flags, પૂર્ણાંક v)
{
	પસંદગી (v) {
	સ્થિતિ SEND_PACK_ધકેલો_CERT_NEVER:
		*flags &= ~(TRANSPORT_ધકેલો_CERT_ALWAYS | TRANSPORT_ધકેલો_CERT_IF_ASKED);
		તોડો;
	સ્થિતિ SEND_PACK_ધકેલો_CERT_ALWAYS:
		*flags |= TRANSPORT_ધકેલો_CERT_ALWAYS;
		*flags &= ~TRANSPORT_ધકેલો_CERT_IF_ASKED;
		તોડો;
	સ્થિતિ SEND_PACK_ધકેલો_CERT_IF_ASKED:
		*flags |= TRANSPORT_ધકેલો_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_ધકેલો_CERT_ALWAYS;
		તોડો;
	}
}


સ્થિર પૂર્ણાંક git_ધકેલો_config(અચલ અક્ષર *k, અચલ અક્ષર *v,
			   અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	અચલ અક્ષર *slot_name;
	પૂર્ણાંક *flags = cb;

	જો (!strcmp(k, "push.followtags")) {
		જો (git_config_bool(k, v))
			*flags |= TRANSPORT_ધકેલો_FOLLOW_TAGS;
		નહીંતર
			*flags &= ~TRANSPORT_ધકેલો_FOLLOW_TAGS;
		પાછા 0;
	} નહીંતર જો (!strcmp(k, "push.autosetupremote")) {
		જો (git_config_bool(k, v))
			*flags |= TRANSPORT_ધકેલો_AUTO_UPSTREAM;
		પાછા 0;
	} નહીંતર જો (!strcmp(k, "push.gpgsign")) {
		પસંદગી (git_parse_maybe_bool(v)) {
		સ્થિતિ 0:
			set_ધકેલો_cert_flags(flags, SEND_PACK_ધકેલો_CERT_NEVER);
			તોડો;
		સ્થિતિ 1:
			set_ધકેલો_cert_flags(flags, SEND_PACK_ધકેલો_CERT_ALWAYS);
			તોડો;
		મૂળભૂત:
			જો (!strcasecmp(v, "if-asked"))
				set_ધકેલો_cert_flags(flags, SEND_PACK_ધકેલો_CERT_IF_ASKED);
			નહીંતર
				પાછા error(_("invalid value for '%s'"), k);
		}
	} નહીંતર જો (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_ધકેલો_recurse_submodules_arg(k, v);
	} નહીંતર જો (!strcmp(k, "submodule.recurse")) {
		પૂર્ણાંક val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} નહીંતર જો (!strcmp(k, "push.pushoption")) {
		પાછા parse_transport_option(k, v, &ધકેલો_options_config);
	} નહીંતર જો (!strcmp(k, "color.push")) {
		ધકેલો_use_color = git_config_colorbool(k, v);
		પાછા 0;
	} નહીંતર જો (skip_prefix(k, "color.push.", &slot_name)) {
		પૂર્ણાંક slot = parse_ધકેલો_color_slot(slot_name);
		જો (slot < 0)
			પાછા 0;
		જો (!v)
			પાછા config_error_nonbool(k);
		પાછા color_parse(v, ધકેલો_colors[slot]);
	} નહીંતર જો (!strcmp(k, "push.useforceifincludes")) {
		જો (git_config_bool(k, v))
			*flags |= TRANSPORT_ધકેલો_FORCE_IF_INCLUDES;
		નહીંતર
			*flags &= ~TRANSPORT_ધકેલો_FORCE_IF_INCLUDES;
		પાછા 0;
	}

	પાછા git_default_config(k, v, ctx, ખાલી);
}

સ્થિર પૂર્ણાંક ધકેલો_multiple(સંરચના string_list *list,
			 અચલ સંરચના string_list *ધકેલો_options,
			 પૂર્ણાંક flags,
			 પૂર્ણાંક tags,
			 અચલ અક્ષર **refspecs,
			 પૂર્ણાંક refspec_nr)
{
	પૂર્ણાંક result = 0;
	size_t i;
	સંરચના strvec argv = STRVEC_INIT;

	strvec_ધકેલો(&argv, "push");

	જો (flags & TRANSPORT_ધકેલો_FORCE)
		strvec_ધકેલો(&argv, "--force");
	જો (flags & TRANSPORT_ધકેલો_DRY_RUN)
		strvec_ધકેલો(&argv, "--dry-run");
	જો (flags & TRANSPORT_ધકેલો_PORCELAIN)
		strvec_ધકેલો(&argv, "--porcelain");
	જો (flags & TRANSPORT_ધકેલો_PRUNE)
		strvec_ધકેલો(&argv, "--prune");
	જો (flags & TRANSPORT_ધકેલો_NO_HOOK)
		strvec_ધકેલો(&argv, "--no-verify");
	જો (flags & TRANSPORT_ધકેલો_FOLLOW_TAGS)
		strvec_ધકેલો(&argv, "--follow-tags");
	જો (flags & TRANSPORT_ધકેલો_SET_UPSTREAM)
		strvec_ધકેલો(&argv, "--set-upstream");
	જો (flags & TRANSPORT_ધકેલો_FORCE_IF_INCLUDES)
		strvec_ધકેલો(&argv, "--force-if-includes");
	જો (flags & TRANSPORT_ધકેલો_ALL)
		strvec_ધકેલો(&argv, "--all");
	જો (flags & TRANSPORT_ધકેલો_MIRROR)
		strvec_ધકેલો(&argv, "--mirror");

	જો (flags & TRANSPORT_ધકેલો_CERT_ALWAYS)
		strvec_ધકેલો(&argv, "--signed=yes");
	નહીંતર જો (flags & TRANSPORT_ધકેલો_CERT_IF_ASKED)
		strvec_ધકેલો(&argv, "--signed=if-asked");
	જો (!thin)
		strvec_ધકેલો(&argv, "--no-thin");

	જો (deleterefs)
		strvec_ધકેલો(&argv, "--delete");

	જો (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	જો (verbosity >= 2)
		strvec_ધકેલો(&argv, "-v");
	જો (verbosity >= 1)
		strvec_ધકેલો(&argv, "-v");
	નહીંતર જો (verbosity < 0)
		strvec_ધકેલો(&argv, "-q");
	જો (progress > 0)
		strvec_ધકેલો(&argv, "--progress");
	નહીંતર જો (progress == 0)
		strvec_ધકેલો(&argv, "--no-progress");

	જો (family == TRANSPORT_FAMILY_IPV4)
		strvec_ધકેલો(&argv, "--ipv4");
	નહીંતર જો (family == TRANSPORT_FAMILY_IPV6)
		strvec_ધકેલો(&argv, "--ipv6");

	જો (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_ધકેલો(&argv, "--recurse-submodules=check");
	નહીંતર જો (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ધકેલો(&argv, "--recurse-submodules=on-demand");
	નહીંતર જો (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_ધકેલો(&argv, "--recurse-submodules=only");
	નહીંતર જો (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ધકેલો(&argv, "--recurse-submodules=no");


	જો (tags)
		strvec_ધકેલો(&argv, "--tags");

	માટે (i = 0; i < ધકેલો_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     ધકેલો_options->વસ્તુઓ[i].string);

	માટે (i = 0; i < cas.nr; i++) {
		જો (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} નહીંતર જો (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} નહીંતર {
			strvec_ધકેલો(&argv, "--force-with-lease");
		}
	}

	માટે (i = 0; i < list->nr; i++) {
		અચલ અક્ષર *name = list->વસ્તુઓ[i].string;
		સંરચના child_process cmd = CHILD_PROCESS_INIT;
		પૂર્ણાંક j;

		strvec_pushv(&cmd.args, argv.v);
		strvec_ધકેલો(&cmd.args, name);

		માટે (j = 0; j < refspec_nr; j++)
			strvec_ધકેલો(&cmd.args, refspecs[j]);

		જો (verbosity >= 0)
			printf(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		જો (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			result = 1;
		}
	}

	strvec_clear(&argv);
	પાછા result;
}

સ્થિર શૂન્ય die_if_repo_looks_like_ref(અચલ અક્ષર *repo)
{
	અચલ અક્ષર *slash = strchr(repo, '/');
	સંરચના strbuf name = STRBUF_INIT;
	પૂર્ણાંક code;

	જો (!slash || !slash[1] || file_exists(repo))
		પાછા;

	strbuf_add(&name, repo, slash - repo);
	જો (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		પાછા;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_ધકેલો_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	exit(code);
}

પૂર્ણાંક cmd_ધકેલો(પૂર્ણાંક argc,
	     અચલ અક્ષર **argv,
	     અચલ અક્ષર *prefix,
	     સંરચના repository *repository UNUSED)
{
	પૂર્ણાંક flags = 0;
	પૂર્ણાંક tags = 0;
	પૂર્ણાંક ધકેલો_cert = -1;
	પૂર્ણાંક rc = 0;
	પૂર્ણાંક base_flags;
	અચલ અક્ષર *repo = ખાલી;	/* default repository */
	સંરચના string_list ધકેલો_options_cmdline = STRING_LIST_INIT_DUP;
	સંરચના string_list remote_group = STRING_LIST_INIT_DUP;
	સંરચના string_list *ધકેલો_options;
	અચલ સંરચના string_list_item *item;

	સંરચના option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_BIT( 0 , "all", &flags, N_("push all branches"), TRANSPORT_ધકેલો_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_BIT( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_ધકેલો_MIRROR|TRANSPORT_ધકેલો_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &tags, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_BIT('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_ધકેલો_DRY_RUN),
		OPT_BIT( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_ધકેલો_PORCELAIN),
		OPT_BIT('f', "force", &flags, N_("force updates"), TRANSPORT_ધકેલો_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_ધકેલો_cas_option),
		OPT_BIT(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_ધકેલો_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_BIT('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_ધકેલો_SET_UPSTREAM),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_BIT(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_ધકેલો_PRUNE),
		OPT_BIT(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_ધકેલો_NO_HOOK),
		OPT_BIT(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_ધકેલો_FOLLOW_TAGS),
		OPT_CALLBACK_F(0, "signed", &ધકેલો_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_ધકેલો_signed),
		OPT_BIT(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_ધકેલો_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &ધકેલો_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_END()
	};

	packet_trace_identity("push");
	repo_config(the_repository, git_ધકેલો_config, &flags);
	argc = parse_options(argc, argv, prefix, options, ધકેલો_usage, 0);
	ધકેલો_options = (ધકેલો_options_cmdline.nr
		? &ધકેલો_options_cmdline
		: &ધકેલો_options_config);
	set_ધકેલો_cert_flags(&flags, ધકેલો_cert);

	refspec_init_ધકેલો(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  tags, "--tags",
				  flags & TRANSPORT_ધકેલો_ALL, "--all/--branches",
				  flags & TRANSPORT_ધકેલો_MIRROR, "--mirror");
	જો (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	જો (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	નહીંતર જો (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	નહીંતર જો (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	જો (argc > 0)
		repo = argv[0];

	જો (repo) {
		જો (!add_remote_or_group(repo, &remote_group)) {
			સંરચના remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			જો (advice_enabled(ADVICE_ધકેલો_REPO_LOOKS_LIKE_REF))
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
			જો (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_group, r->name);
		}
	} નહીંતર {
		સંરચના remote *r = pushremote_get(ખાલી);
		જો (!r)
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

	જો (!is_empty_cas(&cas) && (flags & TRANSPORT_ધકેલો_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, ધકેલો_options)
		જો (strchr(item->string, '\n'))
			die(_("push options must not have new line characters"));

	જો (remote_group.nr == 1) {
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
			પૂર્ણાંક inner_flags = base_flags;
			સંરચના remote *r = pushremote_get(remote_group.વસ્તુઓ[0].string);
			જો (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.વસ્તુઓ[0].string);

			જો (r->mirror)
				inner_flags |= (TRANSPORT_ધકેલો_MIRROR|TRANSPORT_ધકેલો_FORCE);

			જો (inner_flags & TRANSPORT_ધકેલો_ALL) {
				જો (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			જો (inner_flags & TRANSPORT_ધકેલો_MIRROR) {
				જો (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (સંરચના refspec) REFSPEC_INIT_ધકેલો(the_hash_algo);

			જો (tags)
				refspec_append(&rs, "refs/tags/*");
			જો (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_ધકેલો(inner_flags, ધકેલો_options, r);
		}
	} નહીંતર {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		જો (flags & TRANSPORT_ધકેલો_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = ધકેલો_multiple(&remote_group, ધકેલો_options, flags,
				   tags,
				   argc > 1 ? argv + 1 : ખાલી,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&ધકેલો_options_cmdline, 0);
	string_list_clear(&ધકેલો_options_config, 0);
	string_list_clear(&remote_group, 0);
	clear_cas_option(&cas);

	જો (rc == -1)
		usage_with_options(ધકેલો_usage, options);
	નહીંતર
		પાછા rc;
}
