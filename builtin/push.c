/*
 * "git push"
 */

#define USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "branch.h"
#ضمن "config.h"
#ضمن "dir.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "refspec.h"
#ضمن "run-command.h"
#ضمن "remote.h"
#ضمن "transport.h"
#ضمن "parse-options.h"
#ضمن "pkt-line.h"
#ضمن "submodule.h"
#ضمن "submodule-config.h"
#ضمن "send-pack.h"
#ضمن "trace2.h"
#ضمن "color.h"

ساكن ثابت حرف * ثابت تور_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	عدم,
};

ساكن تعداد git_colorbool تور_use_color = GIT_COLOR_UNKNOWN;
ساكن حرف تور_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

تعداد color_تور {
	تور_COLOR_RESET = 0,
	تور_COLOR_ERROR = 1
};

ساكن صحيح parse_تور_color_slot(ثابت حرف *slot)
{
	إذا (!strcasecmp(slot, "reset"))
		أرجع تور_COLOR_RESET;
	إذا (!strcasecmp(slot, "error"))
		أرجع تور_COLOR_ERROR;
	أرجع -1;
}

ساكن ثابت حرف *تور_get_color(تعداد color_تور ix)
{
	إذا (want_color_stderr(تور_use_color))
		أرجع تور_colors[ix];
	أرجع "";
}

ساكن صحيح thin = 1;
ساكن صحيح deleterefs;
ساكن ثابت حرف *receivepack;
ساكن صحيح verbosity;
ساكن صحيح progress = -1;
ساكن صحيح recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ساكن تعداد transport_family family;

ساكن بنية تور_cas_option cas;

ساكن بنية refspec rs;

ساكن بنية string_list تور_options_config = STRING_LIST_INIT_DUP;

