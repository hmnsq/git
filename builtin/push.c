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

static ကိန်းပြည့် parse_push_color_slot(const var_na *slot)
{
	အကယ်၍ (!strcasecmp(slot, "reset"))
		ပြန်ပေး PUSH_COLOR_RESET;
	အကယ်၍ (!strcasecmp(slot, "error"))
		ပြန်ပေး PUSH_COLOR_ERROR;
	ပြန်ပေး -1;
}

static const var_na *push_get_color(ga_nanaa color_push ix)
{
	အကယ်၍ (want_color_stderr(push_use_color))
		ပြန်ပေး push_colors[ix];
	ပြန်ပေး "";
}

static ကိန်းပြည့် thin = 1;
static ကိန်းပြည့် deleterefs;
static const var_na *receivepack;
static ကိန်းပြည့် verbosity;
static ကိန်းပြည့် progress = -1;
static ကိန်းပြည့် recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
static ga_nanaa transport_family family;

static sa_nracanaa push_cas_option cas;

static sa_nracanaa refspec rs;

static sa_nracanaa string_list push_options_config = STRING_LIST_INIT_DUP;

static void refspec_append_mapped(sa_nracanaa refspec *refspec, const var_na *ref,
				  sa_nracanaa remote *remote, sa_nracanaa ref *matched)
{
	const var_na *branch_name;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	အကယ်၍ (remote->push.nr) {
		sa_nracanaa refspec_item query = {
			.src = matched->name,
		};

		အကယ်၍ (!refspec_find_match(&remote->push, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			free(query.dst);
			ပြန်ပေး;
		}
	}

	အကယ်၍ (cfg->push_default == PUSH_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		sa_nracanaa branch *branch = branch_get(branch_name);
		အကယ်၍ (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			ပြန်ပေး;
		}
	}

	refspec_append(refspec, ref);
}

static void set_refspecs(const var_na **refs, ကိန်းပြည့် nr, sa_nracanaa remote *remote)
{
	sa_nracanaa ref *local_refs = NULL;
	ကိန်းပြည့် i;

	အတွက် (i = 0; i < nr; i++) {
		const var_na *ref = refs[i];
		အကယ်၍ (!strcmp("tag", ref)) {
			အကယ်၍ (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			အကယ်၍ (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			မဟုတ်ပါက
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} မဟုတ်ပါက အကယ်၍ (deleterefs) {
			အကယ်၍ (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} မဟုတ်ပါက အကယ်၍ (!strchr(ref, ':')) {
			sa_nracanaa ref *matched = NULL;

			/* lazily grab local_refs */
			အကယ်၍ (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			အကယ်၍ (count_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			မဟုတ်ပါက
				refspec_append_mapped(&rs, ref, remote, matched);
		} မဟုတ်ပါက
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
	အကယ်၍ (cfg->push_default == PUSH_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	အကယ်၍ (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

static const var_na *get_upstream_ref(ကိန်းပြည့် flags, sa_nracanaa branch *branch, const var_na *remote_name)
{
	အကယ်၍ (branch->merge_nr == 0 && (flags & TRANSPORT_PUSH_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		ပြန်ပေး branch->refname;
	}

	အကယ်၍ (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		const var_na *advice_autosetup_maybe = "";
		အကယ်၍ (!(flags & TRANSPORT_PUSH_AUTO_UPSTREAM)) {
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
	အကယ်၍ (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	ပြန်ပေး branch->merge[0]->src;
}

static void setup_default_push_refspecs(ကိန်းပြည့် *flags, sa_nracanaa remote *remote)
{
	sa_nracanaa branch *branch;
	const var_na *dst;
	ကိန်းပြည့် same_remote;
	sa_nracanaa repo_config_values *cfg = repo_config_values(the_repository);

	switch (cfg->push_default) {
	pak_sa PUSH_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		ပြန်ပေး;

	pak_sa PUSH_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		ပြန်ပေး;
	default:
		bha_nga;
	}

	branch = branch_get(NULL);
	အကယ်၍ (!branch)
		die(_(message_detached_head_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, NULL));

	switch (cfg->push_default) {
	default:
	pak_sa PUSH_DEFAULT_UNSPECIFIED:
	pak_sa PUSH_DEFAULT_SIMPLE:
		အကယ်၍ (!same_remote)
			bha_nga;
		အကယ်၍ (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_push_simple(branch, remote);
		bha_nga;

	pak_sa PUSH_DEFAULT_UPSTREAM:
		အကယ်၍ (!same_remote)
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
	အကယ်၍ ((*flags & TRANSPORT_PUSH_AUTO_UPSTREAM) && branch->merge_nr == 0)
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
	အကယ်၍ (!advice_enabled(ADVICE_PUSH_NON_FF_CURRENT) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		ပြန်ပေး;
	advise(_(message_advice_pull_before_push));
}

static void advise_checkout_pull_push(void)
{
	အကယ်၍ (!advice_enabled(ADVICE_PUSH_NON_FF_MATCHING) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		ပြန်ပေး;
	advise(_(message_advice_checkout_pull_push));
}

static void advise_ref_already_exists(void)
{
	အကယ်၍ (!advice_enabled(ADVICE_PUSH_ALREADY_EXISTS) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		ပြန်ပေး;
	advise(_(message_advice_ref_already_exists));
}

static void advise_ref_fetch_first(void)
{
	အကယ်၍ (!advice_enabled(ADVICE_PUSH_FETCH_FIRST) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		ပြန်ပေး;
	advise(_(message_advice_ref_fetch_first));
}

static void advise_ref_needs_force(void)
{
	အကယ်၍ (!advice_enabled(ADVICE_PUSH_NEEDS_FORCE) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		ပြန်ပေး;
	advise(_(message_advice_ref_needs_force));
}

static void advise_ref_needs_update(void)
{
	အကယ်၍ (!advice_enabled(ADVICE_PUSH_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		ပြန်ပေး;
	advise(_(message_advice_ref_needs_update));
}

static ကိန်းပြည့် push_with_options(sa_nracanaa transport *transport, sa_nracanaa refspec *rs,
			     ကိန်းပြည့် flags)
{
	ကိန်းပြည့် err;
	unsigned ကိန်းပြည့် reject_reasons;
	var_na *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	အကယ်၍ (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : NULL);

	အကယ်၍ (!is_empty_cas(&cas)) {
		အကယ်၍ (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	အကယ်၍ (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_push(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	အကယ်၍ (err != 0) {
		fprintf(stderr, "%s", push_get_color(PUSH_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", push_get_color(PUSH_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	free(anon_url);
	အကယ်၍ (!err)
		ပြန်ပေး 0;

	အကယ်၍ (reject_reasons & REJECT_NON_FF_HEAD) {
		advise_pull_before_push();
	} မဟုတ်ပါက အကယ်၍ (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_push();
	} မဟုတ်ပါက အကယ်၍ (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} မဟုတ်ပါက အကယ်၍ (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} မဟုတ်ပါက အကယ်၍ (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} မဟုတ်ပါက အကယ်၍ (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	ပြန်ပေး 1;
}

static ကိန်းပြည့် do_push(ကိန်းပြည့် flags,
		   const sa_nracanaa string_list *push_options,
		   sa_nracanaa remote *remote)
{
	ကိန်းပြည့် errs;
	sa_nracanaa strvec *url;
	sa_nracanaa refspec *push_refspec = &rs;

	အကယ်၍ (push_options->nr)
		flags |= TRANSPORT_PUSH_OPTIONS;

	အကယ်၍ (!push_refspec->nr && !(flags & TRANSPORT_PUSH_ALL)) {
		အကယ်၍ (remote->push.nr) {
			push_refspec = &remote->push;
		} မဟုတ်ပါက အကယ်၍ (!(flags & TRANSPORT_PUSH_MIRROR))
			setup_default_push_refspecs(&flags, remote);
	}
	errs = 0;
	url = push_url_of_remote(remote);
	အတွက် (size_t i = 0; i < url->nr; i++) {
		sa_nracanaa transport *transport =
			transport_get(remote, url->v[i]);
		အကယ်၍ (flags & TRANSPORT_PUSH_OPTIONS)
			transport->push_options = push_options;
		အကယ်၍ (push_with_options(transport, push_refspec, flags))
			errs++;
	}
	ပြန်ပေး !!errs;
}

static ကိန်းပြည့် option_parse_recurse_submodules(const sa_nracanaa option *opt,
				   const var_na *arg, ကိန်းပြည့် unset)
{
	ကိန်းပြည့် *recurse_submodules = opt->value;

	အကယ်၍ (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	မဟုတ်ပါက {
		အကယ်၍ (!strcmp(arg, "only-is-on-demand")) {
			အကယ်၍ (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} မဟုတ်ပါက {
			*recurse_submodules = parse_push_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	ပြန်ပေး 0;
}

static void set_push_cert_flags(ကိန်းပြည့် *flags, ကိန်းပြည့် v)
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


static ကိန်းပြည့် git_push_config(const var_na *k, const var_na *v,
			   const sa_nracanaa config_context *ctx, void *cb)
{
	const var_na *slot_name;
	ကိန်းပြည့် *flags = cb;

	အကယ်၍ (!strcmp(k, "push.followtags")) {
		အကယ်၍ (git_config_bool(k, v))
			*flags |= TRANSPORT_PUSH_FOLLOW_TAGS;
		မဟုတ်ပါက
			*flags &= ~TRANSPORT_PUSH_FOLLOW_TAGS;
		ပြန်ပေး 0;
	} မဟုတ်ပါက အကယ်၍ (!strcmp(k, "push.autosetupremote")) {
		အကယ်၍ (git_config_bool(k, v))
			*flags |= TRANSPORT_PUSH_AUTO_UPSTREAM;
		ပြန်ပေး 0;
	} မဟုတ်ပါက အကယ်၍ (!strcmp(k, "push.gpgsign")) {
		switch (git_parse_maybe_bool(v)) {
		pak_sa 0:
			set_push_cert_flags(flags, SEND_PACK_PUSH_CERT_NEVER);
			bha_nga;
		pak_sa 1:
			set_push_cert_flags(flags, SEND_PACK_PUSH_CERT_ALWAYS);
			bha_nga;
		default:
			အကယ်၍ (!strcasecmp(v, "if-asked"))
				set_push_cert_flags(flags, SEND_PACK_PUSH_CERT_IF_ASKED);
			မဟုတ်ပါက
				ပြန်ပေး error(_("invalid value for '%s'"), k);
		}
	} မဟုတ်ပါက အကယ်၍ (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_push_recurse_submodules_arg(k, v);
	} မဟုတ်ပါက အကယ်၍ (!strcmp(k, "submodule.recurse")) {
		ကိန်းပြည့် val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} မဟုတ်ပါက အကယ်၍ (!strcmp(k, "push.pushoption")) {
		ပြန်ပေး parse_transport_option(k, v, &push_options_config);
	} မဟုတ်ပါက အကယ်၍ (!strcmp(k, "color.push")) {
		push_use_color = git_config_colorbool(k, v);
		ပြန်ပေး 0;
	} မဟုတ်ပါက အကယ်၍ (skip_prefix(k, "color.push.", &slot_name)) {
		ကိန်းပြည့် slot = parse_push_color_slot(slot_name);
		အကယ်၍ (slot < 0)
			ပြန်ပေး 0;
		အကယ်၍ (!v)
			ပြန်ပေး config_error_nonbool(k);
		ပြန်ပေး color_parse(v, push_colors[slot]);
	} မဟုတ်ပါက အကယ်၍ (!strcmp(k, "push.useforceifincludes")) {
		အကယ်၍ (git_config_bool(k, v))
			*flags |= TRANSPORT_PUSH_FORCE_IF_INCLUDES;
		မဟုတ်ပါက
			*flags &= ~TRANSPORT_PUSH_FORCE_IF_INCLUDES;
		ပြန်ပေး 0;
	}

	ပြန်ပေး git_default_config(k, v, ctx, NULL);
}

static ကိန်းပြည့် push_multiple(sa_nracanaa string_list *list,
			 const sa_nracanaa string_list *push_options,
			 ကိန်းပြည့် flags,
			 ကိန်းပြည့် tags,
			 const var_na **refspecs,
			 ကိန်းပြည့် refspec_nr)
{
	ကိန်းပြည့် result = 0;
	size_t i;
	sa_nracanaa strvec argv = STRVEC_INIT;

	strvec_push(&argv, "push");

	အကယ်၍ (flags & TRANSPORT_PUSH_FORCE)
		strvec_push(&argv, "--force");
	အကယ်၍ (flags & TRANSPORT_PUSH_DRY_RUN)
		strvec_push(&argv, "--dry-run");
	အကယ်၍ (flags & TRANSPORT_PUSH_PORCELAIN)
		strvec_push(&argv, "--porcelain");
	အကယ်၍ (flags & TRANSPORT_PUSH_PRUNE)
		strvec_push(&argv, "--prune");
	အကယ်၍ (flags & TRANSPORT_PUSH_NO_HOOK)
		strvec_push(&argv, "--no-verify");
	အကယ်၍ (flags & TRANSPORT_PUSH_FOLLOW_TAGS)
		strvec_push(&argv, "--follow-tags");
	အကယ်၍ (flags & TRANSPORT_PUSH_SET_UPSTREAM)
		strvec_push(&argv, "--set-upstream");
	အကယ်၍ (flags & TRANSPORT_PUSH_FORCE_IF_INCLUDES)
		strvec_push(&argv, "--force-if-includes");
	အကယ်၍ (flags & TRANSPORT_PUSH_ALL)
		strvec_push(&argv, "--all");
	အကယ်၍ (flags & TRANSPORT_PUSH_MIRROR)
		strvec_push(&argv, "--mirror");

	အကယ်၍ (flags & TRANSPORT_PUSH_CERT_ALWAYS)
		strvec_push(&argv, "--signed=yes");
	မဟုတ်ပါက အကယ်၍ (flags & TRANSPORT_PUSH_CERT_IF_ASKED)
		strvec_push(&argv, "--signed=if-asked");
	အကယ်၍ (!thin)
		strvec_push(&argv, "--no-thin");

	အကယ်၍ (deleterefs)
		strvec_push(&argv, "--delete");

	အကယ်၍ (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	အကယ်၍ (verbosity >= 2)
		strvec_push(&argv, "-v");
	အကယ်၍ (verbosity >= 1)
		strvec_push(&argv, "-v");
	မဟုတ်ပါက အကယ်၍ (verbosity < 0)
		strvec_push(&argv, "-q");
	အကယ်၍ (progress > 0)
		strvec_push(&argv, "--progress");
	မဟုတ်ပါက အကယ်၍ (progress == 0)
		strvec_push(&argv, "--no-progress");

	အကယ်၍ (family == TRANSPORT_FAMILY_IPV4)
		strvec_push(&argv, "--ipv4");
	မဟုတ်ပါက အကယ်၍ (family == TRANSPORT_FAMILY_IPV6)
		strvec_push(&argv, "--ipv6");

	အကယ်၍ (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_push(&argv, "--recurse-submodules=check");
	မဟုတ်ပါက အကယ်၍ (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_push(&argv, "--recurse-submodules=on-demand");
	မဟုတ်ပါက အကယ်၍ (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_push(&argv, "--recurse-submodules=only");
	မဟုတ်ပါက အကယ်၍ (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_push(&argv, "--recurse-submodules=no");


	အကယ်၍ (tags)
		strvec_push(&argv, "--tags");

	အတွက် (i = 0; i < push_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     push_options->items[i].string);

	အတွက် (i = 0; i < cas.nr; i++) {
		အကယ်၍ (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} မဟုတ်ပါက အကယ်၍ (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} မဟုတ်ပါက {
			strvec_push(&argv, "--force-with-lease");
		}
	}

	အတွက် (i = 0; i < list->nr; i++) {
		const var_na *name = list->items[i].string;
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
		ကိန်းပြည့် j;

		strvec_pushv(&cmd.args, argv.v);
		strvec_push(&cmd.args, name);

		အတွက် (j = 0; j < refspec_nr; j++)
			strvec_push(&cmd.args, refspecs[j]);

		အကယ်၍ (verbosity >= 0)
			printf(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		အကယ်၍ (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			result = 1;
		}
	}

	strvec_clear(&argv);
	ပြန်ပေး result;
}

static void die_if_repo_looks_like_ref(const var_na *repo)
{
	const var_na *slash = strchr(repo, '/');
	sa_nracanaa strbuf name = STRBUF_INIT;
	ကိန်းပြည့် code;

	အကယ်၍ (!slash || !slash[1] || file_exists(repo))
		ပြန်ပေး;

	strbuf_add(&name, repo, slash - repo);
	အကယ်၍ (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		ပြန်ပေး;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_PUSH_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	exit(code);
}

ကိန်းပြည့် cmd_push(ကိန်းပြည့် argc,
	     const var_na **argv,
	     const var_na *prefix,
	     sa_nracanaa repository *repository UNUSED)
{
	ကိန်းပြည့် flags = 0;
	ကိန်းပြည့် tags = 0;
	ကိန်းပြည့် push_cert = -1;
	ကိန်းပြည့် rc = 0;
	ကိန်းပြည့် base_flags;
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
	အကယ်၍ (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	အကယ်၍ (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	မဟုတ်ပါက အကယ်၍ (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	မဟုတ်ပါက အကယ်၍ (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	အကယ်၍ (argc > 0)
		repo = argv[0];

	အကယ်၍ (repo) {
		အကယ်၍ (!add_remote_or_group(repo, &remote_group)) {
			sa_nracanaa remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			အကယ်၍ (advice_enabled(ADVICE_PUSH_REPO_LOOKS_LIKE_REF))
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
			အကယ်၍ (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_group, r->name);
		}
	} မဟုတ်ပါက {
		sa_nracanaa remote *r = pushremote_get(NULL);
		အကယ်၍ (!r)
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

	အကယ်၍ (!is_empty_cas(&cas) && (flags & TRANSPORT_PUSH_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, push_options)
		အကယ်၍ (strchr(item->string, '\n'))
			die(_("push options must not have new line characters"));

	အကယ်၍ (remote_group.nr == 1) {
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
			ကိန်းပြည့် inner_flags = base_flags;
			sa_nracanaa remote *r = pushremote_get(remote_group.items[0].string);
			အကယ်၍ (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.items[0].string);

			အကယ်၍ (r->mirror)
				inner_flags |= (TRANSPORT_PUSH_MIRROR|TRANSPORT_PUSH_FORCE);

			အကယ်၍ (inner_flags & TRANSPORT_PUSH_ALL) {
				အကယ်၍ (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			အကယ်၍ (inner_flags & TRANSPORT_PUSH_MIRROR) {
				အကယ်၍ (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (sa_nracanaa refspec) REFSPEC_INIT_PUSH(the_hash_algo);

			အကယ်၍ (tags)
				refspec_append(&rs, "refs/tags/*");
			အကယ်၍ (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_push(inner_flags, push_options, r);
		}
	} မဟုတ်ပါက {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		အကယ်၍ (flags & TRANSPORT_PUSH_ATOMIC)
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

	အကယ်၍ (rc == -1)
		usage_with_options(push_usage, options);
	မဟုတ်ပါက
		ပြန်ပေး rc;
}
