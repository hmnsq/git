/*
 * "git push"
 */

#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "advice.h"
#include "branch.h"
#include "config.h"
#include "dir.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "refspec.h"
#include "run-command.h"
#include "remote.h"
#include "transport.h"
#include "parse-options.h"
#include "pkt-line.h"
#include "submodule.h"
#include "submodule-config.h"
#include "send-pack.h"
#include "trace2.h"
#include "color.h"

static const var_na * const push_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	NULL,
};

static ga_nanaa git_colorbool push_use_color = GIT_COLOR_UNKNOWN;
static var_na push_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

ga_nanaa color_push {
	PUSH_COLOR_RESET = 0,
	PUSH_COLOR_ERROR = 1
};

static පූර්ණ_සංඛ්‍යා parse_push_color_slot(const var_na *slot)
{
	නම් (!strcasecmp(slot, "reset"))
		ආපසු PUSH_COLOR_RESET;
	නම් (!strcasecmp(slot, "error"))
		ආපසු PUSH_COLOR_ERROR;
	ආපසු -1;
}

static const var_na *push_get_color(ga_nanaa color_push ix)
{
	නම් (want_color_stderr(push_use_color))
		ආපසු push_colors[ix];
	ආපසු "";
}

static පූර්ණ_සංඛ්‍යා thin = 1;
static පූර්ණ_සංඛ්‍යා deleterefs;
static const var_na *receivepack;
static පූර්ණ_සංඛ්‍යා verbosity;
static පූර්ණ_සංඛ්‍යා progress = -1;
static පූර්ණ_සංඛ්‍යා recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
static ga_nanaa transport_family family;

static sa_nracanaa push_cas_option cas;

static sa_nracanaa refspec rs;

static sa_nracanaa string_list push_options_config = STRING_LIST_INIT_DUP;

