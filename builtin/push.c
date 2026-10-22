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

static const char * const push_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	NULL,
};

static enum git_colorbool push_use_color = GIT_COLOR_UNKNOWN;
static char push_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

enum color_push {
	PUSH_COLOR_RESET = 0,
	PUSH_COLOR_ERROR = 1
};

static số_nguyên parse_push_color_slot(const char *slot)
{
	nếu (!strcasecmp(slot, "reset"))
		trả_về PUSH_COLOR_RESET;
	nếu (!strcasecmp(slot, "error"))
		trả_về PUSH_COLOR_ERROR;
	trả_về -1;
}

static const char *push_get_color(enum color_push ix)
{
	nếu (want_color_stderr(push_use_color))
		trả_về push_colors[ix];
	trả_về "";
}

static số_nguyên thin = 1;
static số_nguyên deleterefs;
static const char *receivepack;
static số_nguyên verbosity;
static số_nguyên progress = -1;
static số_nguyên recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
static enum transport_family family;

static struct push_cas_option cas;

static struct refspec rs;

static struct string_list push_options_config = STRING_LIST_INIT_DUP;

static void refspec_append_mapped(struct refspec *refspec, const char *ref,
				  struct remote *remote, struct ref *matched)
{
	const char *branch_name;
	struct repo_config_values *cfg = repo_config_values(the_repository);

