/*
 * "git push"
 */

#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "branch.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "refspec.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "remote.h"
#அந்தர்பாவ "transport.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "pkt-line.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "submodule-config.h"
#அந்தர்பாவ "send-pack.h"
#அந்தர்பாவ "trace2.h"
#அந்தர்பாவ "color.h"

நிலையான மாறிலி எழுத்து * மாறிலி தள்ளு_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	வெற்று,
};

நிலையான எண்ணிக்கை git_colorbool தள்ளு_use_color = GIT_COLOR_UNKNOWN;
நிலையான எழுத்து தள்ளு_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

எண்ணிக்கை color_தள்ளு {
	தள்ளு_COLOR_RESET = 0,
	தள்ளு_COLOR_ERROR = 1
};

நிலையான முழு parse_தள்ளு_color_slot(மாறிலி எழுத்து *slot)
{
	எனில் (!strcasecmp(slot, "reset"))
		திரும்பு தள்ளு_COLOR_RESET;
	எனில் (!strcasecmp(slot, "error"))
		திரும்பு தள்ளு_COLOR_ERROR;
	திரும்பு -1;
}

நிலையான மாறிலி எழுத்து *தள்ளு_get_color(எண்ணிக்கை color_தள்ளு ix)
{
	எனில் (want_color_stderr(தள்ளு_use_color))
		திரும்பு தள்ளு_colors[ix];
	திரும்பு "";
}

நிலையான முழு thin = 1;
நிலையான முழு deleterefs;
நிலையான மாறிலி எழுத்து *receivepack;
நிலையான முழு verbosity;
நிலையான முழு progress = -1;
நிலையான முழு recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
நிலையான எண்ணிக்கை transport_family family;

நிலையான கட்டமைப்பு தள்ளு_cas_option cas;

நிலையான கட்டமைப்பு refspec rs;

நிலையான கட்டமைப்பு string_list தள்ளு_options_config = STRING_LIST_INIT_DUP;