static void refspec_append_mapped(sa_nracanaa refspec *refspec, const var_na *ref,
				  sa_nracanaa remote *remote, sa_nracanaa ref *matched)
{
	const var_na *branch_name;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	නම් (remote->push.nr) {
		sa_nracanaa refspec_item query = {
			.src = matched->name,
		};

		නම් (!refspec_find_match(&remote->push, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			free(query.dst);
			ආපසු;
		}
	}

	නම් (cfg->push_default == PUSH_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		sa_nracanaa branch *branch = branch_get(branch_name);
		නම් (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			ආපසු;
		}
	}

	refspec_append(refspec, ref);
}

static void set_refspecs(const var_na **refs, පූර්ණ_සංඛ්‍යා nr, sa_nracanaa remote *remote)
{
	sa_nracanaa ref *local_refs = NULL;
	පූර්ණ_සංඛ්‍යා i;

	සඳහා (i = 0; i < nr; i++) {
		const var_na *ref = refs[i];
		නම් (!strcmp("tag", ref)) {
			නම් (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			නම් (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			නැතහොත්
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} නැතහොත් නම් (deleterefs) {
			නම් (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} නැතහොත් නම් (!strchr(ref, ':')) {
			sa_nracanaa ref *matched = NULL;

			/* lazily grab local_refs */
			නම් (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			නම් (count_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			නැතහොත්
				refspec_append_mapped(&rs, ref, remote, matched);
		} නැතහොත්
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

static NORETURN void die_push_simple(sa_nracanaa branch *branch,
				     sa_nracanaa remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	const var_na *advice_pushdefault_maybe = "";
	const var_na *advice_automergesimple_maybe = "";
	const var_na *short_upstream = branch->merge[0]->src;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	skip_prefix(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	නම් (cfg->push_default == PUSH_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	නම් (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

static const var_na message_detached_head_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

static const var_na *get_upstream_ref(පූර්ණ_සංඛ්‍යා flags, sa_nracanaa branch *branch, const var_na *remote_name)
{
	නම් (branch->merge_nr == 0 && (flags & TRANSPORT_PUSH_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		ආපසු branch->refname;
	}

	නම් (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		const var_na *advice_autosetup_maybe = "";
		නම් (!(flags & TRANSPORT_PUSH_AUTO_UPSTREAM)) {
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
	නම් (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	ආපසු branch->merge[0]->src;
}

static void setup_default_push_refspecs(පූර්ණ_සංඛ්‍යා *flags, sa_nracanaa remote *remote)
{
	sa_nracanaa branch *branch;
	const var_na *dst;
	පූර්ණ_සංඛ්‍යා same_remote;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	switch (cfg->push_default) {
	pak_sa PUSH_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		ආපසු;

	pak_sa PUSH_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		ආපසු;
	default:
		bha_nga;
	}

	branch = branch_get(NULL);
	නම් (!branch)
		die(_(message_detached_head_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, NULL));

	switch (cfg->push_default) {
	default:
	pak_sa PUSH_DEFAULT_UNSPECIFIED:
	pak_sa PUSH_DEFAULT_SIMPLE:
		නම් (!same_remote)
			bha_nga;
		නම් (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_push_simple(branch, remote);
		bha_nga;

	pak_sa PUSH_DEFAULT_UPSTREAM:
		නම් (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		bha_nga;

	pak_sa PUSH_DEFAULT_CURRENT:
		bha_nga;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	නම් ((*flags & TRANSPORT_PUSH_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_PUSH_SET_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

static const var_na message_advice_pull_before_push[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

static const var_na message_advice_checkout_pull_push[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

static const var_na message_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

static const var_na message_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

static const var_na message_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

static const var_na message_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

static void advise_pull_before_push(void)
{
	නම් (!advice_enabled(ADVICE_PUSH_NON_FF_CURRENT) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		ආපසු;
	advise(_(message_advice_pull_before_push));
}

static void advise_checkout_pull_push(void)
{
	නම් (!advice_enabled(ADVICE_PUSH_NON_FF_MATCHING) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		ආපසු;
	advise(_(message_advice_checkout_pull_push));
}

static void advise_ref_already_exists(void)
{
	නම් (!advice_enabled(ADVICE_PUSH_ALREADY_EXISTS) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		ආපසු;
	advise(_(message_advice_ref_already_exists));
}

static void advise_ref_fetch_first(void)
{
	නම් (!advice_enabled(ADVICE_PUSH_FETCH_FIRST) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		ආපසු;
	advise(_(message_advice_ref_fetch_first));
}

static void advise_ref_needs_force(void)
{
	නම් (!advice_enabled(ADVICE_PUSH_NEEDS_FORCE) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		ආපසු;
	advise(_(message_advice_ref_needs_force));
}

static void advise_ref_needs_update(void)
{
	නම් (!advice_enabled(ADVICE_PUSH_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		ආපසු;
	advise(_(message_advice_ref_needs_update));
}

static පූර්ණ_සංඛ්‍යා push_with_options(sa_nracanaa transport *transport, sa_nracanaa refspec *rs,
			     පූර්ණ_සංඛ්‍යා flags)
{
	පූර්ණ_සංඛ්‍යා err;
	unsigned පූර්ණ_සංඛ්‍යා reject_reasons;
	var_na *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	නම් (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : NULL);

	නම් (!is_empty_cas(&cas)) {
		නම් (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	නම් (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_push(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	නම් (err != 0) {
		fprintf(stderr, "%s", push_get_color(PUSH_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", push_get_color(PUSH_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	free(anon_url);
	නම් (!err)
		ආපසු 0;

	නම් (reject_reasons & REJECT_NON_FF_HEAD) {
		advise_pull_before_push();
	} නැතහොත් නම් (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_push();
	} නැතහොත් නම් (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} නැතහොත් නම් (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} නැතහොත් නම් (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} නැතහොත් නම් (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	ආපසු 1;
}

static පූර්ණ_සංඛ්‍යා do_push(පූර්ණ_සංඛ්‍යා flags,
		   const sa_nracanaa string_list *push_options,
		   sa_nracanaa remote *remote)
{
	පූර්ණ_සංඛ්‍යා errs;
	sa_nracanaa strvec *url;
	sa_nracanaa refspec *push_refspec = &rs;

	නම් (push_options->nr)
		flags |= TRANSPORT_PUSH_OPTIONS;

	නම් (!push_refspec->nr && !(flags & TRANSPORT_PUSH_ALL)) {
		නම් (remote->push.nr) {
			push_refspec = &remote->push;
		} නැතහොත් නම් (!(flags & TRANSPORT_PUSH_MIRROR))
			setup_default_push_refspecs(&flags, remote);
	}
	errs = 0;
	url = push_url_of_remote(remote);
	සඳහා (size_t i = 0; i < url->nr; i++) {
		sa_nracanaa transport *transport =
			transport_get(remote, url->v[i]);
		නම් (flags & TRANSPORT_PUSH_OPTIONS)
			transport->push_options = push_options;
		නම් (push_with_options(transport, push_refspec, flags))
			errs++;
	}
	ආපසු !!errs;
}

static පූර්ණ_සංඛ්‍යා option_parse_recurse_submodules(const sa_nracanaa option *opt,
				   const var_na *arg, පූර්ණ_සංඛ්‍යා unset)
{
	පූර්ණ_සංඛ්‍යා *recurse_submodules = opt->value;

	නම් (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	නැතහොත් {
		නම් (!strcmp(arg, "only-is-on-demand")) {
			නම් (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} නැතහොත් {
			*recurse_submodules = parse_push_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	ආපසු 0;
}

static void set_push_cert_flags(පූර්ණ_සංඛ්‍යා *flags, පූර්ණ_සංඛ්‍යා v)
{
	switch (v) {
	pak_sa SEND_PACK_PUSH_CERT_NEVER:
		*flags &= ~(TRANSPORT_PUSH_CERT_ALWAYS | TRANSPORT_PUSH_CERT_IF_ASKED);
		bha_nga;
	pak_sa SEND_PACK_PUSH_CERT_ALWAYS:
		*flags |= TRANSPORT_PUSH_CERT_ALWAYS;
		*flags &= ~TRANSPORT_PUSH_CERT_IF_ASKED;
		bha_nga;
	pak_sa SEND_PACK_PUSH_CERT_IF_ASKED:
		*flags |= TRANSPORT_PUSH_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_PUSH_CERT_ALWAYS;
		bha_nga;
	}
}


static පූර්ණ_සංඛ්‍යා git_push_config(const var_na *k, const var_na *v,
			   const sa_nracanaa config_context *ctx, void *cb)
{
	const var_na *slot_name;
	පූර්ණ_සංඛ්‍යා *flags = cb;

	නම් (!strcmp(k, "push.followtags")) {
		නම් (git_config_bool(k, v))
			*flags |= TRANSPORT_PUSH_FOLLOW_TAGS;
		නැතහොත්
			*flags &= ~TRANSPORT_PUSH_FOLLOW_TAGS;
		ආපසු 0;
	} නැතහොත් නම් (!strcmp(k, "push.autosetupremote")) {
		නම් (git_config_bool(k, v))
			*flags |= TRANSPORT_PUSH_AUTO_UPSTREAM;
		ආපසු 0;
	} නැතහොත් නම් (!strcmp(k, "push.gpgsign")) {
		switch (git_parse_maybe_bool(v)) {
		pak_sa 0:
			set_push_cert_flags(flags, SEND_PACK_PUSH_CERT_NEVER);
			bha_nga;
		pak_sa 1:
			set_push_cert_flags(flags, SEND_PACK_PUSH_CERT_ALWAYS);
			bha_nga;
		default:
			නම් (!strcasecmp(v, "if-asked"))
				set_push_cert_flags(flags, SEND_PACK_PUSH_CERT_IF_ASKED);
			නැතහොත්
				ආපසු error(_("invalid value for '%s'"), k);
		}
	} නැතහොත් නම් (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_push_recurse_submodules_arg(k, v);
	} නැතහොත් නම් (!strcmp(k, "submodule.recurse")) {
		පූර්ණ_සංඛ්‍යා val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} නැතහොත් නම් (!strcmp(k, "push.pushoption")) {
		ආපසු parse_transport_option(k, v, &push_options_config);
	} නැතහොත් නම් (!strcmp(k, "color.push")) {
		push_use_color = git_config_colorbool(k, v);
		ආපසු 0;
	} නැතහොත් නම් (skip_prefix(k, "color.push.", &slot_name)) {
		පූර්ණ_සංඛ්‍යා slot = parse_push_color_slot(slot_name);
		නම් (slot < 0)
			ආපසු 0;
		නම් (!v)
			ආපසු config_error_nonbool(k);
		ආපසු color_parse(v, push_colors[slot]);
	} නැතහොත් නම් (!strcmp(k, "push.useforceifincludes")) {
		නම් (git_config_bool(k, v))
			*flags |= TRANSPORT_PUSH_FORCE_IF_INCLUDES;
		නැතහොත්
			*flags &= ~TRANSPORT_PUSH_FORCE_IF_INCLUDES;
		ආපසු 0;
	}

	ආපසු git_default_config(k, v, ctx, NULL);
}

static පූර්ණ_සංඛ්‍යා push_multiple(sa_nracanaa string_list *list,
			 const sa_nracanaa string_list *push_options,
			 පූර්ණ_සංඛ්‍යා flags,
			 පූර්ණ_සංඛ්‍යා tags,
			 const var_na **refspecs,
			 පූර්ණ_සංඛ්‍යා refspec_nr)
{
	පූර්ණ_සංඛ්‍යා result = 0;
	size_t i;
	sa_nracanaa strvec argv = STRVEC_INIT;

	strvec_push(&argv, "push");

	නම් (flags & TRANSPORT_PUSH_FORCE)
		strvec_push(&argv, "--force");
	නම් (flags & TRANSPORT_PUSH_DRY_RUN)
		strvec_push(&argv, "--dry-run");
	නම් (flags & TRANSPORT_PUSH_PORCELAIN)
		strvec_push(&argv, "--porcelain");
	නම් (flags & TRANSPORT_PUSH_PRUNE)
		strvec_push(&argv, "--prune");
	නම් (flags & TRANSPORT_PUSH_NO_HOOK)
		strvec_push(&argv, "--no-verify");
	නම් (flags & TRANSPORT_PUSH_FOLLOW_TAGS)
		strvec_push(&argv, "--follow-tags");
	නම් (flags & TRANSPORT_PUSH_SET_UPSTREAM)
		strvec_push(&argv, "--set-upstream");
	නම් (flags & TRANSPORT_PUSH_FORCE_IF_INCLUDES)
		strvec_push(&argv, "--force-if-includes");
	නම් (flags & TRANSPORT_PUSH_ALL)
		strvec_push(&argv, "--all");
	නම් (flags & TRANSPORT_PUSH_MIRROR)
		strvec_push(&argv, "--mirror");

	නම් (flags & TRANSPORT_PUSH_CERT_ALWAYS)
		strvec_push(&argv, "--signed=yes");
	නැතහොත් නම් (flags & TRANSPORT_PUSH_CERT_IF_ASKED)
		strvec_push(&argv, "--signed=if-asked");
	නම් (!thin)
		strvec_push(&argv, "--no-thin");

	නම් (deleterefs)
		strvec_push(&argv, "--delete");

	නම් (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	නම් (verbosity >= 2)
		strvec_push(&argv, "-v");
	නම් (verbosity >= 1)
		strvec_push(&argv, "-v");
	නැතහොත් නම් (verbosity < 0)
		strvec_push(&argv, "-q");
	නම් (progress > 0)
		strvec_push(&argv, "--progress");
	නැතහොත් නම් (progress == 0)
		strvec_push(&argv, "--no-progress");

	නම් (family == TRANSPORT_FAMILY_IPV4)
		strvec_push(&argv, "--ipv4");
	නැතහොත් නම් (family == TRANSPORT_FAMILY_IPV6)
		strvec_push(&argv, "--ipv6");

	නම් (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_push(&argv, "--recurse-submodules=check");
	නැතහොත් නම් (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_push(&argv, "--recurse-submodules=on-demand");
	නැතහොත් නම් (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_push(&argv, "--recurse-submodules=only");
	නැතහොත් නම් (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_push(&argv, "--recurse-submodules=no");


	නම් (tags)
		strvec_push(&argv, "--tags");

	සඳහා (i = 0; i < push_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     push_options->items[i].string);

	සඳහා (i = 0; i < cas.nr; i++) {
		නම් (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} නැතහොත් නම් (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} නැතහොත් {
			strvec_push(&argv, "--force-with-lease");
		}
	}

	සඳහා (i = 0; i < list->nr; i++) {
		const var_na *name = list->items[i].string;
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
		පූර්ණ_සංඛ්‍යා j;

		strvec_pushv(&cmd.args, argv.v);
		strvec_push(&cmd.args, name);

		සඳහා (j = 0; j < refspec_nr; j++)
			strvec_push(&cmd.args, refspecs[j]);

		නම් (verbosity >= 0)
			printf(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		නම් (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			result = 1;
		}
	}

	strvec_clear(&argv);
	ආපසු result;
}

static void die_if_repo_looks_like_ref(const var_na *repo)
{
	const var_na *slash = strchr(repo, '/');
	sa_nracanaa strbuf name = STRBUF_INIT;
	පූර්ණ_සංඛ්‍යා code;

	නම් (!slash || !slash[1] || file_exists(repo))
		ආපසු;

	strbuf_add(&name, repo, slash - repo);
	නම් (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		ආපසු;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_PUSH_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	exit(code);
}

පූර්ණ_සංඛ්‍යා cmd_push(පූර්ණ_සංඛ්‍යා argc,
	     const var_na **argv,
	     const var_na *prefix,
	     sa_nracanaa repository *repository UNUSED)
{
	පූර්ණ_සංඛ්‍යා flags = 0;
	පූර්ණ_සංඛ්‍යා tags = 0;
	පූර්ණ_සංඛ්‍යා push_cert = -1;
	පූර්ණ_සංඛ්‍යා rc = 0;
	පූර්ණ_සංඛ්‍යා base_flags;
	const var_na *repo = NULL;	/* default repository */
	sa_nracanaa string_list push_options_cmdline = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list remote_group = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list *push_options;
	const sa_nracanaa string_list_item *item;

	sa_nracanaa option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_BIT( 0 , "all", &flags, N_("push all branches"), TRANSPORT_PUSH_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_BIT( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_PUSH_MIRROR|TRANSPORT_PUSH_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &tags, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_BIT('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_PUSH_DRY_RUN),
		OPT_BIT( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_PUSH_PORCELAIN),
		OPT_BIT('f', "force", &flags, N_("force updates"), TRANSPORT_PUSH_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_push_cas_option),
		OPT_BIT(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_PUSH_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_BIT('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_PUSH_SET_UPSTREAM),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_BIT(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_PUSH_PRUNE),
		OPT_BIT(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_PUSH_NO_HOOK),
		OPT_BIT(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_PUSH_FOLLOW_TAGS),
		OPT_CALLBACK_F(0, "signed", &push_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_push_signed),
		OPT_BIT(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_PUSH_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &push_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_END()
	};

	packet_trace_identity("push");
	repo_config(the_repository, git_push_config, &flags);
	argc = parse_options(argc, argv, prefix, options, push_usage, 0);
	push_options = (push_options_cmdline.nr
		? &push_options_cmdline
		: &push_options_config);
	set_push_cert_flags(&flags, push_cert);

	refspec_init_push(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  tags, "--tags",
				  flags & TRANSPORT_PUSH_ALL, "--all/--branches",
				  flags & TRANSPORT_PUSH_MIRROR, "--mirror");
	නම් (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	නම් (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	නැතහොත් නම් (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	නැතහොත් නම් (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	නම් (argc > 0)
		repo = argv[0];

	නම් (repo) {
		නම් (!add_remote_or_group(repo, &remote_group)) {
			sa_nracanaa remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			නම් (advice_enabled(ADVICE_PUSH_REPO_LOOKS_LIKE_REF))
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
			නම් (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_group, r->name);
		}
	} නැතහොත් {
		sa_nracanaa remote *r = pushremote_get(NULL);
		නම් (!r)
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

	නම් (!is_empty_cas(&cas) && (flags & TRANSPORT_PUSH_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, push_options)
		නම් (strchr(item->string, '\n'))
			die(_("push options must not have new line characters"));

	නම් (remote_group.nr == 1) {
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
			පූර්ණ_සංඛ්‍යා inner_flags = base_flags;
			sa_nracanaa remote *r = pushremote_get(remote_group.items[0].string);
			නම් (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.items[0].string);

			නම් (r->mirror)
				inner_flags |= (TRANSPORT_PUSH_MIRROR|TRANSPORT_PUSH_FORCE);

			නම් (inner_flags & TRANSPORT_PUSH_ALL) {
				නම් (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			නම් (inner_flags & TRANSPORT_PUSH_MIRROR) {
				නම් (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (sa_nracanaa refspec) REFSPEC_INIT_PUSH(the_hash_algo);

			නම් (tags)
				refspec_append(&rs, "refs/tags/*");
			නම් (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_push(inner_flags, push_options, r);
		}
	} නැතහොත් {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		නම් (flags & TRANSPORT_PUSH_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = push_multiple(&remote_group, push_options, flags,
				   tags,
				   argc > 1 ? argv + 1 : NULL,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&push_options_cmdline, 0);
	string_list_clear(&push_options_config, 0);
	string_list_clear(&remote_group, 0);
	clear_cas_option(&cas);

	නම් (rc == -1)
		usage_with_options(push_usage, options);
	නැතහොත්
		ආපසු rc;
}