	nếu (remote->push.nr) {
		struct refspec_item query = {
			.src = matched->name,
		};

		nếu (!refspec_find_match(&remote->push, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			free(query.dst);
			trả_về;
		}
	}

	nếu (cfg->push_default == PUSH_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		struct branch *branch = branch_get(branch_name);
		nếu (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			trả_về;
		}
	}

	refspec_append(refspec, ref);
}

static void set_refspecs(const char **refs, số_nguyên nr, struct remote *remote)
{
	struct ref *local_refs = NULL;
	số_nguyên i;

	cho (i = 0; i < nr; i++) {
		const char *ref = refs[i];
		nếu (!strcmp("tag", ref)) {
			nếu (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			nếu (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			không_thì
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} không_thì nếu (deleterefs) {
			nếu (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} không_thì nếu (!strchr(ref, ':')) {
			struct ref *matched = NULL;

			/* lazily grab local_refs */
			nếu (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			nếu (count_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			không_thì
				refspec_append_mapped(&rs, ref, remote, matched);
		} không_thì
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

static NORETURN void die_push_simple(struct branch *branch,
				     struct remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	const char *advice_pushdefault_maybe = "";
	const char *advice_automergesimple_maybe = "";
	const char *short_upstream = branch->merge[0]->src;
	struct repo_config_values *cfg = repo_config_values(the_repository);

	skip_prefix(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	nếu (cfg->push_default == PUSH_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	nếu (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

static const char message_detached_head_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

static const char *get_upstream_ref(số_nguyên flags, struct branch *branch, const char *remote_name)
{
	nếu (branch->merge_nr == 0 && (flags & TRANSPORT_PUSH_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		trả_về branch->refname;
	}

	nếu (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		const char *advice_autosetup_maybe = "";
		nếu (!(flags & TRANSPORT_PUSH_AUTO_UPSTREAM)) {
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
	nếu (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	trả_về branch->merge[0]->src;
}

static void setup_default_push_refspecs(số_nguyên *flags, struct remote *remote)
{
	struct branch *branch;
	const char *dst;
	số_nguyên same_remote;
	struct repo_config_values *cfg = repo_config_values(the_repository);

	switch (cfg->push_default) {
	case PUSH_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		trả_về;

	case PUSH_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		trả_về;
	default:
		break;
	}

	branch = branch_get(NULL);
	nếu (!branch)
		die(_(message_detached_head_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, NULL));

	switch (cfg->push_default) {
	default:
	case PUSH_DEFAULT_UNSPECIFIED:
	case PUSH_DEFAULT_SIMPLE:
		nếu (!same_remote)
			break;
		nếu (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_push_simple(branch, remote);
		break;

	case PUSH_DEFAULT_UPSTREAM:
		nếu (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		break;

	case PUSH_DEFAULT_CURRENT:
		break;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	nếu ((*flags & TRANSPORT_PUSH_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_PUSH_SET_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

static const char message_advice_pull_before_push[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

static const char message_advice_checkout_pull_push[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

static const char message_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

static const char message_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

static const char message_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

static const char message_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

static void advise_pull_before_push(void)
{
	nếu (!advice_enabled(ADVICE_PUSH_NON_FF_CURRENT) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		trả_về;
	advise(_(message_advice_pull_before_push));
}

static void advise_checkout_pull_push(void)
{
	nếu (!advice_enabled(ADVICE_PUSH_NON_FF_MATCHING) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		trả_về;
	advise(_(message_advice_checkout_pull_push));
}

static void advise_ref_already_exists(void)
{
	nếu (!advice_enabled(ADVICE_PUSH_ALREADY_EXISTS) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		trả_về;
	advise(_(message_advice_ref_already_exists));
}

static void advise_ref_fetch_first(void)
{
	nếu (!advice_enabled(ADVICE_PUSH_FETCH_FIRST) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		trả_về;
	advise(_(message_advice_ref_fetch_first));
}

static void advise_ref_needs_force(void)
{
	nếu (!advice_enabled(ADVICE_PUSH_NEEDS_FORCE) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		trả_về;
	advise(_(message_advice_ref_needs_force));
}

static void advise_ref_needs_update(void)
{
	nếu (!advice_enabled(ADVICE_PUSH_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_PUSH_UPDATE_REJECTED))
		trả_về;
	advise(_(message_advice_ref_needs_update));
}

static số_nguyên push_with_options(struct transport *transport, struct refspec *rs,
			     số_nguyên flags)
{
	số_nguyên err;
	unsigned số_nguyên reject_reasons;
	char *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	nếu (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : NULL);

	nếu (!is_empty_cas(&cas)) {
		nếu (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	nếu (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_push(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	nếu (err != 0) {
		fprintf(stderr, "%s", push_get_color(PUSH_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", push_get_color(PUSH_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	free(anon_url);
	nếu (!err)
		trả_về 0;

	nếu (reject_reasons & REJECT_NON_FF_HEAD) {
		advise_pull_before_push();
	} không_thì nếu (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_push();
	} không_thì nếu (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} không_thì nếu (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} không_thì nếu (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} không_thì nếu (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	trả_về 1;
}

static số_nguyên do_push(số_nguyên flags,
		   const struct string_list *push_options,
		   struct remote *remote)
{
	số_nguyên errs;
	struct strvec *url;
	struct refspec *push_refspec = &rs;

	nếu (push_options->nr)
		flags |= TRANSPORT_PUSH_OPTIONS;

	nếu (!push_refspec->nr && !(flags & TRANSPORT_PUSH_ALL)) {
		nếu (remote->push.nr) {
			push_refspec = &remote->push;
		} không_thì nếu (!(flags & TRANSPORT_PUSH_MIRROR))
			setup_default_push_refspecs(&flags, remote);
	}
	errs = 0;
	url = push_url_of_remote(remote);
	cho (size_t i = 0; i < url->nr; i++) {
		struct transport *transport =
			transport_get(remote, url->v[i]);
		nếu (flags & TRANSPORT_PUSH_OPTIONS)
			transport->push_options = push_options;
		nếu (push_with_options(transport, push_refspec, flags))
			errs++;
	}
	trả_về !!errs;
}

static số_nguyên option_parse_recurse_submodules(const struct option *opt,
				   const char *arg, số_nguyên unset)
{
	số_nguyên *recurse_submodules = opt->value;

	nếu (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	không_thì {
		nếu (!strcmp(arg, "only-is-on-demand")) {
			nếu (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} không_thì {
			*recurse_submodules = parse_push_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	trả_về 0;
}

static void set_push_cert_flags(số_nguyên *flags, số_nguyên v)
{
	switch (v) {
	case SEND_PACK_PUSH_CERT_NEVER:
		*flags &= ~(TRANSPORT_PUSH_CERT_ALWAYS | TRANSPORT_PUSH_CERT_IF_ASKED);
		break;
	case SEND_PACK_PUSH_CERT_ALWAYS:
		*flags |= TRANSPORT_PUSH_CERT_ALWAYS;
		*flags &= ~TRANSPORT_PUSH_CERT_IF_ASKED;
		break;
	case SEND_PACK_PUSH_CERT_IF_ASKED:
		*flags |= TRANSPORT_PUSH_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_PUSH_CERT_ALWAYS;
		break;
	}
}


static số_nguyên git_push_config(const char *k, const char *v,
			   const struct config_context *ctx, void *cb)
{
	const char *slot_name;
	số_nguyên *flags = cb;

	nếu (!strcmp(k, "push.followtags")) {
		nếu (git_config_bool(k, v))
			*flags |= TRANSPORT_PUSH_FOLLOW_TAGS;
		không_thì
			*flags &= ~TRANSPORT_PUSH_FOLLOW_TAGS;
		trả_về 0;
	} không_thì nếu (!strcmp(k, "push.autosetupremote")) {
		nếu (git_config_bool(k, v))
			*flags |= TRANSPORT_PUSH_AUTO_UPSTREAM;
		trả_về 0;
	} không_thì nếu (!strcmp(k, "push.gpgsign")) {
		switch (git_parse_maybe_bool(v)) {
		case 0:
			set_push_cert_flags(flags, SEND_PACK_PUSH_CERT_NEVER);
			break;
		case 1:
			set_push_cert_flags(flags, SEND_PACK_PUSH_CERT_ALWAYS);
			break;
		default:
			nếu (!strcasecmp(v, "if-asked"))
				set_push_cert_flags(flags, SEND_PACK_PUSH_CERT_IF_ASKED);
			không_thì
				trả_về error(_("invalid value for '%s'"), k);
		}
	} không_thì nếu (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_push_recurse_submodules_arg(k, v);
	} không_thì nếu (!strcmp(k, "submodule.recurse")) {
		số_nguyên val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} không_thì nếu (!strcmp(k, "push.pushoption")) {
		trả_về parse_transport_option(k, v, &push_options_config);
	} không_thì nếu (!strcmp(k, "color.push")) {
		push_use_color = git_config_colorbool(k, v);
		trả_về 0;
	} không_thì nếu (skip_prefix(k, "color.push.", &slot_name)) {
		số_nguyên slot = parse_push_color_slot(slot_name);
		nếu (slot < 0)
			trả_về 0;
		nếu (!v)
			trả_về config_error_nonbool(k);
		trả_về color_parse(v, push_colors[slot]);
	} không_thì nếu (!strcmp(k, "push.useforceifincludes")) {
		nếu (git_config_bool(k, v))
			*flags |= TRANSPORT_PUSH_FORCE_IF_INCLUDES;
		không_thì
			*flags &= ~TRANSPORT_PUSH_FORCE_IF_INCLUDES;
		trả_về 0;
	}

	trả_về git_default_config(k, v, ctx, NULL);
}

static số_nguyên push_multiple(struct string_list *list,
			 const struct string_list *push_options,
			 số_nguyên flags,
			 số_nguyên tags,
			 const char **refspecs,
			 số_nguyên refspec_nr)
{
	số_nguyên result = 0;
	size_t i;
	struct strvec argv = STRVEC_INIT;

	strvec_push(&argv, "push");

	nếu (flags & TRANSPORT_PUSH_FORCE)
		strvec_push(&argv, "--force");
	nếu (flags & TRANSPORT_PUSH_DRY_RUN)
		strvec_push(&argv, "--dry-run");
	nếu (flags & TRANSPORT_PUSH_PORCELAIN)
		strvec_push(&argv, "--porcelain");
	nếu (flags & TRANSPORT_PUSH_PRUNE)
		strvec_push(&argv, "--prune");
	nếu (flags & TRANSPORT_PUSH_NO_HOOK)
		strvec_push(&argv, "--no-verify");
	nếu (flags & TRANSPORT_PUSH_FOLLOW_TAGS)
		strvec_push(&argv, "--follow-tags");
	nếu (flags & TRANSPORT_PUSH_SET_UPSTREAM)
		strvec_push(&argv, "--set-upstream");
	nếu (flags & TRANSPORT_PUSH_FORCE_IF_INCLUDES)
		strvec_push(&argv, "--force-if-includes");
	nếu (flags & TRANSPORT_PUSH_ALL)
		strvec_push(&argv, "--all");
	nếu (flags & TRANSPORT_PUSH_MIRROR)
		strvec_push(&argv, "--mirror");

	nếu (flags & TRANSPORT_PUSH_CERT_ALWAYS)
		strvec_push(&argv, "--signed=yes");
	không_thì nếu (flags & TRANSPORT_PUSH_CERT_IF_ASKED)
		strvec_push(&argv, "--signed=if-asked");
	nếu (!thin)
		strvec_push(&argv, "--no-thin");

	nếu (deleterefs)
		strvec_push(&argv, "--delete");

	nếu (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	nếu (verbosity >= 2)
		strvec_push(&argv, "-v");
	nếu (verbosity >= 1)
		strvec_push(&argv, "-v");
	không_thì nếu (verbosity < 0)
		strvec_push(&argv, "-q");
	nếu (progress > 0)
		strvec_push(&argv, "--progress");
	không_thì nếu (progress == 0)
		strvec_push(&argv, "--no-progress");

	nếu (family == TRANSPORT_FAMILY_IPV4)
		strvec_push(&argv, "--ipv4");
	không_thì nếu (family == TRANSPORT_FAMILY_IPV6)
		strvec_push(&argv, "--ipv6");

	nếu (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_push(&argv, "--recurse-submodules=check");
	không_thì nếu (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_push(&argv, "--recurse-submodules=on-demand");
	không_thì nếu (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_push(&argv, "--recurse-submodules=only");
	không_thì nếu (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_push(&argv, "--recurse-submodules=no");


	nếu (tags)
		strvec_push(&argv, "--tags");

	cho (i = 0; i < push_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     push_options->items[i].string);

	cho (i = 0; i < cas.nr; i++) {
		nếu (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} không_thì nếu (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} không_thì {
			strvec_push(&argv, "--force-with-lease");
		}
	}

	cho (i = 0; i < list->nr; i++) {
		const char *name = list->items[i].string;
		struct child_process cmd = CHILD_PROCESS_INIT;
		số_nguyên j;

		strvec_pushv(&cmd.args, argv.v);
		strvec_push(&cmd.args, name);

		cho (j = 0; j < refspec_nr; j++)
			strvec_push(&cmd.args, refspecs[j]);

		nếu (verbosity >= 0)
			printf(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		nếu (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			result = 1;
		}
	}

	strvec_clear(&argv);
	trả_về result;
}

static void die_if_repo_looks_like_ref(const char *repo)
{
	const char *slash = strchr(repo, '/');
	struct strbuf name = STRBUF_INIT;
	số_nguyên code;

	nếu (!slash || !slash[1] || file_exists(repo))
		trả_về;

	strbuf_add(&name, repo, slash - repo);
	nếu (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		trả_về;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_PUSH_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	exit(code);
}

số_nguyên cmd_push(số_nguyên argc,
	     const char **argv,
	     const char *prefix,
	     struct repository *repository UNUSED)
{
	số_nguyên flags = 0;
	số_nguyên tags = 0;
	số_nguyên push_cert = -1;
	số_nguyên rc = 0;
	số_nguyên base_flags;
	const char *repo = NULL;	/* default repository */
	struct string_list push_options_cmdline = STRING_LIST_INIT_DUP;
	struct string_list remote_group = STRING_LIST_INIT_DUP;
	struct string_list *push_options;
	const struct string_list_item *item;

	struct option options[] = {
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
	nếu (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	nếu (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	không_thì nếu (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	không_thì nếu (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	nếu (argc > 0)
		repo = argv[0];

	nếu (repo) {
		nếu (!add_remote_or_group(repo, &remote_group)) {
			struct remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			nếu (advice_enabled(ADVICE_PUSH_REPO_LOOKS_LIKE_REF))
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
			nếu (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_group, r->name);
		}
	} không_thì {
		struct remote *r = pushremote_get(NULL);
		nếu (!r)
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

	nếu (!is_empty_cas(&cas) && (flags & TRANSPORT_PUSH_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, push_options)
		nếu (strchr(item->string, '\n'))
			die(_("push options must not have new line characters"));

	nếu (remote_group.nr == 1) {
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
			số_nguyên inner_flags = base_flags;
			struct remote *r = pushremote_get(remote_group.items[0].string);
			nếu (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.items[0].string);

			nếu (r->mirror)
				inner_flags |= (TRANSPORT_PUSH_MIRROR|TRANSPORT_PUSH_FORCE);

			nếu (inner_flags & TRANSPORT_PUSH_ALL) {
				nếu (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			nếu (inner_flags & TRANSPORT_PUSH_MIRROR) {
				nếu (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (struct refspec) REFSPEC_INIT_PUSH(the_hash_algo);

			nếu (tags)
				refspec_append(&rs, "refs/tags/*");
			nếu (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_push(inner_flags, push_options, r);
		}
	} không_thì {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		nếu (flags & TRANSPORT_PUSH_ATOMIC)
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

	nếu (rc == -1)
		usage_with_options(push_usage, options);
	không_thì
		trả_về rc;
}
