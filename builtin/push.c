/*
 * "git push"
 */

#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "advice.h"
#شامل "branch.h"
#شامل "config.h"
#شامل "dir.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "refspec.h"
#شامل "run-command.h"
#شامل "remote.h"
#شامل "transport.h"
#شامل "parse-options.h"
#شامل "pkt-line.h"
#شامل "submodule.h"
#شامل "submodule-config.h"
#شامل "send-pack.h"
#شامل "trace2.h"
#شامل "color.h"

ایستا ثابت نویسه * ثابت افزودن_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	پوچ,
};

ایستا شمارش git_colorbool افزودن_use_color = GIT_COLOR_UNKNOWN;
ایستا نویسه افزودن_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

شمارش color_افزودن {
	افزودن_COLOR_RESET = 0,
	افزودن_COLOR_ERROR = 1
};

ایستا صحیح parse_افزودن_color_slot(ثابت نویسه *slot)
{
	اگر (!strcasecmp(slot, "reset"))
		بازگشت افزودن_COLOR_RESET;
	اگر (!strcasecmp(slot, "error"))
		بازگشت افزودن_COLOR_ERROR;
	بازگشت -1;
}

ایستا ثابت نویسه *افزودن_get_color(شمارش color_افزودن ix)
{
	اگر (want_color_stderr(افزودن_use_color))
		بازگشت افزودن_colors[ix];
	بازگشت "";
}

ایستا صحیح thin = 1;
ایستا صحیح deleterefs;
ایستا ثابت نویسه *receivepack;
ایستا صحیح verbosity;
ایستا صحیح progress = -1;
ایستا صحیح recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ایستا شمارش transport_family family;

ایستا ساختار افزودن_cas_option cas;

ایستا ساختار refspec rs;

ایستا ساختار string_list افزودن_options_config = STRING_LIST_INIT_DUP;