நிலையான வெறுமை refspec_append_mapped(கட்டமைப்பு refspec *refspec, மாறிலி எழுத்து *ref,
				  கட்டமைப்பு remote *remote, கட்டமைப்பு ref *matched)
{
	மாறிலி எழுத்து *branch_name;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	எனில் (remote->தள்ளு.nr) {
		கட்டமைப்பு refspec_item query = {
			.src = matched->name,
		};

		எனில் (!refspec_find_match(&remote->தள்ளு, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			விடுவி(query.dst);
			திரும்பு;
		}
	}

	எனில் (cfg->தள்ளு_default == தள்ளு_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		கட்டமைப்பு branch *branch = branch_get(branch_name);
		எனில் (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			திரும்பு;
		}
	}

	refspec_append(refspec, ref);
}

நிலையான வெறுமை set_refspecs(மாறிலி எழுத்து **refs, முழு nr, கட்டமைப்பு remote *remote)
{
	கட்டமைப்பு ref *local_refs = வெற்று;
	முழு i;

	க்கு (i = 0; i < nr; i++) {
		மாறிலி எழுத்து *ref = refs[i];
		எனில் (!strcmp("tag", ref)) {
			எனில் (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			எனில் (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			இல்லையெனில்
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} இல்லையெனில் எனில் (deleterefs) {
			எனில் (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} இல்லையெனில் எனில் (!strchr(ref, ':')) {
			கட்டமைப்பு ref *matched = வெற்று;

			/* lazily grab local_refs */
			எனில் (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			எனில் (மொத்தம்_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			இல்லையெனில்
				refspec_append_mapped(&rs, ref, remote, matched);
		} இல்லையெனில்
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

நிலையான NORETURN வெறுமை die_தள்ளு_simple(கட்டமைப்பு branch *branch,
				     கட்டமைப்பு remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	மாறிலி எழுத்து *advice_pushdefault_maybe = "";
	மாறிலி எழுத்து *advice_automergesimple_maybe = "";
	மாறிலி எழுத்து *short_upstream = branch->merge[0]->src;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	skip_prefix(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	எனில் (cfg->தள்ளு_default == தள்ளு_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	எனில் (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

நிலையான மாறிலி எழுத்து message_detached_head_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

நிலையான மாறிலி எழுத்து *get_upstream_ref(முழு flags, கட்டமைப்பு branch *branch, மாறிலி எழுத்து *remote_name)
{
	எனில் (branch->merge_nr == 0 && (flags & TRANSPORT_தள்ளு_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		திரும்பு branch->refname;
	}

	எனில் (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		மாறிலி எழுத்து *advice_autosetup_maybe = "";
		எனில் (!(flags & TRANSPORT_தள்ளு_AUTO_UPSTREAM)) {
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
	எனில் (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	திரும்பு branch->merge[0]->src;
}

நிலையான வெறுமை setup_default_தள்ளு_refspecs(முழு *flags, கட்டமைப்பு remote *remote)
{
	கட்டமைப்பு branch *branch;
	மாறிலி எழுத்து *dst;
	முழு same_remote;
	கட்டமைப்பு repo_config_மதிப்புகள் *cfg = repo_config_மதிப்புகள்(the_repository);

	தேர்வு (cfg->தள்ளு_default) {
	நிலை தள்ளு_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		திரும்பு;

	நிலை தள்ளு_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		திரும்பு;
	இயல்பு:
		நிறுத்து;
	}

	branch = branch_get(வெற்று);
	எனில் (!branch)
		die(_(message_detached_head_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, வெற்று));

	தேர்வு (cfg->தள்ளு_default) {
	இயல்பு:
	நிலை தள்ளு_DEFAULT_UNSPECIFIED:
	நிலை தள்ளு_DEFAULT_SIMPLE:
		எனில் (!same_remote)
			நிறுத்து;
		எனில் (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_தள்ளு_simple(branch, remote);
		நிறுத்து;

	நிலை தள்ளு_DEFAULT_UPSTREAM:
		எனில் (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		நிறுத்து;

	நிலை தள்ளு_DEFAULT_CURRENT:
		நிறுத்து;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	எனில் ((*flags & TRANSPORT_தள்ளு_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_தள்ளு_SET_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

நிலையான மாறிலி எழுத்து message_advice_pull_before_தள்ளு[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

நிலையான மாறிலி எழுத்து message_advice_checkout_pull_தள்ளு[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

நிலையான மாறிலி எழுத்து message_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

நிலையான மாறிலி எழுத்து message_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

நிலையான மாறிலி எழுத்து message_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

நிலையான மாறிலி எழுத்து message_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

நிலையான வெறுமை advise_pull_before_தள்ளு(வெறுமை)
{
	எனில் (!advice_enabled(ADVICE_தள்ளு_NON_FF_CURRENT) || !advice_enabled(ADVICE_தள்ளு_UPDATE_REJECTED))
		திரும்பு;
	advise(_(message_advice_pull_before_தள்ளு));
}

நிலையான வெறுமை advise_checkout_pull_தள்ளு(வெறுமை)
{
	எனில் (!advice_enabled(ADVICE_தள்ளு_NON_FF_MATCHING) || !advice_enabled(ADVICE_தள்ளு_UPDATE_REJECTED))
		திரும்பு;
	advise(_(message_advice_checkout_pull_தள்ளு));
}

நிலையான வெறுமை advise_ref_already_exists(வெறுமை)
{
	எனில் (!advice_enabled(ADVICE_தள்ளு_ALREADY_EXISTS) || !advice_enabled(ADVICE_தள்ளு_UPDATE_REJECTED))
		திரும்பு;
	advise(_(message_advice_ref_already_exists));
}

நிலையான வெறுமை advise_ref_fetch_first(வெறுமை)
{
	எனில் (!advice_enabled(ADVICE_தள்ளு_FETCH_FIRST) || !advice_enabled(ADVICE_தள்ளு_UPDATE_REJECTED))
		திரும்பு;
	advise(_(message_advice_ref_fetch_first));
}

நிலையான வெறுமை advise_ref_needs_force(வெறுமை)
{
	எனில் (!advice_enabled(ADVICE_தள்ளு_NEEDS_FORCE) || !advice_enabled(ADVICE_தள்ளு_UPDATE_REJECTED))
		திரும்பு;
	advise(_(message_advice_ref_needs_force));
}

நிலையான வெறுமை advise_ref_needs_update(வெறுமை)
{
	எனில் (!advice_enabled(ADVICE_தள்ளு_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_தள்ளு_UPDATE_REJECTED))
		திரும்பு;
	advise(_(message_advice_ref_needs_update));
}

நிலையான முழு தள்ளு_with_options(கட்டமைப்பு transport *transport, கட்டமைப்பு refspec *rs,
			     முழு flags)
{
	முழு err;
	unsigned முழு reject_reasons;
	எழுத்து *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	எனில் (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : வெற்று);

	எனில் (!is_empty_cas(&cas)) {
		எனில் (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	எனில் (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_தள்ளு(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	எனில் (err != 0) {
		fprintf(stderr, "%s", தள்ளு_get_color(தள்ளு_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", தள்ளு_get_color(தள்ளு_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	விடுவி(anon_url);
	எனில் (!err)
		திரும்பு 0;

	எனில் (reject_reasons & REJECT_NON_FF_HEAD) {
		advise_pull_before_தள்ளு();
	} இல்லையெனில் எனில் (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_தள்ளு();
	} இல்லையெனில் எனில் (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} இல்லையெனில் எனில் (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} இல்லையெனில் எனில் (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} இல்லையெனில் எனில் (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	திரும்பு 1;
}

நிலையான முழு do_தள்ளு(முழு flags,
		   மாறிலி கட்டமைப்பு string_list *தள்ளு_options,
		   கட்டமைப்பு remote *remote)
{
	முழு errs;
	கட்டமைப்பு strvec *url;
	கட்டமைப்பு refspec *தள்ளு_refspec = &rs;

	எனில் (தள்ளு_options->nr)
		flags |= TRANSPORT_தள்ளு_OPTIONS;

	எனில் (!தள்ளு_refspec->nr && !(flags & TRANSPORT_தள்ளு_ALL)) {
		எனில் (remote->தள்ளு.nr) {
			தள்ளு_refspec = &remote->தள்ளு;
		} இல்லையெனில் எனில் (!(flags & TRANSPORT_தள்ளு_MIRROR))
			setup_default_தள்ளு_refspecs(&flags, remote);
	}
	errs = 0;
	url = தள்ளு_url_of_remote(remote);
	க்கு (size_t i = 0; i < url->nr; i++) {
		கட்டமைப்பு transport *transport =
			transport_get(remote, url->v[i]);
		எனில் (flags & TRANSPORT_தள்ளு_OPTIONS)
			transport->தள்ளு_options = தள்ளு_options;
		எனில் (தள்ளு_with_options(transport, தள்ளு_refspec, flags))
			errs++;
	}
	திரும்பு !!errs;
}

நிலையான முழு option_parse_recurse_submodules(மாறிலி கட்டமைப்பு option *opt,
				   மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *recurse_submodules = opt->மதிப்பு;

	எனில் (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	இல்லையெனில் {
		எனில் (!strcmp(arg, "only-is-on-demand")) {
			எனில் (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} இல்லையெனில் {
			*recurse_submodules = parse_தள்ளு_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	திரும்பு 0;
}

நிலையான வெறுமை set_தள்ளு_cert_flags(முழு *flags, முழு v)
{
	தேர்வு (v) {
	நிலை SEND_PACK_தள்ளு_CERT_NEVER:
		*flags &= ~(TRANSPORT_தள்ளு_CERT_ALWAYS | TRANSPORT_தள்ளு_CERT_IF_ASKED);
		நிறுத்து;
	நிலை SEND_PACK_தள்ளு_CERT_ALWAYS:
		*flags |= TRANSPORT_தள்ளு_CERT_ALWAYS;
		*flags &= ~TRANSPORT_தள்ளு_CERT_IF_ASKED;
		நிறுத்து;
	நிலை SEND_PACK_தள்ளு_CERT_IF_ASKED:
		*flags |= TRANSPORT_தள்ளு_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_தள்ளு_CERT_ALWAYS;
		நிறுத்து;
	}
}


நிலையான முழு git_தள்ளு_config(மாறிலி எழுத்து *k, மாறிலி எழுத்து *v,
			   மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	மாறிலி எழுத்து *slot_name;
	முழு *flags = cb;

	எனில் (!strcmp(k, "push.followtags")) {
		எனில் (git_config_bool(k, v))
			*flags |= TRANSPORT_தள்ளு_FOLLOW_TAGS;
		இல்லையெனில்
			*flags &= ~TRANSPORT_தள்ளு_FOLLOW_TAGS;
		திரும்பு 0;
	} இல்லையெனில் எனில் (!strcmp(k, "push.autosetupremote")) {
		எனில் (git_config_bool(k, v))
			*flags |= TRANSPORT_தள்ளு_AUTO_UPSTREAM;
		திரும்பு 0;
	} இல்லையெனில் எனில் (!strcmp(k, "push.gpgsign")) {
		தேர்வு (git_parse_maybe_bool(v)) {
		நிலை 0:
			set_தள்ளு_cert_flags(flags, SEND_PACK_தள்ளு_CERT_NEVER);
			நிறுத்து;
		நிலை 1:
			set_தள்ளு_cert_flags(flags, SEND_PACK_தள்ளு_CERT_ALWAYS);
			நிறுத்து;
		இயல்பு:
			எனில் (!strcasecmp(v, "if-asked"))
				set_தள்ளு_cert_flags(flags, SEND_PACK_தள்ளு_CERT_IF_ASKED);
			இல்லையெனில்
				திரும்பு error(_("invalid value for '%s'"), k);
		}
	} இல்லையெனில் எனில் (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_தள்ளு_recurse_submodules_arg(k, v);
	} இல்லையெனில் எனில் (!strcmp(k, "submodule.recurse")) {
		முழு val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} இல்லையெனில் எனில் (!strcmp(k, "push.pushoption")) {
		திரும்பு parse_transport_option(k, v, &தள்ளு_options_config);
	} இல்லையெனில் எனில் (!strcmp(k, "color.push")) {
		தள்ளு_use_color = git_config_colorbool(k, v);
		திரும்பு 0;
	} இல்லையெனில் எனில் (skip_prefix(k, "color.push.", &slot_name)) {
		முழு slot = parse_தள்ளு_color_slot(slot_name);
		எனில் (slot < 0)
			திரும்பு 0;
		எனில் (!v)
			திரும்பு config_error_nonbool(k);
		திரும்பு color_parse(v, தள்ளு_colors[slot]);
	} இல்லையெனில் எனில் (!strcmp(k, "push.useforceifincludes")) {
		எனில் (git_config_bool(k, v))
			*flags |= TRANSPORT_தள்ளு_FORCE_IF_INCLUDES;
		இல்லையெனில்
			*flags &= ~TRANSPORT_தள்ளு_FORCE_IF_INCLUDES;
		திரும்பு 0;
	}

	திரும்பு git_default_config(k, v, ctx, வெற்று);
}

நிலையான முழு தள்ளு_multiple(கட்டமைப்பு string_list *list,
			 மாறிலி கட்டமைப்பு string_list *தள்ளு_options,
			 முழு flags,
			 முழு tags,
			 மாறிலி எழுத்து **refspecs,
			 முழு refspec_nr)
{
	முழு result = 0;
	size_t i;
	கட்டமைப்பு strvec argv = STRVEC_INIT;

	strvec_தள்ளு(&argv, "push");

	எனில் (flags & TRANSPORT_தள்ளு_FORCE)
		strvec_தள்ளு(&argv, "--force");
	எனில் (flags & TRANSPORT_தள்ளு_DRY_RUN)
		strvec_தள்ளு(&argv, "--dry-run");
	எனில் (flags & TRANSPORT_தள்ளு_PORCELAIN)
		strvec_தள்ளு(&argv, "--porcelain");
	எனில் (flags & TRANSPORT_தள்ளு_PRUNE)
		strvec_தள்ளு(&argv, "--prune");
	எனில் (flags & TRANSPORT_தள்ளு_NO_HOOK)
		strvec_தள்ளு(&argv, "--no-verify");
	எனில் (flags & TRANSPORT_தள்ளு_FOLLOW_TAGS)
		strvec_தள்ளு(&argv, "--follow-tags");
	எனில் (flags & TRANSPORT_தள்ளு_SET_UPSTREAM)
		strvec_தள்ளு(&argv, "--set-upstream");
	எனில் (flags & TRANSPORT_தள்ளு_FORCE_IF_INCLUDES)
		strvec_தள்ளு(&argv, "--force-if-includes");
	எனில் (flags & TRANSPORT_தள்ளு_ALL)
		strvec_தள்ளு(&argv, "--all");
	எனில் (flags & TRANSPORT_தள்ளு_MIRROR)
		strvec_தள்ளு(&argv, "--mirror");

	எனில் (flags & TRANSPORT_தள்ளு_CERT_ALWAYS)
		strvec_தள்ளு(&argv, "--signed=yes");
	இல்லையெனில் எனில் (flags & TRANSPORT_தள்ளு_CERT_IF_ASKED)
		strvec_தள்ளு(&argv, "--signed=if-asked");
	எனில் (!thin)
		strvec_தள்ளு(&argv, "--no-thin");

	எனில் (deleterefs)
		strvec_தள்ளு(&argv, "--delete");

	எனில் (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	எனில் (verbosity >= 2)
		strvec_தள்ளு(&argv, "-v");
	எனில் (verbosity >= 1)
		strvec_தள்ளு(&argv, "-v");
	இல்லையெனில் எனில் (verbosity < 0)
		strvec_தள்ளு(&argv, "-q");
	எனில் (progress > 0)
		strvec_தள்ளு(&argv, "--progress");
	இல்லையெனில் எனில் (progress == 0)
		strvec_தள்ளு(&argv, "--no-progress");

	எனில் (family == TRANSPORT_FAMILY_IPV4)
		strvec_தள்ளு(&argv, "--ipv4");
	இல்லையெனில் எனில் (family == TRANSPORT_FAMILY_IPV6)
		strvec_தள்ளு(&argv, "--ipv6");

	எனில் (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_தள்ளு(&argv, "--recurse-submodules=check");
	இல்லையெனில் எனில் (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_தள்ளு(&argv, "--recurse-submodules=on-demand");
	இல்லையெனில் எனில் (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_தள்ளு(&argv, "--recurse-submodules=only");
	இல்லையெனில் எனில் (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_தள்ளு(&argv, "--recurse-submodules=no");


	எனில் (tags)
		strvec_தள்ளு(&argv, "--tags");

	க்கு (i = 0; i < தள்ளு_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     தள்ளு_options->பொருட்கள்[i].string);

	க்கு (i = 0; i < cas.nr; i++) {
		எனில் (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} இல்லையெனில் எனில் (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} இல்லையெனில் {
			strvec_தள்ளு(&argv, "--force-with-lease");
		}
	}

	க்கு (i = 0; i < list->nr; i++) {
		மாறிலி எழுத்து *name = list->பொருட்கள்[i].string;
		கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;
		முழு j;

		strvec_pushv(&cmd.args, argv.v);
		strvec_தள்ளு(&cmd.args, name);

		க்கு (j = 0; j < refspec_nr; j++)
			strvec_தள்ளு(&cmd.args, refspecs[j]);

		எனில் (verbosity >= 0)
			printf(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		எனில் (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			result = 1;
		}
	}

	strvec_clear(&argv);
	திரும்பு result;
}

நிலையான வெறுமை die_if_repo_looks_like_ref(மாறிலி எழுத்து *repo)
{
	மாறிலி எழுத்து *slash = strchr(repo, '/');
	கட்டமைப்பு strbuf name = STRBUF_INIT;
	முழு code;

	எனில் (!slash || !slash[1] || file_exists(repo))
		திரும்பு;

	strbuf_add(&name, repo, slash - repo);
	எனில் (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		திரும்பு;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_தள்ளு_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	exit(code);
}

முழு cmd_தள்ளு(முழு argc,
	     மாறிலி எழுத்து **argv,
	     மாறிலி எழுத்து *prefix,
	     கட்டமைப்பு repository *repository UNUSED)
{
	முழு flags = 0;
	முழு tags = 0;
	முழு தள்ளு_cert = -1;
	முழு rc = 0;
	முழு base_flags;
	மாறிலி எழுத்து *repo = வெற்று;	/* default repository */
	கட்டமைப்பு string_list தள்ளு_options_cmdline = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list remote_group = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list *தள்ளு_options;
	மாறிலி கட்டமைப்பு string_list_item *item;

	கட்டமைப்பு option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_BIT( 0 , "all", &flags, N_("push all branches"), TRANSPORT_தள்ளு_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_BIT( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_தள்ளு_MIRROR|TRANSPORT_தள்ளு_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &tags, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_BIT('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_தள்ளு_DRY_RUN),
		OPT_BIT( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_தள்ளு_PORCELAIN),
		OPT_BIT('f', "force", &flags, N_("force updates"), TRANSPORT_தள்ளு_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_தள்ளு_cas_option),
		OPT_BIT(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_தள்ளு_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_BIT('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_தள்ளு_SET_UPSTREAM),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_BIT(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_தள்ளு_PRUNE),
		OPT_BIT(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_தள்ளு_NO_HOOK),
		OPT_BIT(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_தள்ளு_FOLLOW_TAGS),
		OPT_CALLBACK_F(0, "signed", &தள்ளு_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_தள்ளு_signed),
		OPT_BIT(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_தள்ளு_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &தள்ளு_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_END()
	};

	packet_trace_identity("push");
	repo_config(the_repository, git_தள்ளு_config, &flags);
	argc = parse_options(argc, argv, prefix, options, தள்ளு_usage, 0);
	தள்ளு_options = (தள்ளு_options_cmdline.nr
		? &தள்ளு_options_cmdline
		: &தள்ளு_options_config);
	set_தள்ளு_cert_flags(&flags, தள்ளு_cert);

	refspec_init_தள்ளு(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  tags, "--tags",
				  flags & TRANSPORT_தள்ளு_ALL, "--all/--branches",
				  flags & TRANSPORT_தள்ளு_MIRROR, "--mirror");
	எனில் (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	எனில் (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	இல்லையெனில் எனில் (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	இல்லையெனில் எனில் (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	எனில் (argc > 0)
		repo = argv[0];

	எனில் (repo) {
		எனில் (!add_remote_or_group(repo, &remote_group)) {
			கட்டமைப்பு remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			எனில் (advice_enabled(ADVICE_தள்ளு_REPO_LOOKS_LIKE_REF))
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
			எனில் (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_group, r->name);
		}
	} இல்லையெனில் {
		கட்டமைப்பு remote *r = pushremote_get(வெற்று);
		எனில் (!r)
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

	எனில் (!is_empty_cas(&cas) && (flags & TRANSPORT_தள்ளு_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, தள்ளு_options)
		எனில் (strchr(item->string, '\n'))
			die(_("push options must not have new line characters"));

	எனில் (remote_group.nr == 1) {
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
			முழு inner_flags = base_flags;
			கட்டமைப்பு remote *r = pushremote_get(remote_group.பொருட்கள்[0].string);
			எனில் (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.பொருட்கள்[0].string);

			எனில் (r->mirror)
				inner_flags |= (TRANSPORT_தள்ளு_MIRROR|TRANSPORT_தள்ளு_FORCE);

			எனில் (inner_flags & TRANSPORT_தள்ளு_ALL) {
				எனில் (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			எனில் (inner_flags & TRANSPORT_தள்ளு_MIRROR) {
				எனில் (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (கட்டமைப்பு refspec) REFSPEC_INIT_தள்ளு(the_hash_algo);

			எனில் (tags)
				refspec_append(&rs, "refs/tags/*");
			எனில் (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_தள்ளு(inner_flags, தள்ளு_options, r);
		}
	} இல்லையெனில் {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		எனில் (flags & TRANSPORT_தள்ளு_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = தள்ளு_multiple(&remote_group, தள்ளு_options, flags,
				   tags,
				   argc > 1 ? argv + 1 : வெற்று,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&தள்ளு_options_cmdline, 0);
	string_list_clear(&தள்ளு_options_config, 0);
	string_list_clear(&remote_group, 0);
	clear_cas_option(&cas);

	எனில் (rc == -1)
		usage_with_options(தள்ளு_usage, options);
	இல்லையெனில்
		திரும்பு rc;
}