ساكن فراغ refspec_append_mapped(بنية refspec *refspec, ثابت حرف *ref,
				  بنية remote *remote, بنية ref *matched)
{
	ثابت حرف *branch_name;
	بنية repo_config_درجوجي *cfg = repo_config_درجوجي(the_repository);

	إذا (remote->تور.nr) {
		بنية refspec_item query = {
			.src = matched->name,
		};

		إذا (!refspec_find_match(&remote->تور, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			حرر(query.dst);
			أرجع;
		}
	}

	إذا (cfg->تور_default == تور_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		بنية branch *branch = branch_get(branch_name);
		إذا (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			أرجع;
		}
	}

	refspec_append(refspec, ref);
}

ساكن فراغ set_refspecs(ثابت حرف **refs, صحيح nr, بنية remote *remote)
{
	بنية ref *local_refs = عدم;
	صحيح i;

	لكل (i = 0; i < nr; i++) {
		ثابت حرف *ref = refs[i];
		إذا (!strcmp("tag", ref)) {
			إذا (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			إذا (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			وإلا
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} وإلا إذا (deleterefs) {
			إذا (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} وإلا إذا (!strchr(ref, ':')) {
			بنية ref *matched = عدم;

			/* lazily grab local_refs */
			إذا (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			إذا (كدي_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			وإلا
				refspec_append_mapped(&rs, ref, remote, matched);
		} وإلا
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

ساكن NORETURN فراغ die_تور_simple(بنية branch *branch,
				     بنية remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	ثابت حرف *advice_pushdefault_maybe = "";
	ثابت حرف *advice_automergesimple_maybe = "";
	ثابت حرف *short_upstream = branch->merge[0]->src;
	بنية repo_config_درجوجي *cfg = repo_config_درجوجي(the_repository);

	skip_prefix(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	إذا (cfg->تور_default == تور_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	إذا (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

ساكن ثابت حرف message_detached_head_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

ساكن ثابت حرف *get_upstream_ref(صحيح flags, بنية branch *branch, ثابت حرف *remote_name)
{
	إذا (branch->merge_nr == 0 && (flags & TRANSPORT_تور_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		أرجع branch->refname;
	}

	إذا (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		ثابت حرف *advice_autosetup_maybe = "";
		إذا (!(flags & TRANSPORT_تور_AUTO_UPSTREAM)) {
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
	إذا (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	أرجع branch->merge[0]->src;
}

ساكن فراغ setup_default_تور_refspecs(صحيح *flags, بنية remote *remote)
{
	بنية branch *branch;
	ثابت حرف *dst;
	صحيح same_remote;
	بنية repo_config_درجوجي *cfg = repo_config_درجوجي(the_repository);

	بدل (cfg->تور_default) {
	حالة تور_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		أرجع;

	حالة تور_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		أرجع;
	افتراضي:
		اكسر;
	}

	branch = branch_get(عدم);
	إذا (!branch)
		die(_(message_detached_head_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, عدم));

	بدل (cfg->تور_default) {
	افتراضي:
	حالة تور_DEFAULT_UNSPECIFIED:
	حالة تور_DEFAULT_SIMPLE:
		إذا (!same_remote)
			اكسر;
		إذا (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_تور_simple(branch, remote);
		اكسر;

	حالة تور_DEFAULT_UPSTREAM:
		إذا (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		اكسر;

	حالة تور_DEFAULT_CURRENT:
		اكسر;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	إذا ((*flags & TRANSPORT_تور_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_تور_SET_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

ساكن ثابت حرف message_advice_pull_before_تور[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ساكن ثابت حرف message_advice_checkout_pull_تور[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ساكن ثابت حرف message_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ساكن ثابت حرف message_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

ساكن ثابت حرف message_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

ساكن ثابت حرف message_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ساكن فراغ advise_pull_before_تور(فراغ)
{
	إذا (!advice_enabled(ADVICE_تور_NON_FF_CURRENT) || !advice_enabled(ADVICE_تور_UPDATE_REJECTED))
		أرجع;
	advise(_(message_advice_pull_before_تور));
}

ساكن فراغ advise_checkout_pull_تور(فراغ)
{
	إذا (!advice_enabled(ADVICE_تور_NON_FF_MATCHING) || !advice_enabled(ADVICE_تور_UPDATE_REJECTED))
		أرجع;
	advise(_(message_advice_checkout_pull_تور));
}

ساكن فراغ advise_ref_already_exists(فراغ)
{
	إذا (!advice_enabled(ADVICE_تور_ALREADY_EXISTS) || !advice_enabled(ADVICE_تور_UPDATE_REJECTED))
		أرجع;
	advise(_(message_advice_ref_already_exists));
}

ساكن فراغ advise_ref_fetch_first(فراغ)
{
	إذا (!advice_enabled(ADVICE_تور_FETCH_FIRST) || !advice_enabled(ADVICE_تور_UPDATE_REJECTED))
		أرجع;
	advise(_(message_advice_ref_fetch_first));
}

ساكن فراغ advise_ref_needs_force(فراغ)
{
	إذا (!advice_enabled(ADVICE_تور_NEEDS_FORCE) || !advice_enabled(ADVICE_تور_UPDATE_REJECTED))
		أرجع;
	advise(_(message_advice_ref_needs_force));
}

ساكن فراغ advise_ref_needs_update(فراغ)
{
	إذا (!advice_enabled(ADVICE_تور_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_تور_UPDATE_REJECTED))
		أرجع;
	advise(_(message_advice_ref_needs_update));
}

ساكن صحيح تور_with_options(بنية transport *transport, بنية refspec *rs,
			     صحيح flags)
{
	صحيح err;
	unsigned صحيح reject_reasons;
	حرف *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	إذا (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : عدم);

	إذا (!is_empty_cas(&cas)) {
		إذا (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	إذا (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_تور(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	إذا (err != 0) {
		fprintf(stderr, "%s", تور_get_color(تور_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", تور_get_color(تور_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	حرر(anon_url);
	إذا (!err)
		أرجع 0;

	إذا (reject_reasons & REJECT_NON_FF_HEAD) {
		advise_pull_before_تور();
	} وإلا إذا (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_تور();
	} وإلا إذا (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} وإلا إذا (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} وإلا إذا (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} وإلا إذا (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	أرجع 1;
}

ساكن صحيح do_تور(صحيح flags,
		   ثابت بنية string_list *تور_options,
		   بنية remote *remote)
{
	صحيح errs;
	بنية strvec *url;
	بنية refspec *تور_refspec = &rs;

	إذا (تور_options->nr)
		flags |= TRANSPORT_تور_OPTIONS;

	إذا (!تور_refspec->nr && !(flags & TRANSPORT_تور_ALL)) {
		إذا (remote->تور.nr) {
			تور_refspec = &remote->تور;
		} وإلا إذا (!(flags & TRANSPORT_تور_MIRROR))
			setup_default_تور_refspecs(&flags, remote);
	}
	errs = 0;
	url = تور_url_of_remote(remote);
	لكل (size_t i = 0; i < url->nr; i++) {
		بنية transport *transport =
			transport_get(remote, url->v[i]);
		إذا (flags & TRANSPORT_تور_OPTIONS)
			transport->تور_options = تور_options;
		إذا (تور_with_options(transport, تور_refspec, flags))
			errs++;
	}
	أرجع !!errs;
}

ساكن صحيح option_parse_recurse_submodules(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	صحيح *recurse_submodules = opt->درج;

	إذا (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	وإلا {
		إذا (!strcmp(arg, "only-is-on-demand")) {
			إذا (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} وإلا {
			*recurse_submodules = parse_تور_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	أرجع 0;
}

ساكن فراغ set_تور_cert_flags(صحيح *flags, صحيح v)
{
	بدل (v) {
	حالة SEND_PACK_تور_CERT_NEVER:
		*flags &= ~(TRANSPORT_تور_CERT_ALWAYS | TRANSPORT_تور_CERT_IF_ASKED);
		اكسر;
	حالة SEND_PACK_تور_CERT_ALWAYS:
		*flags |= TRANSPORT_تور_CERT_ALWAYS;
		*flags &= ~TRANSPORT_تور_CERT_IF_ASKED;
		اكسر;
	حالة SEND_PACK_تور_CERT_IF_ASKED:
		*flags |= TRANSPORT_تور_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_تور_CERT_ALWAYS;
		اكسر;
	}
}


ساكن صحيح git_تور_config(ثابت حرف *k, ثابت حرف *v,
			   ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت حرف *slot_name;
	صحيح *flags = cb;

	إذا (!strcmp(k, "push.followtags")) {
		إذا (git_config_bool(k, v))
			*flags |= TRANSPORT_تور_FOLLOW_TAGS;
		وإلا
			*flags &= ~TRANSPORT_تور_FOLLOW_TAGS;
		أرجع 0;
	} وإلا إذا (!strcmp(k, "push.autosetupremote")) {
		إذا (git_config_bool(k, v))
			*flags |= TRANSPORT_تور_AUTO_UPSTREAM;
		أرجع 0;
	} وإلا إذا (!strcmp(k, "push.gpgsign")) {
		بدل (git_parse_maybe_bool(v)) {
		حالة 0:
			set_تور_cert_flags(flags, SEND_PACK_تور_CERT_NEVER);
			اكسر;
		حالة 1:
			set_تور_cert_flags(flags, SEND_PACK_تور_CERT_ALWAYS);
			اكسر;
		افتراضي:
			إذا (!strcasecmp(v, "if-asked"))
				set_تور_cert_flags(flags, SEND_PACK_تور_CERT_IF_ASKED);
			وإلا
				أرجع error(_("invalid value for '%s'"), k);
		}
	} وإلا إذا (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_تور_recurse_submodules_arg(k, v);
	} وإلا إذا (!strcmp(k, "submodule.recurse")) {
		صحيح val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} وإلا إذا (!strcmp(k, "push.pushoption")) {
		أرجع parse_transport_option(k, v, &تور_options_config);
	} وإلا إذا (!strcmp(k, "color.push")) {
		تور_use_color = git_config_colorbool(k, v);
		أرجع 0;
	} وإلا إذا (skip_prefix(k, "color.push.", &slot_name)) {
		صحيح slot = parse_تور_color_slot(slot_name);
		إذا (slot < 0)
			أرجع 0;
		إذا (!v)
			أرجع config_error_nonbool(k);
		أرجع color_parse(v, تور_colors[slot]);
	} وإلا إذا (!strcmp(k, "push.useforceifincludes")) {
		إذا (git_config_bool(k, v))
			*flags |= TRANSPORT_تور_FORCE_IF_INCLUDES;
		وإلا
			*flags &= ~TRANSPORT_تور_FORCE_IF_INCLUDES;
		أرجع 0;
	}

	أرجع git_default_config(k, v, ctx, عدم);
}

ساكن صحيح تور_multiple(بنية string_list *list,
			 ثابت بنية string_list *تور_options,
			 صحيح flags,
			 صحيح tags,
			 ثابت حرف **refspecs,
			 صحيح refspec_nr)
{
	صحيح result = 0;
	size_t i;
	بنية strvec argv = STRVEC_INIT;

	strvec_تور(&argv, "push");

	إذا (flags & TRANSPORT_تور_FORCE)
		strvec_تور(&argv, "--force");
	إذا (flags & TRANSPORT_تور_DRY_RUN)
		strvec_تور(&argv, "--dry-run");
	إذا (flags & TRANSPORT_تور_PORCELAIN)
		strvec_تور(&argv, "--porcelain");
	إذا (flags & TRANSPORT_تور_PRUNE)
		strvec_تور(&argv, "--prune");
	إذا (flags & TRANSPORT_تور_NO_HOOK)
		strvec_تور(&argv, "--no-verify");
	إذا (flags & TRANSPORT_تور_FOLLOW_TAGS)
		strvec_تور(&argv, "--follow-tags");
	إذا (flags & TRANSPORT_تور_SET_UPSTREAM)
		strvec_تور(&argv, "--set-upstream");
	إذا (flags & TRANSPORT_تور_FORCE_IF_INCLUDES)
		strvec_تور(&argv, "--force-if-includes");
	إذا (flags & TRANSPORT_تور_ALL)
		strvec_تور(&argv, "--all");
	إذا (flags & TRANSPORT_تور_MIRROR)
		strvec_تور(&argv, "--mirror");

	إذا (flags & TRANSPORT_تور_CERT_ALWAYS)
		strvec_تور(&argv, "--signed=yes");
	وإلا إذا (flags & TRANSPORT_تور_CERT_IF_ASKED)
		strvec_تور(&argv, "--signed=if-asked");
	إذا (!thin)
		strvec_تور(&argv, "--no-thin");

	إذا (deleterefs)
		strvec_تور(&argv, "--delete");

	إذا (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	إذا (verbosity >= 2)
		strvec_تور(&argv, "-v");
	إذا (verbosity >= 1)
		strvec_تور(&argv, "-v");
	وإلا إذا (verbosity < 0)
		strvec_تور(&argv, "-q");
	إذا (progress > 0)
		strvec_تور(&argv, "--progress");
	وإلا إذا (progress == 0)
		strvec_تور(&argv, "--no-progress");

	إذا (family == TRANSPORT_FAMILY_IPV4)
		strvec_تور(&argv, "--ipv4");
	وإلا إذا (family == TRANSPORT_FAMILY_IPV6)
		strvec_تور(&argv, "--ipv6");

	إذا (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_تور(&argv, "--recurse-submodules=check");
	وإلا إذا (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_تور(&argv, "--recurse-submodules=on-demand");
	وإلا إذا (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_تور(&argv, "--recurse-submodules=only");
	وإلا إذا (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_تور(&argv, "--recurse-submodules=no");


	إذا (tags)
		strvec_تور(&argv, "--tags");

	لكل (i = 0; i < تور_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     تور_options->ابوبو[i].string);

	لكل (i = 0; i < cas.nr; i++) {
		إذا (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} وإلا إذا (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} وإلا {
			strvec_تور(&argv, "--force-with-lease");
		}
	}

	لكل (i = 0; i < list->nr; i++) {
		ثابت حرف *name = list->ابوبو[i].string;
		بنية child_process cmd = CHILD_PROCESS_INIT;
		صحيح j;

		strvec_pushv(&cmd.حجوجي, argv.v);
		strvec_تور(&cmd.حجوجي, name);

		لكل (j = 0; j < refspec_nr; j++)
			strvec_تور(&cmd.حجوجي, refspecs[j]);

		إذا (verbosity >= 0)
			printf(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		إذا (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			result = 1;
		}
	}

	strvec_clear(&argv);
	أرجع result;
}

ساكن فراغ die_if_repo_looks_like_ref(ثابت حرف *repo)
{
	ثابت حرف *slash = strchr(repo, '/');
	بنية strbuf name = STRBUF_INIT;
	صحيح code;

	إذا (!slash || !slash[1] || file_exists(repo))
		أرجع;

	strbuf_add(&name, repo, slash - repo);
	إذا (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		أرجع;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_تور_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	exit(code);
}

صحيح cmd_تور(صحيح argc,
	     ثابت حرف **argv,
	     ثابت حرف *prefix,
	     بنية repository *repository UNUSED)
{
	صحيح flags = 0;
	صحيح tags = 0;
	صحيح تور_cert = -1;
	صحيح rc = 0;
	صحيح base_flags;
	ثابت حرف *repo = عدم;	/* default repository */
	بنية string_list تور_options_cmdline = STRING_LIST_INIT_DUP;
	بنية string_list remote_group = STRING_LIST_INIT_DUP;
	بنية string_list *تور_options;
	ثابت بنية string_list_item *item;

	بنية option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_BIT( 0 , "all", &flags, N_("push all branches"), TRANSPORT_تور_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_BIT( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_تور_MIRROR|TRANSPORT_تور_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &tags, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_BIT('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_تور_DRY_RUN),
		OPT_BIT( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_تور_PORCELAIN),
		OPT_BIT('f', "force", &flags, N_("force updates"), TRANSPORT_تور_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_تور_cas_option),
		OPT_BIT(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_تور_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_BIT('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_تور_SET_UPSTREAM),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_BIT(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_تور_PRUNE),
		OPT_BIT(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_تور_NO_HOOK),
		OPT_BIT(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_تور_FOLLOW_TAGS),
		OPT_CALLBACK_F(0, "signed", &تور_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_تور_signed),
		OPT_BIT(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_تور_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &تور_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_END()
	};

	packet_trace_identity("push");
	repo_config(the_repository, git_تور_config, &flags);
	argc = parse_options(argc, argv, prefix, options, تور_usage, 0);
	تور_options = (تور_options_cmdline.nr
		? &تور_options_cmdline
		: &تور_options_config);
	set_تور_cert_flags(&flags, تور_cert);

	refspec_init_تور(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  tags, "--tags",
				  flags & TRANSPORT_تور_ALL, "--all/--branches",
				  flags & TRANSPORT_تور_MIRROR, "--mirror");
	إذا (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	إذا (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	وإلا إذا (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	وإلا إذا (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	إذا (argc > 0)
		repo = argv[0];

	إذا (repo) {
		إذا (!add_remote_or_group(repo, &remote_group)) {
			بنية remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			إذا (advice_enabled(ADVICE_تور_REPO_LOOKS_LIKE_REF))
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
			إذا (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_group, r->name);
		}
	} وإلا {
		بنية remote *r = pushremote_get(عدم);
		إذا (!r)
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

	إذا (!is_empty_cas(&cas) && (flags & TRANSPORT_تور_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, تور_options)
		إذا (strchr(item->string, '\n'))
			die(_("push options must not have new line characters"));

	إذا (remote_group.nr == 1) {
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
			صحيح inner_flags = base_flags;
			بنية remote *r = pushremote_get(remote_group.ابوبو[0].string);
			إذا (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.ابوبو[0].string);

			إذا (r->mirror)
				inner_flags |= (TRANSPORT_تور_MIRROR|TRANSPORT_تور_FORCE);

			إذا (inner_flags & TRANSPORT_تور_ALL) {
				إذا (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			إذا (inner_flags & TRANSPORT_تور_MIRROR) {
				إذا (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (بنية refspec) REFSPEC_INIT_تور(the_hash_algo);

			إذا (tags)
				refspec_append(&rs, "refs/tags/*");
			إذا (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_تور(inner_flags, تور_options, r);
		}
	} وإلا {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		إذا (flags & TRANSPORT_تور_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = تور_multiple(&remote_group, تور_options, flags,
				   tags,
				   argc > 1 ? argv + 1 : عدم,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&تور_options_cmdline, 0);
	string_list_clear(&تور_options_config, 0);
	string_list_clear(&remote_group, 0);
	clear_cas_option(&cas);

	إذا (rc == -1)
		usage_with_options(تور_usage, options);
	وإلا
		أرجع rc;
}