ایستا تهی refspec_append_mapped(ساختار refspec *refspec, ثابت نویسه *ref,
				  ساختار remote *remote, ساختار ref *matched)
{
	ثابت نویسه *branch_name;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	اگر (remote->افزودن.nr) {
		ساختار refspec_item query = {
			.src = matched->name,
		};

		اگر (!refspec_find_match(&remote->افزودن, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			آزاد(query.dst);
			بازگشت;
		}
	}

	اگر (cfg->افزودن_default == افزودن_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		ساختار branch *branch = branch_get(branch_name);
		اگر (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			بازگشت;
		}
	}

	refspec_append(refspec, ref);
}

ایستا تهی set_refspecs(ثابت نویسه **refs, صحیح nr, ساختار remote *remote)
{
	ساختار ref *local_refs = پوچ;
	صحیح i;

	برای (i = 0; i < nr; i++) {
		ثابت نویسه *ref = refs[i];
		اگر (!strcmp("tag", ref)) {
			اگر (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			اگر (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			وگرنه
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} وگرنه اگر (deleterefs) {
			اگر (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} وگرنه اگر (!strchr(ref, ':')) {
			ساختار ref *matched = پوچ;

			/* lazily grab local_refs */
			اگر (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			اگر (شمار_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			وگرنه
				refspec_append_mapped(&rs, ref, remote, matched);
		} وگرنه
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

ایستا NORETURN تهی die_افزودن_simple(ساختار branch *branch,
				     ساختار remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	ثابت نویسه *advice_pushdefault_maybe = "";
	ثابت نویسه *advice_automergesimple_maybe = "";
	ثابت نویسه *short_upstream = branch->merge[0]->src;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	skip_prefix(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	اگر (cfg->افزودن_default == افزودن_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	اگر (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

ایستا ثابت نویسه message_detached_head_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

ایستا ثابت نویسه *get_upstream_ref(صحیح flags, ساختار branch *branch, ثابت نویسه *remote_name)
{
	اگر (branch->merge_nr == 0 && (flags & TRANSPORT_افزودن_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		بازگشت branch->refname;
	}

	اگر (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		ثابت نویسه *advice_autosetup_maybe = "";
		اگر (!(flags & TRANSPORT_افزودن_AUTO_UPSTREAM)) {
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
	اگر (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	بازگشت branch->merge[0]->src;
}

ایستا تهی setup_default_افزودن_refspecs(صحیح *flags, ساختار remote *remote)
{
	ساختار branch *branch;
	ثابت نویسه *dst;
	صحیح same_remote;
	ساختار repo_config_مقادیر *cfg = repo_config_مقادیر(the_repository);

	انتخاب (cfg->افزودن_default) {
	حالت افزودن_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		بازگشت;

	حالت افزودن_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		بازگشت;
	پیشفرض:
		بشکن;
	}

	branch = branch_get(پوچ);
	اگر (!branch)
		die(_(message_detached_head_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, پوچ));

	انتخاب (cfg->افزودن_default) {
	پیشفرض:
	حالت افزودن_DEFAULT_UNSPECIFIED:
	حالت افزودن_DEFAULT_SIMPLE:
		اگر (!same_remote)
			بشکن;
		اگر (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_افزودن_simple(branch, remote);
		بشکن;

	حالت افزودن_DEFAULT_UPSTREAM:
		اگر (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		بشکن;

	حالت افزودن_DEFAULT_CURRENT:
		بشکن;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	اگر ((*flags & TRANSPORT_افزودن_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_افزودن_SET_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

ایستا ثابت نویسه message_advice_pull_before_افزودن[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ایستا ثابت نویسه message_advice_checkout_pull_افزودن[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ایستا ثابت نویسه message_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ایستا ثابت نویسه message_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

ایستا ثابت نویسه message_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

ایستا ثابت نویسه message_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ایستا تهی advise_pull_before_افزودن(تهی)
{
	اگر (!advice_enabled(ADVICE_افزودن_NON_FF_CURRENT) || !advice_enabled(ADVICE_افزودن_UPDATE_REJECTED))
		بازگشت;
	advise(_(message_advice_pull_before_افزودن));
}

ایستا تهی advise_checkout_pull_افزودن(تهی)
{
	اگر (!advice_enabled(ADVICE_افزودن_NON_FF_MATCHING) || !advice_enabled(ADVICE_افزودن_UPDATE_REJECTED))
		بازگشت;
	advise(_(message_advice_checkout_pull_افزودن));
}

ایستا تهی advise_ref_already_exists(تهی)
{
	اگر (!advice_enabled(ADVICE_افزودن_ALREADY_EXISTS) || !advice_enabled(ADVICE_افزودن_UPDATE_REJECTED))
		بازگشت;
	advise(_(message_advice_ref_already_exists));
}

ایستا تهی advise_ref_fetch_first(تهی)
{
	اگر (!advice_enabled(ADVICE_افزودن_FETCH_FIRST) || !advice_enabled(ADVICE_افزودن_UPDATE_REJECTED))
		بازگشت;
	advise(_(message_advice_ref_fetch_first));
}

ایستا تهی advise_ref_needs_force(تهی)
{
	اگر (!advice_enabled(ADVICE_افزودن_NEEDS_FORCE) || !advice_enabled(ADVICE_افزودن_UPDATE_REJECTED))
		بازگشت;
	advise(_(message_advice_ref_needs_force));
}

ایستا تهی advise_ref_needs_update(تهی)
{
	اگر (!advice_enabled(ADVICE_افزودن_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_افزودن_UPDATE_REJECTED))
		بازگشت;
	advise(_(message_advice_ref_needs_update));
}

ایستا صحیح افزودن_with_options(ساختار transport *transport, ساختار refspec *rs,
			     صحیح flags)
{
	صحیح err;
	unsigned صحیح reject_reasons;
	نویسه *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	اگر (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : پوچ);

	اگر (!is_empty_cas(&cas)) {
		اگر (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	اگر (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_افزودن(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	اگر (err != 0) {
		fprintf(stderr, "%s", افزودن_get_color(افزودن_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", افزودن_get_color(افزودن_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	آزاد(anon_url);
	اگر (!err)
		بازگشت 0;

	اگر (reject_reasons & REJECT_NON_FF_HEAD) {
		advise_pull_before_افزودن();
	} وگرنه اگر (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_افزودن();
	} وگرنه اگر (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} وگرنه اگر (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} وگرنه اگر (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} وگرنه اگر (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	بازگشت 1;
}

ایستا صحیح do_افزودن(صحیح flags,
		   ثابت ساختار string_list *افزودن_options,
		   ساختار remote *remote)
{
	صحیح errs;
	ساختار strvec *url;
	ساختار refspec *افزودن_refspec = &rs;

	اگر (افزودن_options->nr)
		flags |= TRANSPORT_افزودن_OPTIONS;

	اگر (!افزودن_refspec->nr && !(flags & TRANSPORT_افزودن_ALL)) {
		اگر (remote->افزودن.nr) {
			افزودن_refspec = &remote->افزودن;
		} وگرنه اگر (!(flags & TRANSPORT_افزودن_MIRROR))
			setup_default_افزودن_refspecs(&flags, remote);
	}
	errs = 0;
	url = افزودن_url_of_remote(remote);
	برای (size_t i = 0; i < url->nr; i++) {
		ساختار transport *transport =
			transport_get(remote, url->v[i]);
		اگر (flags & TRANSPORT_افزودن_OPTIONS)
			transport->افزودن_options = افزودن_options;
		اگر (افزودن_with_options(transport, افزودن_refspec, flags))
			errs++;
	}
	بازگشت !!errs;
}

ایستا صحیح option_parse_recurse_submodules(ثابت ساختار option *opt,
				   ثابت نویسه *arg, صحیح unset)
{
	صحیح *recurse_submodules = opt->مقدار;

	اگر (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	وگرنه {
		اگر (!strcmp(arg, "only-is-on-demand")) {
			اگر (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} وگرنه {
			*recurse_submodules = parse_افزودن_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	بازگشت 0;
}

ایستا تهی set_افزودن_cert_flags(صحیح *flags, صحیح v)
{
	انتخاب (v) {
	حالت SEND_PACK_افزودن_CERT_NEVER:
		*flags &= ~(TRANSPORT_افزودن_CERT_ALWAYS | TRANSPORT_افزودن_CERT_IF_ASKED);
		بشکن;
	حالت SEND_PACK_افزودن_CERT_ALWAYS:
		*flags |= TRANSPORT_افزودن_CERT_ALWAYS;
		*flags &= ~TRANSPORT_افزودن_CERT_IF_ASKED;
		بشکن;
	حالت SEND_PACK_افزودن_CERT_IF_ASKED:
		*flags |= TRANSPORT_افزودن_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_افزودن_CERT_ALWAYS;
		بشکن;
	}
}


ایستا صحیح git_افزودن_config(ثابت نویسه *k, ثابت نویسه *v,
			   ثابت ساختار config_context *ctx, تهی *cb)
{
	ثابت نویسه *slot_name;
	صحیح *flags = cb;

	اگر (!strcmp(k, "push.followtags")) {
		اگر (git_config_bool(k, v))
			*flags |= TRANSPORT_افزودن_FOLLOW_TAGS;
		وگرنه
			*flags &= ~TRANSPORT_افزودن_FOLLOW_TAGS;
		بازگشت 0;
	} وگرنه اگر (!strcmp(k, "push.autosetupremote")) {
		اگر (git_config_bool(k, v))
			*flags |= TRANSPORT_افزودن_AUTO_UPSTREAM;
		بازگشت 0;
	} وگرنه اگر (!strcmp(k, "push.gpgsign")) {
		انتخاب (git_parse_maybe_bool(v)) {
		حالت 0:
			set_افزودن_cert_flags(flags, SEND_PACK_افزودن_CERT_NEVER);
			بشکن;
		حالت 1:
			set_افزودن_cert_flags(flags, SEND_PACK_افزودن_CERT_ALWAYS);
			بشکن;
		پیشفرض:
			اگر (!strcasecmp(v, "if-asked"))
				set_افزودن_cert_flags(flags, SEND_PACK_افزودن_CERT_IF_ASKED);
			وگرنه
				بازگشت error(_("invalid value for '%s'"), k);
		}
	} وگرنه اگر (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_افزودن_recurse_submodules_arg(k, v);
	} وگرنه اگر (!strcmp(k, "submodule.recurse")) {
		صحیح val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} وگرنه اگر (!strcmp(k, "push.pushoption")) {
		بازگشت parse_transport_option(k, v, &افزودن_options_config);
	} وگرنه اگر (!strcmp(k, "color.push")) {
		افزودن_use_color = git_config_colorbool(k, v);
		بازگشت 0;
	} وگرنه اگر (skip_prefix(k, "color.push.", &slot_name)) {
		صحیح slot = parse_افزودن_color_slot(slot_name);
		اگر (slot < 0)
			بازگشت 0;
		اگر (!v)
			بازگشت config_error_nonbool(k);
		بازگشت color_parse(v, افزودن_colors[slot]);
	} وگرنه اگر (!strcmp(k, "push.useforceifincludes")) {
		اگر (git_config_bool(k, v))
			*flags |= TRANSPORT_افزودن_FORCE_IF_INCLUDES;
		وگرنه
			*flags &= ~TRANSPORT_افزودن_FORCE_IF_INCLUDES;
		بازگشت 0;
	}

	بازگشت git_default_config(k, v, ctx, پوچ);
}

ایستا صحیح افزودن_multiple(ساختار string_list *list,
			 ثابت ساختار string_list *افزودن_options,
			 صحیح flags,
			 صحیح tags,
			 ثابت نویسه **refspecs,
			 صحیح refspec_nr)
{
	صحیح result = 0;
	size_t i;
	ساختار strvec argv = STRVEC_INIT;

	strvec_افزودن(&argv, "push");

	اگر (flags & TRANSPORT_افزودن_FORCE)
		strvec_افزودن(&argv, "--force");
	اگر (flags & TRANSPORT_افزودن_DRY_RUN)
		strvec_افزودن(&argv, "--dry-run");
	اگر (flags & TRANSPORT_افزودن_PORCELAIN)
		strvec_افزودن(&argv, "--porcelain");
	اگر (flags & TRANSPORT_افزودن_PRUNE)
		strvec_افزودن(&argv, "--prune");
	اگر (flags & TRANSPORT_افزودن_NO_HOOK)
		strvec_افزودن(&argv, "--no-verify");
	اگر (flags & TRANSPORT_افزودن_FOLLOW_TAGS)
		strvec_افزودن(&argv, "--follow-tags");
	اگر (flags & TRANSPORT_افزودن_SET_UPSTREAM)
		strvec_افزودن(&argv, "--set-upstream");
	اگر (flags & TRANSPORT_افزودن_FORCE_IF_INCLUDES)
		strvec_افزودن(&argv, "--force-if-includes");
	اگر (flags & TRANSPORT_افزودن_ALL)
		strvec_افزودن(&argv, "--all");
	اگر (flags & TRANSPORT_افزودن_MIRROR)
		strvec_افزودن(&argv, "--mirror");

	اگر (flags & TRANSPORT_افزودن_CERT_ALWAYS)
		strvec_افزودن(&argv, "--signed=yes");
	وگرنه اگر (flags & TRANSPORT_افزودن_CERT_IF_ASKED)
		strvec_افزودن(&argv, "--signed=if-asked");
	اگر (!thin)
		strvec_افزودن(&argv, "--no-thin");

	اگر (deleterefs)
		strvec_افزودن(&argv, "--delete");

	اگر (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	اگر (verbosity >= 2)
		strvec_افزودن(&argv, "-v");
	اگر (verbosity >= 1)
		strvec_افزودن(&argv, "-v");
	وگرنه اگر (verbosity < 0)
		strvec_افزودن(&argv, "-q");
	اگر (progress > 0)
		strvec_افزودن(&argv, "--progress");
	وگرنه اگر (progress == 0)
		strvec_افزودن(&argv, "--no-progress");

	اگر (family == TRANSPORT_FAMILY_IPV4)
		strvec_افزودن(&argv, "--ipv4");
	وگرنه اگر (family == TRANSPORT_FAMILY_IPV6)
		strvec_افزودن(&argv, "--ipv6");

	اگر (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_افزودن(&argv, "--recurse-submodules=check");
	وگرنه اگر (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_افزودن(&argv, "--recurse-submodules=on-demand");
	وگرنه اگر (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_افزودن(&argv, "--recurse-submodules=only");
	وگرنه اگر (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_افزودن(&argv, "--recurse-submodules=no");


	اگر (tags)
		strvec_افزودن(&argv, "--tags");

	برای (i = 0; i < افزودن_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     افزودن_options->اقلام[i].string);

	برای (i = 0; i < cas.nr; i++) {
		اگر (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} وگرنه اگر (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} وگرنه {
			strvec_افزودن(&argv, "--force-with-lease");
		}
	}

	برای (i = 0; i < list->nr; i++) {
		ثابت نویسه *name = list->اقلام[i].string;
		ساختار child_process cmd = CHILD_PROCESS_INIT;
		صحیح j;

		strvec_pushv(&cmd.args, argv.v);
		strvec_افزودن(&cmd.args, name);

		برای (j = 0; j < refspec_nr; j++)
			strvec_افزودن(&cmd.args, refspecs[j]);

		اگر (verbosity >= 0)
			printf(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		اگر (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			result = 1;
		}
	}

	strvec_clear(&argv);
	بازگشت result;
}

ایستا تهی die_if_repo_looks_like_ref(ثابت نویسه *repo)
{
	ثابت نویسه *slash = strchr(repo, '/');
	ساختار strbuf name = STRBUF_INIT;
	صحیح code;

	اگر (!slash || !slash[1] || file_exists(repo))
		بازگشت;

	strbuf_add(&name, repo, slash - repo);
	اگر (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		بازگشت;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_افزودن_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	exit(code);
}

صحیح cmd_افزودن(صحیح argc,
	     ثابت نویسه **argv,
	     ثابت نویسه *prefix,
	     ساختار repository *repository UNUSED)
{
	صحیح flags = 0;
	صحیح tags = 0;
	صحیح افزودن_cert = -1;
	صحیح rc = 0;
	صحیح base_flags;
	ثابت نویسه *repo = پوچ;	/* default repository */
	ساختار string_list افزودن_options_cmdline = STRING_LIST_INIT_DUP;
	ساختار string_list remote_group = STRING_LIST_INIT_DUP;
	ساختار string_list *افزودن_options;
	ثابت ساختار string_list_item *item;

	ساختار option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_BIT( 0 , "all", &flags, N_("push all branches"), TRANSPORT_افزودن_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_BIT( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_افزودن_MIRROR|TRANSPORT_افزودن_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &tags, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_BIT('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_افزودن_DRY_RUN),
		OPT_BIT( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_افزودن_PORCELAIN),
		OPT_BIT('f', "force", &flags, N_("force updates"), TRANSPORT_افزودن_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_افزودن_cas_option),
		OPT_BIT(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_افزودن_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_BIT('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_افزودن_SET_UPSTREAM),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_BIT(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_افزودن_PRUNE),
		OPT_BIT(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_افزودن_NO_HOOK),
		OPT_BIT(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_افزودن_FOLLOW_TAGS),
		OPT_CALLBACK_F(0, "signed", &افزودن_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_افزودن_signed),
		OPT_BIT(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_افزودن_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &افزودن_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_END()
	};

	packet_trace_identity("push");
	repo_config(the_repository, git_افزودن_config, &flags);
	argc = parse_options(argc, argv, prefix, options, افزودن_usage, 0);
	افزودن_options = (افزودن_options_cmdline.nr
		? &افزودن_options_cmdline
		: &افزودن_options_config);
	set_افزودن_cert_flags(&flags, افزودن_cert);

	refspec_init_افزودن(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  tags, "--tags",
				  flags & TRANSPORT_افزودن_ALL, "--all/--branches",
				  flags & TRANSPORT_افزودن_MIRROR, "--mirror");
	اگر (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	اگر (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	وگرنه اگر (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	وگرنه اگر (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	اگر (argc > 0)
		repo = argv[0];

	اگر (repo) {
		اگر (!add_remote_or_group(repo, &remote_group)) {
			ساختار remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			اگر (advice_enabled(ADVICE_افزودن_REPO_LOOKS_LIKE_REF))
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
			اگر (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_group, r->name);
		}
	} وگرنه {
		ساختار remote *r = pushremote_get(پوچ);
		اگر (!r)
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

	اگر (!is_empty_cas(&cas) && (flags & TRANSPORT_افزودن_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, افزودن_options)
		اگر (strchr(item->string, '\n'))
			die(_("push options must not have new line characters"));

	اگر (remote_group.nr == 1) {
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
			صحیح inner_flags = base_flags;
			ساختار remote *r = pushremote_get(remote_group.اقلام[0].string);
			اگر (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.اقلام[0].string);

			اگر (r->mirror)
				inner_flags |= (TRANSPORT_افزودن_MIRROR|TRANSPORT_افزودن_FORCE);

			اگر (inner_flags & TRANSPORT_افزودن_ALL) {
				اگر (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			اگر (inner_flags & TRANSPORT_افزودن_MIRROR) {
				اگر (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (ساختار refspec) REFSPEC_INIT_افزودن(the_hash_algo);

			اگر (tags)
				refspec_append(&rs, "refs/tags/*");
			اگر (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_افزودن(inner_flags, افزودن_options, r);
		}
	} وگرنه {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		اگر (flags & TRANSPORT_افزودن_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = افزودن_multiple(&remote_group, افزودن_options, flags,
				   tags,
				   argc > 1 ? argv + 1 : پوچ,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&افزودن_options_cmdline, 0);
	string_list_clear(&افزودن_options_config, 0);
	string_list_clear(&remote_group, 0);
	clear_cas_option(&cas);

	اگر (rc == -1)
		usage_with_options(افزودن_usage, options);
	وگرنه
		بازگشت rc;
}
